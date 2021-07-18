/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    process.c

Abstract:

    Process functions

Author:

    Skulltrail 07-March-2021

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(advapi32);

typedef BOOL (WINAPI *CREATE_PROCESS_TOKEN)(
    HANDLE,
	DWORD,
	LPCWSTR,
	LPWSTR,
	DWORD,
	PVOID,
	LPCWSTR,
	LPSTARTUPINFOW,
	LPPROCESS_INFORMATION);
	
/*
 * @implemented
 */
BOOL 
WINAPI 
CreateProcessWithTokenW(
	HANDLE token, 
	DWORD logon_flags, 
	LPCWSTR application_name, 
	LPWSTR command_line,
    DWORD creation_flags, 
	void *environment, 
	LPCWSTR current_directory, 
	STARTUPINFOW *startup_info,
    PROCESS_INFORMATION *process_information 
)
{
	CREATE_PROCESS_TOKEN createProcessToken;
	
    createProcessToken = (CREATE_PROCESS_TOKEN) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase.dll")),
                            "CreateProcessWithTokenW");
    if (NULL == createProcessToken) 
    {
		FIXME("%p 0x%08x %s %s 0x%08x %p %s %p %p - semi-stub\n", token,
			  logon_flags, debugstr_w(application_name), debugstr_w(command_line),
			  creation_flags, environment, debugstr_w(current_directory),
			  startup_info, process_information);

		/* FIXME: check if handles should be inherited */
		return CreateProcessW( application_name, command_line, NULL, NULL, FALSE, creation_flags, environment,
							   current_directory, startup_info, process_information );

    }else{
		return (BOOL)createProcessToken(token, 
										logon_flags,
										application_name,
										command_line,
										creation_flags,
										environment,
										current_directory,
										startup_info,
										process_information);
	}
}	