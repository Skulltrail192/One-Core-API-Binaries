/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * PURPOSE:         Stub functions
 * PROGRAMMER:      Thomas Weidenmueller <w3seek@reactos.com>
 */

/* INCLUDES *******************************************************************/

#include <k32.h>

#define NDEBUG
#include <debug.h>

typedef enum _FILE_INFO_BY_HANDLE_CLASS {
    FileBasicInfo,
    FileStandardInfo,
    FileNameInfo,
    FileRenameInfo,
    FileDispositionInfo,
    FileAllocationInfo,
    FileEndOfFileInfo,
    FileStreamInfo,
    FileCompressionInfo,
    FileAttributeTagInfo,
    FileIdBothDirectoryInfo,
    FileIdBothDirectoryRestartInfo,
    FileIoPriorityHintInfo,
    FileRemoteProtocolInfo,
    FileFullDirectoryInfo,
    FileFullDirectoryRestartInfo,
    FileStorageInfo,
    FileAlignmentInfo,
    FileIdInfo,
    FileIdExtdDirectoryInfo,
    FileIdExtdDirectoryRestartInfo,
    MaximumFileInfoByHandlesClass
} FILE_INFO_BY_HANDLE_CLASS, *PFILE_INFO_BY_HANDLE_CLASS;

typedef enum _FILE_ID_TYPE {
    FileIdType,
    ObjectIdType,
    ExtendedFileIdType,
    MaximumFileIdType
} FILE_ID_TYPE, *PFILE_ID_TYPE;

typedef struct _FILE_ID_DESCRIPTOR {
    DWORD        dwSize;
    FILE_ID_TYPE Type;
    union {
        LARGE_INTEGER FileId;
        GUID          ObjectId;
    } DUMMYUNIONNAME;
} FILE_ID_DESCRIPTOR, *LPFILE_ID_DESCRIPTOR;

BOOL WINAPI CreateProcessAsUserW(
  _In_opt_    HANDLE                hToken,
  _In_opt_    LPCWSTR               lpApplicationName,
  _Inout_opt_ LPWSTR                lpCommandLine,
  _In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
  _In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
  _In_        BOOL                  bInheritHandles,
  _In_        DWORD                 dwCreationFlags,
  _In_opt_    LPVOID                lpEnvironment,
  _In_opt_    LPWSTR               lpCurrentDirectory,
  _In_        LPSTARTUPINFO         lpStartupInfo,
  _Out_       LPPROCESS_INFORMATION lpProcessInformation
)
{
	return FALSE;
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

int IdnToAscii(
  _In_      DWORD   dwFlags,
  _In_      LPCWSTR lpUnicodeCharStr,
  _In_      int     cchUnicodeChar,
  _Out_opt_ LPWSTR  lpASCIICharStr,
  _In_      int     cchASCIIChar
)
{
	return 0;
}

int IdnToUnicode(
  _In_      DWORD   dwFlags,
  _In_      LPCWSTR lpASCIICharStr,
  _In_      int     cchASCIIChar,
  _Out_opt_ LPWSTR  lpUnicodeCharStr,
  _In_      int     cchUnicodeChar
)
{
	return 0;
}

BOOL WINAPI ImpersonateNamedPipeClient(
  _In_ HANDLE hNamedPipe
)
{
	return FALSE;
}

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

BOOL WINAPI OpenProcessToken(
  _In_  HANDLE  ProcessHandle,
  _In_  DWORD   DesiredAccess,
  _Out_ PHANDLE TokenHandle
)
{
	return FALSE;
}

BOOL WINAPI OpenThreadToken(
  _In_  HANDLE  ThreadHandle,
  _In_  DWORD   DesiredAccess,
  _In_  BOOL    OpenAsSelf,
  _Out_ PHANDLE TokenHandle
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

BOOL WINAPI SetThreadToken(
  _In_opt_ PHANDLE Thread,
  _In_opt_ HANDLE  Token
)
{
	return FALSE;
}

BOOL WINAPI InitOnceComplete(
  _Inout_  LPINIT_ONCE lpInitOnce,
  _In_     DWORD       dwFlags,
  _In_opt_ LPVOID      lpContext
)
{
	return FALSE;
}

BOOL WINAPI InitOnceExecuteOnce(
  _Inout_     PINIT_ONCE    InitOnce,
  _In_        PINIT_ONCE_FN InitFn,
  _Inout_opt_ PVOID         Parameter,
  _Out_opt_   LPVOID        *Context
)
{
	return FALSE;
}

HRESULT WINAPI PathCchAddBackslash(WCHAR *path, SIZE_T size)
{
	return S_OK;
}

HRESULT WINAPI PathCchAddBackslashEx(WCHAR *path, SIZE_T size, WCHAR **endptr, SIZE_T *remaining)
{
	return S_OK;
}

HRESULT WINAPI PathCchCombineEx(WCHAR *out, SIZE_T size, const WCHAR *path1, const WCHAR *path2, DWORD flags)
{
	return S_OK;	
}