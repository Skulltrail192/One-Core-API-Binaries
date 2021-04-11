/*++

Copyright (c) 2021 Shorthorn Project

Module Name:

    atom.c

Abstract:

    This module contains the Win32 Atom Management APIs

Author:

    Skulltrail 25-March-2021

Revision History:

--*/

#include "main.h"

ATOM GlobalAddAtomExA(
  LPCSTR lpString,
  DWORD  Flags //Ignored for now
)
{
	return GlobalAddAtomA(lpString);
}

ATOM GlobalAddAtomExW(
  LPCWSTR lpString,
  DWORD  Flags //Ignored for now
)
{
	return GlobalAddAtomA(lpString);
}