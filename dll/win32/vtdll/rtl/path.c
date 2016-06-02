/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */
#define NDEBUG 
 
#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vtdll);

const UNICODE_STRING DeviceRootString = RTL_CONSTANT_STRING(L"\\\\.\\");
const UNICODE_STRING RtlpDosLPTDevice = RTL_CONSTANT_STRING(L"LPT");
const UNICODE_STRING RtlpDosCOMDevice = RTL_CONSTANT_STRING(L"COM");
const UNICODE_STRING RtlpDosPRNDevice = RTL_CONSTANT_STRING(L"PRN");
const UNICODE_STRING RtlpDosAUXDevice = RTL_CONSTANT_STRING(L"AUX");
const UNICODE_STRING RtlpDosCONDevice = RTL_CONSTANT_STRING(L"CON");
const UNICODE_STRING RtlpDosNULDevice = RTL_CONSTANT_STRING(L"NUL");
const UNICODE_STRING RtlpDosSlashCONDevice   = RTL_CONSTANT_STRING(L"\\\\.\\CON");
const UNICODE_STRING RtlpDosDevicesUncPrefix = RTL_CONSTANT_STRING(L"\\??\\UNC\\");
const UNICODE_STRING RtlpDosDevicesPrefix    = RTL_CONSTANT_STRING(L"\\??\\");
const UNICODE_STRING RtlpWin32NtRootSlash    = RTL_CONSTANT_STRING(L"\\\\?\\");

PRTLP_CURDIR_REF RtlpCurDirRef;

NTSTATUS
NTAPI
RtlpCheckDeviceName(IN PUNICODE_STRING FileName,
                    IN ULONG Length,
                    OUT PBOOLEAN NameInvalid)
{
    PWCHAR Buffer;
    NTSTATUS Status;

    /* Allocate a large enough buffer */
    Buffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, FileName->Length);
    if (Buffer)
    {
        /* Assume failure */
        *NameInvalid = TRUE;

        /* Copy the filename */
        RtlCopyMemory(Buffer, FileName->Buffer, FileName->Length);

        /* And add a dot at the end */
        Buffer[Length / sizeof(WCHAR)] = L'.';
        Buffer[(Length / sizeof(WCHAR)) + 1] = UNICODE_NULL;

        /* Check if the file exists or not */
        *NameInvalid = RtlDoesFileExists_U(Buffer) ? FALSE: TRUE;

        /* Get rid of the buffer now */
        Status = RtlFreeHeap(RtlGetProcessHeap(), 0, Buffer);
    }
    else
    {
        /* Assume the name is ok, but fail the call */
        *NameInvalid = FALSE;
        Status = STATUS_NO_MEMORY;
    }

    /* Return the status */
    return Status;
}

/******************************************************************
 *    RtlpCollapsePath (from WINE)
 *
 * Helper for RtlGetFullPathName_U
 *
 * 1) Converts slashes into backslashes and gets rid of duplicated ones;
 * 2) Gets rid of . and .. components in the path.
 *
 * Returns the full path length without its terminating NULL character.
 */
static ULONG
RtlpCollapsePath(PWSTR Path, /* ULONG PathBufferSize, ULONG PathLength, */ ULONG mark, BOOLEAN SkipTrailingPathSeparators)
{
    PWSTR p, next;

    // FIXME: Do not suppose NULL-terminated strings!!

    ULONG PathLength = wcslen(Path);
    PWSTR EndBuffer = Path + PathLength; // Path + PathBufferSize / sizeof(WCHAR);
    PWSTR EndPath;

    /* Convert slashes into backslashes */
    for (p = Path; *p; p++)
    {
        if (*p == L'/') *p = L'\\';
    }

    /* Collapse duplicate backslashes */
    next = Path + max( 1, mark );
    for (p = next; *p; p++)
    {
        if (*p != L'\\' || next[-1] != L'\\') *next++ = *p;
    }
    *next = UNICODE_NULL;
    EndPath = next;

    p = Path + mark;
    while (*p)
    {
        if (*p == L'.')
        {
            switch (p[1])
            {
                case UNICODE_NULL:  /* final . */
                    if (p > Path + mark) p--;
                    *p = UNICODE_NULL;
                    EndPath = p;
                    continue;

                case L'\\': /* .\ component */
                    next = p + 2;
                    // ASSERT(EndPath - next == wcslen(next));
                    RtlMoveMemory(p, next, (EndPath - next + 1) * sizeof(WCHAR));
                    EndPath -= (next - p);
                    continue;

                case L'.':
                    if (p[2] == L'\\')  /* ..\ component */
                    {
                        next = p + 3;
                        if (p > Path + mark)
                        {
                            p--;
                            while (p > Path + mark && p[-1] != L'\\') p--;
                        }
                        // ASSERT(EndPath - next == wcslen(next));
                        RtlMoveMemory(p, next, (EndPath - next + 1) * sizeof(WCHAR));
                        EndPath -= (next - p);
                        continue;
                    }
                    else if (p[2] == UNICODE_NULL)  /* final .. */
                    {
                        if (p > Path + mark)
                        {
                            p--;
                            while (p > Path + mark && p[-1] != L'\\') p--;
                            if (p > Path + mark) p--;
                        }
                        *p = UNICODE_NULL;
                        EndPath = p;
                        continue;
                    }
                    break;
            }
        }

        /* Skip to the next component */
        while (*p && *p != L'\\') p++;
        if (*p == L'\\')
        {
            /* Remove last dot in previous dir name */
            if (p > Path + mark && p[-1] == L'.')
            {
                // ASSERT(EndPath - p == wcslen(p));
                RtlMoveMemory(p - 1, p, (EndPath - p + 1) * sizeof(WCHAR));
                EndPath--;
            }
            else
            {
                p++;
            }
        }
    }

    /* Remove trailing backslashes if needed (after the UNC part if it exists) */
    if (SkipTrailingPathSeparators)
    {
        while (p > Path + mark && IS_PATH_SEPARATOR(p[-1])) p--;
    }

    /* Remove trailing spaces and dots (for all the path) */
    while (p > Path && (p[-1] == L' ' || p[-1] == L'.')) p--;

    /*
     * Zero-out the discarded buffer zone, starting just after
     * the path string and going up to the end of the buffer.
     * It also NULL-terminate the path string.
     */
    ASSERT(EndBuffer >= p);
    RtlZeroMemory(p, (EndBuffer - p + 1) * sizeof(WCHAR));

    /* Return the real path length */
    PathLength = (p - Path);
    // ASSERT(PathLength == wcslen(Path));
    return (PathLength * sizeof(WCHAR));
}


/******************************************************************
 *    RtlpSkipUNCPrefix (from WINE)
 *
 * Helper for RtlGetFullPathName_U
 *
 * Skips the \\share\dir part of a file name and returns the new position
 * (which can point on the last backslash of "dir\").
 */
static SIZE_T
RtlpSkipUNCPrefix(PCWSTR FileNameBuffer)
{
    PCWSTR UncPath = FileNameBuffer + 2;
    DbgPrint("RtlpSkipUNCPrefix(%S)\n", FileNameBuffer);

    while (*UncPath && !IS_PATH_SEPARATOR(*UncPath)) UncPath++;  /* share name */
    while (IS_PATH_SEPARATOR(*UncPath)) UncPath++;
    while (*UncPath && !IS_PATH_SEPARATOR(*UncPath)) UncPath++;  /* dir name */
    /* while (IS_PATH_SEPARATOR(*UncPath)) UncPath++; */

    return (UncPath - FileNameBuffer);
}

RTL_PATH_TYPE
NTAPI
RtlDetermineDosPathNameType_Ustr(IN PCUNICODE_STRING PathString)
{
    PWCHAR Path;
    ULONG Chars;

    Path = PathString->Buffer;
    Chars = PathString->Length / sizeof(WCHAR);

    /* Return if there are no characters */
    if (!Chars) return RtlPathTypeRelative;

    /*
     * The algorithm is similar to RtlDetermineDosPathNameType_U but here we
     * actually check for the path length before touching the characters
     */
    if (IS_PATH_SEPARATOR(Path[0]))
    {
        if ((Chars < 2) || !(IS_PATH_SEPARATOR(Path[1]))) return RtlPathTypeRooted;                /* \x             */
        if ((Chars < 3) || ((Path[2] != L'.') && (Path[2] != L'?'))) return RtlPathTypeUncAbsolute;/* \\x            */
        if ((Chars >= 4) && (IS_PATH_SEPARATOR(Path[3]))) return RtlPathTypeLocalDevice;           /* \\.\x or \\?\x */
        if (Chars != 3) return RtlPathTypeUncAbsolute;                                             /* \\.x or \\?x   */
        return RtlPathTypeRootLocalDevice;                                                         /* \\. or \\?     */
    }
    else
    {
        if ((Chars < 2) || (Path[1] != L':')) return RtlPathTypeRelative;                          /* x              */
        if ((Chars < 3) || !(IS_PATH_SEPARATOR(Path[2]))) return RtlPathTypeDriveRelative;         /* x:             */
        return RtlPathTypeDriveAbsolute;                                                           /* x:\            */
    }
}

ULONG
NTAPI
RtlIsDosDeviceName_Ustr(IN PCUNICODE_STRING PathString)
{
    UNICODE_STRING PathCopy;
    PWCHAR Start, End;
    USHORT PathChars, ColonCount = 0;
    USHORT ReturnOffset = 0, ReturnLength, OriginalLength;
    WCHAR c;

    /* Validate the input */
    if (!PathString) return 0;

    /* Check what type of path this is */
    switch (RtlDetermineDosPathNameType_Ustr(PathString))
    {
        /* Fail for UNC or unknown paths */
        case RtlPathTypeUnknown:
        case RtlPathTypeUncAbsolute:
            return 0;

        /* Make special check for the CON device */
        case RtlPathTypeLocalDevice:
            if (RtlEqualUnicodeString(PathString, &RtlpDosSlashCONDevice, TRUE))
            {
                /* This should return 0x80006 */
                return MAKELONG(RtlpDosCONDevice.Length, DeviceRootString.Length);
            }
            return 0;

        default:
            break;
    }

    /* Make a copy of the string */
    PathCopy = *PathString;
    OriginalLength = PathString->Length;

    /* Return if there's no characters */
    PathChars = PathCopy.Length / sizeof(WCHAR);
    if (!PathChars) return 0;

    /* Check for drive path and truncate */
    if (PathCopy.Buffer[PathChars - 1] == L':')
    {
        /* Fixup the lengths */
        PathCopy.Length -= sizeof(WCHAR);
        if (!--PathChars) return 0;

        /* Remember this for later */
        ColonCount = 1;
    }

    /* Check for extension or space, and truncate */
    do
    {
        /* Stop if we hit something else than a space or period */
        c = PathCopy.Buffer[PathChars - 1];
        if ((c != L'.') && (c != L' ')) break;

        /* Fixup the lengths */
        PathCopy.Length -= sizeof(WCHAR);

        /* Remember this for later */
        ColonCount++;
    } while (--PathChars);

    /* Anything still left? */
    if (PathChars)
    {
        /* Loop from the end */
        for (End = &PathCopy.Buffer[PathChars - 1];
             End >= PathCopy.Buffer;
             --End)
        {
            /* Check if the character is a path or drive separator */
            c = *End;
            if (IS_PATH_SEPARATOR(c) || ((c == L':') && (End == PathCopy.Buffer + 1)))
            {
                /* Get the next lower case character */
                End++;
                c = RtlDowncaseUnicodeChar(*End);

                /* Check if it's a DOS device (LPT, COM, PRN, AUX, or NUL) */
                if ((End < &PathCopy.Buffer[OriginalLength / sizeof(WCHAR)]) &&
                    ((c == L'l') || (c == L'c') || (c == L'p') || (c == L'a') || (c == L'n')))
                {
                    /* Calculate the offset */
                    ReturnOffset = (USHORT)((PCHAR)End - (PCHAR)PathCopy.Buffer);

                    /* Build the final string */
                    PathCopy.Length = OriginalLength - ReturnOffset - (ColonCount * sizeof(WCHAR));
                    PathCopy.Buffer = End;

                    /* Save new amount of chars in the path */
                    PathChars = PathCopy.Length / sizeof(WCHAR);

                    break;
                }
                else
                {
                    return 0;
                }
            }
        }

        /* Get the next lower case character and check if it's a DOS device */
        c = RtlDowncaseUnicodeChar(*PathCopy.Buffer);
        if ((c != L'l') && (c != L'c') && (c != L'p') && (c != L'a') && (c != L'n'))
        {
            /* Not LPT, COM, PRN, AUX, or NUL */
            return 0;
        }
    }

    /* Now skip past any extra extension or drive letter characters */
    Start = PathCopy.Buffer;
    End = &Start[PathChars];
    while (Start < End)
    {
        c = *Start;
        if ((c == L'.') || (c == L':')) break;
        Start++;
    }

    /* And then go backwards to get rid of spaces */
    while ((Start > PathCopy.Buffer) && (Start[-1] == L' ')) --Start;

    /* Finally see how many characters are left, and that's our size */
    PathChars = (USHORT)(Start - PathCopy.Buffer);
    PathCopy.Length = PathChars * sizeof(WCHAR);

    /* Check if this is a COM or LPT port, which has a digit after it */
    if ((PathChars == 4) &&
        (iswdigit(PathCopy.Buffer[3]) && (PathCopy.Buffer[3] != L'0')))
    {
        /* Don't compare the number part, just check for LPT or COM */
        PathCopy.Length -= sizeof(WCHAR);
        if ((RtlEqualUnicodeString(&PathCopy, &RtlpDosLPTDevice, TRUE)) ||
            (RtlEqualUnicodeString(&PathCopy, &RtlpDosCOMDevice, TRUE)))
        {
            /* Found it */
            ReturnLength = sizeof(L"COM1") - sizeof(WCHAR);
            return MAKELONG(ReturnLength, ReturnOffset);
        }
    }
    else if ((PathChars == 3) &&
             ((RtlEqualUnicodeString(&PathCopy, &RtlpDosPRNDevice, TRUE)) ||
              (RtlEqualUnicodeString(&PathCopy, &RtlpDosAUXDevice, TRUE)) ||
              (RtlEqualUnicodeString(&PathCopy, &RtlpDosNULDevice, TRUE)) ||
              (RtlEqualUnicodeString(&PathCopy, &RtlpDosCONDevice, TRUE))))
    {
        /* Otherwise this was something like AUX, NUL, PRN, or CON */
        ReturnLength = sizeof(L"AUX") - sizeof(WCHAR);
        return MAKELONG(ReturnLength, ReturnOffset);
    }

    /* Otherwise, this is not a valid DOS device */
    return 0;
}

ULONG
NTAPI
RtlGetFullPathName_Ustr(
    _In_ PUNICODE_STRING FileName,
    _In_ ULONG Size,
    _Out_z_bytecap_(Size) PWSTR Buffer,
    _Out_opt_ PCWSTR *ShortName,
    _Out_opt_ PBOOLEAN InvalidName,
    _Out_ RTL_PATH_TYPE *PathType)
{
    NTSTATUS Status;
    PWCHAR FileNameBuffer;
    ULONG FileNameLength, FileNameChars, DosLength, DosLengthOffset, FullLength;
    BOOLEAN SkipTrailingPathSeparators;
    WCHAR c;


    ULONG               reqsize = 0;
    PCWSTR              ptr;

    PCUNICODE_STRING    CurDirName;
    UNICODE_STRING      EnvVarName, EnvVarValue;
    WCHAR EnvVarNameBuffer[4];

    ULONG  PrefixCut    = 0;    // Where the path really starts (after the skipped prefix)
    PWCHAR Prefix       = NULL; // pointer to the string to be inserted as the new path prefix
    ULONG  PrefixLength = 0;
    PWCHAR Source;
    ULONG  SourceLength;


    /* For now, assume the name is valid */
    //DbgPrint("Filename: %wZ\n", FileName);
    //DbgPrint("Size and buffer: %lx %p\n", Size, Buffer);
    if (InvalidName) *InvalidName = FALSE;

    /* Handle initial path type and failure case */
    *PathType = RtlPathTypeUnknown;
    if ((FileName->Length == 0) || (FileName->Buffer[0] == UNICODE_NULL)) return 0;

    /* Break filename into component parts */
    FileNameBuffer = FileName->Buffer;
    FileNameLength = FileName->Length;
    FileNameChars  = FileNameLength / sizeof(WCHAR);

    /* Kill trailing spaces */
    c = FileNameBuffer[FileNameChars - 1];
    while ((FileNameLength != 0) && (c == L' '))
    {
        /* Keep going, ignoring the spaces */
        FileNameLength -= sizeof(WCHAR);
        if (FileNameLength != 0) c = FileNameBuffer[FileNameLength / sizeof(WCHAR) - 1];
    }

    /* Check if anything is left */
    if (FileNameLength == 0) return 0;

    /*
     * Check whether we'll need to skip trailing path separators in the
     * computed full path name. If the original file name already contained
     * trailing separators, then we keep them in the full path name. On the
     * other hand, if the original name didn't contain any trailing separators
     * then we'll skip it in the full path name.
     */
    SkipTrailingPathSeparators = !IS_PATH_SEPARATOR(FileNameBuffer[FileNameChars - 1]);

    /* Check if this is a DOS name */
    DosLength = RtlIsDosDeviceName_Ustr(FileName);
    //DbgPrint("DOS length for filename: %lx %wZ\n", DosLength, FileName);
    if (DosLength != 0)
    {
        /* Zero out the short name */
        if (ShortName) *ShortName = NULL;

        /* See comment for RtlIsDosDeviceName_Ustr if this is confusing... */
        DosLengthOffset = HIWORD(DosLength);
        DosLength       = LOWORD(DosLength);

        /* Do we have a DOS length, and does the caller want validity? */
        if (InvalidName && (DosLengthOffset != 0))
        {
            /* Do the check */
            Status = RtlpCheckDeviceName(FileName, DosLengthOffset, InvalidName);

            /* If the check failed, or the name is invalid, fail here */
            if (!NT_SUCCESS(Status)) return 0;
            if (*InvalidName) return 0;
        }

        /* Add the size of the device root and check if it fits in the size */
        FullLength = DosLength + DeviceRootString.Length;
        if (FullLength < Size)
        {
            /* Add the device string */
            RtlMoveMemory(Buffer, DeviceRootString.Buffer, DeviceRootString.Length);

            /* Now add the DOS device name */
            RtlMoveMemory((PCHAR)Buffer + DeviceRootString.Length,
                          (PCHAR)FileNameBuffer + DosLengthOffset,
                          DosLength);

            /* Null terminate */
            *(PWCHAR)((ULONG_PTR)Buffer + FullLength) = UNICODE_NULL;
            return FullLength;
        }

        /* Otherwise, there's no space, so return the buffer size needed */
        if ((FullLength + sizeof(UNICODE_NULL)) > UNICODE_STRING_MAX_BYTES) return 0;
        return FullLength + sizeof(UNICODE_NULL);
    }

    /* Zero-out the destination buffer. FileName must be different from Buffer */
    RtlZeroMemory(Buffer, Size);

    /* Get the path type */
    *PathType = RtlDetermineDosPathNameType_U(FileNameBuffer);



    /**********************************************
     **    CODE REWRITING IS HAPPENING THERE     **
     **********************************************/
    Source       = FileNameBuffer;
    SourceLength = FileNameLength;
    EnvVarValue.Buffer = NULL;

    /* Lock the PEB to get the current directory */
    RtlAcquirePebLock();
    CurDirName = &NtCurrentPeb()->ProcessParameters->CurrentDirectory.DosPath;

    switch (*PathType)
    {
        case RtlPathTypeUncAbsolute:        /* \\foo   */
            PrefixCut = RtlpSkipUNCPrefix(FileNameBuffer);
            break;

        case RtlPathTypeLocalDevice:        /* \\.\foo */
            PrefixCut = 4;
            break;

        case RtlPathTypeDriveAbsolute:      /* c:\foo  */
            ASSERT(FileNameBuffer[1] == L':');
            ASSERT(IS_PATH_SEPARATOR(FileNameBuffer[2]));

            Prefix = FileNameBuffer;
            PrefixLength = 3 * sizeof(WCHAR);
            Source += 3;
            SourceLength -= 3 * sizeof(WCHAR);

            PrefixCut = 3;
            break;

        case RtlPathTypeDriveRelative:      /* c:foo   */
            Source += 2;
            SourceLength -= 2 * sizeof(WCHAR);
            if (RtlUpcaseUnicodeChar(FileNameBuffer[0]) != RtlUpcaseUnicodeChar(CurDirName->Buffer[0]) ||
                CurDirName->Buffer[1] != L':')
            {
                EnvVarNameBuffer[0] = L'=';
                EnvVarNameBuffer[1] = FileNameBuffer[0];
                EnvVarNameBuffer[2] = L':';
                EnvVarNameBuffer[3] = UNICODE_NULL;
    
                EnvVarName.Length = 3 * sizeof(WCHAR);
                EnvVarName.MaximumLength = EnvVarName.Length + sizeof(WCHAR);
                EnvVarName.Buffer = EnvVarNameBuffer;

                // FIXME: Is it possible to use the user-given buffer ?
                // RtlInitEmptyUnicodeString(&EnvVarValue, NULL, Size);
                EnvVarValue.Length = 0;
                EnvVarValue.MaximumLength = (USHORT)Size;
                EnvVarValue.Buffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, Size);
                if (EnvVarValue.Buffer == NULL)
                {
                    Prefix       = NULL;
                    PrefixLength = 0;
                    goto Quit;
                }

                Status = RtlQueryEnvironmentVariable_U(NULL, &EnvVarName, &EnvVarValue);
                switch (Status)
                {
                    case STATUS_SUCCESS:
                        /*
                         * (From Wine)
                         * FIXME: Win2k seems to check that the environment
                         * variable actually points to an existing directory.
                         * If not, root of the drive is used (this seems also
                         * to be the only place in RtlGetFullPathName that the
                         * existence of a part of a path is checked).
                         */
                        EnvVarValue.Buffer[EnvVarValue.Length / sizeof(WCHAR)] = L'\\';
                        Prefix       = EnvVarValue.Buffer;
                        PrefixLength = EnvVarValue.Length + sizeof(WCHAR); /* Append trailing '\\' */
                        break;

                    case STATUS_BUFFER_TOO_SMALL:
                        reqsize = EnvVarValue.Length + SourceLength + sizeof(UNICODE_NULL);
                        goto Quit;

                    default:
                        DbgPrint("RtlQueryEnvironmentVariable_U returned 0x%08lx\n", Status);

                        EnvVarNameBuffer[0] = FileNameBuffer[0];
                        EnvVarNameBuffer[1] = L':';
                        EnvVarNameBuffer[2] = L'\\';
                        EnvVarNameBuffer[3] = UNICODE_NULL;
                        Prefix       = EnvVarNameBuffer;
                        PrefixLength = 3 * sizeof(WCHAR);

                        RtlFreeHeap(RtlGetProcessHeap(), 0, EnvVarValue.Buffer);
                        EnvVarValue.Buffer = NULL;
                        break;
                }
                PrefixCut = 3;
                break;
            }
            /* Fall through */
            DbgPrint("RtlPathTypeDriveRelative - Using fall-through to RtlPathTypeRelative\n");

        case RtlPathTypeRelative:           /* foo     */
            Prefix       = CurDirName->Buffer;
            PrefixLength = CurDirName->Length;
            if (CurDirName->Buffer[1] != L':')
            {
                PrefixCut = RtlpSkipUNCPrefix(CurDirName->Buffer);
            }
            else
            {
                PrefixCut = 3;
            }
            break;

        case RtlPathTypeRooted:             /* \xxx    */
            if (CurDirName->Buffer[1] == L':')
            {
                // The path starts with "C:\"
                ASSERT(CurDirName->Buffer[1] == L':');
                ASSERT(IS_PATH_SEPARATOR(CurDirName->Buffer[2]));

                Prefix = CurDirName->Buffer;
                PrefixLength = 3 * sizeof(WCHAR); // Skip "C:\"

                PrefixCut = 3;      // Source index location incremented of + 3
            }
            else
            {
                PrefixCut = RtlpSkipUNCPrefix(CurDirName->Buffer);
                PrefixLength = PrefixCut * sizeof(WCHAR);
                Prefix = CurDirName->Buffer;
            }
            break;

        case RtlPathTypeRootLocalDevice:    /* \\.     */
            Prefix       = DeviceRootString.Buffer;
            PrefixLength = DeviceRootString.Length;
            Source += 3;
            SourceLength -= 3 * sizeof(WCHAR);

            PrefixCut = 4;
            break;

        case RtlPathTypeUnknown:
            goto Quit;
    }

    /* Do we have enough space for storing the full path? */
    reqsize = PrefixLength;
    if (reqsize + SourceLength + sizeof(WCHAR) > Size)
    {
        /* Not enough space, return needed size (including terminating '\0') */
        reqsize += SourceLength + sizeof(WCHAR);
        goto Quit;
    }

    /*
     * Build the full path
     */
    /* Copy the path's prefix */
    if (PrefixLength) RtlMoveMemory(Buffer, Prefix, PrefixLength);
    /* Copy the remaining part of the path */
    RtlMoveMemory(Buffer + PrefixLength / sizeof(WCHAR), Source, SourceLength + sizeof(WCHAR));

    /* Some cleanup */
    Prefix = NULL;
    if (EnvVarValue.Buffer)
    {
        RtlFreeHeap(RtlGetProcessHeap(), 0, EnvVarValue.Buffer);
        EnvVarValue.Buffer = NULL;
    }

    /*
     * Finally, put the path in canonical form (remove redundant . and ..,
     * (back)slashes...) and retrieve the length of the full path name
     * (without its terminating null character) (in chars).
     */
    reqsize = RtlpCollapsePath(Buffer, /* Size, reqsize, */ PrefixCut, SkipTrailingPathSeparators);

    /* Find the file part, which is present after the last path separator */
    if (ShortName)
    {
        ptr = wcsrchr(Buffer, L'\\');
        if (ptr) ++ptr; // Skip it

        /*
         * For UNC paths, the file part is after the \\share\dir part of the path.
         */
        PrefixCut = (*PathType == RtlPathTypeUncAbsolute ? PrefixCut : 3);

        if (ptr && *ptr && (ptr >= Buffer + PrefixCut))
        {
            *ShortName = ptr;
        }
        else
        {
            /* Zero-out the short name */
            *ShortName = NULL;
        }
    }

Quit:
    /* Release PEB lock */
    RtlReleasePebLock();

    return reqsize;
}

NTSTATUS
NTAPI
RtlpWin32NTNameToNtPathName_U(IN PUNICODE_STRING DosPath,
                              OUT PUNICODE_STRING NtPath,
                              OUT PCWSTR *PartName,
                              OUT PRTL_RELATIVE_NAME_U RelativeName)
{
    ULONG DosLength;
    PWSTR NewBuffer, p;

    /* Validate the input */
    if (!DosPath) return STATUS_OBJECT_NAME_INVALID;

    /* Validate the DOS length */
    DosLength = DosPath->Length;
    if (DosLength >= UNICODE_STRING_MAX_BYTES) return STATUS_NAME_TOO_LONG;

    /* Make space for the new path */
    NewBuffer = RtlAllocateHeap(RtlGetProcessHeap(),
                                0,
                                DosLength + sizeof(UNICODE_NULL));
    if (!NewBuffer) return STATUS_NO_MEMORY;

    /* Copy the prefix, and then the rest of the DOS path, and NULL-terminate */
    RtlCopyMemory(NewBuffer, RtlpDosDevicesPrefix.Buffer, RtlpDosDevicesPrefix.Length);
    RtlCopyMemory((PCHAR)NewBuffer + RtlpDosDevicesPrefix.Length,
                  DosPath->Buffer + RtlpDosDevicesPrefix.Length / sizeof(WCHAR),
                  DosPath->Length - RtlpDosDevicesPrefix.Length);
    NewBuffer[DosLength / sizeof(WCHAR)] = UNICODE_NULL;

    /* Did the caller send a relative name? */
    if (RelativeName)
    {
        /* Zero initialize it */
        RtlInitEmptyUnicodeString(&RelativeName->RelativeName, NULL, 0);
        RelativeName->ContainingDirectory = NULL;
        RelativeName->CurDirRef = 0;
    }

    /* Did the caller request a partial name? */
    if (PartName)
    {
        /* Loop from the back until we find a path separator */
        p = &NewBuffer[DosLength / sizeof(WCHAR)];
        while (--p > NewBuffer)
        {
            /* We found a path separator, move past it */
            if (*p == OBJ_NAME_PATH_SEPARATOR)
            {
                ++p;
                break;
            }
        }

        /* Check whether a separator was found and if something remains */
        if ((p > NewBuffer) && *p)
        {
            /* What follows the path separator is the partial name */
            *PartName = p;
        }
        else
        {
            /* The path ends with a path separator, no partial name */
            *PartName = NULL;
        }
    }

    /* Build the final NT path string */
    NtPath->Buffer = NewBuffer;
    NtPath->Length = (USHORT)DosLength;
    NtPath->MaximumLength = (USHORT)DosLength + sizeof(UNICODE_NULL);
    return STATUS_SUCCESS;
}


NTSTATUS
NTAPI
RtlpDosPathNameToRelativeNtPathName_Ustr(
	IN BOOLEAN HaveRelative,
	IN PCUNICODE_STRING DosName,
	OUT PUNICODE_STRING NtName,
	OUT PCWSTR *PartName,
	OUT PRTL_RELATIVE_NAME_U RelativeName)
{
    WCHAR BigBuffer[MAX_PATH + 1];
    PWCHAR PrefixBuffer, NewBuffer, Buffer;
    ULONG MaxLength, PathLength, PrefixLength, PrefixCut, LengthChars, Length;
    UNICODE_STRING CapturedDosName, PartNameString, FullPath;
    BOOLEAN QuickPath;
    RTL_PATH_TYPE InputPathType, BufferPathType;
    NTSTATUS Status;
    BOOLEAN NameInvalid;
    PCURDIR CurrentDirectory;

    /* Assume MAX_PATH for now */
    //DbgPrint("Relative: %lx DosName: %wZ NtName: %p, PartName: %p, RelativeName: %p\n",
           // HaveRelative, DosName, NtName, PartName, RelativeName);
    MaxLength = sizeof(BigBuffer);

    /* Validate the input */
    if (!DosName) return STATUS_OBJECT_NAME_INVALID;

    /* Capture input string */
    CapturedDosName = *DosName;

    /* Check for the presence or absence of the NT prefix "\\?\" form */
    // if (!RtlPrefixUnicodeString(&RtlpWin32NtRootSlash, &CapturedDosName, FALSE))
    if ((CapturedDosName.Length <= RtlpWin32NtRootSlash.Length) ||
        (CapturedDosName.Buffer[0] != RtlpWin32NtRootSlash.Buffer[0]) ||
        (CapturedDosName.Buffer[1] != RtlpWin32NtRootSlash.Buffer[1]) ||
        (CapturedDosName.Buffer[2] != RtlpWin32NtRootSlash.Buffer[2]) ||
        (CapturedDosName.Buffer[3] != RtlpWin32NtRootSlash.Buffer[3]))
    {
        /* NT prefix not present */

        /* Quick path won't be used */
        QuickPath = FALSE;

        /* Use the static buffer */
        Buffer = BigBuffer;
        MaxLength += RtlpDosDevicesUncPrefix.Length;

        /* Allocate a buffer to hold the path */
        NewBuffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, MaxLength);
        //DbgPrint("MaxLength: %lx\n", MaxLength);
        if (!NewBuffer) return STATUS_NO_MEMORY;
    }
    else
    {
        /* NT prefix present */

        /* Use the optimized path after acquiring the lock */
        QuickPath = TRUE;
        NewBuffer = NULL;
    }

    /* Lock the PEB and check if the quick path can be used */
    RtlAcquirePebLock();
    if (QuickPath)
    {
        /* Some simple fixups will get us the correct path */
        //DbgPrint("Quick path\n");
        Status = RtlpWin32NTNameToNtPathName_U(&CapturedDosName,
                                               NtName,
                                               PartName,
                                               RelativeName);

        /* Release the lock, we're done here */
        RtlReleasePebLock();
        return Status;
    }

    /* Call the main function to get the full path name and length */
    PathLength = RtlGetFullPathName_Ustr(&CapturedDosName,
                                         MAX_PATH * sizeof(WCHAR),
                                         Buffer,
                                         PartName,
                                         &NameInvalid,
                                         &InputPathType);
    if ((NameInvalid) || !(PathLength) || (PathLength > (MAX_PATH * sizeof(WCHAR))))
    {
        /* Invalid name, fail */
        DbgPrint("Invalid name: %lx Path Length: %lx\n", NameInvalid, PathLength);
        RtlFreeHeap(RtlGetProcessHeap(), 0, NewBuffer);
        RtlReleasePebLock();
        return STATUS_OBJECT_NAME_INVALID;
    }

    /* Start by assuming the path starts with \??\ (DOS Devices Path) */
    PrefixLength = RtlpDosDevicesPrefix.Length;
    PrefixBuffer = RtlpDosDevicesPrefix.Buffer;
    PrefixCut = 0;

    /* Check where it really is */
    BufferPathType = RtlDetermineDosPathNameType_U(Buffer);
    //DbgPrint("Buffer: %S Type: %lx\n", Buffer, BufferPathType);
    switch (BufferPathType)
    {
        /* It's actually a UNC path in \??\UNC\ */
        case RtlPathTypeUncAbsolute:
            PrefixLength = RtlpDosDevicesUncPrefix.Length;
            PrefixBuffer = RtlpDosDevicesUncPrefix.Buffer;
            PrefixCut = 2;
            break;

        case RtlPathTypeLocalDevice:
            /* We made a good guess, go with it but skip the \??\ */
            PrefixCut = 4;
            break;

        case RtlPathTypeDriveAbsolute:
        case RtlPathTypeDriveRelative:
        case RtlPathTypeRooted:
        case RtlPathTypeRelative:
            /* Our guess was good, roll with it */
            break;

        /* Nothing else is expected */
        default:
            ASSERT(FALSE);
    }

    /* Now copy the prefix and the buffer */
    RtlCopyMemory(NewBuffer, PrefixBuffer, PrefixLength);
    RtlCopyMemory((PCHAR)NewBuffer + PrefixLength,
                  Buffer + PrefixCut,
                  PathLength - (PrefixCut * sizeof(WCHAR)));

    /* Compute the length */
    Length = PathLength + PrefixLength - PrefixCut * sizeof(WCHAR);
    LengthChars = Length / sizeof(WCHAR);

    /* Setup the actual NT path string and terminate it */
    NtName->Buffer = NewBuffer;
    NtName->Length = (USHORT)Length;
    NtName->MaximumLength = (USHORT)MaxLength;
    NewBuffer[LengthChars] = UNICODE_NULL;
    //DbgPrint("New buffer: %S\n", NewBuffer);
    //DbgPrint("NT Name: %wZ\n", NtName);

    /* Check if a partial name was requested */
    if ((PartName) && (*PartName))
    {
        /* Convert to Unicode */
        Status = RtlInitUnicodeStringEx(&PartNameString, *PartName);
        if (NT_SUCCESS(Status))
        {
            /* Set the partial name */
            *PartName = &NewBuffer[LengthChars - (PartNameString.Length / sizeof(WCHAR))];
        }
        else
        {
            /* Fail */
            RtlFreeHeap(RtlGetProcessHeap(), 0, NewBuffer);
            RtlReleasePebLock();
            return Status;
        }
    }

    /* Check if a relative name was asked for */
    if (RelativeName)
    {
        /* Setup the structure */
        RtlInitEmptyUnicodeString(&RelativeName->RelativeName, NULL, 0);
        RelativeName->ContainingDirectory = NULL;
        RelativeName->CurDirRef = NULL;

        /* Check if the input path itself was relative */
        if (InputPathType == RtlPathTypeRelative)
        {
            /* Get current directory */
            CurrentDirectory = &(NtCurrentPeb()->ProcessParameters->CurrentDirectory);
            if (CurrentDirectory->Handle)
            {
                Status = RtlInitUnicodeStringEx(&FullPath, Buffer);
                if (!NT_SUCCESS(Status))
                {
                    RtlFreeHeap(RtlGetProcessHeap(), 0, NewBuffer);
                    RtlReleasePebLock();
                    return Status;
                }

                /* If current directory is bigger than full path, there's no way */
                if (CurrentDirectory->DosPath.Length > FullPath.Length)
                {
                    RtlReleasePebLock();
                    return Status;
                }

                /* File is in current directory */
                if (RtlEqualUnicodeString(&FullPath, &CurrentDirectory->DosPath, TRUE))
                {
                    /* Make relative name string */
                    RelativeName->RelativeName.Buffer = (PWSTR)((ULONG_PTR)NewBuffer + PrefixLength + FullPath.Length - PrefixCut * sizeof(WCHAR));
                    RelativeName->RelativeName.Length = (USHORT)(PathLength - FullPath.Length);
                    /* If relative name starts with \, skip it */
                    if (RelativeName->RelativeName.Buffer[0] == OBJ_NAME_PATH_SEPARATOR)
                    {
                        RelativeName->RelativeName.Buffer++;
                        RelativeName->RelativeName.Length -= sizeof(WCHAR);
                    }
                    RelativeName->RelativeName.MaximumLength = RelativeName->RelativeName.Length;
                    DbgPrint("RelativeName: %wZ\n", &(RelativeName->RelativeName));

                    if (!HaveRelative)
                    {
                        RelativeName->ContainingDirectory = CurrentDirectory->Handle;
                        return Status;
                    }

                    /* Give back current directory data & reference counter */
                    RelativeName->CurDirRef = RtlpCurDirRef;
                    if (RelativeName->CurDirRef)
                    {
                        InterlockedIncrement(&RtlpCurDirRef->RefCount);
                    }

                    RelativeName->ContainingDirectory = CurrentDirectory->Handle;
                }
            }
        }
    }

    /* Done */
    RtlReleasePebLock();
    return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
RtlpDosPathNameToRelativeNtPathName_U( 	
	IN BOOLEAN  	HaveRelative,
	IN PCWSTR  	DosName,
	OUT PUNICODE_STRING  	NtName,
	OUT PCWSTR *  	PartName,
	OUT PRTL_RELATIVE_NAME_U  	RelativeName 
) 		
{
    NTSTATUS Status;
    UNICODE_STRING NameString;

    /* Create the unicode name */
    Status = RtlInitUnicodeStringEx(&NameString, DosName);
    if (NT_SUCCESS(Status))
    {
        /* Call the unicode function */
        Status = RtlpDosPathNameToRelativeNtPathName_Ustr(HaveRelative,
                                                          &NameString,
                                                          NtName,
                                                          PartName,
                                                          RelativeName);
    }

    /* Return status */
    return Status;
}

BOOLEAN NTAPI RtlDosPathNameToRelativeNtPathName_U( 	
		IN PCWSTR  	DosName,
		OUT PUNICODE_STRING  	NtName,
		OUT PCWSTR *  	PartName,
		OUT PRTL_RELATIVE_NAME_U  	RelativeName 
) 	
{
    // Call the internal function
    ASSERT(RelativeName);
    return NT_SUCCESS(RtlpDosPathNameToRelativeNtPathName_U(TRUE,
                                                            DosName,
                                                            NtName,
                                                            PartName,
                                                            RelativeName));
}

VOID NTAPI RtlReleaseRelativeName( 	
	IN PRTL_RELATIVE_NAME_U  	RelativeName	
) 	
{
    // Check if a directory reference was grabbed 
    if (RelativeName->CurDirRef)
    {
        // Decrease reference count 
        if (!InterlockedDecrement(&RelativeName->CurDirRef->RefCount))
        {
            // If no one uses it any longer, close handle & free 
            NtClose(RelativeName->CurDirRef->Handle);
            RtlFreeHeap(RtlGetProcessHeap(), 0, RelativeName->CurDirRef);
        }
        RelativeName->CurDirRef = NULL;
    }
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlDosPathNameToNtPathName_U_WithStatus(IN PCWSTR DosName,
                                        OUT PUNICODE_STRING NtName,
                                        OUT PCWSTR *PartName,
                                        OUT PRTL_RELATIVE_NAME_U RelativeName)
{
    /* Call the internal function */
    return RtlpDosPathNameToRelativeNtPathName_U(FALSE,
                                                 DosName,
                                                 NtName,
                                                 PartName,
                                                 RelativeName);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlDosPathNameToRelativeNtPathName_U_WithStatus(IN PCWSTR DosName,
                                                OUT PUNICODE_STRING NtName,
                                                OUT PCWSTR *PartName,
                                                OUT PRTL_RELATIVE_NAME_U RelativeName)
{
    /* Call the internal function */
    ASSERT(RelativeName);
    return RtlpDosPathNameToRelativeNtPathName_U(TRUE,
                                                 DosName,
                                                 NtName,
                                                 PartName,
                                                 RelativeName);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlGetFullPathName_UstrEx(IN PUNICODE_STRING FileName,
                          IN PUNICODE_STRING StaticString,
                          IN PUNICODE_STRING DynamicString,
                          IN PUNICODE_STRING *StringUsed,
                          IN PSIZE_T FilePartSize,
                          OUT PBOOLEAN NameInvalid,
                          OUT RTL_PATH_TYPE* PathType,
                          OUT PSIZE_T LengthNeeded)
{
    NTSTATUS Status;
    PWCHAR StaticBuffer;
    PCWCH ShortName;
    ULONG Length;
    USHORT StaticLength;
    UNICODE_STRING TempDynamicString;

    /* Initialize all our locals */
    ShortName = NULL;
    StaticBuffer = NULL;
    TempDynamicString.Buffer = NULL;

    /* Initialize the input parameters */
    if (StringUsed) *StringUsed = NULL;
    if (LengthNeeded) *LengthNeeded = 0;
    if (FilePartSize) *FilePartSize = 0;

    /* Check for invalid parameters */
    if ((DynamicString) && !(StringUsed) && (StaticString))
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* Check if we did not get an input string */
    if (!StaticString)
    {
        /* Allocate one */
        StaticLength = MAX_PATH * sizeof(WCHAR);
        StaticBuffer = RtlpAllocateStringMemory(MAX_PATH * sizeof(WCHAR), TAG_USTR);
        if (!StaticBuffer) return STATUS_NO_MEMORY;
    }
    else
    {
        /* Use the one we received */
        StaticBuffer = StaticString->Buffer;
        StaticLength = StaticString->MaximumLength;
    }

    /* Call the lower-level function */
    Length = RtlGetFullPathName_Ustr(FileName,
                                     StaticLength,
                                     StaticBuffer,
                                     &ShortName,
                                     NameInvalid,
                                     PathType);
    DbgPrint("Length: %u StaticBuffer: %S\n", Length, StaticBuffer);
    if (!Length)
    {
        /* Fail if it failed */
        DbgPrint("%s(%d) - RtlGetFullPathName_Ustr() returned 0\n",
                 __FUNCTION__,
                 __LINE__);
        Status = STATUS_OBJECT_NAME_INVALID;
        goto Quickie;
    }

    /* Check if it fits inside our static string */
    if ((StaticString) && (Length < StaticLength))
    {
        /* Set the final length */
        StaticString->Length = (USHORT)Length;

        /* Set the file part size */
        if (FilePartSize) *FilePartSize = ShortName ? (ShortName - StaticString->Buffer) : 0;

        /* Return the static string if requested */
        if (StringUsed) *StringUsed = StaticString;

        /* We are done with success */
        Status = STATUS_SUCCESS;
        goto Quickie;
    }

    /* Did we not have an input dynamic string ?*/
    if (!DynamicString)
    {
        /* Return the length we need */
        if (LengthNeeded) *LengthNeeded = Length;

        /* And fail such that the caller can try again */
        Status = STATUS_BUFFER_TOO_SMALL;
        goto Quickie;
    }

    /* Check if it fits in our static buffer */
    if ((StaticBuffer) && (Length < StaticLength))
    {
        /* NULL-terminate it */
        StaticBuffer[Length / sizeof(WCHAR)] = UNICODE_NULL;

        /* Set the settings for the dynamic string the caller sent */
        DynamicString->MaximumLength = StaticLength;
        DynamicString->Length = (USHORT)Length;
        DynamicString->Buffer = StaticBuffer;

        /* Set the part size */
        if (FilePartSize) *FilePartSize = ShortName ? (ShortName - StaticBuffer) : 0;

        /* Return the dynamic string if requested */
        if (StringUsed) *StringUsed = DynamicString;

        /* Do not free the static buffer on exit, and return success */
        StaticBuffer = NULL;
        Status = STATUS_SUCCESS;
        goto Quickie;
    }

    /* Now try again under the PEB lock */
    RtlAcquirePebLock();
    Length = RtlGetFullPathName_Ustr(FileName,
                                     StaticLength,
                                     StaticBuffer,
                                     &ShortName,
                                     NameInvalid,
                                     PathType);
    if (!Length)
    {
        /* It failed */
        DbgPrint("%s line %d: RtlGetFullPathName_Ustr() returned 0\n",
                 __FUNCTION__, __LINE__);
        Status = STATUS_OBJECT_NAME_INVALID;
        goto Release;
    }

    /* Check if it fits inside our static string now */
    if ((StaticString) && (Length < StaticLength))
    {
        /* Set the final length */
        StaticString->Length = (USHORT)Length;

        /* Set the file part size */
        if (FilePartSize) *FilePartSize = ShortName ? (ShortName - StaticString->Buffer) : 0;

        /* Return the static string if requested */
        if (StringUsed) *StringUsed = StaticString;

        /* We are done with success */
        Status = STATUS_SUCCESS;
        goto Release;
    }

    /* Check if the path won't even fit in a real string */
    if ((Length + sizeof(WCHAR)) > UNICODE_STRING_MAX_BYTES)
    {
        /* Name is way too long, fail */
        Status = STATUS_NAME_TOO_LONG;
        goto Release;
    }

    /* Allocate the string to hold the path name now */
    TempDynamicString.Buffer = RtlpAllocateStringMemory(Length + sizeof(WCHAR),
                                                        TAG_USTR);
    if (!TempDynamicString.Buffer)
    {
        /* Out of memory, fail */
        Status = STATUS_NO_MEMORY;
        goto Release;
    }

    /* Add space for a NULL terminator, and now check the full path */
    TempDynamicString.MaximumLength = (USHORT)Length + sizeof(UNICODE_NULL);
    Length = RtlGetFullPathName_Ustr(FileName,
                                     Length,
                                     TempDynamicString.Buffer,
                                     &ShortName,
                                     NameInvalid,
                                     PathType);
    if (!Length)
    {
        /* Some path error, so fail out */
        DbgPrint("%s line %d: RtlGetFullPathName_Ustr() returned 0\n",
                 __FUNCTION__, __LINE__);
        Status = STATUS_OBJECT_NAME_INVALID;
        goto Release;
    }

    /* It should fit in the string we just allocated */
    ASSERT(Length < (TempDynamicString.MaximumLength - sizeof(WCHAR)));
    if (Length > TempDynamicString.MaximumLength)
    {
        /* This is really weird and would mean some kind of race */
        Status = STATUS_INTERNAL_ERROR;
        goto Release;
    }

    /* Return the file part size */
    if (FilePartSize) *FilePartSize = ShortName ? (ShortName - TempDynamicString.Buffer) : 0;

    /* Terminate the whole string now */
    TempDynamicString.Buffer[Length / sizeof(WCHAR)] = UNICODE_NULL;

    /* Finalize the string and return it to the user */
    DynamicString->Buffer = TempDynamicString.Buffer;
    DynamicString->Length = (USHORT)Length;
    DynamicString->MaximumLength = TempDynamicString.MaximumLength;
    if (StringUsed) *StringUsed = DynamicString;

    /* Return success and make sure we don't free the buffer on exit */
    TempDynamicString.Buffer = NULL;
    Status = STATUS_SUCCESS;

Release:
    /* Release the PEB lock */
    RtlReleasePebLock();

Quickie:
    /* Free any buffers we should be freeing */
    DbgPrint("Status: %lx %S %S\n", Status, StaticBuffer, TempDynamicString.Buffer);
    if ((StaticString) && (StaticBuffer) && (StaticBuffer != StaticString->Buffer))
    {
        RtlpFreeMemory(StaticBuffer, TAG_USTR);
    }
    if (TempDynamicString.Buffer)
    {
        RtlpFreeMemory(TempDynamicString.Buffer, TAG_USTR);
    }

    /* Print out any unusual errors */
    if ((NT_ERROR(Status)) &&
        (Status != STATUS_NO_SUCH_FILE) && (Status != STATUS_BUFFER_TOO_SMALL))
    {
        DbgPrint("RTL: %s - failing on filename %wZ with status %08lx\n",
                __FUNCTION__, FileName, Status);
    }

    /* Return, we're all done */
    return Status;
}