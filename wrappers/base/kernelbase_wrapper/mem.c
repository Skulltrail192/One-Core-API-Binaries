/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    mem.c

Abstract:

    This module contains the Win32 Global Memory Management APIs

Author:

    Skulltrail 19-March-2017

Revision History:

--*/

#include "main.h"

#define DIV 1024

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
GetPhysicallyInstalledSystemMemory(
  _Out_  PULONGLONG TotalMemoryInKilobytes
)
{
	MEMORYSTATUSEX memory;
	memory.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memory);
	*TotalMemoryInKilobytes = memory.ullTotalPhys/DIV;
	return TRUE;
}

BOOL 
WINAPI 
AllocateUserPhysicalPagesNuma(
	HANDLE hProcess, 
	PULONG_PTR NumberOfPages, 
	PULONG_PTR PageArray, 
	DWORD nndPreferred
)
{
  DWORD_PTR ThreadAffinitiyMask;
  NTSTATUS Status; 
  HANDLE CurrentThread; 
  ULONGLONG ProcessorMask; 
  
  DbgPrint("AllocateUserPhysicalPagesNuma called\n");

  ThreadAffinitiyMask = 0;
  if ( nndPreferred != -1 )
  {
    if ( !GetNumaNodeProcessorMask(nndPreferred, &ProcessorMask) )
      return AllocateUserPhysicalPages(hProcess, NumberOfPages, PageArray);
    ThreadAffinitiyMask = ProcessorMask;
    CurrentThread = GetCurrentThread();
    ThreadAffinitiyMask = SetThreadAffinityMask(CurrentThread, ThreadAffinitiyMask);
    if ( !ThreadAffinitiyMask )
      return FALSE;
  }
  Status = NtAllocateUserPhysicalPages(hProcess, NumberOfPages, PageArray);
  if ( ThreadAffinitiyMask )
  {
    CurrentThread = GetCurrentThread();
    SetThreadAffinityMask(CurrentThread, ThreadAffinitiyMask);
  }
  if ( NT_SUCCESS(Status) )
    return TRUE;
  BaseSetLastNTError(Status);
  return FALSE;
}

LPVOID 
WINAPI 
VirtualAllocExNuma(
	HANDLE ProcessHandle, 
	LPVOID BaseAddress, 
	SIZE_T AllocationSize, 
	DWORD AllocationType, 
	DWORD Protect, 
	DWORD nndPreferred
)
{
	if ( nndPreferred != -1 )
      AllocationType |= nndPreferred + 1;
    return VirtualAllocEx(ProcessHandle,
						  BaseAddress,
						  AllocationSize,
						  AllocationType,
						  Protect);
}