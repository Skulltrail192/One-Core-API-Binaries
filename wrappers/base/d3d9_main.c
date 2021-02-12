/*
 * Direct3D 9
 *
 * Copyright 2002-2003 Jason Edmeades
 * Copyright 2002-2003 Raphael Junqueira
 * Copyright 2005 Oliver Stieber
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
#include "initguid.h"
#include "d3d9_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(d3d9);

IDirect3D9 *ppD3D;
IDirect3DDevice9 *ppDevice3D;

static int D3DPERF_event_level = 0;

HRESULT WINAPI DECLSPEC_HOTPATCH Direct3DCreate9Ex(UINT sdk_version, IDirect3D9Ex **d3d9ex)
{
    struct d3d9 *object;

    TRACE("sdk_version %#x, d3d9ex %p.\n", sdk_version, d3d9ex);
	
	ppD3D = Direct3DCreate9(sdk_version);

    if (!(object = heap_alloc_zero(sizeof(*object))))
        return E_OUTOFMEMORY;

    if (!d3d9_init(object, TRUE))
    {
        WARN("Failed to initialize d3d9.\n");
        heap_free(object);
        return D3DERR_NOTAVAILABLE;
    }

    TRACE("Created d3d9 object %p.\n", object);
    *d3d9ex = &object->IDirect3D9Ex_iface;

    return D3D_OK;
}

void d3d9_resource_cleanup(struct d3d9_resource *resource)
{
    wined3d_private_store_cleanup(&resource->private_store);
}

HRESULT d3d9_resource_free_private_data(struct d3d9_resource *resource, const GUID *guid)
{
    struct wined3d_private_data *entry;

    wined3d_mutex_lock();
    entry = wined3d_private_store_get_private_data(&resource->private_store, guid);
    if (!entry)
    {
        wined3d_mutex_unlock();
        return D3DERR_NOTFOUND;
    }

    wined3d_private_store_free_private_data(&resource->private_store, entry);
    wined3d_mutex_unlock();

    return D3D_OK;
}

HRESULT d3d9_resource_get_private_data(struct d3d9_resource *resource, const GUID *guid,
        void *data, DWORD *data_size)
{
    const struct wined3d_private_data *stored_data;
    DWORD size_in;
    HRESULT hr;

    wined3d_mutex_lock();
    stored_data = wined3d_private_store_get_private_data(&resource->private_store, guid);
    if (!stored_data)
    {
        hr = D3DERR_NOTFOUND;
        goto done;
    }

    size_in = *data_size;
    *data_size = stored_data->size;
    if (!data)
    {
        hr = D3D_OK;
        goto done;
    }
    if (size_in < stored_data->size)
    {
        hr = D3DERR_MOREDATA;
        goto done;
    }

    if (stored_data->flags & WINED3DSPD_IUNKNOWN)
        IUnknown_AddRef(stored_data->content.object);
    memcpy(data, stored_data->content.data, stored_data->size);
    hr = D3D_OK;

done:
    wined3d_mutex_unlock();
    return hr;
}

void d3d9_resource_init(struct d3d9_resource *resource)
{
    resource->refcount = 1;
    wined3d_private_store_init(&resource->private_store);
}

HRESULT d3d9_resource_set_private_data(struct d3d9_resource *resource, const GUID *guid,
        const void *data, DWORD data_size, DWORD flags)
{
    HRESULT hr;

    wined3d_mutex_lock();
    hr = wined3d_private_store_set_private_data(&resource->private_store, guid, data, data_size, flags);
    wined3d_mutex_unlock();
    return hr;
}
