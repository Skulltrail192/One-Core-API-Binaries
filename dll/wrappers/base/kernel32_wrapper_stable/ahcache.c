/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    ahcache.c

Abstract:

    Cache handling functions for use in kernel32.dll

Author:

    Skulltrail 19-March-2017

Revision History:

--*/

#include <main.h>

/*
 * @implemented
 */
VOID
WINAPI
BasepFreeAppCompatData(IN PVOID AppCompatData,
                       IN PVOID AppCompatSxsData)
{
    /* Free the input pointers if present */
    if (AppCompatData) RtlFreeHeap(RtlGetProcessHeap(), 0, AppCompatData);
    if (AppCompatSxsData) RtlFreeHeap(RtlGetProcessHeap(), 0, AppCompatSxsData);
}