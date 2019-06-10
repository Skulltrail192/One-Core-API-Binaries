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
 * File: nvmePwrMgmt.h
 */

#ifndef __NVME_PWRMGMT_H__
#define __NVME_PWRMGMT_H__

typedef enum _NVME_PWR_ACTION
{
    NVME_PWR_NONE = 0,
    NVME_PWR_ADAPTER_OFF,
    NVME_PWR_ADAPTER_ENTER_S3_S4,
    NVME_PWR_ADAPTER_RESUME_FROM_S3_S4,
    NVME_PWR_ADAPTER_MAX_POWER_ACTION
} NVME_PWR_ACTION;

BOOLEAN NVMeAdapterControlPowerUp(
    IN PNVME_DEVICE_EXTENSION pAdapterExtension
);

BOOLEAN NVMeAdapterControlPowerDown(
    IN PNVME_DEVICE_EXTENSION pAdapterExtension
);

BOOLEAN NVMePowerControl(
    IN PNVME_DEVICE_EXTENSION pAdapterExtension,
    IN PSCSI_REQUEST_BLOCK Srb
);

#if (NTDDI_VERSION > NTDDI_WIN7)
BOOLEAN NVMeAdapterPowerControl(
	IN PNVME_DEVICE_EXTENSION pAE,
	IN ULONG DevicePowerState,
	IN ULONG PowerAction
);
#endif

#endif /* __NVME_PWRMGMT_H__ */
