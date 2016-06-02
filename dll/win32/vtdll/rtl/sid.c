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

    sid.c
 
 */
 
#define NDEBUG

#include <main.h>

NTSTATUS 
NTAPI 
RtlCreateServiceSid(PUNICODE_STRING ServiceName,	PSID *ServiceSid, PULONG ServiceSidLength) 
{
  BOOL resp; // cf@3
  NTSTATUS result; // eax@4
  PVOID v5; // eax@7
  PUNICODE_STRING v6 = NULL; // [sp+8h] [bp-84h]@6
  PVOID Src = NULL; // [sp+Ch] [bp-80h]@7
  PSHA_CTX context = NULL; // [sp+10h] [bp-7Ch]@7
  PSID_IDENTIFIER_AUTHORITY identifier; // [sp+6Ch] [bp-20h]@1
  char v10; // [sp+6Dh] [bp-1Fh]@1
  char v11; // [sp+6Eh] [bp-1Eh]@1
  char v12; // [sp+6Fh] [bp-1Dh]@1
  char v13; // [sp+70h] [bp-1Ch]@1
  char v14; // [sp+71h] [bp-1Bh]@1
  PULONG third = NULL; // [sp+74h] [bp-18h]@7
  PVOID v16 = 0; // [sp+78h] [bp-14h]@7
  PVOID LangIDTemp = 0; // [sp+7Ch] [bp-10h]@7
  PVOID v18 = 0; // [sp+80h] [bp-Ch]@7
  PVOID v19 = 0; // [sp+84h] [bp-8h]@7

  identifier = 0;
  v10 = 0;
  v11 = 0;
  v12 = 0;
  v13 = 0;
  v14 = 5;
  if ( ServiceName && ServiceSidLength )
  {
    resp = *(DWORD *)ServiceSidLength < 0x20u;
    *(DWORD *)ServiceSidLength = 32;
    if ( resp )
    {
      result = STATUS_BUFFER_TOO_SMALL;
    }
    else
    {
      result = RtlUpcaseUnicodeString(v6, ServiceName, 1);
      if ( result >= 0 )
      {
        A_SHAInit(context);
        A_SHAUpdate(context, Src, (UINT)v6);
        A_SHAFinal(context, third);
        RtlFreeAnsiString((PANSI_STRING)v6);
        RtlInitializeSid(ServiceSid, identifier, 6);
        v5 = (PVOID)third;   
        ServiceSid[2] = (PSID)80;
        ServiceSid[3] = v5;
        ServiceSid[4] = v16;
        ServiceSid[5] = LangIDTemp;
        ServiceSid[6] = v18;
        ServiceSid[7] = v19;		
        result = 0;
      }
    }
  }
  else
  {
    result = STATUS_INVALID_PARAMETER;
  }
  return result;
}	