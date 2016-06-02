/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
 *
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

/* INCLUDES ******************************************************************/

/* C Headers */
#include <rtl.h>
#include <winsvc.h>
#include <evntprov.h>
#include <wine/debug.h>
#include <ntdllnew.h>
#include <ndk/cmfuncs.h>

/* DEFINES ******************************************************************/
#define SHUTDOWN_GRACE_OVERRIDE 0x00000020
#define SHUTDOWN_RESTART 0x00000004
#define MAX_DEFAULT_HANDLES   6
#define REG_MAX_NAME_SIZE     256
#define REG_MAX_DATA_SIZE     2048
#define ClosePredefKey(Handle)                                                 \
    if ((ULONG_PTR)Handle & 0x1) {                                             \
        NtClose(Handle);                                                       \
    }
#define IsPredefKey(HKey)                                                      \
    (((ULONG_PTR)(HKey) & 0xF0000000) == 0x80000000)
#define GetPredefKeyIndex(HKey)                                                \
    ((ULONG_PTR)(HKey) & 0x0FFFFFFF)

/* PROTOTYPES ***************************************************************/
	
static NTSTATUS MapDefaultKey (PHANDLE ParentKey, HKEY Key);
//static VOID CloseDefaultKeys(VOID);
static NTSTATUS OpenClassesRootKey(PHANDLE KeyHandle);
static NTSTATUS OpenLocalMachineKey (PHANDLE KeyHandle);
static NTSTATUS OpenUsersKey (PHANDLE KeyHandle);
static NTSTATUS OpenCurrentConfigKey(PHANDLE KeyHandle);

/* GLOBALS ******************************************************************/

static RTL_CRITICAL_SECTION HandleTableCS;
static HANDLE DefaultHandleTable[MAX_DEFAULT_HANDLES];
static HANDLE ProcessHeap;
static BOOLEAN DefaultHandlesDisabled = FALSE;
static BOOLEAN DefaultHandleHKUDisabled = FALSE;
static BOOLEAN DllInitialized = FALSE; /* HACK */

typedef VOID( CALLBACK * PFN_SC_NOTIFY_CALLBACK ) (
    IN PVOID pParameter 
);

typedef struct _SERVICE_NOTIFY {
  DWORD                  dwVersion;
  PFN_SC_NOTIFY_CALLBACK pfnNotifyCallback;
  PVOID                  pContext;
  DWORD                  dwNotificationStatus;
  SERVICE_STATUS_PROCESS ServiceStatus;
  DWORD                  dwNotificationTriggered;
  LPTSTR                 pszServiceNames;
} SERVICE_NOTIFY, *PSERVICE_NOTIFY;
	
WINE_DEFAULT_DEBUG_CHANNEL(advapi32);

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    TRACE("fdwReason %u\n", fdwReason);

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            //DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}

/*INTERNAL FUNCTIONS ********************************************/

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
/*
static VOID
CloseDefaultKeys(VOID)
{
    ULONG i;
    RegInitialize();
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
}*/


static NTSTATUS
OpenClassesRootKey(PHANDLE KeyHandle)
{
    OBJECT_ATTRIBUTES Attributes;
    UNICODE_STRING KeyName = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\Software\\CLASSES");

    TRACE("OpenClassesRootKey()\n");

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

/* unimplemented*/
ULONG WINAPI EventRegister(
  _In_      LPCGUID ProviderId,
  _In_opt_  PENABLECALLBACK EnableCallback,
  _In_opt_  PVOID CallbackContext,
  _Out_     PREGHANDLE RegHandle
)
{
	return ERROR_SUCCESS;
	//return EtwEventRegister(ProviderId, EnableCallback, CallbackContext, RegHandle);
}

/* unimplemented*/
ULONG WINAPI EventUnregister(
  _In_  REGHANDLE RegHandle
)
{
	//return EtwEventUnregister(RegHandle);
	return ERROR_SUCCESS;
}

/* unimplemented*/
ULONG WINAPI EventWrite(
  _In_      REGHANDLE RegHandle,
  _In_      PCEVENT_DESCRIPTOR EventDescriptor,
  _In_      ULONG UserDataCount,
  _In_opt_  PEVENT_DATA_DESCRIPTOR UserData
)
{
	//return EtwEventWrite(RegHandle, EventDescriptor, UserDataCount, UserData);
	return ERROR_SUCCESS;
}

ULONG WINAPI
EventWriteEndScenario(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return EtwEventWriteEndScenario(RegHandle, EventDescriptor, UserDataCount, UserData);
}

ULONG WINAPI
EventWriteStartScenario(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	//return EtwEventWriteStartScenario(RegHandle, EventDescriptor, UserDataCount, UserData);
	return ERROR_SUCCESS;
}

ULONG WINAPI EventWriteString(
  _In_  REGHANDLE RegHandle,
  _In_  UCHAR Level,
  _In_  ULONGLONG Keyword,
  _In_  PCWSTR String
)
{
	return ERROR_SUCCESS;
}

/* unimplemented*/
BOOLEAN WINAPI EventEnabled(
  IN REGHANDLE RegHandle,
  IN PCEVENT_DESCRIPTOR EventDescriptor)
{
	//return EtwEventEnabled(RegHandle, EventDescriptor);
	return TRUE;
}

/* unimplemented*/
ULONG WINAPI EventWriteEx(
  _In_      REGHANDLE RegHandle,
  _In_      PCEVENT_DESCRIPTOR EventDescriptor,
  _In_      ULONG64 Filter,
  _In_      ULONG Flags,
  _In_opt_  LPCGUID ActivityId,
  _In_      LPCGUID RelatedActivityId,
  _In_      ULONG UserDataCount,
  _In_opt_  PEVENT_DATA_DESCRIPTOR UserData
)
{
	return ERROR_SUCCESS;
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
/*
ULONG WINAPI EnableTraceEx(
  _In_      LPCGUID ProviderId,
  _In_opt_  LPCGUID SourceId,
  _In_      TRACEHANDLE TraceHandle,
  _In_      ULONG IsEnabled,
  _In_      UCHAR Level,
  _In_      ULONGLONG MatchAnyKeyword,
  _In_      ULONGLONG MatchAllKeyword,
  _In_      ULONG EnableProperty,
  _In_opt_  PEVENT_FILTER_DESCRIPTOR EnableFilterDesc
)
{
	return ERROR_SUCCESS;	
}*/

ULONG WINAPI EventActivityIdControl(
  _In_     ULONG ControlCode,
  _Inout_  LPGUID ActivityId
)
{
	return EtwEventActivityIdControl(ControlCode, ActivityId);
}

/* unimplemented*/
BOOLEAN WINAPI EventProviderEnabled(
  _In_  REGHANDLE RegHandle,
  _In_  UCHAR Level,
  _In_  ULONGLONG Keyword
)
{
	return TRUE;
}

/* unimplemented*/
ULONG WINAPI EventAccessControl(
  _In_  LPGUID Guid,
  _In_  ULONG Operation,
  _In_  PSID Sid,
  _In_  ULONG Rights,
  _In_  BOOLEAN AllowOrDeny
)
{
	return ERROR_SUCCESS;
}


ULONG WINAPI EventWriteTransfer(
  _In_      REGHANDLE RegHandle,
  _In_      PCEVENT_DESCRIPTOR EventDescriptor,
  _In_opt_  LPCGUID ActivityId,
  _In_      LPCGUID RelatedActivityId,
  _In_      ULONG UserDataCount,
  _In_opt_  PEVENT_DATA_DESCRIPTOR UserData
)
{
	return ERROR_SUCCESS;
}


/* implemented
DWORD WINAPI InitiateShutdownW(
  _In_opt_  LPWSTR lpMachineName,
  _In_opt_  LPWSTR lpMessage,
  _In_      DWORD dwGracePeriod,
  _In_      DWORD dwShutdownFlags,
  _In_      DWORD dwReason
)
{
	BOOL shutdown;
	DWORD time = dwGracePeriod;
	switch(dwShutdownFlags)
	{
		case SHUTDOWN_GRACE_OVERRIDE:
			time = 0;
			shutdown = 1;
			break;
		case SHUTDOWN_RESTART:
			shutdown = 0;
			break;
		default:
			shutdown = 1;
			break;
	}
	InitiateSystemShutdownW(lpMachineName,lpMessage, 0, 1, shutdown);
	return ERROR_SUCCESS;
}*/
/*
DWORD WINAPI NotifyServiceStatusChangeW(
  _In_  SC_HANDLE hService,
  _In_  DWORD dwNotifyMask,
  _In_  PSERVICE_NOTIFY pNotifyBuffer
)
{
	return ERROR_SUCCESS;
}
*/
// Non-recursive RegDeleteTreeW implementation by Thomas, however it needs bugfixing
static NTSTATUS
RegpDeleteTree(IN HKEY hKey)
{
    typedef struct
    {
        LIST_ENTRY ListEntry;
        HANDLE KeyHandle;
    } REGP_DEL_KEYS, *PREG_DEL_KEYS;

    LIST_ENTRY delQueueHead;
    PREG_DEL_KEYS delKeys, newDelKeys;
    HANDLE ProcessHeap;
    ULONG BufferSize;
    PKEY_BASIC_INFORMATION BasicInfo;
    PREG_DEL_KEYS KeyDelRoot;
    NTSTATUS Status = STATUS_SUCCESS;
    NTSTATUS Status2 = STATUS_SUCCESS;

    InitializeListHead(&delQueueHead);

    ProcessHeap = RtlGetProcessHeap();

    /* NOTE: no need to allocate enough memory for an additional KEY_BASIC_INFORMATION
             structure for the root key, we only do that for subkeys as we need to
             allocate REGP_DEL_KEYS structures anyway! */
    KeyDelRoot = RtlAllocateHeap(ProcessHeap,
                                 0,
                                 sizeof(REGP_DEL_KEYS));
    if (KeyDelRoot != NULL)
    {
        KeyDelRoot->KeyHandle = hKey;
        InsertTailList(&delQueueHead,
                       &KeyDelRoot->ListEntry);

        do
        {
            delKeys = CONTAINING_RECORD(delQueueHead.Flink,
                                        REGP_DEL_KEYS,
                                        ListEntry);

            BufferSize = 0;
            BasicInfo = NULL;
            newDelKeys = NULL;

ReadFirstSubKey:
            /* check if this key contains subkeys and delete them first by queuing
               them at the head of the list */
            Status2 = NtEnumerateKey(delKeys->KeyHandle,
                                     0,
                                     KeyBasicInformation,
                                     BasicInfo,
                                     BufferSize,
                                     &BufferSize);

            if (NT_SUCCESS(Status2))
            {
                OBJECT_ATTRIBUTES ObjectAttributes;
                UNICODE_STRING SubKeyName;

                ASSERT(newDelKeys != NULL);
                ASSERT(BasicInfo != NULL);

                /* don't use RtlInitUnicodeString as the string is not NULL-terminated! */
                SubKeyName.Length = (ULONG)(USHORT)BasicInfo->NameLength;
                SubKeyName.MaximumLength = (ULONG)(USHORT)BasicInfo->NameLength;
                SubKeyName.Buffer = BasicInfo->Name;

                InitializeObjectAttributes(&ObjectAttributes,
                                           &SubKeyName,
                                           OBJ_CASE_INSENSITIVE,
                                           delKeys->KeyHandle,
                                           NULL);

                /* open the subkey */
                Status2 = NtOpenKey(&newDelKeys->KeyHandle,
                                    DELETE | KEY_ENUMERATE_SUB_KEYS,
                                    &ObjectAttributes);
                if (!NT_SUCCESS(Status2))
                {
                    goto SubKeyFailure;
                }

                /* enqueue this key to the head of the deletion queue */
                InsertHeadList(&delQueueHead,
                               &newDelKeys->ListEntry);

                /* try again from the head of the list */
                continue;
            }
            else
            {
                if (Status2 == STATUS_BUFFER_TOO_SMALL)
                {
                    newDelKeys = RtlAllocateHeap(ProcessHeap,
                                                 0,
                                                 BufferSize + sizeof(REGP_DEL_KEYS));
                    if (newDelKeys != NULL)
                    {
                        BasicInfo = (PKEY_BASIC_INFORMATION)(newDelKeys + 1);

                        /* try again */
                        goto ReadFirstSubKey;
                    }
                    else
                    {
                        /* don't break, let's try to delete as many keys as possible */
                        Status2 = STATUS_INSUFFICIENT_RESOURCES;
                        goto SubKeyFailureNoFree;
                    }
                }
                else if (Status2 == STATUS_BUFFER_OVERFLOW)
                {
                    PREG_DEL_KEYS newDelKeys2;

                    ASSERT(newDelKeys != NULL);

                    /* we need more memory to query the key name */
                    newDelKeys2 = RtlReAllocateHeap(ProcessHeap,
                                                    0,
                                                    newDelKeys,
                                                    BufferSize + sizeof(REGP_DEL_KEYS));
                    if (newDelKeys2 != NULL)
                    {
                        newDelKeys = newDelKeys2;
                        BasicInfo = (PKEY_BASIC_INFORMATION)(newDelKeys + 1);

                        /* try again */
                        goto ReadFirstSubKey;
                    }
                    else
                    {
                        /* don't break, let's try to delete as many keys as possible */
                        Status2 = STATUS_INSUFFICIENT_RESOURCES;
                    }
                }
                else if (Status2 == STATUS_NO_MORE_ENTRIES)
                {
                    /* in some race conditions where another thread would delete
                       the same tree at the same time, newDelKeys could actually
                       be != NULL! */
                    if (newDelKeys != NULL)
                    {
                        RtlFreeHeap(ProcessHeap,
                                    0,
                                    newDelKeys);
                    }
                    break;
                }

SubKeyFailure:
                /* newDelKeys can be NULL here when NtEnumerateKey returned an
                   error other than STATUS_BUFFER_TOO_SMALL or STATUS_BUFFER_OVERFLOW! */
                if (newDelKeys != NULL)
                {
                    RtlFreeHeap(ProcessHeap,
                                0,
                                newDelKeys);
                }

SubKeyFailureNoFree:
                /* don't break, let's try to delete as many keys as possible */
                if (NT_SUCCESS(Status))
                {
                    Status = Status2;
                }
            }

            Status2 = NtDeleteKey(delKeys->KeyHandle);

            /* NOTE: do NOT close the handle anymore, it's invalid already! */

            if (!NT_SUCCESS(Status2))
            {
                /* close the key handle so we don't leak handles for keys we were
                   unable to delete. But only do this for handles not supplied
                   by the caller! */

                if (delKeys->KeyHandle != hKey)
                {
                    NtClose(delKeys->KeyHandle);
                }

                if (NT_SUCCESS(Status))
                {
                    /* don't break, let's try to delete as many keys as possible */
                    Status = Status2;
                }
            }

            /* remove the entry from the list */
            RemoveEntryList(&delKeys->ListEntry);

            RtlFreeHeap(ProcessHeap,
                        0,
                        delKeys);
        } while (!IsListEmpty(&delQueueHead));
    }
    else
        Status = STATUS_INSUFFICIENT_RESOURCES;

    return Status;
}

BOOL WINAPI AddMandatoryAce(PACL pAcl, DWORD dwAceRevision, DWORD AceFlags, DWORD MandatoryPolicy, PSID pLabelSid)
{
  NTSTATUS resp; // eax@1
  BOOL result; // eax@2

  resp = RtlAddMandatoryAce(pAcl, dwAceRevision, AceFlags, MandatoryPolicy, 17, pLabelSid);
  if ( resp < 0 )
  {
    result = 0;
  }
  else
  {
    result = 1;
  }
  return result;
}