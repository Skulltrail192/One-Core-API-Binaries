@ stdcall LsaAddAccountRights(ptr ptr ptr long) advapi32.LsaAddAccountRights
@ stdcall LsaClose(ptr) advapi32.LsaClose
@ stdcall LsaCreateSecret(ptr ptr long ptr) advapi32.LsaCreateSecret
@ stdcall LsaEnumerateAccountRights(ptr ptr ptr ptr) advapi32.LsaEnumerateAccountRights
@ stdcall LsaEnumerateAccountsWithUserRight(ptr ptr ptr ptr) advapi32.LsaEnumerateAccountsWithUserRight
@ stdcall LsaFreeMemory(ptr) advapi32.LsaFreeMemory
@ stdcall LsaICLookupNames(long long long ptr ptr long long long long long) advapi32.LsaICLookupNames
@ stdcall LsaICLookupNamesWithCreds(wstr wstr long long ptr long long long long long long long) advapi32.LsaICLookupNamesWithCreds
@ stdcall LsaICLookupSids(long long long ptr ptr long long long long) advapi32.LsaICLookupSids
@ stdcall LsaICLookupSidsWithCreds(wstr wstr long long ptr long long long long long long long) advapi32.LsaICLookupSidsWithCreds
@ stdcall LsaLookupNames2(ptr long long ptr ptr ptr) advapi32.LsaLookupNames2
@ stdcall LsaLookupSids(ptr long ptr ptr ptr) advapi32.LsaLookupSids
@ stdcall LsaLookupSids2(ptr long long ptr ptr ptr) advapi32.LsaLookupSids2
@ stdcall LsaOpenPolicy(ptr ptr long ptr) advapi32.LsaOpenPolicy
@ stdcall LsaOpenSecret(ptr ptr long ptr) advapi32.LsaOpenSecret
@ stdcall LsaQueryInformationPolicy(ptr long ptr) advapi32.LsaQueryInformationPolicy
@ stdcall LsaQuerySecret(ptr ptr ptr ptr ptr) advapi32.LsaQuerySecret
@ stdcall LsaRemoveAccountRights(ptr ptr long ptr long) advapi32.LsaRemoveAccountRights
@ stdcall LsaRetrievePrivateData(ptr ptr ptr) advapi32.LsaRetrievePrivateData
@ stdcall LsaSetInformationPolicy(ptr long ptr) advapi32.LsaSetInformationPolicy
@ stdcall LsaSetSecret(ptr ptr ptr) advapi32.LsaSetSecret
@ stdcall LsaStorePrivateData(ptr ptr ptr) advapi32.LsaStorePrivateData