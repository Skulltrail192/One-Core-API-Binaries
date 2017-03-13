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
 
#define NDEBUG

#include "main.h"

BOOLEAN BaseRunningInServerProcess = FALSE;

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

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
