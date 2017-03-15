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
Copyright (c) 2015  Microsoft Corporation 
 
Module Name:

    ldrrsrc.c
 
 */
 
#define NDEBUG

#include <main.h>

#define DOS_MAX_PATH_LENGTH   (260)

#define NO_ALTERNATE_RESOURCE_MODULE    (PVOID)(-1)

#define LDR_RESOURCE_ID_NAME_MASK ((~(ULONG_PTR)0) << 16) /* lower 16bits clear */

#define  USE_FIRSTAVAILABLE_LANGID   (0xFFFFFFFF)

#define  MEMBLOCKSIZE 32

#define  RESMODSIZE sizeof(ALT_RESOURCE_MODULE)

#define LDR_DATAFILE_TO_VIEW(x) ((PVOID)(((ULONG_PTR)(x)) & ~(ULONG_PTR)1))

LANGID UILangId, InstallLangId, DefaultLangId, ImpersonateLangId;

ULONG AlternateResourceModuleCount;

ULONG AltResMemBlockCount;

PALT_RESOURCE_MODULE AlternateResourceModules;

BOOLEAN 
NTAPI
LdrAlternateResourcesEnabled(void)
{
  if ( (!UILangId || ImpersonateLangId || NtCurrentTeb()->IsImpersonating) && NtQueryDefaultUILanguage(&UILangId) >= 0 )
    ImpersonateLangId = NtCurrentTeb()->IsImpersonating != 0 ? UILangId : 0;
  return UILangId && (InstallLangId || NtQueryInstallUILanguage(&InstallLangId) >= 0);
}

PVOID
LdrGetAlternateResourceModuleHandle(
    IN PVOID Module
    )
/*++

Routine Description:

    This function gets the alternate resource module from the table
    containing the handle.

Arguments:

    Module - Module of which alternate resource module needs to loaded.

Return Value:

   Handle of the alternate resource module.

--*/

{
    ULONG ModuleIndex;

    for (ModuleIndex = 0;
         ModuleIndex < AlternateResourceModuleCount;
         ModuleIndex++ ){
        if (AlternateResourceModules[ModuleIndex].ModuleBase ==
            Module){
            return AlternateResourceModules[ModuleIndex].AlternateModule;
        }
    }
    return NULL;
}

BOOLEAN
LdrpSetAlternateResourceModuleHandle(
    IN PVOID Module,
    IN PVOID AlternateModule
    )

/*++

Routine Description:

    This function records the handle of the base module and alternate
    resource module in an array.

Arguments:

    Module - The handle of the base module.
    AlternateModule - The handle of the alternate resource module

Return Value:

    TBD.

--*/

{
    PALT_RESOURCE_MODULE NewModules;

    if (AlternateResourceModules == NULL){
        //
        //  Allocate memory of initial size MEMBLOCKSIZE.
        //
        NewModules = RtlAllocateHeap(
                        RtlProcessHeap(),
                        HEAP_ZERO_MEMORY,
                        RESMODSIZE * MEMBLOCKSIZE);
        if (!NewModules){
            return FALSE;
            }
        AlternateResourceModules = NewModules;
        AltResMemBlockCount = MEMBLOCKSIZE;
        }
    else
    if (AlternateResourceModuleCount >= AltResMemBlockCount ){
        //
        //  ReAllocate another chunk of memory.
        //
        NewModules = RtlReAllocateHeap(
                        RtlProcessHeap(),
                        0,
                        AlternateResourceModules,
                        (AltResMemBlockCount + MEMBLOCKSIZE) * RESMODSIZE
                        );

        if (!NewModules){
            return FALSE;
            }
        AlternateResourceModules = NewModules;
        AltResMemBlockCount += MEMBLOCKSIZE;
        }

    AlternateResourceModules[AlternateResourceModuleCount].ModuleBase = Module;
    AlternateResourceModules[AlternateResourceModuleCount].AlternateModule = AlternateModule;



    AlternateResourceModuleCount++;

    return TRUE;

}

PVOID
NTAPI
LdrLoadAlternateResourceModuleEx(
	IN LANGID CustomLangId,
    IN PVOID Module,
    IN LPCWSTR PathToAlternateModule OPTIONAL
    )

/*++

Routine Description:

    This function does the acutally loading into memory of the alternate
    resource module, or loads from the table if it was loaded before.

Arguments:

    Module - The handle of the base module.
    PathToAlternateModule - Optional path from which module is being loaded.

Return Value:

    Handle to the alternate resource module.

--*/

{
    PVOID AlternateModule, DllBase;
    PLDR_DATA_TABLE_ENTRY Entry;
    HANDLE FileHandle, MappingHandle;
    PIMAGE_NT_HEADERS NtHeaders;
    NTSTATUS Status;
    OBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING AltDllName;
    PVOID FreeBuffer;
    LPWSTR BaseDllName, p;
    WCHAR DllPathName[DOS_MAX_PATH_LENGTH];
    ULONG DllPathNameLength, BaseDllNameLength, CopyCount;
    ULONG Digit;
    int i, RetryCount;
    WCHAR AltModulePath[DOS_MAX_PATH_LENGTH];
    WCHAR AltModulePathMUI[DOS_MAX_PATH_LENGTH];
	WCHAR AltLocaleNamePathMUI[DOS_MAX_PATH_LENGTH];	
    WCHAR AltModulePathFallback[DOS_MAX_PATH_LENGTH];
    WCHAR AltModuleCulturePathFallback[DOS_MAX_PATH_LENGTH];	
    IO_STATUS_BLOCK IoStatusBlock;
    RTL_RELATIVE_NAME_U RelativeName;
    SIZE_T ViewSize;
    LARGE_INTEGER SectionOffset;
    WCHAR LangIdDir[6];
    UNICODE_STRING AltModulePathList[5];
    UNICODE_STRING NtSystemRoot;
	UNICODE_STRING LocaleName;

    /*if (!LdrAlternateResourcesEnabled()) {
        return NULL;
        }*/

    RtlEnterCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);

    AlternateModule = LdrGetAlternateResourceModuleHandle(Module);
	if(!CustomLangId)
	{
		if ( (!UILangId || ImpersonateLangId || NtCurrentTeb()->IsImpersonating) && ZwQueryDefaultUILanguage(&UILangId) >= 0 )
			ImpersonateLangId = NtCurrentTeb()->IsImpersonating != 0 ? UILangId : 0;
		CustomLangId = UILangId;
	}
	//AlternateModule = LdrGetAlternateResourceModuleHandle(Module, CustomLangId);
    if (AlternateModule == NO_ALTERNATE_RESOURCE_MODULE){
        //
        //  We tried to load this module before but failed. Don't try
        //  again in the future.
        //
        RtlLeaveCriticalSection(
            (PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
        return NULL;
        }
    else if (AlternateModule > 0){
        //
        //  We found the previously loaded match
        //
        RtlLeaveCriticalSection(
            (PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
        return AlternateModule;
        }

    if (ARGUMENT_PRESENT(PathToAlternateModule)){
        //
        //  Caller suplied path.
        //

        CopyCount = wcslen(PathToAlternateModule);

        for (p = (LPWSTR) PathToAlternateModule + CopyCount;
             p > PathToAlternateModule;
             p--){
            if (*(p-1) == L'\\'){
                break;
                }
            }

        if (p == PathToAlternateModule){
            goto error_exit;
            }

        DllPathNameLength = (ULONG)(p - PathToAlternateModule) * sizeof(WCHAR);

        RtlCopyMemory(
            DllPathName,
            PathToAlternateModule,
            DllPathNameLength
            );

        BaseDllName = p ;
        BaseDllNameLength = CopyCount * sizeof(WCHAR) - DllPathNameLength;

        }
    else{
        //
        //  Try to get full dll path from Ldr data table.
        //
        Status = LdrFindEntryForAddress(Module, &Entry);
        if (!NT_SUCCESS( Status )){
            goto error_exit;
            }

        DllPathNameLength = Entry->FullDllName.Length -
                            Entry->BaseDllName.Length;

        RtlCopyMemory(
            DllPathName,
            Entry->FullDllName.Buffer,
            DllPathNameLength);

        BaseDllName = Entry->BaseDllName.Buffer;
        BaseDllNameLength = Entry->BaseDllName.Length;
        }

    DllPathName[DllPathNameLength / sizeof(WCHAR)] = UNICODE_NULL;

    //
    //  Generate the langid directory like "0804\"
    //
    if (!UILangId){
        Status = NtQueryDefaultUILanguage( &UILangId );
        if (!NT_SUCCESS( Status )) {		
            goto error_exit;
            }
        }

    CopyCount = 0;
    for (i = 12; i >= 0; i -= 4){
        Digit = ((CustomLangId >> i) & 0xF);
        if (Digit >= 10){
            LangIdDir[CopyCount++] = (WCHAR) (Digit - 10 + L'A');
            }
        else{
            LangIdDir[CopyCount++] = (WCHAR) (Digit + L'0');
            }
        }

    LangIdDir[CopyCount++] = L'\\';
    LangIdDir[CopyCount++] = UNICODE_NULL;
	
	//
	// Get culture name by LANGID of CustomLangId
	//
	RtlLCIDToCultureName(CustomLangId,&LocaleName);

    //
    //  Generate the first path c:\winnt\system32\mui\0409\ntdll.dll.mui
    //
    AltModulePathList[1].Buffer = AltModulePath;
    AltModulePathList[1].Length = 0;
    AltModulePathList[1].MaximumLength = sizeof(AltModulePath);

    RtlAppendUnicodeToString(&AltModulePathList[1], DllPathName);
    RtlAppendUnicodeToString(&AltModulePathList[1], L"mui\\");
    RtlAppendUnicodeToString(&AltModulePathList[1], LangIdDir);
    RtlAppendUnicodeToString(&AltModulePathList[1], BaseDllName);	

    //
    //  Generate the first path c:\winnt\system32\mui\0409\ntdll.dll
    //
    AltModulePathList[0].Buffer = AltModulePathMUI;
    AltModulePathList[0].Length = 0;
    AltModulePathList[0].MaximumLength = sizeof(AltModulePathMUI);

    RtlCopyUnicodeString(&AltModulePathList[0], &AltModulePathList[1]);
    RtlAppendUnicodeToString(&AltModulePathList[0], L".mui");	

	//
    //  Generate the first path c:\winnt\system32\en-US\ntdll.mui
    //
    AltModulePathList[2].Buffer = AltLocaleNamePathMUI;
    AltModulePathList[2].Length = 0;
    AltModulePathList[2].MaximumLength = sizeof(AltLocaleNamePathMUI);	

    RtlAppendUnicodeToString(&AltModulePathList[2], DllPathName);
    RtlAppendUnicodeToString(&AltModulePathList[2], LocaleName.Buffer);
	RtlAppendUnicodeToString(&AltModulePathList[2], L"\\");
    RtlAppendUnicodeToString(&AltModulePathList[2], BaseDllName);
    RtlAppendUnicodeToString(&AltModulePathList[2], L".mui");	

    //
    //  Generate path c:\winnt\mui\fallback\0409\foo.exe.mui
    //
    AltModulePathList[3].Buffer = AltModulePathFallback;
    AltModulePathList[3].Length = 0;
    AltModulePathList[3].MaximumLength = sizeof(AltModulePathFallback);

    RtlInitUnicodeString(&NtSystemRoot, SharedUserData->NtSystemRoot);
    RtlAppendUnicodeStringToString(&AltModulePathList[3], &NtSystemRoot);
    RtlAppendUnicodeToString(&AltModulePathList[3], L"\\mui\\fallback\\");
    RtlAppendUnicodeToString(&AltModulePathList[3], LangIdDir);
    RtlAppendUnicodeToString(&AltModulePathList[3], BaseDllName);
    RtlAppendUnicodeToString(&AltModulePathList[3], L".mui");
	
	//
    //  Generate path c:\winnt\mui\fallback\0409\foo.exe.mui
    //
    AltModulePathList[4].Buffer = AltModuleCulturePathFallback;
    AltModulePathList[4].Length = 0;
    AltModulePathList[4].MaximumLength = sizeof(AltModuleCulturePathFallback);

    RtlInitUnicodeString(&NtSystemRoot, SharedUserData->NtSystemRoot);
    RtlAppendUnicodeStringToString(&AltModulePathList[4], &NtSystemRoot);
	RtlAppendUnicodeToString(&AltModulePathList[4], L"\\");	
    RtlAppendUnicodeToString(&AltModulePathList[4], LocaleName.Buffer);
	RtlAppendUnicodeToString(&AltModulePathList[4], L"\\");	
    RtlAppendUnicodeToString(&AltModulePathList[4], BaseDllName);
    RtlAppendUnicodeToString(&AltModulePathList[4], L".mui");
	
    //
    //  Try name with .mui extesion first.
    //
    RetryCount = 0;
    while (RetryCount < sizeof(AltModulePathList)/sizeof(UNICODE_STRING)){
        if (!RtlDosPathNameToNtPathName_U(
                    AltModulePathList[RetryCount].Buffer,
                    &AltDllName,
                    NULL,
                    &RelativeName
                    )){					
            goto error_exit;
            }

        FreeBuffer = AltDllName.Buffer;
        if ( RelativeName.RelativeName.Length ) {
            AltDllName = *(PUNICODE_STRING)&RelativeName.RelativeName;
            }
        else {
            RelativeName.ContainingDirectory = NULL;
            }

        InitializeObjectAttributes(
            &ObjectAttributes,
            &AltDllName,
            OBJ_CASE_INSENSITIVE,
            RelativeName.ContainingDirectory,
            NULL
            );


        Status = NtCreateFile(
                &FileHandle,
                (ACCESS_MASK) GENERIC_READ | SYNCHRONIZE | FILE_READ_ATTRIBUTES,
                &ObjectAttributes,
                &IoStatusBlock,
                NULL,
                0L,
                FILE_SHARE_READ | FILE_SHARE_DELETE,
                FILE_OPEN,
                0L,
                NULL,
                0L
                );

        RtlFreeHeap(RtlProcessHeap(), 0, FreeBuffer);

        if (NT_SUCCESS( Status )){
            break;
            }
        if (Status != STATUS_OBJECT_NAME_NOT_FOUND && RetryCount == 0) {
            //
            //  Error other than the file name with .mui not found.
            //  Most likely directory is missing.  Skip file name w/o .mui
            //  and goto fallback directory.
            //
            RetryCount++;
            }

        RetryCount++;
        }

    Status = NtCreateSection(
                &MappingHandle,
                STANDARD_RIGHTS_REQUIRED | SECTION_QUERY | SECTION_MAP_READ,
                NULL,
                NULL,
                PAGE_WRITECOPY,
                SEC_COMMIT,
                FileHandle
                );

    if (!NT_SUCCESS( Status )){
        goto error_exit;
        }

    NtClose( FileHandle );

    SectionOffset.LowPart = 0;
    SectionOffset.HighPart = 0;
    ViewSize = 0;
    DllBase = NULL;

    Status = NtMapViewOfSection(
                MappingHandle,
                NtCurrentProcess(),
                &DllBase,
                0L,
                0L,
                &SectionOffset,
                &ViewSize,
                ViewShare,
                0L,
                PAGE_WRITECOPY
                );

    NtClose( MappingHandle );

    if (!NT_SUCCESS( Status )){		
        goto error_exit;
        }

    NtHeaders = RtlImageNtHeader( DllBase );
    if (!NtHeaders) {
        NtUnmapViewOfSection(NtCurrentProcess(), (PVOID) DllBase);	
        goto error_exit;
        }

    AlternateModule = (HANDLE)((ULONG_PTR)DllBase | 0x00000001);

    LdrpSetAlternateResourceModuleHandle(Module, AlternateModule);
    RtlLeaveCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
    return AlternateModule;

error_exit:
    LdrpSetAlternateResourceModuleHandle(Module, NO_ALTERNATE_RESOURCE_MODULE);
    RtlLeaveCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
    return NULL;
}

PVOID
NTAPI
LdrLoadAlternateResourceModule(
    IN PVOID Module,
    IN LPCWSTR PathToAlternateModule OPTIONAL
    )
{
	return LdrLoadAlternateResourceModuleEx(0, Module, PathToAlternateModule);
}

BOOLEAN
NTAPI
LdrUnloadAlternateResourceModule(
    IN PVOID Module
    )

/*++

Routine Description:

    This function unmaps an alternate resource module from the process'
    address space and updates alternate resource module table.

Arguments:

    Module - handle of the base module.

Return Value:

    TBD.

--*/

{
    ULONG ModuleIndex;
    PALT_RESOURCE_MODULE AltModule;

    RtlEnterCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
    if (AlternateResourceModuleCount == 0){
        RtlLeaveCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
        return TRUE;
        }

    for (ModuleIndex = AlternateResourceModuleCount;
         ModuleIndex > 0;
         ModuleIndex--){
        if (AlternateResourceModules[ModuleIndex-1].ModuleBase == Module){
            break;
        }
    }

    if (ModuleIndex == 0){
        RtlLeaveCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
        return FALSE;
        }
    //
    //  Adjust to the actual index
    //
    ModuleIndex --;

    AltModule = &AlternateResourceModules[ModuleIndex];
    if (AltModule->AlternateModule != NO_ALTERNATE_RESOURCE_MODULE) {
        NtUnmapViewOfSection(
            NtCurrentProcess(),
            (PVOID)((ULONG_PTR)AltModule->AlternateModule & ~0x00000001)
            );
    }

    __try {
        if (ModuleIndex != AlternateResourceModuleCount - 1){
            //
            //  Consolidate the array.  Skip this if unloaded item
            //  is the last element.
            //
            RtlMoveMemory(
                AltModule,
                AltModule + 1,
                (AlternateResourceModuleCount - ModuleIndex - 1) * RESMODSIZE
                );
            }
        }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        RtlLeaveCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
        return FALSE;
        }

    AlternateResourceModuleCount --;

    if (AlternateResourceModuleCount == 0){
        RtlFreeHeap(
            RtlProcessHeap(),
            0,
            AlternateResourceModules
            );
        AlternateResourceModules = NULL;
        AltResMemBlockCount = 0;
        }
    else
    if (AlternateResourceModuleCount < AltResMemBlockCount - MEMBLOCKSIZE){
        AltModule = RtlReAllocateHeap(
                        RtlProcessHeap(),
                        0,
                        AlternateResourceModules,
                        (AltResMemBlockCount - MEMBLOCKSIZE) * RESMODSIZE);

        if (!AltModule){
            RtlLeaveCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
            return FALSE;
            }

        AlternateResourceModules = AltModule;
        AltResMemBlockCount -= MEMBLOCKSIZE;
        }
    RtlLeaveCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
    return TRUE;
}


BOOLEAN
LdrFlushAlternateResourceModules(
    VOID
    )

/*++

Routine Description:

    This function unmaps all the alternate resouce modules for the
    process address space. This function would be used mainly by
    CSRSS, and any sub-systems that are permanent during logon and
    logoff.


Arguments:

    None

Return Value:

    TRUE  : Successful
    FALSE : Failed

--*/

{
    ULONG ModuleIndex;
    PALT_RESOURCE_MODULE AltModule;

    //
    // Grab the loader lock
    //
    RtlEnterCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);

    if (AlternateResourceModuleCount > 0) {
        //
        // Let's unmap the alternate resource modules from the process
        // address space
        //
        for (ModuleIndex=0;
             ModuleIndex<AlternateResourceModuleCount;
             ModuleIndex++) {

            AltModule = &AlternateResourceModules[ModuleIndex];

            if (AltModule->AlternateModule != NO_ALTERNATE_RESOURCE_MODULE) {
                NtUnmapViewOfSection(NtCurrentProcess(),
                                     (PVOID)((ULONG_PTR)AltModule->AlternateModule & ~0x00000001));
                }
            }

        //
        // Cleanup alternate resource modules memory
        //
        RtlFreeHeap(RtlProcessHeap(), 0, AlternateResourceModules);
        AlternateResourceModules = NULL;
        AlternateResourceModuleCount = 0;
        AltResMemBlockCount = 0;

        }

    //
    // Re-Initialize the UI language for the current process,
    // and leave the LoaderLock
    //
    UILangId = 0;
    RtlLeaveCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);

    return TRUE;
}

/**********************************************************************
 *  is_data_file_module
 *
 * Check if a module handle is for a LOAD_LIBRARY_AS_DATAFILE module.
 */
static int is_data_file_module( PVOID BaseAddress )
{
    return (ULONG_PTR)BaseAddress & 1;
}

int page_fault(ULONG ExceptionCode) 	
{
    if (ExceptionCode == EXCEPTION_ACCESS_VIOLATION ||
        ExceptionCode == EXCEPTION_PRIV_INSTRUCTION)
        return EXCEPTION_EXECUTE_HANDLER;
    return EXCEPTION_CONTINUE_SEARCH;
}

NTSTATUS
LdrpAccessResourceDataNoMultipleLanguage(
    IN PVOID DllHandle,
    IN const IMAGE_RESOURCE_DATA_ENTRY* ResourceDataEntry,
    OUT PVOID *Address OPTIONAL,
    OUT PULONG Size OPTIONAL
    )

/*++

Routine Description:

    This function returns the data necessary to actually examine the
    contents of a particular resource, without allowing for the .mui
    feature. It used to be the tail of LdrpAccessResourceData, from
    which it is now called.

Arguments:

    DllHandle - Supplies a handle to the image file that the resource is
        contained in.

    ResourceDataEntry - Supplies a pointer to the resource data entry in
        the resource data directory of the image file specified by the
        DllHandle parameter.  This pointer should have been one returned
        by the LdrFindResource function.

    Address - Optional pointer to a variable that will receive the
        address of the resource specified by the first two parameters.

    Size - Optional pointer to a variable that will receive the size of
        the resource specified by the first two parameters.

--*/

{
    PIMAGE_RESOURCE_DIRECTORY ResourceDirectory;
    ULONG ResourceSize;
    PIMAGE_NT_HEADERS NtHeaders;
    ULONG_PTR VirtualAddressOffset;
    PIMAGE_SECTION_HEADER NtSection;
    NTSTATUS Status = STATUS_SUCCESS;

    RTL_PAGED_CODE();

    _SEH2_TRY {
        ResourceDirectory = (PIMAGE_RESOURCE_DIRECTORY)
            RtlImageDirectoryEntryToData(DllHandle,
                                         TRUE,
                                         IMAGE_DIRECTORY_ENTRY_RESOURCE,
                                         &ResourceSize
                                         );
        if (!ResourceDirectory) {
            return STATUS_RESOURCE_DATA_NOT_FOUND;
        }

        if (LDR_IS_DATAFILE(DllHandle)) {
            ULONG ResourceRVA;
            DllHandle = LDR_DATAFILE_TO_VIEW(DllHandle);
            NtHeaders = RtlImageNtHeader( DllHandle );
            if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
                ResourceRVA=((PIMAGE_NT_HEADERS32)NtHeaders)->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_RESOURCE ].VirtualAddress;
            } else if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
                ResourceRVA=((PIMAGE_NT_HEADERS64)NtHeaders)->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_RESOURCE ].VirtualAddress;
            } else {
                ResourceRVA = 0;
            }

            if (!ResourceRVA) {
                return STATUS_RESOURCE_DATA_NOT_FOUND;
                }

            VirtualAddressOffset = (ULONG_PTR)DllHandle + ResourceRVA - (ULONG_PTR)ResourceDirectory;

            //
            // Now, we must check to see if the resource is not in the
            // same section as the resource table.  If it's in .rsrc1,
            // we've got to adjust the RVA in the ResourceDataEntry
            // to point to the correct place in the non-VA data file.
            //
            NtSection = RtlSectionTableFromVirtualAddress( NtHeaders, DllHandle, ResourceRVA);

            if (!NtSection) {
                return STATUS_RESOURCE_DATA_NOT_FOUND;
            }

            if ( ResourceDataEntry->OffsetToData > NtSection->Misc.VirtualSize ) {
                ULONG rva;

                rva = NtSection->VirtualAddress;
                NtSection = RtlSectionTableFromVirtualAddress(NtHeaders,
                                                             DllHandle,
                                                             ResourceDataEntry->OffsetToData
                                                             );
                if (!NtSection) {
                    return STATUS_RESOURCE_DATA_NOT_FOUND;
                }
                VirtualAddressOffset +=
                        ((ULONG_PTR)NtSection->VirtualAddress - rva) -
                        ((ULONG_PTR)RtlAddressInSectionTable ( NtHeaders, DllHandle, NtSection->VirtualAddress ) - (ULONG_PTR)ResourceDirectory);
            }
        } else {
            VirtualAddressOffset = 0;
        }

        if (ARGUMENT_PRESENT( Address )) {
            *Address = (PVOID)( (PCHAR)DllHandle +
                                (ResourceDataEntry->OffsetToData - VirtualAddressOffset)
                              );
        }

        if (ARGUMENT_PRESENT( Size )) {
            *Size = ResourceDataEntry->Size;
        }

    }    _SEH2_EXCEPT (EXCEPTION_EXECUTE_HANDLER) {
        Status = GetExceptionCode();
    }

    return Status;
}

NTSTATUS
LdrpAccessResourceData(
    IN PVOID DllHandle,
    IN const IMAGE_RESOURCE_DATA_ENTRY* ResourceDataEntry,
    OUT PVOID *Address OPTIONAL,
    OUT PULONG Size OPTIONAL
    )

/*++

Routine Description:

    This function returns the data necessary to actually examine the
    contents of a particular resource.

Arguments:

    DllHandle - Supplies a handle to the image file that the resource is
        contained in.

    ResourceDataEntry - Supplies a pointer to the resource data entry in
   the resource data directory of the image file specified by the
        DllHandle parameter.  This pointer should have been one returned
        by the LdrFindResource function.

    Address - Optional pointer to a variable that will receive the
        address of the resource specified by the first two parameters.

    Size - Optional pointer to a variable that will receive the size of
        the resource specified by the first two parameters.

--*/

{
    PIMAGE_RESOURCE_DIRECTORY ResourceDirectory;
    ULONG ResourceSize;
    PIMAGE_NT_HEADERS NtHeaders;
    ULONG_PTR VirtualAddressOffset;
    PIMAGE_SECTION_HEADER NtSection;
	NTSTATUS Status = STATUS_SUCCESS;

    RTL_PAGED_CODE();

    ResourceDirectory = (PIMAGE_RESOURCE_DIRECTORY)
        RtlImageDirectoryEntryToData(DllHandle,
                                     TRUE,
                                     IMAGE_DIRECTORY_ENTRY_RESOURCE,
                                     &ResourceSize
                                     );
    if (!ResourceDirectory) {
        Status = STATUS_RESOURCE_DATA_NOT_FOUND;
    }

    if ((ULONG_PTR)ResourceDataEntry < (ULONG_PTR) ResourceDirectory ){
        DllHandle = LdrLoadAlternateResourceModule (DllHandle, NULL);
    } else{
        NtHeaders = RtlImageNtHeader(
                        (PVOID)((ULONG_PTR)DllHandle & ~0x00000001)
                        );
        if (NtHeaders) {
            // Find the bounds of the image so we can see if this resource entry is in an alternate
            // resource dll.

            ULONG_PTR ImageStart = (ULONG_PTR)DllHandle & ~0x00000001;
            SIZE_T ImageSize = 0;

            if ((ULONG_PTR)DllHandle & 0x00000001) {
                // mapped as datafile.  Ask mm for the size
                MEMORY_BASIC_INFORMATION MemInfo;

                Status = NtQueryVirtualMemory(
                            NtCurrentProcess(),
                            (PVOID) ImageStart,
                            MemoryBasicInformation,
                            (PVOID)&MemInfo,
                            sizeof(MemInfo),
                            NULL
                            );

                if ( !NT_SUCCESS(Status) ) {
                    ImageSize = 0;
                } else {
                    ImageSize = MemInfo.RegionSize;
                }
            } else {
                ImageSize = ((PIMAGE_NT_HEADERS32)NtHeaders)->OptionalHeader.SizeOfImage;
            }

            if (!(((ULONG_PTR)ResourceDataEntry >= ImageStart) && ((ULONG_PTR)ResourceDataEntry < (ImageStart + ImageSize)))) {
                // Doesn't fall within the specified image.  Must be an alternate dll.
                DllHandle = LdrLoadAlternateResourceModule (DllHandle, NULL);
            }
        }
    }

	if ( DllHandle )
	{
		Status = LdrpAccessResourceDataNoMultipleLanguage(DllHandle, ResourceDataEntry, Address, Size);
	}else{
        Status = STATUS_RESOURCE_DATA_NOT_FOUND;
    }
	return Status;
}


NTSTATUS
NTAPI
LdrAccessResource(
    IN PVOID DllHandle,
    IN PIMAGE_RESOURCE_DATA_ENTRY ResourceDataEntry,
    OUT PVOID *Address OPTIONAL,
    OUT PULONG Size OPTIONAL
    )

/*++

Routine Description:

    This function locates the address of the specified resource in the
    specified DLL and returns its address.

Arguments:

    DllHandle - Supplies a handle to the image file that the resource is
        contained in.

    ResourceDataEntry - Supplies a pointer to the resource data entry in
        the resource data section of the image file specified by the
        DllHandle parameter.  This pointer should have been one returned
        by the LdrFindResource function.

    Address - Optional pointer to a variable that will receive the
        address of the resource specified by the first two parameters.

    Size - Optional pointer to a variable that will receive the size of
        the resource specified by the first two parameters.

Return Value:

    TBD

--*/

{
    RTL_PAGED_CODE();

    return LdrpAccessResourceData(
      DllHandle,
      ResourceDataEntry,
      Address,
      Size
      );
}

int 
push_language( 	
	USHORT *  	list,
	ULONG  	pos,
	WORD  	lang 
) 		
{
    ULONG i;
    for (i = 0; i < pos; i++) 
		if (list[i] == lang) 
			return pos;
    list[pos++] = lang;
    return pos;
}

LONG
LdrpCompareResourceNames_U(
    IN ULONG_PTR ResourceName,
    IN PIMAGE_RESOURCE_DIRECTORY ResourceDirectory,
    IN PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirectoryEntry
    )
{
    LONG li;
    PIMAGE_RESOURCE_DIR_STRING_U ResourceNameString;

    if (ResourceName & LDR_RESOURCE_ID_NAME_MASK) {
        if (!ResourceDirectoryEntry->NameIsString) {
            return( -1 );
            }

        ResourceNameString = (PIMAGE_RESOURCE_DIR_STRING_U)
            ((PCHAR)ResourceDirectory + ResourceDirectoryEntry->NameOffset);

        li = wcsncmp( (LPWSTR)ResourceName,
            ResourceNameString->NameString,
            ResourceNameString->Length
          );

        if (!li && wcslen((PWSTR)ResourceName) != ResourceNameString->Length) {
       return( 1 );
       }

   return(li);
        }
    else {
        if (ResourceDirectoryEntry->NameIsString) {
            return( 1 );
            }

        return( (ULONG)(ResourceName - ResourceDirectoryEntry->Name) );
        }
}

PIMAGE_RESOURCE_DIRECTORY 
LdrpSearchResourceSection_U_by_id( 	
	PIMAGE_RESOURCE_DIRECTORY  	dir,
	WORD  	id,
	PVOID  	root,
	int  	want_dir 
) 		
{
    const IMAGE_RESOURCE_DIRECTORY_ENTRY *entry;
    int min, max, pos;

    entry = (const IMAGE_RESOURCE_DIRECTORY_ENTRY *)(dir + 1);
    min = dir->NumberOfNamedEntries;
    max = min + dir->NumberOfIdEntries - 1;
    while (min <= max)
    {
        pos = (min + max) / 2;
        if (entry[pos].Id == id)
        {
            if (!entry[pos].DataIsDirectory == !want_dir)
            {
                return (IMAGE_RESOURCE_DIRECTORY *)((char *)root + entry[pos].OffsetToDirectory);
            }
            break;
        }
        if (entry[pos].Id > id) max = pos - 1;
        else min = pos + 1;
    }
    return NULL;
}

PIMAGE_RESOURCE_DIRECTORY 
LdrpSearchResourceSection_U_by_name( 	
	PIMAGE_RESOURCE_DIRECTORY  	dir,
	LPCWSTR  	name,
	PVOID  	root,
	int  want_dir 
) 		
{
    const IMAGE_RESOURCE_DIRECTORY_ENTRY *entry;
    const IMAGE_RESOURCE_DIR_STRING_U *str;
    int min, max, res, pos;
    size_t namelen;

    if (!((ULONG_PTR)name & 0xFFFF0000)) return LdrpSearchResourceSection_U_by_id( dir, (ULONG_PTR)name & 0xFFFF, root, want_dir );
    entry = (const IMAGE_RESOURCE_DIRECTORY_ENTRY *)(dir + 1);
    namelen = wcslen(name);
    min = 0;
    max = dir->NumberOfNamedEntries - 1;
    while (min <= max)
    {
        pos = (min + max) / 2;
        str = (const IMAGE_RESOURCE_DIR_STRING_U *)((const char *)root + entry[pos].NameOffset);
        res = _wcsnicmp( name, str->NameString, str->Length );
        if (!res && namelen == str->Length)
        {
            if (!entry[pos].DataIsDirectory == !want_dir)
            {
                return (IMAGE_RESOURCE_DIRECTORY *)((PCHAR)root + entry[pos].OffsetToDirectory);
            }
            break;
        }
        if (res < 0) max = pos - 1;
        else min = pos + 1;
    }
    return NULL;
}

PIMAGE_RESOURCE_DIRECTORY 
find_first_entry( 	
	PIMAGE_RESOURCE_DIRECTORY  	dir,
	PVOID root,
	int want_dir 
) 		
{
    const IMAGE_RESOURCE_DIRECTORY_ENTRY *entry = (const IMAGE_RESOURCE_DIRECTORY_ENTRY *)(dir + 1);
    int pos;

    for (pos = 0; pos < dir->NumberOfNamedEntries + dir->NumberOfIdEntries; pos++)
    {
        if (!entry[pos].DataIsDirectory == !want_dir)
            return (IMAGE_RESOURCE_DIRECTORY *)((PCHAR)root + entry[pos].OffsetToDirectory);
    }
    return NULL;
}

NTSTATUS 
LdrpSearchResourceSection_U( 	
	PVOID  	BaseAddress,
	LDR_RESOURCE_INFO *  	info,
	ULONG  	level,
	PVOID *  	ret,
	int  	want_dir 
) 		
{
    ULONG size;
    PVOID root;
    PIMAGE_RESOURCE_DIRECTORY resdirptr;
    USHORT list[9];  /* list of languages to try */
    int i, pos = 0;
    LCID user_lcid, system_lcid;
	PVOID AlternateModule;
	NTSTATUS Status;

    root = RtlImageDirectoryEntryToData( BaseAddress, TRUE, IMAGE_DIRECTORY_ENTRY_RESOURCE, &size );
	
	AlternateModule = LdrLoadAlternateResourceModule(BaseAddress, NULL);
	if(AlternateModule)
	{
		Status = LdrpSearchResourceSection_U(AlternateModule,
							info,
							level,
							ret,
							want_dir);
		if(NT_SUCCESS(Status))
		{
			return Status;
		}
	}
	
    if (!root) 
		return STATUS_RESOURCE_DATA_NOT_FOUND;
    if (size < sizeof(*resdirptr)) 
		return STATUS_RESOURCE_DATA_NOT_FOUND;
    resdirptr = root;

    if (!level--) 
		goto done;
    if (!(*ret = LdrpSearchResourceSection_U_by_name( resdirptr, (LPCWSTR)info->Type, root, want_dir || level )))
        return STATUS_RESOURCE_TYPE_NOT_FOUND;
    if (!level--) 
		return STATUS_SUCCESS;

    resdirptr = *ret;
    if (!(*ret = LdrpSearchResourceSection_U_by_name( resdirptr, (LPCWSTR)info->Name, root, want_dir || level )))
        return STATUS_RESOURCE_NAME_NOT_FOUND;
    if (!level--) 
		return STATUS_SUCCESS;
    if (level) 
		return STATUS_INVALID_PARAMETER;  /* level > 3 */

    /* 1. specified language */
    pos = push_language( list, pos, info->Language );

    /* 2. specified language with neutral sublanguage */
    pos = push_language( list, pos, MAKELANGID( PRIMARYLANGID(info->Language), SUBLANG_NEUTRAL ) );

    /* 3. neutral language with neutral sublanguage */
    pos = push_language( list, pos, MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) );

    /* if no explicitly specified language, try some defaults */
    if (PRIMARYLANGID(info->Language) == LANG_NEUTRAL)
    {
        /* user defaults, unless SYS_DEFAULT sublanguage specified  */
        if (SUBLANGID(info->Language) != SUBLANG_SYS_DEFAULT)
        {
            /* 4. current thread locale language */
            pos = push_language( list, pos, LANGIDFROMLCID(NtCurrentTeb()->CurrentLocale) );

            if (NT_SUCCESS(NtQueryDefaultLocale(TRUE, &user_lcid)))
            {
                /* 5. user locale language */
                pos = push_language( list, pos, LANGIDFROMLCID(user_lcid) );

                /* 6. user locale language with neutral sublanguage  */
                pos = push_language( list, pos, MAKELANGID( PRIMARYLANGID(user_lcid), SUBLANG_NEUTRAL ) );
            }
        }

        /* now system defaults */

        if (NT_SUCCESS(NtQueryDefaultLocale(FALSE, &system_lcid)))
        {
            /* 7. system locale language */
            pos = push_language( list, pos, LANGIDFROMLCID( system_lcid ) );

            /* 8. system locale language with neutral sublanguage */
            pos = push_language( list, pos, MAKELANGID( PRIMARYLANGID(system_lcid), SUBLANG_NEUTRAL ) );
        }

        /* 9. English */
        pos = push_language( list, pos, MAKELANGID( LANG_ENGLISH, SUBLANG_DEFAULT ) );
    }

    resdirptr = *ret;
    for (i = 0; i < pos; i++)
        if ((*ret = LdrpSearchResourceSection_U_by_id( resdirptr, list[i], root, want_dir ))) return STATUS_SUCCESS;

    /* if no explicitly specified language, return the first entry */
    if (PRIMARYLANGID(info->Language) == LANG_NEUTRAL)
    {
        if ((*ret = find_first_entry( resdirptr, root, want_dir ))) return STATUS_SUCCESS;
    }
    return STATUS_RESOURCE_LANG_NOT_FOUND;

done:
    *ret = resdirptr;
    return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
LdrFindResource_U( 	
	PVOID  	BaseAddress,
	PLDR_RESOURCE_INFO  ResourceInfo,
	ULONG  	Level,
	PIMAGE_RESOURCE_DATA_ENTRY *  ResourceDataEntry 
) 		
{
    PVOID res;
    NTSTATUS status = STATUS_SUCCESS;

    _SEH2_TRY
    {
        status = LdrpSearchResourceSection_U( BaseAddress, ResourceInfo, Level, &res, FALSE );
        if (NT_SUCCESS(status))
            *ResourceDataEntry = res;
    }
    _SEH2_EXCEPT(page_fault(_SEH2_GetExceptionCode()))
    {
        status = _SEH2_GetExceptionCode();
    }
    _SEH2_END;
    return status;
}

NTSTATUS 
NTAPI 
LdrFindResourceDirectory_U( 	
	IN PVOID  	BaseAddress,
	IN PLDR_RESOURCE_INFO  	info,
	IN ULONG  	level,
	OUT PIMAGE_RESOURCE_DIRECTORY *  	addr 
) 	
{
    PVOID res;
    NTSTATUS status = STATUS_SUCCESS;

    _SEH2_TRY
    {
        status = LdrpSearchResourceSection_U( BaseAddress, info, level, &res, TRUE );
        if (NT_SUCCESS(status))
            *addr = res;
    }
    _SEH2_EXCEPT(page_fault(_SEH2_GetExceptionCode()))
    {
        status = _SEH2_GetExceptionCode();
    }
    _SEH2_END;
    return status;
}