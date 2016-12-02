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
#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

PVOID 
WINAPI 
ResolveDelayLoadedAPI(
  _In_        PVOID ParentModuleBase,
  _In_        PCIMAGE_DELAYLOAD_DESCRIPTOR DelayloadDescriptor,
  _In_opt_    PDELAYLOAD_FAILURE_DLL_CALLBACK FailureDllHook,
  _In_opt_    PDELAYLOAD_FAILURE_SYSTEM_ROUTINE FailureSystemHook,
  _Out_       PIMAGE_THUNK_DATA ThunkAddress,
  _Reserved_  ULONG Flags
)
{
	return LdrResolveDelayLoadedAPI(ParentModuleBase, 
									DelayloadDescriptor, 
									FailureDllHook, 
									FailureSystemHook, 
									ThunkAddress, 
									Flags);
}

NTSTATUS 
WINAPI 
ResolveDelayLoadsFromDll(
  _In_        PVOID ParentBase,
  _In_        LPCSTR TargetDllName,
  _Reserved_  ULONG Flags
)
{
	return LdrResolveDelayLoadsFromDll(ParentBase, TargetDllName, Flags);
}


WCHAR szAppInit[KEY_LENGTH];

BOOL
GetDllList()
{
    NTSTATUS Status;
    OBJECT_ATTRIBUTES Attributes;
    BOOL bRet = FALSE;
    BOOL bLoad;
    HANDLE hKey = NULL;
    DWORD dwSize;
    PKEY_VALUE_PARTIAL_INFORMATION kvpInfo = NULL;

    UNICODE_STRING szKeyName = RTL_CONSTANT_STRING(L"\\Registry\\Machine\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows");
    UNICODE_STRING szLoadName = RTL_CONSTANT_STRING(L"LoadAppInit_DLLs");
    UNICODE_STRING szDllsName = RTL_CONSTANT_STRING(L"AppInit_DLLs");

    InitializeObjectAttributes(&Attributes, &szKeyName, OBJ_CASE_INSENSITIVE, NULL, NULL);
    Status = NtOpenKey(&hKey, KEY_READ, &Attributes);
    if (NT_SUCCESS(Status))
    {
        dwSize = sizeof(KEY_VALUE_PARTIAL_INFORMATION) + sizeof(DWORD);
        kvpInfo = HeapAlloc(GetProcessHeap(), 0, dwSize);
        if (!kvpInfo)
            goto end;

        Status = NtQueryValueKey(hKey,
                                 &szLoadName,
                                 KeyValuePartialInformation,
                                 kvpInfo,
                                 dwSize,
                                 &dwSize);
        if (!NT_SUCCESS(Status))
            goto end;

        RtlMoveMemory(&bLoad,
                      kvpInfo->Data,
                      kvpInfo->DataLength);

        HeapFree(GetProcessHeap(), 0, kvpInfo);
        kvpInfo = NULL;

        if (bLoad)
        {
            Status = NtQueryValueKey(hKey,
                                     &szDllsName,
                                     KeyValuePartialInformation,
                                     NULL,
                                     0,
                                     &dwSize);
            if (Status != STATUS_BUFFER_TOO_SMALL)
                goto end;

            kvpInfo = HeapAlloc(GetProcessHeap(), 0, dwSize);
            if (!kvpInfo)
                goto end;

            Status = NtQueryValueKey(hKey,
                                     &szDllsName,
                                     KeyValuePartialInformation,
                                     kvpInfo,
                                     dwSize,
                                     &dwSize);
            if (NT_SUCCESS(Status))
            {
                LPWSTR lpBuffer = (LPWSTR)kvpInfo->Data;
                if (*lpBuffer != UNICODE_NULL)
                {
                    INT bytesToCopy, nullPos;

                    bytesToCopy = min(kvpInfo->DataLength, KEY_LENGTH * sizeof(WCHAR));

                    if (bytesToCopy != 0)
                    {
                        RtlMoveMemory(szAppInit,
                                      kvpInfo->Data,
                                      bytesToCopy);

                        nullPos = (bytesToCopy / sizeof(WCHAR)) - 1;

                        /* ensure string is terminated */
                        szAppInit[nullPos] = UNICODE_NULL;

                        bRet = TRUE;
                    }
                }
            }
        }
    }

end:
    if (hKey)
        NtClose(hKey);

    if (kvpInfo)
        HeapFree(GetProcessHeap(), 0, kvpInfo);

    return bRet;
}

VOID
LoadAppInitDlls()
{
    szAppInit[0] = UNICODE_NULL;

    if (GetDllList())
    {
        WCHAR buffer[KEY_LENGTH];
        LPWSTR ptr;
		size_t i;

        RtlCopyMemory(buffer, szAppInit, KEY_LENGTH * sizeof(WCHAR) );

		for (i = 0; i < KEY_LENGTH; ++ i)
		{
			if(buffer[i] == L' ' || buffer[i] == L',')
				buffer[i] = 0;
		}

		for (i = 0; i < KEY_LENGTH; )
		{
			if(buffer[i] == 0)
				++ i;
			else
			{
				ptr = buffer + i;
				i += wcslen(ptr);
				LoadLibraryW(ptr);
			}
		}
    }
}


DWORD WINAPI LoadStringBaseExW(HMODULE hModule, LPCWSTR string, HANDLE handle, DWORD other, WORD wLanguage)
{
  DWORD resp; // esi@2
  HRSRC findResource; // eax@2
  HGLOBAL global; // eax@3
  BOOL verification; // edi@4
  void *unknown; // eax@5
  HANDLE localHandle; // [sp-Ch] [bp-14h]@8

  if ( !handle )
    return 0;
  resp = 0;
  findResource = FindResourceExW(
                   hModule,
                   (LPCWSTR)6,
                   (LPCWSTR)(((unsigned int)(unsigned __int16)string >> 4) + 1),
                   wLanguage);
  if ( !findResource || (global = LoadResource(hModule, findResource)) == 0 )
    goto LABEL_13;
  verification = (DWORD)string & 0xF;
  while ( 1 )
  {
    resp = *(WORD *)global;
    unknown = (char *)global + 2;
    if ( !verification )
      break;
    --verification;
    global = (char *)unknown + 2 * resp;
  }
  if ( other )
  {
    --other;
    if ( (signed int)resp > (signed int)other )
      resp = other;
    memcpy(handle, unknown, 2 * resp);
LABEL_13:
    if ( other )
      *((WORD *)handle + resp) = 0;
    goto LABEL_15;
  }
  handle = unknown;
LABEL_15:
  if ( (signed int)resp > 0 )
  {
    if ( other )
      localHandle = handle;
    else
      localHandle = *(HANDLE *)handle;
    WerpNotifyLoadStringResource(hModule, string, localHandle, 4);
  }
  return resp;
}

DWORD WINAPI LoadStringBaseW(HMODULE hModule, LPWSTR string, HANDLE file, DWORD other)
{
  return LoadStringBaseExW(hModule, string, file, other, 0);
}
