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


BOOL WINAPI GetDisplayAutoRotationPreferences(
  _Out_  ORIENTATION_PREFERENCE *pOrientation
)
{
	*pOrientation = Orientation;
	TRACE("UNIMPLEMENTED");	
	return TRUE;
}

BOOL WINAPI SetDisplayAutoRotationPreferences(
  _In_  ORIENTATION_PREFERENCE pOrientation
)
{
	Orientation = pOrientation;
	TRACE("UNIMPLEMENTED");	
	return TRUE;
}

//aqui
LONG WINAPI DisplayConfigGetDeviceInfo(
  _Inout_  DISPLAYCONFIG_DEVICE_INFO_HEADER *requestPacket
)
{
	TRACE("UNIMPLEMENTED");
	return ERROR_SUCCESS;
}

LONG WINAPI DisplayConfigSetDeviceInfo(
  _Inout_  DISPLAYCONFIG_DEVICE_INFO_HEADER *setPacket
)
{
	TRACE("UNIMPLEMENTED");
	return ERROR_SUCCESS;
}

LONG WINAPI GetDisplayConfigBufferSizes(
  _In_   UINT32 Flags,
  _Out_  UINT32 *pNumPathArrayElements,
  _Out_  UINT32 *pNumModeInfoArrayElements
)
{
	pNumPathArrayElements = 0;
	pNumModeInfoArrayElements = 0;
	TRACE("UNIMPLEMENTED");
	return ERROR_SUCCESS;
}


LONG WINAPI QueryDisplayConfig(
  _In_       UINT32 Flags,
  _Inout_    UINT32 *pNumPathArrayElements,
  _Out_      DISPLAYCONFIG_PATH_INFO *pPathInfoArray,
  _Inout_    UINT32 *pNumModeInfoArrayElements,
  _Out_      DISPLAYCONFIG_MODE_INFO *pModeInfoArray,
  _Out_opt_  DISPLAYCONFIG_TOPOLOGY_ID *pCurrentTopologyId
)
{
	TRACE("UNIMPLEMENTED");
	return ERROR_SUCCESS;
}

LONG WINAPI SetDisplayConfig(
  _In_      UINT32 numPathArrayElements,
  _In_opt_  DISPLAYCONFIG_PATH_INFO *pathArray,
  _In_      UINT32 numModeInfoArrayElements,
  _In_opt_  DISPLAYCONFIG_MODE_INFO *modeInfoArray,
  _In_      UINT32 Flags
)
{
	TRACE("UNIMPLEMENTED");
	return ERROR_SUCCESS;
}

BOOL WINAPI GetWindowDisplayAffinity(
  _In_   HWND hWnd,
  _Out_  DWORD *dwAffinity
)
{
	TRACE("UNIMPLEMENTED");
	return TRUE;
}

BOOL WINAPI SetWindowDisplayAffinity(
  _In_  HWND hWnd,
  _In_  DWORD dwAffinity
)
{
	TRACE("UNIMPLEMENTED");
	return TRUE;
}