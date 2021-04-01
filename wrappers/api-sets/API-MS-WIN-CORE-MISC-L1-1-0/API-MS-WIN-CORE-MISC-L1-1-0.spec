@ stdcall EnumSystemLocalesA(ptr long) kernelbase.EnumSystemLocalesA
@ stdcall FormatMessageA(long ptr long long ptr long ptr) kernelbase.FormatMessageA
@ stdcall FormatMessageW(long ptr long long ptr long ptr) kernelbase.FormatMessageW
@ stdcall GlobalAlloc(long long) kernelbase.GlobalAlloc
@ stdcall GlobalFree(long) kernelbase.GlobalFree
@ stdcall IsProcessInJob(long long ptr) kernelbase.IsProcessInJob
@ stdcall IsWow64Process(ptr ptr) kernelbase.IsWow64Process
@ stdcall FatalAppExitA(long str) kernelbase.FatalAppExitA
@ stdcall FatalAppExitW(long wstr) kernelbase.FatalAppExitW
@ stdcall FatalExit(long) kernelbase.FatalExit
@ stdcall LCMapStringA(long long str long ptr long) kernelbase.LCMapStringA
@ stdcall LCMapStringW(long long wstr long ptr long) kernelbase.LCMapStringW
@ stdcall LocalAlloc(long long) kernelbase.LocalAlloc
@ stdcall LocalCompact(long) kernelbase.LocalCompact
@ stdcall LocalFileTimeToFileTime(ptr ptr) kernelbase.LocalFileTimeToFileTime
@ stdcall LocalFlags(long) kernelbase.LocalFlags
@ stdcall LocalFree(long) kernelbase.LocalFree
@ stdcall LocalHandle(ptr) kernelbase.LocalHandle
@ stdcall LocalLock(long) kernelbase.LocalLock
@ stdcall LocalReAlloc(long long long) kernelbase.LocalReAlloc
@ stdcall LocalShrink(long long) kernelbase.LocalShrink
@ stdcall LocalSize(long) kernelbase.LocalSize
@ stdcall LocalUnlock(long) kernelbase.LocalUnlock
@ stdcall lstrcmp(str str) kernelbase.lstrcmp
@ stdcall lstrcmpA(str str) kernelbase.lstrcmpA
@ stdcall lstrcmpW(wstr wstr) kernelbase.lstrcmpW
@ stdcall lstrcmpi(str str) kernelbase.lstrcmpi
@ stdcall lstrcmpiA(str str) kernelbase.lstrcmpiA
@ stdcall lstrcmpiW(wstr wstr) kernelbase.lstrcmpiW
@ stdcall lstrcpy(ptr str) kernelbase.lstrcpy
@ stdcall lstrcpyA(ptr str) kernelbase.lstrcpyA
@ stdcall lstrcpyW(ptr wstr) kernelbase.lstrcpyW
@ stdcall lstrcpyn(ptr str long) kernelbase.lstrcpyn 
@ stdcall lstrcpynA(ptr str long) kernelbase.lstrcpynA
@ stdcall lstrcpynW(ptr wstr long) kernelbase.lstrcpynW
@ stdcall lstrlen(str) kernelbase.lstrlen
@ stdcall lstrlenA(str) kernelbase.lstrlenA
@ stdcall lstrlenW(wstr) kernelbase.lstrlenW
@ stdcall NeedCurrentDirectoryForExePathA(str) kernelbase.NeedCurrentDirectoryForExePathA
@ stdcall NeedCurrentDirectoryForExePathW(wstr) kernelbase.NeedCurrentDirectoryForExePathW
@ stdcall PulseEvent(long) kernelbase.PulseEvent
@ stdcall SetHandleCount(long) kernelbase.SetHandleCount
@ stdcall Sleep(long) kernelbase.Sleep
@ stdcall Wow64DisableWow64FsRedirection(ptr) kernelbase.Wow64DisableWow64FsRedirection
@ stdcall Wow64RevertWow64FsRedirection(ptr) kernelbase.Wow64RevertWow64FsRedirection