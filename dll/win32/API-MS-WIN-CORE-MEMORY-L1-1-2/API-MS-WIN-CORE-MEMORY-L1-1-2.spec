@ stdcall AllocateUserPhysicalPages(long ptr ptr) kernel32.AllocateUserPhysicalPages
@ stdcall AllocateUserPhysicalPagesNuma(ptr ptr ptr long) kernel32.AllocateUserPhysicalPagesNuma
@ stdcall FreeUserPhysicalPages(long ptr ptr) kernel32.FreeUserPhysicalPages
@ stdcall GetMemoryErrorHandlingCapabilities(ptr) kernel32.GetMemoryErrorHandlingCapabilities
@ stdcall MapUserPhysicalPages(ptr long ptr) kernel32.MapUserPhysicalPages
@ stdcall RegisterBadMemoryNotification(ptr) kernel32.RegisterBadMemoryNotification
@ stdcall UnregisterBadMemoryNotification(ptr) kernel32.UnregisterBadMemoryNotification
@ stdcall VirtualAllocExNuma(ptr ptr long long long long) kernel32.VirtualAllocExNuma
@ stdcall VirtualAlloc(ptr long long long) kernel32.VirtualAlloc
@ stdcall VirtualAllocEx(long ptr long long long) kernel32.VirtualAllocEx
@ stdcall VirtualFree(ptr long long) kernel32.VirtualFree
@ stdcall VirtualFreeEx(long ptr long long) kernel32.VirtualFreeEx
@ stdcall VirtualLock(ptr long) kernel32.VirtualLock
@ stdcall VirtualProtect(ptr long long ptr) kernel32.VirtualProtect
@ stdcall VirtualProtectEx(long ptr long long ptr) kernel32.VirtualProtectEx
@ stdcall VirtualQuery(ptr ptr long) kernel32.VirtualQuery
@ stdcall VirtualQueryEx(long ptr ptr long) kernel32.VirtualQueryEx
@ stdcall VirtualUnlock(ptr long) kernel32.VirtualUnlock
