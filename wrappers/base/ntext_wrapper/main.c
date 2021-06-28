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

extern RTL_CRITICAL_SECTION time_tz_section;
extern RTL_CRITICAL_SECTION localeCritSection;
extern RTL_CRITICAL_SECTION loader_section;
extern RTL_CRITICAL_SECTION dlldir_section;
VOID RtlpInitializeKeyedEvent(VOID);
VOID RtlpCloseKeyedEvent(VOID);
void load_global_options(void);
void init_locale();
static UNICODE_STRING Kernel32DllName = RTL_CONSTANT_STRING(L"kernel32.dll");

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
		RtlInitializeCriticalSection(&time_tz_section);
		RtlInitializeCriticalSection(&localeCritSection);
		RtlInitializeCriticalSection(&loader_section);
		RtlInitializeCriticalSection(&dlldir_section);
		load_global_options();		
		init_locale();
        LdrDisableThreadCalloutsForDll(hDll);
        RtlpInitializeKeyedEvent();
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        RtlpCloseKeyedEvent();
    }	
	
    return TRUE;
}