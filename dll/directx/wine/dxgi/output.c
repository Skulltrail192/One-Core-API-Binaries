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
 */

#include "config.h"
#include "wine/port.h"

#include <windef.h>
#include <winbase.h>
#include <wingdi.h>

#include "dxgi_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(dxgi);

static void dxgi_mode_from_wined3d(DXGI_MODE_DESC *mode, const struct wined3d_display_mode *wined3d_mode)
{
    mode->Width = wined3d_mode->width;
    mode->Height = wined3d_mode->height;
    mode->RefreshRate.Numerator = wined3d_mode->refresh_rate;
    mode->RefreshRate.Denominator = 1;
    mode->Format = dxgi_format_from_wined3dformat(wined3d_mode->format_id);
    mode->ScanlineOrdering = wined3d_mode->scanline_ordering;
    mode->Scaling = DXGI_MODE_SCALING_UNSPECIFIED; /* DbgPrint */
}

static inline struct dxgi_output *impl_from_IDXGIOutput4(IDXGIOutput4 *iface)
{
    return CONTAINING_RECORD(iface, struct dxgi_output, IDXGIOutput4_iface);
}

/* IUnknown methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_QueryInterface(IDXGIOutput4 *iface, REFIID riid, void **object)
{
    DbgPrint("iface %p, riid %s, object %p.\n", iface, debugstr_guid(riid), object);

    if (IsEqualGUID(riid, &IID_IDXGIOutput)
            || IsEqualGUID(riid, &IID_IDXGIObject)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        IUnknown_AddRef(iface);
        *object = iface;
        return S_OK;
    }

    DbgPrint("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(riid));

    *object = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE dxgi_output_AddRef(IDXGIOutput4 *iface)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);
    ULONG refcount = InterlockedIncrement(&output->refcount);

    DbgPrint("%p increasing refcount to %u.\n", output, refcount);

    return refcount;
}

static ULONG STDMETHODCALLTYPE dxgi_output_Release(IDXGIOutput4 *iface)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);
    ULONG refcount = InterlockedDecrement(&output->refcount);

    DbgPrint("%p decreasing refcount to %u.\n", output, refcount);

    if (!refcount)
    {
        wined3d_private_store_cleanup(&output->private_store);
        IWineDXGIAdapter_Release(&output->adapter->IWineDXGIAdapter_iface);
        heap_free(output);
    }

    return refcount;
}

/* IDXGIObject methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_SetPrivateData(IDXGIOutput4 *iface,
        REFGUID guid, UINT data_size, const void *data)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);

    DbgPrint("iface %p, guid %s, data_size %u, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return dxgi_set_private_data(&output->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE dxgi_output_SetPrivateDataInterface(IDXGIOutput4 *iface,
        REFGUID guid, const IUnknown *object)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);

    DbgPrint("iface %p, guid %s, object %p.\n", iface, debugstr_guid(guid), object);

    return dxgi_set_private_data_interface(&output->private_store, guid, object);
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetPrivateData(IDXGIOutput4 *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);

    DbgPrint("iface %p, guid %s, data_size %p, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return dxgi_get_private_data(&output->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetParent(IDXGIOutput4 *iface,
        REFIID riid, void **parent)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);

    DbgPrint("iface %p, riid %s, parent %p.\n", iface, debugstr_guid(riid), parent);

    return IWineDXGIAdapter_QueryInterface(&output->adapter->IWineDXGIAdapter_iface, riid, parent);
}

/* IDXGIOutput methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_GetDesc(IDXGIOutput4 *iface, DXGI_OUTPUT_DESC *desc)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);
    struct wined3d_output_desc wined3d_desc;
    HRESULT hr;

    DbgPrint("iface %p, desc %p.\n", iface, desc);

    if (!desc)
        return E_INVALIDARG;

    wined3d_mutex_lock();
    hr = wined3d_get_output_desc(output->adapter->factory->wined3d,
            output->adapter->ordinal, &wined3d_desc);
    wined3d_mutex_unlock();

    if (FAILED(hr))
    {
        DbgPrint("Failed to get output desc, hr %#x.\n", hr);
        return hr;
    }

    memcpy(desc->DeviceName, wined3d_desc.device_name, sizeof(desc->DeviceName));
    desc->DesktopCoordinates = wined3d_desc.desktop_rect;
    desc->AttachedToDesktop = wined3d_desc.attached_to_desktop;
    desc->Rotation = wined3d_desc.rotation;
    desc->Monitor = wined3d_desc.monitor;

    return S_OK;
}

size_t GetFormatBpp(DXGI_FORMAT fmt)
{
    switch ((int)fmt)
    {
    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
    case DXGI_FORMAT_R32G32B32A32_UINT:
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return 128;

    case DXGI_FORMAT_R32G32B32_TYPELESS:
    case DXGI_FORMAT_R32G32B32_FLOAT:
    case DXGI_FORMAT_R32G32B32_UINT:
    case DXGI_FORMAT_R32G32B32_SINT:
        return 96;

    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
    case DXGI_FORMAT_R16G16B16A16_UNORM:
    case DXGI_FORMAT_R16G16B16A16_UINT:
    case DXGI_FORMAT_R16G16B16A16_SNORM:
    case DXGI_FORMAT_R16G16B16A16_SINT:
    case DXGI_FORMAT_R32G32_TYPELESS:
    case DXGI_FORMAT_R32G32_FLOAT:
    case DXGI_FORMAT_R32G32_UINT:
    case DXGI_FORMAT_R32G32_SINT:
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    case DXGI_FORMAT_Y416:
    case DXGI_FORMAT_Y210:
    case DXGI_FORMAT_Y216:
        return 64;

    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    case DXGI_FORMAT_R10G10B10A2_UNORM:
    case DXGI_FORMAT_R10G10B10A2_UINT:
    case DXGI_FORMAT_R11G11B10_FLOAT:
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UINT:
    case DXGI_FORMAT_R8G8B8A8_SNORM:
    case DXGI_FORMAT_R8G8B8A8_SINT:
    case DXGI_FORMAT_R16G16_TYPELESS:
    case DXGI_FORMAT_R16G16_FLOAT:
    case DXGI_FORMAT_R16G16_UNORM:
    case DXGI_FORMAT_R16G16_UINT:
    case DXGI_FORMAT_R16G16_SNORM:
    case DXGI_FORMAT_R16G16_SINT:
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R32_SINT:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
    case DXGI_FORMAT_B8G8R8X8_UNORM:
    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
    case DXGI_FORMAT_AYUV:
    case DXGI_FORMAT_Y410:
    case DXGI_FORMAT_YUY2:
    // case XBOX_DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
    // case XBOX_DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
    // case XBOX_DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM:
        return 32;

    case DXGI_FORMAT_P010:
    case DXGI_FORMAT_P016:
    // case XBOX_DXGI_FORMAT_D16_UNORM_S8_UINT:
    // case XBOX_DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
    // case XBOX_DXGI_FORMAT_X16_TYPELESS_G8_UINT:
    // case WIN10_DXGI_FORMAT_V408:
        return 24;

    case DXGI_FORMAT_R8G8_TYPELESS:
    case DXGI_FORMAT_R8G8_UNORM:
    case DXGI_FORMAT_R8G8_UINT:
    case DXGI_FORMAT_R8G8_SNORM:
    case DXGI_FORMAT_R8G8_SINT:
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_R16_FLOAT:
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_UNORM:
    case DXGI_FORMAT_R16_UINT:
    case DXGI_FORMAT_R16_SNORM:
    case DXGI_FORMAT_R16_SINT:
    case DXGI_FORMAT_B5G6R5_UNORM:
    case DXGI_FORMAT_B5G5R5A1_UNORM:
    case DXGI_FORMAT_A8P8:
    case DXGI_FORMAT_B4G4R4A4_UNORM:
    // case WIN10_DXGI_FORMAT_P208:
    // case WIN10_DXGI_FORMAT_V208:
        return 16;

    case DXGI_FORMAT_NV12:
    case DXGI_FORMAT_420_OPAQUE:
    case DXGI_FORMAT_NV11:
        return 12;

    case DXGI_FORMAT_R8_TYPELESS:
    case DXGI_FORMAT_R8_UNORM:
    case DXGI_FORMAT_R8_UINT:
    case DXGI_FORMAT_R8_SNORM:
    case DXGI_FORMAT_R8_SINT:
    case DXGI_FORMAT_A8_UNORM:
    case DXGI_FORMAT_AI44:
    case DXGI_FORMAT_IA44:
    case DXGI_FORMAT_P8:
    //case XBOX_DXGI_FORMAT_R4G4_UNORM:
        return 8;

    case DXGI_FORMAT_R1_UNORM:
        return 1;

    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
        return 4;

    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
    case DXGI_FORMAT_BC6H_TYPELESS:
    case DXGI_FORMAT_BC6H_UF16:
    case DXGI_FORMAT_BC6H_SF16:
    case DXGI_FORMAT_BC7_TYPELESS:
    case DXGI_FORMAT_BC7_UNORM:
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        return 8;

    default:
        return 0;
    }
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetDisplayModeList(
	IDXGIOutput4 *iface,
    DXGI_FORMAT EnumFormat, 
	UINT flags, 
	UINT *pNumModes, 
	DXGI_MODE_DESC *pDesc
)
{
    //struct dxgi_output *output = impl_from_IDXGIOutput4(iface);
    enum wined3d_format_id wined3d_format;
    unsigned int i, max_count;
    struct wined3d *wined3d;
	// Walk over all modes that the display supports and
    // return those that match the requested format etc.
	DEVMODEW devMode;	
    UINT srcModeId = 0;
	UINT dstModeId = 0;	
	DXGI_MODE_DESC *mode;
	
	mode = pDesc;
	
    DbgPrint("iface %p, format %s, flags %#x, mode_count %p, desc %p partial stub!\n",
            iface, debug_dxgi_format(EnumFormat), flags, pNumModes, pDesc);

    if (pNumModes == NULL)
        return DXGI_ERROR_INVALID_CALL;
	
    if (EnumFormat == DXGI_FORMAT_UNKNOWN)
    {
        *pNumModes = 0;
        return S_OK;
    }
	
	DbgPrint("Trying retrieve modes\n");
	
	while (EnumDisplaySettingsW(NULL, srcModeId++, &devMode)) {
		  DbgPrint("Enumrating Display Settings phase 1\n");	
		  // Skip interlaced modes altogether
		  if (devMode.dmDisplayFlags & DM_INTERLACED)
			continue;
		  
		  // Skip modes with incompatible formats
		  if (devMode.dmBitsPerPel != GetFormatBpp(EnumFormat))
			continue;		  
		  
		  DbgPrint("Enumrating Display Settings phase 2\n");	
		  if (pDesc != NULL) {
			DXGI_RATIONAL rate;
			
			DbgPrint("Enumrating Display Settings phase 3\n");	
			
			rate.Numerator = devMode.dmDisplayFrequency * 1000;
			rate.Denominator = 1000;
			
			DbgPrint("DEVMODEW Width %d\n", devMode.dmPelsWidth);	
			
			DbgPrint("DEVMODEW Height %d\n", devMode.dmPelsHeight);	
			
			mode->Width            = devMode.dmPelsWidth;
			mode->Height           = devMode.dmPelsHeight;
			mode->RefreshRate      = rate;
			mode->Format           = EnumFormat;
			mode->ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
			mode->Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;
			
			DbgPrint("Enumrating Display Settings phase 4\n");
			
			mode++;
			DbgPrint("Enumrating Display Settings phase 5\n");
		  }
		  
		  DbgPrint("Retrieving Display Modes\n");
		  
		  dstModeId += 1;
	}
	
	    // If requested, write out the first set of display
    // modes to the destination array.
    if (pDesc != NULL) {
	  DbgPrint("pDesc is not null\n");	
      for (i = 0; i < *pNumModes && i < dstModeId; i++)
        pDesc[i] = mode[i];
      
      if (dstModeId > *pNumModes)
        return DXGI_ERROR_MORE_DATA;
    }
    
	*pNumModes = dstModeId;

    // wined3d = output->adapter->factory->wined3d;
    // wined3d_format = wined3dformat_from_dxgi_format(format);

    // wined3d_mutex_lock();
    // max_count = wined3d_get_adapter_mode_count(wined3d, output->adapter->ordinal,
            // wined3d_format, WINED3D_SCANLINE_ORDERING_UNKNOWN);

    // if (!desc)
    // {
        // wined3d_mutex_unlock();
        // *mode_count = max_count;
        // return S_OK;
    // }

    // if (max_count > *mode_count)
    // {
        // wined3d_mutex_unlock();
        // return DXGI_ERROR_MORE_DATA;
    // }

    // *mode_count = max_count;

    // for (i = 0; i < *mode_count; ++i)
    // {
        // struct wined3d_display_mode mode;
        // HRESULT hr;

        // hr = wined3d_enum_adapter_modes(wined3d, output->adapter->ordinal, wined3d_format,
                // WINED3D_SCANLINE_ORDERING_UNKNOWN, i, &mode);
        // if (FAILED(hr))
        // {
            // DbgPrint("EnumAdapterModes failed, hr %#x.\n", hr);
            // wined3d_mutex_unlock();
            // return hr;
        // }

        // dxgi_mode_from_wined3d(&desc[i], &mode);
    // }
    // wined3d_mutex_unlock();

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_FindClosestMatchingMode(IDXGIOutput4 *iface,
        const DXGI_MODE_DESC *mode, DXGI_MODE_DESC *closest_match, IUnknown *device)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);
    struct wined3d_display_mode wined3d_mode;
    struct dxgi_adapter *adapter;
    struct wined3d *wined3d;
    HRESULT hr;

    DbgPrint("iface %p, mode %p, closest_match %p, device %p.\n", iface, mode, closest_match, device);

    if ((!mode->Width && mode->Height) || (mode->Width && !mode->Height))
        return DXGI_ERROR_INVALID_CALL;

    if (mode->Format == DXGI_FORMAT_UNKNOWN && !device)
        return DXGI_ERROR_INVALID_CALL;

    DbgPrint("Mode: %s.\n", debug_dxgi_mode(mode));
    if (mode->Format == DXGI_FORMAT_UNKNOWN)
    {
        DbgPrint("Matching formats to device not implemented.\n");
        return E_NOTIMPL;
    }

    adapter = output->adapter;
    wined3d = adapter->factory->wined3d;

    wined3d_mutex_lock();
    wined3d_display_mode_from_dxgi(&wined3d_mode, mode);
    hr = wined3d_find_closest_matching_adapter_mode(wined3d, adapter->ordinal, &wined3d_mode);
    wined3d_mutex_unlock();

    if (SUCCEEDED(hr))
    {
        dxgi_mode_from_wined3d(closest_match, &wined3d_mode);
        DbgPrint("Returning %s.\n", debug_dxgi_mode(closest_match));
    }

    return hr;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_WaitForVBlank(IDXGIOutput4 *iface)
{
    static BOOL once = FALSE;

    if (!once++)
        DbgPrint("iface %p stub!\n", iface);
    else
        DbgPrint("iface %p stub!\n", iface);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_TakeOwnership(IDXGIOutput4 *iface, IUnknown *device, BOOL exclusive)
{
    DbgPrint("iface %p, device %p, exclusive %d stub!\n", iface, device, exclusive);

    return E_NOTIMPL;
}

static void STDMETHODCALLTYPE dxgi_output_ReleaseOwnership(IDXGIOutput4 *iface)
{
    DbgPrint("iface %p stub!\n", iface);
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetGammaControlCapabilities(IDXGIOutput4 *iface,
        DXGI_GAMMA_CONTROL_CAPABILITIES *gamma_caps)
{
    int i;

    DbgPrint("iface %p, gamma_caps %p.\n", iface, gamma_caps);

    if (!gamma_caps)
        return E_INVALIDARG;

    gamma_caps->ScaleAndOffsetSupported = FALSE;
    gamma_caps->MaxConvertedValue = 1.0;
    gamma_caps->MinConvertedValue = 0.0;
    gamma_caps->NumGammaControlPoints = 256;

    for (i = 0; i < 256; i++)
        gamma_caps->ControlPointPositions[i] = i / 255.0f;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_SetGammaControl(IDXGIOutput4 *iface,
        const DXGI_GAMMA_CONTROL *gamma_control)
{
    DbgPrint("iface %p, gamma_control %p stub!\n", iface, gamma_control);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetGammaControl(IDXGIOutput4 *iface, DXGI_GAMMA_CONTROL *gamma_control)
{
    DbgPrint("iface %p, gamma_control %p stub!\n", iface, gamma_control);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_SetDisplaySurface(IDXGIOutput4 *iface, IDXGISurface *surface)
{
    DbgPrint("iface %p, surface %p stub!\n", iface, surface);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetDisplaySurfaceData(IDXGIOutput4 *iface, IDXGISurface *surface)
{
    DbgPrint("iface %p, surface %p stub!\n", iface, surface);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetFrameStatistics(IDXGIOutput4 *iface, DXGI_FRAME_STATISTICS *stats)
{
    DbgPrint("iface %p, stats %p stub!\n", iface, stats);

    return E_NOTIMPL;
}

/* IDXGIOutput1 methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_GetDisplayModeList1(IDXGIOutput4 *iface,
        DXGI_FORMAT format, UINT flags, UINT *mode_count, DXGI_MODE_DESC1 *modes)
{
    DbgPrint("iface %p, format %#x, flags %#x, mode_count %p, modes %p stub!\n",
            iface, format, flags, mode_count, modes);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_FindClosestMatchingMode1(IDXGIOutput4 *iface,
        const DXGI_MODE_DESC1 *mode, DXGI_MODE_DESC1 *closest_match, IUnknown *device)
{
    DbgPrint("iface %p, mode %p, closest_match %p, device %p stub!\n",
            iface, mode, closest_match, device);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetDisplaySurfaceData1(IDXGIOutput4 *iface,
        IDXGIResource *resource)
{
    DbgPrint("iface %p, resource %p stub!\n", iface, resource);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_DuplicateOutput(IDXGIOutput4 *iface,
        IUnknown *device, IDXGIOutputDuplication **output_duplication)
{
    DbgPrint("iface %p, device %p, output_duplication %p stub!\n", iface, device, output_duplication);

    return E_NOTIMPL;
}

/* IDXGIOutput2 methods */

static BOOL STDMETHODCALLTYPE dxgi_output_SupportsOverlays(IDXGIOutput4 *iface)
{
    DbgPrint("iface %p stub!\n", iface);

    return FALSE;
}

/* IDXGIOutput3 methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_CheckOverlaySupport(IDXGIOutput4 *iface,
        DXGI_FORMAT format, IUnknown *device, UINT *flags)
{
    DbgPrint("iface %p, format %#x, device %p, flags %p stub!\n", iface, format, device, flags);

    return E_NOTIMPL;
}

/* IDXGIOutput4 methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_CheckOverlayColorSpaceSupport(IDXGIOutput4 *iface,
        DXGI_FORMAT format, DXGI_COLOR_SPACE_TYPE color_space, IUnknown *device, UINT *flags)
{
    DbgPrint("iface %p, format %#x, color_space %#x, device %p, flags %p stub!\n",
            iface, format, color_space, device, flags);

    return E_NOTIMPL;
}

static const struct IDXGIOutput4Vtbl dxgi_output_vtbl =
{
    dxgi_output_QueryInterface,
    dxgi_output_AddRef,
    dxgi_output_Release,
    /* IDXGIObject methods */
    dxgi_output_SetPrivateData,
    dxgi_output_SetPrivateDataInterface,
    dxgi_output_GetPrivateData,
    dxgi_output_GetParent,
    /* IDXGIOutput methods */
    dxgi_output_GetDesc,
    dxgi_output_GetDisplayModeList,
    dxgi_output_FindClosestMatchingMode,
    dxgi_output_WaitForVBlank,
    dxgi_output_TakeOwnership,
    dxgi_output_ReleaseOwnership,
    dxgi_output_GetGammaControlCapabilities,
    dxgi_output_SetGammaControl,
    dxgi_output_GetGammaControl,
    dxgi_output_SetDisplaySurface,
    dxgi_output_GetDisplaySurfaceData,
    dxgi_output_GetFrameStatistics,
    /* IDXGIOutput1 methods */
    dxgi_output_GetDisplayModeList1,
    dxgi_output_FindClosestMatchingMode1,
    dxgi_output_GetDisplaySurfaceData1,
    dxgi_output_DuplicateOutput,
    /* IDXGIOutput2 methods */
    dxgi_output_SupportsOverlays,
    /* IDXGIOutput3 methods */
    dxgi_output_CheckOverlaySupport,
    /* IDXGIOutput4 methods */
    dxgi_output_CheckOverlayColorSpaceSupport,
};

static void dxgi_output_init(struct dxgi_output *output, struct dxgi_adapter *adapter)
{
    output->IDXGIOutput4_iface.lpVtbl = &dxgi_output_vtbl;
    output->refcount = 1;
    wined3d_private_store_init(&output->private_store);
    output->adapter = adapter;
    IWineDXGIAdapter_AddRef(&output->adapter->IWineDXGIAdapter_iface);
}

HRESULT dxgi_output_create(struct dxgi_adapter *adapter, struct dxgi_output **output)
{
    if (!(*output = heap_alloc_zero(sizeof(**output))))
        return E_OUTOFMEMORY;

    dxgi_output_init(*output, adapter);
    return S_OK;
}
