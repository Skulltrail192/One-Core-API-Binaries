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

#include <main.h>
#include <windef.h>
#include <pidl.h>
 
WINE_DEFAULT_DEBUG_CHANNEL(shell);

typedef enum _ASSOCCLASS{
	ASSOCCLASS_APP_KEY,
	ASSOCCLASS_CLSID_KEY,
	ASSOCCLASS_CLSID_STR,
	ASSOCCLASS_PROGID_KEY,
	ASSOCCLASS_SHELL_KEY,
	ASSOCCLASS_PROGID_STR,
	ASSOCCLASS_SYSTEM_STR,
	ASSOCCLASS_APP_STR,
	ASSOCCLASS_FOLDER,
	ASSOCCLASS_STAR,
	ASSOCCLASS_FIXED_PROGID_STR,
	ASSOCCLASS_PROTOCOL_STR,
}ASSOCCLASS;

typedef enum tagSCNRT_STATUS { 
  SCNRT_ENABLE   = 0,
  SCNRT_DISABLE  = 1
} SCNRT_STATUS;

typedef struct _ASSOCIATIONELEMENT {
  ASSOCCLASS ac;
  HKEY       hkClass;
  PCWSTR     pszClass;
} ASSOCIATIONELEMENT;

typedef enum _ASSOC_FILTER{
  ASSOC_FILTER_NONE,
  ASSOC_FILTER_RECOMMENDED
}ASSOC_FILTER;

static const GUID CLSID_Bind_Query = 
{0x000214e6, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

EXTERN_C HANDLE WINAPI IsEnabled()
{
	return NULL;
}

HRESULT 
WINAPI 
Shell_NotifyIconGetRect(
	const NOTIFYICONIDENTIFIER *identifier, 
	RECT *iconLocation
)
{
	return E_NOTIMPL;
}