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

DEP_SYSTEM_POLICY_TYPE WINAPI 
GetSystemDEPPolicy(void)
{
	return SharedUserData->NXSupportPolicy;	
}

BOOL 
WINAPI 
GetProcessDEPPolicy(HANDLE ProcessInformation, LPDWORD lpFlags, PBOOL lpPermanent)
{
	NTSTATUS Status;
	ULONG depFlags;
	
	Status = NtQueryInformationProcess( GetCurrentProcess(), ProcessExecuteFlags,
											&depFlags, sizeof(depFlags), NULL );
	if (!Status)
	{
		if (lpFlags)
		{
			*lpFlags = 0;
			if (depFlags & MEM_EXECUTE_OPTION_DISABLE)
				*lpFlags |= PROCESS_DEP_ENABLE;
			if (depFlags & MEM_EXECUTE_OPTION_DISABLE_THUNK_EMULATION)
				*lpFlags |= PROCESS_DEP_DISABLE_ATL_THUNK_EMULATION;
			}

			if (lpPermanent)
				*lpPermanent = (depFlags & MEM_EXECUTE_OPTION_PERMANENT) != 0;
	}
	if (Status) 
		SetLastError( RtlNtStatusToDosError(Status) );
	return !Status;		
}

BOOL 
WINAPI 
SetProcessDEPPolicy(DWORD dwFlags)
{
	NTSTATUS Status;
	ULONG depFlags = 0;

	if (dwFlags & PROCESS_DEP_ENABLE)
		depFlags |= MEM_EXECUTE_OPTION_DISABLE | MEM_EXECUTE_OPTION_PERMANENT;
	if (dwFlags & PROCESS_DEP_DISABLE_ATL_THUNK_EMULATION)
		depFlags |= MEM_EXECUTE_OPTION_DISABLE_THUNK_EMULATION;

	Status = NtSetInformationProcess( GetCurrentProcess(), ProcessExecuteFlags,
													  &depFlags, sizeof(depFlags) );
	if(NT_SUCCESS(Status)){
		return TRUE;
	}else{
		BaseSetLastNTError(Status);
		return FALSE;
	}
}