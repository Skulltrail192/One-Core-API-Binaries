/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    console.c

Abstract:

    This module implements Win32 console functions

Author:

    Skulltrail 30-April-2017

Revision History:

--*/

#include "main.h"
#include "csrss.h"
#include <conmsg.h>

BOOL
WINAPI
DECLSPEC_HOTPATCH
SetConsoleFont(IN HANDLE hConsoleOutput,
               IN DWORD nFont);

/*--------------------------------------------------------------
 *  SetConsoleHistoryInfo
 *
 * @implemented
 */
BOOL
WINAPI
SetConsoleHistoryInfo(
	IN PCONSOLE_HISTORY_INFO lpConsoleHistoryInfo
)
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
 *  GetConsoleHistoryInfo
 *
 * @implemented - new
 */
BOOL
WINAPI
GetConsoleHistoryInfo(
	PCONSOLE_HISTORY_INFO lpConsoleHistoryInfo
)
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

BOOL 
WINAPI 
GetConsoleScreenBufferInfoEx(
	HANDLE hConsoleOutput, 
	CONSOLE_SCREEN_BUFFER_INFOEX *ConsoleScreenBufferInfoEx
)
{
  BOOL result; 
  CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo; 

  ConsoleScreenBufferInfoEx->cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
  if ( GetConsoleScreenBufferInfo(hConsoleOutput, &ConsoleScreenBufferInfo) )
  {
    ConsoleScreenBufferInfoEx->dwSize.X = ConsoleScreenBufferInfo.dwSize.X;
    ConsoleScreenBufferInfoEx->dwCursorPosition.X = ConsoleScreenBufferInfo.dwCursorPosition.X;
    ConsoleScreenBufferInfoEx->wAttributes = ConsoleScreenBufferInfo.wAttributes;
    ConsoleScreenBufferInfoEx->srWindow.Left = ConsoleScreenBufferInfo.srWindow.Left;
    ConsoleScreenBufferInfoEx->srWindow.Right = ConsoleScreenBufferInfo.srWindow.Right;
    ConsoleScreenBufferInfoEx->dwMaximumWindowSize.X = ConsoleScreenBufferInfo.dwMaximumWindowSize.X;
    result = TRUE;
  }
  else
  {
    result = FALSE;
  }
  return result;
}

/*--------------------------------------------------------------
 *  SetConsoleScreenBufferInfoEx
 *
 * @implemented - new
 */
BOOL
WINAPI
SetConsoleScreenBufferInfoEx(
	HANDLE hConsoleOutput, 
	PCONSOLE_SCREEN_BUFFER_INFOEX lpConsoleScreenBufferInfoEx
)
{
     CONSOLE_API_MESSAGE ApiMessage;
     PCONSOLE_GETSCREENBUFFERINFO ScreenBufferInfoRequest = &ApiMessage.Data.ScreenBufferInfoRequest;
	
	  if ( lpConsoleScreenBufferInfoEx->cbSize != sizeof(CONSOLE_SCREEN_BUFFER_INFOEX) )
	  {
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	  }	

    ScreenBufferInfoRequest->ConsoleHandle = NtCurrentPeb()->ProcessParameters->ConsoleHandle;
    ScreenBufferInfoRequest->OutputHandle  = hConsoleOutput;

    ScreenBufferInfoRequest->ScreenBufferSize 	   = lpConsoleScreenBufferInfoEx->dwSize;
    ScreenBufferInfoRequest->CursorPosition   	   = lpConsoleScreenBufferInfoEx->dwCursorPosition;
    ScreenBufferInfoRequest->Attributes            = lpConsoleScreenBufferInfoEx->wAttributes;
    ScreenBufferInfoRequest->ViewOrigin.X          = lpConsoleScreenBufferInfoEx->srWindow.Left;
    ScreenBufferInfoRequest->ViewOrigin.Y          = lpConsoleScreenBufferInfoEx->srWindow.Top;
    ScreenBufferInfoRequest->MaximumViewSize = lpConsoleScreenBufferInfoEx->dwMaximumWindowSize;
	
    CsrClientCallServer((PCSR_API_MESSAGE)&ApiMessage,
                        NULL,
                        (CSR_API_NUMBER)0x20258,//Hack because reactos doesn't have imlemented CSR_CREATE_API_NUMBER(CONSRV_SERVERDLL_INDEX, ConsolepSetScreenBufferInfo),
                        sizeof(*ScreenBufferInfoRequest));	
						
    if (!NT_SUCCESS(ApiMessage.Status))
    {
        BaseSetLastNTError(ApiMessage.Status);
        return FALSE;
    }						
						
    return TRUE; 
}

DWORD 
WINAPI 
GetConsoleOriginalTitleA(
  _Out_  LPTSTR lpConsoleTitle,
  _In_   DWORD nSize
)
{
	return GetConsoleTitleA(lpConsoleTitle, nSize);
}

DWORD 
WINAPI 
GetConsoleOriginalTitleW(
  _Out_  LPWSTR lpConsoleTitle,
  _In_   DWORD nSize
)
{
	return GetConsoleTitleW(lpConsoleTitle, nSize);
}

BOOL
WINAPI
GetCurrentConsoleFontEx(
	IN HANDLE hConsoleOutput,
    IN BOOL bMaximumWindow,
    OUT PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx
)
{
	CONSOLE_FONT_INFO lpConsoleCurrentFont;
	
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	
	if(GetCurrentConsoleFont(hConsoleOutput, bMaximumWindow, &lpConsoleCurrentFont)){
		lpConsoleCurrentFontEx->nFont = lpConsoleCurrentFont.nFont;
		lpConsoleCurrentFontEx->dwFontSize = lpConsoleCurrentFont.dwFontSize;
		lpConsoleCurrentFontEx->FontWeight = 400;
		wcscpy(lpConsoleCurrentFontEx->FaceName, L"Raster Fonts");
		lpConsoleCurrentFontEx->FontFamily = FF_DONTCARE;
		return TRUE;
	}else{
		return FALSE;
	}
}

BOOL 
WINAPI 
SetCurrentConsoleFontEx(
	HANDLE hConsoleOutput, 
	BOOL bMaximumWindow, 
	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx
)
{
	return SetConsoleFont(hConsoleOutput, lpConsoleCurrentFontEx->nFont);
}