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

BOOL 
WINAPI 
QueryIdleProcessorCycleTime(
	PULONG ReturnLength, 
	PULONG64 SystemInformation
)
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

BOOL 
WINAPI 
QueryIdleProcessorCycleTimeEx(
	USHORT Group, 
	PULONG BufferLength, 
	PULONG64 ProcessorIdleCycleTime
)
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

BOOL 
WINAPI 
GetProcessGroupAffinity(
	HANDLE ProcessHandle, 
	PUSHORT GroupCount, 
	PUSHORT GroupArray
)
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
  if ( status >= 0 || status == STATUS_BUFFER_TOO_SMALL )
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

BOOL 
WINAPI 
GetProcessorSystemCycleTime(
	USHORT Group, 
	PSYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION Buffer, 
	PDWORD ReturnedLength
)
{
  NTSTATUS status; // eax@4

  if ( !ReturnedLength )
  {
    SetLastError(87);
    return FALSE;
  }
  status = NtQuerySystemInformationEx(108, &Group, 2, (PULONG64)Buffer->CycleTime, (PULONG*)ReturnedLength, ReturnedLength);
  if ( status == STATUS_INFO_LENGTH_MISMATCH )
    status = STATUS_BUFFER_TOO_SMALL;
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    return FALSE;
  }
  return TRUE;
}

DWORD 
WINAPI 
GetMaximumProcessorCount(
  _In_  WORD GroupNumber
)
{
	//Windows XP/2003 don't support more than 64 processors, so, we have only one processor group
	return 64;
}

WORD 
WINAPI 
GetMaximumProcessorGroupCount(void)
{
	//Windows XP/2003 don't support more than 64 processors, so, we have only one processor group
	return 1;
}

WORD 
WINAPI 
GetActiveProcessorGroupCount(void)
{
	//Windows XP/2003 don't support more than 64 processors, so, we have only one processor group
	return 1;
}

DWORD 
CountSetBits(
	ULONG_PTR bitMask
)
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

DWORD 
WINAPI 
GetActiveProcessorCount(
  _In_  WORD GroupNumber
)
{
	//We don't support really groups, so, we emulate to support ALL_PROCESSOR_GROUPS	
	SYSTEM_INFO sysinfo;	
	GetSystemInfo( &sysinfo );
	return sysinfo.dwNumberOfProcessors;;
}

BOOL
WINAPI
GetLogicalProcessorInformation(
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION Buffer,
    PDWORD ReturnedLength
    )
{
    NTSTATUS Status;

    if (ReturnedLength == NULL) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    Status = NtQuerySystemInformation( SystemLogicalProcessorInformation,
                                       Buffer,
                                       *ReturnedLength,
                                       ReturnedLength);
    if (Status == STATUS_INFO_LENGTH_MISMATCH) {
        Status = STATUS_BUFFER_TOO_SMALL;
    }

    if (!NT_SUCCESS(Status)) {
        BaseSetLastNTError(Status);
        return FALSE;
    } else {
        return TRUE;
    }
}

BOOL 
WINAPI 
GetLogicalProcessorInformationEx(
  _In_       LOGICAL_PROCESSOR_RELATIONSHIP RelationshipType,
  _Out_opt_  PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX Buffer,
  _Inout_    PDWORD ReturnedLength
)
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION information = NULL;
	DWORD byteOffset = 0;
	SYSTEM_INFO sysinfo;
	
	Buffer->Size = sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
	
	if(!GetLogicalProcessorInformation(information, ReturnedLength)){
		return FALSE;
	}
	Buffer->Relationship = information->Relationship;
	
	//We don't support really groups, so, we emulate to support ALL_PROCESSOR_GROUPS
	while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= *ReturnedLength) {
		switch(Buffer->Relationship){
			case RelationNumaNode:
				Buffer->NumaNode.NodeNumber = information->NumaNode.NodeNumber;
				Buffer->NumaNode.GroupMask.Group = ALL_PROCESSOR_GROUPS;
				break;
			case RelationProcessorCore:
				Buffer->Processor.Flags = information->ProcessorCore.Flags;
				Buffer->Processor.GroupCount = 1;
				Buffer->Processor.GroupMask[0].Group = ALL_PROCESSOR_GROUPS;
				break;
			case RelationProcessorPackage:
				Buffer->Processor.Flags = 0;
				Buffer->Processor.GroupCount = 1;
				Buffer->Processor.GroupMask[0].Group = ALL_PROCESSOR_GROUPS;			
				break;
			case RelationCache:
				Buffer->Cache.Level = information->Cache.Level;
				Buffer->Cache.Associativity = information->Cache.Associativity;
				Buffer->Cache.LineSize = information->Cache.LineSize;
				Buffer->Cache.CacheSize = information->Cache.Size;
				Buffer->Cache.Type = information->Cache.Type;
				Buffer->Cache.GroupMask.Group = ALL_PROCESSOR_GROUPS;				
				break;
			case RelationGroup:
				GetSystemInfo( &sysinfo );
				Buffer->Group.MaximumGroupCount = 1;
				Buffer->Group.ActiveGroupCount = 1;
				Buffer->Group.GroupInfo[0].MaximumProcessorCount = 64;
				Buffer->Group.GroupInfo[0].ActiveProcessorCount = sysinfo.dwNumberOfProcessors;
				break;
		}
	}	
	
	return TRUE;
}