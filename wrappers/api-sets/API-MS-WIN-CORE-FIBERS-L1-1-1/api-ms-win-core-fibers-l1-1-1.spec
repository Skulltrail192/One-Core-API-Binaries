@ stdcall ConvertFiberToThread() kernelbase.ConvertFiberToThread
@ stdcall ConvertThreadToFiber(ptr) kernelbase.ConvertThreadToFiber
@ stdcall CreateFiber(long ptr ptr) kernelbase.CreateFiber
@ stdcall DeleteFiber(ptr) kernelbase.DeleteFiber
@ stdcall FlsAlloc(ptr) kernelbase.FlsAlloc
@ stdcall FlsFree(long) kernelbase.FlsFree
@ stdcall FlsGetValue(long) kernelbase.FlsGetValue
@ stdcall FlsSetValue(long ptr) kernelbase.FlsSetValue
@ stdcall SwitchToFiber(ptr) kernelbase.SwitchToFiber
