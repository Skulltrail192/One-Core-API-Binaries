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
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wine/list.h"
#include <ft2build.h>
#include <freetype.h>
#include <unicode.h>
#include <tttables.h>

WINE_DEFAULT_DEBUG_CHANNEL(gdi);

#define FIRST_FONT_HANDLE 1
#define MAX_FONT_HANDLES  256

#define MS_MAKE_TAG( _x1, _x2, _x3, _x4 ) \
          ( ( (FT_ULong)_x4 << 24 ) |     \
            ( (FT_ULong)_x3 << 16 ) |     \
            ( (FT_ULong)_x2 <<  8 ) |     \
			  (FT_ULong)_x1 )

#define MS_TTCF_TAG MS_MAKE_TAG('t', 't', 'c', 'f')

typedef struct {
    GLYPHMETRICS gm;
    ABC          abc;  /* metrics of the unrotated char */
    BOOL         init;
} GM;

typedef struct {
    FLOAT eM11, eM12;
    FLOAT eM21, eM22;
} FMAT2;

typedef struct {
    DWORD hash;
    LOGFONTW lf;
    FMAT2 matrix;
    BOOL can_use_bitmap;
} FONT_DESC;

typedef struct tagGdiFont GdiFont;

struct tagGdiFont {
    struct list entry;
    struct list unused_entry;
    unsigned int refcount;
    GM **gm;
    DWORD gmsize;
    OUTLINETEXTMETRICW *potm;
    DWORD total_kern_pairs;
    KERNINGPAIR *kern_pairs;
    struct list child_fonts;

    /* the following members can be accessed without locking, they are never modified after creation */
    FT_Face ft_face;
    struct font_mapping *mapping;
    LPWSTR name;
    int charset;
    int codepage;
    BOOL fake_italic;
    BOOL fake_bold;
    BYTE underline;
    BYTE strikeout;
    INT orientation;
    FONT_DESC font_desc;
    LONG aveWidth, ppem;
    double scale_y;
    SHORT yMax;
    SHORT yMin;
    DWORD ntmFlags;
    DWORD aa_flags;
    UINT ntmCellHeight, ntmAvgWidth;
    FONTSIGNATURE fs;
    GdiFont *base_font;
    VOID *GSUB_Table;
    const VOID *vert_feature;
    ULONG ttc_item_offset; /* 0 if font is not a part of TrueType collection */
    DWORD cache_num;
    DWORD instance_id;
    struct font_fileinfo *fileinfo;
};

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
BOOL WINAPI GetFontFileInfo( DWORD instance_id, DWORD unknown, struct font_fileinfo *info, DWORD size, DWORD *needed )
{
    struct font_handle_entry *entry = handle_entry( instance_id );
    const GdiFont *font;

    if (!entry)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    font = entry->obj;
    *needed = sizeof(*info) + strlenW(font->fileinfo->path) * sizeof(WCHAR);
    if (*needed > size)
    {
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        return FALSE;
    }

    /* path is included too */
    memcpy(info, font->fileinfo, *needed);
    return TRUE;
}

/*************************************************************
 *           GetFontRealizationInfo    (GDI32.@)
 */
BOOL 
WINAPI 
GetFontRealizationInfo(HDC hdc, struct font_realization_info *info)
{
    BOOL is_v0 = info->size == FIELD_OFFSET(struct font_realization_info, unk);
    PHYSDEV dev;
    BOOL ret;
    DC *dc;

    if (info->size != sizeof(*info) && !is_v0)
        return FALSE;

    dc = get_dc_ptr(hdc);
    if (!dc) return FALSE;
    dev = GET_DC_PHYSDEV( dc, pGetFontRealizationInfo );
    ret = dev->funcs->pGetFontRealizationInfo( dev, info );
    release_dc_ptr(dc);
    return ret;
}

static DWORD get_font_data( GdiFont *font, DWORD table, DWORD offset, LPVOID buf, DWORD cbData)
{
    FT_Face ft_face = font->ft_face;
    FT_ULong len;
    FT_Error err;

    if (!FT_IS_SFNT(ft_face)) return GDI_ERROR;

    if(!buf)
        len = 0;
    else
        len = cbData;

    /* if font is a member of TTC, 'ttcf' tag allows reading from beginning of TTC file,
       0 tag means to read from start of collection member data. */
    if (font->ttc_item_offset)
    {
        if (table == MS_TTCF_TAG)
            table = 0;
        else if (table == 0)
            offset += font->ttc_item_offset;
    }

    table = RtlUlongByteSwap( table );  /* MS tags differ in endianness from FT ones */

    /* make sure value of len is the value freetype says it needs */
    if (buf && len)
    {
        FT_ULong needed = 0;
        err = FT_Load_Sfnt_Table(ft_face, table, offset, NULL, &needed);
        if( !err && needed < len) len = needed;
    }
    err = FT_Load_Sfnt_Table(ft_face, table, offset, buf, &len);
    if (err)
    {
        table = RtlUlongByteSwap( table );
        TRACE("Can't find table %s\n", debugstr_an((char*)&table, 4));
	return GDI_ERROR;
    }
    return len;
}

/*************************************************************************
 *             GetFontFileData   (GDI32.@)
 */
BOOL WINAPI GetFontFileData( DWORD instance_id, DWORD unknown, UINT64 offset, void *buff, DWORD buff_size )
{
    struct font_handle_entry *entry = handle_entry( instance_id );
    DWORD tag = 0, size;
    GdiFont *font;
    if (!entry)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    font = entry->obj;
    if (font->ttc_item_offset)
        tag = MS_TTCF_TAG;
    size = get_font_data( font, tag, 0, NULL, 0 );
    if (size < buff_size || offset > size - buff_size)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    /* For now this only works for SFNT case. */
    return get_font_data( font, tag, offset, buff, buff_size ) != 0;
}