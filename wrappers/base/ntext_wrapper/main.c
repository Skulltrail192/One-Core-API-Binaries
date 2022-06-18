/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    main.c

Abstract:

    main file

Author:

    Skulltrail 17-March-2018

Revision History:

--*/

#include <main.h>

VOID RtlpInitializeKeyedEvent(VOID);
VOID RtlpCloseKeyedEvent(VOID);
void load_global_options(void);
void init_locale();

/*****************************************************
 *      DllMain
 */
BOOL 
WINAPI 
LdrInitialize(
	HANDLE hDll, 
	DWORD dwReason, 
	LPVOID reserved
)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
		// RtlInitializeCriticalSection(&time_tz_section);
		// RtlInitializeCriticalSection(&localeCritSection);
		// RtlInitializeCriticalSection(&loader_section);
		// RtlInitializeCriticalSection(&dlldir_section);
		load_global_options();		
		init_locale();
        // LdrDisableThreadCalloutsForDll(hDll);
        RtlpInitializeKeyedEvent();
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        RtlpCloseKeyedEvent();
    }	
	
    return TRUE;
}