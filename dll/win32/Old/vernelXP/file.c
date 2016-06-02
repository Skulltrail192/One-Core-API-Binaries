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

/*
 * @implemented
 */
BOOL WINAPI SetFileBandwidthReservation(HANDLE hFile, DWORD nPeriodMilliseconds, DWORD nBytesPerPeriod, BOOL bDiscardable, LPDWORD lpTransferSize, LPDWORD lpNumOutstandingRequests)
{
  NTSTATUS status; // eax@1
  unsigned int v8; // ecx@5
  DWORD v9; // edx@6
  DWORD v10; // [sp-4h] [bp-20h]@10
  DWORD FileInformation; // [sp+0h] [bp-1Ch]@1
  DWORD v12; // [sp+4h] [bp-18h]@8
  char v13; // [sp+8h] [bp-14h]@8
  char v14; // [sp+9h] [bp-13h]@8
  DWORD localLpTransferSize = (DWORD)lpTransferSize; // [sp+Ch] [bp-10h]@5
  DWORD localLpNumOutstandingRequests = (DWORD)lpNumOutstandingRequests;
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+14h] [bp-8h]@1

  status = NtQueryInformationFile(
         hFile,
         &IoStatusBlock,
         &FileInformation,
         0x14u,
         FileInformationReserved32|FileNamesInformation);
  if ( status < 0 )
    goto LABEL_2;
  v8 = localLpTransferSize;
  if ( !localLpTransferSize )
  {
    *lpTransferSize &= localLpTransferSize;
    *lpNumOutstandingRequests &= v8;
    v10 = 1;
LABEL_7:
    SetLastError(v10);
    return 0;
  }
  v9 = localLpNumOutstandingRequests;
  *lpTransferSize = localLpTransferSize;
  *lpNumOutstandingRequests = v9;
  FileInformation = nBytesPerPeriod / v8;
  if ( !(nBytesPerPeriod / v8) && nBytesPerPeriod )
  {
    v10 = 87;
    goto LABEL_7;
  }
  v12 = nPeriodMilliseconds;
  v14 = bDiscardable;
  v13 = 1;
  status = NtSetInformationFile(
         hFile,
         &IoStatusBlock,
         &FileInformation,
         0x14u,
         FileInformationReserved32|FileNamesInformation);
  if ( status < 0 )
  {
LABEL_2:
    SetLastError(status);
    return 0;
  }
  return 1;
}

/*
 * @implemented
 */
BOOL WINAPI GetFileBandwidthReservation(HANDLE hFile, LPDWORD lpPeriodMilliseconds, LPDWORD lpBytesPerPeriod, LPBOOL pDiscardable, LPDWORD lpTransferSize, LPDWORD lpNumOutstandingRequests)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  DWORD localSize; // eax@4
  DWORD localRequest; // edx@4
  int FileInformation; // [sp+0h] [bp-1Ch]@1
  DWORD localLpPeriodMilliseconds = (DWORD)lpPeriodMilliseconds; // [sp+4h] [bp-18h]@4
  DWORD localpDiscardable = (DWORD)pDiscardable; // [sp+9h] [bp-13h]@4
  DWORD localLpTransferSize = (DWORD)lpTransferSize; // [sp+Ch] [bp-10h]@4
  DWORD localLpNumOutstandingRequests = (DWORD)lpNumOutstandingRequests; // [sp+10h] [bp-Ch]@4
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+14h] [bp-8h]@1

  status = NtQueryInformationFile(
             hFile,
             &IoStatusBlock,
             &FileInformation,
             0x14u,
             FileInformationReserved32|FileNamesInformation);
  if ( status >= 0 )
  {
    localSize = localLpTransferSize;
    localRequest = localLpNumOutstandingRequests;
    *lpTransferSize = localLpTransferSize;
    *lpNumOutstandingRequests = localRequest;
    *lpPeriodMilliseconds = localLpPeriodMilliseconds;
    *lpBytesPerPeriod = localSize * FileInformation;
    *pDiscardable = localpDiscardable;
    result = 1;
  }
  else
  {
    SetLastError(status);
    result = 0;
  }
  return result;
}

/*
 * @implemented
 */
BOOL WINAPI GetFileInformationByHandleEx(HANDLE FileHandle, FILE_INFO_BY_HANDLE_CLASS FileInformationClass, PVOID FileInformation, DWORD FileInformationLength)
{
  DWORD lenght; // ecx@1
  BOOL verification; // edx@1
  FILE_INFORMATION_CLASS file; // eax@4
  NTSTATUS status; // eax@8
  DWORD sei; // [sp-4h] [bp-14h]@4
  DWORD la; // [sp-4h] [bp-14h]@19
  DWORD error; // [sp-4h] [bp-14h]@23
  NTSTATUS receiveStatus; // [sp-4h] [bp-14h]@32
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+8h] [bp-8h]@1

  lenght = 8;
  verification = 0;
  IoStatusBlock.Information = 0;
  if ( (signed int)FileInformationClass <= 8 )
  {
    if ( FileInformationClass == 8 )
    {
      file = 28;
      sei = 16;
      goto LABEL_5;
    }
    if ( !FileInformationClass )
    {
      file = 4;
      sei = 40;
LABEL_5:
      lenght = sei;
      goto LABEL_6;
    }
    if ( FileInformationClass == 1 )
    {
      file = 5;
      sei = 24;
      goto LABEL_5;
    }
    if ( FileInformationClass != 2 )
    {
      if ( FileInformationClass == 7 )
      {
        file = 22;
        sei = 32;
        goto LABEL_5;
      }
LABEL_23:
      error = 87;
LABEL_24:
      SetLastError(error);
      return 0;
    }
    la = 9;
    goto LABEL_29;
  }
  if ( FileInformationClass == 9 )
  {
    la = 35;
LABEL_29:
    file = la;
    goto LABEL_6;
  }
  if ( FileInformationClass == 10 )
  {
    file = 37;
    lenght = 112;
    verification = 1;
    IoStatusBlock.Information = 0;
  }
  else
  {
    if ( FileInformationClass != 11 )
      goto LABEL_23;
    file = 37;
    verification = 1;
    lenght = 112;
    IoStatusBlock.Information = 1;
  }
LABEL_6:
  if ( lenght > FileInformationLength )
  {
    error = 24;
    goto LABEL_24;
  }
  if ( verification )
    status = NtQueryDirectoryFile(
               FileHandle,
               NULL,
               NULL,
               NULL,
               &IoStatusBlock,
               FileInformation,
               FileInformationLength,
               file,
               0,
               NULL,
               LOBYTE(IoStatusBlock.Information));
  else
    status = NtQueryInformationFile(FileHandle, &IoStatusBlock, FileInformation, FileInformationLength, file);
  if ( status < 0 )
  {
    receiveStatus = status;
  }
  else
  {
    if ( FileInformationClass != 7 || IoStatusBlock.Information )
      return 1;
    receiveStatus = 0xC0000011u;
  }
 SetLastError(receiveStatus);
  return 0;
}

/*
 * @implemented - new
 */
HANDLE
WINAPI
OpenFileById(IN HANDLE hFile,
             IN FILE_ID_DESCRIPTOR *lpFileID,
             IN DWORD dwDesiredAccess,
             IN DWORD ShareAccess,
             IN LPSECURITY_ATTRIBUTES lpSecurityAttributes  OPTIONAL,
             IN DWORD dwFlags)
{
  FILE_ID_TYPE type; // ecx@3
  DWORD compare; // eax@7
  DWORD handle; // ecx@7
  NTSTATUS status; // eax@13
  OBJECT_ATTRIBUTES ObjectAttributes; // [sp+4h] [bp-28h]@7
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+1Ch] [bp-10h]@13
  ULONG other; // [sp+24h] [bp-8h]@4
  ULONG *part; // [sp+28h] [bp-4h]@7

  if ( !lpFileID || lpFileID->dwSize < 0x18 )
    goto LABEL_16;
  type = lpFileID->Type;
  if ( type )
  {
    if ( type == 1 )
    {
      other = 0x100010u;
      goto LABEL_7;
    }
LABEL_16:
    SetLastError(87u);
    return (HANDLE)-1;
  }
  other = 0x80008u;
LABEL_7:
  part = &lpFileID->FileId.LowPart;
  ObjectAttributes.RootDirectory = hFile;
  ObjectAttributes.ObjectName = (PUNICODE_STRING)&other;
  ObjectAttributes.Length = 24;
  compare = ~((dwFlags >> 25) & 0x20) | (((dwFlags & 0x2000000 )| (((dwFlags & 0x10000000) | (((dwFlags & 0x8000000) | (((dwFlags & 0x20000000) | ((dwFlags >> 4) & 0x8000000)) >> 1)) >> 8)) >> 6)) >> 11);
  handle = dwDesiredAccess;
  ObjectAttributes.Attributes = 64;
  ObjectAttributes.SecurityDescriptor = NULL;
  ObjectAttributes.SecurityQualityOfService = NULL;
  if ( dwFlags & 0x4000000 )
  {
    compare |= 0x1000u;
    handle = dwDesiredAccess | 0x10000;
  }
  if ( dwFlags & 0x200000 )
    compare |= 0x200000u;
  if ( dwFlags & 0x100000 )
    compare |= 0x400000u;
  status = NtCreateFile(
             (PHANDLE)&dwDesiredAccess,
             handle | 0x100080,
             &ObjectAttributes,
             &IoStatusBlock,
             NULL,
             0,
             ShareAccess,
             3u,
             compare | 0x2000,
             NULL,
             0);
  if ( status < 0 )
  {
    dwDesiredAccess = -1;
    SetLastError(status);
  }
  return (HANDLE)dwDesiredAccess;
}

/*
 * @unimplemented
 */
DWORD
WINAPI
GetFinalPathNameByHandleW(IN HANDLE hFile,
                          OUT LPWSTR lpszFilePath,
                          IN DWORD cchFilePath,
                          IN DWORD dwFlags)
{
    if (dwFlags & ~(VOLUME_NAME_DOS | VOLUME_NAME_GUID | VOLUME_NAME_NT |
                    VOLUME_NAME_NONE | FILE_NAME_NORMALIZED | FILE_NAME_OPENED))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }

    UNIMPLEMENTED;
    return 0;
}

/*
 * @implemented
*/
DWORD
WINAPI
GetFinalPathNameByHandleA(IN HANDLE hFile,
                          OUT LPSTR lpszFilePath,
                          IN DWORD cchFilePath,
                          IN DWORD dwFlags)
{
    WCHAR FilePathW[MAX_PATH];
    UNICODE_STRING FilePathU;
    DWORD PrevLastError;
    DWORD Ret = 0;

    if (cchFilePath != 0 &&
        cchFilePath > sizeof(FilePathW) / sizeof(FilePathW[0]))
    {
        FilePathU.Length = 0;
        FilePathU.MaximumLength = (USHORT)cchFilePath * sizeof(WCHAR);
        FilePathU.Buffer = RtlAllocateHeap(RtlGetProcessHeap(),
                                           0,
                                           FilePathU.MaximumLength);
        if (FilePathU.Buffer == NULL)
        {
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            return 0;
        }
    }
    else
    {
        FilePathU.Length = 0;
        FilePathU.MaximumLength = sizeof(FilePathW);
        FilePathU.Buffer = FilePathW;
    }

    /* save the last error code */
    PrevLastError = GetLastError();
    SetLastError(ERROR_SUCCESS);

    /* call the unicode version that does all the work */
    Ret = GetFinalPathNameByHandleW(hFile,
                                    FilePathU.Buffer,
                                    cchFilePath,
                                    dwFlags);

    if (GetLastError() == ERROR_SUCCESS)
    {
        /* no error, restore the last error code and convert the string */
        SetLastError(PrevLastError);

        Ret = FilenameU2A_FitOrFail(lpszFilePath,
                                    cchFilePath,
                                    &FilePathU);
    }

    /* free allocated memory if necessary */
    if (FilePathU.Buffer != FilePathW)
    {
        RtlFreeHeap(RtlGetProcessHeap(),
                    0,
                    FilePathU.Buffer);
    }

    return Ret;
}

/*
 * @implemented
 */
BOOL WINAPI SetFileInformationByHandle(HANDLE FileHandle, FILE_INFO_BY_HANDLE_CLASS FileInformationClass, PVOID FileInformation, DWORD FileInformationLength)
{
  FILE_INFORMATION_CLASS file; // eax@10
  DWORD receive; // ecx@12
  NTSTATUS status; // eax@18
  DWORD error; // [sp-4h] [bp-Ch]@6
  DWORD other; // [sp-4h] [bp-Ch]@9
  DWORD compose; // [sp-4h] [bp-Ch]@10
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+0h] [bp-8h]@18

  if ( FileInformationClass )
  {
    if ( FileInformationClass == 3 )
    {
      file = 10;
      compose = 16;
    }
    else
    {
      if ( FileInformationClass == 4 )
      {
        file = 13;
        receive = 1;
        goto LABEL_16;
      }
      if ( FileInformationClass == 5 )
      {
        other = 19;
      }
      else
      {
        if ( FileInformationClass != 6 )
        {
          error = 87;
LABEL_7:
          SetLastError(error);
          return 0;
        }
        other = 20;
      }
      file = other;
      compose = 8;
    }
  }
  else
  {
    file = 4;
    compose = 40;
  }
  receive = compose;
LABEL_16:
  if ( receive > FileInformationLength )
  {
    error = 24;
    goto LABEL_7;
  }
  status = NtSetInformationFile(FileHandle, &IoStatusBlock, FileInformation, FileInformationLength, file);
  if ( status < 0 )
  {
    SetLastError(status);
    return 0;
  }
  return 1;
}

/*
 * @implemented - new
 */
ULONG WINAPI GetModuleFileNameFromHandleNoLock(HMODULE hModule, LPWSTR lpFilename, DWORD nSize)
{
  HANDLE v3; // eax@5
  NTSTATUS v4; // eax@5
  ULONG result; // eax@2
  char MemoryInformation; // [sp+Ch] [bp-20Ch]@5
  unsigned __int16 *v7 = NULL; // [sp+10h] [bp-208h]@6

  if ( GetModuleFileNameW(hModule, lpFilename, nSize) )
    return 0;
  if ( (int)hModule & 1 )
  {
    v3 = GetCurrentProcess();
    v4 = NtQueryVirtualMemory(
           v3,
           (PVOID)((unsigned int)hModule & 0xFFFFFFFE),
           MemorySectionName,
           &MemoryInformation,
           0x208u,
           NULL);
    if ( v4 )
    {
      result = v4 | 0x10000000;
    }
    else
    {
      result = StringCchCopyNW(lpFilename, nSize, v7, nSize - 1);
      if ( result >= 0 )
        return 0;
    }
  }
  else
  {
    if ( (signed int)GetLastError() > 0 )
      result = (unsigned __int16)GetLastError() | 0x80070000;
    else
      result = GetLastError();
  }
  return result;
}

/*
 * @implemented - new
 */
BOOL WINAPI CancelSynchronousIo(_In_  HANDLE hFile)
{
  NTSTATUS status; // eax@3
  BOOL result; // eax@4
  PIO_STATUS_BLOCK localLpOverlapped = NULL;

  status = NtCancelIoFile(hFile, localLpOverlapped);
  if ( status < 0 )
  {
    SetLastError(status);
    result = 0;
  }
  else
  {
    result = 1;
  }
  return result;
}

/*
 * @implemented - new
 */
BOOL WINAPI CancelIoEx(
  _In_      HANDLE hFile,
  _In_opt_  LPOVERLAPPED lpOverlapped
)
{
  LPOVERLAPPED localLpOverlapped; // eax@1
  NTSTATUS status; // eax@3
  BOOL result; // eax@4

  localLpOverlapped = NULL;
  if ( lpOverlapped )
    localLpOverlapped = lpOverlapped;
  status = NtCancelIoFile(hFile, (PIO_STATUS_BLOCK)localLpOverlapped);
  if ( status < 0 )
  {
    SetLastError(status);
    result = 0;
  }
  else
  {
    result = 1;
  }
  return result;
}

HANDLE WINAPI CreateFileMappingNumaW(HANDLE FileHandle, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, PCWSTR lpName, DWORD nndPreferred)
{
  DWORD protect; // esi@4
  DWORD otherProtect; // edi@4
  ACCESS_MASK mask; // ebx@4
  NTSTATUS status; // eax@18
  LARGE_INTEGER *largeNumber; // eax@21
  LSA_UNICODE_STRING *security; // [sp-14h] [bp-3Ch]@16
  PCWSTR *string; // [sp-10h] [bp-38h]@16
  DWORD hum; // [sp-10h] [bp-38h]@32
  POBJECT_ATTRIBUTES verification = NULL; // [sp+0h] [bp-28h]@18
  LSA_UNICODE_STRING DestinationString; // [sp+18h] [bp-10h]@16
  DWORD otherNumeber; // [sp+20h] [bp-8h]@22
  DWORD number; // [sp+24h] [bp-4h]@22

  if ( nndPreferred != -1 && nndPreferred >= 0x10 )
  {
    SetLastError(0x57u);
    return 0;
  }
  protect = flProtect & 0x9D800000;
  otherProtect = flProtect & 0x9D800000 ^ flProtect;
  mask = 983045;
  if ( !(flProtect & 0x9D800000) )
    protect = 0x8000000u;
  if ( otherProtect == 4 )
  {
    mask = 983047;
  }
  else
  {
    if ( otherProtect == 64 )
    {
      mask = 983055;
    }
    else
    {
      if ( otherProtect != 32 && otherProtect != 128 )
      {
        if ( otherProtect != 2 && otherProtect != 8 )
        {
LABEL_25:
          SetLastError(0x57u);
          return 0;
        }
      }
      else
      {
        mask = 983053;
      }
    }
  }
  if ( lpName )
  {
    RtlInitUnicodeString(&DestinationString, lpName);
    string = &lpName;
    security = &DestinationString;
  }
  else
  {
    string = &lpName;
    security = 0;
  }
  status = (NTSTATUS)BaseFormatObjectAttributes(verification, lpFileMappingAttributes, security);
  if ( status < 0 )
    goto LABEL_37;
  if ( dwMaximumSizeLow || dwMaximumSizeHigh )
  {
    largeNumber = (LARGE_INTEGER *)&otherNumeber;
    otherNumeber = dwMaximumSizeLow;
    number = dwMaximumSizeHigh;
  }
  else
  {
    largeNumber = 0;
  }
  if ( FileHandle == (HANDLE)-1 )
  {
    FileHandle = 0;
    if ( !largeNumber )
      goto LABEL_25;
  }
  if ( nndPreferred != -1 )
    protect |= nndPreferred + 1;
  status = NtCreateSection(
             (PHANDLE)&flProtect,
             mask,
             (POBJECT_ATTRIBUTES)lpName,
             largeNumber,
             otherProtect,
             protect,
             FileHandle);
  if ( status < 0 )
  {
LABEL_37:
    BaseSetLastNTError(status);
    return 0;
  }
  if ( status == 0x40000000 )
    hum = 183;
  else
    hum = 0;
  SetLastError(hum);
  return (HANDLE)flProtect;
}

HANDLE WINAPI CreateFileMappingNumaA(HANDLE FileHandle, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCTSTR SourceString, DWORD nndPreferred)
{
  LPCWSTR string; // esi@1
  PUNICODE_STRING unicodeString; // esi@2
  NTSTATUS status; // eax@2
  STRING DestinationString; // [sp+4h] [bp-8h]@2

  string = 0;
  if ( !SourceString )
    return CreateFileMappingNumaW(
             FileHandle,
             lpFileMappingAttributes,
             flProtect,
             dwMaximumSizeHigh,
             dwMaximumSizeLow,
             string,
             nndPreferred);
  #ifdef _M_IX86
	unicodeString = (PUNICODE_STRING)(__readfsdword(24) + 3064);
  #elif defined(_M_AMD64)
	unicodeString = (PUNICODE_STRING)(__readgsqword(24) + 3064);
  #endif 
  RtlInitAnsiString(&DestinationString, SourceString);
  status = RtlAnsiStringToUnicodeString(unicodeString, &DestinationString, 0);
  if ( status >= 0 )
  {
    string = unicodeString->Buffer;
    return CreateFileMappingNumaW(
             FileHandle,
             lpFileMappingAttributes,
             flProtect,
             dwMaximumSizeHigh,
             dwMaximumSizeLow,
             string,
             nndPreferred);
  }
  if ( status == 0x80000005 )
    SetLastError(0xCEu);
  else
    BaseSetLastNTError(status);
  return 0;
}

BOOL WINAPI SetFileIoOverlappedRange(HANDLE FileHandle, PUCHAR OverlappedRangeStart, ULONG Length)
{
  NTSTATUS status; // eax@4
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+0h] [bp-10h]@4
  PUCHAR FileInformation; // [sp+8h] [bp-8h]@4
  ULONG localLegngth; // [sp+Ch] [bp-4h]@4

  if ( Length < 0x14 )
  {
    SetLastError(87);
    return 0;
  }
  localLegngth = Length;
  FileInformation = OverlappedRangeStart;
  status = NtSetInformationFile(
             FileHandle,
             &IoStatusBlock,
             &FileInformation,
             8u,
             FileNetworkOpenInformation|FileAccessInformation);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    return 0;
  }
  return 1;
}

PVOID WINAPI BasepInitializeFindFileHandle(HANDLE a1)
{
  PVOID hum; // eax@1
  PVOID dois; // esi@1

  
  #ifdef _M_IX86
	hum = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), BaseDllTag + 786432, 0x2Cu);
  #elif defined(_M_AMD64)
	hum = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), BaseDllTag + 786432, 0x2Cu);
  #endif  
  dois = hum;
  if ( hum )
  {
    hum = a1;
    *((DWORD *)hum + 1) = 0;
    *((DWORD *)hum + 2) = 0;
    *((DWORD *)hum + 3) = 0;
    *((DWORD *)hum + 4) = 0;
    if ( RtlInitializeCriticalSection((PRTL_CRITICAL_SECTION)((char *)hum + 20)) < 0 )
    {
      #ifdef _M_IX86
		RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, dois);
	  #elif defined(_M_AMD64)
		RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, dois);
	  #endif  
      dois = 0;
    }
  }
  return dois;
}

unsigned int WINAPI BasepBuildNameFromComponentsW(unsigned int *a1, int a2, int a3, PWCHAR Dst)
{
  unsigned int *v4; // edx@1
  unsigned int v5; // esi@1
  int v6; // eax@1
  unsigned int result; // eax@3
  unsigned int v8; // [sp+8h] [bp-4h]@1
  char v9; // [sp+17h] [bp+Bh]@1

  v4 = a1;
  v8 = 0;
  v5 = *a1;
  v9 = *a1 == 2;
  v6 = (v5 >> 1) + (v9 == 0) + *(DWORD *)(a2 + 16) + 1;
  if ( (unsigned int)v6 > *(DWORD *)a3 )
    v8 = 0x80000005u;
  *(DWORD *)a3 = v6;
  result = v8;
  if ( (v8 & 0x80000000u) == 0 )
  {
    memcpy(Dst, v4 + 1, v5);
    if ( !v9 )
    {
      *(PWCHAR)((char *)Dst + v5) = 92;
      v5 += 2;
    }
    memcpy((char *)Dst + v5, (const void *)(a2 + 20), 2 * *(DWORD *)(a2 + 16));
    *(WCHAR *)((char *)&Dst[*(DWORD *)(a2 + 16)] + v5) = 0;
    result = v8;
  }
  return result;
}

HANDLE WINAPI FindFirstFileNameW(PWSTR lpFileName, DWORD dwFlags, LPDWORD StringLength, PWCHAR LinkName)
{
  HANDLE handle; // esi@1
  HANDLE result; // eax@3
  ULONG *otherResult; // edi@12
  int seven; // edi@18
  int hum; // edi@19
  DWORD error; // [sp-4h] [bp-68h]@2
  OBJECT_ATTRIBUTES ObjectAttributes; // [sp+10h] [bp-54h]@6
  LSA_UNICODE_STRING ntname; // [sp+28h] [bp-3Ch]@4
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+30h] [bp-34h]@6
  ULONG FileInformationLength; // [sp+38h] [bp-2Ch]@1
  HANDLE a1; // [sp+3Ch] [bp-28h]@1
  HANDLE hFindFile; // [sp+40h] [bp-24h]@1
  NTSTATUS NtStatus; // [sp+44h] [bp-20h]@6
  PVOID Address; // [sp+48h] [bp-1Ch]@1
  CPPEH_RECORD ms_exc; // [sp+4Ch] [bp-18h]@6

  Address = 0;
  FileInformationLength = 96;
  a1 = (HANDLE)-1;
  handle = 0;
  hFindFile = 0;
  if ( dwFlags )
  {
    error = 87;
LAST_ERROR:
    SetLastError(error);
    return (HANDLE)-1;
  }
  if ( !RtlDosPathNameToNtPathName_U(lpFileName, &ntname, 0, 0) )
  {
    error = 3;
    goto LAST_ERROR;
  }
  ObjectAttributes.Length = 24;
  ObjectAttributes.RootDirectory = 0;
  ObjectAttributes.Attributes = 64;
  ObjectAttributes.ObjectName = &ntname;
  ObjectAttributes.SecurityDescriptor = 0;
  ObjectAttributes.SecurityQualityOfService = 0;
  ms_exc.registration.TryLevel = 0;
  result = (HANDLE)NtCreateFile(&a1, 0x100080u, &ObjectAttributes, &IoStatusBlock, 0, 0x80u, 7u, 1u, 0x200020u, 0, 0);
  NtStatus = (NTSTATUS)result;
  if ( !result )
  {
    result = BasepInitializeFindFileHandle(a1);
    handle = result;
    hFindFile = result;
    if ( result )
    {
      a1 = 0;
      do
      {
        if ( Address )
          RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, Address);
        result = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, FileInformationLength);
        otherResult = (ULONG *)result;
        Address = result;
        if ( !result )
        {
          NtStatus = 0xC000009Au;
          handle = hFindFile;
          goto EXCEPTION;
        }
        handle = hFindFile;
        result = (HANDLE)NtQueryInformationFile(
                           *(HANDLE *)hFindFile,
                           &IoStatusBlock,
                           result,
                           FileInformationLength,
                           FileNetworkOpenInformation|FileNamesInformation);
        NtStatus = (NTSTATUS)result;
        FileInformationLength = *otherResult;
      }
      while ( result == (HANDLE)0x80000005 );
      if ( !result )
      {
        if ( IoStatusBlock.Information )
        {
          handle = Address;
          *((DWORD *)handle + 3) = IoStatusBlock.Information;
          *((DWORD *)handle + 4) = IoStatusBlock.Information;
          Address = 0;
          seven = *((DWORD *)handle + 1);
          result = 0x00000000;
          NtStatus = (NTSTATUS)result;
          if ( (signed int)result >= 0 )
          {
            hum = seven + 8;
            result = (HANDLE)BasepBuildNameFromComponentsW((unsigned int *)Address, hum, (int)StringLength, LinkName);
            NtStatus = (NTSTATUS)result;
            if ( (signed int)result >= 0 )
            {
              if ( *(DWORD *)hum <= 0u )
                result = (HANDLE)(IoStatusBlock.Information + *((DWORD *)handle + 1));
              else
                result = (HANDLE)(*(DWORD *)hum + *((DWORD *)handle + 1) + 8);
              *((HANDLE *)handle + 2) = result;
            }
          }
        }
        else
        {
          NtStatus = 0xC0000011u;
        }
      }
    }
    else
    {
      NtStatus = 0xC000009Au;
    }
  }
EXCEPTION:
  ms_exc.registration.TryLevel = -2;
  if ( ntname.Length )
  {
    result = (HANDLE)RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, ntname.Buffer);
    handle = hFindFile;
  }
  if ( Address )
  {
    result = (HANDLE)RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, Address);
    handle = hFindFile;
  }
  if ( NtStatus )
  {
    BaseSetLastNTError(NtStatus);
    if ( a1 != (HANDLE)-1 )
      result = (HANDLE)NtClose(a1);
    if ( handle )
      result = (HANDLE)FindClose(handle);
  }
  return result;
}

HANDLE WINAPI FindFirstFileNameTransactedW(PWSTR lpFileName, DWORD dwFlags, LPDWORD StringLength, PWCHAR LinkName, HANDLE hTransaction)
{
  DWORD error; // [sp-4h] [bp-30h]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      FindFirstFileNameW(lpFileName, dwFlags, StringLength, LinkName);
      return (HANDLE)RtlSetCurrentTransaction(0);
    }
    error = 6725;
  }
  else
  {
    error = 6700;
  }
  SetLastError(error);
  return (HANDLE)-1;
}

/*
/*unmplemented
*/
BOOL WINAPI FindNextFileNameW(
  _In_     HANDLE hFindStream,
  _Inout_  LPDWORD StringLength,
  _Inout_  PWCHAR LinkName
)
{
	UNIMPLEMENTED;
	return TRUE;
}

HRESULT WINAPI CopyFile2(
  _In_      PCWSTR pwszExistingFileName,
  _In_      PCWSTR pwszNewFileName,
  _In_opt_  COPYFILE2_EXTENDED_PARAMETERS *pExtendedParameters
)
{
	BOOL result;
	HRESULT resp;
	result = CopyFile((PCSTR)pwszExistingFileName, (PCSTR)pwszNewFileName, FALSE);
	if(result == TRUE)
	{
		resp =  S_OK;
	}
	return resp;
}

HANDLE WINAPI CreateFile2(
  _In_      LPCWSTR lpFileName,
  _In_      DWORD dwDesiredAccess,
  _In_      DWORD dwShareMode,
  _In_      DWORD dwCreationDisposition,
  _In_opt_  LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams
)
{
	return CreateFile((PCSTR)lpFileName, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, 2 ,NULL);
}

PVOID WINAPI MapViewOfFileFromApp(HANDLE hFileMappingObject, ULONG DesiredAccess, ULONG64 FileOffset, SIZE_T NumberOfBytesToMap)
{
  ULONG localAccess; // ecx@1
  ULONG otherAccess; // esi@2
  ULONG number; // edx@2
  ULONG compose; // eax@6
  NTSTATUS status; // eax@7
  PVOID result; // eax@9
  ULONG64 SectionOffset; // [sp+0h] [bp-8h]@2

  localAccess = DesiredAccess;
  if ( DesiredAccess & 0x20 )
  {
    RtlSetLastWin32Error(87);
    result = 0;
  }
  else
  {
    SectionOffset = FileOffset;
    otherAccess = 0;
    number = 0;
    DesiredAccess = 0;
    if ( (localAccess & 0x80000000u) != 0 )
    {
      localAccess &= 0x7FFFFFFFu;
      number = 8192;
    }
    if ( localAccess == 33 )
    {
      compose = 128;
    }
    else
    {
      if ( localAccess == 1 )
      {
        compose = 8;
      }
      else
      {
        if ( localAccess & 2 )
        {
          compose = (localAccess & 0x20) != 0 ? 64 : 4;
        }
        else
        {
          if ( localAccess & 4 )
            compose = (localAccess & 0x20) != 0 ? 32 : 2;
          else
            compose = 1;
        }
      }
    }
    status = NtMapViewOfSection(
               hFileMappingObject,
               (HANDLE)0xFFFFFFFF,
               (PVOID *)&DesiredAccess,
               0,
               0,
               (PLARGE_INTEGER)&SectionOffset,
               &NumberOfBytesToMap,
               ViewShare,
               number,
               compose);
    if ( status < 0 )
      BaseSetLastNTError(status);
    else
      otherAccess = DesiredAccess;
    result = (PVOID)otherAccess;
  }
  return result;
}

HANDLE WINAPI CreateFileMappingFromApp(HANDLE hFile, PSECURITY_ATTRIBUTES SecurityAttributes, ULONG PageProtection, ULONG64 MaximumSize, PCWSTR Name)
{
  HANDLE result; // eax@6

  if ( PageProtection != 16
    && PageProtection != 32
    && PageProtection != 64
    && PageProtection != 128
    && (!(PageProtection & 0x1000000) || PageProtection & 0x11000000) )
  {
    result = (HANDLE)CreateFileMappingNumaW(
                       hFile,
                       SecurityAttributes,
                       PageProtection,
                       HIWORD(MaximumSize),
                       (DWORD)MaximumSize,
                       Name,
                       -1);
  }
  else
  {
    RtlSetLastWin32Error(87);
    result = 0;
  }
  return result;
}

BOOL WINAPI UnmapViewOfFileEx(PVOID MemoryCache, int a2)
{
  NTSTATUS otherStatus; // eax@1
  NTSTATUS status; // esi@1
  BOOL result; // eax@2

  otherStatus = NtUnmapViewOfSectionEx((HANDLE)-1, MemoryCache, a2);
  status = otherStatus;
  if ( otherStatus >= 0
    || otherStatus == 0xC0000045
    && RtlFlushSecureMemoryCache(MemoryCache, 0)
    && (status = NtUnmapViewOfSectionEx((HANDLE)-1, MemoryCache, a2), status >= 0) )
  {
    result = 1;
  }
  else
  {
    BaseSetLastNTError(status);
    result = 0;
  }
  return result;
}

BOOL WINAPI GetFileAttributesByHandle( 	
		IN HANDLE  	hFile,
		OUT LPDWORD  	dwFileAttributes,
		IN DWORD  	dwFlags 
) 		
{
    FILE_BASIC_INFORMATION FileBasic;
    IO_STATUS_BLOCK IoStatusBlock;
    NTSTATUS Status;

    UNREFERENCED_PARAMETER(dwFlags);

    Status = NtQueryInformationFile(hFile,
                                    &IoStatusBlock,
                                    &FileBasic,
                                    sizeof(FileBasic),
                                    FileBasicInformation);
    if (NT_SUCCESS(Status))
    {
        *dwFileAttributes = FileBasic.FileAttributes;
        return TRUE;
    }

    BaseSetLastNTError(Status);
    return FALSE;
}

BOOL WINAPI SetFileAttributesByHandle( 	
		IN HANDLE  	hFile,
		IN DWORD  	dwFileAttributes,
		IN DWORD  	dwFlags 
) 		
{
    FILE_BASIC_INFORMATION FileBasic;
    IO_STATUS_BLOCK IoStatusBlock;
    NTSTATUS Status;

    UNREFERENCED_PARAMETER(dwFlags);

    Status = NtQueryInformationFile(hFile,
                                    &IoStatusBlock,
                                    &FileBasic,
                                    sizeof(FileBasic),
                                    FileBasicInformation);
    if (NT_SUCCESS(Status))
    {
        FileBasic.FileAttributes = dwFileAttributes;

        Status = NtSetInformationFile(hFile,
                                      &IoStatusBlock,
                                      &FileBasic,
                                      sizeof(FileBasic),
                                      FileBasicInformation);
    }

    if (!NT_SUCCESS(Status))
    {
        BaseSetLastNTError(Status);
        return FALSE;
    }

    return TRUE;
}

BOOL WINAPI SetEndOfFileEx(HANDLE FileHandle, struct _IO_STATUS_BLOCK IoStatusBlock)
{
  NTSTATUS status; // eax@4
  NTSTATUS otherStatus; // [sp-4h] [bp-Ch]@2
  NTSTATUS FileInformation; // [sp+0h] [bp-8h]@4
  ULONG information; // [sp+4h] [bp-4h]@4

  if ( ((unsigned int)FileHandle & 0x10000003) == 3 )
  {
    otherStatus = 0xC0000008u;
LABEL_3:
    BaseSetLastNTError(otherStatus);
    return 0;
  }
  FileInformation = IoStatusBlock.Status;
  information = IoStatusBlock.Information;
  status = NtSetInformationFile(FileHandle, &IoStatusBlock, &FileInformation, 8u, FileEndOfFileInformation);
  if ( status < 0 )
  {
    otherStatus = status;
    goto LABEL_3;
  }
  return 1;
}

BOOL WINAPI SetFileCompletionNotificationModes(HANDLE FileHandle, int FileInformation)
{
  NTSTATUS status; // eax@3
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+0h] [bp-8h]@3

  if ( FileInformation & 0xFC )
  {
    RtlSetLastWin32Error(87);
  }
  else
  {
    if ( !(FileInformation & 3)
      || (FileInformation = (unsigned __int8)FileInformation,
          status = NtSetInformationFile(
                     FileHandle,
                     &IoStatusBlock,
                     &FileInformation,
                     4u,
                     FileInformationReserved33|FileAccessInformation),
          status >= 0) )
      return 1;
    BaseSetLastNTError(status);
  }
  return 0;
}