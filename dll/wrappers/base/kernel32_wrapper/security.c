/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    security.c

Abstract:

    This module implements the Win32 security checks.

Author:

    Skulltrail 20-May-2017

Revision History:

--*/

#include <main.h>

/*
* @unimplemented
*/
DWORD 
WINAPI 
CheckElevationEnabled(
    BOOL *pResult
) 
{
	*pResult = FALSE;
	return S_OK;
}

/*
* @unimplemented
*/
DWORD 
WINAPI 
CheckElevation(
   LPCWSTR lpApplicationName,
   PDWORD pdwFlags,
   HANDLE hChildToken,
   PDWORD pdwRunLevel,
   PDWORD pdwReason
)
{
	return S_OK;
}