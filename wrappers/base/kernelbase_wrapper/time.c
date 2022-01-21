/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    time.c

Abstract:

    This module implements Time functions for the Win32 APIs.

Author:

    Skulltrail 06-May-2017

Revision History:

--*/

#include <main.h>

/***********************************************************************
 *           QueryUnbiasedInterruptTime   (KERNEL32.@)
 */
BOOL 
WINAPI 
QueryUnbiasedInterruptTime(ULONGLONG *time)
{
    if (!time) return FALSE;
    RtlQueryUnbiasedInterruptTime(time);
    return TRUE;
}