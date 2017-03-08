 /*
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
 
#define NDEBUG

#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

/*
* @unimplemented - need implementation
*/
NTSTATUS 
WINAPI 
GetOSProductNameW(
	PCWSTR Source, 
	ULONG var, 
	ULONG parameter
)
{
	Source = L"Microsoft Windows Codename \"Longhorn\" Professional Version 2003 Copyright ";
	return STATUS_SUCCESS;
}

/*
* @unimplemented - need implementation
*/
NTSTATUS 
WINAPI 
GetOSProductNameA(
	PCSTR Source, 
	ULONG var, 
	ULONG parameter
)
{
	Source = "Microsoft Windows Codename \"Longhorn\" Professional Version 2003 Copyright ";
	return STATUS_SUCCESS;
}

/*
 * @implemented - new
 */
BOOL 
WINAPI 
GetProductInfo(
  _In_   DWORD dwOSMajorVersion,
  _In_   DWORD dwOSMinorVersion,
  _In_   DWORD dwSpMajorVersion,
  _In_   DWORD dwSpMinorVersion,
  _Out_  PDWORD pdwReturnedProductType
)
{
	return RtlGetProductInfo(dwOSMajorVersion, 
							 dwOSMinorVersion,
                             dwSpMajorVersion, 
							 dwSpMinorVersion, 
							 pdwReturnedProductType);
}