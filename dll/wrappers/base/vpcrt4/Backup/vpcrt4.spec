@ stdcall CreateProxyFromTypeInfo(ptr ptr ptr ptr ptr)
@ stdcall CreateStubFromTypeInfo(ptr ptr ptr ptr)
@ stdcall I_RpcServerTurnOnOffKeepalives() rpcrtbase.I_RpcServerTurnOnOffKeepalives
@ stdcall CStdStubBuffer_AddRef(ptr) 
@ stdcall CStdStubBuffer_Connect(ptr ptr) 
@ stdcall CStdStubBuffer_CountRefs(ptr) 
@ stdcall CStdStubBuffer_DebugServerQueryInterface(ptr ptr)
@ stdcall CStdStubBuffer_DebugServerRelease(ptr ptr)
@ stdcall CStdStubBuffer_Disconnect(ptr)
@ stdcall CStdStubBuffer_Invoke(ptr ptr ptr)
@ stdcall CStdStubBuffer_IsIIDSupported(ptr ptr)
@ stdcall CStdStubBuffer_QueryInterface(ptr ptr ptr)
@ stdcall DceErrorInqTextA (long ptr)
@ stdcall DceErrorInqTextW (long ptr)
@ stdcall DllGetClassObject() rpcrtbase.DllGetClassObject
@ stdcall DllInstall() rpcrtbase.DllInstall
#@ stdcall DllRegisterServer() rpcrtbase.DllRegisterServer
@ stdcall GlobalMutexClearExternal() rpcrtbase.GlobalMutexClearExternal
@ stdcall GlobalMutexRequestExternal() rpcrtbase.GlobalMutexRequestExternal
@ stdcall IUnknown_AddRef_Proxy(ptr)
@ stdcall IUnknown_QueryInterface_Proxy(ptr ptr ptr)
@ stdcall IUnknown_Release_Proxy(ptr)
@ stdcall I_RpcAbortAsyncCall(ptr long) I_RpcAsyncAbortCall
@ stdcall I_RpcAllocate(long)
@ stdcall I_RpcAsyncAbortCall(ptr long)
@ stdcall I_RpcAsyncSetHandle(ptr ptr)
@ stdcall I_RpcBCacheAllocate() rpcrtbase.I_RpcBCacheAllocate
@ stdcall I_RpcBCacheFree() rpcrtbase.I_RpcBCacheFree
@ stdcall I_RpcBindingCopy() rpcrtbase.I_RpcBindingCopy
@ stdcall I_RpcBindingHandleToAsyncHandle() rpcrtbase.I_RpcBindingHandleToAsyncHandle
@ stdcall I_RpcBindingInqConnId() rpcrtbase.I_RpcBindingInqConnId
@ stdcall I_RpcBindingInqDynamicEndpoint() rpcrtbase.I_RpcBindingInqDynamicEndpoint
@ stdcall I_RpcBindingInqDynamicEndpointA() rpcrtbase.I_RpcBindingInqDynamicEndpointA
@ stdcall I_RpcBindingInqDynamicEndpointW() rpcrtbase.I_RpcBindingInqDynamicEndpointW
@ stdcall I_RpcBindingInqLocalClientPID() rpcrtbase.I_RpcBindingInqLocalClientPID 
@ stdcall I_RpcBindingInqMarshalledTargetInfo() rpcrtbase.I_RpcBindingInqMarshalledTargetInfo
@ stdcall I_RpcBindingInqSecurityContext() rpcrtbase.I_RpcBindingInqSecurityContext
@ stdcall I_RpcBindingInqTransportType(ptr ptr)
@ stdcall I_RpcBindingInqWireIdForSnego() rpcrtbase.I_RpcBindingInqWireIdForSnego
@ stdcall I_RpcBindingIsClientLocal() rpcrtbase.I_RpcBindingIsClientLocal
@ stdcall I_RpcBindingToStaticStringBindingW() rpcrtbase.I_RpcBindingToStaticStringBindingW
@ stdcall I_RpcClearMutex() rpcrtbase.I_RpcClearMutex
@ stdcall I_RpcConnectionInqSockBuffSize() rpcrtbase.I_RpcConnectionInqSockBuffSize
@ stdcall I_RpcConnectionSetSockBuffSize() rpcrtbase.I_RpcConnectionSetSockBuffSize
@ stdcall I_RpcDeleteMutex() rpcrtbase.I_RpcDeleteMutex
@ stdcall I_RpcEnableWmiTrace() rpcrtbase.I_RpcEnableWmiTrace 
@ stdcall I_RpcExceptionFilter(long)
@ stdcall I_RpcFree(ptr)
@ stdcall I_RpcFreeBuffer(ptr)
@ stdcall I_RpcFreePipeBuffer() rpcrtbase.I_RpcFreePipeBuffer
@ stdcall I_RpcGetBuffer(ptr)
@ stdcall I_RpcGetBufferWithObject() rpcrtbase.I_RpcGetBufferWithObject
@ stdcall I_RpcGetCurrentCallHandle()
@ stdcall I_RpcGetExtendedError() rpcrtbase.I_RpcGetExtendedError
@ stdcall I_RpcIfInqTransferSyntaxes() rpcrtbase.I_RpcIfInqTransferSyntaxes
@ stdcall I_RpcLogEvent() rpcrtbase.I_RpcLogEvent
@ stdcall I_RpcMapWin32Status(long)
@ stdcall I_RpcNDRCGetWireRepresentation() rpcrtbase.I_RpcNDRCGetWireRepresentation
@ stdcall I_RpcNDRSContextEmergencyCleanup() rpcrtbase.I_RpcNDRSContextEmergencyCleanup
@ stdcall I_RpcNegotiateTransferSyntax(ptr)
@ stdcall I_RpcNsBindingSetEntryName() rpcrtbase.I_RpcNsBindingSetEntryName
@ stdcall I_RpcNsBindingSetEntryNameA() rpcrtbase.I_RpcNsBindingSetEntryNameA
@ stdcall I_RpcNsBindingSetEntryNameW() rpcrtbase.I_RpcNsBindingSetEntryNameW
@ stdcall I_RpcNsInterfaceExported() rpcrtbase.I_RpcNsInterfaceExported
@ stdcall I_RpcNsInterfaceUnexported() rpcrtbase.I_RpcNsInterfaceUnexported
@ stdcall I_RpcParseSecurity() rpcrtbase.I_RpcParseSecurity
@ stdcall I_RpcPauseExecution() rpcrtbase.I_RpcPauseExecution
@ stdcall I_RpcProxyNewConnection() rpcrtbase.I_RpcProxyNewConnection 
@ stdcall I_RpcReallocPipeBuffer() rpcrtbase.I_RpcReallocPipeBuffer
@ stdcall I_RpcReceive(ptr)
@ stdcall I_RpcRecordCalloutFailure() rpcrtbase.I_RpcRecordCalloutFailure
@ stdcall I_RpcReplyToClientWithStatus() rpcrtbase.I_RpcReplyToClientWithStatus
@ stdcall I_RpcRequestMutex() rpcrtbase.I_RpcRequestMutex
#@ stdcall I_RpcSNCHOption() rpcrtbase.I_RpcSNCHOption
@ stub I_RpcSNCHOption
@ stdcall I_RpcSend(ptr)
@ stdcall I_RpcSendReceive(ptr)
@ stdcall I_RpcServerAllocateIpPort() rpcrtbase.I_RpcServerAllocateIpPort
@ stdcall I_RpcServerCheckClientRestriction() rpcrtbase.I_RpcServerCheckClientRestriction
@ stdcall I_RpcServerInqAddressChangeFn() rpcrtbase.I_RpcServerInqAddressChangeFn
@ stdcall I_RpcServerInqLocalConnAddress() rpcrtbase.I_RpcServerInqLocalConnAddress 
@ stdcall I_RpcServerInqTransportType() rpcrtbase.I_RpcServerInqTransportType
@ stdcall I_RpcServerIsClientDisconnected() rpcrtbase.I_RpcServerIsClientDisconnected
@ stdcall I_RpcServerRegisterForwardFunction() rpcrtbase.I_RpcServerRegisterForwardFunction
@ stdcall I_RpcServerSetAddressChangeFn() rpcrtbase.I_RpcServerSetAddressChangeFn
@ stdcall I_RpcServerUseProtseq2A() rpcrtbase.I_RpcServerUseProtseq2A
@ stdcall I_RpcServerUseProtseq2W() rpcrtbase.I_RpcServerUseProtseq2W
@ stdcall I_RpcServerUseProtseqEp2A() rpcrtbase.I_RpcServerUseProtseqEp2A
@ stdcall I_RpcServerUseProtseqEp2W() rpcrtbase.I_RpcServerUseProtseqEp2W
@ stdcall I_RpcSessionStrictContextHandle() rpcrtbase.I_RpcSessionStrictContextHandle
@ stdcall I_RpcSetAsyncHandle() rpcrtbase.I_RpcSetAsyncHandle
@ stdcall I_RpcSsDontSerializeContext() rpcrtbase.I_RpcSsDontSerializeContext
@ stdcall I_RpcSystemFunction001() rpcrtbase.I_RpcSystemFunction001  
@ stdcall I_RpcTransConnectionAllocatePacket() rpcrtbase.I_RpcTransConnectionAllocatePacket
@ stdcall I_RpcTransConnectionFreePacket() rpcrtbase.I_RpcTransConnectionFreePacket
@ stdcall I_RpcTransConnectionReallocPacket() rpcrtbase.I_RpcTransConnectionReallocPacket
@ stdcall I_RpcTransDatagramAllocate2() rpcrtbase.I_RpcTransDatagramAllocate2
@ stdcall I_RpcTransDatagramAllocate() rpcrtbase.I_RpcTransDatagramAllocate
@ stdcall I_RpcTransDatagramFree() rpcrtbase.I_RpcTransDatagramFree
@ stdcall I_RpcTransGetThreadEvent() rpcrtbase.I_RpcTransGetThreadEvent
@ stdcall I_RpcTransIoCancelled() rpcrtbase.I_RpcTransIoCancelled
@ stdcall I_RpcTransServerNewConnection() rpcrtbase.I_RpcTransServerNewConnection
@ stdcall I_RpcTurnOnEEInfoPropagation() rpcrtbase.I_RpcTurnOnEEInfoPropagation 
@ stdcall I_UuidCreate() rpcrtbase.I_UuidCreate
@ stdcall MIDL_wchar_strcpy() rpcrtbase.MIDL_wchar_strcpy
@ stdcall MIDL_wchar_strlen() rpcrtbase.MIDL_wchar_strlen
@ stdcall MesBufferHandleReset() rpcrtbase.MesBufferHandleReset
@ stdcall MesDecodeBufferHandleCreate(ptr long ptr)
@ stdcall MesDecodeIncrementalHandleCreate(ptr ptr ptr)
@ stdcall MesEncodeDynBufferHandleCreate(ptr ptr ptr)
@ stdcall MesEncodeFixedBufferHandleCreate(ptr long ptr ptr)
@ stdcall MesEncodeIncrementalHandleCreate(ptr ptr ptr ptr)
@ stdcall MesHandleFree(ptr)
@ stdcall MesIncrementalHandleReset(ptr ptr ptr ptr ptr long)
@ stdcall MesInqProcEncodingId() rpcrtbase.MesInqProcEncodingId
@ stdcall -arch=x86_64 Ndr64AsyncClientCall() rpcrtbase.Ndr64AsyncClientCall
@ stdcall -arch=x86_64 Ndr64AsyncServerCall64() rpcrtbase.Ndr64AsyncServerCall64
@ stdcall -arch=x86_64 Ndr64AsyncServerCallAll() rpcrtbase.Ndr64AsyncServerCallAll
@ stdcall -arch=x86_64 Ndr64DcomAsyncClientCall() rpcrtbase.Ndr64DcomAsyncClientCall
@ stdcall -arch=x86_64 Ndr64DcomAsyncStubCall() rpcrtbase.Ndr64DcomAsyncStubCall
@ stdcall NDRCContextBinding(ptr)
@ stdcall NDRCContextMarshall(ptr ptr)
@ stdcall NDRCContextUnmarshall(ptr ptr ptr long)
@ stdcall NDRSContextMarshall2(ptr ptr ptr ptr ptr long)
@ stdcall NDRSContextMarshall(ptr ptr ptr)
@ stdcall NDRSContextMarshallEx(ptr ptr ptr ptr)
@ stdcall NDRSContextUnmarshall2(ptr ptr ptr ptr long)
@ stdcall NDRSContextUnmarshall(ptr ptr)
@ stdcall NDRSContextUnmarshallEx(ptr ptr ptr)
@ stdcall NDRcopy() rpcrtbase.NDRcopy
@ stdcall NdrAllocate(ptr long)
@ varargs NdrAsyncClientCall(ptr ptr)
@ stdcall NdrAsyncServerCall() rpcrtbase.NdrAsyncServerCall
@ stdcall NdrByteCountPointerBufferSize(ptr ptr ptr)
@ stdcall NdrByteCountPointerFree(ptr ptr ptr)
@ stdcall NdrByteCountPointerMarshall(ptr ptr ptr)
@ stdcall NdrByteCountPointerUnmarshall(ptr ptr ptr long)
@ stdcall NdrCStdStubBuffer2_Release(ptr ptr)
@ stdcall NdrCStdStubBuffer_Release(ptr ptr)
@ stdcall NdrClearOutParameters(ptr ptr ptr)
@ varargs -arch=i386 NdrClientCall(ptr ptr) NdrClientCall2
@ varargs NdrClientCall2(ptr ptr)
@ varargs -arch=x86_64 NdrClientCall3(ptr ptr)
@ stdcall NdrClientContextMarshall(ptr ptr long)
@ stdcall NdrClientContextUnmarshall(ptr ptr ptr)
@ stdcall NdrClientInitialize() rpcrtbase.NdrClientInitialize
@ stdcall NdrClientInitializeNew(ptr ptr ptr long)
@ stdcall NdrComplexArrayBufferSize(ptr ptr ptr)
@ stdcall NdrComplexArrayFree(ptr ptr ptr)
@ stdcall NdrComplexArrayMarshall(ptr ptr ptr)
@ stdcall NdrComplexArrayMemorySize(ptr ptr)
@ stdcall NdrComplexArrayUnmarshall(ptr ptr ptr long)
@ stdcall NdrComplexStructBufferSize(ptr ptr ptr)
@ stdcall NdrComplexStructFree(ptr ptr ptr)
@ stdcall NdrComplexStructMarshall(ptr ptr ptr)
@ stdcall NdrComplexStructMemorySize(ptr ptr)
@ stdcall NdrComplexStructUnmarshall(ptr ptr ptr long)
@ stdcall NdrConformantArrayBufferSize(ptr ptr ptr)
@ stdcall NdrConformantArrayFree(ptr ptr ptr)
@ stdcall NdrConformantArrayMarshall(ptr ptr ptr)
@ stdcall NdrConformantArrayMemorySize(ptr ptr)
@ stdcall NdrConformantArrayUnmarshall(ptr ptr ptr long)
@ stdcall NdrConformantStringBufferSize(ptr ptr ptr)
@ stdcall NdrConformantStringMarshall(ptr ptr ptr)
@ stdcall NdrConformantStringMemorySize(ptr ptr)
@ stdcall NdrConformantStringUnmarshall(ptr ptr ptr long)
@ stdcall NdrConformantStructBufferSize(ptr ptr ptr)
@ stdcall NdrConformantStructFree(ptr ptr ptr)
@ stdcall NdrConformantStructMarshall(ptr ptr ptr)
@ stdcall NdrConformantStructMemorySize(ptr ptr)
@ stdcall NdrConformantStructUnmarshall(ptr ptr ptr long)
@ stdcall NdrConformantVaryingArrayBufferSize(ptr ptr ptr)
@ stdcall NdrConformantVaryingArrayFree(ptr ptr ptr)
@ stdcall NdrConformantVaryingArrayMarshall(ptr ptr ptr)
@ stdcall NdrConformantVaryingArrayMemorySize(ptr ptr)
@ stdcall NdrConformantVaryingArrayUnmarshall(ptr ptr ptr long)
@ stdcall NdrConformantVaryingStructBufferSize(ptr ptr ptr)
@ stdcall NdrConformantVaryingStructFree(ptr ptr ptr)
@ stdcall NdrConformantVaryingStructMarshall(ptr ptr ptr)
@ stdcall NdrConformantVaryingStructMemorySize(ptr ptr)
@ stdcall NdrConformantVaryingStructUnmarshall(ptr ptr ptr long)
@ stdcall NdrContextHandleInitialize(ptr ptr)
@ stdcall NdrContextHandleSize(ptr ptr ptr)
@ stdcall NdrConvert2(ptr ptr long)
@ stdcall NdrConvert(ptr ptr)
@ stdcall NdrCorrelationFree(ptr)
@ stdcall NdrCorrelationInitialize(ptr ptr long long)
@ stdcall NdrCorrelationPass(ptr)
@ stdcall NdrCreateServerInterfaceFromStub() rpcrtbase.NdrCreateServerInterfaceFromStub
@ stdcall NdrDcomAsyncClientCall() rpcrtbase.NdrDcomAsyncClientCall
@ stdcall NdrDcomAsyncStubCall() rpcrtbase.NdrDcomAsyncStubCall
@ stdcall NdrDllCanUnloadNow(ptr)
@ stdcall NdrDllGetClassObject(ptr ptr ptr ptr ptr ptr)
@ stdcall NdrDllRegisterProxy(long ptr ptr)
@ stdcall NdrDllUnregisterProxy(long ptr ptr)
@ stdcall NdrEncapsulatedUnionBufferSize(ptr ptr ptr)
@ stdcall NdrEncapsulatedUnionFree(ptr ptr ptr)
@ stdcall NdrEncapsulatedUnionMarshall(ptr ptr ptr)
@ stdcall NdrEncapsulatedUnionMemorySize(ptr ptr)
@ stdcall NdrEncapsulatedUnionUnmarshall(ptr ptr ptr long)
@ stdcall NdrFixedArrayBufferSize(ptr ptr ptr)
@ stdcall NdrFixedArrayFree(ptr ptr ptr)
@ stdcall NdrFixedArrayMarshall(ptr ptr ptr)
@ stdcall NdrFixedArrayMemorySize(ptr ptr)
@ stdcall NdrFixedArrayUnmarshall(ptr ptr ptr long)
@ stdcall NdrFreeBuffer(ptr)
@ stdcall NdrFullPointerFree(ptr ptr)
@ stdcall NdrFullPointerInsertRefId(ptr long ptr)
@ stdcall NdrFullPointerQueryPointer(ptr ptr long ptr)
@ stdcall NdrFullPointerQueryRefId(ptr long long ptr)
@ stdcall NdrFullPointerXlatFree(ptr)
@ stdcall NdrFullPointerXlatInit(long long) 
@ stdcall NdrGetBuffer(ptr long ptr)
@ stdcall NdrGetDcomProtocolVersion() rpcrtbase.NdrGetDcomProtocolVersion
@ stdcall NdrGetSimpleTypeBufferAlignment() rpcrtbase.NdrGetSimpleTypeBufferAlignment 
@ stdcall NdrGetSimpleTypeBufferSize() rpcrtbase.NdrGetSimpleTypeBufferSize 
@ stdcall NdrGetSimpleTypeMemorySize() rpcrtbase.NdrGetSimpleTypeMemorySize 
@ stdcall NdrGetTypeFlags() rpcrtbase.NdrGetTypeFlags 
@ stdcall NdrGetUserMarshalInfo(ptr long ptr)
@ stdcall NdrInterfacePointerBufferSize(ptr ptr ptr)
@ stdcall NdrInterfacePointerFree(ptr ptr ptr)
@ stdcall NdrInterfacePointerMarshall(ptr ptr ptr)
@ stdcall NdrInterfacePointerMemorySize(ptr ptr)
@ stdcall NdrInterfacePointerUnmarshall(ptr ptr ptr long)
@ stdcall NdrMapCommAndFaultStatus(ptr ptr ptr long)
@ varargs NdrMesProcEncodeDecode(ptr ptr ptr)
@ stdcall NdrMesProcEncodeDecode2() rpcrtbase.NdrMesProcEncodeDecode2
@ stdcall -arch=x86_64 NdrMesProcEncodeDecode3() rpcrtbase.NdrMesProcEncodeDecode3
@ stdcall NdrMesSimpleTypeAlignSize() rpcrtbase.NdrMesSimpleTypeAlignSize
@ stdcall -arch=x86_64 NdrMesSimpleTypeAlignSizeAll() rpcrtbase.NdrMesSimpleTypeAlignSizeAll
@ stdcall NdrMesSimpleTypeDecode() rpcrtbase.NdrMesSimpleTypeDecode
@ stdcall -arch=x86_64 NdrMesSimpleTypeDecodeAll() rpcrtbase.NdrMesSimpleTypeDecodeAll
@ stdcall NdrMesSimpleTypeEncode() rpcrtbase.NdrMesSimpleTypeEncode
@ stdcall -arch=x86_64 NdrMesSimpleTypeEncodeAll() rpcrtbase.NdrMesSimpleTypeEncodeAll
@ stdcall NdrMesTypeAlignSize2() rpcrtbase.NdrMesTypeAlignSize2
@ stdcall -arch=x86_64 NdrMesTypeAlignSize3() rpcrtbase.NdrMesTypeAlignSize3
@ stdcall NdrMesTypeAlignSize() rpcrtbase.NdrMesTypeAlignSize
@ stdcall NdrMesTypeDecode2(ptr ptr ptr ptr ptr)
@ stdcall -arch=x86_64 NdrMesTypeDecode3(ptr ptr ptr ptr ptr) rpcrtbase.NdrMesTypeDecode3
@ stdcall NdrMesTypeDecode() rpcrtbase.NdrMesTypeDecode
@ stdcall NdrMesTypeEncode2(ptr ptr ptr ptr ptr)
@ stdcall -arch=x86_64 NdrMesTypeEncode3(ptr ptr ptr ptr ptr) rpcrtbase.NdrMesTypeEncode3
@ stdcall NdrMesTypeEncode() rpcrtbase.NdrMesTypeEncode
@ stdcall NdrMesTypeFree2(ptr ptr ptr ptr ptr)
@ stdcall -arch=x86_64 NdrMesTypeFree3(ptr ptr ptr ptr ptr) rpcrtbase.NdrMesTypeFree3
@ stdcall NdrNonConformantStringBufferSize(ptr ptr ptr)
@ stdcall NdrNonConformantStringMarshall(ptr ptr ptr)
@ stdcall NdrNonConformantStringMemorySize(ptr ptr)
@ stdcall NdrNonConformantStringUnmarshall(ptr ptr ptr long)
@ stdcall NdrNonEncapsulatedUnionBufferSize(ptr ptr ptr)
@ stdcall NdrNonEncapsulatedUnionFree(ptr ptr ptr)
@ stdcall NdrNonEncapsulatedUnionMarshall(ptr ptr ptr)
@ stdcall NdrNonEncapsulatedUnionMemorySize(ptr ptr)
@ stdcall NdrNonEncapsulatedUnionUnmarshall(ptr ptr ptr long)
@ stdcall NdrNsGetBuffer() rpcrtbase.NdrNsGetBuffer
@ stdcall NdrNsSendReceive() rpcrtbase.NdrNsSendReceive
@ stdcall NdrOleAllocate(long)
@ stdcall NdrOleFree(ptr)
@ stdcall NdrOutInit() rpcrtbase.NdrOutInit
@ stdcall NdrPartialIgnoreClientBufferSize() rpcrtbase.NdrPartialIgnoreClientBufferSize
@ stdcall NdrPartialIgnoreClientMarshall() rpcrtbase.NdrPartialIgnoreClientMarshall 
@ stdcall NdrPartialIgnoreServerInitialize() rpcrtbase.NdrPartialIgnoreServerInitialize 
@ stdcall NdrPartialIgnoreServerUnmarshall() rpcrtbase.NdrPartialIgnoreServerUnmarshall 
@ stdcall NdrPointerBufferSize(ptr ptr ptr)
@ stdcall NdrPointerFree(ptr ptr ptr)
@ stdcall NdrPointerMarshall(ptr ptr ptr)
@ stdcall NdrPointerMemorySize(ptr ptr)
@ stdcall NdrPointerUnmarshall(ptr ptr ptr long)
@ stdcall NdrProxyErrorHandler(long)
@ stdcall NdrProxyFreeBuffer(ptr ptr)
@ stdcall NdrProxyGetBuffer(ptr ptr)
@ stdcall NdrProxyInitialize(ptr ptr ptr ptr long)
@ stdcall NdrProxySendReceive(ptr ptr)
@ stdcall NdrRangeUnmarshall(ptr ptr ptr long)
@ stdcall NdrRpcSmClientAllocate() rpcrtbase.NdrRpcSmClientAllocate
@ stdcall NdrRpcSmClientFree() rpcrtbase.NdrRpcSmClientFree
@ stdcall NdrRpcSmSetClientToOsf(ptr)
@ stdcall NdrRpcSsDefaultAllocate() rpcrtbase.NdrRpcSsDefaultAllocate
@ stdcall NdrRpcSsDefaultFree() rpcrtbase.NdrRpcSsDefaultFree
@ stdcall NdrRpcSsDisableAllocate() rpcrtbase.NdrRpcSsDisableAllocate
@ stdcall NdrRpcSsEnableAllocate() rpcrtbase.NdrRpcSsEnableAllocate
@ stdcall NdrSendReceive(ptr ptr)
@ stdcall NdrServerCall2(ptr)
@ stdcall -arch=x86_64 NdrServerCallAll() rpcrtbase.NdrServerCallAll
@ stdcall -arch=x86_64 NdrServerCallNdr64() rpcrtbase.NdrServerCallNdr64
@ stdcall -arch=i386 NdrServerCall(ptr)
@ stdcall NdrServerContextMarshall(ptr ptr long)
@ stdcall NdrServerContextNewMarshall(ptr ptr ptr ptr) 
@ stdcall NdrServerContextNewUnmarshall(ptr ptr) 
@ stdcall NdrServerContextUnmarshall(ptr)
@ stdcall NdrServerInitialize() rpcrtbase.NdrServerInitialize
@ stdcall NdrServerInitializeMarshall() rpcrtbase.NdrServerInitializeMarshall
@ stdcall NdrServerInitializeNew(ptr ptr ptr)
@ stdcall NdrServerInitializePartial() rpcrtbase.NdrServerInitializePartial 
@ stdcall NdrServerInitializeUnmarshall() rpcrtbase.NdrServerInitializeUnmarshall
@ stdcall NdrServerMarshall() rpcrtbase.NdrServerMarshall
@ stdcall NdrServerUnmarshall() rpcrtbase.NdrServerUnmarshall
@ stdcall NdrSimpleStructBufferSize(ptr ptr ptr)
@ stdcall NdrSimpleStructFree(ptr ptr ptr)
@ stdcall NdrSimpleStructMarshall(ptr ptr ptr)
@ stdcall NdrSimpleStructMemorySize(ptr ptr)
@ stdcall NdrSimpleStructUnmarshall(ptr ptr ptr long)
@ stdcall NdrSimpleTypeMarshall(ptr ptr long)
@ stdcall NdrSimpleTypeUnmarshall(ptr ptr long)
@ stdcall NdrStubCall2(ptr ptr ptr ptr)
@ stdcall -arch=x86_64 NdrStubCall3(ptr ptr ptr ptr)
@ stdcall -arch=i386 NdrStubCall(ptr ptr ptr ptr)
@ stdcall NdrStubForwardingFunction(ptr ptr ptr ptr)
@ stdcall NdrStubGetBuffer(ptr ptr ptr)
@ stdcall NdrStubInitialize(ptr ptr ptr ptr)
@ stdcall NdrStubInitializeMarshall() rpcrtbase.NdrStubInitializeMarshall
@ stdcall NdrTypeFlags() rpcrtbase.NdrTypeFlags 
@ stdcall NdrTypeFree() rpcrtbase.NdrTypeFree 
@ stdcall NdrTypeMarshall() rpcrtbase.NdrTypeMarshall 
@ stdcall NdrTypeSize() rpcrtbase.NdrTypeSize 
@ stdcall NdrTypeUnmarshall() rpcrtbase.NdrTypeUnmarshall 
@ stdcall NdrUnmarshallBasetypeInline() rpcrtbase.NdrUnmarshallBasetypeInline 
@ stdcall NdrUserMarshalBufferSize(ptr ptr ptr)
@ stdcall NdrUserMarshalFree(ptr ptr ptr)
@ stdcall NdrUserMarshalMarshall(ptr ptr ptr)
@ stdcall NdrUserMarshalMemorySize(ptr ptr)
@ stdcall NdrUserMarshalSimpleTypeConvert() rpcrtbase.NdrUserMarshalSimpleTypeConvert
@ stdcall NdrUserMarshalUnmarshall(ptr ptr ptr long)
@ stdcall NdrVaryingArrayBufferSize(ptr ptr ptr)
@ stdcall NdrVaryingArrayFree(ptr ptr ptr)
@ stdcall NdrVaryingArrayMarshall(ptr ptr ptr)
@ stdcall NdrVaryingArrayMemorySize(ptr ptr)
@ stdcall NdrVaryingArrayUnmarshall(ptr ptr ptr long)
@ stdcall NdrXmitOrRepAsBufferSize(ptr ptr ptr)
@ stdcall NdrXmitOrRepAsFree(ptr ptr ptr)
@ stdcall NdrXmitOrRepAsMarshall(ptr ptr ptr)
@ stdcall NdrXmitOrRepAsMemorySize(ptr ptr)
@ stdcall NdrXmitOrRepAsUnmarshall(ptr ptr ptr long)
@ stdcall NdrpCreateProxy() rpcrtbase.NdrpCreateProxy 
@ stdcall NdrpCreateStub() rpcrtbase.NdrpCreateStub 
@ stdcall NdrpGetProcFormatString() rpcrtbase.NdrpGetProcFormatString 
@ stdcall NdrpGetTypeFormatString() rpcrtbase.NdrpGetTypeFormatString 
@ stdcall NdrpGetTypeGenCookie() rpcrtbase.NdrpGetTypeGenCookie 
@ stdcall NdrpMemoryIncrement() rpcrtbase.NdrpMemoryIncrement 
@ stdcall NdrpReleaseTypeFormatString() rpcrtbase.NdrpReleaseTypeFormatString 
@ stdcall NdrpReleaseTypeGenCookie () rpcrtbase.NdrpReleaseTypeGenCookie
@ stdcall NdrpSetRpcSsDefaults() rpcrtbase.NdrpSetRpcSsDefaults
@ stdcall NdrpVarVtOfTypeDesc() rpcrtbase.NdrpVarVtOfTypeDesc 
@ stdcall RpcAbortAsyncCall(ptr long) RpcAsyncAbortCall
@ stdcall RpcAsyncAbortCall(ptr long)
@ stdcall RpcAsyncCancelCall(ptr long)
@ stdcall RpcAsyncCompleteCall(ptr ptr)
@ stdcall RpcAsyncGetCallStatus(ptr)
@ stdcall RpcAsyncInitializeHandle(ptr long)
@ stdcall RpcAsyncRegisterInfo() rpcrtbase.RpcAsyncRegisterInfo
@ stdcall RpcBindingCopy(ptr ptr)
@ stdcall RpcBindingFree(ptr)
@ stdcall RpcBindingFromStringBindingA(str  ptr)
@ stdcall RpcBindingFromStringBindingW(wstr ptr)
@ stdcall RpcBindingInqAuthClientA(ptr ptr ptr ptr ptr ptr)
@ stdcall RpcBindingInqAuthClientExA(ptr ptr ptr ptr ptr ptr long)
@ stdcall RpcBindingInqAuthClientExW(ptr ptr ptr ptr ptr ptr long)
@ stdcall RpcBindingInqAuthClientW(ptr ptr ptr ptr ptr ptr)
@ stdcall RpcBindingInqAuthInfoA(ptr ptr ptr ptr ptr ptr)
@ stdcall RpcBindingInqAuthInfoExA(ptr ptr ptr ptr ptr ptr long ptr)
@ stdcall RpcBindingInqAuthInfoExW(ptr ptr ptr ptr ptr ptr long ptr)
@ stdcall RpcBindingInqAuthInfoW(ptr ptr ptr ptr ptr ptr)
@ stdcall RpcBindingInqObject(ptr ptr)
@ stdcall RpcBindingInqOption() rpcrtbase.RpcBindingInqOption
@ stdcall RpcBindingReset(ptr)
@ stdcall RpcBindingServerFromClient() rpcrtbase.RpcBindingServerFromClient
@ stdcall RpcBindingSetAuthInfoA(ptr str long long ptr long)
@ stdcall RpcBindingSetAuthInfoExA(ptr str long long ptr long ptr)
@ stdcall RpcBindingSetAuthInfoExW(ptr wstr long long ptr long ptr)
@ stdcall RpcBindingSetAuthInfoW(ptr wstr long long ptr long)
@ stdcall RpcBindingSetObject(ptr ptr)
@ stdcall RpcBindingSetOption(ptr long long)
@ stdcall RpcBindingToStringBindingA(ptr ptr)
@ stdcall RpcBindingToStringBindingW(ptr ptr)
@ stdcall RpcBindingVectorFree(ptr)
@ stdcall RpcCancelAsyncCall(ptr long) RpcAsyncCancelCall
@ stdcall RpcCancelThread(ptr)
@ stdcall RpcCancelThreadEx(ptr long)
@ stdcall RpcCertGeneratePrincipalNameA() rpcrtbase.RpcCertGeneratePrincipalNameA
@ stdcall RpcCertGeneratePrincipalNameW() rpcrtbase.RpcCertGeneratePrincipalNameW
@ stdcall RpcCompleteAsyncCall(ptr ptr) RpcAsyncCompleteCall
@ stdcall RpcEpRegisterA(ptr ptr ptr str)
@ stdcall RpcEpRegisterNoReplaceA(ptr ptr ptr str)
@ stdcall RpcEpRegisterNoReplaceW(ptr ptr ptr wstr)
@ stdcall RpcEpRegisterW(ptr ptr ptr wstr)
@ stdcall RpcEpResolveBinding(ptr ptr)
@ stdcall RpcEpUnregister(ptr ptr ptr)
@ stdcall RpcErrorAddRecord() rpcrtbase.RpcErrorAddRecord 
@ stdcall RpcErrorClearInformation() rpcrtbase.RpcErrorClearInformation 
@ stdcall RpcErrorEndEnumeration(ptr)
@ stdcall RpcErrorGetNextRecord(ptr long ptr)
@ stdcall RpcErrorGetNumberOfRecords() rpcrtbase.RpcErrorGetNumberOfRecords
@ stdcall RpcErrorLoadErrorInfo(ptr long ptr)
@ stdcall RpcErrorResetEnumeration () rpcrtbase.RpcErrorResetEnumeration
@ stdcall RpcErrorSaveErrorInfo(ptr ptr ptr)
@ stdcall RpcErrorStartEnumeration(ptr)
@ stdcall RpcFreeAuthorizationContext() rpcrtbase.RpcFreeAuthorizationContext 
@ stdcall RpcGetAsyncCallStatus(ptr) RpcAsyncGetCallStatus
@ stdcall RpcGetAuthorizationContextForClient() rpcrtbase.RpcGetAuthorizationContextForClient
@ stdcall RpcIfIdVectorFree() rpcrtbase.RpcIfIdVectorFree
@ stdcall RpcIfInqId() rpcrtbase.RpcIfInqId
@ stdcall RpcImpersonateClient(ptr)
@ stdcall RpcInitializeAsyncHandle(ptr long) RpcAsyncInitializeHandle
@ stdcall RpcMgmtEnableIdleCleanup()
@ stdcall RpcMgmtEpEltInqBegin(ptr long ptr long ptr ptr)
@ stdcall RpcMgmtEpEltInqDone() rpcrtbase.RpcMgmtEpEltInqDone
@ stdcall RpcMgmtEpEltInqNextA() rpcrtbase.RpcMgmtEpEltInqNextA
@ stdcall RpcMgmtEpEltInqNextW() rpcrtbase.RpcMgmtEpEltInqNextW
@ stdcall RpcMgmtEpUnregister() rpcrtbase.RpcMgmtEpUnregister
@ stdcall RpcMgmtInqComTimeout() rpcrtbase.RpcMgmtInqComTimeout
@ stdcall RpcMgmtInqDefaultProtectLevel() rpcrtbase.RpcMgmtInqDefaultProtectLevel
@ stdcall RpcMgmtInqIfIds(ptr ptr)
@ stdcall RpcMgmtInqServerPrincNameA() rpcrtbase.RpcMgmtInqServerPrincNameA
@ stdcall RpcMgmtInqServerPrincNameW() rpcrtbase.RpcMgmtInqServerPrincNameW
@ stdcall RpcMgmtInqStats(ptr ptr)
@ stdcall RpcMgmtIsServerListening(ptr)
@ stdcall RpcMgmtSetAuthorizationFn(ptr)
@ stdcall RpcMgmtSetCancelTimeout(long)
@ stdcall RpcMgmtSetComTimeout(ptr long)
@ stdcall RpcMgmtSetServerStackSize(long)
@ stdcall RpcMgmtStatsVectorFree(ptr)
@ stdcall RpcMgmtStopServerListening(ptr)
@ stdcall RpcMgmtWaitServerListen()
@ stdcall RpcNetworkInqProtseqsA(ptr)
@ stdcall RpcNetworkInqProtseqsW(ptr)
@ stdcall RpcNetworkIsProtseqValidA(ptr)
@ stdcall RpcNetworkIsProtseqValidW(ptr)
@ stdcall RpcNsBindingInqEntryNameA() rpcrtbase.RpcNsBindingInqEntryNameA
@ stdcall RpcNsBindingInqEntryNameW() rpcrtbase.RpcNsBindingInqEntryNameW
@ stdcall RpcObjectInqType() rpcrtbase.RpcObjectInqType
@ stdcall RpcObjectSetInqFn() rpcrtbase.RpcObjectSetInqFn
@ stdcall RpcObjectSetType(ptr ptr)
@ stdcall RpcProtseqVectorFreeA(ptr)
@ stdcall RpcProtseqVectorFreeW(ptr)
@ stdcall RpcRaiseException(long)
@ stdcall RpcRegisterAsyncInfo() rpcrtbase.RpcRegisterAsyncInfo
@ stdcall RpcRevertToSelf()
@ stdcall RpcRevertToSelfEx(ptr)
@ stdcall RpcServerInqBindings(ptr)
@ stdcall RpcServerInqCallAttributesA() rpcrtbase.RpcServerInqCallAttributesA 
@ stdcall RpcServerInqCallAttributesW() rpcrtbase.RpcServerInqCallAttributesW 
@ stdcall RpcServerInqDefaultPrincNameA(long ptr)
@ stdcall RpcServerInqDefaultPrincNameW(long ptr)
@ stdcall RpcServerInqIf() rpcrtbase.RpcServerInqIf
@ stdcall RpcServerListen(long long long)
@ stdcall RpcServerRegisterAuthInfoA(str  long ptr ptr)
@ stdcall RpcServerRegisterAuthInfoW(wstr long ptr ptr)
@ stdcall RpcServerRegisterIf2(ptr ptr ptr long long long ptr)
@ stdcall RpcServerRegisterIf(ptr ptr ptr)
@ stdcall RpcServerRegisterIfEx(ptr ptr ptr long long ptr)
@ stdcall RpcServerTestCancel() rpcrtbase.RpcServerTestCancel
@ stdcall RpcServerUnregisterIf(ptr ptr long)
@ stdcall RpcServerUnregisterIfEx(ptr ptr long)
@ stdcall RpcServerUseAllProtseqs() rpcrtbase.RpcServerUseAllProtseqs
@ stdcall RpcServerUseAllProtseqsEx() rpcrtbase.RpcServerUseAllProtseqsEx
@ stdcall RpcServerUseAllProtseqsIf() rpcrtbase.RpcServerUseAllProtseqsIf
@ stdcall RpcServerUseAllProtseqsIfEx() rpcrtbase.RpcServerUseAllProtseqsIfEx
@ stdcall RpcServerUseProtseqA(str long ptr)
@ stdcall RpcServerUseProtseqEpA(str  long str  ptr)
@ stdcall RpcServerUseProtseqEpExA(str  long str  ptr ptr)
@ stdcall RpcServerUseProtseqEpExW(wstr long wstr ptr ptr)
@ stdcall RpcServerUseProtseqEpW(wstr long wstr ptr) 
@ stdcall RpcServerUseProtseqExA() rpcrtbase.RpcServerUseProtseqExA
@ stdcall RpcServerUseProtseqExW() rpcrtbase.RpcServerUseProtseqExW
@ stdcall RpcServerUseProtseqIfA() rpcrtbase.RpcServerUseProtseqIfA
@ stdcall RpcServerUseProtseqIfExA() rpcrtbase.RpcServerUseProtseqIfExA
@ stdcall RpcServerUseProtseqIfExW() rpcrtbase.RpcServerUseProtseqIfExW
@ stdcall RpcServerUseProtseqIfW() rpcrtbase.RpcServerUseProtseqIfW
@ stdcall RpcServerUseProtseqW(wstr long ptr)
@ stdcall RpcServerYield() rpcrtbase.RpcServerYield
@ stdcall RpcSmAllocate() rpcrtbase.RpcSmAllocate
@ stdcall RpcSmClientFree() rpcrtbase.RpcSmClientFree
@ stdcall RpcSmDestroyClientContext(ptr)
@ stdcall RpcSmDisableAllocate() rpcrtbase.RpcSmDisableAllocate
@ stdcall RpcSmEnableAllocate() rpcrtbase.RpcSmEnableAllocate
@ stdcall RpcSmFree() rpcrtbase.RpcSmFree
@ stdcall RpcSmGetThreadHandle() rpcrtbase.RpcSmGetThreadHandle
@ stdcall RpcSmSetClientAllocFree() rpcrtbase.RpcSmSetClientAllocFree
@ stdcall RpcSmSetThreadHandle() rpcrtbase.RpcSmSetThreadHandle
@ stdcall RpcSmSwapClientAllocFree() rpcrtbase.RpcSmSwapClientAllocFree
@ stdcall RpcSsAllocate() rpcrtbase.RpcSsAllocate
@ stdcall RpcSsContextLockExclusive() rpcrtbase.RpcSsContextLockExclusive 
@ stdcall RpcSsContextLockShared() rpcrtbase.RpcSsContextLockShared 
@ stdcall RpcSsDestroyClientContext(ptr)
@ stdcall RpcSsDisableAllocate() rpcrtbase.RpcSsDisableAllocate
@ stdcall RpcSsDontSerializeContext() rpcrtbase.RpcSsDontSerializeContext
@ stdcall RpcSsEnableAllocate() rpcrtbase.RpcSsEnableAllocate
@ stdcall RpcSsFree() rpcrtbase.RpcSsFree
@ stdcall RpcSsGetContextBinding() rpcrtbase.RpcSsGetContextBinding
@ stdcall RpcSsGetThreadHandle() rpcrtbase.RpcSsGetThreadHandle
@ stdcall RpcSsSetClientAllocFree() rpcrtbase.RpcSsSetClientAllocFree
@ stdcall RpcSsSetThreadHandle() rpcrtbase.RpcSsSetThreadHandle
@ stdcall RpcSsSwapClientAllocFree() rpcrtbase.RpcSsSwapClientAllocFree
@ stdcall RpcStringBindingComposeA(str  str  str  str  str  ptr)
@ stdcall RpcStringBindingComposeW(wstr wstr wstr wstr wstr ptr)
@ stdcall RpcStringBindingParseA(str  ptr ptr ptr ptr ptr)
@ stdcall RpcStringBindingParseW(wstr ptr ptr ptr ptr ptr)
@ stdcall RpcStringFreeA(ptr)
@ stdcall RpcStringFreeW(ptr)
@ stdcall RpcTestCancel() rpcrtbase.RpcTestCancel
@ stdcall RpcUserFree() rpcrtbase.RpcUserFree 
@ stdcall SimpleTypeAlignment() rpcrtbase.SimpleTypeAlignment 
@ stdcall SimpleTypeBufferSize() rpcrtbase.SimpleTypeBufferSize 
@ stdcall SimpleTypeMemorySize() rpcrtbase.SimpleTypeMemorySize 
@ stdcall TowerConstruct(ptr ptr ptr ptr ptr ptr)
@ stdcall TowerExplode(ptr ptr ptr ptr ptr ptr)
@ stdcall UuidCompare(ptr ptr ptr)
@ stdcall UuidCreate(ptr)
@ stdcall UuidCreateNil(ptr)
@ stdcall UuidCreateSequential(ptr) 
@ stdcall UuidEqual(ptr ptr ptr)
@ stdcall UuidFromStringA(str ptr)
@ stdcall UuidFromStringW(wstr ptr)
@ stdcall UuidHash(ptr ptr)
@ stdcall UuidIsNil(ptr ptr)
@ stdcall UuidToStringA(ptr ptr)
@ stdcall UuidToStringW(ptr ptr)
@ stdcall char_array_from_ndr() rpcrtbase.char_array_from_ndr
@ stdcall char_from_ndr() rpcrtbase.char_from_ndr
@ stdcall CheckVerificationTrailer() rpcrtbase.CheckVerificationTrailer
@ stdcall data_from_ndr() rpcrtbase.data_from_ndr
@ stdcall data_into_ndr() rpcrtbase.data_into_ndr
@ stdcall data_size_ndr() rpcrtbase.data_size_ndr
@ stdcall double_array_from_ndr() rpcrtbase.double_array_from_ndr
@ stdcall double_from_ndr() rpcrtbase.double_from_ndr
@ stdcall enum_from_ndr() rpcrtbase.enum_from_ndr
@ stdcall float_array_from_ndr() rpcrtbase.float_array_from_ndr
@ stdcall float_from_ndr() rpcrtbase.float_from_ndr
@ stdcall long_array_from_ndr() rpcrtbase.long_array_from_ndr
@ stdcall long_from_ndr() rpcrtbase.long_from_ndr
@ stdcall long_from_ndr_temp() rpcrtbase.long_from_ndr_temp
@ stdcall pfnFreeRoutines() rpcrtbase.pfnFreeRoutines 
@ stdcall pfnMarshallRoutines() rpcrtbase.pfnMarshallRoutines 
@ stdcall pfnSizeRoutines() rpcrtbase.pfnSizeRoutines 
@ stdcall pfnUnmarshallRoutines() rpcrtbase.pfnUnmarshallRoutines 
@ stdcall short_array_from_ndr() rpcrtbase.short_array_from_ndr
@ stdcall short_from_ndr() rpcrtbase.short_from_ndr
@ stdcall short_from_ndr_temp() rpcrtbase.short_from_ndr_temp
@ stdcall tree_into_ndr() rpcrtbase.tree_into_ndr
@ stdcall tree_peek_ndr() rpcrtbase.tree_peek_ndr
@ stdcall tree_size_ndr() rpcrtbase.tree_size_ndr

#From Longhorn/Vista
@ stdcall DllRegisterServer()
@ stdcall I_RpcBindingCreateNP(wstr wstr wstr ptr)
@ stub I_RpcCompleteAndFree
@ stdcall I_RpcGetPortAllocationData(ptr)
@ stdcall I_RpcInitHttpImports(ptr)
@ stdcall I_RpcServerStartService(wstr wstr ptr)
@ stdcall I_RpcVerifierCorruptionExpected()
@ stdcall NdrGetBaseInterfaceFromStub(ptr ptr ptr)
@ stdcall RpcBindingBind(ptr ptr ptr)
@ stdcall RpcBindingCreateA(ptr ptr ptr ptr)
@ stdcall RpcBindingCreateW(ptr ptr ptr ptr)
@ stdcall RpcBindingUnbind(ptr)
@ stdcall RpcExceptionFilter(long)
@ stdcall RpcServerCompleteSecurityCallback(ptr long)
@ stdcall RpcServerInqBindingHandle(ptr)
@ stdcall RpcServerSubscribeForNotification(ptr long long ptr)
@ stdcall RpcServerUnsubscribeForNotification(ptr long ptr)
@ stdcall I_RpcBindingInqSecurityContextKeyInfo(ptr ptr)
@ stub I_RpcFwThisIsTheManager
@ stdcall I_RpcServerInqRemoteConnAddress(ptr ptr ptr ptr)
@ stub RpcCertMatchPrincipalName	