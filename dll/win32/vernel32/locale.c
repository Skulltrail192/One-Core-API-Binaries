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
#include <winnls.h>
#include <nameprep.h>
#include <idndl.h>
#include <unimplemented.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

LCID gSystemLocale = 0;
int gpSysLocHashN = 0;

static LOCALE LocaleList[3];

void WINAPI InitTable()
{
	LocaleList[0].description = L"English - United States"; LocaleList[0].cultureName = L"en-US"; LocaleList[0].lcidHex = 0x0409; LocaleList[0].lcidDec = 1033;	
	LocaleList[1].description = L"Portuguese - Brazil"; LocaleList[1].cultureName = L"pt-BR"; LocaleList[1].lcidHex = 0x0416; LocaleList[1].lcidDec = 1046;
	LocaleList[2].description = L"Chinese"; LocaleList[2].cultureName = L"zh-CN"; LocaleList[2].lcidHex = 0x0804; LocaleList[2].lcidDec = 2052;	
}

enum {
    BASE = 36,
    TMIN = 1,
    TMAX = 26,
    SKEW = 38,
    DAMP = 700,
    INIT_BIAS = 72,
    INIT_N = 128
};

typedef BOOL (CALLBACK *CALINFO_ENUMPROCEXEX)(
LPWSTR lpCalendarInfoString,
CALID Calendar,
LPWSTR lpReserved,
LPARAM lParam
);

typedef BOOL (CALLBACK* DATEFMT_ENUMPROCEXEX)(LPWSTR, CALID, LPARAM);

typedef BOOL (CALLBACK* TIMEFMT_ENUMPROCEX)(LPWSTR, LPARAM);

static inline INT adapt(INT delta, INT numpoints, BOOL firsttime)
{
    INT k;

    delta /= (firsttime ? DAMP : 2);
    delta += delta/numpoints;

    for(k=0; delta>((BASE-TMIN)*TMAX)/2; k+=BASE)
        delta /= BASE-TMIN;
    return k+((BASE-TMIN+1)*delta)/(delta+SKEW);
}

static inline unsigned short get_table_entry( const unsigned short *table, WCHAR ch )
{
    return table[table[table[ch >> 8] + ((ch >> 4) & 0x0f)] + (ch & 0xf)];
}

/******************************************************************************
 *           CompareStringOrdinal    (KERNEL32.@) implemented - need test
 */
INT WINAPI CompareStringOrdinal(LPCWSTR str1, int len1, LPCWSTR str2, int len2, BOOL ignore_case)
{
    int ret, len;

    if (!str1 || !str2)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }
    if (len1 < 0) len1 = strlenW(str1);
    if (len2 < 0) len2 = strlenW(str2);

    len = min(len1, len2);
    if (ignore_case)
    {
        ret = memicmpW(str1, str2, len);
    }
    else
    {
        ret = 0;
        for (; len > 0; len--)
            if ((ret = (*str1++ - *str2++))) break;
    }
    if (!ret) ret = len1 - len2;

    if (ret < 0) return CSTR_LESS_THAN;
    if (ret > 0) return CSTR_GREATER_THAN;
    return CSTR_EQUAL;
}

/*
 * @implemented - need test
 */
/******************************************************************************
 *           CompareStringEx    (KERNEL32.@)
 */
INT WINAPI CompareStringEx(LPCWSTR locale, DWORD flags, LPCWSTR str1, INT len1,
                           LPCWSTR str2, INT len2, LPNLSVERSIONINFO version, LPVOID reserved, LPARAM lParam)
{
    INT ret;

    if (version) FIXME("unexpected version parameter\n");
    if (reserved) FIXME("unexpected reserved value\n");
    if (lParam) FIXME("unexpected lParam\n");

    if (!str1 || !str2)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }

    if( flags & ~(NORM_IGNORECASE|NORM_IGNORENONSPACE|NORM_IGNORESYMBOLS|
        SORT_STRINGSORT|NORM_IGNOREKANATYPE|NORM_IGNOREWIDTH|LOCALE_USE_CP_ACP|0x10000000) )
    {
        SetLastError(ERROR_INVALID_FLAGS);
        return 0;
    }

    /* this style is related to diacritics in Arabic, Japanese, and Hebrew */
    if (flags & 0x10000000)
        WARN("Ignoring unknown flags 0x10000000\n");

    if (len1 < 0) len1 = strlenW(str1);
    if (len2 < 0) len2 = strlenW(str2);

    ret = wine_compare_string(flags, str1, len1, str2, len2);

    if (ret) /* need to translate result */
        return (ret < 0) ? CSTR_LESS_THAN : CSTR_GREATER_THAN;
    return CSTR_EQUAL;
}

/*
 * @implemented - need test
 */
LCID 
WINAPI 
LocaleNameToLCID(
  _In_  LPCWSTR lpName,
  _In_  DWORD dwFlags
)
{
	int i;	
	LCID result;
	
	InitTable();
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(strcmp(lpName,LocaleList[i].cultureName) == 0)
		{
			result = LocaleList[i].lcidHex;
			return result;
		}
	}
	return 0;
}

/*
 * @implemented - new - locale
 */
int 
WINAPI 
LCIDToLocaleName( 
	LCID lcid, 
	LPWSTR name, 
	INT count, 
	DWORD flags 
)
{
	int i;
	InitTable();
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lcid == LocaleList[i].lcidHex)
		{
			name = LocaleList[i].cultureName;			
			return wcslen(name);
		}
	} 
	return 0;
}

/*
 * @semi-implemented - need test and may be implemented by lcid table
 */
int 
WINAPI 
GetUserDefaultLocaleName(
  _Out_  LPWSTR lpLocaleName,
  _In_   int cchLocaleName
)
{
	LCID UILangID;
	NTSTATUS Status;
	int i;
	
	InitTable();
	
	Status = NtQueryDefaultUILanguage(&UILangID);
	if(NT_SUCCESS(Status))
	{
		for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
		{
			if(UILangID == LocaleList[i].lcidHex)
			{
				lpLocaleName = LocaleList[i].cultureName;
				return wcslen(lpLocaleName);
			}
		}
	}
	lpLocaleName = L"en-US";
	return 0;	
}

/*
 * @semi-implemented - need test and may be implemented by lcid table
 */
int WINAPI GetSystemDefaultLocaleName(
  _Out_  LPWSTR lpLocaleName,
  _In_   int cchLocaleName
)
{
	lpLocaleName = L"en-US";
	return 5;
}

/*
 * @implemented - need test
 */
int WINAPI LocaleNameFromLCID(LCID Locale, LPWSTR lpLCData, int cchData)
{
  return GetLocaleInfoW(Locale, 0x5Cu, lpLCData, cchData);
}

/*************************************************************************
 *           LCMapStringEx   (KERNEL32.@)
 *
 * Map characters in a locale sensitive string.
 *
 * PARAMS
 *  name     [I] Locale name for the conversion.
 *  flags    [I] Flags controlling the mapping (LCMAP_ constants from "winnls.h")
 *  src      [I] String to map
 *  srclen   [I] Length of src in chars, or -1 if src is NUL terminated
 *  dst      [O] Destination for mapped string
 *  dstlen   [I] Length of dst in characters
 *  version  [I] reserved, must be NULL
 *  reserved [I] reserved, must be NULL
 *  lparam   [I] reserved, must be 0
 *
 * RETURNS
 *  Success: The length of the mapped string in dst, including the NUL terminator.
 *  Failure: 0. Use GetLastError() to determine the cause.
 */
INT WINAPI LCMapStringEx(LPCWSTR name, DWORD flags, LPCWSTR src, INT srclen, LPWSTR dst, INT dstlen,
                         LPNLSVERSIONINFO version, LPVOID reserved, LPARAM lparam)
{
    LPWSTR dst_ptr;

    if (version) FIXME("unsupported version structure %p\n", version);
    if (reserved) FIXME("unsupported reserved pointer %p\n", reserved);
    if (lparam) FIXME("unsupported lparam %lx\n", lparam);

    if (!src || !srclen || dstlen < 0)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }

    /* mutually exclusive flags */
    if ((flags & (LCMAP_LOWERCASE | LCMAP_UPPERCASE)) == (LCMAP_LOWERCASE | LCMAP_UPPERCASE) ||
        (flags & (LCMAP_HIRAGANA | LCMAP_KATAKANA)) == (LCMAP_HIRAGANA | LCMAP_KATAKANA) ||
        (flags & (LCMAP_HALFWIDTH | LCMAP_FULLWIDTH)) == (LCMAP_HALFWIDTH | LCMAP_FULLWIDTH) ||
        (flags & (LCMAP_TRADITIONAL_CHINESE | LCMAP_SIMPLIFIED_CHINESE)) == (LCMAP_TRADITIONAL_CHINESE | LCMAP_SIMPLIFIED_CHINESE))
    {
        SetLastError(ERROR_INVALID_FLAGS);
        return 0;
    }

    if (!dstlen) dst = NULL;

    if (flags & LCMAP_SORTKEY)
    {
        INT ret;
        if (src == dst)
        {
            SetLastError(ERROR_INVALID_FLAGS);
            return 0;
        }

        if (srclen < 0) srclen = strlenW(src);

        TRACE("(%s,0x%08x,%s,%d,%p,%d)\n",
              debugstr_w(name), flags, debugstr_wn(src, srclen), srclen, dst, dstlen);

        ret = wine_get_sortkey(flags, src, srclen, (char *)dst, dstlen);
        if (ret == 0)
            SetLastError(ERROR_INSUFFICIENT_BUFFER);
        else
            ret++;
        return ret;
    }

    /* SORT_STRINGSORT must be used exclusively with LCMAP_SORTKEY */
    if (flags & SORT_STRINGSORT)
    {
        SetLastError(ERROR_INVALID_FLAGS);
        return 0;
    }

    if (srclen < 0) srclen = strlenW(src) + 1;

    TRACE("(%s,0x%08x,%s,%d,%p,%d)\n",
          debugstr_w(name), flags, debugstr_wn(src, srclen), srclen, dst, dstlen);

    if (!dst) /* return required string length */
    {
        INT len;

        for (len = 0; srclen; src++, srclen--)
        {
            WCHAR wch = *src;
            /* tests show that win2k just ignores NORM_IGNORENONSPACE,
             * and skips white space and punctuation characters for
             * NORM_IGNORESYMBOLS.
             */
            if ((flags & NORM_IGNORESYMBOLS) && (get_char_typeW(wch) & (C1_PUNCT | C1_SPACE)))
                continue;
            len++;
        }
        return len;
    }

    if (flags & LCMAP_UPPERCASE)
    {
        for (dst_ptr = dst; srclen && dstlen; src++, srclen--)
        {
            WCHAR wch = *src;
            if ((flags & NORM_IGNORESYMBOLS) && (get_char_typeW(wch) & (C1_PUNCT | C1_SPACE)))
                continue;
            *dst_ptr++ = toupperW(wch);
            dstlen--;
        }
    }
    else if (flags & LCMAP_LOWERCASE)
    {
        for (dst_ptr = dst; srclen && dstlen; src++, srclen--)
        {
            WCHAR wch = *src;
            if ((flags & NORM_IGNORESYMBOLS) && (get_char_typeW(wch) & (C1_PUNCT | C1_SPACE)))
                continue;
            *dst_ptr++ = tolowerW(wch);
            dstlen--;
        }
    }
    else
    {
        if (src == dst)
        {
            SetLastError(ERROR_INVALID_FLAGS);
            return 0;
        }
        for (dst_ptr = dst; srclen && dstlen; src++, srclen--)
        {
            WCHAR wch = *src;
            if ((flags & NORM_IGNORESYMBOLS) && (get_char_typeW(wch) & (C1_PUNCT | C1_SPACE)))
                continue;
            *dst_ptr++ = wch;
            dstlen--;
        }
    }

    if (srclen)
    {
        SetLastError(ERROR_INSUFFICIENT_BUFFER);
        return 0;
    }

    return dst_ptr - dst;
}


/***********************************************************************
 *		get_lcid_codepage
 *
 * Retrieve the ANSI codepage for a given locale.
 */
static inline UINT get_lcid_codepage( LCID lcid )
{
    UINT ret;
    if (!GetLocaleInfoW( lcid, LOCALE_IDEFAULTANSICODEPAGE|LOCALE_RETURN_NUMBER, (WCHAR *)&ret,
                         sizeof(ret)/sizeof(WCHAR) )) ret = 0;
    return ret;
}

/*
 * @implemented - need test
 */
int 
WINAPI 
GetLocaleInfoEx(_In_opt_   LPCWSTR lpLocaleName,
					_In_      LCTYPE LCType,
					_Out_opt_ LPWSTR lpLCData,
					_In_      int cchData)
{
	LCID lcid = 0x0409;
	int i;
	
	InitTable();
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lstrcmpW(lpLocaleName,LocaleList[i].cultureName) == 0)
		{
			lcid = LocaleList[i].lcidHex;
		}
	}
	return GetLocaleInfoW(lcid,LCType,lpLCData,cchData);
}

/*
 * @implemented - need test
 */
int 
WINAPI 
GetNumberFormatEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       DWORD dwFlags,
  _In_       LPCWSTR lpValue,
  _In_opt_   const NUMBERFMT *lpFormat,
  _Out_opt_  LPWSTR lpNumberStr,
  _In_       int cchNumber
)
{
	int i;
	InitTable();
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lstrcmpW(lpLocaleName,LocaleList[i].cultureName) == 0)
		{
			return GetNumberFormat(LocaleList[i].lcidHex, dwFlags, lpValue, lpFormat, lpNumberStr, cchNumber);
		}
	}
	return GetNumberFormat(0x0409, dwFlags, lpValue, lpFormat, lpNumberStr, cchNumber);
}

/* UNIMPLEMENTED SECTION **********************************************/

/***********************************************************************
  *              GetThreadUILanguage (KERNEL32.@)
  *
  * Get the current thread's language identifier.
  *
  * PARAMS
  *  None.
  *
  * RETURNS
  *  The current thread's language identifier.
*/
LANGID WINAPI GetThreadUILanguage( void )
{
     LANGID lang;
     NtQueryDefaultUILanguage( &lang );
     FIXME(": stub, returning default language.\n");
     return lang;
}

/*
 * @unimplemented - need reimplementation
 */
BOOL WINAPI SetThreadPreferredUILanguages(
  _In_       DWORD dwFlags,
  _In_opt_   PCZZWSTR pwszLanguagesBuffer,
  _Out_opt_  PULONG pulNumLanguages
)
{
	*pulNumLanguages = 1;
	if(dwFlags == MUI_LANGUAGE_ID)
	{
		pwszLanguagesBuffer = L"0409";
	}
	if(dwFlags == MUI_LANGUAGE_NAME)
	{
		pwszLanguagesBuffer = L"en-US";
	}
	return TRUE;
}

/*
 * @unimplemented - need reimplementation
 */
BOOL WINAPI GetThreadPreferredUILanguages(
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
)
{
	*pulNumLanguages = 1;
	if(dwFlags == MUI_LANGUAGE_ID)
	{
		pwszLanguagesBuffer = L"0409";
	}
	if(dwFlags == MUI_LANGUAGE_NAME)
	{
		pwszLanguagesBuffer = L"en-US";
	}
	return TRUE;
}

/*
 * @unimplemented - need reimplementation
 */
DWORD WINAPI NlsUpdateSystemLocale(LCID Locale, BOOL verification)
{
  DWORD resp; // esi@1
  LCID lcid; // edi@1
  UINT unknown; // [sp+8h] [bp-8h]@1
  UINT language; // [sp+Ch] [bp-4h]@1

  resp = 0;
  lcid = Locale;
  language = 0;
  unknown = 0;
  if ( Locale )
  {
    if ( verification != 1 )
      return 1004;
    GetLocaleInfoEx((LPCWSTR)Locale, 536875012, (LPWSTR)language, 2);
    GetLocaleInfoEx((LPCWSTR)lcid, 536870923, (LPWSTR)unknown, 2);
    if ( language == GetACP() )
    {
      if ( unknown == GetOEMCP() )
      {
        if ( LocaleNameToLCID((LPCWSTR)Locale, 0) < 0 )
          return RtlNtStatusToDosError(0);
        if ( Locale != 4096 && Locale != 3072 )
        {
          if ( NtSetDefaultLocale(0, Locale) >= 0 )
          {
            gSystemLocale = Locale;
            gpSysLocHashN = 0;
            return resp;
          }
          return RtlNtStatusToDosError(0);
        }
      }
    }
  }
  return 87;
}

DWORD WINAPI NlsPadInt64ToUnicodeStr(ULONGLONG Val, unsigned int a2, int a3, int a4, int a5)
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

int WINAPI ParseDuration(ULONGLONG longNumber, LPCWSTR lpFormat, LPWSTR lpDurationStr, int cchDuration)
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
  sevenTeen = sixteen >> 32;
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

/*
 * @unimplemented - need reimplementation
*/
NTSTATUS WINAPI NlsUpdateLocale(LCID locale, DWORD dwFlags) 
{
	return STATUS_NOT_IMPLEMENTED;
}

int WINAPI GetDurationFormatWorker(LCID Locale, int dwFlags, const SYSTEMTIME *lpDuration, ULONGLONG ullDuration, LPCWSTR lpFormat, LPWSTR lpDurationStr, int cchDuration)
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
int WINAPI GetDurationFormat(
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
  LCID lcid = 0; // eax@1
  int i;
  
  InitTable();
 
  for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lstrcmpW(lpLocaleName,LocaleList[i].cultureName) == 0)
		{
			lcid = LocaleList[i].lcidHex;
		}
	}
    if(lcid == 0)
	{
		lcid = 0x0409;
	}
	return GetDurationFormat(lcid, dwFlags, lpDuration, ullDuration, lpFormat, lpDurationStr, cchDuration);
}

/*
* @semi-implemented need test
*/
int 
WINAPI 
GetCalendarInfoEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       CALID Calendar,
  _In_opt_   LPCWSTR lpReserved,
  _In_       CALTYPE CalType,
  _Out_opt_  LPWSTR lpCalData,
  _In_       int cchData,
  _Out_opt_  LPDWORD lpValue
)
{
	int i;
	InitTable();	
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lstrcmpW(lpLocaleName,LocaleList[i].cultureName) == 0)
		{
			return GetCalendarInfoW(LocaleList[i].lcidHex,Calendar,CalType,lpCalData,cchData,lpValue);
		}
	}
	cchData = 0;
	return 1;
}

/*
* @semi-implemented need test
*/
int WINAPI GetDateFormatEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       DWORD dwFlags,
  _In_opt_   const SYSTEMTIME *lpDate,
  _In_opt_   LPCWSTR lpFormat,
  _Out_opt_  LPWSTR lpDateStr,
  _In_       int cchDate,
  _In_opt_   LPCWSTR lpCalendar
)
{
	int i;
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lstrcmpW(lpLocaleName,LocaleList[i].cultureName) == 0)
		{
			return GetDateFormatW(LocaleList[i].lcidHex,dwFlags,lpDate,lpFormat,lpDateStr,cchDate);
		}
	}
	cchDate = 0;
	return 1;
}

/*
* @semi-implemented need test
*/
BOOL WINAPI IsValidLocaleName(
  _In_  LPCWSTR lpLocaleName
)
{
	int count = 0;
	int i;
	BOOL result;
	
	InitTable();
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lstrcmpiW(lpLocaleName,LocaleList[i].cultureName) == 0)
		{
			count = 1;
		}
	}	
	if(count!=0)
		result = TRUE;
	else
		result = FALSE;
	return result;
}

/*
* @unimplemented need test
*/
BOOL WINAPI EnumSystemLocalesEx(
  _In_      LOCALE_ENUMPROCEX lpLocaleEnumProcEx,
  _In_      DWORD dwFlags,
  _In_      LPARAM lParam,
  _In_opt_  LPVOID lpReserved
)
{
	return TRUE;
}

/*
* @semi-implemented need test
*/
BOOL WINAPI GetSystemPreferredUILanguages(
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
)
{
    *pulNumLanguages = 1;
	if(dwFlags == MUI_LANGUAGE_ID)
	{
		pwszLanguagesBuffer = L"0409";
	}
	if(dwFlags == MUI_LANGUAGE_NAME)
	{
		pwszLanguagesBuffer = L"en-US";
	}
	return TRUE;
}

/*
* @semi-implemented need test
*/
int WINAPI GetTimeFormatEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       DWORD dwFlags,
  _In_opt_   const SYSTEMTIME *lpTime,
  _In_opt_   LPCWSTR lpFormat,
  _Out_opt_  LPWSTR lpTimeStr,
  _In_       int cchTime
)
{
	int i;
	
	InitTable();
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lstrcmpW(lpLocaleName,LocaleList[i].cultureName) == 0)
		{
			return GetTimeFormatW(LocaleList[i].lcidHex,dwFlags,lpTime,lpFormat,lpTimeStr,cchTime);
		}
	}
	return GetTimeFormatW(0x0409,dwFlags,lpTime,lpFormat,lpTimeStr,cchTime);
}

/*
* @semi-implemented need test
*/
int WINAPI GetCurrencyFormatEx(
     LPCWSTR lpLocaleName,
     DWORD dwFlags,
     LPCWSTR lpValue,
     const CURRENCYFMT *lpFormat,
     LPWSTR lpCurrencyStr,
     int cchCurrency
)
{
	int i;
	
	InitTable();
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lstrcmpW(lpLocaleName,LocaleList[i].cultureName) == 0)
		{
			return GetCurrencyFormatW(LocaleList[i].lcidHex,dwFlags,lpValue,lpFormat,lpCurrencyStr,cchCurrency);
		}
	}
	return GetCurrencyFormatW(0x0409,dwFlags,lpValue,lpFormat,lpCurrencyStr,cchCurrency);
}

int WINAPI FindNLSString(
  _In_ 		 LCID Locale,
  _In_       DWORD dwFindNLSStringFlags,
  _In_       LPCWSTR lpStringSource,
  _In_       int cchSource,
  _In_       LPCWSTR lpStringValue,
  _In_       int cchValue,
  _Out_opt_  LPINT pcchFound)
{
	SetLastError(50);
	return 0;
}

int WINAPI FindNLSStringEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       DWORD dwFindNLSStringFlags,
  _In_       LPCWSTR lpStringSource,
  _In_       int cchSource,
  _In_       LPCWSTR lpStringValue,
  _In_       int cchValue,
  _Out_opt_  LPINT pcchFound,
  _In_opt_   LPNLSVERSIONINFO lpVersionInformation,
  _In_opt_   LPVOID lpReserved,
  _In_opt_   LPARAM sortHandle
)
{
  return 0;
}	

/*
 * @unimplemented
*/
BOOL WINAPI GetNLSVersionEx(
  _In_      NLS_FUNCTION function,
  _In_opt_  LPCWSTR lpLocaleName,
  _Inout_   LPNLSVERSIONINFOEX lpVersionInformation
)
{
	SetLastError(ERROR_INVALID_FLAGS);
	return FALSE;
}

BOOL 
WINAPI 
GetFileMUIPath(
	DWORD dwFlags, 
	PCWSTR pcwszFilePath, 
	PWSTR pwszLanguage, 
	PULONG pcchLanguage, 
	PWSTR pwszFileMUIPath,
	PULONG pcchFileMUIPath, 
	PULONGLONG pululEnumerator
)
{
  /*DWORD error; // eax@3
  BOOL resp; // esi@1
  NTSTATUS status; // eax@1
  HINSTANCE hLib;*/
  typedef NTSTATUS (WINAPI * cfunc)(DWORD dwFlags, 
						   PCWSTR pcwszFilePath, 
						   PWSTR pwszLanguage, 
						   PULONG pcchLanguage, 
						   PWSTR pwszFileMUIPath,
						   PULONG pcchFileMUIPath, 
						   PULONGLONG pululEnumerator);
						
    DbgPrint("GetFileMUIPath: dwFlags = %d\n", dwFlags);
	DbgPrint("GetFileMUIPath: pcwszFilePath = %ws\n", pcwszFilePath);
	DbgPrint("GetFileMUIPath: pwszLanguage = %ws\n", pwszLanguage);
	DbgPrint("GetFileMUIPath: pcchLanguage = %d\n", pcchLanguage);
	DbgPrint("GetFileMUIPath: pwszFileMUIPath = %ws\n", pwszFileMUIPath);
	DbgPrint("GetFileMUIPath: pwszFileMUIPath = %d\n", pcchFileMUIPath);
  /*cfunc NtGetFileMUIInfo;					  
  TRACE("Passei por aqui");
  resp = TRUE;
  hLib=LoadLibrary("ntdllnew.dll");
  NtGetFileMUIInfo=(cfunc)GetProcAddress((HMODULE)hLib, "RtlGetFileMUIPath");	    
  status = NtGetFileMUIInfo(
             dwFlags,
             pcwszFilePath,
             pwszLanguage,
             pcchLanguage,
             pwszFileMUIPath,
             pcchFileMUIPath,
             pululEnumerator);
  TRACE("%d\n",dwFlags);
  TRACE("%s\n",pcwszFilePath);
  TRACE("%s\n",pwszLanguage);
  TRACE("%d\n",&pcchLanguage);
  TRACE("%s\n",pwszFileMUIPath);
  TRACE("%d\n",&pcchFileMUIPath);
  TRACE("%d\n",&pululEnumerator);
	   
  if ( status < 0 )
  {
    error = RtlNtStatusToDosError(status);
    SetLastError(error);
    resp = FALSE;
  }
  TRACE("%d\n",resp);
  return resp;*/
  return TRUE;
}

//NEED TEST!!! - USE FUNCTION FROM NTDLLNEW
BOOL WINAPI GetUILanguageInfo(DWORD dwFlags, PCZZWSTR pwmszLanguage, PZZWSTR pwszFallbackLanguages, PDWORD pcchFallbackLanguages, PDWORD pdwAttributes)
{
  NTSTATUS status; // eax@1
  DWORD flags; // eax@3
  BOOL result; // eax@5
  PDWORD other; // [sp+0h] [bp-4h]@1

  other = 0;
  status = RtlGetUILanguageInfo(dwFlags, pwmszLanguage, pwszFallbackLanguages, pcchFallbackLanguages, other);
  if ( status >= 0 )
  {
    if ( pdwAttributes )
      *pdwAttributes = (DWORD)other & 0x67;
    SetLastError(0);
    result = 1;
  }
  else
  {
    if ( status == 0xC00000BB )
      flags = 13;
    else
      flags = RtlNtStatusToDosError(status);
    SetLastError(flags);
    result = 0;
  }
  return result;
}

//Need Test
BOOL WINAPI EnumCalendarInfoExEx(
  _In_      CALINFO_ENUMPROCEXEX pCalInfoEnumProcExEx,
  _In_opt_  LPCWSTR lpLocaleName,
  _In_      CALID Calendar,
  _In_opt_  LPCWSTR lpReserved,
  _In_      CALTYPE CalType,
  _In_      LPARAM lParam
)
{
  LCID lcid;
  BOOL result;
  
  lcid = 0;
  lcid = LocaleNameToLCID(lpLocaleName, 0); 
  if(lcid == 0)
  {
		lcid = 0x0409;
  }
  if ( lpReserved )
  {
    SetLastError(0x57u);
    result = 0;
  }
  else
  {
	result = EnumCalendarInfoExW((CALINFO_ENUMPROCEX)pCalInfoEnumProcExEx, lcid, Calendar, CalType);
  }
  return result;
}

//Need Test
BOOL WINAPI EnumDateFormatsExEx(
  _In_      DATEFMT_ENUMPROCEXEX lpDateFmtEnumProcExEx,
  _In_opt_  LPCWSTR lpLocaleName,
  _In_      DWORD dwFlags,
  _In_      LPARAM lParam
)
{
    LCID lcid;
  
    lcid = 0;
	lcid = LocaleNameToLCID(lpLocaleName, 0); 
	if(lcid == 0)
	{
		lcid = 0x0409;
	}
	return EnumDateFormatsEx((DATEFMT_ENUMPROCEX)lpDateFmtEnumProcExEx, lcid, dwFlags);
}

BOOL WINAPI GetUserPreferredUILanguages(DWORD dwFlags, PULONG pulNumLanguages, PZZWSTR pwszLanguagesBuffer, PULONG pcchLanguagesBuffer)
{
  NTSTATUS status; // eax@2
  DWORD error; // eax@6

  if ( dwFlags & 0xFFFFFFF1 )
  {
    status = 0xC000000Du;
  }
  else
  {
    status = RtlGetUserPreferredUILanguages(dwFlags, 0, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
    if ( status >= 0 )
      return 1;
  }
  error = RtlNtStatusToDosError(status);
  SetLastError(error);
  return 0;
}

BOOL WINAPI EnumResourceNamesExW(HMODULE hModule, 
								 LPCWSTR lpType, 
								 ENUMRESNAMEPROCW lpEnumFunc, 
								 LONG_PTR lParam, 
								 DWORD dwFlags, 
								 LANGID LangId)
{
	return EnumResourceNamesW(hModule, lpType, lpEnumFunc, lParam);
}	

BOOL WINAPI EnumResourceNamesExA(HMODULE hModule, 
								 LPCSTR lpType, 
								 ENUMRESNAMEPROCA lpEnumFunc, 
								 LONG_PTR lParam, 
								 DWORD dwFlags, 
								 LANGID LangId)
{
	return EnumResourceNamesA(hModule, lpType, lpEnumFunc, lParam);
}	

BOOL WINAPI EnumResourceTypesExA(
  _In_opt_  HMODULE hModule,
  _In_      ENUMRESTYPEPROCA lpEnumFunc,
  _In_      LONG_PTR lParam,
  _In_      DWORD dwFlags,
  _In_      LANGID LangId
)	
{
	return EnumResourceTypesA(hModule, lpEnumFunc, lParam);
}

BOOL WINAPI EnumResourceTypesExW(
  _In_opt_  HMODULE hModule,
  _In_      ENUMRESTYPEPROCW lpEnumFunc,
  _In_      LONG_PTR lParam,
  _In_      DWORD dwFlags,
  _In_      LANGID LangId
)	
{
	return EnumResourceTypesW(hModule, lpEnumFunc, lParam);
}		

BOOL WINAPI EnumResourceLanguagesExA(
  _In_  HMODULE hModule,
  _In_  LPCTSTR lpType,
  _In_  LPCTSTR lpName,
  _In_  ENUMRESLANGPROCA lpEnumFunc,
  _In_  LONG_PTR lParam,
  _In_  DWORD dwFlags,
  _In_  LANGID LangId
)
{
	return EnumResourceLanguagesA(hModule, lpType, lpName, lpEnumFunc, lParam);
}	

BOOL WINAPI EnumResourceLanguagesExW(
  _In_  HMODULE hModule,
  _In_  LPCWSTR lpType,
  _In_  LPCWSTR lpName,
  _In_  ENUMRESLANGPROCW lpEnumFunc,
  _In_  LONG_PTR lParam,
  _In_  DWORD dwFlags,
  _In_  LANGID LangId
)
{
	return EnumResourceLanguagesW(hModule, lpType, lpName, lpEnumFunc, lParam);
}	

BOOL WINAPI EnumTimeFormatsEx(
  _In_      TIMEFMT_ENUMPROCEX lpTimeFmtEnumProcEx,
  _In_opt_  LPCWSTR lpLocaleName,
  _In_      DWORD dwFlags,
  _In_      LPARAM lParam
)
{
    LCID lcid;
  
    lcid = 0;
	lcid = LocaleNameToLCID(lpLocaleName, 0); 
	if(lcid == 0)
	{
		lcid = 0x0409;
	}
	return EnumTimeFormatsA((TIMEFMT_ENUMPROCA)lpTimeFmtEnumProcEx, lcid, dwFlags);
}	

ULONG WINAPI NlsWriteEtwEvent(REGHANDLE RegHandle, PCEVENT_DESCRIPTOR EventDescriptor, ULONG UserDataCount, PEVENT_DATA_DESCRIPTOR UserData)
{
  ULONG result; // eax@2

  if ( RegHandle )
  {
    result = EtwEventWrite(RegHandle, EventDescriptor, UserDataCount, UserData);
  }
  else
  {
    result = EtwEventRegister(0, 0, 0, &RegHandle);
    if ( !result )
    {
      EtwEventWrite(RegHandle, EventDescriptor, UserDataCount, UserData);
      result = EtwEventUnregister(RegHandle);
    }
  }
  return result;
}

ULONG WINAPI NlsEventDataDescCreate(LPSYSTEMTIME systemTime, LPWSTR Buffer, LPWSTR lpBuffer, ULONG ultimate)
{
  LPWSTR One; // ebx@5
  DWORD error; // edi@8
  LPWSTR Two; // eax@8
  LPSYSTEMTIME localSystemTime; // ecx@9
  LPWSTR otherBuffer; // ebx@10
  DWORD other; // eax@12
  LPWSTR localBuffer; // edi@17
  ULONG number; // ebx@18
  DWORD Three; // eax@19
  DWORD Four; // eax@22

  if ( !Buffer )
  {
    One = lpBuffer;
    if ( !lpBuffer )
    {
      Four = GetModuleFileNameW(0, lpBuffer, 1u);
      if ( !Four )
        Four = 2;
      return 2 * Four;
    }
    if ( GetModuleFileNameW(0, (LPWSTR)lpBuffer, ultimate >> 1) )
      goto LABEL_8;
    goto LABEL_27;
  }
  if ( Buffer == (LPWSTR)1 )
  {
    localBuffer = lpBuffer;
    if ( !lpBuffer )
      return 4;
    number = ultimate;
    if ( ultimate >= 4 )
    {
      Three = GetCurrentProcessId();
      localSystemTime = systemTime;
      localBuffer = (LPWSTR)Three;
      Two = localBuffer;
      systemTime->wHour = number;
      goto LABEL_28;
    }
    goto LABEL_20;
  }
  if ( Buffer == (LPWSTR)2 )
  {
    otherBuffer = lpBuffer;
    if ( !lpBuffer )
      return 4;
    other = GetLastError();
    error = 2 * FormatMessageW(0x1100u, 0, other, 0x400u, Buffer, 0, 0) + 2;
    Two = Buffer;
    if ( Buffer )
    {
      if ( ultimate >= 4 )
      {
        otherBuffer = Buffer;
        goto LABEL_9;
      }
    }
    else
    {
      if ( ultimate >= 4 )
      {
        otherBuffer = 0;
        goto LABEL_27;
      }
    }
LABEL_20:
    ultimate = 0;
    SetLastError(0x7Au);
    return ultimate;
  }
  if ( Buffer == (LPWSTR)3 )
  {
    One = lpBuffer;
    if ( !lpBuffer )
      return 2 * GetSystemWindowsDirectoryW(lpBuffer, 1u) + 2;
    if ( GetSystemWindowsDirectoryW((LPWSTR)lpBuffer, ultimate >> 1) )
    {
LABEL_8:
      Two = One;
LABEL_9:
      localSystemTime = systemTime;
      systemTime->wHour = error;
LABEL_28:
      localSystemTime->wYear = (WORD)Two;
      localSystemTime->wSecond = 0;
      return ultimate;
    }
LABEL_27:
    localSystemTime = systemTime;
    systemTime->wHour = 2;
    goto LABEL_28;
  }
  return ultimate;
}

/*
/*unimplemented
*/
DWORD WINAPI NlsCheckPolicy(LPWSTR string, ULONG number)
{
  UNIMPLEMENTED;
  return 87;
}

BOOL WINAPI NotifyUILanguageChange(
  _In_       DWORD dwFlags,
  _In_opt_   PCWSTR pcwstrNewLanguage,
  _In_opt_   PCWSTR pcwstrPreviousLanguage,
  _In_       DWORD dwReserved,
  _Out_opt_  PDWORD pdwStatusRtrn
)
{
	return TRUE;
}

BOOL WINAPI _FillBuffer(PFILEMUIINFO a1, unsigned int a2, const void *Src, size_t Size, int a5, PCWSTR a6)
{
  if ( a5 + Size > a2 )
  {
    a6 = (PCWSTR)a5 + Size - a2;
    return 0;
  }
  if ( !a1 )
  {
    a6 = 0;
    return 0;
  }
  memcpy((char *)a1 + a5, Src, Size);
  a6 = (PCWSTR)a5 + Size;
  return 1;
}

BOOL WINAPI GetFileMUIInfo(DWORD dwFlags, PCWSTR pcwszFilePath, PFILEMUIINFO pFileMUIInfo, DWORD *pcbFileMUIInfo)
{
  DWORD *fileInfo; // edi@1
  DWORD filePath; // esi@3
  BOOLEAN verfication; // zf@3
  PFILEMUIINFO localMuiFileInfo; // ebx@3
  NTSTATUS status; // eax@18
  PFILEMUIINFO otherMuiFileInfo; // eax@21
  DWORD LanguageNameOffset; // ecx@34
  DWORD pServiceChecksum; // edx@34
  const void *one; // ecx@35
  DWORD receive; // edi@46
  const void *otherCompose; // ecx@52
  DWORD compose; // eax@52
  DWORD other; // edi@53
  DWORD fileType; // edx@59
  DWORD fileTypeOffset; // edi@61
  DWORD TypeMUISize; // edi@65
  DWORD TypeNameMUISize; // edi@74
  PCWSTR muiString; // [sp+Ch] [bp-30h]@18
  int uneeded; // [sp+10h] [bp-2Ch]@18
  int unneeded; // [sp+14h] [bp-28h]@18
  ULONG DataLength; // [sp+18h] [bp-24h]@20
  DWORD otherFilePath; // [sp+1Ch] [bp-20h]@8
  PVOID hLibModule; // [sp+20h] [bp-1Ch]@16
  PIMAGE_RESOURCE_DATA_ENTRY DataEntry; // [sp+24h] [bp-18h]@1
  DWORD size; // [sp+28h] [bp-14h]@3
  PVOID Data; // [sp+2Ch] [bp-10h]@1
  DWORD dwErrCode; // [sp+30h] [bp-Ch]@1
  DWORD otherPath; // [sp+34h] [bp-8h]@1
  BOOL resp; // [sp+38h] [bp-4h]@1

  fileInfo = 0;
  DataEntry = 0;
  resp = 1;
  dwErrCode = 0;
  otherPath = 72;
  Data = 0;
  SetLastError(0);
  if ( !pcwszFilePath
    || !pcbFileMUIInfo
    || (filePath = *pcbFileMUIInfo,
        verfication = *pcbFileMUIInfo == 0,
        localMuiFileInfo = pFileMUIInfo,
        size = *pcbFileMUIInfo,
        !verfication)
    && !pFileMUIInfo )
    goto LABEL_11;
  if ( filePath < 0x50 && pFileMUIInfo )
  {
    resp = 0;
    dwErrCode = 122;
    localMuiFileInfo = 0;
  }
  otherFilePath = filePath;
  *pcbFileMUIInfo = 0;
  if ( localMuiFileInfo )
  {
    if ( localMuiFileInfo->dwSize != filePath
      || (memset(&localMuiFileInfo->dwFileType, 0, (filePath - 4)), localMuiFileInfo->dwVersion < 1) )
    {
LABEL_11:
      SetLastError(0x57u);
      return 0;
    }
  }
  else
  {
    filePath = 72;
    otherFilePath = 72;
  }
  if ( !dwFlags )
    dwFlags = 3;
  hLibModule = LoadLibraryExW(pcwszFilePath, 0, 0x22u);
  if ( hLibModule )
  {
    muiString = L"MUI";
    uneeded = 1;
    unneeded = 0;
    status = LdrFindResourceEx_U(16, hLibModule, (ULONG_PTR *)muiString, 3, &DataEntry);
    if ( status < 0 || !DataEntry )
    {
      if ( dwFlags & 1 )
      {
        if ( localMuiFileInfo )
          localMuiFileInfo->dwFileType = 1;
        else
          *pcbFileMUIInfo = 80;
        goto LABEL_88;
      }
      dwErrCode = RtlNtStatusToDosError(status);
LABEL_87:
      resp = *fileInfo;
LABEL_88:
      FreeLibrary(hLibModule);
      if ( dwErrCode != *fileInfo )
        SetLastError(dwErrCode);
      return resp;
    }
    if ( LdrAccessResource(hLibModule, DataEntry, &Data, &DataLength) < 0 || (otherMuiFileInfo = Data) == 0 )
    {
      dwErrCode = 0;
      goto LABEL_87;
    }
    if ( Data != (PVOID)0xFECDFECD )
    {
      dwErrCode = 15102;
      goto LABEL_87;
    }
    verfication = (*((BYTE *)Data + 16) & 1) == 0;
    pFileMUIInfo->dwFileType = 4;
    if ( !verfication )
      pFileMUIInfo->dwFileType = 2;
    if ( localMuiFileInfo )
    {
      if ( dwFlags & 1 )
        localMuiFileInfo->dwFileType = pFileMUIInfo->dwFileType;
      if ( dwFlags & 2 )
      {
        *&localMuiFileInfo->pChecksum[0] = otherMuiFileInfo->dwLanguageNameOffset;
        *&localMuiFileInfo->pChecksum[4] = otherMuiFileInfo->dwTypeIDMainSize;
        *&localMuiFileInfo->pChecksum[8] = otherMuiFileInfo->dwTypeIDMainOffset;
        *&localMuiFileInfo->pChecksum[12] = otherMuiFileInfo->dwTypeNameMainOffset;
        *&localMuiFileInfo->pServiceChecksum[0] = *&otherMuiFileInfo->pServiceChecksum[0];
        *&localMuiFileInfo->pServiceChecksum[4] = *&otherMuiFileInfo->pServiceChecksum[4];
        *&localMuiFileInfo->pServiceChecksum[8] = *&otherMuiFileInfo->pServiceChecksum[8];
        *&localMuiFileInfo->pServiceChecksum[12] = *&otherMuiFileInfo->pServiceChecksum[12];
        filePath = otherFilePath;
        fileInfo = 0;
      }
    }
    if ( dwFlags & 4 )
    {
      if ( pFileMUIInfo->dwFileType == 2 )
      {
        LanguageNameOffset = otherMuiFileInfo[1].dwLanguageNameOffset;
        pServiceChecksum = otherMuiFileInfo[1].dwTypeIDMainSize;
      }
      else
      {
        if ( pFileMUIInfo->dwFileType != 4 )
          goto LABEL_87;
        LanguageNameOffset = *&otherMuiFileInfo[1].pServiceChecksum[8];
        pServiceChecksum = *&otherMuiFileInfo[1].pServiceChecksum[12];
      }
      one = otherMuiFileInfo + LanguageNameOffset;
      if ( pServiceChecksum > (DWORD)fileInfo )
      {
        if ( _FillBuffer(localMuiFileInfo, filePath, one, pServiceChecksum, 72, pcwszFilePath) )
        {
          otherPath = (DWORD)pcwszFilePath;
          localMuiFileInfo->dwLanguageNameOffset = 72;
        }
        else
        {
          filePath = (DWORD)(filePath + pcwszFilePath);
          otherPath = filePath;
          if ( localMuiFileInfo->dwFileType != *fileInfo )
          {
            dwErrCode = 122;
            resp = *fileInfo;
          }
        }
        otherMuiFileInfo = Data;
      }
    }
    if ( !(dwFlags & 8) )
      goto LABEL_78;
    if ( pFileMUIInfo->dwFileType != 2 )
    {
      if ( pFileMUIInfo->dwFileType != 4 )
      {
        fileInfo = 0;
        goto LABEL_87;
      }
      receive = *&otherMuiFileInfo[1].pChecksum[4];
      if ( !receive )
        goto LABEL_52;
      if ( filePath > size )
        goto LABEL_93;
      if ( _FillBuffer(
             localMuiFileInfo,
             filePath,
             otherMuiFileInfo + *&otherMuiFileInfo[1].pChecksum[0],
             receive,
             otherPath,
             pcwszFilePath) )
      {
        localMuiFileInfo->dwTypeIDMUIOffset = otherPath;
        otherPath = (DWORD)pcwszFilePath;
        otherMuiFileInfo = Data;
        localMuiFileInfo->dwTypeIDMUISize = receive >> 2;
        goto LABEL_52;
      }
      filePath = (DWORD)(filePath + pcwszFilePath);
      otherMuiFileInfo = Data;
      otherPath = filePath;
      if ( localMuiFileInfo )
      {
LABEL_93:
        resp = 0;
        dwErrCode = 122;
      }
LABEL_52:
      otherCompose = otherMuiFileInfo + otherMuiFileInfo[1].dwVersion;
      compose = otherMuiFileInfo[1].dwFileType;
LABEL_72:
      if ( compose )
      {
        if ( filePath > size )
          goto LABEL_94;
        TypeNameMUISize = otherPath;
        if ( _FillBuffer(localMuiFileInfo, filePath, otherCompose, compose, otherPath, pcwszFilePath) )
        {
          localMuiFileInfo->dwTypeNameMUIOffset = TypeNameMUISize;
          goto LABEL_78;
        }
        filePath = (DWORD)(filePath + pcwszFilePath);
        if ( localMuiFileInfo )
        {
LABEL_94:
          resp = 0;
          dwErrCode = 122;
        }
      }
LABEL_78:
      if ( filePath < 0x50 )
        filePath = 80;
      *pcbFileMUIInfo = filePath;
      fileInfo = 0;
      goto LABEL_88;
    }
    other = *&otherMuiFileInfo[1].pChecksum[4];
    if ( !other )
      goto LABEL_59;
    if ( filePath > size )
      goto LABEL_95;
    if ( _FillBuffer(
           localMuiFileInfo,
           filePath,
           otherMuiFileInfo + *&otherMuiFileInfo[1].pChecksum[0],
           other,
           otherPath,
           pcwszFilePath) )
    {
      localMuiFileInfo->dwTypeIDMainOffset = otherPath;
      otherPath = (DWORD)pcwszFilePath;
      otherMuiFileInfo = Data;
      localMuiFileInfo->dwTypeIDMainSize = other >> 2;
      goto LABEL_59;
    }
    filePath = (DWORD)(filePath + pcwszFilePath);
    otherMuiFileInfo = Data;
    otherPath = filePath;
    if ( localMuiFileInfo )
    {
LABEL_95:
      resp = 0;
      dwErrCode = 122;
    }
LABEL_59:
    fileType = otherMuiFileInfo[1].dwFileType;
    if ( fileType )
    {
      if ( filePath > size )
        goto LABEL_96;
      fileTypeOffset = otherPath;
      if ( _FillBuffer(
             localMuiFileInfo,
             filePath,
             otherMuiFileInfo + otherMuiFileInfo[1].dwVersion,
             fileType,
             otherPath,
             pcwszFilePath) )
      {
        otherPath = (DWORD)pcwszFilePath;
        otherMuiFileInfo = Data;
        localMuiFileInfo->dwTypeNameMainOffset = fileTypeOffset;
        goto LABEL_65;
      }
      filePath = (DWORD)(filePath + pcwszFilePath);
      otherMuiFileInfo = Data;
      otherPath = filePath;
      if ( localMuiFileInfo )
      {
LABEL_96:
        resp = 0;
        dwErrCode = 122;
      }
    }
LABEL_65:
    TypeMUISize = *&otherMuiFileInfo[1].pServiceChecksum[4];
    if ( TypeMUISize )
    {
      if ( filePath > size )
        goto LABEL_97;
      if ( _FillBuffer(
             localMuiFileInfo,
             filePath,
             otherMuiFileInfo + *&otherMuiFileInfo[1].pServiceChecksum[0],
             TypeMUISize,
             otherPath,
             pcwszFilePath) )
      {
        localMuiFileInfo->dwTypeIDMUIOffset = otherPath;
        otherPath = (DWORD)pcwszFilePath;
        otherMuiFileInfo = Data;
        localMuiFileInfo->dwTypeIDMUISize = TypeMUISize >> 2;
        goto LABEL_71;
      }
      filePath = (DWORD)(filePath + pcwszFilePath);
      otherMuiFileInfo = Data;
      otherPath = filePath;
      if ( localMuiFileInfo )
      {
LABEL_97:
        resp = 0;
        dwErrCode = 122;
      }
    }
LABEL_71:
    otherCompose = otherMuiFileInfo + *&otherMuiFileInfo[1].pChecksum[8];
    compose = *&otherMuiFileInfo[1].pChecksum[12];
    goto LABEL_72;
  }
  return 0;
}

int WINAPI FindStringOrdinal(
  _In_  DWORD dwFindStringOrdinalFlags,
  _In_  LPCWSTR lpStringSource,
  _In_  int cchSource,
  _In_  LPCWSTR lpStringValue,
  _In_  int cchValue,
  _In_  BOOL bIgnoreCase
)
{
	UNIMPLEMENTED;
	return 0;
}


BOOL WINAPI GetProcessPreferredUILanguages(
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
)
{
	UNIMPLEMENTED;
	return TRUE;	
}

int  WINAPI ResolveLocaleName(
  _In_opt_   LPCWSTR lpNameToResolve,
  _Out_opt_  LPWSTR lpLocaleName,
  _In_       int cchLocaleName
)
{
	UNIMPLEMENTED;
	return 0;	
}


BOOL WINAPI SetProcessPreferredUILanguages(
  _In_       DWORD dwFlags,
  _In_opt_   PCZZWSTR pwszLanguagesBuffer,
  _Out_opt_  PULONG pulNumLanguages
)
{
	UNIMPLEMENTED;
	return TRUE;	
}


DWORD WINAPI IsValidNLSVersion(
  _In_      NLS_FUNCTION function,
  _In_opt_  LPCWSTR lpLocaleName,
  _In_      LPNLSVERSIONINFOEX lpVersionInformation
)
{
	UNIMPLEMENTED;
	return 0;	
}

/* Internal call cause error on Server 2003 SP1 and XP SP2
int WINAPI GetStringScripts(
        __in                         DWORD   dwFlags,        // optional behavior flags
        __in                         LPCWSTR lpString,       // Unicode character input string
        __in                         int     cchString,      // size of input string
        __out_ecount_opt(cchScripts) LPWSTR  lpScripts,      // Script list output string
        __in                         int     cchScripts      // size of output string
)
{
	return DownlevelGetStringScripts(dwFlags, lpString, cchString, lpScripts, cchScripts);
}

BOOL WINAPI VerifyScripts(DWORD dwFlags, LPCWSTR lpLocaleScripts, int cchLocaleScripts, LPCWSTR lpTestScripts, int cchTestScripts)
{
  return DownlevelVerifyScripts(dwFlags, lpLocaleScripts, cchLocaleScripts, lpTestScripts, cchTestScripts);
}*/