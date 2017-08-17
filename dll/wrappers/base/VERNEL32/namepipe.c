/*++

Copyright (c) 2017  Shorthorn Project

Module Name:

    namepipe.c

Abstract:

    This module contains the Win32 Named Pipe API

Author:

    Skulltrail 21-March-2017

Revision History:

--*/

#include "main.h"

BOOL 
WINAPI 
GetNamedPipeAttribute(
	HANDLE Pipe, 
	PIPE_ATTRIBUTE_TYPE AttributeType, 
	PSTR AttributeName, 
	PVOID AttributeValue, 
	PSIZE_T AttributeValueLength
)
{
  ULONG fscltCode; // edx@3
  NTSTATUS status; // eax@4
  BOOL result; // eax@5
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+0h] [bp-8h]@4

  if ( AttributeType )
  {
    if ( AttributeType == 1 )
    {
      fscltCode = 0x110030;
    }
    else
    {
      if ( AttributeType != 2 )
      {
        SetLastError(0x57u);
        return FALSE;
      }
      fscltCode = 0x110038;
    }
  }
  else
  {
    fscltCode = 0x110028;
  }
  status = NtFsControlFile(
             Pipe,
             0,
             0,
             0,
             &IoStatusBlock,
             fscltCode,
             AttributeName,
             strlen(AttributeName) + 1,
             AttributeValue,
             *AttributeValueLength);
  if ( NT_SUCCESS(status))
  {
    *AttributeValueLength = IoStatusBlock.Information;
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
GetNamedPipeServerSessionId(
	HANDLE Pipe, 
	PULONG ServerSessionId
)
{
  UINT_PTR AttributeValueLength; // [sp+0h] [bp-4h]@1

  AttributeValueLength = 4;
  return GetNamedPipeAttribute(Pipe, 0, "ServerSessionId", ServerSessionId, &AttributeValueLength);
}

BOOL 
WINAPI 
GetNamedPipeClientProcessId(
	HANDLE Pipe, 
	PULONG ClientProcessId
)
{
  UINT_PTR AttributeValueLength; // [sp+0h] [bp-4h]@1

  AttributeValueLength = 4;
  return GetNamedPipeAttribute(Pipe, PipeConnectionAttribute, "ClientProcessId", ClientProcessId, &AttributeValueLength);
}

BOOL 
WINAPI 
GetNamedPipeServerProcessId(
  _In_   HANDLE Pipe,
  _Out_  PULONG ServerProcessId
)
{
  UINT_PTR AttributeValueLength; // [sp+0h] [bp-4h]@1

  AttributeValueLength = 4;
  return GetNamedPipeAttribute(Pipe, PipeConnectionAttribute, "ServerProcessId", ServerProcessId, &AttributeValueLength);
}

BOOL 
WINAPI 
GetNamedPipeClientSessionId(
  _In_   HANDLE Pipe,
  _Out_  PULONG ClientSessionId
)
{
  UINT_PTR AttributeValueLength; // [sp+0h] [bp-4h]@1

  AttributeValueLength = 4;
  return GetNamedPipeAttribute(Pipe, PipeConnectionAttribute, "ClientSessionId", ClientSessionId, &AttributeValueLength);	
}

BOOL 
WINAPI 
GetNamedPipeClientComputerNameW(
	HANDLE Pipe, 
	LPWSTR ClientComputerName, 
	ULONG ClientComputerNameLength
)
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

BOOL 
WINAPI 
GetNamedPipeClientComputerNameA(
	HANDLE Pipe, 
	LPSTR ClientComputerName, 
	ULONG ClientComputerNameLength
)
{
	PUNICODE_STRING string;
	string = Basep8BitStringToStaticUnicodeString(ClientComputerName);
	return GetNamedPipeClientComputerNameW(Pipe, string->Buffer, ClientComputerNameLength);
}

BOOL 
WINAPI 
SetNamedPipeAttribute(
	HANDLE Pipe, 
	PIPE_ATTRIBUTE_TYPE AttributeType, 
	PSTR AttributeName, 
	PVOID AttributeValue, 
	SIZE_T AttributeValueLength
)
{
  BOOL result; // eax@4
  SIZE_T size; // ebx@8
  size_t localSize; // esi@8
  PVOID alloc; // eax@8
  PVOID other; // edi@8
  ULONG number; // eax@10
  NTSTATUS status; // eax@12
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+0h] [bp-8h]@12
  ULONG AttributeTypea; // [sp+14h] [bp+Ch]@5
  ULONG AttributeValueLengtha; // [sp+20h] [bp+18h]@8

  if ( AttributeType )
  {
    if ( AttributeType == 1 )
    {
      AttributeTypea = 0x110034;
    }
    else
    {
      if ( AttributeType != 2 )
      {
        SetLastError(0x57u);
        return FALSE;
      }
      AttributeTypea = 0x11003C;
    }
  }
  else
  {
    AttributeTypea = 0x11002C;
  }
  size = AttributeValueLength;
  localSize = strlen(AttributeName) + 1;
  AttributeValueLengtha = localSize + AttributeValueLength;

  alloc = RtlAllocateHeap(GetProcessHeap(), 0, AttributeValueLengtha);
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
      result = TRUE;
    }
    else
    {
      BaseSetLastNTError(status);
      result = FALSE;
    }
	RtlFreeHeap(GetProcessHeap(), 0, other);
  }
  else
  {
    SetLastError(8u);
    result = FALSE;
  }
  return result;
}