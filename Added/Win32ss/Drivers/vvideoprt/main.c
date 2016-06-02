/*
 * PROJECT:         ReactOS WMI driver
 * COPYRIGHT:       GPL - See COPYING in the top level directory
 * FILE:            drivers/bootanim/main.c
 * PURPOSE:         Windows Management Intstrumentation
 * PROGRAMMERS:     Aleksey Bragin (aleksey@reactos.org)
 *                  
 */

/* INCLUDES *****************************************************************/

#include <stdio.h>
#include <ntddk.h>
#include <wmilib.h>

#define NDEBUG
#include <debug.h>
#include <ntimage.h>
#include <inbvfuncs.h>
#include <bootvid.h>
#include <ldrfuncs.h>
#include <..\Watchdog\watchdog.h>

PVOID GlobalSystemArgument1;
PVOID GlobalSystemArgument2;
PVOID GlobalDeferredContext;
LONG g_lWdpDisplayHandlerState;
PWORK_QUEUE_ITEM g_WdpWorkQueueItem;
ULONG GlobalCount = 0;
ULONG g_WdpBugCheckData = 0;

NTSTATUS
NTAPI
DriverEntry(IN PDRIVER_OBJECT DriverObject,
            IN PUNICODE_STRING RegistryPath)
{
	return STATUS_SUCCESS;
}

NTSTATUS NTAPI WdpBugCheckStuckDriverWorkItem(PVOID Thread)
{
  PVOID v1; // esi@1
  NTSTATUS result; // eax@4
  struct _KTHREAD *v3; // eax@7
  KPRIORITY v4; // eax@7
  HANDLE Handle; // [sp+8h] [bp-4h]@5

  v1 = Thread;
  if ( !Thread )
    ExRaiseStatus(0xC000000Du);
  if ( !(Thread))
    goto LABEL_13;
  result = WdMadeAnyProgress(Thread);
  if ( result )
    return result;
  if ( PsCreateSystemThread(&Handle, 0x1F03FFu, 0, 0, 0, NULL, v1) < 0 )
LABEL_13:
    KeBugCheckEx(0,0,0,0,0);
  if ( ObReferenceObjectByHandle(Handle, 0x20u, 0, 0, &Thread, 0) >= 0 )
  {
    v3 = KeGetCurrentThread();
    v4 = KeQueryPriorityThread(v3);
    KeSetPriorityThread((PKTHREAD)Thread, v4);
    ObfDereferenceObject(Thread);
  }
  return ZwClose(Handle);
}

VOID
NTAPI
WdDdiWatchdogDpcCallback(
    _In_ PKDPC Dpc,
    _In_opt_ PVOID DeferredContext,
    _In_opt_ PVOID SystemArgument1,
    _In_opt_ PVOID SystemArgument2)
{
  if ( _InterlockedCompareExchange(&g_lWdpDisplayHandlerState, 1, 0) )
  {
    WdCompleteEvent(SystemArgument2, SystemArgument1);
  }
  else
  {
    ++GlobalCount;
    g_WdpWorkQueueItem->List.Flink = 0;
    GlobalSystemArgument1 = SystemArgument1;
    GlobalSystemArgument2 = SystemArgument2;
    g_WdpBugCheckData = 234;
    GlobalDeferredContext = DeferredContext;
    g_WdpWorkQueueItem->WorkerRoutine = WdpBugCheckStuckDriverWorkItem;
    g_WdpWorkQueueItem->Parameter = &g_WdpBugCheckData;
    ExQueueWorkItem(g_WdpWorkQueueItem, HyperCriticalWorkQueue);
  }
}
