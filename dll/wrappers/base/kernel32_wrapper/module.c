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

static RTL_CRITICAL_SECTION dlldir_section;

static WCHAR *dll_directory;  /* extra path for SetDllDirectoryW */

static struct list dll_dir_list = LIST_INIT( dll_dir_list ); 

static DWORD default_search_flags;  /* default flags set by SetDefaultDllDirectories */

static BOOL oem_file_apis;

/******************************************************************
 *		append_path_len
 *
 * Append a counted string to the load path. Helper for MODULE_get_dll_load_path.
 */
static inline WCHAR *append_path_len( WCHAR *p, const WCHAR *str, DWORD len )
{
    if (!len) return p;
    memcpy( p, str, len * sizeof(WCHAR) );
    p[len] = ';';
    return p + len + 1;
}

/******************************************************************
 *		append_path
 *
 * Append a string to the load path. Helper for MODULE_get_dll_load_path.
 */
static inline WCHAR *append_path( WCHAR *p, const WCHAR *str )
{
    return append_path_len( p, str, strlenW(str) );
}

struct dll_dir_entry
{
    struct list entry;
    WCHAR       dir[1];
};

void InitializeCriticalForDirectories(){
	 RtlInitializeCriticalSection(&dlldir_section);
}

/******************************************************************
 *		get_module_path_end
 *
 * Returns the end of the directory component of the module path.
 */
static inline const WCHAR *get_module_path_end(const WCHAR *module)
{
    const WCHAR *p;
    const WCHAR *mod_end = module;
    if (!module) return mod_end;

    if ((p = strrchrW( mod_end, '\\' ))) mod_end = p;
    if ((p = strrchrW( mod_end, '/' ))) mod_end = p;
    if (mod_end == module + 2 && module[1] == ':') mod_end++;
    if (mod_end == module && module[0] && module[1] == ':') mod_end += 2;

    return mod_end;
}

/***********************************************************************
 *           get_dll_system_path
 */
static const WCHAR *get_dll_system_path(void)
{
    static WCHAR *cached_path;

    if (!cached_path)
    {
        WCHAR *p, *path;
        int len = 1;

        len += 2 * GetSystemDirectoryW( NULL, 0 );
        len += GetWindowsDirectoryW( NULL, 0 );
        p = path = HeapAlloc( GetProcessHeap(), 0, len * sizeof(WCHAR) );
        GetSystemDirectoryW( p, path + len - p);
        p += strlenW(p);
        /* if system directory ends in "32" add 16-bit version too */
        if (p[-2] == '3' && p[-1] == '2')
        {
            *p++ = ';';
            GetSystemDirectoryW( p, path + len - p);
            p += strlenW(p) - 2;
        }
        *p++ = ';';
        GetWindowsDirectoryW( p, path + len - p);
        cached_path = path;
    }
    return cached_path;
}

/***********************************************************************
 *           get_dll_safe_mode
 */
static BOOL get_dll_safe_mode(void)
{
    static const WCHAR keyW[] = {'\\','R','e','g','i','s','t','r','y','\\',
                                 'M','a','c','h','i','n','e','\\',
                                 'S','y','s','t','e','m','\\',
                                 'C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\',
                                 'C','o','n','t','r','o','l','\\',
                                 'S','e','s','s','i','o','n',' ','M','a','n','a','g','e','r',0};
    static const WCHAR valueW[] = {'S','a','f','e','D','l','l','S','e','a','r','c','h','M','o','d','e',0};

    static int safe_mode = -1;

    if (safe_mode == -1)
    {
        //char buffer[offsetof(KEY_VALUE_PARTIAL_INFORMATION, Data[sizeof(DWORD)])];
		char buffer[256];
        KEY_VALUE_PARTIAL_INFORMATION *info = (KEY_VALUE_PARTIAL_INFORMATION *)buffer;
        OBJECT_ATTRIBUTES attr;
        UNICODE_STRING nameW;
        HANDLE hkey;
        DWORD size = sizeof(buffer);

        attr.Length = sizeof(attr);
        attr.RootDirectory = 0;
        attr.ObjectName = &nameW;
        attr.Attributes = 0;
        attr.SecurityDescriptor = NULL;
        attr.SecurityQualityOfService = NULL;

        safe_mode = 1;
        RtlInitUnicodeString( &nameW, keyW );
        if (!NtOpenKey( &hkey, KEY_READ, &attr ))
        {
            RtlInitUnicodeString( &nameW, valueW );
            if (!NtQueryValueKey( hkey, &nameW, KeyValuePartialInformation, buffer, size, &size ) &&
                info->Type == REG_DWORD && info->DataLength == sizeof(DWORD))
                safe_mode = !!*(DWORD *)info->Data;
            NtClose( hkey );
        }
        if (!safe_mode) TRACE( "SafeDllSearchMode disabled through the registry\n" );
    }
    return safe_mode;
}

/******************************************************************
 *		MODULE_get_dll_load_path
 *
 * Compute the load path to use for a given dll.
 * Returned pointer must be freed by caller.
 */
WCHAR *MODULE_get_dll_load_path( LPCWSTR module, int safe_mode )
{
    static const WCHAR pathW[] = {'P','A','T','H',0};
    static const WCHAR dotW[] = {'.',0};

    const WCHAR *system_path = get_dll_system_path();
    const WCHAR *mod_end = NULL;
    UNICODE_STRING name, value;
    WCHAR *p, *ret;
    int len = 0, path_len = 0;

    /* adjust length for module name */

    if (module)
        mod_end = get_module_path_end( module );
    /* if module is NULL or doesn't contain a path, fall back to directory
     * process was loaded from */
    if (module == mod_end)
    {
        module = NtCurrentTeb()->ProcessEnvironmentBlock->ProcessParameters->ImagePathName.Buffer;
        mod_end = get_module_path_end( module );
    }
    len += (mod_end - module) + 1;

    len += strlenW( system_path ) + 2;

    /* get the PATH variable */

    RtlInitUnicodeString( &name, pathW );
    value.Length = 0;
    value.MaximumLength = 0;
    value.Buffer = NULL;
    if (RtlQueryEnvironmentVariable_U( NULL, &name, &value ) == STATUS_BUFFER_TOO_SMALL)
        path_len = value.Length;

    RtlEnterCriticalSection( &dlldir_section );
    if (safe_mode == -1) safe_mode = get_dll_safe_mode();
    if (dll_directory) len += strlenW(dll_directory) + 1;
    else len += 2;  /* current directory */
    if ((p = ret = HeapAlloc( GetProcessHeap(), 0, path_len + len * sizeof(WCHAR) )))
    {
        if (module) p = append_path_len( p, module, mod_end - module );

        if (dll_directory) p = append_path( p, dll_directory );
        else if (!safe_mode) p = append_path( p, dotW );

        p = append_path( p, system_path );

        if (!dll_directory && safe_mode) p = append_path( p, dotW );
    }
    RtlLeaveCriticalSection( &dlldir_section );
    if (!ret) return NULL;

    value.Buffer = p;
    value.MaximumLength = path_len;

    while (RtlQueryEnvironmentVariable_U( NULL, &name, &value ) == STATUS_BUFFER_TOO_SMALL)
    {
        WCHAR *new_ptr;

        /* grow the buffer and retry */
        path_len = value.Length;
        if (!(new_ptr = HeapReAlloc( GetProcessHeap(), 0, ret, path_len + len * sizeof(WCHAR) )))
        {
            HeapFree( GetProcessHeap(), 0, ret );
            return NULL;
        }
        value.Buffer = new_ptr + (value.Buffer - ret);
        value.MaximumLength = path_len;
        ret = new_ptr;
    }
    value.Buffer[value.Length / sizeof(WCHAR)] = 0;
    return ret;
}

/******************************************************************
 *		get_dll_load_path_search_flags
 */
static WCHAR *get_dll_load_path_search_flags( LPCWSTR module, DWORD flags )
{
    const WCHAR *image = NULL, *mod_end, *image_end;
    struct dll_dir_entry *dir;
    WCHAR *p, *ret;
    int len = 1;

    if (flags & LOAD_LIBRARY_SEARCH_DEFAULT_DIRS)
        flags |= (LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
                  LOAD_LIBRARY_SEARCH_USER_DIRS |
                  LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (flags & LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR)
    {
        DWORD type = RtlDetermineDosPathNameType_U( module );
        if (type != ABSOLUTE_DRIVE_PATH && type != ABSOLUTE_PATH)
        {
            SetLastError( ERROR_INVALID_PARAMETER );
            return NULL;
        }
        mod_end = get_module_path_end( module );
        len += (mod_end - module) + 1;
    }
    else module = NULL;

    RtlEnterCriticalSection( &dlldir_section );

    if (flags & LOAD_LIBRARY_SEARCH_APPLICATION_DIR)
    {
        image = NtCurrentTeb()->ProcessEnvironmentBlock->ProcessParameters->ImagePathName.Buffer;
        image_end = get_module_path_end( image );
        len += (image_end - image) + 1;
    }

    if (flags & LOAD_LIBRARY_SEARCH_USER_DIRS)
    {
        LIST_FOR_EACH_ENTRY( dir, &dll_dir_list, struct dll_dir_entry, entry )
            len += strlenW( dir->dir ) + 1;
        if (dll_directory) len += strlenW(dll_directory) + 1;
    }

    if (flags & LOAD_LIBRARY_SEARCH_SYSTEM32) len += GetSystemDirectoryW( NULL, 0 );

    if ((p = ret = HeapAlloc( GetProcessHeap(), 0, len * sizeof(WCHAR) )))
    {
        if (module) p = append_path_len( p, module, mod_end - module );
        if (image) p = append_path_len( p, image, image_end - image );
        if (flags & LOAD_LIBRARY_SEARCH_USER_DIRS)
        {
            LIST_FOR_EACH_ENTRY( dir, &dll_dir_list, struct dll_dir_entry, entry )
                p = append_path( p, dir->dir );
            if (dll_directory) p = append_path( p, dll_directory );
        }
        if (flags & LOAD_LIBRARY_SEARCH_SYSTEM32) GetSystemDirectoryW( p, ret + len - p );
        else
        {
            if (p > ret) p--;
            *p = 0;
        }
    }

    RtlLeaveCriticalSection( &dlldir_section );
    return ret;
}

/******************************************************************
 *		load_library_as_datafile
 */
static BOOL load_library_as_datafile( LPCWSTR name, HMODULE *hmod, DWORD flags )
{
    static const WCHAR dotDLL[] = {'.','d','l','l',0};

    WCHAR filenameW[MAX_PATH];
    HANDLE hFile = INVALID_HANDLE_VALUE;
    HANDLE mapping;
    HMODULE module;
    DWORD sharing = FILE_SHARE_READ;

    *hmod = 0;

    if (!(flags & LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE)) sharing |= FILE_SHARE_WRITE;

    if (SearchPathW( NULL, name, dotDLL, sizeof(filenameW) / sizeof(filenameW[0]),
                     filenameW, NULL ))
    {
        hFile = CreateFileW( filenameW, GENERIC_READ, sharing, NULL, OPEN_EXISTING, 0, 0 );
    }
    if (hFile == INVALID_HANDLE_VALUE) return FALSE;

    mapping = CreateFileMappingW( hFile, NULL, PAGE_READONLY, 0, 0, NULL );
    CloseHandle( hFile );
    if (!mapping) return FALSE;

    module = MapViewOfFile( mapping, FILE_MAP_READ, 0, 0, 0 );
    CloseHandle( mapping );
    if (!module) return FALSE;

    /* make sure it's a valid PE file */
    if (!RtlImageNtHeader(module))
    {
        UnmapViewOfFile( module );
        return FALSE;
    }
    *hmod = (HMODULE)((char *)module + 1);  /* set low bit of handle to indicate datafile module */
    return TRUE;
}

/******************************************************************
 *		load_library
 *
 * Helper for LoadLibraryExA/W.
 */
static HMODULE load_library( const UNICODE_STRING *libname, DWORD flags )
{
    NTSTATUS nts;
    HMODULE hModule;
    WCHAR *load_path;
    const DWORD load_library_search_flags = (LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR |
                                             LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
                                             LOAD_LIBRARY_SEARCH_USER_DIRS |
                                             LOAD_LIBRARY_SEARCH_SYSTEM32 |
                                             LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
    const DWORD unsupported_flags = (LOAD_IGNORE_CODE_AUTHZ_LEVEL |
                                     LOAD_LIBRARY_AS_IMAGE_RESOURCE |
                                     LOAD_LIBRARY_REQUIRE_SIGNED_TARGET);

    if (!(flags & load_library_search_flags)) flags |= default_search_flags;

    if( flags & unsupported_flags)
        FIXME("unsupported flag(s) used (flags: 0x%08x)\n", flags);

    if (flags & load_library_search_flags)
        load_path = get_dll_load_path_search_flags( libname->Buffer, flags );
    else
        load_path = MODULE_get_dll_load_path( flags & LOAD_WITH_ALTERED_SEARCH_PATH ? libname->Buffer : NULL, -1 );
    if (!load_path) return 0;

    if (flags & (LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE))
    {
        ULONG_PTR magic;

        LdrLockLoaderLock( 0, NULL, &magic );
        if (!LdrGetDllHandle( load_path, &flags, libname, &hModule ))
        {
            LdrAddRefDll( 0, hModule );
            LdrUnlockLoaderLock( 0, magic );
            goto done;
        }
        LdrUnlockLoaderLock( 0, magic );

        /* The method in load_library_as_datafile allows searching for the
         * 'native' libraries only
         */
        if (load_library_as_datafile( libname->Buffer, &hModule, flags )) goto done;
        flags |= DONT_RESOLVE_DLL_REFERENCES; /* Just in case */
        /* Fallback to normal behaviour */
    }

    nts = LdrLoadDll( load_path, &flags, libname, &hModule );
    if (nts != STATUS_SUCCESS)
    {
        hModule = 0;
        if (nts == STATUS_DLL_NOT_FOUND && (GetVersion() & 0x80000000))
            SetLastError( ERROR_DLL_NOT_FOUND );
        else
            SetLastError( RtlNtStatusToDosError( nts ) );
    }
done:
    HeapFree( GetProcessHeap(), 0, load_path );
    return hModule;
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
	
	DbgPrint("LoadLibraryExInternalW:: Module Name: %ws\n",libnameW);
	
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
    WCHAR path[MAX_PATH];
    DWORD len;
    struct dll_dir_entry *ptr;
    DOS_PATHNAME_TYPE type = RtlDetermineDosPathNameType_U( dir );

    if (type != ABSOLUTE_PATH && type != ABSOLUTE_DRIVE_PATH)
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return NULL;
    }
    if (!(len = GetFullPathNameW( dir, MAX_PATH, path, NULL ))) return NULL;
    if (GetFileAttributesW( path ) == INVALID_FILE_ATTRIBUTES) return NULL;

    if (!(ptr = HeapAlloc( GetProcessHeap(), 0, offsetof(struct dll_dir_entry, dir[++len] )))) return NULL;
    memcpy( ptr->dir, path, len * sizeof(WCHAR) );
    TRACE( "%s\n", debugstr_w( ptr->dir ));

    RtlEnterCriticalSection( &dlldir_section );
    list_add_head( &dll_dir_list, &ptr->entry );
    RtlLeaveCriticalSection( &dlldir_section );
    return ptr;
}


/****************************************************************************
 *              RemoveDllDirectory   (KERNEL32.@)
 */
BOOL WINAPI RemoveDllDirectory( DLL_DIRECTORY_COOKIE cookie )
{
    struct dll_dir_entry *ptr = cookie;

    TRACE( "%s\n", debugstr_w( ptr->dir ));

    RtlEnterCriticalSection( &dlldir_section );
    list_remove( &ptr->entry );
    HeapFree( GetProcessHeap(), 0, ptr );
    RtlLeaveCriticalSection( &dlldir_section );
    return TRUE;
}

/*************************************************************************
 *           SetDefaultDllDirectories   (KERNEL32.@)
 */
BOOL WINAPI SetDefaultDllDirectories( DWORD flags )
{
    /* LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR doesn't make sense in default dirs */
    const DWORD load_library_search_flags = (LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
                                             LOAD_LIBRARY_SEARCH_USER_DIRS |
                                             LOAD_LIBRARY_SEARCH_SYSTEM32 |
                                             LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
											 
	DbgPrint("SetDefaultDllDirectories :: is called\n");

    if (!flags || (flags & ~load_library_search_flags))
    {
        SetLastError( ERROR_INVALID_PARAMETER );
		DbgPrint("SetDefaultDllDirectories :: invalid parameter, return is FALSE\n");		
        return FALSE;
    }
    default_search_flags = flags;
	DbgPrint("SetDefaultDllDirectories :: setting flags, return is TRUE\n");		
    return TRUE;
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