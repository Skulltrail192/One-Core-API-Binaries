/*
 * Copyright 2000 Juergen Schmied
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
 */

#include "precomp.h"

WINE_DEFAULT_DEBUG_CHANNEL(shell);

typedef HRESULT (WINAPI*PFNPRIVFUNC)(PVOID pCtx);

BOOL WINAPI DDEHandleViewFolderNotify(struct IShellBrowser *a1, HWND a2, HGLOBAL hMem)
{
	return TRUE;
}

HRESULT WINAPI SHInvokePrivilegedFunctionW(
    LPCWSTR pPriv,
    PFNPRIVFUNC pfnFunction,
    PVOID pCtx
) 
{
	return S_OK;
}

HRESULT WINAPI SHGetActiveConsoleSessionId()
{
	return S_OK;
}

BOOL WINAPI SHIsCurrentProcessConsoleSession()
{
	return TRUE;
}

HRESULT WINAPI DisconnectWindowsDialog(HLOCAL a1)
{
	return S_OK;
}

DWORD WINAPI SHCreateInstance(int a1, int a2, int a3)
{
	return 0;
}

BOOL WINAPI SHRegisterDarwinLink(LPITEMIDLIST pidl2, LPCWSTR psz, BOOL a3)
{
	return TRUE;
}

HRESULT WINAPI SHGetComputerDisplayNameW(unsigned __int16 *a1, int a2, WCHAR *a3, DWORD cchMax)
{
	return S_OK;
}

HRESULT WINAPI CheckStagingArea()
{
	return S_OK;
}

HRESULT WINAPI SHLimitInputEditWithFlags(HWND hWnd, int a2)
{
	return S_OK;
}

HRESULT WINAPI DeleteFileThumbnail(size_t *pcchNewDestLength)
{
	return S_OK;
}


HRESULT WINAPI SHGetAttributesFromDataObject(
  _In_opt_   IDataObject *pdo,
  DWORD dwAttributeMask,
  _Out_opt_  DWORD *pdwAttributes,
  _Out_opt_  UINT *pcItems
)
{
	return S_OK;
}

HRESULT WINAPI SHSimulateDropOnClsid(IUnknown *punk, IUnknown *punkSite, int a3)
{
	return S_OK;
}

HRESULT WINAPI SHIsLegacyAnsiProperty(const PVOID a1, int a2, int a3)
{
	return S_OK;
}

HMODULE WINAPI SHGetShellStyleHInstance()
{
	return NULL;
}

BOOL WINAPI SHCreateFileDataObject(LPCITEMIDLIST pidl, int a2, int a3, int a4, int a5)
{
	return TRUE;
}

HRESULT WINAPI SHFileSysBindToStorage(const WCHAR *a, int a3, int a4, int a5, int a6, int a7)
{
	return S_OK;
}

HRESULT WINAPI SHStgOpenStorageA(
  _In_   const CHAR *pwcsName,
  _In_   IStorage *pstgPriority,
  _In_   DWORD grfMode,
  _In_   SNB snbExclude,
  _In_   DWORD reserved,
  _Out_  IStorage **ppstgOpen
)
{
	return S_OK;
}

HRESULT WINAPI SHStgOpenStorageW(
  _In_   const WCHAR *pwcsName,
  _In_   IStorage *pstgPriority,
  _In_   DWORD grfMode,
  _In_   SNB snbExclude,
  _In_   DWORD reserved,
  _Out_  IStorage **ppstgOpen
)
{
	return S_OK;
}

HRESULT WINAPI Create_IEnumUICommand(struct IUnknown *a1, const struct WVTASKITEM *a2, ULONG a3, struct IEnumUICommand **a4)
{
	return S_OK;
}

HRESULT WINAPI Create_IUIElement(int a1, int a2)
{
	return S_OK;
}

LRESULT WINAPI SHEnableServiceObject(const struct _GUID *a1, BOOL a2)
{
	return S_OK;
}

HRESULT WINAPI SHEnumerateUnreadMailAccountsW(HKEY hKeyUser, DWORD dwIndex, LPWSTR pszMailAddress, int cchMailAddress)
{
	return S_OK;
}

BOOL WINAPI SHCreateLocalServerRunDll(int a1, int a2, LPCSTR a3, int a4)
{
	return TRUE;
}