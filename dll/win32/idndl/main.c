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
#include "idndl.h"

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

int WINAPI DownlevelGetLocaleScripts(
    __in                            LPCWSTR lpLocaleName,   // Locale Name
    __out_ecount_opt(cchScripts)    LPWSTR  lpScripts,      // Output buffer for scripts
    __in                            int     cchScripts      // size of output buffer
)
{
	return 0;
}

int WINAPI DownlevelGetStringScripts(
        __in                         DWORD   dwFlags,        // optional behavior flags
        __in                         LPCWSTR lpString,       // Unicode character input string
        __in                         int     cchString,      // size of input string
        __out_ecount_opt(cchScripts) LPWSTR  lpScripts,      // Script list output string
        __in                         int     cchScripts      // size of output string
)
{
	return 0;
}

BOOL WINAPI DownlevelVerifyScripts(
     __in    DWORD   dwFlags,            // optional behavior flags
     __in    LPCWSTR lpLocaleScripts,    // Locale list of scripts string
     __in    int     cchLocaleScripts,   // size of locale script list string
     __in    LPCWSTR lpTestScripts,      // test scripts string
     __in    int     cchTestScripts      // size of test list string
)
{
	return TRUE;
}