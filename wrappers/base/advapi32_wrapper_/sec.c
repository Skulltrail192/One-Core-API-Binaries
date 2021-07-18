/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    sec.c

Abstract:

    Security functions

Author:

    Skulltrail 07-March-2021

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(advapi32); 	

/*
 * @implemented
 */
VOID
WINAPI
QuerySecurityAccessMask(IN SECURITY_INFORMATION SecurityInformation,
                        OUT LPDWORD DesiredAccess)
{
    *DesiredAccess = 0;

    if (SecurityInformation & (OWNER_SECURITY_INFORMATION |
                               GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION))
    {
        *DesiredAccess |= READ_CONTROL;
    }

    if (SecurityInformation & SACL_SECURITY_INFORMATION)
        *DesiredAccess |= ACCESS_SYSTEM_SECURITY;
}


/*
 * @implemented
 */
VOID
WINAPI
SetSecurityAccessMask(IN SECURITY_INFORMATION SecurityInformation,
                      OUT LPDWORD DesiredAccess)
{
    *DesiredAccess = 0;

    if (SecurityInformation & (OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION))
        *DesiredAccess |= WRITE_OWNER;

    if (SecurityInformation & DACL_SECURITY_INFORMATION)
        *DesiredAccess |= WRITE_DAC;

    if (SecurityInformation & SACL_SECURITY_INFORMATION)
        *DesiredAccess |= ACCESS_SYSTEM_SECURITY;
}

/******************************************************************************
 * RegCreateKeyTransactedA   [ADVAPI32.@]
 */
LSTATUS 
WINAPI 
RegCreateKeyTransactedA( 
	HKEY hkey, 
	LPCSTR name, 
	DWORD reserved, 
	LPSTR class,
    DWORD options, 
	REGSAM access, 
	SECURITY_ATTRIBUTES *sa,
    PHKEY retkey, 
	LPDWORD dispos, 
	HANDLE transaction, 
	PVOID reserved2 
)
{
	return RegCreateKeyExA(hkey,
						   name,
						   reserved,
						   class,
						   options,
						   access,
						   sa,
						   retkey,
						   dispos);
}

/******************************************************************************
 * RegCreateKeyTransactedW   [ADVAPI32.@]
 */
LSTATUS 
WINAPI 
RegCreateKeyTransactedW( 
	HKEY hkey, 
	LPCWSTR name, 
	DWORD reserved, 
	LPWSTR class,
    DWORD options, 
	REGSAM access, 
	SECURITY_ATTRIBUTES *sa,
    PHKEY retkey, 
	LPDWORD dispos, 
	HANDLE transaction, 
	PVOID reserved2 
)
{
	return RegCreateKeyExW(hkey,
						   name,
						   reserved,
						   class,
						   options,
						   access,
						   sa,
						   retkey,
						   dispos);
}

LSTATUS 
RegDeleteKeyTransactedA(
  HKEY   hKey,
  LPCSTR lpSubKey,
  REGSAM samDesired,
  DWORD  Reserved,
  HANDLE hTransaction,
  PVOID  pExtendedParameter
)
{
	return RegDeleteKeyExA(hKey,
						   lpSubKey,
						   samDesired,
						   Reserved);
}

LSTATUS 
RegDeleteKeyTransactedW(
  HKEY   hKey,
  LPCWSTR lpSubKey,
  REGSAM samDesired,
  DWORD  Reserved,
  HANDLE hTransaction,
  PVOID  pExtendedParameter
)
{
	return RegDeleteKeyExW(hKey,
						   lpSubKey,
						   samDesired,
						   Reserved);
}

BOOL WINAPI LogonUserExExW(
  _In_      LPTSTR        lpszUsername,
  _In_opt_  LPTSTR        lpszDomain,
  _In_opt_  LPTSTR        lpszPassword,
  _In_      DWORD         dwLogonType,
  _In_      DWORD         dwLogonProvider,
  _In_opt_  PTOKEN_GROUPS pTokenGroups,
  _Out_opt_ PHANDLE       phToken,
  _Out_opt_ PSID          *ppLogonSid,
  _Out_opt_ PVOID         *ppProfileBuffer,
  _Out_opt_ LPDWORD       pdwProfileLength,
  _Out_opt_ PQUOTA_LIMITS pQuotaLimits
)
{
	return LogonUserExA(lpszUsername,
						lpszDomain,
						lpszPassword,
						dwLogonType,
						dwLogonProvider,
						phToken,
						ppLogonSid,
						ppProfileBuffer,
						pdwProfileLength,
						pQuotaLimits);
}

BOOL 
IsValidRelativeSecurityDescriptor(
	PSECURITY_DESCRIPTOR pSecurityDescriptor, 
	ULONG SecurityDescriptorLength, 
	SECURITY_INFORMATION RequiredInformation
)
{
  if ( RtlValidRelativeSecurityDescriptor(pSecurityDescriptor, SecurityDescriptorLength, RequiredInformation) )
    return TRUE;
  BaseSetLastNTError(STATUS_INVALID_SECURITY_DESCR);
  return FALSE;
}