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
@ stdcall BaseProcessInitPostImport() kernelex.BaseProcessInitPostImport; missing in Win 7
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
@ stdcall CreateNlsSecurityDescriptor(ptr long long) kernelex.CreateNlsSecurityDescriptor ; missing in Win 7
@ stdcall CreatePipe(ptr ptr ptr long)
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
@ stdcall GetDefaultSortkeySize(ptr) kernelex.GetDefaultSortkeySize ; missing in Win 7
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
@ stdcall GetLinguistLangSize(ptr) kernelex.GetLinguistLangSize ; missing in Win 7
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
@ stdcall GetModuleHandleExA(long str ptr)
@ stdcall GetModuleHandleExW(long wstr ptr)
@ stdcall GetNamedPipeHandleStateA(long ptr ptr ptr ptr str long)
@ stdcall GetNamedPipeHandleStateW(long ptr ptr ptr ptr wstr long)
@ stdcall GetNamedPipeInfo(long ptr ptr ptr ptr)
@ stdcall GetNativeSystemInfo(ptr)
@ stdcall GetNextVDMCommand(long)
@ stdcall GetNlsSectionName(long long long str str long) kernelex.GetNlsSectionName ; missing in Win 7
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
@ stdcall GetTickCount() kernelex.GetTickCount
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
@ stdcall HeapCreateTagsW(long long wstr wstr) kernelex.HeapCreateTagsW ; missing in Win 7
@ stdcall HeapDestroy(long)
@ stdcall HeapExtend(long long ptr long) kernelex.HeapExtend ; missing in Win 7
@ stdcall HeapFree(long long long) ntext.RtlFreeHeap
@ stdcall HeapLock(long)
@ stdcall HeapQueryInformation(long long ptr long ptr)
@ stdcall HeapQueryTagW(long long long long ptr) kernelex.HeapQueryTagW ; missing in Win 7
@ stdcall HeapReAlloc(long long ptr long) ntext.RtlReAllocateHeap
@ stdcall HeapSize(long long ptr) ntext.RtlSizeHeap
@ stdcall HeapSummary(long long ptr)
@ stdcall HeapUnlock(long)
@ stdcall HeapUsage(long long long long ptr) kernelex.HeapUsage ; missing in Win 7
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
@ stdcall IsSystemResumeAutomatic()
@ stdcall IsValidCodePage(long)
@ stdcall IsValidLanguageGroup(long long)
@ stdcall IsValidLocale(long long)
@ stdcall IsValidUILanguage(long) kernelex.IsValidUILanguage ; missing in Win 7
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
@ stdcall NlsResetProcessLocale() kernelex.NlsResetProcessLocale
@ stdcall OpenConsoleW(wstr long long long)
@ stdcall OpenDataFile(long long) kernelex.OpenDataFile ; missing in Win 7
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
@ stdcall SetCPGlobal(long) kernelex.SetCPGlobal ; missing in Win 7
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
@ stdcall SetConsoleCommandHistoryMode(long) kernelex.SetConsoleCommandHistoryMode ; missing in win 7
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
@ stdcall ValidateLCType(long long ptr ptr) kernelex.ValidateLCType
@ stdcall ValidateLocale(long) kernelex.ValidateLocale
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
@ stdcall BaseIsAppcompatInfrastructureDisabled() IsShimInfrastructureDisabled
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
@ stdcall IsNLSDefinedString(long long ptr long long) kernelex.IsNLSDefinedString
@ stdcall SetFileCompletionNotificationModes(ptr long)

#Missing on XP and 2003 RTM
@ stdcall BaseCheckRunApp(long ptr long long long long long long long long) kernelex.BaseCheckRunApp
@ stdcall BasepCheckBadapp(long ptr long long long long long long long) kernelex.BasepCheckBadapp
@ stdcall EnumSystemFirmwareTables(long ptr long) kernelex.EnumSystemFirmwareTables
@ stdcall GetSystemFileCacheSize(ptr ptr ptr) kernelex.GetSystemFileCacheSize
@ stdcall GetSystemFirmwareTable(long long ptr long) kernelex.GetSystemFirmwareTable
 
#Missing on Server 2003 RTM (only available on 2003 SP1 and SP2 and XP SP3)
@ stdcall BaseQueryModuleData(str str ptr ptr ptr) kernelex.BaseQueryModuleData
@ stdcall BasepCheckWinSaferRestrictions(long long long long long long) kernelex.BasepCheckWinSaferRestrictions
@ stdcall BasepFreeAppCompatData(ptr ptr)
@ stdcall SetSystemFileCacheSize(long long long) 
@ stdcall SetThreadStackGuarantee(ptr)

#Native in XP SP3 and Missing in Server 2003 SP2 with Updates
@ stdcall SetSearchPathMode(long)
@ stdcall GetProcessDEPPolicy(ptr ptr ptr)
@ stdcall GetSystemDEPPolicy() 
@ stdcall SetProcessDEPPolicy(long) 

#Own implementation only for support
@ stdcall CreateProcessA(str str ptr ptr long long ptr str ptr ptr) CreateProcessExA
@ stdcall CreateProcessInternalA(ptr str str ptr ptr long long ptr str ptr ptr long) CreateProcessInternalExA
@ stdcall CreateProcessInternalW(ptr wstr wstr ptr ptr long long ptr wstr ptr ptr long) CreateProcessInternalExW
@ stdcall CreateProcessW(wstr wstr ptr ptr long long ptr wstr ptr ptr) CreateProcessExW
@ stdcall GetModuleHandleA(str) GetModuleHandleInternalA
@ stdcall GetModuleHandleW(wstr) GetModuleHandleInternalW
@ stdcall GetProcAddress(long str) GetProcAddressInternal
@ stdcall IsProcessorFeaturePresent(long) IsProcessorFeaturePresentInternal
@ stdcall LoadLibraryA(str) LoadLibraryInternalA
@ stdcall LoadLibraryW(wstr) LoadLibraryInternalW
@ stdcall LoadLibraryExA( str long long) LoadLibraryExInternalA
@ stdcall LoadLibraryExW(wstr long long) LoadLibraryExInternalW
@ stdcall SetFileApisToANSI() SetpFileApisToANSI
@ stdcall SetFileApisToOEM() SetpFileApisToOEM
@ stdcall HeapSetInformation(ptr long ptr long) HeapSetInformationInternal
@ stdcall SetThreadPriority(ptr long) SetThreadPriorityInternal

#Needed functions for Server 2003 RTM and XP
@ stdcall CreateVirtualBuffer(ptr long long) kernelex.CreateVirtualBuffer
@ stdcall ExtendVirtualBuffer(ptr) kernelex.ExtendVirtualBuffer
@ stdcall FreeVirtualBuffer(ptr) kernelex.FreeVirtualBuffer
@ stdcall TrimVirtualBuffer(ptr) kernelex.TrimVirtualBuffer
@ stdcall VirtualBufferExceptionHandler(long ptr ptr) kernelex.VirtualBufferExceptionHandler

#Only for Windows XP
@ stdcall BaseInitAppcompatCache() kernelex.BaseInitAppcompatCache
@ stdcall BaseCleanupAppcompatCache() kernelex.BaseCleanupAppcompatCache
@ stdcall CreateProcessInternalWSecure() kernelex.CreateProcessInternalWSecure
@ stdcall QueryWin31IniFilesMappedToRegistry(long wstr long ptr) kernelex.QueryWin31IniFilesMappedToRegistry
@ stdcall GetNumaAvailableMemory(ptr long ptr) kernelex.GetNumaAvailableMemory
@ stdcall GetNumaProcessorMap(ptr long ptr) kernelex.GetNumaProcessorMap
@ stdcall NumaVirtualQueryNode(long long long long) kernelex.NumaVirtualQueryNode

#Vista Functions 
@ stdcall AcquireSRWLockExclusive(ptr) ntext.RtlAcquireSRWLockExclusive
@ stdcall AcquireSRWLockShared(ptr) ntext.RtlAcquireSRWLockShared
@ stdcall AdjustCalendarDate(ptr long long)
@ stdcall AllocateUserPhysicalPagesNuma(ptr ptr ptr long) 
@ stdcall ApplicationRecoveryFinished(long)
@ stdcall ApplicationRecoveryInProgress(ptr)
@ stdcall CallbackMayRunLong(ptr)
@ stdcall CancelIoEx(long ptr)
@ stdcall CancelSynchronousIo(ptr)
@ stdcall CancelThreadpoolIo(ptr) ntext.TpCancelAsyncIoOperation
@ stdcall CheckElevation(wstr long ptr long long)
@ stdcall CheckElevationEnabled(ptr) 
@ stdcall CloseThreadpool(ptr) ntext.TpReleasePool
@ stdcall CloseThreadpoolCleanupGroup(ptr) ntext.TpReleaseCleanupGroup
@ stdcall CloseThreadpoolCleanupGroupMembers(ptr long ptr) ntext.TpReleaseCleanupGroupMembers
@ stdcall CloseThreadpoolIo(ptr) ntext.TpReleaseIoCompletion
@ stdcall CloseThreadpoolTimer(ptr) ntext.TpReleaseTimer
@ stdcall CloseThreadpoolWait(ptr) ntext.TpReleaseWait
@ stdcall CloseThreadpoolWork(ptr) ntext.TpReleaseWork
@ stdcall CompareCalendarDates(ptr ptr long)
@ stdcall CompareStringEx(wstr long wstr long wstr long ptr ptr long)
@ stdcall CompareStringOrdinal(wstr long wstr long long)
@ stdcall ConvertCalDateTimeToSystemTime(ptr ptr)
@ stdcall ConvertNLSDayOfWeekToWin32DayOfWeek(long) 
@ stdcall ConvertSystemTimeToCalDateTime(ptr long ptr) 
@ stdcall CopyFileTransactedA(str str ptr ptr ptr long ptr)
@ stdcall CopyFileTransactedW(wstr wstr ptr ptr ptr long ptr)
@ stdcall CreateDirectoryTransactedA(str str ptr ptr)
@ stdcall CreateDirectoryTransactedW(wstr wstr ptr ptr)
@ stdcall CreateEventExA(ptr str ptr long)
@ stdcall CreateEventExW(ptr wstr ptr long) 
@ stdcall CreateFileMappingNumaA(ptr ptr long long long str long)
@ stdcall CreateFileMappingNumaW(ptr ptr long long long wstr long)
@ stdcall CreateFileTransactedA(str long long ptr long long ptr ptr ptr ptr)
@ stdcall CreateFileTransactedW(wstr long long ptr long long ptr ptr ptr ptr)
@ stdcall CreateHardLinkTransactedA(str str ptr ptr)
@ stdcall CreateHardLinkTransactedW(wstr wstr ptr ptr)
@ stdcall CreateMutexExA(ptr str long long)
@ stdcall CreateMutexExW(ptr wstr long long)
@ stdcall CreateSemaphoreExA(ptr long long str ptr long) 
@ stdcall CreateSemaphoreExW(ptr long long wstr ptr long)  
@ stdcall CreateSymbolicLinkA(str str long)
@ stdcall CreateSymbolicLinkW(wstr wstr long) ;EXPERIMENTAL
@ stdcall CreateSymbolicLinkTransactedA(str str long ptr)
@ stdcall CreateSymbolicLinkTransactedW(wstr wstr long ptr)
@ stdcall CreateThreadpool(ptr)
@ stdcall CreateThreadpoolCleanupGroup()
@ stdcall CreateThreadpoolIo(ptr)
@ stdcall CreateThreadpoolTimer(ptr ptr ptr)
@ stdcall CreateThreadpoolWait(ptr ptr ptr) 
@ stdcall CreateThreadpoolWork(ptr ptr ptr)
@ stdcall CreateWaitableTimerExA(ptr str ptr long)
@ stdcall CreateWaitableTimerExW(ptr wstr ptr long)
@ stdcall DeleteFileTransactedA(str ptr)
@ stdcall DeleteFileTransactedW(wstr ptr)
@ stdcall DeleteProcThreadAttributeList(ptr)
@ stdcall DisassociateCurrentThreadFromCallback(ptr) ntext.TpDisassociateCallback
@ stdcall DnsHostnameToComputerNameExW(wstr ptr ptr)
@ stdcall EnumCalendarInfoExEx(ptr wstr long wstr long long)
@ stdcall EnumDateFormatsExEx(ptr wstr long ptr)
@ stdcall EnumResourceLanguagesExA(long str str ptr long long long)
@ stdcall EnumResourceLanguagesExW(long wstr wstr ptr long long long)
@ stdcall EnumResourceNamesExA(ptr str ptr long long long)
@ stdcall EnumResourceNamesExW(ptr wstr ptr long long long)
@ stdcall EnumResourceTypesExA(ptr ptr long long long)
@ stdcall EnumResourceTypesExW(ptr ptr long long long)
@ stdcall EnumSystemLocalesEx(ptr long ptr ptr)
@ stdcall EnumTimeFormatsEx(ptr wstr long ptr)
# @ stub FindFirstFileNameW
@ stdcall FindFirstFileTransactedA(str long ptr long ptr long ptr)
@ stdcall FindFirstFileTransactedW(wstr long ptr long ptr long ptr)
# @ stub FindFirstFileNameTransactedW
@ stdcall FindFirstStreamTransactedW(wstr long ptr long ptr)
# @ stub FindNextFileNameW
@ stdcall FindNLSString(long long ptr long wstr long ptr)
@ stdcall FindNLSStringEx(wstr long wstr long wstr long ptr ptr ptr ptr)
@ stdcall FlushProcessWriteBuffers()
@ stdcall FreeLibraryWhenCallbackReturns(ptr ptr) ntext.TpCallbackUnloadDllOnCompletion
# @ stub GetApplicationRecoveryCallback
@ stdcall GetApplicationRestartSettings(ptr wstr ptr ptr)
@ stdcall GetApplicationRestartSettingsWorker(ptr wstr ptr ptr) GetApplicationRestartSettings
@ stdcall GetCalendarDateFormat(ptr long ptr ptr ptr long) ;unimplemented TODO
@ stdcall GetCalendarDateFormatEx(wstr long ptr wstr wstr long) ;unimplemented TODO
@ stdcall GetCalendarDaysInMonth(long long long long)
@ stdcall GetCalendarDifferenceInDays(ptr ptr ptr)
@ stdcall GetCalendarInfoEx(wstr long wstr long wstr long ptr)
@ stdcall GetCalendarMonthsInYear(long long long)
@ stdcall GetCalendarSupportedDateRange(long ptr ptr) 
@ stdcall GetCalendarWeekNumber(ptr ptr ptr ptr) ;is really implemented?
@ stdcall GetClientDynamicTimeZoneInformation(ptr) ;unimplemented TODOS
@ stdcall GetCompressedFileSizeTransactedA(str ptr ptr)
@ stdcall GetCompressedFileSizeTransactedW(wstr ptr ptr)
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
# @ stub GetFileBandwidthReservation
@ stdcall GetErrorMode()
@ stdcall GetFileAttributesTransactedA(str long ptr ptr)
@ stdcall GetFileAttributesTransactedW(wstr long ptr ptr)
@ stdcall GetFileInformationByHandleEx(long long ptr long) ;fileextd.GetFileInformationByHandleEx
@ stdcall GetFileInformationByHandleExW(long long ptr long) GetFileInformationByHandleEx ;fileextd.GetFileInformationByHandleEx 
@ stdcall GetFinalPathNameByHandleA(ptr str long long)
@ stdcall GetFinalPathNameByHandleW(ptr wstr long long)
@ stdcall GetLocaleInfoEx(wstr long ptr long)
@ stdcall GetFileMUIInfo(long wstr ptr ptr)
@ stdcall GetFileMUIPath(long wstr wstr ptr wstr ptr ptr)
@ stdcall GetFullPathNameTransactedA(str long str str ptr)
@ stdcall GetFullPathNameTransactedW(wstr long wstr wstr ptr)
@ stdcall GetLongPathNameTransactedA(str str long ptr)
@ stdcall GetLongPathNameTransactedW(wstr wstr long ptr)
@ stdcall GetNamedPipeAttribute(ptr long str ptr ptr)
@ stdcall GetNamedPipeClientComputerNameA(ptr str long)
@ stdcall GetNamedPipeClientComputerNameW(ptr wstr long)
@ stdcall GetNamedPipeClientProcessId(ptr ptr)
@ stdcall GetNamedPipeClientSessionId(ptr ptr)
@ stdcall GetNamedPipeServerProcessId(ptr ptr)
@ stdcall GetNamedPipeServerSessionId(ptr ptr)
@ stdcall GetNLSVersionEx(long wstr ptr)
@ stdcall GetNumaProximityNode(long ptr)
@ stdcall GetNumberFormatEx(wstr long wstr ptr wstr long)
@ stdcall GetOSProductNameA(str long long)
@ stdcall GetOSProductNameW(wstr long long)
@ stdcall GetOverlappedResultEx(long ptr ptr long long)
@ stdcall GetPhysicallyInstalledSystemMemory(ptr)
@ stdcall GetProcessPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetProductInfo(long long long long ptr) 
@ stdcall GetQueuedCompletionStatusEx(ptr ptr long ptr long long)
@ stdcall GetStringScripts(long wstr long wstr long) idndl.DownlevelGetStringScripts
@ stdcall GetSystemDefaultLocaleName(ptr long)
@ stdcall GetSystemPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetThreadErrorMode()
@ stdcall GetThreadInformation(ptr long ptr long)
@ stdcall GetThreadPreferredUILanguages(long ptr wstr ptr) 
@ stdcall GetThreadUILanguage()
@ stdcall -ret64 GetTickCount64()
@ stdcall GetTimeFormatEx(wstr long ptr wstr ptr long)
@ stdcall GetTimeZoneInformationForYear(long ptr ptr)
@ stdcall GetUserDefaultLocaleName(wstr long)
@ stdcall GetUserPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetVolumeInformationByHandleW(ptr ptr long ptr ptr ptr ptr long)
@ stdcall IdnToAscii(long wstr long ptr long) normaliz.IdnToAscii
@ stdcall IdnToNameprepUnicode(long wstr long ptr long) normaliz.IdnToNameprepUnicode
@ stdcall IdnToUnicode(long wstr long ptr long) normaliz.IdnToUnicode
@ stdcall InitializeProcThreadAttributeList(ptr long long ptr) ;need test
@ stdcall InitializeConditionVariable(ptr) ntext.RtlInitializeConditionVariable
@ stdcall InitializeCriticalSectionEx(ptr long long) ;EXPERIMENTAL
@ stdcall InitializeSRWLock(ptr) ntext.RtlInitializeSRWLock
@ stdcall InitOnceBeginInitialize(ptr long ptr ptr)
@ stdcall InitOnceComplete(ptr long ptr)
@ stdcall InitOnceExecuteOnce(ptr ptr ptr ptr)
@ stdcall InitOnceInitialize(ptr) ntext.RtlRunOnceInitialize
@ stdcall InterlockedPushListSList(ptr ptr ptr long) ntdll.RtlInterlockedPushListSList
@ stdcall IsCalendarLeapDay(long long long long long)
@ stdcall IsCalendarLeapMonth(long long long long)
@ stdcall IsCalendarLeapYear(long long long)
@ stdcall IsNormalizedString(long wstr long) normaliz.IsNormalizedString
@ stdcall IsThreadAFiber()
@ stdcall IsThreadpoolTimerSet(ptr) ntext.TpIsTimerSet
@ stdcall IsValidCalDateTime(ptr long)
@ stdcall IsValidLocaleName(wstr)
@ stdcall LeaveCriticalSectionWhenCallbackReturns(ptr ptr) ntext.TpCallbackLeaveCriticalSectionOnCompletion
@ stdcall LCMapStringEx(wstr long wstr long ptr long ptr ptr long)
@ stdcall LCIDToLocaleName(long ptr long long)
@ stdcall LocaleNameToLCID(wstr long)
@ stdcall MapViewOfFileExNuma(ptr long long long long ptr long)
@ stdcall MoveFileTransactedA(str str ptr ptr long ptr)
@ stdcall MoveFileTransactedW(wstr wstr ptr ptr long ptr)
@ stdcall NormalizeString(long wstr long wstr long) normaliz.NormalizeString
@ stdcall OpenFileById(long ptr long long ptr long) 
@ stdcall QueryActCtxSettingsW(long ptr wstr wstr ptr long ptr)
@ stdcall QueryFullProcessImageNameA(ptr long ptr ptr) 
@ stdcall QueryFullProcessImageNameW(ptr long ptr ptr)
@ stdcall QueryIdleProcessorCycleTime(ptr ptr)
@ stdcall QueryProcessCycleTime(ptr ptr)
@ stdcall QueryProcessAffinityUpdateMode(ptr long)
@ stdcall QueryThreadCycleTime(ptr ptr)
@ stdcall ReleaseMutexWhenCallbackReturns(ptr long) ntext.TpCallbackReleaseMutexOnCompletion
@ stdcall ReleaseSemaphoreWhenCallbackReturns(ptr long long) ntext.TpCallbackReleaseSemaphoreOnCompletion
@ stdcall ReleaseSRWLockExclusive(ptr) ntext.RtlReleaseSRWLockExclusive
@ stdcall ReleaseSRWLockShared(ptr) ntext.RtlReleaseSRWLockShared
@ stdcall RemoveDirectoryTransactedA(str ptr)
@ stdcall RemoveDirectoryTransactedW(wstr ptr)
@ stdcall ResolveDelayLoadedAPI(ptr ptr ptr ptr ptr long) ntext.LdrResolveDelayLoadedAPI
@ stdcall SetConsoleHistoryInfo(ptr)
@ stdcall SetConsoleScreenBufferInfoEx(ptr ptr)
@ stdcall SetCurrentConsoleFontEx(ptr long ptr)
@ stdcall SetDynamicTimeZoneInformation(ptr)
@ stdcall SetEventWhenCallbackReturns(ptr long) ntext.TpCallbackSetEventOnCompletion
@ stdcall SetFileAttributesTransactedA(str long ptr)
@ stdcall SetFileAttributesTransactedW(wstr long ptr)
@ stdcall SetFileInformationByHandle(long long ptr long) ;fileextd.SetFileInformationByHandle
@ stdcall SetFileInformationByHandleW(long long ptr long) SetFileInformationByHandle ;fileextd.SetFileInformationByHandle
@ stdcall SetNamedPipeAttribute(ptr long str ptr long)
@ stdcall SetProcessAffinityUpdateMode(ptr long)
@ stdcall SetProcessPreferredUILanguages(long wstr ptr)
@ stdcall SetStdHandleEx(long ptr ptr)
@ stdcall SetThreadErrorMode(long ptr)
@ stdcall SetThreadpoolThreadMaximum(ptr long) ntext.TpSetPoolMaxThreads
@ stdcall SetThreadpoolThreadMinimum(ptr long) ntext.TpSetPoolMinThreads
@ stdcall SetThreadpoolTimer(ptr ptr long long)
@ stdcall SetThreadpoolWait(ptr ptr ptr)
@ stdcall SetThreadPreferredUILanguages(long wstr ptr)
@ stdcall SleepConditionVariableCS(ptr ptr long) 
@ stdcall SleepConditionVariableSRW(ptr ptr long long)
@ stdcall StartThreadpoolIo(ptr) ntext.TpStartAsyncIoOperation 
@ stdcall SubmitThreadpoolWork(ptr) ntext.TpPostWork
@ stdcall TrySubmitThreadpoolCallback(ptr ptr ptr)
@ stdcall UnregisterApplicationRestart()
@ stdcall UnregisterApplicationRecoveryCallback()
@ stdcall UpdateCalendarDayOfWeek(ptr) 
@ stdcall UpdateProcThreadAttribute(ptr long long ptr long ptr ptr)
@ stdcall VerifyScripts(long wstr long wstr long) idndl.DownlevelVerifyScripts
@ stdcall VirtualAllocExNuma(long ptr long long long long)
@ stdcall WaitForThreadpoolIoCallbacks(ptr long) ntext.TpWaitForIoCompletion
@ stdcall WaitForThreadpoolTimerCallbacks(ptr long) ntext.TpWaitForTimer
@ stdcall WaitForThreadpoolWaitCallbacks(ptr long) ntext.TpWaitForWait
@ stdcall WaitForThreadpoolWorkCallbacks(ptr long) ntext.TpWaitForWork
@ stdcall WakeAllConditionVariable(ptr) ntext.RtlWakeAllConditionVariable
@ stdcall WakeConditionVariable(ptr) ntext.RtlWakeConditionVariable
@ stdcall WerpNotifyLoadStringResource(ptr wstr ptr long)
@ stub WerpNotifyLoadStringResourceEx
@ stdcall WerpNotifyUseStringResource(ptr)
@ stdcall WerRegisterFile(wstr long long)
@ stdcall WerRegisterMemoryBlock(ptr long)
@ stdcall WerRegisterMemoryBlockWorker(ptr long) WerRegisterMemoryBlock
@ stdcall WerUnregisterMemoryBlock(ptr)
@ stdcall WerUnregisterFile(wstr)
@ stdcall WerGetFlags(ptr ptr)
@ stdcall WerSetFlags(long)
@ stdcall Wow64GetThreadContext(ptr ptr)
@ stdcall Wow64SetThreadContext(ptr ptr)
@ stdcall Wow64SuspendThread(ptr)

#Win7 Functions
@ stdcall AddDllDirectory(wstr)
@ stdcall BaseFormatObjectAttributes(ptr ptr ptr)
@ stdcall BasepAllocateActivationContextActivationBlock(long ptr ptr ptr)
@ stdcall BasepAnsiStringToDynamicUnicodeString(wstr str)
@ stdcall BasepFreeActivationContextActivationBlock(ptr)
@ stdcall BaseFormatTimeOut(int64 long)
@ stdcall BaseGetNamedObjectDirectory()
@ stdcall Basep8BitStringToDynamicUnicodeString(wstr str)
@ stdcall BasepMapModuleHandle(ptr long)
@ stdcall BaseSetLastNTError(long)
@ stub BaseThreadInitThunk
@ stdcall CheckForReadOnlyResource(ptr) BasepCheckForReadOnlyResource
@ stdcall CreateRemoteThreadEx(long ptr long ptr long long ptr ptr)
@ stdcall -arch=x86_64 CreateUmsCompletionList(ptr)
@ stdcall -arch=x86_64 CreateUmsThreadContext(ptr)
@ stdcall -arch=x86_64 DeleteUmsCompletionList(ptr)
@ stdcall -arch=x86_64 DeleteUmsThreadContext(ptr)
@ stdcall -arch=x86_64 DequeueUmsCompletionListItems(ptr long ptr)
@ stdcall -arch=x86_64 EnterUmsSchedulingMode(ptr)
@ stdcall -arch=x86_64 ExecuteUmsThread(ptr)
@ stdcall FindStringOrdinal(long wstr long wstr long long)
@ stdcall GetActiveProcessorCount(long)
@ stdcall GetActiveProcessorGroupCount()
@ stdcall GetCurrentProcessorNumberEx(ptr) ntext.RtlGetCurrentProcessorNumberEx
@ stdcall -arch=x86_64 GetCurrentUmsThread()
@ stdcall -ret64 -arch=i386,x86_64 GetEnabledXStateFeatures()
@ stub GetEraNameCountedString
@ stdcall GetMaximumProcessorCount(long)
@ stdcall GetMaximumProcessorGroupCount()
@ stdcall GetTimeFormatWWorker(long long ptr wstr ptr long) GetTimeFormatW
@ stdcall GetLogicalProcessorInformationEx(long ptr ptr)
@ stdcall -arch=x86_64 GetNextUmsListItem(ptr)
@ stdcall GetNumaAvailableMemoryNodeEx(long ptr)
@ stdcall GetNumaNodeProcessorMaskEx(long ptr)
@ stdcall GetNumaProcessorNodeEx(ptr ptr)
@ stdcall GetNumaProximityNodeEx(long ptr)
@ stdcall GetThreadGroupAffinity(long ptr)
@ stdcall GetThreadIdealProcessorEx(ptr ptr)
@ stdcall -arch=x86_64 GetUmsCompletionListEvent(ptr ptr)
# @ stub GetXStateFeaturesMask
@ stub LoadStringBaseExW
@ stub LoadStringByReference
# @ stub LoadStringBaseW
# @ stub LocateXStateFeature
@ stdcall K32EmptyWorkingSet(ptr) EmptyWorkingSet
@ stdcall K32EnumDeviceDrivers(ptr long ptr) EnumDeviceDrivers
@ stdcall K32EnumPageFilesA(ptr ptr) EnumPageFilesA
@ stdcall K32EnumPageFilesW(ptr ptr) EnumPageFilesW
@ stdcall K32EnumProcessModules(ptr ptr long ptr) EnumProcessModules
@ stdcall K32EnumProcessModulesEx(ptr ptr long ptr long)
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
@ stdcall LoadAppInitDlls()
@ stdcall PowerClearRequest(ptr ptr)
@ stdcall PowerCreateRequest(ptr)
@ stdcall PowerCreateRequestW(ptr) PowerCreateRequest
@ stdcall PowerSetRequest(ptr long)
@ stdcall QueryIdleProcessorCycleTimeEx(long ptr ptr)
@ stdcall QueryThreadpoolStackInformation(ptr ptr)
@ stdcall -arch=x86_64 QueryUmsThreadInformation(ptr long ptr long ptr)
@ stdcall QueryUnbiasedInterruptTime(ptr)
@ stdcall RaiseFailFastException(ptr ptr long)
@ stdcall RegisterApplicationRecoveryCallback(long ptr long long)
@ stdcall RegisterApplicationRestart(wstr long)
@ stdcall RemoveDllDirectory(ptr)
@ stdcall ResolveLocaleName(wstr ptr long)
@ stdcall SetDefaultDllDirectories(long)
@ stdcall SetThreadGroupAffinity(long ptr ptr)
@ stdcall SetThreadIdealProcessorEx(ptr ptr ptr)
@ stdcall SetThreadpoolStackInformation(ptr ptr)
@ stdcall -arch=x86_64 SetUmsThreadInformation(ptr long ptr long)
@ stdcall SetWaitableTimerEx(ptr ptr long ptr ptr ptr long)
@ stdcall SystemTimeToTzSpecificLocalTimeEx(ptr ptr ptr)
@ stdcall TryAcquireSRWLockExclusive(ptr) ntext.RtlTryAcquireSRWLockExclusive
@ stdcall TryAcquireSRWLockShared(ptr) ntext.RtlTryAcquireSRWLockShared
@ stdcall TzSpecificLocalTimeToSystemTimeEx(ptr ptr ptr)
@ stdcall WerRegisterRuntimeExceptionModule(wstr ptr)
@ stdcall WerUnregisterRuntimeExceptionModule(wstr ptr)
@ stdcall Wow64GetThreadSelectorEntry(ptr long ptr)
@ stdcall -arch=x86_64 UmsThreadYield(ptr)

#Win8 Functions 
@ stdcall AppPolicyGetProcessTerminationMethod(ptr ptr) 
@ stdcall AppPolicyGetShowDeveloperDiagnostic(ptr ptr)
@ stdcall AppPolicyGetThreadInitializationType(ptr ptr) 
@ stdcall AppPolicyGetWindowingModel(ptr ptr)
@ stdcall CreateFile2(wstr long long long ptr)
@ stdcall DeleteSynchronizationBarrier(ptr)
@ stdcall EnterSynchronizationBarrier(ptr long)
@ stdcall GetCurrentPackageId(ptr ptr)
@ stdcall GetCurrentPackageFamilyName(ptr ptr)
@ stdcall GetCurrentPackageFullName(ptr ptr)
@ stdcall GetCurrentThreadStackLimits(ptr ptr)
@ stdcall GetPackageFullName(long ptr ptr)
@ stdcall GetProcessMitigationPolicy(long long ptr long)
@ stdcall GetSystemTimePreciseAsFileTime(ptr)
@ stdcall GlobalAddAtomExA(str long)
@ stdcall GlobalAddAtomExW(wstr long)
@ stdcall InitializeSynchronizationBarrier(ptr long long)
@ stdcall IsValidNLSVersion(long wstr ptr)
@ stdcall LoadPackagedLibrary(wstr long) 
@ stdcall PathCchAddBackslash(wstr long)
@ stdcall PathCchAddBackslashEx(wstr long ptr ptr) 
@ stdcall PathCchAddExtension(wstr long wstr)
@ stdcall PathCchFindExtension(wstr long ptr)
@ stdcall PathCchAppend(wstr long wstr) kernelbase.PathCchAppend
@ stdcall PathCchAppendEx(wstr long wstr long) kernelbase.PathCchAppendEx
@ stub PathCchCanonicalize
@ stub PathCchCanonicalizeEx
@ stdcall PathCchCombine(ptr long ptr ptr)
@ stdcall PathCchCombineEx(ptr long ptr ptr long)
@ stdcall PathCchRemoveBackslash(wstr long)
@ stdcall PathCchRemoveBackslashEx(wstr long ptr ptr)
@ stdcall PathCchRenameExtension(wstr long wstr)
@ stdcall PathCchRemoveExtension(wstr long)
@ stdcall SetProcessMitigationPolicy(long ptr long)
@ stdcall WaitOnAddress(ptr ptr long long)
@ stdcall WakeByAddressAll(ptr) ntext.RtlWakeAddressAll
@ stdcall WakeByAddressSingle(ptr) ntext.RtlWakeAddressSingle
@ stdcall WerpNotifyLoadStringResourceWorker(ptr wstr ptr long) WerpNotifyLoadStringResource
@ stdcall WerpNotifyUseStringResourceWorker(ptr) WerpNotifyUseStringResource
@ stdcall EventSetInformation(int64 long ptr long) ntext.EtwEventSetInformation
@ stdcall PerfCreateInstance(long ptr wstr long) 
@ stdcall PerfDeleteInstance(long ptr)
@ stdcall PerfSetCounterRefValue(long ptr long ptr)
@ stdcall PerfSetCounterSetInfo(long ptr long)
@ stdcall PerfStartProvider(ptr ptr ptr)
@ stdcall PerfStartProviderEx(ptr ptr ptr)
@ stdcall PerfStopProvider(long)

; #Win10 functions
; @ stdcall GetThreadDescription(long ptr)
; @ stdcall SetThreadDescription(long ptr)

#Import from advapibase or registry function
@ stdcall AccessCheck(ptr long long ptr ptr ptr ptr ptr) advapibase.AccessCheck
@ stdcall AccessCheckAndAuditAlarmW(wstr ptr wstr wstr ptr long ptr long ptr ptr ptr) advapibase.AccessCheckAndAuditAlarmW
@ stdcall AccessCheckByType(ptr ptr long long ptr long ptr ptr ptr ptr ptr) advapibase.AccessCheckByType
@ stdcall AccessCheckByTypeAndAuditAlarmW(wstr ptr wstr wstr ptr ptr long long long ptr long ptr long ptr ptr ptr) advapibase.AccessCheckByTypeAndAuditAlarmW
@ stdcall AccessCheckByTypeResultList(ptr ptr ptr long ptr long ptr ptr ptr ptr ptr) advapibase.AccessCheckByTypeResultList
@ stdcall AccessCheckByTypeResultListAndAuditAlarmByHandleW(wstr ptr ptr wstr wstr ptr long long long long ptr long ptr long ptr ptr ptr) advapibase.AccessCheckByTypeResultListAndAuditAlarmByHandleW
@ stdcall AccessCheckByTypeResultListAndAuditAlarmW(wstr ptr wstr wstr ptr long long long long ptr long ptr long ptr ptr ptr) advapibase.AccessCheckByTypeResultListAndAuditAlarmW
@ stdcall AddAccessAllowedAce(ptr long long ptr) advapibase.AddAccessAllowedAce
@ stdcall AddAccessAllowedAceEx(ptr long long long ptr) advapibase.AddAccessAllowedAceEx
@ stdcall AddAccessAllowedObjectAce(ptr long long long ptr ptr ptr) advapibase.AddAccessAllowedObjectAce
@ stdcall AddAccessDeniedAce(ptr long long ptr) advapibase.AddAccessDeniedAce
@ stdcall AddAccessDeniedAceEx(ptr long long long ptr) advapibase.AddAccessDeniedAceEx
@ stdcall AddAccessDeniedObjectAce(ptr long long long ptr ptr ptr) advapibase.AddAccessDeniedObjectAce
@ stdcall AddAce(ptr long long ptr long) advapibase.AddAce
@ stdcall AddAuditAccessAce(ptr long long ptr long long) advapibase.AddAuditAccessAce
@ stdcall AddAuditAccessAceEx(ptr long long long ptr long long) advapibase.AddAuditAccessAceEx
@ stdcall AddAuditAccessObjectAce(ptr long long long ptr ptr ptr long long) advapibase.AddAuditAccessObjectAce
@ stdcall AddMandatoryAce(ptr long long long ptr)
@ stdcall AdjustTokenGroups(long long ptr long ptr ptr) advapibase.AdjustTokenGroups
@ stdcall AdjustTokenPrivileges(long long ptr long ptr ptr) advapibase.AdjustTokenPrivileges
@ stdcall AllocateAndInitializeSid(ptr long long long long long long long long long ptr) advapibase.AllocateAndInitializeSid
@ stdcall AllocateLocallyUniqueId(ptr) advapibase.AllocateLocallyUniqueId
@ stdcall AreAllAccessesGranted(long long) advapibase.AreAllAccessesGranted
@ stdcall AreAnyAccessesGranted(long long) advapibase.AreAnyAccessesGranted
@ stdcall CheckTokenMembership(long ptr ptr) advapibase.CheckTokenMembership
@ stdcall ConvertToAutoInheritPrivateObjectSecurity(ptr ptr ptr ptr long ptr) advapibase.ConvertToAutoInheritPrivateObjectSecurity
@ stdcall CopySid(long ptr ptr) advapibase.CopySid
@ stdcall CreatePrivateObjectSecurity(ptr ptr ptr long long ptr) advapibase.CreatePrivateObjectSecurity
@ stdcall CreatePrivateObjectSecurityEx(ptr ptr ptr ptr long long long ptr) advapibase.CreatePrivateObjectSecurityEx
@ stdcall CreatePrivateObjectSecurityWithMultipleInheritance(ptr ptr ptr ptr long long long long ptr) advapibase.CreatePrivateObjectSecurityWithMultipleInheritance
@ stdcall CreateRestrictedToken(long long long ptr long ptr long ptr ptr) advapibase.CreateRestrictedToken
@ stdcall CreateProcessAsUserA(long str str ptr ptr long long ptr str ptr ptr) advapibase.CreateProcessAsUserA
@ stdcall CreateProcessAsUserW(long wstr wstr ptr ptr long long ptr wstr ptr ptr) advapibase.CreateProcessAsUserW
@ stdcall CreateWellKnownSid(long ptr ptr ptr) advapibase.CreateWellKnownSid
@ stdcall DeleteAce(ptr long) advapibase.DeleteAce
@ stdcall DestroyPrivateObjectSecurity(ptr) advapibase.DestroyPrivateObjectSecurity
@ stdcall DuplicateToken(long long ptr) advapibase.DuplicateToken
@ stdcall DuplicateTokenEx(long long ptr long long ptr) advapibase.DuplicateTokenEx
@ stdcall EqualPrefixSid(ptr ptr) advapibase.EqualPrefixSid
@ stdcall EqualSid(ptr ptr) advapibase.EqualSid
@ stdcall EventActivityIdControl(long ptr) ntext.EtwEventActivityIdControl
@ stdcall EventEnabled(int64 ptr) ntext.EtwEventEnabled
@ stdcall EventProviderEnabled(int64 long int64) ntext.EtwEventProviderEnabled
@ stdcall EventRegister(ptr ptr ptr ptr) ntext.EtwEventRegister
@ stdcall EventUnregister(int64) ntext.EtwEventUnregister
@ stdcall EventWrite(int64 ptr long ptr) ntext.EtwEventWrite
@ stdcall EventWriteString(int64 long int64 ptr) ntext.EtwEventWriteString
@ stdcall EventWriteTransfer(int64 ptr ptr ptr long ptr) ntext.EtwEventWriteTransfer
@ stdcall FindFirstFreeAce(ptr ptr) advapibase.FindFirstFreeAce
@ stdcall FreeSid(ptr) advapibase.FreeSid
@ stdcall GetAce(ptr long ptr) advapibase.GetAce
@ stdcall GetAclInformation(ptr ptr long long) advapibase.GetAclInformation
@ stdcall GetFileSecurityA(str long ptr long ptr) advapibase.GetFileSecurityA
@ stdcall GetFileSecurityW(wstr long ptr long ptr) advapibase.GetFileSecurityW
@ stdcall GetLengthSid(ptr) advapibase.GetLengthSid
@ stdcall GetPrivateObjectSecurity(ptr long ptr long ptr) advapibase.GetPrivateObjectSecurity
@ stdcall GetSecurityDescriptorControl(ptr ptr ptr) advapibase.GetSecurityDescriptorControl
@ stdcall GetSecurityDescriptorDacl(ptr ptr ptr ptr) advapibase.GetSecurityDescriptorDacl
@ stdcall GetSecurityDescriptorLength(ptr) ntext.RtlLengthSecurityDescriptor
@ stdcall GetSecurityDescriptorOwner(ptr ptr ptr) advapibase.GetSecurityDescriptorOwner
@ stdcall GetSecurityDescriptorSacl(ptr ptr ptr ptr) advapibase.GetSecurityDescriptorSacl
@ stdcall GetSidIdentifierAuthority(ptr) advapibase.GetSidIdentifierAuthority
@ stdcall GetSidLengthRequired(long) advapibase.GetSidLengthRequired
@ stdcall GetSidSubAuthority(ptr long) advapibase.GetSidSubAuthority
@ stdcall GetSidSubAuthorityCount(ptr) advapibase.GetSidSubAuthorityCount
@ stdcall GetTokenInformation(long long ptr long ptr) advapibase.GetTokenInformation
@ stdcall GetTraceEnableFlags(int64) advapibase.GetTraceEnableFlags
@ stdcall GetTraceEnableLevel(int64) advapibase.GetTraceEnableLevel
@ stdcall -ret64 GetTraceLoggerHandle(ptr) advapibase.GetTraceLoggerHandle
@ stdcall GetWindowsAccountDomainSid(ptr ptr ptr) advapibase.GetWindowsAccountDomainSid
@ stdcall ImpersonateAnonymousToken(long) advapibase.ImpersonateAnonymousToken
@ stdcall ImpersonateLoggedOnUser(long) advapibase.ImpersonateLoggedOnUser
@ stdcall ImpersonateNamedPipeClient(long) advapibase.ImpersonateNamedPipeClient
@ stdcall ImpersonateSelf(long) advapibase.ImpersonateSelf
@ stdcall InitializeAcl(ptr long long) advapibase.InitializeAcl
@ stdcall InitializeSid(ptr ptr long) advapibase.InitializeSid
@ stdcall IsTokenRestricted(long) advapibase.IsTokenRestricted
@ stdcall IsValidAcl(ptr) advapibase.IsValidAcl
@ stdcall InitializeSecurityDescriptor(ptr long) advapibase.InitializeSecurityDescriptor
@ stdcall IsWellKnownSid(ptr long) advapibase.IsWellKnownSid
@ stdcall IsValidSecurityDescriptor(ptr) advapibase.IsValidSecurityDescriptor
@ stdcall IsValidSid(ptr) advapibase.IsValidSid
@ stdcall MakeAbsoluteSD(ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr) advapibase.MakeAbsoluteSD
@ stdcall MakeSelfRelativeSD(ptr ptr ptr) advapibase.MakeSelfRelativeSD
@ stdcall MapGenericMask(ptr ptr) advapibase.MapGenericMask
@ stdcall ObjectCloseAuditAlarmW(wstr ptr long) advapibase.ObjectCloseAuditAlarmW
@ stdcall ObjectDeleteAuditAlarmW(wstr ptr long) advapibase.ObjectDeleteAuditAlarmW
@ stdcall ObjectOpenAuditAlarmW(wstr ptr wstr wstr ptr long long long ptr long long ptr) advapibase.ObjectOpenAuditAlarmW
@ stdcall ObjectPrivilegeAuditAlarmW(wstr ptr long long ptr long) advapibase.ObjectPrivilegeAuditAlarmW
@ stdcall PrivilegeCheck(ptr ptr ptr) advapibase.PrivilegeCheck
@ stdcall PrivilegedServiceAuditAlarmW(wstr wstr long ptr long) advapibase.PrivilegedServiceAuditAlarmW
@ stdcall OpenProcessToken(ptr long ptr) advapibase.OpenProcessToken
@ stdcall OpenThreadToken(ptr long long ptr) advapibase.OpenThreadToken
@ stdcall RegCloseKey(ptr) advapibase.RegCloseKey 
@ stdcall RegCopyTreeW(ptr str ptr)
@ stdcall RegCreateKeyExA(long str long ptr long long ptr ptr ptr) advapibase.RegCreateKeyExA 
@ stdcall RegCreateKeyExW(long wstr long ptr long long ptr ptr ptr) advapibase.RegCreateKeyExW 
@ stdcall RegDeleteKeyValueA(long str str)
@ stdcall RegDeleteKeyValueW(long wstr wstr) advapibase.RegDeleteKeyValueW
@ stdcall RegDeleteKeyExA(long str long long) advapibase.RegDeleteKeyExA 
@ stdcall RegDeleteKeyExW(long wstr long long) advapibase.RegDeleteKeyExW 
@ stdcall RegDeleteTreeA(long str)
@ stdcall RegDeleteTreeW(long wstr)
@ stdcall RegDeleteValueA(long str) advapibase.RegDeleteValueA
@ stdcall RegDeleteValueW(long wstr) advapibase.RegDeleteValueW 
@ stdcall RegDisablePredefinedCacheEx()
@ stdcall RegEnumKeyExA(long long ptr ptr ptr ptr ptr ptr) advapibase.RegEnumKeyExA 
@ stdcall RegEnumKeyExW(long long ptr ptr ptr ptr ptr ptr) advapibase.RegEnumKeyExW 
@ stdcall RegEnumValueA(long long ptr ptr ptr ptr ptr ptr) advapibase.RegEnumValueA 
@ stdcall RegEnumValueW(long long ptr ptr ptr ptr ptr ptr) advapibase.RegEnumValueW 
@ stdcall RegFlushKey(long) advapibase.RegFlushKey 
@ stdcall RegGetKeySecurity(long long ptr ptr) advapibase.RegGetKeySecurity 
@ stdcall RegGetValueA(long str str long ptr ptr ptr) advapibase.RegGetValueA 
@ stdcall RegGetValueW(long wstr wstr long ptr ptr ptr) advapibase.RegGetValueW
@ stdcall RegLoadAppKeyA(str ptr long long long)
@ stdcall RegLoadAppKeyW(wstr ptr long long long)
@ stdcall RegLoadKeyA(long str str) advapibase.RegLoadKeyA 
@ stdcall RegLoadKeyW(long wstr wstr) advapibase.RegLoadKeyW 
@ stdcall RegLoadMUIStringA(long str str long ptr long str)
@ stdcall RegLoadMUIStringW(long wstr wstr long ptr long wstr)
@ stdcall RegNotifyChangeKeyValue(long long long long long) advapibase.RegNotifyChangeKeyValue 
@ stdcall RegOpenCurrentUser(long ptr) advapibase.RegOpenCurrentUser 
@ stdcall RegOpenKeyExA(long str long long ptr) advapibase.RegOpenKeyExA 
@ stdcall RegOpenKeyExW(long wstr long long ptr) advapibase.RegOpenKeyExW 
@ stdcall RegOpenUserClassesRoot(ptr long long ptr) advapibase.RegOpenUserClassesRoot 
@ stdcall RegQueryInfoKeyA(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr) advapibase.RegQueryInfoKeyA 
@ stdcall RegQueryInfoKeyW(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr) advapibase.RegQueryInfoKeyW 
@ stdcall RegQueryValueExA(long str ptr ptr ptr ptr) advapibase.RegQueryValueExA
@ stdcall RegQueryValueExW(long wstr ptr ptr ptr ptr) advapibase.RegQueryValueExW 
@ stdcall RegRestoreKeyA(long str long) advapibase.RegRestoreKeyA 
@ stdcall RegRestoreKeyW(long wstr long) advapibase.RegRestoreKeyW 
@ stdcall RegSaveKeyExA(ptr str ptr long) advapibase.RegSaveKeyExA 
@ stdcall RegSaveKeyExW(ptr str ptr long) advapibase.RegSaveKeyExW 
@ stdcall RegSetKeySecurity(long long ptr) advapibase.RegSetKeySecurity 
@ stdcall RegSetKeyValueA(long str str long ptr long)
@ stdcall RegSetKeyValueW(long wstr wstr long ptr long)
@ stdcall RegSetValueExA(long str long long ptr long) advapibase.RegSetValueExA 
@ stdcall RegSetValueExW(long wstr long long ptr long) advapibase.RegSetValueExW 
@ stdcall RegUnLoadKeyA(long str) advapibase.RegUnLoadKeyA 
@ stdcall RegUnLoadKeyW(long wstr) advapibase.RegUnLoadKeyW
@ stdcall RevertToSelf() advapibase.RevertToSelf
@ stdcall RegisterTraceGuidsW(ptr ptr ptr long ptr wstr wstr ptr) advapibase.RegisterTraceGuidsW
@ stdcall SetAclInformation(ptr ptr long long) advapibase.SetAclInformation
@ stdcall SetFileSecurityA(str long ptr) advapibase.SetFileSecurityA
@ stdcall SetFileSecurityW(wstr long ptr) advapibase.SetFileSecurityW
@ stdcall SetKernelObjectSecurity(long long ptr) advapibase.SetKernelObjectSecurity
@ stdcall SetPrivateObjectSecurity(long ptr ptr ptr long) advapibase.SetPrivateObjectSecurity
@ stdcall SetSecurityDescriptorControl(ptr long long) advapibase.SetSecurityDescriptorControl
@ stdcall SetSecurityDescriptorDacl(ptr long ptr long) advapibase.SetSecurityDescriptorDacl
@ stdcall SetSecurityDescriptorGroup(ptr ptr long) advapibase.SetSecurityDescriptorGroup
@ stdcall SetSecurityDescriptorOwner(ptr ptr long) advapibase.SetSecurityDescriptorOwner
@ stdcall SetSecurityDescriptorSacl(ptr long ptr long) advapibase.SetSecurityDescriptorSacl
@ stdcall SetThreadToken(ptr ptr) advapibase.SetThreadToken
@ stdcall SetTokenInformation(long long ptr long) advapibase.SetTokenInformation
@ stdcall TraceEvent(int64 ptr) advapibase.TraceEvent
@ varargs TraceMessage(int64 long ptr long) advapibase.TraceMessage
@ stdcall TraceMessageVa(int64 long ptr long ptr) advapibase.TraceMessageVa
@ stdcall UnregisterTraceGuids(int64) advapibase.UnregisterTraceGuids

#Import from Version
@ stdcall VerQueryValueA(ptr str ptr ptr)
@ stdcall VerQueryValueW(ptr wstr ptr ptr)
@ stdcall GetFileVersionInfoA(str long long ptr)
;@ stdcall GetFileVersionInfoByHandle(str ptr ptr long)
@ stdcall GetFileVersionInfoExA(long str long long ptr)
@ stdcall GetFileVersionInfoExW(long wstr long long ptr)
@ stdcall GetFileVersionInfoSizeA(str ptr)
@ stdcall GetFileVersionInfoSizeExA(long str ptr)
@ stdcall GetFileVersionInfoSizeExW(long wstr ptr)
@ stdcall GetFileVersionInfoSizeW(wstr ptr)
@ stdcall GetFileVersionInfoW(wstr long long ptr)

#Import from user32
@ stdcall CharLowerA(str) user32.CharLowerA
@ stdcall CharLowerBuffA(str long) user32.CharLowerBuffA
@ stdcall CharLowerBuffW(wstr long) user32.CharLowerBuffW
@ stdcall CharLowerW(wstr) user32.CharLowerW
@ stdcall CharNextA(str) user32.CharNextA
@ stdcall CharNextExA(long str long) user32.CharNextExA
@ stdcall CharNextW(wstr) user32.CharNextW
@ stdcall CharPrevA(str str) user32.CharPrevA
@ stdcall CharPrevExA(long str str long) user32.CharPrevExA
@ stdcall CharPrevW(wstr wstr) user32.CharPrevW
@ stdcall CharUpperA(str) user32.CharUpperA
@ stdcall CharUpperBuffA(str long) user32.CharUpperBuffA
@ stdcall CharUpperBuffW(wstr long) user32.CharUpperBuffW
@ stdcall CharUpperW(wstr) user32.CharUpperW
@ stdcall IsCharAlphaA(long) user32.IsCharAlphaA
@ stdcall IsCharAlphaNumericA(long) user32.IsCharAlphaNumericA
@ stdcall IsCharAlphaNumericW(long) user32.IsCharAlphaNumericW
@ stdcall IsCharAlphaW(long) user32.IsCharAlphaW
@ stdcall IsCharLowerA(long) user32.IsCharLowerA
@ stdcall IsCharLowerW(long) user32.IsCharLowerW
@ stdcall IsCharUpperA(long) user32.IsCharUpperA
@ stdcall IsCharUpperW(long) user32.IsCharUpperW
@ stdcall LoadStringA(ptr long ptr long)
@ stdcall LoadStringW(ptr long ptr long)

#API-SET functions
@ stdcall QuirkIsEnabled3(ptr ptr)

#Import from shlwapi
@ stdcall ChrCmpIA(long long) shlwapi.ChrCmpIA
@ stdcall ChrCmpIW(long long) shlwapi.ChrCmpIW
@ stdcall GetAcceptLanguagesA(ptr ptr) shlwapi.GetAcceptLanguagesA
@ stdcall GetAcceptLanguagesW(ptr ptr) shlwapi.GetAcceptLanguagesW
@ stdcall HashData(ptr long ptr long) shlwapi.HashData
@ stdcall IsCharBlankW(long) shlwapi.IsCharBlankW
@ stdcall IsCharCntrlW(ptr) shlwapi.IsCharCntrlW
@ stdcall IsCharDigitW(long) shlwapi.IsCharDigitW
@ stdcall IsCharPunctW(long) shlwapi.IsCharPunctW
@ stdcall IsCharSpaceA(long) shlwapi.IsCharSpaceA
@ stdcall IsCharSpaceW(long) shlwapi.IsCharSpaceW
@ stdcall IsCharXDigitW(long) shlwapi.IsCharXDigitW
@ stdcall IsInternetESCEnabled() shlwapi.IsInternetESCEnabled
@ stdcall PathCombineA(ptr str str) shlwapi.PathCombineA
@ stdcall PathCombineW(ptr wstr wstr) shlwapi.PathCombineW
@ stdcall PathCommonPrefixA(str str ptr) shlwapi.PathCommonPrefixA
@ stdcall PathCommonPrefixW(wstr wstr ptr) shlwapi.PathCommonPrefixW
@ stdcall PathCreateFromUrlA(str ptr ptr long) shlwapi.PathCreateFromUrlA
@ stdcall PathCreateFromUrlAlloc(wstr ptr long)
@ stdcall PathCreateFromUrlW(wstr ptr ptr long) shlwapi.PathCreateFromUrlW
@ stdcall PathFileExistsA(str) shlwapi.PathFileExistsA
@ stdcall PathFileExistsW(wstr) shlwapi.PathFileExistsW
@ stdcall PathFindExtensionA(str) shlwapi.PathFindExtensionA
@ stdcall PathFindExtensionW(wstr) shlwapi.PathFindExtensionW
@ stdcall PathFindFileNameA(str) shlwapi.PathFindFileNameA
@ stdcall PathFindFileNameW(wstr) shlwapi.PathFindFileNameW
@ stdcall PathFindNextComponentA(str) shlwapi.PathFindNextComponentA
@ stdcall PathFindNextComponentW(wstr) shlwapi.PathFindNextComponentW
@ stdcall PathGetArgsA(str) shlwapi.PathGetArgsA
@ stdcall PathGetArgsW(wstr) shlwapi.PathGetArgsW
@ stdcall PathGetCharTypeA(long) shlwapi.PathGetCharTypeA
@ stdcall PathGetCharTypeW(long) shlwapi.PathGetCharTypeW
@ stdcall PathGetDriveNumberA(str) shlwapi.PathGetDriveNumberA
@ stdcall PathGetDriveNumberW(wstr) shlwapi.PathGetDriveNumberW
@ stdcall PathIsFileSpecA(str) shlwapi.PathIsFileSpecA
@ stdcall PathIsFileSpecW(wstr) shlwapi.PathIsFileSpecW
@ stdcall PathIsLFNFileSpecA(str) shlwapi.PathIsLFNFileSpecA
@ stdcall PathIsLFNFileSpecW(wstr) shlwapi.PathIsLFNFileSpecW
@ stdcall PathIsPrefixA(str str) shlwapi.PathIsPrefixA
@ stdcall PathIsPrefixW(wstr wstr) shlwapi.PathIsPrefixW
@ stdcall PathIsRelativeA(str) shlwapi.PathIsRelativeA
@ stdcall PathIsRelativeW(wstr) shlwapi.PathIsRelativeW
@ stdcall PathIsRootA(str) shlwapi.PathIsRootA
@ stdcall PathIsRootW(wstr) shlwapi.PathIsRootW
@ stdcall PathIsSameRootA(str str) shlwapi.PathIsSameRootA
@ stdcall PathIsSameRootW(wstr wstr) shlwapi.PathIsSameRootW
@ stdcall PathIsUNCA(str) shlwapi.PathIsUNCA
# @ stub PathIsUNCEx
@ stdcall PathIsUNCServerA(str) shlwapi.PathIsUNCServerA
@ stdcall PathIsUNCServerShareA(str) shlwapi.PathIsUNCServerShareA
@ stdcall PathIsUNCServerShareW(wstr) shlwapi.PathIsUNCServerShareW
@ stdcall PathIsUNCServerW(wstr) shlwapi.PathIsUNCServerW
@ stdcall PathIsUNCW(wstr) shlwapi.PathIsUNCW
@ stdcall PathIsURLA(str) shlwapi.PathIsURLA
@ stdcall PathIsURLW(wstr) shlwapi.PathIsURLW
@ stdcall PathIsValidCharA(long long) shlwapi.PathIsValidCharA
@ stdcall PathIsValidCharW(long long) shlwapi.PathIsValidCharW
@ stdcall PathMatchSpecA(str str) shlwapi.PathMatchSpecA
# @ stub PathMatchSpecExA
# @ stub PathMatchSpecExW
@ stdcall PathMatchSpecW(wstr wstr) shlwapi.PathMatchSpecW
@ stdcall PathParseIconLocationA(str) shlwapi.PathParseIconLocationA
@ stdcall PathParseIconLocationW(wstr) shlwapi.PathParseIconLocationW
@ stdcall PathQuoteSpacesA(str) shlwapi.PathQuoteSpacesA
@ stdcall PathQuoteSpacesW(wstr) shlwapi.PathQuoteSpacesW
@ stdcall PathRelativePathToA(ptr str long str long) shlwapi.PathRelativePathToA
@ stdcall PathRelativePathToW(ptr wstr long wstr long) shlwapi.PathRelativePathToW
@ stdcall PathRemoveBackslashA(str) shlwapi.PathRemoveBackslashA
@ stdcall PathRemoveBackslashW(wstr) shlwapi.PathRemoveBackslashW
@ stdcall PathRemoveBlanksA(str) shlwapi.PathRemoveBlanksA
@ stdcall PathRemoveBlanksW(wstr) shlwapi.PathRemoveBlanksW
@ stdcall PathRemoveExtensionA(str) shlwapi.PathRemoveExtensionA
@ stdcall PathRemoveExtensionW(wstr) shlwapi.PathRemoveExtensionW
@ stdcall PathRemoveFileSpecA(str) shlwapi.PathRemoveFileSpecA
@ stdcall PathRemoveFileSpecW(wstr) shlwapi.PathRemoveFileSpecW
@ stdcall PathRenameExtensionA(str str) shlwapi.PathRenameExtensionA
@ stdcall PathRenameExtensionW(wstr wstr) shlwapi.PathRenameExtensionW
@ stdcall PathSearchAndQualifyA(str ptr long) shlwapi.PathSearchAndQualifyA
@ stdcall PathSearchAndQualifyW(wstr ptr long) shlwapi.PathSearchAndQualifyW
@ stdcall PathSkipRootA(str) shlwapi.PathSkipRootA
@ stdcall PathSkipRootW(wstr) shlwapi.PathSkipRootW
@ stdcall PathStripPathA(str) shlwapi.PathStripPathA
@ stdcall PathStripPathW(wstr) shlwapi.PathStripPathW
@ stdcall PathStripToRootA(str) shlwapi.PathStripToRootA
@ stdcall PathStripToRootW(wstr) shlwapi.PathStripToRootW
@ stdcall PathUnExpandEnvStringsA(str ptr long) shlwapi.PathUnExpandEnvStringsA
@ stdcall PathUnExpandEnvStringsW(wstr ptr long) shlwapi.PathUnExpandEnvStringsW
@ stdcall PathUnquoteSpacesA(str) shlwapi.PathUnquoteSpacesA
@ stdcall PathUnquoteSpacesW(wstr) shlwapi.PathUnquoteSpacesW
@ stdcall QISearch(long long long long) shlwapi.QISearch
@ stdcall SHLoadIndirectString(wstr ptr long ptr) shlwapi.SHLoadIndirectString
@ stdcall SHRegCloseUSKey(ptr) shlwapi.SHRegCloseUSKey
@ stdcall SHRegCreateUSKeyA(str long long ptr long) shlwapi.SHRegCreateUSKeyA
@ stdcall SHRegCreateUSKeyW(wstr long long ptr long) shlwapi.SHRegCreateUSKeyW
@ stdcall SHRegDeleteEmptyUSKeyA(long str long) shlwapi.SHRegDeleteEmptyUSKeyA
@ stdcall SHRegDeleteEmptyUSKeyW(long wstr long) shlwapi.SHRegDeleteEmptyUSKeyW
@ stdcall SHRegDeleteUSValueA(long str long) shlwapi.SHRegDeleteUSValueA
@ stdcall SHRegDeleteUSValueW(long wstr long) shlwapi.SHRegDeleteUSValueW
@ stdcall SHRegEnumUSKeyA(long long str ptr long) shlwapi.SHRegEnumUSKeyA
@ stdcall SHRegEnumUSKeyW(long long wstr ptr long) shlwapi.SHRegEnumUSKeyW
@ stdcall SHRegEnumUSValueA(long long ptr ptr ptr ptr ptr long) shlwapi.SHRegEnumUSValueA
@ stdcall SHRegEnumUSValueW(long long ptr ptr ptr ptr ptr long) shlwapi.SHRegEnumUSValueW
@ stdcall SHRegGetBoolUSValueA(str str long long) shlwapi.SHRegGetBoolUSValueA
@ stdcall SHRegGetBoolUSValueW(wstr wstr long long) shlwapi.SHRegGetBoolUSValueW
@ stdcall SHRegGetUSValueA( str str ptr ptr ptr long ptr long ) shlwapi.SHRegGetUSValueA
@ stdcall SHRegGetUSValueW( wstr wstr ptr ptr ptr long ptr long ) shlwapi.SHRegGetUSValueW
@ stdcall SHRegOpenUSKeyA( str long long long long ) shlwapi.SHRegOpenUSKeyA
@ stdcall SHRegOpenUSKeyW( wstr long long long long ) shlwapi.SHRegOpenUSKeyW
@ stdcall SHRegQueryInfoUSKeyA( long ptr ptr ptr ptr long ) shlwapi.SHRegQueryInfoUSKeyA
@ stdcall SHRegQueryInfoUSKeyW( long ptr ptr ptr ptr long ) shlwapi.SHRegQueryInfoUSKeyW
@ stdcall SHRegQueryUSValueA( long str ptr ptr ptr long ptr long ) shlwapi.SHRegQueryUSValueA
@ stdcall SHRegQueryUSValueW( long wstr ptr ptr ptr long ptr long ) shlwapi.SHRegQueryUSValueW
@ stdcall SHRegSetUSValueA( str str long ptr long long) shlwapi.SHRegSetUSValueA
@ stdcall SHRegSetUSValueW( wstr wstr long ptr long long) shlwapi.SHRegSetUSValueW
@ stdcall SHRegWriteUSValueA(long str long ptr long long) shlwapi.SHRegWriteUSValueA
@ stdcall SHRegWriteUSValueW(long wstr long ptr long long) shlwapi.SHRegWriteUSValueW
@ stdcall SHTruncateString(str long) shlwapi.SHTruncateString
@ stdcall StrCSpnA(str str) shlwapi.StrCSpnA
@ stdcall StrCSpnIA(str str) shlwapi.StrCSpnIA
@ stdcall StrCSpnIW(wstr wstr) shlwapi.StrCSpnIW
@ stdcall StrCSpnW(wstr wstr) shlwapi.StrCSpnW
@ stdcall StrCatBuffA(str str long) shlwapi.StrCatBuffA
@ stdcall StrCatBuffW(wstr wstr long) shlwapi.StrCatBuffW
@ stdcall StrCatChainW(ptr long long wstr) shlwapi.StrCatChainW
@ stdcall StrChrA(str long) shlwapi.StrChrA
# @ stub StrChrA_MB
@ stdcall StrChrIA(str long) shlwapi.StrChrIA
@ stdcall StrChrIW(wstr long) shlwapi.StrChrIW
# @ stub StrChrNIW
@ stdcall StrChrNW(wstr long long) shlwapi.StrChrNW
@ stdcall StrChrW(wstr long) shlwapi.StrChrW
@ stdcall StrCmpCA(str str) shlwapi.StrCmpCA
@ stdcall StrCmpCW(wstr wstr) shlwapi.StrCmpCW
@ stdcall StrCmpICA(str str) shlwapi.StrCmpICA
@ stdcall StrCmpICW(wstr wstr) shlwapi.StrCmpICW
@ stdcall StrCmpIW(wstr wstr) shlwapi.StrCmpIW
@ stdcall StrCmpLogicalW(wstr wstr) shlwapi.StrCmpLogicalW
@ stdcall StrCmpNA(str str long) shlwapi.StrCmpNA
@ stdcall StrCmpNCA(str ptr long) shlwapi.StrCmpNCA
@ stdcall StrCmpNCW(wstr wstr long) shlwapi.StrCmpNCW
@ stdcall StrCmpNIA(str str long) shlwapi.StrCmpNIA
@ stdcall StrCmpNICA(long long long) shlwapi.StrCmpNICA
@ stdcall StrCmpNICW(wstr wstr long) shlwapi.StrCmpNICW
@ stdcall StrCmpNIW(wstr wstr long) shlwapi.StrCmpNIW
@ stdcall StrCmpNW(wstr wstr long) shlwapi.StrCmpNW
@ stdcall StrCmpW(wstr wstr) shlwapi.StrCmpW
@ stdcall StrCpyNW(ptr wstr long) shlwapi.StrCpyNW
@ stdcall StrCpyNXA(ptr str long) shlwapi.StrCpyNXA
@ stdcall StrCpyNXW(ptr wstr long) shlwapi.StrCpyNXW
@ stdcall StrDupA(str) shlwapi.StrDupA
@ stdcall StrDupW(wstr) shlwapi.StrDupW
@ stdcall StrIsIntlEqualA(long str str long) shlwapi.StrIsIntlEqualA
@ stdcall StrIsIntlEqualW(long wstr wstr long) shlwapi.StrIsIntlEqualW
@ stdcall StrPBrkA(str str) shlwapi.StrPBrkA
@ stdcall StrPBrkW(wstr wstr) shlwapi.StrPBrkW
@ stdcall StrRChrA(str str long) shlwapi.StrRChrA
@ stdcall StrRChrIA(str str long) shlwapi.StrRChrIA
@ stdcall StrRChrIW(wstr wstr long) shlwapi.StrRChrIW
@ stdcall StrRChrW(wstr wstr long) shlwapi.StrRChrW
@ stdcall StrRStrIA(str str str) shlwapi.StrRStrIA
@ stdcall StrRStrIW(wstr wstr wstr) shlwapi.StrRStrIW
@ stdcall StrSpnA(str str) shlwapi.StrSpnA
@ stdcall StrSpnW(wstr wstr) shlwapi.StrSpnW
@ stdcall StrStrA(str str) shlwapi.StrStrA
@ stdcall StrStrIA(str str) shlwapi.StrStrIA
@ stdcall StrStrIW(wstr wstr) shlwapi.StrStrIW
@ stdcall StrStrNIW(wstr wstr long) shlwapi.StrStrNIW
@ stdcall StrStrNW(wstr wstr long) shlwapi.StrStrNW
@ stdcall StrStrW(wstr wstr) shlwapi.StrStrW
@ stdcall StrToInt64ExA(str long ptr) shlwapi.StrToInt64ExA
@ stdcall StrToInt64ExW(wstr long ptr) shlwapi.StrToInt64ExW
@ stdcall StrToIntA(str) shlwapi.StrToIntA
@ stdcall StrToIntExA(str long ptr) shlwapi.StrToIntExA
@ stdcall StrToIntExW(wstr long ptr) shlwapi.StrToIntExW
@ stdcall StrToIntW(wstr) shlwapi.StrToIntW
@ stdcall StrTrimA(str str) shlwapi.StrTrimA
@ stdcall StrTrimW(wstr wstr) shlwapi.StrTrimW
@ stdcall UrlApplySchemeA(str ptr ptr long) shlwapi.UrlApplySchemeA
@ stdcall UrlApplySchemeW(wstr ptr ptr long) shlwapi.UrlApplySchemeW
@ stdcall UrlCanonicalizeA(str ptr ptr long) shlwapi.UrlCanonicalizeA
@ stdcall UrlCanonicalizeW(wstr ptr ptr long) shlwapi.UrlCanonicalizeW
@ stdcall UrlCombineA(str str ptr ptr long) shlwapi.UrlCombineA
@ stdcall UrlCombineW(wstr wstr ptr ptr long) shlwapi.UrlCombineW
@ stdcall UrlCompareA(str str long) shlwapi.UrlCompareA
@ stdcall UrlCompareW(wstr wstr long) shlwapi.UrlCompareW
@ stdcall UrlCreateFromPathA(str ptr ptr long) shlwapi.UrlCreateFromPathA
@ stdcall UrlCreateFromPathW(wstr ptr ptr long) shlwapi.UrlCreateFromPathW
@ stdcall UrlEscapeA(str ptr ptr long) shlwapi.UrlEscapeA
@ stdcall UrlEscapeW(wstr ptr ptr long) shlwapi.UrlEscapeW
@ stdcall UrlFixupW(wstr wstr long) shlwapi.UrlFixupW
@ stdcall UrlGetLocationA(str) shlwapi.UrlGetLocationA
@ stdcall UrlGetLocationW(wstr) shlwapi.UrlGetLocationW
@ stdcall UrlGetPartA(str ptr ptr long long) shlwapi.UrlGetPartA
@ stdcall UrlGetPartW(wstr ptr ptr long long) shlwapi.UrlGetPartW
@ stdcall UrlHashA(str ptr long) shlwapi.UrlHashA
@ stdcall UrlHashW(wstr ptr long) shlwapi.UrlHashW
@ stdcall UrlIsA(str long) shlwapi.UrlIsA
@ stdcall UrlIsNoHistoryA(str) shlwapi.UrlIsNoHistoryA
@ stdcall UrlIsNoHistoryW(wstr) shlwapi.UrlIsNoHistoryW
@ stdcall UrlIsOpaqueA(str) shlwapi.UrlIsOpaqueA
@ stdcall UrlIsOpaqueW(wstr) shlwapi.UrlIsOpaqueW
@ stdcall UrlIsW(wstr long) shlwapi.UrlIsW
@ stdcall UrlUnescapeA(str ptr ptr long) shlwapi.UrlUnescapeA
@ stdcall UrlUnescapeW(wstr ptr ptr long) shlwapi.UrlUnescapeW