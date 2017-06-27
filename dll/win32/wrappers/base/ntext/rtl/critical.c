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
 *  critical.c 
 */
  
#define NDEBUG

#include <main.h>

/*++
 * RtlGetCriticalSectionRecursionCount
 * @implemented NT5.2 SP1
 *
 *     Retrieves the recursion count of a given critical section.
 *
 * Params:
 *     CriticalSection - Critical section to retrieve its recursion count.
 *
 * Returns:
 *     The recursion count.
 *
 * Remarks:
 *     We return the recursion count of the critical section if it is owned
 *     by the current thread, and otherwise we return zero.
 *
 *--*/
LONG
NTAPI
RtlGetCriticalSectionRecursionCount(PRTL_CRITICAL_SECTION CriticalSection)
{
    if (CriticalSection->OwningThread == NtCurrentTeb()->ClientId.UniqueThread)
    {
        /*
         * The critical section is owned by the current thread,
         * therefore retrieve its actual recursion count.
         */
        return CriticalSection->RecursionCount;
    }
    else
    {
        /*
         * It is not owned by the current thread, so
         * for this thread there is no recursion.
         */
        return 0;
    }
}

/***********************************************************************
 *           RtlIsCriticalSectionLockedByThread   (NTDLL.@)
 *
 * Checks if the critical section is locked by the current thread.
 *
 * PARAMS
 *  crit [I/O] Critical section to check.
 *
 * RETURNS
 *  Success: TRUE. The critical section is locked.
 *  Failure: FALSE. The critical section is not locked.
 */
BOOL WINAPI RtlIsCriticalSectionLockedByThread( RTL_CRITICAL_SECTION *crit )
{
    return crit->OwningThread == NtCurrentTeb()->ClientId.UniqueThread &&
           crit->RecursionCount;
}
