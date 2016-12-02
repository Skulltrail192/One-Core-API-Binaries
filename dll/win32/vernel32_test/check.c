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

/*
* @unimplemented
*/
DWORD WINAPI CheckElevationEnabled(
    BOOL *pResult
) 
{
	*pResult = FALSE;
	return 1;
}

/*
* @unimplemented
*/
DWORD WINAPI CheckElevation(
   LPCWSTR lpApplicationName,
   PDWORD pdwFlags,
   HANDLE hChildToken,
   PDWORD pdwRunLevel,
   PDWORD pdwReason
)
{
	return 1;
}


BOOL WINAPI CheckForReadOnlyResource(DWORD flags, BOOL other)
{
  DWORD *otherNumber = 0; // eax@2
  MEMORY_INFORMATION_CLASS MemoryInformation; // [sp+10h] [bp-54h]@5
  PVOID ImageBase = NULL; // [sp+14h] [bp-50h]@8
  DWORD hum = 0; // [sp+24h] [bp-40h]@6
  DWORD compare = 0; // [sp+28h] [bp-3Ch]@7
  ULONG OldProtect; // [sp+2Ch] [bp-38h]@17
  ULONG ProtectSize; // [sp+30h] [bp-34h]@17
  ULONG Size; // [sp+34h] [bp-30h]@8
  PVOID image; // [sp+38h] [bp-2Ch]@8
  BOOL verification; // [sp+40h] [bp-24h]@6
  int unneed; // [sp+44h] [bp-20h]@8
  PVOID BaseAddress; // [sp+48h] [bp-1Ch]@5
  CPPEH_RECORD ms_exc; // [sp+4Ch] [bp-18h]@8
  BOOL flagsa; // [sp+6Ch] [bp+8h]@7

  if ( !(other & 0xFFFFFFFE) )
  {
    *otherNumber = flags;
    if ( flags == STATUS_ACCESS_VIOLATION )
    {
      if ( otherNumber[5] == 1 )
      {
        if ( otherNumber[4] >= 2 )
        {
          BaseAddress = (PVOID)otherNumber[6];
          if ( NtQueryVirtualMemory((HANDLE)0xFFFFFFFF, BaseAddress, (MEMORY_INFORMATION_CLASS)0, &MemoryInformation, 0x1Cu, 0) >= 0 )
          {
            verification = 0;
            if ( hum != 2 )
              return verification;
            flagsa = 0;
            if ( compare == 0x1000000 )
            {
              ms_exc.registration.TryLevel = 0;
              image = RtlImageDirectoryEntryToData(ImageBase, 1u, 2u, &Size);
              unneed = 0;
              ms_exc.registration.TryLevel = -2;
              if ( !image || BaseAddress < image || BaseAddress >= (PVOID)image )
                goto LABEL_16;
            }
            else
            {
              if ( compare != 262144 || !(other & 1) )
              {
LABEL_16:
                if ( flagsa )
                {
                  ProtectSize = 1;
                  if ( NtProtectVirtualMemory((HANDLE)0xFFFFFFFF, &BaseAddress, (SIZE_T*)ProtectSize, 8u, &OldProtect) >= 0 )
                    verification = -1;
                }
                return verification;
              }
            }
            flagsa = 1;
            goto LABEL_16;
          }
        }
      }
    }
  }
  return 0;
}


BOOL WINAPI CheckForReadOnlyResourceFilter(DWORD access)
{
  BOOL result; // eax@2

  if ( BasepAllowResourceConversion )
    result = CheckForReadOnlyResource(access, 0);
  else
    result = 0;
  return result;
}

BOOL 
WINAPI 
CheckTokenMembershipEx(HANDLE HANDLE, PSID SidToCheck, DWORD Flags, PBOOL IsMember)
{
  PBOOL otherMember; // esi@2
  NTSTATUS status; // eax@3
  BOOL result; // eax@4
  NTSTATUS error; // [sp-4h] [bp-8h]@7

  if ( !SidToCheck || (otherMember = IsMember) == 0 )
  {
    error = STATUS_INVALID_PARAMETER;
    goto Default_error;
  }
  *IsMember = 0;
  status = RtlCheckTokenMembershipEx(HANDLE, SidToCheck, Flags, IsMember);
  if ( status < 0 )
  {
    error = status;
Default_error:
    BaseSetLastNTError(error);
    return 0;
  }
  result = 1;
  if (IsMember )
    *otherMember = 1;
  return result;
}

BOOL 
WINAPI 
CheckTokenCapability(
	HANDLE TokenHandle, 
	PSID CapabilitySidToCheck, 
	PBOOL HasCapability
)
{
  PBOOL verification; // esi@2
  NTSTATUS resp; // eax@3
  BOOL result; // eax@4
  NTSTATUS error; // [sp-4h] [bp-8h]@7

  if ( !CapabilitySidToCheck || (verification = HasCapability) == 0 )
  {
    error = STATUS_INVALID_PARAMETER;
    goto LABEL_9;
  }
  *HasCapability = 0;
  resp = RtlCheckTokenCapability(TokenHandle, CapabilitySidToCheck, HasCapability);
  if ( resp < 0 )
  {
    error = resp;
LABEL_9:
    BaseSetLastNTError(error);
    return FALSE;
  }
  result = TRUE;
  if ( HasCapability )
    *verification = 1;
  return result;
}

BOOL 
WINAPI 
CheckAllowDecryptedRemoteDestinationPolicy()
{
  ULONG ResultLength; // [sp+0h] [bp-20h]@3
  HANDLE Handle; // [sp+4h] [bp-1Ch]@2
  PVOID KeyValueInformation; // [sp+8h] [bp-18h]@3
  int four = 0; // [sp+Ch] [bp-14h]@4
  int five = 0; // [sp+10h] [bp-10h]@5
  int six = 0; // [sp+14h] [bp-Ch]@6
  OBJECT_ATTRIBUTES ObjectCopy;
  LSA_UNICODE_STRING CopyFile;
  BOOL result;
  BOOL verification = FALSE;
  
  CopyFile.Buffer = L"CopyFileAllowDecryptedRemoteDestination";
  if ( !verification )
  {
    if ( NtOpenKey(&Handle, 1u, &ObjectCopy) >= 0 )
    {
      if ( NtQueryValueKey(Handle, &CopyFile, KeyValuePartialInformation, &KeyValueInformation, 0x14u, &ResultLength) >= 0
        && four == 4
        && five == 4
        && six == 1 )
        result = 1;
      NtClose(Handle);
    }
    verification = 1;
  }
  return result;
}