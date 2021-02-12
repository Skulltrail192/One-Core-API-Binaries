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

static DPI_AWARENESS dpi_awareness;

WINE_DEFAULT_DEBUG_CHANNEL(process); 

BOOL WINAPI SetProcessRestrictionExemption(BOOL verification)
{
  BOOL resp; // ebx@1
  PVOID *other; // eax@2

  resp = 1;
  
  EnterCriticalSection(lpCriticalSection);
  if ( verification )
  {
    other = GetCurrentProcess();
    other[106] = (PVOID)(((unsigned int)other[106] ^ (verification << 8)) & 0x100 ^ (unsigned int)other[106]);
  }
  else
  {
    resp = 0;
    SetLastError(5);
  }
  LeaveCriticalSection(lpCriticalSection);
  return resp;
}


/***********************************************************************
 *              SetProcessDPIAware   (USER32.@)
 */
BOOL WINAPI SetProcessDPIAware(void)
{
    DbgPrint("SetProcessDPIAware called\n");
    InterlockedCompareExchange( &dpi_awareness, 0x11, 0 );
    return TRUE;
}

BOOL WINAPI IsProcessDPIAware()
{
	return gfdDPIProcess;
}

/**********************************************************************
 *              EnableNonClientDpiScaling   (USER32.@)
 */
BOOL WINAPI EnableNonClientDpiScaling( HWND hwnd )
{
    FIXME("(%p): stub\n", hwnd);
    SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
    return FALSE;
}
