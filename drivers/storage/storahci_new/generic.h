/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    generic.h

Abstract:

    holder of header files

Notes:

Revision History:

        Michael Xing (xiaoxing),  December 2009
--*/


// common header files
#include <ntddk.h>
#include <ata.h>
#include <storport.h>

#include "ntddscsi.h"
#define DEVICE_TYPE ULONG
#include "ntddstor.h"



// storahci header files
#include "common.h"
#include "ahci.h"
#include "entrypts.h"
#include "pnppower.h"
#include "hbastat.h"
#include "io.h"
#include "util.h"

//
// Data structures for SMART drive fault prediction.
//
// GETVERSIONINPARAMS contains the data returned from the
// Get Driver Version function.
//

#include <pshpack1.h>
typedef struct _GETVERSIONINPARAMS {
        UCHAR    bVersion;               // Binary driver version.
        UCHAR    bRevision;              // Binary driver revision.
        UCHAR    bReserved;              // Not used.
        UCHAR    bIDEDeviceMap;          // Bit map of IDE devices.
        ULONG   fCapabilities;          // Bit mask of driver capabilities.
        ULONG   dwReserved[4];          // For future use.
} GETVERSIONINPARAMS, *PGETVERSIONINPARAMS, *LPGETVERSIONINPARAMS;
#include <poppack.h>

//
// Valid values for the bCommandReg member of IDEREGS.
//

#define ATAPI_ID_CMD    0xA1            // Returns ID sector for ATAPI.
#define ID_CMD          0xEC            // Returns ID sector for ATA.
#define SMART_CMD       0xB0            // Performs SMART cmd.
                                        // Requires valid bFeaturesReg,
                                        // bCylLowReg, and bCylHighReg

//
// Cylinder register defines for SMART command
//

#define SMART_CYL_LOW   0x4F
#define SMART_CYL_HI    0xC2

//
// Bits returned in the fCapabilities member of GETVERSIONINPARAMS
//

#define CAP_ATA_ID_CMD          1       // ATA ID command supported
#define CAP_ATAPI_ID_CMD        2       // ATAPI ID command supported
#define CAP_SMART_CMD           4       // SMART commannds supported

//
// SENDCMDINPARAMS contains the input parameters for the
// Send Command to Drive function.
//

#include <pshpack1.h>
typedef struct _SENDCMDINPARAMS {
        ULONG   cBufferSize;             // Buffer size in bytes
        ATAREGISTERS irDriveRegs;        // Structure with drive register values.
        UCHAR    bDriveNumber;           // Physical drive number to send command to.
        UCHAR    bReserved[3];           // Reserved for future expansion.
        ULONG    dwReserved[4];          // For future use.
        UCHAR    bBuffer[1];             // Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;
#include <poppack.h>

//
// Status returned from driver
//

#include <pshpack1.h>
typedef struct _DRIVERSTATUS {
        UCHAR    bDriverError;           // Error code from driver,
                                                                // or 0 if no error.
        UCHAR    bIDEError;                      // Contents of IDE Error register.
                                                                // Only valid when bDriverError
                                                                // is SMART_IDE_ERROR.
        UCHAR    bReserved[2];           // Reserved for future expansion.
        ULONG   dwReserved[2];          // Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;
#include <poppack.h>

//
// bDriverError values
//

#define SMART_NO_ERROR          0       // No error
#define SMART_IDE_ERROR         1       // Error from IDE controller
#define SMART_INVALID_FLAG      2       // Invalid command flag
#define SMART_INVALID_COMMAND   3       // Invalid command byte
#define SMART_INVALID_BUFFER    4       // Bad buffer (null, invalid addr..)
#define SMART_INVALID_DRIVE     5       // Drive number not valid
#define SMART_INVALID_IOCTL     6       // Invalid IOCTL
#define SMART_ERROR_NO_MEM      7       // Could not lock user's buffer
#define SMART_INVALID_REGISTER  8       // Some IDE Register not valid
#define SMART_NOT_SUPPORTED     9       // Invalid cmd flag set
#define SMART_NO_IDE_DEVICE     10      // Cmd issued to device not present
                                        // although drive number is valid
//
// SMART sub commands for execute offline diags
//
#define SMART_OFFLINE_ROUTINE_OFFLINE       0
#define SMART_SHORT_SELFTEST_OFFLINE        1
#define SMART_EXTENDED_SELFTEST_OFFLINE     2
#define SMART_ABORT_OFFLINE_SELFTEST        127
#define SMART_SHORT_SELFTEST_CAPTIVE        129
#define SMART_EXTENDED_SELFTEST_CAPTIVE     130

#include <pshpack1.h>
typedef struct _SENDCMDOUTPARAMS {
        ULONG                   cBufferSize;            // Size of bBuffer in bytes
        DRIVERSTATUS            DriverStatus;           // Driver status structure.
        UCHAR                   bBuffer[1];             // Buffer of arbitrary length in which to store the data read from the                                                                                  // drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;
#include <poppack.h>


#define READ_ATTRIBUTE_BUFFER_SIZE  512
#define IDENTIFY_BUFFER_SIZE        512
#define READ_THRESHOLD_BUFFER_SIZE  512
#define SMART_LOG_SECTOR_SIZE       512

//
// Feature register defines for SMART "sub commands"
//

#define READ_ATTRIBUTES         0xD0
#define READ_THRESHOLDS         0xD1
#define ENABLE_DISABLE_AUTOSAVE 0xD2
#define SAVE_ATTRIBUTE_VALUES   0xD3
#define EXECUTE_OFFLINE_DIAGS   0xD4
#define SMART_READ_LOG          0xD5
#define SMART_WRITE_LOG         0xd6
#define ENABLE_SMART            0xD8
#define DISABLE_SMART           0xD9
#define RETURN_SMART_STATUS     0xDA
#define ENABLE_DISABLE_AUTO_OFFLINE 0xDB

/* SCSI SPC/SBC opcodes not defined by Storport.h (WDK) */
#define SCSIOP_COMPARE_AND_WRITE                   0x89
#define SCSIOP_SECURITY_PROTOCOL_IN                0xA2
#define SCSIOP_SECURITY_PROTOCOL_OUT               0xB5
#define SCSIOP_UNMAP                               0x42
#define SCSIOP_WRITE_LONG16                        0x9F

#define IDE_STATUS_ERROR   0x01
#define IDE_ERROR_DATA_ERROR   0x40

//
// Features for IDE_COMMAND_DATA_SET_MANAGEMENT
//
#define IDE_DSM_FEATURE_TRIM 0x0001 //bit 0 of WORD

//
// IDE error definitions
//
#define IDE_ERROR_BAD_BLOCK          0x80
#define IDE_ERROR_CRC_ERROR          IDE_ERROR_BAD_BLOCK
#define IDE_ERROR_DATA_ERROR         0x40
#define IDE_ERROR_MEDIA_CHANGE       0x20
#define IDE_ERROR_ID_NOT_FOUND       0x10
#define IDE_ERROR_MEDIA_CHANGE_REQ   0x08
#define IDE_ERROR_COMMAND_ABORTED    0x04
#define IDE_ERROR_END_OF_MEDIA       0x02
#define IDE_ERROR_ILLEGAL_LENGTH     0x01
#define IDE_ERROR_ADDRESS_NOT_FOUND IDE_ERROR_ILLEGAL_LENGTH

#define FIXED_SENSE_DATA                           0x70

#define SET_DATA_LENGTH(pSrb, len)  (SrbSetDataTransferLength((PVOID)pSrb, len))

FORCEINLINE VOID
SrbSetDataTransferLength(
    IN PVOID Srb,
    IN ULONG DataTransferLength
    )
{
#if (NTDDI_VERSION > NTDDI_WIN7)	
    PSTORAGE_REQUEST_BLOCK srb = (PSTORAGE_REQUEST_BLOCK)Srb;

    if (srb->Function == SRB_FUNCTION_STORAGE_REQUEST_BLOCK)
    {
        srb->DataTransferLength = DataTransferLength;
    }
#else	
    ((PSCSI_REQUEST_BLOCK)Srb)->DataTransferLength = DataTransferLength;
#endif	
}