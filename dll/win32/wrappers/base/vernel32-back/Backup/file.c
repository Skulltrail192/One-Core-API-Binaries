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

static BOOL oem_file_apis;

/*
 * @implemented
 */
BOOL 
WINAPI 
SetFileBandwidthReservation(
	HANDLE hFile, 
	DWORD nPeriodMilliseconds, 
	DWORD nBytesPerPeriod, 
	BOOL bDiscardable, 
	LPDWORD lpTransferSize, 
	LPDWORD lpNumOutstandingRequests
)
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
BOOL 
WINAPI 
GetFileBandwidthReservation(
	HANDLE hFile, 
	LPDWORD lpPeriodMilliseconds, 
	LPDWORD lpBytesPerPeriod, 
	LPBOOL pDiscardable, 
	LPDWORD lpTransferSize, 
	LPDWORD lpNumOutstandingRequests
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  DWORD localSize; // eax@4
  DWORD localRequest; // edx@4
  int FileInformation; // [sp+0h] [bp-1Ch]@1
  DWORD localLpPeriodMilliseconds = *lpPeriodMilliseconds; // [sp+4h] [bp-18h]@4
  DWORD localpDiscardable = *pDiscardable; // [sp+9h] [bp-13h]@4
  DWORD localLpTransferSize = *lpTransferSize; // [sp+Ch] [bp-10h]@4
  DWORD localLpNumOutstandingRequests = *lpNumOutstandingRequests; // [sp+10h] [bp-Ch]@4
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
/***********************************************************************
*             GetFileInformationByHandleEx (KERNEL32.@)
*/
BOOL WINAPI GetFileInformationByHandleEx( HANDLE handle, FILE_INFO_BY_HANDLE_CLASS class,
                                          LPVOID info, DWORD size )
{
    NTSTATUS status;
    IO_STATUS_BLOCK io;

    switch (class)
    {
    case FileStreamInfo:
    case FileCompressionInfo:
    case FileAttributeTagInfo:
    case FileRemoteProtocolInfo:
    case FileFullDirectoryInfo:
    case FileFullDirectoryRestartInfo:
    case FileStorageInfo:
    case FileAlignmentInfo:
    case FileIdInfo:
    case FileIdExtdDirectoryInfo:
    case FileIdExtdDirectoryRestartInfo:
        FIXME( "%p, %u, %p, %u\n", handle, class, info, size );
        SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
        return FALSE;

    case FileBasicInfo:
        status = NtQueryInformationFile( handle, &io, info, size, FileBasicInformation );
        break;

    case FileStandardInfo:
        status = NtQueryInformationFile( handle, &io, info, size, FileStandardInformation );
        break;

    case FileNameInfo:
        status = NtQueryInformationFile( handle, &io, info, size, FileNameInformation );
        break;

    case FileIdBothDirectoryRestartInfo:
    case FileIdBothDirectoryInfo:
        status = NtQueryDirectoryFile( handle, NULL, NULL, NULL, &io, info, size,
                                       FileIdBothDirectoryInformation, FALSE, NULL,
                                       (class == FileIdBothDirectoryRestartInfo) );
        break;

    case FileRenameInfo:
    case FileDispositionInfo:
    case FileAllocationInfo:
    case FileIoPriorityHintInfo:
    case FileEndOfFileInfo:
    default:
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }

    if (status != STATUS_SUCCESS)
    {
        SetLastError( RtlNtStatusToDosError( status ) );
        return FALSE;
    }
    return TRUE;
}
/*
 * @implemented - new (CHANGE URGENCY!!!)
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

/***********************************************************************
 *           K32GetDeviceDriverFileNameW (KERNEL32.@)
 */
DWORD WINAPI K32GetDeviceDriverFileNameW(void *image_base, LPWSTR file_name, DWORD size)
{
    FIXME("(%p, %p, %d): stub\n", image_base, file_name, size);

    if (file_name && size)
        file_name[0] = '\0';

    return 0;
}

/***********************************************************************
 *           GetFinalPathNameByHandleW (KERNEL32.@)
 */
DWORD WINAPI GetFinalPathNameByHandleW(HANDLE file, LPWSTR path, DWORD charcount, DWORD flags)
{
    WCHAR buffer[sizeof(OBJECT_NAME_INFORMATION) + MAX_PATH + 1];
    OBJECT_NAME_INFORMATION *info = (OBJECT_NAME_INFORMATION*)&buffer;
    WCHAR drive_part[MAX_PATH];
    DWORD drive_part_len = 0;
    NTSTATUS status;
    DWORD result = 0;
    ULONG dummy;
    WCHAR *ptr;

    TRACE( "(%p,%p,%d,%x)\n", file, path, charcount, flags );

    if (flags & ~(FILE_NAME_OPENED | VOLUME_NAME_GUID | VOLUME_NAME_NONE | VOLUME_NAME_NT))
    {
        WARN("Unknown flags: %x\n", flags);
        SetLastError( ERROR_INVALID_PARAMETER );
        return 0;
    }

    /* get object name */
    status = NtQueryObject( file, ObjectNameInformation, &buffer, sizeof(buffer) - sizeof(WCHAR), &dummy );
    if (status != STATUS_SUCCESS)
    {
        SetLastError( RtlNtStatusToDosError( status ) );
        return 0;
    }
    if (!info->Name.Buffer)
    {
        SetLastError( ERROR_INVALID_HANDLE );
        return 0;
    }
    if (info->Name.Length < 4 * sizeof(WCHAR) || info->Name.Buffer[0] != '\\' ||
             info->Name.Buffer[1] != '?' || info->Name.Buffer[2] != '?' || info->Name.Buffer[3] != '\\' )
    {
        FIXME("Unexpected object name: %s\n", debugstr_wn(info->Name.Buffer, info->Name.Length / sizeof(WCHAR)));
        SetLastError( ERROR_GEN_FAILURE );
        return 0;
    }

    /* add terminating null character, remove "\\??\\" */
    info->Name.Buffer[info->Name.Length / sizeof(WCHAR)] = 0;
    info->Name.Length -= 4 * sizeof(WCHAR);
    info->Name.Buffer += 4;

    /* FILE_NAME_OPENED is not supported yet, and would require Wineserver changes */
    if (flags & FILE_NAME_OPENED)
    {
        FIXME("FILE_NAME_OPENED not supported\n");
        flags &= ~FILE_NAME_OPENED;
    }

    /* Get information required for VOLUME_NAME_NONE, VOLUME_NAME_GUID and VOLUME_NAME_NT */
    if (flags == VOLUME_NAME_NONE || flags == VOLUME_NAME_GUID || flags == VOLUME_NAME_NT)
    {
        if (!GetVolumePathNameW( info->Name.Buffer, drive_part, MAX_PATH ))
            return 0;

        drive_part_len = strlenW(drive_part);
        if (!drive_part_len || drive_part_len > strlenW(info->Name.Buffer) ||
                drive_part[drive_part_len-1] != '\\' ||
                strncmpiW( info->Name.Buffer, drive_part, drive_part_len ))
        {
            FIXME("Path %s returned by GetVolumePathNameW does not match file path %s\n",
                debugstr_w(drive_part), debugstr_w(info->Name.Buffer));
            SetLastError( ERROR_GEN_FAILURE );
            return 0;
        }
    }

    if (flags == VOLUME_NAME_NONE)
    {
        ptr    = info->Name.Buffer + drive_part_len - 1;
        result = strlenW(ptr);
        if (result < charcount)
            memcpy(path, ptr, (result + 1) * sizeof(WCHAR));
        else result++;
    }
    else if (flags == VOLUME_NAME_GUID)
    {
        WCHAR volume_prefix[51];

        /* GetVolumeNameForVolumeMountPointW sets error code on failure */
        if (!GetVolumeNameForVolumeMountPointW( drive_part, volume_prefix, 50 ))
            return 0;

        ptr    = info->Name.Buffer + drive_part_len;
        result = strlenW(volume_prefix) + strlenW(ptr);
        if (result < charcount)
        {
            path[0] = 0;
            strcatW(path, volume_prefix);
            strcatW(path, ptr);
        }
        else
        {
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            result++;
        }
    }
    else if (flags == VOLUME_NAME_NT)
    {
        WCHAR nt_prefix[MAX_PATH];

        /* QueryDosDeviceW sets error code on failure */
        drive_part[drive_part_len - 1] = 0;
        if (!QueryDosDeviceW( drive_part, nt_prefix, MAX_PATH ))
            return 0;

        ptr    = info->Name.Buffer + drive_part_len - 1;
        result = strlenW(nt_prefix) + strlenW(ptr);
        if (result < charcount)
        {
            path[0] = 0;
            strcatW(path, nt_prefix);
            strcatW(path, ptr);
        }
        else
        {
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            result++;
        }
    }
    else if (flags == VOLUME_NAME_DOS)
    {
        static const WCHAR dos_prefix[] = {'\\','\\','?','\\', '\0'};

        result = strlenW(dos_prefix) + strlenW(info->Name.Buffer);
        if (result < charcount)
        {
            path[0] = 0;
            strcatW(path, dos_prefix);
            strcatW(path, info->Name.Buffer);
        }
        else
        {
            SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            result++;
        }
    }
    else
    {
        /* Windows crashes here, but we prefer returning ERROR_INVALID_PARAMETER */
        WARN("Invalid combination of flags: %x\n", flags);
        SetLastError( ERROR_INVALID_PARAMETER );
    }

    return result;
}

/***********************************************************************
 *           GetFinalPathNameByHandleA (KERNEL32.@)
 */
DWORD WINAPI GetFinalPathNameByHandleA(HANDLE file, LPSTR path, DWORD charcount, DWORD flags)
{
    WCHAR *str;
    DWORD result, len, cp;

    TRACE( "(%p,%p,%d,%x)\n", file, path, charcount, flags);

    len = GetFinalPathNameByHandleW(file, NULL, 0, flags);
    if (len == 0)
        return 0;

    str = HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
    if (!str)
    {
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return 0;
    }

    result = GetFinalPathNameByHandleW(file, str, len, flags);
    if (result != len - 1)
    {
        HeapFree(GetProcessHeap(), 0, str);
        WARN("GetFinalPathNameByHandleW failed unexpectedly: %u\n", result);
        return 0;
    }

    cp = oem_file_apis ? CP_OEMCP : CP_ACP;

    len = WideCharToMultiByte(cp, 0, str, -1, NULL, 0, NULL, NULL);
    if (!len)
    {
        HeapFree(GetProcessHeap(), 0, str);
        WARN("Failed to get multibyte length\n");
        return 0;
    }

    if (charcount < len)
    {
        HeapFree(GetProcessHeap(), 0, str);
        return len - 1;
    }

    len = WideCharToMultiByte(cp, 0, str, -1, path, charcount, NULL, NULL);
    if (!len)
    {
        HeapFree(GetProcessHeap(), 0, str);
        WARN("WideCharToMultiByte failed\n");
        return 0;
    }

    HeapFree(GetProcessHeap(), 0, str);

    return len - 1;
}

/***********************************************************************
 *           SetFileInformationByHandle   (KERNEL32.@)
 */
BOOL WINAPI SetFileInformationByHandle( HANDLE file, FILE_INFO_BY_HANDLE_CLASS class, VOID *info, DWORD size )
{
    NTSTATUS status;
    IO_STATUS_BLOCK io;

    TRACE( "%p %u %p %u\n", file, class, info, size );

    switch (class)
    {
    case FileBasicInfo:
    case FileNameInfo:
    case FileRenameInfo:
    case FileAllocationInfo:
    case FileEndOfFileInfo:
    case FileStreamInfo:
    case FileIdBothDirectoryInfo:
    case FileIdBothDirectoryRestartInfo:
    case FileIoPriorityHintInfo:
    case FileFullDirectoryInfo:
    case FileFullDirectoryRestartInfo:
    case FileStorageInfo:
    case FileAlignmentInfo:
    case FileIdInfo:
    case FileIdExtdDirectoryInfo:
    case FileIdExtdDirectoryRestartInfo:
        FIXME( "%p, %u, %p, %u\n", file, class, info, size );
        SetLastError( ERROR_CALL_NOT_IMPLEMENTED );
        return FALSE;

    case FileDispositionInfo:
        status = NtSetInformationFile( file, &io, info, size, FileDispositionInformation );
        break;

    case FileStandardInfo:
    case FileCompressionInfo:
    case FileAttributeTagInfo:
    case FileRemoteProtocolInfo:
    default:
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }

    if (status != STATUS_SUCCESS)
    {
        SetLastError( RtlNtStatusToDosError( status ) );
        return FALSE;
    }
    return TRUE;
}

/*
 * @implemented - new
 */
ULONG 
WINAPI 
GetModuleFileNameFromHandleNoLock(
	HMODULE hModule, 
	LPWSTR lpFilename, 
	DWORD nSize
)
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
BOOL 
WINAPI 
CancelSynchronousIo(
	_In_  HANDLE hFile
)
{
  NTSTATUS status; // eax@3
  BOOL result; // eax@4
  PIO_STATUS_BLOCK localLpOverlapped = NULL;

  status = NtCancelIoFile(hFile, localLpOverlapped);
  if ( status < 0 )
  {
    SetLastError(status);
    result = FALSE;
  }
  else
  {
    result = TRUE;
  }
  return result;
}

/*
 * @implemented - new
 */
BOOL 
WINAPI 
CancelIoEx(
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
    result = FALSE;
  }
  else
  {
    result = TRUE;
  }
  return result;
}
//Change Urgency!!!
HANDLE 
WINAPI 
CreateFileMappingNumaW(HANDLE FileHandle, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, PCWSTR lpName, DWORD nndPreferred)
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
//Change Urgency!!!
BOOL
WINAPI 
SetFileIoOverlappedRange(HANDLE FileHandle, PUCHAR OverlappedRangeStart, ULONG Length)
{
  NTSTATUS status; // eax@4
  struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+0h] [bp-10h]@4
  PUCHAR FileInformation; // [sp+8h] [bp-8h]@4
  ULONG localLegngth; // [sp+Ch] [bp-4h]@4

  if ( Length < 0x14 )
  {
    SetLastError(87);
    return FALSE;
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
    return FALSE;
  }
  return TRUE;
}
//Reimplement!!!!
PVOID 
WINAPI 
BasepInitializeFindFileHandle(HANDLE a1)
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

unsigned int 
WINAPI 
BasepBuildNameFromComponentsW(unsigned int *a1, int a2, int a3, PWCHAR Dst)
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
//Change Urgency 
HANDLE 
WINAPI 
FindFirstFileNameW(PWSTR lpFileName, DWORD dwFlags, LPDWORD StringLength, PWCHAR LinkName)
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
		#ifdef _M_IX86
			RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, Address);
			result = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, FileInformationLength);
		#elif defined(_M_AMD64)
			RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, Address);
			result = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, FileInformationLength);
		#endif
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
    #ifdef _M_IX86
		result = (HANDLE)RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, ntname.Buffer);
	#elif defined(_M_AMD64)
		result = (HANDLE)RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, ntname.Buffer);
	#endif
    handle = hFindFile;
  }
  if ( Address )
  {
	#ifdef _M_IX86
		result = (HANDLE)RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, Address);
	#elif defined(_M_AMD64)
		result = (HANDLE)RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, Address);
	#endif
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

HANDLE 
WINAPI 
FindFirstFileNameTransactedW(PWSTR lpFileName, DWORD dwFlags, LPDWORD StringLength, PWCHAR LinkName, HANDLE hTransaction)
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
BOOL 
WINAPI 
FindNextFileNameW(
  _In_     HANDLE hFindStream,
  _Inout_  LPDWORD StringLength,
  _Inout_  PWCHAR LinkName
)
{
	UNIMPLEMENTED;
	return TRUE;
}

HRESULT 
WINAPI 
CopyFile2(
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

HANDLE 
WINAPI 
CreateFile2(
  _In_      LPCWSTR lpFileName,
  _In_      DWORD dwDesiredAccess,
  _In_      DWORD dwShareMode,
  _In_      DWORD dwCreationDisposition,
  _In_opt_  LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams
)
{
	return CreateFile((PCSTR)lpFileName, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, 2 ,NULL);
}

PVOID 
WINAPI 
MapViewOfFileFromApp(
	HANDLE hFileMappingObject, 
	ULONG DesiredAccess, 
	ULONG64 FileOffset, 
	SIZE_T NumberOfBytesToMap
)
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

HANDLE 
WINAPI 
CreateFileMappingFromApp(
	HANDLE hFile, 
	PSECURITY_ATTRIBUTES SecurityAttributes, 
	ULONG PageProtection, 
	ULONG64 MaximumSize, 
	PCWSTR Name
)
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

BOOL 
WINAPI 
UnmapViewOfFileEx(
	PVOID MemoryCache, 
	int a2
)
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

BOOL 
WINAPI 
GetFileAttributesByHandle( 	
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

BOOL 
WINAPI 
SetFileAttributesByHandle( 	
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

BOOL 
WINAPI 
SetEndOfFileEx(
	HANDLE FileHandle, 
	struct _IO_STATUS_BLOCK IoStatusBlock
)
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

BOOL 
WINAPI 
SetSystemFileCacheSize(
	SIZE_T MinimumFileCacheSize, 
	SIZE_T MaximumFileCacheSize, 
	int Flags
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  char SystemInformation; // [sp+0h] [bp-24h]@1

  status = NtSetSystemInformation(SystemObjectInformation|0x40, &SystemInformation, 0x24u);
  if ( status < 0 )
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  else
  {
    result = TRUE;
  }
  return result;
}

HANDLE
WINAPI
ReOpenFile(
    HANDLE  hOriginalFile,
    DWORD   dwDesiredAccess,
    DWORD   dwShareMode,
    DWORD   dwFlags
    )
/*++

Routine Description:

    This API allows an application to reopen a file with different access, share modes
    and flags given an already open handle. This API should be used if the application wants
    to ensure that the original file does not go away but wants to reopen it with 

Arguments:

    hOriginalFile - Supplies the handle to the original file relative to which 
        we open a new handle.

    dwDesiredAccess - Supplies the caller's desired access to the file. Any combination of 
        flags can be passed in (like FILE_READ_ATTRIBUTES)

        Possible DesiredAccess Flags:

        GENERIC_READ - Read access to the file is requested.  This
            allows data to be read from the file and the file pointer to
            be modified.

        GENERIC_WRITE - Write access to the file is requested.  This
            allows data to be written to the file and the file pointer to
            be modified.


    dwShareMode - Supplies a set of flags that indicates how this file is
        to be shared with other openers of the file.  A value of zero
        for this parameter indicates no sharing of the file, or
        exclusive access to the file is to occur.

        ShareMode Flags:

        FILE_SHARE_READ - Other open operations may be performed on the
            file for read access.

        FILE_SHARE_WRITE - Other open operations may be performed on the
            file for write access.

        FILE_SHARE_DELETE - Other open operations may be performed on the
            file for delete access.

    dwFlags - Specifies flags and attributes for the file.

        The attributes are not accepted by this API as they are used only for Creating a file
        This API reopens an existing file. All FILE_ATTRIBUTE_* flags are not allowed.
        
        dwFlagsAndAttributes Flags:


        FILE_FLAG_WRITE_THROUGH - Indicates that the system should
            always write through any intermediate cache and go directly
            to the file.  The system may still cache writes, but may not
            lazily flush the writes.

        FILE_FLAG_OVERLAPPED - Indicates that the system should initialize
            the file so that ReadFile and WriteFile operations that may
            take a significant time to complete will return ERROR_IO_PENDING.
            An event will be set to the signalled state when the operation
            completes. When FILE_FLAG_OVERLAPPED is specified the system will
            not maintain the file pointer. The position to read/write from
            is passed to the system as part of the OVERLAPPED structure
            which is an optional parameter to ReadFile and WriteFile.

        FILE_FLAG_NO_BUFFERING - Indicates that the file is to be opened
            with no intermediate buffering or caching done by the
            system.  Reads and writes to the file must be done on sector
            boundries.  Buffer addresses for reads and writes must be
            aligned on at least disk sector boundries in memory.

        FILE_FLAG_RANDOM_ACCESS - Indicates that access to the file may
            be random. The system cache manager may use this to influence
            its caching strategy for this file.

        FILE_FLAG_SEQUENTIAL_SCAN - Indicates that access to the file
            may be sequential.  The system cache manager may use this to
            influence its caching strategy for this file.  The file may
            in fact be accessed randomly, but the cache manager may
            optimize its cacheing policy for sequential access.

        FILE_FLAG_DELETE_ON_CLOSE - Indicates that the file is to be
            automatically deleted when the last handle to it is closed.

        FILE_FLAG_BACKUP_SEMANTICS - Indicates that the file is being opened
            or created for the purposes of either a backup or a restore
            operation.  Thus, the system should make whatever checks are
            appropriate to ensure that the caller is able to override
            whatever security checks have been placed on the file to allow
            this to happen.

        FILE_FLAG_POSIX_SEMANTICS - Indicates that the file being opened
            should be accessed in a manner compatible with the rules used
            by POSIX.  This includes allowing multiple files with the same
            name, differing only in case.  WARNING:  Use of this flag may
            render it impossible for a DOS, WIN-16, or WIN-32 application
            to access the file.

        FILE_FLAG_OPEN_REPARSE_POINT - Indicates that the file being opened
            should be accessed as if it were a reparse point.  WARNING:  Use
            of this flag may inhibit the operation of file system filter drivers
            present in the I/O subsystem.

        FILE_FLAG_OPEN_NO_RECALL - Indicates that all the state of the file
            should be acessed without changing its storage location.  Thus,
            in the case of files that have parts of its state stored at a
            remote servicer, no permanent recall of data is to happen.

    Security Quality of Service information may also be specified in
        the dwFlagsAndAttributes parameter.  These bits are meaningful
        only if the file being opened is the client side of a Named
        Pipe.  Otherwise they are ignored.

        SECURITY_SQOS_PRESENT - Indicates that the Security Quality of
            Service bits contain valid values.

    Impersonation Levels:

        SECURITY_ANONYMOUS - Specifies that the client should be impersonated
            at Anonymous impersonation level.

        SECURITY_IDENTIFICAION - Specifies that the client should be impersonated
            at Identification impersonation level.

        SECURITY_IMPERSONATION - Specifies that the client should be impersonated
            at Impersonation impersonation level.

        SECURITY_DELEGATION - Specifies that the client should be impersonated
            at Delegation impersonation level.

    Context Tracking:

        SECURITY_CONTEXT_TRACKING - A boolean flag that when set,
            specifies that the Security Tracking Mode should be
            Dynamic, otherwise Static.

        SECURITY_EFFECTIVE_ONLY - A boolean flag indicating whether
            the entire security context of the client is to be made
            available to the server or only the effective aspects of
            the context.

Return Value:

    Not -1 - Returns an open handle to the specified file.  Subsequent
        access to the file is controlled by the DesiredAccess parameter.

    0xffffffff - The operation failed. Extended error status is available
        using GetLastError.

--*/
{
    ULONG CreateFlags = 0;
    ULONG CreateDisposition;
    NTSTATUS Status;
    OBJECT_ATTRIBUTES Obja;
    HANDLE Handle;
    IO_STATUS_BLOCK IoStatusBlock;
    DWORD SQOSFlags;
    SECURITY_IMPERSONATION_LEVEL ImpersonationLevel = 0;
    SECURITY_QUALITY_OF_SERVICE SecurityQualityOfService;
    UNICODE_STRING  FileName;

    //
    // Don't support console handles.
    //

    if (((ULONG)hOriginalFile & 0x10000003) == 3 ){
        BaseSetLastNTError(STATUS_INVALID_HANDLE);
        return INVALID_HANDLE_VALUE;
    }

    //
    // The attributes are useless as this reopen of an existing file.
    //

    if (dwFlags &  FILE_ATTRIBUTE_VALID_FLAGS) {
        BaseSetLastNTError(STATUS_INVALID_PARAMETER);
        return INVALID_HANDLE_VALUE;
    }

    //
    // Initialize all the create flags from the Attribute flags.
    //

    CreateFlags |= (dwFlags & FILE_FLAG_NO_BUFFERING ? FILE_NO_INTERMEDIATE_BUFFERING : 0 );
    CreateFlags |= (dwFlags & FILE_FLAG_WRITE_THROUGH ? FILE_WRITE_THROUGH : 0 );
    CreateFlags |= (dwFlags & FILE_FLAG_OVERLAPPED ? 0 : FILE_SYNCHRONOUS_IO_NONALERT );
    CreateFlags |= (dwFlags & FILE_FLAG_SEQUENTIAL_SCAN ? FILE_SEQUENTIAL_ONLY : 0 );
    CreateFlags |= (dwFlags & FILE_FLAG_RANDOM_ACCESS ? FILE_RANDOM_ACCESS : 0 );
    CreateFlags |= (dwFlags & FILE_FLAG_BACKUP_SEMANTICS ? FILE_OPEN_FOR_BACKUP_INTENT : 0 );
    CreateFlags |= (dwFlags & FILE_FLAG_OPEN_REPARSE_POINT ? FILE_OPEN_REPARSE_POINT : 0 );
    CreateFlags |= (dwFlags & FILE_FLAG_OPEN_NO_RECALL ? FILE_OPEN_NO_RECALL : 0 );

    if ( dwFlags & FILE_FLAG_DELETE_ON_CLOSE ) {
        CreateFlags |= FILE_DELETE_ON_CLOSE;
        dwDesiredAccess |= DELETE;
        }

    CreateFlags |= FILE_NON_DIRECTORY_FILE;
    CreateDisposition = FILE_OPEN;

    RtlInitUnicodeString( &FileName, L"");
    
    //
    // Pass a NULL name relative to the original handle.
    //

    InitializeObjectAttributes(
        &Obja,
        &FileName,  
        dwFlags & FILE_FLAG_POSIX_SEMANTICS ? 0 : OBJ_CASE_INSENSITIVE,
        hOriginalFile,  // Related handle
        NULL
        );

    SQOSFlags = dwFlags & SECURITY_VALID_SQOS_FLAGS;

    if ( SQOSFlags & SECURITY_SQOS_PRESENT ) {

        SQOSFlags &= ~SECURITY_SQOS_PRESENT;

        if (SQOSFlags & SECURITY_CONTEXT_TRACKING) {

            SecurityQualityOfService.ContextTrackingMode = (SECURITY_CONTEXT_TRACKING_MODE) TRUE;
            SQOSFlags &= ~SECURITY_CONTEXT_TRACKING;

        } else {

            SecurityQualityOfService.ContextTrackingMode = (SECURITY_CONTEXT_TRACKING_MODE) FALSE;
        }

        if (SQOSFlags & SECURITY_EFFECTIVE_ONLY) {

            SecurityQualityOfService.EffectiveOnly = TRUE;
            SQOSFlags &= ~SECURITY_EFFECTIVE_ONLY;

        } else {

            SecurityQualityOfService.EffectiveOnly = FALSE;
        }

        SecurityQualityOfService.ImpersonationLevel = SQOSFlags >> 16;


    } else {

        SecurityQualityOfService.ContextTrackingMode = SECURITY_DYNAMIC_TRACKING;
        SecurityQualityOfService.ImpersonationLevel = SecurityImpersonation;
        SecurityQualityOfService.EffectiveOnly = TRUE;
    }

    SecurityQualityOfService.Length = sizeof( SECURITY_QUALITY_OF_SERVICE );
    Obja.SecurityQualityOfService = &SecurityQualityOfService;

    Status = NtCreateFile(
                &Handle,
                (ACCESS_MASK)dwDesiredAccess | SYNCHRONIZE | FILE_READ_ATTRIBUTES,
                &Obja,
                &IoStatusBlock,
                NULL,
                0,
                dwShareMode,
                CreateDisposition,
                CreateFlags,
                NULL,
                0
                );

    if ( !NT_SUCCESS(Status) ) {
        BaseSetLastNTError(Status);
        return INVALID_HANDLE_VALUE;
    }

    SetLastError(0);

    return Handle;
}
