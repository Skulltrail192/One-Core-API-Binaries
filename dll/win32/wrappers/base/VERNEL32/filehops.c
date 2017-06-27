/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    filehops.c

Abstract:

    This module implements Win32 file handle APIs

Author:

    Skulltrail 18-March-2017

Revision History:

--*/

#include "main.h"

/***********************************************************************
 *           Wow64EnableWow64FsRedirection   (KERNEL32.@)
 */
BOOLEAN 
WINAPI 
Wow64EnableWow64FsRedirection( 
	BOOLEAN enable 
)
{
    NTSTATUS status = RtlWow64EnableFsRedirection( enable );
    if (status) SetLastError( RtlNtStatusToDosError(status) );
    return !status;
}


/***********************************************************************
 *           Wow64DisableWow64FsRedirection   (KERNEL32.@)
 */
BOOL
WINAPI 
Wow64DisableWow64FsRedirection( 
	PVOID *old_value 
)
{
    NTSTATUS status = RtlWow64EnableFsRedirectionEx( TRUE, (ULONG *)old_value );
    if (status) SetLastError( RtlNtStatusToDosError(status) );
    return !status;
}


/***********************************************************************
 *           Wow64RevertWow64FsRedirection   (KERNEL32.@)
 */
BOOL 
WINAPI 
Wow64RevertWow64FsRedirection( 
	PVOID old_value 
)
{
    NTSTATUS status = RtlWow64EnableFsRedirection( !old_value );
    if (status) SetLastError( RtlNtStatusToDosError(status) );
    return !status;
}

/***********************************************************************
*             GetFileInformationByHandleEx (KERNEL32.@)
*/
BOOL 
WINAPI 
GetFileInformationByHandleEx( 
	HANDLE handle, 
	FILE_INFO_BY_HANDLE_CLASS class,
    LPVOID info, 
	DWORD size 
)
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
    case FileIdExtdDirectoryInfo:
    case FileIdExtdDirectoryRestartInfo:
        DbgPrint( "GetFileInformationByHandleEx is UNIMPLEMENTED for FileIdExtdDirectoryRestartInfo option\n" );
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

    case FileIdInfo:
        status = NtQueryInformationFile( handle, &io, info, size, FileIdInformation );
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

/***********************************************************************
 *           SetFileInformationByHandle   (KERNEL32.@)
 */
BOOL 
WINAPI 
SetFileInformationByHandle( 
	HANDLE file, 
	FILE_INFO_BY_HANDLE_CLASS class, 
	VOID *info, 
	DWORD size 
)
{
    NTSTATUS status;
    IO_STATUS_BLOCK io;

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

/***********************************************************************
 *             OpenFileById (KERNEL32.@)
 */
HANDLE 
WINAPI 
OpenFileById( 
	HANDLE handle, 
	LPFILE_ID_DESCRIPTOR id, 
	DWORD access,
    DWORD share, 
	LPSECURITY_ATTRIBUTES sec_attr, 
	DWORD flags 
)
{
    UINT options;
    HANDLE result;
    OBJECT_ATTRIBUTES attr;
    NTSTATUS status;
    IO_STATUS_BLOCK io;
    UNICODE_STRING objectName;

    if (!id)
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return INVALID_HANDLE_VALUE;
    }

    options = FILE_OPEN_BY_FILE_ID;
    if (flags & FILE_FLAG_BACKUP_SEMANTICS)
        options |= FILE_OPEN_FOR_BACKUP_INTENT;
    else
        options |= FILE_NON_DIRECTORY_FILE;
    if (flags & FILE_FLAG_NO_BUFFERING) options |= FILE_NO_INTERMEDIATE_BUFFERING;
    if (!(flags & FILE_FLAG_OVERLAPPED)) options |= FILE_SYNCHRONOUS_IO_NONALERT;
    if (flags & FILE_FLAG_RANDOM_ACCESS) options |= FILE_RANDOM_ACCESS;
    flags &= FILE_ATTRIBUTE_VALID_FLAGS;

    objectName.Length             = sizeof(ULONGLONG);
    objectName.Buffer             = (WCHAR *)&id->FileId;
    attr.Length                   = sizeof(attr);
    attr.RootDirectory            = handle;
    attr.Attributes               = 0;
    attr.ObjectName               = &objectName;
    attr.SecurityDescriptor       = sec_attr ? sec_attr->lpSecurityDescriptor : NULL;
    attr.SecurityQualityOfService = NULL;
    if (sec_attr && sec_attr->bInheritHandle) attr.Attributes |= OBJ_INHERIT;

    status = NtCreateFile( &result, access | SYNCHRONIZE, &attr, &io, NULL, flags,
                           share, OPEN_EXISTING, options, NULL, 0 );
    if (status != STATUS_SUCCESS)
    {
        SetLastError( RtlNtStatusToDosError( status ) );
        return INVALID_HANDLE_VALUE;
    }
    return result;
}

BOOL
WINAPI
CancelIoEx(
    HANDLE hFile,
	LPOVERLAPPED lpOverlapped	
)
{
    NTSTATUS Status;
    IO_STATUS_BLOCK IoStatusBlock;

    Status = NtCancelIoFile(hFile, &IoStatusBlock);

    if ( NT_SUCCESS(Status) ) {
        return TRUE;
        }
    else {
        BaseSetLastNTError(Status);
        return FALSE;
        }

}

/*
 * @implemented - new
 */
BOOL 
WINAPI 
CancelSynchronousIo(
	HANDLE hFile
)
{
  NTSTATUS status; // eax@3
  BOOL result; // eax@4
  IO_STATUS_BLOCK IoStatusBlock;

  status = NtCancelIoFile(hFile, &IoStatusBlock);
  if ( NT_SUCCESS(status))
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

BOOL 
WINAPI 
SetStdHandleEx(
  _In_ DWORD  nStdHandle,
  _In_ HANDLE hHandle,
  _In_ HANDLE oldHandle  
)
{
	oldHandle = GetStdHandle(nStdHandle);
	return SetStdHandle(nStdHandle, hHandle);
}