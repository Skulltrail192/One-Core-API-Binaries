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

WINE_DEFAULT_DEBUG_CHANNEL(error); 

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


/***********************************************************************
 *	GetOverlappedResultEx   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH GetOverlappedResultEx( HANDLE file, OVERLAPPED *overlapped,
                                                     DWORD *result, DWORD timeout, BOOL alertable )
{
    NTSTATUS status;
    DWORD ret;

    TRACE( "(%p %p %p %u %d)\n", file, overlapped, result, timeout, alertable );

    status = overlapped->Internal;
    if (status == STATUS_PENDING)
    {
        if (!timeout)
        {
            SetLastError( ERROR_IO_INCOMPLETE );
            return FALSE;
        }
        ret = WaitForSingleObjectEx( overlapped->hEvent ? overlapped->hEvent : file, timeout, alertable );
        if (ret == WAIT_FAILED)
            return FALSE;
        else if (ret)
        {
            SetLastError( ret );
            return FALSE;
        }

        status = overlapped->Internal;
        if (status == STATUS_PENDING) status = STATUS_SUCCESS;
    }

    *result = overlapped->InternalHigh;
	
	if(!NT_SUCCESS(status))
	{
		BaseSetLastNTError(status);
        return FALSE;
	}else{
		return TRUE;
	}
}

/******************************************************************************
 *              GetQueuedCompletionStatusEx   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH GetQueuedCompletionStatusEx( HANDLE port, LPOVERLAPPED_ENTRY entries,
                                                           ULONG count, ULONG *written,
                                                           DWORD timeout, BOOL alertable )
{
    LARGE_INTEGER time;
    NTSTATUS ret;

    TRACE( "%p %p %u %p %u %u\n", port, entries, count, written, timeout, alertable );

    ret = NtRemoveIoCompletionEx( port, (FILE_IO_COMPLETION_INFORMATION *)entries, count,
                                  written, get_nt_timeout( &time, timeout ), alertable );
    if (ret == STATUS_SUCCESS) return TRUE;
    else if (ret == STATUS_TIMEOUT) SetLastError( WAIT_TIMEOUT );
    else if (ret == STATUS_USER_APC) SetLastError( WAIT_IO_COMPLETION );
    else SetLastError( RtlNtStatusToDosError(ret) );
    return FALSE;
}