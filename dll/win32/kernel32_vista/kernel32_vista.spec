
@ stdcall InitOnceExecuteOnce(ptr ptr ptr ptr)
@ stdcall GetFileInformationByHandleEx(long long ptr long)
@ stdcall -ret64 GetTickCount64()

@ stdcall InitializeSRWLock(ptr) InitializeSRWLock_XP
@ stdcall AcquireSRWLockExclusive(ptr) AcquireSRWLockExclusive_XP
@ stdcall AcquireSRWLockShared(ptr) AcquireSRWLockShared_XP
@ stdcall ReleaseSRWLockExclusive(ptr) ReleaseSRWLockExclusive_XP
@ stdcall ReleaseSRWLockShared(ptr) ReleaseSRWLockShared_XP
@ stdcall TryAcquireSRWLockExclusive(ptr) TryAcquireSRWLockExclusive_XP
@ stdcall TryAcquireSRWLockShared(ptr) TryAcquireSRWLockShared_XP

@ stdcall InitializeConditionVariable(ptr)
@ stdcall SleepConditionVariableCS(ptr ptr long)
@ stdcall SleepConditionVariableSRW(ptr ptr long long)
@ stdcall WakeAllConditionVariable(ptr)
@ stdcall WakeConditionVariable(ptr)
