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
 
WINE_DEFAULT_DEBUG_CHANNEL(comctl32);

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    TRACE("fdwReason %u\n", fdwReason);

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}

int WINAPI SnapIconSize(int a1)
{
  int result; // eax@1
  unsigned int v2; // ecx@1
  int v3; // [sp+0h] [bp-14h]@1
  int v4; // [sp+4h] [bp-10h]@1
  int v5; // [sp+8h] [bp-Ch]@1
  int v6; // [sp+Ch] [bp-8h]@1

  result = a1;
  v3 = 16;
  v4 = 32;
  v5 = 48;
  v6 = 256;
  v2 = 0;
  while ( a1 > (unsigned int)*(&v3 + v2) )
  {
    ++v2;
    if ( v2 >= 4 )
      return result;
  }
  return *(&v3 + v2);
}

HRESULT WINAPI LoadIconWithScaleDown(HINSTANCE hInst, LPCWSTR name, int a3, int cy, HICON *a5)
{
  HICON *localIcon; // edi@1
  int snap; // eax@2
  HANDLE oneHandle; // eax@6
  HANDLE twoHandle; // eax@10
  HANDLE Threehandle; // eax@16
  HICON otherIcon; // ebx@16
  int load; // [sp+Ch] [bp-Ch]@2
  int iconSize; // [sp+10h] [bp-8h]@2
  int error; // [sp+30h] [bp+18h]@1

  localIcon = a5;
  *a5 = 0;
  error = 0x80004005u;
  if ( name )
  {
    iconSize = SnapIconSize(a3);
    snap = SnapIconSize(cy);
    load = snap;
    if ( iconSize != a3 || snap != cy )
    {
      twoHandle = LoadImageW(hInst, name, 1u, a3, cy, 0x10000u);
      *localIcon = twoHandle;
      if ( twoHandle )
      {
        error = 0;
      }
      else
      {
        Threehandle = LoadImageW(hInst, name, 1u, iconSize, load, 0x10000u);
        otherIcon = Threehandle;
        if ( Threehandle )
        {
          //error = GetIconSize(Threehandle, a3, cy, (int)localIcon);
          DestroyIcon(otherIcon);
        }
      }
    }
    if ( !*localIcon )
    {
      if ( a3 != GetSystemMetrics(11) || cy != GetSystemMetrics(12) )
        oneHandle = LoadImageW(hInst, name, 1u, a3, cy, 0);
      else
        oneHandle = LoadIconW(hInst, name);
      *localIcon = oneHandle;
      if ( oneHandle )
        error = 0;
      else
        error = GetLastError();
    }
  }
  else
  {
    error = 0x80070057u;
  }
  return error;
}

HRESULT WINAPI LoadIconMetric(HINSTANCE hInst, LPCWSTR name, int lims, HICON *phico)
{
  HICON *icon; // ebx@1
  HRESULT result; // eax@1
  int metrics; // eax@2
  int otherMetrics; // [sp+1Ch] [bp+14h]@2

  icon = phico;
  *phico = 0;
  result = 0x80070057u;
  if ( (unsigned int)lims <= 1 )
  {
    otherMetrics = GetSystemMetrics(lims != 1 ? 49 : 11);
    metrics = GetSystemMetrics(lims != 1 ? 50 : 12);
    result = LoadIconWithScaleDown(hInst, name, otherMetrics, metrics, icon);
  }
  return result;
}

HANDLE WINAPI PremultiplyAlphaChannel(HANDLE handle, int a2)
{
  HANDLE v2; // edi@1
  BOOL pv; // [sp+4h] [bp-1Ch]@2
  int v5; // [sp+8h] [bp-18h]@5
  int v6; // [sp+Ch] [bp-14h]@5
  __int16 v7; // [sp+16h] [bp-Ah]@4
  int v8; // [sp+18h] [bp-8h]@3

  v2 = CopyImage(handle, 0, 0, 0, 0x2000u);
  return v2;
}

int WINAPI TestWF(HWND hWnd, __int16 a2)
{
  LONG numberLong; // eax@1
  int result; // eax@2

  numberLong = GetWindowLongW(hWnd, -1);
  if ( numberLong )
    result = (numberLong + 4 * (HIBYTE(a2) >> 2)) & (a2 << 8 * (HIBYTE(a2) & 3));
  else
    result = 0;
  return result;
}

HWND WINAPI SizeBoxHwnd(HWND hWnd)
{
  HWND handle; // ebx@1
  BOOL test; // esi@1
  BOOL comparation; // edi@1
  LONG receive; // esi@1
  LONG left; // eax@10
  LONG otherBottom; // [sp+Ch] [bp-20h]@12
  LONG bottom; // [sp+10h] [bp-1Ch]@3
  struct tagRECT Rect; // [sp+18h] [bp-14h]@1

  handle = hWnd;
  test = TestWF(hWnd, 2624) != 0;
  comparation = test ^ TestWF(hWnd, 2368) != 0;
  GetWindowRect(hWnd, &Rect);
  receive = Rect.left;
  if ( !comparation )
    receive = Rect.right;
  bottom = Rect.bottom;
  while ( handle )
  {
    if ( TestWF(handle, 3588) )
    {
      if ( TestWF(handle, 3841) )
        return 0;
      GetClientRect(handle, &Rect);
      MapWindowPoints(handle, 0, (LPPOINT)&Rect, 2u);
      left = Rect.left;
      if ( !comparation )
        left = Rect.right;
      otherBottom = Rect.bottom;
      if ( comparation )
      {
        if ( receive - GetSystemMetrics(45) > left )
          return 0;
      }
      else
      {
        if ( receive + GetSystemMetrics(45) < left )
          return 0;
      }
      if ( bottom + GetSystemMetrics(46) >= otherBottom )
        return handle;
      return 0;
    }
    if ( !TestWF(handle, 3904) || TestWF(handle, 8) )
      return 0;
    handle = GetParent(handle);
  }
  return 0;
}

/***********************************************************************
 * TaskDialogIndirect [COMCTL32.@]
 */
HRESULT WINAPI TaskDialogIndirect(const TASKDIALOGCONFIG *pTaskConfig, int *pnButton,
                                  int *pnRadioButton, BOOL *pfVerificationFlagChecked)
{
    UINT uType = 0;
    INT  ret;
    FIXME("%p, %p, %p, %p\n", pTaskConfig, pnButton, pnRadioButton, pfVerificationFlagChecked);

    if (pTaskConfig->dwCommonButtons & TDCBF_YES_BUTTON &&
        pTaskConfig->dwCommonButtons & TDCBF_NO_BUTTON &&
        pTaskConfig->dwCommonButtons & TDCBF_CANCEL_BUTTON)
        uType |= MB_YESNOCANCEL;
    else
    if (pTaskConfig->dwCommonButtons & TDCBF_YES_BUTTON &&
        pTaskConfig->dwCommonButtons & TDCBF_NO_BUTTON)
        uType |= MB_YESNO;
    else
    if (pTaskConfig->dwCommonButtons & TDCBF_RETRY_BUTTON &&
        pTaskConfig->dwCommonButtons & TDCBF_CANCEL_BUTTON)
        uType |= MB_RETRYCANCEL;
    else
    if (pTaskConfig->dwCommonButtons & TDCBF_OK_BUTTON &&
        pTaskConfig->dwCommonButtons & TDCBF_CANCEL_BUTTON)
        uType |= MB_OKCANCEL;
    else
    if (pTaskConfig->dwCommonButtons & TDCBF_OK_BUTTON)
        uType |= MB_OK;
    ret = MessageBoxW(pTaskConfig->hwndParent, pTaskConfig->pszMainInstruction,
                      pTaskConfig->pszWindowTitle, uType);
    FIXME("dwCommonButtons=%x uType=%x ret=%x\n", pTaskConfig->dwCommonButtons, uType, ret);

    if (pnButton) *pnButton = ret;
    if (pnRadioButton) *pnRadioButton = pTaskConfig->nDefaultButton;
    if (pfVerificationFlagChecked) *pfVerificationFlagChecked = TRUE;
    return S_OK;
}

HRESULT TaskDialog(
  _In_   HWND hWndParent,
  _In_   HINSTANCE hInstance,
  _In_   PCWSTR pszWindowTitle,
  _In_   PCWSTR pszMainInstruction,
  _In_   PCWSTR pszContent,
  _In_   TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons,
  _In_   PCWSTR pszIcon,
  _Out_  int *pnButton
)
{
	return S_OK;
}