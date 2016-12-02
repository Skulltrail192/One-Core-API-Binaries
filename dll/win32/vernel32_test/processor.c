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
#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

BOOL WINAPI QueryIdleProcessorCycleTime(PULONG ReturnLength, PULONG64 SystemInformation)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@3

  status = NtQuerySystemInformation(
             SystemInstructionEmulationCounts|0x40,
             SystemInformation,
             *ReturnLength,
             ReturnLength);
  if ( status < 0 && SystemInformation )
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

BOOL WINAPI QueryIdleProcessorCycleTimeEx(USHORT Group, PULONG BufferLength, PULONG64 ProcessorIdleCycleTime)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@3

  status = NtQuerySystemInformationEx(83, 
  &Group, 
  2, 
  ProcessorIdleCycleTime, 
  &BufferLength,
  BufferLength);
  if ( status < 0 && ProcessorIdleCycleTime )
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

BOOL WINAPI GetProcessGroupAffinity(HANDLE ProcessHandle, PUSHORT GroupCount, PUSHORT GroupArray)
{
  PUSHORT LocalGroupCount; // esi@1
  NTSTATUS status; // eax@1
  BOOL result; // eax@5

  LocalGroupCount = GroupCount;
  status = NtQueryInformationProcess(
             ProcessHandle,
             ProcessWorkingSetWatch|0x20,
             GroupArray,
             2 * *GroupCount,
             (PULONG)&GroupCount);
  if ( status >= 0 || status == 0xC0000023 )
    *LocalGroupCount = (unsigned int)GroupCount >> 1;
  if ( status >= 0 )
  {
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  return result;
}

BOOL WINAPI GetProcessorSystemCycleTime(USHORT Group, PSYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION Buffer, PDWORD ReturnedLength)
{
  NTSTATUS status; // eax@4

  if ( !ReturnedLength )
  {
    SetLastError(87);
    return FALSE;
  }
  status = NtQuerySystemInformationEx(108, &Group, 2, (PULONG64)Buffer->CycleTime, (PULONG*)ReturnedLength, ReturnedLength);
  if ( status == 0xC0000004 )
    status = 0xC0000023u;
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    return FALSE;
  }
  return TRUE;
}

DWORD WINAPI GetMaximumProcessorCount(
  _In_  WORD GroupNumber
)
{
	UNIMPLEMENTED;
	DbgPrint("THIS IS A HACK");
	return 256;
}

WORD WINAPI GetMaximumProcessorGroupCount(void)
{
	UNIMPLEMENTED;
	DbgPrint("THIS IS A HACK");	
	return 64;
}

WORD WINAPI GetActiveProcessorGroupCount(void)
{
	UNIMPLEMENTED;
	DbgPrint("THIS IS A HACK");	
	return 64;
}

DWORD CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR)*8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;    
    DWORD i;
    
    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest)?1:0);
        bitTest/=2;
    }

    return bitSetCount;
}

DWORD WINAPI GetActiveProcessorCount(
  _In_  WORD GroupNumber
)
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
	DWORD lenght = 0;
	DWORD processorCoreCount = 0;
	DWORD logicalProcessorCount = 0;
	DWORD processorPackageCount = 0;
	DWORD byteOffset = 0;
	GetLogicalProcessorInformation(buffer, &lenght);
	while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= lenght) 
    {
        switch (buffer->Relationship) 
        {
        case RelationProcessorCore:
            processorCoreCount++;

            // A hyperthreaded core supplies more than one logical processor.
            logicalProcessorCount += CountSetBits(buffer->ProcessorMask);
			GroupNumber = logicalProcessorCount;
            break;

        case RelationProcessorPackage:
            // Logical processors share a physical package.
            processorPackageCount++;
			GroupNumber = processorPackageCount;
            break;

        default:
            DbgPrint("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n");
            break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        buffer++;
    }
	return GroupNumber;
}

BOOL WINAPI GetLogicalProcessorInformation(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION Buffer, PDWORD ReturnedLength)
{
  NTSTATUS status; // eax@5

  if ( !ReturnedLength )
  {
    SetLastError(0x57u);
    return FALSE;
  }
  status = NtQuerySystemInformation(SystemLogicalProcessorInformation|0x40, Buffer, *ReturnedLength, ReturnedLength);
  if ( status == STATUS_INFO_LENGTH_MISMATCH )
    status = STATUS_BUFFER_TOO_SMALL;
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    return FALSE;
  }
  return TRUE;
}

BOOL WINAPI GetLogicalProcessorInformationEx(
  _In_       LOGICAL_PROCESSOR_RELATIONSHIP RelationshipType,
  _Out_opt_  PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX Buffer,
  _Inout_    PDWORD ReturnedLength
)
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION information = NULL;
	return GetLogicalProcessorInformation(information, ReturnedLength);
}