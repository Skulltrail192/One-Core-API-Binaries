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
#include <asm.h>

BOOL WINAPI ShutdownBlockReasonQuery(HWND hWnd, LPWSTR pwszBuff, DWORD *pcchBuff)
{
  int count; // ebx@1
  DWORD flags; // esi@5
  BOOL compare; // esi@13
  DWORD other; // eax@17
  DWORD otherOther; // [sp-4h] [bp-60h]@9
  PCSR_API_MESSAGE verification = NULL; // [sp+8h] [bp-54h]@13
  NTSTATUS NtStatus = 0x00000000; // [sp+28h] [bp-34h]@13
  HWND window; // [sp+34h] [bp-28h]@10
  DWORD buffer; // [sp+38h] [bp-24h]@10
  STRSAFE_LPCWSTR pszSrc = L""; // [sp+3Ch] [bp-20h]@11
  PCSR_CAPTURE_BUFFER pointer; // [sp+58h] [bp-4h]@1

  count = 0;
  pointer = 0;
  if (!pcchBuff || pwszBuff && !*pcchBuff )
  {
    otherOther = 87;
    goto LABEL_22;
  }
  flags = *pcchBuff;
  if ( *pcchBuff > 0x101 )
    flags = 257;
  if ( pwszBuff )
  {
    count = 2 * flags;
    pointer = CsrAllocateCaptureBuffer(1, (2 * flags + 3) & 0xFFFFFFFC);
    if ( !pointer )
    {
      otherOther = 14;
LABEL_22:
      SetLastError(otherOther);
      return 0;
    }
  }
  window = hWnd;
  buffer = flags;
  if ( pointer )
    CsrAllocateMessagePointer(pointer, count, pszSrc);
  else
    pszSrc = 0;
  CsrClientCallServer(verification, pointer, 197643, 16);
  compare = NtStatus >= 0;
  if ( NtStatus < 0 )
  {
    other = RtlNtStatusToDosError(NtStatus);
    SetLastError(other);
  }
  else
  {
    if ( pointer )
      StringCchCopyW(pwszBuff, *pcchBuff, pszSrc);
    *pcchBuff = buffer;
  }
  if ( pointer )
    CsrFreeCaptureBuffer(pointer);
  return compare;
}

/***********************************************************************
 *		ShutdownBlockReasonCreate (USER32.@)
 */
BOOL WINAPI ShutdownBlockReasonCreate(HWND hwnd, LPCWSTR reason)
{
    DbgPrint("ShutdownBlockReasonCreate (%p, %s): stub\n", hwnd, debugstr_w(reason));
    SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
    return FALSE;
}

/***********************************************************************
 *		ShutdownBlockReasonDestroy (USER32.@)
 */
BOOL WINAPI ShutdownBlockReasonDestroy(HWND hwnd)
{
    DbgPrint("ShutdownBlockReasonDestroy (%p): stub\n", hwnd);
    SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
    return FALSE;
}

BOOL WINAPI CancelShutdown()
{
  DWORD error; // eax@2
  BOOL result; // eax@2
  PCSR_API_MESSAGE message = NULL; // [sp+0h] [bp-E0h]@1
  NTSTATUS NtStatus = 0; // [sp+20h] [bp-C0h]@1

  CsrClientCallServer(message, 0, 197642, 0);
  if ( NtStatus >= 0 )
  {
    result = 1;
  }
  else
  {
	error = 87;
    SetLastError(error);
    result = 0;
  }
  return result;
}

ULONG WINAPI ShutdownWindows(BOOL reason)
{
  NTSTATUS status = 2;// eax@1
  ULONG result; // eax@2
  DWORD error; // esi@4
  DWORD compose; // eax@5
  BOOL verification;

  if ( status >= 0 )
  {
    do
      verification = reason & 4 || reason & 0x10;
    while ( EndTask(0, (reason & 0xB) != 0, verification) );
    error = GetLastError();
    if ( error == 259 )
    {
      error = 0;
      compose = 0;
    }
    else
    {
      compose = 0xC0000120;
    }
    result = error;
  }
  else
  {
    result = RtlNtStatusToDosError(status);
  }
  return result;
}