/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    powrmgmt.c

Abstract:

    This module implements Power functions for the Win32 APIs.

Author:

    Skulltrail 14-May-2017

Revision History:

--*/

#include <main.h>

BOOL 
WINAPI 
PowerSetRequest(
	HANDLE PowerRequest, 
	POWER_REQUEST_TYPE RequestType
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  HANDLE InputBuffer; // [sp+0h] [bp-Ch]@1

  status = NtPowerInformation(PowerRequestAction, &InputBuffer, 0xCu, &RequestType, 0);
  if ( !NT_SUCCESS(status) )
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  else
  {
    result = TRUE;
  }
  return result;
}

BOOL 
WINAPI 
PowerClearRequest(
	HANDLE PowerRequest, 
	POWER_REQUEST_TYPE RequestType
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  HANDLE InputBuffer; // [sp+0h] [bp-Ch]@1

  status = NtPowerInformation(PowerRequestAction, &InputBuffer, 0xCu, &RequestType, 0);
  if ( !NT_SUCCESS(status) )
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  else
  {
    result = TRUE;
  }
  return result;
}

HANDLE 
WINAPI 
PowerCreateRequest(
	REASON_CONTEXT *context
)
{
  NTSTATUS status; // eax@1
  PVOID address = NULL; // ST10_4@4
  HANDLE proAddress; // eax@4
  HANDLE OutputBuffer; // [sp+0h] [bp-8h]@1

  OutputBuffer = (HANDLE)-1;
  if(status = NtPowerInformation(PowerRequestCreate, address, 0x1Cu, &OutputBuffer, 4u), status < 0) 
    BaseSetLastNTError(status);
  if ( address )
  {
    proAddress = GetProcessHeap();
    HeapFree(proAddress, 0, address);
  }
  return OutputBuffer;
}