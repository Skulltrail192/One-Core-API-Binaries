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
#include <shlwapi_undoc.h>
 
WINE_DEFAULT_DEBUG_CHANNEL(shell);

typedef enum  { 
  QUNS_NOT_PRESENT              = 1,
  QUNS_BUSY                     = 2,
  QUNS_RUNNING_D3D_FULL_SCREEN  = 3,
  QUNS_PRESENTATION_MODE        = 4,
  QUNS_ACCEPTS_NOTIFICATIONS    = 5,
  QUNS_QUIET_TIME               = 6,
  QUNS_APP                      = 7
} QUERY_USER_NOTIFICATION_STATE;

BOOL WINAPI WaitForExplorerRestartW(int one, int two, LPCWSTR string, int other)
{
  HANDLE handle; // eax@1
  void *closeHandle; // edi@1
  BOOL result; // eax@3
  struct _STARTUPINFOW StartupInfo; // [sp+Ch] [bp-264h]@3
  struct _PROCESS_INFORMATION ProcessInformation; // [sp+50h] [bp-220h]@3
  WCHAR CommandLine; // [sp+60h] [bp-210h]@1
  char Dst; // [sp+62h] [bp-20Eh]@1

  CommandLine = 0;
  memset(&Dst, 0, 0x208u);
  handle = OpenMutexW(0x100000u, 0, L"RestartExplorerMutex");
  closeHandle = handle;
  if ( handle )
  {
    SHWaitForSendMessageThread(handle, -1);
    CloseHandle(closeHandle);
  }
  memset(&StartupInfo, 0, 0x44u);
  ProcessInformation.hProcess = 0;
  ProcessInformation.hThread = 0;
  ProcessInformation.dwProcessId = 0;
  ProcessInformation.dwThreadId = 0;
  StartupInfo.cb = 68;
  StringCchCopyW(&CommandLine, 0x105u, string);
  result = CreateProcessW(0, &CommandLine, 0, 0, 0, 0, 0, 0, &StartupInfo, &ProcessInformation);
  if ( result )
  {
    CloseHandle(ProcessInformation.hProcess);
    result = CloseHandle(ProcessInformation.hThread);
  }
  return result;
}

//unimplemented
DWORD WINAPI SHInitializeNamespace()
{
	return 0;
}

//unimplemented
LPVOID WINAPI SHUninitializeNamespace()
{
	return NULL;
}

HRESULT WINAPI SHQueryUserNotificationState(QUERY_USER_NOTIFICATION_STATE *pquns)
{
  HRESULT resp; // ebx@1
  HWND window; // eax@2
  ULONG_PTR dwResult; // [sp+8h] [bp-8h]@3

  resp = 0x80070057u;
  if ( pquns )
  {
    resp = 0;
    *pquns = 2;
    window = FindWindowW(L"Shell_TrayWnd", 0);
    if ( window )
    {
      if ( SendMessageTimeoutW(window, 0x4EFu, 0, 0, 3u, 0x1B58u, &dwResult) )
        *pquns = dwResult;
    }
  }
  return resp;
}

HRESULT 
WINAPI 
SHBindToFolderIDListParentEx(
  _In_opt_   IShellFolder *psfRoot,
  _In_       PCUIDLIST_RELATIVE pidl,
  _In_opt_   IBindCtx *ppbc,
  _In_       REFIID riid,
  _Out_      void **ppv,
  _Out_opt_  PCUITEMID_CHILD *ppidlLast
)
{
	return S_OK;
	//return SHBindToFolderIDListParent(psfRoot, pidl, riid, ppv, ppidlLast);
}

HRESULT WINAPI SHFormatForDisplay(
  _In_   REFPROPERTYKEY propkey,
  _In_   REFPROPVARIANT propvar,
  _In_   PROPDESC_FORMAT_FLAGS pdfFlags,
  _Out_  LPWSTR pwszText,
  _In_   DWORD cchText
)
{
	return PSFormatForDisplay(propkey, propvar, pdfFlags, pwszText, cchText);
}
