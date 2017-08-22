#include "k32_vista.h"

#define NDEBUG
#include <debug.h>
#include <rtlfuncs.h>

VOID
NTAPI
RtlInitializeConditionVariable(OUT PRTL_CONDITION_VARIABLE ConditionVariable);

VOID
NTAPI
RtlWakeConditionVariable(IN OUT PRTL_CONDITION_VARIABLE ConditionVariable);

VOID
NTAPI
RtlWakeAllConditionVariable(IN OUT PRTL_CONDITION_VARIABLE ConditionVariable);

NTSTATUS
NTAPI
RtlSleepConditionVariableCS(IN OUT PRTL_CONDITION_VARIABLE ConditionVariable,
                            IN OUT PRTL_CRITICAL_SECTION CriticalSection,
                            IN PLARGE_INTEGER TimeOut OPTIONAL);

NTSTATUS
NTAPI
RtlSleepConditionVariableSRW(IN OUT PRTL_CONDITION_VARIABLE ConditionVariable,
                             IN OUT PRTL_SRWLOCK SRWLock,
                             IN PLARGE_INTEGER TimeOut OPTIONAL,
                             IN ULONG Flags);

VOID
NTAPI
RtlInitializeSRWLock(OUT PRTL_SRWLOCK SRWLock);

VOID
NTAPI
RtlAcquireSRWLockShared(IN OUT PRTL_SRWLOCK SRWLock);

VOID
NTAPI
RtlReleaseSRWLockShared(IN OUT PRTL_SRWLOCK SRWLock);

VOID
NTAPI
RtlAcquireSRWLockExclusive(IN OUT PRTL_SRWLOCK SRWLock);

VOID
NTAPI
RtlReleaseSRWLockExclusive(IN OUT PRTL_SRWLOCK SRWLock);

// ULONG
// NTAPI
// RtlNtStatusToDosError(IN NTSTATUS Status);

// VOID
// WINAPI
// AcquireSRWLockExclusive(PSRWLOCK Lock)
// {
    // RtlAcquireSRWLockExclusive((PRTL_SRWLOCK)Lock);
// }

// VOID
// WINAPI
// AcquireSRWLockShared(PSRWLOCK Lock)
// {
    // RtlAcquireSRWLockShared((PRTL_SRWLOCK)Lock);
// }

// VOID
// WINAPI
// InitializeConditionVariable(PCONDITION_VARIABLE ConditionVariable)
// {
    // RtlInitializeConditionVariable((PRTL_CONDITION_VARIABLE)ConditionVariable);
// }

// VOID
// WINAPI
// InitializeSRWLock(PSRWLOCK Lock)
// {
    // RtlInitializeSRWLock((PRTL_SRWLOCK)Lock);
// }

// VOID
// WINAPI
// ReleaseSRWLockExclusive(PSRWLOCK Lock)
// {
    // RtlReleaseSRWLockExclusive((PRTL_SRWLOCK)Lock);
// }

// VOID
// WINAPI
// ReleaseSRWLockShared(PSRWLOCK Lock)
// {
    // RtlReleaseSRWLockShared((PRTL_SRWLOCK)Lock);
// }


VOID
WINAPI
InitializeConditionVariable(PCONDITION_VARIABLE ConditionVariable)
{
    RtlInitializeConditionVariable((PRTL_CONDITION_VARIABLE)ConditionVariable);
}

VOID
WINAPI
InitializeSRWLock_XP(PRTL_CRITICAL_SECTION crit)
{
	RtlInitializeCriticalSectionAndSpinCount(crit, 1024);
}

void 
WINAPI
checkIsCriticalNotInitialized(PRTL_CRITICAL_SECTION crit)
{
	if(!crit){
		InitializeSRWLock_XP(crit);
	}
}

VOID
WINAPI
AcquireSRWLockExclusive_XP(PRTL_CRITICAL_SECTION crit)
{
    checkIsCriticalNotInitialized(crit);	
	RtlEnterCriticalSection(crit);
}

VOID
WINAPI
AcquireSRWLockShared_XP(PRTL_CRITICAL_SECTION crit)
{
    checkIsCriticalNotInitialized(crit);	
    RtlEnterCriticalSection(crit);
}

BOOL
WINAPI
TryAcquireSRWLockExclusive_XP(PRTL_CRITICAL_SECTION crit)
{
    checkIsCriticalNotInitialized(crit);	
	return RtlTryEnterCriticalSection(crit);
}

BOOL
WINAPI
TryAcquireSRWLockShared_XP(PRTL_CRITICAL_SECTION crit)
{
    checkIsCriticalNotInitialized(crit);	
    return RtlTryEnterCriticalSection(crit);
}

VOID
WINAPI
ReleaseSRWLockExclusive_XP(PRTL_CRITICAL_SECTION crit)
{
   checkIsCriticalNotInitialized(crit);
   RtlLeaveCriticalSection(crit);
}

VOID
WINAPI
ReleaseSRWLockShared_XP(PRTL_CRITICAL_SECTION crit)
{
    checkIsCriticalNotInitialized(crit);	
	RtlLeaveCriticalSection(crit);
}

FORCEINLINE
PLARGE_INTEGER
GetNtTimeout(PLARGE_INTEGER Time, DWORD Timeout)
{
    if (Timeout == INFINITE) return NULL;
    Time->QuadPart = (ULONGLONG)Timeout * -10000;
    return Time;
}

BOOL
WINAPI
SleepConditionVariableCS(PCONDITION_VARIABLE ConditionVariable, PCRITICAL_SECTION CriticalSection, DWORD Timeout)
{
    NTSTATUS Status;
    LARGE_INTEGER Time;

    Status = RtlSleepConditionVariableCS(ConditionVariable, (PRTL_CRITICAL_SECTION)CriticalSection, GetNtTimeout(&Time, Timeout));
    if (!NT_SUCCESS(Status))
    {
        SetLastError(RtlNtStatusToDosError(Status));
        return FALSE;
    }
    return TRUE;
}

BOOL
WINAPI
SleepConditionVariableSRW(PCONDITION_VARIABLE ConditionVariable, PSRWLOCK Lock, DWORD Timeout, ULONG Flags)
{
    NTSTATUS Status;
    LARGE_INTEGER Time;

    Status = RtlSleepConditionVariableSRW(ConditionVariable, Lock, GetNtTimeout(&Time, Timeout), Flags);
    if (!NT_SUCCESS(Status))
    {
        SetLastError(RtlNtStatusToDosError(Status));
        return FALSE;
    }
    return TRUE;
}

VOID
WINAPI
WakeAllConditionVariable(PCONDITION_VARIABLE ConditionVariable)
{
    RtlWakeAllConditionVariable((PRTL_CONDITION_VARIABLE)ConditionVariable);
}

VOID
WINAPI
WakeConditionVariable(PCONDITION_VARIABLE ConditionVariable)
{
    RtlWakeConditionVariable((PRTL_CONDITION_VARIABLE)ConditionVariable);
}
