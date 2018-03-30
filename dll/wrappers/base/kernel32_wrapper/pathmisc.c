/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    pathmisc.c

Abstract:

    Win32 misceleneous path functions

Author:

    Skulltrail 25-June-2017

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(kernel32file);

#define FS_VOLUME_BUFFER_SIZE (MAX_PATH * sizeof(WCHAR) + sizeof(FILE_FS_VOLUME_INFORMATION))

#define FS_ATTRIBUTE_BUFFER_SIZE (MAX_PATH * sizeof(WCHAR) + sizeof(FILE_FS_ATTRIBUTE_INFORMATION))

/*
 * @implemented
 */
BOOL
WINAPI
GetVolumeInformationByHandleW(
	IN HANDLE hFile,
    IN LPWSTR lpVolumeNameBuffer,
    IN DWORD nVolumeNameSize,
    OUT LPDWORD lpVolumeSerialNumber OPTIONAL,
    OUT LPDWORD lpMaximumComponentLength OPTIONAL,
    OUT LPDWORD lpFileSystemFlags OPTIONAL,
    OUT LPWSTR lpFileSystemNameBuffer OPTIONAL,
    IN DWORD nFileSystemNameSize)
{
  PFILE_FS_VOLUME_INFORMATION FileFsVolume;
  PFILE_FS_ATTRIBUTE_INFORMATION FileFsAttribute;
  IO_STATUS_BLOCK IoStatusBlock;
  UCHAR Buffer[max(FS_VOLUME_BUFFER_SIZE, FS_ATTRIBUTE_BUFFER_SIZE)];

  NTSTATUS errCode;

  FileFsVolume = (PFILE_FS_VOLUME_INFORMATION)Buffer;
  FileFsAttribute = (PFILE_FS_ATTRIBUTE_INFORMATION)Buffer;

  TRACE("FileFsVolume %p\n", FileFsVolume);
  TRACE("FileFsAttribute %p\n", FileFsAttribute);

  if (hFile == INVALID_HANDLE_VALUE)
    {
      return FALSE;
    }

  TRACE("hFile: %p\n", hFile);
  errCode = NtQueryVolumeInformationFile(hFile,
                                         &IoStatusBlock,
                                         FileFsVolume,
                                         FS_VOLUME_BUFFER_SIZE,
                                         FileFsVolumeInformation);
  if ( !NT_SUCCESS(errCode) )
    {
      WARN("Status: %x\n", errCode);
      CloseHandle(hFile);
      BaseSetLastNTError (errCode);
      return FALSE;
    }

  if (lpVolumeSerialNumber)
    *lpVolumeSerialNumber = FileFsVolume->VolumeSerialNumber;

  if (lpVolumeNameBuffer)
    {
      if (nVolumeNameSize * sizeof(WCHAR) >= FileFsVolume->VolumeLabelLength + sizeof(WCHAR))
        {
	  memcpy(lpVolumeNameBuffer,
		 FileFsVolume->VolumeLabel,
		 FileFsVolume->VolumeLabelLength);
	  lpVolumeNameBuffer[FileFsVolume->VolumeLabelLength / sizeof(WCHAR)] = 0;
	}
      else
        {
	  CloseHandle(hFile);
	  SetLastError(ERROR_MORE_DATA);
	  return FALSE;
	}
    }

  errCode = NtQueryVolumeInformationFile (hFile,
	                                  &IoStatusBlock,
	                                  FileFsAttribute,
	                                  FS_ATTRIBUTE_BUFFER_SIZE,
	                                  FileFsAttributeInformation);
  CloseHandle(hFile);
  if (!NT_SUCCESS(errCode))
    {
      WARN("Status: %x\n", errCode);
      BaseSetLastNTError (errCode);
      return FALSE;
    }

  if (lpFileSystemFlags)
    *lpFileSystemFlags = FileFsAttribute->FileSystemAttributes;
  if (lpMaximumComponentLength)
    *lpMaximumComponentLength = FileFsAttribute->MaximumComponentNameLength;
  if (lpFileSystemNameBuffer)
    {
      if (nFileSystemNameSize * sizeof(WCHAR) >= FileFsAttribute->FileSystemNameLength + sizeof(WCHAR))
        {
	  memcpy(lpFileSystemNameBuffer,
		 FileFsAttribute->FileSystemName,
		 FileFsAttribute->FileSystemNameLength);
	  lpFileSystemNameBuffer[FileFsAttribute->FileSystemNameLength / sizeof(WCHAR)] = 0;
	}
      else
        {
	  SetLastError(ERROR_MORE_DATA);
	  return FALSE;
	}
    }
  return TRUE;
}

HRESULT WINAPI PathCchAddBackslash(WCHAR *path, SIZE_T size)
{
    return PathCchAddBackslashEx(path, size, NULL, NULL);
}

HRESULT WINAPI PathCchAddBackslashEx(WCHAR *path, SIZE_T size, WCHAR **endptr, SIZE_T *remaining)
{
    BOOL needs_termination;
    SIZE_T length;

    TRACE("%s, %lu, %p, %p\n", debugstr_w(path), size, endptr, remaining);

    length = strlenW(path);
    needs_termination = size && length && path[length - 1] != '\\';

    if (length >= (needs_termination ? size - 1 : size))
    {
        if (endptr) *endptr = NULL;
        if (remaining) *remaining = 0;
        return STRSAFE_E_INSUFFICIENT_BUFFER;
    }

    if (!needs_termination)
    {
        if (endptr) *endptr = path + length;
        if (remaining) *remaining = size - length;
        return S_FALSE;
    }

    path[length++] = '\\';
    path[length] = 0;

    if (endptr) *endptr = path + length;
    if (remaining) *remaining = size - length;

    return S_OK;
}

/***********************************************************************
 *          PathCchCombineEx (KERNELBASE.@)
 */
HRESULT WINAPI PathCchCombineEx(WCHAR *out, SIZE_T size, const WCHAR *path1, const WCHAR *path2, DWORD flags)
{
    WCHAR result[MAX_PATH];

    FIXME("(%p, %lu, %s, %s, %x): semi-stub\n", out, size, wine_dbgstr_w(path1), wine_dbgstr_w(path2), flags);

    if (!out || !size) return E_INVALIDARG;
    if (flags) FIXME("Flags %x not supported\n", flags);

    // if (!PathCombineW(result, path1, path2))
        // return E_INVALIDARG;

    if (strlenW(result) + 1 > size)
    {
        out[0] = 0;
        return STRSAFE_E_INSUFFICIENT_BUFFER;
    }

    strcpyW(out, result);
    return S_OK;
}
