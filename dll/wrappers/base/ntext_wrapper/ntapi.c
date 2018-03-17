/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    ntapi.c

Abstract:

    Implement User Mode Nt functions

Author:

    Skulltrail 06-March-2018

Revision History:

--*/

#include <main.h>
#include <lpcfuncs.h>
#include <cmfuncs.h>

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
NtLoadKeyEx(
	IN POBJECT_ATTRIBUTES TargetKey,
	IN POBJECT_ATTRIBUTES SourceFile,
	IN ULONG Flags,
	IN HANDLE TrustClassKey
) 
{
	return NtLoadKey(TargetKey, SourceFile);
}

NTSTATUS 
NTAPI
NtQueryOpenSubKeysEx(
	IN POBJECT_ATTRIBUTES TargetKey,
	IN ULONG BufferLength,
	IN PVOID Buffer,
	IN PULONG RequiredSize
)
{
	return NtQueryOpenSubKeys(TargetKey, RequiredSize);
}

NTSTATUS 
WINAPI 
NtUnmapViewOfSectionEx(
	HANDLE handle, 
	PVOID MemoryCache, 
	ULONG number)
{
	return NtUnmapViewOfSection(handle, MemoryCache);	
}

NTSTATUS
NTAPI
NtCreateMutantEx(
  OUT PHANDLE             MutantHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
  IN BOOLEAN              InitialOwner, 
  IN ULONG 				  Flags
)
{
	return NtCreateMutant(MutantHandle, DesiredAccess, ObjectAttributes, InitialOwner);
}

NTSTATUS 
NTAPI 
NtCreateIoCompletionEx(
	__out PHANDLE IoCompletionHandle, 
	__in ACCESS_MASK DesiredAccess, 
	__in_opt POBJECT_ATTRIBUTES ObjectAttributes, 
	__in_opt ULONG Count
)
{
	return NtCreateIoCompletion(IoCompletionHandle, DesiredAccess, ObjectAttributes, Count);
} 

NTSTATUS 
NTAPI
NtRemoveIoCompletionEx(
    __in HANDLE IoCompletionHandle,
    __out FILE_IO_COMPLETION_INFORMATION IoCompletionInformation,
    __in ULONG Count,
    __out PVOID NumEntriesRemoved,
    __in_opt PLARGE_INTEGER Timeout,
    __in BOOLEAN Alertable
)
{
	return NtRemoveIoCompletion(IoCompletionHandle,
								NULL,
								NULL,
								NULL,
								Timeout);
}

NTSTATUS 
NTAPI 
NtCreateSemaphoreEx(
  IN OPTIONAL   HANDLE lpSemaphoreAttributes,
  IN       LONG                  lInitialCount,
  IN       LONG                  lMaximumCount,
  IN OPTIONAL  LPCTSTR               lpName,
  IN 	   DWORD                 dwFlags,
  IN       DWORD                 dwDesiredAccess
)
{
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
NtQuerySystemInformationEx(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation, 
	ULONG QueryInformationLength, 
	PVOID SystemInformatiom, 
	ULONG BufferLength, 
	PULONG ReturnLength
)
{
	return NtQuerySystemInformation(SystemInformationClass,
									SystemInformation,
									BufferLength,
									ReturnLength);
}

NTSTATUS 
NTAPI 
NtCreateKeyTransacted(
	__out PHANDLE KeyHandle, 
	__in ACCESS_MASK DesiredAccess, 
	__in POBJECT_ATTRIBUTES ObjectAttributes, 
	__reserved ULONG TitleIndex, 
	__in_opt PUNICODE_STRING Class, 
	__in ULONG CreateOptions, 
	__in HANDLE TransactionHandle, 
	__out_opt PULONG Disposition)
{
	return NtCreateKey(KeyHandle, 
					   DesiredAccess, 
					   ObjectAttributes, 
					   TitleIndex, 
					   Class, 
					   CreateOptions, 
					   Disposition);
}

NTSTATUS 
NTAPI 
NtOpenKeyTransacted(
	__out PHANDLE KeyHandle, 
	__in ACCESS_MASK DesiredAccess, 
	__in POBJECT_ATTRIBUTES ObjectAttributes, 
	__in HANDLE TransactionHandle)
{
	return NtOpenKey(KeyHandle, 
					 DesiredAccess, 
					 ObjectAttributes);
}

/* Section Unimplemented*/
NTSTATUS
NTAPI
NtOpenPrivateNamespace(
  __out PHANDLE NamespaceHandle,
  __in ACCESS_MASK DesiredAccess,
  __in_opt POBJECT_ATTRIBUTES ObjectAttributes,
  __in PVOID BoundaryDescriptor
)
{
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
NtSetInformationVirtualMemory(HANDLE hProcess,
							  BOOL access,
							  ULONG_PTR NumberOfEntries, 
							  PWIN32_MEMORY_RANGE_ENTRY VirtualAddresses, 
							  PULONG Flags,
							  DWORD other
)
{
	return STATUS_SUCCESS;
}	

NTSTATUS 
NTAPI 	
NtReplacePartitionUnit(__in PUNICODE_STRING TargetInstancePath, 
					   __in PUNICODE_STRING SpareInstancePath,
					   __in ULONG Flags)
{
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
NtDeletePrivateNamespace(
  __in HANDLE NamespaceHandle
)
{
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
NtCreatePrivateNamespace(
  __out PHANDLE NamespaceHandle,
  __in ACCESS_MASK DesiredAccess,
  __in_opt POBJECT_ATTRIBUTES ObjectAttributes,
  __in PVOID BoundaryDescriptor
)
{
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI
NtTraceControl(
	ULONG FunctionCode, 
	PVOID InBuffer, 
	ULONG InBufferLen, 
	PVOID OutBuffer, 
	ULONG OutBufferLen, 
	PULONG ReturnLength
)
{
	return STATUS_SUCCESS;
}

VOID 
NTAPI 
NtFlushProcessWriteBuffers()	
{
	;
}

NTAPI 
NtSetSystemPowerState( 	
	IN POWER_ACTION  	SystemAction,
	IN SYSTEM_POWER_STATE  	MinSystemState,
	IN ULONG  	Flags 
)
{
	return STATUS_SUCCESS;
} 	

NTSTATUS 
WINAPI 
NtCreateThreadEx(PHANDLE hThread,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    HANDLE ProcessHandle,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter,
    ULONG CreateFlags,
    ULONG StackZeroBits,
    ULONG SizeOfStackCommit,
    ULONG SizeOfStackReserve,
    LPVOID lpBytesBuffer)
{
    NTSTATUS status = RtlCreateUserThread(ProcessHandle, 
										  NULL, 
										  CreateFlags & 1, 
										  0, 
										  0, 
										  0, 
										  lpStartAddress, 
										  lpParameter, 
										  hThread, 
										  NULL);
    return status;
}

NTSTATUS 
NTAPI
NtCreateTransaction(
  _Out_    PHANDLE            TransactionHandle,
  _In_     ACCESS_MASK        DesiredAccess,
  _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
  _In_opt_ LPGUID             Uow,
  _In_opt_ HANDLE             TmHandle,
  _In_opt_ ULONG              CreateOptions,
  _In_opt_ ULONG              IsolationLevel,
  _In_opt_ ULONG              IsolationFlags,
  _In_opt_ PLARGE_INTEGER     Timeout,
  _In_opt_ PUNICODE_STRING    Description
)
{
	*TransactionHandle = NULL;
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS 
NTAPI
NtQueryInformationTransaction(
  _In_      HANDLE                        TransactionHandle,
  _In_      TRANSACTION_INFORMATION_CLASS TransactionInformationClass,
  _Out_     PVOID                         TransactionInformation,
  _In_      ULONG                         TransactionInformationLength,
  _Out_opt_ PULONG                        ReturnLength
)
{
	TransactionInformation = NULL;
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS 
NTAPI
NtSetInformationTransaction(
  _In_ HANDLE                        TransactionHandle,
  _In_ TRANSACTION_INFORMATION_CLASS TransactionInformationClass,
  _In_ PVOID                         TransactionInformation,
  _In_ ULONG                         TransactionInformationLength
)
{
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS 
NTAPI
NtRollbackTransaction(
  _In_ HANDLE  TransactionHandle,
  _In_ BOOLEAN Wait
)
{
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS 
NTAPI
NtCommitTransaction(
  _In_ HANDLE  TransactionHandle,
  _In_ BOOLEAN Wait
)
{
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS 
NTAPI
NtOpenTransaction(
  _Out_    PHANDLE            TransactionHandle,
  _In_     ACCESS_MASK        DesiredAccess,
  _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
  _In_     LPGUID             Uow,
  _In_opt_ HANDLE             TmHandle
)
{
	*TransactionHandle = NULL;
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS 
WINAPI 
NtOpenKeyEx( 
	PHANDLE retkey, 
	ACCESS_MASK access, 
	const OBJECT_ATTRIBUTES *attr, 
	ULONG options 
)
{
    return NtOpenKey( retkey, access, attr);
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
	NTSTATUS status;
	
	switch(tokeninfoclass){
		case TokenElevationType:
			{
				// const char *str = getenv("STAGING_ELEVATIONTYPE");
		
					// TOKEN_ELEVATION_TYPE *elevation_type = tokeninfo;
					// FIXME("QueryInformationToken( ..., TokenElevationType, ...) semi-stub. If the application complains that\n"
						  // "it should be run as normal user, set the STAGING_ELEVATIONTYPE environmentvariable to Default\n");
		
					// if( str && !strcmp(str, "Default"))
						// *elevation_type = TokenElevationTypeDefault;
					// else 
						// *elevation_type = TokenElevationTypeFull;	
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
			DbgPrint("QueryInformationToken (IntegrityLevel) token\n");
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
				DbgPrint("TokenIsAppContainer semi-stub\n");				
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

				//req->handle = wine_server_obj_handle( token );
				//req->which_sid = tokeninfoclass;
				//wine_server_set_reply( req, sid, sid_len );
				//status = wine_server_call( req );
				// if (retlen) 
					// *retlen = reply->sid_len + sizeof(TOKEN_GROUPS);
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
			return NtQueryInformationToken(token,
										   tokeninfoclass,
										   tokeninfo,
										   tokeninfolength,
										   retlen);
	}
	
	return STATUS_SUCCESS;
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
        DbgPrint("TokenIntegrityLevel stub!\n");
        return STATUS_SUCCESS;
        	
    default:
	return NtSetInformationToken(TokenHandle,
								 TokenInformationClass,
								 TokenInformation,
								 TokenInformationLength);		
	}
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
        return Status;	
	default:
		return NtQueryInformationThread(handle,
										class,
										data,
										length,
										ret_len);
	}
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
								 
	DbgPrint("NtAcceptConnectPort :: Status: %08x\n",Status);							
						
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
							
	DbgPrint("NtAccessCheck :: Status: %08x\n",Status);							
							
	return Status;
}