/*
 * PROJECT:         ReactOS Storage Stack
 * LICENSE:         DDK - see license.txt in the root dir
 * FILE:            drivers/storage/atapi/atapi.c
 * PURPOSE:         ATAPI IDE miniport driver
 * PROGRAMMERS:     Based on a source code sample from Microsoft NT4 DDK
 */

#include <ntddk.h>
//#include "atapi.h"               // includes scsi.h
//#include <ntddscsi.h>
#include <ntdddisk.h>
#include <ntddstor.h>
#include <storport.h>

//#define NDEBUG
#include <debug.h>

ULONG
NTAPI
DriverEntry(
    IN PVOID DriverObject,
    IN PVOID Argument2
    )
{
	return 0;
}

STORPORT_API
VOID
StorPortWriteRegisterUlong(
    IN PVOID HwDeviceExtension,
    IN PULONG Register,
    IN ULONG Value
    )
{
	;
}

PSTOR_SCATTER_GATHER_LIST 
StorPortGetScatterGatherList(
  _In_ PVOID               HwDeviceExtension,
  _In_ PSCSI_REQUEST_BLOCK Srb
)
{
	return NULL;
}

BOOLEAN 
StorPortSetDeviceQueueDepth(
  _In_ PVOID HwDeviceExtension,
  _In_ UCHAR PathId,
  _In_ UCHAR TargetId,
  _In_ UCHAR Lun,
  _In_ ULONG Depth
)
{
	return FALSE;
}

STORPORT_API
ULONG
StorPortInitialize(
    IN PVOID Argument1,
    IN PVOID Argument2,
    IN struct _HW_INITIALIZATION_DATA *HwInitializationData,
    IN PVOID HwContext
    )
{
	return 0;
}