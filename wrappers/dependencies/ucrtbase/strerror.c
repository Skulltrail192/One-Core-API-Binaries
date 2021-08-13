/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
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
 *
 */
#include <ucrtbase.h>

WINE_DEFAULT_DEBUG_CHANNEL(strerror);

/**********************************************************************
 *		__wcserror_s	(MSVCRT.@)
 */
int CDECL __wcserror_s(wchar_t* buffer, size_t nc, const wchar_t* str)
{
    int err;
    static const WCHAR colonW[] = {':', ' ', '\0'};
    static const WCHAR nlW[] = {'\n', '\0'};
    size_t len;

    err = *_errno();
    if (err < 0 || err > _sys_nerr) err = _sys_nerr;

    len = MultiByteToWideChar(CP_ACP, 0, _sys_errlist[err], -1, NULL, 0) + 1 /* \n */;
    if (str && *str) len += lstrlenW(str) + 2 /* ': ' */;
    if (len > nc)
    {
        MSVCRT_INVALID_PMT("buffer[nc] is too small", ERANGE);
        return ERANGE;
    }
    if (str && *str)
    {
        lstrcpyW(buffer, str);
        lstrcatW(buffer, colonW);
    }
    else buffer[0] = '\0';
    len = lstrlenW(buffer);
    MultiByteToWideChar(CP_ACP, 0, _sys_errlist[err], -1, buffer + len, 256 - len);
    lstrcatW(buffer, nlW);

    return 0;
}