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
#include <debug.h>

ATOM g_aCC32Subclass;

const WCHAR c_szCC32Subclass;

BOOL globalVerification = FALSE;

COLORREF g_clrWindow;

COLORREF g_clrWindowText;

COLORREF g_clrWindowFrame;

COLORREF g_clrGrayText;

COLORREF g_clrBtnText;

COLORREF g_clrBtnFace;

COLORREF g_clrBtnShadow;

COLORREF g_clrBtnHighlight;

COLORREF g_clrHighlight;

COLORREF g_clrHighlightText;

COLORREF g_clrInfoText;

COLORREF g_clrInfoBk;

COLORREF g_clr3DDkShadow;

COLORREF g_clr3DLight;

COLORREF g_clrMenuHilight;

COLORREF g_clrMenuText;

HBRUSH g_hbrGrayText;

HBRUSH g_hbrWindow;

COLORREF g_hbrWindowText;

COLORREF g_hbrWindowFrame;

HGDIOBJ g_hbrBtnFace;

HGDIOBJ g_hbrBtnHighlight;

HGDIOBJ g_hbrBtnShadow;

HBRUSH g_hbrHighlight;

HGDIOBJ g_hfontSystem;

HGDIOBJ g_hbrMenuHilight;

HGDIOBJ g_hbrMenuText;
 
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

BOOL WINAPI FixupEnumChildWindowProc(HWND hWnd, LPCWSTR lpString)
{
  HANDLE handle; // eax@1

  handle = RemovePropW(hWnd, (LPCWSTR)(unsigned __int16)g_aCC32Subclass);
  if ( handle )
    SetPropW(hWnd, lpString, handle);
  return 1;
}

BOOL WINAPI FixupEnumWindowProc(DWORD this, HWND hWndParent, LPARAM lParam)
{
  DWORD dwProcessId; // [sp+0h] [bp-4h]@1

  dwProcessId = this;
  if ( GetWindowThreadProcessId(hWndParent, &dwProcessId) && dwProcessId == GetCurrentProcessId() )
  {
    FixupEnumChildWindowProc(hWndParent, (LPCWSTR)lParam);
    EnumChildWindows(hWndParent, FixupEnumChildWindowProc, lParam);
  }
  return TRUE;
}

BOOL WINAPI FixupEnumDesktopProc(LPCWSTR lpszDesktop, LPARAM lParam)
{
  HDESK desk; // ebx@1
  DWORD number; // eax@2
  HDESK otherDesk; // edi@2
  BOOL resp; // [sp+4h] [bp-4h]@1

  resp = FALSE;
  desk = OpenDesktopW(lpszDesktop, 0, 0, 0xC1u);
  if ( desk )
  {
    number = GetCurrentThreadId();
    otherDesk = GetThreadDesktop(number);
    if ( otherDesk && SetThreadDesktop(desk) )
    {
      EnumWindows((WNDENUMPROC)FixupEnumWindowProc, lParam);
      SetThreadDesktop(otherDesk);
      resp = TRUE;
    }
    CloseDesktop(desk);
  }
  return resp;
}

void WINAPI FixupSubclassRecordsAfterLogoff()
{
  ATOM atom; // ax@2
  ATOM otherAtom; // di@2
  LPARAM param; // ST08_4@3
  HWINSTA insta; // eax@3

  if ( g_aCC32Subclass )
  {
    atom = GlobalAddAtomW(&c_szCC32Subclass);
    otherAtom = atom;
    if ( atom != g_aCC32Subclass )
    {
      param = atom;
      insta = GetProcessWindowStation();
      if ( !EnumDesktopsW(insta, (DESKTOPENUMPROCW)FixupEnumDesktopProc, param) )
        EnumWindows((WNDENUMPROC)FixupEnumWindowProc, otherAtom);
      g_aCC32Subclass = otherAtom;
    }
  }
}

int WINAPI GetBorderSelectColor(int value, DWORD dWord)
{
  int compose; // ecx@1

  compose = 0;
  *(WORD *)((char *)&compose + 1) = (unsigned __int8)((dWord >> 16)
                                                     + value * (255 - (unsigned __int8)(dWord >> 16)) / 100);
  compose = dWord + value * (255 - dWord) / 100;
  return (compose << 8) | (unsigned __int8)(dWord + value * (255 - (unsigned __int8)dWord) / 100);
}

HBRUSH WINAPI InitGlobalColors()
{
  DWORD ColorBorder; // eax@2
  DWORD systemColor; // eax@3
  HBRUSH otherBrush; // eax@7
  HBRUSH result; // eax@9
  int menuTextColor; // [sp-4h] [bp-10h]@2
  int colorBrush; // [sp-4h] [bp-10h]@7
  int pvParam; // [sp+8h] [bp-4h]@1

  pvParam = 0;
  g_clrWindow = GetSysColor(5);
  g_clrWindowText = GetSysColor(8);
  g_clrWindowFrame = GetSysColor(6);
  g_clrGrayText = GetSysColor(17);
  g_clrBtnText = GetSysColor(18);
  g_clrBtnFace = GetSysColor(15);
  g_clrBtnShadow = GetSysColor(16);
  g_clrBtnHighlight = GetSysColor(20);
  g_clrHighlight = GetSysColor(13);
  g_clrHighlightText = GetSysColor(14);
  g_clrInfoText = GetSysColor(23);
  g_clrInfoBk = GetSysColor(24);
  g_clr3DDkShadow = GetSysColor(21);
  g_clr3DLight = GetSysColor(22);
  SystemParametersInfoW(0x1022u, 0, &pvParam, 0);
  if ( pvParam )
  {
    ColorBorder = GetSysColor(29);
    menuTextColor = 7;
  }
  else
  {
    systemColor = GetSysColor(13);
    ColorBorder = GetBorderSelectColor(60, systemColor);
    menuTextColor = 14;
  }
  g_clrMenuHilight = ColorBorder;
  g_clrMenuText = GetSysColor(menuTextColor);
  g_hbrGrayText = GetSysColorBrush(17);
  g_hbrWindow = GetSysColorBrush(5);
  g_hbrWindowText = (int)GetSysColorBrush(8);
  g_hbrWindowFrame = (int)GetSysColorBrush(6);
  g_hbrBtnFace = GetSysColorBrush(15);
  g_hbrBtnHighlight = GetSysColorBrush(20);
  g_hbrBtnShadow = GetSysColorBrush(16);
  g_hbrHighlight = GetSysColorBrush(13);
  g_hfontSystem = GetStockObject(13);
  if ( globalVerification )
    DeleteObject(g_hbrMenuHilight);
  if ( pvParam )
  {
    globalVerification = 0;
    otherBrush = GetSysColorBrush(29);
    colorBrush = 7;
  }
  else
  {
    globalVerification = 1;
    otherBrush = CreateSolidBrush(g_clrMenuHilight);
    colorBrush = 14;
  }
  g_hbrMenuHilight = otherBrush;
  result = GetSysColorBrush(colorBrush);
  g_hbrMenuText = result;
  return result;
}

BOOL WINAPI InitForWinlogon(HINSTANCE a1, int a2)
{
  FixupSubclassRecordsAfterLogoff();
  //InitGlobalMetrics(0);
  InitGlobalColors();
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

HANDLE WINAPI PremultiplyAlphaChannel(HANDLE h, int second)
{
  HANDLE otherHandle; // edi@1

  otherHandle = CopyImage(h, 0, 0, 0, 0x2000u);
  return otherHandle;
}

/***********************************************************************
 * TaskDialogIndirect [COMCTL32.@]

HRESULT WINAPI TaskDialogIndirect(const TASKDIALOGCONFIG *pTaskConfig, int *pnButton,
                                  int *pnRadioButton, BOOL *pfVerificationFlagChecked)
{
    /*UINT uType = 0;
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
	MessageBox(
        NULL,
        L"Resource not available\nDo you want to try again?",
        L"Account Details",
        MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
    );
	return S_OK;
} */

typedef BOOL (WINAPI *DllEntryProc)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

PIMAGE_NT_HEADERS32 PEHeader;

DllEntryProc entry = (DllEntryProc)(baseAddress + PEHeader->OptionalHeader.AddressOfEntryPoint);
(*entry)((HINSTANCE)baseAddress, DLL_PROCESS_ATTACH, 0);