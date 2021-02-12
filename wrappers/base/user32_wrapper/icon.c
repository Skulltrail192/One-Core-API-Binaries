/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    cursor.c

Abstract:

        This file implements the NT icons routines.

Author:

    Skulltrail 18-April-2018

Revision History:

--*/
#include <main.h>

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

// static struct cursoricon_object *get_icon_ptr( HICON handle )
// {
    // struct cursoricon_object *obj = get_user_handle_ptr( handle, USER_ICON );
    // if (obj == OBJ_OTHER_PROCESS)
    // {
        // WARN( "icon handle %p from other process\n", handle );
        // obj = NULL;
    // }
    // return obj;
// }

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

    // if (info->cbSize != sizeof(*info))
    // {
        // SetLastError( ERROR_INVALID_PARAMETER );
        // return FALSE;
    // }
    // if (!(ptr = get_icon_ptr( icon )))
    // {
        // SetLastError( ERROR_INVALID_CURSOR_HANDLE );
        // return FALSE;
    // }

    // frame = get_icon_frame( ptr, 0 );
    // if (!frame)
    // {
        // release_user_handle_ptr( ptr );
        // SetLastError( ERROR_INVALID_CURSOR_HANDLE );
        // return FALSE;
    // }

    // TRACE("%p => %dx%d\n", icon, frame->width, frame->height);

    // info->fIcon        = ptr->is_icon;
    // info->xHotspot     = ptr->hotspot.x;
    // info->yHotspot     = ptr->hotspot.y;
    // info->hbmColor     = copy_bitmap( frame->color );
    // info->hbmMask      = copy_bitmap( frame->mask );
    // info->wResID       = 0;
    // info->szModName[0] = 0;
    // info->szResName[0] = 0;
    // if (ptr->module)
    // {
        // if (IS_INTRESOURCE( ptr->resname )) info->wResID = LOWORD( ptr->resname );
        // else lstrcpynW( info->szResName, ptr->resname, MAX_PATH );
    // }
    // if (!info->hbmMask || (!info->hbmColor && frame->color))
    // {
        // DeleteObject( info->hbmMask );
        // DeleteObject( info->hbmColor );
        // ret = FALSE;
    // }
    // module = ptr->module;
    // release_icon_frame( ptr, frame );
    // release_user_handle_ptr( ptr );
    // if (ret && module) GetModuleFileNameW( module, info->szModName, MAX_PATH );
    return ret;
}

void WINAPI InternalGetWindowIcon(HWND window, HICON icon)
{
	;
}