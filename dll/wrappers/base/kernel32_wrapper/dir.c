/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    dir.c

Abstract:

    This module implements Win32 Directory functions.

Author:

    Skulltrail 18-April-2018

Revision History:

--*/

#include "main.h"

BOOL WINAPI RemoveDirectoryTransactedW(LPCWSTR lpPathName, HANDLE hTransaction)
{
  return RemoveDirectoryW(lpPathName);
}

BOOL
WINAPI 
RemoveDirectoryTransactedA(LPCTSTR lpPathName, HANDLE hTransaction)
{
  return RemoveDirectoryA(lpPathName);
}
