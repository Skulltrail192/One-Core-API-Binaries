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

#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>
#include <subauth.h>
#include <shlwapi.h>

HRESULT 
WINAPI
PathMatchSpecExA(
  _In_ LPCTSTR pszFile,
  _In_ LPCTSTR pszSpec,
  _In_ DWORD   dwFlags
)
{
	return PathMatchSpecA(pszFile, pszSpec);
}

HRESULT 
WINAPI
PathMatchSpecExW(
  _In_ LPCWSTR pszFile,
  _In_ LPCWSTR pszSpec,
  _In_ DWORD   dwFlags
)
{
	return PathMatchSpecW(pszFile, pszSpec);
}

HRESULT 
WINAPI 
PathCreateFromUrlAlloc(
	LPCWSTR pszUrl, 
	LPWSTR *pszPath,
    DWORD dwReserved
)
{
    WCHAR pathW[MAX_PATH];
    DWORD size;
    HRESULT hr;

    size = MAX_PATH;
    hr = PathCreateFromUrlW(pszUrl, pathW, &size, dwReserved);
    if (SUCCEEDED(hr))
    {
        /* Yes, this is supposed to crash if pszPath is NULL */
        *pszPath = StrDupW(pathW);
    }
    return hr;
}