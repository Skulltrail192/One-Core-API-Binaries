@ stdcall CredpValidateTargetName() netapibase.CredpValidateTargetName
@ stdcall DsAddressToSiteNamesA(str long ptr str)
@ stdcall DsAddressToSiteNamesExA(str long ptr str str)
@ stdcall DsAddressToSiteNamesExW(wstr long ptr wstr wstr)
@ stdcall DsAddressToSiteNamesW(wstr long ptr wstr)
@ stdcall DsDeregisterDnsHostRecordsA(str str ptr ptr str)
@ stdcall DsDeregisterDnsHostRecordsW(wstr wstr ptr ptr str)
@ stdcall DsEnumerateDomainTrustsA(str long ptr ptr)
@ stdcall DsEnumerateDomainTrustsW(wstr long ptr ptr)
@ stdcall DsGetDcCloseW(ptr)
@ stdcall DsGetDcNameA(str str ptr str long ptr)
@ stdcall DsGetDcNameW(wstr wstr ptr wstr long ptr)
@ stdcall DsGetDcNameWithAccountA() netapibase.DsGetDcNameWithAccountA
@ stdcall DsGetDcNameWithAccountW() netapibase.DsGetDcNameWithAccountW
@ stdcall DsGetDcNextA(ptr ptr ptr str)
@ stdcall DsGetDcNextW(ptr ptr ptr wstr)
@ stdcall DsGetDcOpenA(str long str ptr str long ptr)
@ stdcall DsGetDcOpenW(wstr long wstr ptr wstr long ptr)
@ stdcall DsGetDcSiteCoverageA(str ptr str)
@ stdcall DsGetDcSiteCoverageW(wstr ptr wstr)
@ stdcall DsGetForestTrustInformationW() netapibase.DsGetForestTrustInformationW
@ stdcall DsGetSiteNameA(str str)
@ stdcall DsGetSiteNameW(wstr wstr)
@ stdcall DsMergeForestTrustInformationW(wstr wstr long long)
@ stdcall DsRoleAbortDownlevelServerUpgrade() netapibase.DsRoleAbortDownlevelServerUpgrade
@ stdcall DsRoleCancel() netapibase.DsRoleCancel
@ stdcall DsRoleDcAsDc() netapibase.DsRoleDcAsDc
@ stdcall DsRoleDcAsReplica() netapibase.DsRoleDcAsReplica
@ stdcall DsRoleDemoteDc() netapibase.DsRoleDemoteDc
@ stdcall DsRoleDnsNameToFlatName() netapibase.DsRoleDnsNameToFlatName
@ stdcall DsRoleFreeMemory(ptr)
@ stdcall DsRoleGetDatabaseFacts() netapibase.DsRoleGetDatabaseFacts
@ stdcall DsRoleGetDcOperationProgress() netapibase.DsRoleGetDcOperationProgress
@ stdcall DsRoleGetDcOperationResults() netapibase.DsRoleGetDcOperationResults
@ stdcall DsRoleGetPrimaryDomainInformation(wstr long ptr)
@ stdcall DsRoleIfmHandleFree() netapibase.DsRoleIfmHandleFree
@ stdcall DsRoleServerSaveStateForUpgrade() netapibase.DsRoleServerSaveStateForUpgrade
@ stdcall DsRoleUpgradeDownlevelServer() netapibase.DsRoleUpgradeDownlevelServer
@ stdcall DsValidateSubnetNameA() netapibase.DsValidateSubnetNameA
@ stdcall DsValidateSubnetNameW() netapibase.DsValidateSubnetNameW
@ stdcall I_BrowserDebugCall() netapibase.I_BrowserDebugCall
@ stdcall I_BrowserDebugTrace() netapibase.I_BrowserDebugTrace
@ stdcall I_BrowserQueryEmulatedDomains(wstr ptr ptr)
@ stdcall I_BrowserQueryOtherDomains() netapibase.I_BrowserQueryOtherDomains
@ stdcall I_BrowserQueryStatistics() netapibase.I_BrowserQueryStatistics
@ stdcall I_BrowserResetNetlogonState() netapibase.I_BrowserResetNetlogonState
@ stdcall I_BrowserResetStatistics() netapibase.I_BrowserResetStatistics
@ stdcall I_BrowserServerEnum() netapibase.I_BrowserServerEnum
@ stdcall I_BrowserSetNetlogonState(wstr wstr wstr long)
@ stdcall I_NetAccountDeltas() netapibase.I_NetAccountDeltas
@ stdcall I_NetAccountSync() netapibase.I_NetAccountSync
@ stdcall I_NetCollectPerfData() netapibase.I_NetCollectPerfData
@ stdcall I_NetDatabaseDeltas() netapibase.I_NetDatabaseDeltas
@ stdcall I_NetDatabaseRedo() netapibase.I_NetDatabaseRedo
@ stdcall I_NetDatabaseSync2() netapibase.I_NetDatabaseSync2
@ stdcall I_NetDatabaseSync() netapibase.I_NetDatabaseSync
@ stdcall I_NetDfsCreateExitPoint() netapibase.I_NetDfsCreateExitPoint
@ stdcall I_NetDfsCreateLocalPartition() netapibase.I_NetDfsCreateLocalPartition
@ stdcall I_NetDfsDeleteExitPoint() netapibase.I_NetDfsDeleteExitPoint
@ stdcall I_NetDfsDeleteLocalPartition() netapibase.I_NetDfsDeleteLocalPartition
@ stdcall I_NetDfsFixLocalVolume() netapibase.I_NetDfsFixLocalVolume
@ stdcall I_NetDfsGetFtServers() netapibase.I_NetDfsGetFtServers
@ stdcall I_NetDfsGetVersion() netapibase.I_NetDfsGetVersion
@ stdcall I_NetDfsIsThisADomainName() netapibase.I_NetDfsIsThisADomainName
@ stdcall I_NetDfsManagerReportSiteInfo() netapibase.I_NetDfsManagerReportSiteInfo
@ stdcall I_NetDfsModifyPrefix() netapibase.I_NetDfsModifyPrefix
@ stdcall I_NetDfsSetLocalVolumeState() netapibase.I_NetDfsSetLocalVolumeState
@ stdcall I_NetDfsSetServerInfo() netapibase.I_NetDfsSetServerInfo
@ stdcall I_NetGetDCList() netapibase.I_NetGetDCList
@ stdcall I_NetGetForestTrustInformation() netapibase.I_NetGetForestTrustInformation
@ stdcall I_NetListCanonicalize() netapibase.I_NetListCanonicalize
@ stdcall I_NetListTraverse() netapibase.I_NetListTraverse
@ stdcall I_NetLogonControl2() netapibase.I_NetLogonControl2
@ stdcall I_NetLogonControl() netapibase.I_NetLogonControl
@ stdcall I_NetLogonGetDomainInfo() netapibase.I_NetLogonGetDomainInfo
@ stdcall I_NetLogonSamLogoff() netapibase.I_NetLogonSamLogoff
@ stdcall I_NetLogonSamLogon() netapibase.I_NetLogonSamLogon
@ stdcall I_NetLogonSamLogonEx() netapibase.I_NetLogonSamLogonEx
@ stdcall I_NetLogonSamLogonWithFlags() netapibase.I_NetLogonSamLogonWithFlags
@ stdcall I_NetLogonSendToSam() netapibase.I_NetLogonSendToSam
@ stdcall I_NetLogonUasLogoff() netapibase.I_NetLogonUasLogoff
@ stdcall I_NetLogonUasLogon() netapibase.I_NetLogonUasLogon
@ stdcall I_NetNameCanonicalize() netapibase.I_NetNameCanonicalize
@ stdcall I_NetNameCompare(ptr wstr wstr ptr ptr)
@ stdcall I_NetNameValidate(ptr wstr ptr ptr)
@ stdcall I_NetPathCanonicalize() netapibase.I_NetPathCanonicalize
@ stdcall I_NetPathCompare() netapibase.I_NetPathCompare
@ stdcall I_NetPathType() netapibase.I_NetPathType
@ stdcall I_NetServerAuthenticate2() netapibase.I_NetServerAuthenticate2
@ stdcall I_NetServerAuthenticate3() netapibase.I_NetServerAuthenticate3
@ stdcall I_NetServerAuthenticate() netapibase.I_NetServerAuthenticate
@ stdcall I_NetServerGetTrustInfo() netapibase.I_NetServerGetTrustInfo
@ stdcall I_NetServerPasswordGet() netapibase.I_NetServerPasswordGet
@ stdcall I_NetServerPasswordSet2() netapibase.I_NetServerPasswordSet2
@ stdcall I_NetServerPasswordSet() netapibase.I_NetServerPasswordSet
@ stdcall I_NetServerReqChallenge() netapibase.I_NetServerReqChallenge
@ stdcall I_NetServerSetServiceBits() netapibase.I_NetServerSetServiceBits
@ stdcall I_NetServerSetServiceBitsEx() netapibase.I_NetServerSetServiceBitsEx
@ stdcall I_NetServerTrustPasswordsGet() netapibase.I_NetServerTrustPasswordsGet
@ stdcall I_NetlogonComputeClientDigest() netapibase.I_NetlogonComputeClientDigest
@ stdcall I_NetlogonComputeServerDigest() netapibase.I_NetlogonComputeServerDigest
@ stdcall I_NetlogonGetTrustRid() netapibase.I_NetlogonGetTrustRid
@ stdcall NetAddAlternateComputerName() netapibase.NetAddAlternateComputerName
@ stdcall NetAlertRaise() netapibase.NetAlertRaise
@ stdcall NetAlertRaiseEx() netapibase.NetAlertRaiseEx
@ stdcall NetApiBufferAllocate(long ptr)
@ stdcall NetApiBufferFree(ptr)
@ stdcall NetApiBufferReallocate(ptr long ptr)
@ stdcall NetApiBufferSize(ptr ptr)
@ stdcall NetAuditClear() netapibase.NetAuditClear
@ stdcall NetAuditRead() netapibase.NetAuditRead
@ stdcall NetAuditWrite() netapibase.NetAuditWrite
@ stdcall NetBrowserStatisticsGet() netapibase.NetBrowserStatisticsGet
@ stdcall NetConfigGet() netapibase.NetConfigGet
@ stdcall NetConfigGetAll() netapibase.NetConfigGetAll
@ stdcall NetConfigSet() netapibase.NetConfigSet
@ stdcall NetConnectionEnum() netapibase.NetConnectionEnum
@ stdcall NetDfsAdd() netapibase.NetDfsAdd
@ stdcall NetDfsAddFtRoot() netapibase.NetDfsAddFtRoot
@ stdcall NetDfsAddStdRoot() netapibase.NetDfsAddStdRoot
@ stdcall NetDfsAddStdRootForced() netapibase.NetDfsAddStdRootForced
@ stdcall NetDfsEnum() netapibase.NetDfsEnum
@ stdcall NetDfsGetClientInfo() netapibase.NetDfsGetClientInfo
@ stdcall NetDfsGetDcAddress() netapibase.NetDfsGetDcAddress
@ stdcall NetDfsGetFtContainerSecurity() netapibase.NetDfsGetFtContainerSecurity
@ stdcall NetDfsGetInfo() netapibase.NetDfsGetInfo
@ stdcall NetDfsGetSecurity() netapibase.NetDfsGetSecurity
@ stdcall NetDfsGetStdContainerSecurity() netapibase.NetDfsGetStdContainerSecurity
@ stdcall NetDfsManagerGetConfigInfo() netapibase.NetDfsManagerGetConfigInfo
@ stdcall NetDfsManagerInitialize() netapibase.NetDfsManagerInitialize
@ stdcall NetDfsManagerSendSiteInfo() netapibase.NetDfsManagerSendSiteInfo
@ stdcall NetDfsMove() netapibase.NetDfsMove
@ stdcall NetDfsRemove() netapibase.NetDfsRemove
@ stdcall NetDfsRemoveFtRoot() netapibase.NetDfsRemoveFtRoot
@ stdcall NetDfsRemoveFtRootForced() netapibase.NetDfsRemoveFtRootForced
@ stdcall NetDfsRemoveStdRoot() netapibase.NetDfsRemoveStdRoot
@ stdcall NetDfsRename() netapibase.NetDfsRename
@ stdcall NetDfsSetClientInfo() netapibase.NetDfsSetClientInfo
@ stdcall NetDfsSetFtContainerSecurity() netapibase.NetDfsSetFtContainerSecurity
@ stdcall NetDfsSetInfo() netapibase.NetDfsSetInfo
@ stdcall NetDfsSetSecurity() netapibase.NetDfsSetSecurity
@ stdcall NetDfsSetStdContainerSecurity() netapibase.NetDfsSetStdContainerSecurity
@ stdcall NetEnumerateComputerNames() netapibase.NetEnumerateComputerNames
@ stdcall NetEnumerateTrustedDomains() netapibase.NetEnumerateTrustedDomains
@ stdcall NetErrorLogClear() netapibase.NetErrorLogClear
@ stdcall NetErrorLogRead() netapibase.NetErrorLogRead
@ stdcall NetErrorLogWrite() netapibase.NetErrorLogWrite
@ stdcall NetFileClose() netapibase.NetFileClose
@ stdcall NetFileEnum(wstr wstr wstr long ptr long ptr ptr ptr)
@ stdcall NetFileGetInfo() netapibase.NetFileGetInfo
@ stdcall NetGetAnyDCName() netapibase.NetGetAnyDCName
@ stdcall NetGetDCName(wstr wstr ptr)
@ stdcall NetGetDisplayInformationIndex() netapibase.NetGetDisplayInformationIndex
@ stdcall NetGetJoinInformation(wstr ptr ptr)
@ stdcall NetGetJoinableOUs() netapibase.NetGetJoinableOUs
@ stdcall NetGroupAdd(wstr long ptr ptr)
@ stdcall NetGroupAddUser(wstr wstr wstr)
@ stdcall NetGroupDel(wstr wstr)
@ stdcall NetGroupDelUser(wstr wstr wstr)
@ stdcall NetGroupEnum(wstr long ptr long ptr ptr ptr)
@ stdcall NetGroupGetInfo(wstr wstr long ptr)
@ stdcall NetGroupGetUsers(wstr wstr long ptr long ptr ptr ptr)
@ stdcall NetGroupSetInfo(wstr wstr long ptr ptr)
@ stdcall NetGroupSetUsers(wstr wstr long ptr long)
@ stdcall NetJoinDomain() netapibase.NetJoinDomain
@ stdcall NetLocalGroupAdd(wstr long ptr ptr)
@ stdcall NetLocalGroupAddMember(wstr wstr ptr)
@ stdcall NetLocalGroupAddMembers(wstr wstr long ptr long)
@ stdcall NetLocalGroupDel(wstr wstr)
@ stdcall NetLocalGroupDelMember(wstr wstr ptr)
@ stdcall NetLocalGroupDelMembers(wstr wstr long ptr long)
@ stdcall NetLocalGroupEnum(wstr long ptr long ptr ptr ptr)
@ stdcall NetLocalGroupGetInfo(wstr wstr long ptr)
@ stdcall NetLocalGroupGetMembers(wstr wstr long ptr long ptr ptr ptr)
@ stdcall NetLocalGroupSetInfo(wstr wstr long ptr ptr)
@ stdcall NetLocalGroupSetMembers(wstr wstr long ptr long)
@ stdcall NetLogonGetTimeServiceParentDomain() netapibase.NetLogonGetTimeServiceParentDomain
@ stdcall NetLogonSetServiceBits() netapibase.NetLogonSetServiceBits
@ stdcall NetMessageBufferSend() netapibase.NetMessageBufferSend
@ stdcall NetMessageNameAdd() netapibase.NetMessageNameAdd
@ stdcall NetMessageNameDel() netapibase.NetMessageNameDel
@ stdcall NetMessageNameEnum() netapibase.NetMessageNameEnum
@ stdcall NetMessageNameGetInfo() netapibase.NetMessageNameGetInfo
@ stdcall NetQueryDisplayInformation(wstr long long long long ptr ptr)
@ stdcall NetRegisterDomainNameChangeNotification() netapibase.NetRegisterDomainNameChangeNotification
@ stdcall NetRemoteComputerSupports() netapibase.NetRemoteComputerSupports
@ stdcall NetRemoteTOD() netapibase.NetRemoteTOD
@ stdcall NetRemoveAlternateComputerName() netapibase.NetRemoveAlternateComputerName
@ stdcall NetRenameMachineInDomain() netapibase.NetRenameMachineInDomain
@ stdcall NetReplExportDirAdd() netapibase.NetReplExportDirAdd
@ stdcall NetReplExportDirDel() netapibase.NetReplExportDirDel
@ stdcall NetReplExportDirEnum() netapibase.NetReplExportDirEnum
@ stdcall NetReplExportDirGetInfo() netapibase.NetReplExportDirGetInfo
@ stdcall NetReplExportDirLock() netapibase.NetReplExportDirLock
@ stdcall NetReplExportDirSetInfo() netapibase.NetReplExportDirSetInfo
@ stdcall NetReplExportDirUnlock() netapibase.NetReplExportDirUnlock
@ stdcall NetReplGetInfo() netapibase.NetReplGetInfo
@ stdcall NetReplImportDirAdd() netapibase.NetReplImportDirAdd
@ stdcall NetReplImportDirDel() netapibase.NetReplImportDirDel
@ stdcall NetReplImportDirEnum() netapibase.NetReplImportDirEnum
@ stdcall NetReplImportDirGetInfo() netapibase.NetReplImportDirGetInfo
@ stdcall NetReplImportDirLock() netapibase.NetReplImportDirLock
@ stdcall NetReplImportDirUnlock() netapibase.NetReplImportDirUnlock
@ stdcall NetReplSetInfo() netapibase.NetReplSetInfo
@ stdcall NetScheduleJobAdd(wstr ptr ptr)
@ stdcall NetScheduleJobDel(wstr long long)
@ stdcall NetScheduleJobEnum(wstr ptr long ptr ptr ptr)
@ stdcall NetScheduleJobGetInfo() netapibase.NetScheduleJobGetInfo
@ stdcall NetServerComputerNameAdd() netapibase.NetServerComputerNameAdd
@ stdcall NetServerComputerNameDel() netapibase.NetServerComputerNameDel
@ stdcall NetServerDiskEnum(wstr long ptr long ptr ptr ptr)
@ stdcall NetServerEnum(wstr long ptr long ptr ptr long wstr ptr)
@ stdcall NetServerEnumEx(wstr long ptr long ptr ptr long wstr wstr)
@ stdcall NetServerGetInfo(wstr long ptr)
@ stdcall NetServerSetInfo() netapibase.NetServerSetInfo
@ stdcall NetServerTransportAdd() netapibase.NetServerTransportAdd
@ stdcall NetServerTransportAddEx() netapibase.NetServerTransportAddEx
@ stdcall NetServerTransportDel() netapibase.NetServerTransportDel
@ stdcall NetServerTransportEnum() netapibase.NetServerTransportEnum
@ stdcall NetServiceControl() netapibase.NetServiceControl
@ stdcall NetServiceEnum() netapibase.NetServiceEnum
@ stdcall NetServiceGetInfo() netapibase.NetServiceGetInfo
@ stdcall NetServiceInstall() netapibase.NetServiceInstall
@ stdcall NetSessionDel() netapibase.NetSessionDel
@ stdcall NetSessionEnum(wstr wstr wstr long ptr long ptr ptr ptr)
@ stdcall NetSessionGetInfo() netapibase.NetSessionGetInfo
@ stdcall NetSetPrimaryComputerName() netapibase.NetSetPrimaryComputerName
@ stdcall NetShareAdd(wstr long ptr ptr)
@ stdcall NetShareCheck() netapibase.NetShareCheck
@ stdcall NetShareDel(wstr wstr long)
@ stdcall NetShareDelSticky() netapibase.NetShareDelSticky
@ stdcall NetShareEnum(wstr long ptr long ptr ptr ptr)
@ stdcall NetShareEnumSticky() netapibase.NetShareEnumSticky
@ stdcall NetShareGetInfo(wstr wstr long ptr)
@ stdcall NetShareSetInfo() netapibase.NetShareSetInfo
@ stdcall NetStatisticsGet(wstr wstr long long ptr)
@ stdcall NetUnjoinDomain() netapibase.NetUnjoinDomain
@ stdcall NetUnregisterDomainNameChangeNotification() netapibase.NetUnregisterDomainNameChangeNotification
@ stdcall NetUseAdd(wstr long ptr ptr)
@ stdcall NetUseDel() netapibase.NetUseDel
@ stdcall NetUseEnum(wstr long ptr long ptr ptr ptr)
@ stdcall NetUseGetInfo(ptr ptr long ptr)
@ stdcall NetUserAdd(wstr long ptr ptr)
@ stdcall NetUserChangePassword(wstr wstr wstr wstr)
@ stdcall NetUserDel(wstr wstr)
@ stdcall NetUserEnum(wstr long long ptr long ptr ptr ptr)
@ stdcall NetUserGetGroups(wstr wstr long ptr long ptr ptr)
@ stdcall NetUserGetInfo(wstr wstr long ptr)
@ stdcall NetUserGetLocalGroups(wstr wstr long long ptr long ptr ptr)
@ stdcall NetUserModalsGet(wstr long ptr)
@ stdcall NetUserModalsSet(wstr long ptr ptr)
@ stdcall NetUserSetGroups(wstr wstr long ptr long)
@ stdcall NetUserSetInfo(wstr wstr long ptr ptr)
@ stdcall NetValidateName() netapibase.NetValidateName
@ stdcall NetValidatePasswordPolicy() netapibase.NetValidatePasswordPolicy
@ stdcall NetValidatePasswordPolicyFree() netapibase.NetValidatePasswordPolicyFree
@ stdcall NetWkstaGetInfo(wstr long ptr)
@ stdcall NetWkstaSetInfo() netapibase.NetWkstaSetInfo
@ stdcall NetWkstaTransportAdd() netapibase.NetWkstaTransportAdd
@ stdcall NetWkstaTransportDel() netapibase.NetWkstaTransportDel
@ stdcall NetWkstaTransportEnum (wstr long ptr long ptr ptr ptr)
@ stdcall NetWkstaUserEnum(wstr long ptr long ptr ptr ptr)
@ stdcall NetWkstaUserGetInfo(wstr long ptr)
@ stdcall NetWkstaUserSetInfo() netapibase.NetWkstaUserSetInfo
@ stdcall NetapipBufferAllocate(long ptr) NetApiBufferAllocate
@ stdcall Netbios(ptr)
@ stdcall NetpAccessCheck() netapibase.NetpAccessCheck
@ stdcall NetpAccessCheckAndAudit() netapibase.NetpAccessCheckAndAudit
@ stdcall NetpAddTlnFtinfoEntry() netapibase.NetpAddTlnFtinfoEntry
@ stdcall NetpAllocConfigName() netapibase.NetpAllocConfigName
@ stdcall NetpAllocFtinfoEntry() netapibase.NetpAllocFtinfoEntry
@ stdcall NetpAllocStrFromWStr() netapibase.NetpAllocStrFromWStr
@ stdcall NetpAllocWStrFromStr() netapibase.NetpAllocWStrFromStr
@ stdcall NetpAllocWStrFromWStr() netapibase.NetpAllocWStrFromWStr
@ stdcall NetpApiStatusToNtStatus() netapibase.NetpApiStatusToNtStatus
@ stdcall NetpAssertFailed() netapibase.NetpAssertFailed
@ stdcall NetpCleanFtinfoContext() netapibase.NetpCleanFtinfoContext
@ stdcall NetpCloseConfigData() netapibase.NetpCloseConfigData
@ stdcall NetpCopyFtinfoContext() netapibase.NetpCopyFtinfoContext
@ stdcall NetpCopyStringToBuffer() netapibase.NetpCopyStringToBuffer
@ stdcall NetpCreateSecurityObject() netapibase.NetpCreateSecurityObject
@ stdcall NetpDbgPrint() netapibase.NetpDbgPrint
@ stdcall NetpDeleteSecurityObject(long) ntdll.RtlDeleteSecurityObject
@ stdcall NetpGetComputerName(ptr)
@ stdcall NetpGetConfigBool() netapibase.NetpGetConfigBool
@ stdcall NetpGetConfigDword() netapibase.NetpGetConfigDword
@ stdcall NetpGetConfigTStrArray() netapibase.NetpGetConfigTStrArray
@ stdcall NetpGetConfigValue() netapibase.NetpGetConfigValue
@ stdcall NetpGetDomainName() netapibase.NetpGetDomainName
@ stdcall NetpGetFileSecurity() netapibase.NetpGetFileSecurity
@ stdcall NetpGetPrivilege() netapibase.NetpGetPrivilege
@ stdcall NetpHexDump() netapibase.NetpHexDump
@ stdcall NetpInitFtinfoContext() netapibase.NetpInitFtinfoContext
@ stdcall NetpInitOemString(ptr str) ntdll.RtlInitAnsiString
@ stdcall NetpIsRemote() netapibase.NetpIsRemote
@ stdcall NetpIsUncComputerNameValid() netapibase.NetpIsUncComputerNameValid
@ stdcall NetpLocalTimeZoneOffset() netapibase.NetpLocalTimeZoneOffset
@ stdcall NetpLogonPutUnicodeString() netapibase.NetpLogonPutUnicodeString
@ stdcall NetpMergeFtinfo() netapibase.NetpMergeFtinfo
@ stdcall NetpNetBiosAddName() netapibase.NetpNetBiosAddName
@ stdcall NetpNetBiosCall() netapibase.NetpNetBiosCall
@ stdcall NetpNetBiosDelName() netapibase.NetpNetBiosDelName
@ stdcall NetpNetBiosGetAdapterNumbers() netapibase.NetpNetBiosGetAdapterNumbers
@ stdcall NetpNetBiosHangup() netapibase.NetpNetBiosHangup
@ stdcall NetpNetBiosReceive() netapibase.NetpNetBiosReceive
@ stdcall NetpNetBiosReset() netapibase.NetpNetBiosReset
@ stdcall NetpNetBiosSend() netapibase.NetpNetBiosSend
@ stdcall NetpNetBiosStatusToApiStatus(long)
@ stdcall NetpNtStatusToApiStatus(long)
@ stdcall NetpOpenConfigData() netapibase.NetpOpenConfigData
@ stdcall NetpPackString() netapibase.NetpPackString
@ stdcall NetpParmsQueryUserProperty() netapibase.NetpParmsQueryUserProperty
@ stdcall NetpParmsQueryUserPropertyWithLength() netapibase.NetpParmsQueryUserPropertyWithLength
@ stdcall NetpParmsSetUserProperty() netapibase.NetpParmsSetUserProperty
@ stdcall NetpParmsSetUserPropertyWithLength() netapibase.NetpParmsSetUserPropertyWithLength
@ stdcall NetpParmsUserPropertyFree() netapibase.NetpParmsUserPropertyFree
@ stdcall NetpReleasePrivilege() netapibase.NetpReleasePrivilege
@ stdcall NetpSetFileSecurity() netapibase.NetpSetFileSecurity
@ stdcall NetpSmbCheck() netapibase.NetpSmbCheck
@ stdcall NetpStoreIntialDcRecord() netapibase.NetpStoreIntialDcRecord
@ stdcall NetpStringToNetBiosName() netapibase.NetpStringToNetBiosName
@ stdcall NetpTStrArrayEntryCount() netapibase.NetpTStrArrayEntryCount
@ stdcall NetpUpgradePreNT5JoinInfo() netapibase.NetpUpgradePreNT5JoinInfo
@ stdcall NetpwNameCanonicalize() netapibase.NetpwNameCanonicalize
@ stdcall NetpwNameCompare() netapibase.NetpwNameCompare
@ stdcall NetpwNameValidate() netapibase.NetpwNameValidate
@ stdcall NetpwPathCanonicalize() netapibase.NetpwPathCanonicalize
@ stdcall NetpwPathCompare() netapibase.NetpwPathCompare
@ stdcall NetpwPathType() netapibase.NetpwPathType
@ stdcall NlBindingAddServerToCache() netapibase.NlBindingAddServerToCache
@ stdcall NlBindingRemoveServerFromCache() netapibase.NlBindingRemoveServerFromCache
@ stdcall NlBindingSetAuthInfo() netapibase.NlBindingSetAuthInfo
@ stdcall RxNetAccessAdd() netapibase.RxNetAccessAdd
@ stdcall RxNetAccessDel() netapibase.RxNetAccessDel
@ stdcall RxNetAccessEnum() netapibase.RxNetAccessEnum
@ stdcall RxNetAccessGetInfo() netapibase.RxNetAccessGetInfo
@ stdcall RxNetAccessGetUserPerms() netapibase.RxNetAccessGetUserPerms
@ stdcall RxNetAccessSetInfo() netapibase.RxNetAccessSetInfo
@ stdcall RxNetServerEnum() netapibase.RxNetServerEnum
@ stdcall RxNetUserPasswordSet() netapibase.RxNetUserPasswordSet
@ stdcall RxRemoteApi() netapibase.RxRemoteApi

#Only on XP. But, is "sumimplemneted"
@ stdcall I_NetWkstaResetDfsCache()

#Vista functions from netapinew
@ stdcall DavAddConnection(ptr wstr wstr wstr ptr long)
@ stdcall DavDeleteConnection(ptr)
@ stdcall DavFlushFile(ptr)
@ stdcall DavGetExtendedError(ptr ptr wstr ptr)
@ stdcall DavGetHTTPFromUNCPath(wstr wstr ptr)
@ stdcall DavGetUNCFromHTTPPath(wstr wstr ptr)
@ stdcall I_DsUpdateReadOnlyServerDnsRecords() netapinew.I_DsUpdateReadOnlyServerDnsRecords
@ stdcall I_NetChainSetClientAttributes() netapinew.I_NetChainSetClientAttributes
@ stdcall NetDfsAddRootTarget(wstr wstr long wstr long)
@ stdcall NetDfsGetSupportedNamespaceVersion(long wstr ptr)
@ stdcall NetpAccessCheckAndAuditEx(ptr ptr ptr ptr ptr ptr ptr)
@ stdcall NetpEventlogClose(ptr)
@ stdcall NetpEventlogOpen(wstr long)
@ stdcall NetpEventlogWriteEx(ptr long long long long long long)
@ stdcall NetShareDelEx(wstr long ptr)
@ stdcall NetServerAliasAdd(wstr long ptr)
@ stdcall NetServerAliasDel(wstr long ptr)
@ stdcall NetServerAliasEnum(wstr long ptr long ptr ptr ptr)