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
 * File: nvmeSnti.c
 */

#include "precomp.h"
#ifndef DBG
#include "nvmeSnti.tmh"
#endif

HW_INITIALIZATION_DATA gHwInitializationData;

/* NVMe Vendor String */
const char vendorIdString[] = "NVME    ";

/* Current Mode Parameter Block Descriptor Values */
MODE_PARAMETER_BLOCK g_modeParamBlock;

/* Generic Command Status Lookup Table */
SNTI_RESPONSE_BLOCK genericCommandStatusTable[] = {
    /* SUCCESSFUL_COMPLETION - 0x0 */
    {SRB_STATUS_SUCCESS,
        SCSISTAT_GOOD,
        SCSI_SENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE},

    /* INVALID_COMMAND_OPCODE - 0x1 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_ILLEGAL_REQUEST,
        SCSI_ADSENSE_ILLEGAL_COMMAND,
        SCSI_ADSENSE_NO_SENSE},

    /* INVALID_FIELD_IN_COMMAND - 0x2 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_ILLEGAL_REQUEST,
        SCSI_ADSENSE_INVALID_CDB,
        SCSI_ADSENSE_NO_SENSE},

    /* COMMAND_ID_CONFLICT - 0x3 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_ILLEGAL_REQUEST,
        SCSI_ADSENSE_INVALID_CDB,
        SCSI_ADSENSE_NO_SENSE},

    /* DATA_TRANSFER_ERROR - 0x4 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_MEDIUM_ERROR,
        SCSI_ADSENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE},

    /* COMMANDS_ABORTED_DUE_TO_POWER_LOSS_NOTIFICATION - 0x5 */
    {SRB_STATUS_ABORTED,
        SCSISTAT_TASK_ABORTED,
        SCSI_SENSE_ABORTED_COMMAND,
        SCSI_ADSENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE},

    /* INTERNAL_DEVICE_ERROR - 0x6 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_HARDWARE_ERROR,
        SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
        SCSI_ADSENSE_NO_SENSE},

    /* COMMAND_ABORT_REQUESTED - 0x7 */
    {SRB_STATUS_ABORTED,
        SCSISTAT_TASK_ABORTED,
        SCSI_SENSE_ABORTED_COMMAND,
        SCSI_ADSENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE},

    /* COMMAND_ABORTED_DUE_TO_SQ_DELETION - 0x8 */
    {SRB_STATUS_ABORTED,
        SCSISTAT_TASK_ABORTED,
        SCSI_SENSE_ABORTED_COMMAND,
        SCSI_ADSENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE},

    /* COMMAND_ABORTED_DUE_TO_FAILED_FUSED_COMMAND - 0x9 */
    {SRB_STATUS_ABORTED,
        SCSISTAT_TASK_ABORTED,
        SCSI_SENSE_ABORTED_COMMAND,
        SCSI_ADSENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE},

    /* COMMAND_ABORTED_DUE_TO_MISSING_FUSED_COMMAND - 0xA */
    {SRB_STATUS_ABORTED,
        SCSISTAT_TASK_ABORTED,
        SCSI_SENSE_ABORTED_COMMAND,
        SCSI_ADSENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE},

    /* INVALID_NAMESPACE_OR_FORMAT - 0xB */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_ILLEGAL_REQUEST,
        SCSI_ADSENSE_ILLEGAL_COMMAND,
        SCSI_SENSEQ_INVALID_LUN_ID},

    /* Generic Command Status (NVM Command Set) Lookup Table */

    /* LBA_OUT_OF_RANGE - 0x80 (0xC) */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_ILLEGAL_REQUEST,
        SCSI_ADSENSE_ILLEGAL_BLOCK,
        SCSI_ADSENSE_NO_SENSE},

    /* CAPACITY_EXCEEDED - 0x81 (0xD) */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_MEDIUM_ERROR,
        SCSI_ADSENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE},

    /* NAMESPACE_NOT_READY - 0x82 (0xE) */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_NOT_READY,
        SCSI_ADSENSE_LUN_NOT_READY,
        SCSI_SENSEQ_BECOMING_READY}
};

/* Command Specific Status Lookup Table */
SNTI_RESPONSE_BLOCK commandSpecificStatusTable[] = {
    /* COMPLETION_QUEUE_INVALID - 0x0 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_ILLEGAL_REQUEST,
        SCSI_ADSENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE},

    /* INVALID_QUEUE_IDENTIFIER - 0x1 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_UNIT_ATTENTION,
        SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
        SCSI_ADSENSE_NO_SENSE},

    /* MAXIMUM_QUEUE_SIZE_EXCEEDED - 0x2 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_UNIT_ATTENTION,
        SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
        SCSI_ADSENSE_NO_SENSE},

    /* ABORT_COMMAND_LIMIT_EXCEEDED - 0x3 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_ILLEGAL_REQUEST,
        SCSI_ADSENSE_NO_SENSE,
        SCSI_ADSENSE_NO_SENSE},

    /* RESERVED (REQUESTED_COMMAND_TO_ABORT_NOT_FOUND) - 0x4 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_UNIT_ATTENTION,
        SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
        SCSI_ADSENSE_NO_SENSE},

    /* ASYNCHRONOUS_EVENT_REQUEST_LIMIT_EXCEEDED - 0x5 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_UNIT_ATTENTION,
        SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
        SCSI_ADSENSE_NO_SENSE},

    /* INVALID_FIRMWARE_SLOT - 0x6 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_UNIT_ATTENTION,
        SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
        SCSI_ADSENSE_NO_SENSE},

    /* INVALID_FIRMWARE_IMAGE - 0x7 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_UNIT_ATTENTION,
        SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
        SCSI_ADSENSE_NO_SENSE},

    /* INVALID_INTERRUPT_VECTOR - 0x8 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_UNIT_ATTENTION,
        SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
        SCSI_ADSENSE_NO_SENSE},

    /* INVALID_LOG_PAGE - 0x9 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_UNIT_ATTENTION,
        SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
        SCSI_ADSENSE_NO_SENSE},

    /* INVALID_FORMAT - 0xA */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_ILLEGAL_REQUEST,
        SCSI_ADSENSE_FORMAT_COMMAND_FAILED,
        SCSI_SENSEQ_FORMAT_COMMAND_FAILED},

    /* Command Specific Status (NVM Command Set) Lookup Table */

    /* CONFLICTING_ATTRIBUTES - 0x80 (0xB) */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_ILLEGAL_REQUEST,
        SCSI_ADSENSE_INVALID_CDB,
        SCSI_ADSENSE_NO_SENSE},

    /* RESERVED  -0x81 (0xC) */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_UNIT_ATTENTION,
        SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
        SCSI_ADSENSE_NO_SENSE},

    /* ATTEMPTED WRITE TO READ ONLY RANGE - 0x82 (0xD) */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_DATA_PROTECT,
        SCSI_ADSENSE_WRITE_PROTECT,
        SCSI_ADSENSE_NO_SENSE}
};

#define SCSI_ADSENSE_ACCESS_DENIED 0x20
#define SCSI_SENSEQ_NO_ACCESS_RIGHTS 0x02

/* Media Error Lookup Table */
SNTI_RESPONSE_BLOCK mediaErrorTable[] = {
    /* WRITE_FAULT - 0x80 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_MEDIUM_ERROR,
        SCSI_ADSENSE_PERIPHERAL_DEV_WRITE_FAULT,
        SCSI_ADSENSE_NO_SENSE},

    /* UNRECOVERED_READ_ERROR - 0x81 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_MEDIUM_ERROR,
        SCSI_ADSENSE_UNRECOVERED_READ_ERROR,
        SCSI_ADSENSE_NO_SENSE},

    /* END_TO_END_GUARD_CHECK_ERROR - 0x82 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_MEDIUM_ERROR,
        SCSI_ADSENSE_LOG_BLOCK_GUARD_CHECK_FAILED,
        SCSI_SENSEQ_LOG_BLOCK_GUARD_CHECK_FAILED},

    /* END_TO_END_APPLICATION_TAG_CHECK_ERROR - 0x83 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_MEDIUM_ERROR,
        SCSI_ADSENSE_LOG_BLOCK_APPTAG_CHECK_FAILED,
        SCSI_SENSEQ_LOG_BLOCK_APPTAG_CHECK_FAILED},

    /* END_TO_END_REFERENCE_TAG_CHECK_ERROR - 0x84 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_MEDIUM_ERROR,
        SCSI_ADSENSE_LOG_BLOCK_REFTAG_CHECK_FAILED,
        SCSI_SENSEQ_LOG_BLOCK_REFTAG_CHECK_FAILED},

    /* COMPARE_FAILURE - 0x85 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_MISCOMPARE,
        SCSI_ADSENSE_MISCOMPARE_DURING_VERIFY,
        SCSI_ADSENSE_NO_SENSE},

    /* ACCESS_DENIED - 0x86 */
    {SRB_STATUS_ERROR,
        SCSISTAT_CHECK_CONDITION,
        SCSI_SENSE_DATA_PROTECT,
        SCSI_ADSENSE_ACCESS_DENIED,
        SCSI_SENSEQ_NO_ACCESS_RIGHTS}
};

/******************************************************************************
 * SntiTranslateCommand
 *
 * @brief This method parses the supplied SCSI request and then invokes the SCSI
 *        to NVMe translation function for that particular request. Unsupported
 *        SCSI commands in the open source NVMe driver:
 *
 *        - COMPARE AND WRITE (NVME_COMPARE is required for translation and this
 *          command will not be supported in the Windows open source driver)
 *
 *        - WRITE LONG 10/16 (NVME_WRITE_UNCORRECTABLE is required for
 *          translation and this command will not be supported in the Windows
 *          open source driver)
 *
 *        - UNMAP (NVME_DATASET_MANAGEMENT is required for translation and this
 *          command was not initially supported in Windows open source driver, but
 *          now when compiled for Windows 8, the support is provided)
 *
 *
 * @param pAdapterExtension - pointer to the adapter device extension
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateCommand(
    PNVME_DEVICE_EXTENSION pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_SUCCESS;
    BOOLEAN supportsVwc = pAdapterExtension->controllerIdentifyData.VWC.Present;
#if (NTDDI_VERSION > NTDDI_WIN7)
    UCHAR scsiStatus = SCSISTAT_GOOD;

    /* Sanity Check if SrbGetCdb returns NULL pointer. */ 
    PCDB pCdb = SrbGetCdb((void*)pSrb);
    if (pCdb == NULL) {
        StorPortDebugPrint(INFO, "SNTI: CDB pointer is NULL!\n");

        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        returnStatus = SNTI_UNSUPPORTED_SCSI_REQUEST;
        return returnStatus;
    }
#endif
    /* DEBUG ONLY - Turn off for main I/O */
#if DBG
    StorPortDebugPrint(INFO,
                       "SNTI: Translating opcode - 0x%02x BTL (%d %d %d)\n",
                       GET_OPCODE(pSrb),
#if (NTDDI_VERSION > NTDDI_WIN7)
                       SrbGetPathId((void*)pSrb),
                       SrbGetTargetId((void*)pSrb),
                       SrbGetLun((void*)pSrb));
#else
                       pSrb->PathId,
                       pSrb->TargetId,
                       pSrb->Lun);
#endif
#endif /* DBG */


    returnStatus = SntiValidateNacaSetting(pAdapterExtension, pSrb);
    if (returnStatus != SNTI_SUCCESS) {
        return returnStatus;
    }

    switch (GET_OPCODE(pSrb)) {
        case SCSIOP_READ6:
        case SCSIOP_READ:
        case SCSIOP_READ12:
        case SCSIOP_READ16:
            returnStatus = SntiTranslateRead(pSrb);
            if (returnStatus == SNTI_COMMAND_COMPLETED) {
                pSrb->SrbStatus = SRB_STATUS_SUCCESS;
#if (NTDDI_VERSION > NTDDI_WIN7)
                scsiStatus = SCSISTAT_GOOD;
                SrbSetScsiData(pSrb, NULL, NULL, &scsiStatus, NULL, NULL);
#else
                pSrb->ScsiStatus = SCSISTAT_GOOD;
#endif
                SET_DATA_LENGTH(pSrb, 0);
            }
        break;
        case SCSIOP_WRITE6:
        case SCSIOP_WRITE:
        case SCSIOP_WRITE12:
        case SCSIOP_WRITE16:
            returnStatus = SntiTranslateWrite(pSrb);
            if (returnStatus == SNTI_COMMAND_COMPLETED) {
                pSrb->SrbStatus = SRB_STATUS_SUCCESS;
#if (NTDDI_VERSION > NTDDI_WIN7)
                scsiStatus = SCSISTAT_GOOD;
                SrbSetScsiData(pSrb, NULL, NULL, &scsiStatus, NULL, NULL);
#else
                pSrb->ScsiStatus = SCSISTAT_GOOD;
#endif
                SET_DATA_LENGTH(pSrb, 0);
            }
        break;
        case SCSIOP_INQUIRY:
            returnStatus = SntiTranslateInquiry(pSrb);
            /* set the maximum queue depth per LUN */
            if (pSrb->SrbStatus == SRB_STATUS_SUCCESS) {
                StorPortSetDeviceQueueDepth(pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
                            SrbGetPathId((void*)pSrb),
                            SrbGetTargetId((void*)pSrb),
                            SrbGetLun((void*)pSrb),
#else
                            pSrb->PathId,
                            pSrb->TargetId,
                            pSrb->Lun,
#endif
                            SNTI_STORPORT_QUEUE_DEPTH
                            );
            }
        break;
        case SCSIOP_LOG_SENSE:
            returnStatus = SntiTranslateLogSense(pSrb);
        break;
        case SCSIOP_MODE_SELECT:
        case SCSIOP_MODE_SELECT10:
            returnStatus = SntiTranslateModeSelect(pSrb, supportsVwc);
        break;
        case SCSIOP_MODE_SENSE:
        case SCSIOP_MODE_SENSE10:
            returnStatus = SntiTranslateModeSense(pSrb, supportsVwc);
        break;
        case SCSIOP_READ_CAPACITY:
        case SCSIOP_READ_CAPACITY16:
            returnStatus = SntiTranslateReadCapacity(pSrb);
        break;
        case SCSIOP_REPORT_LUNS:
            returnStatus = SntiTranslateReportLuns(pSrb);
        break;

        case SCSIOP_PERSISTENT_RESERVE_IN:
        case SCSIOP_PERSISTENT_RESERVE_OUT:
            if (pAdapterExtension->controllerIdentifyData.ONCS.SupportsReservations == TRUE) {
                if (GET_OPCODE(pSrb) == SCSIOP_PERSISTENT_RESERVE_IN) {
                    returnStatus = SntiTranslatePersistentReserveIn(pSrb);
                } else {
                    returnStatus = SntiTranslatePersistentReserveOut(pSrb);
                }
            } else {
                StorPortDebugPrint(INFO,
                    "SntiTranslateCommand. SCSI Persistent Reserve In/Out unsupported - 0x%02x\n",
                    GET_OPCODE(pSrb));

                SntiSetScsiSenseData(pSrb,
                    SCSISTAT_CHECK_CONDITION,
                    SCSI_SENSE_ILLEGAL_REQUEST,
                    SCSI_ADSENSE_ILLEGAL_COMMAND,
                    SCSI_ADSENSE_NO_SENSE);

                pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
                SET_DATA_LENGTH(pSrb, 0);

                returnStatus = SNTI_UNSUPPORTED_SCSI_REQUEST;
            }
        break;

        case SCSIOP_REQUEST_SENSE:
            returnStatus = SntiTranslateRequestSense(pSrb);
        break;
        case SCSIOP_SECURITY_PROTOCOL_IN:
        case SCSIOP_SECURITY_PROTOCOL_OUT:
            /* Need support added QEMU for NVME SECURITY SEND/RECEIVE */
            /* returnStatus = SntiTranslateSecurityProtocol(pSrb); */
            StorPortDebugPrint(INFO,
                "SNTI: SCSIOP_SECURITY_PROTOCOL_IN_OUT unsupported - 0x%02x\n",
                GET_OPCODE(pSrb));

            SntiSetScsiSenseData(pSrb,
                                 SCSISTAT_CHECK_CONDITION,
                                 SCSI_SENSE_ILLEGAL_REQUEST,
                                 SCSI_ADSENSE_ILLEGAL_COMMAND,
                                 SCSI_ADSENSE_NO_SENSE);

            pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
            SET_DATA_LENGTH(pSrb, 0);

            returnStatus = SNTI_UNSUPPORTED_SCSI_REQUEST;
        break;
        case SCSIOP_START_STOP_UNIT:
            returnStatus = SntiTranslateStartStopUnit(pSrb);
        break;
        case SCSIOP_SYNCHRONIZE_CACHE:
        case SCSIOP_SYNCHRONIZE_CACHE16:
            if (supportsVwc == TRUE) {
            returnStatus = SntiTranslateSynchronizeCache(pSrb);
            } else {
                pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                SET_DATA_LENGTH(pSrb, 0);
                returnStatus = SNTI_COMMAND_COMPLETED;
            }
        break;
        case SCSIOP_FORMAT_UNIT:
            /* Will never get this request from OS */
            /* returnStatus = SntiTranslateFormatUnit(pSrb); */
            StorPortDebugPrint(INFO,
                "SNTI: SCSIOP_FORMAT_UNIT unsupported - 0x%02x\n",
                GET_OPCODE(pSrb));

            SntiSetScsiSenseData(pSrb,
                                 SCSISTAT_CHECK_CONDITION,
                                 SCSI_SENSE_ILLEGAL_REQUEST,
                                 SCSI_ADSENSE_ILLEGAL_COMMAND,
                                 SCSI_ADSENSE_NO_SENSE);

            pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
            SET_DATA_LENGTH(pSrb, 0);

            returnStatus = SNTI_UNSUPPORTED_SCSI_REQUEST;
        break;
        case SCSIOP_TEST_UNIT_READY:
            returnStatus = SntiTranslateTestUnitReady(pSrb);
        break;
        case SCSIOP_WRITE_DATA_BUFF:
            returnStatus = SntiTranslateWriteBuffer(pSrb);
        break;

        /*UNMAP not supported prior to Win 8*/
#if (NTDDI_VERSION > NTDDI_WIN7)
        case SCSIOP_UNMAP:
            returnStatus = SntiTranslateUnmap(pSrb);
        break;
#endif

        default:
            StorPortDebugPrint(INFO,
                               "SNTI: UNSUPPORTED opcode - 0x%02x\n",
                               GET_OPCODE(pSrb));

            SntiSetScsiSenseData(pSrb,
                                 SCSISTAT_CHECK_CONDITION,
                                 SCSI_SENSE_ILLEGAL_REQUEST,
                                 SCSI_ADSENSE_ILLEGAL_COMMAND,
                                 SCSI_ADSENSE_NO_SENSE);

            pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
            SET_DATA_LENGTH(pSrb, 0);

            returnStatus = SNTI_UNSUPPORTED_SCSI_REQUEST;
        break;
   } /* end switch */

    return returnStatus;
} /* SntiTranslateCommand */

/******************************************************************************
 * SntiTranslateInquiry
 *
 * @brief Translates the SCSI Inquiry command and populate the appropriate SCSI
 *        Inquiry fields based on the NVMe Translation spec. Do not need to
 *        create an SQE here as we just complete the command in the build phase
 *        (by returning FALSE to StorPort with SRB status of SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateInquiry(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    UINT8 pageCode;
    BOOLEAN evpd;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    SNTI_STATUS status;

    /* Default the translation status to command completed */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;

    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    StorPortDebugPrint(INFO, "SntiTranslateInquiry: called.\n");

    evpd = GET_INQ_EVPD_BIT(pSrb);
    pageCode = GET_INQ_PAGE_CODE(pSrb);

    ASSERT(pSrbExt != NULL);

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        SntiMapInternalErrorStatus(pSrb, status);
        return SNTI_FAILURE_CHECK_RESPONSE_DATA;
    }

    /*
     * Set SRB status to success to indicate the command will complete
     * successfully (assuming no errors occur during translation) and reset the
     * status value to use below.
     */
    pSrb->SrbStatus = SRB_STATUS_SUCCESS;

    /* Set the completion routine - no translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    /* EVPD = 0: Standard Inquiry Page; EVPD = 1: VPD Inquiry Page */
    if (evpd != 0) {
        switch (pageCode) {
            case VPD_SUPPORTED_PAGES:
                SntiTranslateSupportedVpdPages(pSrb);
            break;
            case VPD_SERIAL_NUMBER:
                SntiTranslateUnitSerialPage(pSrb, pLunExt);
            break;
            case VPD_DEVICE_IDENTIFIERS:
                SntiTranslateDeviceIdentificationPage(pSrb);
            break;

            /* 
              UNMAP and related VPD pages not supported prior to Win 8 
              (Note: VPD_BLOCK_LIMITS, etc. not defined in Win 7 WDK)
            */
#if (NTDDI_VERSION > NTDDI_WIN7)
            case VPD_BLOCK_LIMITS:
                SntiTranslateBlockLimitsPage(pSrb);
            break;
            case VPD_BLOCK_DEVICE_CHARACTERISTICS:
                SntiTranslateBlockDeviceCharacteristicsPage(pSrb);
            break;
            case VPD_LOGICAL_BLOCK_PROVISIONING:
                SntiTranslateLogicalBlockProvisioningPage(pSrb, pLunExt);
            break;
#endif

            default:
                SntiSetScsiSenseData(pSrb,
                                     SCSISTAT_CHECK_CONDITION,
                                     SCSI_SENSE_ILLEGAL_REQUEST,
                                     SCSI_ADSENSE_INVALID_CDB,
                                     SCSI_ADSENSE_NO_SENSE);

                pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
                SET_DATA_LENGTH(pSrb, 0);
                returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
            break;
        } /* end switch */
    } else {
        /* For Standard Inquiry, the page code must be 0 */
        if (pageCode == INQ_STANDARD_INQUIRY_PAGE) {
            SntiTranslateStandardInquiryPage(pSrb);
        } else {
            /* Ensure correct sense data for SCSI compliance test case 1.4 */
           SntiSetScsiSenseData(pSrb,
                                SCSISTAT_CHECK_CONDITION,
                                SCSI_SENSE_ILLEGAL_REQUEST,
                                SCSI_ADSENSE_INVALID_CDB,
                                SCSI_ADSENSE_NO_SENSE);

           pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
           SET_DATA_LENGTH(pSrb, 0);
           returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
        }
    }

    /*
     * For Inquiry commands, we shall always used cached data from the HW and
     * therefore shall always return SNTI_COMMAND_COMPLETED unless and error
     * occurred during the translation phase.
     */
    return returnStatus;
} /* SntiTranslateInquiry */

/******************************************************************************
 * SntiTranslateSupportedVpdPages
 *
 * @brief Translates the SCSI Inquiry VPD page - Supported VPD Pages. Populates
 *        the appropriate SCSI Inqiry response fields based on the NVMe
 *        Translation spec. Do not need to create SQE here as we just complete
 *        the command in the build phase (by returning FALSE to StorPort with
 *        SRB status of SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateSupportedVpdPages(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PVPD_SUPPORTED_PAGES_PAGE pSupportedVpdPages = NULL;
    UINT16 allocLength;
#define VPD_SUPPORTED_PAGES_PAGE_LENGTH (FIELD_OFFSET(VPD_SUPPORTED_PAGES_PAGE, SupportedPageList) + INQ_NUM_SUPPORTED_VPD_PAGES)
    UCHAR tmpSupportedVpdPages[VPD_SUPPORTED_PAGES_PAGE_LENGTH];

    pSupportedVpdPages = (PVPD_SUPPORTED_PAGES_PAGE)GET_DATA_BUFFER(pSrb);
    allocLength = GET_INQ_ALLOC_LENGTH(pSrb);
    if (allocLength < VPD_SUPPORTED_PAGES_PAGE_LENGTH) {
        pSupportedVpdPages = (PVPD_SUPPORTED_PAGES_PAGE)tmpSupportedVpdPages;
    }

    if (allocLength > 0) {
        memset(pSupportedVpdPages, 0, allocLength);
        pSupportedVpdPages->DeviceType                  = DIRECT_ACCESS_DEVICE;
        pSupportedVpdPages->DeviceTypeQualifier         = DEVICE_CONNECTED;
        pSupportedVpdPages->PageCode                    = VPD_SUPPORTED_PAGES;
        pSupportedVpdPages->Reserved                    = INQ_RESERVED;
        pSupportedVpdPages->PageLength                  = INQ_NUM_SUPPORTED_VPD_PAGES;
        pSupportedVpdPages->SupportedPageList[BYTE_0]   = VPD_SUPPORTED_PAGES;
        pSupportedVpdPages->SupportedPageList[BYTE_1]   = VPD_SERIAL_NUMBER;
        pSupportedVpdPages->SupportedPageList[BYTE_2]   = VPD_DEVICE_IDENTIFIERS;

        /* 
          For Windows 8 Unmap support, we supply these additional VPD pages 
          (Note: #defines for VPD_BLOCK_LIMITS etc. are not defined in Win 7 WDK)
        */
#if (NTDDI_VERSION > NTDDI_WIN7)
        pSupportedVpdPages->SupportedPageList[BYTE_3]   = VPD_BLOCK_LIMITS;
        pSupportedVpdPages->SupportedPageList[BYTE_4]   = VPD_BLOCK_DEVICE_CHARACTERISTICS;
        pSupportedVpdPages->SupportedPageList[BYTE_5]   = VPD_LOGICAL_BLOCK_PROVISIONING;
#endif 
    }
    if (allocLength > 0 && allocLength < VPD_SUPPORTED_PAGES_PAGE_LENGTH) {
        StorPortCopyMemory(GET_DATA_BUFFER(pSrb), pSupportedVpdPages, allocLength);
    }

    SET_DATA_LENGTH(pSrb, min(allocLength, VPD_SUPPORTED_PAGES_PAGE_LENGTH));
} /* SntiTranslateSupportedVpdPages */

/******************************************************************************
 * SntiTranslateUnitSerialPage
 *
 * @brief Translates the SCSI Inquiry VPD page - Unit Serial Number Page.
 *        Populates the appropriate SCSI Inqiry response fields based on the
 *        NVMe Translation spec. Do not need to create SQE here as we just
 *        complete the command in the build phase (by returning FALSE to
 *        StorPort with SRB status of SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateUnitSerialPage(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_LUN_EXTENSION pLunExt
)
{
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PVPD_SERIAL_NUMBER_PAGE pSerialNumberPage = NULL;

    // Depending on the NVMe EUI64, NVMe NGUID fields or the NVMe standard implemented,
    // This inquiry page will be constructed differently. 
    // A local buffer is allocated here to build the inquiry data. At the end of the method
    // this buffer will be copied to the caller's buffer. This local buffer
    // is allocated as the largest possible required size. (The maximum size occurs
    // when the NGUID is used to build the Serial number). The actual 
    // size is calculated as the buffer is filled in.
#define VPD_SERIAL_NUMBER_PAGE_SIZE (FIELD_OFFSET(VPD_SERIAL_NUMBER_PAGE, SerialNumber) + INQ_SN_FROM_NGUID_LENGTH)
    UCHAR tmpSerialNumberPage[VPD_SERIAL_NUMBER_PAGE_SIZE] = { 0 };
    UINT16 totalDataSize = 0;
    UINT16 allocLength = 0;
    UINT16 copyLength = 0;
    PUCHAR dstSN = NULL;
    USHORT count = 0;

    ULONGLONG tempLLong = 0;
    PNVME_SRB_EXTENSION  pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pDevExt = pSrbExt->pNvmeDevExt;
    allocLength = GET_INQ_ALLOC_LENGTH(pSrb);

    // Always build the page in a local buffer
    pSerialNumberPage = (PVPD_SERIAL_NUMBER_PAGE)tmpSerialNumberPage;

    // Get a pointer to the SN portion of the page
    dstSN = pSerialNumberPage->SerialNumber;

    if (allocLength > 0) {
        pSerialNumberPage->DeviceType = DIRECT_ACCESS_DEVICE;
        pSerialNumberPage->DeviceTypeQualifier = DEVICE_CONNECTED;
        pSerialNumberPage->PageCode = VPD_SERIAL_NUMBER;
        pSerialNumberPage->Reserved = INQ_RESERVED;

        // If there is an NGUID, use that to create the SN. 
        if ((pLunExt->identifyData.NGUID.LowerBytes != 0) ||
            (pLunExt->identifyData.NGUID.UpperBytes != 0)) {
            /*
            * The serial number should be in the following format
            *  0x0123456789ABCDEF0123456789ABCDEF would be converted to
            * “0123_4567_89AB_CDEF_0123_4567_89AB_CDEF.”
            */

            // Convert the NGUID upper bytes to little endian 
            REVERSE_BYTES_QUAD(&tempLLong, &pLunExt->identifyData.NGUID.UpperBytes);

            /* Convert the upper byte nibbles to ASCII */
            count = SntiConvertULLongToA(
                dstSN,
                tempLLong,
                NIBBLES_PER_LONGLONG,
                TRUE,
                FALSE
                );

            // Get a copy of the lower bytes of the NGUID
            REVERSE_BYTES_QUAD(&tempLLong, &pLunExt->identifyData.NGUID.LowerBytes);

            /* Convert the lower byte nibbles to ASCII */
            SntiConvertULLongToA(
                dstSN + count,
                tempLLong,
                NIBBLES_PER_LONGLONG,
                TRUE,
                TRUE
                );


            pSerialNumberPage->PageLength = INQ_SN_FROM_NGUID_LENGTH;
        } else if (*((PULONGLONG)pLunExt->identifyData.EUI64) != 0) {
            // If there is no NGUID, check to see if there is a valid EUI64. If
            // so, use it to create the SN.

            /*
             * The serial number should be in the following format in order to pass
             * SCSI Compliance test:
             * "0123_4567_89AB_CDEF." for a number like "0123456789ABCDEF".
             */
            // Convert the EUI64 to little endian
            REVERSE_BYTES_QUAD(&tempLLong, pLunExt->identifyData.EUI64);
            SntiConvertULLongToA(
                dstSN,
                tempLLong,
                NIBBLES_PER_LONGLONG,
                TRUE,
                TRUE
                );

            pSerialNumberPage->PageLength = INQ_SN_FROM_EUI64_LENGTH;
        } else {
            // If there is no NGUID or EUI64, use the Identify Controller SN
            // (only valid for NVMe 1.0 devices)

            // First part of the SN comes from the Identify Controller SN
            memcpy(dstSN, pDevExt->controllerIdentifyData.SN, sizeof(pDevExt->controllerIdentifyData.SN));
            dstSN = &pSerialNumberPage->SerialNumber[20];
            *dstSN = '_';
            dstSN++;

            // Concatenate the NSID (converted to ASCII)
            tempLLong = pLunExt->namespaceId;

            /* move the NSID to the most significant bytes */
            tempLLong = tempLLong << 32;

            count = SntiConvertULLongToA(
                dstSN,
                tempLLong,
                NIBBLES_PER_LONG,
                FALSE,
                FALSE
                );

            dstSN += count;
            *dstSN = '.';

            pSerialNumberPage->PageLength = INQ_V10_SN_LENGTH;
        }
    }

    // Calculate the actual size of the entire page
    totalDataSize = FIELD_OFFSET(VPD_SERIAL_NUMBER_PAGE, SerialNumber) + pSerialNumberPage->PageLength;

    // Calculate the size of the data to be copied
    copyLength = min(allocLength, totalDataSize);

    // Copy data from the local buffer to the caller's buffer
    StorPortCopyMemory(GET_DATA_BUFFER(pSrb), pSerialNumberPage, copyLength);

    SET_DATA_LENGTH(pSrb, copyLength);
} /* SntiTranslateUnitSerialPage */



/******************************************************************************
* SntiTranslateDeviceIdentificationPage
*
* @brief Translates the SCSI Inquiry VPD page - Device Identification Page.
*        Populates the appropriate SCSI Inqiry response fields based on the
*        NVMe Translation spec. Do not need to create SQE here as we just
*        complete the command in the build phase (by returning FALSE to
*        StorPort with SRB status of SUCCESS).
*
* @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
*               that the user can access the SCSI CDB, response, and data from
*               this pointer. For example, if there is a failure in translation
*               resulting in sense data, then SNTI will call the appropriate
*               internal error handling code and set the status info/data and
*               pass the pSrb pointer as a parameter.
*
* @return VOID
******************************************************************************/
VOID SntiTranslateDeviceIdentificationPage(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PVPD_IDENTIFICATION_PAGE pDeviceIdPage = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    SNTI_STATUS status;
    NVMe_VERSION specVersion = { 0 };
    BOOLEAN eui64Valid = FALSE;
    BOOLEAN nguidValid = FALSE;
    BOOLEAN version10 = FALSE;
    BOOLEAN srbBufSpaceAvail = TRUE;
    SNTI_VPD_DESCRIPTOR_FLAGS descFlags = { 0 };
    PVOID pAlloc = NULL;
    PUINT8 pNext = NULL;
    UINT16 currentLength = 0;
    UINT16 allocLength = 0;
    UINT16 actualLength = 0;
    PVOID pSrbBuffer = NULL;
    UINT16 srbBufLength = 0;


    // Build the device identification page according to the SNTL 1.5 section 6.1.4
    // Attempt to build the following designators: NAA IEEE Registered Extended designator,
    // T10 Vendor ID based designator, SCSI Name String Designator and the EUI-64 based 
    // Designator. The value of these designators will depend on the NVMe EUI64, NVMe NGUID
    // fields and the NVMe Version level. 
    // 
    // In the first steps, this method determines which designators using which values. 
    // It also calculates the total amount of space required to build the response.
    // Next, it allocates a buffer for building the response page followed by building 
    // the response page in that buffer. 
    // Finally, the response page is copied to the caller's buffer.

    srbBufLength = GET_INQ_ALLOC_LENGTH(pSrb);
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pDevExt = pSrbExt->pNvmeDevExt;

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        /* Map the translation error to a SCSI error */
        SntiMapInternalErrorStatus(pSrb, status);
        return;
    }

    /*******************   Set flags to track which descriptors should be built *************/
    specVersion.AsUlong = StorPortReadRegisterUlong(pDevExt,
        (PULONG)(&pDevExt->pCtrlRegister->VS));


    if ((specVersion.MJR == 1) && (specVersion.MNR == 0)) {
        version10 = TRUE;
    }
    if (*((PULONGLONG)pLunExt->identifyData.EUI64) != 0) {
        eui64Valid = TRUE;
    }
    if ((pLunExt->identifyData.NGUID.LowerBytes != 0) ||
        (pLunExt->identifyData.NGUID.UpperBytes != 0)) {
        nguidValid = TRUE;
    }


    /*********** Calculate buffer size for header of VPD Identification page - used in all cases **************/

    // Calculate the buffer size required to build the Identification data   
    /* Include the size of the header for the vpd data page */
    allocLength = sizeof(VPD_IDENTIFICATION_PAGE);


    /**********Calculate space required for NAA IEEE Registerd Extended descriptor SNTL 1.5, Section 6.1.4.1***************/
    /* Track whether we should build V1.0 or EUI64 descriptor */
    if (eui64Valid) {
        descFlags.NaaIeeEui64Des = TRUE;
        /* Add in the size of the descriptor header and the descriptor data. */
        allocLength += VPD_ID_DESCRIPTOR_HDR_LENGTH + sizeof(SNTI_NAA_IEEE_EXT_DESCRIPTOR);
    } else if (version10 == TRUE) {
        descFlags.NaaIeeV10Des = TRUE;
        allocLength += VPD_ID_DESCRIPTOR_HDR_LENGTH + sizeof(SNTI_NAA_IEEE_EXT_DESCRIPTOR);
    }

    /**********Calculate space required for T10 Vendor ID descriptor SNTL 1.5, Section 6.1.4.3*****************************/
    /* Add in the size of the T10 descriptor header and the VID portion of the descriptor */

    // Calulate size of the Vendor Specific Identifier data
    if (version10 == TRUE) {
        descFlags.T10VidV10Des = TRUE;
        /* Data is comprised of descriptor header, VID port of the descriptor, */
        /* Product Identification field from the Inquiry data and the 100 bits */
        /* derived from VID/SN/NSID */
        allocLength += VPD_ID_DESCRIPTOR_HDR_LENGTH + T10_VID_DES_VID_FIELD_SIZE + PRODUCT_ID_ASCII_SIZE + 
            VENDOR_ID_ASCII_SIZE + VENDOR_SPEC_V10_SN_ASCII_SIZE + VENDOR_SPEC_V10_NSID_ASCII_SIZE;

    } else if (nguidValid == TRUE) {
        descFlags.T10VidNguidDes = TRUE;
        /* Data is comprised of descriptor header, VID port of the descriptor, */
        /* translation of the Product Identification field from the Inquiry data */
        /* and the NGUID field from Identify controller */
        allocLength += VPD_ID_DESCRIPTOR_HDR_LENGTH + T10_VID_DES_VID_FIELD_SIZE + 
            PRODUCT_ID_ASCII_SIZE + NGUID_ASCII_SIZE;

    } else if (eui64Valid == TRUE) {
        descFlags.T10VidEui64Des = TRUE;
        /* Data is comprised of descriptor header, VID port of the descriptor, */
        /* translation of the Product Identification field from the Inquiry data */
        /* and the EUI64 field from Identify Namespace */
        allocLength += VPD_ID_DESCRIPTOR_HDR_LENGTH + T10_VID_DES_VID_FIELD_SIZE + 
            PRODUCT_ID_ASCII_SIZE + EUI64_ASCII_SIZE;
    }


    /**********Calculate space required for SCSI Name String descriptor SNTL 1.5, Section 6.1.4.4*****************************/
    if (nguidValid == TRUE) {
        // Using the NGUID for building the SCSI Name String descriptor - SNTL 1.5, Section 6.1.4.4.1.1
        descFlags.ScsiNguidDes = TRUE;
        /* Add in the size of the SCSI Name String Header, the string "EUI." */
        /* and the translated NGUID value */
        allocLength += VPD_ID_DESCRIPTOR_HDR_LENGTH + EUI_ASCII_SIZE + NGUID_ASCII_SIZE;

    } else if (eui64Valid == TRUE) {
        // Using the EUI64 for building the SCSI Name String descriptor - SNTL 1.5, Section 6.1.4.4.1.2
        descFlags.ScsiEui64Des = TRUE;
        /* Add in the size of the SCSI Name String Header, the string "EUI." */
        /* and the translated EUI64 value */
        allocLength += VPD_ID_DESCRIPTOR_HDR_LENGTH + EUI_ASCII_SIZE + EUI64_ASCII_SIZE;

    } else if (version10 == TRUE) {
        // Using the NGUID for building the SCSI Name String descriptor - SNTL 1.5, Section 6.1.4.4.1.3
        descFlags.ScsiV10Des = TRUE;
        /* Add in the size of the SCSI Name String Header, */
        /* the V1.0 version of the string (68 bytes comprised of 4 bytes VID, */
        /* 40 bytes Model Number, 4 bytes NSID, 20 bytes Serial Number) */
        allocLength += VPD_ID_DESCRIPTOR_HDR_LENGTH +
            SCSI_NAME_PCI_VENDOR_ID_SIZE + SCSI_NAME_MODEL_NUM_SIZE +
            SCSI_NAME_NAMESPACE_ID_SIZE + SCSI_NAME_SERIAL_NUM_SIZE;
    }


    /**********Calculate space required for EUI-64 based descriptor SNTL 1.5, Section 6.1.4.5*****************************/
    /* EUI-64 or NGUID value must be valid to create this descriptor */
    if (nguidValid == TRUE) {
        // Using the NGUID for building the EUI-64 based descriptor - SNTL 1.5, Section 6.1.4.5.1
        descFlags.Eui64NguidDes = TRUE;
        // Allow buffer space for descriptor header and the NGUID value
        allocLength += VPD_ID_DESCRIPTOR_HDR_LENGTH + NGUID_DATA_SIZE;

    } else if (eui64Valid == TRUE) {
        // Using the EUI64 for building the EUI-64 based descriptor - SNTL 1.5, Section 6.1.4.5.1
        descFlags.Eui64Des = TRUE;
        // Allow buffer space for descriptor header and the NGUID value
        allocLength += VPD_ID_DESCRIPTOR_HDR_LENGTH + EUI64_DATA_SIZE;

    }

    // Allocate buffer for building the descriptors
    pAlloc = NVMeAllocatePool(pDevExt, allocLength);
    if (pAlloc == NULL) {
        /* Map the memory allocation failure to a SCSI error */
        SntiMapInternalErrorStatus(pSrb, SNTI_NO_MEMORY);
        return;
    }
    pDeviceIdPage = (PVPD_IDENTIFICATION_PAGE)pAlloc;
    memset(pDeviceIdPage, 0, allocLength);



    /*********************Build the VPD Identification Page header ****************/
    pDeviceIdPage->DeviceType = DIRECT_ACCESS_DEVICE;
    pDeviceIdPage->DeviceTypeQualifier = DEVICE_CONNECTED;
    pDeviceIdPage->PageCode = VPD_DEVICE_IDENTIFIERS;

    // Keep a running total of the space used and a pointer to the
    // next location to place descriptor data
    currentLength = sizeof(VPD_IDENTIFICATION_PAGE);
    pNext = pDeviceIdPage->Descriptors;

    // Determine if the srb buffer has any space left for remaining descriptor data
    // If not, don't bother to build those descriptors
    if (currentLength >= srbBufLength) {
        srbBufSpaceAvail = FALSE;
    }

    if (srbBufSpaceAvail == TRUE) {

        /* Build the NAA IEEE Registered Extended descriptor */
        srbBufSpaceAvail = SntiBuildIeeeRegExtDesc(
            &descFlags,
            &currentLength,
            srbBufLength,
            &pNext,
            pLunExt,
            pDevExt);


        if (srbBufSpaceAvail == TRUE) {
            /* Build the T10 Vendor ID based Descriptor */
            srbBufSpaceAvail = SntiBuildT10VidBasedDesc(
                &descFlags,
                &currentLength,
                srbBufLength,
                &pNext,
                pLunExt,
                pDevExt
                );

            if (srbBufSpaceAvail == TRUE) {
                /* Build the SCSI Name string descriptor */
                srbBufSpaceAvail = SntiBuildScsiNameStringDesc(
                    &descFlags,
                    &currentLength,
                    srbBufLength,
                    &pNext,
                    pLunExt,
                    pDevExt
                    );

                if (srbBufSpaceAvail == TRUE) {
                    /* Build the EUI-64 based descriptor */
                    SntiBuildEui64BasedDesc(
                        &descFlags,
                        &currentLength,
                        srbBufLength,
                        &pNext,
                        pLunExt,
                        pDevExt
                        );
                }
            }

        }
    }

    /* Get pointer to actual response buffer */
    pSrbBuffer = GET_DATA_BUFFER(pSrb);

    /* Calculate how much data to return */
    actualLength = min(currentLength, srbBufLength);
    SET_DATA_LENGTH(pSrb, actualLength);

    /* Set the length to the amount of all the data, whether or not it is all returned. */
    /* If the SRB buffer allocation is too small to transfer all of the data */
    /* the PageLength field shall not be adjusted to reflect the truncation */
    pDeviceIdPage->PageLength = (UCHAR)allocLength - sizeof(VPD_IDENTIFICATION_PAGE);


    /* Copy the descriptor data into the SRB buffer */
    if (actualLength > 0) {
        StorPortCopyMemory(pSrbBuffer, pAlloc, actualLength);
    }


    StorPortFreePool((PVOID)pDevExt, pAlloc);

} /* SntiTranslateDeviceIdentificationPage */


/******************************************************************************
* SntiBuildIeeeRegExtDesc
*
* @brief This method builds a NAA IEEE Registered Extended Descriptor
*        to be returned in the Device Identification Data Page.
*        Based on SNTL 1.5, section 6.1.4.1
*
* @param pDescFlags A Structure with flags indicating whether this method should
*                    build a Version 1.0 or EUI64-based descriptor
* @param pCurrentLength - Used to track the amount of all the data in the
*                         Device Identification Data Page
* @param ppNext - Point to the location of the next data in the Device Identification
*                 Data Page
* @param pLunExt - Pointer to the LUN extension
* @param pDevExt - Pointer to the device extension
*
* @return srbSpaceAvailable - BOOLEAN indicating if there is additional SRB buffer space
*                                     for any more descriptors
******************************************************************************/
BOOLEAN SntiBuildIeeeRegExtDesc(
    PSNTI_VPD_DESCRIPTOR_FLAGS pDescFlags,
    UINT16 *pCurrentLength,
    UINT16 srbBufLength,
    PUINT8 *ppNext,
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_DEVICE_EXTENSION pDevExt
    )
{
    PVPD_IDENTIFICATION_DESCRIPTOR pIdDescriptor = NULL;
    PSNTI_NAA_IEEE_EXT_DESCRIPTOR pNaaData = NULL;
    USHORT tempShort = 0;
    ULONG tempLong = 0;
    ULONGLONG tempLongLong = 0;

    UINT64 eui64Id = 0;
    BOOLEAN srbSpaceAvailable = TRUE;

    if ((pDescFlags->NaaIeeEui64Des == FALSE) &&
        (pDescFlags->NaaIeeV10Des == FALSE)) {
        return srbSpaceAvailable;
    }

    /* Setup the descriptor header */
    pIdDescriptor = (PVPD_IDENTIFICATION_DESCRIPTOR)*ppNext;

    /* Build the descriptor header */
    pIdDescriptor->CodeSet = VpdCodeSetBinary;
    pIdDescriptor->IdentifierType = VpdIdentifierTypeFCPHName;
    pIdDescriptor->Association = VpdAssocDevice;

    /* Track size of the descriptor header */
    *ppNext += VPD_ID_DESCRIPTOR_HDR_LENGTH;
    *pCurrentLength += VPD_ID_DESCRIPTOR_HDR_LENGTH;

    /* Build the descriptor*/
    pNaaData = (PSNTI_NAA_IEEE_EXT_DESCRIPTOR)*ppNext;
    pNaaData->Naa = NAA_IEEE_EX;
    pNaaData->IeeeCompIdMSB = pDevExt->controllerIdentifyData.IEEE[0] >> 4;

    tempShort = (pDevExt->controllerIdentifyData.IEEE[0] & 0x0F) << 12;
    tempShort |= pDevExt->controllerIdentifyData.IEEE[1] << 4;
    tempShort |= pDevExt->controllerIdentifyData.IEEE[2] >> 4;

    /* Convert data to big endian when it's writing to the actual descriptor */
    REVERSE_BYTES_SHORT(&pNaaData->IeeeCompId, &tempShort);
    pNaaData->IeeeCompIdLSB = pDevExt->controllerIdentifyData.IEEE[2];

    /* Build a EUI64 type descriptor? */
    if (pDescFlags->NaaIeeEui64Des == TRUE) {
        /* 100 bit field is created by concatenating EUI64 followed by 0's */

        /* Setup the Naa Vendor Id data */
        /*  Get a Little endian copy of the EUI64 */
        REVERSE_BYTES_QUAD(&eui64Id, &pLunExt->identifyData.EUI64);
        pNaaData->VendIdMSB = eui64Id >> 60;
        tempLong = (ULONG)(eui64Id >> 28);
        /* Write Big Endian Vendor Id */
        REVERSE_BYTES(&pNaaData->VendId, &tempLong);

        tempLongLong = eui64Id << 36;
        /* Write big endian Vendor specific ext data */
        REVERSE_BYTES_QUAD(&pNaaData->VenSpecIdExt, &tempLongLong);
    } else {
        /* Build the Version 1.0 descriptor */

        /* Build descriptor based 100 bit value comprised of VID/SN and NSID */
        tempLong = pDevExt->controllerIdentifyData.VID;
        /* set bits 99:96 */
        pNaaData->VendIdMSB = (UCHAR)(tempLong >> 12);

        /* set bits 95:84 */
        tempLong = tempLong << 20;

        /* set bits 83:76 */
        tempLong |= pDevExt->controllerIdentifyData.SN[0] << 12;

        /* set bits 75:68 */
        tempLong |= pDevExt->controllerIdentifyData.SN[1] << 4;

        /* set bits 67:64 */
        tempLong |= (pDevExt->controllerIdentifyData.SN[2] & UPPER_NIBBLE_MASK) >> 4;

        /* write data into the descriptor as BE data */
        REVERSE_BYTES(&pNaaData->VendId, &tempLong);


        /* Set bits 63:32 */
        REVERSE_BYTES_QUAD(&tempLongLong, &pDevExt->controllerIdentifyData.SN[2]);
        tempLongLong = tempLongLong << NIBBLE_SHIFT;
        tempLongLong &= 0xffffffff00000000;

        /* Set bits 31:0 */
        tempLongLong |= pLunExt->namespaceId;

        /* write data into the descriptor as BE data */
        REVERSE_BYTES_QUAD(&pNaaData->VenSpecIdExt, &tempLongLong);
    }

    /* Track size of the descriptor data */
    *ppNext += sizeof(SNTI_NAA_IEEE_EXT_DESCRIPTOR);
    *pCurrentLength += sizeof(SNTI_NAA_IEEE_EXT_DESCRIPTOR);
    pIdDescriptor->IdentifierLength = sizeof(SNTI_NAA_IEEE_EXT_DESCRIPTOR);

    if (*pCurrentLength >= srbBufLength) {
        srbSpaceAvailable = FALSE;
    }
    return srbSpaceAvailable;
}


/******************************************************************************
* SntiBuildT10VidBasedDesc
*
* @brief This method builds a T10 Vendor Id Based descriptor
*        to be returned in the Device Identification Data Page.
*        Based on SNTL 1.5, section 6.1.4.3
*
* @param pDescFlags A Structure with flags indicating whether this method should
*                    build a Version 1.0 or EUI64-based descriptor
* @param pCurrentLength - Used to track the amount of all the data in the
*                         Device Identification Data Page
* @param ppNext - Point to the location of the next data in the Device Identification
*                 Data Page
* @param pLunExt - Pointer to the LUN extension
* @param pDevExt - Pointer to the device extension
*
* @return srbSpaceAvailable - BOOLEAN indicating if there is additional SRB buffer space
*                                     for any more descriptors
******************************************************************************/
BOOLEAN SntiBuildT10VidBasedDesc(
    PSNTI_VPD_DESCRIPTOR_FLAGS pDescFlags,
    UINT16 *pCurrentLength,
    UINT16 srbBufLength,
    PUINT8 *ppNext,
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_DEVICE_EXTENSION pDevExt
    )
{
    PVPD_IDENTIFICATION_DESCRIPTOR pIdDescriptor = NULL;


    PSNTI_T10_VID_DESCRIPTOR pT10Data = NULL;
    USHORT i = 0;
    USHORT startIdx = 0;
    ULONGLONG tempLongLong = 0;
    BOOLEAN srbSpaceAvailable = TRUE;
    UCHAR VendorId[9] = "NVMe    ";
    /* Setup the descriptor header */
    pIdDescriptor = (PVPD_IDENTIFICATION_DESCRIPTOR)*ppNext;

    if ((pDescFlags->T10VidEui64Des == FALSE) &&
        (pDescFlags->T10VidNguidDes == FALSE) &&
        (pDescFlags->T10VidV10Des == FALSE)) {
        return srbSpaceAvailable;
    }

    /* Build the descriptor header */
    pIdDescriptor->CodeSet = VpdCodeSetAscii;
    pIdDescriptor->IdentifierType = VpdIdentifierTypeVendorId;
    pIdDescriptor->Association = VpdAssocDevice;

    /* Track size of the descriptor header */
    *ppNext += VPD_ID_DESCRIPTOR_HDR_LENGTH;
    *pCurrentLength += VPD_ID_DESCRIPTOR_HDR_LENGTH;

    /* Build the descriptor*/
    pT10Data = (PSNTI_T10_VID_DESCRIPTOR)*ppNext;

    /* T10 Vendor ID */
    memcpy(pT10Data->VendorId, VendorId, 8);

    // Write the Product Identification data into Vendor specific identifier 
    // Copy a byte at a time to enable checking for NULL character
    for (i = 0; i < PRODUCT_ID_ASCII_SIZE; i++) {
        if (pDevExt->controllerIdentifyData.MN[i] == 0x00) {
            pT10Data->VendorSpecific[i] = ASCII_SPACE_CHAR_VALUE;
        } else {
            pT10Data->VendorSpecific[i] = pDevExt->controllerIdentifyData.MN[i];
        }
    }

    *ppNext += T10_VID_DES_VID_FIELD_SIZE + PRODUCT_ID_ASCII_SIZE;
    *pCurrentLength += T10_VID_DES_VID_FIELD_SIZE + PRODUCT_ID_ASCII_SIZE;
    pIdDescriptor->IdentifierLength = T10_VID_DES_VID_FIELD_SIZE + PRODUCT_ID_ASCII_SIZE;

    /* Build Version 1.0 based data? */
    if (pDescFlags->T10VidV10Des) {
        tempLongLong = pDevExt->controllerIdentifyData.VID;
        tempLongLong = tempLongLong << 48;
        startIdx = PRODUCT_ID_ASCII_SIZE;
        SntiConvertULLongToA(
            &pT10Data->VendorSpecific[startIdx],
            tempLongLong,
            VENDOR_ID_ASCII_SIZE,
            FALSE,
            FALSE
            );

        /* Convert 52 bits of SN (bytes 10:04 of Identify controller */
        REVERSE_BYTES_QUAD(&tempLongLong, &pDevExt->controllerIdentifyData.SN[0]);
        startIdx += VENDOR_ID_ASCII_SIZE;
        SntiConvertULLongToA(
            &pT10Data->VendorSpecific[startIdx],
            tempLongLong,
            VENDOR_SPEC_V10_SN_ASCII_SIZE,
            FALSE,
            FALSE
            );

        /* Convert NSID */
        tempLongLong = pLunExt->namespaceId;
        tempLongLong = tempLongLong << 32;
        startIdx += VENDOR_SPEC_V10_SN_ASCII_SIZE;
        SntiConvertULLongToA(
            &pT10Data->VendorSpecific[startIdx],
            tempLongLong,
            VENDOR_SPEC_V10_NSID_ASCII_SIZE,
            FALSE,
            FALSE
            );

        *ppNext += VENDOR_ID_ASCII_SIZE + VENDOR_SPEC_V10_SN_ASCII_SIZE + VENDOR_SPEC_V10_NSID_ASCII_SIZE;
        *pCurrentLength += VENDOR_ID_ASCII_SIZE + VENDOR_SPEC_V10_SN_ASCII_SIZE + VENDOR_SPEC_V10_NSID_ASCII_SIZE;
        pIdDescriptor->IdentifierLength += VENDOR_ID_ASCII_SIZE + VENDOR_SPEC_V10_SN_ASCII_SIZE + VENDOR_SPEC_V10_NSID_ASCII_SIZE;

    } else if (pDescFlags->T10VidNguidDes == TRUE) {

        /* Convert the NGUID upper bytes */
        REVERSE_BYTES_QUAD(&tempLongLong, &pLunExt->identifyData.NGUID.UpperBytes);
        startIdx = PRODUCT_ID_ASCII_SIZE;
        SntiConvertULLongToA(
            &pT10Data->VendorSpecific[startIdx],
            tempLongLong,
            NGUID_ASCII_SIZE / 2,
            FALSE,
            FALSE
            );

        /* Convert the NGUID lower bytes */
        REVERSE_BYTES_QUAD(&tempLongLong, &pLunExt->identifyData.NGUID.LowerBytes);
        startIdx += NGUID_ASCII_SIZE / 2;
        SntiConvertULLongToA(
            &pT10Data->VendorSpecific[startIdx],
            tempLongLong,
            NGUID_ASCII_SIZE / 2,
            FALSE,
            FALSE
            );

        *ppNext += NGUID_ASCII_SIZE;
        *pCurrentLength += NGUID_ASCII_SIZE;
        pIdDescriptor->IdentifierLength += NGUID_ASCII_SIZE;
    } else if (pDescFlags->T10VidEui64Des == TRUE) {
        // Not using _ui64toa_s - it strips off leading 0's
        REVERSE_BYTES_QUAD(&tempLongLong, (PULONGLONG)(&pLunExt->identifyData.EUI64[0]));

        startIdx = PRODUCT_ID_ASCII_SIZE;
        SntiConvertULLongToA(
            &pT10Data->VendorSpecific[startIdx],
            tempLongLong,
            EUI64_ASCII_SIZE,
            FALSE,
            FALSE
            );

        /* Track size of the descriptor data */
        *ppNext += EUI64_ASCII_SIZE;
        *pCurrentLength += EUI64_ASCII_SIZE;
        pIdDescriptor->IdentifierLength += EUI64_ASCII_SIZE;
    }

    if (*pCurrentLength >= srbBufLength) {
        srbSpaceAvailable = FALSE;
    }

    return srbSpaceAvailable;
}


/******************************************************************************
* SntiBuildScsiNameStringDesc
*
* @brief This method builds a T10 Vendor Id Based descriptor
*        to be returned in the Device Identification Data Page.
*        Based on SNTL 1.5, section 6.1.4.4
*
* @param pDescFlags A Structure with flags indicating whether this method should
*                    build a Version 1.0 or EUI64-based descriptor
* @param pCurrentLength - Used to track the amount of all the data in the
*                         Device Identification Data Page
* @param ppNext - Point to the location of the next data in the Device Identification
*                 Data Page
* @param pHtoAConversion - Array for converting Hex nibble to ASCII byte.
* @param pLunExt - Pointer to the LUN extension
* @param pDevExt - Pointer to the device extension
*
* @return srbSpaceAvailable - BOOLEAN indicating if there is additional SRB buffer space
*                                     for any more descriptors
******************************************************************************/
BOOLEAN SntiBuildScsiNameStringDesc(
    PSNTI_VPD_DESCRIPTOR_FLAGS pDescFlags,
    UINT16 *pCurrentLength,
    UINT16 srbBufLength,
    PUINT8 *ppNext,
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_DEVICE_EXTENSION pDevExt
    )
{
    PVPD_IDENTIFICATION_DESCRIPTOR pIdDescriptor = NULL;
    ULONGLONG tempLongLong = 0;
    UINT16 startIdx = 0;
    BOOLEAN srbSpaceAvailable = TRUE;

    if ((pDescFlags->ScsiEui64Des == FALSE) &&
        (pDescFlags->ScsiNguidDes == FALSE) &&
        (pDescFlags->ScsiV10Des == FALSE)) {
        return srbSpaceAvailable;
    }


    /* Setup the descriptor header */
    pIdDescriptor = (PVPD_IDENTIFICATION_DESCRIPTOR)*ppNext;

    /* Build the descriptor header */
    pIdDescriptor->CodeSet = VpdCodeSetUTF8;
    pIdDescriptor->IdentifierType = VpdIdentifierTypeSCSINameString;
    pIdDescriptor->Association = VpdAssocDevice;

    /* Track size of the descriptor data */
    *ppNext += VPD_ID_DESCRIPTOR_HDR_LENGTH;
    *pCurrentLength += VPD_ID_DESCRIPTOR_HDR_LENGTH;

    /* Build the descriptor*/

    /* Build NGUID based data? */
    if (pDescFlags->ScsiNguidDes == TRUE) {
        StorPortCopyMemory(pIdDescriptor->Identifier, "EUI.", EUI_ASCII_SIZE);

        /* convert the NGUID upper bytes */
        REVERSE_BYTES_QUAD(&tempLongLong, &pLunExt->identifyData.NGUID.UpperBytes);

        startIdx = EUI_ASCII_SIZE;
        SntiConvertULLongToA(
            &pIdDescriptor->Identifier[startIdx],
            tempLongLong,
            NGUID_ASCII_SIZE / 2,
            FALSE,
            FALSE
            );

        /* convert the NGUID lower bytes */
        REVERSE_BYTES_QUAD(&tempLongLong, &pLunExt->identifyData.NGUID.LowerBytes);

        startIdx += NGUID_ASCII_SIZE / 2;
        SntiConvertULLongToA(
            &pIdDescriptor->Identifier[startIdx],
            tempLongLong,
            NGUID_ASCII_SIZE / 2,
            FALSE,
            FALSE
            );

        /* Track size of the descriptor data */
        *ppNext += EUI_ASCII_SIZE + NGUID_ASCII_SIZE;
        *pCurrentLength += EUI_ASCII_SIZE + NGUID_ASCII_SIZE;
        pIdDescriptor->IdentifierLength = EUI_ASCII_SIZE + NGUID_ASCII_SIZE;

    } else if (pDescFlags->ScsiEui64Des == TRUE) {
        StorPortCopyMemory(&pIdDescriptor->Identifier[0], "EUI.", EUI_ASCII_SIZE);

        REVERSE_BYTES_QUAD(&tempLongLong, &pLunExt->identifyData.EUI64);

        startIdx = EUI_ASCII_SIZE;
        SntiConvertULLongToA(
            &pIdDescriptor->Identifier[startIdx],
            tempLongLong,
            EUI64_ASCII_SIZE,
            FALSE,
            FALSE
            );

        /* Track size of the descriptor data */
        *ppNext += EUI_ASCII_SIZE + EUI64_ASCII_SIZE;
        *pCurrentLength += EUI_ASCII_SIZE + EUI64_ASCII_SIZE;
        pIdDescriptor->IdentifierLength = EUI_ASCII_SIZE + EUI64_ASCII_SIZE;
    } else if (pDescFlags->ScsiV10Des == TRUE) {
        /* V1.0 descriptor */
        /* Build bytes 67:48 - Copy bytes 23:04 of SN*/
        memcpy(&pIdDescriptor->Identifier[0], &pDevExt->controllerIdentifyData.SN[0], SCSI_NAME_SERIAL_NUM_SIZE);

        /* Build bytes 47:44 - Convert the namespace id */
        tempLongLong = pLunExt->namespaceId;
        tempLongLong = tempLongLong << 48;


        startIdx = SCSI_NAME_SERIAL_NUM_SIZE;
        SntiConvertULLongToA(
            &pIdDescriptor->Identifier[startIdx],
            tempLongLong,
            SCSI_NAME_NAMESPACE_ID_SIZE,
            FALSE,
            FALSE
            );

        /* Build bytes 43:04 - Copy MN, bytes 63:24 of Identify controller */
        startIdx += SCSI_NAME_NAMESPACE_ID_SIZE;
        memcpy(&pIdDescriptor->Identifier[startIdx], &pDevExt->controllerIdentifyData.MN[0], SCSI_NAME_MODEL_NUM_SIZE);

        /* Build bytes 03:00 - Convert the PCI Vendor ID */
        tempLongLong = pDevExt->controllerIdentifyData.VID;
        tempLongLong = tempLongLong << 48;


        startIdx += SCSI_NAME_MODEL_NUM_SIZE;
        SntiConvertULLongToA(
            &pIdDescriptor->Identifier[startIdx],
            tempLongLong,
            SCSI_NAME_PCI_VENDOR_ID_SIZE,
            FALSE,
            FALSE
            );

        *ppNext += SCSI_NAME_NAMESPACE_ID_SIZE + SCSI_NAME_SERIAL_NUM_SIZE + SCSI_NAME_MODEL_NUM_SIZE + SCSI_NAME_PCI_VENDOR_ID_SIZE;
        *pCurrentLength += SCSI_NAME_NAMESPACE_ID_SIZE + SCSI_NAME_SERIAL_NUM_SIZE + SCSI_NAME_MODEL_NUM_SIZE + SCSI_NAME_PCI_VENDOR_ID_SIZE;
        pIdDescriptor->IdentifierLength = SCSI_NAME_NAMESPACE_ID_SIZE + SCSI_NAME_SERIAL_NUM_SIZE + SCSI_NAME_MODEL_NUM_SIZE + SCSI_NAME_PCI_VENDOR_ID_SIZE;
    }


    if (*pCurrentLength >= srbBufLength) {
        srbSpaceAvailable = FALSE;
    }

    return srbSpaceAvailable;
}



/******************************************************************************
* SntiBuildEui64BasedDesc
*
* @brief This method builds a T10 Vendor Id Based descriptor
*        to be returned in the Device Identification Data Page.
*        Based on SNTL 1.5, section 6.1.4.5
*
* @param pDescFlags A Structure with flags indicating whether this method should
*                    build a Version 1.0 or EUI64-based descriptor
* @param pCurrentLength - Used to track the amount of all the data in the
*                         Device Identification Data Page
* @param ppNext - Point to the location of the next data in the Device Identification
*                 Data Page
* @param pLunExt - Pointer to the LUN extension
* @param pDevExt - Pointer to the device extension
*
* @return VOID
******************************************************************************/
VOID SntiBuildEui64BasedDesc(
    PSNTI_VPD_DESCRIPTOR_FLAGS pDescFlags,
    UINT16 *pCurrentLength,
    UINT16 srbBufLength,
    PUINT8 *ppNext,
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_DEVICE_EXTENSION pDevExt
    )
{
    PVPD_IDENTIFICATION_DESCRIPTOR pIdDescriptor = NULL;
    PUINT8 pDescData = NULL;

    if ((pDescFlags->Eui64Des == FALSE) &&
        (pDescFlags->Eui64NguidDes == FALSE)) {
        return;
    }


    /* Setup the descriptor header */
    pIdDescriptor = (PVPD_IDENTIFICATION_DESCRIPTOR)*ppNext;

    /* Build the descriptor header */
    pIdDescriptor->CodeSet = VpdCodeSetBinary;
    pIdDescriptor->IdentifierType = VpdIdentifierTypeEUI64;
    pIdDescriptor->Association = VpdAssocDevice;

    /* Track size of the descriptor header */
    *ppNext += VPD_ID_DESCRIPTOR_HDR_LENGTH;
    *pCurrentLength += VPD_ID_DESCRIPTOR_HDR_LENGTH;

    /* Build the descriptor*/
    pDescData = *ppNext;


    if (pDescFlags->Eui64NguidDes == TRUE) {
        memcpy(pDescData, &pLunExt->identifyData.NGUID.UpperBytes, NGUID_DATA_SIZE);

        /* Track size of the descriptor data */
        *ppNext += NGUID_DATA_SIZE;
        *pCurrentLength += NGUID_DATA_SIZE;
        pIdDescriptor->IdentifierLength = NGUID_DATA_SIZE;
    } else if (pDescFlags->Eui64Des == TRUE) {
        memcpy(pDescData, pLunExt->identifyData.EUI64, EUI64_DATA_SIZE);

        /* Track size of the descriptor data */
        *ppNext += EUI64_DATA_SIZE;
        *pCurrentLength += EUI64_DATA_SIZE;
        pIdDescriptor->IdentifierLength = EUI64_DATA_SIZE;
    }
}

/******************************************************************************
* SnitConvertULLongToA
*
* @brief Take a ULongLong variable and convert each nibble to ascii.
*        Caller provides destination pointer and number of nibbles to convert.
*        The method can optionally insert an underscore every fourth character
*        (terminating the result with a '.')
*
* @param pDest - Pointer to destination buffer for conversion.
* @param data  - ULONGLONG value to be converted to ASCII
* @param size -  Number of nibbles to be converted. Max value is 16
* @param underscore - Boolean indicating whether or not the method should
*                     insert underscores. If this is TRUE, an underscore will
*                     be inserted after every fourth character.
* @param termPeriod - Boolean indicating whether or not the conversion should
*                     be terminated with a period.
* @p
*
* @return Number of characters written to the destination
******************************************************************************/
USHORT SntiConvertULLongToA(
    PUCHAR pDest,
    ULONGLONG data,
    ULONG size,
    BOOLEAN underscore,
    BOOLEAN termPeriod
    )
{
    UCHAR HtoAConversion[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    PUCHAR pCurrent = NULL;
    UCHAR tempChar = 0;
    USHORT idx = 0;
    USHORT count = 0;

    size = size > NIBBLES_PER_LONGLONG ? NIBBLES_PER_LONGLONG : size;
    for (idx = 0, pCurrent = pDest; idx < size; idx++) {
        /* Convert Most Significant nibble to ASCII */
        tempChar = (UCHAR)(data >> 60);
        *pCurrent = HtoAConversion[tempChar];
        pCurrent++;
        count++;
        data = data << NIBBLE_SHIFT;

        if (underscore == TRUE) {
            // Every 4th nibble, insert a "_"
            if (((idx + 1) % INQ_SN_SUB_LENGTH) == 0) {
                *pCurrent = '_';
                pCurrent++;
                count++;
            }
        }
    }

    /* Add a terminating period if requested */
    if ((termPeriod == TRUE) &&
        (size != 0)) {
        pCurrent--;
        if (*pCurrent == '_') {
            *pCurrent = '.';
        } else {
            pCurrent++;
            count++;
            *pCurrent = '.';
        }
        pCurrent++;
    }

    return count;
}


#if (NTDDI_VERSION > NTDDI_WIN7)
/******************************************************************************
 * SntiTranslateBlockLimitsPage
 *
 * @brief Translates the SCSI Block Limits VPD page.
 *        Populates the appropriate block limits page fields based on the
 *        NVMe Translation spec. Do not need to create SQE here as we just
 *        complete the command in the build phase (by returning FALSE to
 *        StorPort with SRB status of SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateBlockLimitsPage(
    PSTORAGE_REQUEST_BLOCK pSrb
)
{
    UINT16 allocLen = 0;
    UINT32 tempVar32 = 0;
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PVPD_BLOCK_LIMITS_PAGE pBLPage = NULL;
    PADMIN_IDENTIFY_CONTROLLER pCntrlIdData = NULL;
    VPD_BLOCK_LIMITS_PAGE tmpBLPage;
    pSrbExt = (PNVME_SRB_EXTENSION)SrbGetMiniportContext(pSrb);

    pBLPage = (PVPD_BLOCK_LIMITS_PAGE)GET_DATA_BUFFER(pSrb);
    allocLen = GET_INQ_ALLOC_LENGTH(pSrb);
    if (allocLen < sizeof(VPD_BLOCK_LIMITS_PAGE)) {
        pBLPage = &tmpBLPage;
    }
    pCntrlIdData = &(pSrbExt->pNvmeDevExt->controllerIdentifyData);

    if (allocLen > 0) {
        memset(pBLPage, 0, allocLen);
        pBLPage->DeviceType = DIRECT_ACCESS_DEVICE;
        pBLPage->DeviceTypeQualifier = DEVICE_CONNECTED;
        pBLPage->PageCode = VPD_BLOCK_LIMITS;
        /* This field in VPD page needs data in big endian format */
        pBLPage->PageLength[1] = BLOCK_LIMITS_PAGE_LENGTH;

        if (pCntrlIdData->ONCS.SupportsDataSetManagement == 1) {
            /* These fields in VPD page also need data in big endian format */
            tempVar32 = NVME_MAX_NUM_BLOCKS_PER_READ_WRITE;
            REVERSE_BYTES(pBLPage->MaximumUnmapLBACount, &tempVar32);

            tempVar32 = MAX_UNMAP_BLOCK_DESCRIPTOR_COUNT;
            REVERSE_BYTES(pBLPage->MaximumUnmapBlockDescriptorCount, &tempVar32);
        }
        else {
            *(PUINT32)(pBLPage->MaximumUnmapLBACount) = 0;
            *(PUINT32)(pBLPage->MaximumUnmapBlockDescriptorCount) = 0;
        }
    }
    if (allocLen > 0 && allocLen < sizeof(VPD_BLOCK_LIMITS_PAGE)) {
        StorPortCopyMemory(GET_DATA_BUFFER(pSrb), pBLPage, allocLen);
    }

    SET_DATA_LENGTH(pSrb, min(sizeof(VPD_BLOCK_LIMITS_PAGE), allocLen));
} /* SntiTranslateBlockLimitsPage */
#endif 

#if (NTDDI_VERSION > NTDDI_WIN7)
/******************************************************************************
 * SntiTranslateBlockDeviceCharacteristicsPage
 *
 * @brief Translates the SCSI Block Device Characterics VPD page.
 *        Populates the appropriate block device Characterics page fields based 
 *        on the NVMe Translation spec. Do not need to create SQE here as we 
 *        just complete the command in the build phase (by returning FALSE to
 *        StorPort with SRB status of SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateBlockDeviceCharacteristicsPage(
    PSTORAGE_REQUEST_BLOCK pSrb
)
{
    UINT16 allocLen = 0;
    PVPD_BLOCK_DEVICE_CHARACTERISTICS_PAGE pBDCPage = NULL;
    VPD_BLOCK_DEVICE_CHARACTERISTICS_PAGE tmpBDCPage;
    
    pBDCPage = (PVPD_BLOCK_DEVICE_CHARACTERISTICS_PAGE)GET_DATA_BUFFER(pSrb);
    allocLen = GET_INQ_ALLOC_LENGTH(pSrb);

    if (allocLen < sizeof(VPD_BLOCK_DEVICE_CHARACTERISTICS_PAGE)) {
        pBDCPage = &tmpBDCPage;
    }

    if (allocLen > 0) {
        memset(pBDCPage, 0, allocLen);
        pBDCPage->DeviceType = DIRECT_ACCESS_DEVICE;
        pBDCPage->DeviceTypeQualifier = DEVICE_CONNECTED;
        pBDCPage->PageCode = VPD_BLOCK_DEVICE_CHARACTERISTICS;
        pBDCPage->PageLength = BLOCK_DEVICE_CHAR_PAGE_LENGTH;
        pBDCPage->MediumRotationRateLsb = (UCHAR)MEDIUM_ROTATIONAL_RATE;
        pBDCPage->MediumRotationRateMsb = (UCHAR)(MEDIUM_ROTATIONAL_RATE >> 8);
        pBDCPage->NominalFormFactor = FORM_FACTOR_NOT_REPORTED;
    }
    if (allocLen > 0 && allocLen < sizeof(VPD_BLOCK_DEVICE_CHARACTERISTICS_PAGE)) {
        StorPortCopyMemory(GET_DATA_BUFFER(pSrb), pBDCPage, allocLen);
    }

    SET_DATA_LENGTH(pSrb,
        min(sizeof(VPD_BLOCK_DEVICE_CHARACTERISTICS_PAGE), allocLen));
} /* SntiTranslateBlockDeviceCharacteristicsPage */
#endif

#if (NTDDI_VERSION > NTDDI_WIN7)
/******************************************************************************
 * SntiTranslateLogicalBlockProvisioningPage
 *
 * @brief Translates the SCSI Logical Block Provisioning VPD page.
 *        Populates the appropriate logical block provisioning page fields 
 *        based on the NVMe Translation spec. Do not need to create SQE here as
 *        we just complete the command in the build phase (by returning FALSE 
 *        to StorPort with SRB status of SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateLogicalBlockProvisioningPage(
    PSTORAGE_REQUEST_BLOCK pSrb,
    PNVME_LUN_EXTENSION pLunExt
)
{
    UINT16 allocLen = 0;
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PADMIN_IDENTIFY_CONTROLLER pCntrlIdData = NULL;
    PVPD_LOGICAL_BLOCK_PROVISIONING_PAGE pLBPPage = NULL;
    VPD_LOGICAL_BLOCK_PROVISIONING_PAGE tmpLBPPage;
    pSrbExt = (PNVME_SRB_EXTENSION)SrbGetMiniportContext(pSrb);
    pCntrlIdData = &(pSrbExt->pNvmeDevExt->controllerIdentifyData);

    pLBPPage = (PVPD_LOGICAL_BLOCK_PROVISIONING_PAGE)GET_DATA_BUFFER(pSrb);
    allocLen = GET_INQ_ALLOC_LENGTH(pSrb);
    if (allocLen < sizeof(VPD_LOGICAL_BLOCK_PROVISIONING_PAGE)) {
        pLBPPage = &tmpLBPPage;
    }

    if (allocLen > 0) {
        memset(pLBPPage, 0, allocLen);
        pLBPPage->DeviceType = DIRECT_ACCESS_DEVICE;
        pLBPPage->DeviceTypeQualifier = DEVICE_CONNECTED;
        pLBPPage->PageCode = VPD_LOGICAL_BLOCK_PROVISIONING;
        /* This field in VPD page needs data in big endian format */
        pLBPPage->PageLength[1] = LOGICAL_BLOCK_PROVISIONING_PAGE_LENGTH;

        pLBPPage->ThresholdExponent = NO_THIN_PROVISIONING_THRESHHOLD;
        if (pCntrlIdData->ONCS.SupportsDataSetManagement == 1) {
            /* LBPU set indicates that we support UNMAP */
            pLBPPage->LBPU = 1;

            /* 
               LBPRZ set indicates whether zeros are returned when deallocated
               LBAs are subsequently read after UNMAP
               */
            pLBPPage->LBPRZ = ZEROS_RETURNED_INDICATOR;    
        } else {
            pLBPPage->LBPU = 0;
        }
        pLBPPage->LBPWS = WR_SAME_16_TO_UNMAP_NOT_SUPPORTED;
        pLBPPage->LBPWS10 = WR_SAME_10_TO_UNMAP_NOT_SUPPORTED;
        pLBPPage->ANC_SUP = ANC_NOT_SUPPORTED;
        pLBPPage->DP = NO_PROVISIONING_GROUP_DESCRIPTOR;

        if (pCntrlIdData->ONCS.SupportsDataSetManagement == 1) {
            pLBPPage->ProvisioningType = PROVISIONING_TYPE_RESOURCE;
        } else {
            if(pLunExt->identifyData.NSFEAT.SupportsThinProvisioning == 1){
                pLBPPage->ProvisioningType = PROVISIONING_TYPE_THIN;
            } else {
                pLBPPage->ProvisioningType = PROVISIONING_TYPE_UNKNOWN;
            }
        }
    }
    if (allocLen > 0 && allocLen < sizeof(VPD_LOGICAL_BLOCK_PROVISIONING_PAGE)) {
        StorPortCopyMemory(GET_DATA_BUFFER(pSrb), pLBPPage, allocLen);
    }

    SET_DATA_LENGTH(pSrb, 
        min(sizeof(VPD_LOGICAL_BLOCK_PROVISIONING_PAGE), allocLen));
} /* SntiTranslateLogicalBlockProvisioningPage */
#endif

/******************************************************************************
 * SntiTranslateExtendedInquiryDataPage
 *
 * @brief Translates the SCSI Inquiry VPD page - Extended Inquiry Data Page.
 *        Populates the appropriate SCSI Inqiry response fields based on the
 *        NVMe Translation spec. Do not need to create SQE here as we just
 *        complete the command in the build phase (by returning FALSE to
 *        StorPort with SRB status of SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateExtendedInquiryDataPage(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PEXTENDED_INQUIRY_DATA pExtInqData = NULL;
    EXTENDED_INQUIRY_DATA tmpExtInqData;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVME_SRB_EXTENSION pSrbExt = NULL;

    UINT16 allocLength;
    UINT8 dataProtectionCapabilities;
    UINT8 dataProtectionSettings;

    SNTI_STATUS status;

    /* Default the translation status to command completed */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pExtInqData = (PEXTENDED_INQUIRY_DATA)GET_DATA_BUFFER(pSrb);
    pDevExt = pSrbExt->pNvmeDevExt;
    allocLength = GET_INQ_ALLOC_LENGTH(pSrb);

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        /* Map the translation error to a SCSI error */
        SntiMapInternalErrorStatus(pSrb, status);
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        /* DPC field check from Namepspace Identify struct */
        if (pLunExt->identifyData.DPC.SupportsProtectionType1 ||
            pLunExt->identifyData.DPC.SupportsProtectionType2 ||
            pLunExt->identifyData.DPC.SupportsProtectionType3 ||
            pLunExt->identifyData.DPC.SupportsProtectionFirst8 ||
            pLunExt->identifyData.DPC.SupportsProtectionLast8) {
            dataProtectionCapabilities = PROTECTION_ENABLED;
        } else {
            dataProtectionCapabilities = PROTECTION_DISABLED;
        }

        /* DPS field check from Namepspace Identify struct */
        if (pLunExt->identifyData.DPS.ProtectionEnabled)
            dataProtectionSettings = PROTECTION_ENABLED;
        else
            dataProtectionSettings = PROTECTION_DISABLED;

        if (allocLength < EXTENDED_INQUIRY_DATA_PAGE_SIZE) {
            pExtInqData = &tmpExtInqData;
        }
        if (allocLength > 0) {
            memset(pExtInqData, 0, allocLength);
            pExtInqData->DeviceType             = DIRECT_ACCESS_DEVICE;
            pExtInqData->DeviceTypeQualifier    = DEVICE_CONNECTED;
            pExtInqData->PageCode               = VPD_EXTENDED_INQUIRY_DATA;
            pExtInqData->Reserved1              = RESERVED_FIELDS;
            pExtInqData->PageLength             = EXTENDED_INQUIRY_DATA_PAGE_LENGTH;
            pExtInqData->ActivateMicrocode      = ACTIVATE_AFTER_HARD_RESET;

            pExtInqData->SupportedProtectionType   = dataProtectionCapabilities;
            pExtInqData->GuardCheck                = dataProtectionSettings;
            pExtInqData->ApplicationTagCheck       = dataProtectionSettings;
            pExtInqData->ReferenceTagCheck         = dataProtectionSettings;
            pExtInqData->Reserved2                 = RESERVED_FIELDS;
            pExtInqData->UACSKDataSupported        = SENSE_KEY_SPECIFIC_DATA;
            pExtInqData->GroupingFunctionSupported = GROUPING_FUNCTION_UNSUPPORTED;

            pExtInqData->PrioritySupported    = COMMAND_PRIORITY_UNSUPPORTED;
            pExtInqData->HeadOfQueueSupported = HEAD_OF_QUEUE_TASK_ATTR_UNSUPPORTED;
            pExtInqData->OrderedSupported     = ORDERED_TASK_ATTR_UNSUPPORTED;
            pExtInqData->SimpleSupported      = SIMPLE_TASK_ATTR_UNSUPPORTED;
            pExtInqData->Reserved3            = RESERVED_FIELDS;

            pExtInqData->WriteUncorrectableSupported =
                WRITE_UNCORRECTABLE_UNSUPPORTED;
            pExtInqData->CorrectionDisableSupported  =
                CORRECTION_DISABLE_UNSUPPORTED;
            pExtInqData->NonVolatileCacheSupported   =
                NON_VOLATILE_CACHE_UNSUPPORTED;
            pExtInqData->VolatileCacheSupported      =
                pDevExt->controllerIdentifyData.VWC.Present;
            pExtInqData->Reserved4                   =
                RESERVED_FIELDS;

            pExtInqData->ProtectionInfoIntervalSupported =
                PROTECTION_INFO_INTERNALS_UNSUPPORTED;

            pExtInqData->Reserved5               = RESERVED_FIELDS;
            pExtInqData->LogicalUnitITNexusClear = LUN_UNIT_ATTENTIONS_CLEARED;
            pExtInqData->Reserved6               = RESERVED_FIELDS;
            pExtInqData->ReferralsSupported      = REFERRALS_UNSUPPORTED;
            pExtInqData->Reserved6               = RESERVED_FIELDS;

            pExtInqData->CapabilityBasedCommandSecurity =
                CAPABILITY_BASED_SECURITY_UNSUPPORTED;
            pExtInqData->Reserved7                      =
                RESERVED_FIELDS;
            pExtInqData->MultiITNexusMicrodeDownload    =
                MICROCODE_DOWNLOAD_VENDOR_SPECIFIC;
        }

        if (allocLength > 0 && allocLength < EXTENDED_INQUIRY_DATA_PAGE_SIZE) {
            StorPortCopyMemory(GET_DATA_BUFFER(pSrb), pExtInqData, allocLength);
        }

        SET_DATA_LENGTH(pSrb,
            min(EXTENDED_INQUIRY_DATA_PAGE_SIZE, allocLength));
    }

    return returnStatus;
} /* SntiTranslateExtendedInquiryDataPage*/

/******************************************************************************
 * SntiTranslateStandardInquiryPage
 *
 * @brief Translates the SCSI Inquiry page - Standard Inquiry Page. Populates
 *        the appropriate SCSI Inqiry response fields based on the NVMe
 *        Translation spec. Do not need to create SQE here as we just complete
 *        the command in the build phase (by returning FALSE to StorPort with
 *        SRB status of SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateStandardInquiryPage(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PINQUIRYDATA pStdInquiry = NULL;
    INQUIRYDATA tmpInquiry;
    UINT16 allocLength;
    UINT8 lastChar = 0;
    UINT8 copyIdx = 0;


    pStdInquiry = (PINQUIRYDATA)GET_DATA_BUFFER(pSrb);
    allocLength = GET_INQ_ALLOC_LENGTH(pSrb);
    if (allocLength < STANDARD_INQUIRY_LENGTH) {
        pStdInquiry = &tmpInquiry;
    }

    pDevExt = ((PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb))->pNvmeDevExt;

    if (allocLength > 0) {
        memset(pStdInquiry, 0, allocLength);
        pStdInquiry->DeviceType          = DIRECT_ACCESS_DEVICE;
        pStdInquiry->DeviceTypeQualifier = DEVICE_CONNECTED;
        pStdInquiry->RemovableMedia      = UNREMOVABLE_MEDIA;
        pStdInquiry->Versions            = VERSION_SPC_4;
        pStdInquiry->NormACA             = ACA_UNSUPPORTED;
        pStdInquiry->HiSupport           = HIERARCHAL_ADDR_UNSUPPORTED;
        pStdInquiry->ResponseDataFormat  = RESPONSE_DATA_FORMAT_SPC_4;
        pStdInquiry->AdditionalLength    = ADDITIONAL_STD_INQ_LENGTH;
        pStdInquiry->EnclosureServices   = EMBEDDED_ENCLOSURE_SERVICES_UNSUPPORTED;
        pStdInquiry->MediumChanger       = MEDIUM_CHANGER_UNSUPPORTED;
        pStdInquiry->CommandQueue        = COMMAND_MANAGEMENT_MODEL;
        pStdInquiry->Wide16Bit           = WIDE_16_BIT_XFERS_UNSUPPORTED;
        pStdInquiry->Addr16              = WIDE_16_BIT_ADDRESES_UNSUPPORTED;
        pStdInquiry->Synchronous         = SYNCHRONOUS_DATA_XFERS_UNSUPPORTED;
        pStdInquiry->Reserved3[0]        = RESERVED_FIELD;

        /*
         *  Fields not defined in Standard Inquiry page from storport.h
         *
         *    - SCCS:    Embedded Storage Arrays
         *    - ACC:     Access Control Coordinator
         *    - TPGS:    Target Port Groupo Suppport
         *    - 3PC:     3rd Party Copy
         *    - Protect: LUN Protection Information
         *    - SPT:     Type of protection LUN supports
         */

        /* T10 Vendor Id */
        pStdInquiry->VendorId[BYTE_0] = 'N';
        pStdInquiry->VendorId[BYTE_1] = 'V';
        pStdInquiry->VendorId[BYTE_2] = 'M';
        pStdInquiry->VendorId[BYTE_3] = 'e';
        pStdInquiry->VendorId[BYTE_4] = ' ';
        pStdInquiry->VendorId[BYTE_5] = ' ';
        pStdInquiry->VendorId[BYTE_6] = ' ';
        pStdInquiry->VendorId[BYTE_7] = ' ';

        /* Product Id - First 16 bytes of model # in Controller Identify structure*/
        StorPortCopyMemory(pStdInquiry->ProductId,
                           pDevExt->controllerIdentifyData.MN,
                           PRODUCT_ID_SIZE);

    /* Find the last valid character in the revision string */
    for (lastChar = 4; lastChar < sizeof(pDevExt->controllerIdentifyData.FR); lastChar++) {
        if ((pDevExt->controllerIdentifyData.FR[lastChar] < 0x21) ||
            (pDevExt->controllerIdentifyData.FR[lastChar] > 0x7e)) {
            break;
        }
    }
    copyIdx = lastChar - 4;

    /* Product Revision Level */
    StorPortCopyMemory(pStdInquiry->ProductRevisionLevel,
        &(pDevExt->controllerIdentifyData.FR[copyIdx]),
        PRODUCT_REVISION_LEVEL_SIZE);
    }

    if (allocLength > 0 && allocLength < STANDARD_INQUIRY_LENGTH) {
        StorPortCopyMemory(GET_DATA_BUFFER(pSrb), pStdInquiry, allocLength);
    }

    SET_DATA_LENGTH(pSrb, min(STANDARD_INQUIRY_LENGTH, allocLength));
} /* SntiTranslateStandardInquiryPage */

/******************************************************************************
 * SntiTranslateReportLuns
 *
 * @brief Translates the SCSI Report LUNs command. Populates the appropriate
 *        SCSI Report LUNs response data based on the NVMe Translation spec. Do
 *        not need to create SQE here as we just complete the command in the
 *        build phase (by returning FALSE to StorPort with SRB status of
 *        SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateReportLuns(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PUCHAR pResponseBuffer = NULL;
    UINT32 numberOfLuns = 0;
    UINT32 numberOfLunsFound = 0;
    UINT32 lunListLength = 0;
    UINT32 allocLength = 0;
    UINT8 lunIdDataOffset = 0;
    UINT8 selectReport = 0;
    UCHAR lunExtIdx = 0;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    UINT8 flbas = 0;
#if (NTDDI_VERSION > NTDDI_WIN7)
    UINT32 lunValue = SrbGetLun((void*)pSrb);
#else
    UINT32 lunValue = pSrb->Lun;
#endif

    /* Default to a successful command completion */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    StorPortDebugPrint(INFO, "SntiTranslateReportLuns: called.\n");

    pDevExt = pSrbExt->pNvmeDevExt;
    pResponseBuffer = (PUCHAR)GET_DATA_BUFFER(pSrb);
    allocLength = GET_REPORT_LUNS_ALLOC_LENGTH(pSrb);
    selectReport = GET_U8_FROM_CDB(pSrb, REPORT_LUNS_SELECT_REPORT_OFFSET);

    /*
     * Set SRB status to success to indicate the command will complete
     * successfully (assuming no errors occur during translation) and
     * reset the status value to use below.
     */
    pSrb->SrbStatus = SRB_STATUS_SUCCESS;

    /* Set the completion routine - no translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    if ((selectReport != LOGICAL_AND_SUBS_LUNS_RET) &&
        (selectReport != ADMIN_AND_LOGICAL_LUNS_RET) &&
        (selectReport != ADMIN_LUNS_RETURNED) &&
        (selectReport != ALL_LUNS_RETURNED) &&
        (selectReport != ALL_WELL_KNOWN_LUNS_RETURNED) &&
        (selectReport != RESTRICTED_LUNS_RETURNED)) {
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);

        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        if (((lunValue != 0) &&
            (selectReport == ADMIN_AND_LOGICAL_LUNS_RET)) ||
            (selectReport == LOGICAL_AND_SUBS_LUNS_RET) ||
            (selectReport == ADMIN_LUNS_RETURNED) ||
            (selectReport == ALL_WELL_KNOWN_LUNS_RETURNED)) {
            numberOfLuns = 0;
        } else {
        /*
         * Per the NVM Express spec, namespaces ids shall be allocated in order
         * (starting with 0) and packed sequentially.
         */
        numberOfLuns = pDevExt->visibleLuns;
        }


        lunListLength = numberOfLuns * LUN_ENTRY_SIZE;

        memset(pResponseBuffer, 0, allocLength);
        lunIdDataOffset = REPORT_LUNS_FIRST_LUN_OFFSET;

        if (numberOfLuns != 0) {
        /* The first LUN Id will always be 0 per the SAM spec */
        for (lunExtIdx = 0; lunExtIdx < MAX_NAMESPACES; lunExtIdx++) {
             pLunExt = pDevExt->pLunExtensionTable[lunExtIdx];
             /*
              * Don't report the LUN when the namespace:
              * (1) with zero size in capacity, or
              * (2) is formatted using metadata, which is not supported now.
              */
             flbas = pLunExt->identifyData.FLBAS.SupportedCombination;
             if ((pLunExt->identifyData.NSZE == 0) || (pLunExt->identifyData.LBAFx[flbas].MS != 0))
                continue;
             if ((pLunExt->slotStatus == ONLINE) &&
                 (++numberOfLunsFound <= numberOfLuns)) {
                 /*
                  * Set the LUN Id and then increment to the next LUN location in
                  * the paramter data (8 byte offset each time),
                  * lunNum position is at byte 1 in the resp buffer per SAM3
                  */
                 #define SINGLE_LVL_LUN_OFFSET (1)
                 if ((UINT32)lunIdDataOffset + SINGLE_LVL_LUN_OFFSET >= allocLength) {
                     break;
                 }
                 pResponseBuffer[lunIdDataOffset + SINGLE_LVL_LUN_OFFSET] = lunExtIdx;
                 lunIdDataOffset += LUN_ENTRY_SIZE;
                }
            }
        }

        /* Set the LUN LIST LENGTH field */
        if (allocLength > BYTE_0) {
            pResponseBuffer[BYTE_0] =
                (UCHAR)((lunListLength & DWORD_MASK_BYTE_3) >> BYTE_SHIFT_3);
        }
        if (allocLength > BYTE_1) {
            pResponseBuffer[BYTE_1] =
                (UCHAR)((lunListLength & DWORD_MASK_BYTE_2) >> BYTE_SHIFT_2);
        }
        if (allocLength > BYTE_2) {
            pResponseBuffer[BYTE_2] =
                (UCHAR)((lunListLength & DWORD_MASK_BYTE_1) >> BYTE_SHIFT_1);
        }
        if (allocLength > BYTE_3) {
            pResponseBuffer[BYTE_3] =
                (UCHAR)((lunListLength & DWORD_MASK_BYTE_0));
        }

        SET_DATA_LENGTH(pSrb,
            min((lunListLength + LUN_DATA_HEADER_SIZE), allocLength));
    }

    return returnStatus;
} /* SntiTranslateReportLuns */
/******************************************************************************
 * SntiTranslateReadCapacity
 *
 * @brief Translates the SCSI Read Capacity command. Populates the appropriate
 *        SCSI Read Capacity parameter data response fields based on the NVMe
 *        Translation spec. Do not need to create SQE here as we just complete
 *        the command in the build phase (by returning FALSE to StorPort with
 *        SRB status of SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateReadCapacity(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PUCHAR pResponseBuffer = NULL;
    SNTI_STATUS status;
    UINT8 opcode;

    /* Default to a successful command completion */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    pResponseBuffer = (PUCHAR)GET_DATA_BUFFER(pSrb);

    StorPortDebugPrint(INFO, "SntiTranslateReadCapacity: called.\n");

    /*
     * Set SRB status to success to indicate the command will complete
     * successfully (assuming no errors occur during translation) and
     * reset the status value to use below.
     */
    pSrb->SrbStatus = SRB_STATUS_SUCCESS;

    /* Set the completion routine - no translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        SntiMapInternalErrorStatus(pSrb, status);
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        opcode = GET_OPCODE(pSrb);
        if (opcode == SCSIOP_READ_CAPACITY) {
            returnStatus = SntiTranslateReadCapacity10(pSrb,
                                                       pResponseBuffer,
                                                       pLunExt);
        } else if (opcode == SCSIOP_READ_CAPACITY16) {
            returnStatus = SntiTranslateReadCapacity16(pSrb,
                                                       pResponseBuffer,
                                                       pLunExt);
        } else {
            ASSERT(FALSE);
        }
    }

    return returnStatus;
} /* SntiTranslateReadCapacity */

/******************************************************************************
 * SntiTranslateReadCapacity10
 *
 * @brief Translates the SCSI Read Capacity 10 command.
 *
 *        NOTE 1: SBC-3 r27 does not define Allocation Length for READ CAP 10
 *        NOTE 2: NVMe/SCSI Translation spec - Returned LBA is 0xFFFFFFFF
 *        NOTE 3: LBA Length in Bytes - Set to LBA Data Size (LBADS) field of
 *                the LBA Format Data structure indicated by the Formatted LBA
 *                Size (FLBAS) field within the Identify Namespace Data
 *                Structure. Bits 0-3 of the FLBAS field indicate is used as an
 *                index into the array at the end of the namespace identify
 *                structure... the array is the types of LBA formats... LBA Data
 *                Size (LBADS) is in terms of power of two (2^n).
 *
 *        SCSI Compliance Testing Note:
 *
 *        For the Microsoft SCSI Compliance Test 2.0, there will be one failed
 *        test case due to revision implementation. This NVM Express driver is
 *        coded to SBC-3 revision 27, but the Microsoft SCSI Compliance Test
 *        suite 2.0 follows the SBC-2 revision 16 spec for a negative test for
 *        READ CAP 10. Because the SCSI complaince test case is testing against
 *        an older revision of the SBC spec, there is a failure that occurs when
 *        the test sends a READ CAP 10 with a PMI bit set to 0 and the LBA is
 *        non zero. The test expects a check condition in this case. However, in
 *        the SBC-3 revision 27 spec, this test scenario is perfecly allowable.
 *        Therefore this driver has been coded according the definition of the
 *        SBC-3 spec. Please note that this failure is a false negative failure
 *        and any future testing may show a failure when, in fact, the code is
 *        correct.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param pResponseBuffer - This parameter is the buffer to which the Read
 *                          Capacity 10 paramter data is written for this
 *                          namespace/LUN.
 * @param pLunExt - This parameter is the LUN Extension pointer that contains
 *                  the namespace identify data structure.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateReadCapacity10(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PUCHAR pResponseBuffer,
    PNVME_LUN_EXTENSION pLunExt
)
{
    PREAD_CAPACITY_DATA pReadCapacityData = NULL;
    READ_CAPACITY_DATA tmpReadCapacityData;
    UINT64 namespaceSize;
    UINT32 lastLba;
    UINT32 lbaLength;
    UINT32 lbaLengthPower;
    UINT8 flbas;
    UINT32 allocLength = GET_DATA_LENGTH(pSrb);

    /* Default to a successful command completion */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;

    /* Use default READ CAP 10 struct provided by StorPort */
    pReadCapacityData = (PREAD_CAPACITY_DATA)pResponseBuffer;
    if (allocLength < READ_CAP_10_PARM_DATA_SIZE) {
        pReadCapacityData = &tmpReadCapacityData;
    }

    /* LBA Length */
    flbas = pLunExt->identifyData.FLBAS.SupportedCombination;
    lbaLengthPower = pLunExt->identifyData.LBAFx[flbas].LBADS;
    lbaLength = 1 << lbaLengthPower;

    if (lbaLength < DEFAULT_SECTOR_SIZE) {
        SntiMapInternalErrorStatus(pSrb, SNTI_FAILURE);
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        if (allocLength > 0) {
            memset(pReadCapacityData, 0, sizeof(READ_CAPACITY_DATA));

            /*
             * NOTE: SCSI Compliance Suite 2.0 incorrect failure happens since
             *       there is no checking of the PMI bit and LBA. Older revisions
             *       of the SBC spec (i.e. SBC-2 r16) support PMI, however, SBC-3
             *       r27, obsoletes PMI and checking for PMI. Refer to comments.
             */

            /*
             * Last LBA - If the NSZE is greater than a DWORD, set to all F's...
             *
             * From SBC-3 r27:
             *
             *   If the RETURNED LOGICAL BLOCK ADDRESS field is set to FFFF_FFFFh,
             *   then the application client should issue a READ CAPACITY (16)
             *   command (see 5.16) to request that the device server transfer the
             *   READ CAPACITY (16) parameter data to the data-in buffer.
             */
            namespaceSize = pLunExt->identifyData.NSZE;

            if ((namespaceSize & UPPER_DWORD_BIT_MASK) != 0)
                lastLba = LBA_MASK_LOWER_32_BITS;
            else
                lastLba = (UINT32)namespaceSize - 1; /* NSZE is not zero based */


            StorPortDebugPrint(INFO, "ReadCapacity10: LastLBA=0x%x, LBALen=%d.\n",
                lastLba, lbaLength);

            /* Must byte swap these as they are returned in big endian */
            REVERSE_BYTES(&pReadCapacityData->LogicalBlockAddress, &lastLba);
            REVERSE_BYTES(&pReadCapacityData->BytesPerBlock, &lbaLength);
        }
        if (allocLength > 0 && allocLength < READ_CAP_10_PARM_DATA_SIZE) {
            StorPortCopyMemory(pResponseBuffer, pReadCapacityData, allocLength);
        }

        SET_DATA_LENGTH(pSrb, min(allocLength, READ_CAP_10_PARM_DATA_SIZE));
    }

    return returnStatus;
} /* SntiTranslateReadCapacity10 */

/******************************************************************************
 * SntiTranslateReadCapacity16
 *
 * @brief Translates the SCSI Read Capacity 16 command.
 *
 *        NOTE 1: NVMe/SCSI Translation spec - Returned LBA is 0xFFFFFFFF
 *        NOTE 2: LBA Length in Bytes - Set to LBA Data Size (LBADS) field of
 *                the LBA Format Data structure indicated by the Formatted LBA
 *                Size (FLBAS) field within the Identify Namespace Data
 *                Structure. Bits 0-3 of the FLBAS field indicate is used as an
 *                index into the array at the end of the namespace identify
 *                structure... the array is the types of LBA formats... LBA Data
 *                Size (LBADS) is in terms of power of two (2^n).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param pResponseBuffer - This parameter is the buffer to which the Read
 *                          Capacity 10 paramter data is written for this
 *                          namespace/LUN.
 * @param pLunExt - This parameter is the LUN Extension pointer that
 *                  contains the namespace identify data structure.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateReadCapacity16(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PUCHAR pResponseBuffer,
    PNVME_LUN_EXTENSION pLunExt
)
{
    PREAD_CAPACITY_16_DATA pReadCapacityData = NULL;
    READ_CAPACITY_16_DATA tmpReadCapacity;
    UINT64 lastLba;
    UINT32 lbaLength;
    UINT32 allocLength;
    UINT32 lbaLengthPower;
    UINT8  flbas;
    UINT8  dps;
    UINT8  protectionType;
    UINT8  protectionEnabled;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PUCHAR pCdb = (PUCHAR)SrbGetCdb((void*)pSrb);
#else
    PUCHAR pCdb = (PUCHAR)pSrb->Cdb;
#endif
    /* Default to a successful command completion */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;

    pReadCapacityData = (PREAD_CAPACITY_16_DATA)pResponseBuffer;
    allocLength = GET_READ_CAP_16_ALLOC_LENGTH(pSrb);
    if (allocLength < READ_CAP_16_PARM_DATA_SIZE) {
        pReadCapacityData = &tmpReadCapacity;
    }

    /*
     * Need to ensure the Service Action is programmed as SERVICE ACTION IN first.
     * If not, return CHECK CONDITION, illegal request invalid field in CDB, etc.
     */
    pCdb++;
    if (*pCdb != READ_CAP_16_SERVICE_ACTION_IN) {
        StorPortDebugPrint(INFO, 
                           "SNTI: ReadCapacity16, UNSUPPORTED Service Action - 0x%02x\n",
                           *pCdb);

        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);
        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);
        return SNTI_UNSUPPORTED_SCSI_REQUEST;
    }

    flbas = pLunExt->identifyData.FLBAS.SupportedCombination;
    lbaLengthPower = pLunExt->identifyData.LBAFx[flbas].LBADS;
    lbaLength = 1 << lbaLengthPower;

    if (lbaLength < DEFAULT_SECTOR_SIZE) {
        SntiMapInternalErrorStatus(pSrb, SNTI_FAILURE);
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        if (allocLength > 0) {
            /* Zero out the area for the response data */
            memset(pReadCapacityData, 0, allocLength);

            /* Get the Data Protection Settings (DPS) */
            dps = pLunExt->identifyData.DPS.ProtectionEnabled;
            lastLba = pLunExt->identifyData.NSZE - 1; /* NSZE is not zero based */

            if (!dps) {
                /* If the DPS settings are 0, then protection is disabled */
                protectionEnabled = PROTECTION_DISABLED;
                protectionType = UNSPECIFIED;
            }
            else {
                protectionEnabled = PROTECTION_ENABLED;
                switch (dps) {
                case 1:
                    /* 000b - NVMe translation spec (6.4 - Table 6-16) */
                    protectionType = 0;
                    break;
                case 2:
                    /* 001b - NVMe translation spec (6.4 - Table 6-16) */
                    protectionType = 1;
                    break;
                case 3:
                    /* 010b - NVMe translation spec (6.4 - Table 6-16) */
                    protectionType = 2;
                    break;
                default:
                    /* Undefined - NVMe translation spec (6.4 - Table 6-16) */
                    protectionType = 0;
                    break;
                }; /* end switch */
            }

            /* Create the response data in a local buffer */
            /* Must byte swap these as they are returned in big endian */
            REVERSE_BYTES_QUAD(&pReadCapacityData->LogicalBlockAddress, &lastLba);
            REVERSE_BYTES(&pReadCapacityData->BytesPerBlock, &lbaLength);

            pReadCapacityData->ProtectionType = protectionType;
            pReadCapacityData->ProtectionEnable = protectionEnabled;
            pReadCapacityData->ProtectionInfoIntervals = UNSPECIFIED;

            pReadCapacityData->LogicalBlocksPerPhysicalBlockExponent =
                ONE_OR_MORE_PHYSICAL_BLOCKS;

            pReadCapacityData->LogicalBlockProvisioningMgmtEnabled = UNSPECIFIED;
            pReadCapacityData->LogicalBlockProvisioningReadZeros = SPECIFIED;
            pReadCapacityData->LowestAlignedLbaMsb = LBA_0;
            pReadCapacityData->LowestAlignedLbaLsb = LBA_0;
        }
        if (allocLength > 0 && allocLength < READ_CAP_16_PARM_DATA_SIZE) {
            StorPortCopyMemory(pResponseBuffer, pReadCapacityData, allocLength);
        }
        SET_DATA_LENGTH(pSrb, min(allocLength, READ_CAP_16_PARM_DATA_SIZE));
    }

    return returnStatus;
} /* SntiTranslateReadCapacity16 */

/******************************************************************************
 * SntiTranslateWrite
 *
 * @brief Translates the SCSI Write command based on the NVMe Translation spec
 *        and populates a temporary SQE stored in the SRB Extension.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateWrite(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PSTOR_SCATTER_GATHER_LIST pSgl = NULL;
    SNTI_STATUS status;
    UINT8 opcode;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    pDevExt = pSrbExt->pNvmeDevExt;

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        /* Map the translation error to a SCSI error */
        SntiMapInternalErrorStatus(pSrb, status);
        return SNTI_FAILURE_CHECK_RESPONSE_DATA;
    }

    pSgl = StorPortGetScatterGatherList(pSrbExt->pNvmeDevExt, 
                                       (PSCSI_REQUEST_BLOCK)pSrb);

    /* Set the SRB status to pending - controller communication necessary */
    pSrb->SrbStatus = SRB_STATUS_PENDING;

    /* Set the completion routine - no translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    /* Zero out the command entry */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));

    pSrbExt->nvmeSqeUnit.CDW0.OPC = NVME_WRITE;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;
    pSrbExt->nvmeSqeUnit.NSID = pLunExt->namespaceId;

    /* TBD: Contiguous physical buffer of metadata (DWORD alinged) */
    pSrbExt->nvmeSqeUnit.MPTR = 0;

    /* PRP Entry/List */
    SntiTranslateSglToPrp(pSrbExt, pSgl);

    /* Complete the non-common translation fields for the command */
    opcode = GET_OPCODE(pSrb);
    switch (opcode) {
        case SCSIOP_WRITE6:
            status = SntiTranslateWrite6(pSrbExt, pLunExt);
        break;
        case SCSIOP_WRITE:
            status = SntiTranslateWrite10(pSrbExt, pLunExt);
        break;
        case SCSIOP_WRITE12:
            status = SntiTranslateWrite12(pSrbExt, pLunExt);
        break;
        case SCSIOP_WRITE16:
            status = SntiTranslateWrite16(pSrbExt, pLunExt);
        break;
        default:
            ASSERT(FALSE);
            status = SNTI_FAILURE;
        break;
    }; /* end switch */

    /* 
     * When number of LBAs is zero in Read/Write Commands (except Read6/Write6),
     * complete the request successfully and immediately
     */
    if (status == SNTI_COMPLETE_CMD)
        return SNTI_COMMAND_COMPLETED;

    if (status != SNTI_SUCCESS)
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;

    return returnStatus;
} /* SntiTranslateWrite */

/******************************************************************************
 * SntiTranslateWrite6
 *
 * @brief Translates the SCSI Write 6 command to an NVMe WRITE command.
 *        NOTE: FUA is not defined for WRITE 6
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 *
 * @return SNTI_STATUS
 *     Indicates internal translation status
 ******************************************************************************/
SNTI_STATUS SntiTranslateWrite6(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
)
{
    SNTI_STATUS status = SNTI_SUCCESS;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    UINT32 lba = 0;
    UINT16 length = 0;

    lba = GET_U24_FROM_CDB(pSrb, WRITE_6_CDB_LBA_OFFSET);
    length = GET_U8_FROM_CDB(pSrb, WRITE_6_CDB_TX_LEN_OFFSET);

    /* For Read6/Write6, it means 256 LBAs when length is 0 */
    if (length == 0)
        length = READ_WRITE_6_MAX_LBA;

    /* Mask off the unnecessary bits and validate the LBA range */
    lba &= WRITE_6_CDB_LBA_MASK;

    status = SntiValidateLbaAndLength(pLunExt,
                                      pSrbExt,
                                      (UINT64)lba,
                                      (UINT32)length);

    if (status == SNTI_SUCCESS) {
        /* Command DWORD 10/11 - Starting LBA */
        pSrbExt->nvmeSqeUnit.CDW10 = lba;
        pSrbExt->nvmeSqeUnit.CDW11 = 0;

        /* Command DWORD 12 - LR/FUA/PRINFO/NLB */
        pSrbExt->nvmeSqeUnit.CDW12 |= FUA_ENABLED;
        pSrbExt->nvmeSqeUnit.CDW12 |= length - 1; /* 0's based */
    }

    return status;
} /* SntiTranslateWrite6 */

/******************************************************************************
 * SntiTranslateWrite10
 *
 * @brief Translates the SCSI Write 10 command to an NVMe WRITE command.
 *        NOTE: The following Write CDB fields are defined as "unspecified" or
 *        "unsupported" in the SCSI/NVMe Translation specification:
 *
 *        - DPO (Disable Page Out)
 *        - FUA_NV (Force Unit Access from Non-Volatile Memory)
 *        - Group Number
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 *
 * @return SNTI_STATUS
 *     Indicates internal translation status
 ******************************************************************************/
SNTI_STATUS SntiTranslateWrite10(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
)
{
    SNTI_STATUS status = SNTI_SUCCESS;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    UINT32 lba = 0;
    UINT16 length = 0;
    UINT8 fua = 0;

    lba = GET_U32_FROM_CDB(pSrb, WRITE_10_CDB_LBA_OFFSET);
    length = GET_U16_FROM_CDB(pSrb, WRITE_10_CDB_TX_LEN_OFFSET);
    fua = GET_U8_FROM_CDB(pSrb, WRITE_CDB_FUA_OFFSET);
    fua &= WRITE_CDB_FUA_MASK;

    /* When number of LBAs is zero, complete the request right away */
    if (length == 0)
        status = SNTI_COMPLETE_CMD;
    else
        status = SntiValidateLbaAndLength(pLunExt,
                                          pSrbExt,
                                          (UINT64)lba,
                                          (UINT32)length);

    if (status == SNTI_SUCCESS) {
        /* Command DWORD 10/11 - Starting LBA */
        pSrbExt->nvmeSqeUnit.CDW10 = lba;
        pSrbExt->nvmeSqeUnit.CDW11 = 0;

        /* Command DWORD 12 - LR/FUA/PRINFO/NLB */
        pSrbExt->nvmeSqeUnit.CDW12 |= (fua ? FUA_ENABLED : FUA_DISABLED);
        pSrbExt->nvmeSqeUnit.CDW12 |= length - 1; /* 0's based */
    }

    return status;
} /* SntiTranslateWrite10 */

/******************************************************************************
 * SntiTranslateWrite12
 *
 * @brief Translates the SCSI Write 12 command to an NVMe WRITE command.
 *        NOTE: The following Write CDB fields are defined as "unspecified" or
 *        "unsupported" in the SCSI/NVMe Translation specification:
 *
 *        - DPO (Disable Page Out)
 *        - FUA_NV (Force Unit Access from Non-Volatile Memory)
 *        - Group Number
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 *
 * @return SNTI_STATUS
 *     Indicates internal translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateWrite12(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
)
{
    SNTI_STATUS status = SNTI_SUCCESS;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    UINT32 lba = 0;
    UINT32 length = 0;
    UINT8 fua = 0;

    lba = GET_U32_FROM_CDB(pSrb, WRITE_12_CDB_LBA_OFFSET);
    length = GET_U32_FROM_CDB(pSrb, WRITE_12_CDB_TX_LEN_OFFSET);
    fua = GET_U8_FROM_CDB(pSrb, WRITE_CDB_FUA_OFFSET);
    fua &= WRITE_CDB_FUA_MASK;

    /* When number of LBAs is zero, complete the request right away */
    if (length == 0)
        status = SNTI_COMPLETE_CMD;
    else
        status = SntiValidateLbaAndLength(pLunExt,
                                          pSrbExt,
                                          (UINT64)lba,
                                          (UINT32)length);

    if (status == SNTI_SUCCESS) {
        /* Command DWORD 10/11 - Starting LBA */
        pSrbExt->nvmeSqeUnit.CDW10 = lba;
        pSrbExt->nvmeSqeUnit.CDW11 = 0;

        /* Command DWORD 12 - LR/FUA/PRINFO/NLB */
        pSrbExt->nvmeSqeUnit.CDW12 |= (fua ? FUA_ENABLED : FUA_DISABLED);
        pSrbExt->nvmeSqeUnit.CDW12 |= (length & DWORD_BIT_MASK) - 1; /* 0's based */
    }

    return status;
} /* SntiTranslateWrite12 */

/******************************************************************************
 * SntiTranslateWrite16
 *
 * @brief Translates the SCSI Write 16 command to an NVMe WRITE command.
 *        NOTE: The following Write CDB fields are defined as "unspecified" or
 *        "unsupported" in the SCSI/NVMe Translation specification:
 *
 *        - DPO (Disable Page Out)
 *        - FUA_NV (Force Unit Access from Non-Volatile Memory)
 *        - Group Number
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 *
 * @return SNTI_STATUS
 *     Indicates internal translation status.
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateWrite16(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
)
{
    SNTI_STATUS status = SNTI_SUCCESS;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    UINT64 lba = 0;
    UINT32 length = 0;
    UINT8 fua = 0;

    lba = (UINT64)
        ((((UINT64)(GET_U32_FROM_CDB(pSrb, WRITE_16_CDB_LBA_OFFSET + 0)))
          << DWORD_SHIFT_MASK) |
         (((UINT64)(GET_U32_FROM_CDB(pSrb, WRITE_16_CDB_LBA_OFFSET + 4)))
          & DWORD_BIT_MASK));

    length = GET_U32_FROM_CDB(pSrb, WRITE_16_CDB_TX_LEN_OFFSET);
    fua = GET_U8_FROM_CDB(pSrb, WRITE_CDB_FUA_OFFSET);
    fua &= WRITE_CDB_FUA_MASK;

    /* When number of LBAs is zero, complete the request right away */
    if (length == 0)
        status = SNTI_COMPLETE_CMD;
    else
        status = SntiValidateLbaAndLength(pLunExt,
                                          pSrbExt,
                                          (UINT64)lba,
                                          (UINT32)length);

    if (status == SNTI_SUCCESS) {
        /* Command DWORD 10/11 - Starting LBA */
        pSrbExt->nvmeSqeUnit.CDW10 = (UINT32)(lba & DWORD_BIT_MASK);
        pSrbExt->nvmeSqeUnit.CDW11 = (UINT32)(lba >> DWORD_SHIFT_MASK);

        /* Command DWORD 12 - LR/FUA/PRINFO/NLB */
        pSrbExt->nvmeSqeUnit.CDW12 |= (fua ? FUA_ENABLED : FUA_DISABLED);
        pSrbExt->nvmeSqeUnit.CDW12 |= (length & DWORD_BIT_MASK) - 1; /* 0's based */
    }

    return status;
} /* SntiTranslateWrite16 */

/******************************************************************************
 * SntiTranslateRead
 *
 * @brief Translates the SCSI Read command based on the NVMe Translation spec
 *        and populates a temporary SQE stored in the SRB Extension.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateRead(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PSTOR_SCATTER_GATHER_LIST pSgl = NULL;
    SNTI_STATUS status;
    UINT8 opcode;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    pDevExt = pSrbExt->pNvmeDevExt;

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        /* Map the translation error to a SCSI error */
        SntiMapInternalErrorStatus(pSrb, status);
        return SNTI_FAILURE_CHECK_RESPONSE_DATA;
    }

    pSgl = StorPortGetScatterGatherList(pSrbExt->pNvmeDevExt, 
                                       (PSCSI_REQUEST_BLOCK)pSrb);

    /* Set the SRB status to pending - controller communication necessary */
    pSrb->SrbStatus = SRB_STATUS_PENDING;
    pSrbExt->pNvmeCompletionRoutine = NULL;

    /* Zero out the command entry */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));

    pSrbExt->nvmeSqeUnit.CDW0.OPC = NVME_READ;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;
    pSrbExt->nvmeSqeUnit.NSID = pLunExt->namespaceId;

    /* TBD: Contiguous physical buffer of metadata (DWORD alinged) */
    pSrbExt->nvmeSqeUnit.MPTR = 0;

    /* PRP Entry/List */
    SntiTranslateSglToPrp(pSrbExt, pSgl);

    /* Complete the non-common translation fields for the command */
    opcode = GET_OPCODE(pSrb);
    switch (opcode) {
        case SCSIOP_READ6:
            status = SntiTranslateRead6(pSrbExt, pLunExt);
        break;
        case SCSIOP_READ:
            status = SntiTranslateRead10(pSrbExt, pLunExt);
        break;
        case SCSIOP_READ12:
            status = SntiTranslateRead12(pSrbExt, pLunExt);
        break;
        case SCSIOP_READ16:
            status = SntiTranslateRead16(pSrbExt, pLunExt);
        break;
        default:
            ASSERT(FALSE);
            status = SNTI_FAILURE;
        break;
    }; /* end switch */

    /* 
     * When number of LBAs is zero in Read/Write Commands (except Read6/Write6),
     * complete the request successfully and immediately
     */
    if (status == SNTI_COMPLETE_CMD)
        return SNTI_COMMAND_COMPLETED;

    if (status != SNTI_SUCCESS)
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;

    return returnStatus;
} /* SntiTranslateRead */

/******************************************************************************
 * SntiTranslateRead6
 *
 * @brief Translates the SCSI READ 6 command to an NVMe READ command.
 *        NOTE: FUA is not defined for READ 6
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 *
 * @return SNTI_STATUS
 *     Indicates internal translation status
 ******************************************************************************/
SNTI_STATUS SntiTranslateRead6(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
)
{
    SNTI_STATUS status = SNTI_SUCCESS;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    UINT32 lba = 0;
    UINT16 length = 0;

    lba = GET_U24_FROM_CDB(pSrb, READ_6_CDB_LBA_OFFSET);
    length = GET_U8_FROM_CDB(pSrb, READ_6_CDB_TX_LEN_OFFSET);

    /* For Read6/Write6, it means 256 LBAs when length is 0 */
    if (length == 0)
        length = READ_WRITE_6_MAX_LBA;

    /* Mask off the unnecessary bits and validate the LBA range */
    lba &= READ_6_CDB_LBA_MASK;

    status = SntiValidateLbaAndLength(pLunExt,
                                      pSrbExt,
                                      (UINT64)lba,
                                      (UINT32)length);

    if (status == SNTI_SUCCESS) {
        /* Command DWORD 10/11 - Starting LBA */
        pSrbExt->nvmeSqeUnit.CDW10 = lba;
        pSrbExt->nvmeSqeUnit.CDW11 = 0;

        /* Command DWORD 12 - LR/FUA/PRINFO/NLB */
        pSrbExt->nvmeSqeUnit.CDW12 |= FUA_ENABLED;
        pSrbExt->nvmeSqeUnit.CDW12 |= length - 1; /* 0's based */
    }

    return status;
} /* SntiTranslateRead6 */

/******************************************************************************
 * SntiTranslateRead10
 *
 * @brief Translates the SCSI Read 10 command to an NVMe READ command.
 *        NOTE: The following Read CDB fields are defined as "unspecified" or
 *        "unsupported" in the SCSI/NVMe Translation specification:
 *
 *        - DPO (Disable Page Out)
 *        - FUA_NV (Force Unit Access from Non-Volatile Memory)
 *        - Group Number
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 *
 * @return SNTI_STATUS
 *     Indicates internal translation status
 ******************************************************************************/
SNTI_STATUS SntiTranslateRead10(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
)
{
    SNTI_STATUS status = SNTI_SUCCESS;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    UINT32 lba = 0;
    UINT16 length = 0;
    UINT8 fua = 0;

    lba = GET_U32_FROM_CDB(pSrb, READ_10_CDB_LBA_OFFSET);
    length = GET_U16_FROM_CDB(pSrb, READ_10_CDB_TX_LEN_OFFSET);
    fua = GET_U8_FROM_CDB(pSrb, READ_CDB_FUA_OFFSET);
    fua &= READ_CDB_FUA_MASK;

    /* When number of LBAs is zero, complete the request right away */
    if (length == 0)
        status = SNTI_COMPLETE_CMD;
    else
        status = SntiValidateLbaAndLength(pLunExt,
                                          pSrbExt,
                                          (UINT64)lba,
                                          (UINT32)length);

    if (status == SNTI_SUCCESS) {
        /* Command DWORD 10/11 - Starting LBA */
        pSrbExt->nvmeSqeUnit.CDW10 = lba;
        pSrbExt->nvmeSqeUnit.CDW11 = 0;

        /* Command DWORD 12 - LR/FUA/PRINFO/NLB */
        pSrbExt->nvmeSqeUnit.CDW12 |= (fua ? FUA_ENABLED : FUA_DISABLED);
        pSrbExt->nvmeSqeUnit.CDW12 |= length - 1; /* 0's based */
    }

    return status;
} /* SntiTranslateRead10 */

/******************************************************************************
 * SntiTranslateRead12
 *
 * @brief Translates the SCSI Read 12 command to an NVMe READ command.
 *        NOTE: The following Read CDB fields are defined as "unspecified" or
 *        "unsupported" in the SCSI/NVMe Translation specification:
 *
 *        - DPO (Disable Page Out)
 *        - FUA_NV (Force Unit Access from Non-Volatile Memory)
 *        - Group Number
 *
 * @param pSrbExt - Pointer to SRB extension
 *
 * @param pLunExt - Pointer to LUN extension
 *
 * @return SNTI_STATUS
 *     Indicates internal translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateRead12(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
)
{
    SNTI_STATUS status = SNTI_SUCCESS;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    UINT32 lba = 0;
    UINT32 length = 0;
    UINT8 fua = 0;

    lba = GET_U32_FROM_CDB(pSrb, READ_12_CDB_LBA_OFFSET);
    length = GET_U32_FROM_CDB(pSrb, READ_12_CDB_TX_LEN_OFFSET);
    fua = GET_U8_FROM_CDB(pSrb, READ_CDB_FUA_OFFSET);
    fua &= READ_CDB_FUA_MASK;

    /* When number of LBAs is zero, complete the request right away */
    if (length == 0)
        status = SNTI_COMPLETE_CMD;
    else
        status = SntiValidateLbaAndLength(pLunExt,
                                          pSrbExt,
                                          (UINT64)lba,
                                          (UINT32)length);

    if (status == SNTI_SUCCESS) {
        /* Command DWORD 10/11 - Starting LBA */
        pSrbExt->nvmeSqeUnit.CDW10 = lba;
        pSrbExt->nvmeSqeUnit.CDW11 = 0;

        /* Command DWORD 12 - LR/FUA/PRINFO/NLB */
        pSrbExt->nvmeSqeUnit.CDW12 |= (fua ? FUA_ENABLED : FUA_DISABLED);

        /* NVMe Translation Spec ERRATA... NLB is only 16 bits!!! */
        pSrbExt->nvmeSqeUnit.CDW12 |= (length & DWORD_BIT_MASK) - 1; /* 0's based */

    }

    return status;
} /* SntiTranslateRead12 */

/******************************************************************************
 * SntiTranslateRead16
 *
 * @brief Translates the SCSI Read 16 command to an NVMe READ command.
 *        NOTE: The following Read CDB fields are defined as "unspecified" or
 *        "unsupported" in the SCSI/NVMe Translation specification:
 *
 *        - DPO (Disable Page Out)
 *        - FUA_NV (Force Unit Access from Non-Volatile Memory)
 *        - Group Number
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 *
 * @return SNTI_STATUS
 *     Indicates internal translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateRead16(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
)
{
    SNTI_STATUS status = SNTI_SUCCESS;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    UINT64 lba = 0;
    UINT32 length = 0;
    UINT8 fua = 0;

    lba = (UINT64)
        ((((UINT64)(GET_U32_FROM_CDB(pSrb, READ_16_CDB_LBA_OFFSET + 0)))
          << DWORD_SHIFT_MASK) |
         (((UINT64)(GET_U32_FROM_CDB(pSrb, READ_16_CDB_LBA_OFFSET + 4)))
          & DWORD_BIT_MASK));

    length = GET_U32_FROM_CDB(pSrb, READ_16_CDB_TX_LEN_OFFSET);
    fua = GET_U8_FROM_CDB(pSrb, READ_CDB_FUA_OFFSET);
    fua &= READ_CDB_FUA_MASK;

    /* When number of LBAs is zero, complete the request right away */
    if (length == 0)
        status = SNTI_COMPLETE_CMD;
    else
        status = SntiValidateLbaAndLength(pLunExt,
                                          pSrbExt,
                                          (UINT64)lba,
                                          (UINT32)length);

    if (status == SNTI_SUCCESS) {
        /* Command DWORD 10/11 - Starting LBA */
        pSrbExt->nvmeSqeUnit.CDW10 = (UINT32)(lba & DWORD_BIT_MASK);
        pSrbExt->nvmeSqeUnit.CDW11 = (UINT32)(lba >> DWORD_SHIFT_MASK);

        /* Command DWORD 12 - LR/FUA/PRINFO/NLB */
        pSrbExt->nvmeSqeUnit.CDW12 |= (fua ? FUA_ENABLED : FUA_DISABLED);
        pSrbExt->nvmeSqeUnit.CDW12 |= (length & DWORD_BIT_MASK) - 1; /* 0's based */
    }

    return status;
} /* SntiTranslateRead16 */

/******************************************************************************
 * SntiTranslateRequestSense
 *
 * @brief Translates the SCSI Request Sense command. Populates the appropriate
 *        SCSI Request Sense response data based on the NVMe Translation spec.
 *        Do not need to create an SQE here as we just complete the command in
 *        the build phase (by returning FALSE to StorPort with SRB status of
 *        SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateRequestSense(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    UINT32 allocLength;
    UINT8 descFormat;

    /* Default to command completed */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    pDevExt = pSrbExt->pNvmeDevExt;
    allocLength = GET_REQUEST_SENSE_ALLOC_LENGTH(pSrb);
    descFormat = GET_U8_FROM_CDB(pSrb, REQUEST_SENSE_DESCRIPTOR_FORMAT_OFFSET);

    descFormat &= REQUEST_SENSE_DESCRIPTOR_FORMAT_MASK;

    /*
     * Set SRB status to success to indicate the command will complete
     * successfully (assuming no errors occur during translation) and
     * reset the status value to use below.
     */
    pSrb->SrbStatus = SRB_STATUS_SUCCESS;

    /* Set the completion routine - no translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    /*
     * NOTE: SCSI to NVMe translation spec statest that DESC bit must only be
     *       set to 1. MS SCSI Compliance Suite will send a Request Sense cmd
     *       with this field set to 0 and expect a good status.
     */
    if (descFormat == DESCRIPTOR_FORMAT_SENSE_DATA_TYPE) {
        /* Descriptor Format Sense Data */
        PDESCRIPTOR_FORMAT_SENSE_DATA pSenseData = NULL;
        DESCRIPTOR_FORMAT_SENSE_DATA tmpSenseData;
        pSenseData = (PDESCRIPTOR_FORMAT_SENSE_DATA)GET_DATA_BUFFER(pSrb);
        if (allocLength < sizeof(DESCRIPTOR_FORMAT_SENSE_DATA)) {
            pSenseData = &tmpSenseData;
        }
        StorPortDebugPrint(INFO, "SntiTranslateRequestSense: Desc Fmt buffer@ 0x%p, len=%d\n",
            pSenseData, allocLength);
        if (pSenseData) {
            memset(pSenseData, 0, allocLength);
            pSenseData->ResponseCode                 = DESC_FORMAT_SENSE_DATA;
            pSenseData->SenseKey                     = SCSI_SENSE_NO_SENSE;
            pSenseData->AdditionalSenseCode          = SCSI_ADSENSE_NO_SENSE;
            pSenseData->AdditionalSenseCodeQualifier = 0;
            pSenseData->AdditionalSenseLength        = 0;
        }
        if (allocLength > 0 && allocLength < sizeof(DESCRIPTOR_FORMAT_SENSE_DATA)) {
            StorPortCopyMemory(GET_DATA_BUFFER(pSrb), pSenseData, allocLength);
        }
        SET_DATA_LENGTH(pSrb,
            min(sizeof(DESCRIPTOR_FORMAT_SENSE_DATA), allocLength));
    } else {
        /* Fixed Format Sense Data */
        PSENSE_DATA pSenseData = NULL;
        SENSE_DATA tmpSenseData;
        pSenseData = (PSENSE_DATA)GET_DATA_BUFFER(pSrb);
        if (allocLength < sizeof(SENSE_DATA)) {
            pSenseData = &tmpSenseData;
        }
        StorPortDebugPrint(INFO, "SntiTranslateRequestSense: Fixed Fmt buffer@ 0x%p, len=%d\n", 
            pSenseData, allocLength);
        if (pSenseData) {
            memset(pSenseData, 0, allocLength);
            pSenseData->ErrorCode                    = FIXED_SENSE_DATA;
            pSenseData->SenseKey                     = SCSI_SENSE_NO_SENSE;
            pSenseData->AdditionalSenseLength        = FIXED_SENSE_DATA_ADD_LENGTH;
            pSenseData->AdditionalSenseCode          = SCSI_ADSENSE_NO_SENSE;
            pSenseData->AdditionalSenseCodeQualifier = 0;
        }
        if (allocLength > 0 && allocLength < sizeof(SENSE_DATA)) {
            StorPortCopyMemory(GET_DATA_BUFFER(pSrb), pSenseData, allocLength);
        }
        SET_DATA_LENGTH(pSrb, min(sizeof(SENSE_DATA), allocLength));
    }

    return returnStatus;
} /* SntiTranslateRequestSense */

/******************************************************************************
 * SntiTranslateSecurityProtocol
 *
 * @brief Translates the SCSI Security Protocol In/Out commands based on the
 *        NVMe Translation spec to a NVMe Security Receive/Send command and
 *        populates a temporary SQE stored in the SRB Extension.
 *
 *        NOTE: From the SPC-4 r.24 specification:
 *
 *        The device server shall retain data resulting from a SECURITY PROTOCOL
 *        OUT command, if any, until one of the following events is processed:
 *
 *           a) transfer of the data via a SECURITY PROTOCOL IN command from
 *              the same I_T_L nexus as defined by the protocol specified by
 *              the SECURITY PROTOCOL field (see table 259);
 *           b) logical unit reset (See SAM-4); or
 *           c) I_T nexus loss (See SAM-4) associated with the I_T nexus that
 *              sent the SECURITY PROTOCOL OUT command.
 *
 *        If the data is lost due to one of these events the application client
 *        may send a new SECURITY PROTOCOL OUT command to retry the operation.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateSecurityProtocol(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    UINT32 length;
    UINT16 secProtocolSp;
    UINT8 secProtocol;
    UINT8 inc_512;
    UINT8 opcode;
    SNTI_STATUS status;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    length = GET_U32_FROM_CDB(pSrb, SECURITY_PROTOCOL_CDB_LENGTH_OFFSET);
    secProtocol = GET_U8_FROM_CDB(pSrb, SECURITY_PROTOCOL_CDB_SEC_PROT_OFFSET);
    secProtocolSp =
        GET_U16_FROM_CDB(pSrb, SECURITY_PROTOCOL_CDB_SEC_PROT_SP_OFFSET);
    inc_512 = GET_U8_FROM_CDB(pSrb, SECURITY_PROTOCOL_CDB_INC_512_OFFSET);
    inc_512 &= SECURITY_PROTOCOL_CDB_INC_512_MASK;

    if ((inc_512 >> SECURITY_PROTOCOL_CDB_INC_512_SHIFT) != 0) {
        /* Ensure correct sense data for SCSI compliance test case 1.4 */
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);

        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        status = GetLunExtension(pSrbExt, &pLunExt);
        if (status != SNTI_SUCCESS) {
            SntiMapInternalErrorStatus(pSrb, status);
            returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
        } else {
            /* Set SRB status to pending - controller communication necessary */
            pSrb->SrbStatus = SRB_STATUS_PENDING;

            /* Set completion routine, no translation necessary on completion */
            pSrbExt->pNvmeCompletionRoutine = NULL;

            if (GET_OPCODE(pSrb) == SCSIOP_SECURITY_PROTOCOL_IN)
                opcode = ADMIN_SECURITY_RECEIVE;
            else
                opcode = ADMIN_SECURITY_SEND;

            SntiBuildSecuritySendReceiveCmd(pSrbExt,
                                            pLunExt,
                                            opcode,
                                            length,
                                            secProtocolSp,
                                            secProtocol);
        }
    }

    return returnStatus;
} /* SntiTranslateSecurityProtocol */


/******************************************************************************
* SntiTranslatePersistentReserveIn
*
* @brief Translates the SCSI Persistent Reserve In command based on the NVMe
*        Translation spec to an NVMe Reservation command.
*
* @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
*               that the user can access the SCSI CDB, response, and data from
*               this pointer. For example, if there is a failure in translation
*               resulting in sense data, then SNTI will call the appropriate
*               internal error handling code and set the status info/data and
*               pass the pSrb pointer as a parameter.
*
* @return SNTI_TRANSLATION_STATUS
*     Indicate translation status
******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslatePersistentReserveIn(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
    )
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    UCHAR serviceAction = 0;
    USHORT allocLength = 0;
    SNTI_STATUS status = SNTI_SUCCESS;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pDevExt = pSrbExt->pNvmeDevExt;

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        SntiMapInternalErrorStatus(pSrb, status);
        SET_DATA_LENGTH(pSrb, 0);
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {

        /* Set SRB status to pending - controller communication necessary */
        pSrb->SrbStatus = SRB_STATUS_PENDING;

        /* Set completion routine for translating the response */
        pSrbExt->pNvmeCompletionRoutine = SntiCompletionCallbackRoutine;

        /* Get service action and length */
        serviceAction = GET_U8_FROM_CDB(pSrb, PERSISTENT_RES_CDB_SER_ACTION_OFFSET);
        allocLength = GET_U16_FROM_CDB(pSrb, PERSISTENT_RESIN_CDB_ALLOC_LEN_OFFSET);

        StorPortDebugPrint(INFO,
            "SntiTranslatePersistentReserveIn: Service Action = 0x%02x\n",
            serviceAction);

        switch (serviceAction) {
        case RESERVATION_ACTION_READ_KEYS:
        case RESERVATION_ACTION_READ_RESERVATIONS:
        case RESERVATION_ACTION_READ_FULL_STATUS:
        {
            PUCHAR pNvmeResStatusBuf = NULL;
            PUCHAR pNvmeResStatusBufAligned = NULL;
            PUCHAR pNvmeNextPageBoundary = NULL;
            ULONG bufSize = 0;
            PHYSICAL_ADDRESS physicalAddress = { 0 };

            PNVM_RESERVATION_REPORT_COMMAND_DW10 pCdw10 =
                (PNVM_RESERVATION_REPORT_COMMAND_DW10)&pSrbExt->nvmeSqeUnit.CDW10;

            /* Chose which queue to use */
            pSrbExt->forAdminQueue = FALSE;

            /* Calculate address of the reservation status buffer */
            pNvmeResStatusBuf = &pSrbExt->resReportStruct[0];

            /* ensure it is aligned on a 4-byte boundary */
            pNvmeResStatusBufAligned = (PUCHAR)(((UINT64)pNvmeResStatusBuf + sizeof(ULONG) - 1) &
                ~(sizeof(ULONG) - 1));
            bufSize = sizeof(NVME_RES_REPORT_STRUCT);
            /* Zero out the buffer */
            memset(pNvmeResStatusBufAligned, 0, bufSize);

            /* Zero out the command entry */
            memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));

            pSrbExt->nvmeSqeUnit.CDW0.OPC = NVM_RESERVATION_REPORT;
            pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
            pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

            pSrbExt->nvmeSqeUnit.NSID = pLunExt->namespaceId;

            NVMePreparePRPs(pDevExt, pSrbExt, pNvmeResStatusBufAligned, bufSize);

            /* DWORD 10 - Size of the buffer for Reservation Status data struct, in DWORDS. */
            /* Size of the buffer minus the size of the header */
            pCdw10->NUMD = (bufSize / sizeof(ULONG)) - 1;
        }
        break;
        case RESERVATION_ACTION_REPORT_CAPABILITIES:
            /* Report Capabiliites */

            /* Issue the GET FEATURES command (Reservation Persistance) */
            SntiBuildGetFeaturesCmd(pSrbExt, RESERVATION_PERSISTANCE);
            break;
        default:
            StorPortDebugPrint(INFO,
                "SntiTranslatePersistentReserveIn: Invalid Service Action = 0x%02x\n",
                serviceAction);

            /* reject command. Invalid service action */
            SntiMapInternalErrorStatus(pSrb, SNTI_INVALID_PARAMETER);
            SET_DATA_LENGTH(pSrb, 0);
            returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
            break;
        }
    }

    return returnStatus;

} /* SntiTranslatePersistentReserveIn */


/******************************************************************************
* SntiTranslatePersistentReserveOut
*
* @brief Translates the SCSI Persistent Reserve Out command based on the NVMe
*        Translation spec to an NVMe Reservation command.
*
* @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
*               that the user can access the SCSI CDB, response, and data from
*               this pointer. For example, if there is a failure in translation
*               resulting in sense data, then SNTI will call the appropriate
*               internal error handling code and set the status info/data and
*               pass the pSrb pointer as a parameter.
*
* @return SNTI_TRANSLATION_STATUS
*     Indicate translation status
******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslatePersistentReserveOut(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
    )
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    UCHAR serviceAction = 0;
    UCHAR scsiRType = 0;
    UCHAR nvmeRType = 0;
    UCHAR scope = 0;
    ULONG allocLength = 0;
    PPERSIST_RES_OUT_PARMS pScsiResOutParms = NULL;
    STOR_PHYSICAL_ADDRESS physAddr = { 0 };
    PUCHAR pNvmeNextPageBoundary = NULL;
    USHORT dataStructSize = 0;
    PUCHAR pParmBuffer = NULL;
    SNTI_STATUS status = SNTI_SUCCESS;
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;
    /* Sntl 1.5, table 4-14 */
    UCHAR ScsiToNVMeRTypeConv[RESERVATION_TYPE_MAX_VALUE + 1] = {
        NVME_RTYPE_NO_RESERVATION,          // Scsi Reservation Type 0: No Reservation 
        NVME_RTYPE_WRITE_EXC,               // Scsi Reservation Type 1: Write exclusive
        NVME_RTYPE_RESERVED,                // Invalid Scsi Reservation Type 2
        NVME_RTYPE_EXCLUSIVE_ACC,           // Scsi Reservation Type 3: Exclusive access
        NVME_RTYPE_RESERVED,                // Invalid Scsi Reservation Type 4
        NVME_RTYPE_WRITE_EXC_REG_ONLY,      // Scsi Reservation Type 5: Write exclusive - registrants only
        NVME_RTYPE_EXC_ACC_REG_ONLY,        // Scsi Reservation Type 6: Exclusive Access - registrants only
        NVME_RTYPE_WRITE_EXC_ALL,           // Scsi Reservation Type 7: Write exclusive - all registrants
        NVME_RTYPE_EXC_ACC_ALL              // Scsi Reservation Type 8: Exclusive Access - all registrants
    };

    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pDevExt = pSrbExt->pNvmeDevExt;

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        SntiMapInternalErrorStatus(pSrb, status);
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        pScsiResOutParms = (PPERSIST_RES_OUT_PARMS)GET_DATA_BUFFER(pSrb);

        /* Get service action, scope and length */
        serviceAction = GET_U8_FROM_CDB(pSrb, PERSISTENT_RES_CDB_SER_ACTION_OFFSET);

        StorPortDebugPrint(INFO,
            "SntiTranslatePersistentReserveOut: Service Action = 0x%02x\n",
            serviceAction);

        scope = GET_U8_FROM_CDB(pSrb, PERSISTENT_RES_CDB_SCOPE_OFFSET);
        scope = (scope & PERSISTENT_RES_CDB_SCOPE_MASK) >> NIBBLE_SHIFT;

        allocLength = GET_U32_FROM_CDB(pSrb, PERSISTENT_RESOUT_CDB_ALLOC_LEN_OFFSET);

        if ((scope != PERSISTENT_RESOUT_LU_SCOPE) ||
            (allocLength < sizeof(PERSIST_RES_OUT_PARMS))) {
            /* reject command. Buffer is not large enough for the paramaters */
            SntiMapInternalErrorStatus(pSrb, SNTI_INVALID_PARAMETER);
            SET_DATA_LENGTH(pSrb, 0);
            returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
            return returnStatus;
        }

        /* Ensure reservations are requested for the NVMe subsystem */
        /* SNTL 1.5, table 6-35, Parm = SPEC_I_PT 
        /* SPEC_I_PT not valid for REGISTER AND MOVE - SPC 5, 6.15.4 */
        if ((pScsiResOutParms->SPEC_I_PT == 0) ||
            (serviceAction == RESERVATION_ACTION_REGISTER_AND_MOVE)) {
            /* Set SRB status to pending - controller communication necessary */
            pSrb->SrbStatus = SRB_STATUS_PENDING;

            /* No translation required on completion */
            pSrbExt->pNvmeCompletionRoutine = NULL;

            /* Zero out the command entry */
            memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));

            switch (serviceAction) {
            case RESERVATION_ACTION_REGISTER:
            case RESERVATION_ACTION_REGISTER_IGNORE_EXISTING:
            case RESERVATION_ACTION_REGISTER_AND_MOVE:
            {
                ADMIN_SET_FEATURES_COMMAND_RESERVATION_PERSISTENCE_DW11 nvmeResPersistDW11 = { 0 };
                /* determine if we need to check PTPL before proceeding */
                if ((serviceAction != RESERVATION_ACTION_REGISTER_AND_MOVE) &&
                    (pScsiResOutParms->APTPL == 1)) {

                    /* Ensure persistence across power loss is supported */
                    if (pLunExt->identifyData.RESCAP.PTPLC == 0) {
                        /* reject command. Buffer is not large enough for the paramaters */
                        SntiMapInternalErrorStatus(pSrb, SNTI_INVALID_PARAMETER);
                        SET_DATA_LENGTH(pSrb, 0);
                        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
                        return returnStatus;
                    }

                    /* SNTL 1.5, table 6-35, Parm = APTPL=1, SA = Register or
                    *  Register and Ignore.  */

                    /* All namespaces that support reservations will support PTPL.    */
                    /* Instead of issuing a Get Features (FID = 0x83), to see if PTPL */
                    /* is active, just issue the Set Features to ensure it is active. */
                    /* When handling the Set Features response, we'll create and issue */
                    /* the NVMe Reservation Register command */
                    nvmeResPersistDW11.PTPL = 1;
                    SntiBuildSetFeaturesCmd(pSrbExt, RESERVATION_PERSISTANCE, *(PULONG)&nvmeResPersistDW11);

                    /* Callback is required to complete the command */
                    pSrbExt->pNvmeCompletionRoutine = SntiCompletionCallbackRoutine;
                    returnStatus = SNTI_TRANSLATION_SUCCESS;
                } else {
                    /* APTPL is not requested, so proceed with building the NVMe Reservation Register command */
                    returnStatus = SntiBuildPersReserveRegisterCmd(pSrb, pSrbExt, pLunExt, pDevExt, serviceAction, pScsiResOutParms);
                }
                return returnStatus;
            }
            break;
            case RESERVATION_ACTION_RESERVE:
            case RESERVATION_ACTION_PREEMPT:
            case RESERVATION_ACTION_PREEMPT_ABORT:
            case RESERVATION_ACTION_RELEASE:
            case RESERVATION_ACTION_CLEAR:
            {
                scsiRType = GET_U8_FROM_CDB(pSrb, PERSISTENT_RES_CDB_RTYPE_OFFSET);
                scsiRType &= PERSISTENT_RES_CDB_RTYPE_MASK;

                if (scsiRType > RESERVATION_TYPE_MAX_VALUE) {
                    nvmeRType = NVME_RTYPE_RESERVED;
                } else {
                    nvmeRType = ScsiToNVMeRTypeConv[scsiRType];
                }

                if (nvmeRType == NVME_RTYPE_RESERVED) {
                    /* reject command. Buffer is not large enough for the paramaters */
                    SntiMapInternalErrorStatus(pSrb, SNTI_INVALID_PARAMETER);
                    SET_DATA_LENGTH(pSrb, 0);
                    returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
                    return returnStatus;
                }

                switch (serviceAction) {
                case RESERVATION_ACTION_RESERVE:
                case RESERVATION_ACTION_PREEMPT:
                case RESERVATION_ACTION_PREEMPT_ABORT:
                {
                    PNVM_RES_ACQUIRE_DATASTRUCT pNvmeAcqDataStruct = (PNVM_RES_ACQUIRE_DATASTRUCT)&pSrbExt->resAcquireData;
                    PNVM_RES_ACQUIRE_COMMAND_DW10 pNvmeResRegCmdDW10 = (PNVM_RES_ACQUIRE_COMMAND_DW10)&pSrbExt->nvmeSqeUnit.CDW10;

                    pSrbExt->nvmeSqeUnit.CDW0.OPC = NVM_RESERVATION_ACQUIRE;

                    /* SNTL 1.5, tables 4-14 (RTYPE) and 4-16 (IEKEY, RQCQA) */
                    pNvmeResRegCmdDW10->RTYPE = nvmeRType;
                    pNvmeResRegCmdDW10->IEKEY = NVM_REG_CHECK_EXISTING_KEY;

                    if (serviceAction == RESERVATION_ACTION_RESERVE) {
                        pNvmeResRegCmdDW10->RACQA = NVM_RES_ACQ_ACTION_ACQUIRE;
                    } else if (serviceAction == RESERVATION_ACTION_PREEMPT) {
                        pNvmeResRegCmdDW10->RACQA = NVM_RES_ACQ_ACTION_PREEMPT;
                    } else if (serviceAction == RESERVATION_ACTION_PREEMPT_ABORT) {
                        pNvmeResRegCmdDW10->RACQA = NVM_RES_ACQ_ACTION_PREEMPT_ABORT;
                    }

                    /* Get a 4-byte aligned pointer to the Nvme acquire data structure */
                    pNvmeAcqDataStruct = (PNVM_RES_ACQUIRE_DATASTRUCT)(((UINT64)pNvmeAcqDataStruct + sizeof(ULONG) - 1) &
                        ~(sizeof(ULONG) - 1));
                    pParmBuffer = (PUCHAR)pNvmeAcqDataStruct;
                    dataStructSize = sizeof(NVM_RES_ACQUIRE_DATASTRUCT);

                    /* SNTL 1.5, table 6-35, Parm = Reservation Key, SA = Reserve,
                    *  Preempt or Preempt and abort */
                    REVERSE_BYTES_QUAD(&pNvmeAcqDataStruct->CRKEY, &pScsiResOutParms->ReservationKey);

                    /* SNTL 1.5, table 6-35, Parm = Service Action Res Key, SA = Preempt or
                    *  Preempt and abort */
                    if ((pScsiResOutParms->ServiceActionRKey != 0) &&
                        (serviceAction != RESERVATION_ACTION_RESERVE)) {
                        REVERSE_BYTES_QUAD(&pNvmeAcqDataStruct->PRKEY, &pScsiResOutParms->ServiceActionRKey);
                    }
                }
                break;
                case RESERVATION_ACTION_RELEASE:
                case RESERVATION_ACTION_CLEAR:
                {
                    PNVM_RES_RELEASE_DATASTRUCT pNvmeRelDataStruct = (PNVM_RES_RELEASE_DATASTRUCT)&pSrbExt->resReleaseData;
                    PNVM_RES_RELEASE_COMMAND_DW10 pNvmeResRelCmdDW10 = (PNVM_RES_RELEASE_COMMAND_DW10)&pSrbExt->nvmeSqeUnit.CDW10;

                    pSrbExt->nvmeSqeUnit.CDW0.OPC = NVM_RESERVATION_RELEASE;

                    /* SNTL 1.5, tables 4-14 (RTYPE) and 4-16 (IEKEY, RRELA) */
                    pNvmeResRelCmdDW10->RTYPE = nvmeRType;
                    pNvmeResRelCmdDW10->IEKEY = NVM_REG_CHECK_EXISTING_KEY;

                    if (serviceAction == RESERVATION_ACTION_RELEASE) {
                        pNvmeResRelCmdDW10->RRELA = NVM_REL_ACTION_RELEASE;
                    } else {
                        pNvmeResRelCmdDW10->RRELA = NVM_REL_ACTION_CLEAR;
                    }

                    /* Get a 4-byte aligned pointer to the Nvme release data structure */
                    pNvmeRelDataStruct = (PNVM_RES_RELEASE_DATASTRUCT)(((UINT64)pNvmeRelDataStruct + sizeof(ULONG) - 1) &
                        ~(sizeof(ULONG) - 1));
                    pParmBuffer = (PUCHAR)pNvmeRelDataStruct;
                    dataStructSize = sizeof(NVM_RES_RELEASE_DATASTRUCT);

                    /* Clear out the register data structure buffer */
                    memset(pNvmeRelDataStruct, 0, sizeof(NVM_RES_ACQUIRE_DATASTRUCT));

                    /* SNTL 1.5, table 6-35, Parm = Reservation Key, SA = Release
                    *  or Clear */
                    REVERSE_BYTES_QUAD(&pNvmeRelDataStruct->CRKEY, &pScsiResOutParms->ReservationKey);
                }
                break;
                }
            }
            break;
            default: 
            {
                StorPortDebugPrint(INFO,
                    "SntiTranslatePersistentReserveOut: Invalid Service Action = 0x%02x\n",
                    serviceAction);

                /* reject command. Invalid service action  */
                SntiMapInternalErrorStatus(pSrb, SNTI_INVALID_PARAMETER);
                SET_DATA_LENGTH(pSrb, 0);
                returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
                return returnStatus;
            }
            break;
            }
            /* Setup common parts of the command */
            pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
            pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;
            pSrbExt->nvmeSqeUnit.NSID = pLunExt->namespaceId;

            NVMePreparePRPs(pDevExt, pSrbExt, pParmBuffer, dataStructSize);

           
            /* Process the ALL_TG_PT for all service actions */
        } else {
            /* reject command. Reservations cannot be applied per I_T_Nexus */
            SntiMapInternalErrorStatus(pSrb, SNTI_INVALID_PARAMETER);
            SET_DATA_LENGTH(pSrb, 0);
            returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
        }
    }

    return returnStatus;

} /* SntiTranslatePersistentReserveOut */


/******************************************************************************
* SntiBuildPersReserveRegisterCmd
*
* @brief Build an NVMe Reserve Register Command for SCSI PERSISTENT Reserve OUT
* service actions of Register, Register and Ignore Existing Settings and
* Register and Move.
*
* @param pSrbExt - Pointer to Srb Extension
* @param pLunExt - Pointer to the Lun extension
* @param pDevExt - Pointer the device extension
* @param serviceAction - The Service action for this SCSI Persistent Reserve
*                        Out command.
* @param pScsiResOutParms - Pointer to the scsi parameter block passed with this
*                           command.
*
* @return SNTI_TRANSLATION_STATUS
*     Indicate translation status
******************************************************************************/
SNTI_TRANSLATION_STATUS SntiBuildPersReserveRegisterCmd(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_DEVICE_EXTENSION pDevExt,
    UCHAR serviceAction,
    PPERSIST_RES_OUT_PARMS pScsiResOutParms
    )
{
    STOR_PHYSICAL_ADDRESS physAddr = { 0 };
    PUCHAR pNvmeNextPageBoundary = NULL;
    SNTI_STATUS status = SNTI_SUCCESS;

    PNVM_RES_REGISTER_COMMAND_DW10 pNvmeResRegCmdDW10 = NULL;
    PNVM_RES_REGISTER_DATASTRUCT pNvmeRegDataStruct = (PNVM_RES_REGISTER_DATASTRUCT)&pSrbExt->resRegisterData;

    /* Get a pointer to the Nvme register data structure */
    pNvmeRegDataStruct = (PNVM_RES_REGISTER_DATASTRUCT)(((UINT64)pNvmeRegDataStruct + sizeof(ULONG) - 1) &
        ~(sizeof(ULONG) - 1));

    /* Clear out the register data structure buffer */
    memset(pNvmeRegDataStruct, 0, sizeof(NVM_RES_REGISTER_DATASTRUCT));
    pNvmeResRegCmdDW10 = (PNVM_RES_REGISTER_COMMAND_DW10)&pSrbExt->nvmeSqeUnit.CDW10;

    /* Zero out the command entry */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));

    /* Set the NVMe opcode */
    pSrbExt->nvmeSqeUnit.CDW0.OPC = NVM_RESERVATION_REGISTER;

    /* Setup the NVMe command */
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;
    pSrbExt->nvmeSqeUnit.NSID = pLunExt->namespaceId;

    NVMePreparePRPs(pDevExt, pSrbExt, pNvmeRegDataStruct, sizeof(NVM_RES_REGISTER_DATASTRUCT));    

    if (serviceAction == RESERVATION_ACTION_REGISTER_AND_MOVE) {
        /* SNTL 1.5, table 6-35, Parm = Reservation Key, SA = Register and
        *  Move  */
        REVERSE_BYTES_QUAD(&pNvmeRegDataStruct->CRKEY, &pScsiResOutParms->ReservationKey);
        pNvmeResRegCmdDW10->IEKEY = NVM_REG_CHECK_EXISTING_KEY;
        pNvmeResRegCmdDW10->RREGA = NVM_REG_ACTION_REPLACE_KEY;

        /* SNTL 1.5, table 6-35, Parm = Service Action Reservation Key != 0,
        * SA = Register and Move  */
        if (pScsiResOutParms->ServiceActionRKey != 0) {
            /* Process the parm list Service Action Reservation Key */
            REVERSE_BYTES_QUAD(&pNvmeRegDataStruct->NRKEY, &pScsiResOutParms->ServiceActionRKey);
        }
    } else {
        /* Register/Register & Ignore */
        if (serviceAction == RESERVATION_ACTION_REGISTER) {
            pNvmeResRegCmdDW10->IEKEY = NVM_REG_CHECK_EXISTING_KEY;
        } else {
            pNvmeResRegCmdDW10->IEKEY = NVM_REG_IGNORE_EXISTING_KEY;
        }

        /* SNTL 1.5, table 6-35, Parm = Reservation Key,
        * SA = Register and Register & Ignore  */
        if (pScsiResOutParms->ServiceActionRKey == 0) {
            /* Process the parm list Reservation Key */
            REVERSE_BYTES_QUAD(&pNvmeRegDataStruct->CRKEY, &pScsiResOutParms->ReservationKey);
            pNvmeResRegCmdDW10->RREGA = NVM_REG_ACTION_UNREG_KEY;
            /* APTL field is ignored */
        } else {
            pNvmeResRegCmdDW10->RREGA = NVM_REG_ACTION_REG_KEY;
            /* Process the parm list Service Action Reservation Key */
            REVERSE_BYTES_QUAD(&pNvmeRegDataStruct->NRKEY, &pScsiResOutParms->ServiceActionRKey);
        }


        /* SNTL 1.5, table 6-35, Parm = APTPL,
        * SA = Register and Register & Ignore  */
        if (pScsiResOutParms->APTPL == 1) {
            pNvmeResRegCmdDW10->CPTPL = NVM_REG_CPTPL_RES_PERSIST;
        } else {
            pNvmeResRegCmdDW10->CPTPL = NVM_REG_CPTPL_RES_RELEASED;
        }
    }
    return status;
}


/******************************************************************************
* SntiTranslatePersReserveInResponse
*
* @brief Translates the SCSI Persistent Reserve In command based on the NVMe
*        Translation spec. Depending on the specified service action, the response
*        to the SCSI command will come from the response to NVM Reservation Report,
*        NVM Identify or NVM Get Features command.
*
* @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
*               that the user can access the SCSI CDB, response, and data from
*               this pointer. For example, if there is a failure in translation
*               resulting in sense data, then SNTI will call the appropriate
*               internal error handling code and set the status info/data and
*               pass the pSrb pointer as a parameter.
*
* @return SNTI_TRANSLATION_STATUS
*     Indicate translation status
******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslatePersReserveInResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
    )
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PUCHAR pNvmeResStatusBuf = NULL;
    PNVM_RES_REPORT_HDR pNvmeResStatusBufAligned = NULL;
    PNVM_REGISTERED_CTRL_DATASTRUCT pNvmeRegCtlData = NULL;
    ULONG nvmeRspBufSize = 0;
    ULONG nvmeRspDataSize = 0;
    ULONG scsiRspBufSize = 0;
    ULONG scsiRemainingBufLen = 0;
    ULONG copyLength = 0;
    ULONG totalCopyLength = 0;
    ULONG i = 0;
    UCHAR serviceAction = 0;
    UCHAR NVMeToScsiRTypeConversion[NVME_RTYPE_MAX_VALUE + 1] = { 
        RESERVATION_TYPE_NONE, 
        RESERVATION_TYPE_WRITE_EXCLUSIVE,
        RESERVATION_TYPE_EXCLUSIVE,
        RESERVATION_TYPE_WRITE_EXCLUSIVE_REGISTRANTS,
        RESERVATION_TYPE_EXCLUSIVE_REGISTRANTS,
        RESERVATION_TYPE_WRITE_EXCLUSIVE_ALL,
        RESERVATION_TYPE_EXCLUSIVE_ACCESS_ALL };

    /* Default to in-progress command sequence */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_SEQUENCE_IN_PROGRESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pDevExt = (PNVME_DEVICE_EXTENSION)pSrbExt->pNvmeDevExt;


    // Get pointer to response data
    /* Calculate address of the reservation status buffer */
    pNvmeResStatusBuf = &pSrbExt->resReportStruct[0];

    /* ensure it is aligned on the size of the report data structure */
    pNvmeResStatusBufAligned = (PNVM_RES_REPORT_HDR)(((UINT64)pNvmeResStatusBuf + sizeof(ULONG) - 1) &
        ~(sizeof(ULONG) - 1));

    /* determine size of the data buffer */
    nvmeRspBufSize = sizeof(NVME_RES_REPORT_STRUCT);
    /* determine the amount of data returned */
    nvmeRspDataSize = sizeof(NVM_RES_REPORT_HDR) + pNvmeResStatusBufAligned->REGCTL*sizeof(NVM_REGISTERED_CTRL_DATASTRUCT);

    /* get pointer to first set of response data */
    pNvmeRegCtlData = (PNVM_REGISTERED_CTRL_DATASTRUCT)((UINT64)pNvmeResStatusBufAligned + sizeof(NVM_RES_REPORT_HDR));


    /* Get size of SCSI response buffer */
    scsiRspBufSize = GET_DATA_LENGTH(pSrb);
    scsiRemainingBufLen = scsiRspBufSize;

    /* get the scsi reserve in service action */
    serviceAction = GET_U8_FROM_CDB(pSrb, PERSISTENT_RES_CDB_SER_ACTION_OFFSET);

    StorPortDebugPrint(INFO,
        "SntiTranslatePersReserveInResponse: Service Action = 0x%02x\n",
        serviceAction);


    switch (serviceAction) {
    case RESERVATION_ACTION_READ_KEYS:
    {
        PULONGLONG pScsiNextReservationKey = NULL;
        PERSIST_RES_PARM_DATA_HDR localScsiRspHdr = { 0 };
        PPERSIST_RES_PARM_DATA_HDR pScsiResParmData = NULL;
        ULONG temp = 0;
        ULONGLONG tempNvmeRKEY = 0;

        /* Get pointer to SCSI response buffer */
        pScsiResParmData = (PPERSIST_RES_PARM_DATA_HDR)GET_DATA_BUFFER(pSrb);

        /* Create local SCSI response header */
        REVERSE_BYTES(&localScsiRspHdr.PRGENERATION, &pNvmeResStatusBufAligned->GEN);
        temp = pNvmeResStatusBufAligned->REGCTL*sizeof(pNvmeRegCtlData->RKEY);
        REVERSE_BYTES(&localScsiRspHdr.AddlLength, &temp);

        /* Copy SCSI response header */
        copyLength = min(scsiRemainingBufLen, sizeof(localScsiRspHdr));
        totalCopyLength = copyLength;
        memcpy(pScsiResParmData, &localScsiRspHdr, copyLength);
        scsiRemainingBufLen -= copyLength;

        /* Loop through Reservation Keys - copying to SCSI response buffer */
        for (i = 0, pScsiNextReservationKey = (PULONGLONG)((UINT64)pScsiResParmData + sizeof(PERSIST_RES_PARM_DATA_HDR));
            (i < pNvmeResStatusBufAligned->REGCTL) && (scsiRemainingBufLen > 0);
            i++,
            scsiRemainingBufLen -= copyLength,
            pNvmeRegCtlData += 1,
            pScsiNextReservationKey += 1) {
            /* Make a local big endian copy of the RKEY */
            REVERSE_BYTES_QUAD(&tempNvmeRKEY, &pNvmeRegCtlData->RKEY);

            /* Determine copy length based on the size of the RKEY and the remaining length in the scsi buffer */
            copyLength = min(scsiRemainingBufLen, sizeof(pNvmeRegCtlData->RKEY));
            totalCopyLength += copyLength;
            /* copy RKEY to the scsi response buffer */
            memcpy(pScsiNextReservationKey, &tempNvmeRKEY, copyLength);
        }
        returnStatus = SNTI_SEQUENCE_COMPLETED;
        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        SET_DATA_LENGTH(pSrb, totalCopyLength);
    }
    break;


    case RESERVATION_ACTION_READ_RESERVATIONS:
    {
        PERSIST_RES_RD_RES_DATA localScsiRspData = { 0 };
        BOOLEAN found = FALSE;
        PPERSIST_RES_RD_RES_DATA pScsiResRdRes = NULL;
        ULONG temp = 0;

        /* Get pointer to SCSI response buffer */
        pScsiResRdRes = (PPERSIST_RES_RD_RES_DATA)GET_DATA_BUFFER(pSrb);

        /* Init the response header */
        REVERSE_BYTES(&localScsiRspData.hdr.PRGENERATION, &pNvmeResStatusBufAligned->GEN);
        localScsiRspData.hdr.AddlLength = 0;

        found = FALSE;

        /* Determine if there is a reservation */
        if ((pNvmeResStatusBufAligned->RTYPE > NVME_RTYPE_NO_RESERVATION) &&
            (pNvmeResStatusBufAligned->RTYPE <= NVME_RTYPE_MAX_VALUE)) {
            /* Loop through Reservation Keys -  */
            for (i = 0, copyLength = 0;
                !found && (i < pNvmeResStatusBufAligned->REGCTL);
                i++, scsiRemainingBufLen -= copyLength, pNvmeRegCtlData += 1) {
                if ((pNvmeRegCtlData->RCSTS & NVME_RCSTS_HOST_HOLDS_RES_MASK) == NVME_RCSTS_HOST_HOLDS_RES_MASK) {
                    REVERSE_BYTES_QUAD(&localScsiRspData.ReservationKey, &pNvmeRegCtlData->RKEY);
                    localScsiRspData.Scope = 0;
                    localScsiRspData.Type = NVMeToScsiRTypeConversion[pNvmeResStatusBufAligned->RTYPE];
                    temp = sizeof(PERSIST_RES_RD_RES_DATA) - sizeof(PERSIST_RES_PARM_DATA_HDR);
                    REVERSE_BYTES(&localScsiRspData.hdr.AddlLength, &temp);

                    /* Copy SCSI response header */
                    copyLength = min(scsiRemainingBufLen, sizeof(localScsiRspData));
                    memcpy((PVOID)pScsiResRdRes, &localScsiRspData, copyLength);
                    found = TRUE;
                }
            }
        }

        if (found != TRUE) {
            /* Copy SCSI response header */
            /* Only copy the header */
            copyLength = min(scsiRemainingBufLen, sizeof(PERSIST_RES_PARM_DATA_HDR));
            memcpy((PVOID)pScsiResRdRes, &localScsiRspData, copyLength);
        }

        returnStatus = SNTI_SEQUENCE_COMPLETED;
        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        SET_DATA_LENGTH(pSrb, copyLength);
    }
    break;

    case RESERVATION_ACTION_READ_FULL_STATUS:
    {
        PPERSIST_RES_PARM_DATA_HDR pScsiResRspHdr = NULL;
        PPERSIST_RES_FULL_STATUS_DESC pScsiFullStatDesc = NULL;
        PERSIST_RES_PARM_DATA_HDR localScsiRspHdr = { 0 };
        PERSIST_RES_FULL_STATUS_DESC localScsiFullStatDesc = { 0 };
        ULONGLONG temp = 0;

        /* Get pointer to SCSI response buffer */
        pScsiResRspHdr = (PPERSIST_RES_PARM_DATA_HDR)GET_DATA_BUFFER(pSrb);


        /* Create local SCSI response header */
        REVERSE_BYTES(&localScsiRspHdr.PRGENERATION, &pNvmeResStatusBufAligned->GEN);
        temp = pNvmeResStatusBufAligned->REGCTL*(sizeof(PERSIST_RES_FULL_STATUS_DESC));
        REVERSE_BYTES(&localScsiRspHdr.AddlLength, &temp);

        /* Copy SCSI response header */
        copyLength = min(scsiRemainingBufLen, sizeof(localScsiRspHdr));
        totalCopyLength = copyLength;
        memcpy(pScsiResRspHdr, &localScsiRspHdr, copyLength);
        scsiRemainingBufLen -= copyLength;

        /* Loop through Reservation Keys - copying to SCSI response buffer */
        for (i = 0, pScsiFullStatDesc = (PPERSIST_RES_FULL_STATUS_DESC)((UINT64)pScsiResRspHdr + sizeof(PERSIST_RES_PARM_DATA_HDR));
            (i < pNvmeResStatusBufAligned->REGCTL) && (scsiRemainingBufLen > 0);
            i++, scsiRemainingBufLen -= copyLength, pNvmeRegCtlData += 1, pScsiFullStatDesc += 1) {
            /* Init the local full status descriptor */
            memset(&localScsiFullStatDesc, 0, sizeof(localScsiFullStatDesc));

            /* */
            REVERSE_BYTES_QUAD(&localScsiFullStatDesc.ReservationKey, &pNvmeRegCtlData->RKEY);
            localScsiFullStatDesc.R_HOLDER = ((pNvmeRegCtlData->RCSTS & NVME_RCSTS_HOST_HOLDS_RES_MASK) == 1) ? 1 : 0;
            localScsiFullStatDesc.ALL_TG_PT = 1;
            localScsiFullStatDesc.Scope = 0;
            if (pNvmeResStatusBufAligned->RTYPE <= NVME_RTYPE_MAX_VALUE) {
                localScsiFullStatDesc.Type = NVMeToScsiRTypeConversion[pNvmeResStatusBufAligned->RTYPE];
            } else {
                StorPortDebugPrint(INFO,
                    "SntiTranslatePersReserveInResponse: Invalid RType from the device =  0x%x \n",
                    pNvmeResStatusBufAligned->RTYPE);
                SntiMapInternalErrorStatus(pSrb, SNTI_FAILURE);
                SET_DATA_LENGTH(pSrb, 0);
                returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
                return returnStatus;
            }
            REVERSE_BYTES_SHORT(&localScsiFullStatDesc.RelTgtPortId, &pNvmeRegCtlData->CNTLID);
            temp = sizeof(localScsiFullStatDesc.TransportID);
            REVERSE_BYTES(&localScsiFullStatDesc.AddlDescLenth, &temp);
            REVERSE_BYTES_QUAD(&localScsiFullStatDesc.TransportID, &pNvmeRegCtlData->HOSTID);

            /* Determine copy length based on the size of the RKEY and the remaining length in the scsi buffer */
            copyLength = min(scsiRemainingBufLen, sizeof(localScsiFullStatDesc));
            totalCopyLength += copyLength;
            memcpy(pScsiFullStatDesc, &localScsiFullStatDesc, copyLength);
        }
        returnStatus = SNTI_SEQUENCE_COMPLETED;
        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        SET_DATA_LENGTH(pSrb, totalCopyLength);
    }
    break;

    case RESERVATION_ACTION_REPORT_CAPABILITIES:
    {
        PERSIST_RES_REP_CAPABILIITES localScsiPersResRepCap = { 0 };
        PPERSIST_RES_REP_CAPABILIITES pScsiResRspHdr = NULL;
        PNVME_LUN_EXTENSION pLunExt;
        SNTI_STATUS status = SNTI_SUCCESS;
        PADMIN_SET_FEATURES_COMMAND_RESERVATION_PERSISTENCE_DW11 pResPersistData = NULL;
        USHORT temp = 0;

        /* Determine if the lun is active/valid */
        status = GetLunExtension(pSrbExt, &pLunExt);
        if (status != SNTI_SUCCESS) {
            INT lun = 0xff;
#if (NTDDI_VERSION > NTDDI_WIN7)
            lun = SrbGetLun((void*)pSrb);
#else
            lun = pSrb->Lun;
#endif
            StorPortDebugPrint(INFO,
                "SntiTranslatePersReserveInResponse: Invalid LUN specified - Lun: 0x%x \n",
                lun);
            SntiMapInternalErrorStatus(pSrb, status);
            SET_DATA_LENGTH(pSrb, 0);
            returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
            return returnStatus;
        }

        /* Get pointer response data from Get Features Reservation Persistance (FID 0x83) */
        pResPersistData = (PADMIN_SET_FEATURES_COMMAND_RESERVATION_PERSISTENCE_DW11)&pSrbExt->pCplEntry->DW0;


        /* Fill out local copy of SCSI response data */
        /* Set according to SNTL 1.5, section 6.7.3*/
        temp = sizeof(PERSIST_RES_REP_CAPABILIITES);
        REVERSE_BYTES_SHORT(&localScsiPersResRepCap.Length, &temp);
        localScsiPersResRepCap.CRH = 0;
        localScsiPersResRepCap.SIP_C = 0;
        localScsiPersResRepCap.ATP_C = 1;
        localScsiPersResRepCap.PTPL_C = pLunExt->identifyData.RESCAP.PTPLC;
        localScsiPersResRepCap.PTPL_A = (UCHAR)pResPersistData->PTPL;
        localScsiPersResRepCap.AllowCmds = 0;
        localScsiPersResRepCap.TMV = 1;
        localScsiPersResRepCap.PersResTypeMask.WR_EX = pLunExt->identifyData.RESCAP.WrEx;
        localScsiPersResRepCap.PersResTypeMask.EX_AC = pLunExt->identifyData.RESCAP.ExAccess;
        localScsiPersResRepCap.PersResTypeMask.WR_EX_RO = pLunExt->identifyData.RESCAP.WrExReg;
        localScsiPersResRepCap.PersResTypeMask.EX_AC_RO = pLunExt->identifyData.RESCAP.ExAccessReg;
        localScsiPersResRepCap.PersResTypeMask.WR_EX_AR = pLunExt->identifyData.RESCAP.WrExAllReg;
        localScsiPersResRepCap.PersResTypeMask.EX_AC_AR = pLunExt->identifyData.RESCAP.ExAccessAllReg;


        /* Get pointer  to SCSI response buffer */
        pScsiResRspHdr = (PPERSIST_RES_REP_CAPABILIITES)GET_DATA_BUFFER(pSrb);
        /* Copy to actual response buffer */

        copyLength = min(scsiRemainingBufLen, sizeof(localScsiPersResRepCap));
        memcpy(pScsiResRspHdr, &localScsiPersResRepCap, copyLength);

        returnStatus = SNTI_SEQUENCE_COMPLETED;
        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        SET_DATA_LENGTH(pSrb, copyLength);
    }
    break;

    default:
        StorPortDebugPrint(INFO,
            "SntiTranslatePersReserveInResponse: Invalid Service Action =  0x%x \n",
            serviceAction);
        /* Map the translation error to a SCSI error */
        SntiMapInternalErrorStatus(pSrb, SNTI_FAILURE);
        returnStatus = SNTI_SEQUENCE_ERROR;
        pSrb->SrbStatus |= SRB_STATUS_ERROR;
        SET_DATA_LENGTH(pSrb, 0);

        ASSERT(FALSE);
        break;
    }

    return returnStatus;

}

/******************************************************************************
* SntiTranslatePersReserveOutResponse
*
* @brief Processing SCSI Persistent Reserve Out command. This response processing
*        will only be called if the first pass of processing issued an NVM Set
*        Features command, this method will complete the processing by issuing
*        an NVM Reservation Register command.
*
* @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
*               that the user can access the SCSI CDB, response, and data from
*               this pointer. For example, if there is a failure in translation
*               resulting in sense data, then SNTI will call the appropriate
*               internal error handling code and set the status info/data and
*               pass the pSrb pointer as a parameter.
*
* @return SNTI_TRANSLATION_STATUS
*     Indicate translation status
******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslatePersReserveOutResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
    )
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    UCHAR serviceAction = 0;
    PPERSIST_RES_OUT_PARMS pScsiResOutParms = NULL;
    SNTI_STATUS status = SNTI_SUCCESS;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pDevExt = pSrbExt->pNvmeDevExt;

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        SntiMapInternalErrorStatus(pSrb, status);
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {

        pScsiResOutParms = (PPERSIST_RES_OUT_PARMS)GET_DATA_BUFFER(pSrb);

        /* Set SRB status to pending - controller communication necessary */
        pSrb->SrbStatus = SRB_STATUS_PENDING;

        /* No translation required on completion */
        pSrbExt->pNvmeCompletionRoutine = NULL;

        /* Get service action */
        serviceAction = GET_U8_FROM_CDB(pSrb, PERSISTENT_RES_CDB_SER_ACTION_OFFSET);

        StorPortDebugPrint(INFO,
            "SntiTranslatePersReserveOutResponse:  Service Action = 0x%x \n",
             serviceAction);

        /* APTPL is not requested, so proceed with building the NVMe Reservation Register command */
        returnStatus = SntiBuildPersReserveRegisterCmd(pSrb, pSrbExt, pLunExt, pDevExt, serviceAction, pScsiResOutParms);

        if (returnStatus == SNTI_SUCCESS) {
            /* Now issue the command via Admin Doorbell register */
            if (ProcessIo(pSrbExt->pNvmeDevExt, pSrbExt, NVME_QUEUE_TYPE_ADMIN, FALSE) == FALSE) {
                returnStatus = SNTI_SEQUENCE_ERROR;
                pSrb->SrbStatus = SRB_STATUS_ERROR;
            } else {
                returnStatus = SNTI_SEQUENCE_IN_PROGRESS;
                pSrb->SrbStatus = SRB_STATUS_PENDING;
            }
        }
    }
    return returnStatus;

}

/******************************************************************************
 * SntiTranslateStartStopUnit
 *
 * @brief Translates the SCSI Start Stop Unit command based on the NVMe
 *        Translation spec to a NVMe Set Features command and populates a
 *        temporary SQE stored in the SRB Extension.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicate translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateStartStopUnit(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    UINT8 immed = 0;
    UINT8 powerCondMod = 0;
    UINT8 powerCond = 0;
    UINT8 noFlush = 0;
    UINT8 start = 0;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    immed = GET_U8_FROM_CDB(pSrb, START_STOP_UNIT_CDB_IMMED_OFFSET);
    powerCondMod =
        GET_U8_FROM_CDB(pSrb, START_STOP_UNIT_CDB_POWER_COND_MOD_OFFSET);
    powerCond = GET_U8_FROM_CDB(pSrb, START_STOP_UNIT_CDB_POWER_COND_OFFSET);
    noFlush = GET_U8_FROM_CDB(pSrb, START_STOP_UNIT_CDB_NO_FLUSH_OFFSET);
    start = GET_U8_FROM_CDB(pSrb, START_STOP_UNIT_CDB_START_OFFSET);

    immed &= START_STOP_UNIT_CDB_IMMED_MASK;
    powerCondMod &= START_STOP_UNIT_CDB_POWER_COND_MOD_MASK;
    powerCond &= START_STOP_UNIT_CDB_POWER_COND_MASK;
    noFlush &= START_STOP_UNIT_CDB_NO_FLUSH_MASK;
    start &= START_STOP_UNIT_CDB_START_MASK;

    /* Default the SRB Status to PENDING */
    pSrb->SrbStatus = SRB_STATUS_PENDING;

    /*
     * Default the completion routine to NULL - no translation will be
     * necessary on completion unless we have to issue a FLUSH first
     */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    if (immed != 0) {
        /* Ensure correct sense data for SCSI compliance test case 1.4 */
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);

        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        if (noFlush == 0) {
            /* Issue NVME FLUSH command prior to translating START STOP UNIT */
            SntiBuildFlushCmd(pSrbExt);

            /*
             * START STOP UNIT translation shall continue on FLUSH completion.
             * Override the completion routine.
             */
            pSrbExt->pNvmeCompletionRoutine = SntiCompletionCallbackRoutine;

            returnStatus = SNTI_TRANSLATION_SUCCESS;
            pSrb->SrbStatus = SRB_STATUS_PENDING;
        } else {
            /* Setup the expected power state transition */
            returnStatus = SntiTransitionPowerState(pSrbExt,
                                                    powerCond,
                                                    powerCondMod,
                                                    start);
        }
    }

    return returnStatus;
} /* SntiTranslateStartStopUnit */

/******************************************************************************
 * SntiTransitionPowerState
 *
 * @brief Transitions power state based on SCSI START/STOP unit command.
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param powerCond - Power condition from START/STOP UNIT command
 * @param powerCondMod - Power condition modifier from START/STOP UNIT command
 * @param start - Start value
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTransitionPowerState(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT8 powerCond,
    UINT8 powerCondMod,
    UINT8 start
)
{
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif

    UINT32 dword11 = 0;
    UINT8 numPowerStatesSupported = 0;
    UINT8 lowestPowerStateSupported = 0;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;
    BOOLEAN cdbValid = TRUE;

    /* Determine lowest and last power state supported */
    numPowerStatesSupported = pSrbExt->pNvmeDevExt->controllerIdentifyData.NPSS;
    lowestPowerStateSupported = 0;

    switch ((powerCond >> NIBBLE_SHIFT)) {
        case NVME_POWER_STATE_START_VALID:
            /* Action unspecified - POWER CONDITION MODIFIER != 0 */
            if (powerCondMod == 0) {
                if (start == 0x1) {
                    /* Issue Set Features - Power State 0 */
                    dword11 &= POWER_STATE_0;
                    SntiBuildSetFeaturesCmd(pSrbExt, POWER_MANAGEMENT, dword11);
                } else {
                    /* Issue Set Features - Lowest Power State supported */
                    dword11 &= lowestPowerStateSupported;
                    SntiBuildSetFeaturesCmd(pSrbExt, POWER_MANAGEMENT, dword11);
                }
            } else {
                cdbValid = FALSE;
            }
        break;
        case NVME_POWER_STATE_ACTIVE:
            /* Action unspecified - POWER CONDITION MODIFIER != 0 */
            if (powerCondMod == 0) {
                /* Issue Set Features - Power State 0 */
                dword11 &= POWER_STATE_0;
                SntiBuildSetFeaturesCmd(pSrbExt, POWER_MANAGEMENT, dword11);
            } else {
                cdbValid = FALSE;
            }
        break;
        case NVME_POWER_STATE_IDLE:
            /* Action unspecified - POWER CONDITION MODIFIER != 0, 1, or 2 */
            if (powerCondMod == 0x0) {
                /* Issue Set Features - Power State 1 */
                dword11 &= POWER_STATE_1;
                SntiBuildSetFeaturesCmd(pSrbExt, POWER_MANAGEMENT, dword11);
            } else if (powerCondMod == 0x1 ) {
                /* Issue Set Features - Power State 2 */
                dword11 &= POWER_STATE_2;
                SntiBuildSetFeaturesCmd(pSrbExt, POWER_MANAGEMENT, dword11);
            } else if (powerCondMod == 0x2 ) {
                /* Issue Set Features - Power State 3 */
                dword11 &= POWER_STATE_3;
                SntiBuildSetFeaturesCmd(pSrbExt, POWER_MANAGEMENT, dword11);
            } else {
                cdbValid = FALSE;
            }
        break;
        case NVME_POWER_STATE_STANDBY:
            /* Action unspecified - POWER CONDITION MODIFIER != 0 or 1 */
            if (powerCondMod == 0x0) {
                /*
                 * Issue Set Features - Power State N-2, where N = last
                 * power state supported
                 */
                dword11 &= (numPowerStatesSupported - 2);
                SntiBuildSetFeaturesCmd(pSrbExt, POWER_MANAGEMENT, dword11);
            } else if (powerCondMod == 0x1 ) {
                /*
                 * Issue Set Features - Power State N-1, where N = last
                 * power state supported
                 */
                dword11 &= (numPowerStatesSupported - 1);
                SntiBuildSetFeaturesCmd(pSrbExt, POWER_MANAGEMENT, dword11);
            } else {
                cdbValid = FALSE;
            }
        break;
        case NVME_POWER_STATE_LU_CONTROL:
            /* Action unspecified */
            pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        break;
        default:
            cdbValid = FALSE;
        break;
    } /* end switch */

    if (!cdbValid) {
        /* Ensure correct sense data for SCSI compliance test case 1.4 */
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);

        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    }

    return returnStatus;
} /* SntiTransitionPowerState */

#if (NTDDI_VERSION > NTDDI_WIN7)
/******************************************************************************
 * SntiTranslateUnmap
 *
 * @brief Translates the SCSI Unmap command based on the NVMe Translation
 *        spec, and generates an NVMe Dataset Managment command. SCSI
 *        Unmap block descriptors describing the LBA ranges to be Unmapped
 *        are translated to NVMe DataSet Managment range definitions.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateUnmap(
    PSTORAGE_REQUEST_BLOCK pSrb
)
{
    ULONG paLength = 0;
    UINT16 numBlockDescriptors = 0;
    UINT16 numRanges = 0;
    STOR_PHYSICAL_ADDRESS physAddr;
    SNTI_STATUS status = SNTI_SUCCESS;
    UINT16 blockDescriptorDataLength = 0;
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVM_DATASET_MANAGEMENT_COMMAND_DW10 pCdw10 = NULL;
    PNVM_DATASET_MANAGEMENT_COMMAND_DW11 pCdw11 = NULL;
    PNVM_DATASET_MANAGEMENT_RANGE pCurrentDsmRange = NULL;
    PNVM_DATASET_MANAGEMENT_RANGE pAlignedDsmRange = NULL;
    PNVM_DATASET_MANAGEMENT_RANGE pPageAlignedDsmRange = NULL;
    PUNMAP_BLOCK_DESCRIPTOR pCurrentUnmapBlockDescriptor = NULL; 
    ULONG dsmRangeSize = 0;
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;

    /*
     * Since LBA anchoring is not supported, if Anchor bit is set, 
     * need to terminate the command now.
     */
    PUCHAR pCdb = (PUCHAR)SrbGetCdb((void*)pSrb);
    pCdb++;
    if ((*pCdb & UNMAP_ANCHAR_BIT) == 1) {
        StorPortDebugPrint(INFO, "SNTI: Unmap Anchor bit is set!\n");
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        returnStatus = SNTI_UNSUPPORTED_SCSI_REQUEST;
    }

    pSrbExt = (PNVME_SRB_EXTENSION)SrbGetMiniportContext(pSrb);

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        /* Map the translation error to a SCSI error */
        SntiMapInternalErrorStatus(pSrb, status);
        return SNTI_FAILURE_CHECK_RESPONSE_DATA;
    }

    /* 
       First, get the number of SCSI Unmap block descriptors and ensure
       that this is within legal range.
       (Must swap bytes, as Unmap request has data in big endian format)
    */

    if (GET_DATA_LENGTH(pSrb) < sizeof(UNMAP_LIST_HEADER)) {
        SntiSetScsiSenseData(pSrb,
            SCSISTAT_CHECK_CONDITION,
            SCSI_SENSE_ILLEGAL_REQUEST,
            SCSI_ADSENSE_INVALID_FIELD_PARAMETER_LIST,
            SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    }
    else {
        REVERSE_BYTES_SHORT(&blockDescriptorDataLength,
            ((UNMAP_LIST_HEADER*)(GET_DATA_BUFFER(pSrb)))->BlockDescrDataLength);

        numBlockDescriptors = blockDescriptorDataLength /
            sizeof(UNMAP_BLOCK_DESCRIPTOR);

        dsmRangeSize = numBlockDescriptors * sizeof(NVM_DATASET_MANAGEMENT_RANGE);

        if (numBlockDescriptors > MAX_UNMAP_BLOCK_DESCRIPTOR_COUNT ||
            GET_DATA_LENGTH(pSrb) < sizeof(UNMAP_LIST_HEADER) + blockDescriptorDataLength) {
            SntiSetScsiSenseData(pSrb,
                SCSISTAT_CHECK_CONDITION,
                SCSI_SENSE_ILLEGAL_REQUEST,
                SCSI_ADSENSE_INVALID_FIELD_PARAMETER_LIST,
                SCSI_ADSENSE_NO_SENSE);

            pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
            returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
        }
        else {
            /* Set up common portions of the NVMe DSM command */
            memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
            /* CDW0 */
            pSrbExt->nvmeSqeUnit.CDW0.OPC = NVM_DATASET_MANAGEMENT;
            pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
            pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;
            pSrbExt->nvmeSqeUnit.NSID = pLunExt->namespaceId;

            /* 
               CDW10 -- zero for now - we will increment NR field as number of
               ranges are counted up
            */
            pCdw10 = (PNVM_DATASET_MANAGEMENT_COMMAND_DW10)
                &(pSrbExt->nvmeSqeUnit.CDW10);
            pCdw10->NR=0;

            /* CDW11 -- set AD bit indicating an attribute of Deallocate*/
            pCdw11 = (PNVM_DATASET_MANAGEMENT_COMMAND_DW11)
                &(pSrbExt->nvmeSqeUnit.CDW11);
            pCdw11->AD = 1;

            // Align the DSM buffer on a 16-byte boundary (size of the DSM range element)
            pAlignedDsmRange =
                (PNVM_DATASET_MANAGEMENT_RANGE)((((UINT64)&pSrbExt->dsmBuffer) + sizeof(NVM_DATASET_MANAGEMENT_RANGE)) &
                ~(sizeof(NVM_DATASET_MANAGEMENT_RANGE)-1));

            /* Clear out the range buffers */
            memset(pAlignedDsmRange, 0, dsmRangeSize);

            // Find the page-aligned address within this buffer
            pPageAlignedDsmRange = PAGE_ALIGN_BUF_PTR(pAlignedDsmRange);

            /* Point PRP1 to our dedicated DSM range definition buffer */
            physAddr = StorPortGetPhysicalAddress(pSrbExt->pNvmeDevExt, 
                NULL, 
                pAlignedDsmRange,
                &paLength);

            if (physAddr.QuadPart != 0) { 
                pSrbExt->nvmeSqeUnit.PRP1 = physAddr.QuadPart;

                /* determine if PRP2 is necessary */

                /* Is the dsm buffer already page-aligned? OR Do then entries fit in the first page */
                if ((pAlignedDsmRange == pPageAlignedDsmRange) ||
                    (((PUINT8)pAlignedDsmRange) + dsmRangeSize) <= ((PUINT8)pPageAlignedDsmRange))
                {
                    pSrbExt->nvmeSqeUnit.PRP2 = 0;
                    pSrbExt->numberOfPrpEntries = 1;
                } else {
                    physAddr = StorPortGetPhysicalAddress(pSrbExt->pNvmeDevExt,
                        NULL,
                        pPageAlignedDsmRange,
                        &paLength);

                    if (physAddr.QuadPart != 0) {
                        pSrbExt->nvmeSqeUnit.PRP2 = physAddr.QuadPart;
                        pSrbExt->numberOfPrpEntries = 2;
            } else { 
                StorPortDebugPrint(INFO, 
                            "SNTI: Second Get PhysAddr for UNMAP failed (pSrbExt = 0x%p)\n",
                            pSrbExt);
                        ASSERT(FALSE);
                    }
                }
            } else {
                StorPortDebugPrint(INFO,
                    "SNTI: First Get PhysAddr for UNMAP failed (pSrbExt = 0x%p)\n",
                    pSrbExt);
                ASSERT(FALSE);
            }

            pCurrentUnmapBlockDescriptor = 
                (PUNMAP_BLOCK_DESCRIPTOR)((UCHAR*)(GET_DATA_BUFFER(pSrb)) +
                sizeof(UNMAP_LIST_HEADER));
        
            pCurrentDsmRange = pAlignedDsmRange;

            /* 
               Iterate over all Unmap block descriptors, converting from big endian
               and moving into NVMe DSM range definitiion entries (true these are
               being "moved" prior to validation, but the number of ranges counter
               and range definition pointer will only be updated if descriptors
               prove to be valid)
            */  

            while ((0 != numBlockDescriptors--) && (status == SNTI_SUCCESS)) {
                REVERSE_BYTES(&(pCurrentDsmRange->LengthInLogicalBlocks), 
                    pCurrentUnmapBlockDescriptor->LbaCount);
                REVERSE_BYTES_QUAD(&(pCurrentDsmRange->StartingLBA), 
                    pCurrentUnmapBlockDescriptor->StartingLba);

                /* Validate incoming SCSI Unmap block descriptor */
                status = SntiValidateUnmapLbaAndLength(pLunExt, 
                    pSrbExt, 
                    pCurrentDsmRange->StartingLBA, 
                    pCurrentDsmRange->LengthInLogicalBlocks);

            
                /* 
                    Per SCSI spec, a length of zero is not considered to 
                    be an error, so we simply skip any that have zero length
                */
                if (status == SNTI_SUCCESS) {
                    if (pCurrentDsmRange->LengthInLogicalBlocks != 0) {
                        pCurrentDsmRange++;
                        numRanges++;
                    }

                    pCurrentUnmapBlockDescriptor++; 
                } 
            } /*while there are block descriptors to process*/

            if (status == SNTI_SUCCESS) {
                /* Adjust, as NR is a 0 based value */
                if (numRanges > 0) {
                    pCdw10->NR = --numRanges;
                }
                else{
                /* 
                   If lengths in all descriptors happened to have been zero
                   then there is no command to send down, so report
                   that things are complete.
                */
                returnStatus = SNTI_COMMAND_COMPLETED;
                }
            } else {
                returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
            }

        }   /* If valid number of block descriptors to process */
    }

    return returnStatus;
} /* SntiTranslateUnmap  */
#endif

#if (NTDDI_VERSION > NTDDI_WIN7)
/******************************************************************************
 * SntiValidateUnmapLbaAndLength
 *
 * @brief Validates the LBA and number of requested logical blocks within an
 *        UNMAP block descriptor associated with a SCSI UNMAP command. If the
 *        LBA + number of logical blocks exceeds the capacity of the namespace,
 *        a check condition with ILLEGAL REQUEST - LBA Out Of Range (5/2100)
 *        shall be returned.
 *
 *        If the total number of logical blocks exceeds the value indicated in
 *        the MAXIMUM UNMAP LBA COUNT field in the Block Limits VPD
 *        Page (which contains NVME_MAX_NUM_BLOCKS_PER_READ_WRITE), a
 *        check condition with ILLEGAL REQUEST and the additional sense
 *        code set to INVALID FIELD IN PARAMETER LIST shall be returned.
 *
 *
 *        NOTE: Namespace Size is in logical blocks (per NVM Express 1.0b)
 *
 * @param: pLunExt - Pointer to LUN extension
 * @param: pSrbExt - Pointer to SRB extension
 * @param: lba - LBA to verify
 * @param: length - Length to verify
 *
 * @return SNTI_STATUS
 *     Indicates internal tranlsation status
 ******************************************************************************/
SNTI_STATUS SntiValidateUnmapLbaAndLength(
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_SRB_EXTENSION pSrbExt,
    UINT64 lba,
    UINT32 length
)
{
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
    SNTI_STATUS status = SNTI_SUCCESS;

    if ((lba + length) > (pLunExt->identifyData.NSZE + 1)) {
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_ILLEGAL_BLOCK,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        status = SNTI_INVALID_PARAMETER;
    }

    if (length > NVME_MAX_NUM_BLOCKS_PER_READ_WRITE) {
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_FIELD_PARAMETER_LIST,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        status = SNTI_INVALID_PARAMETER;
    }

    return status;
} /* SntiValidateUnmapLbaAndLength*/
#endif


/******************************************************************************
 * SntiTranslateWriteBuffer
 *
 * @brief Translates the SCSI Write Buffer command based on the NVMe Translation
 *        spec to a NVMe Firmware Image Download and/or Firmware Activate
 *        command and populates a temporary SQE stored in the SRB Extension.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateWriteBuffer(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    UINT32 bufferOffset = 0; /* 3 byte field */
    UINT32 paramListLength = 0; /* 3 byte field */
    UINT32 dword10 = 0;
    UINT32 dword11 = 0;
    UINT8 bufferId = 0;
    UINT8 mode = 0;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;
    SNTI_STATUS status = SNTI_SUCCESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    mode = GET_U8_FROM_CDB(pSrb, WRITE_BUFFER_CDB_MODE_OFFSET);
    bufferId = GET_U8_FROM_CDB(pSrb, WRITE_BUFFER_CDB_BUFFER_ID_OFFSET);

    bufferOffset =
        GET_U24_FROM_CDB(pSrb, WRITE_BUFFER_CDB_BUFFER_OFFSET_OFFSET);
    paramListLength =
        GET_U24_FROM_CDB(pSrb, WRITE_BUFFER_CDB_PARAM_LIST_LENGTH_OFFSET);

    /* Set the completion routine - translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = SntiCompletionCallbackRoutine;

    switch (mode & WRITE_BUFFER_CDB_MODE_MASK) {
        case DOWNLOAD_SAVE_ACTIVATE:
            /* Issue NVME FIRMWARE IMAGE DOWNLOAD command */
            /* NVMe Spec 1.1, Section 5.8: dword10 (NUMD) is zero-based. */
            dword10 |= ((paramListLength / NUM_BYTES_IN_DWORD)- 1);
            dword11 |= bufferOffset;

            SntiBuildFirmwareImageDownloadCmd(pSrbExt, dword10, dword11);

            /* Activate microcode upon completion of Firmware Image Download */
        break;
        case DOWNLOAD_SAVE_DEFER_ACTIVATE:
            /* Issue NVME FIRMWARE IMAGE DOWNLOAD command */
            /* NVMe Spec 1.1, Section 5.8: dword10 (NUMD) is zero-based. */
            dword10 |= ((paramListLength / NUM_BYTES_IN_DWORD)- 1);
            dword11 |= bufferOffset;

            SntiBuildFirmwareImageDownloadCmd(pSrbExt, dword10, dword11);

            /* Do not activate - must receive a separate activate command */
        break;
        case ACTIVATE_DEFERRED_MICROCODE:
            /* Issue NVME FIRMWARE ACTIVATE command */
            /* Spec Errata - Truncation and Activate Action (AA) */
            dword10 |= bufferId;
            dword10 |= 0x00000008;

            SntiBuildFirmwareActivateCmd(pSrbExt, dword10);

            /* In this case there is no translation on the completion */
            pSrbExt->pNvmeCompletionRoutine = NULL;
        break;
        default:
            status = SNTI_INVALID_PARAMETER;
        break;
    } /* end switch */

    if (status != SNTI_SUCCESS) {
        /* Ensure correct sense data for SCSI compliance test case 1.4 */
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);

        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    }

    return returnStatus;
} /* SntiTranslateWriteBuffer */

/******************************************************************************
 * SntiTranslateSynchronizeCache
 *
 * @brief Translates the SCSI Synchrozize Cache 10/16 commands based on the NVMe
 *        Translation spec to a NVMe Flush command and populates a temporary SQE
 *        stored in the SRB Extension.
 *
 *        NOTE: The NVME FLUSH command is defined:
 *
 *        6.7 Flush command
 *          The Flush command is used by the host to indicate that any data in
 *          volatile storage should be flushed to non-volatile memory.
 *
 *        All command specific fields are reserved.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateSynchronizeCache(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    SNTI_STATUS status;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        /* Map the translation error to a SCSI error */
        SntiMapInternalErrorStatus(pSrb, status);
        return SNTI_FAILURE_CHECK_RESPONSE_DATA;
    }

    /* Set the SRB status to pending - controller communication necessary */
    pSrb->SrbStatus = SRB_STATUS_PENDING;

    /* Set the completion routine - no translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    /* Set up common portions of the NVMe Flush command */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));

    pSrbExt->nvmeSqeUnit.CDW0.OPC = NVME_FLUSH;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;
    pSrbExt->nvmeSqeUnit.NSID = pLunExt->namespaceId;

    return returnStatus;
} /* SntiTranslateSynchronizeCache */

/******************************************************************************
 * SntiTranslateTestUnitReady
 *
 * @brief Translates the SCSI Test Unit Ready command. Populates the appropriate
 *        SCSI fixed sense response data based on the NVMe Translation spec. Do
 *        not need to create SQE here as we just complete the command in the
 *        build phase (by returning FALSE to StorPort with SRB status of
 *        SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateTestUnitReady(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;

    /* Default to command completed */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;

    /* Set the completion routine - no translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    /*
     * Look at GenNextStartStat and ensure that the asynchronous state machine
     * has been successfully started.
     */
    if (pDevExt->DriverState.NextDriverState != NVMeStartComplete) {
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_NOT_READY,
                             SCSI_ADSENSE_LUN_NOT_READY,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
    }

    SET_DATA_LENGTH(pSrb, 0);

    return returnStatus;
} /* SntiTranslateTestUnitReady */

/******************************************************************************
 * SntiTranslateFormatUnit
 *
 * @brief Translates the SCSI Format Unit command.
 *        NOTE: Before performing the operation specified by this command, the
 *        device server shall stop all:
 *
 *           a) enabled power condition timers (see SPC-4):
 *           b) timers for enabled background scan operations (see 4.20); and
 *           c) timers or counters enabled for device-specific background
 *              functions.
 *
 *        After the operation is complete, the device server shall reinitialize
 *        and restart all enabled timers and counters for power conditions and
 *        background functions.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateFormatUnit(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PUINT8 pDataBuf = NULL;
    UINT8 protectionType = 0;
    UINT8 formatProtInfo = 0;
    UINT8 longList = 0;
    UINT8 formatData = 0;
    UINT8 completeList = 0;
    UINT8 defectListFormat = 0;
    UINT8 protFieldUsage = 0;
    UINT8 immed = 0;
    UINT8 protIntervalExp = 0;
    BOOLEAN shortParamList = FALSE;

    SNTI_STATUS status = SNTI_SUCCESS;
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);

    formatProtInfo =
        GET_U8_FROM_CDB(pSrb, FORMAT_UNIT_CDB_FORMAT_PROT_INFO_OFFSET);
    longList = GET_U8_FROM_CDB(pSrb, FORMAT_UNIT_CDB_LONG_LIST_OFFSET);
    formatData = GET_U8_FROM_CDB(pSrb, FORMAT_UNIT_CDB_FORMAT_DATA_OFFSET);
    completeList = GET_U8_FROM_CDB(pSrb,FORMAT_UNIT_CDB_COMPLETE_LIST_OFFSET);
    defectListFormat =
        GET_U8_FROM_CDB(pSrb,FORMAT_UNIT_CDB_DEFECT_LIST_FORMAT_OFFSET);

    formatProtInfo &= FORMAT_UNIT_CDB_FORMAT_PROT_INFO_MASK;
    longList &= FORMAT_UNIT_CDB_LONG_LIST_MASK;
    formatData &= FORMAT_UNIT_CDB_FORMAT_DATA_MASK;
    completeList &= FORMAT_UNIT_CDB_COMPLETE_LIST_MASK;
    defectListFormat &= FORMAT_UNIT_CDB_DEFECT_LIST_FORMAT_MASK;

    /* Set the completion routine - no translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    /*
     * FMTDATA: Unspecified. When set to 1b DEFECT LIST FORMAT is used.
     * CMPLIST: Unspecified. Indicates type of defect list (complete list).
     *          Ignored when FMTDATA is set to 1.
     * DEFECT LIST FORMAT: Unspecifed. If FMTDATA is 1, specifies format of
     *                     address descriptors in defect list.
     */
    if (formatProtInfo == 0) {
        /* Shall be supported by formatting unit w/o protection */

        /* Ignore LONGLIST */
    } else {
        if (longList == 0)
            shortParamList = TRUE;
        else
            shortParamList = FALSE;

        /* Ignore CMPLIST */

        /* DEFECT LIST FORMAT specifies format of address descriptors */
    }

    pDataBuf = (PUINT8)GET_DATA_BUFFER(pSrb);
    protFieldUsage = (*pDataBuf) & PROTECTION_FIELD_USAGE_MASK;
    immed = (*(++pDataBuf)) & FORMAT_UNIT_IMMED_MASK;

    /*
     * FORMAT UNIT Parameter List contains:
     *   - header
     *   - optional initialization pattern descriptor
     *   - optional defect list
     */
    if (immed != 0) {
        /* Ensure correct sense data for SCSI compliance test case 1.4 */
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);

        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
        status = SNTI_INVALID_REQUEST;
    } else {
        if (shortParamList == TRUE) {
            /* Short parameter list header */
        } else {
            /* Long parameter list header */
            protIntervalExp = (*(pDataBuf + 2)) &
                PROTECTION_INTERVAL_EXPONENT_MASK;
            if (protIntervalExp != 0) {
                SntiSetScsiSenseData(pSrb,
                                     SCSISTAT_CHECK_CONDITION,
                                     SCSI_SENSE_ILLEGAL_REQUEST,
                                     SCSI_ADSENSE_INVALID_CDB,
                                     SCSI_ADSENSE_NO_SENSE);

                pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
                SET_DATA_LENGTH(pSrb, 0);

                returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
                status = SNTI_INVALID_REQUEST;
            }
        }
    }

    if (status == SNTI_SUCCESS) {
        if (protFieldUsage == 0) {
            if ((formatProtInfo >> FORMAT_UNIT_CDB_FORMAT_PROT_INFO_SHIFT) == 0)
                protectionType = PROT_TYPE_0;
            else if ((formatProtInfo >>
                      FORMAT_UNIT_CDB_FORMAT_PROT_INFO_SHIFT) == 1)
                protectionType = PROT_TYPE_1;
            else if ((formatProtInfo >>
                      FORMAT_UNIT_CDB_FORMAT_PROT_INFO_SHIFT) == 2)
                protectionType = PROT_TYPE_2;
            else
                ASSERT(FALSE);
        } else if (protFieldUsage == 1) {
            if ((formatProtInfo >> FORMAT_UNIT_CDB_FORMAT_PROT_INFO_SHIFT) == 3)
                protectionType = PROT_TYPE_3;
        } else {
            /* All other values are unspecified */
            ASSERT(FALSE);
        }

        SntiBuildFormatNvmCmd(pSrbExt, protectionType);
    }

    return returnStatus;
} /* SntiTranslateFormatUnit */

/******************************************************************************
 * SntiTranslateLogSense
 *
 * @brief Translates the SCSI Log Sense command. Populates the appropriate SCSI
 *        Log Sense page data based on the NVMe Translation spec. Some log pages
 *        require controller communication and some do not and can be completed
 *        in the build I/O phase.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateLogSense(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    UINT8 saveParameters;
    UINT8 pageControl;
    UINT8 pageCode;

    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;

    /* Extract the Log Sense fields to determine the page */
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    saveParameters = GET_U8_FROM_CDB(pSrb, LOG_SENSE_CDB_SP_OFFSET);
    pageControl = GET_U8_FROM_CDB(pSrb, LOG_SENSE_CDB_PC_OFFSET);
    pageCode = GET_U8_FROM_CDB(pSrb, LOG_SENSE_CDB_PAGE_CODE_OFFSET);

    /* Mask and get SP, PC, and Page code */
    saveParameters &= LOG_SENSE_CDB_SP_MASK;
    pageControl &= LOG_SENSE_CDB_PC_MASK;
    pageCode &= LOG_SENSE_CDB_PAGE_CODE_MASK;

    /*
     * Set SRB status to success to indicate the command will complete
     * successfully (assuming no errors occur during translation) and
     * reset the status value to use below.
     */
    pSrb->SrbStatus = SRB_STATUS_SUCCESS;

    /* Set the completion callback routine to finish the command translation */
    pSrbExt->pNvmeCompletionRoutine = SntiCompletionCallbackRoutine;

    if ((saveParameters != LOG_SENSE_CDB_SP_NOT_ENABLED) ||
        ((pageControl >> LOG_SENSE_CDB_PC_SHIFT) != PC_CUMULATIVE_VALUES)) {
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        switch (pageCode) {
            case LOG_PAGE_SUPPORTED_LOG_PAGES_PAGE:
                SntiTranslateSupportedLogPages(pSrb);

                /* Command is completed in Build I/O phase */
                returnStatus = SNTI_COMMAND_COMPLETED;

                /* Override the completion routine... finished here */
                pSrbExt->pNvmeCompletionRoutine = NULL;
            break;
            case LOG_PAGE_TEMPERATURE_PAGE:
                /* 
                 * These log pages requires a Get Log Page (SMART/Health Info
                 * Log) command and uses the field "Temperature" (after
                 * converting from celsius to Kelvin) to set the "Most Recent
                 * Temperature Reading" field in the SCSI log page. A data
                 * buffer must be allocated here since the SMART/Health Info log
                 * page is 512 bytes long. Therefore, the translation must be
                 * performed on the completion side of this command.
                 *
                 * NOTE: Use the SRB Extension from the Log Sense command.
                 * NOTE: The Temp log page requires a parameter from Get
                 *       Features command and a parameter from Get Log Page
                 *       (Tempterature) command.
                 */
                SntiTranslateTemperature(pSrb);

                /*
                 * Fall through to finish setting up the PRP entries and GET LOG
                 * PAGE command
                 */
                pSrbExt->dataBufferSize =
                    sizeof(ADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY);

                pSrbExt->pDataBuffer =
                    SntiAllocatePhysicallyContinguousBuffer(pSrbExt, pSrbExt->dataBufferSize);

                if (pSrbExt->pDataBuffer != NULL) {
                    SntiBuildGetLogPageCmd(pSrbExt, SMART_HEALTH_INFORMATION);
                    returnStatus = SNTI_TRANSLATION_SUCCESS;
                } else {
                    SntiSetScsiSenseData(pSrb,
                                         SCSISTAT_CHECK_CONDITION,
                                         SCSI_SENSE_UNIQUE,
                                         SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
                                         SCSI_ADSENSE_NO_SENSE);

                    pSrb->SrbStatus |= SRB_STATUS_ERROR;
                    SET_DATA_LENGTH(pSrb, 0);
                    returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
                }
            break;
            case LOG_PAGE_INFORMATIONAL_EXCEPTIONS_PAGE:
                /*
                 * These log pages requires a Get Log Page (SMART/Health Info
                 * Log) command and uses the field "Temperature" (after
                 * converting from celsius to Kelvin) to set the "Most Recent
                 * Temperature Reading" field in the SCSI log page. A data
                 * buffer must be allocated here since the SMART/Health Info log
                 * page is 512 bytes long. Therefore, the translation must be
                 * performed on the completion side of this command.
                 */
                pSrbExt->dataBufferSize =
                  sizeof(ADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY);

                pSrbExt->pDataBuffer =
                    SntiAllocatePhysicallyContinguousBuffer(pSrbExt, pSrbExt->dataBufferSize);

                if (pSrbExt->pDataBuffer != NULL) {
                    SntiBuildGetLogPageCmd(pSrbExt, SMART_HEALTH_INFORMATION);
                    returnStatus = SNTI_TRANSLATION_SUCCESS;
                } else {
                    SntiSetScsiSenseData(pSrb,
                                         SCSISTAT_CHECK_CONDITION,
                                         SCSI_SENSE_UNIQUE,
                                         SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
                                         SCSI_ADSENSE_NO_SENSE);

                    pSrb->SrbStatus |= SRB_STATUS_ERROR;
                    SET_DATA_LENGTH(pSrb, 0);
                    returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
                }
            break;
            default:
                SntiSetScsiSenseData(pSrb,
                                     SCSISTAT_CHECK_CONDITION,
                                     SCSI_SENSE_ILLEGAL_REQUEST,
                                     SCSI_ADSENSE_INVALID_CDB,
                                     SCSI_ADSENSE_NO_SENSE);

                pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
                SET_DATA_LENGTH(pSrb, 0);

                returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
            break;
        } /* end switch */
    }

    return returnStatus;
} /* SntiTranslateLogSense */

/******************************************************************************
 * SntiTranslateSupportedLogPages
 *
 * @brief Translates the Log Sense page - Supported Log Pages Page. Populates
 *        the appropriate log page response fields based on the NVMe Translation
 *        spec. Do not need to create SQE here as we just complete the command
 *        in the build phase (by returning FALSE to StorPort with SRB status of
 *        SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateSupportedLogPages(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PSUPPORTED_LOG_PAGES_LOG_PAGE pLogPage = NULL;
    UINT16 allocLength;

    pLogPage = (PSUPPORTED_LOG_PAGES_LOG_PAGE)GET_DATA_BUFFER(pSrb);
    allocLength = GET_U16_FROM_CDB(pSrb, LOG_SENSE_CDB_ALLOC_LENGTH_OFFSET);

    memset(pLogPage, 0, sizeof(SUPPORTED_LOG_PAGES_LOG_PAGE));
    pLogPage->PageCode = LOG_PAGE_SUPPORTED_LOG_PAGES_PAGE;
    pLogPage->SubPageFormat = SUB_PAGE_FORMAT_UNSUPPORTED;
    pLogPage->DisableSave = DISABLE_SAVE_UNSUPPORTED;
    pLogPage->SubPageCode = SUB_PAGE_CODE_UNSUPPORTED;
    pLogPage->PageLength[BYTE_0] = 0;
    pLogPage->PageLength[BYTE_1] = SUPPORTED_LOG_PAGES_PAGE_LENGTH;
    pLogPage->supportedPages[BYTE_0].PageCode =
        LOG_PAGE_SUPPORTED_LOG_PAGES_PAGE;
    pLogPage->supportedPages[BYTE_1].PageCode =
        LOG_PAGE_TEMPERATURE_PAGE;
    pLogPage->supportedPages[BYTE_2].PageCode =
        LOG_PAGE_INFORMATIONAL_EXCEPTIONS_PAGE;

    SET_DATA_LENGTH(pSrb,
        min(sizeof(SUPPORTED_LOG_PAGES_LOG_PAGE), allocLength));
} /* SntiTranslateSupportedLogPages */

/******************************************************************************
 * SntiTranslateTemperature
 *
 * @brief Translates the Log Sense page - Temperature Log Page. Populates the
 *        appropriate log page response fields based on the NVMe Translation
 *        spec. This log page requires both local data storage and adapter
 *        communication. Create the NVMe Admin command - Get Log Page:
 *        SMART/Health Information and populates the temporary SQE stored in the
 *        SRB Extension.
 *
 *        NOTE: Must free extra buffer used for this command.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateTemperature(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{

    /* Set up the Temperature Log Page in the SRB Data Buffer */
    PTEMPERATURE_LOG_PAGE pLogPage = NULL;
    pLogPage = (PTEMPERATURE_LOG_PAGE)GET_DATA_BUFFER(pSrb);

    memset(pLogPage, 0, sizeof(TEMPERATURE_LOG_PAGE));
    pLogPage->PageCode = LOG_PAGE_TEMPERATURE_PAGE;
    pLogPage->SubPageFormat = SUB_PAGE_FORMAT_UNSUPPORTED;
    pLogPage->DisableSave = DISABLE_SAVE_UNSUPPORTED;
    pLogPage->SubPageCode = SUB_PAGE_CODE_UNSUPPORTED;
    pLogPage->PageLength[BYTE_0] = 0;
    pLogPage->PageLength[BYTE_1] = REMAINING_TEMP_PAGE_LENGTH;
    pLogPage->ParameterCode_Temp[BYTE_0] = TEMPERATURE_PARM_CODE;
    pLogPage->ParameterCode_Temp[BYTE_1] = TEMPERATURE_PARM_CODE;
    pLogPage->FormatAndLinking_Temp = BINARY_FORMAT;
    pLogPage->TMC_Temp = TMC_UNSUPPORTED;
    pLogPage->ETC_Temp = ETC_UNSUPPORTED;
    pLogPage->TSD_Temp = TSD_UNSUPPORTED;
    pLogPage->DU_Temp = DU_UNSUPPORTED;
    pLogPage->ParameterLength_Temp = TEMP_PARM_LENGTH;
    pLogPage->Temperature = 0; /* To be filled after Get Log Page */

    pLogPage->ParameterCode_RefTemp[BYTE_0] = 0;
    pLogPage->ParameterCode_RefTemp[BYTE_1] = REFERENCE_TEMPERATURE_PARM_CODE;
    pLogPage->FormatAndLinking_RefTemp = BINARY_FORMAT;
    pLogPage->TMC_RefTemp = TMC_UNSUPPORTED;
    pLogPage->ETC_RefTemp = ETC_UNSUPPORTED;
    pLogPage->TSD_RefTemp = TSD_UNSUPPORTED;
    pLogPage->DU_RefTemp = DU_UNSUPPORTED;
    pLogPage->ParameterLength_RefTemp = REF_TEMP_PARM_LENGTH;
    pLogPage->ReferenceTemperature = 0; /* To be filled after Get Log Page */

    pSrb->SrbStatus = SRB_STATUS_PENDING;
} /* SntiTranslateTemperature */

/******************************************************************************
 * SntiTranslateModeSense
 *
 * @brief Translates the SCSI Mode Sense command. Populates the appropriate SCSI
 *        Mode Sense page data based on the NVMe Translation spec. Some pages
 *        require controller communication and others can be completed in the
 *        build phase.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param supportsVwc - the ID data that indicates if its legal to sent a VWC
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicate translation status.
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateModeSense(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    BOOLEAN supportsVwc
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    UINT16 allocLength;
    UINT8 longLbaAccepted;
    UINT8 disableBlockDesc;
    UINT8 pageControl;
    UINT8 pageCode;
    UINT8 subPageCode;
    BOOLEAN modeSense10;

    SNTI_STATUS status = SNTI_SUCCESS;
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;

    /* Extract the Log Sense fields to determine the page */
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    longLbaAccepted = GET_U8_FROM_CDB(pSrb, MODE_SENSE_CDB_LLBAA_OFFSET);
    disableBlockDesc = GET_U8_FROM_CDB(pSrb, MODE_SENSE_CDB_DBD_OFFSET);
    pageControl = GET_U8_FROM_CDB(pSrb, MODE_SENSE_CDB_PAGE_CONTROL_OFFSET);
    pageCode = GET_U8_FROM_CDB(pSrb, MODE_SENSE_CDB_PAGE_CODE_OFFSET);
    subPageCode = GET_U8_FROM_CDB(pSrb, MODE_SENSE_CDB_SUBPAGE_CODE_OFFSET);

    if (GET_OPCODE(pSrb) == SCSIOP_MODE_SENSE) {
        /* MODE SENSE 6 */
        allocLength =
            GET_U8_FROM_CDB(pSrb, MODE_SENSE_6_CDB_ALLOC_LENGTH_OFFSET);
        modeSense10 = FALSE;
    } else {
        /* MODE SENSE 10 */
        allocLength =
            GET_U16_FROM_CDB(pSrb, MODE_SENSE_10_CDB_ALLOC_LENGTH_OFFSET);
        modeSense10 = TRUE;
    }

    /* Mask and get LLBAA, DBD, PC, and Page code */
    longLbaAccepted = (longLbaAccepted & MODE_SENSE_CDB_LLBAA_MASK) >>
                       MODE_SENSE_CDB_LLBAA_SHIFT;
    disableBlockDesc = (disableBlockDesc & MODE_SENSE_CDB_DBD_MASK) >>
                        MODE_SENSE_CDB_DBD_SHIFT;
    pageControl = (pageControl & MODE_SENSE_CDB_PAGE_CONTROL_MASK) >>
                   MODE_SENSE_CDB_PAGE_CONTROL_SHIFT;
    pageCode &= MODE_SENSE_CDB_PAGE_CODE_MASK;

    memset((PVOID)(pSrbExt->modeSenseBuf), 0, MODE_SNS_MAX_BUF_SIZE);

    /* Set the completion routine - no translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    if (((pageControl != MODE_SENSE_PC_CURRENT_VALUES)    &&
         (pageControl != MODE_SENSE_PC_CHANGEABLE_VALUES) &&
         (pageControl != MODE_SENSE_PC_DEFAULT_VALUES))   ||
        ((pageCode != MODE_SENSE_RETURN_ALL)      &&
         (pageCode != MODE_PAGE_CACHING)          &&
         (pageCode != MODE_PAGE_CONTROL)          &&
         (pageCode != MODE_PAGE_POWER_CONDITION)  &&
         (pageCode != MODE_PAGE_FAULT_REPORTING))) {
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_ILLEGAL_COMMAND,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    } else {
        status = GetLunExtension(pSrbExt, &pLunExt);
        if (status != SNTI_SUCCESS) {
            SntiMapInternalErrorStatus(pSrb, status);
            returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
        } else {
            switch (pageCode) {
                case MODE_PAGE_CACHING:
                    /* Per NVMe we can't send VWC commands if its not supported */
                    if (supportsVwc == FALSE) {
                    SntiHardCodeCacheModePage(pSrbExt,
                                              pLunExt,
                                              allocLength,
                                              longLbaAccepted,
                                              disableBlockDesc,
                                              modeSense10);

                    pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                    returnStatus = SNTI_COMMAND_COMPLETED;
                    } else {
                    /*
                     * This mode page requires a paramter from Get Features.
                     * Must send Get Features w/ Volatile Write Cache Feature
                     * Identifier and set to the CACHE MODE PAGE field of Write
                     * Back Cache Enable (WCE). However, we can still use the
                     * SRB data buffer since the Get Feature command will
                     * return the data in one of the DWORDs in the completion
                     * queue entry. Therefore, the translation will be done on
                     * the completion side after we get the WCE info.
                     */
                    SntiBuildGetFeaturesCmd(pSrbExt, VOLATILE_WRITE_CACHE);

                    returnStatus = SNTI_TRANSLATION_SUCCESS;
                    pSrb->SrbStatus = SRB_STATUS_PENDING;

                    /*
                     * Override the completion routine - translation necessary
                     * on completion
                     */
                    pSrbExt->pNvmeCompletionRoutine =
                        SntiCompletionCallbackRoutine;
                    }
                break;
                case MODE_PAGE_CONTROL:
                    SntiCreateControlModePage(pSrbExt,
                                              pLunExt,
                                              allocLength,
                                              longLbaAccepted,
                                              disableBlockDesc,
                                              modeSense10);

                    /* Command is completed in Build I/O phase */
                    pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                    returnStatus = SNTI_COMMAND_COMPLETED;
                break;
                case MODE_PAGE_POWER_CONDITION:
                    SntiCreatePowerConditionControlModePage(pSrbExt,
                                                            pLunExt,
                                                            allocLength,
                                                            longLbaAccepted,
                                                            disableBlockDesc,
                                                            modeSense10);

                    /* Command is completed in Build I/O phase */
                    pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                    returnStatus = SNTI_COMMAND_COMPLETED;
                break;
                case MODE_PAGE_FAULT_REPORTING:
                    SntiCreateInformationalExceptionsControlModePage(
                        pSrbExt,
                        pLunExt,
                        allocLength,
                        longLbaAccepted,
                        disableBlockDesc,
                        modeSense10);

                    /* Command is completed in Build I/O phase */
                    pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                    returnStatus = SNTI_COMMAND_COMPLETED;
                break;
                case MODE_SENSE_RETURN_ALL:
	            returnStatus = SntiReturnAllModePages(pSrbExt,
                                                          pLunExt,
                                                          allocLength,
                                                          longLbaAccepted,
                                                          disableBlockDesc,
                                                          modeSense10,
                                                          supportsVwc);
                break;
                default:
                    SntiSetScsiSenseData(pSrb,
                                         SCSISTAT_CHECK_CONDITION,
                                         SCSI_SENSE_ILLEGAL_REQUEST,
                                         SCSI_ADSENSE_ILLEGAL_COMMAND,
                                         SCSI_ADSENSE_NO_SENSE);

                    pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
                    SET_DATA_LENGTH(pSrb, 0);
                    returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
                break;
            } /* end switch */

            if (status != SNTI_SUCCESS) {
                pSrb->SrbStatus = SRB_STATUS_ERROR;
                returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
            }
        }
    }

    return returnStatus;
} /* SntiTranslateModeSense */

/******************************************************************************
 * SntiCreateControlModePage
 *
 * @brief Create the Mode Sense page - Caching Mode Page. Populates the
 *        appropriate mode page response fields based on the NVMe Translation
 *        spec. Do not need to create SQE here as we just complete the command
 *        in the build phase (by returning FALSE to StorPort with SRB status of
 *        SUCCESS).
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 * @param allocLength - Allocation length from Mode Sense CDB
 * @param longLbaAccepted - LLBAA bit from Mode Sense CDB
 * @param disableBlockDesc - DBD bit from Mode Sense CDB
 * @param modeSense10 - Boolean to determine Mode Sense 10
 *
 * @return VOID
 ******************************************************************************/
VOID SntiCreateControlModePage(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10
)
{
    PMODE_PARAMETER_HEADER pModeHeader6 = NULL;
    PMODE_PARAMETER_HEADER10 pModeHeader10 = NULL;
    PMODE_PARAMETER_BLOCK pModeParamBlock = NULL;
    PCONTROL_MODE_PAGE pControlModePage = NULL;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    UINT16 modeDataLength = 0;
    UINT16 blockDescLength = 0;

    /* Determine which Mode Parameter Descriptor Block to use (8 or 16) */
    if (longLbaAccepted == 0)
        blockDescLength = SHORT_DESC_BLOCK;
    else
        blockDescLength = LONG_DESC_BLOCK;

    /* Mode Page Header */
    SntiCreateModeDataHeader(pSrbExt,
                             &pModeParamBlock,
                             &modeDataLength,
                             (disableBlockDesc ? 0 : blockDescLength),
                             modeSense10);


    /* Check if block descriptors enabled... if not then mode page comes next */
    if (disableBlockDesc == BLOCK_DESCRIPTORS_ENABLED)
    {
        /* Mode Parameter Descriptor Block */
    SntiCreateModeParameterDescBlock(pLunExt,
                                     pModeParamBlock,
                                     &modeDataLength);

        /* Increment pointer to after block descriptor */
        pModeParamBlock++;
    }

    /* Control Mode Page */
    pControlModePage = (PCONTROL_MODE_PAGE)pModeParamBlock;
    modeDataLength += sizeof(CONTROL_MODE_PAGE);

    memset(pControlModePage, 0, sizeof(CONTROL_MODE_PAGE));
    pControlModePage->PageSaveable   = MODE_PAGE_PARAM_SAVEABLE_DISABLED;
    pControlModePage->SubPageFormat  = 0;
    pControlModePage->PageCode       = MODE_PAGE_CONTROL;
    pControlModePage->PageLength     = sizeof(CONTROL_MODE_PAGE) - 2;
    pControlModePage->TST            = ONE_TASK_SET;
    pControlModePage->TMF_Only       = ACA_UNSUPPORTED;
    pControlModePage->DPICZ          = RESERVED_FIELD;
    pControlModePage->D_Sense        = SENSE_DATA_DESC_FORMAT;
    pControlModePage->GLTSD          = LOG_PARMS_NOT_IMPLICITLY_SAVED_PER_LUN;
    pControlModePage->RLEC           = LOG_EXCP_COND_NOT_REPORTED;
    pControlModePage->QAlgMod        = CMD_REORDERING_SUPPORTED;
    pControlModePage->NUAR           = RESERVED_FIELD;
    pControlModePage->QERR           = 1; /* TBD: No spec defintion??? */
    pControlModePage->RAC            = BUSY_RETURNS_ENABLED;
    pControlModePage->UA_INTLCK_CTRL = UA_CLEARED_AT_CC_STATUS;
    pControlModePage->SWP            = SW_WRITE_PROTECT_UNSUPPORTED;
    pControlModePage->ATO            = LBAT_LBRT_NOT_MODIFIABLE;
    pControlModePage->TAS            = TASK_ABORTED_STATUS_FOR_ABORTED_CMDS;
    pControlModePage->AutoLodeMode   = MEDIUM_LOADED_FULL_ACCESS;

    pControlModePage->BusyTimeoutPeriod[BYTE_0]   = UNLIMITED_BUSY_TIMEOUT_HIGH;
    pControlModePage->BusyTimeoutPeriod[BYTE_1]   = UNLIMITED_BUSY_TIMEOUT_LOW;
    pControlModePage->ExtSelfTestCompTime[BYTE_0] =
        SMART_SELF_TEST_UNSUPPORTED_HIGH;
    pControlModePage->ExtSelfTestCompTime[BYTE_1] =
        SMART_SELF_TEST_UNSUPPORTED_LOW;

    /* Now go back and set the Mode Data Length in the header */
    if (modeSense10 == FALSE) {
        /* Get the correct header that starts at the buffer beginning */
        pModeHeader6 = (PMODE_PARAMETER_HEADER)(pSrbExt->modeSenseBuf);

        /* Subtract 1 from mode data length - MODE DATA LENGTH field */
        pModeHeader6->ModeDataLength = (UCHAR)(modeDataLength - 1);
    } else {
        /* Get the correct header that starts at the buffer beginning */
        pModeHeader10 = (PMODE_PARAMETER_HEADER10)(pSrbExt->modeSenseBuf);

        /* Subtract 2 from mode data length - MODE DATA LENGTH field */
        pModeHeader10->ModeDataLength[BYTE_0] =
            ((modeDataLength - 2) & WORD_HIGH_BYTE_MASK) >> BYTE_SHIFT_1;
        pModeHeader10->ModeDataLength[BYTE_1] =
            ((modeDataLength - 2) & WORD_LOW_BYTE_MASK);
    }

    SET_DATA_LENGTH(pSrb, min(modeDataLength, allocLength));
    StorPortCopyMemory((PVOID)GET_DATA_BUFFER(pSrb),
        (PVOID)(pSrbExt->modeSenseBuf), GET_DATA_LENGTH(pSrb));

} /* SntiCreateControlModePage*/

VOID SntiHardCodeCacheModePage(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10
)
{
    PMODE_PARAMETER_HEADER pModeHeader6 = NULL;
    PMODE_PARAMETER_HEADER10 pModeHeader10 = NULL;
    PMODE_PARAMETER_BLOCK pModeParamBlock = NULL;
    PCACHING_MODE_PAGE pCachingModePage = NULL;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    UINT16 modeDataLength = 0;
    UINT16 blockDescLength = 0;

    /* Determine which Mode Parameter Descriptor Block to use (8 or 16) */
    if (longLbaAccepted == 0)
        blockDescLength = SHORT_DESC_BLOCK;
    else
        blockDescLength = LONG_DESC_BLOCK;

    /* Mode Page Header */
    SntiCreateModeDataHeader(pSrbExt,
                             &pModeParamBlock,
                             &modeDataLength,
                             (disableBlockDesc ? 0 : blockDescLength),
                             modeSense10);

    /* Check if block descriptors enabled, if not then mode page comes next */
    if (disableBlockDesc == BLOCK_DESCRIPTORS_ENABLED)
    {
        /* Mode Parameter Descriptor Block */
        SntiCreateModeParameterDescBlock(pLunExt,
                                         pModeParamBlock,
                                         &modeDataLength);

        /* Increment pointer to after block descriptor */
        pModeParamBlock++;
    }

    /* Cache Mode Page */
    pCachingModePage = (PCACHING_MODE_PAGE)pModeParamBlock;
    modeDataLength += sizeof(CACHING_MODE_PAGE);

    memset(pCachingModePage, 0, sizeof(CACHING_MODE_PAGE));
    pCachingModePage->PageCode         = MODE_PAGE_CACHING;
    pCachingModePage->PageSavable      = MODE_PAGE_PARAM_SAVEABLE_DISABLED;
    pCachingModePage->PageLength       = CACHING_MODE_PAGE_LENGTH;
    pCachingModePage->WriteCacheEnable = 0;

    /* Now go back and set the Mode Data Length in the header */
    if (modeSense10 == FALSE) {
        pModeHeader6 = (PMODE_PARAMETER_HEADER)(pSrbExt->modeSenseBuf);

        /* Subtract 1 from the mode data length - MODE DATA LENGTH field */
        pModeHeader6->ModeDataLength = (UCHAR)(modeDataLength - 1);
    } else {
        pModeHeader10 = (PMODE_PARAMETER_HEADER10)(pSrbExt->modeSenseBuf);

        /* Subtract 2 from mode data length - MODE DATA LENGTH field */
        pModeHeader10->ModeDataLength[BYTE_0] = ((modeDataLength - 2) &
            WORD_HIGH_BYTE_MASK) >> BYTE_SHIFT_1;
        pModeHeader10->ModeDataLength[BYTE_1] = ((modeDataLength - 2) &
            WORD_LOW_BYTE_MASK);
    }

    SET_DATA_LENGTH(pSrb, min(modeDataLength, allocLength));
    StorPortCopyMemory((PVOID)GET_DATA_BUFFER(pSrb),
        (PVOID)(pSrbExt->modeSenseBuf), GET_DATA_LENGTH(pSrb));
} /* SntiHardCodeCacheModePage */

/******************************************************************************
 * SntiCreatePowerConditionControlModePage
 *
 * @brief Creates the Mode Sense page - Control Mode Page. Populates the
 *        appropriate mode page response fields based on the NVMe Translation
 *        spec. Do not need to create SQE here as we just complete the command
 *        in the build phase (by returning FALSE to StorPort with SRB status of
 *        SUCCESS).
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 * @param allocLength - Allocation length from Mode Sense CDB
 * @param longLbaAccepted - LLBAA bit from Mode Sense CDB
 * @param disableBlockDesc - DBD bit from Mode Sense CDB
 * @param modeSense10 - Boolean to determine Mode Sense 10
 *
 * @return VOID
 ******************************************************************************/
VOID SntiCreatePowerConditionControlModePage(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10
)
{
    PMODE_PARAMETER_HEADER pModeHeader6 = NULL;
    PMODE_PARAMETER_HEADER10 pModeHeader10 = NULL;
    PMODE_PARAMETER_BLOCK pModeParamBlock = NULL;
    PPOWER_CONDITION_MODE_PAGE pPowerConditionModePage = NULL;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    UINT16 modeDataLength = 0;
    UINT16 blockDescLength = 0;

    /* Determine which Mode Parameter Descriptor Block to use (8 or 16) */
    if (longLbaAccepted == 0)
        blockDescLength = SHORT_DESC_BLOCK;
    else
        blockDescLength = LONG_DESC_BLOCK;

    /* Mode Page Header */
    SntiCreateModeDataHeader(pSrbExt,
                             &pModeParamBlock,
                             &modeDataLength,
                             (disableBlockDesc ? 0 : blockDescLength),
                             modeSense10);

    /* Check if block descriptors enabled, if not then mode page comes next */
    if (disableBlockDesc == BLOCK_DESCRIPTORS_ENABLED)
    {
        /* Mode Parameter Descriptor Block */
        SntiCreateModeParameterDescBlock(pLunExt,
                                         pModeParamBlock,
                                         &modeDataLength);

        /* Increment pointer to after block descriptor */
        pModeParamBlock++;
    }

    /* Power Condition Control Mode Page */
    pPowerConditionModePage = (PPOWER_CONDITION_MODE_PAGE)pModeParamBlock;
    modeDataLength += sizeof(POWER_CONDITION_MODE_PAGE);

    memset(pPowerConditionModePage, 0, sizeof(POWER_CONDITION_MODE_PAGE));
    pPowerConditionModePage->PageCode        = MODE_PAGE_POWER_CONDITION;
    pPowerConditionModePage->PageLength      = POWER_COND_MODE_PAGE_LENGTH;
    pPowerConditionModePage->PmBgInteraction = 0;

    /* Now go back and set the Mode Data Length in the header */
    if (modeSense10 == FALSE) {
        pModeHeader6 = (PMODE_PARAMETER_HEADER)(pSrbExt->modeSenseBuf);

        /* Subtract 1 from the mode data length - MODE DATA LENGTH field */
        pModeHeader6->ModeDataLength = (UCHAR)(modeDataLength - 1);
    } else {
        pModeHeader10 = (PMODE_PARAMETER_HEADER10)(pSrbExt->modeSenseBuf);

        /* Subtract 2 from mode data length - MODE DATA LENGTH field */
        pModeHeader10->ModeDataLength[BYTE_0] = ((modeDataLength - 2) &
            WORD_HIGH_BYTE_MASK) >> BYTE_SHIFT_1;
        pModeHeader10->ModeDataLength[BYTE_1] = ((modeDataLength - 2) &
            WORD_LOW_BYTE_MASK);
    }

    SET_DATA_LENGTH(pSrb, min(modeDataLength, allocLength));
    StorPortCopyMemory((PVOID)GET_DATA_BUFFER(pSrb),
        (PVOID)(pSrbExt->modeSenseBuf), GET_DATA_LENGTH(pSrb));
} /* SntiCreatePowerConditionControlModePage */

/******************************************************************************
 * SntiCreateInformationalExceptionsControlModePage
 *
 * @brief Creates the Mode Sense page - Information Exceptions Mode Page.
 *        Populates the appropriate mode page response fields based on the NVMe
 *        Translation spec. Do not need to create SQE here as we just complete
 *        the command in the build phase (by returning FALSE to StorPort with
 *        SRB status of SUCCESS).
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 * @param allocLength - Allocation length from Mode Sense CDB
 * @param longLbaAccepted - LLBAA bit from Mode Sense CDB
 * @param disableBlockDesc - DBD bit from Mode Sense CDB
 * @param modeSense10 - Boolean to determine Mode Sense 10
 *
 * @return VOID
 ******************************************************************************/
VOID SntiCreateInformationalExceptionsControlModePage(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10
)
{
    PMODE_PARAMETER_HEADER pModeHeader6 = NULL;
    PMODE_PARAMETER_HEADER10 pModeHeader10 = NULL;
    PMODE_PARAMETER_BLOCK pModeParamBlock = NULL;
    PINFO_EXCEPTIONS_MODE_PAGE pInfoExceptionsModePage = NULL;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    UINT16 modeDataLength = 0;
    UINT16 blockDescLength = 0;

    /* Determine which Mode Parameter Descriptor Block to use (8 or 16) */
    if (longLbaAccepted == 0)
        blockDescLength = SHORT_DESC_BLOCK;
    else
        blockDescLength = LONG_DESC_BLOCK;

    /* Mode Page Header */
    SntiCreateModeDataHeader(pSrbExt,
                             &pModeParamBlock,
                             &modeDataLength,
                             (disableBlockDesc ? 0 : blockDescLength),
                             modeSense10);

    /* Check if block descriptors enabled, if not then mode page comes next */
    if (disableBlockDesc == BLOCK_DESCRIPTORS_ENABLED)
    {
        /* Mode Parameter Descriptor Block */
        SntiCreateModeParameterDescBlock(pLunExt,
                                         pModeParamBlock,
                                         &modeDataLength);

        /* Increment pointer to after block descriptor */
        pModeParamBlock++;
    }

    /* Informational Exceptions Control Mode Page */
    pInfoExceptionsModePage = (PINFO_EXCEPTIONS_MODE_PAGE)pModeParamBlock;
    modeDataLength += sizeof(INFO_EXCEPTIONS_MODE_PAGE);

    memset(pInfoExceptionsModePage, 0, sizeof(INFO_EXCEPTIONS_MODE_PAGE));
    pInfoExceptionsModePage->PageCode   = MODE_PAGE_FAULT_REPORTING;
    pInfoExceptionsModePage->PageLength = INFO_EXCP_MODE_PAGE_LENGTH;

    /* NOTE: All values shall be zero by default */

    /* Now go back and set the Mode Data Length in the header */
    if (modeSense10 == FALSE) {
        pModeHeader6 = (PMODE_PARAMETER_HEADER)(pSrbExt->modeSenseBuf);

        /* Subtract 1 from the mode data length - MODE DATA LENGTH field*/
        pModeHeader6->ModeDataLength = (UCHAR)(modeDataLength - 1);
    } else {
        pModeHeader10 = (PMODE_PARAMETER_HEADER10)(pSrbExt->modeSenseBuf);

        /* Subtract 2 from mode data length - MODE DATA LENGTH field */
        pModeHeader10->ModeDataLength[BYTE_0] = ((modeDataLength - 2) &
            WORD_HIGH_BYTE_MASK) >> BYTE_SHIFT_1;
        pModeHeader10->ModeDataLength[BYTE_1] = ((modeDataLength - 2) &
            WORD_LOW_BYTE_MASK);
    }

    SET_DATA_LENGTH(pSrb, min(modeDataLength, allocLength));
    StorPortCopyMemory((PVOID)GET_DATA_BUFFER(pSrb),
        (PVOID)(pSrbExt->modeSenseBuf), GET_DATA_LENGTH(pSrb));

} /* SntiCreateInformationalExceptionsControlModePage*/

/******************************************************************************
 * SntiReturnAllModePages
 *
 * @brief Returns all supported mode pages
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 * @param allocLength - Allocation length from Mode Sense CDB
 * @param longLbaAccepted - LLBAA bit from Mode Sense CDB
 * @param disableBlockDesc - DBD bit from Mode Sense CDB
 * @param modeSense10 - Boolean to determine Mode Sense 10
 * @param supportsVwc - From ID data, tells us if we can send VWC cmds or not
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiReturnAllModePages(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10,
    BOOLEAN supportsVwc
)
{
    PMODE_PARAMETER_HEADER pModeHeader6 = NULL;
    PMODE_PARAMETER_HEADER10 pModeHeader10 = NULL;
    PMODE_PARAMETER_BLOCK pModeParamBlock = NULL;
    PCONTROL_MODE_PAGE pControlModePage = NULL;
    PPOWER_CONDITION_MODE_PAGE pPowerConditionModePage = NULL;
    PCACHING_MODE_PAGE pCachingModePage = NULL;
    PINFO_EXCEPTIONS_MODE_PAGE pInfoExceptionsModePage = NULL;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    UINT16 modeDataLength = 0;
    UINT16 blockDescLength = 0;
    SNTI_TRANSLATION_STATUS returnStatus;
    PNVME_DEVICE_EXTENSION pAE = NULL;
    pAE = pSrbExt->pNvmeDevExt;

    memset(GET_DATA_BUFFER(pSrb), 0, min(GET_DATA_LENGTH(pSrb), allocLength));

    /* Determine which Mode Parameter Descriptor Block to use (8 or 16) */
    if (longLbaAccepted == 0)
        blockDescLength = SHORT_DESC_BLOCK;
    else
        blockDescLength = LONG_DESC_BLOCK;

    /* Only use the 8 byte mode parameter block desc... spec errata */

    /* Mode Page Header */
    SntiCreateModeDataHeader(pSrbExt,
                             &pModeParamBlock,
                             &modeDataLength,
                             (disableBlockDesc ? 0 : blockDescLength),
                             modeSense10);

    /* Check if block descriptors enabled, if not then mode pages comes next */
    if (disableBlockDesc == BLOCK_DESCRIPTORS_ENABLED)
    {
        /* Mode Parameter Descriptor Block */
        SntiCreateModeParameterDescBlock(pLunExt,
                                         pModeParamBlock,
                                         &modeDataLength);

        /* Increment pointer to after block descriptor */
        pModeParamBlock++;
    }

    /* Caching Mode Page */
    pCachingModePage = (PCACHING_MODE_PAGE)pModeParamBlock;
    modeDataLength += sizeof(CACHING_MODE_PAGE);

    memset(pCachingModePage, 0, sizeof(CACHING_MODE_PAGE));
    pCachingModePage->PageCode         = MODE_PAGE_CACHING;
    pCachingModePage->PageSavable      = MODE_PAGE_PARAM_SAVEABLE_DISABLED;
    pCachingModePage->PageLength       = CACHING_MODE_PAGE_LENGTH;
    pCachingModePage->WriteCacheEnable = 0; /* Filled in on completion side */

    /* Increment pointer to after the control mode page */
    pCachingModePage++;

    /* Control Mode Page */
    pControlModePage = (PCONTROL_MODE_PAGE)pCachingModePage;
    modeDataLength += sizeof(CONTROL_MODE_PAGE);

    memset(pControlModePage, 0, sizeof(CONTROL_MODE_PAGE));
    pControlModePage->PageCode       = MODE_PAGE_CONTROL;
    pControlModePage->PageSaveable   = MODE_PAGE_PARAM_SAVEABLE_DISABLED;
    pControlModePage->SubPageFormat  = SUB_PAGE_FORMAT_UNSUPPORTED;
    pControlModePage->PageLength     = CONTROL_MODE_PAGE_SIZE;
    pControlModePage->TST            = ONE_TASK_SET;
    pControlModePage->TMF_Only       = ACA_UNSUPPORTED;
    pControlModePage->DPICZ          = RESERVED_FIELD;
    pControlModePage->D_Sense        = SENSE_DATA_DESC_FORMAT;
    pControlModePage->GLTSD          = LOG_PARMS_NOT_IMPLICITLY_SAVED_PER_LUN;
    pControlModePage->RLEC           = LOG_EXCP_COND_NOT_REPORTED;
    pControlModePage->QAlgMod        = CMD_REORDERING_SUPPORTED;
    pControlModePage->NUAR           = RESERVED_FIELD;
    pControlModePage->QERR           = 1; /* TBD: No spec defintion??? */
    pControlModePage->RAC            = BUSY_RETURNS_ENABLED;
    pControlModePage->UA_INTLCK_CTRL = UA_CLEARED_AT_CC_STATUS;
    pControlModePage->SWP            = SW_WRITE_PROTECT_UNSUPPORTED;
    pControlModePage->ATO            = LBAT_LBRT_NOT_MODIFIABLE;
    pControlModePage->TAS            = TASK_ABORTED_STATUS_FOR_ABORTED_CMDS;
    pControlModePage->AutoLodeMode   = MEDIUM_LOADED_FULL_ACCESS;
    pControlModePage->BusyTimeoutPeriod[BYTE_0]   = UNLIMITED_BUSY_TIMEOUT_HIGH;
    pControlModePage->BusyTimeoutPeriod[BYTE_1]   = UNLIMITED_BUSY_TIMEOUT_LOW;
    pControlModePage->ExtSelfTestCompTime[BYTE_0] =
        SMART_SELF_TEST_UNSUPPORTED_HIGH;
    pControlModePage->ExtSelfTestCompTime[BYTE_1] =
        SMART_SELF_TEST_UNSUPPORTED_LOW;

    /* Increment pointer to after the control mode page */
    pControlModePage++;

    /* Power Condition Control Mode Page */
    pPowerConditionModePage = (PPOWER_CONDITION_MODE_PAGE)pControlModePage;
    modeDataLength += sizeof(POWER_CONDITION_MODE_PAGE);

    memset(pPowerConditionModePage, 0, sizeof(POWER_CONDITION_MODE_PAGE));
    pPowerConditionModePage->PageCode        = MODE_PAGE_POWER_CONDITION;
    pPowerConditionModePage->PageSaveable  = MODE_PAGE_PARAM_SAVEABLE_DISABLED;
    pPowerConditionModePage->SubPageFormat = SUB_PAGE_FORMAT_UNSUPPORTED;
    pPowerConditionModePage->PageLength    = POWER_COND_MODE_PAGE_LENGTH;

    /* Increment pointer to after the power condition mode page */
    pPowerConditionModePage++;

    /* Informational Exceptions Control Mode Page */
    pInfoExceptionsModePage =
        (PINFO_EXCEPTIONS_MODE_PAGE)pPowerConditionModePage;
    modeDataLength += sizeof(INFO_EXCEPTIONS_MODE_PAGE);

    memset(pInfoExceptionsModePage, 0, sizeof(INFO_EXCEPTIONS_MODE_PAGE));
    pInfoExceptionsModePage->PageCode   = MODE_PAGE_FAULT_REPORTING;
    pInfoExceptionsModePage->PageLength = INFO_EXCP_MODE_PAGE_LENGTH;

    /* Now go back and set the Mode Data Length in the header */
    if (modeSense10 == FALSE) {
        /* Get the correct header that starts at the buffer beginning */
        pModeHeader6 = (PMODE_PARAMETER_HEADER)(pSrbExt->modeSenseBuf);

        /* Subtract 1 from the mode data length - MODE DATA LENGTH field */
        pModeHeader6->ModeDataLength = (UCHAR)(modeDataLength - 1);
    } else {
        /* Get the correct header that starts at the buffer beginning */
        pModeHeader10 = (PMODE_PARAMETER_HEADER10)(pSrbExt->modeSenseBuf);

        /* Subtract 2 from mode data length - MODE DATA LENGTH field */
        pModeHeader10->ModeDataLength[BYTE_0] = ((modeDataLength - 2) &
            WORD_HIGH_BYTE_MASK) >> BYTE_SHIFT_1;
        pModeHeader10->ModeDataLength[BYTE_1] = ((modeDataLength - 2) &
            WORD_LOW_BYTE_MASK);
    }

    pSrb->SrbStatus = SRB_STATUS_PENDING;

    /* Set the completion callback routine to finish the command translation */
    pSrbExt->pNvmeCompletionRoutine = SntiCompletionCallbackRoutine;

    pSrbExt->dataBufferSize =
	sizeof(ADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY);

    pSrbExt->pDataBuffer = NVMeAllocateMem(pAE,
	sizeof(ADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY), 0);

    if (pSrbExt->pDataBuffer != NULL) {
	SntiBuildGetLogPageCmd(pSrbExt, SMART_HEALTH_INFORMATION);

	pSrbExt->ModeSenseWaitState = MODE_SENSE_WAIT_FOR_GET_LOG_PAGE_RESPONSE;

	SET_DATA_LENGTH(pSrb, min(modeDataLength, allocLength));
	
	pSrb->SrbStatus = SRB_STATUS_PENDING;
	returnStatus = SNTI_TRANSLATION_SUCCESS;
    }
    else {
	SntiSetScsiSenseData(pSrb,
			     SCSISTAT_CHECK_CONDITION,
			     SCSI_SENSE_UNIQUE,
			     SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
			     SCSI_ADSENSE_NO_SENSE);

	pSrb->SrbStatus |= SRB_STATUS_ERROR;
	SET_DATA_LENGTH(pSrb, 0);
	returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    }

    return returnStatus;

} /* SntiReturnAllModePages */

/******************************************************************************
 * SntiTranslateModeSelect
 *
 * @brief Translates the SCSI Mode Select command.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param supportsVwc - the ID data that indicates if its legal to sent a VWC
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateModeSelect(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    BOOLEAN supportsVwc
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    UINT16 paramListLength;
    UINT8 pageFormat;
    UINT8 savePages;
    BOOLEAN isModeSelect10;

    SNTI_STATUS status = SNTI_SUCCESS;
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;

    /* Extract the Log Sense fields to determine the page */
#if (NTDDI_VERSION > NTDDI_WIN7)
    UCHAR scsiStatus = SCSISTAT_GOOD;
#endif
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pageFormat = GET_U8_FROM_CDB(pSrb, MODE_SELECT_CDB_PAGE_FORMAT_OFFSET);
    savePages = GET_U8_FROM_CDB(pSrb, MODE_SELECT_CDB_SAVE_PAGES_OFFSET);

    /* Mask and get page format, save pages, and parameter list length */
    pageFormat &= MODE_SELECT_CDB_PAGE_FORMAT_MASK;
    savePages &= MODE_SELECT_CDB_SAVE_PAGES_MASK;

    /* Set the completion routine - no translation necessary on completion */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    if (GET_OPCODE(pSrb) == SCSIOP_MODE_SELECT) {
        /* MODE SELECT 6 */
        paramListLength = GET_U8_FROM_CDB(pSrb,
            MODE_SELECT_6_CDB_PARAM_LIST_LENGTH_OFFSET);

        isModeSelect10 = FALSE;
    } else {
         /* MODE SENSE 10 */
        paramListLength = GET_U16_FROM_CDB(pSrb,
            MODE_SELECT_10_CDB_PARAM_LIST_LENGTH_OFFSET);

        isModeSelect10 = TRUE;
    }

    if (paramListLength == 0) {
        /*
         * According to SPC-4 r24, a paramter list length field of 0
         * shall not be considered an error
         */
        SET_DATA_LENGTH(pSrb, 0);
#if (NTDDI_VERSION > NTDDI_WIN7)
        SrbSetScsiData(pSrb, NULL, NULL, NULL, NULL, 0);
        scsiStatus = SCSISTAT_GOOD;
        SrbSetScsiData(pSrb, NULL, NULL, &scsiStatus, NULL, NULL);
#else
        pSrb->SenseInfoBufferLength = 0;
        pSrb->ScsiStatus = SCSISTAT_GOOD;
#endif
        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        returnStatus = SNTI_COMMAND_COMPLETED;
    } else {
        status = GetLunExtension(pSrbExt, &pLunExt);
        if (status != SNTI_SUCCESS) {
            SntiMapInternalErrorStatus(pSrb, status);
            returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
        } else {
            returnStatus = SntiTranslateModeData(pSrbExt,
                                                 pLunExt,
                                                 paramListLength,
                                                 isModeSelect10,
                                                 supportsVwc);
        }
    }

    return returnStatus;
} /* SntiTranslateModeSelect */


/******************************************************************************
 * SntiTranslateModeData
 *
 * @brief Parse out the Mode Parameter Header, the Mode Parameter Descriptor
 *        Block, and the Mode Page for the Mode Select 6/10 command.
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 * @param paramListLength - Length of parameter list
 * @param isModeSelect10 - Boolean to determine Mode Select 10
 * @param supportsVwc - From ID data, tells us if we can send VWC cmds or not
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateModeData(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 paramListLength,
    BOOLEAN isModeSelect10,
    BOOLEAN supportsVwc
)
{
    PMODE_PARAMETER_HEADER pModeHeader6 = NULL;
    PMODE_PARAMETER_HEADER10 pModeHeader10 = NULL;
    PMODE_PARAMETER_BLOCK pModeParamBlock = NULL;
    PCACHING_MODE_PAGE pCacheModePage = NULL;
    PPOWER_CONDITION_MODE_PAGE pPowerModePage = NULL;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    PUCHAR pModePagePtr = NULL;
    UINT32 numBlockDesc = 0;
    UINT32 dword11 = 0;
    UINT16 blockDescLength = 0;
    UINT8 longLba = 0;
    UINT8 pageCode = 0;
    UINT64 modePageOffset = 0;
    BOOLEAN paramListValid = TRUE;
    UINT32 srbDataLength = GET_DATA_LENGTH(pSrb);

    SNTI_TRANSLATION_STATUS returnStatus = SNTI_COMMAND_COMPLETED;

    if (isModeSelect10 == FALSE) {
        /* Mode Select 6 */
        pModeHeader6 = (PMODE_PARAMETER_HEADER)(GET_DATA_BUFFER(pSrb));
        blockDescLength = pModeHeader6->BlockDescriptorLength;

        /* Set pointer to Mode Param Desc Block - if any */
        pModeParamBlock = (PMODE_PARAMETER_BLOCK)(pModeHeader6 + 1);
    } else {
        /* Mode Select 10 */
        pModeHeader10 = (PMODE_PARAMETER_HEADER10)(GET_DATA_BUFFER(pSrb));

        blockDescLength = (UINT16) ((pModeHeader10->BlockDescriptorLength[BYTE_0] << 8) | 
                                    (pModeHeader10->BlockDescriptorLength[BYTE_1])); 

        /* Get LONGLBA */
        longLba = pModeHeader10->Reserved[BYTE_0] & LONG_LBA_MASK;

        /* Set pointer to Mode Param Desc Block - if any */
        pModeParamBlock = (PMODE_PARAMETER_BLOCK)(pModeHeader10 + 1);
    }

    /*
     * Block Descriptor Length is equal to # of block descriptors times 8 if
     * if LONGLBA bit is set to zero or times 16 if LONGLBA is set to one.
     */
    if (blockDescLength != 0) {
        /* There are block descriptors to parse */
        numBlockDesc = (blockDescLength / ((longLba == 0) ?
                                           SHORT_DESC_BLOCK : LONG_DESC_BLOCK));

        /* Store off the block descriptor info if a FORMAT UNIT comes later */
        g_modeParamBlock.NumberOfBlocks[BYTE_0] =
            pModeParamBlock->NumberOfBlocks[BYTE_0];
        g_modeParamBlock.NumberOfBlocks[BYTE_1] =
            pModeParamBlock->NumberOfBlocks[BYTE_1];
        g_modeParamBlock.NumberOfBlocks[BYTE_2] =
            pModeParamBlock->NumberOfBlocks[BYTE_2];

        g_modeParamBlock.BlockLength[BYTE_0] =
            pModeParamBlock->BlockLength[BYTE_0];
        g_modeParamBlock.BlockLength[BYTE_1] =
            pModeParamBlock->BlockLength[BYTE_1];
        g_modeParamBlock.BlockLength[BYTE_2] =
            pModeParamBlock->BlockLength[BYTE_2];

        pModePagePtr = (PUCHAR)(pModeParamBlock + numBlockDesc);
    } else {
        /* No block descriptors to parse... go straight to Mode Page */
        pModePagePtr = (PUCHAR)pModeParamBlock;
    }

    modePageOffset = pModePagePtr - (PUCHAR)GET_DATA_BUFFER(pSrb);
    if (modePageOffset < srbDataLength) {
        pageCode = (*pModePagePtr) & MODE_SELECT_PAGE_CODE_MASK;
    } else {
        #define MODE_PAGE_INVALID 0xFF
        pageCode = MODE_PAGE_INVALID;
    }

    switch (pageCode) {
        case MODE_PAGE_CACHING:
            if (modePageOffset + sizeof(CACHING_MODE_PAGE) > srbDataLength) {
                paramListValid = FALSE;
                break;
            }
            /* Per NVMe we can't send VWC commands if its not supported */
            if (supportsVwc == FALSE) {
                pSrb->SrbStatus = SRB_STATUS_SUCCESS;
                returnStatus = SNTI_COMMAND_COMPLETED;
            } else {
            /* Command requires NVMe Get Features to adapter - build DWORD 11 */
            pCacheModePage = (PCACHING_MODE_PAGE)pModePagePtr;
            dword11 = (pCacheModePage->WriteCacheEnable ? 1 : 0);

            SntiBuildSetFeaturesCmd(pSrbExt, VOLATILE_WRITE_CACHE, dword11);

            returnStatus = SNTI_TRANSLATION_SUCCESS;
            pSrb->SrbStatus = SRB_STATUS_PENDING;
            }
        break;
        case MODE_PAGE_CONTROL:
            /* Command is completed in Build I/O phase */
            pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            returnStatus = SNTI_COMMAND_COMPLETED;
        break;
        case MODE_PAGE_POWER_CONDITION:
            if (modePageOffset + sizeof(POWER_CONDITION_MODE_PAGE) > srbDataLength) {
                paramListValid = FALSE;
                break;
            }
            /* Verify the OS is not trying to set timers */
            pPowerModePage = (PPOWER_CONDITION_MODE_PAGE)pModePagePtr;

            /* Command is completed in Build I/O phase */
            pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            returnStatus = SNTI_COMMAND_COMPLETED;
        break;
        default:
            paramListValid = FALSE;
        break;
    } /* end switch */
    if (!paramListValid) {
        SntiSetScsiSenseData(pSrb,
                                SCSISTAT_CHECK_CONDITION,
                                SCSI_SENSE_ILLEGAL_REQUEST,
                                SCSI_ADSENSE_INVALID_CDB,
                                SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);

        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
    }

    return returnStatus;
} /* SntiTranslateModeData */

/******************************************************************************
 * SntiCreateModeDataHeader
 *
 * @brief Creates the Mode Data Header and sets the appropriate ModeDataLength
 *        and pointer for the Mode Parameter Descriptor Block
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param ppModeParamBlock - Double pointer to mode param block
 * @param pModeDataLength - Pointer to mode data length var
 * @param blockDescLength - Block descriptor length
 * @param modeSense10 - Boolean to determine Mode Sense 10
 *
 * @return VOID
 ******************************************************************************/
VOID SntiCreateModeDataHeader(
    PNVME_SRB_EXTENSION pSrbExt,
    PMODE_PARAMETER_BLOCK *ppModeParamBlock,
    PUINT16 pModeDataLength,
    UINT16 blockDescLength,
    BOOLEAN modeSense10
)
{
    PMODE_PARAMETER_HEADER pModeHeader6 = NULL;
    PMODE_PARAMETER_HEADER10 pModeHeader10 = NULL;

    if (modeSense10 == FALSE) {
        /* MODE SENSE 6 */
        pModeHeader6 = (PMODE_PARAMETER_HEADER)(pSrbExt->modeSenseBuf);

        /* Set necessary fields */
        memset(pModeHeader6, 0, sizeof(MODE_PARAMETER_HEADER));
        pModeHeader6->MediumType              = DIRECT_ACCESS_DEVICE;
        pModeHeader6->DeviceSpecificParameter = 0;
        pModeHeader6->BlockDescriptorLength   = (UCHAR)blockDescLength;

        /* Increment pointer and set pointer to Param Desc Block */
        pModeHeader6++;
        *ppModeParamBlock = (PMODE_PARAMETER_BLOCK)pModeHeader6;

        /*
         * Calculate the mode data length - subtract the number of bytes for the
         * mode data length field itself.
         */
        *pModeDataLength += sizeof(MODE_PARAMETER_HEADER);
    } else {
        /* MODE SENSE 10 */
        pModeHeader10 = (PMODE_PARAMETER_HEADER10)(pSrbExt->modeSenseBuf);

        /* Set necessary fields */
        memset(pModeHeader10, 0, sizeof(MODE_PARAMETER_HEADER10));
        pModeHeader10->MediumType               = DIRECT_ACCESS_DEVICE;
        pModeHeader10->DeviceSpecificParameter  = 0;

        pModeHeader10->BlockDescriptorLength[BYTE_0] =
            (blockDescLength & WORD_HIGH_BYTE_MASK) >> BYTE_SHIFT_1;
        pModeHeader10->BlockDescriptorLength[BYTE_1] =
            (blockDescLength & WORD_LOW_BYTE_MASK);

        /* Increment pointer and set pointer to Param Desc Block */
        pModeHeader10++;
        *ppModeParamBlock = (PMODE_PARAMETER_BLOCK)pModeHeader10;

        /*
         * Calculate the mode data length - subtract the number of bytes for the
         * mode data length field itself.
         */
        *pModeDataLength += sizeof(MODE_PARAMETER_HEADER10);
    }
} /* SntiCreateModeDataHeader */

/******************************************************************************
 * SntiCreateModeParameterDescBlock
 *
 * @brief Creates the Mode Parameter Descriptor Block and sets the appropriate
 *        ModeDataLength and pointer for the Mode Parameter Descriptor Block
 *
 * @param pLunExt - Pointer to LUN extension
 * @param pModeParamBlock - Pointer to mode param block
 * @param pModeDataLength - Pointer to mode data length
 *
 * @return VOID
 ******************************************************************************/
VOID SntiCreateModeParameterDescBlock(
    PNVME_LUN_EXTENSION pLunExt,
    PMODE_PARAMETER_BLOCK pModeParamBlock,
    PUINT16 pModeDataLength
)
{
    UINT32 lbaLength;
    UINT32 lbaLengthPower;
    UINT8  flbas;

    *pModeDataLength += sizeof(MODE_PARAMETER_BLOCK);

    memset(pModeParamBlock, 0, sizeof(MODE_PARAMETER_BLOCK));
    if (pLunExt->identifyData.NCAP > MODE_BLOCK_DESC_MAX) {
        pModeParamBlock->NumberOfBlocks[BYTE_0] = MODE_BLOCK_DESC_MAX_BYTE;
        pModeParamBlock->NumberOfBlocks[BYTE_1] = MODE_BLOCK_DESC_MAX_BYTE;
        pModeParamBlock->NumberOfBlocks[BYTE_2] = MODE_BLOCK_DESC_MAX_BYTE;
    } else {
    pModeParamBlock->NumberOfBlocks[BYTE_0] =
        (UCHAR)((pLunExt->identifyData.NCAP &
                 DWORD_MASK_BYTE_2) >> BYTE_SHIFT_2);
    pModeParamBlock->NumberOfBlocks[BYTE_1] =
        (UCHAR)((pLunExt->identifyData.NCAP &
                 DWORD_MASK_BYTE_1) >> BYTE_SHIFT_1);
    pModeParamBlock->NumberOfBlocks[BYTE_2] =
        (UCHAR)((pLunExt->identifyData.NCAP & DWORD_MASK_BYTE_0));
    }

    flbas = pLunExt->identifyData.FLBAS.SupportedCombination;
    lbaLengthPower = pLunExt->identifyData.LBAFx[flbas].LBADS;
    lbaLength = 1 << lbaLengthPower;

    pModeParamBlock->BlockLength[BYTE_0] =
        (UCHAR)((lbaLength & DWORD_MASK_BYTE_2) >> BYTE_SHIFT_2);
    pModeParamBlock->BlockLength[BYTE_1] =
        (UCHAR)((lbaLength & DWORD_MASK_BYTE_1) >> BYTE_SHIFT_1);
    pModeParamBlock->BlockLength[BYTE_2] =
        (UCHAR)((lbaLength & DWORD_MASK_BYTE_0));
} /* SntiCreateModeParameterDescBlock */

/******************************************************************************
 * SntiTranslateSglToPrp
 *
 * @brief Translates the Scatter Gather List (SGL) to PRP Entries/List.
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pSgl - Pointer to Scatter Gather List
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateSglToPrp(
    PNVME_SRB_EXTENSION pSrbExt,
    PSTOR_SCATTER_GATHER_LIST pSgl
)
{
    /* PRP list is needed */
    PHYSICAL_ADDRESS physicalAddress;
    PUINT64 pPrpList = NULL;
    UINT32 numImplicitEntries;
    UINT32 subIndex;
    UINT32 sgElementSize;
    UINT32 index;
    ULONGLONG localPrpEntry = 0;
    ULONG lengthIncrement = PAGE_SIZE;
    ULONG offset;
    PULONGLONG pPrp1 = &pSrbExt->nvmeSqeUnit.PRP1;
    PULONGLONG pPrp2 = &pSrbExt->nvmeSqeUnit.PRP2;
    ULONG modulo;

#if DUMB_DRIVER
        return;
#endif

    pSrbExt->numberOfPrpEntries = 0;
    if (pSgl == NULL) return;
    ASSERT(pSgl->NumberOfElements != 0);

    /* There may not always be a 1:1 ratio of SG elements to PRP entries... */
    pPrpList = &pSrbExt->prpList[0];

    for (index = 0; index < pSgl->NumberOfElements; index++) {

        /* NOTE: This size may be more than a PAGE size */
        sgElementSize = pSgl->List[index].Length;

        /* Sub page size entries can only be the first or last SG elements */
        if (((sgElementSize % PAGE_SIZE) != 0)  &&
              (index != (pSgl->NumberOfElements - 1)) &&
             (index != 0 )) {
            /* Future code to break out child I/O's, if necessary */
            ASSERT(FALSE);
        }

        /*
         * Get the number of implicit PRP entries in this SG element and
         * the starting physical address... this can either be one entry
         * or multiple entries (if not the first or last entry). Note that
         * multiple entries per SG element are required to be multiples of
         * the page size.
         */
        physicalAddress.QuadPart = pSgl->List[index].PhysicalAddress.QuadPart;

        /* calc number of whole pages, if any */
        numImplicitEntries = sgElementSize / PAGE_SIZE;
        /* calc the leftover size minus whole pages */
        modulo = sgElementSize % PAGE_SIZE;
        /* determine if our start is aligned or not */
        offset = physicalAddress.LowPart % PAGE_SIZE;

        /*
         * if we have a length modulo or we're not aligned then
         * we treat this like an unaligned < page size case where
         * we we're definately taking up one page and depending
         * on the size of the leftovers (mod + alignment offset)
         * we may need to account for one more page
         */
        if (modulo || offset) {
            numImplicitEntries += 1 + (modulo + offset - 1) / PAGE_SIZE;
        }

        /* For each SG element */
        for (subIndex = 0; subIndex < numImplicitEntries; subIndex++) {
            /* Get first sub element (implied element) */
            localPrpEntry = (UINT64)(physicalAddress.QuadPart);

            /* Keep track of the number of PRP Entries */
            pSrbExt->numberOfPrpEntries++;

            if (pSrbExt->numberOfPrpEntries == PRP_ENTRY_1) {
                *pPrp1 = localPrpEntry;

                /* Check for offset if first entry is not page aligned */
                offset = physicalAddress.LowPart & PAGE_MASK;
                lengthIncrement = PAGE_SIZE - offset;
            } else if (pSrbExt->numberOfPrpEntries == PRP_ENTRY_2) {
                *pPrp2 = localPrpEntry;
            } else if (pSrbExt->numberOfPrpEntries == PRP_ENTRY_3) {
                /*
                 * Copy the second entry and increment the pointer then zero
                 * out the 2nd entry, it will be updated in the StartIo path
                 * wiht pre-allcoated list memory.
                 */

                *pPrpList = *pPrp2;
                 pPrpList++;
                *pPrp2 = 0;

                /* Place next PRP entry in list and increment the ptr */
                *pPrpList = (UINT64)(physicalAddress.QuadPart);
                pPrpList++;
            } else {
                /* Place next PRP entry in list and increment the ptr */
                *pPrpList = (UINT64)(physicalAddress.QuadPart);
                pPrpList++;
            }

            /*
             * Increment to the next implied SG element (physical address).
             * Note that these entries must be contiguous in physical memory
             * so increment to the next page entry.
             */
            if (pSrbExt->numberOfPrpEntries > 1)
                lengthIncrement = PAGE_SIZE;

            physicalAddress.QuadPart += lengthIncrement;
        } /* end for loop */
    } /* end for loop */
} /* SntiTranslateSglToPrp */

/******************************************************************************
 * SntiValidateLbaAndLength
 *
 * @brief Validates the LBA and TransferLength of a SCSI WRITE 6, 10, 12, 16
 *        command. If the LBA + Transler Length exceeds the capacity of the
 *        namespace, a check condition with ILLEGAL REQUEST - LBA Out Of Range
 *        (5/2100) shall be returned.
 *
 *        NOTE: Transfer Length is in logical blocks (per the SBC-3)
 *        NOTE: Namespace Size is in logical blocks (per NVM Express 1.0b)
 *
 * @param: pLunExt - Pointer to LUN extension
 * @param: pSrbExt - Pointer to SRB extension
 * @param: lba - LBA to verify
 * @param: length - Length to verify
 *
 * @return SNTI_STATUS
 *     Indicates internal tranlsation status
 ******************************************************************************/
SNTI_STATUS SntiValidateLbaAndLength(
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_SRB_EXTENSION pSrbExt,
    UINT64 lba,
    UINT32 length
)
{
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    SNTI_STATUS status = SNTI_SUCCESS;
    UINT32 lbaSize = 0;
    UINT32 lbaLengthPower = 0;
    UINT8 flbas;

    if ((lba + length) > (pLunExt->identifyData.NSZE + 1)) {
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_ILLEGAL_BLOCK,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);
        status = SNTI_INVALID_PARAMETER;
    }

    if (length > NVME_MAX_NUM_BLOCKS_PER_READ_WRITE) {
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_ILLEGAL_BLOCK,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);
        status = SNTI_INVALID_PARAMETER;
    }

    /* Need to check if there is buffer over-run or under-run case */
    flbas = pLunExt->identifyData.FLBAS.SupportedCombination;
    lbaLengthPower = pLunExt->identifyData.LBAFx[flbas].LBADS;
    lbaSize = 1 << lbaLengthPower;
    if ((length * lbaSize) > GET_DATA_LENGTH(pSrb)) {
        SntiSetScsiSenseData(pSrb,
                             SCSISTAT_CHECK_CONDITION,
                             SCSI_SENSE_ILLEGAL_REQUEST,
                             SCSI_ADSENSE_INVALID_CDB,
                             SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_DATA_OVERRUN;
        SET_DATA_LENGTH(pSrb, 0);
        status = SNTI_INVALID_PARAMETER;
    }

    if ((length * lbaSize) < GET_DATA_LENGTH(pSrb)) {
        SET_DATA_LENGTH(pSrb, length * lbaSize);
    }

    return status;
} /* SntiValidateLbaAndLength */

/******************************************************************************
 * SntiSetScsiSenseData
 *
 * @brief Sets up the SCSI sense data in the provided SRB.
 *
 *        NOTE: The caller of this func must set the correlating SRB status
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param scsiStatus - SCSI Status to be stored in the Sense Data buffer
 * @param senseKey - Sense Key for the Sense Data
 * @param asc - Additional Sense Code (ASC)
 * @param ascq - Additional Sense Code Qualifier (ASCQ)
 *
 * @return BOOLEAN
 ******************************************************************************/
BOOLEAN SntiSetScsiSenseData(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    UCHAR scsiStatus,
    UCHAR senseKey,
    UCHAR asc,
    UCHAR ascq
)
{
    PSENSE_DATA pSenseData = NULL;
    BOOLEAN status = TRUE;
    UCHAR senseInfoBufferLength = 0;
    PVOID senseInfoBuffer = NULL;
    UCHAR senseDataLength = sizeof(SENSE_DATA);
#if (NTDDI_VERSION > NTDDI_WIN7)
    SrbSetScsiData(pSrb, NULL, NULL, &scsiStatus, NULL, NULL);
    SrbGetScsiData(pSrb, NULL, NULL, NULL, &senseInfoBuffer, &senseInfoBufferLength);
#else
    pSrb->ScsiStatus = scsiStatus;
    senseInfoBufferLength = pSrb->SenseInfoBufferLength;
    senseInfoBuffer = pSrb->SenseInfoBuffer;
#endif
    if ((scsiStatus != SCSISTAT_GOOD) &&
        (senseInfoBufferLength >= senseDataLength)) {

        pSenseData = (PSENSE_DATA)senseInfoBuffer;

        memset(pSenseData, 0, senseInfoBufferLength);
        pSenseData->ErrorCode                    = FIXED_SENSE_DATA;
        pSenseData->SenseKey                     = senseKey;
        pSenseData->AdditionalSenseCode          = asc;
        pSenseData->AdditionalSenseCodeQualifier = ascq;

        pSenseData->AdditionalSenseLength = senseDataLength - 
            FIELD_OFFSET(SENSE_DATA, CommandSpecificInformation);
#if (NTDDI_VERSION > NTDDI_WIN7)
        SrbSetScsiData(pSrb, NULL, NULL, NULL, NULL, &senseDataLength);
#else
        pSrb->SenseInfoBufferLength = sizeof(SENSE_DATA);
#endif
        pSrb->SrbStatus |= SRB_STATUS_AUTOSENSE_VALID;
    } else {
#if (NTDDI_VERSION > NTDDI_WIN7)
        SrbSetScsiData(pSrb, NULL, NULL, NULL, NULL, 0);
#else
        pSrb->SenseInfoBufferLength = 0;
#endif
        status = FALSE;
    }

   return status;
}

/******************************************************************************
 * GetLunExtension
 *
 * @brief Returns the LUN Extension structure for the B/T/L nexus associated
 *        with the SRB Extension and SRB I/O request.
 *
 * @param pSrbExt - Pointer to the SRB Extension
 * @param ppLunExt - Double pointer to the LUN Extension
 *
 * @return SNTI_STATUS
 *     Indicates if the LUN Extension was found successfully.
 ******************************************************************************/
SNTI_STATUS GetLunExtension(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION *ppLunExt
)
{
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;
    UCHAR PathId = 0;
    UCHAR TargetId = 0;
    UCHAR Lun = 0;
    SNTI_STATUS returnStatus = SNTI_SUCCESS;

#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
    ASSERT(pDevExt != NULL);
    ASSERT(pSrb != NULL);
    PathId = SrbGetPathId((void*)pSrb);
    TargetId = SrbGetTargetId((void*)pSrb);
    Lun = SrbGetLun((void*)pSrb);
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
    ASSERT(pDevExt != NULL);
    ASSERT(pSrb != NULL);
    PathId = pSrb->PathId;
    TargetId = pSrb->TargetId;
    Lun = pSrb->Lun;
#endif

    if ((PathId != VALID_NVME_PATH_ID) ||
        (TargetId != VALID_NVME_TARGET_ID) ||
        (pDevExt->pLunExtensionTable[Lun]->slotStatus != ONLINE)) {
        *ppLunExt = NULL;
        returnStatus = SNTI_INVALID_PATH_TARGET_ID;
    } else {
        *ppLunExt = pDevExt->pLunExtensionTable[Lun];
    }

    return returnStatus;
} /* GetLunExtension */

/******************************************************************************
 * SntiIssueGetFeaturesCmd
 *
 * @brief Builds an internal NVMe GET FEATURES command. Note that the SRB data
 *        buffer will be used for the PRP entry unless indicated by the boolean
 *        value passed in that states a separate buffer is needed.
 *
 * @param pSrbExt - This parameter specifies the SRB Extension and the
 *                  associated SRB with P/T/L nexus.
 * @param featureIdentifier - Identifier for Get Features
 *
 * @return VOID
 ******************************************************************************/
VOID SntiBuildGetFeaturesCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT8 featureIdentifier
)
{
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;
    PSTOR_SCATTER_GATHER_LIST pSgl = NULL;

    /* Make sure we indicate which I/Os are the child and parent */
    pSrbExt->pParentIo = NULL;
    pSrbExt->pChildIo = NULL;

    /* And which queue to use */
    pSrbExt->forAdminQueue = TRUE;

    /* Set up the GET FEATURES command */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
    pSrbExt->nvmeSqeUnit.CDW0.OPC = ADMIN_GET_FEATURES;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

    /* DWORD 10 */
    pSrbExt->nvmeSqeUnit.CDW10 |= featureIdentifier;

    /*
     *  NOTE: The only feature identifer that uses a buffer to return data is
     *        LBA Range Type (0x03). All other feature identifiers will use a
     *        DWORD in the completion queue entry to return the attributes of
     *        a feature requested.
     */
    if (featureIdentifier != LBA_RANGE_TYPE) {
        /* PRP's not used for these requests */
        pSrbExt->nvmeSqeUnit.PRP1 = 0;
        pSrbExt->nvmeSqeUnit.PRP2 = 0;
    } else {
        /* PRP Entry/List - Use the SGL from the original command */
        pSgl = StorPortGetScatterGatherList(pDevExt, 
                                           (PSCSI_REQUEST_BLOCK)pSrbExt->pSrb); 
        ASSERT(pSgl != NULL);

        SntiTranslateSglToPrp(pSrbExt, pSgl);
    }

    /* SRB Status must be set to PENDING */
    pSrbExt->pSrb->SrbStatus = SRB_STATUS_PENDING;
} /* SntiBuildGetFeaturesCmd */

/******************************************************************************
 * SntiBuildSetFeaturesCmd
 *
 * @brief Builds an internal NVMe SET FEATURES command.
 *
 * @param pSrbExt - This parameter specifies the SRB Extension and the
 *                  associated SRB with P/T/L nexus.
 * @param featureIdentifier - Identifer for the Set Features command
 * @param dword11 - DWORD 11 of the Set Features command
 *
 * @return VOID
 ******************************************************************************/
VOID SntiBuildSetFeaturesCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT8 featureIdentifier,
    UINT32 dword11
)
{
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;
    PSTOR_SCATTER_GATHER_LIST pSgl = NULL;

    /* Make sure we indicate which I/Os are the child and parent */
    pSrbExt->pParentIo = NULL;
    pSrbExt->pChildIo = NULL;

    /* And which queue to use */
    pSrbExt->forAdminQueue = TRUE;

    /* Set up the GET FEATURES command */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
    pSrbExt->nvmeSqeUnit.CDW0.OPC = ADMIN_SET_FEATURES;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

    /* DWORD 10 and 11 */
    pSrbExt->nvmeSqeUnit.CDW10 |= featureIdentifier;
    pSrbExt->nvmeSqeUnit.CDW11 = dword11;

    /*
     *  NOTE: The only feature identifer that uses a buffer to return data is
     *        LBA Range Type (0x03). All other feature identifiers will use a
     *        DWORD in the completion queue entry to return the attributes of
     *        a feature requested.
     */
    if (featureIdentifier != LBA_RANGE_TYPE) {
        /* PRP's not used for these requests */
        pSrbExt->nvmeSqeUnit.PRP1 = 0;
        pSrbExt->nvmeSqeUnit.PRP2 = 0;
    } else {
        /* PRP Entry/List - Use the SGL from the original command */
        pSgl = StorPortGetScatterGatherList(pDevExt, 
                                           (PSCSI_REQUEST_BLOCK)pSrbExt->pSrb);
        ASSERT(pSgl != NULL);

        SntiTranslateSglToPrp(pSrbExt, pSgl);
    }

    /* SRB Status must be set to PENDING */
    pSrbExt->pSrb->SrbStatus = SRB_STATUS_PENDING;
} /* SntiBuildSetFeaturesCmd */

/******************************************************************************
 * SntiBuildGetLogPageCmd
 *
 * @brief Builds an internal NVMe GET LOG PAGE command. Note that the SRB data
 *        buffer will be used for the PRP entries.
 *
 * @param pSrbExt - This parameter specifies the SRB Extension and the
 *                  associated SRB with P/T/L nexus.
 * @param logIdentifier - Log page identifier
 *
 * @return VOID
 ******************************************************************************/
VOID SntiBuildGetLogPageCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT8 logIdentifier
)
{
    PNVME_LUN_EXTENSION pLunExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;
    PSTOR_SCATTER_GATHER_LIST pSgl = NULL;
    UINT32 numDwords = 0;

    /* Retrieve LUN Extension here */
    GetLunExtension(pSrbExt, &pLunExt);

    /* Make sure we indicate which I/Os are the child and parent */
    pSrbExt->pParentIo = NULL;
    pSrbExt->pChildIo = NULL;

    /* And which queue to use */
    pSrbExt->forAdminQueue = TRUE;

    /* Set up the GET LOG PAGE command */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
    pSrbExt->nvmeSqeUnit.CDW0.OPC = ADMIN_GET_LOG_PAGE;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

    /* NOTE: This value must not exceed 4K */
    /* The SMART/Health page is namespace based while the others are global */
    switch(logIdentifier) {
        case ERROR_INFORMATION:
            pSrbExt->nvmeSqeUnit.NSID = 0xFFFFFFFF;
            numDwords =
                ((sizeof(ADMIN_GET_LOG_PAGE_ERROR_INFORMATION_LOG_ENTRY) / NUM_BYTES_IN_DWORD) - 1);
        break;
        case SMART_HEALTH_INFORMATION:
            /* 
             * The SMART/Health page is namespace based when bit0 of Log Page
             * Attributes of Identify Controller structure is 1.
             * When cleared to 0, this log page is global to the device.
             */
            if (pDevExt->controllerIdentifyData.LPA.SupportsSMART_HealthInformationLogPage & 1)
                pSrbExt->nvmeSqeUnit.NSID = pLunExt->namespaceId;
            else
                pSrbExt->nvmeSqeUnit.NSID = 0xFFFFFFFF;
            numDwords =
                ((sizeof(ADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY) / NUM_BYTES_IN_DWORD) - 1);
        break;
        case FIRMWARE_SLOT_INFORMATION:
            pSrbExt->nvmeSqeUnit.NSID = 0xFFFFFFFF;
            numDwords =
                ((sizeof(ADMIN_GET_LOG_PAGE_FIRMWARE_SLOT_INFORMATION_LOG_ENTRY) / NUM_BYTES_IN_DWORD) - 1);
        break;
        default:
            ASSERT(FALSE);
        break;
    }

    /*
     * DWORD 10
     * Number of DWORDs is 0's based
     */
    numDwords = numDwords / sizeof(UINT32);
    numDwords -= 1;
    pSrbExt->nvmeSqeUnit.CDW10 |= (numDwords << BYTE_SHIFT_2);
    pSrbExt->nvmeSqeUnit.CDW10 |= (logIdentifier & DWORD_MASK_LOW_WORD);

    if (pSrbExt->pDataBuffer != NULL) {
        /* Use the allocated data buffer for PRP Entries/List */
        STOR_PHYSICAL_ADDRESS physAddr;
        ULONG paLength;

        physAddr = StorPortGetPhysicalAddress(pDevExt,
                                              NULL,
                                              pSrbExt->pDataBuffer,
                                              &paLength);

        if (physAddr.QuadPart != 0) {
            pSrbExt->nvmeSqeUnit.PRP1 = physAddr.QuadPart;
            pSrbExt->nvmeSqeUnit.PRP2 = 0;
        } else {
            StorPortDebugPrint(
                INFO,
                "SNTI: Get PhysAddr for GET LOG PAGE failed (pSrbExt = 0x%p)\n",
                pSrbExt);

            ASSERT(FALSE);
        }
    } else {
        /* Use the SGL from the original command for PRP Entries/List */
        pSgl = StorPortGetScatterGatherList(pDevExt, 
                                           (PSCSI_REQUEST_BLOCK)pSrbExt->pSrb);

        if (pSgl != NULL) {
            #ifdef DEBUG_CHECK
            if ((pSgl->NumberOfElements * sizeof(STOR_SCATTER_GATHER_ELEMENT))
                >= (numDwords/sizeof(UINT32))) {
                /* In this case, fail the command or create a temp buffer */
                ASSERT(FALSE);
                return;
            }
            #endif /* DEBUG_CHECK */

            /* Translate the SRB SGL to a PRP List */
            SntiTranslateSglToPrp(pSrbExt, pSgl);
        } else {
            StorPortDebugPrint(
                INFO,
                "SNTI: Get SGL for GET LOG PAGE failed (pSrbExt = 0x%p)\n",
                pSrbExt);

            ASSERT(FALSE);
        }
    }

    /* SRB Status must be set to PENDING */
    pSrbExt->pSrb->SrbStatus = SRB_STATUS_PENDING;

} /* SntiBuildGetLogPageCmd */

/******************************************************************************
 * SntiBuildFirmwareImageDownloadCmd
 *
 * Builds an internal NVMe FIRMWARE IMAGE DOWNLOAD command.
 *
 * @param pSrbExt - This parameter specifies the SRB Extension and the
 *                  associated SRB with P/T/L nexus.
 * @param dword10 - DWORD 10 of the FIRMWARE IMAGE DOWNLOAD command
 * @param dword11 - DWORD 11 of the FIRMWARE IMAGE DOWNLOAD command
 *
 * return: VOID
 ******************************************************************************/
VOID SntiBuildFirmwareImageDownloadCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT32 dword10,
    UINT32 dword11
)
{
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;
    PSTOR_SCATTER_GATHER_LIST pSgl = NULL;
    UINT32 numDwords = 0;


    /* Make sure we indicate which I/Os are the child and parent */
    pSrbExt->pParentIo = NULL;
    pSrbExt->pChildIo = NULL;

    /* And which queue to use */
    pSrbExt->forAdminQueue = TRUE;

    /* Set up the Firmware Image Download command */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
    pSrbExt->nvmeSqeUnit.CDW0.OPC = ADMIN_FIRMWARE_IMAGE_DOWNLOAD;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

    /* DWORD 10/11 */
    pSrbExt->nvmeSqeUnit.CDW10 = dword10;
    pSrbExt->nvmeSqeUnit.CDW11 = dword11;

    /* PRP Entry/List - Use the SGL from the original command */
    pSgl = StorPortGetScatterGatherList(pDevExt, 
                                       (PSCSI_REQUEST_BLOCK)pSrbExt->pSrb);
    ASSERT(pSgl != NULL);

    #define DEBUG_CHECK
    #ifdef DEBUG_CHECK
    if ((pSgl->NumberOfElements * sizeof(STOR_SCATTER_GATHER_ELEMENT)) >=
        (numDwords/sizeof(UINT32))) {
        /* In this case, must fail the command or create a temp buffer... */

        ASSERT(FALSE);
        return;
    }
    #endif /* DEBUG_CHECK */

    SntiTranslateSglToPrp(pSrbExt, pSgl);

    /* SRB Status must be set to PENDING */
    pSrbExt->pSrb->SrbStatus = SRB_STATUS_PENDING;
} /* SntiBuildFirmwareImageDownloadCmd */

/******************************************************************************
 * SntiBuildFirmwareActivateCmd
 *
 * @brief Builds an internal NVMe FIRMWARE ACTIVATE command.
 *
 * @param pSrbExt - This parameter specifies the SRB Extension and the
 *                  associated SRB with P/T/L nexus.
 * @param dword10 - DWORD 10 of the FIRMWARE ACTIVATE command
 *
 * return: VOID
 ******************************************************************************/
VOID SntiBuildFirmwareActivateCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT32 dword10
)
{
    /* Chose which queue to use */
    pSrbExt->forAdminQueue = TRUE;

    /* Set up Firmware Activate command */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));
    pSrbExt->nvmeSqeUnit.CDW0.OPC = ADMIN_FIRMWARE_ACTIVATE;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

    /* DWORD 10/11 */
    pSrbExt->nvmeSqeUnit.CDW10 = dword10;

    /* SRB Status must be set to PENDING */
    pSrbExt->pSrb->SrbStatus = SRB_STATUS_PENDING;
} /* SntiBuildFirmwareActivateCmd */

/******************************************************************************
 * SntiBuildFlushCmd
 *
 * @brief Builds an internal NVME FLUSH command. The NVME FLUSH command is
 *        defined:
 *
 *        6.7 Flush command
 *
 *        The Flush command is used by the host to indicate that any data in
 *        volatile storage should be flushed to non-volatile memory.
 *
 *        All command specific fields are reserved.
 *
 * @param pSrbExt - This parameter specifies the SRB Extension and the
 *                  associated SRB with P/T/L nexus.
 *
 * @return VOID
 ******************************************************************************/
VOID SntiBuildFlushCmd(
    PNVME_SRB_EXTENSION pSrbExt
)
{
    PNVME_LUN_EXTENSION pLunExt = NULL;
    GetLunExtension(pSrbExt, &pLunExt);

    /* Zero out the command entry */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));

    /* This cmd is called for a specific namespace */
    pSrbExt->nvmeSqeUnit.NSID = pLunExt->namespaceId;
    pSrbExt->nvmeSqeUnit.CDW0.OPC = NVM_FLUSH;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

    /* Set the SRB status to pending - controller communication necessary */
    pSrbExt->pSrb->SrbStatus = SRB_STATUS_PENDING;
} /* SntiBuildFlushCmd */

/******************************************************************************
 * SntiBuildFormatNvmCmd
 *
 * @brief Builds an internal NVME FORMAT NVM command.
 *
 * @param pSrbExt - This parameter specifies the SRB Extension and the
 *                  associated SRB with P/T/L nexus.
 * @param protectionType
 *
 * return: VOID
 ******************************************************************************/
VOID SntiBuildFormatNvmCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT8 protectionType
)
{
    /* Chose which queue to use */
    pSrbExt->forAdminQueue = TRUE;

    /* Set up the command entry for Format NVM command */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));

    pSrbExt->nvmeSqeUnit.CDW0.OPC = ADMIN_FORMAT_NVM;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

    /* DWORD 10 */
    pSrbExt->nvmeSqeUnit.CDW10 |=
        (protectionType << FORMAT_NVM_PROTECTION_INFO_SHIFT_MASK);

    /* Specify the block length and # of blocks from last MODE SELECT */

    /* Set the SRB status to pending - controller communication necessary */
    pSrbExt->pSrb->SrbStatus = SRB_STATUS_PENDING;
} /* SntiBuildFormatNvmCmd */

/******************************************************************************
 * SntiBuildSecuritySendReceiveCmd
 *
 * @brief Builds an internal NVME Security Send/Receive command.
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pLunExt - Pointer to LUN extension
 * @param opcode - Opcode (SEND or RECEIVE)
 * @param transferLength - Length of data transfer
 * @param secProtocolSp - Security protocol specific info
 * @param secProtocol - Security protocol info
 *
 * @return VOID
 ******************************************************************************/
VOID SntiBuildSecuritySendReceiveCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT8 opcode,
    UINT32 transferLength,
    UINT16 secProtocolSp,
    UINT8 secProtocol
)
{
    PSTOR_SCATTER_GATHER_LIST pSgl = NULL;

    PADMIN_SECURITY_SEND_COMMAND_DW10 pCdw10 =
        (PADMIN_SECURITY_SEND_COMMAND_DW10)&pSrbExt->nvmeSqeUnit.CDW10;
    PADMIN_SECURITY_SEND_COMMAND_DW11 pCdw11 =
        (PADMIN_SECURITY_SEND_COMMAND_DW11)&pSrbExt->nvmeSqeUnit.CDW11;

    /* Chose which queue to use */
    pSrbExt->forAdminQueue = TRUE;

    /* Zero out the command entry */
    memset(&pSrbExt->nvmeSqeUnit, 0, sizeof(NVMe_COMMAND));

    pSrbExt->nvmeSqeUnit.CDW0.OPC = opcode;
    pSrbExt->nvmeSqeUnit.CDW0.CID = 0;
    pSrbExt->nvmeSqeUnit.CDW0.FUSE = FUSE_NORMAL_OPERATION;

    pSrbExt->nvmeSqeUnit.NSID = pLunExt->namespaceId;

    /* PRP Entry/List */
    pSgl = StorPortGetScatterGatherList(pSrbExt->pNvmeDevExt, 
                                       (PSCSI_REQUEST_BLOCK)pSrbExt->pSrb);
    ASSERT(pSgl != NULL);

    SntiTranslateSglToPrp(pSrbExt, pSgl);

    /* DWORD 10 */
    pCdw10->SPSP = secProtocolSp;
    pCdw10->SECP = secProtocol;

    /* DWORD 11 */
    pCdw11->AL = transferLength;
} /* SntiBuildSecuritySendReceiveCmd */

/******************************************************************************
 * SntiCompletionCallbackRoutine
 *
 * @brief This method handles the completion callback from the I/O engine for
 *        commands that need to have a status translated, a NVMe response
 *        translated into data, or that need to send additional commands.
 *
 *        Commands that require additional work on completion side:
 *
 *        - Log Sense
 *        - Mode Sense
 *        - Start Stop Unit (with implicit NVMe Flush cmd)
 *        - Write Buffer
 *
 * @param param1 - Pointer to device extension
 * @param param2 - Pointer to SRB extension
 *
 * @return BOOLEAN
 *     TRUE - successful completion of callback
 *     FALSE - unsuccessful completion of callback or error
 ******************************************************************************/
BOOLEAN SntiCompletionCallbackRoutine(
    PVOID param1,
    PVOID param2
)
{
    PNVME_DEVICE_EXTENSION pDevExt = (PNVME_DEVICE_EXTENSION)param1;
    PNVME_SRB_EXTENSION pSrbExt = (PNVME_SRB_EXTENSION)param2;

#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry = pSrbExt->pCplEntry;
    UINT8 statusCodeType;
    UINT8 statusCode;
    BOOLEAN returnValue = TRUE;
    ULONG ulStatus;
    UINT8 opcode;

    /* Default to successful command sequence completion */
    SNTI_TRANSLATION_STATUS translationStatus = SNTI_SEQUENCE_COMPLETED;

    /*get the opcode*/
    opcode = GET_OPCODE(pSrb);

    /* Before calling commpletion code, insure NVMe command succeeded */
    statusCodeType = (UINT8)pSrbExt->pCplEntry->DW3.SF.SCT;
    statusCode = (UINT8)pSrbExt->pCplEntry->DW3.SF.SC;

    if ((statusCodeType == GENERIC_COMMAND_STATUS) &&
        (statusCode == SUCCESSFUL_COMPLETION)) {
        if (pSrb != NULL) {
            switch (opcode) {
                case SCSIOP_LOG_SENSE:
                    translationStatus = SntiTranslateLogSenseResponse(
                        pSrb, pCQEntry);

                    if (translationStatus == SNTI_SEQUENCE_IN_PROGRESS)
                        returnValue = FALSE;
                break;
                case SCSIOP_MODE_SENSE:
                case SCSIOP_MODE_SENSE10:
                    translationStatus = SntiTranslateModeSenseResponse(
                        pSrb, pCQEntry);
                    if (translationStatus == SNTI_SEQUENCE_IN_PROGRESS)
                        returnValue = FALSE;
                break;
                case SCSIOP_START_STOP_UNIT:
                    /* NOTE: This is an implicit NVM Flush command completing */
                    translationStatus =
                        SntiTranslateStartStopUnitResponse(pSrb);

                    if (translationStatus == SNTI_SEQUENCE_IN_PROGRESS)
                        returnValue = FALSE;
                break;
                case SCSIOP_WRITE_DATA_BUFF:
                    translationStatus = SntiTranslateWriteBufferResponse(pSrb);

                    if (translationStatus == SNTI_SEQUENCE_IN_PROGRESS)
                        returnValue = FALSE;
                break;
                case SCSIOP_PERSISTENT_RESERVE_IN:
                    translationStatus = SntiTranslatePersReserveInResponse(pSrb);
                    if (translationStatus == SNTI_SEQUENCE_IN_PROGRESS)
                        returnValue = FALSE;
                    break;
                case SCSIOP_PERSISTENT_RESERVE_OUT:
                    translationStatus = SntiTranslatePersReserveOutResponse(pSrb);
                    if (translationStatus == SNTI_SEQUENCE_IN_PROGRESS)
                        returnValue = FALSE;
                    break;
                default:
                    /* Invalid Condition */
                    ASSERT(FALSE);
                    translationStatus = SNTI_SEQUENCE_ERROR;
                break;
            } /* end switch */
        } else {
            /* SRB is NULL... this function should not be called */
            translationStatus = SNTI_SEQUENCE_ERROR;
        }
    } else {
        if ((pDevExt->ntldrDump == FALSE) && (pSrbExt->pDataBuffer != NULL) &&
            ((opcode == SCSIOP_LOG_SENSE) || (opcode == SCSIOP_MODE_SENSE) ||
            (opcode == SCSIOP_MODE_SENSE10))) {
                    ulStatus = StorPortPatchFreeContiguousMemorySpecifyCache(pDevExt,
                        pSrbExt->pDataBuffer,
                        sizeof(ADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY),
                        MmCached);

                    if (ulStatus != STOR_STATUS_SUCCESS) {
                        ASSERT(FALSE);
                    }
                    pSrbExt->pDataBuffer = NULL;
        }
        /* NVME command status failure */
        pSrb->SrbStatus = SRB_STATUS_ERROR;
        translationStatus = SNTI_SEQUENCE_ERROR;
    }

    if ((pSrb != NULL) &&
        (translationStatus != SNTI_SEQUENCE_COMPLETED) &&
        (translationStatus != SNTI_SEQUENCE_IN_PROGRESS) &&
        (pSrb->SrbStatus != SRB_STATUS_PENDING)) {
        SntiMapCompletionStatus(pSrbExt);
    }

    return returnValue;
} /* SntiCompletionCallbackRoutine */

/******************************************************************************
 * SntiTranslateLogSenseResponse
 *
 * @brief Translates the SCSI Log Sense command. Populates the appropriate SCSI
 *        Log Sense page data based on the NVMe Translation spec. Do not need to
 *        create SQE here as we just complete the command in the build phase (by
 *        returning FALSE to StorPort with SRB status of SUCCESS).
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param pCQEntry - Pointer to completion queue entry
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateLogSenseResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry
)
{
    UINT16 allocLength;
    UINT8 pageCode;

    /* Default to successful command sequence completion */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_SEQUENCE_COMPLETED;

    pageCode = GET_U8_FROM_CDB(pSrb, LOG_SENSE_CDB_PAGE_CODE_OFFSET);
    pageCode &= LOG_SENSE_CDB_PAGE_CODE_MASK;
    allocLength = GET_U16_FROM_CDB(pSrb, LOG_SENSE_CDB_ALLOC_LENGTH_OFFSET);

    /* No parameter checking is necessary... it was done on the submission */

    if (pageCode == LOG_PAGE_INFORMATIONAL_EXCEPTIONS_PAGE) {
        returnStatus =
            SntiTranslateInformationalExceptionsResponse(pSrb, allocLength);
    } else if (pageCode == LOG_PAGE_TEMPERATURE_PAGE) {
        returnStatus =
            SntiTranslateTemperatureResponse(pSrb, pCQEntry, allocLength);
    } else {
        returnStatus = SNTI_SEQUENCE_ERROR;
    }

    return returnStatus;
} /* SntiTranslateLogSenseResponse*/

/******************************************************************************
 * SntiTranslateInformationalExceptionsResponse
 *
 * @brief Translates the Log Sense page - Informational Exceptions Page.
 *        Populates the appropriate log page response fields based on the NVMe
 *        Translation spec. This log page requires both local data storage and
 *        adapter communication. Create the NVMe Admin command - Get Log Page:
 *        SMART/Health Information and populates the temporary SQE stored in the
 *        SRB Extension.
 *
 *        NOTE: Must free extra buffer used for this command.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param allocLength - Allocation Length from Log Sense command
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateInformationalExceptionsResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    UINT16 allocLength
)
{
    PNVME_SRB_EXTENSION pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    PNVME_DEVICE_EXTENSION pDevExt = pSrbExt->pNvmeDevExt;
    PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY pNvmeLogPage = NULL;
    PINFORMATIONAL_EXCEPTIONS_LOG_PAGE pScsiLogPage = NULL;
    UINT16 temperature = 0;
    BOOLEAN storStatus;
    PVOID pBuf = pSrbExt->pDataBuffer;

    /* Default to successful command sequence completion */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_SEQUENCE_COMPLETED;

    /* Parse the SMART/Health Information Log Page */

    /* Must convert temperature from Kelvin to Celsius */
    pNvmeLogPage =
        (PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY)pBuf;
    temperature = pNvmeLogPage->Temperature - KELVIN_TEMP_FACTOR;

    /* Issue DPC to free the data buffer memory in the SRB Extension */
    storStatus = StorPortIssueDpc(
        pDevExt,
        &pDevExt->SntiDpc,
        (PVOID)pSrbExt,
        (PVOID)sizeof(ADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY));

    if (storStatus != TRUE) {
        returnStatus = SNTI_SEQUENCE_ERROR;
        ASSERT(FALSE);
    } else {
        pScsiLogPage = (PINFORMATIONAL_EXCEPTIONS_LOG_PAGE)GET_DATA_BUFFER(pSrb);

        memset(pScsiLogPage, 0, sizeof(INFORMATIONAL_EXCEPTIONS_LOG_PAGE));
        pScsiLogPage->PageCode         = LOG_PAGE_INFORMATIONAL_EXCEPTIONS_PAGE;
        pScsiLogPage->SubPageFormat    = SUB_PAGE_FORMAT_UNSUPPORTED;
        pScsiLogPage->DisableSave      = DISABLE_SAVE_UNSUPPORTED;
        pScsiLogPage->SubPageCode      = SUB_PAGE_CODE_UNSUPPORTED;
        pScsiLogPage->PageLength[0]    = 0;
        pScsiLogPage->PageLength[1]    = REMAINING_INFO_EXCP_PAGE_LENGTH;
        pScsiLogPage->ParameterCode[0] = GENERAL_PARAMETER_DATA;
        pScsiLogPage->ParameterCode[1] = GENERAL_PARAMETER_DATA;
        pScsiLogPage->FormatAndLinking = BINARY_FORMAT_LIST;
        pScsiLogPage->TMC              = TMC_UNSUPPORTED;
        pScsiLogPage->ETC              = ETC_UNSUPPORTED;
        pScsiLogPage->TSD              = LOG_PARAMETER_DISABLED;
        pScsiLogPage->DU               = DU_UNSUPPORTED;
        pScsiLogPage->ParameterLength  = INFO_EXCP_PARM_LENGTH;
        pScsiLogPage->InfoExcpAsc      = INFO_EXCP_ASC_NONE;
        pScsiLogPage->InfoExcpAscq     = INFO_EXCP_ASCQ_NONE;
        pScsiLogPage->MostRecentTempReading = (UINT8)temperature;

        SET_DATA_LENGTH(pSrb,
            min(sizeof(INFORMATIONAL_EXCEPTIONS_LOG_PAGE), allocLength));

        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        returnStatus = SNTI_SEQUENCE_COMPLETED;
    }

    return returnStatus;
} /* SntiTranslateInformationalExceptionsResponse */

/******************************************************************************
 * SntiTranslateTemperatureResponse
 *
 * @brief Translates the NVM Express command(s) for the SCSI Log Sense page -
 *        Temperature Log Page. Populates the appropriate log page response
 *        fields based on the NVMe Translation spec. This log page requires both
 *        local data storage and adapter communication.
 *
 *        NOTE: Must free extra buffer used for this command.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param pCQEntry - Pointer to completion queue entry
 * @param allocLength - Allocation length from Log Sense CDB
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateTemperatureResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry,
    UINT16 allocLength
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PTEMPERATURE_LOG_PAGE pScsiLogPage = NULL;
    PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY pNvmeLogPage = NULL;
    BOOLEAN storStatus;
    UINT16 logTemp;
    PVOID pBuf;

    /* Default to in-progress command sequence */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_SEQUENCE_IN_PROGRESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pDevExt = (PNVME_DEVICE_EXTENSION)pSrbExt->pNvmeDevExt;
    pBuf = pSrbExt->pDataBuffer;

    /*
     * The SCSI Log Page (Temperature page) is being stored in the SRB data
     * buffer
     */
    pScsiLogPage = (PTEMPERATURE_LOG_PAGE)GET_DATA_BUFFER(pSrb);

    /* Check which phase of sequence by checking the SQE unit opcode */
    if (pSrbExt->nvmeSqeUnit.CDW0.OPC == ADMIN_GET_LOG_PAGE) {
        /*
         * Parse the SMART/Health Information Log Page. Must convert temperature
         * from Kelvin to Celsius.
         */
        pNvmeLogPage =
            (PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY)pBuf;
        pScsiLogPage->Temperature = pNvmeLogPage->Temperature -
                                    KELVIN_TEMP_FACTOR;

        /*
         * Do not need the extra buffer any longer since the Get Features
         * response will come in DWORD 0 of the completion queue entry. Issue
         * the DPC to free the data buffer memory in the SRB Extension.
         */
        storStatus = StorPortIssueDpc(
          pDevExt,
          &pDevExt->SntiDpc,
          (PVOID)pSrbExt,
          (PVOID)sizeof(ADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY));

        /* TBD: What do we do if we can't queue the DPC??? */
        if (storStatus != TRUE) {
            returnStatus = SNTI_SEQUENCE_ERROR;
            ASSERT(FALSE);
        } else {
            BOOLEAN ioStarted = FALSE;

            /* Issue the GET FEATURES command (Temperature Threshold Feature) */
            SntiBuildGetFeaturesCmd(pSrbExt, TEMPERATURE_THRESHOLD);

            /* Commmand sequence is still in progress */
            returnStatus = SNTI_SEQUENCE_IN_PROGRESS;
            pSrb->SrbStatus = SRB_STATUS_PENDING;

            /* Issue the command internally */
            ioStarted = ProcessIo(pDevExt, pSrbExt, NVME_QUEUE_TYPE_ADMIN, FALSE);
            if (ioStarted == FALSE)
                ASSERT(FALSE);
        }
    } else if (pSrbExt->nvmeSqeUnit.CDW0.OPC == ADMIN_GET_FEATURES) {
        /* Parse the Get Features DWORD 0 resposne */
        pScsiLogPage->ReferenceTemperature = pCQEntry->DW0 & WORD_LOW_BYTE_MASK;

        /* Commmand sequence is complete */
        returnStatus = SNTI_SEQUENCE_COMPLETED;
        pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        SET_DATA_LENGTH(pSrb, min(sizeof(TEMPERATURE_LOG_PAGE), allocLength));
    } else {
        returnStatus = SNTI_SEQUENCE_ERROR;
        pSrb->SrbStatus = SRB_STATUS_ERROR;
        SET_DATA_LENGTH(pSrb, 0);

        ASSERT(FALSE);
    }

    return returnStatus;
} /* SntiTranslateTemperatureResponse */

/******************************************************************************
 * SntiTranslateModeSenseResponse
 *
 * @brief Translates the NVM Express command(s) response for the indicated SCSI
 *        Mode Sense command. Parses the appropriate SCSI Mode Sense page data
 *        based on the NVMe Translation spec.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param pCQEntry - Pointer to completion queue entry
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicate translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateModeSenseResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_LUN_EXTENSION pLunExt = NULL;
    UINT16 allocLength;
    UINT8 longLbaAccepted;
    UINT8 disableBlockDesc;
    UINT8 pageCode;
    BOOLEAN modeSense10;
	UCHAR scsiStatus = 0;
	BOOLEAN ioStarted = FALSE;
	PNVME_DEVICE_EXTENSION pDevExt = NULL;
	BOOLEAN supportsVwc = FALSE;
	PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY pNvmeLogPage = NULL;
	PVOID pBuf = NULL;
	ULONG ulStatus;


    /* Default to successful command sequence completion */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_SEQUENCE_COMPLETED;
    SNTI_STATUS status = SNTI_SUCCESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    longLbaAccepted = GET_U8_FROM_CDB(pSrb, MODE_SENSE_CDB_LLBAA_OFFSET);
    disableBlockDesc = GET_U8_FROM_CDB(pSrb, MODE_SENSE_CDB_DBD_OFFSET);
    pageCode = GET_U8_FROM_CDB(pSrb, MODE_SENSE_CDB_PAGE_CODE_OFFSET);

    if (GET_OPCODE(pSrb) == SCSIOP_MODE_SENSE) {
        /* MODE SENSE 6 */
        allocLength =
            GET_U8_FROM_CDB(pSrb, MODE_SENSE_6_CDB_ALLOC_LENGTH_OFFSET);
        modeSense10 = FALSE;
    } else {
        /* MODE SENSE 10 */
        allocLength =
            GET_U16_FROM_CDB(pSrb, MODE_SENSE_10_CDB_ALLOC_LENGTH_OFFSET);
        modeSense10 = TRUE;
    }

    /* Mask and get LLBAA, DBD, PC, and Page code */
    longLbaAccepted = (longLbaAccepted & MODE_SENSE_CDB_LLBAA_MASK) >>
                       MODE_SENSE_CDB_LLBAA_SHIFT;
    disableBlockDesc = (disableBlockDesc & MODE_SENSE_CDB_DBD_MASK) >>
                        MODE_SENSE_CDB_DBD_SHIFT;
    pageCode &= MODE_SENSE_CDB_PAGE_CODE_MASK;

    status = GetLunExtension(pSrbExt, &pLunExt);
    if (status != SNTI_SUCCESS) {
        /* Map the translation error to a SCSI error */
        SntiMapInternalErrorStatus(pSrb, status);
        returnStatus = SNTI_SEQUENCE_ERROR;
    } else {
        switch (pageCode) {
            case MODE_PAGE_CACHING:
                SntiTranslateCachingModePageResponse(pSrbExt,
                                                     pLunExt,
                                                     pCQEntry,
                                                     allocLength,
                                                     longLbaAccepted,
                                                     disableBlockDesc,
                                                     modeSense10);
            break;
            case MODE_SENSE_RETURN_ALL:
				pDevExt = ((PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb))->pNvmeDevExt;
				supportsVwc = pDevExt->controllerIdentifyData.VWC.Present;
				pBuf = pSrbExt->pDataBuffer;

				switch (pSrbExt->ModeSenseWaitState)
				{
				case MODE_SENSE_WAIT_FOR_GET_LOG_PAGE_RESPONSE:
					pNvmeLogPage = (PADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY)pBuf;

					if (modeSense10 == FALSE) {
						/* MODE SENSE 6 */
						PMODE_PARAMETER_HEADER pModeHeader6 = (PMODE_PARAMETER_HEADER)(pSrbExt->modeSenseBuf);
						if (pNvmeLogPage->CriticalWarning.MediaInReadOnlyMode == 1) {
							pModeHeader6->DeviceSpecificParameter = pModeHeader6->DeviceSpecificParameter | WRITE_PROTECT;
#if DBG
							StorPortDebugPrint(INFO, "SntiTranslateModeSenseResponse: <Info> Device is in Read Only mode\n");
#endif
							pLunExt->IsNamespaceReadOnly = TRUE;
						}
					}
					else {
						/* MODE SENSE 10 */
						PMODE_PARAMETER_HEADER10 pModeHeader10 = (PMODE_PARAMETER_HEADER10)(pSrbExt->modeSenseBuf);
						if (pNvmeLogPage->CriticalWarning.MediaInReadOnlyMode == 1) {
							pModeHeader10->DeviceSpecificParameter = pModeHeader10->DeviceSpecificParameter | WRITE_PROTECT;
#if DBG
							StorPortDebugPrint(INFO, "SntiTranslateModeSenseResponse: <Info> Device is in Read Only mode\n");
#endif
							pLunExt->IsNamespaceReadOnly = TRUE;
						}
					}

					if ((pDevExt->ntldrDump == FALSE) && (pSrbExt->pDataBuffer != NULL)) {
						ulStatus = StorPortPatchFreeContiguousMemorySpecifyCache(pDevExt,
							pSrbExt->pDataBuffer,
							sizeof(ADMIN_GET_LOG_PAGE_SMART_HEALTH_INFORMATION_LOG_ENTRY),
							MmCached);

						if (ulStatus != STOR_STATUS_SUCCESS) {
							ASSERT(FALSE);
						}
						pSrbExt->pDataBuffer = NULL;
					}

					/*Copy the response of GetLogPage command*/
					StorPortCopyMemory((PVOID)GET_DATA_BUFFER(pSrb),
						(PVOID)(pSrbExt->modeSenseBuf), GET_DATA_LENGTH(pSrb));

					if (supportsVwc == TRUE) {

						pSrbExt->pNvmeCompletionRoutine = SntiCompletionCallbackRoutine;

						/* Finally, make sure we issue the GET FEATURES command */
						SntiBuildGetFeaturesCmd(pSrbExt, VOLATILE_WRITE_CACHE);

						ioStarted = ProcessIo(pSrbExt->pNvmeDevExt,
							pSrbExt,
							NVME_QUEUE_TYPE_ADMIN,
							TRUE);

						if (ioStarted == FALSE)
							ASSERT(FALSE);

						pSrb->SrbStatus = SRB_STATUS_PENDING;

						pSrbExt->ModeSenseWaitState = MODE_SENSE_WAIT_FOR_GET_FEATURE_RESPONSE;
						returnStatus = SNTI_SEQUENCE_IN_PROGRESS;
					}
					else {
#if (NTDDI_VERSION > NTDDI_WIN7)
						scsiStatus = SCSISTAT_GOOD;
						SrbSetScsiData(pSrb, NULL, NULL, &scsiStatus, NULL, NULL);
#else
						pSrb->ScsiStatus = SCSISTAT_GOOD;
#endif
						pSrb->SrbStatus = SRB_STATUS_SUCCESS;
						returnStatus = SNTI_COMMAND_COMPLETED;
					}

					break;

				case MODE_SENSE_WAIT_FOR_GET_FEATURE_RESPONSE:
					SntiTranslateReturnAllModePagesResponse(pSrbExt,
						pCQEntry,
						modeSense10);
					returnStatus = SNTI_COMMAND_COMPLETED;
					break;
				}
				break;
            default:
                ASSERT(FALSE);
                SntiSetScsiSenseData(pSrb,
                                     SCSISTAT_CHECK_CONDITION,
                                     SCSI_SENSE_ILLEGAL_REQUEST,
                                     SCSI_ADSENSE_NO_SENSE,
                                     SCSI_ADSENSE_NO_SENSE);

                pSrb->SrbStatus |= SRB_STATUS_ERROR;
                SET_DATA_LENGTH(pSrb, 0);
                returnStatus = SNTI_SEQUENCE_ERROR;
            break;
        } /* end switch */
    }

    return returnStatus;
} /* SntiTranslateModeSenseResponse */

/******************************************************************************
 * SntiTranslateCachingModePageResponse
 *
 * @brief Translates the NVM Express command(s) to support the Mode Sense page -
 *        Caching Mode Page. Populates the appropriate mode page response fields
 *        based on the NVMe Translation spec.
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param pSrbExt - Pointer to LUN extension
 * @param pCQEntry - Pointer to completion queue entry
 * @param allocLength - Allocation length from Mode Sense CDB
 * @param longLbaAccepted - Boolean for long LBA
 * @param disableBlockDesc - Boolean for DBD form Mose Sense CDB
 * @param modeSense10 - Boolean to determine if Mode Sense 10
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateCachingModePageResponse(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10
)
{
    PMODE_PARAMETER_HEADER pModeHeader6 = NULL;
    PMODE_PARAMETER_HEADER10 pModeHeader10 = NULL;
    PMODE_PARAMETER_BLOCK pModeParamBlock = NULL;
    PCACHING_MODE_PAGE pCachingModePage = NULL;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
    UCHAR scsiStatus = 0;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    UINT32 volatileWriteCache = 0;
    UINT16 modeDataLength = 0;
    UINT16 blockDescLength = 0;


    /* The Volatile Write Cache info will be stored in DWORD 0 of the CQE */
    volatileWriteCache = pCQEntry->DW0;

    memset(GET_DATA_BUFFER(pSrb), 0, allocLength);

    /* Determine which Mode Parameter Descriptor Block to use (8 or 16) */
    if (longLbaAccepted == 0)
        blockDescLength = SHORT_DESC_BLOCK;
    else
        blockDescLength = LONG_DESC_BLOCK;

    /* Mode Page Header */
    SntiCreateModeDataHeader(pSrbExt,
                             &pModeParamBlock,
                             &modeDataLength,
                             (disableBlockDesc ? 0 : blockDescLength),
                             modeSense10);

    /* Check if block descriptors enabled, if not, then mode pages comes next */
    if (disableBlockDesc == BLOCK_DESCRIPTORS_ENABLED) {
        /* Mode Parameter Descriptor Block */
    SntiCreateModeParameterDescBlock(pLunExt,
                                     pModeParamBlock,
                                     &modeDataLength);

        /* Increment pointer to after block descriptor */
        pModeParamBlock++;
    }

    /* Caching Mode Page */
    pCachingModePage = (PCACHING_MODE_PAGE)pModeParamBlock;
    modeDataLength += sizeof(CACHING_MODE_PAGE);

    memset(pCachingModePage, 0, sizeof(CACHING_MODE_PAGE));
    pCachingModePage->PageCode    = MODE_PAGE_CACHING;
    pCachingModePage->PageSavable = MODE_PAGE_PARAM_SAVEABLE_DISABLED;
    pCachingModePage->PageLength  = CACHING_MODE_PAGE_LENGTH;
    pCachingModePage->WriteCacheEnable = volatileWriteCache &
                                         VOLATILE_WRITE_CACHE_MASK;

    /* Now go back and set the Mode Data Length in the header */
    if (modeSense10 == FALSE) {
        /* Get the correct header that starts at the buffer beginning */
        pModeHeader6 = (PMODE_PARAMETER_HEADER)(pSrbExt->modeSenseBuf);
        pModeHeader6->ModeDataLength = (UCHAR)(modeDataLength - 1);
    } else {
        /* Get the correct header that starts at the buffer beginning */
        pModeHeader10 = (PMODE_PARAMETER_HEADER10)(pSrbExt->modeSenseBuf);
        pModeHeader10->ModeDataLength[0] =
            ((modeDataLength - 2) & WORD_HIGH_BYTE_MASK) >> BYTE_SHIFT_1;
        pModeHeader10->ModeDataLength[1] =
            ((modeDataLength - 2) & WORD_LOW_BYTE_MASK);
    }

    SET_DATA_LENGTH(pSrb, min(modeDataLength, allocLength));
    StorPortCopyMemory((PVOID)GET_DATA_BUFFER(pSrb),
        (PVOID)(pSrbExt->modeSenseBuf), GET_DATA_LENGTH(pSrb));
#if (NTDDI_VERSION > NTDDI_WIN7)
    scsiStatus = SCSISTAT_GOOD;
    SrbSetScsiData(pSrb, NULL, NULL, &scsiStatus, NULL, NULL);
#else
    pSrb->ScsiStatus = SCSISTAT_GOOD;
#endif
    pSrbExt->pSrb->SrbStatus = SRB_STATUS_SUCCESS;
} /* SntiTranslateCachingModePageResponse */

/******************************************************************************
 * SntiTranslateReturnAllModePagesResponse
 *
 * @brief Translates the final NVM Express command to complet the Mode Sense
 *        page request - Return All Mode Pages. Populates the appropriate mode
 *        page response fields based on the NVMe Translation spec.
 *
 * @param pSrbExt - Pointer to the SRB extension for this command
 * @param pCQEntry - Pointer to completion queue entry
 * @param modeSense10 - Boolean to determine if Mode Sense 10 request
 *
 * @return VOID
 ******************************************************************************/
VOID SntiTranslateReturnAllModePagesResponse(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry,
    BOOLEAN modeSense10
)
{
    PUCHAR pBuffPtr = NULL;
    PCACHING_MODE_PAGE pCachingModePage = NULL;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
    UCHAR scsiStatus = 0;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    UINT32 volatileWriteCache = 0;

    /* The Volatile Write Cache info will be stored in DWORD 0 of the CQE */
    volatileWriteCache = pCQEntry->DW0;

    /*
     * Find the offset to the caching mode page by using the data transfer
     * length field set in the build I/O phase and then backing up by the size
     * of the caching mode page.
     */
    pBuffPtr = (PUCHAR)GET_DATA_BUFFER(pSrb);
    pBuffPtr += GET_DATA_LENGTH(pSrb);

    /* Subtract the size of the header */
    if (modeSense10 == FALSE)
        pBuffPtr -= sizeof(MODE_PARAMETER_HEADER);
    else
        pBuffPtr -= sizeof(MODE_PARAMETER_HEADER10);

    pCachingModePage = (PCACHING_MODE_PAGE)pBuffPtr;
    pCachingModePage--;

    pCachingModePage->WriteCacheEnable =
        volatileWriteCache & VOLATILE_WRITE_CACHE_MASK;
#if (NTDDI_VERSION > NTDDI_WIN7)
    scsiStatus = SCSISTAT_GOOD;
    SrbSetScsiData(pSrb, NULL, NULL, &scsiStatus, NULL, NULL);
#else
    pSrb->ScsiStatus = SCSISTAT_GOOD;
#endif

    pSrb->SrbStatus = SRB_STATUS_SUCCESS;
} /* SntiTranslateReturnAllModePagesResponse */

/******************************************************************************
 * SntiTranslateStartStopUnitResponse
 *
 * @brief Translates the appropriate NVM Express command response that will
 *        complete the SCSI START/STOP UNIT request.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicate translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateStartStopUnitResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    UINT8 powerCondMod = 0;
    UINT8 powerCond = 0;
    UINT8 start = 0;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_SEQUENCE_IN_PROGRESS;

    /* NOTE: Parameter checking was done on submission side */
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    powerCondMod = GET_U8_FROM_CDB(pSrb,
                                   START_STOP_UNIT_CDB_POWER_COND_MOD_OFFSET);
    powerCond = GET_U8_FROM_CDB(pSrb, START_STOP_UNIT_CDB_POWER_COND_OFFSET);
    start = GET_U8_FROM_CDB(pSrb, START_STOP_UNIT_CDB_START_OFFSET);

    powerCondMod &= START_STOP_UNIT_CDB_POWER_COND_MOD_MASK;
    powerCond &= START_STOP_UNIT_CDB_POWER_COND_MASK;
    start &= START_STOP_UNIT_CDB_START_MASK;

    /* Now perform the expected power state transition */
    returnStatus = SntiTransitionPowerState(pSrbExt,
                                            powerCond,
                                            powerCondMod,
                                            start);

    if (returnStatus == SNTI_TRANSLATION_SUCCESS) {
        BOOLEAN ioStarted = FALSE;

        /*
         * If the power state transtion function is successful, then the
         * command sequence is still in process... make sure we don't complete
         * the START STOP UNIT command yet.
         *
         * Issue the Get/Set Features command internally.
         */
        ioStarted = ProcessIo(pSrbExt->pNvmeDevExt,
                              pSrbExt,
                              NVME_QUEUE_TYPE_ADMIN,
                              FALSE);

        if (ioStarted == FALSE)
            ASSERT(FALSE);

        pSrb->SrbStatus = SRB_STATUS_PENDING;
        returnStatus = SNTI_SEQUENCE_IN_PROGRESS;
    } else {
        /*
         * Otherwise, the command sequence has somehow failed...  reset the SRB
         * status to indicate an error (sense data will already be set).
         */
        pSrb->SrbStatus = 0;
        pSrb->SrbStatus |= SRB_STATUS_ERROR;
        pSrb->SrbStatus |= SRB_STATUS_AUTOSENSE_VALID;
        returnStatus = SNTI_SEQUENCE_ERROR;
    }

    /*
     * Clear the completion callback routine so we don't get back here after
     * the power transition.
     */
    pSrbExt->pNvmeCompletionRoutine = NULL;

    return returnStatus;
} /* SntiTranslateStartStopUnitResponse */

/******************************************************************************
 * SntiTranslateWriteBufferResponse
 *
 * @brief Translates the appropriate NVM Express command for completing the
 *        SCSI Write Buffer requests.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return SNTI_TRANSLATION_STATUS
 *     Indicates translation status
 ******************************************************************************/
SNTI_TRANSLATION_STATUS SntiTranslateWriteBufferResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    PSTOR_SCATTER_GATHER_LIST pSgl = NULL;
    UINT32 bufferOffset = 0; /* 3 byte field */
    UINT32 paramListLength = 0; /* 3 byte field */
    UINT32 dword10 = 0;
    UINT32 dword11 = 0;
    UINT8 bufferId = 0;
    UINT8 mode = 0;

    /* Default to successful translation */
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_SEQUENCE_IN_PROGRESS;
    pSrbExt = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    pDevExt = (PNVME_DEVICE_EXTENSION)pSrbExt->pNvmeDevExt;
    mode = GET_U8_FROM_CDB(pSrb, WRITE_BUFFER_CDB_MODE_OFFSET);
    bufferId = GET_U8_FROM_CDB(pSrb, WRITE_BUFFER_CDB_BUFFER_ID_OFFSET);

    switch (mode & WRITE_BUFFER_CDB_MODE_MASK) {
        case DOWNLOAD_SAVE_ACTIVATE:
            if (pSrbExt->nvmeSqeUnit.CDW0.OPC ==
                ADMIN_FIRMWARE_IMAGE_DOWNLOAD) {
                BOOLEAN ioStarted = FALSE;
                dword10 |= bufferId;
                dword10 |= 0x00000008;

                /* Activate microcode upon completion of FW Image Download */
                SntiBuildFirmwareActivateCmd(pSrbExt, dword10);

                returnStatus = SNTI_SEQUENCE_IN_PROGRESS;
                pSrb->SrbStatus = SRB_STATUS_PENDING;

                /* Issue the command internally */
                ioStarted = ProcessIo(pDevExt, pSrbExt, NVME_QUEUE_TYPE_ADMIN, FALSE);
                if (ioStarted == FALSE)
                 ASSERT(FALSE);
               
            } else if (pSrbExt->nvmeSqeUnit.CDW0.OPC ==
                       ADMIN_FIRMWARE_ACTIVATE) {
                /* Command is complete */
                returnStatus = SNTI_SEQUENCE_COMPLETED;
                pSrb->SrbStatus = SRB_STATUS_SUCCESS;
            } else {
                returnStatus = SNTI_SEQUENCE_ERROR;
                pSrb->SrbStatus = SRB_STATUS_ERROR;
                ASSERT(FALSE);
            }
        break;
        case DOWNLOAD_SAVE_DEFER_ACTIVATE:
        case ACTIVATE_DEFERRED_MICROCODE:
            /* NVME completion status already checked, commands are complete */
            returnStatus = SNTI_SEQUENCE_COMPLETED;
            pSrb->SrbStatus = SRB_STATUS_SUCCESS;
        break;
        default:
            returnStatus = SNTI_SEQUENCE_ERROR;
            pSrb->SrbStatus = SRB_STATUS_ERROR;
        break;
    } /* end switch */

    return returnStatus;
} /* SntiTranslateWriteBufferResponse */

/******************************************************************************
 * SntiMapCompletionStatus
 *
 * @brief Entry function to perform the mapping of the NVM Express Command
 *        Status to a SCSI Status Code, sense key, and Additional Sense
 *        Code/Qualifier (ASC/ASCQ) where applicable.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 *
 * @return BOOLEAN
 *     Status to indicate if the status translation was successful.
 ******************************************************************************/
BOOLEAN SntiMapCompletionStatus(
    PNVME_SRB_EXTENSION pSrbExt
)
{
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#else
    PSCSI_REQUEST_BLOCK pSrb = pSrbExt->pSrb;
#endif
    UINT8 statusCodeType = (UINT8)pSrbExt->pCplEntry->DW3.SF.SCT;
    UINT8 statusCode = (UINT8)pSrbExt->pCplEntry->DW3.SF.SC;
    BOOLEAN returnValue = TRUE;

    if (pSrb != NULL) {
        switch(statusCodeType) {
            case GENERIC_COMMAND_STATUS:
                SntiMapGenericCommandStatus(pSrb, statusCode);
            break;
            case COMMAND_SPECIFIC_ERRORS:
                SntiMapCommandSpecificStatus(pSrb, statusCode);
            break;
            case MEDIA_ERRORS:
                SntiMapMediaErrors(pSrb, statusCode);
            break;
            default:
                returnValue = FALSE;
            break;
        }

        if(pSrb->Function == SRB_FUNCTION_ABORT_COMMAND){                
            if ((pSrb->SrbStatus & SRB_STATUS_SUCCESS) 
                != SRB_STATUS_SUCCESS) {
                    pSrbExt->failedAbortCmdCnt++;
            }
            if (pSrbExt->abortedCmdCount) 
                pSrbExt->abortedCmdCount--;
            if (pSrbExt->issuedAbortCmdCnt) 
                pSrbExt->issuedAbortCmdCnt--;

            if (pSrbExt->issuedAbortCmdCnt == 0) {
                if (pSrbExt->abortedCmdCount || 
                    pSrbExt->failedAbortCmdCnt) {
                        pSrb->SrbStatus = SRB_STATUS_ERROR;
                }
                returnValue = TRUE;
            } else
                returnValue = FALSE;
        }

    } else {
        returnValue = FALSE;
    }
    ASSERT(returnValue == TRUE);
    return returnValue;
} /* SntiMapCompletionStatus */

/******************************************************************************
 * SntiMapGenericCommandStatus
 *
 * @brief Maps the NVM Express Generic Command Status to a SCSI Status Code,
 *        sense key, and Additional Sense Code/Qualifier (ASC/ASCQ) where
 *        applicable.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param genericCommandStatus - NVMe Generic Command Status to translate
 *
 * @return VOID
 ******************************************************************************/
VOID SntiMapGenericCommandStatus(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    UINT8 genericCommandStatus
)
{
    SNTI_RESPONSE_BLOCK responseData;

    memset(&responseData, 0, sizeof(SNTI_RESPONSE_BLOCK));

    /**
     * Perform table lookup for Generic Command Status translation
     *
     * Generic Status Code Values:
     *   0x00 - 0x0B and
     *   0x80 - 0x82 (0x0C - 0xE)
     *
     * Check bit 7 to see if this is a NVM Command Set status, if so then
     * start at 0xC to index into lookup table
     */
    if ((genericCommandStatus & NVM_CMD_SET_STATUS) != NVM_CMD_SET_STATUS)
        responseData = genericCommandStatusTable[genericCommandStatus];
    else
        responseData = genericCommandStatusTable[genericCommandStatus -
                       NVM_CMD_SET_GENERIC_STATUS_OFFSET];

    SntiSetScsiSenseData(pSrb,
                         responseData.StatusCode,
                         responseData.SenseKey,
                         responseData.ASC,
                         responseData.ASCQ);

    /* Override the SRB Status */
    pSrb->SrbStatus |= responseData.SrbStatus;
} /* SntiMapGenericCommandStatus */

/******************************************************************************
 * SntiMapCommandSpecificStatus
 *
 * @brief Maps the NVM Express Command Specific Status to a SCSI Status Code,
 *        sense key, and Additional Sense Code/Qualifier (ASC/ASCQ) where
 *        applicable.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param commandSpecificStatus - NVMe Command Specific Status to translate
 *
 * @return VOID
 ******************************************************************************/
VOID SntiMapCommandSpecificStatus(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    UINT8 commandSpecificStatus
)
{
    SNTI_RESPONSE_BLOCK responseData;

    memset(&responseData, 0, sizeof(SNTI_RESPONSE_BLOCK));
   
	/**
	* Perform table lookup for Generic Command Status translation
	*
	* Command Specific Status Code Values:
	*   0x00 - 0x0A and
	*   0x80 (0x0B)
	*
	* Check bit 7 to see if this is a NVM Command Set status, if so then
	* start at 0xB to index into lookup table
	*/
	if ((commandSpecificStatus & NVM_CMD_SET_STATUS) != NVM_CMD_SET_STATUS)
		responseData = commandSpecificStatusTable[commandSpecificStatus];
	else
		responseData = commandSpecificStatusTable[commandSpecificStatus -
		               NVM_CMD_SET_SPECIFIC_STATUS_OFFSET];
  

    SntiSetScsiSenseData(pSrb,
                         responseData.StatusCode,
                         responseData.SenseKey,
                         responseData.ASC,
                         responseData.ASCQ);

    /* Override the SRB Status */
    pSrb->SrbStatus |= responseData.SrbStatus;
} /* SntiMapCommandSpecificStatus */

/******************************************************************************
 * SntiMapMediaErrors
 *
 * @brief Maps the NVM Express Media Error Status to a SCSI Status Code, sense
 *        key, and Additional Sense Code/Qualifier (ASC/ASCQ) where applicable.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param mediaError - NVMe Media Error Status to translate
 *
 * @return VOID
 ******************************************************************************/
VOID SntiMapMediaErrors(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    UINT8 mediaError
)
{
    SNTI_RESPONSE_BLOCK responseData;

    memset(&responseData, 0, sizeof(SNTI_RESPONSE_BLOCK));

    /*
     * Perform table lookup for Generic Command Status translation
     *
     * Media Error Status Code Values: 0x80 - 0x86
     */
    responseData = mediaErrorTable[mediaError - NVM_MEDIA_ERROR_STATUS_OFFSET];

    SntiSetScsiSenseData(pSrb,
                         responseData.StatusCode,
                         responseData.SenseKey,
                         responseData.ASC,
                         responseData.ASCQ);

    /* Override the SRB Status */
    pSrb->SrbStatus |= responseData.SrbStatus;
} /* SntiMapMediaErrors */

/******************************************************************************
 * SntiMapInternalErrorStatus
 *
 * @brief Maps an Internal Error Status to a SCSI Status Code, sense key, and
 *        Additional Sense Code/Qualifier (ASC/ASCQ) where applicable.
 *
 * @param pSrb - This parameter specifies the SCSI I/O request. SNTI expects
 *               that the user can access the SCSI CDB, response, and data from
 *               this pointer. For example, if there is a failure in translation
 *               resulting in sense data, then SNTI will call the appropriate
 *               internal error handling code and set the status info/data and
 *               pass the pSrb pointer as a parameter.
 * @param status - NVMe Error Status to translate
 *
 * @return VOID
 ******************************************************************************/
VOID SntiMapInternalErrorStatus(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    SNTI_STATUS status
)
{
    /* Switch on internal SNTI error */
    switch (status) {
        case SNTI_FAILURE:
            SntiSetScsiSenseData(pSrb,
                                 SCSISTAT_CHECK_CONDITION,
                                 SCSI_SENSE_UNIQUE,
                                 SCSI_ADSENSE_INTERNAL_TARGET_FAILURE,
                                 SCSI_ADSENSE_NO_SENSE);

            pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        break;
        case SNTI_INVALID_REQUEST:
        case SNTI_INVALID_PATH_TARGET_ID:
            SntiSetScsiSenseData(pSrb,
                                 SCSISTAT_CHECK_CONDITION,
                                 SCSI_SENSE_ILLEGAL_REQUEST,
                                 SCSI_ADSENSE_INVALID_CDB,
                                 SCSI_ADSENSE_NO_SENSE);

            pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        break;
        case SNTI_INVALID_PARAMETER:
            SntiSetScsiSenseData(pSrb,
                                 SCSISTAT_CHECK_CONDITION,
                                 SCSI_SENSE_ILLEGAL_REQUEST,
                                 SCSI_ADSENSE_INVALID_FIELD_PARAMETER_LIST,
                                 SCSI_ADSENSE_NO_SENSE);

            pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        break;
        case SNTI_NO_MEMORY:
            SntiSetScsiSenseData(pSrb,
                                 SCSISTAT_CHECK_CONDITION,
                                 SCSI_SENSE_ILLEGAL_REQUEST,
                                 SCSI_ADSENSE_INVALID_CDB,
                                 SCSI_ADSENSE_NO_SENSE);

            pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        break;
        default:
            ASSERT(FALSE);
        break;
    }
} /* SntiMapInternalErrorStatus */

/******************************************************************************
 * SntiDpcRoutine
 *
 * @brief SNTI DPC routine to feee memory that was allocated (since we cannot
 *        call the Storport API to free memory at DIRQL).
 *
 * @param pDpc - Pointer to SNTI DPC
 * @param pHwDeviceExtension - Pointer to device extension
 * @param pSystemArgument1 - Arg 1
 * @param pSystemArgument2 - Arg 2
 *
 * @return VOID
 ******************************************************************************/
VOID SntiDpcRoutine(
    IN PSTOR_DPC  pDpc,
    IN PVOID  pHwDeviceExtension,
    IN PVOID  pSystemArgument1,
    IN PVOID  pSystemArgument2
)
{
    PNVME_SRB_EXTENSION pSrbExt = NULL;
    PNVME_DEVICE_EXTENSION pDevExt = NULL;
    ULONG bufferSize;
    ULONG status;

    pDevExt = (PNVME_DEVICE_EXTENSION)pHwDeviceExtension;
    pSrbExt = (PNVME_SRB_EXTENSION)pSystemArgument1;
    bufferSize = PtrToUlong(pSystemArgument2);

    status = StorPortPatchFreeContiguousMemorySpecifyCache(pDevExt,
                                                      pSrbExt->pDataBuffer,
                                                      bufferSize,
                                                      MmCached);

    if (status != STOR_STATUS_SUCCESS) {
        ASSERT(FALSE);
    }
    pSrbExt->pDataBuffer = NULL;
} /* SntiDpcRoutine */

/******************************************************************************
 * SntiAllocatePhysicallyContinguousBuffer
 *
 * @brief Allocates a physically contiguous data buffer for use w/ PRP lists.
 *
 * @param pSrbExt - Pointer to SRB extension
 * @param bufferSize - Size of buffer to allcoate
 *
 * @return BOOLEAN
 *     Indicates if the memory allocation was successful.
 ******************************************************************************/
PVOID SntiAllocatePhysicallyContinguousBuffer(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT32 bufferSize
)
{
    PHYSICAL_ADDRESS low;
    PHYSICAL_ADDRESS high;
    PHYSICAL_ADDRESS align;
    PVOID pBuffer = NULL;
    ULONG status = 0;

    /* Set up preferred range and alignment before allocating */
    low.QuadPart = 0;
    high.QuadPart = (-1);
    align.QuadPart = 0;

    status = StorPortPatchAllocateContiguousMemorySpecifyCacheNode(
                 pSrbExt->pNvmeDevExt, bufferSize, low, high,
                 align, MmCached, 0, (PVOID)&pBuffer);

    StorPortDebugPrint(INFO,
                       "NVMeAllocateMem: Size=0x%x\n",
                       bufferSize );

    /* It fails, log the error and return 0 */
    if ((status != 0) || (pBuffer == NULL)) {
        StorPortDebugPrint(ERROR,
                           "NVMeAllocateMem:<Error> Failure, sts=0x%x\n",
                           status);

        return NULL;
    }

    /* Zero out the buffer before return */
    memset (pBuffer, 0, bufferSize);

    return pBuffer;
} /* SntiAllocatePhysicallyContinguousBuffer */


/******************************************************************************
* SntiValidateNacaSetting
*
* @brief Ensure the cdb is not specifying NACA
*
* @param pDevExt - Pointer to the NVMe Device Extension block.
* @param pSrb    - Pointer to the SRB
*
* @return SNTI_TRANSLATION_STATUS
*     Indicate the completion status of this function.
******************************************************************************/
SNTI_TRANSLATION_STATUS SntiValidateNacaSetting(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
    )
{

    UINT8 control = 0;
    UINT32 offset = 0;
    SNTI_TRANSLATION_STATUS returnStatus = SNTI_TRANSLATION_SUCCESS;

    switch (GET_OPCODE(pSrb)) {
    case SCSIOP_READ6:
    case SCSIOP_WRITE6:
    case SCSIOP_INQUIRY:
    case SCSIOP_MODE_SELECT:
    case SCSIOP_MODE_SENSE:
    case SCSIOP_REQUEST_SENSE:
    case SCSIOP_START_STOP_UNIT:
    case SCSIOP_FORMAT_UNIT:
    case SCSIOP_TEST_UNIT_READY:
        offset = CDB_6_CONTROL_OFFSET;
        break;

    case SCSIOP_READ:
    case SCSIOP_WRITE:
    case SCSIOP_LOG_SENSE:
    case SCSIOP_MODE_SELECT10:
    case SCSIOP_MODE_SENSE10:
    case SCSIOP_READ_CAPACITY:
    case SCSIOP_SYNCHRONIZE_CACHE:
    case SCSIOP_WRITE_DATA_BUFF:
    case SCSIOP_PERSISTENT_RESERVE_IN:
    case SCSIOP_PERSISTENT_RESERVE_OUT:
#if (NTDDI_VERSION > NTDDI_WIN7)
    case SCSIOP_UNMAP:
#endif
        offset = CDB_10_CONTROL_OFFSET;
        break;

    case SCSIOP_READ12:
    case SCSIOP_WRITE12:
    case SCSIOP_REPORT_LUNS:
    case SCSIOP_SECURITY_PROTOCOL_IN:
    case SCSIOP_SECURITY_PROTOCOL_OUT:
        offset = CDB_12_CONTROL_OFFSET;
        break;

    case SCSIOP_READ16:
    case SCSIOP_WRITE16:
    case SCSIOP_READ_CAPACITY16:
    case SCSIOP_SYNCHRONIZE_CACHE16:
        offset = CDB_16_CONTROL_OFFSET;
        break;

    default:
        StorPortDebugPrint(ERROR, "SntiValidateNacaSetting: Invalid SCSI Op code: 0x%x\n",
            GET_OPCODE(pSrb));

        SntiSetScsiSenseData(pSrb,
            SCSISTAT_CHECK_CONDITION,
            SCSI_SENSE_ILLEGAL_REQUEST,
            SCSI_ADSENSE_ILLEGAL_COMMAND,
            SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        SET_DATA_LENGTH(pSrb, 0);

        returnStatus = SNTI_UNSUPPORTED_SCSI_REQUEST;
        return returnStatus;
    }

    control = GET_U8_FROM_CDB(pSrb, offset);
    if ((control & CONTROL_BYTE_NACA_MASK) == CONTROL_BYTE_NACA_MASK) {
        StorPortDebugPrint(ERROR, "SntiValidateNacaSetting: Invalid CDB. Naca is set. OpCode: 0x%x    Control: 0x%x\n",
            GET_OPCODE(pSrb),
            control);

        SntiSetScsiSenseData(pSrb,
            SCSISTAT_CHECK_CONDITION,
            SCSI_SENSE_ILLEGAL_REQUEST,
            SCSI_ADSENSE_INVALID_CDB,
            SCSI_ADSENSE_NO_SENSE);

        pSrb->SrbStatus |= SRB_STATUS_INVALID_REQUEST;
        returnStatus = SNTI_FAILURE_CHECK_RESPONSE_DATA;
        SET_DATA_LENGTH(pSrb, 0);
    }

    return returnStatus;
}
