/*
 * Dwmapi
 *
 * Copyright 2007 Andras Kovacs
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

#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H

#include <config.h>
#include <stdarg.h>

#define NONAMELESSUNION
#define NONAMELESSSTRUCT
#define COBJMACROS
#include <windef.h>
#include <winbase.h>
#include <wingdi.h>
#include <winuser.h>
#include <dwmapi.h>
#include <wine/debug.h>

WINE_DEFAULT_DEBUG_CHANNEL(dwmapi);

typedef enum _DWM_SOURCE_FRAME_SAMPLING { 
  DWM_SOURCE_FRAME_SAMPLING_POINT     = 1,
  DWM_SOURCE_FRAME_SAMPLING_COVERAGE,
  DWM_SOURCE_FRAME_SAMPLING_LAST
} DWM_SOURCE_FRAME_SAMPLING;

typedef struct _DWM_THUMBNAIL_PROPERTIES {
  DWORD dwFlags;
  RECT  rcDestination;
  RECT  rcSource;
  BYTE  opacity;
  BOOL  fVisible;
  BOOL  fSourceClientAreaOnly;
} DWM_THUMBNAIL_PROPERTIES, *PDWM_THUMBNAIL_PROPERTIES;

typedef struct _DWM_PRESENT_PARAMETERS {
  UINT32                    cbSize;
  BOOL                      fQueue;
  DWM_FRAME_COUNT           cRefreshStart;
  UINT                      cBuffer;
  BOOL                      fUseSourceRate;
  UNSIGNED_RATIO            rateSource;
  UINT                      cRefreshesPerFrame;
  DWM_SOURCE_FRAME_SAMPLING eSampling;
} DWM_PRESENT_PARAMETERS;

/* At process attach */
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    switch(fdwReason)
    {
    case DLL_WINE_PREATTACH:
        return FALSE;  /* prefer native version */
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls( hInstDLL );
        break;
    }
    return TRUE;
}

/**********************************************************************
 *           DwmIsCompositionEnabled         (DWMAPI.@)
 */
HRESULT WINAPI DwmIsCompositionEnabled(BOOL *enabled)
{
    static int once;
    if (!once)
    {
        FIXME("%p\n", enabled);
        once = 1;
    }
    else
        TRACE("%p\n", enabled);

    *enabled = FALSE;
    return S_OK;
}

/**********************************************************************
 *           DwmEnableComposition         (DWMAPI.102)
 */
HRESULT WINAPI DwmEnableComposition(UINT uCompositionAction)
{
    FIXME("(%d) stub\n", uCompositionAction);

    return S_OK;
}

/**********************************************************************
 *           DwmExtendFrameIntoClientArea    (DWMAPI.@)
 */
HRESULT WINAPI DwmExtendFrameIntoClientArea(HWND hwnd, const MARGINS* margins)
{
    FIXME("(%p, %p) stub\n", hwnd, margins);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmGetColorizationColor      (DWMAPI.@)
 */
HRESULT WINAPI DwmGetColorizationColor(DWORD *colorization, BOOL opaque_blend)
{
    FIXME("(%p, %d) stub\n", colorization, opaque_blend);

    return E_NOTIMPL;
}

/**********************************************************************
 *                  DwmFlush              (DWMAPI.@)
 */
HRESULT WINAPI DwmFlush(void)
{
    FIXME("() stub\n");

    return E_NOTIMPL;
}

/**********************************************************************
 *        DwmInvalidateIconicBitmaps      (DWMAPI.@)
 */
HRESULT WINAPI DwmInvalidateIconicBitmaps(HWND hwnd)
{
    static BOOL once;

    if (!once++) FIXME("(%p) stub\n", hwnd);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmSetWindowAttribute         (DWMAPI.@)
 */
HRESULT WINAPI DwmSetWindowAttribute(HWND hwnd, DWORD attributenum, LPCVOID attribute, DWORD size)
{
    static BOOL once;

    if (!once++) FIXME("(%p, %x, %p, %x) stub\n", hwnd, attributenum, attribute, size);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmGetGraphicsStreamClient         (DWMAPI.@)
 */
HRESULT WINAPI DwmGetGraphicsStreamClient(UINT uIndex, UUID *pClientUuid)
{
    FIXME("(%d, %p) stub\n", uIndex, pClientUuid);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmGetTransportAttributes         (DWMAPI.@)
 */
HRESULT WINAPI DwmGetTransportAttributes(BOOL *pfIsRemoting, BOOL *pfIsConnected, DWORD *pDwGeneration)
{
    FIXME("(%p, %p, %p) stub\n", pfIsRemoting, pfIsConnected, pDwGeneration);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmUnregisterThumbnail         (DWMAPI.@)
 */
HRESULT WINAPI DwmUnregisterThumbnail(HTHUMBNAIL thumbnail)
{
    FIXME("(%p) stub\n", thumbnail);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmEnableMMCSS         (DWMAPI.@)
 */
HRESULT WINAPI DwmEnableMMCSS(BOOL enableMMCSS)
{
    FIXME("(%d) stub\n", enableMMCSS);

    return S_OK;
}

/**********************************************************************
 *           DwmGetGraphicsStreamTransformHint         (DWMAPI.@)
 */
HRESULT WINAPI DwmGetGraphicsStreamTransformHint(UINT uIndex, MilMatrix3x2D *pTransform)
{
    FIXME("(%d, %p) stub\n", uIndex, pTransform);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmEnableBlurBehindWindow         (DWMAPI.@)
 */
HRESULT WINAPI DwmEnableBlurBehindWindow(HWND hWnd, const DWM_BLURBEHIND *pBlurBuf)
{
    FIXME("%p %p\n", hWnd, pBlurBuf);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmDefWindowProc         (DWMAPI.@)
 */
BOOL WINAPI DwmDefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult)
{
    static int i;

    if (!i++) FIXME("stub\n");

    return FALSE;
}

/**********************************************************************
 *           DwmGetWindowAttribute         (DWMAPI.@)
 */
HRESULT WINAPI DwmGetWindowAttribute(HWND hwnd, DWORD attribute, PVOID pv_attribute, DWORD size)
{
    FIXME("(%p %d %p %d) stub\n", hwnd, attribute, pv_attribute, size);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmRegisterThumbnail         (DWMAPI.@)
 */
HRESULT WINAPI DwmRegisterThumbnail(HWND dest, HWND src, PHTHUMBNAIL thumbnail_id)
{
    FIXME("(%p %p %p) stub\n", dest, src, thumbnail_id);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmGetCompositionTimingInfo         (DWMAPI.@)
 */
HRESULT WINAPI DwmGetCompositionTimingInfo(HWND hwnd, DWM_TIMING_INFO *info)
{
    static int i;

    if(!i++) FIXME("(%p %p)\n", hwnd, info);

    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmAttachMilContent         (DWMAPI.@)
 */
HRESULT WINAPI DwmAttachMilContent(HWND hwnd)
{
    FIXME("(%p) stub\n", hwnd);
    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmDetachMilContent         (DWMAPI.@)
 */
HRESULT WINAPI DwmDetachMilContent(HWND hwnd)
{
    FIXME("(%p) stub\n", hwnd);
    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmUpdateThumbnailProperties         (DWMAPI.@)
 */
HRESULT WINAPI DwmUpdateThumbnailProperties(HTHUMBNAIL thumbnail, const DWM_THUMBNAIL_PROPERTIES *props)
{
    return E_NOTIMPL;
}

/**********************************************************************
 *           DwmSetPresentParameters         (DWMAPI.@)
 */
HRESULT WINAPI DwmSetPresentParameters(HWND hwnd, DWM_PRESENT_PARAMETERS *params)
{
    FIXME("(%p %p) stub\n", hwnd, params);
    return S_OK;
}

HRESULT WINAPI DwmSetDxFrameDuration(
   HWND hwnd,
   INT  cRefreshes
)
{
    return E_NOTIMPL;
}

HRESULT WINAPI DwmModifyPreviousDxFrameDuration(
   HWND hwnd,
   INT  cRefreshes,
   BOOL fRelative
)
{
    return E_NOTIMPL;
}

HRESULT WINAPI DwmQueryThumbnailSourceSize(
        HTHUMBNAIL hThumbnail,
  _Out_ PSIZE      pSize
)
{
    return E_NOTIMPL;
}


HRESULT 
WINAPI 
DwmSetIconicLivePreviewBitmap(
           HWND    hwnd,
           HBITMAP hbmp,
  _In_opt_ POINT   *pptClient,
           DWORD   dwSITFlags
)
{
    return E_NOTIMPL;
}

HRESULT WINAPI DwmSetIconicThumbnail(
  _In_ HWND    hwnd,
  _In_ HBITMAP hbmp,
  _In_ DWORD   dwSITFlags
)
{
    return E_NOTIMPL;	
}