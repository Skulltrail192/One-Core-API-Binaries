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

/*
 * @implemented
 */
PLARGE_INTEGER 
WINAPI 
BaseFormatTimeOut(
	OUT PLARGE_INTEGER  	Timeout,
	IN DWORD  	dwMilliseconds 
) 		
{
    /* Check if this is an infinite wait, which means no timeout argument */
    if (dwMilliseconds == INFINITE) return NULL;

    /* Otherwise, convert the time to NT Format */
    Timeout->QuadPart = UInt32x32To64(dwMilliseconds, -10000);
    return Timeout;
}