/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    syblink.c

Abstract:

    This module implements Win32 utilities for others functions APIs

Author:

    Skulltrail 25-March-2017

Revision History:

--*/

#include "main.h"

/*
 * @implemented
 */
VOID
WINAPI
SetpFileApisToANSI(VOID)
{
    /* Set the correct Base Api */
    // Basep8BitStringToUnicodeString = RtlAnsiStringToUnicodeString;
    // BasepUnicodeStringTo8BitString = RtlUnicodeStringToAnsiString;
    // BasepUnicodeStringTo8BitSize = BasepUnicodeStringToAnsiSize;
    // Basep8BitStringToUnicodeSize = BasepAnsiStringToUnicodeSize;

    /* FIXME: Old, deprecated way */
    bIsFileApiAnsi = TRUE;
	SetFileApisToANSI();
}

/*
 * @implemented
 */
VOID
WINAPI
SetpFileApisToOEM(VOID)
{
    /* Set the correct Base Api */
    // Basep8BitStringToUnicodeString = (PRTL_CONVERT_STRING)RtlOemStringToUnicodeString;
    // BasepUnicodeStringTo8BitString = RtlUnicodeStringToOemString;
    // BasepUnicodeStringTo8BitSize = BasepUnicodeStringToOemSize;
    // Basep8BitStringToUnicodeSize = BasepOemStringToUnicodeSize;

    /* FIXME: Old, deprecated way */
    bIsFileApiAnsi = FALSE;
	SetFileApisToOEM();
}