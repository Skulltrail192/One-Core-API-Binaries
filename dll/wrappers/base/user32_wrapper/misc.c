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

HANDLE section = NULL;

UINT_PTR 
WINAPI 
SetCoalescableTimer(
  _In_opt_  HWND hwnd,
  _In_      UINT_PTR nIDEvent,
  _In_      UINT uElapse,
  _In_opt_  TIMERPROC lpTimerFunc,
  _In_      ULONG uToleranceDelay
)
{
	return SetTimer(hwnd, nIDEvent, uElapse, lpTimerFunc);
}

BOOL WINAPI IsSETEnabled()
{
  LONG resp; // eax@1
  LONG query; // eax@6
  LPBYTE data; // eax@11
  BOOLEAN verification; // zf@11
  LPBYTE other; // [sp+8h] [bp-130h]@16
  DWORD cbData; // [sp+Ch] [bp-12Ch]@6
  HKEY hKey; // [sp+10h] [bp-128h]@1
  LPBYTE Data; // [sp+14h] [bp-124h]@1
  struct _OSVERSIONINFOW VersionInformation; // [sp+18h] [bp-120h]@3
  BOOLEAN mistery; // [sp+132h] [bp-6h]@4

  Data = 0;
  mistery = TRUE;
  resp = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Policies\\Microsoft\\Windows NT\\Reliability", 0, 1u, &hKey);
  if ( !resp )
  {
    cbData = 4;
    query = RegQueryValueExW(hKey, L"ShutdownReasonUI", 0, 0, (LPBYTE)&Data, &cbData);
    if ( query )
    {
      if ( query == 2 )
      {
        if ( RegQueryValueExW(hKey, L"ShutdownReasonOn", 0, 0, (LPBYTE)&other, &cbData) == 2 )
        {
          VersionInformation.dwOSVersionInfoSize = 284;
          if ( GetVersionExW(&VersionInformation) )
          {
            Data = (LPBYTE)1;
            if ( mistery == TRUE )
              Data = 0;
          }
        }
      }
      goto Close_Key;
    }
    if ( Data && Data != (LPBYTE)1 )
    {
      VersionInformation.dwOSVersionInfoSize = 284;
      if ( !GetVersionExW(&VersionInformation) )
      {
Close_Key:
        RegCloseKey(hKey);
        return Data != 0;
      }
      if ( mistery == TRUE )
      {
        data = 0;
        verification = Data == (LPBYTE)2;
      }
      else
      {
        data = 0;
        verification = Data == (LPBYTE)3;
      }
    }
    else
    {
      data = 0;
      verification = Data == (LPBYTE)1;
    }
    data = (LPBYTE)verification;
    Data = data;
    goto Close_Key;
  }
  if ( resp == 2 )
  {
    VersionInformation.dwOSVersionInfoSize = 284;
    if ( GetVersionExW(&VersionInformation) )
    {
      Data = (LPBYTE)1;
      if ( mistery == TRUE )
        Data = 0;
    }
  }
  return Data != 0;
}

BOOL WINAPI HangrepRegisterPort(HANDLE Port)
{
	return TRUE;	
}

BOOL WINAPI HangrepUnregisterPort(HANDLE Port)
{
	return TRUE;	
}

HANDLE WINAPI ThemeGetCurrentSection(HANDLE Port)
{
	return section;	
}

BOOL WINAPI ThemeSetCurrentSection(HANDLE Section)
{
	section = Section;
	return TRUE;
}