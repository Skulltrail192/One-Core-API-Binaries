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

/*++

Routine Description:

    This API sets the "last error value" and the "last error string"
    based on the value of Status. For status codes that don't have
    a corresponding error string, the string is set to null.

Arguments:

    Status - Supplies the status value to store as the last error value.

Return Value:

    The corresponding Win32 error code that was stored in the
    "last error value" thread variable.

--*/

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