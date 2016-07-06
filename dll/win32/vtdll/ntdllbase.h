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

#include "config.h"
#include <assert.h>
#include <rtl.h>
#include <evntprov.h>

typedef DWORD WINAPI RTL_RUN_ONCE_INIT_FN(PRTL_RUN_ONCE, PVOID, PVOID*);
typedef RTL_RUN_ONCE_INIT_FN *PRTL_RUN_ONCE_INIT_FN;

ULONG WINAPI EtwEventUnregister(
  _In_  REGHANDLE RegHandle
);

ULONG
WINAPI
EtwEventRegister(
  IN LPCGUID ProviderId,
  IN PENABLECALLBACK EnableCallback OPTIONAL,
  IN PVOID CallbackContext OPTIONAL,
  OUT PREGHANDLE RegHandle);

BOOLEAN WINAPI EtwEventEnabled(
  _In_  REGHANDLE RegHandle,
  _In_  PCEVENT_DESCRIPTOR EventDescriptor
);

ULONG
WINAPI
EtwEventWrite(
  IN REGHANDLE RegHandle,
  IN PCEVENT_DESCRIPTOR EventDescriptor,
  IN ULONG UserDataCount,
  IN PEVENT_DATA_DESCRIPTOR UserData);
  
DWORD 
WINAPI 
RtlRunOnceBeginInitialize( 
	RTL_RUN_ONCE *once, 
	ULONG flags, 
	void **context 
);

DWORD 
WINAPI 
RtlRunOnceComplete( 
	RTL_RUN_ONCE *once, 
	ULONG flags, 
	void *context 
);
DWORD 
WINAPI 
RtlRunOnceExecuteOnce( 
	RTL_RUN_ONCE *once, 
	PRTL_RUN_ONCE_INIT_FN func, 
	void *param, 
	void **context
);

void* 
WINAPI 
LdrResolveDelayLoadedAPI( 
	void* base, 
	const IMAGE_DELAYLOAD_DESCRIPTOR* desc,
    PDELAYLOAD_FAILURE_DLL_CALLBACK dllhook, 
	void* syshook,
    IMAGE_THUNK_DATA* addr, 
	ULONG flags 
);