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
#include "d3d9_private.h"
#include <ntstatus.h>
#define WIN32_NO_STATUS


#define LOCK_D3D9()     EnterCriticalSection(&This->d3d9_cs);
#define UNLOCK_D3D9()   LeaveCriticalSection(&This->d3d9_cs);
#define MEM_ALIGNMENT 0x20
	
//WINE_DEFAULT_DEBUG_CHANNEL(d3d9); 	

IDirect3D9 *ppD3D;

IDirect3D9ExVtbl Direct3D9Ex_Vtbl =
{
    /* IUnknown */
    IDirect3D9ExImpl_QueryInterface,
    IDirect3D9ExImpl_AddRef,
    IDirect3D9ExImpl_Release,
    /* IDirect3D9 */
    IDirect3D9ExImpl_RegisterSoftwareDevice,
    IDirect3D9ExImpl_GetAdapterCount,
    IDirect3D9ExImpl_GetAdapterIdentifier,
    IDirect3D9ExImpl_GetAdapterModeCount,
    IDirect3D9ExImpl_EnumAdapterModes,
    IDirect3D9ExImpl_GetAdapterDisplayMode,
    IDirect3D9ExImpl_CheckDeviceType,
    IDirect3D9ExImpl_CheckDeviceFormat,
    IDirect3D9ExImpl_CheckDeviceMultiSampleType,
    IDirect3D9ExImpl_CheckDepthStencilMatch,
    IDirect3D9ExImpl_CheckDeviceFormatConversion,
    IDirect3D9ExImpl_GetDeviceCaps,
    IDirect3D9ExImpl_GetAdapterMonitor,
    IDirect3D9ExImpl_CreateDevice,
    /* IDirect3D9Ex */
    IDirect3D9ExImpl_GetAdapterModeCountEx,
    IDirect3D9ExImpl_EnumAdapterModesEx,
    IDirect3D9ExImpl_GetAdapterDisplayModeEx,
    IDirect3D9ExImpl_CreateDeviceEx,
    IDirect3D9ExImpl_GetAdapterLUID,
};

static HRESULT WINAPI IDirect3D9ExImpl_RegisterSoftwareDevice(IDirect3D9Ex *iface, void *init_function)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->RegisterSoftwareDevice(ppD3D, init_function);
	}
	
	return D3DERR_INVALIDCALL;	
}

static UINT WINAPI IDirect3D9ExImpl_GetAdapterCount(IDirect3D9Ex * iface)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetAdapterCount(ppD3D);
	}
	
	return 0;	
}

static HRESULT WINAPI IDirect3D9ExImpl_GetAdapterIdentifier(IDirect3D9Ex *iface, UINT adapter,
        DWORD flags, D3DADAPTER_IDENTIFIER9 *identifier)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetAdapterIdentifier(ppD3D, adapter, flags, identifier);
	}
	
	return D3DERR_INVALIDCALL;	
}

static UINT WINAPI IDirect3D9ExImpl_GetAdapterModeCount(IDirect3D9Ex * iface, UINT Adapter, D3DFORMAT Format)	
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetAdapterModeCount(ppD3D, Adapter, Format);
	}
	
	return 0;	
}	

static HRESULT WINAPI IDirect3D9ExImpl_EnumAdapterModes(IDirect3D9Ex * iface, UINT Adapter, D3DFORMAT Format,
                                                      UINT Mode, D3DDISPLAYMODE* pMode)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->EnumAdapterModes(ppD3D, Adapter, Format, Mode, pMode);
	}
	
	return D3DERR_INVALIDCALL;	
}														  

static HRESULT WINAPI IDirect3D9ExImpl_GetAdapterDisplayMode(IDirect3D9Ex * iface, UINT Adapter, D3DDISPLAYMODE* pMode)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetAdapterDisplayMode(ppD3D, Adapter, pMode);
	}
	
	return D3DERR_INVALIDCALL;	
}	

static HRESULT WINAPI IDirect3D9ExImpl_CheckDeviceType(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                     D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CheckDeviceType(ppD3D, Adapter, DeviceType, DisplayFormat, BackBufferFormat, Windowed);
	}
	
	return D3DERR_INVALIDCALL;	
}		

static HRESULT WINAPI IDirect3D9ExImpl_CheckDeviceFormat(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                       D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType,
                                                       D3DFORMAT CheckFormat)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CheckDeviceFormat(ppD3D, Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
	}
	
	return D3DERR_INVALIDCALL;	
}		

static HRESULT WINAPI IDirect3D9ExImpl_CheckDeviceMultiSampleType(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                                D3DFORMAT SurfaceFormat, BOOL Windowed,
                                                                D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)		
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CheckDeviceMultiSampleType(ppD3D, Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
	}
	
	return D3DERR_INVALIDCALL;	
}		

static HRESULT WINAPI IDirect3D9ExImpl_CheckDepthStencilMatch(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                            D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat,
                                                            D3DFORMAT DepthStencilFormat)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CheckDepthStencilMatch(ppD3D, Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
	}
	
	return D3DERR_INVALIDCALL;	
}		

static HRESULT WINAPI IDirect3D9ExImpl_CheckDeviceFormatConversion(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                                 D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)		
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CheckDeviceFormatConversion(ppD3D, Adapter, DeviceType, SourceFormat, TargetFormat);
	}
	
	return D3DERR_INVALIDCALL;	
}	

static HRESULT WINAPI IDirect3D9ExImpl_GetDeviceCaps(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetDeviceCaps(ppD3D, Adapter, DeviceType, pCaps);
	}

	return D3DERR_INVALIDCALL;	
}		

static HMONITOR WINAPI IDirect3D9ExImpl_GetAdapterMonitor(IDirect3D9Ex * iface, UINT Adapter)	
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetAdapterMonitor(ppD3D, Adapter);
	}	
	
	return NULL;
}		

static HRESULT WINAPI IDirect3D9ExImpl_CreateDevice(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                  HWND hFocusWindow, DWORD BehaviourFlags,
                                                  D3DPRESENT_PARAMETERS* pPresentationParameters,
                                                  struct IDirect3DDevice9** ppReturnedDeviceInterface)	
{
	return S_OK;
}	

static UINT WINAPI IDirect3D9ExImpl_GetAdapterModeCountEx(IDirect3D9Ex *iface,
        UINT adapter, const D3DDISPLAYMODEFILTER *filter)
{
    UINT AdapterModeCount;
	
	if(ppD3D!=NULL){
		return IDirect3D9_GetAdapterModeCount(ppD3D,adapter,filter->Format);
	}

	return D3DERR_INVALIDCALL;
}

static HRESULT WINAPI IDirect3D9ExImpl_EnumAdapterModesEx(IDirect3D9Ex *iface,
        UINT adapter, const D3DDISPLAYMODEFILTER *filter, UINT Mode, D3DDISPLAYMODEEX *pmodeEx)	
{
	D3DDISPLAYMODE pMode;
	HRESULT ret;
	//DEVMODEW devMode;
	
	//EnumDisplaySettingsExW(adapter->DeviceName, j++, &devMode, 0))
	
	if(ppD3D!=NULL){
		ret = (ppD3D)->lpVtbl->EnumAdapterModes(ppD3D,adapter,filter->Format, Mode, &pMode);
		if(ret == D3D_OK)
		{
			pmodeEx->Size = sizeof(D3DDISPLAYMODEEX);
			pmodeEx->Width = pMode.Width;
			pmodeEx->Height = pMode.Height;
			pmodeEx->RefreshRate = pMode.RefreshRate;
			pmodeEx->Format = pMode.Format;
			pmodeEx->ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE; //HardCoded for now, need call EnumDisplaySettingsExW
			return ret;
		}
	}
	
	return D3DERR_INVALIDCALL;	
}		

static HRESULT WINAPI IDirect3D9ExImpl_GetAdapterDisplayModeEx(IDirect3D9Ex *iface,
        UINT adapter, D3DDISPLAYMODEEX *pmodeEx, D3DDISPLAYROTATION *rotation)	
{
	D3DDISPLAYMODE pMode;
	HRESULT ret;
	//DEVMODEW devMode;
	
	//EnumDisplaySettingsExW(adapter->DeviceName, j++, &devMode, 0))
	
	if(ppD3D!=NULL){
		ret = (ppD3D)->lpVtbl->GetAdapterDisplayMode(ppD3D, adapter, &pMode);
		if(ret == D3D_OK)
		{
			pmodeEx->Size = sizeof(D3DDISPLAYMODEEX);
			pmodeEx->Width = pMode.Width;
			pmodeEx->Height = pMode.Height;
			pmodeEx->RefreshRate = pMode.RefreshRate;
			pmodeEx->Format = pMode.Format;
			pmodeEx->ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE; //HardCoded for now, need call EnumDisplaySettingsExW
			
			*rotation = D3DDISPLAYROTATION_IDENTITY;
			return ret;
		}
	}
}		

static HRESULT WINAPI IDirect3D9ExImpl_GetAdapterLUID(IDirect3D9Ex *iface, UINT adapter, LUID *luid)
{
	return S_OK;
}		

static HRESULT WINAPI IDirect3D9ExImpl_CreateDeviceEx(IDirect3D9Ex *iface,
        UINT adapter, D3DDEVTYPE device_type, HWND focus_window, DWORD flags,
        D3DPRESENT_PARAMETERS *parameters, D3DDISPLAYMODEEX *mode, IDirect3DDevice9Ex **device)
{
	return S_OK;
}

/* Convert a IDirect3D9 pointer safely to the internal implementation struct */
static LPDIRECT3D9_INT IDirect3D9ToImpl(LPDIRECT3D9EX iface)
{
    if (NULL == iface)
        return NULL;

    return (LPDIRECT3D9_INT)((ULONG_PTR)iface - FIELD_OFFSET(DIRECT3D9_INT, lpVtbl));
}

VOID AlignedFree(IN OUT LPVOID pObject)
{
    CHAR *NonAlignedPtr = pObject;
    ULONG_PTR *AlignedPtr = pObject;

    if (pObject == 0)
        return;

    NonAlignedPtr -= *(AlignedPtr - 1);

    HeapFree(GetProcessHeap(), 0, NonAlignedPtr);
}

/* IDirect3D9: IUnknown implementation */
static HRESULT WINAPI IDirect3D9ExImpl_QueryInterface(LPDIRECT3D9EX iface, REFIID riid, LPVOID* ppvObject)
{
    LPDIRECT3D9_INT This = IDirect3D9ToImpl(iface);

    if (IsEqualGUID(riid, &IID_IUnknown) || IsEqualGUID(riid, &IID_IDirect3D9Ex))
    {
        IUnknown_AddRef(iface);
        *ppvObject = &This->lpVtbl;
        return S_OK;
    }

    *ppvObject = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI IDirect3D9ExImpl_AddRef(LPDIRECT3D9EX iface)
{
    LPDIRECT3D9_INT This = IDirect3D9ToImpl(iface);
    ULONG ref = InterlockedIncrement(&This->refcount);

    return ref;
}

static ULONG WINAPI IDirect3D9ExImpl_Release(LPDIRECT3D9EX iface)
{
    LPDIRECT3D9_INT This = IDirect3D9ToImpl(iface);
    ULONG ref = InterlockedDecrement(&This->refcount);

    if (ref == 0)
    {
        EnterCriticalSection(&This->d3d9_cs);
        /* TODO: Free resources here */
        LeaveCriticalSection(&This->d3d9_cs);
        AlignedFree(This);
    }

	return ref;
}

HRESULT AlignedAlloc(IN OUT LPVOID *ppObject, IN SIZE_T dwSize)
{
    ULONG_PTR AddressOffset;
    ULONG AlignedMask = MEM_ALIGNMENT - 1;
    CHAR *AlignedPtr;
    ULONG_PTR *AlignedOffsetPtr;

    if (ppObject == 0)
        return DDERR_INVALIDPARAMS;

    if (dwSize == 0)
    {
        *ppObject = NULL;
        return S_OK;
    }

    dwSize += MEM_ALIGNMENT;

    AlignedPtr = (CHAR *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);

    if (AlignedPtr == 0)
        return DDERR_OUTOFMEMORY;

    AddressOffset = MEM_ALIGNMENT - ((ULONG_PTR)AlignedPtr & AlignedMask);
	
    AlignedPtr += AddressOffset;

    AlignedOffsetPtr = (ULONG_PTR *)(AlignedPtr - sizeof(ULONG));
    *AlignedOffsetPtr = AddressOffset;

    *ppObject = (ULONG_PTR *)AlignedPtr;

    return S_OK;
}

HRESULT CreateD3D9Ex(OUT LPDIRECT3D9EX *ppDirect3D9, UINT SDKVersion)
{
    LPDIRECT3D9_INT pDirect3D9;

    if (ppDirect3D9 == 0)
        return DDERR_INVALIDPARAMS;

     if (AlignedAlloc((LPVOID *)&pDirect3D9, sizeof(DIRECT3D9_INT)) != S_OK)
         return DDERR_OUTOFMEMORY;

    if (pDirect3D9 == 0)
        return DDERR_OUTOFMEMORY;

    pDirect3D9->lpVtbl = &Direct3D9Ex_Vtbl;
    //pDirect3D9->dwProcessId = GetCurrentThreadId();
    //pDirect3D9->lRefCnt = 1;

    pDirect3D9->SDKVersion = SDKVersion;

   // pDirect3D9->lpInt = pDirect3D9;
	pDirect3D9->refcount = 1;

    InitializeCriticalSection(&pDirect3D9->d3d9_cs);

    // if (FALSE == GetDisplayDeviceInfo(pDirect3D9))
    // {
        // DPRINT1("Could not create Direct3D9 object");
        // AlignedFree(pDirect3D9);
        // return DDERR_GENERIC;
    // }

    *ppDirect3D9 = (LPDIRECT3D9EX)&pDirect3D9->lpVtbl;

    return D3D_OK;
}

static D3DFORMAT Get16BitD3DFormat(LPCSTR lpszDeviceName)
{
    HDC hDC;
    HBITMAP hBitmap;
    LPBITMAPINFO pBitmapInfo;
    D3DFORMAT Format = D3DFMT_R5G6B5;

    if (NULL == (hDC = CreateDCA(NULL, lpszDeviceName, NULL, NULL)))
    {
        return Format;
    }

    if (NULL == (hBitmap = CreateCompatibleBitmap(hDC, 1, 1)))
    {
        DeleteDC(hDC);
        return Format;
    }

    pBitmapInfo = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BITMAPINFOHEADER) + 4 * sizeof(RGBQUAD));
    if (NULL == pBitmapInfo)
    {
        DeleteObject(hBitmap);
        DeleteDC(hDC);
        return Format;
    }

    pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    if (GetDIBits(hDC, hBitmap, 0, 0, NULL, pBitmapInfo, DIB_RGB_COLORS) > 0)
    {
        if (pBitmapInfo->bmiHeader.biCompression == BI_BITFIELDS)
        {
            if (GetDIBits(hDC, hBitmap, 0, pBitmapInfo->bmiHeader.biHeight, NULL, pBitmapInfo, DIB_RGB_COLORS) > 0)
            {
                /* Check if the green field is 6 bits long */
                if (*(DWORD*)(&pBitmapInfo->bmiColors[1]) == 0x000003E0)
                {
                    Format = D3DFMT_X1R5G5B5;
                }
            }
        }
    }

    HeapFree(GetProcessHeap(), 0, pBitmapInfo);
    DeleteObject(hBitmap);
    DeleteDC(hDC);

    return Format;
}

BOOL GetAdapterMode(LPCSTR lpszDeviceName, D3DDISPLAYMODE* pMode)
{
    DEVMODEA DevMode;
    
    memset(&DevMode, 0, sizeof(DEVMODEA));
    DevMode.dmSize = sizeof(DEVMODEA);
    if (FALSE == EnumDisplaySettingsA(lpszDeviceName, ENUM_CURRENT_SETTINGS, &DevMode))
        return FALSE;

    pMode->Width = DevMode.dmPelsWidth;
    pMode->Height = DevMode.dmPelsHeight;
    pMode->RefreshRate = DevMode.dmDisplayFrequency;
    
    switch (DevMode.dmBitsPerPel)
    {
    case 8:
        pMode->Format = D3DFMT_P8;
        break;

    case 16:
        pMode->Format = Get16BitD3DFormat(lpszDeviceName);
        break;

    case 24:
        pMode->Format = D3DFMT_R8G8B8;
        break;

    case 32:
        pMode->Format = D3DFMT_X8R8G8B8;
        break;

    default:
        pMode->Format = D3DFMT_UNKNOWN;
        break;
    }

    return TRUE;
}

HRESULT WINAPI Direct3DCreate9Ex(
  _In_  UINT         SDKVersion,
  _Out_ IDirect3D9Ex **ppD3DEx
)
{
	IDirect3D9 *ppD3D;
	
	ppD3D = Direct3DCreate9(SDKVersion);
	
	CreateD3D9Ex(ppD3DEx, SDKVersion);
	
	return S_OK;
}