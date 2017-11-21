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

DWORD 
NTAPI
AlpcGetHeaderSize(
	DWORD Buffer
)
{
  DWORD result; // eax@1

  result = 8;
  if ( Buffer < 0 )
    result = 20;
  if ( Buffer & 0x40000000 )
    result += 16;
  if ( Buffer & 0x8000000 )
    result += 24;
  return result;
}

PVOID
NTAPI
AlpcGetMessageAttribute(
	PALPC_MESSAGE_ATTRIBUTES Buffer, 
	ULONG AttributeFlag
)
{
  PVOID result; // eax@3

  if ( !(AttributeFlag & Buffer->AllocatedAttributes) || (AttributeFlag - 1) & AttributeFlag )
    result = 0;
  else
    result = (char *)Buffer + AlpcGetHeaderSize(Buffer->AllocatedAttributes & ~(2 * AttributeFlag - 1));
  return result;
}

NTSTATUS 
NTAPI
AlpcInitializeMessageAttribute(
	ULONG AttributeFlags, 
	PALPC_MESSAGE_ATTRIBUTES Buffer, 
	DWORD BufferSize, 
	PULONG RequiredBufferSize
)
{
  DWORD HeaderSize; // eax@1
  NTSTATUS result; // eax@4

  HeaderSize = AlpcGetHeaderSize(AttributeFlags);
  *RequiredBufferSize = HeaderSize;
  if ( HeaderSize > BufferSize )
  {
    result = STATUS_BUFFER_TOO_SMALL;
  }
  else
  {
    if ( Buffer )
    {
      Buffer->ValidAttributes = 0;
      Buffer->AllocatedAttributes = AttributeFlags;
    }
    result = STATUS_SUCCESS;
  }
  return result;
}