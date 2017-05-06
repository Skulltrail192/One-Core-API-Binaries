/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    dllmain.c

Abstract:

    This module initialize Win32 API Base

Author:

    Skulltrail 20-March-2017

Revision History:

--*/


#include "main.h"

static BOOL DllInitialized = FALSE;
PPEB Peb;
HMODULE kernel32_handle = 0;

BOOL
WINAPI
BaseDllInitialize(HANDLE hDll,
        DWORD dwReason,
        LPVOID lpReserved)
{
    NTSTATUS Status;
    WCHAR SessionDir[256];

    /* Cache the PEB and Session ID */
    Peb = NtCurrentPeb();

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            /* Insert more dll attach stuff here! */
			kernel32_handle = GetModuleHandleW(L"kernel32");
            DllInitialized = TRUE;
            break;
        }

        case DLL_PROCESS_DETACH:
        {
            if (DllInitialized == TRUE)
            {
				DllInitialized = FALSE;
            }
            break;
        }
		
        case DLL_THREAD_ATTACH:
        {

        }

        default:
            break;
    }

    return TRUE;
}