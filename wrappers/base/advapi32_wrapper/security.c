/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    security.c

Abstract:

    This module contains security functions

Author:

    Skulltrail 18-July-2021

Revision History:

--*/

#include "main.h"

BOOL 
WINAPI 
AddMandatoryAce(
	PACL pAcl, 
	DWORD dwAceRevision, 
	DWORD AceFlags, 
	DWORD MandatoryPolicy, 
	PSID pLabelSid
)
{
  NTSTATUS Status; // eax@1
  BOOL result; // eax@2

  Status = RtlAddMandatoryAce(pAcl, 
							  dwAceRevision, 
							  AceFlags, 
							  MandatoryPolicy, 
							  SYSTEM_MANDATORY_LABEL_ACE_TYPE, 
							  pLabelSid);
  if ( !NT_SUCCESS(Status))
  {
    result = FALSE;
  }
  else
  {
    result = TRUE;
  }
  return result;
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

BOOL CredUnprotectW(
  BOOL   fAsSelf,
  LPWSTR pszProtectedCredentials,
  DWORD  cchProtectedCredentials,
  LPWSTR pszCredentials,
  DWORD  *pcchMaxChars
)
{
	return FALSE;
}