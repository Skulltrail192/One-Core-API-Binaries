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

#include "main.h"

typedef CRITICAL_SECTION pthread_mutex_t;

typedef struct win32_cond_t {
    pthread_mutex_t mtx_broadcast;
    pthread_mutex_t mtx_waiter_count;
    volatile int waiter_count;
    HANDLE semaphore;
    HANDLE waiters_done;
    volatile int is_broadcast;
} win32_cond_t;

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
    if (status != STATUS_SUCCESS)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }
    return TRUE;
}

void 
WINAPI 
InitializeConditionVariable(
	PCONDITION_VARIABLE cond
) 
{
    win32_cond_t *win32_cond = NULL;
    win32_cond = (win32_cond_t *)LocalAlloc(0x40,sizeof(win32_cond_t));
    if (!win32_cond)
        return;
    cond->Ptr = win32_cond;
    win32_cond->semaphore = CreateSemaphore(NULL, 0, 0x7fffffff, NULL);
    if (!win32_cond->semaphore)
        return;
    win32_cond->waiters_done = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!win32_cond->waiters_done)
        return;
    InitializeCriticalSection(&win32_cond->mtx_waiter_count);
    InitializeCriticalSection(&win32_cond->mtx_broadcast);
    return;
}

void 
WINAPI 
WakeAllConditionVariable(
	PCONDITION_VARIABLE cond
) 
{
    win32_cond_t *win32_cond = (win32_cond_t *)cond->Ptr;
    int have_waiter;
    EnterCriticalSection(&win32_cond->mtx_broadcast);
    EnterCriticalSection(&win32_cond->mtx_waiter_count);
    have_waiter = 0;
    if (win32_cond->waiter_count) {
        win32_cond->is_broadcast = 1;
        have_waiter = 1;
    }
    if (have_waiter) {
        ReleaseSemaphore(win32_cond->semaphore, win32_cond->waiter_count, NULL);
        LeaveCriticalSection(&win32_cond->mtx_waiter_count);
        WaitForSingleObject(win32_cond->waiters_done, INFINITE);
        ResetEvent(win32_cond->waiters_done);
        win32_cond->is_broadcast = 0;
    } else
        LeaveCriticalSection(&win32_cond->mtx_waiter_count);
    LeaveCriticalSection(&win32_cond->mtx_broadcast);
}

void 
WINAPI 
WakeConditionVariable(
	PCONDITION_VARIABLE cond
) 
{
    win32_cond_t *win32_cond = (win32_cond_t *)cond->Ptr;
    int have_waiter;
    EnterCriticalSection(&win32_cond->mtx_broadcast);
    EnterCriticalSection(&win32_cond->mtx_waiter_count);
    have_waiter = win32_cond->waiter_count;
    LeaveCriticalSection(&win32_cond->mtx_waiter_count);
    if (have_waiter) {
        ReleaseSemaphore(win32_cond->semaphore, 1, NULL);
        WaitForSingleObject(win32_cond->waiters_done, INFINITE);
        ResetEvent(win32_cond->waiters_done);
    }
    LeaveCriticalSection(&win32_cond->mtx_broadcast);
}

int 
WINAPI 
SleepConditionVariableCS(
	PCONDITION_VARIABLE cond, 
    pthread_mutex_t *mutex, 
	int slp
)
{
    win32_cond_t *win32_cond = (win32_cond_t *)cond->Ptr;
    int last_waiter;
	int i; 
    EnterCriticalSection(&win32_cond->mtx_broadcast);
    EnterCriticalSection(&win32_cond->mtx_waiter_count);
    win32_cond->waiter_count++;
    LeaveCriticalSection(&win32_cond->mtx_waiter_count);
    LeaveCriticalSection(&win32_cond->mtx_broadcast);

    LeaveCriticalSection(mutex);
    
	i = WaitForSingleObject(win32_cond->semaphore, slp);
    if(i)
        SetLastError(i);
    EnterCriticalSection(&win32_cond->mtx_waiter_count);
    win32_cond->waiter_count--;
    last_waiter = !win32_cond->waiter_count || !win32_cond->is_broadcast;
    LeaveCriticalSection(&win32_cond->mtx_waiter_count);
    if (last_waiter)
        SetEvent(win32_cond->waiters_done);
    EnterCriticalSection(mutex);
    return (BOOL)(i==0);
}