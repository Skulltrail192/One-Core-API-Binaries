/*++

Copyright (c) 2017 Shorthorn Project.

Module Name:

    ansi.c

Abstract:

    This file contains the ANSI versions of the NLS API functions.

Revision History:
--*/


#include <main.h>
	
/******************************************************************************
 *           CompareStringOrdinal    (KERNEL32.@)
 */
INT 
WINAPI 
CompareStringOrdinal(
	const WCHAR *str1, 
	INT len1, 
	const WCHAR *str2, 
	INT len2, 
	BOOL ignore_case
)
{
    int ret;

    if (!str1 || !str2)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }
    if (len1 < 0) len1 = strlenW(str1);
    if (len2 < 0) len2 = strlenW(str2);

    ret = RtlCompareUnicodeStrings( str1, len1, str2, len2, ignore_case );
    if (ret < 0) return CSTR_LESS_THAN;
    if (ret > 0) return CSTR_GREATER_THAN;
    return CSTR_EQUAL;
}

/******************************************************************************
 *           FindStringOrdinal (KERNEL32.@)
 */

INT WINAPI FindStringOrdinal(DWORD flag, const WCHAR *src, INT src_size, const WCHAR *val, INT val_size,
                             BOOL ignore_case)
{
    INT offset, inc, count;

    if (!src || !val)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return -1;
    }

    if (flag != FIND_FROMSTART && flag != FIND_FROMEND && flag != FIND_STARTSWITH && flag != FIND_ENDSWITH)
    {
        SetLastError(ERROR_INVALID_FLAGS);
        return -1;
    }

    if (src_size == -1) src_size = strlenW(src);
    if (val_size == -1) val_size = strlenW(val);

    src_size -= val_size;
    if (src_size < 0)
    {
        SetLastError(NO_ERROR);
        return -1;
    }

    count = flag & (FIND_FROMSTART | FIND_FROMEND) ? src_size + 1 : 1;
    offset = flag & (FIND_FROMSTART | FIND_STARTSWITH) ? 0 : src_size;
    inc = flag & (FIND_FROMSTART | FIND_STARTSWITH) ? 1 : -1;
    while (count--)
    {
        if (CompareStringOrdinal(src + offset, val_size, val, val_size, ignore_case) == CSTR_EQUAL)
        {
            SetLastError(NO_ERROR);
            return offset;
        }
        offset += inc;
    }

    SetLastError(NO_ERROR);
    return -1;
}