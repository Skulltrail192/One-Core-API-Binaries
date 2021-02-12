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
#include <winsock2.h>

#define POLLHUP                    0x0002
#define WS_POLLHUP                 0x0002
#define WS_POLLNVAL                0x0004

#define POLLERR                    0x0001
#define POLLHUP                    0x0002
#define POLLNVAL                   0x0004
#define POLLWRNORM                 0x0010
#define POLLWRBAND                 0x0020
#define POLLRDNORM                 0x0100
#define POLLRDBAND                 0x0200
#define POLLPRI                    0x0400
#define POLLIN                     (POLLRDNORM|POLLRDBAND)
#define POLLOUT                    (POLLWRNORM)

#define WS_POLLERR                 0x0001
#define WS_POLLHUP                 0x0002
#define WS_POLLNVAL                0x0004
#define WS_POLLWRNORM              0x0010
#define WS_POLLWRBAND              0x0020
#define WS_POLLRDNORM              0x0100
#define WS_POLLRDBAND              0x0200
#define WS_POLLPRI                 0x0400
#define WS_POLLIN                  (WS_POLLRDNORM|WS_POLLRDBAND)
#define WS_POLLOUT                 (WS_POLLWRNORM)
#define WSA_NOT_ENOUGH_MEMORY      (ERROR_NOT_ENOUGH_MEMORY)

#define WS_INET6_ADDRSTRLEN     65

#define MAP_OPTION(opt) { WS_##opt, opt }

static const unsigned __int64 epoch = ((unsigned __int64) 116444736000000000ULL);

struct pollfd
{
     int fd;
     short events;
     short revents;
};

typedef struct /*WS(pollfd)*/
{
    SOCKET fd;
    SHORT events;
    SHORT revents;
} WSAPOLLFD;

int poll( struct pollfd *fds, unsigned int count, int timeout );

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

static const int ws_poll_map[][2] =
{
    MAP_OPTION( POLLERR ),
    MAP_OPTION( POLLHUP ),
    MAP_OPTION( POLLNVAL ),
    MAP_OPTION( POLLWRNORM ),
    MAP_OPTION( POLLWRBAND ),
    MAP_OPTION( POLLRDNORM ),
    { WS_POLLRDBAND, POLLPRI }
};

static int convert_poll_u2w(int events)
{
    int i, ret;
    for (i = ret = 0; events && i < sizeof(ws_poll_map) / sizeof(ws_poll_map[0]); i++)
    {
        if (ws_poll_map[i][1] & events)
        {
            ret |= ws_poll_map[i][0];
            events &= ~ws_poll_map[i][1];
        }
    }

    if (events)
        FIXME("Unsupported poll() flags 0x%x\n", events);
    return ret;
}

static int convert_poll_w2u(int events)
{
    int i, ret;
    for (i = ret = 0; events && i < sizeof(ws_poll_map) / sizeof(ws_poll_map[0]); i++)
    {
        if (ws_poll_map[i][0] & events)
        {
            ret |= ws_poll_map[i][1];
            events &= ~ws_poll_map[i][0];
        }
    }

    if (events)
        FIXME("Unsupported WSAPoll() flags 0x%x\n", events);
    return ret;
}

int
gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    FILETIME    file_time;
    SYSTEMTIME  system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.u.LowPart = file_time.dwLowDateTime;
    ularge.u.HighPart = file_time.dwHighDateTime;

    tp->tv_sec = (long) ((ularge.QuadPart - epoch) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);

    return 0;
}

static int do_poll(struct pollfd *pollfds, int count, int timeout)
{
    struct timeval tv1, tv2;
    int ret, torig = timeout;

    if (timeout > 0) gettimeofday( &tv1, 0 );

    while ((ret = poll( pollfds, count, timeout )) < 0)
    {
        if (errno != EINTR) break;
        if (timeout < 0) continue;
        if (timeout == 0) return 0;

        gettimeofday( &tv2, 0 );

        tv2.tv_sec  -= tv1.tv_sec;
        tv2.tv_usec -= tv1.tv_usec;
        if (tv2.tv_usec < 0)
        {
            tv2.tv_usec += 1000000;
            tv2.tv_sec  -= 1;
        }

        timeout = torig - (tv2.tv_sec * 1000) - (tv2.tv_usec + 999) / 1000;
        if (timeout <= 0) return 0;
    }
    return ret;
}

static inline int get_sock_fd( SOCKET s, DWORD access, unsigned int *options )
{
    int fd;
    //if (set_error( wine_server_handle_to_fd( SOCKET2HANDLE(s), access, &fd, options ) ))
        return -1;
    //return fd;
}

static inline void release_sock_fd( SOCKET s, int fd )
{
    //wine_server_release_fd( SOCKET2HANDLE(s), fd );
}

int poll( struct pollfd *fds, unsigned int count, int timeout )
{
	return 0;
}

/***********************************************************************
 *     WSAPoll
 */
int WINAPI WSAPoll(WSAPOLLFD *wfds, ULONG count, int timeout)
{
    int i, ret;
    struct pollfd *ufds;

    if (!count)
    {
        SetLastError(WSAEINVAL);
        return SOCKET_ERROR;
    }
    if (!wfds)
    {
        SetLastError(WSAEFAULT);
        return SOCKET_ERROR;
    }

    if (!(ufds = HeapAlloc(GetProcessHeap(), 0, count * sizeof(ufds[0]))))
    {
        SetLastError(WSAENOBUFS);
        return SOCKET_ERROR;
    }

    for (i = 0; i < count; i++)
    {
        ufds[i].fd = get_sock_fd(wfds[i].fd, 0, NULL);
        ufds[i].events = convert_poll_w2u(wfds[i].events);
        ufds[i].revents = 0;
    }

    ret = do_poll(ufds, count, timeout);

    for (i = 0; i < count; i++)
    {
        if (ufds[i].fd != -1)
        {
            release_sock_fd(wfds[i].fd, ufds[i].fd);
            if (ufds[i].revents & POLLHUP)
            {
                /* Check if the socket still exists */
                int fd = get_sock_fd(wfds[i].fd, 0, NULL);
                if (fd != -1)
                {
                    wfds[i].revents = WS_POLLHUP;
                    release_sock_fd(wfds[i].fd, fd);
                }
                else
                    wfds[i].revents = WS_POLLNVAL;
            }
            else
                wfds[i].revents = convert_poll_u2w(ufds[i].revents);
        }
        else
            wfds[i].revents = WS_POLLNVAL;
    }

    HeapFree(GetProcessHeap(), 0, ufds);
    return ret;
}

/***********************************************************************
*              InetPtonW                      (WS2_32.@)
*/
INT WINAPI InetPtonW(INT family, PCWSTR addr, PVOID buffer)
{
    char *addrA;
    int len;
    INT ret;

    TRACE("family %d, addr %s, buffer (%p)\n", family, debugstr_w(addr), buffer);

    if (!addr)
    {
        SetLastError(WSAEFAULT);
        return SOCKET_ERROR;
    }

    len = WideCharToMultiByte(CP_ACP, 0, addr, -1, NULL, 0, NULL, NULL);
    if (!(addrA = HeapAlloc(GetProcessHeap(), 0, len)))
    {
        SetLastError(WSA_NOT_ENOUGH_MEMORY);
        return SOCKET_ERROR;
    }
    WideCharToMultiByte(CP_ACP, 0, addr, -1, addrA, len, NULL, NULL);

    ret = WS_inet_pton(family, addrA, buffer);

    HeapFree(GetProcessHeap(), 0, addrA);
    return ret;
}

/***********************************************************************
 *              InetNtopW                      (WS2_32.@)
 */
PCWSTR WINAPI InetNtopW(INT family, PVOID addr, PWSTR buffer, SIZE_T len)
{
    char bufferA[WS_INET6_ADDRSTRLEN];
    PWSTR ret = NULL;

    TRACE("family %d, addr (%p), buffer (%p), len %ld\n", family, addr, buffer, len);

    if (WS_inet_ntop(family, addr, bufferA, sizeof(bufferA)))
    {
        if (MultiByteToWideChar(CP_ACP, 0, bufferA, -1, buffer, len))
            ret = buffer;
        else
            SetLastError(ERROR_INVALID_PARAMETER);
    }
    return ret;
}