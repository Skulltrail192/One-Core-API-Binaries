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
RTL_CRITICAL_SECTION  gcsTblPtrs;           // critical section for tbl ptrs
PLOC_HASH             gpSysLocHashN;        // ptr to system loc hash node
PLOC_HASH             gpInvLocHashN;        // ptr to invariant loc hash node
PNLS_USER_INFO        pNlsUserInfo;         // ptr to the user info cache
LCID gSystemLocale;        // system locale value
NLS_USER_INFO         gProcUserInfo;        // The user info cache for the process.
HANDLE                hAltSortsKey;         // handle to Locale\Alternate Sorts key

//
//  Global Variables.
//

PTBL_PTRS  pTblPtrs;              // ptr to structure of table ptrs

ULONG NlsProcessInitialize(void){
	NTSTATUS status;
	ULONG rc;
	status = NtQueryDefaultLocale(FALSE, &gSystemLocale);	
	
	if(!NT_SUCCESS(status)){
		DbgPrint("Kernel32: Error initializing NLS for System Default Locale\n");
		return FALSE;
	}
	
	status = RtlInitializeCriticalSectionAndSpinCount(&gcsTblPtrs, 4000);
	
	
	if(!NT_SUCCESS(status)){
		DbgPrint("Kernel32: Error initializing NLS for Initialize Critical Section\n");
		return FALSE;
	}
	RtlEnterCriticalSection(&gcsTblPtrs);	
	
/* 	if (rc = GetLocaleFileInfo( gSystemLocale,
                                &gpSysLocHashN,
                                TRUE ))
    {
        //
        //  Change the system locale to be the default (English).
        //
        if (GetLocaleFileInfo( MAKELCID(NLS_DEFAULT_LANGID, SORT_DEFAULT),
                               &gpSysLocHashN,
                               TRUE ))
        {
            DbgPrint("Couldn't do English\n");
            RtlLeaveCriticalSection(&gcsTblPtrs);
            return (rc);
        }		
	}	 */
	
	pNlsUserInfo = &gProcUserInfo;
	
    //
    //  Initialize the handles to the various registry keys to NULL.
    //
    hAltSortsKey = NULL;	
	
	return TRUE;
}

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
			kernel32_handle = GetModuleHandleW(L"kernelfull");
			NlsProcessInitialize(); // Initialize NLS
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

/***********************************************************************
 *          QuirkIsEnabled3 (KERNELBASE.@)
 */
BOOL WINAPI QuirkIsEnabled3(void *unk1, void *unk2)
{
    return FALSE;
}