/*
 * propsys main
 *
 * Copyright 1997, 2002 Alexandre Julliard
 * Copyright 2008 James Hawkins
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

#include "propsys_private.h"

#include <rpcproxy.h>

typedef enum  { 
  SFBS_FLAGS_ROUND_TO_NEAREST_DISPLAYED_DIGIT     = 0x00000001,
  SFBS_FLAGS_TRUNCATE_UNDISPLAYED_DECIMAL_DIGITS  = 0x00000002
} SFBS_FLAGS;

HRESULT 
WINAPI 
PSFormatForDisplay(
  _In_   REFPROPERTYKEY propkey,
  _In_   REFPROPVARIANT propvar,
  _In_   PROPDESC_FORMAT_FLAGS pdfFlags,
  _Out_  LPWSTR pwszText,
  _In_   DWORD cchText
)
{
	return S_OK;
}

HRESULT 
WINAPI 
PSStrFormatByteSizeEx(
        ULONGLONG  ull,
        SFBS_FLAGS flags,
  _Out_ PWSTR      pszBuf,
        UINT       cchBuf
)
{
	return S_OK;	
}