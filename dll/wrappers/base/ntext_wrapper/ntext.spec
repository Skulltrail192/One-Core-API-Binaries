@ stdcall PropertyLengthAsVariant(ptr long long long)
@ stdcall RtlConvertPropertyToVariant(ptr long ptr ptr)
@ stdcall RtlConvertVariantToProperty(ptr long ptr ptr ptr long ptr)
@ stdcall RtlActivateActivationContextUnsafeFast(ptr ptr) ntdll.RtlActivateActivationContextUnsafeFast
@ stdcall RtlDeactivateActivationContextUnsafeFast(ptr) ntdll.RtlDeactivateActivationContextUnsafeFast
@ stdcall RtlInterlockedPushListSList(ptr ptr ptr long) ntdll.RtlInterlockedPushListSList
@ stdcall -arch=i386 RtlUlongByteSwap(long) ntdll.RtlUlongByteSwap
@ stdcall -arch=i386 RtlUlonglongByteSwap(double) ntdll.RtlUlonglongByteSwap
@ stdcall -arch=i386 RtlUshortByteSwap(long) ntdll.RtlUshortByteSwap
@ stdcall CsrAllocateCaptureBuffer(long long)
@ stdcall CsrAllocateMessagePointer(ptr long ptr)
@ stdcall CsrCaptureMessageBuffer(ptr ptr long ptr)
@ stdcall CsrCaptureMessageMultiUnicodeStringsInPlace(ptr long ptr)
@ stdcall CsrCaptureMessageString(ptr str long long ptr)
@ stdcall CsrCaptureTimeout(long ptr)
@ stdcall CsrClientCallServer(ptr ptr long long)
@ stdcall CsrClientConnectToServer(str long ptr ptr ptr)
@ stdcall CsrFreeCaptureBuffer(ptr)
@ stdcall CsrGetProcessId()
@ stdcall CsrIdentifyAlertableThread()
@ stdcall CsrNewThread()
@ stdcall CsrProbeForRead(ptr long long)
@ stdcall CsrProbeForWrite(ptr long long)
@ stdcall CsrSetPriorityClass(ptr ptr)
@ stdcall DbgBreakPoint()
@ varargs DbgPrint(str)
@ varargs DbgPrintEx(long long str)
@ varargs DbgPrintReturnControlC(str)
@ stdcall DbgPrompt(ptr ptr long)
@ stdcall DbgQueryDebugFilterState(long long)
@ stdcall DbgSetDebugFilterState(long long long)
@ stdcall DbgUiConnectToDbg()
@ stdcall DbgUiContinue(ptr long)
@ stdcall DbgUiConvertStateChangeStructure(ptr ptr)
@ stdcall DbgUiDebugActiveProcess(ptr)
@ stdcall DbgUiGetThreadDebugObject()
@ stdcall DbgUiIssueRemoteBreakin(ptr)
@ stdcall DbgUiRemoteBreakin()
@ stdcall DbgUiSetThreadDebugObject(ptr)
@ stdcall DbgUiStopDebugging(ptr)
@ stdcall DbgUiWaitStateChange(ptr ptr)
@ stdcall DbgUserBreakPoint()
@ stdcall KiRaiseUserExceptionDispatcher()
@ stdcall KiUserApcDispatcher(ptr ptr ptr ptr)
@ stdcall KiUserCallbackDispatcher(ptr ptr long) ; CHECKME
@ stdcall KiUserExceptionDispatcher(ptr ptr)
@ stdcall LdrAccessOutOfProcessResource(ptr ptr ptr ptr ptr)
@ stdcall LdrAccessResource(long ptr ptr ptr)
@ stdcall LdrAddRefDll(long ptr)
@ stdcall LdrAlternateResourcesEnabled()
@ stdcall LdrCreateOutOfProcessImage(long ptr ptr ptr)
@ stdcall LdrDestroyOutOfProcessImage(ptr)
@ stdcall LdrDisableThreadCalloutsForDll(long)
@ stdcall LdrEnumResources(ptr ptr long ptr ptr)
@ stdcall LdrEnumerateLoadedModules(long ptr long)
@ stdcall LdrFindCreateProcessManifest(long ptr ptr long ptr) ; 5.1 and 5.2 only
@ stdcall LdrFindEntryForAddress(ptr ptr)
@ stdcall LdrFindResourceDirectory_U(long ptr long ptr)
@ stdcall LdrFindResourceEx_U(long ptr long ptr ptr)
@ stdcall LdrFindResource_U(long ptr long ptr) 
@ stdcall LdrFlushAlternateResourceModules()
@ stdcall LdrGetDllHandle(wstr long ptr ptr)
@ stdcall LdrGetDllHandleEx(long wstr long ptr ptr)
@ stdcall LdrGetProcedureAddress(ptr ptr long ptr)
@ stdcall LdrHotPatchRoutine(long)
@ stdcall LdrInitShimEngineDynamic(long)
@ stdcall LdrInitializeThunk(ptr ptr ptr) 
@ stdcall LdrLoadAlternateResourceModule(ptr wstr)
@ stdcall LdrLoadDll(wstr long ptr ptr)
@ stdcall LdrLockLoaderLock(long ptr ptr)
@ stdcall LdrOpenImageFileOptionsKey(ptr long ptr) ; 5.2 SP1 and higher
@ stdcall LdrProcessRelocationBlock(ptr long ptr long)
@ stdcall LdrQueryImageFileExecutionOptions(ptr str long ptr long ptr)
@ stdcall LdrQueryImageFileExecutionOptionsEx(ptr ptr long ptr long ptr long)
@ stdcall LdrQueryImageFileKeyOption(ptr ptr long ptr long ptr) ;implemented here
@ stdcall LdrQueryProcessModuleInformation(ptr long ptr)
@ stdcall LdrSetAppCompatDllRedirectionCallback(long long long)
@ stdcall LdrSetDllManifestProber(ptr)
@ stdcall LdrShutdownProcess()
@ stdcall LdrShutdownThread()
@ stdcall LdrUnloadAlternateResourceModule(ptr)
@ stdcall LdrUnloadDll(ptr)
@ stdcall LdrUnlockLoaderLock(long long)
@ stdcall LdrVerifyImageMatchesChecksum(ptr long long long)
@ extern NlsAnsiCodePage
@ extern NlsMbCodePageTag
@ extern NlsMbOemCodePageTag
@ stdcall NtAcceptConnectPort(ptr long ptr long long ptr)
@ stdcall NtAccessCheck(ptr long long ptr ptr ptr ptr ptr)
@ stdcall NtAccessCheckAndAuditAlarm(ptr long ptr ptr ptr long ptr long ptr ptr ptr)
@ stdcall NtAccessCheckByType(ptr ptr ptr long ptr long ptr ptr long ptr ptr)
@ stdcall NtAccessCheckByTypeAndAuditAlarm(ptr ptr ptr ptr ptr ptr long long long ptr long ptr long ptr ptr ptr)
@ stdcall NtAccessCheckByTypeResultList(ptr ptr ptr long ptr long ptr ptr long ptr ptr)
@ stdcall NtAccessCheckByTypeResultListAndAuditAlarm(ptr ptr ptr ptr ptr ptr long long long ptr long ptr long ptr ptr ptr)
@ stdcall NtAccessCheckByTypeResultListAndAuditAlarmByHandle(ptr ptr ptr ptr ptr ptr ptr long long long ptr long ptr long ptr ptr ptr)
@ stdcall NtAddAtom(ptr long ptr)
@ stdcall NtAddBootEntry(ptr long)
@ stdcall NtAdjustGroupsToken(long long ptr long ptr ptr)
@ stdcall NtAdjustPrivilegesToken(long long long long long long)
@ stdcall NtAlertResumeThread(long ptr)
@ stdcall NtAlertThread(long)
@ stdcall NtAllocateLocallyUniqueId(ptr)
@ stdcall NtAllocateUserPhysicalPages(ptr ptr ptr)
@ stdcall NtAllocateUuids(ptr ptr ptr ptr)
@ stdcall NtAllocateVirtualMemory(long ptr ptr ptr long long)
@ stdcall NtAreMappedFilesTheSame(ptr ptr)
@ stdcall NtAssignProcessToJobObject(long long)
@ stdcall NtCallbackReturn(ptr long long)
@ stdcall NtCancelDeviceWakeupRequest(ptr)
@ stdcall NtCancelIoFile(long ptr)
@ stdcall NtCancelTimer(long ptr)
@ stdcall NtClearEvent(long)
@ stdcall NtClose(long)
@ stdcall NtCloseObjectAuditAlarm(ptr ptr long)
@ stdcall NtCompactKeys(long ptr)
@ stdcall NtCompareTokens(ptr ptr ptr)
@ stdcall NtCompleteConnectPort(ptr)
@ stdcall NtCompressKey(ptr)
@ stdcall NtConnectPort(ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall NtContinue(ptr long)
@ stdcall NtCreateDebugObject(ptr long ptr long)
@ stdcall NtCreateDirectoryObject(long long long)
@ stdcall NtCreateEvent(long long long long long)
@ stdcall NtCreateEventPair(ptr long ptr)
@ stdcall NtCreateFile(ptr long ptr ptr long long long ptr long long ptr)
@ stdcall NtCreateIoCompletion(ptr long ptr long)
@ stdcall NtCreateJobObject(ptr long ptr)
@ stdcall NtCreateJobSet(long ptr long)
@ stdcall NtCreateKey(ptr long ptr long ptr long long)
@ stdcall NtCreateKeyedEvent(ptr long ptr long)
@ stdcall NtCreateMailslotFile(long long long long long long long long)
@ stdcall NtCreateMutant(ptr long ptr long)
@ stdcall NtCreateNamedPipeFile(ptr long ptr ptr long long long long long long long long long ptr)
@ stdcall NtCreatePagingFile(long long long long)
@ stdcall NtCreatePort(ptr ptr long long ptr)
@ stdcall NtCreateProcess(ptr long ptr ptr long ptr ptr ptr)
@ stdcall NtCreateProcessEx(ptr long ptr ptr long ptr ptr ptr long)
@ stdcall NtCreateProfile(ptr ptr ptr long long ptr long long long) ; CHECKME
@ stdcall NtCreateSection(ptr long ptr ptr long long long)
@ stdcall NtCreateSemaphore(ptr long ptr long long)
@ stdcall NtCreateSymbolicLinkObject(ptr long ptr ptr)
@ stdcall NtCreateThread(ptr long ptr ptr ptr ptr ptr long)
@ stdcall NtCreateTimer(ptr long ptr long)
@ stdcall NtCreateToken(ptr long ptr long ptr ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall NtCreateWaitablePort(ptr ptr long long long)
@ stdcall -arch=win32 NtCurrentTeb() 
@ stdcall NtDebugActiveProcess(ptr ptr)
@ stdcall NtDebugContinue(ptr ptr long)
@ stdcall NtDelayExecution(long ptr)
@ stdcall NtDeleteAtom(long)
@ stdcall NtDeleteBootEntry(long)
@ stdcall NtDeleteFile(ptr)
@ stdcall NtDeleteKey(long)
@ stdcall NtDeleteObjectAuditAlarm(ptr ptr long)
@ stdcall NtDeleteValueKey(long ptr)
@ stdcall NtDeviceIoControlFile(long long long long long long long long long long)
@ stdcall NtDisplayString(ptr)
@ stdcall NtDuplicateObject(long long long ptr long long long)
@ stdcall NtDuplicateToken(long long long long long long)
@ stdcall NtEnumerateBootEntries(ptr ptr)
@ stdcall NtEnumerateKey (long long long long long long)
@ stdcall NtEnumerateSystemEnvironmentValuesEx(long ptr long)
@ stdcall NtEnumerateValueKey(long long long long long long)
@ stdcall NtExtendSection(ptr ptr)
@ stdcall NtFilterToken(ptr long ptr ptr ptr ptr)
@ stdcall NtFindAtom(ptr long ptr)
@ stdcall NtFlushBuffersFile(long ptr)
@ stdcall NtFlushInstructionCache(long ptr long)
@ stdcall NtFlushKey(long)
@ stdcall NtFlushVirtualMemory(long ptr ptr long)
@ stdcall NtFlushWriteBuffer()
@ stdcall NtFreeUserPhysicalPages(ptr ptr ptr)
@ stdcall NtFreeVirtualMemory(long ptr ptr long)
@ stdcall NtFsControlFile(long long long long long long long long long long)
@ stdcall NtGetContextThread(long ptr)
@ stdcall NtGetDevicePowerState(ptr ptr)
@ stdcall NtGetPlugPlayEvent(long long ptr long)
@ stdcall NtGetWriteWatch(long long ptr long ptr ptr ptr)
@ stdcall NtImpersonateAnonymousToken(ptr)
@ stdcall NtImpersonateClientOfPort(ptr ptr)
@ stdcall NtImpersonateThread(ptr ptr ptr)
@ stdcall NtInitializeRegistry(long)
@ stdcall NtInitiatePowerAction (long long long long)
@ stdcall NtIsProcessInJob(long long)
@ stdcall NtIsSystemResumeAutomatic()
@ stdcall NtListenPort(ptr ptr)
@ stdcall NtLoadDriver(ptr)
@ stdcall NtLoadKey2(ptr ptr long)
@ stdcall NtLoadKey(ptr ptr)
@ stdcall NtLockFile(long long ptr ptr ptr ptr ptr ptr long long)
@ stdcall NtLockProductActivationKeys(ptr ptr)
@ stdcall NtLockRegistryKey(ptr)
@ stdcall NtLockVirtualMemory(long ptr ptr long)
@ stdcall NtMakePermanentObject(ptr)
@ stdcall NtMakeTemporaryObject(long)
@ stdcall NtMapUserPhysicalPages(ptr ptr ptr)
@ stdcall NtMapUserPhysicalPagesScatter(ptr ptr ptr)
@ stdcall NtMapViewOfSection(long long ptr long long ptr ptr long long long)
@ stdcall NtModifyBootEntry(ptr)
@ stdcall NtNotifyChangeDirectoryFile(long long ptr ptr ptr ptr long long long)
@ stdcall NtNotifyChangeKey(long long ptr ptr ptr long long ptr long long)
@ stdcall NtNotifyChangeMultipleKeys(ptr long ptr ptr ptr ptr ptr long long ptr long long)
@ stdcall NtOpenDirectoryObject(long long long)
@ stdcall NtOpenEvent(long long long)
@ stdcall NtOpenEventPair(ptr long ptr)
@ stdcall NtOpenFile(ptr long ptr ptr long long)
@ stdcall NtOpenIoCompletion(ptr long ptr)
@ stdcall NtOpenJobObject(ptr long ptr)
@ stdcall NtOpenKey(ptr long ptr)
@ stdcall NtOpenKeyedEvent(ptr long ptr)
@ stdcall NtOpenMutant(ptr long ptr)
@ stdcall NtOpenObjectAuditAlarm(ptr ptr ptr ptr ptr ptr long long ptr long long ptr)
@ stdcall NtOpenProcess(ptr long ptr ptr)
@ stdcall NtOpenProcessToken(long long ptr)
@ stdcall NtOpenProcessTokenEx(long long long ptr)
@ stdcall NtOpenSection(ptr long ptr)
@ stdcall NtOpenSemaphore(long long ptr)
@ stdcall NtOpenSymbolicLinkObject (ptr long ptr)
@ stdcall NtOpenThread(ptr long ptr ptr)
@ stdcall NtOpenThreadToken(long long long ptr)
@ stdcall NtOpenThreadTokenEx(long long long long ptr)
@ stdcall NtOpenTimer(ptr long ptr)
@ stdcall NtPlugPlayControl(ptr ptr long)
@ stdcall NtPowerInformation(long ptr long ptr long)
@ stdcall NtPrivilegeCheck(ptr ptr ptr)
@ stdcall NtPrivilegeObjectAuditAlarm(ptr ptr ptr long ptr long)
@ stdcall NtPrivilegedServiceAuditAlarm(ptr ptr ptr ptr long)
@ stdcall NtProtectVirtualMemory(long ptr ptr long ptr)
@ stdcall NtPulseEvent(long ptr)
@ stdcall NtQueryAttributesFile(ptr ptr)
@ stdcall NtQueryBootEntryOrder(ptr ptr)
@ stdcall NtQueryBootOptions(ptr ptr)
@ stdcall NtQueryDebugFilterState(long long)
@ stdcall NtQueryDefaultLocale(long ptr)
@ stdcall NtQueryDefaultUILanguage(ptr)
@ stdcall NtQueryDirectoryFile(long long ptr ptr ptr ptr long long long ptr long)
@ stdcall NtQueryDirectoryObject(long ptr long long long ptr ptr)
@ stdcall NtQueryEaFile(long ptr ptr long long ptr long ptr long)
@ stdcall NtQueryEvent(long long ptr long ptr)
@ stdcall NtQueryFullAttributesFile(ptr ptr)
@ stdcall NtQueryInformationAtom(long long ptr long ptr)
@ stdcall NtQueryInformationFile(long ptr ptr long long)
@ stdcall NtQueryInformationJobObject(long long ptr long ptr)
@ stdcall NtQueryInformationPort(ptr long ptr long ptr)
@ stdcall NtQueryInformationProcess(long long ptr long ptr)
@ stdcall NtQueryInformationThread(long long ptr long ptr)
@ stdcall NtQueryInformationToken(long long ptr long ptr) ;NtpQueryInformationToken #Wrapper needed for Integrity Level Introduzed on Vista
@ stdcall NtQueryInstallUILanguage(ptr)
@ stdcall NtQueryIntervalProfile(long ptr)
@ stdcall NtQueryIoCompletion(long long ptr long ptr)
@ stdcall NtQueryKey (long long ptr long ptr)
@ stdcall NtQueryMultipleValueKey(long ptr long ptr long ptr)
@ stdcall NtQueryMutant(long long ptr long ptr)
@ stdcall NtQueryObject(long long long long long)
@ stdcall NtQueryOpenSubKeys(ptr ptr)
@ stdcall NtQueryPerformanceCounter(ptr ptr)
@ stdcall NtQueryPortInformationProcess()
@ stdcall NtQueryQuotaInformationFile(ptr ptr ptr long long ptr long ptr long)
@ stdcall NtQuerySection(long long long long long) NtQuerySectionInternal
@ stdcall NtQuerySecurityObject (long long long long long)
@ stdcall NtQuerySemaphore (long long ptr long ptr)
@ stdcall NtQuerySymbolicLinkObject(long ptr ptr)
@ stdcall NtQuerySystemEnvironmentValue(ptr ptr long ptr)
@ stdcall NtQuerySystemEnvironmentValueEx(ptr ptr ptr ptr ptr)
@ stdcall NtQuerySystemInformation(long long long long)
@ stdcall NtQuerySystemTime(ptr)
@ stdcall NtQueryTimer(ptr long ptr long ptr)
@ stdcall NtQueryTimerResolution(long long long)
@ stdcall NtQueryValueKey(long long long long long long)
@ stdcall NtQueryVirtualMemory(long ptr long ptr long ptr)
@ stdcall NtQueryVolumeInformationFile(long ptr ptr long long)
@ stdcall NtQueueApcThread(long ptr long long long)
@ stdcall NtRaiseException(ptr ptr long)
@ stdcall NtRaiseHardError(long long long ptr long ptr)
@ stdcall NtReadFile(long long ptr ptr ptr ptr long ptr ptr)
@ stdcall NtReadFileScatter(long long ptr ptr ptr ptr long ptr ptr)
@ stdcall NtReadRequestData(ptr ptr long ptr long ptr)
@ stdcall NtReadVirtualMemory(long ptr ptr long ptr)
@ stdcall NtRegisterThreadTerminatePort(ptr)
@ stdcall NtReleaseKeyedEvent(ptr ptr long ptr)
@ stdcall NtReleaseMutant(long ptr)
@ stdcall NtReleaseSemaphore(long long ptr)
@ stdcall NtRemoveIoCompletion(ptr ptr ptr ptr ptr)
@ stdcall NtRemoveProcessDebug(ptr ptr)
@ stdcall NtRenameKey(ptr ptr)
@ stdcall NtReplaceKey(ptr long ptr)
@ stdcall NtReplyPort(ptr ptr)
@ stdcall NtReplyWaitReceivePort(ptr ptr ptr ptr)
@ stdcall NtReplyWaitReceivePortEx(ptr ptr ptr ptr ptr)
@ stdcall NtReplyWaitReplyPort(ptr ptr)
@ stdcall NtRequestDeviceWakeup(ptr)
@ stdcall NtRequestPort(ptr ptr)
@ stdcall NtRequestWaitReplyPort(ptr ptr ptr)
@ stdcall NtRequestWakeupLatency(long)
@ stdcall NtResetEvent(long ptr)
@ stdcall NtResetWriteWatch(long ptr long)
@ stdcall NtRestoreKey(long long long)
@ stdcall NtResumeProcess(ptr)
@ stdcall NtResumeThread(long long)
@ stdcall NtSaveKey(long long)
@ stdcall NtSaveKeyEx(ptr ptr long)
@ stdcall NtSaveMergedKeys(ptr ptr ptr)
@ stdcall NtSecureConnectPort(ptr ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall NtSetBootEntryOrder(ptr ptr)
@ stdcall NtSetBootOptions(ptr long)
@ stdcall NtSetContextThread(long ptr)
@ stdcall NtSetDebugFilterState(long long long)
@ stdcall NtSetDefaultHardErrorPort(ptr)
@ stdcall NtSetDefaultLocale(long long)
@ stdcall NtSetDefaultUILanguage(long)
@ stdcall NtSetEaFile(long ptr ptr long)
@ stdcall NtSetEvent(long long)
@ stdcall NtSetEventBoostPriority(ptr)
@ stdcall NtSetHighEventPair(ptr)
@ stdcall NtSetHighWaitLowEventPair(ptr)
@ stdcall NtSetInformationDebugObject(ptr long ptr long ptr)
@ stdcall NtSetInformationFile(long long long long long)
@ stdcall NtSetInformationJobObject(long long ptr long)
@ stdcall NtSetInformationKey(long long ptr long)
@ stdcall NtSetInformationObject(long long ptr long)
@ stdcall NtSetInformationProcess(long long long long)
@ stdcall NtSetInformationThread(long long ptr long)
@ stdcall NtSetInformationToken(long long ptr long)
@ stdcall NtSetIntervalProfile(long long)
@ stdcall NtSetIoCompletion(ptr long ptr long long)
@ stdcall NtSetLdtEntries(long double long double) ; CHECKME
@ stdcall NtSetLowEventPair(ptr)
@ stdcall NtSetLowWaitHighEventPair(ptr)
@ stdcall NtSetQuotaInformationFile(ptr ptr ptr long)
@ stdcall NtSetSecurityObject(long long ptr)
@ stdcall NtSetSystemEnvironmentValue(ptr ptr)
@ stdcall NtSetSystemEnvironmentValueEx(ptr ptr ptr long long)
@ stdcall NtSetSystemInformation(long ptr long)
@ stdcall NtSetSystemPowerState(long long long)
@ stdcall NtSetSystemTime(ptr ptr)
@ stdcall NtSetThreadExecutionState(long ptr)
@ stdcall NtSetTimer(long ptr ptr ptr long long ptr)
@ stdcall NtSetTimerResolution(long long ptr)
@ stdcall NtSetUuidSeed(ptr)
@ stdcall NtSetValueKey(long long long long long long)
@ stdcall NtSetVolumeInformationFile(long ptr ptr long long)
@ stdcall NtShutdownSystem(long)
@ stdcall NtSignalAndWaitForSingleObject(long long long ptr)
@ stdcall NtStartProfile(ptr)
@ stdcall NtStopProfile(ptr)
@ stdcall NtSuspendProcess(ptr)
@ stdcall NtSuspendThread(long ptr)
@ stdcall NtSystemDebugControl(long ptr long ptr long ptr)
@ stdcall NtTerminateJobObject(long long)
@ stdcall NtTerminateProcess(long long)
@ stdcall NtTerminateThread(long long)
@ stdcall NtTestAlert()
@ stdcall NtTraceEvent(long long long ptr)
@ stdcall NtTranslateFilePath(ptr long ptr long)
@ stdcall NtUnloadDriver(ptr)
@ stdcall NtUnloadKey(long)
@ stdcall NtUnloadKeyEx(ptr ptr)
@ stdcall NtUnlockFile(long ptr ptr ptr ptr)
@ stdcall NtUnlockVirtualMemory(long ptr ptr long)
@ stdcall NtUnmapViewOfSection(long ptr)
@ stdcall NtVdmControl(long ptr)
@ stdcall NtWaitForDebugEvent(ptr long ptr ptr)
@ stdcall NtWaitForKeyedEvent(ptr ptr long ptr)
@ stdcall NtWaitForMultipleObjects(long ptr long long ptr)
@ stdcall NtWaitForSingleObject(long long long)
@ stdcall NtWaitHighEventPair(ptr)
@ stdcall NtWaitLowEventPair(ptr)
@ stdcall NtWriteFile(long long ptr ptr ptr ptr long ptr ptr)
@ stdcall NtWriteFileGather(long long ptr ptr ptr ptr long ptr ptr)
@ stdcall NtWriteRequestData(ptr ptr long ptr long ptr)
@ stdcall NtWriteVirtualMemory(long ptr ptr long ptr)
@ stdcall NtYieldExecution()
@ stdcall PfxFindPrefix(ptr ptr)
@ stdcall PfxInitialize(ptr)
@ stdcall PfxInsertPrefix(ptr ptr ptr)
@ stdcall PfxRemovePrefix(ptr ptr)
@ stdcall RtlAbortRXact(ptr)
@ stdcall RtlAbsoluteToSelfRelativeSD(ptr ptr ptr)
@ stdcall RtlAcquirePebLock()
@ stdcall RtlAcquireResourceExclusive(ptr long)
@ stdcall RtlAcquireResourceShared(ptr long)
@ stdcall RtlActivateActivationContext(long ptr ptr)
@ stdcall RtlActivateActivationContextEx(long ptr ptr ptr)
@ stdcall RtlAddAccessAllowedAce(ptr long long ptr)
@ stdcall RtlAddAccessAllowedAceEx(ptr long long long ptr)
@ stdcall RtlAddAccessAllowedObjectAce(ptr long long long ptr ptr ptr)
@ stdcall RtlAddAccessDeniedAce(ptr long long ptr)
@ stdcall RtlAddAccessDeniedAceEx(ptr long long long ptr)
@ stdcall RtlAddAccessDeniedObjectAce(ptr long long long ptr ptr ptr)
@ stdcall RtlAddAce(ptr long long ptr long)
@ stdcall RtlAddActionToRXact(ptr long ptr long ptr long)
@ stdcall RtlAddAtomToAtomTable(ptr wstr ptr)
@ stdcall RtlAddAttributeActionToRXact(ptr long ptr ptr ptr long ptr long)
@ stdcall RtlAddAuditAccessAce(ptr long long ptr long long)
@ stdcall RtlAddAuditAccessAceEx(ptr long long long ptr long long)
@ stdcall RtlAddAuditAccessObjectAce(ptr long long long ptr ptr ptr long long)
@ stdcall RtlAddCompoundAce(ptr long long long ptr ptr)
@ stdcall -arch=x86_64 RtlAddFunctionTable(ptr long long)
@ stdcall RtlAddRefActivationContext(ptr)
@ stdcall RtlAddVectoredExceptionHandler(long ptr)
@ stdcall RtlAddressInSectionTable(ptr ptr long)
@ stdcall RtlAdjustPrivilege(long long long ptr)
@ stdcall RtlAllocateAndInitializeSid(ptr long long long long long long long long long ptr)
@ stdcall RtlAllocateHandle(ptr ptr)
@ stdcall RtlAllocateHeap(ptr long ptr)
@ stdcall RtlAnsiCharToUnicodeChar(ptr)
@ stdcall RtlAnsiStringToUnicodeSize(ptr) RtlxAnsiStringToUnicodeSize
@ stdcall RtlAnsiStringToUnicodeString(ptr ptr long)
@ stdcall RtlAppendAsciizToString(ptr str)
@ stdcall RtlAppendPathElement(long ptr ptr)
@ stdcall RtlAppendStringToString(ptr ptr)
@ stdcall RtlAppendUnicodeStringToString(ptr ptr)
@ stdcall RtlAppendUnicodeToString(ptr wstr)
@ stdcall RtlApplicationVerifierStop(long long long long long long long long long long)
@ stdcall RtlApplyRXact(ptr)
@ stdcall RtlApplyRXactNoFlush(ptr)
@ stdcall RtlAreAllAccessesGranted(long long)
@ stdcall RtlAreAnyAccessesGranted(long long)
@ stdcall RtlAreBitsClear(ptr long long)
@ stdcall RtlAreBitsSet(ptr long long)
@ stdcall RtlAssert(ptr ptr long ptr)
@ stdcall RtlCancelTimer(ptr ptr ptr) RtlDeleteTimer
@ stdcall -register RtlCaptureContext(ptr)
@ stdcall RtlCaptureStackBackTrace(long long ptr ptr)
@ stdcall -arch=i386 RtlCaptureStackContext(ptr ptr long)
@ stdcall RtlCharToInteger(ptr long ptr)
@ stdcall RtlCheckForOrphanedCriticalSections(ptr)
@ stdcall RtlCheckProcessParameters(long long long long)
@ stdcall RtlCheckRegistryKey(long ptr)
@ stdcall RtlClearAllBits(ptr)
@ stdcall RtlClearBits(ptr long long)
@ stdcall RtlCompactHeap(long long)
@ stdcall RtlCompareMemory(ptr ptr long)
@ stdcall RtlCompareMemoryUlong(ptr long long)
@ stdcall RtlCompareString(ptr ptr long)
@ stdcall RtlCompareUnicodeString(ptr ptr long)
@ stdcall RtlCompressBuffer(long ptr long ptr long long ptr ptr)
@ stdcall RtlComputeCrc32(long ptr long)
@ stdcall RtlComputeImportTableHash(ptr ptr long)
@ stdcall RtlComputePrivatizedDllName_U(ptr ptr ptr)
@ stdcall RtlConsoleMultiByteToUnicodeN(ptr long ptr ptr long ptr)
@ stdcall RtlConvertExclusiveToShared(ptr)
@ stdcall -arch=win32 -ret64 RtlConvertLongToLargeInteger(long)
@ stdcall RtlConvertSharedToExclusive(ptr)
@ stdcall RtlConvertSidToUnicodeString(ptr ptr long)
@ stdcall RtlConvertToAutoInheritSecurityObject(ptr ptr ptr ptr long ptr)
@ stdcall RtlConvertUiListToApiList(long long long)
@ stdcall -arch=win32 -ret64 RtlConvertUlongToLargeInteger(long)
@ stdcall RtlCopyLuid(ptr ptr)
@ stdcall RtlCopyLuidAndAttributesArray(long ptr ptr)
@ stdcall RtlCopySecurityDescriptor(ptr ptr)
@ stdcall RtlCopySid(long ptr ptr)
@ stdcall RtlCopySidAndAttributesArray(long ptr long ptr ptr ptr ptr)
@ stdcall RtlCopyString(ptr ptr)
@ stdcall RtlCopyUnicodeString(ptr ptr)
@ stdcall RtlCreateAcl(ptr long long)
@ stdcall RtlCreateActivationContext(long ptr long ptr ptr ptr)
@ stdcall RtlCreateAndSetSD(ptr long ptr ptr ptr)
@ stdcall RtlCreateAtomTable(long ptr)
@ stdcall RtlCreateBootStatusDataFile()
@ stdcall RtlCreateEnvironment(long ptr)
@ stdcall RtlCreateHeap(long ptr long long ptr ptr)
@ stdcall RtlCreateProcessParameters(ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall RtlCreateQueryDebugBuffer(long long)
@ stdcall RtlCreateRegistryKey(long wstr)
@ stdcall RtlCreateSecurityDescriptor(ptr long)
@ stdcall RtlCreateSystemVolumeInformationFolder(ptr)
@ stdcall RtlCreateTagHeap(ptr long str str)
@ stdcall RtlCreateTimer(ptr ptr ptr ptr long long long)
@ stdcall RtlCreateTimerQueue(ptr)
@ stdcall RtlCreateUnicodeString(ptr wstr)
@ stdcall RtlCreateUnicodeStringFromAsciiz(ptr str)
@ stdcall RtlCreateUserProcess(ptr long ptr ptr ptr ptr long ptr ptr ptr)
@ stdcall RtlCreateUserSecurityObject(ptr long ptr ptr long ptr ptr)
@ stdcall RtlCreateUserThread(long ptr long ptr long long ptr ptr ptr ptr)
@ stdcall RtlCustomCPToUnicodeN(ptr wstr long ptr str long)
@ stdcall RtlCutoverTimeToSystemTime(ptr ptr ptr long)
@ stdcall RtlDeNormalizeProcessParams(ptr)
@ stdcall RtlDeactivateActivationContext(long long)
@ stdcall RtlDebugPrintTimes()
@ stdcall RtlDecodePointer(ptr)
@ stdcall RtlDecodeSystemPointer(ptr) 
@ stdcall RtlDecompressBuffer(long ptr long ptr long ptr)
@ stdcall RtlDecompressFragment(long ptr long ptr long long ptr ptr)
@ stdcall RtlDefaultNpAcl(ptr)
@ stdcall RtlDelete(ptr)
@ stdcall RtlDeleteAce(ptr long)
@ stdcall RtlDeleteAtomFromAtomTable(ptr long)
@ stdcall RtlDeleteCriticalSection(ptr)
@ stdcall RtlDeleteElementGenericTable(ptr ptr)
@ stdcall RtlDeleteElementGenericTableAvl(ptr ptr)
@ cdecl -arch=x86_64 RtlDeleteFunctionTable(ptr)
@ stdcall RtlDeleteNoSplay(ptr ptr)
@ stdcall RtlDeleteRegistryValue(long ptr ptr)
@ stdcall RtlDeleteResource(ptr)
@ stdcall RtlDeleteSecurityObject(ptr)
@ stdcall RtlDeleteTimer(ptr ptr ptr)
@ stdcall RtlDeleteTimerQueue(ptr)
@ stdcall RtlDeleteTimerQueueEx(ptr ptr)
@ stdcall RtlDeregisterWait(ptr)
@ stdcall RtlDeregisterWaitEx(ptr ptr)
@ stdcall RtlDestroyAtomTable(ptr)
@ stdcall RtlDestroyEnvironment(ptr)
@ stdcall RtlDestroyHandleTable(ptr)
@ stdcall RtlDestroyHeap(long)
@ stdcall RtlDestroyProcessParameters(ptr)
@ stdcall RtlDestroyQueryDebugBuffer(ptr)
@ stdcall RtlDetermineDosPathNameType_U(wstr)
@ stdcall RtlDllShutdownInProgress()
@ stdcall RtlDnsHostNameToComputerName(ptr ptr long)
@ stdcall RtlDoesFileExists_U(wstr)
@ stdcall RtlDosApplyFileIsolationRedirection_Ustr(long ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall RtlDosPathNameToNtPathName_U(wstr ptr ptr ptr)
@ stdcall RtlDosSearchPath_U(wstr wstr wstr long ptr ptr)
@ stdcall RtlDosSearchPath_Ustr(long ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall RtlDowncaseUnicodeChar(long)
@ stdcall RtlDowncaseUnicodeString(ptr ptr long)
@ stdcall RtlDumpResource(ptr)
@ stdcall RtlDuplicateUnicodeString(long ptr ptr)
@ stdcall RtlEmptyAtomTable(ptr long)
@ stdcall RtlEnableEarlyCriticalSectionEventCreation()
@ stdcall RtlEncodePointer(ptr) 
@ stdcall RtlEncodeSystemPointer(ptr) 
@ stdcall -arch=win32 -ret64 RtlEnlargedIntegerMultiply(long long)
@ stdcall -arch=win32 RtlEnlargedUnsignedDivide(double long ptr)
@ stdcall -arch=win32 -ret64 RtlEnlargedUnsignedMultiply(long long)
@ stdcall RtlEnterCriticalSection(ptr)
@ stdcall RtlEnumProcessHeaps(ptr ptr)
@ stdcall RtlEnumerateGenericTable(ptr long)
@ stdcall RtlEnumerateGenericTableAvl(ptr long)
@ stdcall RtlEnumerateGenericTableLikeADirectory(ptr ptr ptr long ptr ptr ptr)
@ stdcall RtlEnumerateGenericTableWithoutSplaying(ptr ptr)
@ stdcall RtlEnumerateGenericTableWithoutSplayingAvl(ptr ptr)
@ stdcall RtlEqualComputerName(ptr ptr)
@ stdcall RtlEqualDomainName(ptr ptr)
@ stdcall RtlEqualLuid(ptr ptr)
@ stdcall RtlEqualPrefixSid(ptr ptr)
@ stdcall RtlEqualSid(long long)
@ stdcall RtlEqualString(ptr ptr long)
@ stdcall RtlEqualUnicodeString(ptr ptr long)
@ stdcall RtlEraseUnicodeString(ptr)
@ stdcall RtlExitUserThread(long)
@ stdcall RtlExpandEnvironmentStrings_U(ptr ptr ptr ptr)
@ stdcall RtlExtendHeap(ptr long ptr ptr)
@ stdcall -arch=win32 -ret64 RtlExtendedIntegerMultiply(double long)
@ stdcall -arch=win32 -ret64 RtlExtendedLargeIntegerDivide(double long ptr)
@ stdcall -arch=win32 -ret64 RtlExtendedMagicDivide(double double long)
@ stdcall RtlFillMemory(ptr long long)
@ stdcall -arch=i386 RtlFillMemoryUlong(ptr long long)
@ stdcall RtlFindActivationContextSectionGuid(long ptr long ptr ptr)
@ stdcall RtlFindActivationContextSectionString(long ptr long ptr ptr)
@ stdcall RtlFindCharInUnicodeString(long ptr ptr ptr)
@ stdcall RtlFindClearBits(ptr long long)
@ stdcall RtlFindClearBitsAndSet(ptr long long)
@ stdcall RtlFindClearRuns(ptr ptr long long)
@ stdcall RtlFindLastBackwardRunClear(ptr long ptr)
@ stdcall RtlFindLeastSignificantBit(double)
@ stdcall RtlFindLongestRunClear(ptr long)
@ stdcall RtlFindMessage(long long long long ptr)
@ stdcall RtlFindMostSignificantBit(double)
@ stdcall RtlFindNextForwardRunClear(ptr long ptr)
@ stdcall RtlFindSetBits(ptr long long)
@ stdcall RtlFindSetBitsAndClear(ptr long long)
@ stdcall RtlFirstEntrySList(ptr)
@ stdcall RtlFirstFreeAce(ptr ptr)
@ stdcall RtlFlushSecureMemoryCache(ptr ptr)
@ stdcall RtlFormatCurrentUserKeyPath(ptr)
@ stdcall RtlFormatMessage(ptr long long long long ptr ptr long ptr)
@ stdcall RtlFreeAnsiString(long)
@ stdcall RtlFreeHandle(ptr ptr)
@ stdcall RtlFreeHeap(long long long)
@ stdcall RtlFreeOemString(ptr)
@ stdcall RtlFreeSid(long)
@ stdcall RtlFreeThreadActivationContextStack()
@ stdcall RtlFreeUnicodeString(ptr)
@ stdcall RtlFreeUserThreadStack(ptr ptr) ; 4.0 to 5.2 only
@ stdcall RtlGUIDFromString(ptr ptr)
@ stdcall RtlGenerate8dot3Name(ptr ptr long ptr)
@ stdcall RtlGetAce(ptr long ptr)
@ stdcall RtlGetActiveActivationContext(ptr)
@ stdcall RtlGetCallersAddress(ptr ptr)
@ stdcall RtlGetCompressionWorkSpaceSize(long ptr ptr)
@ stdcall RtlGetControlSecurityDescriptor(ptr ptr ptr)
@ stdcall RtlGetCurrentDirectory_U(long ptr)
@ stdcall RtlGetCurrentPeb()
@ stdcall RtlGetDaclSecurityDescriptor(ptr ptr ptr ptr)
@ stdcall RtlGetElementGenericTable(ptr long)
@ stdcall RtlGetElementGenericTableAvl(ptr long)
@ stdcall RtlGetFrame()
@ stdcall RtlGetFullPathName_U(wstr long ptr ptr)
@ stdcall RtlGetFullPathName_UstrEx(ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall RtlGetGroupSecurityDescriptor(ptr ptr ptr)
@ stdcall RtlGetLastNtStatus()
@ stdcall RtlGetLastWin32Error()
@ stdcall RtlGetLengthWithoutLastFullDosOrNtPathElement(long ptr ptr)
; Yes, Microsoft really misspelled this one!
@ stdcall RtlGetLengthWithoutTrailingPathSeperators(long ptr ptr) 
@ stdcall RtlGetLongestNtPathLength()
@ stdcall RtlGetNativeSystemInformation(long long long long) NtQuerySystemInformation
@ stdcall RtlGetNtGlobalFlags()
@ stdcall RtlGetNtProductType(ptr)
@ stdcall RtlGetNtVersionNumbers(ptr ptr ptr)
@ stdcall RtlGetOwnerSecurityDescriptor(ptr ptr ptr)
@ stdcall RtlGetProcessHeaps(long ptr)
@ stdcall RtlGetSaclSecurityDescriptor(ptr ptr ptr ptr)
@ stdcall RtlGetSecurityDescriptorRMControl(ptr ptr)
@ stdcall RtlGetSetBootStatusData(ptr long long ptr long long)
@ stdcall RtlGetThreadErrorMode()
@ stdcall RtlGetUnloadEventTrace()
@ stdcall RtlGetUserInfoHeap(ptr long ptr ptr ptr)
@ stdcall RtlGetVersion(ptr)
@ stdcall RtlHashUnicodeString(ptr long long ptr)
@ stdcall RtlIdentifierAuthoritySid(ptr)
@ stdcall RtlImageDirectoryEntryToData(long long long ptr)
@ stdcall RtlImageNtHeader(long)
@ stdcall RtlImageRvaToSection(ptr long long)
@ stdcall RtlImageRvaToVa(ptr long long ptr)
@ stdcall RtlImpersonateSelf(long)
@ stdcall RtlInitAnsiString(ptr str)
@ stdcall RtlInitAnsiStringEx(ptr str)
@ stdcall RtlInitCodePageTable(ptr ptr)
@ stdcall RtlInitNlsTables(ptr ptr ptr ptr)
@ stdcall RtlInitString(ptr str)
@ stdcall RtlInitUnicodeString(ptr wstr)
@ stdcall RtlInitUnicodeStringEx(ptr wstr)
@ stdcall RtlInitializeAtomPackage(long)
@ stdcall RtlInitializeBitMap(ptr long long)
@ stdcall RtlInitializeContext(ptr ptr ptr ptr ptr)
@ stdcall RtlInitializeCriticalSection(ptr)
@ stdcall RtlInitializeCriticalSectionAndSpinCount(ptr long)
@ stdcall RtlInitializeGenericTable(ptr ptr ptr ptr ptr)
@ stdcall RtlInitializeGenericTableAvl(ptr ptr ptr ptr ptr)
@ stdcall RtlInitializeHandleTable(long long ptr)
@ stdcall RtlInitializeRXact(ptr long ptr)
@ stdcall RtlInitializeResource(ptr)
@ stdcall RtlInitializeSListHead(ptr)
@ stdcall RtlInitializeSid(ptr ptr long)
@ stdcall RtlInsertElementGenericTable(ptr ptr long ptr)
@ stdcall RtlInsertElementGenericTableAvl(ptr ptr long ptr)
@ stdcall -arch=x86_64 RtlInstallFunctionTableCallback(double double long ptr ptr ptr)
@ stdcall RtlInt64ToUnicodeString(double long ptr)
@ stdcall RtlIntegerToChar(long long long ptr)
@ stdcall RtlIntegerToUnicodeString(long long ptr)
@ stdcall -arch=i386,x86_64 RtlInterlockedFlushSList(ptr)
@ stdcall -arch=i386,x86_64 RtlInterlockedPopEntrySList(ptr)
@ stdcall -arch=i386,x86_64 RtlInterlockedPushEntrySList(ptr ptr)
@ stdcall RtlIpv4AddressToStringA(ptr ptr)
@ stdcall RtlIpv4AddressToStringExA(ptr long ptr ptr)
@ stdcall RtlIpv4AddressToStringExW(ptr long ptr ptr)
@ stdcall RtlIpv4AddressToStringW(ptr ptr)
@ stdcall RtlIpv4StringToAddressA(str long ptr ptr)
@ stdcall RtlIpv4StringToAddressExA(str long ptr ptr)
@ stdcall RtlIpv4StringToAddressExW(wstr long ptr ptr)
@ stdcall RtlIpv4StringToAddressW(wstr long ptr ptr)
@ stdcall RtlIpv6AddressToStringA(ptr ptr)
@ stdcall RtlIpv6AddressToStringExA(ptr long long ptr ptr)
@ stdcall RtlIpv6AddressToStringExW(ptr long long ptr ptr)
@ stdcall RtlIpv6AddressToStringW(ptr ptr)
@ stdcall RtlIpv6StringToAddressA(str ptr ptr)
@ stdcall RtlIpv6StringToAddressExA(str ptr ptr ptr)
@ stdcall RtlIpv6StringToAddressExW(wstr ptr ptr ptr)
@ stdcall RtlIpv6StringToAddressW(wstr ptr ptr)
@ stdcall RtlIsActivationContextActive(ptr)
@ stdcall RtlIsDosDeviceName_U(wstr)
@ stdcall RtlIsGenericTableEmpty(ptr)
@ stdcall RtlIsGenericTableEmptyAvl(ptr)
@ stdcall RtlIsNameLegalDOS8Dot3(ptr ptr ptr)
@ stdcall RtlIsTextUnicode(ptr long ptr)
@ stdcall RtlIsThreadWithinLoaderCallout()
@ stdcall RtlIsValidHandle(ptr ptr)
@ stdcall RtlIsValidIndexHandle(ptr long ptr)
@ stdcall -arch=win32 -ret64 RtlLargeIntegerAdd(double double)
@ stdcall -arch=win32 -ret64 RtlLargeIntegerArithmeticShift(double long)
@ stdcall -arch=win32 -ret64 RtlLargeIntegerDivide(double double ptr)
@ stdcall -arch=win32 -ret64 RtlLargeIntegerNegate(double)
@ stdcall -arch=win32 -ret64 RtlLargeIntegerShiftLeft(double long)
@ stdcall -arch=win32 -ret64 RtlLargeIntegerShiftRight(double long)
@ stdcall -arch=win32 -ret64 RtlLargeIntegerSubtract(double double)
@ stdcall RtlLargeIntegerToChar(ptr long long ptr)
@ stdcall RtlLeaveCriticalSection(ptr)
@ stdcall RtlLengthRequiredSid(long)
@ stdcall RtlLengthSecurityDescriptor(ptr)
@ stdcall RtlLengthSid(ptr)
@ stdcall RtlLocalTimeToSystemTime(ptr ptr)
@ stdcall RtlLockBootStatusData(ptr)
@ stdcall RtlLockHeap(long)
@ stdcall RtlLogStackBackTrace()
@ stdcall RtlLookupAtomInAtomTable(ptr wstr ptr)
@ stdcall RtlLookupElementGenericTable(ptr ptr)
@ stdcall RtlLookupElementGenericTableAvl(ptr ptr)
@ stdcall -arch=x86_64 RtlLookupFunctionEntry(long ptr ptr)
@ stdcall RtlMakeSelfRelativeSD(ptr ptr ptr)
@ stdcall RtlMapGenericMask(long ptr)
@ stdcall RtlMapSecurityErrorToNtStatus(long)
@ stdcall RtlMoveMemory(ptr ptr long)
@ stdcall RtlMultiAppendUnicodeStringBuffer(ptr long ptr)
@ stdcall RtlMultiByteToUnicodeN(ptr long ptr ptr long)
@ stdcall RtlMultiByteToUnicodeSize(ptr str long)
@ stdcall RtlNewInstanceSecurityObject(long long ptr ptr ptr ptr ptr long ptr ptr)
@ stdcall RtlNewSecurityGrantedAccess(long ptr ptr ptr ptr ptr)
@ stdcall RtlNewSecurityObject(ptr ptr ptr long ptr ptr)
@ stdcall RtlNewSecurityObjectEx(ptr ptr ptr ptr long long ptr ptr)
@ stdcall RtlNewSecurityObjectWithMultipleInheritance(ptr ptr ptr ptr long long long ptr ptr)
@ stdcall RtlNormalizeProcessParams(ptr)
@ stdcall RtlNtPathNameToDosPathName(ptr ptr ptr ptr) ; CHECKME
@ stdcall RtlNtStatusToDosError(long)
@ stdcall RtlNtStatusToDosErrorNoTeb(long)
@ stdcall RtlNumberGenericTableElements(ptr)
@ stdcall RtlNumberGenericTableElementsAvl(ptr)
@ stdcall RtlNumberOfClearBits(ptr)
@ stdcall RtlNumberOfSetBits(ptr)
@ stdcall RtlOemStringToUnicodeSize(ptr) 
@ stdcall RtlOemStringToUnicodeString(ptr ptr long)
@ stdcall RtlOemToUnicodeN(ptr long ptr ptr long)
@ stdcall RtlOpenCurrentUser(long ptr)
@ stdcall RtlPcToFileHeader(ptr ptr)
@ stdcall RtlPinAtomInAtomTable(ptr long)
@ stdcall RtlPopFrame(ptr)
@ stdcall RtlPrefixString(ptr ptr long)
@ stdcall RtlPrefixUnicodeString(ptr ptr long)
@ stdcall RtlProtectHeap(ptr long)
@ stdcall RtlPushFrame(ptr)
@ stdcall RtlQueryAtomInAtomTable(ptr long ptr ptr ptr ptr)
@ stdcall RtlQueryDepthSList(ptr)
@ stdcall RtlQueryEnvironmentVariable_U(ptr ptr ptr)
@ stdcall RtlQueryHeapInformation(long long ptr long ptr)
@ stdcall RtlQueryInformationAcl(ptr ptr long long)
@ stdcall RtlQueryInformationActivationContext(long long ptr long ptr long ptr)
@ stdcall RtlQueryInformationActiveActivationContext(long ptr long ptr)
@ stdcall RtlQueryProcessBackTraceInformation(ptr)
@ stdcall RtlQueryProcessDebugInformation(long long ptr)
@ stdcall RtlQueryProcessHeapInformation(ptr)
@ stdcall RtlQueryProcessLockInformation(ptr)
@ stdcall RtlQueryRegistryValues(long ptr ptr ptr ptr)
@ stdcall RtlQuerySecurityObject(ptr long ptr long ptr)
@ stdcall RtlQueryTagHeap(ptr long long long ptr)
@ stdcall RtlQueryTimeZoneInformation(ptr)
@ stdcall RtlQueueApcWow64Thread(long ptr long long long) NtQueueApcThread
@ stdcall RtlQueueWorkItem(ptr ptr long)
@ stdcall -register RtlRaiseException(ptr)
@ stdcall RtlRaiseStatus(long)
@ stdcall RtlRandom(ptr)
@ stdcall RtlRandomEx(ptr)
@ stdcall RtlReAllocateHeap(long long ptr long)
@ stdcall RtlRealPredecessor(ptr)
@ stdcall RtlRealSuccessor(ptr)
@ stdcall RtlRegisterSecureMemoryCacheCallback(ptr)
@ stdcall RtlRegisterWait(ptr ptr ptr ptr long long)
@ stdcall RtlReleaseActivationContext(ptr)
@ stdcall RtlReleasePebLock()
@ stdcall RtlReleaseResource(ptr)
@ stdcall RtlRemoteCall(ptr ptr ptr long ptr long long)
@ stdcall RtlRemoveVectoredExceptionHandler(ptr)
@ stdcall RtlResetRtlTranslations(ptr)
@ stdcall -arch=x86_64 RtlRestoreContext(ptr ptr)
@ stdcall RtlRestoreLastWin32Error(long) RtlSetLastWin32Error
@ stdcall RtlRunDecodeUnicodeString(long ptr)
@ stdcall RtlRunEncodeUnicodeString(long ptr)
@ stdcall RtlSecondsSince1970ToTime(long ptr)
@ stdcall RtlSecondsSince1980ToTime(long ptr)
@ stdcall RtlSelfRelativeToAbsoluteSD2(ptr ptr)
@ stdcall RtlSelfRelativeToAbsoluteSD(ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall RtlSetAllBits(ptr)
@ stdcall RtlSetAttributesSecurityDescriptor(ptr long ptr)
@ stdcall RtlSetBits(ptr long long)
@ stdcall RtlSetControlSecurityDescriptor(ptr long long)
@ stdcall RtlSetCriticalSectionSpinCount(ptr long)
@ stdcall RtlSetCurrentDirectory_U(ptr)
@ stdcall RtlSetCurrentEnvironment(wstr ptr)
@ stdcall RtlSetDaclSecurityDescriptor(ptr long ptr long)
@ stdcall RtlSetEnvironmentVariable(ptr ptr ptr)
@ stdcall RtlSetGroupSecurityDescriptor(ptr ptr long)
@ stdcall RtlSetHeapInformation(ptr long ptr ptr)
@ stdcall RtlSetInformationAcl(ptr ptr long long)
@ stdcall RtlSetIoCompletionCallback(long ptr long)
@ stdcall RtlSetLastWin32Error(long)
@ stdcall RtlSetLastWin32ErrorAndNtStatusFromNtStatus(long)
@ stdcall RtlSetOwnerSecurityDescriptor(ptr ptr long)
@ cdecl RtlSetProcessIsCritical(long ptr long)
@ stdcall RtlSetSaclSecurityDescriptor(ptr long ptr long)
@ stdcall RtlSetSecurityDescriptorRMControl(ptr ptr)
@ stdcall RtlSetSecurityObject(long ptr ptr ptr ptr)
@ stdcall RtlSetSecurityObjectEx(long ptr ptr long ptr ptr)
@ stdcall RtlSetThreadErrorMode(long ptr)
@ cdecl RtlSetThreadIsCritical(long ptr long)
@ stdcall RtlSetThreadPoolStartFunc(ptr ptr)
@ stdcall RtlSetTimeZoneInformation(ptr)
@ stdcall RtlSetTimer(ptr ptr ptr ptr long long long) RtlCreateTimer
@ stdcall RtlSetUnicodeCallouts(long)
@ stdcall RtlSetUserFlagsHeap(ptr long ptr long long)
@ stdcall RtlSetUserValueHeap(ptr long ptr ptr)
@ stdcall RtlSizeHeap(long long ptr)
@ stdcall RtlSplay(ptr)
@ stdcall RtlStartRXact(ptr)
@ stdcall RtlStringFromGUID(ptr ptr)
@ stdcall RtlSubAuthorityCountSid(ptr)
@ stdcall RtlSubAuthoritySid(ptr long)
@ stdcall RtlSubtreePredecessor(ptr)
@ stdcall RtlSubtreeSuccessor(ptr)
@ stdcall RtlSystemTimeToLocalTime(ptr ptr)
@ stdcall RtlTimeFieldsToTime(ptr ptr)
@ stdcall RtlTimeToElapsedTimeFields(long long)
@ stdcall RtlTimeToSecondsSince1970(ptr ptr)
@ stdcall RtlTimeToSecondsSince1980(ptr ptr)
@ stdcall RtlTimeToTimeFields (long long)
@ stdcall RtlTraceDatabaseAdd(ptr long ptr ptr)
@ stdcall RtlTraceDatabaseCreate(long long long long ptr)
@ stdcall RtlTraceDatabaseDestroy(ptr)
@ stdcall RtlTraceDatabaseEnumerate(ptr ptr ptr)
@ stdcall RtlTraceDatabaseFind(ptr long ptr ptr)
@ stdcall RtlTraceDatabaseLock(ptr)
@ stdcall RtlTraceDatabaseUnlock(ptr)
@ stdcall RtlTraceDatabaseValidate(ptr)
@ stdcall RtlTryEnterCriticalSection(ptr)
@ stdcall RtlUnhandledExceptionFilter2(long long)
@ stdcall RtlUnhandledExceptionFilter(ptr)
@ stdcall RtlUnicodeStringToAnsiSize(str) 
@ stdcall RtlUnicodeStringToAnsiString(ptr ptr long)
@ stdcall RtlUnicodeStringToCountedOemString(ptr ptr long)
@ stdcall RtlUnicodeStringToInteger(ptr long ptr)
@ stdcall RtlUnicodeStringToOemSize(str) 
@ stdcall RtlUnicodeStringToOemString(ptr ptr long)
@ stdcall RtlUnicodeToCustomCPN(ptr ptr long ptr wstr long)
@ stdcall RtlUnicodeToMultiByteN(ptr long ptr ptr long)
@ stdcall RtlUnicodeToMultiByteSize(ptr ptr long)
@ stdcall RtlUnicodeToOemN(ptr long ptr ptr long)
@ stdcall RtlUniform(ptr)
@ stdcall RtlUnlockBootStatusData(ptr)
@ stdcall RtlUnlockHeap(long)
@ stdcall -register RtlUnwind(ptr ptr ptr ptr)
@ stdcall -arch=x86_64 RtlUnwindEx(long long ptr long ptr)
@ stdcall RtlUpcaseUnicodeChar(long)
@ stdcall RtlUpcaseUnicodeString(ptr ptr long)
@ stdcall RtlUpcaseUnicodeStringToAnsiString(ptr ptr long)
@ stdcall RtlUpcaseUnicodeStringToCountedOemString(ptr ptr long)
@ stdcall RtlUpcaseUnicodeStringToOemString(ptr ptr long)
@ stdcall RtlUpcaseUnicodeToCustomCPN(ptr ptr long ptr wstr long)
@ stdcall RtlUpcaseUnicodeToMultiByteN(ptr long ptr ptr long)
@ stdcall RtlUpcaseUnicodeToOemN(ptr long ptr ptr long)
@ stdcall RtlUpdateTimer(ptr ptr long long)
@ stdcall RtlUpperChar(long)
@ stdcall RtlUpperString(ptr ptr)
@ stdcall RtlUsageHeap(ptr long ptr)
@ stdcall RtlValidAcl(ptr)
@ stdcall RtlValidRelativeSecurityDescriptor(ptr long long)
@ stdcall RtlValidSecurityDescriptor(ptr)
@ stdcall RtlValidSid(ptr)
@ stdcall RtlValidateHeap(long long ptr)
@ stdcall RtlValidateProcessHeaps()
@ stdcall RtlValidateUnicodeString(long ptr)
@ stdcall RtlVerifyVersionInfo(ptr long double)
@ stdcall -arch=x86_64 RtlVirtualUnwind(long long long ptr ptr ptr ptr ptr)
@ stdcall RtlWalkFrameChain(ptr long long)
@ stdcall RtlWalkHeap(long ptr)
@ stdcall RtlWriteRegistryValue(long ptr ptr long ptr long)
@ stdcall RtlZeroHeap(ptr long)
@ stdcall RtlZeroMemory(ptr long)
@ stdcall RtlZombifyActivationContext(ptr)
@ stdcall RtlpApplyLengthFunction(long long ptr ptr)
@ stdcall RtlpEnsureBufferSize(ptr ptr ptr) ; CHECKME
@ stdcall RtlpNotOwnerCriticalSection(ptr)
@ stdcall RtlpNtCreateKey(ptr long ptr long ptr ptr)
@ stdcall RtlpNtEnumerateSubKey(ptr ptr long long)
@ stdcall RtlpNtMakeTemporaryKey(ptr)
@ stdcall RtlpNtOpenKey(ptr long ptr long)
@ stdcall RtlpNtQueryValueKey(ptr ptr ptr ptr long)
@ stdcall RtlpNtSetValueKey(ptr long ptr long)
@ stdcall RtlpUnWaitCriticalSection(ptr)
@ stdcall RtlpWaitForCriticalSection(ptr)
@ stdcall RtlxAnsiStringToUnicodeSize(ptr)
@ stdcall RtlxOemStringToUnicodeSize(ptr)
@ stdcall RtlxUnicodeStringToAnsiSize(ptr)
@ stdcall RtlxUnicodeStringToOemSize(ptr) ; RtlUnicodeStringToOemSize
@ stdcall -ret64 VerSetConditionMask(double long long)
@ stdcall ZwAcceptConnectPort(ptr long ptr long long ptr) NtAcceptConnectPort
@ stdcall ZwAccessCheck(ptr long long ptr ptr ptr ptr ptr) NtAccessCheck
@ stdcall ZwAccessCheckAndAuditAlarm(ptr long ptr ptr ptr long ptr long ptr ptr ptr) NtAccessCheckAndAuditAlarm
@ stdcall ZwAccessCheckByType(ptr ptr ptr long ptr long ptr ptr long ptr ptr) NtAccessCheckByType
@ stdcall ZwAccessCheckByTypeAndAuditAlarm(ptr ptr ptr ptr ptr ptr long long long ptr long ptr long ptr ptr ptr) NtAccessCheckByTypeAndAuditAlarm
@ stdcall ZwAccessCheckByTypeResultList(ptr ptr ptr long ptr long ptr ptr long ptr ptr) NtAccessCheckByTypeResultList
@ stdcall ZwAccessCheckByTypeResultListAndAuditAlarm(ptr ptr ptr ptr ptr ptr long long long ptr long ptr long ptr ptr ptr) NtAccessCheckByTypeResultListAndAuditAlarm
@ stdcall ZwAccessCheckByTypeResultListAndAuditAlarmByHandle(ptr ptr ptr ptr ptr ptr ptr long long long ptr long ptr long ptr ptr ptr) NtAccessCheckByTypeResultListAndAuditAlarmByHandle
@ stdcall ZwAddAtom(ptr long ptr) NtAddAtom
@ stdcall ZwAddBootEntry(ptr long)
@ stdcall ZwAdjustGroupsToken(long long long long long long) NtAdjustGroupsToken
@ stdcall ZwAdjustPrivilegesToken(long long long long long long) NtAdjustPrivilegesToken
@ stdcall ZwAlertResumeThread(long ptr) NtAlertResumeThread
@ stdcall ZwAlertThread(long) NtAlertThread
@ stdcall ZwAllocateLocallyUniqueId(ptr) NtAllocateLocallyUniqueId
@ stdcall ZwAllocateUserPhysicalPages(ptr ptr ptr)
@ stdcall ZwAllocateUuids(ptr ptr ptr ptr) NtAllocateUuids
@ stdcall ZwAllocateVirtualMemory(long ptr ptr ptr long long) NtAllocateVirtualMemory
@ stdcall ZwAreMappedFilesTheSame(ptr ptr) NtAreMappedFilesTheSame
@ stdcall ZwAssignProcessToJobObject(long long) NtAssignProcessToJobObject
@ stdcall ZwCallbackReturn(ptr long long)
@ stdcall ZwCancelDeviceWakeupRequest(ptr)
@ stdcall ZwCancelIoFile(long ptr) NtCancelIoFile
@ stdcall ZwCancelTimer(long ptr) NtCancelTimer
@ stdcall ZwClearEvent(long) NtClearEvent
@ stdcall ZwClose(long) NtClose
@ stdcall ZwCloseObjectAuditAlarm(ptr ptr long)
@ stdcall ZwCompactKeys(long ptr) NtCompactKeys
@ stdcall ZwCompareTokens(ptr ptr ptr) NtCompareTokens
@ stdcall ZwCompleteConnectPort(ptr) NtCompleteConnectPort
@ stdcall ZwCompressKey(ptr) NtCompressKey
@ stdcall ZwConnectPort(ptr ptr ptr ptr ptr ptr ptr ptr) NtConnectPort
@ stdcall ZwContinue(ptr long) NtContinue
@ stdcall ZwCreateDebugObject(ptr long ptr long) NtCreateDebugObject
@ stdcall ZwCreateDirectoryObject(long long long) NtCreateDirectoryObject
@ stdcall ZwCreateEvent(long long long long long) NtCreateEvent
@ stdcall ZwCreateEventPair(ptr long ptr) NtCreateEventPair
@ stdcall ZwCreateFile(ptr long ptr ptr long long long ptr long long ptr) NtCreateFile
@ stdcall ZwCreateIoCompletion(ptr long ptr long) NtCreateIoCompletion
@ stdcall ZwCreateJobObject(ptr long ptr) NtCreateJobObject
@ stdcall ZwCreateJobSet(long ptr long) NtCreateJobSet
@ stdcall ZwCreateKey(ptr long ptr long ptr long long) NtCreateKey
@ stdcall ZwCreateKeyedEvent(ptr long ptr long) NtCreateKeyedEvent
@ stdcall ZwCreateMailslotFile(long long long long long long long long) NtCreateMailslotFile
@ stdcall ZwCreateMutant(ptr long ptr long) NtCreateMutant
@ stdcall ZwCreateNamedPipeFile(ptr long ptr ptr long long long long long long long long long ptr) NtCreateNamedPipeFile
@ stdcall ZwCreatePagingFile(long long long long) NtCreatePagingFile
@ stdcall ZwCreatePort(ptr ptr long long long) NtCreatePort
@ stdcall ZwCreateProcess(ptr long ptr ptr long ptr ptr ptr)
@ stdcall ZwCreateProcessEx(ptr long ptr ptr long ptr ptr ptr long) NtCreateProcessEx
@ stdcall ZwCreateProfile(ptr ptr ptr long long ptr long long long) NtCreateProfile ; CHECKME
@ stdcall ZwCreateSection(ptr long ptr ptr long long long) NtCreateSection
@ stdcall ZwCreateSemaphore(ptr long ptr long long) NtCreateSemaphore
@ stdcall ZwCreateSymbolicLinkObject(ptr long ptr ptr) NtCreateSymbolicLinkObject
@ stdcall ZwCreateThread(ptr long ptr ptr ptr ptr ptr long)
@ stdcall ZwCreateTimer(ptr long ptr long) NtCreateTimer
@ stdcall ZwCreateToken(ptr long ptr long ptr ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall ZwCreateWaitablePort(ptr ptr long long long) NtCreateWaitablePort
@ stdcall ZwDebugActiveProcess(ptr ptr) NtDebugActiveProcess
@ stdcall ZwDebugContinue(ptr ptr long) NtDebugContinue
@ stdcall ZwDelayExecution(long ptr) NtDelayExecution
@ stdcall ZwDeleteAtom(long) NtDeleteAtom
@ stdcall ZwDeleteBootEntry(long) NtDeleteBootEntry
@ stdcall ZwDeleteFile(ptr) NtDeleteFile
@ stdcall ZwDeleteKey(long) NtDeleteKey
@ stdcall ZwDeleteObjectAuditAlarm(ptr ptr long)
@ stdcall ZwDeleteValueKey(long ptr) NtDeleteValueKey
@ stdcall ZwDeviceIoControlFile(long long long long long long long long long long) NtDeviceIoControlFile
@ stdcall ZwDisplayString(ptr) NtDisplayString
@ stdcall ZwDuplicateObject(long long long ptr long long long) NtDuplicateObject
@ stdcall ZwDuplicateToken(long long long long long long) NtDuplicateToken
@ stdcall ZwEnumerateBootEntries(ptr ptr)
@ stdcall ZwEnumerateKey(long long long ptr long ptr) NtEnumerateKey
@ stdcall ZwEnumerateSystemEnvironmentValuesEx(long ptr long) NtEnumerateSystemEnvironmentValuesEx
@ stdcall ZwEnumerateValueKey(long long long ptr long ptr) NtEnumerateValueKey
@ stdcall ZwExtendSection(ptr ptr) NtExtendSection
@ stdcall ZwFilterToken(ptr long ptr ptr ptr ptr) NtFilterToken
@ stdcall ZwFindAtom(ptr long ptr) NtFindAtom
@ stdcall ZwFlushBuffersFile(long ptr) NtFlushBuffersFile
@ stdcall ZwFlushInstructionCache(long ptr long) NtFlushInstructionCache
@ stdcall ZwFlushKey(long) NtFlushKey
@ stdcall ZwFlushVirtualMemory(long ptr ptr long) NtFlushVirtualMemory
@ stdcall ZwFlushWriteBuffer()
@ stdcall ZwFreeUserPhysicalPages(ptr ptr ptr)
@ stdcall ZwFreeVirtualMemory(long ptr ptr long) NtFreeVirtualMemory
@ stdcall ZwFsControlFile(long long long long long long long long long long) NtFsControlFile
@ stdcall ZwGetContextThread(long ptr) NtGetContextThread
@ stdcall ZwGetDevicePowerState(ptr ptr)
@ stdcall ZwGetPlugPlayEvent(long long ptr long)
@ stdcall ZwGetWriteWatch(long long ptr long ptr ptr ptr) NtGetWriteWatch
@ stdcall ZwImpersonateAnonymousToken(ptr)
@ stdcall ZwImpersonateClientOfPort(ptr ptr) NtImpersonateClientOfPort
@ stdcall ZwImpersonateThread(ptr ptr ptr) NtImpersonateThread
@ stdcall ZwInitializeRegistry(long)
@ stdcall ZwInitiatePowerAction(long long long long) NtInitiatePowerAction
@ stdcall ZwIsProcessInJob(long long) NtIsProcessInJob
@ stdcall ZwIsSystemResumeAutomatic()
@ stdcall ZwListenPort(ptr ptr) NtListenPort
@ stdcall ZwLoadDriver(ptr) NtLoadDriver
@ stdcall ZwLoadKey2(ptr ptr long) NtLoadKey2
@ stdcall ZwLoadKey(ptr ptr) NtLoadKey
@ stdcall ZwLockFile(long long ptr ptr ptr ptr ptr ptr long long) NtLockFile
@ stdcall ZwLockProductActivationKeys(ptr ptr) NtLockProductActivationKeys
@ stdcall ZwLockRegistryKey(ptr) NtLockRegistryKey
@ stdcall ZwLockVirtualMemory(long ptr ptr long) NtLockVirtualMemory
@ stdcall ZwMakePermanentObject(ptr) NtMakePermanentObject
@ stdcall ZwMakeTemporaryObject(long) NtMakeTemporaryObject
@ stdcall ZwMapUserPhysicalPages(ptr ptr ptr)
@ stdcall ZwMapUserPhysicalPagesScatter(ptr ptr ptr)
@ stdcall ZwMapViewOfSection(long long ptr long long ptr ptr long long long) NtMapViewOfSection
@ stdcall ZwModifyBootEntry(ptr) NtModifyBootEntry
@ stdcall ZwNotifyChangeDirectoryFile(long long ptr ptr ptr ptr long long long) NtNotifyChangeDirectoryFile
@ stdcall ZwNotifyChangeKey(long long ptr ptr ptr long long ptr long long) NtNotifyChangeKey
@ stdcall ZwNotifyChangeMultipleKeys(ptr long ptr ptr ptr ptr ptr long long ptr long long) NtNotifyChangeMultipleKeys
@ stdcall ZwOpenDirectoryObject(long long long) NtOpenDirectoryObject
@ stdcall ZwOpenEvent(long long long) NtOpenEvent
@ stdcall ZwOpenEventPair(ptr long ptr) NtOpenEventPair
@ stdcall ZwOpenFile(ptr long ptr ptr long long) NtOpenFile
@ stdcall ZwOpenIoCompletion(ptr long ptr) NtOpenIoCompletion
@ stdcall ZwOpenJobObject(ptr long ptr) NtOpenJobObject
@ stdcall ZwOpenKey(ptr long ptr) NtOpenKey
@ stdcall ZwOpenKeyedEvent(ptr long ptr) NtOpenKeyedEvent
@ stdcall ZwOpenMutant(ptr long ptr) NtOpenMutant
@ stdcall ZwOpenObjectAuditAlarm(ptr ptr ptr ptr ptr ptr long long ptr long long ptr)
@ stdcall ZwOpenProcess(ptr long ptr ptr) NtOpenProcess
@ stdcall ZwOpenProcessToken(long long ptr) NtOpenProcessToken
@ stdcall ZwOpenProcessTokenEx(long long long ptr) NtOpenProcessTokenEx
@ stdcall ZwOpenSection(ptr long ptr) NtOpenSection
@ stdcall ZwOpenSemaphore(long long ptr) NtOpenSemaphore
@ stdcall ZwOpenSymbolicLinkObject (ptr long ptr) NtOpenSymbolicLinkObject
@ stdcall ZwOpenThread(ptr long ptr ptr) NtOpenThread
@ stdcall ZwOpenThreadToken(long long long ptr) NtOpenThreadToken
@ stdcall ZwOpenThreadTokenEx(long long long long ptr) NtOpenThreadTokenEx
@ stdcall ZwOpenTimer(ptr long ptr) NtOpenTimer
@ stdcall ZwPlugPlayControl(ptr ptr long)
@ stdcall ZwPowerInformation(long ptr long ptr long) NtPowerInformation
@ stdcall ZwPrivilegeCheck(ptr ptr ptr) NtPrivilegeCheck
@ stdcall ZwPrivilegeObjectAuditAlarm(ptr ptr ptr long ptr long)
@ stdcall ZwPrivilegedServiceAuditAlarm(ptr ptr ptr ptr long)
@ stdcall ZwProtectVirtualMemory(long ptr ptr long ptr) NtProtectVirtualMemory
@ stdcall ZwPulseEvent(long ptr) NtPulseEvent
@ stdcall ZwQueryAttributesFile(ptr ptr) NtQueryAttributesFile
@ stdcall ZwQueryBootEntryOrder(ptr ptr) NtQueryBootEntryOrder
@ stdcall ZwQueryBootOptions(ptr ptr) NtQueryBootOptions
@ stdcall ZwQueryDebugFilterState(long long) NtQueryDebugFilterState
@ stdcall ZwQueryDefaultLocale(long ptr) NtQueryDefaultLocale
@ stdcall ZwQueryDefaultUILanguage(ptr) NtQueryDefaultUILanguage
@ stdcall ZwQueryDirectoryFile(long long ptr ptr ptr ptr long long long ptr long) NtQueryDirectoryFile
@ stdcall ZwQueryDirectoryObject(long ptr long long long ptr ptr) NtQueryDirectoryObject
@ stdcall ZwQueryEaFile(long ptr ptr long long ptr long ptr long) NtQueryEaFile
@ stdcall ZwQueryEvent(long long ptr long ptr) NtQueryEvent
@ stdcall ZwQueryFullAttributesFile(ptr ptr) NtQueryFullAttributesFile
@ stdcall ZwQueryInformationAtom(long long ptr long ptr) NtQueryInformationAtom
@ stdcall ZwQueryInformationFile(long ptr ptr long long) NtQueryInformationFile
@ stdcall ZwQueryInformationJobObject(long long ptr long ptr) NtQueryInformationJobObject
@ stdcall ZwQueryInformationPort(ptr long ptr long ptr) NtQueryInformationPort
@ stdcall ZwQueryInformationProcess(long long ptr long ptr) NtQueryInformationProcess
@ stdcall ZwQueryInformationThread(long long ptr long ptr) NtQueryInformationThread
@ stdcall ZwQueryInformationToken(long long ptr long ptr) NtQueryInformationToken
@ stdcall ZwQueryInstallUILanguage(ptr) NtQueryInstallUILanguage
@ stdcall ZwQueryIntervalProfile(long ptr) NtQueryIntervalProfile
@ stdcall ZwQueryIoCompletion(long long ptr long ptr) NtQueryIoCompletion
@ stdcall ZwQueryKey(long long ptr long ptr) NtQueryKey
@ stdcall ZwQueryMultipleValueKey(long ptr long ptr long ptr) NtQueryMultipleValueKey
@ stdcall ZwQueryMutant(long long ptr long ptr) NtQueryMutant
@ stdcall ZwQueryObject(long long long long long) NtQueryObject
@ stdcall ZwQueryOpenSubKeys(ptr ptr) NtQueryOpenSubKeys
@ stdcall ZwQueryPerformanceCounter (long long) NtQueryPerformanceCounter
@ stdcall ZwQueryPortInformationProcess() NtQueryPortInformationProcess
@ stdcall ZwQueryQuotaInformationFile(ptr ptr ptr long long ptr long ptr long) NtQueryQuotaInformationFile
@ stdcall ZwQuerySection (long long long long long) NtQuerySection
@ stdcall ZwQuerySecurityObject (long long long long long) NtQuerySecurityObject
@ stdcall ZwQuerySemaphore (long long long long long) NtQuerySemaphore
@ stdcall ZwQuerySymbolicLinkObject(long ptr ptr) NtQuerySymbolicLinkObject
@ stdcall ZwQuerySystemEnvironmentValue(ptr ptr long ptr) NtQuerySystemEnvironmentValue
@ stdcall ZwQuerySystemEnvironmentValueEx(ptr ptr ptr ptr ptr) NtQuerySystemEnvironmentValueEx
@ stdcall ZwQuerySystemInformation(long long long long) NtQuerySystemInformation
@ stdcall ZwQuerySystemTime(ptr) NtQuerySystemTime
@ stdcall ZwQueryTimer(ptr long ptr long ptr) NtQueryTimer
@ stdcall ZwQueryTimerResolution(long long long) NtQueryTimerResolution
@ stdcall ZwQueryValueKey(long ptr long ptr long ptr) NtQueryValueKey
@ stdcall ZwQueryVirtualMemory(long ptr long ptr long ptr) NtQueryVirtualMemory
@ stdcall ZwQueryVolumeInformationFile(long ptr ptr long long) NtQueryVolumeInformationFile
@ stdcall ZwQueueApcThread(long ptr long long long) NtQueueApcThread
@ stdcall ZwRaiseException(ptr ptr long) NtRaiseException
@ stdcall ZwRaiseHardError(long long long ptr long ptr) NtRaiseHardError
@ stdcall ZwReadFile(long long ptr ptr ptr ptr long ptr ptr) NtReadFile
@ stdcall ZwReadFileScatter(long long ptr ptr ptr ptr long ptr ptr) NtReadFileScatter
@ stdcall ZwReadRequestData(ptr ptr long ptr long ptr) NtReadRequestData
@ stdcall ZwReadVirtualMemory(long ptr ptr long ptr) NtReadVirtualMemory
@ stdcall ZwRegisterThreadTerminatePort(ptr) NtRegisterThreadTerminatePort
@ stdcall ZwReleaseKeyedEvent(ptr ptr long ptr) NtReleaseKeyedEvent
@ stdcall ZwReleaseMutant(long ptr) NtReleaseMutant
@ stdcall ZwReleaseSemaphore(long long ptr) NtReleaseSemaphore
@ stdcall ZwRemoveIoCompletion(ptr ptr ptr ptr ptr) NtRemoveIoCompletion
@ stdcall ZwRemoveProcessDebug(ptr ptr) NtRemoveProcessDebug
@ stdcall ZwRenameKey(ptr ptr) NtRenameKey
@ stdcall ZwReplaceKey(ptr long ptr) NtReplaceKey
@ stdcall ZwReplyPort(ptr ptr) NtReplyPort
@ stdcall ZwReplyWaitReceivePort(ptr ptr ptr ptr) NtReplyWaitReceivePort
@ stdcall ZwReplyWaitReceivePortEx(ptr ptr ptr ptr ptr)
@ stdcall ZwReplyWaitReplyPort(ptr ptr)
@ stdcall ZwRequestDeviceWakeup(ptr)
@ stdcall ZwRequestPort(ptr ptr)
@ stdcall ZwRequestWaitReplyPort(ptr ptr ptr)
@ stdcall ZwRequestWakeupLatency(long)
@ stdcall ZwResetEvent(long ptr)
@ stdcall ZwResetWriteWatch(long ptr long)
@ stdcall ZwRestoreKey(long long long)
@ stdcall ZwResumeProcess(ptr)
@ stdcall ZwResumeThread(long long)
@ stdcall ZwSaveKey(long long)
@ stdcall ZwSaveKeyEx(ptr ptr long)
@ stdcall ZwSaveMergedKeys(ptr ptr ptr)
@ stdcall ZwSecureConnectPort(ptr ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall ZwSetBootEntryOrder(ptr ptr)
@ stdcall ZwSetBootOptions(ptr long)
@ stdcall ZwSetContextThread(long ptr)
@ stdcall ZwSetDebugFilterState(long long long)
@ stdcall ZwSetDefaultHardErrorPort(ptr)
@ stdcall ZwSetDefaultLocale(long long)
@ stdcall ZwSetDefaultUILanguage(long)
@ stdcall ZwSetEaFile(long ptr ptr long)
@ stdcall ZwSetEvent(long long)
@ stdcall ZwSetEventBoostPriority(ptr)
@ stdcall ZwSetHighEventPair(ptr)
@ stdcall ZwSetHighWaitLowEventPair(ptr)
@ stdcall ZwSetInformationDebugObject(ptr long ptr long ptr)
@ stdcall ZwSetInformationFile(long long long long long)
@ stdcall ZwSetInformationJobObject(long long ptr long)
@ stdcall ZwSetInformationKey(long long ptr long)
@ stdcall ZwSetInformationObject(long long ptr long)
@ stdcall ZwSetInformationProcess(long long long long)
@ stdcall ZwSetInformationThread(long long ptr long)
@ stdcall ZwSetInformationToken(long long ptr long)
@ stdcall ZwSetIntervalProfile(long long)
@ stdcall ZwSetIoCompletion(ptr long ptr long long)
@ stdcall ZwSetLdtEntries(long double long double) ; CHECKME
@ stdcall ZwSetLowEventPair(ptr)
@ stdcall ZwSetLowWaitHighEventPair(ptr)
@ stdcall ZwSetQuotaInformationFile(ptr ptr ptr long)
@ stdcall ZwSetSecurityObject(long long ptr)
@ stdcall ZwSetSystemEnvironmentValue(ptr ptr)
@ stdcall ZwSetSystemEnvironmentValueEx(ptr ptr ptr long long) NtSetSystemEnvironmentValueEx
@ stdcall ZwSetSystemInformation(long ptr long)
@ stdcall ZwSetSystemPowerState(long long long)
@ stdcall ZwSetSystemTime(ptr ptr)
@ stdcall ZwSetThreadExecutionState(long ptr)
@ stdcall ZwSetTimer(long ptr ptr ptr long long ptr)
@ stdcall ZwSetTimerResolution(long long ptr)
@ stdcall ZwSetUuidSeed(ptr)
@ stdcall ZwSetValueKey(long long long long long long)
@ stdcall ZwSetVolumeInformationFile(long ptr ptr long long)
@ stdcall ZwShutdownSystem(long)
@ stdcall ZwSignalAndWaitForSingleObject(long long long ptr)
@ stdcall ZwStartProfile(ptr)
@ stdcall ZwStopProfile(ptr)
@ stdcall ZwSuspendProcess(ptr)
@ stdcall ZwSuspendThread(long ptr)
@ stdcall ZwSystemDebugControl(long ptr long ptr long ptr)
@ stdcall ZwTerminateJobObject(long long)
@ stdcall ZwTerminateProcess(long long)
@ stdcall ZwTerminateThread(long long)
@ stdcall ZwTestAlert()
@ stdcall ZwTraceEvent(long long long ptr)
@ stdcall ZwTranslateFilePath(ptr long ptr long)
@ stdcall ZwUnloadDriver(ptr)
@ stdcall ZwUnloadKey(long)
@ stdcall ZwUnloadKeyEx(ptr ptr)
@ stdcall ZwUnlockFile(long ptr ptr ptr ptr)
@ stdcall ZwUnlockVirtualMemory(long ptr ptr long)
@ stdcall ZwUnmapViewOfSection(long ptr)
@ stdcall ZwVdmControl(long ptr)
@ stdcall ZwWaitForDebugEvent(ptr long ptr ptr)
@ stdcall ZwWaitForKeyedEvent(ptr ptr long ptr)
@ stdcall ZwWaitForMultipleObjects(long ptr long long ptr)
@ stdcall ZwWaitForSingleObject(long long long)
@ stdcall ZwWaitHighEventPair(ptr)
@ stdcall ZwWaitLowEventPair(ptr)
@ stdcall ZwWriteFile(long long ptr ptr ptr ptr long ptr ptr)
@ stdcall ZwWriteFileGather(long long ptr ptr ptr ptr long ptr ptr)
@ stdcall ZwWriteRequestData(ptr ptr long ptr long ptr)
@ stdcall ZwWriteVirtualMemory(long ptr ptr long ptr)
@ stdcall ZwYieldExecution()
@ cdecl -arch=i386 _CIcos()
@ cdecl -arch=i386 _CIlog()
@ cdecl -arch=i386 _CIpow()
@ cdecl -arch=i386 _CIsin()
@ cdecl -arch=i386 _CIsqrt()
@ cdecl __isascii(long)
@ cdecl __iscsym(long)
@ cdecl __iscsymf(long)
@ cdecl __toascii(long)
@ cdecl -arch=i386 -ret64 _alldiv(double double)
@ cdecl -arch=i386 _alldvrm()
@ cdecl -arch=i386 -ret64 _allmul(double double)
@ cdecl -arch=i386 -norelay _alloca_probe()
@ cdecl -arch=i386 -ret64 _allrem(double double)
@ cdecl -arch=i386 _allshl()
@ cdecl -arch=i386 _allshr()
@ cdecl -ret64 _atoi64(str)
@ cdecl -arch=i386 -ret64 _aulldiv(double double)
@ cdecl -arch=i386 _aulldvrm()
@ cdecl -arch=i386 -ret64 _aullrem(double double)
@ cdecl -arch=i386 _aullshr()
@ cdecl -arch=i386 _chkstk()
@ cdecl -arch=i386,x86_64 _fltused()
@ cdecl -arch=i386 -ret64 _ftol()
@ cdecl _i64toa(double ptr long)
@ cdecl _i64tow(double ptr long)
@ cdecl _itoa(long ptr long)
@ cdecl _itow(long ptr long)
@ cdecl _lfind(ptr ptr ptr long ptr)
@ cdecl -arch=x86_64 _local_unwind()
@ cdecl _ltoa(long ptr long)
@ cdecl _ltow(long ptr long)
@ cdecl _memccpy(ptr ptr long long)
@ cdecl _memicmp(str str long)
@ cdecl -arch=x86_64 _setjmp(ptr ptr)
@ cdecl -arch=x86_64 _setjmpex(ptr ptr)
@ varargs _snprintf(ptr long str)
@ varargs _snwprintf(ptr long wstr)
@ cdecl _splitpath(str ptr ptr ptr ptr)
@ cdecl _strcmpi(str str) _stricmp
@ cdecl _stricmp(str str)
@ cdecl _strlwr(str)
@ cdecl _strnicmp(str str long)
@ cdecl _strupr(str)
@ cdecl _tolower(long)
@ cdecl _toupper(long)
@ cdecl _ui64toa(double ptr long)
@ cdecl _ui64tow(double ptr long)
@ cdecl _ultoa(long ptr long)
@ cdecl _ultow(long ptr long)
@ cdecl _vscwprintf(wstr ptr)
@ cdecl _vsnprintf(ptr long str ptr)
@ cdecl _vsnwprintf(ptr long wstr ptr)
@ cdecl _wcsicmp(wstr wstr)
@ cdecl _wcslwr(wstr)
@ cdecl _wcsnicmp(wstr wstr long)
@ cdecl _wcstoui64(ptr ptr long)
@ cdecl _wcsupr(wstr)
@ cdecl _wtoi(wstr)
@ cdecl _wtoi64(wstr)
@ cdecl _wtol(wstr)
@ cdecl abs(long)
@ cdecl -arch=i386,x86_64 atan(double)
@ cdecl atoi(str)
@ cdecl atol(str)
@ cdecl bsearch(ptr ptr long long ptr)
@ cdecl -arch=i386,x86_64 ceil(double)
@ cdecl -arch=i386,x86_64 cos(double)
@ cdecl -arch=i386,x86_64 fabs(double)
@ cdecl -arch=i386,x86_64 floor(double)
@ cdecl isalnum(long)
@ cdecl isalpha(long)
@ cdecl iscntrl(long)
@ cdecl isdigit(long)
@ cdecl isgraph(long)
@ cdecl islower(long)
@ cdecl isprint(long)
@ cdecl ispunct(long)
@ cdecl isspace(long)
@ cdecl isupper(long)
@ cdecl iswalpha(long)
@ cdecl iswctype(long long)
@ cdecl iswdigit(long)
@ cdecl iswlower(long)
@ cdecl iswspace(long)
@ cdecl iswxdigit(long)
@ cdecl isxdigit(long)
@ cdecl labs(long)
@ cdecl -arch=i386,x86_64 log(double)
@ cdecl -arch=x86_64 longjmp(ptr)
@ cdecl mbstowcs(ptr str long)
@ cdecl memchr(ptr long long)
@ cdecl memcmp(ptr ptr long)
@ cdecl memcpy(ptr ptr long) memmove
@ cdecl memmove(ptr ptr long)
@ cdecl memset(ptr long long)
@ cdecl -arch=i386,x86_64 pow(double double)
@ cdecl qsort(ptr long long ptr)
@ cdecl -arch=i386,x86_64 sin(double)
@ varargs sprintf(ptr str)
@ cdecl -arch=i386,x86_64 sqrt(double)
@ varargs sscanf(str str)
@ cdecl strcat(str str)
@ cdecl strchr(str long)
@ cdecl strcmp(str str)
@ cdecl strcpy(ptr str)
@ cdecl strcspn(str str)
@ cdecl strlen(str)
@ cdecl strncat(str str long)
@ cdecl strncmp(str str long)
@ cdecl strncpy(ptr str long)
@ cdecl strpbrk(str str)
@ cdecl strrchr(str long)
@ cdecl strspn(str str)
@ cdecl strstr(str str)
@ cdecl strtol(str ptr long)
@ cdecl strtoul(str ptr long)
@ varargs swprintf(ptr wstr)
@ cdecl -arch=i386,x86_64 tan(double)
@ cdecl tolower(long)
@ cdecl toupper(long)
@ cdecl towlower(long)
@ cdecl towupper(long)
@ stdcall vDbgPrintEx(long long str ptr)
@ stdcall vDbgPrintExWithPrefix(str long long str ptr)
@ cdecl vsprintf(ptr str ptr)
@ cdecl wcscat(wstr wstr)
@ cdecl wcschr(wstr long)
@ cdecl wcscmp(wstr wstr)
@ cdecl wcscpy(ptr wstr)
@ cdecl wcscspn(wstr wstr)
@ cdecl wcslen(wstr)
@ cdecl wcsncat(wstr wstr long)
@ cdecl wcsncmp(wstr wstr long)
@ cdecl wcsncpy(ptr wstr long)
@ cdecl wcspbrk(wstr wstr)
@ cdecl wcsrchr(wstr long)
@ cdecl wcsspn(wstr wstr)
@ cdecl wcsstr(wstr wstr)
@ cdecl wcstol(wstr ptr long)
@ cdecl wcstombs(ptr ptr long)
@ cdecl wcstoul(wstr ptr long)

#Missing on XP and Server 2003 RTM
@ stdcall -arch=i386 KiFastSystemCall() ntdll.KiFastSystemCall
@ stdcall -arch=i386 KiFastSystemCallRet() ntdll.KiFastSystemCallRet
@ stdcall -arch=i386 KiIntSystemCall() ntdll.KiIntSystemCall
@ stdcall NtAddDriverEntry(ptr long) ntdll.NtAddDriverEntry ; 5.2 and higher
@ stdcall NtApphelpCacheControl(long ptr) ntdll.NtApphelpCacheControl
@ stdcall NtDeleteDriverEntry(long) ntdll.NtDeleteDriverEntry
@ stdcall NtEnumerateDriverEntries(ptr ptr) ntdll.NtEnumerateDriverEntries
@ stdcall NtGetCurrentProcessorNumber() ntdll.NtGetCurrentProcessorNumber; 5.2 and higher
@ stdcall NtGetTickCount() ntdll.NtGetTickCount
@ stdcall NtLoadKeyEx(ptr ptr long ptr) 
@ stdcall NtQueryDriverEntryOrder(ptr ptr) ntdll.NtQueryDriverEntryOrder
@ stdcall NtModifyDriverEntry(ptr) ntdll.NtModifyDriverEntry
@ stdcall NtQueryOpenSubKeysEx(ptr long ptr ptr) 
@ stdcall NtSetDriverEntryOrder(ptr ptr) ntdll.NtSetDriverEntryOrder
@ stdcall NtUnloadKey2(ptr long) ntdll.NtUnloadKey2
@ stdcall NtWaitForMultipleObjects32(long ptr long long ptr) ntdll.NtWaitForMultipleObjects32
@ stdcall RtlAcquirePrivilege(ptr long long ptr) ntdll.RtlAcquirePrivilege
@ stdcall RtlAddVectoredContinueHandler(long ptr) ntdll.RtlAddVectoredContinueHandler
@ stdcall RtlAllocateActivationContextStack(ptr) ntdll.RtlAllocateActivationContextStack
@ stdcall RtlCopyMappedMemory(ptr ptr long) ntdll.RtlCopyMappedMemory
@ stdcall RtlDosPathNameToNtPathName_U_WithStatus(wstr ptr ptr ptr) ; 5.2 SP1, and higher
@ stdcall RtlDosPathNameToRelativeNtPathName_U_WithStatus(wstr ptr ptr ptr) 
@ stdcall RtlFormatMessageEx(ptr long long long long ptr ptr long ptr long) ntdll.RtlFormatMessageEx
@ stdcall RtlFreeActivationContextStack(ptr) ntdll.RtlFreeActivationContextStack
@ stdcall RtlGetCriticalSectionRecursionCount(ptr) ntdll.RtlGetCriticalSectionRecursionCount
@ stdcall RtlImageNtHeaderEx(long ptr double ptr)
@ stdcall RtlInsertElementGenericTableFull(ptr ptr long ptr ptr long)
@ stdcall RtlInsertElementGenericTableFullAvl(ptr ptr long ptr ptr long)
@ stdcall -arch=win32 -ret64 RtlInterlockedCompareExchange64(ptr double double)
@ stdcall RtlIsCriticalSectionLocked(ptr) ntdll.RtlIsCriticalSectionLocked
@ stdcall RtlIsCriticalSectionLockedByThread(ptr) 
@ stdcall RtlLookupElementGenericTableFull(ptr ptr ptr ptr)
@ stdcall RtlLookupElementGenericTableFullAvl(ptr ptr ptr ptr)
@ stdcall RtlMultipleAllocateHeap(ptr long long long ptr) ntdll.RtlMultipleAllocateHeap
@ stdcall RtlMultipleFreeHeap(ptr long long ptr) ntdll.RtlMultipleFreeHeap
@ stdcall RtlReleasePrivilege(ptr) ntdll.RtlReleasePrivilege
@ stdcall RtlRemoveVectoredContinueHandler(ptr) ntdll.RtlRemoveVectoredContinueHandler
@ stdcall RtlSetEnvironmentStrings(wstr long) 
@ stdcall ZwAddDriverEntry(ptr long) ntdll.ZwAddDriverEntry
@ stdcall ZwApphelpCacheControl(long ptr) ntdll.ZwApphelpCacheControl
@ stdcall ZwDeleteDriverEntry(long) ntdll.ZwDeleteDriverEntry
@ stdcall ZwEnumerateDriverEntries(ptr ptr) ntdll.ZwEnumerateDriverEntries
@ stdcall ZwGetCurrentProcessorNumber() ntdll.ZwGetCurrentProcessorNumber
@ stdcall ZwLoadKeyEx(ptr ptr long ptr) ntdll.ZwLoadKeyEx
@ stdcall ZwModifyDriverEntry(ptr) ntdll.ZwModifyDriverEntry
@ stdcall ZwQueryDriverEntryOrder(ptr ptr) ntdll.ZwQueryDriverEntryOrder
@ stdcall ZwQueryOpenSubKeysEx(ptr long ptr ptr) ntdll.ZwQueryOpenSubKeysEx
@ stdcall ZwSetDriverEntryOrder(ptr ptr) ntdll.ZwSetDriverEntryOrder
@ stdcall ZwUnloadKey2(ptr long) ntdll.ZwUnloadKey2
@ stdcall ZwWaitForMultipleObjects32(long ptr long long ptr) ntdll.ZwWaitForMultipleObjects32

#Missing XP
@ stdcall RtlDosPathNameToRelativeNtPathName_U(ptr ptr ptr ptr)
@ stdcall RtlReleaseRelativeName(ptr)

#Missing function on Server 2003 RTM
@ stdcall -arch=i386,x86_64 ExpInterlockedPopEntrySListEnd() ntdll.ExpInterlockedPopEntrySListEnd
@ stdcall -arch=i386,x86_64 ExpInterlockedPopEntrySListFault() ntdll.ExpInterlockedPopEntrySListFault
@ stdcall -arch=i386,x86_64 ExpInterlockedPopEntrySListResume() ntdll.ExpInterlockedPopEntrySListResume
@ stdcall RtlGetCurrentProcessorNumber() RtlpGetCurrentProcessorNumber ;Fix compilation error
@ stdcall RtlSetUnhandledExceptionFilter(ptr) ntdll.RtlSetUnhandledExceptionFilter
@ stdcall RtlWow64EnableFsRedirection(long) ;is native on 2003 RTM, but, is better to see
@ stdcall RtlWow64EnableFsRedirectionEx(long ptr) 

#Missing on XP x64 ntdll wow
@ stdcall RtlAddRefMemoryStream(ptr) ntdll.RtlAddRefMemoryStream
@ stdcall RtlCloneMemoryStream(ptr ptr) ntdll.RtlCloneMemoryStream
@ stdcall RtlCommitMemoryStream(ptr long) ntdll.RtlCommitMemoryStream
@ stdcall RtlCopyMemoryStreamTo(ptr ptr int64 ptr ptr) ntdll.RtlCopyMemoryStreamTo
@ stdcall RtlCopyOutOfProcessMemoryStreamTo(ptr ptr int64 ptr ptr) ntdll.RtlCopyOutOfProcessMemoryStreamTo
@ stdcall RtlFinalReleaseOutOfProcessMemoryStream(ptr) ntdll.RtlFinalReleaseOutOfProcessMemoryStream
@ stdcall RtlInitMemoryStream(ptr) ntdll.RtlInitMemoryStream
@ stdcall RtlInitOutOfProcessMemoryStream(ptr) ntdll.RtlInitOutOfProcessMemoryStream
@ stdcall RtlLockMemoryStreamRegion(ptr int64 int64 long) ntdll.RtlLockMemoryStreamRegion
@ stdcall RtlQueryInterfaceMemoryStream(ptr ptr ptr) ntdll.RtlQueryInterfaceMemoryStream
@ stdcall RtlReadMemoryStream(ptr ptr long ptr) ntdll.RtlReadMemoryStream
@ stdcall RtlReadOutOfProcessMemoryStream(ptr ptr long ptr) ntdll.RtlReadOutOfProcessMemoryStream
@ stdcall RtlReleaseMemoryStream(ptr) ntdll.RtlReleaseMemoryStream
@ stdcall RtlRevertMemoryStream(ptr) ntdll.RtlRevertMemoryStream
@ stdcall RtlSeekMemoryStream(ptr int64 long ptr) ntdll.RtlSeekMemoryStream
@ stdcall RtlSetMemoryStreamSize(ptr int64) ntdll.RtlSetMemoryStreamSize
@ stdcall RtlStatMemoryStream(ptr ptr long) ntdll.RtlStatMemoryStream
@ stdcall RtlUnlockMemoryStreamRegion(ptr int64 int64 long) ntdll.RtlUnlockMemoryStreamRegion
@ stdcall RtlWriteMemoryStream(ptr ptr long ptr) ntdll.RtlWriteMemoryStream

#Missing functions on Reactos - need realoc
@ stdcall EtwControlTraceA(int64 str ptr long) 
@ stdcall EtwControlTraceW(int64 wstr ptr long) 
@ stdcall EtwCreateTraceInstanceId(ptr ptr)
@ stdcall EtwEnableTrace(long long long ptr int64)
@ stdcall EtwEnumerateTraceGuids(ptr long ptr) 
@ stdcall EtwFlushTraceA(int64 str ptr)
@ stdcall EtwFlushTraceW(int64 wstr ptr)
@ stdcall EtwQueryAllTracesA(ptr long ptr)
@ stdcall EtwQueryAllTracesW(ptr long ptr) 
@ stdcall EtwQueryTraceA(int64 str ptr) 
@ stdcall EtwQueryTraceW(int64 wstr ptr) 
@ stdcall EtwRegisterTraceGuidsA(long ptr ptr long ptr str str ptr)
@ stdcall EtwRegisterTraceGuidsW(long ptr ptr long ptr wstr wstr ptr)
@ stdcall EtwStartTraceA(long str ptr) 
@ stdcall EtwStartTraceW(long wstr ptr) 
@ stdcall EtwStopTraceA(int64 str ptr) 
@ stdcall EtwStopTraceW(int64 wstr ptr) 
@ stdcall EtwTraceEvent(int64 ptr) 
@ stdcall EtwTraceEventInstance(double ptr ptr ptr) 
@ stdcall EtwTraceMessage(int64 long ptr long) 
@ stdcall EtwTraceMessageVa(double long ptr long ptr) 
@ stdcall EtwUpdateTraceA(int64 str ptr) 
@ stdcall EtwUpdateTraceW(int64 wstr ptr)
@ stdcall EtwGetTraceEnableFlags(int64) 
@ stdcall EtwGetTraceEnableLevel(int64) 
@ stdcall EtwGetTraceLoggerHandle(ptr)
@ stdcall EtwUnregisterTraceGuids(int64)
@ stdcall EtwNotificationRegistrationA(long long long long long)
@ stdcall EtwNotificationRegistrationW(long long long long long) 
@ stdcall EtwpGetTraceBuffer(ptr ptr long long) 
@ stdcall EtwpSetHWConfigFunction(ptr long)
@ stdcall EtwReceiveNotificationsA(long long long long) 
@ stdcall EtwReceiveNotificationsW(long long long long)

#Only on XP/2003 x64
@ cdecl -arch=x86_64 __C_specific_handler(ptr long ptr ptr) ntdll.__C_specific_handler
@ cdecl -arch=x86_64 __misaligned_access() ntdll.__misaligned_access
@ cdecl -arch=x86_64 __chkstk() ntdll.__chkstk
@ stdcall -arch=x86_64 LdrGetKnownDllSectionHandle(wstr long ptr) ntdll.LdrGetKnownDllSectionHandle
@ stdcall -arch=x86_64 LdrProcessInitializationComplete() ntdll.LdrProcessInitializationComplete
@ stdcall -arch=x86_64 RtlLookupFunctionTable(int64 ptr ptr) ntdll.RtlLookupFunctionTable
@ stdcall -arch=x86_64 RtlCopyMemory(ptr ptr long) ntdll.RtlCopyMemory
@ stdcall -arch=x86_64 RtlCopyMemoryNonTemporal(ptr ptr long) ntdll.RtlCopyMemoryNonTemporal
@ stdcall -arch=x86_64 RtlGetFunctionTableListHead() ntdll.RtlGetFunctionTableListHead

#XP x64 syscalls
@ stdcall NtWow64CsrAllocateCaptureBuffer(long long) ntdll.NtWow64CsrAllocateCaptureBuffer
@ stdcall NtWow64CsrAllocateMessagePointer(ptr long ptr) ntdll.NtWow64CsrAllocateMessagePointer
@ stdcall NtWow64CsrCaptureMessageBuffer(ptr ptr long ptr) ntdll.NtWow64CsrCaptureMessageBuffer
@ stdcall NtWow64CsrCaptureMessageString(ptr str long long str) ntdll.NtWow64CsrCaptureMessageString
@ stdcall NtWow64CsrClientCallServer(ptr ptr long long) ntdll.NtWow64CsrClientCallServer
@ stdcall NtWow64CsrClientConnectToServer(wstr long ptr ptr ptr ptr) ntdll.NtWow64CsrClientConnectToServer
@ stdcall NtWow64CsrFreeCaptureBuffer(ptr) ntdll.NtWow64CsrFreeCaptureBuffer
@ stdcall NtWow64CsrGetProcessId() ntdll.NtWow64CsrGetProcessId
@ stdcall NtWow64CsrIdentifyAlertableThread() ntdll.NtWow64CsrIdentifyAlertableThread
@ stdcall NtWow64CsrNewThread() ntdll.NtWow64CsrNewThread
@ stdcall NtWow64CsrSetPriorityClass(ptr ptr) ntdll.NtWow64CsrSetPriorityClass
@ stdcall NtWow64DebuggerCall(long long long long long) ntdll.NtWow64DebuggerCall
@ stdcall NtWow64GetNativeSystemInformation(long ptr long ptr) ntdll.NtWow64GetNativeSystemInformation
@ stdcall NtWow64QueryInformationProcess64(ptr long ptr long ptr) ntdll.NtWow64QueryInformationProcess64
@ stdcall NtWow64QueryVirtualMemory64(ptr ptr ptr long ptr long long ptr) ntdll.NtWow64QueryVirtualMemory64
@ stdcall NtWow64ReadVirtualMemory64(ptr ptr ptr ptr long long ptr) ntdll.NtWow64ReadVirtualMemory64
@ stdcall ZwWow64CsrAllocateCaptureBuffer(long long) ntdll.NtWow64CsrAllocateCaptureBuffer
@ stdcall ZwWow64CsrAllocateMessagePointer(ptr long ptr) ntdll.NtWow64CsrAllocateMessagePointer
@ stdcall ZwWow64CsrCaptureMessageBuffer(ptr ptr long ptr) ntdll.NtWow64CsrCaptureMessageBuffer
@ stdcall ZwWow64CsrCaptureMessageString(ptr str long long str) ntdll.NtWow64CsrCaptureMessageString
@ stdcall ZwWow64CsrClientCallServer(ptr ptr long long) ntdll.NtWow64CsrClientCallServer
@ stdcall ZwWow64CsrClientConnectToServer(wstr long ptr ptr ptr ptr) ntdll.NtWow64CsrClientConnectToServer
@ stdcall ZwWow64CsrFreeCaptureBuffer(ptr) ntdll.NtWow64CsrFreeCaptureBuffer
@ stdcall ZwWow64CsrGetProcessId() ntdll.NtWow64CsrGetProcessId
@ stdcall ZwWow64CsrIdentifyAlertableThread() ntdll.NtWow64CsrIdentifyAlertableThread
@ stdcall ZwWow64CsrNewThread() ntdll.NtWow64CsrNewThread
@ stdcall ZwWow64CsrSetPriorityClass(ptr ptr) ntdll.NtWow64CsrSetPriorityClass
@ stdcall ZwWow64DebuggerCall(long long long long long) ntdll.NtWow64DebuggerCall
@ stdcall ZwWow64GetNativeSystemInformation(long ptr long ptr) ntdll.NtWow64GetNativeSystemInformation
@ stdcall ZwWow64QueryInformationProcess64(ptr long ptr long ptr) ntdll.NtWow64QueryInformationProcess64
@ stdcall ZwWow64QueryVirtualMemory64(ptr ptr ptr long ptr long long ptr) ntdll.NtWow64QueryVirtualMemory64
@ stdcall ZwWow64ReadVirtualMemory64(ptr ptr ptr ptr long long ptr) ntdll.NtWow64ReadVirtualMemory64

#Longhorn Functions
@ stdcall EtwpAddLogHeaderToLogFile(ptr ptr long)
@ stdcall EtwpFinalizeLogFileHeader(ptr)
@ stdcall EtwpFlushUmLogger(long ptr ptr ptr)
@ stdcall EtwpIsProcessExiting()
@ stdcall EtwpQueryUmLogger(long ptr ptr ptr)
@ stdcall EtwpSendWmiKMRequest(ptr long ptr long ptr long ptr ptr)
@ stdcall EtwpStartUmLogger(long ptr ptr ptr)
@ stdcall EtwpStopUmLogger(long ptr ptr ptr)
@ stdcall EtwpUpdateUmLogger(long ptr ptr ptr)
@ stdcall RtlCloseDefinitionIdentityHandle(ptr ptr)
@ stdcall RtlGetDefinitionIdentityAttributeValue(ptr ptr ptr ptr)
@ stdcall RtlInitLUnicodeStringFromNullTerminatedString(ptr ptr)
@ stdcall RtlParseDefinitionIdentity(ptr ptr long)

#Vista Functions
@ stdcall AlpcGetMessageAttribute(ptr long)
@ stdcall AlpcInitializeMessageAttribute(long ptr long ptr)
@ stdcall EtwDeliverDataBlock(long)
@ stdcall EtwEnumerateProcessRegGuids(ptr long long)
@ stdcall EtwEventEnabled(long long ptr)
@ stdcall EtwEventRegister(ptr ptr ptr ptr)
@ stdcall EtwEventUnregister(long long)
@ stdcall EtwEventWrite(int64 ptr long ptr)
@ stdcall EtwEventWriteEndScenario(long long ptr long ptr)
@ stdcall EtwEventWriteStartScenario(long long ptr long ptr)
@ stdcall EtwLogTraceEvent(long ptr)
@ stdcall EtwNotificationRegister(long long long long long)
@ stdcall EtwNotificationUnregister(long long long)
@ stdcall EtwpCreateEtwThread(ptr ptr)
@ stdcall EtwpGetCpuSpeed(long long) 
@ stdcall EtwpIsHeapRangeLoggingEnabled()
@ stdcall EtwpIsPrivateLoggerOn()
@ stdcall EtwProcessPrivateLoggerRequest(ptr)
@ stdcall EtwSendNotification(long long long long long)
@ stdcall EtwReplyNotification(long)
@ stdcall EtwTraceDiagnosticEvent(long long long long long)
@ stdcall LdrLoadAlternateResourceModuleEx(long ptr ptr)
@ stdcall NtAlpcAcceptConnectPort(ptr ptr long ptr ptr ptr ptr ptr long) 
@ stdcall NtAlpcCancelMessage(ptr long ptr)	
@ stdcall NtAlpcConnectPort(ptr ptr ptr ptr long ptr ptr ptr ptr ptr ptr)
@ stdcall NtAlpcCreatePort(ptr ptr ptr)
@ stdcall NtAlpcCreatePortSection(ptr long ptr long ptr ptr) 
@ stdcall NtAlpcCreateSecurityContext(ptr long ptr)
@ stdcall NtAlpcDeletePortSection(ptr long ptr) 
@ stdcall NtAlpcDeleteSectionView(ptr long ptr) 
@ stdcall NtAlpcDeleteSecurityContext(ptr long ptr) 
@ stdcall NtAlpcDisconnectPort(ptr long)
@ stdcall NtAlpcCreateResourceReserve(ptr long long ptr)
@ stdcall NtAlpcImpersonateClientOfPort(ptr ptr ptr)
@ stdcall NtAlpcQueryInformation(ptr ptr ptr long ptr)
@ stdcall NtAlpcSendWaitReceivePort(ptr long ptr ptr ptr ptr ptr ptr)
@ stdcall NtAlpcSetInformation(ptr ptr ptr long)
@ stdcall NtAlpcCreateSectionView(ptr long ptr)
@ stdcall NtCreateMutantEx(ptr long ptr long long)
@ stdcall NtCreateSemaphoreEx(ptr long long str long long)
@ stdcall NtCreateThreadEx(ptr long ptr ptr ptr ptr long ptr long long ptr)
@ stdcall NtCreateIoCompletionEx(ptr long ptr long)
@ stdcall NtQueryLicenseValue(ptr ptr ptr long ptr)
@ stdcall NtRemoveIoCompletionEx(ptr ptr long ptr ptr long)
@ stdcall NtCreateKeyTransacted(ptr long ptr long ptr long ptr ptr)
@ stdcall NtOpenKeyTransacted(ptr long ptr ptr)
@ stdcall NtTraceControl(long ptr long ptr long ptr) 
@ stdcall NtUnmapViewOfSectionEx(ptr ptr long)
@ stdcall RtlGetOSProductName(ptr long) ;maybe need redirect to ntdll from longhorn (pre-reset) dll
@ stdcall RtlGetProductInfo(long long long long ptr) 
@ stdcall RtlAcquireSRWLockExclusive(ptr) 
@ stdcall RtlAcquireSRWLockShared(ptr)
@ stdcall RtlCloseStringHandle(long long)
@ stdcall RtlCreateServiceSid(ptr ptr ptr) 
@ stdcall RtlCultureNameToLCID(ptr ptr) ;this functions already have implementation 
@ stdcall RtlGetCurrentTransaction()
@ stdcall RtlGetFileMUIPath(long wstr wstr ptr wstr ptr ptr)
@ stdcall RtlGetIntegerAtom(wstr ptr)
@ stdcall RtlGetSystemPreferredUILanguages(long ptr wstr ptr)
@ stdcall RtlGetThreadPreferredUILanguages(long ptr ptr ptr)
@ stdcall RtlGetUILanguageInfo(long wstr wstr ptr ptr)
@ stdcall RtlGetUserPreferredUILanguages(long long ptr wstr ptr)
@ stdcall RtlInitializeConditionVariable(ptr)
@ stdcall RtlInitializeSRWLock(ptr) 
@ stdcall RtlInternString(long long) ; to review
@ stdcall RtlLCIDToCultureName(long wstr) ;this functions already have implementation
@ stdcall RtlLcidToLocaleName(long ptr long long)
@ stdcall RtlpQueryDefaultUILanguage(long long)
@ stdcall RtlpSetCurrentUserUILanguage(long)
@ stdcall RtlpSetDefaultUILanguage(long)
@ stdcall RtlRegisterThreadWithCsrss()
@ stdcall RtlReleaseSRWLockExclusive(ptr) 
@ stdcall RtlReleaseSRWLockShared(ptr) 
@ stdcall RtlReleaseStringPointer(ptr long long)
@ stdcall RtlReportErrorOrigination(long str long long)
@ stdcall RtlReportErrorPropagation(long long long long)
@ stdcall RtlRemovePrivileges(ptr ptr long)
@ stdcall RtlRunOnceInitialize(ptr)
@ stdcall RtlRunOnceBeginInitialize(ptr long ptr)
@ stdcall RtlRunOnceComplete(ptr long ptr)
@ stdcall RtlRunOnceExecuteOnce(ptr ptr ptr ptr)
@ stdcall RtlSetCurrentTransaction(ptr)
@ stdcall RtlSetThreadPreferredUILanguages(long wstr ptr)
@ stdcall RtlSleepConditionVariableCS(ptr ptr ptr) 
@ stdcall RtlSleepConditionVariableSRW(ptr ptr ptr long) 
@ stdcall RtlWakeAllConditionVariable(ptr) 
@ stdcall RtlWakeConditionVariable(ptr) 
@ stdcall RtlAddMandatoryAce(ptr long long long long ptr) 
@ stdcall ShipAssert(long long) 
@ stdcall TpAllocPool(ptr ptr)
@ stdcall TpCallbackLeaveCriticalSectionOnCompletion(ptr ptr)
@ stdcall TpReleasePool(ptr)
@ stdcall TpReleaseCleanupGroup(ptr)
@ stdcall TpAllocCleanupGroup(ptr)
@ stdcall TpAllocTimer(ptr ptr ptr ptr)
@ stdcall TpAllocWait(ptr ptr ptr ptr)
@ stdcall TpAllocWork(ptr ptr ptr ptr)
@ stdcall TpPostWork(ptr)
@ stdcall TpSetWait(ptr ptr ptr)
@ stdcall TpSetWaitEx(ptr ptr ptr ptr)
@ stdcall TpSimpleTryPost(ptr ptr ptr)
@ stdcall TpSetPoolMaxThreads(ptr long)
@ stdcall TpSetPoolMinThreads(ptr long)
@ stdcall TpDisassociateCallback(ptr)
@ stdcall TpCallbackMayRunLong(ptr)
@ stdcall TpCallbackUnloadDllOnCompletion(ptr ptr)
@ stdcall TpReleaseCleanupGroupMembers(ptr long ptr)
@ stdcall TpReleaseIoCompletion(ptr)
@ stdcall TpReleaseTimer(ptr)
@ stdcall TpReleaseWait(ptr)
@ stdcall TpReleaseWork(ptr)
@ stdcall TpSetTimer(ptr ptr long long)
@ stdcall TpSetTimerEx(ptr ptr long long)
@ stdcall TpWaitForTimer(ptr long)
@ stdcall TpWaitForWait(ptr long)
@ stdcall TpWaitForWork(ptr long)
@ stdcall TpWaitForIoCompletion(ptr long)
@ stdcall TpIsTimerSet(ptr)
@ stdcall TpCancelAsyncIoOperation(ptr)
@ stdcall TpCallbackReleaseMutexOnCompletion(ptr ptr)
@ stdcall TpCallbackReleaseSemaphoreOnCompletion(ptr ptr long)
@ stdcall TpCallbackSetEventOnCompletion(ptr ptr)
@ stdcall TpStartAsyncIoOperation(ptr)
@ stdcall WinSqmEventEnabled()
@ stdcall WinSqmIsOptedIn()
@ stdcall WinSqmSetDWORD(long long long)
@ stdcall WinSqmEventWrite(long long long)
@ stdcall WinSqmAddToStream(ptr long long ptr)
@ stdcall WinSqmAddToStreamEx(ptr long long long long)
@ stdcall WinSqmEndSession(ptr)
@ stdcall WinSqmIncrementDWORD(long long long)
@ stdcall WinSqmSetIfMaxDWORD(long long long)
@ stdcall WinSqmStartSession(long long long)
@ stdcall ZwQueryLicenseValue(ptr ptr ptr long ptr) NtQueryLicenseValue

#win7
@ stdcall LdrQueryModuleServiceTags(ptr ptr long)
@ stdcall RtlGetCurrentProcessorNumberEx(ptr)
@ stdcall RtlGetSystemTimePrecise()
@ stdcall RtlCompareUnicodeStrings(ptr long ptr long long)
@ stdcall RtlQueryDynamicTimeZoneInformation(ptr)
@ stdcall RtlTryAcquireSRWLockExclusive(ptr)
@ stdcall RtlTryAcquireSRWLockShared(ptr)
@ stdcall NtOpenKeyEx(ptr long ptr long)
@ stdcall ZwOpenKeyEx(ptr long ptr long) NtOpenKeyEx
@ stdcall WinSqmAddToAverageDWORD(long long long)

#win8
@ stdcall LdrAddDllDirectory(wstr)
@ stdcall LdrRemoveDllDirectory(ptr)
@ stdcall LdrResolveDelayLoadsFromDll(ptr str long)
@ stdcall LdrResolveDelayLoadedAPI(ptr ptr ptr ptr ptr ptr)
@ stdcall RtlSetExtendedFeaturesMask(ptr long long)
@ stdcall RtlInitializeExtendedContext(ptr long ptr)
@ stdcall RtlAddResourceAttributeAce(ptr long long long ptr ptr ptr)
@ stdcall RtlCheckTokenCapability(ptr ptr ptr)
@ stdcall RtlCheckTokenMembershipEx(ptr ptr long ptr)
@ stdcall RtlAddIntegrityLabelToBoundaryDescriptor(ptr ptr)
@ stdcall RtlAddSIDToBoundaryDescriptor(ptr ptr)
@ stdcall RtlCreateBoundaryDescriptor(ptr long)
@ stdcall RtlDeleteBoundaryDescriptor(ptr)
@ stdcall RtlDeregisterSecureMemoryCacheCallback(ptr)
@ stdcall RtlDisableThreadProfiling(ptr)
@ stdcall RtlEnableThreadProfiling(ptr long int64 ptr)
@ stdcall RtlLocateLegacyContext(ptr long)
@ stdcall RtlLocateExtendedFeature(long long ptr)
@ stdcall RtlQueryThreadProfiling(ptr ptr)
@ stdcall RtlQueryUnbiasedInterruptTime(ptr)
@ stdcall RtlReadThreadProfilingData(ptr long ptr)
@ stdcall RtlCopyContext(ptr long ptr)
@ stdcall RtlGetExtendedContextLength(long ptr)
@ stdcall RtlGetEnabledExtendedFeatures(long long)
@ stdcall RtlGetExtendedFeaturesMask(ptr)
@ stdcall RtlUnsubscribeWnfStateChangeNotification(ptr)
@ stdcall TpSetPoolStackInformation(ptr ptr)
@ stdcall TpQueryPoolStackInformation(ptr ptr)
@ stdcall WinSqmIsOptedInEx(long)

#Unimplemented
@ stdcall NtCommitTransaction(ptr long)
@ stdcall NtCreatePrivateNamespace(ptr long ptr ptr) 
@ stdcall NtCreateTransaction(ptr long ptr ptr ptr long long long ptr ptr)
@ stdcall NtDeletePrivateNamespace(ptr)
@ stdcall NtFlushProcessWriteBuffers()
@ stdcall NtOpenPrivateNamespace(ptr long ptr ptr)
@ stdcall NtOpenTransaction(ptr long ptr ptr ptr)
@ stdcall NtQuerySystemInformationEx(long ptr long ptr ptr ptr)
@ stdcall NtQueryInformationTransaction(ptr long ptr long ptr)
@ stdcall NtReplacePartitionUnit(wstr wstr long)
@ stdcall NtRollbackTransaction(ptr long)
@ stdcall NtSetInformationTransaction(ptr long ptr long)
@ stdcall NtSetInformationVirtualMemory(ptr long long ptr ptr long)