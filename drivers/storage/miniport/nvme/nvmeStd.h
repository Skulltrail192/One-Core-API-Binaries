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
 * File: nvmeStd.h
 */

#ifndef __NVME_STD_H__
#define __NVME_STD_H__

#include <evntrace.h>


#ifdef DUMB_DRIVER
#define DUMB_DRIVER_SZ (1024 * 64)
#endif

#define STOR_ALL_REQUESTS (0xFFFFFFFF)


#define MAX_REGISTERED_CONTROLLERS 128
#define MAX_PCI_BAR                 8
#define NUM_ENTRY_FOR_PAGE_ALIGN    64
#define MAX_NUM_PHYSICAL_BREAKS     32
#define ASYNCH_EVENT_TYPE_MASK      0x7
#define LOG_PAGE_NUM_DWORDS_MASK    0x0FFF0000
#define MAX_MSIX_MSG_PER_DEVICE     256
#define BUFFER_ALIGNMENT_MASK       3
#define MIN_WAIT_TIMEOUT            500 /* One unit (500 milliseonds) */
#define REGISTRY_KEY_NOT_FOUND      0xFFFFFFFF
#define PAGE_SIZE_IN_4KB            0x1000
#define PAGE_SIZE_IN_DWORDS         PAGE_SIZE_IN_4KB / 4
#define RESOURCE_SHARED             0xFFFF
#define DFT_ASYNC_EVENT_REQ_NUMBER  4
#define NVME_ADMIN_MSG_ID           0
#define IDENTIFY_LIST_SIZE          4096

#define IDEN_CONTROLLER             0
#define IDEN_NAMESPACE              1

/* CNS defines for NVMe 1.0 and 1.2 */
#define IDENTIFY_NAMESPACE			0x00
#define IDENTIFY_CNTLR				0x01
/* CNS defines for NVMe 1.2 */
#define LIST_ATTACHED_NAMESPACES	0x02
#define LIST_EXISTING_NAMESPACES	0x10
#define IDEN_NS_FAIL_IF_INVALID		0x11
#define LIST_CNTLRS_ATTACHED_TO_NS	0x12
#define LIST_ALL_CNTLRS				0x13
#define DUMP_POLL_CALLS             3
#define STORPORT_TIMER_CB_us        5000 /* .005 seconds */
#define MAX_STATE_STALL_us          STORPORT_TIMER_CB_us
#define MILLI_TO_MICRO              1000
#define MICRO_TO_NANO               1000
#define MSI_ADDR_RH_DM_MASK         0xC
#define MICRO_TO_SEC                1000

/* Default, minimum, maximum values of Registry keys */
#define DFT_NAMESPACES              16
#define MIN_NAMESPACES              1
#define MAX_NAMESPACES              128
#define MAX_TTL_NAMESPACES          MAX_NAMESPACES // to account for some hidden NS

#ifdef DUMB_DRIVER
#define MIN_TX_SIZE                 DUMB_DRIVER_SZ
#define DFT_TX_SIZE                 MIN_TX_SIZE
#define MAX_TX_SIZE                 MIN_TX_SIZE
#else
#define DFT_TX_SIZE                 (128*1024)
#define MIN_TX_SIZE                 (4*1024)
#define MAX_TX_SIZE                 (1024*1024)
#endif

#ifdef DUMB_DRIVER
#define DFT_AD_QUEUE_ENTRIES        16
#define MIN_AD_QUEUE_ENTRIES        DFT_AD_QUEUE_ENTRIES
#define MAX_AD_QUEUE_ENTRIES        DFT_AD_QUEUE_ENTRIES
#else
#define DFT_AD_QUEUE_ENTRIES        256
#define MIN_AD_QUEUE_ENTRIES        2
#define MAX_AD_QUEUE_ENTRIES        4096
#endif

#ifdef DUMB_DRIVER
#define DFT_IO_QUEUE_ENTRIES        256
#define MIN_IO_QUEUE_ENTRIES        DFT_IO_QUEUE_ENTRIES
#define MAX_IO_QUEUE_ENTRIES        DFT_IO_QUEUE_ENTRIES
#else
#define DFT_IO_QUEUE_ENTRIES        1024
#define MIN_IO_QUEUE_ENTRIES        2
#define MAX_IO_QUEUE_ENTRIES        4096
#endif

#define DUMP_BUFFER_SIZE            ((5*64*1024) + (sizeof(NVME_LUN_EXTENSION)*MAX_NAMESPACES))

#define DFT_INT_COALESCING_TIME     80
#define MIN_INT_COALESCING_TIME     0
#define MAX_INT_COALESCING_TIME     255

#define DFT_INT_COALESCING_ENTRY    16
#define MIN_INT_COALESCING_ENTRY    0
#define MAX_INT_COALESCING_ENTRY    255

#define MASK_INT                    0xFFFFFFFF
#define CLEAR_INT                   0
#define MODE_SNS_MAX_BUF_SIZE       256

/* IOCTL return codes */
#define IOCTL_COMPLETED  TRUE
#define IOCTL_PENDING    FALSE

/* Surprise Removal*/
#define START_SURPRISE_REMOVAL_TIMER	1000000	//1 Second
#define STOP_SURPRISE_REMOVAL_TIMER		0
#define INVALID_DEVICE_REGISTER_VALUE	0xFFFFFFFF

/* Helper Macros */

/* Get field values from specified value */
#define GET_WORD_0(Value)  (Value & 0xFFFF)
#define GET_WORD_1(Value)  ((Value & 0xFFFF0000) >> 16)
#define GET_WORD_2(Value)  ((Value & 0xFFFF00000000) >> 32)
#define GET_WORD_3(Value)  ((Value & 0xFFFF000000000000) >> 48)
#define GET_DWORD_0(Value) (Value & 0xFFFFFFFF)
#define GET_DWORD_1(Value) ((Value & 0xFFFFFFFF00000000) >> 32)

/* Get Destination field embedded in the specified MSI message address */
#define GET_DESTINATION_FIELD(Address)  ((Address & 0xFF000) >> 12)

/* Validate the read back values from Registry */
#define RANGE_CHK(Val, Min, Max) \
    (((Val >= Min) && (Val <= Max)) ? TRUE : FALSE)

/* Align buffer pointer to next system page boundary */
#define PAGE_ALIGN_BUF_PTR(pBuf)                           \
    ((((ULONG_PTR)((PUCHAR)pBuf)) & (PAGE_SIZE-1)) == 0) ? \
    (PVOID)(pBuf) :                                        \
    (PVOID)((((ULONG_PTR)((PUCHAR)pBuf)) + PAGE_SIZE) & ~(PAGE_SIZE-1))

/* Align buffer address to next system page boundary */
#define PAGE_ALIGN_BUF_ADDR(Address) \
    ((Address + PAGE_SIZE) & ~(PAGE_SIZE-1))

/* Exam if the address is aligned at system memory boundary */
#define IS_SYS_PAGE_ALIGNED(Address) \
    (((Address & (PAGE_SIZE-1)) == 0) ? TRUE : FALSE)

#ifdef DUMB_DRIVER
#define DATA_OUT_BIT (1 << 0)
#define DATA_IN_BIT (1 << 1)
#define IS_CMD_DATA_IN(cmd)  \
    (((cmd & DATA_IN_BIT) == DATA_IN_BIT) ? TRUE : FALSE)
#define IS_CMD_DATA_OUT(cmd) \
    (((cmd & DATA_OUT_BIT) == DATA_OUT_BIT) ? TRUE : FALSE)
#endif

#ifdef HISTORY
#define HISTORY_DEPTH (100)

typedef enum _HISTORY_TAG
{
    NO_ENTRY = 0, // always first in the enum
    GETCMD_RETURN_BUSY,
    PRE_ISSUE,
    ISSUE_RETURN_BUSY,
    ISSUE,
    COMPPLETE_CMD,
    SRB_RESET_LUN,
    SRB_RESET_BUS,
    SRB_RESET_DEVICE,
    DPC_RESET,
    DETECTED_PENDING_CMD,
    COMPLETE_CMD_RESET,
    HISTORY_MARKER = 255 // always last in the enum
} HISTORY_TAG;

typedef struct _HISTORY_SUBMIT
{
    HISTORY_TAG tag;
    ULONG queueId;
    ULONG NSID;
    NVMe_COMMAND_DWORD_0 CDW0;
    ULONGLONG parm1;
    ULONGLONG parm2;
    ULONGLONG parm3;
} HISTORY_SUBMIT;
HISTORY_SUBMIT SubmitHistory[HISTORY_DEPTH];

typedef struct _HISTORY_COMPLETE
{
    HISTORY_TAG tag;
    ULONG queueId;
    ULONG CID;
    ULONG SQHD;
    NVMe_COMPLETION_QUEUE_ENTRY_DWORD_3 DW3;
    ULONGLONG parm2;
    ULONGLONG parm3;
} HISTORY_COMPLETE;
HISTORY_COMPLETE CompleteHistory[HISTORY_DEPTH];

typedef struct _HISTORY_EVENT
{
    HISTORY_TAG tag;
    ULONG queueId;
    ULONG CID;
    ULONG parm0;
    ULONGLONG parm1;
    ULONGLONG parm2;
    ULONGLONG parm3;
} HISTORY_EVENT;
HISTORY_EVENT EventHistory[HISTORY_DEPTH];

ULONG SubmitIndex;
ULONG CompleteIndex;
ULONG EventIndex;

void TracePathSubmit(HISTORY_TAG tag, ULONG queueId, ULONG NSID,
        NVMe_COMMAND_DWORD_0 CDW0, ULONGLONG parm1, ULONGLONG parm2, ULONGLONG parm3);

void TracePathComplete(HISTORY_TAG tag, ULONG queueId, ULONG CID,
        ULONG SQHD, NVMe_COMPLETION_QUEUE_ENTRY_DWORD_3 DW3, ULONGLONG parm2, ULONGLONG parm3);

void TraceEvent(HISTORY_TAG tag, ULONG queueId, ULONG CID,
        ULONG parm0, ULONGLONG parm1, ULONGLONG parm2, ULONGLONG parm3);

#endif


/* Callback function prototype for internal request completions */
typedef BOOLEAN (*PNVME_COMPLETION_ROUTINE) (PVOID param1, PVOID param2);

/* Debugging Printing Levels */
enum
{
    INFO = TRACE_LEVEL_INFORMATION,
    WARNING = TRACE_LEVEL_WARNING,
    ERROR = TRACE_LEVEL_ERROR,
    TRACE = TRACE_LEVEL_VERBOSE
};



/*
 * PCI Access Ranges, need at least one PCI BAR memory for Controller Registers
 */
enum
{
    NVME_CTL_BAR = 0,
    NVME_ACCESS_RANGES
};

/* Enabled Interrupt Type */
typedef enum _INT_TYPE
{
    INT_TYPE_NONE = 0,
    INT_TYPE_INTX = 1,
    INT_TYPE_MSI = 2,
    INT_TYPE_MSIX = 4
} INT_TYPE;

/* Queue Type */
typedef enum _NVME_QUEUE_TYPE
{
    NVME_QUEUE_TYPE_ADMIN = 0,
    NVME_QUEUE_TYPE_IO = 1
} NVME_QUEUE_TYPE;

#define NO_SQ_HEAD_CHANGE (-1)

/* Used when decoding the LBA Range tpye after get features on LBA type */
typedef enum _NS_VISBILITY
{
    VISIBLE = 0,
    HIDDEN,
    IGNORED
} NS_VISBILITY;

/* Status of namespace.
 * invalid namespace = free LUN
 * inactive namespace = free LUN
 * Attached namespace = online LUN
 */
typedef enum _NS_STATUS
{
    INVALID = 0,    //Namespace ID does not exist (not known to controller).
    INACTIVE,       //Namespace is created, but not attached to controller.
    ATTACHED        //Namespace is created and attached to controller.
} NS_STATUS;

/*
 * Adapter Error Status; can occur during init state machine
 * or otherwise as the init state machine mechanism is used
 * to effectively halt the controller at any time by changing
 * the state to shutdown and flagging the error with this enum
 *
 * Errors in the state machine start with START_STATE_ otherweise
 * they should start with FATAL_
 *
 * Each bit is used for error situation indicators
 */

enum
{
    START_STATE_RDY_FAILURE = 0,
    START_STATE_INT_COALESCING_FAILURE,
    START_STATE_QUEUE_ALLOC_FAILURE,
    START_STATE_QUEUE_INIT_FAILURE,
    START_STATE_SET_FEATURE_FAILURE,
    START_STATE_LBA_RANGE_CHK_FAILURE,
    START_STATE_IDENTIFY_CTRL_FAILURE,
    START_STATE_LIST_ATTACHED_NS_FAILURE,
    START_STATE_LIST_EXISTING_NS_FAILURE,
    START_STATE_IDENTIFY_NS_FAILURE,
    START_STATE_SUBQ_CREATE_FAILURE,
    START_STATE_CPLQ_CREATE_FAILURE,
    FATAL_SUBQ_DELETE_FAILURE,
    FATAL_CPLQ_DELETE_FAILURE,
    START_STATE_LEARNING_FAILURE,
    START_STATE_AER_FAILURE,
    FATAL_POLLED_ADMIN_CMD_FAILURE,
    START_STATE_UNKNOWN_STATE_FAILURE,
    START_MAX_XFER_MISMATCH_FAILURE,
    START_STATE_TIMEOUT_FAILURE = 31
};

/* Start State Machine states */
enum
{
    NVMeWaitOnRDY = 0x20,
    NVMeWaitOnIdentifyCtrl,
    NVMeWaitOnListAttachedNs,
    NVMeWaitOnListExistingNs,
    NVMeWaitOnIdentifyNS,
    NVMeWaitOnSetFeatures,
    NVMeWaitOnSetupQueues,
    NVMeWaitOnAER,
    NVMeWaitOnIoCQ,
    NVMeWaitOnIoSQ,
    NVMeWaitOnLearnMapping,
    NVMeWaitOnReSetupQueues,
    NVMeWaitOnNamespaceReady,
    NVMeStartComplete = 0x88,
    NVMeShutdown,
    NVMeStateFailed = 0xFF
};

#define ALL_NAMESPACES_APPLIED 0xFFFFFFFF
#define INVALID_LUN_EXTN 0xFFFFFFFF

/* Format NVM State Machine states */
enum
{
    FORMAT_NVM_NO_ACTIVITY = 0,
    FORMAT_NVM_RECEIVED = 0x90,
    FORMAT_NVM_NS_REMOVED,
    FORMAT_NVM_CMD_ISSUED,
    FORMAT_NVM_IDEN_CONTROLLER_ISSUED,
    FORMAT_NVM_IDEN_NAMESPACE_FETCHED
};

/*******************************************************************************
 * Format NVM command specific structure.
 ******************************************************************************/
typedef struct _FORMAT_NVM_INFO
{
    /* Current state after receiving Format NVM request */
    ULONG               State;

    /* LunId corresponding to the visible namespace that is being formatted */
    ULONG               TargetLun;

    /* NSID of the next NS to fetch identify structure for */
    ULONG               NextNs;

    /* LunId corresponding to NextNs */
    ULONG               NextLun;

    /* Indicates if we're formatting one namespace or all namespaces */
    BOOLEAN             FormatAllNamespaces;

    /*
     * Flag to indicate calling StorPortNotification with BusChangeDetected
     * is required to add back the formatted namespace(s)
     */
    BOOLEAN             AddNamespaceNeeded;
} FORMAT_NVM_INFO, *PFORMAT_NVM_INFO;

#define LBA_TYPE_FILESYSTEM 1

/*******************************************************************************
 * Start State Machine structure.
 ******************************************************************************/
typedef struct _START_STATE
{
    /* Starting status, including error status */
    ULONG64 DriverErrorStatus;

    /* Interval in us for Storport to check back */
    ULONG CheckbackInterval;

    /* Next state to proceed */
    UCHAR NextDriverState;

    /* Overall state machine timeout counter */
    UINT32 TimeoutCounter;

    /* State Polling counter */
    ULONG StateChkCount;

    /* Dedicated SRB extension for command issues */
    PVOID pSrbExt;

    /* Used for data transfer during start state */
    PVOID pDataBuffer;

    /* If this is because of a reset */
    BOOLEAN resetDriven;

    /* If its a host driven reset we need the SRB */
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pResetSrb;
#else
    PSCSI_REQUEST_BLOCK pResetSrb;
#endif

    /*
     * After adapter had completed the Identify commands,
     * the callback function is invoked to examine the completion results.
     * If no error:
     * For Namespace structures, IdentifyNamespaceFetched is increased by 1.
     * START_STATE_IDENTIFY_CTRL/NS_FAILURE bit of DriverErrorStatus is set to 1
     * in the case of errors
     */
    ULONG IdentifyNamespaceFetched;

    /* Indicates all the NameSpace is ready when TRUE */
    BOOLEAN AllNamespacesAreReady;

    /* Indicates the NameSpace is ready when TRUE */
    ULONG NextNamespaceToCheckForReady;

    /* Indicates the Interrupt Coalescing configured when TRUE */
    BOOLEAN InterruptCoalescingSet;

    /*
     * Indicates Set Featurs commands is required to configure the current
     * Namespace when it's LBA Range Type is 00b and NLB matches the size of
     * the Namespace.
     */
    BOOLEAN ConfigLbaRangeNeeded;

    /*
     * For each existing Namespace, driver needs to exam its LBA Range
     * information to determine:
     *
     *   1. if it's right type to support?
     *   2. if it's a ready-only drive when supported?
     *   3. if it needs to be reported to host when supported?
     */
    ULONG TtlLbaRangeExamined;

    /* Number of Asynchronous Event Requests commands issued */
    UCHAR NumAERsIssued;

    /* Number of exposed namespaces found at init */
    ULONG VisibleNamespacesExamined;

    /* The NSID of the current device for the init state */
    ULONG CurrentNsid;

    /* Number of namespaces known to driver */
    ULONG NumKnownNamespaces;
} START_STATE, *PSTART_STATE;

/*******************************************************************************
 * Registry Initial Information data structure.
 ******************************************************************************/
typedef struct _INIT_INFO
{
    /* Supported number of namespaces */
    ULONG Namespaces;

    /* Max transfer size via one cmd entry, 128 KB by default */
    ULONG MaxTxSize;

    /* Supported number of entries for Admin queue */
    ULONG AdQEntries;

    /* Supported number of entries for IO queues */
    ULONG IoQEntries;

    /* Aggregation time in 100 us, 0 means disabled */
    ULONG IntCoalescingTime;

    /* Aggregation entries per interrupt vector */
    ULONG IntCoalescingEntry;

} INIT_INFO, *PINIT_INFO;

/*******************************************************************************
 * Command Entry/Information data structure.
 ******************************************************************************/
typedef struct _CMD_INFO
{
    /* Command ID for the used Submission Queue entry */
    ULONG CmdID;

    /* Dedicated PRPList(in virtual address) of the cmd entry */
    PVOID pPRPList;

    STOR_PHYSICAL_ADDRESS prpListPhyAddr;

#ifdef DUMB_DRIVER
    /* this cmd's dbl buffer physical address */
    STOR_PHYSICAL_ADDRESS dblPhy;

    /* this cmd's dbl buffer virtual address */
    PVOID pDblVir;

    STOR_PHYSICAL_ADDRESS dblPrpListPhy; // this commands PRP list
    PVOID pDblPrpListVir; // this commands PRP list
#endif
} CMD_INFO, *PCMD_INFO;

typedef struct _CMD_ENTRY
{
    /* The doubly-linked list entry */
    LIST_ENTRY ListEntry;

    /* TRUE means it’s been acquired, FALSE means a free entry */
    BOOLEAN Pending;

    /*
     * Any context callers wish to save,
     *
     * e.g., the original SRB associated with the request
     */
    PVOID Context;

    /*
     * Command Information structure returned to callers when a command ID is
     * successfully acquired
     */
    CMD_INFO CmdInfo;
} CMD_ENTRY, *PCMD_ENTRY;

/*******************************************************************************
 * Submission Queue Information data structure.
 ******************************************************************************/
typedef struct _SUB_QUEUE_INFO
{
    /* Allocated buffers via StorPortPatchAllocateContiguousMemorySpecifyCacheNode */

    /* Starting virtual addr of the allocated buffer for queue entries */
    PVOID pQueueAlloc;

    /* Byte size of the allocated buffer for queue entries */
    ULONG QueueAllocSize;

    /* Starting virtual addr of the allocated buffer for PRP Lists */
    PVOID pPRPListAlloc;

    /* Byte size of the allocated buffer for PRP Lists */
    ULONG PRPListAllocSize;

    /* Submission Queue */

    /* Submission queue ID, 0 based. Admin queue ID is 0 */
    USHORT SubQueueID;

    /* Reported number of queue entries when creating the queue */
    USHORT SubQEntries;

    /* Current number of free entries in the submission queue */
    USHORT FreeSubQEntries;

    /* Current tail pointer to submission queue */
    USHORT SubQTailPtr;

    /* Current head pointer to submission queue fetched from cpl queue entry */
    USHORT SubQHeadPtr;

    /* Associated doorbell register to ring for submissions */
    PULONG pSubTDBL;

    /* The associated completion queue ID */
    USHORT CplQueueID;

    /* Starting virtual addr of submission Queue (system memory page aligned) */
    PVOID pSubQStart;

    /* Starting physical address of submission queue */
    STOR_PHYSICAL_ADDRESS SubQStart;

    /* Linked-list contains SQ free entries */
    LIST_ENTRY FreeQList;

    /* Indicates the submission is shared among active cores in the system */
    BOOLEAN Shared;

    /* Command Entries */

    /* Starting virtual addr of all command entries */
    PVOID pCmdEntry;

    /* PRP Lists */

    /* Starting virtual addr of PRP Lists (system memory page aligned) */
    PVOID pPRPListStart;

    /* Starting physical address of PRP Lists */
    STOR_PHYSICAL_ADDRESS PRPListStart;

    /* Number of PRP lists one system page can accommodate */
    ULONG NumPRPListOnePage;

    /* Statistics */

    /* Current accumulated, issued requests */
    LONG64 Requests;

#ifdef DUMB_DRIVER
    PVOID pDblBuffAlloc;
    ULONG dblBuffSz;
    PVOID pDlbBuffStartVa;

    PVOID pDblBuffListAlloc;
    ULONG dblBuffListSz;
    PVOID pDlbBuffStartListVa;
#endif
} SUB_QUEUE_INFO, *PSUB_QUEUE_INFO;

/*******************************************************************************
 * Completiond Queue Information data structure.
 ******************************************************************************/
typedef struct _CPL_QUEUE_INFO
{
    /* Completion Queue */

    /* Associated Completion queue ID, 0 based. Admin queue ID is 0 */
    USHORT CplQueueID;

    /* Reported number of queue entries when creating the queue */
    USHORT CplQEntries;

    /* Used to decide if newly completed entry, either 0 or 1 */
    USHORT CurPhaseTag:1;

    /* Reserved */
    USHORT Reserved:15;

    /* Current head pointer to completion queue entries */
    USHORT CplQHeadPtr;

    /* Starting virtual addr of completion Queue (system memory page aligned) */
    PVOID pCplQStart;

    /* Associated doorbell register to ring for completions */
    PULONG pCplHDBL;

    /* Starting physical address of completion queue */
    STOR_PHYSICAL_ADDRESS CplQStart;

    /* Associated MSI Message used to interrupts, which is zero-based */
    USHORT MsiMsgID;

    /* Indicates the completion is shared among active cores in the system */
    BOOLEAN Shared;

    /* Statistics */

    /* Current accumulated, completed requests */
    ULONG64 Completions;
} CPL_QUEUE_INFO, *PCPL_QUEUE_INFO;

/*******************************************************************************
 * Main Queue Information data structure.
 ******************************************************************************/
typedef struct _QUEUE_INFO
{
    /*
     * Number of IO queues allocated from adapter via Set Feature command.
     * After adapter completes the Set Features request with Feature ID = 7
     * the callback function is invoked to examine the completion results.
     * If no error, NumSubIoQAllocFromAdapter and NumCplIoQAllocFromAdapter
     * are set to the allocated number, respectively.
     * In the error case, START_STATE_QUEUE_ALLOC_FAILURE is set as 1,
     * NumSubIoQAllocFromAdapter and NumCplIoQAllocFromAdapter remain zero.
     */
    ULONG NumSubIoQAllocFromAdapter; /* Number of submission queues allocated */
    ULONG NumCplIoQAllocFromAdapter; /* Number of completion queues allocated */

    /*
     * Allocated buffers via
     * StorPortPatchAllocateContiguousMemorySpecifyCacheNode function
     * Number of queues to allocate from system memory depends on:
     *
     *   1. NumSubIoQAllocFromAdapter and NumCplIoQAllocFromAdapter
     *   2. Number of active cores and NUMA nodes in the system
     *
     * The Number of queues can only be:
     *
     *   1. The number of active cores
     *   2. The number of NUMA nodes (all cores of a given NUMA node
     *      share one queue)
     *   3. One, i.e., all cores share the queue
     */
    ULONG NumSubIoQAllocated; /* Number of IO submission queues allocated */
    ULONG NumCplIoQAllocated; /* Number of IO completion queues allocated */

    /*
     * They start with the number of entries fetched from Registry.
     * When failing in the middle of allocating buffers,
     * they store the reduced number of queue entries and are used in
     * later allocations.
     */
    USHORT NumIoQEntriesAllocated; /* Number of IO queue entries allocated */
    USHORT NumAdQEntriesAllocated; /* Number of Admin queue entries allocated */

    /*
     * Number of queues created via Create IO Submission/Completion Queue
     * commands. When all succeeds, they are equal to NumSubIoQAllocated
     * and NumCplIoQAllocated in QUEUE_INFO.
     */
    ULONG NumSubIoQCreated; /* Number of submission queues created */
    ULONG NumCplIoQCreated; /* Number of completion queues created */
    ULONG NumIoQMapped; /* Number of queues mapped to MSI vectors */

    /* Array of Submission Queue Info structures */
    PSUB_QUEUE_INFO pSubQueueInfo; /* Pointing to the first allocated element */

    /* Array of Completion Queue Info structures */
    PCPL_QUEUE_INFO pCplQueueInfo; /* Pointing to the first allocated element */
} QUEUE_INFO, *PQUEUE_INFO;

/*******************************************************************************
 * Processor Group Table Data Structure
 ******************************************************************************/
typedef struct _PROC_GROUP_TBL
{
    /* Its associated first system-wise logical processor number*/
    ULONG BaseProcessor;

    /* Number of associated cores */
    ULONG NumProcessor;

    /* Its associated group number */
    GROUP_AFFINITY GroupAffinity;

} PROC_GROUP_TBL, *PPROC_GROUP_TBL;

/*******************************************************************************
 * NUMA Node Table Data Structure
 ******************************************************************************/
typedef struct _NUMA_NODE_TBL
{
    /* The NUMA node number */
    ULONG NodeNum;

    /* Its associated first core number (system-wise) */
    ULONG FirstCoreNum;

    /* Its associated last core number (system-wise) */
    ULONG LastCoreNum;

    /* Number of associated cores */
    ULONG NumCores;

    /* Its associated group number */
    GROUP_AFFINITY GroupAffinity;

} NUMA_NODE_TBL, *PNUMA_NODE_TBL;

/*******************************************************************************
 * CPU Core Table Data Structure
 ******************************************************************************/
typedef struct _CORE_TBL
{
    /* Its associated NUMA node */
    USHORT NumaNode;

    /* Its processor group number */
    USHORT Group;

    /* The vector associated with this core's QP */
    USHORT MsiMsgID;

    /* The associated queue pair info for this core */
    USHORT SubQueue;
    USHORT CplQueue;
    ULONG  Learned;
} CORE_TBL, *PCORE_TBL;

/*******************************************************************************
 * MSI/MSI-X Message Table Data Structure
 ******************************************************************************/
typedef struct _MSI_MESSAGE_TBL
{
    /* The MSI message number granted from the system */
    ULONG MsgID;

    /* Physical address associated with the message */
    ULONG Data;

    /* Data associated with the message */
    STOR_PHYSICAL_ADDRESS Addr;

    /*
     * The associated completion queue number. When the message is shared
     * (Shared==TRUE), all queues need to be checked for completion entries
     */
    USHORT CplQueueNum;

    /* Indicates MSI message is shared by multiple completion queues */
    BOOLEAN Shared;

    /* Indicates if this MSI vector has been mapped already */
    ULONG Learned;
} MSI_MESSAGE_TBL, *PMSI_MESSAGE_TBL;

/*******************************************************************************
 * Resource Mapping Table Data Structure
 ******************************************************************************/
typedef struct _RES_MAPPING_TBL
{
    /* INTx(1), MSI(2), MSI-X(4) */
    ULONG InterruptType;

    /* Number of active cores in current system */
    ULONG NumActiveCores;

    /* Array of Core Tables, each active core has its own table allocated */
    PCORE_TBL pCoreTbl; /* Pointer to the first table */

    /* Number of MSI/MSI-X vectors granted from system */
    ULONG NumMsiMsgGranted;

    /*
     * Array of sorted MSI/MSI-X message table, each granted message has its
     * own table allocated. The sorting is based on the Destination
     * Field(bit[19..12]) of the MSI/MSI-X Address when it's physical mode for
     * interrupt routing without redirection.
     */
    PMSI_MESSAGE_TBL pMsiMsgTbl; /* Pointer to the first table */

    /* Number of NUMA nodes in system */
    ULONG NumNumaNodes;

    /* Array of NUMA affinity table, retrieved via StorPortPatchGetNodeAffinity */
    PNUMA_NODE_TBL pNumaNodeTbl; /* Topology of NUMA nodes/associated cores */

    /* Number of logical processor groups in current system */
    USHORT NumGroup;
    /* Topology of logical processor group */
    PPROC_GROUP_TBL pProcGroupTbl;
} RES_MAPPING_TBL, *PRES_MAPPING_TBL;

/* LUN Extension */
typedef enum _LUN_SLOT_STATUS
{
    FREE,
    ONLINE,
    OFFLINE
} LUN_SLOT_STATUS;
typedef enum _LUN_OFFLINE_REASON
{
    NOT_OFFLINE,
    FORMAT_IN_PROGRESS,
    DETACH_IN_PROGRESS,
    DELETE_IN_PROGRESS
    // Add more as needed
} LUN_OFFLINE_REASON;

typedef struct _nvme_lun_extension
{
    ADMIN_IDENTIFY_NAMESPACE     identifyData;
    UINT32                       namespaceId;
    NS_STATUS                    nsStatus;
    BOOLEAN                      ReadOnly;
    BOOLEAN                      nsReady;
    BOOLEAN                      IsNamespaceReadOnly;
    LUN_SLOT_STATUS              slotStatus;
    LUN_OFFLINE_REASON           offlineReason;
} NVME_LUN_EXTENSION, *PNVME_LUN_EXTENSION;

/* Submission Queue Entry Unit - 64 Bytes */
typedef struct _submission_queue_entry_unit
{
    /* DWORD 0 */
    UINT32 commandId      : 16;
    UINT32 reserved1      :  6;
    UINT32 fusedOperation :  2;
    UINT32 opcode         :  8;

    /* DWORD 1 */
    UINT32 namespaceId;

    /* DWORD 2 and 3 */
    UINT64 reserved2;

    /* DWORD 4 and 5 */
    UINT64 metadataPointer;

    /* DWORD 6 and 7 */
    UINT64 prpEntry1;

    /* DWORD 8 and 9 */
    UINT64 prpEntry2;

    /* DWORD 10 thru 15 - These fields are command specific */
    UINT32 dword10;
    UINT32 dword11;
    UINT32 dword12;
    UINT32 dword13;
    UINT32 dword14;
    UINT32 dword15;
} SUBMISSION_QUEUE_ENTRY_UNIT, *PSUBMISSION_QUEUE_ENTRY_UNIT;

/* NVMe Miniport Device Extension */
typedef struct _nvme_device_extension
{

    /* Controller register base address */
    PNVMe_CONTROLLER_REGISTERS  pCtrlRegister;

    /* PCI configuration information for the controller */ 
    PPORT_CONFIGURATION_INFORMATION pPCI;

    /* NVMe queue information structure */
    QUEUE_INFO                  QueueInfo;
    BOOLEAN                     IoQueuesAllocated;

    /* Byte size of each PRP list, (MaxTx/PAGE_SIZE)*8 */
    ULONG                       PRPListSize;

    /* Resource Mapping Table */
    RES_MAPPING_TBL             ResMapTbl;
    BOOLEAN                     ResourceTableMapped;

    /* The initial values fetched from Registry */
    INIT_INFO                   InitInfo;

    /* Used to read the PCI config space */
    ULONG                       SystemIoBusNumber;

    /* Bus, Device, Function */
    ULONG                       SlotNumber;

    /* Tables */
    ULONG                       LunExtSize;

    /* Reference by LUN Id and current number of visible NSs */
    PNVME_LUN_EXTENSION         pLunExtensionTable[MAX_NAMESPACES];
    ULONG                       visibleLuns;

    /* Controller Identify Data */
    ADMIN_IDENTIFY_CONTROLLER   controllerIdentifyData;

    /* Scsi WMI info */
    SCSI_WMILIB_CONTEXT         WmiLibContext;
    /* Wmi Custom Data */

    /* General Device Extension info */

    /* State Machine structure */
    START_STATE                 DriverState;

    /* Used to determine if we are in NT context */
    BOOLEAN                     InNTContext;

    /* Used to determine if we are in crashdump mode */
    BOOLEAN                     ntldrDump;

    /* The bytes of memory have been allocated from the dump/hibernation buffer*/
    ULONG                       DumpBufferBytesAllocated;
    /* The memory buffer in dump/hibernation mode. */
    PUCHAR                      DumpBuffer;

    /* saved a few calc'd values based on CAP fields */
    ULONG                       uSecCrtlTimeout;
    ULONG                       strideSz;

    /* DPCs needed for SNTI, AER, and error recovery */
    STOR_DPC                    SntiDpc;
    STOR_DPC                    AerDpc;
    STOR_DPC                    RecoveryDpc;
    BOOLEAN                     RecoveryAttemptPossible;

    /* IO Completion DPC Array Info */
    PVOID                       pDpcArray;
    ULONG                       NumDpc;

    /* INTx interrupt mask flag */
    BOOLEAN                     IntxMasked;
    BOOLEAN                     ShutdownInProgress;

    /* state of shutdown */
    STOR_POWER_ACTION           PowerAction;

    /* Format NVM State Machine information */
    FORMAT_NVM_INFO             FormatNvmInfo;

    /* counter used to determine in learning the vector/core table */
    ULONG                       LearningCores;

    /* Flag to indicate multiple cores are sharing a single queue */
    BOOLEAN                     MultipleCoresToSingleQueueFlag;

    /* Flag to indicate hardReset is in progress in polled mode */
	BOOLEAN                     polledResetInProg;

   /* Array to hold group affinity data */
   PGROUP_AFFINITY             pArrGrpAff;

   /* Flag to check if StorPortInitializePerfOpts API executed succesfully.. */
   BOOLEAN                     IsMsiMappingComplete;

#if DBG
    /* part of debug code to sanity check learning */
    BOOLEAN                     LearningComplete;
#endif

    union 
    {
        struct {
            UCHAR  Reserved;
            UCHAR  MNR;
            USHORT MJR;
        };
        ULONG                       value;
    } originalVersion;
    BOOLEAN                         DeviceRemovedDuringIO;
#if (NTDDI_VERSION > NTDDI_WIN7)
	PVOID Timerhandle;
#endif

} NVME_DEVICE_EXTENSION, *PNVME_DEVICE_EXTENSION;

#pragma pack(1)
typedef struct _nvme_res_report_struct {
    NVM_RES_REPORT_HDR reservationRepData;
    // Add an extra element to allow for alignment
    NVM_REGISTERED_CTRL_DATASTRUCT reservationRegCtlData[MAX_REGISTERED_CONTROLLERS];
} NVME_RES_REPORT_STRUCT, *PNVME_RES_REPORT_STRUCT;
#pragma pack()
/* SRB Extension */
typedef struct _nvme_srb_extension
{
    /* General SRB Extension info */

    /* Pointer back to miniport adpater extension*/
    PNVME_DEVICE_EXTENSION       pNvmeDevExt;

    /* Pointer back to SRB - NULL if internal I/O */
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK       pSrb;
#else
    PSCSI_REQUEST_BLOCK          pSrb;
#endif

    /* Is this an ADMIN command or an NVM command */
    BOOLEAN                      forAdminQueue;

    /* NVMe Specific data */

    /* Submission queue entry */
    NVMe_COMMAND  nvmeSqeUnit;

    /* Completion Entry Data */
    PNVMe_COMPLETION_QUEUE_ENTRY pCplEntry;

    /* Callback completion routine, if needed */
    PNVME_COMPLETION_ROUTINE     pNvmeCompletionRoutine;

    BOOLEAN ModeSenseWaitState;

    /* WMI */

    /* space for keeping info about the active WMI command */
    SCSIWMI_REQUEST_CONTEXT        WmiReqContext;

    /*
     * dsmBuffer adds a DWORD-aligned 2K buffer that -- when combined with 
     * the prpList which immediately follows it -- serves as a 4K area that is 
     * used to house DSM range definitions when issuing DSM commands in 
     * processing SCSI UNMAP requests. 
     * 
     * THIS MUST REMAIN IMMEDIATELY IN FRONT OF prpList IN ORDER TO ACHIEVE 4K 
     * OF BUFFER SPACE (OTHERWISE, IF SRB EXTENSION SIZE IS NOT A CONCERN, IT 
     * COULD SIMPLY BE DEFINED AS PAGE_SIZE_IN_DWORDS TO ELIMINATE DEPENDANCY)
     */

#define PRP_LIST_SIZE            sizeof(UINT64) * (MAX_TX_SIZE / PAGE_SIZE)

    union {
        // To allow for alignment of dsmBuffer on 16-byte boundary, add 1 extra element
        UINT32                       dsmBuffer[(PAGE_SIZE_IN_DWORDS + 4) -
            PRP_LIST_SIZE / sizeof(UINT32)];

        // Buffer may also be used for reservation commands. 
        // Allocate an extra 4 bytes for buffer alignment

        /* Reservation Report data */
        UCHAR resReportStruct[sizeof(NVME_RES_REPORT_STRUCT) + sizeof(ULONG)];

        /* Reservation Register data */
        UCHAR resRegisterData[sizeof(NVM_RES_REGISTER_DATASTRUCT) + sizeof(ULONG)];

        /* Reservation Acquire data */
        UCHAR resAcquireData[sizeof(NVM_RES_ACQUIRE_DATASTRUCT) + sizeof(ULONG)];

        /* Reservation Release data */
        UCHAR resReleaseData[sizeof(NVM_RES_RELEASE_DATASTRUCT) + sizeof(ULONG)];
    };

    /* Temp PRP List */
    UINT64                       prpList[MAX_TX_SIZE / PAGE_SIZE];
    UINT32                       numberOfPrpEntries;

    /* Data buffer pointer for internally allocated memory */
    UINT32                       dataBufferSize;
    PVOID                        pDataBuffer;

    /* Child/Parent pointers for child I/O's needed when holes in SGL's */
    PVOID                        pChildIo;
    PVOID                        pParentIo;

    /* 
	 * Temporary buffer to prepare the modesense data before copying into 
	 * pSrb->DataBuffer
	 */
	UCHAR                        modeSenseBuf[MODE_SNS_MAX_BUF_SIZE];
    ULONG                        abortedCmdCount;
    ULONG                        issuedAbortCmdCnt;
    ULONG                        failedAbortCmdCnt;
    BOOLEAN                      cmdGotAbortedFlag;

#if DBG
    /* used for debug learning the vector/core mappings */
    PROCESSOR_NUMBER             procNum;
#endif

#ifdef DUMB_DRIVER
    PVOID pDblVir;     // this cmd's dbl buffer virtual address
    PVOID pSrbDataVir; // this cmd's SRB databuffer virtual address
    ULONG dataLen;     // dbl buff data length
#endif
} NVME_SRB_EXTENSION, *PNVME_SRB_EXTENSION;

/*******************************************************************************
 *                            Function Prototype Section
 ******************************************************************************/

/* Implemented in nvmeInit.c */

STOR_PHYSICAL_ADDRESS NVMeGetPhysAddr(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in PVOID pVirtAddr
);

BOOLEAN NVMeGetCurCoreNumber(
    __in PNVME_DEVICE_EXTENSION pAE,
    __inout PPROCESSOR_NUMBER pPN
);

VOID NVMeCrashDelay(
    ULONG delayInUsec,
    BOOLEAN ntldrDump
);

PVOID NVMeAllocateMem(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in ULONG Size,
    __in ULONG Node
);

VOID NVMeStallExecution(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in ULONG microSeconds
);

PVOID NVMeAllocatePool(
    PNVME_DEVICE_EXTENSION pAE,
    ULONG Size
);

BOOLEAN NVMeEnumNumaCores(
    __in PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeStrCompare(
    __in PCSTR pTargetString,
    __in PCSTR pArgumentString
);

BOOLEAN NVMeReInitializeController(
	__in PNVME_DEVICE_EXTENSION pAE
	);

BOOLEAN NVMeResetController(
    __in PNVME_DEVICE_EXTENSION pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
    __in PSTORAGE_REQUEST_BLOCK pSrb
#else
    __in PSCSI_REQUEST_BLOCK pSrb
#endif
);

BOOLEAN NVMeEnumMsiMessages(
    __in PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeCallArbiter(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeMsiMapCores(
    __in PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeCompleteResMapTbl(
    __in PNVME_DEVICE_EXTENSION pAE
);

ULONG NVMeMapCore2Queue(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in PPROCESSOR_NUMBER pPN,
    __inout USHORT* pSubQueue,
    __inout USHORT* pCplQueue
);


VOID
IoCompletionRoutine(
    IN PSTOR_DPC  pDpc,
    IN PVOID  pHwDeviceExtension,
    IN PVOID  pSystemArgument1,
    IN PVOID  pSystemArgument2
    );


VOID NVMeInitFreeQ(
    __in PSUB_QUEUE_INFO pSQI,
    __in PNVME_DEVICE_EXTENSION pAE
);

ULONG NVMeInitSubQueue(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in USHORT QueueID
);

ULONG NVMeInitCplQueue(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in USHORT QueueID
);

ULONG NVMeInitCmdEntries(
    PNVME_DEVICE_EXTENSION pAE,
    USHORT QueueID
);

BOOLEAN NVMeResetAdapter(
    __in PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeEnableAdapter(
    __in PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeSetFeaturesCompletion(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in PNVMe_COMMAND pNVMeCmd,
    __in PNVMe_COMPLETION_QUEUE_ENTRY pCplEntry
);

BOOLEAN NVMeInitCallback(
    __in PVOID pAE,
    __in PVOID pSrbExtension
);

BOOLEAN NVMeSetIntCoalescing(
    __in PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeAllocQueueFromAdapter(
    __in PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeAccessLbaRangeEntry(
    __in PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeGetIdentifyStructures(
    PNVME_DEVICE_EXTENSION pAE,
    ULONG NamespaceID,
    USHORT CNS
);

BOOLEAN NVMeCreateCplQueue(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in USHORT QueueID
);

BOOLEAN NVMeCreateSubQueue(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in USHORT QueueID
);

BOOLEAN NVMeDeleteCplQueues(
    __in PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeDeleteSubQueues(
    __in PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeNormalShutdown(
    __in PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeAllocIoQueues(
    __in PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeFreeBuffers (
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeFreeNonContiguousBuffers (
    PNVME_DEVICE_EXTENSION pAE
);

PCMD_ENTRY NVMeAcqQueueEntry(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in PLIST_ENTRY pFreeQ
);

ULONG NVMeGetCmdEntry(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in USHORT QueueID,
    __in PVOID Context,
    __inout PVOID pCmdInfo
);

ULONG NVMeIssueCmd(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in USHORT QueueID,
    __in PVOID pTempSubEntry
);

ULONG NVMeGetCplEntry(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in PCPL_QUEUE_INFO pCQI,
    __inout PVOID pCplEntry
);

BOOLEAN NVMeReadRegistry(
    PNVME_DEVICE_EXTENSION pAE,
    UCHAR* pLabel,
    ULONG Type,
    UCHAR* pBuffer,
    ULONG* pLen
);

BOOLEAN NVMeWriteRegistry(
    PNVME_DEVICE_EXTENSION pAE,
    UCHAR* pLabel,
    ULONG Type,
    UCHAR* pBuffer,
    ULONG Len
);

BOOLEAN NVMeFetchRegistry(
    PNVME_DEVICE_EXTENSION pAE
);


BOOLEAN NVMePreparePRPs(
    PNVME_DEVICE_EXTENSION pAE,
    PNVME_SRB_EXTENSION pSrbExt,
    PVOID pBuffer,
    ULONG TxLength
);

/* Implemented in nvmeStat.c */

BOOLEAN NvmeAdapterReset(
    PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeRunningStartAttempt(
    PNVME_DEVICE_EXTENSION pAE,
    BOOLEAN resetDriven,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pResetSrb
#else
    PSCSI_REQUEST_BLOCK pResetSrb
#endif
);

VOID NVMeRunning(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnRDY(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnIdentifyCtrl(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnListAttachedNs(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnListExistingNs(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnIdentifyNS(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnSetupQueues(
    PNVME_DEVICE_EXTENSION pAE
);

VOID
NVMeRunningWaitOnSetFeatures(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnIoCQ(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnIoSQ(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnLearnMapping(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnReSetupQueues(
    PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeRunningWaitOnNamespaceReady(
	PNVME_DEVICE_EXTENSION pAE
);

VOID NVMeDriverFatalError(
    PNVME_DEVICE_EXTENSION pAE,
    ULONG ErrorNum
);

BOOLEAN NvmeReset(
    PNVME_DEVICE_EXTENSION pAE
);

/* Storport miniport driver entry routines, implemented in nvmeStd.c */

ULONG NVMeFindAdapter(
    __in PVOID AdapterExtension,
    __in PVOID HwContext,
    __in PVOID BusInformation,
    __in PCSTR ArgumentString,
    __inout PPORT_CONFIGURATION_INFORMATION ConfigInfo,
    __out PUCHAR Again
);

ULONG NVMeAllocQueues(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in USHORT QueueID,
    __in ULONG QEntries,
    __in USHORT NumaNode
);

BOOLEAN NVMePassiveInitialize(
    PVOID Context
);

BOOLEAN NVMeInitialize(
    __in PVOID AdapterExtension
);

#if (NTDDI_VERSION > NTDDI_WIN7)
    HW_TIMER_EX IsDeviceRemoved;
#else
    VOID IsDeviceRemoved(PNVME_DEVICE_EXTENSION pAE);
#endif

BOOLEAN NVMeStartIo(
    __in PVOID AdapterExtension,
    __in PSCSI_REQUEST_BLOCK Srb
);

BOOLEAN NVMeBuildIo(
    __in PVOID AdapterExtension,
    __in PSCSI_REQUEST_BLOCK Srb
);

void NVMeStartIoProcessIoctl(
    __in PNVME_DEVICE_EXTENSION pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
    __in PSTORAGE_REQUEST_BLOCK pSrb
#else
    __in PSCSI_REQUEST_BLOCK pSrb
#endif
);

BOOLEAN NVMeIsrIntx(
    __in PVOID AdapterExtension
);

BOOLEAN NVMeIsrMsix(
    __in PVOID AdapterExtension,
    __in ULONG MsgID
);

BOOLEAN NVMeResetBus(
    __in PVOID AdapterExtension,
    __in ULONG PathId
);

SCSI_ADAPTER_CONTROL_STATUS NVMeAdapterControl(
    __in PVOID AdapterExtension,
    __in SCSI_ADAPTER_CONTROL_TYPE ControlType,
    __in PVOID Parameters
);

BOOLEAN NVMeWaitForCtrlRDY(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in ULONG expectedValue
);

VOID RecoveryDpcRoutine(
    IN PSTOR_DPC  pDpc,
    IN PVOID  pHwDeviceExtension,
    IN PVOID  pSystemArgument1,
    IN PVOID  pSystemArgument2
);

VOID NVMeInitSrbExtension(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

VOID NVMeIoctlHotRemoveNamespace(
    PNVME_SRB_EXTENSION pSrbExt
);

VOID NVMeIoctlHotAddNamespace(
    PNVME_SRB_EXTENSION pSrbExt
);

VOID NVMeFormatNVMHotRemoveNamespace(
    PNVME_SRB_EXTENSION pSrbExt
);

VOID NVMeFormatNVMHotAddNamespace(
    PNVME_SRB_EXTENSION pSrbExt
);

BOOLEAN NVMeIoctlFormatNVMCallback(
    PVOID pNVMeDevExt,
    PVOID pSrbExtension
);

BOOLEAN NVMeIoctlFormatNVM(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
);


BOOLEAN NVMeIoctlNamespaceMgmt(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
	PSTORAGE_REQUEST_BLOCK pSrb,
#else
	PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
);

BOOLEAN NVMeIoctlNamespaceAttachment(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
	PSTORAGE_REQUEST_BLOCK pSrb,
#else
	PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_PASS_THROUGH_IOCTL pNvmePtIoctl
);

BOOLEAN NVMeProcessIoctl(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

BOOLEAN NVMeProcessPublicIoctl(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

BOOLEAN NVMeProcessPrivateIoctl(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

BOOLEAN NVMeIoctlCallback(
    PVOID pNVMeDevExt,
    PVOID pSrbExtension
);

BOOLEAN NVMeHandleNVMePassthrough(
    PVOID pNVMeDevExt,
    PNVME_SRB_EXTENSION pSrbExtension
);

BOOLEAN NVMeHandleSmartAttribs(
    PVOID pNVMeDevExt,
    PNVME_SRB_EXTENSION pSrbExtension
);

BOOLEAN NVMeHandleSmartThresholds(
    PVOID pNVMeDevExt,
    PNVME_SRB_EXTENSION pSrbExtension
);

BOOLEAN NVMeCompletionNsAttachment(
    PVOID pNVMeDevExt,
    PNVME_SRB_EXTENSION pSrbExt
);
 
BOOLEAN NVMeCompletionNsMgmt(
    PVOID pNVMeDevExt,
    PNVME_SRB_EXTENSION pSrbExt
);

VOID NVMeBuildIdentify(
    PSENDCMDOUTPARAMS pCmdOutParameters,
    PNVME_DEVICE_EXTENSION pSrbExt
);

ULONG NVMeInitAdminQueues(
    PNVME_DEVICE_EXTENSION pAE
);

BOOLEAN NVMeIsNamespaceVisible(
    __in PNVME_SRB_EXTENSION pSrbExt,
    __in ULONG targetNSID,
    __out PULONG pLunId
);

NS_STATUS NVMeGetNamespaceStatusAndSlot(
    __in PNVME_DEVICE_EXTENSION pDevExt,
    __in ULONG targetNSID,
    __out PULONG pTableIndex
);

ULONG NVMeGetFreeLunSlot(
    __in PNVME_DEVICE_EXTENSION pDevExt
);

VOID NVMeLogError(
    __in PNVME_DEVICE_EXTENSION pAE,
    __in ULONG ErrorNum
);

#if DBG
VOID IO_StorPortNotification(
    __in SCSI_NOTIFICATION_TYPE NotificationType,
    __in PVOID pHwDeviceExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
    __in PSTORAGE_REQUEST_BLOCK pSrb
#else
    __in PSCSI_REQUEST_BLOCK pSrb
#endif
);
#else
#define IO_StorPortNotification StorPortNotification
#endif

#ifndef DBG
VOID WppCleanupRoutine(PVOID arg1);
#endif


#endif /* __NVME_STD_H__ */
