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
//#include <werapi.h>

#define WER_FAULT_REPORTING_ALWAYS_SHOW_UI          16

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

const WCHAR path = 0;

/*
* @unimplemented
*/
HRESULT 
WINAPI 
WerSetFlags(
  _In_  DWORD dwFlags
)
{
	return S_OK;
}

/*
* @unimplemented
*/
LONG 
WINAPI 
WerpCleanupMessageMapping()
{
	UNIMPLEMENTED;	
	return 0;
}

/*
* @unimplemented
*/
HRESULT 
WINAPI 
WerGetFlags(
  _In_   HANDLE hProcess,
  _Out_  PDWORD pdwFlags
)
{
	*pdwFlags = WER_FAULT_REPORTING_ALWAYS_SHOW_UI;	
	return S_OK;
}

/*
* @unimplemented
*/
HRESULT 
WINAPI 
WerRegisterMemoryBlock(
  _In_  PVOID pvAddress,
  _In_  DWORD dwSize
)
{
	UNIMPLEMENTED;	
	return S_OK;
}

/*
* @unimplemented
*/
HRESULT WINAPI WerUnregisterFile(
  _In_  PCWSTR pwzFilePath
)
{
	UNIMPLEMENTED;	
	return S_OK;
}

/*
* @unimplemented
*/
HRESULT WINAPI WerUnregisterMemoryBlock(
  _In_  PVOID pvAddress
)
{
	UNIMPLEMENTED;	
	return S_OK;
}

/*
* @unimplemented
*/
BOOL WINAPI WerpStringLookup(
  _In_  PVOID pvAddress,
  _In_  PVOID OtherAddress  
)
{
	UNIMPLEMENTED;	
	return S_OK;
}


/*
* @unimplemented
*/
int 
WINAPI 
WerpInitiateRemoteRecovery(HANDLE hndl)
{
	UNIMPLEMENTED;
	return 0;
}

/*
* @unimplemented
*/
HRESULT 
WINAPI 
WerRegisterFile(
  _In_  PCWSTR pwzFile,
  _In_  WER_REGISTER_FILE_TYPE regFileType,
  _In_  DWORD dwFlags
)
{
	UNIMPLEMENTED;
	return S_OK;
}

/*
* @unimplemented
*/
void 
WINAPI 
WerpNotifyUseStringResource(HANDLE file)
{
	UNIMPLEMENTED;
}

void 
WINAPI 
WerpNotifyLoadStringResource(
	HMODULE module, 
	LPCWSTR string, 
	HANDLE handle, 
	DWORD flags
)
{
	UNIMPLEMENTED;
}


HRESULT WINAPI WerUnregisterRuntimeExceptionModule(
  _In_      PCWSTR pwszOutOfProcessCallbackDll,
  _In_opt_  PVOID pContext
)
{
	UNIMPLEMENTED;
	return S_OK;
}


HRESULT 
WINAPI 
WerRegisterRuntimeExceptionModule(
  _In_      PCWSTR pwszOutOfProcessCallbackDll,
  _In_opt_  PVOID pContext
)
{
	UNIMPLEMENTED;
	return S_OK;
}

int 
WINAPI 
WerpLaunchAeDebug(void *a1, HANDLE hSourceHandle, char *a3, const void *a4, WCHAR *a5, int a6)
{
  void *v6; // ebx@1
  int v7; // edi@1
  DWORD v8; // edx@1
  DWORD v9; // eax@2
  int v10; // esi@2
  HANDLE v11; // ebx@8
  int v12; // edx@12
  int v13; // eax@14
  char *v14; // eax@15
  int *v15; // esi@16
  wchar_t *v16; // eax@21
  __int32 v17; // eax@23
  __int32 v18; // eax@27
  int v19; // ecx@30
  NTSTATUS v20; // eax@33
  PSTR v22; // [sp-8h] [bp-410h]@24
  char v23; // [sp-4h] [bp-40Ch]@24
  HANDLE hObject; // [sp+Ch] [bp-3FCh]@1
  HANDLE hProcess; // [sp+10h] [bp-3F8h]@1
  int v26; // [sp+14h] [bp-3F4h]@4
  int *v27; // [sp+18h] [bp-3F0h]@1
  char *v28; // [sp+1Ch] [bp-3ECh]@1
  LPVOID lpAddress; // [sp+20h] [bp-3E8h]@1
  PWSTR Format; // [sp+24h] [bp-3E4h]@1
  void *v31; // [sp+28h] [bp-3E0h]@1
  SIZE_T NumberOfBytesWritten; // [sp+2Ch] [bp-3DCh]@16
  const void *v33; // [sp+30h] [bp-3D8h]@1
  DWORD v34; // [sp+34h] [bp-3D4h]@1
  BOOL v35; // [sp+38h] [bp-3D0h]@4
  struct _PROCESS_INFORMATION ProcessInformation; // [sp+3Ch] [bp-3CCh]@1
  HANDLE Handles; // [sp+4Ch] [bp-3BCh]@32
  int v38; // [sp+50h] [bp-3B8h]@32
  struct _SECURITY_ATTRIBUTES EventAttributes; // [sp+54h] [bp-3B4h]@6
  struct _STARTUPINFOW StartupInfo; // [sp+60h] [bp-3A8h]@1
  unsigned __int16 SystemInformation; // [sp+ACh] [bp-35Ch]@10
  int Buffer; // [sp+B8h] [bp-350h]@14
  int v43; // [sp+BCh] [bp-34Ch]@14
  int v44; // [sp+C0h] [bp-348h]@12
  int v45; // [sp+C4h] [bp-344h]@14
  int v46; // [sp+C8h] [bp-340h]@14
  int v47; // [sp+CCh] [bp-33Ch]@14
  char *v48; // [sp+D0h] [bp-338h]@16
  int v49; // [sp+D4h] [bp-334h]@16
  char *v50; // [sp+D8h] [bp-330h]@16
  int v51; // [sp+DCh] [bp-32Ch]@16
  char v52; // [sp+E0h] [bp-328h]@16
  char v53[724]; // [sp+130h] [bp-2D8h]@16

  hProcess = a1;
  v28 = a3;
  v6 = 0;
  v33 = a4;
  Format = a5;
  hObject = 0;
  StartupInfo.cb = 0;
  memset(&StartupInfo.lpReserved, 0, 0x40u);
  lpAddress = 0;
  v27 = 0;
  ProcessInformation.hProcess = 0;
  ProcessInformation.hThread = 0;
  ProcessInformation.dwProcessId = 0;
  ProcessInformation.dwThreadId = 0;
  v31 = 0;
  v7 = 0;
  v8 = GetProcessId(a1);
  v34 = v8;
  if ( !v8 )
  {
    v9 = GetLastError();
    v10 = v9;
    if ( (signed int)v9 > 0 )
      v10 = (unsigned __int16)v9 | 0x80070000;
    goto LABEL_44;
  }
  #ifdef _M_IX86
	v35 = v8 == *(DWORD *)(__readfsdword(24) + 32);
  #elif defined(_M_AMD64)
	v35 = v8 == *(DWORD *)(__readgsqword(24) + 32);
  #endif
  v7 = (int)VirtualAlloc(0, 0x4C0u, 0x1000u, 4u);
  v26 = v7;
  if ( !v7 )
  {
    v10 = -2147024882;
    goto LABEL_44;
  }
  EventAttributes.bInheritHandle = 1;
  EventAttributes.lpSecurityDescriptor = 0;
  EventAttributes.nLength = 12;
  if ( !DuplicateHandle((HANDLE)0xFFFFFFFF, hSourceHandle, (HANDLE)0xFFFFFFFF, &hObject, 0, 1, 2u) )
    hObject = 0;
  v11 = CreateEventW(&EventAttributes, 1, 0, 0);
  if ( v11 && v28 && NtQuerySystemInformation(SystemProcessorInformation, &SystemInformation, 0xCu, 0) >= 0 )
  {
    if ( hSourceHandle )
    {
      v44 = GetThreadId(hSourceHandle);
      v12 = 0;
    }
    else
    {
      v12 = 0;
      v44 = 0;
    }
    v43 = SystemInformation;
    v13 = *((DWORD *)v28 + 3);
    Buffer = 40;
    v45 = v12;
    v46 = v13;
    v47 = v12;
    if ( v35 )
    {
      v50 = (char *)v33;
      v48 = v28;
      v49 = v12;
      v51 = v12;
      v27 = &Buffer;
    }
    else
    {
      v14 = VirtualAllocEx(hProcess, (LPVOID)v12, 0x348u, 0x1000u, 4u);
      lpAddress = v14;
      if ( v14 )
      {
        memcpy(&v52, v28, 0x50u);
        memcpy(v53, v33, 0x2CCu);
        v15 = (int *)v14;
        v49 = 0;
        v51 = 0;
        if ( WriteProcessMemory(hProcess, v14, &Buffer, 0x348u, &NumberOfBytesWritten) )
        {
          v7 = v26;
          if ( NumberOfBytesWritten == 840 )
            v27 = v15;
        }
        else
        {
          v7 = v26;
        }
      }
    }
  }
  v16 = Format;
  if ( *Format != 34 )
  {
    if ( RtlDetermineDosPathNameType_U(Format) == 5 )
    {
      v17 = StringCchPrintfW((wchar_t *)(v7 + 630), 0x125u, L"%s\\system32\\%s", 2147352624, Format);
      v10 = v17;
      if ( v17 < 0 )
      {
        v23 = v17;
        v22 = "StringCchPrintf failed while printing the debugger path with 0x%x";
        goto LABEL_29;
      }
      v16 = (wchar_t *)(v7 + 630);
    }
    else
    {
      v16 = Format;
    }
  }
  v18 = StringCchPrintfW((wchar_t *)v7, 0x13Bu, v16, v34, v11, v27);
  v10 = v18;
  if ( v18 >= 0 )
  {
    v31 = (void *)RtlGetCurrentTransaction();
    RtlSetCurrentTransaction(0);
    v19 = -(a6 != 0);
    StartupInfo.cb = 68;
    StartupInfo.lpDesktop = (LPWSTR)path;
    if ( CreateProcessW(0, (LPWSTR)v7, 0, 0, 1, v19 & 0x40000, 0, 0, &StartupInfo, &ProcessInformation) && v11 )
    {
      Handles = v11;
      v38 = (signed int)ProcessInformation.hProcess;
      do
      {
        do
          v20 = NtWaitForMultipleObjects(2u, &Handles, WaitAny, 1u, 0);
        while ( v20 == 192 );
      }
      while ( v20 == 257 );
      if ( v20 == 1 )
      {
        v10 = 0x8000FFFFu;
      }
      else
      {
        if ( !v20 )
          v10 = 0;
      }
    }
    goto LABEL_39;
  }
  v23 = v18;
  v22 = "StringCchPrintf failed while printng the debugger commandline with 0x%x";
LABEL_29:
	TRACE("Error");
LABEL_39:
  if ( lpAddress )
    VirtualFreeEx(hProcess, lpAddress, 0, 0x8000u);
  if ( v11 )
    CloseHandle(v11);
  v6 = v31;
LABEL_44:
  if ( hObject )
  {
    CloseHandle(hObject);
    hObject = 0;
  }
  if ( (DWORD)ProcessInformation.hProcess )
    CloseHandle(ProcessInformation.hProcess);
  if ( ProcessInformation.hThread )
    CloseHandle(ProcessInformation.hThread);
  if ( v6 )
  {
    RtlSetCurrentTransaction(v6);
    CloseHandle(v6);
  }
  if ( v7 )
    VirtualFree((LPVOID)v7, 0, 0x8000u);
  return v10;
}

BOOL 
WINAPI 
WerpIsProtectedProcess(HANDLE ProcessHandle)
{
  NTSTATUS status; // eax@3
  PVOID ProcessInformation; // [sp+8h] [bp-24h]@1
  char otherMember; // [sp+Ch] [bp-20h]@1

  ProcessInformation = 0;
  memset(&otherMember, 0, 0x1Cu);
  if ( !ProcessHandle )
    return FALSE;
  ProcessInformation = (PVOID)32;
  status = NtQueryInformationProcess(ProcessHandle, 0, &ProcessInformation, 0x20u, 0);
  if ( !NT_SUCCESS(status))
  {
    TRACE(0, "NtQueryInformationProcess failed with status: 0x%x", status);
    return FALSE;
  }
  return TRUE;
}

BOOL 
WINAPI 
WerpIsProcessInAeDebugExclusionList(
	HANDLE ProcessHandle
)
{
  BOOL resp; // ebx@1
  BOOL otherResult; // edi@1
  NTSTATUS status; // ecx@1
  LPCWSTR string; // esi@3
  PUNICODE_STRING unicodeString; // eax@4
  USHORT length; // ecx@5
  PCWSTR maximumLenght; // esi@10
  NTSTATUS receiveStatus; // ecx@14
  LPCSTR failed; // [sp-8h] [bp-274h]@2
  NTSTATUS otherStatus; // [sp-4h] [bp-270h]@2
  PVOID ProcessInformation; // [sp+10h] [bp-25Ch]@1
  LPCWSTR lpString = L""; // [sp+14h] [bp-258h]@3
  LSA_UNICODE_STRING ValueName; // [sp+228h] [bp-44h]@14
  OBJECT_ATTRIBUTES ObjectAttributes; // [sp+230h] [bp-3Ch]@11
  ULONG ResultLength; // [sp+248h] [bp-24h]@1
  HANDLE hObject; // [sp+24Ch] [bp-20h]@1
  int KeyValueInformation; // [sp+250h] [bp-1Ch]@1
  int compose; // [sp+254h] [bp-18h]@1
  int composeTwo; // [sp+25Ch] [bp-10h]@1
  PUNICODE_STRING RegstringIsProtected = NULL;
  PUNICODE_STRING AeDebugString = NULL;

  resp = 0;
  ProcessInformation = 0;
  hObject = 0;
  memset((char *)&ProcessInformation + 2, 0, 0x212u);
  KeyValueInformation = 0;
  compose = 0;
  composeTwo = 0;
  ResultLength = 0;
  otherResult = WerpIsProtectedProcess(ProcessHandle);
  status = NtQueryInformationProcess(ProcessHandle, ProcessLdtSize|0x20, &ProcessInformation, 0x210u, 0);
  if ( (status & 0xC0000000) == 0xC0000000 )
  {
    otherStatus = status;
    failed = "NtQueryInformationProcess failed 0x%x";
LABEL_16:
    TRACE(0, failed, otherStatus);
    goto LABEL_22;
  }
  string = lpString;
  if ( !lpString )
    goto LABEL_27;
  unicodeString = (PUNICODE_STRING)&lpString[lstrlenW(lpString)];
  while ( (unsigned int)unicodeString > (unsigned int)string )
  {
    length = unicodeString->Length;
    if ( length == 92 || length == 47 || length == 58 )
      goto LABEL_10;
  }
LABEL_10:
  maximumLenght = (PCWSTR)unicodeString->MaximumLength;
  if ( unicodeString != (PUNICODE_STRING)-2 )
  {
    RtlInitUnicodeString(RegstringIsProtected, L"\\Registry\\Machine\\Software\\Microsoft\\Windows NT\\CurrentVersion\\AeDebugProtected\\AutoExclusionList");
	ObjectAttributes.Length = 24;
    ObjectAttributes.RootDirectory = 0;
    ObjectAttributes.Attributes = 64;
    ObjectAttributes.ObjectName = RegstringIsProtected;
    if ( !otherResult )
	  RtlInitUnicodeString(RegstringIsProtected, L"\\Registry\\Machine\\Software\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug\\AutoExclusionList");
      ObjectAttributes.ObjectName = AeDebugString;
    ObjectAttributes.SecurityDescriptor = 0;
    ObjectAttributes.SecurityQualityOfService = 0;
    if ( NtOpenKey(&hObject, 1u, &ObjectAttributes) >= 0 )
    {
      ResultLength = 0;
      receiveStatus = RtlInitUnicodeStringEx(&ValueName, maximumLenght);
      if ( (receiveStatus & 0xC0000000) == 0xC0000000 )
      {
        otherStatus = receiveStatus;
        failed = "RtlInitUnicodeStringEx returned 0x%x";
        goto LABEL_16;
      }
      if ( NtQueryValueKey(hObject, &ValueName, KeyValuePartialInformation, &KeyValueInformation, 0x14u, &ResultLength) >= 0
        && compose == 4
        && composeTwo )
        resp = 1;
    }
  }
  else
  {
LABEL_27:
    TRACE(0, "WerpPathTail returned NULL");
  }
LABEL_22:
  if ( hObject )
    CloseHandle(hObject);
  return resp;
}

HRESULT 
WINAPI 
WerpGetDebugger(
	HANDLE ProcessHandle, 
	BOOL second, 
	int thrird, 
	int four, 
	BOOL verificationParameter
)
{
  BOOL resp; // edi@1
  PUNICODE_STRING freeAloocString; // ebx@4
  HRESULT Hresult; // esi@5
  NTSTATUS status; // eax@10
  LSA_UNICODE_STRING *hum; // eax@13
  HRESULT otherHresult; // eax@17
  HRESULT result; // eax@37
  size_t size = 0; // [sp+0h] [bp-38h]@0
  LPCWSTR stringTarget = L""; // [sp+4h] [bp-34h]@0
  HANDLE hObject; // [sp+Ch] [bp-2Ch]@1
  BOOL IsProtect; // [sp+10h] [bp-28h]@4
  ULONG ResultLength; // [sp+14h] [bp-24h]@1
  ULONG KeyValueInformationLength; // [sp+18h] [bp-20h]@4
  BOOL verification; // [sp+1Ch] [bp-1Ch]@1
  OBJECT_ATTRIBUTES ObjectAttributes; // [sp+20h] [bp-18h]@1
  PUNICODE_STRING otherLsaUnicode = NULL;
  LSA_UNICODE_STRING LSAUnicodeString;
  LSA_UNICODE_STRING Launch;
  LSA_UNICODE_STRING AutoString;
  PUNICODE_STRING AeDebugProtected = NULL;
  PUNICODE_STRING AeDebug = NULL;
  
  AutoString.Buffer = L"Auto";
  Launch.Buffer = L"LaunchNonProtected";
  ObjectAttributes.Length = 0;
  ObjectAttributes.RootDirectory = 0;
  ObjectAttributes.ObjectName = 0;
  ObjectAttributes.Attributes = 0;
  ObjectAttributes.SecurityDescriptor = 0;
  ObjectAttributes.SecurityQualityOfService = 0;
  hObject = 0;
  resp = 0;
  ResultLength = 0;
  verification = 0;
  if ( ProcessHandle && second && (unsigned int)(thrird - 293) <= 0x7FFFFED2 )
  {
    IsProtect = WerpIsProtectedProcess(ProcessHandle);
    KeyValueInformationLength = 2 * thrird + 16;
    freeAloocString = (PUNICODE_STRING)VirtualAlloc(0, 2 * thrird + 16, 0x1000u, 4u);
    if ( freeAloocString )
    {
      if ( four )
        four = 0;
      second = 0;
	  RtlInitUnicodeString(AeDebugProtected, L"\\Registry\\Machine\\Software\\Microsoft\\Windows NT\\CurrentVersion\\AeDebugProtected");
      ObjectAttributes.Length = 24;
      ObjectAttributes.RootDirectory = 0;
      ObjectAttributes.Attributes = 64;
	  ObjectAttributes.ObjectName = AeDebugProtected;
      if ( !IsProtect )
	    RtlInitUnicodeString(AeDebug, L"\\Registry\\Machine\\Software\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug");
        ObjectAttributes.ObjectName = AeDebug;
      ObjectAttributes.SecurityDescriptor = 0;
      ObjectAttributes.SecurityQualityOfService = 0;
      status = NtOpenKey(&hObject, 1u, &ObjectAttributes);
      if ( status >= 0 )
      {
        if ( IsProtect )
        {
          RtlInitUnicodeString(otherLsaUnicode, L"\\Registry\\Machine\\Software\\Microsoft\\Windows NT\\CurrentVersion\\AeDebugProtected");
		  hum = otherLsaUnicode;
          verification = 1;
        }
        else
        {
          hum = &LSAUnicodeString;
        }
        if ( NtQueryValueKey(
               hObject,
               hum,
               KeyValuePartialInformation,
               freeAloocString,
               KeyValueInformationLength,
               &ResultLength) < 0
          || freeAloocString->Buffer != (PWSTR)1 )
        {
          Hresult = ERROR_NOT_FOUND;
        }
        else
        {
          otherHresult = StringCchCopyW(freeAloocString[1].Buffer, size, stringTarget);
          Hresult = otherHresult;
          if ( otherHresult >= 0 )
          {
            if ( IsProtect )
            {
              freeAloocString[1].Buffer = 0;
              if ( NtQueryValueKey(
                     hObject,
                     &Launch,
                     KeyValuePartialInformation,
                     freeAloocString,
                     KeyValueInformationLength,
                     &ResultLength) >= 0 )
              {
                if ( freeAloocString->Buffer == (PWSTR)4 )
                  verification = LOBYTE(freeAloocString[1].Buffer) == 0;
              }
            }
            freeAloocString[1].Buffer = 0;
            if ( NtQueryValueKey(
                   hObject,
                   &AutoString,
                   KeyValuePartialInformation,
                   freeAloocString,
                   KeyValueInformationLength,
                   &ResultLength) >= 0
              && freeAloocString->Buffer == (PWSTR)1
              && LOWORD(freeAloocString[1].Buffer) == 49 )
              resp = WerpIsProcessInAeDebugExclusionList(ProcessHandle) == 0;
            Hresult = 0;
          }
          else
          {
            TRACE(0, "Stringcchcopy failed while copying the debugger path 0x%x", otherHresult);
          }
        }
      }
      else
      {
        Hresult = status | 0x10000000;
      }
    }
    else
    {
      Hresult = E_OUTOFMEMORY;
    }
    if ( four )
      four = resp;
    if ( verificationParameter )
      verificationParameter = verification;
    if ( hObject )
    {
      CloseHandle(hObject);
      hObject = 0;
    }
    if ( freeAloocString )
      VirtualFree(freeAloocString, 0, 0x8000u);
    result = Hresult;
  }
  else
  {
    result = E_INVALIDARG;
  }
  return result;
}

BOOL 
WINAPI 
WerpReportExceptionInProcessContext(
	DWORD parameter
)
{
  BOOL resp; // ecx@1

  resp = TRUE;
  if ( parameter == 0xC00000FD )
    resp = FALSE;
  return resp;
}