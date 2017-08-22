/*
 * COPYRIGHT:  See COPYING in the top level directory
 * PROJECT:    ReactOS system libraries
 * FILE:       lib/kernel32/thread/i386/thread.S
 * PURPOSE:    Thread Start Thunks
 * PROGRAMMER: Alex Ionescu (alex@relsoft.net)
 */

#include <asm.inc>
#include <ks386.inc>

.code

EXTERN _BaseThreadStart@8:PROC
EXTERN _BaseProcessStart@4:PROC
EXTERN _RtlInitializeCriticalSectionAndSpinCount@8:PROC
EXTERN _RtlEnterCriticalSection@4:PROC
EXTERN _RtlLeaveCriticalSection@4:PROC
EXTERN _RtlTryEnterCriticalSection@4:PROC

PUBLIC InitializeSRWLock@8
InitializeSRWLock@8:
  		push	00000400h
  		push	dword ptr [esp+08h]
  		call	_RtlInitializeCriticalSectionAndSpinCount@8
  		retn	0004h

L77EA716B:
  		cmp	eax,00000000h
  		jnz	L77EA717F
  		cmp	dword ptr [eax+04h],0FFFFFFFFh ;FFFFFFFFh
  		jmp	L77EA7176
 L77EA7176:
  		push	dword ptr [esp+08h]
  		call	InitializeSRWLock@8
 L77EA717F:
  		retn
		
PUBLIC SUB_L77EA715D
SUB_L77EA715D:
  		mov	eax,[esp+08h]
  		test	eax,eax
  		jnz	L77EA716B
  		mov	eax,3221225477 ;C0000005h
  		retn

PUBLIC AcquireSRWLockExclusive@4
AcquireSRWLockExclusive@4:
    
  		call	SUB_L77EA715D
  		push	dword ptr [esp+04h]
  		call	_RtlEnterCriticalSection@4
  		retn	0004h


PUBLIC AcquireSRWLockShared@4
AcquireSRWLockShared@4:
    	call	SUB_L77EA715D
  		push	dword ptr [esp+04h]
  		call	_RtlEnterCriticalSection@4
  		retn	0004h

PUBLIC ReleaseSRWLockExclusive@4
ReleaseSRWLockExclusive@4: 
  		push	dword ptr [esp+04h]
  		call	_RtlLeaveCriticalSection@4
  		retn	0004h

PUBLIC ReleaseSRWLockShared@4
ReleaseSRWLockShared@4:
  		push	dword ptr [esp+04h]
  		call	_RtlLeaveCriticalSection@4
  		retn	0004h

PUBLIC TryAcquireSRWLockExclusive@4		
TryAcquireSRWLockExclusive@4:
        call	SUB_L77EA715D
  		push	dword ptr [esp+04h]
  		call	_RtlTryEnterCriticalSection@4
  		retn	0004h

PUBLIC TryAcquireSRWLockShared@4
TryAcquireSRWLockShared@4:
        call	SUB_L77EA715D
  		push	dword ptr [esp+04h]
  		call	_RtlTryEnterCriticalSection@4
  		retn	0004h
	
END

/* EOF */
