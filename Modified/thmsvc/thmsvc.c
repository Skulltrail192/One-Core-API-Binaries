/*
 * COPYRIGHT:        See COPYING in the top level directory
 * PROJECT:          ReactOS kernel
 * FILE:             services/thmsvc/thmsvc.c
 * PURPOSE:          Themes service
 * PROGRAMMER:       Giannis Adamopoulos
 */

/* INCLUDES *****************************************************************/

#include <windows.h>
#include <uxtheme.h>
#include <uxundoc.h>
#include <wine/debug.h>

WINE_DEFAULT_DEBUG_CHANNEL(thmsvc);


/* GLOBALS ******************************************************************/

static VOID CALLBACK ServiceMain(DWORD argc, LPWSTR *argv);
static WCHAR ServiceName[] = L"WinUx";
static SERVICE_TABLE_ENTRYW ServiceTable[] =
{
    {ServiceName, ServiceMain},
    {NULL, NULL}
};

SERVICE_STATUS_HANDLE ServiceStatusHandle;
SERVICE_STATUS ServiceStatus;
DWORD pSessionId;
HANDLE hEvent;
DWORD id;

/* FUNCTIONS *****************************************************************/

static VOID
UpdateServiceStatus(DWORD dwState)
{
    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwCurrentState = dwState;

    if (dwState == SERVICE_RUNNING)
        ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    else if (dwState == SERVICE_PAUSED)
        ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE;
    else
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
            UpdateServiceStatus(SERVICE_STOPPED);
            ThemeHooksRemove();
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
            TRACE("  Control %lu received\n");
            return ERROR_CALL_NOT_IMPLEMENTED;
    }
}

BOOLEAN MutexIsCreated()
{
  HANDLE handle; // esi@1

  handle = CreateMutexW(0, 0, L"Local\\WinUx:Mutex");
  if ( !handle )
    return FALSE;
  if ( GetLastError() == 183 )
  {
    CloseHandle(handle);
    return FALSE;
  }
  hObject = handle;
  return TRUE;
}

LSTATUS CloseRegistryKey(HKEY *this)
{
  HKEY *key; // esi@1
  LSTATUS result; // eax@2

  key = this;
  if ( *this )
  {
    result = RegCloseKey(*this);
    *key = 0;
  }
  else
  {
    result = 0;
  }
  return result;
}

LSTATUS OpenRegistryKey(PHKEY phkResult, HKEY hKey, LPCWSTR lpSubKey, REGSAM samDesired)
{
  HKEY *key; // esi@1

  key = phkResult;
  CloseRegistryKey(phkResult);
  return RegOpenKeyExW(hKey, lpSubKey, 0, samDesired, key);
}

BOOL QueryInformation(PVOID info)
{
  PVOID *SystemInformation; // [sp+0h] [bp-4h]@1

  SystemInformation = (PVOID *)this;
  return NtQuerySystemInformation(SystemKernelDebuggerInformation, &SystemInformation, 2u, 0) >= 0
      && (_BYTE)SystemInformation;
}

LSTATUS CompareFunction(HKEY *this, LPCWSTR lpValueName, BYTE *lpData)
{
  LSTATUS result; // eax@1
  DWORD cbData; // [sp+0h] [bp-4h]@1

  cbData = 4;
  result = RegQueryValueExW(this, lpValueName, (LPDWORD)&lpData, lpData, &cbData);
  if ( !result )
  {
    if ( lpData != (BYTE *)4 )
      result = 13;
  }
  return result;
}

DWORD CreateRegistryKey(HKEY *this, HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueName, DWORD lpData)
{
  DWORD data; // edi@1
  LSTATUS status; // eax@2

  data = lpData;
  if ( hKey )
    status = OpenRegistryKey(this, hKey, lpSubKey, 1u);
  else
    status = 0;
  if ( status || CompareFunction(key, lpValueName, (BYTE *)&lpData) )
    lpData = data;
  return lpData;
}

LSTATUS CallDesktop(PVOID info)
{
  LSTATUS result; // eax@1
  DWORD resp; // eax@2
  BOOLEAN verification; // [sp+0h] [bp-8h]@2

  result = QueryInformation(info);
  if ( (_BYTE)result )
  {
    //sub_6F6599B2(&verification);
    resp = CreateRegistryKey(
             (HKEY *)&verification,
             HKEY_LOCAL_MACHINE,
             L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Desktop",
             L"SBPeriodicTimeout",
             0x3Cu);
    if ( resp > 300 )
      resp = 60;
    if ( resp )
      CreateTimerQueueTimer(&Timer, 0, Callback, 0, 1000 * resp, 1000 * resp, 0);
    result = CloseRegistryKey((HKEY *)&verification);
  }
  return result;
}

static VOID CALLBACK
ServiceMain(DWORD argc, LPWSTR *argv)
{
  DWORD ProcessId; // eax@3
  PVOID info; // ecx@4

  if ( MutexIsCreated() )
  {
    hServiceStatus = RegisterServiceCtrlHandlerExW(L"WinUx", HandlerProc, 0);
    if ( hServiceStatus )
    {
      ProcessId = GetCurrentProcessId();
      if ( ProcessIdToSessionId(ProcessId, &pSessionId) )
      {
        hEvent = CreateEventW(0, 1, 0, L"Local\\WinUx:Event:APIReady");
        if ( hEvent )
        {
          if ( pSessionId )
          {
            CallDesktop(info);
            sub_6F652975();
            sub_6F654F0B();
            sub_6F654DE2(a1, a2);
            sub_6F654FAA();
          }
          CloseHandle(hEvent);
          hEvent = 0;
        }
      }
      sub_6F652552(1);
    }
  }

    //ThemeHooksInstall();

    /*TRACE("ServiceMain() done\n");
	UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    TRACE("ServiceMain() called\n");

    ServiceStatusHandle = RegisterServiceCtrlHandlerExW(ServiceName,
                                                        ServiceControlHandler,
                                                        NULL);

    TRACE("Calling SetServiceStatus()\n");
    UpdateServiceStatus(SERVICE_RUNNING);
    TRACE("SetServiceStatus() called\n");

    ThemeHooksInstall();

    TRACE("ServiceMain() done\n");*/
}


int
wmain(int argc, WCHAR *argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    TRACE("thmsvc: main() started\n");

    StartServiceCtrlDispatcher(ServiceTable);

    TRACE("thmsvc: main() done\n");

    return 0;
}

/* EOF */
