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

WINE_DEFAULT_DEBUG_CHANNEL(filehops); 

typedef struct _FILE_REMOTE_PROTOCOL_INFO {
  USHORT StructureVersion;
  USHORT StructureSize;
  ULONG  Protocol;
  USHORT ProtocolMajorVersion;
  USHORT ProtocolMinorVersion;
  USHORT ProtocolRevision;
  USHORT Reserved[8];
  DWORD  Flags;
  struct {
    DWORD Reserved[8];
  } GenericReserved;
//#if (_WIN32_WINNT < _WIN32_WINNT_WIN8)
  struct {
    DWORD Reserved[16];
  } ProtocolSpecificReserved;
// #else 
  // union {
    // struct {
      // struct {
        // DWORD Capabilities;
      // } Server;
      // struct {
        // DWORD Capabilities;
        // DWORD CachingFlags;
      // } Share;
    // } Smb2;
    // DWORD  Reserved[16];
  // } ProtocolSpecific;
//#endif 
} FILE_REMOTE_PROTOCOL_INFO, *PFILE_REMOTE_PROTOCOL_INFO;

//
// Network types
//

#define     WNNC_NET_MSNET       0x00010000
#define     WNNC_NET_SMB         0x00020000
#define     WNNC_NET_NETWARE     0x00030000
#define     WNNC_NET_VINES       0x00040000
#define     WNNC_NET_10NET       0x00050000
#define     WNNC_NET_LOCUS       0x00060000
#define     WNNC_NET_SUN_PC_NFS  0x00070000
#define     WNNC_NET_LANSTEP     0x00080000
#define     WNNC_NET_9TILES      0x00090000
#define     WNNC_NET_LANTASTIC   0x000A0000
#define     WNNC_NET_AS400       0x000B0000
#define     WNNC_NET_FTP_NFS     0x000C0000
#define     WNNC_NET_PATHWORKS   0x000D0000
#define     WNNC_NET_LIFENET     0x000E0000
#define     WNNC_NET_POWERLAN    0x000F0000
#define     WNNC_NET_BWNFS       0x00100000
#define     WNNC_NET_COGENT      0x00110000
#define     WNNC_NET_FARALLON    0x00120000
#define     WNNC_NET_APPLETALK   0x00130000
#define     WNNC_NET_INTERGRAPH  0x00140000
#define     WNNC_NET_SYMFONET    0x00150000
#define     WNNC_NET_CLEARCASE   0x00160000
#define     WNNC_NET_FRONTIER    0x00170000
#define     WNNC_NET_BMC         0x00180000
#define     WNNC_NET_DCE         0x00190000
#define     WNNC_NET_AVID        0x001A0000
#define     WNNC_NET_DOCUSPACE   0x001B0000
#define     WNNC_NET_MANGOSOFT   0x001C0000
#define     WNNC_NET_SERNET      0x001D0000
#define     WNNC_NET_RIVERFRONT1 0X001E0000
#define     WNNC_NET_RIVERFRONT2 0x001F0000
#define     WNNC_NET_DECORB      0x00200000
#define     WNNC_NET_PROTSTOR    0x00210000
#define     WNNC_NET_FJ_REDIR    0x00220000
#define     WNNC_NET_DISTINCT    0x00230000
#define     WNNC_NET_TWINS       0x00240000
#define     WNNC_NET_RDR2SAMPLE  0x00250000
#define     WNNC_NET_CSC         0x00260000
#define     WNNC_NET_3IN1        0x00270000
#define     WNNC_NET_EXTENDNET   0x00290000
#define     WNNC_NET_STAC        0x002A0000
#define     WNNC_NET_FOXBAT      0x002B0000
#define     WNNC_NET_YAHOO       0x002C0000
#define     WNNC_NET_EXIFS       0x002D0000
#define     WNNC_NET_DAV         0x002E0000
#define     WNNC_NET_KNOWARE     0x002F0000
#define     WNNC_NET_OBJECT_DIRE 0x00300000
#define     WNNC_NET_MASFAX      0x00310000
#define     WNNC_NET_HOB_NFS     0x00320000
#define     WNNC_NET_SHIVA       0x00330000
#define     WNNC_NET_IBMAL       0x00340000
#define     WNNC_NET_LOCK        0x00350000
#define     WNNC_NET_TERMSRV     0x00360000
#define     WNNC_NET_SRT         0x00370000
#define     WNNC_NET_QUINCY      0x00380000
#define     WNNC_NET_OPENAFS     0x00390000
#define     WNNC_NET_AVID1       0X003A0000
#define     WNNC_NET_DFS         0x003B0000
#define     WNNC_NET_KWNP        0x003C0000
#define     WNNC_NET_ZENWORKS    0x003D0000
#define     WNNC_NET_DRIVEONWEB  0x003E0000
#define     WNNC_NET_VMWARE      0x003F0000
#define     WNNC_NET_RSFX        0x00400000
#define     WNNC_NET_MFILES      0x00410000
#define     WNNC_NET_MS_NFS      0x00420000
#define     WNNC_NET_GOOGLE      0x00430000
#define     WNNC_NET_NDFS        0x00440000
#define     WNNC_NET_DOCUSHARE   0x00450000

#define     WNNC_CRED_MANAGER   0xFFFF0000

//
// Network type aliases
//

#define     WNNC_NET_LANMAN      WNNC_NET_SMB

//
// Protocol generic flags for version 1 and higher
//

#define REMOTE_PROTOCOL_FLAG_LOOPBACK           0x00000001
#define REMOTE_PROTOCOL_FLAG_OFFLINE            0x00000002

/*
 * @implemented
 */
BOOLEAN
WINAPI
Wow64EnableWow64FsRedirection(IN BOOLEAN Wow64EnableWow64FsRedirection)
{
    NTSTATUS Status;
    BOOL Result;

    Status = RtlWow64EnableFsRedirection(Wow64EnableWow64FsRedirection);
    if (NT_SUCCESS(Status))
    {
        Result = TRUE;
    }
    else
    {
        BaseSetLastNTError(Status);
        Result = FALSE;
    }
    return Result;
}

/*
 * @implemented
 */
BOOL
WINAPI
Wow64DisableWow64FsRedirection(IN PVOID *OldValue)
{
    NTSTATUS Status;
    BOOL Result;

    Status = RtlWow64EnableFsRedirectionEx((PVOID)TRUE, OldValue);
    if (NT_SUCCESS(Status))
    {
        Result = TRUE;
    }
    else
    {
        BaseSetLastNTError(Status);
        Result = FALSE;
    }
    return Result;
}

/*
 * @implemented
 */
BOOL
WINAPI
Wow64RevertWow64FsRedirection(IN PVOID OldValue)
{
    NTSTATUS Status;
    BOOL Result;

    Status = RtlWow64EnableFsRedirectionEx(OldValue, &OldValue);
    if (NT_SUCCESS(Status))
    {
        Result = TRUE;
    }
    else
    {
        BaseSetLastNTError(Status);
        Result = FALSE;
    }
    return Result;
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
	PFILE_REMOTE_PROTOCOL_INFO remoteInfo = NULL;
	
	DbgPrint( "GetFileInformationByHandleEx called\n" );
	
	DbgPrint( "GetFileInformationByHandleEx Information Class is %02X\n",  class);

    switch (class)
    {
    case FileStreamInfo:
    case FileCompressionInfo:
    case FileAttributeTagInfo:    
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
	case FileRemoteProtocolInfo:
		DbgPrint( "GetFileInformationByHandleEx is UNIMPLEMENTED for FileRemoteProtocolInfo option\n" );
		status = STATUS_SUCCESS;
		remoteInfo->StructureVersion = 1;
		remoteInfo->StructureSize = sizeof(FILE_REMOTE_PROTOCOL_INFO);
		remoteInfo->Protocol = WNNC_NET_GOOGLE;
		remoteInfo->ProtocolMajorVersion = 1;
		remoteInfo->ProtocolMinorVersion = 0;
		remoteInfo->ProtocolRevision = 0;
		remoteInfo->Flags = REMOTE_PROTOCOL_FLAG_OFFLINE;
		remoteInfo->GenericReserved.Reserved[0] = 0;
		remoteInfo->ProtocolSpecificReserved.Reserved[0] = 0;
		info = remoteInfo;
		return TRUE;
		// remoteInfo->ProtocolSpecific.Smb2.Server.Capabilities = 0;
		// remoteInfo->ProtocolSpecific.Smb2.Share.Capabilities = 0;
		// remoteInfo->ProtocolSpecific.Smb2.Share.CachingFlags = 0;
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