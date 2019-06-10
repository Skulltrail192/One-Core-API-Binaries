/**
 *******************************************************************************
 ** Copyright (c) 2011-2012                                                   **
 **                                                                           **
 **   Integrated Device Technology, Inc.                                      **
 **   Intel Corporation                                                       **
 **   LSI Corporation                                                         **
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
 * File: nvmeStat.c
 */

#include "precomp.h"
#ifndef DBG
#include "nvmestat.tmh"
#endif

/*******************************************************************************
 * NVMeCallArbiter
 *
 * @brief Calls the init state machine arbiter either via timer callback or
 *        directly depending on crashdump or not
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeCallArbiter(
    PNVME_DEVICE_EXTENSION pAE
)
{
	if ((pAE->ntldrDump == FALSE) && (pAE->polledResetInProg != TRUE)) {
        StorPortNotification(RequestTimerCall,
                             pAE,
                             NVMeRunning,
                             pAE->DriverState.CheckbackInterval);
    } else {
		/*
		 * NoOp in dump mode because NVMeRunningStartAttempt() steps through
		 * the initialization state machine in a while loop.
		 */
    }
} /* NVMeCallArbiter */

/*******************************************************************************
 * NVMeCrashDelay
 *
 * @brief Will delay, by spinning, the amount of time specified without relying
 *        on API not available in crashdump mode.  Should not be used outside of
 *        crashdump mode.
 *
 * @param delayInUsec - uSec to delay
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeCrashDelay(
    ULONG delayInUsec,
    BOOLEAN ntldrDump
)
{
    LARGE_INTEGER currTime;
    LARGE_INTEGER startTime;
    LARGE_INTEGER stopTime;

    /*
     * StorPortQuerySystemTime is updated every .01 and our delay should be
     * min .05 sec.
     */
    if (ntldrDump == FALSE) { 
        StorPortQuerySystemTime(&startTime);

        delayInUsec *= MICRO_TO_NANO;
        stopTime.QuadPart = startTime.QuadPart + delayInUsec;

        do {
            StorPortQuerySystemTime(&currTime);
        } while (currTime.QuadPart < stopTime.QuadPart);
    } else {
        KeStallExecutionProcessor(delayInUsec);
    }    
} /* NVMeCrashDelay */

/*******************************************************************************
 * NVMeRunningStartAttempt
 *
 * @brief NVMeRunningStartAttempt is the entry point of state machine. It is
 *        called to initialize and start the state machine. It returns the
 *        returned status from NVMeRunning to the callers.
 *
 * @param pAE - Pointer to adapter device extension.
 * @param resetDriven - Boolean to determine if reset driven
 * @param pResetSrb - Pointer to SRB for reset
 *
 * @return BOOLEAN
 *     TRUE: When the starting state is completed successfully
 *     FALSE: If the starting state had been determined as a failure
 ******************************************************************************/
BOOLEAN NVMeRunningStartAttempt(
    PNVME_DEVICE_EXTENSION pAE,
    BOOLEAN resetDriven,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pResetSrb
#else
    PSCSI_REQUEST_BLOCK pResetSrb
#endif
)
{
    ULONG passiveTimeout;

    /* Set up the timer interval (time per DPC callback) */
    pAE->DriverState.CheckbackInterval = STORPORT_TIMER_CB_us;

    /* Initializes the state machine and its variables */
    pAE->DriverState.DriverErrorStatus = 0;
    pAE->DriverState.NextDriverState = NVMeWaitOnRDY;
    pAE->DriverState.StateChkCount = 0;
    pAE->DriverState.IdentifyNamespaceFetched = 0;
    pAE->DriverState.CurrentNsid = 0;
    pAE->DriverState.InterruptCoalescingSet = FALSE;
    pAE->DriverState.ConfigLbaRangeNeeded = FALSE;
    pAE->DriverState.TtlLbaRangeExamined = 0;
    pAE->DriverState.NumAERsIssued = 0;
    pAE->DriverState.TimeoutCounter = 0;
    pAE->DriverState.resetDriven = resetDriven;
    pAE->DriverState.pResetSrb = pResetSrb;
    pAE->DriverState.VisibleNamespacesExamined = 0;
    pAE->DriverState.NumKnownNamespaces = 0;
#if DBG
    pAE->LearningComplete = FALSE;
#endif


    /* Zero out SQ cn CQ counters */
    pAE->QueueInfo.NumSubIoQCreated = 0;
    pAE->QueueInfo.NumCplIoQCreated = 0;
    pAE->QueueInfo.NumSubIoQAllocFromAdapter = 0;
    pAE->QueueInfo.NumCplIoQAllocFromAdapter = 0;
    pAE->QueueInfo.NumIoQMapped = 1;  /* mapping starts at 1, since 0 is admin queue */

    /* Zero out the LUN extensions and reset the counter as well */
    memset((PVOID)pAE->pLunExtensionTable[0],
           0,
           sizeof(NVME_LUN_EXTENSION) * MAX_NAMESPACES);

    /*
     * Now, starts state machine by calling NVMeRunning
     * We won't accept IOs until the machine finishes and if it
     * fails to finish we'll never accept IOs and simply log an error
     */
	if (pAE->ntldrDump == FALSE) {
		if (pAE->polledResetInProg == FALSE) {
			NVMeRunning(pAE);
		} else {
			/* 
			* we poll if we're launching the reinit state machine from HwStorResetBus
			* or HwStorAdapterControl->ScsiRestartAdapter path
			*/
			NVMeRunning(pAE);

			/* TO val is based on CAP register plus a few, 5, seconds to init post RDY */
			passiveTimeout = pAE->uSecCrtlTimeout + (STORPORT_TIMER_CB_us * MICRO_TO_SEC);

			while ((pAE->DriverState.NextDriverState != NVMeStartComplete) &&
				(pAE->DriverState.NextDriverState != NVMeStateFailed)){

				/* increment 5000us (timer callback val */
				pAE->DriverState.TimeoutCounter += pAE->DriverState.CheckbackInterval;
				if (pAE->DriverState.TimeoutCounter > passiveTimeout) {
					NVMeDriverFatalError(pAE,
						(1 << START_STATE_TIMEOUT_FAILURE));
					break;
				}

				NVMeRunning(pAE);
				NVMeStallExecution(pAE, STORPORT_TIMER_CB_us);
				IoCompletionRoutine(NULL, pAE, (PVOID)0, 0);
			}

			return (pAE->DriverState.NextDriverState == NVMeStartComplete) ? TRUE : FALSE;
		}
	} else {
		PRES_MAPPING_TBL pRMT = &pAE->ResMapTbl;
		pAE->LearningCores = pRMT->NumActiveCores; /* no need to learn cores in dump mode */

		/* In dump mode there is no timer. We have to poll for completion at each state. */
		while (pAE->DriverState.NextDriverState != NVMeStateFailed &&
			pAE->DriverState.NextDriverState != NVMeStartComplete) {
			NVMeRunning(pAE);
			NVMeCrashDelay(pAE->DriverState.CheckbackInterval, pAE->ntldrDump);
		}
	}

    return (TRUE);
} /* NVMeRunningStartAttempt */

/*******************************************************************************
 * NVMeStallExecution
 *
 * @brief Stalls for the # of usecs specified
 *
 * @param pAE - Pointer to adapter device extension.
 * @param microSeconds - time to stall
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeStallExecution(
    PNVME_DEVICE_EXTENSION pAE,
    ULONG microSeconds
)
{
    ULONG i;

    if (pAE->ntldrDump == FALSE) {
        for (i=0; i < microSeconds / MILLI_TO_MICRO; i++) {
            StorPortStallExecution(MILLI_TO_MICRO - 1);
        }
    } else {
        NVMeCrashDelay(microSeconds, pAE->ntldrDump);
    }
}

/*******************************************************************************
 * NVMeRunning
 *
 * @brief NVMeRunning is called to dispatch the processing depending on the next
 *        state. It can be called by NVMeRunningStartAttempt or Storport to call
 *        the associated function.
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunning(
    PNVME_DEVICE_EXTENSION pAE
)
{
	ULONG newVersion = 0;

    /*
     * Go to the next state in the Start State Machine
     * transitions are managed either in the state handler or
     * in the completion routines and executed via DPC (except crasdump)
     * calling back into this arbiter
     */
    switch (pAE->DriverState.NextDriverState) {
        case NVMeStateFailed:
            NVMeFreeBuffers(pAE);
        break;
        case NVMeWaitOnRDY:
            NVMeRunningWaitOnRDY(pAE);
        break;
        case NVMeWaitOnIdentifyCtrl:
            NVMeRunningWaitOnIdentifyCtrl(pAE);
        break;
        case NVMeWaitOnListAttachedNs:
            NVMeRunningWaitOnListAttachedNs(pAE);
        break;
        case NVMeWaitOnListExistingNs:
            NVMeRunningWaitOnListExistingNs(pAE);
        break;
        case NVMeWaitOnIdentifyNS:
            NVMeRunningWaitOnIdentifyNS(pAE);
        break;
        case NVMeWaitOnSetFeatures:
            NVMeRunningWaitOnSetFeatures(pAE);
        break;
        case NVMeWaitOnSetupQueues:
            NVMeRunningWaitOnSetupQueues(pAE);
        break;
        case NVMeWaitOnIoCQ:
            NVMeRunningWaitOnIoCQ(pAE);
        break;
        case NVMeWaitOnIoSQ:
            NVMeRunningWaitOnIoSQ(pAE);
        break;
        case NVMeWaitOnLearnMapping:
            NVMeRunningWaitOnLearnMapping(pAE);
        break;
        case NVMeWaitOnReSetupQueues:
            NVMeRunningWaitOnReSetupQueues(pAE);
        break;
		case NVMeWaitOnNamespaceReady:
			NVMeRunningWaitOnNamespaceReady(pAE);
		break;
        case NVMeStartComplete:
            pAE->RecoveryAttemptPossible = TRUE;
			newVersion = StorPortReadRegisterUlong(pAE, (PULONG)(&pAE->pCtrlRegister->VS));

			if (pAE->ntldrDump == FALSE  && newVersion != INVALID_DEVICE_REGISTER_VALUE && pAE->DeviceRemovedDuringIO != TRUE) {
#if (NTDDI_VERSION > NTDDI_WIN7)
				if (pAE->Timerhandle != NULL)
					StorPortRequestTimer(pAE, pAE->Timerhandle, IsDeviceRemoved, NULL, START_SURPRISE_REMOVAL_TIMER, 0);//every 1 seconds
#else
					StorPortNotification(RequestTimerCall, pAE, IsDeviceRemoved, START_SURPRISE_REMOVAL_TIMER); //start after 1 seconds
#endif
			}

            /* Indicate learning is done with no unassigned cores */
            pAE->LearningCores = pAE->ResMapTbl.NumActiveCores;

            if (pAE->DriverState.resetDriven) {
                /* If this was at the request of the host, complete that Srb */
                if (pAE->DriverState.pResetSrb != NULL) {
                    pAE->DriverState.pResetSrb->SrbStatus = SRB_STATUS_SUCCESS;

                    IO_StorPortNotification(RequestComplete,
                                            pAE,
                                            pAE->DriverState.pResetSrb);
                }
                pAE->DriverState.resetDriven = FALSE;
                pAE->DriverState.pResetSrb = NULL;
            }
            /* Ready again for host commands */
            StorPortDebugPrint(INFO,"NVMeRunning: StorPortReady...\n");
            StorPortResume(pAE);
        break;
        default:
        break;
    } /* end switch */
} /* NVMeRunning */

/*******************************************************************************
 * NVMeRunningWaitOnRDY
 *
 * @brief NVMeRunningWaitOnRDY is called to verify if the adapter is enabled and
 *        ready to process commands. It is called by NVMeRunning when the state
 *        machine is in NVMeWaitOnRDY state.
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnRDY(
    PNVME_DEVICE_EXTENSION pAE
)
{
    NVMe_CONTROLLER_CONFIGURATION CC;
    NVMe_CONTROLLER_STATUS        CSTS;

    /*
     * Checking to see if we're enabled yet, watching the timeout value
     * we read from the controller CAP register (StateChkCount is incr
     * in uSec in this case)
     */
    if ((pAE->DriverState.StateChkCount > pAE->uSecCrtlTimeout) ||
        (pAE->DriverState.DriverErrorStatus)) {
            NVMeDriverFatalError(pAE,
                                (1 << START_STATE_RDY_FAILURE));
    } else {

        /*
         * Look for signs of life. If it's ready, set NextDriverState to proceed to
         * next state. Otherwise, wait for 1 sec only for crashdump case otherwise
         * ask Storport to call again in normal driver case.
         */
        CSTS.AsUlong =
            StorPortReadRegisterUlong(pAE, &pAE->pCtrlRegister->CSTS.AsUlong);

        if (CSTS.RDY == 1) {
            StorPortDebugPrint(INFO,"NVMeRunningWaitOnRDY: RDY has been set\n");
            pAE->DriverState.NextDriverState = NVMeWaitOnIdentifyCtrl;
            pAE->DriverState.StateChkCount = 0;
        } else {
            if (pAE->DriverState.StateChkCount == 0) {
                StorPortDebugPrint(INFO,"NVMeRunningWaitOnRDY: Waiting...\n");
            }
            pAE->DriverState.NextDriverState = NVMeWaitOnRDY;
            pAE->DriverState.StateChkCount += pAE->DriverState.CheckbackInterval;
        }
    }
    NVMeCallArbiter(pAE);
} /* NVMeRunningWaitOnRDY */

/*******************************************************************************
 * NVMeRunningWaitOnIdentifyCtrl
 *
 * @brief NVMeRunningWaitOnIdentifyCtrl is called to issue Identify command to
 *        retrieve Controller structures.
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnIdentifyCtrl(
    PNVME_DEVICE_EXTENSION pAE
)
{
    /*
     * Issue Identify command for the very first time If failed, fail the state
     * machine
     */
    if (NVMeGetIdentifyStructures(pAE, 0, IDENTIFY_CNTLR) == FALSE) {
        NVMeDriverFatalError(pAE,
                            (1 << START_STATE_IDENTIFY_CTRL_FAILURE));
        NVMeCallArbiter(pAE);
        return;
    }
} /* NVMeRunningWaitOnIdentifyCtrl */

/*******************************************************************************
 * NVMeRunningWaitOnListAttachedNs
 *
 * @brief NVMeRunningWaitOnListAttachedNs is called to issue Identify command to
 *        list all the attached namespaces.
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnListAttachedNs(
    PNVME_DEVICE_EXTENSION pAE
)
{
    if (NVMeGetIdentifyStructures(pAE, 0, LIST_ATTACHED_NAMESPACES) == FALSE) {
        NVMeDriverFatalError(pAE,
                            (1 << START_STATE_LIST_ATTACHED_NS_FAILURE));
        NVMeCallArbiter(pAE);
    }
} /* NVMeRunningWaitOnListAttachedNs */

/*******************************************************************************
 * NVMeRunningWaitOnListExistingNs
 *
 * @brief NVMeRunningWaitOnListExistingNs is called to issue Identify command to
 *        list all known namespaces, attached or not.
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnListExistingNs(
    PNVME_DEVICE_EXTENSION pAE
)
{
    if (NVMeGetIdentifyStructures(pAE, 0, LIST_EXISTING_NAMESPACES) == FALSE) {
        NVMeDriverFatalError(pAE,
                            (1 << START_STATE_LIST_EXISTING_NS_FAILURE));
        NVMeCallArbiter(pAE);
    }
} /* NVMeRunningWaitOnListExistingNs */

/*******************************************************************************
 * NVMeRunningWaitOnIdentifyNS
 *
 * @brief NVMeRunningWaitOnIdentifyNS is called to issue Identify command to
 *        retrieve Namespace structures.
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnIdentifyNS(
    PNVME_DEVICE_EXTENSION pAE
)
{
    ULONG nsid = 0;

    if (pAE->controllerIdentifyData.OACS.SupportsNamespaceMgmtAndAttachment &&
        pAE->DriverState.NumKnownNamespaces > 0)
        nsid = pAE->pLunExtensionTable[pAE->DriverState.IdentifyNamespaceFetched]->namespaceId;
    else
        nsid = pAE->DriverState.CurrentNsid + 1;

    /*
     * Issue an identify command.  The completion handler will keep us at this
     * state if there are more identifies needed based on what the ctlr told us
     * If failed, fail the state machine
     *
     * Please note that NN of Controller structure is 1-based.
     */

    if (NVMeGetIdentifyStructures(pAE, nsid, IDENTIFY_NAMESPACE) == FALSE) {
        NVMeDriverFatalError(pAE,
                            (1 << START_STATE_IDENTIFY_NS_FAILURE));
        NVMeCallArbiter(pAE);
    }
} /* NVMeRunningWaitOnIdentifyNS */

/*******************************************************************************
 * NVMeRunningWaitOnSetupQueues
 *
 * @brief Called as part of init state machine to perform alloc of queues and
 *        setup of the resouce table
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnSetupQueues(
    PNVME_DEVICE_EXTENSION pAE
)
{
    PQUEUE_INFO pQI = &pAE->QueueInfo;
    USHORT QueueID;
    ULONG Status = STOR_STATUS_SUCCESS;

    /*
     * 1. Allocate IO queues
     * 2. Initialize IO queues
     * 3. Complete Resource Table
     *
     * If not, wait for 1 sec only for crashdump case or ask Storport to call
     * again in normal driver case
     */

    /* Allocate IO queues memory if they weren't already allocated */
    if (pAE->IoQueuesAllocated == FALSE) {
        if (NVMeAllocIoQueues( pAE ) == FALSE) {
            NVMeDriverFatalError(pAE,
                                (1 << START_STATE_QUEUE_ALLOC_FAILURE));
            NVMeCallArbiter(pAE);
            return;
        }

        pAE->IoQueuesAllocated = TRUE;
    }

    /*
     * Now we have all resources in place, complete resource mapping table now.
     */
    if (pAE->ResourceTableMapped == FALSE) {
        NVMeCompleteResMapTbl(pAE);
        pAE->ResourceTableMapped = TRUE;
    }

    /* Once we have the queue memory allocated, initialize them */
    for (QueueID = 1; QueueID <= pQI->NumSubIoQAllocated; QueueID++) {
        /* Initialize all Submission queues/entries */
        Status = NVMeInitSubQueue(pAE, QueueID);
        if (Status != STOR_STATUS_SUCCESS) {
            NVMeDriverFatalError(pAE,
                                (1 << START_STATE_QUEUE_INIT_FAILURE));
            NVMeCallArbiter(pAE);
            return;
        }
    }

    for (QueueID = 1; QueueID <= pQI->NumCplIoQAllocated; QueueID++) {
        /* Initialize all Completion queues/entries */
        Status = NVMeInitCplQueue(pAE, QueueID);
        if (Status != STOR_STATUS_SUCCESS) {
            NVMeDriverFatalError(pAE,
                                (1 << START_STATE_QUEUE_INIT_FAILURE));
            NVMeCallArbiter(pAE);
            return;
        }
    }

    for (QueueID = 1; QueueID <= pQI->NumSubIoQAllocated; QueueID++) {
        /* Initialize all CMD_ENTRY entries */
        Status = NVMeInitCmdEntries(pAE, QueueID);
        if (Status != STOR_STATUS_SUCCESS) {
            NVMeDriverFatalError(pAE,
                                (1 << START_STATE_QUEUE_INIT_FAILURE));
            NVMeCallArbiter(pAE);
            return;
        }
    }

    /*
     * Transition to NVMeWaitOnIoCQ state
     * pAE->StartState.NextStartState = NVMeWaitOnIoCQ;
     */
    ASSERT(pQI->NumCplIoQCreated == 0);
    pAE->DriverState.NextDriverState = NVMeWaitOnIoCQ;
    pAE->DriverState.StateChkCount = 0;

    NVMeCallArbiter(pAE);
} /* NVMeRunningWaitOnSetupQueues */

/*******************************************************************************
 * NVMeRunningWaitOnSetFeatures
 *
 * @brief NVMeRunningWaitOnSetFeatures is called to issue the following
 *        commands:
 *
 *        1. Set Features command (Interrupt Coalescing, Feature ID#8)
 *        2. Set Features command (Number of Queues, Feature ID#7)
 *        3. For each existing Namespace, Get Features (LBA Range Type) first.
 *           When its Type is 00b and NLB matches the size of the Namespace,
 *           isssue Set Features (LBA Range Type) to configure:
 *             a. its Type as Filesystem,
 *             b. can be overwritten, and
 *             c. to be visible
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnSetFeatures(
    PNVME_DEVICE_EXTENSION pAE
)
{
    PQUEUE_INFO pQI = &pAE->QueueInfo;

    /*
     * There are multiple steps hanlded in this state as they're all
     * grouped into 'set feature' type things.  This simplifies adding more
     * set features in the future as jst this sub-state machine needs updating
     */
    if (pAE->DriverState.InterruptCoalescingSet == FALSE) {
        if (NVMeSetIntCoalescing(pAE) == FALSE) {
            NVMeDriverFatalError(pAE,
                                (1 << START_STATE_SET_FEATURE_FAILURE));
            NVMeCallArbiter(pAE);
            return;
        }
    } else if (pQI->NumSubIoQAllocFromAdapter == 0) {
        if (NVMeAllocQueueFromAdapter(pAE) == FALSE) {
            NVMeDriverFatalError(pAE,
                                (1 << START_STATE_SET_FEATURE_FAILURE));
            NVMeCallArbiter(pAE);
            return;
        }
    } else {
        if(NVMeAccessLbaRangeEntry(pAE) == FALSE) {
            NVMeDriverFatalError(pAE,
                                (1 << START_STATE_SET_FEATURE_FAILURE));
            NVMeCallArbiter(pAE);
            return;
        }
    }
} /* NVMeRunningWaitOnSetFeatures */


/*******************************************************************************
 * NVMeRunningWaitOnIoCQ
 *
 * @brief NVMeRunningWaitOnIoCQ gets called to create IO completion queues via
 *        issuing Create IO Completion Queue command(s)
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnIoCQ(
    PNVME_DEVICE_EXTENSION pAE
)
{
    PQUEUE_INFO pQI = &pAE->QueueInfo;

    /*
     * Issue Create IO Completion Queue commands when first called
     * If failed, fail the state machine
     */
    if (NVMeCreateCplQueue(pAE, (USHORT)pQI->NumCplIoQCreated + 1 ) == FALSE) {
        NVMeDriverFatalError(pAE,
                            (1 << START_STATE_CPLQ_CREATE_FAILURE));
        NVMeCallArbiter(pAE);
    }
} /* NVMeRunningWaitOnIoCQ */

/*******************************************************************************
 * NVMeRunningWaitOnIoSQ
 *
 * @brief NVMeRunningWaitOnIoSQ gets called to create IO submission queues via
 *        issuing Create IO Submission Queue command(s)
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnIoSQ(
    PNVME_DEVICE_EXTENSION pAE
)
{
    PQUEUE_INFO pQI = &pAE->QueueInfo;

    /*
     * Issue Create IO Submission Queue commands when first called
     * If failed, fail the state machine
     */
    if (NVMeCreateSubQueue(pAE, (USHORT)pQI->NumSubIoQCreated + 1) == FALSE) {
        NVMeDriverFatalError(pAE,
                            (1 << START_STATE_SUBQ_CREATE_FAILURE));
        NVMeCallArbiter(pAE);
    }
} /* NVMeRunningWaitOnIoSQ */

/*******************************************************************************
 * NVMeDriverFatalError
 *
 * @brief NVMeDriverFatalError gets called to mark down the failure of state
 *        machine.
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeDriverFatalError(
    PNVME_DEVICE_EXTENSION pAE,
    ULONG ErrorNum
)
{
    ASSERT(FALSE);

    StorPortDebugPrint(ERROR, "NVMeDriverFatalError!\n");

    pAE->DriverState.DriverErrorStatus |= (ULONG64)(1 << ErrorNum);
    pAE->DriverState.NextDriverState = NVMeStateFailed;

    NVMeLogError(pAE, ErrorNum);

    return;
} /* NVMeDriverFatalError */

/*******************************************************************************
 * NVMeRunningWaitOnLearnMapping
 *
 * @brief NVMeRunningWaitOnLearnMapping is one of the final steps in the init
 *        state machine and simply sends a through each queue in order to
 *        excercise the learning mode in the IO path.
 *
 * NOTE/TODO:  This only works if a namespace exists on boot.  If the ctlr
 *             has no namespace defined until later, the Qs will not be
 *             optimized until the next boot.
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnLearnMapping(
    PNVME_DEVICE_EXTENSION pAE
)
{
    PNVME_SRB_EXTENSION pNVMeSrbExt =
        (PNVME_SRB_EXTENSION)pAE->DriverState.pSrbExt;
    PNVMe_COMMAND pCmd = (PNVMe_COMMAND)(&pNVMeSrbExt->nvmeSqeUnit);
    PRES_MAPPING_TBL pRMT = &pAE->ResMapTbl;
    STOR_PHYSICAL_ADDRESS PhysAddr;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    UINT32 lbaLengthPower, lbaLength;
    UINT8 flbas;
    BOOLEAN error = FALSE;

    __try {
        pLunExt = pAE->pLunExtensionTable[0];
#ifdef DUMB_DRIVER
        {
#else
		if ((pRMT->pMsiMsgTbl->Shared == TRUE) ||
			(pRMT->InterruptType == INT_TYPE_INTX) ||
			(pRMT->InterruptType == INT_TYPE_MSI)) {
#endif
			pAE->LearningCores = pRMT->NumActiveCores;
			pAE->DriverState.NextDriverState = NVMeWaitOnNamespaceReady;
			__leave;
		}

		/* if there is zero namespace, we go to startcomplete*/
		if ((pAE->DriverState.IdentifyNamespaceFetched == 0) ||
			(pAE->visibleLuns == 0) ||
			(pLunExt == NULL)) {
			pAE->LearningCores = pRMT->NumActiveCores;
			pAE->DriverState.NextDriverState = NVMeStartComplete;
			__leave;
		}

        /* Zero out the extension first */
        memset((PVOID)pNVMeSrbExt, 0, sizeof(NVME_SRB_EXTENSION));

        /* Populate SRB_EXTENSION fields */
        pNVMeSrbExt->pNvmeDevExt = pAE;
        pNVMeSrbExt->pNvmeCompletionRoutine = NVMeInitCallback;

        /* send a READ of 1 block to LBA0, NSID 0 */
        flbas = pLunExt->identifyData.FLBAS.SupportedCombination;
        lbaLengthPower = pLunExt->identifyData.LBAFx[flbas].LBADS;
        lbaLength = 1 << lbaLengthPower;

        pNVMeSrbExt->pDataBuffer = NVMeAllocatePool(pAE, lbaLength);
        if (NULL == pNVMeSrbExt->pDataBuffer) {
            /*
             * strange that we can't get a small amount of memory
             * so go ahead and complete the init state machine now
            */
            pAE->LearningCores = pRMT->NumActiveCores;
            pAE->DriverState.NextDriverState = NVMeStartComplete;
            __leave;
        }
        PhysAddr = NVMeGetPhysAddr(pAE, pNVMeSrbExt->pDataBuffer);
        pCmd->CDW0.OPC = NVME_READ;
        pCmd->PRP1 = (ULONGLONG)PhysAddr.QuadPart;
        pCmd->NSID = pLunExt->namespaceId;

        /* Now issue the command via IO queue */
        if (FALSE == ProcessIo(pAE, pNVMeSrbExt, NVME_QUEUE_TYPE_IO, FALSE)) {
            error = TRUE;
            pAE->LearningCores = pRMT->NumActiveCores;
            pAE->DriverState.NextDriverState = NVMeStartComplete;
            __leave;
        }

    } __finally {
        if ((error == TRUE) && (NULL != pNVMeSrbExt->pDataBuffer)) {
            StorPortFreePool((PVOID)pAE, pNVMeSrbExt->pDataBuffer);
        }
        if (pAE->DriverState.NextDriverState == NVMeStartComplete || pAE->DriverState.NextDriverState == NVMeWaitOnNamespaceReady) {
            NVMeCallArbiter(pAE);
        }
    }

    return;
} /* NVMeRunningWaitOnLearnMapping */

/*******************************************************************************
 * NVMeRunningWaitOnReSetupQueues
 *
 * @brief NVMeRunningWaitOnReSetupQueues gets called if learning mode decided
 *        that the queues were not correctly mapped.  It deletes all the
 *        queues and reallocates mem and recreates them based on learned
 *        mappings
 *
 * @param pAE - Pointer to adapter device extension.
 *
 * @return VOID
 ******************************************************************************/
VOID NVMeRunningWaitOnReSetupQueues(
    PNVME_DEVICE_EXTENSION pAE
)
{
    PQUEUE_INFO pQI = &pAE->QueueInfo;

    /* Delete all submission queues */
    if (NVMeDeleteSubQueues(pAE) == FALSE) {
        NVMeDriverFatalError(pAE,
                            (1 << FATAL_SUBQ_DELETE_FAILURE));
        NVMeCallArbiter(pAE);
        return;
    }

    /* Delete all completion queues if we're done deleting submision queues */
    if (pQI->NumSubIoQCreated == 0) {
        if (NVMeDeleteCplQueues(pAE) == FALSE) {
            NVMeDriverFatalError(pAE,
                                (1 << FATAL_SUBQ_DELETE_FAILURE));
            NVMeCallArbiter(pAE);
            return;
        }
    }

} /* NVMeRunningWaitOnReSetupQueues */


/*******************************************************************************
* NVMeRunningWaitOnNamespaceReady
*
* @brief NVMeRunningWaitOnNamespaceReady gets called to test if the namespace
*        is ready to handle I/0s.
*
* @param pAE - Pointer to adapter device extension.
*
* @return VOID
******************************************************************************/
VOID NVMeRunningWaitOnNamespaceReady(
	PNVME_DEVICE_EXTENSION pAE
	)
{
	PNVME_SRB_EXTENSION pNVMeSrbExt =
		(PNVME_SRB_EXTENSION)pAE->DriverState.pSrbExt;
	PNVMe_COMMAND pCmd = (PNVMe_COMMAND)(&pNVMeSrbExt->nvmeSqeUnit);
	PRES_MAPPING_TBL pRMT = &pAE->ResMapTbl;
	PNVME_LUN_EXTENSION pLunExt = NULL;
	STOR_PHYSICAL_ADDRESS PhysAddr;
	UINT32 lbaLengthPower, lbaLength;
	UINT8 flbas;
	BOOLEAN error = FALSE;

	__try {
		/* Zero out the extension first */
		memset((PVOID)pNVMeSrbExt, 0, sizeof(NVME_SRB_EXTENSION));

		/* Populate SRB_EXTENSION fields */
		pNVMeSrbExt->pNvmeDevExt = pAE;

		pLunExt = pAE->pLunExtensionTable[pAE->DriverState.NextNamespaceToCheckForReady];
		pNVMeSrbExt->pNvmeCompletionRoutine = NVMeInitCallback;

		/* Issue read command for attached and valid namespaces */
		if (pAE->controllerIdentifyData.OACS.SupportsNamespaceMgmtAndAttachment) {
			if ((pLunExt->nsStatus != ATTACHED) ||
				(pLunExt->slotStatus != ONLINE)) {
				pAE->DriverState.NextNamespaceToCheckForReady++;
				/* We need to ensure all namespaces are ready in case of multiple namespaces */
				if (pAE->DriverState.NextNamespaceToCheckForReady < pAE->DriverState.NumKnownNamespaces) {
					pAE->DriverState.NextDriverState = NVMeWaitOnNamespaceReady;
				}
				else {
					pAE->DriverState.AllNamespacesAreReady = TRUE;
					pAE->DriverState.NextDriverState = NVMeStartComplete;
				}
				__leave;
			}
		}

		/*Sending Read command to check if Namespace is Ready*/
		flbas = pLunExt->identifyData.FLBAS.SupportedCombination;
		lbaLengthPower = pLunExt->identifyData.LBAFx[flbas].LBADS;
		lbaLength = 1 << lbaLengthPower;
		pNVMeSrbExt->pDataBuffer = NVMeAllocatePool(pAE, lbaLength);
		if (NULL == pNVMeSrbExt->pDataBuffer) {
			/*
			* strange that we can't get a small amount of memory
			* so go ahead and complete the init state machine now
			*/
			pAE->DriverState.NextDriverState = NVMeStartComplete;
			__leave;
		}
		/* Prepare PRP entries, need at least one PRP entry */
		if (NVMePreparePRPs(pAE,
			pNVMeSrbExt,
			pNVMeSrbExt->pDataBuffer,
			lbaLength)
			== FALSE) {
			error = TRUE;
			pAE->LearningCores = pRMT->NumActiveCores;
			pAE->DriverState.NextDriverState = NVMeStartComplete;
			__leave;
		}
		pCmd->CDW0.OPC = NVME_READ;
		pCmd->NSID = pLunExt->namespaceId;

		/* Now issue the command via IO queue */
		if (FALSE == ProcessIo(pAE, pNVMeSrbExt, NVME_QUEUE_TYPE_IO, FALSE)) {
			error = TRUE;
			pAE->DriverState.NextDriverState = NVMeStartComplete;
			__leave;
		}

	}
	__finally{
		if ((error == TRUE) && (NULL != pNVMeSrbExt->pDataBuffer) && (pAE->ntldrDump == FALSE)) {
			StorPortFreePool((PVOID)pAE, pNVMeSrbExt->pDataBuffer);
			pNVMeSrbExt->pDataBuffer = NULL;
		}if ((pAE->DriverState.NextDriverState == NVMeStartComplete) ||
			(pAE->DriverState.NextDriverState == NVMeWaitOnNamespaceReady)){
			NVMeCallArbiter(pAE);
		}
	}
	return;
} /* NVMeRunningWaitOnNamespaceReady */


