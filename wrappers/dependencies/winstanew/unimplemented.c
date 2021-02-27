/*
 * ReactOS WinStation
 * Copyright (C) 2005 ReactOS Team
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
/*
 * PROJECT:         ReactOS winsta.dll
 * FILE:            lib/winsta/main.c
 * PURPOSE:         WinStation
 * PROGRAMMER:      Emanuele Aliberti <ea@reactos.com>
 *                  Samuel Serapi?n
 */

#include "winsta.h"

BOOL WINAPI WinStationConnectEx(int a1, int a2)
{
	return TRUE;
}

BOOLEAN WINAPI WinStationFreeUserCertificates(HLOCAL hMem)
{
	return TRUE;
}

DWORD WINAPI WinStationFreeUserCredentials(PVOID cred)
{
	return 87;
}

BOOLEAN WINAPI WinStationGetConnectionProperty(int a1, int a2, int a3)
{
	return TRUE;
}

BOOLEAN WINAPI WinStationGetDeviceId(int a1, int a2, int a3, int a4)
{
	return TRUE;
}

BOOLEAN WINAPI WinStationGetInitialApplication(int a1, int a2, int a3, int a4, int a5)
{
	return TRUE;
}

BOOL WINAPI WinStationGetLoggedOnCount(int a1, int a2)
{
	return TRUE;
}

BOOL WINAPI WinStationGetSessionIds(int a1, PVOID Dst, PULONG a3)
{
	return TRUE;
}

BOOLEAN WINAPI WinStationGetUserCertificates(HLOCAL hMem)
{
	return TRUE;
}

BOOLEAN WINAPI WinStationGetUserCredentials(HLOCAL hMem)
{
	return TRUE;
}

BOOLEAN WINAPI WinStationGetUserProfile(int a1, int a2, int a3, int a4)
{
	return TRUE;
}

DWORD WINAPI WinStationIsSessionPermitted()
{
	return 87;
}

BOOLEAN WINAPI WinStationQueryAllowConcurrentConnections()
{
	return TRUE;
}

BOOLEAN WINAPI WinStationNegotiateSession(BOOLEAN a1, LPWORD a2, int a3, int a4, int a5)
{
	return TRUE;
}

DWORD WINAPI WinStationReportUIResult(int a1, PVOID Dst, PULONG a3)
{
	return 87;
}

BOOLEAN WINAPI WinStationRevertFromServicesSession()
{
	return TRUE;
}

BOOLEAN WINAPI WinStationSetAutologonPassword(int a1, int a2)
{
	return TRUE;
}

BOOLEAN WINAPI WinStationSetConnectionProperty(int a1, PVOID Dst, PULONG a3)
{
	return TRUE;
}

BOOLEAN WINAPI WinStationSwitchToServicesSession()
{
	return TRUE;
}

BOOLEAN WINAPI WinStationSystemShutdownStarted()
{
	return TRUE;
}

BOOL WINAPI WinStationSystemShutdownWait(DWORD dwMilliseconds, int a2)
{
	return TRUE;
}

BOOL WINAPI WinStationUserLoginAccessCheck(int a1, int a2, int a3, int a4)
{
	return TRUE;
}

BOOLEAN WINAPI WinStationVerify(BOOLEAN a1, LPWORD a2, int a3, int a4, int a5, int a6)
{
	return TRUE;
}

PVOID WINAPI WinStationVirtualOpenEx(void *a1, unsigned __int32 a2, const char *a3, unsigned __int32 a4)
{
	return 0;
}

BOOLEAN WINAPI WinStationFreePropertyValue(PVOID value)
{
	return TRUE;
}

BOOLEAN WINAPI WinStationEnumerateExW(int a1, int a2, int a3)
{
	return TRUE;
}

VOID
WINAPI 
WinStationQueryEnforcementCore(PVOID A,
                               PVOID B,
                               PVOID C,
                               PVOID D,
                               PVOID E,
                               PVOID F)
{
    UNIMPLEMENTED;
}

BOOLEAN WINSTAAPI WinStationQueryInformationW(	
	HANDLE 	hServer,
	ULONG 	LogonId,
	WINSTATIONINFOCLASS 	WinStationInformationClass,
	PVOID 	pWinStationInformation,
	ULONG 	WinStationInformationLength,
	PULONG 	pReturnLength 
)
{
	return FALSE;	
}
