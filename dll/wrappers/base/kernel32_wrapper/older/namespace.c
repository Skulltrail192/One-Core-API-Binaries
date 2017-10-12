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

HANDLE BasePrefixLastHint = NULL;

HANDLE BasePrefixList = NULL;

PRTL_SRWLOCK BasePrivatenamespaceLock;

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

HANDLE WINAPI OpenPrivateNamespaceW(LPVOID lpBoundaryDescriptor, LPCWSTR lpAliasPrefix)
{
  NTSTATUS otherStatus; // eax@1
  NTSTATUS status; // [sp-4h] [bp-Ch]@2
  PUNICODE_STRING HUM = NULL; // [sp+0h] [bp-8h]@1

  otherStatus = RtlInitUnicodeStringEx(HUM, lpAliasPrefix);
  if ( otherStatus < 0
    || (otherStatus = NtOpenPrivateNamespace(&lpAliasPrefix, 33554432, 0, lpBoundaryDescriptor), otherStatus < 0) )
  {
    status = otherStatus;
LAST_ERROR:
    BaseSetLastNTError(status);
    return 0;
  }
  if ( !1 )
  {
    NtClose((HANDLE)lpAliasPrefix);
    status = STATUS_DUPLICATE_NAME;
    goto LAST_ERROR;
  }
  return (HANDLE)lpAliasPrefix;
}

HANDLE WINAPI OpenPrivateNamespaceA(LPVOID lpBoundaryDescriptor, LPCTSTR lpAliasPrefix)
{
  PUNICODE_STRING string; // esi@2
  NTSTATUS status; // eax@2
  NTSTATUS otherStatus; // [sp-4h] [bp-10h]@3
  PCANSI_STRING SourceString = NULL; // [sp+4h] [bp-8h]@2

  if ( !lpAliasPrefix )
  {
    otherStatus = STATUS_INVALID_PARAMETER;
    goto LAST_ERROR;
  }
  
  #ifdef _M_IX86
	string = (PUNICODE_STRING)(__readfsdword(24) + 3064);
  #elif defined(_M_AMD64)
	string = (PUNICODE_STRING)(__readgsqword(24) + 3064);
  #endif
  status = RtlInitAnsiStringEx(SourceString, lpAliasPrefix);
  if ( status < 0 )
    goto RECEIVE_STATUS;
  status = RtlAnsiStringToUnicodeString(string, SourceString, 0);
  if ( status >= 0 )
    return OpenPrivateNamespaceW(lpBoundaryDescriptor, string->Buffer);
  if ( status != STATUS_BUFFER_OVERFLOW )
  {
RECEIVE_STATUS:
    otherStatus = status;
LAST_ERROR:
    BaseSetLastNTError(otherStatus);
    return NULL;
  }
  SetLastError(0xCEu);
  return NULL;
}


HANDLE WINAPI BasepFindPrefixAliasByHandle(HANDLE base)
{
  HANDLE result; // eax@1

  result = (HANDLE)BasePrefixLastHint;
  if ( !BasePrefixLastHint || BasePrefixLastHint )
  {
    for ( result = (HANDLE)BasePrefixList; ; result = *(HANDLE *)result )
    {
      if ( result == &BasePrefixList )
        return 0;
      if ( *((HANDLE *)result + 4) == base )
        break;
    }
    BasePrefixLastHint = result;
  }
  return result;
}

BOOLEAN WINAPI ClosePrivateNamespace(HANDLE Handle, ULONG flags)
{
  PVOID otherAddress; // eax@1
  PVOID address; // edi@1
  BOOLEAN result; // al@3

  RtlAcquireSRWLockExclusive(BasePrivatenamespaceLock);
  otherAddress = (PVOID)BasepFindPrefixAliasByHandle(Handle);
  address = otherAddress;
  if ( otherAddress )
  {
    if ( flags & 1 )
    {
      if ( otherAddress )
        NtDeletePrivateNamespace(Handle);
    }
    NtClose(Handle);
    RtlReleaseSRWLockExclusive(BasePrivatenamespaceLock);
    result = TRUE;
  }
  else
  {
    RtlReleaseSRWLockExclusive(BasePrivatenamespaceLock);
    BaseSetLastNTError(STATUS_INVALID_HANDLE);
    result = FALSE;
  }
  return result;
}

HANDLE WINAPI CreatePrivateNamespaceW(LPSECURITY_ATTRIBUTES lpPrivateNamespaceAttributes, LPVOID lpBoundaryDescriptor, HANDLE Handle)
{
  NTSTATUS otherStatus; // eax@1
  NTSTATUS status; // [sp-4h] [bp-24h]@9
  POBJECT_ATTRIBUTES object = NULL; // [sp+0h] [bp-20h]@4
  PUNICODE_STRING string = NULL; // [sp+18h] [bp-8h]@1

  otherStatus = RtlInitUnicodeStringEx(string, Handle);
  if ( otherStatus < 0 )
    goto LABEL_15;
  if ( lpPrivateNamespaceAttributes && lpPrivateNamespaceAttributes->bInheritHandle )
  {
    status = STATUS_INVALID_PARAMETER;
    goto LABEL_10;
  }
  otherStatus = (NTSTATUS)BaseFormatObjectAttributes(object, lpPrivateNamespaceAttributes, 0);
  if ( otherStatus < 0
    || (otherStatus = NtCreatePrivateNamespace(&Handle, 33554432, (POBJECT_ATTRIBUTES)lpPrivateNamespaceAttributes, lpBoundaryDescriptor),
        otherStatus < 0) )
  {
LABEL_15:
    status = otherStatus;
  }
  else
  {
    if ( 1 )
      return Handle;
    NtDeletePrivateNamespace(Handle);
    NtClose(Handle);
    status = STATUS_DUPLICATE_NAME;
  }
LABEL_10:
  BaseSetLastNTError(status);
  return NULL;
}

HANDLE WINAPI CreatePrivateNamespaceA(LPSECURITY_ATTRIBUTES lpPrivateNamespaceAttributes, LPVOID lpBoundaryDescriptor, HANDLE SourceString)
{
  PUNICODE_STRING string; // esi@2
  NTSTATUS status; // eax@2
  NTSTATUS error; // [sp-4h] [bp-10h]@5
  STRING DestinationString; // [sp+4h] [bp-8h]@2

  if ( !SourceString )
  {
    error = STATUS_INVALID_PARAMETER;
    goto Error_Final;
  }
  #ifdef _M_IX86
	string = (PUNICODE_STRING)(__readfsdword(24) + 3064);
  #elif defined(_M_AMD64)
	string = (PUNICODE_STRING)(__readgsqword(24) + 3064);
  #endif  
  RtlInitAnsiString(&DestinationString, SourceString);
  status = RtlAnsiStringToUnicodeString(string, &DestinationString, 0);
  if ( status >= 0 )
    return CreatePrivateNamespaceW(lpPrivateNamespaceAttributes, lpBoundaryDescriptor, string->Buffer);
  if ( status != STATUS_BUFFER_OVERFLOW )
  {
    error = status;
Error_Final:
    BaseSetLastNTError(error);
    return 0;
  }
  SetLastError(0xCEu);
  return 0;
}