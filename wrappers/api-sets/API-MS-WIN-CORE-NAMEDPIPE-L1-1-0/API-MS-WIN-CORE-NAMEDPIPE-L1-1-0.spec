@ stdcall ConnectNamedPipe(long ptr) kernelbase.ConnectNamedPipe
@ stdcall CreateNamedPipeW(wstr long long long long long long ptr) kernelbase.CreateNamedPipeW
@ stdcall DisconnectNamedPipe(long) kernelbase.DisconnectNamedPipe
@ stdcall GetNamedPipeAttribute(ptr long str ptr ptr) kernelbase.GetNamedPipeAttribute
@ stdcall GetNamedPipeClientComputerNameW(ptr wstr long) kernelbase.GetNamedPipeClientComputerNameW
@ stdcall CreatePipe(ptr ptr ptr long) kernelbase.CreatePipe
@ stdcall ImpersonateNamedPipeClient(long) kernelbase.ImpersonateNamedPipeClient
@ stdcall PeekNamedPipe(long ptr long ptr ptr ptr) kernelbase.PeekNamedPipe
@ stdcall SetNamedPipeHandleState(long ptr ptr ptr) kernelbase.SetNamedPipeHandleState
@ stdcall TransactNamedPipe(long ptr long ptr long ptr ptr) kernelbase.TransactNamedPipe
@ stdcall WaitNamedPipeW(wstr long) kernelbase.WaitNamedPipeW