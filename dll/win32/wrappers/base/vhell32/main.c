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

const char * shdebugstr_guid( const struct _GUID *id )
{
	int i;
	const char* name = NULL;
	char clsidbuf[100];

	if (!id) return "(null)";

	for (i = 0; InterfaceDesc[i].riid && !name; i++) {
	    if (IsEqualIID(InterfaceDesc[i].riid, id)) name = InterfaceDesc[i].name;
	}
	if (!name) {
	    if (HCR_GetClassNameA(id, clsidbuf, 100))
		name = clsidbuf;
	}

        return wine_dbg_sprintf( "%s (%s)", debugstr_guid(id), name ? name : "unknown" );
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

BOOL WINAPI Shell_NotifyIconInternal(PNOTIFYICONDATAW pNotificationData, DWORD dwMessage, __int16 dwOption)
{
  WCHAR *lpszClassName; // edi@1
  HWND localWindow; // eax@6
  HWND window; // ST28_4@13
  WNDCLASSW WndClass; // [sp+8h] [bp-28h]@13

  lpszClassName = (WCHAR *)malloc(0x10u);
  wsprintfW(lpszClassName, L"%x", pNotificationData->hWnd);
  if ( dwMessage )
  {
    // if ( dwMessage != 2 )
    // {
      // free(lpszClassName);
      // pNotificationData->hWnd = globalWindow;
      // if ( dwOption != 1 )
        // return Shell_NotifyIconW(dwMessage, pNotificationData);
      // return Shell_NotifyIconA(dwMessage, (PNOTIFYICONDATAA)pNotificationData);
    // }
    localWindow = FindWindowW(lpszClassName, 0);
    globalWindow = localWindow;
    if ( localWindow )
    {
      DestroyWindow(localWindow);
      localWindow = globalWindow;
    }
    pNotificationData->hWnd = localWindow;
    free(lpszClassName);
    dwMessage = 2;
  }
  else
  {
    if ( hWnd || lpPrevWndFunc )
    {
      free(lpszClassName);
      return 183;
    }
    window = pNotificationData->hWnd;
    WndClass.hInstance = (HINSTANCE)0x10000000;
    WndClass.lpszClassName = lpszClassName;
    WndClass.lpfnWndProc = NotificationWindowCallback;
    WndClass.style = 0;
    WndClass.hIcon = 0;
    WndClass.hCursor = 0;
    WndClass.lpszMenuName = 0;
    WndClass.hbrBackground = 0;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    lpPrevWndFunc = (WNDPROC)GetWindowLongW(window, -4);
    hWnd = pNotificationData->hWnd;
    RegisterClassW(&WndClass);
    globalWindow = CreateWindowExW(0, lpszClassName, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, (HINSTANCE)0x10000000, 0);
    pNotificationData->hWnd = globalWindow;
    free(lpszClassName);
    dwMessage = 0;
  }
  // if ( dwOption == 1 )
    // return Shell_NotifyIconA(dwMessage, (PNOTIFYICONDATAA)pNotificationData);
  //
  //return Shell_NotifyIconW(dwMessage, pNotificationData);
}

BOOL WINAPI Shell_NotifyIconInternalW(DWORD dwMessage, PNOTIFYICONDATAW lpData)
{
  return Shell_NotifyIconInternal(lpData, dwMessage, 2);
}