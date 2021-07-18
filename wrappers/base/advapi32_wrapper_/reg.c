/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    reg.c

Abstract:

    Registry Handle functions

Author:

    Skulltrail 07-March-2021

Revision History:

--*/
#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(advapi32);

/* DEFINES ******************************************************************/

#define MAX_DEFAULT_HANDLES   6
#define REG_MAX_NAME_SIZE     256
#define REG_MAX_DATA_SIZE     2048

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

/************************************************************************
 *  RegInitDefaultHandles
 */
BOOL
RegInitialize(VOID)
{
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

static NTSTATUS
OpenClassesRootKey(PHANDLE KeyHandle)
{
    OBJECT_ATTRIBUTES Attributes;
    UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\Software\\CLASSES");

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
OpenLocalMachineKey(PHANDLE KeyHandle)
{
    OBJECT_ATTRIBUTES Attributes;
    UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"\\Registry\\Machine");
    NTSTATUS Status;

    InitializeObjectAttributes(&Attributes,
                               &KeyName,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);
    Status = NtOpenKey(KeyHandle,
                       MAXIMUM_ALLOWED,
                       &Attributes);

    return Status;
}


static NTSTATUS
OpenUsersKey(PHANDLE KeyHandle)
{
    OBJECT_ATTRIBUTES Attributes;
    UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"\\Registry\\User");

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

static NTSTATUS
RegpCopyTree(IN HKEY hKeySrc,
             IN HKEY hKeyDest)
{
    typedef struct
    {
        LIST_ENTRY ListEntry;
        HANDLE hKeySrc;
        HANDLE hKeyDest;
    } REGP_COPY_KEYS, *PREGP_COPY_KEYS;

    LIST_ENTRY copyQueueHead;
    PREGP_COPY_KEYS copyKeys, newCopyKeys;
    union
    {
        KEY_VALUE_FULL_INFORMATION *KeyValue;
        KEY_NODE_INFORMATION *KeyNode;
        PVOID Buffer;
    } Info;
    ULONG Index, BufferSizeRequired, BufferSize = 0x200;
    NTSTATUS Status = STATUS_SUCCESS;
    NTSTATUS Status2 = STATUS_SUCCESS;

    InitializeListHead(&copyQueueHead);

    Info.Buffer = RtlAllocateHeap(ProcessHeap,
                                  0,
                                  BufferSize);
    if (Info.Buffer == NULL)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    copyKeys = RtlAllocateHeap(ProcessHeap,
                               0,
                               sizeof(REGP_COPY_KEYS));
    if (copyKeys != NULL)
    {
        copyKeys->hKeySrc = hKeySrc;
        copyKeys->hKeyDest = hKeyDest;
        InsertHeadList(&copyQueueHead,
                       &copyKeys->ListEntry);

        /* FIXME - copy security from hKeySrc to hKeyDest or just for the subkeys? */

        do
        {
            copyKeys = CONTAINING_RECORD(copyQueueHead.Flink,
                                         REGP_COPY_KEYS,
                                         ListEntry);

            /* enumerate all values and copy them */
            Index = 0;
            for (;;)
            {
                Status2 = NtEnumerateValueKey(copyKeys->hKeySrc,
                                              Index,
                                              KeyValueFullInformation,
                                              Info.KeyValue,
                                              BufferSize,
                                              &BufferSizeRequired);
                if (NT_SUCCESS(Status2))
                {
                    UNICODE_STRING ValueName;
                    PVOID Data;

                    /* don't use RtlInitUnicodeString as the string is not NULL-terminated! */
                    ValueName.Length = Info.KeyValue->NameLength;
                    ValueName.MaximumLength = ValueName.Length;
                    ValueName.Buffer = Info.KeyValue->Name;

                    Data = (PVOID)((ULONG_PTR)Info.KeyValue + Info.KeyValue->DataOffset);

                    Status2 = NtSetValueKey(copyKeys->hKeyDest,
                                            &ValueName,
                                            Info.KeyValue->TitleIndex,
                                            Info.KeyValue->Type,
                                            Data,
                                            Info.KeyValue->DataLength);

                    /* don't break, let's try to copy as many values as possible */
                    if (!NT_SUCCESS(Status2) && NT_SUCCESS(Status))
                    {
                        Status = Status2;
                    }

                    Index++;
                }
                else if (Status2 == STATUS_BUFFER_OVERFLOW)
                {
                    PVOID Buffer;

                    ASSERT(BufferSize < BufferSizeRequired);

                    Buffer = RtlReAllocateHeap(ProcessHeap,
                                               0,
                                               Info.Buffer,
                                               BufferSizeRequired);
                    if (Buffer != NULL)
                    {
                        Info.Buffer = Buffer;
                        /* try again */
                    }
                    else
                    {
                        /* don't break, let's try to copy as many values as possible */
                        Status2 = STATUS_INSUFFICIENT_RESOURCES;
                        Index++;

                        if (NT_SUCCESS(Status))
                        {
                            Status = Status2;
                        }
                    }
                }
                else
                {
                    /* break to avoid an infinite loop in case of denied access or
                       other errors! */
                    if (Status2 != STATUS_NO_MORE_ENTRIES && NT_SUCCESS(Status))
                    {
                        Status = Status2;
                    }

                    break;
                }
            }

            /* enumerate all subkeys and open and enqueue them */
            Index = 0;
            for (;;)
            {
                Status2 = NtEnumerateKey(copyKeys->hKeySrc,
                                         Index,
                                         KeyNodeInformation,
                                         Info.KeyNode,
                                         BufferSize,
                                         &BufferSizeRequired);
                if (NT_SUCCESS(Status2))
                {
                    HANDLE KeyHandle, NewKeyHandle;
                    OBJECT_ATTRIBUTES ObjectAttributes;
                    UNICODE_STRING SubKeyName, ClassName;

                    /* don't use RtlInitUnicodeString as the string is not NULL-terminated! */
                    SubKeyName.Length = Info.KeyNode->NameLength;
                    SubKeyName.MaximumLength = SubKeyName.Length;
                    SubKeyName.Buffer = Info.KeyNode->Name;
                    ClassName.Length = Info.KeyNode->ClassLength;
                    ClassName.MaximumLength = ClassName.Length;
                    ClassName.Buffer = (PWSTR)((ULONG_PTR)Info.KeyNode + Info.KeyNode->ClassOffset);

                    /* open the subkey with sufficient rights */

                    InitializeObjectAttributes(&ObjectAttributes,
                                               &SubKeyName,
                                               OBJ_CASE_INSENSITIVE,
                                               copyKeys->hKeySrc,
                                               NULL);

                    Status2 = NtOpenKey(&KeyHandle,
                                        KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE,
                                        &ObjectAttributes);
                    if (NT_SUCCESS(Status2))
                    {
                        /* FIXME - attempt to query the security information */

                        InitializeObjectAttributes(&ObjectAttributes,
                                               &SubKeyName,
                                               OBJ_CASE_INSENSITIVE,
                                               copyKeys->hKeyDest,
                                               NULL);

                        Status2 = NtCreateKey(&NewKeyHandle,
                                              KEY_ALL_ACCESS,
                                              &ObjectAttributes,
                                              Info.KeyNode->TitleIndex,
                                              &ClassName,
                                              0,
                                              NULL);
                        if (NT_SUCCESS(Status2))
                        {
                            newCopyKeys = RtlAllocateHeap(ProcessHeap,
                                                          0,
                                                          sizeof(REGP_COPY_KEYS));
                            if (newCopyKeys != NULL)
                            {
                                /* save the handles and enqueue the subkey */
                                newCopyKeys->hKeySrc = KeyHandle;
                                newCopyKeys->hKeyDest = NewKeyHandle;
                                InsertTailList(&copyQueueHead,
                                               &newCopyKeys->ListEntry);
                            }
                            else
                            {
                                NtClose(KeyHandle);
                                NtClose(NewKeyHandle);

                                Status2 = STATUS_INSUFFICIENT_RESOURCES;
                            }
                        }
                        else
                        {
                            NtClose(KeyHandle);
                        }
                    }

                    if (!NT_SUCCESS(Status2) && NT_SUCCESS(Status))
                    {
                        Status = Status2;
                    }

                    Index++;
                }
                else if (Status2 == STATUS_BUFFER_OVERFLOW)
                {
                    PVOID Buffer;

                    ASSERT(BufferSize < BufferSizeRequired);

                    Buffer = RtlReAllocateHeap(ProcessHeap,
                                               0,
                                               Info.Buffer,
                                               BufferSizeRequired);
                    if (Buffer != NULL)
                    {
                        Info.Buffer = Buffer;
                        /* try again */
                    }
                    else
                    {
                        /* don't break, let's try to copy as many keys as possible */
                        Status2 = STATUS_INSUFFICIENT_RESOURCES;
                        Index++;

                        if (NT_SUCCESS(Status))
                        {
                            Status = Status2;
                        }
                    }
                }
                else
                {
                    /* break to avoid an infinite loop in case of denied access or
                       other errors! */
                    if (Status2 != STATUS_NO_MORE_ENTRIES && NT_SUCCESS(Status))
                    {
                        Status = Status2;
                    }

                    break;
                }
            }

            /* close the handles and remove the entry from the list */
            if (copyKeys->hKeySrc != hKeySrc)
            {
                NtClose(copyKeys->hKeySrc);
            }
            if (copyKeys->hKeyDest != hKeyDest)
            {
                NtClose(copyKeys->hKeyDest);
            }

            RemoveEntryList(&copyKeys->ListEntry);

            RtlFreeHeap(ProcessHeap,
                        0,
                        copyKeys);
        } while (!IsListEmpty(&copyQueueHead));
    }
    else
        Status = STATUS_INSUFFICIENT_RESOURCES;

    RtlFreeHeap(ProcessHeap,
                0,
                Info.Buffer);

    return Status;
}

/************************************************************************
 *  RegCopyTreeW
 *
 * @implemented
 */
LONG WINAPI
RegCopyTreeW(IN HKEY hKeySrc,
             IN LPCWSTR lpSubKey  OPTIONAL,
             IN HKEY hKeyDest)
{
    HANDLE DestKeyHandle, KeyHandle, CurKey, SubKeyHandle = NULL;
    NTSTATUS Status;

    Status = MapDefaultKey(&KeyHandle,
                           hKeySrc);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    Status = MapDefaultKey(&DestKeyHandle,
                           hKeyDest);
    if (!NT_SUCCESS(Status))
    {
        goto Cleanup2;
    }

    if (lpSubKey != NULL)
    {
        OBJECT_ATTRIBUTES ObjectAttributes;
        UNICODE_STRING SubKeyName;

        RtlInitUnicodeString(&SubKeyName, lpSubKey);

        InitializeObjectAttributes(&ObjectAttributes,
                                   &SubKeyName,
                                   OBJ_CASE_INSENSITIVE,
                                   KeyHandle,
                                   NULL);

        Status = NtOpenKey(&SubKeyHandle,
                           KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE,
                           &ObjectAttributes);
        if (!NT_SUCCESS(Status))
        {
            goto Cleanup;
        }

        CurKey = SubKeyHandle;
    }
    else
        CurKey = KeyHandle;

    Status = RegpCopyTree(CurKey,
                          hKeyDest);

    if (SubKeyHandle != NULL)
    {
        NtClose(SubKeyHandle);
    }

Cleanup:
    ClosePredefKey(DestKeyHandle);
Cleanup2:
    ClosePredefKey(KeyHandle);

    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    return ERROR_SUCCESS;
}


/************************************************************************
 *  RegCopyTreeA
 *
 * @implemented
 */
LONG WINAPI
RegCopyTreeA(IN HKEY hKeySrc,
             IN LPCSTR lpSubKey  OPTIONAL,
             IN HKEY hKeyDest)
{
    UNICODE_STRING SubKeyName = { 0, 0, NULL };
    LONG Ret;

    if (lpSubKey != NULL &&
        !RtlCreateUnicodeStringFromAsciiz(&SubKeyName, lpSubKey))
    {
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    Ret = RegCopyTreeW(hKeySrc,
                       SubKeyName.Buffer,
                       hKeyDest);

    RtlFreeUnicodeString(&SubKeyName);

    return Ret;
}

/************************************************************************
 *  RegDeleteKeyValueW
 *
 * @implemented
 */
LONG WINAPI
RegDeleteKeyValueW(IN HKEY hKey,
                   IN LPCWSTR lpSubKey  OPTIONAL,
                   IN LPCWSTR lpValueName  OPTIONAL)
{
    UNICODE_STRING ValueName;
    HANDLE KeyHandle, CurKey, SubKeyHandle = NULL;
    NTSTATUS Status;

    Status = MapDefaultKey(&KeyHandle,
                           hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    if (lpSubKey != NULL)
    {
        OBJECT_ATTRIBUTES ObjectAttributes;
        UNICODE_STRING SubKeyName;

        RtlInitUnicodeString(&SubKeyName, lpSubKey);

        InitializeObjectAttributes(&ObjectAttributes,
                                   &SubKeyName,
                                   OBJ_CASE_INSENSITIVE,
                                   KeyHandle,
                                   NULL);

        Status = NtOpenKey(&SubKeyHandle,
                           KEY_SET_VALUE,
                           &ObjectAttributes);
        if (!NT_SUCCESS(Status))
        {
            goto Cleanup;
        }

        CurKey = SubKeyHandle;
    }
    else
        CurKey = KeyHandle;

    RtlInitUnicodeString(&ValueName, lpValueName);

    Status = NtDeleteValueKey(CurKey,
                              &ValueName);

    if (SubKeyHandle != NULL)
    {
        NtClose(SubKeyHandle);
    }

Cleanup:
    ClosePredefKey(KeyHandle);

    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    return ERROR_SUCCESS;
}


/************************************************************************
 *  RegDeleteKeyValueA
 *
 * @implemented
 */
LONG WINAPI
RegDeleteKeyValueA(IN HKEY hKey,
                   IN LPCSTR lpSubKey  OPTIONAL,
                   IN LPCSTR lpValueName  OPTIONAL)
{
    UNICODE_STRING SubKey = { 0, 0, NULL }, ValueName = { 0, 0, NULL };
    LONG Ret;

    if (lpSubKey != NULL &&
        !RtlCreateUnicodeStringFromAsciiz(&SubKey, lpSubKey))
    {
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    if (lpValueName != NULL &&
        !RtlCreateUnicodeStringFromAsciiz(&ValueName, lpValueName))
    {
        RtlFreeUnicodeString(&SubKey);
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    Ret = RegDeleteKeyValueW(hKey,
                             SubKey.Buffer,
                             SubKey.Buffer);

    RtlFreeUnicodeString(&SubKey);
    RtlFreeUnicodeString(&ValueName);

    return Ret;
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

/************************************************************************
 *  RegSetKeyValueW
 *
 * @implemented
 */
LONG WINAPI
RegSetKeyValueW(IN HKEY hKey,
                IN LPCWSTR lpSubKey  OPTIONAL,
                IN LPCWSTR lpValueName  OPTIONAL,
                IN DWORD dwType,
                IN LPCVOID lpData  OPTIONAL,
                IN DWORD cbData)
{
    HANDLE KeyHandle, CurKey, SubKeyHandle = NULL;
    NTSTATUS Status;
    LONG Ret;

    Status = MapDefaultKey(&KeyHandle,
                           hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    if (lpSubKey != NULL)
    {
        OBJECT_ATTRIBUTES ObjectAttributes;
        UNICODE_STRING SubKeyName;

        RtlInitUnicodeString(&SubKeyName, lpSubKey);

        InitializeObjectAttributes(&ObjectAttributes,
                                   &SubKeyName,
                                   OBJ_CASE_INSENSITIVE,
                                   KeyHandle,
                                   NULL);

        Status = NtOpenKey(&SubKeyHandle,
                           KEY_SET_VALUE,
                           &ObjectAttributes);
        if (!NT_SUCCESS(Status))
        {
            Ret = RtlNtStatusToDosError(Status);
            goto Cleanup;
        }

        CurKey = SubKeyHandle;
    }
    else
        CurKey = KeyHandle;

    Ret = RegSetValueExW(CurKey,
                         lpValueName,
                         0,
                         dwType,
                         lpData,
                         cbData);

    if (SubKeyHandle != NULL)
    {
        NtClose(SubKeyHandle);
    }

Cleanup:
    ClosePredefKey(KeyHandle);

    return Ret;
}


/************************************************************************
 *  RegSetKeyValueA
 *
 * @implemented
 */
LONG WINAPI
RegSetKeyValueA(IN HKEY hKey,
                IN LPCSTR lpSubKey  OPTIONAL,
                IN LPCSTR lpValueName  OPTIONAL,
                IN DWORD dwType,
                IN LPCVOID lpData  OPTIONAL,
                IN DWORD cbData)
{
    HANDLE KeyHandle, CurKey, SubKeyHandle = NULL;
    NTSTATUS Status;
    LONG Ret;

    Status = MapDefaultKey(&KeyHandle,
                           hKey);
    if (!NT_SUCCESS(Status))
    {
        return RtlNtStatusToDosError(Status);
    }

    if (lpSubKey != NULL)
    {
        OBJECT_ATTRIBUTES ObjectAttributes;
        UNICODE_STRING SubKeyName;

        if (!RtlCreateUnicodeStringFromAsciiz(&SubKeyName, lpSubKey))
        {
            Ret = ERROR_NOT_ENOUGH_MEMORY;
            goto Cleanup;
        }

        InitializeObjectAttributes(&ObjectAttributes,
                                   &SubKeyName,
                                   OBJ_CASE_INSENSITIVE,
                                   KeyHandle,
                                   NULL);

        Status = NtOpenKey(&SubKeyHandle,
                           KEY_SET_VALUE,
                           &ObjectAttributes);

        RtlFreeUnicodeString(&SubKeyName);

        if (!NT_SUCCESS(Status))
        {
            Ret = RtlNtStatusToDosError(Status);
            goto Cleanup;
        }

        CurKey = SubKeyHandle;
    }
    else
        CurKey = KeyHandle;

    Ret = RegSetValueExA(CurKey,
                         lpValueName,
                         0,
                         dwType,
                         lpData,
                         cbData);

    if (SubKeyHandle != NULL)
    {
        NtClose(SubKeyHandle);
    }

Cleanup:
    ClosePredefKey(KeyHandle);

    return Ret;
}

/* heap allocation helpers */
static void *heap_alloc( size_t len ) __WINE_ALLOC_SIZE(1);
static inline void *heap_alloc( size_t len )
{
    return HeapAlloc( GetProcessHeap(), 0, len );
}

static inline BOOL heap_free( void *mem )
{
    return HeapFree( GetProcessHeap(), 0, mem );
}

/* Taken from Wine advapi32/registry.c */

/******************************************************************************
 * RegDeleteTreeW [ADVAPI32.@]
 *
 */
LSTATUS WINAPI RegDeleteTreeW(HKEY hKey, LPCWSTR lpszSubKey)
{
    LONG ret;
    DWORD dwMaxSubkeyLen, dwMaxValueLen;
    DWORD dwMaxLen, dwSize;
    WCHAR szNameBuf[MAX_PATH], *lpszName = szNameBuf;
    HKEY hSubKey = hKey;

    if(lpszSubKey)
    {
        ret = RegOpenKeyExW(hKey, lpszSubKey, 0, KEY_READ, &hSubKey);
        if (ret) return ret;
    }

    /* Get highest length for keys, values */
    ret = RegQueryInfoKeyW(hSubKey, NULL, NULL, NULL, NULL,
            &dwMaxSubkeyLen, NULL, NULL, &dwMaxValueLen, NULL, NULL, NULL);
    if (ret) goto cleanup;

    dwMaxSubkeyLen++;
    dwMaxValueLen++;
    dwMaxLen = max(dwMaxSubkeyLen, dwMaxValueLen);
    if (dwMaxLen > sizeof(szNameBuf)/sizeof(WCHAR))
    {
        /* Name too big: alloc a buffer for it */
        if (!(lpszName = heap_alloc( dwMaxLen*sizeof(WCHAR))))
        {
            ret = ERROR_NOT_ENOUGH_MEMORY;
            goto cleanup;
        }
    }


    /* Recursively delete all the subkeys */
    while (TRUE)
    {
        dwSize = dwMaxLen;
        if (RegEnumKeyExW(hSubKey, 0, lpszName, &dwSize, NULL,
                          NULL, NULL, NULL)) break;

        ret = RegDeleteTreeW(hSubKey, lpszName);
        if (ret) goto cleanup;
    }

    if (lpszSubKey)
        ret = RegDeleteKeyW(hKey, lpszSubKey);
    else
        while (TRUE)
        {
            dwSize = dwMaxLen;
            if (RegEnumValueW(hKey, 0, lpszName, &dwSize,
                  NULL, NULL, NULL, NULL)) break;

            ret = RegDeleteValueW(hKey, lpszName);
            if (ret) goto cleanup;
        }

cleanup:
    /* Free buffer if allocated */
    if (lpszName != szNameBuf)
        heap_free( lpszName);
    if(lpszSubKey)
        RegCloseKey(hSubKey);
    return ret;
}

/******************************************************************************
 * RegDeleteTreeA [ADVAPI32.@]
 *
 */
LSTATUS WINAPI RegDeleteTreeA(HKEY hKey, LPCSTR lpszSubKey)
{
    LONG ret;
    UNICODE_STRING lpszSubKeyW;

    if (lpszSubKey) RtlCreateUnicodeStringFromAsciiz( &lpszSubKeyW, lpszSubKey);
    else lpszSubKeyW.Buffer = NULL;
    ret = RegDeleteTreeW( hKey, lpszSubKeyW.Buffer);
    RtlFreeUnicodeString( &lpszSubKeyW );
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