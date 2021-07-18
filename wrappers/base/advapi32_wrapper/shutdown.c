/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    shutdown.c

Abstract:

    This module contains the client side wrappers for the Win32 remote
    shutdown APIs

Author:

    Skulltrail 18-July-2021

Revision History:

--*/

#include "main.h"

/* implemented - But, need revision*/
DWORD 
WINAPI 
InitiateShutdownW(
  _In_opt_  LPWSTR lpMachineName,
  _In_opt_  LPWSTR lpMessage,
  _In_      DWORD dwGracePeriod,
  _In_      DWORD dwShutdownFlags,
  _In_      DWORD dwReason
)
{
	BOOL shutdown;
	DWORD time = dwGracePeriod;
	switch(dwShutdownFlags)
	{
		case SHUTDOWN_GRACE_OVERRIDE:
			time = 0;
			shutdown = 1;
			break;
		case SHUTDOWN_RESTART:
			shutdown = 0;
			break;
		default:
			shutdown = 1;
			break;
	}
	InitiateSystemShutdownW(lpMachineName,lpMessage, 0, 1, shutdown);
	return ERROR_SUCCESS;
}

/* implemented - But, need revision*/
DWORD 
WINAPI 
InitiateShutdownA(
  _In_opt_  LPSTR lpMachineName,
  _In_opt_  LPSTR lpMessage,
  _In_      DWORD dwGracePeriod,
  _In_      DWORD dwShutdownFlags,
  _In_      DWORD dwReason
)
{
	BOOL shutdown;
	DWORD time = dwGracePeriod;
	switch(dwShutdownFlags)
	{
		case SHUTDOWN_GRACE_OVERRIDE:
			time = 0;
			shutdown = 1;
			break;
		case SHUTDOWN_RESTART:
			shutdown = 0;
			break;
		default:
			shutdown = 1;
			break;
	}
	InitiateSystemShutdownA(lpMachineName,lpMessage, 0, 1, shutdown);
	return ERROR_SUCCESS;
}