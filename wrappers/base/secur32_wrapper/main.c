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
#include <wincred.h>
#include <winbase.h>
#include <secext.h>
#include <security.h>

DWORD SecTlsIP;
DWORD state;
state = 0;
SecTlsIP = 0;

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

typedef SECURITY_STATUS (WINAPI *SASL_GET_CONTEXT_OPTION)(
    PCtxtHandle,
    ULONG,
    PVOID,
    ULONG,
    PULONG);

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

SECURITY_STATUS WINAPI SecpSetCallFlags(int a1)
{
	SECURITY_STATUS result; // eax@3
    result = 0x80090300u;
	return result;
}

SECURITY_STATUS WINAPI SecpSetIPAddress(const void *ipNumber, ULONG number)
{
  PVOID receive; // edi@3
  SECURITY_STATUS result; // eax@4
  HLOCAL local; // eax@6

  if ( state & 0x20 || number > 0x20 )
  {
    result = 0x80090300u;
  }
  else
  {
    receive = TlsGetValue(SecTlsIP);
    if ( receive )
    {
LABEL_4:
      memcpy(receive, ipNumber, number);
      return 0;
    }
    local = LocalAlloc(0x40u, 0x20u);
    receive = local;
    if ( local )
    {
      TlsSetValue(SecTlsIP, local);
      goto LABEL_4;
    }
    result = 0x80090300u;
  }
  return result;
}


static
SECURITY_STATUS
SaslGetContextOption(
    PCtxtHandle ContextHandle,
    ULONG Option,
    PVOID Value,
    ULONG Size,
    PULONG Needed OPTIONAL
    )
{
    return STATUS_PROCEDURE_NOT_FOUND;
}
