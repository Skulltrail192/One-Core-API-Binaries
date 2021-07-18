/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    trace.c

Abstract:

    Tracing functions

Author:

    Skulltrail 07-March-2021

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(advapi32);

/******************************************************************************
 *     EnableTraceEx2   (sechost.@)
 */
ULONG 
WINAPI 
EnableTraceEx2( 
	TRACEHANDLE handle, 
	const GUID *provider, 
	ULONG control, 
	UCHAR level,
    ULONGLONG match_any, 
	ULONGLONG match_all, 
	ULONG timeout,
    ENABLE_TRACE_PARAMETERS *params 
)
{
    FIXME("(%s, %s, %u, %u, %s, %s, %u, %p): stub\n", wine_dbgstr_longlong(handle),
          debugstr_guid(provider), control, level, wine_dbgstr_longlong(match_any),
          wine_dbgstr_longlong(match_all), timeout, params);

    return ERROR_SUCCESS;
}