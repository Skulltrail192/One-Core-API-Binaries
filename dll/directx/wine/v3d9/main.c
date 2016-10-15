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

#include <winbase.h>
#include <ntsecapi.h>
#include "d3d9.h"

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

struct d3d9
{
    IDirect3D9Ex IDirect3D9Ex_iface;
    LONG refcount;
    struct wined3d *wined3d;
    BOOL extended;
};

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

HRESULT WINAPI Direct3DCreate9Ex(
  _In_   UINT SDKVersion,
  _Out_  IDirect3D9Ex **ppD3DEx
)
{
    struct d3d9 *object;
	IDirect3D9* ppD3D;

    // TRACE("sdk_version %#x, d3d9ex %p.\n", SDKVersion, ppD3D);

    // if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        // return E_OUTOFMEMORY;

    // if (!d3d9_init(object, TRUE))
    // {
        // WARN("Failed to initialize d3d9.\n");
        // HeapFree(GetProcessHeap(), 0, object);
        // return D3DERR_NOTAVAILABLE;
    // }

    // TRACE("Created d3d9 object %p.\n", object);
    // *ppD3D = &object->IDirect3D9Ex_iface;

	*ppD3D = (IDirect3D9Ex)&Direct3DCreate9(SDKVersion);
	
    return D3D_OK;
}