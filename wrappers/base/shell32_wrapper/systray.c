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
#include <windef.h>
#include <pidl.h>
 
WINE_DEFAULT_DEBUG_CHANNEL(shell);

typedef enum _ASSOCCLASS{
	ASSOCCLASS_APP_KEY,
	ASSOCCLASS_CLSID_KEY,
	ASSOCCLASS_CLSID_STR,
	ASSOCCLASS_PROGID_KEY,
	ASSOCCLASS_SHELL_KEY,
	ASSOCCLASS_PROGID_STR,
	ASSOCCLASS_SYSTEM_STR,
	ASSOCCLASS_APP_STR,
	ASSOCCLASS_FOLDER,
	ASSOCCLASS_STAR,
	ASSOCCLASS_FIXED_PROGID_STR,
	ASSOCCLASS_PROTOCOL_STR,
}ASSOCCLASS;

typedef enum tagSCNRT_STATUS { 
  SCNRT_ENABLE   = 0,
  SCNRT_DISABLE  = 1
} SCNRT_STATUS;

typedef struct _ASSOCIATIONELEMENT {
  ASSOCCLASS ac;
  HKEY       hkClass;
  PCWSTR     pszClass;
} ASSOCIATIONELEMENT;

typedef enum _ASSOC_FILTER{
  ASSOC_FILTER_NONE,
  ASSOC_FILTER_RECOMMENDED
}ASSOC_FILTER;

HRESULT WINAPI SHGetPropertyStoreForWindow(
  _In_   HWND hwnd,
  _In_   REFIID riid,
  _Out_  void **ppv
)
{
	return E_FAIL;
}

static const GUID CLSID_Bind_Query = 
{0x000214e6, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

HRESULT WINAPI SHBindToFolderIDListParent(
  _In_opt_   IShellFolder *psfRoot,
  _In_       PCUIDLIST_RELATIVE pidl,
  _In_       REFIID riid,
  _Out_      void **ppv,
  _Out_opt_  PCUITEMID_CHILD *ppidlLast
)
{
	 ppv = NULL;
	 return E_FAIL;
}

EXTERN_C HANDLE WINAPI IsEnabled()
{
	return NULL;
}

HRESULT WINAPI SHCreateDataObject(
  _In_opt_  PCIDLIST_ABSOLUTE pidlFolder,
  _In_      UINT cidl,
  _In_opt_  PCUITEMID_CHILD_ARRAY apidl,
  _In_opt_  IDataObject *pdtInner,
  _In_      REFIID riid,
  _Out_     void **ppv
)
{
	return E_FAIL;
}

//@unimplemented
HRESULT 
WINAPI 
SHEvaluateSystemCommandTemplate(
  _In_       PCWSTR pszCmdTemplate,
  _Out_      PWSTR *ppszApplication,
  _Out_opt_  PWSTR *ppszCommandLine,
  _Out_opt_  PWSTR *ppszParameters
)
{
	return E_FAIL;
}


HRESULT WINAPI SHCreateDefaultExtractIcon(
  REFIID riid,
  _Out_  void **ppv
)
{
	return E_FAIL;
}

HRESULT 
WINAPI 
SHSetTemporaryPropertyForItem(
  _In_  IShellItem *psi,
  _In_  REFPROPERTYKEY propkey,
  _In_  REFPROPVARIANT propvar
)
{
	return E_FAIL;
}

HRESULT 
WINAPI 
SHGetLocalizedName(
  _In_   PCWSTR pszPath,
  _Out_  PWSTR pszResModule,
  UINT cch,
  _Out_  int *pidsRes
)
{
	return E_FAIL;
}


HRESULT 
WINAPI 
ILLoadFromStreamEx(
  _In_   IStream *pstm,
  _Out_  PITEMID_CHILD *ppidl
)
{
	return E_FAIL;
}

BOOL 
WINAPI 
InitNetworkAddressControl(void)
{
	return FALSE;
}

HRESULT WINAPI SHCreateItemInKnownFolder(
  _In_      REFKNOWNFOLDERID kfid,
  DWORD dwKFFlags,
  _In_opt_  PCWSTR pszItem,
  _In_      REFIID riid,
  _Out_     void **ppv
)
{
	return E_FAIL;
}

HRESULT WINAPI SHGetTemporaryPropertyForItem(
  _In_   IShellItem *psi,
  REFPROPERTYKEY pk,
  _Out_  PROPVARIANT *ppropvarInk
)
{
	return E_FAIL;
}

HRESULT WINAPI SHGetPropertyStoreFromIDList(
  _In_   PCIDLIST_ABSOLUTE pidl,
  _In_   GETPROPERTYSTOREFLAGS flags,
  _In_   REFIID riid,
  _Out_  void **ppv
)
{
	return E_FAIL;
}

HRESULT WINAPI AssocCreateForClasses(
  _In_   const ASSOCIATIONELEMENT *rgClasses,
  _In_   ULONG cClasses,
  _In_   REFIID riid,
  _Out_  void **ppv
)
{
	return E_FAIL;
}

HRESULT WINAPI AssocGetDetailsOfPropKey(
  _In_   IShellFolder *psf,
  _In_   PCUITEMID_CHILD pidl,
  _In_   PROPERTYKEY *pkey,
  _Out_  VARIANT *pv,
  _Out_  BOOL *pfFoundPropKey
)
{
	return E_FAIL;
}

HRESULT WINAPI SHAddDefaultPropertiesByExt(
  _In_  PCWSTR pszExt,
  _In_  IPropertyStore *pPropStore
)
{
	return E_FAIL;
}

HRESULT WINAPI SHRemoveLocalizedName(
  _In_  PCWSTR pszPath
)
{
	return E_FAIL;
}

HRESULT WINAPI SHCreateItemFromRelativeName(
  _In_   IShellItem *psiParent,
  _In_   PCWSTR pszName,
  _In_   IBindCtx *pbc,
  _In_   REFIID riid,
  _Out_  void **ppv
)
{
	return E_FAIL;
}

HRESULT 
WINAPI 
SHGetDriveMedia(
  _In_   PCWSTR pszDrive,
  _Out_  DWORD *pdwMediaContent
)
{
	return E_FAIL;
}

HRESULT WINAPI SHCreateAssociationRegistration(
  _In_   REFIID riid,
  _Out_  void **ppv
)
{
	return E_FAIL;
}

void WINAPI SHChangeNotifyRegisterThread(
  SCNRT_STATUS status
)
{
	;
}

HRESULT WINAPI SHCreateThreadUndoManager(int a1, int a2)
{
	return E_FAIL;	
}

HRESULT WINAPI SHGetThreadUndoManager(int a1, int a2)
{
	return E_FAIL;	
}

HRESULT 
WPC_InstallState(
	DWORD *pdwState
){
	*pdwState = 2;
	return S_OK;
}

HRESULT 
WINAPI
SetCurrentProcessExplicitAppUserModelID(
  _In_ PCWSTR AppID
)
{
	return E_NOTIMPL;
}

HRESULT 
WINAPI
GetCurrentProcessExplicitAppUserModelID(
  _Out_ PWSTR *AppID
)
{
	return E_NOTIMPL;
}

HRESULT 
WINAPI 
Shell_NotifyIconGetRect(
	const NOTIFYICONIDENTIFIER *identifier, 
	RECT *iconLocation
)
{
	return E_NOTIMPL;
}