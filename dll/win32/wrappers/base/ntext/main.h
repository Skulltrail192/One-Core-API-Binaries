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

#include <assert.h>
#include <rtl.h>
#include <ntsecapi.h>
#include <cmfuncs.h>
#include "localelcid.h"
#include <rtlavl.h>
#include <rtltypes.h>
#include <winnls.h>
#include <assert.h>
#include <stdarg.h>
#include <limits.h>
#include <list.h>
#include <wmistr.h>
//#include "ntdll_misc.h"

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

/* Definitions *****************************************/

typedef PVOID* PALPC_HANDLE;

typedef PVOID ALPC_HANDLE;

typedef DWORD WINAPI RTL_RUN_ONCE_INIT_FN(PRTL_RUN_ONCE, PVOID, PVOID*);

typedef RTL_RUN_ONCE_INIT_FN *PRTL_RUN_ONCE_INIT_FN;

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

typedef enum _TRANSACTION_INFORMATION_CLASS { 
  TransactionBasicInformation               = 0,
  TransactionPropertiesInformation,
  TransactionEnlistmentInformation,
  TransactionSuperiorEnlistmentInformation
} TRANSACTION_INFORMATION_CLASS;

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

static void SHA1Transform(ULONG State[5], UCHAR Buffer[64])
{
   ULONG a, b, c, d, e;
   ULONG *Block;

   Block = (ULONG*)Buffer;

   /* Copy Context->State[] to working variables */
   a = State[0];
   b = State[1];
   c = State[2];
   d = State[3];
   e = State[4];

   /* 4 rounds of 20 operations each. Loop unrolled. */
   R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
   R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
   R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
   R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
   R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
   R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
   R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
   R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
   R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
   R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
   R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
   R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
   R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
   R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
   R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
   R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
   R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
   R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
   R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
   R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

   /* Add the working variables back into Context->State[] */
   State[0] += a;
   State[1] += b;
   State[2] += c;
   State[3] += d;
   State[4] += e;

   /* Wipe variables */
   a = b = c = d = e = 0;
}

/* Functions Prototypes for public*/

NTSYSAPI
ULONG
NTAPI
RtlIsDosDeviceName_Ustr(
    _In_ PCUNICODE_STRING Name
);

NTSYSAPI
PVOID 
NTAPI 
RtlEncodePointer(PVOID ptr);

NTSYSAPI
PVOID 
NTAPI 
RtlDecodePointer(PVOID ptr);

NTSYSAPI
PVOID 
NTAPI 
RtlEncodeSystemPointer(PVOID ptr);

NTSYSAPI
PVOID 
NTAPI 
RtlDecodeSystemPointer(PVOID ptr);

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
WINAPI
RtlSleepConditionVariableCS(
  RTL_CONDITION_VARIABLE* variable,
  RTL_CRITICAL_SECTION*   crit,
  const LARGE_INTEGER*    timeout
);
 
void 
WINAPI
RtlWakeConditionVariable(
  RTL_CONDITION_VARIABLE* variable
);

NTSYSAPI 
void 
WINAPI 
RtlInitializeConditionVariable(
	PRTL_CONDITION_VARIABLE
);

NTSYSAPI 
void
WINAPI 
RtlWakeAllConditionVariable(
	PRTL_CONDITION_VARIABLE
);

#define WOW64_TLS_FILESYSREDIR      8

inline
PVOID 
Wow64SetFilesystemRedirectorEx (
    IN PVOID NewValue
    )
/*++

Routine Description:

    This routine allows a thread running inside Wow64 to disable file-system 
    redirection for all calls happening in the context of this thread.
    
    
    NOTE: This routine should only called from a wow64 process, and is only available 
          when running on .NET server platforms and beyond. If you component will 
          run on downlevel platforms (XP 2600 for example), you shouldn't use WOW64_FILE_SYSTEM_DISABLE_REDIRECT (see below).

Example (Enumerating files under c:\windows\system32):
    
    {
        HANDLE File;
        WIN32_FIND_DATA FindData;
#ifndef _WIN64        
        BOOL bWow64Process = FALSE;
        PVOID Wow64RedirectionOld;
#endif        

        //
        // Disable Wow64 file system redirection
        //
#ifndef _WIN64        
        IsWow64Process (GetCurrentProcess (), &bWow64Process);
        if (bWow64Process == TRUE) {
            Wow64RedirectionOld = Wow64SetFilesystemRedirectorEx (WOW64_FILE_SYSTEM_DISABLE_REDIRECT);
        }
#endif        
        File = FindFirstFileA ("c:\\windows\\system32\\*.*", &FindData);
        
        do {
        .
        .
        } while (FindNextFileA (File, &FindData) != 0);
        
        FindClose (File);
        
        //
        // Enable Wow64 file-system redirection
        //
#ifndef _WIN64        
        if (bWow64Process == TRUE) {
            Wow64SetFilesystemRedirectorEx (Wow64RedirectionOld);
        }
#endif        
    }


Arguments:

    NewValue - New Wow64 file-system redirection value. This can either be:
               a- pointer to a unicode string with a fully-qualified path name (e.g. L"c:\\windows\\notepad.exe").
               b- any of the following predefined values :
                  * WOW64_FILE_SYSTEM_ENABLE_REDIRECT : Enables file-system redirection (default)
                  * WOW64_FILE_SYSTEM_DISABLE_REDIRECT : Disables file-system redirection on all
                    file I/O operations happening within the context of the current thread.
                  * WOW64_FILE_SYSTEM_DISABLE_REDIRECT_LEGACY: Use this only if you want to run on 
                    download level platforms (for example XP 2600), as it will have no effect
                    and prevents your program from malfunctioning.
    
Return:

    Old Wow64 file-system redirection value

--*/
{
    PVOID OldValue;
    OldValue = (PVOID)(ULONG_PTR)NtCurrentTeb()->TlsSlots[WOW64_TLS_FILESYSREDIR];
    NtCurrentTeb()->TlsSlots[WOW64_TLS_FILESYSREDIR] = NewValue;//(ULONGLONG)PtrToUlong(NewValue);
    return OldValue;
}

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

typedef struct _TIME_DYNAMIC_ZONE_INFORMATION
{
     LONG Bias;
     WCHAR StandardName[32];
     SYSTEMTIME StandardDate;
     LONG StandardBias;
     WCHAR DaylightName[32];
     SYSTEMTIME DaylightDate;
     LONG DaylightBias;
     WCHAR TimeZoneKeyName[128];
     BOOLEAN DynamicDaylightTimeDisabled;
} DYNAMIC_TIME_ZONE_INFORMATION, *PDYNAMIC_TIME_ZONE_INFORMATION;

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

extern RTL_CRITICAL_SECTION TIME_tz_section;