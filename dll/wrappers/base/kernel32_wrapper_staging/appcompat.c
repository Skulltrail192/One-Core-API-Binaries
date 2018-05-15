/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    appcompat.c

Abstract:

    This module implements compatibility checks for Win32 APIs

Author:

    Skulltrail 19-March-2017

Revision History:

--*/

#include <main.h>

ULONG g_ShimsEnabled;

ULONG g_ShimsDisabled = -1;
static BOOL g_ApphelpInitialized = FALSE;
static PVOID g_pApphelpCheckRunAppEx;
static PVOID g_pSdbPackAppCompatData;

#define APPHELP_VALID_RESULT        0x10000
#define APPHELP_RESULT_NOTFOUND     0x20000
#define APPHELP_RESULT_FOUND        0x40000


/* FUNCTIONS ******************************************************************/

BOOLEAN
WINAPI
IsShimInfrastructureDisabled(VOID)
{
    HANDLE KeyHandle;
    NTSTATUS Status;
    KEY_VALUE_PARTIAL_INFORMATION KeyInfo;
    ULONG ResultLength;
    UNICODE_STRING OptionKey = RTL_CONSTANT_STRING(L"\\Registry\\MACHINE\\System\\CurrentControlSet\\Control\\SafeBoot\\Option");
    UNICODE_STRING AppCompatKey = RTL_CONSTANT_STRING(L"\\Registry\\MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\AppCompatibility");
    UNICODE_STRING PolicyKey = RTL_CONSTANT_STRING(L"\\Registry\\MACHINE\\Software\\Policies\\Microsoft\\Windows\\AppCompat");
    UNICODE_STRING OptionValue = RTL_CONSTANT_STRING(L"OptionValue");
    UNICODE_STRING DisableAppCompat = RTL_CONSTANT_STRING(L"DisableAppCompat");
    UNICODE_STRING DisableEngine = RTL_CONSTANT_STRING(L"DisableEngine");
    OBJECT_ATTRIBUTES OptionKeyAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(&OptionKey, OBJ_CASE_INSENSITIVE);
    OBJECT_ATTRIBUTES AppCompatKeyAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(&AppCompatKey, OBJ_CASE_INSENSITIVE);
    OBJECT_ATTRIBUTES PolicyKeyAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(&PolicyKey, OBJ_CASE_INSENSITIVE);

    /*
     * This is a TROOLEAN, -1 means we haven't yet figured it out.
     * 0 means shims are enabled, and 1 means shims are disabled!
     */
    if (g_ShimsDisabled == -1)
    {
        ULONG DisableShims = FALSE;

        /* Open the safe mode key */
        Status = NtOpenKey(&KeyHandle, KEY_QUERY_VALUE, &OptionKeyAttributes);
        if (NT_SUCCESS(Status))
        {
            /* Check if this is safemode */
            Status = NtQueryValueKey(KeyHandle,
                                     &OptionValue,
                                     KeyValuePartialInformation,
                                     &KeyInfo,
                                     sizeof(KeyInfo),
                                     &ResultLength);
            NtClose(KeyHandle);
            if ((NT_SUCCESS(Status)) &&
                 (KeyInfo.Type == REG_DWORD) &&
                 (KeyInfo.DataLength == sizeof(ULONG)) &&
                 (KeyInfo.Data[0] != FALSE))
            {
                /* It is, so disable shims! */
                DisableShims = TRUE;
            }
        }

        if (!DisableShims)
        {
            /* Open the app compatibility engine settings key */
            Status = NtOpenKey(&KeyHandle, KEY_QUERY_VALUE, &AppCompatKeyAttributes);
            if (NT_SUCCESS(Status))
            {
                /* Check if the app compat engine is turned off */
                Status = NtQueryValueKey(KeyHandle,
                                         &DisableAppCompat,
                                         KeyValuePartialInformation,
                                         &KeyInfo,
                                         sizeof(KeyInfo),
                                         &ResultLength);
                NtClose(KeyHandle);
                if ((NT_SUCCESS(Status)) &&
                    (KeyInfo.Type == REG_DWORD) &&
                    (KeyInfo.DataLength == sizeof(ULONG)) &&
                    (KeyInfo.Data[0] == TRUE))
                {
                    /* It is, so disable shims! */
                    DisableShims = TRUE;
                }
            }
        }
        if (!DisableShims)
        {
            /* Finally, open the app compatibility policy key */
            Status = NtOpenKey(&KeyHandle, KEY_QUERY_VALUE, &PolicyKeyAttributes);
            if (NT_SUCCESS(Status))
            {
                /* Check if the system policy disables app compat */
                Status = NtQueryValueKey(KeyHandle,
                                         &DisableEngine,
                                         KeyValuePartialInformation,
                                         &KeyInfo,
                                         sizeof(KeyInfo),
                                         &ResultLength);
                NtClose(KeyHandle);
                if ((NT_SUCCESS(Status)) &&
                    (KeyInfo.Type == REG_DWORD) &&
                    (KeyInfo.DataLength == sizeof(ULONG)) &&
                    (KeyInfo.Data[0] == TRUE))
                {
                    /* It does, so disable shims! */
                    DisableShims = TRUE;
                }
            }
        }
        g_ShimsDisabled = DisableShims;
    }

    /* Return if shims are disabled or not ("Enabled == 1" means disabled!) */
    return g_ShimsDisabled ? TRUE : FALSE;
}