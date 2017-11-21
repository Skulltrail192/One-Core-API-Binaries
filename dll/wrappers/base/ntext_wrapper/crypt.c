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

static void SHA1Transform(ULONG State[5], UCHAR Buffer[64])
{
   ULONG a, b, c, d, e;
   ULONG *Block;

   Block = (ULONG*)Buffer;

   /* Copy Context->State[] to working variables */
   a = State[0];
   b = State[1];
   c = State[2];
   d = State[3];
   e = State[4];

   /* 4 rounds of 20 operations each. Loop unrolled. */
   R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
   R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
   R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
   R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
   R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
   R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
   R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
   R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
   R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
   R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
   R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
   R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
   R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
   R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
   R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
   R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
   R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
   R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
   R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
   R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

   /* Add the working variables back into Context->State[] */
   State[0] += a;
   State[1] += b;
   State[2] += c;
   State[3] += d;
   State[4] += e;

   /* Wipe variables */
   a = b = c = d = e = 0;
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