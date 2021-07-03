/*++

Copyright (c) 2018 Shorthorn Project.

Module Name:

    iphlpapi.c

Abstract:

    This file contains functions related with iphpalpi for support
    new applications designed for Vista or above.

Revision History:

    18-04-2018

--*/

#include "main.h" 

WINE_DEFAULT_DEBUG_CHANNEL(iphlpapi);

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
			initialization();
            DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}

static BOOL map_address_6to4( const SOCKADDR_IN6 *addr6, SOCKADDR_IN *addr4 )
{
    ULONG i;

    if (addr6->sin6_family != WS_AF_INET6) return FALSE;

    for (i = 0; i < 5; i++)
        if (addr6->sin6_addr.u.Word[i]) return FALSE;

    if (addr6->sin6_addr.u.Word[5] != 0xffff) return FALSE;

    addr4->sin_family = WS_AF_INET;
    addr4->sin_port   = addr6->sin6_port;
    addr4->sin_addr.S_un.S_addr = addr6->sin6_addr.u.Word[6] << 16 | addr6->sin6_addr.u.Word[7];
    memset( &addr4->sin_zero, 0, sizeof(addr4->sin_zero) );

    return TRUE;
}

static BOOL find_src_address( MIB_IPADDRTABLE *table, const SOCKADDR_IN *dst, SOCKADDR_IN6 *src )
{
    MIB_IPFORWARDROW row;
    DWORD i, j;

    if (GetBestRoute( dst->sin_addr.S_un.S_addr, 0, &row )) return FALSE;

    for (i = 0; i < table->dwNumEntries; i++)
    {
        /* take the first address */
        if (table->table[i].dwIndex == row.dwForwardIfIndex)
        {
            src->sin6_family   = WS_AF_INET6;
            src->sin6_port     = 0;
            src->sin6_flowinfo = 0;
            for (j = 0; j < 5; j++) src->sin6_addr.u.Word[j] = 0;
            src->sin6_addr.u.Word[5] = 0xffff;
            src->sin6_addr.u.Word[6] = table->table[i].dwAddr & 0xffff;
            src->sin6_addr.u.Word[7] = table->table[i].dwAddr >> 16;
            return TRUE;
        }
    }

    return FALSE;
}

ULONG WINAPI ResolveNeighbor(
  _In_     SOCKADDR *NetworkAddress,
  _Out_    PVOID PhysicalAddress,
  _Inout_  PULONG PhysicalAddressLength
)
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

/******************************************************************
 *    ConvertInterfaceGuidToLuid (IPHLPAPI.@)
 */
DWORD WINAPI ConvertInterfaceGuidToLuid(const GUID *guid, NET_LUID *luid)
{
    DWORD ret;
    MIB_IFROW row;

    TRACE("(%s %p)\n", debugstr_guid(guid), luid);

    if (!guid || !luid) return ERROR_INVALID_PARAMETER;

    row.dwIndex = guid->Data1;
    if ((ret = GetIfEntry( &row ))) return ret;

    luid->Info.Reserved     = 0;
    luid->Info.NetLuidIndex = guid->Data1;
    luid->Info.IfType       = row.dwType;
    return NO_ERROR;
}

/******************************************************************
 *    ConvertInterfaceLuidToIndex (IPHLPAPI.@)
 */
DWORD WINAPI ConvertInterfaceLuidToIndex(const NET_LUID *luid, NET_IFINDEX *index)
{
    DWORD ret;
    MIB_IFROW row;

    TRACE("(%p %p)\n", luid, index);

    if (!luid || !index) return ERROR_INVALID_PARAMETER;

    row.dwIndex = luid->Info.NetLuidIndex;
    if ((ret = GetIfEntry( &row ))) return ret;

    *index = luid->Info.NetLuidIndex;
    return NO_ERROR;
}

/******************************************************************
 *    CancelMibChangeNotify2 (IPHLPAPI.@)
 */
DWORD 
WINAPI 
CancelMibChangeNotify2(HANDLE handle)
{
    FIXME("(handle %p): stub\n", handle);
    return NO_ERROR;
}

VOID WINAPI FreeMibTable(
  _In_  PVOID Memory
)
{
	TRACE("(%p)\n", Memory);
	HeapFree(GetProcessHeap(), 0, Memory);
}

DWORD WINAPI GetTeredoPort(
  _Out_  USHORT *Port
)
{
	UNIMPLEMENTED;
	return ERROR_NOT_SUPPORTED;
}

DWORD 
WINAPI
NotifyStableUnicastIpAddressTable(
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

DWORD 
WINAPI 
GetIpNetEntry2(
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

/******************************************************************
 *    GetIfEntry2 (IPHLPAPI.@)
 */
DWORD WINAPI GetIfEntry2( MIB_IF_ROW2 *row2 )
{
    DWORD ret;
    char buf[MAX_ADAPTER_NAME], *name;
    MIB_IFROW row;

    TRACE("%p\n", row2);

    if (!row2 || (!(name = getInterfaceNameByIndex( row2->InterfaceIndex, buf )) &&
                  !(name = getInterfaceNameByIndex( row2->InterfaceLuid.Info.NetLuidIndex, buf ))))
    {
        return ERROR_INVALID_PARAMETER;
    }
    if ((ret = getInterfaceEntryByName( name, &row ))) return ret;
    if ((ret = getInterfaceStatsByName( name, &row ))) return ret;

    memset( row2, 0, sizeof(*row2) );
    row2->InterfaceLuid.Info.Reserved     = 0;
    row2->InterfaceLuid.Info.NetLuidIndex = row.dwIndex;
    row2->InterfaceLuid.Info.IfType       = row.dwType;
    row2->InterfaceIndex                  = row.dwIndex;
    ConvertInterfaceLuidToGuid( &row2->InterfaceLuid, &row2->InterfaceGuid );
    row2->Type                            = row.dwType;
    row2->Mtu                             = row.dwMtu;
    MultiByteToWideChar( CP_UNIXCP, 0, (const char *)row.bDescr, -1, row2->Description, ARRAY_SIZE(row2->Description) );
    MultiByteToWideChar( CP_UNIXCP, 0, (const char *)row.bDescr, -1, row2->Alias, ARRAY_SIZE(row2->Alias) );
    row2->PhysicalAddressLength           = row.dwPhysAddrLen;
    memcpy( &row2->PhysicalAddress, &row.bPhysAddr, row.dwPhysAddrLen );
    memcpy( &row2->PermanentPhysicalAddress, &row.bPhysAddr, row.dwPhysAddrLen );
    row2->OperStatus                      = row.dwOperStatus == MIB_IF_OPER_STATUS_OPERATIONAL ? IfOperStatusUp : IfOperStatusDown;
    row2->AdminStatus                     = NET_IF_ADMIN_STATUS_UP;
    row2->MediaConnectState               = MediaConnectStateConnected;
    row2->ConnectionType                  = NET_IF_CONNECTION_DEDICATED;
    row2->TransmitLinkSpeed = row2->ReceiveLinkSpeed = row.dwSpeed;
    row2->AccessType = (row2->Type == MIB_IF_TYPE_LOOPBACK) ? NET_IF_ACCESS_LOOPBACK : NET_IF_ACCESS_BROADCAST;
    row2->InterfaceAndOperStatusFlags.ConnectorPresent = row2->Type != MIB_IF_TYPE_LOOPBACK;
    row2->InterfaceAndOperStatusFlags.HardwareInterface = row2->Type != MIB_IF_TYPE_LOOPBACK;

    /* stats */
    row2->InOctets        = row.dwInOctets;
    row2->InUcastPkts     = row.dwInUcastPkts;
    row2->InNUcastPkts    = row.dwInNUcastPkts;
    row2->InDiscards      = row.dwInDiscards;
    row2->InErrors        = row.dwInErrors;
    row2->InUnknownProtos = row.dwInUnknownProtos;
    row2->OutOctets       = row.dwOutOctets;
    row2->OutUcastPkts    = row.dwOutUcastPkts;
    row2->OutNUcastPkts   = row.dwOutNUcastPkts;
    row2->OutDiscards     = row.dwOutDiscards;
    row2->OutErrors       = row.dwOutErrors;

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

/******************************************************************
 *    NotifyRouteChange2 (IPHLPAPI.@)
 */
DWORD WINAPI NotifyRouteChange2(ADDRESS_FAMILY family, PIPFORWARD_CHANGE_CALLBACK callback, VOID* context,
                                BOOLEAN init_notify, HANDLE* handle)
{
    FIXME("(family %d, callback %p, context %p, init_notify %d, handle %p): stub\n",
        family, callback, context, init_notify, handle);
    if (handle) *handle = NULL;
    return NO_ERROR;
}

/******************************************************************
 *    GetBestRoute2 (IPHLPAPI.@)
 */
DWORD WINAPI GetBestRoute2(NET_LUID *luid, NET_IFINDEX index,
                           const SOCKADDR_INET *source, const SOCKADDR_INET *destination,
                           ULONG options, PMIB_IPFORWARD_ROW2 bestroute,
                           SOCKADDR_INET *bestaddress)
{
    static int once;

    if (!once++)
        FIXME("(%p, %d, %p, %p, 0x%08x, %p, %p): stub\n", luid, index, source,
                destination, options, bestroute, bestaddress);

    if (!destination || !bestroute || !bestaddress)
        return ERROR_INVALID_PARAMETER;

    return ERROR_NOT_SUPPORTED;
}

/******************************************************************
 *    NotifyIpInterfaceChange (IPHLPAPI.@)
 */
DWORD WINAPI NotifyIpInterfaceChange(ADDRESS_FAMILY family, PIPINTERFACE_CHANGE_CALLBACK callback,
                                     PVOID context, BOOLEAN init_notify, PHANDLE handle)
{
    FIXME("(family %d, callback %p, context %p, init_notify %d, handle %p): stub\n",
          family, callback, context, init_notify, handle);
    if (handle) *handle = NULL;
    return NO_ERROR;
}

/******************************************************************
 *    CreateSortedAddressPairs (IPHLPAPI.@)
 */
DWORD WINAPI CreateSortedAddressPairs( const PSOCKADDR_IN6 src_list, DWORD src_count,
                                       const PSOCKADDR_IN6 dst_list, DWORD dst_count,
                                       DWORD options, PSOCKADDR_IN6_PAIR *pair_list,
                                       DWORD *pair_count )
{
    DWORD i, size, ret;
    SOCKADDR_IN6_PAIR *pairs;
    SOCKADDR_IN6 *ptr;
    SOCKADDR_IN addr4;
    MIB_IPADDRTABLE table;

    FIXME( "(src_list %p src_count %u dst_list %p dst_count %u options %x pair_list %p pair_count %p): stub\n",
           src_list, src_count, dst_list, dst_count, options, pair_list, pair_count );

    if (src_list || src_count || !dst_list || !pair_list || !pair_count || dst_count > 500)
        return ERROR_INVALID_PARAMETER;

    for (i = 0; i < dst_count; i++)
    {
        if (!map_address_6to4( &dst_list[i], &addr4 ))
        {
            FIXME("only mapped IPv4 addresses are supported\n");
            return ERROR_NOT_SUPPORTED;
        }
    }

    size = dst_count * sizeof(*pairs);
    size += dst_count * sizeof(SOCKADDR_IN6) * 2; /* source address + destination address */
    if (!(pairs = HeapAlloc( GetProcessHeap(), 0, size ))) return ERROR_NOT_ENOUGH_MEMORY;
    ptr = (SOCKADDR_IN6 *)&pairs[dst_count];

    if ((ret = GetIpAddrTable( &table, GetProcessHeap(), 0 )))
    {
        HeapFree( GetProcessHeap(), 0, pairs );
        return ret;
    }

    for (i = 0; i < dst_count; i++)
    {
        pairs[i].SourceAddress = ptr++;
        if (!map_address_6to4( &dst_list[i], &addr4 ) ||
            !find_src_address( &table, &addr4, pairs[i].SourceAddress ))
        {
            //char buf[46];
            memset( pairs[i].SourceAddress, 0, sizeof(*pairs[i].SourceAddress) );
            pairs[i].SourceAddress->sin6_family = WS_AF_INET6;
        }

        pairs[i].DestinationAddress = ptr++;
        memcpy( pairs[i].DestinationAddress, &dst_list[i], sizeof(*pairs[i].DestinationAddress) );
    }
    *pair_list = pairs;
    *pair_count = dst_count;

    HeapFree( GetProcessHeap(), 0, &table );
    return NO_ERROR;
}

/******************************************************************
 *    ConvertLengthToIpv4Mask (IPHLPAPI.@)
 */
DWORD WINAPI ConvertLengthToIpv4Mask(ULONG mask_len, ULONG *mask)
{
    if (mask_len > 32)
    {
        *mask = INADDR_NONE;
        return ERROR_INVALID_PARAMETER;
    }

    if (mask_len == 0)
        *mask = 0;
    else
        *mask = htonl(~0u << (32 - mask_len));

    return NO_ERROR;
}

BOOL isInterface( TDIEntityID *if_maybe ) {
    return
        if_maybe->tei_entity == IF_ENTITY;
}

BOOL isIpEntity( HANDLE tcpFile, TDIEntityID *ent ) {
    return (ent->tei_entity == CL_NL_ENTITY ||
            ent->tei_entity == CO_NL_ENTITY);
}

NTSTATUS tdiGetMibForIfEntity
( HANDLE tcpFile, TDIEntityID *ent, IFEntrySafelySized *entry ) {
    TCP_REQUEST_QUERY_INFORMATION_EX req = TCP_REQUEST_QUERY_INFORMATION_INIT;
    NTSTATUS status = STATUS_SUCCESS;
    DWORD returnSize;

    WARN("TdiGetMibForIfEntity(tcpFile %x,entityId %x)\n",
           (int)tcpFile, (int)ent->tei_instance);

    req.ID.toi_class                = INFO_CLASS_PROTOCOL;
    req.ID.toi_type                 = INFO_TYPE_PROVIDER;
    req.ID.toi_id                   = IF_MIB_STATS_ID;
    req.ID.toi_entity               = *ent;

    status = DeviceIoControl( tcpFile,
                              IOCTL_TCP_QUERY_INFORMATION_EX,
                              &req,
                              sizeof(req),
                              entry,
                              sizeof(*entry),
                              &returnSize,
                              NULL );

    if(!status)
    {
            WARN("IOCTL Failed\n");
            return STATUS_UNSUCCESSFUL;
    }

    TRACE("TdiGetMibForIfEntity() => {\n"
           "  if_index ....................... %x\n"
           "  if_type ........................ %x\n"
           "  if_mtu ......................... %d\n"
           "  if_speed ....................... %x\n"
           "  if_physaddrlen ................. %d\n",
           entry->ent.if_index,
           entry->ent.if_type,
           entry->ent.if_mtu,
           entry->ent.if_speed,
           entry->ent.if_physaddrlen);
    TRACE("  if_physaddr .................... %02x:%02x:%02x:%02x:%02x:%02x\n"
           "  if_descr ....................... %s\n",
           entry->ent.if_physaddr[0] & 0xff,
           entry->ent.if_physaddr[1] & 0xff,
           entry->ent.if_physaddr[2] & 0xff,
           entry->ent.if_physaddr[3] & 0xff,
           entry->ent.if_physaddr[4] & 0xff,
           entry->ent.if_physaddr[5] & 0xff,
           entry->ent.if_descr);
    TRACE("} status %08x\n",status);

    return STATUS_SUCCESS;
}

NTSTATUS getNthIpEntity( HANDLE tcpFile, DWORD index, TDIEntityID *ent ) {
    DWORD numEntities = 0;
    DWORD numRoutes = 0;
    TDIEntityID *entitySet = 0;
    NTSTATUS status = tdiGetEntityIDSet( tcpFile, &entitySet, &numEntities );
    int i;

    if( !NT_SUCCESS(status) )
        return status;

    for( i = 0; i < numEntities; i++ ) {
        if( isIpEntity( tcpFile, &entitySet[i] ) ) {
            TRACE("Entity %d is an IP Entity\n", i);
            if( numRoutes == index ) break;
            else numRoutes++;
        }
    }

    if( numRoutes == index && i < numEntities ) {
        TRACE("Index %d is entity #%d - %04x:%08x\n", index, i,
               entitySet[i].tei_entity, entitySet[i].tei_instance );
        memcpy( ent, &entitySet[i], sizeof(*ent) );
        tdiFreeThingSet( entitySet );
        return STATUS_SUCCESS;
    } else {
        tdiFreeThingSet( entitySet );
        return STATUS_UNSUCCESSFUL;
    }
}

NTSTATUS tdiGetIpAddrsForIpEntity
( HANDLE tcpFile, TDIEntityID *ent, IPAddrEntry **addrs, PDWORD numAddrs ) {
    NTSTATUS status;

    TRACE("TdiGetIpAddrsForIpEntity(tcpFile %x,entityId %x)\n",
           (DWORD)tcpFile, ent->tei_instance);

    status = tdiGetSetOfThings( tcpFile,
                                INFO_CLASS_PROTOCOL,
                                INFO_TYPE_PROVIDER,
                                IP_MIB_ADDRTABLE_ENTRY_ID,
                                CL_NL_ENTITY,
				ent->tei_instance,
                                0,
                                sizeof(IPAddrEntry),
                                (PVOID *)addrs,
                                numAddrs );

    return status;
}

static NTSTATUS getInterfaceInfoSet( HANDLE tcpFile,
                                     IFInfo **infoSet,
                                     PDWORD numInterfaces ) {
    DWORD numEntities;
    TDIEntityID *entIDSet = NULL;
    NTSTATUS status = tdiGetEntityIDSet( tcpFile, &entIDSet, &numEntities );
    IFInfo *infoSetInt = 0;
    int curInterf = 0, i;

    if (!NT_SUCCESS(status)) {
        ERR("getInterfaceInfoSet: tdiGetEntityIDSet() failed: 0x%lx\n", status);
        return status;
    }

    infoSetInt = HeapAlloc( GetProcessHeap(), 0,
                            sizeof(IFInfo) * numEntities );

    if( infoSetInt ) {
        for( i = 0; i < numEntities; i++ ) {
            if( isInterface( &entIDSet[i] ) ) {
                infoSetInt[curInterf].entity_id = entIDSet[i];
                status = tdiGetMibForIfEntity
                    ( tcpFile,
                      &entIDSet[i],
                      &infoSetInt[curInterf].if_info );
                TRACE("tdiGetMibForIfEntity: %08x\n", status);
                if( NT_SUCCESS(status) ) {
                    DWORD numAddrs;
                    IPAddrEntry *addrs;
                    TDIEntityID ip_ent;
                    int j;

                    status = getNthIpEntity( tcpFile, curInterf, &ip_ent );
                    if( NT_SUCCESS(status) )
                        status = tdiGetIpAddrsForIpEntity
                            ( tcpFile, &ip_ent, &addrs, &numAddrs );
                    for( j = 0; NT_SUCCESS(status) && j < numAddrs; j++ ) {
                        TRACE("ADDR %d: index %d (target %d)\n", j, addrs[j].iae_index, infoSetInt[curInterf].if_info.ent.if_index);
                        if( addrs[j].iae_index ==
                            infoSetInt[curInterf].if_info.ent.if_index ) {
                            memcpy( &infoSetInt[curInterf].ip_addr,
                                    &addrs[j],
                                    sizeof( addrs[j] ) );
                            curInterf++;
                            break;
                        }
                    }
                    if ( NT_SUCCESS(status) )
                        tdiFreeThingSet(addrs);
                }
            }
        }

        tdiFreeThingSet(entIDSet);

        if (NT_SUCCESS(status)) {
            *infoSet = infoSetInt;
            *numInterfaces = curInterf;
        } else {
            HeapFree(GetProcessHeap(), 0, infoSetInt);
        }

        return status;
    } else {
        tdiFreeThingSet(entIDSet);
        return STATUS_INSUFFICIENT_RESOURCES;
    }
}

NTSTATUS getInterfaceInfoByName( HANDLE tcpFile, char *name, IFInfo *info ) {
    IFInfo *ifInfo;
    DWORD numInterfaces;
    int i;
    NTSTATUS status = getInterfaceInfoSet( tcpFile, &ifInfo, &numInterfaces );

    if( NT_SUCCESS(status) )
    {
        for( i = 0; i < numInterfaces; i++ ) {
            if( !strcmp((PCHAR)ifInfo[i].if_info.ent.if_descr, name) ) {
                memcpy( info, &ifInfo[i], sizeof(*info) );
                break;
            }
        }

        HeapFree(GetProcessHeap(), 0,ifInfo);

        return i < numInterfaces ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
    }

    return status;
}

DWORD getInterfaceIndexByName(const char *name, PDWORD index)
{
    IFInfo ifInfo;
    HANDLE tcpFile;
    NTSTATUS status = openTcpFile( &tcpFile, FILE_READ_DATA );

    if( NT_SUCCESS(status) ) {
        status = getInterfaceInfoByName( tcpFile, (char *)name, &ifInfo );

        if( NT_SUCCESS(status) ) {
            *index = ifInfo.if_info.ent.if_index;
        }

        closeTcpFile( tcpFile );
    }

    return status;
}

NTSTATUS getInterfaceInfoByIndex( HANDLE tcpFile, DWORD index, IFInfo *info ) {
    IFInfo *ifInfo;
    DWORD numInterfaces;
    NTSTATUS status = getInterfaceInfoSet( tcpFile, &ifInfo, &numInterfaces );
    int i;

    if( NT_SUCCESS(status) )
    {
        for( i = 0; i < numInterfaces; i++ ) {
            if( ifInfo[i].if_info.ent.if_index == index ) {
                memcpy( info, &ifInfo[i], sizeof(*info) );
                break;
            }
        }

        HeapFree(GetProcessHeap(), 0, ifInfo);

        return i < numInterfaces ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
    }

    return status;
}

NETIO_STATUS 
WINAPI 
ConvertInterfaceIndexToLuid(
  _In_  NET_IFINDEX InterfaceIndex,
  _Out_ PNET_LUID   InterfaceLuid
)
{
	MIB_IFROW row;
	if(InterfaceLuid==0)
      return ERROR_INVALID_PARAMETER;
  
	memset(InterfaceLuid, 0, sizeof(*InterfaceLuid));
	
	row.dwIndex = InterfaceIndex;
    if (GetIfEntry(&row)==0) 
		return ERROR_FILE_NOT_FOUND;
	
	InterfaceLuid->Info.Reserved     = 0;
    InterfaceLuid->Info.NetLuidIndex = InterfaceIndex;
    InterfaceLuid->Info.IfType       = row.dwType;
	return NO_ERROR;
}

/******************************************************************
 *    ConvertInterfaceLuidToNameA (IPHLPAPI.@)
 */
DWORD WINAPI ConvertInterfaceLuidToNameA(const NET_LUID *luid, char *name, SIZE_T len)
{
    DWORD err;
    WCHAR nameW[IF_MAX_STRING_SIZE + 1];

    TRACE( "(%p %p %u)\n", luid, name, (DWORD)len );

    if (!luid) return ERROR_INVALID_PARAMETER;
    if (!name || !len) return ERROR_NOT_ENOUGH_MEMORY;

    err = ConvertInterfaceLuidToNameW( luid, nameW, ARRAY_SIZE(nameW) );
    if (err) return err;

    if (!WideCharToMultiByte( CP_UNIXCP, 0, nameW, -1, name, len, NULL, NULL ))
        err = GetLastError();
    return err;
}

/******************************************************************
 *    ConvertInterfaceLuidToNameW (IPHLPAPI.@)
 */
DWORD WINAPI ConvertInterfaceLuidToNameW(const NET_LUID *luid, WCHAR *name, SIZE_T len)
{
    DWORD ret;
    MIB_IFROW row;

    TRACE("(%p %p %u)\n", luid, name, (DWORD)len);

    if (!luid || !name) return ERROR_INVALID_PARAMETER;

    row.dwIndex = luid->Info.NetLuidIndex;
    if ((ret = GetIfEntry( &row ))) return ret;

    if (len < strlenW( row.wszName ) + 1) return ERROR_NOT_ENOUGH_MEMORY;
    strcpyW( name, row.wszName );
    return NO_ERROR;
}

/******************************************************************
 *    ConvertInterfaceNameToLuidA (IPHLPAPI.@)
 */
DWORD WINAPI ConvertInterfaceNameToLuidA(const char *name, NET_LUID *luid)
{
    WCHAR nameW[IF_MAX_STRING_SIZE];

    TRACE( "(%s %p)\n", debugstr_a(name), luid );

    if (!name) return ERROR_INVALID_NAME;
    if (!MultiByteToWideChar( CP_ACP, 0, name, -1, nameW, ARRAY_SIZE(nameW) ))
        return GetLastError();

    return ConvertInterfaceNameToLuidW( nameW, luid );
}

/******************************************************************
 *    ConvertInterfaceNameToLuidW (IPHLPAPI.@)
 */
DWORD WINAPI ConvertInterfaceNameToLuidW(const WCHAR *name, NET_LUID *luid)
{
    DWORD ret;
    IF_INDEX index;
    MIB_IFROW row;
    char nameA[IF_MAX_STRING_SIZE + 1];

    TRACE("(%s %p)\n", debugstr_w(name), luid);

    if (!luid) return ERROR_INVALID_PARAMETER;
    memset( luid, 0, sizeof(*luid) );

    if (!WideCharToMultiByte( CP_ACP, 0, name, -1, nameA, sizeof(nameA), NULL, NULL ))
        return ERROR_INVALID_NAME;

    if ((ret = getInterfaceIndexByName( nameA, &index ))) return ret;

    row.dwIndex = index;
    if ((ret = GetIfEntry( &row ))) return ret;

    luid->Info.Reserved     = 0;
    luid->Info.NetLuidIndex = index;
    luid->Info.IfType       = row.dwType;
    return NO_ERROR;
}

/******************************************************************
 *    if_indextoname (IPHLPAPI.@)
 */
PCHAR WINAPI IPHLP_if_indextoname(NET_IFINDEX index, PCHAR name)
{
    TRACE("(%u, %p)\n", index, name);

    return getInterfaceNameByIndex(index, name);
}

/* The comments say MAX_ADAPTER_NAME is required, but really only IF_NAMESIZE
 * bytes are necessary.
 */
char *getInterfaceNameByIndex(IF_INDEX index, char *name)
{
  return IPHLP_if_indextoname(index, name);
}

/******************************************************************
 *    if_nametoindex (IPHLPAPI.@)
 */
IF_INDEX WINAPI IPHLP_if_nametoindex(const char *name)
{
    IF_INDEX idx;

    TRACE("(%s)\n", name);
    if (getInterfaceIndexByName(name, &idx) == NO_ERROR)
        return idx;

    return 0;
}

/******************************************************************
 *    GetIpForwardTable2 (IPHLPAPI.@)
 */
DWORD WINAPI GetIpForwardTable2(ADDRESS_FAMILY family, PMIB_IPFORWARD_TABLE2 *table)
{
    static int once;

    if (!once++) FIXME("(%u %p): stub\n", family, table);
    return ERROR_NOT_SUPPORTED;
}

void getInterfacePhysicalFromInfo( IFInfo *info,
                                   PDWORD len, PBYTE addr, PDWORD type ) {
    *len = info->if_info.ent.if_physaddrlen;
    memcpy( addr, info->if_info.ent.if_physaddr, *len );
    *type = info->if_info.ent.if_type;
}

DWORD getInterfacePhysicalByName(const char *name, PDWORD len, PBYTE addr,
                                 PDWORD type)
{
    HANDLE tcpFile;
    IFInfo info;
    NTSTATUS status = openTcpFile( &tcpFile, FILE_READ_DATA );

    if( NT_SUCCESS(status) ) {
        status = getInterfaceInfoByName( tcpFile, (char *)name, &info );
        if( NT_SUCCESS(status) )
            getInterfacePhysicalFromInfo( &info, len, addr, type );
        closeTcpFile( tcpFile );
    }

    return status;
}

DWORD getAddrByIndexOrName( char *name, DWORD index, IPHLPAddrType addrType ) {
    IFInfo ifInfo;
    HANDLE tcpFile;
    NTSTATUS status = STATUS_SUCCESS;
    DWORD addrOut = INADDR_ANY;

    status = openTcpFile( &tcpFile, FILE_READ_DATA );

    if( NT_SUCCESS(status) ) {
        status = getIPAddrEntryForIf( tcpFile, name, index, &ifInfo );
        if( NT_SUCCESS(status) ) {
            switch( addrType ) {
            case IPAAddr:  addrOut = ifInfo.ip_addr.iae_addr; break;
            case IPABcast: addrOut = ifInfo.ip_addr.iae_bcastaddr; break;
            case IPAMask:  addrOut = ifInfo.ip_addr.iae_mask; break;
            case IFMtu:    addrOut = ifInfo.if_info.ent.if_mtu; break;
            case IFStatus: addrOut = ifInfo.if_info.ent.if_operstatus; break;
            }
        }
        closeTcpFile( tcpFile );
    }

    return addrOut;
}

DWORD getInterfaceMtuByName(const char *name, PDWORD mtu) {
    *mtu = getAddrByIndexOrName( (char *)name, 0, IFMtu );
    return STATUS_SUCCESS;
}

DWORD getInterfaceEntryByName(const char *name, PMIB_IFROW entry)
{
  BYTE addr[MAX_INTERFACE_PHYSADDR];
  DWORD ret, len = sizeof(addr), type;

  if (!name)
    return ERROR_INVALID_PARAMETER;
  if (!entry)
    return ERROR_INVALID_PARAMETER;

  if (getInterfacePhysicalByName(name, &len, addr, &type) == NO_ERROR) {
    WCHAR *assigner;
    const char *walker;

    memset(entry, 0, sizeof(MIB_IFROW));
    for (assigner = entry->wszName, walker = name; *walker; 
     walker++, assigner++)
      *assigner = *walker;
    *assigner = 0;
    getInterfaceIndexByName(name, &entry->dwIndex);
    entry->dwPhysAddrLen = len;
    memcpy(entry->bPhysAddr, addr, len);
    memset(entry->bPhysAddr + len, 0, sizeof(entry->bPhysAddr) - len);
    entry->dwType = type;
    /* FIXME: how to calculate real speed? */
    getInterfaceMtuByName(name, &entry->dwMtu);
    /* lie, there's no "administratively down" here */
    entry->dwAdminStatus = MIB_IF_ADMIN_STATUS_UP;
    getInterfaceStatusByName(name, (PDWORD)&entry->dwOperStatus);
    /* punt on dwLastChange? */
    entry->dwDescrLen = min(strlen(name), MAX_INTERFACE_DESCRIPTION - 1);
    memcpy(entry->bDescr, name, entry->dwDescrLen);
    entry->bDescr[entry->dwDescrLen] = '\0';
    entry->dwDescrLen++;
    ret = NO_ERROR;
  }
  else
    ret = ERROR_INVALID_DATA;
  return ret;
}

DWORD getInterfaceStatsByName(const char *name, PMIB_IFROW entry)
{
  if (!name)
    return ERROR_INVALID_PARAMETER;
  if (!entry)
    return ERROR_INVALID_PARAMETER;

  return NO_ERROR;
}

DWORD getInterfaceStatsByIndex(DWORD index, PMIB_IFROW entry)
{
    return ERROR_INVALID_PARAMETER;
}

NTSTATUS getIPAddrEntryForIf(HANDLE tcpFile,
                             char *name,
                             DWORD index,
                             IFInfo *ifInfo) {
    NTSTATUS status =
        name ?
        getInterfaceInfoByName( tcpFile, name, ifInfo ) :
        getInterfaceInfoByIndex( tcpFile, index, ifInfo );

    if (!NT_SUCCESS(status)) {
        ERR("getIPAddrEntryForIf returning %lx\n", status);
    }

    return status;
}

DWORD getInterfaceStatusByName(const char *name, PDWORD status) {
    *status = getAddrByIndexOrName( (char *)name, 0, IFStatus );
    return STATUS_SUCCESS;
}

/******************************************************************
 *    GetIpInterfaceTable (IPHLPAPI.@)
 */
DWORD WINAPI GetIpInterfaceTable(ADDRESS_FAMILY family, PMIB_IPINTERFACE_TABLE *table)
{
    FIXME("(%u %p): stub\n", family, table);
    return ERROR_NOT_SUPPORTED;
}

/******************************************************************
 *    GetIpNetTable2 (IPHLPAPI.@)
 */
DWORD WINAPI GetIpNetTable2(ADDRESS_FAMILY family, PMIB_IPNET_TABLE2 *table)
{
    static int once;

    if (!once++) FIXME("(%u %p): stub\n", family, table);
    return ERROR_NOT_SUPPORTED;
}

/******************************************************************
 *    GetTcp6Table (IPHLPAPI.@)
 */
ULONG WINAPI GetTcp6Table(PMIB_TCP6TABLE table, PULONG size, BOOL order)
{
    TRACE("(table %p, size %p, order %d)\n", table, size, order);
    return GetExtendedTcpTable(table, size, order, WS_AF_INET6, TCP_TABLE_BASIC_ALL, 0);
}

/******************************************************************
 *    GetTcp6Table2 (IPHLPAPI.@)
 */
ULONG WINAPI GetTcp6Table2(PMIB_TCP6TABLE2 table, PULONG size, BOOL order)
{
    FIXME("pTcp6Table2 %p, size %p, order %d: stub\n", table, size, order);
    return ERROR_NOT_SUPPORTED;
}

/******************************************************************
 *    GetTcpTable2 (IPHLPAPI.@)
 */
ULONG WINAPI GetTcpTable2(PMIB_TCPTABLE2 table, PULONG size, BOOL order)
{
    FIXME("pTcpTable2 %p, pdwSize %p, bOrder %d: stub\n", table, size, order);
    return ERROR_NOT_SUPPORTED;
}

/******************************************************************
 *    GetUdp6Table (IPHLPAPI.@)
 */
DWORD WINAPI GetUdp6Table(PMIB_UDP6TABLE pUdpTable, PDWORD pdwSize, BOOL bOrder)
{
    return GetExtendedUdpTable(pUdpTable, pdwSize, bOrder, WS_AF_INET6, UDP_TABLE_BASIC, 0);
}

DWORD WINAPI GetUnicastIpAddressEntry(MIB_UNICASTIPADDRESS_ROW *row)
{
    IP_ADAPTER_ADDRESSES_LH *aa, *ptr;
    ULONG size = 0;
    DWORD ret;

    TRACE("%p\n", row);

    if (!row)
        return ERROR_INVALID_PARAMETER;

    ret = GetAdaptersAddresses(row->Address.si_family, 0, NULL, NULL, &size);
    if (ret != ERROR_BUFFER_OVERFLOW)
        return ret;
    if (!(ptr = HeapAlloc(GetProcessHeap(), 0, size)))
        return ERROR_OUTOFMEMORY;
    if ((ret = GetAdaptersAddresses(row->Address.si_family, 0, NULL, (PIP_ADAPTER_ADDRESSES)ptr, &size)))
    {
        HeapFree(GetProcessHeap(), 0, ptr);
        return ret;
    }

    ret = ERROR_FILE_NOT_FOUND;
    for (aa = ptr; aa; aa = aa->Next)
    {
        IP_ADAPTER_UNICAST_ADDRESS *ua;

        if (aa->IfIndex != row->InterfaceIndex &&
            memcmp(&aa->Luid, &row->InterfaceLuid, sizeof(row->InterfaceLuid)))
            continue;
        ret = ERROR_NOT_FOUND;

        ua = aa->FirstUnicastAddress;
        while (ua)
        {
            SOCKADDR_INET *uaaddr = (SOCKADDR_INET *)ua->Address.lpSockaddr;

            if ((row->Address.si_family == WS_AF_INET6 &&
                 !memcmp(&row->Address.Ipv6.sin6_addr, &uaaddr->Ipv6.sin6_addr, sizeof(uaaddr->Ipv6.sin6_addr))) ||
                (row->Address.si_family == WS_AF_INET &&
                 row->Address.Ipv4.sin_addr.S_un.S_addr == uaaddr->Ipv4.sin_addr.S_un.S_addr))
            {
                memcpy(&row->InterfaceLuid, &aa->Luid, sizeof(aa->Luid));
                row->InterfaceIndex     = aa->IfIndex;
                row->PrefixOrigin       = ua->PrefixOrigin;
                row->SuffixOrigin       = ua->SuffixOrigin;
                row->ValidLifetime      = ua->ValidLifetime;
                row->PreferredLifetime  = ua->PreferredLifetime;
                row->OnLinkPrefixLength = ua->OnLinkPrefixLength;
                row->SkipAsSource       = 0;
                row->DadState           = ua->DadState;
                if (row->Address.si_family == WS_AF_INET6)
                    row->ScopeId.Value  = row->Address.Ipv6.sin6_scope_id;
                else
                    row->ScopeId.Value  = 0;
                NtQuerySystemTime(&row->CreationTimeStamp);
                HeapFree(GetProcessHeap(), 0, ptr);
                return NO_ERROR;
            }
            ua = ua->Next;
        }
    }
    HeapFree(GetProcessHeap(), 0, ptr);

    return ret;
}

DWORD WINAPI GetUnicastIpAddressTable(ADDRESS_FAMILY family, MIB_UNICASTIPADDRESS_TABLE **table)
{
    IP_ADAPTER_ADDRESSES_LH *aa, *ptr;
    MIB_UNICASTIPADDRESS_TABLE *data;
    DWORD ret, count = 0;
    ULONG size, flags;

    TRACE("%u, %p\n", family, table);

    if (!table || (family != WS_AF_INET && family != WS_AF_INET6 && family != WS_AF_UNSPEC))
        return ERROR_INVALID_PARAMETER;

    flags = GAA_FLAG_SKIP_ANYCAST |
            GAA_FLAG_SKIP_MULTICAST |
            GAA_FLAG_SKIP_DNS_SERVER |
            GAA_FLAG_SKIP_FRIENDLY_NAME;

    ret = GetAdaptersAddresses(family, flags, NULL, NULL, &size);
    if (ret != ERROR_BUFFER_OVERFLOW)
        return ret;
    if (!(ptr = HeapAlloc(GetProcessHeap(), 0, size)))
        return ERROR_OUTOFMEMORY;
    if ((ret = GetAdaptersAddresses(family, flags, NULL, (IP_ADAPTER_ADDRESSES*)ptr, &size)))
    {
        HeapFree(GetProcessHeap(), 0, ptr);
        return ret;
    }

    for (aa = ptr; aa; aa = aa->Next)
    {
        IP_ADAPTER_UNICAST_ADDRESS *ua = aa->FirstUnicastAddress;
        while (ua)
        {
            count++;
            ua = ua->Next;
        }
    }

    if (!(data = HeapAlloc(GetProcessHeap(), 0, sizeof(*data) + (count - 1) * sizeof(data->Table[0]))))
    {
        HeapFree(GetProcessHeap(), 0, ptr);
        return ERROR_OUTOFMEMORY;
    }

    data->NumEntries = 0;
    for (aa = ptr; aa; aa = aa->Next)
    {
        IP_ADAPTER_UNICAST_ADDRESS *ua = aa->FirstUnicastAddress;
        while (ua)
        {
            MIB_UNICASTIPADDRESS_ROW *row = &data->Table[data->NumEntries];
            memcpy(&row->Address, ua->Address.lpSockaddr, ua->Address.iSockaddrLength);
            memcpy(&row->InterfaceLuid, &aa->Luid, sizeof(aa->Luid));
            row->InterfaceIndex     = aa->IfIndex;
            row->PrefixOrigin       = ua->PrefixOrigin;
            row->SuffixOrigin       = ua->SuffixOrigin;
            row->ValidLifetime      = ua->ValidLifetime;
            row->PreferredLifetime  = ua->PreferredLifetime;
            row->OnLinkPrefixLength = ua->OnLinkPrefixLength;
            row->SkipAsSource       = 0;
            row->DadState           = ua->DadState;
            if (row->Address.si_family == WS_AF_INET6)
                row->ScopeId.Value  = row->Address.Ipv6.sin6_scope_id;
            else
                row->ScopeId.Value  = 0;
            NtQuerySystemTime(&row->CreationTimeStamp);

            data->NumEntries++;
            ua = ua->Next;
        }
    }

    HeapFree(GetProcessHeap(), 0, ptr);

    *table = data;
    return ret;
}

/******************************************************************
 *    SetPerTcpConnectionEStats (IPHLPAPI.@)
 */
DWORD WINAPI SetPerTcpConnectionEStats(PMIB_TCPROW row, TCP_ESTATS_TYPE state, PBYTE rw,
                                       ULONG version, ULONG size, ULONG offset)
{
  FIXME("(row %p, state %d, rw %p, version %u, size %u, offset %u): stub\n",
        row, state, rw, version, size, offset);
  return ERROR_NOT_SUPPORTED;
}