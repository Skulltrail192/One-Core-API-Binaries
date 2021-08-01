/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    security.c

Abstract:

    This module implements the Win32 security checks.

Author:

    Skulltrail 20-May-2017

Revision History:

--*/

#include <main.h>

/*
* @unimplemented
*/
DWORD 
WINAPI 
CheckElevationEnabled(
    BOOL *pResult
) 
{
	*pResult = FALSE;
	return S_OK;
}

/*
* @unimplemented
*/
DWORD 
WINAPI 
CheckElevation(
   LPCWSTR lpApplicationName,
   PDWORD pdwFlags,
   HANDLE hChildToken,
   PDWORD pdwRunLevel,
   PDWORD pdwReason
)
{
	return S_OK;
}

BOOL 
AccessCheckByTypeResultList(
	PSECURITY_DESCRIPTOR pSecurityDescriptor, 
	PSID PrincipalSelfSid, 
	HANDLE ClientToken, 
	DWORD DesiredAccess, 
	POBJECT_TYPE_LIST ObjectTypeList, 
	DWORD ObjectTypeListLength, 
	PGENERIC_MAPPING GenericMapping, 
	PPRIVILEGE_SET PrivilegeSet, 
	LPDWORD PrivilegeSetLength, 
	LPDWORD GrantedAccessList, 
	LPDWORD AccessStatusList)
{
  NTSTATUS Status ; // eax
  BOOL result; // eax
  ULONG i; // esi

  Status  = NtAccessCheckByTypeResultList(
          pSecurityDescriptor,
          PrincipalSelfSid,
          ClientToken,
          DesiredAccess,
          ObjectTypeList,
          ObjectTypeListLength,
          GenericMapping,
          PrivilegeSet,
          PrivilegeSetLength,
          GrantedAccessList,
          (PNTSTATUS)AccessStatusList);
  if ( Status  >= 0 )
  {
    i = 0;
    if ( ObjectTypeListLength )
    {
      do
      {
        if ( AccessStatusList[i] )
          AccessStatusList[i] = RtlNtStatusToDosError(AccessStatusList[i]);
        else
          AccessStatusList[i] = 0;
        ++i;
      }
      while ( i < ObjectTypeListLength );
    }
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(Status);
    result = FALSE;
  }
  return result;
}