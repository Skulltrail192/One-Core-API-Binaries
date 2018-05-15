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

HRESULT InitD3D9BaseDevice(LPDIRECT3DDEVICE9_INT pThisBaseDevice, LPDIRECT3D9_INT pDirect3D9,
                           UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviourFlags,
                           D3DPRESENT_PARAMETERS* pPresentationParameters, DWORD NumAdaptersToCreate)
{
    D3D9ResourceManager* pResourceManager;
    DWORD i;

    // Insert Reset/Ctor here

    if (FAILED(AlignedAlloc((LPVOID *)&pResourceManager, sizeof(D3D9ResourceManager))) ||
        FAILED(InitD3D9ResourceManager(pResourceManager, pThisBaseDevice)))
    {
        DPRINT1("Could not create resource manager");
        return DDERR_OUTOFMEMORY;
    }

    pThisBaseDevice->pResourceManager = pResourceManager;

    pThisBaseDevice->lpVtbl = &Direct3DDevice9HAL_Vtbl;
    pThisBaseDevice->lRefCnt = 1;
    pThisBaseDevice->dwProcessId = GetCurrentThreadId();
    pThisBaseDevice->pUnknown = (IUnknown*) &pThisBaseDevice->lpVtbl;
    InitializeCriticalSection(&pThisBaseDevice->CriticalSection);

    pThisBaseDevice->pDirect3D9 = pDirect3D9;
    pThisBaseDevice->DeviceType = DeviceType;
    pThisBaseDevice->hWnd = hFocusWindow;
    pThisBaseDevice->AdjustedBehaviourFlags = BehaviourFlags;
    pThisBaseDevice->BehaviourFlags = BehaviourFlags;
    pThisBaseDevice->NumAdaptersInDevice = NumAdaptersToCreate;

    // TODO: Query driver for correct DX version
    pThisBaseDevice->dwDXVersion = 9;

    for (i = 0; i < NumAdaptersToCreate; i++)
    {
        if (FALSE == CreateD3D9DeviceData(&pDirect3D9->DisplayAdapters[i], &pThisBaseDevice->DeviceData[i]))
        {
            DPRINT1("Failed to get device data for adapter: %d", i);
            return DDERR_GENERIC;
        }

        pThisBaseDevice->AdapterIndexInGroup[i] = i;
        pThisBaseDevice->CurrentDisplayMode[i].Width = pDirect3D9->DisplayAdapters[i].DriverCaps.dwDisplayWidth;
        pThisBaseDevice->CurrentDisplayMode[i].Height = pDirect3D9->DisplayAdapters[i].DriverCaps.dwDisplayHeight;
        pThisBaseDevice->CurrentDisplayMode[i].RefreshRate = pDirect3D9->DisplayAdapters[i].DriverCaps.dwRefreshRate;
        pThisBaseDevice->CurrentDisplayMode[i].Format = pDirect3D9->DisplayAdapters[i].DriverCaps.RawDisplayFormat;

        pThisBaseDevice->pSwapChains[i] = CreateDirect3DSwapChain9(RT_BUILTIN, pThisBaseDevice, i);
        pThisBaseDevice->pSwapChains2[i] = pThisBaseDevice->pSwapChains[i];

        if (FAILED(Direct3DSwapChain9_Init(pThisBaseDevice->pSwapChains[i], pPresentationParameters)))
        {
            DPRINT1("Failed to init swap chain: %d", i);
            return DDERR_GENERIC;
        }
    }

    return D3D_OK;
}

HRESULT CreateD3D9HalDevice(LPDIRECT3D9Ex_INT pDirect3D9, UINT Adapter,
                            HWND hFocusWindow, DWORD BehaviourFlags,
                            D3DPRESENT_PARAMETERS* pPresentationParameters,
                            DWORD NumAdaptersToCreate,
                            struct IDirect3DDevice9** ppReturnedDeviceInterface)
{
    HRESULT Ret;

    if (FAILED(AlignedAlloc((LPVOID *)ppReturnedDeviceInterface, sizeof(D3D9HALDEVICE))))
    {
        DPRINT1("Not enough memory to create HAL device");
        return DDERR_OUTOFMEMORY;
    }

    Ret = InitD3D9BaseDevice((LPDIRECT3DDEVICE9_INT)*ppReturnedDeviceInterface, pDirect3D9, Adapter,
                             D3DDEVTYPE_HAL, hFocusWindow, BehaviourFlags,
                             pPresentationParameters, NumAdaptersToCreate);

    if (FAILED(Ret))
    {
        AlignedFree((LPVOID)*ppReturnedDeviceInterface);
        return Ret;
    }

    return D3D_OK;
}


/*++
* @name IDirect3D9::CreateDevice
* @implemented
*
* The function IDirect3D9Impl_CreateDevice creates an IDirect3DDevice9 object
* that represents the display adapter.
*
* @param LPDIRECT3D iface
* Pointer to the IDirect3D9 object returned from Direct3DCreate9()
*
* @param UINT Adapter
* Adapter index to get information about. D3DADAPTER_DEFAULT is the primary display.
* The maximum value for this is the value returned by IDirect3D::GetAdapterCount() - 1.
*
* @param D3DDEVTYPE DeviceType
* One of the D3DDEVTYPE enum members.
*
* @param HWND hFocusWindow
* A window handle that is used as a reference when Direct3D should switch between
* foreground mode and background mode.
*
* @param DWORD BehaviourFlags
* Any valid combination of the D3DCREATE constants.
*
* @param D3DPRESENT_PARAMETERS* pPresentationParameters
* Pointer to a D3DPRESENT_PARAMETERS structure describing the parameters for the device
* to be created. If D3DCREATE_ADAPTERGROUP_DEVICE is specified in the BehaviourFlags parameter,
* the pPresentationParameters is treated as an array.
*
* @param IDirect3DDevice9** ppReturnedDeviceInterface
* Return object that represents the created device.
*
* @return HRESULT
* If the method successfully creates a device and returns a valid ppReturnedDeviceInterface object,
* the return value is D3D_OK.
* If Adapter is out of range, DeviceType is invalid, hFocusWindow is not a valid, BehaviourFlags is invalid
* pPresentationParameters is invalid or ppReturnedDeviceInterface is a bad pointer, the return value
* will be D3DERR_INVALIDCALL.
*
*/
static 
HRESULT 
WINAPI 
IDirect3D9Impl_CreateDeviceEx(
	IDirect3D9Ex *iface, 
	UINT Adapter, 
	D3DDEVTYPE DeviceType,
    HWND hFocusWindow, 
	DWORD BehaviourFlags,
    D3DPRESENT_PARAMETERS* pPresentationParameters,
	D3DDISPLAYMODEEX      *pFullscreenDisplayMode,
    struct IDirect3DDevice9Ex** ppReturnedDeviceInterface
)
{
    DWORD NumAdaptersToCreate;
    HRESULT Ret;

    LPDIRECT3D9_INT This = IDirect3D9ToImpl(iface);
    LOCK_D3D9();

    // if (Adapter >= This->NumDisplayAdapters)
    // {
        // DPRINT1("Invalid Adapter number specified");
        // UNLOCK_D3D9();
        // return D3DERR_INVALIDCALL;
    // }

    // if (DeviceType != D3DDEVTYPE_HAL &&
        // DeviceType != D3DDEVTYPE_REF &&
        // DeviceType != D3DDEVTYPE_SW)
    // {
        // DPRINT1("Invalid DeviceType specified");
        // UNLOCK_D3D9();
        // return D3DERR_INVALIDCALL;
    // }

    // if (DeviceType != D3DDEVTYPE_HAL)
    // {
        // UNIMPLEMENTED
        // DPRINT1("Sorry, only D3DDEVTYPE_HAL is implemented at this time...");
        // return D3DERR_INVALIDCALL;
    // }

    // if (hFocusWindow != NULL && FALSE == IsWindow(hFocusWindow))
    // {
        // DPRINT1("Invalid hFocusWindow parameter specified, expected NULL or a valid HWND");
        // UNLOCK_D3D9();
        // return D3DERR_INVALIDCALL;
    // }

    // if (NULL == pPresentationParameters)
    // {
        // DPRINT1("Invalid pPresentationParameters parameter specified");
        // UNLOCK_D3D9();
        // return D3DERR_INVALIDCALL;
    // }

    // if (pPresentationParameters->hDeviceWindow != NULL && FALSE == IsWindow(pPresentationParameters->hDeviceWindow))
    // {
        // DPRINT1("Invalid pPresentationParameters->hDeviceWindow parameter specified, expected NULL or a valid HWND");
        // UNLOCK_D3D9();
        // return D3DERR_INVALIDCALL;
    // }

    // if (FALSE == pPresentationParameters->Windowed && hFocusWindow == NULL)
    // {
        // DPRINT1("When pPresentationParameters->Windowed is not set, hFocusWindow must be a valid HWND");
        // UNLOCK_D3D9();
        // return D3DERR_INVALIDCALL;
    // }

    // if (NULL == hFocusWindow && NULL == pPresentationParameters->hDeviceWindow)
    // {
        // DPRINT1("Any of pPresentationParameters->Windowed and hFocusWindow must be set to a valid HWND");
        // UNLOCK_D3D9();
        // return D3DERR_INVALIDCALL;
    // }

    // if (Adapter > 0 && NULL == pPresentationParameters->hDeviceWindow)
    // {
        // DPRINT1("Invalid pPresentationParameters->hDeviceWindow, must be set to a valid unique HWND when Adapter is greater than 0");
        // UNLOCK_D3D9();
        // return D3DERR_INVALIDCALL;
    // }

    // if (NULL == ppReturnedDeviceInterface)
    // {
        // DPRINT1("Invalid ppReturnedDeviceInterface parameter specified");
        // UNLOCK_D3D9();
        // return D3DERR_INVALIDCALL;
    // }

    // // if ((BehaviourFlags & D3DCREATE_ADAPTERGROUP_DEVICE) != 0)
        // // NumAdaptersToCreate = This->DisplayAdapters[Adapter].NumAdaptersInGroup;
    // // else
        // // NumAdaptersToCreate = 1;

    *ppReturnedDeviceInterface = 0;

    Ret = CreateD3D9HalDevice(This, Adapter, hFocusWindow, BehaviourFlags, pPresentationParameters, NumAdaptersToCreate, ppReturnedDeviceInterface);

    UNLOCK_D3D9();
    return Ret;
}
