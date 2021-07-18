/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    main.c

Abstract:

    Main and common functions

Author:

    Skulltrail 07-March-2021

Revision History:

--*/

/* INCLUDES ******************************************************************/

/* C Headers */
#include <rtl.h>
#include <winsvc.h>
#include <evntprov.h>
#include <ntsecapi.h>
#include <ndk/cmfuncs.h>
#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>


/* DEFINES ******************************************************************/
#define SHUTDOWN_GRACE_OVERRIDE 0x00000020
#define SHUTDOWN_RESTART 0x00000004
#define MAX_DEFAULT_HANDLES   6
#define REG_MAX_NAME_SIZE     256
#define REG_MAX_DATA_SIZE     2048
#define ClosePredefKey(Handle)                                                 \
    if ((ULONG_PTR)Handle & 0x1) {                                             \
        NtClose(Handle);                                                       \
    }
#define IsPredefKey(HKey)                                                      \
    (((ULONG_PTR)(HKey) & 0xF0000000) == 0x80000000)
#define GetPredefKeyIndex(HKey)                                                \
    ((ULONG_PTR)(HKey) & 0x0FFFFFFF)

	
WINE_DEFAULT_DEBUG_CHANNEL(advapi32); 	

/* PROTOTYPES ***************************************************************/
	
static NTSTATUS MapDefaultKey (PHANDLE ParentKey, HKEY Key);
//static VOID CloseDefaultKeys(VOID);
static NTSTATUS OpenClassesRootKey(PHANDLE KeyHandle);
static NTSTATUS OpenLocalMachineKey (PHANDLE KeyHandle);
static NTSTATUS OpenUsersKey (PHANDLE KeyHandle);
static NTSTATUS OpenCurrentConfigKey(PHANDLE KeyHandle);

#define ARGUMENT_PRESENT(ArgumentPointer)((CHAR*)((ULONG_PTR)(ArgumentPointer)) != (CHAR*)NULL)

/* GLOBALS ******************************************************************/

static RTL_CRITICAL_SECTION HandleTableCS;
static HANDLE DefaultHandleTable[MAX_DEFAULT_HANDLES];
static HANDLE ProcessHeap;
static BOOLEAN DefaultHandlesDisabled = FALSE;
static BOOLEAN DefaultHandleHKUDisabled = FALSE;
static BOOLEAN DllInitialized = FALSE; /* HACK */
static const UNICODE_STRING HKLM_ClassesPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\Software\\Classes");

typedef VOID( CALLBACK * PFN_SC_NOTIFY_CALLBACK ) (
    IN PVOID pParameter 
);

typedef PVOID LSA_HANDLE, *PLSA_HANDLE;

typedef ULONG64 TRACEHANDLE,*PTRACEHANDLE;

typedef struct _ENABLE_TRACE_PARAMETERS {
  ULONG                    Version;
  ULONG                    EnableProperty;
  ULONG                    ControlFlags;
  GUID                     SourceId;
  PEVENT_FILTER_DESCRIPTOR EnableFilterDesc;
  ULONG                    FilterDescCount;
} ENABLE_TRACE_PARAMETERS, *PENABLE_TRACE_PARAMETERS;

typedef enum _EVENT_INFO_CLASS { 
  EventProviderBinaryTrackInfo    = 0,
  EventProviderSetTraits          = 1,
  EventProviderUseDescriptorType  = 2,
  MaxEventInfo                    = 3
} EVENT_INFO_CLASS;

extern BOOL RegInitialize(VOID);
extern BOOL RegCleanup(VOID);
extern VOID UnloadNtMarta(VOID);
extern VOID CloseKsecDdHandle(VOID);
extern void ElfReportInitialize(VOID);

BOOL
WINAPI
DllMain(
    HINSTANCE hinstDll,
    DWORD dwReason,
    LPVOID reserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDll);
            //RegInitialize();
            //ElfReportInitialize();
            break;

        case DLL_PROCESS_DETACH:
            //CloseLogonLsaHandle();
            //RegCleanup();
            //UnloadNtMarta();
            //CloseKsecDdHandle();
            break;
    }

    return TRUE;
}

/*INTERNAL FUNCTIONS ********************************************/

FORCEINLINE BOOL IsHKCRKey 	( 	_In_ HKEY  	hKey	) 	
{
	return ((ULONG_PTR)hKey & 0x2) != 0;	
}

FORCEINLINE void MakeHKCRKey 	( 	_Inout_ HKEY *  	hKey	) 	
{
	*hKey = (HKEY)((ULONG_PTR)(*hKey) | 0x2);
}

/* unimplemented*/
ULONG 
WINAPI 
EventRegister(
  _In_      LPCGUID ProviderId,
  _In_opt_  PENABLECALLBACK EnableCallback,
  _In_opt_  PVOID CallbackContext,
  _Out_     PREGHANDLE RegHandle
)
{
	return ERROR_SUCCESS;
	//return EtwEventRegister(ProviderId, EnableCallback, CallbackContext, RegHandle);
}

/* unimplemented*/
ULONG WINAPI EventUnregister(
  _In_  REGHANDLE RegHandle
)
{
	//return EtwEventUnregister(RegHandle);
	return ERROR_SUCCESS;
}

/* unimplemented*/
ULONG WINAPI EventWrite(
  _In_      REGHANDLE RegHandle,
  _In_      PCEVENT_DESCRIPTOR EventDescriptor,
  _In_      ULONG UserDataCount,
  _In_opt_  PEVENT_DATA_DESCRIPTOR UserData
)
{
	//return EtwEventWrite(RegHandle, EventDescriptor, UserDataCount, UserData);
	return ERROR_SUCCESS;
}

ULONG WINAPI
EventWriteEndScenario(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	//return EtwEventWriteEndScenario(RegHandle, EventDescriptor, UserDataCount, UserData);
	return ERROR_SUCCESS;
}

ULONG 
WINAPI
EventWriteStartScenario(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	//return EtwEventWriteStartScenario(RegHandle, EventDescriptor, UserDataCount, UserData);
	return ERROR_SUCCESS;
}

ULONG WINAPI EventWriteString(
  _In_  REGHANDLE RegHandle,
  _In_  UCHAR Level,
  _In_  ULONGLONG Keyword,
  _In_  PCWSTR String
)
{
	return ERROR_SUCCESS;
}

/* unimplemented*/
BOOLEAN WINAPI EventEnabled(
  IN REGHANDLE RegHandle,
  IN PCEVENT_DESCRIPTOR EventDescriptor)
{
	//return EtwEventEnabled(RegHandle, EventDescriptor);
	return TRUE;
}

/* unimplemented*/
ULONG WINAPI EventWriteEx(
  _In_      REGHANDLE RegHandle,
  _In_      PCEVENT_DESCRIPTOR EventDescriptor,
  _In_      ULONG64 Filter,
  _In_      ULONG Flags,
  _In_opt_  LPCGUID ActivityId,
  _In_      LPCGUID RelatedActivityId,
  _In_      ULONG UserDataCount,
  _In_opt_  PEVENT_DATA_DESCRIPTOR UserData
)
{
	return ERROR_SUCCESS;
}

/******************************************************************************
 * load_string [Internal]
 *
 * This is basically a copy of user32/resource.c's LoadStringW. Necessary to
 * avoid importing user32, which is higher level than advapi32. Helper for
 * RegLoadMUIString.
 */
static int load_string(HINSTANCE hModule, UINT resId, LPWSTR pwszBuffer, INT cMaxChars)
{
    HGLOBAL hMemory;
    HRSRC hResource;
    WCHAR *pString;
    int idxString;

    /* Negative values have to be inverted. */
    if (HIWORD(resId) == 0xffff)
        resId = (UINT)(-((INT)resId));

    /* Load the resource into memory and get a pointer to it. */
    hResource = FindResourceW(hModule, MAKEINTRESOURCEW(LOWORD(resId >> 4) + 1), (LPWSTR)RT_STRING);
    if (!hResource) return 0;
    hMemory = LoadResource(hModule, hResource);
    if (!hMemory) return 0;
    pString = LockResource(hMemory);

    /* Strings are length-prefixed. Lowest nibble of resId is an index. */
    idxString = resId & 0xf;
    while (idxString--) pString += *pString + 1;

    /* If no buffer is given, return length of the string. */
    if (!pwszBuffer) return *pString;

    /* Else copy over the string, respecting the buffer size. */
    cMaxChars = (*pString < cMaxChars) ? *pString : (cMaxChars - 1);
    if (cMaxChars >= 0)
    {
        memcpy(pwszBuffer, pString+1, cMaxChars * sizeof(WCHAR));
        pwszBuffer[cMaxChars] = L'\0';
    }

    return cMaxChars;
}

ULONG WINAPI EnableTraceEx(
  _In_      LPCGUID ProviderId,
  _In_opt_  LPCGUID SourceId,
  _In_      TRACEHANDLE TraceHandle,
  _In_      ULONG IsEnabled,
  _In_      UCHAR Level,
  _In_      ULONGLONG MatchAnyKeyword,
  _In_      ULONGLONG MatchAllKeyword,
  _In_      ULONG EnableProperty,
  _In_opt_  PEVENT_FILTER_DESCRIPTOR EnableFilterDesc
)
{
	return ERROR_SUCCESS;	
}

/* unimplemented*/
ULONG WINAPI EventActivityIdControl(
  _In_     ULONG ControlCode,
  _Inout_  LPGUID ActivityId
)
{
	//return EtwEventActivityIdControl(ControlCode, ActivityId);
	return ERROR_SUCCESS;
}

/* unimplemented*/
BOOLEAN WINAPI EventProviderEnabled(
  _In_  REGHANDLE RegHandle,
  _In_  UCHAR Level,
  _In_  ULONGLONG Keyword
)
{
	return TRUE;
}

/* unimplemented*/
ULONG WINAPI EventAccessControl(
  _In_  LPGUID Guid,
  _In_  ULONG Operation,
  _In_  PSID Sid,
  _In_  ULONG Rights,
  _In_  BOOLEAN AllowOrDeny
)
{
	return ERROR_SUCCESS;
}


ULONG WINAPI EventWriteTransfer(
  _In_      REGHANDLE RegHandle,
  _In_      PCEVENT_DESCRIPTOR EventDescriptor,
  _In_opt_  LPCGUID ActivityId,
  _In_      LPCGUID RelatedActivityId,
  _In_      ULONG UserDataCount,
  _In_opt_  PEVENT_DATA_DESCRIPTOR UserData
)
{
	return ERROR_SUCCESS;
}

/* implemented
DWORD WINAPI InitiateShutdownW(
  _In_opt_  LPWSTR lpMachineName,
  _In_opt_  LPWSTR lpMessage,
  _In_      DWORD dwGracePeriod,
  _In_      DWORD dwShutdownFlags,
  _In_      DWORD dwReason
)
{
	BOOL shutdown;
	DWORD time = dwGracePeriod;
	switch(dwShutdownFlags)
	{
		case SHUTDOWN_GRACE_OVERRIDE:
			time = 0;
			shutdown = 1;
			break;
		case SHUTDOWN_RESTART:
			shutdown = 0;
			break;
		default:
			shutdown = 1;
			break;
	}
	InitiateSystemShutdownW(lpMachineName,lpMessage, 0, 1, shutdown);
	return ERROR_SUCCESS;
}*/
/*
DWORD WINAPI NotifyServiceStatusChangeW(
  _In_  SC_HANDLE hService,
  _In_  DWORD dwNotifyMask,
  _In_  PSERVICE_NOTIFY pNotifyBuffer
)
{
	return ERROR_SUCCESS;
}
*/

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
  NTSTATUS resp; // eax@1
  BOOL result; // eax@2

  resp = RtlAddMandatoryAce(pAcl, dwAceRevision, AceFlags, MandatoryPolicy, 17, pLabelSid);
  if ( resp < 0 )
  {
    result = FALSE;
  }
  else
  {
    result = TRUE;
  }
  return result;
}

ULONG 
WINAPI
EventSetInformation(
  _In_ REGHANDLE        RegHandle,
  _In_ EVENT_INFO_CLASS InformationClass,
  _In_ PVOID            EventInformation,
  _In_ ULONG            InformationLength
)
{
	return ERROR_SUCCESS;
}

NTSTATUS 
WINAPI
LsaLookupSids2(
  _In_  LSA_HANDLE                  PolicyHandle,
  _In_  ULONG                       LookupOptions,
  _In_  ULONG                       Count,
  _In_  PSID                        *Sids,
  _Out_ PLSA_REFERENCED_DOMAIN_LIST *ReferencedDomains,
  _Out_ PLSA_TRANSLATED_NAME        *Names
)
{
	return LsaLookupSids(PolicyHandle,
						 Count,
						 Sids,
						 ReferencedDomains,
						 Names);
}

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

/* set last error code from NT status and get the proper boolean return value */
 /* used for functions that are a simple wrapper around the corresponding ntdll API */
 static inline BOOL set_ntstatus( NTSTATUS status )
 {
     if (status) SetLastError( RtlNtStatusToDosError( status ));
     return !status;
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

/*************************************************************************
 * CreateRestrictedToken [ADVAPI32.@]
 *
 * Create a new more restricted token from an existing token.
 *
 * PARAMS
 *   baseToken       [I] Token to base the new restricted token on
 *   flags           [I] Options
 *   nDisableSids    [I] Length of disableSids array
 *   disableSids     [I] Array of SIDs to disable in the new token
 *   nDeletePrivs    [I] Length of deletePrivs array
 *   deletePrivs     [I] Array of privileges to delete in the new token
 *   nRestrictSids   [I] Length of restrictSids array
 *   restrictSids    [I] Array of SIDs to restrict in the new token
 *   newToken        [O] Address where the new token is stored
 *
 * RETURNS
 *  Success: TRUE
 *  Failure: FALSE
 */
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