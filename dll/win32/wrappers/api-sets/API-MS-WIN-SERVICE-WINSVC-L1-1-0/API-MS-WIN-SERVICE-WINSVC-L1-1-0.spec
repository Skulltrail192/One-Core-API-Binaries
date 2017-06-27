@ stdcall ChangeServiceConfig2A(long long ptr) advapi32.ChangeServiceConfig2A
@ stdcall ChangeServiceConfig2W(long long ptr) advapi32.ChangeServiceConfig2W
@ stdcall ChangeServiceConfigA(long long long long wstr wstr ptr wstr wstr wstr wstr) advapi32.ChangeServiceConfigA
@ stdcall ChangeServiceConfigW(long long long long wstr wstr ptr wstr wstr wstr wstr) advapi32.ChangeServiceConfigW
@ stdcall CloseServiceHandle(long) advapi32.CloseServiceHandle
@ stdcall ControlService(long long ptr) advapi32.ControlService
@ stdcall ControlServiceExA(ptr long long ptr) advapi32.ControlServiceExA
@ stdcall ControlServiceExW(ptr long long ptr) advapi32.ControlServiceExW
@ stdcall CreateServiceA(long str str long long long long str str ptr str str str) advapi32.CreateServiceA
@ stdcall I_QueryTagInformation(long long long) advapi32.I_QueryTagInformation
@ stdcall I_ScQueryServiceConfig(long long long) advapi32.I_ScQueryServiceConfig
@ stdcall I_ScSendPnPMessage(long long long long long) advapi32.I_ScSendPnPMessage
@ stdcall I_ScValidatePnPService(long long long) advapi32.I_ScValidatePnPService
@ stdcall NotifyServiceStatusChangeA(ptr long ptr) advapi32.NotifyServiceStatusChangeA
@ stdcall OpenSCManagerA(str str long) advapi32.NotifyServiceStatusChangeA
@ stdcall OpenSCManagerW(wstr wstr long) advapi32.OpenSCManagerW
@ stdcall OpenServiceA(long str long) advapi32.OpenServiceA
@ stdcall OpenServiceW(long wstr long) advapi32.OpenServiceW
@ stdcall QueryServiceConfig2A(long long ptr long ptr) advapi32.QueryServiceConfig2A
@ stdcall QueryServiceConfigA(long ptr long ptr) advapi32.QueryServiceConfigA
@ stdcall QueryServiceConfigW(long ptr long ptr) advapi32.QueryServiceConfigW
@ stdcall QueryServiceStatus(long ptr) advapi32.QueryServiceStatus
@ stdcall RegisterServiceCtrlHandlerA(str ptr) advapi32.RegisterServiceCtrlHandlerA
@ stdcall RegisterServiceCtrlHandlerExA(str ptr ptr) advapi32.RegisterServiceCtrlHandlerExA
@ stdcall RegisterServiceCtrlHandlerW(wstr ptr) advapi32.RegisterServiceCtrlHandlerW
@ stdcall StartServiceA(long long ptr) advapi32.StartServiceA
@ stdcall StartServiceW(long long ptr) advapi32.StartServiceW
@ stdcall StartServiceCtrlDispatcherA(ptr) advapi32.StartServiceCtrlDispatcherA
@ stub I_ScSendTSMessage
@ stub I_ScBroadcastServiceControlMessage
@ stub I_ScIsSecurityProcess
@ stub I_ScPnPGetServiceName
@ stub I_ScRpcBindA
@ stub I_ScRpcBindW