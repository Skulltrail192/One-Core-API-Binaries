@ stdcall InitializeSListHead(ptr) ntdll.RtlInitializeSListHead
@ stdcall -arch=i386 -ret64 InterlockedCompareExchange64(ptr double double) ntdll.RtlInterlockedCompareExchange64
@ stdcall -arch=i386 InterlockedCompareExchange(ptr long long) kernelbase.InterlockedCompareExchange
@ stdcall -arch=i386 InterlockedDecrement(ptr) kernelbase.InterlockedDecrement
@ stdcall -arch=i386 InterlockedExchange(ptr long) kernelbase.InterlockedExchange
@ stdcall -arch=i386 InterlockedExchangeAdd(ptr long) kernelbase.InterlockedExchangeAdd
@ stdcall -arch=i386 InterlockedIncrement(ptr) kernelbase.InterlockedIncrement
@ stdcall InterlockedFlushSList(ptr) ntdll.RtlInterlockedFlushSList
@ stdcall InterlockedPopEntrySList(ptr) ntdll.RtlInterlockedPopEntrySList
@ stdcall InterlockedPushEntrySList(ptr ptr) ntdll.RtlInterlockedPushEntrySList
@ stdcall InterlockedPushListSList(ptr ptr ptr long) ntdll.RtlInterlockedPushListSList
@ stdcall InterlockedPushListSListEx(ptr ptr ptr long) ntdll.RtlInterlockedPushListSList
@ stdcall QueryDepthSList(ptr) ntdll.RtlQueryDepthSList