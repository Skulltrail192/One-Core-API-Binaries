/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    misc.c

Abstract:

    Miscellaneous functions

Author:

    Skulltrail 06-March-2018

Revision History:

--*/

 
#define NDEBUG

#include <main.h>

//unknown prototype
ULONG 
WINAPI 
ShipAssert(
	DWORD a1, 
	DWORD a2
)
{
	return 0;
}

ULONG 
WINAPI 
ShipAssertMsgA(DWORD a1, DWORD a2, LPCSTR a3)
{
	return ShipAssert(a1, a2);
}

ULONG 
WINAPI 
ShipAssertMsgW(DWORD a1, DWORD a2, LPWSTR a3)
{
	return ShipAssert(a1, a2);
}