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

#include <wine/debug.h>
#include <wine/list.h>
#include <wine/unicode.h>
#include <winsock.h>

WINE_DEFAULT_DEBUG_CHANNEL(ws2_32);

/***********************************************************************
 *              inet_ntop                      (WS2_32.@)
 */
PCSTR WINAPI WS_inet_ntop( INT family, PVOID addr, PSTR buffer, SIZE_T len )
{
#ifdef HAVE_INET_NTOP
    struct WS_in6_addr *in6;
    struct WS_in_addr  *in;
    PCSTR pdst;

    TRACE("family %d, addr (%p), buffer (%p), len %ld\n", family, addr, buffer, len);
    if (!buffer)
    {
        SetLastError( STATUS_INVALID_PARAMETER );
        return NULL;
    }

    switch (family)
    {
    case WS_AF_INET:
    {
        in = addr;
        pdst = inet_ntop( AF_INET, &in->WS_s_addr, buffer, len );
        break;
    }
    case WS_AF_INET6:
    {
        in6 = addr;
        pdst = inet_ntop( AF_INET6, in6->WS_s6_addr, buffer, len );
        break;
    }
    default:
        SetLastError( WSAEAFNOSUPPORT );
        return NULL;
    }

    if (!pdst) SetLastError( STATUS_INVALID_PARAMETER );
    return pdst;
#else
    FIXME( "not supported on this platform\n" );
    SetLastError( WSAEAFNOSUPPORT );
    return NULL;
#endif
}

/***********************************************************************
*              inet_pton                      (WS2_32.@)
*/
INT WINAPI WS_inet_pton( INT family, PCSTR addr, PVOID buffer)
{
#ifdef HAVE_INET_PTON
    int unixaf, ret;

    TRACE("family %d, addr %s, buffer (%p)\n", family, debugstr_a(addr), buffer);

    if (!addr || !buffer)
    {
        SetLastError(WSAEFAULT);
        return SOCKET_ERROR;
    }

    unixaf = convert_af_w2u(family);
    if (unixaf != AF_INET && unixaf != AF_INET6)
    {
        SetLastError(WSAEAFNOSUPPORT);
        return SOCKET_ERROR;
    }

    ret = inet_pton(unixaf, addr, buffer);
    if (ret == -1) SetLastError(wsaErrno());
    return ret;
#else
    FIXME( "not supported on this platform\n" );
    SetLastError( WSAEAFNOSUPPORT );
    return SOCKET_ERROR;
#endif
}