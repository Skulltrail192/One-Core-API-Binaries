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

DWORD 
WINAPI 
GetProcessImageFileNameW(
  _In_  HANDLE hProcess,
  _Out_ LPWSTR lpImageFileName,
  _In_  DWORD  nSize
);

DWORD 
WINAPI 
GetProcessImageFileNameA(
  _In_  HANDLE hProcess,
  _Out_ LPTSTR lpImageFileName,
  _In_  DWORD  nSize
);


/******************************************************************
 *		QueryFullProcessImageNameW (KERNEL32.@)
 */
BOOL 
WINAPI 
QueryFullProcessImageNameW(
	HANDLE hProcess, 
	DWORD dwFlags, 
	LPWSTR lpExeName, 
	PDWORD pdwSize
)
{
	if(GetProcessImageFileNameW(hProcess, lpExeName, *pdwSize)>0){
		return TRUE;
	}else{
		return FALSE;
	}
}

/*
 * @implemented
 */
BOOL
WINAPI
QueryFullProcessImageNameA(
	HANDLE hProcess,
    DWORD dwFlags,
    LPSTR lpExeName,
    PDWORD pdwSize)
{
	if(GetProcessImageFileNameA(hProcess, lpExeName, *pdwSize)>0){
		return TRUE;
	}else{
		return FALSE;
	}
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
 
//Helper function to count set bits in the processor mask.
DWORD CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR)*8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;    
    DWORD i;
    
    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest)?1:0);
        bitTest/=2;
    }

    return bitSetCount;
}


typedef BOOL (WINAPI *LPFN_GLPI)(
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, 
    PDWORD);

BOOL 
WINAPI 
GetLogicalProcessorInformationEx(
  _In_       LOGICAL_PROCESSOR_RELATIONSHIP RelationshipType,
  _Out_opt_  PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX SystemInformation,
  _Inout_    PDWORD ReturnedLength
)
{
    LPFN_GLPI glpi;
    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
    DWORD returnLength = 0;
    DWORD logicalProcessorCount = 0;
    DWORD numaNodeCount = 0;
    DWORD processorCoreCount = 0;
    DWORD processorL1CacheCount = 0;
    DWORD processorL2CacheCount = 0;
    DWORD processorL3CacheCount = 0;
    DWORD processorPackageCount = 0;
    DWORD byteOffset = 0;
    PCACHE_DESCRIPTOR Cache;
	ULONG CurrentLength = 0;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX Output;
	ULONG Index = 0;
	SYSTEM_INFO sysinfo;
	
	Output = SystemInformation;

    glpi = (LPFN_GLPI) GetProcAddress(
                            GetModuleHandle(TEXT("kernel32")),
                            "GetLogicalProcessorInformation");
    if (NULL == glpi) 
    {
        DbgPrint(TEXT("\nGetLogicalProcessorInformation is not supported.\n"));
        return FALSE;
    }

    while (!done)
    {
        DWORD rc = glpi(buffer, &returnLength);

        if (FALSE == rc) 
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
            {
                if (buffer) 
					RtlFreeHeap(GetProcessHeap(),0,buffer);

                buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)RtlAllocateHeap(GetProcessHeap(), HEAP_ZERO_MEMORY, returnLength);

                if (NULL == buffer) 
                {
                    DbgPrint(TEXT("\nError: Allocation failure\n"));
                    //return (2);
                }
            } 
            else 
            {
                DbgPrint(TEXT("\nError %d\n"), GetLastError());
                //return (3);
            }
        } 
        else
        {
            done = TRUE;
        }
    }

    ptr = buffer;

    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength) 
    {
        switch (ptr->Relationship) 
        {
        case RelationNumaNode:
            //Non-NUMA systems report a single record of this type.
            //numaNodeCount++;
			CurrentLength += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
			if (CurrentLength <= Output->Size) {
				Output->NumaNode.GroupMask.Mask = ptr->ProcessorMask;
				Output->NumaNode.GroupMask.Group = 0;
				Output->NumaNode.Reserved[0] = 0;
				Output->NumaNode.NodeNumber = ptr->NumaNode.NodeNumber;
				Output += 1;			
			}
            break;
        case RelationProcessorCore:
            CurrentLength += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
            if (CurrentLength <= Output->Size) {
                Output->Processor.GroupMask[0].Mask = ptr->ProcessorMask;
				Output->Processor.GroupMask[0].Group = 0;
                Output->Processor.Reserved[0] = 0;
				Output->Processor.EfficiencyClass = 0;
                Output->Processor.Flags = ptr->ProcessorCore.Flags;
                Output += 1;    
            } 		
            //processorCoreCount++;

            //A hyperthreaded core supplies more than one logical processor.
            //logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
            break;

        case RelationCache:
            //Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
            Cache = &ptr->Cache;
	
            CurrentLength += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
            if (CurrentLength <= Output->Size) {
                Output->Cache.Level = Cache->Level;
                Output->Cache.Associativity = Cache->Associativity;
				Output->Cache.LineSize = Cache->LineSize;
                Output->Cache.CacheSize = Cache->Size;
                Output->Cache.Type = Cache->Type;	
				Output->Cache.GroupMask.Mask = ptr->ProcessorMask;				
                Output += 1;    
            } 			
            break;

        case RelationProcessorPackage:
            //Logical processors share a physical package.
            CurrentLength += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
            if (CurrentLength <= Output->Size) {
                Output->Processor.GroupMask[0].Mask = ptr->ProcessorMask;
				Output->Processor.GroupMask[0].Group = 0;
                Output->Processor.Reserved[0] = 0;
				Output->Processor.EfficiencyClass = 0;
                Output->Processor.Flags = 0;
                Output += 1;    
            } 	
            break;
		case RelationGroup:
			GetSystemInfo( &sysinfo );
            CurrentLength += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
            if (CurrentLength <= Output->Size) {			
				Output->Group.MaximumGroupCount = 1;
				Output->Group.ActiveGroupCount = 1;
				Output->Group.GroupInfo[0].MaximumProcessorCount = sysinfo.dwNumberOfProcessors;
				Output->Group.GroupInfo[0].ActiveProcessorCount = sysinfo.dwNumberOfProcessors;
				Output->Group.GroupInfo[0].ActiveProcessorMask = ptr->ProcessorMask;
                Output += 1;    
            } 			
			break;		
        default:
            DbgPrint(TEXT("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n"));
            break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }
	
    *ReturnedLength = CurrentLength;	
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

/**********************************************************************
 * TlsAlloc             [KERNEL32.@]
 *
 * Allocates a thread local storage index.
 *
 * RETURNS
 *    Success: TLS index.
 *    Failure: 0xFFFFFFFF
 */
DWORD WINAPI TlsAlloc( void )
{
    DWORD index;
    PEB * const peb = NtCurrentTeb()->ProcessEnvironmentBlock;

    RtlAcquirePebLock();
    index = RtlFindClearBitsAndSet( peb->TlsBitmap, 1, 1 );
    if (index != ~0U) NtCurrentTeb()->TlsSlots[index] = 0; /* clear the value */
    else
    {
        index = RtlFindClearBitsAndSet( peb->TlsExpansionBitmap, 1, 0 );
        if (index != ~0U)
        {
            if (!NtCurrentTeb()->TlsExpansionSlots &&
                !(NtCurrentTeb()->TlsExpansionSlots = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY,
                                         8 * sizeof(peb->TlsExpansionBitmapBits) * sizeof(void*) )))
            {
                RtlClearBits( peb->TlsExpansionBitmap, index, 1 );
                index = ~0U;
                SetLastError( ERROR_NOT_ENOUGH_MEMORY );
            }
            else
            {
                NtCurrentTeb()->TlsExpansionSlots[index] = 0; /* clear the value */
                index += TLS_MINIMUM_AVAILABLE;
            }
        }
        else SetLastError( ERROR_NO_MORE_ITEMS );
    }
    RtlReleasePebLock();
    return index;
}


/**********************************************************************
 * TlsFree              [KERNEL32.@]
 *
 * Releases a thread local storage index, making it available for reuse.
 *
 * PARAMS
 *    index [in] TLS index to free.
 *
 * RETURNS
 *    Success: TRUE
 *    Failure: FALSE
 */
BOOL WINAPI TlsFree( DWORD index )
{
    BOOL ret;

    RtlAcquirePebLock();
    if (index >= TLS_MINIMUM_AVAILABLE)
    {
        ret = RtlAreBitsSet( NtCurrentTeb()->ProcessEnvironmentBlock->TlsExpansionBitmap, index - TLS_MINIMUM_AVAILABLE, 1 );
        if (ret) RtlClearBits( NtCurrentTeb()->ProcessEnvironmentBlock->TlsExpansionBitmap, index - TLS_MINIMUM_AVAILABLE, 1 );
    }
    else
    {
        ret = RtlAreBitsSet( NtCurrentTeb()->ProcessEnvironmentBlock->TlsBitmap, index, 1 );
        if (ret) RtlClearBits( NtCurrentTeb()->ProcessEnvironmentBlock->TlsBitmap, index, 1 );
    }
    if (ret) NtSetInformationThread( GetCurrentThread(), ThreadZeroTlsCell, &index, sizeof(index) );
    else SetLastError( ERROR_INVALID_PARAMETER );
    RtlReleasePebLock();
    return ret;
}


/**********************************************************************
 * TlsGetValue          [KERNEL32.@]
 *
 * Gets value in a thread's TLS slot.
 *
 * PARAMS
 *    index [in] TLS index to retrieve value for.
 *
 * RETURNS
 *    Success: Value stored in calling thread's TLS slot for index.
 *    Failure: 0 and GetLastError() returns NO_ERROR.
 */
LPVOID WINAPI TlsGetValue( DWORD index )
{
    LPVOID ret;

    if (index < TLS_MINIMUM_AVAILABLE)
    {
        ret = NtCurrentTeb()->TlsSlots[index];
    }
    else
    {
        index -= TLS_MINIMUM_AVAILABLE;
        if (index >= 8 * sizeof(NtCurrentTeb()->ProcessEnvironmentBlock->TlsExpansionBitmapBits))
        {
            SetLastError( ERROR_INVALID_PARAMETER );
            return NULL;
        }
        if (!NtCurrentTeb()->TlsExpansionSlots) ret = NULL;
        else ret = NtCurrentTeb()->TlsExpansionSlots[index];
    }
    SetLastError( ERROR_SUCCESS );
    return ret;
}


/**********************************************************************
 * TlsSetValue          [KERNEL32.@]
 *
 * Stores a value in the thread's TLS slot.
 *
 * PARAMS
 *    index [in] TLS index to set value for.
 *    value [in] Value to be stored.
 *
 * RETURNS
 *    Success: TRUE
 *    Failure: FALSE
 */
BOOL WINAPI TlsSetValue( DWORD index, LPVOID value )
{
    if (index < TLS_MINIMUM_AVAILABLE)
    {
        NtCurrentTeb()->TlsSlots[index] = value;
    }
    else
    {
        index -= TLS_MINIMUM_AVAILABLE;
        if (index >= 8 * sizeof(NtCurrentTeb()->ProcessEnvironmentBlock->TlsExpansionBitmapBits))
        {
            SetLastError( ERROR_INVALID_PARAMETER );
            return FALSE;
        }
        if (!NtCurrentTeb()->TlsExpansionSlots &&
            !(NtCurrentTeb()->TlsExpansionSlots = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY,
                         8 * sizeof(NtCurrentTeb()->ProcessEnvironmentBlock->TlsExpansionBitmapBits) * sizeof(void*) )))
        {
            SetLastError( ERROR_NOT_ENOUGH_MEMORY );
            return FALSE;
        }
        NtCurrentTeb()->TlsExpansionSlots[index] = value;
    }
    return TRUE;
}

BOOL 
WINAPI 
QueryProcessCycleTime(
  _In_  HANDLE   ThreadHandle,
  _Out_ PULONG64 CycleTime
)
{
	LARGE_INTEGER ltime;
	UINT32 cycles; 
	QueryPerformanceCounter(&ltime);

	cycles = (UINT32) ((ltime.QuadPart >> 8) & 0xFFFFFFF);	
	
	*CycleTime = cycles;
}

BOOL 
WINAPI 
QueryIdleProcessorCycleTime(
  _Inout_  PULONG   BufferLength,
  _Out_ PULONG64 CycleTime
)
{
	LARGE_INTEGER ltime;
	UINT32 cycles; 
	QueryPerformanceCounter(&ltime);

	cycles = (UINT32) ((ltime.QuadPart >> 8) & 0xFFFFFFF);	
	
	*CycleTime = cycles;
	*BufferLength = sizeof(cycles);
}