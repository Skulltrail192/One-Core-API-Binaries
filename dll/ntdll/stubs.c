/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS kernel
 * FILE:            dll/ntdll/unimplemented.c
 * PURPOSE:         CSR APIs exported through NTDLL
 * PROGRAMMER:      Alex Ionescu (alex@relsoft.net)
 */

/* INCLUDES *******************************************************************/

#include <ntdll.h>

#define NDEBUG
#include <debug.h>

typedef ULONG (*PCSR_CALLBACK_ROUTINE)(IN OUT PVOID ReplyMsg);

typedef struct _RTL_TRACE_SEGMENT *PRTL_TRACE_SEGMENT;

typedef struct _RTL_TRACE_DATABASE 	 *PRTL_TRACE_DATABASE;

// typedef struct _PREFIX_TABLE_ENTRY
// {
      // CSHORT NodeTypeCode;
      // CSHORT NameLength;
      // struct _PREFIX_TABLE_ENTRY *NextPrefixTree;
      // RTL_SPLAY_LINKS Links;
      // PSTRING Prefix;
// } PREFIX_TABLE_ENTRY, *PPREFIX_TABLE_ENTRY;

// typedef struct _PREFIX_TABLE
// {
      // CSHORT NodeTypeCode;
      // CSHORT NameLength;
      // PPREFIX_TABLE_ENTRY NextPrefixTree;
// } PREFIX_TABLE, *PPREFIX_TABLE;

typedef enum _COMPARISON {
    IsLessThan,
    IsPrefix,
    IsEqual,
    IsGreaterThan
} COMPARISON;

typedef struct _RTL_STACK_CONTEXT_ENTRY {

    ULONG_PTR Address; // stack address
    ULONG_PTR Data;    // stack contents

} RTL_STACK_CONTEXT_ENTRY, * PRTL_STACK_CONTEXT_ENTRY;

typedef struct _RTL_STACK_CONTEXT {

    ULONG NumberOfEntries;
    RTL_STACK_CONTEXT_ENTRY Entry[1];

} RTL_STACK_CONTEXT, * PRTL_STACK_CONTEXT;

typedef struct _CSR_CALLBACK_INFO {
    ULONG ApiNumberBase;
    ULONG MaxApiNumber;
    PCSR_CALLBACK_ROUTINE *CallbackDispatchTable;
} CSR_CALLBACK_INFO, *PCSR_CALLBACK_INFO;

PPREFIX_TABLE_ENTRY
NTAPI
PfxFindPrefix (
    IN PPREFIX_TABLE PrefixTable,
    IN PSTRING FullName
)
{
	return NULL;
}

VOID
NTAPI
PfxInitialize (
    IN PPREFIX_TABLE PrefixTable
)
{
	;
}

BOOLEAN
NTAPI
PfxInsertPrefix (
    IN PPREFIX_TABLE PrefixTable,
    IN PSTRING Prefix,
    IN PPREFIX_TABLE_ENTRY PrefixTableEntry
)
{
	return TRUE;
}

VOID
NTAPI
PfxRemovePrefix (
    IN PPREFIX_TABLE PrefixTable,
    IN PPREFIX_TABLE_ENTRY PrefixTableEntry
)
{
	;
}

NTSTATUS 
WINAPI 
RtlApplicationVerifierStop(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
	return STATUS_SUCCESS;
}

PRTL_UNLOAD_EVENT_TRACE 
NTAPI
RtlGetUnloadEventTrace(void)
{
	return NULL;
}

BOOLEAN
NTAPI
RtlTraceDatabaseAdd (
    IN PRTL_TRACE_DATABASE Database,
    IN ULONG Count,
    IN PVOID * Trace,
    OUT PRTL_TRACE_BLOCK * TraceBlock OPTIONAL
)
{
	return TRUE;
}

BOOLEAN
NTAPI
RtlTraceDatabaseDestroy (
    IN PRTL_TRACE_DATABASE Database
)
{
	return TRUE;
}

PRTL_TRACE_DATABASE
NTAPI
RtlTraceDatabaseCreate (
    IN ULONG Buckets,
    IN SIZE_T MaximumSize OPTIONAL,
    IN ULONG Flags, // OPTIONAL in User mode
    IN ULONG Tag,   // OPTIONAL in User mode
    IN RTL_TRACE_HASH_FUNCTION HashFunction OPTIONAL
)
{
	return NULL;
}

BOOLEAN
NTAPI
RtlTraceDatabaseEnumerate (
    PRTL_TRACE_DATABASE Database,
    OUT PRTL_TRACE_ENUMERATE Enumerate,
    OUT PRTL_TRACE_BLOCK * TraceBlock
)
{
	return TRUE;
}

BOOLEAN
NTAPI
RtlTraceDatabaseFind (
    PRTL_TRACE_DATABASE Database,
    IN ULONG Count,
    IN PVOID * Trace,
    OUT PRTL_TRACE_BLOCK * TraceBlock OPTIONAL
)
{
	return TRUE;
}

BOOLEAN
NTAPI
RtlTraceDatabaseValidate (
    IN PRTL_TRACE_DATABASE Database
)
{
	return TRUE;
}

VOID 
NTAPI
RtlTraceDatabaseLock (
    IN PRTL_TRACE_DATABASE Database
)
{
	;
}

VOID 
NTAPI
RtlTraceDatabaseUnlock (
    IN PRTL_TRACE_DATABASE Database
)
{
	;
}

_CRTIMP
EXCEPTION_DISPOSITION
__cdecl
__C_specific_handler(
    struct _EXCEPTION_RECORD *_ExceptionRecord,
    void *_EstablisherFrame,
    struct _CONTEXT *_ContextRecord,
    PVOID *DispatcherContext)
{
    UNIMPLEMENTED;
    __debugbreak();
    return 0;
}

void __chkstk()
{
	;
}

void NTAPI LdrProcessInitializationComplete()
{
	;
}

PVOID NTAPI
NtWow64CsrAllocateCaptureBuffer(
    __in ULONG Count,
    __in ULONG Size
    )
{
	return NULL;
}

ULONG NTAPI
NtWow64CsrAllocateMessagePointer(
    __inout PVOID CaptureBuffer,
    __in ULONG Length,
    __out PVOID *CapturedBuffer
)
{
	return 0;
}


VOID NTAPI
NtWow64CsrCaptureMessageBuffer(
    __inout PVOID CaptureBuffer,
    __in_opt PVOID Buffer,
    __in ULONG Length,
    __out PVOID *CapturedBuffer
    )
{
	;
}	

VOID NTAPI
NtWow64CsrCaptureMessageString(
    __inout PVOID CaptureBuffer,
    __in_opt PCSTR String,
    __in ULONG Length,
    __in ULONG MaximumLength,
    __out PSTRING CapturedString
    )
{
	;
}

NTSTATUS NTAPI
NtWow64CsrClientCallServer(
    __inout PVOID ApiMessage,
    __inout_opt PVOID CaptureBuffer,
    __in ULONG ApiNumber,
    __in ULONG ArgLength
    )
{
	return STATUS_SUCCESS;
}

NTSTATUS NTAPI
NtWow64CsrClientConnectToServer(
    __in PWSTR ObjectDirectory,
    __in ULONG ServerDllIndex,
    __in_opt PCSR_CALLBACK_INFO CallbackInformation,
    __in PVOID ConnectionInformation,
    __inout_opt PULONG ConnectionInformationLength,
    __out_opt PBOOLEAN CalledFromServer
    )
{
	return STATUS_SUCCESS;
}

NTSTATUS NTAPI
NtWow64CsrFreeCaptureBuffer(
    __in PVOID CaptureBuffer
    )
{
	return STATUS_SUCCESS;
}

DWORD NTAPI
NtWow64CsrGetProcessId(
    VOID
    )
{
	return 0;
}

NTSTATUS NTAPI
NtWow64CsrIdentifyAlertableThread(
    VOID
    )
{
	return STATUS_SUCCESS;
}


NTSTATUS NTAPI
NtWow64CsrNewThread(
    VOID
    )
{
	return STATUS_SUCCESS;
}

NTSTATUS NTAPI
NtWow64CsrSetPriorityClass(
    __in HANDLE ProcessHandle,
    __inout PULONG PriorityClass
    )
{
	return STATUS_SUCCESS;
}

NTSTATUS NTAPI
NtWow64DebuggerCall(
    __in ULONG ServiceClass,
    __in ULONG Arg1,
    __in ULONG Arg2,
    __in ULONG Arg3,
    __in ULONG Arg4
    )
{
	return STATUS_SUCCESS;
}


NTSTATUS NTAPI
NtWow64GetNativeSystemInformation(
    __in SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __out PVOID SystemInformation,
    __in ULONG SystemInformationLength,
    __out_opt PULONG ReturnLength
    )
{
	return STATUS_SUCCESS;
}

NTSTATUS NTAPI
NtWow64QueryInformationProcess64(
    __in HANDLE ProcessHandle,
    __in PROCESSINFOCLASS ProcessInformationClass,
    __out_bcount(ProcessInformationLength) PVOID ProcessInformation64,
    __in ULONG ProcessInformationLength,
    __out_opt PULONG ReturnLength
    )
{
	return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
NtWow64QueryVirtualMemory64(
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddressLow,
    IN PVOID BaseAddressHigh,
    IN MEMORY_INFORMATION_CLASS MemoryInformationClass,
    OUT PVOID MemoryInformation, /* NB must be 64bit aligned */
    IN ULONG LengthLow,
    IN ULONG LengthHigh,
    OUT PULONGLONG ReturnLength OPTIONAL
    )
{
	return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
NtWow64ReadVirtualMemory64(
    IN HANDLE ProcessHandle,
    IN PVOID AddressLow,
    IN PVOID AddressHigh,
    OUT PVOID Buffer,
    IN ULONG BufferSizeLow,
    IN ULONG BufferSizeHigh,
    OUT PULONGLONG BytesRead OPTIONAL
    )
{
	return STATUS_SUCCESS;
}