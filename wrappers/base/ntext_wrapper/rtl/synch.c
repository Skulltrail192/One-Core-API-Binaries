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

#define COND_VAR_UNUSED_FLAG         ((ULONG_PTR)1)
#define COND_VAR_LOCKED_FLAG         ((ULONG_PTR)2)
#define COND_VAR_FLAGS_MASK          ((ULONG_PTR)3)
#define COND_VAR_ADDRESS_MASK        (~COND_VAR_FLAGS_MASK)

#define RtlpWaitOnAddressSpinCount 1024

typedef SIZE_T SYNCSTATUS;

#define CVF_Full	7	//唤醒申请已满，全部唤醒
#define CVF_Link	8	//修改链表的操作进行中

typedef struct _COND_VAR_WAIT_ENTRY
{
    /* ListEntry must have an alignment of at least 32-bits, since we
       want COND_VAR_ADDRESS_MASK to cover all of the address. */
    LIST_ENTRY ListEntry;
    PVOID WaitKey;
    BOOLEAN ListRemovalHandled;
} COND_VAR_WAIT_ENTRY, * PCOND_VAR_WAIT_ENTRY;

typedef struct _ADDRESS_WAIT_BLOCK
{
	volatile void* Address;
	//因为Windows 8以及更高版本才支持 ZwWaitForAlertByThreadId，所以我们直接把 ThreadId 砍掉了，反正没鸟用
	//ULONG_PTR            ThreadId;

	// 它是后继
	struct _ADDRESS_WAIT_BLOCK* back;
	// 它是前驱
	struct _ADDRESS_WAIT_BLOCK* notify;
	// 似乎指向Root，但是Root时才指向自己，其余情况为 nullptr，这是一种安全性？
	struct _ADDRESS_WAIT_BLOCK* next;
	volatile size_t         flag;
} ADDRESS_WAIT_BLOCK;

#define CONTAINING_COND_VAR_WAIT_ENTRY(address, field) \
    CONTAINING_RECORD(address, COND_VAR_WAIT_ENTRY, field)
	
#define ADDRESS_GET_BLOCK(AW) ((ADDRESS_WAIT_BLOCK*)((SIZE_T)(AW) & (~(SIZE_T)(0x3))))

HANDLE _GlobalKeyedEventHandle = 0;

static 
NTSTATUS 
RtlpWaitOnAddressWithTimeout(
	ADDRESS_WAIT_BLOCK* pWaitBlock, 
	LARGE_INTEGER *TimeOut
);

static 
void 
RtlpWaitOnAddressRemoveWaitBlock(
	ADDRESS_WAIT_BLOCK* pWaitBlock
);

static void RtlpWaitOnAddressRemoveWaitBlock(ADDRESS_WAIT_BLOCK* pWaitBlock);

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

/* INTERNAL FUNCTIONS ********************************************************/

FORCEINLINE
ULONG_PTR
InternalCmpXChgCondVarAcq(IN OUT PRTL_CONDITION_VARIABLE ConditionVariable,
                          IN ULONG_PTR Exchange,
                          IN ULONG_PTR Comperand)
{
    return (ULONG_PTR)InterlockedCompareExchangePointerAcquire(&ConditionVariable->Ptr,
                                                               (PVOID)Exchange,
                                                               (PVOID)Comperand);
}

FORCEINLINE
ULONG_PTR
InternalCmpXChgCondVarRel(IN OUT PRTL_CONDITION_VARIABLE ConditionVariable,
                          IN ULONG_PTR Exchange,
                          IN ULONG_PTR Comperand)
{
    return (ULONG_PTR)InterlockedCompareExchangePointerRelease(&ConditionVariable->Ptr,
                                                               (PVOID)Exchange,
                                                               (PVOID)Comperand);
}

/* GLOBALS *******************************************************************/

extern HANDLE GlobalKeyedEventHandle;
static HANDLE WaitOnAddressKeyedEventHandle;
static RTL_RUN_ONCE init_once_woa = RTL_RUN_ONCE_INIT; 

VOID
RtlpInitializeKeyedEvent(VOID)
{
    ASSERT(GlobalKeyedEventHandle == NULL);
    NtCreateKeyedEvent(&GlobalKeyedEventHandle, EVENT_ALL_ACCESS, NULL, 0);
}

static DWORD WINAPI
RtlpInitializeWaitOnAddressKeyedEvent( RTL_RUN_ONCE *once, void *param, void **context )
{
    NtCreateKeyedEvent(&WaitOnAddressKeyedEventHandle, GENERIC_READ|GENERIC_WRITE, NULL, 0);
	return TRUE; 
}

VOID
RtlpCloseKeyedEvent(VOID)
{
    ASSERT(GlobalKeyedEventHandle != NULL);
    NtClose(GlobalKeyedEventHandle);
    GlobalKeyedEventHandle = NULL;
}

static ULONG_PTR* GetBlockByWaitOnAddressHashTable(LPVOID Address)
{
	static volatile ULONG_PTR WaitOnAddressHashTable[128];

	size_t Index = ((size_t)Address >> 5) & 0x7F;

	return &WaitOnAddressHashTable[Index];
}

static HANDLE __fastcall GetGlobalKeyedEventHandle()
{
	HANDLE KeyedEventHandle;
	//Windows XP等平台则 使用系统自身的 CritSecOutOfMemoryEvent，Vista或者更高平台 我们直接返回 nullptr 即可。
	if (NtCurrentTeb()->ProcessEnvironmentBlock->OSMajorVersion < 6)
	{
		if (_GlobalKeyedEventHandle == 0)
		{
			const wchar_t Name[] = L"\\KernelObjects\\CritSecOutOfMemoryEvent";

			UNICODE_STRING ObjectName = {sizeof(Name) - sizeof(wchar_t),sizeof(Name) - sizeof(wchar_t) ,(PWSTR)Name };
			OBJECT_ATTRIBUTES attr = { sizeof(attr),0,&ObjectName };

			if (NtOpenKeyedEvent(&KeyedEventHandle, MAXIMUM_ALLOWED, &attr) < 0)
			{
				RtlRaiseStatus(STATUS_RESOURCE_NOT_OWNED);
			}

			if (InterlockedCompareExchange((size_t*)&_GlobalKeyedEventHandle, (size_t)KeyedEventHandle, (size_t)0))
			{
				RtlFreeHeap( RtlGetProcessHeap(), 0, KeyedEventHandle );
			}
		}

		return _GlobalKeyedEventHandle;
	}

	return 0;
}

static void RtlpWaitOnAddressWakeEntireList(ADDRESS_WAIT_BLOCK* pBlock)
{
	HANDLE GlobalKeyedEventHandle = GetGlobalKeyedEventHandle();

	for (; pBlock;)
	{
		ADDRESS_WAIT_BLOCK* Tmp = pBlock->back;

		if (InterlockedExchange(&pBlock->flag, 2) == 0)
		{
			NtReleaseKeyedEvent(GlobalKeyedEventHandle, pBlock, 0, 0);
		}

		pBlock = Tmp;
	}
}

static void RtlpOptimizeWaitOnAddressWaitList(volatile ULONG_PTR* ppFirstBlock)
{
	ULONG_PTR Current = *ppFirstBlock;
	ADDRESS_WAIT_BLOCK* pBlock;
	ADDRESS_WAIT_BLOCK* pItem;
	ADDRESS_WAIT_BLOCK* Tmp;
	size_t Last;

	for (;;)
	{
		pBlock = ADDRESS_GET_BLOCK(Current);

		for (pItem = pBlock;;)
		{
			if (pItem->next != 0)
			{
				pBlock->next = pItem->next;
				break;
			}

			Tmp = pItem;
			pItem = pItem->back;
			pItem->notify = Tmp;
		}

		Last = InterlockedCompareExchange(ppFirstBlock, (Current & 1) == 0 ? (size_t)(pBlock) : 0, Current);

		if (Last == Current)
		{
			if(Current & 1)
			{
				RtlpWaitOnAddressWakeEntireList(pBlock);
			}

			return;
		}


		Current = Last;
	}
}

static void RtlpAddWaitBlockToWaitList(ADDRESS_WAIT_BLOCK* pWaitBlock)
{
	ULONG_PTR* ppFirstBlock = GetBlockByWaitOnAddressHashTable((LPVOID)pWaitBlock->Address);
	ULONG_PTR Current = *ppFirstBlock;	
	size_t New;
	ADDRESS_WAIT_BLOCK* back;
	size_t Last;

	for (;;)
	{
		New = (size_t)(pWaitBlock) | ((size_t)(Current) & 0x3);

		back = ADDRESS_GET_BLOCK(Current);
	    pWaitBlock->back = back;
		if (back)
		{
			New |= 0x2;

			pWaitBlock->next = 0;
		}
		else
		{
			pWaitBlock->next = pWaitBlock;
		}

		Last = InterlockedCompareExchange(ppFirstBlock, New, Current);

		if (Last == Current)
		{
			//0x2状态发生变化 才需要重新优化锁。
			if ((Current ^ New) & 0x2)
			{
				RtlpOptimizeWaitOnAddressWaitList(ppFirstBlock);
			}

			return;
		}

		Current = Last;
	}
}

/***********************************************************************
 *           RtlWaitOnAddress   (NTDLL.@)
 */
// NTSTATUS WINAPI RtlWaitOnAddress( const void *addr, const void *cmp, SIZE_T size,
                                  // const LARGE_INTEGER *timeout )
// {
    // // switch (size)
    // // {
        // // case 1:
            // // if (*(const UCHAR *)addr != *(const UCHAR *)cmp)
                // // return STATUS_SUCCESS;
            // // break;
        // // case 2:
            // // if (*(const USHORT *)addr != *(const USHORT *)cmp)
                // // return STATUS_SUCCESS;
            // // break;
        // // case 4:
            // // if (*(const ULONG *)addr != *(const ULONG *)cmp)
                // // return STATUS_SUCCESS;
            // // break;
        // // case 8:
            // // if (*(const ULONG64 *)addr != *(const ULONG64 *)cmp)
                // // return STATUS_SUCCESS;
            // // break;
        // // default:
            // // return STATUS_INVALID_PARAMETER;
    // // }

    // // RtlRunOnceExecuteOnce( &init_once_woa, 
						// // RtlpInitializeWaitOnAddressKeyedEvent, NULL, NULL );
    // // return NtWaitForKeyedEvent( WaitOnAddressKeyedEventHandle, addr, 0, timeout );
			//参数检查，AddressSize 只能 为 1,2,4,8
// NTSTATUS WINAPI RtlWaitOnAddress( const void *addr, const void *cmp, SIZE_T size,
                                  // const LARGE_INTEGER *timeout )
	
static 
NTSTATUS 
RtlpWaitOnAddressWithTimeout(
	ADDRESS_WAIT_BLOCK* pWaitBlock, 
	LARGE_INTEGER *TimeOut
)
{
	NTSTATUS Status;
	ULONG SpinCount;
	HANDLE GlobalKeyedEventHandle;
	//单核 我们无需自旋，直接进入等待过程即可
	if (NtCurrentTeb()->ProcessEnvironmentBlock->NumberOfProcessors > 1 && RtlpWaitOnAddressSpinCount)
	{
		for (SpinCount = 0; SpinCount < RtlpWaitOnAddressSpinCount;++SpinCount)
		{
			if ((pWaitBlock->flag & 1) == 0)
			{
				//自旋过程中，等到了信号改变
				return STATUS_SUCCESS;
			}

			YieldProcessor();
		}
	}

	if (!_interlockedbittestandreset((volatile long *)&pWaitBlock->flag, 0))
	{
		//本来我是拒绝的，但是运气好，状态已经发生了反转
		return STATUS_SUCCESS;
	}

	GlobalKeyedEventHandle = GetGlobalKeyedEventHandle();

	Status = NtWaitForKeyedEvent(GlobalKeyedEventHandle, pWaitBlock, 0, TimeOut);

	if (Status == STATUS_TIMEOUT)
	{
		if (InterlockedExchange(&pWaitBlock->flag, 4) == 2)
		{
			Status = NtWaitForKeyedEvent(GlobalKeyedEventHandle, pWaitBlock, 0, 0);
		}
		else
		{
			RtlpWaitOnAddressRemoveWaitBlock(pWaitBlock);
		}
	}

	return Status;
}	
	
static void RtlpWaitOnAddressRemoveWaitBlock(ADDRESS_WAIT_BLOCK* pWaitBlock)
{
	HANDLE GlobalKeyedEventHandle = GetGlobalKeyedEventHandle();

	ULONG_PTR* ppFirstBlock = GetBlockByWaitOnAddressHashTable((LPVOID)pWaitBlock->Address);
	ULONG_PTR Current = *ppFirstBlock;
	size_t Last;
	size_t New;
	ADDRESS_WAIT_BLOCK* pBlock;
	ADDRESS_WAIT_BLOCK* pItem;
	ADDRESS_WAIT_BLOCK* pNotify;
	ADDRESS_WAIT_BLOCK* Tmp;
	BOOL bFind;

	for (; Current; Current = Last)
	{
		if (Current & 2)
		{
			Last = InterlockedCompareExchange(ppFirstBlock, Current | 1, Current);

			if (Last == Current)
			{
				break;
			}
		}else{
			New = Current | 0x2;
			Last = InterlockedCompareExchange(ppFirstBlock, New, Current);

			if (Last == Current)
			{
				Current = New;

				bFind = FALSE;

							//同步成功！
				pBlock = ADDRESS_GET_BLOCK(New);
				pItem = pBlock;

				pNotify = pBlock->notify;				

				do
				{
					Tmp = pBlock->back;

					if (pBlock != pWaitBlock)
					{
						pBlock->notify = pNotify;
						pNotify = pBlock;


						pBlock = Tmp;
						Tmp = pItem;
						continue;
					}

					bFind = TRUE;


					if (pBlock != pItem)
					{
						pNotify->back = Tmp;
						if (Tmp)
							Tmp->notify = pNotify;
						else
							pNotify->next = pNotify;

						pBlock = Tmp;
						Tmp = pItem;
						continue;
					}

					New = (size_t)(pBlock->back);
					if (Tmp)
					{
						New = (size_t)(Tmp) ^ (Current ^ (size_t)(Tmp)) & 0x3;
					}

					Last = InterlockedCompareExchange(ppFirstBlock, New, Current);

					if (Last == Current)
					{
						if (New == 0)
							return;

						Tmp->notify = 0;
						pBlock = Tmp;
					}else{
						Current = Last;

						Tmp = pBlock = ADDRESS_GET_BLOCK(Current);
						pNotify = pBlock->notify;
					}


					pItem = Tmp;
				} while (pBlock);
							

				if (bFind == FALSE && InterlockedExchange(&pWaitBlock->flag, 0) != 2)
				{
					NtWaitForKeyedEvent(GlobalKeyedEventHandle, pWaitBlock, 0, 0);
				}

				Tmp->next = pNotify;

				for (;;)
				{
					Last = InterlockedCompareExchange(ppFirstBlock, (Current & 1) == 0 ? (size_t)(ADDRESS_GET_BLOCK(Current)) : 0, Current);

					if (Last == Current)
						break;

					Current = Last;
				}

				if (Current & 1)
					RtlpWaitOnAddressWakeEntireList(ADDRESS_GET_BLOCK(Current));


					return;
			}
		}
	}

	if (InterlockedExchange(&pWaitBlock->flag, 1) == 2)
		return;

	RtlpWaitOnAddressWithTimeout(pWaitBlock, 0);
}  
								  
NTSTATUS 
WINAPI
RtlWaitOnAddress( 
	const void *Address, 
	const void *CompareAddress, 
	SIZE_T AddressSize,
    const LARGE_INTEGER *TimeOut )	
{		
	BOOL bSame;
	NTSTATUS Status;
	ADDRESS_WAIT_BLOCK WaitBlock;
			
	if (AddressSize > 8 || AddressSize == 0 || ((AddressSize - 1) & AddressSize) != 0)
	{
		return STATUS_INVALID_PARAMETER;
	}
	
	WaitBlock.Address = Address;
	WaitBlock.back = 0;
	WaitBlock.notify = 0;
	WaitBlock.next = 0;
	WaitBlock.flag = 1;

	RtlpAddWaitBlockToWaitList(&WaitBlock);
			
	switch (AddressSize)
	{
		case 1:
			bSame = *(volatile byte*)Address == *(volatile byte*)CompareAddress;
			break;
		case 2:
			bSame = *(volatile WORD*)Address == *(volatile WORD*)CompareAddress;
			break;
		case 4:
			bSame = *(volatile DWORD*)Address == *(volatile DWORD*)CompareAddress;
			break;
		default:
			//case 8:
#if _WIN64
			//64位自身能保证操作的原子性
			bSame = *(volatile unsigned long long*)Address == *(volatile unsigned long long*)CompareAddress;
#else
			bSame = InterlockedCompareExchange64((volatile long long*)Address, 0, 0) == *(volatile long long*)CompareAddress;
#endif
			break;
	}

	if (!bSame)
	{
		//结果不相同，我们从等待队列移除
		RtlpWaitOnAddressRemoveWaitBlock(&WaitBlock);
		return TRUE;
	}			

	Status = RtlpWaitOnAddressWithTimeout(&WaitBlock, TimeOut);

	return Status;	
}

/***********************************************************************
 *           RtlWakeAddressAll    (NTDLL.@)
 */
void WINAPI RtlWakeAddressAll( const void *addr )
{
    LARGE_INTEGER now;

    RtlRunOnceExecuteOnce( &init_once_woa, RtlpInitializeWaitOnAddressKeyedEvent, NULL, NULL );
    NtQuerySystemTime( &now );
    while (NtReleaseKeyedEvent( WaitOnAddressKeyedEventHandle, addr, 0, &now ) == STATUS_SUCCESS) {}
}

/***********************************************************************
 *           RtlWakeAddressSingle (NTDLL.@)
 */
void WINAPI RtlWakeAddressSingle( const void *addr )
{
    LARGE_INTEGER now;

    RtlRunOnceExecuteOnce( &init_once_woa, RtlpInitializeWaitOnAddressKeyedEvent, NULL, NULL );
    NtQuerySystemTime( &now );
    NtReleaseKeyedEvent( WaitOnAddressKeyedEventHandle, addr, 0, &now );
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

/* SRW locks implementation
 *
 * The memory layout used by the lock is:
 *
 * 32 31            16               0
 *  ________________ ________________
 * | X| #exclusive  |    #shared     |
 *  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
 * Since there is no space left for a separate counter of shared access
 * threads inside the locked section the #shared field is used for multiple
 * purposes. The following table lists all possible states the lock can be
 * in, notation: [X, #exclusive, #shared]:
 *
 * [0,   0,   N] -> locked by N shared access threads, if N=0 it's unlocked
 * [0, >=1, >=1] -> threads are requesting exclusive locks, but there are
 * still shared access threads inside. #shared should not be incremented
 * anymore!
 * [1, >=1, >=0] -> lock is owned by an exclusive thread and the #shared
 * counter can be used again to count the number of threads waiting in the
 * queue for shared access.
 *
 * the following states are invalid and will never occur:
 * [0, >=1,   0], [1,   0, >=0]
 *
 * The main problem arising from the fact that we have no separate counter
 * of shared access threads inside the locked section is that in the state
 * [0, >=1, >=1] above we cannot add additional waiting threads to the
 * shared access queue - it wouldn't be possible to distinguish waiting
 * threads and those that are still inside. To solve this problem the lock
 * uses the following approach: a thread that isn't able to allocate a
 * shared lock just uses the exclusive queue instead. As soon as the thread
 * is woken up it is in the state [1, >=1, >=0]. In this state it's again
 * possible to use the shared access queue. The thread atomically moves
 * itself to the shared access queue and releases the exclusive lock, so
 * that the "real" exclusive access threads have a chance. As soon as they
 * are all ready the shared access threads are processed.
 */

#define SRWLOCK_MASK_IN_EXCLUSIVE     0x80000000
#define SRWLOCK_MASK_EXCLUSIVE_QUEUE  0x7fff0000
#define SRWLOCK_MASK_SHARED_QUEUE     0x0000ffff
#define SRWLOCK_RES_EXCLUSIVE         0x00010000
#define SRWLOCK_RES_SHARED            0x00000001

#ifdef WORDS_BIGENDIAN
#define srwlock_key_exclusive(lock)   (&lock->Ptr)
#define srwlock_key_shared(lock)      ((void *)((char *)&lock->Ptr + 2))
#else
#define srwlock_key_exclusive(lock)   ((void *)((char *)&lock->Ptr + 2))
#define srwlock_key_shared(lock)      (&lock->Ptr)
#endif

static inline void srwlock_check_invalid( unsigned int val )
{
    /* Throw exception if it's impossible to acquire/release this lock. */
    if ((val & SRWLOCK_MASK_EXCLUSIVE_QUEUE) == SRWLOCK_MASK_EXCLUSIVE_QUEUE ||
            (val & SRWLOCK_MASK_SHARED_QUEUE) == SRWLOCK_MASK_SHARED_QUEUE)
        RtlRaiseStatus(STATUS_RESOURCE_NOT_OWNED);
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

static inline void srwlock_leave_exclusive( RTL_SRWLOCK *lock, unsigned int val )
{
    /* Used when a thread leaves an exclusive section. If there are other
     * exclusive access threads they are processed first, followed by
     * the shared waiters. */
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
void WINAPI RtlInitializeConditionVariable( RTL_CONDITION_VARIABLE *variable )
{
    variable->Ptr = NULL;
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
void WINAPI RtlWakeConditionVariable( RTL_CONDITION_VARIABLE *variable )
{
    if (interlocked_dec_if_nonzero( (int *)&variable->Ptr ))
        NtReleaseKeyedEvent( GlobalKeyedEventHandle, &variable->Ptr, FALSE, NULL );
}

/***********************************************************************
 *           RtlWakeAllConditionVariable   (NTDLL.@)
 *
 * See WakeConditionVariable, wakes up all waiting threads.
 */
void NTAPI RtlWakeAllConditionVariable( RTL_CONDITION_VARIABLE *variable )
{
    int val = interlocked_xchg( (int *)&variable->Ptr, 0 );
    while (val-- > 0)
        NtReleaseKeyedEvent( GlobalKeyedEventHandle, &variable->Ptr, FALSE, NULL );
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
RtlSleepConditionVariableCS(IN OUT PRTL_CONDITION_VARIABLE ConditionVariable,
                            IN OUT PRTL_CRITICAL_SECTION CriticalSection,
                            IN const LARGE_INTEGER * TimeOut OPTIONAL)
{
    NTSTATUS status;
    interlocked_xchg_add( (int *)&ConditionVariable->Ptr, 1 );
    RtlLeaveCriticalSection( CriticalSection );

    status = NtWaitForKeyedEvent( GlobalKeyedEventHandle, &ConditionVariable->Ptr, FALSE, TimeOut );
    if (status != STATUS_SUCCESS)
    {
        if (!interlocked_dec_if_nonzero( (int *)&ConditionVariable->Ptr ))
            status = NtWaitForKeyedEvent( GlobalKeyedEventHandle, &ConditionVariable->Ptr, FALSE, NULL );
    }

    RtlEnterCriticalSection( CriticalSection );
    return status;
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
NTSTATUS NTAPI RtlSleepConditionVariableSRW( RTL_CONDITION_VARIABLE *variable, RTL_SRWLOCK *lock,
                                              PLARGE_INTEGER timeout, ULONG flags )
{
    NTSTATUS status;
    interlocked_xchg_add( (int *)&variable->Ptr, 1 );

    if (flags & RTL_CONDITION_VARIABLE_LOCKMODE_SHARED)
        RtlReleaseSRWLockShared( lock );
    else
        RtlReleaseSRWLockExclusive( lock );

    status = NtWaitForKeyedEvent( GlobalKeyedEventHandle, &variable->Ptr, FALSE, timeout );
    if (status != STATUS_SUCCESS)
    {
        if (!interlocked_dec_if_nonzero( (int *)&variable->Ptr ))
            status = NtWaitForKeyedEvent( GlobalKeyedEventHandle, &variable->Ptr, FALSE, NULL );
    }

    if (flags & RTL_CONDITION_VARIABLE_LOCKMODE_SHARED)
        RtlAcquireSRWLockShared( lock );
    else
        RtlAcquireSRWLockExclusive( lock );
    return status;
}