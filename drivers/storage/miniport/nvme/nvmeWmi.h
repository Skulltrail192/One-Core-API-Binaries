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
 * File: nvmeWmi.h
 */

#ifndef __NVME_WMI_H__
#define __NVME_WMI_H__

VOID
DispatchWmi(
__in PNVME_DEVICE_EXTENSION   pHbaExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
IN PSTORAGE_REQUEST_BLOCK Srb
#else
IN PSCSI_REQUEST_BLOCK Srb
#endif
);

VOID
SpUpdateWmiRequest(
__in PNVME_DEVICE_EXTENSION   pHbaExtension,
__in PSCSI_WMI_REQUEST_BLOCK  pSrb,
__in PSCSIWMI_REQUEST_CONTEXT pDispatchContext,
__in UCHAR                    Status,
__in ULONG                    SizeNeeded
                  );

VOID
InitializeWmiContext(__in PNVME_DEVICE_EXTENSION);

BOOLEAN
HandleWmiSrb(
    __in       PNVME_DEVICE_EXTENSION,
    __in __out PSCSI_WMI_REQUEST_BLOCK
    );
UCHAR
QueryWmiRegInfo(
__in PVOID pContext,
__in PSCSIWMI_REQUEST_CONTEXT pRequestContext,
__out PWSTR *pMofResourceName
               );

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
                 );


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
                );

#endif /* __NVME_WMI_H__ */