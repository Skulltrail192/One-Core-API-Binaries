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

WINE_DEFAULT_DEBUG_CHANNEL(string);

/*********************************************************************
 *		_strxfrm_l (MSVCRT.@)
 */
MSVCRT_size_t CDECL _strxfrm_l( char *dest, const char *src,
        size_t len, _locale_t locale )
{
    MSVCRT_pthreadlocinfo locinfo;
    int ret;

    if(!MSVCRT_CHECK_PMT(src)) return INT_MAX;
    if(!MSVCRT_CHECK_PMT(dest || !len)) return INT_MAX;

    if(len > INT_MAX) {
        FIXME("len > INT_MAX not supported\n");
        len = INT_MAX;
    }

    if(!locale)
        locinfo = get_locinfo();
    else
        locinfo = (MSVCRT_pthreadlocinfo)locale->locinfo;

    if(!locinfo->lc_handle[MSVCRT_LC_COLLATE]) {
        strncpy(dest, src, len);
        return strlen(src);
    }

    ret = LCMapStringA(locinfo->lc_handle[MSVCRT_LC_COLLATE],
            LCMAP_SORTKEY, src, -1, NULL, 0);
    if(!ret) {
        if(len) dest[0] = 0;
        *MSVCRT__errno() = MSVCRT_EILSEQ;
        return INT_MAX;
    }
    if(!len) return ret-1;

    if(ret > len) {
        dest[0] = 0;
        *MSVCRT__errno() = MSVCRT_ERANGE;
        return ret-1;
    }

    return LCMapStringA(locinfo->lc_handle[MSVCRT_LC_COLLATE],
            LCMAP_SORTKEY, src, -1, dest, len) - 1;
}


/*********************************************************************
 *                  strnlen   (NTDLL.@)
 */
size_t __cdecl strnlen( const char *str, size_t len )
{
    const char *s = str;
    for (s = str; len && *s; s++, len--) ;
    return s - str;
}