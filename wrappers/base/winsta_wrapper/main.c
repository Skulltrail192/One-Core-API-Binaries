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

static BOOL (WINAPI *pWinStationUnRegisterNotificationEvent)(ULONG_PTR);
static BOOL (WINAPI *pWinStationRegisterNotificationEvent)(HANDLE, ULONG_PTR*, DWORD, DWORD);

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

/**************************************************************************
 *           WinStationUnRegisterNotificationEvent   (WINSTA.@)
 */
BOOL WINAPI WinStationUnRegisterNotificationEvent(ULONG_PTR NotificationId )
{
	HMODULE hWinstaBase = GetModuleHandleA("winstabase.dll");
	HMODULE hWinstaNew = GetModuleHandleA("winstanew.dll");
	
	pWinStationUnRegisterNotificationEvent = (void *)GetProcAddress(hWinstaBase, "WinStationUnRegisterNotificationEvent");
	if(pWinStationUnRegisterNotificationEvent){
		return pWinStationUnRegisterNotificationEvent(NotificationId);
	}else{
		pWinStationUnRegisterNotificationEvent = (void *)GetProcAddress(hWinstaNew, "WinStationUnRegisterNotificationEvent");
		if(pWinStationUnRegisterNotificationEvent){
			return pWinStationUnRegisterNotificationEvent(NotificationId);
		}		
		SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
		return FALSE;
	}
}

/**************************************************************************
 *           WinStationUnRegisterNotificationEvent   (WINSTA.@)
 */
BOOL WINAPI WinStationRegisterNotificationEvent (
                        HANDLE hEventHandle,
                        ULONG_PTR *pNotificationId,
                        DWORD   dwFlags,
                        DWORD   dwMask
                        )
{
	HMODULE hWinstaBase = GetModuleHandleA("winstabase.dll");
	HMODULE hWinstaNew = GetModuleHandleA("winstanew.dll");
	
	pWinStationRegisterNotificationEvent = (void *)GetProcAddress(hWinstaBase, "WinStationRegisterNotificationEvent");
	if(pWinStationRegisterNotificationEvent){
		return pWinStationRegisterNotificationEvent(hEventHandle, pNotificationId, dwFlags, dwMask);
	}else{
		pWinStationRegisterNotificationEvent = (void *)GetProcAddress(hWinstaNew, "WinStationRegisterNotificationEvent");
		if(pWinStationRegisterNotificationEvent){
			return pWinStationRegisterNotificationEvent(hEventHandle, pNotificationId, dwFlags, dwMask);
		}		
		SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
		return FALSE;
	}
}