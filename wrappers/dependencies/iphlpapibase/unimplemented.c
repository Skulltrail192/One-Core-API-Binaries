/*
 * iphlpapi dll implementation -- physical media stubs
 *
 * Copyright (C) 2004 Art Yerkes
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "iphlpapi_private.h"
#include "fltdefs.h"

WINE_DEFAULT_DEBUG_CHANNEL(iphlpapi);

typedef DWORD (WINAPI *PtFuncCompare)(const void *, const void *);

// typedef enum  { 
  // UDP_TABLE_BASIC,
  // UDP_TABLE_OWNER_PID,
  // UDP_TABLE_OWNER_MODULE
// } UDP_TABLE_CLASS, *PUDP_TABLE_CLASS;

DWORD WINAPI AllocateAndGetTcpExTableFromStack(
  _Out_  PVOID *ppTcpTable,
  _In_   BOOL bOrder,
  _In_   HANDLE hHeap,
  _In_   DWORD dwFlags,
  _In_   DWORD dwFamily
)
{
	return 0;
}

BOOL WINAPI CancelSecurityHealthChangeNotify(
  LPOVERLAPPED notifyOverlapped
)
{
	return TRUE;
}

DWORD WINAPI GetExtendedUdpTable(
  _Out_    PVOID pUdpTable,
  _Inout_  PDWORD pdwSize,
  _In_     BOOL bOrder,
  _In_     ULONG ulAf,
  _In_     UDP_TABLE_CLASS TableClass,
  _In_     ULONG Reserved
)
{
	return 0;
}

DWORD WINAPI GetIcmpStatsFromStack(PVOID OutputBuffer)
{
	return 0;
}

DWORD WINAPI GetIcmpStatsFromStackEx(PVOID OutputBuffer, HANDLE handle)
{
	return 0;
}

DWORD WINAPI GetIgmpList(char InputBuffer, PVOID OutputBuffer, UINT_PTR *lp)
{
	return 0;
}

HANDLE WINAPI register_icmp()
{
	HANDLE handle = NULL;
	return handle;
}

DWORD WINAPI _PfUnBindInterface(INTERFACE_HANDLE pInterface)
{
	return 0;
}

DWORD WINAPI _PfTestPacket(INTERFACE_HANDLE pInInterface, HANDLE pOutInterface, DWORD cBytes, PBYTE pbPacket, PPFFORWARD_ACTION ppAction)
{
	return 0;
}

DWORD WINAPI _PfSetLogBuffer(PBYTE pbBuffer, 
							DWORD dwSize, 
							DWORD dwThreshold,
							DWORD dwEntries, 
							PDWORD pdwLoggedEntries, 
							PDWORD pdwLostEntries, 
							PDWORD pdwSizeUsed)
{
	return 0;
}

DWORD WINAPI _PfRemoveGlobalFilterFromInterface(INTERFACE_HANDLE pInterface, GLOBAL_FILTER gfFilter)
{
	return 0;
}

DWORD WINAPI _PfRemoveFiltersFromInterface(INTERFACE_HANDLE ih, 
										   DWORD cInFilters, 
										   PPF_FILTER_DESCRIPTOR pfiltIn, 
										   DWORD cOutFilters, 
										   PPF_FILTER_DESCRIPTOR pfiltOut)
{
	return 0;
}

DWORD WINAPI _PfCreateInterface(DWORD dwName,
								PFFORWARD_ACTION inAction,
								PFFORWARD_ACTION outAction,
								BOOL bUseLog,
								BOOL bMustBeUnique,
								INTERFACE_HANDLE *ppInterface)
{
	return 0;
}

DWORD WINAPI _PfDeleteInterface(INTERFACE_HANDLE pInterface)
{
	return 0;
}

DWORD WINAPI _PfAddFiltersToInterface(INTERFACE_HANDLE ih,
									 DWORD cInFilters,
									 PPF_FILTER_DESCRIPTOR pfiltIn,
									 DWORD cOutFilters,
									 PPF_FILTER_DESCRIPTOR pfiltOut,
									 PFILTER_HANDLE pfHandle)
{
	return 0;
}
										  
DWORD WINAPI _PfRemoveFilterHandles(INTERFACE_HANDLE pInterface,DWORD cFilters,PFILTER_HANDLE pvHandles)
{
	return 0;
}

DWORD WINAPI _PfBindInterfaceToIndex(INTERFACE_HANDLE pInterface,DWORD dwIndex,PFADDRESSTYPE pfatLinkType,PBYTE LinkIPAddress)
{
	return 0;
}

DWORD WINAPI _PfBindInterfaceToIPAddress(INTERFACE_HANDLE pInterface,PFADDRESSTYPE pfatType,PBYTE IPAddress)
{
	return 0;
}

DWORD WINAPI _PfRebindFilters(INTERFACE_HANDLE pInterface,PPF_LATEBIND_INFO pLateBindInfo)
{
	return 0;
}

DWORD WINAPI _PfAddGlobalFilterToInterface(INTERFACE_HANDLE pInterface,GLOBAL_FILTER gfFilter)
{
	return 0;
}

DWORD WINAPI _PfDeleteLog(VOID)
{
	return 0;
}

DWORD WINAPI _PfGetInterfaceStatistics(INTERFACE_HANDLE pInterface,PPF_INTERFACE_STATS ppfStats,PDWORD pdwBufferSize,BOOL fResetCounters)
{
	return 0;
}

DWORD WINAPI _PfMakeLog(HANDLE hEvent)
{
	return 0;
}

DWORD WINAPI SetTcpEntryToStack(HANDLE handle)
{
	return 0;
}

DWORD WINAPI SetIpNetEntryToStack(HANDLE handle, BOOL reason)
{
	return 0;
}

DWORD WINAPI SetIpStatsToStack(HANDLE handle)
{
	return 0;
}

DWORD WINAPI InternalCreateIpForwardEntry(LPVOID lpEntry)
{
	return 0;
}

DWORD WINAPI InternalCreateIpNetEntry(LPVOID lpEntry)
{
	return 0;
}

DWORD WINAPI InternalDeleteIpForwardEntry(LPVOID lpEntry)
{
	return 0;
}

DWORD WINAPI InternalGetIfTable(LPVOID pBuffer, HANDLE hHeap, DWORD dwFlags)
{
	return 0;
}

DWORD WINAPI InternalGetIpAddrTable(LPVOID pBuffer, HANDLE hHeap, DWORD dwFlags)
{
	return 0;
}

DWORD WINAPI InternalGetIpForwardTable(LPVOID pBuffer, HANDLE hHeap, DWORD dwFlags)
{
	return 0;
}

DWORD WINAPI InternalGetIpNetTable(LPVOID pBuffer, HANDLE hHeap, DWORD dwFlags)
{
	return 0;
}

DWORD WINAPI InternalGetTcpTable(LPVOID pBuffer, HANDLE hHeap, DWORD dwFlags)
{
	return 0;
}

DWORD WINAPI InternalGetUdpTable(LPVOID pBuffer, HANDLE hHeap, DWORD dwFlags)
{
	return 0;
}


DWORD WINAPI InternalDeleteIpNetEntry(LPVOID lpEntry)
{
	return 0;
}

DWORD WINAPI InternalSetIfEntry(LPVOID lpEntry)
{
	return 0;
}

DWORD WINAPI InternalSetIpForwardEntry(LPVOID lpEntry)
{
	return 0;
}

DWORD WINAPI InternalSetIpNetEntry(LPVOID lpEntry)
{
	return 0;
}

DWORD WINAPI InternalSetIpStats(LPVOID lpEntry)
{
	return 0;
}

DWORD WINAPI InternalSetTcpEntry(LPVOID lpEntry)
{
	return 0;
}

DWORD WINAPI Icmp6ParseReplies(
  _In_  LPVOID ReplyBuffer,
  _In_  DWORD ReplySize
)
{
	return 0;
}

DWORD WINAPI GetIpNetTableFromStack(PVOID OutputBuffer, int a2, int a3, unsigned int a4)
{
	return 0;
}

DWORD WINAPI GetTcpStatsFromStack(PVOID OutputBuffer)
{
	return 0;
}

DWORD WINAPI GetTcpStatsFromStackEx(PVOID OutputBuffer, HANDLE Handle)
{
	return 0;
}

DWORD WINAPI NTPTimeToNTFileTime(u_long Time, LPFILETIME lpFileTime, signed int a3)
{
	return 0;
}

DWORD WINAPI NotifyRouteChangeEx(LPVOID lp, LPVOID a2, DWORD BytesReturned)
{
	return 0;
}

DWORD WINAPI SetProxyArpEntryToStack(int a1, int a2, int a3, int a4, int a5)
{
	return 0;
}

DWORD WINAPI SetRouteWithRef(LPVOID pointer)
{
	return 0;
}

DWORD WINAPI SetIpRouteEntryToStack(LPVOID pointer)
{
	return 0;
}

DWORD WINAPI SetIpMultihopRouteEntryToStack(LPVOID pointer)
{
	return 0;
}

DWORD WINAPI SetBlockRoutes(PVOID InputBuffer, const void *lp, PVOID OutputBuffer)
{
	return 0;
}

DWORD WINAPI SetAdapterIpAddress(char *a1, int a2, struct in_addr in, struct in_addr a4, struct in_addr a5)
{
	return 0;
}

DWORD WINAPI NotifySecurityHealthChange(
  PHANDLE pHandle,
  LPOVERLAPPED pOverLapped,
  PULONG SecurityHealthFlags
)
{
	return 0;
}

DWORD WINAPI NhpGetInterfaceIndexFromStack(PWSTR Str, LPVOID a2)
{
	return 0;
}

DWORD WINAPI GetUdpTableFromStack(int a1, unsigned int a2, int a3)
{
	return 0;
}

DWORD WINAPI GetUdpStatsFromStack(PVOID OutputBuffer)
{
	return 0;
}

DWORD WINAPI GetUdpStatsFromStackEx(PVOID OutputBuffer, HANDLE Handle)
{
	return 0;
}

DWORD WINAPI GetOwnerModuleFromUdpEntry(
  _In_     PMIB_UDPROW_OWNER_MODULE pUdpEntry,
  _In_     TCPIP_OWNER_MODULE_INFO_CLASS Class,
  _Out_    PVOID Buffer,
  _Inout_  PDWORD pdwSize
)
{
	return 0;
}

DWORD WINAPI GetOwnerModuleFromUdp6Entry(
  _In_     PMIB_UDP6ROW_OWNER_MODULE pUdpEntry,
  _In_     TCPIP_OWNER_MODULE_INFO_CLASS Class,
  _Out_    PVOID Buffer,
  _Inout_  PDWORD pdwSize
)
{
	return 0;
}

DWORD WINAPI GetOwnerModuleFromTcp6Entry(
  _In_     PMIB_TCP6ROW_OWNER_MODULE pTcpEntry,
  _In_     TCPIP_OWNER_MODULE_INFO_CLASS Class,
  _Out_    PVOID Buffer,
  _Inout_  PDWORD pdwSize
)
{
	return 0;
}

DWORD WINAPI GetIpStatsFromStack(PVOID OutputBuffer)
{
	return 0;
}

DWORD WINAPI GetIpStatsFromStackEx(PVOID OutputBuffer, HANDLE Handle)
{
	return 0;
}

DWORD WINAPI GetIpForwardTableFromStack(int a1, unsigned int a2, int a3)
{
	return 0;
}

DWORD WINAPI GetIpAddrTableFromStack(int a1, unsigned int a2, int a3)
{
	return 0;
}

DWORD WINAPI GetIfEntryFromStack(int a1, unsigned int a2, int a3)
{
	return 0;
}

DWORD WINAPI GetBestRouteFromStack(int a1, unsigned int a2, int a3)
{
	return 0;
}

DWORD WINAPI AllocateAndGetUdpExTableFromStack(
  _Out_  PVOID *ppUDPTable,
  _In_   BOOL bOrder,
  _In_   HANDLE hHeap,
  _In_   DWORD dwFlags,
  _In_   DWORD dwFamily
)
{
	return 0;
}

DWORD WINAPI AllocateAndGetTcpExTable2FromStack(int a1, int a2, HANDLE hHeap, DWORD dwFlags, HANDLE Handle, PtFuncCompare compare)
{
	return 0;
}

DWORD WINAPI AllocateAndGetArpEntTableFromStack(LPVOID *a1, int a2, HANDLE hHeap, DWORD dwFlags, DWORD a5)
{
	return 0;
}

DWORD WINAPI AllocateAndGetUdpExTable2FromStack(int a1, int a2, HANDLE hHeap, DWORD dwFlags, HANDLE Handle, PtFuncCompare compare)
{
	return 0;
}

DWORD WINAPI FlushIpNetTableFromStack(char InputBuffer)
{
	return 0;
}

DWORD WINAPI GetBestInterfaceFromStack(PVOID InputBuffer, PVOID OutputBuffer)
{
	return 0;
}

DWORD WINAPI GetIfTableFromStack(int a1, int a2, int a3, int a4)
{
	return 0;
}

DWORD WINAPI GetTcpExTable2FromStack(int a1, int a2, int a3, HANDLE Handle, PtFuncCompare compare)
{
	return 0;
}

DWORD WINAPI GetTcpTableFromStack(int a1, unsigned int a2, int a3)
{
	return 0;
}

DWORD WINAPI GetUdpExTable2FromStack(int a1, int a2, int a3, HANDLE Handle, PtFuncCompare compare)
{
	return 0;
}

DWORD WINAPI IsLocalAddress(void *this, int a2)
{
	return 0;
}

DWORD WINAPI NTTimeToNTPTime(struct _FILETIME a1)
{
	return 0;
}

DWORD WINAPI NhGetInterfaceNameFromGuid(int a1, int a2, int a3, int a4, signed int a5)
{
	return 0;
}

DWORD WINAPI NhGetGuidFromInterfaceName(int a1, int a2, int a3, signed int a5)
{
	return 0;
}

DWORD WINAPI SetIfEntryToStack(int a1, int a2)
{
	return 0;
}

DWORD WINAPI do_echo_rep(int a1, void *a2, signed int a3, int a4, int a5, int a6, int a7, int a8, void *a9, int a10)
{
	return 0;
}

DWORD WINAPI do_echo_req(HANDLE IcmpHandle, IPAddr DestinationAddress, LPVOID RequestData, int RequestSize, UCHAR *a5, int a6, int a7, int a8, char a9, char a10)
{
	return 0;
}

DWORD IcmpParseReplies(
  _In_ LPVOID ReplyBuffer,
  _In_ DWORD  ReplySize
)
{
	return 0;
}