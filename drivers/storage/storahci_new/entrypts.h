/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    entrypts.h

Abstract:

    This header contains the declaration of functions that will be called by Storport driver.

Authors:

    Michael Xing (xiaoxing), December 2009

--*/

#pragma once

#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning(disable:4214)   // bit field types other than int
#pragma warning(disable:4201)   // nameless struct/union


#define AHCI_POOL_TAG               'ichA'  // "Ahci" - StorAHCI miniport driver

#define NUM_ACCESS_RANGES           6

#define AHCI_MAX_PORT_COUNT         32
#define AHCI_MAX_DEVICE             1       //not support Port Multiplier
#define AHCI_MAX_LUN                8       //ATAport supports this much in old implementation.
#define AHCI_MAX_NCQ_REQUEST_COUNT  32

#define KB                          (1024)
#define AHCI_MAX_TRANSFER_LENGTH    (128 * KB)
#define MAX_SETTINGS_PRESERVED      32
#define MAX_CRB_LOG_INDEX           64

#define INQUIRYDATABUFFERSIZE       36

// timeout values and counters for channel start
// some big HDDs (e.g. 2TB) takes ~ 20 seconds to spin up.  We hit timeout during S3 resume for now, 
// we reset after 1/3 * AHCI_PORT_START_TIMEOUT_IN_SECONDS Busy, and give up after AHCI_PORT_START_TIMEOUT_IN_SECONDS seconds

#define AHCI_PORT_START_TIMEOUT_IN_SECONDS  60      // 60 sec.

#define AHCI_PORT_WAIT_ON_DET_COUNT         3       // in unit of 10ms, default 30ms.


// port start states
#define WaitOnDET       0x11
#define WaitWhileDET1   0x12
#define WaitOnDET3      0x13
#define WaitOnFRE       0x14
#define WaitOnBSYDRQ    0x15
#define StartComplete   0x16
#define Stopped         0x20
#define StartFailed     0xff

// registry flags apply to port or device
typedef struct _CHANNEL_REGISTRY_FLAGS {


    ULONG Reserved : 25;

} CHANNEL_REGISTRY_FLAGS, *PCHANNEL_REGISTRY_FLAGS;

// registry flags apply to the whole adapter
typedef struct _ADAPTER_REGISTRY_FLAGS {

    ULONG Reserved2 : 20;


} ADAPTER_REGISTRY_FLAGS, *PADAPTER_REGISTRY_FLAGS;

typedef struct _CHANNEL_STATE_FLAGS {

    ULONG StartCapable : 1;
    ULONG NoMoreIO : 1;
    ULONG QueuePaused : 1;
    ULONG ReservedSlotInUse : 1;    //NOTE: this field is accessed in InterlockedBitTestAndReset, bit position (currently: 3) is used there.

    ULONG NCQ_Activated : 1;
    ULONG NCQ_Succeeded : 1;
    ULONG AN_Enabled : 1;
    ULONG AN_SendGesnToDevice : 1;

    ULONG CallAhciReset : 1;
    ULONG CallAhciNonQueuedErrorRecovery : 1;
    ULONG CallAhciReportBusChange : 1;
    ULONG IgnoreHotplugDueToResetInProgress : 1;

    ULONG Reserved: 20;

} CHANNEL_STATE_FLAGS, *PCHANNEL_STATE_FLAGS;

typedef struct _ATA_IO_RECORD {

    ULONG   SuccessCount;

    ULONG   CrcErrorCount;
    ULONG   MediaErrorCount;
    ULONG   EndofMediaCount;
    ULONG   IllegalCommandCount;
    ULONG   AbortedCommandCount;

    ULONG   OtherErrorCount;

    ULONG   PortDriverResetCount;
    ULONG   TotalResetCount;

} ATA_IO_RECORD, *PATA_IO_RECORD;

typedef struct _STOR_ADDR_BTL8 {
  USHORT Type;
  USHORT Port;
  ULONG  AddressLength;
  UCHAR  Path;
  UCHAR  Target;
  UCHAR  Lun;
  UCHAR  Reserved;
} STOR_ADDR_BTL8, *PSTOR_ADDR_BTL8;

typedef struct _AHCI_DEVICE_EXTENSION {
    STOR_ADDR_BTL8          DeviceAddress;
    ATA_DEVICE_PARAMETERS   DeviceParameters;
    ATA_IO_RECORD           IoRecord;       //IO counts completed by device, the count only change if an IO reaches hardware

    union {
      PIDENTIFY_DEVICE_DATA IdentifyDeviceData;
      PIDENTIFY_PACKET_DATA IdentifyPacketData;
    }; 
    STOR_PHYSICAL_ADDRESS   IdentifyDataPhysicalAddress;
    PUCHAR                  InquiryData;    //for ATAPI device, size:INQUIRYDATABUFFERSIZE
    STOR_PHYSICAL_ADDRESS   InquiryDataPhysicalAddress;


} AHCI_DEVICE_EXTENSION, *PAHCI_DEVICE_EXTENSION;

typedef struct _COMMAND_HISTORY {
    union {                              //0x10 bytes
        ATA_TASK_FILE InitialTaskFile;
        UCHAR         Cdb[16];
    };
    ULONG InitialPx[0x10];               //0x40 bytes
    ULONG CompletionPx[0x10];

    AHCI_D2H_REGISTER_FIS CompletionFIS; //0x14 bytes
    ULONG Tag;
    ULONG Function;
    ULONG SrbStatus;
} COMMAND_HISTORY, *PCOMMAND_HISTORY;

typedef struct _SLOT_MANAGER {
    ULONG HighPriorityAttribute;

    ULONG NCQueueSlice;
    ULONG NormalQueueSlice;
    ULONG SingleIoSlice;

    ULONG CommandsIssued;
    ULONG CommandsToComplete;
} SLOT_MANAGER, *PSLOT_MANAGER;

typedef struct _EXECUTION_HISTORY {
    ULONG        Function;
    ULONG        IS;
    SLOT_MANAGER SlotManager;   //SLOT_MANAGER from _AHCI_CHANNEL_EXTENSION
    ULONG        Px[0x10];      //Px registers value, end to AHCI_SNOTIFICATION -- SNTF
} EXECUTION_HISTORY, *PEXECUTION_HISTORY;

typedef struct _SLOT_STATE_FLAGS {
    UCHAR FUA :1;
    UCHAR Reserved :7;
} SLOT_STATE_FLAGS, *PSLOT_STATE_FLAGS;


typedef struct _SLOT_CONTENT {
    UCHAR                   CommandHistoryIndex;
    SLOT_STATE_FLAGS        StateFlags;
    UCHAR                   Reserved0[2];
    PSCSI_REQUEST_BLOCK     Srb;
    PAHCI_COMMAND_HEADER    CmdHeader;
    PVOID                   Reserved;
} SLOT_CONTENT, *PSLOT_CONTENT;

#pragma pack(1)
typedef struct _ACPI_GTF_IDE_REGISTERS {
    UCHAR    FeaturesReg;
    UCHAR    SectorCountReg;
    UCHAR    SectorNumberReg;
    UCHAR    CylLowReg;
    UCHAR    CylHighReg;
    UCHAR    DriveHeadReg;
    UCHAR    CommandReg;
} ACPI_GTF_IDE_REGISTERS, *PACPI_GTF_IDE_REGISTERS;
#pragma pack()

typedef struct _AHCI_DEVICE_INIT_COMMANDS {
    UCHAR           CommandCount;       // count of totoal commands
    UCHAR           CommandToSend;      // command index indicating which command is to be sent
    PATA_TASK_FILE  CommandTaskFile;    // commands
} AHCI_DEVICE_INIT_COMMANDS, *PAHCI_DEVICE_INIT_COMMANDS;


typedef struct _SET_FEATURE_PARAMS {
    UCHAR Features;
    UCHAR SectorCount;
} SET_FEATURE_PARAMS, *PSET_FEATURE_PARAMS;

typedef struct _PERSISTENT_SETTINGS {
    ULONG Slots;
    ULONG SlotsToSend;
    SET_FEATURE_PARAMS CommandParams[MAX_SETTINGS_PRESERVED];
} PERSISTENT_SETTINGS, *PPERSISTENT_SETTINGS;

typedef struct _CHANNEL_START_STATE {
    UCHAR  ChannelNextStartState;
    UCHAR  ChannelStateDETCount;
    UCHAR  ChannelStateDET1Count;
    UCHAR  ChannelStateDET3Count;
    UCHAR  ChannelStateFRECount;
    UCHAR  AtDIRQL : 1;
    UCHAR  Reserved : 7;
    USHORT ChannelStateBSYDRQCount;
} CHANNEL_START_STATE, *PCHANNEL_START_STATE;

typedef VOID
(*PSRB_COMPLETION_ROUTINE) (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in_opt PSCSI_REQUEST_BLOCK Srb
    );

typedef struct _LOCAL_SCATTER_GATHER_LIST {
    ULONG                       NumberOfElements;
    ULONG_PTR                   Reserved;
    __field_ecount(NumberOfElements) 
    STOR_SCATTER_GATHER_ELEMENT List[33];
} LOCAL_SCATTER_GATHER_LIST, *PLOCAL_SCATTER_GATHER_LIST;

typedef struct _AHCI_SRB_EXTENSION {
    AHCI_COMMAND_TABLE CommandTable;        // this field MUST to be the first one as it's asked to be 128 aligned
    USHORT             AtaFunction;         // if this field is 0, it means the command does not need to be sent to device
    UCHAR              AtaStatus;
    UCHAR              AtaError;
    ULONG              Flags;
    union {
        ATA_TASK_FILE  TaskFile;            // for ATA device
        CDB            Cdb;                 // for ATAPI device
    };
    PVOID              DataBuffer;          // go with Cdb field when needed.
    ULONG              DataTransferLength;  // go with Cdb field when needed.
    PLOCAL_SCATTER_GATHER_LIST  Sgl;        // pointer to the local or port provided SGL
    LOCAL_SCATTER_GATHER_LIST   LocalSgl;   // local SGL
    PSRB_COMPLETION_ROUTINE CompletionRoutine;   // go with Cdb field when needed.
    PVOID                   CompletionContext;   // context information for completionRoutine
    UCHAR              QueueTag;            // for AHCI controller slots
    UCHAR              RetryCount;          // how many times the command has been retired
    ULONGLONG          StartTime;
} AHCI_SRB_EXTENSION, *PAHCI_SRB_EXTENSION;

typedef struct _LOCAL_COMMAND {
    SCSI_REQUEST_BLOCK      Srb;
    PAHCI_SRB_EXTENSION     SrbExtension;
    STOR_PHYSICAL_ADDRESS   SrbExtensionPhysicalAddress;
} LOCAL_COMMAND, *PLOCAL_COMMAND;

typedef enum {
    CallbackTypeNone = 0,       //not used
    CallbackTypeStartPort,      
    CallbackTypeLPM,            
    CallbackTypeMax             //count
} TIMER_CALLBACK_TYPE;

typedef
BOOLEAN
(*PAHCI_TIMER_CALLBACK) (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PVOID Context
    );

typedef struct _PORT_TIMER_CONTEXT {
    ULONG                Interval;       
    PAHCI_TIMER_CALLBACK CallBack;
    PVOID                Context;
} PORT_TIMER_CONTEXT, *PPORT_TIMER_CONTEXT;

typedef struct _STORAHCI_QUEUE {
    PVOID Head;
    PVOID Tail;
    ULONG CurrentDepth;
    ULONG DeepestDepth;
    ULONG DepthHistoryIndex;
    ULONG DepthHistory[100];
} STORAHCI_QUEUE, *PSTORAHCI_QUEUE;

struct AHCI_ADAPTER_EXTENSION;

typedef struct _AHCI_ADAPTER_EXTENSION
               AHCI_ADAPTER_EXTENSION, *PAHCI_ADAPTER_EXTENSION;

typedef struct _AHCI_CHANNEL_EXTENSION {
//Adapter Characteristics
    PAHCI_ADAPTER_EXTENSION AdapterExtension;

//Device Characteristics
    AHCI_DEVICE_EXTENSION   DeviceExtension[1];

//Channel Characteristics
    ULONG                   PortNumber;

//Channel State
    CHANNEL_START_STATE     StartState;
    CHANNEL_STATE_FLAGS     StateFlags;
    CHANNEL_REGISTRY_FLAGS  RegistryFlags;

    UCHAR                   MaxPortQueueDepth;
    UCHAR                   CurrentCommandSlot;           // miniport driver use to remember which slot to put command in.
    UCHAR                   LastActiveSlot;
    UCHAR                   LastUserLpmPowerSetting;      // bit 0: HIPM; bit 1: DIPM

    ULONG                   AutoPartialToSlumberInterval; // in milliSeconds, max: 300,000 (5 minutes)

//IO
    SLOT_MANAGER            SlotManager;
    SLOT_CONTENT            Slot[AHCI_MAX_NCQ_REQUEST_COUNT];

//Port IO Queue 
    STORAHCI_QUEUE          SrbQueue;

//IO Completion Queue and DPC
    STORAHCI_QUEUE          CompletionQueue;
    STOR_DPC                CompletionDpc;

//AHCI defined register interface structures
    PAHCI_PORT              Px;
    PAHCI_COMMAND_HEADER    CommandList;
    PAHCI_RECEIVED_FIS      ReceivedFIS;
    STOR_PHYSICAL_ADDRESS   CommandListPhysicalAddress;
    STOR_PHYSICAL_ADDRESS   ReceivedFisPhysicalAddress;

//Local Command Structures
    LOCAL_COMMAND               Local;
    LOCAL_COMMAND           	Sense;              // used to retrieve Sense Data from device

    AHCI_DEVICE_INIT_COMMANDS   DeviceInitCommands;
    PERSISTENT_SETTINGS         PersistentSettings;

    PUCHAR                  	GesnCachedBuffer;
    STOR_PHYSICAL_ADDRESS   	GesnCachedBufferPhysicalAddress;

//Timer Callback Context. [0] - used for Port Start; [1] - used for device enumeration
    PORT_TIMER_CONTEXT      TimerContext[CallbackTypeMax - 1]; 

//Logging
    UCHAR                   CommandHistoryNextAvailableIndex;
    COMMAND_HISTORY         CommandHistory[64];
    UCHAR                   ExecutionHistoryNextAvailableIndex;
    EXECUTION_HISTORY       ExecutionHistory[100];

} AHCI_CHANNEL_EXTENSION, *PAHCI_CHANNEL_EXTENSION;

typedef struct _ADAPTER_STATE_FLAGS {

    ULONG DumpMode : 1;
    ULONG StoppedState : 1;

    ULONG Reserved : 30;

} ADAPTER_STATE_FLAGS, *PADAPTER_STATE_FLAGS;

typedef union _ADAPTER_LOG_FLAGS {

    struct {
        ULONG ExecutionDetail :1;
        ULONG CommandHistory :1;

        ULONG Reserved: 30;
    };
    
    ULONG AsUlong;

} ADAPTER_LOG_FLAGS, *PADAPTER_LOG_FLAGS;

#define LogCommand(flags)           (flags.CommandHistory != 0)
#define LogExecuteFullDetail(flags) (flags.ExecutionDetail != 0)


typedef struct _AHCI_ADAPTER_EXTENSION {
    ULONG                   AdapterNumber;
    ULONG                   SystemIoBusNumber;
    ULONG                   SlotNumber;
    ULONG                   AhciBaseAddress;    // AHCI Base Address from bus. for example: BAR5 value on PCI bus

    USHORT                  VendorID;
    USHORT                  DeviceID;
    UCHAR                   RevisionID; 

//Flags
    ADAPTER_STATE_FLAGS     StateFlags;
    ULONG                   ErrorFlags;     // save adapter errors
    ADAPTER_LOG_FLAGS       LogFlags;       // internal log levels
    ADAPTER_REGISTRY_FLAGS  RegistryFlags;

//adapter attributes
    ULONG                   PortImplemented;
    ULONG                   HighestPort;
    ULONG                   LastInterruptedPort;

    BOOLEAN                 InRunningPortsProcess;  //in process of starting every implemented ports
    BOOLEAN                 TracingEnabled;

//Memory structures
    PAHCI_MEMORY_REGISTERS  ABAR_Address;           //mapped AHCI Base Address. StorAHCI uses this field to control the adapter and ports.
    AHCI_VERSION            Version;

    PULONG                  IS;
    AHCI_HBA_CAPABILITIES   CAP;
    AHCI_HBA_CAPABILITIES2  CAP2;

//Channel Extensions
    PAHCI_CHANNEL_EXTENSION PortExtension[AHCI_MAX_PORT_COUNT];

//nonCacheExtension
    PVOID                   NonCachedExtension;

} AHCI_ADAPTER_EXTENSION, *PAHCI_ADAPTER_EXTENSION;

// information that will be transferred to dump/hibernate environment
typedef struct _AHCI_DUMP_CONTEXT {
    // adapter information
    USHORT                  VendorID;
    USHORT                  DeviceID;
    UCHAR                   RevisionID; 
    UCHAR                   Reserved[3];

    ULONG                   AhciBaseAddress;

    ADAPTER_LOG_FLAGS       LogFlags;       // internal log levels
    ADAPTER_REGISTRY_FLAGS  AdapterRegistryFlags;

    // port information
    ULONG                   DumpPortNumber;
    CHANNEL_REGISTRY_FLAGS  PortRegistryFlags;

} AHCI_DUMP_CONTEXT, *PAHCI_DUMP_CONTEXT;

#if (NTDDI_VERSION > NTDDI_WIN7)
// Storport miniport driver entry routines, with prefix: "AhciHw"
HW_FIND_ADAPTER AhciHwFindAdapter;

HW_INITIALIZE AhciHwInitialize;

HW_PASSIVE_INITIALIZE_ROUTINE AhciHwPassiveInitialize;

HW_STARTIO AhciHwStartIo;

HW_BUILDIO AhciHwBuildIo;

HW_INTERRUPT AhciHwInterrupt;

HW_RESET_BUS AhciHwResetBus;

HW_ADAPTER_CONTROL AhciHwAdapterControl;
#endif

VOID
AhciGetNextIos (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in BOOLEAN AtDIRQL
    );

VOID 
AhciDeviceStart (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    );


#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4214)
#pragma warning(default:4201)
#endif


