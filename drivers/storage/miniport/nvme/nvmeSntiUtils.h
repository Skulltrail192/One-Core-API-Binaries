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
 * File: nvmeSntitiUtils.h
 */

#ifndef __SNTI_UTILS_H__
#define __SNTI_UTILS_H__

/* MACROs to extrace infor from SRBs and CDBs */
#if (NTDDI_VERSION > NTDDI_WIN7)
#define GET_SRB_EXTENSION(pSrb)     (SrbGetMiniportContext((PVOID)pSrb))
#define GET_OPCODE(pSrb)            (((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[0])
#define GET_DATA_BUFFER(pSrb)       (SrbGetDataBuffer((PVOID)pSrb))
#define GET_DATA_LENGTH(pSrb)       (SrbGetDataTransferLength((PVOID)pSrb))
#define SET_DATA_LENGTH(pSrb, len)  (SrbSetDataTransferLength((PVOID)pSrb, len))
#define GET_PATH_ID(pSrb)           (SrbGetPathId((PVOID)pSrb))
#define GET_TARGET_ID(pSrb)         (SrbGetTargetId((PVOID)pSrb))
#define GET_LUN_ID(pSrb)            (SrbGetLun((PVOID)pSrb))
#define GET_CDB_LENGTH(pSrb)        (SrbGetCdbLength((PVOID)pSrb))

/* Extract fields from CDBs at offsets */
#define GET_U8_FROM_CDB(pSrb, index)   (((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[index])

#define GET_U16_FROM_CDB(pSrb, index) ((((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[index]   << 8) | \
                                       (((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[index+1] << 0))

#define GET_U24_FROM_CDB(pSrb, index) ((((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[index]   << 16)| \
                                       (((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[index+1] << 8) | \
                                       (((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[index+2] << 0))

#define GET_U32_FROM_CDB(pSrb, index) ((((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[index]   << 24)| \
                                       (((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[index+1] << 16)| \
                                       (((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[index+2] << 8) | \
                                       (((PCDB)SrbGetCdb((PVOID)pSrb))->AsByte[index+3] << 0))
#else
#define GET_SRB_EXTENSION(pSrb)      (pSrb)->SrbExtension
#define GET_OPCODE(pSrb)             (pSrb)->Cdb[0]
#define GET_DATA_BUFFER(pSrb)        (pSrb)->DataBuffer
#define GET_DATA_LENGTH(pSrb)        (pSrb)->DataTransferLength
#define SET_DATA_LENGTH(pSrb, len)   ((pSrb)->DataTransferLength = len)
#define GET_PATH_ID(pSrb)            (pSrb)->PathId
#define GET_TARGET_ID(pSrb)          (pSrb)->TargetId
#define GET_LUN_ID(pSrb)             (pSrb)->Lun
#define GET_CDB_LENGTH(pSrb)         (pSrb)->CdbLength

/* Extract fields from CDBs at offsets */
#define GET_U8_FROM_CDB(pSrb, index)   ((pSrb)->Cdb[index] << 0)

#define GET_U16_FROM_CDB(pSrb, index) (((pSrb)->Cdb[index]     <<  8) | \
                                       ((pSrb)->Cdb[index + 1] <<  0))

#define GET_U24_FROM_CDB(pSrb, index) (((pSrb)->Cdb[index] << 16)     | \
                                       ((pSrb)->Cdb[index + 1] <<  8) | \
                                       ((pSrb)->Cdb[index + 2] <<  0))

#define GET_U32_FROM_CDB(pSrb, index) (((pSrb)->Cdb[index]     << 24) | \
                                       ((pSrb)->Cdb[index + 1] << 16) | \
                                       ((pSrb)->Cdb[index + 2] <<  8) | \
                                       ((pSrb)->Cdb[index + 3] <<  0))
#endif
/* Inquiry Helper Macros */
#define GET_INQ_EVPD_BIT(pSrb)                                          \
            ((GET_U8_FROM_CDB(pSrb, INQUIRY_EVPD_BYTE_OFFSET) &         \
              INQUIRY_EVPD_BIT_MASK) ? TRUE : FALSE)

#define GET_INQ_PAGE_CODE(pSrb)                                         \
            (GET_U8_FROM_CDB(pSrb, INQUIRY_PAGE_CODE_BYTE_OFFSET))

#define GET_INQ_ALLOC_LENGTH(pSrb)                                      \
            (GET_U16_FROM_CDB(pSrb, INQUIRY_CDB_ALLOCATION_LENGTH_OFFSET))

/* Report LUNs Helper Macros */
#define GET_REPORT_LUNS_ALLOC_LENGTH(pSrb)                              \
            (GET_U32_FROM_CDB(pSrb, REPORT_LUNS_CDB_ALLOC_LENGTH_OFFSET))

/* Read Capacity Helper Macros */
#define GET_READ_CAP_16_ALLOC_LENGTH(pSrb)                              \
            (GET_U32_FROM_CDB(pSrb, READ_CAP_16_CDB_ALLOC_LENGTH_OFFSET))

/* Request Sense Helper Macros */
#define GET_REQUEST_SENSE_ALLOC_LENGTH(pSrb)                            \
            (GET_U8_FROM_CDB(pSrb, REQUEST_SENSE_CDB_ALLOC_LENGTH_OFFSET))

/* Generic SCSI Helper Macros */
#define GET_VALUE(cdb, index, len)   (cdb->cmnd[index] & len)
#define GET_LENGTH(cdb)              ((cdb->cmnd[7] << 8) | cdb->cmnd[8])

#define GET_DPO(cdb, index)          (GET_VALUE(cdb, index, 0x10) >> 4)
#define GET_FUA(cdb, index)          (GET_VALUE(cdb, index, 0x08) >> 3)
#define GET_FUA_NV(cdb, index)       (GET_VALUE(cdb, index, 0x02) >> 1)
#define GET_PROTECT(cdb, index)      (GET_VALUE(cdb, index, 0xE0) >> 5)
#define GET_GROUP_NUMBER(cdb, index) (GET_VALUE(cdb, index, 0x1F))

#define GET_DATA(cdb_field_p, cdb, index1, index2)                      \
                 cdb_field_p.dpo          = GET_DPO(cdb, index1);       \
                 cdb_field_p.fua          = GET_FUA(cdb, index1);       \
                 cdb_field_p.fua_nv       = GET_FUA_NV(cdb, index1);    \
                 cdb_field_p.protect      = GET_PROTECT(cdb, index1);   \
                 cdb_field_p.group_number = GET_GROUP_NUMBER(cdb, index2);

#define BYTE_SWAP_WORD(word) (((word & 0xFF)       << 8) |          \
                              ((word & 0xFF00)     >> 8))

#define BYTE_SWAP_DWORD(dword) (((dword & 0xFF)       << 24) |          \
                                ((dword & 0xFF00)     << 8)  |          \
                                ((dword & 0xFF0000)   >> 8)  |          \
                                ((dword & 0xFF000000) >> 24))

#define BYTE_SWAP_KEY(key) (((key & 0xFF)               << 56) |  \
                            ((key & 0xFF00)             << 40) |  \
                            ((key & 0xFF0000)           << 24) |  \
                            ((key & 0xFF000000)          << 8) |  \
                            ((key & 0xFF00000000)        >> 8) |  \
                            ((key & 0xFF0000000000)     >> 24) |  \
                            ((key & 0xFF000000000000)   >> 40) |  \
                            ((key & 0xFF00000000000000) >> 56))
#endif /* __SNTI_UTILS_H__ */
