@ stdcall EnumSystemLocalesA(ptr long) kernel32.EnumSystemLocalesA
@ stdcall FormatMessageA(long ptr long long ptr long ptr) kernel32.FormatMessageA
@ stdcall FormatMessageW(long ptr long long ptr long ptr) kernel32.FormatMessageW
@ stdcall GlobalAlloc(long long) kernel32.GlobalAlloc
@ stdcall GlobalFree(long) kernel32.GlobalFree
@ stdcall IsProcessInJob(long long ptr) kernel32.IsProcessInJob
@ stdcall IsWow64Process(ptr ptr) kernel32.IsWow64Process
@ stdcall FatalAppExitA(long str) kernel32.FatalAppExitA
@ stdcall FatalAppExitW(long wstr) kernel32.FatalAppExitW
@ stdcall FatalExit(long) kernel32.FatalExit
@ stdcall LCMapStringA(long long str long ptr long) kernel32.LCMapStringA
@ stdcall LCMapStringW(long long wstr long ptr long) kernel32.LCMapStringW
@ stdcall LocalAlloc(long long) kernel32.LocalAlloc
@ stdcall LocalCompact(long) kernel32.LocalCompact
@ stdcall LocalFileTimeToFileTime(ptr ptr) kernel32.LocalFileTimeToFileTime
@ stdcall LocalFlags(long) kernel32.LocalFlags
@ stdcall LocalFree(long) kernel32.LocalFree
@ stdcall LocalHandle(ptr) kernel32.LocalHandle
@ stdcall LocalLock(long) kernel32.LocalLock
@ stdcall LocalReAlloc(long long long) kernel32.LocalReAlloc
@ stdcall LocalShrink(long long) kernel32.LocalShrink
@ stdcall LocalSize(long) kernel32.LocalSize
@ stdcall LocalUnlock(long) kernel32.LocalUnlock
@ stdcall lstrcmp(str str) kernel32.lstrcmp
@ stdcall lstrcmpA(str str) kernel32.lstrcmpA
@ stdcall lstrcmpW(wstr wstr) kernel32.lstrcmpW
@ stdcall lstrcmpi(str str) kernel32.lstrcmpi
@ stdcall lstrcmpiA(str str) kernel32.lstrcmpiA
@ stdcall lstrcmpiW(wstr wstr) kernel32.lstrcmpiW
@ stdcall lstrcpy(ptr str) kernel32.lstrcpy
@ stdcall lstrcpyA(ptr str) kernel32.lstrcpyA
@ stdcall lstrcpyW(ptr wstr) kernel32.lstrcpyW
@ stdcall lstrcpyn(ptr str long) kernel32.lstrcpyn 
@ stdcall lstrcpynA(ptr str long) kernel32.lstrcpynA
@ stdcall lstrcpynW(ptr wstr long) kernel32.lstrcpynW
@ stdcall lstrlen(str) kernel32.lstrlen
@ stdcall lstrlenA(str) kernel32.lstrlenA
@ stdcall lstrlenW(wstr) kernel32.lstrlenW
@ stdcall NeedCurrentDirectoryForExePathA(str) kernel32.NeedCurrentDirectoryForExePathA
@ stdcall NeedCurrentDirectoryForExePathW(wstr) kernel32.NeedCurrentDirectoryForExePathW
@ stdcall PulseEvent(long) kernel32.PulseEvent
@ stdcall SetHandleCount(long) kernel32.SetHandleCount
@ stdcall Sleep(long) kernel32.Sleep
@ stdcall Wow64DisableWow64FsRedirection(ptr) kernel32.Wow64DisableWow64FsRedirection
@ stdcall Wow64RevertWow64FsRedirection(ptr) kernel32.Wow64RevertWow64FsRedirection