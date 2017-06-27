/*++

Copyright (c) 2017  Shorthorn Project

Module Name:

    support.c

Abstract:

    This module implements various conversion routines
    that transform Win32 parameters into NT parameters.

Author:

    Skulltrail 7-March-2017

Revision History:

--*/

#include <main.h>

BOOLEAN BaseRunningInServerProcess = FALSE;

VOID
WINAPI
BaseThreadStart(
    IN LPTHREAD_START_ROUTINE lpStartAddress,
    IN LPVOID lpParameter
    )
{
    /* Attempt to call the Thread Start Address */
    _SEH2_TRY
    {
        /* Legacy check which is still used today for Win32 threads */
        if (NtCurrentTeb()->NtTib.Version == (30 << 8)) // OS/2 V3.0 ("Cruiser")
        {
            /* This registers the termination port with CSRSS */
            if (!BaseRunningInServerProcess) CsrNewThread();
        }

        /* Get the exit code from the Thread Start */
        ExitThread((lpStartAddress)((PVOID)lpParameter));
    }
    _SEH2_EXCEPT((UnhandledExceptionFilter( GetExceptionInformation() )) )
    {
        /* Get the Exit code from the SEH Handler */
        if (!BaseRunningInServerProcess)
        {
            /* Kill the whole process, usually */
            ExitProcess(_SEH2_GetExceptionCode());
        }
        else
        {
            /* If running inside CSRSS, kill just this thread */
            ExitThread(_SEH2_GetExceptionCode());
        }
    }
    _SEH2_END;
}

VOID
WINAPI
BaseProcessStart(PPROCESS_START_ROUTINE lpStartAddress)
{
    _SEH2_TRY
    {
        /* Set our Start Address */
        NtSetInformationThread(NtCurrentThread(),
                               ThreadQuerySetWin32StartAddress,
                               &lpStartAddress,
                               sizeof(PPROCESS_START_ROUTINE));

        /* Call the Start Routine */
        ExitThread(lpStartAddress());
    }
    _SEH2_EXCEPT(UnhandledExceptionFilter(_SEH2_GetExceptionInformation()))
    {
        /* Get the Exit code from the SEH Handler */
        if (!BaseRunningInServerProcess)
        {
            /* Kill the whole process, usually */
            ExitProcess(_SEH2_GetExceptionCode());
        }
        else
        {
            /* If running inside CSRSS, kill just this thread */
            ExitThread(_SEH2_GetExceptionCode());
        }
    }
    _SEH2_END;
}