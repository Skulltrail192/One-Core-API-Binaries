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
#include <winternl.h>

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

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{


    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}

/*
 * @implemented
 */
BOOL 
WINAPI 
SetFileInformationByHandle(
	HANDLE FileHandle, 
	FILE_INFO_BY_HANDLE_CLASS FileInformationClass, 
	PVOID FileInformation, 
	DWORD FileInformationLength
)
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