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

typedef struct _ShellItem2 {
    IShellItem2             IShellItem2_iface;
    LONG                    ref;
    LPITEMIDLIST            pidl;
    IPersistIDList          IPersistIDList_iface;
} ShellItem2;

typedef struct {
    REFIID riid;
    const char *name;
}Interface;

Interface InterfaceDesc[] = {
    {&IID_IUnknown,					"IID_IUnknown"},
    {&IID_IClassFactory,        "IID_IClassFactory"},
    {&IID_IShellView,        "IID_IShellView"},
    {&IID_IOleCommandTarget,    "IID_IOleCommandTarget"},
    {&IID_IDropTarget,        "IID_IDropTarget"},
    {&IID_IDropSource,        "IID_IDropSource"},
    {&IID_IViewObject,        "IID_IViewObject"},
    {&IID_IContextMenu,        "IID_IContextMenu"},
    {&IID_IShellExtInit,        "IID_IShellExtInit"},
    {&IID_IShellFolder,        "IID_IShellFolder"},
    {&IID_IShellFolder2,        "IID_IShellFolder2"},
    {&IID_IPersist,            "IID_IPersist"},
    {&IID_IPersistFolder,        "IID_IPersistFolder"},
    {&IID_IPersistFolder2,        "IID_IPersistFolder2"},
    {&IID_IPersistFolder3,        "IID_IPersistFolder3"},
    {&IID_IExtractIconA,        "IID_IExtractIconA"},
    {&IID_IExtractIconW,        "IID_IExtractIconW"},
    {&IID_IDataObject,        "IID_IDataObject"},
    {&IID_IAutoComplete,            "IID_IAutoComplete"},
    {&IID_IAutoComplete2,           "IID_IAutoComplete2"},
    {&IID_IShellLinkA,              "IID_IShellLinkA"},
    {&IID_IShellLinkW,              "IID_IShellLinkW"},
};

FORCEINLINE ULONG IShellItem_AddRef(IShellItem* This) {
    return This->lpVtbl->AddRef(This);
}

FORCEINLINE ULONG IShellItem_Release(IShellItem* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE ULONG IShellItemArray_Release(IShellItemArray* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE ULONG IShellFolder_AddRef(IShellFolder* This) {
    return This->lpVtbl->AddRef(This);
}

FORCEINLINE HRESULT IShellItem_GetAttributes(IShellItem* This,SFGAOF sfgaoMask,SFGAOF *psfgaoAttribs) {
    return This->lpVtbl->GetAttributes(This,sfgaoMask,psfgaoAttribs);
}

FORCEINLINE ULONG IShellItem2_AddRef(IShellItem2* This) {
    return This->lpVtbl->AddRef(This);
}

FORCEINLINE HRESULT IShellItemArray_GetItemAt(IShellItemArray* This,DWORD dwIndex,IShellItem **ppsi) {
    return This->lpVtbl->GetItemAt(This,dwIndex,ppsi);
}

FORCEINLINE ULONG IShellItemArray_AddRef(IShellItemArray* This) {
    return This->lpVtbl->AddRef(This);
}

FORCEINLINE HRESULT IShellItemArray_GetCount(IShellItemArray* This,DWORD *pdwNumItems) {
    return This->lpVtbl->GetCount(This,pdwNumItems);
}

FORCEINLINE HRESULT IShellItemArray_QueryInterface(IShellItemArray* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}

FORCEINLINE ULONG IEnumShellItems_Release(IEnumShellItems* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE HRESULT IEnumShellItems_QueryInterface(IEnumShellItems* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}

FORCEINLINE HRESULT IDataObject_GetData(IDataObject* This,FORMATETC *pformatetcIn,STGMEDIUM *pmedium) {
    return This->lpVtbl->GetData(This,pformatetcIn,pmedium);
}

FORCEINLINE ULONG IUnknown_AddRef(IUnknown* This) {
    return This->lpVtbl->AddRef(This);
}

FORCEINLINE ULONG IShellFolder_Release(IShellFolder* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE HRESULT IShellFolder_QueryInterface(IShellFolder* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}

FORCEINLINE HRESULT IShellFolder_GetUIObjectOf(IShellFolder* This,HWND hwndOwner,UINT cidl,LPCITEMIDLIST *apidl,REFIID riid,UINT *prgfInOut,void **ppvOut) {
    return This->lpVtbl->GetUIObjectOf(This,hwndOwner,cidl,apidl,riid,prgfInOut,ppvOut);
}

FORCEINLINE ULONG IUnknown_Release(IUnknown* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE HRESULT IShellFolder_GetAttributesOf(IShellFolder* This,UINT cidl,LPCITEMIDLIST *apidl,SFGAOF *rgfInOut) {
    return This->lpVtbl->GetAttributesOf(This,cidl,apidl,rgfInOut);
}

FORCEINLINE HRESULT IShellItem2_GetDisplayName(IShellItem2* This,SIGDN sigdnName,LPWSTR *ppszName) {
    return This->lpVtbl->GetDisplayName(This,sigdnName,ppszName);
}

FORCEINLINE ULONG IShellItem2_Release(IShellItem2* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE HRESULT IShellItem_GetDisplayName(IShellItem* This,SIGDN sigdnName,LPWSTR *ppszName) {
    return This->lpVtbl->GetDisplayName(This,sigdnName,ppszName);
}

FORCEINLINE HRESULT IShellItem2_QueryInterface(IShellItem2* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}

FORCEINLINE HRESULT IShellFolder_BindToObject(IShellFolder* This,LPCITEMIDLIST pidl,LPBC pbcReserved,REFIID riid,void **ppvOut) {
    return This->lpVtbl->BindToObject(This,pidl,pbcReserved,riid,ppvOut);
}

/***************************************************************************************
*	HCR_GetDefaultIcon	[internal]
*
* Gets the icon for a filetype
*/
static BOOL HCR_RegOpenClassIDKey(REFIID riid, HKEY *hkey)
{
	char	xriid[50];
    sprintf( xriid, "CLSID\\{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
                 riid->Data1, riid->Data2, riid->Data3,
                 riid->Data4[0], riid->Data4[1], riid->Data4[2], riid->Data4[3],
                 riid->Data4[4], riid->Data4[5], riid->Data4[6], riid->Data4[7] );

 	TRACE("%s\n",xriid );

	return !RegOpenKeyExA(HKEY_CLASSES_ROOT, xriid, 0, KEY_READ, hkey);
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

/*************************************************************************
 * IShellItemArray implementation
 */
typedef struct {
    IShellItemArray IShellItemArray_iface;
    LONG ref;

    IShellItem **array;
    DWORD item_count;
} IShellItemArrayImpl;

/*************************************************************************
 * IEnumShellItems implementation
 */
typedef struct {
    IEnumShellItems IEnumShellItems_iface;
    LONG ref;

    IShellItemArray *array;
    DWORD count;
    DWORD position;
} IEnumShellItemsImpl;

static inline ShellItem2 *impl_from_IShellItem2(IShellItem2 *iface)
{
    return CONTAINING_RECORD(iface, 
	ShellItem2, 
	IShellItem2_iface);
}

static HRESULT WINAPI ShellItem_QueryInterface(IShellItem2 *iface, REFIID riid,
    void **ppv)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);

    TRACE("(%p,%p,%p)\n", iface, riid, ppv);

    if (!ppv) return E_INVALIDARG;

    if (IsEqualIID(&IID_IUnknown, riid) || IsEqualIID(&IID_IShellItem, riid) ||
        IsEqualIID(&IID_IShellItem2, riid))
    {
        *ppv = &This->IShellItem2_iface;
    }
    else if (IsEqualIID(&IID_IPersist, riid) || IsEqualIID(&IID_IPersistIDList, riid))
    {
        *ppv = &This->IPersistIDList_iface;
    }
    else {
        FIXME("not implemented for %s\n", shdebugstr_guid(riid));
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    IUnknown_AddRef((IUnknown*)*ppv);
    return S_OK;
}

static ULONG WINAPI ShellItem_AddRef(IShellItem2 *iface)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    ULONG ref = InterlockedIncrement(&This->ref);

    TRACE("(%p), new refcount=%i\n", iface, ref);

    return ref;
}

static ULONG WINAPI ShellItem_Release(IShellItem2 *iface)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    ULONG ref = InterlockedDecrement(&This->ref);

    TRACE("(%p), new refcount=%i\n", iface, ref);

    if (ref == 0)
    {
        ILFree(This->pidl);
        HeapFree(GetProcessHeap(), 0, This);
    }

    return ref;
}

static HRESULT ShellItem_get_parent_pidl(ShellItem2 *This, LPITEMIDLIST *parent_pidl)
{
    *parent_pidl = ILClone(This->pidl);
    if (*parent_pidl)
    {
        if (ILRemoveLastID(*parent_pidl))
            return S_OK;
        else
        {
            ILFree(*parent_pidl);
            *parent_pidl = NULL;
            return E_INVALIDARG;
        }
    }
    else
    {
        *parent_pidl = NULL;
        return E_OUTOFMEMORY;
    }
}

static HRESULT ShellItem_get_parent_shellfolder(ShellItem2 *This, IShellFolder **ppsf)
{
    LPITEMIDLIST parent_pidl;
    IShellFolder *desktop;
    HRESULT ret;

    ret = ShellItem_get_parent_pidl(This, &parent_pidl);
    if (SUCCEEDED(ret))
    {
        ret = SHGetDesktopFolder(&desktop);
        if (SUCCEEDED(ret))
        {
            ret = IShellFolder_BindToObject(desktop, parent_pidl, NULL, &IID_IShellFolder, (void**)ppsf);
            IShellFolder_Release(desktop);
        }
        ILFree(parent_pidl);
    }

    return ret;
}

static HRESULT ShellItem_get_shellfolder(ShellItem2 *This, IBindCtx *pbc, IShellFolder **ppsf)
{
    IShellFolder *desktop;
    HRESULT ret;

    ret = SHGetDesktopFolder(&desktop);
    if (SUCCEEDED(ret))
    {
        if (_ILIsDesktop(This->pidl))
        {
            *ppsf = desktop;
            IShellFolder_AddRef(*ppsf);
        }
        else
        {
            ret = IShellFolder_BindToObject(desktop, This->pidl, pbc, &IID_IShellFolder, (void**)ppsf);
        }

        IShellFolder_Release(desktop);
    }

    return ret;
}

static HRESULT WINAPI ShellItem_BindToHandler(IShellItem2 *iface, IBindCtx *pbc,
    REFGUID rbhid, REFIID riid, void **ppvOut)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    HRESULT ret;
    TRACE("(%p,%p,%s,%p,%p)\n", iface, pbc, shdebugstr_guid(rbhid), riid, ppvOut);

    *ppvOut = NULL;
    if (IsEqualGUID(rbhid, &BHID_SFObject))
    {
        IShellFolder *psf;
        ret = ShellItem_get_shellfolder(This, pbc, &psf);
        if (SUCCEEDED(ret))
        {
            ret = IShellFolder_QueryInterface(psf, riid, ppvOut);
            IShellFolder_Release(psf);
        }
        return ret;
    }
    else if (IsEqualGUID(rbhid, &BHID_SFUIObject))
    {
        IShellFolder *psf_parent;
        if (_ILIsDesktop(This->pidl))
            ret = SHGetDesktopFolder(&psf_parent);
        else
            ret = ShellItem_get_parent_shellfolder(This, &psf_parent);

        if (SUCCEEDED(ret))
        {
            LPCITEMIDLIST pidl = ILFindLastID(This->pidl);
            ret = IShellFolder_GetUIObjectOf(psf_parent, NULL, 1, &pidl, riid, NULL, ppvOut);
            IShellFolder_Release(psf_parent);
        }
        return ret;
    }
    else if (IsEqualGUID(rbhid, &BHID_DataObject))
    {
        return ShellItem_BindToHandler(&This->IShellItem2_iface, pbc, &BHID_SFUIObject,
                                       &IID_IDataObject, ppvOut);
    }

    FIXME("Unsupported BHID %s.\n", debugstr_guid(rbhid));

    return MK_E_NOOBJECT;
}

static HRESULT WINAPI ShellItem_GetParent(IShellItem2 *iface, IShellItem **ppsi)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    LPITEMIDLIST parent_pidl;
    HRESULT ret;

    TRACE("(%p,%p)\n", iface, ppsi);

    ret = ShellItem_get_parent_pidl(This, &parent_pidl);
    if (SUCCEEDED(ret))
    {
        ret = SHCreateShellItem(NULL, NULL, parent_pidl, ppsi);
        ILFree(parent_pidl);
    }

    return ret;
}

static HRESULT WINAPI ShellItem_GetDisplayName(IShellItem2 *iface, SIGDN sigdnName,
    LPWSTR *ppszName)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    TRACE("(%p,%x,%p)\n", iface, sigdnName, ppszName);

    return SHGetNameFromIDList(This->pidl, sigdnName, ppszName);
}

static HRESULT WINAPI ShellItem_GetAttributes(IShellItem2 *iface, SFGAOF sfgaoMask,
    SFGAOF *psfgaoAttribs)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    IShellFolder *parent_folder;
    LPITEMIDLIST child_pidl;
    HRESULT ret;

    TRACE("(%p,%x,%p)\n", iface, sfgaoMask, psfgaoAttribs);

    if (_ILIsDesktop(This->pidl))
        ret = SHGetDesktopFolder(&parent_folder);
    else
        ret = ShellItem_get_parent_shellfolder(This, &parent_folder);
    if (SUCCEEDED(ret))
    {
        child_pidl = ILFindLastID(This->pidl);
        *psfgaoAttribs = sfgaoMask;
        ret = IShellFolder_GetAttributesOf(parent_folder, 1, (LPCITEMIDLIST*)&child_pidl, psfgaoAttribs);
        *psfgaoAttribs &= sfgaoMask;
        IShellFolder_Release(parent_folder);

        if(sfgaoMask == *psfgaoAttribs)
            return S_OK;
        else
            return S_FALSE;
    }

    return ret;
}

static HRESULT WINAPI ShellItem_Compare(IShellItem2 *iface, IShellItem *oth,
    SICHINTF hint, int *piOrder)
{
    LPWSTR dispname, dispname_oth;
    HRESULT ret;
    TRACE("(%p,%p,%x,%p)\n", iface, oth, hint, piOrder);

    if(hint & (SICHINT_CANONICAL | SICHINT_ALLFIELDS))
        FIXME("Unsupported flags 0x%08x\n", hint);

    ret = IShellItem2_GetDisplayName(iface, SIGDN_DESKTOPABSOLUTEEDITING, &dispname);
    if(SUCCEEDED(ret))
    {
        ret = IShellItem_GetDisplayName(oth, SIGDN_DESKTOPABSOLUTEEDITING, &dispname_oth);
        if(SUCCEEDED(ret))
        {
            *piOrder = lstrcmpiW(dispname, dispname_oth);
            CoTaskMemFree(dispname_oth);
        }
        CoTaskMemFree(dispname);
    }

    if(SUCCEEDED(ret) && *piOrder &&
       (hint & SICHINT_TEST_FILESYSPATH_IF_NOT_EQUAL))
    {
        LPWSTR dispname, dispname_oth;

        TRACE("Testing filesystem path.\n");
        ret = IShellItem2_GetDisplayName(iface, SIGDN_FILESYSPATH, &dispname);
        if(SUCCEEDED(ret))
        {
            ret = IShellItem_GetDisplayName(oth, SIGDN_FILESYSPATH, &dispname_oth);
            if(SUCCEEDED(ret))
            {
                *piOrder = lstrcmpiW(dispname, dispname_oth);
                CoTaskMemFree(dispname_oth);
            }
            CoTaskMemFree(dispname);
        }
    }

    if(FAILED(ret))
        return ret;

    if(*piOrder)
        return S_FALSE;
    else
        return S_OK;
}

static HRESULT WINAPI ShellItem2_GetPropertyStore(IShellItem2 *iface, GETPROPERTYSTOREFLAGS flags,
    REFIID riid, void **ppv)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%d, %s, %p)\n", This, flags, shdebugstr_guid(riid), ppv);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetPropertyStoreWithCreateObject(IShellItem2 *iface,
    GETPROPERTYSTOREFLAGS flags, IUnknown *punkCreateObject, REFIID riid, void **ppv)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%08x, %p, %s, %p)\n",
          This, flags, punkCreateObject, shdebugstr_guid(riid), ppv);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetPropertyStoreForKeys(IShellItem2 *iface, const PROPERTYKEY *rgKeys,
    UINT cKeys, GETPROPERTYSTOREFLAGS flags, REFIID riid, void **ppv)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p, %d, %08x, %s, %p)\n",
          This, rgKeys, cKeys, flags, shdebugstr_guid(riid), ppv);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetPropertyDescriptionList(IShellItem2 *iface,
    REFPROPERTYKEY keyType, REFIID riid, void **ppv)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p, %s, %p)\n", This, keyType, debugstr_guid(riid), ppv);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_Update(IShellItem2 *iface, IBindCtx *pbc)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p)\n", This, pbc);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetProperty(IShellItem2 *iface, REFPROPERTYKEY key, PROPVARIANT *ppropvar)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p, %p)\n", This, key, ppropvar);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetCLSID(IShellItem2 *iface, REFPROPERTYKEY key, CLSID *pclsid)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p, %p)\n", This, key, pclsid);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetFileTime(IShellItem2 *iface, REFPROPERTYKEY key, FILETIME *pft)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p, %p)\n", This, key, pft);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetInt32(IShellItem2 *iface, REFPROPERTYKEY key, int *pi)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p, %p)\n", This, key, pi);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetString(IShellItem2 *iface, REFPROPERTYKEY key, LPWSTR *ppsz)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p, %p)\n", This, key, ppsz);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetUInt32(IShellItem2 *iface, REFPROPERTYKEY key, ULONG *pui)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p, %p)\n", This, key, pui);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetUInt64(IShellItem2 *iface, REFPROPERTYKEY key, ULONGLONG *pull)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p, %p)\n", This, key, pull);
    return E_NOTIMPL;
}

static HRESULT WINAPI ShellItem2_GetBool(IShellItem2 *iface, REFPROPERTYKEY key, BOOL *pf)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);
    FIXME("Stub: %p (%p, %p)\n", This, key, pf);
    return E_NOTIMPL;
}


static const IShellItem2Vtbl ShellItem2_Vtbl = {
    ShellItem_QueryInterface,
    ShellItem_AddRef,
    ShellItem_Release,
    ShellItem_BindToHandler,
    ShellItem_GetParent,
    ShellItem_GetDisplayName,
    ShellItem_GetAttributes,
    ShellItem_Compare,
    ShellItem2_GetPropertyStore,
    ShellItem2_GetPropertyStoreWithCreateObject,
    ShellItem2_GetPropertyStoreForKeys,
    ShellItem2_GetPropertyDescriptionList,
    ShellItem2_Update,
    ShellItem2_GetProperty,
    ShellItem2_GetCLSID,
    ShellItem2_GetFileTime,
    ShellItem2_GetInt32,
    ShellItem2_GetString,
    ShellItem2_GetUInt32,
    ShellItem2_GetUInt64,
    ShellItem2_GetBool
};

static inline ShellItem2 *impl_from_IPersistIDList( IPersistIDList *iface )
{
    return CONTAINING_RECORD(iface, ShellItem2, IPersistIDList_iface);
}

static HRESULT WINAPI ShellItem_IPersistIDList_QueryInterface(IPersistIDList *iface,
    REFIID riid, void **ppv)
{
    ShellItem2 *This = impl_from_IPersistIDList(iface);
    return IShellItem2_QueryInterface(&This->IShellItem2_iface, riid, ppv);
}

static ULONG WINAPI ShellItem_IPersistIDList_AddRef(IPersistIDList *iface)
{
    ShellItem2 *This = impl_from_IPersistIDList(iface);
    return IShellItem2_AddRef(&This->IShellItem2_iface);
}

static ULONG WINAPI ShellItem_IPersistIDList_Release(IPersistIDList *iface)
{
    ShellItem2 *This = impl_from_IPersistIDList(iface);
    return IShellItem2_Release(&This->IShellItem2_iface);
}

static HRESULT WINAPI ShellItem_IPersistIDList_GetClassID(IPersistIDList* iface,
    CLSID *pClassID)
{
    *pClassID = CLSID_ShellItem;
    return S_OK;
}

static HRESULT WINAPI ShellItem_IPersistIDList_SetIDList(IPersistIDList* iface,
    LPCITEMIDLIST pidl)
{
    ShellItem2 *This = impl_from_IPersistIDList(iface);
    LPITEMIDLIST new_pidl;

    TRACE("(%p,%p)\n", This, pidl);

    new_pidl = ILClone(pidl);

    if (new_pidl)
    {
        ILFree(This->pidl);
        This->pidl = new_pidl;
        return S_OK;
    }
    else
        return E_OUTOFMEMORY;
}

static HRESULT WINAPI ShellItem_IPersistIDList_GetIDList(IPersistIDList* iface,
    LPITEMIDLIST *ppidl)
{
    ShellItem2 *This = impl_from_IPersistIDList(iface);

    TRACE("(%p,%p)\n", This, ppidl);

    *ppidl = ILClone(This->pidl);
    if (*ppidl)
        return S_OK;
    else
        return E_OUTOFMEMORY;
}

static const IPersistIDListVtbl ShellItem_IPersistIDList_Vtbl = {
    ShellItem_IPersistIDList_QueryInterface,
    ShellItem_IPersistIDList_AddRef,
    ShellItem_IPersistIDList_Release,
    ShellItem_IPersistIDList_GetClassID,
    ShellItem_IPersistIDList_SetIDList,
    ShellItem_IPersistIDList_GetIDList
};

HRESULT WINAPI IShellItem_Constructor(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
    ShellItem2 *This;
    HRESULT ret;

    TRACE("(%p,%s)\n",pUnkOuter, debugstr_guid(riid));

    *ppv = NULL;

    if (pUnkOuter) return CLASS_E_NOAGGREGATION;

    This = HeapAlloc(GetProcessHeap(), 0, sizeof(ShellItem2));
    This->IShellItem2_iface.lpVtbl = &ShellItem2_Vtbl;
    This->ref = 1;
    This->pidl = NULL;
    This->IPersistIDList_iface.lpVtbl = &ShellItem_IPersistIDList_Vtbl;

    ret = IShellItem2_QueryInterface(&This->IShellItem2_iface, riid, ppv);
    IShellItem2_Release(&This->IShellItem2_iface);

    return ret;
}

static inline IShellItemArrayImpl *impl_from_IShellItemArray(IShellItemArray *iface)
{
    return CONTAINING_RECORD(iface, IShellItemArrayImpl, IShellItemArray_iface);
}

static HRESULT WINAPI IShellItemArray_fnGetCount(IShellItemArray *iface,
                                                 DWORD *pdwNumItems)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    TRACE("%p (%p)\n", This, pdwNumItems);

    *pdwNumItems = This->item_count;

    return S_OK;
}

static HRESULT WINAPI IShellItemArray_fnGetItemAt(IShellItemArray *iface,
                                                  DWORD dwIndex,
                                                  IShellItem **ppsi)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    TRACE("%p (%x, %p)\n", This, dwIndex, ppsi);

    /* zero indexed */
    if(dwIndex + 1 > This->item_count)
        return E_FAIL;

    *ppsi = This->array[dwIndex];
    IShellItem_AddRef(*ppsi);

    return S_OK;
}

static HRESULT WINAPI IShellItemArray_fnQueryInterface(IShellItemArray *iface,
                                                       REFIID riid,
                                                       void **ppvObject)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);

    *ppvObject = NULL;
    if(IsEqualIID(riid, &IID_IShellItemArray) ||
       IsEqualIID(riid, &IID_IUnknown))
    {
        *ppvObject = &This->IShellItemArray_iface;
    }

    if(*ppvObject)
    {
        //IUnknown_AddRef((IUnknown*)*ppvObject);
        return S_OK;
    }

    return E_NOINTERFACE;
}

static ULONG WINAPI IShellItemArray_fnAddRef(IShellItemArray *iface)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    LONG ref = InterlockedIncrement(&This->ref);
    TRACE("%p - ref %d\n", This, ref);

    return ref;
}

static ULONG WINAPI IShellItemArray_fnRelease(IShellItemArray *iface)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    LONG ref = InterlockedDecrement(&This->ref);
    TRACE("%p - ref %d\n", This, ref);

    if(!ref)
    {
        UINT i;
        TRACE("Freeing.\n");

        for(i = 0; i < This->item_count; i++)
            IShellItem_Release(This->array[i]);

        HeapFree(GetProcessHeap(), 0, This->array);
        HeapFree(GetProcessHeap(), 0, This);
        return 0;
    }

    return ref;
}

static HRESULT WINAPI IShellItemArray_fnBindToHandler(IShellItemArray *iface,
                                                      IBindCtx *pbc,
                                                      REFGUID bhid,
                                                      REFIID riid,
                                                      void **ppvOut)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    FIXME("Stub: %p (%p, %s, %s, %p)\n",
          This, pbc, shdebugstr_guid(bhid), shdebugstr_guid(riid), ppvOut);

    return E_NOTIMPL;
}

static HRESULT WINAPI IShellItemArray_fnGetPropertyStore(IShellItemArray *iface,
                                                         GETPROPERTYSTOREFLAGS flags,
                                                         REFIID riid,
                                                         void **ppv)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    FIXME("Stub: %p (%x, %s, %p)\n", This, flags, shdebugstr_guid(riid), ppv);

    return E_NOTIMPL;
}

static HRESULT WINAPI IShellItemArray_fnGetPropertyDescriptionList(IShellItemArray *iface,
                                                                   REFPROPERTYKEY keyType,
                                                                   REFIID riid,
                                                                   void **ppv)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    FIXME("Stub: %p (%p, %s, %p)\n",
          This, keyType, shdebugstr_guid(riid), ppv);

    return E_NOTIMPL;
}

static HRESULT WINAPI IShellItemArray_fnGetAttributes(IShellItemArray *iface,
                                                      SIATTRIBFLAGS AttribFlags,
                                                      SFGAOF sfgaoMask,
                                                      SFGAOF *psfgaoAttribs)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    HRESULT hr = S_OK;
    SFGAOF attr;
    UINT i;
    TRACE("%p (%x, %x, %p)\n", This, AttribFlags, sfgaoMask, psfgaoAttribs);

    if(AttribFlags & ~(SIATTRIBFLAGS_AND|SIATTRIBFLAGS_OR))
        FIXME("%08x contains unsupported attribution flags", AttribFlags);

    for(i = 0; i < This->item_count; i++)
    {
        hr = IShellItem_GetAttributes(This->array[i], sfgaoMask, &attr);
        if(FAILED(hr))
            break;

        if(i == 0)
        {
            *psfgaoAttribs = attr;
            continue;
        }

        switch(AttribFlags & SIATTRIBFLAGS_MASK)
        {
        case SIATTRIBFLAGS_AND:
            *psfgaoAttribs &= attr;
            break;
        case SIATTRIBFLAGS_OR:
            *psfgaoAttribs |= attr;
            break;
        }
    }

    if(SUCCEEDED(hr))
    {
        if(*psfgaoAttribs == sfgaoMask)
            return S_OK;

        return S_FALSE;
    }

    return hr;
}

static inline IEnumShellItemsImpl *impl_from_IEnumShellItems(IEnumShellItems *iface)
{
    return CONTAINING_RECORD(iface, IEnumShellItemsImpl, IEnumShellItems_iface);
}

static HRESULT WINAPI IEnumShellItems_fnQueryInterface(IEnumShellItems *iface,
                                                       REFIID riid,
                                                       void **ppvObject)
{
    IEnumShellItemsImpl *This = impl_from_IEnumShellItems(iface);
    TRACE("%p (%s, %p)\n", This, shdebugstr_guid(riid), ppvObject);

    *ppvObject = NULL;
    if(IsEqualIID(riid, &IID_IEnumShellItems) ||
       IsEqualIID(riid, &IID_IUnknown))
    {
        *ppvObject = &This->IEnumShellItems_iface;
    }

    if(*ppvObject)
    {
        IUnknown_AddRef_Proxy((IUnknown*)*ppvObject);
        return S_OK;
    }

    return E_NOINTERFACE;
}

static ULONG WINAPI IEnumShellItems_fnAddRef(IEnumShellItems *iface)
{
    IEnumShellItemsImpl *This = impl_from_IEnumShellItems(iface);
    LONG ref = InterlockedIncrement(&This->ref);
    TRACE("%p - ref %d\n", This, ref);

    return ref;
}

static ULONG WINAPI IEnumShellItems_fnRelease(IEnumShellItems *iface)
{
    IEnumShellItemsImpl *This = impl_from_IEnumShellItems(iface);
    LONG ref = InterlockedDecrement(&This->ref);
    TRACE("%p - ref %d\n", This, ref);

    if(!ref)
    {
        TRACE("Freeing.\n");
        IShellItemArray_Release(This->array);
        HeapFree(GetProcessHeap(), 0, This);
        return 0;
    }

    return ref;
}

static HRESULT WINAPI IEnumShellItems_fnNext(IEnumShellItems* iface,
                                             ULONG celt,
                                             IShellItem **rgelt,
                                             ULONG *pceltFetched)
{
    IEnumShellItemsImpl *This = impl_from_IEnumShellItems(iface);
    HRESULT hr = S_FALSE;
    UINT i;
    ULONG fetched = 0;
    TRACE("%p (%d %p %p)\n", This, celt, rgelt, pceltFetched);

    if(pceltFetched == NULL && celt != 1)
        return E_INVALIDARG;

    for(i = This->position; fetched < celt && i < This->count; i++) {
        hr = IShellItemArray_GetItemAt(This->array, i, &rgelt[fetched]);
        if(FAILED(hr))
            break;
        fetched++;
        This->position++;
    }

    if(SUCCEEDED(hr))
    {
        if(pceltFetched != NULL)
            *pceltFetched = fetched;

        if(fetched > 0)
            return S_OK;

        return S_FALSE;
    }

    return hr;
}

static HRESULT WINAPI IEnumShellItems_fnSkip(IEnumShellItems* iface, ULONG celt)
{
    IEnumShellItemsImpl *This = impl_from_IEnumShellItems(iface);
    TRACE("%p (%d)\n", This, celt);

    This->position = min(This->position + celt, This->count-1);

    return S_OK;
}

static HRESULT WINAPI IEnumShellItems_fnReset(IEnumShellItems* iface)
{
    IEnumShellItemsImpl *This = impl_from_IEnumShellItems(iface);
    TRACE("%p\n", This);

    This->position = 0;

    return S_OK;
}

static HRESULT WINAPI IEnumShellItems_fnClone(IEnumShellItems* iface, IEnumShellItems **ppenum)
{
    IEnumShellItemsImpl *This = impl_from_IEnumShellItems(iface);
    TRACE("%p (%p)\n", This, ppenum);

    /* Not implemented anywhere */
    *ppenum = NULL;

    return E_NOTIMPL;
}

static const IEnumShellItemsVtbl vt_IEnumShellItems = {
    IEnumShellItems_fnQueryInterface,
    IEnumShellItems_fnAddRef,
    IEnumShellItems_fnRelease,
    IEnumShellItems_fnNext,
    IEnumShellItems_fnSkip,
    IEnumShellItems_fnReset,
    IEnumShellItems_fnClone
};

static HRESULT IEnumShellItems_Constructor(IShellItemArray *array, IEnumShellItems **ppesi)
{
    IEnumShellItemsImpl *This;
    HRESULT ret;

    This = HeapAlloc(GetProcessHeap(), 0, sizeof(IEnumShellItemsImpl));
    if(!This)
        return E_OUTOFMEMORY;

    This->ref = 1;
    This->IEnumShellItems_iface.lpVtbl = &vt_IEnumShellItems;
    This->array = array;
    This->position = 0;

    IShellItemArray_AddRef(This->array);
    IShellItemArray_GetCount(This->array, &This->count);

    ret = IEnumShellItems_QueryInterface(&This->IEnumShellItems_iface, &IID_IEnumShellItems, (void**)ppesi);
    IEnumShellItems_Release(&This->IEnumShellItems_iface);

    return ret;
}

static HRESULT WINAPI IShellItemArray_fnEnumItems(IShellItemArray *iface,
                                                  IEnumShellItems **ppenumShellItems)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    HRESULT hr;
    TRACE("%p (%p)\n", This, ppenumShellItems);

    hr = IEnumShellItems_Constructor(iface, ppenumShellItems);

    return hr;
}

static const IShellItemArrayVtbl vt_IShellItemArray = {
    IShellItemArray_fnQueryInterface,
    IShellItemArray_fnAddRef,
    IShellItemArray_fnRelease,
    IShellItemArray_fnBindToHandler,
    IShellItemArray_fnGetPropertyStore,
    IShellItemArray_fnGetPropertyDescriptionList,
    IShellItemArray_fnGetAttributes,
    IShellItemArray_fnGetCount,
    IShellItemArray_fnGetItemAt,
    IShellItemArray_fnEnumItems
};

static HRESULT IShellItemArray_Constructor(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
    IShellItemArrayImpl *This;
    HRESULT ret;

    TRACE("(%p, %s, %p)\n",pUnkOuter, debugstr_guid(riid), ppv);

    if(pUnkOuter)
        return CLASS_E_NOAGGREGATION;

    This = HeapAlloc(GetProcessHeap(), 0, sizeof(IShellItemArrayImpl));
    if(!This)
        return E_OUTOFMEMORY;

    This->ref = 1;
    This->IShellItemArray_iface.lpVtbl = &vt_IShellItemArray;
    This->array = NULL;
    This->item_count = 0;

    ret = IShellItemArray_QueryInterface(&This->IShellItemArray_iface, riid, ppv);
    IShellItemArray_Release(&This->IShellItemArray_iface);

    return ret;
}

HRESULT WINAPI SHCreateShellItemArray(PCIDLIST_ABSOLUTE pidlParent,
                                      IShellFolder *psf,
                                      UINT cidl,
                                      PCUITEMID_CHILD_ARRAY ppidl,
                                      IShellItemArray **ppsiItemArray)
{
    IShellItemArrayImpl *This;
    IShellItem **array;
    HRESULT ret = E_FAIL;
    UINT i;

    TRACE("%p, %p, %d, %p, %p\n", pidlParent, psf, cidl, ppidl, ppsiItemArray);

    if(!pidlParent && !psf)
        return E_POINTER;

    if(!ppidl)
        return E_INVALIDARG;

    array = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cidl*sizeof(IShellItem*));
    if(!array)
        return E_OUTOFMEMORY;

    for(i = 0; i < cidl; i++)
    {
        ret = SHCreateShellItem(pidlParent, psf, ppidl[i], &array[i]);
        if(FAILED(ret)) break;
    }

    if(SUCCEEDED(ret))
    {
        ret = IShellItemArray_Constructor(NULL, &IID_IShellItemArray, (void**)&This);
        if(SUCCEEDED(ret))
        {
            This->array = array;
            This->item_count = cidl;
            *ppsiItemArray = &This->IShellItemArray_iface;

            return ret;
        }
    }

    /* Something failed, clean up. */
    for(i = 0; i < cidl; i++)
        if(array[i]) IShellItem_Release(array[i]);
    HeapFree(GetProcessHeap(), 0, array);
    *ppsiItemArray = NULL;
    return ret;
}

HRESULT WINAPI SHCreateShellItemArrayFromDataObject(IDataObject *pdo, REFIID riid, void **ppv)
{
    IShellItemArray *psia;
    FORMATETC fmt;
    STGMEDIUM medium;
    HRESULT ret;

    TRACE("%p, %s, %p\n", pdo, shdebugstr_guid(riid), ppv);

    if(!pdo)
        return E_INVALIDARG;

    *ppv = NULL;

    fmt.cfFormat = RegisterClipboardFormatW(CFSTR_SHELLIDLISTW);
    fmt.ptd = NULL;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.tymed = TYMED_HGLOBAL;

    ret = IDataObject_GetData(pdo, &fmt, &medium);
    if(SUCCEEDED(ret))
    {
        LPIDA pida = GlobalLock(medium.hGlobal);
        LPCITEMIDLIST parent_pidl;
        LPCITEMIDLIST *children;
        UINT i;
        TRACE("Converting %d objects.\n", pida->cidl);

        parent_pidl = (LPCITEMIDLIST) ((LPBYTE)pida+pida->aoffset[0]);

        children = HeapAlloc(GetProcessHeap(), 0, sizeof(LPCITEMIDLIST)*pida->cidl);
        for(i = 0; i < pida->cidl; i++)
            children[i] = (LPCITEMIDLIST) ((LPBYTE)pida+pida->aoffset[i+1]);

        ret = SHCreateShellItemArray(parent_pidl, NULL, pida->cidl, children, &psia);

        HeapFree(GetProcessHeap(), 0, children);

        GlobalUnlock(medium.hGlobal);
        GlobalFree(medium.hGlobal);
    }

    if(SUCCEEDED(ret))
    {
        ret = IShellItemArray_QueryInterface(psia, riid, ppv);
        IShellItemArray_Release(psia);
    }

    return ret;
}

HRESULT WINAPI SHCreateShellItemArrayFromShellItem(IShellItem *psi, REFIID riid, void **ppv)
{
    IShellItemArrayImpl *This;
    IShellItem **array;
    HRESULT ret;

    TRACE("%p, %s, %p\n", psi, shdebugstr_guid(riid), ppv);

    array = HeapAlloc(GetProcessHeap(), 0, sizeof(IShellItem*));
    if(!array)
        return E_OUTOFMEMORY;

    ret = IShellItemArray_Constructor(NULL, riid, (void**)&This);
    if(SUCCEEDED(ret))
    {
        array[0] = psi;
        IShellItem_AddRef(psi);
        This->array = array;
        This->item_count = 1;
        *ppv = This;
    }
    else
    {
        HeapFree(GetProcessHeap(), 0, array);
        *ppv = NULL;
    }

    return ret;
}

HRESULT 
WINAPI 
SHCreateShellItemArrayFromIDLists(
	UINT cidl,
    CIDLIST_ABSOLUTE_ARRAY pidl_array,
    IShellItemArray **psia
)
{
    IShellItemArrayImpl *This;
    IShellItem **array;
    HRESULT ret;
    UINT i;
    TRACE("%d, %p, %p\n", cidl, pidl_array, psia);

    *psia = NULL;

    if(cidl == 0)
        return E_INVALIDARG;

    array = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(IShellItem*));
    if(!array)
        return E_OUTOFMEMORY;

    for(i = 0; i < cidl; i++)
    {
        ret = SHCreateShellItem(NULL, NULL, pidl_array[i], &array[i]);
        if(FAILED(ret))
            break;
    }

    if(SUCCEEDED(ret))
    {
        ret = IShellItemArray_Constructor(NULL, &IID_IShellItemArray, (void**)psia);
        if(SUCCEEDED(ret))
        {
            This = impl_from_IShellItemArray(*psia);
            This->array = array;
            This->item_count = cidl;
            return S_OK;
        }
    }

    for(i = 0; i < cidl; i++)
        if(array[i]) IShellItem_Release(array[i]);
    HeapFree(GetProcessHeap(), 0, array);
    *psia = NULL;
    return ret;
}

HRESULT 
WINAPI 
SHCreateItemFromIDList(
	PCIDLIST_ABSOLUTE pidl, 
	REFIID riid, 
	void **ppv
)
{
    ShellItem2 *psiimpl;
    HRESULT ret;

    if(!pidl)
        return E_INVALIDARG;

    ret = IShellItem_Constructor(NULL, riid, ppv);
    if(SUCCEEDED(ret))
    {
        psiimpl = (ShellItem2*)*ppv;
        psiimpl->pidl = ILClone(pidl);
    }

    return ret;
}

HRESULT 
WINAPI 
SHGetItemFromDataObject(
	IDataObject *pdtobj,
    DATAOBJ_GET_ITEM_FLAGS dwFlags, 
	REFIID riid, 
	void **ppv
)
{
    FORMATETC fmt;
    STGMEDIUM medium;
    HRESULT ret;

    TRACE("%p, %x, %s, %p\n", pdtobj, dwFlags, debugstr_guid(riid), ppv);

    if(!pdtobj)
        return E_INVALIDARG;

    fmt.cfFormat = RegisterClipboardFormatW(CFSTR_SHELLIDLISTW);
    fmt.ptd = NULL;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.tymed = TYMED_HGLOBAL;

    ret = IDataObject_GetData(pdtobj, &fmt, &medium);
    if(SUCCEEDED(ret))
    {
        LPIDA pida = GlobalLock(medium.hGlobal);

        if((pida->cidl > 1 && !(dwFlags & DOGIF_ONLY_IF_ONE)) ||
           pida->cidl == 1)
        {
            LPITEMIDLIST pidl;

            /* Get the first pidl (parent + child1) */
            pidl = ILCombine((LPCITEMIDLIST) ((LPBYTE)pida+pida->aoffset[0]),
                             (LPCITEMIDLIST) ((LPBYTE)pida+pida->aoffset[1]));

            ret = SHCreateItemFromIDList(pidl, riid, ppv);
            ILFree(pidl);
        }
        else
        {
            ret = E_FAIL;
        }

        GlobalUnlock(medium.hGlobal);
        GlobalFree(medium.hGlobal);
    }

    if(FAILED(ret) && !(dwFlags & DOGIF_NO_HDROP))
    {
        TRACE("Attempting to fall back on CF_HDROP.\n");

        fmt.cfFormat = CF_HDROP;
        fmt.ptd = NULL;
        fmt.dwAspect = DVASPECT_CONTENT;
        fmt.lindex = -1;
        fmt.tymed = TYMED_HGLOBAL;

        ret = IDataObject_GetData(pdtobj, &fmt, &medium);
        if(SUCCEEDED(ret))
        {
            DROPFILES *df = GlobalLock(medium.hGlobal);
            LPBYTE files = (LPBYTE)df + df->pFiles;
            BOOL multiple_files = FALSE;

            ret = E_FAIL;
            if(!df->fWide)
            {
                WCHAR filename[MAX_PATH];
                PCSTR first_file = (PCSTR)files;
                if(*(files + lstrlenA(first_file) + 1) != 0)
                    multiple_files = TRUE;

                if( !(multiple_files && (dwFlags & DOGIF_ONLY_IF_ONE)) )
                {
                    MultiByteToWideChar(CP_ACP, 0, first_file, -1, filename, MAX_PATH);
                    ret = SHCreateItemFromParsingName(filename, NULL, riid, ppv);
                }
            }
            else
            {
                PCWSTR first_file = (PCWSTR)files;
                if(*((PCWSTR)files + lstrlenW(first_file) + 1) != 0)
                    multiple_files = TRUE;

                if( !(multiple_files && (dwFlags & DOGIF_ONLY_IF_ONE)) )
                    ret = SHCreateItemFromParsingName(first_file, NULL, riid, ppv);
            }

            GlobalUnlock(medium.hGlobal);
            GlobalFree(medium.hGlobal);
        }
    }

    if(FAILED(ret) && !(dwFlags & DOGIF_NO_URL))
    {
        FIXME("Failed to create item, should try CF_URL.\n");
    }

    return ret;
}

HRESULT 
WINAPI 
SHCreateItemFromParsingName(
	PCWSTR pszPath,
    IBindCtx *pbc, 
	REFIID riid, 
	void **ppv
)
{
    LPITEMIDLIST pidl;
    HRESULT ret;

    *ppv = NULL;

    ret = SHParseDisplayName(pszPath, pbc, &pidl, SFGAO_FOLDER, NULL);
	if(SUCCEEDED(ret))
    {
        ret = SHCreateShellItem(NULL, NULL, pidl, (IShellItem **) ppv);
        if(!SUCCEEDED(ret))
        {
            ILFree(pidl);
        }
    }
    return ret;
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
	HRESULT ret;
	*ppvItem = NULL;
	ret = SHCreateShellItem(NULL, NULL, pidl, (IShellItem **) ppvItem);
    if(!SUCCEEDED(ret))
    {
		ILFree(pidl);
    }
	return ret;
}