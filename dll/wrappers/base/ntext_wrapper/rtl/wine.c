#include <main.h>
#include <intrin.h>
#include <config.h>
#include <port.h>

//#ifdef WORDS_BIGENDIAN
#define srwlock_key_exclusive(lock)   (&lock->Ptr)
#define srwlock_key_shared(lock)      ((void *)((char *)&lock->Ptr + 2))

#define STATUS_RESOURCE_NOT_OWNED   ((NTSTATUS)0xC0000264)

#define SRWLOCK_MASK_IN_EXCLUSIVE     0x80000000
#define SRWLOCK_MASK_EXCLUSIVE_QUEUE  0x7fff0000
#define SRWLOCK_MASK_SHARED_QUEUE     0x0000ffff
#define SRWLOCK_RES_EXCLUSIVE         0x00010000
#define SRWLOCK_RES_SHARED            0x00000001

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
        if ((tmp = interlocked_cmpxchg( (volatile long int *)dest, tmp, val )) == val)
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
        if ((tmp = interlocked_cmpxchg( (volatile long int *)dest, tmp, val )) == val)
            break;
    }
    return val;
}

static inline void srwlock_leave_exclusive( RTL_SRWLOCK *lock, unsigned int val )
{
    /* Used when a thread leaves an exclusive section. If there are other
     * exclusive access threads they are processed first, followed by
     * the shared waiters. */
	 //InitKeyedEvent();
    if (val & SRWLOCK_MASK_EXCLUSIVE_QUEUE)
        NtReleaseKeyedEvent( GlobalKeyedEventHandle, srwlock_key_exclusive(lock), FALSE, NULL );
    else
    {
        val &= SRWLOCK_MASK_SHARED_QUEUE; /* remove SRWLOCK_MASK_IN_EXCLUSIVE */
        while (val--)
            NtReleaseKeyedEvent( GlobalKeyedEventHandle, srwlock_key_shared(lock), FALSE, NULL );
    }
}

static inline void srwlock_leave_shared( RTL_SRWLOCK *lock, unsigned int val )
{
	//InitKeyedEvent();
    /* Wake up one exclusive thread as soon as the last shared access thread
     * has left. */
    if ((val & SRWLOCK_MASK_EXCLUSIVE_QUEUE) && !(val & SRWLOCK_MASK_SHARED_QUEUE))
        NtReleaseKeyedEvent( GlobalKeyedEventHandle, srwlock_key_exclusive(lock), FALSE, NULL );
}

/***********************************************************************
 *              RtlInitializeSRWLock (NTDLL.@)
 *
 * NOTES
 *  Please note that SRWLocks do not keep track of the owner of a lock.
 *  It doesn't make any difference which thread for example unlocks an
 *  SRWLock (see corresponding tests). This implementation uses two
 *  keyed events (one for the exclusive waiters and one for the shared
 *  waiters) and is limited to 2^15-1 waiting threads.
 */
void WINAPI RtlInitializeSRWLock( RTL_SRWLOCK *lock )
{
    lock->Ptr = NULL;
}

/***********************************************************************
 *              RtlAcquireSRWLockExclusive (NTDLL.@)
 *
 * NOTES
 *  Unlike RtlAcquireResourceExclusive this function doesn't allow
 *  nested calls from the same thread. "Upgrading" a shared access lock
 *  to an exclusive access lock also doesn't seem to be supported.
 */
void WINAPI RtlAcquireSRWLockExclusive( RTL_SRWLOCK *lock )
{
    if (srwlock_lock_exclusive( (unsigned int *)&lock->Ptr, SRWLOCK_RES_EXCLUSIVE ))
        NtWaitForKeyedEvent( GlobalKeyedEventHandle, srwlock_key_exclusive(lock), FALSE, NULL );
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

    /* Drop exclusive access again and instead requeue for shared access. */
    if ((val & SRWLOCK_MASK_EXCLUSIVE_QUEUE) && !(val & SRWLOCK_MASK_IN_EXCLUSIVE))
    {
        NtWaitForKeyedEvent( GlobalKeyedEventHandle, srwlock_key_exclusive(lock), FALSE, NULL );
        val = srwlock_unlock_exclusive( (unsigned int *)&lock->Ptr, (SRWLOCK_RES_SHARED
                                        - SRWLOCK_RES_EXCLUSIVE) ) - SRWLOCK_RES_EXCLUSIVE;
        srwlock_leave_exclusive( lock, val );
    }

    if (val & SRWLOCK_MASK_EXCLUSIVE_QUEUE)
        NtWaitForKeyedEvent( GlobalKeyedEventHandle, srwlock_key_shared(lock), FALSE, NULL );
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
 *              RtlReleaseSRWLockShared (NTDLL.@)
 */
void WINAPI RtlReleaseSRWLockShared( RTL_SRWLOCK *lock )
{
    srwlock_leave_shared( lock, srwlock_lock_exclusive( (unsigned int *)&lock->Ptr,
                          - SRWLOCK_RES_SHARED ) - SRWLOCK_RES_SHARED );
}

/***********************************************************************
 *              RtlTryAcquireSRWLockExclusive (NTDLL.@)
 *
 * NOTES
 *  Similar to AcquireSRWLockExclusive recusive calls are not allowed
 *  and will fail with return value FALSE.
 */
BOOLEAN WINAPI RtlTryAcquireSRWLockExclusive( RTL_SRWLOCK *lock )
{
    return interlocked_cmpxchg( (int *)&lock->Ptr, SRWLOCK_MASK_IN_EXCLUSIVE |
                                SRWLOCK_RES_EXCLUSIVE, 0 ) == 0;
}

/***********************************************************************
 *              RtlTryAcquireSRWLockShared (NTDLL.@)
 */
BOOLEAN WINAPI RtlTryAcquireSRWLockShared( RTL_SRWLOCK *lock )
{
    unsigned int val, tmp;
    for (val = *(unsigned int *)&lock->Ptr;; val = tmp)
    {
        if (val & SRWLOCK_MASK_EXCLUSIVE_QUEUE)
            return FALSE;
        if ((tmp = interlocked_cmpxchg( (int *)&lock->Ptr, val + SRWLOCK_RES_SHARED, val )) == val)
            break;
    }
    return TRUE;
}