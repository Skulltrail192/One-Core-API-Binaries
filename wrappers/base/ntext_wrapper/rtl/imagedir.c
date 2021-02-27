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

PIMAGE_SECTION_HEADER
RtlSectionTableFromVirtualAddress (
    IN PIMAGE_NT_HEADERS NtHeaders,
    IN PVOID Base,
    IN ULONG Address
    )

/*++

Routine Description:

    This function locates a VirtualAddress within the image header
    of a file that is mapped as a file and returns a pointer to the
    section table entry for that virtual address

Arguments:

    NtHeaders - Supplies the pointer to the image or data file.

    Base - Supplies the base of the image or data file.

    Address - Supplies the virtual address to locate.

Return Value:

    NULL - The file does not contain data for the specified directory entry.

    NON-NULL - Returns the pointer of the section entry containing the data.

--*/

{
    ULONG i;
    PIMAGE_SECTION_HEADER NtSection;

    NtSection = IMAGE_FIRST_SECTION( NtHeaders );
    for (i=0; i<NtHeaders->FileHeader.NumberOfSections; i++) {
        if ((ULONG)Address >= NtSection->VirtualAddress &&
            (ULONG)Address < NtSection->VirtualAddress + NtSection->SizeOfRawData
           ) {
            return NtSection;
            }
        ++NtSection;
        }

    return NULL;
}

PVOID
NTAPI
RtlAddressInSectionTable (
    IN PIMAGE_NT_HEADERS NtHeaders,
    IN PVOID Base,
    IN ULONG Address
    )

/*++

Routine Description:

    This function locates a VirtualAddress within the image header
    of a file that is mapped as a file and returns the seek address
    of the data the Directory describes.

Arguments:

    NtHeaders - Supplies the pointer to the image or data file.

    Base - Supplies the base of the image or data file.

    Address - Supplies the virtual address to locate.

Return Value:

    NULL - The file does not contain data for the specified directory entry.

    NON-NULL - Returns the address of the raw data the directory describes.

--*/

{
    PIMAGE_SECTION_HEADER NtSection;

    NtSection = RtlSectionTableFromVirtualAddress( NtHeaders,
                                                   Base,
                                                   Address
                                                 );
    if (NtSection != NULL) {
        return( ((PCHAR)Base + ((ULONG_PTR)Address - NtSection->VirtualAddress) + NtSection->PointerToRawData) );
        }
    else {
        return( NULL );
        }
}

/*
 * @implemented
 */
NTSYSAPI 
NTSTATUS
NTAPI
RtlImageNtHeaderEx(IN ULONG Flags,
                   IN PVOID Base,
                   IN ULONG64 Size,
                   OUT PIMAGE_NT_HEADERS *OutHeaders)
{
    PIMAGE_NT_HEADERS NtHeaders;
    PIMAGE_DOS_HEADER DosHeader;
    BOOLEAN WantsRangeCheck;

    /* You must want NT Headers, no? */
    if (!OutHeaders) return STATUS_INVALID_PARAMETER;

    /* Assume failure */
    *OutHeaders = NULL;

    /* Validate Flags */
    if (Flags &~ RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK)
    {
        DbgPrint("Invalid flag combination... check for new API flags?\n");
        return STATUS_INVALID_PARAMETER;
    }

    /* Validate base */
    if (!(Base) || (Base == (PVOID)-1)) return STATUS_INVALID_PARAMETER;

    /* Check if the caller wants validation */
    WantsRangeCheck = !(Flags & RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK);
    if (WantsRangeCheck)
    {
        /* Make sure the image size is at least big enough for the DOS header */
        if (Size < sizeof(IMAGE_DOS_HEADER))
        {
            DbgPrint("Size too small\n");
            return STATUS_INVALID_IMAGE_FORMAT;
        }
    }

    /* Check if the DOS Signature matches */
    DosHeader = Base;
    if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        /* Not a valid COFF */
        //DbgPrint("Not an MZ file\n");
        return STATUS_INVALID_IMAGE_FORMAT;
    }

    /* Check if the caller wants validation */
    if (WantsRangeCheck)
    {
        /* The offset should fit in the passsed-in size */
        if (DosHeader->e_lfanew >= Size)
        {
            /* Fail */
            DbgPrint("e_lfanew is larger than PE file\n");
            return STATUS_INVALID_IMAGE_FORMAT;
        }

        /* It shouldn't be past 4GB either */
        if (DosHeader->e_lfanew >=
            (MAXULONG - sizeof(IMAGE_DOS_SIGNATURE) - sizeof(IMAGE_FILE_HEADER)))
        {
            /* Fail */
            DbgPrint("e_lfanew is larger than 4GB\n");
            return STATUS_INVALID_IMAGE_FORMAT;
        }

        /* And the whole file shouldn't overflow past 4GB */
        if ((DosHeader->e_lfanew +
            sizeof(IMAGE_DOS_SIGNATURE) - sizeof(IMAGE_FILE_HEADER)) >= Size)
        {
            /* Fail */
            DbgPrint("PE is larger than 4GB\n");
            return STATUS_INVALID_IMAGE_FORMAT;
        }
    }

    /* The offset also can't be larger than 256MB, as a hard-coded check */
    if (DosHeader->e_lfanew >= (256 * 1024 * 1024))
    {
        /* Fail */
        DbgPrint("PE offset is larger than 256MB\n");
        return STATUS_INVALID_IMAGE_FORMAT;
    }

    /* Now it's safe to get the NT Headers */
    NtHeaders = (PIMAGE_NT_HEADERS)((ULONG_PTR)Base + DosHeader->e_lfanew);

    /* Verify the PE Signature */
    if (NtHeaders->Signature != IMAGE_NT_SIGNATURE)
    {
        /* Fail */
        DbgPrint("PE signature missing\n");
        return STATUS_INVALID_IMAGE_FORMAT;
    }

    /* Now return success and the NT header */
    *OutHeaders = NtHeaders;
    return STATUS_SUCCESS;
}