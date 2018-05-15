/*++

Copyright (c) 2018  Shorthorn Project

Module Name:

    vdm.c

Abstract:

    This module implements Win32 APIs for VDMs

Author:

    Skulltrail 21-April-2018

Revision History:

--*/

#include "main.h"

DWORD 
WINAPI 
GetLongPathNameTransactedA(
	LPCSTR lpszShortPath, 
	LPSTR lpszLongPath, 
	DWORD cchBuffer, 
	HANDLE hTransaction
)
{
	return GetLongPathNameA(lpszShortPath, lpszLongPath, cchBuffer);
}

DWORD 
WINAPI 
GetLongPathNameTransactedW(
	LPCWSTR lpszShortPath, 
	LPWSTR lpszLongPath, 
	DWORD cchBuffer, 
	HANDLE hTransaction
)
{
	return GetLongPathNameW(lpszShortPath, lpszLongPath, cchBuffer);
}