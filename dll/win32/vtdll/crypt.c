/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
 * Copyright 2015 Samuel Marins 
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
#include <main.h>

void WINAPI A_SHAInit(PSHA_CTX Context) 	
{
   /* SHA1 initialization constants */
   Context->State[0] = 0x67452301;
   Context->State[1] = 0xEFCDAB89;
   Context->State[2] = 0x98BADCFE;
   Context->State[3] = 0x10325476;
   Context->State[4] = 0xC3D2E1F0;
   Context->Count[0] =
   Context->Count[1] = 0;
}

VOID WINAPI A_SHAUpdate(PSHA_CTX Context,
		const unsigned char *  	Buffer,
		UINT  	BufferSize 
) 		
{
   ULONG BufferContentSize;

   BufferContentSize = Context->Count[1] & 63;
   Context->Count[1] += BufferSize;
   if (Context->Count[1] < BufferSize)
      Context->Count[0]++;
   Context->Count[0] += (BufferSize >> 29);

   if (BufferContentSize + BufferSize < 64)
   {
      RtlCopyMemory(&Context->Buffer[BufferContentSize], Buffer,
                    BufferSize);
   }
   else
   {
      while (BufferContentSize + BufferSize >= 64)
      {
         RtlCopyMemory(Context->Buffer + BufferContentSize, Buffer,
                       64 - BufferContentSize);
         Buffer += 64 - BufferContentSize;
         BufferSize -= 64 - BufferContentSize;
         SHA1Transform(Context->State, Context->Buffer);
         BufferContentSize = 0;
      }
      RtlCopyMemory(Context->Buffer + BufferContentSize, Buffer, BufferSize);
   }
}

VOID WINAPI A_SHAFinal(PSHA_CTX Context, PULONG Result) 		
{
   INT Pad, Index;
   UCHAR Buffer[72];
   ULONG *Count;
   ULONG BufferContentSize, LengthHi, LengthLo;

   BufferContentSize = Context->Count[1] & 63;
   if (BufferContentSize >= 56)
      Pad = 56 + 64 - BufferContentSize;
   else
      Pad = 56 - BufferContentSize;

   LengthHi = (Context->Count[0] << 3) | (Context->Count[1] >> (32 - 3));
   LengthLo = (Context->Count[1] << 3);

   RtlZeroMemory(Buffer + 1, Pad - 1);
   Buffer[0] = 0x80;
   Count = (ULONG*)(Buffer + Pad);
   Count[0] = DWORD2BE(LengthHi);
   Count[1] = DWORD2BE(LengthLo);
   A_SHAUpdate(Context, Buffer, Pad + 8);

   for (Index = 0; Index < 5; Index++)
      Result[Index] = DWORD2BE(Context->State[Index]);

   A_SHAInit(Context);
}