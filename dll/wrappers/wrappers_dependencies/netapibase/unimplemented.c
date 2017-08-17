/* Copyright 2001 Mike McCormack
 * Copyright 2003 Juan Lang
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
 */

#include "netapi32.h"

#include <lmserver.h>
#include <dsgetdc.h>

typedef struct sockaddr {
   USHORT sa_family;
   CHAR sa_data[14];
} SOCKADDR, *PSOCKADDR, FAR *LPSOCKADDR;

typedef struct _SOCKET_ADDRESS {
   LPSOCKADDR lpSockaddr;
   INT iSockaddrLength;
} SOCKET_ADDRESS, *PSOCKET_ADDRESS, *LPSOCKET_ADDRESS;

DWORD WINAPI DsAddressToSiteNamesA(LPCSTR ComputerName, DWORD EntryCount, PSOCKET_ADDRESS SocketAddresses, LPSTR **SiteNames)
{
	return 0;
}

DWORD WINAPI DsAddressToSiteNamesW(LPCWSTR ComputerName, DWORD EntryCount, PSOCKET_ADDRESS SocketAddresses, LPWSTR **SiteNames)
{
	return 0;
}


DWORD WINAPI DsAddressToSiteNamesExA(LPCSTR ComputerName, DWORD EntryCount, PSOCKET_ADDRESS SocketAddresses, LPSTR **SiteNames, LPSTR **SubnetNames)
{
	return 0;
}

DWORD WINAPI DsAddressToSiteNamesExW(LPCWSTR ComputerName, DWORD EntryCount, PSOCKET_ADDRESS SocketAddresses, LPWSTR **SiteNames, LPWSTR **SubnetNames)
{
	return 0;
}

DWORD WINAPI DsDeregisterDnsHostRecordsA(LPSTR ServerName, LPSTR DnsDomainName, GUID *DomainGuid, GUID *DsaGuid, LPSTR DnsHostName)
{
	return 0;
}

DWORD WINAPI DsDeregisterDnsHostRecordsW(LPWSTR ServerName, LPWSTR DnsDomainName, GUID *DomainGuid, GUID *DsaGuid, LPWSTR DnsHostName)
{
	return 0;
}

DWORD WINAPI DsEnumerateDomainTrustsA(LPSTR ServerName, ULONG Flags, PDS_DOMAIN_TRUSTSA *Domains, PULONG DomainCount)
{
	return 0;
}

DWORD WINAPI DsEnumerateDomainTrustsW(LPWSTR ServerName, ULONG Flags, PDS_DOMAIN_TRUSTSW *Domains, PULONG DomainCount)
{
	return 0;
}

void WINAPI DsGetDcCloseW(HANDLE GetDcContextHandle)
{
	;
}

DWORD WINAPI DsGetDcNextA(HANDLE GetDcContextHandle, PULONG SockAddressCount, LPSOCKET_ADDRESS *SockAddresses, LPSTR *DnsHostName)
{
	return 0;
}

DWORD WINAPI DsGetDcNextW(HANDLE GetDcContextHandle, PULONG SockAddressCount, LPSOCKET_ADDRESS *SockAddresses, LPWSTR *DnsHostName)
{
	return 0;
}

DWORD WINAPI DsGetDcOpenA(LPCSTR DnsName, 
						  ULONG OptionFlags, 
						  LPCSTR SiteName, 
						  GUID *DomainGuid, 
						  LPCSTR DnsForestName, 
						  ULONG DcFlags, 
						  PHANDLE RetGetDcContext)
{
	return 0;
}

DWORD WINAPI DsGetDcOpenW(LPCWSTR DnsName, 
						  ULONG OptionFlags, 
						  LPCWSTR SiteName, 
						  GUID *DomainGuid, 
						  LPCWSTR DnsForestName, 
						  ULONG DcFlags, 
						  PHANDLE RetGetDcContext)
{
	return 0;
}

DWORD WINAPI DsGetDcSiteCoverageA(LPCSTR ServerName, PULONG EntryCount, LPSTR **SiteNames)
{
	return 0;
}

DWORD WINAPI DsGetDcSiteCoverageW(LPCWSTR ServerName, PULONG EntryCount, LPWSTR **SiteNames)
{
	return 0;
}

DWORD WINAPI DsGetSiteNameA(LPCSTR ComputerName, LPSTR *SiteName)
{
	return 0;
}

DWORD WINAPI DsMergeForestTrustInformationW(PCWSTR SourceString, PCUNICODE_STRING String1, int a3, int a4)
{
	return 0;
}