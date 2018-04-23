/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    filemap.c

Abstract:

    This module implements Win32 mapped file APIs

Author:

    Skulltrail 21-April-2018

Revision History:

--*/

#include "main.h"

/*
 * @implemented
 */
HANDLE
NTAPI
CreateFileMappingNumaW(
	HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCWSTR lpName,
	DWORD nndPreferred
)
{
    NTSTATUS Status;
    HANDLE SectionHandle;
    OBJECT_ATTRIBUTES LocalAttributes;
    POBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING SectionName;
    ACCESS_MASK DesiredAccess;
    LARGE_INTEGER LocalSize;
    PLARGE_INTEGER SectionSize = NULL;
    ULONG Attributes;

    /* Set default access */
    DesiredAccess = STANDARD_RIGHTS_REQUIRED | SECTION_QUERY | SECTION_MAP_READ;

    /* Get the attributes for the actual allocation and cleanup flProtect */
    Attributes = flProtect & (SEC_FILE | SEC_IMAGE | SEC_RESERVE | SEC_NOCACHE | SEC_COMMIT | SEC_LARGE_PAGES);
    flProtect ^= Attributes;

    /* If the caller didn't say anything, assume SEC_COMMIT */
    if (!Attributes) Attributes = SEC_COMMIT;

    /* Now check if the caller wanted write access */
    if (flProtect == PAGE_READWRITE)
    {
        /* Give it */
        DesiredAccess |= SECTION_MAP_WRITE;
    }
    else if (flProtect == PAGE_EXECUTE_READWRITE)
    {
        /* Give it */
        DesiredAccess |= (SECTION_MAP_WRITE | SECTION_MAP_EXECUTE);
    }
    else if (flProtect == PAGE_EXECUTE_READ)
    {
        /* Give it */
        DesiredAccess |= SECTION_MAP_EXECUTE;
    }
    else if ((flProtect == PAGE_EXECUTE_WRITECOPY) &&
             (NtCurrentPeb()->OSMajorVersion >= 6))
    {
        /* Give it */
        DesiredAccess |= (SECTION_MAP_WRITE | SECTION_MAP_EXECUTE);
    }
    else if ((flProtect != PAGE_READONLY) && (flProtect != PAGE_WRITECOPY))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }

    /* Now check if we got a name */
    if (lpName) RtlInitUnicodeString(&SectionName, lpName);

    /* Now convert the object attributes */
    ObjectAttributes = BaseFormatObjectAttributes(&LocalAttributes,
                                                    lpFileMappingAttributes,
                                                    lpName ? &SectionName : NULL);

    /* Check if we got a size */
    if (dwMaximumSizeLow || dwMaximumSizeHigh)
    {
        /* Use a LARGE_INTEGER and convert */
        SectionSize = &LocalSize;
        SectionSize->LowPart = dwMaximumSizeLow;
        SectionSize->HighPart = dwMaximumSizeHigh;
    }

    /* Make sure the handle is valid */
    if (hFile == INVALID_HANDLE_VALUE)
    {
        /* It's not, we'll only go on if we have a size */
        hFile = NULL;
        if (!SectionSize)
        {
            /* No size, so this isn't a valid non-mapped section */
            SetLastError(ERROR_INVALID_PARAMETER);
            return NULL;
        }
    }
	
	if ( nndPreferred != -1 )
		flProtect |= nndPreferred + 1;

    /* Now create the actual section */
    Status = NtCreateSection(&SectionHandle,
                             DesiredAccess,
                             ObjectAttributes,
                             SectionSize,
                             flProtect,
                             Attributes,
                             hFile);
    if (!NT_SUCCESS(Status))
    {
        /* We failed */
        BaseSetLastNTError(Status);
        return NULL;
    }
    else if (Status == STATUS_OBJECT_NAME_EXISTS)
    {
        SetLastError(ERROR_ALREADY_EXISTS);
    }
    else
    {
        SetLastError(ERROR_SUCCESS);
    }

    /* Return the section */
    return SectionHandle;
}

/*
 * @implemented
 */
HANDLE
NTAPI
CreateFileMappingNumaA(
	IN HANDLE hFile,
    IN LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    IN DWORD flProtect,
    IN DWORD dwMaximumSizeHigh,
    IN DWORD dwMaximumSizeLow,
    IN LPCSTR lpName,
	IN DWORD nndPreferred
)
{
    PUNICODE_STRING Unicode;
    ANSI_STRING AnsiString;
    NTSTATUS Status;
    LPCWSTR NameBuffer;

    NameBuffer = NULL;
    if ( ARGUMENT_PRESENT(lpName) ) {
        Unicode = &NtCurrentTeb()->StaticUnicodeString;
        RtlInitAnsiString(&AnsiString,lpName);
        Status = RtlAnsiStringToUnicodeString(Unicode,&AnsiString,FALSE);
        if ( !NT_SUCCESS(Status) ) {
            if ( Status == STATUS_BUFFER_OVERFLOW ) {
                SetLastError(ERROR_FILENAME_EXCED_RANGE);
                }
            else {
                BaseSetLastNTError(Status);
                }
            return NULL;
            }
        NameBuffer = (LPCWSTR)Unicode->Buffer;
        }

    return CreateFileMappingNumaW(
                hFile,
                lpFileMappingAttributes,
                flProtect,
                dwMaximumSizeHigh,
                dwMaximumSizeLow,
                NameBuffer,
                nndPreferred);
}

/*
 * @implemented
 */
LPVOID
NTAPI
MapViewOfFileExNuma(
	HANDLE hFileMappingObject,
	DWORD dwDesiredAccess,
	DWORD dwFileOffsetHigh,
	DWORD dwFileOffsetLow,
	SIZE_T dwNumberOfBytesToMap,
	LPVOID lpBaseAddress,
	DWORD nndPreferred
)
{
    NTSTATUS Status;
    LARGE_INTEGER SectionOffset;
    SIZE_T ViewSize;
    ULONG Protect;
    LPVOID ViewBase;

    /* Convert the offset */
    SectionOffset.LowPart = dwFileOffsetLow;
    SectionOffset.HighPart = dwFileOffsetHigh;

    /* Save the size and base */
    ViewBase = lpBaseAddress;
    ViewSize = dwNumberOfBytesToMap;

    /* Convert flags to NT Protection Attributes */
    if (dwDesiredAccess == FILE_MAP_COPY)
    {
        Protect = PAGE_WRITECOPY;
    }
    else if (dwDesiredAccess & FILE_MAP_WRITE)
    {
        Protect = (dwDesiredAccess & FILE_MAP_EXECUTE) ?
                   PAGE_EXECUTE_READWRITE : PAGE_READWRITE;
    }
    else if (dwDesiredAccess & FILE_MAP_READ)
    {
        Protect = (dwDesiredAccess & FILE_MAP_EXECUTE) ?
                   PAGE_EXECUTE_READ : PAGE_READONLY;
    }
    else
    {
        Protect = PAGE_NOACCESS;
    }
	
	if ( nndPreferred != -1 )
		Protect |= nndPreferred + 1;

    /* Map the section */
    Status = NtMapViewOfSection(hFileMappingObject,
                                NtCurrentProcess(),
                                &ViewBase,
                                0,
                                0,
                                &SectionOffset,
                                &ViewSize,
                                ViewShare,
                                0,
                                Protect);
    if (!NT_SUCCESS(Status))
    {
        /* We failed */
        BaseSetLastNTError(Status);
        return NULL;
    }

    /* Return the base */
    return ViewBase;
}
