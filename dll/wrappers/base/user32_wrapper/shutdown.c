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

BOOL WINAPI ShutdownBlockReasonDestroy(HWND hWnd)
{
  BOOL result; // eax@2
  BOOL verification; // edi@3
  DWORD flags; // eax@4
  PCSR_API_MESSAGE message = NULL; // [sp+4h] [bp-50h]@3
  NTSTATUS NtStatus = STATUS_SUCCESS; // [sp+24h] [bp-30h]@3

  if (hWnd)
  {
    //CsrClientCallServer(message, 0, 197643, 16);
    verification = NtStatus >= 0;
    if ( NtStatus < 0 )
    {
      flags = RtlNtStatusToDosError(NtStatus);
      SetLastError(flags);
    }
    result = verification;
  }
  else
  {
    SetLastError(0x57u);
    result = 0;
  }
  return result;
}


BOOL WINAPI ShutdownBlockReasonCreate(HWND hWnd, LPCWSTR pwszReason)
{
  size_t size; // esi@5
  PCSR_CAPTURE_BUFFER buffer; // ebx@5
  DWORD flags; // [sp-10h] [bp-68h]@7
  PCSR_API_MESSAGE message = NULL; // [sp+0h] [bp-58h]@6
  NTSTATUS NtStatus = STATUS_SUCCESS; // [sp+20h] [bp-38h]@6
  HWND window; // [sp+2Ch] [bp-2Ch]@6
  size_t cchDest; // [sp+30h] [bp-28h]@6
  STRSAFE_LPWSTR pszDest; // [sp+34h] [bp-24h]@6
  size_t pcchLength; // [sp+50h] [bp-8h]@3
  BOOL verification; // [sp+54h] [bp-4h]@1

  verification = 0;
  buffer = 0;
  if ( StringCchLengthW(pwszReason, 0x7FFFFFFEu, &pcchLength) >= 0 && pcchLength <= 0x100 )
  {
    size = pcchLength + 1;
    buffer = CsrAllocateCaptureBuffer(1, (2 * (pcchLength + 1) + 3) & 0xFFFFFFFC);
    if ( buffer )
    {
      window = hWnd;
      cchDest = size;
      CsrAllocateMessagePointer(buffer, 2 * size, &pszDest);
      StringCchCopyW(pszDest, size, pwszReason);
      // CsrClientCallServer(message, buffer, KUSER_SHARED_SYSCALL_RET, 16);
      CsrFreeCaptureBuffer(buffer);
      verification = NtStatus >= 0;
      if ( !(NtStatus < 0) )
        return verification;
      flags = RtlNtStatusToDosError(NtStatus);
    }
    else
    {
      flags = 14;
    }
    SetLastError(flags);
    return verification;
  }
  SetLastError(0x57u);
  return 0;
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