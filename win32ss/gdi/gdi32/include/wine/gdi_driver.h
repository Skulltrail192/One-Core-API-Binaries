/*
 * Definitions for Wine GDI drivers
 *
 * Copyright 2011 Alexandre Julliard
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

#ifndef __WINE_WINE_GDI_DRIVER_H
#define __WINE_WINE_GDI_DRIVER_H

//#include "wine/list.h"

struct gdi_dc_funcs;
struct opengl_funcs;

typedef struct gdi_physdev
{
    const struct gdi_dc_funcs *funcs;
    struct gdi_physdev        *next;
    HDC                        hdc;
} *PHYSDEV;

struct bitblt_coords
{
    int  log_x;     /* original position and size, in logical coords */
    int  log_y;
    int  log_width;
    int  log_height;
    int  x;         /* mapped position and size, in device coords */
    int  y;
    int  width;
    int  height;
    RECT visrect;   /* rectangle clipped to the visible part, in device coords */
    DWORD layout;   /* DC layout */
};

struct gdi_image_bits
{
    void   *ptr;       /* pointer to the bits */
    BOOL    is_copy;   /* whether this is a copy of the bits that can be modified */
    void  (*free)(struct gdi_image_bits *);  /* callback for freeing the bits */
    void   *param;     /* extra parameter for callback private use */
};

struct brush_pattern
{
    BITMAPINFO           *info;     /* DIB info */
    struct gdi_image_bits bits;     /* DIB bits */
    UINT                  usage;    /* color usage for DIB info */
};

/* increment this when you change the DC function table */
#define WINE_GDI_DRIVER_VERSION 46

#define GDI_PRIORITY_NULL_DRV        0  /* null driver */
#define GDI_PRIORITY_FONT_DRV      100  /* any font driver */
#define GDI_PRIORITY_GRAPHICS_DRV  200  /* any graphics driver */
#define GDI_PRIORITY_DIB_DRV       300  /* the DIB driver */
#define GDI_PRIORITY_PATH_DRV      400  /* the path driver */

static inline PHYSDEV get_physdev_entry_point( PHYSDEV dev, size_t offset )
{
    while (!((void **)dev->funcs)[offset / sizeof(void *)]) dev = dev->next;
    return dev;
}

#define GET_NEXT_PHYSDEV(dev,func) \
    get_physdev_entry_point( (dev)->next, FIELD_OFFSET(struct gdi_dc_funcs,func))
/*
static inline void push_dc_driver( PHYSDEV *dev, PHYSDEV physdev, const struct gdi_dc_funcs *funcs )
{
    while ((*dev)->funcs->priority > funcs->priority) dev = &(*dev)->next;
    physdev->funcs = funcs;
    physdev->next = *dev;
    physdev->hdc = (*dev)->hdc;
    *dev = physdev;
}
*/
/* support for window surfaces */

struct window_surface;

struct window_surface_funcs
{
    void  (*lock)( struct window_surface *surface );
    void  (*unlock)( struct window_surface *surface );
    void* (*get_info)( struct window_surface *surface, BITMAPINFO *info );
    RECT* (*get_bounds)( struct window_surface *surface );
    void  (*set_region)( struct window_surface *surface, HRGN region );
    void  (*flush)( struct window_surface *surface );
    void  (*destroy)( struct window_surface *surface );
};

struct window_surface
{
    const struct window_surface_funcs *funcs; /* driver-specific implementations  */
//    struct list                        entry; /* entry in global list managed by user32 */
    LONG                               ref;   /* reference count */
    RECT                               rect;  /* constant, no locking needed */
    /* driver-specific fields here */
};

static inline ULONG window_surface_add_ref( struct window_surface *surface )
{
    return InterlockedIncrement( &surface->ref );
}

static inline ULONG window_surface_release( struct window_surface *surface )
{
    ULONG ret = InterlockedDecrement( &surface->ref );
    if (!ret) surface->funcs->destroy( surface );
    return ret;
}

/* the DC hook support is only exported on Win16, the 32-bit version is a Wine extension */

#define DCHC_INVALIDVISRGN      0x0001
#define DCHC_DELETEDC           0x0002
#define DCHF_INVALIDATEVISRGN   0x0001
#define DCHF_VALIDATEVISRGN     0x0002
#define DCHF_RESETDC            0x0004  /* Wine extension */

typedef BOOL (CALLBACK *DCHOOKPROC)(HDC,WORD,DWORD_PTR,LPARAM);

WINGDIAPI DWORD_PTR WINAPI GetDCHook(HDC,DCHOOKPROC*);
WINGDIAPI BOOL      WINAPI SetDCHook(HDC,DCHOOKPROC,DWORD_PTR);
WINGDIAPI WORD      WINAPI SetHookFlags(HDC,WORD);

extern void CDECL __wine_make_gdi_object_system( HGDIOBJ handle, BOOL set );
extern void CDECL __wine_set_visible_region( HDC hdc, HRGN hrgn, const RECT *vis_rect,
                                             const RECT *device_rect, struct window_surface *surface );
extern void CDECL __wine_set_display_driver( HMODULE module );
extern struct opengl_funcs * CDECL __wine_get_wgl_driver( HDC hdc, UINT version );

#endif /* __WINE_WINE_GDI_DRIVER_H */
