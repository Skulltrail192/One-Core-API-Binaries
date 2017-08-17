/*
 * Copyright 2002 Mike McCormack for CodeWeavers
 * Copyright 2005-2006 Juan Lang
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "crypt32_private.h"

PCTL_ENTRY 
WINAPI 
CertFindSubjectInCTL(
  _In_ DWORD         dwEncodingType,
  _In_ DWORD         dwSubjectType,
  _In_ void          *pvSubject,
  _In_ PCCTL_CONTEXT pCtlContext,
  _In_ DWORD         dwFlags
)
{
	return NULL;
}

HANDLE 
CreateFileU(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess, 
	DWORD dwShareMode, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
	DWORD dwCreationDisposition, 
	DWORD dwFlagsAndAttributes, 
	HANDLE hTemplateFile
)
{
	return NULL;
}

BOOL 
WINAPI 
CryptAcquireContextU(
   HCRYPTPROV *phProv,
   LPCWSTR    pszContainer,
   LPCWSTR    pszProvider,
   DWORD      dwProvType,
   DWORD      dwFlags
)
{
	return FALSE;
}

BOOL
CryptCloseAsyncHandle(
	HCRYPTASYNC hAsync
)
{
	return FALSE;
}

BOOL
WINAPI
CryptCreateAsyncHandle(
	DWORD dwFlags, 
	PHCRYPTASYNC phAsync
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptDecodeMessage(
	DWORD dwMsgTypeFlags, 
	PCRYPT_DECRYPT_MESSAGE_PARA pDecryptPara, 
	PCRYPT_VERIFY_MESSAGE_PARA pVerifyPara, 
	DWORD dwSignerIndex, 
	const BYTE *pbEncodedBlob, 
	DWORD cbEncodedBlob, 
	DWORD dwPrevInnerContentType, 
	DWORD *pdwMsgType, 
	DWORD *pdwInnerContentType, 
	BYTE *pbDecoded, 
	DWORD *pcbDecoded, 
	PCCERT_CONTEXT *ppXchgCert, 
	PCCERT_CONTEXT *ppSignerCert
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptDecryptAndVerifyMessageSignature(
	PCRYPT_DECRYPT_MESSAGE_PARA pDecryptPara, 
	PCRYPT_VERIFY_MESSAGE_PARA pVerifyPara, 
	DWORD dwSignerIndex, 
	const BYTE *pbEncryptedBlob, 
	DWORD cbEncryptedBlob, 
	BYTE *pbDecrypted, 
	DWORD *pcbDecrypted, 
	PCCERT_CONTEXT *ppXchgCert, 
	PCCERT_CONTEXT *ppSignerCert
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptDecryptMessage(
	PCRYPT_DECRYPT_MESSAGE_PARA pDecryptPara, 
	const BYTE *pbEncryptedBlob, 
	DWORD cbEncryptedBlob, 
	BYTE *pbDecrypted, 
	DWORD *pcbDecrypted, 
	PCCERT_CONTEXT *ppXchgCert
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptEnumOIDFunction(
	DWORD dwEncodingType, 
	LPCSTR pszFuncName, 
	LPCSTR pszOID, 
	DWORD dwFlags, 
	void *pvArg, 
	PFN_CRYPT_ENUM_OID_FUNC pfnEnumOIDFunc
)
{
	return FALSE;
}

BOOL
WINAPI CryptEnumProvidersU(
                           IN DWORD dwIndex,
                           IN DWORD *pdwReserved,
                           IN DWORD dwFlags,
                           OUT DWORD *pdwProvType,
                           OUT LPWSTR pwszProvName,
                           IN OUT DWORD *pcbProvName
                           )
{
	return FALSE;
}

BOOL 
WINAPI
CryptExportPKCS8(
	HCRYPTPROV hCryptProv, 
	DWORD dwKeySpec, 
	LPSTR pszPrivateKeyObjId, 
	DWORD dwFlags, 
	void *pvAuxInfo, 
	BYTE *pbPrivateKeyBlob, 
	DWORD *pcbPrivateKeyBlob
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptGetAsyncParam(
	HCRYPTASYNC hAsync, 
	LPSTR pszParamOid, 
	LPVOID *ppvParam, 
	PFN_CRYPT_ASYNC_PARAM_FREE_FUNC *ppfnFree
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptImportPKCS8(
	PVOID sPrivateKeyAndParams, 
	DWORD dwFlags, 
	HCRYPTPROV *phCryptProv, 
	void *pvAuxInfo
)
{
	return FALSE;
}

BOOL  
WINAPI
CryptLoadSip(
	const GUID *pgSubject, 
	DWORD dwFlags, 
	SIP_DISPATCH_INFO *pSipDispatch
)
{
	return FALSE;
}

DWORD 
WINAPI
CryptMsgCalculateEncodedLength(
	DWORD dwMsgEncodingType, 
	DWORD dwFlags, 
	DWORD dwMsgType, 
	const void *pvMsgEncodeInfo, 
	LPSTR pszInnerContentObjID, 
	DWORD cbData
)
{
	return 0;
}

BOOL 
WINAPI
CryptMsgCountersign(
	HCRYPTMSG hCryptMsg, 
	DWORD dwIndex, 
	DWORD cCountersigners, 
	PCMSG_SIGNER_ENCODE_INFO rgCountersigners
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptMsgCountersignEncoded(
	DWORD dwEncodingType, 
	PBYTE pbSignerInfo, 
	DWORD cbSignerInfo, 
	DWORD cCountersigners, 
	PCMSG_SIGNER_ENCODE_INFO rgCountersigners, 
	PBYTE pbCountersignature, 
	PDWORD pcbCountersignature
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptMsgVerifyCountersignatureEncoded(
	HCRYPTPROV_LEGACY hCryptProv, 
	DWORD dwEncodingType, 
	PBYTE pbSignerInfo, 
	DWORD cbSignerInfo, 
	PBYTE pbSignerInfoCountersignature, 
	DWORD cbSignerInfoCountersignature, 
	PCERT_INFO pciCountersigner
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptSetAsyncParam(
	HCRYPTASYNC hAsync, 
	LPSTR pszParamOid, 
	LPVOID pvParam, 
	PFN_CRYPT_ASYNC_PARAM_FREE_FUNC pfnFree
)
{
	return FALSE;
}

BOOL WINAPI CryptSetProviderU(
    LPCWSTR lpProvName,
    DWORD dwProvType) {
{
	return FALSE;
}

BOOL  
WINAPI
CryptSignAndEncryptMessage(
	PCRYPT_SIGN_MESSAGE_PARA pSignPara, 
	PCRYPT_ENCRYPT_MESSAGE_PARA pEncryptPara, 
	DWORD cRecipientCert, 
	PCCERT_CONTEXT *rgpRecipientCert, 
	const BYTE *pbToBeSignedAndEncrypted, 
	DWORD cbToBeSignedAndEncrypted, 
	BYTE *pbSignedAndEncryptedBlob, 
	DWORD *pcbSignedAndEncryptedBlob
)
{
	return FALSE;
}

BOOL WINAPI CryptSignHashU(
    HCRYPTHASH hHash,
    DWORD dwKeySpec,
    LPCWSTR lpDescription,
    DWORD dwFlags,
    BYTE *pbSignature,
    DWORD *pdwSigLen)
{
	return FALSE;
}

BOOL 
WINAPI
CryptSignMessageWithKey(
	PCRYPT_KEY_SIGN_MESSAGE_PARA pSignPara, 
	const BYTE *pbToBeSigned, 
	DWORD cbToBeSigned, 
	BYTE *pbSignedBlob, 
	DWORD *pcbSignedBlob
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptUnregisterOIDInfo(
	PCCRYPT_OID_INFO pInfo
)
{
	return FALSE;
}

BOOL 
WINAPI
CryptVerifyMessageSignatureWithKey(
	PCRYPT_KEY_VERIFY_MESSAGE_PARA pVerifyPara, 
	PCERT_PUBLIC_KEY_INFO pPublicKeyInfo, 
	const BYTE *pbSignedBlob, 
	DWORD cbSignedBlob, 
	BYTE *pbDecoded, 
	DWORD *pcbDecoded
)
{
	return FALSE;
}

BOOL
WINAPI
CryptVerifySignatureU(
	HCRYPTHASH a1, 
	BYTE *a2, 
	DWORD a3, 
	HCRYPTKEY a4, 
	const WCHAR *a5, 
	DWORD a6
)
{
	return FALSE;
}

HCRYPTPROV
WINAPI
I_CryptGetDefaultCryptProvForEncrypt(
    IN ALG_ID aiPubKey,
    IN ALG_ID aiEncrypt,
    IN DWORD dwBitLen
    )
{
	return FALSE;
}

DWORD 
WINAPI
I_CryptInsertLruEntry(
	volatile LONG *lpAddend, 
	int a2
)
{
	return 0;
}

DWORD
WINAPI
I_CryptReleaseLruEntry(
	volatile LONG *lpAddend
)
{
	return 0;
}

LSTATUS  
WINAPI
RegCreateHKCUKeyExU(
	HKEY this, 
	HKEY a2, 
	LPCWSTR lpSubKey, 
	DWORD Reserved, 
	LPWSTR lpClass, 
	DWORD dwOptions, 
	REGSAM samDesired, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult, 
	LPDWORD lpdwDisposition
)
{
	return 0;
}

LSTATUS 
WINAPI
RegCreateKeyExU(
	HKEY hKey, 
	LPCWSTR lpSubKey, 
	DWORD Reserved, 
	LPWSTR lpClass, 
	DWORD dwOptions, 
	REGSAM samDesired, 
	const LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
	PHKEY phkResult, 
	LPDWORD lpdwDisposition
)
{
	return 0;
}

LSTATUS 
WINAPI
RegDeleteValueU(
	HKEY hKey, 
	LPCWSTR lpValueName
)
{
	return 0;
}

LSTATUS 
WINAPI
RegEnumValueU(
	HKEY hKey, 
	DWORD dwIndex, 
	LPWSTR lpValueName, 
	LPDWORD lpcchValueName, 
	LPDWORD lpReserved, 
	LPDWORD lpType, 
	LPBYTE lpData, 
	LPDWORD lpcbData
)
{
	return 0;
}

LSTATUS 
WINAPI
RegOpenHKCUKeyExU(
	HKEY this,
	int a2, 
	LPCWSTR lpSubKey, 
	DWORD ulOptions, 
	REGSAM samDesired, 
	PHKEY phkResult
)
{
	return 0;
}

LSTATUS 
WINAPI
RegOpenKeyExU(
	HKEY hKey, 
	LPCWSTR lpSubKey, 
	DWORD ulOptions, 
	REGSAM samDesired, 
	PHKEY phkResult
)
{
	return 0;
}

LSTATUS 
WINAPI
RegQueryInfoKeyU(
	HKEY hKey, 
	LPWSTR lpClass,
	LPDWORD lpcchClass, 
	LPDWORD lpReserved, 
	LPDWORD lpcSubKeys, 
	LPDWORD lpcbMaxSubKeyLen, 
	LPDWORD lpcbMaxClassLen, 
	LPDWORD lpcValues, 
	LPDWORD lpcbMaxValueNameLen, 
	LPDWORD lpcbMaxValueLen, 
	LPDWORD lpcbSecurityDescriptor, 
	PFILETIME lpftLastWriteTime
)
{
	return 0;
}

LSTATUS 
WINAPI
RegQueryValueExU(
	HKEY hKey, 
	LPCWSTR lpValueName, 
	int a3, 
	LPDWORD lpType, 
	LPBYTE lpData, 
	LPDWORD lpcbData
)
{
	return 0;
}

LSTATUS  
WINAPI
RegSetValueExU(
	HKEY hKey, 
	LPCWSTR lpValueName, 
	DWORD Reserved, 
	DWORD dwType, 
	const BYTE *lpData,
	DWORD cbData
)
{
	return 0;
}

BOOL
WINAPI
I_CryptUninstallOssGlobal(
    IN PVOID hOssGlobal
    )
{
    return TRUE;
}

BOOL
WINAPI
CertAddCRLLinkToStore(
    IN HCERTSTORE hCertStore,
    IN PCCRL_CONTEXT pCrlContext,
    IN DWORD dwAddDisposition,
    OUT OPTIONAL PCCRL_CONTEXT *ppStoreContext
)
{
    return TRUE;
}

BOOL
WINAPI
CertAddCTLLinkToStore(
    IN HCERTSTORE hCertStore,
    IN PCCTL_CONTEXT pCtlContext,
    IN DWORD dwAddDisposition,
    OUT OPTIONAL PCCTL_CONTEXT *ppStoreContext
)
{
    return TRUE;
}

BOOL
WINAPI
CertCreateCTLEntryFromCertificateContextProperties(
    IN PCCERT_CONTEXT pCertContext,
    IN DWORD cOptAttr,
    IN OPTIONAL PCRYPT_ATTRIBUTE rgOptAttr,
    IN DWORD dwFlags,
    IN OPTIONAL void *pvReserved,
    OUT OPTIONAL PCTL_ENTRY pCtlEntry,
    IN OUT DWORD *pcbCtlEntry
)
{
    return TRUE;
}

BOOL
WINAPI
CertEnumSubjectInSortedCTL(
    IN PCCTL_CONTEXT pCtlContext,
    IN OUT void **ppvNextSubject,
    OUT OPTIONAL PCRYPT_DER_BLOB pSubjectIdentifier,
    OUT OPTIONAL PCRYPT_DER_BLOB pEncodedAttributes
)
{
    return TRUE;
}

BOOL
WINAPI
CertEnumSystemStoreLocation(
    IN DWORD dwFlags,
    IN void *pvArg,
    IN PFN_CERT_ENUM_SYSTEM_STORE_LOCATION pfnEnum
)
{
    return TRUE;
}

BOOL
WINAPI
CertRegisterSystemStore(
    IN const void *pvSystemStore,
    IN DWORD dwFlags,
    IN PCERT_SYSTEM_STORE_INFO pStoreInfo,
    IN OPTIONAL void *pvReserved
    )
{
    return TRUE;
}	

BOOL WINAPI
CertResyncCertificateChainEngine (
    IN HCERTCHAINENGINE hChainEngine
    )
{
    return TRUE;
}		

BOOL
WINAPI
CertSetCertificateContextPropertiesFromCTLEntry(
    IN PCCERT_CONTEXT pCertContext,
    IN PCTL_ENTRY pCtlEntry,
    IN DWORD dwFlags
    )
{
	return FALSE;
}

BOOL
WINAPI
CertUnregisterPhysicalStore(
    IN const void *pvSystemStore,
    IN DWORD dwFlags,
    IN LPCWSTR pwszStoreName
    )
{
	return FALSE;
}

BOOL
WINAPI
CertUnregisterSystemStore(
    IN const void *pvSystemStore,
    IN DWORD dwFlags
    )
{
	return FALSE;
}

BOOL WINAPI
ChainWlxLogoffEvent(
	PVOID pNotificationInfo
)
{
	return FALSE;
}

DWORD
APIENTRY
CloseCertPerformanceData()
{
    return ERROR_SUCCESS;
}

DWORD
APIENTRY
CollectCertPerformanceData(
    IN      LPWSTR  pwszValueName,
    IN OUT  LPVOID  *ppvData,
    IN OUT  LPDWORD pcbTotalBytes,
    IN OUT  LPDWORD pNumObjectTypes
    )
{
    return ERROR_SUCCESS;
}

BOOL
WINAPI
CryptCreateKeyIdentifierFromCSP(
    IN DWORD dwCertEncodingType,
    IN OPTIONAL LPCSTR pszPubKeyOID,
    IN const PUBLICKEYSTRUC *pPubKeyStruc,
    IN DWORD cbPubKeyStruc,
    IN DWORD dwFlags,
    IN OPTIONAL void *pvReserved,
    OUT BYTE *pbHash,
    IN OUT DWORD *pcbHash
    )
{
    return FALSE;
}

BOOL
WINAPI
CryptEnumKeyIdentifierProperties(
    IN OPTIONAL const CRYPT_HASH_BLOB *pKeyIdentifier,
    IN DWORD dwPropId,
    IN DWORD dwFlags,
    IN OPTIONAL LPCWSTR pwszComputerName,
    IN OPTIONAL void *pvReserved,
    IN OPTIONAL void *pvArg,
    IN PVOID pfnEnum
    )
{
    return FALSE;
}

BOOL
WINAPI
CryptGetKeyIdentifierProperty(
    IN const CRYPT_HASH_BLOB *pKeyIdentifier,
    IN DWORD dwPropId,
    IN DWORD dwFlags,
    IN OPTIONAL LPCWSTR pwszComputerName,
    IN OPTIONAL void *pvReserved,
    OUT void *pvData,
    IN OUT DWORD *pcbData
    )
{
    return FALSE;
}	

BOOL
WINAPI
CryptInstallDefaultContext(
    IN HCRYPTPROV hCryptProv,
    IN DWORD dwDefaultType,
    IN const void *pvDefaultPara,
    IN DWORD dwFlags,
    IN void *pvReserved,
    OUT HCRYPTDEFAULTCONTEXT *phDefaultContext
    )
{
    return FALSE;
}	

BOOL 
WINAPI 
CryptProtectMemory(
  _Inout_ LPVOID pData,
  _In_    DWORD  cbData,
  _In_    DWORD  dwFlags
)
{
    return FALSE;
}	

BOOL
WINAPI
CryptSetKeyIdentifierProperty(
    IN const CRYPT_HASH_BLOB *pKeyIdentifier,
    IN DWORD dwPropId,
    IN DWORD dwFlags,
    IN OPTIONAL LPCWSTR pwszComputerName,
    IN OPTIONAL void *pvReserved,
    IN const void *pvData
    )
{
    return FALSE;
}

BOOL 
WINAPI 
CryptSIPRetrieveSubjectGuidForCatalogFile(
	IN LPCWSTR FileName, 
	IN OPTIONAL HANDLE hFileIn, 
	OUT GUID *pgSubject
)
{
    return FALSE;
}

BOOL
WINAPI
CryptUninstallDefaultContext(
    HCRYPTDEFAULTCONTEXT hDefaultContext,
    IN DWORD dwFlags,
    IN void *pvReserved
    )
{
    return FALSE;
}

BOOL 
WINAPI 
CryptUnprotectMemory(
  _Inout_ LPVOID pData,
  _In_    DWORD  cbData,
  _In_    DWORD  dwFlags
)
{
    return FALSE;
}

BOOL
WINAPI
I_CertProtectFunction(
    IN DWORD dwFuncId,
    IN DWORD dwFlags,
    IN OPTIONAL LPCWSTR pwszIn,
    IN OPTIONAL BYTE *pbIn,
    IN DWORD cbIn,
    OUT OPTIONAL BYTE **ppbOut,
    OUT OPTIONAL DWORD *pcbOut
    )
{
    return FALSE;
}

DWORD
WINAPI
I_CertSrvProtectFunction(
    IN HANDLE hRpc,
    IN DWORD dwFuncId,
    IN DWORD dwFlags,
    IN LPCWSTR pwszIn,
    IN BYTE *pbIn,
    IN DWORD cbIn,
    OUT BYTE **ppbOut,
    OUT DWORD *pcbOut,
    IN PVOID pfnAlloc,
    IN PVOID pfnFree
    )
{
    return FALSE;
}

BOOL
WINAPI
I_CertSyncStore(
    IN OUT HCERTSTORE hOriginalStore,
    IN OUT HCERTSTORE hNewStore
    )
{
    return FALSE;
}

VOID WINAPI
I_CryptAddRefLruEntry (
       IN PVOID hEntry
       )
{
	;
}

BOOL WINAPI
I_CryptAddSmartCardCertToStore (
       IN HCERTSTORE hStore,
       IN PCRYPT_DATA_BLOB pEncodedCert,
       IN OPTIONAL LPWSTR pwszCertFriendlyName,
       IN PCRYPT_DATA_BLOB pSmartCardData,
       IN PCRYPT_KEY_PROV_INFO pKeyProvInfo
       )
{
    return FALSE;
}

VOID WINAPI
I_CryptDisableLruOfEntries (
       IN PVOID hCache
       )
{
	;
}	  

VOID WINAPI
I_CryptEnableLruOfEntries (
       IN PVOID hCache,
       IN OPTIONAL LPVOID pvLruRemovalContext
       )
{
	;
}	

PVOID WINAPI
I_CryptEnumMatchingLruEntries (
       IN PVOID hPrevEntry
       )
{
	return NULL;
}

PCCERT_CONTEXT WINAPI
I_CryptFindSmartCardCertInStore (
       IN HCERTSTORE hStore,
       IN PCCERT_CONTEXT pPrevCert,
       IN OPTIONAL PVOID pFindData,
       IN OUT OPTIONAL PCRYPT_DATA_BLOB* ppSmartCardData
       )
{
	return NULL;
}

DWORD  
I_CryptGetFileVersion(
	LPCWSTR lpWideCharStr, 
	int a2, 
	int a3
)
{
	return ERROR_SUCCESS;
}

LPVOID WINAPI
I_CryptGetLruEntryData (
       IN PVOID hEntry
       )
{
	return NULL;
}	 

PCRYPT_DATA_BLOB WINAPI
I_CryptGetLruEntryIdentifier (
       IN PVOID hEntry
       )
{
	return NULL;
}	 

BOOL 
WINAPI
I_CryptRegisterSmartCardStore (
       IN LPCWSTR pwszCardName,
       IN OPTIONAL LPCWSTR pwszProvider,
       IN OPTIONAL DWORD dwProviderType,
       IN OPTIONAL LPCWSTR pwszContainer,
       IN DWORD dwFlags
)
{
	return FALSE;
}	

VOID WINAPI
I_CryptRemoveLruEntry (
       IN PVOID hEntry,
       IN DWORD dwFlags,
       IN LPVOID pvLruRemovalContext
       )
{
	;
}

VOID WINAPI
I_CryptToucPVOID (
       IN PVOID hEntry,
       IN DWORD dwFlags
)
{
	;
}

BOOL 
WINAPI
I_CryptUnregisterSmartCardStore (
       IN LPCWSTR pwszCardName
       )
{
	return FALSE;
}

VOID 
WINAPI
I_CryptWalkAllLruCacheEntries (
       IN PVOID hCache,
       IN PVOID pfnWalk,
       IN LPVOID pvParameter
)
{
	;
}

DWORD
APIENTRY
OpenCertPerformanceData(
    IN LPWSTR lpDeviceNames
    )
{
	return ERROR_SUCCESS;
}

VOID WINAPI
I_CryptTouchLruEntry (
       IN PVOID hEntry,
       IN DWORD dwFlags
       )
{
	;
}

BOOL
WINAPI
CertFindSubjectInSortedCTL(
    IN PCRYPT_DATA_BLOB pSubjectIdentifier,
    IN PCCTL_CONTEXT pCtlContext,
    IN DWORD dwFlags,
    IN void *pvReserved,
    OUT OPTIONAL PCRYPT_DER_BLOB pEncodedAttributes
    )
{
	return FALSE;
}

void DbgInitOSS(PVOID t)
{
	;
}

void DbgPrintf(LPCWSTR t)
{
	;
}