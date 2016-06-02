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

#include <../kernel32/k32.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

BOOL WINAPI AllocateUserPhysicalPagesNuma(HANDLE ProcessHandle, PULONG NumberOfPages, PULONG PageFrameNumbers, int Node)
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
  return 0;
}