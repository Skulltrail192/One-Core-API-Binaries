/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    winlink.c

Abstract:

    This module implements Win32 CreateHardLink

Author:

    Skulltrail 21-April-2018

Revision History:

--*/

#include "main.h"

BOOL 
WINAPI 
CreateHardLinkTransactedA(
	LPCTSTR lpFileName, 
	LPCTSTR lpExistingFileName, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
	HANDLE hTransaction
)
{
	return CreateHardLinkA(lpFileName,
						   lpExistingFileName,
						   lpSecurityAttributes);
}

BOOL 
WINAPI 
CreateHardLinkTransactedW(
	LPCWSTR lpFileName, 
	LPCWSTR lpExistingFileName, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
	HANDLE hTransaction
)
{
	return CreateHardLinkW(lpFileName,
						   lpExistingFileName,
						   lpSecurityAttributes);
}