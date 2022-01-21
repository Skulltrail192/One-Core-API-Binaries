/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    hooks.c

Abstract:

    Implement Hooks of Nt functions

Author:

    Skulltrail 18-March-2018

Revision History:

--*/

#include "main.h"

static UNICODE_STRING NtDllName = RTL_CONSTANT_STRING(L"ntdll.dll");

static ANSI_STRING NtGetpCurrentProcessorNumberProcName = RTL_CONSTANT_STRING("NtGetCurrentProcessorNumber");

ULONG NTAPI RtlpGetCurrentProcessorNumber();

ULONG (NTAPI *NtGetpCurrentProcessorNumber)(IN VOID);

static ULONG execute_flags = MEM_EXECUTE_OPTION_DISABLE;

ULONG_PTR get_system_affinity_mask(void)
{
    ULONG num_cpus = NtCurrentTeb()->ProcessEnvironmentBlock->NumberOfProcessors;
    if (num_cpus >= sizeof(ULONG_PTR) * 8) return ~(ULONG_PTR)0;
    return ((ULONG_PTR)1 << num_cpus) - 1;
}

NTSTATUS
NTAPI
NtQuerySectionInternal(
  IN HANDLE               SectionHandle,
  IN SECTION_INFORMATION_CLASS InformationClass,
  OUT PVOID               InformationBuffer,
  IN SIZE_T               InformationBufferSize,
  OUT PSIZE_T             ResultLength OPTIONAL 
)
{
	NTSTATUS Status;
	PSECTION_IMAGE_INFORMATION Sii;
	
	//Implement hook for bypass major version verification on CreateProcessInternal
	Status = NtQuerySection(SectionHandle, InformationClass, InformationBuffer, InformationBufferSize, ResultLength);
	if(NT_SUCCESS(Status) && InformationClass == SectionImageInformation)
	{
		Sii = (PSECTION_IMAGE_INFORMATION)InformationBuffer;
		Sii->SubSystemMajorVersion = 5;
		Sii->SubSystemMinorVersion = 0;
		InformationBuffer = Sii;
	}	
	return Status;
}

/******************************************************************************
*  NtQueryInformationToken		[NTDLL.@]
*  ZwQueryInformationToken		[NTDLL.@]
*
* NOTES
*  Buffer for TokenUser:
*   0x00 TOKEN_USER the PSID field points to the SID
*   0x08 SID
*
*/
NTSTATUS 
WINAPI 
NtQueryInformationTokenInternal(
	HANDLE token,
	TOKEN_INFORMATION_CLASS tokeninfoclass,
	PVOID tokeninfo,
	ULONG tokeninfolength,
	PULONG retlen )
{
    static const ULONG info_len [] =
    {
        0,
        0,    /* TokenUser */
        0,    /* TokenGroups */
        0,    /* TokenPrivileges */
        0,    /* TokenOwner */
        0,    /* TokenPrimaryGroup */
        0,    /* TokenDefaultDacl */
        sizeof(TOKEN_SOURCE), /* TokenSource */
        sizeof(TOKEN_TYPE),  /* TokenType */
        sizeof(SECURITY_IMPERSONATION_LEVEL), /* TokenImpersonationLevel */
        sizeof(TOKEN_STATISTICS), /* TokenStatistics */
        0,    /* TokenRestrictedSids */
        sizeof(DWORD), /* TokenSessionId */
        0,    /* TokenGroupsAndPrivileges */
        0,    /* TokenSessionReference */
        0,    /* TokenSandBoxInert */
        0,    /* TokenAuditPolicy */
        0,    /* TokenOrigin */
        sizeof(TOKEN_ELEVATION_TYPE), /* TokenElevationType */
        0,    /* TokenLinkedToken */
        sizeof(TOKEN_ELEVATION), /* TokenElevation */
        0,    /* TokenHasRestrictions */
        0,    /* TokenAccessInformation */
        0,    /* TokenVirtualizationAllowed */
        0,    /* TokenVirtualizationEnabled */
        sizeof(TOKEN_MANDATORY_LABEL) + sizeof(SID), /* TokenIntegrityLevel [sizeof(SID) includes one SubAuthority] */
        0,    /* TokenUIAccess */
        0,    /* TokenMandatoryPolicy */
        0,    /* TokenLogonSid */
        sizeof(DWORD), /* TokenIsAppContainer */
        0,    /* TokenCapabilities */
        sizeof(TOKEN_APPCONTAINER_INFORMATION) + sizeof(SID), /* TokenAppContainerSid */
        0,    /* TokenAppContainerNumber */
        0,    /* TokenUserClaimAttributes*/
        0,    /* TokenDeviceClaimAttributes */
        0,    /* TokenRestrictedUserClaimAttributes */
        0,    /* TokenRestrictedDeviceClaimAttributes */
        0,    /* TokenDeviceGroups */
        0,    /* TokenRestrictedDeviceGroups */
        0,    /* TokenSecurityAttributes */
        0,    /* TokenIsRestricted */
        0     /* TokenProcessTrustLevel */
    };

    ULONG len = 0;
    NTSTATUS status = STATUS_SUCCESS;

    if (tokeninfoclass < MaxTokenInfoClass)
        len = info_len[tokeninfoclass];

    if (retlen) *retlen = len;

    if (tokeninfolength < len)
        return STATUS_BUFFER_TOO_SMALL;
	
	
	status =  NtQueryInformationToken(token,
									  tokeninfoclass,
									  tokeninfo,
									  tokeninfolength,
									  retlen);			
	
	if(status == STATUS_INVALID_INFO_CLASS)
	{
		switch(tokeninfoclass){
			case TokenElevationType:
				{	
					TOKEN_ELEVATION_TYPE *elevation_type = tokeninfo;
					DbgPrint("NtQueryInformationTokenInternal( ..., TokenElevationType, ...) semi-stub\n");
					*elevation_type = TokenElevationTypeFull;
				}			
				break;
			case TokenElevation:
				{
					TOKEN_ELEVATION *elevation = tokeninfo;
					DbgPrint("QueryInformationToken( ..., TokenElevation, ...) semi-stub\n");
					elevation->TokenIsElevated = TRUE;
				}
				break;		
			case TokenIntegrityLevel:
				{
					/* report always "S-1-16-12288" (high mandatory level) for now */
					static const SID high_level = {SID_REVISION, 1, {SECURITY_MANDATORY_LABEL_AUTHORITY},
																	{SECURITY_MANDATORY_HIGH_RID}};

					TOKEN_MANDATORY_LABEL *tml = tokeninfo;
					PSID psid = tml + 1;

					tml->Label.Sid = psid;
					tml->Label.Attributes = SE_GROUP_INTEGRITY | SE_GROUP_INTEGRITY_ENABLED;
					memcpy(psid, &high_level, sizeof(SID));
				}
				break;	
			case TokenAppContainerSid:
				{
					TOKEN_APPCONTAINER_INFORMATION *container = tokeninfo;
					DbgPrint("QueryInformationToken( ..., TokenAppContainerSid, ...) semi-stub\n");
					container->TokenAppContainer = NULL;
				}
				break;
			 case TokenIsAppContainer:
				{
					*(DWORD*)tokeninfo = 0;
					DbgPrint("NtQueryInformationTokenInternal :: TokenIsAppContainer semi-stub\n");				
					break;
				}				
			case TokenLogonSid:
				{
					TOKEN_GROUPS * groups = tokeninfo;
					TOKEN_GROUPS * receiveToken = NULL;
					//PSID sid = groups + 1;
					DWORD sid_len = tokeninfolength < sizeof(TOKEN_GROUPS) ? 0 : tokeninfolength - sizeof(TOKEN_GROUPS);
					PVOID tokenGroupInfo = NULL;
					
					status = NtQueryInformationToken(token,
													 TokenGroups,
													 tokenGroupInfo,
													 sid_len,
													 retlen);
													 
													 
					DbgPrint("NtQueryInformationTokenInternal :: NtQueryInformationToken Status: %08x\n",status);										 

					if (status == STATUS_SUCCESS)
					{
						receiveToken = (TOKEN_GROUPS *)tokenGroupInfo;
						groups->GroupCount = 1;
						groups->Groups[0].Sid = receiveToken->Groups[0].Sid;
						groups->Groups[0].Attributes = 0;
					}
				}	
				break;			
			default:
				{
					status =  NtQueryInformationToken(token,
												   tokeninfoclass,
												   tokeninfo,
												   tokeninfolength,
												   retlen);			
				}
				break;
		}

		return STATUS_SUCCESS;
	}
	
	return status;
}

/******************************************************************************
*  NtSetInformationToken		[NTDLL.@]
*  ZwSetInformationToken		[NTDLL.@]
*/
NTSTATUS 
WINAPI 
NtSetInformationTokenInternal(
    HANDLE TokenHandle,
    TOKEN_INFORMATION_CLASS TokenInformationClass,
    PVOID TokenInformation,
    ULONG TokenInformationLength
)
{
	switch(TokenInformationClass){	
		case TokenIntegrityLevel:
		{
			DbgPrint("TokenIntegrityLevel stub!\n");
		}
		break;	
		default:
		{
			return NtSetInformationToken(TokenHandle,
										 TokenInformationClass,
										 TokenInformation,
										 TokenInformationLength);				
		}
	
	}
	return STATUS_SUCCESS;	
}

/******************************************************************************
 *              NtQueryInformationThread  (NTDLL.@)
 *              ZwQueryInformationThread  (NTDLL.@)
 */
NTSTATUS 
WINAPI 
NtQueryInformationThreadInternal( 
	HANDLE handle, 
	THREADINFOCLASS class,
    void *data, 
	ULONG length, 
	ULONG *ret_len 
)
{
	NTSTATUS Status = STATUS_SUCCESS;
	
	switch(class)
	{
    case ThreadGroupInformation:
        {
            const ULONG_PTR affinity_mask = get_system_affinity_mask();
            GROUP_AFFINITY affinity;

            memset(&affinity, 0, sizeof(affinity));
            affinity.Group = 0; /* Wine only supports max 64 processors */

			affinity.Mask = affinity_mask;
            if (data) memcpy( data, &affinity, min( length, sizeof(affinity) ));
            if (ret_len) *ret_len = min( length, sizeof(affinity) );
		}
        break;	
	default:
		return NtQueryInformationThread(handle,
										class,
										data,
										length,
										ret_len);
	}
	return Status;
}

/******************************************************************************
 * NtSetInformationProcess [NTDLL.@]
 * ZwSetInformationProcess [NTDLL.@]
 */
NTSTATUS 
WINAPI 
NtSetInformationProcessInternal(
	IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	IN PVOID ProcessInformation,
	IN ULONG ProcessInformationLength)
{
    NTSTATUS Status = STATUS_SUCCESS;
	
    switch (ProcessInformationClass)
    {
    case ProcessExecuteFlags:
        if (ProcessInformationLength != sizeof(ULONG))
            return STATUS_INVALID_PARAMETER;
        else if (execute_flags & MEM_EXECUTE_OPTION_PERMANENT)
            return STATUS_ACCESS_DENIED;
        else
        {
            BOOL enable;
            switch (*(ULONG *)ProcessInformation & (MEM_EXECUTE_OPTION_ENABLE|MEM_EXECUTE_OPTION_DISABLE))
            {
            case MEM_EXECUTE_OPTION_ENABLE:
                enable = TRUE;
                break;
            case MEM_EXECUTE_OPTION_DISABLE:
                enable = FALSE;
                break;
            default:
                return STATUS_INVALID_PARAMETER;
            }
            execute_flags = *(ULONG *)ProcessInformation;
        }
        break;
	default:
		return NtSetInformationProcess(ProcessHandle,
									  ProcessInformationClass,
									  ProcessInformation,
									  ProcessInformationLength);
	}
	
	return Status;
}

ULONG 
NTAPI 
NtGetCurrentProcessorNumber() 	
{
	NTSTATUS st;
	PVOID ProcessorNumberHandle;
		
	st = LdrLoadDll(NULL, NULL, &NtDllName, &ProcessorNumberHandle);
		
	if (!NT_SUCCESS(st)) {
		DbgPrint("NtGetCurrentProcessorNumber: Ntdll.dll not found.  Status=%x\n", st);
		return st;
	}	
		
	st = LdrGetProcedureAddress (ProcessorNumberHandle,
								&NtGetpCurrentProcessorNumberProcName,
								0,
								(PVOID*)&NtGetpCurrentProcessorNumber);
									
	if (!NT_SUCCESS(st)) {
		return RtlpGetCurrentProcessorNumber();
	}	

	return (*NtGetpCurrentProcessorNumber)();
}

//
// This section is made for better debugging experience

NTSYSCALLAPI 
NTSTATUS 
NTAPI 	
NtAcceptConnectPortInternal(
	_Out_ PHANDLE PortHandle, 
	_In_opt_ PVOID PortContext, 
	_In_ PPORT_MESSAGE ConnectionRequest,
	_In_ BOOLEAN AcceptConnection, 
	_Inout_opt_ PPORT_VIEW ServerView, 
	_Out_opt_ PREMOTE_PORT_VIEW ClientView
)
{
	NTSTATUS Status;
	Status = NtAcceptConnectPort(PortHandle,
								 PortContext,
								 ConnectionRequest,
								 AcceptConnection,
								 ServerView,
								 ClientView);
								 
	if(!NT_SUCCESS(Status))
	{
		DbgPrint("NtAcceptConnectPort :: Status: %08x\n",Status);		
	}						
						
	return Status;
}

NTSYSCALLAPI 
NTSTATUS 
NTAPI 	
NtAccessCheckInternal(
	_In_ PSECURITY_DESCRIPTOR SecurityDescriptor, 
	_In_ HANDLE ClientToken, 
	_In_ ACCESS_MASK DesiredAccess, 
	_In_ PGENERIC_MAPPING GenericMapping, 
	_Out_writes_bytes_(*PrivilegeSetLength) PPRIVILEGE_SET PrivilegeSet, 
	_Inout_ PULONG PrivilegeSetLength, 
	_Out_ PACCESS_MASK GrantedAccess, 
	_Out_ PNTSTATUS AccessStatus
)
{
	NTSTATUS Status;
	Status = NtAccessCheck(SecurityDescriptor,
							ClientToken,
							DesiredAccess,
							GenericMapping,
							PrivilegeSet,
							PrivilegeSetLength,
							GrantedAccess,
							AccessStatus);
							
	// if(!NT_SUCCESS(Status))
	// {
		// DbgPrint("NtAccessCheck :: Status: %08x\n",Status);		
	// }
	
	return Status;
}

NTSTATUS 
NTAPI 
NtAccessCheckAndAuditAlarmInternal( 	
	_In_ PUNICODE_STRING  	SubsystemName,
	_In_opt_ PVOID  	HandleId,
	_In_ PUNICODE_STRING  	ObjectTypeName,
	_In_ PUNICODE_STRING  	ObjectName,
	_In_ PSECURITY_DESCRIPTOR  	SecurityDescriptor,
	_In_ ACCESS_MASK  	DesiredAccess,
	_In_ PGENERIC_MAPPING  	GenericMapping,
	_In_ BOOLEAN  	ObjectCreation,
	_Out_ PACCESS_MASK  	GrantedAccess,
	_Out_ PNTSTATUS  	AccessStatus,
	_Out_ PBOOLEAN  	GenerateOnClose 
) 	
{
	NTSTATUS Status;
	Status = NtAccessCheckAndAuditAlarm(SubsystemName,
										HandleId,
										ObjectTypeName,
										ObjectName,
										SecurityDescriptor,
										DesiredAccess,
										GenericMapping,
										ObjectCreation,
										GrantedAccess,
										AccessStatus,
										GenerateOnClose);
							
	if(!NT_SUCCESS(Status))
	{
		DbgPrint("NtAccessCheckAndAuditAlarm :: Status: %08x\n",Status);		
	}
	
	return Status;	
}

NTSTATUS
NTAPI
NtAccessCheckByTypeInternal(
    _In_ PSECURITY_DESCRIPTOR SecurityDescriptor,
    _In_ PSID PrincipalSelfSid,
    _In_ HANDLE ClientToken,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_TYPE_LIST ObjectTypeList,
    _In_ ULONG ObjectTypeLength,
    _In_ PGENERIC_MAPPING GenericMapping,
    _In_ PPRIVILEGE_SET PrivilegeSet,
    _Inout_ PULONG PrivilegeSetLength,
    _Out_ PACCESS_MASK GrantedAccess,
    _Out_ PNTSTATUS AccessStatus
)
{
	NTSTATUS Status;
	Status = NtAccessCheckByType(SecurityDescriptor,
								PrincipalSelfSid,
								ClientToken,
								DesiredAccess,
								ObjectTypeList,
								ObjectTypeLength,
								GenericMapping,
								PrivilegeSet,
								PrivilegeSetLength,
								GrantedAccess,
								AccessStatus);
							
	if(!NT_SUCCESS(Status))
	{
		DbgPrint("NtAccessCheckByType :: Status: %08x\n",Status);		
	}
	
	return Status;		
}	
	
NTSTATUS
NTAPI
NtAccessCheckByTypeAndAuditAlarmInternal(
    _In_ PUNICODE_STRING SubsystemName,
    _In_opt_ PVOID HandleId,
    _In_ PUNICODE_STRING ObjectTypeName,
    _In_ PUNICODE_STRING ObjectName,
    _In_ PSECURITY_DESCRIPTOR SecurityDescriptor,
    _In_opt_ PSID PrincipalSelfSid,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ AUDIT_EVENT_TYPE AuditType,
    _In_ ULONG Flags,
    _In_reads_opt_(ObjectTypeLength) POBJECT_TYPE_LIST ObjectTypeList,
    _In_ ULONG ObjectTypeLength,
    _In_ PGENERIC_MAPPING GenericMapping,
    _In_ BOOLEAN ObjectCreation,
    _Out_ PACCESS_MASK GrantedAccess,
    _Out_ PNTSTATUS AccessStatus,
    _Out_ PBOOLEAN GenerateOnClose)
{
	NTSTATUS Status;
	Status = NtAccessCheckByTypeAndAuditAlarm(SubsystemName,
											  HandleId,
											  ObjectTypeName,
											  ObjectName,
											  SecurityDescriptor,
											  PrincipalSelfSid,
											  DesiredAccess,
											  AuditType,
											  Flags,
											  ObjectTypeList,
											  ObjectTypeLength,
											  GenericMapping,
											  ObjectCreation,
											  GrantedAccess,
											  AccessStatus,
											  GenerateOnClose);
							
	if(!NT_SUCCESS(Status))
	{
		DbgPrint("NtAccessCheckByTypeAndAuditAlarm :: Status: %08x\n",Status);		
	}
	
	return Status;			
}	