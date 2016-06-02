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
#include <ntsecapi.h>
#include <normalization.h>

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    TRACE("fdwReason %u\n", fdwReason);

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}


int
WINAPI NormalizeString( __in                      NORM_FORM NormForm,
                        __in_ecount(cwSrcLength)  LPCWSTR   lpSrcString,
                        __in                      int       cwSrcLength,
                        __out_ecount(cwDstLength) LPWSTR    lpDstString,
                        __in                      int       cwDstLength )
{
	return 0;
}

BOOL
WINAPI IsNormalizedString( __in                   NORM_FORM NormForm,
                           __in_ecount(cwLength)  LPCWSTR   lpString,
                           __in                   int       cwLength )
{
	return TRUE;
}
						   

//
// IDN (International Domain Name) Functions
//
int
WINAPI IdnToAscii(__in                        DWORD    dwFlags,
                  __in_ecount(cchUnicodeChar) LPCWSTR  lpUnicodeCharStr,
                  __in                        int      cchUnicodeChar,
                  __out_ecount(cchASCIIChar)  LPWSTR   lpASCIICharStr,
                  __in                        int      cchASCIIChar)
{
	return 0;
}


int
WINAPI IdnToNameprepUnicode(__in                            DWORD   dwFlags,
                            __in_ecount(cchUnicodeChar)     LPCWSTR lpUnicodeCharStr,
                            __in                            int     cchUnicodeChar,
                            __out_ecount(cchNameprepChar)   LPWSTR  lpNameprepCharStr,
                            __in                            int     cchNameprepChar)
{
	return 0;
}
							

int
WINAPI IdnToUnicode(__in                         DWORD   dwFlags,
                    __in_ecount(cchASCIIChar)    LPCWSTR lpASCIICharStr,
                    __in                         int     cchASCIIChar,
                    __out_ecount(cchUnicodeChar) LPWSTR  lpUnicodeCharStr,
                    __in                         int     cchUnicodeChar)
{
	return 0;
}
					
