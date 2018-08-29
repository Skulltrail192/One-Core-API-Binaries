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

extern RTL_CRITICAL_SECTION TIME_tz_section;
extern RTL_CRITICAL_SECTION LocaleCritSection;
extern RTL_CRITICAL_SECTION loader_section;

VOID RtlpInitializeKeyedEvent(VOID);
VOID RtlpCloseKeyedEvent(VOID);

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
		RtlInitializeCriticalSection(&TIME_tz_section);
		RtlInitializeCriticalSection(&LocaleCritSection);
		RtlInitializeCriticalSection(&loader_section);
        LdrDisableThreadCalloutsForDll(hDll);
        RtlpInitializeKeyedEvent();
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        RtlpCloseKeyedEvent();
    }	
	
    return TRUE;
}