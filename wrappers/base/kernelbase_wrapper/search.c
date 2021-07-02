/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    numa.c

Abstract:

    This module implements Win32 Non Uniform Memory Architecture
    information APIs.

Author:

    Skulltrail 18-April-2018

Revision History:

--*/

#include <assert.h>

#define COBJMACROS
#define NONAMELESSUNION

#include "initguid.h"
#include "wine/debug.h"
#include "wine/list.h"
#include "shlwapi.h"

WINE_DEFAULT_DEBUG_CHANNEL(search);

/*************************************************************************
 *      @	[SHLWAPI.219]
 *
 * Call IUnknown_QueryInterface() on a table of objects.
 *
 * RETURNS
 *  Success: S_OK.
 *  Failure: E_POINTER or E_NOINTERFACE.
 */
HRESULT WINAPI QISearch(
	void *base,         /* [in]   Table of interfaces */
	const QITAB *table, /* [in]   Array of REFIIDs and indexes into the table */
	REFIID riid,        /* [in]   REFIID to get interface for */
	void **ppv)         /* [out]  Destination for interface pointer */
{
	HRESULT ret;
	IUnknown *a_vtbl;
	const QITAB *xmove;

	TRACE("(%p %p %s %p)\n", base, table, debugstr_guid(riid), ppv);
	if (ppv) {
	    xmove = table;
	    while (xmove->piid) {
		TRACE("trying (offset %d) %s\n", xmove->dwOffset, debugstr_guid(xmove->piid));
		if (IsEqualIID(riid, xmove->piid)) {
		    a_vtbl = (IUnknown*)(xmove->dwOffset + (LPBYTE)base);
		    TRACE("matched, returning (%p)\n", a_vtbl);
                    *ppv = a_vtbl;
		    IUnknown_AddRef(a_vtbl);
		    return S_OK;
		}
		xmove++;
	    }

	    if (IsEqualIID(riid, &IID_IUnknown)) {
		a_vtbl = (IUnknown*)(table->dwOffset + (LPBYTE)base);
		TRACE("returning first for IUnknown (%p)\n", a_vtbl);
                *ppv = a_vtbl;
		IUnknown_AddRef(a_vtbl);
		return S_OK;
	    }
	    *ppv = 0;
	    ret = E_NOINTERFACE;
	} else
	    ret = E_POINTER;

	TRACE("-- 0x%08x\n", ret);
	return ret;
}