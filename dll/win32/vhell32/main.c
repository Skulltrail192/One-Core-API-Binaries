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
 
WINE_DEFAULT_DEBUG_CHANNEL(shell);

HINSTANCE shell32_hInstance = 0;

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

/************************************************************************/

BOOL WINAPI StrRetToStrNW(LPWSTR dest, DWORD len, LPSTRRET src, const ITEMIDLIST *pidl)
{

    if (!dest)
        return FALSE;

    switch (src->uType)
    {
        case STRRET_WSTR:
            lstrcpynW(dest, src->pOleStr, len);
            CoTaskMemFree(src->pOleStr);
            break;
        case STRRET_CSTR:
            if (!MultiByteToWideChar(CP_ACP, 0, src->cStr, -1, dest, len) && len)
                dest[len-1] = 0;
            break;
        case STRRET_OFFSET:
            if (!MultiByteToWideChar(CP_ACP, 0, ((LPCSTR)&pidl->mkid)+src->uOffset, -1, dest, len)
                    && len)
                dest[len-1] = 0;
            break;
        default:
            FIXME("unknown type %u!\n", src->uType);
            if (len)
                *dest = '\0';
            return FALSE;
    }
    return TRUE;
}