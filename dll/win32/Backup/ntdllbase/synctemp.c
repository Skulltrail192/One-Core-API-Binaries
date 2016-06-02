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
void WINAPI RtlInitializeConditionVariable( RTL_CONDITION_VARIABLE *variable )
{
	//NTSTATUS status;
	//variable->Ptr = 0;
	variable->Ptr = 0;	
	waiters = 0;
	NtCreateKeyedEvent(&Globalkeyed, -1, NULL, 0); 
}

/***********************************************************************
 *           RtlWakeAllConditionVariable   (NTDLL.@)
 *
 * See WakeConditionVariable, wakes up all waiting threads.
 */
void WINAPI RtlWakeAllConditionVariable( PRTL_CONDITION_VARIABLE variable )
{
	NTSTATUS status;
	int val = InterlockedExchange( (int *)&variable->Ptr, 0 );
    while (val-- > 0)
	{
		DbgPrint("RtlWakeAllConditionVariable. Val value: %d\n", val);
        status = NtReleaseKeyedEvent( Globalkeyed, &variable->Ptr, FALSE, NULL );
		DbgPrint("Function: RtlWakeConditionVariable. Status from: NtReleaseKeyedEvent. Status %08x\n", status);		
	}
}

void WINAPI RtlWakeConditionVariable( RTL_CONDITION_VARIABLE *variable)
{
	NTSTATUS status;
	if (interlocked_dec_if_nonzero( (int *)&variable->Ptr ))
	{
        status = NtReleaseKeyedEvent( Globalkeyed, &variable->Ptr, FALSE, NULL );
		DbgPrint("Function: RtlWakeConditionVariable. Status from: NtReleaseKeyedEvent. Status %08x\n", status);
	}
}

BOOL RtlpWakeSingle(RTL_CONDITION_VARIABLE *variable)
{
	DbgPrint("RtlWakeConditionVariable called by RtlpWakeSingle and RtlSleepConditionVariableCS\n");
	RtlWakeConditionVariable(variable);
	return TRUE;
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
 NTSTATUS NTAPI RtlSleepConditionVariableCS(RTL_CONDITION_VARIABLE *variable, RTL_CRITICAL_SECTION *crit, const LARGE_INTEGER *timeout)
{
   //RtlLeaveCriticalSection(crit);
   waiters++;
   variable->Ptr = (PVOID)waiters;
   NtWaitForKeyedEvent(Globalkeyed, variable->Ptr, 0, NULL);
   DbgPrint("RtlSleepConditionVariableCS\n");   
   //RtlEnterCriticalSection(crit);
}