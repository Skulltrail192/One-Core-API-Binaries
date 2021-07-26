@ stdcall CancelIo(long) kernelbase.CancelIo
@ stdcall CancelIoEx(long ptr) kernelbase.CancelIoEx
@ stdcall CancelSynchronousIo(ptr) kernelbase.CancelSynchronousIo
@ stdcall CreateIoCompletionPort(long long long long) kernelbase.CreateIoCompletionPort
@ stdcall DeviceIoControl(long long ptr long ptr long ptr ptr) kernelbase.DeviceIoControl
@ stdcall GetOverlappedResult(long ptr ptr long) kernelbase.GetOverlappedResult
@ stdcall GetOverlappedResultEx(ptr ptr ptr long long) kernelbase.GetOverlappedResultEx
@ stdcall GetQueuedCompletionStatus(long ptr ptr ptr long) kernelbase.GetQueuedCompletionStatus
;@ stdcall GetQueuedCompletionStatusEx(ptr ptr long ptr long long) kernelbase.GetQueuedCompletionStatusEx
@ stdcall PostQueuedCompletionStatus(long long ptr ptr) kernelbase.PostQueuedCompletionStatus
