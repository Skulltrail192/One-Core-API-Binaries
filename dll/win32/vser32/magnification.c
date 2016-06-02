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

int WINAPI SetMagnificationDesktopMagnification(double integer, ULONG a2, signed int a3)
{
  double v4; // [sp+0h] [bp-20h]@1
  double v5; // [sp+8h] [bp-18h]@1
  double v6; // [sp+10h] [bp-10h]@1
  double v7; // [sp+18h] [bp-8h]@1

  v4 = integer;
  v5 = integer;
  v6 = (double)a2;
  v7 = (double)a3;
  return 1;
}

BOOL WINAPI SetMagnificationDesktopColorEffect(PMAGCOLOREFFECT pEffect)
{
	
	pEffectGlobal = pEffect;
	DbgPrint("SetMagnificationDesktopColorEffect is UNIMPLEMENTED\n"); 
	return TRUE;
}

BOOL WINAPI GetMagnificationDesktopMagnification(double *scale, int *x, int *y)
{
	DbgPrint("GetMagnificationDesktopMagnification is UNIMPLEMENTED\n"); 
	return TRUE;
}

PMAGCOLOREFFECT WINAPI GetMagnificationDesktopColorEffect(HANDLE hdw)
{
  int result; // eax@1
  float number; // [sp+0h] [bp-68h]@1
  char Dst; // [sp+4h] [bp-64h]@1
  float position; // [sp+18h] [bp-50h]@1
  float numberPosition; // [sp+30h] [bp-38h]@1
  float complex; // [sp+48h] [bp-20h]@1
  float hum; // [sp+60h] [bp-8h]@1
  int absolute; // [sp+64h] [bp-4h]@1

  memset(&Dst, 0, 0x60u);
  number = 1.0;
  position = 1.0;
  numberPosition = 1.0;
  complex = 1.0;
  hum = 1.0;
  absolute = 100;
  result = 1;
  if ( result )
  {
    if ( hdw )
      memcpy(hdw, &number, 0x64u);
  }
  return pEffectGlobal;
}