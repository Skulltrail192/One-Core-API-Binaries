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

BOOLEAN
WINAPI
BaseIsAppcompatInfrastructureDisabled(VOID)
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
    if (g_ShimsEnabled == -1)
    {
        /* Open the safe mode key */
        Status = NtOpenKey(&KeyHandle, 1, &OptionKeyAttributes);
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
                 (KeyInfo.Data[0] == TRUE))
            {
                /* It is, so disable shims! */
                g_ShimsEnabled = TRUE;
            }
            else
            {
                /* Open the app compatibility engine settings key */
                Status = NtOpenKey(&KeyHandle, 1, &AppCompatKeyAttributes);
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
                        g_ShimsEnabled = TRUE;
                    }
                    else
                    {
                        /* Finally, open the app compatibility policy key */
                        Status = NtOpenKey(&KeyHandle, 1, &PolicyKeyAttributes);
                        if (NT_SUCCESS(Status))
                        {
                            /* Check if the system policy disables app compat */
                            Status = NtQueryValueKey(KeyHandle,
                                                     &DisableEngine,
                                                     KeyValuePartialInformation,
                                                     &KeyInfo,
                                                     sizeof(KeyInfo),
                                                     &ResultLength),
                                                     NtClose(KeyHandle);
                            if ((NT_SUCCESS(Status)) &&
                                (KeyInfo.Type == REG_DWORD) &&
                                (KeyInfo.DataLength == sizeof(ULONG)) &&
                                (KeyInfo.Data[0] == TRUE))
                            {
                                /* It does, so disable shims! */
                                g_ShimsEnabled = TRUE;
                            }
                            else
                            {
                                /* No keys are set, so enable shims! */
                                g_ShimsEnabled = FALSE;
                            }
                        }
                    }
                }
            }
        }
    }

    /* Return if shims are disabled or not ("Enabled == 1" means disabled!) */
    return g_ShimsEnabled ? TRUE : FALSE;
}