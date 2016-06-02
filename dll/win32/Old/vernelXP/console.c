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
#include "csrss.h"

/*--------------------------------------------------------------
 *  SetConsoleHistoryInfo
 *
 * @implemented
 */
BOOL
WINAPI
SetConsoleHistoryInfo(IN PCONSOLE_HISTORY_INFO lpConsoleHistoryInfo)
{
    CSR_API_MESSAGE_KERNEL Request;
    ULONG CsrRequest = MAKE_CSR_API(GET_HISTORY_INFO, CSR_CONSOLE);
    NTSTATUS Status;
    if (lpConsoleHistoryInfo->cbSize != sizeof(CONSOLE_HISTORY_INFO))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    Request.Data.SetHistoryInfo.HistoryBufferSize      = lpConsoleHistoryInfo->HistoryBufferSize;
    Request.Data.SetHistoryInfo.NumberOfHistoryBuffers = lpConsoleHistoryInfo->NumberOfHistoryBuffers;
    Request.Data.SetHistoryInfo.dwFlags                = lpConsoleHistoryInfo->dwFlags;
    Status = CsrClientCallServer((PCSR_API_MESSAGE)&Request, NULL, CsrRequest, sizeof(CSR_API_MESSAGE));
    if (!NT_SUCCESS(Status) || !NT_SUCCESS(Status = Request.Status))
    {
        SetLastError(Status);
        return FALSE;
    }
    return TRUE;
}

/*--------------------------------------------------------------
 *  GetCurrentConsoleFontEx
 *
 * @implemented - new
 */
BOOL
WINAPI
GetCurrentConsoleFontEx(IN HANDLE hConsoleOutput,
                        IN BOOL bMaximumWindow,
                        OUT PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx)
{
  DWORD status; // [sp-4h] [bp-ACh]@2
  CSR_API_MESSAGE server; // [sp+4h] [bp-A4h]@3
  NTSTATUS NtStatus = 0; // [sp+24h] [bp-84h]@3
  int read; // [sp+2Ch] [bp-7Ch]@3
  HANDLE handle; // [sp+30h] [bp-78h]@3
  char maxi; // [sp+34h] [bp-74h]@3
  DWORD font = 0; // [sp+38h] [bp-70h]@4
  DWORD size = 0; // [sp+3Ch] [bp-6Ch]@4
  UINT family = 0; // [sp+40h] [bp-68h]@4
  UINT wight = 0; // [sp+44h] [bp-64h]@4
  const wchar_t pszSrc; // [sp+48h] [bp-60h]@4

  if ( lpConsoleCurrentFontEx->cbSize == 84 )
  {
    #ifdef _M_IX86
		read = *(DWORD *)(*(DWORD *)(*(DWORD *)(__readfsdword(24) + 48) + 16) + 16);
	#elif defined(_M_AMD64)
		read = *(DWORD *)(*(DWORD *)(*(DWORD *)(__readgsqword(24) + 48) + 16) + 16);
	#endif
    maxi = bMaximumWindow;
    handle = hConsoleOutput;
    CsrClientCallServer(&server, 0, 131600, 92);
    if ( NtStatus >= 0 )
    {
      *(DWORD *)&lpConsoleCurrentFontEx->dwFontSize.X = size;
      lpConsoleCurrentFontEx->nFont = font;
      lpConsoleCurrentFontEx->FontFamily = family;
      lpConsoleCurrentFontEx->FontWeight = wight;
      StringCchCopyW(lpConsoleCurrentFontEx->FaceName, 0x20u, &pszSrc);
      return 1;
    }
    status = RtlNtStatusToDosError(NtStatus);
  }
  else
  {
    status = 87;
  }
  SetLastError(status);
  return 0;
}

/*--------------------------------------------------------------
 *  SetConsoleScreenBufferInfoEx
 *
 * @implemented - new
 */
BOOL
WINAPI
SetConsoleScreenBufferInfoEx(HANDLE hConsoleOutput, PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx)
{
  ULONG resp; // [sp-4h] [bp-A8h]@2
  CSR_API_MESSAGE message; // [sp+0h] [bp-A4h]@5
  NTSTATUS NtStatus = 0; // [sp+20h] [bp-84h]@5
  unsigned long position; // [sp+28h] [bp-7Ch]@5
  HANDLE output; // [sp+2Ch] [bp-78h]@5
  SHORT indexx; // [sp+30h] [bp-74h]@5
  SHORT cursor; // [sp+34h] [bp-70h]@5
  WORD atributes; // [sp+3Ch] [bp-68h]@5
  SMALL_RECT various; // [sp+3Eh] [bp-66h]@5
  WORD popup; // [sp+46h] [bp-5Eh]@5
  char copy; // [sp+48h] [bp-5Ch]@5

  if ( lpConsoleScreenBufferInfoEx->cbSize != 92 )
  {
    resp = 87;
    return 0;
  }
  #ifdef _M_IX86
		 position = *(DWORD *)(*(DWORD *)(*(DWORD *)(__readfsdword(24) + 48) + 16) + 16);
  #elif defined(_M_AMD64)
		position = *(DWORD *)(*(DWORD *)(*(DWORD *)(__readgsqword(24) + 48) + 16) + 16);
  #endif
  output = hConsoleOutput;
  indexx = *&lpConsoleScreenBufferInfoEx->dwSize.X;
  cursor = *&lpConsoleScreenBufferInfoEx->dwCursorPosition.X;
  atributes = lpConsoleScreenBufferInfoEx->wAttributes;
  popup = lpConsoleScreenBufferInfoEx->wPopupAttributes;
  various.Left = lpConsoleScreenBufferInfoEx->srWindow.Right - lpConsoleScreenBufferInfoEx->srWindow.Left;
  various.Top = lpConsoleScreenBufferInfoEx->srWindow.Bottom - lpConsoleScreenBufferInfoEx->srWindow.Top;
  *&various.Right = *&lpConsoleScreenBufferInfoEx->dwMaximumWindowSize.X;
  memcpy(&copy, &lpConsoleScreenBufferInfoEx->bFullscreenSupported, 0x40u);
  CsrClientCallServer(&message, 0, 131671, 96);
  if ( NtStatus < 0 )
  {
    resp = RtlNtStatusToDosError(NtStatus);
    resp = 87;
    return 0;
  }
  return 1;
}

/*--------------------------------------------------------------
 *  GetConsoleHistoryInfo
 *
 * @implemented - new
 */
BOOL
WINAPI
GetConsoleHistoryInfo(PCONSOLE_HISTORY_INFO lpConsoleHistoryInfo)
{
    CSR_API_MESSAGE_KERNEL Request;
    ULONG CsrRequest = MAKE_CSR_API(GET_HISTORY_INFO, CSR_CONSOLE);
    NTSTATUS Status;
    if (lpConsoleHistoryInfo->cbSize != sizeof(CONSOLE_HISTORY_INFO))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    Status = CsrClientCallServer((PCSR_API_MESSAGE)&Request, NULL, CsrRequest, sizeof(CSR_API_MESSAGE));
    if (!NT_SUCCESS(Status) || !NT_SUCCESS(Status = Request.Status))
    {
        SetLastError(Status);
        return FALSE;
    }
    lpConsoleHistoryInfo->HistoryBufferSize      = Request.Data.GetHistoryInfo.HistoryBufferSize;
    lpConsoleHistoryInfo->NumberOfHistoryBuffers = Request.Data.GetHistoryInfo.NumberOfHistoryBuffers;
    lpConsoleHistoryInfo->dwFlags                = Request.Data.GetHistoryInfo.dwFlags;
    return TRUE;
}

DWORD WINAPI GetConsoleOriginalTitleA(
  _Out_  LPTSTR lpConsoleTitle,
  _In_   DWORD nSize
)
{
	return GetConsoleTitleA(lpConsoleTitle, nSize);
}

DWORD WINAPI GetConsoleOriginalTitleW(
  _Out_  LPWSTR lpConsoleTitle,
  _In_   DWORD nSize
)
{
	return GetConsoleTitleW(lpConsoleTitle, nSize);
}

BOOL WINAPI GetConsoleScreenBufferInfoEx(HANDLE hConsoleOutput, PCONSOLE_SCREEN_BUFFER_INFOEX ConsoleScreenBufferInfoEx)
{
  BOOL result; // eax@2
  struct _CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo; // [sp+0h] [bp-60h]@1

  ConsoleScreenBufferInfoEx->cbSize = 96;
  if ( GetConsoleScreenBufferInfo(hConsoleOutput, &ConsoleScreenBufferInfo) )
  {
    ConsoleScreenBufferInfoEx->dwSize.X = ConsoleScreenBufferInfo.dwSize.X;
    ConsoleScreenBufferInfoEx->dwCursorPosition.X = ConsoleScreenBufferInfo.dwCursorPosition.X;
    ConsoleScreenBufferInfoEx->wAttributes = ConsoleScreenBufferInfo.wAttributes;
    ConsoleScreenBufferInfoEx->srWindow.Left = ConsoleScreenBufferInfo.srWindow.Left;
    ConsoleScreenBufferInfoEx->srWindow.Right = ConsoleScreenBufferInfo.srWindow.Right;
    ConsoleScreenBufferInfoEx->dwMaximumWindowSize.X = ConsoleScreenBufferInfo.dwMaximumWindowSize.X;
    result = 1;
  }
  else
  {
    result = 0;
  }
  return result;
}

BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx)
{
  DWORD error; // [sp-4h] [bp-A8h]@2
  PCSR_API_MESSAGE message = NULL; // [sp+0h] [bp-A4h]@3
  NTSTATUS NtStatus = 0x00000000; // [sp+20h] [bp-84h]@3
  DWORD version; // [sp+28h] [bp-7Ch]@3
  HANDLE handle; // [sp+2Ch] [bp-78h]@3
  BOOL verification; // [sp+30h] [bp-74h]@3
  DWORD font; // [sp+34h] [bp-70h]@3
  SHORT size; // [sp+38h] [bp-6Ch]@3
  UINT family; // [sp+3Ch] [bp-68h]@3
  UINT weght; // [sp+40h] [bp-64h]@3
  wchar_t pszDest; // [sp+44h] [bp-60h]@3

  if ( lpConsoleCurrentFontEx->cbSize == 84 )
  {
    version = (__readfsdword(24) + 48);
	#ifdef _M_IX86
		version = (__readfsdword(24) + 48);
	#elif defined(_M_AMD64)
		version = (__readgsqword(24) + 48);
	#endif 
    handle = hConsoleOutput;
    verification = bMaximumWindow;
    size = lpConsoleCurrentFontEx->dwFontSize.X;
    font = lpConsoleCurrentFontEx->nFont;
    family = lpConsoleCurrentFontEx->FontFamily;
    weght = lpConsoleCurrentFontEx->FontWeight;
    StringCchCopyW(&pszDest, 0x20u, lpConsoleCurrentFontEx->FaceName);
    CsrClientCallServer(message, 0, 131671, 92);
    if ( NtStatus >= 0 )
      return 1;
    error = RtlNtStatusToDosError(NtStatus);
  }
  else
  {
    error = 87;
  }
  SetLastError(error);
  return 0;
}