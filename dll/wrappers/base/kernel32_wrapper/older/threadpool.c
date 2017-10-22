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

#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

VOID 
WINAPI 
CloseThreadpoolIo(
  _Inout_  PTP_IO pio
)
{
    DbgPrint("Function: CloseThreadpoolIo. Result from: TpAllocCleanupGroup. Status: %08x\n");  			
	;
}

PTP_IO 
WINAPI 
CreateThreadpoolIo(
  _In_        HANDLE                fl,
  _In_        PTP_WIN32_IO_CALLBACK pfnio,
  _Inout_opt_ PVOID                 pv,
  _In_opt_    PTP_CALLBACK_ENVIRON  pcbe
)
{
	return NULL;
}

/***********************************************************************
 *              CreateThreadpoolCleanupGroup (KERNEL32.@)
 */
PTP_CLEANUP_GROUP WINAPI 
CreateThreadpoolCleanupGroup( void )
{
    TP_CLEANUP_GROUP *group;
    NTSTATUS status;

    TRACE( "\n" );

    status = TpAllocCleanupGroup( &group );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return NULL;
    }

    return group;
}

/***********************************************************************
 *              CreateThreadpool (KERNEL32.@)
 */
PTP_POOL 
WINAPI 
CreateThreadpool( 
	PVOID reserved 
)
{
    TP_POOL *pool;
    NTSTATUS status;

    TRACE( "%p\n", reserved );

    status = TpAllocPool( &pool, reserved );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return NULL;
    }

    return pool;
}

/***********************************************************************
 *              CreateThreadpoolTimer (KERNEL32.@)
 */
PTP_TIMER 
WINAPI 
CreateThreadpoolTimer( 
	PTP_TIMER_CALLBACK callback, 
	PVOID userdata,
	TP_CALLBACK_ENVIRON *environment
)
{
    TP_TIMER *timer;
    NTSTATUS status;

    TRACE( "%p, %p, %p\n", callback, userdata, environment );

    status = TpAllocTimer( &timer, callback, userdata, environment );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return NULL;
    }

    return timer;
}

/***********************************************************************
 *              CreateThreadpoolWait (KERNEL32.@)
 */
PTP_WAIT 
WINAPI 
CreateThreadpoolWait( 
	PTP_WAIT_CALLBACK callback, 
	PVOID userdata,
	TP_CALLBACK_ENVIRON *environment 
)
{
    TP_WAIT *wait;
    NTSTATUS status;

    TRACE( "%p, %p, %p\n", callback, userdata, environment );

    status = TpAllocWait( &wait, callback, userdata, environment );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return NULL;
    }

    return wait;
}

/***********************************************************************
 *              CreateThreadpoolWork (KERNEL32.@)
 */
PTP_WORK 
WINAPI 
CreateThreadpoolWork( 
	PTP_WORK_CALLBACK callback, 
	PVOID userdata,
	TP_CALLBACK_ENVIRON *environment 
)
{
    TP_WORK *work;
    NTSTATUS status;

    TRACE( "%p, %p, %p\n", callback, userdata, environment );

    status = TpAllocWork( &work, callback, userdata, environment );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return NULL;
    }

    return work;
}

/***********************************************************************
 *              TrySubmitThreadpoolCallback (KERNEL32.@)
 */
BOOL 
WINAPI 
TrySubmitThreadpoolCallback( 
	PTP_SIMPLE_CALLBACK callback, 
	PVOID userdata,
    TP_CALLBACK_ENVIRON *environment 
)
{
    NTSTATUS status;

    TRACE( "%p, %p, %p\n", callback, userdata, environment );

    status = TpSimpleTryPost( callback, userdata, environment );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }

    return TRUE;
}

/***********************************************************************
 *              CreateThreadpoolCleanupGroup (KERNEL32.@)
 */
PTP_CLEANUP_GROUP WINAPI 
CreateThreadpoolCleanupGroup( void )
{
    TP_CLEANUP_GROUP *group;
    NTSTATUS status;

    TRACE( "\n" );

    status = TpAllocCleanupGroup( &group );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return NULL;
    }

    return group;
}