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
 * File: nvmeSnti.h
 */

#ifndef __NVME_SNTI_H__
#define __NVME_SNTI_H__

#define MODE_SENSE_WAIT_FOR_GET_LOG_PAGE_RESPONSE   0x1
#define MODE_SENSE_WAIT_FOR_GET_FEATURE_RESPONSE    0x2

#define VALID_NVME_PATH_ID   0
#define VALID_NVME_TARGET_ID 0
#define SNTI_STORPORT_QUEUE_DEPTH (254)
#define MODE_BLOCK_DESC_MAX (0xFFFFFF)
#define MODE_BLOCK_DESC_MAX_BYTE (0xFF)
#define NVME_MAX_NUM_BLOCKS_PER_READ_WRITE (0xFFFF)
#define WRITE_PROTECT       0x80

/*******************************************************************************
 * VPD_EXTENDED_INQUIRY_DATA
 *
 * This structure defines Inquiry VPD page - VPD_EXTENDED_INQUIRY_DATA as is
 * defined in SPC-4 rev. 24.
 ******************************************************************************/
typedef struct _extended_inquiry_data
{
   UCHAR DeviceType                      : 5;
   UCHAR DeviceTypeQualifier             : 3;
   UCHAR PageCode;
   UCHAR Reserved1;
   UCHAR PageLength;

   UCHAR ReferenceTagCheck               : 1;
   UCHAR ApplicationTagCheck             : 1;
   UCHAR GuardCheck                      : 1;
   UCHAR SupportedProtectionType         : 3;
   UCHAR ActivateMicrocode               : 2;


   UCHAR SimpleSupported                 : 1;
   UCHAR OrderedSupported                : 1;
   UCHAR HeadOfQueueSupported            : 1;
   UCHAR PrioritySupported               : 1;
   UCHAR GroupingFunctionSupported       : 1;
   UCHAR UACSKDataSupported              : 1;
   UCHAR Reserved2                       : 2;

   UCHAR VolatileCacheSupported          : 1;
   UCHAR NonVolatileCacheSupported       : 1;
   UCHAR CorrectionDisableSupported      : 1;
   UCHAR WriteUncorrectableSupported     : 1;
   UCHAR Reserved3                       : 4;

   UCHAR LogicalUnitITNexusClear         : 1;
   UCHAR Reserved5                       : 3;
   UCHAR ProtectionInfoIntervalSupported : 1;
   UCHAR Reserved4                       : 3;

   UCHAR CapabilityBasedCommandSecurity  : 1;
   UCHAR Reserved7                       : 3;
   UCHAR ReferralsSupported              : 1;
   UCHAR Reserved6                       : 3;

   UCHAR MultiITNexusMicrodeDownload     : 4;
   UCHAR Reserved8                       : 4;

   UCHAR Reserved9[54];
} EXTENDED_INQUIRY_DATA, *PEXTENDED_INQUIRY_DATA;

/*******************************************************************************
 * READ_CAPACITY_16_DATA
 *
 * This structure defines READ CAP 16 Data Page as is defined in SPC-4 rev. 24.
 ******************************************************************************/
typedef struct _read_capacity_16_data
{
    UINT64 LogicalBlockAddress;
    UINT32 BytesPerBlock;
    UINT8  ProtectionEnable                      : 1;
    UINT8  ProtectionType                        : 3;
    UINT8  Reserved1                             : 4;

    UINT8  LogicalBlocksPerPhysicalBlockExponent : 4;
    UINT8  ProtectionInfoIntervals               : 4;

    UINT8  LowestAlignedLbaMsb                   : 6;
    UINT8  LogicalBlockProvisioningReadZeros     : 1;
    UINT8  LogicalBlockProvisioningMgmtEnabled   : 1;

    UINT8  LowestAlignedLbaLsb;
    UINT8  Reserved2[16];

} READ_CAPACITY_16_DATA, *PREAD_CAPACITY_16_DATA;

/*******************************************************************************
 * DESCRIPTOR_FORMAT_SENSE_DATA
 *
 * This structure defines Descriptor Formate Sense Data as defined in SPC-4
 * rev. 24.
 ******************************************************************************/
typedef struct _descriptor_format_sense_data
{
    UINT8 ResponseCode                  : 7;
    UINT8 Reserved1                     : 1;
    UINT8 SenseKey                      : 4;
    UINT8 Reserved2                     : 4;
    UINT8 AdditionalSenseCode;
    UINT8 AdditionalSenseCodeQualifier;
    UINT8 Reserved3[3];
    UINT8 AdditionalSenseLength;

    /* Determine what Descriptor Types will be needed, if any */
    PUCHAR SenseDataDescriptor[1];
} DESCRIPTOR_FORMAT_SENSE_DATA, *PDESCRIPTOR_FORMAT_SENSE_DATA;

/*******************************************************************************
 * SUPPORTED_PAGE_DESCRIPTOR
 ******************************************************************************/
typedef struct _supported_page_descriptor
{
    UINT8 PageCode : 6;
    UINT8 Reserved : 2;
} SUPPORTED_PAGE_DESCRIPTOR, *PSUPPORTED_PAGE_DESCRIPTOR;

/*******************************************************************************
 * SUPPORTED_LOG_PAGES_LOG_PAGE
 ******************************************************************************/
typedef struct _supported_log_pages_log_page
{
    UINT8  PageCode      : 6;
    UINT8  SubPageFormat : 1;
    UINT8  DisableSave   : 1;
    UINT8  SubPageCode;
    UINT8 PageLength[2];
    SUPPORTED_PAGE_DESCRIPTOR supportedPages[3];
} SUPPORTED_LOG_PAGES_LOG_PAGE, *PSUPPORTED_LOG_PAGES_LOG_PAGE;

/*******************************************************************************
 * INFORMATIONAL_EXCEPTIONS_LOG_PAGE
 ******************************************************************************/
typedef struct _informational_exceptions_log_page
{
    UINT8  PageCode         : 6;
    UINT8  SubPageFormat    : 1;
    UINT8  DisableSave      : 1;
    UINT8  SubPageCode;
    UINT8 PageLength[2];
    UINT8 ParameterCode[2];
    UINT8  FormatAndLinking : 2;
    UINT8  TMC              : 2;
    UINT8  ETC              : 1;
    UINT8  TSD              : 1;
    UINT8  Reserved1        : 1;
    UINT8  DU               : 1;
    UINT8  ParameterLength;
    UINT8  InfoExcpAsc;
    UINT8  InfoExcpAscq;
    UINT8  MostRecentTempReading; /* Data from SMART/Health NVMe Log Page */
    UINT8  Reserved2;
} INFORMATIONAL_EXCEPTIONS_LOG_PAGE, *PINFORMATIONAL_EXCEPTIONS_LOG_PAGE;

/*******************************************************************************
 * TEMPERATURE_LOG_PAGE
 ******************************************************************************/
typedef struct _temperature_log_page
{
    UINT8  PageCode                 : 6;
    UINT8  SubPageFormat            : 1;
    UINT8  DisableSave              : 1;
    UINT8  SubPageCode;
    UINT8 PageLength[2];
    UINT8 ParameterCode_Temp[2];
    UINT8  FormatAndLinking_Temp    : 2;
    UINT8  TMC_Temp                 : 2;
    UINT8  ETC_Temp                 : 1;
    UINT8  TSD_Temp                 : 1;
    UINT8  Reserved1_Temp           : 1;
    UINT8  DU_Temp                  : 1;
    UINT8  ParameterLength_Temp;
    UINT8  Reserved2_Temp; 
    UINT8  Temperature;
    UINT8 ParameterCode_RefTemp[2];
    UINT8  FormatAndLinking_RefTemp : 2;
    UINT8  TMC_RefTemp              : 2;
    UINT8  ETC_RefTemp              : 1;
    UINT8  TSD_RefTemp              : 1;
    UINT8  Reserved1_RefTemp        : 1;
    UINT8  DU_RefTemp               : 1;
    UINT8  ParameterLength_RefTemp;
    UINT8  Reserved2_RefTemp;
    UINT8  ReferenceTemperature; /* Data from SMART/Health NVMe Log Page */
} TEMPERATURE_LOG_PAGE, *PTEMPERATURE_LOG_PAGE;

/*******************************************************************************
 * SMART_HEALTH_INFO_LOG
 ******************************************************************************/
typedef struct _smart_health_info_log
{

    UINT8  CriticalWarning;            /* Bytes:       0 */
    UINT16 Temperature;                /* Bytes:     1:2 */
    UINT8  AvailableSpare;             /* Bytes:       3 */
    UINT8  AvailableSpareThreshold;    /* Bytes:       4 */
    UINT8  PercentageUsed;             /* Bytes:       5 */
    UINT8  Reserved1[26];              /* Bytes:    6:31 */
    UINT8  DataUnitsRead[16];          /* Bytes:   32:47 */
    UINT8  DataUnitsWritten[16];       /* Bytes:   48:63 */
    UINT8  HostReadCommands[16];       /* Bytes:   64:79 */
    UINT8  HostWriteCommands[16];      /* Bytes:   80:95 */
    UINT8  ControllerBusyTime[16];     /* Bytes:  96:111 */
    UINT8  PowerCycles[16];            /* Bytes: 112:127 */
    UINT8  PowerOnHours[16];           /* Bytes: 128:143 */
    UINT8  UnsafeShutdowns[16];        /* Bytes: 144:159 */
    UINT8  MediaErrors[16];            /* Bytes: 160:175 */
    UINT8  NumErrorInfoLogEntries[16]; /* Bytes: 176:191 */
    UINT8  Reserved2[321];             /* Bytes: 192:512 */
} SMART_HEALTH_INFO_LOG, *PSMART_HEALTH_INFO_LOG;

/*******************************************************************************
 * CACHING_MODE_PAGE - 0x08
 ******************************************************************************/
typedef struct _caching_mode_page
{
    UINT8 PageCode                    : 6; /* 0x08 */
    UINT8 Reserved1                   : 1;
    UINT8 PageSavable                 : 1;
    UINT8 PageLength;                      /* 0x12 */
    UINT8 ReadDisableCache            : 1;
    UINT8 MultiplicationFactor        : 1;
    UINT8 WriteCacheEnable            : 1;
    UINT8 Reserved2                   : 5;
    UINT8 WriteRetensionPriority      : 4;
    UINT8 ReadRetensionPriority       : 4;
    UINT8 DisablePrefetchTransfer[2];
    UINT8 MinimumPrefetch[2];
    UINT8 MaximumPrefetch[2];
    UINT8 MaximumPrefetchCeiling[2];
    UINT8 NV_DIS                      : 1;
    UINT8 Reserved3                   : 2;
    UINT8 VendorSpecific              : 2;
    UINT8 DRA                         : 1;
    UINT8 LBCSS                       : 1;
    UINT8 FSW                         : 1;
    UINT8 NumberOfCacheSegments;
    UINT8 CacheSegmentSize[2];
    UINT8 Reserved4[4];
} CACHING_MODE_PAGE, *PCACHING_MODE_PAGE;

/*******************************************************************************
 * CONTROL_MODE_PAGE - 0x0A
 ******************************************************************************/
typedef struct _control_mode_page
{
    UINT8 PageCode       : 6; /* 0x0A */
    UINT8 SubPageFormat  : 1;
    UINT8 PageSaveable   : 1;
    UINT8 PageLength;         /* 0x0A */
    UINT8 RLEC           : 1;
    UINT8 GLTSD          : 1;
    UINT8 D_Sense        : 1;
    UINT8 DPICZ          : 1;
    UINT8 TMF_Only       : 1;
    UINT8 TST            : 3;
    UINT8 Reserved1      : 1;
    UINT8 QERR           : 2;
    UINT8 NUAR           : 1;
    UINT8 QAlgMod        : 4;
    UINT8 Reserved2      : 3;
    UINT8 SWP            : 1;
    UINT8 UA_INTLCK_CTRL : 2;
    UINT8 RAC            : 1;
    UINT8 VS             : 1;
    UINT8 AutoLodeMode   : 3;
    UINT8 Reserved3      : 3;
    UINT8 TAS            : 1;
    UINT8 ATO            : 1;
    UINT8 Reserved4[2];
    UINT8 BusyTimeoutPeriod[2];
    UINT8 ExtSelfTestCompTime[2];
} CONTROL_MODE_PAGE, *PCONTROL_MODE_PAGE;

/*******************************************************************************
 * POWER_CONDITION_MODE_PAGE - 0x1A
 ******************************************************************************/
typedef struct _power_condition_mode_page
{
    UINT8 PageCode          : 6; /* 0x1A */
    UINT8 SubPageFormat     : 1;
    UINT8 PageSaveable      : 1;
    UINT8 PageLength;            /* 0x26 */
    UINT8 Standby_Y         : 1;
    UINT8 Reserved1         : 5;
    UINT8 PmBgInteraction   : 2;
    UINT8 Standby_Z         : 1;
    UINT8 Idle_A            : 1;
    UINT8 Idle_B            : 1;
    UINT8 Idle_C            : 1;
    UINT8 Reserved2         : 4;
    UINT8 Idle_A_ConditionTimer[4];
    UINT8 Standby_Z_ConditionTimer[4];
    UINT8 Idle_B_ConditionTimer[4];
    UINT8 Idle_C_ConditionTimer[4];
    UINT8 Standby_Y_ConditionTimer[4];
    UINT8 Reserved3[16];
} POWER_CONDITION_MODE_PAGE, *PPOWER_CONDITION_MODE_PAGE;

/*******************************************************************************
 * INFO_EXCEPTIONS_MODE_PAGE - 0x1C
 ******************************************************************************/
typedef struct _info_exceptions_mode_page
{
    UINT8 PageCode      : 6; /* 0x1C */
    UINT8 SubPageFormat : 1;
    UINT8 PageSaveable  : 1;
    UINT8 PageLength;        /* 0x0A */
    UINT8 LogErr        : 1;
    UINT8 EbackErr      : 1;
    UINT8 Test          : 1;
    UINT8 DExcpt        : 1;
    UINT8 EWasc         : 1;
    UINT8 Ebf           : 1;
    UINT8 Reserved1     : 1;
    UINT8 Perf          : 1;
    UINT8 Mrie          : 4;
    UINT8 Reserved2     : 4;
    UINT8 IntervalTimer[4];
    UINT8 ReportCount[4];
} INFO_EXCEPTIONS_MODE_PAGE, *PINFO_EXCEPTIONS_MODE_PAGE;


#pragma pack(1)
/*******************************************************************************
* PERSISTENT RESERVE IN parameter data header
******************************************************************************/
typedef struct _persist_res_parm_data_hdr
{
    ULONG PRGENERATION;
    ULONG AddlLength;
} PERSIST_RES_PARM_DATA_HDR, *PPERSIST_RES_PARM_DATA_HDR;
#pragma pack()


#pragma pack(1)
/*******************************************************************************
* PERSISTENT RESERVE IN parameter data for READ RESERVATION
******************************************************************************/
typedef struct _persist_res_rd_res_data
{
    PERSIST_RES_PARM_DATA_HDR hdr;
    ULONGLONG ReservationKey;
    ULONG Obsolete;
    UCHAR Reserved;
    UCHAR Type  : 4;
    UCHAR Scope : 4;
    USHORT Obsolete2;
} PERSIST_RES_RD_RES_DATA, *PPERSIST_RES_RD_RES_DATA;
#pragma pack()

#pragma pack(1)
/*******************************************************************************
* PERSISTENT RESERVE IN response data - Persistent Reservation Type Mask
******************************************************************************/
typedef struct _persist_res_type_mask
{
    UCHAR Reserved1 : 1;
    UCHAR WR_EX     : 1;
    UCHAR Reserved2 : 1;
    UCHAR EX_AC     : 1;
    UCHAR Reserved3 : 1;
    UCHAR WR_EX_RO  : 1;
    UCHAR EX_AC_RO  : 1;
    UCHAR WR_EX_AR  : 1;

    UCHAR EX_AC_AR  : 1;
    UCHAR Reserved4 : 7;
} PERSIST_RES_TYPE_MASK, *PPERSIST_RES_TYPE_MASK;
#pragma pack()

#pragma pack(1)
/*******************************************************************************
* PERSISTENT RESERVE IN response data for REPORT CAPABILITIES
******************************************************************************/
typedef struct _persist_res_rep_capabilities
{
    USHORT Length;
    UCHAR PTPL_C    : 1;
    UCHAR Reserved1 : 1;
    UCHAR ATP_C     : 1;
    UCHAR SIP_C     : 1;
    UCHAR CRH       : 1;
    UCHAR Reserved2 : 2;
    UCHAR RLR_C     : 1;

    UCHAR PTPL_A    : 1;
    UCHAR Reserved3 : 3;
    UCHAR AllowCmds : 3;
    UCHAR TMV       : 1;
    PERSIST_RES_TYPE_MASK PersResTypeMask;
    USHORT Reserved4;
} PERSIST_RES_REP_CAPABILIITES, *PPERSIST_RES_REP_CAPABILIITES;
#pragma pack()


#pragma pack(1)
/*******************************************************************************
* PERSISTENT RESERVE IN full status descriptor 
******************************************************************************/
typedef struct _persist_res_full_status_desc
{
    ULONGLONG ReservationKey;
    ULONG Reserved;
    UCHAR R_HOLDER  : 1;
    UCHAR ALL_TG_PT : 1;
    UCHAR Reserved2 : 6;

    UCHAR Type      : 4;
    UCHAR Scope     : 4;

    ULONG Reserved3;
    USHORT RelTgtPortId;
    ULONG AddlDescLenth;
    ULONGLONG TransportID;
} PERSIST_RES_FULL_STATUS_DESC, *PPERSIST_RES_FULL_STATUS_DESC;
#pragma pack()

#pragma pack(1)
/*******************************************************************************
* PERSISTENT RESERVE OUT parm list
******************************************************************************/
typedef struct _persist_res_out_parms
{
    ULONGLONG ReservationKey;
    ULONGLONG ServiceActionRKey;
    ULONG Obsolete1;

    UCHAR APTPL     : 1;
    UCHAR Reserved1 : 1;
    UCHAR ALL_TG_PT : 1;
    UCHAR SPEC_I_PT : 1;
    UCHAR Reserved2 : 4;

    UCHAR Reserved3;
    USHORT Obsolete2;
} PERSIST_RES_OUT_PARMS, *PPERSIST_RES_OUT_PARMS;
#pragma pack()


/*******************************************************************************
 * SNTI_TRANSLATION_STATUS
 *
 * The SNTI_TRANSLATION_STATUS enumeration defines all possible status codes
 * for a translation sequence. 
 ******************************************************************************/
typedef enum _snti_translation_status
{
    SNTI_TRANSLATION_SUCCESS = 0,     /* Translation occurred w/o error */
    SNTI_COMMAND_COMPLETED,           /* Command completed in xlation phase */
    SNTI_SEQUENCE_IN_PROGRESS,        /* Command sequence still in progress */
    SNTI_SEQUENCE_COMPLETED,          /* Command sequence completed */
    SNTI_SEQUENCE_ERROR,              /* Error in command sequence */
    SNTI_FAILURE_CHECK_RESPONSE_DATA, /* Check SCSI status, device error */
    SNTI_UNSUPPORTED_SCSI_REQUEST,    /* Unsupported SCSI opcode */
    SNTI_UNSUPPORTED_SCSI_TM_REQUEST, /* Unsupported SCSI TM opcode */
    SNTI_INVALID_SCSI_REQUEST_PARM,   /* An invalid SCSI request parameter */
    SNTI_INVALID_SCSI_PATH_ID,        /* An invalid SCSI path id */
    SNTI_INVALID_SCSI_TARGET_ID,      /* An invalid SCSI target id */
    SNTI_UNRECOVERABLE_ERROR,         /* Unrecoverable error */
    SNTI_RESERVED                     /* Reserved for future use */
     
    /* TBD: Add additional codes as necessary */

} SNTI_TRANSLATION_STATUS;

/*******************************************************************************
 * SNTI_STATUS
 *
 * The SNTI_STATUS enumeration defines all possible "internal" status codes
 * for a translation sequence.
 ******************************************************************************/
typedef enum _snti_status
{
    SNTI_SUCCESS = 0,
    SNTI_FAILURE,
    SNTI_INVALID_REQUEST,
    SNTI_INVALID_PARAMETER,
    SNTI_INVALID_PATH_TARGET_ID,
    SNTI_NO_MEMORY,
    SNTI_COMPLETE_CMD
    /* TBD: Add fields as necessary */

} SNTI_STATUS;

/******************************************************************************
 * SNTI_RESPONSE_BLOCK
 ******************************************************************************/
typedef struct _snti_response_block
{
    UINT8 SrbStatus;
    UINT8 StatusCode;
    UINT8 SenseKey;
    UINT8 ASC;
    UINT8 ASCQ;
} SNTI_RESPONSE_BLOCK, *PSNTI_RESPONSE_BLOCK;

#pragma pack(1)
/********************************************************************************
* SNTI_VPD_DESRIPTOR_FLAGS
********************************************************************************/
typedef struct _snti_vpd_descriptor_flags {
    USHORT NaaIeeEui64Des : 1;
    USHORT NaaIeeV10Des : 1;
    USHORT T10VidEui64Des : 1;
    USHORT T10VidNguidDes : 1;
    USHORT T10VidV10Des : 1;
    USHORT ScsiEui64Des : 1;
    USHORT ScsiNguidDes : 1;
    USHORT ScsiV10Des : 1;
    USHORT Eui64Des : 1;
    USHORT Eui64NguidDes : 1;
    USHORT Reserved : 6;
} SNTI_VPD_DESCRIPTOR_FLAGS, *PSNTI_VPD_DESCRIPTOR_FLAGS;
#pragma pack()

#pragma pack(1)
/********************************************************************************
* SNTI_NAA_IEEE_REG_DESCRIPTOR
********************************************************************************/
typedef struct _snti_naa_ieee_ext_descriptor
{
    UCHAR IeeeCompIdMSB : 4;
    UCHAR Naa : 4;
    USHORT IeeeCompId;
    UCHAR VendIdMSB : 4;
    UCHAR IeeeCompIdLSB : 4;
    ULONG VendId;
    UINT64 VenSpecIdExt;
} SNTI_NAA_IEEE_EXT_DESCRIPTOR, *PSNTI_NAA_IEEE_EXT_DESCRIPTOR;
#pragma pack()

#pragma pack(1)
/********************************************************************************
* SNTI_T10_VID_DESCRIPTOR
********************************************************************************/
typedef struct _snti_t10_vid_descriptor
{
    UCHAR VendorId[8];
    UCHAR VendorSpecific[1];
} SNTI_T10_VID_DESCRIPTOR, *PSNTI_T10_VID_DESCRIPTOR;
#pragma pack()


/***  Public Interfaces  ***/

SNTI_TRANSLATION_STATUS SntiTranslateCommand(
    PNVME_DEVICE_EXTENSION pAdapterExtension,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

BOOLEAN SntiCompletionCallbackRoutine(
    PVOID param1,
    PVOID param2
);

BOOLEAN SntiMapCompletionStatus(
    PNVME_SRB_EXTENSION pSrbExt
);

/*** Private Interfaces ***/

SNTI_TRANSLATION_STATUS SntiTranslateInquiry(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

VOID SntiTranslateSupportedVpdPages(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

VOID SntiTranslateUnitSerialPage(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVME_LUN_EXTENSION pLunExt
);

VOID SntiTranslateDeviceIdentificationPage(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

BOOLEAN SntiBuildIeeeRegExtDesc(
    PSNTI_VPD_DESCRIPTOR_FLAGS pDescFlags,
    UINT16 *pCurrentLength,
    UINT16 srbBufLength,
    PUINT8 *ppNext,
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_DEVICE_EXTENSION pDevExt
   );

BOOLEAN SntiBuildT10VidBasedDesc(
    PSNTI_VPD_DESCRIPTOR_FLAGS pDescFlags,
    UINT16 *pCurrentLength,
    UINT16 srbBufLength,
    PUINT8 *ppNext,
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_DEVICE_EXTENSION pDevExt
    );

BOOLEAN SntiBuildScsiNameStringDesc(
    PSNTI_VPD_DESCRIPTOR_FLAGS pDescFlags,
    UINT16 *pCurrentLength,
    UINT16 srbBufLength,
    PUINT8 *ppNext,
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_DEVICE_EXTENSION pDevExt
    );

VOID SntiBuildEui64BasedDesc(
    PSNTI_VPD_DESCRIPTOR_FLAGS pDescFlags,
    UINT16 *pCurrentLength,
    UINT16 srbBufLength,
    PUINT8 *ppNext,
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_DEVICE_EXTENSION pDevExt
    );

USHORT SntiConvertULLongToA(
    PUCHAR pDest,
    ULONGLONG data,
    ULONG size,
    BOOLEAN underscore,
    BOOLEAN termPeriod
    );


SNTI_TRANSLATION_STATUS SntiTranslateExtendedInquiryDataPage(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);


#if (NTDDI_VERSION > NTDDI_WIN7)
VOID SntiTranslateBlockLimitsPage(
   PSTORAGE_REQUEST_BLOCK pSrb
);

VOID SntiTranslateBlockDeviceCharacteristicsPage(
   PSTORAGE_REQUEST_BLOCK pSrb
);

VOID SntiTranslateLogicalBlockProvisioningPage(
   PSTORAGE_REQUEST_BLOCK pSrb,
   PNVME_LUN_EXTENSION pLunExt
);
#endif


VOID SntiTranslateStandardInquiryPage(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslateReportLuns(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslateReadCapacity(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslateReadCapacity10(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PUCHAR pResponseBuffer,
    PNVME_LUN_EXTENSION pLunExtension
);

SNTI_TRANSLATION_STATUS SntiTranslateReadCapacity16(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PUCHAR pResponseBuffer,
    PNVME_LUN_EXTENSION pLunExtension
);
SNTI_TRANSLATION_STATUS SntiTranslateWrite(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_STATUS SntiTranslateWrite6(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
);

SNTI_STATUS SntiTranslateWrite10(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
);

SNTI_STATUS SntiTranslateWrite12(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
);

SNTI_STATUS SntiTranslateWrite16(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
);

SNTI_TRANSLATION_STATUS SntiTranslateRead(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_STATUS SntiTranslateRead6(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
);

SNTI_STATUS SntiTranslateRead10(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
);

SNTI_STATUS SntiTranslateRead12(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
);

SNTI_STATUS SntiTranslateRead16(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt
);

SNTI_TRANSLATION_STATUS SntiTranslateRequestSense(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslateSecurityProtocol(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslatePersistentReserveIn(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);


SNTI_TRANSLATION_STATUS SntiTranslatePersistentReserveOut(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);



SNTI_TRANSLATION_STATUS SntiTranslateStartStopUnit(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTransitionPowerState(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT8 powerCond,
    UINT8 powerCondMod,
    UINT8 start
);


#if (NTDDI_VERSION > NTDDI_WIN7)
SNTI_STATUS SntiValidateUnmapLbaAndLength(
    PNVME_LUN_EXTENSION pLunExt,
    PNVME_SRB_EXTENSION pSrbExt,
    UINT64 lba,
    UINT32 length
);

SNTI_TRANSLATION_STATUS SntiTranslateUnmap(
    PSTORAGE_REQUEST_BLOCK pSrb
);
#endif


SNTI_TRANSLATION_STATUS SntiTranslateWriteBuffer(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslateSynchronizeCache(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslateTestUnitReady(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslateFormatUnit(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslateLogSense(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

VOID SntiTranslateSupportedLogPages(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

VOID SntiTranslateTemperature(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

VOID SntiTranslateInfoExceptions(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslateModeSense(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    BOOLEAN supportsVwc
);

VOID SntiCreateControlModePage(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10
);

VOID SntiHardCodeCacheModePage(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10
);

VOID SntiCreatePowerConditionControlModePage(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10
);

VOID SntiCreateInformationalExceptionsControlModePage(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10
);

SNTI_TRANSLATION_STATUS SntiReturnAllModePages(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10,
    BOOLEAN supportsVwc
);

SNTI_TRANSLATION_STATUS SntiTranslateModeSelect(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    BOOLEAN supportsVwc
);

SNTI_TRANSLATION_STATUS SntiTranslateModeData(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT16 paramListLength,
    BOOLEAN isModeSelect10,
    BOOLEAN supportsVwc
);

VOID SntiCreateModeDataHeader(
    PNVME_SRB_EXTENSION pSrbExt,
    PMODE_PARAMETER_BLOCK *ppModeParamBlock,
    PUINT16 pModeDataLength,
    UINT16 blockDescLength,
    BOOLEAN modeSense10
);

VOID SntiCreateModeParameterDescBlock(
    PNVME_LUN_EXTENSION pLunExt,
    PMODE_PARAMETER_BLOCK pModeParamBlock,
    PUINT16 pModeDataLength
);

VOID SntiTranslateSglToPrp(
    PNVME_SRB_EXTENSION pSrbExt,
    PSTOR_SCATTER_GATHER_LIST pSgl
);

SNTI_STATUS SntiValidateLbaAndLength(
    PNVME_LUN_EXTENSION pLunExtension,
    PNVME_SRB_EXTENSION pSrbExtension,
    UINT64 lba,
    UINT32 length
);

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
);

VOID SntiMapGenericCommandStatus(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    UINT8 genericCommandStatus
);

VOID SntiMapCommandSpecificStatus(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    UINT8 commandSpecificStatus
);

VOID SntiMapMediaErrors(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    UINT8 mediaError 
);

SNTI_STATUS GetLunExtension(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION *ppLunExt
);

VOID SntiBuildGetFeaturesCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT8 featureIdentifier
);

VOID SntiBuildSetFeaturesCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT8 featureIdentifier,
    UINT32 dword11
);

VOID SntiBuildGetLogPageCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT8 logIdentifier
);

VOID SntiBuildFirmwareImageDownloadCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT32 dword10,
    UINT32 dword11
);

VOID SntiBuildFirmwareActivateCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT32 dword10
);

VOID SntiBuildFlushCmd(
    PNVME_SRB_EXTENSION pSrbExt
);

VOID SntiBuildFormatNvmCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT8 protectionType
);

VOID SntiBuildSecuritySendReceiveCmd(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    UINT8 opcode,
    UINT32 transferLength,
    UINT16 secProtocolSp,
    UINT8 secProtocol
);

VOID SntiMapInternalErrorStatus(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    SNTI_STATUS status
);

SNTI_TRANSLATION_STATUS SntiTranslateLogSenseResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry
);

SNTI_TRANSLATION_STATUS SntiTranslatePersReserveInResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
    );

SNTI_TRANSLATION_STATUS SntiTranslatePersReserveOutResponseOut(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
    );

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
);


SNTI_TRANSLATION_STATUS SntiTranslateInformationalExceptionsResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    UINT16 allocLength
);

SNTI_STATUS SntiTranslateTemperatureResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry,
    UINT16 allocLength
);

SNTI_TRANSLATION_STATUS SntiTranslateModeSenseResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb,
#else
    PSCSI_REQUEST_BLOCK pSrb,
#endif
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry
);

VOID SntiTranslateCachingModePageResponse(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVME_LUN_EXTENSION pLunExt,
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry,
    UINT16 allocLength,
    UINT8 longLbaAccepted,
    UINT8 disableBlockDesc,
    BOOLEAN modeSense10
);

VOID SntiTranslateReturnAllModePagesResponse(
    PNVME_SRB_EXTENSION pSrbExt,
    PNVMe_COMPLETION_QUEUE_ENTRY pCQEntry,
    BOOLEAN modeSense10
);

SNTI_TRANSLATION_STATUS SntiTranslateStartStopUnitResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

SNTI_TRANSLATION_STATUS SntiTranslateWriteBufferResponse(
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
);

VOID SntiDpcRoutine(
    IN PSTOR_DPC  pDpc,
    IN PVOID  pHwDeviceExtension,
    IN PVOID  pSystemArgument1,
    IN PVOID  pSystemArgument2
);

PVOID SntiAllocatePhysicallyContinguousBuffer(
    PNVME_SRB_EXTENSION pSrbExt,
    UINT32 bufferSize
);

SNTI_TRANSLATION_STATUS SntiValidateNacaSetting(
    PNVME_DEVICE_EXTENSION pDevExt,
#if (NTDDI_VERSION > NTDDI_WIN7)
    PSTORAGE_REQUEST_BLOCK pSrb
#else
    PSCSI_REQUEST_BLOCK pSrb
#endif
    );

#endif /* __NVME_SNTI_H__ */
