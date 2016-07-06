
 /*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */
 
#define NDEBUG

#include <main.h>
#include <malloc.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

ULONG g_ShimsEnabled;

static VOID
CopyStreamData(IN OUT PFIND_STREAM_DATA FindStreamData,
               OUT PWIN32_FIND_STREAM_DATA lpFindStreamData)
{
    _SEH2_TRY
    {
        ASSERT(FindStreamData->CurrentInfo);

        switch (FindStreamData->InfoLevel)
        {
            case FindStreamInfoStandard:
            {
                ULONG StreamNameLen = min(FindStreamData->CurrentInfo->StreamNameLength,
                                          sizeof(lpFindStreamData->cStreamName) - sizeof(WCHAR));

                RtlZeroMemory(lpFindStreamData, sizeof(*lpFindStreamData));

                lpFindStreamData->StreamSize.QuadPart = FindStreamData->CurrentInfo->StreamSize.QuadPart;
                RtlCopyMemory(lpFindStreamData->cStreamName,
                              FindStreamData->CurrentInfo->StreamName,
                              StreamNameLen);
                lpFindStreamData->cStreamName[StreamNameLen / sizeof(WCHAR)] = UNICODE_NULL;

                break;
            }

            default:
            {
                /* Invalid InfoLevel */
                ASSERT(FALSE);
                break;
            }
        }
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
    }
    _SEH2_END;

    return;
}

/*
 * @implemented
 */
DWORD
WINAPI
GetThreadId(IN HANDLE Thread)
{
    THREAD_BASIC_INFORMATION ThreadBasic;
    NTSTATUS Status;

    Status = NtQueryInformationThread(Thread,
                                      ThreadBasicInformation,
                                      &ThreadBasic,
                                      sizeof(THREAD_BASIC_INFORMATION),
                                      NULL);
    if (!NT_SUCCESS(Status))
    {
        BaseSetLastNTError(Status);
        return 0;
    }

    return HandleToUlong(ThreadBasic.ClientId.UniqueThread);
}

/*
 * @implemented
 */
HANDLE
WINAPI
FindFirstStreamW(IN LPCWSTR lpFileName,
                 IN STREAM_INFO_LEVELS InfoLevel,
                 OUT LPVOID lpFindStreamData,
                 IN DWORD dwFlags)
{
    PFIND_DATA_HANDLE FindDataHandle = NULL;
    PFIND_STREAM_DATA FindStreamData;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    UNICODE_STRING NtFilePath;
    HANDLE FileHandle = NULL;
    NTSTATUS Status;
    ULONG BufferSize = 0;

    if (dwFlags != 0 || InfoLevel != FindStreamInfoStandard ||
        lpFindStreamData == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return INVALID_HANDLE_VALUE;
    }

    /* Validate and translate the filename */
    if (!RtlDosPathNameToNtPathName_U(lpFileName,
                                      &NtFilePath,
                                      NULL, NULL))
    {
        SetLastError(ERROR_PATH_NOT_FOUND);
        return INVALID_HANDLE_VALUE;
    }

    /* Open the file */
    InitializeObjectAttributes(&ObjectAttributes,
                               &NtFilePath,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);

    Status = NtCreateFile(&FileHandle,
                          0,
                          &ObjectAttributes,
                          &IoStatusBlock,
                          NULL, 0,
                          FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
                          FILE_OPEN,
                          0, NULL, 0);
    if (!NT_SUCCESS(Status)) goto Cleanup;

    /*
     * Initialization of the search handle.
     */
    FindDataHandle = RtlAllocateHeap(RtlGetProcessHeap(),
                                     HEAP_ZERO_MEMORY,
                                     sizeof(FIND_DATA_HANDLE) +
                                         sizeof(FIND_STREAM_DATA));
    if (!FindDataHandle)
    {
        Status = STATUS_NO_MEMORY;
        goto Cleanup;
    }

    FindDataHandle->Type = FindStream;
    FindDataHandle->u.FindStreamData = (PFIND_STREAM_DATA)(FindDataHandle + 1);
    FindStreamData = FindDataHandle->u.FindStreamData;

    FindStreamData->InfoLevel = InfoLevel;
    FindStreamData->FileStreamInfo = NULL;
    FindStreamData->CurrentInfo = NULL;

    /* The critical section must always be initialized */
    Status = RtlInitializeCriticalSection(&FindDataHandle->Lock);
    if (!NT_SUCCESS(Status))
    {
        RtlFreeHeap(RtlGetProcessHeap(), 0, FindDataHandle);
        goto Cleanup;
    }

    /* Capture all information about the streams */
    do
    {
        BufferSize += 0x1000;

        if (FindStreamData->FileStreamInfo == NULL)
        {
            FindStreamData->FileStreamInfo = RtlAllocateHeap(RtlGetProcessHeap(),
                                                              HEAP_ZERO_MEMORY,
                                                              BufferSize);
            if (FindStreamData->FileStreamInfo == NULL)
            {
                Status = STATUS_NO_MEMORY;
                break;
            }
        }
        else
        {
            PFILE_STREAM_INFORMATION pfsi;

            pfsi = RtlReAllocateHeap(RtlGetProcessHeap(),
                                     0, // HEAP_ZERO_MEMORY,
                                     FindStreamData->FileStreamInfo,
                                     BufferSize);
            if (pfsi == NULL)
            {
                Status = STATUS_NO_MEMORY;
                break;
            }

            FindStreamData->FileStreamInfo = pfsi;
        }

        Status = NtQueryInformationFile(FileHandle,
                                        &IoStatusBlock,
                                        FindStreamData->FileStreamInfo,
                                        BufferSize,
                                        FileStreamInformation);

    } while (Status == STATUS_BUFFER_TOO_SMALL);

    if (NT_SUCCESS(Status))
    {
        /* Select the first stream and return the information */
        FindStreamData->CurrentInfo = FindStreamData->FileStreamInfo;
        CopyStreamData(FindStreamData, lpFindStreamData);

        /* All done */
        Status = STATUS_SUCCESS;
    }
    else
    {
        if (FindStreamData->FileStreamInfo)
        {
            RtlFreeHeap(RtlGetProcessHeap(), 0, FindStreamData->FileStreamInfo);
        }

        RtlFreeHeap(RtlGetProcessHeap(), 0, FindDataHandle);
    }

Cleanup:
    if (FileHandle) NtClose(FileHandle);

    RtlFreeHeap(RtlGetProcessHeap(), 0, NtFilePath.Buffer);

    if (NT_SUCCESS(Status))
    {
        return (HANDLE)FindDataHandle;
    }
    else
    {
        BaseSetLastNTError(Status);
        return INVALID_HANDLE_VALUE;
    }
}
/*
UINT WINAPI GetSystemFirmwareTable( 	
		IN DWORD  	FirmwareTableProviderSignature,
		IN DWORD  	FirmwareTableID,
		OUT PVOID  	pFirmwareTableBuffer,
		IN DWORD  	BufferSize 
	) 		
{
    //STUB;
    return 0;
}*/

BOOLEAN
WINAPI
BaseIsAppcompatInfrastructureDisabled(VOID)
{
    HANDLE KeyHandle;
    NTSTATUS Status;
    KEY_VALUE_PARTIAL_INFORMATION KeyInfo;
    ULONG ResultLength;
    UNICODE_STRING OptionKey = RTL_CONSTANT_STRING(L"\\Registry\\MACHINE\\System\\CurrentControlSet\\Control\\SafeBoot\\Option");
    UNICODE_STRING AppCompatKey = RTL_CONSTANT_STRING(L"\\Registry\\MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\AppCompatibility");
    UNICODE_STRING PolicyKey = RTL_CONSTANT_STRING(L"\\Registry\\MACHINE\\Software\\Policies\\Microsoft\\Windows\\AppCompat");
    UNICODE_STRING OptionValue = RTL_CONSTANT_STRING(L"OptionValue");
    UNICODE_STRING DisableAppCompat = RTL_CONSTANT_STRING(L"DisableAppCompat");
    UNICODE_STRING DisableEngine = RTL_CONSTANT_STRING(L"DisableEngine");
    OBJECT_ATTRIBUTES OptionKeyAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(&OptionKey, OBJ_CASE_INSENSITIVE);
    OBJECT_ATTRIBUTES AppCompatKeyAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(&AppCompatKey, OBJ_CASE_INSENSITIVE);
    OBJECT_ATTRIBUTES PolicyKeyAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(&PolicyKey, OBJ_CASE_INSENSITIVE);

    /*
     * This is a TROOLEAN, -1 means we haven't yet figured it out.
     * 0 means shims are enabled, and 1 means shims are disabled!
     */
    if (g_ShimsEnabled == -1)
    {
        /* Open the safe mode key */
        Status = NtOpenKey(&KeyHandle, 1, &OptionKeyAttributes);
        if (NT_SUCCESS(Status))
        {
            /* Check if this is safemode */
            Status = NtQueryValueKey(KeyHandle,
                                     &OptionValue,
                                     KeyValuePartialInformation,
                                     &KeyInfo,
                                     sizeof(KeyInfo),
                                     &ResultLength);
            NtClose(KeyHandle);
            if ((NT_SUCCESS(Status)) &&
                 (KeyInfo.Type == REG_DWORD) &&
                 (KeyInfo.DataLength == sizeof(ULONG)) &&
                 (KeyInfo.Data[0] == TRUE))
            {
                /* It is, so disable shims! */
                g_ShimsEnabled = TRUE;
            }
            else
            {
                /* Open the app compatibility engine settings key */
                Status = NtOpenKey(&KeyHandle, 1, &AppCompatKeyAttributes);
                if (NT_SUCCESS(Status))
                {
                    /* Check if the app compat engine is turned off */
                    Status = NtQueryValueKey(KeyHandle,
                                             &DisableAppCompat,
                                             KeyValuePartialInformation,
                                             &KeyInfo,
                                             sizeof(KeyInfo),
                                             &ResultLength);
                    NtClose(KeyHandle);
                    if ((NT_SUCCESS(Status)) &&
                        (KeyInfo.Type == REG_DWORD) &&
                        (KeyInfo.DataLength == sizeof(ULONG)) &&
                        (KeyInfo.Data[0] == TRUE))
                    {
                        /* It is, so disable shims! */
                        g_ShimsEnabled = TRUE;
                    }
                    else
                    {
                        /* Finally, open the app compatibility policy key */
                        Status = NtOpenKey(&KeyHandle, 1, &PolicyKeyAttributes);
                        if (NT_SUCCESS(Status))
                        {
                            /* Check if the system policy disables app compat */
                            Status = NtQueryValueKey(KeyHandle,
                                                     &DisableEngine,
                                                     KeyValuePartialInformation,
                                                     &KeyInfo,
                                                     sizeof(KeyInfo),
                                                     &ResultLength),
                                                     NtClose(KeyHandle);
                            if ((NT_SUCCESS(Status)) &&
                                (KeyInfo.Type == REG_DWORD) &&
                                (KeyInfo.DataLength == sizeof(ULONG)) &&
                                (KeyInfo.Data[0] == TRUE))
                            {
                                /* It does, so disable shims! */
                                g_ShimsEnabled = TRUE;
                            }
                            else
                            {
                                /* No keys are set, so enable shims! */
                                g_ShimsEnabled = FALSE;
                            }
                        }
                    }
                }
            }
        }
    }

    /* Return if shims are disabled or not ("Enabled == 1" means disabled!) */
    return g_ShimsEnabled ? TRUE : FALSE;
}

BOOL 
WINAPI 
SetThreadStackGuarantee(
	PULONG StackSizeInBytes
)
{
  DWORD dwPageSize; // esi@1 MAPDST
  ULONG GuaranteedStackBytes; // ebx@1
  PVOID Allocation; // esp@1 MAPDST
  int Size; // eax@3
  PVOID DeallocationStack; // edi@7
  NTSTATUS status; // eax@10
  struct _SYSTEM_INFO SystemInfo; // [sp+Ch] [bp-30h]@1
  PVOID BaseAddress; // [sp+34h] [bp-8h]@9
  ULONG AllocationSize; // [sp+38h] [bp-4h]@1 MAPDST

  GetSystemInfo(&SystemInfo);
  dwPageSize = SystemInfo.dwPageSize;
  AllocationSize = *StackSizeInBytes;
  GuaranteedStackBytes = NtCurrentTeb()->GuaranteedStackBytes;
  *StackSizeInBytes = GuaranteedStackBytes;
  Allocation = alloca(4);
  if ( AllocationSize && AllocationSize >= GuaranteedStackBytes )
  {
    Size = ~(dwPageSize - 1) & (2 * dwPageSize + AllocationSize - 1);
    if ( Size < AllocationSize )
      goto Error;
    AllocationSize = ~(dwPageSize - 1) & (2 * dwPageSize + AllocationSize - 1);
    DeallocationStack = NtCurrentTeb()->DeallocationStack;
    if ( Size < 2 * dwPageSize )
      AllocationSize = 2 * dwPageSize;
    BaseAddress = (PVOID)(((ULONG)&Allocation & ~(dwPageSize - 1)) - AllocationSize);
    if ( BaseAddress < (char *)DeallocationStack + dwPageSize )
    {
Error:
      SetLastError(0x57u);
      return FALSE;
    }
    status = NtAllocateVirtualMemory((HANDLE)0xFFFFFFFF, &BaseAddress, 0, &AllocationSize, 0x1000, 0x104);
    if ( status < STATUS_SUCCESS && status != STATUS_ALREADY_COMMITTED )
    {
      BaseSetLastNTError(status);
      return FALSE;
    }
    NtCurrentTeb()->GuaranteedStackBytes = AllocationSize - dwPageSize;
  }
  return TRUE;
}