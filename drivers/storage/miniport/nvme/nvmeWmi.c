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
 * File: nvmeWmi.c
 */

#include "precomp.h"
#ifndef DBG
#include "nvmeWmi.tmh"
#endif
#include <scsiwmi.h>


SCSIWMIGUIDREGINFO WmiGuidList[] =                    // GUIDs supported.                       
{
    {&NVMe_QueryDevInfo_GUID, 1, 0},
    {&NVMe_Method_GUID, 1, 0}
};

enum {
    NVMe_QueryDevInfo_Idx = 0,
    NVMe_Method_Idx = 1
};

#define WmiGuidCount (sizeof(WmiGuidList) / sizeof(SCSIWMIGUIDREGINFO))


/**************************************************************************************************/
/*                                                                                                */
/* InitializeWmiContext.                                                                          */
/*                                                                                                */
/* Build WMILIB block, with pointers to supported callbacks, GUIDs, etc.                          */
/*                                                                                                */
/**************************************************************************************************/
void
InitializeWmiContext(__in PNVME_DEVICE_EXTENSION pHbaExtension)
{
    PSCSI_WMILIB_CONTEXT pWmiLibContext =             // Point to WMI context block. 
        &(pHbaExtension->WmiLibContext);

    memset(pWmiLibContext, 0, sizeof(SCSI_WMILIB_CONTEXT)); // Clear WMI context block.

    pWmiLibContext->GuidCount = WmiGuidCount;
    pWmiLibContext->GuidList = WmiGuidList;

    //
    // Point to WMI callback routines.
    //
    pWmiLibContext->QueryWmiRegInfo = QueryWmiRegInfo;
    pWmiLibContext->QueryWmiDataBlock = QueryWmiDataBlock;
    pWmiLibContext->SetWmiDataBlock = NULL;
    pWmiLibContext->SetWmiDataItem = NULL;
    pWmiLibContext->ExecuteWmiMethod = ExecuteWmiMethod;
    pWmiLibContext->WmiFunctionControl = NULL;
} /* InitializeWmiContext */


/**************************************************************************************************/
/*                                                                                                */
/* DispatchWmi.                                                                                   */
/*                                                                                                */
/* Initial handling of WMI SRB's                                                                  */
/*                                                                                                */
/**************************************************************************************************/
VOID
DispatchWmi(
__in struct _nvme_device_extension* pDevExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
__in PSTORAGE_REQUEST_BLOCK pSrb
#else
__in PSCSI_REQUEST_BLOCK pSrb
#endif
)
{
    PNVME_SRB_EXTENSION pSrbExtension = (PNVME_SRB_EXTENSION)GET_SRB_EXTENSION(pSrb);
    PSCSIWMI_REQUEST_CONTEXT pRequestContext = &(pSrbExtension->WmiReqContext);
    BOOLEAN pending = TRUE;
    BOOLEAN adapterRequest = TRUE;

#if (NTDDI_VERSION > NTDDI_WIN7)
    PSRBEX_DATA_WMI pSrbWmi = (PSRBEX_DATA_WMI)SrbGetSrbExDataByType((PSTORAGE_REQUEST_BLOCK)pSrb,
        SrbExDataTypeWmi);
#else
    PSCSI_WMI_REQUEST_BLOCK pSrbWmi = (PSCSI_WMI_REQUEST_BLOCK)pSrb;
#endif

    memset(pRequestContext, 0, sizeof(SCSIWMI_REQUEST_CONTEXT)); // Clear WMI context block.

    //
    // Save the pointer to the SRB in UserContext
    // of SCSIWMI_REQUEST_CONTEXT
    //
    pRequestContext->UserContext = pSrb;

    //
    // Check if the WMI SRB is targetted for
    // the iScsi adapter or one of the devices
    //
    adapterRequest = (pSrbWmi->WMIFlags & SRB_WMI_FLAGS_ADAPTER_REQUEST) == SRB_WMI_FLAGS_ADAPTER_REQUEST;

    if (adapterRequest) {
        //
        // Process the incoming WMI request.
        //
        StorPortDebugPrint(INFO, 
            "Entering DispatchWmi. WMISubFunction: 0x%Ix  WMI Flags: 0x%Ix\n", 
            pSrbWmi->WMISubFunction, 
            pSrbWmi->WMIFlags);

        pending = ScsiPortWmiDispatchFunction(
            &pDevExtension->WmiLibContext,
            pSrbWmi->WMISubFunction,
            pDevExtension,
            pRequestContext,
            pSrbWmi->DataPath,
            GET_DATA_LENGTH(pSrb),
            GET_DATA_BUFFER(pSrb));
    } else {
        SET_DATA_LENGTH(pSrb, 0);
        pSrb->SrbStatus = SRB_STATUS_ERROR;

        StorPortDebugPrint(INFO, "Error: Wmi call not targeted at adapter. WMIFlags: 0x%Ix\n", pSrbWmi->WMIFlags);
        return;
    }

    if (pending) {
        SET_DATA_LENGTH(pSrb, 0);
        pSrb->SrbStatus = SRB_STATUS_ERROR;

        StorPortDebugPrint(INFO, "Error: Wmi cannot pend requests\n");
    } else {
        pSrb->SrbStatus = ScsiPortWmiGetReturnStatus(pRequestContext);
        SET_DATA_LENGTH(pSrb, ScsiPortWmiGetReturnSize(pRequestContext));
    }

    return;
} /* DispatchWmi */


/**************************************************************************************************/
/*                                                                                                */
/* QueryWmiRegInfo.                                                                               */
/*                                                                                                */
/* Answers query for WMI registration information.                                                */
/*                                                                                                */
/**************************************************************************************************/
UCHAR
QueryWmiRegInfo(
__in PVOID pContext,
__in PSCSIWMI_REQUEST_CONTEXT pRequestContext,
__out PWSTR *pMofResourceName
)
{
    UNREFERENCED_PARAMETER(pContext);
    UNREFERENCED_PARAMETER(pRequestContext);

    *pMofResourceName = L"MofResource";

    return SRB_STATUS_SUCCESS;
} /* QueryWmiRegInfo */

/**************************************************************************************************/
/*                                                                                                */
/* QueryWmiDataBlock.                                                                             */
/*                                                                                                */
/* Answers query for WMI data block.                                                              */
/*                                                                                                */
/**************************************************************************************************/
BOOLEAN
QueryWmiDataBlock(
__in PVOID pContext,
__in PSCSIWMI_REQUEST_CONTEXT pDispatchContext,
__in ULONG GuidIndex,
__in ULONG InstanceIndex,
__in ULONG InstanceCount,
__inout PULONG pInstanceLenArr,
__in ULONG BufferAvail,
__out PUCHAR pBuffer
)
{
    PNVME_DEVICE_EXTENSION pDevExtension = (PNVME_DEVICE_EXTENSION)pContext;
    PSCSI_WMI_REQUEST_BLOCK pSrb = (PSCSI_WMI_REQUEST_BLOCK)pDispatchContext->UserContext;
    ULONG                   sizeNeeded = 0;
    UCHAR                   status = SRB_STATUS_SUCCESS;
    if (BufferAvail > 0) {
        memset(pBuffer, 0, BufferAvail);
    }

    switch (GuidIndex) {
    case NVMe_QueryDevInfo_Idx: {
        PNVMe_QueryDevInfo pQueryDevInfo;
        sizeNeeded = NVMe_QueryDevInfo_SIZE;

        if (BufferAvail >= sizeNeeded){
            *pInstanceLenArr = sizeNeeded;
            pQueryDevInfo = (PNVMe_QueryDevInfo)pBuffer;
            pQueryDevInfo->maxDataXferSize = pDevExtension->InitInfo.MaxTxSize;
            pQueryDevInfo->numberOfNamespaces = pDevExtension->controllerIdentifyData.NN;
            pQueryDevInfo->Data.Id = 0x01;
            pQueryDevInfo->Data.u64 = 0x02;
            status = SRB_STATUS_SUCCESS;
        } else {
            status = SRB_STATUS_DATA_OVERRUN;
        }
    }
    break;

    case NVMe_Method_Idx:
        //
        // Even though this class only has methods, we need to
        // respond to any queries for it since WMI expects that
        // there is an actual instance of the class on which to
        // execute the method
        //
        sizeNeeded = sizeof(ULONG);

        if (BufferAvail < sizeNeeded) {
            status = SRB_STATUS_DATA_OVERRUN;
            break;
        } else{
            *pInstanceLenArr = sizeNeeded;
            status = SRB_STATUS_SUCCESS;
        }
    break;

    default:                                      // Unsupported GUID.

        status = SRB_STATUS_ERROR;                // Follow practice in iSCSI client.
    break;
    }                                                 // End switch(GuidIndex).

    SpUpdateWmiRequest(pDevExtension, pSrb, pDispatchContext, status, sizeNeeded);

    return status;
} /* QueryWmiDataBlock */


/**************************************************************************************************/
/*                                                                                                */
/* ExecuteWmiMethod.                                                                              */
/*                                                                                                */
/* Executes a WMI method.                                                                         */
/*                                                                                                */
/* Note: The support for MSFC GUIDs is only illustrative, showing how to produce plausible        */
/*       results.                                                                                 */
/*                                                                                                */
/**************************************************************************************************/
UCHAR
ExecuteWmiMethod(
__in PVOID pContext,
__in PSCSIWMI_REQUEST_CONTEXT pDispatchContext,
__in ULONG GuidIndex,
__in ULONG InstanceIndex,
__in ULONG MethodId,
__in ULONG InBufferSize,
__in ULONG OutBufferSize,
__inout PUCHAR pBuffer
)
{
    PNVME_DEVICE_EXTENSION             pDevExtension = (PNVME_DEVICE_EXTENSION)pContext;
    PSCSI_WMI_REQUEST_BLOCK pSrb = (PSCSI_WMI_REQUEST_BLOCK)pDispatchContext->UserContext;
    ULONG                   sizeNeeded = 0;
    UCHAR                   status = SRB_STATUS_SUCCESS;
    UINT8 strSize = 0;

    UNREFERENCED_PARAMETER(InstanceIndex);

    switch (GuidIndex) {
    case NVMe_Method_Idx:
    {
        switch (MethodId) {
        case GetControllerInfo: {
            PGetControllerInfo_OUT pControllerOut = NULL;
            sizeNeeded = GetControllerInfo_OUT_SIZE;

            if (OutBufferSize < sizeNeeded) {
                status = SRB_STATUS_DATA_OVERRUN;
                break;
            } else{
                pControllerOut = (PGetControllerInfo_OUT)pBuffer;
                pControllerOut->pciVendorId = pDevExtension->controllerIdentifyData.VID;
                pControllerOut->pciSsVendId = pDevExtension->controllerIdentifyData.SSVID;
                status = SRB_STATUS_SUCCESS;
            }
        }
            break;

        case GetNameSpaceInfo: {
            PGetNameSpaceInfo_IN  pGetNsInfoIn;
            PGetNameSpaceInfo_OUT pGetNsInfoOut;
            UINT32 lunId = 0;

            if (InBufferSize < GetNameSpaceInfo_IN_SIZE) {
                status = SRB_STATUS_INVALID_REQUEST;
                break;
            }

            pGetNsInfoIn = (PGetNameSpaceInfo_IN)pBuffer;
            lunId = pGetNsInfoIn->lunId;

            if (lunId >= pDevExtension->controllerIdentifyData.NN) {
                status = SRB_STATUS_INVALID_REQUEST;
                break;
            }

            sizeNeeded = GetNameSpaceInfo_OUT_SIZE;

            if (OutBufferSize < sizeNeeded) {
                status = SRB_STATUS_DATA_OVERRUN;
                break;
            }
            pGetNsInfoOut = (PGetNameSpaceInfo_OUT)pBuffer;

            pGetNsInfoOut->nSize = pDevExtension->pLunExtensionTable[lunId]->identifyData.NSZE;
            pGetNsInfoOut->nCap = pDevExtension->pLunExtensionTable[lunId]->identifyData.NCAP;
            status = SRB_STATUS_SUCCESS;
        }
            break;

        default:
            status = SRB_STATUS_INVALID_REQUEST;
            break;
        }
    }
        break;


    default:                                     // Unsupported GUID.
        status = SRB_STATUS_INVALID_REQUEST;
    break;
    }                                                 // End switch(GuidIndex).

    SpUpdateWmiRequest(pDevExtension, pSrb, pDispatchContext, status, sizeNeeded);

    return status;
} /* ExecuteWmiMethod */

/**************************************************************************************************/
/*                                                                                                */
/**************************************************************************************************/
VOID
SpUpdateWmiRequest(
__in struct _nvme_device_extension* pHbaExtension,
// __in PNVME_DEVICE_EXTENSION   pHbaExtension,
__in PSCSI_WMI_REQUEST_BLOCK  pSrb,
__in PSCSIWMI_REQUEST_CONTEXT pDispatchContext,
__in UCHAR                    Status,
__in ULONG                    SizeNeeded
)
{
    UNREFERENCED_PARAMETER(pHbaExtension);

    // Update the request if the status is NOT pending or NOT already completed within the callback.

    if (SRB_STATUS_PENDING != Status) {
        //
        // Request completed successfully or there was an error.
        //
        ScsiPortWmiPostProcess(pDispatchContext, Status, SizeNeeded);

        pSrb->SrbStatus = ScsiPortWmiGetReturnStatus(pDispatchContext);
        pSrb->DataTransferLength = ScsiPortWmiGetReturnSize(pDispatchContext);
    }
} /* SpUpdateWmiRequest */
