@ stdcall CreateProcessA(str str ptr ptr long long ptr str ptr ptr) kernelbase.CreateProcessA
@ stdcall CreateProcessAsUserW(long wstr wstr ptr ptr long long ptr wstr ptr ptr) kernelbase.CreateProcessAsUserW
@ stdcall CreateProcessW(wstr wstr ptr ptr long long ptr wstr ptr ptr) kernelbase.CreateProcessW
@ stdcall CreateRemoteThread(long ptr long ptr long long ptr) kernelbase.CreateRemoteThread
@ stdcall CreateRemoteThreadEx(long ptr long ptr long long ptr ptr) kernelbase.CreateRemoteThreadEx
@ stdcall CreateThread(ptr long ptr long long ptr) kernelbase.CreateThread
@ stdcall DeleteProcThreadAttributeList(ptr) kernelbase.DeleteProcThreadAttributeList
@ stdcall ExitProcess(long) kernelbase.ExitProcess
@ stdcall ExitThread(long) kernelbase.ExitThread
@ stdcall FlushProcessWriteBuffers() kernelbase.FlushProcessWriteBuffers
@ stdcall GetCurrentProcess() kernelbase.GetCurrentProcess
@ stdcall GetCurrentProcessId() kernelbase.GetCurrentProcessId
@ stdcall GetCurrentThread() kernelbase.GetCurrentThread
@ stdcall GetCurrentThreadId() kernelbase.GetCurrentThreadId
@ stdcall GetExitCodeProcess(long ptr) kernelbase.GetExitCodeProcess
@ stdcall GetExitCodeThread(long ptr) kernelbase.GetExitCodeThread
@ stdcall GetPriorityClass(long) kernelbase.GetPriorityClass
@ stdcall GetProcessId(long) kernelbase.GetProcessId
@ stdcall GetProcessIdOfThread(ptr) kernelbase.GetProcessIdOfThread
@ stdcall GetProcessTimes(long ptr ptr ptr ptr) kernelbase.GetProcessTimes
@ stdcall GetProcessVersion(long) kernelbase.GetProcessVersion
@ stdcall GetStartupInfoW(ptr) kernelbase.GetStartupInfoW
@ stdcall GetThreadId(ptr) kernelbase.GetThreadId
@ stdcall GetThreadPriority(long) kernelbase.GetThreadPriority
@ stdcall GetThreadPriorityBoost(long ptr) kernelbase.GetThreadPriorityBoost
@ stdcall InitializeProcThreadAttributeList(ptr long long ptr) kernelbase.InitializeProcThreadAttributeList
@ stdcall OpenProcessToken(ptr long ptr) kernelbase.OpenProcessToken
@ stdcall OpenThread(long long long) kernelbase.OpenThread
@ stdcall OpenThreadToken(ptr long long ptr) kernelbase.OpenThreadToken
@ stdcall ProcessIdToSessionId(long ptr) kernelbase.ProcessIdToSessionId
@ stdcall QueryProcessAffinityUpdateMode(ptr long) kernelbase.QueryProcessAffinityUpdateMode
@ stdcall QueueUserAPC(ptr long long) kernelbase.QueueUserAPC
@ stdcall ResumeThread(long) kernelbase.ResumeThread
@ stdcall SetPriorityClass(long long) kernelbase.SetPriorityClass
@ stdcall SetProcessAffinityUpdateMode(ptr long) kernelbase.SetProcessAffinityUpdateMode
@ stdcall SetProcessShutdownParameters(long long) kernelbase.SetProcessShutdownParameters
@ stdcall SetThreadPriority(long long) kernelbase.SetThreadPriority
@ stdcall SetThreadPriorityBoost(long long) kernelbase.SetThreadPriorityBoost
@ stdcall SetThreadStackGuarantee(ptr) kernelbase.SetThreadStackGuarantee
@ stdcall SetThreadToken(ptr ptr) kernelbase.SetThreadToken
@ stdcall SuspendThread(long) kernelbase.SuspendThread
@ stdcall SwitchToThread() kernelbase.SwitchToThread
@ stdcall TerminateProcess(long long) kernelbase.TerminateProcess
@ stdcall TerminateThread(long long) kernelbase.TerminateThread
@ stdcall TlsAlloc() kernelbase.TlsAlloc
@ stdcall TlsFree(long) kernelbase.TlsFree
@ stdcall TlsGetValue(long) kernelbase.TlsGetValue
@ stdcall TlsSetValue(long ptr) kernelbase.TlsSetValue
@ stdcall UpdateProcThreadAttribute(ptr long long ptr long ptr ptr) kernelbase.UpdateProcThreadAttribute