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
#include <rtl.h>
#include <winsvc.h>
#include <wmistr.h>
#include <evntrace.h>
#include <wincred.h>
#include <winefs.h>
#include "wct.h"
#include "perflib.h"
#include <accctrl.h>

typedef LPVOID (CALLBACK *PERF_MEM_ALLOC)(SIZE_T AllocSize,LPVOID pContext);

typedef ULONG (WINAPI *PERFLIBREQUEST)(ULONG RequestCode,PVOID Buffer,ULONG BufferSize);

typedef void (CALLBACK *PERF_MEM_FREE)(LPVOID pBuffer,LPVOID pContext);

typedef VOID (CALLBACK *PFN_SC_NOTIFY_CALLBACK)( PVOID );

typedef PVOID PLSA_SID_NAME_MAPPING_OPERATION_INPUT;

typedef PVOID PLSA_SID_NAME_MAPPING_OPERATION_OUTPUT;

typedef HANDLE HWCT;

typedef HRESULT (*PCOGETCALLSTATE)(int, PULONG);

typedef HRESULT (*PCOGETACTIVATIONSTATE)(GUID, DWORD, DWORD*);

typedef enum _CRED_PROTECTION_TYPE { 
  CredUnprotected        = 0,
  CredUserProtection     = 1,
  CredTrustedProtection  = 2
} CRED_PROTECTION_TYPE, *PCRED_PROTECTION_TYPE;

typedef struct _POLICY_AUDIT_SID_ARRAY {
  ULONG UsersCount;
  PSID  *UserSidArray;
} POLICY_AUDIT_SID_ARRAY, *PPOLICY_AUDIT_SID_ARRAY;

typedef struct _AUDIT_POLICY_INFORMATION {
  GUID  AuditSubCategoryGuid;
  ULONG AuditingInformation;
  GUID  AuditCategoryGuid;
} AUDIT_POLICY_INFORMATION, *PAUDIT_POLICY_INFORMATION;

typedef enum _POLICY_AUDIT_EVENT_TYPE { 
  AuditCategorySystem,
  AuditCategoryLogon,
  AuditCategoryObjectAccess,
  AuditCategoryPrivilegeUse,
  AuditCategoryDetailedTracking,
  AuditCategoryPolicyChange,
  AuditCategoryAccountManagement,
  AuditCategoryDirectoryServiceAccess,
  AuditCategoryAccountLogon
} POLICY_AUDIT_EVENT_TYPE, *PPOLICY_AUDIT_EVENT_TYPE;

typedef enum _LSA_SID_NAME_MAPPING_OPERATION_TYPE { 
  Add          = 1,
  Remove       = 2,
  AddMultiple  = 3
} LSA_SID_NAME_MAPPING_OPERATION_TYPE;

BOOL WINAPI ControlServiceExA(
  _In_     SC_HANDLE hService,
  _In_     DWORD dwControl,
  _In_     DWORD dwInfoLevel,
  _Inout_  PVOID pControlParams
)
{
	return TRUE;
}

BOOL WINAPI ControlServiceExW(
  _In_     SC_HANDLE hService,
  _In_     DWORD dwControl,
  _In_     DWORD dwInfoLevel,
  _Inout_  PVOID pControlParams
)
{
	return TRUE;
}


LONG WINAPI RegCopyTreeA(
  _In_      HKEY hKeySrc,
  _In_opt_  LPCTSTR lpSubKey,
  _In_      HKEY hKeyDest
)
{
	return 0;
}
LONG WINAPI RegCopyTreeW(
  _In_      HKEY hKeySrc,
  _In_opt_  LPCWSTR lpSubKey,
  _In_      HKEY hKeyDest
)
{
	return 0;
}

BOOL WINAPI CredFindBestCredentialA(
  _In_   LPCTSTR TargetName,
  _In_   DWORD Type,
  _In_   DWORD Flags,
  _Out_  PCREDENTIALA *Credential
)
{
	return TRUE;
}

BOOL WINAPI CredFindBestCredentialW(
  _In_   LPCWSTR TargetName,
  _In_   DWORD Type,
  _In_   DWORD Flags,
  _Out_  PCREDENTIALW *Credential
)
{
	return TRUE;
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
	return 0;
}

BOOL WINAPI LogonUserExExW(
  _In_       LPTSTR lpszUsername,
  _In_opt_   LPTSTR lpszDomain,
  _In_opt_   LPTSTR lpszPassword,
  _In_       DWORD dwLogonType,
  _In_       DWORD dwLogonProvider,
  _In_opt_   PTOKEN_GROUPS pTokenGroups,
  _Out_opt_  PHANDLE phToken,
  _Out_opt_  PSID *ppLogonSid,
  _Out_opt_  PVOID *ppProfileBuffer,
  _Out_opt_  LPDWORD pdwProfileLength,
  _Out_opt_  PQUOTA_LIMITS pQuotaLimits
)
{
	return TRUE;
}

LONG WINAPI RegLoadMUIStringA(
  _In_       HKEY hKey,
  _In_opt_   LPCTSTR pszValue,
  _Out_opt_  LPTSTR pszOutBuf,
  _In_       DWORD cbOutBuf,
  _Out_opt_  LPDWORD pcbData,
  _In_       DWORD Flags,
  _In_opt_   LPCTSTR pszDirectory
)
{
	return 0;
}

LONG WINAPI RegLoadMUIStringW(
  _In_       HKEY hKey,
  _In_opt_   LPCWSTR pszValue,
  _Out_opt_  LPWSTR pszOutBuf,
  _In_       DWORD cbOutBuf,
  _Out_opt_  LPDWORD pcbData,
  _In_       DWORD Flags,
  _In_opt_   LPCWSTR pszDirectory
)
{
	return 0;
}

LONG WINAPI RegDeleteKeyValueA(
  _In_      HKEY hKey,
  _In_opt_  LPCTSTR lpSubKey,
  _In_opt_  LPCTSTR lpValueName
)
{
	return 0;
}

LONG WINAPI RegDeleteKeyValueW(
  _In_      HKEY hKey,
  _In_opt_  LPCWSTR lpSubKey,
  _In_opt_  LPCWSTR lpValueName
)
{
	return 0;
}

LONG WINAPI RegDeleteTreeA(
  _In_      HKEY hKey,
  _In_opt_  LPCTSTR lpSubKey
)
{
	return 0;
}

LONG WINAPI RegDeleteTreeW(
  _In_      HKEY hKey,
  _In_opt_  LPCWSTR lpSubKey
)
{
	return 0;
}

DWORD WINAPI NotifyServiceStatusChange(
  _In_  SC_HANDLE hService,
  _In_  DWORD dwNotifyMask,
  _In_  PSERVICE_NOTIFY pNotifyBuffer
)
{
	return ERROR_SUCCESS;
}

DWORD WINAPI InitiateShutdownW(
  _In_opt_  LPWSTR lpMachineName,
  _In_opt_  LPWSTR lpMessage,
  _In_      DWORD dwGracePeriod,
  _In_      DWORD dwShutdownFlags,
  _In_      DWORD dwReason
)
{
	return ERROR_SUCCESS;
}

DWORD WINAPI InitiateShutdownA(
  _In_opt_  LPSTR lpMachineName,
  _In_opt_  LPSTR lpMessage,
  _In_      DWORD dwGracePeriod,
  _In_      DWORD dwShutdownFlags,
  _In_      DWORD dwReason
)
{
	return ERROR_SUCCESS;
}

DWORD WINAPI GetEncryptedFileMetadata(LPCWSTR name, PDWORD value, PBYTE number)
{
	return ERROR_SUCCESS;
}

DWORD WINAPI FlushEfsCache(PVOID pInputData)
{
	return ERROR_SUCCESS;
}

void WINAPI FreeEncryptedFileMetadata(PBYTE palavra)
{
	;
}

DWORD WINAPI SetEncryptedFileMetadata(LPCWSTR name, 
									  PBYTE value, 
									  PBYTE number, 
									  PENCRYPTION_CERTIFICATE_HASH hash, 
									  DWORD flags, 
									  PENCRYPTION_CERTIFICATE_HASH_LIST list)
{
	return ERROR_SUCCESS;
}

LONG WINAPI RegDisablePredefinedCacheEx(void)
{
	return 0;
}

VOID WINAPI SetSecurityAccessMask(
  _In_   SECURITY_INFORMATION SecurityInformation,
  _Out_  LPDWORD DesiredAccess
)
{
	;
}

VOID WINAPI RegisterWaitChainCOMCallback(
  _In_  PCOGETCALLSTATE CallStateCallback,
  _In_  PCOGETACTIVATIONSTATE ActivationStateCallback
)
{
	;
}


BOOL WINAPI GetTokenInformation(
  _In_       HANDLE TokenHandle,
  _In_       TOKEN_INFORMATION_CLASS TokenInformationClass,
  _Out_opt_  LPVOID TokenInformation,
  _In_       DWORD TokenInformationLength,
  _Out_      PDWORD ReturnLength
)
{
	return TRUE;
}

DWORD
WINAPI
SetUserFileEncryptionKeyEx(
    __in            PENCRYPTION_CERTIFICATE     pEncryptionCertificate,
                    DWORD                       dwCapabilities, 
                    DWORD                       dwFlags,
    __reserved      LPVOID                      pvReserved
)
{
	return ERROR_SUCCESS;
}


LONG WINAPI RegSetKeyValueA(
  _In_      HKEY hKey,
  _In_opt_  LPCTSTR lpSubKey,
  _In_opt_  LPCTSTR lpValueName,
  _In_      DWORD dwType,
  _In_opt_  LPCVOID lpData,
  _In_      DWORD cbData
)
{
	return 0;
}

LONG WINAPI RegSetKeyValueW(
  _In_      HKEY hKey,
  _In_opt_  LPCWSTR lpSubKey,
  _In_opt_  LPCWSTR lpValueName,
  _In_      DWORD dwType,
  _In_opt_  LPCVOID lpData,
  _In_      DWORD cbData
)
{
	return 0;
}

VOID WINAPI QuerySecurityAccessMask(
  _In_   SECURITY_INFORMATION SecurityInformation,
  _Out_  LPDWORD DesiredAccess
)
{
	;
}

LONG WINAPI RegCreateKeyExA(
  _In_        HKEY hKey,
  _In_        LPCTSTR lpSubKey,
  _Reserved_  DWORD Reserved,
  _In_opt_    LPTSTR lpClass,
  _In_        DWORD dwOptions,
  _In_        REGSAM samDesired,
  _In_opt_    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  _Out_       PHKEY phkResult,
  _Out_opt_   LPDWORD lpdwDisposition
)
{
	return 0;
}

LONG WINAPI RegCreateKeyExW(
  _In_        HKEY hKey,
  _In_        LPCWSTR lpSubKey,
  _Reserved_  DWORD Reserved,
  _In_opt_    LPWSTR lpClass,
  _In_        DWORD dwOptions,
  _In_        REGSAM samDesired,
  _In_opt_    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  _Out_       PHKEY phkResult,
  _Out_opt_   LPDWORD lpdwDisposition
)
{
	return 0;
}

DWORD WINAPI I_ScSendPnPMessage(int a1, int a2, int a3, int a4, int a5, int a6)
{
	return 87;
}

DWORD WINAPI I_ScValidatePnPService(int a1, int a2, int a3)
{
	return 87;
}

VOID WINAPI CloseThreadWaitChainSession(
  _In_  HWCT WctHandle
)
{
	;
}

BOOL WINAPI CredIsProtectedA(
  _In_   LPTSTR pszProtectedCredentials,
  _Out_  CRED_PROTECTION_TYPE *pProtectionType
)
{
	return TRUE;
}

BOOL WINAPI CredIsProtectedW(
  _In_   LPWSTR pszProtectedCredentials,
  _Out_  CRED_PROTECTION_TYPE *pProtectionType
)
{
	return TRUE;
}

BOOL WINAPI CredProtectA(
  _In_     BOOL fAsSelf,
  _In_     LPTSTR pszCredentials,
  _In_     DWORD cchCredentials,
  _Out_    LPTSTR pszProtectedCredentials,
  _Inout_  DWORD *pcchMaxChars,
  _Out_    CRED_PROTECTION_TYPE *ProtectionType
)
{
	return TRUE;
}

BOOL WINAPI CredProtectW(
  _In_     BOOL fAsSelf,
  _In_     LPWSTR pszCredentials,
  _In_     DWORD cchCredentials,
  _Out_    LPWSTR pszProtectedCredentials,
  _Inout_  DWORD *pcchMaxChars,
  _Out_    CRED_PROTECTION_TYPE *ProtectionType
)
{
	return TRUE;
}


BOOL WINAPI CredUnprotectA(
  _In_     BOOL fAsSelf,
  _In_     LPTSTR pszProtectedCredentials,
  _In_     DWORD cchCredentials,
  _Out_    LPTSTR pszCredentials,
  _Inout_  DWORD *pcchMaxChars
)
{
	return TRUE;
}

BOOL WINAPI CredUnprotectW(
  _In_     BOOL fAsSelf,
  _In_     LPWSTR pszProtectedCredentials,
  _In_     DWORD cchCredentials,
  _Out_    LPWSTR pszCredentials,
  _Inout_  DWORD *pcchMaxChars
)
{
	return TRUE;
}


ULONG WINAPI EnumerateTraceGuidsEx(
  _In_   TRACE_QUERY_INFO_CLASS TraceQueryInfoClass,
  _In_   PVOID InBuffer,
  _In_   ULONG InBufferSize,
  _Out_  PVOID OutBuffer,
  _In_   ULONG OutBufferSize,
  _Out_  PULONG ReturnLength
)
{
	return 0;
}

ULONG WINAPI EventAccessControl(
  _In_  LPGUID Guid,
  _In_  ULONG Operation,
  _In_  PSID Sid,
  _In_  ULONG Rights,
  _In_  BOOLEAN AllowOrDeny
)
{
	return 0;
}

ULONG WINAPI EventAccessQuery(
  _In_     LPGUID Guid,
  _Inout_  PSECURITY_DESCRIPTOR Buffer,
  _Inout_  PULONG BufferSize
)
{
	return 0;
}

ULONG WINAPI EventAccessRemove(
  _In_  LPGUID Guid
)
{
	return 0;
}


BOOL WINAPI GetThreadWaitChain(
  _In_      HWCT WctHandle,
  _In_opt_  DWORD_PTR Context,
  _In_      DWORD Flags,
  _In_      DWORD ThreadId,
  _Inout_   LPDWORD NodeCount,
  _Out_     PWAITCHAIN_NODE_INFO NodeInfoArray,
  _Out_     LPBOOL IsCycle
)
{
	return TRUE;
}

HWCT WINAPI
OpenThreadWaitChainSession (
    __in DWORD Flags,
    __in_opt PWAITCHAINCALLBACK callback
)
{
	return NULL;
}

LONG WINAPI RegLoadKeyA(
  _In_      HKEY hKey,
  _In_opt_  LPCTSTR lpSubKey,
  _In_      LPCTSTR lpFile
)
{
	return 0;
}

LONG WINAPI RegLoadKeyW(
  _In_      HKEY hKey,
  _In_opt_  LPCWSTR lpSubKey,
  _In_      LPCWSTR lpFile
)
{
	return 0;
}

LONG WINAPI RegOpenKeyTransactedA(
  _In_        HKEY hKey,
  _In_opt_    LPCTSTR lpSubKey,
  _In_        DWORD ulOptions,
  _In_        REGSAM samDesired,
  _Out_       PHKEY phkResult,
  _In_        HANDLE hTransaction,
  _Reserved_  PVOID pExtendedParameter
)
{
	return 0;
}

LONG WINAPI RegOpenKeyTransactedW(
  _In_        HKEY hKey,
  _In_opt_    LPCWSTR lpSubKey,
  _In_        DWORD ulOptions,
  _In_        REGSAM samDesired,
  _Out_       PHKEY phkResult,
  _In_        HANDLE hTransaction,
  _Reserved_  PVOID pExtendedParameter
)
{
	return 0;
}

LSTATUS WINAPI RegRenameKey(HKEY hKey, LPCWSTR lpSubKey, PCWSTR a3)
{
	return 0x00000000;
}

LONG WINAPI RegLoadAppKeyA(
  _In_        LPCTSTR lpFile,
  _Out_       PHKEY phkResult,
  _In_        REGSAM samDesired,
  _In_        DWORD dwOptions,
  _Reserved_  DWORD Reserved
)
{
	return 0;
}

LONG WINAPI RegLoadAppKeyW(
  _In_        LPCWSTR lpFile,
  _Out_       PHKEY phkResult,
  _In_        REGSAM samDesired,
  _In_        DWORD dwOptions,
  _Reserved_  DWORD Reserved
)
{
	return 0;
}

ULONG WINAPI PerfStopProvider(HANDLE ProviderHandle)
{
	return 0;
}

ULONG WINAPI PerfStartProviderEx(LPGUID ProviderGuid, 
									PPERF_PROVIDER_CONTEXT ProviderContext, 
									PHANDLE Provider)
{
	return 0;
}

ULONG WINAPI PerfSetCounterRefValue(
  _In_  HANDLE hProvider,
  _In_  PPERF_COUNTERSET_INSTANCE pInstance,
  _In_  ULONG CounterId,
  _In_  PVOID lpAddr
)
{
	return 0;
}	

									
ULONG WINAPI PerfStartProvider(LPGUID ProviderGuid, 
							   PERFLIBREQUEST ControlCallback,
							   HANDLE *phProvider)
{
	return 0;
}	

ULONG WINAPI PerfSetULongLongCounterValue(HANDLE Provider, 
										  PPERF_COUNTERSET_INSTANCE Instance, 
										  ULONG CounterId, 
										  ULONGLONG Value)	
{
	return 0;
}	
	
ULONG WINAPI PerfSetULongCounterValue(HANDLE Provider, 
									  PPERF_COUNTERSET_INSTANCE Instance, 
									  ULONG CounterId, 
									  ULONG Value)
{
	return 0;
}	

ULONG WINAPI PerfSetCounterSetInfo(HANDLE ProviderHandle, 
								   PPERF_COUNTERSET_INFO Template, 
								   ULONG TemplateSize)									  
{
	return 0;
}		

ULONG WINAPI PerfAddCounters(HANDLE ProviderHandle, 
							 PPERF_COUNTERSET_INFO Template, 
							 ULONG TemplateSize)									  
{
	return 0;
}

ULONG WINAPI PerfCloseQueryHandle(HANDLE *cbDest)
{
	return 0;
}	

PPERF_COUNTERSET_INSTANCE WINAPI PerfCreateInstance(
  _In_  HANDLE hProvider,
  _In_  LPCGUID CounterSetGuid,
  _In_  LPCWSTR szInstanceName,
  _In_  ULONG dwInstance
)
{
	return NULL;
}	

ULONG WINAPI PerfDecrementULongCounterValue(
  _In_  HANDLE hProvider,
  _In_  PPERF_COUNTERSET_INSTANCE pInstance,
  _In_  ULONG CounterId,
  _In_  ULONG lValue
)
{
	return 0;
}	

ULONG WINAPI PerfDecrementULongLongCounterValue(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE pInstance,
	ULONG CounterId,
	ULONGLONG llValue
)
{
	return 0;
}

ULONG WINAPI PerfDeleteCounters(HANDLE hProvider, PPERF_COUNTERSET_INSTANCE pInstance, ULONG a3)
{
	return 0;
}

ULONG WINAPI PerfDeleteInstance(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE InstanceBlock
)
{
	return 0;
}

ULONG WINAPI PerfEnumerateCounterSet(DWORD StringBinding, 
									 PPERF_COUNTERSET_INSTANCE InstanceBlock, 
									 ULONG a3, 
									 DWORD a4)
{
	return 0;
}	

ULONG WINAPI PerfEnumerateCounterSetInstances(DWORD StringBinding, 
									 PPERF_COUNTERSET_INSTANCE InstanceBlock, 
									 RPC_BINDING_HANDLE a3, 
									 DWORD a4,
									 ULONG a5)
{
	return 0;
}	

ULONG WINAPI PerfIncrementULongCounterValue(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE pInstance,
	ULONG CounterId,
	ULONG lValue
)
{
	return 0;
}	


ULONG WINAPI PerfIncrementULongLongCounterValue(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE pInstance,
	ULONG CounterId,
	ULONGLONG llValue
)	
{
	return 0;
}	
	
PPERF_COUNTERSET_INSTANCE WINAPI PerfQueryInstance(
	HANDLE hProvider,
	LPCGUID CounterSetGuid,
	LPCWSTR szInstance,
	ULONG dwInstance
)
{
	return NULL;
}	

PPERF_COUNTERSET_INSTANCE PerfOpenQueryHandle(DWORD StringBinding, HANDLE a2)	
{
	return NULL;
}	

ULONG WINAPI PerfQueryCounterData(HANDLE a1, PPERF_COUNTERSET_INFO a2, ULONG a3, int a4)
{
	return 0;
}	

ULONG WINAPI PerfQueryCounterInfo(HANDLE a1, PPERF_COUNTERSET_INFO a2, ULONG a3, int a4)
{
	return 0;
}

ULONG WINAPI PerfQueryCounterSetRegistrationInfo(DWORD StringBinding, int a2, int a3, WORD wLanguage, RPC_BINDING_HANDLE Dst, size_t Size, int a7)
{
	return 0;
}

BOOLEAN WINAPI AuditComputeEffectivePolicyBySid(
  _In_   const PSID pSid,
  _In_   const GUID *pSubCategoryGuids,
  _In_   ULONG PolicyCount,
  _Out_  PAUDIT_POLICY_INFORMATION *ppAuditPolicy
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditComputeEffectivePolicyByToken(
  _In_   HANDLE hTokenHandle,
  _In_   const GUID *pSubCategoryGuids,
  _In_   ULONG PolicyCount,
  _Out_  PAUDIT_POLICY_INFORMATION *ppAuditPolicy
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditEnumerateCategories(
  _Out_  GUID **ppAuditCategoriesArray,
  _Out_  PULONG pCountReturned
)
{
	return TRUE;
}


BOOLEAN WINAPI AuditEnumeratePerUserPolicy(
  _Out_  PPOLICY_AUDIT_SID_ARRAY *ppAuditSidArray
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditEnumerateSubCategories(
  _In_   const GUID *pAuditCategoryGuid,
  _In_   BOOLEAN bRetrieveAllSubCategories,
  _Out_  GUID **ppAuditSubCategoriesArray,
  _Out_  PULONG pCountReturned
)
{
	return TRUE;
}

VOID WINAPI AuditFree(
  _In_  PVOID Buffer
)
{
	;
}

BOOLEAN WINAPI AuditLookupCategoryGuidFromCategoryId(
  _In_   POLICY_AUDIT_EVENT_TYPE AuditCategoryId,
  _Out_  GUID *pAuditCategoryGuid
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditLookupCategoryIdFromCategoryGuid(
  _In_   const GUID *pAuditCategoryGuid,
  _Out_  PPOLICY_AUDIT_EVENT_TYPE pAuditCategoryId
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditLookupCategoryNameA(
  _In_   const GUID *pAuditCategoryGuid,
  _Out_  PTSTR *ppszCategoryName
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditLookupCategoryNameW(
  _In_   const GUID *pAuditCategoryGuid,
  _Out_  PWSTR *ppszCategoryName
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditLookupSubCategoryNameA(
  _In_   const GUID *pAuditSubCategoryGuid,
  _Out_  PTSTR *ppszSubCategoryName
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditLookupSubCategoryNameW(
  _In_   const GUID *pAuditSubCategoryGuid,
  _Out_  PWSTR *ppszSubCategoryName
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditQueryPerUserPolicy(
  _In_   const PSID pSid,
  _In_   const GUID *pSubCategoryGuids,
  _In_   ULONG PolicyCount,
  _Out_  PAUDIT_POLICY_INFORMATION *ppAuditPolicy
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditQuerySecurity(
  _In_   SECURITY_INFORMATION SecurityInformation,
  _Out_  PSECURITY_DESCRIPTOR *ppSecurityDescriptor
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditQuerySystemPolicy(
  _In_   const GUID *pSubCategoryGuids,
  _In_   ULONG PolicyCount,
  _Out_  PAUDIT_POLICY_INFORMATION *ppAuditPolicy
)
{
	return TRUE;
}


BOOLEAN WINAPI AuditSetPerUserPolicy(
  _In_  const PSID pSid,
  _In_  PAUDIT_POLICY_INFORMATION pAuditPolicy,
  _In_  ULONG PolicyCount
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditSetSecurity(
  _In_  SECURITY_INFORMATION SecurityInformation,
  _In_  PSECURITY_DESCRIPTOR pSecurityDescriptor
)
{
	return TRUE;
}

BOOLEAN WINAPI AuditSetSystemPolicy(
  _In_  PAUDIT_POLICY_INFORMATION pAuditPolicy,
  _In_  ULONG PolicyCount
)
{
	return TRUE;
}

BOOLEAN WINAPI CredBackupCredentials(int a1, PVOID Src, PVOID a3, size_t Size, int a5)
{
	return TRUE;
}

BOOLEAN WINAPI CredRestoreCredentials(PVOID Src, PVOID a2, size_t Size, int a4)
{
	return TRUE;
}

BOOL WINAPI IsValidRelativeSecurityDescriptor(PSECURITY_DESCRIPTOR pSecurityDescriptor, 
											  ULONG SecurityDescriptorLength, 
											  SECURITY_INFORMATION RequiredInformation)
{
	return TRUE;
}

DWORD WINAPI TreeSetNamedSecurityInfoA(
  _In_      LPTSTR pObjectName,
  _In_      SE_OBJECT_TYPE ObjectType,
  _In_      SECURITY_INFORMATION SecurityInfo,
  _In_opt_  PSID pOwner,
  _In_opt_  PSID pGroup,
  _In_opt_  PACL pDacl,
  _In_opt_  PACL pSacl,
  _In_      DWORD dwAction,
  _In_      FN_PROGRESS fnProgress,
  _In_      PROG_INVOKE_SETTING ProgressInvokeSetting,
  _In_opt_  PVOID Args
)
{
	return ERROR_SUCCESS;
}

DWORD WINAPI TreeSetNamedSecurityInfoW(
  _In_      LPWSTR pObjectName,
  _In_      SE_OBJECT_TYPE ObjectType,
  _In_      SECURITY_INFORMATION SecurityInfo,
  _In_opt_  PSID pOwner,
  _In_opt_  PSID pGroup,
  _In_opt_  PACL pDacl,
  _In_opt_  PACL pSacl,
  _In_      DWORD dwAction,
  _In_      FN_PROGRESS fnProgress,
  _In_      PROG_INVOKE_SETTING ProgressInvokeSetting,
  _In_opt_  PVOID Args
)
{
	return ERROR_SUCCESS;
}

LONG WINAPI RegCreateKeyTransactedA(
  _In_        HKEY hKey,
  _In_        LPCTSTR lpSubKey,
  _Reserved_  DWORD Reserved,
  _In_opt_    LPTSTR lpClass,
  _In_        DWORD dwOptions,
  _In_        REGSAM samDesired,
  _In_opt_    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  _Out_       PHKEY phkResult,
  _Out_opt_   LPDWORD lpdwDisposition,
  _In_        HANDLE hTransaction,
  _Reserved_  PVOID pExtendedParemeter
)
{
	return 0;
}

LONG WINAPI RegCreateKeyTransactedW(
  _In_        HKEY hKey,
  _In_        LPCWSTR lpSubKey,
  _Reserved_  DWORD Reserved,
  _In_opt_    LPWSTR lpClass,
  _In_        DWORD dwOptions,
  _In_        REGSAM samDesired,
  _In_opt_    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  _Out_       PHKEY phkResult,
  _Out_opt_   LPDWORD lpdwDisposition,
  _In_        HANDLE hTransaction,
  _Reserved_  PVOID pExtendedParemeter
)
{
	return 0;
}

LONG WINAPI RegDeleteKeyTransactedA(
  _In_        HKEY hKey,
  _In_        LPCTSTR lpSubKey,
  _In_        REGSAM samDesired,
  _Reserved_  DWORD Reserved,
  _In_        HANDLE hTransaction,
  _Reserved_  PVOID pExtendedParameter
)
{
	return 0;
}

LONG WINAPI RegDeleteKeyTransactedW(
  _In_        HKEY hKey,
  _In_        LPCWSTR lpSubKey,
  _In_        REGSAM samDesired,
  _Reserved_  DWORD Reserved,
  _In_        HANDLE hTransaction,
  _Reserved_  PVOID pExtendedParameter
)
{
	return 0;
}	
											  
void WINAPI LsaManageSidNameMapping(
  _In_   LSA_SID_NAME_MAPPING_OPERATION_TYPE OpType,
  _In_   PLSA_SID_NAME_MAPPING_OPERATION_INPUT OpInput,
  _Out_  PLSA_SID_NAME_MAPPING_OPERATION_OUTPUT *OpOutput
)
{
	;
}

DWORD WINAPI AddUsersToEncryptedFileEx(int a1, int a2, int a3, int a4)
{
	return 0;
}

BOOL WINAPI CheckAppInitBlockedServiceIdentity(BOOL a1)
{
	return TRUE;
}

NTSTATUS WINAPI CredEncryptAndMarshalBinaryBlob(int a1, int a2, int a3)
{
	return 0x00000000;
}

DWORD WINAPI CredpConvertOneCredentialSize(int a1, int a2)
{
	return 0;
}

DWORD WINAPI CredpEncodeSecret(int a1, PVOID Src, size_t Size, int a4, int a5)
{
	return 0;
}

BOOL WINAPI CredProfileUnloaded()
{
	return TRUE;
}

BOOL WINAPI CredReadByTokenHandle(int a1, PVOID Src, int a3, int a4, int a5)
{
	return TRUE;
}

BOOL WINAPI I_ScQueryServiceConfig(BOOL a1, int a2, int a3)
{
	return 0;
}

DWORD WINAPI UsePinForEncryptedFilesA(int a1, int a2, int a3)
{
	return 0;
}

DWORD WINAPI UsePinForEncryptedFilesW(int a1, int a2, int a3)
{
	return 0;
}

BOOL WINAPI CreateProcessWithTokenW(
		IN HANDLE  	hToken,
		IN DWORD  	dwLogonFlags,
		IN LPCWSTR lpApplicationName  	OPTIONAL,
		IN OUT LPWSTR lpCommandLine  	OPTIONAL,
		IN DWORD  	dwCreationFlags,
		IN LPVOID lpEnvironment  	OPTIONAL,
		IN LPCWSTR lpCurrentDirectory  	OPTIONAL,
		IN LPSTARTUPINFOW  	lpStartupInfo,
		OUT LPPROCESS_INFORMATION  	lpProcessInfo 
	) 		
{
    return FALSE;
}

NTSTATUS
NTAPI
ElfReportEventAndSourceW (
    IN      HANDLE          LogHandle,
    IN      ULONG EventTime,
    IN      PUNICODE_STRING pComputerNameU,
    IN      USHORT          EventType,
    IN      USHORT          EventCategory,
    IN      ULONG           EventID,
    IN      PSID            UserSid,
    IN      PUNICODE_STRING     UNCSourceName,
    IN      USHORT          NumStrings,
    IN      ULONG           DataSize,
    __in_ecount_opt(NumStrings)           PUNICODE_STRING *Strings,
    __in_bcount_opt(DataSize)      PVOID           Data,
    IN      USHORT          Flags,
    IN  PULONG          RecordNumber,
    IN  PULONG          TimeWritten
)
{
	return 0x00000000;	
}

DWORD WINAPI I_QueryTagInformation(int a1, int a2, int a3)
{
	return 0;
}

LSTATUS WINAPI RegConnectRegistryExA(LPCSTR lpMachineName, 
									 HKEY hKey, 
									 ULONG Flags, 
									 PHKEY phkResult)
{
	return 0x00000000;
}

LSTATUS WINAPI RegConnectRegistryExW(LPCWSTR lpMachineName, 
									 HKEY hKey, 
									 ULONG Flags, 
									 PHKEY phkResult)
{
	return 0x00000000;
}

LONG WINAPI RegDeleteKeyExA( 
		HKEY  	hKey,
		LPCSTR  	lpSubKey,
		REGSAM  	samDesired,
		DWORD  	Reserved 
) 	
{
	return 0;
}

LONG WINAPI RegDeleteKeyExW( 
		HKEY  	hKey,
		LPCWSTR  	lpSubKey,
		REGSAM  	samDesired,
		DWORD  	Reserved 
) 	
{
	return 0;
}

LONG WINAPI RegDisableReflectionKey(IN HKEY hBase) 	
{
	return 0;
}

LONG WINAPI RegEnableReflectionKey(IN HKEY hBase) 	
{
	return 0;
}

LSTATUS WINAPI RegGetValueA( 	
		HKEY  	hKey,
		LPCSTR  	pszSubKey,
		LPCSTR  	pszValue,
		DWORD  	dwFlags,
		LPDWORD  	pdwType,
		PVOID  	pvData,
		LPDWORD  	pcbData 
) 
{
	return 0x00000000;
}	

LSTATUS WINAPI RegGetValueW( 	
		HKEY  		hKey,
		LPCWSTR  	pszSubKey,
		LPCWSTR  	pszValue,
		DWORD  		dwFlags,
		LPDWORD  	pdwType,
		PVOID  		pvData,
		LPDWORD  	pcbData 
) 
{
	return 0x00000000;
}

LONG WINAPI RegQueryReflectionKey( 	
		IN HKEY  	hBase,
		OUT BOOL *  	bIsReflectionDisabled 
) 	
{
	return 0;
}

SERVICE_STATUS_HANDLE WINAPI RegisterServiceCtrlHandlerExA(
  _In_      LPCTSTR lpServiceName,
  _In_      LPHANDLER_FUNCTION_EX lpHandlerProc,
  _In_opt_  LPVOID lpContext
)
{
	return STATUS_SUCCESS;	
}

SERVICE_STATUS_HANDLE WINAPI RegisterServiceCtrlHandlerExW(
  _In_      LPCWSTR lpServiceName,
  _In_      LPHANDLER_FUNCTION_EX lpHandlerProc,
  _In_opt_  LPVOID lpContext
)
{
	return STATUS_SUCCESS;	
}

LONG 
WINAPI 
RegOpenKeyExA(
  _In_     HKEY    hKey,
  _In_opt_ LPCTSTR lpSubKey,
  _In_     DWORD   ulOptions,
  _In_     REGSAM  samDesired,
  _Out_    PHKEY   phkResult
)
{
	return 0;
}

LONG 
WINAPI 
RegOpenKeyExW(
  _In_     HKEY    hKey,
  _In_opt_ LPCWSTR lpSubKey,
  _In_     DWORD   ulOptions,
  _In_     REGSAM  samDesired,
  _Out_    PHKEY   phkResult
)
{
	return 0;
}

LONG 
WINAPI 
RegQueryValueExA(
  _In_        HKEY    hKey,
  _In_opt_    LPCTSTR lpValueName,
  _Reserved_  LPDWORD lpReserved,
  _Out_opt_   LPDWORD lpType,
  _Out_opt_   LPBYTE  lpData,
  _Inout_opt_ LPDWORD lpcbData
)
{
	return 0;
}

LONG 
WINAPI 
RegQueryValueExAW(
  _In_        HKEY    hKey,
  _In_opt_    LPCWSTR lpValueName,
  _Reserved_  LPDWORD lpReserved,
  _Out_opt_   LPDWORD lpType,
  _Out_opt_   LPBYTE  lpData,
  _Inout_opt_ LPDWORD lpcbData
)
{
	return 0;
}

BOOL WINAPI CreateRestrictedToken(
  _In_     HANDLE               ExistingTokenHandle,
  _In_     DWORD                Flags,
  _In_     DWORD                DisableSidCount,
  _In_opt_ PSID_AND_ATTRIBUTES  SidsToDisable,
  _In_     DWORD                DeletePrivilegeCount,
  _In_opt_ PLUID_AND_ATTRIBUTES PrivilegesToDelete,
  _In_     DWORD                RestrictedSidCount,
  _In_opt_ PSID_AND_ATTRIBUTES  SidsToRestrict,
  _Out_    PHANDLE              NewTokenHandle
)
{
	return FALSE;
}


BOOL WINAPI SetThreadToken(
  _In_opt_ PHANDLE Thread,
  _In_opt_ HANDLE  Token
)
{
	return FALSE;
}