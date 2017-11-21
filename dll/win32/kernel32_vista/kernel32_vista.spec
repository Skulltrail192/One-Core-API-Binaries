
@ stdcall InitOnceExecuteOnce(ptr ptr ptr ptr)
@ stdcall GetFileInformationByHandleEx(long long ptr long)
@ stdcall -ret64 GetTickCount64()

@ stdcall InitializeSRWLock(ptr) 
@ stdcall AcquireSRWLockExclusive(ptr) 
@ stdcall AcquireSRWLockShared(ptr) 
@ stdcall ReleaseSRWLockExclusive(ptr) 
@ stdcall ReleaseSRWLockShared(ptr) 
;@ stdcall TryAcquireSRWLockExclusive(ptr) 
;@ stdcall TryAcquireSRWLockShared(ptr) 

@ stdcall InitializeConditionVariable(ptr)
@ stdcall SleepConditionVariableCS(ptr ptr long)
@ stdcall SleepConditionVariableSRW(ptr ptr long long)
@ stdcall WakeAllConditionVariable(ptr)
@ stdcall WakeConditionVariable(ptr)
