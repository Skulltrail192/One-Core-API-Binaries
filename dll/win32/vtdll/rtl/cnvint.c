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
Copyright (c) 2015  Microsoft Corporation 
 
Module Name:

    cnvint.c
 
 */
 
#define NDEBUG

#include <main.h> 

BOOLEAN 
WINAPI 
RtlGetIntegerAtom(PWSTR AtomName, PUSHORT IntegerAtom)
{
  PWSTR string; // ecx@5
  WCHAR other; // dx@5
  PWSTR recept; // eax@5
  PWSTR unknown; // eax@8
  PUSHORT v7; // ecx@11
  PUNICODE_STRING receiveString; // [sp+0h] [bp-8h]@16
  PUNICODE_STRING stringReceive; // [sp+2h] [bp-6h]@16
  PWSTR obj; // [sp+4h] [bp-4h]@16

  if ( !((unsigned int)AtomName & 0xFFFF0000) )
  {
    unknown = AtomName;
    if ( (unsigned int)AtomName < 0xC000u )
    {
      if ( !AtomName )
        unknown = (PWSTR)0xC000u;
      v7 = IntegerAtom;
      if ( !IntegerAtom )
        return 1;
      goto LABEL_12;
    }
    return 0;
  }
  if ( *AtomName != 35 )
    return 0;
  string = AtomName + 1;
  other = AtomName[1];
  recept = AtomName + 1;
  if ( other )
  {
    while ( other >= 0x30u && other <= 0x39u )
    {
      ++recept;
      other = *recept;
      if ( !*recept )
        goto LABEL_16;
    }
    return 0;
  }
LABEL_16:
  AtomName = NULL;
  receiveString = (PUNICODE_STRING)recept;
  stringReceive = (PUNICODE_STRING)string;
  obj = string;
  if ( RtlUnicodeStringToInteger(receiveString, 10, (PULONG)AtomName) < 0 )
    return 0;
  v7 = IntegerAtom;
  if ( !IntegerAtom )
    return 1;
  unknown = AtomName;
  if ( !AtomName || (unsigned int)AtomName > 0xC000 )
  {
    *IntegerAtom = 0xC000u;
    return 1;
  }
LABEL_12:
  *v7 = (unsigned int)unknown;
  return 1;
}