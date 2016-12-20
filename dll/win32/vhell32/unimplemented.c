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

HRESULT WINAPI SHBindToObject(
  IShellFolder *psf,
  PCUIDLIST_RELATIVE pidl,
  _In_   IBindCtx *pbc,
  REFIID riid,
  _Out_  void **ppv
)
{
	 ppv = NULL;
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
HRESULT WINAPI SHEvaluateSystemCommandTemplate(
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
SHOpenWithDialog(
  _In_opt_  HWND hwnd,
  _In_ const OPENASINFO *poainfo
)
{
	LPCWSTR strCmd= L"shell32.dll,OpenAs_RunDLL ";
	StrCatW(strCmd, poainfo->pcszFile);

	ShellExecuteW(hwnd,
				  L"open", 
				  L"Rundll32.exe",
				  strCmd,
				  NULL,
				  SW_SHOW);

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

HRESULT WINAPI SHGetPropertyStoreFromParsingName(
  _In_      PCWSTR pszPath,
  _In_opt_  IBindCtx *pbc,
  _In_      GETPROPERTYSTOREFLAGS flags,
  _In_      REFIID riid,
  _Out_     void **ppv
)
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

// HRESULT 
// WINAPI 
// Shell_NotifyIconGetRect(
	// const NOTIFYICONIDENTIFIER *identifier, 
	// RECT *iconLocation
// )
HRESULT 
WINAPI 
Shell_NotifyIconGetRect(
	PVOID *identifier, 
	PVOID *iconLocation
)
{
	return E_NOTIMPL;
}
// {
  // char *unused1; // edi@3
  // HRESULT result; // eax@6 MAPDST
  // WPARAM v6; // eax@11
  // HWND v7; // ST08_4@14
  // HWND v8; // ST08_4@17
  // LONG rigth; // edi@18
  // LONG bottom; // ecx@18
  // signed int v12; // [sp+Ch] [bp-4Ch]@11
  // LPARAM lParam; // [sp+14h] [bp-44h]@11
  // int v15; // [sp+18h] [bp-40h]@11
  // int *v16; // [sp+1Ch] [bp-3Ch]@11
  // ULONG_PTR v17; // [sp+20h] [bp-38h]@11
  // BOOL pfEnabled; // [sp+24h] [bp-34h]@4
  // ULONG_PTR dwResult; // [sp+28h] [bp-30h]@6 MAPDST
  // int v20; // [sp+2Ch] [bp-2Ch]@11
  // int v21; // [sp+30h] [bp-28h]@11
  // int v22; // [sp+34h] [bp-24h]@11
  // int v23; // [sp+38h] [bp-20h]@11
  // WPARAM v24; // [sp+3Ch] [bp-1Ch]@11
  // UINT v25; // [sp+40h] [bp-18h]@11 MAPDST
  // DWORD v26; // [sp+44h] [bp-14h]@11
  // int v27; // [sp+48h] [bp-10h]@11
  // int v28; // [sp+4Ch] [bp-Ch]@11
  // int v29; // [sp+50h] [bp-8h]@11

  // result = 0x80004005;
  // if ( identifier && iconLocation )
  // {
    // iconLocation->left = 0;
    // iconLocation->top = 0;
    // unused1 = (char *)&iconLocation->right;
    // *(_DWORD *)unused1 = 0;
    // *((_DWORD *)unused1 + 1) = 0;
    // if ( !memcmp(&identifier->guidItem, &GUID_ShellNotifyTaskbarOffset, 0x10u) )
    // {
      // result = 0;
      // pfEnabled = 0;
      // if ( DwmIsCompositionEnabled(&pfEnabled) >= 0 )
      // {
        // if ( pfEnabled )
        // {
          // dwResult = 8;
          // SHLogicalToPhysicalDPI(0, (int *)&dwResult);
          // iconLocation->bottom = dwResult;
          // iconLocation->right = dwResult;
        // }
      // }
    // }
    // else
    // {
      // pfEnabled = (BOOL)FindWindowW(c_szTrayClass, 0);
      // if ( pfEnabled )
      // {
        // if ( identifier->cbSize == 28 )
        // {
          // memset(&v21, 0, 0x24u);
          // v25 = identifier->uID;
          // v6 = (WPARAM)identifier->hWnd;
          // v23 = 0;
          // v22 = 32;
          // v24 = v6;
          // v16 = &v20;
          // v26 = identifier->guidItem.Data1;
          // v27 = *(_DWORD *)&identifier->guidItem.Data2;
          // lParam = 3;
          // v28 = *(_DWORD *)&identifier->guidItem.Data4[0];
          // v29 = *(_DWORD *)&identifier->guidItem.Data4[4];
          // v20 = 880096291;
          // v15 = 40;
          // v21 = 2;
          // v12 = 0;
          // v17 = SendMessageTimeoutW((HWND)pfEnabled, 0x4Au, v6, (LPARAM)&lParam, 3u, 0x1B58u, &dwResult);
          // if ( v17 )
          // {
            // if ( dwResult == -1 && memcmp(&v26, &GUID_ShellNotifyChevron, 0x10u) )
            // {
              // v26 = GUID_ShellNotifyChevron.Data1;
              // v27 = *(_DWORD *)&GUID_ShellNotifyChevron.Data2;
              // v7 = identifier->hWnd;
              // v28 = *(_DWORD *)&GUID_ShellNotifyChevron.Data4[0];
              // v29 = *(_DWORD *)&GUID_ShellNotifyChevron.Data4[4];
              // v17 = SendMessageTimeoutW((HWND)pfEnabled, 0x4Au, (WPARAM)v7, (LPARAM)&lParam, 3u, 0x1B58u, &dwResult);
              // v12 = 1;
            // }
            // if ( v17 )
            // {
              // if ( dwResult )
              // {
                // v8 = identifier->hWnd;
                // v21 = 1;
                // if ( SendMessageTimeoutW((HWND)pfEnabled, 0x4Au, (WPARAM)v8, (LPARAM)&lParam, 3u, 0x1B58u, &v17) )
                // {
                  // dwResult = SHIWORD(v17);
                  // rigth = (signed __int16)v17 + (signed __int16)dwResult;
                  // bottom = SHIWORD(v17) + SHIWORD(dwResult);
                  // iconLocation->left = (signed __int16)v17;
                  // iconLocation->top = dwResult;
                  // iconLocation->right = rigth;
                  // iconLocation->bottom = bottom;
                  // result = v12 != 0;
                // }
              // }
            // }
          // }
        // }
        // else
        // {
          // result = 0x80070057;
        // }
      // }
    // }
  // }
  // else
  // {
    // result = 0x80004003;
  // }
  // return result;
// }