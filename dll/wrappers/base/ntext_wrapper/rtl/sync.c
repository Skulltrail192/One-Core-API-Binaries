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

#define ReadForWriteAccess(p) (*(p))

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

//#define EX_PUSH_LOCK_FLAGS_EXCLUSIVE  (0x1)
#define EX_PUSH_LOCK_FLAGS_SPINNING_V (0x1)
#define EX_PUSH_LOCK_FLAGS_SPINNING   (0x2)

//#ifdef WORDS_BIGENDIAN
#define srwlock_key_exclusive(lock)   (&lock->Ptr)
#define srwlock_key_shared(lock)      ((void *)((char *)&lock->Ptr + 2))
// #else
// #define srwlock_key_exclusive(lock)   ((void *)((char *)&lock->Ptr + 2))
// #define srwlock_key_shared(lock)      (&lock->Ptr)
// #endif

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

#define EX_PUSH_LOCK_PTR_BITS ((ULONG_PTR)0xf)

#define RtlpSetKeyedEventHandle(xxHandle) ((HANDLE)(((ULONG_PTR)xxHandle)|1))

ULONG ExPushLockSpinCount = 0;

typedef struct _RTL_BACKOFF {
   ULONG Delay;
   ULONG Salt;
} RTL_BACKOFF, *PRTL_BACKOFF;

/* GLOBALS *******************************************************************/

static HANDLE GlobalKeyedEventHandle = NULL;

static RTL_CRITICAL_SECTION criticalTrySection;

int ConditionVariableSpinCount = 400;

int SRWLockSpinCount = 400;

int localConditionCount = 10;

int countTimes;

// void InitKeyedEvent(){
	// if(!GlobalKeyedEventHandle)
	// {
		// NtCreateKeyedEvent(&GlobalKeyedEventHandle, -1, NULL, 0);
		// DbgPrint("GlobalKeyedEventHandle initialized\n");
	// }
// }
VOID
RtlpInitializeKeyedEvent(VOID)
{
	if(NtCurrentTeb()->ProcessEnvironmentBlock->NumberOfProcessors>1)
	{
		ExPushLockSpinCount = 1024;
	}
    ASSERT(GlobalKeyedEventHandle == NULL);
	RtlInitializeCriticalSection(&criticalTrySection);
    NtCreateKeyedEvent(&GlobalKeyedEventHandle, EVENT_ALL_ACCESS, NULL, 0);
}

VOID
RtlpCloseKeyedEvent(VOID)
{
    ASSERT(GlobalKeyedEventHandle != NULL);
    NtClose(GlobalKeyedEventHandle);
    GlobalKeyedEventHandle = NULL;
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

static inline int interlocked_dec_if_nonzero( volatile long int *dest )
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
NTAPI 
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
	
	//InitKeyedEvent();

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
                NtWaitForKeyedEvent( GlobalKeyedEventHandle, &next, FALSE, NULL );
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
NTAPI
RtlRunOnceComplete( 
	PRTL_RUN_ONCE once, 
	ULONG flags, 
	PVOID context 
)
{
    if ((ULONG_PTR)context & 3) return STATUS_INVALID_PARAMETER;

    if (flags & RTL_RUN_ONCE_INIT_FAILED)
    {
        if (context) return STATUS_INVALID_PARAMETER;
        if (flags & RTL_RUN_ONCE_ASYNC) return STATUS_INVALID_PARAMETER;
    }
    else context = (void *)((ULONG_PTR)context | 2);

	//InitKeyedEvent();

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
                NtReleaseKeyedEvent( GlobalKeyedEventHandle, (void *)val, FALSE, NULL );
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
NTAPI 
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
NTAPI 
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
NTAPI 
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
NTAPI 
RtlSleepConditionVariableCS( 
	PRTL_CONDITION_VARIABLE variable, 
	PRTL_CRITICAL_SECTION crit,
	PLARGE_INTEGER timeout 
)
{
    NTSTATUS status;
	
    InterlockedExchangeAdd( (volatile long int *)&variable->Ptr, 1 );
    RtlLeaveCriticalSection(crit);	
	//InitKeyedEvent();
	status = NtWaitForKeyedEvent(GlobalKeyedEventHandle, &variable->Ptr, 0, timeout);
    RtlEnterCriticalSection(crit);
    return status;
}

/***********************************************************************
 *           RtlWakeAllConditionVariable   (NTDLL.@)
 *
 * See WakeConditionVariable, wakes up all waiting threads.
 */
void 
NTAPI 
RtlWakeAllConditionVariable( 
	PRTL_CONDITION_VARIABLE variable 
)
{
	NTSTATUS status;
	//InitKeyedEvent();
	
	while(variable->Ptr){
		InterlockedDecrement((volatile long int *)&variable->Ptr);
		status = NtReleaseKeyedEvent(GlobalKeyedEventHandle, &variable->Ptr, FALSE, NULL );	
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
NTAPI 
RtlWakeConditionVariable( 
	PRTL_CONDITION_VARIABLE variable 
)
{
	NTSTATUS status;
	//InitKeyedEvent();
	
	if(variable->Ptr){ 		
		InterlockedDecrement((volatile long int *)&variable->Ptr); 		
		status = NtReleaseKeyedEvent( GlobalKeyedEventHandle, &variable->Ptr, FALSE, NULL ); 		
		DbgPrint("RtlWakeConditionVariable. Status: %08x\n",status);		
	}
}

/************************************************** SRW API *****************************************************************/

// VOID
// NTAPI
// RtlInitializeSRWLock(RTL_SRWLOCK *lock)
// {
    // lock->Ptr = NULL;
// }

// /***********************************************************************
 // *              RtlAcquireSRWLockExclusive (NTDLL.@)
 // *
 // * NOTES
 // *  Unlike RtlAcquireResourceExclusive this function doesn't allow
 // *  nested calls from the same thread. "Upgrading" a shared access lock
 // *  to an exclusive access lock also doesn't seem to be supported.
 // */
// VOID 
// NTAPI 
// RtlAcquireSRWLockExclusive( 
	// RTL_SRWLOCK *lock
// )
// {
	// // //InitKeyedEvent();
    // if (srwlock_lock_exclusive( (unsigned int *)&lock->Ptr, SRWLOCK_RES_EXCLUSIVE ))
        // NtWaitForKeyedEvent( GlobalKeyedEventHandle, srwlock_key_exclusive(lock), FALSE, NULL );
// }

// /***********************************************************************
 // *              RtlAcquireSRWLockShared (NTDLL.@)
 // *
 // * NOTES
 // *   Do not call this function recursively - it will only succeed when
 // *   there are no threads waiting for an exclusive lock!
 // */
// void NTAPI RtlAcquireSRWLockShared( RTL_SRWLOCK *lock )
// {
    // unsigned int val, tmp;
    // /* Acquires a shared lock. If it's currently not possible to add elements to
     // * the shared queue, then request exclusive access instead. */
    // for (val = *(unsigned int *)&lock->Ptr;; val = tmp)
    // {
        // if ((val & SRWLOCK_MASK_EXCLUSIVE_QUEUE) && !(val & SRWLOCK_MASK_IN_EXCLUSIVE))
            // tmp = val + SRWLOCK_RES_EXCLUSIVE;
        // else
            // tmp = val + SRWLOCK_RES_SHARED;
        // if ((tmp = interlocked_cmpxchg( (volatile long int *)&lock->Ptr, tmp, val )) == val)
            // break;
    // }
	
	// //InitKeyedEvent();

    // /* Drop exclusive access again and instead requeue for shared access. */
    // if ((val & SRWLOCK_MASK_EXCLUSIVE_QUEUE) && !(val & SRWLOCK_MASK_IN_EXCLUSIVE))
    // {
        // NtWaitForKeyedEvent( GlobalKeyedEventHandle, srwlock_key_exclusive(lock), FALSE, NULL );
		
        // val = srwlock_unlock_exclusive( (unsigned int *)&lock->Ptr, (SRWLOCK_RES_SHARED
                                        // - SRWLOCK_RES_EXCLUSIVE) ) - SRWLOCK_RES_EXCLUSIVE;
        // srwlock_leave_exclusive( lock, val );
    // }

	
    // if (val & SRWLOCK_MASK_EXCLUSIVE_QUEUE)
        // NtWaitForKeyedEvent( GlobalKeyedEventHandle, srwlock_key_shared(lock), FALSE, NULL );
// }

// void __stdcall RtlBackoff(unsigned int *pCount)  
// {  
    // unsigned int nBackCount;
	// unsigned int i;
  
    // nBackCount = *pCount;  
    // if ( nBackCount )  
    // {  
        // if ( nBackCount < 0x1FFF )  
            // nBackCount *= 2;  
    // }  
    // else  
    // {  
        // // __readfsdword(24) --> teb  
        // // teb+48           --> peb  
        // // peb+100          --> NumberOfProcessors  
        // if ( *(DWORD *)(*(DWORD *)(__readfsdword(24) + 48) + 100) == 1 ) // 获取cpu个数(核数)  
            // return;  
  
        // // ================for ia64================  
        // // NtCurrentTeb()  
        // // teb+48h  --> tid(64bits)  
        // // teb+60h  --> peb(64bits)  
        // // peb+b8h  --> NumberOfProcessors(32bits)  
  
        // nBackCount = 64;  
    // }  
  
    // nBackCount = ((nBackCount - 1) & __rdtsc()) + nBackCount;  
  
    // for ( i = 0; i < nBackCount; i++ )  
    // {  
        // _mm_pause();  
    // }  
  
    // return;  
// }  

void NTAPI RtlBackoff(PRTL_BACKOFF a1)
{
  signed int v1; // ecx@1
  unsigned __int64 v2; // rax@4
  unsigned int v3; // ecx@4
  PRTL_BACKOFF a1a; // [sp+8h] [bp+8h]@4

  v1 = a1->Delay;
  if ( a1->Delay )
  {
    if ( (unsigned int)v1 < 0x1FFF )
      v1 *= 2;
  }
  else
  {
    if ( NtCurrentTeb()->ProcessEnvironmentBlock->NumberOfProcessors == 1 )
      return;
    v1 = 64;
  }
  a1->Delay = v1;
  v2 = __rdtsc();
  v3 = ((v1 - 1) & v2) + v1;
  for ( a1a = 0; (unsigned int)a1a < v3; a1a = (PRTL_BACKOFF)((char *)a1a + 1) )
    YieldProcessor();
}

typedef struct _SyncItem  
{  
    struct _SyncItem* back;  
    struct _SyncItem* notify;  
    struct _SyncItem* next;  
    size_t shareCount;  
    size_t flag;      
}_SyncItem;  


// /***********************************************************************
 // *              RtlReleaseSRWLockShared (NTDLL.@)
 // */
// VOID 
// NTAPI
// RtlReleaseSRWLockShared(RTL_SRWLOCK *lock)
// {
    // srwlock_leave_shared( lock, srwlock_lock_exclusive( (unsigned int *)&lock->Ptr,
                          // - SRWLOCK_RES_SHARED ) - SRWLOCK_RES_SHARED );
// }

// /***********************************************************************
 // *              RtlReleaseSRWLockExclusive (NTDLL.@)
 // */
// void WINAPI RtlReleaseSRWLockExclusive(RTL_SRWLOCK *lock)
// {
    // srwlock_leave_exclusive( lock, srwlock_unlock_exclusive( (unsigned int *)&lock->Ptr,
                              // - SRWLOCK_RES_EXCLUSIVE ) - SRWLOCK_RES_EXCLUSIVE );
// }

// /***********************************************************************
 // *              RtlTryAcquireSRWLockExclusive (NTDLL.@)
 // *
 // * NOTES
 // *  Similar to AcquireSRWLockExclusive recusive calls are not allowed
 // *  and will fail with return value FALSE.
 // */
// BOOLEAN WINAPI RtlTryAcquireSRWLockExclusive( RTL_SRWLOCK *lock )
// {
    // return interlocked_cmpxchg( (int *)&lock->Ptr, SRWLOCK_MASK_IN_EXCLUSIVE |
                                // SRWLOCK_RES_EXCLUSIVE, 0 ) == 0;
// }

// /***********************************************************************
 // *              RtlTryAcquireSRWLockShared (NTDLL.@)
 // */
// BOOLEAN WINAPI RtlTryAcquireSRWLockShared( RTL_SRWLOCK *lock )
// {
    // unsigned int val, tmp;
    // for (val = *(unsigned int *)&lock->Ptr;; val = tmp)
    // {
        // if (val & SRWLOCK_MASK_EXCLUSIVE_QUEUE)
            // return FALSE;
        // if ((tmp = interlocked_cmpxchg( (int *)&lock->Ptr, val + SRWLOCK_RES_SHARED, val )) == val)
            // break;
    // }
    // return TRUE;
//}

typedef struct _EX_PUSH_LOCK
{
     union
     {
          ULONG Locked: 1;
          ULONG Waiting: 1;
          ULONG Waking: 1;
          ULONG MultipleShared: 1;
          ULONG Shared: 28;
          ULONG Value;
          PVOID Ptr;
     };
} EX_PUSH_LOCK, *PEX_PUSH_LOCK;

typedef struct _EX_PUSH_LOCK_WAIT_BLOCK
{
     union
     {
            PRTLP_SRWLOCK_SHARED_WAKE WakeGate;
            PRTLP_SRWLOCK_SHARED_WAKE WakeEvent;
     };
     struct _EX_PUSH_LOCK_WAIT_BLOCK *Next;
     struct _EX_PUSH_LOCK_WAIT_BLOCK *Last;
     struct _EX_PUSH_LOCK_WAIT_BLOCK *Previous;
     LONG ShareCount;
     LONG Flags;
} EX_PUSH_LOCK_WAIT_BLOCK, *PEX_PUSH_LOCK_WAIT_BLOCK;

// void NTAPI RtlpWakeSRWLock(PRTL_SRWLOCK SRWLOCK, RTL_SRWLOCK TopValue)
// {
  // RTL_SRWLOCK NewValue; // edx@1 MAPDST
  // PRTLP_SRWLOCK_WAITBLOCK PreviousWaitBlock; // eax@3
  // PRTLP_SRWLOCK_WAITBLOCK WaitBlock; // esi@3
  // PRTLP_SRWLOCK_WAITBLOCK NextWaitBlock; // eax@4
  // RTL_SRWLOCK OldValue; // eax@5
  // PRTLP_SRWLOCK_WAITBLOCK FirstWaitBlock; // esi@15

  // for ( NewValue.Ptr = TopValue.Ptr; !((unsigned __int8)NewValue.Ptr & 1); NewValue.Ptr = OldValue.Ptr )
  // {
// ASSIGMENT:
    // PreviousWaitBlock = (PRTLP_SRWLOCK_WAITBLOCK)((unsigned int)NewValue.Ptr & 0xFFFFFFF0);
    // WaitBlock = *(PRTLP_SRWLOCK_WAITBLOCK *)(((unsigned int)NewValue.Ptr & 0xFFFFFFF0) + 4);
    // if ( !WaitBlock )
    // {
      // do
      // {
        // FirstWaitBlock = PreviousWaitBlock;
        // PreviousWaitBlock = (PRTLP_SRWLOCK_WAITBLOCK)PreviousWaitBlock->SharedCount;
        // PreviousWaitBlock->Next = FirstWaitBlock;
        // WaitBlock = PreviousWaitBlock->Last;
      // }
      // while ( !WaitBlock );
      // if ( (PRTLP_SRWLOCK_WAITBLOCK)((unsigned int)NewValue.Ptr & 0xFFFFFFF0) != PreviousWaitBlock )
         // WaitBlock = (PRTLP_SRWLOCK_WAITBLOCK)(NewValue.Ptr & ~(ULONG_PTR)EX_PUSH_LOCK_PTR_BITS);
		   // //*(_DWORD *)(((unsigned int)NewValue.Ptr & 0xFFFFFFF0) + 4) = WaitBlock;
    // }
    // NextWaitBlock = WaitBlock->Next;
    // TopValue.Ptr = (VOID *)WaitBlock;
    // if ( NextWaitBlock && (unsigned int)WaitBlock->LastSharedWake & 1 )
    // {
      // WaitBlock->Next = NULL;
      // (((unsigned int)NewValue.Ptr & 0xFFFFFFF0) + 4) = NextWaitBlock;
      // InterlockedAnd ((LONG *)SRWLOCK, 0xFFFFFFFB);
      // do
      // {
// LABEL_6:
        // WaitBlock = WaitBlock->Next;
        // if ( !_interlockedbittestandreset((volatile signed __int32 *)TopValue.Ptr + 4, 1u) )
          // NtReleaseKeyedEvent(GlobalKeyedEventHandle, TopValue.Ptr, 0, 0);
        // TopValue.Ptr = (VOID *)WaitBlock;
      // }
      // while ( WaitBlock );
      // return;
    // }
    // OldValue.Ptr = (VOID *)_InterlockedCompareExchange(
                             // (volatile signed __int32 *)SRWLOCK,
                             // 0,
                             // (signed __int32)NewValue.Ptr);
    // if ( OldValue.Ptr == NewValue.Ptr )
      // goto LABEL_6;
		// WaitBlock = (PRTLP_SRWLOCK_WAITBLOCK)((ULONG_PTR)NewValue.Ptr & ~(ULONG_PTR)EX_PUSH_LOCK_PTR_BITS);
		// //= WaitBlock;
  // }
  // while ( 1 )
  // {
    // NewValue.Ptr = (VOID *)_InterlockedCompareExchange(
                             // (volatile signed __int32 *)SRWLOCK,
                             // (signed __int32)((char *)NewValue.Ptr - 4),
                             // (signed __int32)NewValue.Ptr);
    // if ( NewValue.Ptr == NewValue.Ptr )
      // break;
    // if ( !((unsigned __int8)NewValue.Ptr & 1) )
      // goto ASSIGMENT;
  // }
// }

// void NTAPI RtlpOptimizeSRWLockList(PRTL_SRWLOCK SRWLock, RTL_SRWLOCK TopValue)
// {
  // RTL_SRWLOCK OldValue; // ecx@1
  // PRTLP_SRWLOCK_WAITBLOCK WaitBlock; // eax@2
  // PRTLP_SRWLOCK_WAITBLOCK PreviousWaitBlock; // edx@4
  // RTL_SRWLOCK NewValue; // eax@6

  // OldValue.Ptr = TopValue.Ptr;
  // while ( 1 )
  // {
      // if (!OldValue.Locked) {
        // RtlpWakeSRWLock (SRWLock, OldValue);
        // break;
	  // }	
      // WaitBlock = (PRTLP_SRWLOCK_WAITBLOCK)((unsigned int)OldValue.Ptr & EX_PUSH_LOCK_PTR_BITS);
      // if ( (unsigned int)OldValue.Ptr & EX_PUSH_LOCK_PTR_BITS )
      // {
        // while ( !WaitBlock->Last )
        // {
          // PreviousWaitBlock = WaitBlock;
          // WaitBlock = (PRTLP_SRWLOCK_WAITBLOCK)WaitBlock->SharedCount;
          // WaitBlock->Next = PreviousWaitBlock;
        // }
		// NewValue.Ptr = OldValue.Ptr - EX_PUSH_LOCK_WAKING;
        // //*(_DWORD *)(((unsigned int)OldValue.Ptr & 0xFFFFFFF0) + 4) = WaitBlock->Last;
      // }
      // NewValue.Ptr = InterlockedCompareExchangePointer(
                               // (volatile PVOID *)SRWLock,
                               // ((char *)OldValue.Ptr - 4),
                               // OldValue.Ptr);
      // if ( NewValue.Ptr == OldValue.Ptr )
        // break;
      // OldValue.Ptr = NewValue.Ptr;
    // }
// }

// void NTAPI RtlAcquireSRWLockExclusive(PRTL_SRWLOCK SRWLock)
// {
  // ULONG i; // eax@4 MAPDST
  // RTL_SRWLOCK OldValue; // ecx@6
  // RTL_SRWLOCK TopValue; // eax@15
  // RTL_SRWLOCK NewValue; // esi@16
  // RTLP_SRWLOCK_WAITBLOCK v10; // [sp-28h] [bp-30h]@14 MAPDST
  // RTLP_SRWLOCK_WAITBLOCK WaitBlock; // [sp-24h] [bp-2Ch]@7
  // RTL_BACKOFF Backoff = {0}; // [sp-Ch] [bp-14h]@1
  // BOOLEAN Optimize; // [sp-5h] [bp-Dh]@3

  // if ( InterlockedBitTestAndSetPointer(SRWLock, RTL_SRWLOCK_OWNED_BIT) )
  // {
    // for ( OldValue.Ptr = SRWLock->Ptr; (unsigned __int8)OldValue.Ptr & 1; OldValue.Ptr = NewValue.Ptr )
    // {
      // WaitBlock.SharedCount = 3;
      // i = 0;
      // Optimize = FALSE;
      // if ( (unsigned __int8)OldValue.Ptr & 2 )
      // {
        // v10.SharedCount = (unsigned int)OldValue.Ptr & EX_PUSH_LOCK_PTR_BITS;
        // TopValue.Ptr = (VOID *)((unsigned int)&v10 | (unsigned __int8)OldValue.Ptr & 8 | 7);
        // v10.SharedCount = 0;
        // if ( !((unsigned __int8)OldValue.Ptr & 4) )
          // Optimize = TRUE;
      // }
      // else
      // {
        // v10.SharedCount = (unsigned int)OldValue.Ptr >> 4;
        // if ( (unsigned int)OldValue.Ptr >> 4 > 1 )
        // {
          // TopValue.Ptr = (VOID *)((unsigned int)&v10 | 0xB);
        // }
        // else
        // {
          // v10.SharedCount = 0;
          // TopValue.Ptr = (VOID *)((unsigned int)&v10 | 3);
        // }
      // }
      // NewValue.Ptr = InterlockedCompareExchangePointer(
                               // (volatile PVOID *)SRWLock,
                               // TopValue.Ptr,
                               // OldValue.Ptr);
      // if ( NewValue.Ptr != OldValue.Ptr )
      // {
        // RtlBackoff(&Backoff);
// RETRAY:
        // NewValue.Ptr = SRWLock->Ptr;
        // continue;
      // }
      // if ( Optimize )
        // RtlpOptimizeSRWLockList(SRWLock, TopValue);
      // for ( i = SRWLockSpinCount; i; --i )
      // {
        // if ( !(WaitBlock.SharedCount & 2) )
          // break;
        // YieldProcessor();
      // }
      // if ( InterlockedBitTestAndReset(&WaitBlock.SharedCount, EX_PUSH_LOCK_FLAGS_SPINNING_V) )
        // NtWaitForKeyedEvent(GlobalKeyedEventHandle, &v10, 0, 0);
    // }
    // if ( InterlockedCompareExchangePointer(
                   // (volatile PVOID *)SRWLock,
                   // ((char *)OldValue.Ptr + 1),
                   // OldValue.Ptr) == OldValue.Ptr )
      // return;
    // RtlBackoff(&Backoff);
    // goto RETRAY;
  // }
// }

VOID
FASTCALL
ExfWakePushLock (
    IN PEX_PUSH_LOCK PushLock,
    IN EX_PUSH_LOCK TopValue
    )
/*++

Routine Description:

    Walks the pushlock waiting list and wakes waiters if the lock is still unacquired.

Arguments:

    PushLock - Push lock to be walked

    TopValue - Start of the chain (*PushLock)

Return Value:

    None

--*/
{
    EX_PUSH_LOCK OldValue, NewValue;
    PEX_PUSH_LOCK_WAIT_BLOCK WaitBlock, NextWaitBlock, FirstWaitBlock, PreviousWaitBlock;
    //KIRQL OldIrql;

    OldValue = TopValue;

    while (1) {

        //
        // Nobody should be walking the list while we manipulate it.
        //

        ASSERT (!OldValue.MultipleShared);

        //
        // No point waking somebody to find a locked lock. Just clear the waking bit
        //

        while (OldValue.Locked) {
            NewValue.Value = OldValue.Value - EX_PUSH_LOCK_WAKING;
            ASSERT (!NewValue.Waking);
            ASSERT (NewValue.Locked);
            ASSERT (NewValue.Waiting);
            if ((NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                                   NewValue.Ptr,
                                                                   OldValue.Ptr)) == OldValue.Ptr) {
                return;
            }
            OldValue = NewValue;
        }

        WaitBlock = (PEX_PUSH_LOCK_WAIT_BLOCK)
           (OldValue.Value & ~(ULONG_PTR)EX_PUSH_LOCK_PTR_BITS);

        FirstWaitBlock = WaitBlock;

        while (1) {

            NextWaitBlock = WaitBlock->Last;
            if (NextWaitBlock != NULL) {
                WaitBlock = NextWaitBlock;
                break;
            }

            PreviousWaitBlock = WaitBlock;
            WaitBlock = WaitBlock->Next;
            WaitBlock->Previous = PreviousWaitBlock;
        }

        if (WaitBlock->Flags&EX_PUSH_LOCK_FLAGS_EXCLUSIVE &&
            (PreviousWaitBlock = WaitBlock->Previous) != NULL) {

            FirstWaitBlock->Last = PreviousWaitBlock;

            WaitBlock->Previous = NULL;

            ASSERT (FirstWaitBlock != WaitBlock);

            ASSERT (PushLock->Waiting);

#if defined (_WIN64)
            InterlockedAnd64 ((LONG64 *)&PushLock->Value, ~EX_PUSH_LOCK_WAKING);
#else
            InterlockedAnd ((LONG *)&PushLock->Value, ~EX_PUSH_LOCK_WAKING);
#endif

            break;
        } else {
            NewValue.Value = 0;
            ASSERT (!NewValue.Waking);
            if ((NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                                   NewValue.Ptr,
                                                                   OldValue.Ptr)) == OldValue.Ptr) {
                break;
            }
            OldValue = NewValue;
        }
    }

    //
    // If we are waking more than one thread then raise to DPC level to prevent us
    // getting rescheduled part way through the operation
    //

    // OldIrql = DISPATCH_LEVEL;
    // if (WaitBlock->Previous != NULL) {
        // KeRaiseIrql (DISPATCH_LEVEL, &OldIrql);
    // }

    while (1) {

        NextWaitBlock = WaitBlock->Previous;
        if (!InterlockedBitTestAndReset (&WaitBlock->Flags, EX_PUSH_LOCK_FLAGS_SPINNING_V)) {
            NtReleaseKeyedEvent(GlobalKeyedEventHandle, &WaitBlock, 0, 0);
        }

        WaitBlock = NextWaitBlock;
        if (WaitBlock == NULL) {
            break;
        }
    }

    // if (OldIrql != DISPATCH_LEVEL) {
        // KeLowerIrql (OldIrql);
    // }
}

VOID
FASTCALL
ExpOptimizePushLockList (
    IN PEX_PUSH_LOCK PushLock,
    IN EX_PUSH_LOCK TopValue
    )
/*++

Routine Description:

    Walks the pushlock waiting list during contention and optimizes it

Arguments:

    PushLock - Push lock to be walked

    TopValue - Start of the chain (*PushLock)

Return Value:

    None

--*/
{
    EX_PUSH_LOCK OldValue, NewValue;
    PEX_PUSH_LOCK_WAIT_BLOCK WaitBlock, PreviousWaitBlock, FirstWaitBlock, NextWaitBlock;

    OldValue = TopValue;
    while (1) {
        if (!OldValue.Locked) {
            ExfWakePushLock (PushLock, OldValue);
            break;
        }

        WaitBlock = (PEX_PUSH_LOCK_WAIT_BLOCK)(OldValue.Value & ~(EX_PUSH_LOCK_PTR_BITS));

        FirstWaitBlock = WaitBlock;

        while (1) {

            NextWaitBlock = WaitBlock->Last;
            if (NextWaitBlock != NULL) {
                FirstWaitBlock->Last = NextWaitBlock;
                break;
            }

            PreviousWaitBlock = WaitBlock;
            WaitBlock = WaitBlock->Next;
            WaitBlock->Previous = PreviousWaitBlock;
        }

        NewValue.Value = OldValue.Value - EX_PUSH_LOCK_WAKING;
        ASSERT (NewValue.Locked);
        ASSERT (!NewValue.Waking);
        if ((NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                               NewValue.Ptr,
                                                               OldValue.Ptr)) == OldValue.Ptr) {
            break;
        }
        OldValue = NewValue;
    }
}

VOID
NTAPI
RtlAcquireSRWLockShared (
     __inout PEX_PUSH_LOCK PushLock
     )
/*++

Routine Description:

    Acquire a push lock shared

Arguments:

    PushLock - Push lock to be acquired

Return Value:

    None

--*/
{
    EX_PUSH_LOCK OldValue, NewValue, TopValue;
    EX_PUSH_LOCK_WAIT_BLOCK WaitBlock;
    BOOLEAN Optimize;
    RTL_BACKOFF Backoff = {0};

    OldValue = ReadForWriteAccess (PushLock);

    while (1) {
        //
        // If the lock is already held we need to wait if its not held shared
        //
        if (!OldValue.Locked || (!OldValue.Waiting && OldValue.Shared > 0)) {

            if (OldValue.Waiting) {
                NewValue.Value = OldValue.Value + EX_PUSH_LOCK_LOCK;
            } else {
                NewValue.Value = (OldValue.Value + EX_PUSH_LOCK_SHARE_INC) | EX_PUSH_LOCK_LOCK;
            }
            ASSERT (NewValue.Locked);
            NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                              NewValue.Ptr,
                                                              OldValue.Ptr);
            if (NewValue.Ptr == OldValue.Ptr) {
                break;
            }

            //
            // Use backof to limit memory bandwidth
            //
            RtlBackoff (&Backoff);

            NewValue = *PushLock;

        } else {
            WaitBlock.Flags = EX_PUSH_LOCK_FLAGS_SPINNING;
            WaitBlock.ShareCount = 0;
            Optimize = FALSE;
            WaitBlock.Previous = NULL;
 
            //
            // Move the sharecount to our wait block if need be.
            //
            if (OldValue.Waiting) {
                WaitBlock.Last = NULL;
                WaitBlock.Next = (PEX_PUSH_LOCK_WAIT_BLOCK)
                                     (OldValue.Value & ~EX_PUSH_LOCK_PTR_BITS);
                NewValue.Ptr = (PVOID)(((ULONG_PTR) &WaitBlock) |
                                    (OldValue.Value & (EX_PUSH_LOCK_LOCK | EX_PUSH_LOCK_MULTIPLE_SHARED)) |
                                    EX_PUSH_LOCK_WAITING | EX_PUSH_LOCK_WAKING);
                if (!OldValue.Waking) {
                    Optimize = TRUE;
                }
            } else {
                WaitBlock.Last = &WaitBlock;
                NewValue.Ptr = (PVOID)(((ULONG_PTR) &WaitBlock) |
                                    (OldValue.Value & EX_PUSH_LOCK_PTR_BITS) |
                                    EX_PUSH_LOCK_WAITING);
            }
             
            ASSERT (NewValue.Waiting);
            TopValue = NewValue;
            NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                              NewValue.Ptr,
                                                              OldValue.Ptr);

            if (NewValue.Ptr == OldValue.Ptr) {
                ULONG i;

                //
                // If we set the waiting bit then optimize the list
                //
                if (Optimize) {
                    ExpOptimizePushLockList (PushLock, TopValue);
                }

                //
                // It is safe to initialize the gate here, as the interlocked operation below forces 
                // a gate signal to always follow gate initialization.
                //
                //KeInitializeGate (&WaitBlock.WakeGate);

                for (i = ExPushLockSpinCount; i > 0; i--) {
                    if (((*(volatile LONG *)&WaitBlock.Flags)&EX_PUSH_LOCK_FLAGS_SPINNING) == 0) {
                        break;
                    }
                    YieldProcessor ();
                }

                if (InterlockedBitTestAndReset ((LONG*)&WaitBlock.Flags, EX_PUSH_LOCK_FLAGS_SPINNING_V)) {
					NtWaitForKeyedEvent(GlobalKeyedEventHandle, &WaitBlock, 0, 0);
                }

            } else {

                //
                // Use backof to limit memory bandwidth
                //
                RtlBackoff (&Backoff);

                NewValue = *PushLock;
            }

        }
        OldValue = NewValue;
    }

}

VOID
NTAPI
RtlAcquireSRWLockExclusive (
     __inout PEX_PUSH_LOCK PushLock
     )
/*++

Routine Description:

    Acquire a push lock exclusively

Arguments:

    PushLock - Push lock to be acquired

Return Value:

    None

--*/
{
    EX_PUSH_LOCK OldValue, NewValue, TopValue;
    EX_PUSH_LOCK_WAIT_BLOCK WaitBlock;
    BOOLEAN Optimize;
//#if defined (USE_EXP_BACKOFF)
    RTL_BACKOFF Backoff = {0};
//#endif

    OldValue = ReadForWriteAccess (PushLock);

    while (1) {
        //
        // If the lock is already held exclusively/shared or there are waiters then
        // we need to wait.
        //
        if ((OldValue.Value&EX_PUSH_LOCK_LOCK) == 0) {
            NewValue.Value = OldValue.Value + EX_PUSH_LOCK_LOCK;

            ASSERT (NewValue.Locked);

            NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                              NewValue.Ptr,
                                                              OldValue.Ptr);
            if (NewValue.Ptr == OldValue.Ptr) {
                break;
            }

//#if defined (USE_EXP_BACKOFF)

            //
            // Use backof to limit memory bandwidth
            //
            RtlBackoff (&Backoff);

            NewValue = *PushLock;

//#endif
        } else {
            WaitBlock.Flags = EX_PUSH_LOCK_FLAGS_EXCLUSIVE | EX_PUSH_LOCK_FLAGS_SPINNING;
            WaitBlock.Previous = NULL;
            Optimize = FALSE;
 
            //
            // Move the sharecount to our wait block if need be.
            //
            if (OldValue.Waiting) {
                WaitBlock.Last = NULL;
                WaitBlock.Next = (PEX_PUSH_LOCK_WAIT_BLOCK)
                                     (OldValue.Value & ~EX_PUSH_LOCK_PTR_BITS);
                WaitBlock.ShareCount = 0;
                NewValue.Ptr = (PVOID)(((ULONG_PTR) &WaitBlock) |
                                    (OldValue.Value & EX_PUSH_LOCK_PTR_BITS) |
                                    EX_PUSH_LOCK_WAITING | EX_PUSH_LOCK_WAKING |
                                    EX_PUSH_LOCK_LOCK);
                if (!OldValue.Waking) {
                    Optimize = TRUE;
                }
            } else {
                WaitBlock.Last = &WaitBlock;
                WaitBlock.ShareCount = (ULONG) OldValue.Shared;
                if (WaitBlock.ShareCount > 1) {
                    NewValue.Ptr = (PVOID)(((ULONG_PTR) &WaitBlock) | EX_PUSH_LOCK_WAITING |
                                                                      EX_PUSH_LOCK_LOCK |
                                                                      EX_PUSH_LOCK_MULTIPLE_SHARED);
                } else {
                    WaitBlock.ShareCount = 0;
                    NewValue.Ptr = (PVOID)(((ULONG_PTR) &WaitBlock) | EX_PUSH_LOCK_WAITING |
                                                                      EX_PUSH_LOCK_LOCK);
                }
            }
             
// #if DBG
            // WaitBlock.Signaled = FALSE;
            // WaitBlock.OldValue = OldValue.Ptr;
            // WaitBlock.NewValue = NewValue.Ptr;
            // WaitBlock.PushLock = PushLock;
// #endif
            ASSERT (NewValue.Waiting);
            ASSERT (OldValue.Locked);
            ASSERT (NewValue.Locked);

            TopValue = NewValue;
            NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                              NewValue.Ptr,
                                                              OldValue.Ptr);
            if (NewValue.Ptr == OldValue.Ptr) {
                ULONG i;

                //
                // If we set the waiting bit then optimize the list
                //
                if (Optimize) {
                    ExpOptimizePushLockList (PushLock, TopValue);
                }

                //KeInitializeGate (&WaitBlock.WakeGate);

                for (i = ExPushLockSpinCount; i > 0; i--) {
                    if (((*(volatile LONG *)&WaitBlock.Flags)&EX_PUSH_LOCK_FLAGS_SPINNING) == 0) {
                        break;
                    }
                    YieldProcessor ();
                }

                if (InterlockedBitTestAndReset (&WaitBlock.Flags, EX_PUSH_LOCK_FLAGS_SPINNING_V)) {

					NtWaitForKeyedEvent(GlobalKeyedEventHandle, &WaitBlock, 0, 0);
                    //KeWaitForGate (&WaitBlock.WakeGate, WrPushLock, KernelMode);

                }
                ASSERT ((WaitBlock.ShareCount == 0));
            } else {

                //
                // Use backof to limit memory bandwidth
                //
                RtlBackoff (&Backoff);

                NewValue = *PushLock;
            }
        }
        OldValue = NewValue;
    }

}

VOID
NTAPI
RtlReleaseSRWLockExclusive (
     __inout PEX_PUSH_LOCK PushLock
     )
/*++

Routine Description:

    Release a push lock that was acquired exclusive

Arguments:

    PushLock - Push lock to be released

Return Value:

    None

--*/
{
    EX_PUSH_LOCK OldValue, NewValue, TopValue;

    OldValue = ReadForWriteAccess (PushLock);

    while (1) {

        ASSERT (OldValue.Locked);
        ASSERT (OldValue.Waiting || OldValue.Shared == 0);

        if (OldValue.Waiting && !OldValue.Waking) {
            NewValue.Value = OldValue.Value - EX_PUSH_LOCK_LOCK + EX_PUSH_LOCK_WAKING;
            ASSERT (NewValue.Waking && !NewValue.Locked);
            TopValue = NewValue;
            if ((NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                                   NewValue.Ptr,
                                                                   OldValue.Ptr)) == OldValue.Ptr) {
                ExfWakePushLock (PushLock, TopValue);
                break;
            }
        } else {
            NewValue.Value = OldValue.Value - EX_PUSH_LOCK_LOCK;
            ASSERT (NewValue.Waking || !NewValue.Waiting);
            if ((NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                                   NewValue.Ptr,
                                                                   OldValue.Ptr)) == OldValue.Ptr) {
                break;
            }
        }
        OldValue = NewValue;
    }
}

VOID
NTAPI
RtlReleaseSRWLockShared (
     __inout PEX_PUSH_LOCK PushLock
     )
/*++

Routine Description:

    Release a push lock that was acquired shared

Arguments:

    PushLock - Push lock to be released

Return Value:

    None

--*/
{
    EX_PUSH_LOCK OldValue, NewValue, TopValue;
    PEX_PUSH_LOCK_WAIT_BLOCK WaitBlock, NextWaitBlock;

    OldValue = ReadForWriteAccess (PushLock);

    while (!OldValue.Waiting) {

        if (OldValue.Shared > 1) {
            NewValue.Value = OldValue.Value - EX_PUSH_LOCK_SHARE_INC;
        } else {
            NewValue.Value = 0;
        }

        if ((NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                               NewValue.Ptr,
                                                               OldValue.Ptr)) == OldValue.Ptr) {
            return;
        }
        OldValue = NewValue;
    }

    if (OldValue.MultipleShared) {
        WaitBlock = (PEX_PUSH_LOCK_WAIT_BLOCK) (OldValue.Value & ~(ULONG_PTR)EX_PUSH_LOCK_PTR_BITS);

        while (1) {

            NextWaitBlock = WaitBlock->Last;
            if (NextWaitBlock != NULL) {
                WaitBlock = NextWaitBlock;
                break;
            }

            WaitBlock = WaitBlock->Next;
        }

        ASSERT (WaitBlock->ShareCount > 0);
        ASSERT (WaitBlock->Flags&EX_PUSH_LOCK_FLAGS_EXCLUSIVE);

        if (InterlockedDecrement (&WaitBlock->ShareCount) > 0) {
            return;
        }
    }

    while (1) {

        if (OldValue.Waking) {
            NewValue.Value = OldValue.Value & ~(EX_PUSH_LOCK_LOCK|EX_PUSH_LOCK_MULTIPLE_SHARED);
            ASSERT (NewValue.Waking && !NewValue.Locked && !NewValue.MultipleShared);
            if ((NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                                   NewValue.Ptr,
                                                                   OldValue.Ptr)) == OldValue.Ptr) {
                return;
            }
        } else {
            NewValue.Value = (OldValue.Value & ~(EX_PUSH_LOCK_LOCK |
                                                 EX_PUSH_LOCK_MULTIPLE_SHARED)) |
                                      EX_PUSH_LOCK_WAKING;
            ASSERT (NewValue.Waking && !NewValue.Locked && !NewValue.MultipleShared);
            TopValue = NewValue;
            if ((NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                                   NewValue.Ptr,
                                                                   OldValue.Ptr)) == OldValue.Ptr) {
                ExfWakePushLock (PushLock, TopValue);
                return;
            }
        }

        OldValue = NewValue;
    }
}

VOID
NTAPI
RtlInitializeSRWLock(PEX_PUSH_LOCK Lock)
{
    *(PULONG_PTR)Lock = 0;
}

BOOLEAN
NTAPI
RtlTryAcquireSRWLockExclusive (
     IN PEX_PUSH_LOCK PushLock
     )
/*++

Routine Description:

    Try and acquire a push lock exclusively

Arguments:

    PushLock - Push lock to be acquired

Return Value:

    BOOLEAN - TRUE: Acquire was successful, FALSE: Lock was already acquired

--*/
{
#if defined (_WIN64)
    if (!InterlockedBitTestAndSet64 ((LONG64 *)&PushLock->Value, EX_PUSH_LOCK_LOCK_V)) {
#else
    if (!InterlockedBitTestAndSet ((LONG *)&PushLock->Value, EX_PUSH_LOCK_LOCK_V)) {
#endif
        ASSERT (PushLock->Locked);
        return TRUE;
    } else {
        return FALSE;
    }
}	

BOOLEAN
NTAPI
RtlTryAcquireSRWLockShared (
     __inout PEX_PUSH_LOCK PushLock
     )
/*++

Routine Description:

    Try to acquire a push lock shared without blocking

Arguments:

    PushLock - Push lock to be acquired

Return Value:

    BOOLEAN - TRUE, The lock was acquired, FALSE otherwise

--*/
{
    EX_PUSH_LOCK OldValue, NewValue;

    OldValue = ReadForWriteAccess (PushLock);

    while (1) {
        //
        // If the lock is already held we need to wait if its not held shared
        //
        if (!OldValue.Locked || (!OldValue.Waiting && OldValue.Shared > 0)) {

            if (OldValue.Waiting) {
                NewValue.Value = OldValue.Value + EX_PUSH_LOCK_LOCK;
            } else {
                NewValue.Value = (OldValue.Value + EX_PUSH_LOCK_SHARE_INC) | EX_PUSH_LOCK_LOCK;
            }
            ASSERT (NewValue.Locked);
            NewValue.Ptr = InterlockedCompareExchangePointer (&PushLock->Ptr,
                                                              NewValue.Ptr,
                                                              OldValue.Ptr);
            if (NewValue.Ptr == OldValue.Ptr) {
                return TRUE;
            }
        } else {
            return FALSE;
        }
        OldValue = NewValue;
    }

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
NTAPI 
RtlSleepConditionVariableSRW( 
	RTL_CONDITION_VARIABLE *variable, 
	RTL_SRWLOCK *lock,
    PLARGE_INTEGER timeout, 
	ULONG flags 
)
{
    NTSTATUS status;
    interlocked_xchg_add( (volatile long int *)&variable->Ptr, 1 );

    if (flags & RTL_CONDITION_VARIABLE_LOCKMODE_SHARED)
        RtlReleaseSRWLockShared( lock );
    else
        RtlReleaseSRWLockExclusive( lock );

    status = NtWaitForKeyedEvent( GlobalKeyedEventHandle, &variable->Ptr, FALSE, timeout );
    if (status != STATUS_SUCCESS)
    {
        if (!interlocked_dec_if_nonzero( (volatile long int *)&variable->Ptr ))
            status = NtWaitForKeyedEvent( GlobalKeyedEventHandle, &variable->Ptr, FALSE, NULL );
    }

    if (flags & RTL_CONDITION_VARIABLE_LOCKMODE_SHARED)
        RtlAcquireSRWLockShared( lock );
    else
        RtlAcquireSRWLockExclusive( lock );
    return status;
}