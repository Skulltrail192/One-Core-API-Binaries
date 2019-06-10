/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    util.h

Abstract:

    Internal support routines used in STORAHCI

Notes:

Revision History:

        Nathan Obr (natobr),  February 2005 
        Michael Xing (xiaoxing),  December 2009
--*/


#pragma once

__inline
VOID
AhciZeroMemory(
    __in __field_bcount(BufferSize) PCHAR Buffer,
    __in ULONG BufferSize
    )
{
    ULONG i;

    for (i = 0; i < BufferSize; i++) {
        Buffer[i] = 0;
    }
}

__inline
VOID
AhciFillMemory(
    __in __field_bcount(BufferSize) PCHAR Buffer,
    __in ULONG BufferSize,
    __in CHAR  Fill
    )
{
    ULONG i;

    for (i = 0; i < BufferSize; i++) {
        Buffer[i] = Fill;
    }
}

__inline
BOOLEAN
IsPortValid(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension,
    __in ULONG PathId
    )
{
    return ( (PathId <= AdapterExtension->HighestPort) && (AdapterExtension->PortExtension[PathId] != NULL) );
}


__inline
PAHCI_SRB_EXTENSION
GetSrbExtension (
    __in PSCSI_REQUEST_BLOCK Srb
    )
{
    //use lower 32bit is good enough for this calculation even on 64bit system, as 128 only takes 8 bits.
    ULONG_PTR left = ((ULONG_PTR)Srb->SrbExtension) % 128;

    if (left == 0) {
        //the buffer is already aligned.
        return (PAHCI_SRB_EXTENSION)Srb->SrbExtension;
    } else {
        //need to align to 128.
        return (PAHCI_SRB_EXTENSION)( ((PCHAR)Srb->SrbExtension) + 128 - left );
    }
}

__inline
VOID
MarkSrbToBeCompleted(
    __in PSCSI_REQUEST_BLOCK Srb
    )
{
    PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);
    SETMASK(srbExtension->Flags, ATA_FLAGS_COMPLETE_SRB);
}

__inline
BOOLEAN
IsDataTransferNeeded(
    __in PSCSI_REQUEST_BLOCK Srb
    )
{
    PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);
    return ( (srbExtension->Flags & (ATA_FLAGS_DATA_IN | ATA_FLAGS_DATA_OUT)) || (srbExtension->DataBuffer != NULL) );
}


__inline
BOOLEAN
IsDumpMode(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    return (AdapterExtension->StateFlags.DumpMode == 1);
}

__inline
BOOLEAN
IsMsnEnabled(
    __in PAHCI_DEVICE_EXTENSION DeviceExtension
    ) 
{
    return ( (DeviceExtension->DeviceParameters.AtaDeviceType == DeviceIsAta) &&
             (DeviceExtension->IdentifyDeviceData->MsnSupport == 1) );
}

__inline
BOOLEAN
IsRmfEnabled(
    __in PAHCI_DEVICE_EXTENSION DeviceExtension
    ) 
{
    return ( (DeviceExtension->DeviceParameters.AtaDeviceType == DeviceIsAta) &&
             (DeviceExtension->IdentifyDeviceData->CommandSetSupport.RemovableMediaFeature == 1) &&
             (DeviceExtension->IdentifyDeviceData->GeneralConfiguration.RemovableMedia == 1) );
}

__inline
BOOLEAN
NoFlushDevice(
    __in PAHCI_DEVICE_EXTENSION DeviceExtension
    ) 
{
    return ( (DeviceExtension->DeviceParameters.AtaDeviceType == DeviceIsAta) &&
             (DeviceExtension->IdentifyDeviceData->CommandSetSupport.WriteCache == 0) &&
             (DeviceExtension->IdentifyDeviceData->CommandSetSupport.FlushCache == 0) &&
             (DeviceExtension->IdentifyDeviceData->CommandSetSupport.FlushCacheExt == 0) );
}

__inline
BOOLEAN
IsTapeDevice(
    __in PAHCI_DEVICE_EXTENSION DeviceExtension
    )
{
    return ( (DeviceExtension->DeviceParameters.AtaDeviceType == DeviceIsAtapi) &&
             (DeviceExtension->IdentifyPacketData->GeneralConfiguration.CommandPacketType == 1) );
}


__inline
BOOLEAN
IsNCQCommand (
    PSCSI_REQUEST_BLOCK Srb
    )
/*++
    This could be a macro

Return Value:
    TRUE if CommandRegister is 0x60 or 0x61
--*/
{
    PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);

    if ( (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_READ_FPDMA_QUEUED) || 
         (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_FPDMA_QUEUED) ) {
        return TRUE;
    } else {
        return FALSE;
    }
}

__inline
BOOLEAN
IsCdromGESNCommand (
    PSCSI_REQUEST_BLOCK Srb
    )
/*
    This function recognizes the polling GESN command pattern sent by cdrom.sys.

Return Value:
    TRUE if command meets the Pattern. Otherwise, FALSE.
*/
{
    if ( (Srb->Cdb[0] == 0x4a ) &&
         (Srb->Cdb[1] == 0x01 ) &&
         (Srb->Cdb[2] == 0x00 ) &&
         (Srb->Cdb[3] == 0x00 ) &&
         //(Srb->Cdb[4] == 0x5a ) &&
         (Srb->Cdb[5] == 0x00 ) &&
         (Srb->Cdb[6] == 0x00 ) &&
         (Srb->Cdb[7] == 0x00 ) &&
         (Srb->Cdb[8] == 0x08 ) &&
         (Srb->Cdb[9] == 0x00 ) ) {
        return TRUE;
    } else {
        return FALSE;
    }
}

__inline
BOOLEAN
IsNormalCommand (
    PSCSI_REQUEST_BLOCK Srb
    )
{
/*++
    This could be a macro

Return Value:
    TRUE if any Command Register is any non NCQ IO command 
--*/
    PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);

    if ((srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_READ) || 
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE) ||
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_READ_EXT) || 
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_EXT) ||
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_READ_DMA) || 
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_DMA) || 
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_READ_DMA_EXT) || 
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_DMA_EXT) ||
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_DMA_FUA_EXT) || 
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_READ_DMA_QUEUED_EXT) || 
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_DMA_QUEUED_EXT) ||
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_DMA_QUEUED_FUA_EXT) ||
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_READ_MULTIPLE) || 
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_MULTIPLE) ||
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_READ_MULTIPLE_EXT) || 
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_MULTIPLE_EXT) ||  
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_MULTIPLE_FUA_EXT) ||
        (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_WRITE_DMA_QUEUED) ||
        (srbExtension->AtaFunction == ATA_FUNCTION_ATAPI_COMMAND) ) {
        return TRUE;
    } else {
        return FALSE;
    }
}

__inline
BOOLEAN
NeedRequestSense (
    PSCSI_REQUEST_BLOCK Srb
    )
{
    PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);
    
    return ( IsAtapiCommand(srbExtension->AtaFunction) &&  
             !IsRequestSenseSrb(srbExtension->AtaFunction) && 
             (Srb->SrbStatus == SRB_STATUS_ERROR) && 
             (Srb->SenseInfoBuffer != NULL) && 
             (Srb->SenseInfoBufferLength > 0) );
}

BOOLEAN
__inline
IsDeviceSupportsTrim (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
    // word 169: bit 0 -- Trim function is supported.
    return (ChannelExtension->DeviceExtension[0].IdentifyDeviceData->DataSetManagementFeature.SupportsTrim == 1);
}



__inline 
BOOLEAN
IsDeviceSupportsNCQ(
    __in PIDENTIFY_DEVICE_DATA IdentifyDeviceData
    )
{
#if (NTDDI_VERSION > NTDDI_WIN7)	
    return (IdentifyDeviceData->SerialAtaCapabilities.NCQ == TRUE);
#else
	UNREFERENCED_PARAMETER(IdentifyDeviceData);
	return FALSE;
#endif	
}

__inline 
BOOLEAN
IsDeviceSupportsAN(
    __in PIDENTIFY_PACKET_DATA IdentifyPacketData
    )
{
#if (NTDDI_VERSION > NTDDI_WIN7)	
    // This bit is actually from IDENTIFY_PACKET_DATA structure for ATAPI devices.
    return (IdentifyPacketData->SerialAtaFeaturesSupported.AsynchronousNotification == TRUE);
#else
	UNREFERENCED_PARAMETER(IdentifyPacketData);
	return FALSE;
#endif	
}

__inline 
BOOLEAN
IsExternalPort(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
    if ((ChannelExtension->Px->CMD.HPCP) || 
        ((ChannelExtension->AdapterExtension->CAP.SXS) && (ChannelExtension->Px->CMD.ESP))) {
        //Px->CMD.HPCP indicates that the port is hot-pluggable. (both signal and power cable)
        //CAP.SXS && Px->CMD.ESP indicates that it's an ESATA port. (only signal cable)
        return TRUE;
    } 
    
    return FALSE;
}

__inline
BOOLEAN
DeviceIdentificationComplete(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    ULONG i;

    for (i = 0; i <= AdapterExtension->HighestPort; i++) {
        if ( (AdapterExtension->PortExtension[i] != NULL) &&
             (AdapterExtension->PortExtension[i]->DeviceExtension->DeviceParameters.AtaDeviceType == DeviceUnknown) ) {
            return FALSE;
        }
    }

    return TRUE;
}

__inline
PULONG
SrbGetDataTransferLength (
    PSCSI_REQUEST_BLOCK Srb
    )
{
    PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);

    return ((srbExtension->DataTransferLength > 0) ? &srbExtension->DataTransferLength : &Srb->DataTransferLength);
}

__inline
PCDB
SrbGetCdb (
    PSCSI_REQUEST_BLOCK Srb
    )
{
    PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);

    return (IsNewCdbUsed(srbExtension->Flags) ? &srbExtension->Cdb : (PCDB)Srb->Cdb);
}

VOID
RecordExecutionHistory(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    ULONG Function
  );

VOID
RecordInterruptHistory(
    PAHCI_CHANNEL_EXTENSION ChannelExtension, 
    ULONG PxIS,
    ULONG PxSSTS,
    ULONG PxSERR,
    ULONG PxCI,
    ULONG PxSACT,
    ULONG Function
  );

VOID
Set_PxIE(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    PAHCI_INTERRUPT_ENABLE IE
    );


__inline
BOOLEAN
IsFuaSupported(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    ) 
{
    return (ChannelExtension->DeviceExtension->DeviceParameters.StateFlags.FuaSupported == 1);
}

__inline
BOOLEAN
NoLpmSupport(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
    return (ChannelExtension->AdapterExtension->CAP.SALP == 0);
}


__inline
BOOLEAN
NeedToSetTransferMode(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
    UNREFERENCED_PARAMETER(ChannelExtension); // make WDK sample build clean
    return FALSE;
}

__inline
BOOLEAN
IsSingleIoDevice(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    UNREFERENCED_PARAMETER(AdapterExtension); // make WDK sample build clean
    return FALSE;
}

__inline
BOOLEAN
AdapterResetInInit(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    UNREFERENCED_PARAMETER(AdapterExtension); // make WDK sample build clean
    return FALSE;
}

__inline
BOOLEAN
AdapterIgnoreHotPlug(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    UNREFERENCED_PARAMETER(AdapterExtension); // make WDK sample build clean
    return FALSE;
}

__inline
BOOLEAN
AdapterNoNonQueuedErrorRecovery(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    UNREFERENCED_PARAMETER(AdapterExtension); // make WDK sample build clean
    return FALSE;
}

__inline
BOOLEAN
CloResetEnabled(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    UNREFERENCED_PARAMETER(AdapterExtension); // make WDK sample build clean
    return (AdapterExtension->CAP.SCLO == 1);
}

__inline
BOOLEAN
NCQNotAllowed(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    UNREFERENCED_PARAMETER(AdapterExtension); // make WDK sample build clean
    return FALSE;
}

__inline
ULONG
GetOccupiedSlots (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
    return ( ChannelExtension->SlotManager.CommandsIssued | 
             ChannelExtension->SlotManager.NCQueueSlice | 
             ChannelExtension->SlotManager.NormalQueueSlice | 
             ChannelExtension->SlotManager.SingleIoSlice |
             ChannelExtension->SlotManager.CommandsToComplete );
}

__inline
BOOLEAN
ErrorRecoveryIsPending (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
    return ( (ChannelExtension->StateFlags.CallAhciReset == 1) ||
             (ChannelExtension->StateFlags.CallAhciReportBusChange == 1) ||
             (ChannelExtension->StateFlags.CallAhciNonQueuedErrorRecovery == 1) );
}

__inline
BOOLEAN
IsMiniportInternalSrb (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    )
{
    return ( (Srb == &ChannelExtension->Local.Srb) || (Srb == &ChannelExtension->Sense.Srb) );
}

PAHCI_MEMORY_REGISTERS
GetABARAddress(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension,
    __in PPORT_CONFIGURATION_INFORMATION ConfigInfo
    );

VOID
AhciCompleteJustSlottedRequest(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb,
    __in BOOLEAN AtDIRQL
    );

VOID
AhciCompleteRequest(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb,
    __in BOOLEAN AtDIRQL
    );

BOOLEAN
UpdateSetFeatureCommands(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in UCHAR OldFeatures,
    __in UCHAR NewFeatures,
    __in UCHAR OldSectorCount,
    __in UCHAR NewSectorCount
  );

VOID
GetAvailableSlot(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    PSCSI_REQUEST_BLOCK Srb
    );

VOID
AhciReportBusChange(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID
ReleaseSlottedCommand(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in UCHAR SlotNumber,
    __in BOOLEAN AtDIRQL
    );

VOID
RestorePreservedSettings(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in BOOLEAN AtDIRQL
    );

VOID
AhciPortIssueInitCommands(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
  );

BOOLEAN
AhciRequestTimerCallback (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PAHCI_TIMER_CALLBACK CallbackRoutine,
    __in_opt PVOID CallbackContext,
    __in ULONG Interval,
    __in TIMER_CALLBACK_TYPE CallbackType
    );

VOID 
AhciTimerCallback(
    __in PVOID  AdapterExtension
    );

BOOLEAN
CompareId (
    __in_opt PSTR DeviceId, 
    __in ULONG  DeviceIdLength,
    __in_opt __nullnullterminated PSTR TargetId,
    __in ULONG  TargetIdLength,
    __out_opt PULONG Value
);

ULONG
GetStringLength (
    __in PSTR   String, 
    __in ULONG  MaxLength
    );


