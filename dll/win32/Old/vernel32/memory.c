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
  PVOID *one; // eax@1
  PVOID *two; // ebx@1
  NTSTATUS status; // eax@2
  ULONG ReturnLength; // [sp+10h] [bp-24h]@1
  PVOID other; // [sp+14h] [bp-20h]@1
  NTSTATUS NtStatus; // [sp+18h] [bp-1Ch]@1
  CPPEH_RECORD ms_exc; // [sp+1Ch] [bp-18h]@1

  NtStatus = 0;
  ReturnLength = 0;
  other = 0;
#ifdef _M_IX86
	one = (PVOID *)RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), BaseDllTag, BufferSize + 20);
#elif defined(_M_AMD64)
	one = (PVOID *)RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), BaseDllTag, BufferSize + 20);
#endif
  two = one;
  ms_exc.registration.TryLevel = 0;
  if ( one )
  {
    *one = (PVOID)FirmwareTableProviderSignature;
    one[2] = (PVOID)FirmwareTableID;
    one[3] = (PVOID)BufferSize;
    one[1] = (PVOID)1;
    status = NtQuerySystemInformation(SystemLockInformation|0x40, one, BufferSize + 20, &ReturnLength);
    NtStatus = status;
    if ( status >= 0 || status == STATUS_BUFFER_TOO_SMALL )
      other = two[3];
    if ( status >= 0 && pFirmwareTableBuffer )
    {
      memcpy(pFirmwareTableBuffer, two + 4, (unsigned int)two[3]);
      NtStatus = 0;
    }
  }
  else
  {
    NtStatus = STATUS_INSUFFICIENT_RESOURCES;
  }
  ms_exc.registration.TryLevel = -1;
  if ( two )
#ifdef _M_IX86
    RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, two);
#elif defined(_M_AMD64)
    RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, two);
#endif
  BaseSetLastNTError(NtStatus);
  return BufferSize;
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