@ stdcall CLSIDFromProgID(wstr ptr) olebase.CLSIDFromProgID
@ stdcall CLSIDFromString(wstr ptr) olebase.CLSIDFromString
@ stdcall CoAddRefServerProcess() olebase.CoAddRefServerProcess
@ stub CoAllowUnmarshalerCLSID ;(long) ole.CoAllowUnmarshalerCLSID
@ stdcall CoCancelCall(long long)
@ stdcall CoCopyProxy(ptr ptr) olebase.CoCopyProxy
@ stdcall CoCreateFreeThreadedMarshaler(ptr ptr) olebase.CoCreateFreeThreadedMarshaler
@ stdcall CoCreateGuid(ptr) olebase.CoCreateGuid
@ stdcall CoCreateInstance(ptr ptr long ptr ptr) olebase.CoCreateInstance
@ stdcall CoCreateInstanceEx(ptr ptr long ptr long ptr) olebase.CoCreateInstanceEx
@ stub CoCreateInstanceFromApp
@ stub CoDecodeProxy
@ stub CoDecrementMTAUsage
@ stub CoDisableCallCancellation
@ stub CoDisconnectContext
@ stdcall CoDisconnectObject(ptr long) olebase.CoDisconnectObject
@ stub CoEnableCallCancellation
@ stdcall CoFreeUnusedLibraries() olebase.CoFreeUnusedLibraries
@ stdcall CoFreeUnusedLibrariesEx(long long) olebase.CoFreeUnusedLibrariesEx
@ stub CoGetApartmentType
@ stdcall CoGetCallContext(ptr ptr) olebase.CoGetCallContext
@ stdcall CoGetCallerTID(ptr) olebase.CoGetCallerTID
@ stub CoGetCancelObject
@ stdcall CoGetClassObject(ptr long ptr ptr ptr) olebase.CoGetClassObject
@ stdcall CoGetContextToken(ptr) olebase.CoGetContextToken
@ stdcall CoGetCurrentLogicalThreadId(ptr) olebase.CoGetCurrentLogicalThreadId
@ stdcall CoGetCurrentProcess() olebase.CoGetCurrentProcess
@ stdcall CoGetDefaultContext(long ptr ptr) olebase.CoGetDefaultContext
@ stdcall CoGetInterfaceAndReleaseStream(ptr ptr ptr) olebase.CoGetInterfaceAndReleaseStream
@ stdcall CoGetMalloc(long ptr) olebase.CoGetMalloc
@ stdcall CoGetMarshalSizeMax(ptr ptr ptr long ptr long) olebase.CoGetMarshalSizeMax
@ stdcall CoGetObjectContext(ptr ptr) olebase.CoGetObjectContext
@ stdcall CoGetPSClsid(ptr ptr) olebase.CoGetPSClsid
@ stdcall CoGetStandardMarshal(ptr ptr long ptr long ptr) olebase.CoGetStandardMarshal
@ stub CoGetStdMarshalEx
@ stdcall CoGetTreatAsClass(ptr ptr) olebase.CoGetTreatAsClass
@ stdcall CoImpersonateClient() olebase.CoImpersonateClient
@ stub CoIncrementMTAUsage
@ stdcall CoInitializeEx(ptr long) olebase.CoInitializeEx
@ stdcall CoInitializeSecurity(ptr long ptr ptr long long ptr long ptr) olebase.CoInitializeSecurity
@ stdcall CoInvalidateRemoteMachineBindings(str) olebase.CoInvalidateRemoteMachineBindings
@ stdcall CoIsHandlerConnected(ptr) olebase.CoIsHandlerConnected
@ stdcall CoLockObjectExternal(ptr long long) olebase.CoLockObjectExternal
@ stdcall CoMarshalHresult(ptr long) olebase.CoMarshalHresult
@ stdcall CoMarshalInterThreadInterfaceInStream(ptr ptr ptr) olebase.CoMarshalInterThreadInterfaceInStream
@ stdcall CoMarshalInterface(ptr ptr ptr long ptr long) olebase.CoMarshalInterface
@ stdcall CoQueryAuthenticationServices(ptr ptr) ole.CoQueryAuthenticationServices
@ stdcall CoQueryClientBlanket(ptr ptr ptr ptr ptr ptr ptr) olebase.CoQueryClientBlanket
@ stdcall CoQueryProxyBlanket(ptr ptr ptr ptr ptr ptr ptr ptr) olebase.CoQueryProxyBlanket
@ stdcall CoRegisterClassObject(ptr ptr long long ptr) olebase.CoRegisterClassObject
@ stdcall CoRegisterPSClsid(ptr ptr) olebase.CoRegisterPSClsid
@ stdcall CoRegisterSurrogate(ptr) olebase.CoRegisterSurrogate
@ stdcall CoReleaseMarshalData(ptr) olebase.CoReleaseMarshalData
@ stdcall CoReleaseServerProcess() olebase.CoReleaseServerProcess
@ stdcall CoResumeClassObjects() olebase.CoResumeClassObjects
@ stdcall CoRevertToSelf() olebase.CoRevertToSelf
@ stdcall CoRevokeClassObject(long) olebase.CoRevokeClassObject
@ stdcall CoSetCancelObject(ptr) olebase.CoSetCancelObject
@ stdcall CoSetProxyBlanket(ptr long long ptr long long ptr long) olebase.CoSetProxyBlanket
@ stdcall CoSuspendClassObjects() olebase.CoSuspendClassObjects
@ stdcall CoSwitchCallContext(ptr ptr) olebase.CoSwitchCallContext
@ stdcall CoTaskMemAlloc(long) olebase.CoTaskMemAlloc
@ stdcall CoTaskMemFree(ptr) olebase.CoTaskMemFree
@ stdcall CoTaskMemRealloc(ptr long) olebase.CoTaskMemRealloc
@ stdcall CoTestCancel() ole.CoTestCancel
@ stdcall CoUninitialize() olebase.CoUninitialize
@ stdcall CoUnmarshalHresult(ptr ptr) olebase.CoUnmarshalHresult
@ stdcall CoUnmarshalInterface(ptr ptr ptr) olebase.CoUnmarshalInterface
@ stdcall CoWaitForMultipleHandles(long long long ptr ptr) olebase.CoWaitForMultipleHandles
@ stub CoWaitForMultipleObjects ;(long long long ptr ptr) combase.CoWaitForMultipleObjects
@ stdcall CreateStreamOnHGlobal(ptr long ptr) olebase.CreateStreamOnHGlobal
@ stdcall FreePropVariantArray(long ptr) olebase.FreePropVariantArray
@ stdcall GetHGlobalFromStream(ptr ptr) olebase.GetHGlobalFromStream
@ stdcall IIDFromString(wstr ptr) olebase.IIDFromString
@ stdcall ProgIDFromCLSID(ptr ptr) olebase.ProgIDFromCLSID
@ stdcall PropVariantClear(ptr) olebase.PropVariantClear
@ stdcall PropVariantCopy(ptr ptr) olebase.PropVariantCopy
@ stdcall StringFromCLSID(ptr ptr) olebase.StringFromCLSID
@ stdcall StringFromGUID2(ptr ptr long) olebase.StringFromGUID2
@ stdcall StringFromIID(ptr ptr) olebase.StringFromIID
