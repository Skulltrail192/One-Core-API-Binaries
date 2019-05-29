/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    io.h

Abstract:

    

Notes:

Revision History:

        Nathan Obr (natobr),  February 2005
        Michael Xing (xiaoxing),  December 2009
--*/

#pragma once

ULONG
GetSlotToActivate(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in ULONG                   TargetSlots
    );

UCHAR
GetSingleIo(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

BOOLEAN
ActivateQueue(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in BOOLEAN AtDIRQL
    );

VOID
AddQueue (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __inout PSTORAHCI_QUEUE Queue,
    __in PSCSI_REQUEST_BLOCK Srb,
    __in ULONG Signature,
    __in UCHAR Tag
    );

PSCSI_REQUEST_BLOCK
RemoveQueue (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __inout PSTORAHCI_QUEUE Queue,
    __in ULONG Signature,
    __in UCHAR Tag
    );

VOID
AhciCompleteIssuedSRBs(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in UCHAR SrbStatus,
    __in BOOLEAN AtDIRQL
  );

VOID
SRBtoATA_CFIS(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    PSLOT_CONTENT SlotContent
  );

VOID
SRBtoATAPI_CFIS(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    PSLOT_CONTENT SlotContent
  );

ULONG
SRBtoPRDT(
    __in PVOID ChannelExtension,
    __in PSLOT_CONTENT SlotContent
  );

VOID
SRBtoCmdHeader(
    __in PVOID ChannelExtension,
    __in PSLOT_CONTENT SlotContent,
    __in ULONG Length,
    __in BOOLEAN Reset
  );

BOOLEAN
AhciProcessIo(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb,
    __in BOOLEAN AtDIRQL
    );

BOOLEAN
AhciFormIo(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in PSCSI_REQUEST_BLOCK Srb,
    __in BOOLEAN AtDIRQL
    );

PSCSI_REQUEST_BLOCK
BuildRequestSenseSrb(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension, 
    __in PSCSI_REQUEST_BLOCK FailingSrb
    );

VOID
AhciPortFailAllIos(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in UCHAR   SrbStatus,
    __in BOOLEAN AtDIRQL
    );

HW_DPC_ROUTINE AhciPortSrbCompletionDpcRoutine;

