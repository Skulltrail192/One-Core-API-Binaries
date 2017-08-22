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

#define  MEMBLOCKSIZE 32

#define  RESMODSIZE sizeof(ALT_RESOURCE_MODULE)

#define LDR_DATAFILE_TO_VIEW(x) ((PVOID)(((ULONG_PTR)(x)) & ~(ULONG_PTR)1))

#define LDR_FIND_RESOURCE_LANGUAGE_EXACT (0x00000004)

#define LDR_FIND_RESOURCE_LANGUAGE_REDIRECT_VERSION (0x00000008)

#define LDRP_FIND_RESOURCE_DIRECTORY (0x00000002)

#define MUI_NEUTRAL_LANGID  MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US )


LANGID UILangId, InstallLangId, DefaultLangId, ImpersonateLangId;

ULONG AlternateResourceModuleCount;

ULONG AltResMemBlockCount;

PALT_RESOURCE_MODULE AlternateResourceModules;

//
// This macro ensures that correct UI language will be retrieved.
//
#define GET_UI_LANGID()                                                            \
{                                                                                                       \
    if (!UILangId ||                                                                         \
        ImpersonateLangId ||                                                             \
        NtCurrentTeb()->IsImpersonating)                                          \
    {                                                                                                   \
        if (NT_SUCCESS( NtQueryDefaultUILanguage( &UILangId ) ))                    \
        {                                                                                                \
            ImpersonateLangId = NtCurrentTeb()->IsImpersonating? UILangId : 0;          \
        }                                                                                                \
    }                                                                                                    \
}       

NTSTATUS 
NTAPI
LdrpLoadResourceFromAlternativeModule(
   IN PVOID DllHandle,
   IN ULONG_PTR* ResourceIdPath,
   IN ULONG ResourceIdPathLength,
   IN ULONG Flags,
   OUT PVOID *ResourceDirectoryOrData );

BOOLEAN
NTAPI
LdrAlternateResourcesEnabled(
    VOID
    )
{
    return TRUE;
}

PVOID
NTAPI
LdrGetAlternateResourceModuleHandle(
    IN PVOID Module
    )
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
    ULONG DllPathNameLength, CopyCount;//,BaseDllNameLength;
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

    if (!LdrAlternateResourcesEnabled()) {
        return NULL;
    }

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
        //BaseDllNameLength = CopyCount * sizeof(WCHAR) - DllPathNameLength;

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
        //BaseDllNameLength = Entry->BaseDllName.Length;
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

#undef LdrLoadAlternateResourceModule

#define LdrLoadAlternateResourceModule(x,y) LdrLoadAlternateResourceModuleExt(x, y) 
PVOID
NTAPI
LdrLoadAlternateResourceModuleExt(
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

     _SEH2_TRY {
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
    _SEH2_EXCEPT (EXCEPTION_EXECUTE_HANDLER) {
        RtlLeaveCriticalSection((PRTL_CRITICAL_SECTION)NtCurrentPeb()->LoaderLock);
        return FALSE;
        }
	_SEH2_END 
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

    RTL_PAGED_CODE;

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

    }_SEH2_EXCEPT (EXCEPTION_EXECUTE_HANDLER) {
        Status = _SEH2_GetExceptionCode();
    }
	_SEH2_END

    return Status;
}

NTSTATUS
LdrpAccessResourceData(
    IN PVOID DllHandle,
    IN const IMAGE_RESOURCE_DATA_ENTRY* ResourceDataEntry,
    OUT PVOID *Address OPTIONAL,
    OUT PULONG Size OPTIONAL
    )
{
    PIMAGE_RESOURCE_DIRECTORY ResourceDirectory;
    ULONG ResourceSize;
    PIMAGE_NT_HEADERS NtHeaders;
    NTSTATUS Status = STATUS_SUCCESS;

    RTL_PAGED_CODE;

    ResourceDirectory = (PIMAGE_RESOURCE_DIRECTORY)
        RtlImageDirectoryEntryToData(DllHandle,
                                     TRUE,
                                     IMAGE_DIRECTORY_ENTRY_RESOURCE,
                                     &ResourceSize
                                     );
    if (!ResourceDirectory) {
        Status = STATUS_RESOURCE_DATA_NOT_FOUND;
        goto Exit;
    }

    if ((ULONG_PTR)ResourceDataEntry < (ULONG_PTR) ResourceDirectory ){
        DllHandle = LdrLoadAlternateResourceModule (DllHandle, NULL);
        if (!DllHandle){
            if (!InstallLangId){
               Status = NtQueryInstallUILanguage (&InstallLangId);

               if (!NT_SUCCESS( Status )) {
                goto Exit;
                }
            }
            if (InstallLangId != UILangId) {
                DllHandle = LdrLoadAlternateResourceModuleEx (InstallLangId, DllHandle, NULL);
            }
        }
    } else{
        NtHeaders = RtlImageNtHeader(LDR_DATAFILE_TO_VIEW(DllHandle));
        if (NtHeaders) {
            // Find the bounds of the image so we can see if this resource entry is in an alternate
            // resource dll.

            ULONG_PTR ImageStart = (ULONG_PTR)LDR_DATAFILE_TO_VIEW(DllHandle);
            SIZE_T ImageSize = 0;

            if (LDR_IS_DATAFILE(DllHandle)) {

                // mapped as datafile.  Ask mm for the size

                NTSTATUS xStatus;
                MEMORY_BASIC_INFORMATION MemInfo;

                xStatus = NtQueryVirtualMemory(
                            NtCurrentProcess(),
                            (PVOID) ImageStart,
                            MemoryBasicInformation,
                            &MemInfo,
                            sizeof(MemInfo),
                            NULL
                            );

                if ( !NT_SUCCESS(xStatus) ) {
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
            if (!DllHandle) {
                if (!InstallLangId){
                   Status = NtQueryInstallUILanguage (&InstallLangId);
                   if (!NT_SUCCESS( Status )) {
                        goto Exit;
                        }
                    }
                if (InstallLangId != UILangId) {
                    DllHandle = LdrLoadAlternateResourceModuleEx (InstallLangId, DllHandle, NULL);
                    }
                }
            }
        }
    }

    if (!DllHandle){
        Status = STATUS_RESOURCE_DATA_NOT_FOUND;
        goto Exit;
    }
    Status =
        LdrpAccessResourceDataNoMultipleLanguage(
            DllHandle,
            ResourceDataEntry,
            Address,
            Size
            );

Exit:
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

{
    RTL_PAGED_CODE;

    return LdrpAccessResourceData(
      DllHandle,
      ResourceDataEntry,
      Address,
      Size
      );
}

LONG
LdrpCompareResourceNames_U(
    IN ULONG_PTR ResourceName,
    IN const IMAGE_RESOURCE_DIRECTORY* ResourceDirectory,
    IN const IMAGE_RESOURCE_DIRECTORY_ENTRY* ResourceDirectoryEntry
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

// Language ids are 16bits so any value with any bits
// set above 16 should be ok, and this value only has
// to fit in a ULONG_PTR. 0x10000 should be sufficient.
// The value used is actually 0xFFFF regardless of 32bit or 64bit,
// I guess assuming this is not an actual langid, which it isn't,
// due to the relatively small number of languages, around 70.
#define  USE_FIRSTAVAILABLE_LANGID   (0xFFFFFFFF & ~LDR_RESOURCE_ID_NAME_MASK)
NTSTATUS
LdrpSearchResourceSection_U(
    IN PVOID DllHandle,
    IN const ULONG_PTR* ResourceIdPath,
    IN ULONG ResourceIdPathLength,
    IN ULONG Flags,
    OUT PVOID *ResourceDirectoryOrData
    )
{
    NTSTATUS Status;
    PIMAGE_RESOURCE_DIRECTORY LanguageResourceDirectory, ResourceDirectory, TopResourceDirectory;
    PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirEntLow;
    PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirEntMiddle;
    PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirEntHigh;
    PIMAGE_RESOURCE_DATA_ENTRY ResourceEntry;
    USHORT n, half;
    LONG dir;
    ULONG size;
    ULONG_PTR ResourceIdRetry;
    ULONG RetryCount;
    LANGID NewLangId;
    const ULONG_PTR* IdPath = ResourceIdPath;
    ULONG IdPathLength = ResourceIdPathLength;
    BOOLEAN fIsNeutral = FALSE;
    LANGID GivenLanguage;
    LCID DefaultThreadLocale, DefaultSystemLocale;
    PVOID AltResourceDllHandle = NULL;
    ULONG_PTR UIResourceIdPath[3];
	
    RTL_PAGED_CODE;

    _SEH2_TRY {
        TopResourceDirectory = (PIMAGE_RESOURCE_DIRECTORY)
            RtlImageDirectoryEntryToData(DllHandle,
                                         TRUE,
                                         IMAGE_DIRECTORY_ENTRY_RESOURCE,
                                         &size
                                         );
        if (!TopResourceDirectory) {
            return( STATUS_RESOURCE_DATA_NOT_FOUND );
        }

        ResourceDirectory = TopResourceDirectory;
        ResourceIdRetry = USE_FIRSTAVAILABLE_LANGID;
        RetryCount = 0;
        ResourceEntry = NULL;
        LanguageResourceDirectory = NULL;
        while (ResourceDirectory != NULL && ResourceIdPathLength--) {
            //
            // If search path includes a language id, then attempt to
            // match the following language ids in this order:
            //
            //   (0)  use given language id
            //   (1)  use primary language of given language id
            //   (2)  use id 0  (neutral resource)
            //   (4)  use user UI language
            //
            // If the PRIMARY language id is ZERO, then ALSO attempt to
            // match the following language ids in this order:
            //
            //   (3)  use thread language id for console app
            //   (4)  use user UI language
            //   (5)  use lang id of TEB for windows app if it is different from user locale
            //   (6)  use UI lang from exe resource
            //   (7)  use primary UI lang from exe resource
            //   (8)  use Install Language
            //   (9)  use lang id from user's locale id
            //   (10)  use primary language of user's locale id
            //   (11) use lang id from system default locale id
            //   (12) use lang id of system default locale id
            //   (13) use primary language of system default locale id
            //   (14) use US English lang id
            //   (15) use any lang id that matches requested info
            //
            if (ResourceIdPathLength == 0 && IdPathLength == 3) {
                LanguageResourceDirectory = ResourceDirectory;
                }

            if (LanguageResourceDirectory != NULL) {
                GivenLanguage = (LANGID)IdPath[ 2 ];
                fIsNeutral = (PRIMARYLANGID( GivenLanguage ) == LANG_NEUTRAL);
TryNextLangId:
                switch( RetryCount++ ) {
                    case 0:     // Use given language id
                        NewLangId = GivenLanguage;
                        break;

                    case 1:     // Use primary language of given language id
                        if ( Flags & LDR_FIND_RESOURCE_LANGUAGE_EXACT) {
                            //
                            //  Did not find an exact language match.
                            //  Stop looking.
                            //
                            goto ReturnFailure;
                        }
                        NewLangId = PRIMARYLANGID( GivenLanguage );
                        break;

                    case 2:     // Use id 0  (neutral resource)
                        NewLangId = 0;
                        break;

                    case 3:     // Use thread langid if caller is a console app
                        if ( !fIsNeutral ) {
                            // Stop looking - Not in the neutral case
                            NewLangId = (LANGID)ResourceIdRetry;
                            break;
                        }

                        if (NtCurrentPeb()->ProcessParameters->ConsoleHandle)
                        {
                            NewLangId = LANGIDFROMLCID(NtCurrentTeb()->CurrentLocale);
                        }
                        else
                        {
                            NewLangId = (LANGID)ResourceIdRetry;
                        }
                        break;

                    case 4:     // Use user's default UI language
                        GET_UI_LANGID();
                        if (!UILangId){
                            NewLangId = (LANGID)ResourceIdRetry;
                            break;
                        }

                        // Don't load alternative modules for console process if UI language doesn't match system locale
                        if (NtCurrentPeb()->ProcessParameters->ConsoleHandle &&
                            LANGIDFROMLCID(NtCurrentTeb()->CurrentLocale) != UILangId)
                        {
                            NewLangId = (LANGID)ResourceIdRetry;
                            break;
                        }

                        NewLangId = UILangId;

                        //
                        // Arabic/Hebrew MUI files may contain resources with LANG ID different than 401/40d.
                        // e.g. Comdlg32.dll has two sets of Arabic/Hebrew resources one mirrored (401/40d)
                        // and one flipped (801/80d).
                        //
                        if( !fIsNeutral &&
                            ((PRIMARYLANGID (GivenLanguage) == LANG_ARABIC) || (PRIMARYLANGID (GivenLanguage) == LANG_HEBREW)) &&
                            (PRIMARYLANGID (GivenLanguage) == PRIMARYLANGID (NewLangId))) {
                                NewLangId = GivenLanguage;
                        }

                            //
                            // Bug #246044 WeiWu 12/07/00
                            // BiDi modules use version block FileDescription field to store LRM markers,
                            // LDR_FIND_RESOURCE_LANGUAGE_REDIRECT_VERSION will allow lpk.dll to get version resource from MUI alternative modules.
                            //
                        if ( ( (IdPath[0] != (ULONG_PTR)RT_VERSION) && (IdPath[0] != (ULONG_PTR)RT_MANIFEST) ) ||
                            (Flags & LDR_FIND_RESOURCE_LANGUAGE_REDIRECT_VERSION)) {
                            //
                            //  Load alternate resource dll when:
                            //      1. language is neutral
                            //         or
                            //         Given language is not tried.
                            //      and
                            //      2. the resource to load is not a version info.
                            //
                            AltResourceDllHandle=LdrLoadAlternateResourceModule(
                                                    DllHandle,
                                                    NULL);

                            if (!AltResourceDllHandle){
                                //
                                //  Alternate resource dll not available.
                                //  Skip this lookup.
                                //
                                NewLangId = (LANGID)ResourceIdRetry;
                                break;
                            }

                            //
                            //  Map to alternate resource dll and search
                            //  it instead.
                            //

                            UIResourceIdPath[0]=IdPath[0];
                            UIResourceIdPath[1]=IdPath[1];
                            UIResourceIdPath[2]=NewLangId;

                            Status = LdrpSearchResourceSection_U(
                                        AltResourceDllHandle,
                                        UIResourceIdPath,
                                        3,
                                        Flags | LDR_FIND_RESOURCE_LANGUAGE_EXACT,
                                        (PVOID *)ResourceDirectoryOrData
                                        );

                            if (NT_SUCCESS(Status)){
                                //
                                // We sucessfully found alternate resource,
                                // return it.
                                //
                                return Status;
                            }
                        }
                        //
                        //  Caller does not want alternate resource, or
                        //  alternate resource not found.
                        //
                        NewLangId = (LANGID)ResourceIdRetry;
                        break;

                    case 5:     // Use langid of the thread locale if caller is a Windows app and thread locale is different from user locale
                        if ( !fIsNeutral ) {
                            // Stop looking - Not in the neutral case
                            goto ReturnFailure;
                            break;
                        }

                        if (!NtCurrentPeb()->ProcessParameters->ConsoleHandle && NtCurrentTeb()){
                            Status = NtQueryDefaultLocale(
                                        TRUE,
                                        &DefaultThreadLocale
                                        );
                            if (NT_SUCCESS( Status ) &&
                                DefaultThreadLocale !=
                                NtCurrentTeb()->CurrentLocale) {
                                //
                                // Thread locale is different from
                                // default locale.
                                //
                                NewLangId = LANGIDFROMLCID(NtCurrentTeb()->CurrentLocale);
                                break;
                            }
                        }


                        NewLangId = (LANGID)ResourceIdRetry;
                        break;

                    case 6:   // UI language from the executable resource

                        if (!UILangId){
                            NewLangId = (LANGID)ResourceIdRetry;
                        } else {
                            NewLangId = UILangId;
                        }
                        break;

                    case 7:   // Parimary lang of UI language from the executable resource

                        if (!UILangId){
                            NewLangId = (LANGID)ResourceIdRetry;
                        } else {
                            NewLangId = PRIMARYLANGID( (LANGID) UILangId );
                        }
                        break;

                    case 8:   // Use install -native- language
                        //
                        // Thread locale is the same as the user locale, then let's
                        // try loading the native (install) ui language resources.
                        //
                        if (!InstallLangId){
                            Status = NtQueryInstallUILanguage(&InstallLangId);
                            if (!NT_SUCCESS( Status )) {
                                //
                                // Failed reading key.  Skip this lookup.
                                //
                                NewLangId = (LANGID)ResourceIdRetry;
                                break;

                            }
                        }

                        NewLangId = InstallLangId;
                        break;

                    case 9:     // Use lang id from locale in TEB
                        if (SUBLANGID( GivenLanguage ) == SUBLANG_SYS_DEFAULT) {
                            // Skip over all USER locale options
                            DefaultThreadLocale = 0;
                            RetryCount += 2;
                            break;
                        }

                        if (NtCurrentTeb() != NULL) {
                            NewLangId = LANGIDFROMLCID(NtCurrentTeb()->CurrentLocale);
                        }
                        break;

                    case 10:     // Use User's default locale
                        Status = NtQueryDefaultLocale( TRUE, &DefaultThreadLocale );
                        if (NT_SUCCESS( Status )) {
                            NewLangId = LANGIDFROMLCID(DefaultThreadLocale);
                            break;
                            }

                        RetryCount++;
                        break;

                    case 11:     // Use primary language of User's default locale
                        NewLangId = PRIMARYLANGID( (LANGID)ResourceIdRetry );
                        break;

                    case 12:     // Use System default locale
                        Status = NtQueryDefaultLocale( FALSE, &DefaultSystemLocale );
                        if (!NT_SUCCESS( Status )) {
                            RetryCount++;
                            break;
                        }
                        if (DefaultSystemLocale != DefaultThreadLocale) {
                            NewLangId = LANGIDFROMLCID(DefaultSystemLocale);
                            break;
                        }

                        RetryCount += 2;
                        // fall through

                    case 14:     // Use US English language
                        NewLangId = MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US );
                        break;

                    case 13:     // Use primary language of System default locale
                        NewLangId = PRIMARYLANGID( (LANGID)ResourceIdRetry );
                        break;

                    case 15:     // Take any lang id that matches
                        NewLangId = USE_FIRSTAVAILABLE_LANGID;
                        break;
                    default:    // No lang ids to match
                        goto ReturnFailure;
                        break;
                }

                //
                // If looking for a specific language id and same as the
                // one we just looked up, then skip it.
                //
                if (NewLangId != USE_FIRSTAVAILABLE_LANGID &&
                    NewLangId == ResourceIdRetry
                   ) {
                    goto TryNextLangId;
                    }

                //
                // Try this new language Id
                //
                ResourceIdRetry = (ULONG_PTR)NewLangId;
                ResourceIdPath = &ResourceIdRetry;
                ResourceDirectory = LanguageResourceDirectory;
                }

            n = ResourceDirectory->NumberOfNamedEntries;
            ResourceDirEntLow = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(ResourceDirectory+1);
            if (!(*ResourceIdPath & LDR_RESOURCE_ID_NAME_MASK)) { // No string(name),so we need ID
                ResourceDirEntLow += n;
                n = ResourceDirectory->NumberOfIdEntries;
                }

            if (!n) {
                ResourceDirectory = NULL;
                goto NotFound;  // Resource directory contains zero types or names or langID.
                }

            if (LanguageResourceDirectory != NULL &&
                *ResourceIdPath == USE_FIRSTAVAILABLE_LANGID
               ) {
                ResourceDirectory = NULL;
                ResourceIdRetry = ResourceDirEntLow->Name;
                ResourceEntry = (PIMAGE_RESOURCE_DATA_ENTRY)
                    ((PCHAR)TopResourceDirectory +
                            ResourceDirEntLow->OffsetToData
                    );

                break;
                }

            ResourceDirectory = NULL;
            ResourceDirEntHigh = ResourceDirEntLow + n - 1;
            while (ResourceDirEntLow <= ResourceDirEntHigh) {
                if ((half = (n >> 1)) != 0) {
                    ResourceDirEntMiddle = ResourceDirEntLow;
                    if (*(PUCHAR)&n & 1) {
                        ResourceDirEntMiddle += half;
                        }
                    else {
                        ResourceDirEntMiddle += half - 1;
                        }
                    dir = LdrpCompareResourceNames_U( *ResourceIdPath,
                                                      TopResourceDirectory,
                                                      ResourceDirEntMiddle
                                                    );
                    if (!dir) {
                        if (ResourceDirEntMiddle->DataIsDirectory) {
                            ResourceDirectory = (PIMAGE_RESOURCE_DIRECTORY)
                    ((PCHAR)TopResourceDirectory +
                                    ResourceDirEntMiddle->OffsetToDirectory
                                );
                            }
                        else {
                            ResourceDirectory = NULL;
                            ResourceEntry = (PIMAGE_RESOURCE_DATA_ENTRY)
                                ((PCHAR)TopResourceDirectory +
                                 ResourceDirEntMiddle->OffsetToData
                                );
                            }

                        break;
                        }
                    else {
                        if (dir < 0) {  // Order in the resource: Name, ID.
                            ResourceDirEntHigh = ResourceDirEntMiddle - 1;
                            if (*(PUCHAR)&n & 1) {
                                n = half;
                                }
                            else {
                                n = half - 1;
                                }
                            }
                        else {
                            ResourceDirEntLow = ResourceDirEntMiddle + 1;
                            n = half;
                            }
                        }
                    }
                else {
                    if (n != 0) {
                        dir = LdrpCompareResourceNames_U( *ResourceIdPath,
                          TopResourceDirectory,
                                                          ResourceDirEntLow
                                                        );
                        if (!dir) {   // find, or it fail to set ResourceDirectory so go to NotFound.
                            if (ResourceDirEntLow->DataIsDirectory) {
                                ResourceDirectory = (PIMAGE_RESOURCE_DIRECTORY)
                                    ((PCHAR)TopResourceDirectory +
                                        ResourceDirEntLow->OffsetToDirectory
                                    );
                                }
                            else {
                                ResourceEntry = (PIMAGE_RESOURCE_DATA_ENTRY)
                                    ((PCHAR)TopResourceDirectory +
                      ResourceDirEntLow->OffsetToData
                                    );
                                }
                            }
                        }

                    break;
                    }
                }

            ResourceIdPath++;
            }

        if (ResourceEntry != NULL && !(Flags & LDRP_FIND_RESOURCE_DIRECTORY)) {
            *ResourceDirectoryOrData = (PVOID)ResourceEntry;
            Status = STATUS_SUCCESS;
            }
        else
        if (ResourceDirectory != NULL && (Flags & LDRP_FIND_RESOURCE_DIRECTORY)) {
            *ResourceDirectoryOrData = (PVOID)ResourceDirectory;
            Status = STATUS_SUCCESS;
            }
        else {
NotFound:
            switch( IdPathLength - ResourceIdPathLength) {
                case 3:     Status = STATUS_RESOURCE_LANG_NOT_FOUND; break;
                case 2:     Status = STATUS_RESOURCE_NAME_NOT_FOUND; break;
                case 1:     Status = STATUS_RESOURCE_TYPE_NOT_FOUND; break;
                default:    Status = STATUS_INVALID_PARAMETER; break;
                }
            }

        if (Status == STATUS_RESOURCE_LANG_NOT_FOUND &&
            LanguageResourceDirectory != NULL
           ) {
            ResourceEntry = NULL;
            goto TryNextLangId;
ReturnFailure: ;
            Status = STATUS_RESOURCE_LANG_NOT_FOUND;
            }
        //
        // Load resource from alternative modules if
        // resource type doesn't exist in the main DLL.
        //
        else if (Status == STATUS_RESOURCE_TYPE_NOT_FOUND &&
            3 == IdPathLength ) {
                GET_UI_LANGID();

                if (UILangId) {
                    UIResourceIdPath[0]=IdPath[0];
                    UIResourceIdPath[1]=IdPath[1];
                    UIResourceIdPath[2]=UILangId;

//                  ASSERT (IdPath[0] != RT_VERSION);
                    if (IdPath[0] != (ULONG_PTR)RT_MANIFEST && IdPath[0] != (ULONG_PTR)RT_VERSION) {
                        Status = LdrpLoadResourceFromAlternativeModule(DllHandle,
                                    UIResourceIdPath,
                                    3,
                                    Flags,
                                    ResourceDirectoryOrData );
                    } else {
                        }

                }
            }
        }
    _SEH2_EXCEPT (EXCEPTION_EXECUTE_HANDLER) {
        Status = _SEH2_GetExceptionCode();
        }

	_SEH2_END
    return Status;
}

NTSTATUS 
NTAPI
LdrpLoadResourceFromAlternativeModule(
   IN PVOID DllHandle,
   IN ULONG_PTR* ResourceIdPath,
   IN ULONG ResourceIdPathLength,
   IN ULONG Flags,
   OUT PVOID *ResourceDirectoryOrData )
{
    NTSTATUS Status = STATUS_RESOURCE_DATA_NOT_FOUND;
    PVOID AltResourceDllHandle = NULL;
    LANGID MUIDirLang;

    if (3 != ResourceIdPathLength)
        return Status;

    GET_UI_LANGID();

    if (!UILangId){
        return Status;
    }

    if (!InstallLangId){
        Status = NtQueryInstallUILanguage (&InstallLangId);
        if (!NT_SUCCESS( Status )) {
            //
            //  Failed to get Install LangID.  AltResource not enabled.
            //
            return FALSE;
        }
    }


    if (ResourceIdPath[2]) {
        //
        // Arabic/Hebrew MUI files may contain resources with LANG ID different than 401/40d.
        // e.g. Comdlg32.dll has two sets of Arabic/Hebrew resources one mirrored (401/40d)
        // and one flipped (801/80d).
        //
        if( (ResourceIdPath[2] != UILangId) &&
            ((PRIMARYLANGID ( UILangId) == LANG_ARABIC) || (PRIMARYLANGID ( UILangId) == LANG_HEBREW)) &&
            (PRIMARYLANGID (ResourceIdPath[2]) == PRIMARYLANGID (UILangId))
          ) {
            ResourceIdPath[2] = UILangId;
        }
    }
    else {
        ResourceIdPath[2] = UILangId;
    }

    // Don't load alternative modules for console process if UI language doesn't match system locale
    // In this case, we always load English
    if (NtCurrentPeb()->ProcessParameters->ConsoleHandle &&
        LANGIDFROMLCID(NtCurrentTeb()->CurrentLocale) != ResourceIdPath[2])
    {
        ResourceIdPath[2] = MUI_NEUTRAL_LANGID;
        UILangId = MUI_NEUTRAL_LANGID;
    }

        //
        // Bug #246044 WeiWu 12/07/00
        // BiDi modules use version block FileDescription field to store LRM markers,
        // LDR_FIND_RESOURCE_LANGUAGE_REDIRECT_VERSION will allow lpk.dll to get version resource from MUI alternative modules.
        //
    if ((ResourceIdPath[0] != (ULONG_PTR)RT_VERSION) ||
        (Flags & LDR_FIND_RESOURCE_LANGUAGE_REDIRECT_VERSION)) {

RESOURCE_TRY_AGAIN:
        //
        //  Load alternate resource dll when:
        //      1. language is neutral
        //         or
        //         Given language is not tried.
        //      and
        //      2. the resource to load is not a version info.
        //
        AltResourceDllHandle=LdrLoadAlternateResourceModuleEx(
                                (LANGID) ResourceIdPath[2],
                                DllHandle,
                                NULL);

        if (!AltResourceDllHandle){
            //
            //  Alternate resource dll not available.
            //  Skip this lookup.
            //
            if ((LANGID) ResourceIdPath[2] != InstallLangId){
                ResourceIdPath[2] = InstallLangId;
                //UILangId = InstallLangId;
                goto RESOURCE_TRY_AGAIN;
                }
            else {
               return Status;
            }
        }
        //
        // Add fallback steps here for alternative module search
        // 1) Given langid
        // 2) Primary langid of given langid if 2 != 1
        // 3) System installed langid if 3 != 1
        //
        MUIDirLang = (LANGID)ResourceIdPath[2];

SearchResourceSection:
        Status = LdrpSearchResourceSection_U(
                    AltResourceDllHandle,
                    ResourceIdPath,
                    3,
                    Flags | LDR_FIND_RESOURCE_LANGUAGE_EXACT,
                    (PVOID *)ResourceDirectoryOrData
                    );
        if (!NT_SUCCESS(Status) ) {
               if ((LANGID) ResourceIdPath[2] != 0x409) {
                    // some English components are not localized. but this unlocalized mui file
                    // is saved under \mui\fallback\%LocalizedLang%\ so we just repeat search.
                    // this is a temporary hack, we'll have a better solution
                    ResourceIdPath[2] = 0x409;
                    goto SearchResourceSection;
               }

               if ( (LANGID) MUIDirLang != InstallLangId) {
                    ResourceIdPath[2] = InstallLangId;
                    goto RESOURCE_TRY_AGAIN;
                }
            }
       }

    return Status;
}

#undef LdrFindResource_U

#define LdrFindResource_U(x, y, w, z) LdrFindResource_U_Ext(x, y, w, z) 

NTSTATUS
NTAPI
LdrFindResource_U_Ext(
    IN PVOID DllHandle,
    IN const ULONG_PTR* ResourceIdPath,
    IN ULONG ResourceIdPathLength,
    OUT PIMAGE_RESOURCE_DATA_ENTRY *ResourceDataEntry
    )
{
    RTL_PAGED_CODE;

    return LdrpSearchResourceSection_U(
      DllHandle,
      ResourceIdPath,
      ResourceIdPathLength,
      0,                // Look for a leaf node, ineaxt lang match
      (PVOID *)ResourceDataEntry
      );
}

NTSTATUS
NTAPI
LdrFindResourceEx_U(
    IN ULONG Flags,
    IN PVOID DllHandle,
    IN const ULONG_PTR* ResourceIdPath,
    IN ULONG ResourceIdPathLength,
    OUT PIMAGE_RESOURCE_DATA_ENTRY *ResourceDataEntry
    )
{
    RTL_PAGED_CODE;

    return LdrpSearchResourceSection_U(
      DllHandle,
      ResourceIdPath,
      ResourceIdPathLength,
      Flags,
      (PVOID *)ResourceDataEntry
      );
}

#undef LdrLoadAlternateResourceModule

#define LdrFindResourceDirectory_U(x, y, w, z) LdrFindResourceDirectory_U_EX(x, y, w, z) 

NTSTATUS
NTAPI
LdrFindResourceDirectory_U_EX(
    IN PVOID DllHandle,
    IN const ULONG_PTR* ResourceIdPath,
    IN ULONG ResourceIdPathLength,
    OUT PIMAGE_RESOURCE_DIRECTORY *ResourceDirectory
    )
{
    RTL_PAGED_CODE;

    return LdrpSearchResourceSection_U(
      DllHandle,
      ResourceIdPath,
      ResourceIdPathLength,
      LDRP_FIND_RESOURCE_DIRECTORY,                 // Look for a directory node
      (PVOID *)ResourceDirectory
      );
}