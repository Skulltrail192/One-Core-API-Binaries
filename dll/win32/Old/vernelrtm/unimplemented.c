 /*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */
 
#define NDEBUG

#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

typedef DWORD TP_VERSION, *PTP_VERSION;

typedef DWORD TP_WAIT_RESULT;

typedef struct _TP_POOL TP_POOL, *PTP_POOL;

typedef struct _TP_TIMER TP_TIMER, *PTP_TIMER;

typedef struct _TP_CLEANUP_GROUP TP_CLEANUP_GROUP, *PTP_CLEANUP_GROUP;

typedef struct _TP_CALLBACK_INSTANCE TP_CALLBACK_INSTANCE, *PTP_CALLBACK_INSTANCE;

typedef VOID (*PTP_TIMER_CALLBACK)(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_TIMER Timer);

typedef struct _TP_WAIT TP_WAIT, *PTP_WAIT;

typedef TP_CALLBACK_ENVIRON_V1 TP_CALLBACK_ENVIRON, *PTP_CALLBACK_ENVIRON;

typedef VOID (*PTP_WAIT_CALLBACK)(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WAIT Wait, TP_WAIT_RESULT WaitResult);

/*UNIMPLEMENTED SECTION *******************************************/
/*
 * @unimplemented
 */
HRESULT
WINAPI
GetApplicationRecoveryCallback(IN HANDLE hProcess,
                               OUT APPLICATION_RECOVERY_CALLBACK* pRecoveryCallback,
                               OUT PVOID* ppvParameter,
                               PDWORD dwPingInterval,
                               PDWORD dwFlags)
{
    UNIMPLEMENTED;
    return E_FAIL;
}


/*
 * @unimplemented
 */
HRESULT
WINAPI
GetApplicationRestart(IN HANDLE hProcess,
                      OUT PWSTR pwzCommandline  OPTIONAL,
                      IN OUT PDWORD pcchSize,
                      OUT PDWORD pdwFlags  OPTIONAL)
{
    UNIMPLEMENTED;
    return E_FAIL;
}

/*
 * @unimplemented
 */
VOID
WINAPI
RecoveryFinished(IN BOOL bSuccess)
{
    UNIMPLEMENTED;
}


/*
 * @unimplemented
 */
HRESULT
WINAPI
RecoveryInProgress(OUT PBOOL pbCancelled)
{
    UNIMPLEMENTED;
    return E_FAIL;
}


/*
 * @unimplemented
 */
HRESULT
WINAPI
RegisterApplicationRecoveryCallback(IN APPLICATION_RECOVERY_CALLBACK pRecoveyCallback,
                                    IN PVOID pvParameter  OPTIONAL,
                                    DWORD dwPingInterval,
                                    DWORD dwFlags)
{
    UNIMPLEMENTED;
    return E_FAIL;
}


/*
 * @unimplemented
 */
HRESULT
WINAPI
RegisterApplicationRestart(IN PCWSTR pwzCommandline  OPTIONAL,
                           IN DWORD dwFlags)
{
    UNIMPLEMENTED;
    return E_FAIL;
}

/*
 * @implemented
 */
int WINAPI GetLocaleNullStringFromArrayInPool(int a1, int a2, int a3)
{
  int result; // eax@2

  if ( a2 )
    result = a1 + 2 * (a1 + 2 * (a2 + 2 * a3) + 2) + 2;
  else
    result = a1;
  return result;
}

/*
 * @unimplemented
*/
HRESULT WINAPI GetApplicationRestartSettings(
  _In_       HANDLE hProcess,
  _Out_opt_  PWSTR pwzCommandline,
  _Inout_    PDWORD pcchSize,
  _Out_opt_  PDWORD pdwFlags
)
{
	return S_OK;
}

VOID WINAPI RaiseFailFastException(
  _In_opt_  PEXCEPTION_RECORD pExceptionRecord,
  _In_opt_  PCONTEXT pContextRecord,
  _In_      DWORD dwFlags
)
{
	;
}

/*
 * @unimplemented
*/
VOID WINAPI ApplicationRecoveryFinished(
  _In_  BOOL bSuccess
)
{
	bSuccess = TRUE;
}

BOOL WINAPI SetThreadStackGuarantee(IN OUT PULONG  	StackSizeInBytes) 	
{
    UNIMPLEMENTED;
    return FALSE;
}

BOOL WINAPI Wow64DisableWow64FsRedirection(IN PVOID * OldValue) 
{
	return FALSE;
}	

BOOL WINAPI Wow64RevertWow64FsRedirection(IN PVOID * OldValue) 
{
	return FALSE;
}	