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
#include <exfuncs.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

BOOL WINAPI 
GetFirmwareEnvironmentVariableExW(
	PCWSTR lpName, 
	PCWSTR lpGuid, 
	PVOID pValue, 
	DWORD nSize, 
	PDWORD pdwAttributes
)
{
  NTSTATUS status; // eax@3
  LSA_UNICODE_STRING DestinationString; // [sp+8h] [bp-2Ch]@3
  LSA_UNICODE_STRING GuidString; // [sp+10h] [bp-24h]@3
  PDWORD attributes; // [sp+18h] [bp-1Ch]@1
  BOOL size; // [sp+1Ch] [bp-18h]@5
  GUID Guid; // [sp+20h] [bp-14h]@3

  attributes = pdwAttributes;
  if ( lpName && lpGuid )
  {
    RtlInitUnicodeString(&DestinationString, lpName);
    RtlInitUnicodeString(&GuidString, lpGuid);
    status = RtlGUIDFromString(&GuidString, &Guid);
    if ( status >= 0 )
    {
      size = nSize;
      status = NtQuerySystemEnvironmentValueEx(&DestinationString, &Guid, pValue, &size, attributes);
      if ( status >= 0 )
        return size;
    }
    BaseSetLastNTError(status);
  }
  else
  {
    RtlSetLastWin32Error(87);
  }
  return FALSE;
}

BOOL WINAPI GetFirmwareEnvironmentVariableExA(LPCSTR lpName, LPCSTR lpGuid, PVOID pValue, DWORD nSize, PDWORD pdwAttributes)
{
  NTSTATUS status; // eax@3
  NTSTATUS otherStatus; // esi@6
  BOOL resp; // edi@8
  NTSTATUS errorStatus; // [sp-4h] [bp-2Ch]@4
  STRING string; // [sp+8h] [bp-20h]@6
  struct _STRING DestinationString; // [sp+10h] [bp-18h]@3
  PUNICODE_STRING uniCodeString; // [sp+18h] [bp-10h]@6
  LSA_UNICODE_STRING UnicodeString; // [sp+20h] [bp-8h]@3

  if ( lpName && lpGuid )
  {
    RtlInitString(&DestinationString, lpName);
    status = RtlAnsiStringToUnicodeString(&UnicodeString, &DestinationString, 1u);
    if ( status >= 0 )
    {
      RtlInitString(&string, lpGuid);
      otherStatus = RtlAnsiStringToUnicodeString((PUNICODE_STRING)&uniCodeString, &string, 1u);
      if ( otherStatus >= 0 )
      {
        resp = GetFirmwareEnvironmentVariableExW(UnicodeString.Buffer, (LPCWSTR)lpGuid, pValue, nSize, pdwAttributes);
        RtlFreeUnicodeString(&UnicodeString);
        RtlFreeUnicodeString((PUNICODE_STRING)&uniCodeString);
        return resp;
      }
      RtlFreeUnicodeString(&UnicodeString);
      errorStatus = otherStatus;
    }
    else
    {
      errorStatus = status;
    }
    BaseSetLastNTError(errorStatus);
  }
  else
  {
    RtlSetLastWin32Error(87);
  }
  return 0;
}

BOOL WINAPI GetFirmwareType(PFIRMWARE_TYPE FirmwareType)
{
  NTSTATUS status; // eax@4
  PFIRMWARE_TYPE SystemInformation; // [sp+4h] [bp-24h]@4

  if ( !FirmwareType )
  {
    RtlSetLastWin32Error(87);
    return 0;
  }
  status = NtQuerySystemInformation(SystemLoadImage|0x40, &SystemInformation, 0x20u, 0);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    return 0;
  }
  FirmwareType = SystemInformation;
  return 1;
}

BOOL WINAPI SetFirmwareEnvironmentVariableExW(LPCWSTR lpName, LPCWSTR lpGuid, PVOID pValue, DWORD nSize, DWORD dwAttributes)
{
  NTSTATUS error; // eax@3
  LSA_UNICODE_STRING GuidString; // [sp+8h] [bp-24h]@3
  LSA_UNICODE_STRING DestinationString; // [sp+10h] [bp-1Ch]@3
  GUID Guid; // [sp+18h] [bp-14h]@3

  if ( lpName && lpGuid )
  {
    RtlInitUnicodeString(&DestinationString, lpName);
    RtlInitUnicodeString(&GuidString, lpGuid);
    error = RtlGUIDFromString(&GuidString, &Guid);
    if ( error >= 0 )
    {
      error = NtSetSystemEnvironmentValueEx(&DestinationString, &Guid, pValue, nSize, dwAttributes); //commented for now
      if ( error >= 0 )
        return 1;
    }
    BaseSetLastNTError(error);
  }
  else
  {
    RtlSetLastWin32Error(87);
  }
  return 0;
}

BOOL WINAPI SetFirmwareEnvironmentVariableExA(LPCTSTR SourceString, LPCTSTR lpGuid, PVOID pValue, DWORD nSize, DWORD dwAttributes)
{
  NTSTATUS statusConvert; // eax@3
  NTSTATUS otherStatus; // esi@6
  BOOL rsp; // edi@8
  NTSTATUS error; // [sp-4h] [bp-2Ch]@4
  STRING ansiString; // [sp+8h] [bp-20h]@6
  struct _STRING DestinationString; // [sp+10h] [bp-18h]@3
  PUNICODE_STRING localLpGuid = NULL; // [sp+18h] [bp-10h]@6
  LSA_UNICODE_STRING UnicodeString; // [sp+20h] [bp-8h]@3

  if ( SourceString && lpGuid )
  {
    RtlInitString(&DestinationString, SourceString);
    statusConvert = RtlAnsiStringToUnicodeString(&UnicodeString, &DestinationString, 1u);
    if ( statusConvert >= 0 )
    {
      RtlInitString(&ansiString, lpGuid);
      otherStatus = RtlAnsiStringToUnicodeString(localLpGuid, &ansiString, 1u);
      if ( otherStatus >= 0 )
      {
        rsp = SetFirmwareEnvironmentVariableExW(UnicodeString.Buffer, localLpGuid->Buffer, pValue, nSize, dwAttributes);
        RtlFreeUnicodeString(&UnicodeString);
        RtlFreeUnicodeString(localLpGuid);
        return rsp;
      }
      RtlFreeUnicodeString(&UnicodeString);
      error = otherStatus;
    }
    else
    {
      error = statusConvert;
    }
    BaseSetLastNTError(error);
  }
  else
  {
    RtlSetLastWin32Error(87);
  }
  return 0;
}

UINT 
WINAPI 
pGetSystemFirmwareTable(
	DWORD FirmwareTableProviderSignature, 
	DWORD FirmwareTableID, 
	PVOID pFirmwareTableBuffer, 
	DWORD BufferSize
)
{
  PSYSTEM_BASIC_INFORMATION SystemInformation; 
  PSYSTEM_BASIC_INFORMATION LocalSytemInformation; 
  NTSTATUS Status; 
  ULONG ReturnLength;
  UINT NumberOfPhysicalPages; 
  NTSTATUS NtStatus; 
  CPPEH_RECORD ms_exc; 

  NtStatus = 0;
  ReturnLength = 0;
  NumberOfPhysicalPages = 0;
  SystemInformation = (PSYSTEM_BASIC_INFORMATION)RtlAllocateHeap(
                                                   RtlGetProcessHeap(),
                                                   BaseDllTag,
                                                   BufferSize + 20);
  LocalSytemInformation = SystemInformation;
  ms_exc.registration.TryLevel = 0;
  if ( SystemInformation )
  {
    SystemInformation->Reserved = FirmwareTableProviderSignature;
    SystemInformation->PageSize = FirmwareTableID;
    SystemInformation->NumberOfPhysicalPages = BufferSize;
    SystemInformation->TimerResolution = 1;
    Status = NtQuerySystemInformation(0x40, SystemInformation, BufferSize + 20, &ReturnLength);
    NtStatus = Status;
    if ( Status >= STATUS_SUCCESS || Status == STATUS_BUFFER_TOO_SMALL )
      NumberOfPhysicalPages = LocalSytemInformation->NumberOfPhysicalPages;
    if ( Status >= STATUS_SUCCESS && pFirmwareTableBuffer )
    {
      memcpy(
        pFirmwareTableBuffer,
        &LocalSytemInformation->LowestPhysicalPageNumber,
        LocalSytemInformation->NumberOfPhysicalPages);
      NtStatus = STATUS_SUCCESS;
    }
  }
  else
  {
    NtStatus = STATUS_INSUFFICIENT_RESOURCES;
  }
  ms_exc.registration.TryLevel = -1;
  if ( LocalSytemInformation )
    RtlFreeHeap(RtlGetProcessHeap(), 0, LocalSytemInformation);
  BaseSetLastNTError(NtStatus);
  return NumberOfPhysicalPages;
}

// UINT  //Bug with Skype
// WINAPI 
// GetSystemFirmwareTable(
	// SYSTEM_FIRMWARE_TABLE_INFORMATION *FirmwareTableProviderSignature, 
	// DWORD FirmwareTableID, 
	// PVOID pFirmwareTableBuffer, 
	// DWORD BufferSize
// )
// {
  // SYSTEM_FIRMWARE_TABLE_INFORMATION *SysInfo; // eax@1
  // NTSTATUS Status; // eax@2 MAPDST
  // ULONG ReturnLength; // [sp+14h] [bp-24h]@1
  // UINT resp; // [sp+18h] [bp-20h]@1
  // CPPEH_RECORD ms_exc; // [sp+20h] [bp-18h]@1

  // Status = STATUS_SUCCESS;
  // ReturnLength = 0;
  // resp = FALSE;
  // SysInfo = (SYSTEM_FIRMWARE_TABLE_INFORMATION *)RtlAllocateHeap(
                                                    // NtCurrentTeb()->ProcessEnvironmentBlock->ProcessHeap,
                                                    // BaseDllTag,
                                                    // BufferSize);
  // FirmwareTableProviderSignature = SysInfo;
  // ms_exc.registration.TryLevel = 0;
  // if ( SysInfo )
  // {
    // SysInfo->ProviderSignature = (ULONG)FirmwareTableProviderSignature;
    // SysInfo->TableID = FirmwareTableID;
    // SysInfo->TableBufferLength = BufferSize;
    // SysInfo->Action = 1;
    // Status = NtQuerySystemInformation(SystemFirmwareTableInformation|0x40, &SysInfo, BufferSize, &ReturnLength);
    // if ( (Status & 0x80000000) == 0 || Status == STATUS_BUFFER_TOO_SMALL )
      // resp = FirmwareTableProviderSignature->TableBufferLength;
    // if ( (Status & 0x80000000) == 0 && pFirmwareTableBuffer )
    // {
      // ms_exc.registration.TryLevel = 1;
      // memcpy(
        // pFirmwareTableBuffer,
        // FirmwareTableProviderSignature->TableBuffer,
        // FirmwareTableProviderSignature->TableBufferLength);
      // ms_exc.registration.TryLevel = 0;
      // Status = STATUS_SUCCESS;
    // }
  // }
  // else
  // {
    // Status = STATUS_INSUFFICIENT_RESOURCES;
  // }
  // ms_exc.registration.TryLevel = -2;
  // if ( FirmwareTableProviderSignature )
    // RtlFreeHeap(NtCurrentTeb()->ProcessEnvironmentBlock->ProcessHeap, 0, FirmwareTableProviderSignature);
  // BaseSetLastNTError(Status);
  // return resp;
// }