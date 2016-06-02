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
 * @implemented - need test
 */
BOOL WINAPI GetNamedPipeAttribute(HANDLE Pipe, PIPE_ATTRIBUTE_TYPE AttributeType, PSTR AttributeName, PVOID AttributeValue, PSIZE_T AttributeValueLength)
{
  ULONG resp; // edx@3
  NTSTATUS status; // eax@4
  BOOL result; // eax@5
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+0h] [bp-8h]@4

  if ( AttributeType )
  {
    if ( AttributeType == 1 )
    {
      resp = 0x110030u;
    }
    else
    {
      if ( AttributeType != 2 )
      {
        SetLastError(0x57u);
        return 0;
      }
      resp = 1114168;
    }
  }
  else
  {
    resp = 1114152;
  }
  status = NtFsControlFile(
             Pipe,
             NULL,
             NULL,
             NULL,
             &IoStatusBlock,
             resp,
             AttributeName,
             strlen(AttributeName) + 1,
             AttributeValue,
             *AttributeValueLength);
  if ( status >= 0 )
  {
    *AttributeValueLength = IoStatusBlock.Information;
    result = 1;
  }
  else
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  return result;
}

BOOL WINAPI GetNamedPipeServerSessionId(HANDLE Pipe, PULONG ServerSessionId)
{
  UINT_PTR AttributeValueLength; // [sp+0h] [bp-4h]@1

  AttributeValueLength = 4;
  return GetNamedPipeAttribute(Pipe, 0, "ServerSessionId", ServerSessionId, &AttributeValueLength);
}

BOOL WINAPI GetNamedPipeClientProcessId(HANDLE Pipe, PULONG ClientProcessId)
{
  UINT_PTR AttributeValueLength; // [sp+0h] [bp-4h]@1

  AttributeValueLength = 4;
  return GetNamedPipeAttribute(Pipe, PipeConnectionAttribute, "ClientProcessId", ClientProcessId, &AttributeValueLength);
}

BOOL WINAPI GetNamedPipeServerProcessId(
  _In_   HANDLE Pipe,
  _Out_  PULONG ServerProcessId
)
{
  UINT_PTR AttributeValueLength; // [sp+0h] [bp-4h]@1

  AttributeValueLength = 4;
  return GetNamedPipeAttribute(Pipe, PipeConnectionAttribute, "ServerProcessId", ServerProcessId, &AttributeValueLength);
}


BOOL WINAPI GetNamedPipeClientSessionId(
  _In_   HANDLE Pipe,
  _Out_  PULONG ClientSessionId
)
{
  UINT_PTR AttributeValueLength; // [sp+0h] [bp-4h]@1

  AttributeValueLength = 4;
  return GetNamedPipeAttribute(Pipe, PipeConnectionAttribute, "ClientSessionId", ClientSessionId, &AttributeValueLength);	
}

BOOL WINAPI GetNamedPipeClientComputerNameW(HANDLE Pipe, LPWSTR ClientComputerName, ULONG ClientComputerNameLength)
{
  BOOL resp; // esi@1

  resp = GetNamedPipeAttribute(
           Pipe,
           PipeConnectionAttribute,
           "ClientComputerName",
           ClientComputerName,
           &ClientComputerNameLength);
  if ( !resp && GetLastError() == 1168 )
    SetLastError(0xE5u);
  return resp;
}

BOOL WINAPI GetNamedPipeClientComputerNameA(HANDLE Pipe, LPSTR ClientComputerName, ULONG ClientComputerNameLength)
{
	PUNICODE_STRING string;
	string = Basep8BitStringToStaticUnicodeString(ClientComputerName);
	return GetNamedPipeClientComputerNameW(Pipe, string->Buffer, ClientComputerNameLength);
}

BOOL WINAPI SetNamedPipeAttribute(HANDLE Pipe, PIPE_ATTRIBUTE_TYPE AttributeType, PSTR AttributeName, PVOID AttributeValue, SIZE_T AttributeValueLength)
{
  BOOL result; // eax@4
  SIZE_T size; // ebx@8
  size_t localSize; // esi@8
  PVOID alloc; // eax@8
  PVOID other; // edi@8
  ULONG number; // eax@10
  NTSTATUS status; // eax@12
  BOOL verification = FALSE; // esi@13
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+0h] [bp-8h]@12
  ULONG AttributeTypea; // [sp+14h] [bp+Ch]@5
  ULONG AttributeValueLengtha; // [sp+20h] [bp+18h]@8

  if ( AttributeType )
  {
    if ( AttributeType == 1 )
    {
      AttributeTypea = 0x110034u;
    }
    else
    {
      if ( AttributeType != 2 )
      {
        SetLastError(0x57u);
        return 0;
      }
      AttributeTypea = 0x11003Cu;
    }
  }
  else
  {
    AttributeTypea = 0x11002Cu;
  }
  size = AttributeValueLength;
  localSize = strlen(AttributeName) + 1;
  AttributeValueLengtha = localSize + AttributeValueLength;
  #ifdef _M_IX86
		alloc = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, AttributeValueLengtha);
  #elif defined(_M_AMD64)
		alloc = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, AttributeValueLengtha);
  #endif    
    result = verification;
  other = alloc;
  if ( alloc )
  {
    memcpy(alloc, AttributeName, localSize);
    number = 0;
    if ( size )
    {
      memcpy((char *)other + localSize, AttributeValue, size);
      number = 0;
    }
    status = NtFsControlFile(
               Pipe,
               (HANDLE)number,
               (PIO_APC_ROUTINE)number,
               (PVOID)number,
               &IoStatusBlock,
               AttributeTypea,
               other,
               AttributeValueLengtha,
               (PVOID)number,
               number);
    if ( status >= 0 )
    {
      verification = TRUE;
    }
    else
    {
      BaseSetLastNTError(status);
      verification = FALSE;
    }
	#ifdef _M_IX86
		RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, other);
	#elif defined(_M_AMD64)
		RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, other);
	#endif    
    result = verification;
  }
  else
  {
    SetLastError(8u);
    result = 0;
  }
  return result;
}