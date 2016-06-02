@ stdcall CreateDirectoryA(str ptr) kernel32.CreateDirectoryA
@ stdcall CreateDirectoryW(wstr ptr) kernel32.CreateDirectoryW
@ stdcall CreateFileA(str long long ptr long long long) kernel32.CreateFileA
@ stdcall CreateFileW(wstr long long ptr long long long) kernel32.CreateFileW
@ stdcall DeleteFileA(str) kernel32.DeleteFileA
@ stdcall DeleteFileW(wstr) kernel32.DeleteFileW
@ stdcall FileTimeToLocalFileTime(ptr ptr) kernel32.FileTimeToLocalFileTime
@ stdcall FileTimeToSystemTime(ptr ptr) kernel32.FileTimeToSystemTime
@ stdcall FindClose(long) kernel32.FindClose
@ stdcall FindFirstFileA(str ptr) kernel32.FindFirstFileA
@ stdcall FindFirstFileW(wstr ptr) kernel32.FindFirstFileW
@ stdcall FindNextFileA(long ptr) kernel32.FindNextFileA
@ stdcall FindNextFileW(long ptr) kernel32.FindNextFileW
@ stdcall FlushFileBuffers(long) kernel32.FlushFileBuffers
@ stdcall GetDiskFreeSpaceA(str ptr ptr ptr ptr) kernel32.GetDiskFreeSpaceA
@ stdcall GetDriveTypeA(str) kernel32.GetDriveTypeA
@ stdcall GetDriveTypeW(wstr) kernel32.GetDriveTypeW
@ stdcall GetFileAttributesA(str) kernel32.GetFileAttributesA
@ stdcall GetFileAttributesW(wstr) kernel32.GetFileAttributesW
@ stdcall GetFileInformationByHandle(long ptr) kernel32.GetFileInformationByHandle
@ stdcall GetFileType(long) kernel32.GetFileType
@ stdcall GetFullPathNameA(str long ptr ptr) kernel32.GetFullPathNameA
@ stdcall GetFullPathNameW(wstr long ptr ptr) kernel32.GetFullPathNameW
@ stdcall GetLogicalDrives() kernel32.GetLogicalDrives
@ stdcall LocalFileTimeToFileTime(ptr ptr) kernel32.LocalFileTimeToFileTime
@ stdcall LockFile(long long long long long) kernel32.LockFile
@ stdcall ReadFile(long ptr long ptr ptr) kernel32.ReadFile
@ stdcall RemoveDirectoryA(str) kernel32.RemoveDirectoryA
@ stdcall RemoveDirectoryW(wstr) kernel32.RemoveDirectoryW
@ stdcall SetEndOfFile(long) kernel32.SetEndOfFile
@ stdcall SetFileAttributesA(str long) kernel32.SetFileAttributesA
@ stdcall SetFileAttributesW(wstr long) kernel32.SetFileAttributesW
@ stdcall SetFileInformationByHandle(long long ptr long) kernel32.SetFileInformationByHandle
@ stdcall SetFilePointer(long long ptr long) kernel32.SetFilePointer
@ stdcall SetFileTime(long ptr ptr ptr) kernel32.SetFileTime
@ stdcall UnlockFile(long long long long long) kernel32.UnlockFile
@ stdcall WriteFile(long ptr long ptr ptr) kernel32.WriteFile