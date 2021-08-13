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

WINE_DEFAULT_DEBUG_CHANNEL(wcs);

/*********************************************************************
 *              _wcsxfrm_l (MSVCRT.@)
 */
MSVCRT_size_t CDECL _wcsxfrm_l(MSVCRT_wchar_t *dest, const MSVCRT_wchar_t *src,
        size_t len, _locale_t  locale)
{
    MSVCRT_pthreadlocinfo locinfo;
    int i, ret;

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
        wcsncpy(dest, src, len);
        return wcslen(src);
    }

    ret = LCMapStringW(locinfo->lc_handle[MSVCRT_LC_COLLATE],
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

    ret = LCMapStringW(locinfo->lc_handle[MSVCRT_LC_COLLATE],
            LCMAP_SORTKEY, src, -1, dest, len) - 1;
    for(i=ret; i>=0; i--)
        dest[i] = ((unsigned char*)dest)[i];
    return ret;
}

/******************************************************************
 *  wcsnlen (MSVCRT.@)
 */
MSVCRT_size_t CDECL wcsnlen(const MSVCRT_wchar_t *s, size_t maxlen)
{
    MSVCRT_size_t i;

    for (i = 0; i < maxlen; i++)
        if (!s[i]) break;
    return i;
}
