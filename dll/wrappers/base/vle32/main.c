/*
 * Performance Data Helper (pdh.dll)
 *
 * Copyright 2007 Andrey Turkin
 * Copyright 2007 Hans Leidekker
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

#define WIN32_NO_STATUS

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define COBJMACROS
#define NONAMELESSUNION

#include "windef.h"
#include "winbase.h"
#include "winerror.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "winreg.h"
#include "ole2.h"
#include "ole2ver.h"

#include "wine/unicode.h"
#include "olestd.h"

#include "wine/list.h"

#include "windef.h"
#include "winbase.h"
#include "wtypes.h"
#include "dcom_p.h"
#include "winreg.h"
#include "winternl.h"

struct apartment;
typedef struct apartment APARTMENT;
typedef struct LocalServer LocalServer;

/*
 * This is a marshallable object exposing registered local servers.
 * IServiceProvider is used only because it happens meet requirements
 * and already has proxy/stub code. If more functionality is needed,
 * a custom interface may be used instead.
 */
struct LocalServer
{
    PVOID IServiceProvider_iface;//IServiceProvider IServiceProvider_iface; Modified to not implement really functions
    LONG ref;
    APARTMENT *apt;
    IStream *marshal_stream;
};

struct apartment
{
  struct list entry;

  LONG  refs;              /* refcount of the apartment (LOCK) */
  BOOL multi_threaded;     /* multi-threaded or single-threaded apartment? (RO) */
  DWORD tid;               /* thread id (RO) */
  OXID oxid;               /* object exporter ID (RO) */
  LONG ipidc;              /* interface pointer ID counter, starts at 1 (LOCK) */
  CRITICAL_SECTION cs;     /* thread safety */
  struct list proxies;     /* imported objects (CS cs) */
  struct list stubmgrs;    /* stub managers for exported objects (CS cs) */
  BOOL remunk_exported;    /* has the IRemUnknown interface for this apartment been created yet? (CS cs) */
  LONG remoting_started;   /* has the RPC system been started for this apartment? (LOCK) */
  struct list psclsids;    /* list of registered PS CLSIDs (CS cs) */
  struct list loaded_dlls; /* list of dlls loaded by this apartment (CS cs) */
  DWORD host_apt_tid;      /* thread ID of apartment hosting objects of differing threading model (CS cs) */
  HWND host_apt_hwnd;      /* handle to apartment window of host apartment (CS cs) */
  LocalServer *local_server; /* A marshallable object exposing local servers (CS cs) */

  /* FIXME: OIDs should be given out by RPCSS */
  OID oidc;                /* object ID counter, starts at 1, zero is invalid OID (CS cs) */

  /* STA-only fields */
  HWND win;                /* message window (LOCK) */
  LPMESSAGEFILTER filter;  /* message filter (CS cs) */
  BOOL main;               /* is this a main-threaded-apartment? (RO) */
};

/* this is what is stored in TEB->ReservedForOle */
struct oletls
{
    struct apartment *apt;
    IErrorInfo       *errorinfo;   /* see errorinfo.c */
    IUnknown         *state;       /* see CoSetState */
    DWORD             apt_mask;    /* apartment mask (+0Ch on x86) */
    IInitializeSpy   *spy;         /* The "SPY" from CoInitializeSpy */
    DWORD            inits;        /* number of times CoInitializeEx called */
    DWORD            ole_inits;    /* number of times OleInitialize called */
    GUID             causality_id; /* unique identifier for each COM call */
    LONG             pending_call_count_client; /* number of client calls pending */
    LONG             pending_call_count_server; /* number of server calls pending */
    DWORD            unknown;
    IObjContext     *context_token; /* (+38h on x86) */
    IUnknown        *call_state;    /* current call context (+3Ch on x86) */
    DWORD            unknown2[46];
    IUnknown        *cancel_object; /* cancel object set by CoSetCancelObject (+F8h on x86) */
};

typedef enum  { 
  APTTYPEQUALIFIER_NONE                = 0,
  APTTYPEQUALIFIER_IMPLICIT_MTA        = 1,
  APTTYPEQUALIFIER_NA_ON_MTA           = 2,
  APTTYPEQUALIFIER_NA_ON_STA           = 3,
  APTTYPEQUALIFIER_NA_ON_IMPLICIT_MTA  = 4,
  APTTYPEQUALIFIER_NA_ON_MAINSTA       = 5
} APTTYPEQUALIFIER;

/* will create if necessary */
static inline struct oletls *COM_CurrentInfo(void)
{
    if (!NtCurrentTeb()->ReservedForOle)
        NtCurrentTeb()->ReservedForOle = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct oletls));

    return NtCurrentTeb()->ReservedForOle;
}

WINE_DEFAULT_DEBUG_CHANNEL(ole32);

/***********************************************************************
 *           CoGetApartmentType [OLE32.@]
 */
HRESULT WINAPI CoGetApartmentType(APTTYPE *type, APTTYPEQUALIFIER *qualifier)
{
    struct oletls *info = COM_CurrentInfo();

    //FIXME("(%p, %p): semi-stub\n", type, qualifier);

    if (!type || !qualifier)
        return E_INVALIDARG;

    if (!info)
        return E_OUTOFMEMORY;

    if (!info->apt)
        *type = APTTYPE_CURRENT;
    else if (info->apt->multi_threaded)
        *type = APTTYPE_MTA;
    else if (info->apt->main)
        *type = APTTYPE_MAINSTA;
    else
        *type = APTTYPE_STA;

    *qualifier = APTTYPEQUALIFIER_NONE;

    return info->apt ? S_OK : CO_E_NOTINITIALIZED;
}


HRESULT 
WINAPI
CoDisconnectContext(
  _In_ DWORD dwTimeout
)
{
	return S_OK;
}
