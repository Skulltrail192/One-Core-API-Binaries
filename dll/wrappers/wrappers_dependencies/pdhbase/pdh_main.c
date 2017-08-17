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

typedef struct _PDH_RAW_LOG_RECORD {
    DWORD       dwStructureSize;
    DWORD       dwRecordType;
    DWORD       dwItems;
    UCHAR       RawBytes[1];
} PDH_RAW_LOG_RECORD, * PPDH_RAW_LOG_RECORD;

typedef struct _PDH_TIME_INFO {
    LONGLONG    StartTime;
    LONGLONG    EndTime;
    DWORD       SampleCount;
} PDH_TIME_INFO, * PPDH_TIME_INFO;

typedef struct _PDH_FMT_COUNTERVALUE_ITEM_A {
    LPSTR                   szName;
    PDH_FMT_COUNTERVALUE    FmtValue;
} PDH_FMT_COUNTERVALUE_ITEM_A, * PPDH_FMT_COUNTERVALUE_ITEM_A;

typedef struct _PDH_FMT_COUNTERVALUE_ITEM_W {
    LPWSTR                  szName;
    PDH_FMT_COUNTERVALUE    FmtValue;
} PDH_FMT_COUNTERVALUE_ITEM_W, * PPDH_FMT_COUNTERVALUE_ITEM_W;

typedef struct _PDH_RAW_COUNTER_ITEM_A {
    LPSTR           szName;
    PDH_RAW_COUNTER RawValue;
} PDH_RAW_COUNTER_ITEM_A, * PPDH_RAW_COUNTER_ITEM_A;

typedef struct _PDH_RAW_COUNTER_ITEM_W {
    LPWSTR          szName;
    PDH_RAW_COUNTER RawValue;
} PDH_RAW_COUNTER_ITEM_W, * PPDH_RAW_COUNTER_ITEM_W;

typedef struct _PDH_STATISTICS {
    DWORD                   dwFormat;
    DWORD                   count;
    PDH_FMT_COUNTERVALUE    min;
    PDH_FMT_COUNTERVALUE    max;
    PDH_FMT_COUNTERVALUE    mean;
} PDH_STATISTICS, * PPDH_STATISTICS;

typedef PDH_STATUS (__stdcall *CounterPathCallBack)(DWORD);

typedef struct _BrowseDlgConfig_W {
    // Configuration flags
    DWORD   bIncludeInstanceIndex:1,
            bSingleCounterPerAdd:1,
            bSingleCounterPerDialog:1,
            bLocalCountersOnly:1,
            bWildCardInstances:1,
            bHideDetailBox:1,
            bInitializePath:1,
            bDisableMachineSelection:1,
            bIncludeCostlyObjects:1,
            bReserved:23;

    HWND                hWndOwner;
    LPWSTR              szDataSource;
    LPWSTR              szReturnPathBuffer;
    DWORD               cchReturnPathLength;
    CounterPathCallBack pCallBack;
    DWORD               dwCallBackArg;
    PDH_STATUS          CallBackStatus;
    DWORD               dwDefaultDetailLevel;
    LPWSTR              szDialogBoxCaption;
} PDH_BROWSE_DLG_CONFIG_W, *PPDH_BROWSE_DLG_CONFIG_W;

typedef struct _BrowseDlgConfig_A {
    // Configuration flags
    DWORD   bIncludeInstanceIndex:1,
            bSingleCounterPerAdd:1,
            bSingleCounterPerDialog:1,
            bLocalCountersOnly:1,
            bWildCardInstances:1,
            bHideDetailBox:1,
            bInitializePath:1,
            bDisableMachineSelection:1,
            bIncludeCostlyObjects:1,
            bReserved:23;

    HWND                hWndOwner;
    LPSTR               szDataSource;
    LPSTR               szReturnPathBuffer;
    DWORD               cchReturnPathLength;
    CounterPathCallBack pCallBack;
    DWORD               dwCallBackArg;
    PDH_STATUS          CallBackStatus;
    DWORD               dwDefaultDetailLevel;
    LPSTR               szDialogBoxCaption;
} PDH_BROWSE_DLG_CONFIG_A, *PPDH_BROWSE_DLG_CONFIG_A;

typedef PDH_STATUS (__stdcall * CounterPathCallBack)(DWORD_PTR);

typedef struct _BrowseDlgConfig_HW {
    // Configuration flags
    DWORD   bIncludeInstanceIndex:1,
            bSingleCounterPerAdd:1,
            bSingleCounterPerDialog:1,
            bLocalCountersOnly:1,
            bWildCardInstances:1,
            bHideDetailBox:1,
            bInitializePath:1,
            bDisableMachineSelection:1,
            bIncludeCostlyObjects:1,
            bShowObjectBrowser:1,
            bReserved:22;

    HWND                hWndOwner;
    PDH_HLOG            hDataSource;
    LPWSTR              szReturnPathBuffer;
    DWORD               cchReturnPathLength;
    CounterPathCallBack pCallBack;
    DWORD_PTR           dwCallBackArg;
    PDH_STATUS          CallBackStatus;
    DWORD               dwDefaultDetailLevel;
    LPWSTR              szDialogBoxCaption;
} PDH_BROWSE_DLG_CONFIG_HW, * PPDH_BROWSE_DLG_CONFIG_HW;

typedef struct _BrowseDlgConfig_HA {
    // Configuration flags
    DWORD   bIncludeInstanceIndex:1,
            bSingleCounterPerAdd:1,
            bSingleCounterPerDialog:1,
            bLocalCountersOnly:1,
            bWildCardInstances:1,
            bHideDetailBox:1,
            bInitializePath:1,
            bDisableMachineSelection:1,
            bIncludeCostlyObjects:1,
            bShowObjectBrowser:1,
            bReserved:22;

    HWND                hWndOwner;
    PDH_HLOG            hDataSource;
    LPSTR               szReturnPathBuffer;
    DWORD               cchReturnPathLength;
    CounterPathCallBack pCallBack;
    DWORD_PTR           dwCallBackArg;
    PDH_STATUS          CallBackStatus;
    DWORD               dwDefaultDetailLevel;
    LPSTR               szDialogBoxCaption;
} PDH_BROWSE_DLG_CONFIG_HA, * PPDH_BROWSE_DLG_CONFIG_HA;

static CRITICAL_SECTION pdh_handle_cs;
static CRITICAL_SECTION_DEBUG pdh_handle_cs_debug =
{
    0, 0, &pdh_handle_cs,
    { &pdh_handle_cs_debug.ProcessLocksList,
      &pdh_handle_cs_debug.ProcessLocksList },
      0, 0, { (DWORD_PTR)(__FILE__ ": pdh_handle_cs") }
};
static CRITICAL_SECTION pdh_handle_cs = { &pdh_handle_cs_debug, -1, 0, 0, 0, 0 };

static inline void *heap_alloc( SIZE_T size )
{
    return HeapAlloc( GetProcessHeap(), 0, size );
}

static inline void *heap_alloc_zero( SIZE_T size )
{
    return HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, size );
}

static inline void heap_free( LPVOID mem )
{
    HeapFree( GetProcessHeap(), 0, mem );
}

static inline WCHAR *pdh_strdup( const WCHAR *src )
{
    WCHAR *dst;

    if (!src) return NULL;
    if ((dst = heap_alloc( (strlenW( src ) + 1) * sizeof(WCHAR) ))) strcpyW( dst, src );
    return dst;
}

static inline WCHAR *pdh_strdup_aw( const char *src )
{
    int len;
    WCHAR *dst;

    if (!src) return NULL;
    len = MultiByteToWideChar( CP_ACP, 0, src, -1, NULL, 0 );
    if ((dst = heap_alloc( len * sizeof(WCHAR) ))) MultiByteToWideChar( CP_ACP, 0, src, -1, dst, len );
    return dst;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    TRACE("(0x%p, %d, %p)\n",hinstDLL,fdwReason,lpvReserved);
    switch (fdwReason)
    {
    case DLL_WINE_PREATTACH:
        return FALSE;    /* prefer native version */
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDLL);
        break;
    case DLL_PROCESS_DETACH:
        if (lpvReserved) break;
        DeleteCriticalSection(&pdh_handle_cs);
        break;
    }

    return TRUE;
}

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

#define PDH_MAGIC_COUNTER   0x50444831 /* 'PDH1' */

static struct counter *create_counter( void )
{
    struct counter *counter;

    if ((counter = heap_alloc_zero( sizeof(struct counter) )))
    {
        counter->magic = PDH_MAGIC_COUNTER;
        return counter;
    }
    return NULL;
}

static void destroy_counter( struct counter *counter )
{
    counter->magic = 0;
    heap_free( counter->path );
    heap_free( counter );
}

#define PDH_MAGIC_QUERY     0x50444830 /* 'PDH0' */

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

static struct query *create_query( void )
{
    struct query *query;

    if ((query = heap_alloc_zero( sizeof(struct query) )))
    {
        query->magic = PDH_MAGIC_QUERY;
        list_init( &query->counters );
        return query;
    }
    return NULL;
}

static void destroy_query( struct query *query )
{
    query->magic = 0;
    heap_free( query );
}

struct source
{
    DWORD           index;                          /* name index */
    const WCHAR    *path;                           /* identifier */
    void (CALLBACK *collect)( struct counter * );   /* collect callback */
    DWORD           type;                           /* counter type */
    LONG            scale;                          /* default scale factor */
    LONGLONG        base;                           /* samples per second */
};

static const WCHAR path_processor_time[] =
    {'\\','P','r','o','c','e','s','s','o','r','(','_','T','o','t','a','l',')',
     '\\','%',' ','P','r','o','c','e','s','s','o','r',' ','T','i','m','e',0};
static const WCHAR path_uptime[] =
    {'\\','S','y','s','t','e','m', '\\', 'S','y','s','t','e','m',' ','U','p',' ','T','i','m','e',0};

static void CALLBACK collect_processor_time( struct counter *counter )
{
    counter->two.largevalue = 500000; /* FIXME */
    counter->status = PDH_CSTATUS_VALID_DATA;
}

static void CALLBACK collect_uptime( struct counter *counter )
{
    counter->two.largevalue = GetTickCount64();
    counter->status = PDH_CSTATUS_VALID_DATA;
}

#define TYPE_PROCESSOR_TIME \
    (PERF_SIZE_LARGE | PERF_TYPE_COUNTER | PERF_COUNTER_RATE | PERF_TIMER_100NS | PERF_DELTA_COUNTER | \
     PERF_INVERSE_COUNTER | PERF_DISPLAY_PERCENT)

#define TYPE_UPTIME \
    (PERF_SIZE_LARGE | PERF_TYPE_COUNTER | PERF_COUNTER_ELAPSED | PERF_OBJECT_TIMER | PERF_DISPLAY_SECONDS)

/* counter source registry */
static const struct source counter_sources[] =
{
    { 6,    path_processor_time,    collect_processor_time,     TYPE_PROCESSOR_TIME,    -5,     10000000 },
    { 674,  path_uptime,            collect_uptime,             TYPE_UPTIME,            -3,     1000 }
};

static BOOL is_local_machine( const WCHAR *name, DWORD len )
{
    WCHAR buf[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD buflen = sizeof(buf) / sizeof(buf[0]);

    if (!GetComputerNameW( buf, &buflen )) return FALSE;
    return len == buflen && !memicmpW( name, buf, buflen );
}

static BOOL pdh_match_path( LPCWSTR fullpath, LPCWSTR path )
{
    const WCHAR *p;

    if (path[0] == '\\' && path[1] == '\\' && (p = strchrW( path + 2, '\\' )) &&
        is_local_machine( path + 2, p - path - 2 ))
    {
        path += p - path;
    }
    if (strchrW( path, '\\' )) p = fullpath;
    else p = strrchrW( fullpath, '\\' ) + 1;
    return !strcmpW( p, path );
}

/***********************************************************************
 *              PdhAddCounterA   (PDH.@)
 */
PDH_STATUS WINAPI PdhAddCounterA( PDH_HQUERY query, LPCSTR path,
                                  DWORD_PTR userdata, PDH_HCOUNTER *counter )
{
    PDH_STATUS ret;
    WCHAR *pathW;

    TRACE("%p %s %lx %p\n", query, debugstr_a(path), userdata, counter);

    if (!path) return PDH_INVALID_ARGUMENT;

    if (!(pathW = pdh_strdup_aw( path )))
        return PDH_MEMORY_ALLOCATION_FAILURE;

    ret = PdhAddCounterW( query, pathW, userdata, counter );

    heap_free( pathW );
    return ret;
}

/***********************************************************************
 *              PdhAddCounterW   (PDH.@)
 */
PDH_STATUS WINAPI PdhAddCounterW( PDH_HQUERY hquery, LPCWSTR path,
                                  DWORD_PTR userdata, PDH_HCOUNTER *hcounter )
{
    struct query *query = hquery;
    struct counter *counter;
    unsigned int i;

    TRACE("%p %s %lx %p\n", hquery, debugstr_w(path), userdata, hcounter);

    if (!path  || !hcounter) return PDH_INVALID_ARGUMENT;

    EnterCriticalSection( &pdh_handle_cs );
    if (!query || query->magic != PDH_MAGIC_QUERY)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }

    *hcounter = NULL;
    for (i = 0; i < sizeof(counter_sources) / sizeof(counter_sources[0]); i++)
    {
        if (pdh_match_path( counter_sources[i].path, path ))
        {
            if ((counter = create_counter()))
            {
                counter->path         = pdh_strdup( counter_sources[i].path );
                counter->collect      = counter_sources[i].collect;
                counter->type         = counter_sources[i].type;
                counter->defaultscale = counter_sources[i].scale;
                counter->base         = counter_sources[i].base;
                counter->queryuser    = query->user;
                counter->user         = userdata;

                list_add_tail( &query->counters, &counter->entry );
                *hcounter = counter;

                LeaveCriticalSection( &pdh_handle_cs );
                return ERROR_SUCCESS;
            }
            LeaveCriticalSection( &pdh_handle_cs );
            return PDH_MEMORY_ALLOCATION_FAILURE;
        }
    }
    LeaveCriticalSection( &pdh_handle_cs );
    return PDH_CSTATUS_NO_COUNTER;
}

/***********************************************************************
 *              PdhAddEnglishCounterA   (PDH.@)
 */
PDH_STATUS WINAPI PdhAddEnglishCounterA( PDH_HQUERY query, LPCSTR path,
                                         DWORD_PTR userdata, PDH_HCOUNTER *counter )
{
    TRACE("%p %s %lx %p\n", query, debugstr_a(path), userdata, counter);

    if (!query) return PDH_INVALID_ARGUMENT;
    return PdhAddCounterA( query, path, userdata, counter );
}

/***********************************************************************
 *              PdhAddEnglishCounterW   (PDH.@)
 */
PDH_STATUS WINAPI PdhAddEnglishCounterW( PDH_HQUERY query, LPCWSTR path,
                                         DWORD_PTR userdata, PDH_HCOUNTER *counter )
{
    TRACE("%p %s %lx %p\n", query, debugstr_w(path), userdata, counter);

    if (!query) return PDH_INVALID_ARGUMENT;
    return PdhAddCounterW( query, path, userdata, counter );
}

/* caller must hold counter lock */
static PDH_STATUS format_value( struct counter *counter, DWORD format, union value *raw1,
                                union value *raw2, PDH_FMT_COUNTERVALUE *value )
{
    LONG factor;

    factor = counter->scale ? counter->scale : counter->defaultscale;
    if (format & PDH_FMT_LONG)
    {
        if (format & PDH_FMT_1000) value->u.longValue = raw2->longvalue * 1000;
        else value->u.longValue = raw2->longvalue * pow( 10, factor );
    }
    else if (format & PDH_FMT_LARGE)
    {
        if (format & PDH_FMT_1000) value->u.largeValue = raw2->largevalue * 1000;
        else value->u.largeValue = raw2->largevalue * pow( 10, factor );
    }
    else if (format & PDH_FMT_DOUBLE)
    {
        if (format & PDH_FMT_1000) value->u.doubleValue = raw2->doublevalue * 1000;
        else value->u.doubleValue = raw2->doublevalue * pow( 10, factor );
    }
    else
    {
        WARN("unknown format %x\n", format);
        return PDH_INVALID_ARGUMENT;
    }
    return ERROR_SUCCESS;
}

/***********************************************************************
 *              PdhCalculateCounterFromRawValue   (PDH.@)
 */
PDH_STATUS WINAPI PdhCalculateCounterFromRawValue( PDH_HCOUNTER handle, DWORD format,
                                                   PPDH_RAW_COUNTER raw1, PPDH_RAW_COUNTER raw2,
                                                   PPDH_FMT_COUNTERVALUE value )
{
    PDH_STATUS ret;
    struct counter *counter = handle;

    TRACE("%p 0x%08x %p %p %p\n", handle, format, raw1, raw2, value);

    if (!value) return PDH_INVALID_ARGUMENT;

    EnterCriticalSection( &pdh_handle_cs );
    if (!counter || counter->magic != PDH_MAGIC_COUNTER)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }

    ret = format_value( counter, format, (union value *)&raw1->SecondValue,
                                         (union value *)&raw2->SecondValue, value );

    LeaveCriticalSection( &pdh_handle_cs );
    return ret;
}


/***********************************************************************
 *              PdhCloseQuery   (PDH.@)
 */
PDH_STATUS WINAPI PdhCloseQuery( PDH_HQUERY handle )
{
    struct query *query = handle;
    struct list *item, *next;

    TRACE("%p\n", handle);

    EnterCriticalSection( &pdh_handle_cs );
    if (!query || query->magic != PDH_MAGIC_QUERY)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }

    if (query->thread)
    {
        HANDLE thread = query->thread;
        SetEvent( query->stop );
        LeaveCriticalSection( &pdh_handle_cs );

        WaitForSingleObject( thread, INFINITE );

        EnterCriticalSection( &pdh_handle_cs );
        if (query->magic != PDH_MAGIC_QUERY)
        {
            LeaveCriticalSection( &pdh_handle_cs );
            return ERROR_SUCCESS;
        }
        CloseHandle( query->stop );
        CloseHandle( query->thread );
        query->thread = NULL;
    }

    LIST_FOR_EACH_SAFE( item, next, &query->counters )
    {
        struct counter *counter = LIST_ENTRY( item, struct counter, entry );

        list_remove( &counter->entry );
        destroy_counter( counter );
    }

    destroy_query( query );

    LeaveCriticalSection( &pdh_handle_cs );
    return ERROR_SUCCESS;
}

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
 *              PdhCollectQueryData   (PDH.@)
 */
PDH_STATUS WINAPI PdhCollectQueryData( PDH_HQUERY handle )
{
    struct query *query = handle;

    TRACE("%p\n", handle);

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

    LeaveCriticalSection( &pdh_handle_cs );
    return ERROR_SUCCESS;
}

static DWORD CALLBACK collect_query_thread( void *arg )
{
    struct query *query = arg;
    DWORD interval = query->interval;
    HANDLE stop = query->stop;

    for (;;)
    {
        if (WaitForSingleObject( stop, interval ) != WAIT_TIMEOUT) ExitThread( 0 );

        EnterCriticalSection( &pdh_handle_cs );
        if (query->magic != PDH_MAGIC_QUERY)
        {
            LeaveCriticalSection( &pdh_handle_cs );
            ExitThread( PDH_INVALID_HANDLE );
        }

        collect_query_data( query );

        if (!SetEvent( query->wait ))
        {
            LeaveCriticalSection( &pdh_handle_cs );
            ExitThread( 0 );
        }
        LeaveCriticalSection( &pdh_handle_cs );
    }
}

/***********************************************************************
 *              PdhCollectQueryDataEx   (PDH.@)
 */
PDH_STATUS WINAPI PdhCollectQueryDataEx( PDH_HQUERY handle, DWORD interval, HANDLE event )
{
    PDH_STATUS ret;
    struct query *query = handle;

    TRACE("%p %d %p\n", handle, interval, event);

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
    if (query->thread)
    {
        HANDLE thread = query->thread;
        SetEvent( query->stop );
        LeaveCriticalSection( &pdh_handle_cs );

        WaitForSingleObject( thread, INFINITE );

        EnterCriticalSection( &pdh_handle_cs );
        if (query->magic != PDH_MAGIC_QUERY)
        {
            LeaveCriticalSection( &pdh_handle_cs );
            return PDH_INVALID_HANDLE;
        }
        CloseHandle( query->thread );
        query->thread = NULL;
    }
    else if (!(query->stop = CreateEventW( NULL, FALSE, FALSE, NULL )))
    {
        ret = GetLastError();
        LeaveCriticalSection( &pdh_handle_cs );
        return ret;
    }
    query->wait = event;
    query->interval = interval * 1000;
    if (!(query->thread = CreateThread( NULL, 0, collect_query_thread, query, 0, NULL )))
    {
        ret = GetLastError();
        CloseHandle( query->stop );

        LeaveCriticalSection( &pdh_handle_cs );
        return ret;
    }

    LeaveCriticalSection( &pdh_handle_cs );
    return ERROR_SUCCESS;
}

/***********************************************************************
 *              PdhCollectQueryDataWithTime   (PDH.@)
 */
PDH_STATUS WINAPI PdhCollectQueryDataWithTime( PDH_HQUERY handle, LONGLONG *timestamp )
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
 *              PdhExpandWildCardPathA   (PDH.@)
 */
PDH_STATUS WINAPI PdhExpandWildCardPathA( LPCSTR szDataSource, LPCSTR szWildCardPath, LPSTR mszExpandedPathList, LPDWORD pcchPathListLength, DWORD dwFlags )
{
    FIXME("%s, %s, %p, %p, 0x%x: stub\n", debugstr_a(szDataSource), debugstr_a(szWildCardPath), mszExpandedPathList, pcchPathListLength, dwFlags);
    return PDH_NOT_IMPLEMENTED;
}

/***********************************************************************
 *              PdhExpandWildCardPathW   (PDH.@)
 */
PDH_STATUS WINAPI PdhExpandWildCardPathW( LPCWSTR szDataSource, LPCWSTR szWildCardPath, LPWSTR mszExpandedPathList, LPDWORD pcchPathListLength, DWORD dwFlags )
{
    FIXME("%s, %s, %p, %p, 0x%x: stub\n", debugstr_w(szDataSource), debugstr_w(szWildCardPath), mszExpandedPathList, pcchPathListLength, dwFlags);
    return PDH_NOT_IMPLEMENTED;
}

/***********************************************************************
 *              PdhExpandCounterPathA   (PDH.@)
 */
PDH_STATUS WINAPI PdhExpandCounterPathA( LPCSTR szWildCardPath, LPSTR mszExpandedPathList, LPDWORD pcchPathListLength )
{
    FIXME("%s, %p, %p: stub\n", debugstr_a(szWildCardPath), mszExpandedPathList, pcchPathListLength);
    return PdhExpandWildCardPathA(NULL, szWildCardPath, mszExpandedPathList, pcchPathListLength, 0);
}

/***********************************************************************
 *              PdhExpandCounterPathW   (PDH.@)
 */
PDH_STATUS WINAPI PdhExpandCounterPathW( LPCWSTR szWildCardPath, LPWSTR mszExpandedPathList, LPDWORD pcchPathListLength )
{
    FIXME("%s, %p, %p: stub\n", debugstr_w(szWildCardPath), mszExpandedPathList, pcchPathListLength);
    return PdhExpandWildCardPathW(NULL, szWildCardPath, mszExpandedPathList, pcchPathListLength, 0);
}

/***********************************************************************
 *              PdhGetCounterInfoA   (PDH.@)
 */
PDH_STATUS WINAPI PdhGetCounterInfoA( PDH_HCOUNTER handle, BOOLEAN text, LPDWORD size, PPDH_COUNTER_INFO_A info )
{
    struct counter *counter = handle;

    TRACE("%p %d %p %p\n", handle, text, size, info);

    EnterCriticalSection( &pdh_handle_cs );
    if (!counter || counter->magic != PDH_MAGIC_COUNTER)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }
    if (!size)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_ARGUMENT;
    }
    if (*size < sizeof(PDH_COUNTER_INFO_A))
    {
        *size = sizeof(PDH_COUNTER_INFO_A);
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_MORE_DATA;
    }

    memset( info, 0, sizeof(PDH_COUNTER_INFO_A) );

    info->dwType          = counter->type;
    info->CStatus         = counter->status;
    info->lScale          = counter->scale;
    info->lDefaultScale   = counter->defaultscale;
    info->dwUserData      = counter->user;
    info->dwQueryUserData = counter->queryuser;

    *size = sizeof(PDH_COUNTER_INFO_A);

    LeaveCriticalSection( &pdh_handle_cs );
    return ERROR_SUCCESS;
}

/***********************************************************************
 *              PdhGetCounterInfoW   (PDH.@)
 */
PDH_STATUS WINAPI PdhGetCounterInfoW( PDH_HCOUNTER handle, BOOLEAN text, LPDWORD size, PPDH_COUNTER_INFO_W info )
{
    struct counter *counter = handle;

    TRACE("%p %d %p %p\n", handle, text, size, info);

    EnterCriticalSection( &pdh_handle_cs );
    if (!counter || counter->magic != PDH_MAGIC_COUNTER)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }
    if (!size)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_ARGUMENT;
    }
    if (*size < sizeof(PDH_COUNTER_INFO_W))
    {
        *size = sizeof(PDH_COUNTER_INFO_W);
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_MORE_DATA;
    }

    memset( info, 0, sizeof(PDH_COUNTER_INFO_W) );

    info->dwType          = counter->type;
    info->CStatus         = counter->status;
    info->lScale          = counter->scale;
    info->lDefaultScale   = counter->defaultscale;
    info->dwUserData      = counter->user;
    info->dwQueryUserData = counter->queryuser;

    *size = sizeof(PDH_COUNTER_INFO_W);

    LeaveCriticalSection( &pdh_handle_cs );
    return ERROR_SUCCESS;
}

/***********************************************************************
 *              PdhGetCounterTimeBase   (PDH.@)
 */
PDH_STATUS WINAPI PdhGetCounterTimeBase( PDH_HCOUNTER handle, LONGLONG *base )
{
    struct counter *counter = handle;

    TRACE("%p %p\n", handle, base);

    if (!base) return PDH_INVALID_ARGUMENT;

    EnterCriticalSection( &pdh_handle_cs );
    if (!counter || counter->magic != PDH_MAGIC_COUNTER)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }

    *base = counter->base;

    LeaveCriticalSection( &pdh_handle_cs );
    return ERROR_SUCCESS;
}

/***********************************************************************
 *              PdhGetDllVersion   (PDH.@)
 */
PDH_STATUS WINAPI PdhGetDllVersion( LPDWORD version )
{
    if (!version)
        return PDH_INVALID_ARGUMENT;

    *version = PDH_VERSION;

    return ERROR_SUCCESS;
}

/***********************************************************************
 *              PdhGetFormattedCounterValue   (PDH.@)
 */
PDH_STATUS WINAPI PdhGetFormattedCounterValue( PDH_HCOUNTER handle, DWORD format,
                                               LPDWORD type, PPDH_FMT_COUNTERVALUE value )
{
    PDH_STATUS ret;
    struct counter *counter = handle;

    TRACE("%p %x %p %p\n", handle, format, type, value);

    if (!value) return PDH_INVALID_ARGUMENT;

    EnterCriticalSection( &pdh_handle_cs );
    if (!counter || counter->magic != PDH_MAGIC_COUNTER)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }
    if (counter->status)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_DATA;
    }
    if (!(ret = format_value( counter, format, &counter->one, &counter->two, value )))
    {
        value->CStatus = ERROR_SUCCESS;
        if (type) *type = counter->type;
    }

    LeaveCriticalSection( &pdh_handle_cs );
    return ret;
}

/***********************************************************************
 *              PdhGetRawCounterValue   (PDH.@)
 */
PDH_STATUS WINAPI PdhGetRawCounterValue( PDH_HCOUNTER handle, LPDWORD type,
                                         PPDH_RAW_COUNTER value )
{
    struct counter *counter = handle;

    TRACE("%p %p %p\n", handle, type, value);

    if (!value) return PDH_INVALID_ARGUMENT;

    EnterCriticalSection( &pdh_handle_cs );
    if (!counter || counter->magic != PDH_MAGIC_COUNTER)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }

    value->CStatus                  = counter->status;
    value->TimeStamp.dwLowDateTime  = counter->stamp.dwLowDateTime;
    value->TimeStamp.dwHighDateTime = counter->stamp.dwHighDateTime;
    value->FirstValue               = counter->one.largevalue;
    value->SecondValue              = counter->two.largevalue;
    value->MultiCount               = 1; /* FIXME */

    if (type) *type = counter->type;

    LeaveCriticalSection( &pdh_handle_cs );
    return ERROR_SUCCESS;
}

/***********************************************************************
 *              PdhLookupPerfIndexByNameA   (PDH.@)
 */
PDH_STATUS WINAPI PdhLookupPerfIndexByNameA( LPCSTR machine, LPCSTR name, LPDWORD index )
{
    PDH_STATUS ret;
    WCHAR *machineW = NULL;
    WCHAR *nameW;

    TRACE("%s %s %p\n", debugstr_a(machine), debugstr_a(name), index);

    if (!name) return PDH_INVALID_ARGUMENT;

    if (machine && !(machineW = pdh_strdup_aw( machine ))) return PDH_MEMORY_ALLOCATION_FAILURE;

    if (!(nameW = pdh_strdup_aw( name )))
        return PDH_MEMORY_ALLOCATION_FAILURE;

    ret = PdhLookupPerfIndexByNameW( machineW, nameW, index );

    heap_free( nameW );
    heap_free( machineW );
    return ret;
}

/***********************************************************************
 *              PdhLookupPerfIndexByNameW   (PDH.@)
 */
PDH_STATUS WINAPI PdhLookupPerfIndexByNameW( LPCWSTR machine, LPCWSTR name, LPDWORD index )
{
    unsigned int i;

    TRACE("%s %s %p\n", debugstr_w(machine), debugstr_w(name), index);

    if (!name || !index) return PDH_INVALID_ARGUMENT;

    if (machine)
    {
        FIXME("remote machine not supported\n");
        return PDH_CSTATUS_NO_MACHINE;
    }
    for (i = 0; i < sizeof(counter_sources) / sizeof(counter_sources[0]); i++)
    {
        if (pdh_match_path( counter_sources[i].path, name ))
        {
            *index = counter_sources[i].index;
            return ERROR_SUCCESS;
        }
    }
    return PDH_STRING_NOT_FOUND;
}

/***********************************************************************
 *              PdhLookupPerfNameByIndexA   (PDH.@)
 */
PDH_STATUS WINAPI PdhLookupPerfNameByIndexA( LPCSTR machine, DWORD index, LPSTR buffer, LPDWORD size )
{
    PDH_STATUS ret;
    WCHAR *machineW = NULL;
    WCHAR bufferW[PDH_MAX_COUNTER_NAME];
    DWORD sizeW = sizeof(bufferW) / sizeof(WCHAR);

    TRACE("%s %d %p %p\n", debugstr_a(machine), index, buffer, size);

    if (!buffer || !size) return PDH_INVALID_ARGUMENT;

    if (machine && !(machineW = pdh_strdup_aw( machine ))) return PDH_MEMORY_ALLOCATION_FAILURE;

    if (!(ret = PdhLookupPerfNameByIndexW( machineW, index, bufferW, &sizeW )))
    {
        int required = WideCharToMultiByte( CP_ACP, 0, bufferW, -1, NULL, 0, NULL, NULL );

        if (size && *size < required) ret = PDH_MORE_DATA;
        else WideCharToMultiByte( CP_ACP, 0, bufferW, -1, buffer, required, NULL, NULL );
        if (size) *size = required;
    }
    heap_free( machineW );
    return ret;
}

/***********************************************************************
 *              PdhLookupPerfNameByIndexW   (PDH.@)
 */
PDH_STATUS WINAPI PdhLookupPerfNameByIndexW( LPCWSTR machine, DWORD index, LPWSTR buffer, LPDWORD size )
{
    PDH_STATUS ret;
    unsigned int i;

    TRACE("%s %d %p %p\n", debugstr_w(machine), index, buffer, size);

    if (machine)
    {
        FIXME("remote machine not supported\n");
        return PDH_CSTATUS_NO_MACHINE;
    }

    if (!buffer || !size) return PDH_INVALID_ARGUMENT;
    if (!index) return ERROR_SUCCESS;

    for (i = 0; i < sizeof(counter_sources) / sizeof(counter_sources[0]); i++)
    {
        if (counter_sources[i].index == index)
        {
            WCHAR *p = strrchrW( counter_sources[i].path, '\\' ) + 1;
            unsigned int required = strlenW( p ) + 1;

            if (*size < required) ret = PDH_MORE_DATA;
            else
            {
                strcpyW( buffer, p );
                ret = ERROR_SUCCESS;
            }
            *size = required;
            return ret;
        }
    }
    return PDH_INVALID_ARGUMENT;
}

/***********************************************************************
 *              PdhOpenQueryA   (PDH.@)
 */
PDH_STATUS WINAPI PdhOpenQueryA( LPCSTR source, DWORD_PTR userdata, PDH_HQUERY *query )
{
    PDH_STATUS ret;
    WCHAR *sourceW = NULL;

    TRACE("%s %lx %p\n", debugstr_a(source), userdata, query);

    if (source && !(sourceW = pdh_strdup_aw( source ))) return PDH_MEMORY_ALLOCATION_FAILURE;

    ret = PdhOpenQueryW( sourceW, userdata, query );
    heap_free( sourceW );

    return ret;
}

/***********************************************************************
 *              PdhOpenQueryW   (PDH.@)
 */
PDH_STATUS WINAPI PdhOpenQueryW( LPCWSTR source, DWORD_PTR userdata, PDH_HQUERY *handle )
{
    struct query *query;

    TRACE("%s %lx %p\n", debugstr_w(source), userdata, handle);

    if (!handle) return PDH_INVALID_ARGUMENT;

    if (source)
    {
        FIXME("log file data source not supported\n");
        return PDH_INVALID_ARGUMENT;
    }
    if ((query = create_query()))
    {
        query->user = userdata;
        *handle = query;

        return ERROR_SUCCESS;
    }
    return PDH_MEMORY_ALLOCATION_FAILURE;
}

/***********************************************************************
 *              PdhRemoveCounter   (PDH.@)
 */
PDH_STATUS WINAPI PdhRemoveCounter( PDH_HCOUNTER handle )
{
    struct counter *counter = handle;

    TRACE("%p\n", handle);

    EnterCriticalSection( &pdh_handle_cs );
    if (!counter || counter->magic != PDH_MAGIC_COUNTER)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }

    list_remove( &counter->entry );
    destroy_counter( counter );

    LeaveCriticalSection( &pdh_handle_cs );
    return ERROR_SUCCESS;
}

/***********************************************************************
 *              PdhSetCounterScaleFactor   (PDH.@)
 */
PDH_STATUS WINAPI PdhSetCounterScaleFactor( PDH_HCOUNTER handle, LONG factor )
{
    struct counter *counter = handle;

    TRACE("%p\n", handle);

    EnterCriticalSection( &pdh_handle_cs );
    if (!counter || counter->magic != PDH_MAGIC_COUNTER)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_HANDLE;
    }
    if (factor < PDH_MIN_SCALE || factor > PDH_MAX_SCALE)
    {
        LeaveCriticalSection( &pdh_handle_cs );
        return PDH_INVALID_ARGUMENT;
    }

    counter->scale = factor;

    LeaveCriticalSection( &pdh_handle_cs );
    return ERROR_SUCCESS;
}

/***********************************************************************
 *              PdhValidatePathA   (PDH.@)
 */
PDH_STATUS WINAPI PdhValidatePathA( LPCSTR path )
{
    PDH_STATUS ret;
    WCHAR *pathW;

    TRACE("%s\n", debugstr_a(path));

    if (!path) return PDH_INVALID_ARGUMENT;
    if (!(pathW = pdh_strdup_aw( path ))) return PDH_MEMORY_ALLOCATION_FAILURE;

    ret = PdhValidatePathW( pathW );

    heap_free( pathW );
    return ret;
}

static PDH_STATUS validate_path( LPCWSTR path )
{
    if (!path || !*path) return PDH_INVALID_ARGUMENT;
    if (*path++ != '\\' || !strchrW( path, '\\' )) return PDH_CSTATUS_BAD_COUNTERNAME;
    return ERROR_SUCCESS;
 }

/***********************************************************************
 *              PdhValidatePathW   (PDH.@)
 */
PDH_STATUS WINAPI PdhValidatePathW( LPCWSTR path )
{
    PDH_STATUS ret;
    unsigned int i;

    TRACE("%s\n", debugstr_w(path));

    if ((ret = validate_path( path ))) return ret;

    for (i = 0; i < sizeof(counter_sources) / sizeof(counter_sources[0]); i++)
        if (pdh_match_path( counter_sources[i].path, path )) return ERROR_SUCCESS;

    return PDH_CSTATUS_NO_COUNTER;
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

/***********************************************************************
 *              PdhMakeCounterPathA   (PDH.@)
 */
PDH_STATUS WINAPI PdhMakeCounterPathA( PDH_COUNTER_PATH_ELEMENTS_A *e, LPSTR buffer,
                                       LPDWORD buflen, DWORD flags )
{
    PDH_STATUS ret = PDH_MEMORY_ALLOCATION_FAILURE;
    PDH_COUNTER_PATH_ELEMENTS_W eW;
    WCHAR *bufferW;
    DWORD buflenW;

    TRACE("%p %p %p 0x%08x\n", e, buffer, buflen, flags);

    if (!e || !buflen) return PDH_INVALID_ARGUMENT;

    memset( &eW, 0, sizeof(eW) );
    if (e->szMachineName    && !(eW.szMachineName    = pdh_strdup_aw( e->szMachineName ))) goto done;
    if (e->szObjectName     && !(eW.szObjectName     = pdh_strdup_aw( e->szObjectName ))) goto done;
    if (e->szInstanceName   && !(eW.szInstanceName   = pdh_strdup_aw( e->szInstanceName ))) goto done;
    if (e->szParentInstance && !(eW.szParentInstance = pdh_strdup_aw( e->szParentInstance ))) goto done;
    if (e->szCounterName    && !(eW.szCounterName    = pdh_strdup_aw( e->szCounterName ))) goto done;
    eW.dwInstanceIndex = e->dwInstanceIndex;

    buflenW = 0;
    ret = PdhMakeCounterPathW( &eW, NULL, &buflenW, flags );
    if (ret == PDH_MORE_DATA)
    {
        if ((bufferW = heap_alloc( buflenW * sizeof(WCHAR) )))
        {
            if (!(ret = PdhMakeCounterPathW( &eW, bufferW, &buflenW, flags )))
            {
                int len = WideCharToMultiByte(CP_ACP, 0, bufferW, -1, NULL, 0, NULL, NULL);
                if (*buflen >= len) WideCharToMultiByte(CP_ACP, 0, bufferW, -1, buffer, *buflen, NULL, NULL);
                else ret = PDH_MORE_DATA;
                *buflen = len;
            }
            heap_free( bufferW );
        }
        else
            ret = PDH_MEMORY_ALLOCATION_FAILURE;
    }

done:
    heap_free( eW.szMachineName );
    heap_free( eW.szObjectName );
    heap_free( eW.szInstanceName );
    heap_free( eW.szParentInstance );
    heap_free( eW.szCounterName );
    return ret;
}

/***********************************************************************
 *              PdhMakeCounterPathW   (PDH.@)
 */
PDH_STATUS WINAPI PdhMakeCounterPathW( PDH_COUNTER_PATH_ELEMENTS_W *e, LPWSTR buffer,
                                       LPDWORD buflen, DWORD flags )
{
    static const WCHAR bslash[] = {'\\',0};
    static const WCHAR fslash[] = {'/',0};
    static const WCHAR lparen[] = {'(',0};
    static const WCHAR rparen[] = {')',0};
    static const WCHAR fmt[]    = {'#','%','u',0};

    WCHAR path[PDH_MAX_COUNTER_NAME], instance[12];
    PDH_STATUS ret = ERROR_SUCCESS;
    DWORD len;

    TRACE("%p %p %p 0x%08x\n", e, buffer, buflen, flags);

    if (flags) FIXME("unimplemented flags 0x%08x\n", flags);

    if (!e || !e->szCounterName || !e->szObjectName || !buflen)
        return PDH_INVALID_ARGUMENT;

    path[0] = 0;
    if (e->szMachineName)
    {
        strcatW(path, bslash);
        strcatW(path, bslash);
        strcatW(path, e->szMachineName);
    }
    strcatW(path, bslash);
    strcatW(path, e->szObjectName);
    if (e->szInstanceName)
    {
        strcatW(path, lparen);
        if (e->szParentInstance)
        {
            strcatW(path, e->szParentInstance);
            strcatW(path, fslash);
        }
        strcatW(path, e->szInstanceName);
        sprintfW(instance, fmt, e->dwInstanceIndex);
        strcatW(path, instance);
        strcatW(path, rparen);
    }
    strcatW(path, bslash);
    strcatW(path, e->szCounterName);

    len = strlenW(path) + 1;
    if (*buflen >= len) strcpyW(buffer, path);
    else ret = PDH_MORE_DATA;
    *buflen = len;
    return ret;
}

/***********************************************************************
 *              PdhEnumObjectItemsA   (PDH.@)
 */
PDH_STATUS WINAPI PdhEnumObjectItemsA(LPCSTR szDataSource, LPCSTR szMachineName, LPCSTR szObjectName,
                                      LPSTR mszCounterList, LPDWORD pcchCounterListLength, LPSTR mszInstanceList,
                                      LPDWORD pcchInstanceListLength, DWORD dwDetailLevel, DWORD dwFlags)
{
    FIXME("%s, %s, %s, %p, %p, %p, %p, %d, 0x%x: stub\n", debugstr_a(szDataSource), debugstr_a(szMachineName),
         debugstr_a(szObjectName), mszCounterList, pcchCounterListLength, mszInstanceList,
         pcchInstanceListLength, dwDetailLevel, dwFlags);

    return PDH_NOT_IMPLEMENTED;
}

/***********************************************************************
 *              PdhEnumObjectItemsW   (PDH.@)
 */
PDH_STATUS WINAPI PdhEnumObjectItemsW(LPCWSTR szDataSource, LPCWSTR szMachineName, LPCWSTR szObjectName,
                                      LPWSTR mszCounterList, LPDWORD pcchCounterListLength, LPWSTR mszInstanceList,
                                      LPDWORD pcchInstanceListLength, DWORD dwDetailLevel, DWORD dwFlags)
{
    FIXME("%s, %s, %s, %p, %p, %p, %p, %d, 0x%x: stub\n", debugstr_w(szDataSource), debugstr_w(szMachineName),
         debugstr_w(szObjectName), mszCounterList, pcchCounterListLength, mszInstanceList,
         pcchInstanceListLength, dwDetailLevel, dwFlags);

    return PDH_NOT_IMPLEMENTED;
}

/***********************************************************************
 *              PdhSetDefaultRealTimeDataSource   (PDH.@)
 */
PDH_STATUS WINAPI PdhSetDefaultRealTimeDataSource( DWORD source )
{
    FIXME("%u\n", source);
    return ERROR_SUCCESS;
}

PDH_STATUS 
PdhAdd009CounterA(
	PDH_HQUERY hQuery, 
	LPCSTR lpMultiByteStr, 
	DWORD_PTR dwUserData, 
	PDH_HCOUNTER *phCounter
)
{
    return ERROR_SUCCESS;	
}

PDH_STATUS 
PdhAdd009CounterW(
	PDH_HQUERY hQuery, 
	LPWSTR lpMultiByteStr, 
	DWORD_PTR dwUserData, 
	PDH_HCOUNTER *phCounter
)
{
    return ERROR_SUCCESS;	
}

PDH_STATUS 
PdhBindInputDataSourceA(
	PDH_HLOG *phDataSource, 
	LPCSTR LogFileNameList
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhBindInputDataSourceW(
	PDH_HLOG *phDataSource, 
	LPWSTR LogFileNameList
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhBrowseCountersA(
   PPDH_BROWSE_DLG_CONFIG_A pBrowseDlgData
)
{
    return ERROR_SUCCESS;	
}

PDH_STATUS 
PdhBrowseCountersW(
   PPDH_BROWSE_DLG_CONFIG_W pBrowseDlgData
)
{
    return ERROR_SUCCESS;
}

PDH_STATUS 
PdhBrowseCountersHA(
	PPDH_BROWSE_DLG_CONFIG_HA pBrowseDlgData
)
{
    return ERROR_SUCCESS;	
}

PDH_STATUS 
PdhBrowseCountersHW(
	PPDH_BROWSE_DLG_CONFIG_HW pBrowseDlgData
)
{
    return ERROR_SUCCESS;	
}

PDH_STATUS 
PdhCloseLog(
	PDH_HLOG hLog, 
	DWORD dwFlags
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhComputeCounterStatistics(
	PDH_HCOUNTER hCounter, 
	DWORD dwFormat, 
	DWORD dwFirstEntry, 
	DWORD dwNumEntries, 
	PPDH_RAW_COUNTER lpRawValueArray, 
	PPDH_STATISTICS data
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhConnectMachineA(
	LPCSTR szMachineName
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhConnectMachineW(
	LPWSTR szMachineName
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhCreateSQLTablesA(
	LPCSTR szDataSource
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhCreateSQLTablesW(
	LPWSTR szDataSource
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumLogSetNamesA(
	LPCSTR szDataSource, 
	PZZSTR mszDataSetNameList, 
	LPDWORD pcchBufferLength
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumLogSetNamesW(
	LPWSTR szDataSource, 
	PZZWSTR mszDataSetNameList, 
	LPDWORD pcchBufferLength
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumMachinesA(
	LPCSTR szDataSource, 
	PZZSTR mszMachineList, 
	LPDWORD pcchBuffe
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumMachinesW(
	LPWSTR szDataSource, 
	PZZWSTR mszMachineList, 
	LPDWORD pcchBuffe
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumMachinesHA(
	PDH_HLOG hDataSource, 
	PZZSTR mszMachineList, 
	LPDWORD pcchBuffe
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumMachinesHW(
	PDH_HLOG hDataSource, 
	PZZWSTR mszMachineList, 
	LPDWORD pcchBuffe
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumObjectItemsHA(
	PDH_HLOG hDataSource, 
	LPCSTR szMachineName, 
	LPCSTR szObjectName, 
	PZZSTR mszCounterList, 
	LPDWORD pcchCounterListLength,
	PZZSTR mszInstanceList, 
	LPDWORD pcchInstanceListLength, 
	DWORD dwDetailLevel, 
	DWORD dwFlags
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumObjectItemsHW(
	PDH_HLOG hDataSource, 
	LPWSTR szMachineName, 
	LPWSTR szObjectName, 
	PZZSTR mszCounterList, 
	LPDWORD pcchCounterListLength,
	PZZWSTR mszInstanceList, 
	LPDWORD pcchInstanceListLength, 
	DWORD dwDetailLevel, 
	DWORD dwFlags
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumObjectsA(
	LPCSTR szDataSource, 
	LPCSTR szMachineName, 
	PZZSTR mszObjectList, 
	LPDWORD pcchBufferSize, 
	DWORD dwDetailLevel, 
	BOOL bRefresh
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumObjectsW(
	LPCSTR szDataSource, 
	LPCSTR szMachineName, 
	PZZSTR mszObjectList, 
	LPDWORD pcchBufferSize, 
	DWORD dwDetailLevel, 
	BOOL bRefresh
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumObjectsHA(
	PDH_HLOG hDataSource, 
	LPCSTR szMachineName, 
	PZZSTR mszObjectList, 
	LPDWORD pcchBufferSize, 
	DWORD dwDetailLevel, 
	BOOL bRefresh
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhEnumObjectsHW(
	PDH_HLOG hDataSource, 
	LPWSTR szMachineName, 
	PZZWSTR mszObjectList, 
	LPDWORD pcchBufferSize, 
	DWORD dwDetailLevel, 
	BOOL bRefresh
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhExpandWildCardPathHA(
	PDH_HLOG hDataSource, 
	LPCSTR szWildCardPath, 
	PZZSTR mszExpandedPathList, 
	LPDWORD pcchPathListLength, 
	DWORD dwFlags
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhExpandWildCardPathHW(
	PDH_HLOG hDataSource, 
	LPWSTR szWildCardPath, 
	PZZWSTR mszExpandedPathList, 
	LPDWORD pcchPathListLength, 
	DWORD dwFlags
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhFormatFromRawValue(
	DWORD dwCounterType, 
	DWORD dwFormat, 
	LONGLONG *pTimeBase, 
	PPDH_RAW_COUNTER pRawValue1, 
	PPDH_RAW_COUNTER pRawValue2, 
	PPDH_FMT_COUNTERVALUE pFmtValue
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhGetDataSourceTimeRangeA(
	LPCSTR szDataSource, 
	LPDWORD pdwNumEntries, 
	PPDH_TIME_INFO pInfo, 
	LPDWORD pdwBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhGetDataSourceTimeRangeW(
	LPWSTR szDataSource, 
	LPDWORD pdwNumEntries, 
	PPDH_TIME_INFO pInfo, 
	LPDWORD pdwBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetDataSourceTimeRangeH(
	PDH_HLOG hDataSource, 
	LPDWORD pdwNumEntries, 
	PPDH_TIME_INFO pInfo, 
	LPDWORD pdwBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetDefaultPerfCounterA(
	LPCSTR szDataSource, 
	LPCSTR szMachineName, 
	LPCSTR szObjectName, 
	LPSTR szDefaultCounterName, 
	LPDWORD pcchBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetDefaultPerfCounterW(
	LPWSTR szDataSource, 
	LPWSTR szMachineName, 
	LPWSTR szObjectName, 
	LPWSTR szDefaultCounterName, 
	LPDWORD pcchBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetDefaultPerfCounterHA(
	PDH_HLOG hDataSource, 
	LPCSTR szMachineName, 
	LPCSTR szObjectName, 
	LPSTR szDefaultCounterName, 
	LPDWORD pcchBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetDefaultPerfCounterHW(
	PDH_HLOG hDataSource, 
	LPWSTR szMachineName, 
	LPWSTR szObjectName, 
	LPWSTR szDefaultCounterName, 
	LPDWORD pcchBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetDefaultPerfObjectA(
	LPCSTR szDataSource, 
	LPCSTR szMachineName, 
	LPSTR szDefaultObjectName, 
	LPDWORD pcchBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetDefaultPerfObjectW(
	LPWSTR szDataSource, 
	LPWSTR szMachineName, 
	LPWSTR szDefaultObjectName, 
	LPDWORD pcchBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetDefaultPerfObjectHA(
	PDH_HLOG hDataSource, 
	LPCSTR szMachineName, 
	LPSTR szDefaultObjectName, 
	LPDWORD pcchBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetDefaultPerfObjectHW(
	PDH_HLOG hDataSources, 
	LPWSTR szMachineName, 
	LPWSTR szDefaultObjectName, 
	LPDWORD pcchBufferSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetFormattedCounterArrayA(
	PDH_HCOUNTER hCounter, 
	DWORD dwFormat, 
	LPDWORD lpdwBufferSize, 
	LPDWORD lpdwItemCount, 
	PPDH_FMT_COUNTERVALUE_ITEM_A ItemBuffer
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetFormattedCounterArrayW(
	PDH_HCOUNTER hCounter, 
	DWORD dwFormat, 
	LPDWORD lpdwBufferSize, 
	LPDWORD lpdwItemCount, 
	PPDH_FMT_COUNTERVALUE_ITEM_W ItemBuffer
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetLogFileSize(
	PDH_HLOG hLog, 
	LONGLONG *llSize
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetLogFileTypeA(
	LPCSTR lpMultiByteStr, 
	DWORD *type
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetLogFileTypeW(
	LPWSTR lpMultiByteStr, 
	DWORD *type
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhGetLogSetGUID(
	PDH_HLOG hLog, 
	GUID *pGuid, int *pRunId
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhGetRawCounterArrayA(
	PDH_HCOUNTER hCounter, 
	LPDWORD lpdwBufferSize, 
	LPDWORD lpdwItemCount, 
	PPDH_RAW_COUNTER_ITEM_A ItemBuffer
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhGetRawCounterArrayW(
	PDH_HCOUNTER hCounter, 
	LPDWORD lpdwBufferSize, 
	LPDWORD lpdwItemCount, 
	PPDH_RAW_COUNTER_ITEM_W ItemBuffer
)
{
    return ERROR_SUCCESS;		
}

BOOL
PdhIsRealTimeQuery(
	PDH_HQUERY hQuery
)
{
	return TRUE;
}

PDH_STATUS
PdhListLogFileHeaderA(
	LPCSTR szLogFileName, 
	LPWSTR pszDest, 
	size_t cchDest
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhListLogFileHeaderW(
	LPWSTR szLogFileName, 
	LPWSTR pszDest, 
	size_t cchDest
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhOpenLogA(
	LPCSTR szLogFileName, 
	DWORD dwAccessFlags, 
	LPDWORD lpdwLogType, 
	PDH_HQUERY hQuery, 
	DWORD dwMaxSize, 
	LPCSTR szUserCaption, 
	PDH_HLOG *phLog
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhOpenLogW(
	LPWSTR szLogFileName, 
	DWORD dwAccessFlags, 
	LPDWORD lpdwLogType, 
	PDH_HQUERY hQuery, 
	DWORD dwMaxSize, 
	LPWSTR szUserCaption, 
	PDH_HLOG *phLog
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhOpenQueryH(
	PDH_HLOG hDataSource, 
	DWORD_PTR dwUserData, 
	PDH_HQUERY *phQuery
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhParseCounterPathA(
	LPCSTR szFullPathBuffer, 
	PPDH_COUNTER_PATH_ELEMENTS_A pCounterPathElements, 
	LPDWORD pdwBufferSize, 
	DWORD dwFlags
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhParseCounterPathW(
	LPWSTR szFullPathBuffer, 
	PPDH_COUNTER_PATH_ELEMENTS_W pCounterPathElements, 
	LPDWORD pdwBufferSize, 
	DWORD dwFlags
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhParseInstanceNameA(
	LPCSTR szInstanceString, 
	LPSTR szInstanceName, 
	LPDWORD pcchInstanceNameLength, 
	LPSTR szParentName, 
	LPDWORD pcchParentNameLength, 
	LPDWORD lpIndex
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhParseInstanceNameW(
	LPWSTR szInstanceString, 
	LPWSTR szInstanceName, 
	LPDWORD pcchInstanceNameLength, 
	LPWSTR szParentName, 
	LPDWORD pcchParentNameLength, 
	LPDWORD lpIndex
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaCreateA(
	LPSTR pszSrc, 
	LPSTR szInstanceName, 
	DWORD Flags
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaCreateW(
	LPWSTR pszSrc, 
	LPWSTR szInstanceName, 
	DWORD Flags
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaDeleteA(
	LPSTR Destination, 
	LPSTR Source
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaDeleteW(
	LPWSTR Destination, 
	LPWSTR Source
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaEnumCollectionsA(
	LPSTR pszSrc, 
	PULONG a2, 
	PVOID a3
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaEnumCollectionsW(
	LPWSTR pszSrc, 
	PULONG a2, 
	PVOID a3
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaGetInfoA(
	LPSTR pszSrc, 
	LPSTR a2, 
	PULONG a3, 
	PVOID a4
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaGetInfoW(
	LPWSTR pszSrc, 
	LPWSTR a2, 
	PULONG a3, 
	PVOID a4
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhPlaGetLogFileNameW(
	wchar_t *pszSrc, 
	DWORD lpMachineName, 
	const void *a3, 
	unsigned __int32 a4, 
	unsigned int *a5, 
	unsigned __int16 *a6
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PlaTimeInfoToMilliSeconds(PVOID a1, PVOID a2)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhiPlaSetRunAs(
	LPWSTR pszSrc, 
	unsigned __int16 *a2, 
	LPWSTR a3, 
	LPWSTR a4
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhiPlaRunAs(
	wchar_t *pszSrc, 
	wchar_t *a2, 
	HANDLE a3
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhiPlaGetVersion(
	const wchar_t *lpMachineName, 
	PVOID a2
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhiPlaFormatBlanksA(
	char *lpMachineName, 
	unsigned __int16 *a2
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhiPlaFormatBlanksW(
	wchar_t *lpMachineName, 
	unsigned __int16 *a2
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhiPla2003SP1Installed(
	const wchar_t *lpMachineName, 
	HANDLE a2)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhVerifySQLDBA(
	LPCSTR szDataSource
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhVerifySQLDBW(
	LPCWSTR szDataSource
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhVbUpdateLog(
	PDH_HLOG hLog, 
	LPCSTR szUserString
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhVbOpenQuery(
	PDH_HQUERY a1
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhVbOpenLog(
	LPCSTR szLogFileName, 
	DWORD dwAccessFlags, 
	LPDWORD lpdwLogType, 
	PDH_HQUERY hQuery, 
	DWORD dwMaxSize, 
	LPCSTR szUserCaption, 
	PDH_HLOG *phLog
)
{
    return ERROR_SUCCESS;		
}

BOOL 
PdhVbIsGoodStatus(
	int a1)
{
    return TRUE;		
}	

PDH_STATUS 
PdhVbGetOneCounterPath(
	CHAR *lpString, 
	DWORD a2, 
	DWORD a3, 
	CHAR *a4
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS  
PdhVbGetLogFileSize(
	PDH_HLOG hLog,
	int a2
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS  
PdhVbGetDoubleCounterValue(
	PDH_HLOG hLog,
	int a2
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhVbGetCounterPathFromList(
	HANDLE a1, 
	LPSTR pszDest, 
	DWORD cchDest
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhVbGetCounterPathElements(
	LPCSTR szFullPathBuffer, 
	LPCSTR pszDest, 
	char *a3, 
	char *a4, 
	char *a5, 
	char *a6, 
	size_t cchDest
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhVbCreateCounterPathList(
	DWORD a1, 
	CHAR *a2
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS  
PdhVbAddCounter(
	PDH_HQUERY hQuery, 
	LPCSTR szFullCounterPath, 
	PDH_HCOUNTER a3
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS  
PdhUpdateLogA(
	PDH_HLOG hLog, 
	LPCSTR szUserString
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS  
PdhUpdateLogW(
	PDH_HLOG hLog, 
	LPCWSTR szUserString
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS  
PdhUpdateLogFileCatalog(
	PDH_HLOG hLog
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhTranslateLocaleCounterA(
	LPCSTR lpString, 
	unsigned __int16 *a2, 
	int a3
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhTranslateLocaleCounterW(
	LPCWSTR lpString, 
	unsigned __int16 *a2, 
	int a3
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhTranslate009CounterA(
	LPCSTR lpString, 
	unsigned __int16 *a2, 
	int a3
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhTranslate009CounterW(
	LPCWSTR lpString, 
	unsigned __int16 *a2, 
	int a3
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhSetQueryTimeRange(
	PDH_HQUERY hQuery, 
	PPDH_TIME_INFO pInfo
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS 
PdhSetLogSetRunID(
	PDH_HLOG hLog, 
	int RunId
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS  
PdhSelectDataSourceA(
	HWND hWndOwner,
	DWORD dwFlags, 
	LPSTR szDataSource, 
	LPDWORD pcchBufferLength
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS  
PdhSelectDataSourceW(
	HWND hWndOwner,
	DWORD dwFlags, 
	LPWSTR szDataSource, 
	LPDWORD pcchBufferLength
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS  
PdhRelogA(
	PDH_HLOG hDataSource, 
	LPSTR a2
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS  
PdhRelogW(
	PDH_HLOG hDataSource, 
	LPWSTR a2
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS 
PdhReadRawLogRecord(
	PDH_HLOG hLog, 
	FILETIME ftRecord, 
	PPDH_RAW_LOG_RECORD pRawLogRecord, 
	LPDWORD pdwBufferLength
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS 
PdhPlaValidateInfoA(
	LPSTR pszSrc, 
	LPSTR a2, 
	int a3
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS 
PdhPlaValidateInfoW(
	wchar_t *pszSrc, 
	wchar_t *a2, 
	int a3
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaStartA(
	LPSTR hSCObject, 
	LPSTR pszSrc
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaStartW(
	LPWSTR hSCObject, 
	LPWSTR pszSrc
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaStopA(
	LPSTR hSCObject, 
	LPSTR pszSrc
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaStopW(
	LPWSTR hSCObject, 
	LPWSTR pszSrc
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaSetRunAsA(
	LPSTR *pszSrc, 
	LPSTR *a2, 
	unsigned __int16 *a3, 
	unsigned __int16 *a4
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaSetRunAsW(
	wchar_t *pszSrc, 
	wchar_t *a2, 
	unsigned __int16 *a3, 
	unsigned __int16 *a4
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaSetItemListA(
	LPSTR pszSrc, 
	LPSTR a2, 
	struct _PDH_PLA_ITEM_A *a3
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaSetItemListW(
	LPWSTR pszSrc, 
	LPWSTR a2, 
	struct _PDH_PLA_ITEM_W *a3
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaSetInfoA(
	LPSTR pszSrc, 
	LPSTR lpMachineName, 
	int a3
)
{
    return ERROR_SUCCESS;		
}		

PDH_STATUS
PdhPlaSetInfoW(
	LPWSTR pszSrc, 
	LPWSTR lpMachineName, 
	int a3
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaScheduleA(
	LPSTR pszSrc, 
	LPSTR a2, 
	unsigned __int32 a3, 
	const void *a4
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaScheduleW(
	LPWSTR pszSrc, 
	LPWSTR a2, 
	unsigned __int32 a3, 
	const void *a4
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaRemoveAllItemsA(
	LPSTR pszSrc, 
	LPSTR a2
)
{
    return ERROR_SUCCESS;		
}		

PDH_STATUS
PdhPlaRemoveAllItemsW(
	LPWSTR pszSrc, 
	LPWSTR a2
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaGetScheduleA(
	LPSTR pszSrc, 
	LPSTR a2, 
	int a3, 
	int a4, 
	int a5
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaGetScheduleW(
	LPWSTR pszSrc, 
	LPWSTR a2, 
	int a3, 
	int a4, 
	int a5
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaAddItemA(
	int a1, 
	int a2, 
	int a3
)
{
    return ERROR_SUCCESS;		
}	

PDH_STATUS
PdhPlaAddItemW(
	int a1, 
	int a2, 
	int a3
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaGetLogFileNameA(
	char *a1, 
	char *a2, 
	struct _PDH_PLA_INFO_A *a3, 
	unsigned __int32 a4, 
	unsigned __int32 *a5, 
	char *a6
)
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaUpgradeW()
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhPlaDowngradeW()
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhiPlaDowngrade()
{
    return ERROR_SUCCESS;		
}

PDH_STATUS
PdhiPlaUpgrade()
{
    return ERROR_SUCCESS;		
}

HRESULT 
DllInstall(
	BOOL bInstall, 
	LPCWSTR pszCmdLine
)
{
	return 0;
}