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

VOID WINAPI DeleteProcThreadAttributeList(
  _Inout_  LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList
)
{
	;
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

/*
 * @implemented - need test
 */
BOOL WINAPI InitializeProcThreadAttributeList(LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, DWORD dwAttributeCount, DWORD dwFlags, PSIZE_T lpSize)
{
  SIZE_T size; // esi@3
  BOOL resp; // edi@5
  BOOL result; // eax@6
  NTSTATUS error; // [sp-4h] [bp-4h]@9

  if ( dwFlags )
  {
    error = 0xC00000F1u;
LABEL_10:
    BaseSetLastNTError(error);
    return 0;
  }
  if ( dwAttributeCount > 3 )
  {
    error = 0xC00000F0u;
    goto LABEL_10;
  }
  size = 12 * dwAttributeCount + 20;
  if ( lpAttributeList && *lpSize >= size )
  {
    *(DWORD *)lpAttributeList = 0;
    *((DWORD *)lpAttributeList + 4) = 0;
    *((DWORD *)lpAttributeList + 1) = dwAttributeCount;
    *((DWORD *)lpAttributeList + 2) = 0;
    resp = 1;
  }
  else
  {
    resp = 0;
    SetLastError(0x7Au);
  }
  result = resp;
  *lpSize = size;
  return result;
}

BOOL WINAPI QueryProcessCycleTime(HANDLE ProcessHandle, PULONG64 CycleTime)
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

BOOL WINAPI QueryProcessAffinityUpdateMode(HANDLE ProcessInformation, DWORD lpdwFlags)
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

BOOL WINAPI SetProcessAffinityUpdateMode(HANDLE ProcessHandle, DWORD ProcessInformation)
{
  NTSTATUS status; // eax@4
  NTSTATUS otherStatus; // [sp-4h] [bp-4h]@2

  if ( ProcessInformation & 0xFFFFFFFE )
  {
    otherStatus = 0xC00000F1u;
LABEL_3:
    BaseSetLastNTError(otherStatus);
    return 0;
  }
  ProcessInformation = ((ProcessInformation & 1) == 0) + 1;
  status = NtSetInformationProcess(ProcessHandle, ProcessIoPortHandlers|0x20, &ProcessInformation, 4u);
  if ( status < 0 )
  {
    otherStatus = status;
    goto LABEL_3;
  }
  return 1;
}

BOOL WINAPI UpdateProcThreadAttribute(LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, DWORD dwFlags, DWORD_PTR Attribute, PVOID lpValue, SIZE_T cbSize, PVOID lpPreviousValue, PSIZE_T lpReturnSize)
{
  DWORD_PTR localAttributes; // ebx@2
  BOOL hum; // edi@2
  LPCSTR string; // edx@5
  PVOID v10; // eax@5
  int uneeded; // esi@9
  int otherUnneeded; // esi@10
  BOOL otherLogical; // zf@24
  NTSTATUS receiveStatus; // eax@33
  NTSTATUS otherStatus; // [sp-10h] [bp-10h]@32
  NTSTATUS status; // [sp-Ch] [bp-Ch]@23

  if ( dwFlags & 0xFFFFFFFE )
  {
    BaseSetLastNTError(0xC00000F0u);
    return 0;
  }
  localAttributes = Attribute;
  hum = 1 << Attribute;
  if ( !(localAttributes & 0x40000) )
  {
    if ( *((DWORD *)lpAttributeList + 2) == *((DWORD *)lpAttributeList + 1) )
    {
      status = 0xC0000001u;
    }
    else
    {
      if ( hum & *(DWORD *)lpAttributeList )
      {
        status = 0x40000000u;
      }
      else
      {
        if ( lpPreviousValue )
        {
          status = 0xC00000F4u;
        }
        else
        {
          if ( !(dwFlags & 1) )
            goto LABEL_3;
          status = 0xC00000F0u;
        }
      }
    }
    BaseSetLastNTError(status);
    return 0;
  }
LABEL_3:
  if ( Attribute & 0x20000 && lpReturnSize )
  {
    otherStatus = -1073741579;
LABEL_42:
    BaseSetLastNTError(otherStatus);
    return 0;
  }
  string = (char *)lpAttributeList + 12 * *((DWORD *)lpAttributeList + 2) + 20;
  v10 = lpValue;
  if ( Attribute == 131072 )
  {
    otherLogical = cbSize == 4;
  }
  else
  {
    if ( Attribute != 131074 )
    {
      if ( Attribute != 393217 )
      {
        receiveStatus = 0xC00000BBu;
LABEL_41:
        otherStatus = receiveStatus;
        goto LABEL_42;
      }
      if ( cbSize == 4 )
      {
        uneeded = *((DWORD *)lpAttributeList + 4);
        if ( uneeded )
        {
          string = (LPCSTR)*((DWORD *)lpAttributeList + 4);
          otherUnneeded = *(DWORD *)(uneeded + 8);
          hum = 0;
        }
        else
        {
          lpAttributeList= (LPPROC_THREAD_ATTRIBUTE_LIST)string;
          otherUnneeded = 0;
        }
        v10 = *(PVOID *)lpValue;
        if ( !(*(DWORD *)lpValue & 0xFFFFFFFC) )
        {
          if ( !(dwFlags & 1) && otherUnneeded )
            v10 = (PVOID)(otherUnneeded | (unsigned int)v10);
          if ( lpPreviousValue )
            *(DWORD *)lpPreviousValue = otherUnneeded;
        }
        receiveStatus = 0xC000000Du;
        goto LABEL_41;
      }
LABEL_40:
      receiveStatus = 0xC0000004u;
      goto LABEL_41;
    }
    if ( !cbSize )
      goto LABEL_40;
    otherLogical = (cbSize & 3) == 0;
  }
  if ( !otherLogical )
    goto LABEL_40;
  if ( hum )
  {
    *(DWORD *)string = Attribute;
    *((DWORD *)string + 1) = cbSize;
    ++*((DWORD *)lpAttributeList + 2);
    *(DWORD *)lpAttributeList |= hum;
  }
  return 1;
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

/*++

Routine Description:

    This function allows the caller to determine the minimum and maximum working
    set sizes of the specified process. The working set sizes effect the virtual
    memory paging behavior for the process.

Arguments:

    hProcess - Supplies an open handle to the specified process.  The
        handle must have been created with PROCESS_QUERY_INFORMATION
        access.

    lpMinimumWorkingSetSize - Supplies the address of the variable that
        will receive the minimum working set size of the specified
        process.  The virtual memory manager will attempt to keep at
        least this much memory resident in the process whenever the
        process is active.


    lpMaximumWorkingSetSize - Supplies the address of the variable that
        will receive the maximum working set size of the specified
        process.  In tight memory situations, the virtual memory manager
        will attempt to keep at no more than this much memory resident
        in the process whenever the process is active.

    Flags - Output flags, QUOTA_LIMITS_HARDWS_ENABLE enables hard WS
                          QUOTA_LIMITS_HARDWS_DISABLE disabled hard WS

Return Value:

    TRUE - The API was successful

    FALSE - The operation failed.  Extended error status is available
        using GetLastError.

--*/

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

/*++

Routine Description:

    This function determines whether or not the current directory
    should be used as part of the process of locating an executable
    whose name contains no directory components.

Arguments:

    None.

Return Value:

    TRUE - The current directory should be a part of the path used to
           search for executables whose names contain no directory
           components.

    FALSE - The current directory should NOT be a part of the path
            used to search for executables whose names contain no
            directory components.

--*/

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

/*++

Routine Description:

    This function allows the caller to set the minimum and maximum
    working set sizes of the specified process.  The working set sizes
    effect the virtual memory paging behavior for the process.  The
    specified process's working set be emptied (essentially swapping out
    the process) by specifying the distinguished values 0xffffffff for
    both the minimum and maximum working set sizes.

    If you are not trimming an address space, SE_INC_BASE_PRIORITY_PRIVILEGE
    must be held by the process

Arguments:

    hProcess - Supplies an open handle to the specified process.  The
        handle must have been created with PROCESS_SET_QUOTA
        access.

    dwMinimumWorkingSetSize - Supplies the minimum working set size for
        the specified process.  The virtual memory manager will attempt
        to keep at least this much memory resident in the process
        whenever the process is active.  A value of (SIZE_T)-1 and the
        same value in dwMaximumWorkingSetSize will temporarily trim the
        working set of the specified process (essentially out swap the
        process).


    dwMaximumWorkingSetSize - Supplies the maximum working set size for
        the specified process.  In tight memory situations, the virtual
        memory manager will attempt to keep at no more than this much
        memory resident in the process whenever the process is active.
        A value of (SIZE_T)-1 and the same value in
        dwMinimumWorkingSetSize will temporarily trim the working set of
        the specified process (essentially out swap the process).

    Flags - Supplied flags, QUOTA_LIMITS_HARDWS_ENABLE enables hard WS
                            QUOTA_LIMITS_HARDWS_DISABLE disabled hard ws

Return Value:

    TRUE - The API was successful

    FALSE - The operation failed.  Extended error status is available
        using GetLastError.

--*/

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
