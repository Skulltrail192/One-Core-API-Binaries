/**
 *******************************************************************************
 ** Copyright (c) 2011-2012                                                   **
 **                                                                           **
 **   Integrated Device Technology, Inc.                                      **
 **   Intel Corporation                                                       **
 **   LSI Corporation
 **                                                                           **
 ** All rights reserved.                                                      **
 **                                                                           **
 *******************************************************************************
 **                                                                           **
 ** Redistribution and use in source and binary forms, with or without        **
 ** modification, are permitted provided that the following conditions are    **
 ** met:                                                                      **
 **                                                                           **
 **   1. Redistributions of source code must retain the above copyright       **
 **      notice, this list of conditions and the following disclaimer.        **
 **                                                                           **
 **   2. Redistributions in binary form must reproduce the above copyright    **
 **      notice, this list of conditions and the following disclaimer in the  **
 **      documentation and/or other materials provided with the distribution. **
 **                                                                           **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS   **
 ** IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, **
 ** THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR    **
 ** PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR         **
 ** CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL,      **
 ** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       **
 ** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        **
 ** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    **
 ** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      **
 ** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        **
 ** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              **
 **                                                                           **
 ** The views and conclusions contained in the software and documentation     **
 ** are those of the authors and should not be interpreted as representing    **
 ** official policies, either expressed or implied, of Intel Corporation,     **
 ** Integrated Device Technology Inc., or Sandforce Corporation.              **
 **                                                                           **
 *******************************************************************************
**/

/*
 * File: nvmeStd.c
 */

#include "precomp.h"
#ifndef DBG
#include "nvmestd.tmh"
#endif


// Global variables
#ifndef DBG
PVOID TraceContext;
#endif

#ifdef HISTORY
void TracePathSubmit(HISTORY_TAG tag, ULONG queueId, ULONG NSID,
        NVMe_COMMAND_DWORD_0 CDW0, ULONGLONG parm1, ULONGLONG parm2, ULONGLONG parm3)
{

    SubmitHistory[SubmitIndex].tag = tag;
    SubmitHistory[SubmitIndex].queueId = queueId;
    SubmitHistory[SubmitIndex].NSID = NSID;
    SubmitHistory[SubmitIndex].CDW0 = CDW0;
    SubmitHistory[SubmitIndex].parm1 = parm1;
    SubmitHistory[SubmitIndex].parm2 = parm2;
    SubmitHistory[SubmitIndex].parm3 = parm3;

    if((++SubmitIndex) >= HISTORY_DEPTH)
        SubmitIndex = 0;

    SubmitHistory[SubmitIndex].tag = HISTORY_MARKER;
}

void TracePathComplete(HISTORY_TAG tag, ULONG queueId, ULONG CID,
        ULONG SQHD,NVMe_COMPLETION_QUEUE_ENTRY_DWORD_3 DW3, ULONGLONG parm2, ULONGLONG parm3)
{

    CompleteHistory[CompleteIndex].tag = tag;
    CompleteHistory[CompleteIndex].queueId = queueId;
    CompleteHistory[CompleteIndex].CID = CID;
    CompleteHistory[CompleteIndex].SQHD = SQHD;
    CompleteHistory[CompleteIndex].DW3 = DW3;
    CompleteHistory[CompleteIndex].parm2 = parm2;
    CompleteHistory[CompleteIndex].parm3 = parm3;

    if((++CompleteIndex) >= HISTORY_DEPTH)
        CompleteIndex = 0;

    CompleteHistory[CompleteIndex].tag = HISTORY_MARKER;
}

void TraceEvent(HISTORY_TAG tag, ULONG queueId, ULONG CID,
        ULONG parm0, ULONGLONG parm1, ULONGLONG parm2, ULONGLONG parm3)
{

    EventHistory[EventIndex].tag = tag;
    EventHistory[EventIndex].queueId = queueId;
    EventHistory[EventIndex].CID = CID;
    EventHistory[EventIndex].parm0 = parm0;
    EventHistory[EventIndex].parm1 = parm1;
    EventHistory[EventIndex].parm2 = parm2;
    EventHistory[EventIndex].parm3 = parm3;

    if((++EventIndex) >= HISTORY_DEPTH)
        EventIndex = 0;

    EventHistory[EventIndex].tag = HISTORY_MARKER;
}

#endif

/*******************************************************************************
 * DriverEntry
 *
 * @brief Driver entry point for Storport Miniport driver.
 *
 * @param DriverObject - The driver object associated with miniport driver
 * @param RegistryPath - Used when registering with Storport driver
 *
 * @eturn ULONG
 *     Status indicating whether successfully registering with Storport driver.
 ******************************************************************************/
ULONG DriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath
)
{
    HW_INITIALIZATION_DATA hwInitData = { 0 };
    ULONG Status = 0;
#ifndef DBG
    STORAGE_TRACE_INIT_INFO initInfo;
#endif

    /* DbgBreakPoint(); */

    /* Set size of hardware initialization structure. */
    hwInitData.HwInitializationDataSize = sizeof(HW_INITIALIZATION_DATA);

    /* Identify required miniport entry point routines. */
    hwInitData.HwInitialize = NVMeInitialize;
    hwInitData.HwStartIo = NVMeStartIo;
    hwInitData.HwInterrupt = NVMeIsrIntx;
    hwInitData.HwFindAdapter = NVMeFindAdapter;
    hwInitData.HwResetBus = NVMeResetBus;
    hwInitData.HwAdapterControl = NVMeAdapterControl;
    hwInitData.HwBuildIo = NVMeBuildIo;

    /* Specifiy adapter specific information. */
    hwInitData.AutoRequestSense = TRUE;
    hwInitData.NeedPhysicalAddresses = TRUE;
    hwInitData.NumberOfAccessRanges = NVME_ACCESS_RANGES;
    hwInitData.AdapterInterfaceType = PCIBus;
    hwInitData.MapBuffers = STOR_MAP_NON_READ_WRITE_BUFFERS;
    hwInitData.TaggedQueuing = TRUE;
    hwInitData.MultipleRequestPerLu = TRUE;
    hwInitData.HwDmaStarted = NULL;
    hwInitData.HwAdapterState = NULL;
    
#if (NTDDI_VERSION > NTDDI_WIN7)
    /* Specify support/use SRB Extension for Windows 8 and up */
    hwInitData.SrbTypeFlags = SRB_TYPE_FLAG_STORAGE_REQUEST_BLOCK;
    hwInitData.FeatureSupport = STOR_FEATURE_FULL_PNP_DEVICE_CAPABILITIES;
#endif
    /* Set required extension sizes. */
    hwInitData.DeviceExtensionSize = sizeof(NVME_DEVICE_EXTENSION);
    hwInitData.SrbExtensionSize = sizeof(NVME_SRB_EXTENSION);

    /* Call StorPortInitialize to register with hwInitData */
    Status = StorPortInitialize(DriverObject,
                                RegistryPath,
                                &hwInitData,
                                NULL );

#ifndef DBG
    // 
    // Initialize storage tracing library
    //
    TraceContext = NULL;

    memset(&initInfo, 0, sizeof(STORAGE_TRACE_INIT_INFO));
    initInfo.Size = sizeof(STORAGE_TRACE_INIT_INFO);
    initInfo.DriverObject = DriverObject;
    initInfo.NumErrorLogRecords = 5;
    initInfo.TraceCleanupRoutine = WppCleanupRoutine;
    initInfo.TraceContext = NULL;

    WPP_INIT_TRACING(DriverObject, RegistryPath);

    //
    // Save the traceContext 
    // 
    if (initInfo.TraceContext != NULL) {
        TraceContext = initInfo.TraceContext;
    }


#endif

    // initialize the Processor Group compatibility library (procgrp.lib)
    WdmlibProcgrpInitialize();

    StorPortDebugPrint(INFO, "StorPortInitialize returns Status(0x%x)\n", Status);

    return (Status);
} /* DriverEntry */

/*******************************************************************************
 * NVMeFindAdapter
 *
 * @brief This function gets called to fill in the Port Configuration
 *        Information structure that indicates more capabillites the adapter
 *        supports.
 *
 * @param Context - Pointer to hardware device extension.
 * @param Reserved1 - Unused.
 * @param Reserved2 - Unused.
 * @param ArgumentString - DriverParameter string.
 * @param pPCI - Pointer to PORT_CONFIGURATION_INFORMATION structure.
 * @param Reserved3 - Unused.
 *
 * @return ULONG
 *     Returns status based upon results of adapter parameter acquisition.
 ******************************************************************************/
ULONG
NVMeFindAdapter(
    PVOID Context,
    PVOID Reserved1,
    PVOID Reserved2,
    PCSTR ArgumentString,
    PPORT_CONFIGURATION_INFORMATION pPCI,
    UCHAR* Reserved3
)
{
    PNVME_DEVICE_EXTENSION pAE = (PNVME_DEVICE_EXTENSION)Context;
    PACCESS_RANGE pMM_Range;
    PRES_MAPPING_TBL pRMT = NULL;
    ULONG pciStatus = 0;
#if (NTDDI_VERSION > NTDDI_WIN7)
	ULONG storStatus = STOR_STATUS_UNSUCCESSFUL;
#endif
    NVMe_CONTROLLER_CAPABILITIES CAP = {0};

    UNREFERENCED_PARAMETER( Reserved1 );
    UNREFERENCED_PARAMETER( Reserved2 );
    UNREFERENCED_PARAMETER( Reserved3 );

    /* Initialize the hardware device extension structure. */
    memset ((void*)pAE, 0, sizeof(NVME_DEVICE_EXTENSION));


    /*
     * Get memory-mapped access range information
     * NVMe Adapter needs to request one access range for accessing
     * NVMe Controller registers
     */
    pMM_Range = NULL;
    pMM_Range = &(*(pPCI->AccessRanges))[0];
    if (pMM_Range == NULL) {
        return (SP_RETURN_NOT_FOUND);
    }

    /* Mapping BAR memory to the virtual address of Control registers */
    pAE->pCtrlRegister = (PNVMe_CONTROLLER_REGISTERS)StorPortGetDeviceBase(pAE,
                          pPCI->AdapterInterfaceType,
                          pPCI->SystemIoBusNumber,
                          pMM_Range->RangeStart,
                          pMM_Range->RangeLength,
                          FALSE);

    if (pAE->pCtrlRegister == NULL ||
        pMM_Range->RangeInMemory == FALSE) {
        return (SP_RETURN_NOT_FOUND);
    } else {
        /* Print out where it is */
        StorPortDebugPrint(INFO,
            "Access Range, VirtualAddr=0x%p.\n",
            pAE->pCtrlRegister);
    }

    pAE->originalVersion.value = StorPortReadRegisterUlong(pAE, (PULONG)(&pAE->pCtrlRegister->VS));

    /*
     * Parse the ArgumentString to find out if it's a normal driver loading
     * If so, go on the collection resource information. Otherwise, set up
     * PORT_CONFIGURATION_INFORMATION structure and return
     */
    if (NVMeStrCompare("dump=1", ArgumentString) == TRUE)
        pAE->ntldrDump = TRUE;

/* Code Analysis fails on StoPortReadRegisterUlong64 */
#if 0 // (NTDDI_VERSION > NTDDI_WIN7) && defined(_WIN64)
    CAP.AsUlonglong = StorPortReadRegisterUlong64(pAE,
        (PULONG64)(&pAE->pCtrlRegister->CAP));
#else
    CAP.HighPart = StorPortReadRegisterUlong(pAE,
        (PULONG)(&pAE->pCtrlRegister->CAP.HighPart));
    CAP.LowPart = StorPortReadRegisterUlong(pAE,
        (PULONG)(&pAE->pCtrlRegister->CAP.LowPart));
#endif

    /* setup ctrl timeout and stride info */
    pAE->uSecCrtlTimeout = (ULONG)(CAP.TO * MIN_WAIT_TIMEOUT);
    pAE->uSecCrtlTimeout = (pAE->uSecCrtlTimeout == 0) ?
        MIN_WAIT_TIMEOUT : pAE->uSecCrtlTimeout;
    pAE->uSecCrtlTimeout *= MILLI_TO_MICRO;

    /* save off the DB Stride Size */
    pAE->strideSz = 1 << (2 + CAP.DSTRD);
    StorPortDebugPrint(INFO, "NVMeFindAdapter: Stride Size set to 0x%x\n", pAE->strideSz);

    /*
     * If desired access range is not available or
     * RangeLength isn't enough to cover all regisgters, including
     * one Admin queue pair and one IO queue pair, or
     * it's not memory-mapped, return now with SP_RETURN_NOT_FOUND
     */
    #define NUM_DB_PER_QP (2)
    if ((pMM_Range->RangeLength <
        (NVME_DB_START + 2 * sizeof(NVMe_QUEUE_Y_DOORBELL)
          + 2 * NUM_DB_PER_QP * pAE->strideSz))) {
        /*
         * If no access range granted, treat it as error case and return
         * Otherwise, jump out of the loop
         */
        return (SP_RETURN_NOT_FOUND);
    }

    /*
     * Pre-program with default values in case of failure in accessing Registry
     * Defaultly, it can support up to 16 LUNs per target
     */
    pAE->InitInfo.Namespaces = DFT_NAMESPACES;

    /* Max transfer size is 128KB by default */
    pAE->InitInfo.MaxTxSize = DFT_TX_SIZE;
    pAE->PRPListSize = ((pAE->InitInfo.MaxTxSize / PAGE_SIZE) * sizeof(UINT64));

    /* 128 entries by default for Admin queue. */
    pAE->InitInfo.AdQEntries = DFT_AD_QUEUE_ENTRIES;

    /* 1024 entries by default for IO queues. */
    pAE->InitInfo.IoQEntries = DFT_IO_QUEUE_ENTRIES;

    /* Interrupt coalescing by default: 8 millisecond/16 completions. */
    pAE->InitInfo.IntCoalescingTime = DFT_INT_COALESCING_TIME;
    pAE->InitInfo.IntCoalescingEntry = DFT_INT_COALESCING_ENTRY;

    /* Information for accessing pciCfg space */
    pAE->SystemIoBusNumber  =  pPCI->SystemIoBusNumber;
    pAE->SlotNumber         =  pPCI->SlotNumber;

    /*
     * Access Registry and enumerate NUMA/cores topology when normal driver is
     * being loaded.
     */
    if (pAE->ntldrDump == FALSE) {
        /* Call NVMeFetchRegistry to retrieve all designated values */
        NVMeFetchRegistry(pAE);

        /* regardless of hardcoded or reg overrides, IOQ is limited by CAP */
        if (pAE->InitInfo.IoQEntries > (ULONG)(CAP.MQES + 1)) {
            StorPortDebugPrint(INFO, "IO Q size limited by HW to 0x%x\n",
                (pAE->pCtrlRegister->CAP.MQES + 1));
            pAE->InitInfo.IoQEntries = CAP.MQES + 1;
        }

        /* updte in case someone used the registry to change MaxTxSie */
        pAE->PRPListSize = ((pAE->InitInfo.MaxTxSize / PAGE_SIZE) * sizeof(UINT64));

        /*
         * Get the CPU Affinity of current system and construct NUMA table,
         * including if NUMA supported, how many CPU cores, NUMA nodes, etc
         */
        if (NVMeEnumNumaCores(pAE) == FALSE)
            return (SP_RETURN_NOT_FOUND);

        /*
         * Allocate buffer for MSI_MESSAGE_TBL structure array. If fails, return
         * FALSE.
         */
        pRMT = &pAE->ResMapTbl;
        pRMT->pMsiMsgTbl = (PMSI_MESSAGE_TBL)
            NVMeAllocatePool(pAE, pRMT->NumActiveCores *
                             sizeof(MSI_MESSAGE_TBL));

        if (pRMT->pMsiMsgTbl == NULL)
            return (SP_RETURN_NOT_FOUND);

        /*
         * Allocate buffer for DPC completiong array. If fails, return
         * FALSE.
         */
        pAE->NumDpc = pRMT->NumActiveCores + 1;
        pAE->pDpcArray = NVMeAllocatePool(pAE,
            pAE->NumDpc * sizeof(STOR_DPC));

        if ( pAE->pDpcArray == NULL )
            return (SP_RETURN_NOT_FOUND);

		/* 
		 * Allocate buffer for array of group affinity for message target.
		 * If fails, return FALSE
		 */
		pAE->pArrGrpAff = (PGROUP_AFFINITY)NVMeAllocatePool(pAE,
			pAE->ResMapTbl.NumActiveCores * sizeof(GROUP_AFFINITY));

		if (pAE->pArrGrpAff == NULL) 
			return (SP_RETURN_NOT_FOUND);

#if (NTDDI_VERSION > NTDDI_WIN7)
		storStatus = StorPortInitializeTimer(pAE, &pAE->Timerhandle);

		if (storStatus != STOR_STATUS_SUCCESS) {
			StorPortDebugPrint(ERROR, "---NVMeFindAdapter: <Error> Intialization of timer failed---\n");
			pAE->Timerhandle = NULL;
		}
#endif
    }

    /* Populate all PORT_CONFIGURATION_INFORMATION fields... */
    pPCI->MaximumTransferLength = pAE->InitInfo.MaxTxSize;
    pPCI->NumberOfPhysicalBreaks = pAE->InitInfo.MaxTxSize / PAGE_SIZE;
    pPCI->NumberOfBuses = 1;
    pPCI->ScatterGather = TRUE;
    pPCI->AlignmentMask = BUFFER_ALIGNMENT_MASK;  /* Double WORD Aligned */
    pPCI->CachesData = FALSE;

    /* Support SRB_FUNCTION_RESET_DEVICE */
    pPCI->ResetTargetSupported = TRUE;

    /* Set the supported number of Targets per bus. */
    pPCI->MaximumNumberOfTargets = 1;

    /* Set the supported number of LUNs per target. */
    pPCI->MaximumNumberOfLogicalUnits = (UCHAR)pAE->InitInfo.Namespaces;

    /* Set driver to run in full duplex mode */
    pPCI->SynchronizationModel = StorSynchronizeFullDuplex;

    /* Specify the size of SrbExtension */
    pPCI->SrbExtensionSize = sizeof(NVME_SRB_EXTENSION);

    /* For 64-bit systems, controller supports 64-bit addressing, */
    if (pPCI->Dma64BitAddresses == SCSI_DMA64_SYSTEM_SUPPORTED)
        pPCI->Dma64BitAddresses = SCSI_DMA64_MINIPORT_SUPPORTED;

    pPCI->InterruptSynchronizationMode = InterruptSynchronizePerMessage;

    /* Specify NVMe MSI/MSI-X ISR here */
    pPCI->HwMSInterruptRoutine = NVMeIsrMsix;
    
    /* requesting memory buffers in crash dump or hibernation mode. */
    pPCI->RequestedDumpBufferSize = DUMP_BUFFER_SIZE;
    pAE->pPCI = pPCI;    
    pPCI->WmiDataProvider = TRUE;


    InitializeWmiContext(pAE);

    /* Confirm with Storport that device is found */
    return(SP_RETURN_FOUND);
} /* NVMeFindAdapter */

/*******************************************************************************
 * NVMePassiveInitialize
 *
 * @brief NVMePassiveInitialize gets called to do the following for the
 *        Controller:
 *
 *        1. Allocate memory buffers for Admin and IO queues
 *        2. Initialize the queues
 *        3. Initialize/Enable the adapter
 *        4. Construct resource mapping table
 *        5. Issue Admin commands for the initialization
 *        6. Initialize DPCs for command completions that need to free memory
 *        7. Enter Start State machine for other initialization
 *
 * @param Context - Pointer to hardware device extension.
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMePassiveInitialize(
    PVOID Context
)
{
    PNVME_DEVICE_EXTENSION pAE = (PNVME_DEVICE_EXTENSION)Context;
    ULONG Status = STOR_STATUS_SUCCESS;
    PQUEUE_INFO pQI = &pAE->QueueInfo;
    PRES_MAPPING_TBL pRMT = &pAE->ResMapTbl;
    ULONG Lun;
    ULONG i;
    ULONG passiveTimeout;
    ULONG newVersion = 0;

    /* Ensure the Context is valid first */
    if (pAE == NULL)
        return (FALSE);

    /*
     * Based on the number of active cores in the system, allocate sub/cpl queue
     * info structure array first. The total number of structures should be the
     * number of active cores plus one (Admin queue).
     */
    pQI->pSubQueueInfo =
        (PSUB_QUEUE_INFO)NVMeAllocatePool(pAE, sizeof(SUB_QUEUE_INFO) *
                                          (pRMT->NumActiveCores + 1));

    if (pQI->pSubQueueInfo == NULL) {
        /* Free the allocated SUB_QUEUE_INFO structure memory */
        NVMeFreeBuffers(pAE);
        return (FALSE);
    }

    pQI->pCplQueueInfo =
        (PCPL_QUEUE_INFO)NVMeAllocatePool(pAE, sizeof(CPL_QUEUE_INFO) *
                                          (pRMT->NumActiveCores + 1));

    if (pQI->pCplQueueInfo == NULL) {
        /* Free the allocated SUB_QUEUE_INFO structure memory */
        NVMeFreeBuffers(pAE);
        return (FALSE);
    }

    /*
     * Allocate Admin queue first from NUMA node#0 by default If failed, return
     * failure.
     */
    Status = NVMeAllocQueues(pAE,
                             0,
                             pAE->InitInfo.AdQEntries,
                             0);

    if (Status != STOR_STATUS_SUCCESS) {
        /* Free the allocated SUB/CPL_QUEUE_INFO structures memory */
        NVMeFreeBuffers(pAE);
        return (FALSE);
    }

    /* Mark down the actual number of entries allocated for Admin queue */
    pQI->pSubQueueInfo->SubQEntries = pQI->NumAdQEntriesAllocated;
    pQI->pCplQueueInfo->CplQEntries = pQI->NumAdQEntriesAllocated;

    Status = NVMeInitAdminQueues(pAE);
    if (Status != STOR_STATUS_SUCCESS) {
        NVMeFreeBuffers(pAE);
        return (FALSE);
    }

    /* Allocate one SRB Extension for Start State Machine command submissions */
    pAE->DriverState.pSrbExt = NVMeAllocatePool(pAE, sizeof(NVME_SRB_EXTENSION));
    if (pAE->DriverState.pSrbExt == NULL) {
        /* Free the allocated buffers before returning */
        NVMeFreeBuffers (pAE);
        return (FALSE);
    }

    /* Allocate memory for LUN extensions */
    pAE->LunExtSize = MAX_NAMESPACES * sizeof(NVME_LUN_EXTENSION);
    pAE->pLunExtensionTable[0] =
        (PNVME_LUN_EXTENSION)NVMeAllocateMem(pAE, pAE->LunExtSize, 0);

    if (pAE->pLunExtensionTable[0] == NULL) {
        /* Free the allocated buffers before returning */
        NVMeFreeBuffers (pAE);
        return (FALSE);
    }

    /* Populate each LUN extension table with a valid address */
    for (Lun = 1; Lun < MAX_NAMESPACES; Lun++)
        pAE->pLunExtensionTable[Lun] = pAE->pLunExtensionTable[0] + Lun;

    /*
     * Allocate buffer for data transfer in Start State Machine before State
     * Machine starts
     */
    pAE->DriverState.pDataBuffer = NVMeAllocateMem(pAE, PAGE_SIZE, 0);
    if ( pAE->DriverState.pDataBuffer == NULL ) {
        /* Free the allocated buffers before returning */
        NVMeFreeBuffers(pAE);
        return (FALSE);
    }

#ifdef HISTORY
    SubmitIndex = 0;
    CompleteIndex = 0;
    EventIndex = 0;
    memset(&SubmitHistory, 0, sizeof(HISTORY_SUBMIT) * HISTORY_DEPTH);
    memset(&CompleteHistory, 0, sizeof(HISTORY_COMPLETE) * HISTORY_DEPTH);
    memset(&EventHistory, 0, sizeof(HISTORY_EVENT) * HISTORY_DEPTH);
#endif

    /* Initialize a DPC for command completions that need to free memory */
    StorPortInitializeDpc(pAE, &pAE->SntiDpc, SntiDpcRoutine);
    StorPortInitializeDpc(pAE, &pAE->RecoveryDpc, RecoveryDpcRoutine);

    /* Initialize DPC objects for IO completions */
    for (i = 0; i < pAE->NumDpc; i++) {
        StorPortInitializeDpc(pAE,
            (PSTOR_DPC)pAE->pDpcArray + i,
            IoCompletionRoutine);
    }

    pAE->RecoveryAttemptPossible = FALSE;
    pAE->IoQueuesAllocated = FALSE;
    pAE->ResourceTableMapped = FALSE;
    pAE->LearningCores = 0;
    pAE->DriverState.AllNamespacesAreReady = FALSE;
    pAE->DriverState.NextNamespaceToCheckForReady = 0;

    /*
     * Start off the state machine here, the following commands need to be
     * issued before initialization can be finalized:
     *
     *   Identify (Controller structure)
     *   Identify (Namespace structures)
     *   Asynchronous Event Requests (4 commands by default)
     *   Create IO Completion Queues
     *   Create IO Submission Queues
     *   Go through learning mode to match cores/vestors
     */
     NVMeRunningStartAttempt(pAE, FALSE, NULL);


     /*
      * Check timeout, if we fail to start (or recover from reset) then
      * we leave the controller in this state (NextDriverState) and we
      * won't accept any IO.  We'll also log an error.
      *
      */

     /* TO val is based on CAP registre plus a few, 5, seconds to init post RDY */
     passiveTimeout = pAE->uSecCrtlTimeout + (STORPORT_TIMER_CB_us * MICRO_TO_SEC);
     while ((pAE->DriverState.NextDriverState != NVMeStartComplete) &&
            (pAE->DriverState.NextDriverState != NVMeStateFailed)){

        newVersion = StorPortReadRegisterUlong(pAE, (PULONG)(&pAE->pCtrlRegister->VS));
        /* increment 5000us (timer callback val */
        pAE->DriverState.TimeoutCounter += pAE->DriverState.CheckbackInterval;
        if ((pAE->DriverState.TimeoutCounter > passiveTimeout) || (pAE->originalVersion.value != newVersion)) {

#if (NTDDI_VERSION > NTDDI_WIN7)
            if (pAE->Timerhandle != NULL) {
                StorPortFreeTimer(pAE, pAE->Timerhandle);
                pAE->Timerhandle = NULL;
            }
#endif

            pAE->DriverState.NextDriverState = NVMeStartComplete;
            pAE->DeviceRemovedDuringIO = TRUE;
            StorPortDebugPrint(ERROR, "NVMePassiveInitialize: <Error> State machine timeout or device removed during \
                          controller initialization\n");
            break;
        }
        NVMeStallExecution(pAE,STORPORT_TIMER_CB_us);
     }

     return (pAE->DriverState.NextDriverState == NVMeStartComplete) ? TRUE : FALSE;

} /* NVMePassiveInitialize */

/*******************************************************************************
 * NVMeInitialize
 *
 * @brief NVMeInitialize gets called to initialize the following resources after
 *        resetting the adpater. In normal driver loading, enable passive
 *        initialization to handle most of the it. Otherwise, initialization
 *        needs to be finished here.
 *
 *        0. Set up NUMA I/O optimizations
 *        1. Allocate non-paged system memroy queue entries, structures, etc
 *        2. Initialize queues
 *        3. Issue Admin commands to gather more adapter information
 *        4. Construct resource mapping table
 *
 * @param Context - Pointer to hardware device extension.
 *
 * @return VOID
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeInitialize(
    PVOID Context
)
{
    PNVME_DEVICE_EXTENSION pAE = (PNVME_DEVICE_EXTENSION)Context;
    PQUEUE_INFO pQI = &pAE->QueueInfo;
    PRES_MAPPING_TBL pRMT = &pAE->ResMapTbl;
    ULONG Status = STOR_STATUS_SUCCESS;
    USHORT QueueID;
    ULONG QEntries;
    ULONG Lun;
    NVMe_CONTROLLER_CONFIGURATION CC = {0};
    PERF_CONFIGURATION_DATA perfQueryData = {0};
    PERF_CONFIGURATION_DATA perfData = {0};

    /* Ensure the Context is valid first */
    if (pAE == NULL)
        return (FALSE);

    pAE->IsMsiMappingComplete = FALSE;

    CC.AsUlong =
        StorPortReadRegisterUlong(pAE, (PULONG)(&pAE->pCtrlRegister->CC));

    if (CC.EN == 1) {
        NVMe_CONTROLLER_STATUS CSTS = {0};
        ULONG time = 0;

        StorPortDebugPrint(INFO, "NVMeInitialize:  EN already set, wait for RDY...\n");
        /*
         * Before we transition to 0, make sure the ctrl is actually RDY
         * NOTE:  Some HW implementations may not require this wait and
         * if not then it could be removed as waiting at this IRQL is
         * not recommended.  The spec is not clear on whether we need
         * to wait for RDY to transition EN back to 0 or not.
         */
        if(FALSE == NVMeWaitForCtrlRDY(pAE, 1)) {
            return FALSE;
        }

        StorPortDebugPrint(INFO, "NVMeInitialize:  Clearing EN...\n");
        /* Now reset */
        CC.EN = 0;
        StorPortWriteRegisterUlong(pAE,
                                   (PULONG)(&pAE->pCtrlRegister->CC),
                                   CC.AsUlong);

        /* Need to ensure it's cleared in CSTS */
        if(FALSE == NVMeWaitForCtrlRDY(pAE, 0)) {
            return FALSE;
        }
    }

    /*
     * NULLify all to-be-allocated buffer pointers. In failure cases we need to
     * free the buffers in NVMeFreeBuffers, it has not yet been allocated. If
     * it's NULL, nothing needs to be done.
     */
    pAE->DriverState.pSrbExt = NULL;
    pAE->pLunExtensionTable[0] = NULL;
    pAE->QueueInfo.pSubQueueInfo = NULL;
    pAE->QueueInfo.pCplQueueInfo = NULL;

    /*
     * When Crashdump/Hibernation driver is being loaded, need to complete the
     * entire initialization here. In the case of normal driver loading, enable
     * passive initialization and let NVMePassiveInitialization handle the rest
     * of the initialization
     */
    if (pAE->ntldrDump == FALSE) {
        /* Enumerate granted MSI/MSI-X messages if there is any */
        if (NVMeEnumMsiMessages(pAE) == FALSE) {
            NVMeFreeBuffers(pAE);
            return (FALSE);
        }



		perfQueryData.Version = STOR_PERF_VERSION;
		perfQueryData.Size = sizeof(PERF_CONFIGURATION_DATA);
		Status = StorPortInitializePerfOpts(pAE, TRUE, &perfQueryData);
		// we remove the ASSERT here, because the extended storport function StorPortInitializePerfOpts
		// is not present on Windows Server 2003 and we will receive the return code STOR_STATUS_NOT_IMPLEMENTED (0xC1000002L)
		//ASSERT(STOR_STATUS_SUCCESS == Status);
		if (STOR_STATUS_SUCCESS == Status) {

			perfData.Version = STOR_PERF_VERSION;
			perfData.Size = sizeof(PERF_CONFIGURATION_DATA);

			/* Allow optimization of storport DPCs */
			if (perfQueryData.Flags & STOR_PERF_DPC_REDIRECTION) {
				perfData.Flags = STOR_PERF_DPC_REDIRECTION;

				/* Allow optimization of Interrupt Redirection and Group Affinity */
				if ((perfQueryData.Flags & (STOR_PERF_INTERRUPT_MESSAGE_RANGES | STOR_PERF_ADV_CONFIG_LOCALITY)) ==
					(STOR_PERF_INTERRUPT_MESSAGE_RANGES | STOR_PERF_ADV_CONFIG_LOCALITY)){
					perfData.Flags |= STOR_PERF_INTERRUPT_MESSAGE_RANGES;
					perfData.Flags |= STOR_PERF_ADV_CONFIG_LOCALITY;

					perfData.FirstRedirectionMessageNumber = 0;
					perfData.LastRedirectionMessageNumber = (pRMT->NumMsiMsgGranted - 1);
					perfData.MessageTargets = pAE->pArrGrpAff;
				}
			}

			Status = StorPortInitializePerfOpts(pAE, FALSE, &perfData);
			ASSERT(STOR_STATUS_SUCCESS == Status);
			if (STOR_STATUS_SUCCESS == Status){
				pAE->IsMsiMappingComplete = TRUE;
			}
		}

        /* Call StorPortPassiveInitialization to enable passive init */
        StorPortEnablePassiveInitialization(pAE, NVMePassiveInitialize);

        return (TRUE);
    } else {
        if (pAE->DumpBuffer == NULL) {
            pAE->DumpBuffer = pAE->pPCI->DumpRegion.VirtualBase;
            ASSERT(DUMP_BUFFER_SIZE == pAE->pPCI->DumpRegion.Length);
        }
        pAE->DumpBufferBytesAllocated = 0;
        
        /* Initialize members of resource mapping table first */
        pRMT->InterruptType = INT_TYPE_INTX;
        pRMT->NumActiveCores = 1;
        pAE->QueueInfo.NumCplIoQAllocFromAdapter = 1;
        pAE->QueueInfo.NumSubIoQAllocFromAdapter = 1;

        /*
         * Allocate sub/cpl queue info structure array first. The total number
         * of structures should be two, one IO queue and one Admin queue.
         */
        pQI->pSubQueueInfo =
            (PSUB_QUEUE_INFO)NVMeAllocatePool(pAE, sizeof(SUB_QUEUE_INFO) *
                                              (pRMT->NumActiveCores + 1));

        if (pQI->pSubQueueInfo == NULL) {
            NVMeFreeBuffers(pAE);
            return (FALSE);
        }

        pQI->pCplQueueInfo =
            (PCPL_QUEUE_INFO)NVMeAllocatePool(pAE, sizeof(CPL_QUEUE_INFO) *
                                              (pRMT->NumActiveCores + 1));

        if (pQI->pCplQueueInfo == NULL) {
            NVMeFreeBuffers(pAE);
            return (FALSE);
        }

        /*
         * Allocate buffers for each queue and initialize them if any failures,
         * free allocated buffers and terminate the initialization
         * unsuccessfully
         */
        for (QueueID = 0; QueueID <= pRMT->NumActiveCores; QueueID++) {
            /*
             * Based on the QueueID (0 means Admin queue, others are IO queues),
             * decide number of queue entries to allocate.  Learning mode is
             * not applicable for INTX
             */
            if (pAE->ntldrDump == FALSE) {
                QEntries = (QueueID == 0) ? pAE->InitInfo.AdQEntries:
                                            pAE->InitInfo.IoQEntries;
            } else {
                QEntries = MIN_IO_QUEUE_ENTRIES;
            }

            Status = NVMeAllocQueues(pAE,
                                     QueueID,
                                     QEntries,
                                     0);

            if (Status != STOR_STATUS_SUCCESS) {
                /* Free the allocated buffers before returning */
                NVMeFreeBuffers(pAE);
                return (FALSE);
            }

            /* Initialize Submission queue */
            Status = NVMeInitSubQueue(pAE, QueueID);
            if (Status != STOR_STATUS_SUCCESS) {
                /* Free the allocated buffers before returning */
                NVMeFreeBuffers(pAE);
                return (FALSE);
            }

            /* Initialize Completion queue */
            Status = NVMeInitCplQueue(pAE, QueueID);
            if (Status != STOR_STATUS_SUCCESS) {
                /* Free the allocated buffers before returning */
                NVMeFreeBuffers(pAE);
                return (FALSE);
            }

            /* Initialize Command Entries */
            Status = NVMeInitCmdEntries(pAE, QueueID);
            if (Status != STOR_STATUS_SUCCESS) {
                /* Free the allocated buffers before returning */
                NVMeFreeBuffers(pAE);
                return (FALSE);
            }
        }

        /* Now, conclude how many IO queue memory are allocated */
        pQI->NumSubIoQAllocated = pRMT->NumActiveCores;
        pQI->NumCplIoQAllocated = pRMT->NumActiveCores;

        /*
         * Enable adapter after initializing some controller and Admin queue
         * registers. Need to ensure the adapter is ready for processing
         * commands after entering Start State Machine.
         */
        if ((NVMeEnableAdapter(pAE)) == FALSE){
            return (FALSE);
        }

        /*
         * Allocate one SRB Extension for Start State Machine command
         * submissions
         */
        pAE->DriverState.pSrbExt =
            NVMeAllocatePool(pAE, sizeof(NVME_SRB_EXTENSION));

        if (pAE->DriverState.pSrbExt == NULL) {
            /* Free the allocated buffers before returning */
            NVMeFreeBuffers (pAE);
            return (FALSE);
        }

        /* Allocate memory for LUN extensions */
        pAE->LunExtSize = MAX_NAMESPACES * sizeof(NVME_LUN_EXTENSION);
        pAE->pLunExtensionTable[0] =
            (PNVME_LUN_EXTENSION)NVMeAllocateMem(pAE, pAE->LunExtSize, 0);
        if (pAE->pLunExtensionTable[0] == NULL) {
            /* Free the allocated buffers before returning */
            NVMeFreeBuffers(pAE);
            return (FALSE);
        }

        /* Populate each LUN extension table with valid an address */
        for (Lun = 1; Lun < MAX_NAMESPACES; Lun++)
            pAE->pLunExtensionTable[Lun] = pAE->pLunExtensionTable[0] + Lun;

        /*
         * Allocate buffer for data transfer in Start State Machine before State
         * Machine starts
         */
        pAE->DriverState.pDataBuffer = NVMeAllocateMem(pAE, PAGE_SIZE, 0);
        if (pAE->DriverState.pDataBuffer == NULL) {
            /* Free the allocated buffers before returning */
            NVMeFreeBuffers(pAE);
            return (FALSE);
        }

        /*
         * Start off the state machine here, the following commands need to be
         * issued before initialization can be finalized:
         *
         *   Identify (Controller structure)
         *   Identify (Namespace structures)
         *   Set Features (Feature ID# 7)
         *   Asynchronous Event Requests (4 commands by default)
         *   Create IO Completion Queues
         *   Create IO Submission Queues
         */
         return NVMeRunningStartAttempt(pAE, FALSE, NULL);
    }
} /* NVMeInitialize */

/*******************************************************************************
 * NVMeAdapterControl
 *
 * @brief NVMeAdpaterControl handles calls from the Storport to handle power
 *        type requests
 *
 * @param AdapterExtension - Pointer to device extension
 * @param ControlType - Type of adapter control request
 * @param Parameters - Parameters for the adapter control request
 *
 * @return SCSI_ADAPTER_CONTROL_STATUS
 ******************************************************************************/
SCSI_ADAPTER_CONTROL_STATUS NVMeAdapterControl(
    __in PVOID AdapterExtension,
    __in SCSI_ADAPTER_CONTROL_TYPE ControlType,
    __in PVOID Parameters
)
{
    PNVME_DEVICE_EXTENSION pAE = (PNVME_DEVICE_EXTENSION) AdapterExtension;
    PSCSI_SUPPORTED_CONTROL_TYPE_LIST pList;
    SCSI_ADAPTER_CONTROL_STATUS scsiStatus = ScsiAdapterControlSuccess;
    BOOLEAN status = FALSE;
    ULONG PowerAction = 0;
    ULONG DevicePowerState = 0;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTOR_ADAPTER_CONTROL_POWER adapterPower = NULL;
#endif

    switch (ControlType) {
        /* Determine which control types (routines) are supported */
        case ScsiQuerySupportedControlTypes:
            /* Get pointer to control type list */
            pList = (PSCSI_SUPPORTED_CONTROL_TYPE_LIST)Parameters;

            /* Report StopAdapter and RestartAdapter are supported. */
            if (ScsiQuerySupportedControlTypes < pList->MaxControlType) {
                pList->SupportedTypeList[ScsiQuerySupportedControlTypes] = TRUE;
            }

            if (ScsiStopAdapter < pList->MaxControlType) {
                pList->SupportedTypeList[ScsiStopAdapter] = TRUE;
            }

            if (ScsiRestartAdapter < pList->MaxControlType) {
                pList->SupportedTypeList[ScsiRestartAdapter] = TRUE;
            }

#if (NTDDI_VERSION > NTDDI_WIN7)
            if (ScsiAdapterPower < pList->MaxControlType) {
                pList->SupportedTypeList[ScsiAdapterPower] = TRUE;
            }
#endif
        break;
        /* StopAdapter routine called just before power down of adapter */
        case ScsiStopAdapter:
            StorPortDebugPrint(INFO,"NVMeAdapterControl: ScsiStopAdapter\n");

#if (NTDDI_VERSION <= NTDDI_WIN7)
            if ((pAE->ntldrDump == FALSE) && (pAE->ShutdownInProgress == FALSE)) {
                StorPortNotification(RequestTimerCall, pAE, IsDeviceRemoved, STOP_SURPRISE_REMOVAL_TIMER);
            }
            status = NVMeAdapterControlPowerDown(pAE);
#endif
            scsiStatus = ScsiAdapterControlSuccess;
        break;
        /*
         * Routine to reinitialize adapter while system in running. Since the
         * adapter DeviceExtension is maintained through a power management
         * cycle, we can just restore the scripts and reinitialize the chip.
         */
        case ScsiRestartAdapter:
            StorPortDebugPrint(INFO,"NVMeAdapterControl: ScsiRestartAdapter\n");
#if (NTDDI_VERSION <= NTDDI_WIN7)
            status = NVMeAdapterControlPowerUp(pAE);
#endif
            scsiStatus = ScsiAdapterControlSuccess;
        break;


#if (NTDDI_VERSION > NTDDI_WIN7)
        case ScsiAdapterPower:
			adapterPower = (PSTOR_ADAPTER_CONTROL_POWER)Parameters;
			PowerAction = adapterPower->PowerAction;
			DevicePowerState = adapterPower->PowerState;
			status = NVMeAdapterPowerControl(pAE, DevicePowerState, PowerAction);
			scsiStatus = ScsiAdapterControlSuccess;
	    break;
#endif
        default:
            scsiStatus = ScsiAdapterControlUnsuccessful;
        break;
    } /* end of switch */

    return scsiStatus;
} /* NVMeAdapterControl */


/*******************************************************************************
* NVMeIsReadWriteCmd
*
* @brief NVMeIsReadWriteCmd helps to determine if the request is a SCSI read or
*        write command
* @param function - SRB defined Function
* @param opCode - SCSI opcodes
*
* @return BOOLEAN
*     TRUE - Indicates it's a read or write command
*     FALSE - It's not a read or write command
******************************************************************************/
BOOLEAN NVMeIsReadWriteCmd(
    UCHAR function,
    UCHAR opCode
    )
{
    if (function == SRB_FUNCTION_EXECUTE_SCSI) {
        if ((opCode == SCSIOP_READ6) ||
            (opCode == SCSIOP_READ) ||
            (opCode == SCSIOP_READ12) ||
            (opCode == SCSIOP_READ16) ||
            (opCode == SCSIOP_WRITE6) ||
            (opCode == SCSIOP_WRITE) ||
            (opCode == SCSIOP_WRITE12) ||
            (opCode == SCSIOP_WRITE16))
            return TRUE;
    }
    return FALSE;
} /* NVMeIsReadWriteCmd */


/*******************************************************************************
 * NVMeBuildIo
 *
 * @brief NVMeBuildIo is the Storport entry function for HwStorBuildIo. All pre
 *        work that can be done on an I/O request will be done in this routine
 *        including SCSI to NVMe translation.
 *
 * @param AdapterExtension - Pointer to device extension
 * @param Srb - Pointer to SRB
 *
 * @return BOOLEAN
 *     TRUE - Indicates that HwStorStartIo shall be called on this SRB
 *     FALSE - HwStorStartIo shall not be called on this SRB
 ******************************************************************************/
BOOLEAN NVMeBuildIo(
    __in PVOID AdapterExtension,
    __in PSCSI_REQUEST_BLOCK Srb
)
{
    PNVME_DEVICE_EXTENSION pAdapterExtension =
        (PNVME_DEVICE_EXTENSION)AdapterExtension;
    PSCSI_PNP_REQUEST_BLOCK pPnpSrb = NULL;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSRBEX_DATA_PNP pSrbExPnp = NULL;
    UCHAR PathId = SrbGetPathId((void*)Srb);
    UCHAR TargetId = SrbGetTargetId((void*)Srb);
    UCHAR Lun = SrbGetLun((void*)Srb);
    UCHAR scsiStatus = 0;
#else
    UCHAR PathId = Srb->PathId;
    UCHAR TargetId = Srb->TargetId;
    UCHAR Lun = Srb->Lun;
#endif
    ULONG SrbPnPFlags = 0;
    ULONG PnPAction = 0;
    UCHAR Function = Srb->Function;
    SNTI_TRANSLATION_STATUS sntiStatus;
    BOOLEAN ioctlStatus = FALSE;
    UCHAR opCode = 0;
    PCDB pCdb = NULL;

#if (NTDDI_VERSION > NTDDI_WIN7)
    if (Function == SRB_FUNCTION_STORAGE_REQUEST_BLOCK) {
        Function = (UCHAR)((PSTORAGE_REQUEST_BLOCK)Srb)->SrbFunction;
    }
#endif

    if(pAdapterExtension->DeviceRemovedDuringIO == TRUE) {
        Srb->SrbStatus = SRB_STATUS_ERROR;
        IO_StorPortNotification(RequestComplete, 
                                AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                (PSCSI_REQUEST_BLOCK)Srb);
#endif
        StorPortDebugPrint(ERROR, "BuildIo: <Error> NO DEVICE\n");
        return FALSE;
    }

    /*
     * Need to ensure the PathId, TargetId and Lun is supported by the
     * controller before processing the request.
     */
    if ((PathId != VALID_NVME_PATH_ID) ||
        (TargetId != VALID_NVME_TARGET_ID) ||
        (pAdapterExtension->pLunExtensionTable[0] == NULL) ||
        ((pAdapterExtension->RecoveryAttemptPossible != TRUE) &&
         (pAdapterExtension->pLunExtensionTable[Lun]->slotStatus != ONLINE) && 
         (SRB_FUNCTION_IO_CONTROL != Function))) {
        Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
        IO_StorPortNotification(RequestComplete, 
                                AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                (PSCSI_REQUEST_BLOCK)Srb);
#endif
        return FALSE;
    }

    /* Check to see if the controller has started yet */
    if ((Function != SRB_FUNCTION_POWER) && (pAdapterExtension != NULL) &&
         (pAdapterExtension->DriverState.NextDriverState != NVMeStartComplete)) {
        Srb->SrbStatus = SRB_STATUS_BUSY;
        IO_StorPortNotification(RequestComplete, 
                                AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                (PSCSI_REQUEST_BLOCK)Srb);
#endif
        return FALSE;
    }

    switch (Function) {
        case SRB_FUNCTION_ABORT_COMMAND:
            NVMeInitSrbExtension((PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(Srb),
                                 pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
                                 (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                 (PSCSI_REQUEST_BLOCK)Srb);
#endif
            return TRUE;

        case SRB_FUNCTION_RESET_DEVICE:
        case SRB_FUNCTION_RESET_BUS:
        case SRB_FUNCTION_RESET_LOGICAL_UNIT:
            /* Handle in startio */
            return TRUE;
        break;
        case SRB_FUNCTION_POWER:
            StorPortDebugPrint(INFO, "BuildIo: SRB_FUNCTION_POWER\n");
            /* At this point we will just return and handle it in StartIo */
            return TRUE;
        break;
        case SRB_FUNCTION_FLUSH:
            StorPortDebugPrint(INFO, "BuildIo: SRB_FUNCTION_FLUSH\n");

            Srb->SrbStatus = SRB_STATUS_SUCCESS;
            IO_StorPortNotification(RequestComplete, 
                                    AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                    (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                    (PSCSI_REQUEST_BLOCK)Srb);
#endif
            return FALSE;
        break;
        case SRB_FUNCTION_SHUTDOWN:
            StorPortDebugPrint(0, "BuildIo: SRB_FUNCTION_SHUTDOWN\n");

            /*
             * We just return here and will handle the actual shutdown in the
             * SRB_FUNCTION_POWER. When user does shutdown, Storport sends down
             * SRB_FUNCTION_SHUTDOWN and then sends down SRB_FUNCTION_POWER.
             * Whereas for the hibernate, it sends down sync cache and then
             * sends down SRB_FUNCTION_POWER.
             */
            pAdapterExtension->ShutdownInProgress = TRUE;
            if (pAdapterExtension->ntldrDump == FALSE) {
#if (NTDDI_VERSION > NTDDI_WIN7)					
			if (pAdapterExtension->Timerhandle != NULL) {
				StorPortRequestTimer(pAdapterExtension, pAdapterExtension->Timerhandle, IsDeviceRemoved, NULL, STOP_SURPRISE_REMOVAL_TIMER, 0);
				StorPortFreeTimer(pAdapterExtension, pAdapterExtension->Timerhandle);
				pAdapterExtension->Timerhandle = NULL;
			}
#else
			StorPortNotification(RequestTimerCall, pAdapterExtension, IsDeviceRemoved, STOP_SURPRISE_REMOVAL_TIMER);
#endif
            }
            Srb->SrbStatus = SRB_STATUS_SUCCESS;
            IO_StorPortNotification(RequestComplete, 
                                    AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                    (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                    (PSCSI_REQUEST_BLOCK)Srb);
#endif
            return FALSE;
        break;
        case SRB_FUNCTION_PNP:
#if (NTDDI_VERSION > NTDDI_WIN7)
            pSrbExPnp = (PSRBEX_DATA_PNP)SrbGetSrbExDataByType(
                (PSTORAGE_REQUEST_BLOCK)Srb, SrbExDataTypePnP);
            if (pSrbExPnp != NULL) {
                SrbPnPFlags = pSrbExPnp->SrbPnPFlags;
                PnPAction = pSrbExPnp->PnPAction;
            } else {
                pPnpSrb = (PSCSI_PNP_REQUEST_BLOCK)Srb;
                SrbPnPFlags = pPnpSrb->SrbPnPFlags;
                PnPAction = pPnpSrb->PnPAction;
            }
#else
            pPnpSrb = (PSCSI_PNP_REQUEST_BLOCK)Srb;
            SrbPnPFlags = pPnpSrb->SrbPnPFlags;
            PnPAction = pPnpSrb->PnPAction;
#endif
            StorPortDebugPrint(INFO, "BuildIo: SRB_FUNCTION_PNP\n");

            if (((SrbPnPFlags & SRB_PNP_FLAGS_ADAPTER_REQUEST) == 0) &&
                (PnPAction == StorQueryCapabilities)                 &&
                (GET_DATA_LENGTH(Srb) >=
                 sizeof(STOR_DEVICE_CAPABILITIES))) {
                /*
                 * Process StorQueryCapabilities request for device, not
                 * adapter. Fill in fields of STOR_DEVICE_CAPABILITIES_EX.
                 */
#if (NTDDI_VERSION > NTDDI_WIN7)
                PSTOR_DEVICE_CAPABILITIES_EX pDevCapabilities =
                    (PSTOR_DEVICE_CAPABILITIES_EX)GET_DATA_BUFFER(Srb);
#else
                PSTOR_DEVICE_CAPABILITIES pDevCapabilities =
                    (PSTOR_DEVICE_CAPABILITIES)GET_DATA_BUFFER(Srb);
#endif
                pDevCapabilities->Version           = 0;
                pDevCapabilities->DeviceD1          = 0;
                pDevCapabilities->DeviceD2          = 0;
                pDevCapabilities->LockSupported     = 0;
                pDevCapabilities->EjectSupported    = 0;
                pDevCapabilities->Removable         = 1;
                pDevCapabilities->DockDevice        = 0;
                pDevCapabilities->UniqueID          = 0;
                pDevCapabilities->SilentInstall     = 0;
                pDevCapabilities->SurpriseRemovalOK = 0;
                pDevCapabilities->NoDisplayInUI     = 0;

                Srb->SrbStatus = SRB_STATUS_SUCCESS;
                StorPortNotification(RequestComplete,
                                     AdapterExtension,
                                     Srb);
            } else if (((SrbPnPFlags &
                         SRB_PNP_FLAGS_ADAPTER_REQUEST) != 0)   &&
                       ((PnPAction == StorRemoveDevice) ||
                        (PnPAction == StorSurpriseRemoval))) {
                /*
                 * The adapter is going to be removed, release all resources
                 * allocated for it.
                 */
                if (pAdapterExtension->ntldrDump == FALSE) {
#if (NTDDI_VERSION > NTDDI_WIN7)
					if (pAdapterExtension->Timerhandle != NULL) {
						StorPortRequestTimer(pAdapterExtension, pAdapterExtension->Timerhandle, IsDeviceRemoved, NULL, STOP_SURPRISE_REMOVAL_TIMER, 0);
						StorPortFreeTimer(pAdapterExtension, pAdapterExtension->Timerhandle);
						pAdapterExtension->Timerhandle = NULL;
					}
#else
					StorPortNotification(RequestTimerCall, pAdapterExtension, IsDeviceRemoved, STOP_SURPRISE_REMOVAL_TIMER);
#endif
                    NVMeAdapterControlPowerDown(pAdapterExtension);
                    NVMeFreeBuffers(pAdapterExtension);
                }

                Srb->SrbStatus = SRB_STATUS_SUCCESS;
                IO_StorPortNotification(RequestComplete, 
                                        AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                        (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                        (PSCSI_REQUEST_BLOCK)Srb);
#endif
            } else {
                Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
                IO_StorPortNotification(RequestComplete, 
                                        AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                        (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                        (PSCSI_REQUEST_BLOCK)Srb);
#endif
            }

            /* Return FALSE so that StartIo is not called */
            return FALSE;
        break;
        case SRB_FUNCTION_IO_CONTROL:
            /* Confirm SRB data buffer is valid first */
            if (GET_DATA_BUFFER(Srb) == NULL) {
                Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
                IO_StorPortNotification(RequestComplete, 
                                        AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                        (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                        (PSCSI_REQUEST_BLOCK)Srb);
#endif
                return FALSE;
            }
            NVMeInitSrbExtension((PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(Srb),
                                 pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
                                 (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                 (PSCSI_REQUEST_BLOCK)Srb);
#endif

            /*
             * Call NVMeProcessIoctl to process the request. When it returns
             * IOCTL_COMPLETED, indicates complete the request back to Storport
             * right away.
             */
            ioctlStatus = NVMeProcessIoctl(pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
                                          (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                          (PSCSI_REQUEST_BLOCK)Srb);
#endif
            if (ioctlStatus == IOCTL_COMPLETED) {
                IO_StorPortNotification(RequestComplete, 
                                        AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                        (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                        (PSCSI_REQUEST_BLOCK)Srb);
#endif
                return FALSE;
            }

            /* Return TRUE if more processing in StartIo needed */
            return TRUE;
        break;
        case SRB_FUNCTION_EXECUTE_SCSI:
			/*
			* Block Read/Write commands while Format NVM is in progress
			* Case 1: All namespace are being formatted.
			* Case 2: Only one namespace is being formatted.
			*/
			if (pAdapterExtension->FormatNvmInfo.State != FORMAT_NVM_NO_ACTIVITY) {
				opCode = GET_OPCODE(Srb);
				if (pAdapterExtension->FormatNvmInfo.FormatAllNamespaces == TRUE) {
					if (NVMeIsReadWriteCmd(Function, opCode) == TRUE) {
						Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
						IO_StorPortNotification(RequestComplete,
							AdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
							(PSTORAGE_REQUEST_BLOCK)Srb);
#else
							(PSCSI_REQUEST_BLOCK)Srb);
#endif
						return FALSE;
					}
				}
				else {
					if ((pAdapterExtension->FormatNvmInfo.TargetLun == Lun) &&
						(NVMeIsReadWriteCmd(Function, opCode) == TRUE)) {
						Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
						IO_StorPortNotification(RequestComplete,
							AdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
							(PSTORAGE_REQUEST_BLOCK)Srb);
#else
							(PSCSI_REQUEST_BLOCK)Srb);
#endif
						return FALSE;
					}
				}
			}
#if DBG
            /* Debug print only - turn off for main I/O */
            StorPortDebugPrint(INFO, "BuildIo: SRB_FUNCTION_EXECUTE_SCSI\n");
#endif /* DBG */

            /*
             * An SRB that makes it to this point needs to be processed and
             * have a valid SRB Extension... initialize its contents.
             */
            NVMeInitSrbExtension((PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(Srb),
                                 pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
                                 (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                 (PSCSI_REQUEST_BLOCK)Srb);
#endif

            /* Perform SCSI to NVMe translation */
            sntiStatus = SntiTranslateCommand(pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7) 
                                             (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                             (PSCSI_REQUEST_BLOCK)Srb);
#endif

            switch (sntiStatus) {
                case SNTI_COMMAND_COMPLETED:
                    /*
                     * Command completed in build phase, return FALSE so start
                     * I/O is not called for this command. The appropriate SRB
                     * status is already set.
                     */
                    IO_StorPortNotification(RequestComplete, 
                                            AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                            (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                            (PSCSI_REQUEST_BLOCK)Srb);
#endif

                    return FALSE;
                break;
                case SNTI_TRANSLATION_SUCCESS:
                    /*
                     * Command translation completed successfully, return TRUE
                     * so start I/O is called for this command.
                     */
                    return TRUE;
                break;
                case SNTI_FAILURE_CHECK_RESPONSE_DATA:
                case SNTI_UNSUPPORTED_SCSI_REQUEST:
                case SNTI_UNSUPPORTED_SCSI_TM_REQUEST:
                case SNTI_INVALID_SCSI_REQUEST_PARM:
                case SNTI_INVALID_SCSI_PATH_ID:
                case SNTI_INVALID_SCSI_TARGET_ID:
                case SNTI_UNRECOVERABLE_ERROR:
                    /*
                     * SNTI encountered an error during command translation, do
                     * not call start I/O for this command. The appropriate SRB
                     * status and SCSI sense data will aleady be set.
                     */
                    IO_StorPortNotification(RequestComplete, 
                                            AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                            (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                            (PSCSI_REQUEST_BLOCK)Srb);
#endif

                    return FALSE;
                break;
                default:
                    /* Invalid status returned */
#if (NTDDI_VERSION > NTDDI_WIN7)
                    scsiStatus = SCSISTAT_CHECK_CONDITION;
                    SrbSetScsiData((PSTORAGE_REQUEST_BLOCK)Srb, NULL, NULL,
                                   &scsiStatus, NULL, NULL);
#else
                    Srb->ScsiStatus = SCSISTAT_CHECK_CONDITION;
#endif
                    Srb->SrbStatus = SRB_STATUS_ERROR;
                    IO_StorPortNotification(RequestComplete, 
                                            AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                            (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                            (PSCSI_REQUEST_BLOCK)Srb);
#endif

                    return FALSE;
                break;
            } /* switch */
        break;
        case SRB_FUNCTION_WMI:
            NVMeInitSrbExtension((PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(Srb),
                pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
                (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                (PSCSI_REQUEST_BLOCK)Srb);
#endif
            /* For WMI requests, just turn around and complete successfully */
            StorPortDebugPrint(INFO, "BuildIo: SRB_FUNCTION_WMI\n");
            return TRUE;

            /* handle in startio */    
        break;
        default:
            /*
             * For unsupported SRB_FUNCTIONs, complete with status:
             * SRB_STATUS_INVALID_REQUEST
             */
            StorPortDebugPrint(INFO,
                               "BuildIo: Unsupported SRB Function = 0x%x\n",
                               Function);

            Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
            IO_StorPortNotification(RequestComplete, 
                                    AdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                    (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                    (PSCSI_REQUEST_BLOCK)Srb);
#endif
            return FALSE;
        break;
    } /* end switch */

    return TRUE;
} /* NVMeBuildIo */

#if (NTDDI_VERSION > NTDDI_WIN7)
  /*******************************************************************************
  * IsDeviceRemoved
  *
  * @brief Surprise removal timer routine for the Ioctl path to check if the device
  * is surprise removed by checking the validity of the Version register. Whenever
  * device is surprise removed, the memory mapped registers will be unmapped and
  * returns 1 when read.
  *
  * @param pAE - Pointer to Device extension.
  *
  * @return VOID
  ******************************************************************************/
VOID IsDeviceRemoved(
	PNVME_DEVICE_EXTENSION pAE, 
	PVOID Context
)
{
	ULONG Version = 0;
	if (pAE->ShutdownInProgress == TRUE)
		return;
	Version = StorPortReadRegisterUlong(pAE, (PULONG)(&pAE->pCtrlRegister->VS));

	if (Version == INVALID_DEVICE_REGISTER_VALUE) {

		pAE->DeviceRemovedDuringIO = TRUE;
		StorPortPause(pAE, STOR_ALL_REQUESTS);
#if DBG
		StorPortDebugPrint(ERROR, "CheckSurpriseRemoval: <Info> Device removed with outstanding IO\n");
#endif
		NVMeDetectPendingCmds(pAE, TRUE, SRB_STATUS_ERROR);
		if (pAE->Timerhandle != NULL) {
				StorPortFreeTimer(pAE, pAE->Timerhandle);
				pAE->Timerhandle = NULL;
		}
		NVMeFreeBuffers(pAE);
		StorPortResume(pAE);
	}
	else {
		if (pAE->DriverState.NextDriverState == NVMeStartComplete)
			if (pAE->Timerhandle != NULL)
				StorPortRequestTimer(pAE, pAE->Timerhandle, IsDeviceRemoved, NULL, START_SURPRISE_REMOVAL_TIMER, 0);//every 1 seconds
	}

}/*  IsDeviceRemoved */

#else
/*******************************************************************************
* IsDeviceRemoved
*
* @brief Surprise removal timer routine to check if the device is surprise 
* removed by checking the validity of the Version register. Whenever device is 
* surprise removed, the memory mapped registers will be unmapped and returns 1 
* when read. 
*
* @param pAE - Pointer to Device extension.
*
* @return VOID
******************************************************************************/
VOID IsDeviceRemoved(
    PNVME_DEVICE_EXTENSION pAE
)
{
    ULONG Version = 0;

    if (pAE->ShutdownInProgress == TRUE)
        return;

    Version = StorPortReadRegisterUlong(pAE, (PULONG)(&pAE->pCtrlRegister->VS));

    if(Version == INVALID_DEVICE_REGISTER_VALUE) {
        pAE->DeviceRemovedDuringIO = TRUE;
        StorPortPause(pAE, STOR_ALL_REQUESTS);
#if DBG
        StorPortDebugPrint(ERROR, "IsDeviceRemoved: <Error> Device removed with outstanding IO\n");
#endif
        NVMeDetectPendingCmds(pAE, TRUE, SRB_STATUS_ERROR);     
        NVMeFreeBuffers(pAE);
        StorPortResume(pAE);
    } else {
        if(pAE->DriverState.NextDriverState == NVMeStartComplete)
            StorPortNotification(RequestTimerCall, pAE, IsDeviceRemoved, START_SURPRISE_REMOVAL_TIMER); //every 1 seconds
    }
}
#endif

/*******************************************************************************
 * NVMeIssueAbortCmd
 *
 * @brief Abort a specfic command request by the host .
 *
 * @param pSrbExt - Pointer to SRB extension.
 * @param QueueID - Submit Queue index.
 * @param CID     - Context index.
 *
 * @return BOOLEAN
 *     TRUE if command is aborted 
 *     FALSE if command is not aborted 
 ******************************************************************************/
BOOLEAN NVMeIssueAbortCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    USHORT QueueID,
    USHORT CID)
{
    PNVME_DEVICE_EXTENSION pAE = pSrbExt->pNvmeDevExt;
    PNVMe_COMMAND pNVMeCmd = (PNVMe_COMMAND)(&pSrbExt->nvmeSqeUnit);
    PADMIN_ABORT_COMMAND_DW10 pAbortCmd = (PADMIN_ABORT_COMMAND_DW10)&pNVMeCmd->CDW10;

    /* Zero out the NVME command */
    memset((PVOID)pNVMeCmd, 0, sizeof(NVMe_COMMAND));

    pSrbExt->abortedCmdCount++;

    /* Populate submission entry fields */
    pNVMeCmd->CDW0.OPC = COMMAND_ABORT_REQUESTED;
    pAbortCmd->CID = CID;
    pAbortCmd->SQID = QueueID;


    /* Now issue the command via Admin Doorbell register */
    return ProcessIo(pAE, pSrbExt, NVME_QUEUE_TYPE_ADMIN, FALSE);

} /* NVMeIssueAbortCmd */

/*******************************************************************************
 * NVMeProcessAbortCmd
 *
 * @brief Abort a specfic command request by the host .
 *
 * @param pAE - Pointer to hardware device extension.
 * @param pSrb - Pointer to Srb request
 *
 * @return BOOLEAN
 *     TRUE if command is aborted 
 *     FALSE if command is not aborted 
 ******************************************************************************/
BOOLEAN NVMeProcessAbortCmd(
    PNVME_DEVICE_EXTENSION pAE,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif

)
{
    PQUEUE_INFO pQI = &pAE->QueueInfo;
    PSUB_QUEUE_INFO pSQI = NULL;
    PCMD_ENTRY pCmdEntry = NULL;
    USHORT CmdID;
    USHORT QueueID = 0;
    PNVME_SRB_EXTENSION pSrbExtension = NULL;
    BOOLEAN issueAbortCmdFlag = FALSE;
    PNVMe_COMMAND pNVMeCmd = NULL;
    PNVME_SRB_EXTENSION pResetSrbExt = GET_SRB_EXTENSION(pSrb);


    /* Simply return FALSE when buffer had been freed */
    if (pQI->pSubQueueInfo == NULL)
        return FALSE;
    
    pResetSrbExt->abortedCmdCount = 0;
    pResetSrbExt->issuedAbortCmdCnt = 0;
    pResetSrbExt->failedAbortCmdCnt = 0;

    /* Search all submission queues */
    for (QueueID = 0; QueueID <= pQI->NumSubIoQCreated; QueueID++) {
        pSQI = pQI->pSubQueueInfo + QueueID;

        for (CmdID = 0; CmdID < pSQI->SubQEntries; CmdID++) {
            pCmdEntry = ((PCMD_ENTRY)pSQI->pCmdEntry) + CmdID;
            
            /* 
             * Pending bit is not set continue 
             */
            if (pCmdEntry->Pending == FALSE){
                continue;
            }

            pSrbExtension = (PNVME_SRB_EXTENSION)pCmdEntry->Context;
            
            /* 
             * SrbExt or Srb is NULL continue 
             */
            if((pSrbExtension == NULL) ||
                (pSrbExtension->pSrb == NULL)) {
                    continue;
            }

            issueAbortCmdFlag = FALSE;
            pNVMeCmd = &pSrbExtension->nvmeSqeUnit;

            if (pSrb == pSrbExtension->pSrb) {
                issueAbortCmdFlag = TRUE;
            }

            if (issueAbortCmdFlag) {
                pSrbExtension->cmdGotAbortedFlag = TRUE;
                pResetSrbExt->issuedAbortCmdCnt++;
                if ((NVMeIssueAbortCmd(pResetSrbExt, pSQI->SubQueueID, 
                                        pNVMeCmd->CDW0.CID)) == FALSE) {
                    pResetSrbExt->issuedAbortCmdCnt--;
                }

                if (pResetSrbExt->issuedAbortCmdCnt == 0) {
                    return FALSE;
                } else {
                    return TRUE;
                }
            }
        } /* for cmds on the SQ */
    } /* for the SQ */

    if (pResetSrbExt->issuedAbortCmdCnt == 0) {
        return FALSE;
    }
    return TRUE;
} /* NVMeProcessAbortCmd */

/*******************************************************************************
 * NVMeStartIo
 *
 * @brief NVMeStartIo is the Storport entry function for HwStorStartIo. This
 *        function will be used to process and start any I/O requests.
 *
 * @param AdapterExtension - Pointer to device extension
 * @param Srb - Pointer to SRB
 *
 * @return BOOLEAN
 *     TRUE - Required to return TRUE per MSDN
 ******************************************************************************/
BOOLEAN NVMeStartIo(
    __in PVOID AdapterExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    )
{
    PNVME_DEVICE_EXTENSION pAdapterExtension =
        (PNVME_DEVICE_EXTENSION)AdapterExtension;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSRBEX_DATA_POWER pPowerSrb = NULL;
#else
    PSCSI_POWER_REQUEST_BLOCK pPowerSrb = NULL;
#endif
    UCHAR Function = Srb->Function;
    PNVME_SRB_EXTENSION pSrbExtension;
    BOOLEAN status;
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
    PSRB_IO_CONTROL pSrbIoCtrl = NULL;
    PNVMe_COMMAND pNvmeCmd = NULL;
    PNVMe_COMMAND_DWORD_0 pNvmeCmdDW0 = NULL;
    PFORMAT_NVM_INFO pFormatNvmInfo = NULL;
    ULONG Wait = 5;
    ULONG Version = 0;

#if (NTDDI_VERSION > NTDDI_WIN7)
    if (Function == SRB_FUNCTION_STORAGE_REQUEST_BLOCK)
        Function = (UCHAR)((PSTORAGE_REQUEST_BLOCK)Srb)->SrbFunction;
#endif
    /*
     * Initialize Variables. Determine if the request requires controller
     * resources, slot and command history. Check if command processing should
     * happen.
     */
    if (pAdapterExtension->DriverState.NextDriverState != NVMeStartComplete) {

		Version = StorPortReadRegisterUlong(pAdapterExtension, (PULONG)(&pAdapterExtension->pCtrlRegister->VS));
		if (Version == INVALID_DEVICE_REGISTER_VALUE) {
#if (NTDDI_VERSION > NTDDI_WIN7)
			if (pAdapterExtension->Timerhandle != NULL) {
				StorPortFreeTimer(pAdapterExtension, pAdapterExtension->Timerhandle);
				pAdapterExtension->Timerhandle = NULL;
			}
#endif
		}

        Srb->SrbStatus = SRB_STATUS_NO_DEVICE;
        IO_StorPortNotification(RequestComplete, 
                                pAdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                (PSCSI_REQUEST_BLOCK)Srb);
#endif
        return TRUE;
    }

    pSrbExtension = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(Srb);

    switch (Function) {
        case SRB_FUNCTION_ABORT_COMMAND:
            status = NVMeProcessAbortCmd(pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
                                        (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                        (PSCSI_REQUEST_BLOCK)Srb);
#endif
            if (status == FALSE) {
                Srb->SrbStatus = SRB_STATUS_ERROR;
                IO_StorPortNotification(RequestComplete, 
                                        pAdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                        (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                        (PSCSI_REQUEST_BLOCK)Srb);
#endif
            }
            break;

        case SRB_FUNCTION_TERMINATE_IO:
            Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
            break;

        case SRB_FUNCTION_RESET_DEVICE:
        case SRB_FUNCTION_RESET_LOGICAL_UNIT:
        case SRB_FUNCTION_RESET_BUS:
         //   ASSERT(FALSE);
#ifdef HISTORY
            if (Function == SRB_FUNCTION_RESET_DEVICE) {
                TraceEvent(SRB_RESET_DEVICE, 0,0,0,Srb->Lun, 0,0);
            } else if (Function == SRB_FUNCTION_RESET_LOGICAL_UNIT) {
                TraceEvent(SRB_RESET_LUN, 0,0,0,Srb->Lun, 0,0);
            } else if (Function == SRB_FUNCTION_RESET_BUS) {
                TraceEvent(SRB_RESET_BUS, 0,0,0,Srb->Lun, 0,0);
            }
#endif
            status = NVMeResetController(pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
                                        (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                        (PSCSI_REQUEST_BLOCK)Srb);
#endif
            if (status == FALSE) {
                Srb->SrbStatus = SRB_STATUS_ERROR;
                IO_StorPortNotification(RequestComplete, 
                                        pAdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                        (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                        (PSCSI_REQUEST_BLOCK)Srb);
#endif
            }
        break;
        case SRB_FUNCTION_IO_CONTROL:
            NVMeStartIoProcessIoctl(pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
                                    (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                    (PSCSI_REQUEST_BLOCK)Srb);
#endif
        break;
        case SRB_FUNCTION_EXECUTE_SCSI:
            if (pSrbExtension->forAdminQueue == TRUE) {
                status = ProcessIo(pAdapterExtension,
                                   pSrbExtension,
                                   NVME_QUEUE_TYPE_ADMIN,
                                   FALSE);
            } else {
                /* If already completed by BuildIo, no need to process. */
                if (pAdapterExtension->ntldrDump == TRUE &&
                    Srb->SrbStatus == SRB_STATUS_SUCCESS) {
                    return TRUE; 
                }
                status = ProcessIo(pAdapterExtension,
                                   pSrbExtension,
                                   NVME_QUEUE_TYPE_IO,
                                   FALSE);
            }
        break;
        case SRB_FUNCTION_POWER:
            StorPortDebugPrint(INFO, "NVMeStartIo: SRB_FUNCTION_POWER");
            Srb->SrbStatus = SRB_STATUS_SUCCESS;
#if (NTDDI_VERSION <= NTDDI_WIN7)
            status = NVMePowerControl(pAdapterExtension, Srb);
#endif
            IO_StorPortNotification(RequestComplete, 
                                    pAdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                    (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                    (PSCSI_REQUEST_BLOCK)Srb);
#endif
        break;
        case SRB_FUNCTION_WMI:
            /* For WMI requests, just turn around and complete successfully */
            StorPortDebugPrint(INFO, "NVMeStartIo: SRB_FUNCTION_WMI\n");

            DispatchWmi(pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
                (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                (PSCSI_REQUEST_BLOCK)Srb);
#endif

            Srb->SrbStatus = SRB_STATUS_SUCCESS;
            IO_StorPortNotification(RequestComplete, pAdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                (PSCSI_REQUEST_BLOCK)Srb);
#endif
            break;
        default:
            /*
             * For unsupported SRB, complete with status:
             * SRB_STATUS_INVALID_REQUEST
             */
            Srb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
            IO_StorPortNotification(RequestComplete, 
                                    pAdapterExtension, 
#if (NTDDI_VERSION > NTDDI_WIN7)
                                    (PSTORAGE_REQUEST_BLOCK)Srb);
#else
                                    (PSCSI_REQUEST_BLOCK)Srb);
#endif
        break;
    }

    return TRUE;
} /* NVMeStartIo */

/*******************************************************************************
 * NVMeStartIoProcessIoctl
 *
 * @brief Handles IOCTL processing for StartIO
 *
 * @param pAdapterExtension - Pointer to device extension
 * @param pSrb - Pointer to the SRB
 *
 * @return nothing
 ******************************************************************************/
void NVMeStartIoProcessIoctl(
    __in PNVME_DEVICE_EXTENSION pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
    __in PSTORAGE_REQUEST_BLOCK pSrb
#else
    __in PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)(GET_DATA_BUFFER(pSrb));

    /* Validate buffer length */
    if (GET_DATA_LENGTH(pSrb) < (pSrbIoCtrl->Length + sizeof(SRB_IO_CONTROL))) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_OUT_BUFFER;
        pSrb->SrbStatus = SRB_STATUS_ERROR;
        IO_StorPortNotification(RequestComplete, pAdapterExtension,pSrb);
        return ;
    }

    StorPortDebugPrint(INFO,
                       "NVMeStartIoProcessIoctl: Code = 0x%x, Signature = 0x%s\n",
                       pSrbIoCtrl->ControlCode,
                       &pSrbIoCtrl->Signature[0]);

    /* currently only private IOCTLs currently make it as far as startIO */
    if (strncmp((const char*)pSrbIoCtrl->Signature,
                NVME_SIG_STR,
                NVME_SIG_STR_LEN) == 0) {
        PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
        PNVMe_COMMAND pNvmeCmd = NULL;
        PNVMe_COMMAND_DWORD_0 pNvmeCmdDW0 = NULL;
        PFORMAT_NVM_INFO pFormatNvmInfo = NULL;
        PNVME_SRB_EXTENSION pSrbExtension;

        pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);
        pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;
        pNvmeCmdDW0 = (PNVMe_COMMAND_DWORD_0)&pNvmePtIoctl->NVMeCmd[0];
        pSrbExtension = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

        /*
         * Private Pass Through IOCTLs
         */
        switch (pSrbIoCtrl->ControlCode) {
            case NVME_PASS_THROUGH_SRB_IO_CODE:
                switch (pNvmeCmdDW0->OPC) {
                case ADMIN_FORMAT_NVM:
                    pFormatNvmInfo = &pAdapterExtension->FormatNvmInfo;

                    if (pFormatNvmInfo->State != FORMAT_NVM_RECEIVED) {
                        /*
                        * Wrong FormatNvmInfo state and no pre-processing done,
                        * need to fail it here.
                        */
                        pFormatNvmInfo->State = FORMAT_NVM_NO_ACTIVITY;
                        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                        IO_StorPortNotification(RequestComplete,
                            pAdapterExtension,
                            pSrb);
                        return;
                    }

                    /* Save the original SRB for later completion */
                    if (NVMeIoctlFormatNVM(pAdapterExtension,
                                           pSrb,
                                           pNvmePtIoctl) == IOCTL_COMPLETED) {
                        /*
                         * Need to reset FormatNvmInfo state or the next format
                         * will fail.
                         */
                        pFormatNvmInfo->State = FORMAT_NVM_NO_ACTIVITY;
                        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                        IO_StorPortNotification(RequestComplete,
                                                pAdapterExtension,
                                                pSrb);
                        return;
                    }
                    /*
                     * If the namespace is visible, then we've to call Storport
                     * to add back the formatted namespace
                     */
                    if (FORMAT_NVM_NS_REMOVED == pFormatNvmInfo->State) {

                        StorPortDebugPrint(INFO,
                            "NVMeStartIoProcessIoctl: Add NS needed.\n");

                        pFormatNvmInfo->AddNamespaceNeeded = TRUE;
                    }
                    /*
                     * Set Format NVM State Machine as FORMAT_NVM_CMD_ISSUED
                     */
                    pFormatNvmInfo->State = FORMAT_NVM_CMD_ISSUED;
                    break;
                default:
                    /* fall through for PT processing */
                    break;
                }  /* OPC switch */
                break;

            /*
             * Still Private (but non pass through) IOCTLs
             */
            case NVME_HOT_ADD_NAMESPACE:
                pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                /* Call NVMeIoctlHotAddNamespace to add the namespace */
                NVMeIoctlHotAddNamespace(pSrbExtension);
                IO_StorPortNotification(RequestComplete, pAdapterExtension,pSrb);
                return;
                break;
            case NVME_HOT_REMOVE_NAMESPACE:
                pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                /* Call NVMeIoctlHotRemoveNamespace to remove the namespace */
                NVMeIoctlHotRemoveNamespace(pSrbExtension);
                IO_StorPortNotification(RequestComplete, pAdapterExtension, pSrb);
                return;
                break;
            case NVME_RESET_DEVICE:
                /*
                 * Need to reset the controller per request from applications,
                 * e.g. after Firmware Activate command completed successfully.
                 * Schedule the reset in recovery DPC when possible.
                 */
                StorPortDebugPrint(INFO,
                       "NVMeStartIoProcessIoctl: in NVME_RESET_DEVICE now.\n");
                if (NVMeResetController(pAdapterExtension, pSrb) == FALSE) {
                    pSrb->SrbStatus = SRB_STATUS_ERROR;
                    IO_StorPortNotification(RequestComplete, pAdapterExtension, pSrb);
                }
                return;
                break;
            default:
                pSrb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
                IO_StorPortNotification(RequestComplete, pAdapterExtension,pSrb);
                return;
                break;
        } /* switch on control code */

        /* No processing required, just issue the command */
        if (pSrbExtension->forAdminQueue == TRUE) {
            ProcessIo(pAdapterExtension,
                               pSrbExtension,
                               NVME_QUEUE_TYPE_ADMIN,
                               FALSE);
        } else {
            ProcessIo(pAdapterExtension,
                               pSrbExtension,
                               NVME_QUEUE_TYPE_IO,
                               FALSE);
        }
    } else {
        pSrb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
        IO_StorPortNotification(RequestComplete, pAdapterExtension,pSrb);
    }
}
/*******************************************************************************
 * NVMeCheckPendingCpl
 *
 * @brief Helper function to determine if there is any pending completion entries
 *
 * @param pAE - Pointer to device extension
 *
 * @return BOOLEAN
 *     TRUE - Indicates we do have at least one pending entry
 *     FALSE - Indicates there is no completion pending entry
 ******************************************************************************/
BOOLEAN NVMeCheckPendingCpl(
    PNVME_DEVICE_EXTENSION pAE
    )
{
    ULONG queueID = 0;
    PQUEUE_INFO pQI = &pAE->QueueInfo;
    PCPL_QUEUE_INFO pCQI = pQI->pCplQueueInfo;
    PNVMe_COMPLETION_QUEUE_ENTRY pCQE = NULL;

    for (queueID = 0; queueID <= pQI->NumCplIoQCreated; queueID++) {
        pCQI = pQI->pCplQueueInfo + queueID;
        pCQE = (PNVMe_COMPLETION_QUEUE_ENTRY)pCQI->pCplQStart;
        pCQE += pCQI->CplQHeadPtr;

        /* Check Phase Tag to determine if it's a newly completed entry */
        if (pCQI->CurPhaseTag != pCQE->DW3.SF.P)
            return TRUE;
    }
    return FALSE;
}

/*******************************************************************************
 * NVMeIsrIntx
 *
 * @brief Legacy interupt routine
 *
 * @param AdapterExtension - Pointer to device extension
 *
 * @return BOOLEAN
 *     TRUE - Indiciates successful completion
 *     FALSE - Unsuccessful completion or error
 ******************************************************************************/
BOOLEAN NVMeIsrIntx(
    __in PVOID AdapterExtension
)
{
    PNVME_DEVICE_EXTENSION pAE = (PNVME_DEVICE_EXTENSION)AdapterExtension;
    BOOLEAN InterruptClaimed = FALSE;

    /*
     * Loop thru all queues to find out if we own the interrupt
     */
    InterruptClaimed = NVMeCheckPendingCpl(pAE);
    if (InterruptClaimed == TRUE) {
        NVMeIsrMsix(pAE, 0);
        return TRUE;
    } else {
        return FALSE;
    }
}


/*******************************************************************************
 * IoCompletionRoutine
 *
 * @brief IO completion routine; can either be scheduled to run as a DPC or 
 * called directly
 * 
 * @param pHwDeviceExtension - Pointer to device extension
 * @param pSystemArgument1 - MSI-X message Id
 *
 * @return void
 ******************************************************************************/
VOID
IoCompletionRoutine(
    IN PSTOR_DPC  pDpc,
    IN PVOID  pHwDeviceExtension,
    IN PVOID  pSystemArgument1,
    IN PVOID  pSystemArgument2
    )
{
    PNVME_DEVICE_EXTENSION pAE = (PNVME_DEVICE_EXTENSION)pHwDeviceExtension;
    ULONG MsgID = PtrToUlong(pSystemArgument1);
    PNVMe_COMPLETION_QUEUE_ENTRY pCplEntry = NULL;
    PNVME_SRB_EXTENSION pSrbExtension = NULL;
    SNTI_TRANSLATION_STATUS sntiStatus = SNTI_TRANSLATION_SUCCESS;
    ULONG entryStatus = STOR_STATUS_SUCCESS;
    PMSI_MESSAGE_TBL pMMT = NULL;
    PQUEUE_INFO pQI = &pAE->QueueInfo;
    PRES_MAPPING_TBL pRMT = &pAE->ResMapTbl;
    PSUB_QUEUE_INFO pSQI = NULL;
    PCPL_QUEUE_INFO pCQI = NULL;
    USHORT firstCheckQueue = 0;
    USHORT lastCheckQueue = 0;
    USHORT indexCheckQueue = 0;
    BOOLEAN InterruptClaimed = FALSE;
    STOR_LOCK_HANDLE DpcLockhandle = { 0 };
    STOR_LOCK_HANDLE StartLockHandle = { 0 };
    BOOLEAN completeStatus = FALSE;

    if (pDpc != NULL) {
        ASSERT(pAE->ntldrDump == FALSE);
        if (pAE->MultipleCoresToSingleQueueFlag) {
            StorPortAcquireSpinLock(pAE, StartIoLock, NULL, &StartLockHandle);
        } else {
            StorPortAcquireSpinLock(pAE, DpcLock, pDpc, &DpcLockhandle);
        }
    }
    
    /* Use the message id to find the correct entry in the MSI_MESSAGE_TBL */
    pMMT = pRMT->pMsiMsgTbl + MsgID;

    if (pAE->ntldrDump == FALSE && pMMT->Shared == FALSE) {
        /* Determine which CQ to look in based on start state */
        if (pAE->DriverState.NextDriverState == NVMeStartComplete) {

            firstCheckQueue = lastCheckQueue = pMMT->CplQueueNum;
        } else if (pAE->DriverState.NextDriverState == NVMeWaitOnLearnMapping) {

            /* while learning we setup the CT so that this is always true */
            firstCheckQueue = lastCheckQueue = (USHORT)MsgID + 1;
        }
		/* Determine which CQ to look in based on WaitOnNamespaceReady state */
		else if (pAE->DriverState.NextDriverState == NVMeWaitOnNamespaceReady){
			firstCheckQueue = lastCheckQueue = pMMT->CplQueueNum;
		}
        /* else we're init'ing so admin queue is in use */
    } else {
        /*
         * when Qs share an MSI, the we don't learn anything about core
         * mapping, etc., we just look through all of the queues all the time
         */
        firstCheckQueue = 0;
        lastCheckQueue = (USHORT)pQI->NumCplIoQCreated;
    }

    /* loop through all the queues we've decided we need to look at */
    indexCheckQueue = firstCheckQueue;
    do {
        pCQI = pQI->pCplQueueInfo + indexCheckQueue;
        pSQI = pQI->pSubQueueInfo + indexCheckQueue;
        indexCheckQueue++;
        /* loop through each queue itself */
        do {
            entryStatus = NVMeGetCplEntry(pAE, pCQI, &pCplEntry);
            if (entryStatus == STOR_STATUS_SUCCESS) {
                /*
                 * Mask the interrupt only when first pending completed entry
                 * found.
                 */
                if ((pRMT->InterruptType == INT_TYPE_INTX) &&
                    (pAE->IntxMasked == FALSE)) {
                    StorPortWriteRegisterUlong(pAE,
                                               &pAE->pCtrlRegister->IVMS,
                                               1);

                    pAE->IntxMasked = TRUE;
                }

                InterruptClaimed = TRUE;

#pragma prefast(suppress:6011,"This pointer is not NULL")
                completeStatus = NVMeCompleteCmd(pAE,
                                                 pCplEntry->DW2.SQID,
                                                 pCplEntry->DW2.SQHD,
                                                 pCplEntry->DW3.CID,
                                                (PVOID)&pSrbExtension);
                if (completeStatus == FALSE) {
                    return;
                }
#ifdef HISTORY
                TracePathComplete(COMPPLETE_CMD, pCplEntry->DW2.SQID,
                    pCplEntry->DW3.CID, pCplEntry->DW2.SQHD,
                    pCplEntry->DW3,
                    (ULONGLONG)pSrbExtension->pNvmeCompletionRoutine,
                    0);
#endif

                if (pSrbExtension != NULL) {
                    BOOLEAN callStorportNotification = FALSE;
#if DBG
                    /* for checked builds, sanity check our learning mode */
                    PROCESSOR_NUMBER procNum;

                    if ((pAE->LearningComplete == TRUE) && (firstCheckQueue > 0)) {
                        StorPortPatchGetCurrentProcessorNumber((PVOID)pAE, &procNum);
                        if ((pSrbExtension->procNum.Group != procNum.Group) ||
                            (pSrbExtension->procNum.Number != procNum.Number))           
                            StorPortDebugPrint(INFO, 
                                "Affinity Check Failed: sub:grp(%d)core(%d), cpl:grp(%d)core(%d)\n",
                                pSrbExtension->procNum.Group, pSrbExtension->procNum.Number,
                                procNum.Group, procNum.Number);
                    }
#endif

                    pSrbExtension->pCplEntry = pCplEntry;

                    /*
                     * If we're learning and this is an IO queue then update
                     * the PCT to note which QP to start using for this core
                     */
                    if (pAE->DriverState.NextDriverState == NVMeWaitOnLearnMapping) {
                        PCORE_TBL pCT = NULL;
                        PQUEUE_INFO pQI = &pAE->QueueInfo;
                        PCPL_QUEUE_INFO pCQI = NULL;
                        PROCESSOR_NUMBER procNum;
                        ULONG coreNum = 0;
                        PPROC_GROUP_TBL pPGT = NULL;

                        pMMT = pRMT->pMsiMsgTbl + MsgID;

                        /* 
                         *  Make sure we don't overwrite this MSI vector 
                         *  if it's already been learned 
                         */
                        
                        if (pMMT->Learned == FALSE) {
                            pMMT->Learned = TRUE;

                            StorPortPatchGetCurrentProcessorNumber((PVOID)pAE,
                                     &procNum);

                            /* Figure out the final core number with group number */
                            pPGT = pRMT->pProcGroupTbl + procNum.Group;
                            coreNum = (ULONG)(procNum.Number + pPGT->BaseProcessor);
                            
                            /* reference appropriate tables */
                            pCT = pRMT->pCoreTbl + coreNum;

                            /* assign new queue pair */
                            /*Only if MSIGranted is less than active core
                            the QueueNo will be remapped in sequential manner
                            Otherwise QueueNo remains same as before*/
                            if (pAE->ResMapTbl.NumMsiMsgGranted < pAE->ResMapTbl.NumActiveCores) {
                                pCT->CplQueue = pCT->SubQueue = (USHORT)pQI->NumIoQMapped;
                            }
                            pCQI = pQI->pCplQueueInfo + pCT->CplQueue;

                            /* update based on current completion info */
                            pCT->MsiMsgID = (USHORT)MsgID;
                            pCT->Learned = TRUE;
                            pCQI->MsiMsgID = pCT->MsiMsgID;
                            pMMT->CplQueueNum = pCT->CplQueue;

                            /* increment our learning counter */
                            pAE->LearningCores++;
                            pQI->NumIoQMapped++;
                            StorPortDebugPrint(INFO, 
                                "Mapped#%d: core(%d) to MSI ID(%d)\n",
                                    pAE->LearningCores, coreNum, MsgID);
                            /* free the read buffer for learning IO */
                            ASSERT(pSrbExtension->pDataBuffer);
                            if (NULL != pSrbExtension->pDataBuffer) {
                                StorPortFreePool((PVOID)pAE, pSrbExtension->pDataBuffer);
                                pSrbExtension->pDataBuffer = NULL;
                            }
                        }
                    }
					
					if (pSrbExtension->pNvmeCompletionRoutine == NULL) {
                        /*
                         * if no comp routine, call only if we had a valid
                         * status translation, otherwise let it timeout if
                         * if was host based
                         */
                        callStorportNotification =
                            SntiMapCompletionStatus(pSrbExtension);
                    } else {
                        /*
                         * if we have a completion routine, call it and then
                         * complete onlt if this was a host request (srb exsits)
                         * In this case the completion routine is responsible
                         * for mapping Srb status
                         */
                        callStorportNotification =
                            pSrbExtension->pNvmeCompletionRoutine(pAE, (PVOID)pSrbExtension)
                            && (pSrbExtension->pSrb != NULL);
                    }
                    /*
                     * This is to signal to NVMeIsrMsix()and ultimately ProcessIo() in dump mode
                     *  that the Admin request has been completed.
                     */
                    if (pAE->ntldrDump && pSystemArgument1 != NULL) {
                        *(BOOLEAN*)pSystemArgument1 = TRUE;
                    }

                    /* for async calls, call storport if needed */
                    if (callStorportNotification) {
                        IO_StorPortNotification(RequestComplete,
                                                pAE,
                                                pSrbExtension->pSrb);
                    }
                } /* If there was an SRB Extension */
            } /* If a completed command was collected */
        } while (entryStatus == STOR_STATUS_SUCCESS);

        if (InterruptClaimed == TRUE) {
            /* Now update the Completion Head Pointer via Doorbell register */
            StorPortWriteRegisterUlong(pAE,
                                       pCQI->pCplHDBL,
                                       (ULONG)pCQI->CplQHeadPtr);
            InterruptClaimed = FALSE;
        }
        /*
         * If we serviced another queue on MSIX0 then we also have to check
         * the admin queue (admin queue shared with one other QP)
         */
        if ((firstCheckQueue > 0) &&
            (MsgID == 0)) {
            firstCheckQueue = lastCheckQueue = indexCheckQueue = 0;
        }
    } while (indexCheckQueue <= lastCheckQueue); /* end queue checking loop */

    /* Un-mask interrupt if it had been masked */
    if (pAE->IntxMasked == TRUE) {
        StorPortWriteRegisterUlong(pAE, &pAE->pCtrlRegister->INTMC, 1);
        pAE->IntxMasked = FALSE;
    }
    if (pDpc != NULL) {
        if (pAE->MultipleCoresToSingleQueueFlag) {
            StorPortReleaseSpinLock(pAE, &StartLockHandle);
        } else {
            StorPortReleaseSpinLock(pAE, &DpcLockhandle);
        }
    }
} /* IoCompletionRoutine */


/*******************************************************************************
 * NVMeIsrMsix
 *
 * @brief MSI-X interupt routine
 *
 * @param AdapterExtension - Pointer to device extension
 * @param MsgId - MSI-X message Id to be parsed
 *
 * @return BOOLEAN
 *     TRUE - Indiciates successful completion
 *     FALSE - Unsuccessful completion or error
 ******************************************************************************/
BOOLEAN
NVMeIsrMsix (
    __in PVOID AdapterExtension,
    __in ULONG MsgID )
{
    PNVME_DEVICE_EXTENSION        pAE = (PNVME_DEVICE_EXTENSION)AdapterExtension;
    PRES_MAPPING_TBL              pRMT = &pAE->ResMapTbl;
    PMSI_MESSAGE_TBL              pMMT = pRMT->pMsiMsgTbl;
    ULONG                         qNum = 0;
    BOOLEAN                       status = FALSE;

    if (pAE->polledResetInProg)
        return TRUE;

    if (pAE->ntldrDump == TRUE) {
        /* status will return TRUE, if the request has been completed. */
        IoCompletionRoutine(NULL, AdapterExtension, &status, 0);
        return status;
    }
    
    /*
     * For shared mode, we'll use the DPC for queue 0,
     * otherwise we'll use the DPC assoiated with the known
     * queue number
     */
    if (pMMT->Shared == FALSE) {
        pMMT = pRMT->pMsiMsgTbl + MsgID;
        qNum = pMMT->CplQueueNum;
    }

    StorPortIssueDpc(pAE,
                (PSTOR_DPC)pAE->pDpcArray + qNum,
                (PVOID)MsgID,
                NULL);

    return TRUE;
}



/*******************************************************************************
 * NVMeWaitForCtrlRDY
 *
 * @brief helper routine to wait for controller status RDY transition
 *
 * @param pAE - Pointer to device extension
 *
 * @return
 *     TRUE - Indiciates successful completion
 *     FALSE - Unsuccessful completion or error
 ******************************************************************************/
BOOLEAN NVMeWaitForCtrlRDY(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in ULONG expectedValue
)
{   
    NVMe_CONTROLLER_STATUS CSTS = {0};
    ULONG time = 0;

     CSTS.AsUlong =
         StorPortReadRegisterUlong(pAE,
                                   &pAE->pCtrlRegister->CSTS.AsUlong);
     while (CSTS.RDY != expectedValue) {
        NVMeStallExecution(pAE, MAX_STATE_STALL_us);
        time += STORPORT_TIMER_CB_us;
        if (time > pAE->uSecCrtlTimeout) {
            return FALSE;
        }
        CSTS.AsUlong =
            StorPortReadRegisterUlong(pAE,
                                      &pAE->pCtrlRegister->CSTS.AsUlong);

        if(CSTS.AsUlong == INVALID_DEVICE_REGISTER_VALUE) {
            /* During hot removal, return immediately */
            return FALSE;
        }
     }

     return TRUE;
}

/*******************************************************************************
 * RecoveryDpcRoutine
 *
 * @brief DPC routine for recovery and resets
 *
 * @param pDpc - Pointer to DPC
 * @param pHwDeviceExtension - Pointer to device extension
 * @param pSystemArgument1
 * @param pSystemArgument2
 *
 * @return VOID
 ******************************************************************************/
VOID RecoveryDpcRoutine(
    IN PSTOR_DPC pDpc,
    IN PVOID pHwDeviceExtension,
    IN PVOID pSystemArgument1,
    IN PVOID pSystemArgument2
)
{
    PNVME_DEVICE_EXTENSION pAE = (PNVME_DEVICE_EXTENSION)pHwDeviceExtension;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = (PSTORAGE_REQUEST_BLOCK)pSystemArgument1;
#else
    PSCSI_REQUEST_BLOCK pSrb = (PSCSI_REQUEST_BLOCK)pSystemArgument1;
#endif
    
    STOR_LOCK_HANDLE startLockhandle = { 0 };

    StorPortDebugPrint(INFO, "RecoveryDpcRoutine: Entry\n");
#ifdef HISTORY
    TraceEvent(DPC_RESET,0,0,0,0, 0,0);
#endif
    /*
     * Get spinlocks in order, this assures we don't have submission or
     * completion threads happening before or during reset
     */
    StorPortAcquireSpinLock(pAE, StartIoLock, NULL, &startLockhandle);

    /*
     * Reset the controller; if any steps fail we just quit which
     * will leave the controller un-usable(storport queues frozen)
     * on purpose to prevent possible data corruption
     */
    if (NVMeResetAdapter(pAE) == TRUE) {
        /* 10 msec "settle" delay post reset */
        NVMeStallExecution(pAE, 10000);

        /* Complete outstanding commands on submission queues */
        StorPortNotification(ResetDetected, pAE, 0);

       /*
        * detect and complete all commands
        */
        NVMeDetectPendingCmds(pAE, TRUE, SRB_STATUS_BUS_RESET);

        /*
         * Don't need to hold this anymore, we won't accept new IOs until the
         * init state machine has completed.
         */
        StorPortReleaseSpinLock(pAE, &startLockhandle);

        /* Prepare for new commands */
        if (NVMeInitAdminQueues(pAE) == STOR_STATUS_SUCCESS) {
            /*
             * Start the state machine, if all goes well we'll complete the
             * reset Srb when the machine is done.
             */
            NVMeRunningStartAttempt(pAE, TRUE, pSrb);
        } /* init the admin queues */
    } else {
		pAE->DeviceRemovedDuringIO = TRUE;
#if (NTDDI_VERSION > NTDDI_WIN7)
		if (pAE->Timerhandle != NULL) {
			StorPortFreeTimer(pAE, pAE->Timerhandle);
			pAE->Timerhandle = NULL;
		}
#endif

		NVMeDetectPendingCmds(pAE, TRUE, SRB_STATUS_ERROR);
        StorPortReleaseSpinLock(pAE, &startLockhandle);
        NVMeFreeBuffers(pAE);
		StorPortResume(pAE);
    }  /* reset the controller */
} /* RecoveryDpcRoutine */

/*******************************************************************************
 * NVMeResetController
 *
 * @brief Main routine entry for resetting the controller
 *
 * @param pHwDeviceExtension - Pointer to device extension
 * @param pSrb - Pointer to SRB
 *
 * @return BOOLEAN
 *    TRUE - DPC was successufully issued
 *    FALSE - DPC was unsuccessufully issued or recovery not possible
 ******************************************************************************/
BOOLEAN NVMeResetController(
    __in PNVME_DEVICE_EXTENSION pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
    __in PSTORAGE_REQUEST_BLOCK pSrb
#else
    __in PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    BOOLEAN storStatus = FALSE;

    /* In Hibernation/CrashDump mode, reset command is ignored. */
    if (pAdapterExtension->ntldrDump == TRUE) {
        return TRUE; 
    }
    
    /**
     * We only allow one recovery attempt at a time
     * if the DPC is sceduled then one has started,
     * when completed and we're ready for IOs again
     * we'll set the flag to allow recovery again.
     * Recoery runs at DPC level and grabs the startio
     * and INT spinlocks to assure no submission or
     * completion threads are in progress during reset
     */
    if (pAdapterExtension->RecoveryAttemptPossible == TRUE) {
        /* We don't want any new stoport requests during reset */
        StorPortPause(pAdapterExtension, STOR_ALL_REQUESTS);
        StorPortDebugPrint(INFO,
                       "NVMeResetController: Issue DPC.\n");
        storStatus = StorPortIssueDpc(pAdapterExtension,
                                      &pAdapterExtension->RecoveryDpc,
                                      pSrb,
                                      NULL);

        if (storStatus == TRUE) {
            pAdapterExtension->RecoveryAttemptPossible = FALSE;
            StorPortDebugPrint(INFO,
                       "NVMeResetController: Issue DPC succeeds.\n");
        }
    } else {
        DbgPrint("NVMeResetController: reset called but already pending\n");
        storStatus = TRUE;
    }

    return storStatus;
} /* NVMeResetController */

/*******************************************************************************
 * NVMeReInitializeController
 *
 * @This is the syncrhonized routine that resets NVMe controller.  This routine
 * is only called from NVMeResetBus or NVMeAdapterControl->ScsRestartAdapter 
 * paths, both of which are synchronous. i.e., must not return to caller 
 * till done
 *
 * @param pAE - Pointer to device extension
 *
 * @return BOOLEAN
 ******************************************************************************/
BOOLEAN NVMeReInitializeController(
    __in PNVME_DEVICE_EXTENSION pAE
    )
{
    ULONG passiveTimeout;

    if (pAE->polledResetInProg)
        return TRUE;

    pAE->polledResetInProg = TRUE;

    /* pause the adapter to block any new I/Os */
    StorPortPause(pAE, STOR_ALL_REQUESTS);

    /*
    * perform the reset operations, Reset the controller; if any steps fail we
    * just quit which will leave the controller un-usable(storport queues frozen)
    * on purpose to prevent possible data corruption
    */
    if (NVMeResetAdapter(pAE) == TRUE) {

        /* Complete outstanding commands on submission queues */
        StorPortNotification(ResetDetected, pAE, 0);

        /*
        * detect and complete all commands
        */
        NVMeDetectPendingCmds(pAE, TRUE, SRB_STATUS_BUS_RESET);

        /* Prepare for new commands */
        if (NVMeInitAdminQueues(pAE) == STOR_STATUS_SUCCESS) {
            /*
            * Start the state machine, if all goes well we'll complete the
            * reset Srb when the machine is done.
            */
            NVMeRunningStartAttempt(pAE, FALSE, NULL);
        
        } /* init the admin queues */
	} else {
		pAE->DeviceRemovedDuringIO = TRUE;
#if (NTDDI_VERSION > NTDDI_WIN7)
		if (pAE->Timerhandle != NULL) {
			StorPortFreeTimer(pAE, pAE->Timerhandle);
			pAE->Timerhandle = NULL;
		}
#endif

		NVMeDetectPendingCmds(pAE, TRUE, SRB_STATUS_ERROR);
		NVMeFreeBuffers(pAE);
	}

    pAE->polledResetInProg = FALSE;

    /* if init state machine completed successfully, we allow IOs to resume */
	if ((pAE->DriverState.NextDriverState == NVMeStartComplete) && (pAE->DeviceRemovedDuringIO == FALSE)) {
		NVMeRunning(pAE);
    } else {
        StorPortResume(pAE);
    }

    return (pAE->DriverState.NextDriverState == NVMeStartComplete) ? TRUE : FALSE;
} /* NVMeReInitializeController */

/*******************************************************************************
 * NVMeResetBus
 *
 * @brief Main routine entry for resetting the bus
 *
 * @param pHwDeviceExtension - Pointer to device extension
 * @param PathId - SCSI Path Id (bus)
 *
 * @return BOOLEAN
 ******************************************************************************/
BOOLEAN NVMeResetBus(
    __in PVOID AdapterExtension,
    __in ULONG PathId
)
{
    PNVME_DEVICE_EXTENSION pAE = (PNVME_DEVICE_EXTENSION)AdapterExtension;

    UNREFERENCED_PARAMETER(PathId);

    /* In Hibernation/CrashDump mode, reset command is ignored. */
    if (pAE->ntldrDump == TRUE) {
        return TRUE;
    }

    return NVMeReInitializeController(AdapterExtension);
} /* NVMeResetBus */

/*******************************************************************************
 * NVMeInitSrbExtension
 *
 * @brief Helper function to initialize the SRB extension
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pDevExt - Pointer to device extension
 * @param pSrb - Pointer to SRB
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeInitSrbExtension(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    memset(pSrbExt, 0, sizeof(NVME_SRB_EXTENSION));

    pSrbExt->pNvmeDevExt = pDevExt;
    pSrbExt->pSrb = pSrb;

    /* Any future initializations go here... */
} /* NVMeInitSrbExtension */

/*******************************************************************************
 * NVMeIoctlCallback
 *
 * @brief NVMeIoctlCallback is the callback function used to complete IOCTL
 *        requests. This routine needs to finish the following before returning
 *        to Storport:
 *
 *        - Modify DataTransferLength of SRB as the sum of ReturnBufferLen and
 *          sizeof(NVME_PASS_THROUGH_IOCTL) if transferring data from device to
 *          host.
 *        - Modify DataTransferLength of SRB as the
 *          sizeof(NVME_PASS_THROUGH_IOCTL) if transferring data from host to
 *          device.
 *        - Fill CplEntry of NVME_PASS_THROUGH_IOCTL with the entire completion
 *          entry before completing the request back to Storport.
 *        - Always mark SrbStatus and ReturnCode of SRB_IO_CONTROL as SUCCESS to
 *          hint user applications to exam the completion entry.
 *        - For IOCTL_SCSI_MINIPORT_SMART_READ_ATTRIBS it handles the NVMe Get
 *          Log Page command, converts it to the ATA SMART definition and
 *          returns it back in the SRB buffer.
 *        - For IOCTL_SCSI_MINIPORT_SMART_READ_THRESHOLDS it handles the NVMe
 *          Get Log Page command, converts it to the ATA SMART definition,
 *          issues an NVMe Get Features command for the temperature threshold
 *          via ProcessIo, handles and converts that and then returns them in
 *          the SRB buffer. That means this callback will get called twice. The
 *          first time it will return FALSE so that the SRB will not be
 *          completed and the second time will return TRUE to complete it.
 *
 * @param pNVMeDevExt - Pointer to hardware device extension.
 * @param pSrbExtension - Pointer to SRB extension
 *
 * @return BOOLEAN
 *     TRUE - If the command is completed
 *     FALSE - If additional processing is required (i.e. for thresholds)
 ******************************************************************************/
BOOLEAN NVMeIoctlCallback(
    PVOID pNVMeDevExt,
    PVOID pSrbExtension
)
{
    PNVME_SRB_EXTENSION pSrbExt = (PNVME_SRB_EXTENSION)pSrbExtension;
    PSRB_IO_CONTROL pSic = (PSRB_IO_CONTROL)GET_DATA_BUFFER(pSrbExt->pSrb);
    BOOLEAN srbDone = TRUE;

    switch (pSic->ControlCode) {
        case NVME_PASS_THROUGH_SRB_IO_CODE:
            srbDone = NVMeHandleNVMePassthrough(pNVMeDevExt, pSrbExt);
        break;
        case IOCTL_SCSI_MINIPORT_READ_SMART_ATTRIBS:
            srbDone = NVMeHandleSmartAttribs(pNVMeDevExt, pSrbExt);
        break;
        case IOCTL_SCSI_MINIPORT_READ_SMART_THRESHOLDS:
            srbDone = NVMeHandleSmartThresholds(pNVMeDevExt, pSrbExt);
        break;
        default:
            ASSERT(FALSE);
            SET_DATA_LENGTH(pSrbExt->pSrb, 0);
            pSrbExt->pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            pSic->ReturnCode = SRB_STATUS_ERROR;
        break;
    }

    return srbDone;
} /* NVMeIoctlCallback */

/*******************************************************************************
 * NVMeHandleNVMePassthrough
 *
 * @brief Handles NVMe Pass Through
 *
 * @param pNVMeDevExt - Pointer to hardware device extension.
 * @param pSrbExtension - Pointer to SRB extension
 *
 * @return BOOLEAN
 *     TRUE - If the command is completed
 *     FALSE - If additional processing is required
 ******************************************************************************/
BOOLEAN NVMeHandleNVMePassthrough(
    PVOID pNVMeDevExt,
    PNVME_SRB_EXTENSION pSrbExtension
)
{
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExtension->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExtension->pSrb;
#endif
    PNVMe_COMMAND_DWORD_0 pNvmeCmdDW0 = NULL;
    
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);

    /* Adjust the SRB transfer length */
    if (pNvmePtIoctl->Direction == NVME_FROM_HOST_TO_DEV) {
        SET_DATA_LENGTH(pSrb, sizeof(NVME_PASS_THROUGH_IOCTL));
    } else if (pNvmePtIoctl->Direction == NVME_FROM_DEV_TO_HOST) {
        SET_DATA_LENGTH(pSrb, pNvmePtIoctl->ReturnBufferLen);
    }

    /* Copy the completion entry to NVME_PASS_THROUGH_IOCTL structure */
    StorPortCopyMemory((PVOID)pNvmePtIoctl->CplEntry,
                        (PVOID)pSrbExtension->pCplEntry,
                       sizeof(NVMe_COMPLETION_QUEUE_ENTRY));
    pNvmeCmdDW0 = (PNVMe_COMMAND_DWORD_0)&pNvmePtIoctl->NVMeCmd[0];

    switch (pNvmeCmdDW0->OPC) {
    case ADMIN_NAMESPACE_MANAGEMENT:
        return NVMeCompletionNsMgmt(pNVMeDevExt, pSrbExtension);
    break;
    case ADMIN_NAMESPACE_ATTACHMENT:
        return NVMeCompletionNsAttachment(pNVMeDevExt, pSrbExtension);
    break;
    default:
    break;
    } /* state switch */

    /*
     * Mark down the ReturnCode in SRB_IO_CONTROL as NVME_IOCTL_SUCCESS
     * and SrbStatus as SRB_STATUS_SUCCESS before returning.
     */
    pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_SUCCESS;
    pSrb->SrbStatus = SRB_STATUS_SUCCESS;

    return TRUE;
}

/*******************************************************************************
 * NVMeHandleSmartAttribs
 *
 * @brief Handles IOCTL_SCSI_MINIPORT_SMART_READ_ATTRIBS
 *
 * @param pNVMeDevExt - Pointer to hardware device extension.
 * @param pSrbExtension - Pointer to SRB extension
 *
 * @return BOOLEAN
 *     TRUE - If the command is completed
 *     FALSE - If additional processing is required
 ******************************************************************************/
BOOLEAN NVMeHandleSmartAttribs(
    PVOID pNVMeDevExt,
    PNVME_SRB_EXTENSION pSrbExtension
)
{
    PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY logPageData = NULL;
    PSENDCMDOUTPARAMS pCmdOutParameters = (PSENDCMDOUTPARAMS)GET_DATA_BUFFER(pSrbExtension->pSrb);
    PNVME_SMART_READ_ATTRIBUTES_DATA smartAttribs =
        (PNVME_SMART_READ_ATTRIBUTES_DATA)
        pCmdOutParameters->bBuffer;

    pCmdOutParameters->cBufferSize = sizeof(PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY);

    if (pSrbExtension->pCplEntry->DW3.SF.SCT != 0) {
        /*
         * Don't care what the error was, if an error occured then just return
         * error
         */
        SET_DATA_LENGTH(pSrbExtension->pSrb, 0);
        pSrbExtension->pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        smartAttribs->SrbIoCtrl.ReturnCode = SRB_STATUS_ERROR;
    } else {
        logPageData =
            (PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY)
            pSrbExtension->pDataBuffer;

        /* Build Drive Temperature Attribute */
        #define SMART_TEMP_SHIFT (8)
        smartAttribs->DriveTemperature.Code  = DRIVE_TEMPERATURE_CODE;
        smartAttribs->DriveTemperature.Value = logPageData->Temperature >> SMART_TEMP_SHIFT;

        /* Build Reallocated Sectors Count Attribute */
        smartAttribs->ReallocatedSectorsCount.Code  =
            REALLOCATED_SECTORS_COUNT_CODE;
        smartAttribs->ReallocatedSectorsCount.Value =
            logPageData->AvailableSpare;

        /* Build Endurance Remaining Attribute */
        smartAttribs->EnduranceRemaining.Code  = ENDURANCE_REMAINING_CODE;
        smartAttribs->EnduranceRemaining.Value = logPageData->PercentageUsed;

        /* Build LBAs Read Attribute */
        #define NIBBLE_MASK (0xf)
        smartAttribs->LBAsRead.Code  = LBAS_READ_CODE;
        smartAttribs->LBAsRead.Value =
            logPageData->HostReadCommands.Lower & NIBBLE_MASK;

        /* Build LBAs Written Attribute */
        smartAttribs->LBAsWritten.Code  = LBAS_WRITTEN_CODE;
        smartAttribs->LBAsWritten.Value =
            logPageData->HostWriteCommands.Lower & NIBBLE_MASK;

        /* Build Loaded Hours Attribute */
        smartAttribs->LoadedHours.Code  = LOADED_HOURS_CODE;
        smartAttribs->LoadedHours.Value =
            logPageData->ControllerBusyTime.Lower & NIBBLE_MASK;

        /* Build Power Cycle Count Attribute */
        smartAttribs->PowerCycleCount.Code  = POWER_CYCLE_COUNT_CODE;
        smartAttribs->PowerCycleCount.Value =
            logPageData->PowerCycles.Lower & NIBBLE_MASK;

        /* Build Power-On Hours Attribute */
        smartAttribs->PowerOnHours.Code  = POWER_ON_HOURS_CODE;
        smartAttribs->PowerOnHours.Value =
            logPageData->PowerOnHours.Lower & NIBBLE_MASK;

        /* Build Reported Uncorrectable Errors Attribute */
        smartAttribs->ReportedUncorrectableErrors.Code  =
            REPORTED_UNCORRECTABLE_ERRORS_CODE;
        smartAttribs->ReportedUncorrectableErrors.Value =
            logPageData->MediaErrors.Lower & NIBBLE_MASK;

        pSrbExtension->pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        smartAttribs->SrbIoCtrl.ReturnCode = SRB_STATUS_SUCCESS;
        SET_DATA_LENGTH(pSrbExtension->pSrb, sizeof(NVME_SMART_READ_ATTRIBUTES_DATA));
    }

    /* Free the 4K Data Buffer */
    StorPortFreePool(pNVMeDevExt, pSrbExtension->pDataBuffer);
    pSrbExtension->pDataBuffer = NULL;

    return TRUE;
}

/*******************************************************************************
 * NVMeHandleSmartThresholds
 *
 * @brief Handles IOCTL_SCSI_MINIPORT_SMART_READ_THRESHOLDS
 *
 * @param pNVMeDevExt - Pointer to hardware device extension.
 * @param pSrbExtension - Pointer to SRB extension
 *
 * @return BOOLEAN
 *     TRUE - If the command is completed
 *     FALSE - If additional processing is required
 ******************************************************************************/
BOOLEAN NVMeHandleSmartThresholds(
    PVOID pNVMeDevExt,
    PNVME_SRB_EXTENSION pSrbExtension
)
{
    PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY logPageData = NULL;
    PSENDCMDOUTPARAMS pCmdOutParameters = (PSENDCMDOUTPARAMS)GET_DATA_BUFFER(pSrbExtension->pSrb);
    PNVME_SMART_READ_THRESHOLDS_DATA smartThresholds =
        (PNVME_SMART_READ_THRESHOLDS_DATA)
        pCmdOutParameters->bBuffer;
    BOOLEAN srbDone = TRUE;

    pCmdOutParameters->cBufferSize = sizeof(PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY);

    /* Thresholds is handled in two phases */
    if (pSrbExtension->nvmeSqeUnit.CDW0.OPC == ADMIN_GET_LOG_PAGE) {
        if (pSrbExtension->pCplEntry->DW3.SF.SCT != 0) {
            /*
             * Don't care what the error was, if an error occured then just
             * return error
             */
            SET_DATA_LENGTH(pSrbExtension->pSrb, 0);
            pSrbExtension->pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            smartThresholds->SrbIoCtrl.ReturnCode = SRB_STATUS_ERROR;

            /* Free the 4K Data Buffer */
            StorPortFreePool(pNVMeDevExt, pSrbExtension->pDataBuffer);
            pSrbExtension->pDataBuffer = NULL;
        } else {
            logPageData =
                (PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY)
                pSrbExtension->pDataBuffer;

            /* Build Reallocated Sectors Count Attribute */
            smartThresholds->ReallocatedSectorsCount.Code =
                REALLOCATED_SECTORS_COUNT_CODE;
            smartThresholds->ReallocatedSectorsCount.Value =
                logPageData->AvailableSpareThreshold;

            /* Set up the GET FEATURES command */
            memset(&pSrbExtension->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
            pSrbExtension->nvmeSqeUnit.CDW0.OPC = ADMIN_GET_FEATURES;
            pSrbExtension->nvmeSqeUnit.CDW0.CID = 0;
            pSrbExtension->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

            /* DWORD 10 */
            pSrbExtension->nvmeSqeUnit.CDW10 |= TEMPERATURE_THRESHOLD;

            /* Issue the Get Features Command */
            ProcessIo((PNVME_DEVICE_EXTENSION)pNVMeDevExt,
                pSrbExtension, NVME_QUEUE_TYPE_ADMIN, FALSE);

            /* Do not complete this SRB yet */
            srbDone = FALSE;
        }
    } else if (pSrbExtension->nvmeSqeUnit.CDW0.OPC == ADMIN_GET_FEATURES) {
        if (pSrbExtension->pCplEntry->DW3.SF.SCT != 0) {
            /*
             * Don't care what the error was, if an error occured then
             * just return error
             */
            SET_DATA_LENGTH(pSrbExtension->pSrb, 0);
            pSrbExtension->pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            smartThresholds->SrbIoCtrl.ReturnCode = SRB_STATUS_ERROR;
        } else {
            /* Build Drive Temperature Attribute */
            smartThresholds->DriveTemperature.Code  = DRIVE_TEMPERATURE_CODE;
            smartThresholds->DriveTemperature.Value =
                pSrbExtension->pCplEntry->DW0 & NIBBLE_MASK;

            pSrbExtension->pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            smartThresholds->SrbIoCtrl.ReturnCode = SRB_STATUS_SUCCESS;
            SET_DATA_LENGTH(pSrbExtension->pSrb,
                sizeof(NVME_SMART_READ_THRESHOLDS_DATA));
        }

        /* Free the 4K Data Buffer */
        StorPortFreePool(pNVMeDevExt, pSrbExtension->pDataBuffer);
        pSrbExtension->pDataBuffer = NULL;
    } else {
        SET_DATA_LENGTH(pSrbExtension->pSrb, 0);
        pSrbExtension->pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        smartThresholds->SrbIoCtrl.ReturnCode = SRB_STATUS_ERROR;

        /* Free the 4K Data Buffer */
        StorPortFreePool(pNVMeDevExt, pSrbExtension->pDataBuffer);
        pSrbExtension->pDataBuffer = NULL;
    }

    return srbDone;
}


/*******************************************************************************
 * NVMeIoctlGetLogPage
 *
 * @brief NVMeIoctlGetLogPage handles the GET LOG PAGE command
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeIoctlGetLogPage(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PADMIN_GET_LOG_PAGE_COMMAND_DW10 pGetLogPageDW10 = NULL;
    ULONG DataBufferSize = 0;
    ULONG IoctlHdrSize = sizeof(NVME_PASS_THROUGH_IOCTL);

    pGetLogPageDW10 =
        (PADMIN_GET_LOG_PAGE_COMMAND_DW10)&pNvmePtIoctl->NVMeCmd[10];
    /* DW10 is zero based, so don't forget to add 1 */
    DataBufferSize = (pGetLogPageDW10->NUMD + 1) * sizeof(ULONG);

    /*
     * Ensure the size of return buffer is big enough to accommodate the header
     * and log.
     */
    if (pNvmePtIoctl->ReturnBufferLen < (IoctlHdrSize + DataBufferSize - 1)) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_OUT_BUFFER;
        return IOCTL_COMPLETED;
    }

    /* Prepare the PRP entries for the transfer */
    if (NVMePreparePRPs(pDevExt,
                        (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb),
                        pNvmePtIoctl->DataBuffer,
                        DataBufferSize) == FALSE) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;
        return IOCTL_COMPLETED;
    }

    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlGetLogPage */

/*******************************************************************************
 * NVMeIoctlIdentify
 *
 * @brief NVMeIoctlIdentify handles the Identify command
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeIoctlIdentify(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PADMIN_IDENTIFY_COMMAND_DW10 pIdentifyDW10 = NULL;
    ULONG DataBufferSize = 0;
    ULONG IoctlHdrSize = sizeof(NVME_PASS_THROUGH_IOCTL);

    pIdentifyDW10 = (PADMIN_IDENTIFY_COMMAND_DW10)&pNvmePtIoctl->NVMeCmd[10];

    if (pIdentifyDW10->CNS == 0 || pIdentifyDW10->CNS == 0x11)
        DataBufferSize = sizeof(ADMIN_IDENTIFY_NAMESPACE);
    else if (pIdentifyDW10->CNS == 1)
        DataBufferSize = sizeof(ADMIN_IDENTIFY_CONTROLLER);
    else
        DataBufferSize = IDENTIFY_LIST_SIZE;


    /*
     * Ensure the size of return buffer is big enough to accommodate the header
     * and log.
     */
    if (pNvmePtIoctl->ReturnBufferLen < (IoctlHdrSize + DataBufferSize - 1)) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_OUT_BUFFER;
        return IOCTL_COMPLETED;
    }

    /* Prepare the PRP entries for the transfer */
    if (NVMePreparePRPs(pDevExt,
                        (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb),
                        pNvmePtIoctl->DataBuffer,
                        DataBufferSize) == FALSE) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;
        return IOCTL_COMPLETED;
    }

    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlIdentify */

/*******************************************************************************
 * NVMeIoctlFwDownload
 *
 * @brief NVMeIoctlFwDownload handles the FW Download command
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeIoctlFwDownload(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PADMIN_FIRMWARE_IMAGE_DOWNLOAD_COMMAND_DW10 pFirmwareImageDW10 = NULL;
    ULONG DataBufferSize = 0;
    ULONG IoctlHdrSize = sizeof(NVME_PASS_THROUGH_IOCTL);
    PADMIN_IDENTIFY_CONTROLLER pAdminCntlrData =
        &pDevExt->controllerIdentifyData;
    USHORT supportsFwActFwDwnld = 0;

    supportsFwActFwDwnld =
        pAdminCntlrData->OACS.SupportsFirmwareActivateFirmwareDownload;

    pFirmwareImageDW10 = (PADMIN_FIRMWARE_IMAGE_DOWNLOAD_COMMAND_DW10)
                         &pNvmePtIoctl->NVMeCmd[10];
    /* DW10 is zero based, so don't forget to add 1 */
    DataBufferSize = (pFirmwareImageDW10->NUMD + 1) * sizeof(ULONG);

    /* Ensure the command is supported */
    if (supportsFwActFwDwnld == 0) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_UNSUPPORTED_ADMIN_CMD;

        return IOCTL_COMPLETED;
    }

    /* Ensure the size of input buffer is big enough to accommodate the header
     * and image.
     */
    if (pNvmePtIoctl->DataBufferLen < DataBufferSize) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_IN_BUFFER;

        return IOCTL_COMPLETED;
    }

    /* Prepare the PRP entries for the transfer */
    if (NVMePreparePRPs(pDevExt,
                        (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb),
                        pNvmePtIoctl->DataBuffer,
                        DataBufferSize) == FALSE) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;
        return IOCTL_COMPLETED;
    }

    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlFwDownload */

/*******************************************************************************
 * NVMeIoctlSetGetFeatures
 *
 * @brief NVMeIoctlSetGetFeatures handles the Get/Set Features command
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 * @param OPC - opcode
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeIoctlSetGetFeatures(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif    
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl,
    UCHAR OPC
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PADMIN_SET_FEATURES_COMMAND_DW10 pSetFeaturesDW10 = NULL;
    ULONG DataBufferSize = 0;
    ULONG IoctlHdrSize = sizeof(NVME_PASS_THROUGH_IOCTL);

    pSetFeaturesDW10 =
        (PADMIN_SET_FEATURES_COMMAND_DW10)&pNvmePtIoctl->NVMeCmd[10];

    /* Reject Number of Queues via Set Features command */
    if ((OPC == ADMIN_SET_FEATURES) &&
        (pSetFeaturesDW10->FID == NUMBER_OF_QUEUES)) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_UNSUPPORTED_ADMIN_CMD;

        return IOCTL_COMPLETED;
    }

    /* Only LBA_RANGE_TYPE feature needs pre-processing here */
    if (pSetFeaturesDW10->FID == LBA_RANGE_TYPE) {
        DataBufferSize =
            sizeof(ADMIN_SET_FEATURES_LBA_COMMAND_RANGE_TYPE_ENTRY);

        /* Ensure the size of input/output buffer is big enough */
        if (OPC == ADMIN_SET_FEATURES) {
            if (pNvmePtIoctl->DataBufferLen < DataBufferSize) {
                pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_IN_BUFFER;

                return IOCTL_COMPLETED;
            }
        } else if (OPC == ADMIN_GET_FEATURES) {
            if (pNvmePtIoctl->ReturnBufferLen <
                (DataBufferSize + IoctlHdrSize - 1)) {
                pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_OUT_BUFFER;

                return IOCTL_COMPLETED;
            }
        } else {
            pSrbIoCtrl->ReturnCode = NVME_IOCTL_UNSUPPORTED_ADMIN_CMD;
            return IOCTL_COMPLETED;
        }

        /* Prepare the PRP entries for the transfer */
        if (NVMePreparePRPs(pDevExt,
                            (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb),
                            pNvmePtIoctl->DataBuffer,
                            DataBufferSize) == FALSE) {
            pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;
            return IOCTL_COMPLETED;
        }
    }

    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlSetGetFeatures */

/*******************************************************************************
 * NVMeIoctlSecuritySendRcv
 *
 * @brief NVMeIoctlSecuritySendRcv handles the Secuirty Send/Rcv commands
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 * @param OPC - opcode
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeIoctlSecuritySendRcv(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif    
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl,
    UCHAR OPC
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PADMIN_SECURITY_SEND_COMMAND_DW11 pSecuritySendDW11 = NULL;
    ULONG IoctlHdrSize = sizeof(NVME_PASS_THROUGH_IOCTL);
    ULONG DataBufferSize = 0;
    USHORT supportsSecSendRcv = 0;
    PADMIN_IDENTIFY_CONTROLLER pAdminCntlrData =
        &pDevExt->controllerIdentifyData;

    supportsSecSendRcv =
        pAdminCntlrData->OACS.SupportsSecuritySendSecurityReceive;

    /* Ensure the command is supported */
    if (supportsSecSendRcv == 0) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_UNSUPPORTED_ADMIN_CMD;
        return IOCTL_COMPLETED;
    }

    pSecuritySendDW11 =
        (PADMIN_SECURITY_SEND_COMMAND_DW11)&pNvmePtIoctl->NVMeCmd[11];

    DataBufferSize = pSecuritySendDW11->AL;

    /* Ensure the size of input/output buffer is big enough */
    if (OPC == ADMIN_SECURITY_SEND) {
        if (pNvmePtIoctl->DataBufferLen < DataBufferSize) {
            pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_IN_BUFFER;

            return IOCTL_COMPLETED;
        }
    } else if (OPC == ADMIN_SECURITY_RECEIVE) {
        if (pNvmePtIoctl->ReturnBufferLen <
            (DataBufferSize + IoctlHdrSize - 1)) {
            pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_OUT_BUFFER;

            return IOCTL_COMPLETED;
        }
    } else {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_UNSUPPORTED_ADMIN_CMD;

        return IOCTL_COMPLETED;
    }

    if(DataBufferSize != 0)	{
        /* Prepare the PRP entries for the transfer */
        if (NVMePreparePRPs(pDevExt,
                            (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb),
                            pNvmePtIoctl->DataBuffer,
                            DataBufferSize) == FALSE) {
            pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;

            return IOCTL_COMPLETED;
        }
    }

    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlSecuritySendRcv */

/*******************************************************************************
 * NVMeIoctlCompare
 *
 * @brief NVMeIoctlCompare handles the Compare command
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeIoctlCompare(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
)
{
    PNVME_SRB_EXTENSION pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PNVM_COMPARE_COMMAND_DW12 pCompareDW12 = NULL;
    ULONG DataBufferSize = 0;
    ULONG IoctlHdrSize = sizeof(NVME_PASS_THROUGH_IOCTL);
    UINT8 flbas = 0;
    ULONG lbaLength = 0, lbaLengthPower = 0;

    if (GetLunExtension(pSrbExt, &pLunExt) != SNTI_SUCCESS) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INVALID_PATH_TARGET_ID;
        return IOCTL_COMPLETED;
    } else {
        pCompareDW12 = (PNVM_COMPARE_COMMAND_DW12)&pNvmePtIoctl->NVMeCmd[12];

        /* Need to figure out the byte size of LBA first */
        flbas = pLunExt->identifyData.FLBAS.SupportedCombination;
        lbaLengthPower = pLunExt->identifyData.LBAFx[flbas].LBADS;
        lbaLength = 1 << lbaLengthPower;

        /* Figure out the data size */
        DataBufferSize = lbaLength * (pCompareDW12->NLB + 1);

        /**
         * Ensure the size of input buffer is big enough to accommodate the
         * header and image.
         */
        if (pNvmePtIoctl->DataBufferLen < DataBufferSize) {
            pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_IN_BUFFER;

            return IOCTL_COMPLETED;
        }

        /* Prepare the PRP entries for the transfer */
        if (NVMePreparePRPs(pDevExt,
                            (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb),
                            pNvmePtIoctl->DataBuffer,
                            DataBufferSize) == FALSE) {
            pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;
            return IOCTL_COMPLETED;
        }
    }

    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlCompare */

/*******************************************************************************
 * NVMeIoctlDataSetManagement
 *
 * @brief NVMeIoctlDataSetManagement handles the DSM command
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeIoctlDataSetManagement(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PNVM_DATASET_MANAGEMENT_COMMAND_DW10 pDatasetManagementDW10 = NULL;
    ULONG DataBufferSize = 0;
    ULONG IoctlHdrSize = sizeof(NVME_PASS_THROUGH_IOCTL);

    pDatasetManagementDW10 = (PNVM_DATASET_MANAGEMENT_COMMAND_DW10)
                             &pNvmePtIoctl->NVMeCmd[12];

    /* Need to figure out the byte size data */
    DataBufferSize = (pDatasetManagementDW10->NR + 1) *
                      sizeof(NVM_DATASET_MANAGEMENT_RANGE);

    /*
     * Ensure the size of input buffer is big enough to accommodate the header
     * and image.
     */
    if (pNvmePtIoctl->DataBufferLen < DataBufferSize) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_IN_BUFFER;

        return IOCTL_COMPLETED;
    }

    /* Prepare the PRP entries for the transfer */
    if (NVMePreparePRPs(pDevExt,
                        (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb),
                        pNvmePtIoctl->DataBuffer,
                        DataBufferSize) == FALSE) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;

        return IOCTL_COMPLETED;
    }

    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlDataSetManagement */

/******************************************************************************
 * NVMeIoctlHotRemoveNamespace
 *
 * @brief This function calls StorPortNotification with BusChangeDetected to
 *        force a bus re-enumeration after removing a namespace which used to
 *        be seen by Windows system.
 *
 * @param pSrbExt - Pointer to Srb Extension allocated in SRB.
 *
 * @return None
 ******************************************************************************/
VOID NVMeIoctlHotRemoveNamespace (
    PNVME_SRB_EXTENSION pSrbExt
)
{
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
    PNVMe_COMMAND pNvmeCmd = NULL;
    ULONG NS = 0;
    ULONG lunId = 0;
    PNVME_LUN_EXTENSION pLunExt = NULL;

    pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);
    pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;

    /*
     * If we're in the middle of formatting NVM,
     * simply reject current request:
     * SrbStatus = SRB_STATUS_INVALID_REQUEST
     * ReturCode = NVME_IOCTL_UNSUPPORTED_OPERATION
     */
    if ((pDevExt->FormatNvmInfo.State != FORMAT_NVM_RECEIVED) &&
        (pDevExt->FormatNvmInfo.State != FORMAT_NVM_NO_ACTIVITY)) {
        pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);
        pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_UNSUPPORTED_OPERATION;
        pSrb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
        return;
    }

    /* Set the namespace(s) as unavailable now */
    if (pNvmeCmd->NSID == ALL_NAMESPACES_APPLIED) {
        /*
         * Set all supported namespaces as unavailable
         * by setting all lun slot statuses to OFFLINE
         */
        pDevExt->FormatNvmInfo.FormatAllNamespaces = TRUE;

        for (lunId = 0; lunId < MAX_NAMESPACES; lunId++) {
            pLunExt = pDevExt->pLunExtensionTable[lunId];
            if (ONLINE == pLunExt->slotStatus) {
                pLunExt->slotStatus = OFFLINE;
                pLunExt->offlineReason = FORMAT_IN_PROGRESS;
                /* Now we are in namespace hot remove state. */
                pDevExt->FormatNvmInfo.State = FORMAT_NVM_NS_REMOVED;
            }
        }
    } else if (TRUE == NVMeIsNamespaceVisible(pSrbExt, 0, &lunId)) {
        /* We just have to format one namespace; OFFLINE it if it's visible */
        if (INVALID_LUN_EXTN != lunId) {
            pLunExt = pDevExt->pLunExtensionTable[lunId];
            if (ONLINE == pLunExt->slotStatus) {
                pDevExt->FormatNvmInfo.TargetLun = lunId;
                pLunExt->slotStatus = OFFLINE;
                pLunExt->offlineReason = FORMAT_IN_PROGRESS;
                /* Now we are in namespace hot remove state. */
                pDevExt->FormatNvmInfo.State = FORMAT_NVM_NS_REMOVED;
            }
        }
    }

    if (FORMAT_NVM_NS_REMOVED == pDevExt->FormatNvmInfo.State) {
        /*
         * Force bus re-enumeration,
         * then, Windows won't see the target namespace(s)
         */
        StorPortNotification(BusChangeDetected, pDevExt);
    } else {
        /* We did not take any lun extension offline */
        pDevExt->FormatNvmInfo.TargetLun = INVALID_LUN_EXTN;
    }
} /* NVMeIoctlHotRemoveNamespace */

/******************************************************************************
 * NVMeIoctlHotAddNamespace
 *
 * @brief This function calls StorPortNotification with BusChangeDetected to
 *        force a bus re-enumeration to let Windows discover the newly added
 *        namespace.
 *
 *
 * @param pSrbExt - Pointer to Srb Extension allocated in SRB.
 *
 * @return None
 ******************************************************************************/
VOID NVMeIoctlHotAddNamespace (
    PNVME_SRB_EXTENSION pSrbExt
)
{
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
    ULONG lunId = 0;
    PNVME_LUN_EXTENSION pLunExt = NULL;

    /*
     * If haven't received NVME_PASS_THROUGH_SRB_IO_CODE(Format NVM) or
     * NVME_HOT_REMOVE_NAMESPACE request, reject current request:
     * SrbStatus = SRB_STATUS_SUCCESS
     * ReturCode = NVME_IOCTL_UNSUPPORTED_OPERATION
     */
    if (INVALID_LUN_EXTN == pDevExt->FormatNvmInfo.TargetLun) {
        pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);
        pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_UNSUPPORTED_OPERATION;
        pSrb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
        return;
    }
    /*
     * We previously OFFLINED one or more luns, we'll make
     * them ONLINE now.
     */
    if (TRUE == pDevExt->FormatNvmInfo.FormatAllNamespaces) {
        for (lunId = 0; lunId < MAX_NAMESPACES - 1; lunId++) {
            pLunExt = pDevExt->pLunExtensionTable[lunId];
            if ((OFFLINE == pLunExt->slotStatus) &&
                (FORMAT_IN_PROGRESS == pLunExt->offlineReason)) {
                pLunExt->slotStatus = ONLINE;
                pLunExt->offlineReason = NOT_OFFLINE;
            }
        }
    } else {
        lunId = pDevExt->FormatNvmInfo.TargetLun;
        if (INVALID_LUN_EXTN != lunId) {
            pLunExt = pDevExt->pLunExtensionTable[lunId];
            if ((OFFLINE == pLunExt->slotStatus) &&
                (FORMAT_IN_PROGRESS == pLunExt->offlineReason)) {
                pLunExt->slotStatus = ONLINE;
                pLunExt->offlineReason = NOT_OFFLINE;
            }
        }
    }

    /*
     * Force bus re-enumeration
     * and report the available namespace(s) via Inquiry commands
     */
    StorPortNotification(BusChangeDetected, pDevExt);
} /* NVMeIoctlHotAddNamespace */


/******************************************************************************
* NVMeFormatNVMHotRemoveNamespace
*
* @brief This function calls StorPortNotification with BusChangeDetected to
*        force a bus re-enumeration after removing a namespace which used to
*        be seen by Windows system. After removal, a Format NVM command is
*        applied to the removed namespace(s).
*
* @param pSrbExt - Pointer to Srb Extension allocated in SRB.
*
* @return None
******************************************************************************/
VOID NVMeFormatNVMHotRemoveNamespace(
    PNVME_SRB_EXTENSION pSrbExt
)
{
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
    PNVMe_COMMAND pNvmeCmd = NULL;
    ULONG NS = 0;
    ULONG lunId = INVALID_LUN_EXTN;
    PNVME_LUN_EXTENSION pLunExt = NULL;

    pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);
    pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;

    /*
     * If we're in the middle of formatting NVM,
     * simply reject current request:
     * SrbStatus = SRB_STATUS_INVALID_REQUEST
     * ReturCode = NVME_IOCTL_UNSUPPORTED_OPERATION
     */
    if ((pDevExt->FormatNvmInfo.State != FORMAT_NVM_RECEIVED) &&
        (pDevExt->FormatNvmInfo.State != FORMAT_NVM_NO_ACTIVITY)) {
        pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_UNSUPPORTED_OPERATION;
        pSrb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
        return;
    }

    StorPortDebugPrint(INFO, "NVMeFormatNVMHotRemoveNamespace: NSID:0x%x\n",
        pNvmeCmd->NSID);

    /* Set the namespace(s) as unavailable now */
    if (pNvmeCmd->NSID == ALL_NAMESPACES_APPLIED) {
        /*
         * Set all reported namespaces as unavailable
         * by setting all lun slot statuses to OFFLINE
         */
        pDevExt->FormatNvmInfo.FormatAllNamespaces = TRUE;
        for (lunId = 0; lunId < MAX_NAMESPACES; lunId++) {
            pLunExt = pDevExt->pLunExtensionTable[lunId];
            if (ONLINE == pLunExt->slotStatus && pLunExt->identifyData.NSZE != 0) {
                pLunExt->slotStatus = OFFLINE;
                pLunExt->offlineReason = FORMAT_IN_PROGRESS;
                /* Now we are in namespace hot remove state. */
                pDevExt->FormatNvmInfo.State = FORMAT_NVM_NS_REMOVED;
            }
        }
    } else if (TRUE == NVMeIsNamespaceVisible(pSrbExt, 0, &lunId)) {
       /*
        * Mark down the target namespace to format.
        */
       StorPortDebugPrint(INFO, 
           "NVMeFormatNVMHotRemoveNamespace: Target lunID=%d.\n", lunId);
       pDevExt->FormatNvmInfo.TargetLun = lunId;

       /* We just have to format one namespace; OFFLINE it if it's visible */
       if (INVALID_LUN_EXTN != lunId) {
           pLunExt = pDevExt->pLunExtensionTable[lunId];
           if (ONLINE == pLunExt->slotStatus && pLunExt->identifyData.NSZE != 0) {
               pDevExt->FormatNvmInfo.TargetLun = lunId;
               pLunExt->slotStatus = OFFLINE;
               pLunExt->offlineReason = FORMAT_IN_PROGRESS;
               /* Now we are in namespace hot remove state. */
               pDevExt->FormatNvmInfo.State = FORMAT_NVM_NS_REMOVED;
           }
        }
    }

    StorPortDebugPrint(INFO, "NVMeFormatNVMHotRemoveNamespace: Hold off IOs.\n");
    /*
     * Notify Storport the device is busy and stop sending more requests down
     * While re-enumerating bus.
     */
    if (StorPortBusy(pDevExt, ALL_NAMESPACES_APPLIED) != TRUE) {
        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_FORMAT_NVM_FAILED;
        return;
    }

    if (FORMAT_NVM_NS_REMOVED == pDevExt->FormatNvmInfo.State) {
        StorPortDebugPrint(INFO, 
            "NVMeFormatNVMHotRemoveNamespace: Start re-enumeration.\n");
        /*
         * Force bus re-enumeration,
         * then, Windows won't see the target namespace(s)
         */
        StorPortNotification(BusChangeDetected, pDevExt);
    } else {
        /* We did not take any lun extension offline */
        pDevExt->FormatNvmInfo.TargetLun = INVALID_LUN_EXTN;
    }
} /* NVMeFormatNVMHotRemoveNamespace */


/******************************************************************************
* NVMeFormatNVMHotAddNamespace
*
* @brief This function calls StorPortNotification with BusChangeDetected to
*        force a bus re-enumeration to let Windows discover the newly added
*        namespace after successful completion of a Format NVM command.
*
* @param pSrbExt - Pointer to Srb Extension allocated in SRB.
*
* @return None
******************************************************************************/
VOID NVMeFormatNVMHotAddNamespace(
    PNVME_SRB_EXTENSION pSrbExt
)
{
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
    ULONG lunId = 0;
    PNVME_LUN_EXTENSION pLunExt = NULL;

    /*
     * If haven't received NVME_PASS_THROUGH_SRB_IO_CODE(Format NVM) or
     * NVME_HOT_REMOVE_NAMESPACE request, reject current request:
     * SrbStatus = SRB_STATUS_SUCCESS
     * ReturCode = NVME_IOCTL_UNSUPPORTED_OPERATION
     */
    if (INVALID_LUN_EXTN == pDevExt->FormatNvmInfo.TargetLun) {
        pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);
        pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_UNSUPPORTED_OPERATION;
        pSrb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
        return;
    }

    StorPortDebugPrint(INFO, "NVMeFormatNVMHotAddNamespace: For all NS:0x%x\n",
        pDevExt->FormatNvmInfo.FormatAllNamespaces);

    /*
     * We previously OFFLINED one or more luns, we'll make
     * them ONLINE now.
     */
    if (TRUE == pDevExt->FormatNvmInfo.FormatAllNamespaces) {
        for (lunId = 0; lunId < MAX_NAMESPACES; lunId++) {
            pLunExt = pDevExt->pLunExtensionTable[lunId];
            if ((OFFLINE == pLunExt->slotStatus) &&
                (FORMAT_IN_PROGRESS == pLunExt->offlineReason)) {
                pLunExt->slotStatus = ONLINE;
                pLunExt->offlineReason = NOT_OFFLINE;
            }
        }
    } else {
        lunId = pDevExt->FormatNvmInfo.TargetLun;
        if (INVALID_LUN_EXTN != lunId) {
            pLunExt = pDevExt->pLunExtensionTable[lunId];
            if ((OFFLINE == pLunExt->slotStatus) &&
                (FORMAT_IN_PROGRESS == pLunExt->offlineReason)) {
                pLunExt->slotStatus = ONLINE;
                pLunExt->offlineReason = NOT_OFFLINE;
            }
        }
    }

    /*
     * Reset FORMAT_NVM_INFO structure to zero
     * since the request is completed
     */
    memset((PVOID)(&pDevExt->FormatNvmInfo), 0, sizeof(FORMAT_NVM_INFO));

    /*
     * Force bus re-enumeration
     * and report the available namespace(s) via Inquiry commands
     */
    NVMeStallExecution(pDevExt, MAX_STATE_STALL_us);
    StorPortNotification(BusChangeDetected, pDevExt);
} /* NVMeFormatNVMHotAddNamespace */

/******************************************************************************
 * NVMeIoctlNamespaceAttachment
 *
 * @brief NVMeIoctlNamespaceAttachment handles the NAMESPACE ATTACHMENT Command
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 *****************************************************************************/
BOOLEAN NVMeIoctlNamespaceAttachment(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
	PSTORAGE_REQUEST_BLOCK pSrb,
#else
	PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PNVME_SRB_EXTENSION pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    PNVMe_COMMAND pNvmeCmd = NULL;
    PADMIN_NAMESPACE_ATTACHMENT_DW10 pNsAttachDW10 = NULL;
    PNVMe_CONTROLLER_LIST pNsCtrlsList = NULL;
    ULONG lunId = INVALID_LUN_EXTN;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    ULONG currentNSID = 0;


    pNsAttachDW10 =
        (PADMIN_NAMESPACE_ATTACHMENT_DW10)&pNvmePtIoctl->NVMeCmd[10];
    pNsCtrlsList = (PNVMe_CONTROLLER_LIST)&pNvmePtIoctl->DataBuffer;

    pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd; 
    currentNSID = pNvmeCmd->NSID;
    if (INVALID != NVMeGetNamespaceStatusAndSlot(pDevExt, currentNSID, &lunId)) {
        StorPortDebugPrint(INFO, 
            "NVMeIoctlNamespaceAttachment: Target lunID=%d.\n", lunId);
    
        /*
         * Notify visbile LUN change when attach/detach 
         * controller list having current controller
         */ 
        if(pNsCtrlsList->Identifers[0] == 
            pDevExt->controllerIdentifyData.CNTLID) {
            pLunExt = pDevExt->pLunExtensionTable[lunId];
            if (pNsAttachDW10->SEL == NAMESPACE_DETACH) {
                if (FREE != pLunExt->slotStatus) {
                    if (ONLINE == pLunExt->slotStatus) {
                        pLunExt->slotStatus = OFFLINE;
                        pLunExt->offlineReason = DETACH_IN_PROGRESS;
                    } else if (pLunExt->offlineReason == DETACH_IN_PROGRESS) {
                        /* Wait for pending detach/attach to complete */
                        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                        pNvmePtIoctl->SrbIoCtrl.ReturnCode =
                            NVME_IOCTL_ATTACH_NAMESPACE_FAILED;
                        return IOCTL_COMPLETED;
                    }

                    /*
                     * Notify Storport the device is busy and stop sending 
                     * more requests down.
                     */
                    if (StorPortDeviceBusy(pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
						SrbGetPathId((void*) pSrb),
						SrbGetTargetId((void*) pSrb),
						(UCHAR)lunId,
#else
						pSrb->PathId,
						pSrb->TargetId,
						(UCHAR)lunId,
#endif
					    ALL_NAMESPACES_APPLIED) != TRUE) {
                        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                        pNvmePtIoctl->SrbIoCtrl.ReturnCode =
                            NVME_IOCTL_ATTACH_NAMESPACE_FAILED;
                        return IOCTL_COMPLETED;
                    }

                    /*
                     * Force bus re-enumeration,
                     * then, Windows won't see the target namespace(s)
                     */
                    StorPortNotification(BusChangeDetected, pDevExt);
                }
            }
        }
    } else {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_ATTACH_NAMESPACE_FAILED;
        return IOCTL_COMPLETED;

    }

    /* Prepare the PRP entries for the transfer for create command */
    if (NVMePreparePRPs(pDevExt,
                        pSrbExt,
                        pNvmePtIoctl->DataBuffer,
                        pNvmePtIoctl->DataBufferLen) == FALSE) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;
        return IOCTL_COMPLETED;
    }
    
    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlNamespaceAttachment */

/*******************************************************************************
 * NVMeIoctlNamespaceMgmt
 *
 * @brief NVMeIoctlNamespaceMgmt handles the NAMESPACE MANAGEMENT Command
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeIoctlNamespaceMgmt(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
	PSTORAGE_REQUEST_BLOCK pSrb,
#else
	PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
)
{
    PNVME_SRB_EXTENSION pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PADMIN_NAMESPACE_MANAGEMENT_DW10 pNsMgmtDW10 = NULL;
    PADMIN_NAMESPACE_ATTACHMENT_DW10 pNsAttachDW10 = NULL;
    ULONG lunId = INVALID_LUN_EXTN;
    NS_STATUS nsStatus = INVALID;
    PNVMe_COMMAND pNvmeCmd = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    ULONG currentNSID = 0;
    PNVMe_CONTROLLER_LIST pNsCtrlsList = NULL;


    pNsMgmtDW10 =
        (PADMIN_NAMESPACE_MANAGEMENT_DW10)&pNvmePtIoctl->NVMeCmd[10];
    
    if (pNsMgmtDW10->SEL == NAMESPACE_MANAGEMENT_CREATE && 
        pNvmePtIoctl->DataBufferLen < sizeof(ADMIN_IDENTIFY_NAMESPACE)) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_IN_BUFFER;
        return IOCTL_COMPLETED;
    }
    
    pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd; 

    if (pNsMgmtDW10->SEL == NAMESPACE_MANAGEMENT_DELETE) {
		currentNSID = pNvmeCmd->NSID;
		nsStatus = NVMeGetNamespaceStatusAndSlot(pDevExt, currentNSID, &lunId);
		StorPortDebugPrint(INFO,
			"NVMeIoctlNamespaceMgmt: Target lunID=%d.\n", lunId);

		/* If namespace is attached, detach before deleting */
        if (ATTACHED == nsStatus) {
            pLunExt = pDevExt->pLunExtensionTable[lunId];
            if (ONLINE == pLunExt->slotStatus) {
                pLunExt->slotStatus = OFFLINE;
                pLunExt->offlineReason = DELETE_IN_PROGRESS;
                pNvmeCmd->CDW0.OPC = ADMIN_NAMESPACE_ATTACHMENT;
                pNsAttachDW10 = 
                    (PADMIN_NAMESPACE_ATTACHMENT_DW10)&pNvmePtIoctl->NVMeCmd[10];
                pNsAttachDW10->SEL = NAMESPACE_DETACH;
				pSrbExt->pDataBuffer = 
					NVMeAllocatePool(pDevExt, sizeof(NVMe_CONTROLLER_LIST));
				memset(pSrbExt->pDataBuffer, 0, sizeof(NVMe_CONTROLLER_LIST));
                pNsCtrlsList = (PNVMe_CONTROLLER_LIST)pSrbExt->pDataBuffer;
                pNsCtrlsList->NumberOfIdentifiers = 1;
                pNsCtrlsList->Identifers[0] = 
                    pDevExt->controllerIdentifyData.CNTLID;
                return NVMeIoctlNamespaceAttachment(pDevExt, pSrb, pNvmePtIoctl);
            }
        }
    }

    /* Prepare the PRP entries for the transfer for create command */
    if (pNsMgmtDW10->SEL == NAMESPACE_MANAGEMENT_CREATE &&
        NVMePreparePRPs(pDevExt,
                        pSrbExt,
                        pNvmePtIoctl->DataBuffer,
                        pNvmePtIoctl->DataBufferLen) == FALSE) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;
        return IOCTL_COMPLETED;
    }
    
    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlNamespaceMgmt */

/******************************************************************************
 * NVMeCompletionNsAttachment
 *
 * @brief NVMeCompletionNsAttachment handles the NAMESPACE ATTACHMENT Command
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrbExt - Pointer to SRB extension
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 *****************************************************************************/
BOOLEAN NVMeCompletionNsAttachment(
    PVOID pNVMeDevExt,
    PNVME_SRB_EXTENSION pSrbExt
)
{
    BOOLEAN returnStatus = TRUE;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = (PNVME_DEVICE_EXTENSION) pNVMeDevExt;
    ULONG lunId = INVALID_LUN_EXTN;
    PNVMe_COMMAND pNvmeCmd = NULL;
    PNVMe_COMMAND pNsMgmtCmd = NULL;
    PADMIN_NAMESPACE_ATTACHMENT_DW10 pNsAttachDW10 = NULL;
    PADMIN_NAMESPACE_MANAGEMENT_DW10 pNsMgmtDW10 = NULL;

    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
    PNVMe_COMMAND pIdentifyCmd = NULL;
    NS_STATUS nsStatus = INVALID;
#if (NTDDI_VERSION > NTDDI_WIN7)
	PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
	PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    pSrb->SrbStatus = SRB_STATUS_SUCCESS;

    pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);
    pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;       
    pNsAttachDW10 =
        (PADMIN_NAMESPACE_ATTACHMENT_DW10)&pNvmePtIoctl->NVMeCmd[10];
    nsStatus = NVMeGetNamespaceStatusAndSlot(pDevExt, pNvmeCmd->NSID, &lunId);

    if ((INVALID == nsStatus) || (INVALID_LUN_EXTN == lunId)) {
        pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_INVALID_PATH_TARGET_ID;
        return TRUE;
    } else {
        pLunExt = pDevExt->pLunExtensionTable[lunId];
    }

    if ((pSrbExt->pCplEntry->DW3.SF.SC == SUCCESSFUL_COMPLETION) &&
        (pSrbExt->pCplEntry->DW3.SF.SCT == GENERIC_COMMAND_STATUS)) {

        if (pSrbExt->nvmeSqeUnit.CDW0.OPC == ADMIN_NAMESPACE_ATTACHMENT) {
            if (pNsAttachDW10->SEL == NAMESPACE_ATTACH) {
                if (INACTIVE == nsStatus) {
                    //Attach (part 1): first get latest identify data for the NSID
                    pIdentifyCmd = &pSrbExt->nvmeSqeUnit;
                    pIdentifyCmd->CDW0.OPC = ADMIN_IDENTIFY;
                    pIdentifyCmd->NSID = pNvmeCmd->NSID;
                    if (NVMePreparePRPs(pDevExt, pSrbExt,
                                        (PVOID)&pLunExt->identifyData,
                                        sizeof(ADMIN_IDENTIFY_NAMESPACE)) == TRUE) {

                        if (TRUE == ProcessIo(pDevExt, pSrbExt, 
                            NVME_QUEUE_TYPE_ADMIN, TRUE)) {
                                returnStatus = FALSE;
                        }
                    }
                } else {
                    //According to our record, this namespace was already attached!!
                    pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_INTERNAL_ERROR;
                }
            } else {
                if (ATTACHED == nsStatus) {
                    //Detach: Just set the correct status, and call StorPort 
                    pLunExt->nsStatus = INACTIVE;
                    pLunExt->slotStatus = FREE;
                    pLunExt->ReadOnly = TRUE;
                    if (pLunExt->offlineReason != DELETE_IN_PROGRESS)
                        pLunExt->offlineReason = NOT_OFFLINE;

                    pDevExt->visibleLuns--;
#if (NTDDI_VERSION > NTDDI_WIN7)
					StorPortDeviceReady(pDevExt, 
						SrbGetPathId((void*) pSrb),
						SrbGetTargetId((void*) pSrb),
						(UCHAR) lunId);
#else
					StorPortDeviceReady(pDevExt, pSrb->PathId, pSrb->TargetId, (UCHAR) lunId);

#endif
                    //If this detach was part of delete in progress, now send delete cmd.
                    if (pLunExt->offlineReason == DELETE_IN_PROGRESS) {
                        pNsMgmtCmd = &pSrbExt->nvmeSqeUnit;
                        pNsMgmtCmd->CDW0.OPC = ADMIN_NAMESPACE_MANAGEMENT;
                        pNsMgmtCmd->NSID = pNvmeCmd->NSID;
                        pNvmeCmd->CDW0.OPC = ADMIN_NAMESPACE_MANAGEMENT;
                        StorPortFreePool(pNVMeDevExt, pSrbExt->pDataBuffer);
                        pNsMgmtDW10 = (PADMIN_NAMESPACE_MANAGEMENT_DW10)&pNvmePtIoctl->NVMeCmd[10];
                        pNsMgmtDW10->SEL = NAMESPACE_MANAGEMENT_DELETE;
                        if (IOCTL_PENDING == NVMeIoctlNamespaceMgmt(pDevExt, pSrb, pNvmePtIoctl)) {
                            if (TRUE == ProcessIo(pDevExt, pSrbExt, 
                                NVME_QUEUE_TYPE_ADMIN, TRUE)) {
                                    returnStatus = FALSE;
                            }
                        }
                    }
                    pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_SUCCESS;
                } else {
                    //According to our record, this namespace was already detached!!
                    pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_INTERNAL_ERROR;
                }
             }
        } else if (pSrbExt->nvmeSqeUnit.CDW0.OPC == ADMIN_IDENTIFY) {
            //Attach (part 2): Received identify data, now complete attachment
            pLunExt->nsStatus = ATTACHED;
            pLunExt->slotStatus = ONLINE;
            pLunExt->ReadOnly = FALSE;

            pDevExt->visibleLuns++;
            StorPortNotification(BusChangeDetected, pDevExt);
            pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_SUCCESS;
        }
    } else {
        if ((pLunExt->slotStatus == OFFLINE) &&
            ((pLunExt->offlineReason == DELETE_IN_PROGRESS) ||
            (pLunExt->offlineReason == DETACH_IN_PROGRESS))) {

			//Free memory allocated during delete/detach path 
			if (pLunExt->offlineReason == DELETE_IN_PROGRESS) {
				StorPortFreePool(pNVMeDevExt, pSrbExt->pDataBuffer);
			}

#if (NTDDI_VERSION > NTDDI_WIN7)
			StorPortDeviceReady(pDevExt,
				SrbGetPathId((void*) pSrb),
				SrbGetTargetId((void*) pSrb),
				(UCHAR) lunId);
#else
			StorPortDeviceReady(pDevExt, pSrb->PathId, pSrb->TargetId, (UCHAR) lunId);

#endif

            //We were online and attached when we started. Restore status back.
            pLunExt->slotStatus = ONLINE;
            pLunExt->offlineReason = NOT_OFFLINE;
            StorPortNotification(BusChangeDetected, pDevExt);
        }
        StorPortDebugPrint(INFO,
            "NVMeCompletionNsAttachment: Command Failed!\n");
        pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_INTERNAL_ERROR;
        returnStatus = TRUE;		
    }
    return returnStatus;
} /* NVMeCompletionNsAttachment */

/*******************************************************************************
 * NVMeCompletionNsMgmt
 *
 * @brief NVMeCompletionNsMgmt handles the NAMESPACE MANAGEMENT Command
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrbExt - Pointer to SRB extension
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeCompletionNsMgmt(
    PVOID pNVMeDevExt,
    PNVME_SRB_EXTENSION pSrbExt
)
{
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = (PNVME_DEVICE_EXTENSION) pNVMeDevExt;
    ULONG lunId = INVALID_LUN_EXTN;
    PNVMe_COMMAND pNvmeCmd = NULL;
    PADMIN_NAMESPACE_MANAGEMENT_DW10 pNsMgmtDW10 = NULL;
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
    ULONG currentNSID = 0;
    BOOLEAN notifyStorPort = FALSE;

#if (NTDDI_VERSION > NTDDI_WIN7)
	PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
	PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    pSrb->SrbStatus = SRB_STATUS_SUCCESS;

    pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);
    pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;       
    pNsMgmtDW10 =
        (PADMIN_NAMESPACE_MANAGEMENT_DW10)&pNvmePtIoctl->NVMeCmd[10];
    
    if ((pSrbExt->pCplEntry->DW3.SF.SC == SUCCESSFUL_COMPLETION) &&
        (pSrbExt->pCplEntry->DW3.SF.SCT == GENERIC_COMMAND_STATUS)) {

        if (pNsMgmtDW10->SEL == NAMESPACE_MANAGEMENT_CREATE) {
            currentNSID = pSrbExt->pCplEntry->DW0;
            if (INVALID == NVMeGetNamespaceStatusAndSlot(pDevExt, currentNSID, &lunId)) {
                lunId = NVMeGetFreeLunSlot(pDevExt);
                if (INVALID_LUN_EXTN == lunId) {
                    pNvmePtIoctl->SrbIoCtrl.ReturnCode = 
                        NVME_IOCTL_INVALID_PATH_TARGET_ID;
                    return TRUE;
                }
				pDevExt->DriverState.NumKnownNamespaces++;
                pLunExt = pDevExt->pLunExtensionTable[lunId];
                pLunExt->namespaceId = currentNSID;
                pLunExt->nsStatus = INACTIVE;
                pLunExt->slotStatus = FREE;
                pLunExt->offlineReason = NOT_OFFLINE;
                //memcpy_s(&pLunExt->identifyData,
                //        sizeof(pLunExt->identifyData),
                //        pNvmePtIoctl->DataBuffer,
                //        sizeof(ADMIN_IDENTIFY_NAMESPACE));
                RtlCopyMemory(&pLunExt->identifyData,pNvmePtIoctl->DataBuffer,min(sizeof(pLunExt->identifyData),sizeof(ADMIN_IDENTIFY_NAMESPACE)));

                pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_SUCCESS;
            } else {
                //Namespace was already visible. Shouldn't have happened
                pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_INTERNAL_ERROR;
            }
        } else {
            if (INVALID != NVMeGetNamespaceStatusAndSlot(pDevExt, pNvmeCmd->NSID, &lunId)) {
                pLunExt = pDevExt->pLunExtensionTable[lunId];
                if (FREE != pLunExt->slotStatus)
                    pDevExt->visibleLuns--;
                memset(pLunExt, 0, sizeof(NVME_LUN_EXTENSION));
                pDevExt->DriverState.NumKnownNamespaces--;
                pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_SUCCESS;
            } else {
                //Namespace is not visible. Should have been if we are about to delete it...
                pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_INTERNAL_ERROR;
            }
         }
    } else {
        // Command failed
        pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_INTERNAL_ERROR;
    }
    
    return TRUE;
} /* NVMeCompletionNsMgmt */

/******************************************************************************
 * FormatNVMGetIdentify
 *
 * @brief This helper function re-uses the submission entry in Srb Extension
 *        to issue Identify commands by calling ProcessIo.
 *
 *
 * @param pSrbExt - Pointer to Srb Extension allocated in SRB.
 * @param NamespaceID - Specifies which structure to retrieve.
 *
 * @return TRUE - Indicates the request should be completed due to errors.
 *         FALSE - Indicates more processing required and no error detected.
 ******************************************************************************/
BOOLEAN FormatNVMGetIdentify(
    PNVME_SRB_EXTENSION pSrbExt,
    ULONG NamespaceID
)
{
    PNVME_DEVICE_EXTENSION pAE = pSrbExt->pNvmeDevExt;
    PNVMe_COMMAND pIdentify = NULL;
    PADMIN_IDENTIFY_CONTROLLER pIdenCtrl = &pAE->controllerIdentifyData;
    PADMIN_IDENTIFY_NAMESPACE pIdenNS = NULL;
    PADMIN_IDENTIFY_COMMAND_DW10 pIdentifyCDW10 = NULL;
    BOOLEAN Status = TRUE;
    ULONG lunId = INVALID_LUN_EXTN;

    /* Populate submission entry fields */
    pIdentify = &pSrbExt->nvmeSqeUnit;
    pIdentify->CDW0.OPC = ADMIN_IDENTIFY;

    if (NamespaceID == IDEN_CONTROLLER) {
        /* Indicate it's for Controller structure */
        pIdentifyCDW10 = (PADMIN_IDENTIFY_COMMAND_DW10) &pIdentify->CDW10;
        pIdentifyCDW10->CNS = 1;
        /* Prepare PRP entries, need at least one PRP entry */
        if (NVMePreparePRPs(pAE, 
                            pSrbExt,
                            (PVOID)pIdenCtrl,
                            sizeof(ADMIN_IDENTIFY_CONTROLLER)) == FALSE)
            Status = FALSE;
    } else {
        if (pIdenCtrl == NULL) {
            Status = FALSE;
        } else {
			/* Assign the destination buffer for retrieved structure */
			if (FALSE == NVMeIsNamespaceVisible(pSrbExt, NamespaceID, &lunId)) {
				/*
				* Not a visible namespace, so we can skip fetching identify
				* namespace structure. Return TRUE so we can complete the
				* request.
				*/
				Status = TRUE;
			}
			else {
				ASSERT(INVALID_LUN_EXTN != lunId);
				/* Assign the destination buffer for retrieved structure */
				pIdenNS = &pAE->pLunExtensionTable[lunId]->identifyData;
				/* Namespace ID is 1-based. */
				pIdentify->NSID = NamespaceID;
				/* Prepare PRP entries, need at least one PRP entry */
				if (NVMePreparePRPs(pAE,
									pSrbExt,
									(PVOID)pIdenNS,
									sizeof(ADMIN_IDENTIFY_NAMESPACE)) == FALSE)
				Status = FALSE;
			}
        }
    }
    /* Complete the request now if something goes wrong */
    if (Status == FALSE) {
        pSrbExt->pSrb->SrbStatus = SRB_STATUS_ERROR;
        return Status;
    }
    /* Now issue the command via Admin Doorbell register */
    return ProcessIo(pAE, pSrbExt, NVME_QUEUE_TYPE_ADMIN, TRUE);
} /* FormatNVMGetIdentify */


/******************************************************************************
 * FormatNVMFailure
 *
 * @brief This helper function handles the failure of completions and
 *        preparations Format NVM and Identify commands in
 *        NVMeIoctlFormatNVMCallback:
 *        Depending on the value of AddNamespaceNeeded:
 *        If TRUE, call NVMeFormatNVMHotAddNamespace to force bus re-enumeration
 *        and add back the formatted namespace(s).
 *        If FALSE, simply complete the request. User applications need to
 *        issue a NVME_HOT_ADD_NAMESPACE request to add back namespace(s).
 *
 * @param pNVMeDevExt - Pointer to hardware device extension.
 * @param pSrbExten - Pointer to Srb Extension allocated in SRB.
 *
 * @return None.
 *
 ******************************************************************************/
BOOLEAN FormatNVMFailure(
    PNVME_DEVICE_EXTENSION pDevExt,
    PNVME_SRB_EXTENSION pSrbExt
)
{
    PFORMAT_NVM_INFO pFormatNvmInfo = &pDevExt->FormatNvmInfo;

    StorPortDebugPrint(INFO, "FormatNVMFailure: Called!\n");

    /*
     * If AddNamespaceNeeded is TRUE, add back the namespace(s) via 
     * NVMeFormatNVMHotAddNamespace. Then clear the FORMAT_NVM_INFO structure and
     * return TRUE in order to complete the request. Since we hit an error
     * we need to finish it.
     */

    if (pFormatNvmInfo->AddNamespaceNeeded == TRUE) {
        /* Need to add back namespace(s) first */
        NVMeFormatNVMHotAddNamespace(pSrbExt);
    }
    /*
     * Reset FORMAT_NVM_INFO structure to zero
     * since the request is completed
     */
    memset((PVOID)pFormatNvmInfo, 0, sizeof(FORMAT_NVM_INFO));
    return TRUE;
 

}

/******************************************************************************
 * NVMeIoctlFormatNVMCallback
 *
 * @brief This function handles the Format NVM State Machine when commands are
 *        completed and ISR is called. It also re-use the Srb Extension to
 *        issue Identify commands to re-fetch Controller and specific structures
 *        after Format NVM command had been completed successfully.
 *
 * @param pNVMeDevExt - Pointer to hardware device extension.
 * @param pSrbExtension - Pointer to Srb Extension allocated in SRB.
 *
 * @return TRUE - Indicates the request should be completed, either due to
 *         errors, or after the format sequence completes succesfully.
 *         FALSE - Indicates more processing required and no error detected.
 ******************************************************************************/
BOOLEAN NVMeIoctlFormatNVMCallback(
    PVOID pNVMeDevExt,
    PVOID pSrbExtension
)
{
    PNVME_DEVICE_EXTENSION pDevExt = (PNVME_DEVICE_EXTENSION)pNVMeDevExt;
    PNVME_SRB_EXTENSION pSrbExt = (PNVME_SRB_EXTENSION)pSrbExtension;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
    PFORMAT_NVM_INFO pFormatNvmInfo = &pDevExt->FormatNvmInfo;
    PNVMe_COMMAND pNvmeCmd = NULL;
    ULONG NS = 0;
    ULONG lunId = 0;
    PNVME_LUN_EXTENSION pLunExt = NULL;

    pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);
    pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;
    NS = pNvmeCmd->NSID;

    StorPortDebugPrint(INFO, "NVMeIoctlFormatNVMCallback: State=0x%x\n",
        pFormatNvmInfo->State);

    switch (pFormatNvmInfo->State) {
        case FORMAT_NVM_CMD_ISSUED:
            /*
             * If it's not completed successfully,
             * complete it and notify Storport
             */
            if ((pSrbExt->pCplEntry->DW3.SF.SC != 0) ||
                (pSrbExt->pCplEntry->DW3.SF.SCT != 0)) {
                StorPortDebugPrint(ERROR,
                    "NVMeIoctlFormatNVMCallback: Format NVM command failed.\n");

                /*
                 * Copy the completion entry to
                 * NVME_PASS_THROUGH_IOCTL structure
                 */
                StorPortCopyMemory((PVOID)pNvmePtIoctl->CplEntry,
                                   (PVOID)pSrbExt->pCplEntry,
                                   sizeof(NVMe_COMPLETION_QUEUE_ENTRY));
                /*
                 * Format NVM fails, need to complete the request now.
                 */
                return FormatNVMFailure(pDevExt, pSrbExt);
            } else {
                /* Re-use the SrbExt to fetch Identify Controller structure */
                memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
                if (FormatNVMGetIdentify(pSrbExt, IDEN_CONTROLLER) == FALSE) {
                    /*
                     * Can't issue Identify command, complete the request now
                     */
                    return FormatNVMFailure(pDevExt, pSrbExt);
                }
                /* Now, Identify/Controller command is issued successfully. */
                pFormatNvmInfo->State = FORMAT_NVM_IDEN_CONTROLLER_ISSUED;
                StorPortDebugPrint(INFO,
                    "NVMeIoctlFormatNVMCallback: Fetching controller data.\n");

                return FALSE;
            }
        break;

        case FORMAT_NVM_IDEN_CONTROLLER_ISSUED:
            /**
             * If it's not completed successfully,
             * complete it and notify Storport
             */
            if ((pSrbExt->pCplEntry->DW3.SF.SC != 0) ||
                (pSrbExt->pCplEntry->DW3.SF.SCT != 0)) {

                StorPortDebugPrint(ERROR,
                    "NVMeIoctlFormatNVMCallback: Identify command failed.\n");

                /*
                 * Copy the completion entry to
                 * NVME_PASS_THROUGH_IOCTL structure
                 */
                StorPortCopyMemory((PVOID)pNvmePtIoctl->CplEntry,
                                   (PVOID)pSrbExt->pCplEntry,
                                   sizeof(NVMe_COMPLETION_QUEUE_ENTRY));
                /*
                 * Identify command fails, need to complete the request now.
                 */
                return FormatNVMFailure(pDevExt, pSrbExt);
            } else {
                /*
                 * Identify Controller structure is ready now.
                 * Re-use the SrbExt to fetch Identify Namespace structure(s)
                 * one at a time.
                 * We need to fetch the identify_namespace structure only
                 * if the NS we just formatted was a visible NS. For the
                 * hidden ones, we do not store the identify structure in
                 * the lun extension table, so there's no stale data to update.
                 */
                memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
                if (TRUE == pFormatNvmInfo->FormatAllNamespaces) {
                    /*
                     * Case 1: We formatted all namespaces, so fetch identify
                     * for all existing visible namespaces one at a time.
                     */

                    lunId = pFormatNvmInfo->NextLun;

                    while (lunId < MAX_NAMESPACES) {
                        pLunExt = pDevExt->pLunExtensionTable[lunId];
                        if ((OFFLINE == pLunExt->slotStatus) &&
                            (FORMAT_IN_PROGRESS == pLunExt->offlineReason)) {
                            /*
                             * Fetch identify namespace structure for the NS
                             * associated with this lun extension
                             */
                            pFormatNvmInfo->NextNs = pLunExt->namespaceId;
                            if (FormatNVMGetIdentify(pSrbExt,
                                pFormatNvmInfo->NextNs) == FALSE) {
                                /*
                                 * Can't issue Identify command, complete request now
                                 */
                                return FormatNVMFailure(pDevExt, pSrbExt);
                            }

                            pFormatNvmInfo->State = FORMAT_NVM_IDEN_CONTROLLER_ISSUED;
                            StorPortDebugPrint(INFO,
                                "NVMeIoctlFormatNVMCallback: Fetching NS(ID=%d) data.\n",
                                pFormatNvmInfo->NextNs);
                            pFormatNvmInfo->NextLun++;
                            return FALSE;
                        } else {
                            pFormatNvmInfo->NextLun++;
                            lunId = pFormatNvmInfo->NextLun;
                        }
                    } /* while looping thru LUN Ext */
                    /*
                     * We've fetched all identify_namespace structures for
                     * all namespaces; Complete the request now.
                     */
                    StorPortDebugPrint(INFO,
                        "NVMeIoctlFormatNVMCallback: All IDEN data fetched!\n");
                    pFormatNvmInfo->State = FORMAT_NVM_IDEN_NAMESPACE_FETCHED;
                    if (TRUE == pFormatNvmInfo->AddNamespaceNeeded) {
                        NVMeFormatNVMHotAddNamespace(pSrbExt);
                    }
                    pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                    pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_SUCCESS;
                    return TRUE;

                } else if (INVALID_LUN_EXTN != pFormatNvmInfo->TargetLun) {
                    /*
                     * Case 2: We formatted only one namespace, and that is a
                     * visible namespace, so we have to fetch identify.
                     */
                    pFormatNvmInfo->NextNs = NS;
                    if (FormatNVMGetIdentify(pSrbExt,
                                             pFormatNvmInfo->NextNs) == FALSE) {
                        /*
                         * Can't issue Identify command, complete request now
                         */
                        return FormatNVMFailure(pDevExt, pSrbExt);
                    }
                    /*
                     * All done here; move on to the next state
                     */
                    pFormatNvmInfo->State = FORMAT_NVM_IDEN_NAMESPACE_FETCHED;
                    StorPortDebugPrint(INFO,
                        "NVMeIoctlFormatNVMCallback: Fetching NS(ID=%d) data.\n",
                        pFormatNvmInfo->NextNs);
                    return FALSE;

                } else {
                    /*
                     * Case 3: We formatted one or more hidden namespaces;
                     * nothing more to do here.
                     */
                    pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                    pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_SUCCESS;
                    /*
                     * Reset FORMAT_NVM_INFO structure to zero
                     * since the request is completed
                     */
                    memset((PVOID)pFormatNvmInfo, 0, sizeof(FORMAT_NVM_INFO));
                    StorPortDebugPrint(INFO,
                        "NVMeIoctlFormatNVMCallback: Format hidden NS.\n");

                    return TRUE;
                }
                return FALSE;
            }
        break;

        case FORMAT_NVM_IDEN_NAMESPACE_FETCHED:
            StorPortDebugPrint(INFO,
                "NVMeIoctlFormatNVMCallback: All IDEN data fetched!\n");
            if (TRUE == pFormatNvmInfo->AddNamespaceNeeded) {
                NVMeFormatNVMHotAddNamespace(pSrbExt);
            }
            pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            pNvmePtIoctl->SrbIoCtrl.ReturnCode = NVME_IOCTL_SUCCESS;

            return TRUE;
        break;

        default:
        break;
    } /* state switch */
    /* Don't complete the request back to Storport just yet */
    return FALSE;
} /* NVMeIoctlFormatNVMCallback */

/******************************************************************************
 * NVMeIoctlFormatNVM
 *
 * @brief This function is the entry point handling Format NVM command:
 *        1. It does some checkings and initial settings
 *        2. Calls NVMeFormatNVMHotRemoveNamespace if no NVME_HOT_REMOVE_NAMESPACE
 *           previously received.
 *        3. NVMeStartIo will issue Format NVM command later.
 *
 * @param pDevext - Pointer to hardware device extension.
 * @param pSrb - This parameter specifies the SCSI I/O request.
 * @param pNvmePtIoctl - Pointer to NVME_PASS_THROUGH_IOCTL Structure
 *
 * @return IOCTL_COMPLETED - Indicates the request should be completed due to
 *                           certain errors.
 *         IOCTL_PEDNING - Indicates more processing required.
 ******************************************************************************/
BOOLEAN NVMeIoctlFormatNVM (
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PNVMe_COMMAND pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;
    ULONG IoctlHdrSize = sizeof(NVME_PASS_THROUGH_IOCTL);
    PNVME_SRB_EXTENSION pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    ULONG NS = 0;

    /*
     * Ensure the Namespace ID is valid:
     * It's 1-based.
     * It can be 0xFFFFFFFF for all namespaces
     */
    if ((pNvmeCmd->NSID == 0) ||
        ((pNvmeCmd->NSID != ALL_NAMESPACES_APPLIED) &&
         (pNvmeCmd->NSID > pDevExt->controllerIdentifyData.NN))) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INVALID_NAMESPACE_ID;
        return IOCTL_COMPLETED;
    }

    /*
     * When no namespace had been removed yet,
     * call NVMeFormatNVMHotRemoveNamespace to remove the namespace(s) first
     */
    if (pDevExt->FormatNvmInfo.State == FORMAT_NVM_RECEIVED) {
        NVMeFormatNVMHotRemoveNamespace(pSrbExt);
    } else {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_FORMAT_NVM_PENDING;
        return IOCTL_COMPLETED;
    }

    return IOCTL_PENDING;
} /* NVMeIoctlFormatNVM */


/******************************************************************************
 * NVMeIsNamespaceVisible
 *
 * @brief This function checks if a given NSID is a visible or a hidden NS.
 *
 * @param pSrbExtension - Pointer to Srb Extension allocated in SRB.
 *
 * @return TRUE - Indicates that the namespace is visible/exposed.
 *         FALSE - Indicates that the namespace is hidden.
 ******************************************************************************/
BOOLEAN NVMeIsNamespaceVisible(
    PNVME_SRB_EXTENSION pSrbExt,
    ULONG targetNSID,
    PULONG pLunId
)
{
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = NULL;
#else
    PSCSI_REQUEST_BLOCK pSrb = NULL;
#endif
    
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
    PNVMe_COMMAND pNvmeCmd = NULL;
    ULONG NSID = 0;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    BOOLEAN bNamespaceIsVisible = FALSE;
    ULONG lunId = 0;
    ULONG lunIdToBeReturned = INVALID_LUN_EXTN;

    pSrb = pSrbExt->pSrb;
    pDevExt = pSrbExt->pNvmeDevExt;
    pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)GET_DATA_BUFFER(pSrb);
    pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;

    /*
     * Need to identify the association between NSID and Lun extension
     * If calling function specifies a valid NSID, use it. Otherwise, use
     * the one specified in command entry.
     */
    if (targetNSID != 0)
        NSID = targetNSID;
    else
        NSID = pNvmeCmd->NSID;

    for (lunId = 0; lunId < MAX_NAMESPACES; lunId++) {
        pLunExt = pDevExt->pLunExtensionTable[lunId];
        if (NSID == pLunExt->namespaceId) {
            /*
             * We found the lun extension of the visible namespace
             * corresponding to NSID; return the lun id. We don't care
             * if the lun status is online or offline; all we care about
             * is the fact this namespace has an associated lun extension
             * and is therefore, exposed.
            */
            bNamespaceIsVisible = TRUE;
            lunIdToBeReturned = lunId;
            break;
        }
    } // for (lunId)

    if (NULL != pLunId) {
        *pLunId = lunIdToBeReturned;
    }

    return (bNamespaceIsVisible);
}

/******************************************************************************
 * NVMeGetNamespaceStatusAndSlot
 *
 * @brief This function returns status of a given NSID and the LUN Id it's assigned
 *
 * @param pDevExt - Pointer to device extension
 *		  targetNSID - ID of namespace in question.
 *
 * @return NS_STATUS - Indicates the namespace: INVALID, INACTIVE, or ATTACHED.
 *		   pTableIndex - index of namespace in the table.
 ******************************************************************************/
NS_STATUS NVMeGetNamespaceStatusAndSlot(
    PNVME_DEVICE_EXTENSION pDevExt,
    ULONG targetNSID,
    PULONG pLunId
)
{
    int i=0;
    for (i = 0; i < MAX_NAMESPACES; i++) {
        if (pDevExt->pLunExtensionTable[i]->namespaceId == targetNSID) {
            if (NULL != pLunId) {
                *pLunId = (ULONG)i;
            }
            return pDevExt->pLunExtensionTable[i]->nsStatus;
        }
    }

    if (NULL != pLunId) {
        *pLunId = (ULONG)INVALID_LUN_EXTN;
    }
    return INVALID;
} /* NVMeGetNamespaceStatus */

/******************************************************************************
 * NVMeGetFreeLunSlot
 *
 * @brief This function returns status of a given NSID.
 *
 * @param pDevExt - Pointer to device extension
 *		  targetNSID - ID of namespace in question.
 *
 * @return NS_STATUS - Indicates the namespace: INVALID, INACTIVE, or ATTACHED.
 *		   pTableIndex - index of namespace in the table.
 ******************************************************************************/
ULONG NVMeGetFreeLunSlot(
    PNVME_DEVICE_EXTENSION pDevExt
)
{
    int i;
    for (i = 0; i < MAX_NAMESPACES; i++) {
        if ((0 == pDevExt->pLunExtensionTable[i]->namespaceId) &&
            (FREE == pDevExt->pLunExtensionTable[i]->slotStatus) &&
            (INVALID == pDevExt->pLunExtensionTable[i]->nsStatus)) {
            return (ULONG)i;
        }
    }

    return INVALID_LUN_EXTN;
} /* NVMeGetFreeLunSlot */

/*******************************************************************************
 * NVMeIoctlTxDataToHost
 *
 * @brief NVMeIoctlTxDataToHost handles the data transfer from device to host
 * for pass thru commands
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 *
 * @return IOCTL_COMPLETED - Indicates the request should be completed due to
 *                           certain errors.
 *         IOCTL_PEDNING - Indicates more processing required.
 ******************************************************************************/
BOOLEAN NVMeIoctlTxDataToHost(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PNVMe_COMMAND pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;
    ULONG DataBufferSize = pNvmeCmd->NDP * sizeof(ULONG);
    ULONG IoctlHdrSize = sizeof(NVME_PASS_THROUGH_IOCTL);

    /*
     * If NDP is zero, return NVME_IOCTL_ZERO_DATA_TX_LENGTH_ERROR.
     */
    if (DataBufferSize == 0) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_ZERO_DATA_TX_LENGTH_ERROR;
        return IOCTL_COMPLETED;
    }
    /*
     * Ensure the size of return buffer is big enough to accommodate the header
     * and data.
     */
    if (pNvmePtIoctl->ReturnBufferLen < (IoctlHdrSize + DataBufferSize - 1)) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_OUT_BUFFER;
        return IOCTL_COMPLETED;
    }

    /* Prepare the PRP entries for the transfer */
    if (NVMePreparePRPs(pDevExt,
                        (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb),
                        pNvmePtIoctl->DataBuffer,
                        DataBufferSize) == FALSE) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;
        return IOCTL_COMPLETED;
    }

    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlTxDataToHost */

/*******************************************************************************
 * NVMeIoctlTxDataToDev
 *
 * @brief NVMeIoctlTxDataToDev handles the data transfer from host to device
 * for pass thru commands
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 * @param pNvmePtIoctl - Pointer to pass through IOCTL
 *
 * @return IOCTL_COMPLETED - Indicates the request should be completed due to
 *                           certain errors.
 *         IOCTL_PEDNING - Indicates more processing required.
 ******************************************************************************/
BOOLEAN NVMeIoctlTxDataToDev(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)pNvmePtIoctl;
    PNVMe_COMMAND pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;
    ULONG DataBufferSize = pNvmeCmd->NDP * sizeof(ULONG);
    ULONG IoctlHdrSize = sizeof(NVME_PASS_THROUGH_IOCTL);

    /*
     * If NDP is zero, return NVME_IOCTL_ZERO_DATA_TX_LENGTH_ERROR.
     */
    if (DataBufferSize == 0) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_ZERO_DATA_TX_LENGTH_ERROR;
        return IOCTL_COMPLETED;
    }
    /*
     * Ensure the size of data buffer is big enough to accommodate the data.
     */
    if (pNvmePtIoctl->DataBufferLen < DataBufferSize) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_IN_BUFFER;
        return IOCTL_COMPLETED;
    }

    /* Prepare the PRP entries for the transfer */
    if (NVMePreparePRPs(pDevExt,
                        (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb),
                        pNvmePtIoctl->DataBuffer,
                        DataBufferSize) == FALSE) {
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_PRP_TRANSLATION_ERROR;
        return IOCTL_COMPLETED;
    }

    pSrbIoCtrl->ReturnCode = NVME_IOCTL_SUCCESS;

    return IOCTL_PENDING;
} /* NVMeIoctlTxDataToDev */

/*******************************************************************************
 * NVMeProcessIoctl
 *
 * @brief Process IOCTLs based on signature
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 *
 * @return BOOLEAN
 *     TRUE - If all resources are allocated and initialized properly
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeProcessIoctl(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)GET_DATA_BUFFER(pSrb);
    BOOLEAN status = IOCTL_PENDING;

    StorPortDebugPrint(INFO,
                       "NVMeProcessIoctl: Code = 0x%x, Signature = 0x%s\n",
                       pSrbIoCtrl->ControlCode,
                       &pSrbIoCtrl->Signature[0]);

    /*
     * Call the appropriate process function for each signature. The processing
     * function is expected to set SrbStatus and the Return Code appropriately.
     */
    if (strncmp((const char*)pSrbIoCtrl->Signature,
                NVME_SIG_STR,
                NVME_SIG_STR_LEN) == 0) {
        status = NVMeProcessPrivateIoctl(pDevExt, pSrb);
    } else if (strncmp((const char*)pSrbIoCtrl->Signature,
                SCSI_SIG_STR,
                SCSI_SIG_STR_LEN) == 0) {
        status = NVMeProcessPublicIoctl(pDevExt, pSrb);
    } else {
        /* Indicate no valid signature was detected */
        pSrb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
        pSrbIoCtrl->ReturnCode = NVME_IOCTL_INVALID_SIGNATURE;
        status = IOCTL_COMPLETED;
    }

    return status;
} /* NVMeProcessIoctl */

/*******************************************************************************
 * NVMeProcessPublicIoctl
 *
 * @brief Process the Microsoft Public IOCTLs
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 *
 * @return IOCTL_COMPLETED - Indicates the request should be completed due to
 *                           certain errors.
 *         IOCTL_PEDNING - Indicates more processing required.
 ******************************************************************************/
BOOLEAN NVMeProcessPublicIoctl(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PSRB_IO_CONTROL pSrbIoCtrl = (PSRB_IO_CONTROL)GET_DATA_BUFFER(pSrb);
    PGETVERSIONINPARAMS versionParameters = NULL;
    PSENDCMDOUTPARAMS pCmdOutParameters = NULL;
    STOR_PHYSICAL_ADDRESS physAddr;
    ULONG paLength;
    BOOLEAN status = IOCTL_PENDING;
    UINT32 numDwords = 0;

    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    switch (pSrbIoCtrl->ControlCode) {
        case IOCTL_SCSI_MINIPORT_SMART_VERSION:
            /*
             * check data transfer length to make sure we have enough room for
             * the identify data -1 is because the first byte of data output is
             * contained in PGETVERSIONINPARAMS
             */
            if (GET_DATA_LENGTH(pSrb) <
                (sizeof(SRB_IO_CONTROL) + sizeof(GETVERSIONINPARAMS))) {
                pSrbIoCtrl->ReturnCode = SRB_STATUS_DATA_OVERRUN;
                pSrb->SrbStatus = SRB_STATUS_DATA_OVERRUN;
                status = IOCTL_COMPLETED;
                break;
            }

            versionParameters = (PGETVERSIONINPARAMS)(((PUCHAR)GET_DATA_BUFFER(pSrb))
                + sizeof(SRB_IO_CONTROL));

            StorPortDebugPrint(
                INFO,
                "NVMeProcessIoctl: IOCTL SCSI MINIPORT SMART VERSION\n");

            /* Version and revision per SMART 1.03 */
            versionParameters->bVersion = 1;
            versionParameters->bRevision = 1;
            versionParameters->bReserved = 0;

            /*
             * ToDo - Code copied from AHCI driver, need to update this
             */
            versionParameters->fCapabilities = (CAP_ATA_ID_CMD   |
                                                CAP_ATAPI_ID_CMD |
                                                CAP_SMART_CMD);

            pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            pSrbIoCtrl->ReturnCode = SRB_STATUS_SUCCESS;
            status = IOCTL_COMPLETED;
        break;
        case IOCTL_SCSI_MINIPORT_IDENTIFY:
            /*
             * check data transfer length to make sure we have enough room for
             * the identify data -1 is because the first byte of data output is
             * contained in SENDCMDOUTPARAMS
             */
            if (GET_DATA_LENGTH(pSrb) <
                (sizeof(SRB_IO_CONTROL) + sizeof(SENDCMDOUTPARAMS)
                                        + IDENTIFY_BUFFER_SIZE -1)) {
                pSrbIoCtrl->ReturnCode = SRB_STATUS_DATA_OVERRUN;
                pSrb->SrbStatus = SRB_STATUS_DATA_OVERRUN;
                status = IOCTL_COMPLETED;
                break;
            }

            StorPortDebugPrint(
                INFO,
                "NVMeProcessIoctl: IOCTL SCSI MINIPORT IDENTIFY\n");

            pCmdOutParameters = (PSENDCMDOUTPARAMS)(((PUCHAR)GET_DATA_BUFFER(pSrb))
                + sizeof(SRB_IO_CONTROL));

            memset((PUCHAR)pCmdOutParameters, 0, sizeof(SENDCMDOUTPARAMS)
                + IDENTIFY_BUFFER_SIZE - 1);

            /* Build status block. */
            pCmdOutParameters->cBufferSize = IDENTIFY_BUFFER_SIZE;
            pCmdOutParameters->DriverStatus.bDriverError = 0;
            pCmdOutParameters->DriverStatus.bIDEError = 0;

            /* Build the identify data */
            NVMeBuildIdentify(pCmdOutParameters, pDevExt);

            pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            pSrbIoCtrl->ReturnCode = SRB_STATUS_SUCCESS;
            status = IOCTL_COMPLETED;
        break;
        case IOCTL_SCSI_MINIPORT_ENABLE_SMART:

            /*
             * Do nothing as NVMe currently does not map an Enable for SMART
             * commands.
             */

            pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            pSrbIoCtrl->ReturnCode = SRB_STATUS_SUCCESS;
            status = IOCTL_COMPLETED;
        break;
        case IOCTL_SCSI_MINIPORT_DISABLE_SMART:

            /*
             * Do nothing as NVMe currently does not map an Disable for SMART
             * commands.
             */

            pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            pSrbIoCtrl->ReturnCode = SRB_STATUS_SUCCESS;
            status = IOCTL_COMPLETED;
        break;
        case IOCTL_SCSI_MINIPORT_READ_SMART_ATTRIBS:
            /*
             * check data transfer length to make sure we have enough room for
             * the identify data -1 is because the first byte of data output is
             * contained in SENDCMDOUTPARAMS
             */
            if (GET_DATA_LENGTH(pSrb) <
                (sizeof(SRB_IO_CONTROL) +
                 sizeof(NVME_SMART_READ_ATTRIBUTES_DATA) -1)) {
                pSrbIoCtrl->ReturnCode = SRB_STATUS_DATA_OVERRUN;
                pSrb->SrbStatus = SRB_STATUS_DATA_OVERRUN;
                status = IOCTL_COMPLETED;
                break;
            }

            /* Allocate new buffer for NVMe Read Log */
            pSrbExt->pDataBuffer = NVMeAllocatePool(pDevExt, PAGE_SIZE_IN_4KB);

            /* Set up the GET LOG PAGE command */
            memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
            pSrbExt->nvmeSqeUnit.CDW0.OPC = ADMIN_GET_LOG_PAGE;
            pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
            pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

            /* DWORD 10 */
            pSrbExt->nvmeSqeUnit.CDW10 |=
                (sizeof(ADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY)
                << BYTE_SHIFT_2);
            pSrbExt->nvmeSqeUnit.CDW10 |=
                (SMART_HEALTH_INFORMATION & DWORD_MASK_LOW_WORD);

            physAddr = StorPortGetPhysicalAddress(pDevExt,
                                                    NULL,
                                                    pSrbExt->pDataBuffer,
                                                    &paLength);

            /* This command only requires a single 4K page */
            if (physAddr.QuadPart != 0) {
                pSrbExt->nvmeSqeUnit.PRP1 = physAddr.QuadPart;
                pSrbExt->nvmeSqeUnit.PRP2 = 0;
            } else {
                pSrbIoCtrl->ReturnCode = SRB_STATUS_ERROR;
                pSrb->SrbStatus = SRB_STATUS_ERROR;
                status = IOCTL_COMPLETED;
                break;
            }

            /* This command goes to the admin queue */
            pSrbExt->forAdminQueue = TRUE;

            /* Set the custom completion routine */
            pSrbExt->pNvmeCompletionRoutine =
                (PNVME_COMPLETION_ROUTINE)NVMeIoctlCallback;

            /* Indicate there is no PRP list */
            pSrbExt->prpList[0] = 0;

            pSrb->SrbStatus = IOCTL_PENDING;
        break;
        case IOCTL_SCSI_MINIPORT_READ_SMART_THRESHOLDS:
            /*
             * check data transfer length to make sure we have enough room for
             * the identify data -1 is because the first byte of data output is
             * contained in SENDCMDOUTPARAMS
             */
            if (GET_DATA_LENGTH(pSrb) <
                (sizeof(SRB_IO_CONTROL) +
                 sizeof(NVME_SMART_READ_THRESHOLDS_DATA) -1)) {
                pSrbIoCtrl->ReturnCode = SRB_STATUS_DATA_OVERRUN;
                pSrb->SrbStatus = SRB_STATUS_DATA_OVERRUN;
                status = IOCTL_COMPLETED;
                break;
            }

            /* Allocate new buffer for NVMe Read Log */
            pSrbExt->pDataBuffer = NVMeAllocatePool(pDevExt, PAGE_SIZE_IN_4KB);

            /* Set up the GET LOG PAGE command */
            memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
            pSrbExt->nvmeSqeUnit.CDW0.OPC = ADMIN_GET_LOG_PAGE;
            pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
            pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

            /* DWORD 10 */
            numDwords =
                ((sizeof(ADMIN_GET_LOG_PAGE_ERROR_INFORMATION_LOG_ENTRY) / NUM_BYTES_IN_DWORD) - 1);
            pSrbExt->nvmeSqeUnit.CDW10 |= (numDwords << BYTE_SHIFT_2);

            pSrbExt->nvmeSqeUnit.CDW10 |=
                (SMART_HEALTH_INFORMATION & DWORD_MASK_LOW_WORD);

            physAddr = StorPortGetPhysicalAddress(pDevExt,
                                                    NULL,
                                                    pSrbExt->pDataBuffer,
                                                    &paLength);

            /* This command only requires a single 4K page */
            if (physAddr.QuadPart != 0) {
                pSrbExt->nvmeSqeUnit.PRP1 = physAddr.QuadPart;
                pSrbExt->nvmeSqeUnit.PRP2 = 0;
            } else {
                pSrbIoCtrl->ReturnCode = SRB_STATUS_ERROR;
                pSrb->SrbStatus = SRB_STATUS_ERROR;
            status = IOCTL_COMPLETED;
        break;
            }

            /* This command goes to the admin queue */
            pSrbExt->forAdminQueue = TRUE;

            /* Set the custom completion routine */
            pSrbExt->pNvmeCompletionRoutine =
                (PNVME_COMPLETION_ROUTINE)NVMeIoctlCallback;

            /* Indicate there is no PRP list */
            pSrbExt->prpList[0] = 0;

            status = IOCTL_PENDING;
        break;
        default:
            StorPortDebugPrint(
                INFO,
                "NVMeProcessMsftPublicIoctl: UNSUPPORTED Ctrl Code (0x%x)\n",
                       pSrbIoCtrl->ControlCode);

            pSrb->SrbStatus = SRB_STATUS_INVALID_REQUEST;
            pSrbIoCtrl->ReturnCode = NVME_IOCTL_INVALID_IOCTL_CODE;

            status = IOCTL_COMPLETED;
        break;
    }

    return status;
} /* NVMeProcessPublicIoctl */

/*******************************************************************************
 * NVMeProcessPrivateIoctl
 *
 * @brief Process the IOCTLs Private to this driver
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 *
 * @return BOOLEAN
 *     IOCTL_COMPLETED - do not call StartIO
 *     IOCTL_PENDING; - mor processing needed, call Startio
 ******************************************************************************/
BOOLEAN NVMeProcessPrivateIoctl(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PSRB_IO_CONTROL pSrbIoCtrl = NULL;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pSrbIoCtrl = (PSRB_IO_CONTROL)(GET_DATA_BUFFER(pSrb));

    /*
     * We only have 2 types of private IOCTLs, Pass Through with
     * several variations and Private with several control codes
     */

    if (pSrbIoCtrl->ControlCode == NVME_PASS_THROUGH_SRB_IO_CODE) {

        PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl = NULL;
        ULONG PtIoctlSize = sizeof(NVME_PASS_THROUGH_IOCTL) - 1;
        PNVMe_COMMAND pNvmeCmd = NULL;
        PNVMe_COMMAND_DWORD_0 pNvmeCmdDW0 = NULL;
        USHORT supportFwActFwDl;
        PADMIN_IDENTIFY_CONTROLLER pCntrlIdData;

        pNvmePtIoctl = (PNVME_PASS_THROUGH_IOCTL)(GET_DATA_BUFFER(pSrb));

        /*
         * If the input buffer length is not big enough, note it in
         * ReturnCode of SRB_IO_CONTROL and return.
         */
        if (GET_DATA_LENGTH(pSrb) < PtIoctlSize) {
            pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_IN_BUFFER;
            return IOCTL_COMPLETED;
        }

        /*
         * If return buffer length is less than size of
         * NVME_PASS_THROUGH_IOCTL, note it in ReturnCode of SRB_IO_CONTROL
         * and return
         */
        if (pNvmePtIoctl->ReturnBufferLen < PtIoctlSize) {
            pSrbIoCtrl->ReturnCode = NVME_IOCTL_INSUFFICIENT_OUT_BUFFER;
            return IOCTL_COMPLETED;
        }

        /* Process the request based on the Control code */
        pNvmeCmd = (PNVMe_COMMAND)pNvmePtIoctl->NVMeCmd;
        pNvmeCmdDW0 = (PNVMe_COMMAND_DWORD_0)&pNvmePtIoctl->NVMeCmd[0];

        /*
         * Setup generic IOCTL callback, indiviudal IOCTLs may overide
         * but are repsonislbe for all post processing if they do -
         * or they should call the generic callback directly before
         * completing (and overwrite status if needed)
         */
        pSrbExt->pNvmeCompletionRoutine = NVMeIoctlCallback;

        /* Separate Admin and NVM commands via QueueId */
        if (pNvmePtIoctl->QueueId == 0) {
            /*
             * Process Admin commands here. In case of errors, complete the
             * request and return IOCTL_COMPLETED. Otherwise, return
             * IOCTL_PENDING.
             */
            pSrbExt->forAdminQueue = TRUE;
            switch (pNvmeCmdDW0->OPC) {
                case ADMIN_CREATE_IO_SUBMISSION_QUEUE:
                case ADMIN_CREATE_IO_COMPLETION_QUEUE:
                case ADMIN_DELETE_IO_SUBMISSION_QUEUE:
                case ADMIN_DELETE_IO_COMPLETION_QUEUE:
                case ADMIN_ABORT:
                    /* Reject unsupported commands */
                    pSrbIoCtrl->ReturnCode =
                        NVME_IOCTL_UNSUPPORTED_ADMIN_CMD;

                    return IOCTL_COMPLETED;
                break;

                case ADMIN_ASYNCHRONOUS_EVENT_REQUEST:
                    /*
                     * Add this command to AERs already issued. Calculate if over
                     * the limit, if so, return FALSE. The AER limit indicated in
                     * Controller structure is 0-based.
                     */
                    if (++(pDevExt->DriverState.NumAERsIssued) >
                        pDevExt->controllerIdentifyData.UAERL + 1) {
                        pNvmePtIoctl->SrbIoCtrl.ControlCode = NVME_IOCTL_MAX_AER_REACHED;
                        return IOCTL_COMPLETED;
                    }
                break;
            case ADMIN_NAMESPACE_MANAGEMENT:
                
                pCntrlIdData = &pDevExt->controllerIdentifyData;
                
                /* Ensure the command is supported first. */
                if (pCntrlIdData->OACS.SupportsNamespaceMgmtAndAttachment) {
                    if (NVMeIoctlNamespaceMgmt(pDevExt, pSrb, pNvmePtIoctl) ==
                                        IOCTL_COMPLETED) {
                        return IOCTL_COMPLETED;
                    }
                } else {
                    pSrbIoCtrl->ReturnCode = NVME_IOCTL_UNSUPPORTED_ADMIN_CMD;
                    return IOCTL_COMPLETED;
                }
            break;
            case ADMIN_NAMESPACE_ATTACHMENT:
                
                pCntrlIdData = &pDevExt->controllerIdentifyData;
                
                /* Ensure the command is supported first. */
                if (pCntrlIdData->OACS.SupportsNamespaceMgmtAndAttachment) {
                    if (NVMeIoctlNamespaceAttachment(pDevExt, pSrb,
                            pNvmePtIoctl) == IOCTL_COMPLETED) {
                        return IOCTL_COMPLETED;
                    }
                } else {
                    pSrbIoCtrl->ReturnCode = NVME_IOCTL_UNSUPPORTED_ADMIN_CMD;
                    return IOCTL_COMPLETED;
                }
            break;
                case ADMIN_GET_LOG_PAGE:
                    if (NVMeIoctlGetLogPage(pDevExt, pSrb, pNvmePtIoctl) ==
                                            IOCTL_COMPLETED) {
                        return IOCTL_COMPLETED;
                    }
                break;
                case ADMIN_IDENTIFY:
                    if (NVMeIoctlIdentify(pDevExt, pSrb, pNvmePtIoctl) ==
                                            IOCTL_COMPLETED) {
                        return IOCTL_COMPLETED;
                    }
                break;
                case ADMIN_SET_FEATURES:
                case ADMIN_GET_FEATURES:
                    if (NVMeIoctlSetGetFeatures(
                               pDevExt, pSrb, pNvmePtIoctl, pNvmeCmdDW0->OPC)
                                == IOCTL_COMPLETED) {
                        return IOCTL_COMPLETED;
                    }
                break;
                case ADMIN_FIRMWARE_IMAGE_DOWNLOAD:
                    if (NVMeIoctlFwDownload(pDevExt, pSrb, pNvmePtIoctl) ==
                                            IOCTL_COMPLETED) {
                        return IOCTL_COMPLETED;
                    }
                break;
                case ADMIN_SECURITY_SEND:
                case ADMIN_SECURITY_RECEIVE:
                    if (NVMeIoctlSecuritySendRcv(
                            pDevExt, pSrb, pNvmePtIoctl, pNvmeCmdDW0->OPC)
                            == IOCTL_COMPLETED) {
                        return IOCTL_COMPLETED;
                    }
                break;
                case ADMIN_FORMAT_NVM:

                    /* Ensure the command is supported */
                    if (pDevExt->controllerIdentifyData.OACS.SupportsFormatNVM == 0) {
                        pSrbIoCtrl->ReturnCode = NVME_IOCTL_UNSUPPORTED_ADMIN_CMD;
                        return IOCTL_COMPLETED;
                    }

                    if (pDevExt->FormatNvmInfo.State !=
                            FORMAT_NVM_NO_ACTIVITY) {
                        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                        pSrbIoCtrl->ReturnCode =
                                    NVME_IOCTL_FORMAT_NVM_FAILED;
                        return IOCTL_COMPLETED;
                    }

                    pDevExt->FormatNvmInfo.State = FORMAT_NVM_RECEIVED;

                    /* Set status as pending and handle it in StartIo */
                    pSrbExt->pNvmeCompletionRoutine = NVMeIoctlFormatNVMCallback;
                    pSrb->SrbStatus = SRB_STATUS_PENDING;
                break;
                case ADMIN_FIRMWARE_ACTIVATE:
                    /*
                     * No pre-processing required. Ensure the command is
                     * supported first.
                     */

                    pCntrlIdData = &pDevExt->controllerIdentifyData;
                    supportFwActFwDl =
                        pCntrlIdData->OACS.SupportsFirmwareActivateFirmwareDownload;

                    if (supportFwActFwDl == 0) {
                        pSrbIoCtrl->ReturnCode =
                            NVME_IOCTL_UNSUPPORTED_ADMIN_CMD;
                        return IOCTL_COMPLETED;
                    }
                break;
                default:
                    /*
                     * Supported Opcodes of Admin vendor specific commands
                     * are from 0xC0 to 0xFF.
                     */
                    if (pNvmeCmdDW0->OPC < ADMIN_VENDOR_SPECIFIC_START) {
                        pSrbIoCtrl->ReturnCode =
                                NVME_IOCTL_INVALID_ADMIN_VENDOR_SPECIFIC_OPCODE;

                        return IOCTL_COMPLETED;
                    }
                    /*
                     * Check if AVSCC bit is set as 1, indicating support of
                     * ADMIN vendor specific command handling via Pass
                     * Through.
                     */
                    if (pDevExt->controllerIdentifyData.AVSCC == 0) {
                        pSrbIoCtrl->ReturnCode =
                                NVME_IOCTL_ADMIN_VENDOR_SPECIFIC_NOT_SUPPORTED;

                        return IOCTL_COMPLETED;
                    } else {
                        /*
                         * Handle data transfer if necessary
                         */
                        if (pNvmePtIoctl->Direction == NVME_FROM_HOST_TO_DEV) {
                            if (NVMeIoctlTxDataToDev(pDevExt,
                                                     pSrb,
                                                     pNvmePtIoctl)
                                                     == IOCTL_COMPLETED) {
                                return IOCTL_COMPLETED;
                            }
                        } else if (pNvmePtIoctl->Direction == NVME_FROM_DEV_TO_HOST) {
                            if (NVMeIoctlTxDataToHost(pDevExt,
                                                      pSrb,
                                                      pNvmePtIoctl)
                                                      == IOCTL_COMPLETED) {
                                return IOCTL_COMPLETED;
                            }
                        } else if (pNvmePtIoctl->Direction >= NVME_BI_DIRECTION) {
                            /* Currently, no transferring data bi-directionally */
                            pSrbIoCtrl->ReturnCode =
                                NVME_IOCTL_INVALID_DIRECTION_SPECIFIED;

                            return IOCTL_COMPLETED;
                        }
                    }
                break;
            } /* end switch */
        /* Process ADMIN commands ends */
        } else {
            /* Process NVM commands here */
            pSrbExt->forAdminQueue = FALSE;

            switch (pNvmeCmdDW0->OPC) {
                case NVM_WRITE:
                case NVM_READ:
                    /* Reject unsupported commands */
                    pSrbIoCtrl->ReturnCode = NVME_IOCTL_UNSUPPORTED_NVM_CMD;
                    return IOCTL_COMPLETED;
                break;
                case NVM_COMPARE:
                    if (NVMeIoctlCompare(pDevExt,
                                         pSrb,
                                         pNvmePtIoctl) == IOCTL_COMPLETED)
                         return IOCTL_COMPLETED;
                break;
                case NVM_DATASET_MANAGEMENT:
                    if (NVMeIoctlDataSetManagement(pDevExt,
                                                   pSrb,
                                                   pNvmePtIoctl) == IOCTL_COMPLETED)
                        return IOCTL_COMPLETED;
                break;
                case NVM_FLUSH:
                case NVM_WRITE_UNCORRECTABLE:
                    /* No pre-processing required */
                break;
                default:
                    /*
                     * Supported Opcodes of NVM vendor specific commands
                     * are from 0x80 to 0xFF
                     */
                    if (pNvmeCmdDW0->OPC < NVM_VENDOR_SPECIFIC_START) {
                        pSrbIoCtrl->ReturnCode =
                            NVME_IOCTL_INVALID_NVM_VENDOR_SPECIFIC_OPCODE;
                        return IOCTL_COMPLETED;
                    }
                    /*
                     * Check if NVSCC bit is set as 1, indicating support of
                     * NVM vendor specific command handling via Pass Through
                     */
                    if (pDevExt->controllerIdentifyData.NVSCC == 0) {
                        pSrbIoCtrl->ReturnCode =
                            NVME_IOCTL_NVM_VENDOR_SPECIFIC_NOT_SUPPORTED;
                        return IOCTL_COMPLETED;
                    } else {
                        /*
                         * Handle data transfer if necessary
                         */
                        if (pNvmePtIoctl->Direction == NVME_FROM_HOST_TO_DEV) {
                            if (NVMeIoctlTxDataToDev(pDevExt,
                                                     pSrb,
                                                     pNvmePtIoctl)
                                                     == IOCTL_COMPLETED) {
                                return IOCTL_COMPLETED;
                            }
                        } else if (pNvmePtIoctl->Direction == NVME_FROM_DEV_TO_HOST) {
                            if (NVMeIoctlTxDataToHost(pDevExt,
                                                      pSrb,
                                                      pNvmePtIoctl)
                                                      == IOCTL_COMPLETED) {
                                return IOCTL_COMPLETED;
                            }
                        } else if (pNvmePtIoctl->Direction >= NVME_BI_DIRECTION) {
                            pSrbIoCtrl->ReturnCode =
                                NVME_IOCTL_INVALID_DIRECTION_SPECIFIED;
                             return IOCTL_COMPLETED;
                        }
                    }
                break;
            } /* end of switch */
        } /* Process NVM commands ends */
        pNvmeCmd->PRP1 = pSrbExt->nvmeSqeUnit.PRP1;
        pNvmeCmd->PRP2 = pSrbExt->nvmeSqeUnit.PRP2;

        StorPortCopyMemory((PVOID)&pSrbExt->nvmeSqeUnit,
                               (PVOID)pNvmePtIoctl->NVMeCmd,
                               sizeof(NVMe_COMMAND));
    } else {
        /* Process private IOCTLs */
    }

    return IOCTL_PENDING;
} /* NVMeProcessPrivateIoctl */

/*******************************************************************************
 * NVMeBuildIdentify
 *
 * @brief Convert the NVMe ID data to the ATA Identify Structure
 *
 * @param pDevExt - Pointer to hardware device extension.
 * @param pSrb - Pointer to SRB
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeBuildIdentify(
    PSENDCMDOUTPARAMS pCmdOutParameters,
    PNVME_DEVICE_EXTENSION pDevExt
)
{
    PIDENTIFY_DEVICE_DATA id_data =
        (PIDENTIFY_DEVICE_DATA)(pCmdOutParameters->bBuffer);
    UINT8 checksum = 0;
    UINT32 cs_i = 0;

    /*
     * ToDo - Fill out all the entire Identify Structure
     */

    id_data->CommandSetSupport.SmartCommands = TRUE;
    id_data->CommandSetActive.SmartCommands = TRUE;

    /* Copy Serial Number */
    StorPortCopyMemory(
        id_data->SerialNumber,
        pDevExt->controllerIdentifyData.SN,
        20);

    /* Copy Model Number */
    StorPortCopyMemory(
        id_data->ModelNumber,
        pDevExt->controllerIdentifyData.MN,
        40);

    /* Copy Firmware Version */
    StorPortCopyMemory(id_data->FirmwareRevision,
        pDevExt->controllerIdentifyData.FR,
        8);

    /* Generate 8-bit Checksum */
    for (cs_i = 0; cs_i < IDENTIFY_BUFFER_SIZE; cs_i++)
    {
        checksum += ((PUINT8)id_data)[cs_i];
    }
    id_data->CheckSum = checksum;
} /* NVMeBuildIdentify */

/*******************************************************************************
 * NVMeInitAdminQueues
 *
 * @brief This function initializes the admin queues (SQ/CQ pair)
 *
 * @param pAE - Pointer to device extension
 *
 * @return ULONG
 *     Returns status based upon results of init'ing the admin queues
 ******************************************************************************/
ULONG NVMeInitAdminQueues(
    PNVME_DEVICE_EXTENSION pAE
)
{
    ULONG Status;

    /* Initialize Admin Submission queue */
    Status = NVMeInitSubQueue(pAE, 0);
    if (Status != STOR_STATUS_SUCCESS) {
        return (Status);
    }

    /* Initialize Admin Completion queue */
    Status = NVMeInitCplQueue(pAE, 0);
    if (Status != STOR_STATUS_SUCCESS) {
        return (Status);
    }

    /* Initialize Command Entries */
    Status = NVMeInitCmdEntries(pAE, 0);
    if (Status != STOR_STATUS_SUCCESS) {
        return (Status);
    }

    /*
     * Enable adapter after initializing some controller and Admin queue
     * registers. Need to determine if the adapter is ready for
     * processing commands after entering Start State Machine
     */
    if ((NVMeEnableAdapter(pAE)) == FALSE){
        return (STOR_STATUS_UNSUCCESSFUL);
    }

    return (STOR_STATUS_SUCCESS);
} /* NVMeInitAdminQueues */

/*******************************************************************************
 * NVMeLogError
 *
 * @brief Logs error to storport.
 *
 * @param pAE - Pointer to device extension
 * @param ErrorNum - Code to log.  The error code will show up in the 4th DWORD
 *        (starting with 0) inthe system event log detailed data
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeLogError(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in ULONG ErrorNum
)
{

    StorPortDebugPrint(INFO,
                   "NvmeLogError: logging error (0x%x)\n",
                   ErrorNum);

    StorPortLogError(pAE,
                     NULL,
                     0,
                     0,
                     0,
                     SP_INTERNAL_ADAPTER_ERROR,
                     ErrorNum);
}

#if DBG
/*******************************************************************************
 * IO_StorPortNotification
 *
 * @brief Debug routine for completing IO through one function to Storport.
 *
 * @param NotificationType - Type of StorPort notification
 * @param pHwDeviceExtension - Pointer to device extension
 * @param pSrb - Pointer to SRB
 *
 * @return VOID
 ******************************************************************************/
VOID IO_StorPortNotification(
    __in SCSI_NOTIFICATION_TYPE NotificationType,
    __in PVOID pHwDeviceExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
    __in PSTORAGE_REQUEST_BLOCK pSrb
#else
    __in PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
#if (NTDDI_VERSION > NTDDI_WIN7)
    PCDB pCdb = SrbGetCdb((void*)pSrb);
    UCHAR scsiStatus = 0;
    SrbGetScsiData(pSrb, NULL, NULL, &scsiStatus, NULL, NULL);
#endif

    if ((pSrb->SrbStatus != SRB_STATUS_SUCCESS) ||
#if (NTDDI_VERSION > NTDDI_WIN7)
        (scsiStatus != SCSISTAT_GOOD)) {
        if (pCdb == NULL) {
            StorPortDebugPrint(INFO, "FYI: CDB pointer is NULL!\n");
            StorPortDebugPrint(INFO,
                           "FYI: SRB status 0x%x scsi 0x%x for BTL %d %d %d\n",
                           pSrb->SrbStatus,
                           scsiStatus,
                           SrbGetPathId((void*)pSrb),
                           SrbGetTargetId((void*)pSrb),
                           SrbGetLun((void*)pSrb));
        } else {
            StorPortDebugPrint(INFO,
                           "FYI: SRB status 0x%x scsi 0x%x for CDB 0x%x BTL %d %d %d\n",
                           pSrb->SrbStatus,
                           scsiStatus,
                           *pCdb,
                           SrbGetPathId((void*)pSrb),
                           SrbGetTargetId((void*)pSrb),
                           SrbGetLun((void*)pSrb));
        }
#else
        (pSrb->ScsiStatus != SCSISTAT_GOOD)){

        /* DbgBreakPoint(); */
        StorPortDebugPrint(INFO,
                           "FYI: SRB status 0x%x scsi 0x%x for CDB 0x%x BTL %d %d %d\n",
                           pSrb->SrbStatus,
                           pSrb->ScsiStatus,
                           pSrb->Cdb[0],
                           pSrb->PathId,
                           pSrb->TargetId,
                           pSrb->Lun);
#endif
    }

    StorPortNotification(NotificationType,
                         pHwDeviceExtension,
                         pSrb);
}
#endif

#ifndef DBG
VOID WppCleanupRoutine(PVOID arg1) {
    StorPortDebugPrint(INFO, "WppCleanupRoutine\n");

    WPP_CLEANUP(NULL);
}
#endif

