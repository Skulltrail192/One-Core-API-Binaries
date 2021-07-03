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
#include "windows.h"
#include "winbase.h"
// #include <iotypes.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <ifdef.h>
#include <tcpioctl.h>
#include <winsock2.h>
#include <ws2ipdef.h>
#include <netioapi.h>
#include <iptypes.h>
#include <tdiinfo.h>
#include <tdilib.h>
#include <wine/debug.h>
#include <tcpestats.h>
#include <wine/unicode.h>
#include <wine/winternl.h>
#include <stdio.h>

//Hack to not include ntstatus.h or similar
#define STATUS_SUCCESS                ((NTSTATUS)0x00000000L)
#define STATUS_UNSUCCESSFUL           ((NTSTATUS)0xC0000001L)
#define STATUS_INSUFFICIENT_RESOURCES    ((NTSTATUS)0xC000009AL)     // ntsubauth

#define WS_AF_UNSPEC               0

#ifndef IF_NAMESIZE
#define IF_NAMESIZE 16
#endif

#define DNS_MAX_NAME_BUFFER_LENGTH 256

#ifndef ANY_SIZE
#define ANY_SIZE 1
#endif

#define WS_AF_INET                 2
#define WS_AF_INET6                23

#define NT_SUCCESS(StatCode) ((NTSTATUS)(StatCode) >= 0)

#define TCP_REQUEST_QUERY_INFORMATION_INIT { { { 0 } } }
#define TCP_REQUEST_SET_INFORMATION_INIT { { 0 } }

#define MAX_INTERFACE_PHYSADDR    8
#define MAX_INTERFACE_DESCRIPTION 256

#define 	CP_UNIXCP   CP_ACP

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

typedef VOID (WINAPI *PIPFORWARD_CHANGE_CALLBACK) (PVOID CallerContext, PMIB_IPFORWARD_ROW2 Row, MIB_NOTIFICATION_TYPE NotificationType);

typedef VOID (WINAPI *PUNICAST_IPADDRESS_CHANGE_CALLBACK)(PVOID, PMIB_UNICASTIPADDRESS_ROW,
                                                          MIB_NOTIFICATION_TYPE);

typedef void (WINAPI * PIO_APC_ROUTINE)(PVOID,PIO_STATUS_BLOCK,ULONG);

typedef ULONG NET_IFINDEX, *PNET_IFINDEX;

typedef NET_IFINDEX IF_INDEX, *PIF_INDEX;

typedef struct _InterfaceIndexTable {
   DWORD numIndexes;
   IF_INDEX indexes[1];
} InterfaceIndexTable;

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

typedef
VOID
(NETIOAPI_API_ *PSTABLE_UNICAST_IPADDRESS_TABLE_CALLBACK) (
    _In_ PVOID CallerContext,
    _In_ PMIB_UNICASTIPADDRESS_TABLE AddressTable
);

//
// ARP and IPv6 Neighbor management routines.
//

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

typedef enum _IPHLPAddrType {
    IPAAddr, IPABcast, IPAMask, IFMtu, IFStatus
} IPHLPAddrType;

typedef VOID (WINAPI *PIPINTERFACE_CHANGE_CALLBACK)(PVOID, PMIB_IPINTERFACE_ROW,
                                                          MIB_NOTIFICATION_TYPE);
														  
														  /* ByName/ByIndex versions of various getter functions. */

/* can be used as quick check to see if you've got a valid index, returns NULL
 * if not.  Overwrites your buffer, which should be at least of size
 * MAX_ADAPTER_NAME.
 */
char *getInterfaceNameByIndex(IF_INDEX index, char *name) DECLSPEC_HIDDEN;

/* Fills in the MIB_IFROW by name.  Doesn't fill in interface statistics,
 * see ipstats.h for that.
 * Returns ERROR_INVALID_PARAMETER if name is NULL, ERROR_INVALID_DATA
 * if name isn't valid, and NO_ERROR otherwise.
 */
DWORD getInterfaceEntryByName(const char *name, PMIB_IFROW entry) DECLSPEC_HIDDEN;


/* Fills in entry's interface stats, using name to find them.
 * Returns ERROR_INVALID_PARAMETER if name or entry is NULL, NO_ERROR otherwise.
 */
DWORD getInterfaceStatsByName(const char *name, PMIB_IFROW entry) DECLSPEC_HIDDEN;

/* Fills in entry's interface stats, using name to find them.
 * Returns ERROR_INVALID_PARAMETER if name or entry is NULL, NO_ERROR otherwise.
 */
DWORD getInterfaceStatsByName(const char *name, PMIB_IFROW entry);

PCHAR WINAPI if_indextoname(NET_IFINDEX,PCHAR);


/* Gets a few physical charactersistics of a device:  MAC addr len, MAC addr,
 * and type as one of the MIB_IF_TYPEs.
 * len's in-out: on in, needs to say how many bytes are available in addr,
 * which to be safe should be MAX_INTERFACE_PHYSADDR.  On out, it's how many
 * bytes were set, or how many were required if addr isn't big enough.
 * Returns ERROR_INVALID_PARAMETER if name, len, addr, or type is NULL.
 * Returns ERROR_INVALID_DATA if name/index isn't valid.
 * Returns ERROR_INSUFFICIENT_BUFFER if addr isn't large enough for the
 * physical address; *len will contain the required size.
 * May return other errors, e.g. ERROR_OUTOFMEMORY or ERROR_NO_MORE_FILES,
 * if internal errors occur.
 * Returns NO_ERROR on success.
 */
DWORD getInterfacePhysicalByName(const char *name, PDWORD len, PBYTE addr,
 PDWORD type);
 
NTSTATUS getIPAddrEntryForIf(HANDLE tcpFile,
                             char *name,
                             DWORD index,
                             IFInfo *ifInfo); 
							 
/* Get the operational status as a (MIB_)IF_OPER_STATUS type.
 */
DWORD getInterfaceStatusByName(const char *name, PDWORD status);							 