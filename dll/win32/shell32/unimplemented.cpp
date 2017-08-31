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

HRESULT WINAPI SHGetIDListFromObject(
  _In_  IUnknown         *punk,
  _Out_ PIDLIST_ABSOLUTE *ppidl
)
{
	return S_OK;
}

HRESULT WINAPI SHCreateShellItemArray(
  _In_  PCIDLIST_ABSOLUTE     pidlParent,
  _In_  IShellFolder          *psf,
  _In_  UINT                  cidl,
  _In_  PCUITEMID_CHILD_ARRAY ppidl,
  _Out_ IShellItemArray       **ppsiItemArray
)
{
	return S_OK;
}

HRESULT WINAPI SHCreateShellItemArrayFromShellItem(
  _In_  IShellItem *psi,
  _In_  REFIID     riid,
  _Out_ void       **ppv
)
{
	return S_OK;
}