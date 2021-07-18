/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    sec.c

Abstract:

    Utils functions

Author:

    Skulltrail 26-March-2021

Revision History:

--*/

#include "main.h"

/*
* @implemented
*/
DWORD
WINAPI
BaseSetLastNTError(IN NTSTATUS Status)
{
    DWORD dwErrCode;

    /* Convert from NT to Win32, then set */
    dwErrCode = RtlNtStatusToDosError(Status);
    SetLastError(dwErrCode);
    return dwErrCode;
}