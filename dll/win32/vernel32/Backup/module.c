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

#define MAX_MODULES 0x2710      // Matches 10.000 modules

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

/*
 * @implemented
 */
static BOOL NTAPI
FindModule(IN HANDLE hProcess,
           IN HMODULE hModule OPTIONAL,
           OUT PLDR_DATA_TABLE_ENTRY Module)
{
    DWORD Count;
    NTSTATUS Status;
    PPEB_LDR_DATA LoaderData;
    PLIST_ENTRY ListHead, ListEntry;
    PROCESS_BASIC_INFORMATION ProcInfo;

    /* Query the process information to get its PEB address */
    Status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &ProcInfo, sizeof(ProcInfo), NULL);
    if (!NT_SUCCESS(Status))
    {
        SetLastError(RtlNtStatusToDosError(Status));
        return FALSE;
    }

    /* If no module was provided, get base as module */
    if (hModule == NULL)
    {
        if (!ReadProcessMemory(hProcess, &ProcInfo.PebBaseAddress->ImageBaseAddress, &hModule, sizeof(hModule), NULL))
        {
            return FALSE;
        }
    }

    /* Read loader data address from PEB */
    if (!ReadProcessMemory(hProcess, &ProcInfo.PebBaseAddress->Ldr, &LoaderData, sizeof(LoaderData), NULL))
    {
        return FALSE;
    }

    if (LoaderData == NULL)
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    /* Store list head address */
    ListHead = &(LoaderData->InMemoryOrderModuleList);

    /* Read first element in the modules list */
    if (!ReadProcessMemory(hProcess,
                           &(LoaderData->InMemoryOrderModuleList.Flink),
                           &ListEntry,
                           sizeof(ListEntry),
                           NULL))
    {
        return FALSE;
    }

    Count = 0;

    /* Loop on the modules */
    while (ListEntry != ListHead)
    {
        /* Load module data */
        if (!ReadProcessMemory(hProcess,
                               CONTAINING_RECORD(ListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderModuleList),
                               Module,
                               sizeof(*Module),
                               NULL))
        {
            return FALSE;
        }

        /* Does that match the module we're looking for? */
        if (Module->DllBase == hModule)
        {
            return TRUE;
        }

        ++Count;
        if (Count > MAX_MODULES)
        {
            break;
        }

        /* Get to next listed module */
        ListEntry = Module->InMemoryOrderModuleList.Flink;
    }

    SetLastError(ERROR_INVALID_HANDLE);
    return FALSE;
}

/*
 * @implemented
 */
DWORD
WINAPI
GetModuleFileNameExW(HANDLE hProcess,
                     HMODULE hModule,
                     LPWSTR lpFilename,
                     DWORD nSize)
{
    DWORD Len;
    LDR_DATA_TABLE_ENTRY Module;

    /* Get the matching module */
    if (!FindModule(hProcess, hModule, &Module))
    {
        return 0;
    }

    /* Get the maximum len we have/can write in given size */
    Len = Module.FullDllName.Length + sizeof(UNICODE_NULL);
    if (nSize * sizeof(WCHAR) < Len)
    {
        Len = nSize * sizeof(WCHAR);
    }

    /* Read string */
    if (!ReadProcessMemory(hProcess, (&Module.FullDllName)->Buffer, lpFilename, Len, NULL))
    {
        return 0;
    }

    /* If we are at the end of the string, prepare to override to nullify string */
    if (Len == Module.FullDllName.Length + sizeof(UNICODE_NULL))
    {
        Len -= sizeof(UNICODE_NULL);
    }

    /* Nullify at the end if needed */
    if (Len >= nSize * sizeof(WCHAR))
    {
        if (nSize)
        {
            ASSERT(nSize >= sizeof(UNICODE_NULL));
            lpFilename[nSize - 1] = UNICODE_NULL;
        }
    }
    /* Otherwise, nullify at last writen char */
    else
    {
        ASSERT(Len + sizeof(UNICODE_NULL) <= nSize * sizeof(WCHAR));
        lpFilename[Len / sizeof(WCHAR)] = UNICODE_NULL;
    }

    return Len / sizeof(WCHAR);
}

/*
 * @implemented
 */
DWORD
WINAPI
GetModuleFileNameExA(HANDLE hProcess,
                     HMODULE hModule,
                     LPSTR lpFilename,
                     DWORD nSize)
{
    DWORD Len;
    PWSTR Filename;

    /* Allocate internal buffer for conversion */
    Filename = LocalAlloc(LMEM_FIXED, nSize * sizeof(WCHAR));
    if (Filename == NULL)
    {
        return 0;
    }

    /* Call W API */
    Len = GetModuleFileNameExW(hProcess, hModule, Filename, nSize);
    /* And convert output */
    if (WideCharToMultiByte(CP_ACP, 0, Filename, (Len < nSize) ? Len + 1 : Len, lpFilename, nSize, NULL, NULL) == 0)
    {
        Len = 0;
    }

    LocalFree(Filename);

    return Len;
}
