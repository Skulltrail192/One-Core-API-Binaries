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

/* helper for kernel32->ntdll timeout format conversion */
static inline PLARGE_INTEGER get_nt_timeout( PLARGE_INTEGER pTime, DWORD timeout )
{
    if (timeout == INFINITE) return NULL;
    pTime->QuadPart = (ULONGLONG)timeout * -10000;
    return pTime;
}
 
/*
 * @unimplemented
 */
BOOL WINAPI SleepConditionVariableCS(PCONDITION_VARIABLE ConditionVariable, PRTL_CRITICAL_SECTION CriticalSection, DWORD dwMilliseconds)
{
	NTSTATUS status;
    LARGE_INTEGER time;

    status = RtlSleepConditionVariableCS( ConditionVariable, CriticalSection, get_nt_timeout( &time, dwMilliseconds ) );

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
BOOL WINAPI SleepConditionVariableSRW(PRTL_CONDITION_VARIABLE ConditionVariable, PRTL_SRWLOCK SRWLock, DWORD dwMilliseconds, ULONG Flags)
{
	NTSTATUS status;
    LARGE_INTEGER time;

    status = RtlSleepConditionVariableSRW( ConditionVariable, SRWLock, get_nt_timeout( &time, dwMilliseconds ), Flags );

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
BOOL WINAPI InitOnceBeginInitialize( INIT_ONCE *once, DWORD flags, BOOL *pending, void **context )
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
BOOL WINAPI InitOnceComplete( INIT_ONCE *once, DWORD flags, void *context )
{
     NTSTATUS status = RtlRunOnceComplete( once, flags, context );
     if (status != STATUS_SUCCESS) SetLastError( RtlNtStatusToDosError(status) );
     return !status;
}

/***********************************************************************
  *           InitOnceExecuteOnce    (KERNEL32.@)
  */
BOOL WINAPI InitOnceExecuteOnce( INIT_ONCE *once, PINIT_ONCE_FN func, void *param, void **context )
{
    return !RtlRunOnceExecuteOnce( once, (PRTL_RUN_ONCE_INIT_FN)func, param, context );
}

/***********************************************************************
  *           InitOnceInitialize    (KERNEL32.@)
  */
VOID WINAPI InitOnceInitialize(
  _Out_  PINIT_ONCE InitOnce
)
{
	 InitOnce->Ptr = NULL;
}

/*
 * @implemented - need test
 */
HANDLE WINAPI CreateWaitableTimerExW(LPSECURITY_ATTRIBUTES atributes, PCWSTR SourceString, PHANDLE comparation, ACCESS_MASK access)
{
  PHANDLE receive; // esi@1
  NTSTATUS status  = -1; // eax@4
  LSA_UNICODE_STRING *unicode; // [sp-8h] [bp-2Ch]@3
  NTSTATUS receiveStatus; // [sp-4h] [bp-28h]@11
  PCWSTR *string; // [sp-4h] [bp-28h]@3
  DWORD error; // [sp-4h] [bp-28h]@7
  POBJECT_ATTRIBUTES obj = NULL;
  LSA_UNICODE_STRING DestinationString; // [sp+1Ch] [bp-8h]@10

  receive = comparation;
  if ( (unsigned int)comparation & 0xFFFFFFFE )
  {
    receiveStatus = 0xC00000F1u;
    goto LABEL_14;
  }
  if ( SourceString )
  {
    RtlInitUnicodeString(&DestinationString, SourceString);
    string = &SourceString;
    unicode = &DestinationString;
  }
  else
  {
    string = &SourceString;
    unicode = NULL;
  }
  status = (NTSTATUS)BaseFormatObjectAttributes(obj,atributes, unicode);
  if ( status < 0 || (status = NtCreateTimer(comparation, access, (POBJECT_ATTRIBUTES)SourceString, (int)receive & 1), status < 0) )
  {
    receiveStatus = status;
LABEL_14:
    SetLastError(receiveStatus);
    return 0;
  }
  if ( status == 1073741824 )
    error = 183;
  else
    error = 0;
  SetLastError(error);
  return comparation;
}

HANDLE WINAPI CreateWaitableTimerExA(LPSECURITY_ATTRIBUTES atributes, PCSTR SourceString, PHANDLE Handle, ACCESS_MASK mask)
{
  PCWSTR string; // esi@1
  PUNICODE_STRING unicode; // esi@4
  NTSTATUS status; // eax@4
  STRING DestinationString; // [sp+4h] [bp-8h]@4

  string = NULL;
  if ( !SourceString )
    return CreateWaitableTimerExW(atributes, string, Handle, mask);
  #ifdef _M_IX86
		unicode = (PUNICODE_STRING)(__readfsdword(24) + 3064);
  #elif defined(_M_AMD64)
		unicode = (PUNICODE_STRING)(__readgsqword(24) + 3064);
  #endif
  RtlInitAnsiString(&DestinationString, SourceString);
  status = RtlAnsiStringToUnicodeString(unicode, &DestinationString, 0);
  if ( status >= 0 )
  {
    string = unicode->Buffer;
    return CreateWaitableTimerExW(atributes, string, Handle, mask);
  }
  if ( status == 0x80000005 )
    SetLastError(0xCEu);
  else
    SetLastError(status);
  return 0;
}

/*
 * @implemented - need test
 */
HANDLE WINAPI CreateSemaphoreExW(LPSECURITY_ATTRIBUTES atributes, LONG InitialCount, LONG MaximumCount, PCWSTR ObjectAttributes, HANDLE SemaphoreHandle, ACCESS_MASK DesiredAccess)
{
  NTSTATUS status = -1; // eax@4
  LSA_UNICODE_STRING *unicode; // [sp-8h] [bp-28h]@3
  NTSTATUS otherStatus; // [sp-4h] [bp-24h]@11
  PCWSTR *string; // [sp-4h] [bp-24h]@3
  DWORD error; // [sp-4h] [bp-24h]@7
  LSA_UNICODE_STRING DestinationString; // [sp+18h] [bp-8h]@10

  if ( SemaphoreHandle )
  {
    otherStatus = 0xC00000F3u;
    goto LABEL_14;
  }
  if ( ObjectAttributes )
  {
    RtlInitUnicodeString(&DestinationString, ObjectAttributes);
    string = &ObjectAttributes;
    unicode = &DestinationString;
  }
  else
  {
    string = &ObjectAttributes;
    unicode = NULL;
  }
  if ( status < 0
    || (status = NtCreateSemaphore(
                   &SemaphoreHandle,
                   DesiredAccess,
                   (POBJECT_ATTRIBUTES)ObjectAttributes,
                   InitialCount,
                   MaximumCount),
        status < 0) )
  {
    otherStatus = status;
LABEL_14:
    SetLastError(otherStatus);
    return 0;
  }
  if ( status == 0x40000000 )
    error = 183;
  else
    error = 0;
  SetLastError(error);
  return SemaphoreHandle;
}

/*
 * @implemented - need test
 */
HANDLE WINAPI CreateSemaphoreExA(LPSECURITY_ATTRIBUTES atributes, LONG InitialCount, LONG MaximumCount, PCSTR SourceString, HANDLE SemaphoreHandle, ACCESS_MASK DesiredAccess)
{
  PUNICODE_STRING unicode; // esi@4
  NTSTATUS status; // eax@4
  PCWSTR string; // esi@1
  STRING DestinationString; // [sp+4h] [bp-8h]@4

  string = NULL;
  if ( !SourceString )
    return CreateSemaphoreExW(atributes, InitialCount, MaximumCount, string, SemaphoreHandle, DesiredAccess);
  #ifdef _M_IX86
		unicode = (PUNICODE_STRING)(__readfsdword(24) + 3064);
  #elif defined(_M_AMD64)
		unicode = (PUNICODE_STRING)(__readgsqword(24) + 3064);
  #endif
  RtlInitAnsiString(&DestinationString, SourceString);
  status = RtlAnsiStringToUnicodeString(unicode, &DestinationString, 0);
  if ( status >= 0 )
  {
    string = unicode->Buffer;
    return CreateSemaphoreExW(atributes, InitialCount, MaximumCount, string, SemaphoreHandle, DesiredAccess);
  }
  if ( status == 0x80000005 )
    SetLastError(0xCEu);
  else
    SetLastError(status);
  return 0;
} 

/*
 * @implemented - need test
 */
HANDLE WINAPI CreateMutexExW(LPSECURITY_ATTRIBUTES atributes, PCWSTR ObjectAttributes, DWORD MutantHandle, ACCESS_MASK DesiredAccess)
{
  DWORD other; // ebx@1
  NTSTATUS mutant = -1; // eax@4
  LSA_UNICODE_STRING *unicode; // [sp-8h] [bp-2Ch]@3
  NTSTATUS status; // [sp-4h] [bp-28h]@11
  PCWSTR *string; // [sp-4h] [bp-28h]@3
  DWORD error; // [sp-4h] [bp-28h]@7
  LSA_UNICODE_STRING DestinationString; // [sp+1Ch] [bp-8h]@3

  other = MutantHandle;
  if ( MutantHandle & 0xFFFFFFFE )
  {
    status = 0xC00000F1u;
    goto LABEL_12;
  }
  if ( ObjectAttributes )
  {
    RtlInitUnicodeString(&DestinationString, ObjectAttributes);
    string = &ObjectAttributes;
    unicode = &DestinationString;
  }
  else
  {
    string = &ObjectAttributes;
    unicode = NULL;
  }
  if ( mutant < 0
    || (mutant = NtCreateMutant((PHANDLE)&MutantHandle, DesiredAccess, (POBJECT_ATTRIBUTES)ObjectAttributes, other & 1),
        mutant < 0) )
  {
    status = mutant;
LABEL_12:
    SetLastError(status);
    return 0;
  }
  if ( mutant == 1073741824 )
    error = 183;
  else
    error = 0;
  SetLastError(error);
  return (HANDLE)MutantHandle;
}

/*
 * @implemented - need test
 */
HANDLE WINAPI CreateMutexExA(LPSECURITY_ATTRIBUTES atributes, PCSTR SourceString, DWORD MutantHandle, ACCESS_MASK DesiredAccess)
{
  PCWSTR other; // esi@1
  PUNICODE_STRING unicode; // esi@2
  NTSTATUS status; // eax@2
  STRING DestinationString; // [sp+4h] [bp-8h]@2

  other = NULL;
  if ( !SourceString )
    return CreateMutexExW(atributes, other, MutantHandle, DesiredAccess);
  #ifdef _M_IX86
		unicode = (PUNICODE_STRING)(__readfsdword(24) + 3064);
  #elif defined(_M_AMD64)
		unicode = (PUNICODE_STRING)(__readgsqword(24) + 3064);
  #endif
  RtlInitAnsiString(&DestinationString, SourceString);
  status = RtlAnsiStringToUnicodeString(unicode, &DestinationString, 0);
  if ( status >= 0 )
  {
    other = unicode->Buffer;
    return CreateMutexExW(atributes, other, MutantHandle, DesiredAccess);
  }
  if ( status == 0x80000005 )
    SetLastError(0xCEu);
  else
    SetLastError(status);
  return 0;
}

/*
 * @implemented - need test
 */
HANDLE WINAPI CreateEventExW(PSECURITY_ATTRIBUTES security, PCWSTR ObjectAttributes, PHANDLE EventHandle, ACCESS_MASK DesiredAccess)
{
  PHANDLE handle; // esi@1
  NTSTATUS resp = 1; // eax@4
  LSA_UNICODE_STRING *string; // [sp-8h] [bp-2Ch]@3
  NTSTATUS status; // [sp-4h] [bp-28h]@10
  PCWSTR *atributes; // [sp-4h] [bp-28h]@3
  DWORD error; // [sp-4h] [bp-28h]@7
  LSA_UNICODE_STRING DestinationString; // [sp+1Ch] [bp-8h]@13

  handle = EventHandle;
  if ( (unsigned int)EventHandle & 0xFFFFFFFC )
  {
    status = 0xC00000F1u;
    goto LABEL_11;
  }
  if ( ObjectAttributes )
  {
    RtlInitUnicodeString(&DestinationString, ObjectAttributes);
    atributes = &ObjectAttributes;
    string = &DestinationString;
  }
  else
  {
    atributes = &ObjectAttributes;
    string = NULL;
  }
  if ( resp < 0
    || (resp = NtCreateEvent(
                 EventHandle,
                 DesiredAccess,
                 (POBJECT_ATTRIBUTES)ObjectAttributes,
                 ((int)handle & 1),
                 ((unsigned int)handle >> 1) & 1),
        resp < 0) )
  {
    status = resp;
LABEL_11:
   SetLastError(status);
    return 0;
  }
  if ( resp == 0x40000000 )
    error = 183;
  else
    error = 0;
  SetLastError(error);
  return EventHandle;
}

/*
 * @implemented - need test
 */
HANDLE WINAPI CreateEventExA(PSECURITY_ATTRIBUTES atributes, PCSTR SourceString, HANDLE *EventHandle, ACCESS_MASK DesiredAccess)
{
  PUNICODE_STRING unicode; // esi@4
  NTSTATUS status; // eax@4
  PCWSTR string; // esi@1
  STRING DestinationString; // [sp+4h] [bp-8h]@4

  string = NULL;
  if ( !SourceString )
    return CreateEventExW(atributes, string, EventHandle, DesiredAccess);
  #ifdef _M_IX86
		unicode = (PUNICODE_STRING)(__readfsdword(24) + 3064);
  #elif defined(_M_AMD64)
		unicode = (PUNICODE_STRING)(__readgsqword(24) + 3064);
  #endif
  RtlInitAnsiString(&DestinationString, SourceString);
  status = RtlAnsiStringToUnicodeString(unicode, &DestinationString, 0);
  if ( status >= 0 )
  {
    string = unicode->Buffer;
    return CreateEventExW(atributes, string, EventHandle, DesiredAccess);
  }
  if ( status == 0x80000005 )
    SetLastError(206u);
  else
    SetLastError(status);
  return 0;
}

/*
 * @implemented
 */
BOOL WINAPI InitializeCriticalSectionEx(OUT LPCRITICAL_SECTION lpCriticalSection,
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

MMRESULT WINAPI timeBeginPeriod(UINT uPeriod)
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