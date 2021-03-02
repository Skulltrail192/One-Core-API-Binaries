/*++

Copyright (c) 2017  Shorthorn Project

Module Name:

    dep.c

Abstract:

    This module implements DEP APIs for Win32

Author:

    Skulltrail 11-April-2017

Revision History:

--*/

#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(kernel32);

static BOOL (WINAPI *pGetProcessDEPPolicy)(HANDLE, LPDWORD, PBOOL);
static BOOL (WINAPI *pSetProcessDEPPolicy)(DWORD);
static DEP_SYSTEM_POLICY_TYPE (WINAPI *pGetSystemDEPPolicy)();

DEP_SYSTEM_POLICY_TYPE WINAPI 
GetSystemDEPPolicy(void)
{
	HMODULE hkernel32 = GetModuleHandleA("kernelex.dll");
	
	pGetSystemDEPPolicy = (void *)GetProcAddress(hkernel32, "GetSystemDEPPolicy");
	if(pGetSystemDEPPolicy){
		return pGetSystemDEPPolicy();
	}else{
		return SharedUserData->NXSupportPolicy;	
	}	
}

BOOL 
WINAPI 
GetProcessDEPPolicy(HANDLE ProcessInformation, LPDWORD lpFlags, PBOOL lpPermanent)
{
	HMODULE hkernel32 = GetModuleHandleA("kernelex.dll");
	NTSTATUS status;
	ULONG dep_flags;
	
	pGetProcessDEPPolicy = (void *)GetProcAddress(hkernel32, "GetProcessDEPPolicy");
	if(pGetProcessDEPPolicy){
		return pGetProcessDEPPolicy(ProcessInformation, lpFlags, lpPermanent);
	}else{

		status = NtQueryInformationProcess( GetCurrentProcess(), ProcessExecuteFlags,
											&dep_flags, sizeof(dep_flags), NULL );
		if (!status)
		{
			if (lpFlags)
			{
				*lpFlags = 0;
				if (dep_flags & MEM_EXECUTE_OPTION_DISABLE)
					*lpFlags |= PROCESS_DEP_ENABLE;
				if (dep_flags & MEM_EXECUTE_OPTION_DISABLE_THUNK_EMULATION)
					*lpFlags |= PROCESS_DEP_DISABLE_ATL_THUNK_EMULATION;
			}

			if (lpPermanent)
				*lpPermanent = (dep_flags & MEM_EXECUTE_OPTION_PERMANENT) != 0;

		}
		if (status) SetLastError( RtlNtStatusToDosError(status) );
		return !status;		
	}
}

BOOL 
WINAPI 
SetProcessDEPPolicy(DWORD dwFlags)
{
	HMODULE hkernel32 = GetModuleHandleA("kernelex.dll");
	NTSTATUS status;
	ULONG depFlags = 0;
	
	pSetProcessDEPPolicy = (void *)GetProcAddress(hkernel32, "SetProcessDEPPolicy");
	if(pSetProcessDEPPolicy){
		return pSetProcessDEPPolicy(dwFlags);
	}else{
		if (dwFlags & PROCESS_DEP_ENABLE)
			depFlags |= MEM_EXECUTE_OPTION_DISABLE | MEM_EXECUTE_OPTION_PERMANENT;
		if (dwFlags & PROCESS_DEP_DISABLE_ATL_THUNK_EMULATION)
			depFlags |= MEM_EXECUTE_OPTION_DISABLE_THUNK_EMULATION;

		status = NtSetInformationProcess( GetCurrentProcess(), ProcessExecuteFlags,
													  &depFlags, sizeof(depFlags) );

		if(NT_SUCCESS(status)){
			return TRUE;
		}else{
			BaseSetLastNTError(status);
			return FALSE;
		}
	}
}