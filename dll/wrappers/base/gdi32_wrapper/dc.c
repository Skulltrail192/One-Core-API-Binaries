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

#include <main.h>

#define FIRST_GDI_HANDLE 32
#define MAX_GDI_HANDLES  16384

static struct gdi_handle_entry gdi_handles[MAX_GDI_HANDLES];
static struct gdi_handle_entry *next_free;
static struct gdi_handle_entry *next_unused = gdi_handles;
static LONG debug_count;
HMODULE gdi32_module = 0;

static CRITICAL_SECTION gdi_section;

static inline struct gdi_handle_entry *handle_entry( HGDIOBJ handle )
{
    unsigned int idx = LOWORD(handle) - FIRST_GDI_HANDLE;

    if (idx < MAX_GDI_HANDLES && gdi_handles[idx].type)
    {
        if (!HIWORD( handle ) || HIWORD( handle ) == gdi_handles[idx].generation)
            return &gdi_handles[idx];
    }
    if (handle) DbgPrint( "invalid handle %p\n", handle );
    return NULL;
}

static inline HGDIOBJ entry_to_handle( struct gdi_handle_entry *entry )
{
    unsigned int idx = entry - gdi_handles + FIRST_GDI_HANDLE;
    return LongToHandle( idx | (entry->generation << 16) );
}

/***********************************************************************
 *           release_dc_ptr
 */
void release_dc_ptr( DC *dc )
{
    LONG ref;

    dc->thread = 0;
    ref = InterlockedDecrement( &dc->refcount );
    ASSERT( ref >= 0 );
    if (ref) dc->thread = GetCurrentThreadId();  /* we still own it */
}

/***********************************************************************
 *           GDI_ReleaseObj
 *
 */
void GDI_ReleaseObj( HGDIOBJ handle )
{
    LeaveCriticalSection( &gdi_section );
}

/***********************************************************************
 *           get_any_obj_ptr
 *
 * Return a pointer to, and the type of, the GDI object
 * associated with the handle.
 * The object must be released with GDI_ReleaseObj.
 */
void *get_any_obj_ptr( HGDIOBJ handle, WORD *type )
{
    void *ptr = NULL;
    struct gdi_handle_entry *entry;

    EnterCriticalSection( &gdi_section );

    if ((entry = handle_entry( handle )))
    {
        ptr = entry->obj;
        *type = entry->type;
    }

    if (!ptr) LeaveCriticalSection( &gdi_section );
    return ptr;
}

static inline DC *get_dc_obj( HDC hdc )
{
    WORD type;
    DC *dc = get_any_obj_ptr( hdc, &type );
    if (!dc) return NULL;

    switch (type)
    {
    case OBJ_DC:
    case OBJ_MEMDC:
    case OBJ_METADC:
    case OBJ_ENHMETADC:
        return dc;
    default:
        GDI_ReleaseObj( hdc );
        SetLastError( ERROR_INVALID_HANDLE );
        return NULL;
    }
}

/***********************************************************************
 *           get_dc_ptr
 *
 * Retrieve a DC pointer but release the GDI lock.
 */
DC *get_dc_ptr( HDC hdc )
{
    DC *dc = get_dc_obj( hdc );
    if (!dc) return NULL;
    if (dc->disabled)
    {
        GDI_ReleaseObj( hdc );
        return NULL;
    }

    if (!InterlockedCompareExchange( &dc->refcount, 1, 0 ))
    {
        dc->thread = GetCurrentThreadId();
    }
    else if (dc->thread != GetCurrentThreadId())
    {
        DbgPrint( "dc %p belongs to thread %04x\n", hdc, dc->thread );
        GDI_ReleaseObj( hdc );
        return NULL;
    }
    else InterlockedIncrement( &dc->refcount );

    GDI_ReleaseObj( hdc );
    return dc;
}
