@ stdcall PropVariantChangeType(ptr ptr long long) olebase.PropVariantChangeType
@ stdcall BindMoniker(ptr long ptr ptr)
@ stdcall CLIPFORMAT_UserFree(ptr ptr)
@ stdcall CLIPFORMAT_UserMarshal(ptr ptr ptr)
@ stdcall CLIPFORMAT_UserSize(ptr long ptr)
@ stdcall CLIPFORMAT_UserUnmarshal(ptr ptr ptr)
@ stdcall CLSIDFromOle1Class(wstr ptr ptr) olebase.CLSIDFromOle1Class
@ stdcall CLSIDFromProgID(wstr ptr)
@ stdcall CLSIDFromProgIDEx(wstr ptr)
@ stdcall CLSIDFromString(wstr ptr)
@ stdcall CoAddRefServerProcess()
@ stdcall CoAllowSetForegroundWindow(ptr ptr)
@ stdcall CoBuildVersion()
@ stdcall CoCancelCall(ptr long)
@ stdcall CoCopyProxy(ptr ptr)
@ stdcall CoCreateFreeThreadedMarshaler(ptr ptr)
@ stdcall CoCreateGuid(ptr)
@ stdcall CoCreateInstance(ptr ptr long ptr ptr)
@ stdcall CoCreateInstanceEx(ptr ptr long ptr long ptr)
@ stdcall CoCreateObjectInContext(ptr ptr ptr ptr) olebase.CoCreateObjectInContext
@ stdcall CoDeactivateObject(long long) olebase.CoDeactivateObject
@ stdcall CoDisableCallCancellation(ptr)
@ stdcall CoDisconnectObject(ptr long)
@ stdcall CoDosDateTimeToFileTime(long long ptr) kernel32.DosDateTimeToFileTime
@ stdcall CoEnableCallCancellation(ptr)
@ stdcall CoFileTimeNow(ptr)
@ stdcall CoFileTimeToDosDateTime(ptr ptr ptr) kernel32.FileTimeToDosDateTime
@ stdcall CoFreeAllLibraries()
@ stdcall CoFreeLibrary(long)
@ stdcall CoFreeUnusedLibraries()
@ stdcall CoFreeUnusedLibrariesEx(long long)
@ stdcall CoGetApartmentID(long ptr) olebase.CoGetApartmentID
@ stdcall CoGetCallContext(ptr ptr)
@ stdcall CoGetCallerTID(ptr)
@ stdcall CoGetCancelObject(long ptr ptr)
@ stdcall CoGetClassObject(ptr long ptr ptr ptr)
@ stdcall CoGetClassVersion(ptr ptr ptr) olebase.CoGetClassVersion
@ stdcall CoGetComCatalog(long ptr) olebase.CoGetComCatalog
@ stdcall CoGetContextToken(ptr)
@ stdcall CoGetCurrentLogicalThreadId(ptr)
@ stdcall CoGetCurrentProcess()
@ stdcall CoGetDefaultContext(long ptr ptr)
@ stdcall CoGetInstanceFromFile(ptr ptr ptr long long wstr long ptr)
@ stdcall CoGetInstanceFromIStorage(ptr ptr ptr long ptr long ptr)
@ stdcall CoGetInterceptor(long ptr long ptr)
@ stdcall CoGetInterceptorFromTypeInfo(long ptr ptr long ptr)
@ stdcall CoGetInterfaceAndReleaseStream(ptr ptr ptr)
@ stdcall CoGetMalloc(long ptr)
@ stdcall CoGetMarshalSizeMax(ptr ptr ptr long ptr long)
@ stdcall CoGetObject(wstr ptr ptr ptr)
@ stdcall CoGetObjectContext(ptr ptr)
@ stdcall CoGetPSClsid(ptr ptr)
@ stdcall CoGetProcessIdentifier(ptr) olebase.CoGetProcessIdentifier
@ stdcall CoGetStandardMarshal(ptr ptr long ptr long ptr)
@ stdcall CoGetState(ptr)
@ stdcall CoGetStdMarshalEx(ptr long ptr)
@ stdcall CoGetSystemSecurityPermissions(long ptr) olebase.CoGetSystemSecurityPermissions
@ stdcall CoGetTreatAsClass(ptr ptr)
@ stdcall CoImpersonateClient()
@ stdcall CoInitialize(ptr)
@ stdcall CoInitializeEx(ptr long)
@ stdcall CoInitializeSecurity(ptr long ptr ptr long long ptr long ptr)
@ stdcall CoInitializeWOW(long long)
@ stdcall CoInstall(ptr long ptr ptr wstr) olebase.CoInstall
@ stdcall CoInvalidateRemoteMachineBindings(str) olebase.CoInvalidateRemoteMachineBindings
@ stdcall CoIsHandlerConnected(ptr)
@ stdcall CoIsOle1Class (ptr)
@ stdcall CoLoadLibrary(wstr long)
@ stdcall CoLockObjectExternal(ptr long long)
@ stdcall CoMarshalHresult(ptr long)
@ stdcall CoMarshalInterThreadInterfaceInStream(ptr ptr ptr)
@ stdcall CoMarshalInterface(ptr ptr ptr long ptr long)
@ stdcall CoPopServiceDomain(ptr) olebase.CoPopServiceDomain
@ stdcall CoPushServiceDomain(ptr) olebase.CoPushServiceDomain
@ stdcall CoQueryAuthenticationServices(ptr ptr)
@ stdcall CoQueryClientBlanket(ptr ptr ptr ptr ptr ptr ptr)
@ stdcall CoQueryProxyBlanket(ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall CoQueryReleaseObject(ptr)
@ stdcall CoReactivateObject(long ptr) olebase.CoReactivateObject
@ stdcall CoRegisterChannelHook(ptr ptr)
@ stdcall CoRegisterClassObject(ptr ptr long long ptr)
@ stdcall CoRegisterInitializeSpy(ptr ptr)
@ stdcall CoRegisterMallocSpy (ptr)
@ stdcall CoRegisterMessageFilter(ptr ptr)
@ stdcall CoRegisterPSClsid(ptr ptr)
@ stdcall CoRegisterSurrogate(ptr)
@ stdcall CoRegisterSurrogateEx(ptr ptr)
@ stdcall CoReleaseMarshalData(ptr)
@ stdcall CoReleaseServerProcess()
@ stdcall CoResumeClassObjects()
@ stdcall CoRetireServer(ptr) olebase.CoRetireServer
@ stdcall CoRevertToSelf()
@ stdcall CoRevokeClassObject(long)
@ stdcall CoRevokeInitializeSpy(int64)
@ stdcall CoRevokeMallocSpy()
@ stdcall CoSetCancelObject(ptr) olebase.CoSetCancelObject
@ stdcall CoSetProxyBlanket(ptr long long ptr long long ptr long)
@ stdcall CoSetState(ptr)
@ stdcall CoSuspendClassObjects()
@ stdcall CoSwitchCallContext(ptr ptr)
@ stdcall CoTaskMemAlloc(long)
@ stdcall CoTaskMemFree(ptr)
@ stdcall CoTaskMemRealloc(ptr long)
@ stdcall CoTestCancel() olebase.CoTestCancel
@ stdcall CoTreatAsClass(ptr ptr)
@ stdcall CoUninitialize()
@ stdcall CoUnloadingWOW(long)
@ stdcall CoUnmarshalHresult(ptr ptr)
@ stdcall CoUnmarshalInterface(ptr ptr ptr)
@ stdcall CoWaitForMultipleHandles(long long long ptr ptr)
@ stdcall ComPs_NdrDllCanUnloadNow(ptr) olebase.ComPs_NdrDllCanUnloadNow
@ stdcall ComPs_NdrDllGetClassObject(long long ptr ptr ptr ptr) olebase.ComPs_NdrDllGetClassObject
@ stdcall ComPs_NdrDllRegisterProxy(ptr ptr ptr ptr ptr) olebase.ComPs_NdrDllRegisterProxy
@ stdcall ComPs_NdrDllUnregisterProxy(ptr ptr ptr ptr ptr) olebase.ComPs_NdrDllUnregisterProxy
@ stdcall CreateAntiMoniker(ptr)
@ stdcall CreateBindCtx(long ptr)
@ stdcall CreateClassMoniker(ptr ptr)
@ stdcall CreateDataAdviseHolder(ptr)
@ stdcall CreateDataCache(ptr ptr ptr ptr)
@ stdcall CreateErrorInfo(ptr)
@ stdcall CreateFileMoniker(wstr ptr)
@ stdcall CreateGenericComposite(ptr ptr ptr)
@ stdcall CreateILockBytesOnHGlobal(ptr long ptr)
@ stdcall CreateItemMoniker(wstr wstr ptr)
@ stdcall CreateObjrefMoniker(ptr ptr)
@ stdcall CreateOleAdviseHolder(ptr)
@ stdcall CreatePointerMoniker(ptr ptr)
@ stdcall CreateStdProgressIndicator(ptr str ptr ptr) olebase.CreateStdProgressIndicator
@ stdcall CreateStreamOnHGlobal(ptr long ptr)
@ stdcall DcomChannelSetHResult(ptr ptr ptr) olebase.DcomChannelSetHResult
@ stdcall DllDebugObjectRPCHook(long ptr)
@ stdcall DllGetClassObject(ptr ptr ptr)
@ stdcall DllGetClassObjectWOW(long ptr ptr)
@ stdcall DllRegisterServer()
@ stdcall DoDragDrop(ptr ptr long ptr)
@ stdcall EnableHookObject(long ptr)
@ stdcall FmtIdToPropStgName(ptr wstr)
@ stdcall FreePropVariantArray(long ptr)
@ stdcall GetClassFile(wstr ptr)
@ stdcall GetConvertStg(ptr)
@ stdcall GetDocumentBitStg(ptr)
@ stdcall GetErrorInfo(long ptr)
@ stdcall GetHGlobalFromILockBytes(ptr ptr)
@ stdcall GetHGlobalFromStream(ptr ptr)
@ stdcall GetHookInterface(ptr)
@ stdcall GetRunningObjectTable(long ptr)
@ stdcall HACCEL_UserFree(ptr ptr)
@ stdcall HACCEL_UserMarshal(ptr ptr ptr)
@ stdcall HACCEL_UserSize(ptr long ptr)
@ stdcall HACCEL_UserUnmarshal(ptr ptr ptr)
@ stdcall HBITMAP_UserFree(ptr ptr)
@ stdcall HBITMAP_UserMarshal(ptr ptr ptr)
@ stdcall HBITMAP_UserSize(ptr long ptr)
@ stdcall HBITMAP_UserUnmarshal(ptr ptr ptr)
@ stdcall HBRUSH_UserFree(ptr ptr)
@ stdcall HBRUSH_UserMarshal(ptr ptr ptr)
@ stdcall HBRUSH_UserSize(ptr ptr)
@ stdcall HBRUSH_UserUnmarshal(ptr ptr ptr)
@ stdcall HDC_UserFree(ptr ptr)
@ stdcall HDC_UserMarshal(ptr ptr ptr)
@ stdcall HDC_UserSize(ptr long ptr)
@ stdcall HDC_UserUnmarshal(ptr ptr ptr)
@ stdcall HENHMETAFILE_UserFree(ptr ptr)
@ stdcall HENHMETAFILE_UserMarshal(ptr ptr ptr)
@ stdcall HENHMETAFILE_UserSize(ptr long ptr)
@ stdcall HENHMETAFILE_UserUnmarshal(ptr ptr ptr)
@ stdcall HGLOBAL_UserFree(ptr ptr)
@ stdcall HGLOBAL_UserMarshal(ptr ptr ptr)
@ stdcall HGLOBAL_UserSize(ptr long ptr)
@ stdcall HGLOBAL_UserUnmarshal(ptr ptr ptr)
@ stdcall HICON_UserFree(ptr ptr)
@ stdcall HICON_UserMarshal(ptr ptr ptr)
@ stdcall HICON_UserSize(ptr long ptr)
@ stdcall HICON_UserUnmarshal(ptr ptr ptr)
@ stdcall HMENU_UserFree(ptr ptr)
@ stdcall HMENU_UserMarshal(ptr ptr ptr)
@ stdcall HMENU_UserSize(ptr long ptr)
@ stdcall HMENU_UserUnmarshal(ptr ptr ptr)
@ stdcall HMETAFILEPICT_UserFree(ptr ptr)
@ stdcall HMETAFILEPICT_UserMarshal(ptr ptr ptr)
@ stdcall HMETAFILEPICT_UserSize(ptr long ptr)
@ stdcall HMETAFILEPICT_UserUnmarshal(ptr ptr ptr)
@ stdcall HMETAFILE_UserFree(ptr ptr)
@ stdcall HMETAFILE_UserMarshal(ptr ptr ptr)
@ stdcall HMETAFILE_UserSize(ptr long ptr)
@ stdcall HMETAFILE_UserUnmarshal(ptr ptr ptr)
@ stdcall HPALETTE_UserFree(ptr ptr)
@ stdcall HPALETTE_UserMarshal(ptr ptr ptr)
@ stdcall HPALETTE_UserSize(ptr long ptr)
@ stdcall HPALETTE_UserUnmarshal(ptr ptr ptr)
@ stdcall HWND_UserFree(ptr ptr)
@ stdcall HWND_UserMarshal(ptr ptr ptr)
@ stdcall HWND_UserSize(ptr long ptr)
@ stdcall HWND_UserUnmarshal(ptr ptr ptr)
@ stdcall  HkOleRegisterObject(ptr ptr ptr long) olebase.HkOleRegisterObject
@ stdcall IIDFromString(wstr ptr)
@ stdcall IsAccelerator(long long ptr long)
@ stdcall IsEqualGUID(ptr ptr)
@ stdcall IsValidIid(ptr)
@ stdcall IsValidInterface(ptr)
@ stdcall IsValidPtrIn(ptr long)
@ stdcall IsValidPtrOut(ptr long)
@ stdcall MkParseDisplayName(ptr ptr ptr ptr)
@ stdcall MonikerCommonPrefixWith(ptr ptr ptr)
@ stdcall MonikerRelativePathTo(ptr ptr ptr long)
@ stdcall OleBuildVersion()
@ stdcall OleConvertIStorageToOLESTREAM(ptr ptr)
@ stdcall OleConvertIStorageToOLESTREAMEx(ptr ptr long long long ptr ptr)
@ stdcall OleConvertOLESTREAMToIStorage(ptr ptr ptr)
@ stdcall OleConvertOLESTREAMToIStorageEx(ptr ptr ptr ptr ptr ptr ptr)
@ stdcall OleCreate(ptr ptr long ptr ptr ptr ptr)
@ stdcall OleCreateDefaultHandler(ptr ptr ptr ptr)
@ stdcall OleCreateEmbeddingHelper(ptr ptr long ptr ptr ptr)
@ stdcall OleCreateEx(long long long long long ptr ptr ptr ptr ptr ptr ptr)
@ stdcall OleCreateFromData(ptr ptr long ptr ptr ptr ptr)
@ stdcall OleCreateFromDataEx(ptr ptr long long long ptr ptr ptr ptr ptr ptr ptr)
@ stdcall OleCreateFromFile(ptr ptr ptr long ptr ptr ptr ptr)
@ stdcall OleCreateFromFileEx(long str long long long long ptr ptr ptr ptr ptr ptr ptr)
@ stdcall OleCreateLink(ptr ptr long ptr ptr ptr ptr)
@ stdcall OleCreateLinkEx(ptr long long long long ptr ptr ptr ptr ptr ptr)
@ stdcall OleCreateLinkFromData(ptr ptr long ptr ptr ptr ptr)
@ stdcall OleCreateLinkFromDataEx(ptr long long long long ptr ptr ptr ptr ptr ptr ptr)
@ stdcall OleCreateLinkToFile(ptr ptr long ptr ptr ptr ptr)
@ stdcall OleCreateLinkToFileEx(str long long long long ptr ptr ptr ptr ptr ptr ptr)
@ stdcall OleCreateMenuDescriptor(long ptr)
@ stdcall OleCreateStaticFromData(ptr ptr long ptr ptr ptr ptr)
@ stdcall OleDestroyMenuDescriptor(long)
@ stdcall OleDoAutoConvert(ptr ptr)
@ stdcall OleDraw(ptr long long ptr)
@ stdcall OleDuplicateData(long long long)
@ stdcall OleFlushClipboard()
@ stdcall OleGetAutoConvert(ptr ptr)
@ stdcall OleGetClipboard(ptr)
@ stdcall OleGetIconOfClass(ptr ptr long)
@ stdcall OleGetIconOfFile(str long)
@ stdcall OleInitialize(ptr)
@ stdcall OleInitializeWOW(long long)
@ stdcall OleIsCurrentClipboard(ptr)
@ stdcall OleIsRunning(ptr)
@ stdcall OleLoad(ptr ptr ptr ptr)
@ stdcall OleLoadFromStream(ptr ptr ptr)
@ stdcall OleLockRunning(ptr long long)
@ stdcall OleMetafilePictFromIconAndLabel(long ptr ptr long)
@ stdcall OleNoteObjectVisible(ptr long)
@ stdcall OleQueryCreateFromData(ptr)
@ stdcall OleQueryLinkFromData(ptr)
@ stdcall OleRegEnumFormatEtc(ptr long ptr)
@ stdcall OleRegEnumVerbs(long ptr)
@ stdcall OleRegGetMiscStatus(ptr long ptr)
@ stdcall OleRegGetUserType(long long ptr)
@ stdcall OleRun(ptr)
@ stdcall OleSave(ptr ptr long)
@ stdcall OleSaveToStream(ptr ptr)
@ stdcall OleSetAutoConvert(ptr ptr)
@ stdcall OleSetClipboard(ptr)
@ stdcall OleSetContainedObject(ptr long)
@ stdcall OleSetMenuDescriptor(long long long ptr ptr)
@ stdcall OleTranslateAccelerator(ptr ptr ptr)
@ stdcall OleUninitialize()
@ stdcall OpenOrCreateStream(ptr str ptr)
@ stdcall ProgIDFromCLSID(ptr ptr)
@ stdcall PropStgNameToFmtId(wstr ptr)
@ stdcall PropSysAllocString(wstr)
@ stdcall PropSysFreeString(wstr)
@ stdcall PropVariantClear(ptr)
@ stdcall PropVariantCopy(ptr ptr)
@ stdcall ReadClassStg(ptr ptr)
@ stdcall ReadClassStm(ptr ptr)
@ stdcall ReadFmtUserTypeStg(ptr ptr ptr)
@ stdcall ReadOleStg(ptr ptr ptr ptr ptr ptr)
@ stdcall ReadStringStream(ptr str)
@ stdcall RegisterDragDrop(long ptr)
@ stdcall ReleaseStgMedium(ptr)
@ stdcall RevokeDragDrop(long)
@ stdcall SNB_UserFree(ptr ptr)
@ stdcall SNB_UserMarshal(ptr ptr ptr)
@ stdcall SNB_UserSize(ptr long ptr)
@ stdcall SNB_UserUnmarshal(ptr ptr ptr)
@ stdcall STGMEDIUM_UserFree(ptr ptr)
@ stdcall STGMEDIUM_UserMarshal(ptr ptr ptr)
@ stdcall STGMEDIUM_UserSize(ptr long ptr)
@ stdcall STGMEDIUM_UserUnmarshal(ptr ptr ptr)
@ stdcall SetConvertStg(ptr long)
@ stdcall SetDocumentBitStg(ptr long)
@ stdcall SetErrorInfo(long ptr)
@ stdcall StgConvertPropertyToVariant(ptr long ptr ptr)
@ stdcall StgConvertVariantToProperty(ptr long ptr ptr long long ptr)
@ stdcall StgCreateDocfile(wstr long long ptr)
@ stdcall StgCreateDocfileOnILockBytes(ptr long long ptr)
@ stdcall StgCreatePropSetStg(ptr long ptr)
@ stdcall StgCreatePropStg(ptr ptr ptr long long ptr)
@ stdcall StgCreateStorageEx(wstr long long long ptr ptr ptr ptr)
@ stdcall StgGetIFillLockBytesOnFile(ptr ptr)
@ stdcall StgGetIFillLockBytesOnILockBytes(ptr ptr)
@ stdcall StgIsStorageFile(wstr)
@ stdcall StgIsStorageILockBytes(ptr)
@ stdcall StgOpenAsyncDocfileOnIFillLockBytes(ptr long long ptr)
@ stdcall StgOpenPropStg(ptr ptr long long ptr)
@ stdcall StgOpenStorage(wstr ptr long ptr long ptr)
@ stdcall StgOpenStorageEx(wstr long long long ptr ptr ptr ptr)
@ stdcall StgOpenStorageOnHandle(ptr long long long ptr ptr) olebase.StgOpenStorageOnHandle
@ stdcall StgOpenStorageOnILockBytes(ptr ptr long long long ptr)
@ stdcall StgPropertyLengthAsVariant(ptr long long long) olebase.StgPropertyLengthAsVariant
@ stdcall StgSetTimes(wstr ptr ptr ptr )
@ stdcall StringFromCLSID(ptr ptr)
@ stdcall StringFromGUID2(ptr ptr long)
@ stdcall StringFromIID(ptr ptr) StringFromCLSID
@ stdcall UpdateDCOMSettings()
@ stdcall UtConvertDvtd16toDvtd32(ptr ptr ptr)
@ stdcall UtConvertDvtd32toDvtd16(ptr ptr ptr)
@ stdcall UtGetDvtd16Info(ptr ptr)
@ stdcall UtGetDvtd32Info(ptr ptr)
@ stdcall WdtpInterfacePointer_UserFree(ptr)
@ stdcall WdtpInterfacePointer_UserMarshal(ptr long ptr ptr ptr)
@ stdcall WdtpInterfacePointer_UserSize(ptr long ptr long ptr)
@ stdcall WdtpInterfacePointer_UserUnmarshal(ptr ptr ptr ptr)
@ stdcall WriteClassStg(ptr ptr)
@ stdcall WriteClassStm(ptr ptr)
@ stdcall WriteFmtUserTypeStg(ptr long ptr)
@ stdcall WriteOleStg(ptr ptr long ptr)
@ stdcall WriteStringStream(ptr str)

#missing on XP
@ stdcall CoVrfCheckThreadState(ptr) olebase.CoVrfCheckThreadState
@ stdcall CoVrfGetThreadState(ptr) olebase.CoVrfGetThreadState
@ stdcall CoVrfReleaseThreadState(ptr) olebase.CoVrfReleaseThreadState
@ stdcall UpdateProcessTracing(ptr ptr) olebase.UpdateProcessTracing
@ stdcall CoGetModuleType(wstr ptr) olebase.CoGetModuleType

#win Vista functions
@ stdcall CoDisconnectContext(long)
#Comment because link with propsys cause a error on Office 2010 installation
; @ stdcall PropVariantToVariant(ptr ptr) propsys.PropVariantToVariant
; @ stdcall StgDeserializePropVariant(ptr long ptr) propsys.StgDeserializePropVariant
; @ stdcall StgSerializePropVariant(ptr ptr ptr) propsys.StgSerializePropVariant
; @ stdcall VariantToPropVariant(ptr ptr) propsys.VariantToPropVariant

#win7 functions
@ stdcall CoGetApartmentType(ptr ptr)
@ stdcall CoGetActivationState(int128 long ptr)
@ stdcall CoGetCallState(long ptr)

#for WinXP
@ stdcall ComPs_CStdStubBuffer_AddRef(ptr) olebase.ComPs_CStdStubBuffer_AddRef
@ stdcall ComPs_CStdStubBuffer_Connect(ptr ptr) olebase.ComPs_CStdStubBuffer_Connect
@ stdcall ComPs_CStdStubBuffer_CountRefs(ptr) olebase.ComPs_CStdStubBuffer_CountRefs
@ stdcall ComPs_CStdStubBuffer_DebugServerQueryInterface(ptr ptr) olebase.ComPs_CStdStubBuffer_DebugServerQueryInterface
@ stdcall ComPs_CStdStubBuffer_DebugServerRelease(ptr ptr) olebase.ComPs_CStdStubBuffer_DebugServerRelease
@ stdcall ComPs_CStdStubBuffer_Disconnect(ptr) olebase.ComPs_CStdStubBuffer_Disconnect
@ stdcall ComPs_CStdStubBuffer_Invoke(ptr ptr ptr) olebase.ComPs_CStdStubBuffer_Invoke
@ stdcall ComPs_CStdStubBuffer_IsIIDSupported(ptr long) olebase.ComPs_CStdStubBuffer_IsIIDSupported
@ stdcall ComPs_CStdStubBuffer_QueryInterface(ptr long ptr) olebase.ComPs_CStdStubBuffer_QueryInterface
@ stdcall ComPs_IUnknown_AddRef_Proxy(ptr) olebase.ComPs_IUnknown_AddRef_Proxy
@ stdcall ComPs_IUnknown_QueryInterface_Proxy(ptr long ptr) olebase.ComPs_IUnknown_QueryInterface_Proxy 
@ stdcall ComPs_IUnknown_Release_Proxy(ptr) olebase.ComPs_IUnknown_Release_Proxy 
@ stdcall ComPs_NdrClientCall2(ptr ptr ptr) olebase.ComPs_NdrClientCall2 
@ stdcall ComPs_NdrClientCall2_va(ptr ptr ptr) olebase.ComPs_NdrClientCall2_va
@ stdcall ComPs_NdrCStdStubBuffer2_Release(ptr ptr) olebase.ComPs_NdrCStdStubBuffer2_Release
@ stdcall ComPs_NdrCStdStubBuffer_Release(ptr ptr) olebase.ComPs_NdrCStdStubBuffer_Release
@ stdcall ComPs_NdrStubCall2(ptr ptr ptr ptr) olebase.ComPs_NdrStubCall2
@ stdcall ComPs_NdrStubForwardingFunction(ptr ptr ptr ptr) olebase.ComPs_NdrStubForwardingFunction 
@ stdcall CoRegisterActivationFilter(ptr) olebase.CoRegisterActivationFilter

#for XP x64
@ stdcall -arch=x86_64 CLIPFORMAT_UserFree64(ptr ptr) olebase.CLIPFORMAT_UserFree64
@ stdcall -arch=x86_64 CLIPFORMAT_UserMarshal64(ptr ptr ptr) olebase.CLIPFORMAT_UserMarshal64
@ stdcall -arch=x86_64 CLIPFORMAT_UserSize64(ptr long ptr) olebase.CLIPFORMAT_UserSize64
@ stdcall -arch=x86_64 CLIPFORMAT_UserUnmarshal64(ptr ptr ptr) olebase.CLIPFORMAT_UserUnmarshal64
@ stdcall -arch=x86_64 HACCEL_UserFree64(ptr ptr) olebase.HACCEL_UserFree64
@ stdcall -arch=x86_64 HACCEL_UserMarshal64(ptr ptr ptr) olebase.HACCEL_UserMarshal64
@ stdcall -arch=x86_64 HACCEL_UserSize64(ptr long ptr) olebase.HACCEL_UserSize64
@ stdcall -arch=x86_64 HACCEL_UserUnmarshal64(ptr ptr ptr) olebase.HACCEL_UserUnmarshal64
@ stdcall -arch=x86_64 HBITMAP_UserFree64(ptr ptr) olebase.HBITMAP_UserFree64
@ stdcall -arch=x86_64 HBITMAP_UserMarshal64(ptr ptr ptr) olebase.HBITMAP_UserMarshal64
@ stdcall -arch=x86_64 HBITMAP_UserSize64(ptr long ptr) olebase.HBITMAP_UserSize64
@ stdcall -arch=x86_64 HBITMAP_UserUnmarshal64(ptr ptr ptr) olebase.HBITMAP_UserUnmarshal64
@ stdcall -arch=x86_64 HBRUSH_UserFree64(ptr ptr) olebase.HBRUSH_UserFree64
@ stdcall -arch=x86_64 HBRUSH_UserMarshal64(ptr ptr ptr) olebase.HBRUSH_UserMarshal64
@ stdcall -arch=x86_64 HBRUSH_UserSize64(ptr ptr) olebase.HBRUSH_UserSize64
@ stdcall -arch=x86_64 HBRUSH_UserUnmarshal64(ptr ptr ptr) olebase.HBRUSH_UserUnmarshal64
@ stdcall -arch=x86_64 HDC_UserFree64(ptr ptr) olebase.HDC_UserFree64
@ stdcall -arch=x86_64 HDC_UserMarshal64(ptr ptr ptr) olebase.HDC_UserMarshal64
@ stdcall -arch=x86_64 HDC_UserSize64(ptr long ptr) olebase.HDC_UserSize64
@ stdcall -arch=x86_64 HDC_UserUnmarshal64(ptr ptr ptr) olebase.HDC_UserUnmarshal64
@ stdcall -arch=x86_64 HENHMETAFILE_UserFree64(ptr ptr) olebase.HENHMETAFILE_UserFree64
@ stdcall -arch=x86_64 HENHMETAFILE_UserMarshal64(ptr ptr ptr) olebase.HENHMETAFILE_UserMarshal64
@ stdcall -arch=x86_64 HENHMETAFILE_UserSize64(ptr long ptr) olebase.HENHMETAFILE_UserSize64
@ stdcall -arch=x86_64 HENHMETAFILE_UserUnmarshal64(ptr ptr ptr) olebase.HENHMETAFILE_UserUnmarshal64
@ stdcall -arch=x86_64 HGLOBAL_UserFree64(ptr ptr) olebase.HGLOBAL_UserFree64
@ stdcall -arch=x86_64 HGLOBAL_UserMarshal64(ptr ptr ptr) olebase.HGLOBAL_UserMarshal64
@ stdcall -arch=x86_64 HGLOBAL_UserSize64(ptr long ptr) olebase.HGLOBAL_UserSize64
@ stdcall -arch=x86_64 HGLOBAL_UserUnmarshal64(ptr ptr ptr) olebase.HGLOBAL_UserUnmarshal64
@ stdcall -arch=x86_64 HICON_UserFree64(ptr ptr) olebase.HICON_UserFree64
@ stdcall -arch=x86_64 HICON_UserMarshal64(ptr ptr ptr) olebase.HICON_UserMarshal64
@ stdcall -arch=x86_64 HICON_UserSize64(ptr long ptr) olebase.HICON_UserSize64
@ stdcall -arch=x86_64 HICON_UserUnmarshal64(ptr ptr ptr) olebase.HICON_UserUnmarshal64
@ stdcall -arch=x86_64 HMENU_UserFree64(ptr ptr) olebase.HMENU_UserFree64
@ stdcall -arch=x86_64 HMENU_UserMarshal64(ptr ptr ptr) olebase.HMENU_UserMarshal64
@ stdcall -arch=x86_64 HMENU_UserSize64(ptr long ptr) olebase.HMENU_UserSize64
@ stdcall -arch=x86_64 HMENU_UserUnmarshal64(ptr ptr ptr) olebase.HMENU_UserUnmarshal64
@ stdcall -arch=x86_64 HMETAFILE_UserFree64(ptr ptr) olebase.HMETAFILE_UserFree64
@ stdcall -arch=x86_64 HMETAFILE_UserMarshal64(ptr ptr ptr) olebase.HMETAFILE_UserMarshal64
@ stdcall -arch=x86_64 HMETAFILE_UserSize64(ptr long ptr) olebase.HMETAFILE_UserSize64
@ stdcall -arch=x86_64 HMETAFILE_UserUnmarshal64(ptr ptr ptr) olebase.HMETAFILE_UserUnmarshal64
@ stdcall -arch=x86_64 HMETAFILEPICT_UserFree64(ptr ptr) olebase.HMETAFILEPICT_UserFree64
@ stdcall -arch=x86_64 HMETAFILEPICT_UserMarshal64(ptr ptr ptr) olebase.HMETAFILEPICT_UserMarshal64
@ stdcall -arch=x86_64 HMETAFILEPICT_UserSize64(ptr long ptr) olebase.HMETAFILEPICT_UserSize64
@ stdcall -arch=x86_64 HMETAFILEPICT_UserUnmarshal64(ptr ptr ptr) olebase.HMETAFILEPICT_UserUnmarshal64
@ stdcall -arch=x86_64 HPALETTE_UserFree64(ptr ptr) olebase.HPALETTE_UserFree64
@ stdcall -arch=x86_64 HPALETTE_UserMarshal64(ptr ptr ptr) olebase.HPALETTE_UserMarshal64
@ stdcall -arch=x86_64 HPALETTE_UserSize64(ptr long ptr) olebase.HPALETTE_UserSize64
@ stdcall -arch=x86_64 HPALETTE_UserUnmarshal64(ptr ptr ptr) olebase.HPALETTE_UserUnmarshal64
@ stdcall -arch=x86_64 HWND_UserFree64(ptr ptr) olebase.HWND_UserFree64
@ stdcall -arch=x86_64 HWND_UserMarshal64(ptr ptr ptr) olebase.HWND_UserMarshal64
@ stdcall -arch=x86_64 HWND_UserSize64(ptr long ptr) olebase.HWND_UserSize64
@ stdcall -arch=x86_64 HWND_UserUnmarshal64(ptr ptr ptr) olebase.HWND_UserUnmarshal64
@ stdcall -arch=x86_64 SNB_UserFree64(ptr ptr) olebase.SNB_UserFree64
@ stdcall -arch=x86_64 SNB_UserMarshal64(ptr ptr ptr) olebase.SNB_UserMarshal64
@ stdcall -arch=x86_64 SNB_UserSize64(ptr long ptr) olebase.SNB_UserSize64
@ stdcall -arch=x86_64 SNB_UserUnmarshal64(ptr ptr ptr) olebase.SNB_UserUnmarshal64
@ stdcall -arch=x86_64 STGMEDIUM_UserFree64(ptr ptr) olebase.STGMEDIUM_UserFree64
@ stdcall -arch=x86_64 STGMEDIUM_UserMarshal64(ptr ptr ptr) olebase.STGMEDIUM_UserMarshal64
@ stdcall -arch=x86_64 STGMEDIUM_UserSize64(ptr long ptr) olebase.STGMEDIUM_UserSize64
@ stdcall -arch=x86_64 STGMEDIUM_UserUnmarshal64(ptr ptr ptr) olebase.STGMEDIUM_UserUnmarshal64
@ stdcall -arch=x86_64 WdtpInterfacePointer_UserFree64(ptr) olebase.WdtpInterfacePointer_UserFree64
@ stdcall -arch=x86_64 WdtpInterfacePointer_UserMarshal64(ptr long ptr ptr ptr) olebase.WdtpInterfacePointer_UserMarshal64
@ stdcall -arch=x86_64 WdtpInterfacePointer_UserSize64(ptr long ptr long ptr) olebase.WdtpInterfacePointer_UserSize64
@ stdcall -arch=x86_64 WdtpInterfacePointer_UserUnmarshal64(ptr ptr ptr ptr) olebase.WdtpInterfacePointer_UserUnmarshal64