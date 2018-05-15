/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    number.c

Abstract:

    This file contains functions that form properly formatted number and
    currency strings for a given locale.


Author:

    Skulltrail 18-March-2017
	
--*/

#include "main.h"

/**************************************************************************
 *              GetNumberFormatEx	(KERNEL32.@)
 */
INT 
WINAPI 
GetNumberFormatEx(
	LPCWSTR name, 
	DWORD flags,
    LPCWSTR value, 
	const NUMBERFMTW *format,
    LPWSTR number, int numout
)
{
  LCID lcid  = 0x0409;	

  lcid = LocaleNameToLCID(name, 0);
  if (!lcid)
    return 0;

  return GetNumberFormatW(lcid, flags, value, format, number, numout);
}


int 
WINAPI
GetCurrencyFormatEx(
  _In_opt_        LPCWSTR     lpLocaleName,
  _In_            DWORD       dwFlags,
  _In_            LPCWSTR     lpValue,
  _In_opt_  const CURRENCYFMT *lpFormat,
  _Out_opt_       LPWSTR      lpCurrencyStr,
  _In_            int         cchCurrency
)
{
  LCID lcid  = 0x0409;	

  lcid = LocaleNameToLCID(lpLocaleName, 0);
  if (!lcid)
    return 0;

  return GetCurrencyFormatW(lcid, dwFlags, lpValue, (const CURRENCYFMTW *)lpFormat, lpCurrencyStr, cchCurrency);
}

DWORD 
WINAPI 
NlsPadInt64ToUnicodeStr(ULONGLONG Val, unsigned int a2, int a3, int a4, int a5)
{
  unsigned int v6; // esi@5
  wchar_t *v7; // eax@7
  wchar_t *v8; // edx@7
  int v9; // edi@8
  wchar_t v10; // cx@9
  int v11; // eax@10
  int v12; // esi@11
  int v13; // ecx@13
  unsigned int v14; // ebx@16
  void *v15; // esi@16
  int v16; // edi@17
  int i; // ecx@17
  int v18; // esi@25
  DWORD v19; // [sp-8h] [bp-98h]@32
  DWORD v20; // [sp+4h] [bp-8Ch]@1
  int v21; // [sp+8h] [bp-88h]@1
  __int16 DstBuf[64]; // [sp+Ch] [bp-84h]@7

  v20 = 0;
  v21 = a4;
  if ( a2 >= 0x16 && a2 > 0x40 )
  {
    SetLastError(0x57u);
    return -1;
  }
  if ( a3 )
    v6 = 7;
  else
    v6 = a2;
  v7 = _ui64tow(Val, (wchar_t *)&DstBuf[v6], 10);
  v8 = v7;
  if ( v7 )
  {
    v9 = (int)(v7 + 1);
    do
    {
      v10 = *v7;
      ++v7;
    }
    while ( v10 );
    v11 = (signed int)((char *)v7 - v9) >> 1;
    if ( v11 < v6 )
    {
      v12 = v6 - v11;
      v11 += v12;
      do
      {
        --v8;
        --v12;
        *v8 = 48;
      }
      while ( v12 );
    }
    v13 = 0;
    if ( a3 )
    {
      if ( a2 >= 7 )
      {
        v14 = a2 - 7;
        v15 = &v8[v11];
        if ( a2 - 7 > 0 )
        {
          memset(v15, 3145776, v14 >> 1);
          v16 = (int)((char *)v15 + 4 * (v14 >> 1));
          for ( i = v14 & 1; i; --i )
          {
            *(WORD *)v16 = 48;
            v16 += 2;
          }
          v15 = (char *)v15 + 2 * v14;
          v13 = 0;
        }
        *(WORD *)v15 = v13;
      }
      else
      {
        *(&v8[a2 - 7] + v11) = 0;
      }
    }
    while ( *v8 != (WORD)v13 )
    {
      if ( v21 != v13 )
      {
        if ( a5 <= v13 )
          goto LABEL_32;
        v18 = v21;
        --a5;
        v21 += 2;
        *(WORD *)v18 = *v8;
      }
      ++v8;
      ++v20;
    }
    if ( v21 == v13 )
      return v20;
    if ( a5 > v13 )
    {
      *(WORD *)v21 = v13;
      return v20;
    }
LABEL_32:
    v19 = 122;
  }
  else
  {
    v19 = 87;
  }
  SetLastError(v19);
  return -1;
}

int 
WINAPI 
ParseDuration(
	ULONGLONG longNumber, 
	LPCWSTR lpFormat, 
	LPWSTR lpDurationStr, 
	int cchDuration
)
{
  int one; // ebx@1
  int two; // di@1
  int six; // edx@1
  int seven; // ecx@1
  signed __int16 three; // ax@2
  int eight; // eax@4
  int ten; // eax@5
  int eleven; // eax@6
  int twelve; // eax@7
  int threeten; // eax@8
  unsigned __int8 fourten; // cf@31
  ULONGLONG largeNumber; // qcx@31
  signed int sixteen; // eax@32
  __int64 sevenTeen; // qdi@35
  ULONGLONG eighteen; // qax@48
  int nineteen; // eax@53
  int twentyone; // ebx@63
  __int16 twentytwo; // ax@63
  signed __int16 twentythree; // [sp+Ch] [bp-10h]@1
  int twentythreea; // [sp+Ch] [bp-10h]@43
  signed __int16 twentyfive; // [sp+10h] [bp-Ch]@41
  int twentysix; // [sp+14h] [bp-8h]@1
  unsigned int twentyseven; // [sp+18h] [bp-4h]@1
  int twentyeitght; // [sp+30h] [bp+14h]@1

  twentysix = 0;
  twentyseven = 0;
  one =  (DWORD)lpFormat;
  two =  (DWORD)lpFormat;
  six = cchDuration != 0 ?  (DWORD)lpDurationStr : 0;
  twentyeitght = cchDuration != 0 ?  (DWORD)lpDurationStr : 0;
  seven = (DWORD)lpFormat;
  twentythree = *lpFormat;
  if ( !*lpFormat )
    goto LABEL_59;
  three = *lpFormat;
  while ( three != 100 )
  {
    eight = three - 39;
    if ( !eight )
    {
      do
        seven += 2;
      while ( seven && seven != 39 );
      goto LABEL_16;
    }
    ten = eight - 33;
    if ( !ten )
      goto LABEL_13;
    eleven = ten - 30;
    if ( eleven )
    {
      twelve = eleven - 2;
      if ( twelve )
      {
        threeten = twelve - 5;
        if ( threeten )
        {
          if ( threeten == 6 )
            twentyseven |= 2u;
        }
        else
        {
          twentyseven |= 4u;
        }
        goto LABEL_16;
      }
LABEL_13:
      twentyseven |= 8u;
      goto LABEL_16;
    }
    twentyseven |= 1u;
LABEL_16:
    seven += 2;
    three = seven;
    if ( !seven )
      goto LABEL_20;
  }
  twentyseven |= 0x10u;
  while ( 1 )
  {
LABEL_20:
    if ( two == 39 )
    {
      twentyone = one + 2;
      twentytwo = twentyone;
      if ( twentyone )
      {
        while ( twentytwo != 39 )
        {
          if ( six )
          {
            if ( twentysix >= cchDuration )
              goto LABEL_75;
            six = twentytwo;
LABEL_72:
            six += 2;
            twentyeitght = six;
          }
LABEL_73:
          twentyone += 2;
          twentytwo = twentyone;
          ++twentysix;
          lpFormat = (LPCWSTR)twentyone;
          if ( !twentyone )
            goto LABEL_58;
        }
        twentyone += 2;
        lpFormat =  (LPWSTR)twentyone;
        if ( twentyone != 39 )
          goto LABEL_58;
        if ( !six )
          goto LABEL_73;
        if ( twentysix >= cchDuration )
          goto LABEL_75;
        six = 39;
        goto LABEL_72;
      }
LABEL_59:
      if ( six )
        six = 0;
      return twentysix + 1;
    }
    if ( two == 72 )
      break;
    if ( two == 100 )
    {
      largeNumber = 864000000000i64;
LABEL_37:
      sevenTeen = 0;
LABEL_38:
      sevenTeen = 0;
      goto LABEL_41;
    }
    if ( two == 102 )
    {
      largeNumber = 1i64;
      if ( twentyseven < 2 )
        goto LABEL_37;
      sevenTeen = 10000000;
      goto LABEL_38;
    }
    if ( two == 104 )
      break;
    if ( two == 109 )
    {
      fourten = twentyseven < 8;
      largeNumber = 600000000i64;
      goto LABEL_32;
    }
    if ( two == 115 )
    {
      fourten = twentyseven < 4;
      largeNumber = 10000000i64;
LABEL_32:
      sixteen = (-fourten & 0xFFFFFFC4) + 60;
      goto LABEL_40;
    }
    if ( six )
    {
      if ( twentysix >= cchDuration )
        goto LABEL_75;
      six = two;
      six += 2;
      twentyeitght = six;
    }
    ++twentysix;
    lpFormat = (LPCWSTR)(one + 2);
LABEL_58:
    one = (DWORD)lpFormat;
    twentythree = *lpFormat;
    if ( !*lpFormat )
      goto LABEL_59;
    two = *lpFormat;
  }
  largeNumber = 36000000000i64;
  sixteen = twentyseven < 0x10 ? 0 : 24;
LABEL_40:
  sevenTeen = sixteen;
LABEL_41:
  twentyfive = twentythree;
  if ( twentythree == 72 )
    twentyfive = 104;
  twentythreea = 0;
  while ( *lpFormat == twentyfive || twentyfive == 104 && *lpFormat == 72 )
  {
    ++twentythreea;
    ++lpFormat;
  }
  eighteen = longNumber / largeNumber;
  if ( sevenTeen )
    eighteen %= sevenTeen;
  if ( twentythreea > 9 && twentyfive == 102 )
    twentythreea = 9;
  nineteen = NlsPadInt64ToUnicodeStr(
               eighteen,
               twentythreea,
               twentyfive == 102,
               twentyeitght,
               twentyeitght != 0 ? cchDuration - twentysix : 0);
  if ( nineteen < 0 )
    return 0;
  twentysix += nineteen;
  if ( !twentyeitght )
  {
LABEL_57:
    six = twentyeitght;
    goto LABEL_58;
  }
  if ( twentysix < cchDuration )
  {
    twentyeitght += 2 * nineteen;
    goto LABEL_57;
  }
LABEL_75:
  SetLastError(0x7Au);
  return 0;
}

int 
WINAPI 
GetDurationFormatWorker(
	LCID Locale, 
	int dwFlags, 
	const SYSTEMTIME *lpDuration, 
	ULONGLONG ullDuration, 
	LPCWSTR lpFormat, 
	LPWSTR lpDurationStr, 
	int cchDuration
)
{
  LPCWSTR format; // edi@1
  ULONG one; // ebx@2
  ULONG two; // esi@2
  LPCWSTR receive; // eax@4

  format = 0;
  if ( lpDuration )
  {
    one = (10000000 * lpDuration->wMilliseconds / 0x3E8
         + 10000000
         * (lpDuration->wSecond + 60 * (lpDuration->wMinute + 60 * (lpDuration->wHour + 24i64 * lpDuration->wDay)))) >> 32;
    two = 10000000 * lpDuration->wMilliseconds / 0x3E8
        + 10000000
        * (lpDuration->wSecond + 60 * (lpDuration->wMinute + 60 * (lpDuration->wHour + 24 * lpDuration->wDay)));
    format = 0;
  }
  else
  {
    one = (ULONG)ullDuration;
    two = (ULONG)ullDuration;
  }
  receive = lpFormat;
  return ParseDuration(ullDuration, receive, lpDurationStr, cchDuration);
}

/*
* @unimplemented
*/
int 
WINAPI 
GetDurationFormat(
  _In_       LCID Locale,
  _In_       DWORD dwFlags,
  _In_opt_   const SYSTEMTIME *lpDuration,
  _In_       ULONGLONG ullDuration,
  _In_opt_   LPCWSTR lpFormat,
  _Out_opt_  LPWSTR lpDurationStr,
  _In_       int cchDuration
)
{
  DWORD error; // [sp-4h] [bp-8h]@11

  if (cchDuration < 0 || !lpDurationStr && cchDuration || lpDuration && ullDuration )
  {
    error = 87;
  }
  else
  {
    if ( !(dwFlags & 0xBFFFFFFF) && (!lpFormat || !(dwFlags & 0x80000000)) )
      return GetDurationFormatWorker(Locale, dwFlags, lpDuration, ullDuration, lpFormat, lpDurationStr, cchDuration);
    error = 1004;
  }
  SetLastError(error);
  return 0;
}

int 
WINAPI
GetDurationFormatEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       DWORD dwFlags,
  _In_opt_   const SYSTEMTIME *lpDuration,
  _In_       ULONGLONG ullDuration,
  _In_opt_   LPCWSTR lpFormat,
  _Out_opt_  LPWSTR lpDurationStr,
  _In_       int cchDuration
)
{
  LCID lcid = 0x0409; // eax@1
  lcid = LocaleNameToLCID(lpLocaleName, 0);
  
  return GetDurationFormat(lcid, dwFlags, lpDuration, ullDuration, lpFormat, lpDurationStr, cchDuration);
}