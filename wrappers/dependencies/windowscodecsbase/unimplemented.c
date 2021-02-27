/*
 * Copyright 2010 Vincent Povirk for CodeWeavers
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

#include "wincodecs_private.h"

HRESULT WINAPI IEnumString_Next_WIC_Proxy(
  _In_  IEnumString *THIS_PTR
)
{
	return S_OK;
}

HRESULT WINAPI IEnumString_Reset_WIC_Proxy(
  _In_   IEnumString *THIS_PTR,
  _In_   ULONG celt,
  _Out_  LPOLESTR *rgelt,
  _Out_  ULONG *pceltFetched
)
{
	return S_OK;
}

HRESULT WINAPI WICGetMetadataContentSize(
  _In_   REFGUID guidContainerFormat,
  _In_   IWICMetadataWriter *pIWriter,
  _Out_  ULARGE_INTEGER *pcbSize
)
{
	return S_OK;
}

HRESULT WINAPI WICMatchMetadataContent(
  _In_   REFGUID guidContainerFormat,
  _In_   const GUID *pguidVendor,
  _In_   IStream *pIStream,
  _Out_  GUID *pguidMetadataFormat
)
{
	return S_OK;
}

HRESULT WINAPI WICSerializeMetadataContent(
  _In_  REFGUID guidContainerFormat,
  _In_  IWICMetadataWriter *pIWriter,
  _In_  DWORD dwPersistOptions,
  _In_  IStream *pIStream
)
{
	return S_OK;
}