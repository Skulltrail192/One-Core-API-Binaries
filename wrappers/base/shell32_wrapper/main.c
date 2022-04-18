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
 
WINE_DEFAULT_DEBUG_CHANNEL(shell);

HINSTANCE shell32_hInstance = 0;

static const struct {
	REFIID riid;
	const char *name;
} InterfaceDesc[] = {
	{&IID_IUnknown,			"IID_IUnknown"},
	{&IID_IClassFactory,		"IID_IClassFactory"},
	{&IID_IShellView,		"IID_IShellView"},
	{&IID_IOleCommandTarget,	"IID_IOleCommandTarget"},
	{&IID_IDropTarget,		"IID_IDropTarget"},
	{&IID_IDropSource,		"IID_IDropSource"},
	{&IID_IViewObject,		"IID_IViewObject"},
	{&IID_IContextMenu,		"IID_IContextMenu"},
	{&IID_IShellExtInit,		"IID_IShellExtInit"},
	{&IID_IShellFolder,		"IID_IShellFolder"},
	{&IID_IShellFolder2,		"IID_IShellFolder2"},
	{&IID_IPersist,			"IID_IPersist"},
	{&IID_IPersistFolder,		"IID_IPersistFolder"},
	{&IID_IPersistFolder2,		"IID_IPersistFolder2"},
	{&IID_IPersistFolder3,		"IID_IPersistFolder3"},
	{&IID_IExtractIconA,		"IID_IExtractIconA"},
	{&IID_IExtractIconW,		"IID_IExtractIconW"},
	{&IID_IDataObject,		"IID_IDataObject"},
	{&IID_IAutoComplete,            "IID_IAutoComplete"},
	{&IID_IAutoComplete2,           "IID_IAutoComplete2"},
        {&IID_IShellLinkA,              "IID_IShellLinkA"},
        {&IID_IShellLinkW,              "IID_IShellLinkW"},
	{NULL,NULL}};

// BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
// {
    // TRACE("fdwReason %u\n", fdwReason);

    // switch(fdwReason)
    // {
        // case DLL_PROCESS_ATTACH:
            // DisableThreadLibraryCalls(hInstDLL);
            // break;
    // }

    // return TRUE;
// }

/************************************************************************/

BOOL WINAPI StrRetToStrNW(LPWSTR dest, DWORD len, LPSTRRET src, const ITEMIDLIST *pidl)
{

    if (!dest)
        return FALSE;

    switch (src->uType)
    {
        case STRRET_WSTR:
            lstrcpynW(dest, src->pOleStr, len);
            CoTaskMemFree(src->pOleStr);
            break;
        case STRRET_CSTR:
            if (!MultiByteToWideChar(CP_ACP, 0, src->cStr, -1, dest, len) && len)
                dest[len-1] = 0;
            break;
        case STRRET_OFFSET:
            if (!MultiByteToWideChar(CP_ACP, 0, ((LPCSTR)&pidl->mkid)+src->uOffset, -1, dest, len)
                    && len)
                dest[len-1] = 0;
            break;
        default:
            FIXME("unknown type %u!\n", src->uType);
            if (len)
                *dest = '\0';
            return FALSE;
    }
    return TRUE;
}

WNDPROC lpPrevWndFunc;

LRESULT 
WINAPI 
NotificationWindowCallback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  LRESULT result; // eax@2
  unsigned __int16 cursorPosition; // [sp+0h] [bp-Ch]@7 MAPDST

  if ( Msg == 1024 )
  {
    if ( lParam == 516 )
    {
      GetCursorPos((LPPOINT)&cursorPosition);
      result = CallWindowProcW(lpPrevWndFunc, hWnd, 0x400u, cursorPosition | (cursorPosition << 16), 123);
    }
    else if ( lParam == 517 )
    {
      result = 0;
    }
    else
    {
      result = CallWindowProcW(lpPrevWndFunc, hWnd, 0x400u, wParam, lParam);
    }
  }
  else
  {
    result = DefWindowProcW(hWnd, Msg, wParam, lParam);
  }
  return result;
}

HWND globalWindow;
HWND hWnd;

signed int  Internal_Shell_NotifyIcon(PNOTIFYICONDATA lpdata, DWORD dwMessage, __int16 flags)
{
  void *allocation; // edi@1
  HWND findWindow; // eax@7
  HWND otherWindow; // ST28_4@14
  DWORD localMessage; // [sp-8h] [bp-38h]@5
  struct _NOTIFYICONDATAA *lpDataPointer; // [sp-4h] [bp-34h]@3
  WNDCLASSW WndClass; // [sp+8h] [bp-28h]@14

  allocation = malloc(0x10u);
  wsprintfW((LPWSTR)allocation, L"%x", lpdata->hWnd);
  if ( dwMessage )
  {
    if ( dwMessage != 2 )
    {
      free(allocation);
      lpdata->hWnd = globalWindow;
      lpDataPointer = (struct _NOTIFYICONDATAA *)lpdata;
      if ( flags != 1 )
        return Shell_NotifyIconW(dwMessage, (PNOTIFYICONDATAW)lpdata);
      localMessage = dwMessage;
      return Shell_NotifyIconA(localMessage, lpDataPointer);
    }
    findWindow = FindWindowW((LPCWSTR)allocation, 0);
    globalWindow = findWindow;
    if ( findWindow )
    {
      DestroyWindow(findWindow);
      findWindow = globalWindow;
    }
    lpdata->hWnd = findWindow;
    free(allocation);
    lpDataPointer = (struct _NOTIFYICONDATAA *)lpdata;
    localMessage = 2;
  }
  else
  {
    if ( hWnd || lpPrevWndFunc )
    {
      free(allocation);
      return 183;
    }
    otherWindow = lpdata->hWnd;
    WndClass.hInstance = (HINSTANCE)268435456;
    WndClass.lpszClassName = (LPCWSTR)allocation;
    WndClass.lpfnWndProc = NotificationWindowCallback;
    WndClass.style = 0;
    WndClass.hIcon = 0;
    WndClass.hCursor = 0;
    WndClass.lpszMenuName = 0;
    WndClass.hbrBackground = 0;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    lpPrevWndFunc = (WNDPROC)GetWindowLongW(otherWindow, -4);
    hWnd = lpdata->hWnd;
    RegisterClassW(&WndClass);
    globalWindow = CreateWindowExW(0, (LPCWSTR)allocation, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, (HINSTANCE)0x10000000, 0);
    lpdata->hWnd = globalWindow;
    free(allocation);
    lpDataPointer = (struct _NOTIFYICONDATAA *)lpdata;
    localMessage = 0;
  }
  if ( flags == 1 )
    return Shell_NotifyIconA(localMessage, lpDataPointer);
  return Shell_NotifyIconW(localMessage, (PNOTIFYICONDATAW)lpDataPointer);
}

BOOL __stdcall Shell_NotifyIconInternal(DWORD dwMessage, PNOTIFYICONDATA lpdata)
{
  return Internal_Shell_NotifyIcon(lpdata, dwMessage, 0);
}

BOOL __stdcall Shell_NotifyIconInternalA(DWORD dwMessage, PNOTIFYICONDATAA lpData)
{
  return Internal_Shell_NotifyIcon((PNOTIFYICONDATA)lpData, dwMessage, 1);
}

BOOL __stdcall Shell_NotifyIconInternalW(DWORD dwMessage, PNOTIFYICONDATAW lpData)
{
  return Internal_Shell_NotifyIcon((PNOTIFYICONDATA)lpData, dwMessage, 2);
}