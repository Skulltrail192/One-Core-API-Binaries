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
 */
 
#define NDEBUG

#include "main.h"
#include <ketypes.h>

/***********************************************************************
 *           FlsAlloc   (KERNEL32.@) - For XP support
 */
DWORD 
WINAPI 
FlsAlloc(
	PFLS_CALLBACK_FUNCTION lpCallback
)
{
	return TlsAlloc();
}
/***********************************************************************
 *           FlsFree   (KERNEL32.@) - For XP support
 */
BOOL 
WINAPI 
FlsFree(
	DWORD dwFlsIndex
)
{
	return TlsFree(dwFlsIndex);
}

/***********************************************************************
 *           FlsGetValue   (KERNEL32.@) - For XP support
 */
PVOID 
WINAPI 
FlsGetValue( 
	DWORD index 
)
{
	return TlsGetValue(index);
}

/***********************************************************************
 *           FlsSetValue   (KERNEL32.@) - For XP support
 */
BOOL 
WINAPI 
FlsSetValue(
	DWORD dwFlsIndex, 
	PVOID lpFlsData
)
{
  return TlsSetValue(dwFlsIndex, lpFlsData);
}


/***********************************************************************
 *           ConvertThreadToFiberEx   (KERNEL32.@) - For XP support
 *
 * @implemented
 */
LPVOID
WINAPI
ConvertThreadToFiberEx(
	_In_opt_ LPVOID lpParameter,
    _In_ DWORD dwFlags
)
{
    PTEB Teb;
    PFIBER Fiber;

    /* Check for invalid flags */
    if (dwFlags & ~FIBER_FLAG_FLOAT_SWITCH)
    {
        /* Fail */
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }

    /* Are we already a fiber? */
    Teb = NtCurrentTeb();
    if (Teb->HasFiberData)
    {
        /* Fail */
        SetLastError(ERROR_ALREADY_FIBER);
        return NULL;
    }

    /* Allocate the fiber */
    Fiber = RtlAllocateHeap(RtlGetProcessHeap(),
                            0,
                            sizeof(FIBER));
    if (!Fiber)
    {
        /* Fail */
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return NULL;
    }

    /* Copy some contextual data from the thread to the fiber */
    Fiber->FiberData = lpParameter;
    Fiber->ExceptionList = Teb->NtTib.ExceptionList;
    Fiber->StackBase = Teb->NtTib.StackBase;
    Fiber->StackLimit = Teb->NtTib.StackLimit;
    Fiber->DeallocationStack = Teb->DeallocationStack;
    Fiber->FlsData = Teb->FlsData;
    Fiber->GuaranteedStackBytes = Teb->GuaranteedStackBytes;
    Fiber->ActivationContextStackPointer = Teb->ActivationContextStackPointer;

    /* Save FPU State if requested, otherwise just the basic registers */
    Fiber->FiberContext.ContextFlags = (dwFlags & FIBER_FLAG_FLOAT_SWITCH) ?
                                       (CONTEXT_FULL | CONTEXT_FLOATING_POINT) :
                                       CONTEXT_FULL;

    /* Associate the fiber to the current thread */
    Teb->NtTib.FiberData = Fiber;
    Teb->HasFiberData = TRUE;

    /* Return opaque fiber data */
    return (LPVOID)Fiber;
}