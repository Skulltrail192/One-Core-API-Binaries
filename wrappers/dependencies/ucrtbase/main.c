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

#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>
#include <winternl.h>

char* CDECL strupr_l(char *str, HANDLE locale)
{
    return str;
}


/*********************************************************************
 *              _towlower_l (MSVCRT.@)
 */
int CDECL MSVCRT__towlower_l(wint_t c, HANDLE locale)
{
    return 0;
}

/*********************************************************************
 *              _towupper_l (MSVCRT.@)
 */
int CDECL MSVCRT__towupper_l(wint_t c, HANDLE locale)
{
    return 0;
}

/******************************************************************
 *		_wcslwr_l (MSVCRT.@)
 */
wchar_t* CDECL MSVCRT__wcslwr_l(wchar_t* str, _locale_t locale )
{
    return str;
}

/******************************************************************
 *              _wcsupr_l (MSVCRT.@)
 */
wchar_t* CDECL MSVCRT__wcsupr_l(wchar_t *str, _locale_t locale )
{
    return str;
}

/*********************************************************************
 *		_abs64 (MSVCRT.@)
 */
__int64 CDECL abs64( __int64 n )
{
    return n >= 0 ? n : -n;
}

/*********************************************************************
 * _byteswap_uint64 (MSVCR100.@)
 */
unsigned __int64 CDECL byteswap_uint64(unsigned __int64 i)
{
    return (i<<56) + ((i&0xFF00)<<40) + ((i&0xFF0000)<<24) + ((i&0xFF000000)<<8) +
        ((i>>8)&0xFF000000) + ((i>>24)&0xFF0000) + ((i>>40)&0xFF00) + (i>>56);
}

/*********************************************************************
 * _byteswap_ulong (MSVCR100.@)
 */
ULONG CDECL byteswap_ulong(ULONG l)
{
    return (l<<24) + ((l<<8)&0xFF0000) + ((l>>8)&0xFF00) + (l>>24);
}

/*********************************************************************
 * _byteswap_ushort (MSVCR100.@)
 */
unsigned short CDECL byteswap_ushort(unsigned short s)
{
    return (s<<8) + (s>>8);
}

/*********************************************************************
 *		_rotl64 (MSVCRT.@)
 */
unsigned __int64 CDECL rotl64(unsigned __int64 num, int shift)
{
  shift &= 63;
  return (num << shift) | (num >> (64-shift));
}

/*********************************************************************
 *		_rotr64 (MSVCRT.@)
 */
unsigned __int64 CDECL rotr64(unsigned __int64 num, int shift)
{
    shift &= 63;
    return (num >> shift) | (num << (64-shift));
}