/*++

Copyright (c) 2017  Shorthorn Project

Module Name:

    thread.c

Abstract:

    This module implements Win32 Thread Object APIs

Author:

    Skulltrail 16-March-2017

Revision History:

--*/
 
#define NDEBUG

#include "main.h"

static BOOL (WINAPI *pSetThreadStackGuarantee)(PULONG);
static DWORD (WINAPI *pConsoleIMERoutine)(LPVOID);
static DWORD (WINAPI *pCtrlRoutine)(LPVOID);

/***********************************************************************
 *           FlsAlloc   (KERNEL32.@) - For XP support
 */
DWORD 
WINAPI 
FlsAlloc(
	PFLS_CALLBACK_FUNCTION lpCallback
)
{
	return TlsAlloc();
}
/***********************************************************************
 *           FlsFree   (KERNEL32.@) - For XP support
 */
BOOL 
WINAPI 
FlsFree(
	DWORD dwFlsIndex
)
{
	return TlsFree(dwFlsIndex);
}

/***********************************************************************
 *           FlsGetValue   (KERNEL32.@) - For XP support
 */
PVOID 
WINAPI 
FlsGetValue( 
	DWORD index 
)
{
	return TlsGetValue(index);
}

/***********************************************************************
 *           FlsSetValue   (KERNEL32.@) - For XP support
 */
BOOL 
WINAPI 
FlsSetValue(
	DWORD dwFlsIndex, 
	PVOID lpFlsData
)
{
  return TlsSetValue(dwFlsIndex, lpFlsData);
}


/***********************************************************************
 *           ConvertThreadToFiberEx   (KERNEL32.@) - For XP support
 *
 * @implemented
 */
LPVOID
WINAPI
ConvertThreadToFiberEx(
	_In_opt_ LPVOID lpParameter,
    _In_ DWORD dwFlags
)
{
    PTEB Teb;
    PFIBER Fiber;

    /* Check for invalid flags */
    if (dwFlags & ~FIBER_FLAG_FLOAT_SWITCH)
    {
        /* Fail */
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }

    /* Are we already a fiber? */
    Teb = NtCurrentTeb();
    if (Teb->HasFiberData)
    {
        /* Fail */
        SetLastError(ERROR_ALREADY_FIBER);
        return NULL;
    }

    /* Allocate the fiber */
    Fiber = RtlAllocateHeap(RtlGetProcessHeap(),
                            0,
                            sizeof(FIBER));
    if (!Fiber)
    {
        /* Fail */
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return NULL;
    }

    /* Copy some contextual data from the thread to the fiber */
    Fiber->FiberData = lpParameter;
    Fiber->ExceptionList = Teb->NtTib.ExceptionList;
    Fiber->StackBase = Teb->NtTib.StackBase;
    Fiber->StackLimit = Teb->NtTib.StackLimit;
    Fiber->DeallocationStack = Teb->DeallocationStack;
    Fiber->FlsData = Teb->FlsData;
    Fiber->GuaranteedStackBytes = Teb->GuaranteedStackBytes;
    Fiber->ActivationContextStackPointer = Teb->ActivationContextStackPointer;

    /* Save FPU State if requested, otherwise just the basic registers */
    Fiber->FiberContext.ContextFlags = (dwFlags & FIBER_FLAG_FLOAT_SWITCH) ?
                                       (CONTEXT_FULL | CONTEXT_FLOATING_POINT) :
                                       CONTEXT_FULL;

    /* Associate the fiber to the current thread */
    Teb->NtTib.FiberData = Fiber;
    Teb->HasFiberData = TRUE;

    /* Return opaque fiber data */
    return (LPVOID)Fiber;
}

/**********************************************************************
 *           SetThreadStackGuarantee   (KERNEL32.@)
 */
BOOL 
WINAPI 
SetThreadStackGuarantee(
	PULONG stacksize
)
{
	HMODULE hkernel32 = GetModuleHandleA("kernelfull.dll");
	pSetThreadStackGuarantee = (void *)GetProcAddress(hkernel32, "SetThreadStackGuarantee");
	if(pSetThreadStackGuarantee){
		return pSetThreadStackGuarantee(stacksize);
	}else{
		static int once;
		if (once++ == 0)
			DbgPrint("SetThreadStackGuarantee: stub\n", stacksize);
		return TRUE;
	}    
}

DWORD
WINAPI
ConsoleIMERoutine(
    IN LPVOID lpThreadParameter
    )
{
	HMODULE hkernel32 = GetModuleHandleA("kernelfull.dll");
	pConsoleIMERoutine = (void *)GetProcAddress(hkernel32, "ConsoleIMERoutine");
	if(pConsoleIMERoutine){
		return pConsoleIMERoutine(lpThreadParameter);
	}else{		
		return 0;
	}   		
}

DWORD
WINAPI
CtrlRoutine(
    IN LPVOID lpThreadParameter
)
{
	HMODULE hkernel32 = GetModuleHandleA("kernelfull.dll");
	pCtrlRoutine = (void *)GetProcAddress(hkernel32, "CtrlRoutine");
	if(pCtrlRoutine){
		return pCtrlRoutine(lpThreadParameter);
	}else{		
		return 0;
	}  	
}

/*
 * @implemented
 */
DWORD
WINAPI
GetThreadId(IN HANDLE Thread)
{
    THREAD_BASIC_INFORMATION ThreadBasic;
    NTSTATUS Status;

    Status = NtQueryInformationThread(Thread,
                                      ThreadBasicInformation,
                                      &ThreadBasic,
                                      sizeof(THREAD_BASIC_INFORMATION),
                                      NULL);
    if (!NT_SUCCESS(Status))
    {
        BaseSetLastNTError(Status);
        return 0;
    }

    return HandleToUlong(ThreadBasic.ClientId.UniqueThread);
}

DWORD
APIENTRY
GetProcessIdOfThread(
    HANDLE Thread
    )
{
    NTSTATUS Status;
    THREAD_BASIC_INFORMATION ThreadBasic;

    Status = NtQueryInformationThread (Thread,
                                       ThreadBasicInformation,
                                       &ThreadBasic,
                                       sizeof (ThreadBasic),
                                       NULL);

    if (!NT_SUCCESS (Status)) {
        BaseSetLastNTError (Status);
        return 0;
    }

    return HandleToUlong (ThreadBasic.ClientId.UniqueProcess);
}

BOOL 
WINAPI 
GetThreadInformation(
	HANDLE ProcessHandle, 
	THREADINFOCLASS ThreadInformationClass,
	PVOID ThreadInformation, 
	DWORD ThreadInformationSize
)
{
  BOOL result = FALSE; // esi@2
  NTSTATUS status; // eax@3

  if ( ThreadInformationClass )
  {
    BaseSetLastNTError(STATUS_INVALID_PARAMETER);
	return FALSE;
  }
  else
  {
    status = NtQueryInformationProcess(
               ProcessHandle,
               ProcessDebugPort|0x20,
               ThreadInformation,
               ThreadInformationSize,
               0);
    if ( NT_SUCCESS(status) )
      result = TRUE;
    else
      BaseSetLastNTError(status);
  }
  return result;
}

/***********************************************************************
 *              CreateThreadpoolWork (KERNEL32.@)
 */
PTP_WORK 
WINAPI 
CreateThreadpoolWork( 
	PTP_WORK_CALLBACK callback, 
	PVOID userdata,
	TP_CALLBACK_ENVIRON *environment 
)
{
    TP_WORK *work;
    NTSTATUS status;

    status = TpAllocWork( &work, callback, userdata, environment );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return NULL;
    }

    return work;
}

/***********************************************************************
 *              CreateThreadpoolTimer (KERNEL32.@)
 */
PTP_TIMER 
WINAPI 
CreateThreadpoolTimer( 
	PTP_TIMER_CALLBACK callback, 
	PVOID userdata,
	TP_CALLBACK_ENVIRON *environment
)
{
    TP_TIMER *timer;
    NTSTATUS status;

    status = TpAllocTimer( &timer, callback, userdata, environment );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return NULL;
    }

    return timer;
}

/***********************************************************************
 *              CreateThreadpoolWait (KERNEL32.@)
 */
PTP_WAIT 
WINAPI 
CreateThreadpoolWait( 
	PTP_WAIT_CALLBACK callback, 
	PVOID userdata,
	TP_CALLBACK_ENVIRON *environment 
)
{
    TP_WAIT *wait;
    NTSTATUS status;

    status = TpAllocWait( &wait, callback, userdata, environment );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return NULL;
    }

    return wait;
}

/***********************************************************************
 *              GetThreadGroupAffinity (KERNEL32.@)
 */
BOOL 
WINAPI 
GetThreadGroupAffinity(
	HANDLE thread, 
	GROUP_AFFINITY *affinity 
)
{
    NTSTATUS status;

    if (!affinity)
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }

    status = NtQueryInformationThread( thread, MaxThreadInfoClass|ThreadPriorityBoost,
                                       affinity, sizeof(*affinity), NULL );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }

    return TRUE;
}

/***********************************************************************
 *              SetThreadGroupAffinity (KERNEL32.@)
 */
BOOL 
WINAPI 
SetThreadGroupAffinity( 
	HANDLE thread, 
	const GROUP_AFFINITY *affinity_new,
    GROUP_AFFINITY *affinity_old 
)
{
    NTSTATUS status;

    if (affinity_old && !GetThreadGroupAffinity( thread, affinity_old ))
        return FALSE;

    status = NtSetInformationThread( thread, MaxThreadInfoClass|ThreadPriorityBoost,
                                     (PVOID)affinity_new, sizeof(*affinity_new) );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }

    return TRUE;
}

/*************************************************************************
 *              rtlmode_to_win32mode
 */
static 
DWORD 
rtlmode_to_win32mode( 
	DWORD rtlmode 
)
{
    DWORD win32mode = 0;

    if (rtlmode & 0x10)
        win32mode |= SEM_FAILCRITICALERRORS;
    if (rtlmode & 0x20)
        win32mode |= SEM_NOGPFAULTERRORBOX;
    if (rtlmode & 0x40)
        win32mode |= SEM_NOOPENFILEERRORBOX;

    return win32mode;
}

/***********************************************************************
 *              SetThreadErrorMode (KERNEL32.@)
 *
 * Set the thread local error mode.
 *
 * PARAMS
 *  mode    [I] The new error mode, a bitwise or of SEM_FAILCRITICALERRORS,
 *              SEM_NOGPFAULTERRORBOX and SEM_NOOPENFILEERRORBOX.
 *  oldmode [O] Destination of the old error mode (may be NULL)
 *
 * RETURNS
 *  Success: TRUE
 *  Failure: FALSE, check GetLastError
 */
BOOL 
WINAPI 
SetThreadErrorMode( 
	DWORD mode, 
	LPDWORD oldmode 
)
{
    NTSTATUS status;
    DWORD tmp = 0;

    if (mode & ~(SEM_FAILCRITICALERRORS |
                 SEM_NOGPFAULTERRORBOX |
                 SEM_NOOPENFILEERRORBOX))
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }

    if (mode & SEM_FAILCRITICALERRORS)
        tmp |= 0x10;
    if (mode & SEM_NOGPFAULTERRORBOX)
        tmp |= 0x20;
    if (mode & SEM_NOOPENFILEERRORBOX)
        tmp |= 0x40;

    status = RtlSetThreadErrorMode( tmp, oldmode );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }

    if (oldmode)
        *oldmode = rtlmode_to_win32mode(*oldmode);

    return TRUE;
}

/***********************************************************************
 *              GetThreadErrorMode (KERNEL32.@)
 *
 * Get the thread local error mode.
 *
 * PARAMS
 *  None.
 *
 * RETURNS
 *  The current thread local error mode.
 */
DWORD 
WINAPI 
GetThreadErrorMode( void )
{
    return rtlmode_to_win32mode( RtlGetThreadErrorMode() );
}

VOID 
WINAPI 
RaiseFailFastException(
  _In_opt_  PEXCEPTION_RECORD pExceptionRecord,
  _In_opt_  PCONTEXT pContextRecord,
  _In_      DWORD dwFlags
)
{
	RaiseException(pExceptionRecord->ExceptionCode, 
				   pExceptionRecord->ExceptionFlags, 
				   pExceptionRecord->NumberParameters, 
				   &pExceptionRecord->ExceptionInformation[0]);
}

BOOL 
WINAPI 
QueryThreadCycleTime(
  _In_  HANDLE   ThreadHandle,
  _Out_ PULONG64 CycleTime
)
{
	LARGE_INTEGER ltime;
	UINT32 cycles; 
	QueryPerformanceCounter(&ltime);

	cycles = (UINT32) ((ltime.QuadPart >> 8) & 0xFFFFFFF);	
	
	*CycleTime = cycles;
	return TRUE;
}

HANDLE 
WINAPI
CreateRemoteThreadEx(
  _In_      HANDLE                       hProcess,
  _In_opt_  LPSECURITY_ATTRIBUTES        lpThreadAttributes,
  _In_      SIZE_T                       dwStackSize,
  _In_      LPTHREAD_START_ROUTINE       lpStartAddress,
  _In_opt_  LPVOID                       lpParameter,
  _In_      DWORD                        dwCreationFlags,
  _In_opt_  LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,
  _Out_opt_ LPDWORD                      lpThreadId
)
{
	HANDLE handle = NULL;
	
	handle = CreateRemoteThread(hProcess,
								lpThreadAttributes,
								dwStackSize,
								lpStartAddress,
								lpParameter,
								dwCreationFlags,
								lpThreadId);
	//TODO create Thread with ThreadAttibutesList
	return handle;	
}