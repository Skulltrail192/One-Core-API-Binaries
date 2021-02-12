/*
 * Performance Data Helper (pdh.dll)
 *
 * Copyright 2007 Andrey Turkin
 * Copyright 2007 Hans Leidekker
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
 */

#define WIN32_NO_STATUS

#include <stdarg.h>
#include <math.h>

#define NONAMELESSUNION
#define NONAMELESSSTRUCT
#include <windef.h>
#include <winbase.h>

#include <pdh.h>
#include <pdhmsg.h>
//#include "winperf.h"

#include <wine/debug.h>
#include <wine/list.h>
#include <wine/unicode.h>

WINE_DEFAULT_DEBUG_CHANNEL(pdh);

#define PDH_MAGIC_QUERY     0x50444830 /* 'PDH0' */

static CRITICAL_SECTION pdh_handle_cs;

struct query
{
    DWORD       magic;      /* signature */
    DWORD_PTR   user;       /* user data */
    HANDLE      thread;     /* collect thread */
    DWORD       interval;   /* collect interval */
    HANDLE      wait;       /* wait event */
    HANDLE      stop;       /* stop event */
    struct list counters;   /* counter list */
};

union value
{
    LONG     longvalue;
    double   doublevalue;
    LONGLONG largevalue;
};

struct counter
{
    DWORD           magic;                          /* signature */
    struct list     entry;                          /* list entry */
    WCHAR          *path;                           /* identifier */
    DWORD           type;                           /* counter type */
    DWORD           status;                         /* update status */
    LONG            scale;                          /* scale factor */
    LONG            defaultscale;                   /* default scale factor */
    DWORD_PTR       user;                           /* user data */
    DWORD_PTR       queryuser;                      /* query user data */
    LONGLONG        base;                           /* samples per second */
    FILETIME        stamp;                          /* time stamp */
    void (CALLBACK *collect)( struct counter * );   /* collect callback */
    union value     one;                            /* first value */
    union value     two;                            /* second value */
};

/* caller must hold query lock */
static void collect_query_data( struct query *query )
{
    struct list *item;

    LIST_FOR_EACH( item, &query->counters )
    {
        SYSTEMTIME time;
        struct counter *counter = LIST_ENTRY( item, struct counter, entry );

        counter->collect( counter );

        GetLocalTime( &time );
        SystemTimeToFileTime( &time, &counter->stamp );
    }
}


/***********************************************************************
 *              PdhAddEnglishCounterA   (PDH.@)
 */
PDH_STATUS 
WINAPI 
PdhAddEnglishCounterA( 
	PDH_HQUERY query, 
	LPCSTR path,
    DWORD_PTR userdata, 
	PDH_HCOUNTER *counter 
)
{
    TRACE("%p %s %lx %p\n", query, debugstr_a(path), userdata, counter);

    if (!query) return PDH_INVALID_ARGUMENT;
    return PdhAddCounterA( query, path, userdata, counter );
}

/***********************************************************************
 *              PdhAddEnglishCounterW   (PDH.@)
 */
PDH_STATUS WINAPI 
PdhAddEnglishCounterW( 
	PDH_HQUERY query, 
	LPCWSTR path,
    DWORD_PTR userdata, 
	PDH_HCOUNTER *counter 
)
{
    TRACE("%p %s %lx %p\n", query, debugstr_w(path), userdata, counter);

    if (!query) return PDH_INVALID_ARGUMENT;
    return PdhAddCounterW( query, path, userdata, counter );
}

/***********************************************************************
 *              PdhCollectQueryDataWithTime   (PDH.@)
 */
PDH_STATUS WINAPI 
PdhCollectQueryDataWithTime( 
	PDH_HQUERY handle, 
	LONGLONG *timestamp 
)
{
    struct query *query = handle;
    struct counter *counter;
    struct list *item;

    TRACE("%p %p\n", handle, timestamp);

    if (!timestamp) return PDH_INVALID_ARGUMENT;

    EnterCriticalSection( &pdh_handle_cs );
    if (!query || query->magic != PDH_MAGIC_QUERY)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }
    if (list_empty( &query->counters ))
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_NO_DATA;
    }

    collect_query_data( query );

    item = list_head( &query->counters );
    counter = LIST_ENTRY( item, struct counter, entry );

    *timestamp = ((LONGLONG)counter->stamp.dwHighDateTime << 32) | counter->stamp.dwLowDateTime;

    LeaveCriticalSection( &pdh_handle_cs );
    return ERROR_SUCCESS;
}

/***********************************************************************
 *              PdhValidatePathExA   (PDH.@)
 */
PDH_STATUS WINAPI PdhValidatePathExA( PDH_HLOG source, LPCSTR path )
{
    TRACE("%p %s\n", source, debugstr_a(path));

    if (source)
    {
        FIXME("log file data source not supported\n");
        return ERROR_SUCCESS;
    }
    return PdhValidatePathA( path );
}

/***********************************************************************
 *              PdhValidatePathExW   (PDH.@)
 */
PDH_STATUS WINAPI PdhValidatePathExW( PDH_HLOG source, LPCWSTR path )
{
    TRACE("%p %s\n", source, debugstr_w(path));

    if (source)
    {
        FIXME("log file data source not supported\n");
        return ERROR_SUCCESS;
    }
    return PdhValidatePathW( path );
}