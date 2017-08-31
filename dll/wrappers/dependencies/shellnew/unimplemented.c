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

typedef enum  { 
  QUNS_NOT_PRESENT              = 1,
  QUNS_BUSY                     = 2,
  QUNS_RUNNING_D3D_FULL_SCREEN  = 3,
  QUNS_PRESENTATION_MODE        = 4,
  QUNS_ACCEPTS_NOTIFICATIONS    = 5,
  QUNS_QUIET_TIME               = 6,
  QUNS_APP                      = 7
} QUERY_USER_NOTIFICATION_STATE;

typedef enum tagSCNRT_STATUS { 
  SCNRT_ENABLE   = 0,
  SCNRT_DISABLE  = 1
} SCNRT_STATUS;

typedef struct _ASSOCIATIONELEMENT {
  ASSOCCLASS ac;
  HKEY       hkClass;
  PCWSTR     pszClass;
} ASSOCIATIONELEMENT;

HRESULT WINAPI SHCreateItemFromIDList(PCIDLIST_ABSOLUTE pidl, REFIID riid, void **ppv)
{
	return S_OK;
}

HRESULT WINAPI SHCreateItemFromParsingName(
	PCWSTR pszPath,
    IBindCtx *pbc, 
	REFIID riid, 
	void **ppv
)
{
	return S_OK;
}

HRESULT WINAPI SHChangeNotifyDeregisterWindow(WPARAM wParam)
{
	return S_OK;
}

HRESULT 
WINAPI 
SHBindToObject(
	IShellFolder *psf, 
	struct _ITEMIDLIST_RELATIVE *pidl, 
	IBindCtx *pbc, 
	const IID *const riid, 
	void **ppv
)
{
	return S_OK;
}

HRESULT WINAPI SHBindToFolderIDListParent(
  _In_opt_   IShellFolder *psfRoot,
  _In_       PCUIDLIST_RELATIVE pidl,
  _In_       REFIID riid,
  _Out_      void **ppv,
  _Out_opt_  PCUITEMID_CHILD *ppidlLast
)
{
	return S_OK;
}

BOOL WINAPI Shell_NotifyIconA(
  _In_ DWORD           dwMessage,
  _In_ PNOTIFYICONDATAA lpdata
)
{
	return TRUE;
}

BOOL WINAPI Shell_NotifyIconW(
  _In_ DWORD           dwMessage,
  _In_ PNOTIFYICONDATAW lpdata
)
{
	return TRUE;
}

HRESULT SHBindToFolderIDListParentEx(
  _In_opt_  IShellFolder       *psfRoot,
  _In_      PCUIDLIST_RELATIVE pidl,
  _In_opt_  IBindCtx           *ppbc,
  _In_      REFIID             riid,
  _Out_     void               **ppv,
  _Out_opt_ PCUITEMID_CHILD    *ppidlLast
)
{
	return S_OK;
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
SHCreateItemWithParent(
  _In_   PCIDLIST_ABSOLUTE pidlParent,
  _In_   IShellFolder *psfParent,
  _In_   PCUITEMID_CHILD pidl,
  _In_   REFIID riid,
  _Out_  void **ppvItem
)
{
	return S_OK;
}

HRESULT 
WINAPI 
SHCreateShellItemArray(
	PCIDLIST_ABSOLUTE pidlParent,
    IShellFolder *psf,
    UINT cidl,
    PCUITEMID_CHILD_ARRAY ppidl,
    IShellItemArray **ppsiItemArray
)
{
	return S_OK;
}

HRESULT 
WINAPI 
SHCreateShellItemArrayFromIDLists(
	UINT cidl,
    PCIDLIST_ABSOLUTE_ARRAY pidl_array,
    IShellItemArray **psia
)
{
	return S_OK;
}

int Shell_GetCachedImageIndexW(
  _In_ PCWSTR pwszIconPath,
       int    iIconIndex,
       UINT   uIconFlags
)
{
	return 0;
}

int Shell_GetCachedImageIndexA(
  _In_ PCSTR pwszIconPath,
       int    iIconIndex,
       UINT   uIconFlags
)
{
	return 0;
}

/*************************************************************************
 * SHGetKnownFolderPath           [SHELL32.@]
 */
HRESULT 
WINAPI 
SHGetKnownFolderPath(
	REFKNOWNFOLDERID id, 
	DWORD flags, 
	HANDLE token, 
	PWSTR *path
)
{
	return S_OK;
}

HRESULT 
WINAPI 
SHCreateShellItemArrayFromDataObject(IDataObject *pdo, REFIID riid, void **ppv)
{
	return S_OK;	
}

HRESULT WINAPI SHCreateShellItemArrayFromShellItem(IShellItem *item, REFIID riid, void **ppv)
{
	return S_OK;	
}

HRESULT WINAPI SHGetPropertyStoreFromParsingName(const WCHAR *path, IBindCtx *pbc, GETPROPERTYSTOREFLAGS flags,
    REFIID riid, void **ppv)
{
	return S_OK;
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
	return S_OK;
}

HRESULT WINAPI AssocCreateForClasses(
  _In_   const ASSOCIATIONELEMENT *rgClasses,
  _In_   ULONG cClasses,
  _In_   REFIID riid,
  _Out_  void **ppv
)
{
	return S_OK;
}

HRESULT WINAPI AssocGetDetailsOfPropKey(
  _In_   IShellFolder *psf,
  _In_   PCUITEMID_CHILD pidl,
  _In_   PROPERTYKEY *pkey,
  _Out_  VARIANT *pv,
  _Out_  BOOL *pfFoundPropKey
)
{
	return S_OK;
}

HRESULT WINAPI SHAssocEnumHandlers(
  _In_  PCWSTR             pszExtra,
  _In_  ASSOC_FILTER       afFilter,
  _Out_ IEnumAssocHandlers **ppEnumHandler
)
{
	return S_OK;
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

HRESULT WINAPI SHAddDefaultPropertiesByExt(
  _In_ PCWSTR         pszExt,
  _In_ IPropertyStore *pPropStore
)
{
	return S_OK;
}	

void WINAPI SHChangeNotifyRegisterThread(
   SCNRT_STATUS status
)
{
	;
}

HRESULT 
WINAPI 
SHGetNameFromIDList(
	PCIDLIST_ABSOLUTE pidl, 
	SIGDN sigdnName, 
	PWSTR *ppszName)
{
	return S_OK;
}	

HRESULT 
WINAPI 
SHGetIDListFromObject(
	IUnknown *punk, 
	PIDLIST_ABSOLUTE *ppidl
)
{
	return S_OK;
}	

BOOL InitNetworkAddressControl(void)
{
	return FALSE;
}

HRESULT SHCreateAssociationRegistration(
  _In_  REFIID riid,
  _Out_ void   **ppv
)
{
	return S_OK;
}	

HRESULT SHRemoveLocalizedName(
  _In_ PCWSTR pszPath
)
{
	return S_OK;
}	

HRESULT 
WINAPI 
SHGetDriveMedia(
  _In_   PCWSTR pszDrive,
  _Out_  DWORD *pdwMediaContent
)
{
	return S_OK;
}	

HRESULT WINAPI SHGetTemporaryPropertyForItem(
  _In_   IShellItem *psi,
  REFPROPERTYKEY pk,
  _Out_  PROPVARIANT *ppropvarInk
)
{
	return S_OK;
}	

HRESULT 
WINAPI 
SHEvaluateSystemCommandTemplate(
  _In_       PCWSTR pszCmdTemplate,
  _Out_      PWSTR *ppszApplication,
  _Out_opt_  PWSTR *ppszCommandLine,
  _Out_opt_  PWSTR *ppszParameters
)
{
	return S_OK;
}	

HRESULT WINAPI SHCreateDefaultExtractIcon(
  REFIID riid,
  _Out_  void **ppv
)
{
	return S_OK;
}	

HRESULT 
WINAPI 
SHSetTemporaryPropertyForItem(
  _In_  IShellItem *psi,
  _In_  REFPROPERTYKEY propkey,
  _In_  REFPROPVARIANT propvar
)
{
	return S_OK;
}

HRESULT WINAPI SHGetPropertyStoreFromIDList(
  _In_   PCIDLIST_ABSOLUTE pidl,
  _In_   GETPROPERTYSTOREFLAGS flags,
  _In_   REFIID riid,
  _Out_  void **ppv
)
{
	return S_OK;
}

HRESULT WINAPI SHQueryUserNotificationState(QUERY_USER_NOTIFICATION_STATE *pquns)
{
	return S_OK;
}

HRESULT 
WINAPI 
SHCreateDefaultContextMenu(const DEFCONTEXTMENU *pdcm, REFIID riid, void **ppv)
{
	return S_OK;
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
	return S_OK;
}

HRESULT 
WINAPI 
ILLoadFromStreamEx(
  _In_   IStream *pstm,
  _Out_  PITEMID_CHILD *ppidl
)
{
	return S_OK;
}

HRESULT WINAPI SHCreateItemInKnownFolder(
  _In_      REFKNOWNFOLDERID kfid,
  DWORD dwKFFlags,
  _In_opt_  PCWSTR pszItem,
  _In_      REFIID riid,
  _Out_     void **ppv
)
{
	return S_OK;
}

BOOL 
WINAPI 
SHGetPathFromIDListEx(
  _In_   PCIDLIST_ABSOLUTE pidl,
  _Out_  PWSTR pszPath,
  DWORD cchPath,
  GPFIDL_FLAGS uOpts
)
{
	return S_OK;
}

HRESULT WINAPI SHCreateThreadUndoManager(PVOID a1, PVOID a2)
{
	return E_FAIL;	
}

HRESULT WINAPI SHGetThreadUndoManager(int a1, int a2)
{
	return E_FAIL;	
}

HRESULT 
WINAPI 
PathComparePaths(
	LPWSTR external, 
	LPCWSTR lpString
)
{
	return S_OK;
}