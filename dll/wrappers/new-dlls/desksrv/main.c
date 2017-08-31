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
#include <stdio.h>
#include <ntsecapi.h>
#include <rtlfuncs.h>
#include <lpcfuncs.h>

WINE_DEFAULT_DEBUG_CHANNEL(wkssvc);

VOID ReportSvcStatus( DWORD dwCurrentState,
                      DWORD dwWin32ExitCode,
                      DWORD dwWaitHint);
					  
VOID SvcInit( DWORD dwArgc, LPTSTR *lpszArgv);					  

/* GLOBALS ******************************************************************/

/*static WCHAR ServiceName[] = L"WinUx";

static SERVICE_STATUS_HANDLE ServiceStatusHandle;
static SERVICE_STATUS ServiceStatus;
HANDLE hObject = NULL;
HANDLE Timer = NULL;*/
#define SVCNAME TEXT("WinUx")
#define SVC_ERROR                        ((DWORD)0xC0000001L)

SERVICE_STATUS          gSvcStatus; 
SERVICE_STATUS_HANDLE   gSvcStatusHandle; 
HANDLE                  ghSvcStopEvent = NULL;
HANDLE UxDeskSB;

/* FUNCTIONS *****************************************************************/

VOID WINAPI SvcCtrlHandler( DWORD dwCtrl )
{
   // Handle the requested control code. 

   switch(dwCtrl) 
   {  
      case SERVICE_CONTROL_STOP: 
         ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

         // Signal the service to stop.

         SetEvent(ghSvcStopEvent);
         ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
         
         return;
 
      case SERVICE_CONTROL_INTERROGATE: 
         break; 
 
      default: 
         break;
   } 
   
}


//
// Purpose: 
//   Logs messages to the event log
//
// Parameters:
//   szFunction - name of function that failed
// 
// Return value:
//   None
//
// Remarks:
//   The service must have an entry in the Application event log.
//
VOID SvcReportEvent(LPTSTR szFunction) 
{ 
  HANDLE hEventSource;
  LPCTSTR lpszStrings[2];
  char Buffer[80];

  hEventSource = RegisterEventSource(NULL, SVCNAME);

  if( NULL != hEventSource )
  {
    sprintf(Buffer, "%-60s failed with %d", szFunction, GetLastError());

    lpszStrings[0] = SVCNAME;
    lpszStrings[1] = Buffer;

    ReportEvent(hEventSource,        // event log handle
		EVENTLOG_ERROR_TYPE, // event type
		0,                   // event category
		SVC_ERROR,           // event identifier
		NULL,                // no security identifier
		2,                   // size of lpszStrings array
		0,                   // no binary data
		lpszStrings,         // array of strings
		NULL);               // no binary data

    DeregisterEventSource(hEventSource);
  }
}

//
// Purpose: 
//   Entry point for the service
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None.
//
VOID WINAPI ServiceMain( DWORD dwArgc, LPTSTR *lpszArgv )
{
	HANDLE result;
	
	// Register the initial event for the service	
	
	result = CreateEventW(0, 1, 0, L"UxDeskSB:Event:APIReady");
	
	if(!result)
	{
		DbgPrint("The UxDeskSB:Event:APIReady coun't be created\n");		
	}
	
    // Register the handler function for the service

    gSvcStatusHandle = RegisterServiceCtrlHandler( 
        SVCNAME, 
        SvcCtrlHandler);

    if( !gSvcStatusHandle )
    { 
        SvcReportEvent(TEXT("RegisterServiceCtrlHandler")); 
        return; 
    } 

    // These SERVICE_STATUS members remain as set here

    gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
    gSvcStatus.dwServiceSpecificExitCode = 0;    
	gSvcStatus.dwControlsAccepted = 0;
	gSvcStatus.dwWin32ExitCode = 0;
	gSvcStatus.dwCheckPoint = 0;

    // Report initial status to the SCM

    ReportSvcStatus( SERVICE_START_PENDING, NO_ERROR, 3000 );

    // Perform service-specific initialization and work.

    SvcInit( dwArgc, lpszArgv );
}

void CreatePortForUxTheme()
{
	UNICODE_STRING DestinationString;
	wchar_t Dest;
	OBJECT_ATTRIBUTES ObjectAttributes;
	const WCHAR SourceString;
	
	DestinationString.Buffer =  L"\\Sessions\\%d\\";/*(PWSTR)SetPortPath(
                                        HKEY_LOCAL_MACHINE,
                                        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Desktop",
                                        L"SBPortPath",
                                        L"Windows",
                                        L"UxDeskSB:APIPort",
                                        &Dest,
                                        260);*/
    if ( DestinationString.Buffer >= 0 )
    {
      RtlInitUnicodeString(&DestinationString, &SourceString);
      ObjectAttributes.RootDirectory = 0;
      ObjectAttributes.Attributes = 0;
      ObjectAttributes.SecurityQualityOfService = 0;
      ObjectAttributes.Length = 24;
      ObjectAttributes.ObjectName = (PUNICODE_STRING)&DestinationString;
      //ObjectAttributes.SecurityDescriptor = v2;
      DestinationString.Buffer = (PWSTR)NtCreatePort(&UxDeskSB, &ObjectAttributes, 0x80u, 0x100u, 0x1000u);
	}
}

//
// Purpose: 
//   The service code
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None
//
VOID SvcInit( DWORD dwArgc, LPTSTR *lpszArgv)
{
    // TO_DO: Declare and set any required variables.
    //   Be sure to periodically call ReportSvcStatus() with 
    //   SERVICE_START_PENDING. If initialization fails, call
    //   ReportSvcStatus with SERVICE_STOPPED.

    // Create an event. The control handler function, SvcCtrlHandler,
    // signals this event when it receives the stop control code.

    ghSvcStopEvent = CreateEvent(
                         NULL,    // default security attributes
                         TRUE,    // manual reset event
                         FALSE,   // not signaled
                         NULL);   // no name

    if ( ghSvcStopEvent == NULL)
    {
        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }

    // Report running status when initialization is complete.

    ReportSvcStatus( SERVICE_RUNNING, NO_ERROR, 0 );
	
	CreatePortForUxTheme();

    // TO_DO: Perform work until service stops.

    while(1)
    {
        // Check whether to stop the service.

        WaitForSingleObject(ghSvcStopEvent, INFINITE);

        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
    }
}

//
// Purpose: 
//   Sets the current service status and reports it to the SCM.
//
// Parameters:
//   dwCurrentState - The current state (see SERVICE_STATUS)
//   dwWin32ExitCode - The system error code
//   dwWaitHint - Estimated time for pending operation, 
//     in milliseconds
// 
// Return value:
//   None
//
VOID ReportSvcStatus( DWORD dwCurrentState,
                      DWORD dwWin32ExitCode,
                      DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure.

    gSvcStatus.dwCurrentState = dwCurrentState;
    gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    gSvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        gSvcStatus.dwControlsAccepted = 0;
    else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ( (dwCurrentState == SERVICE_RUNNING) ||
           (dwCurrentState == SERVICE_STOPPED) )
        gSvcStatus.dwCheckPoint = 0;
    else gSvcStatus.dwCheckPoint = dwCheckPoint++;

    // Report the status of the service to the SCM.
    SetServiceStatus( gSvcStatusHandle, &gSvcStatus );
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
