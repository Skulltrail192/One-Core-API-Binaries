/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    number.c

Abstract:

    This file contains functions that form properly formatted number and
    currency strings for a given locale.


Author:

    Skulltrail 18-March-2017
	
--*/

#include "main.h"

/**************************************************************************
 *              GetNumberFormatEx	(KERNEL32.@)
 */
INT 
WINAPI 
GetNumberFormatEx(
	LPCWSTR name, 
	DWORD flags,
    LPCWSTR value, 
	const NUMBERFMTW *format,
    LPWSTR number, int numout
)
{
  LCID lcid  = 0x0409;	

  lcid = LocaleNameToLCID(name, 0);
  if (!lcid)
    return 0;

  return GetNumberFormatW(lcid, flags, value, format, number, numout);
}


int 
WINAPI
GetCurrencyFormatEx(
  _In_opt_        LPCWSTR     lpLocaleName,
  _In_            DWORD       dwFlags,
  _In_            LPCWSTR     lpValue,
  _In_opt_  const CURRENCYFMT *lpFormat,
  _Out_opt_       LPWSTR      lpCurrencyStr,
  _In_            int         cchCurrency
)
{
  LCID lcid  = 0x0409;	

  lcid = LocaleNameToLCID(lpLocaleName, 0);
  if (!lcid)
    return 0;

  return GetCurrencyFormatW(lcid, dwFlags, lpValue, (const CURRENCYFMTW *)lpFormat, lpCurrencyStr, cchCurrency);
}
