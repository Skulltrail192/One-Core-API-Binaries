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

BOOL WINAPI QueryThreadCycleTime(HANDLE ThreadHandle, PULONG64 CycleTime)
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
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  else
  {
    CycleTime = ThreadInformation;
    result = 1;
  }
  return result;
}

BOOL WINAPI SetThreadErrorMode(DWORD dwNewMode, LPDWORD lpOldMode)
{
  BOOL result; // eax@2
  ULONG NewMode; // eax@3
  NTSTATUS status; // eax@9
  DWORD error; // eax@12

  if ( dwNewMode & 0xFFFF7FFC )
  {
    BaseSetLastNTError(-1073741811);
    result = 0;
  }
  else
  {
    NewMode = 0;
    if ( dwNewMode & 1 )
      NewMode = 16;
    if ( dwNewMode & 2 )
      NewMode |= 0x20u;
    if ( dwNewMode & 0x8000 )
      NewMode |= 0x40u;
    status = RtlSetThreadErrorMode(NewMode, &dwNewMode);
    if ( status >= 0 )
    {
      if ( lpOldMode )
      {
        error = 0;
        if ( dwNewMode & 0x10 )
          error = 1;
        if ( dwNewMode & 0x20 )
          error |= 2u;
        if ( dwNewMode & 0x40 )
          error |= 0x8000u;
        *lpOldMode = error;
      }
      result = 1;
    }
    else
    {
      BaseSetLastNTError(status);
      result = 0;
    }
  }
  return result;
}

BOOL WINAPI SetThreadpoolStackInformation(PTP_POOL ptpp, PTP_POOL_STACK_INFORMATION ptpsi)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = TpSetPoolStackInformation(ptpp, ptpsi);
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

BOOL WINAPI SetThreadIdealProcessorEx(HANDLE ThreadHandle, PPROCESSOR_NUMBER ThreadInformation, PPROCESSOR_NUMBER lpPreviousIdealProcessor)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  ThreadInformation = *(PPROCESSOR_NUMBER *)&ThreadInformation->Group;
  status = NtSetInformationThread(ThreadHandle, (THREADINFOCLASS)0x21u, &ThreadInformation, 4u);
  if ( status >= 0 )
  {
    if ( lpPreviousIdealProcessor )
      lpPreviousIdealProcessor->Group = ThreadInformation->Group;
    result = 1;
  }
  else
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  return result;
}

BOOL WINAPI SetThreadGroupAffinity(HANDLE ThreadHandle, const GROUP_AFFINITY *GroupAffinity, PGROUP_AFFINITY PreviousGroupAffinity)
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
        status < 0)
    || (status = NtSetInformationThread(ThreadHandle, MaxThreadInfoClass|ThreadPriorityBoost, GroupAffinity, 0xCu),
        status < 0) )
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  else
  {
    if ( PreviousGroupAffinity )
    {
      PreviousGroupAffinity->Mask = (KAFFINITY)ThreadInformation;
      PreviousGroupAffinity->Group = one;
      PreviousGroupAffinity->Reserved[1] = two;
    }
    result = 1;
  }
  return result;
}

DWORD WINAPI ReadThreadProfilingData(HANDLE PerformanceDataHandle, DWORD Flags, PPERFORMANCE_DATA PerformanceData)
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

BOOL WINAPI QueryThreadpoolStackInformation(PTP_POOL ptpp, PTP_POOL_STACK_INFORMATION ptpsi)
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

DWORD WINAPI QueryThreadProfiling(HANDLE HANDLE, PBOOLEAN Enabled)
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

BOOL WINAPI GetThreadIdealProcessorEx(HANDLE ThreadHandle, PPROCESSOR_NUMBER ThreadInformation)
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
    return 1;
  }
  status = NtQueryInformationThread(ThreadHandle, (THREADINFOCLASS)33u, ThreadInformation, 4u, 0);
  if ( status >= 0 )
    return 1;
  BaseSetLastNTError(status);
  return 0;
}

BOOL WINAPI GetThreadGroupAffinity(HANDLE ThreadHandle, PGROUP_AFFINITY ThreadInformation)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = NtQueryInformationThread(ThreadHandle, MaxThreadInfoClass|ThreadPriorityBoost, ThreadInformation, 0xCu, 0);
  if ( status >= 0 )
  {
    result = 1;
  }
  else
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  return result;
}

DWORD WINAPI DisableThreadProfiling(HANDLE PerformanceDataHandle)
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

DWORD WINAPI GetThreadErrorMode()
{
  ULONG error; // ecx@1
  DWORD result; // eax@1

  error = RtlGetThreadErrorMode();
  result = 0;
  if ( error & 0x10 )
    result = 1;
  if ( error & 0x20 )
    result |= 2u;
  if ( error & 0x40 )
    result |= 0x8000u;
  return result;
}

HANDLE WINAPI CreateRemoteThreadEx(
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

DWORD WINAPI EnableThreadProfiling(HANDLE ThreadHandle, DWORD Flags, DWORD64 HardwareCounters, HANDLE PerformanceDataHandle)
{
  NTSTATUS status; // eax@1
  DWORD result; // eax@2

  status = RtlEnableThreadProfiling(
             ThreadHandle,
             Flags,
             HardwareCounters,
             PerformanceDataHandle);
  if ( status >= 0 )
    result = 0;
  else
    result = RtlNtStatusToDosErrorNoTeb(status);
  return result;
}

BOOL WINAPI GetThreadInformation(HANDLE ProcessHandle, THREADINFOCLASS ProcessInformationClass, PVOID ProcessInformation, DWORD ProcessInformationLength)
{
  BOOL resp; // esi@2
  NTSTATUS status; // eax@3

  if ( ProcessInformationClass )
  {
    BaseSetLastNTError(0xC000000Du);
    resp = 0;
  }
  else
  {
    resp = 0;
    status = NtQueryInformationProcess(
               ProcessHandle,
               ProcessDebugPort|0x20,
               ProcessInformation,
               ProcessInformationLength,
               0);
    if ( status >= 0 )
      resp = 1;
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
      BaseSetLastNTError(0xC000000Du);
      return 0;
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
    result = 0;
  }
  else
  {
    result = 1;
  }
  return result;
}

VOID WINAPI GetCurrentThreadStackLimits(PULONG_PTR LowLimit, PULONG_PTR HighLimit)
{
  PULONG_PTR numberOne; // edx@1
  
  #ifdef _M_IX86
		numberOne = (PULONG_PTR)__readfsdword(24);;
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