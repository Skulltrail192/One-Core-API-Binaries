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

#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>
#include <ntsecapi.h>
#include <bcrypt.h>
#include <wingdi.h>
#include <math.h>
#include <dll/desk/deskcplx.h>

typedef HANDLE D3DKMT_HANDLE;

typedef struct _D3DKMT_CLOSEADAPTER {
  D3DKMT_HANDLE hAdapter;
} D3DKMT_CLOSEADAPTER;

typedef struct _D3DKMT_OPENADAPTERFROMDEVICENAME {
  PCWSTR        pDeviceName;
  D3DKMT_HANDLE hAdapter;
  LUID          AdapterLuid;
} D3DKMT_OPENADAPTERFROMDEVICENAME;

NTSTATUS APIENTRY D3DKMTCloseAdapter(
  _In_ D3DKMT_CLOSEADAPTER *pData
)
{
	if(!pData->hAdapter)
	{
		return STATUS_INVALID_HANDLE;
	}
	pData->hAdapter = NULL;
	return STATUS_SUCCESS;
}