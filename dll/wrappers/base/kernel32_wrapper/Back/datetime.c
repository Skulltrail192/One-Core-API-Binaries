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

/*
* @semi-implemented need test
*/
int 
WINAPI 
GetTimeFormatEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       DWORD dwFlags,
  _In_opt_   const SYSTEMTIME *lpTime,
  _In_opt_   LPCWSTR lpFormat,
  _Out_opt_  LPWSTR lpTimeStr,
  _In_       int cchTime
)
{
	LCID locale = LocaleNameToLCID(lpLocaleName, 0);
	return GetTimeFormatW(locale,
						  dwFlags,
						  lpTime,
						  lpFormat,
						  lpTimeStr,
						  cchTime);
}

int 
WINAPI 
GetDateFormatEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       DWORD dwFlags,
  _In_opt_   const SYSTEMTIME *lpDate,
  _In_opt_   LPCWSTR lpFormat,
  _Out_opt_  LPWSTR lpDateStr,
  _In_       int cchDate,
  _In_opt_   LPCWSTR lpCalendar
)
{
	LCID locale = LocaleNameToLCID(lpLocaleName, 0);
	return GetDateFormatW(locale,
						  dwFlags,
						  lpDate,
						  lpFormat,
						  lpDateStr,
						  cchDate);	
}

int 
WINAPI 
GetCalendarInfoEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       CALID Calendar,
  _In_opt_   LPCWSTR lpReserved,
  _In_       CALTYPE CalType,
  _Out_opt_  LPWSTR lpCalData,
  _In_       int cchData,
  _Out_opt_  LPDWORD lpValue
)
{	
	LCID locale = LocaleNameToLCID(lpLocaleName, 0);
	return GetCalendarInfoW(locale ,
							Calendar,
							CalType,
							lpCalData,
							cchData,
							lpValue);
}