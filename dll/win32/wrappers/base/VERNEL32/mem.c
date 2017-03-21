/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    mem.c

Abstract:

    This module contains the Win32 Global Memory Management APIs

Author:

    Steve Wood (stevewo) 19-March-2017

Revision History:

--*/

#include "main.h"

SIZE_T
WINAPI
GetLargePageMinimum (
    VOID
)
{
    return (SIZE_T) SharedUserData->LargePageMinimum;
}

BOOL 
WINAPI 
SetSystemFileCacheSize(
	SIZE_T MinimumFileCacheSize, 
	SIZE_T MaximumFileCacheSize, 
	int Flags
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  char SystemInformation; // [sp+0h] [bp-24h]@1

  status = NtSetSystemInformation(SystemObjectInformation|0x40, &SystemInformation, 0x24u);
  if ( status < 0 )
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