#define _WS2IPDEF_

#include <wine/debug.h>
#include <winbase.h>
#include <ntsecapi.h>
#include <iphlpapi.h>
#include <netioapi.h>

#define ANY_SIZE 1
#define DNS_MAX_NAME_BUFFER_LENGTH (256)

typedef enum {
  NldsInvalid,
  NldsTentative,
  NldsDuplicate,
  NldsDeprecated,
  NldsPreferred,
  IpDadStateInvalid = 0,
  IpDadStateTentative,
  IpDadStateDuplicate,
  IpDadStateDeprecated,
  IpDadStatePreferred,
} NL_DAD_STATE;

typedef enum {
  NlsoOther = 0,
  NlsoManual,
  NlsoWellKnown,
  NlsoDhcp,
  NlsoLinkLayerAddress,
  NlsoRandom,
  IpSuffixOriginOther = 0,
  IpSuffixOriginManual,
  IpSuffixOriginWellKnown,
  IpSuffixOriginDhcp,
  IpSuffixOriginLinkLayerAddress,
  IpSuffixOriginRandom,
  IpSuffixOriginUnchanged = 1 << 4
} NL_SUFFIX_ORIGIN;

typedef enum {
  IpPrefixOriginOther = 0,
  IpPrefixOriginManual,
  IpPrefixOriginWellKnown,
  IpPrefixOriginDhcp,
  IpPrefixOriginRouterAdvertisement,
  IpPrefixOriginUnchanged = 1 << 4
} NL_PREFIX_ORIGIN;

typedef USHORT ADDRESS_FAMILY;

typedef struct SOCKADDR {
        USHORT  sa_family;
        char    sa_data[14];
}SOCKADDR;

typedef struct {
  union {
    struct {
      ULONG Zone:28;
      ULONG Level:4;
    };
    ULONG Value;
  };
} SCOPE_ID, *PSCOPE_ID;

typedef struct in_addr {
  union {
    struct {UCHAR s_b1,s_b2,s_b3,s_b4;} S_un_b;
    struct {USHORT s_w1,s_w2;} S_un_w;
    ULONG S_addr;
  } S_un;
} IN_ADDR, *PIN_ADDR, FAR *LPIN_ADDR;

typedef struct sockaddr_in {
  ADDRESS_FAMILY sin_family;
  USHORT sin_port;
  IN_ADDR sin_addr;
  CHAR sin_zero[8];
} SOCKADDR_IN, *PSOCKADDR_IN;

typedef struct in6_addr {
  union {
    UCHAR Byte[16];
    USHORT Word[8];
  } u;
} IN6_ADDR, *PIN6_ADDR, FAR *LPIN6_ADDR;

typedef struct sockaddr_in6 {
  ADDRESS_FAMILY sin6_family;
  USHORT sin6_port;
  ULONG sin6_flowinfo;
  IN6_ADDR sin6_addr;
  union {
    ULONG sin6_scope_id;
    SCOPE_ID sin6_scope_struct;
  };
} SOCKADDR_IN6, *PSOCKADDR_IN6, FAR *LPSOCKADDR_IN6;

typedef struct _SOCKADDR_INET {
  SOCKADDR_IN    Ipv4;
  SOCKADDR_IN6   Ipv6;
  ADDRESS_FAMILY si_family;
} SOCKADDR_INET, *PSOCKADDR_INET;

typedef enum _NL_NEIGHBOR_STATE { 
  NlnsUnreachable,
  NlnsIncomplete,
  NlnsProbe,
  NlnsDelay,
  NlnsStale,
  NlnsReachable,
  NlnsPermanent,
  NlnsMaximum
} NL_NEIGHBOR_STATE, *PNL_NEIGHBOR_STATE;

typedef struct _MIB_IPNET_ROW2 {
  PSOCKADDR_INET     Address;
  NET_IFINDEX       InterfaceIndex;
  NET_LUID          InterfaceLuid;
   UCHAR            PhysicalAddress[IF_MAX_PHYS_ADDRESS_LENGTH];
  ULONG             PhysicalAddressLength;
  NL_NEIGHBOR_STATE State;
  union {
    struct {
      BOOLEAN IsRouter  :1;
      BOOLEAN IsUnreachable  :1;
    };
    UCHAR  Flags;
  };
  union {
    ULONG LastReachable;
    ULONG LastUnreachable;
  } ReachabilityTime;
} MIB_IPNET_ROW2, *PMIB_IPNET_ROW2;

typedef struct _MIB_UNICASTIPADDRESS_ROW {
  SOCKADDR_INET    Address;
  NET_LUID         InterfaceLuid;
  NET_IFINDEX      InterfaceIndex;
  NL_PREFIX_ORIGIN PrefixOrigin;
  NL_SUFFIX_ORIGIN SuffixOrigin;
  ULONG            ValidLifetime;
  ULONG            PreferredLifetime;
  UINT8            OnLinkPrefixLength;
  BOOLEAN          SkipAsSource;
  NL_DAD_STATE     DadState;
  SCOPE_ID         ScopeId;
  LARGE_INTEGER    CreationTimeStamp;
} MIB_UNICASTIPADDRESS_ROW, *PMIB_UNICASTIPADDRESS_ROW;

typedef struct _MIB_UNICASTIPADDRESS_TABLE {
  ULONG                    NumEntries;
  MIB_UNICASTIPADDRESS_ROW Table[ANY_SIZE];
} MIB_UNICASTIPADDRESS_TABLE, *PMIB_UNICASTIPADDRESS_TABLE;

typedef enum NET_ADDRESS_FORMAT_ {
    NET_ADDRESS_FORMAT_UNSPECIFIED  = 0,
    NET_ADDRESS_DNS_NAME            = 1,
    NET_ADDRESS_IPV4                = 2,
    NET_ADDRESS_IPV6                = 3
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

typedef VOID (*PSTABLE_UNICAST_IPADDRESS_TABLE_CALLBACK)(PVOID,PMIB_UNICASTIPADDRESS_TABLE);
