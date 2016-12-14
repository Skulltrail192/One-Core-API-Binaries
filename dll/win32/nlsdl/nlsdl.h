#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>
#include <ntsecapi.h>
#include <bcrypt.h>

int 
WINAPI 
DownlevelLCIDToLocaleName( 
	LCID lcid, 
	LPWSTR name, 
	INT count, 
	DWORD flags 
);

LCID 
WINAPI 
DownlevelLocaleNameToLCID(
  _In_  LPCWSTR lpName,
  _In_  DWORD dwFlags
);