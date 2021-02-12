/*++

Copyright (c) 2018 Shorthorn Project.

Module Name:

    iphlpapi.h

Abstract:

    Header for functions related with iphpalpi for support
    new applications designed for Vista or above.

Revision History:

    18-04-2018

--*/

#include "windef.h"
#include "winbase.h"
#include <iotypes.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <ifdef.h>
#include <tcpioctl.h>
#include <netioapi.h>
#include <tdiinfo.h>
#include <tdilib.h>
#include <wine/debug.h>
#include <wine/unicode.h>
#include <ntstatus.h>
#include <winsock.h>
#define WIN32_NO_STATUS

#define ScopeLevelCount 16

#define DNS_MAX_NAME_BUFFER_LENGTH 256

#ifndef ANY_SIZE
#define ANY_SIZE 1
#endif

#define WS_AF_INET                 2
#define WS_AF_INET6                23

#define NT_SUCCESS(StatCode) ((NTSTATUS)(StatCode) >= 0)

#define TCP_REQUEST_QUERY_INFORMATION_INIT { { { 0 } } }
#define TCP_REQUEST_SET_INFORMATION_INIT { { 0 } }

typedef USHORT ADDRESS_FAMILY;

typedef enum _MIB_NOTIFICATION_TYPE
{
    MibParameterNotification,
    MibAddInstance,
    MibDeleteInstance,
    MibInitialNotification,
} MIB_NOTIFICATION_TYPE, *PMIB_NOTIFICATION_TYPE;

typedef struct sockaddr_in6 {
  short sin6_family;
  USHORT sin6_port;
  ULONG sin6_flowinfo;
  struct in6_addr sin6_addr;
  ULONG sin6_scope_id;
} SOCKADDR_IN6, *PSOCKADDR_IN6, FAR *LPSOCKADDR_IN6;

typedef union _SOCKADDR_INET {
  SOCKADDR_IN Ipv4;
  SOCKADDR_IN6 Ipv6;
  ADDRESS_FAMILY si_family;
} SOCKADDR_INET, *PSOCKADDR_INET;

typedef struct _IP_ADDRESS_PREFIX {
  SOCKADDR_INET Prefix;
  UINT8         PrefixLength;
} IP_ADDRESS_PREFIX, *PIP_ADDRESS_PREFIX;

// typedef enum _NL_ROUTE_ORIGIN { 
  // NlroManual,
  // NlroWellKnown,
  // NlroDHCP,
  // NlroRouterAdvertisement,
  // Nlro6to4
// } NL_ROUTE_ORIGIN, *PNL_ROUTE_ORIGIN;

#define MAKE_ROUTE_PROTOCOL(N, V) MIB_IPPROTO_ ## N = V, PROTO_IP_ ## N = V

// typedef enum {
  // RouteProtocolOther = 1,
  // RouteProtocolLocal = 2,
  // RouteProtocolNetMgmt = 3,
  // RouteProtocolIcmp = 4,
  // RouteProtocolEgp = 5,
  // RouteProtocolGgp = 6,
  // RouteProtocolHello = 7,
  // RouteProtocolRip = 8,
  // RouteProtocolIsIs = 9,
  // RouteProtocolEsIs = 10,
  // RouteProtocolCisco = 11,
  // RouteProtocolBbn = 12,
  // RouteProtocolOspf = 13,
  // RouteProtocolBgp = 14
// } NL_ROUTE_PROTOCOL,*PNL_ROUTE_PROTOCOL;

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

typedef struct
{
    union {
        struct {
            ULONG Zone  : 28;
            ULONG Level : 4;
        } DUMMYSTRUCTNAME;
        ULONG Value;
    } DUMMYUNIONNAME;
} SCOPE_ID, *PSCOPE_ID;

typedef struct _MIB_UNICASTIPADDRESS_ROW
{
    SOCKADDR_INET       Address;
    NET_LUID            InterfaceLuid;
    NET_IFINDEX         InterfaceIndex;
    NL_PREFIX_ORIGIN    PrefixOrigin;
    NL_SUFFIX_ORIGIN    SuffixOrigin;
    ULONG               ValidLifetime;
    ULONG               PreferredLifetime;
    UINT8               OnLinkPrefixLength;
    BOOLEAN             SkipAsSource;
    NL_DAD_STATE        DadState;
    SCOPE_ID            ScopeId;
    LARGE_INTEGER       CreationTimeStamp;
} MIB_UNICASTIPADDRESS_ROW, *PMIB_UNICASTIPADDRESS_ROW;

typedef VOID (WINAPI *PIPFORWARD_CHANGE_CALLBACK) (PVOID CallerContext, PMIB_IPFORWARD_ROW2 Row, MIB_NOTIFICATION_TYPE NotificationType);

typedef VOID (WINAPI *PUNICAST_IPADDRESS_CHANGE_CALLBACK)(PVOID, PMIB_UNICASTIPADDRESS_ROW,
                                                          MIB_NOTIFICATION_TYPE);

typedef void (WINAPI * PIO_APC_ROUTINE)(PVOID,PIO_STATUS_BLOCK,ULONG);

typedef ULONG NET_IFINDEX, *PNET_IFINDEX;

typedef NET_IFINDEX IF_INDEX, *PIF_INDEX;

typedef struct _sockaddr_in6_pair {
  PSOCKADDR_IN6 SourceAddress;
  PSOCKADDR_IN6 DestinationAddress;
} SOCKADDR_IN6_PAIR, *PSOCKADDR_IN6_PAIR;

typedef enum _MIB_IF_TABLE_LEVEL { 
  MibIfTableNormal                   = 0,
  MibIfTableRaw                      = 1,
  MibIfTableNormalWithoutStatistics  = 2
} MIB_IF_TABLE_LEVEL, *PMIB_IF_TABLE_LEVEL;

typedef struct _InterfaceIndexTable {
   DWORD numIndexes;
   IF_INDEX indexes[1];
} InterfaceIndexTable;

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

typedef enum _NDIS_MEDIUM
{
    NdisMedium802_3,
    NdisMedium802_5,
    NdisMediumFddi,
    NdisMediumWan,
    NdisMediumLocalTalk,
    NdisMediumDix,
    NdisMediumArcnetRaw,
    NdisMediumArcnet878_2,
    NdisMediumAtm,
    NdisMediumWirelessWan,
    NdisMediumIrda,
    NdisMediumBpc,
    NdisMediumCoWan,
    NdisMedium1394,
    NdisMediumInfiniBand,
    NdisMediumTunnel,
    NdisMediumNative802_11,
    NdisMediumLoopback,
    NdisMediumWiMAX,
    NdisMediumIP,
    NdisMediumMax
} NDIS_MEDIUM, *PNDIS_MEDIUM;

typedef enum _NDIS_PHYSICAL_MEDIUM
{
    NdisPhysicalMediumUnspecified,
    NdisPhysicalMediumWirelessLan,
    NdisPhysicalMediumCableModem,
    NdisPhysicalMediumPhoneLine,
    NdisPhysicalMediumPowerLine,
    NdisPhysicalMediumDSL,
    NdisPhysicalMediumFibreChannel,
    NdisPhysicalMedium1394,
    NdisPhysicalMediumWirelessWan,
    NdisPhysicalMediumNative802_11,
    NdisPhysicalMediumBluetooth,
    NdisPhysicalMediumInfiniband,
    NdisPhysicalMediumWiMax,
    NdisPhysicalMediumUWB,
    NdisPhysicalMedium802_3,
    NdisPhysicalMedium802_5,
    NdisPhysicalMediumIrda,
    NdisPhysicalMediumWiredWAN,
    NdisPhysicalMediumWiredCoWan,
    NdisPhysicalMediumOther,
    NdisPhysicalMediumMax
} NDIS_PHYSICAL_MEDIUM, *PNDIS_PHYSICAL_MEDIUM;

typedef struct _MIB_IF_ROW2
{
    NET_LUID InterfaceLuid;
    NET_IFINDEX InterfaceIndex;
    GUID InterfaceGuid;
    WCHAR Alias[IF_MAX_STRING_SIZE + 1];
    WCHAR Description[IF_MAX_STRING_SIZE + 1];
    ULONG PhysicalAddressLength;
    UCHAR PhysicalAddress[IF_MAX_PHYS_ADDRESS_LENGTH];
    UCHAR PermanentPhysicalAddress[IF_MAX_PHYS_ADDRESS_LENGTH];
    ULONG Mtu;
    IFTYPE Type;
    TUNNEL_TYPE TunnelType;
    NDIS_MEDIUM MediaType;
    NDIS_PHYSICAL_MEDIUM PhysicalMediumType;
    NET_IF_ACCESS_TYPE AccessType;
    NET_IF_DIRECTION_TYPE DirectionType;
    struct
    {
        BOOLEAN HardwareInterface : 1;
        BOOLEAN FilterInterface : 1;
        BOOLEAN ConnectorPresent : 1;
        BOOLEAN NotAuthenticated : 1;
        BOOLEAN NotMediaConnected : 1;
        BOOLEAN Paused : 1;
        BOOLEAN LowPower : 1;
        BOOLEAN EndPointInterface : 1;
    } InterfaceAndOperStatusFlags;
    IF_OPER_STATUS OperStatus;
    NET_IF_ADMIN_STATUS AdminStatus;
    NET_IF_MEDIA_CONNECT_STATE MediaConnectState;
    NET_IF_NETWORK_GUID NetworkGuid;
    NET_IF_CONNECTION_TYPE ConnectionType;
    ULONG64 TransmitLinkSpeed;
    ULONG64 ReceiveLinkSpeed;
    ULONG64 InOctets;
    ULONG64 InUcastPkts;
    ULONG64 InNUcastPkts;
    ULONG64 InDiscards;
    ULONG64 InErrors;
    ULONG64 InUnknownProtos;
    ULONG64 InUcastOctets;
    ULONG64 InMulticastOctets;
    ULONG64 InBroadcastOctets;
    ULONG64 OutOctets;
    ULONG64 OutUcastPkts;
    ULONG64 OutNUcastPkts;
    ULONG64 OutDiscards;
    ULONG64 OutErrors;
    ULONG64 OutUcastOctets;
    ULONG64 OutMulticastOctets;
    ULONG64 OutBroadcastOctets;
    ULONG64 OutQLen;
} MIB_IF_ROW2, *PMIB_IF_ROW2;

typedef enum  { 
  NET_ADDRESS_FORMAT_UNSPECIFIED  = 0,
  NET_ADDRESS_DNS_NAME,
  NET_ADDRESS_IPV4,
  NET_ADDRESS_IPV6
} NET_ADDRESS_FORMAT;

typedef struct NET_ADDRESS_INFO_ {
  NET_ADDRESS_FORMAT Format;
  union {
    struct {
      WCHAR Address[DNS_MAX_NAME_BUFFER_LENGTH];
      WCHAR Port[6];
    } NamedAddress;
    SOCKADDR_IN  Ipv4Address;
    SOCKADDR_IN6 Ipv6Address;
    SOCKADDR     IpAddress;
  };
} NET_ADDRESS_INFO, *PNET_ADDRESS_INFO;

typedef struct _MIB_UNICASTIPADDRESS_TABLE {
    ULONG NumEntries;
    MIB_UNICASTIPADDRESS_ROW Table[ANY_SIZE];
} MIB_UNICASTIPADDRESS_TABLE, *PMIB_UNICASTIPADDRESS_TABLE;

typedef
VOID
(NETIOAPI_API_ *PSTABLE_UNICAST_IPADDRESS_TABLE_CALLBACK) (
    _In_ PVOID CallerContext,
    _In_ PMIB_UNICASTIPADDRESS_TABLE AddressTable
);

//
// ARP and IPv6 Neighbor management routines.
//

typedef struct _MIB_IPNET_ROW2 {
    //
    // Key Struture.
    //
    SOCKADDR_INET Address;
    NET_IFINDEX InterfaceIndex;
    NET_LUID InterfaceLuid;

    //
    // Read-Write.
    //
    UCHAR PhysicalAddress[IF_MAX_PHYS_ADDRESS_LENGTH];

    //
    // Read-Only.
    //
    ULONG PhysicalAddressLength;
    NL_NEIGHBOR_STATE State;

    union {
        struct {
            BOOLEAN IsRouter : 1;
            BOOLEAN IsUnreachable : 1;
        };
        UCHAR Flags;
    };

    union {
        ULONG LastReachable;
        ULONG LastUnreachable;
    } ReachabilityTime;
} MIB_IPNET_ROW2, *PMIB_IPNET_ROW2;

typedef struct _MIB_IF_TABLE2 {
    ULONG NumEntries;
    MIB_IF_ROW2 Table[ANY_SIZE];
} MIB_IF_TABLE2, *PMIB_IF_TABLE2;

typedef union _IFEntrySafelySized {
    CHAR MaxSize[sizeof(DWORD) +
		 sizeof(IFEntry) +
		 MAX_ADAPTER_DESCRIPTION_LENGTH + 1];
    IFEntry ent;
} IFEntrySafelySized;

/* Encapsulates information about an interface */
typedef struct _IFInfo {
    TDIEntityID        entity_id;
    IFEntrySafelySized if_info;
    IPAddrEntry        ip_addr;
} IFInfo;

typedef VOID (WINAPI *PIPINTERFACE_CHANGE_CALLBACK)(PVOID, PMIB_IPINTERFACE_ROW,
                                                          MIB_NOTIFICATION_TYPE);