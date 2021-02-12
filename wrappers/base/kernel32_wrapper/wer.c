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

VOID 
WINAPI 
ApplicationRecoveryFinished(
  _In_  BOOL bSuccess
)
{
	bSuccess = TRUE;
}

HRESULT 
WINAPI
ApplicationRecoveryInProgress(
	PBOOL pbCancelled
)
{
  *pbCancelled = TRUE;
  return S_OK;
}

/*
* @unimplemented
*/
void 
WINAPI 
WerpNotifyUseStringResource(HANDLE file)
{
	UNIMPLEMENTED;
}

void 
WINAPI 
WerpNotifyLoadStringResource(
	HMODULE module, 
	LPCWSTR string, 
	HANDLE handle, 
	DWORD flags
)
{
	UNIMPLEMENTED;
}

/***********************************************************************
* WerSetFlags (KERNEL32.@)
*
* Sets error reporting flags for the current process.
*/
HRESULT WINAPI WerSetFlags(DWORD flags)
{
     FIXME("(%d) stub!\n", flags);
     return E_NOTIMPL;
}


HRESULT 
WINAPI 
WerUnregisterFile(
  _In_ PCWSTR pwzFilePath
)
{
	return S_OK;
}

/***********************************************************************
  * WerRegisterRuntimeExceptionModule (KERNEL32.@)
  *
  * Register a custom runtime exception handler.
  */
HRESULT WINAPI WerRegisterRuntimeExceptionModule(
  _In_     PCWSTR pwszOutOfProcessCallbackDll,
  _In_opt_ PVOID  pContext
)
{
	FIXME("(%s, %p) stub!\n", debugstr_w(pwszOutOfProcessCallbackDll), pContext);
	return S_OK;
}

/***********************************************************************
  * WerUnRegisterRuntimeExceptionModule (KERNEL32.@)
  *
  * UnRegister a custom runtime exception handler.
  */
HRESULT WINAPI WerUnregisterRuntimeExceptionModule(
  _In_     PCWSTR pwszOutOfProcessCallbackDll,
  _In_opt_ PVOID  pContext
)
{
	FIXME("(%s, %p) stub!\n", debugstr_w(pwszOutOfProcessCallbackDll), pContext);
	return S_OK;
}

/***********************************************************************
  * WerUnregisterMemoryBlock (KERNEL32.@)
8  */
HRESULT 
WINAPI 
WerUnregisterMemoryBlock(void *block)
{
     FIXME("(%p) stub\n", block);
     return E_NOTIMPL;
}

/*
* @unimplemented
*/
HRESULT 
WINAPI 
WerGetFlags(
  _In_   HANDLE hProcess,
  _Out_  PDWORD pdwFlags
)
{
	*pdwFlags = WER_FAULT_REPORTING_FLAG_NOHEAP;	
	return S_OK;
}
