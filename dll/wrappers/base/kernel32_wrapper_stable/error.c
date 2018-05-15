/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    error.c

Abstract:

    This module contains the Win32 error APIs.

Author:

    Skulltrail 19-March-2017

Revision History:

--*/

#include "main.h"

ULONG
WINAPI
BaseSetLastNTError(
    IN NTSTATUS Status
    )

{
    ULONG dwErrorCode;

    dwErrorCode = RtlNtStatusToDosError( Status );
    SetLastError( dwErrorCode );
    return( dwErrorCode );
}

UINT
WINAPI
GetErrorMode()
{

    UINT PreviousMode;
    NTSTATUS Status;

    Status = NtQueryInformationProcess(
                NtCurrentProcess(),
                ProcessDefaultHardErrorMode,
                (PVOID) &PreviousMode,
                sizeof(PreviousMode),
                NULL
                );
    if ( !NT_SUCCESS(Status) ) {
        BaseSetLastNTError(Status);
        return 0;
        }

    if (PreviousMode & 1) {
        PreviousMode &= ~SEM_FAILCRITICALERRORS;
        }
    else {
        PreviousMode |= SEM_FAILCRITICALERRORS;
        }
    return PreviousMode;
}