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

//
// File system redirection values
//

#define WOW64_FILE_SYSTEM_ENABLE_REDIRECT          (UlongToPtr(0x00))   // enable file-system redirection for the currently executing thread
#define WOW64_FILE_SYSTEM_DISABLE_REDIRECT         (UlongToPtr(0x01))   // disable file-system redirection for the currently executing thread

static UNICODE_STRING NtDllName = RTL_CONSTANT_STRING(L"ntdll.dll");
static ANSI_STRING RtlWow64EnableFsRedirectionProcName = RTL_CONSTANT_STRING("RtlWow64EnableFsRedirection");
static ANSI_STRING RtlWow64EnableFsRedirectionExProcName = RTL_CONSTANT_STRING("RtlWow64EnableFsRedirectionEx");

NTSTATUS (NTAPI *RtlpWow64EnableFsRedirection)(IN BOOLEAN  Wow64FsEnableRedirection);
NTSTATUS (NTAPI *RtlpWow64EnableFsRedirectionEx)(IN PVOID Wow64FsEnableRedirection, OUT PVOID * OldFsRedirectionLevel);

inline
PVOID 
Wow64SetFilesystemRedirectorEx (
    IN PVOID NewValue
    )
/*++

Routine Description:

    This routine allows a thread running inside Wow64 to disable file-system 
    redirection for all calls happening in the context of this thread.
    
    
    NOTE: This routine should only called from a wow64 process, and is only available 
          when running on .NET server platforms and beyond. If you component will 
          run on downlevel platforms (XP 2600 for example), you shouldn't use WOW64_FILE_SYSTEM_DISABLE_REDIRECT (see below).

Example (Enumerating files under c:\windows\system32):
    
    {
        HANDLE File;
        WIN32_FIND_DATA FindData;
#ifndef _WIN64        
        BOOL bWow64Process = FALSE;
        PVOID Wow64RedirectionOld;
#endif        

        //
        // Disable Wow64 file system redirection
        //
#ifndef _WIN64        
        IsWow64Process (GetCurrentProcess (), &bWow64Process);
        if (bWow64Process == TRUE) {
            Wow64RedirectionOld = Wow64SetFilesystemRedirectorEx (WOW64_FILE_SYSTEM_DISABLE_REDIRECT);
        }
#endif        
        File = FindFirstFileA ("c:\\windows\\system32\\*.*", &FindData);
        
        do {
        .
        .
        } while (FindNextFileA (File, &FindData) != 0);
        
        FindClose (File);
        
        //
        // Enable Wow64 file-system redirection
        //
#ifndef _WIN64        
        if (bWow64Process == TRUE) {
            Wow64SetFilesystemRedirectorEx (Wow64RedirectionOld);
        }
#endif        
    }


Arguments:

    NewValue - New Wow64 file-system redirection value. This can either be:
               a- pointer to a unicode string with a fully-qualified path name (e.g. L"c:\\windows\\notepad.exe").
               b- any of the following predefined values :
                  * WOW64_FILE_SYSTEM_ENABLE_REDIRECT : Enables file-system redirection (default)
                  * WOW64_FILE_SYSTEM_DISABLE_REDIRECT : Disables file-system redirection on all
                    file I/O operations happening within the context of the current thread.
                  * WOW64_FILE_SYSTEM_DISABLE_REDIRECT_LEGACY: Use this only if you want to run on 
                    download level platforms (for example XP 2600), as it will have no effect
                    and prevents your program from malfunctioning.
    
Return:

    Old Wow64 file-system redirection value

--*/
{
    PVOID OldValue;
    OldValue = (PVOID)(ULONG_PTR)NtCurrentTeb()->TlsSlots[WOW64_TLS_FILESYSREDIR];
    NtCurrentTeb()->TlsSlots[WOW64_TLS_FILESYSREDIR] = NewValue;//(ULONGLONG)PtrToUlong(NewValue);
    return OldValue;
}

BOOL
NTAPI
RtlIsWow64Process(    
	HANDLE hProcess,
    PBOOL Wow64Process)
{
	NTSTATUS NtStatus;
    ULONG_PTR Peb32;

    NtStatus = NtQueryInformationProcess (
        hProcess,
        ProcessWow64Information,
        &Peb32,
        sizeof (Peb32),
        NULL
        );

    if (!NT_SUCCESS (NtStatus)) {

        return STATUS_UNSUCCESSFUL;
    } else {

        if (Peb32 == 0) {
            *Wow64Process = FALSE;
        } else {
            *Wow64Process = TRUE;
        }
    }

    return NtStatus;
}

NTSYSAPI
NTSTATUS 
NTAPI 
RtlWow64EnableFsRedirectionEx(
	IN PVOID Wow64FsEnableRedirection,
	OUT PVOID * OldFsRedirectionLevel 
) 		
{
#ifndef _M_AMD64
	BOOL isWow64;
	RtlIsWow64Process(NtCurrentProcess(),&isWow64);
	
	if(isWow64){		
		NTSTATUS st;
		PVOID Wow64Handle;
		
		st = LdrLoadDll(NULL, NULL, &NtDllName, &Wow64Handle);
		
		if (!NT_SUCCESS(st)) {
		   DbgPrint("RtlWow64EnableFsRedirectionEx: Ntdll.dll not found.  Status=%x\n", st);
		   return st;
		}	
		
		st = LdrGetProcedureAddress (Wow64Handle,
									&RtlWow64EnableFsRedirectionExProcName,
									 0,
									(PVOID*)&RtlpWow64EnableFsRedirectionEx);
									
		if (!NT_SUCCESS(st)) {
		   DbgPrint("RtlWow64EnableFsRedirectionEx: RtlWow64EnableFsRedirectionEx function not found.  Status=%x\n", st);
		   return st;
		}

		return (*RtlpWow64EnableFsRedirectionEx)(Wow64FsEnableRedirection, OldFsRedirectionLevel);
	}else{
		return STATUS_NOT_IMPLEMENTED;	
	}
#else
	return STATUS_NOT_IMPLEMENTED;	
#endif
}

NTSYSAPI
NTSTATUS 
NTAPI 
RtlWow64EnableFsRedirection(
	IN BOOLEAN  Wow64FsEnableRedirection
) 	
{
#ifndef _M_AMD64	
	BOOL isWow64;
	RtlIsWow64Process(NtCurrentProcess(),&isWow64);
	
	if(isWow64){
		NTSTATUS st;
		PVOID Wow64Handle;
		
		st = LdrLoadDll(NULL, NULL, &NtDllName, &Wow64Handle);
		
		if (!NT_SUCCESS(st)) {
		   DbgPrint("RtlWow64EnableFsRedirection: Ntdll.dll not found.  Status=%x\n", st);
		   return st;
		}	
		
		st = LdrGetProcedureAddress (Wow64Handle,
									&RtlWow64EnableFsRedirectionProcName,
									 0,
									(PVOID*)&RtlpWow64EnableFsRedirection);
									
		if (!NT_SUCCESS(st)) {
		   DbgPrint("RtlWow64EnableFsRedirection: RtlWow64EnableFsRedirection function not found.  Status=%x\n", st);
		   return st;
		}	

		return (*RtlpWow64EnableFsRedirection)(Wow64FsEnableRedirection);
	}else{
		return STATUS_NOT_IMPLEMENTED;
	}	
#else
	return STATUS_NOT_IMPLEMENTED;	
#endif
}