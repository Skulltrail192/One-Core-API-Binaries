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

#define MAX_GDI_HANDLES  16384
#define FIRST_GDI_HANDLE 32
#define MAX_FONT_HANDLES  256

WINE_DEFAULT_DEBUG_CHANNEL(gdi);

static CRITICAL_SECTION gdi_section;

typedef struct tagBITMAPOBJ
{
     DIBSECTION          dib;
     SIZE                size;   /* For SetBitmapDimension() */
     RGBQUAD            *color_table;  /* DIB color table if <= 8bpp (always 1 << bpp in size) */
} BITMAPOBJ;

struct font_handle_entry
{
    void *obj;
    WORD  generation; /* generation count for reusing handle values */
};

static struct font_handle_entry font_handles[MAX_FONT_HANDLES];
static struct font_handle_entry *next_free;
static struct font_handle_entry *next_unused = font_handles;
static struct gdi_handle_entry gdi_handles[MAX_GDI_HANDLES];

static LONG debug_count;

static inline int get_dib_stride( int width, int bpp )
{
    return ((width * bpp + 31) >> 3) & ~3;
}

const RGBQUAD *get_default_color_table( int bpp )
{
    static const RGBQUAD table_1[2] =
    {
        { 0x00, 0x00, 0x00 }, { 0xff, 0xff, 0xff }
    };
    static const RGBQUAD table_4[16] =
    {
        { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x80 }, { 0x00, 0x80, 0x00 }, { 0x00, 0x80, 0x80 },
        { 0x80, 0x00, 0x00 }, { 0x80, 0x00, 0x80 }, { 0x80, 0x80, 0x00 }, { 0x80, 0x80, 0x80 },
        { 0xc0, 0xc0, 0xc0 }, { 0x00, 0x00, 0xff }, { 0x00, 0xff, 0x00 }, { 0x00, 0xff, 0xff },
        { 0xff, 0x00, 0x00 }, { 0xff, 0x00, 0xff }, { 0xff, 0xff, 0x00 }, { 0xff, 0xff, 0xff },
    };
    static const RGBQUAD table_8[256] =
    {
        /* first and last 10 entries are the default system palette entries */
        { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x80 }, { 0x00, 0x80, 0x00 }, { 0x00, 0x80, 0x80 },
        { 0x80, 0x00, 0x00 }, { 0x80, 0x00, 0x80 }, { 0x80, 0x80, 0x00 }, { 0xc0, 0xc0, 0xc0 },
        { 0xc0, 0xdc, 0xc0 }, { 0xf0, 0xca, 0xa6 }, { 0x00, 0x20, 0x40 }, { 0x00, 0x20, 0x60 },
        { 0x00, 0x20, 0x80 }, { 0x00, 0x20, 0xa0 }, { 0x00, 0x20, 0xc0 }, { 0x00, 0x20, 0xe0 },
        { 0x00, 0x40, 0x00 }, { 0x00, 0x40, 0x20 }, { 0x00, 0x40, 0x40 }, { 0x00, 0x40, 0x60 },
        { 0x00, 0x40, 0x80 }, { 0x00, 0x40, 0xa0 }, { 0x00, 0x40, 0xc0 }, { 0x00, 0x40, 0xe0 },
        { 0x00, 0x60, 0x00 }, { 0x00, 0x60, 0x20 }, { 0x00, 0x60, 0x40 }, { 0x00, 0x60, 0x60 },
        { 0x00, 0x60, 0x80 }, { 0x00, 0x60, 0xa0 }, { 0x00, 0x60, 0xc0 }, { 0x00, 0x60, 0xe0 },
        { 0x00, 0x80, 0x00 }, { 0x00, 0x80, 0x20 }, { 0x00, 0x80, 0x40 }, { 0x00, 0x80, 0x60 },
        { 0x00, 0x80, 0x80 }, { 0x00, 0x80, 0xa0 }, { 0x00, 0x80, 0xc0 }, { 0x00, 0x80, 0xe0 },
        { 0x00, 0xa0, 0x00 }, { 0x00, 0xa0, 0x20 }, { 0x00, 0xa0, 0x40 }, { 0x00, 0xa0, 0x60 },
        { 0x00, 0xa0, 0x80 }, { 0x00, 0xa0, 0xa0 }, { 0x00, 0xa0, 0xc0 }, { 0x00, 0xa0, 0xe0 },
        { 0x00, 0xc0, 0x00 }, { 0x00, 0xc0, 0x20 }, { 0x00, 0xc0, 0x40 }, { 0x00, 0xc0, 0x60 },
        { 0x00, 0xc0, 0x80 }, { 0x00, 0xc0, 0xa0 }, { 0x00, 0xc0, 0xc0 }, { 0x00, 0xc0, 0xe0 },
        { 0x00, 0xe0, 0x00 }, { 0x00, 0xe0, 0x20 }, { 0x00, 0xe0, 0x40 }, { 0x00, 0xe0, 0x60 },
        { 0x00, 0xe0, 0x80 }, { 0x00, 0xe0, 0xa0 }, { 0x00, 0xe0, 0xc0 }, { 0x00, 0xe0, 0xe0 },
        { 0x40, 0x00, 0x00 }, { 0x40, 0x00, 0x20 }, { 0x40, 0x00, 0x40 }, { 0x40, 0x00, 0x60 },
        { 0x40, 0x00, 0x80 }, { 0x40, 0x00, 0xa0 }, { 0x40, 0x00, 0xc0 }, { 0x40, 0x00, 0xe0 },
        { 0x40, 0x20, 0x00 }, { 0x40, 0x20, 0x20 }, { 0x40, 0x20, 0x40 }, { 0x40, 0x20, 0x60 },
        { 0x40, 0x20, 0x80 }, { 0x40, 0x20, 0xa0 }, { 0x40, 0x20, 0xc0 }, { 0x40, 0x20, 0xe0 },
        { 0x40, 0x40, 0x00 }, { 0x40, 0x40, 0x20 }, { 0x40, 0x40, 0x40 }, { 0x40, 0x40, 0x60 },
        { 0x40, 0x40, 0x80 }, { 0x40, 0x40, 0xa0 }, { 0x40, 0x40, 0xc0 }, { 0x40, 0x40, 0xe0 },
        { 0x40, 0x60, 0x00 }, { 0x40, 0x60, 0x20 }, { 0x40, 0x60, 0x40 }, { 0x40, 0x60, 0x60 },
        { 0x40, 0x60, 0x80 }, { 0x40, 0x60, 0xa0 }, { 0x40, 0x60, 0xc0 }, { 0x40, 0x60, 0xe0 },
        { 0x40, 0x80, 0x00 }, { 0x40, 0x80, 0x20 }, { 0x40, 0x80, 0x40 }, { 0x40, 0x80, 0x60 },
        { 0x40, 0x80, 0x80 }, { 0x40, 0x80, 0xa0 }, { 0x40, 0x80, 0xc0 }, { 0x40, 0x80, 0xe0 },
        { 0x40, 0xa0, 0x00 }, { 0x40, 0xa0, 0x20 }, { 0x40, 0xa0, 0x40 }, { 0x40, 0xa0, 0x60 },
        { 0x40, 0xa0, 0x80 }, { 0x40, 0xa0, 0xa0 }, { 0x40, 0xa0, 0xc0 }, { 0x40, 0xa0, 0xe0 },
        { 0x40, 0xc0, 0x00 }, { 0x40, 0xc0, 0x20 }, { 0x40, 0xc0, 0x40 }, { 0x40, 0xc0, 0x60 },
        { 0x40, 0xc0, 0x80 }, { 0x40, 0xc0, 0xa0 }, { 0x40, 0xc0, 0xc0 }, { 0x40, 0xc0, 0xe0 },
        { 0x40, 0xe0, 0x00 }, { 0x40, 0xe0, 0x20 }, { 0x40, 0xe0, 0x40 }, { 0x40, 0xe0, 0x60 },
        { 0x40, 0xe0, 0x80 }, { 0x40, 0xe0, 0xa0 }, { 0x40, 0xe0, 0xc0 }, { 0x40, 0xe0, 0xe0 },
        { 0x80, 0x00, 0x00 }, { 0x80, 0x00, 0x20 }, { 0x80, 0x00, 0x40 }, { 0x80, 0x00, 0x60 },
        { 0x80, 0x00, 0x80 }, { 0x80, 0x00, 0xa0 }, { 0x80, 0x00, 0xc0 }, { 0x80, 0x00, 0xe0 },
        { 0x80, 0x20, 0x00 }, { 0x80, 0x20, 0x20 }, { 0x80, 0x20, 0x40 }, { 0x80, 0x20, 0x60 },
        { 0x80, 0x20, 0x80 }, { 0x80, 0x20, 0xa0 }, { 0x80, 0x20, 0xc0 }, { 0x80, 0x20, 0xe0 },
        { 0x80, 0x40, 0x00 }, { 0x80, 0x40, 0x20 }, { 0x80, 0x40, 0x40 }, { 0x80, 0x40, 0x60 },
        { 0x80, 0x40, 0x80 }, { 0x80, 0x40, 0xa0 }, { 0x80, 0x40, 0xc0 }, { 0x80, 0x40, 0xe0 },
        { 0x80, 0x60, 0x00 }, { 0x80, 0x60, 0x20 }, { 0x80, 0x60, 0x40 }, { 0x80, 0x60, 0x60 },
        { 0x80, 0x60, 0x80 }, { 0x80, 0x60, 0xa0 }, { 0x80, 0x60, 0xc0 }, { 0x80, 0x60, 0xe0 },
        { 0x80, 0x80, 0x00 }, { 0x80, 0x80, 0x20 }, { 0x80, 0x80, 0x40 }, { 0x80, 0x80, 0x60 },
        { 0x80, 0x80, 0x80 }, { 0x80, 0x80, 0xa0 }, { 0x80, 0x80, 0xc0 }, { 0x80, 0x80, 0xe0 },
        { 0x80, 0xa0, 0x00 }, { 0x80, 0xa0, 0x20 }, { 0x80, 0xa0, 0x40 }, { 0x80, 0xa0, 0x60 },
        { 0x80, 0xa0, 0x80 }, { 0x80, 0xa0, 0xa0 }, { 0x80, 0xa0, 0xc0 }, { 0x80, 0xa0, 0xe0 },
        { 0x80, 0xc0, 0x00 }, { 0x80, 0xc0, 0x20 }, { 0x80, 0xc0, 0x40 }, { 0x80, 0xc0, 0x60 },
        { 0x80, 0xc0, 0x80 }, { 0x80, 0xc0, 0xa0 }, { 0x80, 0xc0, 0xc0 }, { 0x80, 0xc0, 0xe0 },
        { 0x80, 0xe0, 0x00 }, { 0x80, 0xe0, 0x20 }, { 0x80, 0xe0, 0x40 }, { 0x80, 0xe0, 0x60 },
        { 0x80, 0xe0, 0x80 }, { 0x80, 0xe0, 0xa0 }, { 0x80, 0xe0, 0xc0 }, { 0x80, 0xe0, 0xe0 },
        { 0xc0, 0x00, 0x00 }, { 0xc0, 0x00, 0x20 }, { 0xc0, 0x00, 0x40 }, { 0xc0, 0x00, 0x60 },
        { 0xc0, 0x00, 0x80 }, { 0xc0, 0x00, 0xa0 }, { 0xc0, 0x00, 0xc0 }, { 0xc0, 0x00, 0xe0 },
        { 0xc0, 0x20, 0x00 }, { 0xc0, 0x20, 0x20 }, { 0xc0, 0x20, 0x40 }, { 0xc0, 0x20, 0x60 },
        { 0xc0, 0x20, 0x80 }, { 0xc0, 0x20, 0xa0 }, { 0xc0, 0x20, 0xc0 }, { 0xc0, 0x20, 0xe0 },
        { 0xc0, 0x40, 0x00 }, { 0xc0, 0x40, 0x20 }, { 0xc0, 0x40, 0x40 }, { 0xc0, 0x40, 0x60 },
        { 0xc0, 0x40, 0x80 }, { 0xc0, 0x40, 0xa0 }, { 0xc0, 0x40, 0xc0 }, { 0xc0, 0x40, 0xe0 },
        { 0xc0, 0x60, 0x00 }, { 0xc0, 0x60, 0x20 }, { 0xc0, 0x60, 0x40 }, { 0xc0, 0x60, 0x60 },
        { 0xc0, 0x60, 0x80 }, { 0xc0, 0x60, 0xa0 }, { 0xc0, 0x60, 0xc0 }, { 0xc0, 0x60, 0xe0 },
        { 0xc0, 0x80, 0x00 }, { 0xc0, 0x80, 0x20 }, { 0xc0, 0x80, 0x40 }, { 0xc0, 0x80, 0x60 },
        { 0xc0, 0x80, 0x80 }, { 0xc0, 0x80, 0xa0 }, { 0xc0, 0x80, 0xc0 }, { 0xc0, 0x80, 0xe0 },
        { 0xc0, 0xa0, 0x00 }, { 0xc0, 0xa0, 0x20 }, { 0xc0, 0xa0, 0x40 }, { 0xc0, 0xa0, 0x60 },
        { 0xc0, 0xa0, 0x80 }, { 0xc0, 0xa0, 0xa0 }, { 0xc0, 0xa0, 0xc0 }, { 0xc0, 0xa0, 0xe0 },
        { 0xc0, 0xc0, 0x00 }, { 0xc0, 0xc0, 0x20 }, { 0xc0, 0xc0, 0x40 }, { 0xc0, 0xc0, 0x60 },
        { 0xc0, 0xc0, 0x80 }, { 0xc0, 0xc0, 0xa0 }, { 0xf0, 0xfb, 0xff }, { 0xa4, 0xa0, 0xa0 },
        { 0x80, 0x80, 0x80 }, { 0x00, 0x00, 0xff }, { 0x00, 0xff, 0x00 }, { 0x00, 0xff, 0xff },
        { 0xff, 0x00, 0x00 }, { 0xff, 0x00, 0xff }, { 0xff, 0xff, 0x00 }, { 0xff, 0xff, 0xff },
    };

    switch (bpp)
    {
    case 1: return table_1;
    case 4: return table_4;
    case 8: return table_8;
    default: return NULL;
    }
}

static const char *gdi_obj_type( unsigned type )
{
    switch ( type )
    {
        case OBJ_PEN: return "OBJ_PEN";
        case OBJ_BRUSH: return "OBJ_BRUSH";
        case OBJ_DC: return "OBJ_DC";
        case OBJ_METADC: return "OBJ_METADC";
        case OBJ_PAL: return "OBJ_PAL";
        case OBJ_FONT: return "OBJ_FONT";
        case OBJ_BITMAP: return "OBJ_BITMAP";
        case OBJ_REGION: return "OBJ_REGION";
        case OBJ_METAFILE: return "OBJ_METAFILE";
        case OBJ_MEMDC: return "OBJ_MEMDC";
        case OBJ_EXTPEN: return "OBJ_EXTPEN";
        case OBJ_ENHMETADC: return "OBJ_ENHMETADC";
        case OBJ_ENHMETAFILE: return "OBJ_ENHMETAFILE";
        case OBJ_COLORSPACE: return "OBJ_COLORSPACE";
        default: return "UNKNOWN";
    }
}

NTSTATUS 
APIENTRY 
D3DKMTCloseAdapter(
  _In_ D3DKMT_CLOSEADAPTER *pData
)
{
	if(!pData->hAdapter)
	{
		return STATUS_INVALID_HANDLE;
	}
	pData->hAdapter = NULL;
	return STATUS_SUCCESS;
}

static inline HGDIOBJ entry_to_handle( struct gdi_handle_entry *entry )
{
    unsigned int idx = entry - gdi_handles + FIRST_GDI_HANDLE;
    return LongToHandle( idx | (entry->generation << 16) );
}

static void dump_gdi_objects( void )
{
    struct gdi_handle_entry *entry;

    TRACE( "%u objects:\n", MAX_GDI_HANDLES );

    EnterCriticalSection( &gdi_section );
    for (entry = gdi_handles; entry < next_unused; entry++)
    {
        if (!entry->type)
            TRACE( "handle %p FREE\n", entry_to_handle( entry ));
        else
            TRACE( "handle %p obj %p type %s selcount %u deleted %u\n",
                   entry_to_handle( entry ), entry->obj, gdi_obj_type( entry->type ),
                   entry->selcount, entry->deleted );
    }
    LeaveCriticalSection( &gdi_section );
}

struct gdi_obj_funcs
{
    HGDIOBJ (*pSelectObject)( HGDIOBJ handle, HDC hdc );
    INT     (*pGetObjectA)( HGDIOBJ handle, INT count, LPVOID buffer );
    INT     (*pGetObjectW)( HGDIOBJ handle, INT count, LPVOID buffer );
    BOOL    (*pUnrealizeObject)( HGDIOBJ handle );
    BOOL    (*pDeleteObject)( HGDIOBJ handle );
};

static HGDIOBJ DIB_SelectObject( HGDIOBJ handle, HDC hdc );
static INT DIB_GetObject( HGDIOBJ handle, INT count, LPVOID buffer );
static BOOL DIB_DeleteObject( HGDIOBJ handle );

static const struct gdi_obj_funcs dib_funcs =
{
    DIB_SelectObject,  /* pSelectObject */
    DIB_GetObject,     /* pGetObjectA */
    DIB_GetObject,     /* pGetObjectW */
    NULL,              /* pUnrealizeObject */
    DIB_DeleteObject   /* pDeleteObject */
};

void GDI_ReleaseObj( HGDIOBJ handle );
/***********************************************************************
 *           GDI_GetObjPtr
 *
 * Return a pointer to the GDI object associated with the handle.
 * Return NULL if the object has the wrong type.
 * The object must be released with GDI_ReleaseObj.
 */
void *GDI_GetObjPtr( HGDIOBJ handle, WORD type )
{
    WORD ret_type;
    void *ptr = get_any_obj_ptr( handle, &ret_type );
    if (ptr && ret_type != type)
    {
        GDI_ReleaseObj( handle );
        ptr = NULL;
    }
    return ptr;
}

static inline struct gdi_handle_entry *handle_entry( HGDIOBJ handle )
{
    unsigned int idx = LOWORD(handle) - FIRST_GDI_HANDLE;

    if (idx < MAX_GDI_HANDLES && gdi_handles[idx].type)
    {
        if (!HIWORD( handle ) || HIWORD( handle ) == gdi_handles[idx].generation)
            return &gdi_handles[idx];
    }
    if (handle) WARN( "invalid handle %p\n", handle );
    return NULL;
}

/***********************************************************************
 *           GDI_get_ref_count
 *
 * Retrieve the reference count of a GDI object.
 * Note: the object must be locked otherwise the count is meaningless.
 */
UINT GDI_get_ref_count( HGDIOBJ handle )
{
    struct gdi_handle_entry *entry;
    UINT ret = 0;

    EnterCriticalSection( &gdi_section );
    if ((entry = handle_entry( handle ))) ret = entry->selcount;
    LeaveCriticalSection( &gdi_section );
    return ret;
}


/***********************************************************************
 *           GDI_inc_ref_count
 *
 * Increment the reference count of a GDI object.
 */
HGDIOBJ GDI_inc_ref_count( HGDIOBJ handle )
{
    struct gdi_handle_entry *entry;

    EnterCriticalSection( &gdi_section );
    if ((entry = handle_entry( handle ))) entry->selcount++;
    else handle = 0;
    LeaveCriticalSection( &gdi_section );
    return handle;
}

/***********************************************************************
 *           GDI_dec_ref_count
 *
 * Decrement the reference count of a GDI object.
 */
BOOL GDI_dec_ref_count( HGDIOBJ handle )
{
    struct gdi_handle_entry *entry;

    EnterCriticalSection( &gdi_section );
    if ((entry = handle_entry( handle )))
    {
        ASSERT( entry->selcount );
        if (!--entry->selcount && entry->deleted)
        {
            /* handle delayed DeleteObject*/
            entry->deleted = 0;
            LeaveCriticalSection( &gdi_section );
            TRACE( "executing delayed DeleteObject for %p\n", handle );
            DeleteObject( handle );
            return TRUE;
        }
    }
    LeaveCriticalSection( &gdi_section );
    return entry != NULL;
}

/* Return the total DC region (if any) */
static inline HRGN get_dc_region( DC *dc )
{
    if (dc->region) return dc->region;
    if (dc->hVisRgn) return dc->hVisRgn;
    if (dc->hClipRgn) return dc->hClipRgn;
    return dc->hMetaRgn;
}

/***********************************************************************
 *           update_dc_clipping
 *
 * Update the DC and device clip regions when the ClipRgn or VisRgn have changed.
 */
void update_dc_clipping( DC * dc )
{
    PHYSDEV physdev = GET_DC_PHYSDEV( dc, pSetDeviceClipping );
    HRGN regions[3];
    int count = 0;

    if (dc->hVisRgn)  regions[count++] = dc->hVisRgn;
    if (dc->hClipRgn) regions[count++] = dc->hClipRgn;
    if (dc->hMetaRgn) regions[count++] = dc->hMetaRgn;

    if (count > 1)
    {
        if (!dc->region) dc->region = CreateRectRgn( 0, 0, 0, 0 );
        CombineRgn( dc->region, regions[0], regions[1], RGN_AND );
        if (count > 2) CombineRgn( dc->region, dc->region, regions[2], RGN_AND );
    }
    else  /* only one region, we don't need the total region */
    {
        if (dc->region) DeleteObject( dc->region );
        dc->region = 0;
    }
    physdev->funcs->pSetDeviceClipping( physdev, get_dc_region( dc ));
}

/***********************************************************************
 *           DIB_GetObject
 */
static INT DIB_GetObject( HGDIOBJ handle, INT count, LPVOID buffer )
{
    INT ret = 0;
    BITMAPOBJ *bmp = GDI_GetObjPtr( handle, OBJ_BITMAP );

    if (!bmp) return 0;

    if (!buffer) ret = sizeof(BITMAP);
    else if (count >= sizeof(DIBSECTION))
    {
        DIBSECTION *dib = buffer;
        *dib = bmp->dib;
        dib->dsBm.bmWidthBytes = get_dib_stride( dib->dsBm.bmWidth, dib->dsBm.bmBitsPixel );
        dib->dsBmih.biHeight = abs( dib->dsBmih.biHeight );
        ret = sizeof(DIBSECTION);
    }
    else if (count >= sizeof(BITMAP))
    {
        BITMAP *bitmap = buffer;
        *bitmap = bmp->dib.dsBm;
        bitmap->bmWidthBytes = get_dib_stride( bitmap->bmWidth, bitmap->bmBitsPixel );
        ret = sizeof(BITMAP);
    }

    GDI_ReleaseObj( handle );
    return ret;
}

/***********************************************************************
 *           free_gdi_handle
 *
 * Free a GDI handle and return a pointer to the object.
 */
void *free_gdi_handle( HGDIOBJ handle )
{
    void *object = NULL;
    struct gdi_handle_entry *entry;

    EnterCriticalSection( &gdi_section );
    if ((entry = handle_entry( handle )))
    {
        TRACE( "freed %s %p %u/%u\n", gdi_obj_type( entry->type ), handle,
               InterlockedDecrement( &debug_count ) + 1, MAX_GDI_HANDLES );
        object = entry->obj;
        entry->type = 0;
        entry->obj = next_free;
        next_free = entry;
    }
    LeaveCriticalSection( &gdi_section );
    return object;
}

/***********************************************************************
 *           DIB_DeleteObject
 */
static BOOL DIB_DeleteObject( HGDIOBJ handle )
{
    BITMAPOBJ *bmp;

    if (!(bmp = free_gdi_handle( handle ))) return FALSE;

    if (bmp->dib.dshSection)
    {
        SYSTEM_INFO SystemInfo;
        GetSystemInfo( &SystemInfo );
        UnmapViewOfFile( (char *)bmp->dib.dsBm.bmBits -
                         (bmp->dib.dsOffset % SystemInfo.dwAllocationGranularity) );
    }
    else VirtualFree( bmp->dib.dsBm.bmBits, 0, MEM_RELEASE );

    HeapFree(GetProcessHeap(), 0, bmp->color_table);
    return HeapFree( GetProcessHeap(), 0, bmp );
}

BOOL WINAPI SetVirtualResolution(HDC hdc, DWORD horz_res, DWORD vert_res,
                                 DWORD horz_size, DWORD vert_size);

/***********************************************************************
 *           DC_InitDC
 *
 * Setup device-specific DC values for a newly created DC.
 */
void DC_InitDC( DC* dc )
{
    PHYSDEV physdev = GET_DC_PHYSDEV( dc, pRealizeDefaultPalette );
    physdev->funcs->pRealizeDefaultPalette( physdev );
    SetTextColor( dc->hSelf, dc->textColor );
    SetBkColor( dc->hSelf, dc->backgroundColor );
    SelectObject( dc->hSelf, dc->hPen );
    SelectObject( dc->hSelf, dc->hBrush );
    SelectObject( dc->hSelf, dc->hFont );
    update_dc_clipping( dc );
    SetVirtualResolution( dc->hSelf, 0, 0, 0, 0 );
    physdev = GET_DC_PHYSDEV( dc, pSetBoundsRect );
    physdev->funcs->pSetBoundsRect( physdev, &dc->bounds, dc->bounds_enabled ? DCB_ENABLE : DCB_DISABLE );
}


/***********************************************************************
 *           DIB_SelectObject
 */
static HGDIOBJ DIB_SelectObject( HGDIOBJ handle, HDC hdc )
{
    HGDIOBJ ret;
    BITMAPOBJ *bitmap;
    DC *dc;
    PHYSDEV physdev;

    if (!(dc = get_dc_ptr( hdc ))) return 0;

    if (GetObjectType( hdc ) != OBJ_MEMDC)
    {
        ret = 0;
        goto done;
    }
    ret = dc->hBitmap;
    if (handle == dc->hBitmap) goto done;  /* nothing to do */

    if (!(bitmap = GDI_GetObjPtr( handle, OBJ_BITMAP )))
    {
        ret = 0;
        goto done;
    }

    if (GDI_get_ref_count( handle ))
    {
        WARN( "Bitmap already selected in another DC\n" );
        GDI_ReleaseObj( handle );
        ret = 0;
        goto done;
    }

    physdev = GET_DC_PHYSDEV( dc, pSelectBitmap );
    if (!physdev->funcs->pSelectBitmap( physdev, handle ))
    {
        GDI_ReleaseObj( handle );
        ret = 0;
    }
    else
    {
        dc->hBitmap = handle;
        GDI_inc_ref_count( handle );
        dc->dirty = 0;
        dc->vis_rect.left   = 0;
        dc->vis_rect.top    = 0;
        dc->vis_rect.right  = bitmap->dib.dsBm.bmWidth;
        dc->vis_rect.bottom = bitmap->dib.dsBm.bmHeight;
        dc->device_rect = dc->vis_rect;
        GDI_ReleaseObj( handle );
        DC_InitDC( dc );
        GDI_dec_ref_count( ret );
    }

 done:
    release_dc_ptr( dc );
    return ret;
}

/***********************************************************************
 *           alloc_gdi_handle
 *
 * Allocate a GDI handle for an object, which must have been allocated on the process heap.
 */
HGDIOBJ alloc_gdi_handle( void *obj, WORD type, const struct gdi_obj_funcs *funcs )
{
    struct gdi_handle_entry *entry;
    HGDIOBJ ret;

    ASSERT( type );  /* type 0 is reserved to mark free entries */

    EnterCriticalSection( &gdi_section );

    entry = next_free;
    if (entry)
        next_free = entry->obj;
    else if (next_unused < gdi_handles + MAX_GDI_HANDLES)
        entry = next_unused++;
    else
    {
        LeaveCriticalSection( &gdi_section );
        ERR( "out of GDI object handles, expect a crash\n" );
        if (TRACE_ON(gdi)) dump_gdi_objects();
        return 0;
    }
    entry->obj      = obj;
    entry->funcs    = funcs;
    entry->hdcs     = NULL;
    entry->type     = type;
    entry->selcount = 0;
    entry->system   = 0;
    entry->deleted  = 0;
    if (++entry->generation == 0xffff) entry->generation = 1;
    ret = entry_to_handle( entry );
    LeaveCriticalSection( &gdi_section );
    TRACE( "allocated %s %p %u/%u\n", gdi_obj_type(type), ret,
           InterlockedIncrement( &debug_count ), MAX_GDI_HANDLES );
    return ret;
}

/***********************************************************************
 *           D3DKMTCreateDCFromMemory    (GDI32.@)
 */
NTSTATUS WINAPI D3DKMTCreateDCFromMemory( D3DKMT_CREATEDCFROMMEMORY *desc )
{
    const struct d3dddi_format_info
    {
        D3DDDIFORMAT format;
        unsigned int bit_count;
        DWORD compression;
        unsigned int palette_size;
        DWORD mask_r, mask_g, mask_b;
    } *format = NULL;
    BITMAPOBJ *bmp = NULL;
    HBITMAP bitmap;
    unsigned int i;
    HDC dc;

    static const struct d3dddi_format_info format_info[] =
    {
        { D3DDDIFMT_R8G8B8,   24, BI_RGB,       0,   0x00000000, 0x00000000, 0x00000000 },
        { D3DDDIFMT_A8R8G8B8, 32, BI_RGB,       0,   0x00000000, 0x00000000, 0x00000000 },
        { D3DDDIFMT_X8R8G8B8, 32, BI_RGB,       0,   0x00000000, 0x00000000, 0x00000000 },
        { D3DDDIFMT_R5G6B5,   16, BI_BITFIELDS, 0,   0x0000f800, 0x000007e0, 0x0000001f },
        { D3DDDIFMT_X1R5G5B5, 16, BI_BITFIELDS, 0,   0x00007c00, 0x000003e0, 0x0000001f },
        { D3DDDIFMT_A1R5G5B5, 16, BI_BITFIELDS, 0,   0x00007c00, 0x000003e0, 0x0000001f },
        { D3DDDIFMT_A4R4G4B4, 16, BI_BITFIELDS, 0,   0x00000f00, 0x000000f0, 0x0000000f },
        { D3DDDIFMT_X4R4G4B4, 16, BI_BITFIELDS, 0,   0x00000f00, 0x000000f0, 0x0000000f },
        { D3DDDIFMT_P8,       8,  BI_RGB,       256, 0x00000000, 0x00000000, 0x00000000 },
    };

    if (!desc) return STATUS_INVALID_PARAMETER;

    TRACE("memory %p, format %#x, width %u, height %u, pitch %u, device dc %p, color table %p.\n",
          desc->pMemory, desc->Format, desc->Width, desc->Height,
          desc->Pitch, desc->hDeviceDc, desc->pColorTable);

    if (!desc->pMemory) return STATUS_INVALID_PARAMETER;

    for (i = 0; i < sizeof(format_info) / sizeof(*format_info); ++i)
    {
        if (format_info[i].format == desc->Format)
        {
            format = &format_info[i];
            break;
        }
    }
    if (!format) return STATUS_INVALID_PARAMETER;

    if (desc->Width > (UINT_MAX & ~3) / (format->bit_count / 8) ||
        !desc->Pitch || desc->Pitch < get_dib_stride( desc->Width, format->bit_count ) ||
        !desc->Height || desc->Height > UINT_MAX / desc->Pitch) return STATUS_INVALID_PARAMETER;

    if (!desc->hDeviceDc || !(dc = CreateCompatibleDC( desc->hDeviceDc ))) return STATUS_INVALID_PARAMETER;

    if (!(bmp = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*bmp) ))) goto error;

    bmp->dib.dsBm.bmWidth      = desc->Width;
    bmp->dib.dsBm.bmHeight     = desc->Height;
    bmp->dib.dsBm.bmWidthBytes = desc->Pitch;
    bmp->dib.dsBm.bmPlanes     = 1;
    bmp->dib.dsBm.bmBitsPixel  = format->bit_count;
    bmp->dib.dsBm.bmBits       = desc->pMemory;

    bmp->dib.dsBmih.biSize         = sizeof(bmp->dib.dsBmih);
    bmp->dib.dsBmih.biWidth        = desc->Width;
    bmp->dib.dsBmih.biHeight       = -(LONG)desc->Height;
    bmp->dib.dsBmih.biPlanes       = 1;
    bmp->dib.dsBmih.biBitCount     = format->bit_count;
    bmp->dib.dsBmih.biCompression  = format->compression;
    bmp->dib.dsBmih.biClrUsed      = format->palette_size;
    bmp->dib.dsBmih.biClrImportant = format->palette_size;

    bmp->dib.dsBitfields[0] = format->mask_r;
    bmp->dib.dsBitfields[1] = format->mask_g;
    bmp->dib.dsBitfields[2] = format->mask_b;

    if (format->palette_size)
    {
        if (!(bmp->color_table = HeapAlloc( GetProcessHeap(), 0, format->palette_size * sizeof(*bmp->color_table) )))
            goto error;
        if (desc->pColorTable)
        {
            for (i = 0; i < format->palette_size; ++i)
            {
                bmp->color_table[i].rgbRed      = desc->pColorTable[i].peRed;
                bmp->color_table[i].rgbGreen    = desc->pColorTable[i].peGreen;
                bmp->color_table[i].rgbBlue     = desc->pColorTable[i].peBlue;
                bmp->color_table[i].rgbReserved = 0;
            }
        }
        else
        {
            memcpy( bmp->color_table, get_default_color_table( format->bit_count ),
                    format->palette_size * sizeof(*bmp->color_table) );
        }
    }

    if (!(bitmap = alloc_gdi_handle( bmp, OBJ_BITMAP, &dib_funcs ))) goto error;

    desc->hDc = dc;
    desc->hBitmap = bitmap;
    SelectObject( dc, bitmap );
    return STATUS_SUCCESS;

error:
    if (bmp) HeapFree( GetProcessHeap(), 0, bmp->color_table );
    HeapFree( GetProcessHeap(), 0, bmp );
    DeleteDC( dc );
    return STATUS_INVALID_PARAMETER;
}

NTSTATUS 
APIENTRY 
D3DKMTDestroyDCFromMemory( 
 	_In_ const D3DKMT_DESTROYDCFROMMEMORY *desc )
{
    if (!desc) return STATUS_INVALID_PARAMETER;

    if (GetObjectType( desc->hDc ) != OBJ_MEMDC ||
        GetObjectType( desc->hBitmap ) != OBJ_BITMAP) return STATUS_INVALID_PARAMETER;
    DeleteObject( desc->hBitmap );
    DeleteDC( desc->hDc );

    return STATUS_SUCCESS;
}

NTSTATUS 
APIENTRY 
D3DKMTCheckMonitorPowerState(
  _In_ const D3DKMT_CHECKMONITORPOWERSTATE *pData
)
{
	HMONITOR mon;
	BOOL state;
	mon = MonitorFromWindow( NULL,MONITOR_DEFAULTTONULL );	
	GetDevicePowerState(mon, &state);
	if(state){
		return STATUS_SUCCESS;
	}else{
		return STATUS_INVALID_PARAMETER;
	}		
}

/******************************************************************************
 *		D3DKMTOpenAdapterFromHdc [GDI32.@]
 */
NTSTATUS 
WINAPI 
D3DKMTOpenAdapterFromHdc( void *pData )
{
    return STATUS_NO_MEMORY;
}

/******************************************************************************
 *		D3DKMTEscape [GDI32.@]
 */
NTSTATUS 
WINAPI 
D3DKMTEscape( const void *pData )
{
    return STATUS_NO_MEMORY;
}