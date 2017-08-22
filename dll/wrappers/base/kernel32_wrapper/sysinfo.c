/*++

Copyright (c) 2017  Shorthorn Project

Module Name:

    sysinfo.c

Abstract:

    This module implements System Information APIs for Win32

Author:

    Skulltrail 11-April-2017

Revision History:

--*/

#include <main.h>

/*
* @unimplemented - need implementation
*/
BOOL 
WINAPI 
GetOSProductNameW(
	PCWSTR Source, 
	ULONG var, 
	ULONG parameter
)
{
	Source = L"Microsoft Windows Codename \"Longhorn\" Professional Version 2003 Copyright ";
	return TRUE;
}

/*
* @unimplemented - need implementation
*/
BOOL 
WINAPI 
GetOSProductNameA(
	PCSTR Source, 
	ULONG var, 
	ULONG parameter
)
{
	Source = "Microsoft Windows Codename \"Longhorn\" Professional Version 2003 Copyright ";
	return TRUE;
}

/*
 * @implemented - new
 */
BOOL 
WINAPI 
GetProductInfo(
  _In_   DWORD dwOSMajorVersion,
  _In_   DWORD dwOSMinorVersion,
  _In_   DWORD dwSpMajorVersion,
  _In_   DWORD dwSpMinorVersion,
  _Out_  PDWORD pdwReturnedProductType
)
{
	return RtlGetProductInfo(dwOSMajorVersion, 
							 dwOSMinorVersion,
                             dwSpMajorVersion, 
							 dwSpMinorVersion, 
							 pdwReturnedProductType);
}