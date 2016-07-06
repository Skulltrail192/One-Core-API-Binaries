/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */

#include <assert.h>
#include <rtl.h>

int WINAPI WinSqmEventEnabled()
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
    DbgPrint("NIMPLEMENTED: WinSqmEndSession");
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
    DbgPrint("NIMPLEMENTED: WinSqmIsOptedIn");
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
    DbgPrint("NIMPLEMENTED: WinSqmStartSession");
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
    DbgPrint("NIMPLEMENTED: WinSqmAddToStream");		
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
    DbgPrint("NIMPLEMENTED: WinSqmAddToStreamEx");	
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
	DbgPrint("UNIMPLEMENTED: WinSqmSetDWORD");		
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
	DbgPrint("UNIMPLEMENTED: WinSqmIncrementDWORD");		
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
	DbgPrint("UNIMPLEMENTED: WinSqmSetIfMaxDWORD");		
	return 0;
}

BOOL 
NTAPI 
WinSqmIsOptedInEx(ULONG number)
{
	DbgPrint("UNIMPLEMENTED: WinSqmIsOptedInEx");	
	return TRUE;
}