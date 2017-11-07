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

WINE_DEFAULT_DEBUG_CHANNEL(process); 

typedef struct _SYSTEM_LOGICAL_INFORMATION_FILLED{
	CACHE_DESCRIPTOR  CacheLevel1;
	CACHE_DESCRIPTOR  CacheLevel2;
	CACHE_DESCRIPTOR  CacheLevel3;
	DWORD PackagesNumber;
	DWORD CoresNumber;
	DWORD LogicalProcessorsNumber;
	DWORD NumaNumber;
}SYSTEM_LOGICAL_INFORMATION_FILLED, *PSYSTEM_LOGICAL_INFORMATION_FILLED;

UNICODE_STRING NoDefaultCurrentDirectoryInExePath = RTL_CONSTANT_STRING(L"NoDefaultCurrentDirectoryInExePath");

typedef BOOL (WINAPI *LPFN_GLPI)(
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, 
    PDWORD);
	
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
    NTSTATUS Status;
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

static BOOL grow_logical_proc_buf(SYSTEM_LOGICAL_PROCESSOR_INFORMATION **pdata,
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX **pdataex, DWORD *max_len)
{
    if (pdata)
    {
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION *new_data;

        *max_len *= 2;
        new_data = RtlReAllocateHeap(GetProcessHeap(), 0, *pdata, *max_len*sizeof(*new_data));
        if (!new_data)
            return FALSE;

        *pdata = new_data;
    }
    else
    {
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *new_dataex;

        *max_len *= 2;
        new_dataex = RtlReAllocateHeap(GetProcessHeap(), HEAP_ZERO_MEMORY, *pdataex, *max_len*sizeof(*new_dataex));
        if (!new_dataex)
            return FALSE;

        *pdataex = new_dataex;
    }

    return TRUE;
}

static DWORD log_proc_ex_size_plus(DWORD size)
{
    /* add SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX.Relationship and .Size */
    return sizeof(LOGICAL_PROCESSOR_RELATIONSHIP) + sizeof(DWORD) + size;
}

static inline BOOL logical_proc_info_add_by_id(
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX **pdataex, 
	DWORD *len, 
	DWORD *pmax_len,
    LOGICAL_PROCESSOR_RELATIONSHIP rel, 
	DWORD id, 
	ULONG_PTR mask
)
{
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *dataex;
        DWORD ofs = 0;

        while(ofs < *len)
        {
            dataex = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *)(((char *)*pdataex) + ofs);
            if (rel == RelationProcessorPackage && dataex->Relationship == rel && dataex->Processor.Reserved[1] == id)
            {
                dataex->Processor.GroupMask[0].Mask |= mask;
                return TRUE;
            }
            ofs += dataex->Size;
        }

        /* TODO: For now, just one group. If more than 64 processors, then we
         * need another group. */

        while (ofs + log_proc_ex_size_plus(sizeof(PROCESSOR_RELATIONSHIP)) > *pmax_len)
        {
            if (!grow_logical_proc_buf(NULL, pdataex, pmax_len))
                return FALSE;
        }

        dataex = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *)(((char *)*pdataex) + ofs);

        dataex->Relationship = rel;
        dataex->Size = log_proc_ex_size_plus(sizeof(PROCESSOR_RELATIONSHIP));
        dataex->Processor.Flags = 0; /* TODO */
        dataex->Processor.EfficiencyClass = 0;
        dataex->Processor.GroupCount = 1;
        dataex->Processor.GroupMask[0].Mask = mask;
        dataex->Processor.GroupMask[0].Group = 0;
        /* mark for future lookup */
        dataex->Processor.Reserved[0] = 0;
        dataex->Processor.Reserved[1] = id;

        *len += dataex->Size;
    return TRUE;
}

static inline BOOL logical_proc_info_add_cache(
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION **pdata,
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX **pdataex, 
	DWORD *len,
    DWORD *pmax_len, 
	ULONG_PTR mask, 
	CACHE_DESCRIPTOR *cache
)
{
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *dataex;
        DWORD ofs;

        for (ofs = 0; ofs < *len; )
        {
            dataex = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *)(((char *)*pdataex) + ofs);
            if (dataex->Relationship == RelationCache && dataex->Cache.GroupMask.Mask == mask &&
                    dataex->Cache.Level == cache->Level && dataex->Cache.Type == cache->Type)
                return TRUE;
            ofs += dataex->Size;
        }

        while (ofs + log_proc_ex_size_plus(sizeof(CACHE_RELATIONSHIP)) > *pmax_len)
        {
            if (!grow_logical_proc_buf(NULL, pdataex, pmax_len))
                return FALSE;
        }

        dataex = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *)(((char *)*pdataex) + ofs);

        dataex->Relationship = RelationCache;
        dataex->Size = log_proc_ex_size_plus(sizeof(CACHE_RELATIONSHIP));
        dataex->Cache.Level = cache->Level;
        dataex->Cache.Associativity = cache->Associativity;
        dataex->Cache.LineSize = cache->LineSize;
        dataex->Cache.CacheSize = cache->Size;
        dataex->Cache.Type = cache->Type;
        dataex->Cache.GroupMask.Mask = mask;
        dataex->Cache.GroupMask.Group = 0;

        *len += dataex->Size;
    return TRUE;
}

static inline BOOL logical_proc_info_add_numa_node(
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION **pdata,
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX **pdataex, 
	DWORD *len, 
	DWORD *pmax_len, 
	ULONG_PTR mask,
    DWORD node_id
)
{
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *dataex;

        while (*len + log_proc_ex_size_plus(sizeof(NUMA_NODE_RELATIONSHIP)) > *pmax_len)
        {
            if (!grow_logical_proc_buf(NULL, pdataex, pmax_len))
                return FALSE;
        }

        dataex = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *)(((char *)*pdataex) + *len);

        dataex->Relationship = RelationNumaNode;
        dataex->Size = log_proc_ex_size_plus(sizeof(NUMA_NODE_RELATIONSHIP));
        dataex->NumaNode.NodeNumber = node_id;
        dataex->NumaNode.GroupMask.Mask = mask;
        dataex->NumaNode.GroupMask.Group = 0;

        *len += dataex->Size;
    return TRUE;
}

static inline BOOL logical_proc_info_add_group(
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX **pdataex,
    DWORD *len, 
	DWORD *pmax_len, 
	DWORD num_cpus, 
	ULONG_PTR mask
)
{
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *dataex;

    while (*len + log_proc_ex_size_plus(sizeof(GROUP_RELATIONSHIP)) > *pmax_len)
    {
        if (!grow_logical_proc_buf(NULL, pdataex, pmax_len))
            return FALSE;
    }

    dataex = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *)(((char *)*pdataex) + *len);

    dataex->Relationship = RelationGroup;
    dataex->Size = log_proc_ex_size_plus(sizeof(GROUP_RELATIONSHIP));
    dataex->Group.MaximumGroupCount = 1;
    dataex->Group.ActiveGroupCount = 1;
    dataex->Group.GroupInfo[0].MaximumProcessorCount = num_cpus;
    dataex->Group.GroupInfo[0].ActiveProcessorCount = num_cpus;
    dataex->Group.GroupInfo[0].ActiveProcessorMask = mask;

    *len += dataex->Size;

    return TRUE;
}	

SYSTEM_LOGICAL_INFORMATION_FILLED _cdecl GetLogicalInfo()
{
    LPFN_GLPI glpi;
    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
	SYSTEM_LOGICAL_INFORMATION_FILLED fill = {1};
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

    glpi = (LPFN_GLPI) GetProcAddress(
                            GetModuleHandle(TEXT("kernel32")),
                            "GetLogicalProcessorInformation");
    if (NULL == glpi) 
    {
        DbgPrint(TEXT("\nGetLogicalProcessorInformation is not supported.\n"));
        return fill;
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
                    return fill;
                }
            } 
            else 
            {
                DbgPrint(TEXT("\nError %d\n"), GetLastError());
                return fill;
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
            // Non-NUMA systems report a single record of this type.
            numaNodeCount++;
            break;

        case RelationProcessorCore:
            processorCoreCount++;

            // A hyperthreaded core supplies more than one logical processor.
            logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
            break;

        case RelationCache:
            // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
            Cache = &ptr->Cache;
			
            if (Cache->Level == 1)
            {
                processorL1CacheCount++;
				fill.CacheLevel1 = *Cache;
            }
            else if (Cache->Level == 2)
            {
				fill.CacheLevel2 = *Cache;
                processorL2CacheCount++;
            }
            else if (Cache->Level == 3)
            {
				fill.CacheLevel3 = *Cache;
                processorL3CacheCount++;
            }
            break;

        case RelationProcessorPackage:
            // Logical processors share a physical package.
            processorPackageCount++;
            break;

        default:
            DbgPrint(TEXT("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n"));
            break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }
	
	fill.NumaNumber = numaNodeCount;
	fill.PackagesNumber = processorPackageCount;
	fill.CoresNumber = processorCoreCount;
	fill.LogicalProcessorsNumber = logicalProcessorCount;
    
    RtlFreeHeap(GetProcessHeap(),0,buffer);

    return fill;
}

/* for 'data', max_len is the array count. for 'dataex', max_len is in bytes */
static NTSTATUS create_logical_proc_info(SYSTEM_LOGICAL_PROCESSOR_INFORMATION **data,
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX **dataex, DWORD *max_len)
{
    DWORD pkgs_no, cores_no, lcpu_no, lcpu_per_core, cores_per_package, assoc, len = 0;
    DWORD cache_ctrs[10] = {0};
    ULONG_PTR all_cpus_mask = 0;
    CACHE_DESCRIPTOR cache[10];
    LONGLONG cache_size, cache_line_size, cache_sharing[10];
    size_t size;
    DWORD p,i,j,k;
	SYSTEM_LOGICAL_INFORMATION_FILLED fill;
	
	fill = GetLogicalInfo();

    lcpu_no = fill.LogicalProcessorsNumber;
	
	pkgs_no = fill.PackagesNumber;
	cores_no = fill.CoresNumber;

    DbgPrint("Kernel32 :: create_logical_proc_info :: %u logical CPUs from %u physical cores across %u packages\n",
            lcpu_no, cores_no, pkgs_no);

    lcpu_per_core = lcpu_no / cores_no;
    cores_per_package = cores_no / pkgs_no;
	
//(DWORD)32 << 10; /* 16 KB */
    memset(cache, 0, sizeof(cache));
    cache[1].Level = 1;
	cache[1].Size = fill.CacheLevel1.Size;
    cache[1].Type = CacheInstruction;
    cache[1].Associativity = 8; /* reasonable default */
    cache[1].LineSize = 0x40; /* reasonable default */
    cache[2].Level = 1;
	cache[2].Size = fill.CacheLevel1.Size;
    cache[2].Type = CacheData;
    cache[2].Associativity = 8;
    cache[2].LineSize = 0x40;
    cache[3].Level = 2;
	cache[3].Size = fill.CacheLevel2.Size;
    cache[3].Type = CacheUnified;
    cache[3].Associativity = fill.CacheLevel2.Associativity;
    cache[3].LineSize = 0x40;
    cache[4].Level = 3;
	cache[4].Size = fill.CacheLevel3.Size;
    cache[4].Type = CacheUnified;
    cache[4].Associativity = 12;
    cache[4].LineSize = 0x40;
	
    cache_sharing[1] = lcpu_per_core;
    cache_sharing[2] = lcpu_per_core;
    cache_sharing[3] = lcpu_per_core;
    cache_sharing[4] = lcpu_no;

    for(p = 0; p < pkgs_no; ++p){
        for(j = 0; j < cores_per_package && p * cores_per_package + j < cores_no; ++j){
            ULONG_PTR mask = 0;

            for(k = 0; k < lcpu_per_core; ++k)
                mask |= (ULONG_PTR)1 << (j * lcpu_per_core + k);

            all_cpus_mask |= mask;

            /* add to package */
            if(!logical_proc_info_add_by_id(dataex, &len, max_len, RelationProcessorPackage, p, mask))
                return STATUS_NO_MEMORY;

            /* add new core */
            if(!logical_proc_info_add_by_id(dataex, &len, max_len, RelationProcessorCore, p, mask))
                return STATUS_NO_MEMORY;

            for(i = 1; i < 5; ++i){
                if(cache_ctrs[i] == 0 && cache[i].Size > 0){
                    mask = 0;
                    for(k = 0; k < cache_sharing[i]; ++k)
                        mask |= (ULONG_PTR)1 << (j * lcpu_per_core + k);

                    if(!logical_proc_info_add_cache(data, dataex, &len, max_len, mask, &cache[i]))
                        return STATUS_NO_MEMORY;
                }

                cache_ctrs[i] += lcpu_per_core;

                if(cache_ctrs[i] == cache_sharing[i])
                    cache_ctrs[i] = 0;
            }
        }
    }
	
	DbgPrint("create_logical_proc_info part 6\n");	

    /* OSX doesn't support NUMA, so just make one NUMA node for all CPUs */
    if(!logical_proc_info_add_numa_node(data, dataex, &len, max_len, all_cpus_mask, 0))
        return STATUS_NO_MEMORY;

    if(dataex)
        logical_proc_info_add_group(dataex, &len, max_len, lcpu_no, all_cpus_mask);

    if(data)
        *max_len = len * sizeof(**data);
    else
        *max_len = len;
	
	DbgPrint("create_logical_proc_info part 7\n");	

    return STATUS_SUCCESS;
}

/******************************************************************************
 * NtQuerySystemInformationEx [NTDLL.@]
 * ZwQuerySystemInformationEx [NTDLL.@]
 */
NTSTATUS WINAPI NtQuerySystemInformationExInternal(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
    void *Query, 
	ULONG QueryLength, 
	void *SystemInformation, ULONG Length, ULONG *ResultLength)
{
    ULONG len = 0;
    NTSTATUS ret = STATUS_NOT_IMPLEMENTED;
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *buf;

    DbgPrint("(0x%08x,%p,%u,%p,%u,%p) stub\n", SystemInformationClass, Query, QueryLength, SystemInformation,
        Length, ResultLength);

    // switch (SystemInformationClass) {
    // case SystemLogicalProcessorInformationEx:
     //   {
            

            // if (!Query || QueryLength < sizeof(DWORD))
            // {
                // ret = STATUS_INVALID_PARAMETER;
                // return ret;
            // }

            // if (*(DWORD*)Query != RelationAll)
                // DbgPrint("Relationship filtering not implemented: 0x%x\n", *(DWORD*)Query);

             len = 3 * sizeof(*buf);
             buf = RtlAllocateHeap(GetProcessHeap(), 0, len);
            // if (!buf)
            // {
                // ret = STATUS_NO_MEMORY;
                // return ret;
            // }

            ret = create_logical_proc_info(NULL, &buf, &len);
			DbgPrint("NtQuerySystemInformationExInternal:: Status: %0x%08x\n", ret);
            if (ret != STATUS_SUCCESS)
            {
                RtlFreeHeap(GetProcessHeap(), 0, buf);
                return ret;
            }

            if (Length >= len)
            {
                if (!SystemInformation)
                    ret = STATUS_ACCESS_VIOLATION;
                else
                    memcpy( SystemInformation, buf, len);
            }
            else
                ret = STATUS_INFO_LENGTH_MISMATCH;

            RtlFreeHeap(GetProcessHeap(), 0, buf);

     //       break;
    //    }
    // default:
        // FIXME("(0x%08x,%p,%u,%p,%u,%p) stub\n", SystemInformationClass, Query, QueryLength, SystemInformation,
            // Length, ResultLength);
        // break;
    // }

    if (ResultLength)
        *ResultLength = len;

    return ret;
}

/***********************************************************************
 *           GetLogicalProcessorInformationEx   (KERNEL32.@)
 */
BOOL WINAPI GetLogicalProcessorInformationEx(LOGICAL_PROCESSOR_RELATIONSHIP relationship, SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *buffer, DWORD *len)
{
    NTSTATUS status;

    DbgPrint("(%u,%p,%p)\n", relationship, buffer, len);

    if (!len)
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }

    status = NtQuerySystemInformationExInternal( 1, &relationship, sizeof(relationship),
        buffer, *len, len );
    if (status == STATUS_INFO_LENGTH_MISMATCH)
    {
        SetLastError( ERROR_INSUFFICIENT_BUFFER );
        return FALSE;
    }
    if (status != STATUS_SUCCESS)
    {
        SetLastError( RtlNtStatusToDosError( status ) );
        return FALSE;
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

HRESULT 
WINAPI 
UnregisterApplicationRestart(void)
{
	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	return 0;
}

HRESULT 
WINAPI 
UnregisterApplicationRecoveryCallback(void)
{
	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	return 0;	
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
	BOOL resp;
	
	resp = QueryPerformanceCounter(&ltime);

	cycles = (UINT32) ((ltime.QuadPart >> 8) & 0xFFFFFFF);	
	
	*CycleTime = cycles;
	return resp;
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
	BOOL resp;	
	
	resp = QueryPerformanceCounter(&ltime);

	cycles = (UINT32) ((ltime.QuadPart >> 8) & 0xFFFFFFF);	
	
	*CycleTime = cycles;
	*BufferLength = sizeof(cycles);
	return resp;
}

BOOL 
WINAPI 
SetProcessAffinityUpdateMode(
  _In_ HANDLE ProcessHandle,
  _In_ DWORD  dwFlags
)
{
	//We don't support this feature for now
	return TRUE;
}

BOOL 
WINAPI 
QueryProcessAffinityUpdateMode(
  _In_      HANDLE ProcessHandle,
  _Out_opt_ DWORD  lpdwFlags
)
{
	//We don't support this feature for now, setting disabling feature
	lpdwFlags = 0;
	return TRUE;	
}