/*
 * Copyright (C) 2007 Francois Gouget
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

#ifndef __WINE_BCRYPT_H
#define __WINE_BCRYPT_H

#ifndef WINAPI
#define WINAPI __stdcall
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#ifndef WINE_NTSTATUS_DECLARED
#define WINE_NTSTATUS_DECLARED
typedef LONG NTSTATUS;
#endif

// #define BCRYPT_ALGORITHM_NAME (const WCHAR []){'A','l','g','o','r','i','t','h','m','N','a','m','e',0}
// #define BCRYPT_AUTH_TAG_LENGTH (const WCHAR []){'A','u','t','h','T','a','g','L','e','n','g','t','h',0}
// #define BCRYPT_BLOCK_LENGTH (const WCHAR []){'B','l','o','c','k','L','e','n','g','t','h',0}
// #define BCRYPT_BLOCK_SIZE_LIST (const WCHAR []){'B','l','o','c','k','S','i','z','e','L','i','s','t',0}
// #define BCRYPT_CHAINING_MODE (const WCHAR []){'C','h','a','i','n','i','n','g','M','o','d','e',0}
// #define BCRYPT_EFFECTIVE_KEY_LENGTH (const WCHAR []){'E','f','f','e','c','t','i','v','e','K','e','y','L','e','n','g','t','h',0}
// #define BCRYPT_HASH_BLOCK_LENGTH (const WCHAR []){'H','a','s','h','B','l','o','c','k','L','e','n','g','t','h',0}
// #define BCRYPT_HASH_LENGTH (const WCHAR []){'H','a','s','h','D','i','g','e','s','t','L','e','n','g','t','h',0}
// #define BCRYPT_HASH_OID_LIST (const WCHAR []){'H','a','s','h','O','I','D','L','i','s','t',0}
// #define BCRYPT_KEY_LENGTH (const WCHAR []){'K','e','y','L','e','n','g','t','h',0}
// #define BCRYPT_KEY_LENGTHS (const WCHAR []){'K','e','y','L','e','n','g','t','h','s',0}
// #define BCRYPT_KEY_OBJECT_LENGTH (const WCHAR []){'K','e','y','O','b','j','e','c','t','L','e','n','g','t','h',0}
// #define BCRYPT_KEY_STRENGTH (const WCHAR []){'K','e','y','S','t','r','e','n','g','t','h',0}
// #define BCRYPT_OBJECT_LENGTH (const WCHAR []){'O','b','j','e','c','t','L','e','n','g','t','h',0}
// #define BCRYPT_PADDING_SCHEMES (const WCHAR []){'P','a','d','d','i','n','g','S','c','h','e','m','e','s',0}
// #define BCRYPT_PROVIDER_HANDLE (const WCHAR []){'P','r','o','v','i','d','e','r','H','a','n','d','l','e',0}
// #define BCRYPT_SIGNATURE_LENGTH (const WCHAR []){'S','i','g','n','a','t','u','r','e','L','e','n','g','t','h',0}

// #define MS_PRIMITIVE_PROVIDER (const WCHAR [])\
    // {'M','i','c','r','o','s','o','f','t',' ','P','r','i','m','i','t','i','v','e',' ','P','r','o','v','i','d','e','r',0}
// #define MS_PLATFORM_CRYPTO_PROVIDER (const WCHAR [])\
    // {'M','i','c','r','o','s','o','f','t',' ','P','l','a','t','f','o','r','m',' ','C','r','y','p','t','o',' ','P','r','o','v','i','d','e','r',0}

// #define BCRYPT_AES_ALGORITHM        (const WCHAR []){'A','E','S',0}
// #define BCRYPT_MD5_ALGORITHM        (const WCHAR []){'M','D','5',0}
// #define BCRYPT_RNG_ALGORITHM        (const WCHAR []){'R','N','G',0}
// #define BCRYPT_SHA1_ALGORITHM       (const WCHAR []){'S','H','A','1',0}
// #define BCRYPT_SHA256_ALGORITHM     (const WCHAR []){'S','H','A','2','5','6',0}
// #define BCRYPT_SHA384_ALGORITHM     (const WCHAR []){'S','H','A','3','8','4',0}
// #define BCRYPT_SHA512_ALGORITHM     (const WCHAR []){'S','H','A','5','1','2',0}

// #define BCRYPT_CHAIN_MODE_NA        (const WCHAR []){'C','h','a','i','n','i','n','g','M','o','d','e','N','/','A',0}
// #define BCRYPT_CHAIN_MODE_CBC       (const WCHAR []){'C','h','a','i','n','i','n','g','M','o','d','e','C','B','C',0}
// #define BCRYPT_CHAIN_MODE_ECB       (const WCHAR []){'C','h','a','i','n','i','n','g','M','o','d','e','E','C','B',0}

#define BCRYPT_RSA_ALGORITHM				L"RSA"
#define BCRYPT_RSA_SIGN_ALGORITHM			L"RSA_SIGN"
#define BCRYPT_DH_ALGORITHM				L"DH"
#define BCRYPT_DSA_ALGORITHM				L"DSA"
#define BCRYPT_RC2_ALGORITHM				L"RC2"
#define BCRYPT_RC4_ALGORITHM				L"RC4"
#define BCRYPT_AES_ALGORITHM				L"AES"
#define BCRYPT_DES_ALGORITHM				L"DES"
#define BCRYPT_DESX_ALGORITHM				L"DESX"
#define BCRYPT_3DES_ALGORITHM				L"3DES"
#define BCRYPT_3DES_112_ALGORITHM			L"3DES_112"
#define BCRYPT_MD2_ALGORITHM				L"MD2"
#define BCRYPT_MD4_ALGORITHM				L"MD4"
#define BCRYPT_MD5_ALGORITHM				L"MD5"
#define BCRYPT_SHA1_ALGORITHM				L"SHA1"
#define BCRYPT_SHA256_ALGORITHM				L"SHA256"
#define BCRYPT_SHA384_ALGORITHM				L"SHA384"
#define BCRYPT_SHA512_ALGORITHM				L"SHA512"
#define BCRYPT_AES_GMAC_ALGORITHM			L"AES-GMAC"
#define BCRYPT_ECDSA_P256_ALGORITHM			L"ECDSA_P256"
#define BCRYPT_ECDSA_P384_ALGORITHM			L"ECDSA_P384"
#define BCRYPT_ECDSA_P521_ALGORITHM			L"ECDSA_P521"
#define BCRYPT_ECDH_P256_ALGORITHM			L"ECDH_P256"
#define BCRYPT_ECDH_P384_ALGORITHM			L"ECDH_P384"
#define BCRYPT_ECDH_P521_ALGORITHM			L"ECDH_P521"
#define BCRYPT_RNG_ALGORITHM				L"RNG"
#define BCRYPT_RNG_FIPS186_DSA_ALGORITHM		L"FIPS186DSARNG"
#define BCRYPT_RNG_DUAL_EC_ALGORITHM			L"DUALECRNG"

#define MS_PRIMITIVE_PROVIDER				L"Microsoft Primitive Provider"

#define BCRYPT_ALG_HANDLE_HMAC_FLAG			0x00000008
#define BCRYPT_PROV_DISPATCH				0x00000001

#define BCRYPT_OBJECT_LENGTH				L"ObjectLength"
#define BCRYPT_ALGORITHM_NAME				L"AlgorithmName"
#define BCRYPT_PROVIDER_HANDLE				L"ProviderHandle"
#define BCRYPT_CHAINING_MODE				L"ChainingMode"
#define BCRYPT_BLOCK_LENGTH				L"BlockLength"
#define BCRYPT_KEY_LENGTH				L"KeyLength"
#define BCRYPT_KEY_OBJECT_LENGTH			L"KeyObjectLength"
#define BCRYPT_KEY_STRENGTH				L"KeyStrength"
#define BCRYPT_KEY_LENGTHS				L"KeyLengths"
#define BCRYPT_BLOCK_SIZE_LIST				L"BlockSizeList"
#define BCRYPT_EFFECTIVE_KEY_LENGTH			L"EffectiveKeyLength"
#define BCRYPT_HASH_LENGTH				L"HashDigestLength"
#define BCRYPT_HASH_OID_LIST				L"HashOIDList"
#define BCRYPT_PADDING_SCHEMES				L"PaddingSchemes"
#define BCRYPT_SIGNATURE_LENGTH				L"SignatureLength"
#define BCRYPT_HASH_BLOCK_LENGTH			L"HashBlockLength"
#define BCRYPT_AUTH_TAG_LENGTH				L"AuthTagLength"
#define BCRYPT_PRIMITIVE_TYPE				L"PrimitiveType"
#define BCRYPT_IS_KEYED_HASH	L"IsKeyedHash"

 #define BCRYPT_CHAIN_MODE_CBC L"ChainingModeCBC"
 #define BCRYPT_CHAIN_MODE_CCM L"ChainingModeCCM"
 #define BCRYPT_CHAIN_MODE_CFB L"ChainingModeCFB"
 #define BCRYPT_CHAIN_MODE_ECB L"ChainingModeECB"
 #define BCRYPT_CHAIN_MODE_GCM L"ChainingModeGCM"
 #define BCRYPT_CHAIN_MODE_NA L"ChainingModeN/A"
 #define BCRYPT_CHAINING_MODE L"ChainingMode"

typedef struct _BCRYPT_ALGORITHM_IDENTIFIER
{
    LPWSTR pszName;
    ULONG  dwClass;
    ULONG  dwFlags;
} BCRYPT_ALGORITHM_IDENTIFIER;

typedef struct __BCRYPT_KEY_LENGTHS_STRUCT
{
    ULONG dwMinLength;
    ULONG dwMaxLength;
    ULONG dwIncrement;
} BCRYPT_KEY_LENGTHS_STRUCT;

typedef PVOID BCRYPT_ALG_HANDLE;
typedef PVOID BCRYPT_KEY_HANDLE;
typedef PVOID BCRYPT_HANDLE;
typedef PVOID BCRYPT_HASH_HANDLE;

/* Flags for BCryptGenRandom */
#define BCRYPT_RNG_USE_ENTROPY_IN_BUFFER 0x00000001
#define BCRYPT_USE_SYSTEM_PREFERRED_RNG  0x00000002

/* Flags for BCryptOpenAlgorithmProvider */
#define BCRYPT_ALG_HANDLE_HMAC_FLAG 0x00000008

/* Flags for BCryptEncrypt/BCryptDecrypt */
#define BCRYPT_BLOCK_PADDING        0x00000001

NTSTATUS WINAPI BCryptCloseAlgorithmProvider(BCRYPT_ALG_HANDLE, ULONG);
NTSTATUS WINAPI BCryptCreateHash(BCRYPT_ALG_HANDLE, BCRYPT_HASH_HANDLE *, PUCHAR, ULONG, PUCHAR, ULONG, ULONG);
NTSTATUS WINAPI BCryptDecrypt(BCRYPT_KEY_HANDLE, PUCHAR, ULONG, VOID *, PUCHAR, ULONG, PUCHAR, ULONG, ULONG *, ULONG);
NTSTATUS WINAPI BCryptDestroyHash(BCRYPT_HASH_HANDLE);
NTSTATUS WINAPI BCryptDestroyKey(BCRYPT_KEY_HANDLE);
NTSTATUS WINAPI BCryptEncrypt(BCRYPT_KEY_HANDLE, PUCHAR, ULONG, VOID *, PUCHAR, ULONG, PUCHAR, ULONG, ULONG *, ULONG);
NTSTATUS WINAPI BCryptEnumAlgorithms(ULONG, ULONG *, BCRYPT_ALGORITHM_IDENTIFIER **, ULONG);
NTSTATUS WINAPI BCryptFinishHash(BCRYPT_HASH_HANDLE, PUCHAR, ULONG, ULONG);
NTSTATUS WINAPI BCryptGenerateSymmetricKey(BCRYPT_ALG_HANDLE, BCRYPT_KEY_HANDLE *, PUCHAR, ULONG, PUCHAR, ULONG, ULONG);
NTSTATUS WINAPI BCryptGenRandom(BCRYPT_ALG_HANDLE, PUCHAR, ULONG, ULONG);
NTSTATUS WINAPI BCryptGetFipsAlgorithmMode(BOOLEAN *);
NTSTATUS WINAPI BCryptGetProperty(BCRYPT_HANDLE, LPCWSTR, PUCHAR, ULONG, ULONG *, ULONG);
NTSTATUS WINAPI BCryptHash(BCRYPT_ALG_HANDLE, PUCHAR, ULONG, PUCHAR, ULONG, PUCHAR, ULONG);
NTSTATUS WINAPI BCryptHashData(BCRYPT_HASH_HANDLE, PUCHAR, ULONG, ULONG);
NTSTATUS WINAPI BCryptOpenAlgorithmProvider(BCRYPT_ALG_HANDLE *, LPCWSTR, LPCWSTR, ULONG);
NTSTATUS WINAPI BCryptSetProperty(BCRYPT_HANDLE, LPCWSTR, PUCHAR, ULONG, ULONG);
NTSTATUS WINAPI BCryptDuplicateHash(BCRYPT_HASH_HANDLE, BCRYPT_HASH_HANDLE *, UCHAR *, ULONG, ULONG);

#endif  /* __WINE_BCRYPT_H */
