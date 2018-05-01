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

static const struct IDirect3D9ExVtbl Direct3D9Ex_Vtbl =
{
    /* IUnknown */
     IDirect3D9Impl_QueryInterface,
     IDirect3D9Impl_AddRef,
     IDirect3D9Impl_Release,
    // /* IDirect3D9Ex */
    // IDirect3D9Impl_GetAdapterModeCountEx,
    // IDirect3D9Impl_EnumAdapterModesEx,
    // IDirect3D9Impl_GetAdapterDisplayModeEx,
    // IDirect3D9Impl_CreateDeviceEx,
    // IDirect3D9Impl_GetAdapterLUID,
};

typedef struct _DIRECT3D9_INT
{
    IDirect3D9Ex IDirect3D9Ex_iface;
    LONG refcount;
	CRITICAL_SECTION d3d9_cs;
	struct IDirect3D9ExVtbl *lpVtbl; /* LPDIRECTD3D9 function table */
	UINT SDKVersion;
    //Direct3D9DisplayAdapterInfo DisplayAdapters[D3D9_INT_MAX_NUM_ADAPTERS];	
    //struct wined3d *wined3d;
    //BOOL extended;
}DIRECT3D9_INT, *LPDIRECT3D9_INT;

/* Convert a IDirect3D9 pointer safely to the internal implementation struct */
static LPDIRECT3D9_INT IDirect3D9ToImpl(LPDIRECT3D9 iface)
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
static HRESULT WINAPI IDirect3D9Impl_QueryInterface(LPDIRECT3D9 iface, REFIID riid, LPVOID* ppvObject)
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

static ULONG WINAPI IDirect3D9Impl_AddRef(LPDIRECT3D9 iface)
{
    LPDIRECT3D9_INT This = IDirect3D9ToImpl(iface);
    ULONG ref = InterlockedIncrement(&This->refcount);

    return ref;
}

static ULONG WINAPI IDirect3D9Impl_Release(LPDIRECT3D9 iface)
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

static UINT WINAPI IDirect3D9Impl_GetAdapterModeCountEx(
   UINT                 Adapter,
   const D3DDISPLAYMODEFILTER *pFilter
)
{
    UINT AdapterModeCount;
	
	if(ppD3D!=NULL){
		return IDirect3D9_GetAdapterModeCount(ppD3D,Adapter,pFilter->Format);
	}

	return D3DERR_INVALIDCALL;
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

    *ppDirect3D9 = (LPDIRECT3D9)&pDirect3D9->lpVtbl;

    return D3D_OK;
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