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
static UNICODE_STRING Kernel32DllName = RTL_CONSTANT_STRING(L"kernel32.dll");

/******************************************************************
 *		LdrInitializeThunk (NTDLL.@)
 *
 * Attach to all the loaded dlls.
 * If this is the first time, perform the full process initialization.
 */
VOID
NTAPI
LdrInitializeThunkInternal(
    ULONG Unknown1,
    ULONG Unknown2,
    ULONG Unknown3,
    ULONG Unknown4
)
{
	load_global_options();		
	init_locale();
    //LdrDisableThreadCalloutsForDll(hDll);
    RtlpInitializeKeyedEvent();	
	LdrInitializeThunk(Unknown1, Unknown2, Unknown3, Unknown4);
}