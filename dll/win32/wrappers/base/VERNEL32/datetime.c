/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    datetime.c

Abstract:

    This module implements Win32 time of day functions

Author:

    Skulltrail 20-March-2017

Revision History:

--*/

#include "main.h"

/*
* @implemented
*/
BOOL
WINAPI
IsTimeZoneRedirectionEnabled(VOID)
{
     /* Return if a TS Timezone ID is active */
     return (BaseStaticServerData->TermsrvClientTimeZoneId != TIME_ZONE_ID_INVALID);
}

/***********************************************************************
 *              GetSystemTimePreciseAsFileTime  (KERNEL32.@)
 *
 *  Get the current time in utc format, with <1 us precision.
 *
 *  RETURNS
 *   Nothing.
 */
VOID 
WINAPI 
GetSystemTimePreciseAsFileTime(
	LPFILETIME lpSystemTimeAsFileTime
)
{
  GetSystemTimeAsFileTime(lpSystemTimeAsFileTime);
}