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

#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

VOID WINAPI CloseThreadpool(
  _Inout_  PTP_POOL ptpp
)
{
    DbgPrint("Function: CloseThreadpool. Result from: TpAllocCleanupGroup. Status: %08x\n");  	
	;	
}

VOID WINAPI CloseThreadpoolWait(
  _Inout_  PTP_WAIT pwa
)
{
    DbgPrint("Function: CloseThreadpoolWait. Result from: TpAllocCleanupGroup. Status: %08x\n");  		
	;	
}

VOID WINAPI CloseThreadpoolTimer(
  _Inout_  PTP_TIMER pti
)
{
    DbgPrint("Function: CloseThreadpoolTimer. Result from: TpAllocCleanupGroup. Status: %08x\n");  		
	;	
}

VOID WINAPI CloseThreadpoolWork(
  _Inout_  PTP_WORK pwk
)
{
    DbgPrint("Function: CloseThreadpoolWork. Result from: TpAllocCleanupGroup. Status: %08x\n");  	
	;	
}

VOID WINAPI CloseThreadpoolCleanupGroup(
  _Inout_  PTP_CLEANUP_GROUP ptpcg
)
{
    DbgPrint("Function: CloseThreadpoolCleanupGroup. Result from: TpAllocCleanupGroup. Status: %08x\n");  		
	;	
}


VOID WINAPI CloseThreadpoolIo(
  _Inout_  PTP_IO pio
)
{
    DbgPrint("Function: CloseThreadpoolIo. Result from: TpAllocCleanupGroup. Status: %08x\n");  			
	;
}

VOID WINAPI CloseThreadpoolCleanupGroupMembers(
  _Inout_      PTP_CLEANUP_GROUP ptpcg,
  _In_         BOOL fCancelPendingCallbacks,
  _Inout_opt_  PVOID pvCleanupContext
)
{
    DbgPrint("Function: CloseThreadpoolIo. Result from: TpAllocCleanupGroup. Status: %08x\n");  	
	;
}

PTP_CLEANUP_GROUP WINAPI CreateThreadpoolCleanupGroup()
{
  NTSTATUS status; // eax@1
  PTP_CLEANUP_GROUP result; // eax@2
  PVOID group = NULL; // [sp+0h] [bp-4h]@1

#ifdef _M_IX86
	  group = RtlAllocateHeap(*(HANDLE *)(__readfsdword(48) + 24), 0, 56);//TpAllocCleanupGroup(group);
#elif defined(_M_AMD64)
      group = RtlAllocateHeap(*(HANDLE *)(__readgsqword(48) + 24), 0, 56);//TpAllocCleanupGroup(group);
#endif
  if ( !group)
  {
	status = STATUS_NOT_IMPLEMENTED;
	DbgPrint("Function: CreateThreadpoolCleanupGroup. Result from: TpAllocCleanupGroup. Status: %08x\n");  
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    result = 0;
  }
  else
  {
    result = (PTP_CLEANUP_GROUP)group;
  }
  return result;
}

PVOID WINAPI CreateThreadpool(PVOID reserved)
{
  NTSTATUS status; // eax@1
  PVOID result; // eax@2

  status = TpAllocPool(&reserved, reserved);
  DbgPrint("Function: CreateThreadpool. Result from: TpAllocPool. Status: %08x\n", status);  
  if ( status < 0 )
  {
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    result = 0;
  }
  else
  {
    result = reserved;
  }
  return result;
}

PTP_TIMER WINAPI CreateThreadpoolTimer(PTP_TIMER_CALLBACK PTP_TIMER_CALLBACK, PVOID pv, PTP_CALLBACK_ENVIRON pcbe)
{
  NTSTATUS status; // eax@1
  PTP_TIMER result; // eax@2

  status = STATUS_SUCCESS;//TpAllocTimer((PTP_TIMER *)pcbe, PTP_TIMER_CALLBACK, pv, pcbe);
  DbgPrint("Function: CreateThreadpoolTimer. Result from: TpAllocTimer. Status: %08x\n", status);  
  if ( status < 0 )
  {
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    result = 0;
  }
  else
  {
    result = (PTP_TIMER)pcbe;
  }
  return result;
}

PTP_WAIT WINAPI CreateThreadpoolWait(PTP_WAIT_CALLBACK pfnwa, PVOID pv, PTP_CALLBACK_ENVIRON pcbe)
{
  NTSTATUS status; // eax@1
  PTP_WAIT result; // eax@2

  status = STATUS_SUCCESS;//TpAllocWait((PTP_WAIT *)pcbe, pfnwa, pv, pcbe);
  DbgPrint("Function: CreateThreadpoolWait. Result from: TpAllocWait. Status: %08x\n", status);  
  if ( status < 0 )
  {
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    result = 0;
  }
  else
  {
    result = (PTP_WAIT)pcbe;
  }
  return result;
}

PTP_WORK WINAPI CreateThreadpoolWork(PTP_WORK_CALLBACK pfnwk, PVOID pv, PTP_CALLBACK_ENVIRON pcbe)
{
  NTSTATUS status; // eax@1
  PTP_WORK result; // eax@2

  status  = STATUS_SUCCESS;//TpAllocWork((PTP_WORK *)pcbe, pfnwk, pv, pcbe);
  DbgPrint("Function: CreateThreadpoolWork. Result from: TpAllocWork. Status: %08x\n", status);    
  if ( status < 0 )
  {
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    result = 0;
  }
  else
  {
    result = (PTP_WORK)pcbe;
  }
  return result;
}

VOID WINAPI LeaveCriticalSectionWhenCallbackReturns(
  _Inout_  PTP_CALLBACK_INSTANCE pci,
  _Inout_  PCRITICAL_SECTION pcs
)
{
	;	
}

BOOL WINAPI SetThreadpoolThreadMinimum(PTP_POOL ptpp, DWORD cthrdMic)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = STATUS_SUCCESS;//TpSetPoolMinThreads(ptpp, cthrdMic);
  DbgPrint("Function: SetThreadpoolThreadMinimum. Result from: TpSetPoolMinThreads. Status: %08x\n", status);  
  if ( status < 0 )
  {
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    result = 0;
  }
  else
  {
    result = 1;
  }
  return result;
}

BOOL WINAPI TrySubmitThreadpoolCallback(PTP_SIMPLE_CALLBACK a1, PVOID a2, PTP_CALLBACK_ENVIRON a3)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = STATUS_SUCCESS;//TpSimpleTryPost(a1, a2, a3);
  DbgPrint("Function: TrySubmitThreadpoolCallback. Result from: TpSimpleTryPost. Status: %08x\n", status);
  if ( status < 0 )
  {
    RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
    result = FALSE;
  }
  else
  {
    result = TRUE;
  }
  return result;
}

VOID WINAPI WaitForThreadpoolWaitCallbacks(
  _Inout_  PTP_WAIT pwa,
  _In_     BOOL fCancelPendingCallbacks
)
{
	;	
}