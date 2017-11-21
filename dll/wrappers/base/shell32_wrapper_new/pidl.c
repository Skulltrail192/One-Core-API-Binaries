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

FORCEINLINE HRESULT IShellFolder_GetDisplayNameOf(IShellFolder* This,LPCITEMIDLIST pidl,SHGDNF uFlags,STRRET *lpName) {
    return This->lpVtbl->GetDisplayNameOf(This,pidl,uFlags,lpName);
}

FORCEINLINE ULONG IShellFolder_Release(IShellFolder* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE HRESULT IUnknown_QueryInterface(IUnknown* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}

FORCEINLINE HRESULT IPersistIDList_GetIDList(IPersistIDList* This,LPITEMIDLIST *ppidl) {
    return This->lpVtbl->GetIDList(This,ppidl);
}

FORCEINLINE ULONG IPersistIDList_Release(IPersistIDList* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE HRESULT IPersistFolder2_GetCurFolder(IPersistFolder2* This,LPITEMIDLIST *ppidl) {
    return This->lpVtbl->GetCurFolder(This,ppidl);
}

FORCEINLINE ULONG IPersistFolder2_Release(IPersistFolder2* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE ULONG IShellItem_Release(IShellItem* This) {
    return This->lpVtbl->Release(This);
}

FORCEINLINE ULONG IDataObject_Release(IDataObject* This)
{
	return This->lpVtbl->Release(This);
} 

FORCEINLINE HRESULT IFolderView_GetFolder(IFolderView* This,REFIID riid,void **ppv) {
    return This->lpVtbl->GetFolder(This,riid,ppv);
}

FORCEINLINE ULONG IFolderView_Release(IFolderView* This) {
    return This->lpVtbl->Release(This);
}
// /*************************************************************************
 // * SHGetNameFromIDList             [SHELL32.@]
 // */
// HRESULT WINAPI SHGetNameFromIDList(PCIDLIST_ABSOLUTE pidl, SIGDN sigdnName, PWSTR *ppszName)
// {
    // IShellFolder *psfparent;
    // LPCITEMIDLIST child_pidl;
    // STRRET disp_name;
    // HRESULT ret;

    // TRACE("%p 0x%08x %p\n", pidl, sigdnName, ppszName);

    // *ppszName = NULL;
    // ret = SHBindToParent(pidl, &IID_IShellFolder, (void**)&psfparent, &child_pidl);
    // if(SUCCEEDED(ret))
    // {
        // switch(sigdnName)
        // {
                                                // /* sigdnName & 0xffff */
        // case SIGDN_NORMALDISPLAY:               /* SHGDN_NORMAL */
        // case SIGDN_PARENTRELATIVEPARSING:       /* SHGDN_INFOLDER | SHGDN_FORPARSING */
        // case SIGDN_PARENTRELATIVEEDITING:       /* SHGDN_INFOLDER | SHGDN_FOREDITING */
        // case SIGDN_DESKTOPABSOLUTEPARSING:      /* SHGDN_FORPARSING */
        // case SIGDN_DESKTOPABSOLUTEEDITING:      /* SHGDN_FOREDITING | SHGDN_FORADDRESSBAR*/
        // case SIGDN_PARENTRELATIVEFORADDRESSBAR: /* SIGDN_INFOLDER | SHGDN_FORADDRESSBAR */
        // case SIGDN_PARENTRELATIVE:              /* SIGDN_INFOLDER */

            // disp_name.uType = STRRET_WSTR;
            // ret = IShellFolder_GetDisplayNameOf(psfparent, child_pidl,
                                                // sigdnName & 0xffff,
                                                // &disp_name);
            // if(SUCCEEDED(ret))
                // ret = StrRetToStrW(&disp_name, pidl, ppszName);

            // break;

        // case SIGDN_FILESYSPATH:
            // *ppszName = CoTaskMemAlloc(sizeof(WCHAR)*MAX_PATH);
            // if(SHGetPathFromIDListW(pidl, *ppszName))
            // {
                // TRACE("Got string %s\n", debugstr_w(*ppszName));
                // ret = S_OK;
            // }
            // else
            // {
                // CoTaskMemFree(*ppszName);
                // ret = E_INVALIDARG;
            // }
            // break;

        // case SIGDN_URL:
        // default:
            // FIXME("Unsupported SIGDN %x\n", sigdnName);
            // ret = E_FAIL;
        // }

        // IShellFolder_Release(psfparent);
    // }
    // return ret;
// }

/**************************************************************************
 *
 *    ### 4. getting pointers to parts of pidls ###
 *
 **************************************************************************
 *  _ILGetDataPointer()
 */
LPPIDLDATA _ILGetDataPointer(LPCITEMIDLIST pidl)
{
    if(!_ILIsEmpty(pidl))
        return (LPPIDLDATA)pidl->mkid.abID;
    return NULL;
}

// /*************************************************************************
 // * SHGetIDListFromObject             [SHELL32.@]
 // */
// HRESULT WINAPI SHGetIDListFromObject(IUnknown *punk, PIDLIST_ABSOLUTE *ppidl)
// {
    // IPersistIDList *ppersidl;
    // IPersistFolder2 *ppf2;
    // IDataObject *pdo;
    // IFolderView *pfv;
    // HRESULT ret;

    // if(!punk)
        // return E_NOINTERFACE;

    // *ppidl = NULL;

    // /* Try IPersistIDList */
    // ret = IUnknown_QueryInterface(punk, &IID_IPersistIDList, (void**)&ppersidl);
    // if(SUCCEEDED(ret))
    // {
        // TRACE("IPersistIDList (%p)\n", ppersidl);
        // ret = IPersistIDList_GetIDList(ppersidl, ppidl);
        // IPersistIDList_Release(ppersidl);
        // if(SUCCEEDED(ret))
            // return ret;
    // }

    // /* Try IPersistFolder2 */
    // ret = IUnknown_QueryInterface(punk, &IID_IPersistFolder2, (void**)&ppf2);
    // if(SUCCEEDED(ret))
    // {
        // TRACE("IPersistFolder2 (%p)\n", ppf2);
        // ret = IPersistFolder2_GetCurFolder(ppf2, ppidl);
        // IPersistFolder2_Release(ppf2);
        // if(SUCCEEDED(ret))
            // return ret;
    // }

    // /* Try IDataObject */
    // ret = IUnknown_QueryInterface(punk, &IID_IDataObject, (void**)&pdo);
    // if(SUCCEEDED(ret))
    // {
        // IShellItem *psi;
        // TRACE("IDataObject (%p)\n", pdo);
        // ret = SHGetItemFromDataObject(pdo, DOGIF_ONLY_IF_ONE,
                                      // &IID_IShellItem, (void**)&psi);
        // if(SUCCEEDED(ret))
        // {
            // ret = SHGetIDListFromObject((IUnknown*)psi, ppidl);
            // IShellItem_Release(psi);
        // }
        // IDataObject_Release(pdo);

        // if(SUCCEEDED(ret))
            // return ret;
    // }

    // /* Try IFolderView */
    // ret = IUnknown_QueryInterface(punk, &IID_IFolderView, (void**)&pfv);
    // if(SUCCEEDED(ret))
    // {
        // IShellFolder *psf;
        // TRACE("IFolderView (%p)\n", pfv);
        // ret = IFolderView_GetFolder(pfv, &IID_IShellFolder, (void**)&psf);
        // if(SUCCEEDED(ret))
        // {
            // /* We might be able to get IPersistFolder2 from a shellfolder. */
            // ret = SHGetIDListFromObject((IUnknown*)psf, ppidl);
        // }
        // IFolderView_Release(pfv);
        // return ret;
    // }

    // return ret;
// }

/**************************************************************************
 *  _ILGetTextPointer()
 * gets a pointer to the long filename string stored in the pidl
 */
LPSTR _ILGetTextPointer(LPCITEMIDLIST pidl)
{
    /* TRACE(pidl,"(pidl%p)\n", pidl);*/

    LPPIDLDATA pdata = _ILGetDataPointer(pidl);

    if (!pdata)
        return NULL;

    switch (pdata->type)
    {
    case PT_GUID:
    case PT_SHELLEXT:
    case PT_YAGUID:
        return NULL;

    case PT_DRIVE:
    case PT_DRIVE1:
    case PT_DRIVE2:
    case PT_DRIVE3:
        return pdata->u.drive.szDriveName;

    case PT_FOLDER:
    case PT_FOLDER1:
    case PT_VALUE:
    case PT_IESPECIAL1:
    case PT_IESPECIAL2:
        return pdata->u.file.szNames;

    case PT_WORKGRP:
    case PT_COMP:
    case PT_NETWORK:
    case PT_NETPROVIDER:
    case PT_SHARE:
        return pdata->u.network.szNames;
    }
    return NULL;
}

/**************************************************************************
 *  _ILGetTextPointerW()
 * gets a pointer to the unicode long filename string stored in the pidl
 */
static LPWSTR _ILGetTextPointerW(LPCITEMIDLIST pidl)
{
    /* TRACE(pidl,"(pidl%p)\n", pidl);*/

    LPPIDLDATA pdata = _ILGetDataPointer(pidl);

    if (!pdata)
        return NULL;

    switch (pdata->type)
    {
    case PT_GUID:
    case PT_SHELLEXT:
    case PT_YAGUID:
        return NULL;

    case PT_DRIVE:
    case PT_DRIVE1:
    case PT_DRIVE2:
    case PT_DRIVE3:
        /*return (LPSTR)&(pdata->u.drive.szDriveName);*/
        return NULL;

    case PT_FOLDER:
    case PT_FOLDER1:
    case PT_VALUE:
    case PT_IESPECIAL1:
    case PT_IESPECIAL2:
        /*return (LPSTR)&(pdata->u.file.szNames);*/
        return NULL;

    case PT_WORKGRP:
    case PT_COMP:
    case PT_NETWORK:
    case PT_NETPROVIDER:
    case PT_SHARE:
        /*return (LPSTR)&(pdata->u.network.szNames);*/
        return NULL;

    case PT_VALUEW:
        return (LPWSTR)pdata->u.file.szNames;
    }
    return NULL;
}


 /**************************************************************************
 *  _ILSimpleGetText
 *
 * gets the text for the first item in the pidl (eg. simple pidl)
 *
 * returns the length of the string
 */
DWORD _ILSimpleGetText (LPCITEMIDLIST pidl, LPSTR szOut, UINT uOutSize)
{
    DWORD        dwReturn=0;
    LPSTR        szSrc;
    LPWSTR       szSrcW;
    GUID const * riid;
    char szTemp[MAX_PATH];

    TRACE("(%p %p %x)\n",pidl,szOut,uOutSize);

    if (!pidl)
        return 0;

    if (szOut)
        *szOut = 0;

    if (_ILIsDesktop(pidl))
    {
        /* desktop */
        if (HCR_GetClassNameA(&CLSID_ShellDesktop, szTemp, MAX_PATH))
        {
            if (szOut)
                lstrcpynA(szOut, szTemp, uOutSize);

            dwReturn = strlen (szTemp);
        }
    }
    else if (( szSrc = _ILGetTextPointer(pidl) ))
    {
        /* filesystem */
        if (szOut)
            lstrcpynA(szOut, szSrc, uOutSize);

        dwReturn = strlen(szSrc);
    }
    else if (( szSrcW = _ILGetTextPointerW(pidl) ))
    {
        /* unicode filesystem */
        WideCharToMultiByte(CP_ACP,0,szSrcW, -1, szTemp, MAX_PATH, NULL, NULL);

        if (szOut)
            lstrcpynA(szOut, szTemp, uOutSize);

        dwReturn = strlen (szTemp);
    }
    else if (( riid = _ILGetGUIDPointer(pidl) ))
    {
        /* special folder */
        if ( HCR_GetClassNameA(riid, szTemp, MAX_PATH) )
        {
            if (szOut)
                lstrcpynA(szOut, szTemp, uOutSize);

            dwReturn = strlen (szTemp);
        }
    }
    else
    {
        ERR("-- no text\n");
    }

    TRACE("-- (%p=%s 0x%08x)\n",szOut,debugstr_a(szOut),dwReturn);
    return dwReturn;
}


BOOL _ILGetExtension (LPCITEMIDLIST pidl, LPSTR pOut, UINT uOutSize)
{
    char szTemp[MAX_PATH];
    const char * pPoint;
    LPCITEMIDLIST  pidlTemp=pidl;

    TRACE("pidl=%p\n",pidl);

    if (!pidl)
        return FALSE;

    pidlTemp = ILFindLastID(pidl);

    if (!_ILIsValue(pidlTemp))
        return FALSE;
    if (!_ILSimpleGetText(pidlTemp, szTemp, MAX_PATH))
        return FALSE;

    pPoint = PathFindExtensionA(szTemp);

    if (!*pPoint)
        return FALSE;

    pPoint++;
    lstrcpynA(pOut, pPoint, uOutSize);
    TRACE("%s\n",pOut);

    return TRUE;
}

/*************************************************************************
 * ILFreeaPidl
 *
 * frees an aPidl struct
 */
void _ILFreeaPidl(LPITEMIDLIST * apidl, UINT cidl)
{
    UINT   i;

    if (apidl)
    {
        for (i = 0; i < cidl; i++)
            SHFree(apidl[i]);
        SHFree(apidl);
    }
}

BOOL _ILIsValue(LPCITEMIDLIST pidl)
{
    LPPIDLDATA lpPData = _ILGetDataPointer(pidl);

    TRACE("(%p)\n",pidl);

    return (pidl && lpPData && PT_VALUE == lpPData->type);
}

/**************************************************************************
 *    _ILIsPidlSimple
 */
BOOL _ILIsPidlSimple(LPCITEMIDLIST pidl)
{
    BOOL ret = TRUE;

    if(! _ILIsDesktop(pidl))    /* pidl=NULL or mkid.cb=0 */
    {
        WORD len = pidl->mkid.cb;
        LPCITEMIDLIST pidlnext = (LPCITEMIDLIST) (((const BYTE*)pidl) + len );

        if (pidlnext->mkid.cb)
            ret = FALSE;
    }

    TRACE("%s\n", ret ? "Yes" : "No");
    return ret;
}

/**************************************************************************
 * _ILGetGUIDPointer()
 *
 * returns reference to guid stored in some pidls
 */
IID* _ILGetGUIDPointer(LPCITEMIDLIST pidl)
{
    LPPIDLDATA pdata =_ILGetDataPointer(pidl);

    TRACE("%p\n", pidl);

    if (!pdata)
        return NULL;

    TRACE("pdata->type 0x%04x\n", pdata->type);
    switch (pdata->type)
    {
    case PT_SHELLEXT:
    case PT_GUID:
    case PT_YAGUID:
        return &(pdata->u.guid.guid);

    default:
        TRACE("Unknown pidl type 0x%04x\n", pdata->type);
        break;
    }
    return NULL;
}

BOOL _ILIsSpecialFolder (LPCITEMIDLIST pidl)
{
    LPPIDLDATA lpPData = _ILGetDataPointer(pidl);

    TRACE("(%p)\n",pidl);

    return (pidl && ( (lpPData && (PT_GUID== lpPData->type || PT_SHELLEXT== lpPData->type || PT_YAGUID == lpPData->type)) ||
              (pidl && pidl->mkid.cb == 0x00)
            ));
}

BOOL _ILIsFolder(LPCITEMIDLIST pidl)
{
    LPPIDLDATA lpPData = _ILGetDataPointer(pidl);

    TRACE("(%p)\n",pidl);

    return (pidl && lpPData && (PT_FOLDER == lpPData->type || PT_FOLDER1 == lpPData->type));
}

/*************************************************************************
 * ILCopyaPidl
 *
 * copies an aPidl struct
 */
LPITEMIDLIST* _ILCopyaPidl(const LPCITEMIDLIST * apidlsrc, UINT cidl)
{
    UINT i;
    LPITEMIDLIST *apidldest;

    if (!apidlsrc)
        return NULL;

    apidldest = SHAlloc(cidl * sizeof(LPITEMIDLIST));

    for (i = 0; i < cidl; i++)
        apidldest[i] = ILClone(apidlsrc[i]);

    return apidldest;
}

/*************************************************************************
 * _ILCopyCidaToaPidl
 *
 * creates aPidl from CIDA
 */
LPITEMIDLIST* _ILCopyCidaToaPidl(LPITEMIDLIST* pidl, const CIDA * cida)
{
    UINT i;
    LPITEMIDLIST *dst;

    dst = SHAlloc(cida->cidl * sizeof(LPITEMIDLIST));
    if (!dst)
        return NULL;

    if (pidl)
        *pidl = ILClone((LPCITEMIDLIST)(&((const BYTE*)cida)[cida->aoffset[0]]));

    for (i = 0; i < cida->cidl; i++)
        dst[i] = ILClone((LPCITEMIDLIST)(&((const BYTE*)cida)[cida->aoffset[i + 1]]));

    return dst;
}

/******************************************************************************
 * _ILGetFileStructW [Internal]
 *
 * Get pointer the a SHITEMID's FileStructW field if present
 *
 * PARAMS
 *  pidl [I] The SHITEMID
 *
 * RETURNS
 *  Success: Pointer to pidl's FileStructW field.
 *  Failure: NULL
 */
FileStructW* _ILGetFileStructW(LPCITEMIDLIST pidl) {
    FileStructW *pFileStructW;
    WORD cbOffset;
    
    if (!(_ILIsValue(pidl) || _ILIsFolder(pidl)))
        return NULL;

    cbOffset = *(const WORD *)((const BYTE *)pidl + pidl->mkid.cb - sizeof(WORD));
    pFileStructW = (FileStructW*)((LPBYTE)pidl + cbOffset);

    /* Currently I don't see a fool prove way to figure out if a pidl is for sure of WinXP
     * style with a FileStructW member. If we switch all our shellfolder-implementations to
     * the new format, this won't be a problem. For now, we do as many sanity checks as possible. */
    if ((cbOffset & 0x1) || /* FileStructW member is word aligned in the pidl */
        /* FileStructW is positioned after FileStruct */
        cbOffset < sizeof(pidl->mkid.cb) + sizeof(PIDLTYPE) + sizeof(FileStruct) ||
        /* There has to be enough space at cbOffset in the pidl to hold FileStructW and cbOffset */
        cbOffset > pidl->mkid.cb - sizeof(cbOffset) - sizeof(FileStructW) ||
        pidl->mkid.cb != cbOffset + pFileStructW->cbLen)
    {
        WARN("Invalid pidl format (cbOffset = %d)!\n", cbOffset);
        return NULL;
    }

    return pFileStructW;
}

BOOL ILGetDisplayNameExW(LPSHELLFOLDER psf, LPCITEMIDLIST pidl, LPWSTR path, DWORD type)
{
    LPSHELLFOLDER psfParent, lsf = psf;
    HRESULT ret = NO_ERROR;
    LPCITEMIDLIST pidllast;
    STRRET strret;
    DWORD flag;

    TRACE("%p %p %p %x\n", psf, pidl, path, type);

    if (!pidl || !path)
        return FALSE;

    if (!lsf)
    {
        ret = SHGetDesktopFolder(&lsf);
        if (FAILED(ret))
            return FALSE;
    }

    switch (type)
    {
    case ILGDN_FORPARSING:
        flag = SHGDN_FORPARSING | SHGDN_FORADDRESSBAR;
        break;
    case ILGDN_NORMAL:
        flag = SHGDN_NORMAL;
        break;
    case ILGDN_INFOLDER:
        flag = SHGDN_INFOLDER;
        break;
    default:
        FIXME("Unknown type parameter = %x\n", type);
        flag = SHGDN_FORPARSING | SHGDN_FORADDRESSBAR;
        break;
    }

    if (!*(const WORD*)pidl || type == ILGDN_FORPARSING)
    {
        ret = IShellFolder_GetDisplayNameOf(lsf, pidl, flag, &strret);
        if (SUCCEEDED(ret))
        {
            if(!StrRetToStrNW(path, MAX_PATH, &strret, pidl))
                ret = E_FAIL;
        }
    }
    else
    {
        ret = SHBindToParent(pidl, &IID_IShellFolder, (LPVOID*)&psfParent, &pidllast);
        if (SUCCEEDED(ret))
        {
            ret = IShellFolder_GetDisplayNameOf(psfParent, pidllast, flag, &strret);
            if (SUCCEEDED(ret))
            {
                if(!StrRetToStrNW(path, MAX_PATH, &strret, pidllast))
                    ret = E_FAIL;
            }
            IShellFolder_Release(psfParent);
        }
    }

    TRACE("%p %p %s\n", psf, pidl, debugstr_w(path));

    if (!psf)
        IShellFolder_Release(lsf);
    return SUCCEEDED(ret);
}

 /**************************************************************************
 *  _ILSimpleGetTextW
 *
 * gets the text for the first item in the pidl (eg. simple pidl)
 *
 * returns the length of the string
 */
DWORD _ILSimpleGetTextW (LPCITEMIDLIST pidl, LPWSTR szOut, UINT uOutSize)
{
    DWORD   dwReturn;
    FileStructW *pFileStructW = _ILGetFileStructW(pidl);

    TRACE("(%p %p %x)\n",pidl,szOut,uOutSize);

    if (pFileStructW) {
        lstrcpynW(szOut, pFileStructW->wszName, uOutSize);
        dwReturn = lstrlenW(pFileStructW->wszName);
    } else {
        GUID const * riid;
        WCHAR szTemp[MAX_PATH];
        LPSTR szSrc;
        LPWSTR szSrcW;
        dwReturn=0;

        if (!pidl)
            return 0;

        if (szOut)
            *szOut = 0;

        if (_ILIsDesktop(pidl))
        {
            /* desktop */
            if (HCR_GetClassNameW(&CLSID_ShellDesktop, szTemp, MAX_PATH))
            {
                if (szOut)
                    lstrcpynW(szOut, szTemp, uOutSize);

                dwReturn = lstrlenW (szTemp);
            }
        }
        else if (( szSrcW = _ILGetTextPointerW(pidl) ))
        {
            /* unicode filesystem */
            if (szOut)
                lstrcpynW(szOut, szSrcW, uOutSize);

            dwReturn = lstrlenW(szSrcW);
        }
        else if (( szSrc = _ILGetTextPointer(pidl) ))
        {
            /* filesystem */
            MultiByteToWideChar(CP_ACP, 0, szSrc, -1, szTemp, MAX_PATH);

            if (szOut)
                lstrcpynW(szOut, szTemp, uOutSize);

            dwReturn = lstrlenW (szTemp);
        }
        else if (( riid = _ILGetGUIDPointer(pidl) ))
        {
            /* special folder */
            if ( HCR_GetClassNameW(riid, szTemp, MAX_PATH) )
            {
                if (szOut)
                    lstrcpynW(szOut, szTemp, uOutSize);

                dwReturn = lstrlenW (szTemp);
            }
        }
        else
        {
            ERR("-- no text\n");
        }
    }

    TRACE("-- (%p=%s 0x%08x)\n",szOut,debugstr_w(szOut),dwReturn);
    return dwReturn;
}