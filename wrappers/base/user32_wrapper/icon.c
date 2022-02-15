/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    icon.c

Abstract:

        This file implements the NT icons routines.

Author:

    Skulltrail 18-April-2018

Revision History:

--*/

#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(user32);

#define NB_USER_HANDLES  ((LAST_USER_HANDLE - FIRST_USER_HANDLE + 1) >> 1)
#define USER_HANDLE_TO_INDEX(hwnd) ((LOWORD(hwnd) - FIRST_USER_HANDLE) >> 1)

static void *user_handles[NB_USER_HANDLES];

/**********************************************************************
 * User objects management
 */

struct cursoricon_frame
{
    UINT               width;    /* frame-specific width */
    UINT               height;   /* frame-specific height */
    UINT               delay;    /* frame-specific delay between this frame and the next (in jiffies) */
    HBITMAP            color;    /* color bitmap */
    HBITMAP            alpha;    /* pre-multiplied alpha bitmap for 32-bpp icons */
    HBITMAP            mask;     /* mask bitmap (followed by color for 1-bpp icons) */
};

struct cursoricon_object
{
    struct user_object      obj;        /* object header */
    struct list             entry;      /* entry in shared icons list */
    ULONG_PTR               param;      /* opaque param used by 16-bit code */
    HMODULE                 module;     /* module for icons loaded from resources */
    LPWSTR                  resname;    /* resource name for icons loaded from resources */
    HRSRC                   rsrc;       /* resource for shared icons */
    BOOL                    is_icon;    /* whether icon or cursor */
    BOOL                    is_ani;     /* whether this object is a static cursor or an animated cursor */
    UINT                    delay;      /* delay between this frame and the next (in jiffies) */
    POINT                   hotspot;
};

struct static_cursoricon_object
{
    struct cursoricon_object shared;
    struct cursoricon_frame  frame;      /* frame-specific icon data */
};

struct animated_cursoricon_object
{
    struct cursoricon_object shared;
    UINT                     num_frames; /* number of frames in the icon/cursor */
    UINT                     num_steps;  /* number of sequence steps in the icon/cursor */
    HICON                    frames[1];  /* list of animated cursor frames */
};

/***********************************************************************
 *           get_user_handle_ptr
 */
void *get_user_handle_ptr( HANDLE handle, enum user_obj_type type )
{
    struct user_object *ptr;
    WORD index = USER_HANDLE_TO_INDEX( handle );

    if (index >= NB_USER_HANDLES) return NULL;

    USER_Lock();
    if ((ptr = user_handles[index]))
    {
        if (ptr->type == type &&
            ((UINT)(UINT_PTR)ptr->handle == (UINT)(UINT_PTR)handle ||
             !HIWORD(handle) || HIWORD(handle) == 0xffff))
            return ptr;
        ptr = NULL;
    }
    else ptr = OBJ_OTHER_PROCESS;
    USER_Unlock();
    return ptr;
}

static struct cursoricon_object *get_icon_ptr( HICON handle )
{
    struct cursoricon_object *obj = get_user_handle_ptr( handle, USER_ICON );
    if (obj == OBJ_OTHER_PROCESS)
    {
        WARN( "icon handle %p from other process\n", handle );
        obj = NULL;
    }
    return obj;
}

static struct cursoricon_frame *get_icon_frame( struct cursoricon_object *obj, int istep )
{
    struct static_cursoricon_object *req_frame;

    if (obj->is_ani)
    {
        struct animated_cursoricon_object *ani_icon_data;
        struct cursoricon_object *frameobj;

        ani_icon_data = (struct animated_cursoricon_object *) obj;
        if (!(frameobj = get_icon_ptr( ani_icon_data->frames[istep] )))
            return 0;
        req_frame = (struct static_cursoricon_object *) frameobj;
    }
    else
        req_frame = (struct static_cursoricon_object *) obj;

    return &req_frame->frame;
}

/***********************************************************************
 *           release_user_handle_ptr
 */
void release_user_handle_ptr( void *ptr )
{
    ASSERT( ptr && ptr != OBJ_OTHER_PROCESS );
    USER_Unlock();
}

static void release_icon_frame( struct cursoricon_object *obj, struct cursoricon_frame *frame )
{
    if (obj->is_ani)
    {
        struct cursoricon_object *frameobj;

        frameobj = (struct cursoricon_object *) (((char *)frame) - FIELD_OFFSET(struct static_cursoricon_object, frame));
        release_user_handle_ptr( frameobj );
    }
}

/***********************************************************************
 *             copy_bitmap
 *
 * Helper function to duplicate a bitmap.
 */
static HBITMAP copy_bitmap( HBITMAP bitmap )
{
    HDC src, dst = 0;
    HBITMAP new_bitmap = 0;
    BITMAP bmp;

    if (!bitmap) return 0;
    if (!GetObjectW( bitmap, sizeof(bmp), &bmp )) return 0;

    if ((src = CreateCompatibleDC( 0 )) && (dst = CreateCompatibleDC( 0 )))
    {
        SelectObject( src, bitmap );
        if ((new_bitmap = CreateCompatibleBitmap( src, bmp.bmWidth, bmp.bmHeight )))
        {
            SelectObject( dst, new_bitmap );
            BitBlt( dst, 0, 0, bmp.bmWidth, bmp.bmHeight, src, 0, 0, SRCCOPY );
        }
    }
    DeleteDC( dst );
    DeleteDC( src );
    return new_bitmap;
}

BOOL 
WINAPI 
PrivateRegisterICSProc(RegisterCallback registrator)
{
  BOOL result; // eax@2

  if ( gpICSProc )
  {
    result = FALSE;
  }
  else
  {
    gpICSProc = registrator;
    result = TRUE;
  }
  return result;
}

/**********************************************************************
 *              GetIconInfoExA (USER32.@)
 */
BOOL WINAPI GetIconInfoExA( HICON icon, ICONINFOEXA *info )
{
    ICONINFOEXW infoW;

    if (info->cbSize != sizeof(*info))
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }
    infoW.cbSize = sizeof(infoW);
    if (!GetIconInfoExW( icon, &infoW )) return FALSE;
    info->fIcon    = infoW.fIcon;
    info->xHotspot = infoW.xHotspot;
    info->yHotspot = infoW.yHotspot;
    info->hbmColor = infoW.hbmColor;
    info->hbmMask  = infoW.hbmMask;
    info->wResID   = infoW.wResID;
    WideCharToMultiByte( CP_ACP, 0, infoW.szModName, -1, info->szModName, MAX_PATH, NULL, NULL );
    WideCharToMultiByte( CP_ACP, 0, infoW.szResName, -1, info->szResName, MAX_PATH, NULL, NULL );
    return TRUE;
}

/**********************************************************************
 *              GetIconInfoExW (USER32.@)
 */
BOOL WINAPI GetIconInfoExW( HICON icon, ICONINFOEXW *info )
{
    struct cursoricon_frame *frame;
    struct cursoricon_object *ptr;
    HMODULE module;
    BOOL ret = TRUE;

    if (info->cbSize != sizeof(*info))
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }
    if (!(ptr = get_icon_ptr( icon )))
    {
        SetLastError( ERROR_INVALID_CURSOR_HANDLE );
        return FALSE;
    }

    frame = get_icon_frame( ptr, 0 );
    if (!frame)
    {
        release_user_handle_ptr( ptr );
        SetLastError( ERROR_INVALID_CURSOR_HANDLE );
        return FALSE;
    }

    TRACE("%p => %dx%d\n", icon, frame->width, frame->height);

    info->fIcon        = ptr->is_icon;
    info->xHotspot     = ptr->hotspot.x;
    info->yHotspot     = ptr->hotspot.y;
    info->hbmColor     = copy_bitmap( frame->color );
    info->hbmMask      = copy_bitmap( frame->mask );
    info->wResID       = 0;
    info->szModName[0] = 0;
    info->szResName[0] = 0;
    if (ptr->module)
    {
        if (IS_INTRESOURCE( ptr->resname )) info->wResID = LOWORD( ptr->resname );
        else lstrcpynW( info->szResName, ptr->resname, MAX_PATH );
    }
    if (!info->hbmMask || (!info->hbmColor && frame->color))
    {
        DeleteObject( info->hbmMask );
        DeleteObject( info->hbmColor );
        ret = FALSE;
    }
    module = ptr->module;
    release_icon_frame( ptr, frame );
    release_user_handle_ptr( ptr );
    if (ret && module) GetModuleFileNameW( module, info->szModName, MAX_PATH );
    return ret;
}

/*******************************************************************
 *		InternalGetWindowIcon (USER32.@)
 */
INT WINAPI InternalGetWindowIcon(HWND hwnd, UINT iconType )
{
    return 0;
}