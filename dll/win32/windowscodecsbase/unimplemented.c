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

HRESULT WINAPI WICCreateBitmapFromSection(
  _In_   UINT width,
  _In_   UINT height,
  REFWICPixelFormatGUID pixelFormat,
  _In_   HANDLE hSection,
  _In_   UINT stride,
  _In_   UINT offset,
  _Out_  IWICBitmap **pIBitmap
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


HRESULT WINAPI WICMapGuidToShortName(
  _In_     REFGUID guid,
  _In_     UINT cchName,
  _Inout_  WCHAR *wzName,
  _Out_    UINT *pcchActual
)
{
	return S_OK;
}

HRESULT WINAPI WICMapSchemaToName(
  _In_     REFGUID guidMetadataFormat,
  _In_     LPWSTR pwzSchema,
  _In_     UINT cchName,
  _Inout_  WCHAR *wzName,
  _Out_    UINT pcchActual
)
{
	return S_OK;
}

HRESULT WINAPI WICMapShortNameToGuid(
  _In_   const WCHAR *wzName,
  _Out_  GUID *pguid
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