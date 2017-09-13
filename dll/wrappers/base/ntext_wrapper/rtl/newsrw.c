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

#define SRWLOCK_MASK_IN_EXCLUSIVE     0x80000000
#define SRWLOCK_MASK_EXCLUSIVE_QUEUE  0x7fff0000
#define SRWLOCK_MASK_SHARED_QUEUE     0x0000ffff
#define SRWLOCK_RES_EXCLUSIVE         0x00010000
#define SRWLOCK_RES_SHARED            0x00000001

#define SRWLockSpinCount 1024  
#define Busy_Lock       1   // 已经有人获取了锁  
#define Wait_Lock       2   // 有人等待锁  
#define Release_Lock    4   // 说明已经有人释放一次锁  
#define Mixed_Lock      8   // 共享锁、独占锁并存  

#define EXTRACT_ADDR(s) ((s) & (~0xf)) 

typedef struct _SyncItem  
{  
    struct _SyncItem* back;  
    struct _SyncItem* notify;  
    struct _SyncItem* next;  
    size_t shareCount;  
    size_t flag;      
}_SyncItem;  

void NTAPI RtlpWakeSRWLock(RTL_SRWLOCK* pSRWLock, size_t st);  
void NTAPI RtlBackoff(unsigned int *pCount);  
void NTAPI RtlpOptimizeSRWLockList(RTL_SRWLOCK* pSRWLock, size_t st);  

void NTAPI RtlInitializeSRWLock(RTL_SRWLOCK* pSRWLock)  
{  
    pSRWLock->Ptr = 0;  
}  
  
 void NTAPI RtlAcquireSRWLockExclusive(RTL_SRWLOCK *pSRWLock)  
{  
    __declspec( align( 16 ) ) _SyncItem syn = {0};  
  
    size_t newStatus;   
    size_t curStatus;   
    size_t lastStatus;   
    unsigned int nBackOff = 0;   
    char bOptimize;  
	int i;
  
    if ( _interlockedbittestandset((volatile LONG *)pSRWLock, 0) )  
    {  
        lastStatus = (size_t)pSRWLock->Ptr;  
  
        while (1)  
        {  
            if ( lastStatus & Busy_Lock )// locked  
            {  
    //          if ( RtlpWaitCouldDeadlock() )  
    //              ZwTerminateProcess((HANDLE)0xFFFFFFFF, -1073741749);  
                syn.flag = 3;  
                syn.next = 0;  
                bOptimize = 0;  
  
                if ( lastStatus & Wait_Lock )// someone is waiting the lock earlier than me.  
                {  
                    syn.notify = NULL;  
                    syn.shareCount = 0;  
                    syn.back = (_SyncItem *)(EXTRACT_ADDR(lastStatus));  
                    newStatus = (size_t)&syn | lastStatus & 8 | 7;// (8==1000b，继承混合等待的状态标志) (7==0111b)  
                      
                    if ( !(lastStatus & Release_Lock) )// v15 & 0100b, lock is not released now  
                        bOptimize = 1;  
                }  
                else// i am the first one to wait the lock.(另外，全部是share-lock的情况下，也不存在有人等待的情况)  
                {  
                    syn.notify = &syn;// i must be the next one who want to wait the lock         
                    syn.shareCount = (size_t)lastStatus >> 4;  
                    if ( syn.shareCount > 1 )  
                    {// share locked by other thread  
                        newStatus = (size_t)&syn | 0xB;  
                    }  
                    else  
                    {// i am the first one want owner-lock  
                        newStatus = (size_t)&syn | 3;  
                    }  
                }  
      
                //if value in lock has not been changed by other thread,   
                // change it with my value!  
                curStatus = _InterlockedCompareExchange((volatile LONG *)pSRWLock, newStatus, lastStatus);  
      
                if ( curStatus != lastStatus )// not changed by me  
                {  
                    RtlBackoff(&nBackOff);  
                    lastStatus = (size_t)pSRWLock->Ptr;  
                    continue;  
                }  
      
                if ( bOptimize )  
                    RtlpOptimizeSRWLockList(pSRWLock, newStatus);  
      
                for ( i = SRWLockSpinCount; i>0; --i )  
                {  
                    // flag(bit1) can be reset by release-lock operation in other thread  
                    if ( !(syn.flag & 2) )  
                        break;  
                    YieldProcessor();  
                }  
  
                // if flag(bit1) reset by Release operation,  
                // no need to wait event anymore  
                if ( _interlockedbittestandreset((volatile LONG *)&syn.flag, 1u) )  
                    NtWaitForKeyedEvent(GlobalKeyedEventHandle, &syn, 0, 0);  
      
                lastStatus = curStatus;  
            }  
            else  
            {  
                // try to get lock  
                if ( _InterlockedCompareExchange((volatile LONG *)pSRWLock, lastStatus + 1, lastStatus) == lastStatus )  
                    return;// get lock successfully.  
      
                // status of the lock was changed by other thread  
                // get lock failed  
                RtlBackoff(&nBackOff);  
                lastStatus = (size_t)pSRWLock->Ptr;  
            }  
        }  
    }  
  
    return;  
}  
   
void NTAPI RtlAcquireSRWLockShared(RTL_SRWLOCK *pSRWLock)  
{  
    __declspec( align( 16 ) ) _SyncItem syn = {0};  
  
    size_t newStatus;   
    size_t curStatus;   
    size_t lastStatus;   
    unsigned int nBackOff = 0;   
    char bOptimize; 
	int i;	
  
    lastStatus = _InterlockedCompareExchange((volatile LONG *)pSRWLock, 17, 0);  
      
    if ( lastStatus )// someone is looking at the lock  
    {  
        while ( 1 )  
        {  
            // get_share_lock 只有在有人独占锁的情况才会等待  
            // x & 1，有人获取了锁  
            // x & Wait_Lock != 0，有人在等待锁释放（必定存在独占锁）  
            // (x & 0xFFFFFFF0) == 0，有人独占锁，但是可能还没有人等待  
            if ( lastStatus & Busy_Lock && ((lastStatus & Wait_Lock) != 0 || !(EXTRACT_ADDR(lastStatus))) )  
            {  
//              if ( RtlpWaitCouldDeadlock() )  
//                  ZwTerminateProcess((HANDLE)0xFFFFFFFF, -1073741749);  
                syn.flag = 2;  
                syn.shareCount = 0;  
                bOptimize = 0;  
                syn.next = 0;  
  
                if ( lastStatus & Wait_Lock )// someone is waiting the lock earlier than me.  
                {  
                    syn.back = (_SyncItem *)(EXTRACT_ADDR(lastStatus));  
                    newStatus = (size_t)&syn | lastStatus & 9 | 6;// 9==1001 , 6==0110（因为lastStatus的bit0必为1，等价于(x & 8) | 7）  
                    syn.notify = NULL;  
                    if ( !(lastStatus & Release_Lock) )//(bit2 not set) lock is not released now.  
                        bOptimize = 1;  
                }  
                else // i am the first one to wait the lock.  
                {  
                    syn.notify = &syn;  
                    newStatus = (size_t)&syn | 3;// 3==0011b  
                }  
  
                curStatus = _InterlockedCompareExchange((volatile LONG *)pSRWLock, newStatus, lastStatus);  
                if ( curStatus == lastStatus )  
                {  
                    if ( bOptimize )  
                    {  
                        RtlpOptimizeSRWLockList(pSRWLock, newStatus);  
                    }  
  
                    for(i = SRWLockSpinCount; i > 0; --i)  
                    {  
                        if ( !(syn.flag & 2) )// flag(bit1) can be reset by release-lock operation  
                            break;  
                        YieldProcessor();  
                    }  
                    // if flag(bit1) is reset by release-lock operation  
                    // no need to wait event anymore  
                    if ( _interlockedbittestandreset((volatile LONG *)&syn.flag, 1u) )  
                        NtWaitForKeyedEvent(GlobalKeyedEventHandle, &syn, 0, 0);  
                }  
                else  
                {  
                    RtlBackoff(&nBackOff);  
                    curStatus = (size_t)pSRWLock->Ptr;  
                }  
            }  
            else  
            {  
                if ( lastStatus & Wait_Lock )// 2 == 0010b，有人等待锁，但是没有进入if，说明bit0已经被清除  
                    newStatus = lastStatus + 1;// （有人处于过渡态，直接获取锁，不管他是哪种类型）  
                else// 当前是共享锁，没有人获取了独占锁或者等待独占锁  
                    newStatus = (lastStatus + 16) | 1;  
  
                // try to get lock  
                if ( lastStatus == _InterlockedCompareExchange((volatile LONG *)pSRWLock, newStatus, lastStatus))  
                    return;// get lock successfully  
  
                // status of the lock was changed by other thread  
                // get lock failed  
                RtlBackoff(&nBackOff);  
                curStatus = (size_t)pSRWLock->Ptr;  
            }  
  
            lastStatus = curStatus;  
        }  
    }  
  
    return;  
}  
  
  
void NTAPI RtlReleaseSRWLockExclusive(RTL_SRWLOCK* pSRWLock)  
{  
    size_t newStatus;   
    size_t curStatus;   
    size_t lastWaiter;   
  
    lastWaiter = InterlockedExchangeAdd((volatile LONG *)pSRWLock, -1); // reset lock flag  
  
    if ( !(lastWaiter & Busy_Lock) ) // bit0 != 1  
    {  
        ASSERT("STATUS_RESOURCE_NOT_OWNED" && 0);  
    }  
  
    if ( lastWaiter & Wait_Lock &&      // some one is waiting (0010b)  
        !(lastWaiter & Release_Lock) )  // lock is not released, bit2==0(0100b)  
    {  
        newStatus = lastWaiter - 1; // reset lock flag  
        curStatus = InterlockedCompareExchange((volatile LONG *)pSRWLock, newStatus | Release_Lock, newStatus); // set released flag, set bit2 (0100b)  
  
        // lock is not changed by others, and now it is changed with my new value  
        if ( curStatus == newStatus )  
            RtlpWakeSRWLock(pSRWLock, (newStatus | Release_Lock));  
    }  
  
    return;  
}  
  
  
void NTAPI RtlReleaseSRWLockShared(RTL_SRWLOCK *pSRWLock)  
{  
    size_t newStatus;   
    size_t curStatus;   
    size_t lastStatus;   
    _SyncItem* pLastNode;  
  
    lastStatus = InterlockedCompareExchange((volatile LONG *)pSRWLock, 0, 17);  
  
    if ( lastStatus != 17 ) // not single share lock, release lock failed.  
    {  
        if ( !(lastStatus & Busy_Lock) )  
        {  
            ASSERT("STATUS_RESOURCE_NOT_OWNED" && 0);  
        }  
  
        while ( 1 )  
        {  
            if ( lastStatus & Wait_Lock )  
            {  
                if ( lastStatus & Mixed_Lock ) // 两种锁混合等待  
                {  
                    pLastNode = (_SyncItem*)(EXTRACT_ADDR(lastStatus));  
                    while (!pLastNode->notify)  
                    {  
                        pLastNode = pLastNode->back;  
                    }  
  
                    // 既然是在释放共享锁，说明一定有人获取了共享锁  
                    // 如果有人获取了共享锁，就一定没有人获取独到占锁  
                    // 只需要把共享次数减1  
                    // 取出notify节点的共享次数变量的地址, 原子减  
                    if ( InterlockedDecrement((volatile LONG *)&(pLastNode->notify->shareCount)) > 0 )  
                    {  
                        return;  
                    }  
                }  
  
                while ( 1 )  
                {  
                    newStatus = lastStatus & (~0x9); //0xFFFFFFF6;// reset bit0 and bit3 (0110b)  
                    if ( lastStatus & Release_Lock )// test bit2 is set  
                    {  
                        curStatus = InterlockedCompareExchange((volatile LONG *)pSRWLock, newStatus, lastStatus);// reset bit0 and bit3  
                        if ( curStatus == lastStatus )  
                            return ;  
                    }  
                    else  
                    {  
                        curStatus = InterlockedCompareExchange((volatile LONG *)pSRWLock, newStatus | Release_Lock, lastStatus);// set bit2(0100b)  
                        if ( curStatus == lastStatus )  
                            RtlpWakeSRWLock(pSRWLock, (newStatus | Release_Lock));// set bit2(0100b)  
							return;
                    }  
  
                    lastStatus = curStatus;  
                }  
  
                break;  
            }  
            else  
            {   // 只存在share lock  
                newStatus = (EXTRACT_ADDR(lastStatus)) <= 0x10 ?         // share lock count == 0  
                                                                0 :         // set to not locked  
                                                        lastStatus - 16;    // share lock count -1  
  
                curStatus = InterlockedCompareExchange((volatile LONG *)pSRWLock, newStatus, lastStatus);  
                if ( curStatus == lastStatus )  
                    break;  
  
                lastStatus = curStatus;  
            }  
        }  
    }  
  
    return;  
}  

void NTAPI RtlpWakeSRWLock(RTL_SRWLOCK *pSRWLock, size_t st)  
{  
    size_t status;   
    size_t curStatus;   
    _SyncItem* notify;   
    _SyncItem* tmp1;   
    _SyncItem* tmp2;   
  
    status = st;  
  
    while (1)  
    {  
        if(!(status & Busy_Lock))  
        {  
            // 找出需要notify的节点  
            for(tmp1 = (_SyncItem*)(EXTRACT_ADDR(status)),notify = tmp1->notify; !notify; notify = tmp1->notify)  
            {  
                tmp2 = tmp1;  
                tmp1 = tmp1->back;  
                tmp1->next = tmp2;  
            }  
            ((_SyncItem*)(EXTRACT_ADDR(status)))->notify = notify;  
  
      
            // notify->next!=0,说明v6不是唯一一个等待的  
            // test flag bit0, notify is waiting owner-lock  
            // 因此只通知notify  
            if ( notify->next && notify->flag & 1 )//flag  
            {  
                ((_SyncItem *)(EXTRACT_ADDR(status)))->notify = notify->next;//notify  
                notify->next = 0;//next  
                  
                _InterlockedAnd((volatile LONG *)pSRWLock, -5);  
  
                if ( !_interlockedbittestandreset((volatile LONG *)&(notify->flag), 1u) ) //flag  
                    NtReleaseKeyedEvent(GlobalKeyedEventHandle, (PVOID)notify, 0, 0);  
      
                return;  
            }  
      
            // notify是唯一一个等待者  
            // 或者notify是等待share锁  
            // 通知notify以及后续所有节点(如果有的话)  
            curStatus = _InterlockedCompareExchange((volatile LONG *)pSRWLock, 0, status);  
  
            if ( curStatus == status )  
            {// change status successfully.  
                tmp2 = notify;  
  
                do  
                {  
                    notify = notify->next;//通知之后，v6中的地址可能在其他线程释放已经无效，必须先保存next  
  
                    if ( !_interlockedbittestandreset((volatile LONG *)&(tmp2->flag), 1u) ) //flag  
                        NtReleaseKeyedEvent(GlobalKeyedEventHandle, (PVOID)tmp2, 0, 0);  
                      
                    tmp2 = notify;  
                }while ( notify );  
  
                return;  
            }  
      
            // status was changed by other thread earlier than me  
            // change status failed  
            ((_SyncItem*)(EXTRACT_ADDR(status)))->notify = notify; //notify  
      
            status = curStatus;  
        }  
        else  
        {  
            curStatus = _InterlockedCompareExchange((volatile LONG *)pSRWLock, status - 4, status);  
            if ( curStatus == status )  
                return;  
  
            status = curStatus;  
        }  
    }  
}  
  
  
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
  
void NTAPI RtlpOptimizeSRWLockList(RTL_SRWLOCK* pSRWLock, size_t st)  
{  
    size_t status;   
    _SyncItem* tmp1;   
    _SyncItem* tmp2;   
    size_t curStatus;   
  
    status = st;  
  
    while ( 1 )  
    {     
        if ( status & Busy_Lock )  
        {  
            tmp1 = (_SyncItem*)(EXTRACT_ADDR(status));  
            if ( tmp1 )  
            {  
                while ( !tmp1->notify )  
                {  
                    tmp2 = tmp1;  
                    tmp1 = (_SyncItem *)tmp1->back; // *v3 ->back pointer of list-entry  
                    tmp1->next = tmp2; // *v3+8 ->next pointer of list-entry  
                }  
  
                ((_SyncItem*)(EXTRACT_ADDR(status)))->notify = tmp1->notify;   
            }  
  
            curStatus = InterlockedCompareExchange((volatile LONG *)pSRWLock, status - 4, status); // v2-4, set v2 not released  
            if ( curStatus == status )  
                break;  
  
            status = curStatus;  
        }  
        else  
        {  
            RtlpWakeSRWLock(pSRWLock, status);  
            break;  
        }  
    }  
  
    return;  
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