
/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    ata.h

Abstract:

    Defines the structures used by ATA port and the miniport drivers.

Authors:

Revision History:

--*/

#ifndef _NTATA_
#define _NTATA_

#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning(disable:4200) // zero-sized array in struct/union
#pragma warning(disable:4214) // bit field types other than int
#pragma warning(disable:4201) // nameless struct/union

//
// 11/7/2011: update to ACS3 - D2161r1b; and SATA 3.1 Gold version.
//

//
// IDENTIFY device data (response to 0xEC)
//
#pragma pack(push, id_device_data, 1)
typedef struct _IDENTIFY_DEVICE_DATA {

    struct {
        USHORT Reserved1 : 1;
        USHORT Retired3 : 1;
        USHORT ResponseIncomplete : 1;
        USHORT Retired2 : 3;
        USHORT FixedDevice : 1;                 // obsolete
        USHORT RemovableMedia : 1;              // obsolete
        USHORT Retired1 : 7;
        USHORT DeviceType : 1;
    } GeneralConfiguration;                     // word 0

    USHORT NumCylinders;                        // word 1, obsolete
    USHORT SpecificConfiguration;               // word 2
    USHORT NumHeads;                            // word 3, obsolete
    USHORT Retired1[2];
    USHORT NumSectorsPerTrack;                  // word 6, obsolete
    USHORT VendorUnique1[3];
    UCHAR  SerialNumber[20];                    // word 10-19
    USHORT Retired2[2];
    USHORT Obsolete1;
    UCHAR  FirmwareRevision[8];                 // word 23-26
    UCHAR  ModelNumber[40];                     // word 27-46
    UCHAR  MaximumBlockTransfer;                // word 47. 01h-10h = Maximum number of sectors that shall be transferred per interrupt on READ/WRITE MULTIPLE commands
    UCHAR  VendorUnique2;

    struct {
        USHORT FeatureSupported : 1;
        USHORT Reserved : 15;
    }TrustedComputing;                          // word 48

    struct {
        UCHAR CurrentLongPhysicalSectorAlignment : 2;
        UCHAR ReservedByte49 : 6;

        UCHAR DmaSupported : 1;
        UCHAR LbaSupported : 1;                 // Shall be set to one to indicate that LBA is supported.
        UCHAR IordyDisable : 1;
        UCHAR IordySupported : 1;
        UCHAR Reserved1 : 1;                    // Reserved for the IDENTIFY PACKET DEVICE command
        UCHAR StandybyTimerSupport : 1;
        UCHAR Reserved2 : 2;                    // Reserved for the IDENTIFY PACKET DEVICE command

        USHORT ReservedWord50;
    }Capabilities;                              // word 49-50

    USHORT ObsoleteWords51[2];

    USHORT TranslationFieldsValid : 3;            // word 53, bit 0 - Obsolete; bit 1 - words 70:64 valid; bit 2; word 88 valid
    USHORT Reserved3 : 5;
    USHORT FreeFallControlSensitivity : 8;

    USHORT NumberOfCurrentCylinders;            // word 54, obsolete
    USHORT NumberOfCurrentHeads;                // word 55, obsolete
    USHORT CurrentSectorsPerTrack;              // word 56, obsolete
    ULONG  CurrentSectorCapacity;               // word 57, word 58, obsolete

    UCHAR  CurrentMultiSectorSetting;           // word 59
    UCHAR  MultiSectorSettingValid : 1;
    UCHAR  ReservedByte59 : 3;
    UCHAR  SanitizeFeatureSupported : 1;
    UCHAR  CryptoScrambleExtCommandSupported : 1;
    UCHAR  OverwriteExtCommandSupported : 1;
    UCHAR  BlockEraseExtCommandSupported : 1;

    ULONG  UserAddressableSectors;              // word 60-61, for 28-bit commands

    USHORT ObsoleteWord62;

    USHORT MultiWordDMASupport : 8;             // word 63
    USHORT MultiWordDMAActive : 8;

    USHORT AdvancedPIOModes : 8;                // word 64. bit 0:1 - PIO mode supported
    USHORT ReservedByte64 : 8;

    USHORT MinimumMWXferCycleTime;              // word 65
    USHORT RecommendedMWXferCycleTime;          // word 66
    USHORT MinimumPIOCycleTime;                 // word 67
    USHORT MinimumPIOCycleTimeIORDY;            // word 68

    struct {
        USHORT Reserved : 2;
        USHORT NonVolatileWriteCache : 1;                   // All write cache is non-volatile
        USHORT ExtendedUserAddressableSectorsSupported : 1;
        USHORT DeviceEncryptsAllUserData : 1;
        USHORT ReadZeroAfterTrimSupported : 1;
        USHORT Optional28BitCommandsSupported : 1;
        USHORT IEEE1667 : 1;            // Reserved for IEEE 1667
        USHORT DownloadMicrocodeDmaSupported : 1;
        USHORT SetMaxSetPasswordUnlockDmaSupported : 1;
        USHORT WriteBufferDmaSupported : 1;
        USHORT ReadBufferDmaSupported : 1;
        USHORT DeviceConfigIdentifySetDmaSupported : 1;     // obsolete
        USHORT LPSAERCSupported : 1;                        // Long Physical Sector Alignment Error Reporting Control is supported.
        USHORT DeterministicReadAfterTrimSupported : 1;
        USHORT CFastSpecSupported : 1;
    }AdditionalSupported;                       // word 69

    USHORT ReservedWords70[5];                  // word 70 - reserved
                                                // word 71:74 - Reserved for the IDENTIFY PACKET DEVICE command

    //Word 75
    USHORT QueueDepth : 5;          //  Maximum queue depth - 1
    USHORT ReservedWord75 : 11;

    struct {
    // Word 76
        USHORT Reserved0    : 1;    // shall be set to 0
        USHORT SataGen1     : 1;    // Supports SATA Gen1 Signaling Speed (1.5Gb/s)
        USHORT SataGen2     : 1;    // Supports SATA Gen2 Signaling Speed (3.0Gb/s)
        USHORT SataGen3     : 1;    // Supports SATA Gen3 Signaling Speed (6.0Gb/s)

        USHORT Reserved1    : 4;

        USHORT NCQ          : 1;    // Supports the NCQ feature set
        USHORT HIPM         : 1;    // Supports HIPM
        USHORT PhyEvents    : 1;    // Supports the SATA Phy Event Counters log
        USHORT NcqUnload    : 1;    // Supports Unload while NCQ commands are outstanding

        USHORT NcqPriority  : 1;    // Supports NCQ priority information
        USHORT HostAutoPS   : 1;    // Supports Host Automatic Partial to Slumber transitions
        USHORT DeviceAutoPS : 1;    // Supports Device Automatic Partial to Slumber transitions
        USHORT ReadLogDMA   : 1;    // Supports READ LOG DMA EXT as equivalent to READ LOG EXT

    // Word 77
        USHORT Reserved2    : 1;                // shall be set to 0
        USHORT CurrentSpeed : 3;                // Coded value indicating current negotiated Serial ATA signal speed

        USHORT NcqStreaming : 1;                // Supports NCQ Streaming
        USHORT NcqQueueMgmt : 1;                // Supports NCQ Queue Management Command
        USHORT NcqReceiveSend : 1;              // Supports RECEIVE FPDMA QUEUED and SEND FPDMA QUEUED commands
        USHORT DEVSLPtoReducedPwrState : 1;

        USHORT Reserved3    : 8;
    }SerialAtaCapabilities;

    // Word 78
    struct {
        USHORT Reserved0    : 1;                //shall be set to 0
        USHORT NonZeroOffsets:1;                // Device supports non-zero buffer offsets in DMA Setup FIS
        USHORT DmaSetupAutoActivate:1;          // Device supports DMA Setup auto-activation
        USHORT DIPM         : 1;                // Device supports DIPM

        USHORT InOrderData  : 1;                // Device supports in-order data delivery
        USHORT HardwareFeatureControl:1;        // Hardware Feature Control is supported
        USHORT SoftwareSettingsPreservation:1;  // Device supports Software Settings Preservation
        USHORT NCQAutosense:1;                  // Supports NCQ Autosense

        USHORT DEVSLP               :1;         // Device supports link power state - device sleep
        USHORT HybridInformation    :1;         // Device supports Hybrid Information Feature (If the device does not support NCQ (word 76 bit 8 is 0), then this bit shall be cleared to 0.)

        USHORT Reserved1    : 6;
    }SerialAtaFeaturesSupported;

    // Word 79
    struct {
        USHORT Reserved0    : 1;                // shall be set to 0
        USHORT NonZeroOffsets:1;                // Non-zero buffer offsets in DMA Setup FIS enabled
        USHORT DmaSetupAutoActivate:1;          // DMA Setup auto-activation optimization enabled
        USHORT DIPM         : 1;                // DIPM enabled

        USHORT InOrderData  : 1;                // In-order data delivery enabled
        USHORT HardwareFeatureControl:1;        // Hardware Feature Control is enabled
        USHORT SoftwareSettingsPreservation:1;  // Software Settings Preservation enabled
        USHORT DeviceAutoPS : 1;                // Device Automatic Partial to Slumber transitions enabled

        USHORT DEVSLP               :1;         // link power state - device sleep is enabled
        USHORT HybridInformation    :1;         // Hybrid Information Feature is enabled

        USHORT Reserved1    : 6;
    }SerialAtaFeaturesEnabled;

    USHORT MajorRevision;                       // word 80. bit 5 - supports ATA5; bit 6 - supports ATA6; bit 7 - supports ATA7; bit 8 - supports ATA8-ACS; bit 9 - supports ACS-2;
    USHORT MinorRevision;                       // word 81. T13 minior version number

    struct {

        //
        // Word 82
        //
        USHORT SmartCommands : 1;           // The SMART feature set is supported
        USHORT SecurityMode : 1;            // The Security feature set is supported
        USHORT RemovableMediaFeature : 1;   // obsolete
        USHORT PowerManagement : 1;         // shall be set to 1
        USHORT Reserved1 : 1;               // PACKET feature set, set to 0 indicates not supported for ATA devices (only support for ATAPI devices)
        USHORT WriteCache : 1;              // The volatile write cache is supported
        USHORT LookAhead : 1;               // Read look-ahead is supported
        USHORT ReleaseInterrupt : 1;        // obsolete
        USHORT ServiceInterrupt : 1;        // obsolete
        USHORT DeviceReset : 1;             // Shall be cleared to zero to indicate that the DEVICE RESET command is not supported
        USHORT HostProtectedArea : 1;       // obsolete
        USHORT Obsolete1 : 1;
        USHORT WriteBuffer : 1;             // The WRITE BUFFER command is supported
        USHORT ReadBuffer : 1;              // The READ BUFFER command is supported
        USHORT Nop : 1;                     // The NOP command is supported
        USHORT Obsolete2 : 1;

        //
        // Word 83
        //
        USHORT DownloadMicrocode : 1;       // The DOWNLOAD MICROCODE command is supported
        USHORT DmaQueued : 1;               // obsolete
        USHORT Cfa : 1;                     // The CFA feature set is supported
        USHORT AdvancedPm : 1;              // The APM feature set is supported
        USHORT Msn : 1;                     // obsolete
        USHORT PowerUpInStandby : 1;        // The PUIS feature set is supported
        USHORT ManualPowerUp : 1;           // SET FEATURES subcommand is required to spin-up after power-up
        USHORT Reserved2 : 1;
        USHORT SetMax : 1;                  // obsolete
        USHORT Acoustics : 1;               // obsolete
        USHORT BigLba : 1;                  // The 48-bit Address feature set is supported
        USHORT DeviceConfigOverlay : 1;     // obsolete
        USHORT FlushCache : 1;              // Shall be set to one to indicate that the mandatory FLUSH CACHE command is supported
        USHORT FlushCacheExt : 1;           // The FLUSH CACHE EXT command is supported
        USHORT WordValid83 : 2;             // shall be 01b


        //
        // Word 84
        //
        USHORT SmartErrorLog : 1;           // SMART error logging is supported
        USHORT SmartSelfTest : 1;           // The SMART self-test is supported
        USHORT MediaSerialNumber : 1;       // Media serial number is supported
        USHORT MediaCardPassThrough : 1;    // obsolete
        USHORT StreamingFeature : 1;        // The Streaming feature set is supported
        USHORT GpLogging : 1;               // The GPL feature set is supported
        USHORT WriteFua : 1;                // The WRITE DMA FUA EXT and WRITE MULTIPLE FUA EXT commands are supported
        USHORT WriteQueuedFua : 1;          // obsolete
        USHORT WWN64Bit : 1;                // The 64-bit World wide name is supported
        USHORT URGReadStream : 1;           // obsolete
        USHORT URGWriteStream : 1;          // obsolete
        USHORT ReservedForTechReport : 2;
        USHORT IdleWithUnloadFeature : 1;   // The IDLE IMMEDIATE command with UNLOAD feature is supported
        USHORT WordValid : 2;               // shall be 01b

    }CommandSetSupport;

    struct {

        //
        // Word 85
        //
        USHORT SmartCommands : 1;           // The SMART feature set is enabled
        USHORT SecurityMode : 1;            // The Security feature set is enabled
        USHORT RemovableMediaFeature : 1;   // obsolete
        USHORT PowerManagement : 1;         // Shall be set to one to indicate that the mandatory Power Management feature set is supported
        USHORT Reserved1 : 1;               // Shall be cleared to zero to indicate that the PACKET feature set is not supported
        USHORT WriteCache : 1;              // The volatile write cache is enabled
        USHORT LookAhead : 1;               // Read look-ahead is enabled
        USHORT ReleaseInterrupt : 1;        // The release interrupt is enabled
        USHORT ServiceInterrupt : 1;        // The SERVICE interrupt is enabled
        USHORT DeviceReset : 1;             // Shall be cleared to zero to indicate that the DEVICE RESET command is not supported
        USHORT HostProtectedArea : 1;       // obsolete
        USHORT Obsolete1 : 1;
        USHORT WriteBuffer : 1;             // The WRITE BUFFER command is supported
        USHORT ReadBuffer : 1;              // The READ BUFFER command is supported
        USHORT Nop : 1;                     // The NOP command is supported
        USHORT Obsolete2 : 1;

        //
        // Word 86
        //
        USHORT DownloadMicrocode : 1;       // The DOWNLOAD MICROCODE command is supported
        USHORT DmaQueued : 1;               // obsolete
        USHORT Cfa : 1;                     // The CFA feature set is supported
        USHORT AdvancedPm : 1;              // The APM feature set is enabled
        USHORT Msn : 1;                     // obsolete
        USHORT PowerUpInStandby : 1;        // The PUIS feature set is enabled
        USHORT ManualPowerUp : 1;           // SET FEATURES subcommand is required to spin-up after power-up
        USHORT Reserved2 : 1;
        USHORT SetMax : 1;                  // obsolete
        USHORT Acoustics : 1;               // obsolete
        USHORT BigLba : 1;                  // The 48-bit Address features set is supported
        USHORT DeviceConfigOverlay : 1;     // obsolete
        USHORT FlushCache : 1;              // FLUSH CACHE command supported
        USHORT FlushCacheExt : 1;           // FLUSH CACHE EXT command supported
        USHORT Resrved3 : 1;
        USHORT Words119_120Valid : 1;       // Words 119..120 are valid

        //
        // Word 87
        //
        USHORT SmartErrorLog : 1;           // SMART error logging is supported
        USHORT SmartSelfTest : 1;           // SMART self-test supported
        USHORT MediaSerialNumber : 1;       // Media serial number is valid
        USHORT MediaCardPassThrough : 1;    // obsolete
        USHORT StreamingFeature : 1;        // obsolete
        USHORT GpLogging : 1;               // The GPL feature set is supported
        USHORT WriteFua : 1;                // The WRITE DMA FUA EXT and WRITE MULTIPLE FUA EXT commands are supported
        USHORT WriteQueuedFua : 1;          // obsolete
        USHORT WWN64Bit : 1;                // The 64-bit World wide name is supported
        USHORT URGReadStream : 1;           // obsolete
        USHORT URGWriteStream : 1;          // obsolete
        USHORT ReservedForTechReport : 2;
        USHORT IdleWithUnloadFeature : 1;   // The IDLE IMMEDIATE command with UNLOAD FEATURE is supported
        USHORT Reserved4 : 2;               // bit 14 shall be set to 1; bit 15 shall be cleared to 0

    }CommandSetActive;

    USHORT UltraDMASupport : 8;                 // word 88. bit 0 - UDMA mode 0 is supported ... bit 6 - UDMA mode 6 and below are supported
    USHORT UltraDMAActive  : 8;                 // word 88. bit 8 - UDMA mode 0 is selected ... bit 14 - UDMA mode 6 is selected

    struct {                                    // word 89
        USHORT TimeRequired : 15;
        USHORT ExtendedTimeReported : 1;
    } NormalSecurityEraseUnit;

    struct {                                    // word 90
        USHORT TimeRequired : 15;
        USHORT ExtendedTimeReported : 1;
    } EnhancedSecurityEraseUnit;

    USHORT CurrentAPMLevel : 8;                 // word 91
    USHORT ReservedWord91 : 8;

    USHORT MasterPasswordID;                    // word 92. Master Password Identifier

    USHORT HardwareResetResult;                 // word 93

    USHORT CurrentAcousticValue : 8;            // word 94. obsolete
    USHORT RecommendedAcousticValue : 8;

    USHORT StreamMinRequestSize;                // word 95
    USHORT StreamingTransferTimeDMA;            // word 96
    USHORT StreamingAccessLatencyDMAPIO;        // word 97
    ULONG  StreamingPerfGranularity;            // word 98, 99

    ULONG  Max48BitLBA[2];                      // word 100-103

    USHORT StreamingTransferTime;               // word 104. Streaming Transfer Time - PIO

    USHORT DsmCap;                              // word 105

    struct {
        USHORT LogicalSectorsPerPhysicalSector : 4;         // n power of 2: logical sectors per physical sector
        USHORT Reserved0 : 8;
        USHORT LogicalSectorLongerThan256Words : 1;
        USHORT MultipleLogicalSectorsPerPhysicalSector : 1;
        USHORT Reserved1 : 2;                               // bit 14 - shall be set to  1; bit 15 - shall be clear to 0
    } PhysicalLogicalSectorSize;                // word 106

    USHORT InterSeekDelay;                      //word 107.     Inter-seek delay for ISO 7779 standard acoustic testing
    USHORT WorldWideName[4];                    //words 108-111
    USHORT ReservedForWorldWideName128[4];      //words 112-115
    USHORT ReservedForTlcTechnicalReport;       //word 116
    USHORT WordsPerLogicalSector[2];            //words 117-118 Logical sector size (DWord)

    struct {
        USHORT ReservedForDrqTechnicalReport : 1;
        USHORT WriteReadVerify : 1;                 // The Write-Read-Verify feature set is supported
        USHORT WriteUncorrectableExt : 1;           // The WRITE UNCORRECTABLE EXT command is supported
        USHORT ReadWriteLogDmaExt : 1;              // The READ LOG DMA EXT and WRITE LOG DMA EXT commands are supported
        USHORT DownloadMicrocodeMode3 : 1;          // Download Microcode mode 3 is supported
        USHORT FreefallControl : 1;                 // The Free-fall Control feature set is supported
        USHORT SenseDataReporting : 1;              // Sense Data Reporting feature set is supported
        USHORT ExtendedPowerConditions : 1;         // Extended Power Conditions feature set is supported
        USHORT Reserved0 : 6;
        USHORT WordValid : 2;                       // shall be 01b
    }CommandSetSupportExt;                      //word 119

    struct {
        USHORT ReservedForDrqTechnicalReport : 1;
        USHORT WriteReadVerify : 1;                 // The Write-Read-Verify feature set is enabled
        USHORT WriteUncorrectableExt : 1;           // The WRITE UNCORRECTABLE EXT command is supported
        USHORT ReadWriteLogDmaExt : 1;              // The READ LOG DMA EXT and WRITE LOG DMA EXT commands are supported
        USHORT DownloadMicrocodeMode3 : 1;          // Download Microcode mode 3 is supported
        USHORT FreefallControl : 1;                 // The Free-fall Control feature set is enabled
        USHORT SenseDataReporting : 1;              // Sense Data Reporting feature set is enabled
        USHORT ExtendedPowerConditions : 1;         // Extended Power Conditions feature set is enabled
        USHORT Reserved0 : 6;
        USHORT Reserved1 : 2;                       // bit 14 - shall be set to  1; bit 15 - shall be clear to 0
    }CommandSetActiveExt;                       //word 120

    USHORT ReservedForExpandedSupportandActive[6];

    USHORT MsnSupport : 2;                      //word 127. obsolete
    USHORT ReservedWord127 : 14;

    struct {                                    //word 128
        USHORT SecuritySupported : 1;
        USHORT SecurityEnabled : 1;
        USHORT SecurityLocked : 1;
        USHORT SecurityFrozen : 1;
        USHORT SecurityCountExpired : 1;
        USHORT EnhancedSecurityEraseSupported : 1;
        USHORT Reserved0 : 2;
        USHORT SecurityLevel : 1;                   // Master Password Capability: 0 = High, 1 = Maximum
        USHORT Reserved1 : 7;
    } SecurityStatus;

    USHORT ReservedWord129[31];                 //word 129...159. Vendor specific

    struct {                                    //word 160
        USHORT MaximumCurrentInMA : 12;
        USHORT CfaPowerMode1Disabled : 1;
        USHORT CfaPowerMode1Required : 1;
        USHORT Reserved0 : 1;
        USHORT Word160Supported : 1;
    } CfaPowerMode1;

    USHORT ReservedForCfaWord161[7];                //Words 161-167

    USHORT NominalFormFactor : 4;                   //Word 168
    USHORT ReservedWord168 : 12;

    struct {                                        //Word 169
        USHORT SupportsTrim : 1;
        USHORT Reserved0    : 15;
    } DataSetManagementFeature;

    USHORT AdditionalProductID[4];                  //Words 170-173

    USHORT ReservedForCfaWord174[2];                //Words 174-175

    USHORT CurrentMediaSerialNumber[30];            //Words 176-205

    struct {                                        //Word 206
        USHORT Supported : 1;                           // The SCT Command Transport is supported
        USHORT Reserved0 : 1;                           // obsolete
        USHORT WriteSameSuported : 1;                   // The SCT Write Same command is supported
        USHORT ErrorRecoveryControlSupported : 1;       // The SCT Error Recovery Control command is supported
        USHORT FeatureControlSuported : 1;              // The SCT Feature Control command is supported
        USHORT DataTablesSuported : 1;                  // The SCT Data Tables command is supported
        USHORT Reserved1 : 6;
        USHORT VendorSpecific : 4;
    } SCTCommandTransport;

    USHORT ReservedWord207[2];                      //Words 207-208

    struct {                                        //Word 209
        USHORT AlignmentOfLogicalWithinPhysical: 14;
        USHORT Word209Supported: 1;                     // shall be set to 1
        USHORT Reserved0: 1;                            // shall be cleared to 0
    } BlockAlignment;

    USHORT WriteReadVerifySectorCountMode3Only[2];  //Words 210-211
    USHORT WriteReadVerifySectorCountMode2Only[2];  //Words 212-213

    struct {
        USHORT NVCachePowerModeEnabled: 1;
        USHORT Reserved0: 3;
        USHORT NVCacheFeatureSetEnabled: 1;
        USHORT Reserved1: 3;
        USHORT NVCachePowerModeVersion: 4;
        USHORT NVCacheFeatureSetVersion: 4;
    } NVCacheCapabilities;                  //Word 214. obsolete
    USHORT NVCacheSizeLSW;                  //Word 215. obsolete
    USHORT NVCacheSizeMSW;                  //Word 216. obsolete

    USHORT NominalMediaRotationRate;        //Word 217; value 0001h means non-rotating media.

    USHORT ReservedWord218;                 //Word 218

    struct {
        UCHAR NVCacheEstimatedTimeToSpinUpInSeconds;
        UCHAR Reserved;
    } NVCacheOptions;                       //Word 219. obsolete

    USHORT  WriteReadVerifySectorCountMode : 8;     //Word 220. Write-Read-Verify feature set current mode
    USHORT  ReservedWord220 : 8;

    USHORT  ReservedWord221;                //Word 221

    struct {                                //Word 222 Transport major version number
        USHORT  MajorVersion : 12;              // 0000h or FFFFh = device does not report version
        USHORT  TransportType : 4;
    } TransportMajorVersion;

    USHORT  TransportMinorVersion;          // Word 223

    USHORT  ReservedWord224[6];             // Word 224...229

    ULONG   ExtendedNumberOfUserAddressableSectors[2];  // Words 230...233 Extended Number of User Addressable Sectors

    USHORT  MinBlocksPerDownloadMicrocodeMode03;        // Word 234 Minimum number of 512-byte data blocks per Download Microcode mode 03h operation
    USHORT  MaxBlocksPerDownloadMicrocodeMode03;        // Word 235 Maximum number of 512-byte data blocks per Download Microcode mode 03h operation

    USHORT ReservedWord236[19];             // Word 236...254

    USHORT Signature : 8;                   //Word 255
    USHORT CheckSum : 8;

} IDENTIFY_DEVICE_DATA, *PIDENTIFY_DEVICE_DATA;
#pragma pack (pop, id_device_data)

//
// identify packet data (response to 0xA1)
//
#pragma pack (push, id_packet_data, 1)
typedef struct _IDENTIFY_PACKET_DATA {

    struct {
        USHORT PacketType : 2;              // 00b = 12 byte command packet; 01b = 16 byte command packet
        USHORT IncompleteResponse : 1;      // Incomplete response
        USHORT Reserved1 : 2;
        USHORT DrqDelay : 2;                // 00b = Device shall set DRQ to one within 3 ms of receiving PACKET command. 10b = Device shall set DRQ to one within 50 us of receiving PACKET command.
        USHORT RemovableMedia : 1;          // obsolete
        USHORT CommandPacketType : 5;       // This value follows the peripheral device type as defined in SPC-4 (e.g., 05h indicates a CD/DVD device).
        USHORT Reserved2 : 1;
        USHORT DeviceType : 2;              // 10b = ATAPI device
    }GeneralConfiguration;

    USHORT ResevedWord1;                    // word 1
    USHORT UniqueConfiguration;             // word 2
    USHORT ReservedWords3[7];               // word 3...9
    UCHAR  SerialNumber[20];                // word 10...19
    USHORT ReservedWords20[3];              // word 20...22
    UCHAR  FirmwareRevision[8];             // word 23...26
    UCHAR  ModelNumber[40];                 // word 27...46
    USHORT ReservedWords47[2];              // word 47...48

    struct {                                // word 49...50
        USHORT VendorSpecific : 8;
        USHORT DmaSupported : 1;
        USHORT LbaSupported : 1;                        // shall be set to 1
        USHORT IordyDisabled : 1;
        USHORT IordySupported : 1;
        USHORT Obsolete : 1;
        USHORT OverlapSupported : 1;                    // obsolete
        USHORT QueuedCommandsSupported : 1;             // obsolete
        USHORT InterleavedDmaSupported : 1;             // obsolete

        USHORT DeviceSpecificStandbyTimerValueMin : 1;  // Shall be set to one to indicate a device specific Standby timer value minimum.
        USHORT Obsolete1 : 1;
        USHORT ReservedWord50 : 12;
        USHORT WordValid : 2;                           // value shall be 01b

    } Capabilities;

    USHORT ObsoleteWords51[2];

    USHORT TranslationFieldsValid:3;        // word 53.  bit 0 - obsolete; bit 1 - words 64..70 are valid; bit 2 -  word 88 is valid
    USHORT Reserved3:13;

    USHORT ReservedWords54[8];              // word 54...61

    struct {                                // word 62
        USHORT UDMA0Supported : 1;
        USHORT UDMA1Supported : 1;
        USHORT UDMA2Supported : 1;
        USHORT UDMA3Supported : 1;
        USHORT UDMA4Supported : 1;
        USHORT UDMA5Supported : 1;
        USHORT UDMA6Supported : 1;
        USHORT MDMA0Supported : 1;
        USHORT MDMA1Supported : 1;
        USHORT MDMA2Supported : 1;
        USHORT DMASupported : 1;
        USHORT ReservedWord62 : 4;
        USHORT DMADIRBitRequired : 1;           // DMADIR bit in the PACKET command is required for DMA transfers
    } DMADIR;

    USHORT MultiWordDMASupport : 8;             // word 63
    USHORT MultiWordDMAActive : 8;

    USHORT AdvancedPIOModes : 8;                // word 64
    USHORT ReservedByte64 : 8;

    USHORT MinimumMWXferCycleTime;              // word 65. Minimum Multiword DMA transfer cycle time per word. Cycle time in nanoseconds
    USHORT RecommendedMWXferCycleTime;          // word 66. Manufacturer recommended Multiword DMA transfer cycle time. Cycle time in nanoseconds
    USHORT MinimumPIOCycleTime;                 // word 67. Minimum PIO transfer cycle time without flow control. Cycle time in nanoseconds
    USHORT MinimumPIOCycleTimeIORDY;            // word 68. Minimum PIO transfer cycle time with IORDY. Cycle time in nanoseconds

    USHORT ReservedWords69[2];                  // word 69...70

    USHORT BusReleaseDelay;                     // word 71. obsolete
    USHORT ServiceCommandDelay;                 // word 72. obsolete

    USHORT ReservedWords73[2];                  // word 73...74

    //Word 75. obsolete
    USHORT QueueDepth : 5;
    USHORT ReservedWord75 : 11;

    struct {
    // Word 76
        USHORT Reserved0    : 1;    //shall be set to 0
        USHORT SataGen1     : 1;    // The SATA Gen1 Signaling Speed (1.5Gb/s) is supported
        USHORT SataGen2     : 1;    // The SATA Gen2 Signaling Speed (3.0Gb/s) is supported
        USHORT SataGen3     : 1;    // The SATA Gen3 Signaling Speed (6.0Gb/s) is supported

        USHORT Reserved1    : 5;

        USHORT HIPM         : 1;    // Support HIPM
        USHORT PhyEvents    : 1;    // The SATA Phy Event Counters log is supported

        USHORT Reserved3    : 2;

        USHORT HostAutoPS   : 1;    // Supports Host Automatic Partial to Slumber transitions
        USHORT DeviceAutoPS : 1;    // Supports Device Automatic Partial to Slumber transitions
        USHORT Reserved4    : 1;

    // Word 77
        USHORT Reserved5     : 1;               //shall be set to 0
        USHORT CurrentSpeed : 3;                // Coded value indicating current negotiated Serial ATA signal speed

        USHORT SlimlineDeviceAttention:1;       // Supports Device Attention on slimline connected device
        USHORT HostEnvironmentDetect:1;         // Supports host environment detect
        USHORT Reserved     : 10;
    }SerialAtaCapabilities;

    // Word 78
    struct {
        USHORT Reserved0    : 1;                    //shall be set to 0
        USHORT Reserved1    : 2;
        USHORT DIPM         : 1;                    // Device initiated power management is supported

        USHORT Reserved2    : 1;
        USHORT AsynchronousNotification:1;          // Asynchronous notification supported
        USHORT SoftwareSettingsPreservation:1;      // The SSP feature set is supported

        USHORT Reserved3    : 9;
    }SerialAtaFeaturesSupported;

    // Word 79
    struct {
        USHORT Reserved0    : 1;                    // shall be set to 0
        USHORT Reserved1    : 2;
        USHORT DIPM         : 1;                    // Device initiated power management is enabled

        USHORT Reserved2    : 1;
        USHORT AsynchronousNotification:1;          // Asynchronous notification enabled
        USHORT SoftwareSettingsPreservation:1;      // The SSP feature set is enabled
        USHORT DeviceAutoPS : 1;                    // Device Automatic Partial to Slumber transitions enabled

        USHORT Reserved3    : 8;
    }SerialAtaFeaturesEnabled;

    USHORT MajorRevision;                   // word 80. 0000h or FFFFh = device does not report version; bit 0...4 obsolete; bit 5 - ATA5 is supported; bit 6 - ATA6 is supported; bit 7 - ATA7 is supported; bit 8 - ATA8-ACS is supported;
    USHORT MinorRevision;                   // word 81

    struct {                                // word 82...83
        USHORT SmartCommands : 1;               // Shall be cleared to zero to indicate that the SMART feature set is not supported
        USHORT SecurityMode : 1;                // The Security feature set is supported
        USHORT RemovableMedia : 1;              // obsolete
        USHORT PowerManagement : 1;             // The Power Management feature set supported
        USHORT PacketCommands : 1;              // Shall be set to one indicating the PACKET feature set is supported.
        USHORT WriteCache : 1;                  // The volatile write cache is supported
        USHORT LookAhead : 1;                   // Read look-ahead supported
        USHORT ReleaseInterrupt : 1;            // obsolete
        USHORT ServiceInterrupt : 1;            // obsolete
        USHORT DeviceReset : 1;                 // Shall be set to one to indicate that the DEVICE RESET command is supported
        USHORT HostProtectedArea : 1;           // obsolete
        USHORT Obsolete1 : 1;
        USHORT WriteBuffer : 1;                 // Shall be cleared to zero to indicate that the WRITE BUFFER command is not supported
        USHORT ReadBuffer : 1;                  // Shall be cleared to zero to indicate that the READ BUFFER command is not supported
        USHORT Nop : 1;                         // Shall be set to one to indicate that the NOP command is supported
        USHORT Obsolete2 : 1;

        USHORT DownloadMicrocode : 1;           // Shall be cleared to zero to indicate that the DOWNLOAD MICROCODE command is not supported
        USHORT Reserved1 : 2;
        USHORT AdvancedPm : 1;                  // The APM feature set is supported
        USHORT Msn : 1;                         // obsolete
        USHORT PowerUpInStandby : 1;            // The PUIS feature set is supported
        USHORT ManualPowerUp : 1;               // The SET FEATURES subcommand is required to spin-up after power-up
        USHORT Reserved2 : 1;
        USHORT SetMax : 1;                      // obsolete
        USHORT Reserved3 : 3;
        USHORT FlushCache : 1;                  // The FLUSH CACHE command is supported
        USHORT Reserved4 : 1;
        USHORT WordValid : 2;                   // shall be 01b
    } CommandSetSupport;

    struct {                                // word 84
        USHORT Reserved0 : 5;
        USHORT GpLogging : 1;                       // The GPL feature set is supported
        USHORT Reserved1 : 2;
        USHORT WWN64Bit  : 1;                       // shall be set to one to indicate that the mandator WWN is supported
        USHORT Reserved2 : 5;
        USHORT WordValid : 2;                       // shall be 01b
    }CommandSetSupportExt;


    struct {                                // word 85...86
        USHORT SmartCommands : 1;                   // Shall be cleared to zero to indicate that the SMART feature set is not supported
        USHORT SecurityMode : 1;                    // The Security feature set is enabled
        USHORT RemovableMedia : 1;                  // obsolete
        USHORT PowerManagement : 1;                 // Power Management feature set is enabled
        USHORT PacketCommands : 1;                  // Shall be set to one indicating the PACKET feature set is supported.
        USHORT WriteCache : 1;                      // The volatile write cache is enabled
        USHORT LookAhead : 1;                       // Read look-ahead is enabled
        USHORT ReleaseInterrupt : 1;                // obsolete
        USHORT ServiceInterrupt : 1;                // obsolete
        USHORT DeviceReset : 1;                     // Shall be set to one to indicate that the DEVICE RESET command is supported
        USHORT HostProtectedArea : 1;               // obsolete
        USHORT Obsolete1 : 1;
        USHORT WriteBuffer : 1;                     // Shall be cleared to zero to indicate that the WRITE BUFFER command is not supported
        USHORT ReadBuffer : 1;                      // Shall be cleared to zero to indicate that the READ BUFFER command is not supported
        USHORT Nop : 1;                             // Shall be set to one to indicate that the NOP command is supported
        USHORT Obsolete2 : 1;

        USHORT DownloadMicrocode : 1;               // Shall be cleared to zero to indicate that the DOWNLOAD MICROCODE command is not supported
        USHORT Reserved1 : 2;
        USHORT AdvancedPm : 1;                      // The APM feature set is enabled
        USHORT Msn : 1;                             // obsolete
        USHORT PowerUpInStandby : 1;                // The PUIS feature set is enabled
        USHORT ManualPowerUp : 1;                   // SET FEATURES subcommand required to spin-up after power-up
        USHORT Reserved2 : 1;
        USHORT SetMax : 1;                          // obsolete
        USHORT Reserved3 : 3;
        USHORT FlushCache : 1;                      // The FLUSH CACHE command is supported
        USHORT Reserved : 3;
    } CommandSetActive;

    struct {                                // word 87
        USHORT Reserved0 : 5;
        USHORT GpLogging : 1;                       // This bit is a copy of word 84 bit 5
        USHORT Reserved1 : 2;
        USHORT WWN64Bit  : 1;                       // shall be set to one to indicate that the mandator WWN is supported
        USHORT Reserved2 : 5;
        USHORT WordValid : 2;                       // shall be 01b
    }CommandSetActiveExt;

    USHORT UltraDMASupport : 8;             // word 88
    USHORT UltraDMAActive  : 8;

    USHORT TimeRequiredForNormalEraseModeSecurityEraseUnit;     // word 89
    USHORT TimeRequiredForEnhancedEraseModeSecurityEraseUnit;   // word 90
    USHORT CurrentAPMLevel;                 // word 91
    USHORT MasterPasswordID;                // word 92

    USHORT HardwareResetResult;             // word 93

    USHORT ReservedWords94[14];             // word 94...107

    USHORT WorldWideName[4];                // word 108...111

    USHORT ReservedWords112[13];            // word 112...124

    USHORT AtapiZeroByteCount;              // word 125

    USHORT ReservedWord126;                 // word 126. obsolete

    USHORT MsnSupport : 2;                  // word 127. obsolete
    USHORT ReservedWord127 : 14;

    USHORT SecurityStatus;                  // word 128

    USHORT VendorSpecific[31];              // Word 129...159
    USHORT ReservedWord160[16];             // Word 160...175. Reserved for assignment by the CompactFlash Association
    USHORT ReservedWord176[46];             // Word 176...221

    struct {                                //Word 222 Transport major version number.  0000h or FFFFh = device does not report version
        USHORT  MajorVersion : 12;
        USHORT  TransportType : 4;
    } TransportMajorVersion;

    USHORT  TransportMinorVersion;          // Word 223

    USHORT  ReservedWord224[31];            // Word 224...254

    USHORT Signature : 8;
    USHORT CheckSum : 8;

} IDENTIFY_PACKET_DATA, *PIDENTIFY_PACKET_DATA;
#pragma pack (pop, id_packet_data)

//
// Register FIS
//
#pragma pack (push, regfis, 1)
typedef struct _REGISTER_FIS {

    //
    // dword 0
    //
    UCHAR FisType;
    UCHAR Reserved0 : 7;
    UCHAR CmdReg : 1;
    UCHAR Command;
    UCHAR Features;

    //
    // dword 1
    //
    UCHAR SectorNumber;
    UCHAR CylinderLow;
    UCHAR CylinderHigh;
    UCHAR DeviceHead;

    //
    // dword 2
    //
    UCHAR SectorNumberExp;
    UCHAR CylinderLowExp;
    UCHAR CylinderHighExp;
    UCHAR FeaturesExp;

    //
    // dword 3
    //
    UCHAR SectorCount;
    UCHAR SectorCountExp;
    UCHAR Reserved2;
    UCHAR Control;

    //
    // dword 4
    //
    ULONG Reserved3;

}REGISTER_FIS, *PREGISTER_FIS;
#pragma pack (pop, regfis)

//
// ATAPI specific scsiops
//
#define ATAPI_MODE_SENSE        0x5A
#define ATAPI_MODE_SELECT       0x55
#define ATAPI_LS120_FORMAT_UNIT 0x24

//
// IDE driveSelect register bit for LBA mode
//
#define IDE_LBA_MODE   (1 << 6)

//
// IDE drive control definitions
//
#define IDE_DC_DISABLE_INTERRUPTS    0x02
#define IDE_DC_RESET_CONTROLLER      0x04
#define IDE_DC_REENABLE_CONTROLLER   0x00

//
// IDE status definitions
//
#define IDE_STATUS_ERROR             0x01
#define IDE_STATUS_INDEX             0x02
#define IDE_STATUS_CORRECTED_ERROR   0x04
#define IDE_STATUS_DRQ               0x08
#define IDE_STATUS_DSC               0x10
#define IDE_STATUS_DEVICE_FAULT      0x20
#define IDE_STATUS_DRDY              0x40
#define IDE_STATUS_IDLE              0x50
#define IDE_STATUS_BUSY              0x80

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
#define IDE_ERROR_ADDRESS_NOT_FOUND  IDE_ERROR_ILLEGAL_LENGTH


//
// IDE command definitions
//
#define IDE_COMMAND_NOP                         0x00
#define IDE_COMMAND_DATA_SET_MANAGEMENT         0x06
#define IDE_COMMAND_ATAPI_RESET                 0x08
#define IDE_COMMAND_READ                        0x20
#define IDE_COMMAND_READ_EXT                    0x24
#define IDE_COMMAND_READ_DMA_EXT                0x25
#define IDE_COMMAND_READ_DMA_QUEUED_EXT         0x26
#define IDE_COMMAND_READ_MULTIPLE_EXT           0x29
#define IDE_COMMAND_READ_LOG_EXT                0x2f
#define IDE_COMMAND_WRITE                       0x30
#define IDE_COMMAND_WRITE_EXT                   0x34
#define IDE_COMMAND_WRITE_DMA_EXT               0x35
#define IDE_COMMAND_WRITE_DMA_QUEUED_EXT        0x36
#define IDE_COMMAND_WRITE_MULTIPLE_EXT          0x39
#define IDE_COMMAND_WRITE_DMA_FUA_EXT           0x3D
#define IDE_COMMAND_WRITE_DMA_QUEUED_FUA_EXT    0x3E
#define IDE_COMMAND_WRITE_LOG_EXT               0x3f
#define IDE_COMMAND_VERIFY                      0x40
#define IDE_COMMAND_VERIFY_EXT                  0x42
#define IDE_COMMAND_WRITE_LOG_DMA_EXT           0x57
#define IDE_COMMAND_TRUSTED_NON_DATA            0x5B
#define IDE_COMMAND_TRUSTED_RECEIVE             0x5C
#define IDE_COMMAND_TRUSTED_RECEIVE_DMA         0x5D
#define IDE_COMMAND_TRUSTED_SEND                0x5E
#define IDE_COMMAND_TRUSTED_SEND_DMA            0x5F
#define IDE_COMMAND_READ_FPDMA_QUEUED           0x60        // NCQ Read command
#define IDE_COMMAND_WRITE_FPDMA_QUEUED          0x61        // NCQ Write command
#define IDE_COMMAND_NCQ_NON_DATA                0x63        // NCQ Non-Data command
#define IDE_COMMAND_SEND_FPDMA_QUEUED           0x64        // NCQ Send command
#define IDE_COMMAND_RECEIVE_FPDMA_QUEUED        0x65        // NCQ Receive command
#define IDE_COMMAND_SET_DATE_AND_TIME           0x77        // optional 48bit command
#define IDE_COMMAND_EXECUTE_DEVICE_DIAGNOSTIC   0x90
#define IDE_COMMAND_SET_DRIVE_PARAMETERS        0x91
#define IDE_COMMAND_DOWNLOAD_MICROCODE          0x92        // Optional 28bit command
#define IDE_COMMAND_DOWNLOAD_MICROCODE_DMA      0x93        // Optional 28bit command
#define IDE_COMMAND_ATAPI_PACKET                0xA0
#define IDE_COMMAND_ATAPI_IDENTIFY              0xA1
#define IDE_COMMAND_SMART                       0xB0
#define IDE_COMMAND_READ_LOG_DMA_EXT            0xB1
#define IDE_COMMAND_SANITIZE_DEVICE             0xB4
#define IDE_COMMAND_READ_MULTIPLE               0xC4
#define IDE_COMMAND_WRITE_MULTIPLE              0xC5
#define IDE_COMMAND_SET_MULTIPLE                0xC6
#define IDE_COMMAND_READ_DMA                    0xC8
#define IDE_COMMAND_WRITE_DMA                   0xCA
#define IDE_COMMAND_WRITE_DMA_QUEUED            0xCC
#define IDE_COMMAND_WRITE_MULTIPLE_FUA_EXT      0xCE
#define IDE_COMMAND_GET_MEDIA_STATUS            0xDA
#define IDE_COMMAND_DOOR_LOCK                   0xDE
#define IDE_COMMAND_DOOR_UNLOCK                 0xDF
#define IDE_COMMAND_STANDBY_IMMEDIATE           0xE0
#define IDE_COMMAND_IDLE_IMMEDIATE              0xE1
#define IDE_COMMAND_CHECK_POWER                 0xE5
#define IDE_COMMAND_SLEEP                       0xE6
#define IDE_COMMAND_FLUSH_CACHE                 0xE7
#define IDE_COMMAND_FLUSH_CACHE_EXT             0xEA
#define IDE_COMMAND_IDENTIFY                    0xEC
#define IDE_COMMAND_MEDIA_EJECT                 0xED
#define IDE_COMMAND_SET_FEATURE                 0xEF
#define IDE_COMMAND_SECURITY_SET_PASSWORD       0xF1
#define IDE_COMMAND_SECURITY_UNLOCK             0xF2
#define IDE_COMMAND_SECURITY_ERASE_PREPARE      0xF3
#define IDE_COMMAND_SECURITY_ERASE_UNIT         0xF4
#define IDE_COMMAND_SECURITY_FREEZE_LOCK        0xF5
#define IDE_COMMAND_SECURITY_DISABLE_PASSWORD   0xF6
#define IDE_COMMAND_NOT_VALID                   0xFF

//
// IDE Set Transfer Mode
//
#define IDE_SET_DEFAULT_PIO_MODE(mode)      ((UCHAR) 1)     // disable I/O Ready
#define IDE_SET_ADVANCE_PIO_MODE(mode)      ((UCHAR) ((1 << 3) | (mode)))
#define IDE_SET_SWDMA_MODE(mode)            ((UCHAR) ((1 << 4) | (mode)))
#define IDE_SET_MWDMA_MODE(mode)            ((UCHAR) ((1 << 5) | (mode)))
#define IDE_SET_UDMA_MODE(mode)             ((UCHAR) ((1 << 6) | (mode)))

//
// Set features parameter list
//
#define IDE_FEATURE_ENABLE_WRITE_CACHE          0x2
#define IDE_FEATURE_SET_TRANSFER_MODE           0x3
#define IDE_FEATURE_ENABLE_PUIS                 0x6
#define IDE_FEATURE_PUIS_SPIN_UP                0x7
#define IDE_FEATURE_ENABLE_SATA_FEATURE         0x10
#define IDE_FEATURE_DISABLE_MSN                 0x31
#define IDE_FEATURE_DISABLE_REVERT_TO_POWER_ON  0x66
#define IDE_FEATURE_DISABLE_WRITE_CACHE         0x82
#define IDE_FEATURE_DISABLE_PUIS                0x86
#define IDE_FEATURE_DISABLE_SATA_FEATURE        0x90
#define IDE_FEATURE_ENABLE_MSN                  0x95

//
// SATA Features Sector Count parameter list
//

#define IDE_SATA_FEATURE_NON_ZERO_DMA_BUFFER_OFFSET         0x1
#define IDE_SATA_FEATURE_DMA_SETUP_FIS_AUTO_ACTIVATE        0x2
#define IDE_SATA_FEATURE_DEVICE_INITIATED_POWER_MANAGEMENT  0x3
#define IDE_SATA_FEATURE_GUARANTEED_IN_ORDER_DELIVERY       0x4
#define IDE_SATA_FEATURE_ASYNCHRONOUS_NOTIFICATION          0x5
#define IDE_SATA_FEATURE_SOFTWARE_SETTINGS_PRESERVATION     0x6
#define IDE_SATA_FEATURE_DEVICE_AUTO_PARTIAL_TO_SLUMBER     0x7
#define IDE_SATA_FEATURE_ENABLE_HARDWARE_FEATURE_CONTROL    0x8
#define IDE_SATA_FEATURE_DEVSLP                             0x9
#define IDE_SATA_FEATURE_HYBRID_INFORMATION                 0xa

//
// SMART sub command list
//
#define IDE_SMART_READ_ATTRIBUTES               0xD0
#define IDE_SMART_READ_THRESHOLDS               0xD1
#define IDE_SMART_ENABLE_DISABLE_AUTOSAVE       0xD2
#define IDE_SMART_SAVE_ATTRIBUTE_VALUES         0xD3
#define IDE_SMART_EXECUTE_OFFLINE_DIAGS         0xD4
#define IDE_SMART_READ_LOG                      0xD5
#define IDE_SMART_WRITE_LOG                     0xD6
#define IDE_SMART_ENABLE                        0xD8
#define IDE_SMART_DISABLE                       0xD9
#define IDE_SMART_RETURN_STATUS                 0xDA
#define IDE_SMART_ENABLE_DISABLE_AUTO_OFFLINE   0xDB

//
// Features for IDE_COMMAND_DATA_SET_MANAGEMENT
//
#define IDE_DSM_FEATURE_TRIM                  0x0001    //bit 0 of WORD

//
// NCQ sub command list
//

// SubCommand of IDE_COMMAND_NCQ_NON_DATA
#define IDE_NCQ_NON_DATA_ABORT_NCQ_QUEUE                0x00
#define IDE_NCQ_NON_DATA_DEADLINE_HANDLING              0x01
#define IDE_NCQ_NON_DATA_HYBRID_CHANGE_BY_SIZE          0x02    // this subCommand has been renamed to Hybrid Demote by Size.
#define IDE_NCQ_NON_DATA_HYBRID_DEMOTE_BY_SIZE          0x02
#define IDE_NCQ_NON_DATA_HYBRID_CHANGE_BY_LBA_RANGE     0x03
#define IDE_NCQ_NON_DATA_HYBRID_CONTROL                 0x04

// SubCommand of IDE_COMMAND_SEND_FPDMA_QUEUED
#define IDE_NCQ_SEND_DATA_SET_MANAGEMENT                0x00
#define IDE_NCQ_SEND_HYBRID_EVICT                       0x01

//
// "Hybrid Information Field Bits" structure definition. e.g. Auxiliary(23:16) field.
//
#pragma pack(push, ata_hybrid_info_fields, 1)
typedef union _ATA_HYBRID_INFO_FIELDS {

    struct {
        UCHAR   HybridPriority  : 4;
        UCHAR   Reserved0       : 1;
        UCHAR   InfoValid       : 1;
        UCHAR   Reserved1       : 2;
    };

    UCHAR   AsUchar;

} ATA_HYBRID_INFO_FIELDS, *PATA_HYBRID_INFO_FIELDS;
#pragma pack (pop, ata_hybrid_info_fields)

//
// "Device Set Password " structure definition.
//
#pragma pack(push, setp_device_data, 1)
typedef struct _DEVICE_SET_PASSWORD {

    struct {
        USHORT PasswordIdentifier:1;            // (0 - set user password, 1 = set master password)
        USHORT Reserved1 : 7;
        USHORT MasterPasswordCapability : 1;    // (0 = High, 1 = Maximum)
        USHORT Reserved2 : 7;
    } ControlWord;
    USHORT  Password[16];                       // Bytes 2-33
    USHORT  MasterPasswordIdentifier;           // byte 34
    USHORT  Reserved[238];                      // Bytes 36-511    

} DEVICE_SET_PASSWORD, *PDEVICE_SET_PASSWORD;
#pragma pack (pop, setp_device_data)

#define ATA_DEVICE_SET_PASSWORD_MASTER       0x01
#define ATA_DEVICE_SET_PASSWORD_USER         0x00


//
// Features and definitions for IDE_COMMAND_READ_LOG_EXT, aka general purpose logging
//

/*
    Table A.2 from ACS-4 , snapshot as of June 2010

    Log     Address     Log     Name    Feature     Set     R/W     Access
    =======================================================================
    00h     Log directory, see A.2 and A.3 N/A RO GPL,SL
    01h Summary SMART Error Log, see A.16 SMART RO SL
    02h Comprehensive SMART Error Log, see A.4 SMART RO SL
    03h Extended Comprehensive SMART Error Log,
    see A.7
    SMART RO GPL
    04h Device Statistics, see A.5 N/A RO GPL,SL
    05h Reserved
    06h SMART Self-Test Log, see A.15 SMART RO SL
    07h Extended SMART Self-Test Log, see A.8 SMART RO GPL
    08h Reserved N/A Reserved
    09h Selective Self-Test Log, see A.14 SMART R/W SL
    0Ah-0Ch Reserved N/A Reserved
    0Dh LPS Mis-alignment log, see A.10 LPS RO GPL,SL
    0Eh-0Fh Reserved
    10h NCQ Command Error, see A.11 NCQ RO GPL
    11h SATA Phy Event Counters, see A.13 N/A RO GPL
    12h-17h Reserved for Serial ATA N/A Reserved
    18h-1Fh Reserved N/A Reserved
    20h Obsolete
    21h Write Stream Error Log, see A.17 Streaming RO GPL
    22h Read Stream Error Log, see A.12 Streaming RO GPL
    23h Obsolete
    24h-7Fh Reserved N/A Reserved
    80h-9Fh Host Vendor Specific, see A.9 SMART R/W GPL,SL
    A0h-DFh Device Vendor Specific, see A.6 SMART VS GPL,SL
    E0h SCT Command/Status, see 8.1 SCT R/W GPL,SL
    E1h SCT Data Transfer, see 8.1 SCT R/W GPL,SL
    E2h-FFh Reserved N/A

    Table A3 - General Purpose Log Directory

    Word Description
    ===============================================
    0 General Purpose Logging Version (word)
    1 Number of log pages at log address 01h (word)
    2 Number of log pages at log address 02h (word)
    3 Number of log pages at log address 03h (word)
    4 Number of log pages at log address 04h (word)

    128 Number of log pages at log address 80h (word)
    129 Number of log pages at log address 81h (word)

    255 Number of log pages at log address FFh (word)
*/

//
// GP Log directory resides at the address 0
//

#define IDE_GP_LOG_DIRECTORY_ADDRESS                0x00

#define IDE_GP_SUMMARY_SMART_ERROR                  0x01    // Access: SMART Logging
#define IDE_GP_COMPREHENSIVE_SMART_ERROR            0x02    // Access: SMART Logging
#define IDE_GP_EXTENDED_COMPREHENSIVE_SMART_ERROR   0x03
#define IDE_GP_LOG_DEVICE_STATISTICS_ADDRESS        0x04
#define IDE_GP_SMART_SELF_TEST                      0x06    // Access: SMART Logging
#define IDE_GP_EXTENDED_SMART_SELF_TEST             0x07
#define IDE_GP_LOG_POWER_CONDITIONS                 0x08
#define IDE_GP_SELECTIVE_SELF_TEST                  0x09    // Access: SMART Logging
#define IDE_GP_DEVICE_STATISTICS_NOTIFICATION       0x0A
#define IDE_GP_PENDING_DEFECTS                      0x0C
#define IDE_GP_LPS_MISALIGNMENT                     0x0D

#define IDE_GP_LOG_NCQ_COMMAND_ERROR_ADDRESS        0x10
#define IDE_GP_LOG_PHY_EVENT_COUNTER_ADDRESS        0x11
#define IDE_GP_LOG_NCQ_NON_DATA_ADDRESS             0x12
#define IDE_GP_LOG_NCQ_SEND_RECEIVE_ADDRESS         0x13
#define IDE_GP_LOG_HYBRID_INFO_ADDRESS              0x14
#define IDE_GP_LOG_REBUILD_ASSIST                   0x15
#define IDE_GP_LOG_LBA_STATUS                       0x19

#define IDE_GP_LOG_WRITE_STREAM_ERROR               0x21
#define IDE_GP_LOG_READ_STREAM_ERROR                0x22
#define IDE_GP_LOG_CURRENT_DEVICE_INTERNAL_STATUS   0x24
#define IDE_GP_LOG_SAVED_DEVICE_INTERNAL_STATUS     0x25

#define IDE_GP_LOG_IDENTIFY_DEVICE_DATA_ADDRESS     0x30

#define IDE_GP_LOG_SCT_COMMAND_STATUS               0xE0
#define IDE_GP_LOG_SCT_DATA_TRANSFER                0xE1

#define IDE_GP_LOG_SECTOR_SIZE                      0x200   // 512 bytes - independent of the device media sector / block size GP log sector size is always 512
#define IDE_GP_LOG_VERSION                          0x0001

#define IDE_GP_LOG_SUPPORTED_PAGES                  0x00    // common value used by multiple Log Address if multiple pages supported


//
// Log page for Identify Device Data log
//
#define IDE_GP_LOG_IDENTIFY_DEVICE_DATA_SUPPORTED_CAPABILITIES_PAGE     0x03
#define IDE_GP_LOG_IDENTIFY_DEVICE_DATA_SATA_PAGE                       0x08

#pragma pack(push, identify_device_data_log_page_header, 1)
typedef struct _IDENTIFY_DEVICE_DATA_LOG_PAGE_HEADER {
    ULONGLONG   RevisionNumber : 16;    // Shall be set to 0001h
    ULONGLONG   PageNumber : 8;         // Shall be set to the page number
    ULONGLONG   Reserved : 40;
} IDENTIFY_DEVICE_DATA_LOG_PAGE_HEADER, *PIDENTIFY_DEVICE_DATA_LOG_PAGE_HEADER;
#pragma pack (pop, identify_device_data_log_page_header)


#pragma pack(push, identify_device_data_log_page_supported_capabilities, 1)
typedef struct _IDENTIFY_DEVICE_DATA_LOG_PAGE_SUPPORTED_CAPABILITIES {
    IDENTIFY_DEVICE_DATA_LOG_PAGE_HEADER Header;        // byte 0..7

    struct {
        ULONGLONG WRV : 1;
        ULONGLONG WriteUncorrectable : 1;
        ULONGLONG GplDma : 1;
        ULONGLONG DmMode3 : 1;
        ULONGLONG FreeFall : 1;
        ULONGLONG SenseData : 1;
        ULONGLONG EPC : 1;
        ULONGLONG SmartErrorLogging : 1;

        ULONGLONG SmartSelfTest : 1;
        ULONGLONG Reserved9 : 1;
        ULONGLONG Streaming : 1;
        ULONGLONG GPL : 1;
        ULONGLONG WriteFuaExt : 1;
        ULONGLONG Unload : 1;
        ULONGLONG DownloadMicrocode : 1;
        ULONGLONG Reserved15ForCFA : 1;

        ULONGLONG APM : 1;
        ULONGLONG PUIS : 1;
        ULONGLONG SpinUp : 1;
        ULONGLONG Reserved19 : 1;
        ULONGLONG Cmd48Bit : 1;
        ULONGLONG Reserved21 : 1;
        ULONGLONG FlushCacheExt : 1;
        ULONGLONG Smart : 1;

        ULONGLONG VolatileWriteCache : 1;
        ULONGLONG ReadLookahead : 1;
        ULONGLONG Reserved26 : 1;
        ULONGLONG WriteBuffer : 1;
        ULONGLONG ReadBuffer : 1;
        ULONGLONG NOP : 1;
        ULONGLONG Reserved30 : 1;
        ULONGLONG RZAT : 1;

        ULONGLONG Cmd28bit : 1;
        ULONGLONG DownloadMicrocodeDma : 1;
        ULONGLONG Reserved34 : 1;
        ULONGLONG WriteBufferDma : 1;
        ULONGLONG ReadBufferDma : 1;
        ULONGLONG Reserved37 : 1;
        ULONGLONG LpsMisalignmentReporting : 1;
        ULONGLONG DRAT : 1;

        ULONGLONG Reserved40ForCFA : 1;
        ULONGLONG AmaxAddr : 1;
        ULONGLONG SetEpcPowerSource : 1;
        ULONGLONG LowPowerStandby : 1;
        ULONGLONG DSN : 1;
        ULONGLONG RequestSenseDeviceFault: 1;
        ULONGLONG Reserved : 17;

        ULONGLONG Valid : 1;
    } SupportedCapabilities;                            // byte 8..15

    struct {
        ULONGLONG DmMinTransferSize : 16;

        ULONGLONG DmMaxTransferSize : 16;

        ULONGLONG DmOffsetsImmediateSupported : 1;  // subcommand 03h is supported.
        ULONGLONG DmImmediateSupported : 1;         // subcommand 07h is supported.
        ULONGLONG DmOffsetsDeferredSupported : 1;   // subcommand 0Eh and 0Fh are supported.
        ULONGLONG Reserved : 28;

        ULONGLONG Valid : 1;
    } DownloadMicrocodeCapabilities;                    // byte 16..23

    struct {
        ULONGLONG Rate : 16;

        ULONGLONG Reserved : 47;

        ULONGLONG Valid : 1;
    } NominalMediaRotationRate;                         // byte 24..31

    struct {
        ULONGLONG Factor : 4;

        ULONGLONG Reserved : 59;

        ULONGLONG Valid : 1;
    } NominalFormFactor;                                // byte 32..39

    struct {
        ULONGLONG Count : 32;

        ULONGLONG Reserved : 31;

        ULONGLONG Valid : 1;
    } WRVSectorCountMode3;                              // byte 40..47

    struct {
        ULONGLONG Count : 32;

        ULONGLONG Reserved : 31;

        ULONGLONG Valid : 1;
    } WRVSectorCountMode2;                              // byte 48..55

    struct {
        ULONGLONG Name;

        ULONGLONG Reserved : 63;

        ULONGLONG Valid : 1;
    } WorldWideName;                                    // byte 56..71

    struct {
        ULONGLONG TrimSupported : 1;

        ULONGLONG Reserved : 62;

        ULONGLONG Valid : 1;
    } DataSetManagement;                                // byte 72..79

    UCHAR  Reserved[432];                               // byte 80..511

} IDENTIFY_DEVICE_DATA_LOG_PAGE_SUPPORTED_CAPABILITIES, *PIDENTIFY_DEVICE_DATA_LOG_PAGE_SUPPORTED_CAPABILITIES;
#pragma pack (pop, identify_device_data_log_page_supported_capabilities)


//
// "Device Internal Status Data Log" structure definition.
//
#pragma pack(push, gplog_device_data, 1)
typedef struct _SAVED_DEVICE_INTERNAL_STATUS_LOG {

    UCHAR   LogAddress;                         // Byte 0
    UCHAR   Reserved0[3];                       // Byte 1-3
    ULONG   OrganizationID;                     // Bytes 4-7, 31:24 reserved, 23:0 - IEEE OUI
    USHORT  Area1LastLogPage;                   // Bytes 8-9
    USHORT  Area2LastLogPage;                   // Bytes 10-11
    USHORT  Area3LastLogPage;                   // Bytes 12-13
    UCHAR   Reserved2[368];                     // Bytes 14-381
    UCHAR   SavedDataAvailable;                 // Byte 382
    UCHAR   GenerationNumber;                   // Byte 383
    UCHAR   ReasonIdentifier[128];              // Bytes 384-511

} SAVED_DEVICE_INTERNAL_STATUS_LOG, *PSAVED_DEVICE_INTERNAL_STATUS_LOG;
#pragma pack (pop, gplog_device_data)


//
// Log page for Device Statistics log
//

#define IDE_GP_LOG_DEVICE_STATISTICS_GENERAL_PAGE           0x01
#define IDE_GP_LOG_DEVICE_STATISTICS_FREE_FALL_PAGE         0x02
#define IDE_GP_LOG_DEVICE_STATISTICS_ROTATING_MEDIA_PAGE    0x03
#define IDE_GP_LOG_DEVICE_STATISTICS_GENERAL_ERROR_PAGE     0x04
#define IDE_GP_LOG_DEVICE_STATISTICS_TEMPERATURE_PAGE       0x05
#define IDE_GP_LOG_DEVICE_STATISTICS_TRANSPORT_PAGE         0x06
#define IDE_GP_LOG_DEVICE_STATISTICS_SSD_PAGE               0x07

#pragma pack(push, device_statistics_log_page_header, 1)
typedef struct _DEVICE_STATISTICS_LOG_PAGE_HEADER {
    ULONGLONG   RevisionNumber : 16;    // Shall be set to 0001h
    ULONGLONG   PageNumber : 8;         // Shall be set to the page number
    ULONGLONG   Reserved : 40;
} DEVICE_STATISTICS_LOG_PAGE_HEADER, *PDEVICE_STATISTICS_LOG_PAGE_HEADER;
#pragma pack (pop, device_statistics_log_page_header)

//
// Device Statistics Log Page structure definitions.
// Snapshot from February 18 2016 draft of ACS-4.
//

#define GP_LOG_DEVICE_STATISTICS_FLAGS \
    ULONGLONG ReservedFlags : 3; \
    ULONGLONG MonitoredConditionMet : 1; \
    ULONGLONG StatisticsSupportsDsn : 1; \
    ULONGLONG Normalized    : 1; \
    ULONGLONG ValidValue    : 1; \
    ULONGLONG Supported     : 1
        
//
// Supported Device Statistics Log Pages structure definition.
//
#pragma pack(push, gplog_supported_device_statistics_log, 1)
typedef struct _GP_LOG_SUPPORTED_DEVICE_STATISTICS {

    DEVICE_STATISTICS_LOG_PAGE_HEADER Header;   //byte 0..7

    UCHAR NumberOfEntries;                  //byte 8

    UCHAR LogPageNumbers[503];              //byte 9..512

} GP_LOG_SUPPORTED_DEVICE_STATISTICS, *PGP_LOG_SUPPORTED_DEVICE_STATISTICS;
#pragma pack(pop, gplog_supported_device_statistics_log)

C_ASSERT(sizeof(GP_LOG_SUPPORTED_DEVICE_STATISTICS) == IDE_GP_LOG_SECTOR_SIZE);

//
// General Statistics Log Page structure definition.
//
#pragma pack(push, gplog_general_statistics_log, 1)
typedef struct _GP_LOG_GENERAL_STATISTICS {

    DEVICE_STATISTICS_LOG_PAGE_HEADER Header;   //byte 0..7

    struct {                                //byte 8..15
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } LifeTimePoweronResets;

    struct {                                //byte 16..23
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } PoweronHours;

    struct {                                //byte 24..31
        ULONGLONG Count : 48;
        ULONGLONG Reserved : 8;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } LogicalSectorsWritten;

    struct {                                //byte 32..39
        ULONGLONG Count : 48;
        ULONGLONG Reserved : 8;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } WriteCommandCount;

    struct {                                //byte 40..47
        ULONGLONG Count : 48;
        ULONGLONG Reserved : 8;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } LogicalSectorsRead;

    struct {                                //byte 48..55
        ULONGLONG Count : 48;
        ULONGLONG Reserved : 8;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } ReadCommandCount;

    struct {                                //byte 56..63
        ULONGLONG TimeStamp : 48;
        ULONGLONG Reserved : 8;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } DateAndTime;

    struct {                                //byte 64..71
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } PendingErrorCount;

    struct {                                //byte 72..79
        ULONGLONG Value : 16;
        ULONGLONG Reserved : 40;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } WorkloadUtilizaton;

    struct {                                //byte 80..87
        ULONGLONG Value : 8;
        ULONGLONG Reserved0 : 28;
        ULONGLONG RateBasis : 4;
        ULONGLONG RateValidity : 8;
        ULONGLONG Reserved1 : 8;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } UtilizationUsageRate;

    UCHAR Reserved[424];

} GP_LOG_GENERAL_STATISTICS, *PGP_LOG_GENERAL_STATISTICS;
#pragma pack(pop, gplog_general_statistics_log)

C_ASSERT(sizeof(GP_LOG_GENERAL_STATISTICS) == IDE_GP_LOG_SECTOR_SIZE);

//
// Free Fall Statistics Log Page structure defintion.
//
#pragma pack(push, gplog_free_fall_statistics_log, 1)
typedef struct _GP_LOG_FREE_FALL_STATISTICS {

    DEVICE_STATISTICS_LOG_PAGE_HEADER Header;   //byte 0..7

    struct {                                //byte 8..15
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } NumberofFreeFallEventsDetected;

    struct {                                //byte 16..23
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } OverlimitShockEvents;

    UCHAR Reserved[488];

} GP_LOG_FREE_FALL_STATISTICS, *PGP_LOG_FREE_FALL_STATISTICS;
#pragma pack(pop, gplog_free_fall_statistics_log)

C_ASSERT(sizeof(GP_LOG_FREE_FALL_STATISTICS) == IDE_GP_LOG_SECTOR_SIZE);

//
// Rotating Media Statistics Log Page structure defintion.
//
#pragma pack(push, gplog_rotating_media_statistics_log, 1)
typedef struct _GP_LOG_ROTATING_MEDIA_STATISTICS {

    DEVICE_STATISTICS_LOG_PAGE_HEADER Header;   //byte 0..7

    struct {                                //byte 8..15
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } SpindleMotorPoweronHours;

    struct {                                //byte 16..23
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } HeadFlyingHours;

    struct {                                //byte 24..31
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } HeadLoadEvents;

    struct {                                //byte 32..39
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } NumberOfReallocatedLogicalSectors;

    struct {                                //byte 40..47
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } ReadRecoveryAttempts;

    struct {                                //byte 48..55
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } NumberOfMechanicalStartFailures;

    struct {                                //byte 56..63
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } NumberOfReallocationCandidateLogicalSectors;

    struct {                                //byte 64..71
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } NumberOfHighPriorityUnloadEvents;

    UCHAR Reserved[440];

} GP_LOG_ROTATING_MEDIA_STATISTICS, *PGP_LOG_ROTATING_MEDIA_STATISTICS;
#pragma pack(pop, gplog_rotating_media_statistics_log)

C_ASSERT(sizeof(GP_LOG_ROTATING_MEDIA_STATISTICS) == IDE_GP_LOG_SECTOR_SIZE);

//
// General Error Statistics Log Page structure defintion.
//
#pragma pack(push, gplog_general_error_statistics_log, 1)
typedef struct _GP_LOG_GENERAL_ERROR_STATISTICS {

    DEVICE_STATISTICS_LOG_PAGE_HEADER Header;   //byte 0..7

    struct {                                //byte 8..15
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } NumberOfReportedUncorrectableErrors;

    struct {                                //byte 16..23
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } NumberOfResetsBetweenCommandAcceptanceAndCommandCompletion;

    UCHAR Reserved[488];

} GP_LOG_GENERAL_ERROR_STATISTICS, *PGP_LOG_GENERAL_ERROR_STATISTICS;
#pragma pack(pop, gplog_general_error_statistics_log)

C_ASSERT(sizeof(GP_LOG_GENERAL_ERROR_STATISTICS) == IDE_GP_LOG_SECTOR_SIZE);

//
// Temperature Statistics Log Page structure definition.
// NOTE: all temperature value fields are signed byte.
//
#pragma pack(push, gplog_temperature_statistics_log, 1)
typedef struct _GP_LOG_TEMPERATURE_STATISTICS {

    DEVICE_STATISTICS_LOG_PAGE_HEADER Header;   //byte 0..7

    struct {                                //byte 8..15
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } CurrentTemperature;

    struct {                                //byte 16..23
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } AverageShortTermTemperature;

    struct {                                //byte 24..31
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } AverageLongTermTemperature;

    struct {                                //byte 32..39
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } HighestTemperature;

    struct {                                //byte 40..47
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } LowestTemperature;

    struct {                                //byte 48..55
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } HighestAverageShortTermTemperature;

    struct {                                //byte 56..63
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } LowestAverageShortTermTemperature;

    struct {                                //byte 64..71
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } HighstAverageLongTermTemperature;

    struct {                                //byte 72..79
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } LowestAverageLongTermTemperature;

    struct {                                //byte 80..87
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } TimeInOverTemperature;

    struct {                                //byte 88..95
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } SpecifiedMaximumOperatingTemperature;

    struct {                                //byte 96..103
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } TimeInUnderTemperature;

    struct {                                //byte 104..111
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } SpecifiedMinimumOperatingTemperature;

    UCHAR Reserved[400];

} GP_LOG_TEMPERATURE_STATISTICS, *PGP_LOG_TEMPERATURE_STATISTICS;
#pragma pack(pop, gplog_temperature_statistics_log)

C_ASSERT(sizeof(GP_LOG_TEMPERATURE_STATISTICS) == IDE_GP_LOG_SECTOR_SIZE);

//
// Transport Statistics Log Page structure defintion.
//
#pragma pack(push, gplog_transport_statistics_log, 1)
typedef struct _GP_LOG_TRANSPORT_STATISTICS {

    DEVICE_STATISTICS_LOG_PAGE_HEADER Header;   //byte 0..7

    struct {                                //byte 8..15
        ULONGLONG Count : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } NumberOfHardwareResets;

    struct {                                //byte 16..23
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } NumberOfAsrEvents;

    struct {                                //byte 24..31
        ULONGLONG Count : 32;
        ULONGLONG Reserved : 24;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } NumberOfInterfaceCrcErrors;

    UCHAR Reserved[480];

} GP_LOG_TRANSPORT_STATISTICS, *PGP_LOG_TRANSPORT_STATISTICS;
#pragma pack(pop, gplog_transport_statistics_log)

C_ASSERT(sizeof(GP_LOG_TRANSPORT_STATISTICS) == IDE_GP_LOG_SECTOR_SIZE);

//
// Solid State Device Statistics Log Page structure defintion.
//
#pragma pack(push, gplog_solid_state_device_statistics_log, 1)
typedef struct _GP_LOG_SOLID_STATE_DEVICE_STATISTICS {

    DEVICE_STATISTICS_LOG_PAGE_HEADER Header;   //byte 0..7

    struct {                                //byte 8..15
        ULONGLONG Value : 8;
        ULONGLONG Reserved : 48;
        GP_LOG_DEVICE_STATISTICS_FLAGS;
    } PercentageUsedEnduranceIndicator;

    UCHAR Reserved[496];

} GP_LOG_SOLID_STATE_DEVICE_STATISTICS, *PGP_LOG_SOLID_STATE_DEVICE_STATISTICS;
#pragma pack(pop, gplog_solid_state_device_statistics_log)

C_ASSERT(sizeof(GP_LOG_SOLID_STATE_DEVICE_STATISTICS) == IDE_GP_LOG_SECTOR_SIZE);

//
// "NCQ Command Error log page" structure definition.
//
#pragma pack(push, ncq_error_log, 1)
typedef struct _GP_LOG_NCQ_COMMAND_ERROR {
    UCHAR   NcqTag          : 5;
    UCHAR   Reserved0       : 1;
    UCHAR   UNL             : 1;        // error: IDLE IMMEDIATE with UNLOAD
    UCHAR   NonQueuedCmd    : 1;

    UCHAR   Reserved1;
    UCHAR   Status;
    UCHAR   Error;
    UCHAR   LBA7_0;
    UCHAR   LBA15_8;
    UCHAR   LBA23_16;
    UCHAR   Device;
    UCHAR   LBA31_24;
    UCHAR   LBA39_32;
    UCHAR   LBA47_40;
    UCHAR   Reserved2;
    UCHAR   Count7_0;
    UCHAR   Count15_8;
    UCHAR   SenseKey;
    UCHAR   ASC;
    UCHAR   ASCQ;
    UCHAR   Reserved3[239];
    UCHAR   Vendor[255];
    UCHAR   Checksum;
} GP_LOG_NCQ_COMMAND_ERROR, *PGP_LOG_NCQ_COMMAND_ERROR;
#pragma pack (pop, ncq_error_log)


//
// "NCQ Non-data log page" structure definition.
//
#pragma pack(push, ncq_non_data_log, 1)
typedef struct _GP_LOG_NCQ_NON_DATA {

    struct {
        ULONG   AbortNcq            : 1;
        ULONG   AbortAll            : 1;
        ULONG   AbortStreaming      : 1;
        ULONG   AbortNonStreaming   : 1;
        ULONG   AbortSelectedTTag   : 1;
        ULONG   Reserved            : 27;
    } SubCmd0;

    struct {
        ULONG   DeadlineHandling        : 1;
        ULONG   WriteDataNotContinue    : 1;
        ULONG   ReadDataNotContinue     : 1;
        ULONG   Reserved                : 29;
    } SubCmd1;

    struct {
        ULONG   HybridDemoteBySize  : 1;
        ULONG   Reserved            : 31;
    } SubCmd2;

    struct {
        ULONG   HybridChangeByLbaRange  : 1;
        ULONG   Reserved                : 31;
    } SubCmd3;

    struct {
        ULONG   HybridControl   : 1;
        ULONG   Reserved        : 31;
    } SubCmd4;

    struct {
        ULONG   Reserved        : 32;
    } SubCmd5;

    struct {
        ULONG   Reserved        : 32;
    } SubCmd6;

    struct {
        ULONG   Reserved        : 32;
    } SubCmd7;

    struct {
        ULONG   Reserved        : 32;
    } SubCmd8;

    struct {
        ULONG   Reserved        : 32;
    } SubCmd9;

    struct {
        ULONG   Reserved        : 32;
    } SubCmdA;

    struct {
        ULONG   Reserved        : 32;
    } SubCmdB;

    struct {
        ULONG   Reserved        : 32;
    } SubCmdC;

    struct {
        ULONG   Reserved        : 32;
    } SubCmdD;

    struct {
        ULONG   Reserved        : 32;
    } SubCmdE;

    struct {
        ULONG   Reserved        : 32;
    } SubCmdF;

    ULONG   Reserved[112];

} GP_LOG_NCQ_NON_DATA, *PGP_LOG_NCQ_NON_DATA;
#pragma pack (pop, ncq_non_data_log)

//
// "NCQ Send Receive log page" strucutre definition
//
#pragma pack(push, ncq_send_receive_log, 1)
typedef struct _GP_LOG_NCQ_SEND_RECEIVE {

    struct {
        ULONG   DataSetManagement   : 1;
        ULONG   HybridEvict         : 1;
        ULONG   Reserved            : 30;
    } SubCmd;

    struct {
        ULONG   Trim        : 1;
        ULONG   Reserved    : 31;
    } DataSetManagement;

    ULONG   Reserved[126];

} GP_LOG_NCQ_SEND_RECEIVE, *PGP_LOG_NCQ_SEND_RECEIVE;
#pragma pack (pop, ncq_send_receive_log)

//
// "Hybrid Information log page" strucutre definition
//
#define HYBRID_INFORMATION_DISABLED             0x00
#define HYBRID_INFORMATION_DISABLE_IN_PROCESS   0x80
#define HYBRID_INFORMATION_ENABLED              0xFF

#define HYBRID_HEALTH_UNUSEABLE                 0x01
#define HYBRID_HEALTH_NVM_SIZE_CHANGED          0x02
#define HYBRID_HEALTH_READ_ONLY                 0x04
#define HYBRID_HEALTH_DATA_LOSS                 0x08

#pragma pack(push, hybrid_info_header, 1)
typedef struct _GP_LOG_HYBRID_INFORMATION_HEADER {
    USHORT  HybridInfoDescrCount        : 4;
    USHORT  Reserved0                   : 12;

    UCHAR   Enabled;                //Can be 0x00, 0x80 or 0xFF
    UCHAR   HybridHealth;
    UCHAR   DirtyLowThreshold;
    UCHAR   DirtyHighThreshold;
    UCHAR   OptimalWriteGranularity;

    UCHAR   MaximumHybridPriorityLevel  : 4;
    UCHAR   Reserved1                   : 4;

    UCHAR   PowerCondidtion;
    UCHAR   CachingMediumEnabled ;

    struct {
        UCHAR   MaximumPriorityBehavior : 1;
        UCHAR   SupportCacheBehavior    : 1;
        UCHAR   Reserved                : 6;
    } SupportedOptions;

    UCHAR   Reserved2;              // byte 11

    ULONG       TimeSinceEnabled;
    ULONGLONG   NVMSize;
    ULONGLONG   EnableCount;

    USHORT  MaximumEvictionCommands : 5;
    USHORT  Reserved3               : 11;

    USHORT  MaximumEvictionDataBlocks;  // how many data blocks (one block is 512 bytes) an Evict command can carry with.

    UCHAR   Reserved[28];

} GP_LOG_HYBRID_INFORMATION_HEADER, *PGP_LOG_HYBRID_INFORMATION_HEADER;
#pragma pack (pop, hybrid_info_header)

#pragma pack(push, hybrid_info_descriptor, 1)
typedef struct _GP_LOG_HYBRID_INFORMATION_DESCRIPTOR {

    UCHAR   HybridPriority;
    UCHAR   ConsumedNVMSizeFraction;
    UCHAR   ConsumedMappingResourcesFraction;
    UCHAR   ConsumedNVMSizeForDirtyDataFraction;
    UCHAR   ConsumedMappingResourcesForDirtyDataFraction;

    UCHAR   Reserved[11];

} GP_LOG_HYBRID_INFORMATION_DESCRIPTOR, *PGP_LOG_HYBRID_INFORMATION_DESCRIPTOR;
#pragma pack (pop, hybrid_info_descriptor)

#pragma pack(push, hybrid_info_log, 1)
typedef struct _GP_LOG_HYBRID_INFORMATION {

    GP_LOG_HYBRID_INFORMATION_HEADER        Header;

    GP_LOG_HYBRID_INFORMATION_DESCRIPTOR    Descriptor[0];

} GP_LOG_HYBRID_INFORMATION, *PGP_LOG_HYBRID_INFORMATION;
#pragma pack (pop, hybrid_info_log)


#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4214)
#pragma warning(default:4200)
#pragma warning(default:4201)
#endif

#endif
