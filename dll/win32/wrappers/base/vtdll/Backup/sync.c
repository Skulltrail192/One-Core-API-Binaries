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
#include <config.h>
#include <port.h>

#ifdef _WIN64
#define InterlockedBitTestAndSetPointer(ptr,val) InterlockedBitTestAndSet64((PLONGLONG)ptr,(LONGLONG)val)
#define InterlockedAddPointer(ptr,val) InterlockedAdd64((PLONGLONG)ptr,(LONGLONG)val)
#define InterlockedAndPointer(ptr,val) InterlockedAnd64((PLONGLONG)ptr,(LONGLONG)val)
#define InterlockedOrPointer(ptr,val) InterlockedOr64((PLONGLONG)ptr,(LONGLONG)val)
#else
#define InterlockedBitTestAndSetPointer(ptr,val) InterlockedBitTestAndSet((PLONG)ptr,(LONG)val)
#define InterlockedAddPointer(ptr,val) InterlockedAdd((PLONG)ptr,(LONG)val)
#define InterlockedAndPointer(ptr,val) InterlockedAnd((PLONG)ptr,(LONG)val)
#define InterlockedOrPointer(ptr,val) InterlockedOr((PLONG)ptr,(LONG)val)
#endif

#ifdef WORDS_BIGENDIAN
#define srwlock_key_exclusive(lock)   (&lock->Ptr)
#define srwlock_key_shared(lock)      ((void *)((char *)&lock->Ptr + 2))
#else
#define srwlock_key_exclusive(lock)   ((void *)((char *)&lock->Ptr + 2))
#define srwlock_key_shared(lock)      (&lock->Ptr)
#endif

#define RTL_SRWLOCK_OWNED_BIT   0
#define RTL_SRWLOCK_CONTENDED_BIT   1
#define RTL_SRWLOCK_SHARED_BIT  2
#define RTL_SRWLOCK_CONTENTION_LOCK_BIT 3
#define RTL_SRWLOCK_OWNED   (1 << RTL_SRWLOCK_OWNED_BIT)
#define RTL_SRWLOCK_CONTENDED   (1 << RTL_SRWLOCK_CONTENDED_BIT)
#define RTL_SRWLOCK_SHARED  (1 << RTL_SRWLOCK_SHARED_BIT)
#define RTL_SRWLOCK_CONTENTION_LOCK (1 << RTL_SRWLOCK_CONTENTION_LOCK_BIT)
#define RTL_SRWLOCK_MASK    (RTL_SRWLOCK_OWNED | RTL_SRWLOCK_CONTENDED | \
                             RTL_SRWLOCK_SHARED | RTL_SRWLOCK_CONTENTION_LOCK)
#define RTL_SRWLOCK_BITS    4

#define STATUS_RESOURCE_NOT_OWNED   ((NTSTATUS)0xC0000264)

#define SRWLOCK_MASK_IN_EXCLUSIVE     0x80000000
#define SRWLOCK_MASK_EXCLUSIVE_QUEUE  0x7fff0000
#define SRWLOCK_MASK_SHARED_QUEUE     0x0000ffff
#define SRWLOCK_RES_EXCLUSIVE         0x00010000
#define SRWLOCK_RES_SHARED            0x00000001

#define FAST_M_WAKE		256
#define FAST_M_WAIT		512

#define RtlpSetKeyedEventHandle(xxHandle) ((HANDLE)(((ULONG_PTR)xxHandle)|1))

int ConditionVariableSpinCount = 400;

int localConditionCount = 10;

int countTimes;

HANDLE Key_Event = NULL;

void InitKeyedEvent(){
	if(!Key_Event)
	{
		NtCreateKeyedEvent(&Key_Event, -1, NULL, 0);
		DbgPrint("Key_Event initialized\n");
	}
}

static inline void srwlock_check_invalid( unsigned int val )
{
    /* Throw exception if it's impossible to acquire/release this lock. */
    if ((val & SRWLOCK_MASK_EXCLUSIVE_QUEUE) == SRWLOCK_MASK_EXCLUSIVE_QUEUE ||
            (val & SRWLOCK_MASK_SHARED_QUEUE) == SRWLOCK_MASK_SHARED_QUEUE)
        RtlRaiseStatus(STATUS_RESOURCE_NOT_OWNED);
}

static inline unsigned int srwlock_unlock_exclusive( unsigned int *dest, int incr )
{
    unsigned int val, tmp;
    /* Atomically modifies the value of *dest by adding incr. If the queue of
     * threads waiting for exclusive access is empty, then remove the
     * SRWLOCK_MASK_IN_EXCLUSIVE flag (only the shared queue counter will
     * remain). */
    for (val = *dest;; val = tmp)
    {
        tmp = val + incr;
        srwlock_check_invalid( tmp );
        if (!(tmp & SRWLOCK_MASK_EXCLUSIVE_QUEUE))
            tmp &= SRWLOCK_MASK_SHARED_QUEUE;
        if ((tmp = interlocked_cmpxchg( (int *)dest, tmp, val )) == val)
            break;
    }
    return val;
}

static inline unsigned int srwlock_lock_exclusive( unsigned int *dest, int incr )
{
    unsigned int val, tmp;
    /* Atomically modifies the value of *dest by adding incr. If the shared
     * queue is empty and there are threads waiting for exclusive access, then
     * sets the mark SRWLOCK_MASK_IN_EXCLUSIVE to signal other threads that
     * they are allowed again to use the shared queue counter. */
    for (val = *dest;; val = tmp)
    {
        tmp = val + incr;
        srwlock_check_invalid( tmp );
        if ((tmp & SRWLOCK_MASK_EXCLUSIVE_QUEUE) && !(tmp & SRWLOCK_MASK_SHARED_QUEUE))
            tmp |= SRWLOCK_MASK_IN_EXCLUSIVE;
        if ((tmp = interlocked_cmpxchg( (int *)dest, tmp, val )) == val)
            break;
    }
    return val;
}

static inline void srwlock_leave_exclusive( RTL_SRWLOCK *lock, unsigned int val )
{
    /* Used when a thread leaves an exclusive section. If there are other
     * exclusive access threads they are processed first, followed by
     * the shared waiters. */
	 InitKeyedEvent();
    if (val & SRWLOCK_MASK_EXCLUSIVE_QUEUE)
        NtReleaseKeyedEvent( Key_Event, srwlock_key_exclusive(lock), FALSE, NULL );
    else
    {
        val &= SRWLOCK_MASK_SHARED_QUEUE; /* remove SRWLOCK_MASK_IN_EXCLUSIVE */
        while (val--)
            NtReleaseKeyedEvent( Key_Event, srwlock_key_shared(lock), FALSE, NULL );
    }
}

static inline void srwlock_leave_shared( RTL_SRWLOCK *lock, unsigned int val )
{
	InitKeyedEvent();
    /* Wake up one exclusive thread as soon as the last shared access thread
     * has left. */
    if ((val & SRWLOCK_MASK_EXCLUSIVE_QUEUE) && !(val & SRWLOCK_MASK_SHARED_QUEUE))
        NtReleaseKeyedEvent( Key_Event, srwlock_key_exclusive(lock), FALSE, NULL );
}

static inline int interlocked_dec_if_nonzero( int *dest )
{
     int val, tmp;
     for (val = *dest;; val = tmp)
     {
         if (!val || (tmp = InterlockedCompareExchange( dest, val - 1, val )) == val)
             break;
     }
     return val;
}

/******************************************************************
 *              RtlRunOnceBeginInitialize (NTDLL.@)
 */
DWORD 
WINAPI 
RtlRunOnceBeginInitialize( 
	RTL_RUN_ONCE *once, 
	ULONG flags, 
	void **context 
)
{
    if (flags & RTL_RUN_ONCE_CHECK_ONLY)
    {
        ULONG_PTR val = (ULONG_PTR)once->Ptr;

        if (flags & RTL_RUN_ONCE_ASYNC) return STATUS_INVALID_PARAMETER;
        if ((val & 3) != 2) return STATUS_UNSUCCESSFUL;
        if (context) *context = (void *)(val & ~3);
        return STATUS_SUCCESS;
    }
	
	InitKeyedEvent();

    for (;;)
    {
        ULONG_PTR next, val = (ULONG_PTR)once->Ptr;

        switch (val & 3)
        {
        case 0:  /* first time */
            if (!interlocked_cmpxchg_ptr( &once->Ptr,
                                          (flags & RTL_RUN_ONCE_ASYNC) ? (void *)3 : (void *)1, 0 ))
                return STATUS_PENDING;
            break;

        case 1:  /* in progress, wait */			
            if (flags & RTL_RUN_ONCE_ASYNC) return STATUS_INVALID_PARAMETER;
            next = val & ~3;
            if (interlocked_cmpxchg_ptr( &once->Ptr, (void *)((ULONG_PTR)&next | 1),
                                         (void *)val ) == (void *)val)
                NtWaitForKeyedEvent( Key_Event, &next, FALSE, NULL );
            break;

        case 2:  /* done */
            if (context) *context = (void *)(val & ~3);
            return STATUS_SUCCESS;

        case 3:  /* in progress, async */
            if (!(flags & RTL_RUN_ONCE_ASYNC)) return STATUS_INVALID_PARAMETER;
            return STATUS_PENDING;
        }
    }
}

/*
 * @implemented - need test
 */
/******************************************************************
 *              RtlRunOnceComplete (NTDLL.@)
 */
DWORD 
WINAPI 
RtlRunOnceComplete( 
	RTL_RUN_ONCE *once, 
	ULONG flags, 
	void *context 
)
{
    if ((ULONG_PTR)context & 3) return STATUS_INVALID_PARAMETER;

    if (flags & RTL_RUN_ONCE_INIT_FAILED)
    {
        if (context) return STATUS_INVALID_PARAMETER;
        if (flags & RTL_RUN_ONCE_ASYNC) return STATUS_INVALID_PARAMETER;
    }
    else context = (void *)((ULONG_PTR)context | 2);

	InitKeyedEvent();

    for (;;)
    {
        ULONG_PTR val = (ULONG_PTR)once->Ptr;

        switch (val & 3)
        {
        case 1:  /* in progress */
            if (interlocked_cmpxchg_ptr( &once->Ptr, context, (void *)val ) != (void *)val) break;
            val &= ~3;
            while (val)
            {
                ULONG_PTR next = *(ULONG_PTR *)val;
                NtReleaseKeyedEvent( Key_Event, (void *)val, FALSE, NULL );
                val = next;
            }
            return STATUS_SUCCESS;

        case 3:  /* in progress, async */
            if (!(flags & RTL_RUN_ONCE_ASYNC)) return STATUS_INVALID_PARAMETER;
            if (interlocked_cmpxchg_ptr( &once->Ptr, context, (void *)val ) != (void *)val) break;
            return STATUS_SUCCESS;

        default:
            return STATUS_UNSUCCESSFUL;
        }
    }
}

/******************************************************************
  *              RtlRunOnceExecuteOnce (NTDLL.@)
  */
DWORD 
WINAPI 
RtlRunOnceExecuteOnce( 
	RTL_RUN_ONCE *once, 
	PRTL_RUN_ONCE_INIT_FN func,
    void *param, void **context 
)
{
     DWORD ret = RtlRunOnceBeginInitialize( once, 0, context );
 
     if (ret != STATUS_PENDING) return ret;
 
     if (!func( once, param, context ))
     {
         RtlRunOnceComplete( once, RTL_RUN_ONCE_INIT_FAILED, NULL );
         return STATUS_UNSUCCESSFUL;
     } 
     return RtlRunOnceComplete( once, 0, context ? *context : NULL );
}

/******************************************************************
 *              RtlRunOnceInitialize (NTDLL.@)
 */
void 
WINAPI 
RtlRunOnceInitialize( 
	RTL_RUN_ONCE *once 
)
{
    once->Ptr = NULL;
}

/************************************************************* Condition Variable API *********************************************************************/

/***********************************************************************
  *           RtlInitializeConditionVariable   (NTDLL.@)
  *
  * Initializes the condition variable with NULL.
  *
  * PARAMS
  *  variable [O] condition variable
  *
  * RETURNS
  *  Nothing.
  */
void 
WINAPI 
RtlInitializeConditionVariable( 
	PRTL_CONDITION_VARIABLE variable
)
{
    variable->Ptr = NULL;
}

/***********************************************************************
 *           RtlSleepConditionVariableCS   (NTDLL.@)
 *
 * Atomically releases the critical section and suspends the thread,
 * waiting for a Wake(All)ConditionVariable event. Afterwards it enters
 * the critical section again and returns.
 *
 * PARAMS
 *  variable  [I/O] condition variable
 *  crit      [I/O] critical section to leave temporarily
 *  timeout   [I]   timeout
 *
 * RETURNS
 *  see NtWaitForKeyedEvent for all possible return values.
 */
NTSTATUS 
WINAPI 
RtlSleepConditionVariableCS( 
	PRTL_CONDITION_VARIABLE variable, 
	PRTL_CRITICAL_SECTION crit,
	const LARGE_INTEGER *timeout 
)
{
    NTSTATUS status;
	
    InterlockedExchangeAdd( (int *)&variable->Ptr, 1 );
    RtlLeaveCriticalSection(crit);	
	InitKeyedEvent();
	status = NtWaitForKeyedEvent(Key_Event, &variable->Ptr, 0, timeout);
    RtlEnterCriticalSection(crit);
    return status;
}

/***********************************************************************
 *           RtlWakeAllConditionVariable   (NTDLL.@)
 *
 * See WakeConditionVariable, wakes up all waiting threads.
 */
void 
WINAPI 
RtlWakeAllConditionVariable( 
	PRTL_CONDITION_VARIABLE variable 
)
{
	NTSTATUS status;
	InitKeyedEvent();
	
	while(variable->Ptr){
		InterlockedDecrement((int *)&variable->Ptr);
		status = NtReleaseKeyedEvent(Key_Event, &variable->Ptr, FALSE, NULL );	
		DbgPrint("RtlWakeAllConditionVariable. Status: %08x\n",status);
	}
}

/***********************************************************************
 *           RtlWakeConditionVariable   (NTDLL.@)
 *
 * Wakes up one thread waiting on the condition variable.
 *
 * PARAMS
 *  variable [I/O] condition variable to wake up.
 *
 * RETURNS
 *  Nothing.
 *
 * NOTES
 *  The calling thread does not have to own any lock in order to call
 *  this function.
 */
void 
WINAPI 
RtlWakeConditionVariable( 
	PRTL_CONDITION_VARIABLE variable 
)
{
	NTSTATUS status;
	InitKeyedEvent();
	
	if(variable->Ptr){ 		
		InterlockedDecrement((int *)&variable->Ptr); 		
		status = NtReleaseKeyedEvent( Key_Event, &variable->Ptr, FALSE, NULL ); 		
		DbgPrint("RtlWakeConditionVariable. Status: %08x\n",status);		
	}
}

/************************************************** SRW API *****************************************************************/

VOID
NTAPI
RtlInitializeSRWLock(OUT PRTL_SRWLOCK SRWLock)
{
    SRWLock->Ptr = NULL;
}

/***********************************************************************
 *              RtlAcquireSRWLockShared (NTDLL.@)
 *
 * NOTES
 *   Do not call this function recursively - it will only succeed when
 *   there are no threads waiting for an exclusive lock!
 */
void WINAPI RtlAcquireSRWLockShared( RTL_SRWLOCK *lock )
{
    unsigned int val, tmp;
	NTSTATUS status;
    /* Acquires a shared lock. If it's currently not possible to add elements to
     * the shared queue, then request exclusive access instead. */
    for (val = *(unsigned int *)&lock->Ptr;; val = tmp)
    {
        if ((val & SRWLOCK_MASK_EXCLUSIVE_QUEUE) && !(val & SRWLOCK_MASK_IN_EXCLUSIVE))
            tmp = val + SRWLOCK_RES_EXCLUSIVE;
        else
            tmp = val + SRWLOCK_RES_SHARED;
        if ((tmp = interlocked_cmpxchg( (int *)&lock->Ptr, tmp, val )) == val)
            break;
    }
	
	InitKeyedEvent();

    /* Drop exclusive access again and instead requeue for shared access. */
    if ((val & SRWLOCK_MASK_EXCLUSIVE_QUEUE) && !(val & SRWLOCK_MASK_IN_EXCLUSIVE))
    {
        status = NtWaitForKeyedEvent( Key_Event, srwlock_key_exclusive(lock), FALSE, NULL );
		
        val = srwlock_unlock_exclusive( (unsigned int *)&lock->Ptr, (SRWLOCK_RES_SHARED
                                        - SRWLOCK_RES_EXCLUSIVE) ) - SRWLOCK_RES_EXCLUSIVE;
        srwlock_leave_exclusive( lock, val );
    }

	
    if (val & SRWLOCK_MASK_EXCLUSIVE_QUEUE)
        NtWaitForKeyedEvent( Key_Event, srwlock_key_shared(lock), FALSE, NULL );
}

/***********************************************************************
 *              RtlTryAcquireSRWLockExclusive (NTDLL.@)
 *
 * NOTES
 *  Similar to AcquireSRWLockExclusive recusive calls are not allowed
 *  and will fail with return value FALSE.
 */
BOOLEAN 
WINAPI 
RtlTryAcquireSRWLockExclusive( 
	RTL_SRWLOCK *lock 
)
{
    return InterlockedCompareExchange( (int *)&lock->Ptr, SRWLOCK_MASK_IN_EXCLUSIVE |
                                SRWLOCK_RES_EXCLUSIVE, 0 ) == 0;
}

/***********************************************************************
 *              RtlTryAcquireSRWLockShared (NTDLL.@)
 */
BOOLEAN 
WINAPI 
RtlTryAcquireSRWLockShared( 
	RTL_SRWLOCK *lock 
)
{
    unsigned int val, tmp;
    for (val = *(unsigned int *)&lock->Ptr;; val = tmp)
    {
        if (val & SRWLOCK_MASK_EXCLUSIVE_QUEUE)
            return FALSE;
        if ((tmp = InterlockedCompareExchange( (int *)&lock->Ptr, val + SRWLOCK_RES_SHARED, val )) == val)
            break;
    }
    return TRUE;
}

/***********************************************************************
 *              RtlAcquireSRWLockExclusive (NTDLL.@)
 *
 * NOTES
 *  Unlike RtlAcquireResourceExclusive this function doesn't allow
 *  nested calls from the same thread. "Upgrading" a shared access lock
 *  to an exclusive access lock also doesn't seem to be supported.
 */
VOID WINAPI RtlAcquireSRWLockExclusive( RTL_SRWLOCK *lock )
{
	InitKeyedEvent();
    if (srwlock_lock_exclusive( (unsigned int *)&lock->Ptr, SRWLOCK_RES_EXCLUSIVE ))
        NtWaitForKeyedEvent( Key_Event, srwlock_key_exclusive(lock), FALSE, NULL );
}

/***********************************************************************
 *              RtlReleaseSRWLockShared (NTDLL.@)
 */
VOID WINAPI RtlReleaseSRWLockShared( RTL_SRWLOCK *lock )
{
    srwlock_leave_shared( lock, srwlock_lock_exclusive( (unsigned int *)&lock->Ptr,
                          - SRWLOCK_RES_SHARED ) - SRWLOCK_RES_SHARED );
}

/***********************************************************************
 *              RtlReleaseSRWLockExclusive (NTDLL.@)
 */
void WINAPI RtlReleaseSRWLockExclusive( RTL_SRWLOCK *lock )
{
    srwlock_leave_exclusive( lock, srwlock_unlock_exclusive( (unsigned int *)&lock->Ptr,
                             - SRWLOCK_RES_EXCLUSIVE ) - SRWLOCK_RES_EXCLUSIVE );
}

/***********************************************************************
 *           RtlSleepConditionVariableSRW   (NTDLL.@)
 *
 * Atomically releases the SRWLock and suspends the thread,
 * waiting for a Wake(All)ConditionVariable event. Afterwards it enters
 * the SRWLock again with the same access rights and returns.
 *
 * PARAMS
 *  variable  [I/O] condition variable
 *  lock      [I/O] SRWLock to leave temporarily
 *  timeout   [I]   timeout
 *  flags     [I]   type of the current lock (exclusive / shared)
 *
 * RETURNS
 *  see NtWaitForKeyedEvent for all possible return values.
 *
 * NOTES
 *  the behaviour is undefined if the thread doesn't own the lock.
 */
NTSTATUS 
WINAPI 
RtlSleepConditionVariableSRW( 
	RTL_CONDITION_VARIABLE *variable, 
	RTL_SRWLOCK *lock,
    const LARGE_INTEGER *timeout, 
	ULONG flags 
)
{
    NTSTATUS status;
    interlocked_xchg_add( (int *)&variable->Ptr, 1 );

    if (flags & RTL_CONDITION_VARIABLE_LOCKMODE_SHARED)
        RtlReleaseSRWLockShared( lock );
    else
        RtlReleaseSRWLockExclusive( lock );

	InitKeyedEvent();
    status = NtWaitForKeyedEvent( Key_Event, &variable->Ptr, FALSE, timeout );
    if (status != STATUS_SUCCESS)
    {
        if (!interlocked_dec_if_nonzero( (int *)&variable->Ptr ))
            status = NtWaitForKeyedEvent( Key_Event, &variable->Ptr, FALSE, NULL );
    }

    if (flags & RTL_CONDITION_VARIABLE_LOCKMODE_SHARED)
        RtlAcquireSRWLockShared( lock );
    else
        RtlAcquireSRWLockExclusive( lock );
    return status;
}