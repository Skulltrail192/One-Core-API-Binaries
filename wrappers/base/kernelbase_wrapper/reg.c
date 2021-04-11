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

LONG
RegDisablePredefinedCacheEx(
)
{
	return RegDisablePredefinedCache();
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
