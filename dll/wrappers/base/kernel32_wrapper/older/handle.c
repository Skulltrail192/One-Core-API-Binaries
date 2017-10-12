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

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

BOOLEAN WINAPI InternalSortCloseHandle(PVOID Address)
{
  BOOLEAN result; // al@4

  if ( Address )
  {
    if ( *((PVOID *)Address + 69) != Address )
      InternalSortCloseHandle(*((PVOID *)Address + 69));
	#ifdef _M_IX86
		result = RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, Address);
    #elif defined(_M_AMD64)
		result = RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, Address);
    #endif
  }
  return result;
}

BOOLEAN WINAPI SortCloseHandle(PVOID address)
{
  return InternalSortCloseHandle(address);
}

BOOL WINAPI SetStdHandleEx(DWORD nStdHandle, HANDLE hHandle, BOOL verification)
{
	return SetStdHandle(nStdHandle, hHandle);
}