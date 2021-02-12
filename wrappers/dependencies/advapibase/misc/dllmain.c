/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * FILE:            lib/advapi32/misc/dllmain.c
 * PURPOSE:         Library main function
 * PROGRAMMER:      ???
 * UPDATE HISTORY:
 *                  Created ???
 */

#include <advapi32.h>
WINE_DEFAULT_DEBUG_CHANNEL(advapi);

extern BOOL RegInitialize(VOID);
extern BOOL RegCleanup(VOID);
extern VOID UnloadNtMarta(VOID);

BOOL
WINAPI
DllMain(
    HINSTANCE hinstDll,
    DWORD dwReason,
    LPVOID reserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDll);
            RegInitialize();
            break;

        case DLL_PROCESS_DETACH:
            CloseLogonLsaHandle();
            RegCleanup();
            UnloadNtMarta();
            break;
    }

    return TRUE;
}


BOOL 
WINAPI 
LogonUserExExW(
  _In_      LPTSTR        lpszUsername,
  _In_opt_  LPTSTR        lpszDomain,
  _In_opt_  LPTSTR        lpszPassword,
  _In_      DWORD         dwLogonType,
  _In_      DWORD         dwLogonProvider,
  _In_opt_  PTOKEN_GROUPS pTokenGroups,
  _Out_opt_ PHANDLE       phToken,
  _Out_opt_ PSID          *ppLogonSid,
  _Out_opt_ PVOID         *ppProfileBuffer,
  _Out_opt_ LPDWORD       pdwProfileLength,
  _Out_opt_ PQUOTA_LIMITS pQuotaLimits
)
{
	return FALSE;
}
/* EOF */
