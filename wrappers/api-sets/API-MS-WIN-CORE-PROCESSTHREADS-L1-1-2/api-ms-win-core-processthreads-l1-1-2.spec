@ stdcall CreateProcessA(str str ptr ptr long long ptr str ptr ptr) kernelbase.CreateProcessA
@ stdcall CreateProcessAsUserW(long str str ptr ptr long long ptr str ptr ptr) kernelbase.CreateProcessAsUserW
@ stdcall CreateProcessW(wstr wstr ptr ptr long long ptr wstr ptr ptr) kernelbase.CreateProcessW
@ stdcall CreateRemoteThread(long ptr long ptr long long ptr) kernelbase.CreateRemoteThread
@ stdcall CreateRemoteThreadEx(long ptr long ptr long long ptr ptr) kernelbase.CreateRemoteThreadEx
@ stdcall CreateThread(ptr long ptr long long ptr) kernelbase.CreateThread
@ stdcall DeleteProcThreadAttributeList(ptr) kernelbase.DeleteProcThreadAttributeList
@ stdcall ExitProcess(long) kernelbase.ExitProcess
@ stdcall ExitThread(long) kernelbase.ExitThread
@ stdcall FlushInstructionCache(long long long) kernelbase.FlushInstructionCache
@ stdcall FlushProcessWriteBuffers() kernelbase.FlushProcessWriteBuffers
@ stdcall -norelay GetCurrentProcess() kernelbase.GetCurrentProcess
@ stdcall -norelay GetCurrentProcessId() kernelbase.GetCurrentProcessId
@ stdcall GetCurrentProcessorNumber() kernelbase.GetCurrentProcessorNumber
@ stdcall GetCurrentProcessorNumberEx(ptr) kernelbase.GetCurrentProcessorNumberEx
@ stdcall -norelay GetCurrentThread() kernelbase.GetCurrentThread
@ stdcall -norelay GetCurrentThreadId() kernelbase.GetCurrentThreadId
@ stdcall GetCurrentThreadStackLimits(ptr ptr) kernelbase.GetCurrentThreadStackLimits
@ stdcall GetExitCodeProcess(long ptr) kernelbase.GetExitCodeProcess
@ stdcall GetExitCodeThread(long ptr) kernelbase.GetExitCodeThread
@ stdcall GetPriorityClass(long) kernelbase.GetPriorityClass
@ stdcall GetProcessHandleCount(long ptr) kernelbase.GetProcessHandleCount
@ stdcall GetProcessId(long) kernelbase.GetProcessId
@ stdcall GetProcessIdOfThread(ptr) kernelbase.GetProcessIdOfThread
@ stdcall GetProcessMitigationPolicy(ptr long ptr long) kernelbase.GetProcessMitigationPolicy
@ stdcall GetProcessPriorityBoost(long ptr) kernelbase.GetProcessPriorityBoost
@ stdcall GetProcessTimes(long ptr ptr ptr ptr) kernelbase.GetProcessTimes
@ stdcall GetProcessVersion(long) kernelbase.GetProcessVersion
@ stdcall GetStartupInfoW(ptr) kernelbase.GetStartupInfoW
@ stdcall GetSystemTimes(ptr ptr ptr)
@ stdcall GetThreadContext(long ptr) kernelbase.GetThreadContext
@ stdcall GetThreadIOPendingFlag(long ptr)
@ stdcall GetThreadId(ptr) kernelbase.GetThreadId
@ stdcall GetThreadIdealProcessorEx(ptr ptr) kernelbase.GetThreadIdealProcessorEx
@ stdcall GetThreadInformation(ptr long ptr long)
@ stdcall GetThreadPriority(long) kernelbase.GetThreadPriority
@ stdcall GetThreadPriorityBoost(long ptr) kernelbase.GetThreadPriorityBoost
@ stdcall GetThreadTimes(long ptr ptr ptr ptr) kernelbase.GetThreadTimes
@ stdcall InitializeProcThreadAttributeList(ptr long long ptr) kernelbase.InitializeProcThreadAttributeList
@ stdcall IsProcessCritical(ptr ptr) kernelbase.IsProcessCritical
@ stdcall IsProcessorFeaturePresent(long) kernelbase.IsProcessorFeaturePresent
@ stdcall OpenProcess(long long long) kernelbase.OpenProcess
@ stdcall OpenProcessToken(ptr long ptr) kernelbase.OpenProcessToken
@ stdcall OpenThread(long long long) kernelbase.OpenThread
@ stdcall OpenThreadToken(ptr long long ptr) kernelbase.OpenThreadToken
@ stdcall ProcessIdToSessionId(long ptr) kernelbase.ProcessIdToSessionId
@ stdcall QueryProcessAffinityUpdateMode(ptr long) kernelbase.QueryProcessAffinityUpdateMode
@ stdcall QueueUserAPC(ptr long long) kernelbase.QueueUserAPC
@ stdcall ResumeThread(long) kernelbase.ResumeThread
@ stdcall SetPriorityClass(long long) kernelbase.SetPriorityClass
@ stdcall SetProcessAffinityUpdateMode(ptr long) kernelbase.SetProcessAffinityUpdateMode
@ stdcall SetProcessMitigationPolicy(ptr ptr long) kernelbase.SetProcessMitigationPolicy
@ stdcall SetProcessPriorityBoost(long long) kernelbase.SetProcessPriorityBoost
@ stdcall SetProcessShutdownParameters(long long) kernelbase.SetProcessShutdownParameters
@ stdcall SetThreadContext(long ptr) kernelbase.SetThreadContext
@ stdcall SetThreadIdealProcessorEx(ptr ptr ptr) kernelbase.SetThreadIdealProcessorEx
@ stdcall SetThreadInformation(ptr long ptr long) kernelbase.SetThreadInformation
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