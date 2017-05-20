/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    process.c

Abstract:

    This module implements Win32 Thread Object APIs

Author:

    Skulltrail 18-March-2017

Revision History:

--*/

#include <main.h>

UNICODE_STRING NoDefaultCurrentDirectoryInExePath = RTL_CONSTANT_STRING(L"NoDefaultCurrentDirectoryInExePath");

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

BOOL
WINAPI
BasepIsCurDirAllowedForPlainExeNames(VOID)
{
    NTSTATUS Status;
    UNICODE_STRING EmptyString;

    RtlInitEmptyUnicodeString(&EmptyString, NULL, 0);
    Status = RtlQueryEnvironmentVariable_U(NULL,
                                           &NoDefaultCurrentDirectoryInExePath,
                                           &EmptyString);
    return !NT_SUCCESS(Status) && Status != STATUS_BUFFER_TOO_SMALL;
}

/*
 * @implemented
 */
BOOL
WINAPI
NeedCurrentDirectoryForExePathW(IN LPCWSTR ExeName)
{
    if (wcschr(ExeName, L'\\')) return TRUE;

    return BasepIsCurDirAllowedForPlainExeNames();
}

/*
 * @implemented
 */
BOOL
WINAPI
NeedCurrentDirectoryForExePathA(IN LPCSTR ExeName)
{
    if (strchr(ExeName, '\\')) return TRUE;

    return BasepIsCurDirAllowedForPlainExeNames();
}

BOOL
WINAPI
SetEnvironmentStringsA(
    LPSTR NewEnvironment
)
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

#if defined(_M_AMD64)
    ASSERT(dwMinimumWorkingSetSize != 0xffffffff && dwMaximumWorkingSetSize != 0xffffffff);
#endif

    if (dwMinimumWorkingSetSize == 0 || dwMaximumWorkingSetSize == 0) {
        Status = STATUS_INVALID_PARAMETER;
        rv = FALSE;
    } else {

        QuotaLimits.MaximumWorkingSetSize = dwMaximumWorkingSetSize;
        QuotaLimits.MinimumWorkingSetSize = dwMinimumWorkingSetSize;
        QuotaLimits.Flags = Flags;

        PrivStatus = STATUS_SUCCESS;

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

/*
 * @implemented
 */
BOOL
WINAPI
GetProcessWorkingSetSizeEx(
	IN HANDLE hProcess,
	OUT PSIZE_T lpMinimumWorkingSetSize,
	OUT PSIZE_T lpMaximumWorkingSetSize,
	OUT PDWORD Flags
)
{
    QUOTA_LIMITS_EX QuotaLimits;
    NTSTATUS Status;

    /* Query the kernel about this */
    Status = NtQueryInformationProcess(hProcess,
                                       ProcessQuotaLimits,
                                       &QuotaLimits,
                                       sizeof(QuotaLimits),
                                       NULL);
    if (!NT_SUCCESS(Status))
    {
        /* Return error */
        BaseSetLastNTError(Status);
        return FALSE;
    }

    /* Copy the quota information out */
    *lpMinimumWorkingSetSize = QuotaLimits.MinimumWorkingSetSize;
    *lpMaximumWorkingSetSize = QuotaLimits.MaximumWorkingSetSize;
    *Flags = QuotaLimits.Flags;
    return TRUE;
}

/*
 * @implemented
 */
BOOL
WINAPI
GetLogicalProcessorInformation(
	OUT PSYSTEM_LOGICAL_PROCESSOR_INFORMATION Buffer,
    IN OUT PDWORD ReturnLength
)
{
    NTSTATUS Status;

    if (!ReturnLength)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    Status = NtQuerySystemInformation(SystemLogicalProcessorInformation,
                                      Buffer,
                                      *ReturnLength,
                                      ReturnLength);

    /* Normalize the error to what Win32 expects */
    if (Status == STATUS_INFO_LENGTH_MISMATCH) Status = STATUS_BUFFER_TOO_SMALL;
    if (!NT_SUCCESS(Status))
    {
        BaseSetLastNTError(Status);
        return FALSE;
    }

    return TRUE;
}

 /**********************************************************************
*           FlushProcessWriteBuffers     (KERNEL32.@)
*/
VOID 
WINAPI 
FlushProcessWriteBuffers(void)
{
    static int once = 0;
 
    if (!once++)
         DbgPrint("FlushProcessWriteBuffers is stub\n");
}

BOOL 
WINAPI 
GetLogicalProcessorInformationEx(
  _In_       LOGICAL_PROCESSOR_RELATIONSHIP RelationshipType,
  _Out_opt_  PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX Buffer,
  _Inout_    PDWORD ReturnedLength
)
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION information = NULL;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
	DWORD byteOffset = 0;
	SYSTEM_INFO sysinfo;
	BOOLEAN result;
	
	Buffer->Size = sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
	
	result = GetLogicalProcessorInformation(information, ReturnedLength);
	
	if(!result){
		return FALSE;
	}
	Buffer->Relationship = information->Relationship;
	
	//We don't support really groups, so, we emulate to support ALL_PROCESSOR_GROUPS
	while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= *ReturnedLength) {
		switch(Buffer->Relationship){
			case RelationNumaNode:
				Buffer->NumaNode.NodeNumber = information->NumaNode.NodeNumber;
				Buffer->NumaNode.GroupMask.Group = ALL_PROCESSOR_GROUPS;
				break;
			case RelationProcessorCore:
				Buffer->Processor.Flags = information->ProcessorCore.Flags;
				Buffer->Processor.GroupCount = 1;
				Buffer->Processor.GroupMask[0].Group = ALL_PROCESSOR_GROUPS;
				break;
			case RelationProcessorPackage:
				Buffer->Processor.Flags = 0;
				Buffer->Processor.GroupCount = 1;
				Buffer->Processor.GroupMask[0].Group = ALL_PROCESSOR_GROUPS;			
				break;
			case RelationCache:
				Buffer->Cache.Level = information->Cache.Level;
				Buffer->Cache.Associativity = information->Cache.Associativity;
				Buffer->Cache.LineSize = information->Cache.LineSize;
				Buffer->Cache.CacheSize = information->Cache.Size;
				Buffer->Cache.Type = information->Cache.Type;
				Buffer->Cache.GroupMask.Group = ALL_PROCESSOR_GROUPS;				
				break;
			case RelationGroup:
				GetSystemInfo( &sysinfo );
				Buffer->Group.MaximumGroupCount = 1;
				Buffer->Group.ActiveGroupCount = 1;
				Buffer->Group.GroupInfo[0].MaximumProcessorCount = 64;
				Buffer->Group.GroupInfo[0].ActiveProcessorCount = sysinfo.dwNumberOfProcessors;
				break;
		}
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
BasepIsImageVersionOk(
    IN ULONG ImageMajorVersion,
    IN ULONG ImageMinorVersion
    )
{
	return TRUE;
}

/*
 * @unimplemented
*/
HRESULT
WINAPI
RegisterApplicationRecoveryCallback(
	IN APPLICATION_RECOVERY_CALLBACK pRecoveyCallback,
    IN PVOID pvParameter  OPTIONAL,
    DWORD dwPingInterval,
    DWORD dwFlags
)
{
    UNIMPLEMENTED;
    return S_OK;
}

/*
 * @unimplemented
 */
HRESULT
WINAPI
RegisterApplicationRestart(
	IN PCWSTR pwzCommandline  OPTIONAL,
    IN DWORD dwFlags
)
{
    UNIMPLEMENTED;
    return S_OK;
}