 /*
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
 * Copyright (c) 2015  Microsoft Corporation 
 * 
 * Module Name:
 *
 *   actctx.c
 *
 */
 
#define NDEBUG

#include <main.h> 

/*Maybe need for universal code*/
VOID NTAPI RtlFreeActivationContextStack 	( 	PACTIVATION_CONTEXT_STACK  	Stack	) 	
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
NTSTATUS 
NTAPI 
RtlAllocateActivationContextStack( 	
	IN PVOID *  	Context	
) 	
{
    PACTIVATION_CONTEXT_STACK ContextStack;

    /* Check if it's already allocated */
    if (*Context) return STATUS_SUCCESS;

    /* Allocate space for the context stack */
    ContextStack = RtlAllocateHeap(RtlGetProcessHeap(), HEAP_ZERO_MEMORY, sizeof (ACTIVATION_CONTEXT_STACK) );
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

    *Context = ContextStack;

    return STATUS_SUCCESS;
}