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
 * File: nvmePwrMgmt.c
 */

#include "precomp.h"
#ifndef DBG
#include "nvmePwrMgmt.tmh"
#endif

/*******************************************************************************
 * NVMeAdapterControlPowerUp
 *
 * @brief Routine for powering up the controller
 * 
 * @param pAE - pointer to device extension
 * 
 * @return BOOLEAN
 *     TRUE - command was processed successfully
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeAdapterControlPowerUp(
    IN PNVME_DEVICE_EXTENSION pAE
)
{
    BOOLEAN status = TRUE;

    /* Reset the controller */
    if (NVMeReInitializeController(pAE) == FALSE) {
        NVMeFreeBuffers(pAE);
        return (FALSE);
    }

    pAE->ShutdownInProgress = FALSE;
    StorPortDebugPrint(INFO, "NvmeAdapterControlPowerUp: returning TRUE\n");
    return status;
}

/*******************************************************************************
 * NVMeAdapterControlPowerDown
 *
 * @brief This function is called when srb function is SRB_FUNCTION_POWER or
 *        from NVMeAdapterControl when Control type is ScsiStopAdapter.
 * 
 * @param pAE - pointer to device extension
 * 
 * @return BOOLEAN
 *     TRUE - command was processed successfully
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMeAdapterControlPowerDown(
    IN PNVME_DEVICE_EXTENSION pAE
)
{
    BOOLEAN status = FALSE;

    ASSERT(pAE != NULL);

    /*
     * The controller is powered down when we get SRB_FUNCTION_POWER in
     * StartIo... nothing else to do, just return.
     */
    if (pAE->ShutdownInProgress == TRUE) {
        /* Shutdown */
        status = TRUE;
    } else {
		pAE->ShutdownInProgress = TRUE;

        /* Hibernate or Sleep - sanity check that there is no cmd pending */
        if (NVMeDetectPendingCmds(pAE, FALSE, SRB_STATUS_BUS_RESET) == TRUE)
            return status;

        /* Stop the controller, but do not free the resources */
        if (NVMeResetAdapter(pAE) != TRUE) {
            return (FALSE);
        }
    }

    StorPortDebugPrint(INFO,
                       "NvmeAdapterControlPowerDown: returning %d\n",
                       status);
    return status;
}

/*******************************************************************************
 * NVMePowerControl
 *
 * @brief This function handles the power requests srb from the storport. This
 *        routine handles request from system to go to S3/S4 state or resume
 *        from S3/S4 state.
 * 
 * @param pAE - pointer to device extension
 * @param pPowerSrb - Power SRB pointer
 * 
 * @return BOOLEAN
 *     TRUE - command was processed successfully
 *     FALSE - If anything goes wrong
 ******************************************************************************/
BOOLEAN NVMePowerControl(
    IN PNVME_DEVICE_EXTENSION pAE,
    IN PSCSI_REQUEST_BLOCK Srb
)
{
    BOOLEAN status = FALSE;
    BOOLEAN powerActionValid = FALSE;
    NVME_PWR_ACTION nvmePwrAction = NVME_PWR_NONE;
    PSCSI_POWER_REQUEST_BLOCK pPowerSrb = (PSCSI_POWER_REQUEST_BLOCK)Srb;
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSRBEX_DATA_POWER pSrbExPower = NULL;
#endif

    ULONG PowerAction = 0;
    UCHAR SrbPowerFlags = 0;
    ULONG DevicePowerState = 0;

#if (NTDDI_VERSION > NTDDI_WIN7)
    pSrbExPower = (PSRBEX_DATA_POWER)SrbGetSrbExDataByType((PSTORAGE_REQUEST_BLOCK)Srb,
                                                            SrbExDataTypePower);
    if (pSrbExPower != NULL) {
        PowerAction = pSrbExPower->PowerAction;
        SrbPowerFlags = pSrbExPower->SrbPowerFlags;
        DevicePowerState = pSrbExPower->DevicePowerState;
    }
    else {
        PowerAction = pPowerSrb->PowerAction;
        SrbPowerFlags = pPowerSrb->SrbPowerFlags;
        DevicePowerState = pPowerSrb->DevicePowerState;
    }
#else
    PowerAction = pPowerSrb->PowerAction;
    SrbPowerFlags = pPowerSrb->SrbPowerFlags;
    DevicePowerState = pPowerSrb->DevicePowerState;
#endif

    if ((SrbPowerFlags & SRB_POWER_FLAGS_ADAPTER_REQUEST) == FALSE) {
        /* 
         * Storport should not send the device level request (i.e PathId,
         * TargetId, LUNID) but in case if it does then ignore it and return
         * success.
         */
        Srb->SrbStatus = SRB_STATUS_SUCCESS;
        return FALSE;
    }

    switch (DevicePowerState) {
        case StorPowerDeviceD0:
        case StorPowerDeviceD3:
            StorPortDebugPrint(INFO,
                               "Device Power State request %d\n",
                               DevicePowerState);
            powerActionValid = TRUE;
        break;
        case StorPowerDeviceD1:
        case StorPowerDeviceD2:
            StorPortDebugPrint(INFO,
                               "Device Power State request %d\n",
                               DevicePowerState);
            powerActionValid = FALSE;
            status = TRUE;
        break;
        case StorPowerDeviceUnspecified:
            StorPortDebugPrint(WARNING,
                               "Unsupported Device Power State received %d\n",
                               DevicePowerState);
            powerActionValid = FALSE;
        break;
        default:
            ASSERT(FALSE);
        break;
    }

    /* 
     * Based on power state validity, which we determined above, we will
     * transition into appropriate state.
     */
    if (powerActionValid == TRUE) {
        switch (PowerAction) {
            case StorPowerActionNone:
            break;
            case StorPowerActionHibernate:
            case StorPowerActionSleep:
                switch (DevicePowerState) {
                    case StorPowerDeviceD0:
                        nvmePwrAction = NVME_PWR_ADAPTER_RESUME_FROM_S3_S4;
                    break;
                    case StorPowerDeviceD3:
                        nvmePwrAction = NVME_PWR_ADAPTER_ENTER_S3_S4;

                        /* 
                         * Save the PowerAction to prevent 
                         * SCSIOP_START_STOP_UNIT to issue any cmds if we are 
                         * waking from the Hibernate and before our controller 
                         * is ready.
                         */
                        pAE->PowerAction = PowerAction;
                    break;
                    default:
                        /* nvmePwrAction already initialized to NVME_PWR_NONE */
                    break;
                } /* end DevicePowerState switch */
            break;
            case StorPowerActionShutdown:
            case StorPowerActionShutdownReset:
            case StorPowerActionShutdownOff:
            case StorPowerActionWarmEject:
                nvmePwrAction =  NVME_PWR_ADAPTER_OFF;
            break;
            case StorPowerActionReserved:
            default:
                StorPortDebugPrint(ERROR,
                                   "Unsupported Power Action requested %d\n",
                                   PowerAction);
            break;
        } /* end PowerAction switch */
    } /* end if (powerActionValid == TRUE) */

    /* Now at this point we know what power action we need to take */
    switch (nvmePwrAction) {
        case NVME_PWR_ADAPTER_OFF:
            status = NVMeNormalShutdown(pAE);
        break;
        case NVME_PWR_ADAPTER_ENTER_S3_S4:
            status = NVMeAdapterControlPowerDown(pAE);
        break;
        case NVME_PWR_ADAPTER_RESUME_FROM_S3_S4:
            status = NVMeAdapterControlPowerUp(pAE);
        break;
        case NVME_PWR_NONE:
        default:
            /* Do nothing, just complete */
            Srb->SrbStatus= SRB_STATUS_SUCCESS;
            return FALSE;
        break;
    } /* end switch */

    return status;
}

#if (NTDDI_VERSION > NTDDI_WIN7)
/*******************************************************************************
* NVMeAdapterPowerControl
*
* @brief This function handles the power requests srb from the storport. This
*        routine handles request from system to go to S3/S4 state or resume
*        from S3/S4 state.
*
* @param pAE - pointer to device extension
* @param DevicePowerState - Device Power State
* @param PowerAction - Power Action
*
* @return BOOLEAN
*     TRUE - command was processed successfully
*     FALSE - If anything goes wrong
******************************************************************************/
BOOLEAN NVMeAdapterPowerControl(
    IN PNVME_DEVICE_EXTENSION pAE,
    IN ULONG DevicePowerState,
	IN ULONG PowerAction
	)
{
    BOOLEAN status = FALSE;
    BOOLEAN powerActionValid = FALSE;
    NVME_PWR_ACTION nvmePwrAction = NVME_PWR_NONE;

    switch (DevicePowerState) {
    case StorPowerDeviceD0:
    case StorPowerDeviceD3:
        StorPortDebugPrint(INFO, "NVMePowerControl: <Info> Device Power State request %d\n", DevicePowerState);
        powerActionValid = TRUE;
        break;
    case StorPowerDeviceD1:
    case StorPowerDeviceD2:
        StorPortDebugPrint(INFO, "NVMePowerControl: <Info> Device Power State request %d\n", DevicePowerState);
        powerActionValid = FALSE;
        status = TRUE;
        break;
	case StorPowerDeviceUnspecified:
        StorPortDebugPrint(ERROR, "NVMePowerControl: <Error> Unsupported Device Power State received %d\n", DevicePowerState);
        powerActionValid = FALSE;
        break;
	default:
        ASSERT(FALSE);
        break;
	}

	/*
	* Based on power state validity, which we determined above, we will
	* transition into appropriate state.
	*/
    if (powerActionValid == TRUE) {
        switch (PowerAction) {
        case StorPowerActionNone:
            break;
        case StorPowerActionHibernate:
        case StorPowerActionSleep:
            switch (DevicePowerState) {
            case StorPowerDeviceD0:
                nvmePwrAction = NVME_PWR_ADAPTER_RESUME_FROM_S3_S4;
                break;
            case StorPowerDeviceD3:
                nvmePwrAction = NVME_PWR_ADAPTER_ENTER_S3_S4;

                /*
                * Save the PowerAction to prevent
                * SCSIOP_START_STOP_UNIT to issue any cmds if we are
                * waking from the Hibernate and before our controller
                * is ready.
                */
                pAE->PowerAction = PowerAction;
                break;
            default:
                /* nvmePwrAction already initialized to NVME_PWR_NONE */
                break;
            } /* end DevicePowerState switch */
            break;
        case StorPowerActionShutdown:
        case StorPowerActionShutdownReset:
        case StorPowerActionShutdownOff:
        case StorPowerActionWarmEject:
            nvmePwrAction = NVME_PWR_ADAPTER_OFF;
            break;
        case StorPowerActionReserved:
        default:
            StorPortDebugPrint(ERROR, "NVMePowerControl: <Error> Unsupported Device Power Action requested %d\n", PowerAction);
            break;
        } /* end PowerAction switch */
    } /* end if (powerActionValid == TRUE) */

    /* Now at this point we know what power action we need to take */
    switch (nvmePwrAction) {
    case NVME_PWR_ADAPTER_OFF:
        status = NVMeNormalShutdown(pAE);
        break;
    case NVME_PWR_ADAPTER_ENTER_S3_S4:
        StorPortPause(pAE, STOR_ALL_REQUESTS);
        status = NVMeAdapterControlPowerDown(pAE);
        break;
    case NVME_PWR_ADAPTER_RESUME_FROM_S3_S4:
        status = NVMeAdapterControlPowerUp(pAE);
        break;
    case NVME_PWR_NONE:
    default:
        return FALSE;
        break;
    } /* end switch */

    return status;
}
#endif
