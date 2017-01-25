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
#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

HANDLE event = NULL;

HANDLE keyed_event = NULL;

/* helper for kernel32->ntdll timeout format conversion */
static inline PLARGE_INTEGER get_nt_timeout( PLARGE_INTEGER pTime, DWORD timeout )
{
    if (timeout == INFINITE) return NULL;
    pTime->QuadPart = (ULONGLONG)timeout * -10000;
    return pTime;
}

static inline int interlocked_dec_if_nonzero( int *dest )
{
     int val, tmp;
     for (val = *dest;; val = tmp)
     {
         if (!val || (tmp = interlocked_cmpxchg( dest, val - 1, val )) == val)
             break;
     }
     return val;
}
 
/*
 * @unimplemented
 */
/***********************************************************************
 *           SleepConditionVariableCS   (KERNEL32.@)
*/
BOOL 
WINAPI 
SleepConditionVariableCS(
	PCONDITION_VARIABLE ConditionVariable, 
	PCRITICAL_SECTION CriticalSection, 
	DWORD dwMilliseconds
)
{
	NTSTATUS status;
    LARGE_INTEGER time;

    status = RtlSleepConditionVariableCS( ConditionVariable, CriticalSection, get_nt_timeout( &time, dwMilliseconds ) );
	
	DbgPrint("Function: RtlSleepConditionVariableCS. From ntdll function: RtlSleepConditionVariableCS. Status: %08x\n",status);

    if (status != STATUS_SUCCESS)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }
    return TRUE;
} 

/*
 * @implemented - need test - used function from ntdllnew
 */
BOOL 
WINAPI 
SleepConditionVariableSRW(
	PRTL_CONDITION_VARIABLE ConditionVariable, 
	PRTL_SRWLOCK SRWLock, 
	DWORD dwMilliseconds, 
	ULONG Flags
)
{
	NTSTATUS status;
    LARGE_INTEGER time;

    status = RtlSleepConditionVariableSRW( ConditionVariable, SRWLock, get_nt_timeout( &time, dwMilliseconds ), Flags );
	
    DbgPrint("Function: SleepConditionVariableSRW. From ntdll function: RtlSleepConditionVariableSRW. Status: %08x\n",status);
    if (status != STATUS_SUCCESS)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }
    return TRUE;
}

/*
 * @implemented - need test
 */
/***********************************************************************
*           InitOnceBeginInitialize    (KERNEL32.@)
*/
BOOL 
WINAPI 
InitOnceBeginInitialize( 
	INIT_ONCE *once, 
	DWORD flags, 
	BOOL *pending, 
	void **context 
)
{
     NTSTATUS status = RtlRunOnceBeginInitialize( once, flags, context );
     if (status >= 0) 
	 {
		*pending = (status == STATUS_PENDING);
	 }else{ 
		SetLastError( RtlNtStatusToDosError(status) );
	 }
     return status >= 0;
}

/*
 * @implemented - need test
 */
/***********************************************************************
  *           InitOnceComplete    (KERNEL32.@)
  */
BOOL 
WINAPI 
InitOnceComplete( 
	INIT_ONCE *once, 
	DWORD flags, 
	void *context 
)
{
     NTSTATUS status = RtlRunOnceComplete( once, flags, context );
     if (status != STATUS_SUCCESS) SetLastError( RtlNtStatusToDosError(status) );
     return !status;
}

/***********************************************************************
  *           InitOnceExecuteOnce    (KERNEL32.@)
  */
BOOL 
WINAPI 
InitOnceExecuteOnce( 
	INIT_ONCE *once, 
	PINIT_ONCE_FN func, 
	void *param, 
	void **context 
)
{
    return !RtlRunOnceExecuteOnce( once, (PRTL_RUN_ONCE_INIT_FN)func, param, context );
}

/*
 * @implemented - need test
 */
HANDLE 
WINAPI 
CreateWaitableTimerExW(LPSECURITY_ATTRIBUTES lpTimerAttributes, LPCWSTR lpTimerName, DWORD dwFlags, DWORD dwDesiredAccess)
{
  NTSTATUS Status = STATUS_SUCCESS; // eax@3
  POBJECT_ATTRIBUTES Obj = NULL; // [sp+4h] [bp-20h]@3
  LSA_UNICODE_STRING DestinationString; // [sp+1Ch] [bp-8h]@10
  HANDLE Timer;

  if ( dwFlags & 0xFFFFFFFE )
  {
    BaseSetLastNTError(STATUS_INVALID_PARAMETER_3);
    return NULL;
  }
  if ( lpTimerName )
  {
    RtlInitUnicodeString(&DestinationString, (PCWSTR)lpTimerName);
    Obj = BaseFormatObjectAttributes(Obj, lpTimerAttributes, &DestinationString);
  }
  else
  {
    Obj = BaseFormatObjectAttributes(Obj, lpTimerAttributes, 0);
  }
  if ( Status < 0
    || (Status = NtCreateTimer(&Timer, dwDesiredAccess, lpTimerName, ~dwFlags & 1), Status < 0) )
  {
    BaseSetLastNTError(Status);
    return NULL;
  }
  if ( Status == STATUS_OBJECT_NAME_EXISTS )
    SetLastError(0xB7u);
  else
    SetLastError(0);
  return Timer;
}

HANDLE 
WINAPI 
CreateWaitableTimerExA(
	LPSECURITY_ATTRIBUTES lpTimerAttributes, 
	LPCTSTR lpTimerName, 
	DWORD dwFlags, 
	DWORD dwDesiredAccess
)
{
  PWSTR TimerName; // esi@1
  PUNICODE_STRING Unicode; // esi@4
  NTSTATUS Status; // eax@4
  STRING DestinationString; // [sp+4h] [bp-8h]@4

  TimerName = L"";
  if ( !lpTimerName )
    return CreateWaitableTimerExW(lpTimerAttributes, TimerName, dwFlags, dwDesiredAccess);
  Unicode = &NtCurrentTeb()->StaticUnicodeString;
  RtlInitAnsiString(&DestinationString, lpTimerName);
  Status = RtlAnsiStringToUnicodeString(Unicode, &DestinationString, 0);
  if ( Status >= STATUS_SUCCESS )
  {
    TimerName = Unicode->Buffer;
    return CreateWaitableTimerExW(lpTimerAttributes, TimerName, dwFlags, dwDesiredAccess);
  }
  if ( Status == STATUS_BUFFER_OVERFLOW )
    SetLastError(0xCEu);
  else
    BaseSetLastNTError(Status);
  return NULL;
}

/*
 * @implemented - need test
 */
HANDLE 
WINAPI 
CreateSemaphoreExW(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG InitialCount, LONG MaximumCount, LPCWSTR lpName, DWORD dwFlags, DWORD DesiredAccess)
{
  OBJECT_ATTRIBUTES *localAttributes; // ecx@4
  NTSTATUS status; // eax@5
  BOOL verification; // ecx@13
  DWORD error; // [sp-4h] [bp-34h]@9
  NTSTATUS respStatus; // [sp-4h] [bp-34h]@17
  OBJECT_ATTRIBUTES *attributes; // [sp+8h] [bp-28h]@12
  HANDLE SemaphoreHandle; // [sp+Ch] [bp-24h]@7
  LSA_UNICODE_STRING DestinationString; // [sp+10h] [bp-20h]@12
  OBJECT_ATTRIBUTES localObject; // [sp+18h] [bp-18h]@12

  if ( dwFlags )
  {
    respStatus = STATUS_INVALID_PARAMETER_5;
    goto LasNttError;
  }
  if ( lpName )
  {
    RtlInitUnicodeString(&DestinationString, lpName);
    attributes = BaseFormatObjectAttributes(&localObject, lpSemaphoreAttributes, &DestinationString);
    localAttributes = attributes;
  }
  else
  {
    if ( lpSemaphoreAttributes )
    {
      verification = lpSemaphoreAttributes->bInheritHandle;
      localObject.SecurityDescriptor = lpSemaphoreAttributes->lpSecurityDescriptor;
      localObject.Length = 24;
      localObject.RootDirectory = 0;
      localObject.Attributes = verification != 0 ? 2 : 0;
      localObject.ObjectName = 0;
      localObject.SecurityQualityOfService = 0;
      localAttributes = &localObject;
    }
    else
    {
      localAttributes = 0;
    }
    status = 0;
  }
  if ( status < 0
    || (status = NtCreateSemaphore(&SemaphoreHandle, DesiredAccess, localAttributes, InitialCount, MaximumCount),
        status < 0) )
  {
    respStatus = status;
LasNttError:
    BaseSetLastNTError(respStatus);
    return 0;
  }
  if ( status == STATUS_OBJECT_NAME_EXISTS )
    error = 183;
  else
    error = 0;
  RtlSetLastWin32Error(error);
  return SemaphoreHandle;
}

/*
 * @implemented - need test
 */
HANDLE 
WINAPI 
CreateSemaphoreExA(
	LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, 
	LONG lInitialCount, 
	LONG lMaximumCount, 
	LPCTSTR lpName, 
	DWORD dwFlags,
	DWORD dwDesiredAccess
)
{
  LPCWSTR localString; // eax@2
  HANDLE resultHandle; // edi@3
  LSA_UNICODE_STRING UnicodeString; // [sp+4h] [bp-8h]@2

  if ( lpName )
  {
    if ( !Basep8BitStringToDynamicUnicodeString(&UnicodeString, lpName) )
      return 0;
    localString = UnicodeString.Buffer;
  }
  else
  {
    localString = 0;
    UnicodeString.Buffer = 0;
  }
  resultHandle = CreateSemaphoreExW(
                   lpSemaphoreAttributes,
                   lInitialCount,
                   lMaximumCount,
                   localString,
                   dwFlags,
                   dwDesiredAccess);
  if ( lpName )
    RtlFreeUnicodeString(&UnicodeString);
  return resultHandle;
}

/*
 * @implemented - need test
 */
HANDLE 
WINAPI 
CreateMutexExW(
	LPSECURITY_ATTRIBUTES lpMutexAttributes, 
	LPCWSTR lpName, 
	DWORD dwFlags, 
	DWORD DesiredAccess)
{
  NTSTATUS status; // eax@4
  LSA_UNICODE_STRING *lsaAttributes; // [sp-8h] [bp-38h]@3
  POBJECT_ATTRIBUTES objAttributes; // [sp-4h] [bp-34h]@3
  DWORD error; // [sp-4h] [bp-34h]@7
  NTSTATUS otherStatus; // [sp-4h] [bp-34h]@12
  POBJECT_ATTRIBUTES ObjectAttributes; // [sp+8h] [bp-28h]@3
  HANDLE MutantHandle; // [sp+Ch] [bp-24h]@5
  LSA_UNICODE_STRING DestinationString; // [sp+10h] [bp-20h]@3
  OBJECT_ATTRIBUTES localAttributes; // [sp+18h] [bp-18h]@4

  if ( dwFlags & 0xFFFFFFFE )
  {
    otherStatus = STATUS_INVALID_PARAMETER_3;
    goto LastNtError;
  }
  if ( lpName )
  {
    RtlInitUnicodeString(&DestinationString, lpName);
    lsaAttributes = &DestinationString;
  }
  else
  {
    lsaAttributes = 0;
  }
  objAttributes = BaseFormatObjectAttributes(&localAttributes, lpMutexAttributes, lsaAttributes);
  if ( (status = NtCreateMutant(&MutantHandle, DesiredAccess, objAttributes, dwFlags & 1), status < 0) )
  {
    otherStatus = status;
LastNtError:
    BaseSetLastNTError(otherStatus);
    return 0;
  }
  if ( status == STATUS_OBJECT_NAME_EXISTS )
    error = 183;
  else
    error = 0;
  RtlSetLastWin32Error(error);
  return MutantHandle;
}

/*
 * @implemented - need test
 */
HANDLE 
WINAPI 
CreateMutexExA(LPSECURITY_ATTRIBUTES lpMutexAttributes, LPCTSTR lpName, DWORD dwFlags, DWORD DesiredAccess)
{
  BOOL result; // eax@2
  LPCWSTR localWstring; // eax@3
  HANDLE respHandle; // edi@5
  LSA_UNICODE_STRING UnicodeString; // [sp+4h] [bp-8h]@2

  if ( lpName )
  {
    result = Basep8BitStringToDynamicUnicodeString(&UnicodeString, lpName);
    if ( !result )
      return respHandle = NULL;
    localWstring = UnicodeString.Buffer;
  }
  else
  {
    localWstring = 0;
    UnicodeString.Buffer = 0;
  }
  respHandle = CreateMutexExW(lpMutexAttributes, localWstring, dwFlags, DesiredAccess);
  if ( lpName )
    RtlFreeUnicodeString(&UnicodeString);
  return respHandle;
}

/*
 * @implemented - need test
 */
HANDLE WINAPI CreateEventExW(LPSECURITY_ATTRIBUTES lpEventAttributes, PCWSTR SourceString, DWORD dwFlags, DWORD DesiredAccess)
{
  OBJECT_ATTRIBUTES *obj; // edx@3
  NTSTATUS status; // eax@3
  DWORD error; // [sp-4h] [bp-34h]@7
  NTSTATUS otherStatus; // [sp-4h] [bp-34h]@10
  POBJECT_ATTRIBUTES otherObj; // [sp+8h] [bp-28h]@11
  HANDLE EventHandle; // [sp+Ch] [bp-24h]@5
  LSA_UNICODE_STRING DestinationString; // [sp+10h] [bp-20h]@11
  OBJECT_ATTRIBUTES localObj; // [sp+18h] [bp-18h]@11

  if ( dwFlags & 0xFFFFFFFC )
  {
    otherStatus = STATUS_INVALID_PARAMETER_3;
    goto LastNtError;
  }
  if ( SourceString )
  {
    RtlInitUnicodeString(&DestinationString, SourceString);
    otherObj = BaseFormatObjectAttributes(&localObj, lpEventAttributes, &DestinationString);
    obj = otherObj;
  }
  else
  {
    obj = 0;
    status = 0;
  }
  if ( status < 0
    || (status = NtCreateEvent(&EventHandle, DesiredAccess, obj, (~dwFlags & 1), (dwFlags >> 1) & 1), status < 0) )
  {
    otherStatus = status;
LastNtError:
    BaseSetLastNTError(otherStatus);
    return 0;
  }
  if ( status == STATUS_OBJECT_NAME_EXISTS )
    error = 183;
  else
    error = 0;
  RtlSetLastWin32Error(error);
  return EventHandle;
}
/*
 * @implemented - need test
 */
HANDLE WINAPI CreateEventExA(LPSECURITY_ATTRIBUTES lpEventAttributes, LPCTSTR lpName, DWORD dwFlags, DWORD DesiredAccess)
{
  LPCWSTR localString; // eax@2
  HANDLE receiveHandle; // edi@3
  HANDLE result; // eax@4
  LSA_UNICODE_STRING UnicodeString; // [sp+8h] [bp-8h]@2

  if ( lpName )
  {
    result = (HANDLE)Basep8BitStringToDynamicUnicodeString(&UnicodeString, lpName);
    if ( !result )
      return result;
    localString = UnicodeString.Buffer;
  }
  else
  {
    localString = 0;
    UnicodeString.Buffer = 0;
  }
  receiveHandle = CreateEventExW(lpEventAttributes, localString, dwFlags, DesiredAccess);
  if ( lpName )
    RtlFreeUnicodeString(&UnicodeString);
  return receiveHandle;
}

/*
 * @implemented
 */
BOOL 
WINAPI 
InitializeCriticalSectionEx(OUT LPCRITICAL_SECTION lpCriticalSection,
                                       IN DWORD dwSpinCount,
                                       IN DWORD flags )
{
    NTSTATUS Status;

    /* FIXME: Flags ignored */

    /* Initialize the critical section */
    Status = RtlInitializeCriticalSectionAndSpinCount(
        (PRTL_CRITICAL_SECTION)lpCriticalSection,
        dwSpinCount);
    if (!NT_SUCCESS(Status))
    {
        /* Set failure code */
        SetLastError(Status);
        return FALSE;
    }

    /* Success */
    return TRUE;
} 

BOOL WINAPI SetWaitableTimerEx(
  _In_  HANDLE hTimer,
  _In_  const LARGE_INTEGER *lpDueTime,
  _In_  LONG lPeriod,
  _In_  PTIMERAPCROUTINE pfnCompletionRoutine,
  _In_  LPVOID lpArgToCompletionRoutine,
  _In_  PREASON_CONTEXT WakeContext,
  _In_  ULONG TolerableDelay
)
{
	return SetWaitableTimer(hTimer, lpDueTime, lPeriod, pfnCompletionRoutine, lpArgToCompletionRoutine, TRUE);
}

MMRESULT 
WINAPI 
timeBeginPeriod(UINT uPeriod)
{
  MMRESULT resp; // edi@1
  UINT localPeriod; // esi@2
  __int16 resultCount; // ax@4
  __int16 count; // ax@5
  ULONG time; // eax@7
  MMRESULT result; // eax@10

  resp = 0;
  if ( InitOnceExecuteOnce(g_InitOnce, TimeInit, 0, 0) )
  {
    localPeriod = uPeriod;
    if ( uPeriod < TDD_MAXRESOLUTION )
    {
      result = 97;
    }
    else
    {
      if ( uPeriod >= globalPeriod )
      {
        result = 0;
      }
      else
      {
        RtlAcquireSRWLockExclusive(lock);
        resultCount = vector[localPeriod - TDD_MAXRESOLUTION];
        if ( resultCount == -1 )
        {
          RtlReleaseSRWLockExclusive(lock);
          result = 97;
        }
        else
        {
          count = resultCount + 1;
          vector[localPeriod - TDD_MAXRESOLUTION] = count;
          if ( localPeriod < timerData && count == 1 )
          {
            time = 10000 * localPeriod;
            uPeriod = 10000 * localPeriod;
            if ( 10000 * localPeriod < MinimumTime )
            {
              time = MinimumTime;
              uPeriod = MinimumTime;
            }
            if ( NtSetTimerResolution(time, 1u, (PULONG)&uPeriod) < 0 )
            {
              --vector[localPeriod - TDD_MAXRESOLUTION];
              resp = 97;
            }
            else
            {
              otherTime = localPeriod;
              timerData = (uPeriod + 9900) / 0x2710;
            }
          }
          RtlReleaseSRWLockExclusive(lock);
          result = resp;
        }
      }
    }
  }
  else
  {
    result = 97;
  }
  return result;
}

MMRESULT WINAPI timeEndPeriod(UINT uPeriod)
{
  MMRESULT resp; // edi@1
  UINT compose; // esi@2
  __int16 *localVector; // ecx@4
  __int16 LocalOtherTime; // ax@5
  MMRESULT result; // eax@11
  ULONG ActualResolution; // [sp+4h] [bp-4h]@10

  resp = 0;
  if ( InitOnceExecuteOnce(g_InitOnce, TimeInit, 0, 0) )
  {
    compose = uPeriod;
    if ( uPeriod < TDD_MAXRESOLUTION )
    {
      result = 97;
    }
    else
    {
      if ( uPeriod >= globalPeriod )
      {
        result = 0;
      }
      else
      {
        RtlAcquireSRWLockExclusive(lock);
        localVector = &vector[compose - TDD_MAXRESOLUTION];
        if ( *localVector )
        {
          LocalOtherTime = *localVector - 1;
          *localVector = LocalOtherTime;
          if ( !LocalOtherTime && compose == otherTime )
          {
            while ( compose < globalPeriod && !*localVector )
            {
              ++compose;
              ++localVector;
            }
            NtSetTimerResolution(10000 * timerData, 0, &ActualResolution);
            timerData = globalPeriod;
            otherTime = compose;
            if ( compose < globalPeriod )
            {
              if ( NtSetTimerResolution(10000 * compose, 1u, (PULONG)&uPeriod) >= 0 )
                timerData = (uPeriod + 9999) / 0x2710;
            }
          }
        }
        else
        {
          resp = 97;
        }
        RtlReleaseSRWLockExclusive(lock);
        result = resp;
      }
    }
  }
  else
  {
    result = 97;
  }
  return result;
}

MMRESULT WINAPI timeGetDevCaps(LPTIMECAPS ptc, UINT cbtc)
{
  MMRESULT result; // eax@4

  if ( InitOnceExecuteOnce(g_InitOnce, TimeInit, 0, 0) && cbtc >= 8 && ptc )
  {
    ptc->wPeriodMin = TDD_MAXRESOLUTION;
    ptc->wPeriodMax = 1000000;
    result = 0;
  }
  else
  {
    result = 97;
  }
  return result;
}

/**************************************************************************
 * 				timeGetTime    [MMSYSTEM.607]
 * 				timeGetTime    [WINMM.@]
 */
DWORD WINAPI timeGetTime(void)
{
#if defined(COMMENTOUTPRIORTODELETING)
    DWORD       count;

    /* FIXME: releasing the win16 lock here is a temporary hack (I hope)
     * that lets mciavi.drv run correctly
     */
    if (pFnReleaseThunkLock) pFnReleaseThunkLock(&count);
    if (pFnRestoreThunkLock) pFnRestoreThunkLock(count);
#endif

    return GetTickCount();
}

MMRESULT WINAPI timeGetSystemTime(LPMMTIME pmmt, UINT cbmmt)
{
  MMRESULT result; // eax@2

  if ( cbmmt >= 0xC )
  {
    if ( pmmt )
    {
      pmmt->u.ms = timeGetTime();
      pmmt->wType = 1;
      result = 0;
    }
    else
    {
      result = 129;
    }
  }
  else
  {
    result = 129;
  }
  return result;
}