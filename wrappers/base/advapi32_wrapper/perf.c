/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    perf.c

Abstract:

    Performance counter functions
	
Author:

    Skulltrail 18-July-2021

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(perf);

/***********************************************************************
 *           PerfCreateInstance   (KERNELBASE.@)
 */
PPERF_COUNTERSET_INSTANCE WINAPI PerfCreateInstance(HANDLE handle, LPCGUID guid,
                                                    const WCHAR *name, ULONG id)
{
    FIXME("%p %s %s %u: stub\n", handle, debugstr_guid(guid), debugstr_w(name), id);
    return NULL;
}

/***********************************************************************
 *           PerfDeleteInstance   (KERNELBASE.@)
 */
ULONG WINAPI PerfDeleteInstance(HANDLE provider, PPERF_COUNTERSET_INSTANCE block)
{
    FIXME("%p %p: stub\n", provider, block);
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/***********************************************************************
 *           PerfSetCounterSetInfo   (KERNELBASE.@)
 */
ULONG WINAPI PerfSetCounterSetInfo(HANDLE handle, PPERF_COUNTERSET_INFO template, ULONG size)
{
    FIXME("%p %p %u: stub\n", handle, template, size);
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/***********************************************************************
 *           PerfSetCounterRefValue   (KERNELBASE.@)
 */
ULONG WINAPI PerfSetCounterRefValue(HANDLE provider, PPERF_COUNTERSET_INSTANCE instance,
                                    ULONG counterid, void *address)
{
    FIXME("%p %p %u %p: stub\n", provider, instance, counterid, address);
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/***********************************************************************
 *           PerfStartProvider   (KERNELBASE.@)
 */
ULONG WINAPI PerfStartProvider(GUID *guid, PERFLIBREQUEST callback, HANDLE *provider)
{
    FIXME("%s %p %p: stub\n", debugstr_guid(guid), callback, provider);
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/***********************************************************************
 *           PerfStartProviderEx   (KERNELBASE.@)
 */
ULONG WINAPI PerfStartProviderEx(GUID *guid, PPERF_PROVIDER_CONTEXT context, HANDLE *provider)
{
    FIXME("%s %p %p: stub\n", debugstr_guid(guid), context, provider);
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/***********************************************************************
 *           PerfStopProvider   (KERNELBASE.@)
 */
ULONG WINAPI PerfStopProvider(HANDLE handle)
{
    FIXME("%p: stub\n", handle);
    return ERROR_CALL_NOT_IMPLEMENTED;
}