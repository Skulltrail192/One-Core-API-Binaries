/*
 * PE file resources
 *
 * Copyright 1995 Thomas Sandford
 * Copyright 1996 Martin von Loewis
 * Copyright 2003 Alexandre Julliard
 * Copyright 1993 Robert J. Amstadt
 * Copyright 1997 Marcus Meissner
 *
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* INCLUDES *****************************************************************/

#include <rtl.h>

#define NDEBUG
#include <debug.h>
#include <..\..\include\reactos\wine\exception.h>

/*From Windows Research Kernel Source Code*/
#define LDRP_FIND_RESOURCE_DATA                 (0x00000000)
#define LDRP_FIND_RESOURCE_DIRECTORY            (0x00000002)
#define LDR_RESOURCE_ID_NAME_MASK   ((~(ULONG_PTR)0) << 16) /* lower 16bits clear */
#define LDR_RESOURCE_ID_NAME_MINVAL (( (ULONG_PTR)1) << 16) /* 17th bit set */

LANGID PsDefaultUILanguageId = 0;
LANGID PsInstallUILanguageId = 0;
LCID PsDefaultSystemLocaleId = 0;

// Language ids are 16bits so any value with any bits
// set above 16 should be ok, and this value only has
// to fit in a ULONG_PTR. 0x10000 should be sufficient.
// The value used is actually 0xFFFF regardless of 32bit or 64bit,
// I guess assuming this is not an actual langid, which it isn't,
// due to the relatively small number of languages, around 70.
#define  USE_FIRSTAVAILABLE_LANGID   (0xFFFFFFFF & ~LDR_RESOURCE_ID_NAME_MASK)

NTSTATUS find_entry( PVOID BaseAddress, LDR_RESOURCE_INFO *info,
                     ULONG level, void **ret, int want_dir );

/* FUNCTIONS ****************************************************************/

int page_fault(ULONG ExceptionCode)
{
    if (ExceptionCode == EXCEPTION_ACCESS_VIOLATION ||
        ExceptionCode == EXCEPTION_PRIV_INSTRUCTION)
        return EXCEPTION_EXECUTE_HANDLER;
    return EXCEPTION_CONTINUE_SEARCH;
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


/**********************************************************************
 *  push_language
 *
 * push a language in the list of languages to try
 */
int push_language( USHORT *list, ULONG pos, WORD lang )
{
    ULONG i;
    for (i = 0; i < pos; i++) if (list[i] == lang) return pos;
    list[pos++] = lang;
    return pos;
}


/**********************************************************************
 *  find_first_entry
 *
 * Find the first suitable entry in a resource directory
 */
IMAGE_RESOURCE_DIRECTORY *find_first_entry( IMAGE_RESOURCE_DIRECTORY *dir,
                                            void *root, int want_dir )
{
    const IMAGE_RESOURCE_DIRECTORY_ENTRY *entry = (const IMAGE_RESOURCE_DIRECTORY_ENTRY *)(dir + 1);
    int pos;

    for (pos = 0; pos < dir->NumberOfNamedEntries + dir->NumberOfIdEntries; pos++)
    {
        if (!entry[pos].DataIsDirectory == !want_dir)
            return (IMAGE_RESOURCE_DIRECTORY *)((char *)root + entry[pos].OffsetToDirectory);
    }
    return NULL;
}


/**********************************************************************
 *  find_entry_by_id
 *
 * Find an entry by id in a resource directory
 */
IMAGE_RESOURCE_DIRECTORY *find_entry_by_id( IMAGE_RESOURCE_DIRECTORY *dir,
                                            WORD id, void *root, int want_dir )
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
                DPRINT("root %p dir %p id %04x ret %p\n",
                       root, dir, id, (const char*)root + entry[pos].OffsetToDirectory);
                return (IMAGE_RESOURCE_DIRECTORY *)((char *)root + entry[pos].OffsetToDirectory);
            }
            break;
        }
        if (entry[pos].Id > id) max = pos - 1;
        else min = pos + 1;
    }
    DPRINT("root %p dir %p id %04x not found\n", root, dir, id );
    return NULL;
}


/**********************************************************************
 *  find_entry_by_name
 *
 * Find an entry by name in a resource directory
 */
IMAGE_RESOURCE_DIRECTORY *find_entry_by_name( IMAGE_RESOURCE_DIRECTORY *dir,
                                              LPCWSTR name, void *root,
                                              int want_dir )
{
    const IMAGE_RESOURCE_DIRECTORY_ENTRY *entry;
    const IMAGE_RESOURCE_DIR_STRING_U *str;
    int min, max, res, pos;
    size_t namelen;

    if (!((ULONG_PTR)name & 0xFFFF0000)) return find_entry_by_id( dir, (ULONG_PTR)name & 0xFFFF, root, want_dir );
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
                DPRINT("root %p dir %p name %ws ret %p\n",
                       root, dir, name, (const char*)root + entry[pos].OffsetToDirectory);
                return (IMAGE_RESOURCE_DIRECTORY *)((char *)root + entry[pos].OffsetToDirectory);
            }
            break;
        }
        if (res < 0) max = pos - 1;
        else min = pos + 1;
    }
    DPRINT("root %p dir %p name %ws not found\n", root, dir, name);
    return NULL;
}

#ifdef __i386__
NTSTATUS NTAPI LdrpAccessResource( PVOID BaseAddress, IMAGE_RESOURCE_DATA_ENTRY *entry,
                                   void **ptr, ULONG *size )
#else
static NTSTATUS LdrpAccessResource( PVOID BaseAddress, IMAGE_RESOURCE_DATA_ENTRY *entry,
                                    void **ptr, ULONG *size )
#endif
{
    NTSTATUS status = STATUS_SUCCESS;

    _SEH2_TRY
    {
        ULONG dirsize;

        if (!RtlImageDirectoryEntryToData( BaseAddress, TRUE, IMAGE_DIRECTORY_ENTRY_RESOURCE, &dirsize ))
            status = STATUS_RESOURCE_DATA_NOT_FOUND;
        else
        {
            if (ptr)
            {
                if (is_data_file_module(BaseAddress))
                {
                    PVOID mod = (PVOID)((ULONG_PTR)BaseAddress & ~1);
                    *ptr = RtlImageRvaToVa( RtlImageNtHeader(mod), mod, entry->OffsetToData, NULL );
                }
                else *ptr = (char *)BaseAddress + entry->OffsetToData;
            }
            if (size) *size = entry->Size;
        }
    }
    _SEH2_EXCEPT(page_fault(_SEH2_GetExceptionCode()))
    {
        status = _SEH2_GetExceptionCode();
    }
    _SEH2_END;
    return status;
}


/*
 * @implemented
 */
NTSTATUS NTAPI
LdrFindResource_U(PVOID BaseAddress,
                  PLDR_RESOURCE_INFO ResourceInfo,
                  ULONG Level,
                  PIMAGE_RESOURCE_DATA_ENTRY* ResourceDataEntry)
{
    void *res;
    NTSTATUS status = STATUS_SUCCESS;

    _SEH2_TRY
    {
        if (ResourceInfo)
        {
            DPRINT( "module %p type %lx name %lx lang %04lx level %lu\n",
                     BaseAddress, ResourceInfo->Type, 
                     Level > 1 ? ResourceInfo->Name : 0,
                     Level > 2 ? ResourceInfo->Language : 0, Level );
        }

        status = find_entry( BaseAddress, ResourceInfo, Level, &res, FALSE );
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

#ifndef __i386__
/*
 * @implemented
 */
NTSTATUS NTAPI
LdrAccessResource(IN  PVOID BaseAddress,
                  IN  PIMAGE_RESOURCE_DATA_ENTRY ResourceDataEntry,
                  OUT PVOID* Resource OPTIONAL,
                  OUT PULONG Size OPTIONAL)
{
    return LdrpAccessResource( BaseAddress, ResourceDataEntry, Resource, Size );
}
#endif

/*
 * @implemented
 */
NTSTATUS NTAPI
LdrFindResourceDirectory_U(IN PVOID BaseAddress,
                           IN PLDR_RESOURCE_INFO info,
                           IN ULONG level,
                           OUT PIMAGE_RESOURCE_DIRECTORY* addr)
{
    void *res;
    NTSTATUS status = STATUS_SUCCESS;

    _SEH2_TRY
    {
        if (info)
        {
            DPRINT( "module %p type %ws name %ws lang %04lx level %lu\n",
                     BaseAddress, (LPCWSTR)info->Type,
                     level > 1 ? (LPCWSTR)info->Name : L"",
                     level > 2 ? info->Language : 0, level );
        }

        status = find_entry( BaseAddress, info, level, &res, TRUE );
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


#define NAME_FROM_RESOURCE_ENTRY(RootDirectory, Entry) \
    ((Entry)->NameIsString ? (ULONG_PTR)(RootDirectory) + (Entry)->NameOffset : (Entry)->Id)

static
LONG
LdrpCompareResourceNames_U(
    _In_ PUCHAR ResourceData,
    _In_ PIMAGE_RESOURCE_DIRECTORY_ENTRY Entry,
    _In_ ULONG_PTR CompareName)
{
    PIMAGE_RESOURCE_DIR_STRING_U ResourceString;
    PWSTR String1, String2;
    USHORT ResourceStringLength;
    WCHAR Char1, Char2;

    /* Check if the resource name is an ID */
    if (CompareName <= USHRT_MAX)
    {
        /* Just compare the 2 IDs */
        return (CompareName - Entry->Id);
    }
    else
    {
        /* Get the resource string */
        ResourceString = (PIMAGE_RESOURCE_DIR_STRING_U)(ResourceData +
                                                        Entry->NameOffset);

        /* Get the string length */
        ResourceStringLength = ResourceString->Length;

        String1 = ResourceString->NameString;
        String2 = (PWSTR)CompareName;

        /* Loop all characters of the resource string */
        while (ResourceStringLength--)
        {
            /* Get the next characters */
            Char1 = *String1++;
            Char2 = *String2++;

            /* Check if they don't match, or if the compare string ends */
            if ((Char1 != Char2) || (Char2 == 0))
            {
                /* They don't match, fail */
                return Char2 - Char1;
            }
        }

        /* All characters match, check if the compare string ends here */
        return (*String2 == 0) ? 0 : 1;
    }
}

NTSTATUS
NTAPI
LdrEnumResources(
    _In_ PVOID ImageBase,
    _In_ PLDR_RESOURCE_INFO ResourceInfo,
    _In_ ULONG Level,
    _Inout_ ULONG *ResourceCount,
    _Out_writes_to_(*ResourceCount,*ResourceCount) LDR_ENUM_RESOURCE_INFO *Resources)
{
    PUCHAR ResourceData;
    NTSTATUS Status;
    ULONG i, j, k;
    ULONG NumberOfTypeEntries, NumberOfNameEntries, NumberOfLangEntries;
    ULONG Count, MaxResourceCount;
    PIMAGE_RESOURCE_DIRECTORY TypeDirectory, NameDirectory, LangDirectory;
    PIMAGE_RESOURCE_DIRECTORY_ENTRY TypeEntry, NameEntry, LangEntry;
    PIMAGE_RESOURCE_DATA_ENTRY DataEntry;
    ULONG Size;
    LONG Result;

    /* If the caller wants data, get the maximum count of entries */
    MaxResourceCount = (Resources != NULL) ? *ResourceCount : 0;

    /* Default to 0 */
    *ResourceCount = 0;

    /* Locate the resource directory */
    ResourceData = RtlImageDirectoryEntryToData(ImageBase,
                                                TRUE,
                                                IMAGE_DIRECTORY_ENTRY_RESOURCE,
                                                &Size);
    if (ResourceData == NULL)
        return STATUS_RESOURCE_DATA_NOT_FOUND;

    /* The type directory is at the root, followed by the entries */
    TypeDirectory = (PIMAGE_RESOURCE_DIRECTORY)ResourceData;
    TypeEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(TypeDirectory + 1);

    /* Get the number of entries in the type directory */
    NumberOfTypeEntries = TypeDirectory->NumberOfNamedEntries +
                          TypeDirectory->NumberOfIdEntries;

    /* Start with 0 resources and status success */
    Status = STATUS_SUCCESS;
    Count = 0;

    /* Loop all entries in the type directory */
    for (i = 0; i < NumberOfTypeEntries; ++i, ++TypeEntry)
    {
        /* Check if comparison of types is requested */
        if (Level > RESOURCE_TYPE_LEVEL)
        {
            /* Compare the type with the requested Type */
            Result = LdrpCompareResourceNames_U(ResourceData,
                                                TypeEntry,
                                                ResourceInfo->Type);

            /* Not equal, continue with next entry */
            if (Result != 0) continue;
        }

        /* The entry must point to the name directory */
        if (!TypeEntry->DataIsDirectory)
        {
            return STATUS_INVALID_IMAGE_FORMAT;
        }

        /* Get a pointer to the name subdirectory and it's first entry */
        NameDirectory = (PIMAGE_RESOURCE_DIRECTORY)(ResourceData +
                                                    TypeEntry->OffsetToDirectory);
        NameEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(NameDirectory + 1);

        /* Get the number of entries in the name directory */
        NumberOfNameEntries = NameDirectory->NumberOfNamedEntries +
                              NameDirectory->NumberOfIdEntries;

        /* Loop all entries in the name directory */
        for (j = 0; j < NumberOfNameEntries; ++j, ++NameEntry)
        {
            /* Check if comparison of names is requested */
            if (Level > RESOURCE_NAME_LEVEL)
            {
                /* Compare the name with the requested name */
                Result = LdrpCompareResourceNames_U(ResourceData,
                                                    NameEntry,
                                                    ResourceInfo->Name);

                /* Not equal, continue with next entry */
                if (Result != 0) continue;
            }

            /* The entry must point to the language directory */
            if (!NameEntry->DataIsDirectory)
            {
                return STATUS_INVALID_IMAGE_FORMAT;
            }

            /* Get a pointer to the language subdirectory and it's first entry */
            LangDirectory = (PIMAGE_RESOURCE_DIRECTORY)(ResourceData +
                                                        NameEntry->OffsetToDirectory);
            LangEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(LangDirectory + 1);

            /* Get the number of entries in the language directory */
            NumberOfLangEntries = LangDirectory->NumberOfNamedEntries +
                                  LangDirectory->NumberOfIdEntries;

            /* Loop all entries in the language directory */
            for (k = 0; k < NumberOfLangEntries; ++k, ++LangEntry)
            {
                /* Check if comparison of languages is requested */
                if (Level > RESOURCE_LANGUAGE_LEVEL)
                {
                    /* Compare the language with the requested language */
                    Result = LdrpCompareResourceNames_U(ResourceData,
                                                        LangEntry,
                                                        ResourceInfo->Language);

                    /* Not equal, continue with next entry */
                    if (Result != 0) continue;
                }

                /* This entry must point to data */
                if (LangEntry->DataIsDirectory)
                {
                    return STATUS_INVALID_IMAGE_FORMAT;
                }

                /* Get a pointer to the data entry */
                DataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(ResourceData +
                                                         LangEntry->OffsetToData);

                /* Check if there is still space to store the data */
                if (Count < MaxResourceCount)
                {
                    /* There is, fill the entry */
                    Resources[Count].Type =
                        NAME_FROM_RESOURCE_ENTRY(ResourceData, TypeEntry);
                    Resources[Count].Name =
                        NAME_FROM_RESOURCE_ENTRY(ResourceData, NameEntry);
                    Resources[Count].Language =
                        NAME_FROM_RESOURCE_ENTRY(ResourceData, LangEntry);
                    Resources[Count].Data = (PUCHAR)ImageBase + DataEntry->OffsetToData;
                    Resources[Count].Reserved = 0;
                    Resources[Count].Size = DataEntry->Size;
                }
                else
                {
                    /* There is not enough space, save error status */
                    Status = STATUS_INFO_LENGTH_MISMATCH;
                }

                /* Count this resource */
                ++Count;
            }
        }
    }

    /* Return the number of matching resources */
    *ResourceCount = Count;
    return Status;
}

NTSTATUS
LdrpSearchResourceSection_U(
    IN PVOID DllHandle,
    IN const ULONG_PTR* ResourceIdPath,
    IN ULONG ResourceIdPathLength,
    IN ULONG Flags,
    OUT PVOID *ResourceDirectoryOrData
    )

/*++

Routine Description:

    This function locates the address of the specified resource in the
    specified DLL and returns its address.

Arguments:

    DllHandle - Supplies a handle to the image file that the resource is
        contained in.

    ResourceIdPath - Supplies a pointer to an array of 32-bit resource
        identifiers.  Each identifier is either an integer or a pointer
        to a null terminated string (PSZ) that specifies a resource
        name.  The array is used to traverse the directory structure
        contained in the resource section in the image file specified by
        the DllHandle parameter.

    ResourceIdPathLength - Supplies the number of elements in the
        ResourceIdPath array.

    Flags -
        LDRP_FIND_RESOURCE_DIRECTORY
        searching for a resource directory, otherwise the caller is
        searching for a resource data entry.

        LDR_FIND_RESOURCE_LANGUAGE_EXACT
        searching for a resource with, and only with, the language id
        specified in ResourceIdPath, otherwise the caller wants the routine
        to come up with default when specified langid is not found.

        LDR_FIND_RESOURCE_LANGUAGE_REDIRECT_VERSION
        searching for a resource version in main and alternative
        modules paths

    FindDirectoryEntry - Supplies a boolean that is TRUE if caller is
        searching for a resource directory, otherwise the caller is
        searching for a resource data entry.

    ExactLangMatchOnly - Supplies a boolean that is TRUE if caller is
        searching for a resource with, and only with, the language id
        specified in ResourceIdPath, otherwise the caller wants the routine
        to come up with default when specified langid is not found.

    ResourceDirectoryOrData - Supplies a pointer to a variable that will
        receive the address of the resource directory or data entry in
        the resource data section of the image file specified by the
        DllHandle parameter.
--*/

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

    RTL_PAGED_CODE;

    __TRY {
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
                        NewLangId = PRIMARYLANGID( GivenLanguage );
                        break;

                    case 2:     // Use id 0  (neutral resource)
                        NewLangId = 0;
                        break;

                    case 3:     // Use user's default UI language
                        NewLangId = (LANGID)ResourceIdRetry;
                        break;

                    case 4:     // Use native UI language
                        if ( !fIsNeutral ) {
                            // Stop looking - Not in the neutral case
                            goto ReturnFailure;
                            break;
                        }
                        NewLangId = PsInstallUILanguageId;
                        break;

                    case 5:     // Use default system locale
                        NewLangId = LANGIDFROMLCID(PsDefaultSystemLocaleId);
                        break;

                    case 6:
                        // Use US English language
                        NewLangId = MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US );
                        break;

                    case 7:     // Take any lang id that matches
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
                    dir = LdrpCompareResourceNames_U( (PUCHAR)*ResourceIdPath,
                                                      (PIMAGE_RESOURCE_DIRECTORY_ENTRY)TopResourceDirectory,
                                                      (ULONG_PTR)ResourceDirEntMiddle
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
                        dir = LdrpCompareResourceNames_U( (PUCHAR)*ResourceIdPath,
                          (PIMAGE_RESOURCE_DIRECTORY_ENTRY)TopResourceDirectory,
                                                          (ULONG_PTR)ResourceDirEntLow
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
        }
    _SEH2_EXCEPT (EXCEPTION_EXECUTE_HANDLER) {
        Status = GetExceptionCode();
        }
	__ENDTRY
    return Status;
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

/*++

Routine Description:

    This function locates the address of the specified resource in the
    specified DLL and returns its address.

Arguments:
    Flags -
        LDRP_FIND_RESOURCE_DIRECTORY
        searching for a resource directory, otherwise the caller is
        searching for a resource data entry.

        LDR_FIND_RESOURCE_LANGUAGE_EXACT
        searching for a resource with, and only with, the language id
        specified in ResourceIdPath, otherwise the caller wants the routine
        to come up with default when specified langid is not found.

        LDR_FIND_RESOURCE_LANGUAGE_REDIRECT_VERSION
        searching for a resource version in both main and alternative
        module paths

    DllHandle - Supplies a handle to the image file that the resource is
        contained in.

    ResourceIdPath - Supplies a pointer to an array of 32-bit resource
        identifiers.  Each identifier is either an integer or a pointer
        to a STRING structure that specifies a resource name.  The array
        is used to traverse the directory structure contained in the
        resource section in the image file specified by the DllHandle
        parameter.

    ResourceIdPathLength - Supplies the number of elements in the
        ResourceIdPath array.

    ResourceDataEntry - Supplies a pointer to a variable that will
        receive the address of the resource data entry in the resource
        data section of the image file specified by the DllHandle
        parameter.
--*/

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
