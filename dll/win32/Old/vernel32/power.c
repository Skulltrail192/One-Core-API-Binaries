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
#define NDEBUG 
 
#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

BOOL WINAPI PowerSetRequest(HANDLE PowerRequest, POWER_REQUEST_TYPE RequestType)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  HANDLE InputBuffer; // [sp+0h] [bp-Ch]@1
  POWER_REQUEST_TYPE LocalRequestType; // [sp+4h] [bp-8h]@1

  InputBuffer = PowerRequest;
  LocalRequestType = RequestType;
  status = NtPowerInformation(PowerRequestAction, &InputBuffer, 0xCu, 0, 0);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  else
  {
    result = 1;
  }
  return result;
}

BOOL WINAPI PowerClearRequest(HANDLE PowerRequest, POWER_REQUEST_TYPE RequestType)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  HANDLE InputBuffer; // [sp+0h] [bp-Ch]@1
  POWER_REQUEST_TYPE LocalRequestType; // [sp+4h] [bp-8h]@1

  InputBuffer = PowerRequest;
  LocalRequestType = RequestType;
  status = NtPowerInformation(PowerRequestAction, &InputBuffer, 0xCu, 0, 0);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  else
  {
    result = 1;
  }
  return result;
}

HANDLE WINAPI PowerCreateRequest(REASON_CONTEXT *context)
{
  NTSTATUS status; // eax@1
  PVOID address; // ST10_4@4
  HANDLE proAddress; // eax@4
  HANDLE OutputBuffer; // [sp+0h] [bp-8h]@1
  PVOID adderess; // [sp+4h] [bp-4h]@1

  adderess = 0;
  OutputBuffer = (HANDLE)-1;
  status = BasePrepareReasonContext(context, &adderess);
  if ( status < 0 || (status = NtPowerInformation(PowerRequestCreate, adderess, 0x1Cu, &OutputBuffer, 4u), status < 0) )
    BaseSetLastNTError(status);
  if ( adderess )
  {
    address = adderess;
    proAddress = GetProcessHeap();
    HeapFree(proAddress, 0, address);
  }
  return OutputBuffer;
}