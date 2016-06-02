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
#define NDEBUG 
 
#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vtdll);

NTSTATUS NTAPI RtlGetOSProductName(PUNICODE_STRING string, DWORD flag)
{
	string->Length = 0;
	string->MaximumLength = 400;
	string->Buffer = L"Microsoft Windows Longhorn XP Professional Version 2003";
	return 0x00000000;
}

BOOL WINAPI RtlGetProductInfo(
  _In_   DWORD dwOSMajorVersion,
  _In_   DWORD dwOSMinorVersion,
  _In_   DWORD dwSpMajorVersion,
  _In_   DWORD dwSpMinorVersion,
  _Out_  PDWORD pdwReturnedProductType
)
{
	*pdwReturnedProductType = 0x00000001;
	return TRUE;
}

NTSTATUS WINAPI NtQueryLicenseValue(
    PUNICODE_STRING Name,
    ULONG *Type,
    PVOID Buffer,
    ULONG Length,
    ULONG *DataLength)
{
    return STATUS_SUCCESS;	
}

void RtlGetNtVersionNumbers(
  LPDWORD major,
  LPDWORD minor,
  LPDWORD build
)
{
	*major = 6;
	*minor = 0;
	*build = 6000;
}