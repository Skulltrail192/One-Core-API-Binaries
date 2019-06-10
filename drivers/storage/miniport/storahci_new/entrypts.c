/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    entrypts.c

Abstract:

    This file contains function of entry points to the AHCI miniport.


Notes:

Revision History:

        Michael Xing (xiaoxing),  December 2009 - initial Storport miniport version

--*/

#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning(disable:4152)   // nonstandard extension, function/data pointer conversion in expression
#pragma warning(disable:4214) // bit field types other than int
#pragma warning(disable:4201) // nameless struct/union


#include "generic.h"


// storahci.sys global variables
PVOID   g_AdapterExtension[4] = {0};
UCHAR   g_AdapterExtensionIndex = 0;

BOOLEAN
AllocateResourcesForAdapter(
    __in PAHCI_ADAPTER_EXTENSION         AdapterExtension,
    __in PPORT_CONFIGURATION_INFORMATION ConfigInfo,
    __in ULONG                           PortCount
    )
/*
    Internal function to allocate required memory for Ports

    In AHCI, the controller structures are both the command header list and the received FIS buffer.
        The mechanism for requesting all of this memory is StorPortGetUncachedExtension. This function returns page-aligned address.
        NOTE! StorPortGetUncachedExtension can only be called while in FindAdapter routine.
        NOTE! In order to perform crashdump/hibernate the UncachedExtensionSize should not be larger than 30K.

    Alignment requirement:
    -   Command List Base Addresses: must be 1K aligned, and the Command list is (sizeof (AHCI_COMMAND_HEADER) * cap.NCS), which is some multiple of 32 bytes in length.
    -   The FIS Base Address: must be 256 aligned, and the FIS Receive buffer is sizeof (AHCI_RECEIVED_FIS), 256 bytes in length.
        The Command Table: must be 128 aligned, and is sizeof(AHCI_COMMAND_TABLE), 1280 bytes in length thanks to some padding in the AHCI_COMMAND_TABLE structure.
                           Commmand Table is per request, resides in AHCI_SRB_EXTENSION for each request.

        Size of nonCachedExtensionSize for every channel should round up to KiloBytes, so that the pointer for next channel is starting at 1K boundary.

        the Command Header is variable and must be padded so the Received FIS is on a 256 boundry.
        Therefore the number of Command Headers must be 256/32 = 8. Round cap.NCS to the next multiple of 8
*/
{
    ULONG paddedNCS = 0;
    ULONG paddedSrbExtensionSize = 0;
    ULONG nonCachedExtensionSize = 0;
    PVOID portsChannelExtension = NULL;
    PCHAR portsUncachedExtension = NULL;
    ULONG i = 0;
    ULONG j = 0;

    // 2.1 allocate nonCachedExtension for Hardware access

    // Allocate Identify Data buffer, CommandList, Receive FIS, SRB Extension for Local SRB, INQUIRY buffer, and GESN buffer
    // paddedNCS is (CAP.NCS+1) rounds up to the next multiple of 8.
    paddedNCS = ((AdapterExtension->CAP.NCS) / 8 + 1) * 8;

    // SrbExtension needs to align to 128 bytes, pad the size to be multiple of 128 bytes
    paddedSrbExtensionSize = ((sizeof(AHCI_SRB_EXTENSION) - 1) / 128 + 1) * 128;

    // size per Port
    nonCachedExtensionSize = sizeof(AHCI_COMMAND_HEADER) * paddedNCS +      // align to 1K, padded to multiple of 256 bytes. (nocachedextension is page aligned)
                             sizeof(AHCI_RECEIVED_FIS) +                    // align to 256 bytes.
                             paddedSrbExtensionSize * 2 +                   // align to 128 bytes. Local.SrbExtension and SenseSrbExtension
                             sizeof(IDENTIFY_DEVICE_DATA) +                 // 512 bytes
                             INQUIRYDATABUFFERSIZE + 8;                     // Inquiry Data and GESN buffer
    // round up to KiloBytes if it's not dump mode. this makes sure that NonCachedExtension for next port can align to 1K.
    //if (!IsDumpMode(AdapterExtension)) {
    nonCachedExtensionSize = ((nonCachedExtensionSize - 1) / 0x400 + 1) * 0x400;
    // total size for all ports
    nonCachedExtensionSize *= PortCount;

    AdapterExtension->NonCachedExtension = StorPortGetUncachedExtension(AdapterExtension, ConfigInfo, nonCachedExtensionSize);

    if (AdapterExtension->NonCachedExtension == NULL) {
       // we cannot continue if cannot get nonCachedMemory for Channels.
        return FALSE;
    }

    AhciZeroMemory((PCHAR)AdapterExtension->NonCachedExtension, nonCachedExtensionSize);

    // 2.2 allocate resources for Ports which need AHCI_CHANNEL_EXTENSION for each of them.
    //if (!IsDumpMode(AdapterExtension)) {
	// 2.2 allocate resources for Ports which need AHCI_CHANNEL_EXTENSION for each of them.
		portsChannelExtension = ExAllocatePoolWithTag(NonPagedPool,
												  PortCount * sizeof(AHCI_CHANNEL_EXTENSION),
												  AHCI_POOL_TAG);

        if ((portsChannelExtension == NULL)) {
           // we cannot continue if cannot get memory for ChannelExtension.
            return FALSE;
        }
        AhciZeroMemory((PCHAR)portsChannelExtension, PortCount * sizeof(AHCI_CHANNEL_EXTENSION));
        //get the starting pointer
    portsUncachedExtension = (PCHAR)AdapterExtension->NonCachedExtension;

    // reset nonCachedExtensionSize to be the size for one Port, it works for dump case also as PortCount is '1'.
    nonCachedExtensionSize /= PortCount;

    // assign allocated memory and uncachedExension into ChannelExtensions
    for (i = 0; i <= AdapterExtension->HighestPort; i++) {
        if ( (AdapterExtension->PortImplemented & (1 << i)) != 0 ) {
            // this port is implemented, allocate and initialize extension for the port
            AdapterExtension->PortExtension[i] = (PAHCI_CHANNEL_EXTENSION)((PCHAR)portsChannelExtension + sizeof(AHCI_CHANNEL_EXTENSION) * j);
            AdapterExtension->PortExtension[i]->AdapterExtension = AdapterExtension;
            AdapterExtension->PortExtension[i]->PortNumber = i;
            // set ChannelExtension fields that use NonCachedExtension
            AdapterExtension->PortExtension[i]->CommandList = (PAHCI_COMMAND_HEADER)(portsUncachedExtension + nonCachedExtensionSize * j);
            AdapterExtension->PortExtension[i]->ReceivedFIS = (PAHCI_RECEIVED_FIS)((PCHAR)AdapterExtension->PortExtension[i]->CommandList + sizeof(AHCI_COMMAND_HEADER) * paddedNCS);
            AdapterExtension->PortExtension[i]->Local.SrbExtension = (PAHCI_SRB_EXTENSION)((PCHAR)AdapterExtension->PortExtension[i]->ReceivedFIS + sizeof(AHCI_RECEIVED_FIS));
            AdapterExtension->PortExtension[i]->Sense.SrbExtension = (PAHCI_SRB_EXTENSION)((PCHAR)AdapterExtension->PortExtension[i]->Local.SrbExtension + paddedSrbExtensionSize);
            AdapterExtension->PortExtension[i]->DeviceExtension[0].IdentifyDeviceData = (PIDENTIFY_DEVICE_DATA)((PCHAR)AdapterExtension->PortExtension[i]->Sense.SrbExtension + paddedSrbExtensionSize);
            AdapterExtension->PortExtension[i]->GesnCachedBuffer = (PUCHAR)((PCHAR)AdapterExtension->PortExtension[i]->DeviceExtension[0].IdentifyDeviceData + sizeof(IDENTIFY_DEVICE_DATA));
            AdapterExtension->PortExtension[i]->DeviceExtension[0].InquiryData = (PUCHAR)((PUCHAR)AdapterExtension->PortExtension[i]->GesnCachedBuffer + 8);
            // 
            j++;
        }
    }

    return TRUE;
}

ULONG
AhciHwFindAdapter(
    IN PVOID AdapterExtension,
    IN PVOID HwContext,
    IN PVOID BusInformation,
    IN PCHAR ArgumentString,
    IN OUT PPORT_CONFIGURATION_INFORMATION ConfigInfo,
    OUT PBOOLEAN Again
    )
/*++
    This function is called by the Storport driver indirectly when handling an IRP_MJ_PnP, IRP_MN_START_DEVICE.
    The adapter is being started. This function is called at PASSIVE LEVEL.
It assumes:
    Resources for the AHCI controller exist in a single Memory Mapped range.
    No Channels have been initialized or started; no IO is outstanding.
Called by:
    Storport

It performs:
    (overview)
    1. Check the working mode and retrieve adapter vendor/model info.
    2. Initialize ChannelExtension with Identifying memory mapped IO resources, Version and CAP.
    3. Enabling the AHCI interface as per AHCI 1.1 section 10.1.2 (part one)
    4. Initializing the IDE_CONTROLLER_CONFIGURATION structure with data from the AHCI Interface
    (details)
    1.1 Get dump mode
    1.2 Gather Vendor,Device,Revision IDs from PCI
    2.1 Initialize adapterExtension with AHCI abar
    2.2 Initialize adapterExtension with version & cap values
    3.1 Turn on AE, reset the controller if AE is already set
        AHCI 1.1 Section 10.1.2 - 1.
        "Indicate that system software is AHCI aware by setting GHC.AE to ‘1’."
    3.2 Determine which ports are implemented by the HBA
        AHCI 1.1 Section 10.1.2 - 2.
        "Determine which ports are implemented by the HBA, by reading the PI register. This bitmap value will aid software in determining how many ports are available and which port registers need to be initialized."
    3.3 get biggest port number
    3.4 Initializing the rest of PORT_CONFIGURATION_INFORMATION
    3.5 Register Power Setting Change Notification Guids
    4.1 Turn on IE, pending interrupts will be cleared when port starts
        This has to be done after 3.2 because we need to know the number of channels before we check each PxIS.
        Verify that none of the PxIS registers are loaded, but take no action
        Note: Due to the multi-tiered nature of the AHCI HBA’s interrupt architecture, system software must always ensure that the PxIS (clear this first) and IS.IPS (clear this second) registers are cleared to ‘0’ before programming the PxIE and GHC.IE registers. This will prevent any residual bits set in these registers from causing an interrupt to be asserted.
        However, the interrupt handler hasn't been hooked up by StorPort yet, so no interrupts will be handled by software until that happens.
    4.2 Allocate resources for both DMA use and all Channel/Port extensions.
    4.3 Initialize ports and start them. Dump stack will do this when receiving the INQUIRY command

Affected Variables/Registers:
    AdapterExtension->ABAR_Address
    AdapterExtension->CAP
    AdapterExtension->CAP2
    AdapterExtension->Version

    GHC.AE, GHC.IE, GHC.HR
    IS and all fields in the HBA’s register memory space except PxFB/PxFBU/PxCLB/PxCLBU that are not HwInit

Return Values:
    The miniport driver returns TRUE if it successfully exectute the whole function.
    Any errors causes the driver to return FALSE and prevents the driver from loading.

Note:
    HwStorFindAdapter must set the MaximumTransferLength and NumberOfPhysicalBreaks fields in the ConfigInfo structure.

    Other than these fields, the PORT_CONFIGURATION_INFORMATION (Storport) structure will always fully specify all adapter resources
    that are required to start the adapter.

--*/
{
    PAHCI_ADAPTER_EXTENSION adapterExtension = NULL;
    PAHCI_MEMORY_REGISTERS  abar = NULL;
    //Used to find the number of channels implemented
    UCHAR                   i = 0;
    ULONG                   piMask = 0;
    UCHAR                   numberOfHighestPort = 0;
    ULONG                   portCount = 0;
    //Used to enable the AHCI interface
    AHCI_Global_HBA_CONTROL ghc = {0};
    ULONG pcicfgLen = 0;
    UCHAR pcicfgBuffer[0x30] = {0};	
#if (NTDDI_VERSION > NTDDI_WIN7)	
    //guids
    GUID                    powerSettingChangeGuids[2] = {0};
    PAHCI_DUMP_CONTEXT      dumpContext = (PAHCI_DUMP_CONTEXT)ConfigInfo->MiniportDumpData;
#endif
    UNREFERENCED_PARAMETER(HwContext);
    UNREFERENCED_PARAMETER(BusInformation);
    UNREFERENCED_PARAMETER(Again);
    UNREFERENCED_PARAMETER(ArgumentString);

    adapterExtension = (PAHCI_ADAPTER_EXTENSION)AdapterExtension;

    // "StoppedState == 0" indicates it's the normal HwFindAdapter call; 
    // "StoppedState == 1" indicates the adapter is restarting.
    if (adapterExtension->StateFlags.StoppedState == 0) {
        // use for debugging, assume no more than 4 AHCI adapters in system
        g_AdapterExtension[g_AdapterExtensionIndex++] = AdapterExtension;
        g_AdapterExtensionIndex %= 4;
    }

    //adapterExtension->AdapterNumber = ConfigInfo->SlotNumber;
    adapterExtension->SystemIoBusNumber = ConfigInfo->SystemIoBusNumber;
    adapterExtension->SlotNumber = ConfigInfo->SlotNumber;

  //1.1 Get dump mode
#if (NTDDI_VERSION > NTDDI_WIN7)  
    if (ArgumentString != NULL) {
        // check if doing crash dump
        BOOLEAN dumpMode = CompareId((PSTR)ArgumentString, GetStringLength(ArgumentString, 20), (PSTR)"dump", 4, NULL);
        if (dumpMode == TRUE) {
            adapterExtension->StateFlags.DumpMode = 1;
            if (dumpContext == NULL) {
                NT_ASSERT(FALSE);
                return SP_RETURN_ERROR;
            } else {
                // In dump/hibernation mode, need to mark ConfigInfo->MiniportDumpData and any embedded memory buffer(s) in MiniportDumpData
                StorPortMarkDumpMemory(AdapterExtension, dumpContext, sizeof(AHCI_DUMP_CONTEXT), 0);
            }
        }
    }
#endif
  //1.2 Gather Vendor,Device,Revision IDs from PCI
    //if (!IsDumpMode(adapterExtension)) {


        pcicfgLen = StorPortGetBusData(adapterExtension,
                                       PCIConfiguration,
                                       ConfigInfo->SystemIoBusNumber,
                                       (ULONG)ConfigInfo->SlotNumber,
                                       (PVOID)pcicfgBuffer,
                                       (ULONG)0x30);
        if (pcicfgLen == 0x30) {
            PPCI_COMMON_CONFIG pciConfigData = (PPCI_COMMON_CONFIG)pcicfgBuffer;
            adapterExtension->VendorID = pciConfigData->VendorID;
            adapterExtension->DeviceID = pciConfigData->DeviceID;
            adapterExtension->RevisionID = pciConfigData->RevisionID;
            // on PCI bus, AHCI Base Address is BAR5. Bits 0-3 defined for other usages, not part of address value.
            adapterExtension->AhciBaseAddress = pciConfigData->u.type0.BaseAddresses[5] & (0xFFFFFFF0);
        } else {
            NT_ASSERT(FALSE);
            return SP_RETURN_ERROR;
        }
    //}
#if (NTDDI_VERSION > NTDDI_WIN7)	
	else {
        adapterExtension->VendorID = dumpContext->VendorID;
        adapterExtension->DeviceID = dumpContext->DeviceID;
        adapterExtension->RevisionID = dumpContext->RevisionID;
        adapterExtension->AhciBaseAddress = dumpContext->AhciBaseAddress;

        adapterExtension->RegistryFlags = dumpContext->AdapterRegistryFlags;
        adapterExtension->LogFlags = dumpContext->LogFlags;
    }
#endif	
	

  //2.1 Initialize adapterExtension with AHCI abar
    abar = GetABARAddress(adapterExtension, ConfigInfo);

  //2.1.1 If abar is still NULL after all of that, malformed resources.  We aren't going to get very far.
    if (abar == NULL) {
        return SP_RETURN_ERROR;
    } else {
        adapterExtension->ABAR_Address = abar;
    }

  //2.2 Initialize adapterExtension with version & cap values
    adapterExtension->Version.AsUlong = StorPortReadRegisterUlong(adapterExtension, &abar->VS.AsUlong);
    adapterExtension->CAP.AsUlong = StorPortReadRegisterUlong(adapterExtension, &abar->CAP.AsUlong);
    adapterExtension->CAP2.AsUlong = StorPortReadRegisterUlong(adapterExtension, &abar->CAP2.AsUlong);

  //3.1 Turn on AE (AHCI 1.1 Section 10.1.2 - 1)
    ghc.AsUlong = StorPortReadRegisterUlong(adapterExtension, &abar->GHC.AsUlong);
    if (ghc.AE == 1) {
        if (!AhciAdapterReset(adapterExtension)) {
            return SP_RETURN_ERROR;
        }
    } //AE is 0. Either through power up or reset we are now pretty sure the controller is in 5.2.2.1    H:Init
    ghc.AsUlong = 0;
    ghc.AE = 1;
    StorPortWriteRegisterUlong(adapterExtension, &abar->GHC.AsUlong, ghc.AsUlong);

  //3.2 Determine which ports are implemented by the HBA (AHCI 1.1 Section 10.1.2 - 2)
    adapterExtension->IS = &abar->IS;

  //3.3 get port number information
    adapterExtension->PortImplemented = StorPortReadRegisterUlong(adapterExtension, &abar->PI);

    //3.3.1 get biggest port number value
    numberOfHighestPort = AHCI_MAX_PORT_COUNT - 1;
    //Check from highest bit to lowest bit for the first highest bit set
    for (piMask = (ULONG)(1 << (AHCI_MAX_PORT_COUNT - 1)); piMask != 0; piMask = (ULONG)(piMask >> 1), numberOfHighestPort--){
        if ( (adapterExtension->PortImplemented & piMask) != 0) {
            break;
        }
    } //numberOfHighestPort now holds the correct value

    //3.3.2 get implemented port count
    //if (!IsDumpMode(adapterExtension)) {
        for (i = 0; i <= numberOfHighestPort; i++) {
            if ( (adapterExtension->PortImplemented & (1 << i)) != 0 ) {
                portCount++;
            }
        }
    //} else {
        // in dump environment, only use the desired port.
        //portCount = 1;
    //}

  //3.4 Initializing the rest of PORT_CONFIGURATION_INFORMATION
    ConfigInfo->MaximumTransferLength = AHCI_MAX_TRANSFER_LENGTH;
    ConfigInfo->NumberOfPhysicalBreaks = 0x20;
    ConfigInfo->AlignmentMask = 1;              // ATA devices need WORD alignment
    ConfigInfo->ScatterGather = TRUE;
    ConfigInfo->ResetTargetSupported = TRUE;
    ConfigInfo->NumberOfBuses = (UCHAR)(numberOfHighestPort + 1);
    ConfigInfo->MaximumNumberOfTargets = AHCI_MAX_DEVICE;
    ConfigInfo->MaximumNumberOfLogicalUnits = 1 /*AHCI_MAX_LUN*/;   //NOTE: only supports 1 for now. there is a legacy ATAPI device that may have 2 luns.
    // set driver to run in full duplex mode
    ConfigInfo->SynchronizationModel = StorSynchronizeFullDuplex;
#if (NTDDI_VERSION > NTDDI_WIN7)	
    ConfigInfo->BusResetHoldTime = 0;       // StorAHCI wait RESET to be completed by itself, no need for port driver to wait.
	ConfigInfo->MaxNumberOfIO = portCount * adapterExtension->CAP.NCS;
#endif

    if (adapterExtension->CAP.S64A) {
        ConfigInfo->Dma64BitAddresses = SCSI_DMA64_MINIPORT_SUPPORTED;
        // increase size of SrbExtension to accomodate 64-bit move commands
        // and 1 extra Scripts instruction (turn off 64-bit mode)
        ConfigInfo->SrbExtensionSize += (ULONG)((ConfigInfo->NumberOfPhysicalBreaks + 2) * 4);
    }

#if (NTDDI_VERSION > NTDDI_WIN7)
    ConfigInfo->FeatureSupport |= STOR_ADAPTER_FEATURE_STOP_UNIT_DURING_POWER_DOWN;
#endif

  // 3.4.2 update PortImplemented, HighestPort and portCount if necessary
    //if (!IsDumpMode(adapterExtension)) {
        adapterExtension->HighestPort = numberOfHighestPort;
   // } 
#if (NTDDI_VERSION > NTDDI_WIN7)	
	else {
        adapterExtension->PortImplemented = 1 << dumpContext->DumpPortNumber;
        adapterExtension->HighestPort = dumpContext->DumpPortNumber;
    }
#endif	

  //3.5 Register Power Setting Change Notification Guids
#if (NTDDI_VERSION > NTDDI_WIN7)  
    if (!IsDumpMode(adapterExtension)) {
        /* 0b2d69d7-a2a1-449c-9680-f91c70521c60 -DIPM/HIPM */
        powerSettingChangeGuids[0].Data1 = 0x0b2d69d7;
        powerSettingChangeGuids[0].Data2 = 0xa2a1;
        powerSettingChangeGuids[0].Data3 = 0x449c;
        powerSettingChangeGuids[0].Data4[0] = 0x96;
        powerSettingChangeGuids[0].Data4[1] = 0x80;
        powerSettingChangeGuids[0].Data4[2] = 0xf9;
        powerSettingChangeGuids[0].Data4[3] = 0x1c;
        powerSettingChangeGuids[0].Data4[4] = 0x70;
        powerSettingChangeGuids[0].Data4[5] = 0x52;
        powerSettingChangeGuids[0].Data4[6] = 0x1c;
        powerSettingChangeGuids[0].Data4[7] = 0x60;

        /* DAB60367-53FE-4fbc-825E-521D069D2456 -Adaptive */
        powerSettingChangeGuids[1].Data1 = 0xDAB60367;
        powerSettingChangeGuids[1].Data2 = 0x53FE;
        powerSettingChangeGuids[1].Data3 = 0x4fbc;
        powerSettingChangeGuids[1].Data4[0] = 0x82;
        powerSettingChangeGuids[1].Data4[1] = 0x5E;
        powerSettingChangeGuids[1].Data4[2] = 0x52;
        powerSettingChangeGuids[1].Data4[3] = 0x1D;
        powerSettingChangeGuids[1].Data4[4] = 0x06;
        powerSettingChangeGuids[1].Data4[5] = 0x9D;
        powerSettingChangeGuids[1].Data4[6] = 0x24;
        powerSettingChangeGuids[1].Data4[7] = 0x56;

        StorPortSetPowerSettingNotificationGuids(AdapterExtension, 2, powerSettingChangeGuids);
    }
#endif
  //4.1 Turn on IE, pending interrupts will be cleared when port starts
    adapterExtension->LastInterruptedPort = (ULONG)(-1);
    ghc.IE = 1;
    StorPortWriteRegisterUlong(adapterExtension, &abar->GHC.AsUlong, ghc.AsUlong);


  //4.2 allocate resources, implemented port information needs to be ready before calling the following routine.
    if (adapterExtension->StateFlags.StoppedState == 0) {
        if (!AllocateResourcesForAdapter(adapterExtension, ConfigInfo, portCount)) {
            return SP_RETURN_ERROR;
        }
    }

    // reset "StoppedState". NOTE: This field should not be referenced anymore in this function after following line.
    adapterExtension->StateFlags.StoppedState = 0;

  // 4.3 initialize ports and start them.
    //4.3.1 initialize all AHCI ports
    for (i = 0; i <= adapterExtension->HighestPort; i++) {
        if (adapterExtension->PortExtension[i] != NULL) {
            // in case of PortInitialize fails, ChannelExtension->StateFlags.StartCapable will remain as 'FALSE'. there will be not attempt to start the Port.
            AhciPortInitialize(adapterExtension->PortExtension[i]);
        }
    }

#if (NTDDI_VERSION > NTDDI_WIN7)
    if (IsDumpMode(adapterExtension)) {
        if (adapterExtension->PortExtension[dumpContext->DumpPortNumber] != NULL) {
            adapterExtension->PortExtension[dumpContext->DumpPortNumber]->RegistryFlags = dumpContext->PortRegistryFlags;
        }
    }
#endif	

    //4.3.2 async process to get all ports into running state
    AhciAdapterRunAllPorts(adapterExtension);
	
	DbgPrint("AhciHwFindAdapter finishing\n");

    return SP_RETURN_FOUND;
}

BOOLEAN
AhciHwPassiveInitialize (
    __in PVOID AdapterExtension
    )
{
    UCHAR i;

    PAHCI_ADAPTER_EXTENSION adapterExtension = (PAHCI_ADAPTER_EXTENSION)AdapterExtension;

    for (i = 0; i <= adapterExtension->HighestPort; i++) {
        if (adapterExtension->PortExtension[i] != NULL) {
            StorPortInitializeDpc(AdapterExtension, &adapterExtension->PortExtension[i]->CompletionDpc, AhciPortSrbCompletionDpcRoutine);
        }
    }

    return TRUE;
}

BOOLEAN
AhciHwInitialize (
    __in PVOID AdapterExtension
    )
{
    StorPortEnablePassiveInitialization(AdapterExtension, AhciHwPassiveInitialize);

    return TRUE;
}

SCSI_ADAPTER_CONTROL_STATUS
AhciHwAdapterControl (
    __in PVOID AdapterExtension,
    __in SCSI_ADAPTER_CONTROL_TYPE ControlType,
    __in PVOID Parameters
    )
/*++

Routine Description:

    This is a generic routine to allow for special adapter control activities.
    It maianly handles power change notifications for the adapter.

Arguments:

    AdapterExtension - Pointer to the device extension for adapter.

    ControlType - Specifies the type of call being made through this routine.

    Parameters - Pointer to parameters needed for this control type (optional).

Return Value:

    SCSI_ADAPTER_CONTROL_STATUS - currently either:
        ScsiAdapterControlSuccess (= 0)
        ScsiAdapterControlUnsuccessful (= 1)

--*/

{
    PSCSI_SUPPORTED_CONTROL_TYPE_LIST controlTypeList;
    SCSI_ADAPTER_CONTROL_STATUS status = ScsiAdapterControlSuccess;
#if (NTDDI_VERSION > NTDDI_WIN7)	
    PAHCI_ADAPTER_EXTENSION adapterExtension = (PAHCI_ADAPTER_EXTENSION)AdapterExtension;
#else
	UNREFERENCED_PARAMETER(AdapterExtension);
#endif		

    switch (ControlType) {
        // determine which control types (routines) are supported
        case ScsiQuerySupportedControlTypes:
            // get pointer to control type list
            controlTypeList = (PSCSI_SUPPORTED_CONTROL_TYPE_LIST)Parameters;

            // Report StopAdapter and RestartAdapter are supported.
            if (ScsiQuerySupportedControlTypes < controlTypeList->MaxControlType) {
                controlTypeList->SupportedTypeList[ScsiQuerySupportedControlTypes] = TRUE;
            }
            if (ScsiStopAdapter < controlTypeList->MaxControlType) {
                controlTypeList->SupportedTypeList[ScsiStopAdapter] = TRUE;
            }
            if (ScsiRestartAdapter < controlTypeList->MaxControlType) {
                controlTypeList->SupportedTypeList[ScsiRestartAdapter] = TRUE;
            }
#if (NTDDI_VERSION > NTDDI_WIN7)			
            if (ScsiPowerSettingNotification < controlTypeList->MaxControlType) {
                controlTypeList->SupportedTypeList[ScsiPowerSettingNotification] = TRUE;
            }
#endif			
            break;

        // ScsiStopAdapter maybe called with PnP or Power activities.
        // StorAHCI supports PnP and Power Srb thus does nothing for ScsiStopAdapter. return value is ScsiAdapterControlSuccess.
        case ScsiStopAdapter:
            break;

        // ScsiRestartAdapter is called during adapter powering up.
        // StorAHCI supports PnP and Power Srb thus does nothing for ScsiRestartAdapter. return value is ScsiAdapterControlSuccess.
        case ScsiRestartAdapter:
            break;
#if (NTDDI_VERSION > NTDDI_WIN7)
        // ScsiPowerSettingNotification is called when Storport receives notification from Power Manager when power setting is changed.
        case ScsiPowerSettingNotification:
            AhciAdapterPowerSettingNotification(adapterExtension, (PSTOR_POWER_SETTING_INFO)Parameters);
            break;
#endif
        default:
            status = ScsiAdapterControlUnsuccessful;
            break;

    } // end of switch

    return status;

} // AhciHwAdapterControl

BOOLEAN
AhciHwResetBus (
    __in PVOID AdapterExtension,
    __in ULONG PathId
    )
/*
    Used to Reset the PathId/Port/Channel
*/
{
    BOOLEAN status = FALSE;
    STOR_LOCK_HANDLE lockhandle = {0};
    PAHCI_ADAPTER_EXTENSION adapterExtension = (PAHCI_ADAPTER_EXTENSION)AdapterExtension;

    if ( IsPortValid(adapterExtension, PathId) ) {
        StorPortAcquireSpinLock(adapterExtension, InterruptLock, NULL, &lockhandle);
        status = AhciPortReset(adapterExtension->PortExtension[PathId]);
        StorPortReleaseSpinLock(adapterExtension, &lockhandle);
        adapterExtension->PortExtension[PathId]->DeviceExtension[0].IoRecord.PortDriverResetCount++;
    }

    return status;
}

VOID
AhciAdapterStop (
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
)
/*
    Adapter is being stopped.
*/
{
    // stop all ports.
    ULONG i;

    for (i = 0; i <= AdapterExtension->HighestPort; i++) {
        if (AdapterExtension->PortExtension[i] != NULL) {
            AhciPortStop(AdapterExtension->PortExtension[i]);
        }
    }

    // clear ABAR
    AdapterExtension->ABAR_Address = 0;

    AdapterExtension->StateFlags.StoppedState = 1;

    return;
}

VOID
AhciAdapterRemoval (
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
)
/*
    Release resources allocated for the adapter and its managed ports/devices
*/
{
    ULONG   i;
    PVOID   bufferToFree = NULL;

    if (IsDumpMode(AdapterExtension)) {
        return;
    }

    // release portsChannelExtension. the whole buffer is sliced for Ports, only need to free the first ChannelExtension.
    for (i = 0; i <= AdapterExtension->HighestPort; i++) {
        if (AdapterExtension->PortExtension[i] != NULL) {
            if (bufferToFree == NULL) {
                bufferToFree = AdapterExtension->PortExtension[i];
            }

            if (AdapterExtension->PortExtension[i]->DeviceInitCommands.CommandTaskFile != NULL) {
                StorPortFreePool(AdapterExtension, (PVOID)AdapterExtension->PortExtension[i]->DeviceInitCommands.CommandTaskFile);
                AdapterExtension->PortExtension[i]->DeviceInitCommands.CommandTaskFile = NULL;
            }


            AdapterExtension->PortExtension[i] = NULL;
        }
    }

    if (bufferToFree != NULL) {
        StorPortFreePool(AdapterExtension, bufferToFree);
    }

    return;
}

BOOLEAN
AhciHwBuildIo (
    __in PVOID AdapterExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    )
{
    PAHCI_ADAPTER_EXTENSION adapterExtension = (PAHCI_ADAPTER_EXTENSION)AdapterExtension;
    PAHCI_CHANNEL_EXTENSION channelExtension = NULL;

    // SrbExtension is not Null-ed by Storport, so do it here.
    AhciZeroMemory((PCHAR)GetSrbExtension(Srb), sizeof(AHCI_SRB_EXTENSION));

    if (Srb->SenseInfoBuffer != NULL) {
        AhciZeroMemory((PCHAR)Srb->SenseInfoBuffer, Srb->SenseInfoBufferLength);
    }

    channelExtension = adapterExtension->PortExtension[Srb->PathId];

    // some requests from Storport may bypass power lock, make sure to power up the port to handle them.
    if ( IsPortValid(adapterExtension, Srb->PathId) && 
         (Srb->Function == SRB_FUNCTION_EXECUTE_SCSI) && 
         (channelExtension->StateFlags.StartCapable == FALSE) ) {
        AhciPortPowerUp(channelExtension);
    }

    if ( IsPortValid(adapterExtension, Srb->PathId) && 
         (channelExtension->StartState.ChannelNextStartState != StartComplete) &&
         (Srb->TimeOutValue < AHCI_PORT_START_TIMEOUT_IN_SECONDS) ) {
        // make sure the timeout value is big enough to wait for Port Starting completes.
        Srb->TimeOutValue = AHCI_PORT_START_TIMEOUT_IN_SECONDS;
    }

    return TRUE;
}

BOOLEAN
AhciHwStartIo (
    __in PVOID AdapterExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    )
/*
    1. Process Adapter request
    2. Bail out if it’s adapter request
    3. Validate Port Number, if not valid, bail out.
    4. Process Device/Port request

*/
{
    BOOLEAN status;
    STOR_LOCK_HANDLE lockhandle = {0};
    PAHCI_ADAPTER_EXTENSION adapterExtension = (PAHCI_ADAPTER_EXTENSION)AdapterExtension;
    UCHAR function = Srb->Function;
    BOOLEAN adapterRequest = FALSE;
    BOOLEAN processIO = FALSE;

  //1 Work on Adapter requests
    switch (function) {
        case SRB_FUNCTION_PNP: {
            PSCSI_PNP_REQUEST_BLOCK pnpSrb = (PSCSI_PNP_REQUEST_BLOCK)Srb;

            if ( (pnpSrb->SrbPnPFlags & SRB_PNP_FLAGS_ADAPTER_REQUEST ) != 0 ) {
                // only process PnP request for adapter here.
                adapterRequest = TRUE;

                if ( (pnpSrb->PnPAction == StorRemoveDevice) || (pnpSrb->PnPAction == StorSurpriseRemoval) ) {
                    // the adapter is going to be removed, release all resources allocated for it.
                    AhciAdapterRemoval(adapterExtension);
                    Srb->SrbStatus = SRB_STATUS_SUCCESS;
                } else if (pnpSrb->PnPAction == StorStopDevice) {
                    AhciAdapterStop(adapterExtension);
                    Srb->SrbStatus = SRB_STATUS_SUCCESS;
                } else {
                    Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
                }
                //complete all Adapter PnP request
                StorPortNotification(RequestComplete, AdapterExtension, Srb);
            }
            break;
        }
        case SRB_FUNCTION_POWER: {
            PSCSI_POWER_REQUEST_BLOCK powerSrb = (PSCSI_POWER_REQUEST_BLOCK)Srb;
            
            if (powerSrb->SrbPowerFlags == SRB_POWER_FLAGS_ADAPTER_REQUEST) {
                // only process Power request for adapter here.
                adapterRequest = TRUE;

                if (powerSrb->DevicePowerState == StorPowerDeviceD0) {
                    AhciAdapterPowerUp(adapterExtension);                  //power up
                    Srb->SrbStatus = SRB_STATUS_SUCCESS;
                    StorPortNotification(RequestComplete, AdapterExtension, Srb);
                } else if (powerSrb->DevicePowerState == StorPowerDeviceD3) {
                    AhciAdapterPowerDown(adapterExtension);                //power down
                    Srb->SrbStatus = SRB_STATUS_SUCCESS;
                    StorPortNotification(RequestComplete, AdapterExtension, Srb);
                } else {
                    NT_ASSERT(FALSE);
                    Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
                    StorPortNotification(RequestComplete, AdapterExtension, Srb);
                }
            }
            break;
        }

        case SRB_FUNCTION_FLUSH:  {
            adapterRequest = TRUE;
            //FLUSH Adapter cache. No cache, complete the request.
            Srb->SrbStatus = SRB_STATUS_SUCCESS;
            StorPortNotification(RequestComplete, AdapterExtension, Srb);
            break;
        }

    }

    // 1.1 if the request is for adapter, it has been processed. Return from here.
    if (adapterRequest) {
        return TRUE;
    }

    // 2.1 All requests reach here should be for devices
    if ( !IsPortValid(adapterExtension, Srb->PathId) ) {
        Srb->SrbStatus = SRB_STATUS_NO_DEVICE;
        StorPortNotification(RequestComplete, AdapterExtension, Srb);
        return TRUE;
    }

     // 2.2 work on device reqeusts
    switch (function) {
        case SRB_FUNCTION_RESET_BUS:    // this one may come from class driver, not port driver. same as AhciHwResetBus
        case SRB_FUNCTION_RESET_DEVICE:
        case SRB_FUNCTION_RESET_LOGICAL_UNIT: {
            // these reset requests target to Port
            StorPortAcquireSpinLock(adapterExtension, InterruptLock, NULL, &lockhandle);
            status = AhciPortReset(adapterExtension->PortExtension[Srb->PathId]);
            Srb->SrbStatus = status ? SRB_STATUS_SUCCESS : SRB_STATUS_ERROR;
            StorPortNotification(RequestComplete, AdapterExtension, Srb);
            StorPortReleaseSpinLock(adapterExtension, &lockhandle);
            adapterExtension->PortExtension[Srb->PathId]->DeviceExtension[0].IoRecord.PortDriverResetCount++;
            break;
        }

        case SRB_FUNCTION_PNP: {
            PSCSI_PNP_REQUEST_BLOCK pnpSrb = (PSCSI_PNP_REQUEST_BLOCK)Srb;

            if ( ((pnpSrb->SrbPnPFlags & SRB_PNP_FLAGS_ADAPTER_REQUEST) == 0) &&
                 (pnpSrb->PnPAction == StorQueryCapabilities) &&
#if (NTDDI_VERSION > NTDDI_WIN7)				 
                 (pnpSrb->DataTransferLength >= sizeof(STOR_DEVICE_CAPABILITIES_EX)) ) {
#else					 
                 (pnpSrb->DataTransferLength >= sizeof(STOR_DEVICE_CAPABILITIES)) ) {
#endif					 
                // process StorQueryCapabilities request for device, not adapter.
                // fill in fields of STOR_DEVICE_CAPABILITIES_EX
#if (NTDDI_VERSION > NTDDI_WIN7)				
                PSTOR_DEVICE_CAPABILITIES_EX storCapabilities = (PSTOR_DEVICE_CAPABILITIES_EX)pnpSrb->DataBuffer;
#else			
                PSTOR_DEVICE_CAPABILITIES storCapabilities = (PSTOR_DEVICE_CAPABILITIES)pnpSrb->DataBuffer;
#endif
                storCapabilities->Removable = IsExternalPort(adapterExtension->PortExtension[Srb->PathId]);
                storCapabilities->DeviceD1 = 0;
                storCapabilities->DeviceD2 = 0;
                storCapabilities->LockSupported = 0;
                storCapabilities->EjectSupported = 0;
                storCapabilities->DockDevice = 0;
                storCapabilities->UniqueID = 0;         //no uniqueID, let PnP generates one
                storCapabilities->SilentInstall = 1;
                storCapabilities->SurpriseRemovalOK = 0;
                storCapabilities->NoDisplayInUI = 0;

                Srb->SrbStatus = SRB_STATUS_SUCCESS;
                StorPortNotification(RequestComplete, AdapterExtension, Srb);

            } else {
                Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
                StorPortNotification(RequestComplete, AdapterExtension, Srb);
            }
            break;
        }

        case SRB_FUNCTION_SHUTDOWN: {
            //This request is for the device to flush device data from adapter cache.
            //The miniport driver must hold on to the shutdown request until no data remains in the HBA's internal cache for the target logical unit and, 
            //then, complete the shutdown request. 
            PATA_DEVICE_PARAMETERS  deviceParameters = &adapterExtension->PortExtension[Srb->PathId]->DeviceExtension->DeviceParameters;
            BOOLEAN                 sendStandby = IsDumpMode(adapterExtension);

            deviceParameters->StateFlags.SystemPoweringDown = TRUE;
#if (NTDDI_VERSION > NTDDI_WIN7)			
            if (adapterExtension->PortExtension[Srb->PathId]->DeviceExtension->IdentifyDeviceData->CommandSetSupport.SetTimeStamp == 0x1) {
                IssueSetDateAndTimeCommand(adapterExtension->PortExtension[Srb->PathId], sendStandby);
                processIO = TRUE;
            } else 
#endif				
			if (sendStandby) {
                //in dump mode, this is the last Srb sent after SYNC CACHE, spin down the disk
                PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);
                srbExtension->AtaFunction = ATA_FUNCTION_ATA_COMMAND;
                SetCommandReg((&srbExtension->TaskFile.Current), IDE_COMMAND_STANDBY_IMMEDIATE);
                processIO = TRUE;
            } else {
                Srb->SrbStatus = SRB_STATUS_SUCCESS;
                StorPortNotification(RequestComplete, AdapterExtension, Srb);
            }

            break;
        }

        case SRB_FUNCTION_IO_CONTROL: {
                PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);

                IOCTLtoATA(adapterExtension->PortExtension[Srb->PathId], Srb);
                if (srbExtension->AtaFunction != 0) {
                    if ( ( srbExtension->Sgl == NULL ) && ( IsDataTransferNeeded(Srb) )  ) {
                        srbExtension->Sgl = (PLOCAL_SCATTER_GATHER_LIST)StorPortGetScatterGatherList(adapterExtension, Srb);
                    }
                    processIO = TRUE;
                } else {
                    // complete Srb if no command should be sent to device.
                    NT_ASSERT(Srb->SrbStatus != SRB_STATUS_PENDING);
                    StorPortNotification(RequestComplete, AdapterExtension, Srb);
                }
                break;
            }

        case SRB_FUNCTION_EXECUTE_SCSI: {
                PAHCI_SRB_EXTENSION srbExtension = GetSrbExtension(Srb);

                SCSItoATA(adapterExtension->PortExtension[Srb->PathId], Srb);
                if (srbExtension->AtaFunction != 0) {
                    if ( ( srbExtension->Sgl == NULL ) && ( IsDataTransferNeeded(Srb) )  ) {
                        srbExtension->Sgl = (PLOCAL_SCATTER_GATHER_LIST)StorPortGetScatterGatherList(adapterExtension, Srb);
                    }
                    processIO = TRUE;
                } else {
                    // complete Srb if no command should be sent to device.
                    NT_ASSERT(Srb->SrbStatus != SRB_STATUS_PENDING);
                    StorPortNotification(RequestComplete, AdapterExtension, Srb);
                }
                break;
            }

#if (NTDDI_VERSION > NTDDI_WIN7)
        case SRB_FUNCTION_DUMP_POINTERS: {
            ULONG status = STOR_STATUS_SUCCESS;
            PAHCI_DUMP_CONTEXT dumpContext = NULL;
            PMINIPORT_DUMP_POINTERS dumpPointers = (PMINIPORT_DUMP_POINTERS)Srb->DataBuffer;

            if ( (Srb->DataBuffer == NULL) ||
                 (Srb->DataTransferLength < RTL_SIZEOF_THROUGH_FIELD(MINIPORT_DUMP_POINTERS, MiniportPrivateDumpData)) ) {
                Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
            } else {

                // allocate pool and zero the content
                status = StorPortAllocatePool(AdapterExtension, 
                                              sizeof(AHCI_DUMP_CONTEXT), 
                                              AHCI_POOL_TAG, 
                                              (PVOID*)&dumpContext);

                if ((status != STOR_STATUS_SUCCESS) || (dumpContext == NULL)) {
                   // we cannot continue if cannot get memory for ChannelExtension.
                    Srb->SrbStatus = SRB_STATUS_ERROR;
                } else {
                    dumpPointers->Version = DUMP_MINIPORT_VERSION_1;
                    dumpPointers->Size = sizeof(MINIPORT_DUMP_POINTERS);

                    dumpPointers->MiniportPrivateDumpData = (PVOID)dumpContext;

                    AhciZeroMemory((PCHAR)dumpContext, sizeof(AHCI_DUMP_CONTEXT));
                    dumpContext->VendorID = adapterExtension->VendorID;
                    dumpContext->DeviceID = adapterExtension->DeviceID;
                    dumpContext->RevisionID = adapterExtension->RevisionID;
                    dumpContext->AhciBaseAddress = adapterExtension->AhciBaseAddress;
                    dumpContext->LogFlags = adapterExtension->LogFlags;
                    dumpContext->AdapterRegistryFlags = adapterExtension->RegistryFlags;
                    dumpContext->DumpPortNumber = adapterExtension->PortExtension[Srb->PathId]->PortNumber;
                    dumpContext->PortRegistryFlags = adapterExtension->PortExtension[Srb->PathId]->RegistryFlags;

                    Srb->SrbStatus = SRB_STATUS_SUCCESS;
                }
            }
            StorPortNotification(RequestComplete, AdapterExtension, Srb);
            break;
        }
        case SRB_FUNCTION_FREE_DUMP_POINTERS: {
            ULONG status = STOR_STATUS_SUCCESS;
            PMINIPORT_DUMP_POINTERS dumpPointers = (PMINIPORT_DUMP_POINTERS)Srb->DataBuffer;

            if ( (Srb->DataBuffer == NULL) ||
                 (Srb->DataTransferLength < RTL_SIZEOF_THROUGH_FIELD(MINIPORT_DUMP_POINTERS, MiniportPrivateDumpData)) ) {
                Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
            } else {
                status = StorPortFreePool(AdapterExtension, dumpPointers->MiniportPrivateDumpData);
                Srb->SrbStatus = (status == STOR_STATUS_SUCCESS) ? SRB_STATUS_SUCCESS : SRB_STATUS_ERROR;
            }
            StorPortNotification(RequestComplete, AdapterExtension, Srb);
            break;
        }
#endif
        default: {
            // for unsupported SRB: complete with status: SRB_STATUS_INVALID_REQUEST
            Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
            StorPortNotification(RequestComplete, AdapterExtension, Srb);
            break;
        }

    } //end of switch (function)

    if (processIO) {
        StorPortAcquireSpinLock(adapterExtension, InterruptLock, NULL, &lockhandle);
        AddQueue(adapterExtension->PortExtension[Srb->PathId], &adapterExtension->PortExtension[Srb->PathId]->SrbQueue, Srb, 0xDEADBEEF, 0x11);
        AhciGetNextIos(adapterExtension->PortExtension[Srb->PathId], TRUE);
        StorPortReleaseSpinLock(adapterExtension, &lockhandle);
    }

    return TRUE;
}

VOID
AhciGetNextIos (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in BOOLEAN AtDIRQL
    )
/*
    get Srb from queue and program it to adapter.
    
    assumption: internal request doesn't call this routine. Otherwise, the check of available slot should be changed.
*/
{
    PSCSI_REQUEST_BLOCK Srb;
    BOOLEAN keepFilling;
    ULONG i, commandSlotMask, allocated;

  //1.0 Check if command processing should happen.  If not, this function will be called again when it is ready.
    if (ChannelExtension->StartState.ChannelNextStartState != StartComplete) {
        //We could be waiting ... but it is possible there is no device.  If we know there is no device, then fail all commands.
        if (ChannelExtension->StartState.ChannelNextStartState == StartFailed) {
            // complete all rquests still in queue
            AhciPortFailAllIos(ChannelExtension, SRB_STATUS_NO_DEVICE, AtDIRQL);
        }
        return;
    }

  //1.1 Initialize Variables
    commandSlotMask = 0;

    //If there is a command in the Srb Queue ...
    if (ChannelExtension->SrbQueue.Head != NULL) {
        keepFilling = TRUE;
        //get a mask of all slots expect slot 0, which is reserved for internal use
        for (i = 1; i <= ChannelExtension->AdapterExtension->CAP.NCS; i++) {
            commandSlotMask |= ( 1 << i );
        }
    } else {
        keepFilling = FALSE;
    }

    while (keepFilling) {
        keepFilling = FALSE;
        allocated = GetOccupiedSlots(ChannelExtension);

        if ((~allocated & commandSlotMask) != 0) {
            //there is empty slot. get the next IO
            Srb = RemoveQueue(ChannelExtension, &ChannelExtension->SrbQueue, 0xDEADC0DE, 0x1F);
            if (Srb != NULL) {
                NT_ASSERT(Srb->PathId == ChannelExtension->PortNumber);
                keepFilling = TRUE;

              // get a Srb, try to find an empty slot, fill command table and command header, put Srb into IO slices.
                AhciProcessIo(ChannelExtension, Srb, AtDIRQL);

            } else {  //No more SRBs.  Finish.
                keepFilling = FALSE;
            }
        }
    }

  //Check to see if IO is ready to be programmed to adapter
    ActivateQueue(ChannelExtension, AtDIRQL);
    return;
}


BOOLEAN
AhciHwInterrupt (
    __in PVOID AdapterExtension
    )
{
/*++
AtaHwInterrupt is the interrupt handler.
If the miniport driver requires a large amount of time to process the interrupt it must defer processing to a worker routine.
This routine must attemp one clear the interrupt on the HBA before it returns TRUE.

It assumes:
    The following StorPort routines shall not be called from the AhciHwInterrupt routine – StorPortCompleteAllRequests and StorPortDeviceBusy.
    The miniport could however request for a worker routine and make the calls in the worker routine.

Called by:
    external

It performs:
    (overview)
    1. Prepare for handling the interrupt
    2. Handle interrupts on this port
    3. Clear port interrupt
    4. Complete outstanding commands
    5. Handle error processing if necessary
    (details)
    1.1 Verify the interrupt is for this adapter
    1.2 round robin for selecting the port to process in case of multi-ports have interrupt pending.
    1.3 Initialize Variables
        AHCI 1.1 Section 5.5.3 - 1.
        "Software determines the cause of the interrupt by reading the PxIS register.  It is possible for multiple bits to be set"
    2.1 Understand interrupts on this channel
        AHCI 1.1 Section 5.5.3 - 2.
        "Software clears appropriate bits in the PxIS register corresponding to the cause of the interrupt."
        2.1.1 Handle Fatal Errors
            Clear the PxIS Fatal Error bits
            And prep for error handling
        2.1.3 Handle non Native HotPlug Events
            Clear the PxSERR bits to clear PxIS
            2.1.3.1 Handle Serial ATA Errors (Hotplug insertion or unsolicited COMINIT)
            2.1.3.2 Handle Serial ATA Errors (Hotplug removal or PHY Power Management event)
            2.1.3.3 Handle Serial ATA Errors (everything else)
        2.1.4 Handle Datalength Mismatch Error
            Clear the PxIS Overflow bit bits
            And prep for dataLengthMisMatch handling
        2.1.15 Handle NonFatal Errors
    2.2 Handle Normal Command Completion
    3.1 Clear channel interrupt
        AHCI 1.1 Section 5.5.3 - 3.
        "Software clears the interrupt bit in IS.IPS corresponding to the port."
    4.1 Complete outstanding commands
        AHCI 1.1 Section 5.5.3 - 4.
        "If executing non-queued commands, software reads the PxCI register, and compares the current value to the list of commands previously issued by software that are still outstanding.  If executing native queued commands, software reads the PxSACT register and compares the current value to the list of commands previously issued by software.  Software completes with success any outstanding command whose corresponding bit has been cleared in the respective register. PxCI and PxSACT are volatile registers; 
        software should only use their values to determine commands that have completed, not to determine which commands have previously been issued."
        4.1.1 Determine if there was an ATA error on the command completed. Error register is only valid if bit0 of Status is 1
        4.1.2 Check to see if the right amount of data transfered
            set channelExtension->Slot[i]->DataTransferLength = the amount transfered
        4.1.3 Otherwise, command completed successfully
    5.1 Handle error processing
        Until the error recovery can be completed, don't let any more IO's come through
        AHCI 1.1 Section 5.5.3 - 5.
        "If there were errors, noted in the PxIS register, software performs error recovery actions (see section 6.2.2)."
        AHCI 1.1 Section 6.2.2.1 Non-Queued Error Recovery (this may take a while, better queue a DPC)
        Complete further processing in the worker routine and enable interrupts on the channel
    6.1 Partial to Slumber auto transit

Affected Variables/Registers:

Return Values:
    AtaHwInterrrupt returns TRUE if the interrupt is handled.
    If the adapter, channel/port did not generate the interrupt the routine should return FALSE as soon as possible.

--*/
    PAHCI_ADAPTER_EXTENSION adapterExtension;
    PAHCI_CHANNEL_EXTENSION channelExtension;
    //Interrupt handling structures
    AHCI_INTERRUPT_STATUS   pxis;
    AHCI_INTERRUPT_STATUS   pxisMask;
    AHCI_SERIAL_ATA_STATUS  ssts;
    AHCI_SERIAL_ATA_ERROR   serr;
    AHCI_SERIAL_ATA_ERROR   serrMask;
    AHCI_COMMAND            cmd;
    ULONG                   ci;
    ULONG                   sact;
    ULONG                   outstanding;
    ULONG                   is;
    ULONG                   interruptPorts;
    ULONG                   i;

    adapterExtension = (PAHCI_ADAPTER_EXTENSION)AdapterExtension;

    is = StorPortReadRegisterUlong(AdapterExtension, adapterExtension->IS);
    interruptPorts = (is & adapterExtension->PortImplemented);

  //1.1 Verify the interrupt is for this adapter
    if (interruptPorts == 0) {
        // interrupt is not for this adapter
        return FALSE;
    }

  //1.2 round robin for selecting the port to process in case of multi-ports have interrupt pending.
    i = (adapterExtension->LastInterruptedPort + 1) % (adapterExtension->HighestPort + 1);
    do {
        if ( ((interruptPorts & (1 << i)) != 0) && (adapterExtension->PortExtension[i]->StateFlags.StartCapable == 1) ) {
            break;
        }
        i = (i + 1) % (adapterExtension->HighestPort + 1);

    } while (i != adapterExtension->LastInterruptedPort);

    if ( (i == adapterExtension->LastInterruptedPort) &&
        (((interruptPorts & (1 << i)) == 0) || (adapterExtension->PortExtension[i]->StateFlags.StartCapable == 0)) ) {
        // interrupt is not for this adapter
        return FALSE;
    }

    adapterExtension->LastInterruptedPort = i;
    channelExtension = adapterExtension->PortExtension[i];

    if (LogExecuteFullDetail(adapterExtension->LogFlags)) {
        RecordExecutionHistory(channelExtension, 0x00000005);//AhciHwInterrupt Enter
    }

  //1.3 Initialize Variables
    sact = 0;
    cmd.AsUlong = 0;
    ssts.AsUlong = 0;
    pxisMask.AsUlong = serrMask.AsUlong = 0;

    pxis.AsUlong = StorPortReadRegisterUlong(AdapterExtension, &channelExtension->Px->IS.AsUlong);
    serr.AsUlong = StorPortReadRegisterUlong(AdapterExtension, &channelExtension->Px->SERR.AsUlong);

  //2.1 Understand interrupts on this channel
    //2.1.1 Handle Fatal Errors: Interface Fatal Error Status || Host Bus Data Error Status || Host Bus Fatal Error Status || Task File Error Status
    if (pxis.IFS || pxis.HBDS || pxis.HBFS || pxis.TFES) {
        pxisMask.AsUlong = 0;
        pxisMask.IFS = pxisMask.HBDS = pxisMask.HBFS = pxisMask.TFES = 1;
        StorPortWriteRegisterUlong(AdapterExtension, &channelExtension->Px->IS.AsUlong, pxisMask.AsUlong);

      //call the correct error handling based on current hw queue workload type
        sact = StorPortReadRegisterUlong(AdapterExtension, &channelExtension->Px->SACT);

        if(sact != 0) {
          //5.1 NCQ, Handle error processing
            channelExtension->StateFlags.CallAhciReset = 1;

          //Give NCQ one chance
            if (channelExtension->StateFlags.NCQ_Succeeded == 0) {
                channelExtension->StateFlags.NCQ_Activated = 0;
            }
        } else {
          //5.1 Non-NCQ, Handle error processing
            channelExtension->StateFlags.CallAhciNonQueuedErrorRecovery = 1;
        }
    }

  //2.1.2 Handle non Native HotPlug Events
    // Cold Port Detect Status
    if (pxis.CPDS) {
        pxisMask.AsUlong = 0;
        pxisMask.CPDS = 1;
        StorPortWriteRegisterUlong(AdapterExtension, &channelExtension->Px->IS.AsUlong, pxisMask.AsUlong);
      // Handle bus rescan processing processing
        channelExtension->StateFlags.CallAhciReportBusChange = 1;
    }

    if (pxis.DMPS || pxis.PCS) {
        cmd.AsUlong = StorPortReadRegisterUlong(AdapterExtension, &channelExtension->Px->CMD.AsUlong);
        
        // Device Mechanical Presence Status
        if (pxis.DMPS) {
            pxisMask.AsUlong = 0;
            pxisMask.DMPS = 1;
            StorPortWriteRegisterUlong(AdapterExtension, &channelExtension->Px->IS.AsUlong, pxisMask.AsUlong);
            // Mechanical Presence Switch Attached to Port
            if (cmd.MPSP) {
              // Handle bus rescan processing processing
                channelExtension->StateFlags.CallAhciReportBusChange = 1;
            }
        }

        //2.1.3.1 Handle Serial ATA Errors (Hotplug insertion or unsolicited COMINIT)
        // Port Connect Change Status
        if (pxis.PCS) {
            //When PxSERR.DIAG.X is set to one this bit indicates a COMINIT signal was received.  This bit is reflected in the PxIS.PCS bit.
            serrMask.DIAG.X = 1;
            StorPortWriteRegisterUlong(AdapterExtension, &channelExtension->Px->SERR.AsUlong, serrMask.AsUlong);
            // PCS = 1 could be an unsolicited COMINIT on an already detected drive. See AHCI 6.2.2.3    Recovery of Unsolicited COMINIT
            if ((channelExtension->StateFlags.IgnoreHotplugDueToResetInProgress == 0) && (cmd.ST == 0) ) {
              // Handle bus rescan processing processing
                channelExtension->StateFlags.CallAhciReportBusChange = 1;
            }
        }
    }

    //2.1.3.2 Handle Serial ATA Errors (Hotplug removal or PHY Power Management event)
    // PhyRdy Change Status
    if (pxis.PRCS) {
        //Hot plug removals are detected via the PxIS.PRCS bit that directly reflects the PxSERR.DIAG.N bit.
        //Note that PxSERR.DIAG.N is also set to ‘1’ on insertions and during interface power management entry/exit.
        serrMask.DIAG.N = 1;
        StorPortWriteRegisterUlong(AdapterExtension, &channelExtension->Px->SERR.AsUlong, serrMask.AsUlong);

        ssts.AsUlong = StorPortReadRegisterUlong(AdapterExtension, &channelExtension->Px->SSTS.AsUlong);

#if (NTDDI_VERSION > NTDDI_WIN7)
        //If a drive has already been found and it is a ZPODD (*) ...
        if (channelExtension->DeviceExtension[0].IdentifyPacketData->SerialAtaCapabilities.SlimlineDeviceAttention) {
            if (ssts.DET == 0) {
              // ... (*) and there is no presence on the wire ...
                // ... try to stop the port.
                if ( P_NotRunning(channelExtension, channelExtension->Px) ) {
                    // If that succeeds, complete all outstanding commands.  CI is now cleared.
                    // This is precautionary as there shall be no IO when D3 occured, but the miniport may always create its own commands.
                    channelExtension->SlotManager.CommandsToComplete = GetOccupiedSlots(channelExtension);
                    channelExtension->SlotManager.CommandsIssued = 0;
                    channelExtension->SlotManager.NCQueueSlice = 0;
                    channelExtension->SlotManager.NormalQueueSlice = 0;
                    channelExtension->SlotManager.SingleIoSlice = 0;
                    channelExtension->SlotManager.HighPriorityAttribute = 0;
                } else {
                    NT_ASSERT(FALSE);     // Looks like a hardware issue, will recover in P_Running_StartAttempt() when ZPODD is powered on again.
                }
            } else if (ssts.DET == 3) {
              // ... (*) and there is presence on the wire ...
                // ... try to get the channel started.
                P_Running_StartAttempt(channelExtension, TRUE);
            }
        } else 
#endif			
		if ( !AdapterIgnoreHotPlug(adapterExtension) && (ssts.DET == 0) && (ssts.IPM == 0) ) {
            if (!channelExtension->StateFlags.IgnoreHotplugDueToResetInProgress) {
              // Handle bus rescan processing processing
                channelExtension->StateFlags.CallAhciReportBusChange = 1;
            }
        }
    }

    //2.1.3.3 Handle other Serial ATA Errors (everything else)
    if (serr.AsUlong > 0) {
        StorPortWriteRegisterUlong(AdapterExtension, &channelExtension->Px->SERR.AsUlong, (ULONG)~0);
    }

    //2.1.4 Handle Datalength Mismatch Error
    // Overflow Status
    if (pxis.OFS) {
        pxisMask.AsUlong = 0;
        pxisMask.OFS = 1;
        StorPortWriteRegisterUlong(AdapterExtension, &channelExtension->Px->IS.AsUlong, pxisMask.AsUlong);
      //5.1 Handle error processing
        // AHCI 6.1.5 COMRESET is required by software to clean up from this serious error
        channelExtension->StateFlags.CallAhciReset = 1;
    }

    //2.1.5 Handle NonFatal Errors
    // Interface Non-fatal Error Status
    if (pxis.INFS) {
        pxisMask.AsUlong = 0;
        pxisMask.INFS = 1;
        StorPortWriteRegisterUlong(AdapterExtension, &channelExtension->Px->IS.AsUlong, pxisMask.AsUlong);
    }

    //2.1.6 Handle Asynchronous Notification
    // Set Device Bits Interrupt
    if (( pxis.SDBS == 1) && (channelExtension->StateFlags.AN_Enabled == 1)) {
        if (channelExtension->ReceivedFIS->SetDeviceBitsFis.N) {
            // Asynchronous Notification is received. Allow polling GESN to be sent to device
            channelExtension->StateFlags.AN_SendGesnToDevice = 1;
        }
    }

    //2.3 Handle Normal Command Completion
    pxisMask.AsUlong =0;
    // Device to Host Register FIS Interrupt
    if (pxis.DHRS) {
        pxisMask.DHRS = 1;
    }
    // PIO Setup FIS Interrupt
    if (pxis.PSS) {
        pxisMask.PSS = 1;
    }
    // DMA Setup FIS Interrupt
    if (pxis.DSS) {
        pxisMask.DSS = 1;
    }
    // Set Device Bits Interrupt
    if (pxis.SDBS) {
        pxisMask.SDBS = 1;
    }
    // Descriptor Processed (A PRD with the ‘I’ bit set has transferred all of its data)
    if (pxis.DPS) {
        pxisMask.DPS = 1;
    }
    if (pxisMask.AsUlong != 0 ) {
        StorPortWriteRegisterUlong(AdapterExtension, &channelExtension->Px->IS.AsUlong, pxisMask.AsUlong);
    }

  //3. Clear channel interrupt
    is = 0;
    is |= (1 << channelExtension->PortNumber);
    StorPortWriteRegisterUlong(AdapterExtension, adapterExtension->IS, is);

  //4. Complete outstanding commands
    ci = StorPortReadRegisterUlong(AdapterExtension, &channelExtension->Px->CI);
    sact = StorPortReadRegisterUlong(AdapterExtension, &channelExtension->Px->SACT);
    outstanding = ci | sact;
    
    if( (channelExtension->SlotManager.CommandsIssued & ~outstanding) > 0 ) {
       // all completed commands by hardware will be marked compelted
        channelExtension->SlotManager.CommandsToComplete |= (channelExtension->SlotManager.CommandsIssued & ~outstanding);
        channelExtension->SlotManager.CommandsIssued &= outstanding;

      // recording execution history for completing SRB
        RecordInterruptHistory(channelExtension, pxis.AsUlong, ssts.AsUlong, serr.AsUlong, ci, sact, 0x20000005);   //AhciHwInterrupt complete IO

        AhciCompleteIssuedSRBs(channelExtension, SRB_STATUS_SUCCESS, TRUE);
    } else {
      // recording execution history for no SRB to be completed
        RecordInterruptHistory(channelExtension, pxis.AsUlong, ssts.AsUlong, serr.AsUlong, ci, sact, 0x20010005);   //AhciHwInterrupt No IO completed
    }

   //5. error process
    if ( ErrorRecoveryIsPending(channelExtension) ) {
        AhciPortErrorRecovery(channelExtension);
    }

   //6.1 Partial to Slumber auto transit
    if ((ci == 0) && (sact == 0) &&               // both Px->SACT and Px->CI are cleared.
        !NoLpmSupport(channelExtension) &&          // supports Aggressive LPM
        (channelExtension->Px->CMD.ALPE != 0)) {    // Aggressive LPM is enabled
        //
        if ((adapterExtension->CAP2.APST == 0) ||       // auto Partial to Slumber is not supported.
            (channelExtension->Px->CMD.APSTE == 0)) {   // auto Partial to Slumber is not enabled.
          // able to enter Partial state
            if (channelExtension->Px->CMD.ASP == 0) {
                // the device is going into Partial state.
                // We may need to set a callback function for auto transit to Slumber.
                if ((channelExtension->AutoPartialToSlumberInterval > 0) && (channelExtension->StartState.ChannelNextStartState == StartComplete) ) {
                    // convert inverval value from ms to us.
                    AhciRequestTimerCallback(channelExtension, AhciAutoPartialToSlumber, NULL, channelExtension->AutoPartialToSlumberInterval * 1000, CallbackTypeLPM);
                }

            }
        }
    }

    if (LogExecuteFullDetail(adapterExtension->LogFlags)) {
        RecordExecutionHistory(channelExtension, 0x10000005);//Exit AhciHwInterrupt
    }

    return TRUE;
}

#if (NTDDI_VERSION > NTDDI_WIN7)
VOID
AhciHwTracingEnabled (
    __in PVOID AdapterExtension,
    __in BOOLEAN Enabled
    )
{
    PAHCI_ADAPTER_EXTENSION adapterExtension = (PAHCI_ADAPTER_EXTENSION)AdapterExtension;

    adapterExtension->TracingEnabled = Enabled;

    return;
}

SCSI_UNIT_CONTROL_STATUS
AhciHwUnitControl (
    IN PVOID AdapterExtension,
    IN SCSI_UNIT_CONTROL_TYPE ControlType,
    IN PVOID Parameters
    )
/*++

Routine Description:

    This is a generic routine to allow for special unit control activities.
    It maianly handles device start irp for a specific device.

Arguments:

    AdapterExtension - Pointer to the device extension for adapter.

    ControlType - Specifies the type of call being made through this routine.

    Parameters - Pointer to parameters needed for this control type (optional).

Return Value:

    SCSI_UNIT_CONTROL_STATUS - currently either:
        ScsiUnitControlSuccess = 0,
        ScsiUnitControlUnsuccessful

--*/

{
    SCSI_UNIT_CONTROL_STATUS status;
    PAHCI_ADAPTER_EXTENSION adapterExtension = (PAHCI_ADAPTER_EXTENSION)AdapterExtension;
    PSCSI_SUPPORTED_CONTROL_TYPE_LIST controlTypeList;

    switch (ControlType) {
        // determine which control types (routines) are supported
        case ScsiQuerySupportedUnitControlTypes:
            // get pointer to control type list
            controlTypeList = (PSCSI_SUPPORTED_CONTROL_TYPE_LIST)Parameters;

            // Report ScsiQuerySupportedUnitControlTypes, ScsiUnitStart and ScsiUnitPower are supported. 
            if (ScsiQuerySupportedControlTypes < controlTypeList->MaxControlType) {
                controlTypeList->SupportedTypeList[ScsiQuerySupportedUnitControlTypes] = TRUE;
            }    
            if (ScsiUnitStart < controlTypeList->MaxControlType) {
                controlTypeList->SupportedTypeList[ScsiUnitStart] = TRUE;
            }    
            if (ScsiUnitPower < controlTypeList->MaxControlType) {
                controlTypeList->SupportedTypeList[ScsiUnitPower] = TRUE;
            }    
            status = ScsiUnitControlSuccess;
            break;

        // ScsiUnitStart is called during Storport processes IRP_MN_START_DEVICE
        case ScsiUnitStart: {
                PSTOR_ADDR_BTL8 storAddrBtl8 = (PSTOR_ADDR_BTL8)Parameters;
                // this is the PnP call IRP_MN_START_DEVICE for the LUN. Need to do device initialization work. 
                if (IsPortValid(adapterExtension, storAddrBtl8->Path)) {
                    AhciDeviceStart(adapterExtension->PortExtension[storAddrBtl8->Path]);
                    status = ScsiUnitControlSuccess;
                } else {
                    status = ScsiUnitControlUnsuccessful;
                }
            }
            break;
        
        // ScsiUnitPower is called during Storport processes power Irp for the unit
        case ScsiUnitPower: {
                PSTOR_UNIT_CONTROL_POWER unitControlPower = (PSTOR_UNIT_CONTROL_POWER)Parameters;
                PSTOR_ADDR_BTL8 storAddrBtl8 = (PSTOR_ADDR_BTL8)unitControlPower->Address;
                // this is the power call for the LUN. Need to power up or power down the device as necessary. 
                if (IsPortValid(adapterExtension, storAddrBtl8->Path)) {
                    if (unitControlPower->PowerState == StorPowerDeviceD0) {
                        AhciPortPowerUp(adapterExtension->PortExtension[storAddrBtl8->Path]);
                        status = ScsiUnitControlSuccess;
                    } else if (unitControlPower->PowerState == StorPowerDeviceD3) {
                        AhciPortPowerDown(adapterExtension->PortExtension[storAddrBtl8->Path]);
                        status = ScsiUnitControlSuccess;
                    } else {
                        NT_ASSERT(FALSE);
                        status = ScsiUnitControlUnsuccessful;
                    }
                } else {
                    status = ScsiUnitControlUnsuccessful;
                }
            }
            break;
        
        default:
            status = ScsiUnitControlUnsuccessful;
            break;

    } // end of switch

    return status;
}


#endif
#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4152)
#pragma warning(default:4214)
#pragma warning(default:4201)
#endif

ULONG
DriverEntry (
    __in PVOID DriverObject,
    __in PVOID RegistryPath
    )
	
/*++

Routine Description:

    Initial entry point for miniport driver.

Arguments:

    Driver Object

Return Value:

    Status from StorPortInitialize()

--*/

{
    ULONG status;
    HW_INITIALIZATION_DATA hwInitializationData = {0};

    DebugPrint((1, "\tSTORAHCI, Storport AHCI Miniport Driver.\n"));

    hwInitializationData.HwInitializationDataSize = sizeof(HW_INITIALIZATION_DATA);

    // required miniport entry point routines.
    hwInitializationData.HwInitialize = AhciHwInitialize;
    hwInitializationData.HwStartIo = AhciHwStartIo;
    hwInitializationData.HwInterrupt = AhciHwInterrupt;
    hwInitializationData.HwFindAdapter = AhciHwFindAdapter;
    hwInitializationData.HwResetBus = AhciHwResetBus;
    hwInitializationData.HwAdapterControl = AhciHwAdapterControl;
    hwInitializationData.HwBuildIo = AhciHwBuildIo;
#if (NTDDI_VERSION > NTDDI_WIN7)	
    hwInitializationData.HwTracingEnabled = AhciHwTracingEnabled;
    hwInitializationData.HwUnitControl = AhciHwUnitControl;
#endif	

    // Specifiy adapter specific information.
    hwInitializationData.AutoRequestSense = TRUE;
    hwInitializationData.NeedPhysicalAddresses = TRUE;
    hwInitializationData.NumberOfAccessRanges = NUM_ACCESS_RANGES;
    hwInitializationData.AdapterInterfaceType = PCIBus;
    hwInitializationData.MapBuffers = STOR_MAP_NON_READ_WRITE_BUFFERS;
    hwInitializationData.TaggedQueuing = TRUE;
    hwInitializationData.MultipleRequestPerLu = TRUE;
#if (NTDDI_VERSION > NTDDI_WIN7)	
    hwInitializationData.FeatureSupport |= STOR_FEATURE_ATA_PASS_THROUGH;               // indicating this miniport driver supports ATA PASS-THROUGH(16) command.
    hwInitializationData.FeatureSupport |= STOR_FEATURE_FULL_PNP_DEVICE_CAPABILITIES;   // indicating this miniport driver supplies values for all PnP Device Capability fields.
    hwInitializationData.FeatureSupport |= STOR_FEATURE_DUMP_POINTERS;                  // indicating this miniport support the dump pointers SRBs
    hwInitializationData.FeatureSupport |= STOR_FEATURE_DUMP_RESUME_CAPABLE;            // indicating this miniport driver supports resume capability for dump stack.
    hwInitializationData.FeatureSupport |= STOR_FEATURE_DEVICE_NAME_NO_SUFFIX;          // indicating the miniport driver prefers device friendly name without suffix: "SCSI <type> Device"
#endif
    // Set required extension sizes.
    hwInitializationData.DeviceExtensionSize = sizeof(AHCI_ADAPTER_EXTENSION);

    // NOTE: Command Table (1st field in AHCI_SRB_EXTENSION structure) must align to 128 bytes as physical limitation.
    // StorPort does not have interface allowing miniport requiring this.
    // Adding 128 in SrbExtensionSize so that we can use the part starting from right alignment.
    hwInitializationData.SrbExtensionSize = sizeof(AHCI_SRB_EXTENSION) + 128 ; // SrbExtension contains AHCI_SRB_EXTENSION

    // call StorPort to register HW init data
    status = StorPortInitialize(DriverObject,
                                RegistryPath,
                                &hwInitializationData,
                                NULL);

    return status;

} // end DriverEntry()

