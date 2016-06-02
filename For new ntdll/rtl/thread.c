/*
 * COPYRIGHT:         See COPYING in the top level directory
 * PROJECT:           ReactOS system libraries
 * PURPOSE:           Rtl user thread functions
 * FILE:              lib/rtl/thread.c
 * PROGRAMERS:
 *                    Alex Ionescu (alex@relsoft.net)
 *                    Eric Kohl
 *                    KJK::Hyperion
 */

/* INCLUDES *****************************************************************/

#include <rtl.h>

#define NDEBUG
#include <debug.h>

typedef struct _RTLP_GENERIC_TIMER {

    LIST_ENTRY List ;                   // All Timers and Queues are linked using this.
    ULONG DeltaFiringTime ;             // Time difference in Milliseconds from the TIMER entry
                                        // just before this entry
    union {
        ULONG RefCount ;        // Timer RefCount
        ULONG * RefCountPtr ;   // Pointer to Wait->Refcount
    } ;                         // keeps count of async callbacks still executing

    ULONG State ;               // State of timer: CREATED, DELETE, ACTIVE. DONT_FIRE

    union {

        // Used for Timer Queues

        struct  {

            LIST_ENTRY  TimerList ;     // Timers Hanging off of the queue
            LIST_ENTRY  UncancelledTimerList ;//List of one shot timers not cancelled
                                              //not used for wait timers
            #if DBG1
            ULONG NextDbgId; //ksl
            #endif
            
        } ;

        // Used for Timers

        struct  {
            struct _RTLP_GENERIC_TIMER *Queue ;// Queue to which this timer belongs
            struct _RTLP_WAIT *Wait ;  // Pointer to Wait event if timer is part of waits. else NULL
            ULONG Flags ;              // Flags indicating special treatment for this timer
            PVOID Function ;           // Function to call when timer fires
            PVOID Context ;            // Context to pass to function when timer fires
            ULONG Period ;             // In Milliseconds. Used for periodic timers.
            LIST_ENTRY TimersToFireList;//placed in this list if the timer is fired
        } ;
    } ;

    HANDLE CompletionEvent ;   // Event signalled when the timer is finally deleted

    #if DBG1
    ULONG DbgId; //ksl
    ULONG ThreadId ;
    ULONG ThreadId2 ;
    #endif

}  RTLP_GENERIC_TIMER, *PRTLP_GENERIC_TIMER ;

#define PRTLP_TIMER_QUEUE PRTLP_GENERIC_TIMER
#define RTLP_TIMER_QUEUE RTLP_GENERIC_TIMER
#define PRTLP_TIMER PRTLP_GENERIC_TIMER
#define RTLP_TIMER RTLP_GENERIC_TIMER

typedef VOID (*PPS_APC_ROUTINE)(
       __in_opt PVOID ApcArgument1,
       __in_opt PVOID ApcArgument2,
       __in_opt PVOID ApcArgument3
       );

LIST_ENTRY TimerQueues ;                // All timer queues are linked in this list
HANDLE TimerHandle ;        // Handle to the NT timer used for timeouts
LARGE_INTEGER   Last64BitTickCount ;
LARGE_INTEGER   Resync64BitTickCount ;
LARGE_INTEGER   Firing64BitTickCount ;
ULONG CompletedTimerInitialization ;    // Used for to check if Timer thread is initialized
ULONG TimerThreadId ;                   // Used to check if current thread is a timer thread

/* PRIVATE FUNCTIONS *******************************************************/

NTSTATUS
NTAPI
RtlpCreateUserStack(IN HANDLE hProcess,
                    IN SIZE_T StackReserve OPTIONAL,
                    IN SIZE_T StackCommit OPTIONAL,
                    IN ULONG StackZeroBits OPTIONAL,
                    OUT PINITIAL_TEB InitialTeb)
{
    NTSTATUS Status;
    SYSTEM_BASIC_INFORMATION SystemBasicInfo;
    PIMAGE_NT_HEADERS Headers;
    ULONG_PTR Stack = 0;
    BOOLEAN UseGuard = FALSE;
    ULONG Dummy;
    SIZE_T GuardPageSize;

    /* Get some memory information */
    Status = ZwQuerySystemInformation(SystemBasicInformation,
                                      &SystemBasicInfo,
                                      sizeof(SYSTEM_BASIC_INFORMATION),
                                      NULL);
    if (!NT_SUCCESS(Status)) return Status;

    /* Use the Image Settings if we are dealing with the current Process */
    if (hProcess == NtCurrentProcess())
    {
        /* Get the Image Headers */
        Headers = RtlImageNtHeader(NtCurrentPeb()->ImageBaseAddress);
        if (!Headers) return STATUS_INVALID_IMAGE_FORMAT;

        /* If we didn't get the parameters, find them ourselves */
        if (!StackReserve) StackReserve = Headers->OptionalHeader.
                                          SizeOfStackReserve;
        if (!StackCommit) StackCommit = Headers->OptionalHeader.
                                        SizeOfStackCommit;
    }
    else
    {
        /* Use the System Settings if needed */
        if (!StackReserve) StackReserve = SystemBasicInfo.AllocationGranularity;
        if (!StackCommit) StackCommit = SystemBasicInfo.PageSize;
    }

    /* Check if the commit is higher than the reserve*/
    if (StackCommit >= StackReserve)
    {
        /* Grow the reserve beyond the commit, up to 1MB alignment */
        StackReserve = ROUND_UP(StackCommit, 1024 * 1024);
    }

    /* Align everything to Page Size */
    StackReserve = ROUND_UP(StackReserve, SystemBasicInfo.AllocationGranularity);
    StackCommit = ROUND_UP(StackCommit, SystemBasicInfo.PageSize);

    /* Reserve memory for the stack */
    Status = ZwAllocateVirtualMemory(hProcess,
                                     (PVOID*)&Stack,
                                     StackZeroBits,
                                     &StackReserve,
                                     MEM_RESERVE,
                                     PAGE_READWRITE);
    if (!NT_SUCCESS(Status)) return Status;

    /* Now set up some basic Initial TEB Parameters */
    InitialTeb->PreviousStackBase = NULL;
    InitialTeb->PreviousStackLimit = NULL;
    InitialTeb->AllocatedStackBase = (PVOID)Stack;
    InitialTeb->StackBase = (PVOID)(Stack + StackReserve);

    /* Update the Stack Position */
    Stack += StackReserve - StackCommit;

    /* Check if we will need a guard page */
    if (StackReserve > StackCommit)
    {
        /* Remove a page to set as guard page */
        Stack -= SystemBasicInfo.PageSize;
        StackCommit += SystemBasicInfo.PageSize;
        UseGuard = TRUE;
    }

    /* Allocate memory for the stack */
    Status = ZwAllocateVirtualMemory(hProcess,
                                     (PVOID*)&Stack,
                                     0,
                                     &StackCommit,
                                     MEM_COMMIT,
                                     PAGE_READWRITE);
    if (!NT_SUCCESS(Status)) return Status;

    /* Now set the current Stack Limit */
    InitialTeb->StackLimit = (PVOID)Stack;

    /* Create a guard page */
    if (UseGuard)
    {
        /* Attempt maximum space possible */
        GuardPageSize = SystemBasicInfo.PageSize;
        Status = ZwProtectVirtualMemory(hProcess,
                                        (PVOID*)&Stack,
                                        &GuardPageSize,
                                        PAGE_GUARD | PAGE_READWRITE,
                                        &Dummy);
        if (!NT_SUCCESS(Status)) return Status;

        /* Update the Stack Limit keeping in mind the Guard Page */
        InitialTeb->StackLimit = (PVOID)((ULONG_PTR)InitialTeb->StackLimit +
                                         GuardPageSize);
    }

    /* We are done! */
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
RtlpFreeUserStack(IN HANDLE Process,
                  IN PINITIAL_TEB InitialTeb)
{
    SIZE_T Dummy = 0;
    NTSTATUS Status;

    /* Free the Stack */
    Status = ZwFreeVirtualMemory(Process,
                                 &InitialTeb->AllocatedStackBase,
                                 &Dummy,
                                 MEM_RELEASE);

    /* Clear the initial TEB */
    RtlZeroMemory(InitialTeb, sizeof(INITIAL_TEB));
    return Status;
}

/* FUNCTIONS ***************************************************************/


/*
 * @implemented
 */
NTSTATUS
__cdecl
RtlSetThreadIsCritical(IN BOOLEAN NewValue,
                       OUT PBOOLEAN OldValue OPTIONAL,
                       IN BOOLEAN NeedBreaks)
{
    ULONG BreakOnTermination;

    /* Initialize to FALSE */
    if (OldValue) *OldValue = FALSE;

    /* Fail, if the critical breaks flag is required but is not set */
    if ((NeedBreaks) &&
        !(NtCurrentPeb()->NtGlobalFlag & FLG_ENABLE_SYSTEM_CRIT_BREAKS))
    {
        return STATUS_UNSUCCESSFUL;
    }

    /* Check if the caller wants the old value */
    if (OldValue)
    {
        /* Query and return the old break on termination flag for the process */
        ZwQueryInformationThread(NtCurrentThread(),
                                 ThreadBreakOnTermination,
                                 &BreakOnTermination,
                                 sizeof(ULONG),
                                 NULL);
        *OldValue = (BOOLEAN)BreakOnTermination;
    }

    /* Set the break on termination flag for the process */
    BreakOnTermination = NewValue;
    return ZwSetInformationThread(NtCurrentThread(),
                                  ThreadBreakOnTermination,
                                  &BreakOnTermination,
                                  sizeof(ULONG));
}

/*
 @implemented
*/
NTSTATUS
NTAPI
RtlCreateUserThread(IN HANDLE ProcessHandle,
                    IN PSECURITY_DESCRIPTOR SecurityDescriptor OPTIONAL,
                    IN BOOLEAN CreateSuspended,
                    IN ULONG StackZeroBits OPTIONAL,
                    IN SIZE_T StackReserve OPTIONAL,
                    IN SIZE_T StackCommit OPTIONAL,
                    IN PTHREAD_START_ROUTINE StartAddress,
                    IN PVOID Parameter OPTIONAL,
                    OUT PHANDLE ThreadHandle OPTIONAL,
                    OUT PCLIENT_ID ClientId OPTIONAL)
{
    NTSTATUS Status;
    HANDLE Handle;
    CLIENT_ID ThreadCid;
    INITIAL_TEB InitialTeb;
    OBJECT_ATTRIBUTES ObjectAttributes;
    CONTEXT Context;

    /* First, we'll create the Stack */
    Status = RtlpCreateUserStack(ProcessHandle,
                                 StackReserve,
                                 StackCommit,
                                 StackZeroBits,
                                 &InitialTeb);
    if (!NT_SUCCESS(Status)) return Status;

    /* Next, we'll set up the Initial Context */
    RtlInitializeContext(ProcessHandle,
                         &Context,
                         Parameter,
                         StartAddress,
                         InitialTeb.StackBase);

    /* We are now ready to create the Kernel Thread Object */
    InitializeObjectAttributes(&ObjectAttributes,
                               NULL,
                               0,
                               NULL,
                               SecurityDescriptor);
    Status = ZwCreateThread(&Handle,
                            THREAD_ALL_ACCESS,
                            &ObjectAttributes,
                            ProcessHandle,
                            &ThreadCid,
                            &Context,
                            &InitialTeb,
                            CreateSuspended);
    if (!NT_SUCCESS(Status))
    {
        /* Free the stack */
        RtlpFreeUserStack(ProcessHandle, &InitialTeb);
    }
    else
    {
        /* Return thread data */
        if (ThreadHandle)
            *ThreadHandle = Handle;
        else
            NtClose(Handle);
        if (ClientId) *ClientId = ThreadCid;
    }

    /* Return success or the previous failure */
    return Status;
}

/*
 * @implemented
 */
VOID
NTAPI
RtlExitUserThread(NTSTATUS Status)
{
    /* Call the Loader and tell him to notify the DLLs */
    LdrShutdownThread();

    /* Shut us down */
    NtCurrentTeb()->FreeStackOnTermination = TRUE;
    NtTerminateThread(NtCurrentThread(), Status);
}

/*
 @implemented
*/
VOID
NTAPI
RtlFreeUserThreadStack(HANDLE ProcessHandle,
                       HANDLE ThreadHandle)
{
    NTSTATUS Status;
    THREAD_BASIC_INFORMATION ThreadBasicInfo;
    SIZE_T Dummy, Size = 0;
    PVOID StackLocation;

    /* Query the Basic Info */
    Status = NtQueryInformationThread(ThreadHandle,
                                      ThreadBasicInformation,
                                      &ThreadBasicInfo,
                                      sizeof(THREAD_BASIC_INFORMATION),
                                      NULL);
    if (!NT_SUCCESS(Status) || !ThreadBasicInfo.TebBaseAddress) return;

    /* Get the deallocation stack */
    Status = NtReadVirtualMemory(ProcessHandle,
                                 &((PTEB)ThreadBasicInfo.TebBaseAddress)->
                                 DeallocationStack,
                                 &StackLocation,
                                 sizeof(PVOID),
                                 &Dummy);
    if (!NT_SUCCESS(Status) || !StackLocation) return;

    /* Free it */
    NtFreeVirtualMemory(ProcessHandle, &StackLocation, &Size, MEM_RELEASE);
}

PTEB
NTAPI
_NtCurrentTeb(VOID)
{
    /* Return the TEB */
    return NtCurrentTeb();
}

NTSTATUS
NTAPI
RtlRemoteCall(IN HANDLE Process,
              IN HANDLE Thread,
              IN PVOID CallSite,
              IN ULONG ArgumentCount,
              IN PULONG Arguments,
              IN BOOLEAN PassContext,
              IN BOOLEAN AlreadySuspended)
{
    UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

ULONG
RtlpGetTimeRemaining (
    HANDLE TimerHandle
    )
/*++

Routine Description:

    Gets the time remaining on the specified NT timer

Arguments:

    TimerHandle - Handle to the NT timer

Return Value:

    Time remaining on the timer

--*/
{
    ULONG InfoLen ;
    TIMER_BASIC_INFORMATION Info ;

    NTSTATUS Status ;

    Status = NtQueryTimer (TimerHandle, TimerBasicInformation, &Info, sizeof(Info), &InfoLen) ;

    ASSERT (Status == STATUS_SUCCESS) ;

    // Return 0 if
    //
    // - the timer has already fired then return
    //   OR
    // - the timer is has more than 0x7f0000000 in its high part
    //   (which indicates that the timer was (probably) programmed for -1)

    
    if (Info.TimerState || ((ULONG)Info.RemainingTime.HighPart > 0x7f000000) ) {

        return 0 ;

    } else {

        return (ULONG) (Info.RemainingTime.QuadPart / 10000) ;

    }

}

__inline
LONGLONG
RtlpGet64BitTickCount(
    LARGE_INTEGER *Last64BitTickCount
    )
/*++

Routine Description:

    This routine is used for getting the latest 64bit tick count.

Arguments:

Return Value: 64bit tick count

--*/
{
    LARGE_INTEGER liCurTime ;

    liCurTime.QuadPart = NtGetTickCount() + Last64BitTickCount->HighPart ;

    // see if timer has wrapped.

    if (liCurTime.LowPart < Last64BitTickCount->LowPart) {
        liCurTime.HighPart++ ;
    }

    return (Last64BitTickCount->QuadPart = liCurTime.QuadPart) ;
}

__inline
LONGLONG
RtlpResync64BitTickCount(
    )
/*++

Routine Description:

    This routine is used for getting the latest 64bit tick count.

Arguments:

Return Value: 64bit tick count

Remarks: This call should be made in the first line of any APC queued
    to the timer thread and nowhere else. It is used to reduce the drift

--*/
{
    return Resync64BitTickCount.QuadPart = 
                    RtlpGet64BitTickCount(&Last64BitTickCount);
}

VOID
PrintTimerQueue(PLIST_ENTRY QNode, ULONG Delta, ULONG Count
    )
{
    PLIST_ENTRY Tnode ;
    PRTLP_TIMER Timer ;
    PRTLP_TIMER_QUEUE Queue ;
    
    Queue = CONTAINING_RECORD (QNode, RTLP_TIMER_QUEUE, List) ;
    DbgPrint("<%1d> Queue: %x FiringTime:%d\n", Count, (ULONG_PTR)Queue, 
                Queue->DeltaFiringTime);
    for (Tnode=Queue->TimerList.Flink; Tnode!=&Queue->TimerList; 
            Tnode=Tnode->Flink) 
    {
        Timer = CONTAINING_RECORD (Tnode, RTLP_TIMER, List) ;
        Delta += Timer->DeltaFiringTime ;
        DbgPrint("        Timer: %x Delta:%d Period:%d\n",(ULONG_PTR)Timer,
                    Delta, Timer->Period);
    }

}

VOID
NTAPI
RtlDebugPrintTimes (
    )
{    
    PLIST_ENTRY QNode ;
    ULONG Count = 0 ;
    ULONG Delta = RtlpGetTimeRemaining (TimerHandle) ;
    ULONG CurrentThreadId =  
                        HandleToUlong(NtCurrentTeb()->ClientId.UniqueThread) ;

    RtlpResync64BitTickCount();

    if (CompletedTimerInitialization != 1) {

        DbgPrint("===========RtlTimerThread not yet initialized==========\n");
        return ;
    }

    if (CurrentThreadId == TimerThreadId)
    {
        PRTLP_TIMER_QUEUE Queue ;
        
        DbgPrint("================Printing timerqueues====================\n");
        DbgPrint("TimeRemaining: %d\n", Delta);
        for (QNode = TimerQueues.Flink; QNode != &TimerQueues; 
                QNode = QNode->Flink)
        {
            Queue = CONTAINING_RECORD (QNode, RTLP_TIMER_QUEUE, List) ;
            Delta += Queue->DeltaFiringTime ;
            
            PrintTimerQueue(QNode, Delta, ++Count);
            
        }
        DbgPrint("================Printed ================================\n");
    }

    else
    {
        NtQueueApcThread(
                    TimerThreadHandle,
                    (PKNORMAL_ROUTINE)(PPS_APC_ROUTINE)RtlDebugPrintTimes,
                    NULL,
                    NULL,
                    NULL
                    );
    }
}

NTSTATUS
NTAPI
RtlQueueApcWow64Thread(
  IN HANDLE               ThreadHandle,
  IN PKNORMAL_ROUTINE      ApcRoutine,
  IN PVOID                ApcRoutineContext OPTIONAL,
  IN PIO_STATUS_BLOCK     ApcStatusBlock OPTIONAL,
  IN PVOID                ApcReserved OPTIONAL )
{
	return NtQueueApcThread(ThreadHandle, ApcRoutine, ApcRoutineContext, ApcStatusBlock, ApcReserved);
}