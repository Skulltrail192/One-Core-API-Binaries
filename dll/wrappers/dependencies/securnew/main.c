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
#include <wincred.h>
#include <winbase.h>
#include <secext.h>
#include <security.h>
#include <ntsecapi.h>


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

SECURITY_STATUS WINAPI ChangeAccountPasswordA(SEC_CHAR *pszPackageName, 
											  SEC_CHAR *pszDomainName, 
											  SEC_CHAR *pszAccountName, 
											  SEC_CHAR *pszOldPassword, 
											  SEC_CHAR *pszNewPassword, 
											  BOOLEAN bImpersonating, 
											  unsigned long dwReserved, 
											  PSecBufferDesc pOutput)
{
  return 0x00000000;
}

SECURITY_STATUS WINAPI ChangeAccountPasswordW(SEC_WCHAR *pszPackageName, 
											  SEC_WCHAR *pszDomainName, 
											  SEC_WCHAR *pszAccountName, 
											  SEC_WCHAR *pszOldPassword, 
											  SEC_WCHAR *pszNewPassword, 
											  BOOLEAN bImpersonating, 
											  unsigned long dwReserved, 
											  PSecBufferDesc pOutput)
{
  return 0x00000000;
}

BOOL WINAPI CollectLsaPerformanceData(LPCWSTR lpString1, char **a2, DWORD *a3, DWORD *a4)
{
	return TRUE;
}

int WINAPI CloseLsaPerformanceData()
{
	return 0;
}

SECURITY_STATUS WINAPI CredParseUserNameWithType(const wchar_t *Str, PUNICODE_STRING DestinationString, LSA_UNICODE_STRING *a3, int a4)
{
  return 0x00000000;
}

ULONG WINAPI OpenLsaPerformanceData(unsigned __int16 *a1)
{
	return 0;
}

SECURITY_STATUS WINAPI SeciAllocateAndSetCallFlags(int a1, BOOL a2)
{
  return 0x00000000;
}

SECURITY_STATUS WINAPI SeciAllocateAndSetIPAddress(const void *Src, size_t Size, int a3)
{
  return 0x00000000;
}

void WINAPI SeciFreeCallContext()
{
	;
}
