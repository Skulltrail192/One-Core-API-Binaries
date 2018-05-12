/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    sqm.c

Abstract:

    Implement sqm related functions

Author:

    Skulltrail 06-March-2018

Revision History:

--*/

#include <assert.h>
#include <rtl.h>

int 
WINAPI
WinSqmEventEnabled()
{
  return 0;
}

/******************************************************************************
 *                  WinSqmEndSession   (NTDLL.@)
 */
NTSTATUS 
WINAPI 
WinSqmEndSession(
	HANDLE session
)
{
    DbgPrint("UNIMPLEMENTED: WinSqmEndSession");
    return STATUS_NOT_IMPLEMENTED;
}

/*********************************************************************
 *                  WinSqmIsOptedIn   (NTDLL.@)
 */
BOOL 
WINAPI 
WinSqmIsOptedIn(
	void
)
{
    DbgPrint("UNIMPLEMENTED: WinSqmIsOptedIn");
    return FALSE;
}

/******************************************************************************
 *                  WinSqmStartSession   (NTDLL.@)
 */
HANDLE 
WINAPI 
WinSqmStartSession(
	GUID *sessionguid, 
	DWORD sessionid, 
	DWORD unknown1
)
{
    DbgPrint("UNIMPLEMENTED: WinSqmStartSession");
    return INVALID_HANDLE_VALUE;
}

/*unimplemented */
PVOID 
WINAPI
WinSqmAddToStream(
	PVOID Source1, 
	BOOL unknown1, 
	DWORD unknown2, 
	PVOID unknown3
)
{
    DbgPrint("UNIMPLEMENTED: WinSqmAddToStream");		
	return NULL;
}

/*unimplemented */
BOOL 
WINAPI 
WinSqmAddToStreamEx(
	PVOID unknown1, 
	DWORD unknown2, 
	ULONG unknown3, 
	DWORD unknown4, 
	DWORD unknown5
)
{
    DbgPrint("UNIMPLEMENTED: WinSqmAddToStreamEx");	
	return TRUE;
}

/*unimplemented */
void 
WINAPI 
WinSqmSetDWORD(
	DWORD unknown1, 
	DWORD unknown2, 
	DWORD unknown3
)
{
	DbgPrint("UUNIMPLEMENTED: WinSqmSetDWORD");		
  ;
}

/*unimplemented */
int  
WINAPI 
WinSqmIncrementDWORD(
	DWORD unknown1, 
	DWORD unknown2, 
	DWORD unknown3
)
{
	DbgPrint("UUNIMPLEMENTED: WinSqmIncrementDWORD");		
	return 0;
}

/*unimplemented */
int  
WINAPI
WinSqmSetIfMaxDWORD(
	DWORD unknown1, 
	DWORD unknown2, 
	DWORD unknown3
)
{
	DbgPrint("UUNIMPLEMENTED: WinSqmSetIfMaxDWORD");		
	return 0;
}

BOOL 
NTAPI 
WinSqmIsOptedInEx(ULONG number)
{
	DbgPrint("UUNIMPLEMENTED: WinSqmIsOptedInEx");	
	return TRUE;
}

int 
NTAPI 
WinSqmAddToAverageDWORD(
	DWORD unknown1, 
	DWORD unknown2, 
	DWORD unknown3
)
{
	DbgPrint("UUNIMPLEMENTED: WinSqmSetIfMaxDWORD");		
	return 0;
}

int NTAPI 
WinSqmSetString(void *a1, char a2, const wchar_t *a3)
{
	return 0;
}