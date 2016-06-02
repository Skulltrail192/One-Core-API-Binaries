/* COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * PURPOSE:         Vista functions
 * PROGRAMMER:      Thomas Weidenmueller <w3seek@reactos.com>
 */

#include <k32.h>

#define NDEBUG
#include <debug.h>

typedef struct _VIRTUAL_BUFFER {
    LPVOID Base;
    ULONG PageSize;
    LPVOID CommitLimit;
    LPVOID ReserveLimit;
} VIRTUAL_BUFFER, *PVIRTUAL_BUFFER;

typedef enum _DEP_SYSTEM_POLICY_TYPE {
	AlwaysOn,
	AlwaysOff,
	OptIn,
	OptOut 
} DEP_SYSTEM_POLICY_TYPE;

BOOL WINAPI SetFileCompletionNotificationModes(
  _In_  HANDLE FileHandle,
  _In_  UCHAR Flags
)
{
	return TRUE;
}

void WINAPI BaseProcessStart(LPVOID lpfnStartRoutine)  
{
	;
}

void WINAPI BaseProcessStartThunkEx(LPVOID a1, LPVOID a2)
{
  BaseProcessStart(a1);
}

void WINAPI BaseThreadStart(LPVOID lpfnStartRoutine)  
{
	;
}

void WINAPI BaseThreadStartThunk(LPVOID a1, LPVOID a2)
{
  BaseThreadStart(a1);
}

NTSTATUS WINAPI ConsoleIMERoutine()
{
	return STATUS_SUCCESS;
}

void WINAPI CtrlRoutine(DWORD a1)
{
	;
}

DWORD WINAPI GetModuleFileNameExA(
  _In_      HANDLE hProcess,
  _In_opt_  HMODULE hModule,
  _Out_     LPTSTR lpFilename,
  _In_      DWORD nSize
)
{
	return 0;
}

DWORD WINAPI GetModuleFileNameExW(
  _In_      HANDLE hProcess,
  _In_opt_  HMODULE hModule,
  _Out_     LPWSTR lpFilename,
  _In_      DWORD nSize
)
{
	return 0;
}

DWORD64 WINAPI NumaVirtualQueryNode(DWORD Address, int a2, int a3, DWORD a4)
{
	return 0;
}

int
VirtualBufferExceptionFilter(
    IN DWORD ExceptionCode,
    IN PEXCEPTION_POINTERS ExceptionInfo,
    IN OUT PVIRTUAL_BUFFER Buffer
)
{
	return TRUE;
}

BOOL
TrimVirtualBuffer(
    IN PVIRTUAL_BUFFER Buffer
)
{
	return TRUE;
}

BOOL WINAPI GetNumaProcessorMap(PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength)
{
	return TRUE;
}

BOOL WINAPI GetNumaAvailableMemory(PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength)
{
	return TRUE;
}

BOOL
FreeVirtualBuffer(
    IN PVIRTUAL_BUFFER Buffer
)
{
	return TRUE;
}

BOOL
ExtendVirtualBuffer(
    IN PVIRTUAL_BUFFER Buffer,
    IN LPVOID Address
)
{
	return TRUE;
}

BOOL
CreateVirtualBuffer(
    OUT PVIRTUAL_BUFFER Buffer,
    IN DWORD CommitSize,
    IN DWORD ReserveSize OPTIONAL
)
{
	return TRUE;
}

void WINAPI CreateProcessInternalWSecure()
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
	return TRUE;
}

/*NTSTATUS WINAPI BaseCleanupAppcompatCache(VOID) 	
{
	return 0x00000000;
}*/
/*
BOOL WINAPI BaseInitAppcompatCache() 	
{
	return TRUE;
}*/

BOOL WINAPI GetProcessDEPPolicy(HANDLE ProcessInformation, LPDWORD lpFlags, PBOOL lpPermanent)
{
	return TRUE;
}

DWORD WINAPI SetProcessDEPPolicy(DWORD ProcessInformation)
{
	return 0;
}

DEP_SYSTEM_POLICY_TYPE WINAPI GetSystemDEPPolicy(void)
{
	return OptOut;
}

BOOL WINAPI SetSearchPathMode(DWORD Flags)
{
	return TRUE;
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