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
 
/* We're a core NT DLL, we don't import syscalls */
#define _NTSYSTEM_
#define _NTDLLBUILD_ 

#include <assert.h>
#include <rtl.h>
#include <ntsecapi.h>
#include <cmfuncs.h>
#include "localelcid.h"
#include <rtlavl.h>
#include <rtltypes.h>
#include <winnls.h>
#include <stdarg.h>
#include <limits.h>
#include <list.h>
#include <wmistr.h>
#include <lpcfuncs.h>
#include <ntdllbase.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* Use intrinsics for x86 and x64 */
#if defined(_M_IX86) || defined(_M_AMD64)
#define InterlockedCompareExchange _InterlockedCompareExchange
#define InterlockedIncrement _InterlockedIncrement
#define InterlockedDecrement _InterlockedDecrement
#define InterlockedExchangeAdd _InterlockedExchangeAdd
#define InterlockedExchange _InterlockedExchange
#define InterlockedBitTestAndSet _interlockedbittestandset
#define InterlockedBitTestAndSet64 _interlockedbittestandset64
#endif
#define IS_PATH_SEPARATOR(x) (((x)==L'\\')||((x)==L'/'))

#define MAX_HW_COUNTERS 16
typedef __int64 timeout_t;
#define TIMEOUT_INFINITE (((timeout_t)0x7fffffff) << 32 | 0xffffffff)

#define RTL_ATOM_MAXIMUM_INTEGER_ATOM   (RTL_ATOM)0xC000
#define RTL_ATOM_INVALID_ATOM           (RTL_ATOM)0x0000
#define RTL_ATOM_TABLE_DEFAULT_NUMBER_OF_BUCKETS 37
#define RTL_ATOM_MAXIMUM_NAME_LENGTH    255
#define RTL_ATOM_PINNED 0x01

#if defined(__i386__) || defined(__x86_64__) || defined(__arm__) || defined(__aarch64__)
static const UINT_PTR page_size = 0x1000;
#else
extern UINT_PTR page_size DECLSPEC_HIDDEN;
#endif

/* Definitions *****************************************/

typedef PVOID* PALPC_HANDLE;

typedef PVOID ALPC_HANDLE;

typedef DWORD WINAPI RTL_RUN_ONCE_INIT_FN(PRTL_RUN_ONCE, PVOID, PVOID*);

typedef RTL_RUN_ONCE_INIT_FN *PRTL_RUN_ONCE_INIT_FN;

static RTL_CRITICAL_SECTION loader_section;

/* IDN defines. - From wine's winnls.h*/
#define IDN_ALLOW_UNASSIGNED        0x1
#define IDN_USE_STD3_ASCII_RULES    0x2

/* SHA1 Helper Macros */

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))
/* FIXME: This definition of DWORD2BE is little endian specific! */
#define DWORD2BE(x) (((x) >> 24) & 0xff) | (((x) >> 8) & 0xff00) | (((x) << 8) & 0xff0000) | (((x) << 24) & 0xff000000);
/* FIXME: This definition of blk0 is little endian specific! */
#define blk0(i) (Block[i] = (rol(Block[i],24)&0xFF00FF00)|(rol(Block[i],8)&0x00FF00FF))
#define blk1(i) (Block[i&15] = rol(Block[(i+13)&15]^Block[(i+8)&15]^Block[(i+2)&15]^Block[i&15],1))
#define f1(x,y,z) (z^(x&(y^z)))
#define f2(x,y,z) (x^y^z)
#define f3(x,y,z) ((x&y)|(z&(x|y)))
#define f4(x,y,z) (x^y^z)
/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z+=f1(w,x,y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=f1(w,x,y)+blk1(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=f2(w,x,y)+blk1(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=f3(w,x,y)+blk1(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=f4(w,x,y)+blk1(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

// this is Windows 2000 code
#ifndef ARGUMENT_PRESENT
#define ARGUMENT_PRESENT(x) (((x) != NULL))
#endif

/* This is Windows 2000 code*/
#define 	RtlProcessHeap()   (NtCurrentPeb()->ProcessHeap)

#define STATUS_INCOMPATIBLE_DRIVER_BLOCKED 0xC0000424

#define ACTIVATION_CONTEXT_SECTION_APPLICATION_SETTINGS          10
#define ACTIVATION_CONTEXT_SECTION_COMPATIBILITY_INFO            11

#define RtlGetCurrentThreadId()  (HandleToUlong(NtCurrentTeb()->ClientId.UniqueThread))

#define LOAD_LIBRARY_REQUIRE_SIGNED_TARGET  0x00000080

/* flags for SetSearchPathMode */
#define BASE_SEARCH_PATH_ENABLE_SAFE_SEARCHMODE  0x00001
#define BASE_SEARCH_PATH_DISABLE_SAFE_SEARCHMODE 0x10000
#define BASE_SEARCH_PATH_PERMANENT               0x08000
#define BASE_SEARCH_PATH_INVALID_FLAGS         (~0x18001)

//Because we want don't touch in THREADINFOCLASS on Reactos 
#define ThreadGroupInformation 30

/* Enumarations ****************************************/

/* Structs *********************************************/

typedef struct _LOCALE{
	LPCWSTR description;
	LPCWSTR cultureName;
	LCID lcidHex;
	LCID lcidDec;	
}LOCALE,*PLOCALE;

typedef struct {
   ULONG Unknown[6];
   ULONG State[5];
   ULONG Count[2];
   UCHAR Buffer[64];
} SHA_CTX, *PSHA_CTX;

/* Internal header for table entries */
typedef struct _TABLE_ENTRY_HEADER
{
	RTL_SPLAY_LINKS SplayLinks;
    RTL_BALANCED_LINKS BalancedLinks;
	LIST_ENTRY ListEntry;
    LONGLONG UserData;
} TABLE_ENTRY_HEADER, *PTABLE_ENTRY_HEADER;

typedef enum _RTL_AVL_BALANCE_FACTOR
{
    RtlUnbalancedAvlTree = -2,
    RtlLeftHeavyAvlTree,
    RtlBalancedAvlTree,
    RtlRightHeavyAvlTree,
} RTL_AVL_BALANCE_FACTOR;

typedef struct _RTLP_SRWLOCK_SHARED_WAKE
{
    LONG Wake;
    volatile struct _RTLP_SRWLOCK_SHARED_WAKE *Next;
} volatile RTLP_SRWLOCK_SHARED_WAKE, *PRTLP_SRWLOCK_SHARED_WAKE;

typedef struct _TP_IO TP_IO, *PTP_IO;

typedef ULONG LOGICAL;

typedef DWORD TP_VERSION, *PTP_VERSION;

typedef DWORD TP_WAIT_RESULT;

typedef PVOID* PALPC_HANDLE;

typedef PVOID ALPC_HANDLE;

typedef HANDLE 	DLL_DIRECTORY_COOKIE;

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

// typedef PVOID (WINAPI *PDELAYLOAD_FAILURE_SYSTEM_ROUTINE)(LPCSTR, LPCSTR);

typedef VOID (CALLBACK *PTP_WIN32_IO_CALLBACK)(PTP_CALLBACK_INSTANCE,PVOID,PVOID,ULONG,ULONG_PTR,PTP_IO);

// typedef enum _NORM_FORM { 
  // NormalizationOther  = 0,
  // NormalizationC      = 0x1,
  // NormalizationD      = 0x2,
  // NormalizationKC     = 0x5,
  // NormalizationKD     = 0x6
// } NORM_FORM;

typedef enum _TRANSACTION_INFORMATION_CLASS { 
  TransactionBasicInformation               = 0,
  TransactionPropertiesInformation,
  TransactionEnlistmentInformation,
  TransactionSuperiorEnlistmentInformation
} TRANSACTION_INFORMATION_CLASS;

// typedef struct _WIN32_MEMORY_RANGE_ENTRY {
  // PVOID  VirtualAddress;
  // SIZE_T NumberOfBytes;
// } WIN32_MEMORY_RANGE_ENTRY, *PWIN32_MEMORY_RANGE_ENTRY;

// typedef struct _CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE {
  // PVOID pValue;
  // DWORD ValueLength;
// } CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE, *PCLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE;

// typedef struct _CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE {
  // DWORD64 Version;
  // PWSTR   Name;
// } CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE, *PCLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE;

// typedef struct _CLAIM_SECURITY_ATTRIBUTE_V1 {
  // PWSTR Name;
  // WORD  ValueType;
  // WORD  Reserved;
  // DWORD Flags;
  // DWORD ValueCount;
  // union {
    // PLONG64                                      pInt64;
    // PDWORD64                                     pUint64;
    // PWSTR                                        *ppString;
    // PCLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE         pFqbn;
    // PCLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE pOctetString;
  // } Values;
// } CLAIM_SECURITY_ATTRIBUTE_V1, *PCLAIM_SECURITY_ATTRIBUTE_V1;

// typedef struct _CLAIM_SECURITY_ATTRIBUTES_INFORMATION {
  // WORD  Version;
  // WORD  Reserved;
  // DWORD AttributeCount;
  // union {
    // PCLAIM_SECURITY_ATTRIBUTE_V1 pAttributeV1;
  // } Attribute;
// } CLAIM_SECURITY_ATTRIBUTES_INFORMATION, *PCLAIM_SECURITY_ATTRIBUTES_INFORMATION;

// typedef struct _TP_POOL_STACK_INFORMATION
// {
   // SIZE_T StackReserve;
   // SIZE_T StackCommit;
// } TP_POOL_STACK_INFORMATION,*PTP_POOL_STACK_INFORMATION;

// typedef enum  { 
  // PMCCounter,
  // MaxHardwareCounterType
// } HARDWARE_COUNTER_TYPE;

// typedef struct _HARDWARE_COUNTER_DATA {
  // HARDWARE_COUNTER_TYPE Type;
  // DWORD                 Reserved;
  // DWORD64               Value;
// } HARDWARE_COUNTER_DATA, *PHARDWARE_COUNTER_DATA;

// typedef struct _PERFORMANCE_DATA {
  // WORD                  Size;
  // BYTE                  Version;
  // BYTE                  HwCountersCount;
  // DWORD                 ContextSwitchCount;
  // DWORD64               WaitReasonBitMap;
  // DWORD64               CycleTime;
  // DWORD                 RetryCount;
  // DWORD                 Reserved;
  // HARDWARE_COUNTER_DATA HwCounters[MAX_HW_COUNTERS];
// } PERFORMANCE_DATA, *PPERFORMANCE_DATA;

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

// typedef struct _DELAYLOAD_PROC_DESCRIPTOR
// {
     // ULONG ImportDescribedByName;
     // union {
         // LPCSTR Name;
         // ULONG Ordinal;
     // } Description;
// } DELAYLOAD_PROC_DESCRIPTOR, *PDELAYLOAD_PROC_DESCRIPTOR;

// typedef struct _DELAYLOAD_INFO
// {
     // ULONG Size;
     // PCIMAGE_DELAYLOAD_DESCRIPTOR DelayloadDescriptor;
     // PIMAGE_THUNK_DATA ThunkAddress;
     // LPCSTR TargetDllName;
     // DELAYLOAD_PROC_DESCRIPTOR TargetApiDescriptor;
     // PVOID TargetModuleBase;
     // PVOID Unused;
     // ULONG LastError;
// } DELAYLOAD_INFO, *PDELAYLOAD_INFO;
// typedef PVOID (WINAPI *PDELAYLOAD_FAILURE_DLL_CALLBACK)(ULONG, PDELAYLOAD_INFO);

//
// This structure specifies an offset (from the beginning of CONTEXT_EX
// structure) and size of a single chunk of an extended context structure.
//
// N.B. Offset may be negative.
//

typedef struct _CONTEXT_CHUNK {
    LONG Offset;
    DWORD Length;
} CONTEXT_CHUNK, *PCONTEXT_CHUNK;

//
// CONTEXT_EX structure is an extension to CONTEXT structure. It defines
// a context record as a set of disjoint variable-sized buffers (chunks)
// each containing a portion of processor state. Currently there are only
// two buffers (chunks) are defined:
//
//   - Legacy, that stores traditional CONTEXT structure;
//   - XState, that stores XSAVE save area buffer starting from
//     XSAVE_AREA_HEADER, i.e. without the first 512 bytes.
//
// There a few assumptions exists that simplify conversion of PCONTEXT
// pointer to PCONTEXT_EX pointer.
//
// 1. APIs that work with PCONTEXT pointers assume that CONTEXT_EX is
//    stored right after the CONTEXT structure. It is also assumed that
//    CONTEXT_EX is present if and only if corresponding CONTEXT_XXX
//    flags are set in CONTEXT.ContextFlags.
//
// 2. CONTEXT_EX.Legacy is always present if CONTEXT_EX structure is
//    present. All other chunks are optional.
//
// 3. CONTEXT.ContextFlags unambigiously define which chunks are
//    present. I.e. if CONTEXT_XSTATE is set CONTEXT_EX.XState is valid.
//

typedef struct _CONTEXT_EX {

    //
    // The total length of the structure starting from the chunk with
    // the smallest offset. N.B. that the offset may be negative.
    //

    CONTEXT_CHUNK All;

    //
    // Wrapper for the traditional CONTEXT structure. N.B. the size of
    // the chunk may be less than sizeof(CONTEXT) is some cases (when
    // CONTEXT_EXTENDED_REGISTERS is not set on x86 for instance).
    //

    CONTEXT_CHUNK Legacy;

    //
    // CONTEXT_XSTATE: Extended processor state chunk. The state is
    // stored in the same format XSAVE operation strores it with
    // exception of the first 512 bytes, i.e. staring from
    // XSAVE_AREA_HEADER. The lower two bits corresponding FP and
    // SSE state must be zero.
    //

    CONTEXT_CHUNK XState;

} CONTEXT_EX, *PCONTEXT_EX;

//Windows 2000 or for new ntdll
typedef struct _SCOPETABLE_ENTRY *PSCOPETABLE_ENTRY;

typedef struct _EH3_EXCEPTION_REGISTRATION
{
  struct _EH3_EXCEPTION_REGISTRATION *Next;
  PVOID ExceptionHandler;
  PSCOPETABLE_ENTRY ScopeTable;
  DWORD TryLevel;
}EH3_EXCEPTION_REGISTRATION, *PEH3_EXCEPTION_REGISTRATION;

typedef struct _CPPEH_RECORD
{
  DWORD old_esp;
  EXCEPTION_POINTERS *exc_ptr;
  PEH3_EXCEPTION_REGISTRATION registration;
}CPPEH_RECORD;

typedef struct _RTLP_SRWLOCK_WAITBLOCK
{
    /* SharedCount is the number of shared acquirers. */
    LONG SharedCount;

    /* Last points to the last wait block in the chain. The value
       is only valid when read from the first wait block. */
    volatile struct _RTLP_SRWLOCK_WAITBLOCK *Last;

    /* Next points to the next wait block in the chain. */
    volatile struct _RTLP_SRWLOCK_WAITBLOCK *Next;

    union
    {
        /* Wake is only valid for exclusive wait blocks */
        LONG Wake;
        /* The wake chain is only valid for shared wait blocks */
        struct
        {
            PRTLP_SRWLOCK_SHARED_WAKE SharedWakeChain;
            PRTLP_SRWLOCK_SHARED_WAKE LastSharedWake;
        };
    };

    BOOLEAN Exclusive;
} volatile RTLP_SRWLOCK_WAITBLOCK, *PRTLP_SRWLOCK_WAITBLOCK;

/* Functions Prototypes for public*/
void 
WINAPI 
A_SHAInit(PSHA_CTX Context);

VOID 
WINAPI 
A_SHAUpdate(PSHA_CTX Context,
	const unsigned char *  	Buffer,
	UINT  	BufferSize 
);  

VOID 
WINAPI 
A_SHAFinal(
	PSHA_CTX Context, 
	PULONG Result) ;
	
PIMAGE_SECTION_HEADER
RtlSectionTableFromVirtualAddress (
    IN PIMAGE_NT_HEADERS NtHeaders,
    IN PVOID Base,
    IN ULONG Address
    );
	
PVOID
NTAPI
RtlAddressInSectionTable (
    IN PIMAGE_NT_HEADERS NtHeaders,
    IN PVOID Base,
    IN ULONG Address
    );
	
NTSTATUS 
WINAPI 
RtlLCIDToCultureName(
	IN LCID lcid, 
	OUT PUNICODE_STRING locale
);

void 
NTAPI 
RtlReportErrorPropagation(
	LPSTR a1, 
	int a2, 
	int a3, 
	int a4
);

void 
NTAPI 
RtlReportErrorOrigination(
	LPSTR a1, 
	WCHAR string, 
	int a3, 
	NTSTATUS status
);

//
// Define a RESID
//

typedef PVOID RESID;

//
// Define a RESOURCE_HANDLE
//

typedef HANDLE   RESOURCE_HANDLE;

typedef enum CLUSTER_RESOURCE_STATE { 
  ClusterResourceStateUnknown    = -1,
  ClusterResourceInherited       = 0,
  ClusterResourceInitializing    = 1,
  ClusterResourceOnline          = 2,
  ClusterResourceOffline         = 3,
  ClusterResourceFailed          = 4,
  ClusterResourcePending         = 128, // 0x80
  ClusterResourceOnlinePending   = 129, // 0x81
  ClusterResourceOfflinePending  = 130 // 0x82
} CLUSTER_RESOURCE_STATE, _CLUSTER_RESOURCE_STATE;

//
// Define Resource DLL callback method for logging events.
//
typedef enum LOG_LEVEL {
    LOG_INFORMATION,
    LOG_WARNING,
    LOG_ERROR,
    LOG_SEVERE
} LOG_LEVEL, *PLOG_LEVEL;

typedef struct _RESOURCE_STATUS {
  CLUSTER_RESOURCE_STATE ResourceState;
  DWORD                  CheckPoint;
  DWORD                  WaitHint;
  HANDLE                 EventHandle;
} RESOURCE_STATUS, *PRESOURCE_STATUS;

typedef struct _LOCALE_LCID{
	LPCWSTR localeName;
	LCID lcid;
}LOCALE_LCID;

typedef DWORD (WINAPI *PSET_RESOURCE_STATUS_ROUTINE)(
    _In_ RESOURCE_HANDLE  ResourceHandle,
    _In_ PRESOURCE_STATUS ResourceStatus
);

//
// Define Resource DLL callback method for notifying that a quorum
// resource DLL failed to hold the quorum resource.
//
typedef
VOID
(_stdcall *PQUORUM_RESOURCE_LOST) (
    IN RESOURCE_HANDLE Resource
    );

typedef VOID (WINAPI *PLOG_EVENT_ROUTINE)(
    _In_ RESOURCE_HANDLE ResourceHandle,
    _In_ LOG_LEVEL       LogLevel,
    _In_ LPCWSTR         FormatString,
                         ...
);

typedef
DWORD
(_stdcall *PRESOURCE_TYPE_CONTROL_ROUTINE) (
    IN LPCWSTR ResourceTypeName,
    IN DWORD ControlCode,
    IN PVOID InBuffer,
    IN DWORD InBufferSize,
    OUT PVOID OutBuffer,
    IN DWORD OutBufferSize,
    OUT LPDWORD BytesReturned
    );
	
typedef
DWORD
(_stdcall *PRESOURCE_CONTROL_ROUTINE) (
    IN RESID Resource,
    IN DWORD ControlCode,
    IN PVOID InBuffer,
    IN DWORD InBufferSize,
    OUT PVOID OutBuffer,
    IN DWORD OutBufferSize,
    OUT LPDWORD BytesReturned
    );
	
typedef
DWORD
(_stdcall *PRELEASE_ROUTINE) (
    IN RESID Resource
    );	
	
typedef
DWORD
(_stdcall *PARBITRATE_ROUTINE) (
    IN RESID Resource,
    IN PQUORUM_RESOURCE_LOST LostQuorumResource
    );	
	
typedef
BOOL
(_stdcall *PIS_ALIVE_ROUTINE) (
    IN RESID Resource
    );	
	
typedef
BOOL
(_stdcall *PLOOKS_ALIVE_ROUTINE) (
    IN RESID Resource
    );	
	
typedef
VOID
(_stdcall *PTERMINATE_ROUTINE) (
    IN RESID Resource
    );	

typedef
DWORD
(_stdcall *POFFLINE_ROUTINE) (
    IN RESID Resource
    );	
	
typedef
DWORD
(_stdcall *PONLINE_ROUTINE) (
    IN RESID Resource,
    IN OUT LPHANDLE EventHandle
    );	
	
typedef
VOID
(_stdcall *PCLOSE_ROUTINE) (
    IN RESID Resource
    );	
	
typedef
RESID
(_stdcall *POPEN_ROUTINE) (
    IN LPCWSTR ResourceName,
    IN HKEY ResourceKey,
    IN RESOURCE_HANDLE ResourceHandle
    );	
	
//***************************************************************
//
// Define the Function Table Format
//
//***************************************************************

//
// Version 1 Resource DLL Interface definition
//
typedef struct CLRES_V1_FUNCTIONS {
    POPEN_ROUTINE Open;
    PCLOSE_ROUTINE Close;
    PONLINE_ROUTINE Online;
    POFFLINE_ROUTINE Offline;
    PTERMINATE_ROUTINE Terminate;
    PLOOKS_ALIVE_ROUTINE LooksAlive;
    PIS_ALIVE_ROUTINE IsAlive;
    PARBITRATE_ROUTINE Arbitrate;
    PRELEASE_ROUTINE Release;
    PRESOURCE_CONTROL_ROUTINE ResourceControl;
    PRESOURCE_TYPE_CONTROL_ROUTINE ResourceTypeControl;
} CLRES_V1_FUNCTIONS, *PCLRES_V1_FUNCTIONS;

typedef struct CLRES_FUNCTION_TABLE {
    DWORD   TableSize;
    DWORD   Version;
    union {
        CLRES_V1_FUNCTIONS V1Functions;
    };
} CLRES_FUNCTION_TABLE, *PCLRES_FUNCTION_TABLE;

typedef DWORD (WINAPI *PSTARTUP_ROUTINE)(
    _In_  LPCWSTR                      ResourceType,
    _In_  DWORD                        MinVersionSupported,
    _In_  DWORD                        MaxVersionSupported,
    _In_  PSET_RESOURCE_STATUS_ROUTINE SetResourceStatus,
    _In_  PLOG_EVENT_ROUTINE           LogEvent,
    _Out_ CLRES_FUNCTION_TABLE         *FunctionTable
);

typedef DWORD (CALLBACK *PRTL_WORK_ITEM_ROUTINE)(LPVOID); /* FIXME: not the right name */
typedef void (NTAPI *RTL_WAITORTIMERCALLBACKFUNC)(PVOID,BOOLEAN); /* FIXME: not the right name */
typedef VOID (CALLBACK *PRTL_OVERLAPPED_COMPLETION_ROUTINE)(DWORD,DWORD,LPVOID);

// typedef struct _RTL_CONDITION_VARIABLE {
    // PVOID Ptr;
// } RTL_CONDITION_VARIABLE, *PRTL_CONDITION_VARIABLE;

NTSTATUS
NTAPI
RtlSleepConditionVariableCS(IN OUT PRTL_CONDITION_VARIABLE ConditionVariable,
                            IN OUT PRTL_CRITICAL_SECTION CriticalSection,
                            IN const LARGE_INTEGER * TimeOut OPTIONAL); 
 
void 
WINAPI
RtlWakeConditionVariable(
  RTL_CONDITION_VARIABLE* variable
);

void 
WINAPI 
RtlInitializeConditionVariable(
	PRTL_CONDITION_VARIABLE
);

void
WINAPI 
RtlWakeAllConditionVariable(
	PRTL_CONDITION_VARIABLE
);

#define WOW64_TLS_FILESYSREDIR      8

typedef struct _WMI_LOGGER_INFORMATION {
    WNODE_HEADER Wnode;       // Had to do this since wmium.h comes later
//
// data provider by caller
    ULONG BufferSize;                   // buffer size for logging (in kbytes)
    ULONG MinimumBuffers;               // minimum to preallocate
    ULONG MaximumBuffers;               // maximum buffers allowed
    ULONG MaximumFileSize;              // maximum logfile size (in MBytes)
    ULONG LogFileMode;                  // sequential, circular
    ULONG FlushTimer;                   // buffer flush timer, in seconds
    ULONG EnableFlags;                  // trace enable flags
    LONG  AgeLimit;                     // aging decay time, in minutes
    ULONG Wow;                          // TRUE if the logger started under WOW64
    union {
        HANDLE  LogFileHandle;          // handle to logfile
        ULONG64 LogFileHandle64;
    };

// data returned to caller
// end_wmikm
    union {
// begin_wmikm
        ULONG NumberOfBuffers;          // no of buffers in use
// end_wmikm
        ULONG InstanceCount;            // Number of Provider Instances
    };
    union {
// begin_wmikm
        ULONG FreeBuffers;              // no of buffers free
// end_wmikm
        ULONG InstanceId;               // Current Provider's Id for UmLogger
    };
    union {
// begin_wmikm
        ULONG EventsLost;               // event records lost
// end_wmikm
        ULONG NumberOfProcessors;       // Passed on to UmLogger
    };
// begin_wmikm
    ULONG BuffersWritten;               // no of buffers written to file
    ULONG LogBuffersLost;               // no of logfile write failures
    ULONG RealTimeBuffersLost;          // no of rt delivery failures
    union {
        HANDLE  LoggerThreadId;         // thread id of Logger
        ULONG64 LoggerThreadId64;       // thread is of Logger
    };
    union {
        UNICODE_STRING LogFileName;     // used only in WIN64
        UNICODE_STRING64 LogFileName64; // Logfile name: only in WIN32
    };

// mandatory data provided by caller
    union {
        UNICODE_STRING LoggerName;      // Logger instance name in WIN64
        UNICODE_STRING64 LoggerName64;  // Logger Instance name in WIN32
    };

// private
    union {
        PVOID   Checksum;
        ULONG64 Checksum64;
    };
    union {
        PVOID   LoggerExtension;
        ULONG64 LoggerExtension64;
    };
} WMI_LOGGER_INFORMATION, *PWMI_LOGGER_INFORMATION;

// typedef struct _TIME_DYNAMIC_ZONE_INFORMATION
// {
     // LONG Bias;
     // WCHAR StandardName[32];
     // SYSTEMTIME StandardDate;
     // LONG StandardBias;
     // WCHAR DaylightName[32];
     // SYSTEMTIME DaylightDate;
     // LONG DaylightBias;
     // WCHAR TimeZoneKeyName[128];
     // BOOLEAN DynamicDaylightTimeDisabled;
// } DYNAMIC_TIME_ZONE_INFORMATION, *PDYNAMIC_TIME_ZONE_INFORMATION;

typedef DYNAMIC_TIME_ZONE_INFORMATION RTL_DYNAMIC_TIME_ZONE_INFORMATION;

/*
 * RTL_SYSTEM_TIME and RTL_TIME_ZONE_INFORMATION are the same as
 * the SYSTEMTIME and TIME_ZONE_INFORMATION structures defined
 * in winbase.h, however we need to define them separately so
 * winternl.h doesn't depend on winbase.h.  They are used by
 * RtlQueryTimeZoneInformation and RtlSetTimeZoneInformation.
 * The names are guessed; if anybody knows the real names, let me know.
 */
typedef struct _RTL_SYSTEM_TIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} RTL_SYSTEM_TIME, *PRTL_SYSTEM_TIME;

typedef struct _ASSEMBLY_STORAGE_MAP_ENTRY
{
    ULONG Flags;
    UNICODE_STRING DosPath;
    HANDLE Handle;
} ASSEMBLY_STORAGE_MAP_ENTRY, *PASSEMBLY_STORAGE_MAP_ENTRY;

typedef struct _ASSEMBLY_STORAGE_MAP
{
    ULONG Flags;
    ULONG AssemblyCount;
    PASSEMBLY_STORAGE_MAP_ENTRY *AssemblyArray;
} ASSEMBLY_STORAGE_MAP, *PASSEMBLY_STORAGE_MAP;

 struct file_info
{
     ULONG               type;
     WCHAR              *info;
};

struct assembly_version
{
    USHORT              major;
    USHORT              minor;
    USHORT              build;
    USHORT              revision;
};

struct assembly_identity
{
    WCHAR                *name;
    WCHAR                *arch;
    WCHAR                *public_key;
    WCHAR                *language;
    WCHAR                *type;
    struct assembly_version version;
    BOOL                  optional;
    BOOL                  delayed;
};

struct progids
{
    WCHAR        **progids;
    unsigned int   num;
    unsigned int   allocated;
};

struct entity
{
    DWORD kind;
    union
    {
        struct
        {
            WCHAR *tlbid;
            WCHAR *helpdir;
            WORD   flags;
            WORD   major;
            WORD   minor;
	} typelib;
        struct
        {
            WCHAR *clsid;
            WCHAR *tlbid;
            WCHAR *progid;
            WCHAR *name;    /* clrClass: class name */
            WCHAR *version; /* clrClass: CLR runtime version */
            DWORD  model;
            DWORD  miscstatus;
            DWORD  miscstatuscontent;
            DWORD  miscstatusthumbnail;
            DWORD  miscstatusicon;
            DWORD  miscstatusdocprint;
            struct progids progids;
	} comclass;
	struct {
            WCHAR *iid;
            WCHAR *base;
            WCHAR *tlib;
            WCHAR *name;
            WCHAR *ps32; /* only stored for 'comInterfaceExternalProxyStub' */
            DWORD  mask;
            ULONG  nummethods;
	} ifaceps;
        struct
        {
            WCHAR *name;
            BOOL   versioned;
        } class;
        struct
        {
            WCHAR *name;
            WCHAR *clsid;
            WCHAR *version;
        } clrsurrogate;
        struct
        {
            WCHAR *name;
            WCHAR *value;
        } settings;
    } u;
};

struct entity_array
{
    struct entity        *base;
    unsigned int          num;
    unsigned int          allocated;
};

/* return type of RtlDetermineDosPathNameType_U (FIXME: not the correct names) */
typedef enum
{
    INVALID_PATH = 0,
    UNC_PATH,              /* "//foo" */
    ABSOLUTE_DRIVE_PATH,   /* "c:/foo" */
    RELATIVE_DRIVE_PATH,   /* "c:foo" */
    ABSOLUTE_PATH,         /* "/foo" */
    RELATIVE_PATH,         /* "foo" */
    DEVICE_PATH,           /* "//./foo" */
    UNC_DOT_PATH           /* "//." */
} DOS_PATHNAME_TYPE;

struct assembly
{
    enum assembly_type             type;
    struct assembly_identity       id;
    struct file_info               manifest;
    WCHAR                         *directory;
    BOOL                           no_inherit;
    struct dll_redirect           *dlls;
    unsigned int                   num_dlls;
    unsigned int                   allocated_dlls;
    struct entity_array            entities;
    COMPATIBILITY_CONTEXT_ELEMENT *compat_contexts;
    ULONG                          num_compat_contexts;
    ACTCTX_REQUESTED_RUN_LEVEL     run_level;
    ULONG                          ui_access;
};

typedef struct _ACTIVATION_CONTEXT
{
	ULONG magic;
    LONG RefCount;
    ULONG Flags;
    LIST_ENTRY Links;
    PACTIVATION_CONTEXT_DATA ActivationContextData;
    PVOID NotificationRoutine;
    PVOID NotificationContext;
    ULONG SentNotifications[8];
    ULONG DisabledNotifications[8];
    ASSEMBLY_STORAGE_MAP StorageMap;
    PASSEMBLY_STORAGE_MAP_ENTRY InlineStorageMapEntries;
    ULONG StackTraceIndex;
    PVOID StackTraces[4][4];
    struct file_info config;
    struct file_info appdir;
    struct assembly *assemblies;
    unsigned int num_assemblies;
    unsigned int allocated_assemblies;
    /* section data */
    DWORD sections;
    struct strsection_header *wndclass_section;
    struct strsection_header *dllredirect_section;
    struct strsection_header *progid_section;
    struct guidsection_header *tlib_section;
    struct guidsection_header *comserver_section;
    struct guidsection_header *ifaceps_section;
    struct guidsection_header *clrsurrogate_section;
} ACTIVATION_CONTEXT, *PIACTIVATION_CONTEXT;

typedef struct _TOKEN_APPCONTAINER_INFORMATION {
  	PSID TokenAppContainer;
} TOKEN_APPCONTAINER_INFORMATION, *PTOKEN_APPCONTAINER_INFORMATION;

typedef struct _LDR_DLL_UNLOADED_NOTIFICATION_DATA
{
    ULONG Flags;
    const UNICODE_STRING *FullDllName;
    const UNICODE_STRING *BaseDllName;
    void *DllBase;
    ULONG SizeOfImage;
} LDR_DLL_UNLOADED_NOTIFICATION_DATA, *PLDR_DLL_UNLOADED_NOTIFICATION_DATA;

typedef union _LDR_DLL_NOTIFICATION_DATA
{
    LDR_DLL_LOADED_NOTIFICATION_DATA Loaded;
    LDR_DLL_UNLOADED_NOTIFICATION_DATA Unloaded;
} LDR_DLL_NOTIFICATION_DATA, *PLDR_DLL_NOTIFICATION_DATA;

typedef void (CALLBACK *PLDR_DLL_NOTIFICATION_FUNCTION)(ULONG, LDR_DLL_NOTIFICATION_DATA*, void*);

typedef struct _TP_CALLBACK_ENVIRON_V3
{
    TP_VERSION Version;
    PTP_POOL Pool;
    PTP_CLEANUP_GROUP CleanupGroup;
    PTP_CLEANUP_GROUP_CANCEL_CALLBACK CleanupGroupCancelCallback;
    PVOID RaceDll;
    struct _ACTIVATION_CONTEXT *ActivationContext;
    PTP_SIMPLE_CALLBACK FinalizationCallback;
    union
    {
        DWORD Flags;
        struct
        {
            DWORD LongFunction:1;
            DWORD Persistent:1;
            DWORD Private:30;
        } s;
    } u;
    TP_CALLBACK_PRIORITY CallbackPriority;
    DWORD Size;
} TP_CALLBACK_ENVIRON_V3;

typedef struct _PROCESS_STACK_ALLOCATION_INFORMATION
{
    SIZE_T ReserveSize;
    SIZE_T ZeroBits;
    PVOID  StackBase;
} PROCESS_STACK_ALLOCATION_INFORMATION, *PPROCESS_STACK_ALLOCATION_INFORMATION;

typedef enum _EVENT_INFO_CLASS {
  EventProviderBinaryTrackInfo,
  EventProviderSetReserved1,
  EventProviderSetTraits,
  EventProviderUseDescriptorType,
  MaxEventInfo
} EVENT_INFO_CLASS;

RTL_CRITICAL_SECTION time_tz_section;

RTL_CRITICAL_SECTION localeCritSection;

HANDLE GlobalKeyedEventHandle;

NTSTATUS
NTAPI
NtAccessCheckByTypeAndAuditAlarm(
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
    _Out_ PBOOLEAN GenerateOnClose);
	
DWORD 
NTAPI 
RtlRunOnceExecuteOnce( 
	RTL_RUN_ONCE *once, 
	PRTL_RUN_ONCE_INIT_FN func,
    void *param, void **context 
);	

/* Threadpool functions */

// NTSYSAPI NTSTATUS  WINAPI TpAllocCleanupGroup(TP_CLEANUP_GROUP **);
// NTSYSAPI NTSTATUS  WINAPI TpAllocIoCompletion(TP_IO **,HANDLE,PTP_IO_CALLBACK,void *,TP_CALLBACK_ENVIRON *);
// NTSYSAPI NTSTATUS  WINAPI TpAllocPool(TP_POOL **,PVOID);
// NTSYSAPI NTSTATUS  WINAPI TpAllocTimer(TP_TIMER **,PTP_TIMER_CALLBACK,PVOID,TP_CALLBACK_ENVIRON *);
// NTSYSAPI NTSTATUS  WINAPI TpAllocWait(TP_WAIT **,PTP_WAIT_CALLBACK,PVOID,TP_CALLBACK_ENVIRON *);
// NTSYSAPI NTSTATUS  WINAPI TpAllocWork(TP_WORK **,PTP_WORK_CALLBACK,PVOID,TP_CALLBACK_ENVIRON *);
// NTSYSAPI void      WINAPI TpCallbackLeaveCriticalSectionOnCompletion(TP_CALLBACK_INSTANCE *,RTL_CRITICAL_SECTION *);
// NTSYSAPI NTSTATUS  WINAPI TpCallbackMayRunLong(TP_CALLBACK_INSTANCE *);
// NTSYSAPI void      WINAPI TpCallbackReleaseMutexOnCompletion(TP_CALLBACK_INSTANCE *,HANDLE);
// NTSYSAPI void      WINAPI TpCallbackReleaseSemaphoreOnCompletion(TP_CALLBACK_INSTANCE *,HANDLE,DWORD);
// NTSYSAPI void      WINAPI TpCallbackSetEventOnCompletion(TP_CALLBACK_INSTANCE *,HANDLE);
// NTSYSAPI void      WINAPI TpCallbackUnloadDllOnCompletion(TP_CALLBACK_INSTANCE *,HMODULE);
// NTSYSAPI void      WINAPI TpCancelAsyncIoOperation(TP_IO *);
// NTSYSAPI void      WINAPI TpDisassociateCallback(TP_CALLBACK_INSTANCE *);
// NTSYSAPI BOOL      WINAPI TpIsTimerSet(TP_TIMER *);
// NTSYSAPI void      WINAPI TpPostWork(TP_WORK *);
// NTSYSAPI NTSTATUS  WINAPI TpQueryPoolStackInformation(TP_POOL *, TP_POOL_STACK_INFORMATION *stack_info);
// NTSYSAPI void      WINAPI TpReleaseCleanupGroup(TP_CLEANUP_GROUP *);
// NTSYSAPI void      WINAPI TpReleaseCleanupGroupMembers(TP_CLEANUP_GROUP *,BOOL,PVOID);
// NTSYSAPI void      WINAPI TpReleaseIoCompletion(TP_IO *);
// NTSYSAPI void      WINAPI TpReleasePool(TP_POOL *);
// NTSYSAPI void      WINAPI TpReleaseTimer(TP_TIMER *);
// NTSYSAPI void      WINAPI TpReleaseWait(TP_WAIT *);
// NTSYSAPI void      WINAPI TpReleaseWork(TP_WORK *);
// NTSYSAPI void      WINAPI TpSetPoolMaxThreads(TP_POOL *,DWORD);
// NTSYSAPI BOOL      WINAPI TpSetPoolMinThreads(TP_POOL *,DWORD);
// NTSYSAPI NTSTATUS  WINAPI TpSetPoolStackInformation(TP_POOL *, TP_POOL_STACK_INFORMATION *stack_info);
// NTSYSAPI void      WINAPI TpSetTimer(TP_TIMER *, LARGE_INTEGER *,LONG,LONG);
// NTSYSAPI void      WINAPI TpSetWait(TP_WAIT *,HANDLE,LARGE_INTEGER *);
// NTSYSAPI NTSTATUS  WINAPI TpSimpleTryPost(PTP_SIMPLE_CALLBACK,PVOID,TP_CALLBACK_ENVIRON *);
// NTSYSAPI void      WINAPI TpStartAsyncIoOperation(TP_IO *);
// NTSYSAPI void      WINAPI TpWaitForIoCompletion(TP_IO *,BOOL);
// NTSYSAPI void      WINAPI TpWaitForTimer(TP_TIMER *,BOOL);
// NTSYSAPI void      WINAPI TpWaitForWait(TP_WAIT *,BOOL);
// NTSYSAPI void      WINAPI TpWaitForWork(TP_WORK *,BOOL);
