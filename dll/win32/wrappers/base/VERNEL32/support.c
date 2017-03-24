/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    support.c

Abstract:

    This module implements various conversion routines
    that transform Win32 parameters into NT parameters.

Author:

    Skulltrail 18-March-2017

Revision History:

--*/
 
#define NDEBUG

#include "main.h"

BOOLEAN BaseRunningInServerProcess = FALSE;

PRTL_CONVERT_STRING Basep8BitStringToUnicodeString = RtlAnsiStringToUnicodeString;

VOID
WINAPI
BaseProcessStart(
	PPROCESS_START_ROUTINE lpStartAddress
)
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
    _SEH2_EXCEPT(UnhandledExceptionFilter(GetExceptionInformation()))
    {
        /* Get the Exit code from the SEH Handler */
        if (!BaseRunningInServerProcess)
        {
            /* Kill the whole process, usually */
            ExitProcess(GetExceptionCode());
        }
        else
        {
            /* If running inside CSRSS, kill just this thread */
            ExitThread(GetExceptionCode());
        }
    }
    _SEH2_END;
}

VOID
WINAPI
BaseThreadStart(
    IN LPTHREAD_START_ROUTINE lpStartAddress,
    IN LPVOID lpParameter
)

/*++

Routine Description:

    This function is called to start a Win32 thread. Its purpose
    is to call the thread, and if the thread returns, to terminate
    the thread and delete it's stack.

Arguments:

    lpStartAddress - Supplies the starting address of the new thread.  The
        address is logically a procedure that never returns and that
        accepts a single 32-bit pointer argument.

    lpParameter - Supplies a single parameter value passed to the thread.

Return Value:

    None.

--*/

{
    _SEH2_TRY {

        //
        // test for fiber start or new thread
        //

        //
        // WARNING WARNING DO NOT CHANGE INIT OF NtTib.Version. There is
        // external code depending on this initialization !
        //
		CsrNewThread();
        if ( NtCurrentTeb()->NtTib.Version == 7680 ) {
            if ( !BaseRunningInServerProcess ) {
                CsrNewThread();
                }
        }
        ExitThread((lpStartAddress)(lpParameter));
        }
    _SEH2_EXCEPT(UnhandledExceptionFilter( GetExceptionInformation() )) {
        if ( !BaseRunningInServerProcess ) {
            ExitProcess(GetExceptionCode());
            }
        else {
            ExitThread(GetExceptionCode());
            }
        }
	_SEH2_END;
}

/*
 * Converts an ANSI or OEM String to the TEB StaticUnicodeString
 */
PUNICODE_STRING
WINAPI
Basep8BitStringToStaticUnicodeString(
	IN LPCSTR String
)
{
    PUNICODE_STRING StaticString = &(NtCurrentTeb()->StaticUnicodeString);
    ANSI_STRING AnsiString;
    NTSTATUS Status;

    /* Initialize an ANSI String */
    Status = RtlInitAnsiStringEx(&AnsiString, String);
    if (!NT_SUCCESS(Status))
    {
        Status = STATUS_BUFFER_OVERFLOW;
    }
    else
    {
        /* Convert it */
        Status = Basep8BitStringToUnicodeString(StaticString, &AnsiString, FALSE);
    }

    if (NT_SUCCESS(Status)) return StaticString;

    if (Status == STATUS_BUFFER_OVERFLOW)
    {
        SetLastError(ERROR_FILENAME_EXCED_RANGE);
    }
    else
    {
        BaseSetLastNTError(Status);
    }

    return NULL;
}