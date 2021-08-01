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

#define HKEY_SPECIAL_ROOT_FIRST   HKEY_CLASSES_ROOT
#define HKEY_SPECIAL_ROOT_LAST    HKEY_DYN_DATA

struct perf_provider
{
    HMODULE perflib;
    WCHAR linkage[MAX_PATH];
    WCHAR objects[MAX_PATH];
    PM_OPEN_PROC *pOpen;
    PM_CLOSE_PROC *pClose;
    PM_COLLECT_PROC *pCollect;
};

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

static BOOL is_wow6432node( const UNICODE_STRING *name )
{
    return (name->Length == 11 * sizeof(WCHAR) && !_wcsnicmp( name->Buffer, L"Wow6432Node", 11 ));
}

/* check if value type needs string conversion (Ansi<->Unicode) */
static inline BOOL is_string( DWORD type )
{
    return (type == REG_SZ) || (type == REG_EXPAND_SZ) || (type == REG_MULTI_SZ);
}

typedef LONG (WINAPI *LPFN_REG_SET_VALUE_EX_W)(
    HKEY, 
    LPCWSTR,
	DWORD,
	DWORD,
	CONST BYTE*,
	DWORD);
	
typedef LONG (WINAPI *LPFN_REG_SET_VALUE_EX_A)(
    HKEY, 
    LPCSTR,
	DWORD,
	DWORD,
	CONST BYTE*,
	DWORD);	

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

/* check if current version is NT or Win95 */
static inline BOOL is_version_nt(void)
{
    return !(GetVersion() & 0x80000000);
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
 * RegSetValueExA   (kernelbase.@)
 *
 * See RegSetValueExW.
 *
 * NOTES
 *  win95 does not care about count for REG_SZ and finds out the len by itself (js)
 *  NT does definitely care (aj)
 */
LSTATUS WINAPI DECLSPEC_HOTPATCH RegSetValueInternalExA( HKEY hkey, LPCSTR name, DWORD reserved, DWORD type,
                                                 const BYTE *data, DWORD count )
{
    ANSI_STRING nameA;
    UNICODE_STRING nameW;
    WCHAR *dataW = NULL;
    NTSTATUS status;

    if (!is_version_nt())  /* win95 */
    {
        if (type == REG_SZ)
        {
            if (!data) return ERROR_INVALID_PARAMETER;
            count = strlen((const char *)data) + 1;
        }
    }
    else if (count && is_string(type))
    {
        /* if user forgot to count terminating null, add it (yes NT does this) */
        if (data[count-1] && !data[count]) count++;
    }

    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    if (is_string( type )) /* need to convert to Unicode */
    {
        DWORD lenW;
        RtlMultiByteToUnicodeSize( &lenW, (const char *)data, count );
        if (!(dataW = heap_alloc( lenW ))) return ERROR_OUTOFMEMORY;
        RtlMultiByteToUnicodeN( dataW, lenW, NULL, (const char *)data, count );
        count = lenW;
        data = (BYTE *)dataW;
    }

    RtlInitAnsiString( &nameA, name );
    if (!(status = RtlAnsiStringToUnicodeString( &nameW, &nameA, TRUE )))
    {
        status = NtSetValueKey( hkey, &nameW, 0, type, data, count );
        RtlFreeUnicodeString( &nameW );
    }
    heap_free( dataW );
    return RtlNtStatusToDosError( status );
}

/******************************************************************************
 * RegSetValueExW   (kernelbase.@)
 *
 * Set the data and contents of a registry value.
 *
 * PARAMS
 *  hkey       [I] Handle of key to set value for
 *  name       [I] Name of value to set
 *  reserved   [I] Reserved, must be zero
 *  type       [I] Type of the value being set
 *  data       [I] The new contents of the value to set
 *  count      [I] Size of data
 *
 * RETURNS
 *  Success: ERROR_SUCCESS
 *  Failure: Error code
 */
LSTATUS WINAPI DECLSPEC_HOTPATCH RegSetValueInternalExW( HKEY hkey, LPCWSTR name, DWORD reserved,
                                                 DWORD type, const BYTE *data, DWORD count )
{
    UNICODE_STRING nameW;

    /* no need for version check, not implemented on win9x anyway */

    if ((data && ((ULONG_PTR)data >> 16) == 0) || (!data && count)) return ERROR_NOACCESS;

    if (count && is_string(type))
    {
        LPCWSTR str = (LPCWSTR)data;
        /* if user forgot to count terminating null, add it (yes NT does this) */
        if (str[count / sizeof(WCHAR) - 1] && !str[count / sizeof(WCHAR)])
            count += sizeof(WCHAR);
    }
    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    RtlInitUnicodeString( &nameW, name );
    return RtlNtStatusToDosError( NtSetValueKey( hkey, &nameW, 0, type, data, count ) );
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
        ret = RegSetValueInternalExW( hdst, name_buf, 0, type, value_buf, value_size );
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

LONG
RegDisablePredefinedCacheEx(
)
{
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

	//Need use native Windows advapi32 RegSetValueExA. Reactos and wine's version may corrupt and cause error on .Net Framework 4.5.1+
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

    ret = RegSetValueInternalExW( hkey, name, 0, type, (const BYTE*)data, len );
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

/******************************************************************************
 * RegCloseKey   (kernelbase.@)
 *
 * Close an open registry key.
 *
 * PARAMS
 *  hkey [I] Handle of key to close
 *
 * RETURNS
 *  Success: ERROR_SUCCESS
 *  Failure: Error code
 */
LSTATUS WINAPI DECLSPEC_HOTPATCH RegCloseKey( HKEY hkey )
{
    if (!hkey) return ERROR_INVALID_HANDLE;
    if (hkey >= (HKEY)0x80000000) return ERROR_SUCCESS;
    return RtlNtStatusToDosError( NtClose( hkey ) );
}

/******************************************************************************
 * RegCreateKeyExW   (kernelbase.@)
 *
 * See RegCreateKeyExA.
 */
LSTATUS WINAPI DECLSPEC_HOTPATCH RegCreateKeyExW( HKEY hkey, LPCWSTR name, DWORD reserved, LPWSTR class,
                             DWORD options, REGSAM access, SECURITY_ATTRIBUTES *sa,
                             PHKEY retkey, LPDWORD dispos )
{
    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING nameW, classW;

    if (reserved) return ERROR_INVALID_PARAMETER;
    if (!(hkey = get_special_root_hkey( hkey, access ))) return ERROR_INVALID_HANDLE;

    attr.Length = sizeof(attr);
    attr.RootDirectory = hkey;
    attr.ObjectName = &nameW;
    attr.Attributes = 0;
    attr.SecurityDescriptor = NULL;
    attr.SecurityQualityOfService = NULL;
    if (options & REG_OPTION_OPEN_LINK) attr.Attributes |= OBJ_OPENLINK;
    RtlInitUnicodeString( &nameW, name );
    RtlInitUnicodeString( &classW, class );

    return RtlNtStatusToDosError( create_key( retkey, access, &attr, &classW, options, dispos ) );
}


/******************************************************************************
 * RegCreateKeyExA   (kernelbase.@)
 *
 * Open a registry key, creating it if it doesn't exist.
 *
 * PARAMS
 *  hkey       [I] Handle of the parent registry key
 *  name       [I] Name of the new key to open or create
 *  reserved   [I] Reserved, pass 0
 *  class      [I] The object type of the new key
 *  options    [I] Flags controlling the key creation (REG_OPTION_* flags from "winnt.h")
 *  access     [I] Access level desired
 *  sa         [I] Security attributes for the key
 *  retkey     [O] Destination for the resulting handle
 *  dispos     [O] Receives REG_CREATED_NEW_KEY or REG_OPENED_EXISTING_KEY
 *
 * RETURNS
 *  Success: ERROR_SUCCESS.
 *  Failure: A standard Win32 error code. retkey remains untouched.
 *
 * FIXME
 *  MAXIMUM_ALLOWED in access mask not supported by server
 */
LSTATUS WINAPI DECLSPEC_HOTPATCH RegCreateKeyExA( HKEY hkey, LPCSTR name, DWORD reserved, LPSTR class,
                             DWORD options, REGSAM access, SECURITY_ATTRIBUTES *sa,
                             PHKEY retkey, LPDWORD dispos )
{
    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING classW;
    ANSI_STRING nameA, classA;
    NTSTATUS status;

    if (reserved) return ERROR_INVALID_PARAMETER;
    if (!is_version_nt())
    {
        access = MAXIMUM_ALLOWED;  /* Win95 ignores the access mask */
        if (name && *name == '\\') name++; /* win9x,ME ignores one (and only one) beginning backslash */
    }
    if (!(hkey = get_special_root_hkey( hkey, access ))) return ERROR_INVALID_HANDLE;

    attr.Length = sizeof(attr);
    attr.RootDirectory = hkey;
    attr.ObjectName = &NtCurrentTeb()->StaticUnicodeString;
    attr.Attributes = 0;
    attr.SecurityDescriptor = NULL;
    attr.SecurityQualityOfService = NULL;
    if (options & REG_OPTION_OPEN_LINK) attr.Attributes |= OBJ_OPENLINK;
    RtlInitAnsiString( &nameA, name );
    RtlInitAnsiString( &classA, class );

    if (!(status = RtlAnsiStringToUnicodeString( &NtCurrentTeb()->StaticUnicodeString,
                                                 &nameA, FALSE )))
    {
        if (!(status = RtlAnsiStringToUnicodeString( &classW, &classA, TRUE )))
        {
            status = create_key( retkey, access, &attr, &classW, options, dispos );
            RtlFreeUnicodeString( &classW );
        }
    }
    return RtlNtStatusToDosError( status );
}

/******************************************************************************
 * RegDeleteKeyExW   (kernelbase.@)
 */
LSTATUS WINAPI RegDeleteKeyExW( HKEY hkey, LPCWSTR name, REGSAM access, DWORD reserved )
{
    DWORD ret;
    HKEY tmp;

    if (!name) return ERROR_INVALID_PARAMETER;

    if (!(hkey = get_special_root_hkey( hkey, access ))) return ERROR_INVALID_HANDLE;

    access &= KEY_WOW64_64KEY | KEY_WOW64_32KEY;
    if (!(ret = RegOpenKeyExW( hkey, name, 0, access | DELETE, &tmp )))
    {
        ret = RtlNtStatusToDosError( NtDeleteKey( tmp ) );
        RegCloseKey( tmp );
    }
    TRACE("%s ret=%08x\n", debugstr_w(name), ret);
    return ret;
}


/******************************************************************************
 * RegDeleteKeyExA   (kernelbase.@)
 */
LSTATUS WINAPI RegDeleteKeyExA( HKEY hkey, LPCSTR name, REGSAM access, DWORD reserved )
{
    DWORD ret;
    HKEY tmp;

    if (!name) return ERROR_INVALID_PARAMETER;

    if (!(hkey = get_special_root_hkey( hkey, access ))) return ERROR_INVALID_HANDLE;

    access &= KEY_WOW64_64KEY | KEY_WOW64_32KEY;
    if (!(ret = RegOpenKeyExA( hkey, name, 0, access | DELETE, &tmp )))
    {
        if (!is_version_nt()) /* win95 does recursive key deletes */
        {
            CHAR sub[MAX_PATH];
            DWORD len = sizeof(sub);
            while(!RegEnumKeyExA(tmp, 0, sub, &len, NULL, NULL, NULL, NULL))
            {
                if(RegDeleteKeyExA(tmp, sub, access, reserved))  /* recurse */
                    break;
            }
        }
        ret = RtlNtStatusToDosError( NtDeleteKey( tmp ) );
        RegCloseKey( tmp );
    }
    TRACE("%s ret=%08x\n", debugstr_a(name), ret);
    return ret;
}

/******************************************************************************
 * RegDeleteValueW   (kernelbase.@)
 *
 * See RegDeleteValueA.
 */
LSTATUS WINAPI RegDeleteValueW( HKEY hkey, LPCWSTR name )
{
    return RegDeleteKeyValueW( hkey, NULL, name );
}

/******************************************************************************
 * RegDeleteValueA   (kernelbase.@)
 *
 * Delete a value from the registry.
 *
 * PARAMS
 *  hkey [I] Registry handle of the key holding the value
 *  name [I] Name of the value under hkey to delete
 *
 * RETURNS
 *  Success: ERROR_SUCCESS
 *  Failure: nonzero error code from Winerror.h
 */
LSTATUS WINAPI RegDeleteValueA( HKEY hkey, LPCSTR name )
{
    return RegDeleteKeyValueA( hkey, NULL, name );
}

/******************************************************************************
 * RegEnumKeyExW   (kernelbase.@)
 *
 * Enumerate subkeys of the specified open registry key.
 *
 * PARAMS
 *  hkey         [I] Handle to key to enumerate
 *  index        [I] Index of subkey to enumerate
 *  name         [O] Buffer for subkey name
 *  name_len     [O] Size of subkey buffer
 *  reserved     [I] Reserved
 *  class        [O] Buffer for class string
 *  class_len    [O] Size of class buffer
 *  ft           [O] Time key last written to
 *
 * RETURNS
 *  Success: ERROR_SUCCESS
 *  Failure: System error code. If there are no more subkeys available, the
 *           function returns ERROR_NO_MORE_ITEMS.
 */
LSTATUS WINAPI RegEnumKeyExW( HKEY hkey, DWORD index, LPWSTR name, LPDWORD name_len,
                           LPDWORD reserved, LPWSTR class, LPDWORD class_len, FILETIME *ft )
{
    NTSTATUS status;
    char buffer[256], *buf_ptr = buffer;
    KEY_NODE_INFORMATION *info = (KEY_NODE_INFORMATION *)buffer;
    DWORD total_size;

    TRACE( "(%p,%d,%p,%p(%u),%p,%p,%p,%p)\n", hkey, index, name, name_len,
           name_len ? *name_len : 0, reserved, class, class_len, ft );

    if (reserved) return ERROR_INVALID_PARAMETER;
    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    status = NtEnumerateKey( hkey, index, KeyNodeInformation,
                             buffer, sizeof(buffer), &total_size );

    while (status == STATUS_BUFFER_OVERFLOW)
    {
        /* retry with a dynamically allocated buffer */
        if (buf_ptr != buffer) heap_free( buf_ptr );
        if (!(buf_ptr = heap_alloc( total_size )))
            return ERROR_NOT_ENOUGH_MEMORY;
        info = (KEY_NODE_INFORMATION *)buf_ptr;
        status = NtEnumerateKey( hkey, index, KeyNodeInformation,
                                 buf_ptr, total_size, &total_size );
    }

    if (!status)
    {
        DWORD len = info->NameLength / sizeof(WCHAR);
        DWORD cls_len = info->ClassLength / sizeof(WCHAR);

        if (ft) *ft = *(FILETIME *)&info->LastWriteTime;

        if (len >= *name_len || (class && class_len && (cls_len >= *class_len)))
            status = STATUS_BUFFER_OVERFLOW;
        else
        {
            *name_len = len;
            memcpy( name, info->Name, info->NameLength );
            name[len] = 0;
            if (class_len)
            {
                *class_len = cls_len;
                if (class)
                {
                    memcpy( class, buf_ptr + info->ClassOffset, info->ClassLength );
                    class[cls_len] = 0;
                }
            }
        }
    }

    if (buf_ptr != buffer) heap_free( buf_ptr );
    return RtlNtStatusToDosError( status );
}


/******************************************************************************
 * RegEnumKeyExA   (kernelbase.@)
 *
 * See RegEnumKeyExW.
 */
LSTATUS WINAPI RegEnumKeyExA( HKEY hkey, DWORD index, LPSTR name, LPDWORD name_len,
                           LPDWORD reserved, LPSTR class, LPDWORD class_len, FILETIME *ft )
{
    NTSTATUS status;
    char buffer[256], *buf_ptr = buffer;
    KEY_NODE_INFORMATION *info = (KEY_NODE_INFORMATION *)buffer;
    DWORD total_size;

    TRACE( "(%p,%d,%p,%p(%u),%p,%p,%p,%p)\n", hkey, index, name, name_len,
           name_len ? *name_len : 0, reserved, class, class_len, ft );

    if (reserved) return ERROR_INVALID_PARAMETER;
    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    status = NtEnumerateKey( hkey, index, KeyNodeInformation,
                             buffer, sizeof(buffer), &total_size );

    while (status == STATUS_BUFFER_OVERFLOW)
    {
        /* retry with a dynamically allocated buffer */
        if (buf_ptr != buffer) heap_free( buf_ptr );
        if (!(buf_ptr = heap_alloc( total_size )))
            return ERROR_NOT_ENOUGH_MEMORY;
        info = (KEY_NODE_INFORMATION *)buf_ptr;
        status = NtEnumerateKey( hkey, index, KeyNodeInformation,
                                 buf_ptr, total_size, &total_size );
    }

    if (!status)
    {
        DWORD len, cls_len;

        RtlUnicodeToMultiByteSize( &len, info->Name, info->NameLength );
        RtlUnicodeToMultiByteSize( &cls_len, (WCHAR *)(buf_ptr + info->ClassOffset),
                                   info->ClassLength );
        if (ft) *ft = *(FILETIME *)&info->LastWriteTime;

        if (len >= *name_len || (class && class_len && (cls_len >= *class_len)))
            status = STATUS_BUFFER_OVERFLOW;
        else
        {
            *name_len = len;
            RtlUnicodeToMultiByteN( name, len, NULL, info->Name, info->NameLength );
            name[len] = 0;
            if (class_len)
            {
                *class_len = cls_len;
                if (class)
                {
                    RtlUnicodeToMultiByteN( class, cls_len, NULL,
                                            (WCHAR *)(buf_ptr + info->ClassOffset),
                                            info->ClassLength );
                    class[cls_len] = 0;
                }
            }
        }
    }

    if (buf_ptr != buffer) heap_free( buf_ptr );
    return RtlNtStatusToDosError( status );
}

/******************************************************************************
 * RegEnumValueW   (kernelbase.@)
 *
 * Enumerates the values for the specified open registry key.
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
LSTATUS WINAPI RegEnumValueW( HKEY hkey, DWORD index, LPWSTR value, LPDWORD val_count,
                              LPDWORD reserved, LPDWORD type, LPBYTE data, LPDWORD count )
{
    NTSTATUS status;
    DWORD total_size;
    char buffer[256], *buf_ptr = buffer;
    KEY_VALUE_FULL_INFORMATION *info = (KEY_VALUE_FULL_INFORMATION *)buffer;
    static const int info_size = offsetof( KEY_VALUE_FULL_INFORMATION, Name );

    TRACE("(%p,%d,%p,%p,%p,%p,%p,%p)\n",
          hkey, index, value, val_count, reserved, type, data, count );

    if ((data && !count) || reserved || !value || !val_count)
        return ERROR_INVALID_PARAMETER;
    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    total_size = info_size + (MAX_PATH + 1) * sizeof(WCHAR);
    if (data) total_size += *count;
    total_size = min( sizeof(buffer), total_size );

    status = NtEnumerateValueKey( hkey, index, KeyValueFullInformation,
                                  buffer, total_size, &total_size );

    /* retry with a dynamically allocated buffer */
    while (status == STATUS_BUFFER_OVERFLOW)
    {
        if (buf_ptr != buffer) heap_free( buf_ptr );
        if (!(buf_ptr = heap_alloc( total_size )))
            return ERROR_NOT_ENOUGH_MEMORY;
        info = (KEY_VALUE_FULL_INFORMATION *)buf_ptr;
        status = NtEnumerateValueKey( hkey, index, KeyValueFullInformation,
                                      buf_ptr, total_size, &total_size );
    }

    if (status) goto done;

    if (info->NameLength/sizeof(WCHAR) >= *val_count)
    {
        status = STATUS_BUFFER_OVERFLOW;
        goto overflow;
    }
    memcpy( value, info->Name, info->NameLength );
    *val_count = info->NameLength / sizeof(WCHAR);
    value[*val_count] = 0;

    if (data)
    {
        if (total_size - info->DataOffset > *count)
        {
            status = STATUS_BUFFER_OVERFLOW;
            goto overflow;
        }
        memcpy( data, buf_ptr + info->DataOffset, total_size - info->DataOffset );
        if (total_size - info->DataOffset <= *count-sizeof(WCHAR) && is_string(info->Type))
        {
            /* if the type is REG_SZ and data is not 0-terminated
             * and there is enough space in the buffer NT appends a \0 */
            WCHAR *ptr = (WCHAR *)(data + total_size - info->DataOffset);
            if (ptr > (WCHAR *)data && ptr[-1]) *ptr = 0;
        }
    }

 overflow:
    if (type) *type = info->Type;
    if (count) *count = info->DataLength;

 done:
    if (buf_ptr != buffer) heap_free( buf_ptr );
    return RtlNtStatusToDosError(status);
}


/******************************************************************************
 * RegEnumValueA   (kernelbase.@)
 *
 * See RegEnumValueW.
 */
LSTATUS WINAPI RegEnumValueA( HKEY hkey, DWORD index, LPSTR value, LPDWORD val_count,
                              LPDWORD reserved, LPDWORD type, LPBYTE data, LPDWORD count )
{
    NTSTATUS status;
    DWORD total_size;
    char buffer[256], *buf_ptr = buffer;
    KEY_VALUE_FULL_INFORMATION *info = (KEY_VALUE_FULL_INFORMATION *)buffer;
    static const int info_size = offsetof( KEY_VALUE_FULL_INFORMATION, Name );

    TRACE("(%p,%d,%p,%p,%p,%p,%p,%p)\n",
          hkey, index, value, val_count, reserved, type, data, count );

    if ((data && !count) || reserved || !value || !val_count)
        return ERROR_INVALID_PARAMETER;
    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    total_size = info_size + (MAX_PATH + 1) * sizeof(WCHAR);
    if (data) total_size += *count;
    total_size = min( sizeof(buffer), total_size );

    status = NtEnumerateValueKey( hkey, index, KeyValueFullInformation,
                                  buffer, total_size, &total_size );

    /* we need to fetch the contents for a string type even if not requested,
     * because we need to compute the length of the ASCII string. */

    /* retry with a dynamically allocated buffer */
    while (status == STATUS_BUFFER_OVERFLOW)
    {
        if (buf_ptr != buffer) heap_free( buf_ptr );
        if (!(buf_ptr = heap_alloc( total_size )))
            return ERROR_NOT_ENOUGH_MEMORY;
        info = (KEY_VALUE_FULL_INFORMATION *)buf_ptr;
        status = NtEnumerateValueKey( hkey, index, KeyValueFullInformation,
                                      buf_ptr, total_size, &total_size );
    }

    if (status) goto done;

    if (is_string(info->Type))
    {
        DWORD len;
        RtlUnicodeToMultiByteSize( &len, (WCHAR *)(buf_ptr + info->DataOffset),
                                   total_size - info->DataOffset );
        if (data && len)
        {
            if (len > *count) status = STATUS_BUFFER_OVERFLOW;
            else
            {
                RtlUnicodeToMultiByteN( (char*)data, len, NULL, (WCHAR *)(buf_ptr + info->DataOffset),
                                        total_size - info->DataOffset );
                /* if the type is REG_SZ and data is not 0-terminated
                 * and there is enough space in the buffer NT appends a \0 */
                if (len < *count && data[len-1]) data[len] = 0;
            }
        }
        info->DataLength = len;
    }
    else if (data)
    {
        if (total_size - info->DataOffset > *count) status = STATUS_BUFFER_OVERFLOW;
        else memcpy( data, buf_ptr + info->DataOffset, total_size - info->DataOffset );
    }

    if (!status)
    {
        DWORD len;

        RtlUnicodeToMultiByteSize( &len, info->Name, info->NameLength );
        if (len >= *val_count)
        {
            status = STATUS_BUFFER_OVERFLOW;
            if (*val_count)
            {
                len = *val_count - 1;
                RtlUnicodeToMultiByteN( value, len, NULL, info->Name, info->NameLength );
                value[len] = 0;
            }
        }
        else
        {
            RtlUnicodeToMultiByteN( value, len, NULL, info->Name, info->NameLength );
            value[len] = 0;
            *val_count = len;
        }
    }

    if (type) *type = info->Type;
    if (count) *count = info->DataLength;

 done:
    if (buf_ptr != buffer) heap_free( buf_ptr );
    return RtlNtStatusToDosError(status);
}

/* wrapper for NtOpenKeyEx to handle Wow6432 nodes */
static NTSTATUS open_key( HKEY *retkey, DWORD options, ACCESS_MASK access, OBJECT_ATTRIBUTES *attr )
{
    NTSTATUS status;
    BOOL force_wow32 = is_win64 && (access & KEY_WOW64_32KEY);
    HANDLE subkey, root = attr->RootDirectory;
    WCHAR *buffer = attr->ObjectName->Buffer;
    DWORD pos = 0, i = 0, len = attr->ObjectName->Length / sizeof(WCHAR);
    UNICODE_STRING str;

    *retkey = NULL;

    if (!force_wow32)
    {
        if (options & REG_OPTION_OPEN_LINK) attr->Attributes |= OBJ_OPENLINK;
        return NtOpenKeyEx( (HANDLE *)retkey, access, attr, options );
    }

    if (len && buffer[0] == '\\') return STATUS_OBJECT_PATH_INVALID;
    while (i < len && buffer[i] != '\\') i++;
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
            if (options & REG_OPTION_OPEN_LINK) attr->Attributes |= OBJ_OPENLINK;
            status = NtOpenKeyEx( &subkey, access, attr, options );
        }
        else
        {
            if (!(options & REG_OPTION_OPEN_LINK)) attr->Attributes &= ~OBJ_OPENLINK;
            status = NtOpenKeyEx( &subkey, access, attr, options & ~REG_OPTION_OPEN_LINK );
        }
        if (attr->RootDirectory != root) NtClose( attr->RootDirectory );
        if (status) return status;
        attr->RootDirectory = subkey;
        if (i == len) break;
        while (i < len && buffer[i] == '\\') i++;
        pos = i;
        while (i < len && buffer[i] != '\\') i++;
    }
    if (force_wow32 && (subkey = open_wow6432node( attr->RootDirectory )))
    {
        if (attr->RootDirectory != root) NtClose( attr->RootDirectory );
        attr->RootDirectory = subkey;
    }
    *retkey = attr->RootDirectory;
    return status;
}

/******************************************************************************
 * RegOpenKeyExW   (kernelbase.@)
 *
 * See RegOpenKeyExA.
 */
LSTATUS WINAPI DECLSPEC_HOTPATCH RegOpenKeyExW( HKEY hkey, LPCWSTR name, DWORD options, REGSAM access, PHKEY retkey )
{
    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING nameW;

    if (retkey && (!name || !name[0]) &&
        (HandleToUlong(hkey) >= HandleToUlong(HKEY_SPECIAL_ROOT_FIRST)) &&
        (HandleToUlong(hkey) <= HandleToUlong(HKEY_SPECIAL_ROOT_LAST)))
    {
        *retkey = hkey;
        return ERROR_SUCCESS;
    }

    /* NT+ allows beginning backslash for HKEY_CLASSES_ROOT */
    if (HandleToUlong(hkey) == HandleToUlong(HKEY_CLASSES_ROOT) && name && *name == '\\') name++;

    if (!retkey) return ERROR_INVALID_PARAMETER;
    *retkey = NULL;
    if (!(hkey = get_special_root_hkey( hkey, access ))) return ERROR_INVALID_HANDLE;

    attr.Length = sizeof(attr);
    attr.RootDirectory = hkey;
    attr.ObjectName = &nameW;
    attr.Attributes = 0;
    attr.SecurityDescriptor = NULL;
    attr.SecurityQualityOfService = NULL;
    RtlInitUnicodeString( &nameW, name );
    return RtlNtStatusToDosError( open_key( retkey, options, access, &attr ) );
}


/******************************************************************************
 * RegOpenKeyExA   (kernelbase.@)
 *
 * Open a registry key.
 *
 * PARAMS
 *  hkey       [I] Handle of open key
 *  name       [I] Name of subkey to open
 *  options    [I] Open options (can be set to REG_OPTION_OPEN_LINK)
 *  access     [I] Security access mask
 *  retkey     [O] Handle to open key
 *
 * RETURNS
 *  Success: ERROR_SUCCESS
 *  Failure: A standard Win32 error code. retkey is set to 0.
 *
 * NOTES
 *  Unlike RegCreateKeyExA(), this function will not create the key if it
 *  does not exist.
 */
LSTATUS WINAPI DECLSPEC_HOTPATCH RegOpenKeyExA( HKEY hkey, LPCSTR name, DWORD options, REGSAM access, PHKEY retkey )
{
    OBJECT_ATTRIBUTES attr;
    STRING nameA;
    NTSTATUS status;

    if (retkey && (!name || !name[0]) &&
        (HandleToUlong(hkey) >= HandleToUlong(HKEY_SPECIAL_ROOT_FIRST)) &&
        (HandleToUlong(hkey) <= HandleToUlong(HKEY_SPECIAL_ROOT_LAST)))
    {
        *retkey = hkey;
        return ERROR_SUCCESS;
    }

    if (!is_version_nt()) access = MAXIMUM_ALLOWED;  /* Win95 ignores the access mask */
    else
    {
        /* NT+ allows beginning backslash for HKEY_CLASSES_ROOT */
        if (HandleToUlong(hkey) == HandleToUlong(HKEY_CLASSES_ROOT) && name && *name == '\\') name++;
    }

    if (!(hkey = get_special_root_hkey( hkey, access ))) return ERROR_INVALID_HANDLE;

    attr.Length = sizeof(attr);
    attr.RootDirectory = hkey;
    attr.ObjectName = &NtCurrentTeb()->StaticUnicodeString;
    attr.Attributes = 0;
    attr.SecurityDescriptor = NULL;
    attr.SecurityQualityOfService = NULL;

    RtlInitAnsiString( &nameA, name );
    if (!(status = RtlAnsiStringToUnicodeString( &NtCurrentTeb()->StaticUnicodeString,
                                                 &nameA, FALSE )))
    {
        status = open_key( retkey, options, access, &attr );
    }
    return RtlNtStatusToDosError( status );
}

/******************************************************************************
 * RegQueryInfoKeyW   (kernelbase.@)
 *
 * Retrieves information about the specified registry key.
 *
 * PARAMS
 *    hkey       [I] Handle to key to query
 *    class      [O] Buffer for class string
 *    class_len  [O] Size of class string buffer
 *    reserved   [I] Reserved
 *    subkeys    [O] Buffer for number of subkeys
 *    max_subkey [O] Buffer for longest subkey name length
 *    max_class  [O] Buffer for longest class string length
 *    values     [O] Buffer for number of value entries
 *    max_value  [O] Buffer for longest value name length
 *    max_data   [O] Buffer for longest value data length
 *    security   [O] Buffer for security descriptor length
 *    modif      [O] Modification time
 *
 * RETURNS
 *  Success: ERROR_SUCCESS
 *  Failure: system error code.
 *
 * NOTES
 *  - win95 allows class to be valid and class_len to be NULL
 *  - winnt returns ERROR_INVALID_PARAMETER if class is valid and class_len is NULL
 *  - both allow class to be NULL and class_len to be NULL
 *    (it's hard to test validity, so test !NULL instead)
 */
LSTATUS WINAPI RegQueryInfoKeyW( HKEY hkey, LPWSTR class, LPDWORD class_len, LPDWORD reserved,
                                 LPDWORD subkeys, LPDWORD max_subkey, LPDWORD max_class,
                                 LPDWORD values, LPDWORD max_value, LPDWORD max_data,
                                 LPDWORD security, FILETIME *modif )
{
    NTSTATUS status;
    char buffer[256], *buf_ptr = buffer;
    KEY_FULL_INFORMATION *info = (KEY_FULL_INFORMATION *)buffer;
    DWORD total_size;

    TRACE( "(%p,%p,%d,%p,%p,%p,%p,%p,%p,%p,%p)\n", hkey, class, class_len ? *class_len : 0,
           reserved, subkeys, max_subkey, values, max_value, max_data, security, modif );

    if (class && !class_len && is_version_nt()) return ERROR_INVALID_PARAMETER;
    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    status = NtQueryKey( hkey, KeyFullInformation, buffer, sizeof(buffer), &total_size );
    if (status && status != STATUS_BUFFER_OVERFLOW) goto done;

    if (class && class_len && *class_len)
    {
        /* retry with a dynamically allocated buffer */
        while (status == STATUS_BUFFER_OVERFLOW)
        {
            if (buf_ptr != buffer) heap_free( buf_ptr );
            if (!(buf_ptr = heap_alloc( total_size )))
                return ERROR_NOT_ENOUGH_MEMORY;
            info = (KEY_FULL_INFORMATION *)buf_ptr;
            status = NtQueryKey( hkey, KeyFullInformation, buf_ptr, total_size, &total_size );
        }

        if (status) goto done;

        if (class_len && (info->ClassLength/sizeof(WCHAR) + 1 > *class_len))
        {
            status = STATUS_BUFFER_TOO_SMALL;
        }
        else
        {
            memcpy( class, buf_ptr + info->ClassOffset, info->ClassLength );
            class[info->ClassLength/sizeof(WCHAR)] = 0;
        }
    }
    else status = STATUS_SUCCESS;

    if (class_len) *class_len = info->ClassLength / sizeof(WCHAR);
    if (subkeys) *subkeys = info->SubKeys;
    if (max_subkey) *max_subkey = info->MaxNameLen / sizeof(WCHAR);
    if (max_class) *max_class = info->MaxClassLen / sizeof(WCHAR);
    if (values) *values = info->Values;
    if (max_value) *max_value = info->MaxValueNameLen / sizeof(WCHAR);
    if (max_data) *max_data = info->MaxValueDataLen;
    if (modif) *modif = *(FILETIME *)&info->LastWriteTime;

    if (security)
    {
        FIXME( "security argument not supported.\n");
        *security = 0;
    }

 done:
    if (buf_ptr != buffer) heap_free( buf_ptr );
    return RtlNtStatusToDosError( status );
}


/******************************************************************************
 * RegQueryInfoKeyA   (kernelbase.@)
 *
 * Retrieves information about a registry key.
 *
 * PARAMS
 *  hKey                   [I] Handle to an open key.
 *  lpClass                [O] Class string of the key.
 *  lpcClass               [I/O] size of lpClass.
 *  lpReserved             [I] Reserved; must be NULL.
 *  lpcSubKeys             [O] Number of subkeys contained by the key.
 *  lpcMaxSubKeyLen        [O] Size of the key's subkey with the longest name.
 *  lpcMaxClassLen         [O] Size of the longest string specifying a subkey
 *                             class in TCHARS.
 *  lpcValues              [O] Number of values associated with the key.
 *  lpcMaxValueNameLen     [O] Size of the key's longest value name in TCHARS.
 *  lpcMaxValueLen         [O] Longest data component among the key's values
 *  lpcbSecurityDescriptor [O] Size of the key's security descriptor.
 *  lpftLastWriteTime      [O] FILETIME structure that is the last write time.
 *
 *  RETURNS
 *   Success: ERROR_SUCCESS
 *   Failure: nonzero error code from Winerror.h
 */
LSTATUS WINAPI RegQueryInfoKeyA( HKEY hkey, LPSTR class, LPDWORD class_len, LPDWORD reserved,
                              LPDWORD subkeys, LPDWORD max_subkey, LPDWORD max_class,
                              LPDWORD values, LPDWORD max_value, LPDWORD max_data,
                              LPDWORD security, FILETIME *modif )
{
    NTSTATUS status;
    char buffer[256], *buf_ptr = buffer;
    KEY_FULL_INFORMATION *info = (KEY_FULL_INFORMATION *)buffer;
    DWORD total_size;

    TRACE( "(%p,%p,%d,%p,%p,%p,%p,%p,%p,%p,%p)\n", hkey, class, class_len ? *class_len : 0,
           reserved, subkeys, max_subkey, values, max_value, max_data, security, modif );

    if (class && !class_len && is_version_nt()) return ERROR_INVALID_PARAMETER;
    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    status = NtQueryKey( hkey, KeyFullInformation, buffer, sizeof(buffer), &total_size );
    if (status && status != STATUS_BUFFER_OVERFLOW) goto done;

    if (class || class_len)
    {
        /* retry with a dynamically allocated buffer */
        while (status == STATUS_BUFFER_OVERFLOW)
        {
            if (buf_ptr != buffer) heap_free( buf_ptr );
            if (!(buf_ptr = heap_alloc( total_size )))
                return ERROR_NOT_ENOUGH_MEMORY;
            info = (KEY_FULL_INFORMATION *)buf_ptr;
            status = NtQueryKey( hkey, KeyFullInformation, buf_ptr, total_size, &total_size );
        }

        if (status) goto done;

        if (class && class_len && *class_len)
        {
            DWORD len = *class_len;
            RtlUnicodeToMultiByteN( class, len, class_len,
                                    (WCHAR *)(buf_ptr + info->ClassOffset), info->ClassLength );
            if (*class_len == len)
            {
                status = STATUS_BUFFER_OVERFLOW;
                *class_len -= 1;
            }
            class[*class_len] = 0;
        }
        else if (class_len)
            RtlUnicodeToMultiByteSize( class_len,
                                       (WCHAR *)(buf_ptr + info->ClassOffset), info->ClassLength );
    }
    else status = STATUS_SUCCESS;

    if (subkeys) *subkeys = info->SubKeys;
    if (max_subkey) *max_subkey = info->MaxNameLen / sizeof(WCHAR);
    if (max_class) *max_class = info->MaxClassLen / sizeof(WCHAR);
    if (values) *values = info->Values;
    if (max_value) *max_value = info->MaxValueNameLen / sizeof(WCHAR);
    if (max_data) *max_data = info->MaxValueDataLen;
    if (modif) *modif = *(FILETIME *)&info->LastWriteTime;

    if (security)
    {
        FIXME( "security argument not supported.\n");
        *security = 0;
    }

 done:
    if (buf_ptr != buffer) heap_free( buf_ptr );
    return RtlNtStatusToDosError( status );
}

static void *get_provider_entry(HKEY perf, HMODULE perflib, const char *name)
{
    char buf[MAX_PATH];
    DWORD err, type, len;

    len = sizeof(buf) - 1;
    err = RegQueryValueExA(perf, name, NULL, &type, (BYTE *)buf, &len);
    if (err != ERROR_SUCCESS || type != REG_SZ)
        return NULL;

    buf[len] = 0;
    TRACE("Loading function pointer for %s: %s\n", name, debugstr_a(buf));

    return GetProcAddress(perflib, buf);
}

static BOOL load_provider(HKEY root, const WCHAR *name, struct perf_provider *provider)
{
    WCHAR buf[MAX_PATH], buf2[MAX_PATH];
    DWORD err, type, len;
    HKEY service, perf;

    err = RegOpenKeyExW(root, name, 0, KEY_READ, &service);
    if (err != ERROR_SUCCESS)
        return FALSE;

    provider->linkage[0] = 0;
    err = RegOpenKeyExW(service, L"Linkage", 0, KEY_READ, &perf);
    if (err == ERROR_SUCCESS)
    {
        len = sizeof(buf) - sizeof(WCHAR);
        err = RegQueryValueExW(perf, L"Export", NULL, &type, (BYTE *)buf, &len);
        if (err == ERROR_SUCCESS && (type == REG_SZ || type == REG_MULTI_SZ))
        {
            memcpy(provider->linkage, buf, len);
            provider->linkage[len / sizeof(WCHAR)] = 0;
            TRACE("Export: %s\n", debugstr_w(provider->linkage));
        }
        RegCloseKey(perf);
    }

    err = RegOpenKeyExW(service, L"Performance", 0, KEY_READ, &perf);
    RegCloseKey(service);
    if (err != ERROR_SUCCESS)
        return FALSE;

    provider->objects[0] = 0;
    len = sizeof(buf) - sizeof(WCHAR);
    err = RegQueryValueExW(perf, L"Object List", NULL, &type, (BYTE *)buf, &len);
    if (err == ERROR_SUCCESS && (type == REG_SZ || type == REG_MULTI_SZ))
    {
        memcpy(provider->objects, buf, len);
        provider->objects[len / sizeof(WCHAR)] = 0;
        TRACE("Object List: %s\n", debugstr_w(provider->objects));
    }

    len = sizeof(buf) - sizeof(WCHAR);
    err = RegQueryValueExW(perf, L"Library", NULL, &type, (BYTE *)buf, &len);
    if (err != ERROR_SUCCESS || !(type == REG_SZ || type == REG_EXPAND_SZ))
        goto error;

    buf[len / sizeof(WCHAR)] = 0;
    if (type == REG_EXPAND_SZ)
    {
        len = ExpandEnvironmentStringsW(buf, buf2, MAX_PATH);
        if (!len || len > MAX_PATH) goto error;
        lstrcpyW(buf, buf2);
    }

    if (!(provider->perflib = LoadLibraryW(buf)))
    {
        WARN("Failed to load %s\n", debugstr_w(buf));
        goto error;
    }

    GetModuleFileNameW(provider->perflib, buf, MAX_PATH);
    TRACE("Loaded provider %s\n", wine_dbgstr_w(buf));

    provider->pOpen = get_provider_entry(perf, provider->perflib, "Open");
    provider->pClose = get_provider_entry(perf, provider->perflib, "Close");
    provider->pCollect = get_provider_entry(perf, provider->perflib, "Collect");
    if (provider->pOpen && provider->pClose && provider->pCollect)
    {
        RegCloseKey(perf);
        return TRUE;
    }

    TRACE("Provider is missing required exports\n");
    FreeLibrary(provider->perflib);

error:
    RegCloseKey(perf);
    return FALSE;
}

static DWORD collect_data(struct perf_provider *provider, const WCHAR *query, void **data, DWORD *size, DWORD *obj_count)
{
    WCHAR *linkage = provider->linkage[0] ? provider->linkage : NULL;
    DWORD err;

    if (!query || !query[0])
        query = L"Global";

    err = provider->pOpen(linkage);
    if (err != ERROR_SUCCESS)
    {
        TRACE("Open(%s) error %u (%#x)\n", debugstr_w(linkage), err, err);
        return err;
    }

    *obj_count = 0;
    err = provider->pCollect((WCHAR *)query, data, size, obj_count);
    if (err != ERROR_SUCCESS)
    {
        TRACE("Collect error %u (%#x)\n", err, err);
        *obj_count = 0;
    }

    provider->pClose();
    return err;
}

#define MAX_SERVICE_NAME 260

static DWORD query_perf_data(const WCHAR *query, DWORD *type, void *data, DWORD *ret_size)
{
    DWORD err, i, data_size;
    HKEY root;
    PERF_DATA_BLOCK *pdb;

    if (!ret_size)
        return ERROR_INVALID_PARAMETER;

    data_size = *ret_size;
    *ret_size = 0;

    if (type)
        *type = REG_BINARY;

    if (!data || data_size < sizeof(*pdb))
        return ERROR_MORE_DATA;

    pdb = data;

    pdb->Signature[0] = 'P';
    pdb->Signature[1] = 'E';
    pdb->Signature[2] = 'R';
    pdb->Signature[3] = 'F';
#ifdef WORDS_BIGENDIAN
    pdb->LittleEndian = FALSE;
#else
    pdb->LittleEndian = TRUE;
#endif
    pdb->Version = PERF_DATA_VERSION;
    pdb->Revision = PERF_DATA_REVISION;
    pdb->TotalByteLength = 0;
    pdb->HeaderLength = sizeof(*pdb);
    pdb->NumObjectTypes = 0;
    pdb->DefaultObject = 0;
    NtQueryPerformanceCounter( &pdb->PerfTime, &pdb->PerfFreq );

    data = pdb + 1;
    pdb->SystemNameOffset = sizeof(*pdb);
    pdb->SystemNameLength = (data_size - sizeof(*pdb)) / sizeof(WCHAR);
    if (!GetComputerNameExW(ComputerNameNetBIOS, data, &pdb->SystemNameLength))
        return ERROR_MORE_DATA;

    pdb->SystemNameLength++;
    pdb->SystemNameLength *= sizeof(WCHAR);

    pdb->HeaderLength += pdb->SystemNameLength;

    /* align to 8 bytes */
    if (pdb->SystemNameLength & 7)
        pdb->HeaderLength += 8 - (pdb->SystemNameLength & 7);

    if (data_size < pdb->HeaderLength)
        return ERROR_MORE_DATA;

    pdb->TotalByteLength = pdb->HeaderLength;

    data_size -= pdb->HeaderLength;
    data = (char *)data + pdb->HeaderLength;

    err = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Services", 0, KEY_READ, &root);
    if (err != ERROR_SUCCESS)
        return err;

    i = 0;
    for (;;)
    {
        DWORD collected_size = data_size, obj_count = 0;
        struct perf_provider provider;
        WCHAR name[MAX_SERVICE_NAME];
        DWORD len = ARRAY_SIZE( name );
        void *collected_data = data;

        err = RegEnumKeyExW(root, i++, name, &len, NULL, NULL, NULL, NULL);
        if (err == ERROR_NO_MORE_ITEMS)
        {
            err = ERROR_SUCCESS;
            break;
        }

        if (err != ERROR_SUCCESS)
            continue;

        if (!load_provider(root, name, &provider))
            continue;

        err = collect_data(&provider, query, &collected_data, &collected_size, &obj_count);
        FreeLibrary(provider.perflib);

        if (err == ERROR_MORE_DATA)
            break;

        if (err == ERROR_SUCCESS)
        {
            PERF_OBJECT_TYPE *obj = (PERF_OBJECT_TYPE *)data;

            TRACE("Collect: obj->TotalByteLength %u, collected_size %u\n",
                obj->TotalByteLength, collected_size);

            data_size -= collected_size;
            data = collected_data;

            pdb->TotalByteLength += collected_size;
            pdb->NumObjectTypes += obj_count;
        }
    }

    RegCloseKey(root);

    if (err == ERROR_SUCCESS)
    {
        *ret_size = pdb->TotalByteLength;

        GetSystemTime(&pdb->SystemTime);
        GetSystemTimeAsFileTime((FILETIME *)&pdb->PerfTime100nSec);
    }

    return err;
}

/******************************************************************************
 * RegQueryValueExW   (kernelbase.@)
 *
 * See RegQueryValueExA.
 */
LSTATUS WINAPI DECLSPEC_HOTPATCH RegQueryValueExW( HKEY hkey, LPCWSTR name, LPDWORD reserved, LPDWORD type,
                                                   LPBYTE data, LPDWORD count )
{
    NTSTATUS status;
    UNICODE_STRING name_str;
    DWORD total_size;
    char buffer[256], *buf_ptr = buffer;
    KEY_VALUE_PARTIAL_INFORMATION *info = (KEY_VALUE_PARTIAL_INFORMATION *)buffer;
    static const int info_size = offsetof( KEY_VALUE_PARTIAL_INFORMATION, Data );

    TRACE("(%p,%s,%p,%p,%p,%p=%d)\n",
          hkey, debugstr_w(name), reserved, type, data, count,
          (count && data) ? *count : 0 );

    if ((data && !count) || reserved) return ERROR_INVALID_PARAMETER;

    if (hkey == HKEY_PERFORMANCE_DATA)
        return query_perf_data(name, type, data, count);

    if (!(hkey = get_special_root_hkey( hkey, 0 ))) return ERROR_INVALID_HANDLE;

    RtlInitUnicodeString( &name_str, name );

    if (data) total_size = min( sizeof(buffer), *count + info_size );
    else
    {
        total_size = info_size;
        if (count) *count = 0;
    }

    status = NtQueryValueKey( hkey, &name_str, KeyValuePartialInformation,
                              buffer, total_size, &total_size );
    if (status && status != STATUS_BUFFER_OVERFLOW) goto done;

    if (data)
    {
        /* retry with a dynamically allocated buffer */
        while (status == STATUS_BUFFER_OVERFLOW && total_size - info_size <= *count)
        {
            if (buf_ptr != buffer) heap_free( buf_ptr );
            if (!(buf_ptr = heap_alloc( total_size )))
                return ERROR_NOT_ENOUGH_MEMORY;
            info = (KEY_VALUE_PARTIAL_INFORMATION *)buf_ptr;
            status = NtQueryValueKey( hkey, &name_str, KeyValuePartialInformation,
                                      buf_ptr, total_size, &total_size );
        }

        if (!status)
        {
            memcpy( data, buf_ptr + info_size, total_size - info_size );
            /* if the type is REG_SZ and data is not 0-terminated
             * and there is enough space in the buffer NT appends a \0 */
            if (total_size - info_size <= *count-sizeof(WCHAR) && is_string(info->Type))
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
    if (buf_ptr != buffer) heap_free( buf_ptr );
    return RtlNtStatusToDosError(status);
}


/******************************************************************************
 * RegQueryValueExA   (kernelbase.@)
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
LSTATUS WINAPI DECLSPEC_HOTPATCH RegQueryValueExA( HKEY hkey, LPCSTR name, LPDWORD reserved,
                                                   LPDWORD type, LPBYTE data, LPDWORD count )
{
    NTSTATUS status;
    ANSI_STRING nameA;
    UNICODE_STRING nameW;
    DWORD total_size, datalen = 0;
    char buffer[256], *buf_ptr = buffer;
    KEY_VALUE_PARTIAL_INFORMATION *info = (KEY_VALUE_PARTIAL_INFORMATION *)buffer;
    static const int info_size = offsetof( KEY_VALUE_PARTIAL_INFORMATION, Data );

    TRACE("(%p,%s,%p,%p,%p,%p=%d)\n",
          hkey, debugstr_a(name), reserved, type, data, count, count ? *count : 0 );

    if ((data && !count) || reserved) return ERROR_INVALID_PARAMETER;
    if (hkey != HKEY_PERFORMANCE_DATA && !(hkey = get_special_root_hkey( hkey, 0 )))
        return ERROR_INVALID_HANDLE;

    if (count) datalen = *count;
    if (!data && count) *count = 0;

    /* this matches Win9x behaviour - NT sets *type to a random value */
    if (type) *type = REG_NONE;

    RtlInitAnsiString( &nameA, name );
    if ((status = RtlAnsiStringToUnicodeString( &nameW, &nameA, TRUE )))
        return RtlNtStatusToDosError(status);

    if (hkey == HKEY_PERFORMANCE_DATA)
    {
        DWORD ret = query_perf_data( nameW.Buffer, type, data, count );
        RtlFreeUnicodeString( &nameW );
        return ret;
    }

    status = NtQueryValueKey( hkey, &nameW, KeyValuePartialInformation,
                              buffer, sizeof(buffer), &total_size );
    if (status && status != STATUS_BUFFER_OVERFLOW) goto done;

    /* we need to fetch the contents for a string type even if not requested,
     * because we need to compute the length of the ASCII string. */
    if (data || is_string(info->Type))
    {
        /* retry with a dynamically allocated buffer */
        while (status == STATUS_BUFFER_OVERFLOW)
        {
            if (buf_ptr != buffer) heap_free( buf_ptr );
            if (!(buf_ptr = heap_alloc( total_size )))
            {
                status = STATUS_NO_MEMORY;
                goto done;
            }
            info = (KEY_VALUE_PARTIAL_INFORMATION *)buf_ptr;
            status = NtQueryValueKey( hkey, &nameW, KeyValuePartialInformation,
                                      buf_ptr, total_size, &total_size );
        }

        if (status) goto done;

        if (is_string(info->Type))
        {
            DWORD len;

            RtlUnicodeToMultiByteSize( &len, (WCHAR *)(buf_ptr + info_size),
                                       total_size - info_size );
            if (data && len)
            {
                if (len > datalen) status = STATUS_BUFFER_OVERFLOW;
                else
                {
                    RtlUnicodeToMultiByteN( (char*)data, len, NULL, (WCHAR *)(buf_ptr + info_size),
                                            total_size - info_size );
                    /* if the type is REG_SZ and data is not 0-terminated
                     * and there is enough space in the buffer NT appends a \0 */
                    if (len < datalen && data[len-1]) data[len] = 0;
                }
            }
            total_size = len + info_size;
        }
        else if (data)
        {
            if (total_size - info_size > datalen) status = STATUS_BUFFER_OVERFLOW;
            else memcpy( data, buf_ptr + info_size, total_size - info_size );
        }
    }
    else status = STATUS_SUCCESS;

    if (type) *type = info->Type;
    if (count) *count = total_size - info_size;

 done:
    if (buf_ptr != buffer) heap_free( buf_ptr );
    RtlFreeUnicodeString( &nameW );
    return RtlNtStatusToDosError(status);
}