/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    filehops.c

Abstract:

    This module implements Win32 file handle APIs

Author:

    Skulltrail 18-March-2017

Revision History:

--*/

#include "main.h"

/***********************************************************************
 *           Wow64EnableWow64FsRedirection   (KERNEL32.@)
 */
BOOLEAN 
WINAPI 
Wow64EnableWow64FsRedirection( 
	BOOLEAN enable 
)
{
    NTSTATUS status = RtlWow64EnableFsRedirection( enable );
    if (status) SetLastError( RtlNtStatusToDosError(status) );
    return !status;
}


/***********************************************************************
 *           Wow64DisableWow64FsRedirection   (KERNEL32.@)
 */
BOOL
WINAPI 
Wow64DisableWow64FsRedirection( 
	PVOID *old_value 
)
{
    NTSTATUS status = RtlWow64EnableFsRedirectionEx( TRUE, (ULONG *)old_value );
    if (status) SetLastError( RtlNtStatusToDosError(status) );
    return !status;
}


/***********************************************************************
 *           Wow64RevertWow64FsRedirection   (KERNEL32.@)
 */
BOOL 
WINAPI 
Wow64RevertWow64FsRedirection( 
	PVOID old_value 
)
{
    NTSTATUS status = RtlWow64EnableFsRedirection( !old_value );
    if (status) SetLastError( RtlNtStatusToDosError(status) );
    return !status;
}