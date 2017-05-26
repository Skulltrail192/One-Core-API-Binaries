/*
 * IShellItem and IShellItemArray implementations
 *
 * Copyright 2008 Vincent Povirk for CodeWeavers
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

#include "config.h"
#include "wine/port.h"

#include <stdio.h>
#include <stdarg.h>

#define COBJMACROS
#define NONAMELESSUNION

#include "windef.h"
#include "winbase.h"
#include "wine/debug.h"

#include "pidl.h"
#include "main.h"
#include "debughlp.h"

WINE_DEFAULT_DEBUG_CHANNEL(shell);

typedef struct _ShellItem {
    IShellItem2             IShellItem2_iface;
    LONG                    ref;
    LPITEMIDLIST            pidl;
    IPersistIDList          IPersistIDList_iface;
} ShellItem2;

typedef struct _CustomDestinationList {
    ICustomDestinationList ICustomDestinationList_iface;
    LONG ref;
} CustomDestinationList;

static inline ShellItem2 *impl_from_IShellItem2(IShellItem2 *iface)
{
    return CONTAINING_RECORD(iface, ShellItem2, IShellItem2_iface);
}

static inline ShellItem2 *impl_from_IPersistIDList( IPersistIDList *iface )
{
    return CONTAINING_RECORD(iface, ShellItem2, IPersistIDList_iface);
}

static inline CustomDestinationList *impl_from_ICustomDestinationList( ICustomDestinationList *iface )
{
    return CONTAINING_RECORD(iface, CustomDestinationList, ICustomDestinationList_iface);
}

static HRESULT WINAPI ShellItem_QueryInterface(IShellItem2 *iface, REFIID riid,
    void **ppv)
{
    ShellItem2 *This = impl_from_IShellItem2(iface);

    TRACE("(%p, %s, %p)\n", iface, debugstr_guid(riid), ppv);

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
            if (_ILIsDesktop(parent_pidl))
            {
                *ppsf = desktop;
            }
            else
            {
                ret = IShellFolder_BindToObject(desktop, parent_pidl, NULL, &IID_IShellFolder, (void**)ppsf);
                IShellFolder_Release(desktop);
            }
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

        if (SUCCEEDED(ret))
        {
            if(sfgaoMask == *psfgaoAttribs)
                return S_OK;
            else
                return S_FALSE;
        }
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

HRESULT WINAPI SHCreateItemFromParsingName(PCWSTR pszPath,
    IBindCtx *pbc, REFIID riid, void **ppv)
{
    LPITEMIDLIST pidl;
    HRESULT ret;

    *ppv = NULL;

    ret = SHParseDisplayName(pszPath, pbc, &pidl, 0, NULL);
    if(SUCCEEDED(ret))
    {
        ret = SHCreateItemFromIDList(pidl, riid, ppv);
        ILFree(pidl);
    }
    return ret;
}

HRESULT WINAPI SHCreateItemFromIDList(PCIDLIST_ABSOLUTE pidl, REFIID riid, void **ppv)
{
    IPersistIDList *persist;
    HRESULT ret;

    if(!pidl)
        return E_INVALIDARG;

    *ppv = NULL;
    ret = IShellItem_Constructor(NULL, &IID_IPersistIDList, (void**)&persist);
    if(FAILED(ret))
        return ret;

    ret = IPersistIDList_SetIDList(persist, pidl);
    if(FAILED(ret))
    {
        IPersistIDList_Release(persist);
        return ret;
    }

    ret = IPersistIDList_QueryInterface(persist, riid, ppv);
    IPersistIDList_Release(persist);
    return ret;
}

HRESULT WINAPI SHGetItemFromDataObject(IDataObject *pdtobj,
    DATAOBJ_GET_ITEM_FLAGS dwFlags, REFIID riid, void **ppv)
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
        LPIDA pida = GlobalLock(medium.u.hGlobal);

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

        GlobalUnlock(medium.u.hGlobal);
        GlobalFree(medium.u.hGlobal);
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
            DROPFILES *df = GlobalLock(medium.u.hGlobal);
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

            GlobalUnlock(medium.u.hGlobal);
            GlobalFree(medium.u.hGlobal);
        }
    }

    if(FAILED(ret) && !(dwFlags & DOGIF_NO_URL))
    {
        FIXME("Failed to create item, should try CF_URL.\n");
    }

    return ret;
}

HRESULT WINAPI SHGetItemFromObject(IUnknown *punk, REFIID riid, void **ppv)
{
    LPITEMIDLIST pidl;
    HRESULT ret;

    ret = SHGetIDListFromObject(punk, &pidl);
    if(SUCCEEDED(ret))
    {
        ret = SHCreateItemFromIDList(pidl, riid, ppv);
        ILFree(pidl);
    }

    return ret;
}

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
        IUnknown_AddRef((IUnknown*)*ppvObject);
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


/*************************************************************************
 * IShellItemArray implementation
 */
typedef struct {
    IShellItemArray IShellItemArray_iface;
    LONG ref;

    IShellItem **array;
    DWORD item_count;
} IShellItemArrayImpl;

static inline IShellItemArrayImpl *impl_from_IShellItemArray(IShellItemArray *iface)
{
    return CONTAINING_RECORD(iface, IShellItemArrayImpl, IShellItemArray_iface);
}

static HRESULT WINAPI IShellItemArray_fnQueryInterface(IShellItemArray *iface,
                                                       REFIID riid,
                                                       void **ppvObject)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    TRACE("%p (%s, %p)\n", This, shdebugstr_guid(riid), ppvObject);

    *ppvObject = NULL;
    if(IsEqualIID(riid, &IID_IShellItemArray) ||
       IsEqualIID(riid, &IID_IUnknown))
    {
        *ppvObject = &This->IShellItemArray_iface;
    }

    if(*ppvObject)
    {
        IUnknown_AddRef((IUnknown*)*ppvObject);
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
        FIXME("%08x contains unsupported attribution flags\n", AttribFlags);

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

static HRESULT WINAPI IShellItemArray_fnEnumItems(IShellItemArray *iface,
                                                  IEnumShellItems **ppenumShellItems)
{
    IShellItemArrayImpl *This = impl_from_IShellItemArray(iface);
    TRACE("%p (%p)\n", This, ppenumShellItems);
    return IEnumShellItems_Constructor(iface, ppenumShellItems);
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

/* Caller is responsible to AddRef all items */
static HRESULT create_shellitemarray(IShellItem **items, DWORD count, IShellItemArray **ret)
{
    IShellItemArrayImpl *This;

    TRACE("(%p, %d, %p)\n", items, count, ret);

    This = HeapAlloc(GetProcessHeap(), 0, sizeof(IShellItemArrayImpl));
    if(!This)
        return E_OUTOFMEMORY;

    This->IShellItemArray_iface.lpVtbl = &vt_IShellItemArray;
    This->ref = 1;

    This->array = HeapAlloc(GetProcessHeap(), 0, count*sizeof(IShellItem*));
    if (!This->array)
    {
        HeapFree(GetProcessHeap(), 0, This);
        return E_OUTOFMEMORY;
    }
    memcpy(This->array, items, count*sizeof(IShellItem*));
    This->item_count = count;

    *ret = &This->IShellItemArray_iface;
    return S_OK;
}

HRESULT WINAPI SHCreateShellItemArray(PCIDLIST_ABSOLUTE pidlParent,
                                      IShellFolder *psf,
                                      UINT cidl,
                                      PCUITEMID_CHILD_ARRAY ppidl,
                                      IShellItemArray **ppsiItemArray)
{
    IShellItem **array;
    HRESULT ret = E_FAIL;
    UINT i;

    TRACE("%p, %p, %d, %p, %p\n", pidlParent, psf, cidl, ppidl, ppsiItemArray);

    *ppsiItemArray = NULL;

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
        ret = create_shellitemarray(array, cidl, ppsiItemArray);
        HeapFree(GetProcessHeap(), 0, array);
        if(SUCCEEDED(ret))
            return ret;
    }

    /* Something failed, clean up. */
    for(i = 0; i < cidl; i++)
        if(array[i]) IShellItem_Release(array[i]);
    HeapFree(GetProcessHeap(), 0, array);
    return ret;
}

HRESULT WINAPI SHCreateShellItemArrayFromShellItem(IShellItem *item, REFIID riid, void **ppv)
{
    IShellItemArray *array;
    HRESULT ret;

    TRACE("%p, %s, %p\n", item, shdebugstr_guid(riid), ppv);

    *ppv = NULL;

    IShellItem_AddRef(item);
    ret = create_shellitemarray(&item, 1, &array);
    if(FAILED(ret))
    {
        IShellItem_Release(item);
        return ret;
    }

    ret = IShellItemArray_QueryInterface(array, riid, ppv);
    IShellItemArray_Release(array);
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
        LPIDA pida = GlobalLock(medium.u.hGlobal);
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

        GlobalUnlock(medium.u.hGlobal);
        GlobalFree(medium.u.hGlobal);
    }

    if(SUCCEEDED(ret))
    {
        ret = IShellItemArray_QueryInterface(psia, riid, ppv);
        IShellItemArray_Release(psia);
    }

    return ret;
}

HRESULT WINAPI SHCreateShellItemArrayFromIDLists(UINT cidl,
                                                 PCIDLIST_ABSOLUTE_ARRAY pidl_array,
                                                 IShellItemArray **psia)
{
    IShellItem **array;
    HRESULT ret;
    UINT i;
    TRACE("%d, %p, %p\n", cidl, pidl_array, psia);

    *psia = NULL;

    if(cidl == 0)
        return E_INVALIDARG;

    array = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cidl*sizeof(IShellItem*));
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
        ret = create_shellitemarray(array, cidl, psia);
        HeapFree(GetProcessHeap(), 0, array);
        if(SUCCEEDED(ret))
            return ret;
    }

    for(i = 0; i < cidl; i++)
        if(array[i]) IShellItem_Release(array[i]);
    HeapFree(GetProcessHeap(), 0, array);
    *psia = NULL;
    return ret;
}

HRESULT WINAPI SHGetPropertyStoreFromParsingName(const WCHAR *path, IBindCtx *pbc, GETPROPERTYSTOREFLAGS flags,
    REFIID riid, void **ppv)
{
    IShellItem2 *item;
    HRESULT hr;

    TRACE("(%s %p %#x %p %p)\n", debugstr_w(path), pbc, flags, riid, ppv);

    hr = SHCreateItemFromParsingName(path, pbc, &IID_IShellItem2, (void **)&item);
    if(SUCCEEDED(hr))
    {
        hr = IShellItem2_GetPropertyStore(item, flags, riid, ppv);
        IShellItem2_Release(item);
    }

    return hr;
}

static HRESULT WINAPI CustomDestinationList_QueryInterface(ICustomDestinationList *iface, REFIID riid, void **obj)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);

    TRACE("(%p, %s, %p)\n", This, debugstr_guid(riid), obj);

    if (IsEqualIID(&IID_ICustomDestinationList, riid) || IsEqualIID(&IID_IUnknown, riid))
    {
        *obj = &This->ICustomDestinationList_iface;
    }
    else {
        FIXME("not implemented for %s\n", shdebugstr_guid(riid));
        *obj = NULL;
        return E_NOINTERFACE;
    }

    IUnknown_AddRef((IUnknown*)*obj);
    return S_OK;
}

static ULONG WINAPI CustomDestinationList_AddRef(ICustomDestinationList *iface)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);
    ULONG ref = InterlockedIncrement(&This->ref);

    TRACE("(%p), new refcount=%i\n", This, ref);

    return ref;
}

static ULONG WINAPI CustomDestinationList_Release(ICustomDestinationList *iface)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);
    ULONG ref = InterlockedDecrement(&This->ref);

    TRACE("(%p), new refcount=%i\n", This, ref);

    if (ref == 0)
        HeapFree(GetProcessHeap(), 0, This);

    return ref;
}

static HRESULT WINAPI CustomDestinationList_SetAppID(ICustomDestinationList *iface, const WCHAR *appid)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);

    FIXME("%p (%s): stub\n", This, debugstr_w(appid));

    return E_NOTIMPL;
}

static HRESULT WINAPI CustomDestinationList_BeginList(ICustomDestinationList *iface, UINT *min_slots, REFIID riid, void **obj)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);

    FIXME("%p (%p %s %p): stub\n", This, min_slots, debugstr_guid(riid), obj);

    return E_NOTIMPL;
}

static HRESULT WINAPI CustomDestinationList_AppendCategory(ICustomDestinationList *iface, const WCHAR *category, IObjectArray *array)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);

    FIXME("%p (%s %p): stub\n", This, debugstr_w(category), array);

    return E_NOTIMPL;
}

static HRESULT WINAPI CustomDestinationList_AppendKnownCategory(ICustomDestinationList *iface, KNOWNDESTCATEGORY category)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);

    FIXME("%p (%d): stub\n", This, category);

    return E_NOTIMPL;
}

static HRESULT WINAPI CustomDestinationList_AddUserTasks(ICustomDestinationList *iface, IObjectArray *tasks)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);

    FIXME("%p (%p): stub\n", This, tasks);

    return E_NOTIMPL;
}

static HRESULT WINAPI CustomDestinationList_CommitList(ICustomDestinationList *iface)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);

    FIXME("%p: stub\n", This);

    return E_NOTIMPL;
}

static HRESULT WINAPI CustomDestinationList_GetRemovedDestinations(ICustomDestinationList *iface, REFIID riid, void **obj)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);

    FIXME("%p (%s %p): stub\n", This, debugstr_guid(riid), obj);

    return E_NOTIMPL;
}

static HRESULT WINAPI CustomDestinationList_DeleteList(ICustomDestinationList *iface, const WCHAR *appid)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);

    FIXME("%p (%s): stub\n", This, debugstr_w(appid));

    return E_NOTIMPL;
}

static HRESULT WINAPI CustomDestinationList_AbortList(ICustomDestinationList *iface)
{
    CustomDestinationList *This = impl_from_ICustomDestinationList(iface);

    FIXME("%p: stub\n", This);

    return E_NOTIMPL;
}

static const ICustomDestinationListVtbl CustomDestinationListVtbl =
{
    CustomDestinationList_QueryInterface,
    CustomDestinationList_AddRef,
    CustomDestinationList_Release,
    CustomDestinationList_SetAppID,
    CustomDestinationList_BeginList,
    CustomDestinationList_AppendCategory,
    CustomDestinationList_AppendKnownCategory,
    CustomDestinationList_AddUserTasks,
    CustomDestinationList_CommitList,
    CustomDestinationList_GetRemovedDestinations,
    CustomDestinationList_DeleteList,
    CustomDestinationList_AbortList
};

HRESULT WINAPI CustomDestinationList_Constructor(IUnknown *outer, REFIID riid, void **obj)
{
    CustomDestinationList *list;
    HRESULT hr;

    TRACE("%p %s %p\n", outer, debugstr_guid(riid), obj);

    if (outer)
        return CLASS_E_NOAGGREGATION;

    if(!(list = HeapAlloc(GetProcessHeap(), 0, sizeof(*list))))
        return E_OUTOFMEMORY;

    list->ICustomDestinationList_iface.lpVtbl = &CustomDestinationListVtbl;
    list->ref = 1;

    hr = ICustomDestinationList_QueryInterface(&list->ICustomDestinationList_iface, riid, obj);
    ICustomDestinationList_Release(&list->ICustomDestinationList_iface);
    return hr;
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

typedef struct _NOTIFYICONDATAWEX
{	DWORD cbSize;
	HWND hWnd;
	UINT uID;
	UINT uFlags;
	UINT uCallbackMessage;
	HICON hIcon;
	WCHAR szTip[128];
	DWORD dwState;
	DWORD dwStateMask;
	WCHAR szInfo[256];
	union {
	    UINT uTimeout;
	    UINT uVersion;
	} DUMMYUNIONNAME;
	WCHAR szInfoTitle[64];
	DWORD dwInfoFlags;
	GUID guidItem;
	HICON hBalloonIcon;
} NOTIFYICONDATAWEX, *PNOTIFYICONDATAWEX;

struct notify_data  /* platform-independent format for NOTIFYICONDATA */
{
    LONG  hWnd;
    UINT  uID;
    UINT  uFlags;
    UINT  uCallbackMessage;
    WCHAR szTip[128];
    DWORD dwState;
    DWORD dwStateMask;
    WCHAR szInfo[256];
    union {
        UINT uTimeout;
        UINT uVersion;
    } u;
    WCHAR szInfoTitle[64];
    DWORD dwInfoFlags;
    GUID  guidItem;
    /* data for the icon bitmap */
    UINT width;
    UINT height;
    UINT planes;
    UINT bpp;
};

/* pre IE 5.0 */
#define NOTIFYICONDATAA_V1_SIZE FIELD_OFFSET(NOTIFYICONDATAA, szTip[64])
#define NOTIFYICONDATAW_V1_SIZE FIELD_OFFSET(NOTIFYICONDATAW, szTip[64])

/* pre Window XP */
#define NOTIFYICONDATAA_V2_SIZE FIELD_OFFSET(NOTIFYICONDATAA, guidItem)
#define NOTIFYICONDATAW_V2_SIZE FIELD_OFFSET(NOTIFYICONDATAW, guidItem)

/* pre Window Vista */
#define NOTIFYICONDATAA_V3_SIZE FIELD_OFFSET(NOTIFYICONDATAA, hBalloonIcon)
#define NOTIFYICONDATAW_V3_SIZE FIELD_OFFSET(NOTIFYICONDATAWEX, hBalloonIcon)

static const WCHAR classname[] = /* Shell_TrayWnd */ {'S','h','e','l','l','_','T','r','a','y','W','n','d','\0'};

#define NIF_GUID                0x00000020

/*************************************************************************
 * Shell_NotifyIconW			[SHELL32.298]
 */
BOOL WINAPI Shell_NotifyIconWPlaceHolder(DWORD dwMessage, PNOTIFYICONDATAWEX nid)
{
    HWND tray;
    COPYDATASTRUCT cds;
    struct notify_data data_buffer;
    struct notify_data *data = &data_buffer;
    BOOL ret;

    TRACE("dwMessage = %d, nid->cbSize=%d\n", dwMessage, nid->cbSize);

    /* Validate the cbSize so that WM_COPYDATA doesn't crash the application */
    if (nid->cbSize != NOTIFYICONDATAW_V1_SIZE &&
        nid->cbSize != NOTIFYICONDATAW_V2_SIZE &&
        nid->cbSize != NOTIFYICONDATAW_V3_SIZE &&
        nid->cbSize != sizeof(NOTIFYICONDATAW))
    {
        NOTIFYICONDATAW newNid;

        WARN("Invalid cbSize (%d) - using only Win95 fields (size=%d)\n",
            nid->cbSize, NOTIFYICONDATAW_V1_SIZE);
        CopyMemory(&newNid, nid, NOTIFYICONDATAW_V1_SIZE);
        newNid.cbSize = NOTIFYICONDATAW_V1_SIZE;
        return Shell_NotifyIconW(dwMessage, &newNid);
    }

    tray = FindWindowExW(0, NULL, classname, NULL);
    if (!tray) return FALSE;

    cds.dwData = dwMessage;
    cds.cbData = sizeof(*data);
    memset( data, 0, sizeof(*data) );

    /* FIXME: if statement only needed because we don't support interprocess
     * icon handles */
    if (nid->uFlags & NIF_ICON)
    {
        ICONINFO iconinfo;
        BITMAP bmMask;
        BITMAP bmColour;
        LONG cbMaskBits;
        LONG cbColourBits = 0;
        char *buffer;

        if (!GetIconInfo(nid->hIcon, &iconinfo))
            goto noicon;

        if (!GetObjectW(iconinfo.hbmMask, sizeof(bmMask), &bmMask) ||
            (iconinfo.hbmColor && !GetObjectW(iconinfo.hbmColor, sizeof(bmColour), &bmColour)))
        {
            DeleteObject(iconinfo.hbmMask);
            if (iconinfo.hbmColor) DeleteObject(iconinfo.hbmColor);
            goto noicon;
        }

        cbMaskBits = (bmMask.bmPlanes * bmMask.bmWidth * bmMask.bmHeight * bmMask.bmBitsPixel + 15) / 16 * 2;
        if (iconinfo.hbmColor)
            cbColourBits = (bmColour.bmPlanes * bmColour.bmWidth * bmColour.bmHeight * bmColour.bmBitsPixel + 15) / 16 * 2;
        cds.cbData = sizeof(*data) + cbMaskBits + cbColourBits;
        buffer = HeapAlloc(GetProcessHeap(), 0, cds.cbData);
        if (!buffer)
        {
            DeleteObject(iconinfo.hbmMask);
            if (iconinfo.hbmColor) DeleteObject(iconinfo.hbmColor);
            return FALSE;
        }

        data = (struct notify_data *)buffer;
        memset( data, 0, sizeof(*data) );
        buffer += sizeof(*data);
        GetBitmapBits(iconinfo.hbmMask, cbMaskBits, buffer);
        if (!iconinfo.hbmColor)
        {
            data->width  = bmMask.bmWidth;
            data->height = bmMask.bmHeight / 2;
            data->planes = 1;
            data->bpp    = 1;
        }
        else
        {
            data->width  = bmColour.bmWidth;
            data->height = bmColour.bmHeight;
            data->planes = bmColour.bmPlanes;
            data->bpp    = bmColour.bmBitsPixel;
            buffer += cbMaskBits;
            GetBitmapBits(iconinfo.hbmColor, cbColourBits, buffer);
            DeleteObject(iconinfo.hbmColor);
        }
        DeleteObject(iconinfo.hbmMask);
    }

noicon:
    data->hWnd   = HandleToLong( nid->hWnd );
    data->uID    = nid->uID;
    data->uFlags = nid->uFlags;
    if (data->uFlags & NIF_MESSAGE)
        data->uCallbackMessage = nid->uCallbackMessage;
    if (data->uFlags & NIF_TIP)
        lstrcpynW( data->szTip, nid->szTip, sizeof(data->szTip)/sizeof(WCHAR) );
    if (data->uFlags & NIF_STATE)
    {
        data->dwState     = nid->dwState;
        data->dwStateMask = nid->dwStateMask;
    }
    if (data->uFlags & NIF_INFO)
    {
        lstrcpynW( data->szInfo, nid->szInfo, sizeof(data->szInfo)/sizeof(WCHAR) );
        lstrcpynW( data->szInfoTitle, nid->szInfoTitle, sizeof(data->szInfoTitle)/sizeof(WCHAR) );
        data->u.uTimeout  = nid->u.uTimeout;
        data->dwInfoFlags = nid->dwInfoFlags;
    }
    if (data->uFlags & NIF_GUID)
        data->guidItem = nid->guidItem;
    if (dwMessage == NIM_SETVERSION)
        data->u.uVersion = nid->u.uVersion;
    /* FIXME: balloon icon */

    cds.lpData = data;
    ret = SendMessageW(tray, WM_COPYDATA, (WPARAM)nid->hWnd, (LPARAM)&cds);
    if (data != &data_buffer) HeapFree( GetProcessHeap(), 0, data );
    return ret;
}