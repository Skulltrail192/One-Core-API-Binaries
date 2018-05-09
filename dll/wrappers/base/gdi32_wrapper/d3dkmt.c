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

typedef enum _D3DDDIFORMAT
{
    D3DDDIFMT_UNKNOWN                 = 0,
    D3DDDIFMT_R8G8B8                  = 0x14,
    D3DDDIFMT_A8R8G8B8                = 0x15,
    D3DDDIFMT_X8R8G8B8                = 0x16,
    D3DDDIFMT_R5G6B5                  = 0x17,
    D3DDDIFMT_X1R5G5B5                = 0x18,
    D3DDDIFMT_A1R5G5B5                = 0x19,
    D3DDDIFMT_A4R4G4B4                = 0x1a,
    D3DDDIFMT_R3G3B2                  = 0x1b,
    D3DDDIFMT_A8                      = 0x1c,
    D3DDDIFMT_A8R3G3B2                = 0x1d,
    D3DDDIFMT_X4R4G4B4                = 0x1e,
    D3DDDIFMT_A2B10G10R10             = 0x1f,
    D3DDDIFMT_A8B8G8R8                = 0x20,
    D3DDDIFMT_X8B8G8R8                = 0x21,
    D3DDDIFMT_G16R16                  = 0x22,
    D3DDDIFMT_A2R10G10B10             = 0x23,
    D3DDDIFMT_A16B16G16R16            = 0x24,
    D3DDDIFMT_A8P8                    = 0x28,
    D3DDDIFMT_P8                      = 0x29,
    D3DDDIFMT_L8                      = 0x32,
    D3DDDIFMT_A8L8                    = 0x33,
    D3DDDIFMT_A4L4                    = 0x34,
    D3DDDIFMT_V8U8                    = 0x3c,
    D3DDDIFMT_L6V5U5                  = 0x3d,
    D3DDDIFMT_X8L8V8U8                = 0x3e,
    D3DDDIFMT_Q8W8V8U8                = 0x3f,
    D3DDDIFMT_V16U16                  = 0x40,
    D3DDDIFMT_W11V11U10               = 0x41,
    D3DDDIFMT_A2W10V10U10             = 0x43,
    D3DDDIFMT_D16_LOCKABLE            = 0x46,
    D3DDDIFMT_D32                     = 0x47,
    D3DDDIFMT_S1D15                   = 0x48,
    D3DDDIFMT_D15S1                   = 0x49,
    D3DDDIFMT_S8D24                   = 0x4a,
    D3DDDIFMT_D24S8                   = 0x4b,
    D3DDDIFMT_X8D24                   = 0x4c,
    D3DDDIFMT_D24X8                   = 0x4d,
    D3DDDIFMT_X4S4D24                 = 0x4e,
    D3DDDIFMT_D24X4S4                 = 0x4f,
    D3DDDIFMT_D16                     = 0x50,
    D3DDDIFMT_L16                     = 0x51,
    D3DDDIFMT_D32F_LOCKABLE           = 0x52,
    D3DDDIFMT_D24FS8                  = 0x53,
    D3DDDIFMT_D32_LOCKABLE            = 0x54,
    D3DDDIFMT_S8_LOCKABLE             = 0x55,
    D3DDDIFMT_G8R8                    = 0x5b,
    D3DDDIFMT_R8                      = 0x5c,
    D3DDDIFMT_VERTEXDATA              = 0x64,
    D3DDDIFMT_INDEX16                 = 0x65,
    D3DDDIFMT_INDEX32                 = 0x66,
    D3DDDIFMT_Q16W16V16U16            = 0x6e,
    D3DDDIFMT_R16F                    = 0x6f,
    D3DDDIFMT_G16R16F                 = 0x70,
    D3DDDIFMT_A16B16G16R16F           = 0x71,
    D3DDDIFMT_R32F                    = 0x72,
    D3DDDIFMT_G32R32F                 = 0x73,
    D3DDDIFMT_A32B32G32R32F           = 0x74,
    D3DDDIFMT_CxV8U8                  = 0x75,
    D3DDDIFMT_A1                      = 0x76,
    D3DDDIFMT_A2B10G10R10_XR_BIAS     = 0x77,
    D3DDDIFMT_DXVACOMPBUFFER_BASE     = 0x96,
    D3DDDIFMT_PICTUREPARAMSDATA       = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0,
    D3DDDIFMT_MACROBLOCKDATA          = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x01,
    D3DDDIFMT_RESIDUALDIFFERENCEDATA  = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x02,
    D3DDDIFMT_DEBLOCKINGDATA          = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x03,
    D3DDDIFMT_INVERSEQUANTIZATIONDATA = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x04,
    D3DDDIFMT_SLICECONTROLDATA        = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x05,
    D3DDDIFMT_BITSTREAMDATA           = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x06,
    D3DDDIFMT_MOTIONVECTORBUFFER      = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x07,
    D3DDDIFMT_FILMGRAINBUFFER         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x08,
    D3DDDIFMT_DXVA_RESERVED9          = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x09,
    D3DDDIFMT_DXVA_RESERVED10         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x0a,
    D3DDDIFMT_DXVA_RESERVED11         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x0b,
    D3DDDIFMT_DXVA_RESERVED12         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x0c,
    D3DDDIFMT_DXVA_RESERVED13         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x0d,
    D3DDDIFMT_DXVA_RESERVED14         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x0e,
    D3DDDIFMT_DXVA_RESERVED15         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x0f,
    D3DDDIFMT_DXVA_RESERVED16         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x10,
    D3DDDIFMT_DXVA_RESERVED17         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x11,
    D3DDDIFMT_DXVA_RESERVED18         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x12,
    D3DDDIFMT_DXVA_RESERVED19         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x13,
    D3DDDIFMT_DXVA_RESERVED20         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x14,
    D3DDDIFMT_DXVA_RESERVED21         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x15,
    D3DDDIFMT_DXVA_RESERVED22         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x16,
    D3DDDIFMT_DXVA_RESERVED23         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x17,
    D3DDDIFMT_DXVA_RESERVED24         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x18,
    D3DDDIFMT_DXVA_RESERVED25         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x19,
    D3DDDIFMT_DXVA_RESERVED26         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x1a,
    D3DDDIFMT_DXVA_RESERVED27         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x1b,
    D3DDDIFMT_DXVA_RESERVED28         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x1c,
    D3DDDIFMT_DXVA_RESERVED29         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x1d,
    D3DDDIFMT_DXVA_RESERVED30         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x1e,
    D3DDDIFMT_DXVA_RESERVED31         = D3DDDIFMT_DXVACOMPBUFFER_BASE + 0x1f,
    D3DDDIFMT_DXVACOMPBUFFER_MAX      = D3DDDIFMT_DXVA_RESERVED31,
    D3DDDIFMT_BINARYBUFFER            = 0xc7,
    D3DDDIFMT_DXT1                    = MAKEFOURCC('D', 'X', 'T', '1'),
    D3DDDIFMT_DXT2                    = MAKEFOURCC('D', 'X', 'T', '2'),
    D3DDDIFMT_DXT3                    = MAKEFOURCC('D', 'X', 'T', '3'),
    D3DDDIFMT_DXT4                    = MAKEFOURCC('D', 'X', 'T', '4'),
    D3DDDIFMT_DXT5                    = MAKEFOURCC('D', 'X', 'T', '5'),
    D3DDDIFMT_G8R8_G8B8               = MAKEFOURCC('G', 'R', 'G', 'B'),
    D3DDDIFMT_MULTI2_ARGB8            = MAKEFOURCC('M', 'E', 'T', '1'),
    D3DDDIFMT_R8G8_B8G8               = MAKEFOURCC('R', 'G', 'B', 'G'),
    D3DDDIFMT_UYVY                    = MAKEFOURCC('U', 'Y', 'V', 'Y'),
    D3DDDIFMT_YUY2                    = MAKEFOURCC('Y', 'U', 'Y', '2'),
    D3DDDIFMT_FORCE_UINT              = 0x7fffffff,
} D3DDDIFORMAT;

typedef struct _D3DKMT_CREATEDCFROMMEMORY
{
    void *pMemory;
    D3DDDIFORMAT Format;
    UINT Width;
    UINT Height;
    UINT Pitch;
    HDC hDeviceDc;
    PALETTEENTRY *pColorTable;
    HDC hDc;
    HANDLE hBitmap;
} D3DKMT_CREATEDCFROMMEMORY;

typedef struct _D3DKMT_DESTROYDCFROMMEMORY
{
    HDC hDc;
    HANDLE hBitmap;
} D3DKMT_DESTROYDCFROMMEMORY;

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