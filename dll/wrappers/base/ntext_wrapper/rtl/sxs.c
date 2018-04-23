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

#define ACTCTX_MAGIC       0xC07E3E11

static ACTIVATION_CONTEXT system_actctx = { ACTCTX_MAGIC, 1 };
static ACTIVATION_CONTEXT *process_actctx = &system_actctx;

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

static const WCHAR *find_app_settings( ACTIVATION_CONTEXT *actctx, const WCHAR *settings )
{
    unsigned int i, j;

    for (i = 0; i < actctx->num_assemblies; i++)
    {
        struct assembly *assembly = &actctx->assemblies[i];
        for (j = 0; j < assembly->entities.num; j++)
        {
            struct entity *entity = &assembly->entities.base[j];
            if (entity->kind == ACTIVATION_CONTEXT_SECTION_APPLICATION_SETTINGS &&
                !wcscmp( entity->u.settings.name, settings ))
                return entity->u.settings.value;
        }
    }
    return NULL;
}

static ACTIVATION_CONTEXT *check_actctx( HANDLE h )
{
    ACTIVATION_CONTEXT *ret = NULL, *actctx = h;

    if (!h || h == INVALID_HANDLE_VALUE) return NULL;
    _SEH2_TRY
    {
        if (actctx->magic == ACTCTX_MAGIC) ret = actctx;
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
    }
    _SEH2_END;
    return ret;
}

/***********************************************************************
 *		RtlQueryActivationContextApplicationSettings (NTDLL.@)
 */
NTSTATUS WINAPI RtlQueryActivationContextApplicationSettings( DWORD flags, HANDLE handle, const WCHAR *ns,
                                                              const WCHAR *settings, WCHAR *buffer,
                                                              SIZE_T size, SIZE_T *written )
{
    static const WCHAR namespaceW[] = {'h','t','t','p',':','/','/','s','c','h','e','m','a','s','.','m','i','c','r','o','s','o','f','t','.','c','o','m','/','S','M','I','/','2','0',0};
    ACTIVATION_CONTEXT *actctx = check_actctx( handle );
    const WCHAR *res;

    if (flags)
    {
        DbgPrint( "unknown flags %08x\n", flags );
        return STATUS_INVALID_PARAMETER;
    }

    if (ns && wcsncmp( ns, namespaceW, wcslen(namespaceW) ))
    {
        DbgPrint( "unknown namespace %s\n", ns );
        return STATUS_INVALID_PARAMETER;
    }

    if (!handle) handle = process_actctx;
    if (!(actctx = check_actctx( handle ))) return STATUS_INVALID_PARAMETER;

    if (!(res = find_app_settings( actctx, settings ))) return STATUS_SXS_KEY_NOT_FOUND;

    if (written) *written = wcslen(res) + 1;
    if (size < wcslen(res)) return STATUS_BUFFER_TOO_SMALL;
    wcscpy( buffer, res );
    return STATUS_SUCCESS;
}