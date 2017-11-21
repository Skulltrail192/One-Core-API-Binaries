/*++

Copyright (c) 2017  Shorthorn Project

Module Name:

    synch.c

Abstract:

    This module implements all Win32 syncronization
    objects.

Author:

    Skulltrail 16-March-2017

Revision History:

--*/
 
#define NDEBUG

#include <stdio.h>
#include "main.h"
#include <stdlib.h>
#include <malloc.h>

typedef CRITICAL_SECTION pthread_mutex_t;

VOID
NTAPI
RtlReleaseSRWLockExclusive(IN OUT PRTL_SRWLOCK SRWLock);

VOID
NTAPI
RtlAcquireSRWLockExclusive(IN OUT PRTL_SRWLOCK SRWLock);

VOID
NTAPI
RtlReleaseSRWLockShared(IN OUT PRTL_SRWLOCK SRWLock);

VOID
NTAPI
RtlAcquireSRWLockShared(IN OUT PRTL_SRWLOCK SRWLock);

typedef struct win32_cond_t {
    pthread_mutex_t mtx_broadcast;
    pthread_mutex_t mtx_waiter_count;
    volatile int waiter_count;
    HANDLE semaphore;
    HANDLE waiters_done;
    volatile int is_broadcast;
} win32_cond_t;

typedef struct _RW_LOCK {
    CRITICAL_SECTION countsLock;
    CRITICAL_SECTION writerLock;
    HANDLE noReaders;
    int readerCount;
    BOOL waitingWriter;
} RW_LOCK, *PRW_LOCK;

/* returns directory handle to \\BaseNamedObjects */
HANDLE get_BaseNamedObjects_handle(void)
{
    static HANDLE handle = NULL;
    static const WCHAR basenameW[] = {'\\','S','e','s','s','i','o','n','s','\\','%','u',
                                      '\\','B','a','s','e','N','a','m','e','d','O','b','j','e','c','t','s',0};
    WCHAR buffer[64];
    UNICODE_STRING str;
    OBJECT_ATTRIBUTES attr;

    if (!handle)
    {
        HANDLE dir;

        sprintfW( buffer, basenameW, NtCurrentTeb()->ProcessEnvironmentBlock->SessionId );
        RtlInitUnicodeString( &str, buffer );
        InitializeObjectAttributes(&attr, &str, 0, 0, NULL);
        NtOpenDirectoryObject(&dir, DIRECTORY_CREATE_OBJECT|DIRECTORY_TRAVERSE,
                              &attr);
        if (InterlockedCompareExchangePointer( &handle, dir, 0 ) != 0)
        {
            /* someone beat us here... */
            CloseHandle( dir );
        }
    }
    return handle;
}

/*
 * @implemented
 */
BOOL 
WINAPI 
InitializeCriticalSectionEx(
	OUT LPCRITICAL_SECTION lpCriticalSection,
    IN DWORD dwSpinCount,
    IN DWORD flags 
)
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

// void 
// WINAPI 
// InitializeConditionVariable(
	// PCONDITION_VARIABLE cond
// ) 
// {
    // win32_cond_t *win32_cond = NULL;
    // win32_cond = (win32_cond_t *)LocalAlloc(0x40,sizeof(win32_cond_t));
    // if (!win32_cond)
        // return;
    // cond->Ptr = win32_cond;
    // win32_cond->semaphore = CreateSemaphore(NULL, 0, 0x7fffffff, NULL);
    // if (!win32_cond->semaphore)
        // return;
    // win32_cond->waiters_done = CreateEvent(NULL, TRUE, FALSE, NULL);
    // if (!win32_cond->waiters_done)
        // return;
    // InitializeCriticalSection(&win32_cond->mtx_waiter_count);
    // InitializeCriticalSection(&win32_cond->mtx_broadcast);
    // return;
// }

// void 
// WINAPI 
// WakeAllConditionVariable(
	// PCONDITION_VARIABLE cond
// ) 
// {
    // win32_cond_t *win32_cond = (win32_cond_t *)cond->Ptr;
    // int have_waiter;
    // EnterCriticalSection(&win32_cond->mtx_broadcast);
    // EnterCriticalSection(&win32_cond->mtx_waiter_count);
    // have_waiter = 0;
    // if (win32_cond->waiter_count) {
        // win32_cond->is_broadcast = 1;
        // have_waiter = 1;
    // }
    // if (have_waiter) {
        // ReleaseSemaphore(win32_cond->semaphore, win32_cond->waiter_count, NULL);
        // LeaveCriticalSection(&win32_cond->mtx_waiter_count);
        // WaitForSingleObject(win32_cond->waiters_done, INFINITE);
        // ResetEvent(win32_cond->waiters_done);
        // win32_cond->is_broadcast = 0;
    // } else
        // LeaveCriticalSection(&win32_cond->mtx_waiter_count);
    // LeaveCriticalSection(&win32_cond->mtx_broadcast);
// }

// void 
// WINAPI 
// WakeConditionVariable(
	// PCONDITION_VARIABLE cond
// ) 
// {
    // win32_cond_t *win32_cond = (win32_cond_t *)cond->Ptr;
    // int have_waiter;
    // EnterCriticalSection(&win32_cond->mtx_broadcast);
    // EnterCriticalSection(&win32_cond->mtx_waiter_count);
    // have_waiter = win32_cond->waiter_count;
    // LeaveCriticalSection(&win32_cond->mtx_waiter_count);
    // if (have_waiter) {
        // ReleaseSemaphore(win32_cond->semaphore, 1, NULL);
        // WaitForSingleObject(win32_cond->waiters_done, INFINITE);
        // ResetEvent(win32_cond->waiters_done);
    // }
    // LeaveCriticalSection(&win32_cond->mtx_broadcast);
// }

// int 
// WINAPI 
// SleepConditionVariableCS(
	// PCONDITION_VARIABLE cond, 
    // pthread_mutex_t *mutex, 
	// DWORD slp
// )
// {
    // win32_cond_t *win32_cond = (win32_cond_t *)cond->Ptr;
    // int last_waiter;
	// int i; 
    // EnterCriticalSection(&win32_cond->mtx_broadcast);
    // EnterCriticalSection(&win32_cond->mtx_waiter_count);
    // win32_cond->waiter_count++;
    // LeaveCriticalSection(&win32_cond->mtx_waiter_count);
    // LeaveCriticalSection(&win32_cond->mtx_broadcast);

    // LeaveCriticalSection(mutex);
    
	// i = WaitForSingleObject(win32_cond->semaphore, slp);
    // if(i)
        // SetLastError(i);
    // EnterCriticalSection(&win32_cond->mtx_waiter_count);
    // win32_cond->waiter_count--;
    // last_waiter = !win32_cond->waiter_count || !win32_cond->is_broadcast;
    // LeaveCriticalSection(&win32_cond->mtx_waiter_count);
    // if (last_waiter)
        // SetEvent(win32_cond->waiters_done);
    // EnterCriticalSection(mutex);
    // return (BOOL)(i==0);
// }

/***********************************************************************
 *           SleepConditionVariableCS   (KERNEL32.@)
 */
BOOL WINAPI SleepConditionVariableCS( CONDITION_VARIABLE *variable, CRITICAL_SECTION *crit, DWORD timeout )
{
    NTSTATUS status;
    LARGE_INTEGER time;

    status = RtlSleepConditionVariableCS( variable, crit, get_nt_timeout( &time, timeout ) );

    if (status != STATUS_SUCCESS)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }
    return TRUE;
}

/***********************************************************************
 *           SleepConditionVariableSRW   (KERNEL32.@)
 */
BOOL WINAPI SleepConditionVariableSRW( RTL_CONDITION_VARIABLE *variable, RTL_SRWLOCK *lock, DWORD timeout, ULONG flags )
{
    NTSTATUS status;
    LARGE_INTEGER time;

    status = RtlSleepConditionVariableSRW( variable, lock, get_nt_timeout( &time, timeout ), flags );

    if (status != STATUS_SUCCESS)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }
    return TRUE;
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

/***********************************************************************
 *           CreateEventExW    (KERNEL32.@)
 */
HANDLE 
WINAPI 
DECLSPEC_HOTPATCH 
CreateEventExW( 
	SECURITY_ATTRIBUTES *sa, 
	LPCWSTR name, 
	DWORD flags, 
	DWORD access 
)
{
    HANDLE ret = 0;
    UNICODE_STRING nameW;
    OBJECT_ATTRIBUTES attr;
    NTSTATUS status;

    /* one buggy program needs this
     * ("Van Dale Groot woordenboek der Nederlandse taal")
     */
    if (sa && IsBadReadPtr(sa,sizeof(SECURITY_ATTRIBUTES)))
    {
        SetLastError( ERROR_INVALID_PARAMETER);
        return 0;
    }

    attr.Length                   = sizeof(attr);
    attr.RootDirectory            = 0;
    attr.ObjectName               = NULL;
    attr.Attributes               = OBJ_OPENIF | ((sa && sa->bInheritHandle) ? OBJ_INHERIT : 0);
    attr.SecurityDescriptor       = sa ? sa->lpSecurityDescriptor : NULL;
    attr.SecurityQualityOfService = NULL;
    if (name)
    {
        RtlInitUnicodeString( &nameW, name );
        attr.ObjectName = &nameW;
        attr.RootDirectory = get_BaseNamedObjects_handle();
    }

    status = NtCreateEvent( &ret, access, &attr,
                            (flags & CREATE_EVENT_MANUAL_RESET) ? NotificationEvent : SynchronizationEvent,
                            (flags & CREATE_EVENT_INITIAL_SET) != 0 );
    if (status == STATUS_OBJECT_NAME_EXISTS)
        SetLastError( ERROR_ALREADY_EXISTS );
    else
        SetLastError( RtlNtStatusToDosError(status) );
    return ret;
}

/***********************************************************************
 *           CreateEventExA    (KERNEL32.@)
 */
HANDLE 
WINAPI 
DECLSPEC_HOTPATCH 
CreateEventExA( 
	SECURITY_ATTRIBUTES *sa, 
	LPCSTR name, 
	DWORD flags, 
	DWORD access 
)
{
    WCHAR buffer[MAX_PATH];

    if (!name) return CreateEventExW( sa, NULL, flags, access );

    if (!MultiByteToWideChar( CP_ACP, 0, name, -1, buffer, MAX_PATH ))
    {
        SetLastError( ERROR_FILENAME_EXCED_RANGE );
        return 0;
    }
    return CreateEventExW( sa, buffer, flags, access );
}

/***********************************************************************
 *           CreateSemaphoreExW   (KERNEL32.@)
 */
HANDLE 
WINAPI 
DECLSPEC_HOTPATCH 
CreateSemaphoreExW( 
	SECURITY_ATTRIBUTES *sa, 
	LONG initial, 
	LONG max,
    LPCWSTR name, 
	DWORD flags, 
	DWORD access 
)
{
    HANDLE ret = 0;
    UNICODE_STRING nameW;
    OBJECT_ATTRIBUTES attr;
    NTSTATUS status;

    attr.Length                   = sizeof(attr);
    attr.RootDirectory            = 0;
    attr.ObjectName               = NULL;
    attr.Attributes               = OBJ_OPENIF | ((sa && sa->bInheritHandle) ? OBJ_INHERIT : 0);
    attr.SecurityDescriptor       = sa ? sa->lpSecurityDescriptor : NULL;
    attr.SecurityQualityOfService = NULL;
    if (name)
    {
        RtlInitUnicodeString( &nameW, name );
        attr.ObjectName = &nameW;
        attr.RootDirectory = get_BaseNamedObjects_handle();
    }

    status = NtCreateSemaphore( &ret, access, &attr, initial, max );
    if (status == STATUS_OBJECT_NAME_EXISTS)
        SetLastError( ERROR_ALREADY_EXISTS );
    else
        SetLastError( RtlNtStatusToDosError(status) );
    return ret;
}

/***********************************************************************
 *           CreateSemaphoreExA   (KERNEL32.@)
 */
HANDLE 
WINAPI 
DECLSPEC_HOTPATCH 
CreateSemaphoreExA( 
	SECURITY_ATTRIBUTES *sa, 
	LONG initial, 
	LONG max,
    LPCSTR name, 
	DWORD flags, 
	DWORD access 
)
{
    WCHAR buffer[MAX_PATH];

    if (!name) return CreateSemaphoreExW( sa, initial, max, NULL, flags, access );

    if (!MultiByteToWideChar( CP_ACP, 0, name, -1, buffer, MAX_PATH ))
    {
        SetLastError( ERROR_FILENAME_EXCED_RANGE );
        return 0;
    }
    return CreateSemaphoreExW( sa, initial, max, buffer, flags, access );
}

BOOL 
WINAPI 
SetWaitableTimerEx(
  _In_  HANDLE hTimer,
  _In_  const LARGE_INTEGER *lpDueTime,
  _In_  LONG lPeriod,
  _In_  PTIMERAPCROUTINE pfnCompletionRoutine,
  _In_  LPVOID lpArgToCompletionRoutine,
  _In_  PREASON_CONTEXT WakeContext,
  _In_  ULONG TolerableDelay
)
{
	return SetWaitableTimer(hTimer, 
							lpDueTime, 
							lPeriod, 
							pfnCompletionRoutine, 
							lpArgToCompletionRoutine, 
							TRUE);
}

HANDLE
APIENTRY
CreateMutexExW(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    LPCWSTR lpName,
    DWORD                 dwFlags,
    DWORD                 dwDesiredAccess	
)
{
    NTSTATUS Status;
    OBJECT_ATTRIBUTES Obja;
    POBJECT_ATTRIBUTES pObja;
    HANDLE Handle;
    UNICODE_STRING ObjectName;

    if ( ARGUMENT_PRESENT(lpName) ) {
        RtlInitUnicodeString(&ObjectName,lpName);
        pObja = BaseFormatObjectAttributes(&Obja,lpMutexAttributes,&ObjectName);
        }
    else {
        pObja = BaseFormatObjectAttributes(&Obja,lpMutexAttributes,NULL);
        }

    Status = NtCreateMutant(
                &Handle,
                dwDesiredAccess,
                pObja,
                (dwFlags & CREATE_MUTEX_INITIAL_OWNER) != 0
                );

    if ( NT_SUCCESS(Status) ) {
        if ( Status == STATUS_OBJECT_NAME_EXISTS ) {
            SetLastError(ERROR_ALREADY_EXISTS);
            }
        else {
            SetLastError(0);
            }
        return Handle;
        }
    else {
        BaseSetLastNTError(Status);
        return NULL;
        }
}

HANDLE 
WINAPI 
CreateMutexExA( 
	LPSECURITY_ATTRIBUTES sa, 
	LPCSTR name, 
	DWORD flags, 
	DWORD access 
)
{
    ANSI_STRING nameA;
    NTSTATUS status;

    if (ARGUMENT_PRESENT(name)) 
		return CreateMutexExW( sa, NULL, flags, access );

    RtlInitAnsiString( &nameA, name );
    status = RtlAnsiStringToUnicodeString( &NtCurrentTeb()->StaticUnicodeString, &nameA, FALSE );
    if (status != STATUS_SUCCESS)
    {
        SetLastError( ERROR_FILENAME_EXCED_RANGE );
        return NULL;
    }
    return CreateMutexExW( sa, NtCurrentTeb()->StaticUnicodeString.Buffer, flags, access );
}

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
     if (NT_SUCCESS(status)) 
	 {
		*pending = (status == STATUS_PENDING);
	 }else{ 
		SetLastError( RtlNtStatusToDosError(status) );
	 }
     return status >= 0;
}

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
 *           CreateWaitableTimerExW    (KERNEL32.@)
 */
HANDLE WINAPI CreateWaitableTimerExW( SECURITY_ATTRIBUTES *sa, LPCWSTR name, DWORD flags, DWORD access )
{
    HANDLE handle;
    NTSTATUS status;
    UNICODE_STRING nameW;
    OBJECT_ATTRIBUTES attr;

    attr.Length                   = sizeof(attr);
    attr.RootDirectory            = 0;
    attr.ObjectName               = NULL;
    attr.Attributes               = OBJ_OPENIF | ((sa && sa->bInheritHandle) ? OBJ_INHERIT : 0);
    attr.SecurityDescriptor       = sa ? sa->lpSecurityDescriptor : NULL;
    attr.SecurityQualityOfService = NULL;
    if (name)
    {
        RtlInitUnicodeString( &nameW, name );
        attr.ObjectName = &nameW;
        attr.RootDirectory = get_BaseNamedObjects_handle();
    }

    status = NtCreateTimer( &handle, access, &attr,
                 (flags & CREATE_WAITABLE_TIMER_MANUAL_RESET) ? NotificationTimer : SynchronizationTimer );
    if (status == STATUS_OBJECT_NAME_EXISTS)
        SetLastError( ERROR_ALREADY_EXISTS );
    else
        SetLastError( RtlNtStatusToDosError(status) );
    return handle;
}

/***********************************************************************
 *           CreateWaitableTimerExA    (KERNEL32.@)
 */
HANDLE WINAPI CreateWaitableTimerExA( SECURITY_ATTRIBUTES *sa, LPCSTR name, DWORD flags, DWORD access )
{
    WCHAR buffer[MAX_PATH];

    if (!name) return CreateWaitableTimerExW( sa, NULL, flags, access );

    if (!MultiByteToWideChar( CP_ACP, 0, name, -1, buffer, MAX_PATH ))
    {
        SetLastError( ERROR_FILENAME_EXCED_RANGE );
        return 0;
    }
    return CreateWaitableTimerExW( sa, buffer, flags, access );
}
