/*
 * PROJECT:        ReactOS Kernel
 * LICENSE:        GNU GPLv2 only as published by the Free Software Foundation
 * PURPOSE:        To Implement AHCI Miniport driver targeting storport NT 5.2
 * PROGRAMMERS:    Aman Priyadarshi (aman.eureka@gmail.com)
 */

//#include "storahci.h"

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

PVOID GlobalSystemArgument1;
PVOID GlobalSystemArgument2;
PVOID GlobalDeferredContext;
LONG g_lWdpDisplayHandlerState;
PWORK_QUEUE_ITEM g_WdpWorkQueueItem;
ULONG GlobalCount = 0;
ULONG g_WdpBugCheckData = 0;

#ifndef _NTDDK_
#define STORPORT_API DECLSPEC_IMPORT
#else
#define STORPORT_API
#endif


STORPORT_API
VOID
StorPortWriteRegisterUlong(
    IN PVOID HwDeviceExtension,
    IN PULONG Register,
    IN ULONG Value
    );

NTSTATUS
NTAPI
DriverEntry(IN PDRIVER_OBJECT DriverObject,
            IN PUNICODE_STRING RegistryPath)
{
	StorPortWriteRegisterUlong(NULL, NULL, 0);
	return STATUS_SUCCESS;
}

// BOOLEAN
// AhciPortInitialize (
    // PAHCI_PORT_EXTENSION PortExtension
// )
// {
	// StorPortWriteRegisterUlong(NULL, NULL, 0);
	
	// ScsiPortFreeDeviceBase(NULL,
                                       // NULL);

    // return TRUE;
// }// -- AhciPortInitialize();
