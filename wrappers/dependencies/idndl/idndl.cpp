#define WIN32_LEAN_AND_MEAN
#define STRICT

#include <windows.h>

extern "C"
{
#include <idndl.h>
}

int
WINAPI
DownlevelGetLocaleScripts
(
	LPCWSTR lpLocaleName,
	LPWSTR lpScripts,
	int cchScripts
)
{
	return 0;
}

int
WINAPI
DownlevelGetStringScripts(
  _In_  DWORD   dwFlags,
  _In_  LPCWSTR lpString,
  _In_  int     cchString,
  _Out_ LPWSTR  lpScripts,
  _In_  int     cchScripts
)
{
	return 0;
}

BOOL 
WINAPI
DownlevelVerifyScripts(
  _In_ DWORD   dwFlags,
  _In_ LPCWSTR lpLocaleScripts,
  _In_ int     cchLocaleScripts,
  _In_ LPCWSTR lpTestScripts,
  _In_ int     cchTestScripts
)
{
	return 0;
}

// EOF
