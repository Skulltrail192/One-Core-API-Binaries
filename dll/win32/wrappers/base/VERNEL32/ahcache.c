/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    ahcache.c

Abstract:

    Cache handling functions for use in kernel32.dll

Author:

    Skulltrail 19-March-2017

Revision History:

--*/

#include <main.h>

VOID
WINAPI
BasepFreeAppCompatData(
    PVOID  pAppCompatData,
    SIZE_T cbAppCompatData,
    PVOID  pSxsData,
    SIZE_T cbSxsData
    )
{
	/*Verify if we have pAppCompatData parameter so clean it*/
    if (pAppCompatData) {
        NtFreeVirtualMemory(NtCurrentProcess(),
                            &pAppCompatData,
                            &cbAppCompatData,
                            MEM_RELEASE);
    }

	/*Verify if we have pSxsData parameter and so clean it*/
    if (pSxsData) {
        NtFreeVirtualMemory(NtCurrentProcess(),
                            &pSxsData,
                            &cbSxsData,
                            MEM_RELEASE);

    }
}