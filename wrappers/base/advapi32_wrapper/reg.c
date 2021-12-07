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

typedef LONG (WINAPI *REG_DELETE_KEY_EX_W)(
    HKEY,
	LPCWSTR,
	REGSAM,
	DWORD);
	
typedef LONG (WINAPI *REG_DELETE_KEY_EX_A)(
    HKEY,
	LPCSTR,
	REGSAM,
	DWORD);	
	
typedef LONG (WINAPI *REG_DISABLE_REFLECTION_KEY)(
    HKEY);

typedef LONG (WINAPI *REG_ENABLE_REFLECTION_KEY)(
    HKEY);		

typedef LONG (WINAPI *REG_QUERY_REFLECTION_KEY)(
	HKEY,
	BOOL*);	

typedef LONG (WINAPI *REG_CONNECT_REGISTRY_EX_A)(
    LPCSTR,
	HKEY,
	ULONG,
	PHKEY);	
	
typedef LONG (WINAPI *REG_CONNECT_REGISTRY_EX_W)(
    LPCWSTR,
	HKEY,
	ULONG,
	PHKEY);

typedef LONG (WINAPI *REG_DISABLE_PREDEFINED_CACHE_EX)(
    VOID);		

static const UNICODE_STRING HKLM_ClassesPath = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\Software\\Classes");

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

/******************************************************************************
 * RegRenameKey [ADVAPI32.@]
 *
 */
LSTATUS WINAPI RegRenameKey( HKEY hkey, LPCWSTR lpSubKey, LPCWSTR lpNewName )
{
    FIXME("(%p,%s,%s): stub\n", hkey, debugstr_w(lpSubKey), debugstr_w(lpNewName));
    return ERROR_CALL_NOT_IMPLEMENTED;
}

/******************************************************************************
 * RegLoadAppKeyA (kernelbase.@) - unimplemented
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
 * RegLoadAppKeyW (kernelbase.@) - unimplemented
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

LSTATUS RegOpenKeyTransactedA(
  HKEY   hKey,
  LPCSTR lpSubKey,
  DWORD  ulOptions,
  REGSAM samDesired,
  PHKEY  phkResult,
  HANDLE hTransaction,
  PVOID  pExtendedParemeter
)
{
	if(pExtendedParemeter){
		return STATUS_INVALID_PARAMETER;
	}
	
	return RegOpenKeyExA(hKey,
						 lpSubKey,
						 ulOptions,
						 samDesired,
						 phkResult);
}

LSTATUS 
RegOpenKeyTransactedW(
  HKEY   hKey,
  LPCWSTR lpSubKey,
  DWORD  ulOptions,
  REGSAM samDesired,
  PHKEY  phkResult,
  HANDLE hTransaction,
  PVOID  pExtendedParemeter
)
{
	if(pExtendedParemeter){
		return STATUS_INVALID_PARAMETER;
	}
	
	return RegOpenKeyExW(hKey,
						 lpSubKey,
						 ulOptions,
						 samDesired,
						 phkResult);
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
	REG_DELETE_KEY_EX_W regDeleteKeyExW;

    regDeleteKeyExW = (REG_DELETE_KEY_EX_W) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase.dll")),
                            "RegDeleteKeyExW");	
	if(regDeleteKeyExW == NULL){
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
	}else{
		return regDeleteKeyExW(hKey,
							   lpSubKey,
							   samDesired,
							   Reserved);
	}
}

/************************************************************************
 *  RegDeleteKeyExA
 *
 * @implemented
 */
LONG
WINAPI
RegDeleteKeyExA(
    _In_ HKEY hKey,
    _In_ LPCSTR lpSubKey,
    _In_ REGSAM samDesired,
    _In_ DWORD Reserved)
{
    LONG ErrorCode;
    UNICODE_STRING SubKeyName;
	REG_DELETE_KEY_EX_A regDeleteKeyExA;

    regDeleteKeyExA = (REG_DELETE_KEY_EX_A) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase.dll")),
                            "RegDeleteKeyExA");		

	if(regDeleteKeyExA == NULL){
		if (lpSubKey)
		{
			if (!RtlCreateUnicodeStringFromAsciiz(&SubKeyName, lpSubKey))
				return ERROR_NOT_ENOUGH_MEMORY;
		}
		else
			RtlInitEmptyUnicodeString(&SubKeyName, NULL, 0);

		ErrorCode = RegDeleteKeyExW(hKey, SubKeyName.Buffer, samDesired, Reserved);

		RtlFreeUnicodeString(&SubKeyName);

		return ErrorCode;	
	}else{
		return regDeleteKeyExA(hKey,
							   lpSubKey,
							   samDesired,
							   Reserved);
	}
}

/************************************************************************
 *  RegDisableReflectionKey
 *
 * @unimplemented
 */
LONG WINAPI
RegDisableReflectionKey(IN HKEY hBase)
{
	REG_DISABLE_REFLECTION_KEY regDisableReflectionKey;
    regDisableReflectionKey = (REG_DISABLE_REFLECTION_KEY) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase.dll")),
                            "RegDisableReflectionKey");	
	if(regDisableReflectionKey == NULL){
		FIXME("RegDisableReflectionKey(0x%p) UNIMPLEMENTED!\n", hBase);
		return ERROR_CALL_NOT_IMPLEMENTED;
	}else{
		return regDisableReflectionKey(hBase);
	}    	
}


/************************************************************************
 *  RegEnableReflectionKey
 *
 * @unimplemented
 */
LONG WINAPI
RegEnableReflectionKey(IN HKEY hBase)
{
	REG_ENABLE_REFLECTION_KEY regEnableReflectionKey;
    regEnableReflectionKey = (REG_ENABLE_REFLECTION_KEY) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase.dll")),
                            "RegEnableReflectionKey");	
	if(regEnableReflectionKey == NULL){
		FIXME("RegEnableReflectionKey(0x%p) UNIMPLEMENTED!\n", hBase);
		return ERROR_CALL_NOT_IMPLEMENTED;
	}else{
		return regEnableReflectionKey(hBase);
	} 
}

LONG 
WINAPI 
RegConnectRegistryExA(
	LPCSTR lpMachineName,
	HKEY hKey,
	ULONG Flags,
	PHKEY phkResult
)
{
	REG_CONNECT_REGISTRY_EX_A regConnectRegistryExA;
    regConnectRegistryExA = (REG_CONNECT_REGISTRY_EX_A) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase.dll")),
                            "RegConnectRegistryExA");	
	if(regConnectRegistryExA == NULL){
		FIXME("RegConnectRegistryExA UNIMPLEMENTED!\n");
		return ERROR_CALL_NOT_IMPLEMENTED;
	}else{
		return regConnectRegistryExA(lpMachineName,
									 hKey,
									 Flags,
									 phkResult);
	} 	
}

LONG 
WINAPI 
RegConnectRegistryExW(
	LPCWSTR lpMachineName,
	HKEY hKey,
	ULONG Flags,
	PHKEY phkResult
)
{
	REG_CONNECT_REGISTRY_EX_W regConnectRegistryExW;
    regConnectRegistryExW = (REG_CONNECT_REGISTRY_EX_W) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase.dll")),
                            "RegConnectRegistryExW");	
	if(regConnectRegistryExW == NULL){
		FIXME("RegConnectRegistryExW UNIMPLEMENTED!\n");
		return ERROR_CALL_NOT_IMPLEMENTED;
	}else{
		return regConnectRegistryExW(lpMachineName,
									 hKey,
									 Flags,
									 phkResult);
	} 	
}


/******************************************************************************
 * RegpApplyRestrictions   [internal]
 *
 * Helper function for RegGetValueA/W.
 */
static VOID
RegpApplyRestrictions(DWORD dwFlags,
                      DWORD dwType,
                      DWORD cbData,
                      PLONG ret)
{
    /* Check if the type is restricted by the passed flags */
    if (*ret == ERROR_SUCCESS || *ret == ERROR_MORE_DATA)
    {
        DWORD dwMask = 0;

        switch (dwType)
        {
        case REG_NONE: dwMask = RRF_RT_REG_NONE; break;
        case REG_SZ: dwMask = RRF_RT_REG_SZ; break;
        case REG_EXPAND_SZ: dwMask = RRF_RT_REG_EXPAND_SZ; break;
        case REG_MULTI_SZ: dwMask = RRF_RT_REG_MULTI_SZ; break;
        case REG_BINARY: dwMask = RRF_RT_REG_BINARY; break;
        case REG_DWORD: dwMask = RRF_RT_REG_DWORD; break;
        case REG_QWORD: dwMask = RRF_RT_REG_QWORD; break;
        }

        if (dwFlags & dwMask)
        {
            /* Type is not restricted, check for size mismatch */
            if (dwType == REG_BINARY)
            {
                DWORD cbExpect = 0;

                if ((dwFlags & RRF_RT_ANY) == RRF_RT_DWORD)
                    cbExpect = 4;
                else if ((dwFlags & RRF_RT_ANY) == RRF_RT_QWORD)
                    cbExpect = 8;

                if (cbExpect && cbData != cbExpect)
                    *ret = ERROR_DATATYPE_MISMATCH;
            }
        }
        else *ret = ERROR_UNSUPPORTED_TYPE;
    }
}

/******************************************************************************
 * RegGetValueW   [ADVAPI32.@]
 *
 * Retrieves the type and data for a value name associated with a key,
 * optionally expanding its content and restricting its type.
 *
 * PARAMS
 *  hKey      [I] Handle to an open key.
 *  pszSubKey [I] Name of the subkey of hKey.
 *  pszValue  [I] Name of value under hKey/szSubKey to query.
 *  dwFlags   [I] Flags restricting the value type to retrieve.
 *  pdwType   [O] Destination for the values type, may be NULL.
 *  pvData    [O] Destination for the values content, may be NULL.
 *  pcbData   [I/O] Size of pvData, updated with the size in bytes required to
 *                  retrieve the whole content, including the trailing '\0'
 *                  for strings.
 *
 * RETURNS
 *  Success: ERROR_SUCCESS
 *  Failure: nonzero error code from Winerror.h
 *
 * NOTES
 *  - Unless RRF_NOEXPAND is specified, REG_EXPAND_SZ values are automatically
 *    expanded and pdwType is set to REG_SZ instead.
 *  - Restrictions are applied after expanding, using RRF_RT_REG_EXPAND_SZ
 *    without RRF_NOEXPAND is thus not allowed.
 *    An exception is the case where RRF_RT_ANY is specified, because then
 *    RRF_NOEXPAND is allowed.
 */
LSTATUS WINAPI
RegGetValueW(HKEY hKey,
             LPCWSTR pszSubKey,
             LPCWSTR pszValue,
             DWORD dwFlags,
             LPDWORD pdwType,
             PVOID pvData,
             LPDWORD pcbData)
{
    DWORD dwType, cbData = pcbData ? *pcbData : 0;
    PVOID pvBuf = NULL;
    LONG ret;

    TRACE("(%p,%s,%s,%ld,%p,%p,%p=%ld)\n",
          hKey, debugstr_w(pszSubKey), debugstr_w(pszValue), dwFlags, pdwType,
          pvData, pcbData, cbData);

    if (pvData && !pcbData)
        return ERROR_INVALID_PARAMETER;
    if ((dwFlags & RRF_RT_REG_EXPAND_SZ) && !(dwFlags & RRF_NOEXPAND) &&
            ((dwFlags & RRF_RT_ANY) != RRF_RT_ANY))
        return ERROR_INVALID_PARAMETER;

    if (pszSubKey && pszSubKey[0])
    {
        ret = RegOpenKeyExW(hKey, pszSubKey, 0, KEY_QUERY_VALUE, &hKey);
        if (ret != ERROR_SUCCESS) return ret;
    }

    ret = RegQueryValueExW(hKey, pszValue, NULL, &dwType, pvData, &cbData);

    /* If we are going to expand we need to read in the whole the value even
     * if the passed buffer was too small as the expanded string might be
     * smaller than the unexpanded one and could fit into cbData bytes. */
    if ((ret == ERROR_SUCCESS || ret == ERROR_MORE_DATA) &&
        dwType == REG_EXPAND_SZ && !(dwFlags & RRF_NOEXPAND))
    {
        do
        {
            HeapFree(GetProcessHeap(), 0, pvBuf);

            pvBuf = HeapAlloc(GetProcessHeap(), 0, cbData);
            if (!pvBuf)
            {
                ret = ERROR_NOT_ENOUGH_MEMORY;
                break;
            }

            if (ret == ERROR_MORE_DATA || !pvData)
                ret = RegQueryValueExW(hKey, pszValue, NULL,
                                       &dwType, pvBuf, &cbData);
            else
            {
                /* Even if cbData was large enough we have to copy the
                 * string since ExpandEnvironmentStrings can't handle
                 * overlapping buffers. */
                CopyMemory(pvBuf, pvData, cbData);
            }

            /* Both the type or the value itself could have been modified in
             * between so we have to keep retrying until the buffer is large
             * enough or we no longer have to expand the value. */
        }
        while (dwType == REG_EXPAND_SZ && ret == ERROR_MORE_DATA);

        if (ret == ERROR_SUCCESS)
        {
            /* Recheck dwType in case it changed since the first call */
            if (dwType == REG_EXPAND_SZ)
            {
                cbData = ExpandEnvironmentStringsW(pvBuf, pvData,
                                                   pcbData ? *pcbData : 0) * sizeof(WCHAR);
                dwType = REG_SZ;
                if (pvData && pcbData && cbData > *pcbData)
                    ret = ERROR_MORE_DATA;
            }
            else if (pvData)
                CopyMemory(pvData, pvBuf, *pcbData);
        }

        HeapFree(GetProcessHeap(), 0, pvBuf);
    }

    if (pszSubKey && pszSubKey[0])
        RegCloseKey(hKey);

    RegpApplyRestrictions(dwFlags, dwType, cbData, &ret);

    if (pvData && ret != ERROR_SUCCESS && (dwFlags & RRF_ZEROONFAILURE))
        ZeroMemory(pvData, *pcbData);

    if (pdwType)
        *pdwType = dwType;

    if (pcbData)
        *pcbData = cbData;

    return ret;
}


/******************************************************************************
 * RegGetValueA   [ADVAPI32.@]
 *
 * See RegGetValueW.
 */
LSTATUS WINAPI
RegGetValueA(HKEY hKey,
             LPCSTR pszSubKey,
             LPCSTR pszValue,
             DWORD dwFlags,
             LPDWORD pdwType,
             PVOID pvData,
             LPDWORD pcbData)
{
    DWORD dwType, cbData = pcbData ? *pcbData : 0;
    PVOID pvBuf = NULL;
    LONG ret;

    TRACE("(%p,%s,%s,%ld,%p,%p,%p=%ld)\n",
          hKey, pszSubKey, pszValue, dwFlags, pdwType, pvData, pcbData,
          cbData);

    if (pvData && !pcbData)
        return ERROR_INVALID_PARAMETER;
    if ((dwFlags & RRF_RT_REG_EXPAND_SZ) && !(dwFlags & RRF_NOEXPAND) &&
            ((dwFlags & RRF_RT_ANY) != RRF_RT_ANY))
        return ERROR_INVALID_PARAMETER;

    if (pszSubKey && pszSubKey[0])
    {
        ret = RegOpenKeyExA(hKey, pszSubKey, 0, KEY_QUERY_VALUE, &hKey);
        if (ret != ERROR_SUCCESS) return ret;
    }

    ret = RegQueryValueExA(hKey, pszValue, NULL, &dwType, pvData, &cbData);

    /* If we are going to expand we need to read in the whole the value even
     * if the passed buffer was too small as the expanded string might be
     * smaller than the unexpanded one and could fit into cbData bytes. */
    if ((ret == ERROR_SUCCESS || ret == ERROR_MORE_DATA) &&
        (dwType == REG_EXPAND_SZ && !(dwFlags & RRF_NOEXPAND)))
    {
        do {
            HeapFree(GetProcessHeap(), 0, pvBuf);

            pvBuf = HeapAlloc(GetProcessHeap(), 0, cbData);
            if (!pvBuf)
            {
                ret = ERROR_NOT_ENOUGH_MEMORY;
                break;
            }

            if (ret == ERROR_MORE_DATA || !pvData)
                ret = RegQueryValueExA(hKey, pszValue, NULL,
                                       &dwType, pvBuf, &cbData);
            else
            {
                /* Even if cbData was large enough we have to copy the
                 * string since ExpandEnvironmentStrings can't handle
                 * overlapping buffers. */
                CopyMemory(pvBuf, pvData, cbData);
            }

            /* Both the type or the value itself could have been modified in
             * between so we have to keep retrying until the buffer is large
             * enough or we no longer have to expand the value. */
        } while (dwType == REG_EXPAND_SZ && ret == ERROR_MORE_DATA);

        if (ret == ERROR_SUCCESS)
        {
            /* Recheck dwType in case it changed since the first call */
            if (dwType == REG_EXPAND_SZ)
            {
                cbData = ExpandEnvironmentStringsA(pvBuf, pvData,
                                                   pcbData ? *pcbData : 0);
                dwType = REG_SZ;
                if(pvData && pcbData && cbData > *pcbData)
                    ret = ERROR_MORE_DATA;
            }
            else if (pvData)
                CopyMemory(pvData, pvBuf, *pcbData);
        }

        HeapFree(GetProcessHeap(), 0, pvBuf);
    }

    if (pszSubKey && pszSubKey[0])
        RegCloseKey(hKey);

    RegpApplyRestrictions(dwFlags, dwType, cbData, &ret);

    if (pvData && ret != ERROR_SUCCESS && (dwFlags & RRF_ZEROONFAILURE))
        ZeroMemory(pvData, *pcbData);

    if (pdwType) *pdwType = dwType;
    if (pcbData) *pcbData = cbData;

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

/************************************************************************
 *  RegQueryReflectionKey
 *
 * @unimplemented
 */
LONG WINAPI
RegQueryReflectionKey(IN HKEY hBase,
                      OUT BOOL* bIsReflectionDisabled)
{
	REG_QUERY_REFLECTION_KEY regQueryReflectionKey;
    regQueryReflectionKey = (REG_QUERY_REFLECTION_KEY) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase.dll")),
                            "RegQueryReflectionKey");	
	if(regQueryReflectionKey == NULL){
		FIXME("RegQueryReflectionKey(0x%p, 0x%p) UNIMPLEMENTED!\n",
			  hBase, bIsReflectionDisabled);
		return ERROR_CALL_NOT_IMPLEMENTED;
	}else{
		return regQueryReflectionKey(hBase,
									 bIsReflectionDisabled);
	}
}

LSTATUS RegCreateKeyTransactedA(
  HKEY                        hKey,
  LPCSTR                      lpSubKey,
  DWORD                       Reserved,
  LPSTR                       lpClass,
  DWORD                       dwOptions,
  REGSAM                      samDesired,
  const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  PHKEY                       phkResult,
  LPDWORD                     lpdwDisposition,
  HANDLE                      hTransaction,
  PVOID                       pExtendedParemeter
)
{
	if(pExtendedParemeter){
		return STATUS_INVALID_PARAMETER;
	}
	
	return RegCreateKeyExA(hKey,
						   lpSubKey,
						   Reserved,
						   lpClass,
						   dwOptions,
						   samDesired,
						   lpSecurityAttributes,
						   phkResult,
						   lpdwDisposition);
}

LSTATUS RegCreateKeyTransactedW(
  HKEY                        hKey,
  LPCWSTR                     lpSubKey,
  DWORD                       Reserved,
  LPWSTR                      lpClass,
  DWORD                       dwOptions,
  REGSAM                      samDesired,
  const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  PHKEY                       phkResult,
  LPDWORD                     lpdwDisposition,
  HANDLE                      hTransaction,
  PVOID                       pExtendedParemeter
)
{
	if(pExtendedParemeter){
		return STATUS_INVALID_PARAMETER;
	}
	
	return RegCreateKeyExW(hKey,
						   lpSubKey,
						   Reserved,
						   lpClass,
						   dwOptions,
						   samDesired,
						   lpSecurityAttributes,
						   phkResult,
						   lpdwDisposition);
}

LSTATUS RegDeleteKeyTransactedA(
  HKEY   hKey,
  LPCSTR lpSubKey,
  REGSAM samDesired,
  DWORD  Reserved,
  HANDLE hTransaction,
  PVOID  pExtendedParameter
)
{
	if(pExtendedParameter){
		return STATUS_INVALID_PARAMETER;
	}

	return RegDeleteKeyExA(hKey,
						   lpSubKey,
						   samDesired,
						   Reserved);
}

LSTATUS RegDeleteKeyTransactedW(
  HKEY   hKey,
  LPCWSTR lpSubKey,
  REGSAM samDesired,
  DWORD  Reserved,
  HANDLE hTransaction,
  PVOID  pExtendedParameter
)
{
	if(pExtendedParameter){
		return STATUS_INVALID_PARAMETER;
	}

	return RegDeleteKeyExW(hKey,
						   lpSubKey,
						   samDesired,
						   Reserved);
}