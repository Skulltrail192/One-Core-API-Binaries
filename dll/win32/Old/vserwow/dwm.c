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

WINE_DEFAULT_DEBUG_CHANNEL(vser32);

HANDLE gpepDwm;
LPCRITICAL_SECTION critical;
HANDLE gDwmApiPort;
int IsComposited = 0;

DWORD WINAPI DwmGetRedirSurfaceUpdateId(HANDLE h1, BOOL ver, DWORD flags)
{
	TRACE("UNIMPLEMENTED");	
	return 0;
}

BOOL WINAPI DwmGetRedirSurfacePresentFlags(HANDLE a1, HANDLE a2)
{
	TRACE("UNIMPLEMENTED");	
	a2 = a1;
	return TRUE;
}

BOOL WINAPI DwmSetRedirSurfacePresentFlags(HANDLE a1, BOOL a2)
{
	TRACE("UNIMPLEMENTED");	
	return TRUE;
}

BOOL WINAPI DwmShutdown()
{
	TRACE("UNIMPLEMENTED");	
	return TRUE;
}

NTSTATUS WINAPI DwmStartupClient()
{
	return 1;
}

BOOL WINAPI DwmStartup(HANDLE Object)
{
  ULONG error; // eax@14
  BOOL resp; // esi@1
  NTSTATUS status; // ebx@2

  resp = FALSE;
  if ( gpepDwm )
  {
    SetLastError(5);
  }
  //status = ObReferenceObjectByHandle(Object, 0, (POBJECT_TYPE)LpcPortObjectType, 1, &Object, 0);
  gDwmApiPort = Object;
  gpepDwm = GetCurrentProcess();
  status = DwmStartupClient();
  if ( status >= 0 )
  {
    resp = TRUE;
  }
  if ( gDwmApiPort )
     gDwmApiPort = NULL;
  gpepDwm = 0;
  if ( RtlNtStatusToDosError(status) )
  {
    error = RtlNtStatusToDosError(status);
    SetLastError(error);
  }
  return resp; 
}

BOOL WINAPI DwmGetSurfaceData(HANDLE h1, HANDLE h2)
{
	TRACE("UNIMPLEMENTED");	
	return TRUE;
}

int WINAPI DCEQueryMode()
{
	return 0;
}

int WINAPI StartDCE()
{
	return 0;
}

int WINAPI StopDCE()
{
	return 0;
}

DWORD WINAPI DwmStartRedirection(HWND a1)
{
	TRACE("UNIMPLEMENTED");	
	return 0;
}

DWORD WINAPI DwmStopRedirection()
{
	TRACE("UNIMPLEMENTED");	
	return 0;
}
