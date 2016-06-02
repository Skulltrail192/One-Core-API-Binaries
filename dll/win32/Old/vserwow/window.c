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

BOOL WINAPI ChangeWindowMessageFilter(
  _In_  UINT message,
  _In_  DWORD dwFlag
)
{
	TRACE("UNIMPLEMENTED");
	return TRUE;
}

BOOL WINAPI UpdateWindowTransform(HWND hwnd, D3DMATRIX* pMatrix, DWORD unk) 
{
	TRACE("UNIMPLEMENTED");	
	return TRUE;
}

int WINAPI SetWindowRgnEx(HWND hWnd, HGDIOBJ hRgn, BOOL bRedraw)
{
  int obj = 0; // esi@1

  if ( obj )
    DeleteObject(hRgn);
  return obj;
}

int WINAPI GetWindowRgnEx(HWND hWnd, HGDIOBJ hRgn, BOOL bRedraw)
{
  int obj = 0; // esi@1
  return obj;
}

HWND WINAPI GhostWindowFromHungWindow (
    HWND hwndGhost
) 
{
	TRACE("UNIMPLEMENTED");
	return hwndGhost;
}

HWND WINAPI HungWindowFromGhostWindow(
    HWND hwndGhost
) 
{
	return hwndGhost;
}

BOOL WINAPI GetWindowMinimizeRect (
    HWND hwndToQuery,
    RECT* pRect
) 
{
	TRACE("UNIMPLEMENTED");
	return TRUE;
}

BOOL WINAPI IsWindowRedirectedForPrint(HWND windows)
{
	BOOL result; // eax@1
	TRACE("UNIMPLEMENTED");
	return result = FALSE;
}

HWND WINAPI WindowFromPhysicalPoint(
  _In_  POINT Point
)
{
	TRACE("UNIMPLEMENTED");
	return NULL;
}


BOOL WINAPI SetWindowCompositionAttribute(
    HWND hwnd,
    WINCOMPATTRDATA* pAttrData
) 
{
	TRACE("UNIMPLEMENTED");
	return TRUE;
}

BOOL WINAPI GetWindowCompositionAttribute (
    HWND hwnd,
    WINCOMPATTRDATA* pAttrData
) 
{
	TRACE("UNIMPLEMENTED");
	return TRUE;
}

HWND WINAPI FrostCrashedWindow(
    HWND hwndToReplace,
    HWND hwndErrorReportOwnerWnd
) 
{
	hwndToReplace = hwndErrorReportOwnerWnd;
	return hwndToReplace;
}

BOOL WINAPI ChangeWindowMessageFilterEx(
  _In_         HWND hWnd,
  _In_         UINT message,
  _In_         DWORD action,
  _Inout_opt_  PCHANGEFILTERSTRUCT pChangeFilterStruct
)
{
	TRACE("UNIMPLEMENTED");
	return TRUE;
}

BOOL WINAPI CalculatePopupWindowPosition(
  _In_      const POINT *anchorPoint,
  _In_      const SIZE *windowSize,
  _In_      UINT flags,
  _In_opt_  RECT *excludeRect,
  _Out_     RECT *popupWindowPosition
)
{
	TRACE("UNIMPLEMENTED");
	return TRUE;
}

BOOL WINAPI GetTopLevelWindow(HWND window)
{
	TRACE("UNIMPLEMENTED");	
	return TRUE;
}

DWORD WINAPI NotifyOverlayWindow(HWND a1, DWORD a2)
{
	TRACE("UNIMPLEMENTED. Need implement NtUserCallTwoParam");	
	return 0;
}

DWORD WINAPI RegisterGhostWindow(HWND a1, DWORD a2)
{
	TRACE("UNIMPLEMENTED. Need implement NtUserCallTwoParam");	
	return 0;
}

DWORD WINAPI RegisterFrostWindow(HWND a1, DWORD a2)
{
	TRACE("UNIMPLEMENTED. Need implement NtUserCallTwoParam");	
	return 0;
}


DWORD WINAPI GetWindowCompositionInfo(HWND a1, DWORD a2)
{
	TRACE("UNIMPLEMENTED");	
	return 0;
}

DWORD WINAPI CheckWindowThreadDesktop(HANDLE a1, HANDLE a2)
{
	TRACE("UNIMPLEMENTED");	
	return 0;
}

BOOL WINAPI SetWindowFeedbackSetting(
  _In_      HWND hwnd,
  _In_      FEEDBACK_TYPE feedback,
  _In_      DWORD dwFlags,
  _In_      UINT32 size,
  _In_opt_  const VOID *configuration
)
{
	TRACE("UNIMPLEMENTED");	
	return TRUE;
}

BOOL WINAPI IsTopLevelWindow (
  _In_  HWND hWnd
)
{
	TRACE("UNIMPLEMENTED");
	return FALSE;
}