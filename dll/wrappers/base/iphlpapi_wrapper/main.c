/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */
#include <ntstatus.h>
#include "windef.h"
#include "winbase.h"
// #include <winsock2.h>
// #include <ws2tcpip.h>
#include <iofuncs.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <ifdef.h>
#define WIN32_NO_STATUS

#include <ip.h>

#define ScopeLevelCount 16

typedef enum _MIB_NOTIFICATION_TYPE
{
    MibParameterNotification,
    MibAddInstance,
    MibDeleteInstance,
    MibInitialNotification,
} MIB_NOTIFICATION_TYPE, *PMIB_NOTIFICATION_TYPE;

typedef struct _IP_ADDRESS_PREFIX {
  SOCKADDR_INET Prefix;
  UINT8         PrefixLength;
} IP_ADDRESS_PREFIX, *PIP_ADDRESS_PREFIX;

typedef enum _NL_ROUTE_ORIGIN { 
  NlroManual,
  NlroWellKnown,
  NlroDHCP,
  NlroRouterAdvertisement,
  Nlro6to4
} NL_ROUTE_ORIGIN, *PNL_ROUTE_ORIGIN;

#define MAKE_ROUTE_PROTOCOL(N, V) MIB_IPPROTO_ ## N = V, PROTO_IP_ ## N = V

typedef enum {
  RouteProtocolOther = 1,
  RouteProtocolLocal = 2,
  RouteProtocolNetMgmt = 3,
  RouteProtocolIcmp = 4,
  RouteProtocolEgp = 5,
  RouteProtocolGgp = 6,
  RouteProtocolHello = 7,
  RouteProtocolRip = 8,
  RouteProtocolIsIs = 9,
  RouteProtocolEsIs = 10,
  RouteProtocolCisco = 11,
  RouteProtocolBbn = 12,
  RouteProtocolOspf = 13,
  RouteProtocolBgp = 14
} NL_ROUTE_PROTOCOL,*PNL_ROUTE_PROTOCOL;

typedef struct _MIB_IPFORWARD_ROW2 {
  NET_LUID          InterfaceLuid;
  NET_IFINDEX       InterfaceIndex;
  IP_ADDRESS_PREFIX DestinationPrefix;
  SOCKADDR_INET     NextHop;
  UCHAR             SitePrefixLength;
  ULONG             ValidLifetime;
  ULONG             PreferredLifetime;
  ULONG             Metric;
  NL_ROUTE_PROTOCOL Protocol;
  BOOLEAN           Loopback;
  BOOLEAN           AutoconfigureAddress;
  BOOLEAN           Publish;
  BOOLEAN           Immortal;
  ULONG             Age;
  NL_ROUTE_ORIGIN   Origin;
} MIB_IPFORWARD_ROW2, *PMIB_IPFORWARD_ROW2;

typedef VOID (NETIOAPI_API_ *PIPFORWARD_CHANGE_CALLBACK) (PVOID CallerContext, PMIB_IPFORWARD_ROW2 Row, MIB_NOTIFICATION_TYPE NotificationType);

typedef VOID (WINAPI *PUNICAST_IPADDRESS_CHANGE_CALLBACK)(PVOID, PMIB_UNICASTIPADDRESS_ROW,
                                                          MIB_NOTIFICATION_TYPE);

typedef void (WINAPI * PIO_APC_ROUTINE)(PVOID,PIO_STATUS_BLOCK,ULONG);

typedef ULONG NET_IFINDEX, *PNET_IFINDEX;

typedef NET_IFINDEX IF_INDEX, *PIF_INDEX;

typedef enum _MIB_IF_TABLE_LEVEL { 
  MibIfTableNormal                   = 0,
  MibIfTableRaw                      = 1,
  MibIfTableNormalWithoutStatistics  = 2
} MIB_IF_TABLE_LEVEL, *PMIB_IF_TABLE_LEVEL;

typedef struct _InterfaceIndexTable {
   DWORD numIndexes;
   IF_INDEX indexes[1];
} InterfaceIndexTable;

typedef enum _NL_ROUTER_DISCOVERY_BEHAVIOR { 
  RouterDiscoveryDisabled   = 0,
  RouterDiscoveryEnabled,
  RouterDiscoveryDhcp,
  RouterDiscoveryUnchanged  = -1
} NL_ROUTER_DISCOVERY_BEHAVIOR;

typedef enum _NL_LINK_LOCAL_ADDRESS_BEHAVIOR { 
  LinkLocalAlwaysOff  = 0,
  LinkLocalDelayed,
  LinkLocalAlwaysOn,
  LinkLocalUnchanged  = -1
} NL_LINK_LOCAL_ADDRESS_BEHAVIOR;

typedef struct _NL_INTERFACE_OFFLOAD_ROD {
  BOOLEAN NlChecksumSupported  :1;
  BOOLEAN NlOptionsSupported  :1;
  BOOLEAN TlDatagramChecksumSupported  :1;
  BOOLEAN TlStreamChecksumSupported  :1;
  BOOLEAN TlStreamOptionsSupported  :1;
  BOOLEAN TlStreamFastPathCompatible  :1;
  BOOLEAN TlDatagramFastPathCompatible  :1;
  BOOLEAN TlLargeSendOffloadSupported  :1;
  BOOLEAN TlGiantSendOffloadSupported  :1;
} NL_INTERFACE_OFFLOAD_ROD, *PNL_INTERFACE_OFFLOAD_ROD;

typedef struct _MIB_IPINTERFACE_ROW {
  ADDRESS_FAMILY                 Family;
  NET_LUID                       InterfaceLuid;
  NET_IFINDEX                    InterfaceIndex;
  ULONG                          MaxReassemblySize;
  ULONG64                        InterfaceIdentifier;
  ULONG                          MinRouterAdvertisementInterval;
  ULONG                          MaxRouterAdvertisementInterval;
  BOOLEAN                        AdvertisingEnabled;
  BOOLEAN                        ForwardingEnabled;
  BOOLEAN                        WeakHostSend;
  BOOLEAN                        WeakHostReceive;
  BOOLEAN                        UseAutomaticMetric;
  BOOLEAN                        UseNeighborUnreachabilityDetection;
  BOOLEAN                        ManagedAddressConfigurationSupported;
  BOOLEAN                        OtherStatefulConfigurationSupported;
  BOOLEAN                        AdvertiseDefaultRoute;
  NL_ROUTER_DISCOVERY_BEHAVIOR   RouterDiscoveryBehavior;
  ULONG                          DadTransmits;
  ULONG                          BaseReachableTime;
  ULONG                          RetransmitTime;
  ULONG                          PathMtuDiscoveryTimeout;
  NL_LINK_LOCAL_ADDRESS_BEHAVIOR LinkLocalAddressBehavior;
  ULONG                          LinkLocalAddressTimeout;
  ULONG                          ZoneIndices[ScopeLevelCount];
  ULONG                          SitePrefixLength;
  ULONG                          Metric;
  ULONG                          NlMtu;
  BOOLEAN                        Connected;
  BOOLEAN                        SupportsWakeUpPatterns;
  BOOLEAN                        SupportsNeighborDiscovery;
  BOOLEAN                        SupportsRouterDiscovery;
  ULONG                          ReachableTime;
  NL_INTERFACE_OFFLOAD_ROD       TransmitOffload;
  NL_INTERFACE_OFFLOAD_ROD       ReceiveOffload;
  BOOLEAN                        DisableDefaultRoutes;
} MIB_IPINTERFACE_ROW, *PMIB_IPINTERFACE_ROW;

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

int Ipv4Mask[8];

void initialization()
{
	Ipv4Mask[24] = 0;
	Ipv4Mask[25] = 128;
	Ipv4Mask[26] = 192;
	Ipv4Mask[27] = 224;
	Ipv4Mask[28] = 240;
	Ipv4Mask[29] = 248;
	Ipv4Mask[30] = 252;
	Ipv4Mask[31] = 254;	
}

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    TRACE("fdwReason %u\n", fdwReason);

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}

ULONG WINAPI ResolveNeighbor(
  _In_     SOCKADDR *NetworkAddress,
  _Out_    PVOID PhysicalAddress,
  _Inout_  PULONG PhysicalAddressLength
)
{
	return ERROR_NOT_SUPPORTED;
}

DWORD WINAPI GetIfEntry2(PMIB_IF_ROW2 Row)
{
  return ERROR_NOT_SUPPORTED;
}

DWORD WINAPI ParseNetworkString(
  _In_       const WCHAR *NetworkString,
  _In_       DWORD Types,
  _Out_opt_  PNET_ADDRESS_INFO AddressInfo,
  _Out_opt_  USHORT *PortNumber,
  _Out_opt_  BYTE *PrefixLength
)
{
	return ERROR_INVALID_PARAMETER;
}

DWORD WINAPI ConvertLengthToIpv4Mask(ULONG MaskLength, PULONG Mask)
{
  DWORD result; // eax@2

  if ( MaskLength >= 33 )
  {
    *Mask = -1;
    result = 87;
  }
  else
  {
	initialization();
    *Mask = Ipv4Mask[MaskLength];
    result = 0;
  }
  return result;
}

DWORD WINAPI ConvertInterfaceGuidToLuid(
  _In_   const GUID *InterfaceGuid,
  _Out_  PNET_LUID InterfaceLuid
)
{
	UNIMPLEMENTED;
	return ERROR_INVALID_PARAMETER;
}

DWORD WINAPI ConvertInterfaceLuidToIndex(
  _In_   const NET_LUID *InterfaceLuid,
  _Out_  PNET_IFINDEX InterfaceIndex
)
{
	UNIMPLEMENTED;
	return ERROR_INVALID_PARAMETER;
}

DWORD WINAPI CancelMibChangeNotify2(
  _In_  HANDLE NotificationHandle
)
{
	UNIMPLEMENTED;
	return ERROR_INVALID_PARAMETER;
}

VOID WINAPI FreeMibTable(
  _In_  PVOID Memory
)
{
	;
}

DWORD WINAPI GetTeredoPort(
  _Out_  USHORT *Port
)
{
	UNIMPLEMENTED;
	return ERROR_NOT_SUPPORTED;
}

DWORD WINAPI NotifyStableUnicastIpAddressTable(
  _In_     ADDRESS_FAMILY  Family,
  _Inout_  PMIB_UNICASTIPADDRESS_TABLE *Table,
  _In_     PSTABLE_UNICAST_IPADDRESS_TABLE_CALLBACK CallerCallback,
  _In_     PVOID CallerContext,
  _Inout_  HANDLE *NotificationHandle
)
{
	UNIMPLEMENTED;
	return ERROR_NOT_SUPPORTED;
}

DWORD 
WINAPI 
ResolveIpNetEntry2(
  _Inout_   PMIB_IPNET_ROW2 Row,
  _In_opt_  const SOCKADDR_INET *SourceAddress
)
{
	UNIMPLEMENTED;
	return ERROR_NOT_SUPPORTED;
}

DWORD WINAPI GetIpNetEntry2(
  _Inout_  PMIB_IPNET_ROW2 Row
)
{
	UNIMPLEMENTED;
	return ERROR_NOT_SUPPORTED;
}

/******************************************************************
 *    ConvertInterfaceLuidToGuid (IPHLPAPI.@)
 */
DWORD 
WINAPI 
ConvertInterfaceLuidToGuid(
	const NET_LUID *luid, 
	GUID *guid
)
{
    DWORD ret;
    MIB_IFROW row;

    TRACE("(%p %p)\n", luid, guid);

    if (!luid || !guid) return ERROR_INVALID_PARAMETER;

    row.dwIndex = luid->Info.NetLuidIndex;
    if ((ret = GetIfEntry( &row ))) return ret;

    guid->Data1 = luid->Info.NetLuidIndex;
    return NO_ERROR;
}

/***********************************************************************
 *		IcmpSendEcho2Ex (IPHLPAPI.@)
 */
DWORD WINAPI IcmpSendEcho2Ex(
    HANDLE                   IcmpHandle,
    HANDLE                   Event,
    PIO_APC_ROUTINE          ApcRoutine,
    PVOID                    ApcContext,
    IPAddr                   SourceAddress,
    IPAddr                   DestinationAddress,
    LPVOID                   RequestData,
    WORD                     RequestSize,
    PIP_OPTION_INFORMATION   RequestOptions,
    LPVOID                   ReplyBuffer,
    DWORD                    ReplySize,
    DWORD                    Timeout
    )
{
    TRACE("(%p, %p, %p, %p, %08x, %08x, %p, %d, %p, %p, %d, %d): stub\n", IcmpHandle,
            Event, ApcRoutine, ApcContext, SourceAddress, DestinationAddress, RequestData,
            RequestSize, RequestOptions, ReplyBuffer, ReplySize, Timeout);

    if (Event)
    {
        FIXME("unsupported for events\n");
        return 0;
    }
    if (ApcRoutine)
    {
        FIXME("unsupported for APCs\n");
        return 0;
    }
    if (SourceAddress)
    {
        FIXME("unsupported for source addresses\n");
        return 0;
    }

    return IcmpSendEcho(IcmpHandle, DestinationAddress, RequestData,
            RequestSize, RequestOptions, ReplyBuffer, ReplySize, Timeout);
}

DWORD get_interface_indices( BOOL skip_loopback, InterfaceIndexTable **table )
{
    if (table) *table = NULL;
    return 0;
}

/******************************************************************
 *    GetIfTable2Ex (IPHLPAPI.@)
 */
DWORD WINAPI GetIfTable2Ex( MIB_IF_TABLE_LEVEL level, MIB_IF_TABLE2 **table )
{
    DWORD i, nb_interfaces, size = sizeof(MIB_IF_TABLE2);
    InterfaceIndexTable *index_table;
    MIB_IF_TABLE2 *ret;

    TRACE( "level %u, table %p\n", level, table );

    if (!table || level > MibIfTableRaw)
        return ERROR_INVALID_PARAMETER;

    if (level != MibIfTableNormal)
        FIXME("level %u not fully supported\n", level);

    if ((nb_interfaces = get_interface_indices( FALSE, NULL )) > 1)
        size += (nb_interfaces - 1) * sizeof(MIB_IF_ROW2);

    if (!(ret = HeapAlloc( GetProcessHeap(), 0, size ))) return ERROR_OUTOFMEMORY;

    get_interface_indices( FALSE, &index_table );
    if (!index_table)
    {
        HeapFree( GetProcessHeap(), 0, ret );
        return ERROR_OUTOFMEMORY;
    }

    ret->NumEntries = 0;
    for (i = 0; i < index_table->numIndexes; i++)
    {
        ret->Table[i].InterfaceIndex = index_table->indexes[i];
        GetIfEntry2( &ret->Table[i] );
        ret->NumEntries++;
    }

    HeapFree( GetProcessHeap(), 0, index_table );
    *table = ret;
    return NO_ERROR;
}

/******************************************************************
 *    GetIfTable2 (IPHLPAPI.@)
 */
DWORD WINAPI GetIfTable2( MIB_IF_TABLE2 **table )
{
    TRACE( "table %p\n", table );
    return GetIfTable2Ex(MibIfTableNormal, table);
}

NETIOAPI_API GetIpInterfaceEntry(
  _Inout_ PMIB_IPINTERFACE_ROW Row
)
{
	return ERROR_NOT_FOUND;
}

/******************************************************************
 *    NotifyUnicastIpAddressChange (IPHLPAPI.@)
 */
DWORD WINAPI NotifyUnicastIpAddressChange(ADDRESS_FAMILY family, PUNICAST_IPADDRESS_CHANGE_CALLBACK callback,
                                          PVOID context, BOOLEAN init_notify, PHANDLE handle)
{
    FIXME("(family %d, callback %p, context %p, init_notify %d, handle %p): stub\n",
          family, callback, context, init_notify, handle);
    if (handle) *handle = NULL;
    return ERROR_NOT_SUPPORTED;
}

NETIOAPI_API NotifyRouteChange2(
  _In_    ADDRESS_FAMILY             Family,
  _In_    PIPFORWARD_CHANGE_CALLBACK Callback,
  _In_    PVOID                      CallerContext,
  _In_    BOOLEAN                    InitialNotification,
  _Inout_ HANDLE                     *NotificationHandle
)
{
	return ERROR_NOT_FOUND;
}

NETIOAPI_API GetBestRoute2(
  _In_opt_       NET_LUID            *InterfaceLuid,
  _In_           NET_IFINDEX         InterfaceIndex,
  _In_     const SOCKADDR_INET       *SourceAddress,
  _In_     const SOCKADDR_INET       *DestinationAddress,
  _In_           ULONG               AddressSortOptions,
  _Out_          PMIB_IPFORWARD_ROW2 BestRoute,
  _Out_          SOCKADDR_INET       *BestSourceAddress
)
{
	return ERROR_NOT_FOUND;
}