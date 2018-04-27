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

/* INCLUDES ******************************************************************/

/* C Headers */
#include <rtl.h>
#include <winsvc.h>
#include <evntprov.h>
#include <ntsecapi.h>
#include <ndk/cmfuncs.h>
#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>
	
WINE_DEFAULT_DEBUG_CHANNEL(d3d9); 	

IDirect3D9 *ppD3D;

HRESULT CreateD3D9Ex(OUT LPDIRECT3D9EX *ppDirect3D9, UINT SDKVersion)
{
    LPDIRECT3D9_INT pDirect3D9;

    if (ppDirect3D9 == 0)
        return DDERR_INVALIDPARAMS;

    if (AlignedAlloc((LPVOID *)&pDirect3D9, sizeof(DIRECT3D9_INT)) != S_OK)
        return DDERR_OUTOFMEMORY;

    if (pDirect3D9 == 0)
        return DDERR_OUTOFMEMORY;

    pDirect3D9->lpVtbl = &Direct3D9_Vtbl;
    pDirect3D9->dwProcessId = GetCurrentThreadId();
    pDirect3D9->lRefCnt = 1;

    pDirect3D9->SDKVersion = SDKVersion;

    pDirect3D9->lpInt = pDirect3D9;
    pDirect3D9->unknown000007 = 1;

    InitializeCriticalSection(&pDirect3D9->d3d9_cs);

    // if (FALSE == GetDisplayDeviceInfo(pDirect3D9))
    // {
        // DPRINT1("Could not create Direct3D9 object");
        // AlignedFree(pDirect3D9);
        // return DDERR_GENERIC;
    // }

    *ppDirect3D9 = (LPDIRECT3D9)&pDirect3D9->lpVtbl;

    return D3D_OK;
}


HRESULT Direct3DCreate9Ex(
  _In_  UINT         SDKVersion,
  _Out_ IDirect3D9Ex **ppD3DEx
)
{
	IDirect3D9 *ppD3D;
	
	ppD3D = Direct3DCreate9(SDKVersion);
	
	CreateD3D9Ex(ppD3DEx, SDKVersion);
	
	return S_OK;
}