@ stdcall ConnectNamedPipe(long ptr) kernelbase.ConnectNamedPipe
@ stdcall CreateNamedPipeW(wstr long long long long long long ptr) kernelbase.CreateNamedPipeW
@ stdcall CreatePipe(ptr ptr ptr long) kernelbase.CreatePipe
@ stdcall DisconnectNamedPipe(long) kernelbase.DisconnectNamedPipe
@ stdcall GetNamedPipeClientComputerNameW(ptr wstr long) kernelbase.GetNamedPipeClientComputerNameW
@ stdcall ImpersonateNamedPipeClient(long) advapi32.ImpersonateNamedPipeClient
@ stdcall PeekNamedPipe(long ptr long ptr ptr ptr) kernelbase.PeekNamedPipe
@ stdcall SetNamedPipeHandleState(long ptr ptr ptr) kernelbase.SetNamedPipeHandleState
@ stdcall TransactNamedPipe(long ptr long ptr long ptr ptr) kernelbase.TransactNamedPipe
@ stdcall WaitNamedPipeW(wstr long) kernelbase.WaitNamedPipeW
