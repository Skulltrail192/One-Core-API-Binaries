@ stdcall DavAddConnection(ptr wstr wstr wstr ptr long)
@ stdcall DavDeleteConnection(ptr)
@ stdcall DavFlushFile(ptr)
@ stdcall DavGetExtendedError(ptr ptr wstr ptr)
@ stdcall DavGetHTTPFromUNCPath(wstr wstr ptr)
@ stdcall DavGetUNCFromHTTPPath(wstr wstr ptr)
@ stub I_DsUpdateReadOnlyServerDnsRecords 
@ stub I_NetChainSetClientAttributes
@ stdcall NetDfsAddRootTarget(wstr wstr long wstr long)
@ stdcall NetDfsGetSupportedNamespaceVersion(long wstr ptr)
@ stdcall NetDfsRemoveRootTarget(wstr wstr long)
@ stdcall NetpAccessCheckAndAuditEx(ptr ptr ptr ptr ptr ptr ptr)
@ stdcall NetpEventlogClose(ptr)
@ stdcall NetpEventlogOpen(wstr long)
@ stdcall NetpEventlogWriteEx(ptr long long long long long long)
@ stdcall NetShareDelEx(wstr long ptr)
@ stdcall NetServerAliasAdd(wstr long ptr)
@ stdcall NetServerAliasDel(wstr long ptr)
@ stdcall NetServerAliasEnum(wstr long ptr long ptr ptr ptr)


