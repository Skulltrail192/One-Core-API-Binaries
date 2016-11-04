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

/*
 * @implemented - fazer versão x64
 */
HANDLE 
WINAPI 
GetCurrentTransaction()
{
  return RtlGetCurrentTransaction();
}

/*
 * @implemented - fazer versão x64
 */
BOOL 
WINAPI 
SetCurrentTransaction(
	HANDLE new_transaction
)
{
  return RtlSetCurrentTransaction(new_transaction);
}

/*
 * @implemented
 */
BOOLEAN
WINAPI
CreateSymbolicLinkW(IN LPCWSTR lpSymlinkFileName,
                    IN LPCWSTR lpTargetFileName,
                    IN DWORD dwFlags)
{
    IO_STATUS_BLOCK IoStatusBlock;
    OBJECT_ATTRIBUTES ObjectAttributes;
    HANDLE hSymlink = NULL;
    UNICODE_STRING SymlinkFileName = { 0, 0, NULL };
    UNICODE_STRING TargetFileName = { 0, 0, NULL };
    BOOLEAN bAllocatedTarget = FALSE, bRelativePath = FALSE;
    LPWSTR lpTargetFullFileName = NULL;
    SIZE_T cbPrintName;
    SIZE_T cbReparseData;
    PREPARSE_DATA_BUFFER pReparseData = NULL;
    PBYTE pBufTail;
    NTSTATUS Status;
    ULONG dwCreateOptions;
    DWORD dwErr;

    if(!lpSymlinkFileName || !lpTargetFileName || (dwFlags | SYMBOLIC_LINK_FLAG_DIRECTORY) != SYMBOLIC_LINK_FLAG_DIRECTORY)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if(dwFlags & SYMBOLIC_LINK_FLAG_DIRECTORY)
        dwCreateOptions = FILE_DIRECTORY_FILE;
    else
        dwCreateOptions = FILE_NON_DIRECTORY_FILE;

    switch(RtlDetermineDosPathNameType_U(lpTargetFileName))
    {
    case RtlPathTypeUnknown:
    case RtlPathTypeRooted:
    case RtlPathTypeRelative:
        bRelativePath = TRUE;
        RtlInitUnicodeString(&TargetFileName, lpTargetFileName);
        break;

    case RtlPathTypeDriveRelative:
        {
            LPWSTR FilePart;
            SIZE_T cchTargetFullFileName;

            cchTargetFullFileName = GetFullPathNameW(lpTargetFileName, 0, NULL, &FilePart);

            if(cchTargetFullFileName == 0)
            {
                dwErr = GetLastError();
                goto Cleanup;
            }

            lpTargetFullFileName = RtlAllocateHeap(RtlGetProcessHeap(), 0, cchTargetFullFileName * sizeof(WCHAR));

            if(lpTargetFullFileName == NULL)
            {
                dwErr = ERROR_NOT_ENOUGH_MEMORY;
                goto Cleanup;
            }

            if(GetFullPathNameW(lpTargetFileName, cchTargetFullFileName, lpTargetFullFileName, &FilePart) == 0)
            {
                dwErr = GetLastError();
                goto Cleanup;
            }
        }

        lpTargetFileName = lpTargetFullFileName;

        // fallthrough

    case RtlPathTypeUncAbsolute:
    case RtlPathTypeDriveAbsolute:
    case RtlPathTypeLocalDevice:
    case RtlPathTypeRootLocalDevice:
    default:
        if(!RtlDosPathNameToNtPathName_U(lpTargetFileName, &TargetFileName, NULL, NULL))
        {
            bAllocatedTarget = TRUE;
            dwErr = ERROR_INVALID_PARAMETER;
            goto Cleanup;
        }
    }

    cbPrintName = wcslen(lpTargetFileName) * sizeof(WCHAR);
    cbReparseData = FIELD_OFFSET(REPARSE_DATA_BUFFER, SymbolicLinkReparseBuffer.PathBuffer) + TargetFileName.Length + cbPrintName;
    pReparseData = RtlAllocateHeap(RtlGetProcessHeap(), 0, cbReparseData);

    if(pReparseData == NULL)
    {
        dwErr = ERROR_NOT_ENOUGH_MEMORY;
        goto Cleanup;
    }

    pBufTail = (PBYTE)(pReparseData->SymbolicLinkReparseBuffer.PathBuffer);

    pReparseData->ReparseTag = (ULONG)IO_REPARSE_TAG_SYMLINK;
    pReparseData->ReparseDataLength = (USHORT)cbReparseData - REPARSE_DATA_BUFFER_HEADER_SIZE;
    pReparseData->Reserved = 0;

    pReparseData->SymbolicLinkReparseBuffer.SubstituteNameOffset = 0;
    pReparseData->SymbolicLinkReparseBuffer.SubstituteNameLength = TargetFileName.Length;
    pBufTail += pReparseData->SymbolicLinkReparseBuffer.SubstituteNameOffset;
    RtlCopyMemory(pBufTail, TargetFileName.Buffer, TargetFileName.Length);

    pReparseData->SymbolicLinkReparseBuffer.PrintNameOffset = pReparseData->SymbolicLinkReparseBuffer.SubstituteNameLength;
    pReparseData->SymbolicLinkReparseBuffer.PrintNameLength = (USHORT)cbPrintName;
    pBufTail += pReparseData->SymbolicLinkReparseBuffer.PrintNameOffset;
    RtlCopyMemory(pBufTail, lpTargetFileName, cbPrintName);

    pReparseData->SymbolicLinkReparseBuffer.Flags = 0;

    if(bRelativePath)
        pReparseData->SymbolicLinkReparseBuffer.Flags |= 1; // TODO! give this lone flag a name

    if(!RtlDosPathNameToNtPathName_U(lpSymlinkFileName, &SymlinkFileName, NULL, NULL))
    {
        dwErr = ERROR_PATH_NOT_FOUND;
        goto Cleanup;
    }

    InitializeObjectAttributes(&ObjectAttributes, &SymlinkFileName, OBJ_CASE_INSENSITIVE, NULL, NULL);

    Status = NtCreateFile
    (
        &hSymlink,
        FILE_WRITE_ATTRIBUTES | DELETE | SYNCHRONIZE,
        &ObjectAttributes,
        &IoStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        0,
        FILE_CREATE,
        FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_REPARSE_POINT | dwCreateOptions,
        NULL,
        0
    );

    if(!NT_SUCCESS(Status))
    {
        dwErr = RtlNtStatusToDosError(Status);
        goto Cleanup;
    }

    Status = NtFsControlFile
    (
        hSymlink,
        NULL,
        NULL,
        NULL,
        &IoStatusBlock,
        FSCTL_SET_REPARSE_POINT,
        pReparseData,
        cbReparseData,
        NULL,
        0
    );

    if(!NT_SUCCESS(Status))
    {
        FILE_DISPOSITION_INFORMATION DispInfo;
        DispInfo.DeleteFile = TRUE;
        NtSetInformationFile(hSymlink, &IoStatusBlock, &DispInfo, sizeof(DispInfo), FileDispositionInformation);

        dwErr = RtlNtStatusToDosError(Status);
        goto Cleanup;
    }

    dwErr = NO_ERROR;

Cleanup:
    if(hSymlink)
        NtClose(hSymlink);

    RtlFreeUnicodeString(&SymlinkFileName);
    if (bAllocatedTarget)
    {
        RtlFreeHeap(RtlGetProcessHeap(),
                    0,
                    TargetFileName.Buffer);
    }

    if(lpTargetFullFileName)
        RtlFreeHeap(RtlGetProcessHeap(), 0, lpTargetFullFileName);

    if(pReparseData)
        RtlFreeHeap(RtlGetProcessHeap(), 0, pReparseData);

    if(dwErr)
    {
        SetLastError(dwErr);
        return FALSE;
    }

    return TRUE;
}

/*
 * @implemented - need test
 */
BOOL 
WINAPI 
CopyFileTransactedA(
	LPCSTR lpExistingFileName, 
	LPCSTR lpNewFileName, 
	LPPROGRESS_ROUTINE lpProgressRoutine, 
	LPVOID lpData, 
	LPBOOL pbCancel, 
	DWORD dwCopyFlags, 
	HANDLE hTransaction
)
{
  BOOL resp; // [sp+10h] [bp-1Ch]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !GetCurrentTransaction() )
    {
      SetCurrentTransaction(hTransaction);
      CopyFileExA(lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, pbCancel, dwCopyFlags);
      return SetCurrentTransaction(0);
    }
    SetLastError(6725u);
    resp = 0;
  }
  else
  {
    SetLastError(6700u);
    resp = 0;
  }
  return resp;
}

/*
 * @implemented - need test
 */
BOOL 
WINAPI 
CopyFileTransactedW(
LPCWSTR lpExistingFileName, 
LPCWSTR lpNewFileName, 
LPPROGRESS_ROUTINE lpProgressRoutine, 
LPVOID lpData, 
LPBOOL pbCancel, 
DWORD dwCopyFlags, 
HANDLE hTransaction
)
{
  BOOL resp; // [sp+10h] [bp-1Ch]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !GetCurrentTransaction() )
    {
      SetCurrentTransaction(hTransaction);
      CopyFileExW(lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, pbCancel, dwCopyFlags);
    }
    SetLastError(6725u);
    resp = 0;
  }
  else
  {
    SetLastError(6700u);
    resp = 0;
  }
  return resp;
}

/*
 * @implemented - need test
 */
BOOL WINAPI GetFileAttributesTransactedW(LPCWSTR lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation, HANDLE hTransaction)
{
  BOOL resp; // [sp+10h] [bp-1Ch]@6

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !GetCurrentTransaction() )
    {
      SetCurrentTransaction(hTransaction);
      GetFileAttributesExW(lpFileName, fInfoLevelId, lpFileInformation);
      return SetCurrentTransaction(0);
    }
    SetLastError(0x1A45u);
    resp = 0;
  }
  else
  {
    SetLastError(0x1A2Cu);
    resp = 0;
  }
  return resp;
}


BOOL 
WINAPI 
GetFileAttributesTransactedA(
	LPCSTR lpFileName, 
	GET_FILEEX_INFO_LEVELS fInfoLevelId, 
	LPVOID lpFileInformation, 
	HANDLE hTransaction
)
{
  BOOL result=1;
  PUNICODE_STRING local;

  local = Basep8BitStringToStaticUnicodeString(lpFileName);
  if ( result )
    result = GetFileAttributesTransactedW((LPCWSTR)local, fInfoLevelId, lpFileInformation, hTransaction);
  return result;
}

/*
 * @implemented
 */
BOOL WINAPI DeleteFileTransactedW(LPCWSTR lpFileName, HANDLE hTransaction)
{
  BOOL resp; // [sp+10h] [bp-1Ch]@6

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !GetCurrentTransaction() )
    {
      SetCurrentTransaction(hTransaction);
      DeleteFileW(lpFileName);
      return SetCurrentTransaction(0);
    }
    SetLastError(6725u);
    resp = 0;
  }
  else
  {
    SetLastError(6700u);
    resp = 0;
  }
  return resp;
}

/*
 * @implemented
 */
BOOL WINAPI DeleteFileTransactedA(LPCTSTR lpFileName, HANDLE hTransaction)
{
  BOOL result; // eax@1

  result = (BOOL)Basep8BitStringToStaticUnicodeString(lpFileName);
  if ( result )
    result = DeleteFileTransactedW((LPCWSTR)lpFileName, hTransaction);
  return result;
}

/*
 * @implemented
 */
BOOLEAN WINAPI CreateSymbolicLinkTransactedW(PWSTR dosname, PWSTR Src, DWORD FileInformation, HANDLE hTransaction)
{
  BOOLEAN symbolic; // ST1F_1@5
  BOOLEAN resp; // [sp+13h] [bp-19h]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !GetCurrentTransaction() )
    {
      SetCurrentTransaction(hTransaction);
      symbolic = CreateSymbolicLinkW(dosname, Src, FileInformation);
      return SetCurrentTransaction(0);
    }
    SetLastError(0x1A45u);
    resp = 0;
  }
  else
  {
    SetLastError(0x1A2Cu);
    resp = 0;
  }
  return resp;
}

/*
 * @implemented
 */
BOOLEAN WINAPI CreateSymbolicLinkTransactedA(PSTR dosname, PSTR Src, DWORD FileInformation, HANDLE hTransaction)
{
  PUNICODE_STRING resp; // esi@3
  BOOLEAN symbolic; // bl@5
  LSA_UNICODE_STRING UnicodeString; // [sp+4h] [bp-8h]@4

  if ( dosname && Src )
  {
    resp = Basep8BitStringToStaticUnicodeString(dosname);
    if ( resp && Basep8BitStringToDynamicUnicodeString(&UnicodeString, Src) )
    {
      symbolic = CreateSymbolicLinkTransactedW(resp->Buffer, UnicodeString.Buffer, FileInformation, hTransaction);
      RtlFreeUnicodeString(&UnicodeString);
      return symbolic;
    }
  }
  else
  {
    SetLastError(0x57u);
  }
  return 0;
}

BOOL WINAPI CreateDirectoryTransactedW(LPCWSTR lpTemplateDirectory, LPCWSTR lpNewDirectory, LPSECURITY_ATTRIBUTES lpSecurityAttributes, HANDLE hTransaction)
{
  if ( hTransaction == (HANDLE)-1 || !hTransaction )
  {
    SetLastError(0x1A2Cu);
    return 0;
  }
  if ( RtlGetCurrentTransaction() )
  {
    SetLastError(0x1A45u);
    return 0;
  }
  RtlSetCurrentTransaction(hTransaction);
  if ( lpTemplateDirectory )
    CreateDirectoryExW(lpTemplateDirectory, lpNewDirectory, lpSecurityAttributes);
  else
    CreateDirectoryW(lpNewDirectory, lpSecurityAttributes);
  return RtlSetCurrentTransaction(0);
}

BOOL WINAPI CreateDirectoryTransactedA(LPCTSTR lpTemplateDirectory, LPCTSTR lpNewDirectory, LPSECURITY_ATTRIBUTES lpSecurityAttributes, HANDLE hTransaction)
{
  LPCWSTR string; // esi@1
  PUNICODE_STRING buffer; // eax@2
  BOOL resp; // esi@6
  LSA_UNICODE_STRING UnicodeString; // [sp+4h] [bp-8h]@5

  string = 0;
  if ( lpTemplateDirectory )
  {
    buffer = (PUNICODE_STRING)Basep8BitStringToStaticUnicodeString(lpTemplateDirectory);
    if ( !buffer )
      return 0;
    string = buffer->Buffer;
  }
  if ( !Basep8BitStringToDynamicUnicodeString(&UnicodeString, lpNewDirectory) )
    return 0;
  resp = CreateDirectoryTransactedW(string, UnicodeString.Buffer, lpSecurityAttributes, hTransaction);
  RtlFreeUnicodeString(&UnicodeString);
  return resp;
}

HANDLE WINAPI BasepSetMiniVersionForCreate(HANDLE a1)
{
  HANDLE result; // eax@1
  result = a1;  
  NtCurrentTeb()->SpareBytes1[36] = (UCHAR)a1;
  return result;
}

HANDLE WINAPI BasepGetMiniVersionForCreate()
{
  return &NtCurrentTeb()->SpareBytes1[36];
}

HANDLE WINAPI CreateFileTransactedW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile, HANDLE hTransaction, PUSHORT pusMiniVersion, PVOID pExtendedParameter)
{
  HANDLE result; // eax@6
  DWORD flags; // [sp-4h] [bp-30h]@8

  if ( pExtendedParameter )
  {
    flags = 87;
    goto ERROR_EXIT;
  }
  if ( hTransaction == (HANDLE)-1 || !hTransaction )
  {
    flags = 6700;
ERROR_EXIT:
    SetLastError(flags);
    return (HANDLE)-1;
  }
  if ( RtlGetCurrentTransaction() )
  {
    flags = 6725;
    goto ERROR_EXIT;
  }
  RtlSetCurrentTransaction(hTransaction);
  if ( pusMiniVersion )
  {
    hTransaction = BasepGetMiniVersionForCreate();
    BasepSetMiniVersionForCreate((HANDLE)pusMiniVersion);
  }
  CreateFileW(
    lpFileName,
    dwDesiredAccess,
    dwShareMode,
    lpSecurityAttributes,
    dwCreationDisposition,
    dwFlagsAndAttributes,
    hTemplateFile);
  result = (HANDLE)RtlSetCurrentTransaction(0);
  if ( pusMiniVersion )
    result = (HANDLE)BasepSetMiniVersionForCreate(hTransaction);
  return result;
}

HANDLE WINAPI CreateFileTransactedA(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile, HANDLE hTransaction, PUSHORT pusMiniVersion, PVOID pExtendedParameter)
{
  PUNICODE_STRING string; // eax@1
  HANDLE result; // eax@2

  string = (PUNICODE_STRING)Basep8BitStringToStaticUnicodeString(lpFileName);
  if ( string )
    result = (HANDLE)CreateFileTransactedW(
                       string->Buffer,
                       dwDesiredAccess,
                       dwShareMode,
                       lpSecurityAttributes,
                       dwCreationDisposition,
                       dwFlagsAndAttributes,
                       hTemplateFile,
                       hTransaction,
                       pusMiniVersion,
                       pExtendedParameter);
  else
    result = (HANDLE)-1;
  return result;
}

BOOL WINAPI CreateHardLinkTransactedW(LPCWSTR lpFileName, LPCWSTR lpExistingFileName, LPSECURITY_ATTRIBUTES lpSecurityAttributes, HANDLE hTransaction)
{
  BOOL resp; // [sp+10h] [bp-1Ch]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      CreateHardLinkW(lpFileName, lpExistingFileName, lpSecurityAttributes);
      return RtlSetCurrentTransaction(0);
    }
    SetLastError(0x1A45u);
    resp = 0;
  }
  else
  {
    SetLastError(0x1A2Cu);
    resp = 0;
  }
  return resp;
}

BOOL WINAPI CreateHardLinkTransactedA(LPCTSTR lpFileName, LPCTSTR lpExistingFileName, LPSECURITY_ATTRIBUTES lpSecurityAttributes, HANDLE hTransaction)
{
  PUNICODE_STRING string; // esi@1
  BOOL resp; // esi@7
  LSA_UNICODE_STRING UnicodeString; // [sp+4h] [bp-8h]@4

  string = (PUNICODE_STRING)Basep8BitStringToStaticUnicodeString(lpFileName);
  if ( !string )
    return 0;
  if ( lpExistingFileName )
  {
    if ( !Basep8BitStringToDynamicUnicodeString(&UnicodeString, lpExistingFileName) )
      return 0;
  }
  else
  {
    UnicodeString.Buffer = 0;
  }
  resp = CreateHardLinkTransactedW(string->Buffer, UnicodeString.Buffer, lpSecurityAttributes, hTransaction);
  RtlFreeUnicodeString(&UnicodeString);
  return resp;
}

HANDLE WINAPI FindFirstStreamTransactedW(LPCWSTR lpFileName, STREAM_INFO_LEVELS InfoLevel, LPVOID lpFindStreamData, DWORD dwFlags, HANDLE hTransaction)
{
  DWORD flags; // [sp-4h] [bp-30h]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      FindFirstStreamW(lpFileName, InfoLevel, lpFindStreamData, dwFlags);
      return (HANDLE)RtlSetCurrentTransaction(0);
    }
    flags = 6725;
  }
  else
  {
    flags = 6700;
  }
  SetLastError(flags);
  return (HANDLE)-1;
}

DWORD WINAPI GetCompressedFileSizeTransactedW(LPCWSTR lpFileName, LPDWORD lpFileSizeHigh, HANDLE hTransaction)
{
  DWORD resp; // [sp-4h] [bp-30h]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      GetCompressedFileSizeW(lpFileName, lpFileSizeHigh);
      return RtlSetCurrentTransaction(0);
    }
    resp = 6725;
  }
  else
  {
    resp = 6700;
  }
  SetLastError(resp);
  return -1;
}

DWORD WINAPI GetCompressedFileSizeTransactedA(LPCTSTR lpFileName, LPDWORD lpFileSizeHigh, HANDLE hTransaction)
{
  PUNICODE_STRING string; // eax@1
  DWORD result; // eax@2

  string = (PUNICODE_STRING)Basep8BitStringToStaticUnicodeString(lpFileName);
  if ( string )
    result = GetCompressedFileSizeTransactedW(string->Buffer, lpFileSizeHigh, hTransaction);
  else
    result = -1;
  return result;
}

DWORD WINAPI GetFullPathNameTransactedA(LPCSTR lpFileName, DWORD nBufferLength, LPSTR lpBuffer, LPSTR *lpFilePart, HANDLE hTransaction)
{
  DWORD resp; // [sp+10h] [bp-1Ch]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      GetFullPathNameA(lpFileName, nBufferLength, lpBuffer, lpFilePart);
      return RtlSetCurrentTransaction(0);
    }
    SetLastError(0x1A45u);
    resp = 0;
  }
  else
  {
    SetLastError(0x1A2Cu);
    resp = 0;
  }
  return resp;
}

DWORD WINAPI GetFullPathNameTransactedW(LPCWSTR lpFileName, DWORD nBufferLength, LPWSTR lpBuffer, LPWSTR *lpFilePart, HANDLE hTransaction)
{
  DWORD resp; // [sp+10h] [bp-1Ch]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      GetFullPathNameW(lpFileName, nBufferLength, lpBuffer, lpFilePart);
      return RtlSetCurrentTransaction(0);
    }
    SetLastError(0x1A45u);
    resp = 0;
  }
  else
  {
    SetLastError(0x1A2Cu);
    resp = 0;
  }
  return resp;
}

BOOL WINAPI RemoveDirectoryTransactedW(LPCWSTR lpPathName, HANDLE hTransaction)
{
  BOOL resp; // [sp+10h] [bp-1Ch]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      RemoveDirectoryW(lpPathName);
      return RtlSetCurrentTransaction(0);
    }
    SetLastError(0x1A45u);
    resp = 0;
  }
  else
  {
    SetLastError(0x1A2Cu);
    resp = 0;
  }
  return resp;
}

BOOL WINAPI RemoveDirectoryTransactedA(LPCTSTR lpPathName, HANDLE hTransaction)
{
  PUNICODE_STRING result;
  BOOL resp;// eax@1

  result = Basep8BitStringToStaticUnicodeString(lpPathName);
  if ( result )
    resp = RemoveDirectoryTransactedW(result->Buffer, hTransaction);
  return resp;
}

BOOL WINAPI SetFileAttributesTransactedW(LPCWSTR lpFileName, DWORD dwFileAttributes, HANDLE hTransaction)
{
  BOOL resp; // [sp+10h] [bp-1Ch]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      SetFileAttributesW(lpFileName, dwFileAttributes);
      return RtlSetCurrentTransaction(0);
    }
    SetLastError(0x1A45u);
    resp = 0;
  }
  else
  {
    SetLastError(0x1A2Cu);
    resp = 0;
  }
  return resp;
}

BOOL WINAPI SetFileAttributesTransactedA(LPCTSTR lpFileName, DWORD dwFileAttributes, void *hTransaction)
{
  PUNICODE_STRING result;
  BOOL resp;// eax@1

  result = Basep8BitStringToStaticUnicodeString(lpFileName);
  if ( result )
    resp = SetFileAttributesTransactedW(result->Buffer, dwFileAttributes, hTransaction);
  return resp;
}

DWORD WINAPI GetLongPathNameTransactedA(LPCSTR lpszShortPath, LPSTR lpszLongPath, DWORD cchBuffer, HANDLE hTransaction)
{
  DWORD resp; // [sp+10h] [bp-1Ch]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      GetLongPathNameA(lpszShortPath, lpszLongPath, cchBuffer);
      return RtlSetCurrentTransaction(0);
    }
    SetLastError(0x1A45u);
    resp = 0;
  }
  else
  {
    SetLastError(0x1A2Cu);
    resp = 0;
  }
  return resp;
}

DWORD WINAPI GetLongPathNameTransactedW(LPCWSTR lpszShortPath, LPWSTR lpszLongPath, DWORD cchBuffer, HANDLE hTransaction)
{
  DWORD resp; // [sp+10h] [bp-1Ch]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      GetLongPathNameW(lpszShortPath, lpszLongPath, cchBuffer);
      return RtlSetCurrentTransaction(0);
    }
    SetLastError(0x1A45u);
    resp = 0;
  }
  else
  {
    SetLastError(0x1A2Cu);
    resp = 0;
  }
  return resp;
}

BOOL WINAPI MoveFileTransactedA(LPCTSTR lpExistingFileName, 
								LPCTSTR lpNewFileName, 
								LPPROGRESS_ROUTINE lpProgressRoutine, 
								LPVOID lpData, 
								DWORD dwFlags, 
								HANDLE hTransaction)
{
  DWORD error; // [sp-4h] [bp-30h]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      MoveFileExA(lpExistingFileName, lpNewFileName, dwFlags);
      return RtlSetCurrentTransaction(0);
    }
    error = 6725;
  }
  else
  {
    error = 6700;
  }
  SetLastError(error);
  return FALSE;
}

BOOL WINAPI MoveFileTransactedW(LPCWSTR lpExistingFileName, 
								LPCWSTR lpNewFileName, 
								LPPROGRESS_ROUTINE lpProgressRoutine, 
								LPVOID lpData, 
								DWORD dwFlags, 
								HANDLE hTransaction)
{
  DWORD error; // [sp-4h] [bp-30h]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      MoveFileExW(lpExistingFileName, lpNewFileName, dwFlags);
      return RtlSetCurrentTransaction(0);
    }
    error = 6725;
  }
  else
  {
    error = 6700;
  }
  SetLastError(error);
  return FALSE;
}

HANDLE WINAPI FindFirstFileTransactedA(LPCSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags, HANDLE hTransaction)
{
  DWORD error; // [sp-4h] [bp-30h]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      FindFirstFileExA(lpFileName, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags);
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

HANDLE WINAPI FindFirstFileTransactedW(LPCWSTR lpFileName, FINDEX_INFO_LEVELS fInfoLevelId, LPVOID lpFindFileData, FINDEX_SEARCH_OPS fSearchOp, LPVOID lpSearchFilter, DWORD dwAdditionalFlags, HANDLE hTransaction)
{
  DWORD error; // [sp-4h] [bp-30h]@4

  if ( hTransaction != (HANDLE)-1 && hTransaction )
  {
    if ( !RtlGetCurrentTransaction() )
    {
      RtlSetCurrentTransaction(hTransaction);
      FindFirstFileExW(lpFileName, fInfoLevelId, lpFindFileData, fSearchOp, lpSearchFilter, dwAdditionalFlags);
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
