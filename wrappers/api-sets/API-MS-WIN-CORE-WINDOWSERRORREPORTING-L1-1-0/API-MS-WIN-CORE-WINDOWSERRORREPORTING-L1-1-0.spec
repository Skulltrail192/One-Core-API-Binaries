@ stub GetApplicationRecoveryCallback  ;(ptr ptr ptr ptr ptr) kernelbase.GetApplicationRecoveryCallback
@ stdcall GetApplicationRestartSettings(ptr wstr ptr ptr) kernelbase.GetApplicationRestartSettings
@ stdcall WerpNotifyLoadStringResource(ptr wstr ptr long) kernelbase.WerpNotifyLoadStringResource
@ stdcall WerpNotifyUseStringResource(ptr) kernelbase.WerpNotifyUseStringResource
@ stdcall WerRegisterFile(wstr long long) kernelbase.WerRegisterFile
@ stdcall WerRegisterMemoryBlock(ptr long) kernelbase.WerRegisterMemoryBlock
@ stdcall WerRegisterRuntimeExceptionModule(wstr ptr) kernelbase.WerRegisterRuntimeExceptionModule
@ stdcall WerUnregisterFile(wstr) kernelbase.WerUnregisterFile
@ stdcall WerUnregisterMemoryBlock(ptr) kernelbase.WerUnregisterMemoryBlock
@ stdcall WerUnregisterRuntimeExceptionModule(wstr ptr) kernelbase.WerUnregisterRuntimeExceptionModule
