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
#include <main.h>
 
extern USHORT NlsAnsiCodePage = 0; /* exported */
extern BOOLEAN NlsMbCodePageTag = FALSE; /* exported */
extern BOOLEAN NlsMbOemCodePageTag = FALSE;
//HANDLE Key_Event;

/*****************************************************
 *      DllMain
 */
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved)
{
    DbgPrint("DllMain called\n");
	//NtCreateKeyedEvent(&Key_Event, -1, NULL, 0);

    switch(reason)
    {
    case DLL_PROCESS_ATTACH:
        //DisableThreadLibraryCalls( hinst );
        break;
    }
    return TRUE;
}

/*Functions ********************************************/

/*
ULONG 
NTAPI 
RtlExtendHeap( 	
	IN HANDLE  	Heap,
	IN ULONG  	Flags,
	IN PVOID  	P,
	IN SIZE_T  	Size 
) 		
{
    // TODO 
    DbgPrint("RtlExtendHeap is unimplemented\n");
    return 0;
}

NTSTATUS 
NTAPI 
RtlUsageHeap( 	
	IN HANDLE  	Heap,
	IN ULONG  	Flags,
	OUT PRTL_HEAP_USAGE  	Usage 
) 		
{
    // TODO 
    DbgPrint("RtlUsageHeap is unimplemented\n");
    return STATUS_NOT_IMPLEMENTED;
}
//Maybe need for universal code
NTSTATUS 
NTAPI 
RtlAcquirePrivilege( 	
	IN PULONG  	Privilege,
	IN ULONG  	NumPriv,
	IN ULONG  	Flags,
	OUT PVOID *  	ReturnedState 
) 		
{
    return STATUS_NOT_IMPLEMENTED;
}
//Maybe need for universal code
VOID NTAPI RtlReleasePrivilege 	( 	IN PVOID  	ReturnedState	) 	
{
;
}*/