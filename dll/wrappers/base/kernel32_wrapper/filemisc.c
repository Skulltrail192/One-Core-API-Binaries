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

BOOL 
WINAPI
SetFileAttributesTransactedA(
  _In_ LPCTSTR lpFileName,
  _In_ DWORD   dwFileAttributes,
  _In_ HANDLE  hTransaction
)
{
	return SetFileAttributesA(lpFileName, dwFileAttributes);
}

BOOL 
WINAPI
SetFileAttributesTransactedW(
  _In_ LPCWSTR lpFileName,
  _In_ DWORD   dwFileAttributes,
  _In_ HANDLE  hTransaction
)
{
	return SetFileAttributesW(lpFileName, dwFileAttributes);
}

DWORD 
WINAPI 
GetCompressedFileSizeTransactedW(
	LPCWSTR lpFileName,
	LPDWORD lpFileSizeHigh, 
	HANDLE hTransaction
)
{
	return GetCompressedFileSizeW(lpFileName, lpFileSizeHigh);
}

DWORD 
WINAPI 
GetCompressedFileSizeTransactedA(
	LPCTSTR lpFileName, 
	LPDWORD lpFileSizeHigh, 
	HANDLE hTransaction
)
{
	return GetCompressedFileSizeA(lpFileName, lpFileSizeHigh);
}

/*
 * @implemented - need test
 */
BOOL 
WINAPI 
GetFileAttributesTransactedW(
	LPCWSTR lpFileName, 
	GET_FILEEX_INFO_LEVELS fInfoLevelId, 
	LPVOID lpFileInformation,
	HANDLE hTransaction
)
{
    return GetFileAttributesExW(lpFileName, fInfoLevelId, lpFileInformation);
}


BOOL 
WINAPI 
GetFileAttributesTransactedA(
	LPCSTR lpFileName, 
	GET_FILEEX_INFO_LEVELS fInfoLevelId, 
	LPVOID lpFileInformation, 
	HANDLE hTransaction
)
{
    return GetFileAttributesExA(lpFileName, fInfoLevelId, lpFileInformation);
}