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

#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>
#include <ntsecapi.h>
#include <bcrypt.h>
#include <wingdi.h>
#include <math.h>

#define WIDTH_BYTES_ALIGN32(cx, bpp) ((((cx) * (bpp) + 31) & ~31) >> 3)

static const GUID CLSID_IDeskDisplayAdapter = {0x42071712,0x76d4,0x11d1,{0x8b,0x24,0x00,0xa0,0xc9,0x06,0x8f,0xf3}};

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    TRACE("fdwReason %u\n", fdwReason);

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}

int WINAPI GdiGetBitmapBitsSize(BITMAPINFO * lpbmi) 	
{
    UINT Ret;

    if (!lpbmi)
        return 0;

    if (lpbmi->bmiHeader.biSize == sizeof(BITMAPCOREHEADER))
    {
        PBITMAPCOREHEADER Core = (PBITMAPCOREHEADER) lpbmi;
        Ret =
        WIDTH_BYTES_ALIGN32(Core->bcWidth * Core->bcPlanes,
                Core->bcBitCount) * Core->bcHeight;
    }
    else /// assume BITMAPINFOHEADER 
    {
        if (!(lpbmi->bmiHeader.biCompression) || (lpbmi->bmiHeader.biCompression == BI_BITFIELDS))
        {
            Ret = WIDTH_BYTES_ALIGN32(
                    lpbmi->bmiHeader.biWidth * lpbmi->bmiHeader.biPlanes,
                    lpbmi->bmiHeader.biBitCount) * abs(lpbmi->bmiHeader.biHeight);
        }
        else
        {
            Ret = lpbmi->bmiHeader.biSizeImage;
        }
    }
    return Ret;
}

