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

#define FIRST_FONT_HANDLE 1
#define MAX_FONT_HANDLES  256

typedef struct tagGdiFont GdiFont;

struct font_fileinfo {
    FILETIME writetime;
    LARGE_INTEGER size;
    WCHAR path[1];
};

struct font_handle_entry
{
    void *obj;
    WORD  generation; /* generation count for reusing handle values */
};

static struct font_handle_entry font_handles[MAX_FONT_HANDLES];
static struct font_handle_entry *next_free;
static struct font_handle_entry *next_unused = font_handles;

static inline struct font_handle_entry *handle_entry( DWORD handle )
{
    unsigned int idx = LOWORD(handle) - FIRST_FONT_HANDLE;

    if (idx < MAX_FONT_HANDLES)
    {
        if (!HIWORD( handle ) || HIWORD( handle ) == font_handles[idx].generation)
            return &font_handles[idx];
    }
    return NULL;
}

/*************************************************************************
 *             GetFontFileInfo   (GDI32.@)
 */
BOOL WINAPI GetFontFileInfo( DWORD instance_id, DWORD unknown, struct font_fileinfo *info, DWORD size, DWORD *needed)
{
    *needed = 0;
    return FALSE;
}

// /*************************************************************************
 // *             GetFontFileInfo   (GDI32.@)
 // */
// BOOL WINAPI GetFontFileInfo( DWORD instance_id, DWORD unknown, struct font_fileinfo *info, DWORD size, DWORD *needed )
// {
    // struct font_handle_entry *entry = handle_entry( instance_id );
    // const GdiFont *font;

    // if (!entry)
    // {
        // SetLastError(ERROR_INVALID_PARAMETER);
        // return FALSE;
    // }

    // font = entry->obj;
    // *needed = sizeof(*info) + strlenW(font->fileinfo->path) * sizeof(WCHAR);
    // if (*needed > size)
    // {
        // SetLastError(ERROR_INSUFFICIENT_BUFFER);
        // return FALSE;
    // }

    // /* path is included too */
    // memcpy(info, font->fileinfo, *needed);
    // return TRUE;
// }