/*++

Copyright (c) 2017  Shorthorn Project

Module Name:

    dep.c

Abstract:

    This module implements DEP APIs for Win32

Author:

    Skulltrail 11-April-2017

Revision History:

--*/

#include <main.h>

DEP_SYSTEM_POLICY_TYPE WINAPI 
GetSystemDEPPolicy(void)
{
	return OptOut;
}

BOOL 
WINAPI 
GetProcessDEPPolicy(HANDLE ProcessInformation, LPDWORD lpFlags, PBOOL lpPermanent)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  ULONG number; // eax@3
  LPDWORD value; // ecx@3

  status = NtQueryInformationProcess(ProcessInformation, ProcessIoCounters, &ProcessInformation, 4u, 0);
  if ( status >= 0 )
  {
    number = (ULONG)ProcessInformation;
    value = 0;
    if ( !((ULONG)ProcessInformation & 2) )
    {
    value = (LPDWORD)1;
      if ( (ULONG)ProcessInformation & 4 )
        value = (LPDWORD)3;
    }
    lpFlags = value;
    *lpPermanent = (number >> 3) & 1;
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  return result;
}

DWORD 
WINAPI 
SetProcessDEPPolicy(DWORD dwFlags)
{
  BOOL value; // eax@1
  NTSTATUS status; // [sp-4h] [bp-4h]@2

  value = dwFlags;
  if ( dwFlags & 0xFFFFFFFC )
  {
    status = STATUS_INVALID_PARAMETER;
ERROR:
    BaseSetLastNTError(status);
    return FALSE;
  }
  if ( dwFlags & 1 )
  {
    dwFlags = 9;
    if ( value & 2 )
      dwFlags = 13;
  }
  else
  {
    if ( dwFlags & 2 )
    {
      status = STATUS_INVALID_PARAMETER_MIX;
      goto ERROR;
    }
    dwFlags = 2;
  }
  status = NtSetInformationProcess((HANDLE)0xFFFFFFFF, (PROCESSINFOCLASS)0x22u, &dwFlags, 4u);
  if ( !NT_SUCCESS(status))
  {
    goto ERROR;
  }
  return TRUE;
}