/*
 * Copyright 2008 Henri Verbeet for CodeWeavers
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

#include "config.h"
#include "wine/port.h"

#define DXGI_INIT_GUID
#include "dxgi_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(dxgi);

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, void *reserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(inst);
            break;

        case DLL_PROCESS_DETACH:
            if (!reserved)
                dxgi_main_cleanup();
            break;
    }

    return TRUE;
}

HRESULT WINAPI CreateDXGIFactory2(UINT flags, REFIID iid, void **factory)
{
    DbgPrint("DXGI::CreateDXGIFactory2::flags %#x, iid %s, factory %p.\n", flags, debugstr_guid(iid), factory);

    if (flags)
        DbgPrint("DXGI::CreateDXGIFactory2::Ignoring flags %#x.\n", flags);

    return dxgi_factory_create(iid, factory, TRUE);
}

HRESULT WINAPI CreateDXGIFactory1(REFIID iid, void **factory)
{
    DbgPrint("DXGI::CreateDXGIFactory1::iid %s, factory %p.\n", debugstr_guid(iid), factory);

    return dxgi_factory_create(iid, factory, TRUE);
}

HRESULT WINAPI CreateDXGIFactory(REFIID iid, void **factory)
{
    DbgPrint("DXGI::CreateDXGIFactory::iid %s, factory %p.\n", debugstr_guid(iid), factory);

    return dxgi_factory_create(iid, factory, FALSE);
}

HRESULT WINAPI DXGID3D10CreateDevice(HMODULE d3d10core, IDXGIFactory *factory, IDXGIAdapter *adapter,
        unsigned int flags, const D3D_FEATURE_LEVEL *feature_levels, unsigned int level_count, void **device)
{
    return S_OK;
}

HRESULT WINAPI DXGID3D10RegisterLayers(const struct dxgi_device_layer *layers, UINT layer_count)
{

    return S_OK;
}

HRESULT WINAPI DXGIGetDebugInterface1(UINT flags, REFIID iid, void **debug)
{
    DbgPrint("DXGI::DXGIGetDebugInterface1::flags %#x, iid %s, debug %p.\n", flags, debugstr_guid(iid), debug);

	DbgPrint("DXGI::DXGIGetDebugInterface1::Returning DXGI_ERROR_SDK_COMPONENT_MISSING.\n");
    return DXGI_ERROR_SDK_COMPONENT_MISSING;
}
