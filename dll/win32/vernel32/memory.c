 /*
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

#define SystemLockInformation 12

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

BOOL WINAPI RemoveSecureMemoryCacheCallback(
  _In_  PSECURE_MEMORY_CACHE_CALLBACK pfnCallBack
)
{
	return RtlDeregisterSecureMemoryCacheCallback(pfnCallBack);;
}

BOOL WINAPI AddSecureMemoryCacheCallback(
  _In_  PSECURE_MEMORY_CACHE_CALLBACK pfnCallBack
)
{
	return TRUE;
}

BOOL WINAPI GetPhysicallyInstalledSystemMemory(
  _Out_  PULONGLONG TotalMemoryInKilobytes
)
{
	LPMEMORYSTATUSEX memory = NULL;
	GlobalMemoryStatusEx(memory);
	*TotalMemoryInKilobytes = memory->ullTotalPhys/DIV;
	return TRUE;
}

UINT WINAPI pGetSystemFirmwareTable(DWORD FirmwareTableProviderSignature, DWORD FirmwareTableID, PVOID pFirmwareTableBuffer, DWORD BufferSize)
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

BOOL WINAPI GetMemoryErrorHandlingCapabilities(PULONG Capabilities)
{
  *Capabilities = 0;
  if ( pGetSystemFirmwareTable(0x41435049u, 0x46534152u, 0, 0) )
  {
    *Capabilities |= 1u;
    return 1;
  }
  if ( GetLastError() == 1168 )
    return 1;
  return 0;
}

BOOL WINAPI UnregisterBadMemoryNotification(PVOID RegistrationHandle)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = RtlUnsubscribeWnfStateChangeNotification(RegistrationHandle);
  if ( status >= 0 )
  {
    result = 1;
  }
  else
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  return result;
}


BOOL WINAPI PrefetchVirtualMemory(HANDLE hProcess, ULONG_PTR NumberOfEntries, PWIN32_MEMORY_RANGE_ENTRY VirtualAddresses, ULONG Flags)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = NtSetInformationVirtualMemory(hProcess, 0, NumberOfEntries, VirtualAddresses, &Flags, 4);
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


PVOID WINAPI RegisterBadMemoryNotification(
  _In_  PBAD_MEMORY_CALLBACK_ROUTINE Callback
)
{
	UNIMPLEMENTED;
	return 0;
}