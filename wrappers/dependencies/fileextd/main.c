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

#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>
#include <ntsecapi.h>
#include <bcrypt.h>

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

typedef enum _FILE_ID_TYPE {
    FileIdType,
    ObjectIdType,
    ExtendedFileIdType,
    MaximumFileIdType
} FILE_ID_TYPE, *PFILE_ID_TYPE;

typedef enum _FILE_INFO_BY_HANDLE_CLASS { 
  FileBasicInfo                   = 0,
  FileStandardInfo                = 1,
  FileNameInfo                    = 2,
  FileRenameInfo                  = 3,
  FileDispositionInfo             = 4,
  FileAllocationInfo              = 5,
  FileEndOfFileInfo               = 6,
  FileStreamInfo                  = 7,
  FileCompressionInfo             = 8,
  FileAttributeTagInfo            = 9,
  FileIdBothDirectoryInfo         = 10, // 0xA
  FileIdBothDirectoryRestartInfo  = 11, // 0xB
  FileIoPriorityHintInfo          = 12, // 0xC
  FileRemoteProtocolInfo          = 13, // 0xD
  FileFullDirectoryInfo           = 14, // 0xE
  FileFullDirectoryRestartInfo    = 15, // 0xF
  FileStorageInfo                 = 16, // 0x10
  FileAlignmentInfo               = 17, // 0x11
  FileIdInfo                      = 18, // 0x12
  FileIdExtdDirectoryInfo         = 19, // 0x13
  FileIdExtdDirectoryRestartInfo  = 20, // 0x14
  MaximumFileInfoByHandlesClass
} FILE_INFO_BY_HANDLE_CLASS, *PFILE_INFO_BY_HANDLE_CLASS;

typedef struct {
  DWORD        dwSize;
  FILE_ID_TYPE Type;
  union {
    LARGE_INTEGER FileId;
    GUID          ObjectId;
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
                  ExtendedFileId;
#endif 
  } DUMMYUNIONNAME;
} FILE_ID_DESCRIPTOR, *LPFILE_ID_DESCRIPTOR;


HANDLE WINAPI OpenFileById(
  _In_     HANDLE                hFile,
  _In_     LPFILE_ID_DESCRIPTOR  lpFileID,
  _In_     DWORD                 dwDesiredAccess,
  _In_     DWORD                 dwShareMode,
  _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  _In_     DWORD                 dwFlags
)
{
	return NULL;
}

BOOL WINAPI GetFileInformationByHandleEx(
  _In_  HANDLE                    hFile,
  _In_  FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
  _Out_ LPVOID                    lpFileInformation,
  _In_  DWORD                     dwBufferSize
)
{
	return FALSE;
}

BOOL WINAPI SetFileInformationByHandle(
  _In_ HANDLE                    hFile,
  _In_ FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
  _In_ LPVOID                    lpFileInformation,
  _In_ DWORD                     dwBufferSize
)
{
	return FALSE;
}