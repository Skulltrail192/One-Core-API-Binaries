/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
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
 *
 */

#include <main.h>
 
WINE_DEFAULT_DEBUG_CHANNEL(shell);

typedef enum tagGPFIDL_FLAGS { 
	GPFIDL_DEFAULT = 0x0000, // normal Win32 file name, servers and drive roots included 
	GPFIDL_ALTNAME = 0x0001, // short file name 
	GPFIDL_UNCPRINTER = 0x0002, // include UNC printer names too (non file system item) 
}GPFIDL_FLAGS;

HRESULT WINAPI PathComparePaths(LPWSTR external, LPCWSTR lpString)
{
  HRESULT resp; // eax@2
  int other; // ebx@5
  int one; // edi@5
  LPWSTR string; // esi@6
  int hum; // eax@10
  __int16 compare; // cx@10
  HRESULT respResult; // [sp+8h] [bp-20A0h]@1
  LPWSTR path = L""; // [sp+Ch] [bp-209Ch]@2

  respResult = 1;
  if ( (int)external & 0xFFFF0000 )
    resp = StringCchCopyW(path, 0x825u, external);
  else
    resp = SHGetFolderPathW(0, (int)external | 0x4000, 0, 0, path);
  if ( resp >= 0 )
  {
    if ( StringCchCopyW(path, 0x825u, lpString) >= 0 )
    {
      PathRemoveBackslashW(&path[2086]);
      PathRemoveBackslashW(path);
      other = lstrlenW(&path[2086]);
      one = lstrlenW(lpString);
      if ( other <= one )
      {
        string = path;
        if ( *string == 92 || 
		*path == 92)
          *string = 0;
        if ( 1)
        {
          if ( other >= one )
          {
            respResult = 2;
          }
          else
          {
            hum = (int)(string + 2);
            compare = *(string + 1);
            respResult = 8;
            if ( !compare )
              goto LABEL_23;
            do
            {
              if ( compare == 92 )
                break;
              hum += 2;
              compare = hum;
            }
            while ( hum );
            if ( !hum )
LABEL_23:
              respResult = 12;
          }
        }
      }
    }
  }
  return respResult;
}

HRESULT WINAPI _AccountName2PictureName(LPWSTR other, LPWSTR string, size_t size)
{
  HRESULT v3; // edi@2
  LPWSTR v4; // eax@3
  LPWSTR v5; // ebx@3
  DWORD nSize; // [sp+8h] [bp-62Ch]@5
  const WCHAR String2; // [sp+Ch] [bp-628h]@4
  const WCHAR Start; // [sp+20Ch] [bp-428h]@2
  WCHAR String1; // [sp+610h] [bp-24h]@5

  if ( !other )
    return 0x80004005u;
  v3 = StringCchCopyW((unsigned __int16 *)&Start, 0x201u, other);
  if ( v3 >= 0 )
  {
    v4 = StrChrW(&Start, 0x5Cu);
    v5 = v4;
    if ( v4 )
    {
      v3 = StringCchCopyNW((unsigned __int16 *)&String2, 0x100u, &Start, v4 - &Start);
      if ( v3 < 0 )
        return v3;
      nSize = 16;
      if ( GetComputerNameW(&String1, &nSize) == 1 )
      {
        if ( lstrcmpiW(&String1, &String2) )
          *v5 = 43;
        else
          v3 = StringCchCopyW((unsigned __int16 *)&Start, 0x201u, v5 + 1);
      }
      else
      {
        v3 = 0x80004005u;
      }
    }
    if ( v3 >= 0 )
      v3 = StringCchCopyW(string, size, &Start);
  }
  return v3;
}

DWORD WINAPI SHQualifyUsernameAsLocal(LPCWSTR lpString1, LPWSTR string, size_t size)
{
  DWORD v3; // edi@1
  LPWSTR v4; // eax@5
  int v6; // edi@7
  LPWSTR v7; // eax@7
  WCHAR hum; // cx@8
  DWORD nSize; // [sp+Ch] [bp-28h]@4
  WCHAR String2; // [sp+10h] [bp-24h]@4
  __int16 v12; // [sp+12h] [bp-22h]@9

  v3 = 0x80070057u;
  if ( lpString1 && string && !wcschr(lpString1, 0x40u) )
  {
    nSize = 16;
    if ( !GetComputerNameW(&String2, &nSize) )
      return GetLastError();
    v4 = wcschr(lpString1, 0x5Cu);
    if ( !v4 )
      return StringCchPrintfW(string, size, L"%s\\%s", &String2, lpString1);
    v6 = v4 - lpString1;
    v7 = &String2;
    do
    {
      hum = *v7;
      ++v7;
    }
    while ( hum );
    if ( (signed int)((char *)v7 - (char *)&v12) >> 1 == v6
      && CompareStringW(0x7Fu, 1u, lpString1, v6, &String2, v6) == 2 )
      return StringCchCopyW(string, size, lpString1);
    v3 = 0x80070057u;
  }
  return v3;
}

DWORD WINAPI BuildAccountSidFromRid(PDWORD a1, PSID a2)
{
  DWORD v2; // eax@1
  DWORD v3; // ebx@2
  DWORD v4; // eax@2
  HLOCAL v5; // edi@2
  struct _SID_IDENTIFIER_AUTHORITY *v6; // eax@3
  PDWORD v7; // ST1C_4@5
  PDWORD v8; // eax@5
  DWORD v10; // [sp+Ch] [bp-10h]@1
  DWORD nSubAuthority; // [sp+10h] [bp-Ch]@4
  LPBYTE *Buffer = NULL; // [sp+14h] [bp-8h]@1

  a2 = 0;
  v10 = 0;
  v2 = NetUserModalsGet(0, 2u, Buffer);
  if ( v2 )
  {
    v10 = v2;
  }
  else
  {
    v3 = (DWORD)GetSidSubAuthorityCount(Buffer[1]);
    v4 = GetSidLengthRequired(v3 + 1);
    v5 = LocalAlloc(0x40u, v4);
    if ( v5 )
    {
      v6 = GetSidIdentifierAuthority(Buffer[1]);
      if ( InitializeSid(v5, v6, v3 + 1) )
      {
        v3 = v3;
        nSubAuthority = 0;
        if ( v3 )
        {
          do
          {
            v7 = GetSidSubAuthority(Buffer[1], nSubAuthority);
            v8 = GetSidSubAuthority(v5, nSubAuthority++);
            *v8 = *v7;
          }
          while ( nSubAuthority < v3 );
        }
        a1 = GetSidSubAuthority(v5, v3);
        a2 = v5;
      }
      else
      {
        v10 = GetLastError();
        LocalFree(v5);
      }
    }
    else
    {
      v10 = GetLastError();
    }
    NetApiBufferFree(Buffer);
  }
  return v10;
}

BOOL WINAPI IsAccountGuest(LPCWSTR lpString1)
{
  HLOCAL hum; // eax@6
  HANDLE hnd; // esi@6
  BOOL compare; // [sp+8h] [bp-240h]@1
  HLOCAL hMem; // [sp+Ch] [bp-23Ch]@5
  enum _SID_NAME_USE peUse; // [sp+10h] [bp-238h]@3
  PSID pSid2; // [sp+14h] [bp-234h]@1
  DWORD cchReferencedDomainName; // [sp+18h] [bp-230h]@1
  DWORD cbSid; // [sp+1Ch] [bp-22Ch]@1
  LPCWSTR AccountName; // [sp+20h] [bp-228h]@2

  cbSid = 0;
  cchReferencedDomainName = 0;
  compare = 0;
  if ( !BuildAccountSidFromRid((PDWORD)0x1F5u, &pSid2) )
  {
    if ( (SHQualifyUsernameAsLocal(lpString1, (LPWSTR)&AccountName, 0x111u) & 0x80000000u) == 0 )
    {
      if ( !LookupAccountNameW(0, (LPCWSTR)&AccountName, 0, &cbSid, 0, &cchReferencedDomainName, &peUse) )
      {
        if ( GetLastError() == 122 )
        {
          hMem = LocalAlloc(0x40u, cbSid);
          if ( hMem )
          {
            hum = LocalAlloc(0x40u, 2 * cchReferencedDomainName);
            hnd = hum;
            if ( hum )
            {
              if ( LookupAccountNameW(
                     0,
                     (LPCWSTR)&AccountName,
                     hMem,
                     &cbSid,
                     (LPWSTR)hum,
                     &cchReferencedDomainName,
                     &peUse) )
                compare = EqualSid(hMem, pSid2);
              LocalFree(hnd);
            }
            LocalFree(hMem);
          }
        }
      }
    }
    LocalFree(pSid2);
  }
  return compare;
}

HRESULT WINAPI PathCchFindExtension(size_t a1, signed int a2, LPCWSTR *a3)
{
  LPCWSTR v3; // eax@1
  size_t v4; // edx@4
  LPCWSTR v5; // esi@4
  int v6; // ecx@5

  v3 = (LPCWSTR)a1;
  *a3 = 0;
  if ( !a1 || !a2 || a2 > 0x8000 || (v4 = a1 + 2 * a2, v5 = 0, a1 >= v4) )
    return 0x80070057u;
  do
  {
    v6 = *v3;
    if ( !*v3 )
      break;
    v6 = v6;
    if ( v6 == 32 )
      goto LABEL_21;
    if ( v6 == 46 )
    {
      v5 = v3;
      goto LABEL_9;
    }
    if ( v6 == 92 )
LABEL_21:
      v5 = 0;
LABEL_9:
    ++v3;
  }
  while ( v3 < (LPCWSTR)v4 );
  if ( v3 >= (LPCWSTR)v4 )
    return 0x80070057u;
  if ( v5 )
    v3 = v5;
  *a3 = v3;
  return 0;
}

BOOL WINAPI StrIsEqualWorker(LPCWSTR a1, LPCWSTR a2, int a3)
{
  LPCWSTR v3; // edx@2
  LPCWSTR v4; // esi@2
  int v5; // ecx@3
  int v6; // eax@5
  BOOL result; // eax@9

  if ( a3 )
  {
    v3 = a2;
    v4 = a1;
    do
    {
      v5 = *v4;
      ++v4;
      if ( (v5 - 65) <= 0x19 )
        v5 += 32;
      v6 = *v3;
      ++v3;
      if ( (v6 - 65) <= 0x19 )
        v6 += 32;
      --a3;
    }
    while ( a3 && v5 && v5 == v6 );
    result = v5 == v6;
  }
  else
  {
    result = 1;
  }
  return result;
}

BOOL WINAPI IsValidExtensionWorker(wchar_t *Str)
{
  BOOL result; // eax@1
  wchar_t *other; // eax@2
  wchar_t hum; // cx@3
  int compose; // esi@4

  result = 0;
  if ( Str )
  {
    other = Str;
    do
    {
      hum = *other;
      ++other;
    }
    while ( hum );
    compose = other - (Str + 1);
    result = (compose < 0x100 || compose == 256 && *Str == 46)
          && !wcschr(Str, 0x20u)
          && !wcschr(Str, 0x5Cu)
          && (!compose || !wcschr(Str + 1, 0x2Eu));
  }
  return result;
}

HRESULT WINAPI PathCchRenameExtension(LPCWSTR string, DWORD a2, wchar_t *Str)
{
  signed int v3; // ebx@2
  HRESULT result; // eax@7
  wchar_t v5; // dx@8
  unsigned int v6; // esi@10
  int v7; // ecx@10
  BOOL v8; // [sp+Ch] [bp-10h]@5
  size_t v9; // [sp+10h] [bp-Ch]@10
  LPWSTR a3; // [sp+14h] [bp-8h]@7

  if ( !string || (v3 = a2) == 0 || a2 > 0x8000 || !IsValidExtensionWorker(Str) )
    return 0x80070057u;
  v8 = StrIsEqualWorker(string, L"\\\\?\\", 4);
  if ( !v8 && a2 > 0x104 )
    v3 = 260;
  result = PathCchFindExtension((size_t)string, v3, &a3);
  if ( result >= 0 )
  {
    v5 = *Str;
    if ( *Str && (v5 != 46 || Str[1]) )
    {
      v6 = a3 - string;
      v7 = v3 - v6;
      v9 = v3 - v6;
      if ( v5 != 46 )
      {
        result = StringCchCopyExW(a3, v3, 
		L".", 
		&a3, &v9, 0);
        v7 = v9;
      }
      if ( result >= 0 )
        result = StringCchCopyW(a3, v7, Str);
      if ( result == 0x8007007A )
      {
        if ( v8 )
        {
          if ( v3 != 32768 )
            return result;
        }
        else
        {
          if ( v3 != 260 )
            return result;
        }
        result = 0x800700CEu;
      }
    }
    else
    {
      *a3 = 0;
      result = 0;
    }
  }
  return result;
}

HRESULT WINAPI SHCopyUserTile(DWORD lpStart, HLOCAL hMem, LPCWSTR lpFileName, unsigned __int16 *a4, unsigned int a5)
{
  HRESULT resp; // ebx@1
  const WCHAR *v6; // edi@4
  HANDLE v7; // edi@10
  DWORD NumberOfBytesRead; // [sp+Ch] [bp-4h]@7

  resp = 1;
  if ( a4 && a5 )
    *a4 = 0;
  v6 = hMem;
  if ( hMem )
  {
    if ( CompareStringW(0x7Fu, 1u, hMem, -1, L"bmp", -1) != 2 )
      return 0x800703EEu;
  }
  else
  {
    v6 = L"bmp";
  }
  if ( !1 )
  {
    hMem = 0;
    NumberOfBytesRead = 0;
    if (hMem && NumberOfBytesRead > 0 )
    {
      v7 = CreateFileW(lpFileName, 2u, 0, 0, 2u, 0x80u, 0);
      if ( v7 == (HANDLE)1 )
      {
        resp = GetLastError();
      }
      else
      {
        lpStart = 0;
        if ( WriteFile(v7, hMem, NumberOfBytesRead, (LPDWORD)lpStart, 0) )
          resp = 0;
        else
          resp = GetLastError();
        CloseHandle(v7);
        if ( resp >= 0 )
          goto LABEL_14;
        DeleteFileW(lpFileName);
      }
      if ( resp < 0 && a4 && a5 )
        *a4 = 0;
    }
    else
    {
      resp = 0x800703EEu;
    }
LABEL_14:
    LocalFree(hMem);
  }
  return resp;
}

HRESULT 
WINAPI 
NetGetUserPicture(DWORD lpStart, HLOCAL hMem, LPWSTR lpFileName, size_t a4, LPWSTR a5, size_t a6)
{
  LPWSTR path; // eax@1
  HRESULT result; // eax@3

  path = hMem;
  if ( !hMem )
    path = L"bmp";
  result = PathCchRenameExtension(lpFileName, a4, path);
  if ( result >= 0 )
    result = SHCopyUserTile(lpStart, hMem, lpFileName, a5, a6);
  return result;
}

HRESULT 
WINAPI 
SHGetUserPicturePathEx(LPCWSTR lpStart, DWORD sguppFlags, LPCWSTR pwszDesiredSrcExt, LPWSTR pwszPicPath, UINT picPathLen, LPWSTR pwszSrcPath, UINT pwszSrcPathTwo)
{
  LPCWSTR localStart; // edi@1
  UINT localSize; // esi@2
  LPWSTR string; // eax@3
  HRESULT result; // eax@5
  int size; // eax@10
  int subSize; // esi@10
  DWORD flags; // eax@10
  LPWSTR picturePath; // [sp-8h] [bp-E44h]@23
  ULONG nSize; // [sp+14h] [bp-E28h]@8
  WCHAR NameBuffer; // [sp+18h] [bp-E24h]@8
  LPWSTR otherPath = L""; // [sp+41Ch] [bp-A20h]@7
  WCHAR FileName; // [sp+C30h] [bp-20Ch]@5

  localStart = lpStart;
  if ( sguppFlags & 0x7FFFFFFC )
    return 0x80070057u;
  localSize = 0;
  if ( !pwszPicPath )
    return 0x80070057u;
  string = L"Microsoft\\User Account Pictures\\Default Pictures";
  if ( !(sguppFlags & 2) )
    string = L"Microsoft\\User Account Pictures";
  result = SHGetFolderPathAndSubDirW(0, (sguppFlags & 0x8023FFFF | 0x230000) >> 16, 0, 0, string, &FileName);
  if ( !result )
  {
    if ( sguppFlags & 3 )
    {
LABEL_15:
      result = StringCchCopyW(pwszPicPath, picPathLen, &FileName);
      if ( pwszSrcPath != (LPWSTR)localSize )
      {
        if ( pwszSrcPathTwo > localSize )
          result = StringCchCopyW(pwszSrcPath, pwszSrcPathTwo, otherPath);
      }
      return result;
    }
    StringCchCopyW(otherPath, 0x103u, &FileName);
    if ( !lpStart )
    {
      nSize = 513;
      if ( !GetUserNameExW(NameSamCompatible, &NameBuffer, &nSize) )
      {
        result = GetLastError();
        goto LABEL_14;
      }
      localStart = &NameBuffer;
    }
    size = lstrlenW(localStart);
    subSize = 256 - size;
    flags = GetTempPathW(256 - size, &FileName);
    if ( flags && flags <= subSize )
    {
      result = _AccountName2PictureName(localStart, otherPath, 0x201u);
      if ( result >= 0 )
      {
        //PathCchAppendEx(&FileName, 0x104u, otherPath, 0);
        result = NetGetUserPicture(
                   (DWORD)localStart,
                   (wchar_t *)pwszDesiredSrcExt,
                   &FileName,
                   0x104u,
                   otherPath,
                   0x104u);
        if ( result < 0 )
        {
          if ( sguppFlags & 0x80000000 )
          {
            if ( IsAccountGuest(localStart) )
              picturePath = L"guest.bmp";
            else
              picturePath = L"user.bmp";
            result = StringCchPrintfW(&FileName, 0x104u, L"%s\\%s", &otherPath, picturePath);
          }
        }
      }
    }
    else
    {
      result = 0x80070002;
    }
LABEL_14:
    localSize = 0;
    if ( result )
      return result;
    goto LABEL_15;
  }
  return result;
}

BOOL 
WINAPI 
SHGetPathFromIDListEx(
  _In_   PCIDLIST_ABSOLUTE pidl,
  _Out_  PWSTR pszPath,
  DWORD cchPath,
  GPFIDL_FLAGS uOpts
)
{
	return SHGetPathFromIDListW(pidl, pszPath);
}