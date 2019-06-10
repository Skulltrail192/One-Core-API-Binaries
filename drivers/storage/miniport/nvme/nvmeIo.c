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
 * File: nvmeIo.c
 */

#include "precomp.h"
#ifndef DBG
#include "nvmeIo.tmh"
#endif

#if DBG
BOOLEAN gResetTest = FALSE;
ULONG gResetCounter = 0;
ULONG gResetCount = 20000;
#endif
/*******************************************************************************
 * NVMeIssueCmd
 *
 * @brief NVMeIssueCmd can be called to issue one command at a time by ringing
 *        the specific doorbell register with the updated Submission Queue Tail
 *        Pointer. This routine copies the caller prepared submission entry data
 *        pointed by pTempSubEntry to next available submission entry of the
 *        specific queue before issuing the command.
 *
 * @param pAE - Pointer to hardware device extension.
 * @param QueueID - Which submission queue to issue the command
 * @param pTempSubEntry - The caller prepared Submission entry data
 *
 * @return ULONG
 *     STOR_STATUS_SUCCESS - If the command is issued successfully
 *     Otherwise - If anything goes wrong
 ******************************************************************************/
ULONG NVMeIssueCmd(
    PNVME_DEVICE_EXTENSION pAE,
    USHORT QueueID,
    PVOID pTempSubEntry
)
{
    PQUEUE_INFO pQI = &pAE->QueueInfo;
    PSUB_QUEUE_INFO pSQI = NULL;
    PNVMe_COMMAND pNVMeCmd = NULL;
    USHORT tempSqTail = 0;

    /* Make sure the parameters are valid */
    if (QueueID > pQI->NumSubIoQCreated || pTempSubEntry == NULL)
        return (STOR_STATUS_INVALID_PARAMETER);

    /*
     * Locate the current submission entry and
     * copy the fields from the temp buffer
     */
    pSQI = pQI->pSubQueueInfo + QueueID;

    /* First make sure FW is draining this SQ */
    tempSqTail = ((pSQI->SubQTailPtr + 1) == pSQI->SubQEntries)
        ? 0 : pSQI->SubQTailPtr + 1;

    if (tempSqTail == pSQI->SubQHeadPtr) {
#ifdef HISTORY
        TracePathSubmit(ISSUE_RETURN_BUSY, QueueID, ((PNVMe_COMMAND)pTempSubEntry)->NSID,
            ((PNVMe_COMMAND)pTempSubEntry)->CDW0, 0, 0, 0);
#endif
        return (STOR_STATUS_INSUFFICIENT_RESOURCES);
    }

    pNVMeCmd = (PNVMe_COMMAND)pSQI->pSubQStart;
    pNVMeCmd += pSQI->SubQTailPtr;

    StorPortCopyMemory((PVOID)pNVMeCmd, pTempSubEntry, sizeof(NVMe_COMMAND));

    /* Increase the tail pointer by 1 and reset it if needed */
    pSQI->SubQTailPtr = tempSqTail;

    /*
     * Track # of outstanding requests for this SQ
     */
    pSQI->Requests++;

#ifdef HISTORY
        TracePathSubmit(ISSUE, QueueID, ((PNVMe_COMMAND)pTempSubEntry)->NSID,
            ((PNVMe_COMMAND)pTempSubEntry)->CDW0, pSQI->SubQTailPtr, 0, 0);
#endif
    /* Now issue the command via Doorbell register */
    StorPortWriteRegisterUlong(pAE, pSQI->pSubTDBL, (ULONG)pSQI->SubQTailPtr);

#if DBG
    if (gResetTest && (gResetCounter++ > gResetCount)) {
        gResetCounter = 0;

        /* quick hack for testing internally driven resets */
        NVMeResetController(pAE, NULL);
        return STOR_STATUS_SUCCESS;
    }
#endif /* DBG */

    /*
     * This is for polling mode.  This code can
     * potentially be updated to implement polling for other situations
     * as well keeping in mind that it was originally implmented to account 
     * for prototype Chatham device that didn't have line INTs
     */
#if defined(ALL_POLLING)
    if (pAE->ResMapTbl.NumMsiMsgGranted == 0) {
        ULONG entryStatus = STOR_STATUS_UNSUCCESSFUL;
        PNVMe_COMPLETION_QUEUE_ENTRY pCplEntry = NULL;
        PNVME_SRB_EXTENSION pSrbExtension = NULL;
        PCPL_QUEUE_INFO pCQI = pQI->pCplQueueInfo + QueueID;
        PRES_MAPPING_TBL pRMT = &pAE->ResMapTbl;
        BOOLEAN learning;

        while (entryStatus != STOR_STATUS_SUCCESS) {
            entryStatus = NVMeGetCplEntry(pAE, pCQI, &pCplEntry);
            if (entryStatus == STOR_STATUS_SUCCESS) {

                NVMeCompleteCmd(pAE,
                    pCplEntry->DW2.SQID,
                    pCplEntry->DW2.SQHD,
                    pCplEntry->DW3.CID,
                    (PVOID)&pSrbExtension);

                if (pSrbExtension != NULL) {

                    pSrbExtension->pCplEntry = pCplEntry;
                    learning = ((pAE->LearningCores < pRMT->NumActiveCores) &&
                            (QueueID > 0)) ? TRUE : FALSE;
                    if (learning) {
                        pAE->LearningCores++;
                    }
                }

                if ((pSrbExtension->pNvmeCompletionRoutine == NULL) &&
                    (SntiMapCompletionStatus(pSrbExtension) == TRUE)) {
                    IO_StorPortNotification(RequestComplete,
                                            pAE,
                                            pSrbExtension->pSrb);

                } else if ((pSrbExtension->pNvmeCompletionRoutine(pAE,
                            (PVOID)pSrbExtension) == TRUE) &&
                            (pSrbExtension->pSrb != NULL)) {
                    IO_StorPortNotification(RequestComplete,
                                            pAE,
                                            pSrbExtension->pSrb);
                }
                StorPortWriteRegisterUlong(pAE,
                                           pCQI->pCplHDBL,
                                           (ULONG)pCQI->CplQHeadPtr);
             }
        }
    }
#endif /* POLLING */

    return STOR_STATUS_SUCCESS;
} /* NVMeIssueCmd */

/*******************************************************************************
 * ProcessIo
 *
 * @brief Routine for processing an I/O request (both internal and externa)
 *        and setting up all the necessary info. Then, calls NVMeIssueCmd to
 *        issue the command to the controller.
 *
 * @param AdapterExtension - pointer to device extension
 * @param SrbExtension - SRB extension for this command
 * @param QueueType - type of queue (admin or I/O)
 * @param AcquireLock - if the caller needs the StartIO lock acquired or not
 *
 * @return BOOLEAN
 *     TRUE - command was processed successfully
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN
ProcessIo(
    __in PNVME_DEVICE_EXTENSION pAdapterExtension,
    __in PNVME_SRB_EXTENSION pSrbExtension,
    __in NVME_QUEUE_TYPE QueueType,
    __in BOOLEAN AcquireLock
)
{
    PNVMe_COMMAND pNvmeCmd;
    ULONG StorStatus;
    IO_SUBMIT_STATUS IoStatus = SUBMITTED;
    PCMD_INFO pCmdInfo = NULL;
    PROCESSOR_NUMBER ProcNumber;
    USHORT SubQueue = 0;
    USHORT CplQueue = 0;
    STOR_LOCK_HANDLE hStartIoLock = {0};
    BOOLEAN completeStatus = FALSE;
#ifdef PRP_DBG
    PVOID pVa = NULL;
#endif

     __try {

        if (AcquireLock == TRUE) {
            StorPortAcquireSpinLock(pAdapterExtension,
                                    StartIoLock,
                                    NULL,
                                    &hStartIoLock);
        }

    if (pAdapterExtension->ntldrDump == FALSE) {
        StorStatus = StorPortPatchGetCurrentProcessorNumber((PVOID)pAdapterExtension,
                                                       &ProcNumber);
        if (StorStatus != STOR_STATUS_SUCCESS) {
            IoStatus = NOT_SUBMITTED;
            __leave;
        }
    } else {
        memset(&ProcNumber, 0, sizeof(PROCESSOR_NUMBER));
    }

#if DBG
        /* save off the submitting core info for debug CT learning purposes */
    pSrbExtension->procNum = ProcNumber;
#endif

    /* 1 - Select Queue based on CPU */
    if (QueueType == NVME_QUEUE_TYPE_IO) {

            StorStatus =  NVMeMapCore2Queue(pAdapterExtension,
                                         &ProcNumber,
                                         &SubQueue,
                                         &CplQueue);

            if (StorStatus != STOR_STATUS_SUCCESS) {
                IoStatus = NOT_SUBMITTED;
                __leave;
            }
    } else {
        /* It's an admin queue */
        SubQueue = CplQueue = 0;
    }

    /* 2 - Choose CID for the CMD_ENTRY */
    StorStatus = NVMeGetCmdEntry(pAdapterExtension,
                                 SubQueue,
                                 (PVOID)pSrbExtension,
                                 &pCmdInfo);

    if (StorStatus != STOR_STATUS_SUCCESS) {
            IoStatus = BUSY;
            __leave;
    }

    pNvmeCmd = &pSrbExtension->nvmeSqeUnit;
#pragma prefast(suppress:6011,"This pointer is not NULL")
    pNvmeCmd->CDW0.CID = (USHORT)pCmdInfo->CmdID;

#ifdef DUMB_DRIVER
    /*
     * For reads/writes, create PRP list in pre-allocated
     * space describing the dbl buff location... make sure that
     * we do not double buffer NVMe Flush commands.
     */
    if ((QueueType == NVME_QUEUE_TYPE_IO) &&
        (pNvmeCmd->CDW0.OPC != NVM_FLUSH)) {
        ULONG len = GET_DATA_LENGTH(pSrbExtension->pSrb);
        ULONG i = 1;
        PUINT64 pPrpList = (PUINT64)pCmdInfo->pDblPrpListVir;

        ASSERT(len <= DUMB_DRIVER_SZ);

        if (len <= (PAGE_SIZE * 2)) {
            pNvmeCmd->PRP1 = pCmdInfo->dblPhy.QuadPart;
            if (len > PAGE_SIZE) {
                pNvmeCmd->PRP2 = pCmdInfo->dblPhy.QuadPart + PAGE_SIZE;
            } else {
                pNvmeCmd->PRP2 = 0;
            }
        } else {
            pNvmeCmd->PRP1 = pCmdInfo->dblPhy.QuadPart;
            len -= PAGE_SIZE;
            pNvmeCmd->PRP2 = pCmdInfo->dblPrpListPhy.QuadPart;

            while (len > 0) {
                *pPrpList = pCmdInfo->dblPhy.QuadPart + (PAGE_SIZE * i);
                len -= PAGE_SIZE;
                pPrpList++;
                i++;
            }
        }
        /* Pre-allacted so this had better be true! */
        ASSERT(IS_SYS_PAGE_ALIGNED(pNvmeCmd->PRP1));
        ASSERT(IS_SYS_PAGE_ALIGNED(pNvmeCmd->PRP2));

        // Get Virtual address, only for read or write
        if (IS_CMD_DATA_IN(pNvmeCmd->CDW0.OPC) ||
            IS_CMD_DATA_OUT(pNvmeCmd->CDW0.OPC)) {
            /*
             * Save the dblBuff location, Srb databuff location and len
             * all in one handy location in the srb ext
             */
            StorStatus = StorPortGetSystemAddress(pAdapterExtension,
                                                  pSrbExtension->pSrb,
                                                  &pSrbExtension->pSrbDataVir);

            ASSERT(StorStatus == STOR_STATUS_SUCCESS);

            pSrbExtension->dataLen = GET_DATA_LENGTH(pSrbExtension->pSrb);
            pSrbExtension->pDblVir = pCmdInfo->pDblVir;

            /*
             * For a write, copy data to the dbl buff, read data will
             * be copied out in the ISR
             */
            if (IS_CMD_DATA_OUT(pNvmeCmd->CDW0.OPC)) {
                    StorPortCopyMemory(pSrbExtension->pDblVir,
                                   pSrbExtension->pSrbDataVir,
                                   pSrbExtension->dataLen);
            }
        }
    }
#else /* DUMB_DRIVER */
    /*
     * 3 - If a PRP list is used, copy the buildIO prepared list to the
     * preallocated memory location and update the entry not the pCmdInfo is a
     * stack var but contains a to the pre allocated mem which is what we're
     * updating.
     */
#ifndef PRP_DBG
    if (pSrbExtension->numberOfPrpEntries > 2) {
        pNvmeCmd->PRP2 = pCmdInfo->prpListPhyAddr.QuadPart;

        /*
         * Copy the PRP list pointed to by PRP2. Size of the copy is total num
         * of PRPs -1 because PRP1 is not in the PRP list pointed to by PRP2.
         */
            StorPortCopyMemory(
            (PVOID)pCmdInfo->pPRPList,
            (PVOID)&pSrbExtension->prpList[0],
            ((pSrbExtension->numberOfPrpEntries - 1) * sizeof(UINT64)));
    }
#else
        if (pSrbExtension->pSrb) {
            StorPortGetSystemAddress(pSrbExtension->pNvmeDevExt,
                              pSrbExtension->pSrb,
                              &pVa);
            StorPortDebugPrint(INFO,
                               "NVME: Process Cmd 0x%x VA 0x%x 0x%x SLBA 0x%x 0x%x for LEN 0x%x\n",
                               pNvmeCmd->CDW0.OPC,
                               (ULONGLONG)pVa >> 32, (ULONG)pVa,
                               pNvmeCmd->CDW11,
                               pNvmeCmd->CDW10,
                               GET_DATA_LENGTH(pSrbExtension->pSrb));
        }

        if (pSrbExtension->numberOfPrpEntries > 2) {
            ULONG i;
            pNvmeCmd->PRP2 = pCmdInfo->prpListPhyAddr.QuadPart;

            StorPortCopyMemory(
                (PVOID)pCmdInfo->pPRPList,
                               (PVOID)&pSrbExtension->prpList[0],
                ((pSrbExtension->numberOfPrpEntries - 1) * sizeof(UINT64)));

            StorPortDebugPrint(INFO,
                   "NVME: Process prp1 0x%x 0x%x prp2 0x%x 0x%x (list for 0x%x entries)\n",
                   pNvmeCmd->PRP1 >> 32, pNvmeCmd->PRP1,
                   pNvmeCmd->PRP2 >> 32, pNvmeCmd->PRP2,
                   (pSrbExtension->numberOfPrpEntries - 1));

            for (i=0;i<(pSrbExtension->numberOfPrpEntries - 1);i++) {
                StorPortDebugPrint(INFO,
                   "NVME: Process entry # 0x%x prp 0x%x 0x%x\n",
                   i,
                   pSrbExtension->prpList[i] >> 32, pSrbExtension->prpList[i]
                   );
            }
        } else if (pNvmeCmd->PRP1 != 0) {
                 StorPortDebugPrint(INFO,
                       "NVME: Process prp1 0x%x 0x%x prp2 0x%x 0x%x (no list)\n",
                   pNvmeCmd->PRP1 >> 32, pNvmeCmd->PRP1,
                   pNvmeCmd->PRP2 >> 32, pNvmeCmd->PRP2
                   );
        }
#endif /* PRP_DBG */
#endif /* DBL_BUFF */
#ifdef HISTORY
            TracePathSubmit(PRE_ISSUE, SubQueue, pNvmeCmd->NSID, pNvmeCmd->CDW0,
                pNvmeCmd->PRP1, pNvmeCmd->PRP2, pSrbExtension->numberOfPrpEntries);
#endif

    /* 4 - Issue the Command */
    StorStatus = NVMeIssueCmd(pAdapterExtension, SubQueue, pNvmeCmd);

    if (StorStatus != STOR_STATUS_SUCCESS) {
        completeStatus = NVMeCompleteCmd(pAdapterExtension,
                                         SubQueue,
                                         NO_SQ_HEAD_CHANGE,
                                         pNvmeCmd->CDW0.CID,
                                        (PVOID)pSrbExtension);

        if (completeStatus == FALSE) {
            IoStatus = NOT_SUBMITTED;
            __leave;
        }
        else {
            IoStatus = BUSY;
            __leave;
        }
            
    }

    /*
     * In crashdump we poll on admin command completions
     * in order to allow our init state machine to function.
     * We don't poll on IO commands as storport will poll
     * for us and call our ISR.
     */
    if ((pAdapterExtension->ntldrDump == TRUE) &&
        (QueueType == NVME_QUEUE_TYPE_ADMIN)   &&
        (pAdapterExtension->DriverState.NextDriverState != NVMeStateFailed) && 
        (pAdapterExtension->DriverState.NextDriverState != NVMeStartComplete) &&
        (StorStatus == STOR_STATUS_SUCCESS)) {
        ULONG pollCount = 0;

        while (FALSE == NVMeIsrMsix(pAdapterExtension, NVME_ADMIN_MSG_ID)) {
            NVMeCrashDelay(pAdapterExtension->DriverState.CheckbackInterval, TRUE);
            if (++pollCount > DUMP_POLL_CALLS) {
                /* a polled admin command timeout is considered fatal */
                pAdapterExtension->DriverState.DriverErrorStatus |=
                    (1 << FATAL_POLLED_ADMIN_CMD_FAILURE);
                pAdapterExtension->DriverState.NextDriverState = NVMeStateFailed;
                /*
                 * depending on whether the timer driven thread is dead or not
                 * this error may get loggged twice
                 */
                NVMeLogError(pAdapterExtension,
                    (ULONG)pAdapterExtension->DriverState.DriverErrorStatus);
                    IoStatus = NOT_SUBMITTED;
                    __leave;
                }
            }
        }

    } finally {

        if (AcquireLock == TRUE) {
            StorPortReleaseSpinLock(pAdapterExtension, &hStartIoLock);
        }

        if (IoStatus == BUSY) {
#ifdef HISTORY
            TracePathSubmit(GETCMD_RETURN_BUSY, SubQueue,
                ((PNVMe_COMMAND)(&pSrbExtension->nvmeSqeUnit))->NSID,
                ((PNVMe_COMMAND)(&pSrbExtension->nvmeSqeUnit))->CDW0,
                0, 0, 0);
#endif
             if (pSrbExtension->pSrb != NULL) {
                pSrbExtension->pSrb->SrbStatus = SRB_STATUS_BUSY;
                IO_StorPortNotification(RequestComplete,
                                        pAdapterExtension,
                                        pSrbExtension->pSrb);
            }
        }

		if (IoStatus == NOT_SUBMITTED) {
			if (pSrbExtension->pSrb != NULL) {
				pSrbExtension->pSrb->SrbStatus = SRB_STATUS_ERROR;
				IO_StorPortNotification(RequestComplete,
					pAdapterExtension,
					pSrbExtension->pSrb);
			}
		}
    }

    return (IoStatus == SUBMITTED) ? TRUE : FALSE;

} /* ProcessIo */

/*******************************************************************************
 * NVMeCompleteCmd
 *
 * @brief NVMeCompleteCmd gets called to recover the context saved in the
 *        associated CMD_ENTRY structure with the specificed CmdID. Normally
 *        this routine is called when the caller is about to complete the
 *        request and notify StorPort.
 *
 * @param pAE - Pointer to hardware device extension.
 * @param QueueID - Which submission queue to recover the context from
 * @param CmdID - The acquired CmdID used to de-reference the CMD_ENTRY
 * @param pContext - Caller prepared buffer to save the original context
 *
 * @return BOOLEAN
 ******************************************************************************/
BOOLEAN NVMeCompleteCmd(
    PNVME_DEVICE_EXTENSION pAE,
    USHORT QueueID,
    SHORT NewHead,
    USHORT CmdID,
    PVOID pContext
)
{
    PQUEUE_INFO pQI = &pAE->QueueInfo;
    PSUB_QUEUE_INFO pSQI = NULL;
    PCMD_ENTRY pCmdEntry = NULL;

    /* Make sure the parameters are valid */
    ASSERT((QueueID <= pQI->NumSubIoQCreated) && (pContext != NULL));

    /*
     * Identify the target submission queue/cmd entry
     * and update the head pointer for the SQ if needed
     */
    pSQI = pQI->pSubQueueInfo + QueueID;

    pCmdEntry = ((PCMD_ENTRY)pSQI->pCmdEntry) + CmdID;

    if (NewHead != NO_SQ_HEAD_CHANGE) {
        pSQI->SubQHeadPtr = NewHead;
    }

    /* Ensure the command entry had been acquired */
    ASSERT(pCmdEntry->Pending == TRUE);

    /*
     * Return the original context -- this is a pointer to srb extension
     * (sanity check first that it is not NULL)
     */
    ASSERT(pCmdEntry->Context != NULL);

    if ((pCmdEntry->Pending == FALSE) || (pCmdEntry->Context == NULL)) {
        /*
         * Something bad happened so reset the adapter and hope for the best
         */
        NVMeResetController(pAE, NULL);
        return FALSE;
    }

    *((ULONG_PTR *)pContext) = (ULONG_PTR)pCmdEntry->Context;

#ifdef DUMB_DRIVER
    /*
     * For non admin command read, need to copy from the dbl buff to the
     * SRB data buff
     */
    if ((QueueID > 0) &&
        IS_CMD_DATA_IN(((PNVME_SRB_EXTENSION)pCmdEntry->Context)->nvmeSqeUnit.CDW0.OPC)) {
        PNVME_SRB_EXTENSION pSrbExt = (PNVME_SRB_EXTENSION)pCmdEntry->Context;

        ASSERT(pSrbExt);

        StorPortCopyMemory(pSrbExt->pSrbDataVir,
                           pSrbExt->pDblVir,
                           pSrbExt->dataLen);
    }
#endif /* DUMB_DRIVER */

    /* Clear the fields of CMD_ENTRY before appending back to free list */
    pCmdEntry->Pending = FALSE;
    pCmdEntry->Context = 0;

    InsertTailList(&pSQI->FreeQList, &pCmdEntry->ListEntry);

    return TRUE;
} /* NVMeCompleteCmd */

/*******************************************************************************
 * NVMeDetectPendingCmds
 *
 * @brief NVMeDetectPendingCmds gets called to check for commands that may still
 *        be pending. Called when the caller is about to shutdown per S3 or S4.
 *
 * @param pAE - Pointer to hardware device extension.
 * @param completeCmd - determines if detected commands should be completed
 * @param SrbStatus - Srb Status value for the completing SRBs
 *
 * @return BOOLEAN
 *     TRUE if commands are detected that are still pending
 *     FALSE if no commands pending
 ******************************************************************************/
BOOLEAN NVMeDetectPendingCmds(
    PNVME_DEVICE_EXTENSION pAE,
    BOOLEAN completeCmd,
	UCHAR SrbStatus
)
{
    PQUEUE_INFO pQI = &pAE->QueueInfo;
    PSUB_QUEUE_INFO pSQI = NULL;
    PCMD_ENTRY pCmdEntry = NULL;
    USHORT CmdID;
    USHORT QueueID = 0;
    PNVME_SRB_EXTENSION pSrbExtension = NULL;
    BOOLEAN retValue = FALSE;
    PNVMe_COMMAND pNVMeCmd = NULL;

    /*
     * there is a 0xD1 BSOD on shutdown/restart *with verifier on*
     * something to do with QEMU and IA emulation.  Confirmed the
     * mem in question is safe (Q mem) and this doesn't happen with
     * real HW.  So, if you use QEMU and verifier, uncomment this
     * line
     */
    /* return FALSE; */

    /* Simply return FALSE when buffer had been freed */
    if (pQI->pSubQueueInfo == NULL)
        return retValue;

    /* Search all submission queues */
    for (QueueID = 0; QueueID <= pQI->NumSubIoQCreated; QueueID++) {
        pSQI = pQI->pSubQueueInfo + QueueID;

        for (CmdID = 0; CmdID < pSQI->SubQEntries; CmdID++) {
            pCmdEntry = ((PCMD_ENTRY)pSQI->pCmdEntry) + CmdID;
            if (pCmdEntry->Pending == TRUE) {
                pSrbExtension = (PNVME_SRB_EXTENSION)pCmdEntry->Context;

                /*
                 * Since pending is set, pSrbExtension should exist
                 */
		        ASSERT(pSrbExtension != NULL);
                        
		        pNVMeCmd = &pSrbExtension->nvmeSqeUnit;

		        /*
		         * Internal cmd need to be completed
		         */
		        if (pSrbExtension->pSrb == NULL) {
			        NVMeCompleteCmd(pAE,
                                        pSQI->SubQueueID,
                                        NO_SQ_HEAD_CHANGE,
                                        pNVMeCmd->CDW0.CID,
                                        (PVOID)&pSrbExtension);
			        continue;
		        }

#ifdef HISTORY
                TraceEvent(DETECTED_PENDING_CMD,
                    QueueID,
                    pNVMeCmd->CDW0.CID,
                    pNVMeCmd->CDW0.OPC,
                    pNVMeCmd->PRP1,
                    pNVMeCmd->PRP2,
                    pNVMeCmd->NSID);
#endif

#if DBG
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "NVMeDetectPendingCmds: cmdinfo cmd id 0x%x srbExt 0x%p srb 0x%p\n",
                    pCmdEntry->CmdInfo.CmdID, pSrbExtension, pSrbExtension->pSrb);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme queue 0x%x OPC 0x%x\n",
                    QueueID, pNVMeCmd->CDW0.OPC);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme cmd id 0x%x\n",
                    pNVMeCmd->CDW0.CID);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme nsid 0x%x\n",
                    pNVMeCmd->NSID);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme prp1 0x%x 0x%x\n",
                    pNVMeCmd->PRP1 >> 32,
                    pNVMeCmd->PRP1);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme prp2 0x%x 0x%x\n",
                    pNVMeCmd->PRP2 >> 32,
                    pNVMeCmd->PRP2);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme CDW10 0x%x\n",
                    pNVMeCmd->CDW10);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme CDW11 0x%x\n",
                    pNVMeCmd->CDW11);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme CDW12 0x%x\n",
                    pNVMeCmd->CDW12);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme CDW13 0x%x\n",
                    pNVMeCmd->CDW13);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme CDW14 0x%x\n",
                    pNVMeCmd->CDW14);
                DbgPrintEx(DPFLTR_STORMINIPORT_ID,
                    DPFLTR_ERROR_LEVEL,
                    "\tnvme CDW15 0x%x\n",
                    pNVMeCmd->CDW15);
#endif

                /* don't count AER as an outstanding cmd */
                if ((pNVMeCmd->CDW0.OPC != ADMIN_ASYNCHRONOUS_EVENT_REQUEST) &&
                    (pSrbExtension->pSrb != NULL)) {
                    retValue = TRUE;
                }

                /* if requested, complete the command now */
                if (completeCmd == TRUE) {

                    NVMeCompleteCmd(pAE,
                                    pSQI->SubQueueID,
                                    NO_SQ_HEAD_CHANGE,
                                    pNVMeCmd->CDW0.CID,
                                    (PVOID)&pSrbExtension);

                    if (pSrbExtension->pSrb != NULL) {
#ifdef HISTORY
                        NVMe_COMPLETION_QUEUE_ENTRY_DWORD_3 nullEntry = {0};
                        TracePathComplete(COMPLETE_CMD_RESET,
                            pSQI->SubQueueID,
                            pNVMeCmd->CDW0.CID, 0, nullEntry,
                            (ULONGLONG)pSrbExtension->pNvmeCompletionRoutine,
                            0);
#endif
						pSrbExtension->pSrb->SrbStatus = SrbStatus;
                        IO_StorPortNotification(RequestComplete,
                                                pAE,
                                                pSrbExtension->pSrb);
                    } /* has an Srb */
                } /* complete the command? */
            } /* if cmd is pending */
        } /* for cmds on the SQ */
    } /* for the SQ */

    return retValue;
} /* NVMeDetectPendingCmds */
