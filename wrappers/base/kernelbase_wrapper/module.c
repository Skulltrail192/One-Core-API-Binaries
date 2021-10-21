/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    module.c

Abstract:

    This module contains the Win32 Module Management APIs

Author:

    Skulltrail 15-October-2017

Revision History:

--*/

#include "main.h"
#include "wine/list.h"
#include <ldrfuncs.h>
#include <umfuncs.h>

WINE_DEFAULT_DEBUG_CHANNEL(kernel32file);

static BOOL oem_file_apis;

static BOOL (WINAPI *pSetSearchPathMode)(DWORD);

/* to keep track of LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE file handles */
struct exclusive_datafile
{
    struct list entry;
    HMODULE     module;
    HANDLE      file;
};
static struct list exclusive_datafile_list = LIST_INIT( exclusive_datafile_list );

#ifdef _M_IX86
typedef struct _PEB32
{
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    BOOLEAN SpareBool;
    DWORD   Mutant;
    DWORD   ImageBaseAddress;
    DWORD   LdrData;
} PEB32;
#endif

typedef struct _PEB_LDR_DATA32
{
    ULONG        Length;
    BOOLEAN      Initialized;
    DWORD        SsHandle;
    LIST_ENTRY32 InLoadOrderModuleList;
} PEB_LDR_DATA32;

typedef struct _LDR_DATA_TABLE_ENTRY32
{
    LIST_ENTRY32        InLoadOrderModuleList;
    LIST_ENTRY32        InMemoryOrderModuleList;
    LIST_ENTRY32        InInitializationOrderModuleList;
    DWORD               BaseAddress;
    DWORD               EntryPoint;
    ULONG               SizeOfImage;
    UNICODE_STRING32    FullDllName;
    UNICODE_STRING32    BaseDllName;
} LDR_DATA_TABLE_ENTRY32;

struct module_iterator
{
    HANDLE                 process;
    LIST_ENTRY            *head;
    LIST_ENTRY            *current;
    BOOL                   wow64;
    LDR_DATA_TABLE_ENTRY   ldr_module;
    LDR_DATA_TABLE_ENTRY32 ldr_module32;
};

/******************************************************************
 *      load_library_as_datafile
 */
static BOOL load_library_as_datafile( LPCWSTR load_path, DWORD flags, LPCWSTR name, HMODULE *mod_ret )
{
    WCHAR filenameW[MAX_PATH];
    HANDLE mapping, file = INVALID_HANDLE_VALUE;
    HMODULE module = 0;
    DWORD protect = PAGE_READONLY;

    *mod_ret = 0;

    if (flags & LOAD_LIBRARY_AS_IMAGE_RESOURCE) protect |= SEC_IMAGE;

    if (SearchPathW( NULL, name, L".dll", ARRAY_SIZE( filenameW ), filenameW, NULL ))
    {
        file = CreateFileW( filenameW, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_DELETE,
                            NULL, OPEN_EXISTING, 0, 0 );
    }
    if (file == INVALID_HANDLE_VALUE) return FALSE;

    mapping = CreateFileMappingW( file, NULL, protect, 0, 0, NULL );
    if (!mapping) goto failed;

    module = MapViewOfFile( mapping, FILE_MAP_READ, 0, 0, 0 );
    CloseHandle( mapping );
    if (!module) goto failed;

    if (!(flags & LOAD_LIBRARY_AS_IMAGE_RESOURCE))
    {
        /* make sure it's a valid PE file */
        if (!RtlImageNtHeader( module )) goto failed;
        *mod_ret = (HMODULE)((char *)module + 1); /* set bit 0 for data file module */

        if (flags & LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE)
        {
            struct exclusive_datafile *datafile = HeapAlloc( GetProcessHeap(), 0, sizeof(*datafile) );
            if (!datafile) goto failed;
            datafile->module = *mod_ret;
            datafile->file   = file;
            list_add_head( &exclusive_datafile_list, &datafile->entry );
            TRACE( "delaying close %p for module %p\n", datafile->file, datafile->module );
            return TRUE;
        }
    }
    else *mod_ret = (HMODULE)((char *)module + 2); /* set bit 1 for image resource module */

    CloseHandle( file );
    return TRUE;

failed:
    if (module) UnmapViewOfFile( module );
    CloseHandle( file );
    return FALSE;
}

/******************************************************************
 *      load_library
 */
static HMODULE load_library( const UNICODE_STRING *libname, DWORD flags )
{
    const DWORD unsupported_flags = LOAD_IGNORE_CODE_AUTHZ_LEVEL | LOAD_LIBRARY_REQUIRE_SIGNED_TARGET;
    NTSTATUS status;
    HMODULE module;
    WCHAR *load_path, *dummy;

    if (flags & unsupported_flags) DbgPrint( "load_library :: unsupported flag(s) used %#08x\n", flags );

	status = LdrGetDllPath( libname->Buffer, flags, &load_path, &dummy );
	
	if(!NT_SUCCESS(status)){
		SetLastError(status);
		return 0;
	}

    if (flags & (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE |
                 LOAD_LIBRARY_AS_IMAGE_RESOURCE))
    {
        ULONG_PTR magic;

        LdrLockLoaderLock( 0, NULL, &magic );
        if (!LdrGetDllHandle( load_path, NULL, (PUNICODE_STRING)libname, &module ))
        {
            LdrAddRefDll( 0, module );
            LdrUnlockLoaderLock( 0, magic );
            goto done;
        }
        if (load_library_as_datafile( load_path, flags, libname->Buffer, &module ))
        {
            LdrUnlockLoaderLock( 0, magic );
            goto done;
        }
        LdrUnlockLoaderLock( 0, magic );
        flags |= DONT_RESOLVE_DLL_REFERENCES; /* Just in case */
        /* Fallback to normal behaviour */
    }

    status = LdrLoadDll( load_path, NULL, libname, &module );
    if (status != STATUS_SUCCESS)
    {
        module = 0;
        if (status == STATUS_DLL_NOT_FOUND && (GetVersion() & 0x80000000))
            SetLastError( ERROR_DLL_NOT_FOUND );
        else
            SetLastError( RtlNtStatusToDosError( status ) );
    }
done:
    RtlReleasePath( load_path );
    return module;
}

/***********************************************************************
 *           LoadLibraryExW       (KERNEL32.@)
 *
 * Unicode version of LoadLibraryExA.
 */
HMODULE 
WINAPI 
DECLSPEC_HOTPATCH
LoadLibraryExInternalW(
	LPCWSTR libnameW, 
	HANDLE hfile, 
	DWORD flags
)
{
    UNICODE_STRING      wstr;
    HMODULE             res;
	
	//DbgPrint("LoadLibraryExInternalW:: Module Name: %ws\n",libnameW);
	
	switch(flags){
		case LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE:
		case LOAD_LIBRARY_AS_IMAGE_RESOURCE:
		case LOAD_LIBRARY_SEARCH_APPLICATION_DIR:
		case LOAD_LIBRARY_SEARCH_DEFAULT_DIRS:
		case LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR:
		case LOAD_LIBRARY_SEARCH_SYSTEM32:
		case LOAD_LIBRARY_SEARCH_USER_DIRS:
			if (!libnameW)
			{
				SetLastError(ERROR_INVALID_PARAMETER);
				return 0;
			}
			RtlInitUnicodeString( &wstr, libnameW );
			if (wstr.Buffer[wstr.Length/sizeof(WCHAR) - 1] != ' ')
				return load_library( &wstr, flags );

			/* Library name has trailing spaces */
			RtlCreateUnicodeString( &wstr, libnameW );
			while (wstr.Length > sizeof(WCHAR) &&
				   wstr.Buffer[wstr.Length/sizeof(WCHAR) - 1] == ' ')
			{
				wstr.Length -= sizeof(WCHAR);
			}
			wstr.Buffer[wstr.Length/sizeof(WCHAR)] = '\0';
			res = load_library( &wstr, flags );
			RtlFreeUnicodeString( &wstr );
			break;
		default:
			return LoadLibraryExW(libnameW, hfile, flags);
			
	}
	return res;
}

/***********************************************************************
 *           FILE_name_AtoW
 *
 * Convert a file name to Unicode, taking into account the OEM/Ansi API mode.
 *
 * If alloc is FALSE uses the TEB static buffer, so it can only be used when
 * there is no possibility for the function to do that twice, taking into
 * account any called function.
 */
WCHAR *FILE_name_AtoW( LPCSTR name, BOOL alloc )
{
    ANSI_STRING str;
    UNICODE_STRING strW, *pstrW;
    NTSTATUS status;

    RtlInitAnsiString( &str, name );
    pstrW = alloc ? &strW : &NtCurrentTeb()->StaticUnicodeString;
    if (oem_file_apis)
        status = RtlOemStringToUnicodeString( pstrW, &str, alloc );
    else
        status = RtlAnsiStringToUnicodeString( pstrW, &str, alloc );
    if (status == STATUS_SUCCESS) return pstrW->Buffer;

    if (status == STATUS_BUFFER_OVERFLOW)
        SetLastError( ERROR_FILENAME_EXCED_RANGE );
    else
        SetLastError( RtlNtStatusToDosError(status) );
    return NULL;
}

/******************************************************************
 *		LoadLibraryExA          (KERNEL32.@)
 *
 * Load a dll file into the process address space.
 *
 * PARAMS
 *  libname [I] Name of the file to load
 *  hfile   [I] Reserved, must be 0.
 *  flags   [I] Flags for loading the dll
 *
 * RETURNS
 *  Success: A handle to the loaded dll.
 *  Failure: A NULL handle. Use GetLastError() to determine the cause.
 *
 * NOTES
 * The HFILE parameter is not used and marked reserved in the SDK. I can
 * only guess that it should force a file to be mapped, but I rather
 * ignore the parameter because it would be extremely difficult to
 * integrate this with different types of module representations.
 */
HMODULE 
WINAPI 
DECLSPEC_HOTPATCH 
LoadLibraryExInternalA(
	LPCSTR libname, 
	HANDLE hfile, 
	DWORD flags
)
{
    WCHAR *libnameW;

    if (!(libnameW = FILE_name_AtoW( libname, FALSE ))) return 0;
	
	DbgPrint("LoadLibraryExInternalA:: Module Name: %s\n",libname);	
	
    return LoadLibraryExInternalW( libnameW, hfile, flags );
}

/****************************************************************************
 *              AddDllDirectory   (KERNEL32.@)
 */
DLL_DIRECTORY_COOKIE WINAPI AddDllDirectory( const WCHAR *dir )
{
    UNICODE_STRING str;
    void *cookie;
	NTSTATUS Status;

    RtlInitUnicodeString( &str, dir );
	
	Status = LdrAddDllDirectory( &str, &cookie );

	if(!NT_SUCCESS(Status)){
		return NULL;
	}	
	
    return cookie;
}


/****************************************************************************
 *              RemoveDllDirectory   (KERNEL32.@)
 */
BOOL WINAPI RemoveDllDirectory( DLL_DIRECTORY_COOKIE cookie )
{
	NTSTATUS Status;
	
	Status = LdrRemoveDllDirectory( cookie );
	
	if(NT_SUCCESS(Status)){
		return TRUE;
	}else{
		SetLastError(Status);
		return FALSE;
	}
}

/*************************************************************************
 *	SetDefaultDllDirectories   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH SetDefaultDllDirectories( DWORD flags )
{
	NTSTATUS Status;
	
	Status = LdrSetDefaultDllDirectories( flags );
	
	if(NT_SUCCESS(Status)){
		return TRUE;
	}else{
		SetLastError(Status);
		return FALSE;
	}
}

/*************************************************************************
 *           SetSearchPathMode   (KERNEL32.@)
 */
BOOL WINAPI SetSearchPathMode( DWORD flags )
{
	HMODULE hkernel32 = GetModuleHandleA("kernel32.dll");
	NTSTATUS Status;
	
	pSetSearchPathMode = (void *)GetProcAddress(hkernel32, "SetSearchPathMode");
	if(pSetSearchPathMode){
		return pSetSearchPathMode(flags);
	}else{
		Status = RtlSetSearchPathMode( flags );
		
		if(NT_SUCCESS(Status)){
			return TRUE;
		}else{
			SetLastError(Status);
			return FALSE;
		}	
	}
}

FARPROC 
WINAPI 
GetProcAddressInternal(
  _In_ HMODULE hModule,
  _In_ LPCSTR  lpProcName
)
{
	//DbgPrint("GetProcAddress::Function name: %s\n", lpProcName);
	return GetProcAddress(hModule, lpProcName);
}

HMODULE 
WINAPI 
GetModuleHandleInternalA(
  _In_opt_ LPCTSTR lpModuleName
)
{
	//DbgPrint("GetModuleHandleA::Module name: %s\n", lpModuleName);
	return GetModuleHandleA(lpModuleName);
}

HMODULE 
WINAPI 
GetModuleHandleInternalW(
  _In_opt_ LPCWSTR lpModuleName
)
{
	//DbgPrint("GetModuleHandleW::Module name: %ws\n", lpModuleName);
	return GetModuleHandleW(lpModuleName);
}

HMODULE 
WINAPI 
LoadLibraryInternalA(
  _In_ LPCTSTR lpFileName
)
{
	//DbgPrint("LoadLibraryA::File name: %s\n", lpFileName);
	return LoadLibraryA(lpFileName);
}

HMODULE 
WINAPI 
LoadLibraryInternalW(
  _In_ LPCWSTR lpFileName
)
{
	//DbgPrint("LoadLibraryW::File name: %ws\n", lpFileName);
	return LoadLibraryW(lpFileName);
}

WCHAR szAppInit[KEY_LENGTH];

BOOL
GetDllList()
{
    NTSTATUS Status;
    OBJECT_ATTRIBUTES Attributes;
    BOOL bRet = FALSE;
    BOOL bLoad;
    HANDLE hKey = NULL;
    DWORD dwSize;
    PKEY_VALUE_PARTIAL_INFORMATION kvpInfo = NULL;

    UNICODE_STRING szKeyName = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows");
    UNICODE_STRING szLoadName = RTL_CONSTANT_STRING(L"LoadAppInit_DLLs");
    UNICODE_STRING szDllsName = RTL_CONSTANT_STRING(L"AppInit_DLLs");

    InitializeObjectAttributes(&Attributes, &szKeyName, OBJ_CASE_INSENSITIVE, NULL, NULL);
    Status = NtOpenKey(&hKey, KEY_READ, &Attributes);
    if (NT_SUCCESS(Status))
    {
        dwSize = sizeof(KEY_VALUE_PARTIAL_INFORMATION) + sizeof(DWORD);
        kvpInfo = HeapAlloc(GetProcessHeap(), 0, dwSize);
        if (!kvpInfo)
            goto end;

        Status = NtQueryValueKey(hKey,
                                 &szLoadName,
                                 KeyValuePartialInformation,
                                 kvpInfo,
                                 dwSize,
                                 &dwSize);
        if (!NT_SUCCESS(Status))
            goto end;

        RtlMoveMemory(&bLoad,
                      kvpInfo->Data,
                      kvpInfo->DataLength);

        HeapFree(GetProcessHeap(), 0, kvpInfo);
        kvpInfo = NULL;

        if (bLoad)
        {
            Status = NtQueryValueKey(hKey,
                                     &szDllsName,
                                     KeyValuePartialInformation,
                                     NULL,
                                     0,
                                     &dwSize);
            if (Status != STATUS_BUFFER_TOO_SMALL)
                goto end;

            kvpInfo = HeapAlloc(GetProcessHeap(), 0, dwSize);
            if (!kvpInfo)
                goto end;

            Status = NtQueryValueKey(hKey,
                                     &szDllsName,
                                     KeyValuePartialInformation,
                                     kvpInfo,
                                     dwSize,
                                     &dwSize);
            if (NT_SUCCESS(Status))
            {
                LPWSTR lpBuffer = (LPWSTR)kvpInfo->Data;
                if (*lpBuffer != UNICODE_NULL)
                {
                    INT bytesToCopy, nullPos;

                    bytesToCopy = min(kvpInfo->DataLength, KEY_LENGTH * sizeof(WCHAR));

                    if (bytesToCopy != 0)
                    {
                        RtlMoveMemory(szAppInit,
                                      kvpInfo->Data,
                                      bytesToCopy);

                        nullPos = (bytesToCopy / sizeof(WCHAR)) - 1;

                        /* ensure string is terminated */
                        szAppInit[nullPos] = UNICODE_NULL;

                        bRet = TRUE;
                    }
                }
            }
        }
    }

end:
    if (hKey)
        NtClose(hKey);

    if (kvpInfo)
        HeapFree(GetProcessHeap(), 0, kvpInfo);

    return bRet;
}

VOID
LoadAppInitDlls()
{
    szAppInit[0] = UNICODE_NULL;

    if (GetDllList())
    {
        WCHAR buffer[KEY_LENGTH];
        LPWSTR ptr;
		size_t i;

        RtlCopyMemory(buffer, szAppInit, KEY_LENGTH * sizeof(WCHAR) );

		for (i = 0; i < KEY_LENGTH; ++ i)
		{
			if(buffer[i] == L' ' || buffer[i] == L',')
				buffer[i] = 0;
		}

		for (i = 0; i < KEY_LENGTH; )
		{
			if(buffer[i] == 0)
				++ i;
			else
			{
				ptr = buffer + i;
				i += wcslen(ptr);
				LoadLibraryW(ptr);
			}
		}
    }
}

PVOID 
WINAPI 
BasepMapModuleHandle( 	
	HMODULE  	hModule,
	BOOLEAN  	AsDataFile 
)
{
    /* If no handle is provided - use current image base address */
    if (!hModule) return NtCurrentPeb()->ImageBaseAddress;

    /* Check if it's a normal or a datafile one */
    if (LDR_IS_DATAFILE(hModule) && !AsDataFile)
        return NULL;

    /* It'a a normal DLL, just return its handle */
    return hModule;
}

BOOL 
WINAPI 
EnumResourceNamesExW(
	HMODULE hModule, 
	LPCWSTR lpType, 
	ENUMRESNAMEPROCW lpEnumFunc, 
	LONG_PTR lParam, 
	DWORD dwFlags, 
	LANGID LangId
)
{
	return EnumResourceNamesW(hModule, lpType, lpEnumFunc, lParam);
}	

BOOL 
WINAPI 
EnumResourceNamesExA(
	HMODULE hModule, 
	LPCSTR lpType, 
	ENUMRESNAMEPROCA lpEnumFunc, 
	LONG_PTR lParam, 
	DWORD dwFlags, 
	LANGID LangId)
{
	return EnumResourceNamesA(hModule, lpType, lpEnumFunc, lParam);
}	

BOOL WINAPI EnumResourceTypesExA(
  _In_opt_  HMODULE hModule,
  _In_      ENUMRESTYPEPROCA lpEnumFunc,
  _In_      LONG_PTR lParam,
  _In_      DWORD dwFlags,
  _In_      LANGID LangId
)	
{
	return EnumResourceTypesA(hModule, lpEnumFunc, lParam);
}

BOOL WINAPI EnumResourceTypesExW(
  _In_opt_  HMODULE hModule,
  _In_      ENUMRESTYPEPROCW lpEnumFunc,
  _In_      LONG_PTR lParam,
  _In_      DWORD dwFlags,
  _In_      LANGID LangId
)	
{
	return EnumResourceTypesW(hModule, lpEnumFunc, lParam);
}	

static BOOL init_module_iterator( struct module_iterator *iter, HANDLE process )
{
    PROCESS_BASIC_INFORMATION pbi;
    PPEB_LDR_DATA ldr_data;
	NTSTATUS status;

    if (!IsWow64Process( process, &iter->wow64 )) return FALSE;

	status = NtQueryInformationProcess( process, ProcessBasicInformation,
                                                  &pbi, sizeof(pbi), NULL );

    /* get address of PEB */
    if (!NT_SUCCESS(status))
	{
		RtlNtStatusToDosError(status);
        return FALSE;		
	}

    if (is_win64 && iter->wow64)
    {
        PEB_LDR_DATA32 *ldr_data32_ptr;
        DWORD ldr_data32, first_module;
        PEB32 *peb32;

        peb32 = (PEB32 *)(DWORD_PTR)pbi.PebBaseAddress;
#ifdef _M_IX86		
        if (!ReadProcessMemory( process, &peb32->LdrData, &ldr_data32, sizeof(ldr_data32), NULL ))
#else
        if (!ReadProcessMemory( process, &peb32->Ldr, &ldr_data32, sizeof(ldr_data32), NULL ))	
#endif
            return FALSE;
        ldr_data32_ptr = (PEB_LDR_DATA32 *)(DWORD_PTR) ldr_data32;
        if (!ReadProcessMemory( process, &ldr_data32_ptr->InLoadOrderModuleList.Flink,
                                &first_module, sizeof(first_module), NULL ))
            return FALSE;
        iter->head = (LIST_ENTRY *)&ldr_data32_ptr->InLoadOrderModuleList;
        iter->current = (LIST_ENTRY *)(DWORD_PTR)first_module;
        iter->process = process;
        return TRUE;
    }

    /* read address of LdrData from PEB */
    if (!ReadProcessMemory( process, &pbi.PebBaseAddress->Ldr, &ldr_data, sizeof(ldr_data), NULL ))
        return FALSE;

    /* read address of first module from LdrData */
    if (!ReadProcessMemory( process, &ldr_data->InLoadOrderModuleList.Flink,
                            &iter->current, sizeof(iter->current), NULL ))
        return FALSE;

    iter->head = &ldr_data->InLoadOrderModuleList;
    iter->process = process;
    return TRUE;
}


static int module_iterator_next( struct module_iterator *iter )
{
    if (iter->current == iter->head) return 0;

    if (is_win64 && iter->wow64)
    {
        LIST_ENTRY32 *entry32 = (LIST_ENTRY32 *)iter->current;

        if (!ReadProcessMemory( iter->process,
                                CONTAINING_RECORD(entry32, LDR_DATA_TABLE_ENTRY32, InLoadOrderModuleList),
                                &iter->ldr_module32, sizeof(iter->ldr_module32), NULL ))
            return -1;
        iter->current = (LIST_ENTRY *)(DWORD_PTR)iter->ldr_module32.InLoadOrderModuleList.Flink;
        return 1;
    }

    if (!ReadProcessMemory( iter->process,
                            CONTAINING_RECORD(iter->current, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks),
                            &iter->ldr_module, sizeof(iter->ldr_module), NULL ))
         return -1;

    iter->current = iter->ldr_module.InLoadOrderLinks.Flink;
    return 1;
}

/***********************************************************************
 *         K32EnumProcessModules   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH K32EnumProcessModules( HANDLE process, HMODULE *module,
                                                     DWORD count, DWORD *needed )
{
    struct module_iterator iter;
    DWORD size = 0;
    INT ret;

    if (process == GetCurrentProcess())
    {
        PPEB_LDR_DATA ldr_data = NtCurrentTeb()->ProcessEnvironmentBlock->Ldr;
        PLIST_ENTRY head = &ldr_data->InLoadOrderModuleList;
        PLIST_ENTRY entry = head->Flink;

        if (count && !module)
        {
            SetLastError( ERROR_NOACCESS );
            return FALSE;
        }
        while (entry != head)
        {
            LDR_DATA_TABLE_ENTRY *ldr = CONTAINING_RECORD( entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );
            if (count >= sizeof(HMODULE))
            {
                *module++ = ldr->DllBase;
                count -= sizeof(HMODULE);
            }
            size += sizeof(HMODULE);
            entry = entry->Flink;
        }
        if (!needed)
        {
            SetLastError( ERROR_NOACCESS );
            return FALSE;
        }
        *needed = size;
        return TRUE;
    }

    if (!init_module_iterator( &iter, process )) return FALSE;

    if (count && !module)
    {
        SetLastError( ERROR_NOACCESS );
        return FALSE;
    }

    while ((ret = module_iterator_next( &iter )) > 0)
    {
        if (count >= sizeof(HMODULE))
        {
            if (sizeof(void *) == 8 && iter.wow64)
                *module++ = (HMODULE) (DWORD_PTR)iter.ldr_module32.BaseAddress;
            else
                *module++ = iter.ldr_module.DllBase;
            count -= sizeof(HMODULE);
        }
        size += sizeof(HMODULE);
    }

    if (!needed)
    {
        SetLastError( ERROR_NOACCESS );
        return FALSE;
    }
    *needed = size;
    return ret == 0;
}

/***********************************************************************
 *         K32EnumProcessModulesEx   (kernelbase.@)
 */
BOOL WINAPI K32EnumProcessModulesEx( HANDLE process, HMODULE *module, DWORD count,
                                     DWORD *needed, DWORD filter )
{
    FIXME( "(%p, %p, %d, %p, %d) semi-stub\n", process, module, count, needed, filter );
    return K32EnumProcessModules( process, module, count, needed );
}

BOOL
WINAPI
GetWsChangesEx(
    HANDLE process,
    PPSAPI_WS_WATCH_INFORMATION_EX info,
    PDWORD size
    )
{
    FIXME( "(%p, %p, %p)\n", process, info, size );
    SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
    return FALSE;	
    // NTSTATUS Status;

    // Status = NtQueryInformationProcess(
                // hProcess,
                // ProcessWorkingSetWatchEx,
                // (PVOID *)lpWatchInfo,
                // cb,
                // NULL
                // );
    // if ( NT_SUCCESS(Status) ) {
        // return TRUE;
        // }
    // else {
        // SetLastError( RtlNtStatusToDosError( Status ) );
        // return FALSE;
        // }
}