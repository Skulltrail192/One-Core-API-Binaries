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

typedef struct tagNONCLIENTMETRICSA_LH {
	UINT		cbSize;
	INT		iBorderWidth;
	INT		iScrollWidth;
	INT		iScrollHeight;
	INT		iCaptionWidth;
	INT		iCaptionHeight;
	LOGFONTA	lfCaptionFont;
	INT		iSmCaptionWidth;
	INT		iSmCaptionHeight;
	LOGFONTA	lfSmCaptionFont;
	INT		iMenuWidth;
	INT		iMenuHeight;
	LOGFONTA	lfMenuFont;
	LOGFONTA	lfStatusFont;
	LOGFONTA	lfMessageFont;
	INT		iPaddedBorderWidth;
} NONCLIENTMETRICSA_LH, *PNONCLIENTMETRICSA_LH, *LPNONCLIENTMETRICSA_LH;

typedef struct tagNONCLIENTMETRICSW_LH {
	UINT		cbSize;
	INT		iBorderWidth;
	INT		iScrollWidth;
	INT		iScrollHeight;
	INT		iCaptionWidth;
	INT		iCaptionHeight;
	LOGFONTW	lfCaptionFont;
	INT		iSmCaptionWidth;
	INT		iSmCaptionHeight;
	LOGFONTW	lfSmCaptionFont;
	INT		iMenuWidth;
	INT		iMenuHeight;
	LOGFONTW	lfMenuFont;
	LOGFONTW	lfStatusFont;
	LOGFONTW	lfMessageFont;
	INT		iPaddedBorderWidth;
} NONCLIENTMETRICSW_LH, *PNONCLIENTMETRICSW_LH, *LPNONCLIENTMETRICSW_LH;

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
SystemParametersInfoWInternal(UINT uiAction,
		      UINT uiParam,
		      PVOID pvParam,
		      UINT fWinIni)
{
  BOOL resp = FALSE; 	
  switch(uiAction)
  {

    case SPI_GETNONCLIENTMETRICS:
      {
          //LPNONCLIENTMETRICSW srcnclt = (LPNONCLIENTMETRICSW)pvParam;          
		  
		  if(sizeof(pvParam) == sizeof(NONCLIENTMETRICSW_LH) || ((LPNONCLIENTMETRICSW_LH)pvParam)->cbSize == sizeof(NONCLIENTMETRICSA_LH)){
			  LPNONCLIENTMETRICSW_LH lpnclt = (LPNONCLIENTMETRICSW_LH)pvParam;	
			  LPNONCLIENTMETRICSW srcnclt = {0};
			  DbgPrint("SystemParametersInfoAInternal:: LPNONCLIENTMETRICSW_LH\n");			  
			  resp = SystemParametersInfo(uiAction, uiParam, srcnclt, fWinIni);			  
			  lpnclt->cbSize = sizeof(NONCLIENTMETRICSW_LH);
			  lpnclt->iBorderWidth = srcnclt->iBorderWidth;
			  lpnclt->iScrollWidth = srcnclt->iScrollWidth;
			  lpnclt->iScrollHeight = srcnclt->iScrollHeight;
			  lpnclt->iCaptionWidth = srcnclt->iCaptionWidth;
			  lpnclt->iCaptionHeight = srcnclt->iCaptionHeight;
			  lpnclt->lfCaptionFont = srcnclt->lfCaptionFont;
			  lpnclt->iSmCaptionWidth = srcnclt->iSmCaptionWidth;
			  lpnclt->iSmCaptionHeight = srcnclt->iSmCaptionHeight;
			  lpnclt->lfSmCaptionFont = srcnclt->lfSmCaptionFont;
			  lpnclt->iMenuWidth = srcnclt->iMenuWidth;
			  lpnclt->iMenuHeight = srcnclt->iMenuHeight;
			  lpnclt->lfMenuFont = srcnclt->lfMenuFont;
			  lpnclt->lfStatusFont = srcnclt->lfStatusFont;
			  lpnclt->lfMessageFont = srcnclt->lfMessageFont;
			  lpnclt->iPaddedBorderWidth = 0;
			  pvParam = lpnclt;
			  return resp;
		  }

          return SystemParametersInfoW(uiAction, uiParam, pvParam, fWinIni);          
      }
	  case SPI_SETNONCLIENTMETRICS:
	  {
		  if(sizeof(pvParam) == sizeof(NONCLIENTMETRICSW_LH) || ((LPNONCLIENTMETRICSW_LH)pvParam)->cbSize == sizeof(NONCLIENTMETRICSA_LH)){
			  LPNONCLIENTMETRICSW_LH srcnclt = (LPNONCLIENTMETRICSW_LH)pvParam;	
			  LPNONCLIENTMETRICSW dstnclt = {0};
			  // DbgPrint("SystemParametersInfoAInternal:: LPNONCLIENTMETRICSW_LH\n");
			  dstnclt->cbSize = sizeof(NONCLIENTMETRICSW);
			  dstnclt->iBorderWidth = srcnclt->iBorderWidth;
			  dstnclt->iScrollWidth = srcnclt->iScrollWidth;
			  dstnclt->iScrollHeight = srcnclt->iScrollHeight;
			  dstnclt->iCaptionWidth = srcnclt->iCaptionWidth;
			  dstnclt->iCaptionHeight = srcnclt->iCaptionHeight;
			  dstnclt->lfCaptionFont = srcnclt->lfCaptionFont;
			  dstnclt->iSmCaptionWidth = srcnclt->iSmCaptionWidth;
			  dstnclt->iSmCaptionHeight = srcnclt->iSmCaptionHeight;
			  dstnclt->lfSmCaptionFont = srcnclt->lfSmCaptionFont;
			  dstnclt->iMenuWidth = srcnclt->iMenuWidth;
			  dstnclt->iMenuHeight = srcnclt->iMenuHeight;
			  dstnclt->lfMenuFont = srcnclt->lfMenuFont;
			  dstnclt->lfStatusFont = srcnclt->lfStatusFont;
			  dstnclt->lfMessageFont = srcnclt->lfMessageFont;
			  resp = SystemParametersInfo(uiAction, uiParam, dstnclt, fWinIni);	
			  pvParam = dstnclt;
			  return resp;
		  }
		 return SystemParametersInfoW(uiAction, uiParam, pvParam, fWinIni);	
	  }
  }
  return SystemParametersInfoW(uiAction, uiParam, pvParam, fWinIni);
}

BOOL 
WINAPI
SystemParametersInfoAInternal(UINT uiAction,
		      UINT uiParam,
		      PVOID pvParam,
		      UINT fWinIni)
{
  BOOL resp = FALSE; 	
  switch(uiAction)
  {

    case SPI_GETNONCLIENTMETRICS:
      {  
		if(sizeof(pvParam) == sizeof(NONCLIENTMETRICSA_LH) || ((LPNONCLIENTMETRICSA_LH)pvParam)->cbSize == sizeof(NONCLIENTMETRICSA_LH)){
			  LPNONCLIENTMETRICSA_LH lpnclt = (LPNONCLIENTMETRICSA_LH)pvParam;	
			  LPNONCLIENTMETRICSA srcnclt = {0};	
			  DbgPrint("SystemParametersInfoAInternal:: LPNONCLIENTMETRICSW_LH\n");
			  resp = SystemParametersInfo(uiAction, uiParam, pvParam, fWinIni);			  
			  lpnclt->cbSize = sizeof(NONCLIENTMETRICSA_LH);
			  lpnclt->iBorderWidth = srcnclt->iBorderWidth;
			  lpnclt->iScrollWidth = srcnclt->iScrollWidth;
			  lpnclt->iScrollHeight = srcnclt->iScrollHeight;
			  lpnclt->iCaptionWidth = srcnclt->iCaptionWidth;
			  lpnclt->iCaptionHeight = srcnclt->iCaptionHeight;
			  lpnclt->lfCaptionFont = srcnclt->lfCaptionFont;
			  lpnclt->iSmCaptionWidth = srcnclt->iSmCaptionWidth;
			  lpnclt->iSmCaptionHeight = srcnclt->iSmCaptionHeight;
			  lpnclt->lfSmCaptionFont = srcnclt->lfSmCaptionFont;
			  lpnclt->iMenuWidth = srcnclt->iMenuWidth;
			  lpnclt->iMenuHeight = srcnclt->iMenuHeight;
			  lpnclt->lfMenuFont = srcnclt->lfMenuFont;
			  lpnclt->lfStatusFont = srcnclt->lfStatusFont;
			  lpnclt->lfMessageFont = srcnclt->lfMessageFont;
			  lpnclt->iPaddedBorderWidth = 0;
			  pvParam = lpnclt;
			  return resp;
		  }

          return SystemParametersInfoA(uiAction, uiParam, pvParam, fWinIni);          
      }
	  case SPI_SETNONCLIENTMETRICS:
	  {
		  if(sizeof(pvParam) == sizeof(NONCLIENTMETRICSW_LH) || ((LPNONCLIENTMETRICSW_LH)pvParam)->cbSize == sizeof(NONCLIENTMETRICSA_LH)){
			  LPNONCLIENTMETRICSA_LH srcnclt = (LPNONCLIENTMETRICSA_LH)pvParam;	
			  LPNONCLIENTMETRICSA dstnclt = {0};
			  // DbgPrint("SystemParametersInfoAInternal:: LPNONCLIENTMETRICSW_LH\n");
			  dstnclt->cbSize = sizeof(NONCLIENTMETRICSA);
			  dstnclt->iBorderWidth = srcnclt->iBorderWidth;
			  dstnclt->iScrollWidth = srcnclt->iScrollWidth;
			  dstnclt->iScrollHeight = srcnclt->iScrollHeight;
			  dstnclt->iCaptionWidth = srcnclt->iCaptionWidth;
			  dstnclt->iCaptionHeight = srcnclt->iCaptionHeight;
			  dstnclt->lfCaptionFont = srcnclt->lfCaptionFont;
			  dstnclt->iSmCaptionWidth = srcnclt->iSmCaptionWidth;
			  dstnclt->iSmCaptionHeight = srcnclt->iSmCaptionHeight;
			  dstnclt->lfSmCaptionFont = srcnclt->lfSmCaptionFont;
			  dstnclt->iMenuWidth = srcnclt->iMenuWidth;
			  dstnclt->iMenuHeight = srcnclt->iMenuHeight;
			  dstnclt->lfMenuFont = srcnclt->lfMenuFont;
			  dstnclt->lfStatusFont = srcnclt->lfStatusFont;
			  dstnclt->lfMessageFont = srcnclt->lfMessageFont;
			  resp = SystemParametersInfo(uiAction, uiParam, dstnclt, fWinIni);	
			  pvParam = dstnclt;
			  return resp;
		  }
		 return SystemParametersInfoW(uiAction, uiParam, pvParam, fWinIni);	
	  }
  }
  return SystemParametersInfoA(uiAction, uiParam, pvParam, fWinIni);
}