#undef i386
@ stdcall ActivateActCtx(ptr ptr)
@ stdcall AddAtomA(str)
@ stdcall AddAtomW(wstr)
@ stdcall AddConsoleAliasA(str str str) ;check
@ stdcall AddConsoleAliasW(wstr wstr wstr) ;check
@ stdcall AddLocalAlternateComputerNameA(str ptr)
@ stdcall AddLocalAlternateComputerNameW(wstr ptr)
@ stdcall AddRefActCtx(ptr)
@ stdcall AddVectoredContinueHandler(long ptr) ntdll.RtlAddVectoredContinueHandler
@ stdcall AddVectoredExceptionHandler(long ptr) ntdll.RtlAddVectoredExceptionHandler
@ stdcall AllocConsole()
@ stdcall AllocateUserPhysicalPages(long ptr ptr)
@ stdcall AreFileApisANSI()
@ stdcall AssignProcessToJobObject(ptr ptr)
@ stdcall AttachConsole(long)
@ stdcall BackupRead(ptr ptr long ptr long long ptr)
@ stdcall BackupSeek(ptr long long ptr ptr ptr)
@ stdcall BackupWrite(ptr ptr long ptr long long ptr)
@ stdcall BaseCheckAppcompatCache(long long long ptr)
@ stdcall BaseCleanupAppcompatCacheSupport(ptr)
@ stdcall BaseDumpAppcompatCache()
@ stdcall BaseFlushAppcompatCache()
@ stdcall BaseInitAppcompatCacheSupport()
@ stdcall BaseIsAppcompatInfrastructureDisabled() 
@ stdcall BaseProcessInitPostImport() ; missing in Win 7
@ stdcall BaseUpdateAppcompatCache(long long long)
@ stdcall Beep(long long)
@ stdcall BeginUpdateResourceA(str long)
@ stdcall BeginUpdateResourceW(wstr long)
@ stdcall BindIoCompletionCallback(long ptr long)
@ stdcall BuildCommDCBA(str ptr)
@ stdcall BuildCommDCBAndTimeoutsA(str ptr ptr)
@ stdcall BuildCommDCBAndTimeoutsW(wstr ptr ptr)
@ stdcall BuildCommDCBW(wstr ptr)
@ stdcall CallNamedPipeA(str ptr long ptr long ptr long)
@ stdcall CallNamedPipeW(wstr ptr long ptr long ptr long)
@ stdcall CancelDeviceWakeupRequest(long)
@ stdcall CancelIo(long)
@ stdcall CancelTimerQueueTimer(long long)
@ stdcall CancelWaitableTimer(long)
@ stdcall ChangeTimerQueueTimer(ptr ptr long long)
@ stdcall CheckNameLegalDOS8Dot3A(str str long long long)
@ stdcall CheckNameLegalDOS8Dot3W(wstr str long long long)
@ stdcall CheckRemoteDebuggerPresent(long ptr)
@ stdcall ClearCommBreak(long)
@ stdcall ClearCommError(long ptr ptr)
@ stdcall CloseConsoleHandle(long)
@ stdcall CloseHandle(long)
@ stdcall CloseProfileUserMapping()
@ stdcall CmdBatNotification(long)
@ stdcall CommConfigDialogA(str long ptr)
@ stdcall CommConfigDialogW(wstr long ptr)
@ stdcall CompareFileTime(ptr ptr)
@ stdcall CompareStringA(long long str long str long)
@ stdcall CompareStringW(long long wstr long wstr long)
@ stdcall ConnectNamedPipe(long ptr)
@ stdcall ConsoleMenuControl(long long long)
@ stdcall ContinueDebugEvent(long long long)
@ stdcall ConvertDefaultLocale (long)
@ stdcall ConvertFiberToThread()
@ stdcall ConvertThreadToFiber(ptr)
@ stdcall CopyFileA(str str long)
@ stdcall CopyFileExA (str str ptr ptr ptr long)
@ stdcall CopyFileExW (wstr wstr ptr ptr ptr long)
@ stdcall CopyFileW(wstr wstr long)
@ stdcall CopyLZFile(long long) LZCopy
@ stdcall CreateActCtxA(ptr)
@ stdcall CreateActCtxW(ptr)
@ stdcall CreateConsoleScreenBuffer(long long ptr long ptr)
@ stdcall CreateDirectoryA(str ptr)
@ stdcall CreateDirectoryExA(str str ptr)
@ stdcall CreateDirectoryExW(wstr wstr ptr)
@ stdcall CreateDirectoryW(wstr ptr)
@ stdcall CreateEventA(ptr long long str)
@ stdcall CreateEventW(ptr long long wstr)
@ stdcall CreateFiber(long ptr ptr)
@ stdcall CreateFiberEx(long long long ptr ptr)
@ stdcall CreateFileA(str long long ptr long long long)
@ stdcall CreateFileMappingA(long ptr long long long str)
@ stdcall CreateFileMappingW(long ptr long long long wstr)
@ stdcall CreateFileW(wstr long long ptr long long long)
@ stdcall CreateHardLinkA(str str ptr)
@ stdcall CreateHardLinkW(wstr wstr ptr)
@ stdcall CreateIoCompletionPort(long long long long)
@ stdcall CreateJobObjectA(ptr str)
@ stdcall CreateJobObjectW(ptr wstr)
@ stdcall CreateJobSet(long ptr long)
@ stdcall CreateMailslotA(ptr long long ptr)
@ stdcall CreateMailslotW(ptr long long ptr)
@ stdcall CreateMemoryResourceNotification(long)
@ stdcall CreateMutexA(ptr long str)
@ stdcall CreateMutexW(ptr long wstr)
@ stdcall CreateNamedPipeA(str long long long long long long ptr)
@ stdcall CreateNamedPipeW(wstr long long long long long long ptr)
@ stdcall CreateNlsSecurityDescriptor(ptr long long) ; missing in Win 7
@ stdcall CreatePipe(ptr ptr ptr long)
@ stdcall CreateProcessA(str str ptr ptr long long ptr str ptr ptr)
@ stdcall CreateProcessInternalA(ptr str str ptr ptr long long ptr str ptr ptr long)
@ stdcall CreateProcessInternalW(ptr wstr wstr ptr ptr long long ptr wstr ptr ptr long)
@ stdcall CreateProcessW(wstr wstr ptr ptr long long ptr wstr ptr ptr)
@ stdcall CreateRemoteThread(long ptr long ptr long long ptr)
@ stdcall CreateSemaphoreA(ptr long long str)
@ stdcall CreateSemaphoreW(ptr long long wstr)
@ stdcall -arch=i386 CreateSocketHandle()
@ stdcall CreateTapePartition(long long long long)
@ stdcall CreateThread(ptr long ptr long long ptr)
@ stdcall CreateTimerQueue ()
@ stdcall CreateTimerQueueTimer(ptr long ptr ptr long long long)
@ stdcall CreateToolhelp32Snapshot(long long)
@ stdcall CreateWaitableTimerA(ptr long str) kernelfull.CreateWaitableTimerA
@ stdcall CreateWaitableTimerW(ptr long wstr) kernelfull.CreateWaitableTimerW
@ stdcall DeactivateActCtx(long ptr)
@ stdcall DebugActiveProcess(long)
@ stdcall DebugActiveProcessStop(long)
@ stdcall DebugBreak() ntdll.DbgBreakPoint
@ stdcall DebugBreakProcess(long)
@ stdcall DebugSetProcessKillOnExit(long)
@ stdcall DecodePointer(ptr) 
@ stdcall DecodeSystemPointer(ptr) 
@ stdcall DefineDosDeviceA(long str str)
@ stdcall DefineDosDeviceW(long wstr wstr)
@ stdcall DelayLoadFailureHook(str str)
@ stdcall DeleteAtom(long)
@ stdcall DeleteCriticalSection(ptr) ntdll.RtlDeleteCriticalSection
@ stdcall DeleteFiber(ptr)
@ stdcall DeleteFileA(str)
@ stdcall DeleteFileW(wstr)
@ stdcall DeleteTimerQueue(long)
@ stdcall DeleteTimerQueueEx (long long)
@ stdcall DeleteTimerQueueTimer(long long long)
@ stdcall DeleteVolumeMountPointA(str) ;check
@ stdcall DeleteVolumeMountPointW(wstr) ;check
@ stdcall DeviceIoControl(long long ptr long ptr long ptr ptr)
@ stdcall DisableThreadLibraryCalls(long)
@ stdcall DisconnectNamedPipe(long)
@ stdcall DnsHostnameToComputerNameA(str str ptr)
@ stdcall DnsHostnameToComputerNameW(wstr wstr ptr)
@ stdcall DosDateTimeToFileTime(long long ptr)
@ stdcall DosPathToSessionPathA(long str str)
@ stdcall DosPathToSessionPathW(long wstr wstr)
@ stdcall DuplicateConsoleHandle(long long long long)
@ stdcall DuplicateHandle(long long long ptr long long long)
@ stdcall EncodePointer(ptr) 
@ stdcall EncodeSystemPointer(ptr) 
@ stdcall EndUpdateResourceA(long long)
@ stdcall EndUpdateResourceW(long long)
@ stdcall EnterCriticalSection(ptr) ntdll.RtlEnterCriticalSection
@ stdcall EnumCalendarInfoA(ptr long long long)
@ stdcall EnumCalendarInfoExA(ptr long long long)
@ stdcall EnumCalendarInfoExW(ptr long long long)
@ stdcall EnumCalendarInfoW(ptr long long long)
@ stdcall EnumDateFormatsA(ptr long long)
@ stdcall EnumDateFormatsExA(ptr long long)
@ stdcall EnumDateFormatsExW(ptr long long)
@ stdcall EnumDateFormatsW(ptr long long)
@ stdcall EnumLanguageGroupLocalesA(ptr long long ptr)
@ stdcall EnumLanguageGroupLocalesW(ptr long long ptr)
@ stdcall EnumResourceLanguagesA(long str str ptr long)
@ stdcall EnumResourceLanguagesW(long wstr wstr ptr long)
@ stdcall EnumResourceNamesA(long str ptr long)
@ stdcall EnumResourceNamesW(long wstr ptr long)
@ stdcall EnumResourceTypesA(long ptr long)
@ stdcall EnumResourceTypesW(long ptr long)
@ stdcall EnumSystemCodePagesA(ptr long)
@ stdcall EnumSystemCodePagesW(ptr long)
@ stdcall EnumSystemGeoID(long long ptr)
@ stdcall EnumSystemLanguageGroupsA(ptr long ptr)
@ stdcall EnumSystemLanguageGroupsW(ptr long ptr)
@ stdcall EnumSystemLocalesA(ptr long)
@ stdcall EnumSystemLocalesW(ptr long)
@ stdcall EnumTimeFormatsA(ptr long long)
@ stdcall EnumTimeFormatsW(ptr long long)
@ stdcall EnumUILanguagesA(ptr long long)
@ stdcall EnumUILanguagesW(ptr long long)
@ stdcall EnumerateLocalComputerNamesA(ptr long str ptr)
@ stdcall EnumerateLocalComputerNamesW(ptr long wstr ptr)
@ stdcall EraseTape(ptr long long)
@ stdcall EscapeCommFunction(long long)
@ stdcall ExitProcess(long) ; FIXME: ntdll.RtlExitUserProcess
@ stdcall ExitThread(long) ; FIXME: ntdll.RtlExitUserThread
@ stdcall ExitVDM(long long)
@ stdcall ExpandEnvironmentStringsA(str ptr long)
@ stdcall ExpandEnvironmentStringsW(wstr ptr long)
@ stdcall ExpungeConsoleCommandHistoryA(long) kernelfull.ExpungeConsoleCommandHistoryA
@ stdcall ExpungeConsoleCommandHistoryW(long) kernelfull.ExpungeConsoleCommandHistoryW
@ stdcall FatalAppExitA(long str)
@ stdcall FatalAppExitW(long wstr)
@ stdcall FatalExit(long)
@ stdcall FileTimeToDosDateTime(ptr ptr ptr)
@ stdcall FileTimeToLocalFileTime(ptr ptr)
@ stdcall FileTimeToSystemTime(ptr ptr)
@ stdcall FillConsoleOutputAttribute(long long long long ptr)
@ stdcall FillConsoleOutputCharacterA(long long long long ptr)
@ stdcall FillConsoleOutputCharacterW(long long long long ptr)
@ stdcall FindActCtxSectionGuid(long ptr long ptr ptr)
@ stdcall FindActCtxSectionStringA(long ptr long str ptr)
@ stdcall FindActCtxSectionStringW(long ptr long wstr ptr)
@ stdcall FindAtomA(str)
@ stdcall FindAtomW(wstr)
@ stdcall FindClose(long)
@ stdcall FindCloseChangeNotification(long)
@ stdcall FindFirstChangeNotificationA(str long long)
@ stdcall FindFirstChangeNotificationW(wstr long long)
@ stdcall FindFirstFileA(str ptr)
@ stdcall FindFirstFileExA(str long ptr long ptr long)
@ stdcall FindFirstFileExW(wstr long ptr long ptr long)
@ stdcall FindFirstFileW(wstr ptr)
@ stdcall FindFirstStreamW(wstr ptr ptr long)
@ stdcall FindFirstVolumeA(ptr long)
@ stdcall FindFirstVolumeMountPointA(str ptr long)
@ stdcall FindFirstVolumeMountPointW(wstr ptr long)
@ stdcall FindFirstVolumeW(ptr long)
@ stdcall FindNextChangeNotification(long)
@ stdcall FindNextFileA(long ptr)
@ stdcall FindNextFileW(long ptr)
@ stdcall FindNextVolumeA(long ptr long)
@ stdcall FindNextVolumeMountPointA(long str long)
@ stdcall FindNextVolumeMountPointW(long wstr long)
@ stdcall FindNextVolumeW(long ptr long)
@ stdcall FindResourceA(long str str)
@ stdcall FindResourceExA(long str str long)
@ stdcall FindResourceExW(long wstr wstr long)
@ stdcall FindResourceW(long wstr wstr)
@ stdcall FindVolumeClose(ptr)
@ stdcall FindVolumeMountPointClose(ptr)
@ stdcall FlsAlloc(ptr)
@ stdcall FlsFree(long)
@ stdcall FlsGetValue(long)
@ stdcall FlsSetValue(long ptr)
@ stdcall FlushConsoleInputBuffer(long)
@ stdcall FlushFileBuffers(long)
@ stdcall FlushInstructionCache(long long long)
@ stdcall FlushViewOfFile(ptr long)
@ stdcall FoldStringA(long str long ptr long)
@ stdcall FoldStringW(long wstr long ptr long)
@ stdcall FormatMessageA(long ptr long long ptr long ptr)
@ stdcall FormatMessageW(long ptr long long ptr long ptr)
@ stdcall FreeConsole()
@ stdcall FreeEnvironmentStringsA(ptr)
@ stdcall FreeEnvironmentStringsW(ptr)
@ stdcall FreeLibrary(long)
@ stdcall FreeLibraryAndExitThread(long long)
@ stdcall FreeResource(long)
@ stdcall FreeUserPhysicalPages(long long long)
@ stdcall GenerateConsoleCtrlEvent(long long)
@ stdcall GetACP()
@ stdcall GetAtomNameA(long ptr long)
@ stdcall GetAtomNameW(long ptr long)
@ stdcall GetBinaryType(str ptr) GetBinaryTypeA
@ stdcall GetBinaryTypeA(str ptr)
@ stdcall GetBinaryTypeW(wstr ptr)
@ stdcall GetCPFileNameFromRegistry(long wstr long) ;check missing in Win 7
@ stdcall GetCPInfo(long ptr)
@ stdcall GetCPInfoExA(long long ptr)
@ stdcall GetCPInfoExW(long long ptr)
@ stdcall GetCalendarInfoA(long long long ptr long ptr)
@ stdcall GetCalendarInfoW(long long long ptr long ptr)
@ stdcall GetComPlusPackageInstallStatus()
@ stdcall GetCommConfig(long ptr long)
@ stdcall GetCommMask(long ptr)
@ stdcall GetCommModemStatus(long ptr)
@ stdcall GetCommProperties(long ptr)
@ stdcall GetCommState(long ptr)
@ stdcall GetCommTimeouts(long ptr)
@ stdcall GetCommandLineA()
@ stdcall GetCommandLineW()
@ stdcall GetCompressedFileSizeA(long ptr)
@ stdcall GetCompressedFileSizeW(long ptr)
@ stdcall GetComputerNameA(ptr ptr)
@ stdcall GetComputerNameExA(long ptr ptr)
@ stdcall GetComputerNameExW(long ptr ptr)
@ stdcall GetComputerNameW(ptr ptr)
@ stdcall GetConsoleAliasA(str str long str)
@ stdcall GetConsoleAliasExesA(str long)
@ stdcall GetConsoleAliasExesLengthA()
@ stdcall GetConsoleAliasExesLengthW()
@ stdcall GetConsoleAliasExesW(wstr long)
@ stdcall GetConsoleAliasW(wstr ptr long wstr)
@ stdcall GetConsoleAliasesA(str long str)
@ stdcall GetConsoleAliasesLengthA(str)
@ stdcall GetConsoleAliasesLengthW(wstr)
@ stdcall GetConsoleAliasesW(wstr long wstr)
@ stdcall GetConsoleCP()
@ stdcall GetConsoleCharType(long long ptr)
@ stdcall GetConsoleCommandHistoryA(long long long)
@ stdcall GetConsoleCommandHistoryLengthA(long)
@ stdcall GetConsoleCommandHistoryLengthW(long)
@ stdcall GetConsoleCommandHistoryW(long long long)
@ stdcall GetConsoleCursorInfo(long ptr)
@ stdcall GetConsoleCursorMode(long ptr ptr)
@ stdcall GetConsoleDisplayMode(ptr)
@ stdcall GetConsoleFontInfo(long long long ptr)
@ stdcall GetConsoleFontSize(long long)
@ stdcall GetConsoleHardwareState(long long ptr)
@ stdcall GetConsoleInputExeNameA(long ptr)
@ stdcall GetConsoleInputExeNameW(long ptr)
@ stdcall GetConsoleInputWaitHandle()
@ stdcall GetConsoleKeyboardLayoutNameA(ptr)
@ stdcall GetConsoleKeyboardLayoutNameW(ptr)
@ stdcall GetConsoleMode(long ptr)
@ stdcall GetConsoleNlsMode(long ptr)
@ stdcall GetConsoleOutputCP()
@ stdcall GetConsoleProcessList(ptr long) ; missing in XP SP3
@ stdcall GetConsoleScreenBufferInfo(long ptr)
@ stdcall GetConsoleSelectionInfo(ptr)
@ stdcall GetConsoleTitleA(ptr long)
@ stdcall GetConsoleTitleW(ptr long)
@ stdcall GetConsoleWindow()
@ stdcall GetCurrencyFormatA(long long str ptr str long)
@ stdcall GetCurrencyFormatW(long long str ptr str long)
@ stdcall GetCurrentActCtx(ptr)
@ stdcall GetCurrentConsoleFont(long long ptr)
@ stdcall GetCurrentDirectoryA(long ptr)
@ stdcall GetCurrentDirectoryW(long ptr)
@ stdcall GetCurrentProcess()
@ stdcall GetCurrentProcessId()
@ stdcall GetCurrentProcessorNumber() ntdll.RtlGetCurrentProcessorNumber
@ stdcall GetCurrentThread()
@ stdcall GetCurrentThreadId()
@ stdcall GetDateFormatA(long long ptr str ptr long)
@ stdcall GetDateFormatW(long long ptr wstr ptr long)
@ stdcall GetDefaultCommConfigA(str ptr long)
@ stdcall GetDefaultCommConfigW(wstr ptr long)
@ stdcall GetDefaultSortkeySize(ptr) ; missing in Win 7
@ stdcall GetDevicePowerState(long ptr)
@ stdcall GetDiskFreeSpaceA(str ptr ptr ptr ptr)
@ stdcall GetDiskFreeSpaceExA (str ptr ptr ptr)
@ stdcall GetDiskFreeSpaceExW (wstr ptr ptr ptr)
@ stdcall GetDiskFreeSpaceW(wstr ptr ptr ptr ptr)
@ stdcall GetDllDirectoryA(long ptr)
@ stdcall GetDllDirectoryW(long ptr)
@ stdcall GetDriveTypeA(str)
@ stdcall GetDriveTypeW(wstr)
@ stdcall GetEnvironmentStrings()
@ stdcall GetEnvironmentStringsA() GetEnvironmentStrings
@ stdcall GetEnvironmentStringsW()
@ stdcall GetEnvironmentVariableA(str ptr long)
@ stdcall GetEnvironmentVariableW(wstr ptr long)
@ stdcall GetExitCodeProcess(long ptr)
@ stdcall GetExitCodeThread(long ptr)
@ stdcall GetExpandedNameA(str ptr)
@ stdcall GetExpandedNameW(wstr ptr)
@ stdcall GetFileAttributesA(str)
@ stdcall GetFileAttributesExA(str long ptr)
@ stdcall GetFileAttributesExW(wstr long ptr)
@ stdcall GetFileAttributesW(wstr)
@ stdcall GetFileInformationByHandle(long ptr)
@ stdcall GetFileSize(long ptr)
@ stdcall GetFileSizeEx(long ptr)
@ stdcall GetFileTime(long ptr ptr ptr)
@ stdcall GetFileType(long)
@ stdcall GetFirmwareEnvironmentVariableA(str str ptr long)
@ stdcall GetFirmwareEnvironmentVariableW(wstr wstr ptr long)
@ stdcall GetFullPathNameA(str long ptr ptr)
@ stdcall GetFullPathNameW(wstr long ptr ptr)
@ stdcall GetGeoInfoA(long long ptr long long)
@ stdcall GetGeoInfoW(long long ptr long long)
@ stdcall -arch=i386 GetHandleContext(long) 
@ stdcall GetHandleInformation(long ptr)
@ stdcall GetLargestConsoleWindowSize(long)
@ stdcall GetLastError() ntdll.RtlGetLastWin32Error
@ stdcall GetLinguistLangSize(ptr) ; missing in Win 7
@ stdcall GetLocalTime(ptr)
@ stdcall GetLocaleInfoA(long long ptr long)
@ stdcall GetLocaleInfoW(long long ptr long)
@ stdcall GetLogicalDriveStringsA(long ptr)
@ stdcall GetLogicalDriveStringsW(long ptr)
@ stdcall GetLogicalDrives()
@ stdcall GetLogicalProcessorInformation(ptr ptr)
@ stdcall GetLongPathNameA (str long long)
@ stdcall GetLongPathNameW (wstr long long)
@ stdcall GetMailslotInfo(long ptr ptr ptr ptr)
@ stdcall GetModuleFileNameA(long ptr long)
@ stdcall GetModuleFileNameW(long ptr long)
@ stdcall GetModuleHandleA(str)
@ stdcall GetModuleHandleExA(long ptr ptr)
@ stdcall GetModuleHandleExW(long ptr ptr)
@ stdcall GetModuleHandleW(wstr)
@ stdcall GetNamedPipeHandleStateA(long ptr ptr ptr ptr str long)
@ stdcall GetNamedPipeHandleStateW(long ptr ptr ptr ptr wstr long)
@ stdcall GetNamedPipeInfo(long ptr ptr ptr ptr)
@ stdcall GetNativeSystemInfo(ptr)
@ stdcall GetNextVDMCommand(long)
@ stdcall GetNlsSectionName(long long long str str long) ; missing in Win 7
@ stdcall GetNumaAvailableMemoryNode(long ptr)
@ stdcall GetNumaHighestNodeNumber(ptr)
@ stdcall GetNumaNodeProcessorMask(long ptr)
@ stdcall GetNumaProcessorNode(long ptr)
@ stdcall GetNumberFormatA(long long str ptr ptr long)
@ stdcall GetNumberFormatW(long long wstr ptr ptr long)
@ stdcall GetNumberOfConsoleFonts()
@ stdcall GetNumberOfConsoleInputEvents(long ptr)
@ stdcall GetNumberOfConsoleMouseButtons(ptr)
@ stdcall GetOEMCP()
@ stdcall GetOverlappedResult(long ptr ptr long)
@ stdcall GetPriorityClass(long)
@ stdcall GetPrivateProfileIntA(str str long str)
@ stdcall GetPrivateProfileIntW(wstr wstr long wstr)
@ stdcall GetPrivateProfileSectionA(str ptr long str)
@ stdcall GetPrivateProfileSectionNamesA(ptr long str)
@ stdcall GetPrivateProfileSectionNamesW(ptr long wstr)
@ stdcall GetPrivateProfileSectionW(wstr ptr long wstr)
@ stdcall GetPrivateProfileStringA(str str str ptr long str)
@ stdcall GetPrivateProfileStringW(wstr wstr wstr ptr long wstr)
@ stdcall GetPrivateProfileStructA (str str ptr long str)
@ stdcall GetPrivateProfileStructW(wstr wstr ptr long wstr)
@ stdcall GetProcAddress(long str)
@ stdcall GetProcessAffinityMask(long ptr ptr)
@ stdcall GetProcessHandleCount(long ptr)
@ stdcall GetProcessHeap()
@ stdcall GetProcessHeaps(long ptr)
@ stdcall GetProcessId(long)
@ stdcall GetProcessIoCounters(long ptr)
@ stdcall GetProcessPriorityBoost(long ptr)
@ stdcall GetProcessShutdownParameters(ptr ptr)
@ stdcall GetProcessTimes(long ptr ptr ptr ptr)
@ stdcall GetProcessVersion(long)
@ stdcall GetProcessWorkingSetSize(long ptr ptr)
@ stdcall GetProfileIntA(str str long)
@ stdcall GetProfileIntW(wstr wstr long)
@ stdcall GetProfileSectionA(str ptr long)
@ stdcall GetProfileSectionW(wstr ptr long)
@ stdcall GetProfileStringA(str str str ptr long)
@ stdcall GetProfileStringW(wstr wstr wstr ptr long)
@ stdcall GetQueuedCompletionStatus(long ptr ptr ptr long)
@ stdcall GetShortPathNameA(str ptr long)
@ stdcall GetShortPathNameW(wstr ptr long)
@ stdcall GetStartupInfoA(ptr)
@ stdcall GetStartupInfoW(ptr)
@ stdcall GetStdHandle(long)
@ stdcall GetStringTypeA(long long str long ptr)
@ stdcall GetStringTypeExA(long long str long ptr)
@ stdcall GetStringTypeExW(long long wstr long ptr)
@ stdcall GetStringTypeW(long wstr long ptr)
@ stdcall GetSystemDefaultLCID()
@ stdcall GetSystemDefaultLangID()
@ stdcall GetSystemDefaultUILanguage()
@ stdcall GetSystemDirectoryA(ptr long)
@ stdcall GetSystemDirectoryW(ptr long)
@ stdcall GetSystemInfo(ptr)
@ stdcall GetSystemPowerStatus(ptr)
@ stdcall GetSystemRegistryQuota(ptr ptr)
@ stdcall GetSystemTime(ptr)
@ stdcall GetSystemTimeAdjustment(ptr ptr ptr)
@ stdcall GetSystemTimeAsFileTime(ptr)
@ stdcall GetSystemTimes(ptr ptr ptr)
@ stdcall GetSystemWindowsDirectoryA(ptr long)
@ stdcall GetSystemWindowsDirectoryW(ptr long)
@ stdcall GetSystemWow64DirectoryA(ptr long)
@ stdcall GetSystemWow64DirectoryW(ptr long)
@ stdcall GetTapeParameters(ptr long ptr ptr)
@ stdcall GetTapePosition(ptr long ptr ptr ptr)
@ stdcall GetTapeStatus(ptr)
@ stdcall GetTempFileNameA(str str long ptr)
@ stdcall GetTempFileNameW(wstr wstr long ptr)
@ stdcall GetTempPathA(long ptr)
@ stdcall GetTempPathW(long ptr)
@ stdcall GetThreadContext(long ptr)
@ stdcall GetThreadIOPendingFlag(long ptr)
@ stdcall GetThreadId(ptr)
@ stdcall GetThreadLocale()
@ stdcall GetThreadPriority(long)
@ stdcall GetThreadPriorityBoost(long ptr)
@ stdcall GetThreadSelectorEntry(long long ptr)
@ stdcall GetThreadTimes(long ptr ptr ptr ptr)
@ stdcall GetTickCount()
@ stdcall GetTimeFormatA(long long ptr str ptr long)
@ stdcall GetTimeFormatW(long long ptr wstr ptr long)
@ stdcall GetTimeZoneInformation(ptr)
@ stdcall GetUserDefaultLCID()
@ stdcall GetUserDefaultLangID()
@ stdcall GetUserDefaultUILanguage()
@ stdcall GetUserGeoID(long)
@ stdcall GetVDMCurrentDirectories(long long)
@ stdcall GetVersion()
@ stdcall GetVersionExA(ptr)
@ stdcall GetVersionExW(ptr)
@ stdcall GetVolumeInformationA(str ptr long ptr ptr ptr ptr long)
@ stdcall GetVolumeInformationW(wstr ptr long ptr ptr ptr ptr long)
@ stdcall GetVolumeNameForVolumeMountPointA(str ptr long)
@ stdcall GetVolumeNameForVolumeMountPointW(wstr ptr long)
@ stdcall GetVolumePathNameA(str ptr long)
@ stdcall GetVolumePathNameW(wstr ptr long)
@ stdcall GetVolumePathNamesForVolumeNameA(str str long ptr)
@ stdcall GetVolumePathNamesForVolumeNameW(wstr wstr long ptr)
@ stdcall GetWindowsDirectoryA(ptr long)
@ stdcall GetWindowsDirectoryW(ptr long)
@ stdcall GetWriteWatch(long ptr long ptr ptr ptr)
@ stdcall GlobalAddAtomA(str)
@ stdcall GlobalAddAtomW(wstr)
@ stdcall GlobalAlloc(long long)
@ stdcall GlobalCompact(long)
@ stdcall GlobalDeleteAtom(long)
@ stdcall GlobalFindAtomA(str)
@ stdcall GlobalFindAtomW(wstr)
@ stdcall GlobalFix(long)
@ stdcall GlobalFlags(long)
@ stdcall GlobalFree(long)
@ stdcall GlobalGetAtomNameA(long ptr long)
@ stdcall GlobalGetAtomNameW(long ptr long)
@ stdcall GlobalHandle(ptr)
@ stdcall GlobalLock(long)
@ stdcall GlobalMemoryStatus(ptr)
@ stdcall GlobalMemoryStatusEx(ptr)
@ stdcall GlobalReAlloc(long long long)
@ stdcall GlobalSize(long)
@ stdcall GlobalUnWire(long)
@ stdcall GlobalUnfix(long)
@ stdcall GlobalUnlock(long)
@ stdcall GlobalWire(long)
@ stdcall Heap32First(ptr long long)
@ stdcall Heap32ListFirst(long ptr)
@ stdcall Heap32ListNext(long ptr)
@ stdcall Heap32Next(ptr)
@ stdcall HeapAlloc(long long long) ntdll.RtlAllocateHeap
@ stdcall HeapCompact(long long)
@ stdcall HeapCreate(long long long)
@ stdcall HeapCreateTagsW(long long wstr wstr) ; missing in Win 7
@ stdcall HeapDestroy(long)
@ stdcall HeapExtend(long long ptr long) ; missing in Win 7
@ stdcall HeapFree(long long long) ntdll.RtlFreeHeap
@ stdcall HeapLock(long)
@ stdcall HeapQueryInformation(long long ptr long ptr)
@ stdcall HeapQueryTagW(long long long long ptr) ; missing in Win 7
@ stdcall HeapReAlloc(long long ptr long) ntdll.RtlReAllocateHeap
@ stdcall HeapSetInformation(ptr long ptr long)
@ stdcall HeapSize(long long ptr) ntdll.RtlSizeHeap
@ stdcall HeapSummary(long long ptr)
@ stdcall HeapUnlock(long)
@ stdcall HeapUsage(long long long long ptr) ; missing in Win 7
@ stdcall HeapValidate(long long ptr)
@ stdcall HeapWalk(long ptr)
@ stdcall InitAtomTable(long)
@ stdcall InitializeCriticalSection(ptr)
@ stdcall InitializeCriticalSectionAndSpinCount(ptr long)
@ stdcall InitializeSListHead(ptr) ntdll.RtlInitializeSListHead
@ stdcall -arch=i386 -ret64 InterlockedCompareExchange64(ptr double double) ntdll.RtlInterlockedCompareExchange64
@ stdcall -arch=i386 InterlockedCompareExchange (ptr long long)
@ stdcall -arch=i386 InterlockedDecrement(ptr)
@ stdcall -arch=i386 InterlockedExchange(ptr long)
@ stdcall -arch=i386 InterlockedExchangeAdd(ptr long)
@ stdcall InterlockedFlushSList(ptr) ntdll.RtlInterlockedFlushSList
@ stdcall -arch=i386 InterlockedIncrement(ptr)
@ stdcall InterlockedPopEntrySList(ptr) ntdll.RtlInterlockedPopEntrySList
@ stdcall InterlockedPushEntrySList(ptr ptr) ntdll.RtlInterlockedPushEntrySList
@ stdcall InvalidateConsoleDIBits(long long)
@ stdcall IsBadCodePtr(ptr)
@ stdcall IsBadHugeReadPtr(ptr long)
@ stdcall IsBadHugeWritePtr(ptr long)
@ stdcall IsBadReadPtr(ptr long)
@ stdcall IsBadStringPtrA(ptr long)
@ stdcall IsBadStringPtrW(ptr long)
@ stdcall IsBadWritePtr(ptr long)
@ stdcall IsDBCSLeadByte(long)
@ stdcall IsDBCSLeadByteEx(long long)
@ stdcall IsDebuggerPresent()
@ stdcall IsProcessInJob(long long ptr)
@ stdcall IsProcessorFeaturePresent(long)
@ stdcall IsSystemResumeAutomatic()
@ stdcall IsTimeZoneRedirectionEnabled()
@ stdcall IsValidCodePage(long)
@ stdcall IsValidLanguageGroup(long long)
@ stdcall IsValidLocale(long long)
@ stdcall IsValidUILanguage(long) ; missing in Win 7
@ stdcall IsWow64Process(ptr ptr)
@ stdcall LCMapStringA(long long str long ptr long)
@ stdcall LCMapStringW(long long wstr long ptr long)
@ stdcall LZClose(long)
@ stdcall LZCloseFile(long)
@ stdcall LZCopy(long long)
@ stdcall LZCreateFileW(ptr long long long ptr)
@ stdcall LZDone()
@ stdcall LZInit(long)
@ stdcall LZOpenFileA(str ptr long)
@ stdcall LZOpenFileW(wstr ptr long)
@ stdcall LZRead(long str long)
@ stdcall LZSeek(long long long)
@ stdcall LZStart()
@ stdcall LeaveCriticalSection(ptr) ntdll.RtlLeaveCriticalSection
@ stdcall LoadLibraryA(str)
@ stdcall LoadLibraryExA( str long long)
@ stdcall LoadLibraryExW(wstr long long)
@ stdcall LoadLibraryW(wstr)
@ stdcall LoadModule(str ptr)
@ stdcall LoadResource(long long)
@ stdcall LocalAlloc(long long)
@ stdcall LocalCompact(long)
@ stdcall LocalFileTimeToFileTime(ptr ptr)
@ stdcall LocalFlags(long)
@ stdcall LocalFree(long)
@ stdcall LocalHandle(ptr)
@ stdcall LocalLock(long)
@ stdcall LocalReAlloc(long long long)
@ stdcall LocalShrink(long long)
@ stdcall LocalSize(long)
@ stdcall LocalUnlock(long)
@ stdcall LockFile(long long long long long)
@ stdcall LockFileEx(long long long long long ptr)
@ stdcall LockResource(long)
@ stdcall MapUserPhysicalPages(ptr long ptr)
@ stdcall MapUserPhysicalPagesScatter(ptr long ptr)
@ stdcall MapViewOfFile(long long long long long)
@ stdcall MapViewOfFileEx(long long long long long ptr)
@ stdcall Module32First(long ptr)
@ stdcall Module32FirstW(long ptr)
@ stdcall Module32Next(long ptr)
@ stdcall Module32NextW(long ptr)
@ stdcall MoveFileA(str str)
@ stdcall MoveFileExA(str str long)
@ stdcall MoveFileExW(wstr wstr long)
@ stdcall MoveFileW(wstr wstr)
@ stdcall MoveFileWithProgressA(str str ptr ptr long)
@ stdcall MoveFileWithProgressW(wstr wstr ptr ptr long)
@ stdcall MulDiv(long long long)
@ stdcall MultiByteToWideChar(long long str long ptr long)
@ stdcall NlsConvertIntegerToString(long long long wstr long) ; missing in Win 7
@ stdcall NlsGetCacheUpdateCount()
@ stdcall NlsResetProcessLocale()
@ stdcall OpenConsoleW(wstr long long long)
@ stdcall OpenDataFile(long long) ; missing in Win 7
@ stdcall OpenEventA(long long str)
@ stdcall OpenEventW(long long wstr)
@ stdcall OpenFile(str ptr long)
@ stdcall OpenFileMappingA(long long str)
@ stdcall OpenFileMappingW(long long wstr)
@ stdcall OpenJobObjectA(long long str)
@ stdcall OpenJobObjectW(long long wstr)
@ stdcall OpenMutexA(long long str)
@ stdcall OpenMutexW(long long wstr)
@ stdcall OpenProcess(long long long)
@ stdcall OpenProfileUserMapping()
@ stdcall OpenSemaphoreA(long long str)
@ stdcall OpenSemaphoreW(long long wstr)
@ stdcall OpenThread(long long long)
@ stdcall OpenWaitableTimerA(long long str)
@ stdcall OpenWaitableTimerW(long long wstr)
@ stdcall OutputDebugStringA(str)
@ stdcall OutputDebugStringW(wstr)
@ stdcall PeekConsoleInputA(ptr ptr long ptr)
@ stdcall PeekConsoleInputW(ptr ptr long ptr)
@ stdcall PeekNamedPipe(long ptr long ptr ptr ptr)
@ stdcall PostQueuedCompletionStatus(long long ptr ptr)
@ stdcall PrepareTape(ptr long long)
@ stdcall PrivCopyFileExW(wstr wstr ptr ptr long long)
@ stdcall PrivMoveFileIdentityW(long long long)
@ stdcall Process32First (ptr ptr)
@ stdcall Process32FirstW (ptr ptr)
@ stdcall Process32Next (ptr ptr)
@ stdcall Process32NextW (ptr ptr)
@ stdcall ProcessIdToSessionId(long ptr)
@ stdcall PulseEvent(long)
@ stdcall PurgeComm(long long)
@ stdcall QueryActCtxW(long ptr ptr long ptr long ptr)
@ stdcall QueryDepthSList(ptr) ntdll.RtlQueryDepthSList
@ stdcall QueryDosDeviceA(str ptr long)
@ stdcall QueryDosDeviceW(wstr ptr long)
@ stdcall QueryInformationJobObject(long long ptr long ptr)
@ stdcall QueryMemoryResourceNotification(ptr ptr)
@ stdcall QueryPerformanceCounter(ptr)
@ stdcall QueryPerformanceFrequency(ptr)
@ stdcall QueueUserAPC(ptr long long)
@ stdcall QueueUserWorkItem(ptr ptr long)
@ stdcall RaiseException(long long long ptr)
@ stdcall ReadConsoleA(long ptr long ptr ptr)
@ stdcall ReadConsoleInputA(long ptr long ptr)
@ stdcall ReadConsoleInputExA(long ptr long ptr long)
@ stdcall ReadConsoleInputExW(long ptr long ptr long)
@ stdcall ReadConsoleInputW(long ptr long ptr)
@ stdcall ReadConsoleOutputA(long ptr long long ptr)
@ stdcall ReadConsoleOutputAttribute(long ptr long long ptr)
@ stdcall ReadConsoleOutputCharacterA(long ptr long long ptr)
@ stdcall ReadConsoleOutputCharacterW(long ptr long long ptr)
@ stdcall ReadConsoleOutputW(long ptr long long ptr)
@ stdcall ReadConsoleW(long ptr long ptr ptr)
@ stdcall ReadDirectoryChangesW(long ptr long long long ptr ptr ptr)
@ stdcall ReadFile(long ptr long ptr ptr)
@ stdcall ReadFileEx(long ptr long ptr ptr)
@ stdcall ReadFileScatter(long ptr long ptr ptr)
@ stdcall ReadProcessMemory(long ptr ptr long ptr)
@ stdcall RegisterConsoleIME(ptr ptr)
@ stdcall RegisterConsoleOS2(long)
@ stdcall RegisterConsoleVDM(long long long long long long long long long long long)
@ stdcall RegisterWaitForInputIdle(ptr)
@ stdcall RegisterWaitForSingleObject(ptr long ptr ptr long long)
@ stdcall RegisterWaitForSingleObjectEx(long ptr ptr long long)
@ stdcall RegisterWowBaseHandlers(long)
@ stdcall RegisterWowExec(long)
@ stdcall ReleaseActCtx(ptr)
@ stdcall ReleaseMutex(long)
@ stdcall ReleaseSemaphore(long long ptr)
@ stdcall RemoveDirectoryA(str)
@ stdcall RemoveDirectoryW(wstr)
@ stdcall RemoveLocalAlternateComputerNameA(str long)
@ stdcall RemoveLocalAlternateComputerNameW(wstr long)
@ stdcall RemoveVectoredContinueHandler(ptr) ntdll.RtlRemoveVectoredContinueHandler
@ stdcall RemoveVectoredExceptionHandler(ptr) ntdll.RtlRemoveVectoredExceptionHandler
@ stdcall ReplaceFile(wstr wstr wstr long ptr ptr) ReplaceFileW
@ stdcall ReplaceFileA(str str str long ptr ptr)
@ stdcall ReplaceFileW(wstr wstr wstr long ptr ptr)
@ stdcall RequestDeviceWakeup(long)
@ stdcall RequestWakeupLatency(long)
@ stdcall ResetEvent(long)
@ stdcall ResetWriteWatch(ptr long)
@ stdcall RestoreLastError(long) ntdll.RtlRestoreLastWin32Error
@ stdcall ResumeThread(long)
@ stdcall -register RtlCaptureContext(ptr) ntdll.RtlCaptureContext
@ stdcall RtlCaptureStackBackTrace(long long ptr ptr) ntdll.RtlCaptureStackBackTrace
@ stdcall RtlFillMemory(ptr long long) ntdll.RtlFillMemory
@ stdcall RtlMoveMemory(ptr ptr long) ntdll.RtlMoveMemory
@ stdcall RtlUnwind(ptr ptr ptr long) ntdll.RtlUnwind
@ stdcall RtlZeroMemory(ptr long) ntdll.RtlZeroMemory
@ stdcall ScrollConsoleScreenBufferA(long ptr ptr ptr ptr)
@ stdcall ScrollConsoleScreenBufferW(long ptr ptr ptr ptr)
@ stdcall SearchPathA(str str str long ptr ptr)
@ stdcall SearchPathW(wstr wstr wstr long ptr ptr)
@ stdcall SetCPGlobal(long) ; missing in Win 7
@ stdcall SetCalendarInfoA(long long long str)
@ stdcall SetCalendarInfoW(long long long wstr)
@ stdcall SetClientTimeZoneInformation(ptr)
@ stdcall SetComPlusPackageInstallStatus(ptr)
@ stdcall SetCommBreak(long)
@ stdcall SetCommConfig(long ptr long)
@ stdcall SetCommMask(long ptr)
@ stdcall SetCommState(long ptr)
@ stdcall SetCommTimeouts(long ptr)
@ stdcall SetComputerNameA(str)
@ stdcall SetComputerNameExA(long str)
@ stdcall SetComputerNameExW(long wstr)
@ stdcall SetComputerNameW(wstr)
@ stdcall SetConsoleActiveScreenBuffer(long)
@ stdcall SetConsoleCP(long)
@ stdcall SetConsoleCommandHistoryMode(long) ; missing in win 7
@ stdcall SetConsoleCtrlHandler(ptr long)
@ stdcall SetConsoleCursor(long long)
@ stdcall SetConsoleCursorInfo(long ptr)
@ stdcall SetConsoleCursorMode(long long long)
@ stdcall SetConsoleCursorPosition(long long)
@ stdcall SetConsoleDisplayMode(long long ptr)
@ stdcall SetConsoleFont(long long)
@ stdcall SetConsoleHardwareState(long long long)
@ stdcall SetConsoleIcon(ptr)
@ stdcall SetConsoleInputExeNameA(ptr)
@ stdcall SetConsoleInputExeNameW(ptr)
@ stdcall SetConsoleKeyShortcuts(long long long long)
@ stdcall SetConsoleLocalEUDC(long long long long)
@ stdcall SetConsoleMaximumWindowSize(long long)
@ stdcall SetConsoleMenuClose(long)
@ stdcall SetConsoleMode(long long)
@ stdcall SetConsoleNlsMode(long long)
@ stdcall SetConsoleNumberOfCommandsA(long long)
@ stdcall SetConsoleNumberOfCommandsW(long long)
@ stdcall SetConsoleOS2OemFormat(long)
@ stdcall SetConsoleOutputCP(long)
@ stdcall SetConsolePalette(long long long)
@ stdcall SetConsoleScreenBufferSize(long long)
@ stdcall SetConsoleTextAttribute(long long)
@ stdcall SetConsoleTitleA(str)
@ stdcall SetConsoleTitleW(wstr)
@ stdcall SetConsoleWindowInfo(long long ptr)
@ stdcall SetCriticalSectionSpinCount(ptr long) ntdll.RtlSetCriticalSectionSpinCount
@ stdcall SetCurrentDirectoryA(str)
@ stdcall SetCurrentDirectoryW(wstr)
@ stdcall SetDefaultCommConfigA(str ptr long)
@ stdcall SetDefaultCommConfigW(wstr ptr long)
@ stdcall SetDllDirectoryA(str)
@ stdcall SetDllDirectoryW(wstr)
@ stdcall SetEndOfFile(long)
@ stdcall SetEnvironmentVariableA(str str)
@ stdcall SetEnvironmentVariableW(wstr wstr)
@ stdcall SetErrorMode(long)
@ stdcall SetEvent(long)
@ stdcall SetFileApisToANSI()
@ stdcall SetFileApisToOEM()
@ stdcall SetFileAttributesA(str long)
@ stdcall SetFileAttributesW(wstr long)
@ stdcall SetFileCompletionNotificationModes(ptr str) kernelfull.SetFileCompletionNotificationModes
@ stdcall SetFilePointer(long long ptr long)
@ stdcall SetFilePointerEx(long double ptr long)
@ stdcall SetFileShortNameA(long str)
@ stdcall SetFileShortNameW(long wstr)
@ stdcall SetFileTime(long ptr ptr ptr)
@ stdcall SetFileValidData(long double)
@ stdcall SetFirmwareEnvironmentVariableA(str str ptr long)
@ stdcall SetFirmwareEnvironmentVariableW(wstr wstr ptr long)
@ stdcall -arch=i386 SetHandleContext(long long) ; missing in Win 7 x64
@ stdcall SetHandleCount(long)
@ stdcall SetHandleInformation(long long long)
@ stdcall SetInformationJobObject(long long ptr long)
@ stdcall SetLastConsoleEventActive() ; missing in XP SP3
@ stdcall SetLastError(long) ntdll.RtlSetLastWin32Error
@ stdcall SetLocalPrimaryComputerNameA(long long) ; missing in XP SP3
@ stdcall SetLocalPrimaryComputerNameW(long long) ; missing in XP SP3
@ stdcall SetLocalTime(ptr)
@ stdcall SetLocaleInfoA(long long str)
@ stdcall SetLocaleInfoW(long long wstr)
@ stdcall SetMailslotInfo(long long)
@ stdcall SetMessageWaitingIndicator(ptr long)
@ stdcall SetNamedPipeHandleState(long ptr ptr ptr)
@ stdcall SetPriorityClass(long long)
@ stdcall SetProcessAffinityMask(long long)
@ stdcall SetProcessPriorityBoost(long long)
@ stdcall SetProcessShutdownParameters(long long)
@ stdcall SetProcessWorkingSetSize(long long long)
@ stdcall SetStdHandle(long long)
@ stdcall SetSystemPowerState(long long)
@ stdcall SetSystemTime(ptr)
@ stdcall SetSystemTimeAdjustment(long long)
@ stdcall SetTapeParameters(ptr long ptr)
@ stdcall SetTapePosition(ptr long long long long long)
@ stdcall SetTermsrvAppInstallMode(long)
@ stdcall SetThreadAffinityMask(long long)
@ stdcall SetThreadContext(long ptr)
@ stdcall SetThreadExecutionState(long)
@ stdcall SetThreadIdealProcessor(long long)
@ stdcall SetThreadLocale(long)
@ stdcall SetThreadPriority(long long)
@ stdcall SetThreadPriorityBoost(long long)
@ stdcall SetThreadUILanguage(long)
@ stdcall SetTimeZoneInformation(ptr)
@ stdcall SetTimerQueueTimer(long ptr ptr long long long)
@ stdcall SetUnhandledExceptionFilter(ptr)
@ stdcall SetUserGeoID(long)
@ stdcall SetVDMCurrentDirectories(long long)
@ stdcall SetVolumeLabelA(str str)
@ stdcall SetVolumeLabelW(wstr wstr)
@ stdcall SetVolumeMountPointA(str str)
@ stdcall SetVolumeMountPointW(wstr wstr)
@ stdcall SetWaitableTimer(long ptr long ptr ptr long)
@ stdcall SetupComm(long long long)
@ stdcall ShowConsoleCursor(long long)
@ stdcall SignalObjectAndWait(long long long long)
@ stdcall SizeofResource(long long)
@ stdcall Sleep(long)
@ stdcall SleepEx(long long)
@ stdcall SuspendThread(long)
@ stdcall SwitchToFiber(ptr)
@ stdcall SwitchToThread()
@ stdcall SystemTimeToFileTime(ptr ptr)
@ stdcall SystemTimeToTzSpecificLocalTime (ptr ptr ptr)
@ stdcall TerminateJobObject(long long)
@ stdcall TerminateProcess(long long)
@ stdcall TerminateThread(long long)
@ stdcall TermsrvAppInstallMode()
@ stdcall Thread32First(long ptr)
@ stdcall Thread32Next(long ptr)
@ stdcall TlsAlloc()
@ stdcall TlsFree(long)
@ stdcall TlsGetValue(long)
@ stdcall TlsSetValue(long ptr)
@ stdcall Toolhelp32ReadProcessMemory(long ptr ptr long ptr)
@ stdcall TransactNamedPipe(long ptr long ptr long ptr ptr)
@ stdcall TransmitCommChar(long long)
@ stdcall TryEnterCriticalSection(ptr) ntdll.RtlTryEnterCriticalSection
@ stdcall TzSpecificLocalTimeToSystemTime(ptr ptr ptr)
@ stdcall UTRegister(long str str str ptr ptr ptr)
@ stdcall UTUnRegister(long)
@ stdcall UnhandledExceptionFilter(ptr)
@ stdcall UnlockFile(long long long long long)
@ stdcall UnlockFileEx(long long long long ptr)
@ stdcall UnmapViewOfFile(ptr)
@ stdcall UnregisterConsoleIME()
@ stdcall UnregisterWait(long)
@ stdcall UnregisterWaitEx(long long)
@ stdcall UpdateResourceA(long str str long ptr long)
@ stdcall UpdateResourceW(long wstr wstr long ptr long)
@ stdcall VDMConsoleOperation(long long)
@ stdcall VDMOperationStarted(long)
@ stdcall ValidateLCType(long long ptr ptr)
@ stdcall ValidateLocale(long)
@ stdcall VerLanguageNameA(long str long)
@ stdcall VerLanguageNameW(long wstr long)
@ stdcall VerSetConditionMask(long long long long) ntdll.VerSetConditionMask
@ stdcall VerifyConsoleIoHandle(long)
@ stdcall VerifyVersionInfoA(long long double)
@ stdcall VerifyVersionInfoW(long long double)
@ stdcall VirtualAlloc(ptr long long long)
@ stdcall VirtualAllocEx(long ptr long long long)
@ stdcall VirtualFree(ptr long long)
@ stdcall VirtualFreeEx(long ptr long long)
@ stdcall VirtualLock(ptr long)
@ stdcall VirtualProtect(ptr long long ptr)
@ stdcall VirtualProtectEx(long ptr long long ptr)
@ stdcall VirtualQuery(ptr ptr long)
@ stdcall VirtualQueryEx(long ptr ptr long)
@ stdcall VirtualUnlock(ptr long)
@ stdcall WaitCommEvent(long ptr ptr)
@ stdcall WaitForDebugEvent(ptr long)
@ stdcall WaitForMultipleObjects(long ptr long long)
@ stdcall WaitForMultipleObjectsEx(long ptr long long long)
@ stdcall WaitForSingleObject(long long)
@ stdcall WaitForSingleObjectEx(long long long)
@ stdcall WaitNamedPipeA (str long)
@ stdcall WaitNamedPipeW (wstr long)
@ stdcall WideCharToMultiByte(long long wstr long ptr long ptr ptr)
@ stdcall WinExec(str long)
@ stdcall WriteConsoleA(long ptr long ptr ptr)
@ stdcall WriteConsoleInputA(long ptr long ptr)
@ stdcall WriteConsoleInputVDMA(long long long long)
@ stdcall WriteConsoleInputVDMW(long long long long)
@ stdcall WriteConsoleInputW(long ptr long ptr)
@ stdcall WriteConsoleOutputA(long ptr long long ptr)
@ stdcall WriteConsoleOutputAttribute(long ptr long long ptr)
@ stdcall WriteConsoleOutputCharacterA(long ptr long long ptr)
@ stdcall WriteConsoleOutputCharacterW(long ptr long long ptr)
@ stdcall WriteConsoleOutputW(long ptr long long ptr)
@ stdcall WriteConsoleW(long ptr long ptr ptr)
@ stdcall WriteFile(long ptr long ptr ptr)
@ stdcall WriteFileEx(long ptr long ptr ptr)
@ stdcall WriteFileGather(long ptr long ptr ptr)
@ stdcall WritePrivateProfileSectionA(str str str)
@ stdcall WritePrivateProfileSectionW(wstr wstr wstr)
@ stdcall WritePrivateProfileStringA(str str str str)
@ stdcall WritePrivateProfileStringW(wstr wstr wstr wstr)
@ stdcall WritePrivateProfileStructA (str str ptr long str)
@ stdcall WritePrivateProfileStructW(wstr wstr ptr long wstr)
@ stdcall WriteProcessMemory(long ptr ptr long ptr)
@ stdcall WriteProfileSectionA(str str)
@ stdcall WriteProfileSectionW(str str)
@ stdcall WriteProfileStringA(str str str)
@ stdcall WriteProfileStringW(wstr wstr wstr)
@ stdcall WriteTapemark(ptr long long long)
@ stdcall WTSGetActiveConsoleSessionId()
@ stdcall ZombifyActCtx(ptr)
@ stdcall _hread(long ptr long)
@ stdcall _hwrite(long ptr long)
@ stdcall _lclose(long)
@ stdcall _lcreat(str long)
@ stdcall _llseek(long long long)
@ stdcall _lopen(str long)
@ stdcall _lread(long ptr long) _hread
@ stdcall _lwrite(long ptr long) _hwrite
@ stdcall lstrcat(str str) lstrcatA
@ stdcall lstrcatA(str str)
@ stdcall lstrcatW(wstr wstr)
@ stdcall lstrcmp(str str) lstrcmpA
@ stdcall lstrcmpA(str str)
@ stdcall lstrcmpW(wstr wstr)
@ stdcall lstrcmpi(str str) lstrcmpiA
@ stdcall lstrcmpiA(str str)
@ stdcall lstrcmpiW(wstr wstr)
@ stdcall lstrcpy(ptr str) lstrcpyA
@ stdcall lstrcpyA(ptr str)
@ stdcall lstrcpyW(ptr wstr)
@ stdcall lstrcpyn(ptr str long) lstrcpynA
@ stdcall lstrcpynA(ptr str long)
@ stdcall lstrcpynW(ptr wstr long)
@ stdcall lstrlen(str) 
@ stdcall lstrlenA(str) lstrlen
@ stdcall lstrlenW(wstr)

#Functions needed for Kernel32 for Wow 
@ stdcall -arch=i386 BaseProcessStartThunk(ptr) 
@ stdcall -arch=i386 BaseThreadStartThunk(ptr ptr)
@ stdcall -arch=i386 ConsoleIMERoutine() kernelfull.ConsoleIMERoutine ;Make this function
@ stdcall -arch=i386 CtrlRoutine() kernelfull.CtrlRoutine

#Missing on Server 2003 RTM (only available on 2003 SP1 and SP2)
@ stdcall BaseCheckRunApp(long ptr long long long long long long long long) kernelfull.BaseCheckRunApp
@ stdcall BasepCheckBadapp(long ptr long long long long long long long) kernelfull.BasepCheckBadapp
@ stdcall BasepCheckWinSaferRestrictions(long long long long long long) kernelfull.BasepCheckWinSaferRestrictions
@ stdcall BasepFreeAppCompatData(ptr ptr) ;kernelfull.BasepFreeAppCompatData
@ stdcall BaseQueryModuleData(str str ptr ptr ptr) kernelfull.BaseQueryModuleData
@ stdcall EnumSystemFirmwareTables(long ptr long) kernelfull.EnumSystemFirmwareTables
@ stdcall GetSystemFileCacheSize(ptr ptr ptr) kernelfull.GetSystemFileCacheSize
@ stdcall GetSystemFirmwareTable(long long ptr long) kernelfull.GetSystemFirmwareTable ;(Need implement for XP/2003 RTM)
@ stdcall SetSystemFileCacheSize(long long long) 
@ stdcall SetThreadStackGuarantee(ptr) 
@ stdcall Wow64DisableWow64FsRedirection(ptr) 
@ stdcall Wow64RevertWow64FsRedirection(ptr) 

#Needed functions for Server 2003 RTM
@ stdcall CreateVirtualBuffer(ptr long long) kernelfull.CreateVirtualBuffer
@ stdcall ExtendVirtualBuffer(ptr) kernelfull.ExtendVirtualBuffer
@ stdcall FreeVirtualBuffer(ptr) kernelfull.FreeVirtualBuffer
@ stdcall TrimVirtualBuffer(ptr) kernelfull.TrimVirtualBuffer
@ stdcall VirtualBufferExceptionHandler(long ptr ptr) kernelfull.VirtualBufferExceptionHandler

#Only for Windows XP
@ stdcall BaseInitAppcompatCache() kernelfull.BaseInitAppcompatCache
@ stdcall BaseCleanupAppcompatCache() kernelfull.BaseCleanupAppcompatCache
@ stdcall CreateProcessInternalWSecure() 
@ stdcall QueryWin31IniFilesMappedToRegistry(long wstr long ptr) kernelfull.QueryWin31IniFilesMappedToRegistry
@ stdcall GetNumaAvailableMemory(ptr long ptr) 
@ stdcall GetNumaProcessorMap(ptr long ptr) kernelfull.GetNumaProcessorMap
@ stdcall NumaVirtualQueryNode(long long long long) kernelfull.NumaVirtualQueryNode

#Missing on XP and avaliable for Server 2003
@ stdcall ConvertThreadToFiberEx(ptr long) kernelfull.ConvertThreadToFiberEx
@ stdcall FindNextStreamW(ptr ptr) ;kernelfull.FindNextStreamW ;onwer implementation 
@ stdcall GetLargePageMinimum() ;kernelfull.GetLargePageMinimum ;onwer implementation 
@ stdcall GetNLSVersion(long long ptr) kernelfull.GetNLSVersion
@ stdcall GetProcessIdOfThread(ptr) ;kernelfull.GetProcessIdOfThread ;onwer implementation 
@ stdcall GetProcessWorkingSetSizeEx(long ptr ptr long) ;kernelfull.GetProcessWorkingSetSizeEx ;onwer implementation 
@ stdcall IsNLSDefinedString(long long ptr long long) kernelfull.IsNLSDefinedString
@ stdcall NeedCurrentDirectoryForExePathA(str) ;kernelfull.NeedCurrentDirectoryForExePathA ;onwer implementation 
@ stdcall NeedCurrentDirectoryForExePathW(wstr) ;kernelfull.NeedCurrentDirectoryForExePathW ;onwer implementation 
@ stdcall ReOpenFile(ptr long long long) ;kernelfull.ReOpenFile ;onwer implementation 
@ stdcall SetEnvironmentStringsA(ptr) ;kernelfull.SetEnvironmentStringsA ;onwer implementation 
@ stdcall SetEnvironmentStringsW(ptr) ;kernelfull.SetEnvironmentStringsW ;onwer implementation 
@ stdcall SetProcessWorkingSetSizeEx(long long long long) ;kernelfull.SetProcessWorkingSetSizeEx ;onwer implementation  
@ stdcall Wow64EnableWow64FsRedirection(long) 

#Needed funcions for XP x64
@ stdcall -arch=x86_64 BaseProcessStart(ptr)
@ stdcall -arch=x86_64 BaseThreadStart(ptr ptr)
@ stdcall -arch=x86_64 ConsoleIMERoutine() ;Make this function
@ stdcall -arch=x86_64 CtrlRoutine() 
@ stdcall -arch=x86_64 __C_specific_handler(ptr long ptr ptr) ntdll.__C_specific_handler
@ stdcall -arch=x86_64 __chkstk() ntdll.__chkstk
@ stdcall -arch=x86_64 _local_unwind(ptr ptr) ntdll._local_unwind
@ stdcall -arch=x86_64 __misaligned_access() ntdll.__misaligned_access
@ stdcall -arch=x86_64 uaw_lstrcmpiW(wstr wstr)
@ stdcall -arch=x86_64 uaw_lstrcmpW(wstr wstr)
@ stdcall -arch=x86_64 uaw_lstrlenW(wstr)
@ stdcall -arch=x86_64 uaw_wcschr(wstr long)
@ stdcall -arch=x86_64 uaw_wcscpy(wstr wstr)
@ stdcall -arch=x86_64 uaw_wcsicmp(wstr wstr)
@ stdcall -arch=x86_64 uaw_wcslen(wstr)
@ stdcall -arch=x86_64 uaw_wcsrchr(wstr long)
@ stdcall -arch=x86_64 RtlAddFunctionTable(ptr long long) ntdll.RtlAddFunctionTable
@ stdcall -arch=x86_64 RtlCompareMemory(ptr ptr ptr)
@ stdcall -arch=x86_64 RtlCopyMemory(ptr ptr ptr)
@ stdcall -arch=x86_64 RtlDeleteFunctionTable(ptr)
@ stdcall -arch=x86_64 RtlInstallFunctionTableCallback(double double long ptr ptr ptr)
@ stdcall -arch=x86_64 RtlLookupFunctionEntry(ptr ptr ptr) ntdll.RtlLookupFunctionEntry
@ stdcall -arch=x86_64 RtlPcToFileHeader(ptr ptr) ntdll.RtlPcToFileHeader
@ stdcall -arch=x86_64 RtlRaiseException(ptr) ntdll.RtlRaiseException
@ stdcall -arch=x86_64 RtlRestoreContext(ptr ptr) ntdll.RtlRestoreContext
@ stdcall -arch=x86_64 RtlUnwindEx(ptr ptr ptr ptr ptr ptr) ntdll.RtlUnwindEx
@ stdcall -arch=x86_64 RtlVirtualUnwind(ptr ptr ptr long) ntdll.RtlVirtualUnwind

; #Windows Vista/7/8 Functions
@ stdcall ActivateActCtxWorker(ptr ptr) ActivateActCtx
@ stdcall AddRefActCtxWorker(ptr) AddRefActCtx
@ stdcall AddIntegrityLabelToBoundaryDescriptor(ptr ptr)
@ stdcall AddResourceAttributeAce(ptr long long long ptr ptr ptr)
@ stdcall AddScopedPolicyIDAce(ptr long long long ptr)
@ stdcall AddSecureMemoryCacheCallback(ptr)
@ stdcall AddSIDToBoundaryDescriptor(ptr ptr)
@ stdcall AllocateUserPhysicalPagesNuma(ptr ptr ptr long)
@ stdcall ApplicationRecoveryFinished(long)
@ stdcall ApplicationRecoveryInProgress(ptr)
@ stdcall AcquireSRWLockExclusive(ptr) vtdll.RtlAcquireSRWLockExclusive
@ stdcall AcquireSRWLockShared(ptr) vtdll.RtlAcquireSRWLockShared
@ stdcall AdjustCalendarDate(ptr long long)  
@ stdcall BaseCheckAppcompatCacheWorker(long long long ptr) BaseCheckAppcompatCache
@ stdcall BaseCheckElevation(wstr long ptr long long) CheckElevation
@ stdcall BaseElevationPostProcessing(long long ptr)
@ stdcall BaseCleanupAppcompatCacheSupportWorker(ptr) BaseCleanupAppcompatCacheSupport
@ stdcall BaseDestroyVDMEnvironment(str wstr)
@ stdcall BaseDllReadWriteIniFile(long long long long long long long long)
@ stdcall BaseDumpAppcompatCacheWorker() BaseDumpAppcompatCache
@ stdcall BaseFlushAppcompatCacheWorker() BaseFlushAppcompatCache
@ stdcall BaseFormatObjectAttributes(ptr ptr wstr) 
@ stdcall BaseFormatTimeOut(int64 long)
@ stub BaseGenerateAppCompatData
@ stdcall BaseGetNamedObjectDirectory()
@ stdcall BaseInitAppcompatCacheSupportWorker() BaseInitAppcompatCacheSupport
@ stdcall BaseIsAppcompatInfrastructureDisabledWorker() BaseIsAppcompatInfrastructureDisabled
@ stdcall BaseIsDosApplication(wstr long)
@ stdcall Basep8BitStringToDynamicUnicodeString(wstr str)
@ stdcall BasepAllocateActivationContextActivationBlock(long ptr ptr ptr)
@ stdcall BasepAnsiStringToDynamicUnicodeString(wstr str) 
@ stdcall BasepCheckWebBladeHashes(ptr)
@ stub BasepConstructSxsCreateProcessMessage
@ stdcall BasepFreeActivationContextActivationBlock(ptr)
@ stub BasepGetAppCompatData
@ stdcall BasepGetExeArchType(long ptr wstr)
@ stdcall BasepIsProcessAllowed(wstr)
@ stdcall BasepMapModuleHandle(ptr long)
@ stub BasepProcessInvalidImage
@ stub BasepQueryAppCompat
@ stdcall BasepReportFault(long long)
@ stdcall BaseSetLastNTError(long) 
@ stub BaseThreadInitThunk
@ stdcall BaseUpdateVDMEntry(long ptr long long)
@ stdcall BaseUpdateAppcompatCacheWorker(long long long) BaseUpdateAppcompatCache
@ stub BaseVerifyUnicodeString
@ stdcall BaseWriteErrorElevationRequiredEvent()
@ stdcall CallbackMayRunLong() vtdll.TpCallbackMayRunLong
@ stdcall CancelIoEx(long ptr) 
@ stdcall CancelSynchronousIo(ptr)
@ stdcall CancelThreadpoolIo(ptr) vtdll.TpCancelAsyncIoOperation
@ stdcall DisableThreadProfiling(ptr)
@ stdcall CeipIsOptedIn()
@ stdcall CheckAllowDecryptedRemoteDestinationPolicy()
@ stdcall CheckElevation(wstr long ptr long long)
@ stdcall CheckElevationEnabled(ptr) 
@ stdcall CheckForReadOnlyResource(long long)
@ stdcall CheckForReadOnlyResourceFilter(long)
@ stdcall CheckTokenCapability(ptr ptr ptr)
@ stdcall CheckTokenMembershipEx(ptr ptr long ptr)
@ stdcall ClosePrivateNamespace(ptr long)
@ stdcall CloseThreadpool(ptr) 
@ stdcall CloseThreadpoolCleanupGroup(ptr) 
@ stdcall CloseThreadpoolCleanupGroupMembers(ptr long ptr) 
@ stdcall CloseThreadpoolIo(ptr) 
@ stdcall CloseThreadpoolTimer(ptr) 
@ stdcall CloseThreadpoolWait(ptr) 
@ stdcall CloseThreadpoolWork(ptr) 
@ stdcall CompareCalendarDates(ptr ptr long)
@ stdcall CompareStringEx(wstr long wstr long wstr long ptr ptr long)
@ stdcall CompareStringOrdinal(wstr long wstr long long)
@ stdcall ConvertNLSDayOfWeekToWin32DayOfWeek(long) 
@ stdcall ConvertSystemTimeToCalDateTime(ptr long ptr) 
@ stdcall ConvertCalDateTimeToSystemTime(ptr ptr) 
@ stdcall CopyContext(ptr long ptr)
@ stdcall CopyFile2(wstr wstr ptr)
@ stdcall CopyFileTransactedA(str str ptr ptr ptr long ptr)
@ stdcall CopyFileTransactedW(wstr wstr ptr ptr ptr long ptr)
@ stdcall CreateActCtxWWorker(ptr) CreateActCtxW
@ stdcall CreateBoundaryDescriptorA(str long)
@ stdcall CreateBoundaryDescriptorW(wstr long)
@ stdcall CreateDirectoryTransactedA(str str ptr ptr)
@ stdcall CreateDirectoryTransactedW(wstr wstr ptr ptr)
@ stdcall CreateEventExA(ptr str ptr long)  ;error on MPC #fixed
@ stdcall CreateEventExW(ptr wstr ptr long)  ;error on MPC #fixed
@ stdcall CreateFile2(wstr long long long long)
@ stdcall CreateFileMappingFromApp(ptr ptr long int64 wstr)
@ stdcall CreateFileMappingNumaA(ptr ptr long long long str long)
@ stdcall CreateFileMappingNumaW(ptr ptr long long long wstr long)
@ stdcall CreateFileTransactedA(str long long ptr long long ptr ptr ptr ptr)
@ stdcall CreateFileTransactedW(wstr long long ptr long long ptr ptr ptr ptr)
@ stdcall CreateHardLinkTransactedA(str str ptr ptr)
@ stdcall CreateHardLinkTransactedW(wstr wstr ptr ptr)
@ stdcall CreateMutexExA(ptr str long long)
@ stdcall CreateMutexExW(ptr wstr long long)
@ stdcall CreatePrivateNamespaceA(ptr ptr str)
@ stdcall CreatePrivateNamespaceW(ptr ptr wstr)
@ stdcall CreateRemoteThreadEx(long ptr long ptr long long ptr ptr)
@ stdcall CreateSemaphoreExA(ptr long long str ptr long)  ;error on MPC #fixed
@ stdcall CreateSemaphoreExW(ptr long long wstr ptr long)  ;error on MPC #fixed
@ stdcall -arch=i386 CreateSocketHandle()
@ stdcall CreateSymbolicLinkA(str str long) CreateSymbolicLinkW
@ stdcall CreateSymbolicLinkTransactedA(str str long ptr)
@ stdcall CreateSymbolicLinkTransactedW(wstr wstr long ptr)
@ stdcall CreateSymbolicLinkW(wstr wstr long)
@ stdcall CreateThreadpool(ptr)
@ stdcall CreateThreadpoolCleanupGroup() 
@ stdcall CreateThreadpoolIo(ptr) vtdll.TpReleaseIoCompletion
@ stdcall CreateThreadpoolTimer(ptr ptr ptr) vtdll.TpReleaseTimer
@ stdcall CreateThreadpoolWait(ptr ptr ptr)  vtdll.TpReleaseWait
@ stdcall CreateThreadpoolWork(ptr ptr ptr) vtdll.TpReleaseWork
@ stdcall CreateWaitableTimerExA(ptr str ptr long)
@ stdcall CreateWaitableTimerExW(ptr wstr ptr long)
@ stdcall DeactivateActCtxWorker(long ptr) DeactivateActCtx
@ stdcall DeleteBoundaryDescriptor() vtdll.RtlDeleteBoundaryDescriptor
@ stdcall DeleteFileTransactedA(str ptr)
@ stdcall DeleteFileTransactedW(wstr ptr)
@ stdcall DeleteProcThreadAttributeList(ptr)
@ stdcall DeleteSynchronizationBarrier(ptr)
@ stdcall DisassociateCurrentThreadFromCallback(ptr) vtdll.TpDisassociateCallback
@ stdcall DnsHostnameToComputerNameExW(wstr wstr ptr)
@ stdcall EnterSynchronizationBarrier(ptr long)
@ stdcall EnableThreadProfiling(ptr long int64 ptr)
@ stdcall EnumCalendarInfoExEx(ptr wstr long wstr long ptr)
@ stdcall EnumDateFormatsExEx(ptr wstr long)
@ stdcall EnumResourceLanguagesExA(ptr str str ptr long long long)
@ stdcall EnumResourceLanguagesExW(ptr wstr wstr ptr long long long)
@ stdcall EnumResourceNamesExA(ptr str ptr long long long)
@ stdcall EnumResourceNamesExW(ptr wstr ptr long long long)
@ stdcall EnumResourceTypesExA(ptr ptr long long long)
@ stdcall EnumResourceTypesExW(ptr ptr long long long)
@ stdcall EnumSystemLocalesEx(ptr long ptr ptr) ;unimplemented
@ stdcall EnumTimeFormatsEx(ptr wstr long ptr)
@ stdcall FindActCtxSectionGuidWorker(long ptr long ptr ptr) FindActCtxSectionGuid
@ stdcall FindActCtxSectionStringWWorker(long ptr long wstr ptr) FindActCtxSectionStringW
@ stdcall FindFirstFileNameTransactedW(wstr long ptr wstr ptr)
@ stdcall FindFirstFileNameW(wstr long ptr wstr)
@ stdcall FindFirstFileTransactedA(str long ptr long ptr long ptr)
@ stdcall FindFirstFileTransactedW(wstr long ptr long ptr long ptr)
@ stdcall FindFirstStreamTransactedW(wstr long ptr long ptr)
@ stdcall FindNextFileNameW(ptr ptr wstr)
@ stdcall FindNLSString(long long ptr long wstr long ptr)
@ stdcall FindNLSStringEx(wstr long wstr long wstr long ptr ptr ptr ptr)
@ stdcall FindStringOrdinal(long wstr long wstr long long)
@ stdcall FlushProcessWriteBuffers()
@ stdcall FreeLibraryWhenCallbackReturns(ptr ptr) vtdll.TpCallbackUnloadDllOnCompletion
@ stdcall GetActiveProcessorCount(long)
@ stdcall GetActiveProcessorGroupCount()
@ stdcall GetApplicationRestart(ptr ptr ptr ptr)
@ stdcall GetApplicationRecoveryCallback(ptr ptr ptr ptr ptr)
@ stdcall GetApplicationRecoveryCallbackWorker(ptr ptr ptr ptr ptr) GetApplicationRecoveryCallback
@ stdcall GetApplicationRestartSettings(ptr wstr ptr ptr)
@ stdcall GetApplicationRestartSettingsWorker(ptr wstr ptr ptr) GetApplicationRestartSettings
@ stdcall GetCachedSigningLevel(ptr ptr ptr ptr ptr ptr)
@ stdcall GetCalendarDateFormat(ptr long ptr ptr ptr long) ;unimplemented
@ stdcall GetCalendarDateFormatEx(wstr long ptr wstr wstr long) ;unimplemented
@ stdcall GetCalendarDaysInMonth(long long long long)
@ stdcall GetCalendarDifferenceInDays(ptr ptr ptr) 
@ stdcall GetCalendarInfoEx(wstr long wstr long wstr long ptr)  ;need test
@ stdcall GetCalendarMonthsInYear(long long long)
@ stdcall GetCalendarSupportedDateRange(long ptr ptr)
@ stdcall GetCalendarWeekNumber(ptr ptr ptr ptr) 
@ stdcall GetClientDynamicTimeZoneInformation(ptr)
@ stdcall GetCompressedFileSizeTransactedA(str ptr ptr)
@ stdcall GetCompressedFileSizeTransactedW(wstr ptr ptr)
@ stdcall GetConsoleHistoryInfo(ptr) 
@ stdcall GetConsoleOriginalTitleA(str long)
@ stdcall GetConsoleOriginalTitleW(wstr long)
@ stdcall GetConsoleScreenBufferInfoEx(ptr ptr)
@ stdcall GetCurrentActCtxWorker(ptr) GetCurrentActCtx
@ stdcall GetCurrencyFormatEx(wstr long wstr ptr wstr long) 
@ stdcall GetCurrentConsoleFontEx(ptr long ptr) 
@ stdcall GetCurrentPackageId(ptr ptr)
@ stdcall GetCurrentPackageInfo(long ptr ptr ptr)
@ stdcall GetCurrentProcessorNumberEx(ptr) vtdll.RtlGetCurrentProcessorNumberEx
@ stdcall GetCurrentProcessW() GetCurrentProcess
@ stdcall GetCurrentTransaction()
@ stdcall GetCurrentThreadStackLimits(ptr ptr)
@ stdcall GetDateFormatAWorker(long long ptr wstr ptr long) GetDateFormatA
@ stdcall GetDateFormatEx(wstr long ptr wstr ptr long wstr)  ;need test
@ stdcall GetDateFormatWWorker(long long ptr wstr ptr long) GetDateFormatW
@ stdcall GetDurationFormat(long long ptr long long ptr ptr long) ;need test
@ stdcall GetDurationFormatEx(wstr long ptr long long ptr ptr long) ;need test
@ stdcall GetDynamicTimeZoneInformation(ptr)
@ stdcall GetEnabledXStateFeatures()
@ stub GetEraNameCountedString
@ stdcall GetErrorMode()
@ stdcall GetFileAttributesByHandle(ptr ptr long)
@ stdcall GetFileAttributesTransactedA(str long ptr ptr)
@ stdcall GetFileAttributesTransactedW(wstr long ptr ptr)
@ stdcall GetFileBandwidthReservation(ptr ptr ptr long long long)
@ stdcall GetFileInformationByHandleEx(ptr long ptr long) ;fileextd.GetFileInformationByHandleEx
@ stdcall GetFinalPathNameByHandleA(ptr str long long)
@ stdcall GetFinalPathNameByHandleW(ptr wstr long long)
@ stdcall GetFileMUIInfo(long wstr ptr ptr)
@ stdcall GetFileMUIPath(long wstr wstr ptr wstr ptr ptr) ;need test
@ stdcall GetFirmwareEnvironmentVariableExA(str str ptr long ptr)
@ stdcall GetFirmwareEnvironmentVariableExW(wstr wstr ptr long ptr)
@ stdcall GetFirmwareType(ptr)
@ stdcall GetFullPathNameTransactedA(str long str str ptr)
@ stdcall GetFullPathNameTransactedW(wstr long wstr wstr ptr)
@ stdcall GetLocaleInfoEx(long long ptr long)
@ stdcall GetLocaleNullStringFromArrayInPool(long long long)
@ stdcall GetLogicalProcessorInformationEx(long ptr ptr)
@ stdcall GetLongPathNameTransactedA(str str long ptr)
@ stdcall GetLongPathNameTransactedW(wstr wstr long ptr)
@ stdcall GetMaximumProcessorCount(long)
@ stdcall GetMaximumProcessorGroupCount()
@ stdcall GetMemoryErrorHandlingCapabilities(ptr)
@ stdcall GetModuleFileNameFromHandleNoLock(ptr wstr long)
@ stdcall GetNamedPipeAttribute(ptr long str ptr ptr)
@ stdcall GetNamedPipeClientComputerNameA(ptr str long)
@ stdcall GetNamedPipeClientComputerNameW(ptr wstr long)
@ stdcall GetNamedPipeClientProcessId(ptr ptr)
@ stdcall GetNamedPipeClientSessionId(ptr ptr)
@ stdcall GetNamedPipeServerProcessId(ptr ptr)
@ stdcall GetNamedPipeServerSessionId(ptr ptr)
@ stdcall GetNLSVersionEx(long wstr ptr)
@ stdcall GetNumaAvailableMemoryNodeEx(long ptr)
@ stdcall GetNumaProximityNode(long ptr)
@ stdcall GetNumaNodeNumberFromHandle(ptr ptr)
@ stdcall GetNumaNodeProcessorMaskEx(long ptr)
@ stdcall GetNumaProcessorNodeEx(ptr ptr)
@ stdcall GetNumaProximityNodeEx(long ptr)
@ stdcall GetNumberFormatEx(wstr long wstr ptr wstr long)
@ stdcall GetOverlappedResultEx(ptr ptr ptr long long)
@ stdcall GetPhysicallyInstalledSystemMemory(ptr)
@ stdcall GetProcessDEPPolicy(ptr ptr ptr) ;is native on XP SP3
@ stdcall GetProcessGroupAffinity(ptr ptr ptr)
@ stdcall GetProcessInformation(ptr long ptr long)
@ stdcall GetProcessMitigationPolicy(ptr long ptr long)
@ stdcall GetProcessPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetProcessorSystemCycleTime(long ptr ptr)
@ stdcall GetProcessUserModeExceptionPolicy(ptr)
@ stdcall GetProductInfo(long long long long ptr) 
@ stdcall GetOSProductNameA(str long long)
@ stdcall GetOSProductNameW(wstr long long)
@ stdcall GetQueuedCompletionStatusEx(ptr ptr long ptr long long)
@ stdcall GetStringScripts(long wstr long wstr long) idndl.DownlevelGetStringScripts ;Internal call cause error on Server 2003 SP1 and XP SP2
@ stdcall GetSystemDefaultLocaleName(wstr long)
@ stdcall GetSystemDEPPolicy()
@ stdcall GetSystemPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetSystemTimePreciseAsFileTime(ptr)
@ stdcall GetThreadActualPriority(ptr)
@ stdcall GetThreadErrorMode()
@ stdcall GetThreadIdealProcessorEx(ptr ptr)
@ stdcall GetThreadInformation(ptr long ptr long)
@ stdcall GetThreadGroupAffinity(ptr ptr)
@ stdcall GetThreadPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetThreadUILanguage()
@ stdcall GetTickCount64() 
@ stdcall GetTimeFormatAWorker(long long ptr str ptr long) GetTimeFormatA
@ stdcall GetTimeFormatEx(wstr long ptr wstr ptr long)
@ stdcall GetTimeFormatWWorker(long long ptr wstr ptr long) GetTimeFormatW
@ stdcall GetTimeZoneInformationForYear(long ptr ptr) ;unimplemented
@ stdcall GetUILanguageInfo(long wstr wstr ptr ptr)
@ stdcall GetUserDefaultLocaleName(wstr long)
@ stdcall GetUserPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetVolumeInformationByHandleW(ptr wstr long ptr ptr ptr wstr long)
@ stdcall GetVolumeBandwidthContractProperties(ptr long long long)
@ stdcall GetXStateFeaturesMask(ptr ptr)
@ stdcall GlobalAddAtomExA(str long)
@ stdcall GlobalAddAtomExW(wstr long)
@ stdcall IdnToAscii(long wstr long ptr long) normaliz.IdnToAscii
@ stdcall IdnToUnicode(long wstr long ptr long) normaliz.IdnToUnicode
@ stdcall IdnToNameprepUnicode(long wstr long ptr long) normaliz.IdnToNameprepUnicode
@ stdcall InitializeConditionVariable(ptr) vtdll.RtlInitializeConditionVariable  ;error on K-Lite
@ stdcall InitializeContext(ptr long ptr ptr)
@ stdcall InitializeCriticalSectionEx(ptr long long) 
@ stdcall InitializeProcThreadAttributeList(ptr long long ptr) ;need test
@ stdcall InitializeSRWLock(ptr) vtdll.RtlInitializeSRWLock
@ stdcall InitializeSynchronizationBarrier(ptr long long)
@ stdcall InitOnceBeginInitialize(ptr long ptr ptr) ;- need test
@ stdcall InitOnceComplete(ptr long ptr) ;- need implement
@ stdcall InitOnceExecuteOnce(ptr ptr ptr ptr)
@ stdcall InitOnceInitialize(ptr)
@ stdcall InstallELAMCertificateInfo(ptr)
@ stdcall InterlockedPushListSList(ptr ptr ptr long) ntdll.RtlInterlockedPushListSList
@ stdcall InterlockedPushListSListEx(ptr ptr ptr long) ntdll.RtlInterlockedPushListSList
@ stdcall IsThreadpoolTimerSet(ptr) vtdll.TpIsTimerSet
@ stdcall IntToULong(long ptr)
@ stdcall IsCalendarLeapDay(long long long long long) 
@ stdcall IsCalendarLeapYear(long long long)
@ stdcall IsCalendarLeapMonth(long long long long)
@ stdcall IsNativeVhdBoot(ptr)
@ stdcall IsNormalizedString(long wstr long) normaliz.IsNormalizedString
@ stdcall IsProcessCritical(ptr ptr)
@ stdcall IsThreadAFiber()
@ stdcall IsValidCalDateTime(ptr long)
@ stdcall IsValidLocaleName(wstr) ;see wine implementation ;unimplemented
@ stdcall IsValidNLSVersion(long wstr ptr)
@ stdcall K32EmptyWorkingSet(ptr) EmptyWorkingSet
@ stdcall K32EnumDeviceDrivers(ptr long ptr) EnumDeviceDrivers
@ stdcall K32EnumPageFilesA(ptr ptr) EnumPageFilesA
@ stdcall K32EnumPageFilesW(ptr ptr) EnumPageFilesW
@ stdcall K32EnumProcessModules(ptr ptr long ptr) EnumProcessModules
@ stdcall K32EnumProcessModulesEx(ptr ptr long ptr long) EnumProcessModulesEx
@ stdcall K32EnumProcesses(ptr long ptr) EnumProcesses
@ stdcall K32GetDeviceDriverBaseNameA(ptr ptr long) GetDeviceDriverBaseNameA
@ stdcall K32GetDeviceDriverBaseNameW(ptr ptr long) GetDeviceDriverBaseNameW
@ stdcall K32GetDeviceDriverFileNameA(ptr ptr long) GetDeviceDriverFileNameA
@ stdcall K32GetDeviceDriverFileNameW(ptr ptr long) GetDeviceDriverFileNameW
@ stdcall K32GetMappedFileNameA(ptr ptr ptr long) GetMappedFileNameA
@ stdcall K32GetMappedFileNameW(ptr ptr ptr long) GetMappedFileNameW
@ stdcall K32GetModuleBaseNameA(ptr ptr ptr long) GetModuleBaseNameA
@ stdcall K32GetModuleBaseNameW(ptr ptr ptr long) GetModuleBaseNameW
@ stdcall K32GetModuleFileNameExA(ptr ptr ptr long) GetModuleFileNameExA
@ stdcall K32GetModuleFileNameExW(ptr ptr ptr long) GetModuleFileNameExW
@ stdcall K32GetModuleInformation(ptr ptr ptr long) GetModuleInformation
@ stdcall K32GetPerformanceInfo(ptr long) GetPerformanceInfo
@ stdcall K32GetProcessImageFileNameA(ptr ptr long) GetProcessImageFileNameA
@ stdcall K32GetProcessImageFileNameW(ptr ptr long) GetProcessImageFileNameW
@ stdcall K32GetProcessMemoryInfo(ptr ptr long) GetProcessMemoryInfo
@ stdcall K32GetWsChanges(ptr ptr long) GetWsChanges
@ stdcall K32GetWsChangesEx(ptr ptr ptr) GetWsChangesEx
@ stdcall K32InitializeProcessForWsWatch(ptr) InitializeProcessForWsWatch
@ stdcall K32QueryWorkingSet(ptr ptr long) QueryWorkingSet
@ stdcall K32QueryWorkingSetEx(ptr ptr long) QueryWorkingSetEx
@ stdcall LCIDToLocaleName(long wstr long long)
@ stdcall LCMapStringEx(wstr long wstr long ptr long ptr ptr long)
@ stdcall LeaveCriticalSectionWhenCallbackReturns(ptr ptr) vtdll.TpCallbackLeaveCriticalSectionOnCompletion
@ stdcall LoadAppInitDlls()
@ stdcall LoadPackagedLibrary(wstr long)
@ stdcall LoadStringA(ptr long ptr long)
@ stdcall LoadStringBaseW(ptr wstr ptr long)
@ stdcall LoadStringBaseExW(ptr wstr ptr long long)
@ stdcall LoadStringW(ptr long ptr long)
@ stdcall LocateXStateFeature(ptr long ptr)
@ stdcall LocaleNameFromLCID(long wstr long)
@ stdcall LocaleNameToLCID(wstr long)
@ stdcall MapViewOfFileExNuma(ptr long long long long ptr long)
@ stdcall MapViewOfFileFromApp(ptr long int64 long)
@ stdcall MoveFileTransactedA(str str ptr ptr long ptr)
@ stdcall MoveFileTransactedW(wstr wstr ptr ptr long ptr)
@ stdcall NlsCheckPolicy(wstr long) ;unimplemented
@ stdcall NlsEventDataDescCreate(ptr wstr wstr long)
@ stdcall NlsUpdateLocale(long long) ;need test
@ stdcall NlsUpdateSystemLocale(long long) ;need test
@ stdcall NlsWriteEtwEvent(int64 ptr long ptr)
@ stdcall NormalizeString(long wstr long wstr long) normaliz.NormalizeString
@ stdcall NotifyMountMgr(wstr wstr long)
@ stdcall NotifyUILanguageChange(long wstr wstr long ptr)
@ stdcall NtVdm64CreateProcessInternalW(long wstr wstr wstr ptr ptr long long ptr wstr ptr ptr) ntvdm64.NtVdm64CreateProcess
@ stdcall PowerClearRequest(ptr ptr)
@ stdcall PowerCreateRequest(ptr)
@ stdcall PowerCreateRequestW(ptr) PowerCreateRequest
@ stdcall PowerSetRequest(ptr long)
@ stdcall PrefetchVirtualMemory(ptr long ptr long)
@ stdcall OpenConsoleWStub(wstr long long long) OpenConsoleW
@ stdcall OpenFileById(ptr ptr long long ptr long) 
@ stdcall OpenPrivateNamespaceW(ptr wstr)
@ stdcall OpenPrivateNamespaceA(ptr str)
@ stdcall QueryActCtxSettingsW(long ptr wstr wstr wstr long ptr) ;obs: need implement (link with ntdllnew) RtlQueryActivationContextApplicationSettings 
@ stdcall QueryActCtxSettingsWWorker(long ptr wstr wstr wstr long ptr) QueryActCtxSettingsW
@ stdcall QueryActCtxWWorker(long ptr ptr long ptr long ptr) QueryActCtxW
@ stdcall QueryFullProcessImageNameA(ptr long ptr ptr) 
@ stdcall QueryFullProcessImageNameW(ptr long ptr ptr)
@ stdcall QueryIdleProcessorCycleTime(ptr ptr)
@ stdcall QueryIdleProcessorCycleTimeEx(long ptr ptr)
@ stdcall QueryProcessAffinityUpdateMode(ptr long)
@ stdcall QueryProcessCycleTime(ptr ptr)
@ stdcall QueryThreadCycleTime(ptr ptr)
@ stdcall QueryThreadpoolStackInformation(ptr ptr)
@ stdcall QueryThreadProfiling(ptr ptr)
@ stdcall QueryUnbiasedInterruptTime(ptr)
@ stdcall RaiseFailFastException(ptr ptr long)
@ stdcall ReadThreadProfilingData(ptr long ptr)
@ stdcall RecoveryFinished(long)
@ stdcall RecoveryInProgress(ptr)
@ stdcall RegisterApplicationRecoveryCallback(long ptr long long)
@ stdcall RegisterApplicationRestart(wstr long)
@ stdcall RegisterBadMemoryNotification(ptr)
@ stdcall ReleaseActCtxWorker(ptr) ReleaseActCtx
@ stdcall ReleaseMutexWhenCallbackReturns(ptr ptr) vtdll.TpCallbackReleaseMutexOnCompletion
@ stdcall ReleaseSemaphoreWhenCallbackReturns(ptr ptr long) vtdll.TpCallbackReleaseSemaphoreOnCompletion
@ stdcall ReleaseSRWLockExclusive(ptr) vtdll.RtlReleaseSRWLockExclusive
@ stdcall ReleaseSRWLockShared(ptr) vtdll.RtlReleaseSRWLockShared
@ stdcall ReplacePartitionUnit(wstr wstr long)
@ stdcall RemoveDirectoryTransactedA(str ptr)
@ stdcall RemoveDirectoryTransactedW(wstr ptr)
@ stdcall RemoveSecureMemoryCacheCallback(ptr)
@ stdcall ResolveDelayLoadedAPI(ptr ptr ptr ptr ptr long)
@ stdcall ResolveDelayLoadsFromDll(ptr str long)
@ stdcall ResolveLocaleName(wstr wstr long)
@ stdcall SetCachedSigningLevel(ptr long long ptr)
@ stdcall SetComputerNameEx2W(long long wstr)
@ stdcall SetConsoleHistoryInfo(ptr)
@ stdcall SetConsoleScreenBufferInfoEx(ptr ptr)
@ stdcall SetCurrentConsoleFontEx(ptr long ptr)
@ stdcall SetCurrentTransaction(ptr)
@ stdcall SetDefaultDllDirectories(long)
@ stdcall SetDynamicTimeZoneInformation(ptr)
@ stdcall SetEndOfFileEx(ptr ptr)
@ stdcall SetEventWhenCallbackReturns(ptr ptr) vtdll.TpCallbackSetEventOnCompletion
@ stdcall SetFileAttributesByHandle(ptr long long)
@ stdcall SetFileAttributesTransactedA(str long ptr)
@ stdcall SetFileAttributesTransactedW(wstr long ptr)
@ stdcall SetFileBandwidthReservation(ptr ptr ptr long long long) 
@ stdcall SetFileInformationByHandle(long long ptr long) ;fileextd.SetFileInformationByHandle
@ stdcall SetFileIoOverlappedRange(ptr ptr long)
@ stdcall SetFirmwareEnvironmentVariableExA(str str ptr long long)
@ stdcall SetFirmwareEnvironmentVariableExW(str str ptr long long)
@ stdcall SetStdHandleEx(long ptr long)
@ stdcall SetNamedPipeAttribute(ptr long str ptr long)
@ stdcall SetProcessAffinityUpdateMode(ptr long)
@ stdcall SetProcessDEPPolicy(long) ;is native on XP SP3
@ stdcall SetProcessInformation(ptr long ptr long)
@ stdcall SetProcessMitigationPolicy(ptr ptr long)
@ stdcall SetProcessPreferredUILanguages(long wstr ptr)
@ stdcall SetProcessUserModeExceptionPolicy(long)
@ stdcall SetSearchPathMode(long)
@ stdcall SetThreadErrorMode(long ptr)
@ stdcall SetThreadGroupAffinity(ptr ptr ptr)
@ stdcall SetThreadIdealProcessorEx(ptr ptr ptr)
@ stdcall SetThreadInformation(ptr long ptr long)
@ stdcall SetThreadpoolStackInformation(ptr ptr) 
@ stdcall SetThreadpoolThreadMaximum(ptr long) vtdll.TpSetPoolMaxThreads
@ stdcall SetThreadpoolThreadMinimum(ptr long) 
@ stdcall SetThreadpoolTimer(ptr ptr long long) vtdll.TpSetTimer
@ stdcall SetThreadpoolTimerEx(ptr ptr long long) vtdll.TpSetTimerEx
@ stdcall SetThreadpoolWait(ptr ptr ptr) vtdll.TpSetWait
@ stdcall SetThreadpoolWaitEx(ptr ptr ptr) vtdll.TpSetWaitEx
@ stdcall SetThreadPreferredUILanguages(long wstr ptr)
@ stdcall SetWaitableTimerEx(ptr ptr long ptr ptr ptr long)
@ stdcall SetXStateFeaturesMask(ptr int64)
@ stdcall SetVolumeMountPointWStub(wstr wstr) SetVolumeMountPointW
@ stdcall SleepConditionVariableCS(ptr ptr long)
@ stdcall SleepConditionVariableSRW(ptr ptr long long)
@ stdcall SortCloseHandle(ptr)
@ stub SortGetHandle
@ stdcall StartThreadpoolIo(ptr) vtdll.TpStartAsyncIoOperation
@ stdcall SubmitThreadpoolWork(ptr) vtdll.TpPostWork
@ stdcall SystemTimeToTzSpecificLocalTimeEx(ptr ptr ptr)
@ stdcall timeBeginPeriod(long)
@ stdcall timeEndPeriod(long)
@ stdcall timeGetDevCaps(ptr long)
@ stdcall timeGetSystemTime(ptr long)
@ stdcall timeGetTime()
@ stdcall TryAcquireSRWLockExclusive(ptr) vtdll.RtlTryAcquireSRWLockExclusive
@ stdcall TryAcquireSRWLockShared(ptr) vtdll.RtlTryAcquireSRWLockShared
@ stdcall TrySubmitThreadpoolCallback(ptr ptr ptr) 
@ stdcall TzSpecificLocalTimeToSystemTimeEx(ptr ptr ptr)
@ stdcall UnmapViewOfFileEx(ptr long)
@ stdcall UnregisterApplicationRestart()
@ stdcall UnregisterApplicationRecoveryCallback()
@ stdcall UnregisterBadMemoryNotification(ptr)
@ stdcall UpdateCalendarDayOfWeek(ptr) 
@ stdcall UpdateProcThreadAttribute(ptr long long ptr long ptr ptr)
@ stdcall VerifyScripts(long wstr long wstr long) idndl.DownlevelVerifyScripts
@ stdcall VirtualAllocExNuma(ptr ptr long long long long)
@ stdcall WaitForThreadpoolIoCallbacks(ptr long) vtdll.TpWaitForIoCompletion
@ stdcall WaitForThreadpoolTimerCallbacks(ptr long) vtdll.TpWaitForTimer
@ stdcall WaitForThreadpoolWaitCallbacks(ptr long) vtdll.TpWaitForWait
@ stdcall WaitForThreadpoolWorkCallbacks(ptr long) vtdll.TpWaitForWork
@ stdcall WakeAllConditionVariable(ptr) vtdll.RtlWakeAllConditionVariable ;error on K-Lite
@ stdcall WakeConditionVariable(ptr) vtdll.RtlWakeConditionVariable ;commented for now ;error on K-Lite
@ stdcall WerGetFlags(ptr ptr)
@ stdcall WerpCleanupMessageMapping()
@ stdcall WerpGetDebugger(ptr long long long long)
@ stdcall WerpInitiateRemoteRecovery(ptr)
@ stdcall WerpLaunchAeDebug(ptr ptr str ptr wstr long)
@ stdcall WerpNotifyLoadStringResource(ptr wstr ptr long)
@ stdcall WerpNotifyLoadStringResourceWorker(ptr wstr ptr long) WerpNotifyLoadStringResource
@ stub WerpNotifyLoadStringResourceEx
@ stdcall WerpNotifyUseStringResource(ptr)
@ stdcall WerpNotifyUseStringResourceWorker(ptr) WerpNotifyUseStringResource
@ stdcall WerpStringLookup(ptr ptr)
@ stdcall WerRegisterFile(wstr long long)
@ stdcall WerRegisterFileWorker(wstr long long) WerRegisterFile
@ stdcall WerRegisterMemoryBlock(ptr long)
@ stdcall WerRegisterMemoryBlockWorker(ptr long) WerRegisterMemoryBlock
@ stdcall WerRegisterRuntimeExceptionModule(wstr ptr)
@ stdcall WerRegisterRuntimeExceptionModuleWorker(wstr ptr) WerRegisterRuntimeExceptionModule
@ stdcall WerSetFlags(long)
@ stdcall WerUnregisterFile(wstr) 
@ stdcall WerUnregisterFileWorker(wstr) WerUnregisterFile
@ stdcall WerUnregisterMemoryBlock(ptr) 
@ stdcall WerUnregisterRuntimeExceptionModule(wstr ptr)
@ stdcall WerUnregisterRuntimeExceptionModuleWorker(wstr ptr) WerUnregisterRuntimeExceptionModule
@ stdcall WerUnregisterMemoryBlockWorker(ptr) WerUnregisterMemoryBlock
@ stdcall Wow64GetThreadContext(ptr ptr)
@ stdcall Wow64SetThreadContext(ptr ptr)
@ stdcall Wow64GetThreadSelectorEntry(ptr long ptr)
@ stdcall Wow64SuspendThread(ptr)
@ stdcall ZombifyActCtxWorker(ptr) ZombifyActCtx

#Import from advapi32
@ stdcall AccessCheck(ptr long long ptr ptr ptr ptr ptr) advapi32.AccessCheck
@ stdcall AccessCheckAndAuditAlarmW(wstr ptr wstr wstr ptr long ptr long ptr ptr ptr) advapi32.AccessCheckAndAuditAlarmW
@ stdcall AccessCheckByType(ptr ptr long long ptr long ptr ptr ptr ptr ptr) advapi32.AccessCheckByType
@ stdcall AccessCheckByTypeAndAuditAlarmW(wstr ptr wstr wstr ptr ptr long long long ptr long ptr long ptr ptr ptr) advapi32.AccessCheckByTypeAndAuditAlarmW
@ stdcall AccessCheckByTypeResultList(ptr ptr ptr long ptr long ptr ptr ptr ptr ptr) advapi32.AccessCheckByTypeResultList
@ stdcall AccessCheckByTypeResultListAndAuditAlarmByHandleW(wstr ptr ptr wstr wstr ptr long long long long ptr long ptr long ptr ptr ptr) advapi32.AccessCheckByTypeResultListAndAuditAlarmByHandleW
@ stdcall AccessCheckByTypeResultListAndAuditAlarmW(wstr ptr wstr wstr ptr long long long long ptr long ptr long ptr ptr ptr) advapi32.AccessCheckByTypeResultListAndAuditAlarmW
@ stdcall AddAccessAllowedAce(ptr long long ptr) advapi32.AddAccessAllowedAce
@ stdcall AddAccessAllowedAceEx(ptr long long long ptr) advapi32.AddAccessAllowedAceEx
@ stdcall AddAccessAllowedObjectAce(ptr long long long ptr ptr ptr) advapi32.AddAccessAllowedObjectAce
@ stdcall AddAccessDeniedAce(ptr long long ptr) advapi32.AddAccessDeniedAce
@ stdcall AddAccessDeniedAceEx(ptr long long long ptr) advapi32.AddAccessDeniedAceEx
@ stdcall AddAccessDeniedObjectAce(ptr long long long ptr ptr ptr) advapi32.AddAccessDeniedObjectAce
@ stdcall AddAce(ptr long long ptr long) advapi32.AddAce
@ stdcall AddAuditAccessAce(ptr long long ptr long long) advapi32.AddAuditAccessAce
@ stdcall AddAuditAccessAceEx(ptr long long long ptr long long) advapi32.AddAuditAccessAceEx
@ stdcall AddAuditAccessObjectAce(ptr long long long ptr ptr ptr long long) advapi32.AddAuditAccessObjectAce
@ stdcall AddMandatoryAce(ptr long long long ptr) advapi32.AddMandatoryAce
@ stdcall AdjustTokenGroups(long long ptr long ptr ptr) advapi32.AdjustTokenGroups
@ stdcall AdjustTokenPrivileges(long long ptr long ptr ptr) advapi32.AdjustTokenPrivileges
@ stdcall AllocateAndInitializeSid(ptr long long long long long long long long long ptr) advapi32.AllocateAndInitializeSid
@ stdcall AllocateLocallyUniqueId(ptr) advapi32.AllocateLocallyUniqueId
@ stdcall CreateProcessAsUserA(long str str ptr ptr long long ptr str ptr ptr) advapi32.CreateProcessAsUserA
@ stdcall CreateProcessAsUserW(long wstr wstr ptr ptr long long ptr wstr ptr ptr) advapi32.CreateProcessAsUserW
@ stdcall CreateWellKnownSid(long ptr ptr ptr) advapi32.CreateWellKnownSid
@ stdcall DuplicateTokenEx(long long ptr long long ptr)  advapi32.DuplicateTokenEx
@ stdcall EqualSid(ptr ptr) advapi32.EqualSid
@ stdcall FreeSid(ptr) advapi32.FreeSid
@ stdcall GetFileSecurityA(str long ptr long ptr) advapi32.GetFileSecurityA
@ stdcall GetFileSecurityW(wstr long ptr long ptr) advapi32.GetFileSecurityW
@ stdcall GetLengthSid(ptr) advapi32.GetLengthSid
@ stdcall GetSecurityDescriptorControl(ptr ptr ptr) advapi32.GetSecurityDescriptorControl
@ stdcall GetSecurityDescriptorDacl(ptr ptr ptr ptr) advapi32.GetSecurityDescriptorDacl
@ stdcall GetSecurityDescriptorLength(ptr) ntdll.RtlLengthSecurityDescriptor
@ stdcall GetSecurityDescriptorOwner(ptr ptr ptr) advapi32.GetSecurityDescriptorOwner
@ stdcall GetTokenInformation(long long ptr long ptr) advapi32.GetTokenInformation
@ stdcall ImpersonateLoggedOnUser(long) advapi32.ImpersonateLoggedOnUser
@ stdcall InitializeAcl(ptr long long) advapi32.InitializeAcl
@ stdcall InitializeSecurityDescriptor(ptr long) advapi32.InitializeSecurityDescriptor
@ stdcall IsValidSecurityDescriptor(ptr) advapi32.IsValidSecurityDescriptor
@ stdcall IsValidSid(ptr) advapi32.IsValidSid
@ stdcall IsWellKnownSid(ptr long) advapi32.IsWellKnownSid
@ stdcall MakeSelfRelativeSD(ptr ptr ptr) advapi32.MakeSelfRelativeSD
@ stdcall OpenProcessToken(ptr long ptr) advapi32.OpenProcessToken
@ stdcall OpenThreadToken(ptr long long ptr) advapi32.OpenThreadToken
@ stdcall RegCloseKey(ptr) advapi32.RegCloseKey 
@ stdcall RegCopyTreeW(ptr str ptr) advapi32.RegCopyTreeW 
@ stdcall RegCreateKeyExA(long str long ptr long long ptr ptr ptr) advapi32.RegCreateKeyExA 
@ stdcall RegCreateKeyExW(long wstr long ptr long long ptr ptr ptr) advapi32.RegCreateKeyExW 
@ stdcall RegDeleteKeyExA(long str long long) advapi32.RegDeleteKeyExA 
@ stdcall RegDeleteKeyExW(long wstr long long) advapi32.RegDeleteKeyExW 
@ stdcall RegDeleteTreeA(long str) advapi32.RegDeleteTreeA 
@ stdcall RegDeleteTreeW(long wstr) advapi32.RegDeleteTreeW 
@ stdcall RegDeleteValueA(long str) advapi32.RegDeleteValueA
@ stdcall RegDeleteValueW(long wstr) advapi32.RegDeleteValueW 
@ stdcall RegDisablePredefinedCacheEx() advapi32.RegDisablePredefinedCacheEx 
@ stdcall RegEnumKeyExA(long long ptr ptr ptr ptr ptr ptr) advapi32.RegEnumKeyExA 
@ stdcall RegEnumKeyExW(long long ptr ptr ptr ptr ptr ptr) advapi32.RegEnumKeyExW 
@ stdcall RegEnumValueA(long long ptr ptr ptr ptr ptr ptr) advapi32.RegEnumValueA 
@ stdcall RegEnumValueW(long long ptr ptr ptr ptr ptr ptr) advapi32.RegEnumValueW 
@ stdcall RegFlushKey(long) advapi32.RegFlushKey 
@ stdcall RegGetKeySecurity(long long ptr ptr) advapi32.RegGetKeySecurity 
@ stdcall RegGetValueA(long str str long ptr ptr ptr) advapi32.RegGetValueA 
@ stdcall RegGetValueW(long wstr wstr long ptr ptr ptr) advapi32.RegGetValueW 
@ stdcall RegLoadKeyA(long str str) advapi32.RegLoadKeyA 
@ stdcall RegLoadKeyW(long wstr wstr) advapi32.RegLoadKeyW 
@ stdcall RegLoadMUIStringA(long str str long ptr long str) advapi32.RegLoadMUIStringA 
@ stdcall RegLoadMUIStringW(long wstr wstr long ptr long wstr) advapi32.RegLoadMUIStringW 
@ stdcall RegNotifyChangeKeyValue(long long long long long) advapi32.RegNotifyChangeKeyValue 
@ stdcall RegOpenCurrentUser(long ptr) advapi32.RegOpenCurrentUser 
@ stdcall RegOpenKeyExA(long str long long ptr) advapi32.RegOpenKeyExA 
@ stdcall RegOpenKeyExW(long wstr long long ptr) advapi32.RegOpenKeyExW 
@ stdcall RegOpenUserClassesRoot(ptr long long ptr) advapi32.RegOpenUserClassesRoot 
@ stdcall RegQueryInfoKeyA(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr) advapi32.RegQueryInfoKeyA 
@ stdcall RegQueryInfoKeyW(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr) advapi32.RegQueryInfoKeyW 
@ stdcall RegQueryValueExA(long str ptr ptr ptr ptr) advapi32.RegQueryValueExA
@ stdcall RegQueryValueExW(long wstr ptr ptr ptr ptr) advapi32.RegQueryValueExW 
@ stdcall RegRestoreKeyA(long str long) advapi32.RegRestoreKeyA 
@ stdcall RegRestoreKeyW(long wstr long) advapi32.RegRestoreKeyW 
@ stdcall RegSaveKeyExA(ptr str ptr long) advapi32.RegSaveKeyExA 
@ stdcall RegSaveKeyExW(ptr str ptr long) advapi32.RegSaveKeyExW 
@ stdcall RegSetKeySecurity(long long ptr) advapi32.RegSetKeySecurity 
@ stdcall RegSetValueExA(long str long long ptr long) advapi32.RegSetValueExA 
@ stdcall RegSetValueExW(long wstr long long ptr long) advapi32.RegSetValueExW 
@ stdcall RegUnLoadKeyA(long str) advapi32.RegUnLoadKeyA 
@ stdcall RegUnLoadKeyW(long wstr) advapi32.RegUnLoadKeyW
@ stdcall RevertToSelf() advapi32.RevertToSelf
@ stdcall SetFileSecurityA(str long ptr) advapi32.SetFileSecurityA
@ stdcall SetFileSecurityW(wstr long ptr) advapi32.SetFileSecurityW
@ stdcall SetSecurityDescriptorDacl(ptr long ptr long) advapi32.SetSecurityDescriptorDacl
@ stdcall SetSecurityDescriptorGroup(ptr ptr long) advapi32.SetSecurityDescriptorGroup
@ stdcall SetSecurityDescriptorOwner(ptr ptr long) advapi32.SetSecurityDescriptorOwner
@ stdcall SetThreadToken(ptr ptr) advapi32.SetThreadToken 

#Import from Version
@ stdcall VerQueryValueA(ptr str ptr ptr) version.VerQueryValueA
@ stdcall VerQueryValueW(ptr wstr ptr ptr) version.VerQueryValueW

#Import from user32

#Only Longhorn functions (Pre-reset)
@ stdcall SetThreadActualPriority(ptr ptr)

#Longhorn 5048 functions
@ stdcall BeginUpdateResourceExA(str ptr ptr ptr) kernelfull.BeginUpdateResourceExA
@ stdcall BeginUpdateResourceExW(wstr ptr ptr ptr) kernelfull.BeginUpdateResourceExW
@ stdcall EndUpdateResourceExA(ptr long long long) kernelfull.EndUpdateResourceExA
@ stdcall EndUpdateResourceExW(ptr long long long) kernelfull.EndUpdateResourceExW
@ stdcall InitializeCriticalSectionAndSpinCountEx(ptr ptr ptr) kernelfull.InitializeCriticalSectionAndSpinCountEx
@ stdcall UpdateResourceExA(ptr long str long long long long) kernelfull.UpdateResourceExA
@ stdcall UpdateResourceExW(ptr long str long long long long) kernelfull.UpdateResourceExW
@ stdcall SubmitThreadpoolCallback(ptr ptr) ntdll.SubmitThreadpoolCallback

#Todo 
#GetOsSafeBootMode #(win8)
#EnumDynamicTimeZoneInformation
#GetDynamicTimeZoneInformationEffectiveYears
#SetDynamicTimeZoneInformation
#@ stdcall AddDllDirectory(wstr) vtdll.LdrAddDllDirectory #(win8)
#@ stdcall RemoveDllDirectory(ptr) vtdll.LdrRemoveDllDirectory #(win8)