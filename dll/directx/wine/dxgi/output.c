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

#include "dxgi_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(dxgi);

static void dxgi_mode_from_wined3d(DXGI_MODE_DESC *mode, const struct wined3d_display_mode *wined3d_mode)
{
	DbgPrint("IDXGIDevice::dxgi_mode_from_wined3d\n");	
    mode->Width = wined3d_mode->width;
    mode->Height = wined3d_mode->height;
    mode->RefreshRate.Numerator = wined3d_mode->refresh_rate;
    mode->RefreshRate.Denominator = 1;
    mode->Format = dxgi_format_from_wined3dformat(wined3d_mode->format_id);
    mode->ScanlineOrdering = wined3d_mode->scanline_ordering;
    mode->Scaling = DXGI_MODE_SCALING_UNSPECIFIED; /* FIXME */
}

static inline struct dxgi_output *impl_from_IDXGIOutput4(IDXGIOutput4 *iface)
{
    return CONTAINING_RECORD(iface, struct dxgi_output, IDXGIOutput4_iface);
}

/* IUnknown methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_QueryInterface(IDXGIOutput4 *iface, REFIID iid, void **object)
{
    DbgPrint("IDXGIDevice::dxgi_output_QueryInterface::iface %p, iid %s, object %p.\n", iface, debugstr_guid(iid), object);

    if (IsEqualGUID(iid, &IID_IDXGIOutput4)
            || IsEqualGUID(iid, &IID_IDXGIOutput3)
            || IsEqualGUID(iid, &IID_IDXGIOutput2)
            || IsEqualGUID(iid, &IID_IDXGIOutput1)
            || IsEqualGUID(iid, &IID_IDXGIOutput)
            || IsEqualGUID(iid, &IID_IDXGIObject)
            || IsEqualGUID(iid, &IID_IUnknown))
    {
        IUnknown_AddRef(iface);
        *object = iface;
        return S_OK;
    }

    DbgPrint("IDXGIDevice::dxgi_output_QueryInterface::%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(iid));

    *object = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE dxgi_output_AddRef(IDXGIOutput4 *iface)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);
    ULONG refcount = InterlockedIncrement(&output->refcount);

    DbgPrint("IDXGIDevice::dxgi_output_AddRef::%p increasing refcount to %u.\n", output, refcount);

    return refcount;
}

static ULONG STDMETHODCALLTYPE dxgi_output_Release(IDXGIOutput4 *iface)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);
    ULONG refcount = InterlockedDecrement(&output->refcount);

    DbgPrint("IDXGIDevice::dxgi_output_Release::%p decreasing refcount to %u.\n", output, refcount);

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

    DbgPrint("IDXGIDevice::dxgi_output_SetPrivateData::iface %p, guid %s, data_size %u, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return dxgi_set_private_data(&output->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE dxgi_output_SetPrivateDataInterface(IDXGIOutput4 *iface,
        REFGUID guid, const IUnknown *object)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);

    DbgPrint("IDXGIDevice::dxgi_output_SetPrivateDataInterface::iface %p, guid %s, object %p.\n", iface, debugstr_guid(guid), object);

    return dxgi_set_private_data_interface(&output->private_store, guid, object);
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetPrivateData(IDXGIOutput4 *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);

    DbgPrint("IDXGIDevice::dxgi_output_GetPrivateData::iface %p, guid %s, data_size %p, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return dxgi_get_private_data(&output->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetParent(IDXGIOutput4 *iface,
        REFIID riid, void **parent)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);

    DbgPrint("IDXGIDevice::dxgi_output_GetParent::iface %p, riid %s, parent %p.\n", iface, debugstr_guid(riid), parent);

    return IWineDXGIAdapter_QueryInterface(&output->adapter->IWineDXGIAdapter_iface, riid, parent);
}

/* IDXGIOutput methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_GetDesc(IDXGIOutput4 *iface, DXGI_OUTPUT_DESC *desc)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);
    struct wined3d_output_desc wined3d_desc;
    HRESULT hr;

    DbgPrint("IDXGIDevice::dxgi_output_GetDesc::iface %p, desc %p.\n", iface, desc);

    if (!desc)
        return E_INVALIDARG;

    wined3d_mutex_lock();
    hr = wined3d_get_output_desc(output->adapter->factory->wined3d,
            output->adapter->ordinal, &wined3d_desc);
    wined3d_mutex_unlock();

    if (FAILED(hr))
    {
        DbgPrint("IDXGIDevice::dxgi_output_GetDesc::Failed to get output desc, hr %#x.\n", hr);
        return hr;
    }

    memcpy(desc->DeviceName, wined3d_desc.device_name, sizeof(desc->DeviceName));
    desc->DesktopCoordinates = wined3d_desc.desktop_rect;
    desc->AttachedToDesktop = wined3d_desc.attached_to_desktop;
    desc->Rotation = wined3d_desc.rotation;
    desc->Monitor = wined3d_desc.monitor;

    return S_OK;
}

DXGI_FORMAT dxgi_format_from_wined3dformat(enum wined3d_format_id format)
{
    switch(format)
    {
        case D3DFMT_UNKNOWN   : return DXGI_FORMAT_UNKNOWN;
		case D3DFMT_R8G8B8: return DXGI_FORMAT_R8G8B8A8_UNORM;
        case D3DFMT_A8R8G8B8: return DXGI_FORMAT_B8G8R8A8_UNORM;
        case D3DFMT_X8R8G8B8: return DXGI_FORMAT_B8G8R8X8_UNORM;
        case D3DFMT_R5G6B5: return DXGI_FORMAT_B5G6R5_UNORM;
        //case D3DFMT_X1R5G5B5: return DXGI_FORMAT_R32G32B32A32_SINT;
        case D3DFMT_A1R5G5B5: return return DXGI_FORMAT_B5G5R5A1_UNORM;
        case D3DFMT_A4R4G4B4: return DXGI_FORMAT_B4G4R4A4_UNORM;
        //case D3DFMT_R3G3B2: return DXGI_FORMAT_R32G32B32_UINT;
        case D3DFMT_A8: return DXGI_FORMAT_A8_UNORM;
        //case D3DFMT_A8R3G3B2: return DXGI_FORMAT_R16G16B16A16_TYPELESS;
        //case D3DFMT_X4R4G4B4: return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case D3DFMT_A2B10G10R10: return DXGI_FORMAT_R10G10B10A2_UNORM;
        case D3DFMT_A8B8G8R8: return DXGI_FORMAT_R8G8B8A8_UNORM;
        //case D3DFMT_X8B8G8R8: return DXGI_FORMAT_R16G16B16A16_SNORM;
        case D3DFMT_G16R16: return DXGI_FORMAT_R16G16_UNORM;
        //case D3DFMT_A2R10G10B10: return DXGI_FORMAT_R32G32_TYPELESS;
        //case D3DFMT_A16B16G16R16: return DXGI_FORMAT_R32G32_FLOAT;
        //case D3DFMT_A8P8: return DXGI_FORMAT_R32G32_UINT;
        //case D3DFMT_P8: return DXGI_FORMAT_R32G32_SINT;
        //case D3DFMT_L8: return DXGI_FORMAT_R32G8X24_TYPELESS;
        case D3DFMT_A8L8: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        case D3DFMT_A4L4: return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        case D3DFMT_V8U8: return DXGI_FORMAT_R8G8_SNORM;
        //case D3DFMT_L6V5U5: return DXGI_FORMAT_R10G10B10A2_TYPELESS;
        //case D3DFMT_X8L8V8U8: return DXGI_FORMAT_R10G10B10A2_UNORM;
        case D3DFMT_Q8W8V8U8: return DXGI_FORMAT_R8G8B8A8_SNORM;
        case D3DFMT_V16U16: return DXGI_FORMAT_R16G16_SNORM;
        //case D3DFMT_A2W10V10U10: return DXGI_FORMAT_R8G8B8A8_TYPELESS;
        case D3DFMT_UYVY: return DXGI_FORMAT_R8G8B8A8_UNORM;
		    D3DFMT_UYVY                 = MAKEFOURCC('U', 'Y', 'V', 'Y'),
    // D3DFMT_R8G8_B8G8            = MAKEFOURCC('R', 'G', 'B', 'G'),
    // D3DFMT_YUY2                 = MAKEFOURCC('Y', 'U', 'Y', '2'),
    // D3DFMT_G8R8_G8B8            = MAKEFOURCC('G', 'R', 'G', 'B'),
    // D3DFMT_DXT1                 = MAKEFOURCC('D', 'X', 'T', '1'),
    // D3DFMT_DXT2                 = MAKEFOURCC('D', 'X', 'T', '2'),
    // D3DFMT_DXT3                 = MAKEFOURCC('D', 'X', 'T', '3'),
    // D3DFMT_DXT4                 = MAKEFOURCC('D', 'X', 'T', '4'),
    // D3DFMT_DXT5                 = MAKEFOURCC('D', 'X', 'T', '5'),
        case D3DFMT_D16_LOCKABLE: return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case D3DFMT_D32: return DXGI_FORMAT_D32_FLOAT;
        //case D3DFMT_D15S1: return DXGI_FORMAT_R8G8B8A8_SNORM;
        case D3DFMT_D24S8: return DXGI_FORMAT_D24_UNORM_S8_UINT;;
        //case D3DFMT_D24X8: return DXGI_FORMAT_R16G16_TYPELESS;
       // case D3DFMT_D24X4S4: return DXGI_FORMAT_R16G16_FLOAT;
        case D3DFMT_D16: return DXGI_FORMAT_D16_UNORM;
        case D3DFMT_D32F_LOCKABLE: return DXGI_FORMAT_D32_FLOAT;
        case D3DFMT_D24FS8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
    // D3DFMT_D32_LOCKABLE         = 84,
    // D3DFMT_S8_LOCKABLE          = 85,
        //case D3DFMT_L16: return DXGI_FORMAT_R16G16_SINT;
		//D3DFMT_VERTEXDATA
        case D3DFMT_INDEX16: return DXGI_FORMAT_R16_UINT;
        case D3DFMT_INDEX32: return DXGI_FORMAT_R32_UINT;
        case D3DFMT_Q16W16V16U16: return DXGI_FORMAT_R16G16B16A16_SNORM;
        //case D3DFMT_CxV8U8: return DXGI_FORMAT_R32_UINT;
        case WINED3DFMT_R32_SINT: return DXGI_FORMAT_R32_SINT;
        case WINED3DFMT_R24G8_TYPELESS: return DXGI_FORMAT_R24G8_TYPELESS;
        case WINED3DFMT_D24_UNORM_S8_UINT: return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case WINED3DFMT_R24_UNORM_X8_TYPELESS: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        case WINED3DFMT_X24_TYPELESS_G8_UINT: return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
        case WINED3DFMT_R8G8_TYPELESS: return DXGI_FORMAT_R8G8_TYPELESS;
        case WINED3DFMT_R8G8_UNORM: return DXGI_FORMAT_R8G8_UNORM;
        case WINED3DFMT_R8G8_UINT: return DXGI_FORMAT_R8G8_UINT;
        case WINED3DFMT_R8G8_SNORM: return DXGI_FORMAT_R8G8_SNORM;
        case WINED3DFMT_R8G8_SINT: return DXGI_FORMAT_R8G8_SINT;
        case WINED3DFMT_R16_TYPELESS: return DXGI_FORMAT_R16_TYPELESS;
        case WINED3DFMT_R16_FLOAT: return DXGI_FORMAT_R16_FLOAT;
        case WINED3DFMT_D16_UNORM: return DXGI_FORMAT_D16_UNORM;
        case WINED3DFMT_R16_UNORM: return DXGI_FORMAT_R16_UNORM;
        case WINED3DFMT_R16_UINT: return DXGI_FORMAT_R16_UINT;
        case WINED3DFMT_R16_SNORM: return DXGI_FORMAT_R16_SNORM;
        case WINED3DFMT_R16_SINT: return DXGI_FORMAT_R16_SINT;
        case WINED3DFMT_R8_TYPELESS: return DXGI_FORMAT_R8_TYPELESS;
        case WINED3DFMT_R8_UNORM: return DXGI_FORMAT_R8_UNORM;
        case WINED3DFMT_R8_UINT: return DXGI_FORMAT_R8_UINT;
        case WINED3DFMT_R8_SNORM: return DXGI_FORMAT_R8_SNORM;
        case WINED3DFMT_R8_SINT: return DXGI_FORMAT_R8_SINT;
        case WINED3DFMT_A8_UNORM: return DXGI_FORMAT_A8_UNORM;
        case WINED3DFMT_R1_UNORM: return DXGI_FORMAT_R1_UNORM;
        case WINED3DFMT_R9G9B9E5_SHAREDEXP: return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
        case WINED3DFMT_R8G8_B8G8_UNORM: return DXGI_FORMAT_R8G8_B8G8_UNORM;
        case WINED3DFMT_G8R8_G8B8_UNORM: return DXGI_FORMAT_G8R8_G8B8_UNORM;
        case WINED3DFMT_BC1_TYPELESS: return DXGI_FORMAT_BC1_TYPELESS;
        case WINED3DFMT_BC1_UNORM: return DXGI_FORMAT_BC1_UNORM;
        case WINED3DFMT_BC1_UNORM_SRGB: return DXGI_FORMAT_BC1_UNORM_SRGB;
        case WINED3DFMT_BC2_TYPELESS: return DXGI_FORMAT_BC2_TYPELESS;
        case WINED3DFMT_BC2_UNORM: return DXGI_FORMAT_BC2_UNORM;
        case WINED3DFMT_BC2_UNORM_SRGB: return DXGI_FORMAT_BC2_UNORM_SRGB;
        case WINED3DFMT_BC3_TYPELESS: return DXGI_FORMAT_BC3_TYPELESS;
        case WINED3DFMT_BC3_UNORM: return DXGI_FORMAT_BC3_UNORM;
        case WINED3DFMT_BC3_UNORM_SRGB: return DXGI_FORMAT_BC3_UNORM_SRGB;
        case WINED3DFMT_BC4_TYPELESS: return DXGI_FORMAT_BC4_TYPELESS;
        case WINED3DFMT_BC4_UNORM: return DXGI_FORMAT_BC4_UNORM;
        case WINED3DFMT_BC4_SNORM: return DXGI_FORMAT_BC4_SNORM;
        case WINED3DFMT_BC5_TYPELESS: return DXGI_FORMAT_BC5_TYPELESS;
        case WINED3DFMT_BC5_UNORM: return DXGI_FORMAT_BC5_UNORM;
        case WINED3DFMT_BC5_SNORM: return DXGI_FORMAT_BC5_SNORM;
        case WINED3DFMT_B5G6R5_UNORM: return DXGI_FORMAT_B5G6R5_UNORM;
        case WINED3DFMT_B5G5R5A1_UNORM: return DXGI_FORMAT_B5G5R5A1_UNORM;
        case WINED3DFMT_B8G8R8A8_UNORM: return DXGI_FORMAT_B8G8R8A8_UNORM;
        case WINED3DFMT_B8G8R8X8_UNORM: return DXGI_FORMAT_B8G8R8X8_UNORM;
        case WINED3DFMT_R10G10B10_XR_BIAS_A2_UNORM: return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
        case WINED3DFMT_B8G8R8A8_TYPELESS: return DXGI_FORMAT_B8G8R8A8_TYPELESS;
        case WINED3DFMT_B8G8R8A8_UNORM_SRGB: return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        case WINED3DFMT_B8G8R8X8_TYPELESS: return DXGI_FORMAT_B8G8R8X8_TYPELESS;
        case WINED3DFMT_B8G8R8X8_UNORM_SRGB: return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
        case WINED3DFMT_BC6H_TYPELESS: return DXGI_FORMAT_BC6H_TYPELESS;
        case WINED3DFMT_BC6H_UF16: return DXGI_FORMAT_BC6H_UF16;
        case WINED3DFMT_BC6H_SF16: return DXGI_FORMAT_BC6H_SF16;
        case WINED3DFMT_BC7_TYPELESS: return DXGI_FORMAT_BC7_TYPELESS;
        case WINED3DFMT_BC7_UNORM: return DXGI_FORMAT_BC7_UNORM;
        case WINED3DFMT_BC7_UNORM_SRGB: return DXGI_FORMAT_BC7_UNORM_SRGB;
        case WINED3DFMT_B4G4R4A4_UNORM: return DXGI_FORMAT_B4G4R4A4_UNORM;
        default:
            FIXME("Unhandled wined3d format %#x.\n", format);
            return DXGI_FORMAT_UNKNOWN;
    }
}


static HRESULT STDMETHODCALLTYPE dxgi_output_GetDisplayModeList(IDXGIOutput4 *iface,
        DXGI_FORMAT format, UINT flags, UINT *mode_count, DXGI_MODE_DESC *desc)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);
    enum wined3d_format_id wined3d_format;
    unsigned int i, max_count;
    struct wined3d *wined3d;	
	IDirect3D9* pD3D9 = NULL;
	D3DADAPTER_IDENTIFIER9 pIdentifier;
	LUID uid;
	ULONG i;
	
	pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );	
	
	hr = IDirect3D9_GetAdapterIdentifier(pD3D9, 0, 0, &pIdentifier );

    DbgPrint("IDXGIOutput::dxgi_output_GetDisplayModeList::iface %p, format %s, flags %#x, mode_count %p, desc %p partial stub!\n",
            iface, debug_dxgi_format(format), flags, mode_count, desc);

    if (!mode_count)
        return DXGI_ERROR_INVALID_CALL;

    if (format == DXGI_FORMAT_UNKNOWN)
    {
        *mode_count = 0;
        return S_OK;
    }

    wined3d = output->adapter->factory->wined3d;
    wined3d_format = wined3dformat_from_dxgi_format(format);

    wined3d_mutex_lock();
    max_count = wined3d_get_adapter_mode_count(wined3d, output->adapter->ordinal,
            wined3d_format, WINED3D_SCANLINE_ORDERING_UNKNOWN);

    if (!desc)
    {
        wined3d_mutex_unlock();
        *mode_count = max_count;
        return S_OK;
    }

    if (max_count > *mode_count)
    {
        wined3d_mutex_unlock();
        return DXGI_ERROR_MORE_DATA;
    }

    *mode_count = max_count;

    for (i = 0; i < *mode_count; ++i)
    {
        struct wined3d_display_mode mode;
        HRESULT hr;

        hr = wined3d_enum_adapter_modes(wined3d, output->adapter->ordinal, wined3d_format,
                WINED3D_SCANLINE_ORDERING_UNKNOWN, i, &mode);
        if (FAILED(hr))
        {
            DbgPrint("IDXGIDevice::dxgi_output_GetDisplayModeList::EnumAdapterModes failed, hr %#x.\n", hr);
            wined3d_mutex_unlock();
            return hr;
        }

        dxgi_mode_from_wined3d(&desc[i], &mode);
    }
    wined3d_mutex_unlock();

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

static HRESULT STDMETHODCALLTYPE dxgi_output_FindClosestMatchingMode(IDXGIOutput4 *iface,
        const DXGI_MODE_DESC *mode, DXGI_MODE_DESC *closest_match, IUnknown *device)
{
    struct dxgi_output *output = impl_from_IDXGIOutput4(iface);
    struct wined3d_display_mode wined3d_mode;
    struct dxgi_adapter *adapter;
    struct wined3d *wined3d;
    HRESULT hr;

    DbgPrint("IDXGIDevice::dxgi_output_FindClosestMatchingMode::iface %p, mode %p, closest_match %p, device %p.\n", iface, mode, closest_match, device);

    if ((!mode->Width && mode->Height) || (mode->Width && !mode->Height))
        return DXGI_ERROR_INVALID_CALL;

    if (mode->Format == DXGI_FORMAT_UNKNOWN && !device)
        return DXGI_ERROR_INVALID_CALL;

    DbgPrint("IDXGIDevice::dxgi_output_FindClosestMatchingMode::Mode: %s.\n", debug_dxgi_mode(mode));
    if (mode->Format == DXGI_FORMAT_UNKNOWN)
    {
        DbgPrint("IDXGIDevice::dxgi_output_FindClosestMatchingMode::Matching formats to device not implemented.\n");
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
        DbgPrint("IDXGIDevice::dxgi_output_FindClosestMatchingMode::Returning %s.\n", debug_dxgi_mode(closest_match));
    }

    return hr;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_WaitForVBlank(IDXGIOutput4 *iface)
{
    static BOOL once = FALSE;

    if (!once++)
        DbgPrint("IDXGIDevice::dxgi_output_WaitForVBlank::iface %p stub!\n", iface);
    else
        DbgPrint("IDXGIDevice::dxgi_output_WaitForVBlank::iface %p stub!\n", iface);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_TakeOwnership(IDXGIOutput4 *iface, IUnknown *device, BOOL exclusive)
{
    DbgPrint("IDXGIDevice::dxgi_output_TakeOwnership::iface %p, device %p, exclusive %d stub!\n", iface, device, exclusive);

    return S_OK;//return E_NOTIMPL;
}

static void STDMETHODCALLTYPE dxgi_output_ReleaseOwnership(IDXGIOutput4 *iface)
{
    DbgPrint("IDXGIDevice::dxgi_output_ReleaseOwnership::iface %p stub!\n", iface);
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetGammaControlCapabilities(IDXGIOutput4 *iface,
        DXGI_GAMMA_CONTROL_CAPABILITIES *gamma_caps)
{
    int i;

    DbgPrint("IDXGIDevice::dxgi_output_GetGammaControlCapabilities::iface %p, gamma_caps %p.\n", iface, gamma_caps);

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
    DbgPrint("IDXGIDevice::dxgi_output_SetGammaControl::iface %p, gamma_control %p stub!\n", iface, gamma_control);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetGammaControl(IDXGIOutput4 *iface, DXGI_GAMMA_CONTROL *gamma_control)
{
    DbgPrint("IDXGIDevice::dxgi_output_GetGammaControl::iface %p, gamma_control %p stub!\n", iface, gamma_control);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_SetDisplaySurface(IDXGIOutput4 *iface, IDXGISurface *surface)
{
    DbgPrint("IDXGIDevice::dxgi_output_SetDisplaySurface::iface %p, surface %p stub!\n", iface, surface);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetDisplaySurfaceData(IDXGIOutput4 *iface, IDXGISurface *surface)
{
    DbgPrint("IDXGIDevice::dxgi_output_GetDisplaySurfaceData::iface %p, surface %p stub!\n", iface, surface);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetFrameStatistics(IDXGIOutput4 *iface, DXGI_FRAME_STATISTICS *stats)
{
    DbgPrint("IDXGIDevice::dxgi_output_GetFrameStatistics::iface %p, stats %p stub!\n", iface, stats);

    return E_NOTIMPL;
}

/* IDXGIOutput1 methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_GetDisplayModeList1(IDXGIOutput4 *iface,
        DXGI_FORMAT format, UINT flags, UINT *mode_count, DXGI_MODE_DESC1 *modes)
{
    DbgPrint("IDXGIDevice::dxgi_output_GetDisplayModeList1::iface %p, format %#x, flags %#x, mode_count %p, modes %p stub!\n",
            iface, format, flags, mode_count, modes);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_FindClosestMatchingMode1(IDXGIOutput4 *iface,
        const DXGI_MODE_DESC1 *mode, DXGI_MODE_DESC1 *closest_match, IUnknown *device)
{
    DbgPrint("IDXGIDevice::dxgi_output_FindClosestMatchingMode1::iface %p, mode %p, closest_match %p, device %p stub!\n",
            iface, mode, closest_match, device);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_GetDisplaySurfaceData1(IDXGIOutput4 *iface,
        IDXGIResource *resource)
{
    DbgPrint("IDXGIDevice::dxgi_output_GetDisplaySurfaceData1::iface %p, resource %p stub!\n", iface, resource);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE dxgi_output_DuplicateOutput(IDXGIOutput4 *iface,
        IUnknown *device, IDXGIOutputDuplication **output_duplication)
{
    DbgPrint("IDXGIDevice::dxgi_output_DuplicateOutput::iface %p, device %p, output_duplication %p stub!\n", iface, device, output_duplication);

    return E_NOTIMPL;
}

/* IDXGIOutput2 methods */

static BOOL STDMETHODCALLTYPE dxgi_output_SupportsOverlays(IDXGIOutput4 *iface)
{
    DbgPrint("IDXGIDevice::dxgi_output_SupportsOverlays::iface %p stub!\n", iface);

    return FALSE;
}

/* IDXGIOutput3 methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_CheckOverlaySupport(IDXGIOutput4 *iface,
        DXGI_FORMAT format, IUnknown *device, UINT *flags)
{
    DbgPrint("IDXGIDevice::dxgi_output_CheckOverlaySupport::iface %p, format %#x, device %p, flags %p stub!\n", iface, format, device, flags);

    return E_NOTIMPL;
}

/* IDXGIOutput4 methods */

static HRESULT STDMETHODCALLTYPE dxgi_output_CheckOverlayColorSpaceSupport(IDXGIOutput4 *iface,
        DXGI_FORMAT format, DXGI_COLOR_SPACE_TYPE color_space, IUnknown *device, UINT *flags)
{
    DbgPrint("IDXGIDevice::dxgi_output_CheckOverlayColorSpaceSupport::iface %p, format %#x, color_space %#x, device %p, flags %p stub!\n",
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
    DbgPrint("IDXGIDevice::dxgi_output_init::!\n");
	
    output->IDXGIOutput4_iface.lpVtbl = &dxgi_output_vtbl;
    output->refcount = 1;
    wined3d_private_store_init(&output->private_store);
    output->adapter = adapter;
    IWineDXGIAdapter_AddRef(&output->adapter->IWineDXGIAdapter_iface);
}

HRESULT dxgi_output_create(struct dxgi_adapter *adapter, struct dxgi_output **output)
{
    DbgPrint("IDXGIDevice::dxgi_output_create::!\n");
	
    if (!(*output = heap_alloc_zero(sizeof(**output))))
        return E_OUTOFMEMORY;

    dxgi_output_init(*output, adapter);
    return S_OK;
}
