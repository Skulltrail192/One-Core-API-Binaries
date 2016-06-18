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
#include <main.h>

HANDLE Globalkeyed;
ULONG waiters;
HANDLE Key_Event = NULL;

#define FAST_M_WAKE		256
#define FAST_M_WAIT		512

int ConditionVariableSpinCount = 400;

static inline int interlocked_dec_if_nonzero( int *dest )
{
    int val, tmp;
	DbgPrint("Function: Internal interlocked_dec_if_nonzero. Dest value: %d\n",dest);
    for (val = *dest;; val = tmp)
    {
        if (!val || (tmp = InterlockedCompareExchange( dest, val - 1, val )) == val)
            break;
    }
    return val;
}

/***********************************************************************
  *           RtlInitializeConditionVariable   (NTDLL.@)
  *
  * Initializes the condition variable with NULL.
  *
  * PARAMS
  *  variable [O] condition variable
  *
  * RETURNS
  *  Nothing.
  */
void WINAPI RtlInitializeConditionVariable( PRTL_CONDITION_VARIABLE variable )
{
	DbgPrint("RtlInitializeConditionVariable called\n");
	if(Key_Event == NULL)
	{
		NtCreateKeyedEvent(&Key_Event, -1, NULL, 0);
		DbgPrint("Key_Event initialized\n");
	}
    variable->Ptr = NULL;
}

/***********************************************************************
 *           RtlWakeAllConditionVariable   (NTDLL.@)
 *
 * See WakeConditionVariable, wakes up all waiting threads.
 */
void 
WINAPI 
RtlWakeAllConditionVariable( 
	PRTL_CONDITION_VARIABLE variable 
)
{
	while(variable->Ptr){
		InterlockedDecrement((int *)&variable->Ptr);
		NtReleaseKeyedEvent( Key_Event, &variable, FALSE, NULL );
	}
}

/***********************************************************************
 *           RtlWakeConditionVariable   (NTDLL.@)
 *
 * Wakes up one thread waiting on the condition variable.
 *
 * PARAMS
 *  variable [I/O] condition variable to wake up.
 *
 * RETURNS
 *  Nothing.
 *
 * NOTES
 *  The calling thread does not have to own any lock in order to call
 *  this function.
 */
void 
WINAPI 
RtlWakeConditionVariable( 
	PRTL_CONDITION_VARIABLE variable 
)
{
	if(variable->Ptr){
		InterlockedDecrement((int *)&variable->Ptr);
		NtReleaseKeyedEvent( Key_Event, &variable->Ptr, FALSE, NULL );
	}
}

/***********************************************************************
 *           RtlSleepConditionVariableCS   (NTDLL.@)
 *
 * Atomically releases the critical section and suspends the thread,
 * waiting for a Wake(All)ConditionVariable event. Afterwards it enters
 * the critical section again and returns.
 *
 * PARAMS
 *  variable  [I/O] condition variable
 *  crit      [I/O] critical section to leave temporarily
 *  timeout   [I]   timeout
 *
 * RETURNS
 *  see NtWaitForKeyedEvent for all possible return values.
 */
NTSTATUS 
WINAPI 
RtlSleepConditionVariableCS( 
	PRTL_CONDITION_VARIABLE variable, 
	PRTL_CRITICAL_SECTION crit,
	const LARGE_INTEGER *timeout 
)
{
    NTSTATUS status;
    InterlockedExchangeAdd( (int *)&variable->Ptr, 1 );
    RtlLeaveCriticalSection(crit);
	status = NtWaitForKeyedEvent(Key_Event, &variable->Ptr, 0, NULL);
    RtlEnterCriticalSection(crit);
    return status;
}