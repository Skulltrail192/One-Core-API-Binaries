/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    sxs.c

Abstract:

    Side-by-side activation APIs for Win32

Author:

    Skulltrail     21-April-2018

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(kernel32);

NTSTATUS
NTAPI
BasepAllocateActivationContextActivationBlock(
	IN DWORD Flags,
    IN PVOID CompletionRoutine,
    IN PVOID CompletionContext,
    OUT PBASEP_ACTCTX_BLOCK *ActivationBlock
)
{
    NTSTATUS Status;
    ACTIVATION_CONTEXT_BASIC_INFORMATION ContextInfo;

    /* Clear the info structure */
    ContextInfo.dwFlags = 0;
    ContextInfo.hActCtx = NULL;

    /* Assume failure */
    if (ActivationBlock) *ActivationBlock = NULL;

    /* Only support valid flags */
    if (Flags & ~(1 | 2)) // FIXME: What are they? 2 looks like BASEP_ACTCTX_FORCE_BLOCK
    {
        /* Fail if unknown flags are passed in */
        Status = STATUS_INVALID_PARAMETER_1;
        goto Quickie;
    }

    /* Caller should have passed in an activation block */
    if (!ActivationBlock)
    {
        /* Fail otherwise */
        Status = STATUS_INVALID_PARAMETER_4;
        goto Quickie;
    }

    /* Query RTL for information on the current activation context */
    Status = RtlQueryInformationActivationContext(RTL_QUERY_ACTIVATION_CONTEXT_FLAG_USE_ACTIVE_ACTIVATION_CONTEXT,
                                                  NULL,
                                                  NULL,
                                                  ActivationContextBasicInformation,
                                                  &ContextInfo,
                                                  sizeof(ContextInfo),
                                                  NULL);
    if (!NT_SUCCESS(Status))
    {
        /* Failed -- bail out */
        TRACE("SXS: %s - Failure getting active activation context; ntstatus %08lx\n",
                __FUNCTION__, Status);
        goto Quickie;
    }

    /* Check if the current one should be freed */
    if (ContextInfo.dwFlags & 1)
    {
        /* Release and clear it */
        RtlReleaseActivationContext(ContextInfo.hActCtx);
        ContextInfo.hActCtx = NULL;
    }

    /* Check if there's an active context, or if the caller is forcing one */
    if (!(Flags & 2) || (ContextInfo.hActCtx))
    {
        /* Allocate the block */
        *ActivationBlock = RtlAllocateHeap(RtlGetProcessHeap(),
                                           0,
                                           sizeof(BASEP_ACTCTX_BLOCK));
        if (!(*ActivationBlock))
        {
            /* Ran out of memory, fail */
            Status = STATUS_NO_MEMORY;
            goto Quickie;
        }

        /* Fill it out */
        (*ActivationBlock)->ActivationContext = ContextInfo.hActCtx;
        (*ActivationBlock)->Flags = 0;
        if (Flags & 1) (*ActivationBlock)->Flags |= 1; // Not sure about this flag
        (*ActivationBlock)->CompletionRoutine = CompletionRoutine;
        (*ActivationBlock)->CompletionContext = CompletionContext;

        /* Tell Quickie below not to free anything, since this is success */
        ContextInfo.hActCtx = NULL;
    }

    /* Set success status */
    Status = STATUS_SUCCESS;

Quickie:
    /* Failure or success path, return to caller and free on failure */
    if (ContextInfo.hActCtx) RtlReleaseActivationContext(ContextInfo.hActCtx);
    return Status;
}

VOID
BasepFreeActivationContextActivationBlock(
    PBASE_ACTIVATION_CONTEXT_ACTIVATION_BLOCK ActivationBlock
    )
{
    if (ActivationBlock != NULL) {
        if (ActivationBlock->ActivationContext != NULL) {
            RtlReleaseActivationContext(ActivationBlock->ActivationContext);
            ActivationBlock->ActivationContext = NULL;
        }
        RtlFreeHeap(RtlProcessHeap(), 0, ActivationBlock);
    }
}
