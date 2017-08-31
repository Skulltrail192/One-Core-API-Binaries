/*
 *  ReactOS Services
 *  Copyright (C) 2015 ReactOS Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*
 * COPYRIGHT:        See COPYING in the top level directory
 * PROJECT:          ReactOS Services
 * FILE:             base/services/wkssvc/wkssvc.c
 * PURPOSE:          Workstation service
 * PROGRAMMER:       Eric Kohl
 */

/* INCLUDES *****************************************************************/

#include "precomp.h"
#include <debug.h>

WINE_DEFAULT_DEBUG_CHANNEL(wkssvc);

/* GLOBALS ******************************************************************/

static WCHAR ServiceName[] = L"WinUx";

static SERVICE_STATUS_HANDLE ServiceStatusHandle;
static SERVICE_STATUS ServiceStatus;
HANDLE hObject = NULL;
HANDLE Timer = NULL;

/* FUNCTIONS *****************************************************************/

static VOID
UpdateServiceStatus(DWORD dwState)
{
    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwCurrentState = dwState;
    ServiceStatus.dwControlsAccepted = 0;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;

    if (dwState == SERVICE_START_PENDING ||
        dwState == SERVICE_STOP_PENDING ||
        dwState == SERVICE_PAUSE_PENDING ||
        dwState == SERVICE_CONTINUE_PENDING)
        ServiceStatus.dwWaitHint = 10000;
    else
        ServiceStatus.dwWaitHint = 0;

    SetServiceStatus(ServiceStatusHandle,
                     &ServiceStatus);
}

static DWORD WINAPI
ServiceControlHandler(DWORD dwControl,
                      DWORD dwEventType,
                      LPVOID lpEventData,
                      LPVOID lpContext)
{
    TRACE("ServiceControlHandler() called\n");

    switch (dwControl)
    {
        case SERVICE_CONTROL_STOP:
            TRACE("  SERVICE_CONTROL_STOP received\n");
            /* Stop listening to incoming RPC messages */
            //RpcMgmtStopServerListening(NULL);
            UpdateServiceStatus(SERVICE_STOPPED);
            return ERROR_SUCCESS;

        case SERVICE_CONTROL_PAUSE:
            TRACE("  SERVICE_CONTROL_PAUSE received\n");
            UpdateServiceStatus(SERVICE_PAUSED);
            return ERROR_SUCCESS;

        case SERVICE_CONTROL_CONTINUE:
            TRACE("  SERVICE_CONTROL_CONTINUE received\n");
            UpdateServiceStatus(SERVICE_RUNNING);
            return ERROR_SUCCESS;

        case SERVICE_CONTROL_INTERROGATE:
            TRACE("  SERVICE_CONTROL_INTERROGATE received\n");
            SetServiceStatus(ServiceStatusHandle,
                             &ServiceStatus);
            return ERROR_SUCCESS;

        case SERVICE_CONTROL_SHUTDOWN:
            TRACE("  SERVICE_CONTROL_SHUTDOWN received\n");
            UpdateServiceStatus(SERVICE_STOPPED);
            return ERROR_SUCCESS;

        default :
            TRACE("  Control %lu received\n", dwControl);
            return ERROR_CALL_NOT_IMPLEMENTED;
    }
}


static
DWORD
ServiceInit(VOID)
{
    HANDLE hThread;

    hThread = NULL;

    if (!hThread)
    {
        ERR("Can't create PortThread\n");
        return GetLastError();
    }
    else
        CloseHandle(hThread);

    return ERROR_SUCCESS;
}

BOOL WINAPI DeleteTimerFunction()
{
  BOOL result; // eax@3

  if ( Timer )
  {
    DeleteTimerQueueTimer(0, Timer, 0);
    Timer = 0;
  }
  result = (BOOL)hObject;
  if ( hObject )
  {
    result = CloseHandle(hObject);
    hObject = 0;
  }
  return result;
}

VOID WINAPI
ServiceMain(DWORD argc, LPTSTR *argv)
{
    DWORD dwError;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    DbgPrint("ServiceMain() called\n");
    
	result = CreateEventW(0, 1, 0, L"UxDeskSB:Event:APIReady");
	if(result){
		ServiceStatusHandle = RegisterServiceCtrlHandlerExW(ServiceName,
                                                        ServiceControlHandler,
                                                        NULL);
		if (!ServiceStatusHandle)
		{
			DbgPrint("RegisterServiceCtrlHandlerExW() failed! (Error %lu)\n", GetLastError());
			return;
		}
	}
	
    UpdateServiceStatus(SERVICE_START_PENDING);

    dwError = ServiceInit();
    if (dwError != ERROR_SUCCESS)
    {
        DbgPrint("Service stopped (dwError: %lu\n", dwError);
        UpdateServiceStatus(SERVICE_STOPPED);
        return;
    }

    UpdateServiceStatus(SERVICE_RUNNING);
}


BOOL WINAPI
DllMain(HINSTANCE hinstDLL,
        DWORD fdwReason,
        LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
