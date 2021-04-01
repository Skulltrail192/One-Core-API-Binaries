@ stdcall CreateFileMappingFromApp(ptr ptr long int64 wstr) kernelbase.CreateFileMappingFromApp
@ stdcall CreateFileMappingNumaW(ptr ptr long long long wstr long) kernelbase.CreateFileMappingNumaW
@ stdcall CreateFileMappingW(long ptr long long long wstr) kernelbase.CreateFileMappingW
@ stdcall CreateMemoryResourceNotification(long) kernelbase.CreateMemoryResourceNotification
@ stdcall FlushViewOfFile(ptr long) kernelbase.FlushViewOfFile
@ stdcall GetLargePageMinimum() kernelbase.GetLargePageMinimum
@ stdcall GetProcessWorkingSetSizeEx(long ptr ptr long) kernelbase.GetProcessWorkingSetSizeEx
@ stdcall GetSystemFileCacheSize(ptr ptr ptr) kernelbase.GetSystemFileCacheSize
@ stdcall GetWriteWatch(long ptr long ptr ptr ptr) kernelbase.GetWriteWatch
@ stdcall MapViewOfFile(long long long long long) kernelbase.MapViewOfFile
@ stdcall MapViewOfFileEx(long long long long long ptr) kernelbase.MapViewOfFileEx
@ stdcall MapViewOfFileFromApp(ptr long int64 long) kernelbase.MapViewOfFileFromApp
@ stdcall OpenFileMappingW(long long wstr) kernelbase.OpenFileMappingW
@ stdcall PrefetchVirtualMemory(ptr long ptr long) kernelbase.PrefetchVirtualMemory
@ stdcall QueryMemoryResourceNotification(ptr ptr) kernelbase.QueryMemoryResourceNotification
@ stdcall ReadProcessMemory(long ptr ptr long ptr) kernelbase.ReadProcessMemory
@ stdcall ResetWriteWatch(ptr long) kernelbase.ResetWriteWatch
@ stdcall SetProcessWorkingSetSizeEx(long long long long) kernelbase.SetProcessWorkingSetSizeEx
@ stdcall SetSystemFileCacheSize(long long long) kernelbase.SetSystemFileCacheSize
@ stdcall UnmapViewOfFile(ptr) kernelbase.UnmapViewOfFile
@ stdcall UnmapViewOfFileEx(ptr long) kernelbase.UnmapViewOfFileEx
@ stdcall VirtualAlloc(ptr long long long) kernelbase.VirtualAlloc
@ stdcall VirtualAllocEx(long ptr long long long) kernelbase.VirtualAllocEx
@ stdcall VirtualFree(ptr long long) kernelbase.VirtualFree
@ stdcall VirtualFreeEx(long ptr long long) kernelbase.VirtualFreeEx
@ stdcall VirtualLock(ptr long) kernelbase.VirtualLock
@ stdcall VirtualProtect(ptr long long ptr) kernelbase.VirtualProtect
@ stdcall VirtualProtectEx(long ptr long long ptr) kernelbase.VirtualProtectEx
@ stdcall VirtualQuery(ptr ptr long) kernelbase.VirtualQuery
@ stdcall VirtualQueryEx(long ptr ptr long) kernelbase.VirtualQueryEx
@ stdcall VirtualUnlock(ptr long) kernelbase.VirtualUnlock
@ stdcall WriteProcessMemory(long ptr ptr long ptr) kernelbase.WriteProcessMemory
