/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    hooks.c

Abstract:

    Hooks native functions to implement new features
	
Author:

    Skulltrail 18-July-2021

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(advapi32_hooks); 

BOOL
APIENTRY
GetTokenInformationInternal (
    HANDLE TokenHandle,
    TOKEN_INFORMATION_CLASS TokenInformationClass,
    PVOID TokenInformation,
    DWORD TokenInformationLength,
    PDWORD ReturnLength
    )
{
    // NTSTATUS Status;

    // Status = NtQueryInformationToken (
        // TokenHandle,
        // TokenInformationClass,
        // TokenInformation,
        // TokenInformationLength,
        // ReturnLength
        // );
	// DbgPrint("GetTokenInformationInternal :: NtQueryInformationToken Status: %08x\n",Status);			
		

    // if ( !NT_SUCCESS(Status) ) {
        // //BaseSetLastNTError(Status);
		// DbgPrint("GetTokenInformation :: return FALSE\n",Status);
        // return FALSE;
    // }

    // return TRUE;
	BOOL ret;
	
	ret = GetTokenInformation(TokenHandle,
							  TokenInformationClass,
							  TokenInformation,
							  TokenInformationLength,
							  ReturnLength);
							  
	if(!ret){
		//DbgPrint("GetTokenInformation :: returned False\n");	
	}

	return ret;		
	
}

BOOL
APIENTRY
SetTokenInformationInternal (
    HANDLE TokenHandle,
    TOKEN_INFORMATION_CLASS TokenInformationClass,
    PVOID TokenInformation,
    DWORD TokenInformationLength
    )
{ 
    // NTSTATUS Status;

    // Status = NtSetInformationToken (
        // TokenHandle,
        // TokenInformationClass,
        // TokenInformation,
        // TokenInformationLength
        // );

	// DbgPrint("SetTokenInformation :: NtSetInformationToken Status: %08x\n",Status);
	
	// if(!NT_SUCCESS(Status)){
		// DbgPrint("SetTokenInformation :: return FALSE\n",Status);
		// return FALSE; 
	// }
	
	// return TRUE;
	BOOL ret;
	
	ret = SetTokenInformation(TokenHandle,
							  TokenInformationClass,
							  TokenInformation,
							  TokenInformationLength);
							  
	if(!ret){
		DbgPrint("SetTokenInformation :: returned False\n");	
	}

	return ret;							  
}

BOOL 
WINAPI 
OpenThreadTokenInternal(
  _In_  HANDLE  ThreadHandle,
  _In_  DWORD   DesiredAccess,
  _In_  BOOL    OpenAsSelf,
  _Out_ PHANDLE TokenHandle
)
{
	// BOOL ret;
	// NTSTATUS status;
	
	// DbgPrint("OpenThreadTokenInternal :: caled\n");
	
	// status =  NtOpenThreadToken(ThreadHandle, DesiredAccess, OpenAsSelf, TokenHandle);
	
	// DbgPrint("OpenThreadToken :: Status: %08x\n",status);
	
	// if(!NT_SUCCESS(status)){
		// DbgPrint("OpenThreadToken :: return FALSE\n",status);
		// return FALSE; 
	// }
	
	// return TRUE;

	BOOL ret;
	
	ret = OpenThreadToken(ThreadHandle,
							  DesiredAccess,
							  OpenAsSelf,
							  TokenHandle);
							  
	if(!ret){
		//DbgPrint("OpenThreadToken :: returned False\n");	
	}
	
	return ret;			
}

/******************************************************************************
 * OpenProcessToken			[ADVAPI32.@]
 * Opens the access token associated with a process handle.
 *
 * PARAMS
 *   ProcessHandle [I] Handle to process
 *   DesiredAccess [I] Desired access to process
 *   TokenHandle   [O] Pointer to handle of open access token
 *
 * RETURNS
 *  Success: TRUE. TokenHandle contains the access token.
 *  Failure: FALSE.
 *
 * NOTES
 *  See NtOpenProcessToken.
 */
BOOL 
WINAPI
OpenProcessTokenInternal( 
	HANDLE ProcessHandle, 
	DWORD DesiredAccess,
    HANDLE *TokenHandle 
)
{
	// BOOL ret;
	// NTSTATUS status;
	
	// DbgPrint("OpenProcessTokenInternal :: caled\n");
	
	// status =  NtOpenProcessToken(ProcessHandle, DesiredAccess, TokenHandle);
	
	// DbgPrint("OpenProcessToken :: NtOpenProcessToken Status: %08x\n",status);
	
	// if(!NT_SUCCESS(status)){
		// DbgPrint("OpenProcessToken :: return FALSE\n",status);
		// return FALSE; 
	// }
	
	// return TRUE;
	BOOL ret;
	
	ret = OpenProcessToken(ProcessHandle,
							  DesiredAccess,
							  TokenHandle);
							  
	if(!ret){
		DbgPrint("OpenProcessToken :: returned False\n");	
	}	

	return ret;			
}

BOOL WINAPI CreateRestrictedTokenInternal(
    HANDLE baseToken,
    DWORD flags,
    DWORD nDisableSids,
    PSID_AND_ATTRIBUTES disableSids,
    DWORD nDeletePrivs,
    PLUID_AND_ATTRIBUTES deletePrivs,
    DWORD nRestrictSids,
    PSID_AND_ATTRIBUTES restrictSids,
    PHANDLE newToken)
{
    TOKEN_TYPE type;
    SECURITY_IMPERSONATION_LEVEL level = SecurityAnonymous;
    DWORD size;

    FIXME("(%p, 0x%x, %u, %p, %u, %p, %u, %p, %p): stub\n",
          baseToken, flags, nDisableSids, disableSids,
          nDeletePrivs, deletePrivs,
          nRestrictSids, restrictSids,
          newToken);

    size = sizeof(type);
    if (!GetTokenInformation( baseToken, TokenType, &type, size, &size )) return FALSE;
    if (type == TokenImpersonation)
    {
        size = sizeof(level);
        if (!GetTokenInformation( baseToken, TokenImpersonationLevel, &level, size, &size ))
            return FALSE;
    }
    return DuplicateTokenEx( baseToken, MAXIMUM_ALLOWED, NULL, level, type, newToken );
}

BOOL WINAPI GetKernelObjectSecurityInternal(
  _In_      HANDLE               Handle,
  _In_      SECURITY_INFORMATION RequestedInformation,
  _Out_opt_ PSECURITY_DESCRIPTOR pSecurityDescriptor,
  _In_      DWORD                nLength,
  _Out_     LPDWORD              lpnLengthNeeded
)
{
	//This is a hack, for now is enabled because need a truly implementation of LABEL_SECURITY_INFORMATION (for Chrome and Chromium Framework)
	if(RequestedInformation & LABEL_SECURITY_INFORMATION)
	{
		RequestedInformation = SACL_SECURITY_INFORMATION;
	}
	return GetKernelObjectSecurity(Handle, RequestedInformation, pSecurityDescriptor, nLength, lpnLengthNeeded);
}


BOOL WINAPI SetKernelObjectSecurityInternal(
  _In_ HANDLE               Handle,
  _In_ SECURITY_INFORMATION SecurityInformation,
  _In_ PSECURITY_DESCRIPTOR SecurityDescriptor
)
{
	//This is a hack, for now is enabled because need a truly implementation of LABEL_SECURITY_INFORMATION (for Chrome and Chromium Framework)
	if(SecurityInformation & LABEL_SECURITY_INFORMATION)
	{
		SecurityInformation = SACL_SECURITY_INFORMATION;
	}
	return SetKernelObjectSecurity(Handle, SecurityInformation, SecurityDescriptor);
}