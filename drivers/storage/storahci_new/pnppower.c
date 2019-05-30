/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    pnppower.c

Abstract:

    This file contains function of pnp and power process of the AHCI miniport.  


Notes:

Revision History:

        Nathan Obr (natobr),  February 2005 - September 2006 rev 1 (NCQ, LPM, Hotplug, persistant state) 
                              December 2006 - August 2007    rev 2 (async)
        Michael Xing (xiaoxing),  December 2009 - initial Storport miniport version

--*/

#pragma warning(push)
#pragma warning(disable:26015) //26015: "Potential overflow using expression 'outParams->DriverStatus.bDriverError'. Buffer access is apparently unbounded by the buffer size.  
                               //Output buffer cannot be checked for size.  ATAport provides this validation check as the input buffer size and output buffer size are 2 of the 4 parameters passed in on the SMART IRP.  Storport doesn’t do this and the miniport doesn’t get the IRP so it cannot do this for itself.  This is just the condition of a legacy IOCTL.
                                //26015: "Potential overflow using expression 'nRB->NRBStatus' Buffer access is apparently unbounded by the buffer size.  
                                //The same is true for the NVCache IOCTL.  Instead of the output buffer, this time it is the NVCache_Request_Block.
#pragma warning(disable:26007) //26007: "Possibly incorrect single element annotation on buffer
                                //Concern over overflow comes from the NCS register which is a hardware register of 5 bits. The largest value it can physically hold is 31, even though PreFix thinks it can hold 255.
#pragma warning(disable:4214) // bit field types other than int
#pragma warning(disable:4201) // nameless struct/union

#include "generic.h"
#include "acpiioct.h"

BOOLEAN 
AhciPortInitialize(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
/*++
    This function is used to start an AHCI port
It assumes:

Called By:
    HwFindAdapter

Affected Variables/Registers:
    ChannelExtension->StateFlags.StartCapable
    ChannelExtension->CommandList
    ChannelExtension->ReceivedFIS

    PX.CLB,PX.CLBU
    PX.FB,PX.FBU
    CMD.ST
It performs:
    (overview)
    1. Start with some defensive structure checking and variable initialization
    2. Initialize the ChannelConfiguration structure (final steps)
    3. Enable the AHCI interface as per AHCI 1.1 section 10.1.2 (final steps)
    4. Allocate memory for the CommandList, the Receive FIS buffer, and SRB Extension.
    5. Start the channel processing commands.
    (details)
    1.1 Initialize variables
    1.2 Verify the Channel Configuration
    2.1 Initialize the ChannelConfiguration structure
    2.2 Initialize the Channel's base address and the controller's Interrupt Status register address
    3.1 Enable the AHCI interface
        AHCI 1.1 Section 10.1.2 - 5.    
        "For each implemented port, system software shall allocate memory for and program:
            •    PxCLB and PxCLBU (if CAP.S64A is set to ‘1’)
            •    PxFB and PxFBU (if CAP.S64A is set to ‘1’)
            It is good practice for system software to ‘zero-out’ the memory allocated and referenced by PxCLB and PxFB.  After setting PxFB and PxFBU to the physical address of the FIS receive area, system software shall set PxCMD.FRE to ‘1’."
    3.2 Enable Interrupts on the Channel
        AHCI 1.1 Section 10.1.2 - 7.    
        "Determine which events should cause an interrupt, and set each implemented port’s PxIE register with the appropriate enables."
        Note: Due to the multi-tiered nature of the AHCI HBA’s interrupt architecture, system software must always ensure that the PxIS (clear this first) and IS.IPS (clear this second) registers are cleared to ‘0’ before programming the PxIE and GHC.IE registers. This will prevent any residual bits set in these registers from causing an interrupt to be asserted.
  
    4.1 Allocate memory for the CommandList, the Receive FIS buffer and SRB Extension
        Now is the time to allocate memory that will be used for controller and per request structures.
        In AHCI, the controller structures are both the command header list and the received FIS buffer.
        The per request structure will be recieved through the SRB and will be used to make a Command Table
        The mechanism for requesting all of this memory is AtaPortGetUnCachedExtension.
        NOTE! AtaPortGetUnCachedExtension can only be called while processing a HwControl IdeStart.
        Also NOTE! In order to perform crashdump/hibernate the UncachedExtensionSize cannot be larger than 30K.

        The call to AtaPortGetUnCachedExtension is complicated by alignment restrictions that an AHCI controller has so here are the rules:
        - Command List Base Addresses must be 1K aligned, and the Command list is (sizeof (AHCI_COMMAND_HEADER) * cap.NCS), which is some multiple of 32 bytes in length.
        - The FIS Base Address must be 256 aligned, and the FIS Receive buffer is sizeof (AHCI_RECEIVED_FIS), 256 bytes in length.
        - The Command Table must be 128 aligned, and is sizeof(AHCI_COMMAND_TABLE), 1280 bytes in length thanks to some padding in the AHCI_COMMAND_TABLE structure.
        
        The alignment of the addresses (virtual and physical) returned by the function follow these rules
        - the address returned by AtaPortGetUnCachedExtension will have both its virtual and physical addresses page aligned
        - the memory received through the SRB will either be physically and virtually 4K aligned or SRBExtensionSize aligned. The first allocation will be on a 4K boundry the address of the second will be SRBExtensionSize larger than the first, the third will be SRBExtensionSize larger than the second, etc.
    
        Since the Command Header must be 1K aligned and the uncached extension starts 4K aligned, this works.
        However, the Command Header is variable and must be padded so the Received FIS is on a 256 boundry.
        Therefore the number of Command Headers must be 256/32 = 8. Round cap.NCS to the next multiple of 8
    4.2 Setup the CommandList 
        Although the pointer returned from AtaPortGetUnCachedExtension is useful to this driver, it does the controller no good and can't be used in CLB. The VIRTUAL address must be translated into the PHYSICAL address before being written to the CLB register as the controller doesn't have the CPU's virtual address translation tables.  AtaPortGetPhysicalAddress Returns the physical address for the given Va. The va has to be an offset into any one of the following buffers.
            - SRB's data buffer 
            - SRB's SrbExtension 
            - Miniport's uncached extension
    4.3 Setup the Receive FIS buffer 
        Handle the Receive FIS buffer the same as 4.2 Command List
    4.4 Setup the Local SRB Extension
    5.1 Enable Command Processing 
    5.2 Initialize the ChannelConfiguration structure
        ChannelConfiguration->ChannelNumber and    ChannelConfiguration->ChannelResources are kept default values. 
        If it is found that CI and/or SACT can be changed from a 1 to 0, Number of overlapped requests becomes 1.
        Number of overlapped requests is a 1 based number (1=1, 2=2, etc.), CAP.NCS is a 0 based number.   
    5.3 START COMMAND PROCESSING
Return Values:
    The miniport driver returns TRUE if it successfully exectute the whole function.
    Any errors causes the function to return FALSE and prevents the channel from loading. This ultimately causes a yellow '!' to show up on the channel in device manager.

NOTE: as this routine is invoked from FindAdapter where the adapter might not be fully initialized, do not retrieve registry information.
--*/
    PAHCI_ADAPTER_EXTENSION adapterExtension = NULL;
    PAHCI_MEMORY_REGISTERS  abar = NULL;

   //these are throw away variables
    ULONG   mappedLength = 0;

  //1.1 Initialize variables
    adapterExtension = ChannelExtension->AdapterExtension;
    if (LogExecuteFullDetail(adapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x00000024);//AhciPortInitialize
    }

    ChannelExtension->CurrentCommandSlot = 1; //slot 0 is reserved for internal commands,
    ChannelExtension->StateFlags.IgnoreHotplugDueToResetInProgress = TRUE;

    abar = (PAHCI_MEMORY_REGISTERS)adapterExtension->ABAR_Address;
    ChannelExtension->Px = &abar->PortList[ChannelExtension->PortNumber];
    
  // NonCachedExtension is for CommandList, Receive FIS, SRB Extension for Local SRB., READ_LOG/IDENTIFY buffer and GESN buffer
  // (sizeof(AHCI_COMMAND_HEADER) * paddedNCS) + sizeof(AHCI_RECEIVED_FIS) + sizeof(AHCI_SRB_EXTENSION) + sizeof(AHCI_READ_LOG_EXT_DATA) + 8; 

  //4.2 Setup the CommandList
    ChannelExtension->CommandListPhysicalAddress = StorPortGetPhysicalAddress(adapterExtension, NULL, (PVOID)ChannelExtension->CommandList, &mappedLength);
    if (ChannelExtension->CommandListPhysicalAddress.QuadPart == 0) {
        RecordExecutionHistory(ChannelExtension, 0xff02);//Command List Failed
        return FALSE;
    }
  //3.1.1 PxCLB and PxCLBU (AHCI 1.1 Section 10.1.2 - 5)
    if ( (ChannelExtension->CommandListPhysicalAddress.LowPart % 1024) == 0 ) {
        // validate the alignment is fine
        StorPortWriteRegisterUlong(adapterExtension, &ChannelExtension->Px->CLB.AsUlong, ChannelExtension->CommandListPhysicalAddress.LowPart);
    }else{ 
        RecordExecutionHistory(ChannelExtension, 0xff03);//Command List alignment failed
        return FALSE;
    }
    if (adapterExtension->CAP.S64A) { //If the controller supports 64 bits, write the high part too
        StorPortWriteRegisterUlong(adapterExtension, &ChannelExtension->Px->CLBU, ChannelExtension->CommandListPhysicalAddress.HighPart);
    }

  //4.3 Setup the Receive FIS buffer 
    ChannelExtension->ReceivedFisPhysicalAddress = StorPortGetPhysicalAddress(adapterExtension, NULL, (PVOID)ChannelExtension->ReceivedFIS, &mappedLength);
    if (ChannelExtension->ReceivedFisPhysicalAddress.QuadPart == 0) {
        RecordExecutionHistory(ChannelExtension, 0xff04);//Receive FIS failed
        return FALSE;
    }

  //3.1.2 PxFB and PxFBU (AHCI 1.1 Section 10.1.2 - 5)
    if ((ChannelExtension->ReceivedFisPhysicalAddress.LowPart % 256) == 0) {
        // validate the alignment is fine
        StorPortWriteRegisterUlong(adapterExtension, &ChannelExtension->Px->FB.AsUlong, ChannelExtension->ReceivedFisPhysicalAddress.LowPart);
    } else {
        RecordExecutionHistory(ChannelExtension, 0xff05);//Receive FIS alignment failed
        return FALSE;
    }
    if (adapterExtension->CAP.S64A) { //If the controller supports 64 bits, write the high part too
        StorPortWriteRegisterUlong(adapterExtension, &ChannelExtension->Px->FBU, ChannelExtension->ReceivedFisPhysicalAddress.HighPart);
    }

  //4.4 Setup the Local SRB Extension
    ChannelExtension->Local.SrbExtensionPhysicalAddress = StorPortGetPhysicalAddress(adapterExtension, NULL, (PVOID)ChannelExtension->Local.SrbExtension, &mappedLength);
    ChannelExtension->Sense.SrbExtensionPhysicalAddress = StorPortGetPhysicalAddress(adapterExtension, NULL, (PVOID)ChannelExtension->Sense.SrbExtension, &mappedLength);
    
  //4.5 Setup the TUR and GESN receive buffers.
    ChannelExtension->GesnCachedBufferPhysicalAddress = StorPortGetPhysicalAddress(adapterExtension, NULL, (PVOID)ChannelExtension->GesnCachedBuffer, &mappedLength);

  //4.6 Setup Device Identify Data and Inquiry Data buffers
    ChannelExtension->DeviceExtension[0].IdentifyDataPhysicalAddress = StorPortGetPhysicalAddress(adapterExtension, NULL, (PVOID)ChannelExtension->DeviceExtension[0].IdentifyDeviceData, &mappedLength);
    ChannelExtension->DeviceExtension[0].InquiryDataPhysicalAddress = StorPortGetPhysicalAddress(adapterExtension, NULL, (PVOID)ChannelExtension->DeviceExtension[0].InquiryData, &mappedLength);


  //4.8 Setup STOR_ADDRESS for the device. StorAHCI uses Bus/Target/Lun addressing model, thus uses STOR_ADDRESS_TYPE_BTL8.
  //        Port - not need to be set by miniport, Storport has this knowledge. miniport can get the value by calling StorPortGetSystemPortNumber().
  //        Path - StorAHCI reports (highest implemented port number + 1) as bus number, thus "port number" will be "Path" value.
  //        Target - StorAHCI only supports single device on each port, the "Target" value will be 0.
  //        Lun - StorAHCI only supports single device on each port, the "Lun" value will be 0.
    ChannelExtension->DeviceExtension[0].DeviceAddress.Type = STOR_ADDRESS_TYPE_BTL8;
    ChannelExtension->DeviceExtension[0].DeviceAddress.Port = 0;
    ChannelExtension->DeviceExtension[0].DeviceAddress.AddressLength = STOR_ADDR_BTL8_ADDRESS_LENGTH;
    ChannelExtension->DeviceExtension[0].DeviceAddress.Path = (UCHAR)ChannelExtension->PortNumber;
    ChannelExtension->DeviceExtension[0].DeviceAddress.Target = 0;
    ChannelExtension->DeviceExtension[0].DeviceAddress.Lun = 0;

  //3.2 Clear Enable Interrupts on the Channel (AHCI 1.1 Section 10.1.2 - 7)
  //We will enable interrupt after channel started
    StorPortWriteRegisterUlong(adapterExtension, &ChannelExtension->Px->IS.AsUlong, (ULONG)~0);
    StorPortWriteRegisterUlong(adapterExtension, adapterExtension->IS, (1 << ChannelExtension->PortNumber));

  //5.1 Enable Command Processing 
    ChannelExtension->StateFlags.StartCapable = TRUE;

    if (adapterExtension->CAP.NCS > 0) { //this leaves one emergency slot free if possible, as CAP.NCS is 0-based.
        ChannelExtension->MaxPortQueueDepth = (UCHAR)adapterExtension->CAP.NCS;   
    } else {
        ChannelExtension->MaxPortQueueDepth = 1;
    }

    if ( IsSingleIoDevice(adapterExtension) || IsDumpMode(adapterExtension) ) {
        ChannelExtension->MaxPortQueueDepth = 1;
    }

    ChannelExtension->LastActiveSlot = 0;
    ChannelExtension->DeviceExtension[0].DeviceParameters.MaxDeviceQueueDepth = ChannelExtension->MaxPortQueueDepth;

    if (!IsDumpMode(adapterExtension)) {
        if (AdapterResetInInit(adapterExtension)) {
            P_NotRunning(ChannelExtension, ChannelExtension->Px);
            AhciCOMRESET(ChannelExtension, ChannelExtension->Px);
        }
    }

    RecordExecutionHistory(ChannelExtension, 0x10000024);//Exit AhciPortInitialize
    return TRUE;
}


BOOLEAN 
AhciAdapterPowerUp(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
/*++
    Indicates that the adapter is being powered up.
    Anything that doesn't persist across a power cycle needs to be done here.  
It assumes:
    PCI Ensures the HBA is in D0 (Offset PMCAP + 4h: PMCS[0,1])
Called by:    
    AhciAdapterControl

It performs:
    Enables the AHCI Interface and global Interrupts
Affected Variables/Registers:
    GHC.AE, GHC.IE  
Return Values:
    TRUE always.
--*/                                    //Used to enable the AHCI interface
    AHCI_Global_HBA_CONTROL ghc;
    PAHCI_MEMORY_REGISTERS  abar = (PAHCI_MEMORY_REGISTERS)AdapterExtension->ABAR_Address;

    ghc.AsUlong = StorPortReadRegisterUlong(AdapterExtension, &abar->GHC.AsUlong);
    if (ghc.AE == 0) {
        ghc.AsUlong = 0;
        ghc.AE = 1;
        StorPortWriteRegisterUlong(AdapterExtension, &abar->GHC.AsUlong, ghc.AsUlong);
    }
    if (ghc.IE == 0) {
        ghc.IE = 1;
        StorPortWriteRegisterUlong(AdapterExtension, &abar->GHC.AsUlong, ghc.AsUlong);
    }

    return TRUE;
}

BOOLEAN 
AhciAdapterPowerDown(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
/*++
    Indicates that the adapter is being powered down.
It assumes:
    PCI powers down the HBA after this function returns: D3 Offset PMCAP + 4h: PMCS[0,1]  
Called by:    
    AhciAdapterControl

It performs:
    1. Clear GHC.IE
    AHCI 1.1 Section 8.3.3 
    "Software must disable interrupts (GHC.IE must be cleared to ‘0’) prior to requesting a transition of the HBA to the D3 state.  This precaution by software avoids an interrupt storm if an interrupt occurs during the transition to the D3 state."
Affected Variables/Registers:
    GHC.IE
Return Values:
    TRUE always.
--*/
    AHCI_Global_HBA_CONTROL ghc;
    PAHCI_MEMORY_REGISTERS  abar = (PAHCI_MEMORY_REGISTERS)AdapterExtension->ABAR_Address;

    ghc.AsUlong = StorPortReadRegisterUlong(AdapterExtension, &abar->GHC.AsUlong);
    ghc.IE = 0;
    StorPortWriteRegisterUlong(AdapterExtension, &abar->GHC.AsUlong, ghc.AsUlong);

    return TRUE;
}

VOID 
AhciPortStop(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
/*++

    The miniport driver should stop using the resources allocated for this port.
It assumes:
    AhciAdapterStop is called after all the ports are stopped.
Called by:    

It performs:
    (overview)
    1. Stop the channel
    2. Undefine all references to anything within the Uncached Extension
Affected Variables/Registers:
    CMD.ST, CMD.CR, CMD.FRE, CMD.FR

Return Values:
    TRUE if the function excecuted completely.
    FALSE if the channel could not be stopped.  
--*/

    if (LogExecuteFullDetail(ChannelExtension->AdapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x00000025);//AhciPortStop
    }
  //1. Stop the channel
    if ( !P_NotRunning(ChannelExtension, ChannelExtension->Px) ) {
        // don't need RESET, the port will be tried to start when processing start device request
        RecordExecutionHistory(ChannelExtension, 0xff08);   //AhciPortStop Failed
    }

  //2. Disable Interrupt and disconnect with Port resources

    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->IE.AsUlong, 0);    //disabling interrupts
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->IS.AsUlong, (ULONG)~0);  
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, ChannelExtension->AdapterExtension->IS, (1 << ChannelExtension->PortNumber));
  
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CLB.AsUlong, 0);
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CLBU, (ULONG)0);

    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->FB.AsUlong, 0);
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->FBU, 0);
  
    ChannelExtension->Px = 0;

    ChannelExtension->StateFlags.StartCapable = FALSE;
    ChannelExtension->StateFlags.NoMoreIO = FALSE;

    RecordExecutionHistory(ChannelExtension, 0x10000025);//Exit AhciPortStop

    return;
}

VOID 
AhciPortPowerUp(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
/*++
    Indicates that the channel is being powered up.

Called by:    
    AhciHwStartIo

It performs:
    (overview)
    1. Start the Port. 
    2. If APM is supported, make sure the Link is Active as defined in AHCI1.0 8.3.1.2.   
    3. If Port Multiplier is supported, powered it up.
    (details)
    1.1 Reload the CLB,CBU,FLB,FBU stored in the channel extension
    1.2 Reload PxIE
    1.3 Reinitialize the StateFlags
    1.4 Start the channel
Affected Variables/Registers:
    PxCMD.ST, PxCMD.ICC
    PxCLB,PxCLBU,PxFB,PxFBU
    PxIE
Return Values:
    none
--*/

    AHCI_LPM_POWER_SETTINGS userLpmSettings;
    BOOLEAN                 portStartCapable;

    RecordExecutionHistory(ChannelExtension, 0x00000026);//Enter AhciPortPowerUp 

  //1.0 Reinitialize the StateFlags. e.g. ChannelExtension->StateFlags.StartCapable = TRUE;
    portStartCapable = InterlockedBitTestAndSet((LONG*)&ChannelExtension->StateFlags, 0);    //StartCapable field is at bit 0

    if (portStartCapable == TRUE) {
        RecordExecutionHistory(ChannelExtension, 0x00010026);//AhciPortPowerUp: port already powered up.
        return;
    }

  //1.1 Reload the CLB,CBU,FLB,FBU
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CLB.AsUlong, ChannelExtension->CommandListPhysicalAddress.LowPart);
    if (ChannelExtension->AdapterExtension->CAP.S64A) { //If the controller supports 64 bits, write high part
        StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CLBU, ChannelExtension->CommandListPhysicalAddress.HighPart);
    }
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->FB.AsUlong, ChannelExtension->ReceivedFisPhysicalAddress.LowPart);
    if (ChannelExtension->AdapterExtension->CAP.S64A) { //If the controller supports 64 bits, write high part
        StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->FBU, ChannelExtension->ReceivedFisPhysicalAddress.HighPart);
    }

  //1.2 Clear existing interrupt as their is no command sent by driver yet. Reload PxIE
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->IS.AsUlong, (ULONG)~0);
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, ChannelExtension->AdapterExtension->IS, (1 << ChannelExtension->PortNumber));

  //1.4.1 Restore Perserved Settings
    if (NeedToSetTransferMode(ChannelExtension)) {
        RestorePreservedSettings(ChannelExtension, FALSE);
    }

  //1.4.2 Restore LPM settings in case of hardware loose it during power down.
    userLpmSettings.AsUlong = ChannelExtension->LastUserLpmPowerSetting;
    AhciLpmSettingsModes(ChannelExtension, userLpmSettings);    //ignore the returned value, IO will be restarted anyway.

  //1.5 Start the channel
    P_Running_StartAttempt(ChannelExtension, FALSE);

    RecordExecutionHistory(ChannelExtension, 0x10000026);//Exit AhciPortPowerUp
    return;
}

VOID 
AhciPortPowerDown(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
/*++
Indicates that the channel is being powered down.

It assumes:
    the device has been powered down through ATA commands
    All outstanding IO will be complete before the first power request is sent to the miniport
Called by:    
    AhciHwStartIo

It performs:
    Then each port must be stopped. PxCMD.ST
    If APM is supported, the Link need to be put into Slumber as defined in AHCI 1.1 Section 8.3.1.2
    If Port Multiplier is support, it would need to be powered down next.
Affected Variables/Registers:
    none
Return Values:
    TRUE if the function excecuted completely.
    FALSE if the channel could not be stopped for Power Down.  
    Neither return value is used by ATAport.
--*/
    ChannelExtension->StateFlags.StartCapable = FALSE;
    RecordExecutionHistory(ChannelExtension, 0x10000027);//Exit AhciPortPowerDown
}


VOID
ReportLunsComplete(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    )
{
    //Port start completed. Prepare device list.
    ULONG       lunCount;
    ULONG       lunLength;
    PLUN_LIST   lunList;
    ULONG       i;

    PAHCI_SRB_EXTENSION srbExtension;

    srbExtension = GetSrbExtension(Srb); 
    
    // clean up callback fields so that the SRB can be completed.
    srbExtension->AtaFunction = 0;
    srbExtension->CompletionRoutine = NULL;

    // report error back so that Storport may retry the command.
    // tolerate failure from IDE_COMMAND_READ_LOG_EXT during device enumeration as it's not part of device enumeration commands.
    if ( (srbExtension->TaskFile.Current.bCommandReg != IDE_COMMAND_READ_LOG_EXT) &&
         (Srb->SrbStatus != SRB_STATUS_PENDING) && 
         (Srb->SrbStatus != SRB_STATUS_SUCCESS) && 
         (Srb->SrbStatus != SRB_STATUS_NO_DEVICE) ) {
        return;
    }

    Srb->SrbStatus = SRB_STATUS_SUCCESS;
    Srb->ScsiStatus = SCSISTAT_GOOD;

    lunList = (PLUN_LIST)Srb->DataBuffer;

    if ( ChannelExtension->DeviceExtension->DeviceParameters.AtaDeviceType == DeviceNotExist ) {
        lunCount = 0;
    } else {
        //lunCount = ChannelExtension->DeviceExtension->DeviceParameters.MaximumLun + 1;
        lunCount = 1;
    }

    lunLength = lunCount * 8;

    if ( Srb->DataTransferLength < (sizeof(LUN_LIST) + lunLength) ) {
        Srb->SrbStatus = SRB_STATUS_DATA_OVERRUN;
        if (Srb->DataTransferLength >= sizeof(ULONG)) {
            //fill in required buffer size
            lunList->LunListLength[0] = (UCHAR)(lunLength >> (8*3));
            lunList->LunListLength[1] = (UCHAR)(lunLength >> (8*2));
            lunList->LunListLength[2] = (UCHAR)(lunLength >> (8*1));
            lunList->LunListLength[3] = (UCHAR)(lunLength >> (8*0));
        }
    } else {
        lunList->LunListLength[0] = (UCHAR)(lunLength >> (8*3));
        lunList->LunListLength[1] = (UCHAR)(lunLength >> (8*2));
        lunList->LunListLength[2] = (UCHAR)(lunLength >> (8*1));
        lunList->LunListLength[3] = (UCHAR)(lunLength >> (8*0));

        for (i = 0; i < lunCount; i++) {
            lunList->Lun[i][0] = 0;
            lunList->Lun[i][1] = (UCHAR)i;
            lunList->Lun[i][2] = 0;
            lunList->Lun[i][3] = 0;
            lunList->Lun[i][4] = 0;
            lunList->Lun[i][5] = 0;
            lunList->Lun[i][6] = 0;
            lunList->Lun[i][7] = 0;
        }
    }

    return;
}


VOID
AhciPortIdentifyDevice(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
  )
{
    PAHCI_SRB_EXTENSION srbExtension;

    srbExtension = GetSrbExtension(Srb);

    if (Srb->SrbStatus == SRB_STATUS_SUCCESS) {
        if (srbExtension->TaskFile.Current.bCommandReg == IDE_COMMAND_ATAPI_IDENTIFY) {
            ChannelExtension->DeviceExtension->DeviceParameters.AtaDeviceType = DeviceIsAtapi;
        } else {
            ChannelExtension->DeviceExtension->DeviceParameters.AtaDeviceType = DeviceIsAta;
        } 
        // identify completes, digest identify data / inquiry data
        UpdateDeviceParameters(ChannelExtension);

    } else if (Srb->SrbStatus == SRB_STATUS_NO_DEVICE) {
        // command failed consider as no device
        ChannelExtension->DeviceExtension->DeviceParameters.AtaDeviceType = DeviceNotExist;
    }

    // Identify Device can only be triggered from REPORT LUNS command or 
    //    INQUIRY command (for disk in dump environment)
    if (Srb->Cdb[0] == SCSIOP_REPORT_LUNS) {
        {
            ReportLunsComplete(ChannelExtension, Srb);
        }
    } else if (IsDumpMode(ChannelExtension->AdapterExtension) && (Srb->Cdb[0] == SCSIOP_INQUIRY)) {
        InquiryComplete(ChannelExtension, Srb);
    }
    return;
}
#if (NTDDI_VERSION > NTDDI_WIN7)
VOID
AhciPortNVCacheCompletion(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
  ) 
{
    PSRB_IO_CONTROL             srbControl;
    PNVCACHE_REQUEST_BLOCK      nRB;
    PATA_TASK_FILE              TaskFile;

    UNREFERENCED_PARAMETER(ChannelExtension);

    srbControl = (PSRB_IO_CONTROL)Srb->DataBuffer; 
    nRB = ((PNVCACHE_REQUEST_BLOCK) ( (PSRB_IO_CONTROL) srbControl + 1) );  //26015: "Potential overflow using expression 'nRB->NRBStatus' Buffer access is apparently unbounded by the buffer size.  

    // Return status sucess indicating that the request was handled by the device.
    nRB->NRBStatus = NRB_SUCCESS;

    TaskFile = Srb->SenseInfoBuffer;

    nRB->NVCacheStatus = TaskFile->Current.bCommandReg;
    if (TaskFile->Current.bCommandReg & 1) {  // command failed 
        nRB->NVCacheSubStatus = TaskFile->Current.bFeaturesReg;
    }

    nRB->Count = (TaskFile->Current.bSectorCountReg << 8) + 
                 (TaskFile->Previous.bSectorCountReg);

    nRB->LBA = (ULONGLONG) TaskFile->Previous.bCylHighReg;
    nRB->LBA <<= 8;
    nRB->LBA += (ULONGLONG) TaskFile->Previous.bCylLowReg;
    nRB->LBA <<= 8;
    nRB->LBA += (ULONGLONG) TaskFile->Previous.bSectorNumberReg;
    nRB->LBA <<= 8;
    nRB->LBA += (ULONGLONG) TaskFile->Current.bCylHighReg;
    nRB->LBA <<= 8;
    nRB->LBA += (ULONGLONG) TaskFile->Current.bCylLowReg;
    nRB->LBA <<= 8;
    nRB->LBA += (ULONGLONG) TaskFile->Current.bSectorNumberReg;

    return;
}
#endif

VOID
AhciPortSmartCompletion(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
  )
{
    PSENDCMDOUTPARAMS           outParams;
    PAHCI_SRB_EXTENSION         srbExtension;
    PUCHAR                      buffer;//to make the pointer arithmatic easier

    UNREFERENCED_PARAMETER(ChannelExtension);

    buffer       = (PUCHAR)Srb->DataBuffer + sizeof(SRB_IO_CONTROL);
    outParams    = (PSENDCMDOUTPARAMS) buffer;                          //26015: "Potential overflow using expression 'outParams->DriverStatus.bDriverError'  Buffer access is apparently unbounded by the buffer size.  
    srbExtension = GetSrbExtension(Srb);

    Srb->SenseInfoBuffer = NULL;
    Srb->SenseInfoBufferLength = 0;
    Srb->SrbStatus &= ~SRB_STATUS_AUTOSENSE_VALID;  // remove this flag as there is no data copy back to original Sense Buffer

    if (Srb->SrbStatus == SRB_STATUS_SUCCESS) {
        outParams->DriverStatus.bDriverError = 0;
        outParams->DriverStatus.bIDEError = 0;
       
    } else  {
        // command failed 
        outParams->DriverStatus.bDriverError = SMART_IDE_ERROR;
        outParams->DriverStatus.bIDEError = srbExtension->AtaStatus;
    }

    return;
}

__inline
VOID
BuildLocalCommand(
    __in PAHCI_CHANNEL_EXTENSION        ChannelExtension,
    __in PATA_TASK_FILE                 TaskFile,
    __in_opt PSRB_COMPLETION_ROUTINE    CompletionRountine
    )
/*++

It assumes:
    nothing
Called by:
    IssuePreservedSettingCommands

It performs:
    1 Fills in the local SRB with the ATA command

Affected Variables/Registers:
    none

--*/
{
    PSCSI_REQUEST_BLOCK srb;
    PAHCI_SRB_EXTENSION srbExtension;

  // do not touch field "srb->NextSrb". It should be only touched in queue related operations.
    srb = &ChannelExtension->Local.Srb;
    srb->SrbStatus = SRB_STATUS_PENDING;
    srb->SrbExtension = (PVOID)ChannelExtension->Local.SrbExtension;
    srb->TimeOutValue = 1;      //as it's sent by miniport, no one monitors the timeout value. 

  // Fills in the local SRB with the SetFeatures command
    srbExtension = ChannelExtension->Local.SrbExtension; 
    AhciZeroMemory((PCHAR)srbExtension, sizeof(AHCI_SRB_EXTENSION));
    
    srbExtension->AtaFunction = ATA_FUNCTION_ATA_COMMAND;
    srbExtension->CompletionRoutine = CompletionRountine;

    //setup TaskFile
    StorPortCopyMemory(&srbExtension->TaskFile, TaskFile, sizeof(ATA_TASK_FILE));

    return;
}


VOID
IssuePreservedSettingCommands(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in_opt PSCSI_REQUEST_BLOCK Srb
  )
/*++
    Uses the local SRB to send down the next Preserved Setting
It assumes:
    Local SRB is only used for restoring preserved settings
Called by:
    RestorePreservedSettings,
    IssueInitCommands,
    Itself indirectly through local SRB callback

It performs:
    1 Verify local SRB is not in use
    2 Find the next Preserved Setting
    3 Send it

Affected Variables/Registers:
    none
--*/
{
    UCHAR           i;
    ULONG           allocated;
    ATA_TASK_FILE   taskFile = {0};

    UNREFERENCED_PARAMETER(Srb);

  //1 Verify local SRB is not in use
    allocated = GetOccupiedSlots(ChannelExtension);

    if ((allocated & (1 << 0)) > 0) {
        // Already restoring preserved Settings
        return;
    }

  //2 find the next command to send
    for (i = 0; i < MAX_SETTINGS_PRESERVED; i++) {
        if ( (ChannelExtension->PersistentSettings.SlotsToSend & (1 << i)) > 0 ) {
            ChannelExtension->PersistentSettings.SlotsToSend &= ~(1 << i);
            break;
        }
    }

    //perhaps there is none.  Done.
    if ( i >= MAX_SETTINGS_PRESERVED) {
        InterlockedBitTestAndReset((LONG*)&ChannelExtension->StateFlags, 3);    //ReservedSlotInUse field is at bit 3
        return;
    }
 
  //3 Otherwise use the LocalSRB to send the command. When it is done, call this routine again
    taskFile.Current.bFeaturesReg = ChannelExtension->PersistentSettings.CommandParams[i].Features;
    taskFile.Current.bSectorCountReg = ChannelExtension->PersistentSettings.CommandParams[i].SectorCount;
    taskFile.Current.bDriveHeadReg = 0xA0;
    taskFile.Current.bCommandReg = IDE_COMMAND_SET_FEATURE;

    BuildLocalCommand(ChannelExtension, &taskFile, IssuePreservedSettingCommands);

    return;
}

VOID
IssueInitCommands(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in_opt PSCSI_REQUEST_BLOCK Srb
  )
/*++
    Uses the local SRB to send down the next Init Command or Preserved Setting Command
It assumes:
    Local SRB is only used for restoring preseved settings
Called by:
    AhciIssueInitCommands,
    Itself indirectly through local SRB callback

It performs:
    1 Verify local SRB is not in use
    2 Find the next Init Command or Preserved Setting Command
    3 Send it

Affected Variables/Registers:
    none
--*/
{
    ULONG           allocated;
    PATA_TASK_FILE  taskFile;

    UNREFERENCED_PARAMETER(Srb);

  //1 Verify local SRB is not in use
    allocated = GetOccupiedSlots(ChannelExtension);

    if ((allocated & (1 << 0)) > 0) {
        // Already restoring preserved Settings
        return;
    }

  //2 if all Init commands have been sent, send Preserved Setting Commands
    if (ChannelExtension->DeviceInitCommands.CommandToSend >= ChannelExtension->DeviceInitCommands.CommandCount) {
        ChannelExtension->PersistentSettings.SlotsToSend = ChannelExtension->PersistentSettings.Slots;
        IssuePreservedSettingCommands(ChannelExtension, NULL);
        return;
    }

  //2 find the next command to send
    taskFile = ChannelExtension->DeviceInitCommands.CommandTaskFile + ChannelExtension->DeviceInitCommands.CommandToSend;
    taskFile->Current.bDriveHeadReg = 0xA0;
 
    BuildLocalCommand(ChannelExtension, taskFile, IssueInitCommands);
    ChannelExtension->DeviceInitCommands.CommandToSend++;

    return;
}


VOID
SetDateAndTimeCompletion(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    )
{
    PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);

    UNREFERENCED_PARAMETER(ChannelExtension);

    AhciZeroMemory((PCHAR)srbExtension, sizeof(AHCI_SRB_EXTENSION));

    srbExtension->AtaFunction = ATA_FUNCTION_ATA_COMMAND;
    srbExtension->CompletionRoutine = NULL;
    SetCommandReg((&srbExtension->TaskFile.Current), IDE_COMMAND_STANDBY_IMMEDIATE);

    return;
}

VOID
BuildSetDateAndTimeTaskFile(
    __in PATA_TASK_FILE  TaskFile
)
{
    LARGE_INTEGER       temp;
    ULONGLONG           now;

    AhciZeroMemory((PCHAR)TaskFile, sizeof(ATA_TASK_FILE));

    //setup TaskFile
    StorPortQuerySystemTime(&temp);

    now = (ULONGLONG) temp.QuadPart; 
    now /= 10000;  //4 orders of magnatude

    // 2) subtract 369 years in seconds.
    //    Number of milliseconds in a Julian year = 31,557,600,000 (1millisecond * 1000second * 60minute * 60hour * 24day * 365.25year) 
    //    369 * 31,557,600,000  = 11,644,754,400,000 (0xA97 4173 1300)
    now -= 0xA9741731300;

    // Example 2010-09-29 10 am = 0x1cb5ffd`22c1bf5e 
    // 0x1cb5ffd`22c1bf5e/10000 = 0xbc2`8f496393 (12930255512467 or 12930255512467.8494) milliseconds
    // 0xbc2`8f496393 - 0xa97'41731300 = 0x012b`4dd65093 
    // NOTE: this number won't roll over for another ~8700 years.
        
    TaskFile->Current.bSectorNumberReg =     (UCHAR) (0xFF & now);
    now >>= 8;
    TaskFile->Current.bCylLowReg =           (UCHAR) (0xFF & now);
    now >>= 8;
    TaskFile->Current.bCylHighReg =          (UCHAR) (0xFF & now);
    now >>= 8;
    TaskFile->Previous.bSectorNumberReg =    (UCHAR) (0xFF & now);
    now >>= 8;
    TaskFile->Previous.bCylLowReg =          (UCHAR) (0xFF & now);
    now >>= 8;
    TaskFile->Previous.bCylHighReg =         (UCHAR) (0xFF & now);

    TaskFile->Current.bDriveHeadReg = 0xA0;
    TaskFile->Current.bCommandReg = IDE_COMMAND_SET_DATE_AND_TIME;

    return;
}

VOID
IssueSetDateAndTimeCommand(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in BOOLEAN SendStandBy
  )
/*++    
It assumes:
    nothing
Called by:
    AhciHwStartIo with SRB_FUNCTION_SHUTDOWN

It performs:
    1 Fills in the local SRB with the SetFeatures command
    2 Starts processing the command
Affected Variables/Registers:
    none

--*/
{
    ULONG               allocated;

    ATA_TASK_FILE       taskFile = {0};

  //1 Verify local SRB is not in use
    allocated = GetOccupiedSlots(ChannelExtension);

    if ((allocated & (1 << 0)) > 0) {
        NT_ASSERT(FALSE);
        return;
    }

    //setup TaskFile
    BuildSetDateAndTimeTaskFile(&taskFile);
    
    //setup Srb and SrbExtension
    BuildLocalCommand(ChannelExtension, &taskFile, (SendStandBy ? SetDateAndTimeCompletion : NULL));

    return;
}

BOOLEAN 
AhciDeviceInitialize (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
    STOR_LOCK_HANDLE lockhandle = {0};

    RecordExecutionHistory(ChannelExtension, 0x00000007);   //AhciDeviceInitialize

  //1 update preserved commands per device needs.
    if (IsAtaDevice(&ChannelExtension->DeviceExtension->DeviceParameters)) {

        if (NeedToSetTransferMode(ChannelExtension)) {
            //1.1 Set DMA mode to this device
            UpdateSetFeatureCommands(ChannelExtension, IDE_FEATURE_INVALID, IDE_FEATURE_SET_TRANSFER_MODE, 0, 0x44);
        }

      //1.2 Persist Write Cache
        UpdateSetFeatureCommands(ChannelExtension, IDE_FEATURE_INVALID, IDE_FEATURE_ENABLE_WRITE_CACHE, 0, 0);

    } else if (IsAtapiDevice(&ChannelExtension->DeviceExtension->DeviceParameters)) {
      //2.1 Persist SATA transfer mode for some SATAI/PATAPI bridge chips
        UpdateSetFeatureCommands(ChannelExtension, IDE_FEATURE_INVALID, IDE_FEATURE_SET_TRANSFER_MODE, 0, 0x42);

        if (IsDeviceSupportsAN(ChannelExtension->DeviceExtension->IdentifyPacketData)) {
        //2.2 Enable Asynchronous Notification if supported
            UpdateSetFeatureCommands(ChannelExtension, IDE_FEATURE_INVALID, IDE_FEATURE_ENABLE_SATA_FEATURE, 0, IDE_SATA_FEATURE_ASYNCHRONOUS_NOTIFICATION);
        }
    }
    
    AhciPortGetInitCommands(ChannelExtension);

  //5.1 Configure device with init commands and persistent configuration commands
    AhciPortIssueInitCommands(ChannelExtension);

    StorPortAcquireSpinLock(ChannelExtension->AdapterExtension, InterruptLock, NULL, &lockhandle);
    ActivateQueue(ChannelExtension, TRUE);
    StorPortReleaseSpinLock(ChannelExtension->AdapterExtension, &lockhandle);

    RecordExecutionHistory(ChannelExtension, 0x10000007);//Exit AhciDeviceInitialize
    return TRUE;
}

VOID 
AhciDeviceStart (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
/*
    Running at PASSIVE_LEVEL

    This function is called when IRP_MN_START_DEVICE is being processed.
    device registry access, ACPI calls can be processed in this function.
*/
{
    if (ChannelExtension == NULL) {
        return;
    }


    AhciDeviceInitialize(ChannelExtension);

    return;
}


__inline 
BOOLEAN
IsDeviceSupportsHIPM(
    __in PIDENTIFY_DEVICE_DATA IdentifyDeviceData
    )
{
    return (IdentifyDeviceData->SerialAtaCapabilities.HIPM == TRUE);
}

__inline 
BOOLEAN
IsDeviceSupportsDIPM(
    __in PIDENTIFY_DEVICE_DATA IdentifyDeviceData
    )
{
    return (IdentifyDeviceData->SerialAtaFeaturesSupported.DIPM == TRUE);
}


__inline 
BOOLEAN
IsLpmModeSetting(
    __in PSTOR_POWER_SETTING_INFO PowerInfo
    )
{
    if (PowerInfo->PowerSettingGuid.Data1 == 0x0b2d69d7) {
        if (PowerInfo->PowerSettingGuid.Data2 == 0xa2a1){
            if (PowerInfo->PowerSettingGuid.Data3 == 0x449c){
                if (PowerInfo->PowerSettingGuid.Data4[0] == 0x96){
                    if (PowerInfo->PowerSettingGuid.Data4[1] == 0x80){
                        if (PowerInfo->PowerSettingGuid.Data4[2] == 0xf9){
                            if (PowerInfo->PowerSettingGuid.Data4[3] == 0x1c){
                                if (PowerInfo->PowerSettingGuid.Data4[4] == 0x70){
                                    if (PowerInfo->PowerSettingGuid.Data4[5] == 0x52){
                                        if (PowerInfo->PowerSettingGuid.Data4[6] == 0x1c){
                                            if (PowerInfo->PowerSettingGuid.Data4[7] == 0x60){
                                                return TRUE;
                                            }  }  }  }  }  }  } }  }  }  }

    return FALSE;
}

__inline 
BOOLEAN
IsLpmAdaptiveSetting(
    __in PSTOR_POWER_SETTING_INFO PowerInfo
    )
{
    if (PowerInfo->PowerSettingGuid.Data1 == 0xDAB60367) {
        if (PowerInfo->PowerSettingGuid.Data2 == 0x53FE){
            if (PowerInfo->PowerSettingGuid.Data3 == 0x4fbc){
                if (PowerInfo->PowerSettingGuid.Data4[0] == 0x82){
                    if (PowerInfo->PowerSettingGuid.Data4[1] == 0x5E){
                        if (PowerInfo->PowerSettingGuid.Data4[2] == 0x52){
                            if (PowerInfo->PowerSettingGuid.Data4[3] == 0x1D){
                                if (PowerInfo->PowerSettingGuid.Data4[4] == 0x06){
                                    if (PowerInfo->PowerSettingGuid.Data4[5] == 0x9D){
                                        if (PowerInfo->PowerSettingGuid.Data4[6] == 0x24){
                                            if (PowerInfo->PowerSettingGuid.Data4[7] == 0x56){
                                                return TRUE;
                                            }  }  }  }  }  }  } }  }  }  }

    return FALSE;
}

UCHAR
SetAllowedLpmStates(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
   // Return Value: disabled modes; 
{
    UCHAR lpm;
    AHCI_SERIAL_ATA_CONTROL sctl;

    // 0h  No interface restrictions 
    // 1h  Transitions to the Partial state disabled 
    // 2h  Transitions to the Slumber state disabled 
    // 3h  Transitions to both Partial and Slumber states disabled 
    // disable LPM for eSATA port as hot-plug cannot be detected in partial or slumber state.
    if ((ChannelExtension->LastUserLpmPowerSetting == 0) ||
        IsExternalPort(ChannelExtension)) {
        lpm = 0x03; // slumber and partial disallowed
    } else {
        AHCI_COMMAND cmd;
        cmd.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CMD.AsUlong);

        if ((ChannelExtension->AutoPartialToSlumberInterval == 0) &&    // No software auto partial to slumber
           ( (ChannelExtension->AdapterExtension->CAP2.APST == 0) ||                     // device auto Partial to Slumber is not supported.
              (cmd.APSTE == 0) )) {                                     // device auto Partial to Slumber is not enabled.
            lpm = 0x02; // partial allowed; slumber disallowed
        } else {
            lpm = 0x00; // partial allowed; slumber allowed
        }

    }

    if (ChannelExtension->AdapterExtension->CAP.SSC == 0) {
        // disable Slumber if controller does not support it.
        lpm |= 0x02;
    }

    if (ChannelExtension->AdapterExtension->CAP.PSC == 0) {
        // msachi LPM is to put device into partial, then transit into slumber according to defined interval value.
        // do not enable LPM if partial is not supported. 
        // the case of device supporting slumber but not partial is very rare.
        lpm = 0x03;
    }

    //Set PxSCTL.IPM to 3h to restrict slumber and partial interface power management state transitions.
    sctl.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->SCTL.AsUlong);
    sctl.IPM = lpm; 
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->SCTL.AsUlong, sctl.AsUlong);

    return lpm; 
}

BOOLEAN
AhciLpmSettingsModes(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in AHCI_LPM_POWER_SETTINGS LpmMode
    )
/*
    NOTE: this routine may prepared command in Local.Srb. Caller of this routine should try to start IO process.

Return Value:
    TRUE: the caller should start IO process
*/
{
    AHCI_COMMAND cmd;
    BOOLEAN      needStartIo = FALSE;

    ChannelExtension->LastUserLpmPowerSetting = (UCHAR)LpmMode.AsUlong;
    
    if (LpmMode.AsUlong == 0) {
        // Active Mode.
        //Turn LPM off is Active is chosen
        cmd.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CMD.AsUlong);
        cmd.ALPE = 0;
        cmd.ASP = 0;
        StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CMD.AsUlong, cmd.AsUlong);

        //Set PxSCTL.IPM to 3h to restrict slumber and partial interface power management state transitions.
        SetAllowedLpmStates(ChannelExtension);

        // Disable DIPM
        if (IsDeviceSupportsDIPM(ChannelExtension->DeviceExtension[0].IdentifyDeviceData)) {
            //The enable/disable state for device initiated power management shall persist across software reset.
            //The enable/disable state shall be reset to its default disabled state upon COMRESET.
            UpdateSetFeatureCommands(ChannelExtension, 
                                    IDE_FEATURE_ENABLE_SATA_FEATURE,
                                    IDE_FEATURE_DISABLE_SATA_FEATURE,
                                    IDE_SATA_FEATURE_DEVICE_INITIATED_POWER_MANAGEMENT,
                                    IDE_SATA_FEATURE_DEVICE_INITIATED_POWER_MANAGEMENT);

            //Configure device with persistent configuration commands
            RestorePreservedSettings(ChannelExtension, FALSE);
            needStartIo = TRUE;
        }

    } else {
        // link power management is allowed.

        //Set PxSCTL.IPM for LPM allowed states.
        UCHAR sctlIpm = SetAllowedLpmStates(ChannelExtension);

        // Setting HIPM if it's enabled.
        if (LpmMode.HipmEnabled > 0) {
            // If Partial is capable and device supports HIPM.
            if ((sctlIpm != 0x03) &&
                IsDeviceSupportsHIPM(ChannelExtension->DeviceExtension[0].IdentifyDeviceData)) {

                // Turn on LPM and set it for Partial
                cmd.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CMD.AsUlong);
                cmd.ALPE = 1;
                cmd.ASP = 0; //0 = partial, 1 = slumber
                StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CMD.AsUlong, cmd.AsUlong);
            }
        }

        // Setting DIPM if it's enabled.
        if ((LpmMode.DipmEnabled > 0) &&
            (sctlIpm != 0x03)) {
            // Enable DIPM feature.
            if (IsDeviceSupportsDIPM(ChannelExtension->DeviceExtension[0].IdentifyDeviceData)) {

                //The enable/disable state for device initiated power management shall persist across software reset.
                //The enable/disable state shall be reset to its default disabled state upon COMRESET.
                UpdateSetFeatureCommands(ChannelExtension, 
                                        IDE_FEATURE_DISABLE_SATA_FEATURE,
                                        IDE_FEATURE_ENABLE_SATA_FEATURE,
                                        IDE_SATA_FEATURE_DEVICE_INITIATED_POWER_MANAGEMENT,
                                        IDE_SATA_FEATURE_DEVICE_INITIATED_POWER_MANAGEMENT);

                //Configure device with persistent configuration commands
                RestorePreservedSettings(ChannelExtension, FALSE);
                needStartIo = TRUE;
            }

        } else {
            // Disable DIPM feature.
            if (IsDeviceSupportsDIPM(ChannelExtension->DeviceExtension[0].IdentifyDeviceData)) {

                //The enable/disable state for device initiated power management shall persist across software reset.
                //The enable/disable state shall be reset to its default disabled state upon COMRESET.
                UpdateSetFeatureCommands(ChannelExtension,
                                        IDE_FEATURE_ENABLE_SATA_FEATURE,
                                        IDE_FEATURE_DISABLE_SATA_FEATURE,
                                        IDE_SATA_FEATURE_DEVICE_INITIATED_POWER_MANAGEMENT,
                                        IDE_SATA_FEATURE_DEVICE_INITIATED_POWER_MANAGEMENT);

                //Configure device with persistent configuration commands
                RestorePreservedSettings(ChannelExtension, FALSE);
                needStartIo = TRUE;
            }
        }

    }

    return needStartIo;
}


BOOLEAN
AhciPortPowerSettingNotification(
    IN PAHCI_CHANNEL_EXTENSION ChannelExtension,
    IN PSTOR_POWER_SETTING_INFO PowerInfo
    )
{
    //Make sure the Controller supports LPM, otherwise, don't touch anything.
    if (NoLpmSupport(ChannelExtension)) {
        return FALSE;
    }

    // do nothing if there is no device connected, or port already stopped
    if ( (ChannelExtension->StartState.ChannelNextStartState == StartFailed) ||
         (ChannelExtension->StartState.ChannelNextStartState == Stopped) ||
         (ChannelExtension->DeviceExtension->DeviceParameters.AtaDeviceType == DeviceNotExist) ) {
        return FALSE;
    }

    // Validate input LPM data from the Power Manager
    if (PowerInfo->ValueLength != sizeof(ULONG)) {
        return FALSE;
    }

    if (!IsLpmModeSetting(PowerInfo) && 
        !IsLpmAdaptiveSetting(PowerInfo)) {
        // invalid power policy.
        return FALSE;
    }

    if (IsLpmAdaptiveSetting(PowerInfo)) {
        // max allowed value: 5 minutes (in ms)
        ULONG interval = (ULONG)*((PULONG)PowerInfo->Value);

        if (interval <= 300000) {
            ChannelExtension->AutoPartialToSlumberInterval = interval;

            //Set PxSCTL.IPM register for LPM allowed states.
            SetAllowedLpmStates(ChannelExtension);
        }

    } else if (IsLpmModeSetting(PowerInfo)) {
        BOOLEAN                 needRestartIo;
        AHCI_LPM_POWER_SETTINGS userLpmPowerSettings;

        userLpmPowerSettings.AsUlong = (ULONG)*((PULONG)PowerInfo->Value);

        needRestartIo = AhciLpmSettingsModes(ChannelExtension, userLpmPowerSettings);

        if (needRestartIo) {
            STOR_LOCK_HANDLE lockhandle = {0};
            StorPortAcquireSpinLock(ChannelExtension->AdapterExtension, InterruptLock, NULL, &lockhandle);
            ActivateQueue(ChannelExtension, TRUE);
            StorPortReleaseSpinLock(ChannelExtension->AdapterExtension, &lockhandle);
        }
    }

    return TRUE;
}

BOOLEAN
AhciAutoPartialToSlumber(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in_opt PVOID Context
)
/*
    NOTE: input parameter - Context is required as this is a callback function. But it's not used by this function.
*/
{
    AHCI_SERIAL_ATA_STATUS ssts;
    AHCI_COMMAND cmd;

    UNREFERENCED_PARAMETER(Context);

    cmd.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CMD.AsUlong);

    // 1.1 check the Link Power State should be enabled.
    if (NoLpmSupport(ChannelExtension) ||       // not support Aggressive LPM
        (cmd.ALPE == 0)) {                      // Aggressive LPM is disabled
        return FALSE;
    }

    // 1.2 check if the device is able to automatically go to Slumber from Partial.
    if ((ChannelExtension->AdapterExtension->CAP2.APST == 1) &&   // device auto Partial to Slumber is supported.
        (cmd.APSTE == 1)) {                     // device auto Partial to Slumber is enabled.
        return FALSE;
    }

    ssts.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->SSTS.AsUlong);

    // 1.3 check the Link Power State, should be Partial (value 2).
    if (ssts.IPM != 2) {
        return FALSE;
    }

    // 2. Change LPM State.
    // link should be in idle state.
    if (cmd.ICC == 0) {
        // 2.1 set into Active state.
        cmd.ICC = 1;
        StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CMD.AsUlong, cmd.AsUlong);

        // 2.2 set into Slumber state.
        cmd.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CMD.AsUlong);
        if (cmd.ICC != 0) {
            //by spec, partial to active transition should be completed in 10us. Reading register already takes sometime.
            StorPortStallExecution(10);  //10 microseconds
        }

        //try to bring it into slumber state.
        cmd.ICC = 6;
        StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CMD.AsUlong, cmd.AsUlong);
    }

    return TRUE;
}

BOOLEAN 
AhciAdapterPowerSettingNotification(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension,
    __in PSTOR_POWER_SETTING_INFO PowerSettingInfo
    )
{
    ULONG i;
    for (i = 0; i <= AdapterExtension->HighestPort; i++) {
        if (AdapterExtension->PortExtension[i] != NULL) {
            AhciPortPowerSettingNotification(AdapterExtension->PortExtension[i], PowerSettingInfo);
        }
    }

    return TRUE;
}


#define ACPI_METHOD_GTF   ((ULONG) 'FTG_') // _GTF
#define ACPI_METHOD_RMV   ((ULONG) 'VMR_') // _RMV
#define ACPI_METHOD_STA   ((ULONG) 'ATS_') // _STA
#define ACPI_METHOD_SDD   ((ULONG) 'DDS_') // _SDD

VOID
AhciPortGetInitCommands(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
  )
{
    // Read _GTF from ACPI
    ULONG                    status = STOR_STATUS_SUCCESS;
    ACPI_EVAL_INPUT_BUFFER   inputData = {0};
    PACPI_EVAL_OUTPUT_BUFFER acpiData = NULL;
    PACPI_METHOD_ARGUMENT    argument = NULL;
    ULONG                    acpiDataSize = 256;     // initial size, should be good enough for most cases
#if (NTDDI_VERSION > NTDDI_WIN7)    
	ULONG                    returnedLength = 0;
#endif
    UCHAR                    gtfCommandCount = 0;

    // clear Init Commands area. need to do this for device removed previously (StorAHCI only knows about adapter removal, not device removal)
    ChannelExtension->DeviceInitCommands.CommandCount = 0;
    ChannelExtension->DeviceInitCommands.CommandToSend = 0;
    if (ChannelExtension->DeviceInitCommands.CommandTaskFile != NULL) {
        StorPortFreePool(ChannelExtension->AdapterExtension, (PVOID)ChannelExtension->DeviceInitCommands.CommandTaskFile);
        ChannelExtension->DeviceInitCommands.CommandTaskFile = NULL;
    }


    inputData.Signature = ACPI_EVAL_INPUT_BUFFER_SIGNATURE;
    inputData.MethodNameAsUlong = ACPI_METHOD_GTF;

    status = StorPortAllocatePool(ChannelExtension->AdapterExtension,
                                  acpiDataSize,            
                                  AHCI_POOL_TAG,
                                  (PVOID*)&acpiData);

#if (NTDDI_VERSION > NTDDI_WIN7)
    if (acpiData != NULL) {
        // call API to get required buffer size
        status = StorPortInvokeAcpiMethod(ChannelExtension->AdapterExtension, 
                                          (PSTOR_ADDRESS)&ChannelExtension->DeviceExtension[0].DeviceAddress,
                                          ACPI_METHOD_GTF, 
                                          &inputData, 
                                          sizeof(ACPI_EVAL_INPUT_BUFFER), 
                                          (PVOID)acpiData, 
                                          acpiDataSize,
                                          &returnedLength
                                          );
        
        // in case of the alloate buffer is too small, re-allocate buffer and retry the call
        if ( (status == STOR_STATUS_BUFFER_TOO_SMALL) && (acpiData->Length > acpiDataSize) ) {
            acpiDataSize = acpiData->Length;
            StorPortFreePool(ChannelExtension->AdapterExtension, (PVOID)acpiData);
            acpiData = NULL;
            // re-allocate a bigger buffer
            status = StorPortAllocatePool(ChannelExtension->AdapterExtension,
                                          acpiDataSize,            
                                          AHCI_POOL_TAG,
                                          (PVOID*)&acpiData);

            if (acpiData != NULL) {
                status = StorPortInvokeAcpiMethod(ChannelExtension->AdapterExtension, 
                                                  (PSTOR_ADDRESS)&ChannelExtension->DeviceExtension[0].DeviceAddress,
                                                  ACPI_METHOD_GTF, 
                                                  &inputData, 
                                                  sizeof(ACPI_EVAL_INPUT_BUFFER), 
                                                  (PVOID)acpiData, 
                                                  acpiDataSize,
                                                  &returnedLength
                                                  );
            }
        }
    }
#endif	

    // get _GTF commands count
    if ( (status == STOR_STATUS_SUCCESS) && 
         (acpiData != NULL) &&
         (acpiData->Signature == ACPI_EVAL_OUTPUT_BUFFER_SIGNATURE) &&
         (acpiData->Count == 1) ) {

        argument = acpiData->Argument;

        if (argument->Type == ACPI_METHOD_ARGUMENT_BUFFER) {
            NT_ASSERT ((argument->DataLength % sizeof(ACPI_GTF_IDE_REGISTERS)) == 0);
            gtfCommandCount = (UCHAR)(argument->DataLength / sizeof(ACPI_GTF_IDE_REGISTERS));
        } else {
            NT_ASSERT(argument->Type == ACPI_METHOD_ARGUMENT_BUFFER);
        }
    }

    // Get Init Command count for devices
    if (IsAtaDevice(&ChannelExtension->DeviceExtension[0].DeviceParameters)) {
#if (NTDDI_VERSION > NTDDI_WIN7)		
        if (ChannelExtension->DeviceExtension[0].IdentifyDeviceData->CommandSetSupport.SetTimeStamp == 0x1) {
            ChannelExtension->DeviceInitCommands.CommandCount = gtfCommandCount + 3;
        } else
#endif			
        ChannelExtension->DeviceInitCommands.CommandCount = gtfCommandCount + 2;
    } else {
        ChannelExtension->DeviceInitCommands.CommandCount = gtfCommandCount + 1;
    }

    // copy _GTF commands into buffer
    if (ChannelExtension->DeviceInitCommands.CommandCount > 0) {
        PATA_TASK_FILE taskFile;
        ULONG i;

        status = StorPortAllocatePool(ChannelExtension->AdapterExtension,
                                      ChannelExtension->DeviceInitCommands.CommandCount * sizeof(ATA_TASK_FILE),            
                                      AHCI_POOL_TAG,
                                      (PVOID*)&ChannelExtension->DeviceInitCommands.CommandTaskFile);

        if ( (status != STOR_STATUS_SUCCESS) || (ChannelExtension->DeviceInitCommands.CommandTaskFile == NULL) ) {
            goto exit;
        }
        
        AhciZeroMemory((PCHAR)ChannelExtension->DeviceInitCommands.CommandTaskFile, ChannelExtension->DeviceInitCommands.CommandCount * sizeof(ATA_TASK_FILE));

        for (i=0; i < gtfCommandCount; i++) {

            StorPortCopyMemory(ChannelExtension->DeviceInitCommands.CommandTaskFile + i,
                               argument->Data + (i * sizeof(ACPI_GTF_IDE_REGISTERS)),
                               sizeof(ACPI_GTF_IDE_REGISTERS)
                               );
        }

        // add IDE_FEATURE_DISABLE_REVERT_TO_POWER_ON for all devices
        if ((ChannelExtension->DeviceInitCommands.CommandCount - i) >= 1) {
            taskFile = ChannelExtension->DeviceInitCommands.CommandTaskFile + i;
            taskFile->Current.bFeaturesReg = IDE_FEATURE_DISABLE_REVERT_TO_POWER_ON;
            taskFile->Current.bCommandReg = IDE_COMMAND_SET_FEATURE;
            i++;
        }

        // add more commands for ATA devices
        if (IsAtaDevice(&ChannelExtension->DeviceExtension[0].DeviceParameters)) {
            if ((ChannelExtension->DeviceInitCommands.CommandCount - i) >= 1) {
                taskFile = ChannelExtension->DeviceInitCommands.CommandTaskFile + i;
                taskFile->Current.bCommandReg = IDE_COMMAND_SECURITY_FREEZE_LOCK;
                i++;
            }
#if (NTDDI_VERSION > NTDDI_WIN7)			
            if ((ChannelExtension->DeviceInitCommands.CommandCount - i) >= 1) {
                if (ChannelExtension->DeviceExtension[0].IdentifyDeviceData->CommandSetSupport.SetTimeStamp == 0x1) {
                    taskFile = ChannelExtension->DeviceInitCommands.CommandTaskFile + i;
                    //setup TaskFile
                    BuildSetDateAndTimeTaskFile(taskFile);
                    i++;
                }
            }
#endif			
        }
    }

exit:
    if (acpiData != NULL) {
        StorPortFreePool(ChannelExtension->AdapterExtension, (PVOID)acpiData);
        acpiData = NULL;
    }

    return;
}


#pragma warning(pop) // un-sets any local warning changes
