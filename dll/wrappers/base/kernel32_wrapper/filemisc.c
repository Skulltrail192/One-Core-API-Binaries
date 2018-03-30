/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    filemisc.c

Abstract:

    Misc file operations for Win32

Author:

    Skulltrail 01-December-2017

Revision History:

--*/

#include <main.h>

BOOL 
WINAPI 
MoveFileTransactedA(
	LPCTSTR lpExistingFileName, 
	LPCTSTR lpNewFileName, 
	LPPROGRESS_ROUTINE lpProgressRoutine, 
	LPVOID lpData, 
	DWORD dwFlags, 
	HANDLE hTransaction
)
{
	return MoveFileWithProgressA(lpExistingFileName,
								lpNewFileName,
								lpProgressRoutine,
								lpData,
								dwFlags);
}

BOOL 
WINAPI 
MoveFileTransactedW(
	LPCWSTR lpExistingFileName, 
	LPCWSTR lpNewFileName, 
	LPPROGRESS_ROUTINE lpProgressRoutine, 
	LPVOID lpData, 
	DWORD dwFlags, 
	HANDLE hTransaction
)
{
	return MoveFileWithProgressW(lpExistingFileName,
								lpNewFileName,
								lpProgressRoutine,
								lpData,
								dwFlags);
}

BOOL 
WINAPI
DeleteFileTransactedW(
	LPCWSTR lpFileName, 
	HANDLE hTransaction
)
{
    return DeleteFileW(lpFileName);
}

BOOL 
WINAPI
DeleteFileTransactedA(
	LPCSTR lpFileName, 
	HANDLE hTransaction
)
{
    return DeleteFileA(lpFileName);
}