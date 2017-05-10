/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    wer.c

Abstract:

    This module implements Windows Error Reporting functions for the Win32 APIs.

Author:

    Skulltrail 22-March-2017

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

/*
 * @unimplemented
*/
HRESULT 
WINAPI 
GetApplicationRestartSettings(
  _In_       HANDLE hProcess,
  _Out_opt_  PWSTR pwzCommandline,
  _Inout_    PDWORD pcchSize,
  _Out_opt_  PDWORD pdwFlags
)
{
	pwzCommandline = NULL;
	pcchSize = 0;
	return S_OK;
}

/***********************************************************************
 * WerRegisterMemoryBlock (KERNEL32.@)
 */
HRESULT 
WINAPI 
WerRegisterMemoryBlock(
	void *block, 
	DWORD size
)
{
    return E_NOTIMPL;
}

/***********************************************************************
 * WerRegisterFile (KERNEL32.@)
 *
 * Register a file to be included in WER error report.
 */
HRESULT 
WINAPI 
WerRegisterFile(
	PCWSTR file, 
	WER_REGISTER_FILE_TYPE regfiletype, 
	DWORD flags
)
{
    FIXME("(%s, %d, %d) stub!\n", debugstr_w(file), regfiletype, flags);
    return E_NOTIMPL;
}