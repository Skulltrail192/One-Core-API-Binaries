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

struct ldr_notification
{
    struct list                    entry;
    PLDR_DLL_NOTIFICATION_FUNCTION callback;
    void                           *context;
};

struct dll_dir_entry
{
    struct list entry;
    WCHAR       dir[1];
};

static struct list dll_dir_list = LIST_INIT( dll_dir_list );  /* extra dirs from LdrAddDllDirectory */
RTL_CRITICAL_SECTION dlldir_section;
static struct list ldr_notifications = LIST_INIT( ldr_notifications );
static DWORD default_search_flags;  /* default flags set by LdrSetDefaultDllDirectories */
static UNICODE_STRING dll_directory;  /* extra path for LdrSetDllDirectory */
static ULONG dll_safe_mode = 1;  /* dll search mode */
static ULONG path_safe_mode;  /* path mode set by RtlSetSearchPathMode */

const WCHAR system_dir[] = {'C',':','\\','w','i','n','d','o','w','s','\\',
                            's','y','s','t','e','m','3','2','\\',0};
							
/* system search path */
static const WCHAR system_path[] =
    {'C',':','\\','w','i','n','d','o','w','s','\\','s','y','s','t','e','m','3','2',';',
     'C',':','\\','w','i','n','d','o','w','s','\\','s','y','s','t','e','m',';',
     'C',':','\\','w','i','n','d','o','w','s',0};			

static const WCHAR dotW[] = {'.',0};	 

static NTSTATUS query_dword_option( HANDLE hkey, LPCWSTR name, ULONG *value )
{
    NTSTATUS status;
    UNICODE_STRING str;
    ULONG size;
    WCHAR buffer[64];
    KEY_VALUE_PARTIAL_INFORMATION *info = (KEY_VALUE_PARTIAL_INFORMATION *)buffer;

    RtlInitUnicodeString( &str, name );

    size = sizeof(buffer) - sizeof(WCHAR);
    if ((status = NtQueryValueKey( hkey, &str, KeyValuePartialInformation, buffer, size, &size )))
        return status;

    if (info->Type != REG_DWORD)
    {
        buffer[size / sizeof(WCHAR)] = 0;
        *value = wcstoul( (WCHAR *)info->Data, 0, 16 );
    }
    else memcpy( value, info->Data, sizeof(*value) );
    return status;
}

/******************************************************************
 *		get_module_path_end
 *
 * Returns the end of the directory component of the module path.
 */
static inline const WCHAR *get_module_path_end( const WCHAR *module )
{
    const WCHAR *p;
    const WCHAR *mod_end = module;

    if ((p = wcsrchr( mod_end, '\\' ))) mod_end = p;
    if ((p = wcsrchr( mod_end, '/' ))) mod_end = p;
    if (mod_end == module + 2 && module[1] == ':') mod_end++;
    if (mod_end == module && module[0] && module[1] == ':') mod_end += 2;
    return mod_end;
}


/******************************************************************
 *		append_path
 *
 * Append a counted string to the load path. Helper for get_dll_load_path.
 */
static inline WCHAR *append_path( WCHAR *p, const WCHAR *str, int len )
{
    if (len == -1) len = wcslen(str);
    if (!len) return p;
    memcpy( p, str, len * sizeof(WCHAR) );
    p[len] = ';';
    return p + len + 1;
}

/******************************************************************
 *           get_dll_load_path
 */
static NTSTATUS get_dll_load_path( LPCWSTR module, LPCWSTR dll_dir, ULONG safe_mode, WCHAR **path )
{
    static const WCHAR pathW[] = {'P','A','T','H',0};

    const WCHAR *mod_end = module;
    UNICODE_STRING name, value;
    WCHAR *p, *ret;
    int len = ARRAY_SIZE(system_path) + 1, path_len = 0;

    if (module)
    {
        mod_end = get_module_path_end( module );
        len += (mod_end - module) + 1;
    }

    RtlInitUnicodeString( &name, pathW );
    value.Length = 0;
    value.MaximumLength = 0;
    value.Buffer = NULL;
    if (RtlQueryEnvironmentVariable_U( NULL, &name, &value ) == STATUS_BUFFER_TOO_SMALL)
        path_len = value.Length;

    if (dll_dir) len += wcslen( dll_dir ) + 1;
    else len += 2;  /* current directory */
    if (!(p = ret = RtlAllocateHeap( RtlProcessHeap(), 0, path_len + len * sizeof(WCHAR) )))
        return STATUS_NO_MEMORY;

    p = append_path( p, module, mod_end - module );
    if (dll_dir) p = append_path( p, dll_dir, -1 );
    else if (!safe_mode) p = append_path( p, dotW, -1 );
    p = append_path( p, system_path, -1 );
    if (!dll_dir && safe_mode) p = append_path( p, dotW, -1 );

    value.Buffer = p;
    value.MaximumLength = path_len;

    while (RtlQueryEnvironmentVariable_U( NULL, &name, &value ) == STATUS_BUFFER_TOO_SMALL)
    {
        WCHAR *new_ptr;

        /* grow the buffer and retry */
        path_len = value.Length;
        if (!(new_ptr = RtlReAllocateHeap( RtlProcessHeap(), 0, ret, path_len + len * sizeof(WCHAR) )))
        {
            RtlFreeHeap( RtlProcessHeap(), 0, ret );
            return STATUS_NO_MEMORY;
        }
        value.Buffer = new_ptr + (value.Buffer - ret);
        value.MaximumLength = path_len;
        ret = new_ptr;
    }
    value.Buffer[value.Length / sizeof(WCHAR)] = 0;
    *path = ret;
    return STATUS_SUCCESS;
}

/******************************************************************
 *		get_dll_load_path_search_flags
 */
static NTSTATUS get_dll_load_path_search_flags( LPCWSTR module, DWORD flags, WCHAR **path )
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
        if (type != ABSOLUTE_DRIVE_PATH && type != ABSOLUTE_PATH && type != DEVICE_PATH)
            return STATUS_INVALID_PARAMETER;
        mod_end = get_module_path_end( module );
        len += (mod_end - module) + 1;
    }
    else module = NULL;

    if (flags & LOAD_LIBRARY_SEARCH_APPLICATION_DIR)
    {
        image = NtCurrentTeb()->ProcessEnvironmentBlock->ProcessParameters->ImagePathName.Buffer;
        image_end = get_module_path_end( image );
        len += (image_end - image) + 1;
    }

    if (flags & LOAD_LIBRARY_SEARCH_USER_DIRS)
    {
        LIST_FOR_EACH_ENTRY( dir, &dll_dir_list, struct dll_dir_entry, entry )
            len += wcslen( dir->dir + 4 /* \??\ */ ) + 1;
        if (dll_directory.Length) len += dll_directory.Length / sizeof(WCHAR) + 1;
    }

    if (flags & LOAD_LIBRARY_SEARCH_SYSTEM32) len += wcslen( system_dir );

    if ((p = ret = RtlAllocateHeap( RtlProcessHeap(), 0, len * sizeof(WCHAR) )))
    {
        if (module) p = append_path( p, module, mod_end - module );
        if (image) p = append_path( p, image, image_end - image );
        if (flags & LOAD_LIBRARY_SEARCH_USER_DIRS)
        {
            LIST_FOR_EACH_ENTRY( dir, &dll_dir_list, struct dll_dir_entry, entry )
                p = append_path( p, dir->dir + 4 /* \??\ */, -1 );
            p = append_path( p, dll_directory.Buffer, dll_directory.Length / sizeof(WCHAR) );
        }
        if (flags & LOAD_LIBRARY_SEARCH_SYSTEM32) wcscpy( p, system_dir );
        else
        {
            if (p > ret) p--;
            *p = 0;
        }
    }
    *path = ret;
    return STATUS_SUCCESS;
}

/******************************************************************
 *              LdrRegisterDllNotification (NTDLL.@)
 */
NTSTATUS WINAPI LdrRegisterDllNotification(ULONG flags, PLDR_DLL_NOTIFICATION_FUNCTION callback,
                                           void *context, void **cookie)
{
    struct ldr_notification *notify;

    DbgPrint( "LdrRegisterDllNotification::(%x, %p, %p, %p)\n", flags, callback, context, cookie );

    if (!callback || !cookie)
        return STATUS_INVALID_PARAMETER;

    if (flags)
        DbgPrint( "LdrRegisterDllNotification:: ignoring flags %x\n", flags );

    notify = RtlAllocateHeap( RtlProcessHeap(), 0, sizeof(*notify) );
    if (!notify) return STATUS_NO_MEMORY;
    notify->callback = callback;
    notify->context = context;

    RtlEnterCriticalSection( &loader_section );
    list_add_tail( &ldr_notifications, &notify->entry );
    RtlLeaveCriticalSection( &loader_section );

    *cookie = notify;
    return STATUS_SUCCESS;
}

/******************************************************************
 *              LdrUnregisterDllNotification (NTDLL.@)
 */
NTSTATUS WINAPI LdrUnregisterDllNotification( void *cookie )
{
    struct ldr_notification *notify = cookie;

    DbgPrint( "LdrUnregisterDllNotification::(%p)\n", cookie );

    if (!notify) return STATUS_INVALID_PARAMETER;

    RtlEnterCriticalSection( &loader_section );
    list_remove( &notify->entry );
    RtlLeaveCriticalSection( &loader_section );

    RtlFreeHeap( RtlProcessHeap(), 0, notify );
    return STATUS_SUCCESS;
}

/*************************************************************************
 *		LdrSetDefaultDllDirectories  (NTDLL.@)
 */
NTSTATUS WINAPI LdrSetDefaultDllDirectories( ULONG flags )
{
    /* LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR doesn't make sense in default dirs */
    const ULONG load_library_search_flags = (LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
                                             LOAD_LIBRARY_SEARCH_USER_DIRS |
                                             LOAD_LIBRARY_SEARCH_SYSTEM32 |
                                             LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);

    if (!flags || (flags & ~load_library_search_flags)) 
		return STATUS_INVALID_PARAMETER;
    default_search_flags = flags;
    return STATUS_SUCCESS;
}

/******************************************************************
 *		LdrGetDllPath  (NTDLL.@)
 */
NTSTATUS WINAPI LdrGetDllPath( PCWSTR module, ULONG flags, PWSTR *path, PWSTR *unknown )
{
    NTSTATUS status;
    const ULONG load_library_search_flags = (LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR |
                                             LOAD_LIBRARY_SEARCH_APPLICATION_DIR |
                                             LOAD_LIBRARY_SEARCH_USER_DIRS |
                                             LOAD_LIBRARY_SEARCH_SYSTEM32 |
                                             LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);

    if (flags & LOAD_WITH_ALTERED_SEARCH_PATH)
    {
        if (flags & load_library_search_flags) 
			return STATUS_INVALID_PARAMETER;
        if (default_search_flags) 
			flags |= default_search_flags | LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR;
    }
    else if (!(flags & load_library_search_flags)) flags |= default_search_flags;

    RtlEnterCriticalSection( &dlldir_section );

    if (flags & load_library_search_flags)
    {
        status = get_dll_load_path_search_flags( module, flags, path );
    }
    else
    {
        const WCHAR *dlldir = dll_directory.Length ? dll_directory.Buffer : NULL;
        if (!(flags & LOAD_WITH_ALTERED_SEARCH_PATH))
            module = NtCurrentTeb()->ProcessEnvironmentBlock->ProcessParameters->ImagePathName.Buffer;
        status = get_dll_load_path( module, dlldir, dll_safe_mode, path );
    }

    RtlLeaveCriticalSection( &dlldir_section );
    *unknown = NULL;
    return status;
}

/****************************************************************************
 *		LdrGetDllDirectory  (NTDLL.@)
 */
NTSTATUS WINAPI LdrGetDllDirectory( UNICODE_STRING *dir )
{
    NTSTATUS status = STATUS_SUCCESS;

    RtlEnterCriticalSection( &dlldir_section );
    dir->Length = dll_directory.Length + sizeof(WCHAR);
    if (dir->MaximumLength >= dir->Length) RtlCopyUnicodeString( dir, &dll_directory );
    else
    {
        status = STATUS_BUFFER_TOO_SMALL;
        if (dir->MaximumLength) dir->Buffer[0] = 0;
    }
    RtlLeaveCriticalSection( &dlldir_section );
    return status;
}


/****************************************************************************
 *		LdrSetDllDirectory  (NTDLL.@)
 */
NTSTATUS WINAPI LdrSetDllDirectory( const UNICODE_STRING *dir )
{
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING new;

    if (!dir->Buffer) RtlInitUnicodeString( &new, NULL );
    else if ((status = RtlDuplicateUnicodeString( 1, dir, &new ))) return status;

    RtlEnterCriticalSection( &dlldir_section );
    RtlFreeUnicodeString( &dll_directory );
    dll_directory = new;
    RtlLeaveCriticalSection( &dlldir_section );
    return status;
}


/****************************************************************************
 *		LdrAddDllDirectory  (NTDLL.@)
 */
NTSTATUS WINAPI LdrAddDllDirectory( const UNICODE_STRING *dir, void **cookie )
{
    FILE_BASIC_INFORMATION info;
    UNICODE_STRING nt_name;
    NTSTATUS status;
    OBJECT_ATTRIBUTES attr;
    DWORD len;
    struct dll_dir_entry *ptr;
    DOS_PATHNAME_TYPE type = RtlDetermineDosPathNameType_U( dir->Buffer );

    if (type != ABSOLUTE_PATH && type != ABSOLUTE_DRIVE_PATH)
        return STATUS_INVALID_PARAMETER;

    status = RtlDosPathNameToNtPathName_U_WithStatus( dir->Buffer, &nt_name, NULL, NULL );
    if (status) return status;
    len = nt_name.Length / sizeof(WCHAR);
    if (!(ptr = RtlAllocateHeap( RtlProcessHeap(), 0, offsetof(struct dll_dir_entry, dir[++len] ))))
        return STATUS_NO_MEMORY;
    memcpy( ptr->dir, nt_name.Buffer, len * sizeof(WCHAR) );

    attr.Length = sizeof(attr);
    attr.RootDirectory = 0;
    attr.Attributes = OBJ_CASE_INSENSITIVE;
    attr.ObjectName = &nt_name;
    attr.SecurityDescriptor = NULL;
    attr.SecurityQualityOfService = NULL;
    status = NtQueryAttributesFile( &attr, &info );
    RtlFreeUnicodeString( &nt_name );

    if (!status)
    {
        DbgPrint( "%s\n", ptr->dir );
        RtlEnterCriticalSection( &dlldir_section );
        list_add_head( &dll_dir_list, &ptr->entry );
        RtlLeaveCriticalSection( &dlldir_section );
        *cookie = ptr;
    }
    else RtlFreeHeap( RtlProcessHeap(), 0, ptr );
    return status;
}


/****************************************************************************
 *		LdrRemoveDllDirectory  (NTDLL.@)
 */
NTSTATUS WINAPI LdrRemoveDllDirectory( void *cookie )
{
    struct dll_dir_entry *ptr = cookie;

    DbgPrint( "%s\n", ptr->dir);

    RtlEnterCriticalSection( &dlldir_section );
    list_remove( &ptr->entry );
    RtlFreeHeap( RtlProcessHeap(), 0, ptr );
    RtlLeaveCriticalSection( &dlldir_section );
    return STATUS_SUCCESS;
}

/*************************************************************************
 *		RtlSetSearchPathMode (NTDLL.@)
 */
NTSTATUS WINAPI RtlSetSearchPathMode( ULONG flags )
{
    int val;

    switch (flags)
    {
    case BASE_SEARCH_PATH_ENABLE_SAFE_SEARCHMODE:
        val = 1;
        break;
    case BASE_SEARCH_PATH_DISABLE_SAFE_SEARCHMODE:
        val = 0;
        break;
    case BASE_SEARCH_PATH_ENABLE_SAFE_SEARCHMODE | BASE_SEARCH_PATH_PERMANENT:
        InterlockedExchange( (int *)&path_safe_mode, 2 );
        return STATUS_SUCCESS;
    default:
        return STATUS_INVALID_PARAMETER;
    }

    for (;;)
    {
        int prev = path_safe_mode;
        if (prev == 2) break;  /* permanently set */
        if (InterlockedCompareExchange( (int *)&path_safe_mode, val, prev ) == prev) return STATUS_SUCCESS;
    }
    return STATUS_ACCESS_DENIED;
}


/******************************************************************
 *           RtlGetExePath   (NTDLL.@)
 */
NTSTATUS WINAPI RtlGetExePath( PCWSTR name, PWSTR *path )
{
    static const WCHAR emptyW[1];
    const WCHAR *dlldir = dotW;
    const WCHAR *module = NtCurrentTeb()->ProcessEnvironmentBlock->ProcessParameters->ImagePathName.Buffer;

    /* same check as NeedCurrentDirectoryForExePathW */
    if (!wcschr( name, '\\' ))
    {
        static const WCHAR env_name[] = {'N','o','D','e','f','a','u','l','t','C','u','r','r','e','n','t',
                                         'D','i','r','e','c','t','o','r','y','I','n',
                                         'E','x','e','P','a','t','h',0};
        UNICODE_STRING name, value = { 0 };

        RtlInitUnicodeString( &name, env_name );
        if (RtlQueryEnvironmentVariable_U( NULL, &name, &value ) != STATUS_VARIABLE_NOT_FOUND)
            dlldir = emptyW;
    }
    return get_dll_load_path( module, dlldir, FALSE, path );
}

/******************************************************************
 *           RtlGetSearchPath   (NTDLL.@)
 */
NTSTATUS WINAPI RtlGetSearchPath( PWSTR *path )
{
    const WCHAR *module = NtCurrentTeb()->ProcessEnvironmentBlock->ProcessParameters->ImagePathName.Buffer;
    return get_dll_load_path( module, NULL, path_safe_mode, path );
}

/******************************************************************
 *           RtlReleasePath   (NTDLL.@)
 */
void WINAPI RtlReleasePath( PWSTR path )
{
    RtlFreeHeap( RtlProcessHeap(), 0, path );
}

/***********************************************************************
 *           load_global_options
 */
void load_global_options(void)
{
    static const WCHAR sessionW[] = {'M','a','c','h','i','n','e','\\',
                                     'S','y','s','t','e','m','\\',
                                     'C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\',
                                     'C','o','n','t','r','o','l','\\',
                                     'S','e','s','s','i','o','n',' ','M','a','n','a','g','e','r',0};
    static const WCHAR globalflagW[] = {'G','l','o','b','a','l','F','l','a','g',0};
    static const WCHAR safesearchW[] = {'S','a','f','e','P','r','o','c','e','s','s','S','e','a','r','c','h','M','o','d','e',0};
    static const WCHAR safedllmodeW[] = {'S','a','f','e','D','l','l','S','e','a','r','c','h','M','o','d','e',0};
    static const WCHAR critsectW[] = {'C','r','i','t','i','c','a','l','S','e','c','t','i','o','n','T','i','m','e','o','u','t',0};
    static const WCHAR heapresW[] = {'H','e','a','p','S','e','g','m','e','n','t','R','e','s','e','r','v','e',0};
    static const WCHAR heapcommitW[] = {'H','e','a','p','S','e','g','m','e','n','t','C','o','m','m','i','t',0};
    static const WCHAR decommittotalW[] = {'H','e','a','p','D','e','C','o','m','m','i','t','T','o','t','a','l','F','r','e','e','T','h','r','e','s','h','o','l','d',0};
    static const WCHAR decommitfreeW[] = {'H','e','a','p','D','e','C','o','m','m','i','t','F','r','e','e','B','l','o','c','k','T','h','r','e','s','h','o','l','d',0};

    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING name_str;
    HANDLE hkey;
    //ULONG value;

    attr.Length = sizeof(attr);
    attr.RootDirectory = 0;
    attr.ObjectName = &name_str;
    attr.Attributes = OBJ_CASE_INSENSITIVE;
    attr.SecurityDescriptor = NULL;
    attr.SecurityQualityOfService = NULL;
    RtlInitUnicodeString( &name_str, sessionW );

    if (!NtOpenKey( &hkey, KEY_QUERY_VALUE, &attr ))
    {
        query_dword_option( hkey, globalflagW, &NtCurrentTeb()->ProcessEnvironmentBlock->NtGlobalFlag );
        query_dword_option( hkey, safesearchW, &path_safe_mode );
        query_dword_option( hkey, safedllmodeW, &dll_safe_mode );

        // if (!query_dword_option( hkey, critsectW, &value ))
            // NtCurrentTeb()->Peb->CriticalSectionTimeout.QuadPart = (ULONGLONG)value * -10000000;

        // if (!query_dword_option( hkey, heapresW, &value ))
            // NtCurrentTeb()->Peb->HeapSegmentReserve = value;

        // if (!query_dword_option( hkey, heapcommitW, &value ))
            // NtCurrentTeb()->Peb->HeapSegmentCommit = value;

        // if (!query_dword_option( hkey, decommittotalW, &value ))
            // NtCurrentTeb()->Peb->HeapDeCommitTotalFreeThreshold = value;

        // if (!query_dword_option( hkey, decommitfreeW, &value ))
            // NtCurrentTeb()->Peb->HeapDeCommitFreeBlockThreshold = value;

        NtClose( hkey );
    }
    // LdrQueryImageFileExecutionOptions( &NtCurrentTeb()->Peb->ProcessParameters->ImagePathName,
                                       // globalflagW, REG_DWORD, &NtCurrentTeb()->Peb->NtGlobalFlag,
                                       // sizeof(DWORD), NULL );
    // heap_set_debug_flags( RtlProcessHeap() );
}