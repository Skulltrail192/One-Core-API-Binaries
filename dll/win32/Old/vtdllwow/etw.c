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

#include "config.h"
#include <assert.h>
#include <rtl.h>
#include <evntprov.h>

/*unimplemented */
void WINAPI WinSqmEventWrite(int a1, int a2, int a3)
{
  ;
}

ULONG WINAPI EtwEventUnregister(
  _In_  REGHANDLE RegHandle
)
{
	return ERROR_SUCCESS;
}

ULONG
WINAPI
EtwEventRegister(
  IN LPCGUID ProviderId,
  IN PENABLECALLBACK EnableCallback OPTIONAL,
  IN PVOID CallbackContext OPTIONAL,
  OUT PREGHANDLE RegHandle)
{
	return ERROR_SUCCESS;
}

BOOLEAN WINAPI EtwEventEnabled(
  _In_  REGHANDLE RegHandle,
  _In_  PCEVENT_DESCRIPTOR EventDescriptor
)
{
	return FALSE;
}

ULONG
WINAPI
EtwEventWrite(
  IN REGHANDLE RegHandle,
  IN PCEVENT_DESCRIPTOR EventDescriptor,
  IN ULONG UserDataCount,
  IN PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}

ULONG
WINAPI
EtwEventWriteFull(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    USHORT EventProperty,
    LPCGUID ActivityId,
    LPCGUID RelatedActivityId,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}

ULONG
WINAPI
EtwWriteUMSecurityEvent (
    PCEVENT_DESCRIPTOR EventDescriptor,
    USHORT EventProperty,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}
	
NTSTATUS
WINAPI
EtwReplyNotification(ULONG parameter)
{
	return 0xC000000Du;
}

ULONG WINAPI EtwTraceDiagnosticEvent(BOOLEAN one, int two, DWORD three, BOOL four, BOOL five)
{
	return ERROR_SUCCESS;
}

BOOL WINAPI EtwpIsHeapRangeLoggingEnabled()
{
	return TRUE;
}

BOOL WINAPI EtwpIsPrivateLoggerOn()
{
	return TRUE;
}