/*
 * ExplorerFrame main include
 *
 * Copyright 2010 David Hedberg
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

#ifndef __WINE_EXPLORERFRAME_H
#define __WINE_EXPLORERFRAME_H

#define COBJMACROS

#include "shlobj.h"

extern HINSTANCE explorerframe_hinstance DECLSPEC_HIDDEN;

#define TVS_EX_NOSINGLECOLLAPSE    0x0001
#define TVS_EX_MULTISELECT         0x0002
#define TVS_EX_DOUBLEBUFFER        0x0004
#define TVS_EX_NOINDENTSTATE       0x0008
#define TVS_EX_RICHTOOLTIP         0x0010
#define TVS_EX_AUTOHSCROLL         0x0020
#define TVS_EX_FADEINOUTEXPANDOS   0x0040
#define TVS_EX_PARTIALCHECKBOXES   0x0080
#define TVS_EX_EXCLUSIONCHECKBOXES 0x0100
#define TVS_EX_DIMMEDCHECKBOXES    0x0200
#define TVS_EX_DRAWIMAGEASYNC      0x0400

#define TVM_SETEXTENDEDSTYLE    (TV_FIRST + 44)

#define TVIS_EX_FLAT          0x0001
#define TVIS_EX_DISABLED      0x0002

extern LONG EFRAME_refCount DECLSPEC_HIDDEN;
static inline void EFRAME_LockModule(void) { InterlockedIncrement( &EFRAME_refCount ); }
static inline void EFRAME_UnlockModule(void) { InterlockedDecrement( &EFRAME_refCount ); }

HRESULT NamespaceTreeControl_Constructor(IUnknown *pUnkOuter, REFIID riid, void **ppv) DECLSPEC_HIDDEN;
HRESULT TaskbarList_Constructor(IUnknown*,REFIID,void**) DECLSPEC_HIDDEN;

static inline void* __WINE_ALLOC_SIZE(1) heap_alloc(size_t size)
{
    return HeapAlloc(GetProcessHeap(), 0, size);
}

static inline BOOL heap_free(void *mem)
{
    return HeapFree(GetProcessHeap(), 0, mem);
}

BOOL Shell_GetImageLists(
  _In_ HIMAGELIST *phiml,
  _In_ HIMAGELIST *phimlSmall
);

#endif /* __WINE_EXPLORERFRAME_H */
