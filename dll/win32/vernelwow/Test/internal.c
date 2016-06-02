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
 
#include <main.h>

BOOL bIsFileApiAnsi;

/*
No copy/conversion is done if the dest. buffer is too small.

Returns:
   Success: number of TCHARS copied into dest. buffer NOT including nullterm
   Fail: size of buffer in TCHARS required to hold the converted filename, including nullterm
*/
DWORD
WINAPI
FilenameU2A_FitOrFail(
   LPSTR  DestA,
   INT destLen, /* buffer size in TCHARS incl. nullchar */
   PUNICODE_STRING SourceU
   )
{
   DWORD ret;

   /* destLen should never exceed MAX_PATH */
   if (destLen > MAX_PATH) destLen = MAX_PATH;

   ret = bIsFileApiAnsi? RtlUnicodeStringToAnsiSize(SourceU) : RtlUnicodeStringToOemSize(SourceU);
   /* ret incl. nullchar */

   if (DestA && (INT)ret <= destLen)
   {
      ANSI_STRING str;

      str.Buffer = DestA;
      str.MaximumLength = (USHORT)destLen;


      if (bIsFileApiAnsi)
         RtlUnicodeStringToAnsiString(&str, SourceU, FALSE );
      else
         RtlUnicodeStringToOemString(&str, SourceU, FALSE );

      ret = str.Length;  /* SUCCESS: length without terminating 0 */
   }

   return ret;
}

/*
DWORD WINAPI BaseVerifyUnicodeString(PUNICODE_STRING string)
{
  PWSTR Buffer; // edx@1
  USHORT lenght; // cx@5

  Buffer = string->Buffer;
  if ( Buffer )
  {
    lenght = string->MaximumLength;
    if ( lenght && string->Length <= lenght && !(lenght & 1) && !(string->Length & 1) )
      return (unsigned __int8)CsrVerifyRegion(Buffer, lenght) != 0 ? 0 : -1073741790;
  }
  else
  {
    if ( !string->MaximumLength && !string->Length )
      return 0;
  }
  return 0xC0000022u;
}*/