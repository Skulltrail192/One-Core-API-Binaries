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

typedef PVOID HPOWERNOTIFY, *PHPOWERNOTIFY;

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

typedef enum _POWER_DATA_ACCESSOR {
    ACCESS_AC_POWER_SETTING_INDEX,
    ACCESS_DC_POWER_SETTING_INDEX,
    ACCESS_FRIENDLY_NAME,
    ACCESS_DESCRIPTION,
    ACCESS_POSSIBLE_POWER_SETTING,
    ACCESS_POSSIBLE_POWER_SETTING_FRIENDLY_NAME,
    ACCESS_POSSIBLE_POWER_SETTING_DESCRIPTION,
    ACCESS_DEFAULT_AC_POWER_SETTING,
    ACCESS_DEFAULT_DC_POWER_SETTING,
    ACCESS_POSSIBLE_VALUE_MIN,
    ACCESS_POSSIBLE_VALUE_MAX,
    ACCESS_POSSIBLE_VALUE_INCREMENT,
    ACCESS_POSSIBLE_VALUE_UNITS,
    ACCESS_ICON_RESOURCE,
    ACCESS_DEFAULT_SECURITY_DESCRIPTOR,
    ACCESS_ATTRIBUTES,
    ACCESS_SCHEME,
    ACCESS_SUBGROUP,
    ACCESS_INDIVIDUAL_SETTING,
    ACCESS_ACTIVE_SCHEME,
    ACCESS_CREATE_SCHEME,
    ACCESS_AC_POWER_SETTING_MAX,
    ACCESS_DC_POWER_SETTING_MAX,
    ACCESS_AC_POWER_SETTING_MIN,
    ACCESS_DC_POWER_SETTING_MIN,
    ACCESS_PROFILE,
    ACCESS_OVERLAY_SCHEME,
    ACCESS_ACTIVE_OVERLAY_SCHEME,
} POWER_DATA_ACCESSOR, *PPOWER_DATA_ACCESSOR;

UINT g_LastID = (UINT)-1;

DWORD WINAPI
PowerGetActiveScheme(
	HKEY UserRootPowerKey,
	GUID **polguid
)
{
   return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD 
WINAPI 
PowerSetActiveScheme(
  _In_opt_       HKEY UserRootPowerKey,
  _In_     const GUID *SchemeGuid
)
{
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

DWORD 
WINAPI 
PowerRegisterSuspendResumeNotification(
  _In_  DWORD         Flags,
  _In_  HANDLE        Recipient,
  _Out_ PHPOWERNOTIFY RegistrationHandle
)
{
   return ERROR_CALL_NOT_IMPLEMENTED;
}


DWORD
WINAPI 
PowerUnregisterSuspendResumeNotification(
  _Inout_ HPOWERNOTIFY RegistrationHandle
)
{
   return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD 
WINAPI 
PowerReadACValue(
  _In_opt_          HKEY    RootPowerKey,
  _In_opt_    const GUID    *SchemeGuid,
  _In_opt_    const GUID    *SubGroupOfPowerSettingsGuid,
  _In_opt_    const GUID    *PowerSettingGuid,
  _Out_opt_         PULONG  Type,
  _Out_opt_         LPBYTE  Buffer,
  _Inout_opt_       LPDWORD BufferSize
)
{
   return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD
WINAPI 
PowerSettingRegisterNotification(
  _In_  LPCGUID       SettingGuid,
  _In_  DWORD         Flags,
  _In_  HANDLE        Recipient,
  _Out_ PHPOWERNOTIFY RegistrationHandle
)
{
   return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD 
WINAPI 
PowerSettingUnregisterNotification(
  _Inout_ HPOWERNOTIFY RegistrationHandle
)
{
   return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD 
WINAPI 
PowerWriteACValueIndex(
  _In_opt_       HKEY  RootPowerKey,
  _In_     const GUID  *SchemeGuid,
  _In_opt_ const GUID  *SubGroupOfPowerSettingsGuid,
  _In_opt_ const GUID  *PowerSettingGuid,
  _In_           DWORD AcValueIndex
)
{
   return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD 
WINAPI 
PowerWriteDCValueIndex(
  _In_opt_       HKEY  RootSystemPowerKey,
  _In_     const GUID  *SchemePersonalityGuid,
  _In_opt_ const GUID  *SubGroupOfPowerSettingsGuid,
  _In_     const GUID  *PowerSettingGuid,
  _In_           DWORD DefaultDcIndex
)
{
   return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI PowerReadFriendlyName(
  _In_opt_        HKEY    RootPowerKey,
  _In_opt_  const GUID    *SchemeGuid,
  _In_opt_  const GUID    *SubGroupOfPowerSettingsGuid,
  _In_opt_  const GUID    *PowerSettingGuid,
  _Out_opt_       PUCHAR  Buffer,
  _Inout_         LPDWORD BufferSize
)
{
   return ERROR_CALL_NOT_IMPLEMENTED;
}

DWORD WINAPI PowerEnumerate(HKEY key, const GUID *scheme, const GUID *subgroup, POWER_DATA_ACCESSOR flags,
                        ULONG index, UCHAR *buffer, DWORD *buffer_size)
{
   return ERROR_CALL_NOT_IMPLEMENTED;
}