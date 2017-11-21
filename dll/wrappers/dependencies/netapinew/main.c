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

#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>
#include <ntsecapi.h>
#include <bcrypt.h>
#include <lmcons.h>
#include <rpc.h>

typedef enum  { 
  DFS_NAMESPACE_VERSION_ORIGIN_COMBINED  = 0,
  DFS_NAMESPACE_VERSION_ORIGIN_SERVER    = 1,
  DFS_NAMESPACE_VERSION_ORIGIN_DOMAIN    = 2
} DFS_NAMESPACE_VERSION_ORIGIN;

typedef struct _DFS_SUPPORTED_NAMESPACE_VERSION_INFO {
  ULONG     DomainDfsMajorVersion;
  ULONG     NamespaceMinorVersion;
  ULONGLONG DomainDfsCapabilities;
  ULONG     StandaloneDfsMajorVersion;
  ULONG     StandaloneDfsMinorVersion;
  ULONGLONG StandaloneDfsCapabilities;
} DFS_SUPPORTED_NAMESPACE_VERSION_INFO, *PDFS_SUPPORTED_NAMESPACE_VERSION_INFO;

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

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


DWORD WINAPI DavGetHTTPFromUNCPath(
  _In_     LPCWSTR UncPath,
  _Out_    LPWSTR HttpPath,
  _Inout_  LPDWORD lpSize
)
{
	return 0;
}

DWORD WINAPI DavGetUNCFromHTTPPath(
  _In_     LPCWSTR HttpPath,
  _Out_    LPWSTR UncPath,
  _Inout_  LPDWORD lpSize
)
{
	return 0;
}

NET_API_STATUS WINAPI NetShareDelEx(
  _In_  LPWSTR servername,
  _In_  DWORD level,
  _In_  LPBYTE buf
)
{
	return 0x000000000;
}

NET_API_STATUS WINAPI NetServerAliasAdd(
  _In_  LPWSTR servername,
  _In_  DWORD level,
  _In_  LPBYTE buf
)
{
	return 0x000000000;
}

NET_API_STATUS WINAPI NetServerAliasDel(
  _In_  LPWSTR servername,
  _In_  DWORD level,
  _In_  LPBYTE buf
)
{
	return 0x000000000;
}

DWORD WINAPI NetServerAliasEnum(LPWSTR servername, 
								DWORD level, 
								LPBYTE *bufptr, 
								DWORD prefmaxlen, 
								LPDWORD entriesread, 
								LPDWORD totalentries,
								LPDWORD resumehandle)
{
	return 0;
}

DWORD WINAPI DavFlushFile(
  _In_  HANDLE hFile
)
{
	return 0;
}

DWORD WINAPI DavGetExtendedError(
  _In_     HANDLE hFile,
  _Out_    DWORD *ExtError,
  _Out_    LPWSTR ExtErrorString,
  _Inout_  DWORD *cChSize
)
{
	return 0;
}

DWORD WINAPI DavAddConnection(
  _Inout_   HANDLE *ConnectionHandle,
  _In_      LPCWSTR RemoteName,
  _In_opt_  LPCWSTR UserName,
  _In_opt_  LPCWSTR Password,
  _In_      PBYTE ClientCert,
  _In_      DWORD CertSize
)
{
	return 0;
}

DWORD WINAPI DavDeleteConnection(
  _In_  HANDLE ConnectionHandle
)
{
	return 0;
}

NET_API_STATUS WINAPI NetDfsAddRootTarget(
  _In_      LPWSTR pDfsPath,
  _In_opt_  LPWSTR pTargetPath,
  _In_      ULONG MajorVersion,
  _In_opt_  LPWSTR pComment,
  _In_      ULONG Flags
)
{
	return 0x000000000;
}

NET_API_STATUS WINAPI NetDfsGetSupportedNamespaceVersion(
  _In_   DFS_NAMESPACE_VERSION_ORIGIN Origin,
  _In_   PWSTR pName,
  _Out_  PDFS_SUPPORTED_NAMESPACE_VERSION_INFO *ppVersionInfo
)
{
	return 0x000000000;
}

NET_API_STATUS WINAPI NetDfsRemoveRootTarget(
  _In_      LPWSTR pDfsPath,
  _In_opt_  LPWSTR pTargetPath,
  _In_      ULONG Flags
)
{
	return 0x000000000;
}

RPC_STATUS WINAPI NetpAccessCheckAndAuditEx(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	return 0x000000000;
}

HLOCAL WINAPI NetpEventlogClose(LPCRITICAL_SECTION lpCriticalSection)
{
	return NULL;
}

LPCRITICAL_SECTION WINAPI NetpEventlogOpen(LPWSTR Src, LONG a2)
{
	return NULL;
}

LONG WINAPI NetpEventlogWriteEx(LPCRITICAL_SECTION lpCriticalSection, WORD wType, WORD wCategory, DWORD dwEventID, int a5, int a6, int a7, int a8)
{
	return 0;
}