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

HMODULE hshlwapi;

static LPWSTR (WINAPI *pPathCombineW)(LPWSTR ,LPCWSTR, LPCWSTR);

static HRESULT (WINAPI *pPathCreateFromUrlW)(PCWSTR, PWSTR, DWORD, DWORD);

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

/*************************************************************************
 * StrCpyW	[SHLWAPI.@]
 *
 * Copy a string to another string.
 *
 * PARAMS
 *  lpszStr [O] Destination string
 *  lpszSrc [I] Source string
 *
 * RETURNS
 *  lpszStr.
 */
LPWSTR WINAPI StrCpyW(LPWSTR lpszStr, LPCWSTR lpszSrc)
{
  TRACE("(%p,%s)\n", lpszStr, debugstr_w(lpszSrc));

  if (lpszStr && lpszSrc)
    strcpyW(lpszStr, lpszSrc);
  return lpszStr;
}