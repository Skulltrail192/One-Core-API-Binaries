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

#include <pdh.h>
#include <pdhmsg.h>
//#include "winperf.h"

#include <wine/debug.h>
#include <wine/list.h>
#include <wine/unicode.h>
#include <ws2spi.h>

WINE_DEFAULT_DEBUG_CHANNEL(ws2_32);

typedef enum _WSC_PROVIDER_INFO_TYPE {
   ProviderInfoLspCategories,
   ProviderInfoAudit
} WSC_PROVIDER_INFO_TYPE ;

int WSCInstallProviderAndChains(
  LPGUID              lpProviderId,
  const LPWSTR        lpszProviderDllPath,
  const LPWSTR        lpszLspName,
  DWORD               dwServiceFlags,
  LPWSAPROTOCOL_INFOW lpProtocolInfoList,
  DWORD               dwNumberOfEntries,
  LPDWORD             lpdwCatalogEntryId,
  LPINT               lpErrno
)
{
	//Hack, i don't know if work
	return WSCInstallProvider(lpProviderId,
							  lpszProviderDllPath,
							  lpProtocolInfoList,
							  dwNumberOfEntries,
							  lpErrno);
}

int WSCSetProviderInfo(
  _In_  LPGUID                 lpProviderId,
  _In_  WSC_PROVIDER_INFO_TYPE InfoType,
  _In_  PBYTE                  Info,
  _In_  size_t                 InfoSize,
  _In_  DWORD                  Flags,
  _Out_ LPINT                  lpErrno
)
{
	//Only stub
	return ERROR_CALL_NOT_IMPLEMENTED;
}