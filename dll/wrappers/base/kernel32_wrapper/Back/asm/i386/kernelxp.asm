BITS 32 
; MAKE_IMPORT NtPowerInformation=ntdll.NtPowerInformation
; MAKE_IMPORT NtAllocateVirtualMemory=ntdll.NtAllocateVirtualMemory
; MAKE_IMPORT RtlNtStatusToDosError=ntdll.RtlNtStatusToDosError
; MAKE_IMPORT NtQueryInformationThread=ntdll.NtQueryInformationThread
; MAKE_IMPORT NtCreateFile=ntdll.NtCreateFile
; MAKE_IMPORT alloca_probe=ntdll._alloca_probe
; MAKE_IMPORT allmul=ntdll._allmul
; MAKE_IMPORT RtlEnterCriticalSection=ntdll.RtlEnterCriticalSection
; MAKE_IMPORT RtlLeaveCriticalSection=ntdll.RtlLeaveCriticalSection
; MAKE_IMPORT RtlInitializeCriticalSectionAndSpinCount=ntdll.RtlInitializeCriticalSectionAndSpinCount
; MAKE_IMPORT NtFsControlFile=ntdll.NtFsControlFile
; MAKE_IMPORT RtlRaiseException=ntdll.RtlRaiseException
;------------------------------------------------------------------------------
; SRW function Series
;------------------------------------------------------------------------------
; MAKE_EXPORT AcquireSRWLockExclusive_XP=AcquireSRWLockExclusive
AcquireSRWLockExclusive_XP:
  		call	SUB_L77EA715D
  		push	dword [esp+04h]
  		call	_RtlEnterCriticalSection
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT AcquireSRWLockShared_XP=AcquireSRWLockShared
AcquireSRWLockShared_XP:
    	call	SUB_L77EA715D
  		push	dword [esp+04h]
  		call	_RtlEnterCriticalSection
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT InitializeSRWLock_XP=InitializeSRWLock
InitializeSRWLock_XP:
  		push	00000400h
  		push	dword [esp+08h]
  		call	_RtlInitializeCriticalSectionAndSpinCount
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT ReleaseSRWLockExclusive_XP=ReleaseSRWLockExclusive
ReleaseSRWLockExclusive_XP: 
  		push	dword [esp+04h]
  		call	_RtlLeaveCriticalSection
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT ReleaseSRWLockShared_XP=ReleaseSRWLockShared
ReleaseSRWLockShared_XP:
  		push	dword [esp+04h]
  		call	_RtlLeaveCriticalSection
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT TryAcquireSRWLockExclusive_XP=TryAcquireSRWLockExclusive
TryAcquireSRWLockExclusive_XP:
        call	SUB_L77EA715D
  		push	dword [esp+04h]
  		call	RtlTryEnterCriticalSection
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT TryAcquireSRWLockShared_XP=TryAcquireSRWLockShared
TryAcquireSRWLockShared_XP:
        call	SUB_L77EA715D
  		push	dword [esp+04h]
  		call	RtlTryEnterCriticalSection
  		retn	0004h
;------------------------------------------------------------------------------
 SUB_L77EA715D:
  		mov	eax,[esp+08h]
  		test	eax,eax
  		jnz	L77EA716B
  		mov	eax,3221225477 ;C0000005h
  		retn
;------------------------------------------------------------------------------
 L77EA716B:
  		cmp	dword [eax],00000000h
  		jnz	L77EA717F
  		cmp	dword [eax+04h],4294967295 ;FFFFFFFFh
  		jmp	L77EA7176
 L77EA7176:
  		push	dword [esp+08h]
  		call	InitializeSRWLock_XP
 L77EA717F:
  		retn
;------------------------------------------------------------------------------
RtlTryEnterCriticalSection:
  		mov	ecx,[esp+04h]
  		mov	eax,4294967295;FFFFFFFFh
  		mov	edx,00000000h
L7C901126:
  		lock
  		cmpxchg	[ecx+04h],edx
  		jnz	L7C901145
  		mov	eax,[00000024h]
  		mov	[ecx+0Ch],eax
  		mov	dword [ecx+08h],00000001h
  		mov	eax,00000001h
  		retn	0004h
;------------------------------------------------------------------------------
L7C901145:
  		mov	eax,[00000024h]
  		cmp	[ecx+0Ch],eax
  		jz 	L7C901157
  		xor	eax,eax
  		pause				; SSE2
  		retn	0004h
;------------------------------------------------------------------------------
L7C901157:
  		lock
  		inc	dword[ecx+04h]
  		inc	dword[ecx+08h]
  		mov	eax,00000001h
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT InterlockedCompareExchange64_XP=InterlockedCompareExchange64
InterlockedCompareExchange64_XP:
  		push	ebx
  		push	ebp
  		mov	ebp,[esp+0Ch]
  		mov	ebx,[esp+10h]
  		mov	ecx,[esp+14h]
  		mov	eax,[esp+18h]
  		mov	edx,[esp+1Ch]
  		lock
  		cmpxchg8b	[ebp+00h]
  		pop	ebp
  		pop	ebx
  		retn	0014h
;------------------------------------------------------------------------------
; MAKE_EXPORT InitializeConditionVariable_XP=InitializeConditionVariable
InitializeConditionVariable_XP:
  		mov	edi,edi
  		push	ebp
  		mov	ebp,esp
  		mov	eax,[ebp+08h]
  		and	dword [eax],00000000h
  		pop	ebp
  		retn	0004h
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
; MAKE_EXPORT GetApplicationRecoveryCallback_XP=GetApplicationRecoveryCallback
GetApplicationRecoveryCallback_XP:
          xor    eax,eax
          retn    0014h
        
;------------------------------------------------------------------------------        
; MAKE_EXPORT GetApplicationRestartSettings_XP=GetApplicationRestartSettings
GetApplicationRestartSettings_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetProductInfo_XP=GetProductInfo
GetProductInfo_XP:
          xor    eax,eax
          retn    0014h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetProcessPreferredUILanguages_XP=GetProcessPreferredUILanguages
GetProcessPreferredUILanguages_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetSystemPreferredUILanguages_XP=GetSystemPreferredUILanguages
GetSystemPreferredUILanguages_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetThreadPreferredUILanguages_XP=GetThreadPreferredUILanguages:
GetThreadPreferredUILanguages_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetUserPreferredUILanguages_XP=GetUserPreferredUILanguages
GetUserPreferredUILanguages_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT SetProcessPreferredUILanguages_XP=SetProcessPreferredUILanguages:
SetProcessPreferredUILanguages_XP:
          xor    eax,eax
          retn    000Ch
        
;------------------------------------------------------------------------------
; MAKE_EXPORT SetThreadPreferredUILanguages_XP=SetThreadPreferredUILanguages
SetThreadPreferredUILanguages_XP:
          xor    eax,eax
          retn    000Ch
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetThreadUILanguage_XP=GetThreadUILanguage
GetThreadUILanguage_XP:
          xor    eax,eax
          retn
        
;------------------------------------------------------------------------------
; MAKE_EXPORT QueryFullProcessImageNameA_XP=QueryFullProcessImageNameA
QueryFullProcessImageNameA_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT QueryFullProcessImageNameW_XP=QueryFullProcessImageNameW
QueryFullProcessImageNameW_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT CreateSemaphoreExW_XP=CreateSemaphoreExW
CreateSemaphoreExW_XP:
          push    dword [esp+10h]
          push    dword [esp+10h]
          push    dword [esp+10h]
          push    dword [esp+10h]
          call    CreateSemaphoreW
          retn    0018h
        
;-------------------------------------------------------------------------------
; MAKE_EXPORT CreateSemaphoreExA_XP=CreateSemaphoreExA
CreateSemaphoreExA_XP:
          push    dword [esp+10h]
          push    dword [esp+10h]
          push    dword [esp+10h]
          push    dword [esp+10h]
          call    CreateSemaphoreA
          retn    0018h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT CreateEventExA_XP=CreateEventExA
CreateEventExA_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT CreateEventExW_XP=CreateEventExW
CreateEventExW_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT CreateSymbolicLinkW_XP=CreateSymbolicLinkW
CreateSymbolicLinkW_XP:
          xor    al,al
          retn    000Ch
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetFileInformationByHandleEx_XP=GetFileInformationByHandleEx
GetFileInformationByHandleEx_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetTimeZoneInformationForYear_XP=GetTimeZoneInformationForYear
GetTimeZoneInformationForYear_XP:
          xor    eax,eax
          retn    000Ch
        
;------------------------------------------------------------------------------
; MAKE_EXPORT CompareStringEx_XP=CompareStringEx
CompareStringEx_XP:
          xor    eax,eax
          retn    0024h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetFinalPathNameByHandleA_XP=GetFinalPathNameByHandleA
GetFinalPathNameByHandleA_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetFinalPathNameByHandleW_XP=GetFinalPathNameByHandleW
GetFinalPathNameByHandleW_XP:
          xor    eax,eax
          retn    0010h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT CancelSynchronousIo_XP=CancelSynchronousIo
CancelSynchronousIo_XP:
          push    dword 000003E3h
          pop    eax
          retn    0008h
RtlSetLastWin32Error:
  		push	ebp
  		mov	ebp,esp
  		mov	eax,[00000018h]
  		mov	ecx,[ebp+08h]
  		mov	[eax+34h],ecx
  		pop	ebp
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT SetThreadStackGuarantee_XP=SetThreadStackGuarantee
SetThreadStackGuarantee_XP:
          push    ebp
          mov    ebp,esp
          sub    esp,00000030h
          push    ebx
          push    esi
          push    edi
          lea    eax,[ebp-30h]
          push    eax
          call    GetSystemInfo
          mov    eax,[ebp+08h]
          mov    edi,[eax]
          mov    ecx,[00000018h]
          mov    esi,[ebp-2Ch]
          mov    [ebp-04h],edi
          mov    ebx,[ecx+00000F78h]
          push    00000004h
          mov    [eax],ebx
          pop    eax
          mov    [ebp-0Ch],esi
          call    jmp_alloca_probe
          test    edi,edi
          mov    edx,esp
          jz     L7C5C4CB9
          cmp    edi,ebx
          jc     L7C5C4CB9
          lea    ecx,[esi+esi]
          lea    ebx,[esi-01h]
          lea    eax,[ecx+edi-01h]
          not    ebx
          and    eax,ebx
          cmp    eax,edi
          jnc    L7C5C4C4B
L7C5C4C42:
          push    00000057h
          call    RtlSetLastWin32Error
          jmp    L7C5C4CA2
L7C5C4C4B:
          cmp    eax,ecx
          mov    edi,[00000018h]
          mov    [ebp-04h],eax
          mov    edi,[edi+00000E0Ch]
          jnc    L7C5C4C62
          mov    [ebp-04h],ecx
L7C5C4C62:
          lea    eax,[esi-01h]
          not    eax
          and    eax,edx
          sub    eax,[ebp-04h]
          add    edi,esi
          cmp    eax,edi
          mov    [ebp-08h],eax
          jc     L7C5C4C42
          push    00000104h
          push    00001000h
          lea    eax,[ebp-04h]
          push    eax
          push    00000000h
          lea    eax,[ebp-08h]
          push    eax
          push    4294967295;FFFFFFFFh
          call    NtAllocateVirtualMemory
          test    eax,eax
          jge    L7C5C4CA6
          cmp    eax,3221225505 ;C0000021h
          jz     L7C5C4CA6
          push    eax
          call    SUB_L77E5826D
L7C5C4CA2:
          xor    eax,eax
          jmp    L7C5C4CBC
L7C5C4CA6:
          mov    eax,[ebp-04h]
          sub    eax,[ebp-0Ch]
          mov    ecx,[00000018h]
          mov    [ecx+00000F78h],eax
L7C5C4CB9:
          xor    eax,eax
          inc    eax
L7C5C4CBC:
          lea    esp,[ebp-3Ch]
          pop    edi
          pop    esi
          pop    ebx
          leave
          retn    0004h
jmp_alloca_probe:
          jmp    alloca_probe
;-----------------------------------------------------------------------------
; MAKE_EXPORT FlsAlloc_XP=FlsAlloc
FlsAlloc_XP:
  		mov	edi,edi
  		push	ebp
  		mov	ebp,esp
  		call	TlsAlloc
  		pop	ebp
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT FlsFree_XP=FlsFree
FlsFree_XP:
  		mov	edi,edi
  		push	ebp
  		mov	ebp,esp
  		mov	eax,[ebp+08h]
  		push	eax
  		call	TlsFree
  		pop	ebp
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT FlsGetValue_XP=FlsGetValue
FlsGetValue_XP:
  		mov	edi,edi
  		push	ebp
  		mov	ebp,esp
  		mov	eax,[ebp+08h]
  		push	eax
  		call	TlsGetValue
  		pop	ebp
  		retn	0004h
;------------------------------------------------------------------------------
; MAKE_EXPORT FlsSetValue_XP=FlsSetValue
FlsSetValue_XP:
  		mov	edi,edi
  		push	ebp
  		mov	ebp,esp
  		mov	eax,[ebp+0Ch]
  		push	eax
  		mov	ecx,[ebp+08h]
  		push	ecx
  		call	TlsSetValue
  		pop	ebp
  		retn	0008h
;------------------------------------------------------------------------------
; MAKE_EXPORT InitializeCriticalSectionEx_XP=InitializeCriticalSectionEx
InitializeCriticalSectionEx_XP:
  		push	dword [esp+08h]
  		push	dword [esp+08h]
  		call	InitializeCriticalSectionAndSpinCount
  		retn	000Ch        
;------------------------------------------------------------------------------
;Functions traced by Priothn 
;------------------------------------------------------------------------------
SUB_L77E5826D:        ; win10    ->SUB_L100E7E10
          mov    edi,edi
          push    esi
          push    ecx
          call    _RtlNtStatusToDosError
          mov    esi,eax
          push    esi
          call    RtlSetLastWin32Error
          mov    eax,esi
          pop    esi
          retn  
        
; MAKE_EXPORT GetThreadId_new=GetThreadId
GetThreadId_new:    ;Win10    CUx86 v15063
          mov    edi,edi
          push    ebp
          mov    ebp,esp
          sub    esp,0000001Ch
          lea    eax,[ebp-1Ch]
          push    00000000h
          push    0000001Ch
          push    eax
          push    00000000h
          push    dword [ebp+08h]
          call    _NtQueryInformationThread
          test    eax,eax
          jns    L10172EBA
          mov    ecx,eax
          call    SUB_L77E5826D
          xor    eax,eax
          jmp    L10172EBD
L10172EBA:
          mov    eax,[ebp-10h]
L10172EBD:
          mov    esp,ebp
          pop    ebp
          retn    0004h
        
; MAKE_EXPORT GetProcessIdOfThread_new=GetProcessIdOfThread
GetProcessIdOfThread_new:    ;Win10    CUx86 v15063
          mov    edi,edi
          push    ebp
          mov    ebp,esp
          sub    esp,0000001Ch
          lea    eax,[ebp-1Ch]
          push     00000000h
          push     0000001Ch
          push    eax
          push     00000000h
          push    dword [ebp+08h]
          call    _NtQueryInformationThread
          test    eax,eax
          js     L1011EA7A
          mov    eax,[ebp-14h]
L100D5836:
          mov    esp,ebp
          pop    ebp
          retn    0004h
L1011EA7A:
          mov    ecx,eax
          call    SUB_L77E5826D
          xor    eax,eax
          jmp    L100D5836       
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
; Init function series 
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
; MAKE_EXPORT InitOnceExecuteOnce_new=InitOnceExecuteOnce
InitOnceExecuteOnce_new:        
          push    ebp
          mov    ebp,esp
          push    dword [ebp+14h]
          push    dword [ebp+10h]
          push    dword [ebp+0Ch]
          push    dword [ebp+08h]
          call    RtlRunOnceExecuteOnce_new
          xor    ecx,ecx
          test    eax,eax
          setge    cl
          mov    eax,ecx
          pop    ebp
          retn    0010h
WinSqmAddToStream_new:
  		xor	eax,eax
  		retn	0010h
RtlRunOnceBeginInitialize_new:
  		push	ebp
  		mov	ebp,esp
  		mov	eax,[ebp+0Ch]
  		test	eax,4294967292 ;FFFFFFFCh
  		push	ebx
  		push	esi
  		push	edi
  		jnz	L77FD209C
  		lea	ecx,[eax-01h]
  		test	ecx,eax
  		jnz	L77FD209C
  		mov	edi,[ebp+08h]
  		mov	ecx,[edi]
  		and	dword [ebp+0Ch],00000000h
  		mov	edx,ecx
  		and	dl,03h
  		cmp	dl,02h
  		jnz	L77FD2061
  		xchg	eax,[ebp+08h]
L77FD2049:
  		mov	eax,[ebp+10h]
  		test	eax,eax
  		jnz	L77FD205A
L77FD2050:
  		mov	eax,[ebp+0Ch]
L77FD2053:
  		pop	edi
  		pop	esi
  		pop	ebx
  		pop	ebp
  		retn	000Ch
L77FD205A:
  		and	ecx,4294967292 ;FFFFFFFCh
  		mov	[eax],ecx
  		jmp	L77FD2050
L77FD2061:
  		test	al,01h
  		jnz	L77FD20A3
  		shr	eax,1
  		not	eax
  		and	eax,00000001h
  		mov	esi,eax
L77FD206E:
  		mov	eax,ecx
  		and	eax,00000003h
  		jnz	L77FD20AA
  		test	esi,esi
  		setz 	al
  		mov	edx,ecx
  		mov	ebx,edi
  		lea	eax,[eax+eax+01h]
  		and	eax,00000003h
  		mov	ecx,eax
  		mov	eax,edx
  		lock
  		cmpxchg	[ebx],ecx
  		mov	ecx,eax
  		cmp	ecx,edx
  		jnz	L77FD206E
  		mov	dword [ebp+0Ch],00000103h
  		jmp	L77FD2050
L77FD209C:
  		mov	eax,3221225712 ;C00000F0h
  		jmp	L77FD2053
L77FD20A3:
  		mov	eax,3221225473 ;C0000001h
  		jmp	L77FD2053
L77FD20AA:
  		cmp	eax,00000001h
  		jnz	L77FD20C7
  		test	esi,esi
  		jz 	L77FD20BE
  		push	edi
  		push	ecx
  		call	SUB_L77FD20E8
  		mov	ecx,eax
  		jmp	L77FD206E
L77FD20BE:
  		mov	dword [ebp+0Ch],3221225712 ;C00000F0h
  		jmp	L77FD2050
L77FD20C7:
  		cmp	ecx,00000003h
  		jnz	L77FD2049
  		neg	esi
  		sbb	esi,esi
  		and	esi,3221225453 ;BFFFFFEDh
  		add	esi,00000103h
  		mov	[ebp+0Ch],esi
  		jmp	L77FD2050
SUB_L77FD20E8:
  		mov	edi,edi
  		push	ebp
  		mov	ebp,esp
  		mov	ecx,[ebp+08h]
  		push	ebx
  		push	esi
  		lea	esi,[ebp+08h]
  		and	esi,4294967293 ;FFFFFFFDh
  		push	edi
  		mov	edi,[ebp+0Ch]
  		or	esi,00000001h
L77FD20FF:
  		mov	eax,ecx
  		and	eax,4294967292 ;FFFFFFFCh
  		mov	[ebp+08h],eax
  		mov	edx,esi
  		mov	ebx,edi
  		mov	eax,ecx
  		lock
  		cmpxchg	[ebx],edx
  		cmp	eax,ecx
  		jnz	L77FD2125
  		xor	eax,eax
  		push	eax
  		push	eax
  		lea	ecx,[ebp+08h]
  		push	ecx
  		push	eax
  		call	WinSqmAddToStream_new
  		mov	eax,[edi]
L77FD2125:
  		mov	edx,eax
  		and	dl,03h
  		cmp	dl,01h
  		mov	ecx,eax
  		jz 	L77FD20FF
  		pop	edi
  		pop	esi
  		pop	ebx
  		pop	ebp
  		retn	0008h
RtlRunOnceComplete_new:
  		push	ebp
  		mov	ebp,esp
  		mov	eax,[ebp+0Ch]
  		test	eax,4294967289 ;FFFFFFF9h
  		push	ebx
  		jnz	L77FD21E4
  		lea	ecx,[eax-01h]
  		test	ecx,eax
  		jnz	L77FD21E4
  		mov	ebx,[ebp+0Ch]
  		mov	ecx,[ebp+10h]
  		shr	eax,1
  		not	eax
  		xor	eax,ebx
  		and	eax,00000003h
  		xor	ebx,eax
  		test	ecx,ecx
  		jnz	L77FD21A6
 L77FD216A:
  		mov	edx,[ebp+08h]
  		mov	eax,[edx]
  		mov	[ebp+0Ch],eax
  		and	ecx,4294967292 ;FFFFFFFCh
  		mov	eax,ebx
  		and	eax,00000002h
  		or	eax,ecx
  		mov	ecx,[ebp+0Ch]
  		and	ecx,00000003h
  		cmp	ecx,00000001h
  		jnz	L77FD21BE
  		test	bl,cl
  		jz 	L77FD21E4
  		xchg	eax,[edx]
  		mov	[ebp+0Ch],eax
  		and	al,03h
  		cmp	al,cl
  		jnz	L77FD21B7
  		lea	eax,[ebp+0Ch]
  		push	eax
  		call	SUB_L77FD21EB
L77FD219F:
  		xor	eax,eax
L77FD21A1:
  		pop	ebx
  		pop	ebp
  		retn	000Ch
L77FD21A6:
  		test	bl,02h
  		jz 	L77FD21B0
  		test	cl,03h
  		jz 	L77FD216A
L77FD21B0:
  		mov	eax,3221225713 ;C00000F1h
  		jmp	L77FD21A1
L77FD21B7:
  		mov	eax,3221225562 ;C000005Ah
  		jmp	L77FD21A1
L77FD21BE:
  		cmp	ecx,00000003h
  		jnz	L77FD21DD
  		test	bl,01h
  		jnz	L77FD21E4
  		mov	ecx,eax
  		mov	eax,[ebp+0Ch]
  		lock
  		cmpxchg	[edx],ecx
  		cmp	eax,[ebp+0Ch]
  		jz 	L77FD219F
  		mov	eax,3221225525 ;C0000035h
  		jmp	L77FD21A1
L77FD21DD:
  		mov	eax,3221225473 ;C0000001h
  		jmp	L77FD21A1
L77FD21E4:
  		mov	eax,3221225712 ;C00000F0h
  		jmp	L77FD21A1
SUB_L77FD21EB:
  		mov	edi,edi
  		push	ebp
  		mov	ebp,esp
  		mov	eax,[ebp+08h]
  		mov	eax,[eax]
  		and	eax,4294967292 ;FFFFFFFCh
  		jnz	L77FD21FE
L77FD21FA:
  		pop	ebp
  		retn	0004h
L77FD21FE:
  		push	esi
L77FD21FF:
  		mov	esi,[eax]
  		push	00000000h
  		push	00000000h
  		push	eax
  		push	00000000h
  		call	WinSqmAddToStream_new
  		test	esi,esi
  		mov	eax,esi
  		jnz	L77FD21FF
  		pop	esi
  		jmp	L77FD21FA
RtlRunOnceExecuteOnce_new:
  		push	ebp
  		mov	ebp,esp
  		push	esi
  		push	edi
  		mov	edi,[ebp+14h]
  		push	edi
  		push	00000000h
  		push	dword [ebp+08h]
  		call	RtlRunOnceBeginInitialize_new
  		mov	esi,eax
  		test	esi,esi
  		jl 	L77FD2266
  		cmp	esi,00000103h
  		jz 	L77FD223F
L77FD2237:
  		pop	edi
  		mov	eax,esi
  		pop	esi
  		pop	ebp
  		retn	0010h
;------------------------------------------------------------------------------
L77FD223F:
  		push	edi
  		push	dword [ebp+10h]
  		push	dword [ebp+08h]
  		call	dword [ebp+0Ch]
  		test	eax,eax
  		jz 	L77FD2276
L77FD2251:
  		push	edi
  		push	00000000h
  		push	dword [ebp+08h]
  		call	RtlRunOnceComplete_new
  		mov	esi,eax
  		test	esi,esi
  		jl 	L77FD2270
  		xor	esi,esi
  		jmp	L77FD2237
L77FD2266:
  		mov	byte [ebp+17h],00h
  		jmp	L77FD2291
L77FD226C:
  		mov	edi,[edi]
  		jmp	L77FD2251
L77FD2270:
  		mov	byte [ebp+17h],01h
  		jmp	L77FD2291
L77FD2276:
  		push	00000000h
  		push	00000004h
  		push	dword [ebp+08h]
  		mov	esi,3221225473 ;C0000001h
  		call	RtlRunOnceComplete_new
  		test	eax,eax
  		jge	L77FD2237
  		mov	esi,eax
  		mov	byte [ebp+17h],02h
L77FD2291:
  		lea	eax,[ebp+17h]
  		push	eax
  		push	esi
  		call	SUB_L77FD229D
  		jmp	L77FD2237
SUB_L77FD229D:
  		push	dword [esp+04h]
  		call	_RtlRaiseException
  		retn	0008h        
; MAKE_EXPORT FindNextStreamW_new=FindNextStreamW
FindNextStreamW_new:        ;blackwingcat KB935839 2017.04
          push    ebp
          mov    ebp,esp
          mov    ecx,[ebp+08h]
          mov    edx,[ecx+04h]
          add    edx,[ecx+0Ch]
          mov    eax,[ecx+08h]
          cmp    eax,edx
          jnz    L77EA5881
          push    3221225489 ;C0000011h
          call    SUB_L77E5826D
          xor    eax,eax
          jmp    L77EA58D4
L77EA5881:
          mov    ecx,[eax+08h]
          mov    edx,[ebp+0Ch]
          mov    [edx],ecx
          mov    ecx,[eax+0Ch]
          mov    [edx+04h],ecx
          mov    ecx,[eax+04h]
          push    ebx
          push    esi
          mov    ebx,ecx
          push    edi
          shr    ecx,02h
          lea    esi,[eax+18h]
          lea    edi,[edx+08h]
          rep movsd
          mov    ecx,ebx
          and    ecx,00000003h
          rep movsb
          mov    ecx,[eax+04h]
          shr    ecx,1
          and    word [edx+ecx*2+08h],0000h
          mov    eax,[eax]
          test    eax,eax
          pop    edi
          pop    esi
          pop    ebx
          jbe    L77EA58C5
          mov    ecx,[ebp+08h]
          add    [ecx+08h],eax
          jmp    L77EA58D1
L77EA58C5:
          mov    eax,[ebp+08h]
          mov    ecx,[eax+04h]
          add    ecx,[eax+0Ch]
          mov    [eax+08h],ecx
L77EA58D1:
          xor    eax,eax
          inc    eax
L77EA58D4:
          pop    ebp
          retn    0008h
        
; MAKE_EXPORT OpenFileById_new=OpenFileById        
OpenFileById_new:        ;blackwingcat KB935839 2017.04
          push    ebp
          mov    ebp,esp
          mov    eax,[ebp+0Ch]
          sub    esp,00000028h
          push    ebx
          xor    ebx,ebx
          cmp    eax,ebx
          jz     L77EA7127
          push    00000018h
          pop    edx
          cmp    [eax],edx
          jc     L77EA7127
          mov    ecx,[eax+04h]
          cmp    ecx,ebx
          jnz    L77EA7036
          mov    word [ebp-08h],0008h
          mov    word [ebp-06h],0008h
          jmp    L77EA704B
L77EA7036:
          cmp    ecx,00000001h
          jnz    L77EA7127
          mov    word [ebp-08h],0010h
          mov    word [ebp-06h],0010h
L77EA704B:
          mov    ecx,[ebp+1Ch]
          add    eax,00000008h
          mov    [ebp-04h],eax
          mov    eax,[ebp+08h]
          push    esi
          mov    [ebp-24h],eax
          mov    [ebp-28h],edx
          lea    eax,[ebp-08h]
          mov    [ebp-20h],eax
          mov    esi,ecx
          shr    esi,04h
          mov    eax,08000000h
          and    esi,eax
          mov    edx,ecx
          and    edx,20000000h
          or    esi,edx
          shr    esi,1
          mov    edx,ecx
          and    edx,eax
          or    esi,edx
          shr    esi,08h
          mov    eax,ecx
          and    eax,10000000h
          or    esi,eax
          shr    esi,06h
          mov    eax,ecx
          and    eax,02000000h
          or    esi,eax
          mov    eax,ecx
          shr    eax,19h
          not    eax
          shr    esi,0Bh
          and    eax,00000020h
          or    esi,eax
          test    ecx,04000000h
          push    edi
          mov    edi,[ebp+10h]
          mov    dword [ebp-1Ch],00000040h
          mov    [ebp-18h],ebx
          mov    [ebp-14h],ebx
          jz     L77EA70CE
          or    esi,00001000h
          or    edi,00010000h
L77EA70CE:
          mov    eax,00200000h
          test    ecx,eax
          jz     L77EA70D9
          or    esi,eax
L77EA70D9:
          test    ecx,00100000h
          jz     L77EA70E7
          or    esi,00400000h
L77EA70E7:
          push    ebx
          push    ebx
          or    esi,00002000h
          push    esi
          push    00000003h
          push    dword [ebp+14h]
          lea    ecx,[ebp-10h]
          push    ebx
          push    ebx
          push    ecx
          lea    ecx,[ebp-28h]
          push    ecx
          or    edi,00100080h
          push    edi
          lea    ecx,[ebp+10h]
          push    ecx
          call    _NtCreateFile
          call    eax
          cmp    eax,ebx
          pop    edi
          pop    esi
          jge    L77EA7122
          or    dword [ebp+10h],4294967295 ;FFFFFFFFh
          push    eax
          call    SUB_L77E5826D
L77EA7122:
          mov    eax,[ebp+10h]
          jmp    L77EA7131
L77EA7127:
          push    00000057h
          call    RtlSetLastWin32Error
          or    eax,4294967295 ;FFFFFFFFh
L77EA7131:
          pop    ebx
          leave
          retn    0018h
        
;-------------------------------------------------------------------------------------
; MAKE_EXPORT QueryThreadCycleTime_XP=QueryThreadCycleTime
QueryThreadCycleTime_XP:
          mov    edi,edi
          push    ebp
          mov    ebp,esp
          sub    esp,00000010h
          push    00000000h
          push    00000010h
          lea    eax,[ebp-10h]
          push    eax
          push    00000017h
          push    dword [ebp+08h]
          call    _NtQueryInformationThread
          test    eax,eax
          jl     L77E775BD
          mov    eax,[ebp+0Ch]
          mov    ecx,[ebp-10h]
          mov    [eax],ecx
          mov    ecx,[ebp-0Ch]
          mov    [eax+04h],ecx
          xor    eax,eax
          inc    eax
          jmp    L77E775C5
L77E775BD:
          push    eax
          call    SUB_L77E5826D
          xor    eax,eax
L77E775C5:
          leave
          retn    0008h
        
;-----------------------------------------------------------------------------------------
; Power Functions Implimentation 
;-----------------------------------------------------------------------------------------
; MAKE_EXPORT PowerClearRequest_XP=PowerClearRequest
PowerClearRequest_XP:
          mov    edi,edi
          push    ebp
          mov    ebp,esp
          sub    esp,0000000Ch
          mov    eax,[ebp+08h]
          push    00000000h
          mov    [ebp-0Ch],eax
          mov    eax,[ebp+0Ch]
          push    00000000h
          mov    [ebp-08h],eax
          push    0000000Ch
          lea    eax,[ebp-0Ch]
          push    eax
          push    0000002Ch
          mov    dword [ebp-04h],00h
          call    _NtPowerInformation
          test    eax,eax
          jl     L77E50176
          xor    eax,eax
          inc    eax
L77E13BCA:
          leave
          retn    0008h
L77E50176:
          push    eax
          call    SUB_L77E5826D
          xor    eax,eax
          jmp    L77E13BCA
        
;----------------------------------------------------------------------------------------
; MAKE_EXPORT PowerCreateRequest_XP=PowerCreateRequest
; MAKE_IMPORT RtlFreeHeap=ntdll.RtlFreeHeap
PowerCreateRequest_XP:
          mov    edi,edi
          push    ebp
          mov    ebp,esp
          sub    esp,00000010h
          mov    dword [ebp-10h],00000000h
          mov    dword [ebp-04h],4294967295;FFFFFFFFh
          push    00000004h
          lea    eax,[ebp-04h]
          push    eax
          push    0000001Ch
          mov    ecx,[ebp-10h]
          push    ecx
          push    0000002Bh
          call    _NtPowerInformation
          mov    [ebp-0Ch],eax
          cmp    dword  [ebp-0Ch],00000000h
          jge    L7C70499B
          mov    edx,[ebp-0Ch]
          push    edx
          call    SUB_L77E5826D
L7C70499B:
          cmp    dword  [ebp-10h],00000000h
          jz     L7C7049B8
          call    GetProcessHeap
          mov    [ebp-08h],eax
          mov    eax,[ebp-10h]
          push    eax
          push    00000000h
          mov    ecx,[ebp-08h]
          push    ecx
          call    _RtlFreeHeap
L7C7049B8:
          mov    eax,[ebp-04h]
          mov    esp,ebp
          pop    ebp
          retn    0004h
        
;----------------------------------------------------------------------------------------
; MAKE_EXPORT PowerSetRequest_XP=PowerSetRequest
PowerSetRequest_XP:
          mov    edi,edi
          push    ebp
          mov    ebp,esp
          sub    esp,0000000Ch
          mov    eax,[ebp+08h]
          push    00000000h
          mov    [ebp-0Ch],eax
          mov    eax,[ebp+0Ch]
          push    00000000h
          mov    [ebp-08h],eax
          push    0000000Ch
          lea    eax,[ebp-0Ch]
          push    eax
          push    0000002Ch
          mov    byte  [ebp-04h],01h
          call    _NtPowerInformation
          test    eax,eax
          jl     L77E50183
          xor    eax,eax
          inc    eax
L77E13B8C:
          leave
          retn    0008h
L77E50183:
          push    eax
          call    SUB_L77E5826D
          xor    eax,eax
          jmp    L77E13B8C
        
;-------------------------------------------------------------------------------------
; MAKE_EXPORT QueryUnbiasedInterruptTime_XP=QueryUnbiasedInterruptTime
; Copyright BlackWingCat
; http://blog.livedoor.jp/blackwingcat/archives/1944476.html
QueryUnbiasedInterruptTime_XP:
          push        ebp
        mov         ebp,esp
        sub esp,0x14
        lea eax,[ebp-0xc]
        push eax
        call QueryPerformanceFrequency
        lea eax,[ebp-0x14]
        push eax
        call QueryPerformanceCounter
        wait
        fnstcw  [ebp-2]
        wait
        fild  dword [ebp-0x14]
        and   dword [ebp-0x14],0
        mov   dword [ebp-0x14+4],0x416312d0
        fmul  dword [ebp-0x14]
        fild  dword [ebp-0xc]
;        fdivp st qword (1),st
        mov ecx,[ebp+8]
        mov         ax,word  [ebp-2]
        or          ah,0Ch
        mov         word  [ebp-4],ax
        fldcw       word  [ebp-4]
        fistp       qword [ecx]
        fldcw       word  [ebp-2]
        leave
        retn 4
        
;--------------------------------------------------------------------------------------------
; MAKE_EXPORT GetTickCount64_XP=GetTickCount64
GetTickCount64_XP:
  		push	edi
  		mov	edi,7FFE0300h
loc_77EA5806:
  		mov	eax,[edi+08h]
  		mov	ecx,[edi+04h]
  		mov	edx,[edi]
  		test	eax,eax
  		jnz	L7C88519D
  		call	GetTickCount
  		xor	edx,edx
  		pop	edi
  		retn
;------------------------------------------------------------------------------
L7C88519D:
  		cmp	ecx,eax
  		jnz	L7C8851D2
  		mov	di,0004h
  		mov	eax,[edi]
  		mul	edx
  		shrd	eax,edx,18h
  		push	esi
  		shr	edx,18h
  		mov	esi,eax
  		mov	eax,[edi]
  		mov	edi,edx
  		mul	ecx
  		push	00000000h
  		push	00000100h
  		push	edx
  		push	eax
  		call	_allmul
  		add	esi,eax
  		adc	edi,edx
  		mov	edx,edi
  		mov	eax,esi
  		pop	esi
  		pop	edi
  		retn
;------------------------------------------------------------------------------
L7C8851D2:
        pause
		jmp     short loc_77EA5806
;------------------------------------------------------------------------------
; K32 Function Series
; Code from Windows 2000 KernelEx By BlackWingCat
;------------------------------------------------------------------------------

; MAKE_EXPORT K32GetProcessMemoryInfo_XP=K32GetProcessMemoryInfo
K32GetProcessMemoryInfo_XP:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA76D0_GetProcessMemoryInfo
L77EA7515:
          push    SSZ77E5220C_psapi_dll
          call    LoadLibraryA
          mov    esi,eax
          push    eax
          call    GetProcAddress
          push    dword [ebp+10h]
          push    dword [ebp+0Ch]
          push    dword [ebp+08h]
          call    eax
          push    esi
          call    FreeLibrary
          pop    esi
          leave
          retn    000Ch
        
;------------------------------------------------------------------------------
; MAKE_EXPORT K32EnumProcesses_XP=K32EnumProcesses
K32EnumProcesses_XP:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA76E6_EnumProcesses
          jmp    L77EA7515
        
;------------------------------------------------------------------------------
; MAKE_EXPORT K32EmptyWorkingSet_XP=K32EmptyWorkingSet
K32EmptyWorkingSet_XP:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA76F4_EmptyWorkingSet
L77EA7554:
          push    SSZ77E5220C_psapi_dll
          call    LoadLibraryA
          mov    esi,eax
          push    eax
          call    GetProcAddress
          push    dword [ebp+08h]
          call    eax
          push    esi
          call    FreeLibrary
          pop    esi
          leave
          retn    0004h
        
;---------------------------------------------------------------------------------
; MAKE_EXPORT K32EnumDeviceDrivers_XP=K32EnumDeviceDrivers
K32EnumDeviceDrivers_XP:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA7704_EnumDeviceDrivers
          jmp    L77EA7515
        
;----------------------------------------------------------------------------------
; MAKE_EXPORT K32EnumProcessModules_new=K32EnumProcessModules
K32EnumProcessModules_new:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA7716_EnumProcessModules
L77EA758D:
          push    SSZ77E5220C_psapi_dll
          call    LoadLibraryA
          mov    esi,eax
          push    eax
          call    GetProcAddress
          push    dword [ebp+14h]
          push    dword [ebp+10h]
          push    dword [ebp+0Ch]
          push    dword [ebp+08h]
          call    eax
          push    esi
          call    FreeLibrary
          pop    esi
          leave
          retn    0010h
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetDeviceDriverBaseNameA_xp=K32GetDeviceDriverBaseNameA
K32GetDeviceDriverBaseNameA_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA772A_GetDeviceDriverBaseNameA
          jmp    L77EA7515
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetDeviceDriverBaseNameW_xp=K32GetDeviceDriverBaseNameW
K32GetDeviceDriverBaseNameW_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA7744_GetDeviceDriverBaseNameW
          jmp    L77EA7515
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetDeviceDriverFileNameA_xp=K32GetDeviceDriverFileNameA
K32GetDeviceDriverFileNameA_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA775E_GetDeviceDriverFileNameA
          jmp    L77EA7515
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetDeviceDriverFileNameW_xp=K32GetDeviceDriverFileNameW
K32GetDeviceDriverFileNameW_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA7778_GetDeviceDriverFileNameW
          jmp    L77EA7515
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetMappedFileNameA_xp=K32GetMappedFileNameA
K32GetMappedFileNameA_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA7792_GetMappedFileNameA
          jmp    L77EA758D
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetMappedFileNameW_xp=K32GetMappedFileNameW
K32GetMappedFileNameW_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA77A6_GetMappedFileNameW
          jmp    L77EA758D
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetModuleBaseNameA_xp=K32GetModuleBaseNameA
K32GetModuleBaseNameA_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA77BA_GetModuleBaseNameA
          jmp    L77EA758D
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetModuleBaseNameW_xp=K32GetModuleBaseNameW
K32GetModuleBaseNameW_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA77CE_GetModuleBaseNameW
          jmp    L77EA758D
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetModuleFileNameExA_xp=K32GetModuleFileNameExA
K32GetModuleFileNameExA_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA77E2_GetModuleFileNameExA
          jmp    L77EA758D
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetModuleFileNameExW_xp=K32GetModuleFileNameExW
K32GetModuleFileNameExW_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA77F8_GetModuleFileNameExW
          jmp    L77EA758D
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetModuleInformation_xp=K32GetModuleInformation
K32GetModuleInformation_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA780E_GetModuleInformation
          jmp    L77EA758D
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetPerformanceInfo_xp=K32GetPerformanceInfo
K32GetPerformanceInfo_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA7824_GetPerformanceInfo
          push    SSZ77E5220C_psapi_dll
          call    LoadLibraryA
          mov    esi,eax
          push    eax
          call    GetProcAddress
          push    dword [ebp+0Ch]
          push    dword [ebp+08h]
          call    eax
          push    esi
          call    FreeLibrary
          pop    esi
          leave
          retn    0008h
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetProcessImageFileNameA_xp=K32GetProcessImageFileNameA
K32GetProcessImageFileNameA_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA7838_GetProcessImageFileNameA
          jmp    L77EA7515
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetProcessImageFileNameW_xp=K32GetProcessImageFileNameW
K32GetProcessImageFileNameW_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA7852_GetProcessImageFileNameW
          jmp    L77EA7515
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32GetWsChanges_xp=K32GetWsChanges
K32GetWsChanges_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA786C_GetWsChanges
          jmp    L77EA7515
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32InitializeProcessForWsWatch_xp=K32InitializeProcessForWsWatch
K32InitializeProcessForWsWatch_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA787A_InitializeProcessForWsWatch
          jmp    L77EA7554
        
;-----------------------------------------------------------------------------------
; MAKE_EXPORT K32QueryWorkingSet_xp=K32QueryWorkingSet
K32QueryWorkingSet_xp:
          push    ebp
          mov    ebp,esp
          push    esi
          push    SSZ77EA7896_QueryWorkingSet
          jmp    L77EA7515
        
;-----------------------------------------------------------------------------------
SSZ77E5220C_psapi_dll:
          db    'psapi.dll',0
SSZ77EA76D0_GetProcessMemoryInfo:
          db    'GetProcessMemoryInfo',0
SSZ77EA76E6_EnumProcesses:
          db    'EnumProcesses',0
SSZ77EA76F4_EmptyWorkingSet:
          db    'EmptyWorkingSet',0
SSZ77EA7704_EnumDeviceDrivers:
          db    'EnumDeviceDrivers',0
SSZ77EA7716_EnumProcessModules:
          db    'EnumProcessModules',0
SSZ77EA772A_GetDeviceDriverBaseNameA:
          db    'GetDeviceDriverBaseNameA',0
SSZ77EA7744_GetDeviceDriverBaseNameW:
          db    'GetDeviceDriverBaseNameW',0
SSZ77EA775E_GetDeviceDriverFileNameA:
          db    'GetDeviceDriverFileNameA',0
SSZ77EA7778_GetDeviceDriverFileNameW:
          db    'GetDeviceDriverFileNameW',0
SSZ77EA7792_GetMappedFileNameA:
          db    'GetMappedFileNameA',0
SSZ77EA77A6_GetMappedFileNameW:
          db    'GetMappedFileNameW',0
SSZ77EA77BA_GetModuleBaseNameA:
          db    'GetModuleBaseNameA',0
SSZ77EA77CE_GetModuleBaseNameW:
          db    'GetModuleBaseNameW',0
SSZ77EA77E2_GetModuleFileNameExA:
          db    'GetModuleFileNameExA',0
SSZ77EA77F8_GetModuleFileNameExW:
          db    'GetModuleFileNameExW',0
SSZ77EA780E_GetModuleInformation:
          db    'GetModuleInformation',0
SSZ77EA7824_GetPerformanceInfo:
          db    'GetPerformanceInfo',0 
SSZ77EA7838_GetProcessImageFileNameA:
          db    'GetProcessImageFileNameA',0
SSZ77EA7852_GetProcessImageFileNameW:
          db    'GetProcessImageFileNameW',0
SSZ77EA786C_GetWsChanges:
          db    'GetWsChanges',0
SSZ77EA787A_InitializeProcessForWsWatch:
          db    'InitializeProcessForWsWatch',0
SSZ77EA7896_QueryWorkingSet:
          db    'QueryWorkingSet',0
        
        
;-----------------------------------------------------------------------------------
;Condition Variable functions 
;Extracted from Windows 20000 Kernel by BlackWingCat
;-----------------------------------------------------------------------------------
; MAKE_EXPORT WakeAllConditionVariable_XP=WakeAllConditionVariable
WakeAllConditionVariable_XP:
          mov    eax,[esp+04h]
          push    ebx
          mov    ebx,_RtlEnterCriticalSection
          push    esi
          mov    esi,[eax]
          push    edi
          push    esi
          call    ebx
          lea    edi,[esi+18h]
          push    edi
          call    ebx
          xor    ebx,ebx
          cmp    [esi+30h],ebx
          jz     L77EA7ACF
          mov    dword [esi+3Ch],00000001h
          push    ebx
          push    dword [esi+30h]
          push    dword [esi+34h]
          nop
          call    ReleaseSemaphore
          push    edi
          call    RtlLeaveCriticalSection
          push    4294967295;FFFFFFFFh
          push    dword [esi+38h]
          nop
          call    WaitForSingleObject
          push    dword [esi+38h]
          nop
          call    ResetEvent
          mov    [esi+3Ch],ebx
          jmp    L77EA7AD6
L77EA7ACF:
          push    edi
          call    _RtlLeaveCriticalSection
L77EA7AD6:
          push    esi
          call    _RtlLeaveCriticalSection
          pop    edi
          pop    esi
          pop    ebx
          retn    0004h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT WakeConditionVariable_XP=WakeConditionVariable
WakeConditionVariable_XP:
          mov    eax,[esp+04h]
          push    ebx
          push    esi
          push    edi
          mov    esi,[eax]
          mov    edi,_RtlEnterCriticalSection
          push    esi
          call    edi
          lea    ebx,[esi+18h]
          push    ebx
          call    edi
          mov    edi,[esi+30h]
          push    ebx
          mov    ebx,_RtlLeaveCriticalSection
          call    ebx
          test    edi,edi
          jz     L77EA7B2C
          push    00000000h
          push    00000001h
          push    dword [esi+34h]
          nop
          call    ReleaseSemaphore
          push    4294967295;FFFFFFFFh
          push    dword [esi+38h]
          nop
          call    WaitForSingleObject
          push    dword [esi+38h]
          nop
          call    ResetEvent
L77EA7B2C:
          push    esi
          call    ebx
          pop    edi
          pop    esi
          pop    ebx
          retn    0004h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT SleepConditionVariableCS_XP=SleepConditionVariableCS
SleepConditionVariableCS_XP:
          push    ebp
          mov    ebp,esp
          mov    eax,[ebp+08h]
          push    ebx
          mov    ebx,_RtlEnterCriticalSection
          push    esi
          mov    esi,[eax]
          push    edi
          push    esi
          call    ebx
          lea    edi,[esi+18h]
          push    edi
          call    ebx
          mov    eax,[esi+30h]
          inc    eax
           mov    [esi+30h],eax
          push    edi
          mov    edi,_RtlLeaveCriticalSection
          call    edi
          push    esi
          call    edi
          push    dword [ebp+0Ch]
          call    edi
          push    dword [ebp+10h]
          push    dword [esi+34h]
          nop
          call    WaitForSingleObject
          test    eax,eax
          mov    [ebp+10h],eax
          jz     L77EA7B81
          push    eax
          nop
          call    RtlSetLastWin32Error
L77EA7B81:
          lea    eax,[esi+18h]
          push    eax
          call    ebx
          mov    eax,[esi+30h]
          dec    eax
          mov    [esi+30h],eax
          xor    eax,eax
          cmp    [esi+30h],eax
          jz     L77EA7B9D
          cmp    [esi+3Ch],eax
          mov    [ebp+08h],eax
          jnz    L77EA7BA4
L77EA7B9D:
          mov    dword  [ebp+08h],00000001h
L77EA7BA4:
          lea    eax,[esi+18h]
          push    eax
          call    edi
          cmp    dword  [ebp+08h],00000000h
          jz     L77EA7BB9
          push    dword [esi+38h]
          nop
          call    SetEvent
L77EA7BB9:
          push   dword [ebp+0Ch]
          call    ebx
          cmp    dword  [ebp+10h],00000000h
          pop    edi
          pop    esi
          pop    ebx
          setz     al
          movzx    eax,al
          pop    ebp
          retn    000Ch
        
;-------------------------------------------------------------------------------------------
; MAKE_EXPORT SleepConditionVariableSRW_XP=SleepConditionVariableSRW
SleepConditionVariableSRW_XP:
        push	dword [esp+0Ch]
  		push	dword [esp+0Ch]
  		push	dword [esp+0Ch]
  		call	SleepConditionVariableCS_XP
  		retn	0010h
        
;-------------------------------------------------------------------------------------------
; LOCAL LANGUAGE & LCID FUNCTION
;-------------------------------------------------------------------------------------------

;-------------------------------------------------------------------------------------------
; MAKE_EXPORT LCMapStringEx_XP=LCMapStringEx
LCMapStringEx_XP:
          push    ebp
          mov    ebp,esp
          call    SUB_L77EA4D0C
          push    dword [ebp+1Ch]
          push    dword [ebp+18h]
          push    dword [ebp+14h]
          push    dword [ebp+10h]
          push    dword [ebp+0Ch]
          push    ecx
          call    LCMapStringW
          leave
          retn    0024h
; MAKE_EXPORT LocaleNameToLCID_XP=LocaleNameToLCID
LocaleNameToLCID_XP:
          push    ebp
          mov    ebp,esp
          call    SUB_L77EA4D0C
          mov    eax,ecx
          leave
          retn    0008h
;------------------------------------------------------------------------------
; MAKE_EXPORT GetLocaleInfoEx_XP=GetLocaleInfoEx
GetLocaleInfoEx_XP:
          push    ebp
          mov    ebp,esp
          call    SUB_L77EA4D0C
          push    dword [ebp+14h]
          push    dword [ebp+10h]
          push    dword [ebp+0Ch]
          push    ecx
          call    GetLocaleInfoW
          leave
          retn    0010h

;------------------------------------------------------------------------------
; MAKE_EXPORT GetTimeFormatEx_XP=GetTimeFormatEx
GetTimeFormatEx_XP:
          push    ebp
          mov    ebp,esp
          call    SUB_L77EA4D0C
          xor    eax,eax
          push    dword [ebp+1Ch]
          push    dword [ebp+18h]
          push    dword [ebp+14h]
          push    dword [ebp+10h]
          push    dword [ebp+0Ch]
          push    ecx
          call    GetTimeFormatW
          leave
          retn    0018h
        
;------------------------------------------------------------------------------
; MAKE_EXPORT GetDateFormatEx_XP=GetDateFormatEx
GetDateFormatEx_XP:
          push    ebp
          mov    ebp,esp
          call    SUB_L77EA4D0C
          xor    eax,eax
          push    dword [ebp+20h]
          push    dword [ebp+1Ch]
          push    dword [ebp+18h]
          push    dword [ebp+14h]
          push    dword [ebp+10h]
          push    dword [ebp+0Ch]
          push    ecx
          call    GetDateFormatW
          leave
          retn    001Ch
;------------------------------------------------------------------------------
; MAKE_EXPORT IsValidLocaleName_XP=IsValidLocaleName
IsValidLocaleName_XP:
  		push	ebp
  		mov	ebp,esp
  		push	dword [ebp+08h]
  		call	SUB_L77EA4D0C
  		push	00000000h
  		pop	eax
  		setnz	al
  		leave
  		retn	0004h
;-------------------------------------------------------------------------------
SUB_L77EA4D0C:
          mov    ecx,[ebp+08h]
          test    ecx,ecx
          jnz    L77EA4D2C
L77EA4D13:
          retn
L77EA4D2C:
          mov    edx,[ecx+04h]
          mov    ecx,[ecx]
          shl    edx,08h
          or    ecx,edx
          mov    eax,ecx
          and    ecx,000000FFh
          test    ecx,ecx
          jz     L77EA4D13
          push    edx
          push    eax
          push    SWC77EA5154_LCMapStringEx_
          call    OutputDebugStringW
          push    dword [ebp+08h]
          call    OutputDebugStringW
          pop    eax
          pop    edx
          xor    ecx,ecx
          cmp    al,21h
          jz     L77EA4D13
          test    ah,ah
          jnz    L77EA4D6C
          and    eax,00FFFFFFh
          mov    ah,al
          shr    eax,08h
L77EA4D6C:
          cmp    ah,2Dh
          jnz    L77EA4D76
          mov    ah,al
          shr    eax,08h
L77EA4D76:
          and    eax,00DFDFDFh
          mov    ecx,00000411h
          cmp    eax,004A414Ah
          jz     L77EA4D13
          cmp    eax,0000414Ah
          jz     L77EA4D13
          cmp    eax,00504E4Ah
          jz     L77EA4D13
          mov    ecx,00000409h
          cmp    eax,00554E45h
          jz     L77EA4D13
          cmp    eax,004E5545h
          jz     L77EA4D13
          cmp    eax,00004E45h
          jz     L77EA4D13
          mov    ecx,00000404h
          cmp    eax,0054485Ah
          jz     L77EA4D13
          cmp    eax,00434854h
          jz     L77EA4D13
          mov    ecx,00000804h
          cmp    eax,0043485Ah
          jz     L77EA4D13
          cmp    eax,00485343h
          jz     L77EA4D13
          mov    ecx,00000410h
          cmp    eax,00495449h
          jz     L77EA4D13
          cmp    eax,00005449h
          jz     L77EA4D13
          cmp    eax,00544149h
          jz     L77EA4D13
          mov    ecx,0000040Ch
          cmp    eax,00465246h
          jz     L77EA4D13
          cmp    eax,00005246h
          jz     L77EA4D13
          cmp    eax,00524146h
          jz     L77EA4D13
          mov    ecx,00000407h
          cmp    eax,00444544h
          jz     L77EA4D13
          cmp    eax,00004544h
          jz     L77EA4D13
          cmp    eax,00455544h
          jz     L77EA4D13
          mov    ecx,0000040Eh
          cmp    eax,00485548h
          jz     L77EA4D13
          cmp    eax,00005548h
          jz     L77EA4D13
          cmp    eax,00554E48h
          jz     L77EA4D13
          mov    ecx,00000405h
          cmp    eax,00435343h
          jz     L77EA4D13
          cmp    eax,00005343h
          jz     L77EA4D13
          cmp    eax,00455343h
          jz     L77EA4D13
          mov    ecx,00000C0Ah
          cmp    eax,00455345h
          jz     L77EA4D13
          cmp    eax,00005345h
          jz     L77EA4D13
          cmp    eax,00504153h
          jz     L77EA4D13
          mov    ecx,00000408h
          cmp    eax,00474C45h
          jz     L77EA4D13
          cmp    eax,00004C45h
          jz     L77EA4D13
          cmp    eax,004C4C45h
          jz     L77EA4D13
          mov    ecx,00000413h
          cmp    eax,004E4C4Eh
          jz     L77EA4D13
          cmp    eax,00004C4Eh
          jz     L77EA4D13
          cmp    eax,004C444Eh
          jz     L77EA4D13
          mov    ecx,00000427h
          cmp    eax,004C544Ch
          jz     L77EA4D13
          cmp    eax,0000544Ch
          jz     L77EA4D13
          cmp    eax,0049544Ch
          jz     L77EA4D13
          mov    ecx,00000418h
          cmp    eax,00524F52h
          jz     L77EA4D13
          cmp    eax,00004F52h
          jz     L77EA4D13
          cmp    eax,004F4E52h
          jz     L77EA4D13
          mov    ecx,0000041Ah
          cmp    eax,00485248h
          jz     L77EA4D13
          cmp    eax,00005248h
          jz     L77EA4D13
          cmp    eax,00525648h
          jz     L77EA4D13
          mov    ecx,0000041Eh
          cmp    eax,00544854h
          jz     L77EA4D13
          cmp    eax,00004854h
          jz     L77EA4D13
          cmp    eax,00484154h
          jz     L77EA4D13
          mov    ecx,0000041Bh
          cmp    eax,00534B53h
          jz     L77EA4D13
          cmp    eax,00004B53h
          jz     L77EA4D13
          cmp    eax,004C4B53h
          jz     L77EA4D13
          mov    ecx,0000041Dh
          cmp    eax,00535653h
          jz     L77EA4D13
          cmp    eax,00005653h
          jz     L77EA4D13
          cmp    eax,00574553h
          jz     L77EA4D13
          mov    ecx,00000424h
          cmp    eax,00534C53h
          jz     L77EA4D13
          cmp    eax,00004C53h
          jz     L77EA4D13
          cmp    eax,004C5653h
          jz     L77EA4D13
          mov    ecx,00000416h
          cmp    eax,00425450h
          jz     L77EA4D13
          mov    ecx,00000816h
          cmp    eax,00505450h
          jz     L77EA4D13
          cmp    eax,004F5250h
          jz     L77EA4D13
          mov    ecx,00000415h
          cmp    al,50h
          mov    ecx,00000419h
          cmp    al,52h
          mov    ecx,00000426h
          cmp    al,4Ch
          mov    ecx,00000414h
          cmp    al,4Eh
          jz     L77EA4D13
          mov    ecx,0000041Fh
          cmp    al,54h
          jz     L77EA4D13
          mov    ecx,00000425h
          cmp    al,45h
          jz     L77EA4D13
          mov    ecx,00000809h
          cmp    eax,00474E45h
          jz     L77EA4D13
          mov    ecx,00000412h
          cmp    al,4Bh
          jz     L77EA4D13
          mov    ecx,00000401h
          cmp    al,41h
          jz     L77EA4D13
          mov    ecx,00000402h
          cmp    al,42h
          jz     L77EA4D13
          mov    ecx,00000403h
          cmp    al,43h
          jz     L77EA4D13
          mov    ecx,00000406h
          cmp    al,44h
          jz     L77EA4D13
          mov    ecx,0000040Bh
          cmp    al,45h
          jz     L77EA4D13
          mov    ecx,0000081Ah
          cmp    al,53h
          jz     L77EA4D13
          mov    ecx,0000042Ah
          cmp    al,56h
          jz     L77EA4D13
          mov    ecx,0000040Dh
          cmp    al,48h
          jz     L77EA4D13
          mov    ecx,00000422h
          cmp    al,55h
          jz     L77EA4D13
          mov    ecx,00000409h
          jmp    L77EA4D13
SWC77EA5154_LCMapStringEx_:
		  db	'LCMapStringEx',0000h		;unicode
SWC77EA5154_LCMapStringEx_W:
		  db	'LCMapStringEx',0000h		;unicode
;--------------------------------------------------------------------------------------------

;--------------------------------------------------------------------------------------------
; MAKE_EXPORT LCIDToLocaleName_XP=LCIDToLocaleName
LCIDToLocaleName_XP:
          push    ebp
          mov    ebp,esp
          push    ebx
          mov    eax,[ebp+08h]
          mov    ebx,L77EA5550
L77EA720A:
          cmp    dword  [ebx],00000000h
          mov    ecx,SSZ77EA5558_ja_JP
          jz     L77EA722A
          cmp    [ebx],ax
          lea    ecx,[ebx+08h]
          jz     L77EA722A
          cmp    [ebx+02h],ax
          lea    ecx,[ebx+08h]
          jz     L77EA722A
          add    ebx,00000010h
          jmp    L77EA720A
L77EA722A:
          push    esi
          push    edi
          push    ecx
          xor    eax,eax
          mov    edi,[ebp+0Ch]
          mov    esi,[ebp+10h]
L77EA7235:
          mov    al,[ecx]
          test    eax,eax
          jz     L77EA724B
          test    edi,edi
          jz     L77EA7248
          test    esi,esi
          jz     L77EA7248
          dec    esi
          jz     L77EA7248
          stosw
L77EA7248:
          inc    ecx
          jmp    L77EA7235
L77EA724B:
          test    edi,edi
          jz     L77EA7259
          xor    eax,eax
          stosw
          inc    ecx
          mov    eax,ecx
          pop    ecx
          sub    eax,ecx
L77EA7259:
          nop
          pop    edi
          pop    esi
          pop    ebx
          leave
          retn    0010h
L77EA5550:
         db    11h;
         db    04h;
         db    11h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    31h;   '1'
         db    31h;   '1'
SSZ77EA5558_ja_JP:
          db    'ja-JP',0
         db    00h;
         db    00h;
         db    09h;
         db    04h;
         db    09h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    30h;   '0'
         db    39h;   '9'
         db    65h;   'e'
         db    6Eh;   'n'
         db    2Dh;   '-'
         db    55h;   'U'
         db    53h;   'S'
         db    00h;
         db    00h;
         db    00h;
         db    82h;   '''
         db    04h;
         db    82h;   '''
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    38h;   '8'
         db    32h;   '2'
         db    66h;   'f'
         db    72h;   'r'
         db    2Dh;   '-'
         db    46h;   'F'
         db    52h;   'R'
         db    00h;
         db    00h;
         db    00h;
         db    04h;
         db    04h;
         db    04h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    30h;   '0'
         db    34h;   '4'
         db    7Ah;   'z'
         db    68h;   'h'
         db    2Dh;   '-'
         db    54h;   'T'
         db    57h;   'W'
         db    00h;
         db    00h;
         db    00h;
         db    04h;
         db    08h;
         db    04h;
         db    02h;
         db    30h;   '0'
         db    38h;   '8'
         db    30h;   '0'
         db    34h;   '4'
         db    7Ah;   'z'
         db    68h;   'h'
         db    2Dh;   '-'
         db    43h;   'C'
         db    4Eh;   'N'
         db    00h;
         db    00h;
         db    00h;
         db    10h;
         db    04h;
         db    10h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    31h;   '1'
         db    30h;   '0'
         db    69h;   'i'
         db    74h;   't'
         db    2Dh;   '-'
         db    49h;   'I'
         db    54h;   'T'
         db    00h;
         db    00h;
         db    00h;
         db    09h;
         db    08h;
         db    09h;
         db    02h;
         db    30h;   '0'
         db    38h;   '8'
         db    30h;   '0'
         db    39h;   '9'
         db    65h;   'e'
         db    6Eh;   'n'
         db    2Dh;   '-'
         db    47h;   'G'
         db    42h;   'B'
         db    00h;
         db    00h;
         db    00h;
         db    07h;
         db    04h;
         db    07h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    30h;   '0'
         db    37h;   '7'
         db    64h;   'd'
         db    65h;   'e'
         db    2Dh;   '-'
         db    44h;   'D'
         db    45h;   'E'
         db    00h;
         db    00h;
         db    00h;
         db    12h;
         db    04h;
         db    12h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    31h;   '1'
         db    32h;   '2'
         db    6Bh;   'k'
         db    6Fh;   'o'
         db    2Dh;   '-'
         db    4Bh;   'K'
         db    4Fh;   'O'
         db    00h;
         db    00h;
         db    00h;
         db    16h;
         db    04h;
         db    16h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    31h;   '1'
         db    36h;   '6'
         db    70h;   'p'
         db    74h;   't'
         db    2Dh;   '-'
         db    50h;   'P'
         db    54h;   'T'
         db    00h;
         db    00h;
         db    00h;
         db    16h;
         db    08h;
         db    16h;
         db    02h;
         db    30h;   '0'
         db    38h;   '8'
         db    31h;   '1'
         db    36h;   '6'
         db    70h;   'p'
         db    74h;   't'
         db    2Dh;   '-'
         db    42h;   'B'
         db    52h;   'R'
         db    00h;
         db    00h;
         db    00h;
         db    0Ah;
         db    04h;
         db    0Ah;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    30h;   '0'
         db    41h;   'A'
         db    65h;   'e'
         db    73h;   's'
         db    2Dh;   '-'
         db    45h;   'E'
         db    53h;   'S'
         db    00h;
         db    00h;
         db    00h;
         db    19h;
         db    04h;
         db    19h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    31h;   '1'
         db    39h;   '9'
         db    72h;   'r'
         db    75h;   'u'
         db    2Dh;   '-'
         db    52h;   'R'
         db    55h;   'U'
         db    00h;
         db    00h;
         db    00h;
         db    06h;
         db    04h;
         db    06h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    30h;   '0'
         db    36h;   '6'
         db    64h;   'd'
         db    61h;   'a'
         db    2Dh;   '-'
         db    44h;   'D'
         db    41h;   'A'
         db    00h;
         db    00h;
         db    00h;
         db    01h;
         db    04h;
         db    01h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    30h;   '0'
         db    31h;   '1'
         db    61h;   'a'
         db    72h;   'r'
         db    2Dh;   '-'
         db    53h;   'S'
         db    41h;   'A'
         db    00h;
         db    00h;
         db    00h;
         db    13h;
         db    04h;
         db    13h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    31h;   '1'
         db    33h;   '3'
         db    6Eh;   'n'
         db    6Ch;   'l'
         db    2Dh;   '-'
         db    4Eh;   'N'
         db    4Ch;   'L'
         db    00h;
         db    00h;
         db    00h;
         db    05h;
         db    04h;
         db    05h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    30h;   '0'
         db    35h;   '5'
         db    63h;   'c'
         db    73h;   's'
         db    2Dh;   '-'
         db    43h;   'C'
         db    53h;   'S'
         db    00h;
         db    00h;
         db    00h;
         db    02h;
         db    04h;
         db    02h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    30h;   '0'
         db    32h;   '2'
         db    62h;   'b'
         db    67h;   'g'
         db    2Dh;   '-'
         db    42h;   'B'
         db    47h;   'G'
         db    00h;
         db    00h;
         db    00h;
         db    25h;   '%'
         db    04h;
         db    25h;   '%'
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    32h;   '2'
         db    35h;   '5'
         db    65h;   'e'
         db    74h;   't'
         db    2Dh;   '-'
         db    45h;   'E'
         db    54h;   'T'
         db    00h;
         db    00h;
         db    00h;
         db    08h;
         db    04h;
         db    08h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    30h;   '0'
         db    38h;   '8'
         db    65h;   'e'
         db    6Ch;   'l'
         db    2Dh;   '-'
         db    45h;   'E'
         db    4Ch;   'L'
         db    00h;
         db    00h;
         db    00h;
         db    15h;
         db    04h;
         db    15h;
         db    01h;
         db    30h;   '0'
         db    34h;   '4'
         db    31h;   '1'
         db    35h;   '5'
         db    70h;   'p'
         db    6Ch;   'l'
         db    2Dh;   '-'
         db    50h;   'P'
         db    4Ch;   'L'
         db    00h;
         db    00h;
;----------------------------------------------------------------------------------------------------
; GetNameprocessid funtion series 
;----------------------------------------------------------------------------------------------------
; MAKE_EXPORT GetNamedPipeClientSessionId_XP=GetNamedPipeClientSessionId
GetNamedPipeClientSessionId_XP:
  		push	ebp
  		mov	ebp,esp
  		push	ecx
  		lea	eax,[ebp-04h]
  		push	eax
  		push	dword [ebp+0Ch]
  		mov	dword [ebp-04h],00000004h
  		push	SSZ77EA81D8_ClientSessionId
  		push	00000001h
  		push	dword [ebp+08h]
  		call	GetNamedPipeAttribute_XP
  		leave
  		retn	0008h
;---------------------------------------------------------------------------------------------------
; MAKE_EXPORT GetNamedPipeServerProcessId_XP=GetNamedPipeServerProcessId
GetNamedPipeServerProcessId_XP:
  		push	ebp
  		mov	ebp,esp
  		push	ecx
  		lea	eax,[ebp-04h]
  		push	eax
  		push	dword [ebp+0Ch]
  		mov	dword [ebp-04h],00000004h
  		push	SSZ77EA81E8_ServerProcessId
  		push	00000001h
  		push	dword [ebp+08h]
  		call	GetNamedPipeAttribute_XP
  		leave
  		retn	0008h
;---------------------------------------------------------------------------------------------------
; MAKE_EXPORT GetNamedPipeServerSessionId_XP=GetNamedPipeServerSessionId
GetNamedPipeServerSessionId_XP:
  		push	ebp
  		mov	ebp,esp
  		push	ecx
  		lea	eax,[ebp-04h]
  		push	eax
  		push	dword [ebp+0Ch]
  		mov	dword [ebp-04h],00000004h
  		push	SSZ77EA81F8_ServerSessionId
  		push	00000001h
  		push	dword [ebp+08h]
  		call	GetNamedPipeAttribute_XP
  		leave
  		retn	0008h
;------------------------------------------------------------------------------
; MAKE_EXPORT GetNamedPipeClientProcessId_XP=GetNamedPipeClientProcessId
GetNamedPipeClientProcessId_XP:
  		push	ebp
  		mov	ebp,esp
  		push	ecx
  		lea	eax,[ebp-04h]
  		push	eax
  		push	dword [ebp+0Ch]
  		mov	dword [ebp-04h],00000004h
  		push	SSZ77EA8208_ClientProcessId
  		push	00000001h
  		push	dword [ebp+08h]
  		call	GetNamedPipeAttribute_XP
  		leave
  		retn	0008h
;------------------------------------------------------------------------------
; MAKE_EXPORT GetNamedPipeClientComputerNameW_XP=GetNamedPipeClientComputerNameW
GetNamedPipeClientComputerNameW_XP:
  		push	ebp
  		mov	ebp,esp
  		mov	eax,[ebp+10h]
  		push	esi
  		mov	[ebp+10h],eax
  		lea	eax,[ebp+10h]
  		push	eax
  		push	dword [ebp+0Ch]
  		push	SSZ77EA8218_ClientComputerName
  		push	00000001h
  		push	dword [ebp+08h]
  		call	GetNamedPipeAttribute_XP
  		mov	esi,eax
  		test	esi,esi
  		jnz	L77EA8150
  		call	SUB_Y001
  		cmp	eax,00000490h
  		jnz	L77EA8150
  		push	000000E5h
  		call	RtlSetLastWin32Error
L77EA8150:
  		mov	eax,esi
  		pop	esi
  		pop	ebp
  		retn	000Ch
SUB_Y001
  		mov	eax,[00000018h]
  		mov	eax,[eax+34h]
  		retn
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
; MAKE_EXPORT GetNamedPipeAttribute_XP=GetNamedPipeAttribute
GetNamedPipeAttribute_XP:
  		push	ebp
  		mov	ebp,esp
  		push	ecx
  		push	ecx
  		mov	eax,[ebp+0Ch]
  		sub	eax,00000000h
  		jz 	L77EA81AE
  		dec	eax
  		jnz	L77EA81BF
  		mov	edx,00110030h
L77EA816C:
  		mov	eax,[ebp+10h]
  		push	esi
  		lea	esi,[eax+01h]
L77EA8173:
  		mov	cl,[eax]
  		inc	eax
  		test	cl,cl
  		jnz	L77EA8173
  		sub	eax,esi
  		mov	esi,[ebp+18h]
  		push	dword [esi]
  		inc	eax
  		push	dword [ebp+14h]
  		push	eax
  		push	dword [ebp+10h]
  		lea	eax,[ebp-08h]
  		push	edx
  		push	eax
  		push	00000000h
  		push	00000000h
  		push	00000000h
  		push	dword [ebp+08h]
  		call	_NtFsControlFile
  		test	eax,eax
  		jge	L77EA81B5
  		push	eax
  		call	SUB_L77E5826D
  		xor	eax,eax
L77EA81A9:
  		pop	esi
L77EA81AA:
  		leave
  		retn	0014h
;------------------------------------------------------------------------------
L77EA81AE:
  		mov	edx,00110028h
  		jmp	L77EA816C
L77EA81B5:
  		mov	eax,[ebp-04h]
  		mov	[esi],eax
  		xor	eax,eax
  		inc	eax
  		jmp	L77EA81A9
L77EA81BF:
  		dec	eax
  		jz 	L77EA81CD
  		push	00000057h
  		call	RtlSetLastWin32Error
  		xor	eax,eax
  		jmp	L77EA81AA
L77EA81CD:
  		mov	edx,00110038h
  		jmp	L77EA816C
SSZ77EA81D8_ClientSessionId:
  		db	'ClientSessionId',0
SSZ77EA81E8_ServerProcessId:
  		db	'ServerProcessId',0
SSZ77EA81F8_ServerSessionId:
  		db	'ServerSessionId',0
SSZ77EA8208_ClientProcessId:
  		db	'ClientProcessId',0
SSZ77EA8218_ClientComputerName:
  		db	'ClientComputerName',0
;--------------------------------------------------------------------------------
;Some other essential functions 
;--------------------------------------------------------------------------------
; MAKE_EXPORT GetLargePageMinimum_XP=GetLargePageMinimum
GetLargePageMinimum_XP:
  		xor	eax,eax
  		retn
; MAKE_EXPORT VirtualAllocExNuma_XP=VirtualAllocExNuma
VirtualAllocExNuma_XP:
  		push	dword [esp+14h]
  		push	dword [esp+14h]
  		push	dword [esp+14h]
  		mov	eax,[esp+14h]
  		test	eax,20000000h
  		jz 	L77EA7CBB
  		and	eax,3758096383 ;DFFFFFFFh
  		or	eax,00402000h
L77EA7CBB:
  		push	eax
  		push	dword [esp+14h]
  		call	VirtualAllocEx
  		retn	0018h
; MAKE_EXPORT GetDynamicTimeZoneInformation_XP=GetDynamicTimeZoneInformation
GetDynamicTimeZoneInformation_XP:
  		push	dword [esp+04h]
  		call	GetTimeZoneInformation
  		cmp	eax,4294967295 ;FFFFFFFFh
  		jz 	L77EA7CEA
  		push	eax
  		mov	eax,[esp+08h]
  		and	dword [eax+000000ACh],00000000h
  		and	dword [eax+000001ACh],00000000h
  		pop	eax
L77EA7CEA:
  		retn	0004h










