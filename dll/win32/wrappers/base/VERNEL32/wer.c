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