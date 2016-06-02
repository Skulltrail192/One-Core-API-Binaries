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

BOOL WINAPI GetNumaAvailableMemoryNodeEx(USHORT Node, PULONGLONG AvailableBytes)
{
  NTSTATUS status; // eax@1
  PULONGLONG Avaliable; // edx@6
  ULONG SystemInformation; // [sp+0h] [bp-10Ch]@1
  int hum; // [sp+8h] [bp-104h]@6
  int position[63]; // [sp+Ch] [bp-100h]@6
  ULONG ReturnLength; // [sp+108h] [bp-4h]@1

  status = NtQuerySystemInformation(SystemAddVerifier|SystemProcessorTimes, &SystemInformation, 0x108u, &ReturnLength);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    return 0;
  }
  if ( Node > SystemInformation )
  {
    SetLastError(87);
    return 0;
  }
  Avaliable = (PULONGLONG)*(&hum + 2 * Node);
  *((DWORD *)AvailableBytes + 1) = position[2 * Node];
  AvailableBytes = Avaliable;
  return 1;
}


BOOL WINAPI GetNumaProximityNode(ULONG ProximityId, PUCHAR NodeNumber)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  ULONG SystemInformation; // [sp+0h] [bp-8h]@1
  UCHAR number = 0; // [sp+4h] [bp-4h]@3

  SystemInformation = ProximityId;
  status = NtQuerySystemInformation(SystemRegistryQuotaInformation|0x40, &SystemInformation, 8u, &ProximityId);
  if ( status >= 0 )
  {
    *NodeNumber = number;
    result = 1;
  }
  else
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  return result;
}


LPVOID WINAPI MapViewOfFileExNuma(HANDLE SectionHandle, 
								  DWORD dwDesiredAccess, 
								  DWORD dwFileOffsetHigh,
								  DWORD dwFileOffsetLow, 
								  SIZE_T dwNumberOfBytesToMap, 
								  LPVOID lpBaseAddress, 
								  DWORD nndPreferred)
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

BOOL WINAPI GetNumaNodeNumberFromHandle(HANDLE FileInformation, PUSHORT NodeNumber)
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
  if ( status >= 0 )
  {
    *NodeNumber = (unsigned __int16)FileInformation;
    result = 1;
  }
  else
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  return result;
}

BOOL WINAPI GetNumaNodeProcessorMaskEx(USHORT Node, PGROUP_AFFINITY ProcessorMask)
{
  NTSTATUS status; // eax@1
  KAFFINITY *affinity; // esi@6
  unsigned int SystemInformation; // [sp+0h] [bp-10Ch]@1
  char nodes[256]; // [sp+8h] [bp-104h]@6
  ULONG ReturnLength; // [sp+108h] [bp-4h]@1

  status = NtQuerySystemInformation(
             SystemSessionProcessesInformation|SystemPerformanceInformation,
             &SystemInformation,
             0x108u,
             &ReturnLength);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    return 0;
  }
  if ( Node > SystemInformation )
  {
    SetLastError(87);
    return 0;
  }
  affinity = (KAFFINITY *)&nodes[12 * Node];
  ProcessorMask->Mask = *affinity;
  ++affinity;
  ProcessorMask->Group = (WORD)affinity;
  ProcessorMask->Reserved[1] = (WORD)affinity[1];
  return 1;
}

BOOL WINAPI GetNumaProcessorNodeEx(PPROCESSOR_NUMBER Processor, PUSHORT NodeNumber)
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
  if ( status < 0 )
  {
    *NodeNumber = -1;
    BaseSetLastNTError(status);
    return 0;
  }
  byteLocal = 1i64 << number->Number;
  otherCount = 0;
  while ( 1 )
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
  return 1;
}

BOOL WINAPI GetNumaProximityNodeEx(ULONG ReturnLength, PUSHORT NodeNumber)
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