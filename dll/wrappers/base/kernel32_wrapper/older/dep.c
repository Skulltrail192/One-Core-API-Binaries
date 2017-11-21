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

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

DEP_SYSTEM_POLICY_TYPE WINAPI GetSystemDEPPolicy(void)
{
	return OptOut;
}

BOOL 
WINAPI 
GetProcessDEPPolicy(HANDLE ProcessInformation, LPDWORD lpFlags, PBOOL lpPermanent)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  ULONG number; // eax@3
  LPDWORD value; // ecx@3

  status = NtQueryInformationProcess(ProcessInformation, ProcessIoCounters, &ProcessInformation, 4u, 0);
  if ( status >= 0 )
  {
    number = (ULONG)ProcessInformation;
    value = 0;
    if ( !((ULONG)ProcessInformation & 2) )
    {
    value = (LPDWORD)1;
      if ( (ULONG)ProcessInformation & 4 )
        value = (LPDWORD)3;
    }
    lpFlags = value;
    *lpPermanent = (number >> 3) & 1;
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  return result;
}

DWORD 
WINAPI 
SetProcessDEPPolicy(DWORD dwFlags)
{
  BOOL value; // eax@1
  NTSTATUS status; // [sp-4h] [bp-4h]@2

  value = dwFlags;
  if ( dwFlags & 0xFFFFFFFC )
  {
    status = STATUS_INVALID_PARAMETER;
ERROR:
    BaseSetLastNTError(status);
    return FALSE;
  }
  if ( dwFlags & 1 )
  {
    dwFlags = 9;
    if ( value & 2 )
      dwFlags = 13;
  }
  else
  {
    if ( dwFlags & 2 )
    {
      status = STATUS_INVALID_PARAMETER_MIX;
      goto LABEL_3;
    }
    dwFlags = 2;
  }
  status = NtSetInformationProcess((HANDLE)0xFFFFFFFF, (PROCESSINFOCLASS)0x22u, &dwFlags, 4u);
  if ( !NT_SUCCESS(status))
  {
    goto ERROR;
  }
  return TRUE;
}