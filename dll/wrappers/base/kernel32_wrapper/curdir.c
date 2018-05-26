/*++

Copyright (c) 2018  Shorthorn Project

Module Name:

    curdir.c

Abstract:

    Current directory support

Author:

    Skulltrail 21-April-2018

Revision History:

--*/

#include "main.h"

DWORD 
WINAPI 
GetFullPathNameTransactedA(
	LPCSTR lpFileName, 
	DWORD nBufferLength, 
	LPSTR lpBuffer, 
	LPSTR *lpFilePart, 
	HANDLE hTransaction
)
{
	return GetFullPathNameA(lpFileName, nBufferLength, lpBuffer, lpFilePart);
}

DWORD
WINAPI 
GetFullPathNameTransactedW(
	LPCWSTR lpFileName,
	DWORD nBufferLength, 
	LPWSTR lpBuffer, 
	LPWSTR *lpFilePart, 
	HANDLE hTransaction
)
{
	return GetFullPathNameW(lpFileName, nBufferLength, lpBuffer, lpFilePart);
}