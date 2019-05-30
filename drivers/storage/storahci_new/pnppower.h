/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    pnppower.h

Abstract:

    

Notes:

Revision History:

        Nathan Obr (natobr),  February 2005
        Michael Xing (xiaoxing),  December 2009
--*/

#pragma once

#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning(disable:4214) // bit field types other than int
#pragma warning(disable:4201) // nameless struct/union


typedef union _AHCI_LPM_POWER_SETTINGS {
    struct {
    //LSB
    ULONG HipmEnabled: 1;
    ULONG DipmEnabled: 1;
    ULONG Reserved: 30;
    //MSB
    };

    ULONG AsUlong;

} AHCI_LPM_POWER_SETTINGS, *PAHCI_LPM_POWER_SETTINGS;

BOOLEAN 
AhciPortInitialize(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

BOOLEAN 
AhciAdapterPowerUp(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    );

#if (NTDDI_VERSION > NTDDI_WIN7)
BOOLEAN 
AhciAdapterPowerSettingNotification(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension,
    __in PSTOR_POWER_SETTING_INFO PowerSettingInfo
    );
#endif	

BOOLEAN 
AhciAdapterPowerDown(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    );

VOID 
AhciPortPowerUp(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID 
AhciPortPowerDown(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID 
AhciPortStop(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID
AhciPortSmartCompletion(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
  );

VOID
AhciPortNVCacheCompletion(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb
  );

VOID
AhciPortGetInitCommands(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
  );

VOID
IssuePreservedSettingCommands(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in_opt PSCSI_REQUEST_BLOCK Srb
  );

VOID
IssueInitCommands(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in_opt PSCSI_REQUEST_BLOCK Srb
  );

VOID
IssueSetDateAndTimeCommand(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in BOOLEAN SendStandBy
  );


BOOLEAN
AhciAutoPartialToSlumber(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in_opt PVOID Context
  );

BOOLEAN
AhciLpmSettingsModes(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in AHCI_LPM_POWER_SETTINGS LpmMode
    );

#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4214)
#pragma warning(default:4201)
#endif

