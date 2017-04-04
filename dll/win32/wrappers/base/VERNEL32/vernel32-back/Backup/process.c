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

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);
/*
 * @implemented clean
 */
BOOL
WINAPI
QueryFullProcessImageNameW(HANDLE hProcess,
                           DWORD dwFlags,
                           LPWSTR lpExeName,
                           PDWORD pdwSize)
{
    BYTE Buffer[sizeof(UNICODE_STRING) + MAX_PATH * sizeof(WCHAR)];
    UNICODE_STRING *DynamicBuffer = NULL;
    UNICODE_STRING *Result = NULL;
    NTSTATUS Status;
    DWORD Needed;

    Status = NtQueryInformationProcess(hProcess,
                                       ProcessImageFileName,
                                       Buffer,
                                       sizeof(Buffer) - sizeof(WCHAR),
                                       &Needed);
    if (Status == STATUS_INFO_LENGTH_MISMATCH)
    {
        DynamicBuffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, Needed + sizeof(WCHAR));
        if (!DynamicBuffer)
        {
            SetLastError(STATUS_NO_MEMORY);
            return FALSE;
        }
        Status = NtQueryInformationProcess(hProcess,
                                           ProcessImageFileName,
                                           (LPBYTE)DynamicBuffer,
                                           Needed,
                                           &Needed);
        Result = DynamicBuffer;
    }
    else Result = (PUNICODE_STRING)Buffer;

    if (!NT_SUCCESS(Status)) goto Cleanup;

    if (Result->Length / sizeof(WCHAR) + 1 > *pdwSize)
    {
        Status = STATUS_BUFFER_TOO_SMALL;
        goto Cleanup;
    }

    *pdwSize = Result->Length / sizeof(WCHAR);
    memcpy(lpExeName, Result->Buffer, Result->Length);
    lpExeName[*pdwSize] = 0;

Cleanup:
    RtlFreeHeap(RtlGetProcessHeap(), 0, DynamicBuffer);

    if (!NT_SUCCESS(Status))
    {
        SetLastError(Status);
    }

    return !Status;
}


/*
 * @implemented
 */
BOOL
WINAPI
QueryFullProcessImageNameA(HANDLE hProcess,
                           DWORD dwFlags,
                           LPSTR lpExeName,
                           PDWORD pdwSize)
{
    DWORD pdwSizeW = *pdwSize;
    BOOL Result;
    LPWSTR lpExeNameW;

    lpExeNameW = RtlAllocateHeap(RtlGetProcessHeap(),
                                 HEAP_ZERO_MEMORY,
                                 *pdwSize * sizeof(WCHAR));
    if (!lpExeNameW)
    {
        SetLastError(STATUS_NO_MEMORY);
        return FALSE;
    }

    Result = QueryFullProcessImageNameW(hProcess, dwFlags, lpExeNameW, &pdwSizeW);

    if (Result)
        Result = (0 != WideCharToMultiByte(CP_ACP, 0,
                                           lpExeNameW,
                                           -1,
                                           lpExeName,
                                           *pdwSize,
                                           NULL, NULL));

    if (Result)
        *pdwSize = strlen(lpExeName);

    RtlFreeHeap(RtlGetProcessHeap(), 0, lpExeNameW);
    return Result;
}

/**********************************************************************
  *           FlushProcessWriteBuffers     (KERNEL32.@)
  */
VOID WINAPI FlushProcessWriteBuffers(void)
{
     static int once = 0;
 
     if (!once++)
         FIXME(": stub\n");
}

BOOL 
WINAPI 
QueryProcessCycleTime(
	HANDLE ProcessHandle, 
	PULONG64 CycleTime)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  PULONG64 ProcessInformation; // [sp+0h] [bp-10h]@1

  status = NtQueryInformationProcess(ProcessHandle, ProcessRaisePriority|0x20, &ProcessInformation, 0x10u, 0);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  else
  {
    CycleTime = ProcessInformation;
    result = 1;
  }
  return result;
}

BOOL 
WINAPI 
QueryProcessAffinityUpdateMode(
	HANDLE ProcessInformation, 
	DWORD lpdwFlags
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  DWORD flags; // eax@3

  status = NtQueryInformationProcess(ProcessInformation, ProcessIoPortHandlers|0x20, &ProcessInformation, 4u, 0);
  if ( status >= 0 )
  {
    flags = 0;
    if ( (DWORD)ProcessInformation & 1 )
      flags = 1;
    if ( lpdwFlags )
      lpdwFlags = flags;
    result = 1;
  }
  else
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  return result;
}

BOOL 
WINAPI 
SetProcessAffinityUpdateMode(
	HANDLE ProcessHandle, 
	DWORD ProcessInformation
)
{
  NTSTATUS status; // eax@4

  if ( ProcessInformation & 0xFFFFFFFE )
  {
    status = STATUS_INVALID_PARAMETER_3;
LABEL_3:
    BaseSetLastNTError(status);
    return FALSE;
  }
  ProcessInformation = ((ProcessInformation & 1) == 0) + 1;
  status = NtSetInformationProcess(ProcessHandle, ProcessIoPortHandlers|0x20, &ProcessInformation, 4u);
  if ( !NT_SUCCESS(status))
  {
    goto LABEL_3;
  }
  return TRUE;
}

/***********************************************************************
 *           InitializeProcThreadAttributeList       (KERNEL32.@)
 */
BOOL 
WINAPI 
InitializeProcThreadAttributeList(
	LPPROC_THREAD_ATTRIBUTE_LIST list,
    DWORD count, 
	DWORD flags, 
	SIZE_T *size
)
{
    SIZE_T needed;
    BOOL ret = FALSE;

    needed = FIELD_OFFSET(PROC_THREAD_ATTRIBUTE_LIST, attrs[count]);
    if (list && *size >= needed)
    {
        list->mask = 0;
        list->size = count;
        list->count = 0;
        list->unk = 0;
        ret = TRUE;
    }
    else
        SetLastError(ERROR_INSUFFICIENT_BUFFER);

    *size = needed;
    return ret;
}

/***********************************************************************
 *           UpdateProcThreadAttribute       (KERNEL32.@)
 */
BOOL 
WINAPI 
UpdateProcThreadAttribute(
	LPPROC_THREAD_ATTRIBUTE_LIST list,
    DWORD flags, DWORD_PTR attr, 
	void *value, SIZE_T size,
    void *prev_ret, SIZE_T *size_ret
)
{
    DWORD mask;
    struct proc_thread_attr *entry;

    if (list->count >= list->size)
    {
        SetLastError(ERROR_GEN_FAILURE);
        return FALSE;
    }

    switch (attr)
    {
    case PROC_THREAD_ATTRIBUTE_PARENT_PROCESS:
        if (size != sizeof(HANDLE))
        {
            SetLastError(ERROR_BAD_LENGTH);
            return FALSE;
        }
        break;

    case PROC_THREAD_ATTRIBUTE_HANDLE_LIST:
        if ((size / sizeof(HANDLE)) * sizeof(HANDLE) != size)
        {
            SetLastError(ERROR_BAD_LENGTH);
            return FALSE;
        }
        break;

    case PROC_THREAD_ATTRIBUTE_IDEAL_PROCESSOR:
        if (size != sizeof(PROCESSOR_NUMBER))
        {
            SetLastError(ERROR_BAD_LENGTH);
            return FALSE;
        }
        break;

    default:
        SetLastError(ERROR_NOT_SUPPORTED);
        return FALSE;
    }

    mask = 1 << (attr & PROC_THREAD_ATTRIBUTE_NUMBER);

    if (list->mask & mask)
    {
        SetLastError(ERROR_OBJECT_NAME_EXISTS);
        return FALSE;
    }

    list->mask |= mask;

    entry = list->attrs + list->count;
    entry->attr = attr;
    entry->size = size;
    entry->value = value;
    list->count++;

    return TRUE;
}

/***********************************************************************
 *           DeleteProcThreadAttributeList       (KERNEL32.@)
 */
void 
WINAPI 
DeleteProcThreadAttributeList(
	struct _PROC_THREAD_ATTRIBUTE_LIST *list
)
{
    return;
}

BOOL 
WINAPI 
GetProcessUserModeExceptionPolicy(
	LPDWORD lpFlags
)
{
  BaseSetLastNTError(STATUS_NOT_SUPPORTED);
  return FALSE;
}

BOOL
WINAPI
SetProcessUserModeExceptionPolicy(
    __in DWORD dwFlags
)
{
	return TRUE;
}

BOOL 
WINAPI 
GetProcessInformation(
	HANDLE ProcessHandle, 
	PROCESSINFOCLASS  ProcessInformationClass, 
	PVOID ProcessInformation, 
	DWORD ProcessInformationLength
)
{
  BOOL resp; // esi@2
  NTSTATUS status; // eax@3

  if ( ProcessInformationClass )
  {
    BaseSetLastNTError(STATUS_INVALID_PARAMETER);
    resp = FALSE;
  }
  else
  {
    resp = FALSE;
    status = NtQueryInformationProcess(
               ProcessHandle,
               ProcessDebugPort|0x20,
               ProcessInformation,
               ProcessInformationLength,
               0);
    if ( status >= 0 )
      resp = TRUE;
    else
      BaseSetLastNTError(status);
  }
  return resp;
}

BOOL 
WINAPI 
SetProcessInformation(
	HANDLE ProcessHandle, 
	PROCESSINFOCLASS ProcessInformationClass, 
	PVOID ProcessInformation, 
	DWORD ProcessInformationLength
)
{
  NTSTATUS status; // eax@2
  NTSTATUS error; // [sp-4h] [bp-4h]@5

  if ( ProcessInformationClass )
  {
    error = STATUS_INVALID_PARAMETER;
  }
  else
  {
    status = NtSetInformationProcess(ProcessHandle, ProcessDebugPort|0x20, ProcessInformation, ProcessInformationLength);
    if ( status >= 0 )
      return TRUE;
    error = status;
  }
  BaseSetLastNTError(error);
  return FALSE;
}

BOOL 
WINAPI 
GetProcessMitigationPolicy(
  _In_   HANDLE hProcess,
  _In_   PROCESS_MITIGATION_POLICY MitigationPolicy,
  _Out_  PVOID lpBuffer,
  _In_   SIZE_T dwLength
)
{
	UNIMPLEMENTED;
	return TRUE;
}

BOOL 
WINAPI 
SetProcessMitigationPolicy(
  _In_  PROCESS_MITIGATION_POLICY MitigationPolicy,
  _In_  PVOID lpBuffer,
  _In_  SIZE_T dwLength
)
{
	UNIMPLEMENTED;
	return TRUE;
}

void 
CreateProcessInternalWSecure()
{
  ;
}

BOOL
WINAPI
GetProcessWorkingSetSizeEx(
    HANDLE hProcess,
    PSIZE_T lpMinimumWorkingSetSize,
    PSIZE_T lpMaximumWorkingSetSize,
    LPDWORD Flags
)
{
    QUOTA_LIMITS_EX QuotaLimits;
    NTSTATUS Status;
    BOOL rv;

    Status = NtQueryInformationProcess (hProcess,
                                        ProcessQuotaLimits,
                                        &QuotaLimits,
                                        sizeof (QuotaLimits),
                                        NULL);

    if (NT_SUCCESS (Status)) {
        *lpMinimumWorkingSetSize = QuotaLimits.MinimumWorkingSetSize;
        *lpMaximumWorkingSetSize = QuotaLimits.MaximumWorkingSetSize;
        *Flags = QuotaLimits.Flags;
        rv = TRUE;
    } else {
        rv = FALSE;
        BaseSetLastNTError (Status);
    }
    return rv;
}

BOOL
BasepIsCurDirAllowedForPlainExeNames(
    VOID
    )
{
    NTSTATUS Status;

    static const UNICODE_STRING Name =
        RTL_CONSTANT_STRING(L"NoDefaultCurrentDirectoryInExePath");

    UNICODE_STRING Value;

    RtlInitEmptyUnicodeString(&Value, NULL, 0);

    Status = RtlQueryEnvironmentVariable_U(NULL, &Name, &Value);

    if (Status == STATUS_BUFFER_TOO_SMALL || Status == STATUS_SUCCESS) {
        return FALSE;
    } else {
        return TRUE;
    }
}

BOOL
WINAPI
NeedCurrentDirectoryForExePathA(
    IN LPCSTR ExeName
    )

/*++

Routine Description:

    This function determines whether or not the current directory
    should be used as part of the process of locating an executable.

Arguments:

    ExeName - The name of the exe which will be looked for.

Return Value:

    TRUE - The current directory should be a part of the path used to
           search for executables.

    FALSE - The current directory should NOT be a part of the path
            used to search for executables.

--*/

{
    // N.B. Changes here should be reflected in the Unicode version as well.

    if (strchr(ExeName, '\\')) { // if it contains a '\'
        return TRUE;             // allow the current directory
    }

    // Otherwise, either it doesn't matter, or we want to optionally restrict it.

    return BasepIsCurDirAllowedForPlainExeNames();
}

BOOL
WINAPI
NeedCurrentDirectoryForExePathW(
    IN LPCWSTR ExeName
    )

/*++

Routine Description:

    This function determines whether or not the current directory
    should be used as part of the process of locating an executable.

Arguments:

    ExeName - The name of the exe which will be looked for.

Return Value:

    TRUE - The current directory should be a part of the path used to
           search for executables.

    FALSE - The current directory should NOT be a part of the path
            used to search for executables.

--*/

{
    // N.B. Changes here should be reflected in the ANSI version as well.
    
    if (wcschr(ExeName, L'\\')) { // if it contains a '\'
        return TRUE;              // allow the current directory
    }

    // Otherwise, either it doesn't matter, or we want to optionally restrict it.

    return BasepIsCurDirAllowedForPlainExeNames();
}

BOOL
WINAPI
SetEnvironmentStringsA(
    LPSTR NewEnvironment
    )
/*++

Routine Description:

    This function sets the environment block of the current process

Arguments:

    NewEnvironment - Zero terminated strings terminated by an extra termination

Return Value:

    TRUE - The function suceeded, FALSE - The function failed and GetLastError() gives the error

--*/
{
    PSTR           Temp;
    OEM_STRING     Buffer;
    UNICODE_STRING Unicode;
    SIZE_T         Len;
    NTSTATUS       Status;

    Temp = NewEnvironment;
 
    while (1) {
        Len = strlen (Temp);
        if (Len == 0 || strchr (Temp+1, '=') == NULL) {
            BaseSetLastNTError (STATUS_INVALID_PARAMETER);
            return FALSE;
        }
        Temp += Len + 1;
        if (*Temp == '\0') {
            Temp++;
            break;
        }
    }

    //
    // Calculate total size of buffer needed to hold the block
    //

    Len = Temp - NewEnvironment;

    if (Len > UNICODE_STRING_MAX_CHARS) {
        BaseSetLastNTError (STATUS_INVALID_PARAMETER);
        return FALSE;
    }

    Buffer.Length = (USHORT) Len;
    Buffer.Buffer = NewEnvironment;


    Status = RtlOemStringToUnicodeString (&Unicode, &Buffer, TRUE);
    if (!NT_SUCCESS (Status)) {
        BaseSetLastNTError (STATUS_INVALID_PARAMETER);
        return FALSE;
    }
    Status = RtlSetEnvironmentStrings (Unicode.Buffer, Unicode.Length);

    RtlFreeUnicodeString (&Unicode);

    if (!NT_SUCCESS (Status)) {
        BaseSetLastNTError (STATUS_INVALID_PARAMETER);
        return FALSE;
    }
    return TRUE;
}

BOOL
WINAPI
SetEnvironmentStringsW(
    LPWSTR NewEnvironment
    )
/*++

Routine Description:

    This function sets the environment block of the current process

Arguments:

    NewEnvironment - Zero terminated strings terminated by an extra termination

Return Value:

    TRUE - The function suceeded, FALSE - The function failed and GetLastError() gives the error

--*/
{
    SIZE_T   Len;
    PWSTR    Temp, p;
    NTSTATUS Status;

    Temp = NewEnvironment;
 
    while (1) {
        Len = wcslen (Temp);

        //
        // Reject zero length strings
        //
        if (Len == 0) {
            BaseSetLastNTError (STATUS_INVALID_PARAMETER);
            return FALSE;
        }

        //
        // Reject strings without '=' in the name or if the first part of the string is too big.
        //
        p = wcschr (Temp+1, '=');
        if (p == NULL || (p - Temp) > UNICODE_STRING_MAX_CHARS || Len - (p - Temp) - 1 > UNICODE_STRING_MAX_CHARS) {
            BaseSetLastNTError (STATUS_INVALID_PARAMETER);
            return FALSE;
        }
        Temp += Len + 1;
        if (*Temp == L'\0') {
            Temp++;
            break;
        }
    }

    //
    // Calculate total size of buffer needed to hold the block
    //

    Len = (PUCHAR)Temp - (PUCHAR)NewEnvironment;

    Status = RtlSetEnvironmentStrings (NewEnvironment, Len);
    if (!NT_SUCCESS (Status)) {
        BaseSetLastNTError (STATUS_INVALID_PARAMETER);
        return FALSE;
    }
    return TRUE;
}

BOOL
WINAPI
SetProcessWorkingSetSizeEx(
    HANDLE hProcess,
    SIZE_T dwMinimumWorkingSetSize,
    SIZE_T dwMaximumWorkingSetSize,
    ULONG  Flags
    )
{
    QUOTA_LIMITS_EX QuotaLimits={0};
    NTSTATUS Status, PrivStatus;
    BOOL rv;

#ifdef _WIN64
    ASSERT(dwMinimumWorkingSetSize != 0xffffffff && dwMaximumWorkingSetSize != 0xffffffff);
#endif

    if (dwMinimumWorkingSetSize == 0 || dwMaximumWorkingSetSize == 0) {
        Status = STATUS_INVALID_PARAMETER;
        rv = FALSE;
    } else {

        QuotaLimits.MaximumWorkingSetSize = dwMaximumWorkingSetSize;
        QuotaLimits.MinimumWorkingSetSize = dwMinimumWorkingSetSize;
        QuotaLimits.Flags = Flags;

        //
        // Attempt to acquire the appropriate privilege.  If this
        // fails, it's no big deal -- we'll attempt to make the
        // NtSetInformationProcess call anyway, in case it turns out
        // to be a decrease operation (which will succeed anyway).
        //
        PrivStatus = STATUS_SUCCESS;//BasepAcquirePrivilegeEx (SE_INC_BASE_PRIORITY_PRIVILEGE, &State);

        Status = NtSetInformationProcess (hProcess,
                                          ProcessQuotaLimits,
                                          &QuotaLimits,
                                          sizeof(QuotaLimits));
        if (!NT_SUCCESS (Status)) {
            rv = FALSE;
        } else {
            rv = TRUE;
        }

    }

    if (!rv) {
        BaseSetLastNTError (Status);
    }
    return rv;
}

BOOL 
WINAPI 
IsProcessCritical(
	HANDLE ProcessHandle, 
	PBOOL Critical)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  ULONG breakOnTermination;

  status = NtQueryInformationProcess(ProcessHandle, ProcessBreakOnTermination, &breakOnTermination, sizeof(ULONG), NULL);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  else
  {
    if(!breakOnTermination)
		*Critical = TRUE;
	else
		*Critical = FALSE;
	 result = TRUE;
  }
  return result;
}