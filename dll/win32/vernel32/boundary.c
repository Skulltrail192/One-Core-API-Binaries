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

BOOL 
WINAPI 
AddIntegrityLabelToBoundaryDescriptor(
	HANDLE *BoundaryDescriptor, 
	PSID IntegrityLabel)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = RtlAddIntegrityLabelToBoundaryDescriptor(BoundaryDescriptor, IntegrityLabel);
  if ( status < 0 )
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

HANDLE 
WINAPI 
CreateBoundaryDescriptorW(
	LPCWSTR Name, 
	ULONG Flags)
{
  NTSTATUS status; // eax@1
  HANDLE result; // eax@2
  LSA_UNICODE_STRING *string = NULL; // [sp+0h] [bp-8h]@1

  status = RtlInitUnicodeStringEx(string, Name);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  else
  {
    result = RtlCreateBoundaryDescriptor(string, Flags);
  }
  return result;
}

HANDLE 
WINAPI 
CreateBoundaryDescriptorA(
	LPCTSTR Name, 
	ULONG Flags
)
{
  LSA_UNICODE_STRING *string; // esi@2
  NTSTATUS status; // eax@2
  NTSTATUS otherStatus; // [sp-4h] [bp-10h]@3
  STRING SourceString; // [sp+4h] [bp-8h]@2

  if ( !Name )
  {
    otherStatus = STATUS_INVALID_PARAMETER;
    goto LABEL_9;
  }
  
  #ifdef _M_IX86
	string = (LSA_UNICODE_STRING *)(__readfsdword(24) + 3064);
  #elif defined(_M_AMD64)
	string = (LSA_UNICODE_STRING *)(__readgsqword(24) + 3064);
  #endif
  status = RtlInitAnsiStringEx(&SourceString, Name);
  if ( status < 0 )
    goto LABEL_13;
  status = RtlAnsiStringToUnicodeString(string, &SourceString, 0);
  if ( status >= 0 )
    return RtlCreateBoundaryDescriptor(string, Flags);
  if ( status != STATUS_BUFFER_OVERFLOW )
  {
LABEL_13:
    otherStatus = status;
LABEL_9:
    BaseSetLastNTError(otherStatus);
    return 0;
  }
  SetLastError(0xCEu);
  return 0;
}

BOOL WINAPI AddSIDToBoundaryDescriptor(HANDLE *BoundaryDescriptor, PSID RequiredSid)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = RtlAddSIDToBoundaryDescriptor(BoundaryDescriptor, RequiredSid);
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