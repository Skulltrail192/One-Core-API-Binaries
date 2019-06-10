/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    util.c

Abstract:

    This file contains untility functions of AHCI miniport.  


Notes:

Revision History:

        Nathan Obr (natobr),  February 2005 - September 2006 rev 1 (NCQ, LPM, Hotplug, persistant state) 
                              December 2006 - August 2007    rev 2 (async)
        Michael Xing (xiaoxing),  December 2009 - initial Storport miniport version

--*/

#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning(disable:4054) 
#pragma warning(disable:4055)
#pragma warning(disable:4214) // bit field types other than int
#pragma warning(disable:4201) // nameless struct/union

#include "generic.h"

PAHCI_MEMORY_REGISTERS
GetABARAddress(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension,
    __in PPORT_CONFIGURATION_INFORMATION ConfigInfo
    )
/*++
    Search a set of resources (I/O and memory) for an entry corresponding to an AHCI ABAR address.

It assumes:
    AdapterExtension->AhciBaseAddress has been set
Called by:
    AhciHwFindAdapter
It performs:
    1. If any resources are defined, loop over all resource entries, looking for the AHCI ABAR address. If found, return it.
    2. If this point is reached, ABAR was not found. Return NULL.
Affected Variables/Registers:
    none
ReturnValue:
    Address of AHCI controller memory registers if found, NULL otherwise

--*/
{
    PAHCI_MEMORY_REGISTERS  abar;
    ULONG i;
    
    abar = NULL;

  // loop over resource entries
    if (ConfigInfo->NumberOfAccessRanges > 0) {
        for (i = 0; i < ConfigInfo->NumberOfAccessRanges; i++) {
            if ((*(ConfigInfo->AccessRanges))[i].RangeStart.QuadPart == AdapterExtension->AhciBaseAddress) {
                abar = (PAHCI_MEMORY_REGISTERS)StorPortGetDeviceBase(AdapterExtension,
                                                                     ConfigInfo->AdapterInterfaceType,
                                                                     ConfigInfo->SystemIoBusNumber,
                                                                     (*(ConfigInfo->AccessRanges))[i].RangeStart,
                                                                     (*(ConfigInfo->AccessRanges))[i].RangeLength,
                                                                     (BOOLEAN)!(*(ConfigInfo->AccessRanges))[i].RangeInMemory);
                break;
            }
        }
    }

    return abar;
}


VOID
RecordExecutionHistory(
    PAHCI_CHANNEL_EXTENSION ChannelExtension, 
    ULONG Function
  )
/*++
    Takes a snapshot of the AHCI registers for debugging
It assumes:
    nothing
Called by:
    Everything

It performs:
    (overview)
    1 Select the next available index
    2 Take the snapshot
    (details)
    1.1 Select the next available index
    2.1 Copy over the Channel Extension header
    2.2 Copy over the AHCI registers
Affected Variables/Registers:
    none
Return Value:
    none
--*/
{
    int i;

  //1.1 Select the next available index
    // Actually, ExecutionHistoryNextAvailableIndex is used for current index.
    if ( ChannelExtension->ExecutionHistoryNextAvailableIndex >= 99) {
        ChannelExtension->ExecutionHistoryNextAvailableIndex = 0;
    }else{
        ChannelExtension->ExecutionHistoryNextAvailableIndex++;
    }
    
  //2.1 Copy over the Channel Extension header
    ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].Function = Function;
    if(ChannelExtension->AdapterExtension->IS) {
        StorPortCopyMemory(&ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].IS, &ChannelExtension->StateFlags, sizeof(ULONG));
    } else {
        ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].IS = (ULONG)~0;
    }
    StorPortCopyMemory(&ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].SlotManager, &ChannelExtension->SlotManager, sizeof(SLOT_MANAGER));
  //2.2 Copy over the AHCI registers
    if(ChannelExtension->Px) {
        StorPortCopyMemory(&ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].Px, ChannelExtension->Px, 0x40);
    } else {
        for(i=0; i<0x10; i++) {
            ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].Px[i] = (ULONG)~0;
        }
    }
}

VOID
RecordInterruptHistory(
    PAHCI_CHANNEL_EXTENSION ChannelExtension, 
    ULONG PxIS,
    ULONG PxSSTS,
    ULONG PxSERR,
    ULONG PxCI,
    ULONG PxSACT,
    ULONG Function
  )
/*++
    Takes a snapshot of the Interrupt for issue analysis.
It assumes:
    nothing
Called by:
    Everything

Affected Variables/Registers:
    none
Return Value:
    none
--*/
{
  //1. Select the next available index
    // Actually, ExecutionHistoryNextAvailableIndex is used for current index.
    if ( ChannelExtension->ExecutionHistoryNextAvailableIndex >= 99) {
        ChannelExtension->ExecutionHistoryNextAvailableIndex = 0;
    }else{
        ChannelExtension->ExecutionHistoryNextAvailableIndex++;
    }
    
  //2. Copy data
    ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].Function = Function;
    if(ChannelExtension->AdapterExtension->IS) {
        StorPortCopyMemory(&ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].IS, &ChannelExtension->StateFlags, sizeof(ULONG));
    } else {
        ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].IS = (ULONG)~0;
    }

    StorPortCopyMemory(&ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].SlotManager, &ChannelExtension->SlotManager, sizeof(SLOT_MANAGER));

    AhciZeroMemory((PCHAR)ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].Px, sizeof(ULONG)*16);
    ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].Px[4] = PxIS;
    ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].Px[10] = PxSSTS;
    ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].Px[12] = PxSERR;
    ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].Px[13] = PxSACT;
    ChannelExtension->ExecutionHistory[ChannelExtension->ExecutionHistoryNextAvailableIndex].Px[14] = PxCI;
}

VOID
Set_PxIE(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    PAHCI_INTERRUPT_ENABLE IE
    )
/*++
    This is just a space saver function that makes the interrupt configuration more readable

Called by:

--*/
{
    AHCI_INTERRUPT_ENABLE ie;
    AHCI_COMMAND cmd;
    PAHCI_ADAPTER_EXTENSION adapterExtension = ChannelExtension->AdapterExtension;
    
    ie.AsUlong = StorPortReadRegisterUlong(adapterExtension, &IE->AsUlong);
    ie.DHRE = 1; //Device to Host Register FIS Interrupt (DHRS):  A D2H Register FIS has been received with the ‘I’ bit set, and has been copied into system memory.
    ie.PSE  = 1; //PIO Setup FIS Interrupt (PSS):  A PIO Setup FIS has been received with the ‘I’ bit set, it has been copied into system memory, and the data related to that FIS has been transferred.  This bit shall be set even if the data transfer resulted in an error.
    ie.DSE  = 1; //DMA Setup FIS Interrupt (DSS):  A DMA Setup FIS has been received with the ‘I’ bit set and has been copied into system memory.
    ie.SDBE = 1; //Set Device Bits Interrupt (SDBS):  A Set Device Bits FIS has been received with the ‘I’ bit set and has been copied into system memory.
    
    ie.UFE  = 0; //Unknown FIS Interrupt (UFS): When set to ‘1’, indicates that an unknown FIS was received and has been copied into system memory.  This bit is cleared to ‘0’ by software clearing the PxSERR.DIAG.F bit to ‘0’.  Note that this bit does not directly reflect the PxSERR.DIAG.F bit.  PxSERR.DIAG.F is set immediately when an unknown FIS is detected, whereas this bit is set when that FIS is posted to memory.  Software should wait to act on an unknown FIS until this bit is set to ‘1’ or the two bits may become out of sync.
    ie.DPE  = 0; //Descriptor Processed (DPS):  A PRD with the ‘I’ bit set has transferred all of its data.  Refer to section 5.4.2.
    ie.PCE  = 1; //Port Connect Change Status (PCS): 1=Change in Current Connect Status. 0=No change in Current Connect Status.  This bit reflects the state of PxSERR.DIAG.X.  This bit is only cleared when PxSERR.DIAG.X is cleared.
    if(adapterExtension->CAP.SMPS) {
        ie.DMPE  = 1; //Device Mechanical Presence Status (DMPS): When set, indicates that a mechanical presence switch attached to this port has been opened or closed, which may lead to a change in the connection state of the device.  This bit is only valid if both CAP.SMPS and P0CMD.MPSP are set to ‘1’.
    } else {
        ie.DMPE  = 0;
    }

    //Reserved :14; 
    ie.PRCE = 1; //PhyRdy Change Status (PRCS): When set to ‘1’ indicates the internal PhyRdy signal changed state.  This bit reflects the state of P0SERR.DIAG.N.  To clear this bit, software must clear P0SERR.DIAG.N to ‘0’.
    ie.IPME = 0; //Incorrect Port Multiplier Status (IPMS):  Indicates that the HBA received a FIS from a device whose Port Multiplier field did not match what was expected.  The IPMS bit may be set during enumeration of devices on a Port Multiplier due to the normal Port Multiplier enumeration process.  It is recommended that IPMS only be used after enumeration is complete on the Port Multiplier.
    
    ie.OFE  = 1; //Overflow Status (OFS):  Indicates that the HBA received more bytes from a device than was specified in the PRD table for the command.
    //Reserved2 :1; 
    ie.INFE = 1; //Interface Non-fatal Error Status (INFS):  Indicates that the HBA encountered an error on the Serial ATA interface but was able to continue operation.  Refer to section 6.1.2.
    ie.IFE  = 1; //Interface Fatal Error Status (IFS):  Indicates that the HBA encountered an error on the Serial ATA interface which caused the transfer to stop.  Refer to section 6.1.2.
     
    ie.HBDE = 1; //Host Bus Data Error Status (HBDS):  Indicates that the HBA encountered a data error (uncorrectable ECC / parity) when reading from or writing to system memory.
    ie.HBFE = 1; //Host Bus Fatal Error Status (HBFS):  Indicates that the HBA encountered a host bus error that it cannot recover from, such as a bad software pointer.  In PCI, such an indication would be a target or master abort.
    ie.TFEE = 1; //Task File Error Status (TFES):  This bit is set whenever the status register is updated by the device and the error bit (bit 0) is set.
    cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->CMD.AsUlong);
    if(cmd.CPD) {    //check for PxCMD.CPD set to ‘1’ before setting CPDE
        ie.CPDE = 1; //Cold Port Detect Status (CPDS): When set, a device status has changed as detected by the cold presence detect logic.  This bit can either be set due to a non-connected port receiving a device, or a connected port having its device removed.  This bit is only valid if the port supports cold presence detect as indicated by PxCMD.CPD set to ‘1’.
    } else {
        ie.CPDE = 0;
    }
    StorPortWriteRegisterUlong(adapterExtension, &IE->AsUlong, ie.AsUlong);
}


ULONG
GetStringLength (
    __in PSTR   String, 
    __in ULONG  MaxLength
    )
{
    ULONG i;

    for (i = 0; i < MaxLength; i++) {
        if (String[i] == '\0') {
            break;
        }
    }

    return min(i + 1, MaxLength);
}

VOID
WMultiStringToAscii(
    __inout __nullnullterminated PSTR Strings,
    __in  ULONG CharacterCount
    )
/*+++

    This routine is used to convert the Source buffer in Wide Multi_SZ into ASCII.
    Note that CharacterCount should contain the double '0' as the ending of Multi_SZ
         This routine only works when the input string is ASCII WCHARs.

--*/
{
    PWCHAR  source = (PWCHAR)Strings;
    PUCHAR  destination = (PUCHAR)Strings;
    ULONG   i;

    // the Strings buffer should at least with length of 2 wchars.
    if ( (Strings != NULL) && (CharacterCount >= 4) ) {

        // Run through the Source buffer and convert the WCHAR to ASCII, placing
        // the converted value in Destination. Ensure that the destination buffer isn't overflowed.
        for (i = 0; (i < (CharacterCount / 2 - 1)) && ((*source != '\0') || (*(source + 1) != '\0')); i++) {
            *destination = (UCHAR)*source;
            destination++;
            source++;
        }

        // zero the reset of buffer
        AhciZeroMemory((PCHAR)destination, (CharacterCount * 2) - i);
    }

    return;
}


BOOLEAN
StringToULONG (
    __in PSTR InputString,
    __out PULONG Value
    )
/*
    This function converts string into an ULONG value
    The string should consist of characters of '0' ~ '9' and a '\0' as ending.
    
Return Value:
    TRUE - when the whole string is converted and the value is in range 0 ~ 0xFFFFFFFF
    FALSE - otherwise
*/    
{
    BOOLEAN result = TRUE;
    PSTR    tempString = InputString;
    ULONG   tempValue = 0;
    
    if ( (InputString == NULL) || (Value == NULL) ) {
        return FALSE;
    }
    
    while (*tempString != '\0') {
        if ( (*tempString >= '0') && (*tempString <= '9') ) {
            tempValue = tempValue * 10 + (*tempString++ - '0');
        } else {
            result = FALSE;
            break;
        }
        
        if ( (*tempString != '\0') && (MAXULONG / 10 < tempValue) ) {
            // the value will overflow, bail out
            result = FALSE;
            break;
        }
    }

    if (result == TRUE) {
        *Value = tempValue;
    }
    
    return result;
} 


BOOLEAN
CompareId (
    __in_opt PSTR DeviceId, 
    __in ULONG  DeviceIdLength,
    __in_opt __nullnullterminated PSTR TargetId,
    __in ULONG  TargetIdLength,
    __out_opt PULONG Value
)
/*++
Character comparison between DeviceId and TargetId to the min(DeviceIdLength, TargetIdLength)

Note that TargetId is a multi_sz string. DeviceId will be used to compare with each of strings in TargetId.

When string matches and 'Value' is asked, put value of remaining TargetId into "Value" field

DeviceId -- source string for comparison, in format of ASCII string. 

TargetId -- target string for comparison, in format of multi_sz string.
    TargetId format: xxxxxxxx nnn (first part is the ID to compare, second part is the value)

    Special character in TargetId:
        '?' - matches any character in DeviceId
        '*' - indicating end of TargetId, matches any characters left in DeviceId. It should be used only at the end of string.

Return Value:
    TRUE - On a match or contain
    FALSE - On a mismatch
--*/
{
    ULONG   compareLength;
    ULONG   i;
    PUCHAR  localDeviceId;
    PSTR    localStrings = TargetId;
    PUCHAR  localTargetId = (PUCHAR)localStrings;
    BOOLEAN matches = FALSE;

    if ((DeviceId == NULL) || (TargetId == NULL)) {
        // nothing to compare, return TRUE
        return TRUE;
    }

    while ( (*localStrings != '\0') || (*(localStrings + 1) != '\0') ) {
        localDeviceId = (PUCHAR)DeviceId;
        compareLength = min(DeviceIdLength, GetStringLength(localStrings, TargetIdLength));
        if (compareLength == 0) {
            return FALSE;
        }

        matches = TRUE;
        for(i = 0; i < compareLength; i++) {
            if (*localTargetId == '*') {
                break;  // whole string match
            } else if ( (*localDeviceId == '\0') || (*localTargetId == '\0') ) {
                matches = FALSE;
                break;
            } else if ( (*localDeviceId == *localTargetId) ||
                        (*localTargetId == '?') ) {
                localDeviceId++;
                localTargetId++;
            } else {
                matches = FALSE;
                break;
            }
        }

        if (matches) {
            break;
        }

        localStrings+= GetStringLength(localStrings, TargetIdLength) + 1;
        localTargetId = (PUCHAR)localStrings;
    }

    if ( (Value != NULL) && 
         (matches == TRUE) &&
         (*localTargetId == ' ') ) {

         //convert the left part of localTargetId into a value
         localTargetId++;
         if (!StringToULONG((PSTR)localTargetId, Value)) {
             matches = FALSE;
         }
    }

    return matches;
}


VOID
AhciReportBusChange(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
/*++
    Asks port driver to request a QDR on behalf of the miniport
It assumes:
    nothing
Called by:
    Indirectly by AhciHwInterrupt

It performs:
    1 Kicks off the Start Channel state machine
    2 Requests QDR
Affected Variables/Registers:
    none
--*/
{
    RecordExecutionHistory(ChannelExtension, 0x00000030);//AhciReportBusChange

  //1 Kicks off the Start Channel state machine
    AhciPortReset(ChannelExtension);
  //2 Requests QDR
    StorPortNotification(BusChangeDetected, ChannelExtension->AdapterExtension, ChannelExtension->PortNumber); //PathId
}


VOID
ReleaseSlottedCommand(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in UCHAR SlotNumber,
    __in BOOLEAN AtDIRQL
    )
/*++
    Performs Completion Data marshalling back into the SRB
It assumes:
    SRB STATUS has already been filled out
    ATA status from the AHCI registers is valid for this command
    SlotNumber points to a fully filled out Slot entry
Called by:
    AhciHwStartIo
    AhciNonQueuedErrorRecovery
    AhciCompleteIssuedSRBs

It performs:
    (overview)
    1 Intialize
    2 Marshal completion data
    3 Complete the Command
    (details)
    1.1 Keep track of the completion for our debugging records
    2.1 Handle Request Sense marshalling
    2.2.1 Handle everything else's marshalling
    2.2.2 If this was marked as needing return data, fill in the return Task File
    3.1 Make the slot available again
    3.2 Perform the Slot's last request, if there was one
    3.3.1 IO is completing, that IO may have paused the queue so unpause the queue
    3.3.2  Complete the command
Affected Variables/Registers:
    SRB
--*/
{
    PSLOT_CONTENT           slotContent;
    PAHCI_SRB_EXTENSION     srbExtension;
    PSCSI_REQUEST_BLOCK     srbToRelease;
    PATA_TASK_FILE          returnTaskFile;
    BOOLEAN                 isSenseSrb;

    slotContent = &ChannelExtension->Slot[SlotNumber];
    if (slotContent->Srb == NULL) {
        return;
    }

    srbExtension = GetSrbExtension(slotContent->Srb);
    isSenseSrb = IsRequestSenseSrb(srbExtension->AtaFunction);

    if (LogExecuteFullDetail(ChannelExtension->AdapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x10000053);//ReleaseSlottedCommand
    }

  //2/2 Log command completion part for our debugging records
    if (LogCommand(ChannelExtension->AdapterExtension->LogFlags)) {
        PCOMMAND_HISTORY         cmdHistory;
        slotContent->CommandHistoryIndex %= 64; // should not be 64 or bigger. do it anyway to make sure we are safe

        cmdHistory = &ChannelExtension->CommandHistory[slotContent->CommandHistoryIndex];
        StorPortCopyMemory((PVOID)&cmdHistory->CompletionFIS, (PVOID)&ChannelExtension->ReceivedFIS->D2hRegisterFis, sizeof(AHCI_D2H_REGISTER_FIS));
        StorPortCopyMemory((PVOID)&cmdHistory->CompletionPx, (PVOID)ChannelExtension->Px, 0x40);
        cmdHistory->SrbStatus = slotContent->Srb->SrbStatus;

    }

  //2. Then complete the command
    if (isSenseSrb) {
      //2.1 Handle Request Sense marshalling
        srbToRelease = (PSCSI_REQUEST_BLOCK)slotContent->Srb->OriginalRequest;

        //that original SRB must have SRB_STATUS_AUTOSENSE_VALID set appropriately
        if (slotContent->Srb->SrbStatus == SRB_STATUS_SUCCESS) { 
            srbToRelease->SrbStatus |= SRB_STATUS_AUTOSENSE_VALID;
            srbToRelease->ScsiStatus = SCSISTAT_CHECK_CONDITION;
        } else { 
            srbToRelease->SrbStatus &= ~SRB_STATUS_AUTOSENSE_VALID;
        }

        //finish use of Sense.Srb
        srbExtension->AtaFunction = 0;
    } else {
      //2.2.1 Handle everything else's marshalling
        AHCI_TASK_FILE_DATA taskFileData;

        srbToRelease = slotContent->Srb;

       //Record error and status
        taskFileData.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->TFD.AsUlong);

        srbExtension->AtaStatus = taskFileData.STS.AsUchar;
        srbExtension->AtaError = taskFileData.ERR;
     
      //2.2.2 If this was marked as needing return data, fill in the return Task File.
        if( IsReturnResults(srbExtension->Flags) ) {
            returnTaskFile = (PATA_TASK_FILE)srbToRelease->SenseInfoBuffer;

            returnTaskFile->Current.bCommandReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.Status;
            returnTaskFile->Current.bFeaturesReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.Error;
            returnTaskFile->Current.bCylHighReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.CylHigh;
            returnTaskFile->Current.bCylLowReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.CylLow;
            returnTaskFile->Current.bDriveHeadReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.Dev_Head;
            returnTaskFile->Current.bSectorCountReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.SectorCount;
            returnTaskFile->Current.bSectorNumberReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.SectorNumber;
            //if 48bit, get all of it
            if( Is48BitCommand(srbExtension->Flags)) {
                returnTaskFile->Previous.bCommandReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.Status;
                returnTaskFile->Previous.bFeaturesReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.Error;
                returnTaskFile->Previous.bCylHighReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.CylHigh_Exp;
                returnTaskFile->Previous.bCylLowReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.CylLow_Exp;
                returnTaskFile->Previous.bDriveHeadReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.Dev_Head;
                returnTaskFile->Previous.bSectorCountReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.SectorCount_Exp;
                returnTaskFile->Previous.bSectorNumberReg = ChannelExtension->ReceivedFIS->D2hRegisterFis.SectorNum_Exp;
            } 

            // set flag SRB_STATUS_AUTOSENSE_VALID so that Storport will copy it back to original Sense Buffer
            srbToRelease->SrbStatus |= SRB_STATUS_AUTOSENSE_VALID;
        }

        // interpret ATA error to be SCSI error and fill SenseBuffer
        // also log IO Record
        if (IsAtaCommand(srbExtension->AtaFunction)) {
            // IO Record for ATA device is logged in AtaMapError()
            AtaMapError(ChannelExtension, srbToRelease, slotContent->StateFlags.FUA);
        } else {
            if (SRB_STATUS(srbToRelease->SrbStatus) == SRB_STATUS_SUCCESS) {
                ChannelExtension->DeviceExtension[0].IoRecord.SuccessCount++;
            } else {
                ChannelExtension->DeviceExtension[0].IoRecord.OtherErrorCount++;
            }
        }
    }


  //3.1 Make the slot available again
    slotContent->CmdHeader = NULL;
    slotContent->CommandHistoryIndex = 0;
    slotContent->Srb = NULL;  
    slotContent->StateFlags.FUA = FALSE;

    //Clear the CommandsToComplete bit
    ChannelExtension->SlotManager.CommandsToComplete &= ~(1 << SlotNumber);
    ChannelExtension->SlotManager.HighPriorityAttribute &= ~(1 << SlotNumber);

  //3.3.1 IO is completing, that IO may have paused the queue so unpause the queue
    ChannelExtension->StateFlags.QueuePaused = FALSE;

  //3.3.2 Complete the command
    if ( !isSenseSrb && 
         (srbToRelease->SrbStatus == SRB_STATUS_PARITY_ERROR) &&
         (srbExtension->RetryCount == 0) ) {
        // if a command encounters CRC error, retry once
        srbToRelease->SrbStatus = SRB_STATUS_PENDING;
        srbExtension->RetryCount++;
        AhciProcessIo(ChannelExtension, srbToRelease, AtDIRQL);
    } else {
        // otherwise, complete it.
        AhciCompleteRequest(ChannelExtension, srbToRelease, AtDIRQL);
    }
}

VOID
AhciCompleteJustSlottedRequest(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb,
    __in BOOLEAN AtDIRQL
    )
/*
    This routine is to complete a request that occupies a slot but has not been issued to adapter yet.
    Srb status should be set before calling this routine.
*/
{
    PSLOT_CONTENT            slotContent;
    PAHCI_SRB_EXTENSION      srbExtension;

    srbExtension = GetSrbExtension(Srb);
    slotContent = &ChannelExtension->Slot[srbExtension->QueueTag];

    NT_ASSERT(slotContent->Srb == Srb);

  //1. Make the slot available again
    slotContent->CmdHeader = NULL;
    slotContent->CommandHistoryIndex = 0;
    slotContent->Srb = NULL;  
    slotContent->StateFlags.FUA = FALSE;

  //2. this function can be called from places that don't call ActivateQueue yet. Clear the bit in IO Slices
    if ((ChannelExtension->SlotManager.HighPriorityAttribute & (1 << srbExtension->QueueTag)) != 0) {
        ChannelExtension->SlotManager.HighPriorityAttribute &= ~(1 << srbExtension->QueueTag);
    }
    if ((ChannelExtension->SlotManager.NCQueueSlice & (1 << srbExtension->QueueTag)) != 0) {
        ChannelExtension->SlotManager.NCQueueSlice &= ~(1 << srbExtension->QueueTag);
    }
    if ((ChannelExtension->SlotManager.NormalQueueSlice & (1 << srbExtension->QueueTag)) != 0) {
        ChannelExtension->SlotManager.NormalQueueSlice &= ~(1 << srbExtension->QueueTag);
    }
    if ((ChannelExtension->SlotManager.SingleIoSlice & (1 << srbExtension->QueueTag)) != 0) {
        ChannelExtension->SlotManager.SingleIoSlice &= ~(1 << srbExtension->QueueTag);
    }
    if ((ChannelExtension->SlotManager.CommandsToComplete & (1 << srbExtension->QueueTag)) != 0) {
        ChannelExtension->SlotManager.CommandsToComplete &= ~(1 << srbExtension->QueueTag);
    }

  //3. Complete the command
    AhciCompleteRequest(ChannelExtension, Srb, AtDIRQL);

    return;
}

VOID
AhciCompleteRequest(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb,
    __in BOOLEAN AtDIRQL
    )
/*++
    Wrapper for ComleteRequest to protect against completing the local SRB back to port driver who doesn't know anything about local SRB
It assumes:
    SRB is completely ready to be completed back to the SRB generator
It performs:
    1. If Srb has completion routine, put it in completion queue.
    2. Complete the command back to the owner. Do not complete the Local Srb.
Called by:
    AhciHwStartIo
    RequiresControllerResources
    ReleaseSlottedCommand

NOTE:
    The caller of this routine should call AhciGetNextIos or ActiveQueue to program the command (from Srb completion routine) to adapter

--*/
{
    PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);

  //1. If Srb has completion routine, put it in completion queue. Otherwise, complete it.
  //     when reset happens, don't execute completion routine for LocalSrb. reset process will invoke RestorePreservedSettings() to start over the process.
    if ( (srbExtension->CompletionRoutine != NULL) && 
         (!IsMiniportInternalSrb(ChannelExtension, Srb) || (Srb->SrbStatus != SRB_STATUS_BUS_RESET)) ) {

        STOR_LOCK_HANDLE    lockhandle = {0};

        if (AtDIRQL == FALSE) {
            StorPortAcquireSpinLock(ChannelExtension->AdapterExtension, InterruptLock, NULL, &lockhandle);
        }

        AddQueue(ChannelExtension, &ChannelExtension->CompletionQueue, Srb, 0xDEADBEEF, 0x90);

        if (AtDIRQL == FALSE) {
            StorPortReleaseSpinLock(ChannelExtension->AdapterExtension, &lockhandle);
        }

        if (!IsDumpMode(ChannelExtension->AdapterExtension)) {
            StorPortIssueDpc(ChannelExtension->AdapterExtension, &ChannelExtension->CompletionDpc, ChannelExtension, Srb);
        } else {
            AhciPortSrbCompletionDpcRoutine(&ChannelExtension->CompletionDpc, ChannelExtension->AdapterExtension, ChannelExtension, Srb);
        }

    } else {
      //2. Complete the command back to the owner
        if (IsMiniportInternalSrb(ChannelExtension, Srb)) {
            RecordExecutionHistory(ChannelExtension, 0x20000045); //Exit AhciCompleteRequest,  Local SRB
            return;
        } else {
            NT_ASSERT(Srb->SrbStatus != SRB_STATUS_PENDING);
            StorPortNotification(RequestComplete, ChannelExtension->AdapterExtension, Srb);
        }
    }

    if (LogExecuteFullDetail(ChannelExtension->AdapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x10000045); //Exit AhciCompleteRequest, SRB from port driver
    }

    return;
}

VOID
GetAvailableSlot(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    PSCSI_REQUEST_BLOCK Srb
    )
/*++
    Returns the next available slot that can be used for an ATA command in the QueueTag field of the SRB
It assumes:
    
Called by:

It performs:
    1.1 Initialize variables
    2.1 Special case the slot for the local SRB
    2.2 Chose the slot circularly starting with CurrentCommandSlot
    3.1 Update CurrentCommandSlot

Affected Variables/Registers:

Return Value:
    Next available slot number.  If SRB is localSRB then slot is 0.
    If no slot is avalable tag returned is 0xFF.
--*/
{
    ULONG               allocated;
    UCHAR               limit;
    UCHAR               i;
    PAHCI_SRB_EXTENSION srbExtension;

    srbExtension = GetSrbExtension(Srb);

  //1.1 Initialize variables
    srbExtension->QueueTag = 0xFF;
    limit = ChannelExtension->CurrentCommandSlot;

    allocated = GetOccupiedSlots(ChannelExtension);
    
  //2.1 Use slot 0 for internal commands, don't increment CCS
    if (Srb == &ChannelExtension->Local.Srb ) {
        if ((allocated & (1 << 0)) > 0) {
            srbExtension->QueueTag = 0xFF;
        } else {
            srbExtension->QueueTag = 0;
        }
        return;
    }

  //2.2 Chose the slot circularly starting with CCS
    for (i = limit; i <= ChannelExtension->AdapterExtension->CAP.NCS; i++) {
        if ( (allocated & (1 << i)) == 0 ) {
            srbExtension->QueueTag = i;
            goto getout;
        }
    }
    
    for (i = 1; i < limit; i++) {
        if ( (allocated & (1 << i)) == 0 ) {
            srbExtension->QueueTag = i;
            goto getout;
        }
    }

getout:
  //3.1 Update CurrentCommandSlot
    if (IsRequestSenseSrb(srbExtension->AtaFunction)) {
      //If this SRB is for Request Sense, make sure it is given the next chance to run during ActivateQueue by not incrementing CCS.
        return;      
    }

    ChannelExtension->CurrentCommandSlot++;
    if (ChannelExtension->CurrentCommandSlot == (ChannelExtension->AdapterExtension->CAP.NCS + 1)) {
        ChannelExtension->CurrentCommandSlot = 1;
    }

    return;
}


BOOLEAN
UpdateSetFeatureCommands(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in UCHAR OldFeatures,
    __in UCHAR NewFeatures,
    __in UCHAR OldSectorCount,
    __in UCHAR NewSectorCount
  )
/*++
    Edit a PersistentSetting command
It assumes:
    0xFF means invalid feature
Called by:
    RequiresControllerResources

It performs:
    1.1 Find the location of the given command
    2.1 Replace it with the new command
Affected Variables/Registers:
    PersistentSettings
Return Value:
    TRUE if the edit succeeded
    FALSE if the command did not already exist
--*/
{
    BOOLEAN done = FALSE;
    UCHAR   i = 0;
    
    for (i = 0; i < MAX_SETTINGS_PRESERVED; i++) {
      //
        if ( (ChannelExtension->PersistentSettings.CommandParams[i].Features == OldFeatures) &&
             (ChannelExtension->PersistentSettings.CommandParams[i].SectorCount == OldSectorCount) ) {
            //Replace old one it with the new command
            ChannelExtension->PersistentSettings.CommandParams[i].Features = NewFeatures;
            ChannelExtension->PersistentSettings.CommandParams[i].SectorCount = NewSectorCount;
            done = TRUE;
            break;
        } else if ( (ChannelExtension->PersistentSettings.CommandParams[i].Features == NewFeatures) &&
                    (ChannelExtension->PersistentSettings.CommandParams[i].SectorCount == NewSectorCount) ) {
            //the new command is already in arrary
            done = TRUE;
            break;
        }
    }

    if (!done) {
        //try to find an empty slot 
        ULONG slotsAvailable = ChannelExtension->PersistentSettings.Slots;
        i = 0;
        while (slotsAvailable) {
            slotsAvailable >>= 1;
            i++;
        }
        //Add the new command into the empty slot
        if (i < MAX_SETTINGS_PRESERVED) {   
            ChannelExtension->PersistentSettings.CommandParams[i].Features = NewFeatures;
            ChannelExtension->PersistentSettings.CommandParams[i].SectorCount = NewSectorCount;
            //mark the Persistent settings slot as programmed
            ChannelExtension->PersistentSettings.Slots |= (1 << i);

            done = TRUE;
        } else {
            NT_ASSERT(FALSE);  //we don't expect so much commands for persistent settings.
        }
    }

    return done;
}

VOID
RestorePreservedSettings(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in BOOLEAN AtDIRQL
  )
/*++
    Send each of the commands in the PersistentSettings structure one at a time using the local SRB
It assumes:
    The local SRB is not currently in use
Called by:
    AhciPortIssueInitCommands
    AhciPortReset
    AhciNonQueuedErrorRecovery

It performs:
    1 Reinitialize all the commands to send
    2 Start the first command

Affected Variables/Registers:
--*/
{
    BOOLEAN reservedSlotInUse = 0;

    if (LogExecuteFullDetail(ChannelExtension->AdapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x00000041); //RestorePreservedSettings
    }

  //1 Reinitialize all the commands to send
    ChannelExtension->PersistentSettings.SlotsToSend = ChannelExtension->PersistentSettings.Slots;
    reservedSlotInUse = InterlockedBitTestAndSet((LONG*)&ChannelExtension->StateFlags, 3);    //ReservedSlotInUse field is at bit 3

    if (reservedSlotInUse == 1) {
        // the process has been started.
        return;
    }

  //2 Start the first command
    IssuePreservedSettingCommands(ChannelExtension, NULL);

  //3 Starts processing the command. Only need to do the first command if it exists. all others will be done by processing completion routine.
    if (ChannelExtension->Local.Srb.SrbExtension != NULL) {
        AhciProcessIo(ChannelExtension, &ChannelExtension->Local.Srb, AtDIRQL);
    }

    return;
}

VOID
AhciPortIssueInitCommands(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
  )
/*++
    Send each of the commands in the Init Command list and PersistentSettings structure, one at a time using the local SRB
It assumes:
    The local SRB is not currently in use
Called by:
    AhciDeviceInitialize

It performs:
    1 Reinitialize all the commands to send
    2 Start the first command

Affected Variables/Registers:
--*/
{
    BOOLEAN reservedSlotInUse = 0;

    if (LogExecuteFullDetail(ChannelExtension->AdapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x00000042); //AhciPortIssueInitCommands
    }

  //1 Reinitialize all the commands to send
    ChannelExtension->DeviceInitCommands.CommandToSend = 0;
    reservedSlotInUse = InterlockedBitTestAndSet((LONG*)&ChannelExtension->StateFlags, 3);    //ReservedSlotInUse field is at bit 3

    if (reservedSlotInUse == 1) {
        // the process has been started.
        return;
    }

  //2 Start the first command
    IssueInitCommands(ChannelExtension, NULL);

  //3 Starts processing the command. Only need to do the first command if it exists. all others will be done by processing completion routine.
    if (ChannelExtension->Local.Srb.SrbExtension != NULL) {
        AhciProcessIo(ChannelExtension, &ChannelExtension->Local.Srb, FALSE);
    }

    return;
}


VOID
RegisterTimerCallback (
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    PAHCI_CHANNEL_EXTENSION channelExtension;
    ULONG   i, j;

    // register the first callback from the loop.
    for (i = 0; i <= AdapterExtension->HighestPort; i++) {
        if (AdapterExtension->PortExtension[i] != NULL) {
            channelExtension = AdapterExtension->PortExtension[i];

            for (j = 0; j < ((ULONG)CallbackTypeMax - 1); j++) {
                if (channelExtension->TimerContext[j].CallBack != NULL) {
                    StorPortNotification(RequestTimerCall, AdapterExtension, AhciTimerCallback, channelExtension->TimerContext[j].Interval);
                    break;
                }
            }
        }
    }

    return;
}

VOID 
AhciTimerCallback(
    __in PVOID  AdapterExtension
    )
{
    PAHCI_ADAPTER_EXTENSION adapterExtension;
    PAHCI_CHANNEL_EXTENSION channelExtension;
    ULONG i, j;

    adapterExtension = (PAHCI_ADAPTER_EXTENSION)AdapterExtension;

    for (i = 0; i <= adapterExtension->HighestPort; i++) {
        if (adapterExtension->PortExtension[i] != NULL) {
            channelExtension = adapterExtension->PortExtension[i];
            // fire all callbacks 
            for (j = 0; j < ((ULONG)CallbackTypeMax - 1); j++) {
                if (channelExtension->TimerContext[j].CallBack != NULL) {

                    PAHCI_TIMER_CALLBACK callBack = channelExtension->TimerContext[j].CallBack;
                    PVOID                context = channelExtension->TimerContext[j].Context;
                    
                    //clear timer context area
                    channelExtension->TimerContext[j].CallBack = NULL;
                    channelExtension->TimerContext[j].Context = NULL;
                    channelExtension->TimerContext[j].Interval = 0;

                    //call the callback function
                    if ( (j == (CallbackTypeStartPort - 1)) && (callBack == (PVOID)P_Running) ) {
                        // a timer callback is about to run, the rest of Port Start process is not at DIRQL.
                        channelExtension->StartState.AtDIRQL = 0;
                    }
                    callBack(channelExtension, context);
                }
            }
        }
    }

    return;
}


BOOLEAN
AhciRequestTimerCallback (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PAHCI_TIMER_CALLBACK CallbackRoutine,
    __in_opt PVOID CallbackContext,
    __in ULONG Interval,
    __in TIMER_CALLBACK_TYPE CallbackType
    )
{
    if ( (CallbackType == CallbackTypeNone) || 
         (CallbackType >= CallbackTypeMax) ) {
        NT_ASSERT(FALSE); //programming error
        return FALSE;
    }

    if (ChannelExtension->TimerContext[(ULONG)CallbackType - 1].CallBack != NULL) {
        //NT_ASSERT(FALSE); //programming error
        return FALSE;
    } else {
        ChannelExtension->TimerContext[(ULONG)CallbackType - 1].CallBack = CallbackRoutine;
        ChannelExtension->TimerContext[(ULONG)CallbackType - 1].Context = CallbackContext;
        ChannelExtension->TimerContext[(ULONG)CallbackType - 1].Interval = Interval;
    }

    RegisterTimerCallback(ChannelExtension->AdapterExtension);

    return TRUE;
}




#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4054)
#pragma warning(default:4055)
#pragma warning(default:4214)
#pragma warning(default:4201)
#endif
