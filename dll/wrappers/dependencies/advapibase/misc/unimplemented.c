/* $Id: unimplemented.c 53225 2011-08-14 11:31:23Z akhaldi $
 *
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:     ReactOS system libraries
 * FILE:        lib/advapi32/misc/unimplemented.c
 * PURPOSE:     System unimplemented functions
 * PROGRAMMER:      Emanuele Aliberti
 * UPDATE HISTORY:
 *      19990413 EA     created
 *      19990515 EA
 */

#include <advapi32.h>
#include <Wmistr.h>
#include <evntrace.h>
#include <wincred.h>
#include <winefs.h>
WINE_DEFAULT_DEBUG_CHANNEL(advapi);

#define CYPHER_BLOCK_LENGTH 8
#define SAFER_MAX_HASH_SIZE 64

typedef const char *STRSAFE_LPCSTR;

typedef wchar_t *STRSAFE_LPWSTR;

typedef struct _CYPHER_BLOCK {
  CHAR data[CYPHER_BLOCK_LENGTH];
} CYPHER_BLOCK;

typedef struct _LM_OWF_PASSWORD {
  CYPHER_BLOCK data[2];
} LM_OWF_PASSWORD, *PLM_OWF_PASSWORD;

typedef struct _NT_OWF_PASSWORD {
  CYPHER_BLOCK data[2];
} NT_OWF_PASSWORD, *PNT_OWF_PASSWORD;

typedef struct _SAMPR_ENCRYPTED_USER_PASSWORD {
  UCHAR Buffer[ (256 * 2) + 4 ];
} SAMPR_ENCRYPTED_USER_PASSWORD, *PSAMPR_ENCRYPTED_USER_PASSWORD;

typedef struct _ENCRYPTED_NT_OWF_PASSWORD{
  CYPHER_BLOCK data[2];
} ENCRYPTED_NT_OWF_PASSWORD, *PENCRYPTED_NT_OWF_PASSWORD;

typedef struct _ENCRYPTED_LM_OWF_PASSWORD {
  CYPHER_BLOCK data[2];
} ENCRYPTED_LM_OWF_PASSWORD, *PENCRYPTED_LM_OWF_PASSWORD;

DWORD WINAPI ExportCallback(
  IN      PBYTE pbData,
  IN OPTIONAL  PVOID pvCallbackContext,
  IN      ULONG ulLength
);

typedef DWORD (WINAPI *PFE_EXPORT_FUNC)(
    IN      PBYTE pbData,
    IN OPTIONAL  PVOID pvCallbackContext,
    IN      ULONG ulLength
);

typedef enum _INSTALLSPECTYPE
{
    APPNAME = 1,
    FILEEXT,
    PROGID,
    COMCLASS
} INSTALLSPECTYPE;

typedef union _INSTALLSPEC
{
    struct
    {
        WCHAR * Name;
        GUID    GPOId;
    } AppName;
    WCHAR * FileExt;
    WCHAR * ProgId;
    struct
    {
        GUID    Clsid;
        DWORD   ClsCtx;
    } COMClass;
} INSTALLSPEC;

typedef struct _INSTALLDATA
{
    INSTALLSPECTYPE Type;
    INSTALLSPEC     Spec;
} INSTALLDATA, *PINSTALLDATA;

typedef struct _LOCALMANAGEDAPPLICATION
{
    LPWSTR      pszDeploymentName;
    LPWSTR      pszPolicyName;
    LPWSTR      pszProductId;
    DWORD       dwState;
} LOCALMANAGEDAPPLICATION, *PLOCALMANAGEDAPPLICATION;

typedef struct _APPCATEGORYINFO
{
    LCID        Locale;
    LPWSTR      pszDescription;
    GUID        AppCategoryId;
} APPCATEGORYINFO;

typedef struct _APPCATEGORYINFOLIST
{
    DWORD               cCategory;
#ifdef MIDL_PASS
    [size_is(cCategory)]
#endif
    APPCATEGORYINFO *   pCategoryInfo;
} APPCATEGORYINFOLIST;

typedef struct _MANAGEDAPPLICATION
{
    LPWSTR      pszPackageName;
    LPWSTR      pszPublisher;
    DWORD       dwVersionHi;
    DWORD       dwVersionLo;
    DWORD       dwRevision;
    GUID        GpoId;
    LPWSTR      pszPolicyName;
    GUID        ProductId;
    LANGID      Language;
    LPWSTR      pszOwner;
    LPWSTR      pszCompany;
    LPWSTR      pszComments;
    LPWSTR      pszContact;
    LPWSTR      pszSupportUrl;
    DWORD       dwPathType;
    BOOL        bInstalled;
} MANAGEDAPPLICATION, *PMANAGEDAPPLICATION;

DWORD WINAPI ImportCallback(
  IN      PBYTE pbData,
  IN OPTIONAL  PVOID pvCallbackContext,
  IN OUT   PULONG ulLength
);

typedef DWORD (WINAPI *PFE_IMPORT_FUNC)(
    IN      PBYTE pbData,
    IN OPTIONAL  PVOID pvCallbackContext,
    IN OUT   PULONG ulLength
);

BOOL WINAPI AccessCheckByTypeResultList(PSECURITY_DESCRIPTOR pSecurityDescriptor, 
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
	return TRUE;
}

DWORD WINAPI CancelOverlappedAccess(PACTRL_OVERLAPPED pOverlapped)
{
	return 0;
}

void WINAPI CloseEncryptedFileRaw(PVOID pvContext)
{
	;
}

ULONG WINAPI CloseTrace(TRACEHANDLE TraceHandle)
{
	return 0;
}

DWORD WINAPI ConvertAccessToSecurityDescriptorA(PACTRL_ACCESSA pAccessList, 
												PACTRL_AUDITA pAuditList, 
												LPCSTR lpOwner, 
												LPCSTR lpGroup, 
												PSECURITY_DESCRIPTOR *ppSecDescriptor)
{
	return 0;
}

DWORD WINAPI ConvertAccessToSecurityDescriptorW(PACTRL_ACCESSA pAccessList, 
												PACTRL_AUDITA pAuditList, 
												LPCWSTR lpOwner, 
												LPCWSTR lpGroup, 
												PSECURITY_DESCRIPTOR *ppSecDescriptor)
{
	return 0;
}

DWORD WINAPI ConvertSecurityDescriptorToAccessW(HANDLE hObject, 
												SE_OBJECT_TYPE ObjectType, 
												PSECURITY_DESCRIPTOR pSecDescriptor, 
												PACTRL_ACCESSW *ppAccessList, 
												PACTRL_AUDITW *ppAuditList, 
												LPWSTR *lppOwner, 
												LPWSTR *lppGroup)
{
	return 0;
}

DWORD WINAPI ConvertSecurityDescriptorToAccessA(HANDLE hObject, 
												SE_OBJECT_TYPE ObjectType, 
												PSECURITY_DESCRIPTOR pSecDescriptor, 
												PACTRL_ACCESSW *ppAccessList, 
												PACTRL_AUDITW *ppAuditList, 
												LPSTR *lppOwner, 
												LPSTR *lppGroup)
{
	return 0;
}


/*
 * @implemented
 */
BOOL WINAPI
CreateProcessAsUserSecure(HANDLE hToken,
                     LPCSTR lpApplicationName,
                     LPSTR lpCommandLine,
                     LPSECURITY_ATTRIBUTES lpProcessAttributes,
                     LPSECURITY_ATTRIBUTES lpThreadAttributes,
                     BOOL bInheritHandles,
                     DWORD dwCreationFlags,
                     LPVOID lpEnvironment,
                     LPCSTR lpCurrentDirectory,
                     LPSTARTUPINFOA lpStartupInfo,
                     LPPROCESS_INFORMATION lpProcessInformation)
{
	return TRUE;
}

ULONG WMIAPI CreateTraceInstanceId(
  IN  HANDLE RegHandle,
  IN OUT PEVENT_INSTANCE_INFO pInstInfo
)
{
	return 0;
}

BOOL WINAPI	CredGetTargetInfoA(LPCSTR TargetName, 
							   DWORD Flags, 
							   PCREDENTIAL_TARGET_INFORMATIONA *TargetInfo)
{
	return TRUE;
}

BOOL WINAPI	CredGetTargetInfoW(LPCWSTR TargetName, 
							   DWORD Flags, 
							   PCREDENTIAL_TARGET_INFORMATIONA *TargetInfo)
{
	return TRUE;
}

BOOL WINAPI CredProfileLoaded()
{
	return TRUE;
}

BOOL WINAPI CredRenameA(LPCSTR  OldTargetName,
				 LPCSTR  NewTargetName,
				 DWORD   Type,
				 DWORD   Flags 
) 	
{
	return TRUE;
}

BOOL WINAPI CredRenameW(LPCWSTR  OldTargetName,
				 LPCWSTR  NewTargetName,
				 DWORD   Type,
				 DWORD   Flags 
) 	
{
	return TRUE;
}

DWORD WINAPI DuplicateEncryptionInfoFile(LPCWSTR SrcFileName, 
										 LPCWSTR DstFileName,
										 DWORD dwCreationDistribution,
										 DWORD dwAttributes, 
										 LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	return 0;
}

ULONG WINAPI SetTraceCallback(LPCGUID pGuid, PEVENT_CALLBACK EventCallback)
{
	return 0;
}

DWORD WINAPI SetUserFileEncryptionKey(PENCRYPTION_CERTIFICATE pEncryptionCertificate)
{
	return 0;
}

DWORD WINAPI SetSecurityInfoExA(HANDLE hObject, 
								SE_OBJECT_TYPE ObjectType, 
								SECURITY_INFORMATION SecurityInfo,
								LPCSTR lpProvider, 
								PACTRL_ACCESSW pAccessList, 
								PACTRL_AUDITW pAuditList, 
								LPSTR lpOwner, 
								LPSTR lpGroup, 
								PACTRL_OVERLAPPED pOverlapped)
{
	return 0;
}


DWORD WINAPI SetSecurityInfoExW(HANDLE hObject, 
								SE_OBJECT_TYPE ObjectType, 
								SECURITY_INFORMATION SecurityInfo,
								LPCWSTR lpProvider, 
								PACTRL_ACCESSW pAccessList, 
								PACTRL_AUDITW pAuditList, 
								LPWSTR lpOwner, 
								LPWSTR lpGroup, 
								PACTRL_OVERLAPPED pOverlapped)
{
	return 0;
}

BOOL WINAPI SetPrivateObjectSecurityEx(SECURITY_INFORMATION SecurityInformation, 
									   PSECURITY_DESCRIPTOR ModificationDescriptor, 
									   PSECURITY_DESCRIPTOR *ObjectsSecurityDescriptor,
									   ULONG AutoInheritFlags, 
									   PGENERIC_MAPPING GenericMapping, 
									   HANDLE Token)
{
	return TRUE;
}


DWORD WINAPI SetNamedSecurityInfoExA(LPCSTR lpObject, 
								     SE_OBJECT_TYPE ObjectType, 
									 SECURITY_INFORMATION SecurityInfo, 
									 LPCSTR lpProvider, 
									 PACTRL_ACCESSW pAccessList, 
									 PACTRL_AUDITW pAuditList, 
									 LPSTR lpOwner, 
									 LPSTR lpGroup, 
									 PACTRL_OVERLAPPED pOverlapped)
{
	return 0;
}

DWORD WINAPI SetNamedSecurityInfoExW(LPCWSTR lpObject, 
								     SE_OBJECT_TYPE ObjectType, 
									 SECURITY_INFORMATION SecurityInfo, 
									 LPCWSTR lpProvider, 
									 PACTRL_ACCESSW pAccessList, 
									 PACTRL_AUDITW pAuditList, 
									 LPWSTR lpOwner, 
									 LPWSTR lpGroup, 
									 PACTRL_OVERLAPPED pOverlapped)
{
	return 0;
}

DWORD WINAPI SetEntriesInAuditListA(ULONG cEntries, 
								    PACTRL_ACCESS_ENTRYW pAccessEntryList, 
									ACCESS_MODE AccessMode, 
									LPCSTR lpProperty, 
									PACTRL_AUDITW pOldList, 
									PACTRL_AUDITW *ppNewList)
{
	return 0;
}

DWORD WINAPI SetEntriesInAuditListW(ULONG cEntries, 
								    PACTRL_ACCESS_ENTRYW pAccessEntryList, 
									ACCESS_MODE AccessMode, 
									LPCWSTR lpProperty, 
									PACTRL_AUDITW pOldList, 
									PACTRL_AUDITW *ppNewList)
{
	return 0;
}


DWORD WINAPI SetEntriesInAccessListA(ULONG cEntries, 
									 PACTRL_ACCESS_ENTRYA pAccessEntryList, 
									 ACCESS_MODE AccessMode, 
									 LPCSTR lpProperty, 
									 PACTRL_ACCESSA pOldList, 
									 PACTRL_ACCESSA *ppNewList)
{
	return 0;
}

DWORD WINAPI SetEntriesInAccessListW(ULONG cEntries, 
									 PACTRL_ACCESS_ENTRYA pAccessEntryList, 
									 ACCESS_MODE AccessMode, 
									 LPCWSTR lpProperty, 
									 PACTRL_ACCESSA pOldList, 
									 PACTRL_ACCESSA *ppNewList)
{
	return 0;
}


BOOL WINAPI SaferiSearchMatchingHashRules(ALG_ID HashAlgorithm,
										  PBYTE pHashBytes,
										  DWORD dwHashSize,
										  DWORD dwOriginalImageSize,
										  PDWORD pdwFoundLevel,
										  PDWORD pdwSaferFlags
)
{
	return TRUE;
}

BOOL WINAPI SaferiReplaceProcessThreadTokens(HANDLE HashAlgorithm,
										     PDWORD pdwFoundLevel,
										     PDWORD pdwSaferFlags
)
{
	return TRUE;
}

BOOL WINAPI LogonUserExA(
			LPTSTR lpszUsername,
			LPTSTR lpszDomain,
			LPTSTR lpszPassword,
			DWORD dwLogonType,
			DWORD dwLogonProvider,
			PHANDLE phToken,
			PSID *ppLogonSid,
			PVOID *ppProfileBuffer,
			LPDWORD pdwProfileLength,
			PQUOTA_LIMITS pQuotaLimits
)
{
	return TRUE;
}

BOOL WINAPI LogonUserExW(
			LPCWSTR lpszUsername,
			LPCWSTR lpszDomain,
			LPCWSTR lpszPassword,
			DWORD dwLogonType,
			DWORD dwLogonProvider,
			PHANDLE phToken,
			PSID *ppLogonSid,
			PVOID *ppProfileBuffer,
			LPDWORD pdwProfileLength,
			PQUOTA_LIMITS pQuotaLimits
)
{
	return TRUE;
}

BOOL WINAPI IsTokenUntrusted(HANDLE TokenHandle)
{
	return TRUE;
}

extern DWORD WINAPI MSChapSrvChangePassword(
  IN  PWSTR ServerName,
  IN  PWSTR UserName,
  IN  BOOLEAN LmOldPresent,
  IN  PLM_OWF_PASSWORD LmOldOwfPassword,
  IN  PLM_OWF_PASSWORD LmNewOwfPassword,
  IN  PNT_OWF_PASSWORD NtOldOwfPassword,
  IN  PNT_OWF_PASSWORD NtNewOwfPassword
)
{
	return 0;
}

extern DWORD WINAPI MSChapSrvChangePassword2(
  IN  PWSTR ServerName,
  IN  PWSTR UserName,
  IN  PSAMPR_ENCRYPTED_USER_PASSWORD NewPasswordEncryptedWithOldNt,
  IN  PENCRYPTED_NT_OWF_PASSWORD OldNtOwfPasswordEncryptedWithNewNt,
  IN  BOOLEAN LmPresent,
  IN  PSAMPR_ENCRYPTED_USER_PASSWORD NewPasswordEncryptedWithOldLm,
  IN  PENCRYPTED_LM_OWF_PASSWORD OldLmOwfPasswordEncryptedWithNewLmOrNt
)
{
	return 0;
}

DWORD WINAPI OpenEncryptedFileRawA(LPCSTR lpFileName, ULONG ulFlags, PVOID *pvContext)
{
	return 0;	
}

DWORD WINAPI OpenEncryptedFileRawW(LPCWSTR lpFileName, ULONG ulFlags, PVOID *pvContext)
{
	return 0;	
}

TRACEHANDLE WINAPI OpenTraceA(PEVENT_TRACE_LOGFILEA Logfile)
{
	return 0;	
}

TRACEHANDLE WINAPI OpenTraceW(PEVENT_TRACE_LOGFILEW Logfile)
{
	return 0;	
}

CLIENT_CALL_RETURN WINAPI ProcessIdleTasks()
{
	CLIENT_CALL_RETURN client; 
	client.Pointer = NULL;
	client.Simple = 0;
	return client;
}

ULONG WINAPI ProcessTrace(PTRACEHANDLE HandleArray, 
						  ULONG HandleCount, 
						  LPFILETIME StartTime, 
						  LPFILETIME EndTime
)
{
	return 0;
}

EXTERN_C
ULONG
WMIAPI
QueryTraceA(
  IN TRACEHANDLE TraceHandle,
  IN LPCSTR InstanceName OPTIONAL,
  IN OUT PEVENT_TRACE_PROPERTIES Properties)	
{
	return 0;
}

EXTERN_C
ULONG
WMIAPI
QueryTraceW(
  IN TRACEHANDLE TraceHandle,
  IN LPCWSTR InstanceName OPTIONAL,
  IN OUT PEVENT_TRACE_PROPERTIES Properties)	
{
	return 0;
}

DWORD WINAPI ReadEncryptedFileRaw(PFE_EXPORT_FUNC pfExportCallback, 
								  PVOID pvCallbackContext, 
								  PVOID pvContext)
{
	return 0;
}

ULONG WINAPI FlushTraceA(
  IN TRACEHANDLE TraceHandle,
  IN LPCSTR InstanceName OPTIONAL,
  IN OUT PEVENT_TRACE_PROPERTIES Properties
)
{
	return 0;
}

ULONG WINAPI FlushTraceW(
  IN TRACEHANDLE TraceHandle,
  IN LPCWSTR InstanceName OPTIONAL,
  IN OUT PEVENT_TRACE_PROPERTIES Properties
)
{
	return 0;
}


DWORD WINAPI GetAccessPermissionsForObjectA(LPCSTR lpObject, 
											SE_OBJECT_TYPE ObjectType, 
											LPCSTR lpObjType, 
											LPCSTR lpProvider, 
											PULONG pcEntries, 
											PACTRL_ACCESS_INFOA *ppAccessInfoList, 
											PULONG pcRights, 
											PACTRL_CONTROL_INFOA *ppRightsList, 
											PULONG pfAccessFlags)
{
	return 0;
}

DWORD WINAPI GetAccessPermissionsForObjectW(LPCWSTR lpObject, 
											SE_OBJECT_TYPE ObjectType, 
											LPCWSTR lpObjType, 
											LPCWSTR lpProvider, 
											PULONG pcEntries, 
											PACTRL_ACCESS_INFOW *ppAccessInfoList, 
											PULONG pcRights, 
											PACTRL_CONTROL_INFOW *ppRightsList, 
											PULONG pfAccessFlags)
{
	return 0;
}

DWORD WINAPI InstallApplication(PINSTALLDATA pInstallInfo)
{
	return 0;	
}

BOOL WINAPI I_ScSetServiceBits(int a1, int a2, int a3, int a4, int a5)
{
	return TRUE;
}

DWORD WINAPI I_ScSendTSMessage(int a1, int a2, int a3, int a4)
{
	return 0;
}

NTSTATUS WINAPI I_ScPnPGetServiceName(int a1, LPCSTR Dest, int a3)
{
	return 0x00000000;
}

DWORD WINAPI LookupSecurityDescriptorPartsA(PTRUSTEE_A *ppOwner, 
											PTRUSTEE_A *ppGroup, 
											PULONG pcCountOfAccessEntries, 
											PEXPLICIT_ACCESS_A *ppListOfAccessEntries, 
											PULONG pcCountOfAuditEntries, 
											PEXPLICIT_ACCESS_A *ppListOfAuditEntries, 
											PSECURITY_DESCRIPTOR pSD)
{
	return 0;
}

DWORD WINAPI LookupSecurityDescriptorPartsW(PTRUSTEE_W *ppOwner, 
											PTRUSTEE_W *ppGroup, 
											PULONG pcCountOfAccessEntries, 
											PEXPLICIT_ACCESS_W *ppListOfAccessEntries, 
											PULONG pcCountOfAuditEntries, 
											PEXPLICIT_ACCESS_W *ppListOfAuditEntries, 
											PSECURITY_DESCRIPTOR pSD)
{
	return 0;
}

DWORD WINAPI ConvertSDToStringSDRootDomainA(int a1, 
											PSECURITY_DESCRIPTOR OwnerDefaulted, 
											int cchWideChar, 
											int a4, 
											LPSTR *cbMultiByte, 
											int a6)
{
	return 0;
}

DWORD WINAPI ConvertSDToStringSDRootDomainW(int a1, 
											PSECURITY_DESCRIPTOR OwnerDefaulted, 
											int cchWideChar, 
											int a4, 
											LPWSTR *cbMultiByte, 
											int a6)
{
	return 0;
}

DWORD WINAPI ConvertSecurityDescriptorToAccessNamedA(LPCSTR lpObject, 
													 SE_OBJECT_TYPE ObjectType, 
													 PSECURITY_DESCRIPTOR pSecDescriptor,
													 PACTRL_ACCESSA *ppAccessList, 
													 PACTRL_AUDITA *ppAuditList, 
													 LPSTR *lppOwner, 
													 LPSTR *lppGroup)
{
	return 0;
}

DWORD WINAPI ConvertSecurityDescriptorToAccessNamedW(LPCWSTR lpObject, 
													 SE_OBJECT_TYPE ObjectType, 
													 PSECURITY_DESCRIPTOR pSecDescriptor,
													 PACTRL_ACCESSA *ppAccessList, 
													 PACTRL_AUDITA *ppAuditList, 
													 LPWSTR *lppOwner, 
													 LPWSTR *lppGroup)
{
	return 0;
}

DWORD WINAPI ConvertStringSDToSDDomainA(PSID Sid, 
									  DWORD a2, 
									  LPSTR SourceString, 
									  int a4, 
									  int a5, 
									  int a6)
{
	return 0;
}

DWORD WINAPI ConvertStringSDToSDDomainW(PSID Sid, 
									  DWORD a2, 
									  LPWSTR SourceString, 
									  int a4, 
									  int a5, 
									  int a6)
{
	return 0;
}

NTSTATUS WINAPI CredpConvertCredential(int a1, int a2, int a3, PVOID *a4)
{
	return 0x00000000;
}

ULONG WINAPI EnumerateTraceGuids(
  IN OUT  PTRACE_GUID_PROPERTIES *GuidPropertiesArray,
  IN     ULONG PropertyArrayCount,
  OUT    PULONG GuidCount
)
{
	return 0;
}

NTSTATUS WINAPI EncryptedFileKeyInfo(int a1, int a2, int a3)
{
	return 0x00000000;
}

NTSTATUS WINAPI FreeEncryptedFileKeyInfo(int a1)
{
	return 0x00000000;	
}

void WINAPI GetLocalManagedApplicationData(WCHAR *ProductCode, 
										   LPWSTR *DisplayName, 
										   LPWSTR *SupportUrl)
{
	;
}

DWORD WINAPI GetLocalManagedApplications(BOOL bUserApps, 
											LPDWORD pdwApps, 
											PLOCALMANAGEDAPPLICATION *prgLocalApps)
{
	return 0;
}

DWORD WINAPI GetManagedApplicationCategories(DWORD dwReserved, 
											 APPCATEGORYINFOLIST *pAppCategory)
{
	return 0;
}


DWORD WINAPI GetManagedApplications(
    GUID *               pCategory,
    DWORD                dwQueryFlags,
    DWORD                dwInfoLevel,
    LPDWORD              pdwApps,
    PMANAGEDAPPLICATION* prgManagedApps
)
{
	return 0;
}

DWORD WINAPI GetNamedSecurityInfoExA(LPCSTR lpObject, 
									 SE_OBJECT_TYPE ObjectType,
									 SECURITY_INFORMATION SecurityInfo,
									 LPCSTR lpProvider, 
									 LPCSTR lpProperty, 
									 PACTRL_ACCESSA *ppAccessList,
									 PACTRL_AUDITA *ppAuditList,
									 LPSTR *lppOwner, 
									 LPSTR *lppGroup)
{
	return 0;
}

DWORD WINAPI GetNamedSecurityInfoExW(LPCWSTR lpObject, 
									 SE_OBJECT_TYPE ObjectType,
									 SECURITY_INFORMATION SecurityInfo,
									 LPCWSTR lpProvider, 
									 LPCWSTR lpProperty, 
									 PACTRL_ACCESSA *ppAccessList,
									 PACTRL_AUDITA *ppAuditList,
									 LPWSTR *lppOwner, 
									 LPWSTR *lppGroup)
{
	return 0;
}

DWORD WINAPI GetOverlappedAccessResults(PACTRL_OVERLAPPED pOverlapped, 
										BOOL fWaitForCompletion, 
										PDWORD pResult, 
										PULONG pcItemsProcessed)
{
	return 0;
}

ULONG WINAPI RemoveTraceCallback(LPCGUID pGuid)
{
	return 0;
}

DWORD WINAPI RegisterIdleTask(int a1, int a2, HANDLE *a3, HANDLE *a4)
{
	return 0;
}

BOOL WINAPI SaferGetLevelInformation(
  IN       SAFER_LEVEL_HANDLE LevelHandle,
  IN       SAFER_OBJECT_INFO_CLASS dwInfoType,
  OUT OPTIONAL  LPVOID lpQueryBuffer,
  IN       DWORD dwInBufferSize,
  OUT      LPDWORD lpdwOutBufferSize
)
{
	return TRUE;
}

BOOL WINAPI SaferSetLevelInformation(
  IN  SAFER_LEVEL_HANDLE LevelHandle,
  IN  SAFER_OBJECT_INFO_CLASS dwInfoType,
  IN  LPVOID lpQueryBuffer,
  IN  DWORD dwInBufferSize
)
{
	return TRUE;
}

BOOL WINAPI SaferSetPolicyInformation( 	
		IN DWORD  	dwScopeId,
		IN SAFER_POLICY_INFO_CLASS  	SaferPolicyInfoClass,
		IN DWORD  	InfoBufferSize,
		_In_reads_bytes_(InfoBufferSize) PVOID  	InfoBuffer,
		_Reserved_ PVOID  	pReserved 
) 	
{
	return TRUE;
}

BOOL WINAPI SaferiChangeRegistryScope(PVOID a1, ULONG a2)
{
	return TRUE;
}

BOOL WINAPI SaferiIsExecutableFileType(
  IN  LPCWSTR szFullPath,
  IN  BOOLEAN bFromShellExecute
)
{
	return TRUE;
}

BOOL WINAPI SaferiPopulateDefaultsInRegistry(PVOID *a1, BOOLEAN a2)
{
	return TRUE;
}

EXTERN_C
ULONG
WMIAPI
StopTraceA(
  IN TRACEHANDLE TraceHandle,
  IN LPCSTR InstanceName OPTIONAL,
  IN OUT PEVENT_TRACE_PROPERTIES Properties)
{
	return 0;
}

EXTERN_C
ULONG
WMIAPI
StopTraceW(
  IN TRACEHANDLE TraceHandle,
  IN LPCWSTR InstanceName OPTIONAL,
  IN OUT PEVENT_TRACE_PROPERTIES Properties)
{
	return 0;
}

ULONG WINAPI TraceEventInstance(
  IN  TRACEHANDLE SessionHandle,
  IN  PEVENT_INSTANCE_HEADER EventTrace,
  IN  PEVENT_INSTANCE_INFO pInstInfo,
  IN  PEVENT_INSTANCE_INFO pParentInstInfo
)
{
	return 0;
}

EXTERN_C
ULONG
WMIAPI
TraceMessageVa(
  IN TRACEHANDLE LoggerHandle,
  IN ULONG MessageFlags,
  IN LPCGUID MessageGuid,
  IN USHORT MessageNumber,
  IN va_list MessageArgList)
{
	return 0;
}

DWORD WINAPI TrusteeAccessToObjectA(LPCSTR lpObject,
									SE_OBJECT_TYPE ObjectType, 
									LPCSTR lpProvider, 
									PTRUSTEE_A pTrustee, 
									ULONG cEntries, 
									PTRUSTEE_ACCESSA pTrusteeAccess)
{
	return 0;
}

DWORD WINAPI TrusteeAccessToObjectW(LPCWSTR lpObject,
									SE_OBJECT_TYPE ObjectType, 
									LPCWSTR lpProvider, 
									PTRUSTEE_A pTrustee, 
									ULONG cEntries, 
									PTRUSTEE_ACCESSA pTrusteeAccess)
{
	return 0;
}

DWORD WINAPI UninstallApplication(WCHAR *ProductCode, DWORD dwStatus)
{
	return 0;
}

DWORD WINAPI UnregisterIdleTask(BOOLEAN a1, HANDLE hObject, HANDLE a3)
{
	return 0;
}

SERVICE_STATUS_HANDLE WINAPI WdmWmiServiceMain(PVOID a1, ULONG a2)
{
	return NULL;
}

DWORD WINAPI CloseCodeAuthzLevel(HANDLE handle)
{
	return 0;
}

DWORD WINAPI ComputeAccessTokenFromCodeAuthzLevel(HANDLE handle, DWORD audit)
{
	return 0;
}

BOOL WINAPI CredpDecodeCredential(PVOID handle)
{
	return TRUE;
}

BOOL WINAPI CredpEncodeCredential(PVOID handle)
{
	return TRUE;
}

NTSTATUS WINAPI CredpConvertTargetInfo(int a1, int a2, PULONG a3, int a4)
{
	return 0x00000000;
}

BOOL WINAPI GetInformationCodeAuthzPolicyW()
{
	return TRUE;
}

BOOL WINAPI GetInformationCodeAuthzLevelW()
{
	return TRUE;
}

BOOL WINAPI CreateCodeAuthzLevel()
{
	return TRUE;
}

BOOL WINAPI ConvertStringSDToSDRootDomainA(int a1, int a2, int a3, int a4, int a5)
{
	return TRUE;
}

BOOL WINAPI ConvertStringSDToSDRootDomainW(int a1, int a2, int a3, int a4, int a5)
{
	return TRUE;
}

DWORD WINAPI I_ScGetCurrentGroupStateW(struct SC_HANDLE__ *a1, unsigned __int16 *a2, unsigned __int32 *a3)
{
  return 50;
}

void WINAPI I_ScIsSecurityProcess()
{
	;
}

void WINAPI IdentifyCodeAuthzLevelW()
{
	;
}

NTSTATUS WINAPI LsaICLookupNamesWithCreds(const wchar_t *NetworkAddr, 
											 RPC_WSTR ServerPrincName, 
											 unsigned __int32 AuthnLevel, 
											 unsigned __int32 AuthnSvc, 
											 RPC_AUTH_IDENTITY_HANDLE AuthIdentity,
											 unsigned __int32 AuthzSvc,
											 int a7, 
											 int a8, 
											 int a9, 
											 int a10, 
											 int a11, 
											 int a12)
{
	return 0x00000000;
}

NTSTATUS WINAPI LsaICLookupNames(int a1, 
								ULONG a2, 
								int a3, 
								PVOID *a4, 
								PVOID *a5, 
								int a6, 
								BOOLEAN a7, 
								int a8, 
								int a9,
								int a10)
{
	return 0x00000000;
}

NTSTATUS WINAPI LsaICLookupSids(int a1, 
								ULONG a2, 
								int a3, 
								PVOID *a4, 
								PVOID *a5, 
								int a6, 
								BOOLEAN a7, 
								int a8, 
								int a9)
{
	return 0x00000000;
}

NTSTATUS WINAPI LsaICLookupSidsWithCreds(const wchar_t *NetworkAddr, 
											 RPC_WSTR ServerPrincName, 
											 unsigned __int32 AuthnLevel, 
											 unsigned __int32 AuthnSvc, 
											 RPC_AUTH_IDENTITY_HANDLE AuthIdentity,
											 unsigned __int32 AuthzSvc,
											 int a7, 
											 int a8, 
											 int a9, 
											 int a10, 
											 int a11, 
											 int a12)
{
	return 0x00000000;
}


BOOL WINAPI SaferiCompareTokenLevels(int a1, int a2, int a3)
{
	return TRUE;
}

void WINAPI SaferiRecordEventLogEntry()
{
	;
}

void WINAPI SetInformationCodeAuthzLevelW()
{
	;
}

void WINAPI SetInformationCodeAuthzPolicyW()
{
	;
}

EXTERN_C
ULONG
WMIAPI
UpdateTraceW(
  IN TRACEHANDLE TraceHandle,
  IN LPCWSTR InstanceName OPTIONAL,
  IN OUT PEVENT_TRACE_PROPERTIES Properties)
{
	return 0;
}

EXTERN_C
ULONG
WMIAPI
UpdateTraceA(
  IN TRACEHANDLE TraceHandle,
  IN LPCSTR InstanceName OPTIONAL,
  IN OUT PEVENT_TRACE_PROPERTIES Properties)
{
	return 0;
}
  
DWORD WINAPI WmiCloseBlock(HANDLE hObject)  
{
	return 0;
}

NTSTATUS WINAPI WmiCloseTraceWithCursor(LPCVOID a1)
{
	return 0x00000000;
}

int WINAPI WmiConvertTimestamp(int a1, int a2, int a3)
{
	return 0;
}

ULONG WINAPI WmiDevInstToInstanceNameA(BOOLEAN *a1, size_t cchDest, STRSAFE_LPCSTR pszSrc, int a4)
{
	return 0;
}

ULONG WINAPI WmiDevInstToInstanceNameW(BOOLEAN *a1, size_t cchDest, STRSAFE_LPWSTR pszSrc, int a4)
{
	return 0;
}

DWORD WINAPI WmiEnumerateGuids(int a1, int a2)
{
	return 0;
}

NTSTATUS WINAPI WmiExecuteMethodA(int a1, LPCSTR lpMultiByteStr, int a3, int a4, int a5, int a6, int a7)
{
	return 0x00000000;
}

NTSTATUS WINAPI WmiExecuteMethodW(int a1, LPCWSTR lpMultiByteStr, int a3, int a4, int a5, int a6, int a7)
{
	return 0x00000000;
}

DWORD WINAPI WmiFileHandleToInstanceNameA(int a1, int a2, PULONG a3, int a4)
{
	return 0;
}

DWORD WINAPI WmiFileHandleToInstanceNameW(int a1, int a2, PULONG a3, int a4)
{
	return 0;
}

BOOLEAN WINAPI WmiFreeBuffer(PVOID Address)
{
	return TRUE;
}

DWORD WINAPI WmiGetFirstTraceOffset(TRACEHANDLE a1)
{
	return TRUE;
}

int WINAPI WmiGetNextEvent(int a1)
{
	return 0;
}

int WINAPI WmiGetTraceHeader(int a1, int a2, int a3)
{
	return 0;
}

DWORD WINAPI WriteEncryptedFileRaw(PFE_IMPORT_FUNC pfImportCallback,
								   PVOID pvCallbackContext, 
								   PVOID pvContext)
{
	return 0;
}

DWORD WINAPI Wow64Win32ApiEntry(int a1, BOOLEAN a2, int a3)
{
	return 0;
}

NTSTATUS WINAPI WmiSetSingleItemA(int a1, LPCSTR Str, int a3, int a4, DWORD nOutBufferSize, PVOID a6)
{
	return 0x00000000;
}

NTSTATUS WINAPI WmiSetSingleItemW(int a1, LPCWSTR Str, int a3, int a4, DWORD nOutBufferSize, PVOID a6)
{
	return 0x00000000;
}

NTSTATUS WINAPI WmiSetSingleInstanceA(int a1, LPCSTR Str, int a3, DWORD nOutBufferSize, PVOID a6)
{
	return 0x00000000;
}

NTSTATUS WINAPI WmiSetSingleInstanceW(int a1, LPCWSTR Str, int a3, DWORD nOutBufferSize, PVOID a6)
{
	return 0x00000000;
}

void WINAPI WmiReceiveNotificationsA()
{
	;
}

void WINAPI WmiReceiveNotificationsW()
{
	;
}

DWORD WINAPI WmiQuerySingleInstanceA(int a1, LPCSTR Str, PULONG a3, PVOID a4)
{
	return 0;
}

DWORD WINAPI WmiQuerySingleInstanceW(int a1, LPCWSTR Str, PULONG a3, PVOID a4)
{
	return 0;
}

DWORD WINAPI WmiOpenBlock(int a1, int a2, int a3)
{
	return 0;
}

DWORD WINAPI WmiMofEnumerateResourcesA(int a1, int a2, int a3)
{
	return 0;
}

DWORD WINAPI WmiMofEnumerateResourcesW(int a1, int a2, int a3)
{
	return 0;
}

void WINAPI WmiNotificationRegistrationA()
{
	;
}

void WINAPI WmiNotificationRegistrationW()
{
	;
}

DWORD WINAPI WmiQueryAllDataA(int a1, PULONG a2, PVOID a3)
{
	return 0;
}

DWORD WINAPI WmiQueryAllDataW(int a1, PULONG a2, PVOID a3)
{
	return 0;
}

DWORD WINAPI WmiOpenTraceWithCursor(TRACEHANDLE handle)
{
	return 0;
}

int WINAPI WmiParseTraceEvent(int a1, int a2, int a3, int a4, int a5)
{
	return 0;
}

DWORD WINAPI WmiQueryGuidInformation(LPVOID a1, LPGUID a2)
{
	return 0;
}

DWORD WINAPI WmiQueryAllDataMultipleA(int a1, ULONG a2, DWORD *a3, PVOID a4)
{
	return 0;
}


DWORD WINAPI WmiQueryAllDataMultipleW(int a1, ULONG a2, DWORD *a3, PVOID a4)
{
	return 0;
}

DWORD WINAPI WmiQuerySingleInstanceMultipleA(int a1, DWORD dwErrCode,ULONG a3, int a4, int a5)
{
	return 0;
}

DWORD WINAPI WmiQuerySingleInstanceMultipleW(int a1, DWORD dwErrCode,ULONG a3, int a4, int a5)
{
	return 0;
}

NTSTATUS
NTAPI
ElfBackupEventLogFileA (
    IN  HANDLE LogHandle,
    IN  PSTRING BackupFileName
)
{
	return 0x00000000;
}

NTSTATUS
NTAPI
ElfBackupEventLogFileW (
    IN  HANDLE LogHandle,
    IN  PUNICODE_STRING BackupFileName
)
{
	return 0x00000000;
}


NTSTATUS
NTAPI
ElfChangeNotify (
    IN  HANDLE LogHandle,
    IN  HANDLE Event
)
{
	return 0x00000000;
}

NTSTATUS
NTAPI
ElfClearEventLogFileA (
    IN  HANDLE LogHandle,
    IN OPTIONAL  PSTRING BackupFileName
)
{
	return 0x00000000;
}

NTSTATUS
NTAPI
ElfClearEventLogFileW (
    IN  HANDLE LogHandle,
    IN OPTIONAL  PUNICODE_STRING BackupFileName
)
{
	return 0x00000000;
}

NTSTATUS
NTAPI
ElfCloseEventLog (
    IN  HANDLE LogHandle
)
{
	return 0x00000000;
}


NTSTATUS
NTAPI
ElfDeregisterEventSource (
    IN  HANDLE LogHandle
)
{
	return 0x00000000;
}

NTSTATUS
NTAPI
ElfFlushEventLog (
    IN  HANDLE LogHandle
)
{
	return 0x00000000;
}

NTSTATUS
NTAPI
ElfNumberOfRecords (
    IN  HANDLE LogHandle,
    OUT PULONG NumberOfRecords
)
{
	return 0x00000000;
}


NTSTATUS
NTAPI
ElfOldestRecord (
    IN  HANDLE LogHandle,
    OUT PULONG OldestRecord
)
{
	return 0x00000000;
}

NTSTATUS
NTAPI
ElfOpenBackupEventLogA (
    IN OPTIONAL  PSTRING UNCServerName,
    IN  PSTRING FileName,
    OUT PHANDLE LogHandle
)
{
	return 0x00000000;	
}

NTSTATUS
NTAPI
ElfOpenBackupEventLogW(
    IN OPTIONAL  PUNICODE_STRING UNCServerName,
    IN  PUNICODE_STRING FileName,
    OUT PHANDLE LogHandle
)
{
	return 0x00000000;	
}

NTSTATUS
NTAPI
ElfOpenEventLogA (
    IN OPTIONAL	PSTRING UNCServerName,
    IN		PSTRING SourceName,
    IN		PHANDLE LogHandle
)
{
	return 0x00000000;	
}

	
NTSTATUS
NTAPI
ElfOpenEventLogW (
    IN OPTIONAL	PUNICODE_STRING UNCServerName,
    IN		PUNICODE_STRING SourceName,
    OUT		PHANDLE         LogHandle
)
{
	return 0x00000000;	
}


NTSTATUS
NTAPI
ElfReadEventLogW (
    IN  HANDLE LogHandle,
    IN  ULONG  ReadFlags,
    IN  ULONG  RecordNumber,
    __out_bcount(NumberOfBytesToRead) PVOID  Buffer,
    IN  ULONG  NumberOfBytesToRead,
    OUT PULONG NumberOfBytesRead,
    OUT PULONG MinNumberOfBytesNeeded
)
{
	return 0x00000000;	
}

NTSTATUS
NTAPI
ElfReadEventLogA (
    IN  HANDLE LogHandle,
    IN  ULONG  ReadFlags,
    IN  ULONG  RecordNumber,
    __out_bcount(NumberOfBytesToRead) PVOID  Buffer,
    IN  ULONG  NumberOfBytesToRead,
    OUT PULONG NumberOfBytesRead,
    OUT PULONG MinNumberOfBytesNeeded
)
{
	return 0x00000000;	
}


NTSTATUS
NTAPI
ElfRegisterEventSourceA (
    IN OPTIONAL	 PSTRING UNCServerName,
    IN  PSTRING SourceName,
    OUT PHANDLE LogHandle
)
{
	return 0x00000000;	
}

NTSTATUS
NTAPI
ElfRegisterEventSourceW (
    IN OPTIONAL	 PUNICODE_STRING UNCServerName,
    IN  PUNICODE_STRING SourceName,
    OUT PHANDLE LogHandle
)
{
	return 0x00000000;	
}

NTSTATUS
NTAPI
ElfReportEventW (
    IN     HANDLE      LogHandle,
    IN     USHORT      EventType,
    IN OPTIONAL     USHORT      EventCategory,
    IN     ULONG       EventID,
    IN OPTIONAL     PSID        UserSid,
    IN     USHORT      NumStrings,
    IN     ULONG       DataSize,
    __in_ecount_opt(NumStrings)     PUNICODE_STRING *Strings,
    __in_bcount_opt(DataSize)     PVOID       Data,
    IN     USHORT      Flags,
    IN OUT OPTIONAL PULONG      RecordNumber,
    IN OUT OPTIONAL PULONG      TimeWritten
)
{
	return 0x00000000;	
}

NTSTATUS
NTAPI
ElfReportEventA (
    IN     HANDLE      LogHandle,
    IN     USHORT      EventType,
    IN OPTIONAL     USHORT      EventCategory,
    IN     ULONG       EventID,
    IN OPTIONAL     PSID        UserSid,
    IN     USHORT      NumStrings,
    IN     ULONG       DataSize,
    __in_ecount_opt(NumStrings)     PSTRING *Strings,
    __in_bcount_opt(DataSize)     PVOID       Data,
    IN     USHORT      Flags,
    IN OUT OPTIONAL PULONG      RecordNumber,
    IN OUT OPTIONAL PULONG      TimeWritten
)
{
	return 0x00000000;	
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

BOOL
WINAPI
CredWriteDomainCredentialsW(PCREDENTIAL_TARGET_INFORMATIONW TargetInfo,
                            PCREDENTIALW Credential,
                            DWORD Flags)
{
    return FALSE;
}

BOOL
WINAPI
CredWriteDomainCredentialsA(PCREDENTIAL_TARGET_INFORMATIONA TargetInfo,
                            PCREDENTIALA Credential,
                            DWORD Flags)
{
    return FALSE;
}
