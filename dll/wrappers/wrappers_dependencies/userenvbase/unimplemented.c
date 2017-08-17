/*
 *  ReactOS kernel
 *  Copyright (C) 2004 ReactOS Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * FILE:            dll/win32/userenv/environment.c
 * PURPOSE:         User environment functions
 * PROGRAMMER:      Eric Kohl
 */

#include "precomp.h"

#define NDEBUG
#include <debug.h>

typedef GUID *REFGPEXTENSIONID;

typedef UINT_PTR ASYNCCOMPLETIONHANDLE;

typedef enum _GPO_LINK {
    GPLinkUnknown = 0,                     
    GPLinkMachine,                         
    GPLinkSite,                            
    GPLinkDomain,                          
    GPLinkOrganizationalUnit               
} GPO_LINK, *PGPO_LINK;

typedef struct _GROUP_POLICY_OBJECTW {
    DWORD       dwOptions;                  
    DWORD       dwVersion;                  
    LPWSTR      lpDSPath;                   
    LPWSTR      lpFileSysPath;              
    LPWSTR      lpDisplayName;              
    WCHAR       szGPOName[50];              
    GPO_LINK    GPOLink;                    
    LPARAM      lParam;                     
    struct _GROUP_POLICY_OBJECTW * pNext;   
    struct _GROUP_POLICY_OBJECTW * pPrev;   
    LPWSTR      lpExtensions;               
    LPARAM      lParam2;                    
    LPWSTR      lpLink;                     
                                            
} GROUP_POLICY_OBJECTW, *PGROUP_POLICY_OBJECTW;

typedef struct _GROUP_POLICY_OBJECTA {
    DWORD       dwOptions;                  
    DWORD       dwVersion;                  
    LPSTR      lpDSPath;                   
    LPSTR      lpFileSysPath;              
    LPSTR      lpDisplayName;              
    CHAR       szGPOName[50];              
    GPO_LINK    GPOLink;                    
    LPARAM      lParam;                     
    struct _GROUP_POLICY_OBJECTA * pNext;   
    struct _GROUP_POLICY_OBJECTA * pPrev;   
    LPSTR      lpExtensions;               
    LPARAM      lParam2;                    
    LPSTR      lpLink;                     
                                            
} GROUP_POLICY_OBJECTA, *PGROUP_POLICY_OBJECTA;

BOOL WINAPI FreeGPOListA(
  _In_  PGROUP_POLICY_OBJECTA pGPOList
)
{
	return TRUE;
}

BOOL WINAPI FreeGPOListW(
  _In_  PGROUP_POLICY_OBJECTW pGPOList
)
{
	return TRUE;
}

DWORD WINAPI GetAppliedGPOListA(
  _In_   DWORD dwFlags,
  _In_   LPCTSTR pMachineName,
  _In_   PSID pSidUser,
  _In_   GUID *pGuidExtension,
  _Out_  PGROUP_POLICY_OBJECTA *ppGPOList
)
{
	return 0;
}

DWORD WINAPI GetAppliedGPOListW(
  _In_   DWORD dwFlags,
  _In_   LPCWSTR pMachineName,
  _In_   PSID pSidUser,
  _In_   GUID *pGuidExtension,
  _Out_  PGROUP_POLICY_OBJECTW *ppGPOList
)
{
	return 0;
}

BOOL WINAPI GetGPOListA(
  _In_   HANDLE hToken,
  _In_   LPCTSTR lpName,
  _In_   LPCTSTR lpHostName,
  _In_   LPCTSTR lpComputerName,
  _In_   DWORD dwFlags,
  _Out_  PGROUP_POLICY_OBJECTA *pGPOList
)
{
	return TRUE;
}

BOOL WINAPI GetGPOListW(
  _In_   HANDLE hToken,
  _In_   LPCWSTR lpName,
  _In_   LPCWSTR lpHostName,
  _In_   LPCWSTR lpComputerName,
  _In_   DWORD dwFlags,
  _Out_  PGROUP_POLICY_OBJECTW *pGPOList
)
{
	return TRUE;
}

DWORD WINAPI ProcessGroupPolicyCompleted(
  _In_  REFGPEXTENSIONID extensionId,
  _In_  ASYNCCOMPLETIONHANDLE pAsyncHandle,
  _In_  DWORD dwStatus
)
{
	return 0;
}

DWORD WINAPI ProcessGroupPolicyCompletedEx(
  _In_  REFGPEXTENSIONID extensionId,
  _In_  ASYNCCOMPLETIONHANDLE pAsyncHandle,
  _In_  DWORD dwStatus,
  _In_  HRESULT RsopStatus
)
{
	return 0;
}

DWORD WINAPI GetUserAppDataPathA(HANDLE TokenHandle, DWORD dwErrCode, LPSTR lpMultiByteStr)
{
	return 0;
}

DWORD WINAPI GetUserAppDataPathW(HANDLE TokenHandle, DWORD dwErrCode, LPWSTR lpMultiByteStr)
{
	return 0;
}

DWORD WINAPI GetUserProfileDirFromSidA(PSID Sid, LPSTR lpMultiByteStr, HGLOBAL hMem)
{
	return 0;
}

DWORD WINAPI GetUserProfileDirFromSidW(PSID Sid, LPWSTR lpMultiByteStr, HGLOBAL hMem)
{
	return 0;
}

PSID WINAPI GetUserSid(HANDLE TokenHandle)
{
	return NULL;
}

void WINAPI InitializeUserProfile()
{
	;
}

DWORD WINAPI SecureUserProfiles()
{
	return 0;
}

DWORD WINAPI EnterUserProfileLock(BOOL lpString2)
{
	return 0;
}

DWORD WINAPI LeaveUserProfileLock(BOOL lpString2)
{
	return 0;
}

DWORD WINAPI CheckAccessForPolicyGeneration(HANDLE hdl, PCNZWCH Str, LPWSTR string, BOOL verification, DWORD flags)
{
	return 0;
}

LSTATUS WINAPI GetGroupPolicyNetworkName(LPBYTE lpData, LPDWORD lpcbData)
{
	return 0x00000000;
}

DWORD WINAPI SignalUserPolicyForegroundProcessingDone()
{
	return 0;
}

DWORD WINAPI SignalMachinePolicyForegroundProcessingDone()
{
	return 0;
}

BOOL WINAPI IsSyncForegroundPolicyRefresh(BOOL a1, HANDLE TokenHandle)
{
	return TRUE;
}

BOOL WINAPI CopySystemProfile(HANDLE TokenHandle)
{
	return TRUE;
}

BOOL WINAPI RsopLoggingEnabled()
{
	return TRUE;
}
/*
HRESULT WINAPI RSoPAccessCheckByType(
  _In_   PSECURITY_DESCRIPTOR pSecurityDescriptor,
  _In_   PSID pPrincipalSelfSid,
  _In_   PRSOPTOKEN pRsopToken,
  _In_   DWORD dwDesiredAccessMask,
  _In_   POBJECT_TYPE_LIST pObjectTypeList,
  _In_   DWORD ObjectTypeListLength,
  _In_   PGENERIC_MAPPING pGenericMapping,
  _In_   PPRIVILEGE_SET pPrivilegeSet,
  _In_   LPDWORD pdwPrivilegeSetLength,
  _Out_  LPDWORD pdwGrantedAccessMask,
  _Out_  LPBOOL pbAccessStatus
)
{
	return S_OK;
}*/