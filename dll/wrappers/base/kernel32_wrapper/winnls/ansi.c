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