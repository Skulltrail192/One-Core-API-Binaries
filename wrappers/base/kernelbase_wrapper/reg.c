/*++

Copyright (c) 2021 Shorthorn Project

Module Name:

    reg.c

Abstract:

    This module implements Registry Manage functions for the Win32 APIs.

Author:

    Skulltrail 18-March-2021

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(kernel32_wrapper);

static const UNICODE_STRING HKLM_ClassesPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\Software\\Classes");

/* DEFINES ******************************************************************/

#define MAX_DEFAULT_HANDLES   6
#define REG_MAX_NAME_SIZE     256
#define REG_MAX_DATA_SIZE     2048
#define HKEY_SPECIAL_ROOT_FIRST   HKEY_CLASSES_ROOT
#define HKEY_SPECIAL_ROOT_LAST    HKEY_DYN_DATA

/* GLOBALS ******************************************************************/

static RTL_CRITICAL_SECTION HandleTableCS;
static HANDLE DefaultHandleTable[MAX_DEFAULT_HANDLES];
static HANDLE ProcessHeap;
static BOOLEAN DefaultHandlesDisabled = FALSE;
static BOOLEAN DefaultHandleHKUDisabled = FALSE;
static BOOLEAN DllInitialized = FALSE; /* HACK */

/* PROTOTYPES ***************************************************************/

static NTSTATUS MapDefaultKey (PHANDLE ParentKey, HKEY Key);
static VOID CloseDefaultKeys(VOID);
#define ClosePredefKey(Handle)                                                 \
    if ((ULONG_PTR)Handle & 0x1) {                                             \
        NtClose(Handle);                                                       \
    }
#define IsPredefKey(HKey)                                                      \
    (((ULONG_PTR)(HKey) & 0xF0000000) == 0x80000000)
#define GetPredefKeyIndex(HKey)                                                \
    ((ULONG_PTR)(HKey) & 0x0FFFFFFF)
	
static NTSTATUS OpenClassesRootKey(PHANDLE KeyHandle);
static NTSTATUS OpenLocalMachineKey (PHANDLE KeyHandle);
static NTSTATUS OpenUsersKey (PHANDLE KeyHandle);
static NTSTATUS OpenCurrentConfigKey(PHANDLE KeyHandle);	
	
/* FUNCTIONS ****************************************************************/
/* check if value type needs string conversion (Ansi<->Unicode) */
__inline static int is_string( DWORD type )
{
    return (type == REG_SZ) || (type == REG_EXPAND_SZ) || (type == REG_MULTI_SZ);
}	
	
static const WCHAR * const root_key_names[] =
{
    L"\\Registry\\Machine\\Software\\Classes",
    NULL,         /* HKEY_CURRENT_USER is determined dynamically */
    L"\\Registry\\Machine",
    L"\\Registry\\User",
    L"\\Registry\\PerfData",
    L"\\Registry\\Machine\\System\\CurrentControlSet\\Hardware Profiles\\Current",
    L"\\Registry\\DynData"
};

static HKEY special_root_keys[ARRAY_SIZE(root_key_names)];
static BOOL cache_disabled[ARRAY_SIZE(root_key_names)];

/************************************************************************
 *  RegInitDefaultHandles
 */
BOOL
RegInitialize(VOID)
{
    TRACE("RegInitialize()\n");

    /* Lazy init hack */
    if (!DllInitialized)
    {
        ProcessHeap = RtlGetProcessHeap();
        RtlZeroMemory(DefaultHandleTable,
                      MAX_DEFAULT_HANDLES * sizeof(HANDLE));
        RtlInitializeCriticalSection(&HandleTableCS);

        DllInitialized = TRUE;
    }

    return TRUE;
}

static NTSTATUS
OpenPredefinedKey(IN ULONG Index,
                  OUT HANDLE Handle)
{
    NTSTATUS Status;

    switch (Index)
    {
        case 0: /* HKEY_CLASSES_ROOT */
            Status = OpenClassesRootKey (Handle);
            break;

        case 1: /* HKEY_CURRENT_USER */
            Status = RtlOpenCurrentUser (MAXIMUM_ALLOWED,
                                         Handle);
            break;

        case 2: /* HKEY_LOCAL_MACHINE */
            Status = OpenLocalMachineKey (Handle);
            break;

        case 3: /* HKEY_USERS */
            Status = OpenUsersKey (Handle);
            break;
#if 0
        case 4: /* HKEY_PERFORMANCE_DATA */
            Status = OpenPerformanceDataKey (Handle);
            break;
#endif

        case 5: /* HKEY_CURRENT_CONFIG */
            Status = OpenCurrentConfigKey (Handle);
            break;

        case 6: /* HKEY_DYN_DATA */
            Status = STATUS_NOT_IMPLEMENTED;
            break;

        default:
            WARN("MapDefaultHandle() no handle creator\n");
            Status = STATUS_INVALID_PARAMETER;
            break;
    }

    return Status;
}

static NTSTATUS
MapDefaultKey(OUT PHANDLE RealKey,
              IN HKEY Key)
{
    PHANDLE Handle;
    ULONG Index;
    BOOLEAN DoOpen, DefDisabled;
    NTSTATUS Status = STATUS_SUCCESS;

    TRACE("MapDefaultKey (Key %x)\n", Key);

    if (!IsPredefKey(Key))
    {
        *RealKey = (HANDLE)((ULONG_PTR)Key & ~0x1);
        return STATUS_SUCCESS;
    }

    /* Handle special cases here */
    Index = GetPredefKeyIndex(Key);
    if (Index >= MAX_DEFAULT_HANDLES)
    {
        return STATUS_INVALID_PARAMETER;
    }
    RegInitialize(); /* HACK until delay-loading is implemented */
    RtlEnterCriticalSection (&HandleTableCS);

    if (Key == HKEY_CURRENT_USER)
        DefDisabled = DefaultHandleHKUDisabled;
    else
        DefDisabled = DefaultHandlesDisabled;

    if (!DefDisabled)
    {
        Handle = &DefaultHandleTable[Index];
        DoOpen = (*Handle == NULL);
    }
    else
    {
        Handle = RealKey;
        DoOpen = TRUE;
    }

    if (DoOpen)
    {
        /* create/open the default handle */
        Status = OpenPredefinedKey(Index,
                                   Handle);
    }

    if (NT_SUCCESS(Status))
    {
        if (!DefDisabled)
            *RealKey = *Handle;
        else
            *(PULONG_PTR)Handle |= 0x1;
    }

    RtlLeaveCriticalSection (&HandleTableCS);

    return Status;
}

static VOID
CloseDefaultKeys(VOID)
{
    ULONG i;
    RegInitialize(); /* HACK until delay-loading is implemented */
    RtlEnterCriticalSection(&HandleTableCS);

    for (i = 0; i < MAX_DEFAULT_HANDLES; i++)
    {
        if (DefaultHandleTable[i] != NULL)
        {
            NtClose(DefaultHandleTable[i]);
            DefaultHandleTable[i] = NULL;
        }
    }

    RtlLeaveCriticalSection(&HandleTableCS);
}

static NTSTATUS
OpenClassesRootKey(_Out_ PHANDLE KeyHandle)
{
    OBJECT_ATTRIBUTES Attributes;
    UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\Software\\CLASSES");
    NTSTATUS Status;

    TRACE("OpenClassesRootKey()\n");

    InitializeObjectAttributes(&Attributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    Status = NtOpenKey(KeyHandle,
                       MAXIMUM_ALLOWED,
                       &Attributes);

    if (!NT_SUCCESS(Status))
        return Status;

    /* Mark it as HKCR */
    MakeHKCRKey((HKEY*)KeyHandle);

    return Status;
}

static NTSTATUS
OpenLocalMachineKey(PHANDLE KeyHandle)
{
    OBJECT_ATTRIBUTES Attributes;
    UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"\\Registry\\Machine");
    NTSTATUS Status;

    TRACE("OpenLocalMachineKey()\n");

    InitializeObjectAttributes(&Attributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    Status = NtOpenKey(KeyHandle,
                       MAXIMUM_ALLOWED,
                       &Attributes);

    TRACE("NtOpenKey(%wZ) => %08x\n", &KeyName, Status);

    return Status;
}

static NTSTATUS
OpenUsersKey(PHANDLE KeyHandle)
{
    OBJECT_ATTRIBUTES Attributes;
    UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"\\Registry\\User");

    TRACE("OpenUsersKey()\n");

    InitializeObjectAttributes(&Attributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    return NtOpenKey(KeyHandle,
                     MAXIMUM_ALLOWED,
                     &Attributes);
}

static NTSTATUS
OpenCurrentConfigKey (PHANDLE KeyHandle)
{
    OBJECT_ATTRIBUTES Attributes;
    UNICODE_STRING KeyName =
        RTL_CONSTANT_STRING(L"\\Registry\\Machine\\System\\CurrentControlSet\\Hardware Profiles\\Current");

    TRACE("OpenCurrentConfigKey()\n");

    InitializeObjectAttributes(&Attributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    return NtOpenKey(KeyHandle,
                     MAXIMUM_ALLOWED,
                     &Attributes);
}

/************************************************************************
 *  CreateNestedKey
 *
 *  Create key and all necessary intermediate keys
 */
static NTSTATUS
CreateNestedKey(PHKEY KeyHandle,
                POBJECT_ATTRIBUTES ObjectAttributes,
                PUNICODE_STRING ClassString,
                DWORD dwOptions,
                REGSAM samDesired,
                DWORD *lpdwDisposition)
{
    OBJECT_ATTRIBUTES LocalObjectAttributes;
    UNICODE_STRING LocalKeyName;
    ULONG Disposition;
    NTSTATUS Status;
    ULONG FullNameLength;
    ULONG Length;
    PWCHAR Ptr;
    HANDLE LocalKeyHandle;

    Status = NtCreateKey((PHANDLE) KeyHandle,
                         samDesired,
                         ObjectAttributes,
                         0,
                         ClassString,
                         dwOptions,
                         (PULONG)lpdwDisposition);
    TRACE("NtCreateKey(%wZ) called (Status %lx)\n", ObjectAttributes->ObjectName, Status);
    if (Status != STATUS_OBJECT_NAME_NOT_FOUND)
        return Status;

    /* Copy object attributes */
    RtlCopyMemory(&LocalObjectAttributes,
                  ObjectAttributes,
                  sizeof(OBJECT_ATTRIBUTES));
    RtlCreateUnicodeString(&LocalKeyName,
                           ObjectAttributes->ObjectName->Buffer);
    LocalObjectAttributes.ObjectName = &LocalKeyName;
    FullNameLength = LocalKeyName.Length / sizeof(WCHAR);

    LocalKeyHandle = NULL;

    /* Remove the last part of the key name and try to create the key again. */
    while (Status == STATUS_OBJECT_NAME_NOT_FOUND)
    {
        Ptr = wcsrchr(LocalKeyName.Buffer, '\\');
        if (Ptr == NULL || Ptr == LocalKeyName.Buffer)
        {
            Status = STATUS_UNSUCCESSFUL;
            break;
        }

        *Ptr = (WCHAR)0;
        LocalKeyName.Length = (USHORT)wcslen(LocalKeyName.Buffer) * sizeof(WCHAR);

        Status = NtCreateKey(&LocalKeyHandle,
                             KEY_CREATE_SUB_KEY,
                             &LocalObjectAttributes,
                             0,
                             NULL,
                             0,
                             &Disposition);
        TRACE("NtCreateKey(%wZ) called (Status %lx)\n", &LocalKeyName, Status);
    }

    if (!NT_SUCCESS(Status))
    {
        RtlFreeUnicodeString(&LocalKeyName);
        return Status;
    }

    /* Add removed parts of the key name and create them too. */
    Length = wcslen(LocalKeyName.Buffer);
    while (TRUE)
    {
        if (LocalKeyHandle)
            NtClose (LocalKeyHandle);

        LocalKeyName.Buffer[Length] = L'\\';
        Length = wcslen (LocalKeyName.Buffer);
        LocalKeyName.Length = Length * sizeof(WCHAR);

        if (Length == FullNameLength)
        {
            Status = NtCreateKey((PHANDLE) KeyHandle,
                                 samDesired,
                                 ObjectAttributes,
                                 0,
                                 ClassString,
                                 dwOptions,
                                 (PULONG)lpdwDisposition);
            break;
        }

        Status = NtCreateKey(&LocalKeyHandle,
                             KEY_CREATE_SUB_KEY,
                             &LocalObjectAttributes,
                             0,
                             NULL,
                             0,
                             &Disposition);
        TRACE("NtCreateKey(%wZ) called (Status %lx)\n", &LocalKeyName, Status);
        if (!NT_SUCCESS(Status))
            break;
    }

    RtlFreeUnicodeString(&LocalKeyName);

    return Status;
}

static BOOL is_wow6432node( const UNICODE_STRING *name )
{
    return (name->Length == 11 * sizeof(WCHAR) && !_wcsnicmp( name->Buffer, L"Wow6432Node", 11 ));
}

/* open the Wow6432Node subkey of the specified key */
static HANDLE open_wow6432node( HANDLE key )
{
    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING nameW;
    HANDLE ret;

    attr.Length = sizeof(attr);
    attr.RootDirectory = key;
    attr.ObjectName = &nameW;
    attr.Attributes = 0;
    attr.SecurityDescriptor = NULL;
    attr.SecurityQualityOfService = NULL;
    RtlInitUnicodeString( &nameW, L"Wow6432Node" );
    if (NtOpenKeyEx( &ret, MAXIMUM_ALLOWED, &attr, 0 )) ret = 0;
    return ret;
}

/******************************************************************************
 * RemapPredefinedHandleInternal   (kernelbase.@)
 */
NTSTATUS WINAPI RemapPredefinedHandleInternal( HKEY hkey, HKEY override )
{
    HKEY old_key;
    int idx;

    TRACE("(%p %p)\n", hkey, override);

    if ((HandleToUlong(hkey) < HandleToUlong(HKEY_SPECIAL_ROOT_FIRST))
            || (HandleToUlong(hkey) > HandleToUlong(HKEY_SPECIAL_ROOT_LAST)))
        return STATUS_INVALID_HANDLE;
    idx = HandleToUlong(hkey) - HandleToUlong(HKEY_SPECIAL_ROOT_FIRST);

    if (override)
    {
        NTSTATUS status = NtDuplicateObject( GetCurrentProcess(), override,
                                             GetCurrentProcess(), (HANDLE *)&override,
                                             0, 0, DUPLICATE_SAME_ACCESS );
        if (status) return status;
    }

    old_key = InterlockedExchangePointer( (void **)&special_root_keys[idx], override );
    if (old_key) NtClose( old_key );
    return STATUS_SUCCESS;
}

/******************************************************************************
 * DisablePredefinedHandleTableInternal (kernelbase.@)
 */
NTSTATUS WINAPI DisablePredefinedHandleTableInternal( HKEY hkey )
{
    HKEY old_key;
    int idx;

    TRACE("(%p)\n", hkey);

    if ((HandleToUlong(hkey) < HandleToUlong(HKEY_SPECIAL_ROOT_FIRST))
            || (HandleToUlong(hkey) > HandleToUlong(HKEY_SPECIAL_ROOT_LAST)))
        return STATUS_INVALID_HANDLE;
    idx = HandleToUlong(hkey) - HandleToUlong(HKEY_SPECIAL_ROOT_FIRST);

    cache_disabled[idx] = TRUE;

    old_key = InterlockedExchangePointer( (void **)&special_root_keys[idx], NULL );
    if (old_key) NtClose( old_key );
    return STATUS_SUCCESS;
}


/* wrapper for NtCreateKey that creates the key recursively if necessary */
static NTSTATUS create_key( HKEY *retkey, ACCESS_MASK access, OBJECT_ATTRIBUTES *attr,
                            const UNICODE_STRING *class, ULONG options, PULONG dispos )
{
    BOOL force_wow32 = is_win64 && (access & KEY_WOW64_32KEY);
    NTSTATUS status = STATUS_OBJECT_NAME_NOT_FOUND;
    HANDLE subkey, root = attr->RootDirectory;

    if (!force_wow32) status = NtCreateKey( &subkey, access, attr, 0, class, options, dispos );

    if (status == STATUS_OBJECT_NAME_NOT_FOUND)
    {
        WCHAR *buffer = attr->ObjectName->Buffer;
        DWORD attrs, pos = 0, i = 0, len = attr->ObjectName->Length / sizeof(WCHAR);
        UNICODE_STRING str;

        /* don't try to create registry root */
        if (!attr->RootDirectory && len > 10 && !_wcsnicmp( buffer, L"\\Registry\\", 10 )) i += 10;

        while (i < len && buffer[i] != '\\') i++;
        if (i == len && !force_wow32) return status;

        attrs = attr->Attributes;
        attr->ObjectName = &str;

        for (;;)
        {
            str.Buffer = buffer + pos;
            str.Length = (i - pos) * sizeof(WCHAR);
            if (force_wow32 && pos)
            {
                if (is_wow6432node( &str )) force_wow32 = FALSE;
                else if ((subkey = open_wow6432node( attr->RootDirectory )))
                {
                    if (attr->RootDirectory != root) NtClose( attr->RootDirectory );
                    attr->RootDirectory = subkey;
                    force_wow32 = FALSE;
                }
            }
            if (i == len)
            {
                attr->Attributes = attrs;
                status = NtCreateKey( &subkey, access, attr, 0, class, options, dispos );
            }
            else
            {
                attr->Attributes = attrs & ~OBJ_OPENLINK;
                status = NtCreateKey( &subkey, access, attr, 0, class,
                                      options & ~REG_OPTION_CREATE_LINK, dispos );
            }
            if (attr->RootDirectory != root) NtClose( attr->RootDirectory );
            if (status) return status;
            if (i == len) break;
            attr->RootDirectory = subkey;
            while (i < len && buffer[i] == '\\') i++;
            pos = i;
            while (i < len && buffer[i] != '\\') i++;
        }
    }
    attr->RootDirectory = subkey;
    if (force_wow32 && (subkey = open_wow6432node( attr->RootDirectory )))
    {
        if (attr->RootDirectory != root) NtClose( attr->RootDirectory );
        attr->RootDirectory = subkey;
    }
    *retkey = attr->RootDirectory;
    return status;
}

/* create one of the HKEY_* special root keys */
static HKEY create_special_root_hkey( HKEY hkey, DWORD access )
{
    HKEY ret = 0;
    int idx = HandleToUlong(hkey) - HandleToUlong(HKEY_SPECIAL_ROOT_FIRST);

    if (HandleToUlong(hkey) == HandleToUlong(HKEY_CURRENT_USER))
    {
        if (RtlOpenCurrentUser( access, (HANDLE *)&hkey )) return 0;
        TRACE( "HKEY_CURRENT_USER -> %p\n", hkey );
    }
    else
    {
        OBJECT_ATTRIBUTES attr;
        UNICODE_STRING name;

        attr.Length = sizeof(attr);
        attr.RootDirectory = 0;
        attr.ObjectName = &name;
        attr.Attributes = 0;
        attr.SecurityDescriptor = NULL;
        attr.SecurityQualityOfService = NULL;
        RtlInitUnicodeString( &name, root_key_names[idx] );
        if (create_key( &hkey, access, &attr, NULL, 0, NULL )) return 0;
        TRACE( "%s -> %p\n", debugstr_w(attr.ObjectName->Buffer), hkey );
    }

    if (!cache_disabled[idx] && !(access & (KEY_WOW64_64KEY | KEY_WOW64_32KEY)))
    {
        if (!(ret = InterlockedCompareExchangePointer( (void **)&special_root_keys[idx], hkey, 0 )))
            ret = hkey;
        else
            NtClose( hkey );  /* somebody beat us to it */
    }
    else
        ret = hkey;
    return ret;
}

/* map the hkey from special root to normal key if necessary */
static inline HKEY get_special_root_hkey( HKEY hkey, REGSAM access )
{
    HKEY ret = hkey;

    if ((HandleToUlong(hkey) >= HandleToUlong(HKEY_SPECIAL_ROOT_FIRST))
            && (HandleToUlong(hkey) <= HandleToUlong(HKEY_SPECIAL_ROOT_LAST)))
    {
        REGSAM mask = 0;

        if (HandleToUlong(hkey) == HandleToUlong(HKEY_CLASSES_ROOT))
            mask = KEY_WOW64_32KEY | KEY_WOW64_64KEY;

        if ((access & mask) ||
                !(ret = special_root_keys[HandleToUlong(hkey) - HandleToUlong(HKEY_SPECIAL_ROOT_FIRST)]))
            ret = create_special_root_hkey( hkey, MAXIMUM_ALLOWED | (access & mask) );
    }
    return ret;
}

/******************************************************************************
 * RegCopyTreeW (kernelbase.@)
 *
 */
LSTATUS WINAPI RegCopyTreeW( HKEY hsrc, const WCHAR *subkey, HKEY hdst )
{
    DWORD name_size, max_name;
    DWORD value_size, max_value;
    DWORD max_subkey, i, type;
    WCHAR *name_buf = NULL;
    BYTE *value_buf = NULL;
    HKEY hkey;
    LONG ret;

    TRACE( "(%p, %s, %p)\n", hsrc, debugstr_w(subkey), hdst );

    if (subkey)
    {
        ret = RegOpenKeyExW( hsrc, subkey, 0, KEY_READ, &hsrc );
        if (ret) return ret;
    }

    ret = RegQueryInfoKeyW( hsrc, NULL, NULL, NULL, NULL, &max_subkey,
                            NULL, NULL, &max_name, &max_value, NULL, NULL );
    if (ret)
        goto cleanup;

    max_name = max( max_subkey, max_name ) + 1;
    if (!(name_buf =  HeapAlloc(GetProcessHeap(), 0, max_name * sizeof(WCHAR))))
    {
        ret = ERROR_NOT_ENOUGH_MEMORY;
        goto cleanup;
    }

    if (!(value_buf = HeapAlloc(GetProcessHeap(), 0, max_value)))
    {
        ret = ERROR_NOT_ENOUGH_MEMORY;
        goto cleanup;
    }

    /* Copy values */
    for (i = 0;; i++)
    {
        name_size = max_name;
        value_size = max_value;
        ret = RegEnumValueW( hsrc, i, name_buf, &name_size, NULL, &type, value_buf, &value_size );
        if (ret == ERROR_NO_MORE_ITEMS) break;
        if (ret) goto cleanup;
        ret = RegSetValueExW( hdst, name_buf, 0, type, value_buf, value_size );
        if (ret) goto cleanup;
    }

    /* Recursively copy subkeys */
    for (i = 0;; i++)
    {
        name_size = max_name;
        ret = RegEnumKeyExW( hsrc, i, name_buf, &name_size, NULL, NULL, NULL, NULL );
        if (ret == ERROR_NO_MORE_ITEMS) break;
        if (ret) goto cleanup;
        ret = RegCreateKeyExW( hdst, name_buf, 0, NULL, 0, KEY_WRITE, NULL, &hkey, NULL );
        if (ret) goto cleanup;
        ret = RegCopyTreeW( hsrc, name_buf, hkey );
        RegCloseKey( hkey );
        if (ret) goto cleanup;
    }

    ret = ERROR_SUCCESS;

cleanup:
    HeapFree( GetProcessHeap(), 0, name_buf );
    HeapFree( GetProcessHeap(), 0, value_buf );
    if (subkey)
        RegCloseKey( hsrc );
    return ret;
}

/******************************************************************************
 * RegDeleteKeyValueA   (kernelbase.@)
 */
LONG WINAPI RegDeleteKeyValueA( HKEY hkey, LPCSTR subkey, LPCSTR name )
{
    UNICODE_STRING nameW;
    HKEY hsubkey = 0;
    ANSI_STRING nameA;
    NTSTATUS status;

    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    if (subkey)
    {
        LONG ret = RegOpenKeyExA( hkey, subkey, 0, KEY_SET_VALUE, &hsubkey );
        if (ret)
            return ret;
        hkey = hsubkey;
    }

    RtlInitAnsiString( &nameA, name );
    if (!(status = RtlAnsiStringToUnicodeString( &nameW, &nameA, TRUE )))
    {
        status = NtDeleteValueKey( hkey, &nameW );
        RtlFreeUnicodeString( &nameW );
    }

    if (hsubkey) RegCloseKey( hsubkey );
    return RtlNtStatusToDosError( status );
}


/******************************************************************************
 * RegDeleteKeyValueW   (kernelbase.@)
 */
LONG WINAPI RegDeleteKeyValueW( HKEY hkey, LPCWSTR subkey, LPCWSTR name )
{
    UNICODE_STRING nameW;
    HKEY hsubkey = 0;
    LONG ret;

    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    if (subkey)
    {
        if ((ret = RegOpenKeyExW( hkey, subkey, 0, KEY_SET_VALUE, &hsubkey )))
            return ret;
        hkey = hsubkey;
    }

    RtlInitUnicodeString( &nameW, name );
    ret = RtlNtStatusToDosError( NtDeleteValueKey( hkey, &nameW ) );
    if (hsubkey) RegCloseKey( hsubkey );
    return ret;
}

/******************************************************************************
 * RegDeleteTreeW (kernelbase.@)
 *
 */
LSTATUS WINAPI RegDeleteTreeW( HKEY hkey, const WCHAR *subkey )
{
    DWORD name_size, max_name, max_subkey;
    WCHAR *name_buf = NULL;
    LONG ret;

    TRACE( "(%p, %s)\n", hkey, debugstr_w(subkey) );

    if (subkey && *subkey)
    {
        ret = RegOpenKeyExW( hkey, subkey, 0, KEY_READ, &hkey );
        if (ret) return ret;
    }

    ret = RegQueryInfoKeyW( hkey, NULL, NULL, NULL, NULL, &max_subkey,
                            NULL, NULL, &max_name, NULL, NULL, NULL );
    if (ret)
        goto cleanup;

    max_name = max( max_subkey, max_name ) + 1;
    if (!(name_buf = HeapAlloc(GetProcessHeap(), 0, max_name * sizeof(WCHAR))))
    {
        ret = ERROR_NOT_ENOUGH_MEMORY;
        goto cleanup;
    }

    /* Recursively delete subkeys */
    for (;;)
    {
        name_size = max_name;
        ret = RegEnumKeyExW( hkey, 0, name_buf, &name_size, NULL, NULL, NULL, NULL );
        if (ret == ERROR_NO_MORE_ITEMS) break;
        if (ret) goto cleanup;
        ret = RegDeleteTreeW( hkey, name_buf );
        if (ret) goto cleanup;
    }

    /* Delete the key itself */
    if (subkey && *subkey)
    {
        ret = RegDeleteKeyExW( hkey, L"", 0, 0 );
        goto cleanup;
    }

    /* Delete values */
    for (;;)
    {
        name_size = max_name;
        ret = RegEnumValueW( hkey, 0, name_buf, &name_size, NULL, NULL, NULL, NULL );
        if (ret == ERROR_NO_MORE_ITEMS) break;
        if (ret) goto cleanup;
        ret = RegDeleteValueW( hkey, name_buf );
        if (ret) goto cleanup;
    }

    ret = ERROR_SUCCESS;

cleanup:
	HeapFree(GetProcessHeap(), 0, name_buf);
    if (subkey && *subkey)
        RegCloseKey( hkey );
    return ret;
}

/******************************************************************************
 * RegDeleteTreeA (kernelbase.@)
 *
 */
LSTATUS WINAPI RegDeleteTreeA( HKEY hkey, const char *subkey )
{
    UNICODE_STRING subkeyW;
    LONG ret;

    if (subkey) RtlCreateUnicodeStringFromAsciiz( &subkeyW, subkey );
    else subkeyW.Buffer = NULL;
    ret = RegDeleteTreeW( hkey, subkeyW.Buffer );
    RtlFreeUnicodeString( &subkeyW );
    return ret;
}

/************************************************************************
 *  RegDisablePredefinedCacheEx
 *
 * @implemented
 */
LONG WINAPI
RegDisablePredefinedCacheEx(VOID)
{
    RegInitialize(); /* HACK until delay-loading is implemented */
    RtlEnterCriticalSection(&HandleTableCS);
    DefaultHandlesDisabled = TRUE;
    DefaultHandleHKUDisabled = TRUE;
    RtlLeaveCriticalSection(&HandleTableCS);
    return ERROR_SUCCESS;
}

/******************************************************************************
 * RegSetKeyValueA   (kernelbase.@)
 */
LONG WINAPI RegSetKeyValueA( HKEY hkey, LPCSTR subkey, LPCSTR name, DWORD type, const void *data, DWORD len )
{
    HKEY hsubkey = NULL;
    DWORD ret;

    TRACE("(%p,%s,%s,%d,%p,%d)\n", hkey, debugstr_a(subkey), debugstr_a(name), type, data, len );

    if (subkey && subkey[0])  /* need to create the subkey */
    {
        if ((ret = RegCreateKeyExA( hkey, subkey, 0, NULL, REG_OPTION_NON_VOLATILE,
                                    KEY_SET_VALUE, NULL, &hsubkey, NULL )) != ERROR_SUCCESS) return ret;
        hkey = hsubkey;
    }

    ret = RegSetValueExA( hkey, name, 0, type, (const BYTE*)data, len );
    if (hsubkey) RegCloseKey( hsubkey );
    return ret;
}

/******************************************************************************
 * RegSetKeyValueW   (kernelbase.@)
 */
LONG WINAPI RegSetKeyValueW( HKEY hkey, LPCWSTR subkey, LPCWSTR name, DWORD type, const void *data, DWORD len )
{
    HKEY hsubkey = NULL;
    DWORD ret;

    TRACE("(%p,%s,%s,%d,%p,%d)\n", hkey, debugstr_w(subkey), debugstr_w(name), type, data, len );

    if (subkey && subkey[0])  /* need to create the subkey */
    {
        if ((ret = RegCreateKeyExW( hkey, subkey, 0, NULL, REG_OPTION_NON_VOLATILE,
                                    KEY_SET_VALUE, NULL, &hsubkey, NULL )) != ERROR_SUCCESS) return ret;
        hkey = hsubkey;
    }

    ret = RegSetValueExW( hkey, name, 0, type, (const BYTE*)data, len );
    if (hsubkey) RegCloseKey( hsubkey );
    return ret;
}

/******************************************************************************
 * load_string [Internal]
 *
 * This is basically a copy of user32/resource.c's LoadStringW. Necessary to
 * avoid importing user32, which is higher level than advapi32. Helper for
 * RegLoadMUIString.
 */
static int load_string(HINSTANCE hModule, UINT resId, LPWSTR pwszBuffer, INT cMaxChars)
{
    HGLOBAL hMemory;
    HRSRC hResource;
    WCHAR *pString;
    int idxString;

    /* Negative values have to be inverted. */
    if (HIWORD(resId) == 0xffff)
        resId = (UINT)(-((INT)resId));

    /* Load the resource into memory and get a pointer to it. */
    hResource = FindResourceW(hModule, MAKEINTRESOURCEW(LOWORD(resId >> 4) + 1), (LPWSTR)RT_STRING);
    if (!hResource) return 0;
    hMemory = LoadResource(hModule, hResource);
    if (!hMemory) return 0;
    pString = LockResource(hMemory);

    /* Strings are length-prefixed. Lowest nibble of resId is an index. */
    idxString = resId & 0xf;
    while (idxString--) pString += *pString + 1;

    /* If no buffer is given, return length of the string. */
    if (!pwszBuffer) return *pString;

    /* Else copy over the string, respecting the buffer size. */
    cMaxChars = (*pString < cMaxChars) ? *pString : (cMaxChars - 1);
    if (cMaxChars >= 0)
    {
        memcpy(pwszBuffer, pString+1, cMaxChars * sizeof(WCHAR));
        pwszBuffer[cMaxChars] = L'\0';
    }

    return cMaxChars;
}

/************************************************************************
 *  RegLoadMUIStringW
 *
 * @implemented
 */
LONG WINAPI
RegLoadMUIStringW(IN HKEY hKey,
                  IN LPCWSTR pszValue  OPTIONAL,
                  OUT LPWSTR pszOutBuf,
                  IN DWORD cbOutBuf,
                  OUT LPDWORD pcbData OPTIONAL,
                  IN DWORD Flags,
                  IN LPCWSTR pszDirectory  OPTIONAL)
{
    DWORD dwValueType, cbData;
    LPWSTR pwszTempBuffer = NULL, pwszExpandedBuffer = NULL;
    LONG result;

    /* Parameter sanity checks. */
    if (!hKey || !pszOutBuf)
        return ERROR_INVALID_PARAMETER;

    if (pszDirectory && *pszDirectory)
    {
        FIXME("BaseDir parameter not yet supported!\n");
        return ERROR_INVALID_PARAMETER;
    }

    /* Check for value existence and correctness of it's type, allocate a buffer and load it. */
    result = RegQueryValueExW(hKey, pszValue, NULL, &dwValueType, NULL, &cbData);
    if (result != ERROR_SUCCESS) goto cleanup;
    if (!(dwValueType == REG_SZ || dwValueType == REG_EXPAND_SZ) || !cbData)
    {
        result = ERROR_FILE_NOT_FOUND;
        goto cleanup;
    }
    pwszTempBuffer = HeapAlloc(GetProcessHeap(), 0, cbData);
    if (!pwszTempBuffer)
    {
        result = ERROR_NOT_ENOUGH_MEMORY;
        goto cleanup;
    }
    result = RegQueryValueExW(hKey, pszValue, NULL, &dwValueType, (LPBYTE)pwszTempBuffer, &cbData);
    if (result != ERROR_SUCCESS) goto cleanup;

    /* Expand environment variables, if appropriate, or copy the original string over. */
    if (dwValueType == REG_EXPAND_SZ)
    {
        cbData = ExpandEnvironmentStringsW(pwszTempBuffer, NULL, 0) * sizeof(WCHAR);
        if (!cbData) goto cleanup;
        pwszExpandedBuffer = HeapAlloc(GetProcessHeap(), 0, cbData);
        if (!pwszExpandedBuffer)
        {
            result = ERROR_NOT_ENOUGH_MEMORY;
            goto cleanup;
        }
        ExpandEnvironmentStringsW(pwszTempBuffer, pwszExpandedBuffer, cbData);
    }
    else
    {
        pwszExpandedBuffer = HeapAlloc(GetProcessHeap(), 0, cbData);
        memcpy(pwszExpandedBuffer, pwszTempBuffer, cbData);
    }

    /* If the value references a resource based string, parse the value and load the string.
     * Else just copy over the original value. */
    result = ERROR_SUCCESS;
    if (*pwszExpandedBuffer != L'@') /* '@' is the prefix for resource based string entries. */
    {
        lstrcpynW(pszOutBuf, pwszExpandedBuffer, cbOutBuf / sizeof(WCHAR));
    }
    else
    {
        WCHAR *pComma = wcsrchr(pwszExpandedBuffer, L',');
        UINT uiStringId;
        HMODULE hModule;

        /* Format of the expanded value is 'path_to_dll,-resId' */
        if (!pComma || pComma[1] != L'-')
        {
            result = ERROR_BADKEY;
            goto cleanup;
        }

        uiStringId = _wtoi(pComma+2);
        *pComma = L'\0';

        hModule = LoadLibraryExW(pwszExpandedBuffer + 1, NULL, LOAD_LIBRARY_AS_DATAFILE);
        if (!hModule || !load_string(hModule, uiStringId, pszOutBuf, cbOutBuf / sizeof(WCHAR)))
            result = ERROR_BADKEY;
        FreeLibrary(hModule);
    }

cleanup:
    HeapFree(GetProcessHeap(), 0, pwszTempBuffer);
    HeapFree(GetProcessHeap(), 0, pwszExpandedBuffer);
    return result;
}

/************************************************************************
 *  RegLoadMUIStringA
 *
 * @implemented
 */
LONG WINAPI
RegLoadMUIStringA(IN HKEY hKey,
                  IN LPCSTR pszValue  OPTIONAL,
                  OUT LPSTR pszOutBuf,
                  IN DWORD cbOutBuf,
                  OUT LPDWORD pcbData OPTIONAL,
                  IN DWORD Flags,
                  IN LPCSTR pszDirectory  OPTIONAL)
{
    UNICODE_STRING valueW, baseDirW;
    WCHAR *pwszBuffer;
    DWORD cbData = cbOutBuf * sizeof(WCHAR);
    LONG result;

    valueW.Buffer = baseDirW.Buffer = pwszBuffer = NULL;
    if (!RtlCreateUnicodeStringFromAsciiz(&valueW, pszValue) ||
        !RtlCreateUnicodeStringFromAsciiz(&baseDirW, pszDirectory) ||
        !(pwszBuffer = HeapAlloc(GetProcessHeap(), 0, cbData)))
    {
        result = ERROR_NOT_ENOUGH_MEMORY;
        goto cleanup;
    }

    result = RegLoadMUIStringW(hKey, valueW.Buffer, pwszBuffer, cbData, NULL, Flags,
                               baseDirW.Buffer);

    if (result == ERROR_SUCCESS)
    {
        cbData = WideCharToMultiByte(CP_ACP, 0, pwszBuffer, -1, pszOutBuf, cbOutBuf, NULL, NULL);
        if (pcbData)
            *pcbData = cbData;
    }

cleanup:
    HeapFree(GetProcessHeap(), 0, pwszBuffer);
    RtlFreeUnicodeString(&baseDirW);
    RtlFreeUnicodeString(&valueW);

    return result;
}


/******************************************************************************
 * AddMandatoryAce    (kernelbase.@)
 */
BOOL WINAPI AddMandatoryAce( PACL acl, DWORD rev, DWORD flags, DWORD policy, PSID sid )
{
	NTSTATUS Status;
	
	Status = RtlAddMandatoryAce( acl, rev, flags, policy,
                                             SYSTEM_MANDATORY_LABEL_ACE_TYPE, sid );
	if(NT_SUCCESS(Status)){
		return TRUE;
	}else{
		return FALSE;
	}
}

/******************************************************************************
 * RegLoadAppKeyA (kernelbase.@)
 *
 */
LSTATUS WINAPI RegLoadAppKeyA(const char *file, HKEY *result, REGSAM sam, DWORD options, DWORD reserved)
{
    FIXME("%s %p %u %u %u: stub\n", wine_dbgstr_a(file), result, sam, options, reserved);

    if (!file || reserved)
        return ERROR_INVALID_PARAMETER;

    *result = (HKEY)0xdeadbeef;
    return ERROR_SUCCESS;
}

/******************************************************************************
 * RegLoadAppKeyW (kernelbase.@)
 *
 */
LSTATUS WINAPI RegLoadAppKeyW(const WCHAR *file, HKEY *result, REGSAM sam, DWORD options, DWORD reserved)
{
    FIXME("%s %p %u %u %u: stub\n", wine_dbgstr_w(file), result, sam, options, reserved);

    if (!file || reserved)
        return ERROR_INVALID_PARAMETER;

    *result = (HKEY)0xdeadbeef;
    return ERROR_SUCCESS;
}

/***********************************************************************
 * DnsHostnameToComputerNameExW   (kernelbase.@)
 *
 * FIXME: how is this different from the non-Ex function?
 */
BOOL WINAPI DECLSPEC_HOTPATCH DnsHostnameToComputerNameExW( const WCHAR *hostname, WCHAR *computername,
                                                            DWORD *size )
{
    static const WCHAR allowed[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&')(-_{}";
    WCHAR buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD i, len;

    lstrcpynW( buffer, hostname, MAX_COMPUTERNAME_LENGTH + 1 );
    len = lstrlenW( buffer );
    if (*size < len + 1)
    {
        *size = len;
        SetLastError( ERROR_MORE_DATA );
        return FALSE;
    }
    *size = len;
    if (!computername) return FALSE;
    for (i = 0; i < len; i++)
    {
        if (buffer[i] >= 'a' && buffer[i] <= 'z') computername[i] = buffer[i] + 'A' - 'a';
        else computername[i] = wcschr( allowed, buffer[i] ) ? buffer[i] : '_';
    }
    computername[len] = 0;
    return TRUE;
}

/************************************************************************
 *  RegCloseKey
 *
 * @implemented
 */
LONG WINAPI
RegCloseKey(HKEY hKey)
{
    NTSTATUS Status;

    /* don't close null handle or a pseudo handle */
    if (!hKey)
    {
        return ERROR_INVALID_HANDLE;
    }

    if (((ULONG_PTR)hKey & 0xF0000000) == 0x80000000)
    {
        return ERROR_SUCCESS;
    }

    Status = NtClose(hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

  return ERROR_SUCCESS;
}

/************************************************************************
 *  RegCreateKeyExA
 *
 * @implemented
 */
LONG WINAPI
RegCreateKeyExA(
    _In_ HKEY hKey,
    _In_ LPCSTR lpSubKey,
    _In_ DWORD Reserved,
    _In_ LPSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_ LPDWORD lpdwDisposition)
{
    UNICODE_STRING SubKeyString;
    UNICODE_STRING ClassString;
    DWORD ErrorCode;

    RtlInitEmptyUnicodeString(&ClassString, NULL, 0);
    RtlInitEmptyUnicodeString(&SubKeyString, NULL, 0);

    if (lpClass)
    {
        if (!RtlCreateUnicodeStringFromAsciiz(&ClassString, lpClass))
        {
            ErrorCode = ERROR_NOT_ENOUGH_MEMORY;
            goto Exit;
        }
    }

    if (lpSubKey)
    {
        if (!RtlCreateUnicodeStringFromAsciiz(&SubKeyString, lpSubKey))
        {
            ErrorCode = ERROR_NOT_ENOUGH_MEMORY;
            goto Exit;
        }
    }

    ErrorCode = RegCreateKeyExW(
        hKey,
        SubKeyString.Buffer,
        Reserved,
        ClassString.Buffer,
        dwOptions,
        samDesired,
        lpSecurityAttributes,
        phkResult,
        lpdwDisposition);

Exit:
    RtlFreeUnicodeString(&SubKeyString);
    RtlFreeUnicodeString(&ClassString);

    return ErrorCode;
}

/************************************************************************
 *  RegCreateKeyExW
 *
 * @implemented
 */
LONG
WINAPI
RegCreateKeyExW(
    _In_ HKEY hKey,
    _In_ LPCWSTR lpSubKey,
    _In_ DWORD Reserved,
    _In_opt_ LPWSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_opt_ LPDWORD lpdwDisposition)
{
    UNICODE_STRING SubKeyString;
    UNICODE_STRING ClassString;
    OBJECT_ATTRIBUTES ObjectAttributes;
    HANDLE ParentKey;
    ULONG Attributes = OBJ_CASE_INSENSITIVE;
    NTSTATUS Status;

    TRACE("RegCreateKeyExW() called\n");

    if (lpSecurityAttributes && lpSecurityAttributes->nLength != sizeof(SECURITY_ATTRIBUTES))
        return ERROR_INVALID_USER_BUFFER;

    /* get the real parent key */
    Status = MapDefaultKey(&ParentKey,
                           hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    TRACE("ParentKey %p\n", ParentKey);

    if (IsHKCRKey(ParentKey))
    {
        LONG ErrorCode = CreateHKCRKey(
            ParentKey,
            lpSubKey,
            Reserved,
            lpClass,
            dwOptions,
            samDesired,
            lpSecurityAttributes,
            phkResult,
            lpdwDisposition);
        ClosePredefKey(ParentKey);
        return ErrorCode;
    }

    if (dwOptions & REG_OPTION_OPEN_LINK)
        Attributes |= OBJ_OPENLINK;

    RtlInitUnicodeString(&ClassString,
                         lpClass);
    RtlInitUnicodeString(&SubKeyString,
                         lpSubKey);
    InitializeObjectAttributes(&ObjectAttributes,
                               &SubKeyString,
                               Attributes,
                               (HANDLE)ParentKey,
                               lpSecurityAttributes ? (PSECURITY_DESCRIPTOR)lpSecurityAttributes->lpSecurityDescriptor : NULL);
    Status = CreateNestedKey(phkResult,
                             &ObjectAttributes,
                             (lpClass == NULL)? NULL : &ClassString,
                             dwOptions,
                             samDesired,
                             lpdwDisposition);

    ClosePredefKey(ParentKey);

    TRACE("Status %x\n", Status);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    return ERROR_SUCCESS;
}

static
LONG
GetKeyName(HKEY hKey, PUNICODE_STRING KeyName)
{
    UNICODE_STRING InfoName;
    PKEY_NAME_INFORMATION NameInformation;
    ULONG InfoLength;
    NTSTATUS Status;

    /* Get info length */
    InfoLength = 0;
    Status = NtQueryKey(hKey, KeyNameInformation, NULL, 0, &InfoLength);
    if (Status != STATUS_BUFFER_TOO_SMALL)
    {
        ERR("NtQueryKey returned unexpected Status: 0x%08x\n", Status);
        return RtlNtStatusToDosError(Status);
    }

    /* Get it for real */
    NameInformation = RtlAllocateHeap(RtlGetProcessHeap(), 0, InfoLength);
    if (NameInformation == NULL)
    {
        ERR("Failed to allocate %lu bytes", InfoLength);
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    Status = NtQueryKey(hKey, KeyNameInformation, NameInformation, InfoLength, &InfoLength);
    if (!NT_SUCCESS(Status))
    {
        RtlFreeHeap(RtlGetProcessHeap(), 0, NameInformation);
        ERR("NtQueryKey failed: 0x%08x\n", Status);
        return RtlNtStatusToDosError(Status);
    }

    /* Make it a proper UNICODE_STRING */
    InfoName.Length = NameInformation->NameLength;
    InfoName.MaximumLength = NameInformation->NameLength;
    InfoName.Buffer = NameInformation->Name;

    Status = RtlDuplicateUnicodeString(RTL_DUPLICATE_UNICODE_STRING_NULL_TERMINATE, &InfoName, KeyName);
    if (!NT_SUCCESS(Status))
    {
        RtlFreeHeap(RtlGetProcessHeap(), 0, NameInformation);
        ERR("RtlDuplicateUnicodeString failed: 0x%08x\n", Status);
        return RtlNtStatusToDosError(Status);
    }

    RtlFreeHeap(RtlGetProcessHeap(), 0, NameInformation);

    return ERROR_SUCCESS;
}

static
LONG
GetKeySam(
    _In_ HKEY hKey,
    _Out_ REGSAM* RegSam)
{
    NTSTATUS Status;
    OBJECT_BASIC_INFORMATION ObjectInfo;

    Status = NtQueryObject(hKey, ObjectBasicInformation, &ObjectInfo, sizeof(ObjectInfo), NULL);
    if (!NT_SUCCESS(Status))
    {
        ERR("NtQueryObject failed, Status %x08x\n", Status);
        return RtlNtStatusToDosError(Status);
    }

    *RegSam = ObjectInfo.GrantedAccess;
    return ERROR_SUCCESS;
}

/*
 * Gets a HKLM key from an HKCU key.
 */
static
LONG
GetFallbackHKCRKey(
    _In_ HKEY hKey,
    _Out_ HKEY* MachineKey,
    _In_ BOOL MustCreate)
{
    UNICODE_STRING KeyName;
    LPWSTR SubKeyName;
    LONG ErrorCode;
    REGSAM SamDesired;

    /* Get the key name */
    ErrorCode = GetKeyName(hKey, &KeyName);
    if (ErrorCode != ERROR_SUCCESS)
        return ErrorCode;

    /* See if we really need a conversion */
    if (RtlPrefixUnicodeString(&HKLM_ClassesPath, &KeyName, TRUE))
    {
        RtlFreeUnicodeString(&KeyName);
        *MachineKey = hKey;
        return ERROR_SUCCESS;
    }

    SubKeyName = KeyName.Buffer + 15; /* 15 == wcslen(L"\\Registry\\User\\") */
    /* Skip the user token */
    while (*SubKeyName++ != L'\\')
    {
        if (!*SubKeyName)
        {
            ERR("Key name %S is invalid!\n", KeyName.Buffer);
            return ERROR_INTERNAL_ERROR;
        }
    }

    /* Use the same access mask than the original key */
    ErrorCode = GetKeySam(hKey, &SamDesired);
    if (ErrorCode != ERROR_SUCCESS)
    {
        RtlFreeUnicodeString(&KeyName);
        return ErrorCode;
    }

    if (MustCreate)
    {
        ErrorCode = RegCreateKeyExW(
            HKEY_LOCAL_MACHINE,
            SubKeyName,
            0,
            NULL,
            0,
            SamDesired,
            NULL,
            MachineKey,
            NULL);
    }
    else
    {
        /* Open the key. */
        ErrorCode = RegOpenKeyExW(
            HKEY_LOCAL_MACHINE,
            SubKeyName,
            0,
            SamDesired,
            MachineKey);
    }

    RtlFreeUnicodeString(&KeyName);

    return ErrorCode;
}

/* Get the HKCU key (if it exists) from an HKCR key */
static
LONG
GetPreferredHKCRKey(
    _In_ HKEY hKey,
    _Out_ HKEY* PreferredKey)
{
    UNICODE_STRING KeyName;
    LPWSTR SubKeyName;
    LONG ErrorCode;
    REGSAM SamDesired;

    /* Get the key name */
    ErrorCode = GetKeyName(hKey, &KeyName);
    if (ErrorCode != ERROR_SUCCESS)
        return ErrorCode;

    /* See if we really need a conversion */
    if (!RtlPrefixUnicodeString(&HKLM_ClassesPath, &KeyName, TRUE))
    {
        RtlFreeUnicodeString(&KeyName);
        *PreferredKey = hKey;
        return ERROR_SUCCESS;
    }

    /* 18 == wcslen(L"\\Registry\\Machine\\") */
    SubKeyName = KeyName.Buffer + 18;

    /* Use the same access mask than the original key */
    ErrorCode = GetKeySam(hKey, &SamDesired);
    if (ErrorCode != ERROR_SUCCESS)
    {
        RtlFreeUnicodeString(&KeyName);
        return ErrorCode;
    }

    /* Open the key. */
    ErrorCode = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        SubKeyName,
        0,
        SamDesired,
        PreferredKey);

    RtlFreeUnicodeString(&KeyName);

    return ErrorCode;
}

/* HKCR version of RegCreateKeyExW. */
LONG
WINAPI
CreateHKCRKey(
    _In_ HKEY hKey,
    _In_ LPCWSTR lpSubKey,
    _In_ DWORD Reserved,
    _In_opt_ LPWSTR lpClass,
    _In_ DWORD dwOptions,
    _In_ REGSAM samDesired,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _Out_ PHKEY phkResult,
    _Out_opt_ LPDWORD lpdwDisposition)
{
    LONG ErrorCode;
    HKEY QueriedKey, TestKey;

    ASSERT(IsHKCRKey(hKey));

    /* Remove the HKCR flag while we're working */
    hKey = (HKEY)(((ULONG_PTR)hKey) & ~0x2);

    ErrorCode = GetPreferredHKCRKey(hKey, &QueriedKey);

    if (ErrorCode == ERROR_FILE_NOT_FOUND)
    {
        /* The current key doesn't exist on HKCU side, so we can only create it in HKLM */
        ErrorCode = RegCreateKeyExW(
            hKey,
            lpSubKey,
            Reserved,
            lpClass,
            dwOptions,
            samDesired,
            lpSecurityAttributes,
            phkResult,
            lpdwDisposition);
        if (ErrorCode == ERROR_SUCCESS)
            MakeHKCRKey(phkResult);
        return ErrorCode;
    }

    if (ErrorCode != ERROR_SUCCESS)
    {
        /* Somehow we failed for another reason (maybe deleted key or whatever) */
        return ErrorCode;
    }

    /* See if the subkey already exists in HKCU. */
    ErrorCode = RegOpenKeyExW(QueriedKey, lpSubKey, 0, READ_CONTROL, &TestKey);
    if (ErrorCode != ERROR_FILE_NOT_FOUND)
    {
        if (ErrorCode == ERROR_SUCCESS)
        {
            /* Great. Close the test one and do the real create operation */
            RegCloseKey(TestKey);
            ErrorCode = RegCreateKeyExW(
                QueriedKey,
                lpSubKey,
                Reserved,
                lpClass,
                dwOptions,
                samDesired,
                lpSecurityAttributes,
                phkResult,
                lpdwDisposition);
            if (ErrorCode == ERROR_SUCCESS)
                MakeHKCRKey(phkResult);
        }
        if (QueriedKey != hKey)
            RegCloseKey(QueriedKey);

        return ERROR_SUCCESS;
    }

    if (QueriedKey != hKey)
        RegCloseKey(QueriedKey);

    /* So we must do the create operation in HKLM, creating the missing parent keys if needed. */
    ErrorCode = GetFallbackHKCRKey(hKey, &QueriedKey, TRUE);
    if (ErrorCode != ERROR_SUCCESS)
        return ErrorCode;

    /* Do the key creation */
    ErrorCode = RegCreateKeyExW(
        QueriedKey,
        lpSubKey,
        Reserved,
        lpClass,
        dwOptions,
        samDesired,
        lpSecurityAttributes,
        phkResult,
        lpdwDisposition);

    if (QueriedKey != hKey)
        RegCloseKey(QueriedKey);

    if (ErrorCode == ERROR_SUCCESS)
        MakeHKCRKey(phkResult);

    return ErrorCode;
}

/************************************************************************
 *  RegInit
 */
BOOL
RegCleanup(VOID)
{
    TRACE("RegCleanup()\n");

    CloseDefaultKeys();
    RtlDeleteCriticalSection(&HandleTableCS);

    return TRUE;
}

/************************************************************************
 *  RegDeleteKeyExW
 *
 * @implemented
 */
LONG
WINAPI
RegDeleteKeyExW(
    _In_ HKEY hKey,
    _In_ LPCWSTR lpSubKey,
    _In_ REGSAM samDesired,
    _In_ DWORD Reserved)
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING SubKeyName;
    HANDLE ParentKey;
    HANDLE TargetKey;
    NTSTATUS Status;

    /* Make sure we got a subkey */
    if (!lpSubKey)
    {
        /* Fail */
        return ERROR_INVALID_PARAMETER;
    }

    Status = MapDefaultKey(&ParentKey,
                           hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    if (IsHKCRKey(ParentKey))
    {
        LONG ErrorCode = DeleteHKCRKey(ParentKey, lpSubKey, samDesired, Reserved);
        ClosePredefKey(ParentKey);
        return ErrorCode;
    }

    if (samDesired & KEY_WOW64_32KEY)
        ERR("Wow64 not yet supported!\n");

    if (samDesired & KEY_WOW64_64KEY)
        ERR("Wow64 not yet supported!\n");


    RtlInitUnicodeString(&SubKeyName, lpSubKey);
    InitializeObjectAttributes(&ObjectAttributes,
                               &SubKeyName,
                               OBJ_CASE_INSENSITIVE,
                               ParentKey,
                               NULL);
    Status = NtOpenKey(&TargetKey,
                       DELETE,
                       &ObjectAttributes);
    if (!NT_SUCCESS(Status))
    {
        goto Cleanup;
    }

    Status = NtDeleteKey(TargetKey);
    NtClose(TargetKey);

Cleanup:
    ClosePredefKey(ParentKey);

    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    return ERROR_SUCCESS;
}


/* HKCR version of RegDeleteKeyExW */
LONG
WINAPI
DeleteHKCRKey(
    _In_ HKEY hKey,
    _In_ LPCWSTR lpSubKey,
    _In_ REGSAM RegSam,
    _In_ DWORD Reserved)
{
    HKEY QueriedKey;
    LONG ErrorCode;

    ASSERT(IsHKCRKey(hKey));

    /* Remove the HKCR flag while we're working */
    hKey = (HKEY)(((ULONG_PTR)hKey) & ~0x2);

    ErrorCode = GetPreferredHKCRKey(hKey, &QueriedKey);

    if (ErrorCode == ERROR_FILE_NOT_FOUND)
    {
        /* The key doesn't exist on HKCU side, no chance for a subkey */
        return RegDeleteKeyExW(hKey, lpSubKey, RegSam, Reserved);
    }

    if (ErrorCode != ERROR_SUCCESS)
    {
        /* Somehow we failed for another reason (maybe deleted key or whatever) */
        return ErrorCode;
    }

    ErrorCode = RegDeleteKeyExW(QueriedKey, lpSubKey, RegSam, Reserved);

    /* Close it if we must */
    if (QueriedKey != hKey)
    {
        /* The original key is on the machine view */
        RegCloseKey(QueriedKey);
    }

    /* Anything else than ERROR_FILE_NOT_FOUND means that we found it, even if it is with failures. */
    if (ErrorCode != ERROR_FILE_NOT_FOUND)
        return ErrorCode;

    /* If we're here, we must open from HKLM key. */
    ErrorCode = GetFallbackHKCRKey(hKey, &QueriedKey, FALSE);
    if (ErrorCode != ERROR_SUCCESS)
    {
        /* Maybe the key doesn't exist in the HKLM view */
        return ErrorCode;
    }

    ErrorCode = RegDeleteKeyExW(QueriedKey, lpSubKey, RegSam, Reserved);

    /* Close it if we must */
    if (QueriedKey != hKey)
    {
        RegCloseKey(QueriedKey);
    }

    return ErrorCode;
}

/************************************************************************
 *  RegDeleteValueA
 *
 * @implemented
 */
LONG WINAPI
RegDeleteValueA(HKEY hKey,
                LPCSTR lpValueName)
{
    UNICODE_STRING ValueName;
    HANDLE KeyHandle;
    NTSTATUS Status;

    Status = MapDefaultKey(&KeyHandle,
                           hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    RtlCreateUnicodeStringFromAsciiz(&ValueName, lpValueName);
    Status = NtDeleteValueKey(KeyHandle,
                              &ValueName);
    RtlFreeUnicodeString (&ValueName);

    ClosePredefKey(KeyHandle);

    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    return ERROR_SUCCESS;
}


/************************************************************************
 *  RegDeleteValueW
 *
 * @implemented
 */
LONG WINAPI
RegDeleteValueW(HKEY hKey,
                LPCWSTR lpValueName)
{
    UNICODE_STRING ValueName;
    NTSTATUS Status;
    HANDLE KeyHandle;

    Status = MapDefaultKey(&KeyHandle,
                           hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    RtlInitUnicodeString(&ValueName, lpValueName);

    Status = NtDeleteValueKey(KeyHandle,
                              &ValueName);

    ClosePredefKey(KeyHandle);

    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    return ERROR_SUCCESS;
}

/************************************************************************
 *  RegEnumKeyExA
 *
 * @implemented
 */
LONG
WINAPI
RegEnumKeyExA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_ LPSTR lpName,
    _Inout_ LPDWORD lpcbName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPSTR lpClass,
    _Inout_opt_ LPDWORD lpcbClass,
    _Out_opt_ PFILETIME lpftLastWriteTime)
{
    WCHAR* NameBuffer = NULL;
    WCHAR* ClassBuffer = NULL;
    DWORD NameLength, ClassLength;
    LONG ErrorCode;

    /* Allocate our buffers */
    if (*lpcbName > 0)
    {
        NameLength = *lpcbName;
        NameBuffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, *lpcbName * sizeof(WCHAR));
        if (NameBuffer == NULL)
        {
            ErrorCode = ERROR_NOT_ENOUGH_MEMORY;
            goto Exit;
        }
    }

    if (lpClass)
    {
        if (*lpcbClass > 0)
        {
            ClassLength = *lpcbClass;
            ClassBuffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, *lpcbClass * sizeof(WCHAR));
            if (ClassBuffer == NULL)
            {
                ErrorCode = ERROR_NOT_ENOUGH_MEMORY;
                goto Exit;
            }
        }
    }

    /* Do the actual call */
    ErrorCode = RegEnumKeyExW(
        hKey,
        dwIndex,
        NameBuffer,
        lpcbName,
        lpReserved,
        ClassBuffer,
        lpcbClass,
        lpftLastWriteTime);

    if (ErrorCode != ERROR_SUCCESS)
        goto Exit;

    /* Convert the strings */
    RtlUnicodeToMultiByteN(lpName, *lpcbName, 0, NameBuffer, *lpcbName * sizeof(WCHAR));
    /* NULL terminate if we can */
    if (NameLength > *lpcbName)
        lpName[*lpcbName] = '\0';

    if (lpClass)
    {
        RtlUnicodeToMultiByteN(lpClass, *lpcbClass, 0, NameBuffer, *lpcbClass * sizeof(WCHAR));
        if (ClassLength > *lpcbClass)
            lpClass[*lpcbClass] = '\0';
    }

Exit:
    if (NameBuffer)
        RtlFreeHeap(RtlGetProcessHeap(), 0, NameBuffer);
    if (ClassBuffer)
        RtlFreeHeap(RtlGetProcessHeap(), 0, ClassBuffer);

    return ErrorCode;
}

/************************************************************************
 *  RegEnumKeyExW
 *
 * @implemented
 */
LONG
WINAPI
RegEnumKeyExW(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_ LPWSTR lpName,
    _Inout_ LPDWORD lpcbName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPWSTR lpClass,
    _Inout_opt_ LPDWORD lpcbClass,
    _Out_opt_ PFILETIME lpftLastWriteTime)
{
    union
    {
        KEY_NODE_INFORMATION Node;
        KEY_BASIC_INFORMATION Basic;
    } *KeyInfo;

    ULONG BufferSize;
    ULONG ResultSize;
    ULONG NameLength;
    ULONG ClassLength = 0;
    HANDLE KeyHandle;
    LONG ErrorCode = ERROR_SUCCESS;
    NTSTATUS Status;

    Status = MapDefaultKey(&KeyHandle,
                           hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    if (IsHKCRKey(KeyHandle))
    {
        ErrorCode = EnumHKCRKey(
            KeyHandle,
            dwIndex,
            lpName,
            lpcbName,
            lpReserved,
            lpClass,
            lpcbClass,
            lpftLastWriteTime);
        ClosePredefKey(KeyHandle);
        return ErrorCode;
    }

    if (*lpcbName > 0)
    {
        NameLength = min (*lpcbName - 1, REG_MAX_NAME_SIZE) * sizeof (WCHAR);
    }
    else
    {
        NameLength = 0;
    }

    if (lpClass)
    {
        if (*lpcbClass > 0)
        {
            ClassLength = min (*lpcbClass - 1, REG_MAX_NAME_SIZE) * sizeof(WCHAR);
        }
        else
        {
            ClassLength = 0;
        }

        BufferSize = ((sizeof(KEY_NODE_INFORMATION) + NameLength + 3) & ~3) + ClassLength;
    }
    else
    {
        BufferSize = sizeof(KEY_BASIC_INFORMATION) + NameLength;
    }

    KeyInfo = RtlAllocateHeap(ProcessHeap,
                              0,
                              BufferSize);
    if (KeyInfo == NULL)
    {
        ErrorCode = ERROR_OUTOFMEMORY;
        goto Cleanup;
    }

    Status = NtEnumerateKey(KeyHandle,
                            (ULONG)dwIndex,
                            lpClass ? KeyNodeInformation : KeyBasicInformation,
                            KeyInfo,
                            BufferSize,
                            &ResultSize);
    TRACE("NtEnumerateKey() returned status 0x%X\n", Status);
    if (!NT_SUCCESS(Status))
    {
        ErrorCode = RtlNtStatusToDosError (Status);
    }
    else
    {
        if (lpClass == NULL)
        {
            if (KeyInfo->Basic.NameLength > NameLength)
            {
                ErrorCode = ERROR_MORE_DATA;
            }
            else
            {
                RtlCopyMemory(lpName,
                              KeyInfo->Basic.Name,
                              KeyInfo->Basic.NameLength);
                *lpcbName = (DWORD)(KeyInfo->Basic.NameLength / sizeof(WCHAR));
                lpName[*lpcbName] = 0;
            }
        }
        else
        {
            if (KeyInfo->Node.NameLength > NameLength ||
                KeyInfo->Node.ClassLength > ClassLength)
            {
                ErrorCode = ERROR_MORE_DATA;
            }
            else
            {
                RtlCopyMemory(lpName,
                              KeyInfo->Node.Name,
                              KeyInfo->Node.NameLength);
                *lpcbName = KeyInfo->Node.NameLength / sizeof(WCHAR);
                lpName[*lpcbName] = 0;
                RtlCopyMemory(lpClass,
                              (PVOID)((ULONG_PTR)KeyInfo->Node.Name + KeyInfo->Node.ClassOffset),
                              KeyInfo->Node.ClassLength);
                *lpcbClass = (DWORD)(KeyInfo->Node.ClassLength / sizeof(WCHAR));
                lpClass[*lpcbClass] = 0;
            }
        }

        if (ErrorCode == ERROR_SUCCESS && lpftLastWriteTime != NULL)
        {
            if (lpClass == NULL)
            {
                lpftLastWriteTime->dwLowDateTime = KeyInfo->Basic.LastWriteTime.u.LowPart;
                lpftLastWriteTime->dwHighDateTime = KeyInfo->Basic.LastWriteTime.u.HighPart;
            }
            else
            {
                lpftLastWriteTime->dwLowDateTime = KeyInfo->Node.LastWriteTime.u.LowPart;
                lpftLastWriteTime->dwHighDateTime = KeyInfo->Node.LastWriteTime.u.HighPart;
            }
        }
    }

    RtlFreeHeap(ProcessHeap,
                0,
                KeyInfo);

Cleanup:
    ClosePredefKey(KeyHandle);

    return ErrorCode;
}

/* HKCR version of RegEnumKeyExW */
LONG
WINAPI
EnumHKCRKey(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_ LPWSTR lpName,
    _Inout_ LPDWORD lpcbName,
    _Reserved_ LPDWORD lpReserved,
    _Out_opt_ LPWSTR lpClass,
    _Inout_opt_ LPDWORD lpcbClass,
    _Out_opt_ PFILETIME lpftLastWriteTime)
{
    HKEY PreferredKey, FallbackKey;
    DWORD NumPreferredSubKeys;
    DWORD MaxFallbackSubKeyLen;
    DWORD FallbackIndex;
    WCHAR* FallbackSubKeyName = NULL;
    LONG ErrorCode;

    ASSERT(IsHKCRKey(hKey));

    /* Remove the HKCR flag while we're working */
    hKey = (HKEY)(((ULONG_PTR)hKey) & ~0x2);

    /* Get the preferred key */
    ErrorCode = GetPreferredHKCRKey(hKey, &PreferredKey);
    if (ErrorCode != ERROR_SUCCESS)
    {
        if (ErrorCode == ERROR_FILE_NOT_FOUND)
        {
            /* Only the HKLM key exists */
            return RegEnumKeyExW(
                hKey,
                dwIndex,
                lpName,
                lpcbName,
                lpReserved,
                lpClass,
                lpcbClass,
                lpftLastWriteTime);
        }
        return ErrorCode;
    }

    /* Get the fallback key */
    ErrorCode = GetFallbackHKCRKey(hKey, &FallbackKey, FALSE);
    if (ErrorCode != ERROR_SUCCESS)
    {
        if (PreferredKey != hKey)
            RegCloseKey(PreferredKey);
        if (ErrorCode == ERROR_FILE_NOT_FOUND)
        {
            /* Only the HKCU key exists */
            return RegEnumKeyExW(
                hKey,
                dwIndex,
                lpName,
                lpcbName,
                lpReserved,
                lpClass,
                lpcbClass,
                lpftLastWriteTime);
        }
        return ErrorCode;
    }

    /* Get some info on the HKCU side */
    ErrorCode = RegQueryInfoKeyW(
        PreferredKey,
        NULL,
        NULL,
        NULL,
        &NumPreferredSubKeys,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);
    if (ErrorCode != ERROR_SUCCESS)
        goto Exit;

    if (dwIndex < NumPreferredSubKeys)
    {
        /* HKCU side takes precedence */
        ErrorCode = RegEnumKeyExW(
            PreferredKey,
            dwIndex,
            lpName,
            lpcbName,
            lpReserved,
            lpClass,
            lpcbClass,
            lpftLastWriteTime);
        goto Exit;
    }

    /* Here it gets tricky. We must enumerate the values from the HKLM side,
     * without reporting those which are present on the HKCU side */

    /* Squash out the indices from HKCU */
    dwIndex -= NumPreferredSubKeys;

    /* Get some info */
    ErrorCode = RegQueryInfoKeyW(
        FallbackKey,
        NULL,
        NULL,
        NULL,
        NULL,
        &MaxFallbackSubKeyLen,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);
    if (ErrorCode != ERROR_SUCCESS)
    {
        ERR("Could not query info of key %p (Err: %d)\n", FallbackKey, ErrorCode);
        goto Exit;
    }

    MaxFallbackSubKeyLen++;
    TRACE("Maxfallbacksubkeylen: %d\n", MaxFallbackSubKeyLen);

    /* Allocate our buffer */
    FallbackSubKeyName = RtlAllocateHeap(
        RtlGetProcessHeap(), 0, MaxFallbackSubKeyLen * sizeof(WCHAR));
    if (!FallbackSubKeyName)
    {
        ErrorCode = ERROR_NOT_ENOUGH_MEMORY;
        goto Exit;
    }

    /* We must begin at the very first subkey of the fallback key,
     * and then see if we meet keys that already are in the preferred key.
     * In that case, we must bump dwIndex, as otherwise we would enumerate a key we already
     * saw in a previous call.
     */
    FallbackIndex = 0;
    while (TRUE)
    {
        HKEY PreferredSubKey;
        DWORD FallbackSubkeyLen = MaxFallbackSubKeyLen;

        /* Try enumerating */
        ErrorCode = RegEnumKeyExW(
            FallbackKey,
            FallbackIndex,
            FallbackSubKeyName,
            &FallbackSubkeyLen,
            NULL,
            NULL,
            NULL,
            NULL);
        if (ErrorCode != ERROR_SUCCESS)
        {
            if (ErrorCode != ERROR_NO_MORE_ITEMS)
                ERR("Returning %d.\n", ErrorCode);
            goto Exit;
        }
        FallbackSubKeyName[FallbackSubkeyLen] = L'\0';

        /* See if there is such a value on HKCU side */
        ErrorCode = RegOpenKeyExW(
            PreferredKey,
            FallbackSubKeyName,
            0,
            READ_CONTROL,
            &PreferredSubKey);

        if (ErrorCode == ERROR_SUCCESS)
        {
            RegCloseKey(PreferredSubKey);
            /* So we already enumerated it on HKCU side. */
            dwIndex++;
        }
        else if (ErrorCode != ERROR_FILE_NOT_FOUND)
        {
            ERR("Got error %d while querying for %s on HKCU side.\n", ErrorCode, FallbackSubKeyName);
            goto Exit;
        }

        /* See if we caught up */
        if (FallbackIndex == dwIndex)
            break;

        FallbackIndex++;
    }

    /* We can finally enumerate on the fallback side */
    ErrorCode = RegEnumKeyExW(
        FallbackKey,
        dwIndex,
        lpName,
        lpcbName,
        lpReserved,
        lpClass,
        lpcbClass,
        lpftLastWriteTime);

Exit:
    if (PreferredKey != hKey)
        RegCloseKey(PreferredKey);
    if (FallbackKey != hKey)
        RegCloseKey(FallbackKey);
    if (FallbackSubKeyName)
        RtlFreeHeap(RtlGetProcessHeap(), 0, FallbackSubKeyName);

    return ErrorCode;
}

/************************************************************************
 *  RegEnumValueA
 *
 * @implemented
 */
LONG WINAPI
RegEnumValueA(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_ LPSTR lpName,
    _Inout_ LPDWORD lpcbName,
    _Reserved_ LPDWORD lpdwReserved,
    _Out_opt_ LPDWORD lpdwType,
    _Out_opt_ LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData)
{
    WCHAR* NameBuffer;
    DWORD NameBufferSize, NameLength;
    LONG ErrorCode;
    DWORD LocalType = REG_NONE;
    BOOL NameOverflow = FALSE;

    /* Do parameter checks now, once and for all. */
    if (!lpName || !lpcbName)
        return ERROR_INVALID_PARAMETER;

    if ((lpData && !lpcbData) || lpdwReserved)
        return ERROR_INVALID_PARAMETER;

    /* Get the size of the buffer we must use for the first call to RegEnumValueW */
    ErrorCode = RegQueryInfoKeyW(
        hKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &NameBufferSize, NULL, NULL, NULL);
    if (ErrorCode != ERROR_SUCCESS)
        return ErrorCode;

    /* Add space for the null terminator */
    NameBufferSize++;

    /* Allocate the buffer for the unicode name */
    NameBuffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, NameBufferSize * sizeof(WCHAR));
    if (NameBuffer == NULL)
    {
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    /*
     * This code calls RegEnumValueW twice, because we need to know the type of the enumerated value.
     * So for the first call, we check if we overflow on the name, as we have no way of knowing if this
     * is an overflow on the data or on the name during the the second call. So the first time, we make the
     * call with the supplied value. This is merdique, but this is how it is.
     */
    NameLength = *lpcbName;
    ErrorCode = RegEnumValueW(
        hKey,
        dwIndex,
        NameBuffer,
        &NameLength,
        NULL,
        &LocalType,
        NULL,
        NULL);
    if (ErrorCode != ERROR_SUCCESS)
    {
        if (ErrorCode == ERROR_MORE_DATA)
            NameOverflow = TRUE;
        else
            goto Exit;
    }

    if (is_string(LocalType) && lpcbData)
    {
        /* We must allocate a buffer to get the unicode data */
        DWORD DataBufferSize = *lpcbData * sizeof(WCHAR);
        WCHAR* DataBuffer = NULL;
        DWORD DataLength = *lpcbData;
        LPSTR DataStr = (LPSTR)lpData;

        if (lpData)
            DataBuffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, *lpcbData * sizeof(WCHAR));

        /* Do the real call */
        ErrorCode = RegEnumValueW(
            hKey,
            dwIndex,
            NameBuffer,
            &NameBufferSize,
            lpdwReserved,
            lpdwType,
            (LPBYTE)DataBuffer,
            &DataBufferSize);

        *lpcbData = DataBufferSize / sizeof(WCHAR);

        if (ErrorCode != ERROR_SUCCESS)
        {
            RtlFreeHeap(RtlGetProcessHeap(), 0, DataBuffer);
            goto Exit;
        }

        /* Copy the data whatever the error code is */
        if (lpData)
        {
            /* Do the data conversion */
            RtlUnicodeToMultiByteN(DataStr, DataLength, 0, DataBuffer, DataBufferSize);
            /* NULL-terminate if there is enough room */
            if ((DataLength > *lpcbData) && (DataStr[*lpcbData - 1] != '\0'))
                DataStr[*lpcbData] = '\0';
        }

        RtlFreeHeap(RtlGetProcessHeap(), 0, DataBuffer);
    }
    else
    {
        /* No data conversion needed. Do the call with provided buffers */
        ErrorCode = RegEnumValueW(
            hKey,
            dwIndex,
            NameBuffer,
            &NameBufferSize,
            lpdwReserved,
            lpdwType,
            lpData,
            lpcbData);

        if (ErrorCode != ERROR_SUCCESS)
        {
            goto Exit;
        }
    }

    if (NameOverflow)
    {
        ErrorCode = ERROR_MORE_DATA;
        goto Exit;
    }

    /* Convert the name string */
    RtlUnicodeToMultiByteN(lpName, *lpcbName, lpcbName, NameBuffer, NameBufferSize * sizeof(WCHAR));
    lpName[*lpcbName] = ANSI_NULL;

Exit:
    if (NameBuffer)
        RtlFreeHeap(RtlGetProcessHeap(), 0, NameBuffer);

    return ErrorCode;
}


/******************************************************************************
 * RegEnumValueW   [ADVAPI32.@]
 * @implemented
 *
 * PARAMS
 *  hkey       [I] Handle to key to query
 *  index      [I] Index of value to query
 *  value      [O] Value string
 *  val_count  [I/O] Size of value buffer (in wchars)
 *  reserved   [I] Reserved
 *  type       [O] Type code
 *  data       [O] Value data
 *  count      [I/O] Size of data buffer (in bytes)
 *
 * RETURNS
 *  Success: ERROR_SUCCESS
 *  Failure: nonzero error code from Winerror.h
 */
LONG
WINAPI
RegEnumValueW(
    _In_ HKEY hKey,
    _In_ DWORD index,
    _Out_ LPWSTR value,
    _Inout_ PDWORD val_count,
    _Reserved_ PDWORD reserved,
    _Out_opt_ PDWORD type,
    _Out_opt_ LPBYTE data,
    _Inout_opt_ PDWORD count)
{
    HANDLE KeyHandle;
    NTSTATUS status;
    ULONG total_size;
    char buffer[256], *buf_ptr = buffer;
    KEY_VALUE_FULL_INFORMATION *info = (KEY_VALUE_FULL_INFORMATION *)buffer;
    static const int info_size = FIELD_OFFSET( KEY_VALUE_FULL_INFORMATION, Name );

    TRACE("(%p,%ld,%p,%p,%p,%p,%p,%p)\n",
          hKey, index, value, val_count, reserved, type, data, count );

    if (!value || !val_count)
        return ERROR_INVALID_PARAMETER;

    if ((data && !count) || reserved)
        return ERROR_INVALID_PARAMETER;

    status = MapDefaultKey(&KeyHandle, hKey);
    if (!NT_SUCCESS(status))
    {
        return RtlNtStatusToDosError(status);
    }

    if (IsHKCRKey(KeyHandle))
    {
        LONG ErrorCode = EnumHKCRValue(
            KeyHandle,
            index,
            value,
            val_count,
            reserved,
            type,
            data,
            count);
        ClosePredefKey(KeyHandle);
        return ErrorCode;
    }

    total_size = info_size + (MAX_PATH + 1) * sizeof(WCHAR);
    if (data) total_size += *count;
    total_size = min( sizeof(buffer), total_size );

    status = NtEnumerateValueKey( KeyHandle, index, KeyValueFullInformation,
                                  buffer, total_size, &total_size );
    if (status && (status != STATUS_BUFFER_OVERFLOW) && (status != STATUS_BUFFER_TOO_SMALL)) goto done;

    if (value || data)
    {
        /* retry with a dynamically allocated buffer */
        while ((status == STATUS_BUFFER_OVERFLOW) || (status == STATUS_BUFFER_TOO_SMALL))
        {
            if (buf_ptr != buffer) HeapFree( GetProcessHeap(), 0, buf_ptr );
            if (!(buf_ptr = HeapAlloc( GetProcessHeap(), 0, total_size )))
            {
                status = ERROR_NOT_ENOUGH_MEMORY;
                goto done;
            }
            info = (KEY_VALUE_FULL_INFORMATION *)buf_ptr;
            status = NtEnumerateValueKey( KeyHandle, index, KeyValueFullInformation,
                                          buf_ptr, total_size, &total_size );
        }

        if (status) goto done;

        if (value)
        {
            if (info->NameLength/sizeof(WCHAR) >= *val_count)
            {
                status = STATUS_BUFFER_OVERFLOW;
                goto overflow;
            }
            memcpy( value, info->Name, info->NameLength );
            *val_count = info->NameLength / sizeof(WCHAR);
            value[*val_count] = 0;
        }

        if (data)
        {
            if (info->DataLength > *count)
            {
                status = STATUS_BUFFER_OVERFLOW;
                goto overflow;
            }
            memcpy( data, buf_ptr + info->DataOffset, info->DataLength );
            if (is_string(info->Type) && info->DataLength <= *count - sizeof(WCHAR))
            {
                /* if the type is REG_SZ and data is not 0-terminated
                 * and there is enough space in the buffer NT appends a \0 */
                WCHAR *ptr = (WCHAR *)(data + info->DataLength);
                if (ptr > (WCHAR *)data && ptr[-1]) *ptr = 0;
            }
        }
    }
    else status = STATUS_SUCCESS;

 overflow:
    if (type) *type = info->Type;
    if (count) *count = info->DataLength;

 done:
    if (buf_ptr != buffer) HeapFree( GetProcessHeap(), 0, buf_ptr );
    ClosePredefKey(KeyHandle);
    return RtlNtStatusToDosError(status);
}

/* HKCR version of RegEnumValueW */
LONG
WINAPI
EnumHKCRValue(
    _In_ HKEY hKey,
    _In_ DWORD dwIndex,
    _Out_ LPWSTR lpName,
    _Inout_ PDWORD lpcbName,
    _Reserved_ PDWORD lpReserved,
    _Out_opt_ PDWORD lpdwType,
    _Out_opt_ LPBYTE lpData,
    _Inout_opt_ PDWORD lpcbData)
{
    HKEY PreferredKey, FallbackKey;
    DWORD NumPreferredValues;
    DWORD MaxFallbackValueNameLen;
    DWORD FallbackIndex;
    WCHAR* FallbackValueName = NULL;
    LONG ErrorCode;

    ASSERT(IsHKCRKey(hKey));

    /* Remove the HKCR flag while we're working */
    hKey = (HKEY)(((ULONG_PTR)hKey) & ~0x2);

    /* Get the preferred key */
    ErrorCode = GetPreferredHKCRKey(hKey, &PreferredKey);
    if (ErrorCode != ERROR_SUCCESS)
    {
        if (ErrorCode == ERROR_FILE_NOT_FOUND)
        {
            /* Only the HKLM key exists */
            return RegEnumValueW(
                hKey,
                dwIndex,
                lpName,
                lpcbName,
                lpReserved,
                lpdwType,
                lpData,
                lpcbData);
        }
        return ErrorCode;
    }

    /* Get the fallback key */
    ErrorCode = GetFallbackHKCRKey(hKey, &FallbackKey, FALSE);
    if (ErrorCode != ERROR_SUCCESS)
    {
        if (PreferredKey != hKey)
            RegCloseKey(PreferredKey);
        if (ErrorCode == ERROR_FILE_NOT_FOUND)
        {
            /* Only the HKCU key exists */
            return RegEnumValueW(
                hKey,
                dwIndex,
                lpName,
                lpcbName,
                lpReserved,
                lpdwType,
                lpData,
                lpcbData);
        }
        return ErrorCode;
    }

    /* Get some info on the HKCU side */
    ErrorCode = RegQueryInfoKeyW(
        PreferredKey,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &NumPreferredValues,
        NULL,
        NULL,
        NULL,
        NULL);
    if (ErrorCode != ERROR_SUCCESS)
        goto Exit;

    if (dwIndex < NumPreferredValues)
    {
        /* HKCU side takes precedence */
        return RegEnumValueW(
            PreferredKey,
            dwIndex,
            lpName,
            lpcbName,
            lpReserved,
            lpdwType,
            lpData,
            lpcbData);
        goto Exit;
    }

    /* Here it gets tricky. We must enumerate the values from the HKLM side,
     * without reporting those which are present on the HKCU side */

    /* Squash out the indices from HKCU */
    dwIndex -= NumPreferredValues;

    /* Get some info */
    ErrorCode = RegQueryInfoKeyW(
        FallbackKey,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &MaxFallbackValueNameLen,
        NULL,
        NULL,
        NULL);
    if (ErrorCode != ERROR_SUCCESS)
    {
        ERR("Could not query info of key %p (Err: %d)\n", FallbackKey, ErrorCode);
        goto Exit;
    }

    MaxFallbackValueNameLen++;
    TRACE("Maxfallbacksubkeylen: %d\n", MaxFallbackValueNameLen);

    /* Allocate our buffer */
    FallbackValueName = RtlAllocateHeap(
        RtlGetProcessHeap(), 0, MaxFallbackValueNameLen * sizeof(WCHAR));
    if (!FallbackValueName)
    {
        ErrorCode = ERROR_NOT_ENOUGH_MEMORY;
        goto Exit;
    }

    /* We must begin at the very first subkey of the fallback key,
     * and then see if we meet keys that already are in the preferred key.
     * In that case, we must bump dwIndex, as otherwise we would enumerate a key we already
     * saw in a previous call.
     */
    FallbackIndex = 0;
    while (TRUE)
    {
        DWORD FallbackValueNameLen = MaxFallbackValueNameLen;

        /* Try enumerating */
        ErrorCode = RegEnumValueW(
            FallbackKey,
            FallbackIndex,
            FallbackValueName,
            &FallbackValueNameLen,
            NULL,
            NULL,
            NULL,
            NULL);
        if (ErrorCode != ERROR_SUCCESS)
        {
            if (ErrorCode != ERROR_NO_MORE_ITEMS)
                ERR("Returning %d.\n", ErrorCode);
            goto Exit;
        }
        FallbackValueName[FallbackValueNameLen] = L'\0';

        /* See if there is such a value on HKCU side */
        ErrorCode = RegQueryValueExW(
            PreferredKey,
            FallbackValueName,
            NULL,
            NULL,
            NULL,
            NULL);

        if (ErrorCode == ERROR_SUCCESS)
        {
            /* So we already enumerated it on HKCU side. */
            dwIndex++;
        }
        else if (ErrorCode != ERROR_FILE_NOT_FOUND)
        {
            ERR("Got error %d while querying for %s on HKCU side.\n", ErrorCode, FallbackValueName);
            goto Exit;
        }

        /* See if we caught up */
        if (FallbackIndex == dwIndex)
            break;

        FallbackIndex++;
    }

    /* We can finally enumerate on the fallback side */
    ErrorCode = RegEnumValueW(
        FallbackKey,
        dwIndex,
        lpName,
        lpcbName,
        lpReserved,
        lpdwType,
        lpData,
        lpcbData);

Exit:
    if (PreferredKey != hKey)
        RegCloseKey(PreferredKey);
    if (FallbackKey != hKey)
        RegCloseKey(FallbackKey);
    if (FallbackValueName)
        RtlFreeHeap(RtlGetProcessHeap(), 0, FallbackValueName);

    return ErrorCode;
}

/************************************************************************
 *  RegOpenKeyExA
 *
 * @implemented
 */
LONG WINAPI
RegOpenKeyExA(
    _In_ HKEY hKey,
    _In_ LPCSTR lpSubKey,
    _In_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _Out_ PHKEY phkResult)
{
    UNICODE_STRING SubKeyString;
    LONG ErrorCode;

    TRACE("RegOpenKeyExA hKey 0x%x lpSubKey %s ulOptions 0x%x samDesired 0x%x phkResult %p\n",
          hKey, lpSubKey, ulOptions, samDesired, phkResult);

    if (lpSubKey)
    {
        if (!RtlCreateUnicodeStringFromAsciiz(&SubKeyString, lpSubKey))
            return ERROR_NOT_ENOUGH_MEMORY;
    }
    else
        RtlInitEmptyUnicodeString(&SubKeyString, NULL, 0);

    ErrorCode = RegOpenKeyExW(hKey, SubKeyString.Buffer, ulOptions, samDesired, phkResult);

    RtlFreeUnicodeString(&SubKeyString);

    return ErrorCode;
}


/************************************************************************
 *  RegOpenKeyExW
 *
 * @implemented
 */
LONG WINAPI
RegOpenKeyExW(HKEY hKey,
              LPCWSTR lpSubKey,
              DWORD ulOptions,
              REGSAM samDesired,
              PHKEY phkResult)
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING SubKeyString;
    HANDLE KeyHandle;
    NTSTATUS Status;
    ULONG Attributes = OBJ_CASE_INSENSITIVE;
    LONG ErrorCode = ERROR_SUCCESS;

    TRACE("RegOpenKeyExW hKey 0x%x lpSubKey %S ulOptions 0x%x samDesired 0x%x phkResult %p\n",
          hKey, lpSubKey, ulOptions, samDesired, phkResult);
    if (!phkResult)
    {
        return ERROR_INVALID_PARAMETER;
    }

    Status = MapDefaultKey(&KeyHandle, hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    if (IsHKCRKey(KeyHandle))
    {
        ErrorCode = OpenHKCRKey(KeyHandle, lpSubKey, ulOptions, samDesired, phkResult);
        ClosePredefKey(KeyHandle);
        return ErrorCode;
    }

    if (ulOptions & REG_OPTION_OPEN_LINK)
        Attributes |= OBJ_OPENLINK;

    RtlInitUnicodeString(&SubKeyString, lpSubKey ? lpSubKey : L"");

    InitializeObjectAttributes(&ObjectAttributes,
                               &SubKeyString,
                               Attributes,
                               KeyHandle,
                               NULL);

    Status = NtOpenKey((PHANDLE)phkResult,
                       samDesired,
                       &ObjectAttributes);

    if (Status == STATUS_DATATYPE_MISALIGNMENT)
    {
        HANDLE hAligned;
        UNICODE_STRING AlignedString;

        Status = RtlDuplicateUnicodeString(RTL_DUPLICATE_UNICODE_STRING_NULL_TERMINATE,
                                           &SubKeyString,
                                           &AlignedString);
        if (NT_SUCCESS(Status))
        {
            /* Try again with aligned parameters */
            InitializeObjectAttributes(&ObjectAttributes,
                                       &AlignedString,
                                       Attributes,
                                       KeyHandle,
                                       NULL);

            Status = NtOpenKey(&hAligned,
                               samDesired,
                               &ObjectAttributes);

            RtlFreeUnicodeString(&AlignedString);

            if (NT_SUCCESS(Status))
                *phkResult = hAligned;
        }
        else
        {
            /* Restore the original error */
            Status = STATUS_DATATYPE_MISALIGNMENT;
        }
    }

    if (!NT_SUCCESS(Status))
    {
        ErrorCode = RtlNtStatusToDosError(Status);
    }


    ClosePredefKey(KeyHandle);

    return ErrorCode;
}

/* Same as RegOpenKeyExW, but for HKEY_CLASSES_ROOT subkeys */
LONG
WINAPI
OpenHKCRKey(
    _In_ HKEY hKey,
    _In_ LPCWSTR lpSubKey,
    _In_ DWORD ulOptions,
    _In_ REGSAM samDesired,
    _In_ PHKEY phkResult)
{
    HKEY QueriedKey;
    LONG ErrorCode;

    ASSERT(IsHKCRKey(hKey));

    /* Remove the HKCR flag while we're working */
    hKey = (HKEY)(((ULONG_PTR)hKey) & ~0x2);

    ErrorCode = GetPreferredHKCRKey(hKey, &QueriedKey);

    if (ErrorCode == ERROR_FILE_NOT_FOUND)
    {
        /* The key doesn't exist on HKCU side, no chance for a subkey */
        ErrorCode = RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
        if (ErrorCode == ERROR_SUCCESS)
            MakeHKCRKey(phkResult);
        return ErrorCode;
    }

    if (ErrorCode != ERROR_SUCCESS)
    {
        /* Somehow we failed for another reason (maybe deleted key or whatever) */
        return ErrorCode;
    }

    /* Try on the HKCU side */
    ErrorCode = RegOpenKeyExW(QueriedKey, lpSubKey, ulOptions, samDesired, phkResult);
    if (ErrorCode == ERROR_SUCCESS)
        MakeHKCRKey(phkResult);

    /* Close it if we must */
    if (QueriedKey != hKey)
    {
        RegCloseKey(QueriedKey);
    }

    /* Anything else than ERROR_FILE_NOT_FOUND means that we found it, even if it is with failures. */
    if (ErrorCode != ERROR_FILE_NOT_FOUND)
        return ErrorCode;

    /* If we're here, we must open from HKLM key. */
    ErrorCode = GetFallbackHKCRKey(hKey, &QueriedKey, FALSE);
    if (ErrorCode != ERROR_SUCCESS)
    {
        /* Maybe the key doesn't exist in the HKLM view */
        return ErrorCode;
    }

    ErrorCode = RegOpenKeyExW(QueriedKey, lpSubKey, ulOptions, samDesired, phkResult);
    if (ErrorCode == ERROR_SUCCESS)
        MakeHKCRKey(phkResult);

    /* Close it if we must */
    if (QueriedKey != hKey)
    {
        RegCloseKey(QueriedKey);
    }

    return ErrorCode;
}

/************************************************************************
 *  RegQueryInfoKeyA
 *
 * @implemented
 */
LONG WINAPI
RegQueryInfoKeyA(HKEY hKey,
                 LPSTR lpClass,
                 LPDWORD lpcClass,
                 LPDWORD lpReserved,
                 LPDWORD lpcSubKeys,
                 LPDWORD lpcMaxSubKeyLen,
                 LPDWORD lpcMaxClassLen,
                 LPDWORD lpcValues,
                 LPDWORD lpcMaxValueNameLen,
                 LPDWORD lpcMaxValueLen,
                 LPDWORD lpcbSecurityDescriptor,
                 PFILETIME lpftLastWriteTime)
{
    WCHAR ClassName[MAX_PATH];
    UNICODE_STRING UnicodeString;
    ANSI_STRING AnsiString;
    LONG ErrorCode;
    NTSTATUS Status;
    DWORD cClass = 0;

    if ((lpClass) && (!lpcClass))
    {
        return ERROR_INVALID_PARAMETER;
    }

    RtlInitUnicodeString(&UnicodeString,
                         NULL);
    if (lpClass != NULL)
    {
        RtlInitEmptyUnicodeString(&UnicodeString,
                                  ClassName,
                                  sizeof(ClassName));
        cClass = sizeof(ClassName) / sizeof(WCHAR);
    }

    ErrorCode = RegQueryInfoKeyW(hKey,
                                 UnicodeString.Buffer,
                                 &cClass,
                                 lpReserved,
                                 lpcSubKeys,
                                 lpcMaxSubKeyLen,
                                 lpcMaxClassLen,
                                 lpcValues,
                                 lpcMaxValueNameLen,
                                 lpcMaxValueLen,
                                 lpcbSecurityDescriptor,
                                 lpftLastWriteTime);
    if ((ErrorCode == ERROR_SUCCESS) && (lpClass != NULL))
    {
        if (*lpcClass == 0)
        {
            return ErrorCode;
        }

        RtlInitEmptyAnsiString(&AnsiString, lpClass, *lpcClass);
        UnicodeString.Length = cClass * sizeof(WCHAR);
        Status = RtlUnicodeStringToAnsiString(&AnsiString,
                                              &UnicodeString,
                                              FALSE);
        ErrorCode = RtlNtStatusToDosError(Status);
        cClass = AnsiString.Length;
        lpClass[cClass] = ANSI_NULL;
    }

    if (lpcClass != NULL)
    {
        *lpcClass = cClass;
    }

    return ErrorCode;
}


/************************************************************************
 *  RegQueryInfoKeyW
 *
 * @implemented
 */
LONG WINAPI
RegQueryInfoKeyW(HKEY hKey,
                 LPWSTR lpClass,
                 LPDWORD lpcClass,
                 LPDWORD lpReserved,
                 LPDWORD lpcSubKeys,
                 LPDWORD lpcMaxSubKeyLen,
                 LPDWORD lpcMaxClassLen,
                 LPDWORD lpcValues,
                 LPDWORD lpcMaxValueNameLen,
                 LPDWORD lpcMaxValueLen,
                 LPDWORD lpcbSecurityDescriptor,
                 PFILETIME lpftLastWriteTime)
{
    KEY_FULL_INFORMATION FullInfoBuffer;
    PKEY_FULL_INFORMATION FullInfo;
    ULONG FullInfoSize;
    ULONG ClassLength = 0;
    HANDLE KeyHandle;
    NTSTATUS Status;
    ULONG Length;
    LONG ErrorCode = ERROR_SUCCESS;

    if ((lpClass) && (!lpcClass))
    {
        return ERROR_INVALID_PARAMETER;
    }

    Status = MapDefaultKey(&KeyHandle,
                           hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    if (lpClass != NULL)
    {
        if (*lpcClass > 0)
        {
            ClassLength = min(*lpcClass - 1, REG_MAX_NAME_SIZE) * sizeof(WCHAR);
        }
        else
        {
            ClassLength = 0;
        }

        FullInfoSize = sizeof(KEY_FULL_INFORMATION) + ((ClassLength + 3) & ~3);
        FullInfo = RtlAllocateHeap(ProcessHeap,
                                   0,
                                   FullInfoSize);
        if (FullInfo == NULL)
        {
            ErrorCode = ERROR_OUTOFMEMORY;
            goto Cleanup;
        }
    }
    else
    {
        FullInfoSize = sizeof(KEY_FULL_INFORMATION);
        FullInfo = &FullInfoBuffer;
    }

    if (lpcbSecurityDescriptor != NULL)
        *lpcbSecurityDescriptor = 0;

    Status = NtQueryKey(KeyHandle,
                        KeyFullInformation,
                        FullInfo,
                        FullInfoSize,
                        &Length);
    TRACE("NtQueryKey() returned status 0x%X\n", Status);
    if (!NT_SUCCESS(Status) && Status != STATUS_BUFFER_OVERFLOW)
    {
        ErrorCode = RtlNtStatusToDosError(Status);
        goto Cleanup;
    }

    TRACE("SubKeys %d\n", FullInfo->SubKeys);
    if (lpcSubKeys != NULL)
    {
        *lpcSubKeys = FullInfo->SubKeys;
    }

    TRACE("MaxNameLen %lu\n", FullInfo->MaxNameLen);
    if (lpcMaxSubKeyLen != NULL)
    {
        *lpcMaxSubKeyLen = FullInfo->MaxNameLen / sizeof(WCHAR);
    }

    TRACE("MaxClassLen %lu\n", FullInfo->MaxClassLen);
    if (lpcMaxClassLen != NULL)
    {
        *lpcMaxClassLen = FullInfo->MaxClassLen / sizeof(WCHAR);
    }

    TRACE("Values %lu\n", FullInfo->Values);
    if (lpcValues != NULL)
    {
        *lpcValues = FullInfo->Values;
    }

    TRACE("MaxValueNameLen %lu\n", FullInfo->MaxValueNameLen);
    if (lpcMaxValueNameLen != NULL)
    {
        *lpcMaxValueNameLen = FullInfo->MaxValueNameLen / sizeof(WCHAR);
    }

    TRACE("MaxValueDataLen %lu\n", FullInfo->MaxValueDataLen);
    if (lpcMaxValueLen != NULL)
    {
        *lpcMaxValueLen = FullInfo->MaxValueDataLen;
    }

    if (lpcbSecurityDescriptor != NULL)
    {
        Status = NtQuerySecurityObject(KeyHandle,
                                       OWNER_SECURITY_INFORMATION |
                                       GROUP_SECURITY_INFORMATION |
                                       DACL_SECURITY_INFORMATION,
                                       NULL,
                                       0,
                                       lpcbSecurityDescriptor);
        TRACE("NtQuerySecurityObject() returned status 0x%X\n", Status);
    }

    if (lpftLastWriteTime != NULL)
    {
        lpftLastWriteTime->dwLowDateTime = FullInfo->LastWriteTime.u.LowPart;
        lpftLastWriteTime->dwHighDateTime = FullInfo->LastWriteTime.u.HighPart;
    }

    if (lpClass != NULL)
    {
        if (*lpcClass == 0)
        {
            goto Cleanup;
        }

        if (FullInfo->ClassLength > ClassLength)
        {
            ErrorCode = ERROR_INSUFFICIENT_BUFFER;
        }
        else
        {
            RtlCopyMemory(lpClass,
                          FullInfo->Class,
                          FullInfo->ClassLength);
            lpClass[FullInfo->ClassLength / sizeof(WCHAR)] = UNICODE_NULL;
        }
    }

    if (lpcClass != NULL)
    {
        *lpcClass = FullInfo->ClassLength / sizeof(WCHAR);
    }

Cleanup:
    if (lpClass != NULL)
    {
        RtlFreeHeap(ProcessHeap,
                    0,
                    FullInfo);
    }

    ClosePredefKey(KeyHandle);

    return ErrorCode;
}

/******************************************************************************
 * RegQueryValueExA   [ADVAPI32.@]
 *
 * Get the type and contents of a specified value under with a key.
 *
 * PARAMS
 *  hkey      [I]   Handle of the key to query
 *  name      [I]   Name of value under hkey to query
 *  reserved  [I]   Reserved - must be NULL
 *  type      [O]   Destination for the value type, or NULL if not required
 *  data      [O]   Destination for the values contents, or NULL if not required
 *  count     [I/O] Size of data, updated with the number of bytes returned
 *
 * RETURNS
 *  Success: ERROR_SUCCESS. *count is updated with the number of bytes copied to data.
 *  Failure: ERROR_INVALID_HANDLE, if hkey is invalid.
 *           ERROR_INVALID_PARAMETER, if any other parameter is invalid.
 *           ERROR_MORE_DATA, if on input *count is too small to hold the contents.
 *
 * NOTES
 *   MSDN states that if data is too small it is partially filled. In reality
 *   it remains untouched.
 */
LONG
WINAPI
RegQueryValueExA(
    _In_ HKEY hkeyorg,
    _In_ LPCSTR name,
    _In_ LPDWORD reserved,
    _Out_opt_ LPDWORD type,
    _Out_opt_ LPBYTE data,
    _Inout_opt_ LPDWORD count)
{
    UNICODE_STRING nameW;
    DWORD DataLength;
    DWORD ErrorCode;
    DWORD BufferSize = 0;
    WCHAR* Buffer;
    CHAR* DataStr = (CHAR*)data;
    DWORD LocalType;

    /* Validate those parameters, the rest will be done with the first RegQueryValueExW call */
    if ((data && !count) || reserved)
        return ERROR_INVALID_PARAMETER;

    if (name)
    {
        if (!RtlCreateUnicodeStringFromAsciiz(&nameW, name))
            return ERROR_NOT_ENOUGH_MEMORY;
    }
    else
        RtlInitEmptyUnicodeString(&nameW, NULL, 0);

    ErrorCode = RegQueryValueExW(hkeyorg, nameW.Buffer, NULL, &LocalType, NULL, &BufferSize);
    if (ErrorCode != ERROR_SUCCESS)
    {
        if ((!data) && count)
            *count = 0;
        RtlFreeUnicodeString(&nameW);
        return ErrorCode;
    }

    /* See if we can directly handle the call without caring for conversion */
    if (!is_string(LocalType) || !count)
    {
        ErrorCode = RegQueryValueExW(hkeyorg, nameW.Buffer, reserved, type, data, count);
        RtlFreeUnicodeString(&nameW);
        return ErrorCode;
    }

    /* Allocate a unicode string to get the data */
    Buffer = RtlAllocateHeap(RtlGetProcessHeap(), 0, BufferSize);
    if (!Buffer)
    {
        RtlFreeUnicodeString(&nameW);
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    ErrorCode = RegQueryValueExW(hkeyorg, nameW.Buffer, reserved, type, (LPBYTE)Buffer, &BufferSize);
    if (ErrorCode != ERROR_SUCCESS)
    {
        RtlFreeHeap(RtlGetProcessHeap(), 0, Buffer);
        RtlFreeUnicodeString(&nameW);
        return ErrorCode;
    }

    /* We don't need this anymore */
    RtlFreeUnicodeString(&nameW);

    DataLength = *count;
    RtlUnicodeToMultiByteSize(count, Buffer, BufferSize);

    if ((!data) || (DataLength < *count))
    {
        RtlFreeHeap(RtlGetProcessHeap(), 0, Buffer);
        return  data ? ERROR_MORE_DATA : ERROR_SUCCESS;
    }

    /* We can finally do the conversion */
    RtlUnicodeToMultiByteN(DataStr, DataLength, NULL, Buffer, BufferSize);

    /* NULL-terminate if there is enough room */
    if ((DataLength > *count) && (DataStr[*count - 1] != '\0'))
        DataStr[*count] = '\0';

    RtlFreeHeap(RtlGetProcessHeap(), 0, Buffer);

    return ERROR_SUCCESS;
}


/************************************************************************
 *  RegQueryValueExW
 *
 * @implemented
 */
LONG
WINAPI
RegQueryValueExW(
    _In_ HKEY hkeyorg,
    _In_ LPCWSTR name,
    _In_ LPDWORD reserved,
    _In_ LPDWORD type,
    _In_ LPBYTE data,
    _In_ LPDWORD count)
{
    HANDLE hkey;
    NTSTATUS status;
    UNICODE_STRING name_str;
    DWORD total_size;
    char buffer[256], *buf_ptr = buffer;
    KEY_VALUE_PARTIAL_INFORMATION *info = (KEY_VALUE_PARTIAL_INFORMATION *)buffer;
    static const int info_size = offsetof( KEY_VALUE_PARTIAL_INFORMATION, Data );

    TRACE("(%p,%s,%p,%p,%p,%p=%d)\n",
          hkeyorg, debugstr_w(name), reserved, type, data, count,
          (count && data) ? *count : 0 );

    if ((data && !count) || reserved) return ERROR_INVALID_PARAMETER;

    status = MapDefaultKey(&hkey, hkeyorg);
    if (!NT_SUCCESS(status))
    {
        return RtlNtStatusToDosError(status);
    }

    if (IsHKCRKey(hkey))
    {
        LONG ErrorCode = QueryHKCRValue(hkey, name, reserved, type, data, count);
        ClosePredefKey(hkey);
        return ErrorCode;
    }

    RtlInitUnicodeString( &name_str, name );

    if (data)
        total_size = min( sizeof(buffer), *count + info_size );
    else
        total_size = info_size;


    status = NtQueryValueKey( hkey, &name_str, KeyValuePartialInformation,
                              buffer, total_size, &total_size );

    if (!NT_SUCCESS(status) && status != STATUS_BUFFER_OVERFLOW)
    {
        // NT: Valid handles with inexistant/null values or invalid (but not NULL) handles sets type to REG_NONE
        // On windows these conditions are likely to be side effects of the implementation...
        if (status == STATUS_INVALID_HANDLE && hkey)
        {
            if (type) *type = REG_NONE;
            if (count) *count = 0;
        }
        else if (status == STATUS_OBJECT_NAME_NOT_FOUND)
        {
            if (type) *type = REG_NONE;
            if (data == NULL && count) *count = 0;
        }
        goto done;
    }

    if (data)
    {
        /* retry with a dynamically allocated buffer */
        while (status == STATUS_BUFFER_OVERFLOW && total_size - info_size <= *count)
        {
            if (buf_ptr != buffer) HeapFree( GetProcessHeap(), 0, buf_ptr );
            if (!(buf_ptr = HeapAlloc( GetProcessHeap(), 0, total_size )))
            {
                ClosePredefKey(hkey);
                return ERROR_NOT_ENOUGH_MEMORY;
            }
            info = (KEY_VALUE_PARTIAL_INFORMATION *)buf_ptr;
            status = NtQueryValueKey( hkey, &name_str, KeyValuePartialInformation,
                                      buf_ptr, total_size, &total_size );
        }

        if (NT_SUCCESS(status))
        {
            memcpy( data, buf_ptr + info_size, total_size - info_size );
            /* if the type is REG_SZ and data is not 0-terminated
             * and there is enough space in the buffer NT appends a \0 */
            if (is_string(info->Type) && total_size - info_size <= *count-sizeof(WCHAR))
            {
                WCHAR *ptr = (WCHAR *)(data + total_size - info_size);
                if (ptr > (WCHAR *)data && ptr[-1]) *ptr = 0;
            }
        }
        else if (status != STATUS_BUFFER_OVERFLOW) goto done;
    }
    else status = STATUS_SUCCESS;

    if (type) *type = info->Type;
    if (count) *count = total_size - info_size;

 done:
    if (buf_ptr != buffer) HeapFree( GetProcessHeap(), 0, buf_ptr );
    ClosePredefKey(hkey);
    return RtlNtStatusToDosError(status);
}

/* HKCR version of RegQueryValueExW */
LONG
WINAPI
QueryHKCRValue(
    _In_ HKEY hKey,
    _In_ LPCWSTR Name,
    _In_ LPDWORD Reserved,
    _In_ LPDWORD Type,
    _In_ LPBYTE Data,
    _In_ LPDWORD Count)
{
    HKEY QueriedKey;
    LONG ErrorCode;

    ASSERT(IsHKCRKey(hKey));

    /* Remove the HKCR flag while we're working */
    hKey = (HKEY)(((ULONG_PTR)hKey) & ~0x2);

    ErrorCode = GetPreferredHKCRKey(hKey, &QueriedKey);

    if (ErrorCode == ERROR_FILE_NOT_FOUND)
    {
        /* The key doesn't exist on HKCU side, no chance for a value in it */
        return RegQueryValueExW(hKey, Name, Reserved, Type, Data, Count);
    }

    if (ErrorCode != ERROR_SUCCESS)
    {
        /* Somehow we failed for another reason (maybe deleted key or whatever) */
        return ErrorCode;
    }

    ErrorCode = RegQueryValueExW(QueriedKey, Name, Reserved, Type, Data, Count);

    /* Close it if we must */
    if (QueriedKey != hKey)
    {
        /* The original key is on the machine view */
        RegCloseKey(QueriedKey);
    }

    /* Anything else than ERROR_FILE_NOT_FOUND means that we found it, even if it is with failures. */
    if (ErrorCode != ERROR_FILE_NOT_FOUND)
        return ErrorCode;

    /* If we're here, we must open from HKLM key. */
    ErrorCode = GetFallbackHKCRKey(hKey, &QueriedKey, FALSE);
    if (ErrorCode != ERROR_SUCCESS)
    {
        /* Maybe the key doesn't exist in the HKLM view */
        return ErrorCode;
    }

    ErrorCode = RegQueryValueExW(QueriedKey, Name, Reserved, Type, Data, Count);

    /* Close it if we must */
    if (QueriedKey != hKey)
    {
        RegCloseKey(QueriedKey);
    }

    return ErrorCode;
}

/************************************************************************
 *  RegSetValueExA
 *
 * @implemented
 */
LONG WINAPI
RegSetValueExA(HKEY hKey,
               LPCSTR lpValueName,
               DWORD Reserved,
               DWORD dwType,
               CONST BYTE* lpData,
               DWORD cbData)
{
    UNICODE_STRING ValueName;
    LPWSTR pValueName;
    ANSI_STRING AnsiString;
    UNICODE_STRING Data;
    LONG ErrorCode;
    LPBYTE pData;
    DWORD DataSize;
    NTSTATUS Status;

    /* Convert SubKey name to Unicode */
    if (lpValueName != NULL && lpValueName[0] != '\0')
    {
        if (!RtlCreateUnicodeStringFromAsciiz(&ValueName, lpValueName))
            return ERROR_NOT_ENOUGH_MEMORY;
    }
    else
    {
        ValueName.Buffer = NULL;
    }

    pValueName = (LPWSTR)ValueName.Buffer;


    if (is_string(dwType) && (cbData != 0))
    {
        /* Convert ANSI string Data to Unicode */
        /* If last character NOT zero then increment length */
        LONG bNoNulledStr = ((lpData[cbData-1] != '\0') ? 1 : 0);
        AnsiString.Buffer = (PSTR)lpData;
        AnsiString.Length = cbData + bNoNulledStr;
        AnsiString.MaximumLength = cbData + bNoNulledStr;
        Status = RtlAnsiStringToUnicodeString(&Data,
                                     &AnsiString,
                                     TRUE);

        if (!NT_SUCCESS(Status))
        {
            if (pValueName != NULL)
                RtlFreeUnicodeString(&ValueName);

            return RtlNtStatusToDosError(Status);
        }
        pData = (LPBYTE)Data.Buffer;
        DataSize = cbData * sizeof(WCHAR);
    }
    else
    {
        Data.Buffer = NULL;
        pData = (LPBYTE)lpData;
        DataSize = cbData;
    }

    ErrorCode = RegSetValueExW(hKey,
                               pValueName,
                               Reserved,
                               dwType,
                               pData,
                               DataSize);

    if (pValueName != NULL)
        RtlFreeUnicodeString(&ValueName);

    if (Data.Buffer != NULL)
        RtlFreeUnicodeString(&Data);

    return ErrorCode;
}


/************************************************************************
 *  RegSetValueExW
 *
 * @implemented
 */
LONG
WINAPI
RegSetValueExW(
    _In_ HKEY hKey,
    _In_ LPCWSTR lpValueName,
    _In_ DWORD Reserved,
    _In_ DWORD dwType,
    _In_ CONST BYTE* lpData,
    _In_ DWORD cbData)
{
    UNICODE_STRING ValueName;
    HANDLE KeyHandle;
    NTSTATUS Status;

    Status = MapDefaultKey(&KeyHandle,
                           hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    if (IsHKCRKey(KeyHandle))
    {
        LONG ErrorCode = SetHKCRValue(KeyHandle, lpValueName, Reserved, dwType, lpData, cbData);
        ClosePredefKey(KeyHandle);
        return ErrorCode;
    }

    if (is_string(dwType) && (cbData != 0))
    {
        PWSTR pwsData = (PWSTR)lpData;

        _SEH2_TRY
        {
            if((pwsData[cbData / sizeof(WCHAR) - 1] != L'\0') &&
                (pwsData[cbData / sizeof(WCHAR)] == L'\0'))
            {
                /* Increment length if last character is not zero and next is zero */
                cbData += sizeof(WCHAR);
            }
        }
        _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
        {
            /* Do not fail if we fault where we were told not to go */
        }
        _SEH2_END;
    }

    RtlInitUnicodeString(&ValueName, lpValueName);

    Status = NtSetValueKey(KeyHandle,
                           &ValueName,
                           0,
                           dwType,
                           (PVOID)lpData,
                           (ULONG)cbData);

    ClosePredefKey(KeyHandle);

    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    return ERROR_SUCCESS;
}

/* HKCR version of RegSetValueExW */
LONG
WINAPI
SetHKCRValue(
    _In_ HKEY hKey,
    _In_ LPCWSTR Name,
    _In_ DWORD Reserved,
    _In_ DWORD Type,
    _In_ CONST BYTE* Data,
    _In_ DWORD DataSize)
{
    HKEY QueriedKey;
    LONG ErrorCode;

    ASSERT(IsHKCRKey(hKey));

    /* Remove the HKCR flag while we're working */
    hKey = (HKEY)(((ULONG_PTR)hKey) & ~0x2);

    ErrorCode = GetPreferredHKCRKey(hKey, &QueriedKey);

    if (ErrorCode == ERROR_FILE_NOT_FOUND)
    {
        /* The key doesn't exist on HKCU side, no chance to put a value in it */
        return RegSetValueExW(hKey, Name, Reserved, Type, Data, DataSize);
    }

    if (ErrorCode != ERROR_SUCCESS)
    {
        /* Somehow we failed for another reason (maybe deleted key or whatever) */
        return ErrorCode;
    }

    /* Check if the value already exists in the preferred key */
    ErrorCode = RegQueryValueExW(QueriedKey, Name, NULL, NULL, NULL, NULL);
    if (ErrorCode != ERROR_FILE_NOT_FOUND)
    {
        if (ErrorCode == ERROR_SUCCESS)
        {
            /* Yes, so we have the right to modify it */
            ErrorCode = RegSetValueExW(QueriedKey, Name, Reserved, Type, Data, DataSize);
        }
        if (QueriedKey != hKey)
            RegCloseKey(QueriedKey);
        return ErrorCode;
    }
    if (QueriedKey != hKey)
        RegCloseKey(QueriedKey);

    /* So we must set the value in the HKLM version */
    ErrorCode = GetPreferredHKCRKey(hKey, &QueriedKey);
    if (ErrorCode == ERROR_FILE_NOT_FOUND)
    {
        /* No choice: put this in HKCU */
        return RegSetValueExW(hKey, Name, Reserved, Type, Data, DataSize);
    }
    else if (ErrorCode != ERROR_SUCCESS)
    {
        return ErrorCode;
    }

    ErrorCode = RegSetValueExW(QueriedKey, Name, Reserved, Type, Data, DataSize);

    if (QueriedKey != hKey)
        RegCloseKey(QueriedKey);

    return ErrorCode;
}