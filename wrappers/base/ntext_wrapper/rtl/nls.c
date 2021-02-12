/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    nls.c

Abstract:

    This module implements NLS support functions for NT.

Author:

    Skulltrail 16-March-2017

Environment:

    Kernel or user-mode

Revision History:

--*/

#include <main.h> 
#include <unicode.h>

USHORT NlsAnsiCodePage = 0; /* exported */
BOOLEAN NlsMbCodePageTag = FALSE; /* exported */
BOOLEAN NlsMbOemCodePageTag = FALSE;

/******************************************************************************
 *	RtlCompareUnicodeStrings   (NTDLL.@)
 */
LONG 
WINAPI 
RtlCompareUnicodeStrings( 
	const WCHAR *s1, 
	SIZE_T len1, 
	const WCHAR *s2, 
	SIZE_T len2,
    BOOLEAN case_insensitive 
)
{
    LONG ret = 0;
    SIZE_T len = min( len1, len2 );
	
	NtCurrentTeb()->CurrentTransactionHandle;

    if (case_insensitive)
    {
        while (!ret && len--) ret = toupperW(*s1++) - toupperW(*s2++);
    }
    else
    {
        while (!ret && len--) ret = *s1++ - *s2++;
    }
    if (!ret) ret = len1 - len2;
    return ret;
}

