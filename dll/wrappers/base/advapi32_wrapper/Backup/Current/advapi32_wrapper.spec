1 stdcall I_ScGetCurrentGroupStateW(ptr wstr ptr) advapibase.I_ScGetCurrentGroupStateW
2 stdcall A_SHAFinal(ptr ptr) advapibase.A_SHAFinal
3 stdcall A_SHAInit(ptr) advapibase.A_SHAInit
4 stdcall A_SHAUpdate(ptr ptr long) advapibase.A_SHAUpdate
5 stdcall AbortSystemShutdownA(ptr) advapibase.AbortSystemShutdownA
6 stdcall AbortSystemShutdownW(ptr) advapibase.AbortSystemShutdownW
7 stdcall AccessCheck(ptr long long ptr ptr ptr ptr ptr) advapibase.AccessCheck
8 stdcall AccessCheckAndAuditAlarmA(str ptr str str ptr long ptr long ptr ptr ptr) advapibase.AccessCheckAndAuditAlarmA
9 stdcall AccessCheckAndAuditAlarmW(wstr ptr wstr wstr ptr long ptr long ptr ptr ptr) advapibase.AccessCheckAndAuditAlarmW
10 stdcall AccessCheckByType(ptr ptr long long ptr long ptr ptr ptr ptr ptr) advapibase.AccessCheckByType
11 stdcall AccessCheckByTypeAndAuditAlarmA(str ptr str str ptr ptr long long long ptr long ptr long ptr ptr ptr) advapibase.AccessCheckByTypeAndAuditAlarmA
12 stdcall AccessCheckByTypeAndAuditAlarmW(wstr ptr wstr wstr ptr ptr long long long ptr long ptr long ptr ptr ptr) advapibase.AccessCheckByTypeAndAuditAlarmW
13 stdcall AccessCheckByTypeResultList(ptr ptr ptr long ptr long ptr ptr ptr ptr ptr) advapibase.AccessCheckByTypeResultList
14 stdcall AccessCheckByTypeResultListAndAuditAlarmA(str ptr str str ptr long long long long ptr long ptr long ptr ptr ptr) advapibase.AccessCheckByTypeResultListAndAuditAlarmA
15 stdcall AccessCheckByTypeResultListAndAuditAlarmByHandleA(str ptr ptr str str ptr long long long long ptr long ptr long ptr ptr ptr) advapibase.AccessCheckByTypeResultListAndAuditAlarmByHandleA
16 stdcall AccessCheckByTypeResultListAndAuditAlarmByHandleW(wstr ptr ptr wstr wstr ptr long long long long ptr long ptr long ptr ptr ptr) advapibase.AccessCheckByTypeResultListAndAuditAlarmByHandleW
17 stdcall AccessCheckByTypeResultListAndAuditAlarmW(wstr ptr wstr wstr ptr long long long long ptr long ptr long ptr ptr ptr) advapibase.AccessCheckByTypeResultListAndAuditAlarmW
18 stdcall AddAccessAllowedAce(ptr long long ptr) advapibase.AddAccessAllowedAce
19 stdcall AddAccessAllowedAceEx(ptr long long long ptr) advapibase.AddAccessAllowedAceEx
20 stdcall AddAccessAllowedObjectAce(ptr long long long ptr ptr ptr) advapibase.AddAccessAllowedObjectAce
21 stdcall AddAccessDeniedAce(ptr long long ptr) advapibase.AddAccessDeniedAce
22 stdcall AddAccessDeniedAceEx(ptr long long long ptr) advapibase.AddAccessDeniedAceEx
23 stdcall AddAccessDeniedObjectAce(ptr long long long ptr ptr ptr) advapibase.AddAccessDeniedObjectAce
24 stdcall AddAce(ptr long long ptr long) advapibase.AddAce
25 stdcall AddAuditAccessAce(ptr long long ptr long long) advapibase.AddAuditAccessAce
26 stdcall AddAuditAccessAceEx(ptr long long long ptr long long) advapibase.AddAuditAccessAceEx
27 stdcall AddAuditAccessObjectAce(ptr long long long ptr ptr ptr long long) advapibase.AddAuditAccessObjectAce
28 stdcall AddUsersToEncryptedFile(wstr ptr) advapibase.AddUsersToEncryptedFile
29 stdcall AdjustTokenGroups(long long ptr long ptr ptr) advapibase.AdjustTokenGroups
30 stdcall AdjustTokenPrivileges(long long ptr long ptr ptr) advapibase.AdjustTokenPrivileges
31 stdcall AllocateAndInitializeSid(ptr long long long long long long long long long ptr) advapibase.AllocateAndInitializeSid
32 stdcall AllocateLocallyUniqueId(ptr) advapibase.AllocateLocallyUniqueId
33 stdcall AreAllAccessesGranted(long long) advapibase.AreAllAccessesGranted
34 stdcall AreAnyAccessesGranted(long long) advapibase.AreAnyAccessesGranted
35 stdcall BackupEventLogA(long str) advapibase.BackupEventLogA
36 stdcall BackupEventLogW(long wstr) advapibase.BackupEventLogW
37 stdcall BuildExplicitAccessWithNameA(ptr str long long long) advapibase.BuildExplicitAccessWithNameA
38 stdcall BuildExplicitAccessWithNameW(ptr wstr long long long) advapibase.BuildExplicitAccessWithNameW
39 stdcall BuildImpersonateExplicitAccessWithNameA(ptr str ptr long long long) advapibase.BuildImpersonateExplicitAccessWithNameA
40 stdcall BuildImpersonateExplicitAccessWithNameW(ptr wstr ptr long long long) advapibase.BuildImpersonateExplicitAccessWithNameW
41 stdcall BuildImpersonateTrusteeA(ptr ptr) advapibase.BuildImpersonateTrusteeA
42 stdcall BuildImpersonateTrusteeW(ptr ptr) advapibase.BuildImpersonateTrusteeW
43 stdcall BuildSecurityDescriptorA(ptr ptr long ptr long ptr ptr ptr ptr) advapibase.BuildSecurityDescriptorA
44 stdcall BuildSecurityDescriptorW(ptr ptr long ptr long ptr ptr ptr ptr) advapibase.BuildSecurityDescriptorW
45 stdcall BuildTrusteeWithNameA(ptr str) advapibase.BuildTrusteeWithNameA
46 stdcall BuildTrusteeWithNameW(ptr wstr) advapibase.BuildTrusteeWithNameW
47 stdcall BuildTrusteeWithObjectsAndNameA(ptr ptr long str str str) advapibase.BuildTrusteeWithObjectsAndNameA
48 stdcall BuildTrusteeWithObjectsAndNameW(ptr ptr long wstr wstr wstr) advapibase.BuildTrusteeWithObjectsAndNameW
49 stdcall BuildTrusteeWithObjectsAndSidA(ptr ptr ptr ptr ptr) advapibase.BuildTrusteeWithObjectsAndSidA
50 stdcall BuildTrusteeWithObjectsAndSidW(ptr ptr ptr ptr ptr) advapibase.BuildTrusteeWithObjectsAndSidW
51 stdcall BuildTrusteeWithSidA(ptr ptr) advapibase.BuildTrusteeWithSidA
52 stdcall BuildTrusteeWithSidW(ptr ptr) advapibase.BuildTrusteeWithSidW
53 stdcall CancelOverlappedAccess(ptr) advapibase.CancelOverlappedAccess
54 stdcall ChangeServiceConfig2A(long long ptr) advapibase.ChangeServiceConfig2A
55 stdcall ChangeServiceConfig2W(long long ptr) advapibase.ChangeServiceConfig2W
56 stdcall ChangeServiceConfigA(long long long long wstr str ptr str str str str) advapibase.ChangeServiceConfigA
57 stdcall ChangeServiceConfigW(long long long long wstr wstr ptr wstr wstr wstr wstr) advapibase.ChangeServiceConfigW
58 stdcall CheckTokenMembership(long ptr ptr) advapibase.CheckTokenMembership
59 stdcall ClearEventLogA(long str) advapibase.ClearEventLogA
60 stdcall ClearEventLogW(long wstr) advapibase.ClearEventLogW
61 stdcall CloseCodeAuthzLevel(ptr) advapibase.CloseCodeAuthzLevel
62 stdcall CloseEncryptedFileRaw(ptr) advapibase.CloseEncryptedFileRaw
63 stdcall CloseEventLog(long) advapibase.CloseEventLog
64 stdcall CloseServiceHandle(long) advapibase.CloseServiceHandle
65 stdcall CloseTrace(long long) advapibase.CloseTrace
66 stdcall CommandLineFromMsiDescriptor(wstr ptr ptr) advapibase.CommandLineFromMsiDescriptor
67 stdcall ComputeAccessTokenFromCodeAuthzLevel(ptr long) advapibase.ComputeAccessTokenFromCodeAuthzLevel
68 stdcall ControlService(long long ptr) advapibase.ControlService
69 stdcall ControlTraceA(double str ptr long) advapibase.ControlTraceA ;ntdll.EtwControlTraceA
70 stdcall ControlTraceW(double wstr ptr long) advapibase.ControlTraceW ;ntdll.EtwControlTraceW
71 stdcall ConvertAccessToSecurityDescriptorA(ptr ptr str str ptr) advapibase.ConvertAccessToSecurityDescriptorA
72 stdcall ConvertAccessToSecurityDescriptorW(ptr ptr wstr wstr ptr) advapibase.ConvertAccessToSecurityDescriptorW
73 stdcall ConvertSDToStringSDRootDomainA(long ptr long long str long) advapibase.ConvertSDToStringSDRootDomainA
74 stdcall ConvertSDToStringSDRootDomainW(long ptr long long wstr long) advapibase.ConvertSDToStringSDRootDomainW
75 stdcall ConvertSecurityDescriptorToAccessA(ptr long ptr ptr ptr str str) advapibase.ConvertSecurityDescriptorToAccessA
76 stdcall ConvertSecurityDescriptorToAccessNamedA(str long ptr ptr ptr str str) advapibase.ConvertSecurityDescriptorToAccessNamedA
77 stdcall ConvertSecurityDescriptorToAccessNamedW(wstr long ptr ptr ptr wstr wstr) advapibase.ConvertSecurityDescriptorToAccessNamedW
78 stdcall ConvertSecurityDescriptorToAccessW(ptr long ptr ptr ptr wstr wstr) advapibase.ConvertSecurityDescriptorToAccessW
79 stdcall ConvertSecurityDescriptorToStringSecurityDescriptorA(ptr long long ptr ptr) advapibase.ConvertSecurityDescriptorToStringSecurityDescriptorA
80 stdcall ConvertSecurityDescriptorToStringSecurityDescriptorW(ptr long long ptr ptr) advapibase.ConvertSecurityDescriptorToStringSecurityDescriptorW
81 stdcall ConvertSidToStringSidA(ptr ptr) advapibase.ConvertSidToStringSidA
82 stdcall ConvertSidToStringSidW(ptr ptr) advapibase.ConvertSidToStringSidW
83 stdcall ConvertStringSDToSDDomainA(ptr long str long long long) advapibase.ConvertStringSDToSDDomainA ;need implement
84 stdcall ConvertStringSDToSDDomainW(ptr long wstr long long long) advapibase.ConvertStringSDToSDDomainW ;need implement
85 stdcall ConvertStringSDToSDRootDomainA(long long long long long) advapibase.ConvertStringSDToSDRootDomainA
86 stdcall ConvertStringSDToSDRootDomainW(long long long long long) advapibase.ConvertStringSDToSDRootDomainW
87 stdcall ConvertStringSecurityDescriptorToSecurityDescriptorA(str long ptr ptr) advapibase.ConvertStringSecurityDescriptorToSecurityDescriptorA
88 stdcall ConvertStringSecurityDescriptorToSecurityDescriptorW(wstr long ptr ptr) advapibase.ConvertStringSecurityDescriptorToSecurityDescriptorW
89 stdcall ConvertStringSidToSidA(ptr ptr) advapibase.ConvertStringSidToSidA
90 stdcall ConvertStringSidToSidW(ptr ptr) advapibase.ConvertStringSidToSidW
91 stdcall ConvertToAutoInheritPrivateObjectSecurity(ptr ptr ptr ptr long ptr) advapibase.ConvertToAutoInheritPrivateObjectSecurity
92 stdcall CopySid(long ptr ptr) advapibase.CopySid
93 stdcall CreateCodeAuthzLevel() advapibase.CreateCodeAuthzLevel
94 stdcall CreatePrivateObjectSecurity(ptr ptr ptr long long ptr) advapibase.CreatePrivateObjectSecurity
95 stdcall CreatePrivateObjectSecurityEx(ptr ptr ptr ptr long long ptr ptr) advapibase.CreatePrivateObjectSecurityEx
96 stdcall CreatePrivateObjectSecurityWithMultipleInheritance(ptr ptr ptr ptr long long long ptr ptr) advapibase.CreatePrivateObjectSecurityWithMultipleInheritance
97 stdcall CreateProcessAsUserA(long str str ptr ptr long long ptr str ptr ptr) advapibase.CreateProcessAsUserA
98 stdcall CreateProcessAsUserW(long str str ptr ptr long long ptr str ptr ptr) advapibase.CreateProcessAsUserW
99 stdcall CreateProcessWithLogonW(wstr wstr wstr long wstr wstr long ptr wstr ptr ptr) advapibase.CreateProcessWithLogonW
101 stdcall CreateRestrictedToken(long long long ptr long ptr long ptr ptr) advapibase.CreateRestrictedToken
102 stdcall CreateServiceA(long str str long long long long str str ptr str str str) advapibase.CreateServiceA
103 stdcall CreateServiceW(long wstr wstr long long long long wstr wstr ptr wstr wstr wstr) advapibase.CreateServiceW
104 stdcall CreateTraceInstanceId(ptr ptr) advapibase.CreateTraceInstanceId ;ntdll.EtwCreateTraceInstanceId
105 stdcall CreateWellKnownSid(long ptr ptr ptr) advapibase.CreateWellKnownSid
106 stdcall CredDeleteA(str long long) advapibase.CredDeleteA
107 stdcall CredDeleteW(wstr long long) advapibase.CredDeleteW
108 stdcall CredEnumerateA(str long ptr ptr) advapibase.CredEnumerateA
109 stdcall CredEnumerateW(wstr long ptr ptr) advapibase.CredEnumerateW
110 stdcall CredFree(ptr) advapibase.CredFree
111 stdcall CredGetSessionTypes(long ptr) advapibase.CredGetSessionTypes
112 stdcall CredGetTargetInfoA(str long ptr) advapibase.CredGetTargetInfoA
113 stdcall CredGetTargetInfoW(wstr long ptr) advapibase.CredGetTargetInfoW
114 stdcall CredIsMarshaledCredentialA(str) advapibase.CredIsMarshaledCredentialA
115 stdcall CredIsMarshaledCredentialW(wstr) advapibase.CredIsMarshaledCredentialW
116 stdcall CredMarshalCredentialA(long ptr str) advapibase.CredMarshalCredentialA
117 stdcall CredMarshalCredentialW(long ptr wstr) advapibase.CredMarshalCredentialW
118 stdcall CredProfileLoaded() advapibase.CredProfileLoaded
119 stdcall CredReadA(str long long ptr) advapibase.CredReadA
120 stdcall CredReadDomainCredentialsA(ptr long ptr ptr) advapibase.CredReadDomainCredentialsA
121 stdcall CredReadDomainCredentialsW(ptr long ptr ptr) advapibase.CredReadDomainCredentialsW
122 stdcall CredReadW(wstr long long ptr) advapibase.CredReadW
123 stdcall CredRenameA(str str long long) advapibase.CredRenameA
124 stdcall CredRenameW(wstr wstr long long) advapibase.CredRenameW
125 stdcall CredUnmarshalCredentialA(str ptr ptr) advapibase.CredUnmarshalCredentialA
126 stdcall CredUnmarshalCredentialW(wstr ptr ptr) advapibase.CredUnmarshalCredentialW
127 stdcall CredWriteA(ptr long) advapibase.CredWriteA
128 stdcall CredWriteDomainCredentialsA(ptr ptr long) advapibase.CredWriteDomainCredentialsA
129 stdcall CredWriteDomainCredentialsW(ptr ptr long) advapibase.CredWriteDomainCredentialsW
130 stdcall CredWriteW(ptr long) advapibase.CredWriteW
131 stdcall CredpConvertCredential(long long long ptr) advapibase.CredpConvertCredential
132 stdcall CredpConvertTargetInfo(long long ptr long) advapibase.CredpConvertTargetInfo ;need implement
133 stdcall CredpDecodeCredential(ptr) advapibase.CredpDecodeCredential ;need implement
134 stdcall CredpEncodeCredential(ptr) advapibase.CredpEncodeCredential ;need implement
135 stdcall CryptAcquireContextA(ptr str str long long) advapibase.CryptAcquireContextA
136 stdcall CryptAcquireContextW(ptr wstr wstr long long) advapibase.CryptAcquireContextW
137 stdcall CryptContextAddRef(long ptr long) advapibase.CryptContextAddRef
138 stdcall CryptCreateHash(long long long long ptr) advapibase.CryptCreateHash
139 stdcall CryptDecrypt(long long long long ptr ptr) advapibase.CryptDecrypt
140 stdcall CryptDeriveKey(long long long long ptr) advapibase.CryptDeriveKey
141 stdcall CryptDestroyHash(long) advapibase.CryptDestroyHash
142 stdcall CryptDestroyKey(long) advapibase.CryptDestroyKey
143 stdcall CryptDuplicateHash(long ptr long ptr) advapibase.CryptDuplicateHash
144 stdcall CryptDuplicateKey(long ptr long ptr) advapibase.CryptDuplicateKey
145 stdcall CryptEncrypt(long long long long ptr ptr long) advapibase.CryptEncrypt
146 stdcall CryptEnumProviderTypesA(long ptr long ptr ptr ptr) advapibase.CryptEnumProviderTypesA
147 stdcall CryptEnumProviderTypesW(long ptr long ptr ptr ptr) advapibase.CryptEnumProviderTypesW
148 stdcall CryptEnumProvidersA(long ptr long ptr ptr ptr) advapibase.CryptEnumProvidersA
149 stdcall CryptEnumProvidersW(long ptr long ptr ptr ptr) advapibase.CryptEnumProvidersW
150 stdcall CryptExportKey(long long long long ptr ptr) advapibase.CryptExportKey
151 stdcall CryptGenKey(long long long ptr) advapibase.CryptGenKey
152 stdcall CryptGenRandom(long long ptr) advapibase.CryptGenRandom
153 stdcall CryptGetDefaultProviderA(long ptr long ptr ptr) advapibase.CryptGetDefaultProviderA
154 stdcall CryptGetDefaultProviderW(long ptr long ptr ptr) advapibase.CryptGetDefaultProviderW
155 stdcall CryptGetHashParam(long long ptr ptr long) advapibase.CryptGetHashParam
156 stdcall CryptGetKeyParam(long long ptr ptr long) advapibase.CryptGetKeyParam
157 stdcall CryptGetProvParam(long long ptr ptr long) advapibase.CryptGetProvParam
158 stdcall CryptGetUserKey(long long ptr) advapibase.CryptGetUserKey
159 stdcall CryptHashData(long ptr long long) advapibase.CryptHashData
160 stdcall CryptHashSessionKey(long long long) advapibase.CryptHashSessionKey
161 stdcall CryptImportKey(long ptr long long long ptr) advapibase.CryptImportKey
162 stdcall CryptReleaseContext(long long) advapibase.CryptReleaseContext
163 stdcall CryptSetHashParam(long long ptr long) advapibase.CryptSetHashParam
164 stdcall CryptSetKeyParam(long long ptr long) advapibase.CryptSetKeyParam
165 stdcall CryptSetProvParam(long long ptr long) advapibase.CryptSetProvParam
166 stdcall CryptSetProviderA(str long) advapibase.CryptSetProviderA
167 stdcall CryptSetProviderExA(str long ptr long) advapibase.CryptSetProviderExA
168 stdcall CryptSetProviderExW(wstr long ptr long) advapibase.CryptSetProviderExW
169 stdcall CryptSetProviderW(wstr long) advapibase.CryptSetProviderW
170 stdcall CryptSignHashA(long long ptr long ptr ptr) advapibase.CryptSignHashA
171 stdcall CryptSignHashW(long long ptr long ptr ptr) advapibase.CryptSignHashW
172 stdcall CryptVerifySignatureA(long ptr long long ptr long) advapibase.CryptVerifySignatureA
173 stdcall CryptVerifySignatureW(long ptr long long ptr long) advapibase.CryptVerifySignatureW
174 stdcall DecryptFileA(str long) advapibase.DecryptFileA
175 stdcall DecryptFileW(wstr long) advapibase.DecryptFileW
176 stdcall DeleteAce(ptr long) advapibase.DeleteAce
177 stdcall DeleteService(long) advapibase.DeleteService
178 stdcall DeregisterEventSource(long) advapibase.DeregisterEventSource
179 stdcall DestroyPrivateObjectSecurity(ptr) advapibase.DestroyPrivateObjectSecurity
180 stdcall DuplicateEncryptionInfoFile(wstr wstr long long ptr) advapibase.DuplicateEncryptionInfoFile
181 stdcall DuplicateToken(long long ptr) advapibase.DuplicateToken
182 stdcall DuplicateTokenEx(long long ptr long long ptr) advapibase.DuplicateTokenEx
183 stdcall ElfBackupEventLogFileA(long ptr) advapibase.ElfBackupEventLogFileA
184 stdcall ElfBackupEventLogFileW(long ptr) advapibase.ElfBackupEventLogFileW
185 stdcall ElfChangeNotify(long long) advapibase.ElfChangeNotify
186 stdcall ElfClearEventLogFileA(long ptr) advapibase.ElfClearEventLogFileA
187 stdcall ElfClearEventLogFileW(long ptr) advapibase.ElfClearEventLogFileW
188 stdcall ElfCloseEventLog(long) advapibase.ElfCloseEventLog
189 stdcall ElfDeregisterEventSource(long) advapibase.ElfDeregisterEventSource
190 stdcall ElfFlushEventLog(long) advapibase.ElfFlushEventLog
191 stdcall ElfNumberOfRecords(long ptr) advapibase.ElfNumberOfRecords
192 stdcall ElfOldestRecord(long ptr) advapibase.ElfOldestRecord
193 stdcall ElfOpenBackupEventLogA(ptr ptr ptr) advapibase.ElfOpenBackupEventLogA
194 stdcall ElfOpenBackupEventLogW(ptr ptr ptr) advapibase.ElfOpenBackupEventLogW
195 stdcall ElfOpenEventLogA(ptr ptr ptr) advapibase.ElfOpenEventLogA
196 stdcall ElfOpenEventLogW(ptr ptr ptr) advapibase.ElfOpenEventLogW
197 stdcall ElfReadEventLogA(long long long ptr long ptr ptr) advapibase.ElfReadEventLogA
198 stdcall ElfReadEventLogW(long long long ptr long ptr ptr) advapibase.ElfReadEventLogW
199 stdcall ElfRegisterEventSourceA(ptr ptr ptr) advapibase.ElfRegisterEventSourceA
200 stdcall ElfRegisterEventSourceW(ptr ptr ptr) advapibase.ElfRegisterEventSourceW
201 stdcall ElfReportEventA(long long long long ptr long long ptr ptr long ptr ptr) advapibase.ElfReportEventA
203 stdcall ElfReportEventW(long long long long ptr long long ptr ptr long ptr ptr) advapibase.ElfReportEventW
204 stdcall EnableTrace(long long long ptr double) advapibase.EnableTrace ;ntdll.EtwEnableTrace
205 stdcall EncryptFileA(str) advapibase.EncryptFileA
206 stdcall EncryptFileW(wstr) advapibase.EncryptFileW
207 stdcall EncryptedFileKeyInfo(long long long) advapibase.EncryptedFileKeyInfo
208 stdcall EncryptionDisable(wstr long) advapibase.EncryptionDisable
209 stdcall EnumDependentServicesA(long long ptr long ptr ptr) advapibase.EnumDependentServicesA
210 stdcall EnumDependentServicesW(long long ptr long ptr ptr) advapibase.EnumDependentServicesW
211 stdcall EnumServiceGroupW(ptr long long ptr long ptr ptr ptr wstr) advapibase.EnumServiceGroupW
212 stdcall EnumServicesStatusA(long long long ptr long ptr ptr ptr) advapibase.EnumServicesStatusA
213 stdcall EnumServicesStatusExA(long long long long ptr long ptr ptr ptr str) advapibase.EnumServicesStatusExA
214 stdcall EnumServicesStatusExW(long long long long ptr long ptr ptr ptr wstr) advapibase.EnumServicesStatusExW
215 stdcall EnumServicesStatusW(long long long ptr long ptr ptr ptr) advapibase.EnumServicesStatusW
216 stdcall EnumerateTraceGuids(ptr long ptr) advapibase.EnumerateTraceGuids ;ntdll.EtwEnumerateTraceGuids
217 stdcall EqualDomainSid(ptr ptr ptr) advapibase.EqualDomainSid
218 stdcall EqualPrefixSid(ptr ptr) advapibase.EqualPrefixSid
219 stdcall EqualSid(ptr ptr) advapibase.EqualSid
220 stdcall FileEncryptionStatusA(str ptr) advapibase.FileEncryptionStatusA
221 stdcall FileEncryptionStatusW(wstr ptr) advapibase.FileEncryptionStatusW
222 stdcall FindFirstFreeAce(ptr ptr) advapibase.FindFirstFreeAce
223 stdcall FlushTraceA(double str ptr) advapibase.FlushTraceA ;ntdll.EtwFlushTraceA
224 stdcall FlushTraceW(double wstr ptr) advapibase.FlushTraceW ;ntdll.EtwFlushTraceW
225 stdcall FreeEncryptedFileKeyInfo(long) advapibase.FreeEncryptedFileKeyInfo
226 stdcall FreeEncryptionCertificateHashList(ptr) advapibase.FreeEncryptionCertificateHashList
227 stdcall FreeInheritedFromArray(ptr long ptr) advapibase.FreeInheritedFromArray
228 stdcall FreeSid(ptr) advapibase.FreeSid
229 stdcall GetAccessPermissionsForObjectA(str long str str ptr ptr ptr ptr ptr) advapibase.GetAccessPermissionsForObjectA
230 stdcall GetAccessPermissionsForObjectW(wstr long wstr wstr ptr ptr ptr ptr ptr) advapibase.GetAccessPermissionsForObjectW
231 stdcall GetAce(ptr long ptr) advapibase.GetAce
232 stdcall GetAclInformation(ptr ptr long long) advapibase.GetAclInformation
233 stdcall GetAuditedPermissionsFromAclA(ptr ptr ptr ptr) advapibase.GetAuditedPermissionsFromAclA
234 stdcall GetAuditedPermissionsFromAclW(ptr ptr ptr ptr) advapibase.GetAuditedPermissionsFromAclW
235 stdcall GetCurrentHwProfileA(ptr) advapibase.GetCurrentHwProfileA
236 stdcall GetCurrentHwProfileW(ptr) advapibase.GetCurrentHwProfileW
237 stdcall GetEffectiveRightsFromAclA(ptr ptr ptr) advapibase.GetEffectiveRightsFromAclA
238 stdcall GetEffectiveRightsFromAclW(ptr ptr ptr) advapibase.GetEffectiveRightsFromAclW
239 stdcall GetEventLogInformation(long long ptr long ptr) advapibase.GetEventLogInformation
240 stdcall GetExplicitEntriesFromAclA(ptr ptr ptr) advapibase.GetExplicitEntriesFromAclA ;advapi32.GetExplicitEntriesFromAclW
241 stdcall GetExplicitEntriesFromAclW(ptr ptr ptr) advapibase.GetExplicitEntriesFromAclW
242 stdcall GetFileSecurityA(str long ptr long ptr) advapibase.GetFileSecurityA
243 stdcall GetFileSecurityW(wstr long ptr long ptr) advapibase.GetFileSecurityW
244 stdcall GetInformationCodeAuthzLevelW() advapibase.GetInformationCodeAuthzLevelW
245 stdcall GetInformationCodeAuthzPolicyW() advapibase.GetInformationCodeAuthzPolicyW
246 stdcall GetInheritanceSourceA(str long long long ptr long ptr ptr ptr ptr) advapibase.GetInheritanceSourceA
247 stdcall GetInheritanceSourceW(wstr long long long ptr long ptr ptr ptr ptr) advapibase.GetInheritanceSourceW
248 stdcall GetKernelObjectSecurity(long long ptr long ptr) advapibase.GetKernelObjectSecurity
249 stdcall GetLengthSid(ptr) advapibase.GetLengthSid
250 stdcall GetLocalManagedApplicationData(wstr wstr wstr) advapibase.GetLocalManagedApplicationData
251 stdcall GetLocalManagedApplications(long ptr ptr) advapibase.GetLocalManagedApplications
252 stdcall GetManagedApplicationCategories(long ptr) advapibase.GetManagedApplicationCategories
253 stdcall GetManagedApplications(ptr long long ptr ptr) advapibase.GetManagedApplications
254 stdcall GetMultipleTrusteeA(ptr) advapibase.GetMultipleTrusteeA
255 stdcall GetMultipleTrusteeOperationA(ptr) advapibase.GetMultipleTrusteeOperationA
256 stdcall GetMultipleTrusteeOperationW(ptr) advapibase.GetMultipleTrusteeOperationW
257 stdcall GetMultipleTrusteeW(ptr) advapibase.GetMultipleTrusteeW
258 stdcall GetNamedSecurityInfoA(str long long ptr ptr ptr ptr ptr) advapibase.GetNamedSecurityInfoA
259 stdcall GetNamedSecurityInfoExA(str long long str str ptr ptr str str) advapibase.GetNamedSecurityInfoExA
260 stdcall GetNamedSecurityInfoExW(wstr long long wstr wstr ptr ptr wstr wstr) advapibase.GetNamedSecurityInfoExW
261 stdcall GetNamedSecurityInfoW(wstr long long ptr ptr ptr ptr ptr) advapibase.GetNamedSecurityInfoW
262 stdcall GetNumberOfEventLogRecords(long ptr) advapibase.GetNumberOfEventLogRecords
263 stdcall GetOldestEventLogRecord(long ptr) advapibase.GetOldestEventLogRecord
264 stdcall GetOverlappedAccessResults(ptr long ptr ptr) advapibase.GetOverlappedAccessResults
265 stdcall GetPrivateObjectSecurity(ptr long ptr long ptr) advapibase.GetPrivateObjectSecurity
266 stdcall GetSecurityDescriptorControl(ptr ptr ptr) advapibase.GetSecurityDescriptorControl
267 stdcall GetSecurityDescriptorDacl(ptr ptr ptr ptr) advapibase.GetSecurityDescriptorDacl
268 stdcall GetSecurityDescriptorGroup(ptr ptr ptr) advapibase.GetSecurityDescriptorGroup
269 stdcall GetSecurityDescriptorLength(ptr) advapibase.GetSecurityDescriptorLength ;ntdll.RtlLengthSecurityDescriptor
270 stdcall GetSecurityDescriptorOwner(ptr ptr ptr) advapibase.GetSecurityDescriptorOwner
271 stdcall GetSecurityDescriptorRMControl(ptr ptr) advapibase.GetSecurityDescriptorRMControl
272 stdcall GetSecurityDescriptorSacl(ptr ptr ptr ptr) advapibase.GetSecurityDescriptorSacl
273 stdcall GetSecurityInfo(long long long ptr ptr ptr ptr ptr) advapibase.GetSecurityInfo
274 stdcall GetSecurityInfoExA(long long long str str ptr ptr ptr ptr) advapibase.GetSecurityInfoExA
275 stdcall GetSecurityInfoExW(long long long wstr wstr ptr ptr ptr ptr) advapibase.GetSecurityInfoExW
276 stdcall GetServiceDisplayNameA(ptr str ptr ptr) advapibase.GetServiceDisplayNameA
277 stdcall GetServiceDisplayNameW(ptr wstr ptr ptr) advapibase.GetServiceDisplayNameW
278 stdcall GetServiceKeyNameA(long str ptr ptr) advapibase.GetServiceKeyNameA
279 stdcall GetServiceKeyNameW(long wstr ptr ptr) advapibase.GetServiceKeyNameW
280 stdcall GetSidIdentifierAuthority(ptr) advapibase.GetSidIdentifierAuthority
281 stdcall GetSidLengthRequired(long) advapibase.GetSidLengthRequired
282 stdcall GetSidSubAuthority(ptr long) advapibase.GetSidSubAuthority
283 stdcall GetSidSubAuthorityCount(ptr) advapibase.GetSidSubAuthorityCount
284 stdcall GetTokenInformation(long long ptr long ptr) advapibase.GetTokenInformation
285 stdcall GetTraceEnableFlags(double) advapibase.GetTraceEnableFlags ;ntdll.EtwGetTraceEnableFlags
286 stdcall GetTraceEnableLevel(double) advapibase.GetTraceEnableLevel ;ntdll.EtwGetTraceEnableLevel
287 stdcall GetTraceLoggerHandle(ptr) advapibase.GetTraceLoggerHandle ;ntdll.EtwGetTraceLoggerHandle
288 stdcall GetTrusteeFormA(ptr) advapibase.GetTrusteeFormA
289 stdcall GetTrusteeFormW(ptr) advapibase.GetTrusteeFormW
290 stdcall GetTrusteeNameA(ptr) advapibase.GetTrusteeNameA
291 stdcall GetTrusteeNameW(ptr) advapibase.GetTrusteeNameW
292 stdcall GetTrusteeTypeA(ptr) advapibase.GetTrusteeTypeA
293 stdcall GetTrusteeTypeW(ptr) advapibase.GetTrusteeTypeW
294 stdcall GetUserNameA(ptr ptr) advapibase.GetUserNameA
295 stdcall GetUserNameW(ptr ptr) advapibase.GetUserNameW
296 stdcall GetWindowsAccountDomainSid(ptr ptr ptr) advapibase.GetWindowsAccountDomainSid
298 stdcall I_ScIsSecurityProcess() advapibase.I_ScIsSecurityProcess
299 stdcall I_ScPnPGetServiceName(ptr wstr long) advapibase.I_ScPnPGetServiceName
300 stdcall I_ScSendTSMessage(long long long long) advapibase.I_ScSendTSMessage
301 stdcall I_ScSetServiceBitsA(ptr long long long str) advapibase.I_ScSetServiceBitsA
302 stdcall I_ScSetServiceBitsW(ptr long long long wstr) advapibase.I_ScSetServiceBitsW
303 stdcall IdentifyCodeAuthzLevelW() advapibase.IdentifyCodeAuthzLevelW
304 stdcall ImpersonateAnonymousToken(ptr) advapibase.ImpersonateAnonymousToken
305 stdcall ImpersonateLoggedOnUser(long) advapibase.ImpersonateLoggedOnUser
306 stdcall ImpersonateNamedPipeClient(long) advapibase.ImpersonateNamedPipeClient
307 stdcall ImpersonateSelf(long) advapibase.ImpersonateSelf
308 stdcall InitializeAcl(ptr long long) advapibase.InitializeAcl
309 stdcall InitializeSecurityDescriptor(ptr long) advapibase.InitializeSecurityDescriptor
310 stdcall InitializeSid(ptr ptr long) advapibase.InitializeSid
311 stdcall InitiateSystemShutdownA(str str long long long) advapibase.InitiateSystemShutdownA
312 stdcall InitiateSystemShutdownExA(str str long long long long) advapibase.InitiateSystemShutdownExA
313 stdcall InitiateSystemShutdownExW(wstr wstr long long long long) advapibase.InitiateSystemShutdownExW
314 stdcall InitiateSystemShutdownW(str str long long long) advapibase.InitiateSystemShutdownW
315 stdcall InstallApplication(ptr) advapibase.InstallApplication
316 stdcall IsTextUnicode(ptr long ptr) advapibase.IsTextUnicode
317 stdcall IsTokenRestricted(long) advapibase.IsTokenRestricted
318 stdcall IsTokenUntrusted(ptr) advapibase.IsTokenUntrusted
319 stdcall IsValidAcl(ptr) advapibase.IsValidAcl
320 stdcall IsValidSecurityDescriptor(ptr) advapibase.IsValidSecurityDescriptor
321 stdcall IsValidSid(ptr) advapibase.IsValidSid
322 stdcall IsWellKnownSid(ptr long) advapibase.IsWellKnownSid
323 stdcall LockServiceDatabase(ptr) advapibase.LockServiceDatabase
324 stdcall LogonUserA(str str str long long ptr) advapibase.LogonUserA
325 stdcall LogonUserExA(str str str long long ptr ptr ptr ptr ptr) advapibase.LogonUserExA
326 stdcall LogonUserExW(wstr wstr wstr long long ptr ptr ptr ptr ptr) advapibase.LogonUserExW
327 stdcall LogonUserW(wstr wstr wstr long long ptr) advapibase.LogonUserW
328 stdcall LookupAccountNameA(str str ptr ptr ptr ptr ptr) advapibase.LookupAccountNameA
329 stdcall LookupAccountNameW(wstr wstr ptr ptr ptr ptr ptr) advapibase.LookupAccountNameW
330 stdcall LookupAccountSidA(ptr ptr ptr ptr ptr ptr ptr) advapibase.LookupAccountSidA
331 stdcall LookupAccountSidW(ptr ptr ptr ptr ptr ptr ptr) advapibase.LookupAccountSidW
332 stdcall LookupPrivilegeDisplayNameA(str str str ptr ptr) advapibase.LookupPrivilegeDisplayNameA
333 stdcall LookupPrivilegeDisplayNameW(wstr wstr wstr ptr ptr) advapibase.LookupPrivilegeDisplayNameW
334 stdcall LookupPrivilegeNameA(str ptr ptr long) advapibase.LookupPrivilegeNameA
335 stdcall LookupPrivilegeNameW(wstr ptr ptr long) advapibase.LookupPrivilegeNameW
336 stdcall LookupPrivilegeValueA(ptr ptr ptr) advapibase.LookupPrivilegeValueA
337 stdcall LookupPrivilegeValueW(ptr ptr ptr) advapibase.LookupPrivilegeValueW
338 stdcall LookupSecurityDescriptorPartsA(ptr ptr ptr ptr ptr ptr ptr) advapibase.LookupSecurityDescriptorPartsA
339 stdcall LookupSecurityDescriptorPartsW(ptr ptr ptr ptr ptr ptr ptr) advapibase.LookupSecurityDescriptorPartsW
340 stdcall LsaAddAccountRights(ptr ptr ptr long) advapibase.LsaAddAccountRights
341 stdcall LsaAddPrivilegesToAccount(ptr ptr) advapibase.LsaAddPrivilegesToAccount
342 stdcall LsaClearAuditLog(ptr) advapibase.LsaClearAuditLog
343 stdcall LsaClose(ptr) advapibase.LsaClose
344 stdcall LsaCreateAccount(ptr ptr long ptr) advapibase.LsaCreateAccount
345 stdcall LsaCreateSecret(ptr ptr long ptr) advapibase.LsaCreateSecret
346 stdcall LsaCreateTrustedDomain(ptr ptr long ptr) advapibase.LsaCreateTrustedDomain
347 stdcall LsaCreateTrustedDomainEx(ptr ptr ptr long ptr) advapibase.LsaCreateTrustedDomainEx
348 stdcall LsaDelete(ptr) advapibase.LsaDelete
349 stdcall LsaDeleteTrustedDomain(ptr ptr) advapibase.LsaDeleteTrustedDomain
350 stdcall LsaEnumerateAccountRights(ptr ptr ptr ptr) advapibase.LsaEnumerateAccountRights
351 stdcall LsaEnumerateAccounts(ptr ptr ptr long ptr) advapibase.LsaEnumerateAccounts
352 stdcall LsaEnumerateAccountsWithUserRight(ptr ptr ptr ptr) advapibase.LsaEnumerateAccountsWithUserRight
353 stdcall LsaEnumeratePrivileges(ptr ptr ptr long ptr) advapibase.LsaEnumeratePrivileges
354 stdcall LsaEnumeratePrivilegesOfAccount(ptr ptr) advapibase.LsaEnumeratePrivilegesOfAccount
355 stdcall LsaEnumerateTrustedDomains(ptr ptr ptr long ptr) advapibase.LsaEnumerateTrustedDomains
356 stdcall LsaEnumerateTrustedDomainsEx(ptr ptr ptr long ptr) advapibase.LsaEnumerateTrustedDomainsEx
357 stdcall LsaFreeMemory(ptr) advapibase.LsaFreeMemory
358 stdcall LsaGetQuotasForAccount(ptr ptr) advapibase.LsaGetQuotasForAccount
359 stdcall LsaGetRemoteUserName(ptr ptr ptr) advapibase.LsaGetRemoteUserName
360 stdcall LsaGetSystemAccessAccount(ptr ptr) advapibase.LsaGetSystemAccessAccount
361 stdcall LsaGetUserName(ptr ptr) advapibase.LsaGetUserName
362 stdcall LsaICLookupNames(long long long ptr ptr long long long long long) advapibase.LsaICLookupNames
363 stdcall LsaICLookupNamesWithCreds(wstr wstr long long ptr long long long long long long long) advapibase.LsaICLookupNamesWithCreds
364 stdcall LsaICLookupSids(long long long ptr ptr long long long long) advapibase.LsaICLookupSids
365 stdcall LsaICLookupSidsWithCreds(wstr wstr long long ptr long long long long long long long) advapibase.LsaICLookupSidsWithCreds
366 stdcall LsaLookupNames2(ptr long long ptr ptr ptr) advapibase.LsaLookupNames2
367 stdcall LsaLookupNames(ptr long ptr ptr ptr) advapibase.LsaLookupNames
368 stdcall LsaLookupPrivilegeDisplayName(ptr ptr ptr ptr) advapibase.LsaLookupPrivilegeDisplayName
369 stdcall LsaLookupPrivilegeName(ptr ptr ptr) advapibase.LsaLookupPrivilegeName
370 stdcall LsaLookupPrivilegeValue(ptr ptr ptr) advapibase.LsaLookupPrivilegeValue
371 stdcall LsaLookupSids(ptr long ptr ptr ptr) advapibase.LsaLookupSids
372 stdcall LsaNtStatusToWinError(long) advapibase.LsaNtStatusToWinError
373 stdcall LsaOpenAccount(ptr ptr long ptr) advapibase.LsaOpenAccount
374 stdcall LsaOpenPolicy(ptr ptr long ptr) advapibase.LsaOpenPolicy
375 stdcall LsaOpenPolicySce(ptr ptr long ptr) advapibase.LsaOpenPolicySce
376 stdcall LsaOpenSecret(ptr ptr long ptr) advapibase.LsaOpenSecret
377 stdcall LsaOpenTrustedDomain(ptr ptr long ptr) advapibase.LsaOpenTrustedDomain
378 stdcall LsaOpenTrustedDomainByName(ptr ptr long ptr) advapibase.LsaOpenTrustedDomainByName
379 stdcall LsaQueryDomainInformationPolicy(ptr long ptr) advapibase.LsaQueryDomainInformationPolicy
380 stdcall LsaQueryForestTrustInformation(ptr ptr ptr) advapibase.LsaQueryForestTrustInformation
381 stdcall LsaQueryInfoTrustedDomain(ptr long ptr) advapibase.LsaQueryInfoTrustedDomain
382 stdcall LsaQueryInformationPolicy(ptr long ptr) advapibase.LsaQueryInformationPolicy
383 stdcall LsaQuerySecret(ptr ptr ptr ptr ptr) advapibase.LsaQuerySecret
384 stdcall LsaQuerySecurityObject(ptr long ptr) advapibase.LsaQuerySecurityObject
385 stdcall LsaQueryTrustedDomainInfo(ptr ptr long ptr) advapibase.LsaQueryTrustedDomainInfo
386 stdcall LsaQueryTrustedDomainInfoByName(ptr ptr long ptr) advapibase.LsaQueryTrustedDomainInfoByName
387 stdcall LsaRemoveAccountRights(ptr ptr long ptr long) advapibase.LsaRemoveAccountRights
388 stdcall LsaRemovePrivilegesFromAccount(ptr long ptr) advapibase.LsaRemovePrivilegesFromAccount
389 stdcall LsaRetrievePrivateData(ptr ptr ptr) advapibase.LsaRetrievePrivateData
390 stdcall LsaSetDomainInformationPolicy(ptr long ptr) advapibase.LsaSetDomainInformationPolicy
391 stdcall LsaSetForestTrustInformation(ptr ptr ptr long ptr) advapibase.LsaSetForestTrustInformation
392 stdcall LsaSetInformationPolicy(ptr long ptr) advapibase.LsaSetInformationPolicy
393 stdcall LsaSetInformationTrustedDomain(ptr long ptr) advapibase.LsaSetInformationTrustedDomain
394 stdcall LsaSetQuotasForAccount(ptr ptr) advapibase.LsaSetQuotasForAccount
395 stdcall LsaSetSecret(ptr ptr ptr) advapibase.LsaSetSecret
396 stdcall LsaSetSecurityObject(ptr long ptr) advapibase.LsaSetSecurityObject
397 stdcall LsaSetSystemAccessAccount(ptr long) advapibase.LsaSetSystemAccessAccount
398 stdcall LsaSetTrustedDomainInfoByName(ptr ptr long ptr) advapibase.LsaSetTrustedDomainInfoByName
399 stdcall LsaSetTrustedDomainInformation(ptr ptr long ptr) advapibase.LsaSetTrustedDomainInformation
400 stdcall LsaStorePrivateData(ptr ptr ptr) advapibase.LsaStorePrivateData
401 stdcall MD4Final(ptr) advapibase.MD4Final
402 stdcall MD4Init(ptr) advapibase.MD4Init
403 stdcall MD4Update(ptr ptr long) advapibase.MD4Update
404 stdcall MD5Final(ptr) advapibase.MD5Final
405 stdcall MD5Init(ptr) advapibase.MD5Init
406 stdcall MD5Update(ptr ptr long) advapibase.MD5Update
407 stdcall MSChapSrvChangePassword2(wstr wstr ptr ptr long ptr ptr) advapibase.MSChapSrvChangePassword2
408 stdcall MSChapSrvChangePassword(wstr wstr long ptr ptr ptr ptr) advapibase.MSChapSrvChangePassword
409 stdcall MakeAbsoluteSD2(ptr ptr) advapibase.MakeAbsoluteSD2
410 stdcall MakeAbsoluteSD(ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr) advapibase.MakeAbsoluteSD
411 stdcall MakeSelfRelativeSD(ptr ptr ptr) advapibase.MakeSelfRelativeSD
412 stdcall MapGenericMask(ptr ptr) advapibase.MapGenericMask ;ntdll.RtlMapGenericMask
413 stdcall NotifyBootConfigStatus(long) advapibase.NotifyBootConfigStatus
414 stdcall NotifyChangeEventLog(long long) advapibase.NotifyChangeEventLog
415 stdcall ObjectCloseAuditAlarmA(str ptr long) advapibase.ObjectCloseAuditAlarmA
416 stdcall ObjectCloseAuditAlarmW(wstr ptr long) advapibase.ObjectCloseAuditAlarmW
417 stdcall ObjectDeleteAuditAlarmA(str ptr long) advapibase.ObjectDeleteAuditAlarmA
418 stdcall ObjectDeleteAuditAlarmW(wstr ptr long) advapibase.ObjectDeleteAuditAlarmW
419 stdcall ObjectOpenAuditAlarmA(str ptr str str ptr long long long ptr long long ptr) advapibase.ObjectOpenAuditAlarmA
420 stdcall ObjectOpenAuditAlarmW(wstr ptr wstr wstr ptr long long long ptr long long ptr) advapibase.ObjectOpenAuditAlarmW
421 stdcall ObjectPrivilegeAuditAlarmA(str ptr long long ptr long) advapibase.ObjectPrivilegeAuditAlarmA
422 stdcall ObjectPrivilegeAuditAlarmW(wstr ptr long long ptr long) advapibase.ObjectPrivilegeAuditAlarmW
423 stdcall OpenBackupEventLogA(str str) advapibase.OpenBackupEventLogA
424 stdcall OpenBackupEventLogW(wstr wstr) advapibase.OpenBackupEventLogW
425 stdcall OpenEncryptedFileRawA(str long ptr) advapibase.OpenEncryptedFileRawA
426 stdcall OpenEncryptedFileRawW(wstr long ptr) advapibase.OpenEncryptedFileRawW
427 stdcall OpenEventLogA(str str) advapibase.OpenEventLogA
428 stdcall OpenEventLogW(wstr wstr) advapibase.OpenEventLogW
429 stdcall OpenProcessToken(long long ptr) advapibase.OpenProcessToken
430 stdcall OpenSCManagerA(str str long) advapibase.OpenSCManagerA
431 stdcall OpenSCManagerW(wstr wstr long) advapibase.OpenSCManagerW
432 stdcall OpenServiceA(long str long) advapibase.OpenServiceA
433 stdcall OpenServiceW(long wstr long) advapibase.OpenServiceW
434 stdcall OpenThreadToken(long long long ptr) advapibase.OpenThreadToken
435 stdcall OpenTraceA(ptr) advapibase.OpenTraceA
436 stdcall OpenTraceW(ptr) advapibase.OpenTraceW
437 stdcall PrivilegeCheck(ptr ptr ptr) advapibase.PrivilegeCheck
438 stdcall PrivilegedServiceAuditAlarmA(str str long ptr long) advapibase.PrivilegedServiceAuditAlarmA
439 stdcall PrivilegedServiceAuditAlarmW(wstr wstr long ptr long) advapibase.PrivilegedServiceAuditAlarmW
440 stdcall ProcessIdleTasks() advapibase.ProcessIdleTasks
441 stdcall ProcessTrace(ptr long ptr ptr) advapibase.ProcessTrace
442 stdcall QueryAllTracesA(ptr long ptr) advapibase.QueryAllTracesA ;ntdll.EtwQueryAllTracesA
443 stdcall QueryAllTracesW(ptr long ptr) advapibase.QueryAllTracesW ;ntdll.EtwQueryAllTracesW
444 stdcall QueryRecoveryAgentsOnEncryptedFile(wstr ptr) advapibase.QueryRecoveryAgentsOnEncryptedFile
445 stdcall QueryServiceConfig2A(long long ptr long ptr) advapibase.QueryServiceConfig2A
446 stdcall QueryServiceConfig2W(long long ptr long ptr) advapibase.QueryServiceConfig2W
447 stdcall QueryServiceConfigA(long ptr long ptr) advapibase.QueryServiceConfigA
448 stdcall QueryServiceConfigW(long ptr long ptr) advapibase.QueryServiceConfigW
449 stdcall QueryServiceLockStatusA(long ptr long ptr) advapibase.QueryServiceLockStatusA
450 stdcall QueryServiceLockStatusW(long ptr long ptr) advapibase.QueryServiceLockStatusW
451 stdcall QueryServiceObjectSecurity(long long ptr long ptr) advapibase.QueryServiceObjectSecurity
452 stdcall QueryServiceStatus(long ptr) advapibase.QueryServiceStatus
453 stdcall QueryServiceStatusEx(long long ptr long ptr) advapibase.QueryServiceStatusEx
454 stdcall QueryTraceA(double str ptr) advapibase.QueryTraceA ;ntdll.EtwQueryTraceA
455 stdcall QueryTraceW(double str ptr) advapibase.QueryTraceW ;ntdll.EtwQueryTraceA
456 stdcall QueryUsersOnEncryptedFile(wstr ptr) advapibase.QueryUsersOnEncryptedFile
457 stdcall ReadEncryptedFileRaw(ptr ptr ptr) advapibase.ReadEncryptedFileRaw
458 stdcall ReadEventLogA(long long long ptr long ptr ptr) advapibase.ReadEventLogA
459 stdcall ReadEventLogW(long long long ptr long ptr ptr) advapibase.ReadEventLogW
460 stdcall RegCloseKey(long) advapibase.RegCloseKey
461 stdcall RegConnectRegistryA(str long ptr) advapibase.RegConnectRegistryA
464 stdcall RegConnectRegistryW(wstr long ptr) advapibase.RegConnectRegistryW
465 stdcall RegCreateKeyA(long str ptr) advapibase.RegCreateKeyA
466 stdcall RegCreateKeyExA(long str long ptr long long ptr ptr ptr) advapibase.RegCreateKeyExA
467 stdcall RegCreateKeyExW(long wstr long ptr long long ptr ptr ptr) advapibase.RegCreateKeyExW
468 stdcall RegCreateKeyW(long wstr ptr) advapibase.RegCreateKeyW
469 stdcall RegDeleteKeyA(long str) advapibase.RegDeleteKeyA
472 stdcall RegDeleteKeyW(long wstr) advapibase.RegDeleteKeyW
473 stdcall RegDeleteValueA(long str) advapibase.RegDeleteValueA
474 stdcall RegDeleteValueW(long wstr) advapibase.RegDeleteValueW
475 stdcall RegDisablePredefinedCache() advapibase.RegDisablePredefinedCache
478 stdcall RegEnumKeyA(long long ptr long) advapibase.RegEnumKeyA
479 stdcall RegEnumKeyExA(long long ptr ptr ptr ptr ptr ptr) advapibase.RegEnumKeyExA
480 stdcall RegEnumKeyExW(long long ptr ptr ptr ptr ptr ptr) advapibase.RegEnumKeyExW
481 stdcall RegEnumKeyW(long long ptr long) advapibase.RegEnumKeyW
482 stdcall RegEnumValueA(long long ptr ptr ptr ptr ptr ptr) advapibase.RegEnumValueA
483 stdcall RegEnumValueW(long long ptr ptr ptr ptr ptr ptr) advapibase.RegEnumValueW
484 stdcall RegFlushKey(long) advapibase.RegFlushKey
485 stdcall RegGetKeySecurity(long long ptr ptr) advapibase.RegGetKeySecurity
488 stdcall RegLoadKeyA(long str str) advapibase.RegLoadKeyA
489 stdcall RegLoadKeyW(long wstr wstr) advapibase.RegLoadKeyW
490 stdcall RegNotifyChangeKeyValue(long long long long long) advapibase.RegNotifyChangeKeyValue
491 stdcall RegOpenCurrentUser(long ptr) advapibase.RegOpenCurrentUser
492 stdcall RegOpenKeyA(long str ptr) advapibase.RegOpenKeyA
493 stdcall RegOpenKeyExA(long str long long ptr) advapibase.RegOpenKeyExA
494 stdcall RegOpenKeyExW(long wstr long long ptr) advapibase.RegOpenKeyExW
495 stdcall RegOpenKeyW(long wstr ptr) advapibase.RegOpenKeyW
496 stdcall RegOpenUserClassesRoot(ptr long long ptr) advapibase.RegOpenUserClassesRoot
497 stdcall RegOverridePredefKey(long long) advapibase.RegOverridePredefKey
498 stdcall RegQueryInfoKeyA(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr) advapibase.RegQueryInfoKeyA
499 stdcall RegQueryInfoKeyW(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr) advapibase.RegQueryInfoKeyW
500 stdcall RegQueryMultipleValuesA(long ptr long ptr ptr) advapibase.RegQueryMultipleValuesA
501 stdcall RegQueryMultipleValuesW(long ptr long ptr ptr) advapibase.RegQueryMultipleValuesW
503 stdcall RegQueryValueA(long str ptr ptr) advapibase.RegQueryValueA
504 stdcall RegQueryValueExA(long str ptr ptr ptr ptr) advapibase.RegQueryValueExA
505 stdcall RegQueryValueExW(long wstr ptr ptr ptr ptr) advapibase.RegQueryValueExW
506 stdcall RegQueryValueW(long wstr ptr ptr) advapibase.RegQueryValueW
507 stdcall RegReplaceKeyA(long str str str) advapibase.RegReplaceKeyA
508 stdcall RegReplaceKeyW(long wstr wstr wstr) advapibase.RegReplaceKeyW
509 stdcall RegRestoreKeyA(long str long) advapibase.RegRestoreKeyA
510 stdcall RegRestoreKeyW(long wstr long) advapibase.RegRestoreKeyW
511 stdcall RegSaveKeyA(long ptr ptr) advapibase.RegSaveKeyA
512 stdcall RegSaveKeyExA(long str ptr long) advapibase.RegSaveKeyExA
513 stdcall RegSaveKeyExW(long str ptr long) advapibase.RegSaveKeyExW
514 stdcall RegSaveKeyW(long ptr ptr) advapibase.RegSaveKeyW
515 stdcall RegSetKeySecurity(long long ptr) advapibase.RegSetKeySecurity
516 stdcall RegSetValueA(long str long ptr long) advapibase.RegSetValueA
517 stdcall RegSetValueExA(long str long long ptr long) advapibase.RegSetValueExA
518 stdcall RegSetValueExW(long wstr long long ptr long) advapibase.RegSetValueExW
519 stdcall RegSetValueW(long wstr long ptr long) advapibase.RegSetValueW
520 stdcall RegUnLoadKeyA(long str) advapibase.RegUnLoadKeyA
521 stdcall RegUnLoadKeyW(long wstr) advapibase.RegUnLoadKeyW
522 stdcall RegisterEventSourceA(ptr ptr) advapibase.RegisterEventSourceA
523 stdcall RegisterEventSourceW(ptr ptr) advapibase.RegisterEventSourceW
524 stdcall RegisterIdleTask(long long ptr ptr) advapibase.RegisterIdleTask
525 stdcall RegisterServiceCtrlHandlerA(str ptr) advapibase.RegisterServiceCtrlHandlerA
526 stdcall RegisterServiceCtrlHandlerExA(str ptr ptr) advapibase.RegisterServiceCtrlHandlerExA
527 stdcall RegisterServiceCtrlHandlerExW(wstr ptr ptr) advapibase.RegisterServiceCtrlHandlerExW
528 stdcall RegisterServiceCtrlHandlerW(wstr ptr) advapibase.RegisterServiceCtrlHandlerW
529 stdcall RegisterTraceGuidsA(ptr ptr ptr long ptr str str ptr) advapibase.RegisterTraceGuidsA ;ntdll.EtwRegisterTraceGuidsA
530 stdcall RegisterTraceGuidsW(ptr ptr ptr long ptr wstr wstr ptr) advapibase.RegisterTraceGuidsW ;ntdll.EtwRegisterTraceGuidsW
531 stdcall RemoveTraceCallback(ptr) advapibase.RemoveTraceCallback
532 stdcall RemoveUsersFromEncryptedFile(wstr ptr) advapibase.RemoveUsersFromEncryptedFile
533 stdcall ReportEventA(long long long long ptr long long str ptr) advapibase.ReportEventA
534 stdcall ReportEventW(long long long long ptr long long wstr ptr) advapibase.ReportEventW
535 stdcall RevertToSelf() advapibase.RevertToSelf
536 stdcall SaferCloseLevel(ptr) advapibase.SaferCloseLevel
537 stdcall SaferComputeTokenFromLevel(ptr ptr ptr long ptr) advapibase.SaferComputeTokenFromLevel
538 stdcall SaferCreateLevel(long long long ptr ptr) advapibase.SaferCreateLevel
539 stdcall SaferGetLevelInformation(ptr long ptr long ptr) advapibase.SaferGetLevelInformation
540 stdcall SaferGetPolicyInformation(long long long ptr ptr ptr) advapibase.SaferGetPolicyInformation
541 stdcall SaferIdentifyLevel(long ptr ptr ptr) advapibase.SaferIdentifyLevel
542 stdcall SaferRecordEventLogEntry(ptr wstr ptr) advapibase.SaferRecordEventLogEntry
543 stdcall SaferSetLevelInformation(ptr long ptr long) advapibase.SaferSetLevelInformation
544 stdcall SaferSetPolicyInformation(long long long ptr ptr) advapibase.SaferSetPolicyInformation
545 stdcall SaferiChangeRegistryScope(ptr long) advapibase.SaferiChangeRegistryScope
546 stdcall SaferiCompareTokenLevels(long long long) advapibase.SaferiCompareTokenLevels
547 stdcall SaferiIsExecutableFileType(wstr long) advapibase.SaferiIsExecutableFileType
548 stdcall SaferiPopulateDefaultsInRegistry(ptr long) advapibase.SaferiPopulateDefaultsInRegistry
549 stdcall SaferiRecordEventLogEntry() advapibase.SaferiRecordEventLogEntry
550 stdcall SaferiReplaceProcessThreadTokens(ptr ptr ptr) advapibase.SaferiReplaceProcessThreadTokens
551 stdcall SaferiSearchMatchingHashRules(long ptr long long ptr ptr) advapibase.SaferiSearchMatchingHashRules
552 stdcall SetAclInformation(ptr ptr long long) advapibase.SetAclInformation
553 stdcall SetEntriesInAccessListA(long ptr long str ptr ptr) advapibase.SetEntriesInAccessListA
554 stdcall SetEntriesInAccessListW(long ptr long wstr ptr ptr) advapibase.SetEntriesInAccessListW
555 stdcall SetEntriesInAclA(long ptr ptr ptr) advapibase.SetEntriesInAclA
556 stdcall SetEntriesInAclW(long ptr ptr ptr) advapibase.SetEntriesInAclW
557 stdcall SetEntriesInAuditListA(long ptr long str ptr ptr) advapibase.SetEntriesInAuditListA
558 stdcall SetEntriesInAuditListW(long ptr long wstr ptr ptr) advapibase.SetEntriesInAuditListW
559 stdcall SetFileSecurityA(str long ptr) advapibase.SetFileSecurityA
560 stdcall SetFileSecurityW(wstr long ptr) advapibase.SetFileSecurityW
561 stdcall SetInformationCodeAuthzLevelW() advapibase.SetInformationCodeAuthzLevelW
562 stdcall SetInformationCodeAuthzPolicyW() advapibase.SetInformationCodeAuthzPolicyW
563 stdcall SetKernelObjectSecurity(long long ptr) advapibase.SetKernelObjectSecurity
564 stdcall SetNamedSecurityInfoA(str long ptr ptr ptr ptr ptr) advapibase.SetNamedSecurityInfoA
565 stdcall SetNamedSecurityInfoExA(str long long str ptr ptr str str ptr) advapibase.SetNamedSecurityInfoExA
566 stdcall SetNamedSecurityInfoExW(wstr long long wstr ptr ptr wstr wstr ptr) advapibase.SetNamedSecurityInfoExW
567 stdcall SetNamedSecurityInfoW(wstr long ptr ptr ptr ptr ptr) advapibase.SetNamedSecurityInfoW
568 stdcall SetPrivateObjectSecurity(long ptr ptr ptr long) advapibase.SetPrivateObjectSecurity
569 stdcall SetPrivateObjectSecurityEx(long ptr ptr long ptr ptr) advapibase.SetPrivateObjectSecurityEx
570 stdcall SetSecurityDescriptorControl(ptr long long) advapibase.SetSecurityDescriptorControl
571 stdcall SetSecurityDescriptorDacl(ptr long ptr long) advapibase.SetSecurityDescriptorDacl
572 stdcall SetSecurityDescriptorGroup(ptr ptr long) advapibase.SetSecurityDescriptorGroup
573 stdcall SetSecurityDescriptorOwner(ptr ptr long) advapibase.SetSecurityDescriptorOwner
574 stdcall SetSecurityDescriptorRMControl(ptr ptr) advapibase.SetSecurityDescriptorRMControl
575 stdcall SetSecurityDescriptorSacl(ptr long ptr long) advapibase.SetSecurityDescriptorSacl
576 stdcall SetSecurityInfo(long long long ptr ptr ptr ptr) advapibase.SetSecurityInfo
577 stdcall SetSecurityInfoExA(ptr long long str ptr ptr str str ptr) advapibase.SetSecurityInfoExA
578 stdcall SetSecurityInfoExW(ptr long long wstr ptr ptr wstr wstr ptr) advapibase.SetSecurityInfoExW
579 stdcall SetServiceBits(long long long long) advapibase.SetServiceBits
580 stdcall SetServiceObjectSecurity(long long ptr) advapibase.SetServiceObjectSecurity
581 stdcall SetServiceStatus(long long) advapibase.SetServiceStatus
582 stdcall SetThreadToken(ptr ptr) advapibase.SetThreadToken
583 stdcall SetTokenInformation(long long ptr long) advapibase.SetTokenInformation
584 stdcall SetTraceCallback(ptr ptr) advapibase.SetTraceCallback
585 stdcall SetUserFileEncryptionKey(ptr) advapibase.SetUserFileEncryptionKey
586 stdcall StartServiceA(long long ptr) advapibase.StartServiceA
587 stdcall StartServiceCtrlDispatcherA(ptr) advapibase.StartServiceCtrlDispatcherA
588 stdcall StartServiceCtrlDispatcherW(ptr) advapibase.StartServiceCtrlDispatcherW
589 stdcall StartServiceW(long long ptr) advapibase.StartServiceW
590 stdcall StartTraceA(ptr str ptr) advapibase.StartTraceA ;ntdll.EtwStartTraceA
591 stdcall StartTraceW(ptr wstr ptr) advapibase.StartTraceW ;ntdll.EtwStartTraceW
592 stdcall StopTraceA(double str ptr) advapibase.StopTraceA ;ntdll.EtwStopTraceA
593 stdcall StopTraceW(double wstr ptr) advapibase.StopTraceW ;ntdll.EtwStopTraceA
594 stdcall SystemFunction001(ptr ptr ptr) advapibase.SystemFunction001
595 stdcall SystemFunction002(ptr ptr ptr) advapibase.SystemFunction002
596 stdcall SystemFunction003(ptr ptr) advapibase.SystemFunction003
597 stdcall SystemFunction004(ptr ptr ptr) advapibase.SystemFunction004
598 stdcall SystemFunction005(ptr ptr ptr) advapibase.SystemFunction005
599 stdcall SystemFunction006(ptr ptr) advapibase.SystemFunction006
600 stdcall SystemFunction007(ptr ptr) advapibase.SystemFunction007
601 stdcall SystemFunction008(ptr ptr ptr) advapibase.SystemFunction008
602 stdcall SystemFunction009(ptr ptr ptr) advapibase.SystemFunction009
603 stdcall SystemFunction010(ptr ptr ptr) advapibase.SystemFunction010
604 stdcall SystemFunction011(ptr ptr ptr) advapibase.SystemFunction011 ;SystemFunction010
605 stdcall SystemFunction012(ptr ptr ptr) advapibase.SystemFunction012
606 stdcall SystemFunction013(ptr ptr ptr) advapibase.SystemFunction013
607 stdcall SystemFunction014(ptr ptr ptr) advapibase.SystemFunction014 ;SystemFunction012
608 stdcall SystemFunction015(ptr ptr ptr) advapibase.SystemFunction015 ;SystemFunction013
609 stdcall SystemFunction016(ptr ptr ptr) advapibase.SystemFunction016 ;SystemFunction012
610 stdcall SystemFunction017(ptr ptr ptr) advapibase.SystemFunction017 ;SystemFunction013
611 stdcall SystemFunction018(ptr ptr ptr) advapibase.SystemFunction018 ;SystemFunction012
612 stdcall SystemFunction019(ptr ptr ptr) advapibase.SystemFunction019 ;SystemFunction013
613 stdcall SystemFunction020(ptr ptr ptr) advapibase.SystemFunction020 ;SystemFunction012
614 stdcall SystemFunction021(ptr ptr ptr) advapibase.SystemFunction021 ;SystemFunction013
615 stdcall SystemFunction022(ptr ptr ptr) advapibase.SystemFunction022 ;SystemFunction012
616 stdcall SystemFunction023(ptr ptr ptr) advapibase.SystemFunction023 ;SystemFunction013
617 stdcall SystemFunction024(ptr ptr ptr) advapibase.SystemFunction024
618 stdcall SystemFunction025(ptr ptr ptr) advapibase.SystemFunction025
619 stdcall SystemFunction026(ptr ptr ptr) advapibase.SystemFunction026 ;SystemFunction024
620 stdcall SystemFunction027(ptr ptr ptr) advapibase.SystemFunction027 ;SystemFunction025
621 stdcall SystemFunction028(long long) advapibase.SystemFunction028
622 stdcall SystemFunction029(long long) advapibase.SystemFunction029
623 stdcall SystemFunction030(ptr ptr) advapibase.SystemFunction030
624 stdcall SystemFunction031(ptr ptr) advapibase.SystemFunction031 ;SystemFunction030
625 stdcall SystemFunction032(ptr ptr) advapibase.SystemFunction032
626 stdcall SystemFunction033(long long) advapibase.SystemFunction033
627 stdcall SystemFunction034(long long) advapibase.SystemFunction034
628 stdcall SystemFunction035(str) advapibase.SystemFunction035
629 stdcall SystemFunction036(ptr long) advapibase.SystemFunction036 ;# RtlGenRandom
630 stdcall SystemFunction040(ptr long long) advapibase.SystemFunction040 ;# RtlEncryptMemory
631 stdcall SystemFunction041(ptr long long) advapibase.SystemFunction041 ;# RtlDecryptMemory
632 stdcall TraceEvent(double ptr) advapibase.TraceEvent ;ntdll.EtwTraceEvent
633 stdcall TraceEventInstance(double ptr ptr ptr) advapibase.TraceEventInstance ;ntdll.EtwTraceEventInstance
634 varargs TraceMessage(ptr long ptr long) advapibase.TraceMessage ;ntdll.EtwTraceMessage
635 stdcall TraceMessageVa(double long ptr long ptr) advapibase.TraceMessageVa ;ntdll.EtwTraceMessageVa
636 stdcall TreeResetNamedSecurityInfoA(str ptr ptr ptr ptr ptr ptr long ptr ptr ptr) advapibase.TreeResetNamedSecurityInfoA
637 stdcall TreeResetNamedSecurityInfoW(wstr long long ptr ptr ptr ptr long ptr long ptr) advapibase.TreeResetNamedSecurityInfoW
638 stdcall TrusteeAccessToObjectA(str long str ptr long ptr) advapibase.TrusteeAccessToObjectA
639 stdcall TrusteeAccessToObjectW(wstr long wstr ptr long ptr) advapibase.TrusteeAccessToObjectW
640 stdcall UninstallApplication(ptr long) advapibase.UninstallApplication
641 stdcall UnlockServiceDatabase(ptr) advapibase.UnlockServiceDatabase
642 stdcall UnregisterIdleTask(long ptr ptr) advapibase.UnregisterIdleTask
643 stdcall UnregisterTraceGuids(double) advapibase.UnregisterTraceGuids ;ntdll.EtwUnregisterTraceGuids
644 stdcall UpdateTraceA(double str ptr) advapibase.UpdateTraceA ;ntdll.EtwUpdateTraceA
645 stdcall UpdateTraceW(double wstr ptr) advapibase.UpdateTraceW ;ntdll.EtwUpdateTraceW
646 stdcall WdmWmiServiceMain(ptr long) advapibase.WdmWmiServiceMain
647 stdcall WmiCloseBlock(ptr) advapibase.WmiCloseBlock
648 stdcall WmiCloseTraceWithCursor(ptr) advapibase.WmiCloseTraceWithCursor
649 stdcall WmiConvertTimestamp(long long long) advapibase.WmiConvertTimestamp
650 stdcall WmiDevInstToInstanceNameA(ptr long str long) advapibase.WmiDevInstToInstanceNameA
651 stdcall WmiDevInstToInstanceNameW(ptr long wstr long) advapibase.WmiDevInstToInstanceNameW
652 stdcall WmiEnumerateGuids(long long) advapibase.WmiEnumerateGuids
653 stdcall WmiExecuteMethodA(long str long long long long long) advapibase.WmiExecuteMethodA
654 stdcall WmiExecuteMethodW(long wstr long long long long long) advapibase.WmiExecuteMethodW
655 stdcall WmiFileHandleToInstanceNameA(long long ptr long) advapibase.WmiFileHandleToInstanceNameA
656 stdcall WmiFileHandleToInstanceNameW(long long ptr long) advapibase.WmiFileHandleToInstanceNameW
657 stdcall WmiFreeBuffer(ptr) advapibase.WmiFreeBuffer
658 stdcall WmiGetFirstTraceOffset(long long) advapibase.WmiGetFirstTraceOffset
659 stdcall WmiGetNextEvent(long) advapibase.WmiGetNextEvent
660 stdcall WmiGetTraceHeader(long long long) advapibase.WmiGetTraceHeader
661 stdcall WmiMofEnumerateResourcesA(long long long) advapibase.WmiMofEnumerateResourcesA
662 stdcall WmiMofEnumerateResourcesW(long long long) advapibase.WmiMofEnumerateResourcesW
663 stdcall WmiNotificationRegistrationA(ptr long ptr long long) advapibase.WmiNotificationRegistrationA ;ntdll.EtwNotificationRegistrationA
664 stdcall WmiNotificationRegistrationW(ptr long ptr long long) advapibase.WmiNotificationRegistrationW ;ntdll.EtwNotificationRegistrationW
665 stdcall WmiOpenBlock(long long long) advapibase.WmiOpenBlock
666 stdcall WmiOpenTraceWithCursor(long long) advapibase.WmiOpenTraceWithCursor 
667 stdcall WmiParseTraceEvent(long long long long long) advapibase.WmiParseTraceEvent
668 stdcall WmiQueryAllDataA(long ptr ptr) advapibase.WmiQueryAllDataA
669 stdcall WmiQueryAllDataMultipleA(long long long ptr) advapibase.WmiQueryAllDataMultipleA
670 stdcall WmiQueryAllDataMultipleW(long long long ptr) advapibase.WmiQueryAllDataMultipleW
671 stdcall WmiQueryAllDataW(long ptr ptr) advapibase.WmiQueryAllDataW
672 stdcall WmiQueryGuidInformation(ptr ptr) advapibase.WmiQueryGuidInformation
673 stdcall WmiQuerySingleInstanceA(long str ptr ptr) advapibase.WmiQuerySingleInstanceA
674 stdcall WmiQuerySingleInstanceMultipleA(long long long long long) advapibase.WmiQuerySingleInstanceMultipleA
675 stdcall WmiQuerySingleInstanceMultipleW(long long long long long) advapibase.WmiQuerySingleInstanceMultipleW
676 stdcall WmiQuerySingleInstanceW(long wstr ptr ptr) advapibase.WmiQuerySingleInstanceW
677 stdcall WmiReceiveNotificationsA(long long long long) advapibase.WmiReceiveNotificationsA ;ntdll.EtwReceiveNotificationsA
678 stdcall WmiReceiveNotificationsW(long long long long) advapibase.WmiReceiveNotificationsW ;ntdll.EtwReceiveNotificationsW
679 stdcall WmiSetSingleInstanceA(long str long long ptr) advapibase.WmiSetSingleInstanceA
680 stdcall WmiSetSingleInstanceW(long wstr long long ptr) advapibase.WmiSetSingleInstanceW
681 stdcall WmiSetSingleItemA(long str long long long ptr) advapibase.WmiSetSingleItemA
682 stdcall WmiSetSingleItemW(long wstr long long long ptr) advapibase.WmiSetSingleItemW
683 stdcall Wow64Win32ApiEntry(long long long) advapibase.Wow64Win32ApiEntry
684 stdcall WriteEncryptedFileRaw(ptr ptr ptr) advapibase.WriteEncryptedFileRaw

#Only on XP
@ stdcall CreateProcessAsUserSecure(ptr str str ptr ptr long long ptr str ptr ptr) advapibase.CreateProcessAsUserSecure;only on XP (needed for synchronization)
@ stdcall QueryWindows31FilesMigration(long) advapibase.QueryWindows31FilesMigration
@ stdcall SynchronizeWindows31FilesAndWindowsNTRegistry(long long long long) advapibase.SynchronizeWindows31FilesAndWindowsNTRegistry  ;only on XP (needed for synchronization)

#Missing on XP
100 stdcall CreateProcessWithTokenW(ptr long wstr wstr long ptr wstr ptr ptr) advapibase.CreateProcessWithTokenW ;advapinew.CreateProcessWithTokenW
202 stdcall ElfReportEventAndSourceW(long long ptr long long long ptr ptr long long ptr ptr long ptr ptr) advapibase.ElfReportEventAndSourceW ;advapinew.ElfReportEventAndSourceW
297 stdcall I_QueryTagInformation(long long ptr) advapibase.I_QueryTagInformation
462 stdcall RegConnectRegistryExA(str ptr long ptr) advapibase.RegConnectRegistryExA
463 stdcall RegConnectRegistryExW(wstr ptr long ptr) advapibase.RegConnectRegistryExW
470 stdcall RegDeleteKeyExA(long str long long) advapibase.RegDeleteKeyExA
471 stdcall RegDeleteKeyExW(long wstr long long) advapibase.RegDeleteKeyExW
476 stdcall RegDisableReflectionKey(ptr) advapibase.RegDisableReflectionKey
477 stdcall RegEnableReflectionKey(ptr)	advapibase.RegEnableReflectionKey
502 stdcall RegQueryReflectionKey(ptr ptr)	advapibase.RegQueryReflectionKey ;native on Server 2003, but, missing on XP (needed for synchronization) 

#Missing on Server 2003 RTM
@ stdcall RegDisablePredefinedCacheEx() advapibase.RegDisablePredefinedCacheEx ;advapinew.RegDisablePredefinedCacheEx
486 stdcall RegGetValueA(long str str long ptr ptr ptr) advapibase.RegGetValueA ;advapinew.RegGetValueA
487 stdcall RegGetValueW(long wstr wstr long ptr ptr ptr) advapibase.RegGetValueW ;advapinew.RegGetValueW

@ stdcall LogonUserExExW(str str str long long ptr ptr ptr ptr ptr ptr) advapibase.LogonUserExExW

#Vista Functions implemented (without redirection)
@ stdcall EventActivityIdControl(long ptr)
@ stdcall EventEnabled(long long ptr)
@ stdcall EventProviderEnabled(int64 long long long)
@ stdcall EventRegister(ptr ptr ptr ptr)
@ stdcall EventUnregister(long long)
@ stdcall EventWrite(long long ptr long ptr)
@ stdcall EventWriteEndScenario(long long ptr long ptr)
@ stdcall EventWriteEx(long long ptr long long long ptr ptr long ptr)
@ stdcall EventWriteStartScenario(long long ptr long ptr)
@ stdcall EventWriteString(int64 long long long wstr)
@ stdcall EventWriteTransfer(int64 ptr ptr ptr long ptr)
@ stdcall AddMandatoryAce(ptr long long long ptr)
@ stdcall ProcessIdleTasksW() ProcessIdleTasks

#from Vista/Longhorn advapi32 (advapinew) 
@ stdcall AddUsersToEncryptedFileEx(long long long long)
@ stdcall AuditComputeEffectivePolicyBySid(ptr ptr long ptr)
@ stdcall AuditComputeEffectivePolicyByToken(ptr ptr long ptr)
@ stdcall AuditEnumerateCategories(ptr ptr)
@ stdcall AuditEnumeratePerUserPolicy(ptr)
@ stdcall AuditEnumerateSubCategories(ptr long ptr ptr)
@ stdcall AuditFree(ptr)
@ stdcall AuditLookupCategoryGuidFromCategoryId(ptr ptr)
@ stdcall AuditLookupCategoryIdFromCategoryGuid(ptr ptr)
@ stdcall AuditLookupCategoryNameA(ptr str)
@ stdcall AuditLookupCategoryNameW(ptr wstr)
@ stdcall AuditLookupSubCategoryNameA(ptr str)
@ stdcall AuditLookupSubCategoryNameW(ptr str)
@ stdcall AuditQueryPerUserPolicy(ptr ptr long ptr)
@ stdcall AuditQuerySecurity(long ptr)
@ stdcall AuditQuerySystemPolicy(ptr long ptr)
@ stdcall AuditSetPerUserPolicy(ptr ptr long)
@ stdcall AuditSetSecurity(ptr long)
@ stdcall AuditSetSystemPolicy(ptr long)
@ stdcall CheckAppInitBlockedServiceIdentity(long)
@ stdcall CloseThreadWaitChainSession(ptr)
@ stdcall ControlServiceExA(ptr long long ptr)
@ stdcall ControlServiceExW(ptr long long ptr)
@ stdcall CredBackupCredentials(long ptr ptr long long)
@ stdcall CredEncryptAndMarshalBinaryBlob(long long long)
@ stdcall CredFindBestCredentialA(str long long ptr) 
@ stdcall CredFindBestCredentialW(wstr long long ptr)
@ stdcall CredIsProtectedA(str ptr)
@ stdcall CredIsProtectedW(wstr ptr)
@ stdcall CredpConvertOneCredentialSize(long long)
@ stdcall CredpEncodeSecret(long ptr long long long)
@ stdcall CredProfileUnloaded()
@ stdcall CredProtectA(long str long str long ptr)
@ stdcall CredProtectW(long wstr long wstr long ptr)
@ stdcall CredReadByTokenHandle(long ptr long long long)
@ stdcall CredRestoreCredentials(ptr ptr long long)
@ stdcall CredUnprotectA(long str long str ptr)
@ stdcall CredUnprotectW(long wstr long wstr ptr)
@ stdcall EnableTraceEx(ptr ptr int64 long long long long long long long ptr) 
@ stdcall EnumerateTraceGuidsEx(long ptr long ptr long ptr)
@ stdcall EventAccessControl(ptr long ptr long long)
@ stdcall EventAccessQuery(ptr ptr ptr)
@ stdcall EventAccessRemove(ptr)
@ stdcall FlushEfsCache(ptr)
@ stdcall FreeEncryptedFileMetadata(ptr)
@ stdcall GetEncryptedFileMetadata(wstr ptr ptr)
@ stdcall GetThreadWaitChain(ptr long long long ptr ptr ptr)
@ stdcall I_ScQueryServiceConfig(long long long)
@ stdcall I_ScSendPnPMessage(long long long long long)
@ stdcall I_ScValidatePnPService(long long long)
@ stdcall InitiateShutdownA(str str long long long)
@ stdcall InitiateShutdownW(wstr wstr long long long)
@ stdcall IsValidRelativeSecurityDescriptor(ptr long long)
@ stdcall LsaManageSidNameMapping(long ptr ptr)
@ stdcall NotifyServiceStatusChange(ptr long ptr) 
@ stdcall NotifyServiceStatusChangeA(ptr long ptr) 
@ stdcall NotifyServiceStatusChangeW(ptr long ptr) NotifyServiceStatusChange
@ stdcall PerfAddCounters(ptr ptr long)
@ stdcall PerfCloseQueryHandle(ptr)
@ stdcall PerfCreateInstance(ptr ptr wstr long)
@ stdcall PerfDecrementULongCounterValue(ptr ptr long long)
@ stdcall PerfDecrementULongLongCounterValue(ptr ptr long long long)
@ stdcall PerfDeleteCounters(ptr ptr long)
@ stdcall PerfDeleteInstance(ptr ptr)
@ stdcall PerfEnumerateCounterSet(long ptr long long)
@ stdcall PerfEnumerateCounterSetInstances(long ptr ptr long long)
@ stdcall PerfIncrementULongCounterValue(ptr ptr long long)
@ stdcall PerfIncrementULongLongCounterValue(ptr ptr long long long)
@ stdcall PerfQueryInstance(ptr ptr wstr long)
@ stdcall PerfOpenQueryHandle(long ptr)
@ stdcall PerfQueryCounterData(ptr ptr long long)
@ stdcall PerfQueryCounterInfo(ptr ptr long long)
@ stdcall PerfQueryCounterSetRegistrationInfo(long long long long long long long)
@ stdcall PerfSetCounterSetInfo(ptr ptr long)
@ stdcall PerfSetCounterRefValue(ptr ptr long ptr)
@ stdcall PerfSetULongCounterValue(ptr ptr long long)
@ stdcall PerfSetULongLongCounterValue(ptr ptr long long long)
@ stdcall PerfStartProvider(ptr ptr ptr)
@ stdcall PerfStartProviderEx(ptr ptr ptr)
@ stdcall PerfStopProvider(ptr)
@ stdcall QuerySecurityAccessMask(long ptr)
@ stdcall OpenThreadWaitChainSession(long ptr)
@ stdcall RegCopyTreeA(ptr str ptr)
@ stdcall RegCopyTreeW(ptr str ptr)
@ stdcall RegCreateKeyExA(ptr str long ptr long long ptr ptr ptr) 
@ stdcall RegCreateKeyExW(ptr wstr long ptr long long ptr ptr ptr) 
@ stdcall RegCreateKeyTransactedA(ptr str long str long long ptr ptr ptr ptr ptr)
@ stdcall RegCreateKeyTransactedW(ptr wstr long wstr long long ptr ptr ptr ptr ptr)
@ stdcall RegDeleteKeyTransactedA(ptr str long long ptr ptr)
@ stdcall RegDeleteKeyTransactedW(ptr wstr long long ptr ptr)
@ stdcall RegDeleteKeyValueA(ptr str str)
@ stdcall RegDeleteKeyValueW(ptr wstr wstr) 
@ stdcall RegDeleteTreeA(long str)
@ stdcall RegDeleteTreeW(long wstr)
@ stdcall RegisterWaitChainCOMCallback(ptr ptr)
@ stdcall RegLoadAppKeyA(str ptr long long long)
@ stdcall RegLoadAppKeyW(wstr ptr long long long)
@ stdcall RegLoadKeyA(ptr str str)
@ stdcall RegLoadKeyW(ptr wstr wstr)
@ stdcall RegLoadMUIStringA(ptr str str long ptr long str)
@ stdcall RegLoadMUIStringW(ptr str str long ptr long str)
@ stdcall RegOpenKeyTransactedA(ptr str long long ptr ptr ptr)
@ stdcall RegOpenKeyTransactedW(ptr wstr long long ptr ptr ptr)
@ stdcall RegRenameKey(ptr wstr wstr)
@ stdcall RegSetKeyValueA(ptr str str long ptr long)
@ stdcall RegSetKeyValueW(ptr wstr wstr long ptr long)
@ stdcall SetEncryptedFileMetadata(wstr ptr ptr ptr long ptr)
@ stdcall SetSecurityAccessMask(long ptr)
@ stdcall SetUserFileEncryptionKeyEx(ptr long long ptr)
@ stdcall TreeSetNamedSecurityInfoA(str long long ptr ptr ptr ptr long)
@ stdcall TreeSetNamedSecurityInfoW(wstr long long ptr ptr ptr ptr long)
@ stdcall UsePinForEncryptedFilesA(long long long)
@ stdcall UsePinForEncryptedFilesW(long long long)

#Win7+
@ stdcall EnableTraceEx2(int64 ptr long long int64 int64 long ptr)
@ stdcall LsaLookupSids2(ptr long long ptr ptr ptr)

#Imports from wevtapibase (wevtapi from Longhorn Pre-Reset)
@ stdcall EvtAssertConfig() wevtapibase.EvtIntAssertConfig
@ stdcall EvtClearLog() wevtapibase.EvtIntClearLog
@ stdcall EvtClose() wevtapibase.EvtIntClose
@ stdcall EvtCompileTemplate() wevtapibase.EvtIntCompileTemplate
@ stdcall EvtCreateBookmark() wevtapibase.EvtIntCreateBookmark
@ stdcall EvtCreateMessageRenderContext() wevtapibase.EvtIntCreateMessageRenderContext
@ stdcall EvtCreateValuesRenderContext() wevtapibase.EvtIntCreateValuesRenderContext
@ stdcall EvtDeleteChannelConfig() wevtapibase.EvtIntDeleteChannelConfig
@ stdcall EvtExportLog() wevtapibase.EvtIntExportLog
@ stdcall EvtGetChannelConfigProperty() wevtapibase.EvtIntGetChannelConfigProperty
@ stdcall EvtGetEventMetadataProperty() wevtapibase.EvtIntGetEventMetadataProperty
@ stdcall EvtGetExtendedStatus() wevtapibase.EvtIntGetExtendedStatus
@ stdcall EvtGetMatchedQueryIds() wevtapibase.EvtIntGetMatchedQueryIds
@ stdcall EvtGetNamedObjectInformation() wevtapibase.EvtIntGetNamedObjectInformation
@ stdcall EvtGetPublisherMetadataProperty() wevtapibase.EvtIntGetPublisherMetadataProperty
@ stdcall EvtNext() wevtapibase.EvtIntNext
@ stdcall EvtNextBatch() wevtapibase.EvtIntNextBatch
@ stdcall EvtNextChannelPath() wevtapibase.EvtIntNextChannelPath
@ stdcall EvtNextEventMetadata() wevtapibase.EvtIntNextEventMetadata
@ stdcall EvtNextPublisherId() wevtapibase.EvtIntNextPublisherId
@ stdcall EvtOpenChannelConfig() wevtapibase.EvtIntOpenChannelConfig
@ stdcall EvtOpenChannelEnum() wevtapibase.EvtIntOpenChannelEnum
@ stdcall EvtOpenEventMetadataEnum() wevtapibase.EvtIntOpenEventMetadataEnum
@ stdcall EvtOpenPublisherEnum() wevtapibase.EvtIntOpenPublisherEnum
@ stdcall EvtOpenPublisherMetadata() wevtapibase.EvtIntOpenPublisherMetadata
@ stdcall EvtQueryLog() wevtapibase.EvtIntQueryLog
@ stdcall EvtRegisterSubscription() wevtapibase.EvtIntRegisterSubscription
@ stdcall EvtRender() wevtapibase.EvtIntRender
@ stdcall EvtRenderBookmark() wevtapibase.EvtIntRenderBookmark
@ stdcall EvtRetractConfig() wevtapibase.EvtIntRetractConfig
@ stdcall EvtSaveChannelConfig() wevtapibase.EvtIntSaveChannelConfig
@ stdcall EvtSeek() wevtapibase.EvtIntSeek
@ stdcall EvtSetChannelConfigProperty() wevtapibase.EvtIntSetChannelConfigProperty
@ stdcall EvtUpdateBookmark() wevtapibase.EvtIntUpdateBookmark
@ stdcall EvtUpdateSubscription() wevtapibase.EvtIntUpdateSubscription

#Imports from advapilong (advapi32 from Longhorn Pre-Reset)
@ stdcall EvtGetThreadCorrelationId()
@ stdcall EvtIsActive()
@ stdcall EvtRegisterPublisher()
@ stdcall EvtRegisterSubscriber()
@ stdcall EvtReport()
@ stdcall EvtReportText()
@ stdcall EvtScopePublisher()
@ stdcall EvtSetThreadCorrelationId()