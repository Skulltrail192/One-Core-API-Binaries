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

BOOL
WINAPI
GetNumaAvailableMemoryNode(
    UCHAR Node,
    PULONGLONG AvailableBytes
    )


/*++

Routine Description:

    This routine returns the (aproximate) amount of memory available
    on a given node.

Arguments:

    Node        Node number for which available memory count is
                needed.
    AvailableBytes  Supplies a pointer to a ULONGLONG in which the
                    number of bytes of available memory will be 
                    returned.

Return Value:

    TRUE is this call was successful, FALSE otherwise.

--*/

{
    NTSTATUS Status;
    ULONG ReturnedSize;
    SYSTEM_NUMA_INFORMATION Memory;

    //
    // Get the per node available memory table from the system.
    //

    Status = NtQuerySystemInformation(SystemNumaAvailableMemory,
                                      &Memory,
                                      sizeof(Memory),
                                      &ReturnedSize);
    if (!NT_SUCCESS(Status)) {
        BaseSetLastNTError(Status);
        return FALSE;
    }

    //
    // If the requested node doesn't exist, it doesn't have any
    // available memory either.
    //

    if (Node > Memory.HighestNodeNumber) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    //
    // Return the amount of available memory on the requested node.
    //

    *AvailableBytes = Memory.AvailableMemory[Node];
    return TRUE;
}

BOOL 
WINAPI 
GetNumaAvailableMemoryNodeEx(
	USHORT Node, 
	PULONGLONG AvailableBytes
)
{
	return GetNumaAvailableMemoryNode(Node, AvailableBytes);
}


BOOL 
WINAPI 
GetNumaProximityNode(
	ULONG ProximityId, 
	PUCHAR NodeNumber
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  ULONG SystemInformation; // [sp+0h] [bp-8h]@1
  UCHAR number = 0; // [sp+4h] [bp-4h]@3

  SystemInformation = ProximityId;
  status = NtQuerySystemInformation(SystemRegistryQuotaInformation|0x40, &SystemInformation, 8u, &ProximityId);
  if ( NT_SUCCESS(status))
  {
    *NodeNumber = number;
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  return result;
}


LPVOID 
WINAPI 
MapViewOfFileExNuma(
	HANDLE SectionHandle, 
	DWORD dwDesiredAccess, 
	DWORD dwFileOffsetHigh,
	DWORD dwFileOffsetLow, 
	SIZE_T dwNumberOfBytesToMap, 
	LPVOID lpBaseAddress, 
	DWORD nndPreferred
)
{
  DWORD localBaseAddress; // ecx@1
  ULONG other; // eax@6
  DWORD *seila; // eax@12
  ULONG hum; // ecx@16
  NTSTATUS status; // eax@18
  LARGE_INTEGER SectionOffset; // [sp+0h] [bp-8h]@5

  localBaseAddress = nndPreferred;
  if ( nndPreferred != -1 && nndPreferred >= 0x10 )
  {
    SetLastError(0x57u);
    return 0;
  }
  SectionOffset.LowPart = dwFileOffsetHigh>0;
  dwFileOffsetLow = dwNumberOfBytesToMap;
  nndPreferred = (DWORD)lpBaseAddress;
  if ( dwDesiredAccess == 33 )
  {
    other = 128;
  }
  else
  {
    if ( dwDesiredAccess == 1 )
    {
      other = 8;
    }
    else
    {
      if ( dwDesiredAccess & 2 )
      {
        other = (dwDesiredAccess & 0x20) != 0 ? 64 : 4;
      }
      else
      {
        if ( dwDesiredAccess & 4 )
          seila = (DWORD *)((dwDesiredAccess & 0x20) != 0 ? 31 : 1);
        else
          seila = 0;
        other = (ULONG)((char *)seila + 1);
      }
    }
  }
  if ( localBaseAddress == -1 )
    hum = 0;
  else
    hum = localBaseAddress + 1;
  status = NtMapViewOfSection(
             SectionHandle,
             (HANDLE)0xFFFFFFFF,
             (PVOID *)&nndPreferred,
             0,
             0,
             &SectionOffset,
             &dwFileOffsetLow,
             ViewShare,
             hum,
             other);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    return 0;
  }
  return (LPVOID)nndPreferred;
}

BOOL 
WINAPI 
GetNumaNodeNumberFromHandle(
	HANDLE FileInformation, 
	PUSHORT NodeNumber
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+0h] [bp-8h]@1

  status = NtQueryInformationFile(
             FileInformation,
             &IoStatusBlock,
             &FileInformation,
             2u,
             FileInformationReserved33|FileEndOfFileInformation);
  if ( NT_SUCCESS(status) )
  {
    *NodeNumber = (unsigned __int16)FileInformation;
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  return result;
}

BOOL
WINAPI
GetNumaNodeProcessorMask(
    UCHAR Node,
    PULONGLONG ProcessorMask
    )

{
    NTSTATUS Status;
    ULONG ReturnedSize;
    SYSTEM_NUMA_INFORMATION Map;

    Status = NtQuerySystemInformation(SystemNumaProcessorMap,
                                      &Map,
                                      sizeof(Map),
                                      &ReturnedSize);
    if (!NT_SUCCESS(Status)) {

        BaseSetLastNTError(Status);
        return FALSE;
    }

    if (Node > Map.HighestNodeNumber) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    *ProcessorMask = Map.ActiveProcessorsAffinityMask[Node];
    return TRUE;
}

BOOL 
WINAPI 
GetNumaNodeProcessorMaskEx(
	USHORT Node, 
	PGROUP_AFFINITY ProcessorMask
)
{
	return GetNumaNodeProcessorMask(Node, ProcessorMask);
  // NTSTATUS status; // eax@1
  // KAFFINITY *affinity; // esi@6
  // unsigned int SystemInformation; // [sp+0h] [bp-10Ch]@1
  // char nodes[256]; // [sp+8h] [bp-104h]@6
  // ULONG ReturnLength; // [sp+108h] [bp-4h]@1

  // status = NtQuerySystemInformation(
             // SystemSessionProcessesInformation|SystemPerformanceInformation,
             // &SystemInformation,
             // 0x108u,
             // &ReturnLength);
  // if ( !NT_SUCCESS(status) )
  // {
    // BaseSetLastNTError(status);
    // return FALSE;
  // }
  // if ( Node > SystemInformation )
  // {
    // SetLastError(87);
    // return FALSE;
  // }
  // affinity = (KAFFINITY *)&nodes[12 * Node];
  // ProcessorMask->Mask = *affinity;
  // ++affinity;
  // ProcessorMask->Group = (WORD)affinity;
  // ProcessorMask->Reserved[1] = (WORD)affinity[1];
  // return TRUE;
}

BOOL 
WINAPI 
GetNumaProcessorNodeEx(
	PPROCESSOR_NUMBER Processor, 
	PUSHORT NodeNumber
)
{
  PPROCESSOR_NUMBER number; // esi@1
  int count; // ebx@3
  NTSTATUS status; // eax@4
  BYTE byteLocal; // qax@6
  int otherCount; // ecx@6
  int position; // ecx@7
  unsigned int SystemInformation; // [sp+Ch] [bp-108h]@4
  int other; // [sp+14h] [bp-100h]@8
  WORD group[126]; // [sp+18h] [bp-FCh]@7

  number = Processor;
  if ( Processor->Group >= 1u || Processor->Number >= 0x20u || (count = 0, Processor->Reserved) )
  {
LABEL_10:
    *NodeNumber = -1;
    SetLastError(87);
    return 0;
  }
  status = NtQuerySystemInformation(
             SystemSessionProcessesInformation|SystemPerformanceInformation,
             &SystemInformation,
             0x108u,
             (PULONG)&Processor);
  if ( NT_SUCCESS(status))
  {
    *NodeNumber = -1;
    BaseSetLastNTError(status);
    return FALSE;
  }
  byteLocal = 1i64 << number->Number;
  otherCount = 0;
  while ( TRUE )
  {
    position = 6 * otherCount;
    if ( group[position] == number->Group )
    {
      if ( byteLocal & *(int *)((char *)&other + position * 2) )
        break;
    }
    ++count;
    otherCount = count;
    if ( count > SystemInformation )
      goto LABEL_10;
  }
  *NodeNumber = count;
  return TRUE;
}

BOOL 
WINAPI 
GetNumaProximityNodeEx(
	ULONG ReturnLength, 
	PUSHORT NodeNumber
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  ULONG SystemInformation; // [sp+0h] [bp-8h]@1
  USHORT LocalNodeNumber = 0; // [sp+4h] [bp-4h]@3

  SystemInformation = ReturnLength;
  status = NtQuerySystemInformation(SystemRegistryQuotaInformation|0x40, &SystemInformation, 8u, &ReturnLength);
  if ( status >= 0 )
  {
    *NodeNumber = LocalNodeNumber;
    result = 1;
  }
  else
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  return result;
}

BOOL
WINAPI
GetNumaAvailableMemory(
    PSYSTEM_NUMA_INFORMATION Memory,
    ULONG Length,
    PULONG ReturnedLength
    )

/*++

Routine Description:

    Query the system for the NUMA processor map.

Arguments:

    Memory          Supplies a pointer to a stucture into which the
                    per node available memory data is copied.
    Length          Size of data (ie max size to copy).
    ReturnedLength  Nomber of bytes returned in Memory.

Return Value:

    Returns the length of the data returned.

--*/

{
    NTSTATUS Status;
    ULONG ReturnedSize;

    RtlZeroMemory(Memory, Length);

    //
    // Fill in the user's buffer with the per node available
    // memory table.
    //

    Status = NtQuerySystemInformation(SystemNumaAvailableMemory,
                                      Memory,
                                      Length,
                                      ReturnedLength);
    if (!NT_SUCCESS(Status)) {
        BaseSetLastNTError(Status);
        return FALSE;
    }

    return TRUE;
}

BOOL 
WINAPI 
AllocateUserPhysicalPagesNuma(
	HANDLE ProcessHandle, 
	PULONG NumberOfPages, 
	PULONG PageFrameNumbers, 
	int Node
)
{
  DWORD_PTR number; // esi@1
  BOOL result; // eax@3
  DWORD_PTR compare; // ST0C_4@4
  HANDLE otherHandle; // eax@4
  NTSTATUS status; // edi@5
  HANDLE localHandle; // eax@6
  ULONGLONG dwThreadAffinityMask; // [sp+4h] [bp-8h]@2

  number = 0;
  if ( Node == -1
    || GetNumaNodeProcessorMask(Node, &dwThreadAffinityMask)
    && (compare = (DWORD_PTR)dwThreadAffinityMask,
        otherHandle = GetCurrentThread(),
        (number = SetThreadAffinityMask(otherHandle, compare)) != 0) )
  {
    status = NtAllocateUserPhysicalPages(ProcessHandle, NumberOfPages, PageFrameNumbers);
    if ( number )
    {
      localHandle = GetCurrentThread();
      SetThreadAffinityMask(localHandle, number);
    }
    if ( status < 0 )
    {
      BaseSetLastNTError(status);
      result = FALSE;
    }
    else
    {
      result = TRUE;
    }
  }
  else
  {
    result = FALSE;
  }
  return result;
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
  NTSTATUS status; // eax@8

  if ( BaseAddress && BaseAddress < (LPVOID)BaseStaticServerData || nndPreferred != -1 && nndPreferred >= 0x10 )
  {
    SetLastError(0x57u);
  }
  else
  {
    AllocationType &= 0xFFFFFFF0u;
    if ( nndPreferred != -1 )
      AllocationType |= nndPreferred + 1;
    status = NtAllocateVirtualMemory(ProcessHandle, &BaseAddress, 0, &AllocationSize, AllocationType, Protect);
    if ( status >= 0 )
      return BaseAddress;
    BaseSetLastNTError(status);
  }
  return NULL;
}