/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    hooks.c

Abstract:

    Hooks native functions to implement new features
	
Author:

    Skulltrail 05-December-2021

Revision History:

--*/

#include "main.h"

VOID
WINAPI
UserSetLastError(IN DWORD dwErrCode)
{
    /*
     * Equivalent of SetLastError in kernel32, but without breaking
     * into the debugger nor checking whether the last old error is
     * the same as the one we are going to set.
     */
    NtCurrentTeb()->LastErrorValue = dwErrCode;
}

VOID
WINAPI
UserSetLastNTError(IN NTSTATUS Status)
{
    /*
     * Equivalent of BaseSetLastNTError in kernel32, but using
     * UserSetLastError: convert from NT to Win32, then set.
     */
    UserSetLastError(RtlNtStatusToDosError(Status));
}


/*
 * GetUserObjectSecurity
 *
 * Retrieves security information for user object specified
 * with handle 'hObject'. Descriptor returned in self-relative
 * format.
 *
 * Arguments:
 *  1) hObject - handle to an object to retrieve information for
 *  2) pSecurityInfo - type of information to retrieve
 *  3) pSecurityDescriptor - buffer which receives descriptor
 *  4) dwLength - size, in bytes, of buffer 'pSecurityDescriptor'
 *  5) pdwLengthNeeded - reseives actual size of descriptor
 *
 * Return Vaules:
 *  TRUE on success
 *  FALSE on failure, call GetLastError() for more information
 */
/*
 * @implemented
 */
BOOL
WINAPI
GetUserObjectSecurityInternal(
    IN HANDLE hObject,
    IN PSECURITY_INFORMATION pSecurityInfo,
    OUT PSECURITY_DESCRIPTOR pSecurityDescriptor,
    IN DWORD dwLength,
    OUT PDWORD pdwLengthNeeded
)
{
    NTSTATUS Status;

    Status = NtQuerySecurityObject(hObject,
                                   *pSecurityInfo,
                                   pSecurityDescriptor,
                                   dwLength,
                                   pdwLengthNeeded);
    if (!NT_SUCCESS(Status))
    {
		DbgPrint("GetUserObjectSecurity::NtQuerySecurityObject returned Status: 0x%08lx\n", Status);
        UserSetLastNTError(Status);
        return FALSE;
    }

    return TRUE;
}

BOOL 
WINAPI
SystemParametersInfoWInternal(
	UINT uiAction,
	UINT uiParam,
	PVOID pvParam,
	UINT fWinIni)
{
  switch(uiAction)
  {
    case SPI_GETNONCLIENTMETRICS:
      {
          LPNONCLIENTMETRICSW lpnclt = (LPNONCLIENTMETRICSW)pvParam;
		  lpnclt->cbSize = sizeof(NONCLIENTMETRICSW);
          return SystemParametersInfoW(uiAction, lpnclt->cbSize, lpnclt, fWinIni);          
      }
	  case SPI_SETNONCLIENTMETRICS:
	  {
          LPNONCLIENTMETRICSW lpnclt = (LPNONCLIENTMETRICSW)pvParam;
		  lpnclt->cbSize = sizeof(NONCLIENTMETRICSW);
          return SystemParametersInfoW(uiAction, lpnclt->cbSize, lpnclt, fWinIni);  
	  }
	  default:
		return SystemParametersInfoW(uiAction, uiParam, pvParam, fWinIni);
	}
	return SystemParametersInfoW(uiAction, uiParam, pvParam, fWinIni);
}

BOOL 
WINAPI
SystemParametersInfoAInternal(
	UINT uiAction,
	UINT uiParam,
	PVOID pvParam,
	UINT fWinIni)
{
  switch(uiAction)
  {
    case SPI_GETNONCLIENTMETRICS:
      {
          LPNONCLIENTMETRICSA lpnclt = (LPNONCLIENTMETRICSA)pvParam;	  
		  lpnclt->cbSize = sizeof(NONCLIENTMETRICSA);       
		  return SystemParametersInfoA(uiAction, lpnclt->cbSize, lpnclt, fWinIni);          
      }
	  case SPI_SETNONCLIENTMETRICS:
	  {
          LPNONCLIENTMETRICSA lpnclt = (LPNONCLIENTMETRICSA)pvParam;	  
		  lpnclt->cbSize = sizeof(NONCLIENTMETRICSA);       
		  return SystemParametersInfoA(uiAction, lpnclt->cbSize, lpnclt, fWinIni); 		  
	  }
	  default:
		return SystemParametersInfoA(uiAction, uiParam, pvParam, fWinIni);
  }
  return SystemParametersInfoA(uiAction, uiParam, pvParam, fWinIni); 			   
}