/*++

Copyright (c) 2021 Shorthorn Project

Module Name:

    string.c

Abstract:

    String handle functions for Win32

Author:

    Skulltrail 05-April-2021

Revision History:

--*/

#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(string); 

INT WINAPI DECLSPEC_HOTPATCH LoadStringW(HINSTANCE instance, UINT resource_id, LPWSTR buffer, INT buflen)
{
    int string_num, i;
    HGLOBAL hmem;
    HRSRC hrsrc;
    WCHAR *p;

    TRACE("instance = %p, id = %04x, buffer = %p, length = %d\n", instance, resource_id, buffer, buflen);

    if (!buffer)
        return 0;

    /* Use loword (incremented by 1) as resourceid */
    hrsrc = FindResourceW(instance, MAKEINTRESOURCEW((LOWORD(resource_id) >> 4) + 1), (LPWSTR)RT_STRING);
    if (!hrsrc) return 0;
    hmem = LoadResource(instance, hrsrc);
    if (!hmem) return 0;

    p = LockResource(hmem);
    string_num = resource_id & 0x000f;
    for (i = 0; i < string_num; i++)
        p += *p + 1;

    TRACE("strlen = %d\n", (int)*p );

    /*if buflen == 0, then return a read-only pointer to the resource itself in buffer
    it is assumed that buffer is actually a (LPWSTR *) */
    if (buflen == 0)
    {
        *((LPWSTR *)buffer) = p + 1;
        return *p;
    }

    i = min(buflen - 1, *p);
    if (i > 0)
    {
        memcpy(buffer, p + 1, i * sizeof (WCHAR));
        buffer[i] = 0;
    }
    else
    {
        if (buflen > 1)
        {
            buffer[0] = 0;
            return 0;
        }
    }

    TRACE("returning %s\n", debugstr_w(buffer));
    return i;
}

INT WINAPI DECLSPEC_HOTPATCH LoadStringA(HINSTANCE instance, UINT resource_id, LPSTR buffer, INT buflen)
{
    DWORD retval = 0;
    HGLOBAL hmem;
    HRSRC hrsrc;

    TRACE("instance = %p, id = %04x, buffer = %p, length = %d\n", instance, resource_id, buffer, buflen);

    if (!buflen) return -1;

    /* Use loword (incremented by 1) as resourceid */
    if ((hrsrc = FindResourceW(instance, MAKEINTRESOURCEW((LOWORD(resource_id) >> 4) + 1), (LPWSTR)RT_STRING )) &&
            (hmem = LoadResource(instance, hrsrc)))
    {
        const WCHAR *p = LockResource(hmem);
        unsigned int id = resource_id & 0x000f;

        while (id--) p += *p + 1;

        RtlUnicodeToMultiByteN(buffer, buflen - 1, &retval, p + 1, *p * sizeof(WCHAR));
    }
    buffer[retval] = 0;
    TRACE("returning %s\n", debugstr_a(buffer));
    return retval;
}