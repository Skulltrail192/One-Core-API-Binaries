/* COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * PURPOSE:         Vista functions
 * PROGRAMMER:      Thomas Weidenmueller <w3seek@reactos.com>
 */

#include <k32.h>

#define NDEBUG
#include <debug.h>


BOOL WINAPI SetFileCompletionNotificationModes(
  _In_  HANDLE FileHandle,
  _In_  UCHAR Flags
)
{
	return TRUE;
}

void WINAPI BaseProcessStart(LPVOID lpfnStartRoutine)  
{
	;
}

void WINAPI BaseThreadStart(LPVOID lpfnStartRoutine)  
{
	;
}

DWORD WINAPI GetModuleFileNameExA(
  _In_      HANDLE hProcess,
  _In_opt_  HMODULE hModule,
  _Out_     LPTSTR lpFilename,
  _In_      DWORD nSize
)
{
	return 0;
}

DWORD WINAPI GetModuleFileNameExW(
  _In_      HANDLE hProcess,
  _In_opt_  HMODULE hModule,
  _Out_     LPWSTR lpFilename,
  _In_      DWORD nSize
)
{
	return 0;
}

