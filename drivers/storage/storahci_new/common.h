/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    common.h

Abstract:

    Common functions that might be moved to port lib

Notes:

Revision History:

        Michael Xing (xiaoxing),  December 2009
--*/

#pragma once

#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning(disable:4214) // bit field types other than int
#pragma warning(disable:4201) // nameless struct/union

#define ATA_BLOCK_SIZE                      512     //0x200

#define ATA_INQUIRYDATA_SIZE                0x3C  // contains 1 Version Descriptor for 1667

#define DEFAULT_DSM_DATA_SIZE_CAP_IN_BLOCKS 0x8

// per ATA spec, the SectorCount value range is 1 - 0xffff. 0 means this entry is not valid.
#define MAX_ATA_LBA_RANGE_SECTOR_COUNT_VALUE 0xFFFF

// Max of LBAs that can be represented using 28 bits.
#define MAX_28BIT_LBA   (1 << 28)


#define IDE_COMMAND_READ_FPDMA_QUEUED           0x60
#define IDE_COMMAND_WRITE_FPDMA_QUEUED          0x61

#define NVC_ATA_NV_CACHE_COMMAND                0xB6

#define IDE_FEATURE_INVALID                     0xFF


//
// ATA function code
//

// 0x100 - 0x1FF indicate ATA commands
#define ATA_FUNCTION_ATA_COMMAND        0x100
#define ATA_FUNCTION_ATA_IDENTIFY       0x101
#define ATA_FUNCTION_ATA_READ           0x102
#define ATA_FUNCTION_ATA_WRITE          0x103
#define ATA_FUNCTION_ATA_FLUSH          0x104
#define ATA_FUNCTION_ATA_SMART          0x105

// 0x200 - 0x2FF indicate ATAPI commands
#define ATA_FUNCTION_ATAPI_COMMAND      0x200
#define ATA_FUNCTION_REQUEST_SENSE      0x201 

//
// ATA flags
//

/* defined in ntscsi.h
#define ATA_FLAGS_DRDY_REQUIRED         (1 << 0)
#define ATA_FLAGS_DATA_IN               (1 << 1)
#define ATA_FLAGS_DATA_OUT              (1 << 2)
#define ATA_FLAGS_48BIT_COMMAND         (1 << 3)
#define ATA_FLAGS_USE_DMA               (1 << 4)
#define ATA_FLAGS_NO_MULTIPLE           (1 << 5)
*/

#define ATA_FLAGS_RETURN_RESULTS        (1 << 6)    // task file should be copied back into SenseInfoBuffer
#define ATA_FLAGS_HIGH_PRIORITY         (1 << 7)    // the request should be processed as a high priority request
#define ATA_FLAGS_NEW_CDB               (1 << 8)    // new CDB in SrbExtension should be issued to device rather than CDB in Srb
#define ATA_FLAGS_COMPLETE_SRB          (1 << 9)    // indicates the Srb should be completed, AhciCompleteRequest will not send command from SrbExtension.

//
// helper macros
//
#define IsAtaCommand(AtaFunction)       (AtaFunction & ATA_FUNCTION_ATA_COMMAND)
#define IsAtapiCommand(AtaFunction)     (AtaFunction & ATA_FUNCTION_ATAPI_COMMAND)
#define IsRequestSenseSrb(AtaFunction)  (AtaFunction == ATA_FUNCTION_REQUEST_SENSE)

#define IsReturnResults(AtaFlags)       (AtaFlags & ATA_FLAGS_RETURN_RESULTS)
#define IsNewCdbUsed(AtaFlags)          (AtaFlags & ATA_FLAGS_NEW_CDB) 
#define Is48BitCommand(AtaFlags)        (AtaFlags & ATA_FLAGS_48BIT_COMMAND) 
#define IsHighPriorityCommand(AtaFlags) (AtaFlags & ATA_FLAGS_HIGH_PRIORITY)
#define IsDmaCommand(AtaFlags)          (AtaFlags & ATA_FLAGS_USE_DMA)
#define SrbShouldBeCompleted(AtaFlags)  (AtaFlags & ATA_FLAGS_COMPLETE_SRB)


#define CLRMASK(x, mask)     ((x) &= ~(mask));
#define SETMASK(x, mask)     ((x) |=  (mask));


//
// task file register contents
//
#include <pshpack1.h>
typedef struct _ATAREGISTERS {

    // ATA features/error register
    UCHAR bFeaturesReg;

    // sector count
    UCHAR bSectorCountReg;

    // block number (7:0)
    UCHAR bSectorNumberReg;

    // cylinder number (7:0) or LBA (15:8) 
    UCHAR bCylLowReg;

    // cylinder number (15:8) or LBA (23:16) 
    UCHAR bCylHighReg;

    // device/Head and LBA (27:24)
    UCHAR bDriveHeadReg;

    // command/status register
    UCHAR bCommandReg;

    // Reserved for future use. Shall be 0
    UCHAR bReserved;

} ATAREGISTERS, *PATAREGISTERS;
#include <poppack.h>

//
// task file (to hold 48 bit)
//
typedef struct _ATA_TASK_FILE {
    ATAREGISTERS Current;
    ATAREGISTERS Previous;
} ATA_TASK_FILE, *PATA_TASK_FILE;

// helper functions for setting TaskFile fields
#define SetFeaturesReg(reg, val)      (reg->bFeaturesReg = val)
#define SetSectorCount(reg, val)      (reg->bSectorCountReg = val)
#define SetSectorNumber(reg, val)     (reg->bSectorNumberReg = val)
#define SetCylinderLow(reg, val)      (reg->bCylLowReg = val)
#define SetCylinderHigh(reg, val)     (reg->bCylHighReg = val)
#define SetDeviceReg(reg, val)        (reg->bDriveHeadReg |= (0xA0 | val)) // only have Device0 for SATA
#define SetCommandReg(reg, val)       (reg->bCommandReg = val)

//
// Device type
//
typedef enum  {
    DeviceUnknown = 0,
    DeviceIsAta,
    DeviceIsAtapi,
    DeviceNotExist
} ATA_DEVICE_TYPE;

//
// addressing mode
//
typedef enum {
    UnknownMode = 0,
    LbaMode,
    Lba48BitMode
} ATA_ADDRESS_TRANSLATION;


typedef struct _ATA_DEVICE_PARAMETERS {

    ATA_DEVICE_TYPE AtaDeviceType;  // device type (ata, atapi etc)
    UCHAR           ScsiDeviceType; // Scsi device type from inquiry data; Ata device: DIRECT_ACCESS_DEVICE

    UCHAR           MaximumLun;

    UCHAR           MaxDeviceQueueDepth;  // number of requests the device can handle at a time

    struct {
        ULONG   RemovableMedia: 1;
        ULONG   SystemPoweringDown: 1;
        ULONG   FuaSupported: 1;
        ULONG   FuaSucceeded: 1;

    } StateFlags;

    ULONG   BytesPerLogicalSector;
    ULONG   BytesPerPhysicalSector;
    ULONG   BytesOffsetForSectorAlignment;

    // Geometry
    ATA_ADDRESS_TRANSLATION AddressTranslation;

    LARGE_INTEGER           MaxLba;

    // Strings used to construct device ids, +1 for 'NULL'
    UCHAR   VendorId[40 + 1];
    UCHAR   RevisionId[8 + 1];
    UCHAR   SerialNumber[20 + 1];

    // max number of blocks that can be transferred for DSM - Trim command
    ULONG   DsmCapBlockCount;

} ATA_DEVICE_PARAMETERS, *PATA_DEVICE_PARAMETERS;

typedef struct _ATA_LBA_RANGE {
    ULONGLONG   StartSector:48;
    ULONGLONG   SectorCount:16;
} ATA_LBA_RANGE, *PATA_LBA_RANGE;

typedef struct _ATA_TRIM_CONTEXT {
#if (NTDDI_VERSION > NTDDI_WIN7)	
    // Block Descriptor for UNMAP request
    PUNMAP_BLOCK_DESCRIPTOR BlockDescriptors;
#endif
    // Block Descriptor count for UNMAP request
    ULONG   BlockDescrCount;

    // count of ATA Lba Range entries can be sent by one Trim command
    ULONG   MaxLbaRangeEntryCountPerCmd; 

    // count of ATA Lba Range entries need for an UNMAP request 
    //    max Block Descirptors carried by one Unmap command: 0xFFFE; max ATA Lba Ranges need for a Unmap Block Descriptor: 0xFFFFFFFF/0xFFFF
    //    ULONG is big enough to hold the value.
    ULONG   NeededLbaRangeEntryCount;

    // the length of allocated. Need to cache the value for calling AhciFreeDmaBuffer
    ULONG   AllocatedBufferLength;

    // count of ATA Lba Range entries that already be processed.
    ULONG   ProcessedLbaRangeEntryCount;

    // current Index of the input UNMAP Block Descriptors
    ULONG   BlockDescrIndex;

#if (NTDDI_VERSION > NTDDI_WIN7)
    // current UNMAP Block Descriptor being processed
    UNMAP_BLOCK_DESCRIPTOR  CurrentBlockDescr;
#endif
} ATA_TRIM_CONTEXT, *PATA_TRIM_CONTEXT;


__inline
BOOLEAN
IsAtapiDevice(
    __in PATA_DEVICE_PARAMETERS DeviceParameters
    ) 
{
    return (DeviceParameters->AtaDeviceType == DeviceIsAtapi);
}

__inline
BOOLEAN
IsAtaDevice(
    __in PATA_DEVICE_PARAMETERS DeviceParameters
    ) 
{
    return (DeviceParameters->AtaDeviceType == DeviceIsAta);
}

__inline
BOOLEAN
IsRemovableMedia(
    __in PATA_DEVICE_PARAMETERS DeviceParameters
    ) 
{
    return (DeviceParameters->StateFlags.RemovableMedia == 1);
}

       

__inline
BOOLEAN
Support48Bit(
    __in PATA_DEVICE_PARAMETERS DeviceParameters
    ) 
{
    return (DeviceParameters->AddressTranslation == Lba48BitMode);
}


__inline
ULONG
BytesPerLogicalSector(
    __in PATA_DEVICE_PARAMETERS DeviceParameters
    )
{
    return DeviceParameters->BytesPerLogicalSector;
}

__inline
ULONG64
MaxUserAddressableLba(
    __in PATA_DEVICE_PARAMETERS DeviceParameters
    ) 
{
    return (DeviceParameters->MaxLba.QuadPart);
}


__inline
ULONG64
GetLbaFromCdb(
    __in PCDB  Cdb,
    __in ULONG CdbLength
    )
{
    LARGE_INTEGER lba;

    lba.QuadPart = 0;

    if (CdbLength == 0x10) {
        // 16 bytes CDB
        REVERSE_BYTES_QUAD((&lba.QuadPart), Cdb->CDB16.LogicalBlock);
    } else {
        lba.LowPart = (ULONG)(Cdb->CDB10.LogicalBlockByte3 |
                              Cdb->CDB10.LogicalBlockByte2 << 8 |
                              Cdb->CDB10.LogicalBlockByte1 << 16 |
                              Cdb->CDB10.LogicalBlockByte0 << 24 );
        lba.HighPart = 0;
    }

    return (ULONG64)lba.QuadPart;
}

__inline
ULONG
GetSectorCountFromCdb(
    PCDB  Cdb,
    ULONG CdbLength
    )
{
    ULONG sectorCount = 0;

    if (CdbLength == 0x10) {
        // 16 byte cdb
        REVERSE_BYTES(&sectorCount, Cdb->CDB16.TransferLength);

    } else {
        sectorCount = (ULONG)(Cdb->CDB10.TransferBlocksMsb << 8 |
                              Cdb->CDB10.TransferBlocksLsb);
    }

    return sectorCount;
}

//
// Util routines
//
VOID
__inline
ByteSwap (
    __inout_bcount(Length) PUCHAR Buffer,
    __in                   ULONG Length
    )
/*++

Routine Description:

    Swap the bytes in the buffer.

Arguments:

    Buffer
    Length

Return Value:

    None.

--*/
{
    UCHAR temp;
    ULONG i;

    if (Length == 0) {
        return;
    }

    // swap the bytes
    for (i=0; i < Length-1; i+=2) {
        temp = Buffer[i];
        Buffer[i] = Buffer[i+1];
        Buffer[i+1] = temp;
    }

    return;
}

ULONG
__inline
RemoveTrailingBlanks (
    __inout_bcount(Length) __nullterminated PUCHAR Buffer,
    __in                   ULONG Length
    )
/*++

Routine Description:

    Remove trailing blanks from the buffer and null terminates it

Arguments:

    Buffer
    Length

Return Value:

    Returns the length of the string excluding the null character.

--*/
{
    LONG i;

    if (Length < 2) {
        return 0;
    }

    Buffer[Length-1] = '\0';

    // find the first non-space char from the end
    for (i=Length-2; i >= 0; i--) {

        if (Buffer[i] != ' ') {
            break;
        }
    }

    NT_ASSERT((i + 1) >= 0);

    Buffer[i+1] = '\0';

    return (i+1);
}

ULONG StorPortAllocateContiguousMemorySpecifyCacheNode(
  PVOID               HwDeviceExtension,
  SIZE_T              NumberOfBytes,
  PHYSICAL_ADDRESS    LowestAcceptableAddress,
  PHYSICAL_ADDRESS    HighestAcceptableAddress,
  PHYSICAL_ADDRESS    BoundaryAddressMultiple,
  MEMORY_CACHING_TYPE CacheType,
  NODE_REQUIREMENT    PreferredNode,
  PVOID               *BufferPointer
);

ULONG StorPortFreeContiguousMemorySpecifyCache(
  PVOID               HwDeviceExtension,
  PVOID               BaseAddress,
  SIZE_T              NumberOfBytes,
  MEMORY_CACHING_TYPE CacheType
);

#define AHCI_POOL_TAG               'ichA'  // "Ahci" - StorAHCI miniport driver

ULONG
__inline
AhciAllocateDmaBuffer (
    __in PVOID   AdapterExtension,
    __in ULONG   BufferLength,
    __out PVOID* Buffer
    )
{
    ULONG            status;
    PHYSICAL_ADDRESS minPhysicalAddress;
    PHYSICAL_ADDRESS maxPhysicalAddress;
    PHYSICAL_ADDRESS boundaryPhysicalAddress;

    minPhysicalAddress.QuadPart = 0;
    maxPhysicalAddress.QuadPart = 0x7FFFFFFF;   // (2GB - 1)
    boundaryPhysicalAddress.QuadPart = 0;

#if (NTDDI_VERSION > NTDDI_WIN7)
    status = StorPortAllocateContiguousMemorySpecifyCacheNode(AdapterExtension,
                                                              BufferLength,
                                                              minPhysicalAddress,
                                                              maxPhysicalAddress,
                                                              boundaryPhysicalAddress,
                                                              MmCached,
                                                              MM_ANY_NODE_OK,
                                                              Buffer);
#else
	status = StorPortAllocatePool(AdapterExtension,
								  BufferLength,
								  AHCI_POOL_TAG,
								  Buffer);
#endif
    return status;
}

ULONG
__inline
AhciFreeDmaBuffer (
    __in PVOID      AdapterExtension,
    __in ULONG_PTR  BufferLength,
    __out PVOID     Buffer
    )
{
    ULONG   status;
#if (NTDDI_VERSION > NTDDI_WIN7)	
    status = StorPortFreeContiguousMemorySpecifyCache(AdapterExtension,
                                                      Buffer,
                                                      BufferLength,
                                                      MmCached);
#else
	UNREFERENCED_PARAMETER(BufferLength);
	status = StorPortFreePool(AdapterExtension,
							  Buffer);
#endif	
    return status;
}

__inline
BOOLEAN
DmaSafeAtapiCommand (
    __in UCHAR CdbCommand
    )
{
    ULONG i;
    BOOLEAN useDma;
    static const UCHAR dmaCommandList[] =
        {SCSIOP_READ6,
         SCSIOP_READ,
         SCSIOP_READ_CD,
         SCSIOP_READ_CD_MSF,
         SCSIOP_READ12,
         SCSIOP_WRITE6,
         SCSIOP_WRITE,
         SCSIOP_WRITE12,
         SCSIOP_WRITE_VERIFY,
         SCSIOP_WRITE_VERIFY12,
         0};

    useDma = FALSE;
    i = 0;

    while (dmaCommandList[i]) {
        if (CdbCommand == dmaCommandList[i]) {
            useDma = TRUE;
            break;
        }

        i++;
    }

    return useDma;
}


struct AHCI_CHANNEL_EXTENSION;   
typedef struct _AHCI_CHANNEL_EXTENSION
               AHCI_CHANNEL_EXTENSION,
               *PAHCI_CHANNEL_EXTENSION;

ULONG
SCSItoATA(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
SrbConvertToATAPICommand(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtapiCommonRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtapiInquiryRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtapiModeSenseRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtapiModeSelectRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtapiGesnRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
SrbConvertToATACommand(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaReadWriteRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaVerifyRequest(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaModeSenseRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaModeSelectRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

__drv_aliasesMem
ULONG
AtaReadCapacityRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
InquiryComplete(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaInquiryRequest(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaStartStopUnitRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaTestUnitReadyRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaMediumRemovalRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaFlushCommandRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaPassThroughRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaUnmapRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaSecurityProtocolRequest (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
AtaReportLunsCommand(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PVOID Context
    );

UCHAR
AtaMapError(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb,
    __in BOOLEAN FUAcommand
    );

VOID
UpdateDeviceParameters(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID
DeviceInitAtapiIds(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PINQUIRYDATA InquiryData
    );

VOID
AhciPortIdentifyDevice(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
  );

ULONG
IOCTLtoATA(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
SmartVersion(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

BOOLEAN
  FillClippedSGL(
    __in    PSTOR_SCATTER_GATHER_LIST SourceSgl,
    __inout PSTOR_SCATTER_GATHER_LIST LocalSgl,
    __in    ULONG BytesLeft );

ULONG
SmartIdentifyData(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
SmartGeneric(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );

ULONG
NVCacheGeneric(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
    );


#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4214)
#pragma warning(default:4201)
#endif

