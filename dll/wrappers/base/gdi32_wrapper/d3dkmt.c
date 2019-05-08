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
#include <wingdi.h>

#define MAX_GDI_HANDLES  16384
#define FIRST_GDI_HANDLE 32
#define MAX_FONT_HANDLES  256

WINE_DEFAULT_DEBUG_CHANNEL(gdi);

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
        ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) | \
        ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24))
#endif /* MAKEFOURCC */

static CRITICAL_SECTION gdi_section;

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

#define BMF_TOPDOWN   0x0001
#define BMF_NOZEROINIT   0x0002

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
    BITMAPINFO *bmpInfo = NULL;
    BITMAPV5HEADER *bmpHeader = NULL;
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
	
	DbgPrint("D3DKMTCreateDCFromMemory:: calling function\n");

    if (!desc) return STATUS_INVALID_PARAMETER;

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
        !desc->Pitch || desc->Pitch < (((desc->Width * format->bit_count + 31) >> 3) & ~3) ||
        !desc->Height || desc->Height > UINT_MAX / desc->Pitch) return STATUS_INVALID_PARAMETER;

    if (!desc->hDeviceDc || !(dc = CreateCompatibleDC( desc->hDeviceDc ))) return STATUS_INVALID_PARAMETER;

    if (!(bmpInfo = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*bmpInfo) + (format->palette_size * sizeof(RGBQUAD)) ))) goto error;
    if (!(bmpHeader = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*bmpHeader) ))) goto error;

	
    bmpHeader->bV5Size        = sizeof(*bmpHeader);
    bmpHeader->bV5Width       = desc->Width;
    bmpHeader->bV5Height      = desc->Height;
    bmpHeader->bV5SizeImage   = desc->Pitch;
    bmpHeader->bV5Planes      = 1;
    bmpHeader->bV5BitCount    = format->bit_count;
    bmpHeader->bV5Compression = BI_BITFIELDS;
    bmpHeader->bV5RedMask     = format->mask_r;
    bmpHeader->bV5GreenMask   = format->mask_g;
    bmpHeader->bV5BlueMask    = format->mask_b;

    bmpInfo->bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
    bmpInfo->bmiHeader.biWidth        = desc->Width;
    bmpInfo->bmiHeader.biHeight       = -(LONG)desc->Height;
    bmpInfo->bmiHeader.biPlanes       = 1;
    bmpInfo->bmiHeader.biBitCount     = format->bit_count;
    bmpInfo->bmiHeader.biCompression  = format->compression;
    bmpInfo->bmiHeader.biClrUsed      = format->palette_size;
    bmpInfo->bmiHeader.biClrImportant = format->palette_size;

    if (desc->pColorTable)
    {
        for (i = 0; i < format->palette_size; ++i)
        {
             bmpInfo->bmiColors[i].rgbRed   = desc->pColorTable[i].peRed;
             bmpInfo->bmiColors[i].rgbGreen = desc->pColorTable[i].peGreen;
             bmpInfo->bmiColors[i].rgbBlue = desc->pColorTable[i].peBlue;
             bmpInfo->bmiColors[i].rgbReserved = 0;
        }
    }

    if (!(bitmap = CreateBitmap(desc->Width, desc->Height, 1, format->bit_count, desc->pMemory))) goto error;

    desc->hDc = dc;
    desc->hBitmap = bitmap;
    SelectObject( dc, bitmap );
	DbgPrint("D3DKMTCreateDCFromMemory:: return STATUS_SUCCESS\n");
    return STATUS_SUCCESS;

error:
    if (bmpInfo)  HeapFree( GetProcessHeap(), 0, bmpInfo );
    if (bmpHeader) HeapFree( GetProcessHeap(), 0, bmpHeader );

    DeleteDC( dc );
    return STATUS_INVALID_PARAMETER;
}


/***********************************************************************
 *           D3DKMTDestroyDCFromMemory    (GDI32.@)
 */
NTSTATUS WINAPI D3DKMTDestroyDCFromMemory( const D3DKMT_DESTROYDCFROMMEMORY *desc )
{
    if (!desc) return STATUS_INVALID_PARAMETER;

    TRACE("dc %p, bitmap %p.\n", desc->hDc, desc->hBitmap);

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

/******************************************************************************
 *		D3DKMTCloseAdapter [GDI32.@]
 */
NTSTATUS WINAPI D3DKMTQueryStatistics(D3DKMT_QUERYSTATISTICS *stats)
{
    FIXME("(%p): stub\n", stats);
    return STATUS_SUCCESS;
}