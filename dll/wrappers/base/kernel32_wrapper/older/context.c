/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
 *
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
 */
 
#define NDEBUG

#define XSTATE_LEGACY_FLOATING_POINT (0)

#define XSTATE_MASK_LEGACY_FLOATING_POINT   (1i64 << (XSTATE_LEGACY_FLOATING_POINT))

#include <main.h>
#include <ketypes.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

BOOL WINAPI CopyContext(PCONTEXT Destination, DWORD ContextFlags, PCONTEXT Source)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = RtlCopyContext(Destination, ContextFlags, Source);
  if ( status >= 0 )
  {
    result = 1;
  }
  else
  {
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    result = 0;
  }
  return result;
}

BOOL WINAPI InitializeContext(PVOID Buffer, DWORD ContextFlags, PCONTEXT *Context, PWORD ContextLength)
{
  DWORD flags; // edi@1
  NTSTATUS status; // eax@5
  PWORD LocalContextLength; // esi@7
  NTSTATUS otherStatus; // eax@9
  PCONTEXT oldContext; // [sp+4h] [bp-4h]@9

  flags = ContextFlags;
  if ( flags & 0x10000 || ContextFlags & 0x100000 )
  {
    if ( !RtlGetEnabledExtendedFeatures(XSTATE_MASK_LEGACY_FLOATING_POINT) )
      flags &= 0xFFFFFFBFu;
  }
  status = RtlGetExtendedContextLength(flags, &ContextFlags);
  if ( status < 0 )
  {
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    return 0;
  }
  LocalContextLength = ContextLength;
  if ( Buffer && (DWORD)ContextLength >= ContextFlags )
  {
    otherStatus = RtlInitializeExtendedContext(Buffer, flags, &oldContext);
    if ( otherStatus >= 0 )
    {
      *Context = RtlLocateLegacyContext(oldContext, 0);
      *LocalContextLength = ContextFlags;
      return 1;
    }
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(otherStatus);
  }
  else
  {
    *ContextLength = ContextFlags;
    RtlSetLastWin32Error(122);
  }
  return 0;
}

#if defined(_M_AMD64)
//
// CALLFRAME represents the layout of the stack upon entry to a function,
// including home locations for the first four parameters.
//

typedef struct _CALL_FRAME {
    PVOID ReturnAddress;
    PVOID Param1Home;
    PVOID Param2Home;
    PVOID Param3Home;
    PVOID Param4Home;
} CALL_FRAME, *PCALL_FRAME;

C_ASSERT((sizeof(CALL_FRAME) % 16) == 8);


VOID
BaseInitializeContext (
    OUT PCONTEXT Context,
    IN PVOID Parameter OPTIONAL,
    IN PVOID InitialPc OPTIONAL,
    IN PVOID InitialSp OPTIONAL,
    IN BASE_CONTEXT_TYPE ContextType
    )

/*++

Routine Description:

    This function initializes a context structure for use is an subsequent
    call to create a thread.

Arguments:

    Context - Supplies a pointer to context record to be initialized.

    Parameter - Supplies the thread's parameter.

    InitialPc - Supplies an initial program counter value.

    InitialSp - Supplies an initial stack pointer value.

    NewThread - Supplies a flag that specifies that this is a new thread,
        fiber, or process.

Return Value:

    None.

--*/

{
    PCALL_FRAME CallFrame;

    //
    // Initialize the context record so the thread will start execution
    // in the proper routine.
    //

    RtlZeroMemory((PVOID)Context, sizeof(CONTEXT));
    Context->ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;

    //
    // Build dummy call frame and return address on the stack.
    //

    CallFrame = (PCALL_FRAME)InitialSp - 1;
    CallFrame->ReturnAddress = NULL;

    Context->Rsp = (ULONG64)CallFrame;

    //
    // Initialize the start up parameters.
    //

    Context->Rcx = (ULONG64)InitialPc;
    Context->Rdx = (ULONG64)Parameter;

    //
    // Initialize the starting address dependent on the type of startup.
    //

    if (ContextType == BaseContextTypeProcess) {
        Context->Rip = (ULONG64)BaseProcessStart;

    } else if (ContextType == BaseContextTypeThread ) {
        Context->Rip = (ULONG64)BaseThreadStart;

    } else {
        Context->Rip = (ULONG64)BaseFiberStart;
    }

    return;
}

VOID
BaseFiberStart (
    VOID
    )

/*++

Routine Description:

    This function starts a fiber by calling the thread start up routine with
    the proper parameters.

Arguments:

    None.

Return Value:

    None.

--*/

{

    PFIBER Fiber;

    Fiber = GetCurrentFiber();
    BaseThreadStart((LPTHREAD_START_ROUTINE)Fiber->FiberContext.Rcx,
                    (LPVOID)Fiber->FiberContext.Rdx);

    return;
}
#endif