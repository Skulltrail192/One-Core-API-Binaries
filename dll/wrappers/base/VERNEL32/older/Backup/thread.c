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
 
#define NDEBUG

#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

/*implemented - need test ;need x64 version
*
*/
BOOL WINAPI IsThreadAFiber()
{
  return ((ULONG)(NtCurrentTeb()[1].NtTib.SubSystemTib) >> 2) & 1;
}

BOOL 
WINAPI 
QueryThreadCycleTime(
	HANDLE ThreadHandle, 
	PULONG64 CycleTime
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  PULONG64 ThreadInformation; // [sp+0h] [bp-10h]@1

  status = NtQueryInformationThread(
             ThreadHandle,
             MaxThreadInfoClass|ThreadEnableAlignmentFaultFixup,
             &ThreadInformation,
             0x10u,
             0);
  if ( NT_SUCCESS(status))
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  else
  {
    CycleTime = ThreadInformation;
    result = TRUE;
  }
  return result;
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

BOOL 
WINAPI 
SetThreadpoolStackInformation(
	PTP_POOL ptpp, 
	PTP_POOL_STACK_INFORMATION ptpsi
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = TpSetPoolStackInformation(ptpp, ptpsi);
  if (NT_SUCCESS(status))
  {
    result = TRUE;
  }
  else
  {
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    result = FALSE;
  }
  return result;
}

// BOOL 
// WINAPI 
// SetThreadIdealProcessorEx(
	// HANDLE ThreadHandle, 
	// PPROCESSOR_NUMBER ThreadInformation, 
	// PPROCESSOR_NUMBER lpPreviousIdealProcessor
// )
// {
  // NTSTATUS status; // eax@1
  // BOOL result; // eax@2

  // ThreadInformation = *(PPROCESSOR_NUMBER *)&ThreadInformation->Group;
  // status = NtSetInformationThread(ThreadHandle, (THREADINFOCLASS)0x21u, &ThreadInformation, 4u);
  // if (NT_SUCCESS(status))
  // {
    // if ( lpPreviousIdealProcessor )
      // lpPreviousIdealProcessor->Group = ThreadInformation->Group;
    // result = TRUE;
  // }
  // else
  // {
    // BaseSetLastNTError(status);
    // result = FALSE;
  // }
  // return result;
// }

BOOL 
WINAPI 
SetThreadIdealProcessorEx(
	HANDLE thread, 
	PROCESSOR_NUMBER *processor, 
	PROCESSOR_NUMBER *previous
)
{


    if (!processor || processor->Group > 0 || processor->Number > MAXIMUM_PROCESSORS)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (previous)
    {
        previous->Group = 0;
        previous->Number = 0;
        previous->Reserved = 0;
    }

    return TRUE;
}

BOOL 
WINAPI 
SetThreadGroupAffinity(
	HANDLE ThreadHandle, const GROUP_AFFINITY *GroupAffinity, PGROUP_AFFINITY PreviousGroupAffinity)
{
  NTSTATUS status; // eax@2
  BOOL result; // eax@6
  PVOID ThreadInformation; // [sp+Ch] [bp-10h]@2
  WORD one = 0; // [sp+10h] [bp-Ch]@5
  WORD two = 0; // [sp+14h] [bp-8h]@5

  if ( PreviousGroupAffinity
    && (status = NtQueryInformationThread(
                   ThreadHandle,
                   MaxThreadInfoClass|ThreadPriorityBoost,
                   &ThreadInformation,
                   0xCu,
                   0),
        !NT_SUCCESS(status))
    || (status = NtSetInformationThread(ThreadHandle, MaxThreadInfoClass|ThreadPriorityBoost, GroupAffinity, 0xCu),
        !NT_SUCCESS(status)) )
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  else
  {
    if ( PreviousGroupAffinity )
    {
      PreviousGroupAffinity->Mask = (KAFFINITY)ThreadInformation;
      PreviousGroupAffinity->Group = one;
      PreviousGroupAffinity->Reserved[1] = two;
    }
    result = TRUE;
  }
  return result;
}

DWORD 
WINAPI 
ReadThreadProfilingData(
	HANDLE PerformanceDataHandle, 
	DWORD Flags, 
	PPERFORMANCE_DATA PerformanceData
)
{
  NTSTATUS status; // eax@1
  DWORD result; // eax@2

  status = RtlReadThreadProfilingData(PerformanceDataHandle, Flags, PerformanceData);
  if ( status >= 0 )
    result = 0;
  else
    result = RtlNtStatusToDosErrorNoTeb(status);
  return result;
}

BOOL 
WINAPI 
QueryThreadpoolStackInformation(
	PTP_POOL ptpp, 
	PTP_POOL_STACK_INFORMATION ptpsi
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = TpQueryPoolStackInformation(ptpp, ptpsi);
  if ( status >= 0 )
  {
    result = 1;
  }
  else
  {
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    result = 0;
  }
  return result;
}

DWORD 
WINAPI 
QueryThreadProfiling(
	HANDLE HANDLE, 
	PBOOLEAN Enabled
)
{
  NTSTATUS status; // eax@1
  DWORD result; // eax@2

  status = RtlQueryThreadProfiling(HANDLE, Enabled);
  if ( status >= 0 )
    result = 0;
  else
    result = RtlNtStatusToDosErrorNoTeb(status);
  return result;
}

BOOL 
WINAPI 
GetThreadIdealProcessorEx(
	HANDLE ThreadHandle, 
	PPROCESSOR_NUMBER ThreadInformation
)
{
  NTSTATUS status; // eax@5

  if ( ThreadHandle == (HANDLE)-2 )
  {
    
   #ifdef _M_IX86
	*(DWORD *)&ThreadInformation->Group = *(DWORD *)(__readfsdword(24) + 3956);
   #elif defined(_M_AMD64)
    *(DWORD *)&ThreadInformation->Group = *(DWORD *)(__readgsqword(24) + 3956);
   #endif*/  
    ThreadInformation->Reserved = 0;
    return TRUE;
  }
  status = NtQueryInformationThread(ThreadHandle, (THREADINFOCLASS)33u, ThreadInformation, 4u, 0);
  if ( status >= 0 )
    return TRUE;
  BaseSetLastNTError(status);
  return FALSE;
}

BOOL 
WINAPI 
GetThreadGroupAffinity(
	HANDLE ThreadHandle, 
	PGROUP_AFFINITY ThreadInformation
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = NtQueryInformationThread(ThreadHandle, MaxThreadInfoClass|ThreadPriorityBoost, ThreadInformation, 0xCu, 0);
  if ( status >= 0 )
  {
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  return result;
}

DWORD 
WINAPI 
DisableThreadProfiling(
	HANDLE PerformanceDataHandle
)
{
  NTSTATUS status; // eax@1
  DWORD result; // eax@2

  status = RtlDisableThreadProfiling(PerformanceDataHandle);
  if ( status >= 0 )
    result = 0;
  else
    result = RtlNtStatusToDosErrorNoTeb(status);
  return result;
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

HANDLE 
WINAPI 
CreateRemoteThreadEx(
  _In_       HANDLE hProcess,
  _In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
  _In_       SIZE_T dwStackSize,
  _In_       LPTHREAD_START_ROUTINE lpStartAddress,
  _In_opt_   LPVOID lpParameter,
  _In_       DWORD dwCreationFlags,
  _In_opt_   LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,
  _Out_opt_  LPDWORD lpThreadId
)
{
	return CreateRemoteThread(hProcess, 
							  lpThreadAttributes, 
							  dwStackSize, 
							  lpStartAddress,
							  lpParameter,
							  dwCreationFlags,
							  lpThreadId);
}

DWORD 
WINAPI 
EnableThreadProfiling(
	HANDLE ThreadHandle, 
	DWORD Flags, 
	DWORD64 HardwareCounters, 
	HANDLE PerformanceDataHandle
)
{
  NTSTATUS status; // eax@1
  DWORD result = 0; // eax@2

  status = RtlEnableThreadProfiling(
             ThreadHandle,
             Flags,
             HardwareCounters,
             PerformanceDataHandle);
  if ( NT_SUCCESS(result) )
    result = 0;
  else
    result = RtlNtStatusToDosErrorNoTeb(status);
  return result;
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
  BOOL resp = FALSE; // esi@2
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
      resp = TRUE;
    else
      BaseSetLastNTError(status);
  }
  return resp;
}

BOOL WINAPI SetThreadInformation(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, DWORD ThreadInformationLength)
{
  THREADINFOCLASS threadClass; // eax@2
  NTSTATUS status; // eax@3
  BOOL result; // eax@4

  if ( ThreadInformationClass )
  {
    if ( ThreadInformationClass != 1 )
    {
      BaseSetLastNTError(STATUS_INVALID_PARAMETER);
      return FALSE;
    }
    threadClass = 25;
  }
  else
  {
    threadClass = 24;
  }
  status = NtSetInformationThread(ThreadHandle, threadClass, ThreadInformation, ThreadInformationLength);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  else
  {
    result = TRUE;
  }
  return result;
}

VOID WINAPI GetCurrentThreadStackLimits(PULONG_PTR LowLimit, PULONG_PTR HighLimit)
{
  PULONG_PTR numberOne; // edx@1
  
  #ifdef _M_IX86
		numberOne = (PULONG_PTR)__readfsdword(24);
  #elif defined(_M_AMD64)
		numberOne = (PULONG_PTR)__readgsqword(24);
  #endif
  *LowLimit = numberOne[899];
  *HighLimit = numberOne[1];
}

DWORD WINAPI GetThreadActualPriority(HANDLE ThreadHandle)
{
  NTSTATUS status; // eax@1
  DWORD result; // eax@2
  PVOID ThreadInformation; // [sp+0h] [bp-1Ch]@1
  DWORD other = 0; // [sp+14h] [bp-8h]@3

  status = NtQueryInformationThread(ThreadHandle, 0, &ThreadInformation, 0x1Cu, 0);
  if ( status >= 0 )
  {
    result = other;
  }
  else
  {
    BaseSetLastNTError(status);
    result = 0x7FFFFFFFu;
  }
  return result;
}

DWORD
APIENTRY
GetProcessIdOfThread(
    HANDLE Thread
    )
/*++

Routine Description:

    Gets the process ID of the thread opened via the specified handle

Arguments:

    Thread - Handle of thread to do the query on

Return Value:

    Returns a unique value representing the process ID of the
    executing thread.  The return value may be used to identify a process
    in the system. If the function fails the return value is zero.

--*/

{
    NTSTATUS Status;
    THREAD_BASIC_INFORMATION tbi;

    Status = NtQueryInformationThread (Thread,
                                       ThreadBasicInformation,
                                       &tbi,
                                       sizeof (tbi),
                                       NULL);

    if (!NT_SUCCESS (Status)) {
        BaseSetLastNTError (Status);
        return 0;
    }

    return HandleToUlong (tbi.ClientId.UniqueProcess);
}

BOOL 
WINAPI 
SetThreadActualPriority(
	HANDLE ThreadHandle,
	SYSTEM_THREAD_INFORMATION *ThreadInformation
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = NtSetInformationThread(ThreadHandle, ThreadPriority, &ThreadInformation, 4u);
  if ( NT_SUCCESS(status))
  {
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  return result;
}