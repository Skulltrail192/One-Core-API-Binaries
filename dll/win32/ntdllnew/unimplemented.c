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
#include "ntdllnew.h"

typedef struct _ALPC_PORT_ATTRIBUTES
{
  ULONG Flags;
  SECURITY_QUALITY_OF_SERVICE SecurityQos;
  SIZE_T MaxMessageLength;
  SIZE_T MemoryBandwidth;
  SIZE_T MaxPoolUsage;
  SIZE_T MaxSectionSize;
  SIZE_T MaxViewSize;
  SIZE_T MaxTotalSectionSize;
  ULONG DupObjectTypes;
 #ifdef _M_X64
  ULONG Reserved;
 #endif
} ALPC_PORT_ATTRIBUTES, *PALPC_PORT_ATTRIBUTES;

typedef struct _ALPC_DATA_VIEW_ATTR
{
  ULONG Flags;
  ALPC_HANDLE SectionHandle;
  PVOID ViewBase; // must be zero on input
  SIZE_T ViewSize;
} ALPC_DATA_VIEW_ATTR, *PALPC_DATA_VIEW_ATTR;

typedef enum _ALPC_PORT_INFORMATION_CLASS
{
  AlpcBasicInformation, // q: out ALPC_BASIC_INFORMATION
  AlpcPortInformation, // s: in ALPC_PORT_ATTRIBUTES
  AlpcAssociateCompletionPortInformation, // s: in ALPC_PORT_ASSOCIATE_COMPLETION_PORT
  AlpcConnectedSIDInformation, // q: in SID
  AlpcServerInformation, // q: inout ALPC_SERVER_INFORMATION
  AlpcMessageZoneInformation, // s: in ALPC_PORT_MESSAGE_ZONE_INFORMATION
  AlpcRegisterCompletionListInformation, // s: in ALPC_PORT_COMPLETION_LIST_INFORMATION
  AlpcUnregisterCompletionListInformation, // s: VOID
  AlpcAdjustCompletionListConcurrencyCountInformation, // s: in ULONG
  AlpcRegisterCallbackInformation, // kernel-mode only
  AlpcCompletionListRundownInformation, // s: VOID
  MaxAlpcPortInfoClass
} ALPC_PORT_INFORMATION_CLASS;

typedef struct _ALPC_SECURITY_ATTR
{
  ULONG Flags;
  PSECURITY_QUALITY_OF_SERVICE SecurityQos;
  ALPC_HANDLE ContextHandle; // dbg
  ULONG Reserved1;
  ULONG Reserved2;
} ALPC_SECURITY_ATTR, *PALPC_SECURITY_ATTR;

typedef struct _ALPC_MESSAGE_ATTRIBUTES
{
  ULONG AllocatedAttributes;
  ULONG ValidAttributes;
} ALPC_MESSAGE_ATTRIBUTES, *PALPC_MESSAGE_ATTRIBUTES;

typedef struct _ALPC_CONTEXT_ATTR
{
  PVOID PortContext;
  PVOID MessageContext;
  ULONG Sequence;
  ULONG MessageId;
  ULONG CallbackId;
} ALPC_CONTEXT_ATTR, *PALPC_CONTEXT_ATTR;

VOID NTAPI TpReleasePool (__inout PTP_POOL Pool)
{
	;
}

VOID NTAPI TpReleaseCleanupGroup(__inout PTP_CLEANUP_GROUP CleanupGroup)
{
	;
}

NTSTATUS NTAPI TpAllocCleanupGroup(__out PTP_CLEANUP_GROUP *  	CleanupGroupReturn) 	
{
	return 0x00000000;
}

NTSTATUS NTAPI TpAllocPool(__out PTP_POOL *PoolReturn, __reserved PVOID Reserved)
{
	return 0x00000000;
}

NTSTATUS NTAPI TpAllocWait( 
		__out PTP_WAIT *  	WaitReturn,
		__in PTP_WAIT_CALLBACK  	Callback,
		__inout_opt PVOID  	Context,
		__in_opt PTP_CALLBACK_ENVIRON  	CallbackEnviron 
) 
{
	return 0x00000000;
}

VOID NTAPI TpSetWait(
		__inout PTP_WAIT  	Wait,
		__in_opt HANDLE  	Handle,
		__in_opt PLARGE_INTEGER  	Timeout 
) 	
{
	;
}

VOID NTAPI TpSetWaitEx(
		PTP_WAIT pwa, 
		HANDLE handle, 
		PFILETIME pftTimeout, 
		PVOID Reserved 
) 	
{
	;
}

NTSTATUS NTAPI TpSimpleTryPost( 
		__in PTP_SIMPLE_CALLBACK  	Callback,
		__inout_opt PVOID  	Context,
		__in_opt PTP_CALLBACK_ENVIRON  	CallbackEnviron 
) 	
{
	return 0x00000000;
}

VOID NTAPI TpWaitForIoCompletion (__inout PTP_IO Io, 
__in LOGICAL CancelPendingCallbacks)
{
	;
}

VOID NTAPI TpWaitForWork( 	__inout PTP_WORK  	Work,
		__in LOGICAL  	CancelPendingCallbacks 
)
{
	;
} 	

VOID NTAPI TpWaitForWait( 	
		__inout PTP_WAIT  	Wait,
		__in LOGICAL  	CancelPendingCallbacks 
) 		
{
	;
}

 VOID NTAPI TpReleaseCleanupGroupMembers(
	__inout PTP_CLEANUP_GROUP CleanupGroup, 
	__in LOGICAL CancelPendingCallbacks, 
	__inout_opt PVOID CleanupParameter)
{
	;
}

VOID NTAPI TpReleaseIoCompletion(__inout PTP_IO Io)
{
	;
}

VOID NTAPI TpReleaseTimer( 	__inout PTP_TIMER Timer) 
{
	;
}	

VOID NTAPI 	TpReleaseWait (__inout PTP_WAIT Wait)
{
	;
}

VOID NTAPI TpReleaseWork( 	__inout PTP_WORK  	Work	) 	
{
	;
}

VOID NTAPI TpSetTimer 	( 	__inout PTP_TIMER  	Timer,
		__in_opt PLARGE_INTEGER  	DueTime,
		__in LONG  	Period,
		__in_opt LONG  	WindowLength 
	) 	
{
	;
}

VOID NTAPI 	TpCallbackLeaveCriticalSectionOnCompletion(
	__inout PTP_CALLBACK_INSTANCE Instance, 
	__inout PRTL_CRITICAL_SECTION CriticalSection)
{
	;
}


/*subimplemented*/
NTSTATUS NTAPI
NtAlpcAcceptConnectPort(
    __out HANDLE                         PortHandle,
    __in HANDLE                          ConnectionPortHandle,
    __in ULONG                           Flags,
    __in POBJECT_ATTRIBUTES              ObjectAttributes,
    __in PALPC_PORT_ATTRIBUTES           PortAttributes,
    __in_opt PVOID                       PortContext, // opaque value
    __in PPORT_MESSAGE                   ConnectionRequest,
    __inout_opt PALPC_MESSAGE_ATTRIBUTES MessageAttributes,
    __in BOOLEAN                         AcceptConnection
    )
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcCancelMessage(__in HANDLE  	PortHandle,
		__in ULONG  	Flags,
		__in PALPC_CONTEXT_ATTR  	MessageContext 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcConnectPort( 	__out PHANDLE  	PortHandle,
		__in PUNICODE_STRING  	PortName,
		__in POBJECT_ATTRIBUTES  	ObjectAttributes,
		__in_opt PALPC_PORT_ATTRIBUTES  	PortAttributes,
		__in ULONG  	Flags,
		__in_opt PSID  	RequiredServerSid,
		__inout PPORT_MESSAGE  	ConnectionMessage,
		__inout_opt PULONG  	BufferLength,
		__inout_opt PALPC_MESSAGE_ATTRIBUTES  	OutMessageAttributes,
		__inout_opt PALPC_MESSAGE_ATTRIBUTES  	InMessageAttributes,
		__in_opt PLARGE_INTEGER  	Timeout 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcCreatePort( 	__out PHANDLE  	PortHandle,
		__in POBJECT_ATTRIBUTES  	ObjectAttributes,
		__in_opt PALPC_PORT_ATTRIBUTES  	PortAttributes 
	) 		
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcCreatePortSection( 	__in HANDLE  	PortHandle,
		__in ULONG  	Flags,
		__in_opt HANDLE  	SectionHandle,
		__in SIZE_T  	SectionSize,
		__out PALPC_HANDLE  	AlpcSectionHandle,
		__out PSIZE_T  	ActualSectionSize 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcCreateSecurityContext( 	__in HANDLE  	PortHandle,
		__reserved ULONG  	Flags,
		__inout PALPC_SECURITY_ATTR  	SecurityAttribute 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcDeletePortSection( 	__in HANDLE  	PortHandle,
		__reserved ULONG  	Flags,
		__in ALPC_HANDLE  	SectionHandle 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcDeleteSectionView( 	__in HANDLE  	PortHandle,
		__reserved ULONG  	Flags,
		__in PVOID  	ViewBase 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcDeleteSecurityContext( 	__in HANDLE  	PortHandle,
		__reserved ULONG  	Flags,
		__in ALPC_HANDLE  	ContextHandle 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcImpersonateClientOfPort( 	__in HANDLE  	PortHandle,
		__in PPORT_MESSAGE  	PortMessage,
		__reserved PVOID  	Reserved 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcQueryInformation( 	__in HANDLE  	PortHandle,
		__in ALPC_PORT_INFORMATION_CLASS  	PortInformationClass,
		__out_bcount(Length) PVOID  	PortInformation,
		__in ULONG  	Length,
		__out_opt PULONG  	ReturnLength 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcSendWaitReceivePort( 	__in HANDLE  	PortHandle,
		__in ULONG  	Flags,
		__in_opt PPORT_MESSAGE  	SendMessage,
		__in_opt PALPC_MESSAGE_ATTRIBUTES  	SendMessageAttributes,
		__inout_opt PPORT_MESSAGE  	ReceiveMessage,
		__inout_opt PULONG  	BufferLength,
		__inout_opt PALPC_MESSAGE_ATTRIBUTES  	ReceiveMessageAttributes,
		__in_opt PLARGE_INTEGER  	Timeout 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcSetInformation( 	__in HANDLE  	PortHandle,
		__in ALPC_PORT_INFORMATION_CLASS  	PortInformationClass,
		__in_bcount(Length) PVOID  	PortInformation,
		__in ULONG  	Length 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcCreateSectionView( 	__in HANDLE  	PortHandle,
		__reserved ULONG  	Flags,
		__inout PALPC_DATA_VIEW_ATTR  	ViewAttributes 
	) 	
{
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS NTAPI NtAlpcDisconnectPort(__in HANDLE PortHandle, __in ULONG Flags)
{
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS NTAPI NtAlpcCreateResourceReserve(__in HANDLE PortHandle, __reserved ULONG Flags, __in SIZE_T MessageSize, __out PALPC_HANDLE ResourceId)
{
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS NTAPI RtlCreateServiceSid(__in PUNICODE_STRING ServiceName, 
								   __out_bcount(*ServiceSidLength) PSID ServiceSid, 
								   __inout PULONG ServiceSidLength)
{
	return 0x00000000;
}

NTSTATUS
NTAPI
RtlAddMandatoryAce(
    _Inout_ PACL Acl,
    _In_ ULONG Revision,
    _In_ ULONG Flags,
    _In_ ULONG MandatoryFlags,
    _In_ UCHAR AceType,
    _In_ PSID LabelSid)	
{
	return 0x00000000;
}	

NTSTATUS WINAPI NtTraceControl(ULONG FunctionCode, PVOID InBuffer, ULONG InBufferLen, PVOID OutBuffer, ULONG OutBufferLen, PULONG ReturnLength)
{
	return 0x00000000;
}

//unknown prototype
DWORD WINAPI EtwLogTraceEvent(int a1, int a2, int a3)
{
	return 87;
}

//unknown prototype
DWORD WINAPI EtwpAddLogHeaderToLogFile(int a, int b, int c, int d)
{
	return 0;
}

PVOID WINAPI EtwpCreateEtwThread(HANDLE close, HANDLE handle)
{
  NTSTATUS status; // eax@2

  if ( RtlCreateUserThread((HANDLE)-1, 0, 1, 0, 0, 0, close, handle, handle, 0) >= 0 )
  {
    status = NtResumeThread(handle, 0);
    if ( status >= 0 )
      return handle;
    ZwTerminateThread(handle, status);
    NtClose(handle);
  }
  return 0;
}


ULONG WINAPI EtwEventActivityIdControl(
  _In_     ULONG ControlCode,
  _Inout_  LPGUID ActivityId
)
{
	return 0;
}

NTSTATUS WINAPI EtwpFinalizeLogFileHeader(int a1)
{
	return 0x00000000;
}

NTSTATUS WINAPI EtwpFlushUmLogger(int a1, int a2, int a3, int a4)
{
	return 0x00000000;
}

//unknown prototype
NTSTATUS WINAPI EtwpGetCpuSpeed(int a, int b)
{
	return 0x00000000;
}

BOOL WINAPI EtwpIsHeapRangeLoggingEnabled()
{
	return TRUE;
}

BOOL WINAPI EtwpIsPrivateLoggerOn()
{
	return TRUE;
}

DWORD WINAPI EtwpQueryUmLogger(ULONG a, int b, PULONG c, PULONG d)
{
	return 4312;
}

//unknown prototype
DWORD WINAPI EtwpQueryLogger(PVOID other, int b)
{
	return 4312;
}

//unknown prototype
DWORD WINAPI EtwpSendUmLogRequest(ULONG a, int b, PULONG c, PULONG d)
{
	return 8;
}

//unknown prototype
DWORD WINAPI EtwpStartUmLogger(int a, int b, int c , PULONG d)
{
	return 87;
}

//unknown prototype
DWORD WINAPI EtwpStopUmLogger(int a, int b, int c , PULONG d)
{
	return 4201;
}

//unknown prototype
DWORD WINAPI EtwpUpdateUmLogger(int a, int b, int c , PULONG d)
{
	return 0;
}


BOOL WINAPI EtwpIsProcessExiting()
{
  return TRUE;
}


//unknown prototype
NTSTATUS WINAPI EtwpSendWmiKMRequest(int a, int b, int c, int d, int e, int f, int g, WCHAR h)
{
	return 0x00000000;
}

//unknown prototype
int WINAPI ShipAssert(int a1, int a2)
{
	return 0;
}

/* unimplemented*/
ULONG WINAPI EtwEventRegister(
  _In_      LPCGUID ProviderId,
  _In_opt_  PENABLECALLBACK EnableCallback,
  _In_opt_  PVOID CallbackContext,
  _Out_     PREGHANDLE RegHandle
)
{
	return ERROR_SUCCESS;
}

/* unimplemented*/
ULONG WINAPI EtwEventUnregister(
  _In_  REGHANDLE RegHandle
)
{
	return ERROR_SUCCESS;
}

/* unimplemented*/
ULONG WINAPI EtwEventWrite(
  _In_      REGHANDLE RegHandle,
  _In_      PCEVENT_DESCRIPTOR EventDescriptor,
  _In_      ULONG UserDataCount,
  _In_opt_  PEVENT_DATA_DESCRIPTOR UserData
)
{
	return ERROR_SUCCESS;
}


ULONG WINAPI EtwEventWriteEndScenario(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}


ULONG WINAPI EtwEventWriteStartScenario(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}

/* unimplemented*/
BOOLEAN WINAPI EtwEventEnabled(
  IN REGHANDLE RegHandle,
  IN PCEVENT_DESCRIPTOR EventDescriptor)
{
	return TRUE;
}

//unknown prototype
NTSTATUS WINAPI LdrQueryModuleServiceTags(PVOID Base, PVOID a2, ULONG number)
{
	return 0x00000000;
}		

//unknown prototype
NTSTATUS WINAPI EtwNotificationRegister(int a1, unsigned __int8 a2, int a3, int a4, int a5)
{
	return 0x00000000;
}	

//unknown prototype
NTSTATUS WINAPI EtwNotificationUnregister(int a1, ULONG a2, int a3)
{
	return 0x00000000;
}	

//unknown prototype
NTSTATUS WINAPI EtwSendNotification(int a1, int a2, int a3, int a4, int a5)
{
	return 0x00000000;
}	

BOOL WINAPI RtlGetFileMUIPath(
  _In_         DWORD dwFlags,
  _In_         PCWSTR pcwszFilePath,
  _Inout_opt_  PWSTR pwszLanguage,
  _Inout_      PULONG pcchLanguage,
  _Out_opt_    PWSTR pwszFileMUIPath,
  _Inout_      PULONG pcchFileMUIPath,
  _Inout_      PULONGLONG pululEnumerator
)
{
	return TRUE;
}

BOOL WINAPI RtlGetSystemPreferredUILanguages(
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
)
{
	return TRUE;
}

BOOL NTAPI RtlGetThreadPreferredUILanguages(
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
)
{
	return TRUE;
}

/*
 * @implemented - fazer versão x64
 */
HANDLE WINAPI RtlGetCurrentTransaction()
{
  return NULL;
}

/*
 * @implemented - fazer versão x64
 */
BOOL WINAPI RtlSetCurrentTransaction(HANDLE new_transaction)
{
  return TRUE;
}

DWORD WINAPI EtwEnumerateProcessRegGuids(PVOID a1, ULONG a2, int a3)
{
	return 0;
}

ULONG WINAPI EtwDeliverDataBlock(int a1)
{
	return 0;
}

DWORD WINAPI EtwProcessPrivateLoggerRequest(PVOID number)
{
	return 4209;
}

NTSTATUS NTAPI NtCreateKeyTransacted (__out PHANDLE KeyHandle, 
									  __in ACCESS_MASK DesiredAccess, 
									  __in POBJECT_ATTRIBUTES ObjectAttributes, 
									  __reserved ULONG TitleIndex, 
									  __in_opt PUNICODE_STRING Class, 
									  __in ULONG CreateOptions, 
									  __in HANDLE TransactionHandle, 
									  __out_opt PULONG Disposition)
{
	return 0x00000000;
}

NTSTATUS NTAPI NtOpenKeyTransacted(__out PHANDLE KeyHandle, 
								   __in ACCESS_MASK DesiredAccess, 
								   __in POBJECT_ATTRIBUTES ObjectAttributes, 
								   __in HANDLE TransactionHandle)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlRegisterThreadWithCsrss()
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlRemovePrivileges(HANDLE a1, PULONG a2, ULONG a3)
{
	return 0x00000000;
}

NTSTATUS NTAPI 	TpAllocTimer (__out PTP_TIMER *Timer, 
							  __in PTP_TIMER_CALLBACK Callback, 
							  __inout_opt PVOID Context, 
							  __in_opt PTP_CALLBACK_ENVIRON CallbackEnviron)
{
	return 0x00000000;
}

NTSTATUS NTAPI TpAllocWork 	( 	__out PTP_WORK *  	WorkReturn,
		__in PTP_WORK_CALLBACK  	Callback,
		__inout_opt PVOID  	Context,
		__in_opt PTP_CALLBACK_ENVIRON  	CallbackEnviron )
{
	return 0x00000000;
}

VOID NTAPI TpPostWork(__inout PTP_WORK Work) 	
{
	;
}

VOID NTAPI TpWaitForTimer( 	__inout PTP_TIMER  	Timer,
		__in LOGICAL  	CancelPendingCallbacks ) 
{
	;
}		

VOID NTAPI TpSetPoolMaxThreads( 	__inout PTP_POOL  	Pool,
		__in LONG  	MaxThreads 
) 	
{
	;
}

NTSTATUS NTAPI TpSetPoolMinThreads 	(__inout PTP_POOL  	Pool,
		__in LONG  	MinThreads 
) 	
{
	return 0x00000000;
}

/*
 * @implemented - need test
*/
BOOLEAN NTAPI RtlCultureNameToLCID(__in PUNICODE_STRING String, __out PLCID Lcid)
{
	return TRUE;
}

/*unimplemented */
BOOLEAN NTAPI RtlLCIDToCultureName(__in LCID Lcid,__inout PUNICODE_STRING String)
{
	return TRUE;
} 	

/*unimplemented */
NTSTATUS NTAPI RtlpQueryDefaultUILanguage(LANGID a1, BOOLEAN a2)
{
	return 0x00000000;
}

/*unimplemented */
NTSTATUS NTAPI RtlpSetCurrentUserUILanguage(unsigned __int16 a1)
{
	return 0x00000000;
}

/*unimplemented */
PVOID WINAPI WinSqmAddToStream(PVOID Source1, BOOL a2, int a3, PVOID a4)
{
	return NULL;
}

/*unimplemented */
BOOL WINAPI WinSqmAddToStreamEx(PVOID a1, int a2, ULONG a3, int a4, int a5)
{
	return TRUE;
}

/*unimplemented */
BOOL WINAPI WinSqmEndSession(LPVOID Source1)
{
	return TRUE;
}

/*unimplemented */
int  WINAPI WinSqmIncrementDWORD(int a1, int a2, int a3)
{
  return 0;
}

/*unimplemented */
int  WINAPI WinSqmSetIfMaxDWORD(int a1, int a2, int a3)
{
  return 0;
}

int WINAPI WinSqmStartSession(int Source1, ULONG Value, int number)
{
  return 0;
}

NTSTATUS NTAPI RtlFlsAlloc(PFLS_CALLBACK_FUNCTION lpCallback, DWORD *other)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlFlsFree(DWORD dwFlsIndex)
{
	return 0x00000000;
}

PVOID NTAPI AlpcGetMessageAttribute(__in PALPC_MESSAGE_ATTRIBUTES  	Buffer,
		__in ULONG  	AttributeFlag 
) 	
{
	return NULL;
}


NTSTATUS NTAPI AlpcInitializeMessageAttribute 	( 	__in ULONG  	AttributeFlags,
		__out_opt PALPC_MESSAGE_ATTRIBUTES  	Buffer,
		__in ULONG  	BufferSize,
		__out PULONG  	RequiredBufferSize 
) 	
{
	return 0x00000000;
}


VOID WINAPI RtlWakeAllConditionVariable(
  _Inout_  PRTL_CONDITION_VARIABLE ConditionVariable
)
{
	;
}

VOID NTAPI RtlAcquireSRWLockExclusive(
 IN OUT PRTL_SRWLOCK SRWLock) 	
{
	;
}

VOID NTAPI RtlInitializeSRWLock(OUT PRTL_SRWLOCK SRWLock) 
{
	SRWLock->Ptr = NULL;
}	

VOID NTAPI RtlAcquireSRWLockShared( 
 IN OUT PRTL_SRWLOCK SRWLock) 
{
	;
}

VOID NTAPI RtlReleaseSRWLockExclusive( 	
 IN OUT PRTL_SRWLOCK SRWLock) 
{
	;
}

VOID NTAPI RtlReleaseSRWLockShared(
 IN OUT PRTL_SRWLOCK SRWLock) 
{
	;
}

NTSTATUS NTAPI RtlSleepConditionVariableCS( 	
		IN OUT PRTL_CONDITION_VARIABLE ConditionVariable,
		IN OUT PRTL_CRITICAL_SECTION CriticalSection,
		IN PLARGE_INTEGER TimeOut OPTIONAL
	) 		
{
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS NTAPI RtlSleepConditionVariableSRW( 	
		IN OUT PRTL_CONDITION_VARIABLE  	ConditionVariable,
		IN OUT PRTL_SRWLOCK  	SRWLock,
		IN PLARGE_INTEGER TimeOut  	OPTIONAL,
		IN ULONG  	Flags 
	) 		
{
    return STATUS_NOT_IMPLEMENTED;
}

VOID NTAPI RtlWakeConditionVariable(
	IN OUT PRTL_CONDITION_VARIABLE ConditionVariable) 	
{
	;
} 

BOOL WINAPI TpIsTimerSet(
  _Inout_  PTP_TIMER pti
)
{
	return TRUE;
}

VOID NTAPI TpDisassociateCallback( 	
	__inout PTP_CALLBACK_INSTANCE  	Instance
) 	
{
	;
} 

VOID WINAPI RtlDeleteBoundaryDescriptor(
  _In_  HANDLE BoundaryDescriptor
)
{
	;
} 

VOID NTAPI 	TpStartAsyncIoOperation(
	__inout PTP_IO Io
)
{
	;
} 

VOID NTAPI 	TpCancelAsyncIoOperation(
	__inout PTP_IO Io
)
{
	;
} 

NTSTATUS NTAPI TpCallbackMayRunLong(
	__inout PTP_CALLBACK_INSTANCE  	Instance	
)
{
	return 0x00000000;
} 	

VOID NTAPI 	TpCallbackUnloadDllOnCompletion(
	__inout PTP_CALLBACK_INSTANCE Instance, 
	__in PVOID DllHandle
)
{
	;
} 

VOID NTAPI TpCallbackReleaseMutexOnCompletion( 
		__inout PTP_CALLBACK_INSTANCE  	Instance,
		__in HANDLE  	Mutex 
) 	
{
	;
} 

VOID NTAPI TpCallbackReleaseSemaphoreOnCompletion( 	
		__inout PTP_CALLBACK_INSTANCE  	Instance,
		__in HANDLE  	Semaphore,
		__in LONG  	ReleaseCount 
) 
{
	;
} 	

NTSTATUS WINAPI RtlGetUILanguageInfo(DWORD dwFlags, 
									 PCZZWSTR pwmszLanguage, 
									 PZZWSTR pwszFallbackLanguages,
									 PDWORD pcchFallbackLanguages,
									 PDWORD pdwAttributes)
{
	return 0x00000000;
}

NTSTATUS WINAPI RtlGetUserPreferredUILanguages(DWORD dwFlags, 
											   BOOL verification,
											   PULONG pulNumLanguages, 
											   PZZWSTR pwszLanguagesBuffer, 
											   PULONG pcchLanguagesBuffer)
{
	return 0x00000000;
}

BOOL WINAPI RtlDeregisterSecureMemoryCacheCallback(
  _In_  PRTL_SECURE_MEMORY_CACHE_CALLBACK pfnCallBack
)
{
	return TRUE;
}

NTSTATUS WINAPI RtlRegisterSecureMemoryCacheCallback(
  _In_  PRTL_SECURE_MEMORY_CACHE_CALLBACK pfnCallBack
)
{
	return 0x00000000;
}

NTSTATUS WINAPI RtlAddSIDToBoundaryDescriptor(
  _Inout_  HANDLE *BoundaryDescriptor,
  _In_     PSID RequiredSid
)
{
	return 0x00000000;
}

HANDLE WINAPI RtlCreateBoundaryDescriptor(LSA_UNICODE_STRING *string, ULONG Flags)
{
	return NULL;
}

NTSTATUS NTAPI NtDeletePrivateNamespace(
  __in HANDLE NamespaceHandle
)
{
	return 0x00000000;
}

NTSTATUS NTAPI NtCreatePrivateNamespace(
  __out PHANDLE NamespaceHandle,
  __in ACCESS_MASK DesiredAccess,
  __in_opt POBJECT_ATTRIBUTES ObjectAttributes,
  __in PVOID BoundaryDescriptor
)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlIsNormalizedString(NORM_FORM NormForm, 
									 LPCWSTR lpString, 
									 int cwLength,
									 LPCWSTR other
)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlNormalizeString(
  _In_       NORM_FORM NormForm,
  _In_       LPCWSTR lpSrcString,
  _In_       int cwSrcLength,
  _Out_opt_  LPWSTR lpDstString,
  _In_       int *cwDstLength
)
{
	return 0x00000000;
}

NTSTATUS NTAPI
NtRemoveIoCompletionEx(
    __in HANDLE IoCompletionHandle,
    __out FILE_IO_COMPLETION_INFORMATION IoCompletionInformation,
    __in ULONG Count,
    __out PVOID NumEntriesRemoved,
    __in_opt PLARGE_INTEGER Timeout,
    __in BOOLEAN Alertable
)
{
	return 0x00000000;
}

VOID NTAPI 	TpCallbackSetEventOnCompletion (__inout PTP_CALLBACK_INSTANCE Instance, 
											__in HANDLE Event)
{
	;
}

NTSTATUS
NTAPI
NtOpenPrivateNamespace(
  __out PHANDLE NamespaceHandle,
  __in ACCESS_MASK DesiredAccess,
  __in_opt POBJECT_ATTRIBUTES ObjectAttributes,
  __in PVOID BoundaryDescriptor
)
{
	return 0x00000000;
}

NTSTATUS NTAPI 	NtReplacePartitionUnit (__in PUNICODE_STRING TargetInstancePath, 
										__in PUNICODE_STRING SpareInstancePath,
										__in ULONG Flags)
{
	return 0x00000000;
}

BOOLEAN NTAPI RtlTryAcquireSRWLockExclusive(
  _Inout_  PRTL_SRWLOCK SRWLock
)
{
	return TRUE;
}


BOOLEAN NTAPI RtlTryAcquireSRWLockShared(
  _Inout_  PRTL_SRWLOCK SRWLock
)
{
	return TRUE;
}

NTSTATUS NTAPI TpSetPoolStackInformation(
  _Inout_  PTP_POOL ptpp,
  _In_     PTP_POOL_STACK_INFORMATION ptpsi
)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlReadThreadProfilingData(HANDLE PerformanceDataHandle, DWORD Flags, PPERFORMANCE_DATA PerformanceData)
{
	return 0x00000000;
}

NTSTATUS NTAPI NtQuerySystemInformationEx(SYSTEM_INFORMATION_CLASS information,
										  PUSHORT Group, 
										  DWORD number, 
										  PULONG64 ProcessorIdleCycleTime, 
										  PULONG *BufferLength, 
										  PULONG OtherBufferLength)
{
	return 0x00000000;
}

NTSTATUS NTAPI TpQueryPoolStackInformation (
  __in PTP_POOL Pool, 
  __out PTP_POOL_STACK_INFORMATION PoolStackInformation
)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlQueryThreadProfiling(HANDLE HANDLE, PBOOLEAN Enabled)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlCopyContext(PCONTEXT Destination, DWORD ContextFlags, PCONTEXT Source)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlDisableThreadProfiling(HANDLE PerformanceDataHandle)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlAddIntegrityLabelToBoundaryDescriptor(HANDLE *BoundaryDescriptor, PSID IntegrityLabel)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlEnableThreadProfiling(HANDLE ThreadHandle, 
										DWORD Flags, 
										DWORD64 HardwareCounters, 
										HANDLE PerformanceDataHandle)
{
	return 0x00000000;
}


VOID NTAPI RtlGetCurrentProcessorNumberEx(
  _Out_  PPROCESSOR_NUMBER ProcNumber
)
{
	;
}

PCONTEXT NTAPI RtlLocateLegacyContext(PCONTEXT oldContext, BOOL other)
{
	return oldContext;
}

NTSTATUS NTAPI RtlInitializeExtendedContext(PVOID Buffer, DWORD flags, PCONTEXT *oldContext)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlGetExtendedContextLength(DWORD flags, LPDWORD ContextFlags)
{
	return 0x00000000;
}

ULONG64 NTAPI RtlGetEnabledExtendedFeatures(_In_  ULONG64 FeatureMask)
{
	return FeatureMask;
}

PVOID NTAPI RtlLocateExtendedFeature(BYTE reception, DWORD FeatureId, PDWORD Length)
{
	return NULL;
}

VOID NTAPI RtlSetExtendedFeaturesMask(PCONTEXT Context, DWORD64 FeatureMask)
{
	;
}

BOOL NTAPI WinSqmIsOptedInEx(ULONG number)
{
	return TRUE;
}

DLL_DIRECTORY_COOKIE  WINAPI LdrAddDllDirectory(
  _In_  PCWSTR NewDirectory
)
{
	return NULL;
}

BOOL  WINAPI LdrRemoveDllDirectory(
  _In_  DLL_DIRECTORY_COOKIE Cookie
)
{
	return TRUE;
}

NTSTATUS NTAPI RtlUnsubscribeWnfStateChangeNotification(PVOID RegistrationHandle)
{
	return 0x00000000;
}

DWORD NTAPI RtlGetSystemTimePrecise()
{
	return 0;
}

PDWORD64 WINAPI RtlGetExtendedFeaturesMask(UCHAR *contextLocal)
{
	return 0;
}

NTSTATUS NTAPI RtlCheckTokenCapability(
  _In_opt_  HANDLE TokenHandle,
  _In_      PSID CapabilitySidToCheck,
  _Out_     PBOOL HasCapability
)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlCheckTokenMembershipEx(
	HANDLE TokenHandle, 
	PSID SidToCheck, 
	DWORD Flas, 
	PBOOL IsMember
)
{
	return 0x00000000;
}	

NTSTATUS NTAPI NtSetInformationVirtualMemory(HANDLE hProcess,
											 BOOL access,
											 ULONG_PTR NumberOfEntries, 
											 PWIN32_MEMORY_RANGE_ENTRY VirtualAddresses, 
											 PULONG Flags,
											 DWORD other
)
{
	return 0x00000000;
}	

PVOID NTAPI LdrResolveDelayLoadedAPI(
  _In_        PVOID ParentModuleBase,
  _In_        PCIMAGE_DELAYLOAD_DESCRIPTOR DelayloadDescriptor,
  _In_opt_    PDELAYLOAD_FAILURE_DLL_CALLBACK FailureDllHook,
  _In_opt_    PDELAYLOAD_FAILURE_SYSTEM_ROUTINE FailureSystemHook,
  _Out_       PIMAGE_THUNK_DATA ThunkAddress,
  _Reserved_  ULONG Flags
)
{
	return 0;
}

NTSTATUS NTAPI RtlAddResourceAttributeAce(PACL pAcl, 
										  DWORD dwAceRevision, 
										  DWORD AceFlags, 
										  DWORD AccessMask, 
										  PSID pSid, 
										  PCLAIM_SECURITY_ATTRIBUTES_INFORMATION  pAttributeInfo, 
										  PDWORD pReturnLength)
{
	return 0x00000000;	
}

NTSTATUS WINAPI LdrResolveDelayLoadsFromDll(
  _In_        PVOID ParentBase,
  _In_        LPCSTR TargetDllName,
  _Reserved_  ULONG Flags
)
{
	return 0x00000000;	
}

NTSTATUS WINAPI NtUnmapViewOfSectionEx(HANDLE handle, PVOID MemoryCache, ULONG number)
{
	return 0x00000000;	
}

BOOL WINAPI TpSetTimerEx(PTP_TIMER pti, 
						 PFILETIME pftDueTime, 
						 DWORD msPeriod, 
						 DWORD msWindowLength)
{
	return TRUE;
}

BOOL NTAPI RtlGetIntegerAtom(ULONG number, ATOM receive)
{
	return TRUE;
}

BOOLEAN NTAPI RtlDosPathNameToRelativeNtPathName_U( 	
		IN PCWSTR  	DosName,
		OUT PUNICODE_STRING  	NtName,
		OUT PCWSTR *  	PartName,
		OUT PRTL_RELATIVE_NAME_U  	RelativeName 
) 	
{
	return TRUE;
}

VOID NTAPI RtlReleaseRelativeName( 	
	IN PRTL_RELATIVE_NAME_U  	RelativeName	
) 	
{
	;
}

NTSTATUS NTAPI LdrOpenImageFileOptionsKey( 	
		IN PUNICODE_STRING  	SubKey,
		IN BOOLEAN  	Wow64,
		OUT PHANDLE  	NewKeyHandle
) 	
{
	return 0x00000000;	
}

NTSTATUS NTAPI LdrQueryImageFileKeyOption( 	
		IN HANDLE  	KeyHandle,
		IN PCWSTR  	ValueName,
		IN ULONG  	Type,
		OUT PVOID  	Buffer,
		IN ULONG  	BufferSize,
		OUT PULONG ReturnedLength  	OPTIONAL 
) 	
{
	return 0x00000000;	
}

ULONG NTAPI RtlGetThreadErrorMode(VOID) 	
{
    return 0;
}

NTSTATUS NTAPI RtlInitAnsiStringEx( 
		IN OUT PANSI_STRING  	DestinationString,
		IN PCSZ  	SourceString 
) 	
{
	return 0x00000000;	
}

NTSTATUS NTAPI RtlSetThreadErrorMode( 	
		IN ULONG  	NewMode,
		OUT PULONG OldMode  	OPTIONAL 
) 	
{
	return 0x00000000;	
}

NTSTATUS NTAPI NtLoadKeyEx 	( 	
	IN POBJECT_ATTRIBUTES  	TargetKey,
	IN POBJECT_ATTRIBUTES  	SourceFile,
	IN ULONG  	Flags,
	IN HANDLE  	TrustClassKey 
)
{
	return 0x00000000;	
} 	

NTSTATUS NTAPI NtQueryOpenSubKeysEx( 	
	IN POBJECT_ATTRIBUTES  	TargetKey,
	IN ULONG  	BufferLength,
	IN PVOID  	Buffer,
	IN PULONG  	RequiredSize 
	) 		
{
    return 0x00000000;
}	

NTSTATUS
NTAPI
RtlProcessFlsData (
    ULONG Buffer
)
{
	return 0x00000000;
}

NTSTATUS NTAPI RtlWow64EnableFsRedirectionEx(	
	IN PVOID 	Wow64FsEnableRedirection,
	OUT PVOID * 	OldFsRedirectionLevel 
)	
{
	return STATUS_SUCCESS;			
}

PVOID
NTAPI
LdrLoadAlternateResourceModule(
    IN PVOID Module,
    IN LPCWSTR PathToAlternateModule OPTIONAL
    )
{
	return NULL;
}

BOOLEAN
LdrAlternateResourcesEnabled(
    VOID
    )
{
	return FALSE;	
}

NTSTATUS
NTAPI
LdrAccessResource(
    IN PVOID DllHandle,
    IN PIMAGE_RESOURCE_DATA_ENTRY ResourceDataEntry,
    OUT PVOID *Address OPTIONAL,
    OUT PULONG Size OPTIONAL
    )
{
	return STATUS_SUCCESS;			
}

NTSTATUS
NTAPI
LdrFindResource_U(
    IN PVOID DllHandle,
    IN PULONG_PTR ResourceIdPath,
    IN ULONG ResourceIdPathLength,
    OUT PIMAGE_RESOURCE_DATA_ENTRY *ResourceDataEntry
    )
{
	return STATUS_SUCCESS;			
}