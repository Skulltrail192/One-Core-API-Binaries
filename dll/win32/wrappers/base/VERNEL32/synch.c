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

BOOL 
WINAPI 
SleepConditionVariableSRW(
	PRTL_CONDITION_VARIABLE ConditionVariable, 
	PRTL_SRWLOCK SRWLock, 
	DWORD dwMilliseconds, 
	ULONG Flags
)
{
	CRITICAL_SECTION crit;
	InitializeCriticalSection(&crit);
	return SleepConditionVariableCS(ConditionVariable, &crit, Flags);
	// NTSTATUS status;
    // LARGE_INTEGER time;

    // status = RtlSleepConditionVariableSRW( ConditionVariable, SRWLock, get_nt_timeout( &time, dwMilliseconds ), Flags );
    // if (status != STATUS_SUCCESS)
    // {
        // SetLastError( RtlNtStatusToDosError(status) );
        // return FALSE;
    // }
    // return TRUE;
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

// void 
// WINAPI
// InitializeSRWLock(PSRWLOCK lock)
// {
	// RW_LOCK *rwlock = NULL;
    // rwlock = (RW_LOCK *)LocalAlloc(0x40,sizeof(RW_LOCK));
    // if (!rwlock)
        // return;
	// lock->Ptr = rwlock;
    // InitializeCriticalSection(&rwlock->writerLock);
    // InitializeCriticalSection(&rwlock->countsLock);

    // /*
     // * Could use a semaphore as well.  There can only be one waiter ever,
     // * so I'm showing an auto-reset event here.
     // */
    // rwlock->noReaders = CreateEvent (NULL, FALSE, FALSE, NULL);
// }

// void 
// WINAPI
// AcquireSRWLockShared(PSRWLOCK lock)
// {
    // /*
     // * We need to lock the writerLock too, otherwise a writer could
     // * do the whole of rwlock_wrlock after the readerCount changed
     // * from 0 to 1, but before the event was reset.
     // */
	// RW_LOCK *rwlock = (RW_LOCK *)lock->Ptr;
    // EnterCriticalSection(&rwlock->writerLock);
    // EnterCriticalSection(&rwlock->countsLock);
    // ++rwlock->readerCount;
    // LeaveCriticalSection(&rwlock->countsLock);
    // LeaveCriticalSection(&rwlock->writerLock);
// }

// VOID 
// WINAPI
// AcquireSRWLockExclusive(PSRWLOCK lock)
// {
	// RW_LOCK *rwlock = (RW_LOCK *)lock->Ptr;
	// EnterCriticalSection(&rwlock->writerLock);
	// rwlock->waitingWriter = TRUE;
	// while (rwlock->readerCount > 0) {
		// LeaveCriticalSection(&rwlock->writerLock);
		// WaitForSingleObject(rwlock->noReaders, INFINITE);
		// EnterCriticalSection(&rwlock->writerLock);
	// }
	// rwlock->waitingWriter = FALSE;

    // /* writerLock remains locked.  */
// }

// void 
// WINAPI
// ReleaseSRWLockShared(PSRWLOCK lock)
// {
	// RW_LOCK *rwlock = (RW_LOCK *)lock->Ptr;	
    // EnterCriticalSection(&rwlock->countsLock);
    // assert (rwlock->readerCount > 0);
    // if (--rwlock->readerCount == 0) {
        // if (rwlock->waitingWriter) {
            // /*
             // * Clear waitingWriter here to avoid taking countsLock
             // * again in wrlock.
             // */
            // rwlock->waitingWriter = FALSE;
            // SetEvent(rwlock->noReaders);
        // }
    // }
    // LeaveCriticalSection(&rwlock->countsLock);
// }

// void 
// WINAPI
// ReleaseSRWLockExclusive(PSRWLOCK lock)
// {
	// RW_LOCK *rwlock = (RW_LOCK *)lock->Ptr;	
    // LeaveCriticalSection(&rwlock->writerLock);
// }









/* {{{1 SRWLock and CONDITION_VARIABLE emulation (for Windows XP) */

static CRITICAL_SECTION g_thread_xp_lock;
static DWORD            g_thread_xp_waiter_tls;

/* {{{2 GThreadWaiter utility class for CONDITION_VARIABLE emulation */
typedef struct _GThreadXpWaiter GThreadXpWaiter;
struct _GThreadXpWaiter
{
  HANDLE                     event;
  volatile GThreadXpWaiter  *next;
  volatile GThreadXpWaiter **my_owner;
};

/* {{{2 SRWLock emulation */
typedef struct
{
  CRITICAL_SECTION  writer_lock;
  boolean          ever_shared;    /* protected by writer_lock */
  boolean          writer_locked;  /* protected by writer_lock */

  /* below is only ever touched if ever_shared becomes true */
  CRITICAL_SECTION  atomicity;
  GThreadXpWaiter  *queued_writer; /* protected by atomicity lock */
  int              num_readers;   /* protected by atomicity lock */
} GThreadSRWLock;

/*
 * The G_LIKELY and G_UNLIKELY macros let the programmer give hints to 
 * the compiler about the expected result of an expression. Some compilers
 * can use this information for optimizations.
 *
 * The _G_BOOLEAN_EXPR macro is intended to trigger a gcc warning when
 * putting assignments in g_return_if_fail ().  
 */
#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#define _G_BOOLEAN_EXPR(expr)                   \
 G_GNUC_EXTENSION ({                            \
   int _g_boolean_var_;                         \
   if (expr)                                    \
      _g_boolean_var_ = 1;                      \
   else                                         \
      _g_boolean_var_ = 0;                      \
   _g_boolean_var_;                             \
})
#define G_LIKELY(expr) (__builtin_expect (_G_BOOLEAN_EXPR((expr)), 1))
#define G_UNLIKELY(expr) (__builtin_expect (_G_BOOLEAN_EXPR((expr)), 0))
#else
#define G_LIKELY(expr) (expr)
#define G_UNLIKELY(expr) (expr)
#endif

#define g_abort() abort ()

static void
g_thread_abort (int         status,
                const char *function)
{
  //g_abort ();
}

static GThreadSRWLock * WINAPI
g_thread_xp_get_srwlock (GThreadSRWLock * volatile *lock)
{
  GThreadSRWLock *result;

  /* It looks like we're missing some barriers here, but this code only
   * ever runs on Windows XP, which in turn only ever runs on hardware
   * with a relatively rigid memory model.  The 'volatile' will take
   * care of the compiler.
   */
  result = *lock;

  if G_UNLIKELY (result == NULL)
    {
      EnterCriticalSection (&g_thread_xp_lock);

      /* Check again */
      result = *lock;
      if (result == NULL)
        {
          result = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY,sizeof (GThreadSRWLock));//malloc (sizeof (GThreadSRWLock));

          if (result == NULL)
            ;

          InitializeCriticalSection (&result->writer_lock);
          result->writer_locked = FALSE;
          result->ever_shared = FALSE;
          *lock = result;
        }

      LeaveCriticalSection (&g_thread_xp_lock);
    }

  return result;
}

static void
g_thread_xp_srwlock_become_reader (GThreadSRWLock *lock)
{
  if G_UNLIKELY (!lock->ever_shared)
    {
      InitializeCriticalSection (&lock->atomicity);
      lock->queued_writer = NULL;
      lock->num_readers = 0;

      lock->ever_shared = TRUE;
    }

  EnterCriticalSection (&lock->atomicity);
  lock->num_readers++;
  LeaveCriticalSection (&lock->atomicity);
}

static GThreadXpWaiter *
g_thread_xp_waiter_get (void)
{
  GThreadXpWaiter *waiter;

  waiter = TlsGetValue (g_thread_xp_waiter_tls);

  if G_UNLIKELY (waiter == NULL)
    {
      waiter = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY,sizeof (GThreadXpWaiter));//malloc (sizeof (GThreadXpWaiter));
      if (waiter == NULL)
        g_thread_abort (GetLastError (), "malloc");
      waiter->event = CreateEvent (0, FALSE, FALSE, NULL);
      if (waiter->event == NULL)
        g_thread_abort (GetLastError (), "CreateEvent");
      waiter->my_owner = NULL;

      TlsSetValue (g_thread_xp_waiter_tls, waiter);
    }

  return waiter;
}

void WINAPI
InitializeSRWLock (PSRWLOCK srwlock)
{
  GThreadSRWLock * volatile mutex = NULL;
  srwlock->Ptr = mutex;
}

void WINAPI
AcquireSRWLockExclusive(PSRWLOCK srwlock)
{
  GThreadSRWLock *lock = g_thread_xp_get_srwlock (srwlock->Ptr);

  EnterCriticalSection (&lock->writer_lock);

  /* CRITICAL_SECTION is reentrant, but SRWLock is not.
   * Detect the deadlock that would occur on later Windows version.
   */
  ASSERT (!lock->writer_locked);
  lock->writer_locked = TRUE;

  if (lock->ever_shared)
    {
      GThreadXpWaiter *waiter = NULL;

      EnterCriticalSection (&lock->atomicity);
      if (lock->num_readers > 0)
        lock->queued_writer = waiter = g_thread_xp_waiter_get ();
      LeaveCriticalSection (&lock->atomicity);

      if (waiter != NULL)
        WaitForSingleObject (waiter->event, INFINITE);

      lock->queued_writer = NULL;
    }
}

BOOLEAN WINAPI
TryAcquireSRWLockExclusive (PSRWLOCK srwlock)
{
  GThreadSRWLock *lock = g_thread_xp_get_srwlock (srwlock->Ptr);

  if (!TryEnterCriticalSection (&lock->writer_lock))
    return FALSE;

  /* CRITICAL_SECTION is reentrant, but SRWLock is not.
   * Ensure that this properly returns FALSE (as SRWLock would).
   */
  if G_UNLIKELY (lock->writer_locked)
    {
      LeaveCriticalSection (&lock->writer_lock);
      return FALSE;
    }

  lock->writer_locked = TRUE;

  if (lock->ever_shared)
    {
      boolean available;

      EnterCriticalSection (&lock->atomicity);
      available = lock->num_readers == 0;
      LeaveCriticalSection (&lock->atomicity);

      if (!available)
        {
          LeaveCriticalSection (&lock->writer_lock);
          return FALSE;
        }
    }

  return TRUE;
}

void WINAPI
ReleaseSRWLockExclusive (PSRWLOCK srwlock)
{
  GThreadSRWLock *lock = *(GThreadSRWLock * volatile *) srwlock->Ptr;

  lock->writer_locked = FALSE;

  /* We need this until we fix some weird parts of GLib that try to
   * unlock freshly-allocated mutexes.
   */
  if (lock != NULL)
    LeaveCriticalSection (&lock->writer_lock);
}


void WINAPI
AcquireSRWLockShared (PSRWLOCK srwlock)
{
  GThreadSRWLock *lock = g_thread_xp_get_srwlock (srwlock->Ptr);

  EnterCriticalSection (&lock->writer_lock);

  /* See g_thread_xp_AcquireSRWLockExclusive */
  ASSERT (!lock->writer_locked);

  g_thread_xp_srwlock_become_reader (lock);

  LeaveCriticalSection (&lock->writer_lock);
}

BOOLEAN WINAPI
TryAcquireSRWLockShared (PSRWLOCK srwlock)
{
  GThreadSRWLock *lock = g_thread_xp_get_srwlock (srwlock->Ptr);

  if (!TryEnterCriticalSection (&lock->writer_lock))
    return FALSE;

  /* See g_thread_xp_AcquireSRWLockExclusive */
  if G_UNLIKELY (lock->writer_locked)
    {
      LeaveCriticalSection (&lock->writer_lock);
      return FALSE;
    }

  g_thread_xp_srwlock_become_reader (lock);

  LeaveCriticalSection (&lock->writer_lock);

  return TRUE;
}

void WINAPI
ReleaseSRWLockShared (PSRWLOCK srwlock)
{
  GThreadSRWLock *lock = g_thread_xp_get_srwlock (srwlock->Ptr);

  EnterCriticalSection (&lock->atomicity);

  lock->num_readers--;

  if (lock->num_readers == 0 && lock->queued_writer)
    SetEvent (lock->queued_writer->event);

  LeaveCriticalSection (&lock->atomicity);
}