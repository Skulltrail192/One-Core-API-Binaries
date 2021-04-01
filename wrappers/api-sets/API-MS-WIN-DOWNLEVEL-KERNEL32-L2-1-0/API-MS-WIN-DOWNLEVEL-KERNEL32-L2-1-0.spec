@ stdcall AddAtomA(str) kernelbase.AddAtomA
@ stdcall AddAtomW(wstr) kernelbase.AddAtomW
@ stdcall BackupRead(ptr ptr long ptr long long ptr) kernelbase.BackupRead
@ stdcall BackupWrite(ptr ptr long ptr long long ptr) kernelbase.BackupWrite
@ stdcall BindIoCompletionCallback(long ptr long) kernelbase.BindIoCompletionCallback
@ stdcall ConvertFiberToThread() kernelbase.ConvertFiberToThread
@ stdcall ConvertThreadToFiber(ptr) kernelbase.ConvertThreadToFiber
@ stdcall CopyFileA(str str long) kernelbase.CopyFileA
@ stdcall CopyFileW(wstr wstr long) kernelbase.CopyFileW
@ stdcall CreateFiber(long ptr ptr) kernelbase.CreateFiber
@ stdcall CreateFileMappingA(long ptr long long long str) kernelbase.CreateFileMappingA
@ stdcall CreateFileTransactedW(wstr long long ptr long long ptr ptr ptr ptr)
@ stdcall CreateMailslotA(str long long ptr) kernelbase.CreateMailslotA
@ stdcall CreateNamedPipeA(str long long long long long long ptr) kernelbase.CreateNamedPipeA
@ stdcall CreateSemaphoreW(ptr long long wstr) kernelbase.CreateSemaphoreW
@ stdcall DeleteAtom(long) kernelbase.DeleteAtom
@ stdcall DeleteFiber(ptr) kernelbase.DeleteFiber
@ stdcall DnsHostnameToComputerNameW(wstr ptr ptr) kernelbase.DnsHostnameToComputerNameW
@ stdcall DosDateTimeToFileTime(long long ptr) kernelbase.DosDateTimeToFileTime
@ stdcall FatalAppExitA(long str) kernelbase.FatalAppExitA
@ stdcall FatalAppExitW(long wstr) kernelbase.FatalAppExitW
@ stdcall FileTimeToDosDateTime(ptr ptr ptr) kernelbase.FileTimeToDosDateTime
@ stdcall FindAtomA(str) kernelbase.FindAtomA
@ stdcall FindAtomW(wstr) kernelbase.FindAtomW
@ stdcall FindResourceA(long str str) kernelbase.FindResourceA
@ stdcall FindResourceExA(long str str long) kernelbase.FindResourceExA
@ stdcall FindResourceW(long wstr wstr) kernelbase.FindResourceW
@ stdcall GetActiveProcessorCount(long) kernelbase.GetActiveProcessorCount
@ stdcall GetAtomNameA(long ptr long) kernelbase.GetAtomNameA
@ stdcall GetAtomNameW(long ptr long) kernelbase.GetAtomNameW
@ stdcall GetComputerNameA(ptr ptr) kernelbase.GetComputerNameA
@ stdcall GetComputerNameW(ptr ptr) kernelbase.GetComputerNameW
@ stdcall GetConsoleWindow() kernelbase.GetConsoleWindow
@ stdcall GetDurationFormatEx(wstr long ptr long long ptr ptr long) ;need test
@ stdcall GetFirmwareEnvironmentVariableW(wstr wstr ptr long) kernelbase.GetFirmwareEnvironmentVariableW
@ stdcall GetMaximumProcessorGroupCount()
@ stdcall GetNamedPipeClientProcessId(long ptr) kernelbase.GetNamedPipeClientProcessId
@ stdcall GetNamedPipeServerProcessId(long ptr) kernelbase.GetNamedPipeServerProcessId
@ stdcall GetPrivateProfileIntA(str str long str) kernelbase.GetPrivateProfileIntA
@ stdcall GetPrivateProfileIntW(wstr wstr long wstr) kernelbase.GetPrivateProfileIntW
@ stdcall GetPrivateProfileSectionW(wstr ptr long wstr) kernelbase.GetPrivateProfileSectionW
@ stdcall GetPrivateProfileStringA(str str str ptr long str) kernelbase.GetPrivateProfileStringA
@ stdcall GetPrivateProfileStringW(wstr wstr wstr ptr long wstr) kernelbase.GetPrivateProfileStringW
@ stdcall GetProcessAffinityMask(long ptr ptr) kernelbase.GetProcessAffinityMask
@ stdcall GetProcessIoCounters(long ptr) kernelbase.GetProcessIoCounters
@ stdcall GetProfileIntA(str str long) kernelbase.GetProfileIntA
@ stdcall GetProfileIntW(wstr wstr long) kernelbase.GetProfileIntW
@ stdcall GetProfileSectionA(str ptr long) kernelbase.GetProfileSectionA
@ stdcall GetProfileSectionW(wstr ptr long) kernelbase.GetProfileSectionW
@ stdcall GetProfileStringA(str str str ptr long) kernelbase.GetProfileStringA
@ stdcall GetProfileStringW(wstr wstr wstr ptr long) kernelbase.GetProfileStringW
@ stdcall GetShortPathNameA(str ptr long) kernelbase.GetShortPathNameA
@ stdcall GetStartupInfoA(ptr) kernelbase.GetStartupInfoA
@ stdcall GetStringTypeExA(long long str long ptr) kernelbase.GetStringTypeExA
@ stdcall GetSystemPowerStatus(ptr) kernelbase.GetSystemPowerStatus
@ stdcall GetSystemWow64DirectoryA(ptr long) kernelbase.GetSystemWow64DirectoryA
@ stdcall GetSystemWow64DirectoryW(ptr long) kernelbase.GetSystemWow64DirectoryW
@ stdcall GetTapeParameters(ptr long ptr ptr) kernelbase.GetTapeParameters
@ stdcall GetTempPathA(long ptr) kernelbase.GetTempPathA
@ stdcall GetThreadSelectorEntry(long long ptr) kernelbase.GetThreadSelectorEntry
@ stdcall GlobalAddAtomA(str) kernelbase.GlobalAddAtomA
@ stdcall GlobalAddAtomW(wstr) kernelbase.GlobalAddAtomW
@ stdcall GlobalAlloc(long long) kernelbase.GlobalAlloc
@ stdcall GlobalDeleteAtom(long) kernelbase.GlobalDeleteAtom
@ stdcall GlobalFindAtomA(str) kernelbase.GlobalFindAtomA
@ stdcall GlobalFindAtomW(wstr) kernelbase.GlobalFindAtomW
@ stdcall GlobalFlags(long) kernelbase.GlobalFlags
@ stdcall GlobalFree(long) kernelbase.GlobalFree
@ stdcall GlobalGetAtomNameA(long ptr long) kernelbase.GlobalGetAtomNameA
@ stdcall GlobalGetAtomNameW(long ptr long) kernelbase.GlobalGetAtomNameW
@ stdcall GlobalHandle(ptr) kernelbase.GlobalHandle
@ stdcall GlobalLock(long) kernelbase.GlobalLock
@ stdcall GlobalMemoryStatus(ptr) kernelbase.GlobalMemoryStatus
@ stdcall GlobalReAlloc(long long long) kernelbase.GlobalReAlloc
@ stdcall GlobalSize(long) kernelbase.GlobalSize
@ stdcall GlobalUnlock(long) kernelbase.GlobalUnlock
@ stdcall InitAtomTable(long) kernelbase.InitAtomTable
@ stdcall LoadLibraryA(str) kernelbase.LoadLibraryA
@ stdcall LoadLibraryW(wstr) kernelbase.LoadLibraryW
@ stdcall LocalAlloc(long long) kernelbase.LocalAlloc
@ stdcall LocalFlags(long) kernelbase.LocalFlags
@ stdcall LocalFree(long) kernelbase.LocalFree
@ stdcall LocalLock(long) kernelbase.LocalLock
@ stdcall LocalReAlloc(long long long) kernelbase.LocalReAlloc
@ stdcall LocalSize(long) kernelbase.LocalSize
@ stdcall LocalUnlock(long) kernelbase.LocalUnlock
@ stdcall MoveFileA(str str) kernelbase.MoveFileA
@ stdcall MoveFileExA(str str long) kernelbase.MoveFileExA
@ stdcall MoveFileW(wstr wstr) kernelbase.MoveFileW
@ stdcall MulDiv(long long long) kernelbase.MulDiv
@ stdcall OpenFile(str ptr long) kernelbase.OpenFile
@ stdcall PulseEvent(long) kernelbase.PulseEvent
@ stdcall RaiseFailFastException(ptr ptr long)
@ stdcall RegisterWaitForSingleObject(ptr long ptr ptr long long) kernelbase.RegisterWaitForSingleObject
@ stdcall SetConsoleTitleA(str) kernelbase.SetConsoleTitleA
@ stdcall SetFileCompletionNotificationModes(long long) kernelbase.SetFileCompletionNotificationModes
@ stdcall SetFirmwareEnvironmentVariableW(wstr wstr ptr long) kernelbase.SetFirmwareEnvironmentVariableW
@ stdcall SetHandleCount(long) kernelbase.SetHandleCount
@ stdcall SetMailslotInfo(long long) kernelbase.SetMailslotInfo
@ stdcall SetProcessAffinityMask(long long) kernelbase.SetProcessAffinityMask
@ stdcall SetThreadAffinityMask(long long) kernelbase.SetThreadAffinityMask
@ stdcall SetThreadIdealProcessor(long long) kernelbase.SetThreadIdealProcessor
@ stdcall SetVolumeLabelW(wstr wstr) kernelbase.SetVolumeLabelW
@ stdcall SwitchToFiber(ptr) kernelbase.SwitchToFiber
@ stdcall UnregisterWait(long) kernelbase.UnregisterWait
@ stdcall WTSGetActiveConsoleSessionId() kernelbase.WTSGetActiveConsoleSessionId
@ stdcall WaitForMultipleObjects(long ptr long long) kernelbase.WaitForMultipleObjects
@ stdcall WritePrivateProfileSectionA(str str str) kernelbase.WritePrivateProfileSectionA
@ stdcall WritePrivateProfileSectionW(wstr wstr wstr) kernelbase.WritePrivateProfileSectionW
@ stdcall WritePrivateProfileStringA(str str str str) kernelbase.WritePrivateProfileStringA
@ stdcall WritePrivateProfileStringW(wstr wstr wstr wstr) kernelbase.WritePrivateProfileStringW
@ stdcall lstrcatW(wstr wstr) kernelbase.lstrcatW
@ stdcall lstrcmpA(str str) kernelbase.lstrcmpA
@ stdcall lstrcmpW(wstr wstr) kernelbase.lstrcmpW
@ stdcall lstrcmpiA(str str) kernelbase.lstrcmpiA
@ stdcall lstrcmpiW(wstr wstr) kernelbase.lstrcmpiW
@ stdcall lstrcpyW(ptr wstr) kernelbase.lstrcpyW
@ stdcall lstrcpynA(ptr str long) kernelbase.lstrcpynA
@ stdcall lstrcpynW(ptr wstr long) kernelbase.lstrcpynW
@ stdcall lstrlenA(str) kernelbase.lstrlenA
@ stdcall lstrlenW(wstr) kernelbase.lstrlenW
