@ stdcall CredDeleteA(str long long) advapi32.CredDeleteA
@ stdcall CredDeleteW(wstr long long) advapi32.CredDeleteW
@ stdcall CredEnumerateA(str long ptr ptr) advapi32.CredEnumerateA
@ stdcall CredEnumerateW(wstr long ptr ptr) advapi32.CredEnumerateW
@ stdcall CredFindBestCredentialA(str long long ptr) advapi32.CredFindBestCredentialA
@ stdcall CredFindBestCredentialW(wstr long long ptr) advapi32.CredFindBestCredentialW
@ stdcall CredFree(ptr) advapi32.CredFree
@ stdcall CredGetSessionTypes(long ptr) advapi32.CredGetSessionTypes
@ stdcall CredGetTargetInfoA(str long ptr) advapi32.CredGetTargetInfoA
@ stdcall CredGetTargetInfoW(wstr long ptr) advapi32.CredGetTargetInfoW
@ stdcall CredIsMarshaledCredentialW(wstr) advapi32.CredIsMarshaledCredentialW
@ stdcall CredIsProtectedA(str ptr) advapi32.CredIsProtectedA
@ stdcall CredIsProtectedW(wstr ptr) advapi32.CredIsProtectedW
@ stdcall CredMarshalCredentialA(long ptr ptr) advapi32.CredMarshalCredentialA
@ stdcall CredMarshalCredentialW(long ptr ptr) advapi32.CredMarshalCredentialW
@ stdcall CredProtectA(long str long str long ptr) advapi32.CredProtectA
@ stdcall CredProtectW(long wstr long wstr long ptr) advapi32.CredProtectW
@ stdcall CredReadA(str long long ptr) advapi32.CredReadA
@ stdcall CredReadDomainCredentialsA(ptr long ptr ptr) advapi32.CredReadDomainCredentialsA
@ stdcall CredReadDomainCredentialsW(ptr long ptr ptr) advapi32.CredReadDomainCredentialsW
@ stdcall CredReadW(wstr long long ptr) advapi32.CredReadW
@ stdcall CredUnmarshalCredentialA(str ptr ptr) advapi32.CredUnmarshalCredentialA
@ stdcall CredUnmarshalCredentialW(wstr ptr ptr) advapi32.CredUnmarshalCredentialW
@ stdcall CredUnprotectA(long str long str ptr) advapi32.CredUnprotectA
@ stdcall CredUnprotectW(long wstr long wstr ptr) advapi32.CredUnprotectW
@ stdcall CredWriteA(ptr long) advapi32.CredWriteA
@ stdcall CredWriteDomainCredentialsA(ptr ptr long) advapi32.CredWriteDomainCredentialsA
@ stdcall CredWriteDomainCredentialsW(ptr ptr long) advapi32.CredWriteDomainCredentialsW
@ stdcall CredWriteW(ptr long) advapi32.CredWriteW