/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * FILE:            lib/secur32/unimplemented.c
 * PURPOSE:         Client-side unimplemented functions
 * UPDATE HISTORY:
 *                  Created 05/08/00
 */

#include "precomp.h"
#include <wincred.h>

#define SEC_FAR

typedef enum  { 
  DS_UNKNOWN_NAME             = 0,
  DS_FQDN_1779_NAME           = 1,
  DS_NT4_ACCOUNT_NAME         = 2,
  DS_DISPLAY_NAME             = 3,
  DS_UNIQUE_ID_NAME           = 6,
  DS_CANONICAL_NAME           = 7,
  DS_USER_PRINCIPAL_NAME      = 8,
  DS_CANONICAL_NAME_EX        = 9,
  DS_SERVICE_PRINCIPAL_NAME   = 10,
  DS_SID_OR_SID_HISTORY_NAME  = 11,
  DS_DNS_DOMAIN_NAME          = 12
} DS_NAME_FORMAT;

NTSTATUS WINAPI CredMarshalTargetInfo(
  _In_   PCREDENTIAL_TARGET_INFORMATIONW InTargetInfo,
  _Out_  PUSHORT *Buffer,
  PULONG BufferSize
)
{
	return 0x00000000;
}

SECURITY_STATUS SEC_ENTRY SaslAcceptSecurityContext(
  _In_       PCredHandle phCredential,
  _In_opt_   PCtxtHandle phContext,
  _In_       PSecBufferDesc pInput,
  _In_       unsigned long fContextReq,
  _In_       unsigned long TargetDataRep,
  _Out_      PCtxtHandle phNewContext,
  _Inout_    PSecBufferDesc pOutput,
  _Out_      unsigned long SEC_FAR *pfContextAttr,
  _Out_opt_  PTimeStamp ptsExpiry
)
{
	return 0x00000000;	
}

SECURITY_STATUS SEC_ENTRY SaslEnumerateProfilesA(
  _Out_  LPTSTR *ProfileList,
  _Out_  ULONG *ProfileCount
)
{
	return 0x00000000;	
}

SECURITY_STATUS SEC_ENTRY SaslEnumerateProfilesW(
  _Out_  LPWSTR *ProfileList,
  _Out_  ULONG *ProfileCount
)
{
	return 0x00000000;	
}

SECURITY_STATUS SEC_ENTRY SaslGetProfilePackageA(
  _In_   LPTSTR ProfileName,
  _Out_  PSecPkgInfo *PackageInfo
)
{
	return 0x00000000;	
}

SECURITY_STATUS SEC_ENTRY SaslGetProfilePackageW(
  _In_   LPWSTR ProfileName,
  _Out_  PSecPkgInfo *PackageInfo
)
{
	return 0x00000000;	
}

SECURITY_STATUS SEC_ENTRY SaslIdentifyPackageA(
  _In_   PSecBufferDesc pInput,
  _Out_  PSecPkgInfo *PackageInfo
)
{
	return 0x00000000;	
}

SECURITY_STATUS SEC_ENTRY SaslIdentifyPackageW(
  _In_   PSecBufferDesc pInput,
  _Out_  PSecPkgInfo *PackageInfo
)
{
	return 0x00000000;	
}

SECURITY_STATUS SEC_ENTRY SaslInitializeSecurityContextA(
  _In_       PCredHandle phCredential,
  _In_       PCtxtHandle phContext,
  _In_       LPTSTR pszTargetName,
  _In_       unsigned long fContextReq,
  _In_       unsigned long Reserved1,
  _In_       unsigned long TargetDataRep,
  _In_       PSecBufferDesc pInput,
  _In_       unsigned long Reserved2,
  _Out_      PCtxtHandle phNewContext,
  _Inout_    PSecBufferDesc pOutput,
  _Out_      unsigned long SEC_FAR *pfContextAttr,
  _Out_opt_  PTimeStamp ptsExpiry
)
{
	return 0x00000000;	
}

SECURITY_STATUS SEC_ENTRY SaslInitializeSecurityContextW(
  _In_       PCredHandle phCredential,
  _In_       PCtxtHandle phContext,
  _In_       LPWSTR pszTargetName,
  _In_       unsigned long fContextReq,
  _In_       unsigned long Reserved1,
  _In_       unsigned long TargetDataRep,
  _In_       PSecBufferDesc pInput,
  _In_       unsigned long Reserved2,
  _Out_      PCtxtHandle phNewContext,
  _Inout_    PSecBufferDesc pOutput,
  _Out_      unsigned long SEC_FAR *pfContextAttr,
  _Out_opt_  PTimeStamp ptsExpiry
)
{
	return 0x00000000;	
}

HLOCAL SEC_ENTRY SecpFreeMemory(HLOCAL hMem)
{
  return LocalFree(hMem);
}

SECURITY_STATUS SEC_ENTRY SecDeleteUserModeContext(PCtxtHandle phNewContext)
{
	return 0x00000000;	
}

SECURITY_STATUS SEC_ENTRY SecInitUserModeContext(PCtxtHandle phOldContext, PCtxtHandle phNewContext )
{
	return 0x00000000;	
}

BOOL SEC_ENTRY SecpTranslateName(HLOCAL DomainName, LPWSTR rpNames, DS_NAME_FORMAT formatOffered, DS_NAME_FORMAT formatDesired, int a5, int a6)
{
	return TRUE;
}

BOOL SEC_ENTRY SecpTranslateNameEx(LPCWSTR DomainName, LPCWSTR rpNames, DS_NAME_FORMAT formatOffered, DS_NAME_FORMAT formatDesired, int a5, int a6)
{
	return TRUE;
}

SECURITY_STATUS SEC_ENTRY CredUnmarshalTargetInfo(ULONG a1, int a2, int a3, int a4)
{
	return 0x00000000;	
}

BOOL SEC_ENTRY SecCacheSspiPackages()
{
	return TRUE;
}


SECURITY_STATUS SEC_ENTRY SaslGetContextOption(
  _In_       PCtxtHandle ContextHandle,
  _In_       ULONG Option,
  _Out_      PVOID Value,
  _Out_      ULONG Size,
  _Out_opt_  PULONG Needed
)
{
	return 0x00000000;	
}


SECURITY_STATUS SEC_ENTRY SaslSetContextOption(
  _In_  PCtxtHandle ContextHandle,
  _In_  ULONG Option,
  _In_  PVOID Value,
  _In_  ULONG Size
)
{
	return 0x00000000;	
}

SECURITY_STATUS SEC_ENTRY SecpSetIPAddress(PVOID a1, ULONG a2)
{
	return 0x00000000;	
}

SECURITY_STATUS WINAPI SetCredentialsAttributesA(
  _In_  PCredHandle phCredential,
  _In_  unsigned long ulAttribute,
  _In_  void *pBuffer,
  unsigned long cbBuffer
)
{
	return 0x00000000;	
}

SECURITY_STATUS WINAPI SetCredentialsAttributesW(
  _In_  PCredHandle phCredential,
  _In_  unsigned long ulAttribute,
  _In_  void *pBuffer,
  unsigned long cbBuffer
)
{
	return 0x00000000;	
}

SECURITY_STATUS
WINAPI
ApplyControlTokenW(PCtxtHandle Handle,
                  PSecBufferDesc Buffer)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}

SECURITY_STATUS
WINAPI
ApplyControlTokenA(PCtxtHandle Handle,
                  PSecBufferDesc Buffer)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}