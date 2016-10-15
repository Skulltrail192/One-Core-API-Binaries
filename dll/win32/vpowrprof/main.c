/*
 * Copyright (C) 2005 Benjamin Cutler
 * Copyright (C) 2008 Dmitry Chapyshev
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
 */


#include <stdarg.h>

#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#define NTOS_MODE_USER
#include <ndk/pofuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/setypes.h>
#include <powrprof.h>
#include <wine/debug.h>
#include <wine/unicode.h>

WINE_DEFAULT_DEBUG_CHANNEL(powrprof);


static const WCHAR szPowerCfgSubKey[] =
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Controls Folder\\PowerCfg";
static const WCHAR szUserPowerConfigSubKey[] = 
    L"Control Panel\\PowerCfg";
static const WCHAR szCurrentPowerPolicies[] = 
    L"CurrentPowerPolicy";
static const WCHAR szPolicies[] = L"Policies";
static const WCHAR szName[] = L"Name";
static const WCHAR szDescription[] = L"Description";
static const WCHAR szSemaphoreName[] = L"PowerProfileRegistrySemaphore";
static const WCHAR szDiskMax[] = L"DiskSpindownMax";
static const WCHAR szDiskMin[] = L"DiskSpindownMin";
static const WCHAR szLastID[] = L"LastID";

typedef enum  { 
  PlatformRoleUnspecified        = 0,
  PlatformRoleDesktop            = 1,
  PlatformRoleMobile             = 2,
  PlatformRoleWorkstation        = 3,
  PlatformRoleEnterpriseServer   = 4,
  PlatformRoleSOHOServer         = 5,
  PlatformRoleAppliancePC        = 6,
  PlatformRolePerformanceServer  = 7,
  PlatformRoleSlate              = 8,
  PlatformRoleMaximum
} POWER_PLATFORM_ROLE;

UINT g_LastID = (UINT)-1;

DWORD WINAPI
PowerGetActiveScheme(
	HKEY UserRootPowerKey,
	GUID **polguid
)
{
   FIXME("(%p,%p) stub!\n", UserRootPowerKey, polguid);
   return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD 
WINAPI
PowerReadDCValue(
	HKEY RootPowerKey, 
	const GUID *Scheme, 
	const GUID *SubGroup, 
	const GUID *PowerSettings, 
	PULONG Type, 
	PUCHAR Buffer, 
	DWORD *BufferSize
)
{
   FIXME("(%p,%s,%s,%s,%p,%p,%p) stub!\n", RootPowerKey, debugstr_guid(Scheme), debugstr_guid(SubGroup), debugstr_guid(PowerSettings), Type, Buffer, BufferSize);
   return ERROR_CALL_NOT_IMPLEMENTED;
}

//Need more implementation
POWER_PLATFORM_ROLE WINAPI 
PowerDeterminePlatformRole(void)
{
	return PlatformRoleDesktop;
}

//Need more implementation
POWER_PLATFORM_ROLE WINAPI 
PowerDeterminePlatformRoleEx(
  _In_ ULONG Version
)
{
	return PlatformRoleDesktop;
}