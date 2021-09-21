/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * FILE:            dll/win32/kernel32/stub.c
 * PURPOSE:         Activation contexts - NT-compatible helpers
 * PROGRAMMERS:     Alex Ionescu (alex.ionescu@reactos.org)
 *
 * NOTE:            See also kernel32/stub.c
 */

/* INCLUDES ******************************************************************/

#include <k32.h>
#define NDEBUG
#include <debug.h>

// 
// Virtual Buffer data structure and function prototypes. 
// 
 
typedef struct _VIRTUAL_BUFFER { 
    LPVOID Base; 
    ULONG PageSize; 
    LPVOID CommitLimit; 
    LPVOID ReserveLimit; 
} VIRTUAL_BUFFER, *PVIRTUAL_BUFFER; 

typedef enum DEP_SYSTEM_POLICY_TYPE {
    DEPPolicyAlwaysOff,
    DEPPolicyAlwaysOn,
    DEPPolicyOptIn,
    DEPPolicyOptOut,
    DEPTotalPolicyCount,
}DEP_SYSTEM_POLICY_TYPE;

VOID
WINAPI
CreateProcessInternalWSecure()
{
	;
}	

BOOL
QueryWin31IniFilesMappedToRegistry(
                                  IN DWORD Flags,
                                  OUT PWSTR Buffer,
                                  IN DWORD cchBuffer,
                                  OUT LPDWORD cchUsed
                                  )
{
	return FALSE;
}

BOOL
WINAPI
GetNumaAvailableMemory(
    PSYSTEM_NUMA_INFORMATION Memory,
    ULONG Length,
    PULONG ReturnedLength
    )
{
	return FALSE;
}

BOOL
WINAPI
GetNumaProcessorMap(
    PSYSTEM_NUMA_INFORMATION Map,
    ULONG Length,
    PULONG ReturnedLength
    )
{
	return FALSE;
}

BOOLEAN
CreateVirtualBuffer(
                   OUT PVIRTUAL_BUFFER Buffer,
                   IN ULONG CommitSize OPTIONAL,
                   IN ULONG ReserveSize OPTIONAL
                   )
{
	return FALSE;
}

BOOLEAN
ExtendVirtualBuffer(
                   IN PVIRTUAL_BUFFER Buffer,
                   IN PVOID Address
                   )		
{
	return FALSE;
}

BOOLEAN
FreeVirtualBuffer(
                 IN PVIRTUAL_BUFFER Buffer
                 )				
{
	return FALSE;
}

BOOLEAN
TrimVirtualBuffer(
                 IN PVIRTUAL_BUFFER Buffer
                 )				 
{
	return FALSE;
}		

int
VirtualBufferExceptionHandler(
                             IN DWORD ExceptionCode,
                             IN PEXCEPTION_POINTERS ExceptionInfo,
                             IN OUT PVIRTUAL_BUFFER Buffer
                             )
{
	return 0;
}							 

ULONGLONG
WINAPI
NumaVirtualQueryNode(
    IN  ULONG       NumberOfRanges,
    IN  PULONG_PTR  RangeList,
    OUT PULONG_PTR  VirtualPageAndNode,
    IN  SIZE_T      MaximumOutputLength
    )
{
	return 0;
}

BOOL WINAPI GetProcessDEPPolicy(
  _In_  HANDLE  hProcess,
  _Out_ LPDWORD lpFlags,
  _Out_ PBOOL   lpPermanent
)
{
	return FALSE;
}

DEP_SYSTEM_POLICY_TYPE WINAPI GetSystemDEPPolicy(void)
{
	return DEPPolicyAlwaysOff;
}


BOOL WINAPI SetProcessDEPPolicy(
  _In_ DWORD dwFlags
)
{
	return FALSE;
}

BOOL WINAPI SetSearchPathMode(
  _In_ DWORD Flags
)
{
	return FALSE;
}

DWORD
CtrlRoutine(
    IN LPVOID lpThreadParameter
    )
{
	return 0;
}

DWORD
ConsoleIMERoutine(
    IN LPVOID lpThreadParameter
    )
{
	return 0;
}

VOID
BaseProcessStart(
    PPROCESS_START_ROUTINE lpStartAddress
)
{
	;
}

VOID
BaseThreadStart(
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter
    )
{
	;
}