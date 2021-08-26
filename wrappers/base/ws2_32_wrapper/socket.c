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
 
#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(socket);

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
gettimeofday(PTIMEVAL tp, struct timezone * tzp)
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
    //int fd;
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

static ADDRINFOEXW *addrinfo_AtoW(const PADDRINFOA ai)
{
    ADDRINFOEXW *ret;

    if (!(ret = HeapAlloc(GetProcessHeap(), 0, sizeof(ADDRINFOEXW)))) return NULL;
    ret->ai_flags     = ai->ai_flags;
    ret->ai_family    = ai->ai_family;
    ret->ai_socktype  = ai->ai_socktype;
    ret->ai_protocol  = ai->ai_protocol;
    ret->ai_addrlen   = ai->ai_addrlen;
    ret->ai_canonname = NULL;
    ret->ai_addr      = NULL;
    ret->ai_blob      = NULL;
    ret->ai_bloblen   = 0;
    ret->ai_provider  = NULL;
    ret->ai_next      = NULL;
    if (ai->ai_canonname)
    {
        int len = MultiByteToWideChar(CP_ACP, 0, ai->ai_canonname, -1, NULL, 0);
        if (!(ret->ai_canonname = HeapAlloc(GetProcessHeap(), 0, len*sizeof(WCHAR))))
        {
            HeapFree(GetProcessHeap(), 0, ret);
            return NULL;
        }
        MultiByteToWideChar(CP_ACP, 0, ai->ai_canonname, -1, ret->ai_canonname, len);
    }
    if (ai->ai_addr)
    {
        if (!(ret->ai_addr = HeapAlloc(GetProcessHeap(), 0, ai->ai_addrlen)))
        {
            HeapFree(GetProcessHeap(), 0, ret->ai_canonname);
            HeapFree(GetProcessHeap(), 0, ret);
            return NULL;
        }
        memcpy(ret->ai_addr, ai->ai_addr, ai->ai_addrlen);
    }
    return ret;
}

static ADDRINFOEXW *addrinfo_list_AtoW(PADDRINFOA info)
{
    ADDRINFOEXW *ret, *infoW;

    if (!(ret = infoW = addrinfo_AtoW(info))) return NULL;
    while (info->ai_next)
    {
        if (!(infoW->ai_next = addrinfo_AtoW(info->ai_next)))
        {
            FreeAddrInfoExW(ret);
            return NULL;
        }
        infoW = infoW->ai_next;
        info = info->ai_next;
    }
    return ret;
}

static void WINAPI getaddrinfo_callback(TP_CALLBACK_INSTANCE *instance, void *context)
{
    struct getaddrinfo_args *args = context;
    OVERLAPPED *overlapped = args->overlapped;
    HANDLE event = overlapped->hEvent;
    LPLOOKUPSERVICE_COMPLETION_ROUTINE completion_routine = args->completion_routine;
    PADDRINFOA res;
    int ret;

    ret = getaddrinfo(args->nodename, args->servname, NULL, &res);
    if (res)
    {
        *args->result = addrinfo_list_AtoW(res);
        overlapped->u.Pointer = args->result;
        freeaddrinfo(res);
    }

    HeapFree(GetProcessHeap(), 0, args->nodename);
    HeapFree(GetProcessHeap(), 0, args->servname);
    HeapFree(GetProcessHeap(), 0, args);

    overlapped->Internal = ret;
    if (completion_routine) completion_routine(ret, 0, (LPWSAOVERLAPPED)overlapped);
    if (event) SetEvent(event);
}

static int WS_getaddrinfoW(const WCHAR *nodename, const WCHAR *servname, const PADDRINFOA hints, ADDRINFOEXW **res,
                           OVERLAPPED *overlapped, LPLOOKUPSERVICE_COMPLETION_ROUTINE completion_routine)
{
    int ret = EAI_MEMORY, len, i;
    char *nodenameA = NULL, *servnameA = NULL;
    PADDRINFOA resA;
    WCHAR *local_nodenameW = (WCHAR *)nodename;

    *res = NULL;
    if (nodename)
    {
        /* Is this an IDN? Most likely if any char is above the Ascii table, this
         * is the simplest validation possible, further validation will be done by
         * the native getaddrinfo() */
        for (i = 0; nodename[i]; i++)
        {
            if (nodename[i] > 'z')
                break;
        }
        if (nodename[i])
        {
            if (hints && (hints->ai_flags & WS_AI_DISABLE_IDN_ENCODING))
            {
                /* Name requires conversion but it was disabled */
                ret = WSAHOST_NOT_FOUND;
                WSASetLastError(ret);
                goto end;
            }

            len = IdnToAscii(0, nodename, -1, NULL, 0);
            if (!len)
            {
                ERR("Failed to convert %s to punycode\n", debugstr_w(nodename));
                ret = EAI_FAIL;
                goto end;
            }
            if (!(local_nodenameW = HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR)))) goto end;
            IdnToAscii(0, nodename, -1, local_nodenameW, len);
        }
    }
    if (local_nodenameW)
    {
        len = WideCharToMultiByte(CP_ACP, 0, local_nodenameW, -1, NULL, 0, NULL, NULL);
        if (!(nodenameA = HeapAlloc(GetProcessHeap(), 0, len))) goto end;
        WideCharToMultiByte(CP_ACP, 0, local_nodenameW, -1, nodenameA, len, NULL, NULL);
    }
    if (servname)
    {
        len = WideCharToMultiByte(CP_ACP, 0, servname, -1, NULL, 0, NULL, NULL);
        if (!(servnameA = HeapAlloc(GetProcessHeap(), 0, len))) goto end;
        WideCharToMultiByte(CP_ACP, 0, servname, -1, servnameA, len, NULL, NULL);
    }

    if (overlapped)
    {
        struct getaddrinfo_args *args;

        if (overlapped->hEvent && completion_routine)
        {
            ret = WSAEINVAL;
            goto end;
        }

        if (!(args = HeapAlloc(GetProcessHeap(), 0, sizeof(*args)))) goto end;
        args->overlapped = overlapped;
        args->completion_routine = completion_routine;
        args->result = res;
        args->nodename = nodenameA;
        args->servname = servnameA;

        overlapped->Internal = WSAEINPROGRESS;
        if (!TrySubmitThreadpoolCallback(getaddrinfo_callback, args, NULL))
        {
            HeapFree(GetProcessHeap(), 0, args);
            ret = GetLastError();
            goto end;
        }

        if (local_nodenameW != nodename)
            HeapFree(GetProcessHeap(), 0, local_nodenameW);
        WSASetLastError(ERROR_IO_PENDING);
        return ERROR_IO_PENDING;
    }

    ret = getaddrinfo(nodenameA, servnameA, hints, &resA);
    if (!ret)
    {
        *res = addrinfo_list_AtoW(resA);
        freeaddrinfo(resA);
    }

end:
    if (local_nodenameW != nodename)
        HeapFree(GetProcessHeap(), 0, local_nodenameW);
    HeapFree(GetProcessHeap(), 0, nodenameA);
    HeapFree(GetProcessHeap(), 0, servnameA);
    return ret;
}

/***********************************************************************
 *		GetAddrInfoExW		(WS2_32.@)
 */
int WINAPI GetAddrInfoExW(const WCHAR *name, const WCHAR *servname, DWORD namespace, GUID *namespace_id,
        const ADDRINFOEXW *hints, ADDRINFOEXW **result, struct WS_timeval *timeout, OVERLAPPED *overlapped,
        LPLOOKUPSERVICE_COMPLETION_ROUTINE completion_routine, HANDLE *handle)
{
    int ret;

    TRACE("(%s %s %x %s %p %p %p %p %p %p)\n", debugstr_w(name), debugstr_w(servname), namespace,
          debugstr_guid(namespace_id), hints, result, timeout, overlapped, completion_routine, handle);

    if (namespace != NS_DNS)
        FIXME("Unsupported namespace %u\n", namespace);
    if (namespace_id)
        FIXME("Unsupported naemspace_id %s\n", debugstr_guid(namespace_id));
    if (hints)
        FIXME("Unsupported hints\n");
    if (timeout)
        FIXME("Unsupported timeout\n");
    if (handle)
        FIXME("Unsupported cancel handle\n");

    ret = WS_getaddrinfoW(name, servname, NULL, result, overlapped, completion_routine);
    if (ret) return ret;
    if (handle) *handle = (HANDLE)0xdeadbeef;
    return 0;
}

/***********************************************************************
 *      FreeAddrInfoExW      (WS2_32.@)
 */
void WINAPI FreeAddrInfoExW(ADDRINFOEXW *ai)
{
    TRACE("(%p)\n", ai);

    while (ai)
    {
        ADDRINFOEXW *next;
        HeapFree(GetProcessHeap(), 0, ai->ai_canonname);
        HeapFree(GetProcessHeap(), 0, ai->ai_addr);
        next = ai->ai_next;
        HeapFree(GetProcessHeap(), 0, ai);
        ai = next;
    }
}

/***********************************************************************
 *              WSCGetProviderInfo
 */
INT WINAPI WSCGetProviderInfo( LPGUID provider, WSC_PROVIDER_INFO_TYPE info_type,
                               PBYTE info, size_t* len, DWORD flags, LPINT errcode )
{
    FIXME( "(%s 0x%08x %p %p 0x%08x %p) Stub!\n",
           debugstr_guid(provider), info_type, info, len, flags, errcode );

    if (!errcode)
        return SOCKET_ERROR;

    if (!provider) {
        *errcode = WSAEFAULT;
        return SOCKET_ERROR;
    }

    *errcode = WSANO_RECOVERY;
    return SOCKET_ERROR;
}

/***********************************************************************
 *      FreeAddrInfoEx      (WS2_32.@)
 */
void WINAPI FreeAddrInfoEx(ADDRINFOEXA *ai)
{
    TRACE("(%p)\n", ai);

    while (ai)
    {
        ADDRINFOEXA *next;
        HeapFree(GetProcessHeap(), 0, ai->ai_canonname);
        HeapFree(GetProcessHeap(), 0, ai->ai_addr);
        next = ai->ai_next;
        HeapFree(GetProcessHeap(), 0, ai);
        ai = next;
    }
}

/***********************************************************************
 *		GetAddrInfoExOverlappedResult  (WS2_32.@)
 */
int WINAPI GetAddrInfoExOverlappedResult(OVERLAPPED *overlapped)
{
    TRACE("(%p)\n", overlapped);
    return overlapped->Internal;
}

/***********************************************************************
 *		GetAddrInfoExCancel     (WS2_32.@)
 */
int WINAPI GetAddrInfoExCancel(HANDLE *handle)
{
    FIXME("(%p)\n", handle);
    return WSA_INVALID_HANDLE;
}
