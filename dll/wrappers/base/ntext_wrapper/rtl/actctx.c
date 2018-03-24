/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    actctx.c

Abstract:

    Implement Activation Context functions

Author:

    Skulltrail 17-March-2018

Revision History:

--*/
 
#include <main.h> 

/*Maybe need for universal code*/
VOID
NTAPI
RtlFreeActivationContextStack(
    _In_ PACTIVATION_CONTEXT_STACK Stack
)	
{
    PRTL_ACTIVATION_CONTEXT_STACK_FRAME ActiveFrame, PrevFrame;

    /* Nothing to do if there is no stack */
    if (!Stack) return;

    /* Get the current active frame */
    ActiveFrame = Stack->ActiveFrame;

    /* Go through them in backwards order and release */
    while (ActiveFrame)
    {
        PrevFrame = ActiveFrame->Previous;
        RtlReleaseActivationContext(ActiveFrame->ActivationContext);
        RtlFreeHeap(RtlGetProcessHeap(), 0, ActiveFrame);
        ActiveFrame = PrevFrame;
    }

    /* Zero out the active frame */
    Stack->ActiveFrame = NULL;

    /* TODO: Empty the Frame List Cache */
    ASSERT(IsListEmpty(&Stack->FrameListCache));

    /* Free activation stack memory */
    RtlFreeHeap(RtlGetProcessHeap(), 0, Stack);
}

/*Maybe need for universal code*/
NTSYSAPI
NTSTATUS
NTAPI
RtlAllocateActivationContextStack(IN PACTIVATION_CONTEXT_STACK *Stack)
{
    PACTIVATION_CONTEXT_STACK ContextStack;

    /* Check if it's already allocated */
    if (*Stack) return STATUS_SUCCESS;

    /* Allocate space for the context stack */
    ContextStack = RtlAllocateHeap(RtlGetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ACTIVATION_CONTEXT_STACK));
    if (!ContextStack)
    {
        return STATUS_NO_MEMORY;
    }

    /* Initialize the context stack */
    ContextStack->Flags = 0;
    ContextStack->ActiveFrame = NULL;
    InitializeListHead(&ContextStack->FrameListCache);
    ContextStack->NextCookieSequenceNumber = 1;
    ContextStack->StackId = 1; //TODO: Timer-based

    *Stack = ContextStack;

    return STATUS_SUCCESS;
}