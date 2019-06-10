/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    hbastat.c

Abstract:
    This file contains the core logic for managing AHCI controller state transistions.
    Primarily it focuses on starting and stopping the channel and performing resets.

Notes:

Revision History:

        Nathan Obr (natobr),  February 2005 - September 2006 rev 1 (NCQ, LPM, Hotplug, persistant state) 
                              December 2006 - August 2007    rev 2 (async)
        Michael Xing (xiaoxing) December 2009
--*/

#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning(disable:4214) // bit field types other than int
#pragma warning(disable:4201) // nameless struct/union

#include "generic.h"

BOOLEAN
AhciAdapterReset( 
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
/*
This function brings the HBA and all ports to the H:Idle and P:Idle states by way of HBA Reset and P:Reset

Called By:
    AhciHwFindAdapter if needed.

It assumes:
    AdapterExtension->ABAR_Address is valid.

It performs:
    //10.4.3    HBA Reset
    //5.3.2.1    P:Reset    
    //5.3.2.2    P:Init

Return value:
    BOOLEAN - success or not.  note the many controller registers that hold reset values.
*/
{
    AHCI_Global_HBA_CONTROL ghc;
    PAHCI_MEMORY_REGISTERS  abar = AdapterExtension->ABAR_Address;
    int i;

    if (abar == NULL) {
        return FALSE;
    }

//10.4.3    HBA Reset
  //If the HBA becomes unusable for multiple ports, and a software reset or port reset does not correct the problem, software may reset the entire HBA by setting GHC.HR to ‘1’.  When software sets the GHC.HR bit to ‘1’, the HBA shall perform an internal reset action.  
    ghc.HR = 1;
    StorPortWriteRegisterUlong(AdapterExtension, &abar->GHC.AsUlong, ghc.AsUlong);

  //The bit shall be cleared to ‘0’ by the HBA when the reset is complete.  A software write of ‘0’ to GHC.HR shall have no effect.  To perform the HBA reset, software sets GHC.HR to ‘1’ and may poll until this bit is read to be ‘0’, at which point software knows that the HBA reset has completed.
    ghc.AsUlong = StorPortReadRegisterUlong(AdapterExtension, &abar->GHC.AsUlong);    
    //5.2.2.1    H:Init
    //5.2.2.2    H:WaitForAhciEnable
    for (i = 0;(i < 50) && (ghc.HR == 1); i++) {
        StorPortStallExecution(20000);  //20 milliseconds
        ghc.AsUlong = StorPortReadRegisterUlong(AdapterExtension, &abar->GHC.AsUlong);
    }

    //If the HBA has not cleared GHC.HR to ‘0’ within 1 second of software setting GHC.HR to ‘1’, the HBA is in a hung or locked state.
    if(i == 50) {
        AdapterExtension->ErrorFlags = (1 << 29);
        return FALSE;
    }
        
    //When GHC.HR is set to ‘1’, GHC.AE, GHC.IE, the IS register, and all port register fields (except PxFB/PxFBU/PxCLB/PxCLBU) that are not HwInit in the HBA’s register memory space are reset.  The HBA’s configuration space and all other global registers/bits are not affected by setting GHC.HR to ‘1’.  Any HwInit bits in the port specific registers are not affected by setting GHC.HR to ‘1’.  The port specific registers PxFB, PxFBU, PxCLB, and PxCLBU are not affected by setting GHC.HR to ‘1’.  If the HBA supports staggered spin-up, the PxCMD.SUD bit will be reset to ‘0’; software is responsible for setting the PxCMD.SUD and PxSCTL.DET fields appropriately such that communication can be established on the Serial ATA link.  If the HBA does not support staggered spin-up, the HBA reset shall cause a COMRESET to be sent on the port

    return TRUE;
}   


VOID
AhciCOMRESET(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    PAHCI_PORT Px
    )
/*
PHY Reset:COMRESET
    SCTL.DET Controls the HBA’s device detection and interface initialization.
    DET=1 Performs interface communication initialization sequence to establish communication. This is functionally equivalent to a hard reset and results in the interface being reset and communications reinitialized.  While this field is 1h, COMRESET is transmitted on the interface.  
    Software should leave the DET field set to 1h for a minimum of 1 millisecond to ensure that a COMRESET is sent on the interface.
    since we are in 5.3.2.3    P:NotRunning and PxCMD.SUD = ‘0’ does this still take us to P:StartComm?
Called By:
    AhciPortReset,AhciNonQueuedErrorRecovery,P_Running_WaitWhileDET1,AhciHwControlIdeStart
It assumes:
    nothing
It performs:
    5.3.2.11    P:StartComm
    (overview)
    1 Prepare for COMRSET
    2 Perform COMRESET
    3 Clean up after COMRESET
    (details)
    1.1 make sure ST is 0.  DET cannot be altered while ST == 1 as per AHCI 1.1 section 5.3.2.3.
    1.2 Don't allow a comm init to trigger a hotplug
    1.3 Ignore Hotplug events until the channel is started again
    2.1 Perform COMRESET
    2.2 Wait for DET to be set
    3.1 Clear SERR

Affected Variables,Registers:
    CMD.ST, SCTL.DET, CI, SACT, SERR

Return Values:
    none
*/
{
    AHCI_SERIAL_ATA_CONTROL sctl;
    AHCI_COMMAND            cmd;
    AHCI_INTERRUPT_ENABLE   ieOrig;
    AHCI_INTERRUPT_ENABLE   ieTemp;
    AHCI_SERIAL_ATA_STATUS  ssts;
    UCHAR                   i;

    RecordExecutionHistory(ChannelExtension, 0x00000010);//AhciCOMRESET

  //1.1 make sure ST is 0.  DET cannot be altered while ST == 1 as per AHCI 1.1 section 5.3.2.3.
    cmd.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &Px->CMD.AsUlong);
    if(cmd.ST == 1) {
        RecordExecutionHistory(ChannelExtension, 0x10fa0010);   //AhciCOMRESET, PxCMD.ST is 1. Abort
        return;
    }

  //1.2 Don't allow a COMINIT to trigger a hotplug
    ieTemp.AsUlong = ieOrig.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &Px->IE.AsUlong);
    ieTemp.PRCE = 0;        // "PhyRdy Change Interrupt Enable", no generate interrupt with PxIS.PRCS
    ieTemp.PCE = 0;         // "Port Change Interrupt Enable", no generate interrupt with PxIS.PCS
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &Px->IE.AsUlong, ieTemp.AsUlong);

  //1.3 Ignore Hotplug events until the channel is started again
    ChannelExtension->StateFlags.IgnoreHotplugDueToResetInProgress = TRUE;

  //2.1 Perform COMRESET
    sctl.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &Px->SCTL.AsUlong);
    sctl.DET = 1;
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &Px->SCTL.AsUlong, sctl.AsUlong);
    //DET=1 Performs interface communication initialization sequence to establish communication. This is functionally equivalent to a hard reset and results in the interface being reset and communications reinitialized.  While this field is 1h, COMRESET is transmitted on the interface.  
    //Software should leave the DET field set to 1h for a minimum of 1 millisecond to ensure that a COMRESET is sent on the interface.
    StorPortStallExecution(1000);  

    sctl.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &Px->SCTL.AsUlong);
    sctl.DET = 0;
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &Px->SCTL.AsUlong, sctl.AsUlong);

  //2.2 Wait for DET to be set
    // AHCI 10.4.2 After clearing PxSCTL.DET to 0h, software should wait for communication to be re-established as indicated by bit 0 of PxSSTS.DET being set to ‘1’.
    // typically, it will be done in 10ms. max wait 30ms to be safe
    StorPortStallExecution(50);
    ssts.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &Px->SSTS.AsUlong);

    if (ssts.DET == 0) {
        for (i = 0; i < (AHCI_PORT_WAIT_ON_DET_COUNT * 10); i++) {
            StorPortStallExecution(1000);
            ssts.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &Px->SSTS.AsUlong);
            if (ssts.DET != 0) {
                break;
            }
        }
    }

  //2.3 Enable Hotplug again if it was enabled before
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &Px->IE.AsUlong, ieOrig.AsUlong);

  //3.1 Clear SERR
    // AHCI 10.4.2 software should write all 1s to the PxSERR register to clear any bits that were set as part of the port reset.
    StorPortWriteRegisterUlong(ChannelExtension->AdapterExtension, &Px->SERR.AsUlong, (ULONG)~0);
  
  //3.2 record the count of reset (both interal or asked by port driver)
    ChannelExtension->DeviceExtension[0].IoRecord.TotalResetCount++;

    RecordExecutionHistory(ChannelExtension, 0x10000010);//Exit AhciCOMRESET
}


BOOLEAN
P_NotRunning(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    PAHCI_PORT Px
    )
/*
Called By:

It assumes:
    nothing

It performs:
    5.3.2.3    P:NotRunning
    (overview)
    1 Clear CMD.ST
    2 Clear CMD.FRE
    3 Update the Channel Start State

    (details)
    1.1 Clear CMD.ST
    1.2 Verify CR cleared
    1.3 Verify CI cleared
    2.1 Clear CMD.FRE
    2.2 Verify FR cleared
    3.1 Update the Channel Start State

Affected Variables,Registers:
    cmd.ST cmd.CR cmd.FRE cmd.FR are 0 and the controller is in P:NotRunning

Return value:
    FALSE if the ST,CR,FRE, and FRE could not be cleared.  TRUE if success.
    If FALSE is returned, the caller may issue RESET to recover
*/
{
    PAHCI_ADAPTER_EXTENSION adapterExtension;
    BOOLEAN                 supportsCLO;
    AHCI_COMMAND            cmd; 
    AHCI_TASK_FILE_DATA     tfd;
    ULONG                   ci;
    UCHAR                   i;

    adapterExtension = ChannelExtension->AdapterExtension;
    supportsCLO = (adapterExtension->CAP.SCLO == 0x01);     //3.1.1 make sure HBA supports Command List Override.

    if (ChannelExtension->StartState.ChannelNextStartState == Stopped) {
        RecordExecutionHistory(ChannelExtension, 0x00030011);   //P_NotRunning, already stopped, nothing to do.
        return TRUE;
    }

    // record this function is called.
    if (supportsCLO) {
        RecordExecutionHistory(ChannelExtension, 0x10810011);   //P_NotRunning, CLO is enabled.
    } else {
        RecordExecutionHistory(ChannelExtension, 0x00000011);   //P_NotRunning, CLO isn't enabled.
    }

  //1.1 Clear CMD.ST
    cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &Px->CMD.AsUlong);
    //AHCI 10.3.2 on FRE it says:
    //Software shall not clear this [FRE] bit while PxCMD.ST remains set to ‘1’."
    //System software places a port into the idle state by clearing PxCMD.ST and waiting for PxCMD.CR to return ‘0’ when read.  
    cmd.ST = 0;
    StorPortWriteRegisterUlong(adapterExtension, &Px->CMD.AsUlong, cmd.AsUlong);

  //1.1.1 Update the Channel Start State after we ask the channel to stop
    ChannelExtension->StartState.ChannelNextStartState = Stopped;

    if (supportsCLO) {
        // AHCI 3.3.7 make sure PxCMD.ST is 0.
        for (i = 1; i < 101; i++) {
            cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &Px->CMD.AsUlong);
            if (cmd.ST == 0) {
                break;
            }
            StorPortStallExecution(5000);  //5 milliseconds
        }
        if (i == 101) {
            RecordExecutionHistory(ChannelExtension, 0x10820011); //P_NotRunning, After 500ms of writing 0 to PxCMD.ST, it's still 1.
        }

        tfd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->TFD.AsUlong);
        if (tfd.STS.BSY) {
          // AHCI 3.3.7 Command List Override (CLO):  Setting this bit to ‘1’ causes PxTFD.STS.BSY and PxTFD.STS.DRQ to be cleared to ‘0’.  
          // This allows a software reset to be transmitted to the device regardless of whether the BSY and DRQ bits are still set in the PxTFD.STS register. 
          
          // Do this to make sure the port can stop
            cmd.CLO = 1;
            StorPortWriteRegisterUlong(adapterExtension, &Px->CMD.AsUlong, cmd.AsUlong);
        }
    }

  //1.2 Verify CR cleared
    //AHCI 10.1.2 - 3: wait cmd.CR to be 0. Software should wait at least 500 milliseconds for this to occur.    
    for (i = 1; i < 101; i++) {
        cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &Px->CMD.AsUlong);
        if ( (cmd.CR == 0) && (cmd.ST == 0) ) {
            break;
        }
        StorPortStallExecution(5000);  //5 milliseconds
    }

    //AHCI 10.4.2 If PxCMD.CR or PxCMD.FR do not clear to ‘0’ correctly, then software may attempt a port reset or a full HBA reset to recover.
    if (i == 101) {
        RecordExecutionHistory(ChannelExtension, 0x10350011); // P_NotRunning, PxCMD.CR or PxCMD.FR do not clear to ‘0’ correctly
        return FALSE;
    }

  //1.3 Verify CI cleared
    //This must have the effect of clearling CI as per AHCI section 3.3.14
    for (i = 1; i < 101; i++) {
        ci = StorPortReadRegisterUlong(adapterExtension, &Px->CI);
        if (ci == 0) {
            break;
        }
        StorPortStallExecution(50);  //50 microseconds
    }

    //If CI does not clear to ‘0’ correctly abort the stop
    if (i == 101) {
        RecordExecutionHistory(ChannelExtension, 0x10360011); // P_NotRunning, CI does not clear to ‘0’ 
        return FALSE;
    }

  //2.1 Clear CMD.FRE
    cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &Px->CMD.AsUlong);
    if( (cmd.FRE | cmd.FR) != 0 ) {
        //If PxCMD.FRE is set to ‘1’, software should clear it to ‘0’ and wait at least 500 milliseconds for PxCMD.FR to return ‘0’ when read.  
        //AHCI 10.3.2 Software shall not clear this bit while PxCMD.ST or PxCMD.CR is set to ‘1’.
        cmd.FRE = 0;
        StorPortWriteRegisterUlong(adapterExtension, &Px->CMD.AsUlong, cmd.AsUlong);

        if (supportsCLO) {
            tfd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->TFD.AsUlong);
            if (tfd.STS.BSY) {
                NT_ASSERT(FALSE);     //don't expect BSY is set again
                cmd.CLO = 1;
                StorPortWriteRegisterUlong(adapterExtension, &Px->CMD.AsUlong, cmd.AsUlong);
            }
        }

        //Software should wait at least 500 milliseconds for this to occur.  
        for (i = 1; i < 101; i++) {
            cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &Px->CMD.AsUlong);
            if( (cmd.CR == 0) && (cmd.FR == 0) && (cmd.ST == 0) && (cmd.FRE == 0) ) {
                break;
            }
            StorPortStallExecution(5000);    //  5 milliseconds
        }

        if ( i == 101 ) {  //If PxCMD.CR or PxCMD.FR do not clear to ‘0’ correctly, then software may attempt a port reset or a full HBA reset to recover.
            if (supportsCLO) {
                RecordExecutionHistory(ChannelExtension, 0x10380011); // P_NotRunning, PxCMD.CR or PxCMD.FR do not clear to ‘0’, CLO enabled
            } else {
                RecordExecutionHistory(ChannelExtension, 0x10370011); // P_NotRunning, PxCMD.CR or PxCMD.FR do not clear to ‘0’, CLO not enabled
            }
            return FALSE;
        }
    }

  //2.2 wait for CLO to clear
    // AHCI 3.3.7 Software must wait for CLO to be cleared to ‘0’ before setting PxCMD.ST to ‘1’.
    // register bit CLO might be set in this function, and it should have been cleared before function exit.
    if ( supportsCLO && (cmd.CLO == 0) ) {
        for (i = 1; i < 101; i++) {
            cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &Px->CMD.AsUlong);
            if( (cmd.CLO == 0) ) {
                break;
            }
            StorPortStallExecution(5000);    //  5 milliseconds
        }

        if ( i == 101 ) {
            RecordExecutionHistory(ChannelExtension, 0x10390011); // P_NotRunning, PxCMD.CLO not clear to ‘0’, CLO enabled
            return FALSE;
        }
    }

    RecordExecutionHistory(ChannelExtension, 0x10000011); //Exit P_NotRunning, port stopped

    return TRUE;
}


VOID
AhciAdapterRunAllPorts(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    )
{
    ULONG i;

    //only start with the first one. callback routine will go over all ports.
    for (i = 0; i <= AdapterExtension->HighestPort; i++) {
        if (AdapterExtension->PortExtension[i] != NULL) {
            AdapterExtension->InRunningPortsProcess = TRUE;
            P_Running_StartAttempt(AdapterExtension->PortExtension[i], FALSE);
            return;
        }
    }
    
    return;
}

VOID
RunNextPort(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
{
    PAHCI_ADAPTER_EXTENSION adapterExtension;
    PAHCI_CHANNEL_EXTENSION nextPortExtension;
    ULONG i;

    adapterExtension = ChannelExtension->AdapterExtension;
    nextPortExtension = NULL;

    //starting all ports process either not started or already completed.
    if (!adapterExtension->InRunningPortsProcess) {
        return;
    }

    //get next port extension
    for (i = (ChannelExtension->PortNumber + 1); i <= adapterExtension->HighestPort; i++) {
        if (adapterExtension->PortExtension[i] != NULL) {
            nextPortExtension = adapterExtension->PortExtension[i];
            break;
        }
    }

    // run next port or declare the ports running process completed.
    if (nextPortExtension != NULL) {
        if (nextPortExtension->StartState.ChannelNextStartState == 0) {
            P_Running_StartAttempt(nextPortExtension, FALSE);
        } else {
            // return, do nothing here as the Port Start effort has been made.
            // RunNextPort() is only used in AhciAdapterRunAllPorts() which is one time operation from AhciHwFindAdapter.
            // In other cases, P_Running_StartAttempt() will be called directly for each port.
            return;
        }
    } else {
        //starting all ports process is completed.
        adapterExtension->InRunningPortsProcess = FALSE;
    }

    return;     
}

VOID
P_Running_StartAttempt(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in BOOLEAN AtDIRQL
    )
/*    

Called By:
It assumes:
    Channel Start may already be in progress (not function reentrant)
It performs:
    1 Initializes the Channel Start state machine
    2 Starts the Channel Start state machine

Affected Variables/Registers:
    none
*/
{
  //1 Initializes the Channel Start state machine
    ChannelExtension->StartState.ChannelNextStartState = WaitOnDET;
    ChannelExtension->StartState.ChannelStateDETCount = 0;
    ChannelExtension->StartState.ChannelStateDET1Count = 0;
    ChannelExtension->StartState.ChannelStateDET3Count = 0;
    ChannelExtension->StartState.ChannelStateFRECount = 0;
    ChannelExtension->StartState.ChannelStateBSYDRQCount = 0;
    ChannelExtension->StartState.AtDIRQL = AtDIRQL ? 1 : 0;
  //2 Starts the Channel Start state machine
    P_Running(ChannelExtension, NULL);
    return;
}

BOOLEAN
P_Running(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in_opt PVOID Context
    )
/*
    The purpose of this function is to verify and drive the Start Channel state machine

Called By:
    P_Running_StartAttempt

    Through StorPortRequestTimer which is set up by each of the Start Channel state functions:
            P_Running_WaitOnDET
            P_Running_WaitWhileDET1
            P_Running_WaitOnDET3
            P_Running_WaitOnFRE
            P_Running_WaitOnBSYDRQ
It assumes:
    Nothing

It performs:
    (overview)
    1 Verify Start can/needs to be done
    2 Run the Start Channel state machine

    (details)
    1.1 First, is the channel initialized.  Would turning start on blow up the machine?
    1.2 Next, is the port somehow already running?
    1.3 Make sure the device knows it is supposed to be spun up
    1.4 Check to make sure that FR and CR are both 0.  Attempt to bring the controller into a consistent state by stopping the controller. 
    1.5 CMD.ST has to be set, when that happens check to see that PxSSTS.DET is not ‘4h’

    2.1 Dispatch to the current state (states are responsible for selecting the next state)

Affected Variables/Registers:
    CMD 

Return Values:
    TRUE if the state machine can be run
    FALSE if the state machine can't
*/
{

    AHCI_COMMAND            cmd;
    AHCI_SERIAL_ATA_STATUS  ssts;
    PAHCI_PORT              px = ChannelExtension->Px;

    PAHCI_ADAPTER_EXTENSION adapterExtension = ChannelExtension->AdapterExtension;

    UNREFERENCED_PARAMETER(Context);

    if (LogExecuteFullDetail(adapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x00000015);//P_Running
    }

  //1.1 First, is the channel initialized.  Would turning start on blow up the machine?
    if( ChannelExtension->StateFlags.StartCapable == FALSE ) {
        RecordExecutionHistory(ChannelExtension, 0x10120015);//No Channel Resources
        RunNextPort(ChannelExtension);
        return FALSE;
    }

  //1.2 Next, is the port somehow already running?
    cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &px->CMD.AsUlong);
    if( (cmd.ST == 1) && (cmd.CR == 1) && (cmd.FRE == 1) && (cmd.FR == 1) ) {
        ChannelExtension->StartState.ChannelNextStartState = StartComplete;
        if (LogExecuteFullDetail(adapterExtension->LogFlags)) {
            RecordExecutionHistory(ChannelExtension, 0x30000015);//Channel Already Running
        }
        
        RunNextPort(ChannelExtension);
        return TRUE;
    }
    
  //1.3 Make sure the device knows it is supposed to be spun up
    cmd.SUD = 1;
    StorPortWriteRegisterUlong(adapterExtension, &px->CMD.AsUlong, cmd.AsUlong);

  //1.4 Check to make sure that FR and CR are both 0.  If not then ST and/or FRE are 0 which a bad scenario.  
    if ( ( (cmd.FR == 1) && (cmd.FRE == 0) ) || 
         ( (cmd.CR == 1) && (cmd.ST  == 0) ) ) {
        //Attempt to bring the controller into a consistent state by stopping the controller. 
        if ( !P_NotRunning(ChannelExtension, ChannelExtension->Px) ) {
            RecordExecutionHistory(ChannelExtension, 0x10880015);   //CR or FR 1 when ST or FRE 0
            AhciCOMRESET(ChannelExtension, ChannelExtension->Px);   //Issue COMRESET to recover
        }
    }

  //1.5 CMD.ST has to be set, when that happens check to see that PxSSTS.DET is not ‘4h’
    ssts.AsUlong = StorPortReadRegisterUlong(adapterExtension, &px->SSTS.AsUlong);
    if( ssts.DET == 0x4) {
        P_Running_StartFailed(ChannelExtension);
        RecordExecutionHistory(ChannelExtension, 0x10150015);   //Channel Disabled
        return FALSE;
    }

  //2.1 Ok, run the Start Channel State Machine
    switch(ChannelExtension->StartState.ChannelNextStartState) {

        case WaitOnDET:
            P_Running_WaitOnDET(ChannelExtension);
            break;
        case WaitWhileDET1:
            P_Running_WaitWhileDET1(ChannelExtension);
            break;
        case WaitOnDET3:
            P_Running_WaitOnDET3(ChannelExtension);
            break;
        case WaitOnFRE:
            P_Running_WaitOnFRE(ChannelExtension);
            break;
        case WaitOnBSYDRQ:
            P_Running_WaitOnBSYDRQ(ChannelExtension);
            break;
        default:
            break;
    }

    // record this function is called.
    RecordExecutionHistory(ChannelExtension, 0x10000015);   //Exit P_Running
    return TRUE;
}

VOID
P_Running_WaitOnDET(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
/*   
    Search for Device Activity phase.  Use DET and IPM for any signs of life as defined in AHCI 1.2 section 10.1.2 and 10.3.1.
    Polled 3 times in 30ms

Called By:
    P_Running
It assumes:
    Controller is start capable 
It performs:
    (overview)
    1 Look for signs of life while DET == 0
    2 Look for signs of life while DET == 1
    3 Look for signs of life which is DET == 3
    (details)
    1.1 When looking for device presence, seeing link level power managment shall count as device present as per 10.3.1
    1.2 Wait for 1 second for DET to show signs of life
    1.3 After 30ms give up
    2.1 When looking for device presence, seeing link level power managment shall count as device present as per 10.3.1
    2.2 Otherwise continue on to WaitOnDET1
    3.1 If DET==3 we are ready for WaitOnFRE already.

Affected Variables/Registers:
    none
*/
{
    AHCI_SERIAL_ATA_STATUS ssts;

    PAHCI_ADAPTER_EXTENSION adapterExtension = ChannelExtension->AdapterExtension;

WaitOnDET_Start:
    if (LogExecuteFullDetail(adapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x00000016);//P_Running_WaitOnDET
    }

  //1 Look for signs of life while DET == 0
    ssts.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->SSTS.AsUlong);

    if (ssts.DET == 0)  { //When a COMRESET is sent to the device the PxSSTS.DET field shall be cleared to 0h.
      
      //1.1 When looking for device presence, seeing link level power managment shall count as device present as per 10.3.1
        if ((ssts.IPM == 0x2) || (ssts.IPM == 0x6)) {
            ChannelExtension->StartState.ChannelNextStartState = WaitOnFRE;
            RecordExecutionHistory(ChannelExtension, 0x10800016);//P_Running_WaitOnDET is 0 w/ LPM activity, goto FRE
            P_Running_WaitOnFRE(ChannelExtension);
            return;
      //1.3 After 30ms give up (spec requires 10ms, use 30ms for safety). There is no device connected.
        //from SATA-IO spec 3.0 - 15.2.2.2:   
        //If a device is present, the Phy shall detect device presence within 10 ms of a power-on reset (i.e. COMINIT shall be returned within 10 ms of an issued COMRESET).
        } else if (ChannelExtension->StartState.ChannelStateDETCount > AHCI_PORT_WAIT_ON_DET_COUNT) {
            P_Running_StartFailed(ChannelExtension);
            RecordExecutionHistory(ChannelExtension, 0x10ff0016);//P_Running_WaitOnDET Timed out, No Device
            return;
      //1.2 Wait for 30ms for DET to show signs of life
        } else {
            ChannelExtension->StartState.ChannelStateDETCount++;
            RecordExecutionHistory(ChannelExtension, 0x10810016);//P_Running_WaitOnDET is 0, still waiting
            
            if (IsDumpMode(adapterExtension)) {
                StorPortStallExecution(10000); //10 milliseconds
                goto WaitOnDET_Start;
            } else {
                AhciRequestTimerCallback(ChannelExtension, P_Running, NULL, 10000, CallbackTypeStartPort);  //10 milliseconds
            }
            return;
        }

    } else {
      //2 Look for signs of life while DET == 1
        if (ssts.DET == 1)  {
          //2.1 When looking for device presence, seeing link level power managment shall count as device present as per 10.3.1
            if ((ssts.IPM == 0x2) || (ssts.IPM == 0x6)) {
                ChannelExtension->StartState.ChannelNextStartState = WaitOnFRE;
                RecordExecutionHistory(ChannelExtension, 0x10a00016);//P_Running_WaitOnDET is 1 w/ LPM activity, goto FRE
                P_Running_WaitOnFRE(ChannelExtension);
                return;
          //2.2 Otherwise continue on to WaitOnDET1
            } else {
                ChannelExtension->StartState.ChannelNextStartState = WaitWhileDET1;
                RecordExecutionHistory(ChannelExtension, 0x10a10016);//P_Running_WaitOnDET is 1
                P_Running_WaitWhileDET1(ChannelExtension);
                return;
            }
      //3.1 If DET==3 we are ready for WaitOnFRE already.
        } else if (ssts.DET == 3)  {
            ChannelExtension->StartState.ChannelNextStartState = WaitOnFRE; 
            RecordExecutionHistory(ChannelExtension, 0x10a20016);//P_Running_WaitOnDET is 3
            P_Running_WaitOnFRE(ChannelExtension);
            return;

        } else {
            P_Running_StartFailed(ChannelExtension);
            RecordExecutionHistory(ChannelExtension, 0x10a30016);//P_Running_WaitOnDET is Bogus, aborting
            return;
        }
    }
}

VOID
P_Running_WaitWhileDET1(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
/*       
    Waiting on establishment of link level communications phase.
    A '1' should become a '3'.  If it doesn't, help it along.
    NOTE:   When a COMINIT is received, the PxSSTS.DET field shall be set to 1h.
            That means a device is detected, but communications has not finished
    Polled 100 times in 1 second

Called By:
    P_Running
It assumes:
    Channel is start capable
    DET was previously the value of 1
It performs:
    (overview)
    1 Look for signs of life which is DET == 3
    2 Look for signs of live while DET == 1
    (details)
    1.1 If DET moves from 1 to 3, go to WaitOnFRE
    2.1 Wait for 1 second for DET to become 3
    2.2 After 1 second of waiting, force the controller to 150MB/s speeds and go to WaitOnDET3
Affected Variables/Registers:
    SCTL, SERR
*/
{
    AHCI_SERIAL_ATA_STATUS  ssts;
    AHCI_SERIAL_ATA_CONTROL sctl;
    AHCI_SERIAL_ATA_ERROR   serr;

    PAHCI_ADAPTER_EXTENSION adapterExtension = ChannelExtension->AdapterExtension;

WaitWhileDET1_Start:
    if (LogExecuteFullDetail(adapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x00000017);//P_Running_WaitWhileDET1
    }

  //1.1 If DET moves from 1 to 3, go to WaitOnFRE
    ssts.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->SSTS.AsUlong);

    if (ssts.DET == 3) {  
        ChannelExtension->StartState.ChannelNextStartState = WaitOnFRE; 
        RecordExecutionHistory(ChannelExtension, 0x10a20017);//P_Running_WaitWhileDET1 done
        P_Running_WaitOnFRE(ChannelExtension);
        return;
 
    } else {
      //2.2 After 1 second of waiting, force the controller to 150MB/s speeds and go to WaitOnDET3
        if (ChannelExtension->StartState.ChannelStateDET1Count > 100) {
            //A very wise woman once taught me this trick
            //it is possible that the device is not handling speed negotation very well
            //help it out by allowing only 150MB/s
            sctl.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->SCTL.AsUlong);
            sctl.SPD = 1;
            StorPortWriteRegisterUlong(adapterExtension, &ChannelExtension->Px->SCTL.AsUlong, sctl.AsUlong);
            //Reset the port
            AhciCOMRESET(ChannelExtension, ChannelExtension->Px);
            //and clear out SERR
            serr.AsUlong = (ULONG)~0;
            StorPortWriteRegisterUlong(adapterExtension, &ChannelExtension->Px->SERR.AsUlong, serr.AsUlong);

            ChannelExtension->StartState.ChannelNextStartState = WaitOnDET3;
            RecordExecutionHistory(ChannelExtension, 0x100a0017);//P_Running_WaitWhileDET1 timed out, speed stepping down
            if (IsDumpMode(adapterExtension)) {
                StorPortStallExecution(10000); //10 milliseconds
                P_Running_WaitOnDET3(ChannelExtension);
            } else {
                AhciRequestTimerCallback(ChannelExtension, P_Running, NULL, 10000, CallbackTypeStartPort);  //10 milliseconds
            }
            return;
      //2.1 Wait for DET to become 3 for 1 second
        } else {
            ChannelExtension->StartState.ChannelStateDET1Count++;
            RecordExecutionHistory(ChannelExtension, 0x10600017);//P_Running_WaitWhileDET1 still waiting
            if (IsDumpMode(adapterExtension)) {
                StorPortStallExecution(10000); //10 milliseconds 
                goto WaitWhileDET1_Start;
            } else {
                AhciRequestTimerCallback(ChannelExtension, P_Running, NULL, 10000, CallbackTypeStartPort);  //10 milliseconds
            }
            return;
        }
    }
}

VOID
P_Running_WaitOnDET3(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
/*    
    Done monkeying phase.  
    From here on out only a DET of 3 will do

Called By:
    P_Running

It assumes:
    Channel is start capable

It performs:
    (overview)
    1 Look for signs of life which are DET == 3
    (details)
    1.1 Wait for 1 second until DET becomes 3
    1.2 After waiting for 1 second, give up on starting the channel
    1.3 If DET==3 we are ready for WaitOnFRE

Affected Variables/Registers:
    none
*/
{
    AHCI_SERIAL_ATA_STATUS  ssts;
    PAHCI_ADAPTER_EXTENSION adapterExtension = ChannelExtension->AdapterExtension;

WaitOnDET3_Start:
    if (LogExecuteFullDetail(adapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x00000018);//P_Running_WaitOnDET3
    }

  //1.2 After waiting for 1 second, give up on starting the channel
    ssts.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->SSTS.AsUlong);

    if (ssts.DET != 3) { 

        if (ChannelExtension->StartState.ChannelStateDET3Count > 100) {
            P_Running_StartFailed(ChannelExtension);
            RecordExecutionHistory(ChannelExtension, 0x10ff0018);//P_Running_WaitOnDET3 timed out
            return;
      //1.1 Wait for 1 second until DET becomes 3
        } else {
            ChannelExtension->StartState.ChannelStateDET3Count++;
            RecordExecutionHistory(ChannelExtension, 0x10080018);//P_Running_WaitOnDET3 still waiting
            if (IsDumpMode(adapterExtension)) {
                StorPortStallExecution(10000); //10 milliseconds 
                goto WaitOnDET3_Start;
            } else {
                AhciRequestTimerCallback(ChannelExtension, P_Running, NULL, 10000, CallbackTypeStartPort);  //10 milliseconds
            }
            return;
        }
  //1.3 If DET==3 we are ready for WaitOnFRE
    } else {
        ChannelExtension->StartState.ChannelNextStartState = WaitOnFRE;
        RecordExecutionHistory(ChannelExtension, 0x100a0018);//P_Running_WaitOnDET3 Success
        P_Running_WaitOnFRE(ChannelExtension);
        return;
    }
}

VOID
P_Running_WaitOnFRE(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
/*    
    Start the Receive buffer phase
    Wait for confirmation is a 'nice to have' but isn't necessary.

Called By:
    P_Running

It assumes:
    DET == 3 or IPM == 02 or 06

It performs:
    (overview)
    1 Set FRE
    2 Wait for 50ms for FR to reflect running status
    3 Move to WaitOnBSYDRQ
    (details)
    1.1 Set FRE
    2.1 Wait for 50ms for FR to reflect running status
    3.1 Move to WaitOnBSYDRQ
Affected Variables/Registers:
    CMD
*/
{
    AHCI_COMMAND cmd;

    PAHCI_ADAPTER_EXTENSION adapterExtension = ChannelExtension->AdapterExtension;

WaitOnFRE_Start:
    if (LogExecuteFullDetail(adapterExtension->LogFlags)) {
        RecordExecutionHistory(ChannelExtension, 0x00000019);//P_Running_FRE
    }

    // There is a device connected when we get here. Trace how long the port need to set DET from 0 to 1.
    if (ChannelExtension->StartState.ChannelStateDETCount > 1) {
        DebugPrint((1, "STORAHCI: Channel %u: waited more than %u ms for DET 0 -> 1\n", 
                      ChannelExtension->PortNumber, 
                      10 * ChannelExtension->StartState.ChannelStateDETCount));
    }

  //1.1 Set FRE 
    cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->CMD.AsUlong);
    cmd.FRE = 1;
    StorPortWriteRegisterUlong(adapterExtension, &ChannelExtension->Px->CMD.AsUlong, cmd.AsUlong);
  
    cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->CMD.AsUlong);

    if (cmd.FR == 1) {
      //3.1 Move to WaitOnBSYDRQ  
        ChannelExtension->StartState.ChannelNextStartState = WaitOnBSYDRQ;
        RecordExecutionHistory(ChannelExtension, 0x000a0019);//P_Running_WaitOnFRE Success
        P_Running_WaitOnBSYDRQ(ChannelExtension);
        return;

    } else {
      //3.1 Move to WaitOnBSYDRQ
        if (ChannelExtension->StartState.ChannelStateFRECount > 5) {
            ChannelExtension->StartState.ChannelNextStartState = WaitOnBSYDRQ;
            RecordExecutionHistory(ChannelExtension, 0x00ff0019);//P_Running_WaitOnFRE timed out
            P_Running_WaitOnBSYDRQ(ChannelExtension);
            return;
      //2.1 Wait for 50ms for FR to reflect running status
        } else {
            ChannelExtension->StartState.ChannelStateFRECount++;
            RecordExecutionHistory(ChannelExtension, 0x00080019);//P_Running_WaitOnFRE still waiting
            if (IsDumpMode(adapterExtension)) {
                StorPortStallExecution(10000); //10 milliseconds 
                goto WaitOnFRE_Start;
            } else {
                AhciRequestTimerCallback(ChannelExtension, P_Running, NULL, 10000, CallbackTypeStartPort);  //10 milliseconds
            }
            return; 
        }
    }   
}

VOID
P_Running_WaitOnBSYDRQ(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
/*    
    Home stretch

Called By:
    P_Running
It assumes:
    DET == 3 or IPM == 02 or 06
It performs:
    (overview)
    1 Enable BSY and DRQ to go to 0
    2 Wait for BSY and DRQ to go to 0
    3 Set ST to 1
    (details)
    1.1 Clear serr.DIAG.X
    2.1 Wait for the rest of the 10 seconds for BSY and DRQ to clear
    2.2 After a total amount of 3 seconds working on the start COMRESET
    2.3 After waiting for the rest of the 4 seconds for BSY and DRQ to clear, give up
    3.1 Set ST to 1
Affected Variables/Registers:

*/
{
    AHCI_COMMAND            cmd;
    AHCI_TASK_FILE_DATA     tfd;
    AHCI_SERIAL_ATA_ERROR   serr;
    USHORT                  totalstarttime;

    PAHCI_ADAPTER_EXTENSION adapterExtension = ChannelExtension->AdapterExtension;

WaitOnBSYDRQ_Start:
    RecordExecutionHistory(ChannelExtension, 0x0000001a);//P_Running_WaitOnBSYDRQ
    tfd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->TFD.AsUlong);

  //1.1 Enable BSY and DRQ to go to 0
    if ( (tfd.STS.BSY) || (tfd.STS.DRQ) ) {
        //When [serr.DIAG.X is] set to one this bit indicates a COMINIT signal was received.  This bit is reflected in the P0IS.PCS bit.
        //to allow the TFD to be updated serr.DIAG.X must be cleared.
        serr.AsUlong = 0;
        serr.DIAG.X = 1;
        StorPortWriteRegisterUlong(adapterExtension, &ChannelExtension->Px->SERR.AsUlong, serr.AsUlong);
    }
  //3.1 Set ST to 1
    if ( ( tfd.STS.BSY == 0) && ( tfd.STS.DRQ == 0) ) {
        STOR_LOCK_HANDLE    lockhandle = {0};

      //We made it!  Set ST and start the IO we have collected!
        cmd.AsUlong = StorPortReadRegisterUlong(adapterExtension, &ChannelExtension->Px->CMD.AsUlong);
        cmd.ST = 1;
        StorPortWriteRegisterUlong(adapterExtension, &ChannelExtension->Px->CMD.AsUlong, cmd.AsUlong);

        ChannelExtension->StartState.ChannelNextStartState = StartComplete;
        ChannelExtension->StateFlags.IgnoreHotplugDueToResetInProgress = FALSE;

      //3.2 Enable Interrupts on the channel (AHCI 1.1 Section 10.1.2 - 7)
      //We already cleaned up PxIS and ControllerIS in AhciPortInitialize()
        Set_PxIE(ChannelExtension, &ChannelExtension->Px->IE);

        //Start requests on this Port
        if (ChannelExtension->StartState.AtDIRQL == 0) {
            StorPortAcquireSpinLock(adapterExtension, InterruptLock, NULL, &lockhandle);
        }

        AhciGetNextIos(ChannelExtension, TRUE);

        if (ChannelExtension->StartState.AtDIRQL == 0) {
            StorPortReleaseSpinLock(adapterExtension, &lockhandle);
        }

        RecordExecutionHistory(ChannelExtension, 0x100a001a);//Exit P_Running_WaitOnBSYDRQ Succeeded

        RunNextPort(ChannelExtension);

        return;

    } else {
      //2.3 After waiting for the remainder of the 60 second maximum Channel Start time for BSY and DRQ to clear, give up
      //Some big HDDs takes close to 20 second to spin up
        ULONG portStartTimeoutIn10MS = (AHCI_PORT_START_TIMEOUT_IN_SECONDS * 100);

        // calculate the total time in unit of 10 ms.
        totalstarttime = ChannelExtension->StartState.ChannelStateDETCount  +
                         ChannelExtension->StartState.ChannelStateDET1Count +
                         ChannelExtension->StartState.ChannelStateDET3Count +
                         ChannelExtension->StartState.ChannelStateFRECount  +
                        (ChannelExtension->StartState.ChannelStateBSYDRQCount * 2);

        if ( totalstarttime > portStartTimeoutIn10MS ) {
            P_Running_StartFailed(ChannelExtension);
            RecordExecutionHistory(ChannelExtension, 0x00ff001a);//P_Running_WaitOnBSYDRQ timed out
           return;
      //2.2 After a total amount of 1 second working on the start COMRESET
        } else if ( ChannelExtension->StartState.ChannelStateBSYDRQCount == 50 ){
            //Stop FRE,FR in preperation for RESET
            if ( !P_NotRunning(ChannelExtension, ChannelExtension->Px) ){
                //It takes 1/2 second for stop to fail.  
                //This is taking way too long and the controller is not responding properly. Abort the start.
                P_Running_StartFailed(ChannelExtension);
                RecordExecutionHistory(ChannelExtension, 0x00fc001a);//P_Running_WaitOnBSYDRQ Stop Failed on COMRESET
                return;
            }

            //All set, bring down the hammer
            AhciCOMRESET(ChannelExtension, ChannelExtension->Px);

            //Set the timers for time remaining.  This is a best case scenario and the best that can be offered.
            ChannelExtension->StartState.ChannelStateDETCount = 0;
            ChannelExtension->StartState.ChannelStateDET1Count = 0;
            ChannelExtension->StartState.ChannelStateDET3Count = 0;     // won't exceed 100 * 10ms for 1 second
            ChannelExtension->StartState.ChannelStateFRECount = 0;      // won't exceed 5 * 10ms for 50 ms
            ChannelExtension->StartState.ChannelStateBSYDRQCount = 51;  // won't exceed 3000 * 20ms for 60 seconds minus what DET3 and FRE use.

            //Go back to WaitOnDet3
            ChannelExtension->StartState.ChannelNextStartState = WaitOnDET3;
            RecordExecutionHistory(ChannelExtension, 0x00fd001a);//P_Running_WaitOnBSYDRQ crossing 1 second.  COMRESET done, going to WaitOnDET3
            P_Running_WaitOnDET3(ChannelExtension);
            return;
      //2.1 Wait for the rest of the 4 seconds for BSY and DRQ to clear      
        } else {
            ChannelExtension->StartState.ChannelStateBSYDRQCount++;
            RecordExecutionHistory(ChannelExtension, 0x0008001a);//P_Running_WaitOnBSYDRQ still waiting
            if (IsDumpMode(adapterExtension)) {
                StorPortStallExecution(20000); //20 milliseconds 
                goto WaitOnBSYDRQ_Start;
            } else {
                AhciRequestTimerCallback(ChannelExtension, P_Running, NULL, 20000, CallbackTypeStartPort);  //20 milliseconds
            }
            return;
        }
    }
}

VOID
P_Running_StartFailed(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
/*    

Called By:
    P_Running_WaitOnDET
    P_Running_WaitOnDET3
    P_Running_WaitOnBSYDRQ

It assumes:
    nothing

It performs:
    (overview)
    1 clean up internal state structures
    2 complete all the commands that showed up while the start channel failed
    (details)
    1.1 update state machines
    1.2 clear out the programmed slots
    2.1 complete all outstanding commands

Affected Variables/Registers:
    none
*/
{
    STOR_LOCK_HANDLE    lockhandle = {0};

  //1.1 update state machines
    ChannelExtension->StartState.ChannelNextStartState = StartFailed;
    ChannelExtension->StateFlags.IgnoreHotplugDueToResetInProgress = FALSE;
  //1.2 clear out the programmed slots
    ChannelExtension->SlotManager.CommandsToComplete = GetOccupiedSlots(ChannelExtension);
    ChannelExtension->SlotManager.CommandsIssued = 0;
    ChannelExtension->SlotManager.NCQueueSlice = 0;
    ChannelExtension->SlotManager.NormalQueueSlice = 0;
    ChannelExtension->SlotManager.SingleIoSlice = 0;
    ChannelExtension->SlotManager.HighPriorityAttribute = 0;
  
  //1.3 Enable Interrupts on the Channel (AHCI 1.1 Section 10.1.2 - 7)
  //We already cleaned up PxIS and ControllerIS in AhciPortInitialize()
    Set_PxIE(ChannelExtension, &ChannelExtension->Px->IE);

  //2.1 Call AhciGetNextIos to complete all outstanding commands now that ChannelNextStartState is StartFailed
    if (ChannelExtension->StartState.AtDIRQL == 0) {
        StorPortAcquireSpinLock(ChannelExtension->AdapterExtension, InterruptLock, NULL, &lockhandle);
    }

    AhciPortFailAllIos(ChannelExtension, SRB_STATUS_NO_DEVICE, TRUE);

    if (ChannelExtension->StartState.AtDIRQL == 0) {
        StorPortReleaseSpinLock(ChannelExtension->AdapterExtension, &lockhandle);
    }

    RunNextPort(ChannelExtension);

    return;
}

VOID
AhciNonQueuedErrorRecovery(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
  )
{
/*
AHCI 1.1 Section 6.2.2.1
"The flow for system software to recover from an error when non-queued commands are issued is as follows:
    Reads PxCI to see which commands are still outstanding
    Reads PxCMD.CCS to determine the slot that the HBA was processing when the error occurred
    Clears PxCMD.ST to ‘0’ to reset the PxCI register, waits for PxCMD.CR to clear to ‘0’
    Clears any error bits in PxSERR to enable capturing new errors. 
    Clears status bits in PxIS as appropriate
    If PxTFD.STS.BSY or PxTFD.STS.DRQ is set to ‘1’, issue a COMRESET to the device to put it in an idle state
    Sets PxCMD.ST to ‘1’ to enable issuing new commands    

It assumes:
    Called asynchronously

Called by:
    WorkerDispatch <-- AhciHwInterrupt

It performs:
    (overview)
    1 Recover from an error
    2 Complete the Failed Command
    3 Complete Succeeded Commands

    (details)
    1.1 Initialize
    1.2 Reads PxCI to see which commands are still outstanding
    1.3 Reads PxCMD.CCS to determine the slot that the HBA was processing when the error occurred
    1.4.1 Clears PxCMD.ST to ‘0’ to reset the PxCI register, waits for PxCMD.CR to clear to ‘0’
     1.5 Clears any error bits in PxSERR to enable capturing new errors.
     1.6 Clears status bits in PxIS as appropriate
    1.4.2 If PxTFD.STS.BSY or PxTFD.STS.DRQ is set to ‘1’, issue a COMRESET to the device to put it in an idle state
    1.4.3 If a COMRESET was issued, restore Preserved Settings
    1.4.4 Start the channel
    
    2.1 Complete the command being issued if there was one
    2.2 Restore the unsent programmed commands for careful reprocessing
    2.3 If a request sense SRB was created due to this error

    3.1 If there were commands that are ready to complete, complete them now

Affected Variables/Registers:
    CI, CMD, TFD
    Channel Extension
*/
    BOOLEAN performedCOMRESET;
    ULONG   ci;
    ULONG   localCommandsIssued;
    UCHAR   numberCommandsOutstanding;
    ULONG   failingCommand;

    AHCI_COMMAND        cmd;
    AHCI_TASK_FILE_DATA tfd;
    PSCSI_REQUEST_BLOCK senseSrb;
    PAHCI_SRB_EXTENSION srbExtension;
    
  //1.1 Initialize variables
    RecordExecutionHistory(ChannelExtension, 0x00000013);//AhciNonQueuedErrorRecovery
    
    senseSrb = NULL;
    performedCOMRESET = FALSE;

  //1.2 Reads PxCI to see which commands are still outstanding
    ci = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CI);

  //1.3 Reads PxCMD.CCS to determine the slot that the HBA was processing when the error occurred
    cmd.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->CMD.AsUlong);

  //1.4 Clears PxCMD.ST to ‘0’ to reset the PxCI register, waits for PxCMD.CR to clear to ‘0’
    if ( !P_NotRunning(ChannelExtension, ChannelExtension->Px) ){ //This clears PxCI
        RecordExecutionHistory(ChannelExtension, 0x10160013);//AhciNonQueuedErrorRecovery, Port Stop Failed
        AhciPortReset(ChannelExtension);
        return;
    }
    
  //2.1 Complete the command being issued if there was one
        
    //Determine how many commands are outstanding
    localCommandsIssued = ChannelExtension->SlotManager.CommandsIssued;
    numberCommandsOutstanding = 0;
    failingCommand = 0;
    while (localCommandsIssued) {
        if (localCommandsIssued & 1) {
            numberCommandsOutstanding++;
        }
        //To keep from having to walk through the localCommandsIssued mask again, 
        //if there is only 1 command, failing commands will be 1 greater than that slot after this while loop.
        failingCommand++;
        localCommandsIssued >>= 1;
    }
    
    //If there is only one command outstanding, then we know which one to complete.
    if (numberCommandsOutstanding == 1) {
        failingCommand--;
        //As long as there is an SRB in that command slot.
        if (ChannelExtension->Slot[failingCommand].Srb == NULL) {
            RecordExecutionHistory(ChannelExtension, 0x10170013);//AhciNonQueuedErrorRecovery, didn't find the expected command from slot.
            AhciPortReset(ChannelExtension);
            return;
        }
    //If NonQueuedErrorRecovery should not be done for more than 1 command stop here.
    } else if ( (cmd.ST == 1) && AdapterNoNonQueuedErrorRecovery(ChannelExtension->AdapterExtension) ) {
        RecordExecutionHistory(ChannelExtension, 0x10180013);//AhciNonQueuedErrorRecovery, NCQ_NeverNonQueuedErrorRecovery
        AhciPortReset(ChannelExtension);
        return;
    } else {
        //otherwise the failing command is in CCS, as long as there is an SRB in that command slot.
        if( ((ChannelExtension->SlotManager.CommandsIssued & (1 << cmd.CCS)) > 0) && 
            (ChannelExtension->Slot[cmd.CCS].Srb) ) {
            failingCommand = cmd.CCS;
        } else {
            RecordExecutionHistory(ChannelExtension, 0x10190013);//AhciNonQueuedErrorRecovery, command to be failed is not indicated by PxCMD.CCS
            AhciPortReset(ChannelExtension);
            return;
        }
    }
    
    tfd.AsUlong = StorPortReadRegisterUlong(ChannelExtension->AdapterExtension, &ChannelExtension->Px->TFD.AsUlong);

    // Keep in mind although Px.CMD.ST was just shut off, we are working with the previous snapshot of ST.
    if (cmd.ST == 1) {
        srbExtension = GetSrbExtension(ChannelExtension->Slot[failingCommand].Srb);
        // Remove the errant command from the Commands Issued.
        ci &= ~(1 << failingCommand);
        // Move the errant command from Issued list to the 'to complete' list
        ChannelExtension->SlotManager.CommandsIssued &= ~(1 << failingCommand); 
        ChannelExtension->SlotManager.HighPriorityAttribute &= ~(1 << failingCommand);
        ChannelExtension->SlotManager.CommandsToComplete |= (1 << failingCommand);
        // Fill in the status of the command
        
        srbExtension->AtaStatus = tfd.STS.AsUchar;
        srbExtension->AtaError = tfd.ERR;
        ChannelExtension->Slot[failingCommand].Srb->SrbStatus = SRB_STATUS_ERROR;
        // Handle Request Sense if needed
        if ( NeedRequestSense(ChannelExtension->Slot[failingCommand].Srb) ) {
            // This is for ATAPI device only
            senseSrb = BuildRequestSenseSrb(ChannelExtension, ChannelExtension->Slot[failingCommand].Srb); // it will be programmed to slot later in this routine.
            if (senseSrb != NULL) {
                //Make the slot available again now, its slot content will not be cleaned up later.
                ChannelExtension->Slot[failingCommand].CmdHeader = NULL;
                ChannelExtension->Slot[failingCommand].CommandHistoryIndex = 0;
                ChannelExtension->Slot[failingCommand].Srb = NULL;
                //Do not complete this command normally, it will be completed when it's request sense is completed
                ChannelExtension->SlotManager.CommandsToComplete &= ~(1 << failingCommand);
                RecordExecutionHistory(ChannelExtension, 0x10730013);//NonNCQ Error Received RequestSense
            }
        }
        
        if (senseSrb == NULL) {
            // if not need Request Sense or cannot get Reqeust Sense Srb, Complete the command manually  
            ReleaseSlottedCommand(ChannelExtension, (UCHAR)failingCommand, TRUE); //&ChannelExtension->Slot[cmd.CCS] );
            RecordExecutionHistory(ChannelExtension, 0x10630013);//NonNCQ Error Completed failed command
        }
    }

    //1.5 Clears any error bits in PxSERR to enable capturing new errors. 
        //Handled in the Interrupt routine
    //1.6 Clears status bits in PxIS as appropriate
        //Handled in the Interrupt routine

    //1.4.2 If PxTFD.STS.BSY or PxTFD.STS.DRQ is set to ‘1’, issue a COMRESET to the device to put it in an idle state
    if(tfd.STS.BSY || tfd.STS.DRQ) {
        AhciCOMRESET(ChannelExtension, ChannelExtension->Px);
        performedCOMRESET = TRUE;
    }

    //2.2 Restore the unsent programmed commands for careful reprocessing
    ChannelExtension->SlotManager.NormalQueueSlice |= ci;   //put the commands that didn't get a chance to finish back into the normal queue
    ChannelExtension->SlotManager.CommandsIssued &= ~ci;    //Remove the unfinished commands from the 'issued' list

    //2.3 If there were commands that are ready to complete, complete them
    if (ChannelExtension->SlotManager.CommandsToComplete) {
        AhciCompleteIssuedSRBs(ChannelExtension, SRB_STATUS_SUCCESS, TRUE); //complete the successful commands to start the RS SRB.
    }

    //3.1 If a request sense SRB was created due to this error
    if (senseSrb != NULL) {
        ChannelExtension->StateFlags.QueuePaused = FALSE;
        AhciProcessIo(ChannelExtension, senseSrb, TRUE);  //program Sense Srb into Slot
    }

    //3.2 If a COMRESET was issued, restore Preserved Settings
    if (performedCOMRESET == TRUE) {
        RestorePreservedSettings(ChannelExtension, TRUE);
    }

    //3.3 Start the channel, IO will be resumed after port start complete.
    P_Running_StartAttempt(ChannelExtension, TRUE);

    RecordExecutionHistory(ChannelExtension, 0x10000013);//Exit AhciNonQueuedErrorRecovery
}

VOID
AhciPortErrorRecovery(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
  )
/*
    Error recovery routine for Port
    AHCI: 6.2.2.1 Non-Queued Error Recovery 
          6.2.2.2 Native Command Queuing Error Recovery 
          6.2.2.3 Recovery of Unsolicited COMINIT

It assumes:
    nothing

Called by:  
    AhciHwInterrupt

Return Values:
    None
*/
{
    if (ChannelExtension->StateFlags.CallAhciReportBusChange) {
        // Handle AHCI 6.2.2.3 Recovery of Unsolicited COMINIT and hot plug
        // AhciReportBusChange() will issue RESET, ignore other error recovery marks.
        ChannelExtension->StateFlags.CallAhciReportBusChange = FALSE;
        ChannelExtension->StateFlags.CallAhciNonQueuedErrorRecovery = FALSE;
        ChannelExtension->StateFlags.CallAhciReset = FALSE;

        AhciReportBusChange(ChannelExtension);
    }

    if (ChannelExtension->StateFlags.CallAhciReset) {
        // Handle AHCI 6.2.2.2 Native Command Queuing Error Recovery and other events require RESET.
        ChannelExtension->StateFlags.CallAhciNonQueuedErrorRecovery = FALSE;
        ChannelExtension->StateFlags.CallAhciReset = FALSE;

        AhciPortReset(ChannelExtension);
    }

    if (ChannelExtension->StateFlags.CallAhciNonQueuedErrorRecovery) {
        // Handle AHCI 6.2.2.1 Non-Queued Error Recovery 
        ChannelExtension->StateFlags.CallAhciNonQueuedErrorRecovery = FALSE;

        AhciNonQueuedErrorRecovery(ChannelExtension);
    } 

    return;
}

BOOLEAN
AhciPortReset (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    )
/*++
AhciPortReset can be called even if the miniport driver is not ready for another request. 
The miniport driver should complete all pending requests and must reset the given channel.

It assumes:
    nothing

Called by:    

It performs:
    COMRESET along with completing commands to be retried and restoring settings that may be be persistent across a COMRESET
    
    (overview)
    1 Initialize 
    2 Perform COMReset
    3 Complete all outstanding commands
    4 Restore device configuration

Affected Variables/Registers:
    SCTL, CI, SACT
    Channel Extension

Return Values:
    AhciPortReset return TRUE if the reset operation succeeded. 
    If the reset failed the routine must return FALSE. 
--*/
{
  //1.1 Initialize Variables
    RecordExecutionHistory(ChannelExtension, 0x00000050);//AhciPortReset

  //2.1 Stop the channel
    P_NotRunning(ChannelExtension, ChannelExtension->Px);

  //2.2 Perform the COMRESET
    // AHCI 10.1.2 - 3: If PxCMD.CR or PxCMD.FR do not clear to ‘0’ correctly, then software may attempt a port reset or a full HBA reset to recover.
    AhciCOMRESET(ChannelExtension, ChannelExtension->Px);

  //3.1 Complete all outstanding commands for retry
    ChannelExtension->SlotManager.CommandsToComplete = GetOccupiedSlots(ChannelExtension);
    ChannelExtension->SlotManager.CommandsIssued = 0;
    ChannelExtension->SlotManager.NCQueueSlice = 0;
    ChannelExtension->SlotManager.NormalQueueSlice = 0;
    ChannelExtension->SlotManager.SingleIoSlice = 0;
    ChannelExtension->SlotManager.HighPriorityAttribute = 0;

    AhciCompleteIssuedSRBs(ChannelExtension, SRB_STATUS_BUS_RESET, TRUE); //AhciPortReset is under Interrupt spinlock
 
  //4.1 Restore device configuration
    InterlockedBitTestAndReset((LONG*)&ChannelExtension->StateFlags, 3);    //ReservedSlotInUse field is at bit 3
    RestorePreservedSettings(ChannelExtension, TRUE);

  //2.3 Start the channel
    P_Running_StartAttempt(ChannelExtension, TRUE); //AhciPortReset is under Interrupt spinlock

    // record that the channel is reset.
    RecordExecutionHistory(ChannelExtension, 0x10000050);//Exit AhciPortReset

    return TRUE;
}


#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4152)
#pragma warning(default:4214)
#pragma warning(default:4201)
#endif
