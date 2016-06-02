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

WINE_DEFAULT_DEBUG_CHANNEL(vser32);

UINT_PTR WINAPI SetCoalescableTimer(
  _In_opt_  HWND hwnd,
  _In_      UINT_PTR nIDEvent,
  _In_      UINT uElapse,
  _In_opt_  TIMERPROC lpTimerFunc,
  _In_      ULONG uToleranceDelay
)
{
	return 0;
}

BOOL WINAPI IsSETEnabled()
{
  LSTATUS status; // eax@1
  LSTATUS otherStatus; // eax@6
  BOOL hum; // eax@11
  BOOLEAN veritification; // zf@11
  BYTE query; // [sp+8h] [bp-130h]@16
  DWORD cbData; // [sp+Ch] [bp-12Ch]@6
  HKEY hKey; // [sp+10h] [bp-128h]@1
  BYTE Data[4]; // [sp+14h] [bp-124h]@1
  struct _OSVERSIONINFOW VersionInformation; // [sp+18h] [bp-120h]@3
  unsigned __int8 other = 0; // [sp+132h] [bp-6h]@4

  *Data = 0;
  status = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Policies\\Microsoft\\Windows NT\\Reliability", 0, 1u, &hKey);
  if ( !status )
  {
    cbData = 4;
    otherStatus = RegQueryValueExW(hKey, L"ShutdownReasonUI", 0, 0, Data, &cbData);
    if ( otherStatus )
    {
      if ( otherStatus == 2 )
      {
        if ( RegQueryValueExW(hKey, L"ShutdownReasonOn", 0, 0, &query, &cbData) == 2 )
        {
          VersionInformation.dwOSVersionInfoSize = 284;
          if ( GetVersionExW(&VersionInformation) )
          {
            *Data = 1;
            if ( other == 1 )
              *Data = 0;
          }
        }
      }
      goto LABEL_20;
    }
    if ( *Data && *Data != 1 )
    {
      VersionInformation.dwOSVersionInfoSize = 284;
      if ( !GetVersionExW(&VersionInformation) )
      {
LABEL_20:
        RegCloseKey(hKey);
        return *Data != 0;
      }
      if ( other == 1 )
      {
        hum = 0;
        veritification = *Data == 2;
      }
      else
      {
        hum = 0;
        veritification = *Data == 3;
      }
    }
    else
    {
      hum = 0;
      veritification = *Data == 1;
    }
    hum = veritification;
    *Data = hum;
    goto LABEL_20;
  }
  if ( status == 2 )
  {
    VersionInformation.dwOSVersionInfoSize = 284;
    if ( GetVersionExW(&VersionInformation) )
    {
      *Data = 1;
      if ( other == 1 )
        *Data = 0;
    }
  }
  return *Data != 0;
}