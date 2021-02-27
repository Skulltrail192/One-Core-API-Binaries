/*
 *	Process synchronisation
 *
 * Copyright 1996, 1997, 1998 Marcus Meissner
 * Copyright 1997, 1999 Alexandre Julliard
 * Copyright 1999, 2000 Juergen Schmied
 * Copyright 2003 Eric Pouech
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
 */
 
 #include <main.h>
 
#define EX_PUSH_LOCK_FLAGS_EXCLUSIVE  (0x1)
#define EX_PUSH_LOCK_FLAGS_SPINNING_V (0x1)
#define EX_PUSH_LOCK_FLAGS_SPINNING   (0x2)

#define FAST_M_WAKE		256
#define FAST_M_WAIT		512

#define EX_PUSH_LOCK_PTR_BITS ((ULONG_PTR)0xf)

#define ReadForWriteAccess(p) (*(p))

ULONG ExPushLockSpinCount = 1024;

typedef struct _RTL_BACKOFF {
   ULONG Delay;
   ULONG Salt;
} RTL_BACKOFF, *PRTL_BACKOFF;

void NTAPI RtlBackoff(unsigned int *pCount)  
{  
    unsigned int nBackCount;  
	unsigned int i;
  
    nBackCount = *pCount;  
    if ( nBackCount )  
    {  
        if ( nBackCount < 0x1FFF )  
            nBackCount *= 2;  
    }  
    else  
    {  
        // __readfsdword(24) --> teb  
        // teb+48           --> peb  
        // peb+100          --> NumberOfProcessors  
        if ( *(DWORD *)(*(DWORD *)(__readfsdword(24) + 48) + 100) == 1 ) // 获取cpu个数(核数)  
            return;  
  
        // ================for ia64================  
        // NtCurrentTeb()  
        // teb+48h  --> tid(64bits)  
        // teb+60h  --> peb(64bits)  
        // peb+b8h  --> NumberOfProcessors(32bits)  
  
        nBackCount = 64;  
    }  
  
    nBackCount = ((nBackCount - 1) & __rdtsc()) + nBackCount;  
  
    for ( i = 0; i < nBackCount; i++ )  
    {  
        YieldProcessor();  
    }  
  
    return;  
}  

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
    //*(PULONG_PTR)Lock = 0;
	Lock->Ptr = NULL;
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