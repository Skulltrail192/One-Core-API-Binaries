#undef i386
@ stdcall ActivateActCtx(ptr ptr)
@ stdcall AddAtomA(str)
@ stdcall AddAtomW(wstr)
@ stdcall AddConsoleAliasA(str str str) ;check
@ stdcall AddConsoleAliasW(wstr wstr wstr) ;check
@ stdcall AddLocalAlternateComputerNameA(str ptr)
@ stdcall AddLocalAlternateComputerNameW(wstr ptr)
@ stdcall AddRefActCtx(ptr)
@ stdcall AddVectoredContinueHandler(long ptr) ntext.RtlAddVectoredContinueHandler
@ stdcall AddVectoredExceptionHandler(long ptr) ntext.RtlAddVectoredExceptionHandler
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
@ stdcall BaseProcessInitPostImport() ; missing in Win 7
@ stdcall -arch=x86_64 BaseProcessStart()
@ stdcall -arch=x86_64 BaseThreadStart()
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
@ stdcall -i386 CreateSocketHandle()
@ stdcall CreateTapePartition(long long long long)
@ stdcall CreateThread(ptr long ptr long long ptr)
@ stdcall CreateTimerQueue ()
@ stdcall CreateTimerQueueTimer(ptr long ptr ptr long long long)
@ stdcall CreateToolhelp32Snapshot(long long)
@ stdcall CreateWaitableTimerA(ptr long str)
@ stdcall CreateWaitableTimerW(ptr long wstr)
@ stdcall DeactivateActCtx(long ptr)
@ stdcall DebugActiveProcess(long)
@ stdcall DebugActiveProcessStop(long)
@ stdcall DebugBreak() ntext.DbgBreakPoint
@ stdcall DebugBreakProcess(long)
@ stdcall DebugSetProcessKillOnExit(long)
@ stdcall DecodePointer(ptr) ntext.RtlDecodePointer
@ stdcall DecodeSystemPointer(ptr) ntext.RtlDecodeSystemPointer
@ stdcall DefineDosDeviceA(long str str)
@ stdcall DefineDosDeviceW(long wstr wstr)
@ stdcall DelayLoadFailureHook(str str)
@ stdcall DeleteAtom(long)
@ stdcall DeleteCriticalSection(ptr) ntext.RtlDeleteCriticalSection
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
@ stdcall DnsHostnameToComputerNameA (str ptr ptr)
@ stdcall DnsHostnameToComputerNameW (wstr ptr ptr)
@ stdcall DosDateTimeToFileTime(long long ptr)
@ stdcall DosPathToSessionPathA(long str str)
@ stdcall DosPathToSessionPathW(long wstr wstr)
@ stdcall DuplicateConsoleHandle(long long long long)
@ stdcall DuplicateHandle(long long long ptr long long long)
@ stdcall EncodePointer(ptr) ntext.RtlEncodePointer
@ stdcall EncodeSystemPointer(ptr) ntext.RtlEncodeSystemPointer
@ stdcall EndUpdateResourceA(long long)
@ stdcall EndUpdateResourceW(long long)
@ stdcall EnterCriticalSection(ptr) ntext.RtlEnterCriticalSection
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
@ stdcall ExitProcess(long) ; FIXME: ntext.RtlExitUserProcess
@ stdcall ExitThread(long) ; FIXME: ntext.RtlExitUserThread
@ stdcall ExitVDM(long long)
@ stdcall ExpandEnvironmentStringsA(str ptr long)
@ stdcall ExpandEnvironmentStringsW(wstr ptr long)
@ stdcall ExpungeConsoleCommandHistoryA(long)
@ stdcall ExpungeConsoleCommandHistoryW(long)
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
@ stdcall -norelay GetCurrentProcess()
@ stdcall -norelay GetCurrentProcessId()
@ stdcall GetCurrentProcessorNumber() ntext.RtlGetCurrentProcessorNumber
@ stdcall -norelay GetCurrentThread()
@ stdcall -norelay GetCurrentThreadId()
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
@ stdcall -i386 GetHandleContext(long) ; missing on x64
@ stdcall GetHandleInformation(long ptr)
@ stdcall GetLargestConsoleWindowSize(long)
@ stdcall GetLastError() ntext.RtlGetLastWin32Error
@ stdcall GetLinguistLangSize(ptr) ; missing in Win 7
@ stdcall GetLocalTime(ptr)
@ stdcall GetLocaleInfoA(long long ptr long) ;GeptLocaleInfoA
@ stdcall GetLocaleInfoW(long long ptr long) ;GetpLocaleInfoW
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
@ stdcall GetModuleHandleExA(long str ptr)
@ stdcall GetModuleHandleExW(long wstr ptr)
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
@ stdcall GetProcessAffinityMask(ptr ptr ptr)
@ stdcall GetProcessHandleCount(long ptr)
@ stdcall -norelay GetProcessHeap()
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
@ stdcall HeapAlloc(long long long) ntext.RtlAllocateHeap
@ stdcall HeapCompact(long long)
@ stdcall HeapCreate(long long long)
@ stdcall HeapCreateTagsW(long long wstr wstr) ; missing in Win 7
@ stdcall HeapDestroy(long)
@ stdcall HeapExtend(long long ptr long) ; missing in Win 7
@ stdcall HeapFree(long long long) ntext.RtlFreeHeap
@ stdcall HeapLock(long)
@ stdcall HeapQueryInformation(long long ptr long ptr)
@ stdcall HeapQueryTagW(long long long long ptr) ; missing in Win 7
@ stdcall HeapReAlloc(long long ptr long) ntext.RtlReAllocateHeap
@ stdcall HeapSetInformation(ptr long ptr long)
@ stdcall HeapSize(long long ptr) ntext.RtlSizeHeap
@ stdcall HeapSummary(long long ptr)
@ stdcall HeapUnlock(long)
@ stdcall HeapUsage(long long long long ptr) ; missing in Win 7
@ stdcall HeapValidate(long long ptr)
@ stdcall HeapWalk(long ptr)
@ stdcall InitAtomTable(long)
@ stdcall InitializeCriticalSection(ptr)
@ stdcall InitializeCriticalSectionAndSpinCount(ptr long)
@ stdcall InitializeSListHead(ptr) ntext.RtlInitializeSListHead
@ stdcall -arch=i386 -ret64 InterlockedCompareExchange64(ptr double double) ntext.RtlInterlockedCompareExchange64
@ stdcall -arch=i386 InterlockedCompareExchange (ptr long long)
@ stdcall -arch=i386 InterlockedDecrement(ptr)
@ stdcall -arch=i386 InterlockedExchange(ptr long)
@ stdcall -arch=i386 InterlockedExchangeAdd(ptr long)
@ stdcall InterlockedFlushSList(ptr) ntext.RtlInterlockedFlushSList
@ stdcall -arch=i386 InterlockedIncrement(ptr)
@ stdcall InterlockedPopEntrySList(ptr) ntext.RtlInterlockedPopEntrySList
@ stdcall InterlockedPushEntrySList(ptr ptr) ntext.RtlInterlockedPushEntrySList
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
@ stdcall LeaveCriticalSection(ptr) ntext.RtlLeaveCriticalSection
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
@ stdcall QueryDepthSList(ptr) ntext.RtlQueryDepthSList
@ stdcall QueryDosDeviceA(str ptr long)
@ stdcall QueryDosDeviceW(wstr ptr long)
@ stdcall QueryInformationJobObject(long long ptr long ptr)
@ stdcall QueryMemoryResourceNotification(ptr ptr)
@ stdcall QueryPerformanceCounter(ptr)
@ stdcall QueryPerformanceFrequency(ptr)
@ stdcall QueueUserAPC(ptr long long)
@ stdcall QueueUserWorkItem(ptr ptr long)
@ stdcall -norelay RaiseException(long long long ptr)
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
@ stdcall RemoveVectoredContinueHandler(ptr) ntext.RtlRemoveVectoredContinueHandler
@ stdcall RemoveVectoredExceptionHandler(ptr) ntext.RtlRemoveVectoredExceptionHandler
@ stdcall ReplaceFile(wstr wstr wstr long ptr ptr) ReplaceFileW
@ stdcall ReplaceFileA(str str str long ptr ptr)
@ stdcall ReplaceFileW(wstr wstr wstr long ptr ptr)
@ stdcall RequestDeviceWakeup(long)
@ stdcall RequestWakeupLatency(long)
@ stdcall ResetEvent(long)
@ stdcall ResetWriteWatch(ptr long)
@ stdcall RestoreLastError(long) ntext.RtlRestoreLastWin32Error
@ stdcall ResumeThread(long)
@ stdcall -arch=x86_64 RtlAddFunctionTable(ptr long long) ntext.RtlAddFunctionTable
@ stdcall -register RtlCaptureContext(ptr) ntext.RtlCaptureContext
@ stdcall RtlCaptureStackBackTrace(long long ptr ptr) ntext.RtlCaptureStackBackTrace
@ stdcall -arch=x86_64 RtlCompareMemory(ptr ptr ptr) ntext.RtlCompareMemory
@ stdcall -arch=x86_64 RtlCopyMemory(ptr ptr ptr) ntext.memcpy
@ stdcall -arch=x86_64 RtlDeleteFunctionTable(ptr) ntext.RtlDeleteFunctionTable
@ stdcall RtlFillMemory(ptr long long) ntext.RtlFillMemory
@ stdcall -arch=x86_64 RtlInstallFunctionTableCallback(double double long ptr ptr ptr) ntext.RtlInstallFunctionTableCallback
@ stdcall -arch=x86_64 RtlLookupFunctionEntry(ptr ptr ptr) ntext.RtlLookupFunctionEntry
@ stdcall RtlMoveMemory(ptr ptr long) ntext.RtlMoveMemory
@ stdcall -arch=x86_64 RtlPcToFileHeader(ptr ptr) ntext.RtlPcToFileHeader
@ stdcall -arch=x86_64 RtlRaiseException(ptr) ntext.RtlRaiseException
@ stdcall -arch=x86_64 RtlRestoreContext(ptr ptr) ntext.RtlRestoreContext
@ stdcall RtlUnwind(ptr ptr ptr long) ntext.RtlUnwind
@ stdcall -arch=x86_64 RtlUnwindEx(ptr ptr ptr ptr ptr ptr) ntext.RtlUnwindEx
@ stdcall -arch=x86_64 RtlVirtualUnwind(ptr ptr ptr long) ntext.RtlVirtualUnwind
@ stdcall RtlZeroMemory(ptr long) ntext.RtlZeroMemory
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
@ stdcall SetCriticalSectionSpinCount(ptr long) ntext.RtlSetCriticalSectionSpinCount
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
@ stdcall SetFileAttributesA(str long)
@ stdcall SetFileAttributesW(wstr long)
@ stdcall SetFilePointer(long long ptr long)
@ stdcall SetFilePointerEx(long double ptr long)
@ stdcall SetFileShortNameA(long str)
@ stdcall SetFileShortNameW(long wstr)
@ stdcall SetFileTime(long ptr ptr ptr)
@ stdcall SetFileValidData(long double)
@ stdcall SetFirmwareEnvironmentVariableA(str str ptr long)
@ stdcall SetFirmwareEnvironmentVariableW(wstr wstr ptr long)
@ stdcall -i386 SetHandleContext(long long) ; missing in Win 7 x64
@ stdcall SetHandleCount(long)
@ stdcall SetHandleInformation(long long long)
@ stdcall SetInformationJobObject(long long ptr long)
@ stdcall SetLastConsoleEventActive() ; missing in XP SP3
@ stdcall SetLastError(long) ntext.RtlSetLastWin32Error
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
@ stdcall -norelay TlsGetValue(long)
@ stdcall -norelay TlsSetValue(long ptr)
@ stdcall Toolhelp32ReadProcessMemory(long ptr ptr long ptr)
@ stdcall TransactNamedPipe(long ptr long ptr long ptr ptr)
@ stdcall TransmitCommChar(long long)
@ stdcall TryEnterCriticalSection(ptr) ntext.RtlTryEnterCriticalSection
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
@ stdcall -ret64 VerSetConditionMask(long long long long) ntext.VerSetConditionMask
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
@ stdcall WTSGetActiveConsoleSessionId()
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
@ stdcall ZombifyActCtx(ptr)
@ stdcall -arch=x86_64 __C_specific_handler() ntext.__C_specific_handler
@ stdcall -arch=x86_64 __chkstk() ntext.__chkstk
@ stdcall -arch=x86_64 __misaligned_access() ntext.__misaligned_access
@ stdcall _hread(long ptr long)
@ stdcall _hwrite(long ptr long)
@ stdcall _lclose(long)
@ stdcall _lcreat(str long)
@ stdcall _llseek(long long long)
@ stdcall -arch=x86_64 _local_unwind() ntext._local_unwind
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
@ stdcall lstrlen(str) lstrlenA
@ stdcall lstrlenA(str)
@ stdcall lstrlenW(wstr)
@ stdcall -arch=x86_64 uaw_lstrcmpW(wstr wstr)
@ stdcall -arch=x86_64 uaw_lstrcmpiW(wstr wstr)
@ stdcall -arch=x86_64 uaw_lstrlenW(wstr)
@ stdcall -arch=x86_64 uaw_wcschr(wstr long)
@ stdcall -arch=x86_64 uaw_wcscpy(ptr wstr)
@ stdcall -arch=x86_64 uaw_wcsicmp(wstr wstr)
@ stdcall -arch=x86_64 uaw_wcslen(wstr)
@ stdcall -arch=x86_64 uaw_wcsrchr(wstr long)

#functions for XP x64 WOW
@ stdcall -arch=i386 BaseProcessStartThunk(ptr ptr)
@ stdcall -arch=i386 BaseThreadStartThunk(ptr ptr) 
@ stdcall CtrlRoutine(ptr) 
@ stdcall ConsoleIMERoutine(ptr)

#missing on XP 
@ stdcall BaseIsAppcompatInfrastructureDisabled() 
@ stdcall ConvertThreadToFiberEx(ptr long)
@ stdcall FindFirstStreamW(wstr ptr ptr long)
@ stdcall FindNextStreamW(ptr ptr)
@ stdcall FlsAlloc(ptr)
@ stdcall FlsFree(long)
@ stdcall FlsGetValue(long)
@ stdcall FlsSetValue(long ptr)
@ stdcall GetLargePageMinimum()
@ stdcall GetProcessIdOfThread(ptr)
@ stdcall GetThreadId(ptr)
@ stdcall GetProcessWorkingSetSizeEx(long ptr ptr long)
@ stdcall IsTimeZoneRedirectionEnabled()
@ stdcall NeedCurrentDirectoryForExePathA(str)
@ stdcall NeedCurrentDirectoryForExePathW(wstr)
@ stdcall SetEnvironmentStringsA(ptr)
@ stdcall SetEnvironmentStringsW(ptr)
@ stdcall SetProcessWorkingSetSizeEx(long long long long)
@ stdcall Wow64DisableWow64FsRedirection(ptr)
@ stdcall Wow64EnableWow64FsRedirection(long)
@ stdcall Wow64RevertWow64FsRedirection(ptr)
@ stdcall ReOpenFile(ptr long long long)
@ stdcall GetNLSVersion(long long ptr) 
@ stdcall IsNLSDefinedString(long long ptr long long) kernelfull.IsNLSDefinedString
@ stdcall SetFileCompletionNotificationModes(ptr long) kernelfull.SetFileCompletionNotificationModes

#Missing on XP and 2003 RTM
@ stdcall BaseCheckRunApp(long ptr long long long long long long long long) kernelfull.BaseCheckRunApp
@ stdcall BasepCheckBadapp(long ptr long long long long long long long) kernelfull.BasepCheckBadapp
@ stdcall EnumSystemFirmwareTables(long ptr long) kernelfull.EnumSystemFirmwareTables
@ stdcall GetSystemFileCacheSize(ptr ptr ptr) kernelfull.GetSystemFileCacheSize
@ stdcall GetSystemFirmwareTable(long long ptr long) kernelfull.GetSystemFirmwareTable
 
#Missing on Server 2003 RTM (only available on 2003 SP1 and SP2)
@ stdcall BaseQueryModuleData(str str ptr ptr ptr) kernelfull.BaseQueryModuleData
@ stdcall BasepCheckWinSaferRestrictions(long long long long long long) kernelfull.BasepCheckWinSaferRestrictions
@ stdcall BasepFreeAppCompatData(ptr ptr)
@ stdcall SetSystemFileCacheSize(long long long) kernelfull.SetSystemFileCacheSize
@ stdcall SetThreadStackGuarantee(ptr)

#Native in XP SP3 and Missing in Server 2003 SP2 with Updates
@ stdcall SetSearchPathMode(long) kernelfull.SetSearchPathMode
@ stdcall GetProcessDEPPolicy(ptr ptr ptr)
@ stdcall GetSystemDEPPolicy() 
@ stdcall SetProcessDEPPolicy(long) 

#Onwn implementation only for support
@ stdcall SetFileApisToANSI() ;SetpFileApisToANSI
@ stdcall SetFileApisToOEM() ;SetpFileApisToOEM

#Needed functions for Server 2003 RTM and XP
@ stdcall CreateVirtualBuffer(ptr long long) kernelfull.CreateVirtualBuffer
@ stdcall ExtendVirtualBuffer(ptr) kernelfull.ExtendVirtualBuffer
@ stdcall FreeVirtualBuffer(ptr) kernelfull.FreeVirtualBuffer
@ stdcall TrimVirtualBuffer(ptr) kernelfull.TrimVirtualBuffer
@ stdcall VirtualBufferExceptionHandler(long ptr ptr) kernelfull.VirtualBufferExceptionHandler

#Only for Windows XP
@ stdcall BaseInitAppcompatCache() kernelfull.BaseInitAppcompatCache
@ stdcall BaseCleanupAppcompatCache() kernelfull.BaseCleanupAppcompatCache
@ stdcall CreateProcessInternalWSecure() kernelfull.CreateProcessInternalWSecure
@ stdcall QueryWin31IniFilesMappedToRegistry(long wstr long ptr) kernelfull.QueryWin31IniFilesMappedToRegistry
@ stdcall GetNumaAvailableMemory(ptr long ptr) kernelfull.GetNumaAvailableMemory
@ stdcall GetNumaProcessorMap(ptr long ptr) kernelfull.GetNumaProcessorMap
@ stdcall NumaVirtualQueryNode(long long long long) kernelfull.NumaVirtualQueryNode

; #Vista Functions 
@ stdcall AcquireSRWLockExclusive(ptr) ntext.RtlAcquireSRWLockExclusive
@ stdcall AcquireSRWLockShared(ptr) ntext.RtlAcquireSRWLockShared
@ stdcall ApplicationRecoveryFinished(long)
@ stdcall ApplicationRecoveryInProgress(ptr)
@ stdcall CallbackMayRunLong(ptr)
@ stdcall CancelIoEx(long ptr)
@ stdcall CancelSynchronousIo(ptr)
@ stdcall CheckElevation(wstr long ptr long long)
@ stdcall CheckElevationEnabled(ptr) 
@ stdcall CloseThreadpool(ptr) ntdll.TpReleasePool
@ stdcall CloseThreadpoolCleanupGroup(ptr) ntdll.TpReleaseCleanupGroup
@ stdcall CloseThreadpoolCleanupGroupMembers(ptr long ptr) ntdll.TpReleaseCleanupGroupMembers
@ stdcall CloseThreadpoolTimer(ptr) ntext.TpReleaseTimer
@ stdcall CloseThreadpoolWait(ptr) ntext.TpReleaseWait
@ stdcall CloseThreadpoolWork(ptr) ntext.TpReleaseWork
@ stdcall CompareStringEx(wstr long wstr long wstr long ptr ptr long)
@ stdcall CompareStringOrdinal(wstr long wstr long long)
@ stdcall CopyFileTransactedW(wstr wstr ptr ptr ptr long ptr)
@ stdcall CreateEventExA(ptr str ptr long)
@ stdcall CreateEventExW(ptr wstr ptr long) 
@ stdcall CreateMutexExA(ptr str long long)
@ stdcall CreateMutexExW(ptr wstr long long)
@ stdcall CreateSemaphoreExA(ptr long long str ptr long) 
@ stdcall CreateSemaphoreExW(ptr long long wstr ptr long)  
@ stdcall CreateSymbolicLinkA(str str long)
@ stdcall CreateSymbolicLinkW(wstr wstr long)
@ stdcall CreateThreadpool(ptr)
@ stdcall CreateThreadpoolCleanupGroup()
@ stdcall CreateThreadpoolTimer(ptr ptr ptr)
@ stdcall CreateThreadpoolWait(ptr ptr ptr) 
@ stdcall CreateThreadpoolWork(ptr ptr ptr)
@ stdcall DeleteFileTransactedW(wstr ptr)
@ stdcall DeleteProcThreadAttributeList(ptr)
@ stdcall EnumCalendarInfoExEx(ptr wstr long wstr long long)
@ stdcall EnumCalendarInfoExEx(ptr wstr long wstr long long)
@ stdcall EnumDateFormatsExEx(ptr wstr long ptr)
@ stdcall EnumSystemLocalesEx(ptr long ptr ptr)
@ stdcall EnumTimeFormatsEx(ptr wstr long ptr)
@ stdcall FindNLSString(long long ptr long wstr long ptr)
@ stdcall FindNLSStringEx(wstr long wstr long wstr long ptr ptr ptr ptr)
@ stdcall FlushProcessWriteBuffers()
@ stdcall FreeLibraryWhenCallbackReturns(ptr ptr) ntext.TpCallbackUnloadDllOnCompletion
@ stdcall GetApplicationRestartSettings(ptr wstr ptr ptr)
@ stdcall GetApplicationRestartSettingsWorker(ptr wstr ptr ptr) GetApplicationRestartSettings
@ stdcall GetCalendarInfoEx(wstr long wstr long wstr long ptr) 
@ stdcall GetConsoleHistoryInfo(ptr) 
@ stdcall GetConsoleOriginalTitleA(str long)
@ stdcall GetConsoleOriginalTitleW(wstr long)
@ stdcall GetConsoleScreenBufferInfoEx(ptr ptr)
@ stdcall GetCurrentConsoleFontEx(ptr long ptr) 
@ stdcall GetCurrencyFormatEx(wstr long wstr ptr wstr long)
@ stdcall GetDateFormatEx(wstr long ptr wstr ptr long wstr)
@ stdcall GetDurationFormat(long long ptr long long ptr ptr long) ;need test
@ stdcall GetDurationFormatEx(wstr long ptr long long ptr ptr long) ;need test
@ stdcall GetDynamicTimeZoneInformation(ptr)
@ stdcall GetErrorMode()
@ stdcall GetFileInformationByHandleEx(ptr long ptr long) fileextd.GetFileInformationByHandleEx
@ stdcall GetFileInformationByHandleExW(ptr long ptr long) fileextd.GetFileInformationByHandleEx
@ stdcall GetFinalPathNameByHandleA(ptr str long long)
@ stdcall GetFinalPathNameByHandleW(ptr wstr long long)
@ stdcall GetLocaleInfoEx(wstr long ptr long)
@ stdcall GetFileMUIPath(long wstr wstr ptr wstr ptr ptr)
@ stdcall GetNamedPipeAttribute(ptr long str ptr ptr)
@ stdcall GetNamedPipeClientComputerNameA(ptr str long)
@ stdcall GetNamedPipeClientComputerNameW(ptr wstr long)
@ stdcall GetNamedPipeClientProcessId(ptr ptr)
@ stdcall GetNamedPipeClientSessionId(ptr ptr)
@ stdcall GetNamedPipeServerProcessId(ptr ptr)
@ stdcall GetNamedPipeServerSessionId(ptr ptr)
@ stdcall GetNLSVersionEx(long wstr ptr)
@ stdcall GetNumberFormatEx(wstr long wstr ptr wstr long)
@ stdcall GetOSProductNameA(str long long)
@ stdcall GetOSProductNameW(wstr long long)
@ stdcall GetPhysicallyInstalledSystemMemory(ptr)
@ stdcall GetProductInfo(long long long long ptr) 
@ stdcall GetSystemDefaultLocaleName(ptr long)
@ stdcall GetSystemPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetThreadErrorMode()
@ stdcall GetThreadGroupAffinity(long ptr)
@ stdcall GetThreadInformation(ptr long ptr long)
@ stdcall GetThreadPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetThreadUILanguage()
@ stdcall -ret64 GetTickCount64()
@ stdcall GetTimeFormatEx(wstr long ptr wstr ptr long)
@ stdcall GetTimeZoneInformationForYear(long ptr ptr)
@ stdcall GetUserDefaultLocaleName(wstr long)
;@ stdcall GetUserPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetVolumeInformationW(ptr ptr long ptr ptr ptr ptr long)
@ stdcall GetVolumeInformationByHandleW(ptr ptr long ptr ptr ptr ptr long)
@ stdcall IdnToAscii(long wstr long ptr long) normaliz.IdnToAscii
@ stdcall IdnToUnicode(long wstr long ptr long) normaliz.IdnToUnicode
@ stdcall InitializeProcThreadAttributeList(ptr long long ptr) ;need test
@ stdcall InitializeConditionVariable(ptr) ntext.RtlInitializeConditionVariable
@ stdcall InitializeCriticalSectionEx(ptr long long) 
@ stdcall InitializeSRWLock(ptr) ntext.RtlInitializeSRWLock
@ stdcall InitOnceBeginInitialize(ptr long ptr ptr)
@ stdcall InitOnceComplete(ptr long ptr)
@ stdcall InitOnceExecuteOnce(ptr ptr ptr ptr)
@ stdcall IsThreadAFiber()
@ stdcall IsValidLocaleName(wstr)
@ stdcall LeaveCriticalSectionWhenCallbackReturns(ptr ptr) ntdll.TpCallbackLeaveCriticalSectionOnCompletion
@ stdcall LCMapStringEx(wstr long wstr long ptr long ptr ptr long)
@ stdcall LCIDToLocaleName(long ptr long long)
@ stdcall LocaleNameToLCID(wstr long)
@ stdcall MoveFileTransactedW(wstr wstr ptr ptr long ptr)
@ stdcall PowerClearRequest(ptr ptr)
@ stdcall PowerCreateRequest(ptr)
@ stdcall PowerCreateRequestW(ptr) PowerCreateRequest
@ stdcall PowerSetRequest(ptr long)
@ stdcall OpenFileById(long ptr long long ptr long) fileextd.OpenFileById
@ stdcall QueryFullProcessImageNameA(ptr long ptr ptr) 
@ stdcall QueryFullProcessImageNameW(ptr long ptr ptr)
@ stdcall QueryIdleProcessorCycleTime(ptr ptr)
@ stdcall QueryProcessCycleTime(ptr ptr)
@ stdcall QueryProcessAffinityUpdateMode(ptr long)
@ stdcall QueryThreadCycleTime(ptr ptr)
@ stdcall ReleaseMutexWhenCallbackReturns(ptr long) ntdll.TpCallbackReleaseMutexOnCompletion
@ stdcall ReleaseSemaphoreWhenCallbackReturns(ptr long long) ntdll.TpCallbackReleaseSemaphoreOnCompletion
@ stdcall ReleaseSRWLockExclusive(ptr) ntext.RtlReleaseSRWLockExclusive
@ stdcall ReleaseSRWLockShared(ptr) ntext.RtlReleaseSRWLockShared
@ stdcall ResolveDelayLoadedAPI(ptr ptr ptr ptr ptr long) ntdll.LdrResolveDelayLoadedAPI
@ stdcall SetConsoleHistoryInfo(ptr)
@ stdcall SetConsoleScreenBufferInfoEx(ptr ptr)
@ stdcall SetCurrentConsoleFontEx(ptr long ptr)
@ stdcall SetFileInformationByHandle(long long ptr long) fileextd.SetFileInformationByHandle
@ stdcall SetFileInformationByHandleW(long long ptr long) fileextd.SetFileInformationByHandle
@ stdcall SetNamedPipeAttribute(ptr long str ptr long)
@ stdcall SetProcessAffinityUpdateMode(ptr long)
@ stdcall SetStdHandleEx(long ptr ptr)
@ stdcall SetThreadErrorMode(long ptr)
@ stdcall SetThreadGroupAffinity(long ptr ptr)
@ stdcall SetThreadpoolThreadMaximum(ptr long) ntext.TpSetPoolMaxThreads
@ stdcall SetThreadpoolThreadMinimum(ptr long) ntext.TpSetPoolMinThreads
@ stdcall SetThreadpoolTimer(ptr ptr long long)
@ stdcall SetThreadpoolWait(ptr ptr ptr)
@ stdcall SleepConditionVariableCS(ptr ptr long) 
@ stdcall SleepConditionVariableSRW(ptr ptr long long) 
@ stdcall SubmitThreadpoolWork(ptr) ntext.TpPostWork
@ stdcall TrySubmitThreadpoolCallback(ptr ptr ptr)
@ stdcall UpdateProcThreadAttribute(ptr long long ptr long ptr ptr)
@ stdcall WaitForThreadpoolTimerCallbacks(ptr long) ntext.TpWaitForTimer
@ stdcall WaitForThreadpoolWorkCallbacks(ptr long) ntext.TpWaitForWork
@ stdcall WakeAllConditionVariable(ptr) ntext.RtlWakeAllConditionVariable
@ stdcall WakeConditionVariable(ptr) ntext.RtlWakeConditionVariable
@ stdcall WerpNotifyLoadStringResource(ptr wstr ptr long)
@ stub WerpNotifyLoadStringResourceEx
@ stdcall WerpNotifyUseStringResource(ptr)
@ stdcall WerRegisterFile(wstr long long)
@ stdcall WerRegisterMemoryBlock(ptr long)
@ stdcall WerRegisterMemoryBlockWorker(ptr long) WerRegisterMemoryBlock
@ stdcall WerUnregisterFile(wstr)
@ stdcall WerSetFlags(long)

#Win7 Functions
@ stub BaseThreadInitThunk
@ stdcall CreateRemoteThreadEx(long ptr long ptr long long ptr ptr)
@ stdcall GetCurrentProcessorNumberEx(ptr) ntext.RtlGetCurrentProcessorNumberEx
@ stdcall GetTimeFormatWWorker(long long ptr wstr ptr long) GetTimeFormatW
@ stdcall GetLogicalProcessorInformationEx(long ptr ptr)
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
@ stdcall QueryUnbiasedInterruptTime(ptr)
@ stdcall RaiseFailFastException(ptr ptr long)
@ stdcall RegisterApplicationRecoveryCallback(long ptr long long)
@ stdcall RegisterApplicationRestart(wstr long)
@ stdcall ResolveLocaleName(wstr ptr long)
@ stdcall SetWaitableTimerEx(ptr ptr long ptr ptr ptr long)
@ stdcall TryAcquireSRWLockExclusive(ptr) ntext.RtlTryAcquireSRWLockExclusive
@ stdcall TryAcquireSRWLockShared(ptr) ntext.RtlTryAcquireSRWLockShared

#Win8 Functions 
@ stdcall AddDllDirectory(wstr)
@ stdcall GetCurrentPackageId(ptr ptr)
@ stdcall GetCurrentPackageFamilyName(ptr ptr)
@ stdcall GetCurrentPackageFullName(ptr ptr)
@ stdcall GetPackageFullName(long ptr ptr)
@ stdcall GetSystemTimePreciseAsFileTime(ptr)
@ stdcall RemoveDllDirectory(ptr)
@ stdcall SetDefaultDllDirectories(long)
@ stdcall WerpNotifyLoadStringResourceWorker(ptr wstr ptr long) WerpNotifyLoadStringResource
@ stdcall WerpNotifyUseStringResourceWorker(ptr) WerpNotifyUseStringResource

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
@ stdcall GetSecurityDescriptorLength(ptr) ntext.RtlLengthSecurityDescriptor
@ stdcall GetSecurityDescriptorOwner(ptr ptr ptr) advapi32.GetSecurityDescriptorOwner
@ stdcall GetTokenInformation(long long ptr long ptr) advapi32.GetTokenInformation
@ stdcall ImpersonateLoggedOnUser(long) advapi32.ImpersonateLoggedOnUser
@ stdcall ImpersonateNamedPipeClient(long) advapi32.ImpersonateNamedPipeClient
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
@ stdcall LoadStringA(ptr long ptr long)
@ stdcall LoadStringW(ptr long ptr long)

#API-SET functions
@ stdcall QuirkIsEnabled3(ptr ptr)