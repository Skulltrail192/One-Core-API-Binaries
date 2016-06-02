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
#include <Ntsecapi.h>
#include <evntprov.h>
#include <ntdef.h>

#define MAX_HW_COUNTERS 16

typedef ULONG LOGICAL;

typedef DWORD TP_VERSION, *PTP_VERSION;

typedef DWORD TP_WAIT_RESULT;

typedef PVOID* PALPC_HANDLE;

typedef PVOID ALPC_HANDLE;

typedef HANDLE 	DLL_DIRECTORY_COOKIE;

typedef struct _TP_POOL TP_POOL, *PTP_POOL;

typedef struct _TP_TIMER TP_TIMER, *PTP_TIMER;

typedef struct _TP_CLEANUP_GROUP TP_CLEANUP_GROUP, *PTP_CLEANUP_GROUP;

typedef struct _TP_CALLBACK_INSTANCE TP_CALLBACK_INSTANCE, *PTP_CALLBACK_INSTANCE;

typedef VOID (*PTP_TIMER_CALLBACK)(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_TIMER Timer);

typedef struct _TP_WAIT TP_WAIT, *PTP_WAIT;

typedef TP_CALLBACK_ENVIRON_V1 TP_CALLBACK_ENVIRON, *PTP_CALLBACK_ENVIRON;

typedef VOID (*PTP_WAIT_CALLBACK)(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WAIT Wait, TP_WAIT_RESULT WaitResult);

typedef VOID (WINAPI *PBAD_MEMORY_CALLBACK_ROUTINE)(VOID);

typedef BOOLEAN (*PSECURE_MEMORY_CACHE_CALLBACK)(
    _In_  PVOID Addr,
    _In_  SIZE_T Range
);

typedef PVOID PDELAYLOAD_FAILURE_SYSTEM_ROUTINE; 

typedef struct _TP_IO{
	void * 	dummy;
}TP_IO,*PTP_IO;

typedef enum _NORM_FORM { 
  NormalizationOther  = 0,
  NormalizationC      = 0x1,
  NormalizationD      = 0x2,
  NormalizationKC     = 0x5,
  NormalizationKD     = 0x6
} NORM_FORM;

typedef struct _WIN32_MEMORY_RANGE_ENTRY {
  PVOID  VirtualAddress;
  SIZE_T NumberOfBytes;
} WIN32_MEMORY_RANGE_ENTRY, *PWIN32_MEMORY_RANGE_ENTRY;

typedef struct _CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE {
  PVOID pValue;
  DWORD ValueLength;
} CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE, *PCLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE;

typedef struct _CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE {
  DWORD64 Version;
  PWSTR   Name;
} CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE, *PCLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE;

typedef struct _CLAIM_SECURITY_ATTRIBUTE_V1 {
  PWSTR Name;
  WORD  ValueType;
  WORD  Reserved;
  DWORD Flags;
  DWORD ValueCount;
  union {
    PLONG64                                      pInt64;
    PDWORD64                                     pUint64;
    PWSTR                                        *ppString;
    PCLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE         pFqbn;
    PCLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE pOctetString;
  } Values;
} CLAIM_SECURITY_ATTRIBUTE_V1, *PCLAIM_SECURITY_ATTRIBUTE_V1;

typedef struct _CLAIM_SECURITY_ATTRIBUTES_INFORMATION {
  WORD  Version;
  WORD  Reserved;
  DWORD AttributeCount;
  union {
    PCLAIM_SECURITY_ATTRIBUTE_V1 pAttributeV1;
  } Attribute;
} CLAIM_SECURITY_ATTRIBUTES_INFORMATION, *PCLAIM_SECURITY_ATTRIBUTES_INFORMATION;

typedef struct _TP_POOL_STACK_INFORMATION
{
   SIZE_T StackReserve;
   SIZE_T StackCommit;
} TP_POOL_STACK_INFORMATION,*PTP_POOL_STACK_INFORMATION;

typedef enum  { 
  PMCCounter,
  MaxHardwareCounterType
} HARDWARE_COUNTER_TYPE;

typedef struct _HARDWARE_COUNTER_DATA {
  HARDWARE_COUNTER_TYPE Type;
  DWORD                 Reserved;
  DWORD64               Value;
} HARDWARE_COUNTER_DATA, *PHARDWARE_COUNTER_DATA;

typedef struct _PERFORMANCE_DATA {
  WORD                  Size;
  BYTE                  Version;
  BYTE                  HwCountersCount;
  DWORD                 ContextSwitchCount;
  DWORD64               WaitReasonBitMap;
  DWORD64               CycleTime;
  DWORD                 RetryCount;
  DWORD                 Reserved;
  HARDWARE_COUNTER_DATA HwCounters[MAX_HW_COUNTERS];
} PERFORMANCE_DATA, *PPERFORMANCE_DATA;

typedef struct _DELAYLOAD_PROC_DESCRIPTOR
{
     ULONG ImportDescribedByName;
     union {
         LPCSTR Name;
         ULONG Ordinal;
     } Description;
} DELAYLOAD_PROC_DESCRIPTOR, *PDELAYLOAD_PROC_DESCRIPTOR;

typedef struct _DELAYLOAD_INFO
{
     ULONG Size;
     PCIMAGE_DELAYLOAD_DESCRIPTOR DelayloadDescriptor;
     PIMAGE_THUNK_DATA ThunkAddress;
     LPCSTR TargetDllName;
     DELAYLOAD_PROC_DESCRIPTOR TargetApiDescriptor;
     PVOID TargetModuleBase;
     PVOID Unused;
     ULONG LastError;
} DELAYLOAD_INFO, *PDELAYLOAD_INFO;
typedef PVOID (WINAPI *PDELAYLOAD_FAILURE_DLL_CALLBACK)(ULONG, PDELAYLOAD_INFO);

/* unimplemented*/
ULONG WINAPI EtwEventRegister(
  _In_      LPCGUID ProviderId,
  _In_opt_  PENABLECALLBACK EnableCallback,
  _In_opt_  PVOID CallbackContext,
  _Out_     PREGHANDLE RegHandle
);

/* unimplemented*/
ULONG WINAPI EtwEventUnregister(
  _In_  REGHANDLE RegHandle
);

/* unimplemented*/
ULONG WINAPI EtwEventWrite(
  _In_      REGHANDLE RegHandle,
  _In_      PCEVENT_DESCRIPTOR EventDescriptor,
  _In_      ULONG UserDataCount,
  _In_opt_  PEVENT_DATA_DESCRIPTOR UserData
);

ULONG WINAPI EtwEventWriteEndScenario(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData);


ULONG WINAPI EtwEventWriteStartScenario(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData);


/* unimplemented*/
BOOLEAN WINAPI EtwEventEnabled(
  IN REGHANDLE RegHandle,
  IN PCEVENT_DESCRIPTOR EventDescriptor);

BOOL WINAPI RtlGetFileMUIPath(
  _In_         DWORD dwFlags,
  _In_         PCWSTR pcwszFilePath,
  _Inout_opt_  PWSTR pwszLanguage,
  _Inout_      PULONG pcchLanguage,
  _Out_opt_    PWSTR pwszFileMUIPath,
  _Inout_      PULONG pcchFileMUIPath,
  _Inout_      PULONGLONG pululEnumerator
);

NTSTATUS WINAPI RtlGetUILanguageInfo(DWORD dwFlags, 
									 PCZZWSTR pwmszLanguage, 
									 PZZWSTR pwszFallbackLanguages,
									 PDWORD pcchFallbackLanguages,
									 PDWORD pdwAttributes);

BOOL WINAPI RtlGetSystemPreferredUILanguages(
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
);

HANDLE WINAPI RtlGetCurrentTransaction();

BOOL WINAPI RtlSetCurrentTransaction();

NTSYSAPI
NTSTATUS
NTAPI
RtlAddMandatoryAce(
    _Inout_ PACL Acl,
    _In_ ULONG Revision,
    _In_ ULONG Flags,
    _In_ ULONG MandatoryFlags,
    _In_ UCHAR AceType,
    _In_ PSID LabelSid); 		
							
ULONG WINAPI EtwEventActivityIdControl(
  _In_     ULONG ControlCode,
  _Inout_  LPGUID ActivityId
);

NTSTATUS WINAPI RtlFlsAlloc(PFLS_CALLBACK_FUNCTION lpCallback, 
							DWORD *lpCallbackPointer);

NTSTATUS WINAPI RtlFlsFree(DWORD dwFlsIndex);

NTSTATUS NTAPI TpAllocPool(__out PTP_POOL *PoolReturn, __reserved PVOID Reserved);

NTSTATUS NTAPI TpAllocCleanupGroup(__out PTP_CLEANUP_GROUP *CleanupGroupReturn);

NTSTATUS NTAPI TpAllocTimer (__out PTP_TIMER *Timer, 
							  __in PTP_TIMER_CALLBACK Callback, 
							  __inout_opt PVOID Context, 
							  __in_opt PTP_CALLBACK_ENVIRON CallbackEnviron);
							  
NTSTATUS WINAPI RtlReadThreadProfilingData(HANDLE PerformanceDataHandle, DWORD Flags, PPERFORMANCE_DATA PerformanceData);							  

NTSTATUS NTAPI TpAllocWait( 
		__out PTP_WAIT *  	WaitReturn,
		__in PTP_WAIT_CALLBACK  	Callback,
		__inout_opt PVOID  	Context,
		__in_opt PTP_CALLBACK_ENVIRON  	CallbackEnviron);

NTSTATUS NTAPI TpAllocWork 	( 	__out PTP_WORK *  	WorkReturn,
		__in PTP_WORK_CALLBACK  	Callback,
		__inout_opt PVOID  	Context,
		__in_opt PTP_CALLBACK_ENVIRON  	CallbackEnviron );

VOID NTAPI TpSetPoolMaxThreads 	( 	__inout PTP_POOL  	Pool,
		__in LONG  	MaxThreads 
	);

NTSTATUS NTAPI TpSetPoolMinThreads 	(__inout PTP_POOL  	Pool,
		__in LONG  	MinThreads 
);

BOOL WINAPI TpIsTimerSet(
  _Inout_  PTP_TIMER pti
);

NTSTATUS NTAPI TpAllocWork(
		__out PTP_WORK *  	WorkReturn,
		__in PTP_WORK_CALLBACK  	Callback,
		__inout_opt PVOID  	Context,
		__in_opt PTP_CALLBACK_ENVIRON  	CallbackEnviron 
);

NTSTATUS NTAPI TpSimpleTryPost( 
		__in PTP_SIMPLE_CALLBACK  	Callback,
		__inout_opt PVOID  	Context,
		__in_opt PTP_CALLBACK_ENVIRON  	CallbackEnviron 
);  

NTSTATUS NTAPI NtQuerySystemInformationEx(SYSTEM_INFORMATION_CLASS information,
										  PUSHORT Group, 
										  DWORD number, 
										  PULONG64 ProcessorIdleCycleTime, 
										  PULONG *BufferLength, 
										  PULONG OtherLength);	

NTSTATUS NTAPI RtlSleepConditionVariableSRW( 	
		IN OUT PRTL_CONDITION_VARIABLE  	ConditionVariable,
		IN OUT PRTL_SRWLOCK  	SRWLock,
		IN PLARGE_INTEGER TimeOut  	OPTIONAL,
		IN ULONG  	Flags 
); 	

VOID NTAPI RtlInitializeSRWLock(OUT PRTL_SRWLOCK SRWLock);

NTSTATUS NTAPI RtlSleepConditionVariableCS( 	
		IN OUT PRTL_CONDITION_VARIABLE ConditionVariable,
		IN OUT PRTL_CRITICAL_SECTION CriticalSection,
		IN PLARGE_INTEGER TimeOut OPTIONAL
); 

NTSTATUS WINAPI RtlGetUserPreferredUILanguages(DWORD dwFlags, 
											   BOOL verification,
											   PULONG pulNumLanguages, 
											   PZZWSTR pwszLanguagesBuffer, 
											   PULONG pcchLanguagesBuffer);

VOID NTAPI TpDisassociateCallback(__inout PTP_CALLBACK_INSTANCE Instance); 

VOID NTAPI 	TpStartAsyncIoOperation(__inout PTP_IO Io);

VOID NTAPI 	TpCancelAsyncIoOperation (__inout PTP_IO Io);

VOID WINAPI RtlDeleteBoundaryDescriptor(_In_  HANDLE BoundaryDescriptor);

VOID NTAPI 	TpCallbackUnloadDllOnCompletion (__inout PTP_CALLBACK_INSTANCE Instance, __in PVOID DllHandle);

VOID WINAPI RtlAcquireSRWLockExclusive(IN OUT PRTL_SRWLOCK SRWLock);	

VOID NTAPI RtlReleaseSRWLockExclusive(IN OUT PRTL_SRWLOCK SRWLock);

VOID NTAPI TpCallbackReleaseMutexOnCompletion( 
		__inout PTP_CALLBACK_INSTANCE  	Instance,
		__in HANDLE  	Mutex 
); 

VOID NTAPI TpCallbackReleaseSemaphoreOnCompletion( 	
		__inout PTP_CALLBACK_INSTANCE  	Instance,
		__in HANDLE  	Semaphore,
		__in LONG  	ReleaseCount 
) ;

NTSTATUS WINAPI RtlAddSIDToBoundaryDescriptor(
  _Inout_  HANDLE *BoundaryDescriptor,
  _In_     PSID RequiredSid
);

NTSTATUS NTAPI NtCreatePrivateNamespace(
  __out PHANDLE NamespaceHandle,
  __in ACCESS_MASK DesiredAccess,
  __in_opt POBJECT_ATTRIBUTES ObjectAttributes,
  __in PVOID BoundaryDescriptor
);

NTSTATUS NTAPI
NtRemoveIoCompletionEx(
    __in HANDLE IoCompletionHandle,
    __out FILE_IO_COMPLETION_INFORMATION IoCompletionInformation,
    __in ULONG Count,
    __out PVOID NumEntriesRemoved,
    __in_opt PLARGE_INTEGER Timeout,
    __in BOOLEAN Alertable
);

NTSTATUS NTAPI
NtOpenPrivateNamespace(
  __out PHANDLE NamespaceHandle,
  __in ACCESS_MASK DesiredAccess,
  __in_opt POBJECT_ATTRIBUTES ObjectAttributes,
  __in PVOID BoundaryDescriptor
);

NTSTATUS NTAPI 	NtReplacePartitionUnit(__in PUNICODE_STRING TargetInstancePath, 
										__in PUNICODE_STRING SpareInstancePath,
										__in ULONG Flags);
								
NTSTATUS WINAPI TpSetPoolStackInformation(
  _Inout_  PTP_POOL ptpp,
  _In_     PTP_POOL_STACK_INFORMATION ptpsi
);

NTSTATUS NTAPI 	TpQueryPoolStackInformation (
  __in PTP_POOL Pool, 
  __out PTP_POOL_STACK_INFORMATION PoolStackInformation
);

NTSTATUS NTAPI RtlCheckTokenCapability(
  _In_opt_  HANDLE TokenHandle,
  _In_      PSID CapabilitySidToCheck,
  _Out_     PBOOL HasCapability
);

NTSTATUS NTAPI RtlCheckTokenMembershipEx(
	HANDLE TokenHandle, 
	PSID SidToCheck, 
	DWORD Flas, 
	PBOOL IsMember
);

NTSTATUS NTAPI NtSetInformationVirtualMemory(HANDLE hProcess,
											 BOOL access,
											 ULONG_PTR NumberOfEntries, 
											 PWIN32_MEMORY_RANGE_ENTRY VirtualAddresses, 
											 PULONG Flags,
											 DWORD other
);

PVOID NTAPI LdrResolveDelayLoadedAPI(
  _In_        PVOID ParentModuleBase,
  _In_        PCIMAGE_DELAYLOAD_DESCRIPTOR DelayloadDescriptor,
  _In_opt_    PDELAYLOAD_FAILURE_DLL_CALLBACK FailureDllHook,
  _In_opt_    PDELAYLOAD_FAILURE_SYSTEM_ROUTINE FailureSystemHook,
  _Out_       PIMAGE_THUNK_DATA ThunkAddress,
  _Reserved_  ULONG Flags
);

NTSTATUS NTAPI RtlAddResourceAttributeAce(PACL pAcl, 
										  DWORD dwAceRevision, 
										  DWORD AceFlags, 
										  DWORD AccessMask, 
										  PSID pSid, 
										  PCLAIM_SECURITY_ATTRIBUTES_INFORMATION  pAttributeInfo, 
										  PDWORD pReturnLength
);

NTSTATUS WINAPI LdrResolveDelayLoadsFromDll(
  _In_        PVOID ParentBase,
  _In_        LPCSTR TargetDllName,
  _Reserved_  ULONG Flags
);

PTEB NtCurrentTeb(void);

NTSTATUS NTAPI RtlProcessFlsData(ULONG Buffer);

NTSTATUS WINAPI NtUnmapViewOfSectionEx(HANDLE handle, PVOID MemoryCache, ULONG number);

NTSTATUS NTAPI RtlQueryThreadProfiling(HANDLE HANDLE, PBOOLEAN Enabled);

NTSTATUS NTAPI RtlCopyContext(PCONTEXT Destination, DWORD ContextFlags, PCONTEXT Source);

NTSTATUS NTAPI RtlDisableThreadProfiling(HANDLE PerformanceDataHandle);

NTSTATUS NTAPI NtDeletePrivateNamespace(__in HANDLE NamespaceHandle);

NTSTATUS NTAPI RtlIsNormalizedString(NORM_FORM NormForm, LPCWSTR lpString, int cwLength, LPCWSTR other);

NTSTATUS NTAPI RtlNormalizeString(_In_       NORM_FORM NormForm,  _In_ LPCWSTR lpSrcString,
  _In_       int cwSrcLength,  _Out_opt_  LPWSTR lpDstString,  _In_  int *cwDstLength);

BOOL WINAPI RtlDeregisterSecureMemoryCacheCallback(_In_  PSECURE_MEMORY_CACHE_CALLBACK pfnCallBack);

NTSYSAPI NTSTATUS WINAPI RtlRegisterSecureMemoryCacheCallback(_In_  PRTL_SECURE_MEMORY_CACHE_CALLBACK pfnCallBack);

NTSTATUS NTAPI RtlEnableThreadProfiling(HANDLE ThreadHandle, DWORD Flags, DWORD64 HardwareCounters, HANDLE PerformanceDataHandle);

NTSTATUS NTAPI RtlAddIntegrityLabelToBoundaryDescriptor(HANDLE *BoundaryDescriptor, PSID IntegrityLabel);

VOID NTAPI RtlWakeConditionVariable(IN OUT PRTL_CONDITION_VARIABLE ConditionVariable); 

VOID NTAPI RtlGetCurrentProcessorNumberEx(_Out_  PPROCESSOR_NUMBER ProcNumber);

PCONTEXT NTAPI RtlLocateLegacyContext(PCONTEXT oldContext, BOOL other);

NTSTATUS NTAPI RtlInitializeExtendedContext(PVOID Buffer, DWORD flags, PCONTEXT *oldContext);

NTSTATUS NTAPI RtlGetExtendedContextLength(DWORD flags, LPDWORD ContextFlags);

ULONG64 NTAPI RtlGetEnabledExtendedFeatures(_In_  ULONG64 FeatureMask);

PVOID NTAPI RtlLocateExtendedFeature(BYTE reception, DWORD FeatureId, PDWORD Length);

VOID NTAPI RtlSetExtendedFeaturesMask(PCONTEXT Context, DWORD64 FeatureMask);

VOID NTAPI 	TpCallbackSetEventOnCompletion (__inout PTP_CALLBACK_INSTANCE Instance, __in HANDLE Event);

BOOL NTAPI WinSqmIsOptedInEx(ULONG number);

HANDLE WINAPI RtlCreateBoundaryDescriptor(LSA_UNICODE_STRING *string, ULONG Flags);	

NTSTATUS NTAPI RtlUnsubscribeWnfStateChangeNotification(PVOID RegistrationHandle);

DWORD NTAPI RtlGetSystemTimePrecise();

PDWORD64 WINAPI RtlGetExtendedFeaturesMask(UCHAR *contextLocal);

