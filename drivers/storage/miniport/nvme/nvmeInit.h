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
 * File: nvmeInit.h
 */

#ifndef __NVME_INIT_H__
#define __NVME_INIT_H__

/*******************************************************************************
 * Command Entry/Information data structure.
 ******************************************************************************/
typedef struct _CMD_INFO
{
    /* Command ID for the used Submission Queue entry */
    USHORT CmdID;

    /* Dedicated PRPList(in virtual address) of the cmd entry */
    PVOID  pPRPList;
} CMD_INFO, *PCMD_INFO;

typedef struct _CMD_ENTRY
{
    /* The doubly-linked list entry */
    LIST_ENTRY ListEntry;

    /* Non-zero value means it’s been acquired, zero means a free entry */
    ULONG Pending;

    /*
     * Any context callers wish to save,
     * e.g., the original SRB associated with the request
     */
    ULONG_PTR Context;

    /*
     * Command Information structure returned to callers
     * when a command ID is successfully acquired
     */
    CMD_INFO CmdInfo;
} CMD_ENTRY, *PCMD_ENTRY;

/*******************************************************************************
 * Completion Queue Entry/Information data structure.
 ******************************************************************************/
typedef struct _CPL_INFO
{
    /* Completion Queue ID for the used Completion Queue entry */
    ULONG CplID;

    /* Dedicated PRPList(in virtual address) of the cmd entry */
    PVOID pPRPList;
} CPL_INFO, *PCPL_INFO;

typedef struct _CPL_ENTRY
{
    /* The doubly-linked list entry */
    LIST_ENTRY ListEntry;

    /* Non-zero value means it’s been acquired, zero means a free entry */
    ULONG Pending;

    /*
     * Any context callers wish to save,
     * e.g., the original SRB associated with the request
     */
    ULONG_PTR Context;

    /*
     * Command Information structure returned to callers
     * when a command ID is successfully acquired
     */
    CPL_INFO CplInfo;
} CPL_ENTRY, *PCPL_ENTRY;

/*******************************************************************************
 * Submission Queue Entry/Information data structure.
 ******************************************************************************/
typedef struct _SUB_INFO
{
    /* Submission Queue ID for the used Submission Queue entry */
    ULONG SubID;

    /* Dedicated PRPList (in virtual address) of the cmd entry */
    PVOID pPRPList;
} SUB_INFO, *PSUB_INFO;

typedef struct _SUB_ENTRY
{
    /* The doubly-linked list entry */
    LIST_ENTRY ListEntry;

    /* Non-zero value means it’s been acquired, zero means a free entry */
    ULONG Pending;

    /*
     * Any context callers wish to save,
     * e.g., the original SRB associated with the request
     */
    ULONG_PTR Context;

    /*
     * Command Information structure returned to callers
     * when a command ID is successfully acquired
     */
    CMD_INFO CmdInfo;
} SUB_ENTRY, *PSUB_ENTRY;

/* FUNCTION PROTOTYPES */

BOOLEAN NVMeStrCompare(
    PCSTR pTargetString,
    PCSTR pArgumentString
);

BOOLEAN NVMeEnumNumaCores(
    PNVME_DEVICE_EXTENSION pAE
);

PVOID NVMeAllocateMem(
    PNVME_DEVICE_EXTENSION pAE,
    ULONG Size,
    ULONG Node
);

BOOLEAN NVMeEnumMsiMessages(
    PNVME_DEVICE_EXTENSION pAE
);

ULONG NVMeAllocQueues(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in USHORT QueueID,
    __in ULONG QEntries,
    __in USHORT NumaNode
);

ULONG NVMeInitSubQueue(
    PNVME_DEVICE_EXTENSION pAE,
    USHORT QueueID
);

VOID NVMeInitFreeQ(
    PSUB_QUEUE_INFO pSQI,
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeAppQueueEntry(
    PLIST_ENTRY pFreeQ,
    PLIST_ENTRY pEntry
);

ULONG NVMeInitCplQueue(
    PNVME_DEVICE_EXTENSION pAE,
    USHORT QueueID
);

BOOLEAN NVMeEnableAdapter(
    PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeGetIdentifyStructures(
    PNVME_DEVICE_EXTENSION pAE,
    ULONG NamespaceID,
    USHORT CNS
)

BOOLEAN NVMeAllocQueueFromAdapter(
    PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeCreateCplQueues(
    PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeCreateSubQueues(
    PNVME_DEVICE_EXTENSION pAE
);

STOR_PHYSICAL_ADDRESS NVMeGetPhysAddr(
    PNVME_DEVICE_EXTENSION pAE,
    PVOID pVirtAddr
);

BOOLEAN NVMeAllocIoQueues(
    PNVME_DEVICE_EXTENSION pAE
);

ULONG NVMeGetCmdEntry(
    PNVME_DEVICE_EXTENSION pAE,
    USHORT QueueID,
    ULONG_PTR Context,
    PVOID pCmdInfo
);

ULONG NVMeIssueCmd(
    PNVME_DEVICE_EXTENSION pAE,
    USHORT QueueID,
    USHORT CmdID,
    PVOID pTempSubEntry
);

PCMD_ENTRY NVMeAcqQueueEntry(
    PNVME_DEVICE_EXTENSION pAE,
    PLIST_ENTRY pFreeQ
);

ULONG NVMeGetCplEntry(
    PNVME_DEVICE_EXTENSION pAE,
    USHORT QueueID,
    PVOID pCplEntry
);

VOID NVMeMaskInterrupts(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeUnmaskInterrupts(
    PNVME_DEVICE_EXTENSION pAE
);

#endif /* __NVME_INIT_H__ */
