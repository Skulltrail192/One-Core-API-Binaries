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

typedef struct _NOTIFYICONIDENTIFIER {
  DWORD cbSize;
  HWND  hWnd;
  UINT  uID;
  GUID  guidItem;
} NOTIFYICONIDENTIFIER, *PNOTIFYICONIDENTIFIER;

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

/***********************************************************************
 *              SHCreateDataObject (SHELL32.@)
 */
HRESULT WINAPI SHCreateDataObject(PCIDLIST_ABSOLUTE pidl_folder, UINT count, PCUITEMID_CHILD_ARRAY pidl_array,
                                  IDataObject *object, REFIID riid, void **ppv)
{
    FIXME("%p %d %p %p %s %p: stub\n", pidl_folder, count, pidl_array, object, debugstr_guid(riid), ppv);
    return E_NOTIMPL;
}

//@unimplemented
HRESULT WINAPI SHEvaluateSystemCommandTemplate(
  _In_       PCWSTR pszCmdTemplate,
  _Out_      PWSTR *ppszApplication,
  _Out_opt_  PWSTR *ppszCommandLine,
  _Out_opt_  PWSTR *ppszParameters
)
{
    return E_NOTIMPL;
}

HRESULT WINAPI SHCreateDefaultExtractIcon(
  REFIID riid,
  _Out_  void **ppv
)
{
    return E_NOTIMPL;
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

/***********************************************************************
 *              SHGetLocalizedName (SHELL32.@)
 */
HRESULT WINAPI SHGetLocalizedName(LPCWSTR path, LPWSTR module, UINT size, INT *res)
{
    FIXME("%s %p %u %p: stub\n", debugstr_w(path), module, size, res);
    return E_NOTIMPL;
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

/***********************************************************************
 *              InitNetworkAddressControl (SHELL32.@)
 */
BOOL WINAPI InitNetworkAddressControl(void)
{
    FIXME("stub\n");
    return FALSE;
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

/*************************************************************************
 *              SHRemoveLocalizedName (SHELL32.@)
 */
HRESULT WINAPI SHRemoveLocalizedName(const WCHAR *path)
{
    FIXME("%s stub\n", debugstr_w(path));
    return S_OK;
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

/*************************************************************************
 * Shell_NotifyIconGetRect		[SHELL32.@]
 */
HRESULT WINAPI Shell_NotifyIconGetRect(const NOTIFYICONIDENTIFIER* identifier, RECT* icon_location)
{
    FIXME("stub (%p) (%p)\n", identifier, icon_location);
    return E_NOTIMPL;
}

HRESULT 
WINAPI 
StgMakeUniqueName(
  _In_  IStorage *pstgParent,
  _In_  PCWSTR   pszFileSpec,
  _In_  DWORD    grfMode,
  _In_  REFIID   riid,
  _Out_ void     **ppv
)
{
	return S_OK;
}

BOOL 
IsElevationRequired(LPCWSTR pszPath)
{
	return FALSE;
}

HRESULT WINAPI SHSetDefaultProperties(
  _In_opt_ HWND                       hwnd,
  _In_     IShellItem                 *psi,
           DWORD                      dwFileOpFlags,
  _In_opt_ IFileOperationProgressSink **pfops
)
{
	return S_OK;
}

HRESULT SHUserSetPasswordHint(LPCWSTR lpStart, LPCWSTR lpString)
{
	return S_OK;	
}

HRESULT
WINAPI
StampIconForElevation(HICON icon, int x, int y){
	return S_OK;
}