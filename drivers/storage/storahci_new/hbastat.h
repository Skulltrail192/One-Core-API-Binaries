/*++

Copyright (C) Microsoft Corporation, 2009

Module Name:

    hbastat.h

Abstract:

    

Notes:

Revision History:

        Nathan Obr (natobr),  February 2005
        Michael Xing (xiaoxing),  December 2009
--*/

#pragma once

BOOLEAN
AhciAdapterReset( 
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    );

VOID
AhciCOMRESET(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    PAHCI_PORT Px
    );

BOOLEAN
P_NotRunning(
    PAHCI_CHANNEL_EXTENSION ChannelExtension,
    PAHCI_PORT Px
    );

VOID
AhciAdapterRunAllPorts(
    __in PAHCI_ADAPTER_EXTENSION AdapterExtension
    );

VOID
RunNextPort(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID
P_Running_StartAttempt(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in BOOLEAN AtDIRQL
    );

BOOLEAN
P_Running(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension,
    __in_opt PVOID Context
    );

VOID
P_Running_WaitOnDET(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID
P_Running_WaitWhileDET1(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID
P_Running_WaitOnDET3(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID
P_Running_WaitOnFRE(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID
P_Running_WaitOnBSYDRQ(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID
P_Running_StartFailed(
    PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

BOOLEAN
AhciPortReset (
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

VOID
AhciPortErrorRecovery(
    __in PAHCI_CHANNEL_EXTENSION ChannelExtension
    );

