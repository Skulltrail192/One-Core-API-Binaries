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

#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <windef.h>
#include <winbase.h>
#include <wingdi.h>
#include <winuser.h>
#include <winnls.h>
#include <windowsx.h>
#include <undocuser.h>
#include <uxtheme.h>

typedef int (WINAPI *DTT_CALLBACK_PROC)( HDC, LPWSTR, int, LPRECT, UINT, LPARAM );

HANDLE hHeap = 0;

//unimplemented
typedef struct _DTTOPTS {
  DWORD             dwSize;
  DWORD             dwFlags;
  COLORREF          crText;
  COLORREF          crBorder;
  COLORREF          crShadow;
  int               iTextShadowType;
  POINT             ptShadowOffset;
  int               iBorderSize;
  int               iFontPropId;
  int               iColorPropId;
  int               iStateId;
  BOOL              fApplyOverlay;
  int               iGlowSize;
  DTT_CALLBACK_PROC pfnDrawTextCallback;
  LPARAM            lParam;
} DTTOPTS, *PDTTOPTS;

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    TRACE("fdwReason %u\n", fdwReason);

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}

//unimplemented
HRESULT WINAPI GetThemeTransitionDuration(
  HTHEME hTheme,
  int iPartId,
  int iStateIdFrom,
  int iStateIdTo,
  int iPropId,
  _Out_  DWORD *pdwDuration
)
{
	return S_OK;
}

//unimplemented
HRESULT 
WINAPI 
DrawThemeTextEx(
  _In_     HTHEME hTheme,
  _In_     HDC hdc,
  _In_     int iPartId,
  _In_     int iStateId,
  _In_     LPCWSTR pszText,
  _In_     int iCharCount,
  _In_     DWORD dwFlags,
  _Inout_  LPRECT pRect,
  _In_     const DTTOPTS *pOptions
)
{
	return DrawThemeText(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwFlags, 0, pRect);
}

//unimplemented
HRESULT WINAPI SetWindowThemeAttribute(
  _In_  HWND hwnd,
  _In_  enum WINDOWTHEMEATTRIBUTETYPE eAttribute,
  _In_  PVOID pvAttribute,
  _In_  DWORD cbAttribute
)
{
	return S_OK;
}

//unimplemented
HRESULT WINAPI GetThemeBitmap(
  _In_   HTHEME hTheme,
  _In_   int iPartId,
  _In_   int iStateId,
  _In_   int iPropId,
  _In_   ULONG dwFlags,
  _Out_  HBITMAP *phBitmap
)
{
	return S_OK;
}

/*HRESULT WINAPI DrawThemeBackgroundTransition(HTHEME hTheme, 
											 HDC hDC, 
											 int iPartId, 
											 int iStateId, 
											 int ftn, 
											 ULONG a6, 
											 ULONG a7, 
											 LPCRECT pRect, 
											 const DTBGOPTS *pOptions)
*/

HRESULT WINAPI FreeThemePropertyValues(LPVOID lpMem)
{
  DWORD error; // eax@2
  int result; // eax@2

  if ( HeapFree(hHeap, 0, lpMem))
  {
    result = 0;
  }
  else
  {
    error = GetLastError();
    result = 0x80070000;
  }
  return result;
}

//unimplemented
HRESULT WINAPI FreeThemeSymbols(LPVOID lpMem)
{
  HeapFree(hHeap, 0, lpMem);
  return S_OK;
}

//unimplemented
HRESULT WINAPI BufferedPaintUnInit(void)
{
	return E_FAIL;
}

//unimplemented
HRESULT WINAPI BufferedPaintInit()
{
	return E_FAIL;
}

//unimplemented
HRESULT WINAPI EndBufferedPaint(
  HPAINTBUFFER hBufferedPaint,
  BOOL fUpdateTarget
)
{
	return E_FAIL;
}

//unimplemented
HRESULT WINAPI BufferedPaintClear(
  HPAINTBUFFER hBufferedPaint,
  _In_  const RECT *prc
)
{
	return E_FAIL;
}

//unimplemented
HRESULT WINAPI GetBufferedPaintBits(
  HPAINTBUFFER hBufferedPaint,
  _Out_  RGBQUAD **ppbBuffer,
  _Out_  int *pcxRow
)
{
	return E_FAIL;
}

//unimplemented
HPAINTBUFFER WINAPI BeginBufferedPaint(
  HDC hdcTarget,
  const RECT *prcTarget,
  BP_BUFFERFORMAT dwFormat,
  _In_   BP_PAINTPARAMS *pPaintParams,
  _Out_  HDC *phdc
)
{
	return NULL;
}

HRESULT 
WINAPI 
CheckThemeSignature(LPCWSTR  pszThemeFileName) 	
{
    return S_OK;
}

/***********************************************************************
*      BufferedPaintSetAlpha                              (UXTHEME.@)
*/
HRESULT WINAPI BufferedPaintSetAlpha(HPAINTBUFFER hBufferedPaint, const RECT *prc, BYTE alpha)
{
    FIXME("Stub (%p %p %u)\n", hBufferedPaint, prc, alpha);
	return E_NOTIMPL;
}

