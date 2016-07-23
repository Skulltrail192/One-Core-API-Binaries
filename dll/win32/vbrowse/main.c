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

typedef DWORD CONFIGRET;

/**********************************************************************
 * User objects management
 */
 
#define OBJ_OTHER_PROCESS ((void *)1)  /* returned by get_user_handle_ptr on unknown handles */
 
enum user_obj_type
{
     USER_WINDOW = 1,  /* window */
     USER_MENU,        /* menu */
     USER_ACCEL,       /* accelerator */
     USER_ICON,        /* icon or cursor */
     USER_DWP          /* DeferWindowPos structure */
};
 
 
struct user_object
{
     HANDLE             handle;
     enum user_obj_type type;
};
 
struct cursoricon_frame
{
    UINT               width;    /* frame-specific width */
    UINT               height;   /* frame-specific height */
    UINT               delay;    /* frame-specific delay between this frame and the next (in jiffies) */
    HBITMAP            color;    /* color bitmap */
    HBITMAP            alpha;    /* pre-multiplied alpha bitmap for 32-bpp icons */
    HBITMAP            mask;     /* mask bitmap (followed by color for 1-bpp icons) */
};

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;
    }

    return TRUE;
}

/*************************************************************************
 * SHIsExplorerBrowser			[BROWSEUI.138]
 */
BOOL WINAPI SHIsExplorerBrowser()
{
    return TRUE;
}

/*************************************************************************
 * SHEnumClassesOfCategories	[BROWSEUI.136]
 */
HRESULT WINAPI SHEnumClassesOfCategories(long param8, long paramC, long param10, long param14, long param18)
{
    return E_NOTIMPL;
}

/*************************************************************************
 * SHWriteClassesOfCategories	[BROWSEUI.137]
 */
HRESULT WINAPI SHWriteClassesOfCategories(long param8, long paramC, long param10,
    long param14, long param18, long param1C, long param20)
{
    return E_NOTIMPL;
}
