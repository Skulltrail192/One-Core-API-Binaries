@ stdcall CM_Get_Device_IDW(long wstr long long)
@ stdcall CM_Get_DevNode_Status(ptr ptr long long)
@ stdcall CM_Get_First_Log_Conf_Ex(ptr long long long)
@ stdcall CM_Install_DevNodeW() setupapi.CM_Install_DevNodeW
@ stdcall CM_Locate_DevNodeW(ptr wstr long)
@ stdcall CM_Set_DevNode_Problem(long long long)
@ stdcall pSetupDiInvalidateHelperModules() setupapi.pSetupDiInvalidateHelperModules
@ stdcall pSetupGetGlobalFlags() setupapi.pSetupGetGlobalFlags
@ stdcall pSetupSetGlobalFlags(long)
@ stdcall pSetupStringFromGuid(ptr wstr long)
@ stdcall SetupAddToSourceListW() setupapi.SetupAddToSourceListW
@ stdcall SetupCloseFileQueue(ptr)
@ stdcall SetupCloseInfFile(long)
@ stdcall SetupCommitFileQueueW(long long ptr ptr)
@ stdcall SetupDefaultQueueCallbackW(ptr long long long)
@ stdcall SetupDiBuildClassInfoList(long ptr long ptr)
@ stdcall SetupDiBuildDriverInfoList(long ptr long)
@ stdcall SetupDiCallClassInstaller(long ptr ptr)
@ stdcall SetupDiCancelDriverInfoSearch() setupapi.SetupDiCancelDriverInfoSearch
@ stdcall SetupDiCreateDeviceInfoList(ptr ptr)
@ stdcall SetupDiDeleteDeviceInfo(long ptr)
@ stdcall SetupDiDestroyClassImageList(ptr)
@ stdcall SetupDiDestroyDeviceInfoList(long)
@ stdcall SetupDiDestroyDriverInfoList(long ptr long)
@ stdcall SetupDiEnumDeviceInfo(long long ptr)
@ stdcall SetupDiEnumDriverInfoW(long ptr long long ptr)
@ stdcall SetupDiGetClassDescriptionW(ptr wstr long ptr)
@ stdcall SetupDiGetClassDevsW(ptr ptr long long)
@ stdcall SetupDiGetClassImageIndex(ptr ptr ptr)
@ stdcall SetupDiGetClassImageList(ptr)
@ stdcall SetupDiGetClassInstallParamsW(ptr ptr ptr long ptr)
@ stdcall SetupDiGetClassPropertyW() setupapi.SetupDiGetClassProperty
@ stdcall SetupDiGetDeviceInstallParamsW(ptr ptr ptr)
@ stdcall SetupDiGetDeviceInstanceIdW(ptr ptr wstr long ptr)
@ stdcall SetupDiGetDevicePropertyW() setupapi.SetupDiGetDeviceProperty
@ stdcall SetupDiGetDriverInfoDetailW(ptr ptr ptr ptr long ptr)
@ stdcall SetupDiGetDriverInstallParamsW(ptr ptr ptr ptr)
@ stdcall SetupDiGetSelectedDevice(ptr ptr)
@ stdcall SetupDiGetSelectedDriverW(ptr ptr ptr)
@ stdcall SetupDiGetWizardPage() setupapi.SetupDiGetWizardPage
@ stdcall SetupDiInstallClassW(long wstr long ptr)
@ stdcall SetupDiLoadDeviceIcon() setupapi.SetupDiLoadDeviceIcon
@ stdcall SetupDiOpenDeviceInfoW(ptr wstr long long ptr)
@ stdcall SetupDiOpenDevRegKey(ptr ptr long long long long)
@ stdcall SetupDiSetClassInstallParamsW(ptr ptr ptr long)
@ stdcall SetupDiSetDeviceInstallParamsW(ptr ptr ptr)
@ stdcall SetupDiSetDevicePropertyW() setupapi.SetupDiSetDeviceProperty
@ stdcall SetupDiSetSelectedDevice(ptr ptr)
@ stdcall SetupDiSetSelectedDriverW(ptr ptr ptr)
@ stdcall SetupFindFirstLineW(long wstr wstr ptr)
@ stdcall SetupFindNextMatchLineW(ptr wstr ptr)
@ stdcall SetupFreeSourceListW() setupapi.SetupFreeSourceListW
@ stdcall SetupGetBackupInformationW() setupapi.SetupGetBackupInformationW
@ stdcall SetupGetFileQueueFlags(long ptr)
@ stdcall SetupGetLineCountW(long wstr)
@ stdcall SetupGetNonInteractiveMode() setupapi.SetupGetNonInteractiveMode
@ stdcall SetupGetStringFieldW(ptr long ptr long ptr)
@ stdcall SetupInitDefaultQueueCallbackEx(long long long long ptr)
@ stdcall SetupOpenFileQueue() 
@ stdcall SetupOpenInfFileW(wstr wstr long ptr)
@ stdcall SetupPromptReboot(ptr ptr long)
@ stdcall SetupQuerySourceListW() setupapi.SetupQuerySourceListW
@ stdcall SetupSetNonInteractiveMode(long)
@ stdcall SetupUninstallNewlyCopiedInfs() setupapi.SetupUninstallNewlyCopiedInfs
@ stdcall SetupUninstallOEMInfW(wstr long ptr)
@ stdcall SetupScanFileQueueW(long long long ptr ptr ptr)
@ stdcall CM_Install_DevNode_ExW() setupapi.CM_Install_DevNode_ExW
@ stub pSetupDiBuildInfoDataFromStrongName #need implementation
@ stdcall pSetupDiGetStrongNameForDriverNode() setupapi.pSetupDiGetStrongNameForDriverNode
@ stdcall SetupCloseTextLogSection() setupapi.SetupCloseTextLogSection
@ stdcall SetupCopyOEMInfW(wstr wstr long long ptr long ptr ptr)
@ stdcall SetupCreateTextLogSectionW() setupapi.SetupCreateTextLogSectionW
@ stdcall SetupDiGetActualModelsSectionW() setupapi.SetupDiGetActualModelsSectionW
@ stdcall SetupDiReportDriverNotFoundError() setupapi.SetupDiReportDriverNotFoundError
@ stdcall SetupFindNextLine(ptr ptr)
@ stdcall SetupGetFieldCount(ptr)
@ stdcall SetupGetThreadLogToken() setupapi.SetupGetThreadLogToken
@ stdcall SetupSetThreadLogToken() setupapi.SetupSetThreadLogToken
@ stdcall SetupWriteTextLog() setupapi.SetupWriteTextLog
@ stdcall SetupDiGetDeviceInfoListDetailW(ptr ptr)