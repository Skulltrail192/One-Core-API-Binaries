/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    service.c

Abstract:

    Windows Services functions

Author:

    Skulltrail 07-March-2021

Revision History:

--*/

#include "main.h"

/******************************************************************************
 *     NotifyServiceStatusChangeW   (sechost.@)
 */
DWORD WINAPI DECLSPEC_HOTPATCH NotifyServiceStatusChangeW( SC_HANDLE service, DWORD mask,
                                                           SERVICE_NOTIFYW *notify_buffer )
{
    // DWORD err;
    // BOOL b_dummy = FALSE;
    // GUID g_dummy = {0};
    // struct notify_data *data;

    // TRACE( "%p 0x%x %p\n", service, mask, notify_buffer );

    // if (!(data = heap_alloc_zero( sizeof(*data) )))
        // return ERROR_NOT_ENOUGH_MEMORY;

    // data->service = service;
    // data->notify_buffer = notify_buffer;
    // if (!DuplicateHandle( GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(),
                          // &data->calling_thread, 0, FALSE, DUPLICATE_SAME_ACCESS ))
    // {
        // ERR("DuplicateHandle failed: %u\n", GetLastError());
        // heap_free( data );
        // return ERROR_NOT_ENOUGH_MEMORY;
    // }

    // data->params.dwInfoLevel = 2;
    // data->params.u.params = &data->cparams;

    // data->cparams.dwNotifyMask = mask;

    // EnterCriticalSection( &service_cs );

    // __TRY
    // {
        // err = svcctl_NotifyServiceStatusChange( service, data->params, &g_dummy,
                                                // &g_dummy, &b_dummy, &data->notify_handle );
    // }
    // __EXCEPT(rpc_filter)
    // {
        // err = map_exception_code( GetExceptionCode() );
    // }
    // __ENDTRY

    // if (err != ERROR_SUCCESS)
    // {
        // WARN("NotifyServiceStatusChange server call failed: %u\n", err);
        // LeaveCriticalSection( &service_cs );
        // CloseHandle( data->calling_thread );
        // CloseHandle( data->ready_evt );
        // heap_free( data );
        // return err;
    // }

    // CloseHandle( CreateThread( NULL, 0, &notify_thread, data, 0, NULL ) );

    // list_add_tail( &notify_list, &data->entry );

    // LeaveCriticalSection( &service_cs );

    return ERROR_SUCCESS;
}