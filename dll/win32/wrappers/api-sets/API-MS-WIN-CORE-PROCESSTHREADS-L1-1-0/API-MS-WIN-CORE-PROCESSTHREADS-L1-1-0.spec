@ stdcall CreateProcessA(str str ptr ptr long long ptr str ptr ptr) kernel32.CreateProcessA
@ stdcall CreateProcessW(wstr wstr ptr ptr long long ptr wstr ptr ptr) kernel32.CreateProcessW
@ stdcall CreateThread(ptr long ptr long long ptr) kernel32.CreateThread
@ stdcall ExitProcess(long) kernel32.ExitProcess
@ stdcall ExitThread(long) kernel32.ExitThread
@ stdcall GetCurrentProcess() kernel32.GetCurrentProcess
@ stdcall GetCurrentProcessId() kernel32.GetCurrentProcessId
@ stdcall GetCurrentThread() kernel32.GetCurrentThread
@ stdcall GetCurrentThreadId() kernel32.GetCurrentThreadId
@ stdcall GetExitCodeProcess(long ptr) kernel32.GetExitCodeProcess
@ stdcall GetStartupInfoW(ptr) kernel32.GetStartupInfoW
@ stdcall ResumeThread(long) kernel32.ResumeThread
@ stdcall SetThreadStackGuarantee(ptr) kernel32.SetThreadStackGuarantee
@ stdcall TerminateProcess(long long) kernel32.TerminateProcess
@ stdcall TlsAlloc() kernel32.TlsAlloc
@ stdcall TlsFree(long) kernel32.TlsFree
@ stdcall TlsGetValue(long) kernel32.TlsGetValue
@ stdcall TlsSetValue(long ptr) kernel32.TlsSetValue