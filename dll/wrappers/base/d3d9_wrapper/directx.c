/*==========================================================================;
 *
 *  Copyright (C) 2018 Shorthorn Project.  All Rights Reserved.
 *
 *  File:   directx.c
 *  Content:    Direct3D IDirect3D9 implementation
 */ 

#include "config.h"
#include "d3d9_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(d3d9);

static inline struct d3d9 *impl_from_IDirect3D9Ex(IDirect3D9Ex *iface)
{
    return CONTAINING_RECORD(iface, struct d3d9, IDirect3D9Ex_iface);
}

static HRESULT WINAPI d3d9_QueryInterface(IDirect3D9Ex *iface, REFIID riid, void **out)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);

    TRACE("iface %p, riid %s, out %p.\n", iface, debugstr_guid(riid), out);

    if (IsEqualGUID(riid, &IID_IDirect3D9)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        IDirect3D9Ex_AddRef(&d3d9->IDirect3D9Ex_iface);
        *out = &d3d9->IDirect3D9Ex_iface;
        return S_OK;
    }

    if (IsEqualGUID(riid, &IID_IDirect3D9Ex))
    {
        if (!d3d9->extended)
        {
            WARN("Application asks for IDirect3D9Ex, but this instance wasn't created with Direct3DCreate9Ex.\n");
            *out = NULL;
            return E_NOINTERFACE;
        }

        IDirect3D9Ex_AddRef(&d3d9->IDirect3D9Ex_iface);
        *out = &d3d9->IDirect3D9Ex_iface;
        return S_OK;
    }

    WARN("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(riid));

    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI d3d9_AddRef(IDirect3D9Ex *iface)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    ULONG refcount = InterlockedIncrement(&d3d9->refcount);

    TRACE("%p increasing refcount to %u.\n", iface, refcount);

    return refcount;
}

static ULONG WINAPI d3d9_Release(IDirect3D9Ex *iface)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    ULONG refcount = InterlockedDecrement(&d3d9->refcount);

    TRACE("%p decreasing refcount to %u.\n", iface, refcount);

    if (!refcount)
    {
        wined3d_mutex_lock();
        wined3d_decref(d3d9->wined3d);
        wined3d_mutex_unlock();

        heap_free(d3d9);
    }

    return refcount;
}

static HRESULT WINAPI d3d9_RegisterSoftwareDevice(IDirect3D9Ex *iface, void *init_function)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->RegisterSoftwareDevice(ppD3D, init_function);
	}
	
	return D3DERR_INVALIDCALL; 
}

static UINT WINAPI d3d9_GetAdapterCount(IDirect3D9Ex *iface)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetAdapterCount(ppD3D);
	}
	
	return 0; 
}

static HRESULT WINAPI d3d9_GetAdapterIdentifier(IDirect3D9Ex *iface, UINT adapter,
        DWORD flags, D3DADAPTER_IDENTIFIER9 *identifier)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetAdapterIdentifier(ppD3D, adapter, flags, identifier);
	}
	
	return D3DERR_INVALIDCALL; 
}

static UINT WINAPI d3d9_GetAdapterModeCount(IDirect3D9Ex *iface, UINT adapter, D3DFORMAT format)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetAdapterModeCount(ppD3D, adapter, format);
	}
	
	return 0; 
}

static HRESULT WINAPI d3d9_EnumAdapterModes(IDirect3D9Ex *iface, UINT adapter,
        D3DFORMAT format, UINT mode_idx, D3DDISPLAYMODE *mode)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->EnumAdapterModes(ppD3D, adapter, format, mode_idx, mode);
	}
	
	return D3DERR_INVALIDCALL; 
}

static HRESULT WINAPI d3d9_GetAdapterDisplayMode(IDirect3D9Ex *iface, UINT adapter, D3DDISPLAYMODE *mode)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetAdapterDisplayMode(ppD3D, adapter, mode);
	}
	
	return D3DERR_INVALIDCALL; 
}

static HRESULT WINAPI d3d9_CheckDeviceType(IDirect3D9Ex *iface, UINT adapter, D3DDEVTYPE device_type,
        D3DFORMAT display_format, D3DFORMAT backbuffer_format, BOOL windowed)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CheckDeviceType(ppD3D, adapter, device_type, display_format, backbuffer_format, windowed);
	}
	
	return D3DERR_INVALIDCALL; 
}

static HRESULT WINAPI d3d9_CheckDeviceFormat(IDirect3D9Ex *iface, UINT adapter, D3DDEVTYPE device_type,
        D3DFORMAT adapter_format, DWORD usage, D3DRESOURCETYPE resource_type, D3DFORMAT format)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CheckDeviceFormat(ppD3D, adapter, device_type, adapter_format, usage, resource_type, format);
	}
	
	return D3DERR_INVALIDCALL; 
}

static HRESULT WINAPI d3d9_CheckDeviceMultiSampleType(IDirect3D9Ex *iface, UINT adapter, D3DDEVTYPE device_type,
        D3DFORMAT format, BOOL windowed, D3DMULTISAMPLE_TYPE multisample_type, DWORD *levels)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CheckDeviceMultiSampleType(ppD3D, adapter, device_type, format, windowed, multisample_type, levels);
	}
	
	return D3DERR_INVALIDCALL; 
}

static HRESULT WINAPI d3d9_CheckDepthStencilMatch(IDirect3D9Ex *iface, UINT adapter, D3DDEVTYPE device_type,
        D3DFORMAT adapter_format, D3DFORMAT rt_format, D3DFORMAT ds_format)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CheckDepthStencilMatch(ppD3D, adapter, device_type, adapter_format, rt_format, ds_format);
	}
	
	return D3DERR_INVALIDCALL; 
}

static HRESULT WINAPI d3d9_CheckDeviceFormatConversion(IDirect3D9Ex *iface, UINT adapter,
        D3DDEVTYPE device_type, D3DFORMAT src_format, D3DFORMAT dst_format)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CheckDeviceFormatConversion(ppD3D, adapter, device_type, src_format, dst_format);
	}
	
	return D3DERR_INVALIDCALL; 
}

static HRESULT WINAPI d3d9_GetDeviceCaps(IDirect3D9Ex *iface, UINT adapter, D3DDEVTYPE device_type, D3DCAPS9 *caps)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetDeviceCaps(ppD3D, adapter, device_type, caps);
	}

	return D3DERR_INVALIDCALL; 
}

static HMONITOR WINAPI d3d9_GetAdapterMonitor(IDirect3D9Ex *iface, UINT adapter)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->GetAdapterMonitor(ppD3D, adapter);
	}	
	
	return NULL;
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_CreateDevice(IDirect3D9Ex *iface, UINT adapter,
        D3DDEVTYPE device_type, HWND focus_window, DWORD flags, D3DPRESENT_PARAMETERS *parameters,
        IDirect3DDevice9 **device)
{
	if(ppD3D!=NULL){
		return (ppD3D)->lpVtbl->CreateDevice(ppD3D, adapter, device_type, focus_window, flags, parameters, device);
	}	
	
	return D3DERR_INVALIDCALL;
}

static UINT WINAPI d3d9_GetAdapterModeCountEx(IDirect3D9Ex *iface,
        UINT adapter, const D3DDISPLAYMODEFILTER *filter)
{
    UINT AdapterModeCount;
	
	if(ppD3D!=NULL){
		return IDirect3D9_GetAdapterModeCount(ppD3D,adapter,filter->Format);
	}

	return D3DERR_INVALIDCALL;
}

static HRESULT WINAPI d3d9_EnumAdapterModesEx(IDirect3D9Ex *iface,
        UINT adapter, const D3DDISPLAYMODEFILTER *filter, UINT mode_idx, D3DDISPLAYMODEEX *mode)
{
	D3DDISPLAYMODE pMode;
	HRESULT ret;
	//DEVMODEW devMode;
	
	//EnumDisplaySettingsExW(adapter->DeviceName, j++, &devMode, 0))
	
	if(ppD3D!=NULL){
		ret = (ppD3D)->lpVtbl->EnumAdapterModes(ppD3D,adapter,filter->Format, mode_idx, &pMode);
		if(ret == D3D_OK)
		{
			mode->Size = sizeof(D3DDISPLAYMODEEX);
			mode->Width = pMode.Width;
			mode->Height = pMode.Height;
			mode->RefreshRate = pMode.RefreshRate;
			mode->Format = pMode.Format;
			mode->ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE; //HardCoded for now, need call EnumDisplaySettingsExW
			return ret;
		}
	}
	
	return D3DERR_INVALIDCALL; 
}

static HRESULT WINAPI d3d9_GetAdapterDisplayModeEx(IDirect3D9Ex *iface,
        UINT adapter, D3DDISPLAYMODEEX *mode, D3DDISPLAYROTATION *rotation)
{
	D3DDISPLAYMODE pMode;
	HRESULT ret;
	//DEVMODEW devMode;
	
	//EnumDisplaySettingsExW(adapter->DeviceName, j++, &devMode, 0))
	
	if(ppD3D!=NULL){
		ret = (ppD3D)->lpVtbl->GetAdapterDisplayMode(ppD3D, adapter, &pMode);
		if(ret == D3D_OK)
		{
			mode->Size = sizeof(D3DDISPLAYMODEEX);
			mode->Width = pMode.Width;
			mode->Height = pMode.Height;
			mode->RefreshRate = pMode.RefreshRate;
			mode->Format = pMode.Format;
			mode->ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE; //HardCoded for now, need call EnumDisplaySettingsExW
			
			*rotation = D3DDISPLAYROTATION_IDENTITY;
			return ret;
		}
	}
	return D3DERR_INVALIDCALL;
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_CreateDeviceEx(IDirect3D9Ex *iface,
        UINT adapter, D3DDEVTYPE device_type, HWND focus_window, DWORD flags,
        D3DPRESENT_PARAMETERS *parameters, D3DDISPLAYMODEEX *mode, IDirect3DDevice9Ex **device)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct d3d9_device *object;
    HRESULT hr;
	
	(ppD3D)->lpVtbl->CreateDevice(ppD3D, adapter, device_type, focus_window, flags, parameters, &ppDevice3D);
    TRACE("iface %p, adapter %u, device_type %#x, focus_window %p, flags %#x, parameters %p, mode %p, device %p.\n",
            iface, adapter, device_type, focus_window, flags, parameters, mode, device);

    if (!(object = heap_alloc_zero(sizeof(*object))))
        return E_OUTOFMEMORY;

    hr = device_init(object, d3d9, d3d9->wined3d, adapter, device_type, focus_window, flags, parameters, mode);
    if (FAILED(hr))
    {
        WARN("Failed to initialize device, hr %#x.\n", hr);
        heap_free(object);
        return hr;
    }

    TRACE("Created device %p.\n", object);
    *device = &object->IDirect3DDevice9Ex_iface;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_GetAdapterLUID(IDirect3D9Ex *iface, UINT adapter, LUID *luid)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct wined3d_adapter_identifier adapter_id;
    HRESULT hr;

    TRACE("iface %p, adapter %u, luid %p.\n", iface, adapter, luid);

    adapter_id.driver_size = 0;
    adapter_id.description_size = 0;
    adapter_id.device_name_size = 0;

    wined3d_mutex_lock();
    hr = wined3d_get_adapter_identifier(d3d9->wined3d, adapter, 0, &adapter_id);
    wined3d_mutex_unlock();

    memcpy(luid, &adapter_id.adapter_luid, sizeof(*luid));

    return hr;
}

static const struct IDirect3D9ExVtbl d3d9_vtbl =
{
    /* IUnknown */
    d3d9_QueryInterface,
    d3d9_AddRef,
    d3d9_Release,
    /* IDirect3D9 */
    d3d9_RegisterSoftwareDevice,
    d3d9_GetAdapterCount,
    d3d9_GetAdapterIdentifier,
    d3d9_GetAdapterModeCount,
    d3d9_EnumAdapterModes,
    d3d9_GetAdapterDisplayMode,
    d3d9_CheckDeviceType,
    d3d9_CheckDeviceFormat,
    d3d9_CheckDeviceMultiSampleType,
    d3d9_CheckDepthStencilMatch,
    d3d9_CheckDeviceFormatConversion,
    d3d9_GetDeviceCaps,
    d3d9_GetAdapterMonitor,
    d3d9_CreateDevice,
    /* IDirect3D9Ex */
    d3d9_GetAdapterModeCountEx,
    d3d9_EnumAdapterModesEx,
    d3d9_GetAdapterDisplayModeEx,
    d3d9_CreateDeviceEx,
    d3d9_GetAdapterLUID,
};

BOOL d3d9_init(struct d3d9 *d3d9, BOOL extended)
{
    DWORD flags = WINED3D_PRESENT_CONVERSION | WINED3D_HANDLE_RESTORE | WINED3D_PIXEL_CENTER_INTEGER
            | WINED3D_SRGB_READ_WRITE_CONTROL | WINED3D_LEGACY_UNBOUND_RESOURCE_COLOR
            | WINED3D_NO_PRIMITIVE_RESTART | WINED3D_LEGACY_CUBEMAP_FILTERING
            | WINED3D_NORMALIZED_DEPTH_BIAS;

    if (!extended)
        flags |= WINED3D_VIDMEM_ACCOUNTING;
    else
        flags |= WINED3D_RESTORE_MODE_ON_ACTIVATE;

    d3d9->IDirect3D9Ex_iface.lpVtbl = &d3d9_vtbl;
    d3d9->refcount = 1;

    wined3d_mutex_lock();
    d3d9->wined3d = wined3d_create(flags);
    wined3d_mutex_unlock();
    if (!d3d9->wined3d)
        return FALSE;
    d3d9->extended = extended;

    return TRUE;
}