/*++

Copyright (c) 2017 Shorthorn Project.

Module Name:

    locale.c

Abstract:

    This file contains functions that return information about a
    language group, a UI language, a locale, or a calendar.

Revision History:

    22-03-2017

--*/

#include "main.h"
#include "locale.h"

#define LOCALE_WINDOWS              0x01
#define LOCALE_NEUTRALDATA          0x10
#define LOCALE_SPECIFICDATA         0x20
#define MUI_LANGUAGE_ID             0x04
#define MUI_LANGUAGE_NAME           0x08
#define MUI_MACHINE_LANGUAGE_SETTINGS       0x400
#define MUI_MERGE_USER_FALLBACK 0x20
#define MUI_MERGE_SYSTEM_FALLBACK 0x10
#define MUI_THREAD_LANGUAGES                0x40
#define MUI_UI_FALLBACK                     MUI_MERGE_SYSTEM_FALLBACK | MUI_MERGE_USER_FALLBACK
#define MAX_STRING_LEN 256

extern PNLS_USER_INFO   pNlsUserInfo;       // ptr to the user info cache

WINE_DEFAULT_DEBUG_CHANNEL(locale); 

typedef BOOL (WINAPI *pGetNLSVersion)(
    NLS_FUNCTION, 
	LCID,
	LPNLSVERSIONINFO);

struct locale_name
{
    WCHAR  win_name[128];   /* Windows name ("en-US") */
    WCHAR  lang[128];       /* language ("en") (note: buffer contains the other strings too) */
    WCHAR *country;         /* country ("US") */
    WCHAR *charset;         /* charset ("UTF-8") for Unix format only */
    WCHAR *script;          /* script ("Latn") for Windows format only */
    WCHAR *modifier;        /* modifier or sort order */
    LCID   lcid;            /* corresponding LCID */
    int    matches;         /* number of elements matching LCID (0..4) */
    UINT   codepage;        /* codepage corresponding to charset */
};

struct enum_locale_ex_data
{
    LOCALE_ENUMPROCEX proc;
    DWORD             flags;
    LPARAM            lparam;
};


static WORD get_default_sublang(LCID lang)
{
    switch (PRIMARYLANGID(lang))
    {
    case LANG_SPANISH:
        return SUBLANG_SPANISH_MODERN;
    case LANG_CHINESE:
        return SUBLANG_CHINESE_SIMPLIFIED;
    default:
        return SUBLANG_DEFAULT;
    }
}

/***********************************************************************
 *           find_locale_id_callback
 */
static BOOL CALLBACK find_locale_id_callback( HMODULE hModule, LPCWSTR type,
                                              LPCWSTR name, WORD LangID, LPARAM lParam )
{
    struct locale_name *data = (struct locale_name *)lParam;
    WCHAR buffer[128];
    int matches = 0;
    LCID lcid = MAKELCID( LangID, SORT_DEFAULT );  /* FIXME: handle sort order */

    if (PRIMARYLANGID(LangID) == LANG_NEUTRAL) return TRUE; /* continue search */

    /* first check exact name */
    if (data->win_name[0] &&
        GetLocaleInfoW( lcid, LOCALE_SNAME | LOCALE_NOUSEROVERRIDE,
                        buffer, sizeof(buffer)/sizeof(WCHAR) ))
    {
        if (!strcmpiW( data->win_name, buffer ))
        {
            matches = 4;  /* everything matches */
            goto done;
        }
    }

    if (!GetLocaleInfoW( lcid, LOCALE_SISO639LANGNAME | LOCALE_NOUSEROVERRIDE,
                         buffer, sizeof(buffer)/sizeof(WCHAR) ))
        return TRUE;
    if (strcmpiW( buffer, data->lang )) return TRUE;
    matches++;  /* language name matched */

    if (data->country)
    {
        if (GetLocaleInfoW( lcid, LOCALE_SISO3166CTRYNAME|LOCALE_NOUSEROVERRIDE,
                            buffer, sizeof(buffer)/sizeof(WCHAR) ))
        {
            if (strcmpiW( buffer, data->country )) goto done;
            matches++;  /* country name matched */
        }
    }
    else  /* match default language */
    {
        if (SUBLANGID(LangID) == get_default_sublang( LangID )) matches++;
    }

    if (data->codepage)
    {
        UINT unix_cp;
        if (GetLocaleInfoW( lcid, LOCALE_IDEFAULTUNIXCODEPAGE | LOCALE_RETURN_NUMBER,
                            (LPWSTR)&unix_cp, sizeof(unix_cp)/sizeof(WCHAR) ))
        {
            if (unix_cp == data->codepage) matches++;
        }
    }

    /* FIXME: check sort order */

done:
    if (matches > data->matches)
    {
        data->lcid = lcid;
        data->matches = matches;
    }
    return (data->matches < 4);  /* no need to continue for perfect match */
}

/***********************************************************************
 *		parse_locale_name
 *
 * Parse a locale name into a struct locale_name, handling both Windows and Unix formats.
 * Unix format is: lang[_country][.charset][@modifier]
 * Windows format is: lang[-script][-country][_modifier]
 */
static void parse_locale_name( const WCHAR *str, struct locale_name *name )
{
    static const WCHAR sepW[] = {'-','_','.','@',0};
    static const WCHAR winsepW[] = {'-','_',0};
    static const WCHAR posixW[] = {'P','O','S','I','X',0};
    static const WCHAR cW[] = {'C',0};
    static const WCHAR latinW[] = {'l','a','t','i','n',0};
    static const WCHAR latnW[] = {'-','L','a','t','n',0};
    WCHAR *p;

    name->country = name->charset = name->script = name->modifier = NULL;
    name->lcid = MAKELCID( MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT), SORT_DEFAULT );
    name->matches = 0;
    name->codepage = 0;
    name->win_name[0] = 0;
    lstrcpynW( name->lang, str, sizeof(name->lang)/sizeof(WCHAR) );

    if (!*name->lang)
    {
        name->lcid = LOCALE_INVARIANT;
        name->matches = 4;
        return;
    }

    if (!(p = strpbrkW( name->lang, sepW )))
    {
        if (!strcmpW( name->lang, posixW ) || !strcmpW( name->lang, cW ))
        {
            name->matches = 4;  /* perfect match for default English lcid */
            return;
        }
        strcpyW( name->win_name, name->lang );
    }
    else if (*p == '-')  /* Windows format */
    {
        strcpyW( name->win_name, name->lang );
        *p++ = 0;
        name->country = p;
        if (!(p = strpbrkW( p, winsepW ))) goto done;
        if (*p == '-')
        {
            *p++ = 0;
            name->script = name->country;
            name->country = p;
            if (!(p = strpbrkW( p, winsepW ))) goto done;
        }
        *p++ = 0;
        name->modifier = p;
    }
done:
    EnumResourceLanguagesW( NULL, (LPCWSTR)RT_STRING, (LPCWSTR)LOCALE_ILANGUAGE,
                            find_locale_id_callback, (LPARAM)name );
}

/***********************************************************************
 *           LocaleNameToLCID  (KERNEL32.@)
 */
LCID 
WINAPI 
LocaleNameToLCID( 
	LPCWSTR name, 
	DWORD flags 
)
{
    int i;

    if (name == LOCALE_NAME_USER_DEFAULT)
        return GetUserDefaultLCID();

	for(i=0;i<LOCALE_TABLE_SIZE;i++){
		if(wcscmp(name, locale_table[i].localeName)==0){
			return locale_table[i].lcid;
		}
	}
	
    return GetSystemDefaultLCID();
}

/***********************************************************************
 *           LCIDToLocaleName  (KERNEL32.@)
 */
INT 
WINAPI 
LCIDToLocaleName( 
	LCID lcid, 
	LPWSTR lpName, 
	INT count, 
	DWORD flags 
)
{
	int i;
	int length = 0;
	for(i=0;i<LOCALE_TABLE_SIZE;i++){
		if(lcid == locale_table[i].lcid){
			length = (wcslen(locale_table[i].localeName)+1);
			if(lpName){
				memcpy(lpName, locale_table[i].localeName, sizeof(WCHAR)*(length));
				lpName[length-1] = 0;
			}			
			return length;
		}
	}
	return length;
}

/***********************************************************************
 *		GetSystemDefaultLocaleName (KERNEL32.@)
 */
INT 
WINAPI 
GetSystemDefaultLocaleName(
	LPWSTR localename, 
	INT len
)
{
    LCID lcid = GetSystemDefaultLCID();
    return LCIDToLocaleName(lcid, localename, len, 0);
}

/******************************************************************************
 *           IsValidLocaleName   (KERNEL32.@)
 */
BOOL 
WINAPI 
IsValidLocaleName( 
	LPCWSTR locale 
)
{
	int i;
    if (!locale)
        return FALSE;

	for(i=0;i<LOCALE_TABLE_SIZE;i++){
		if(wcscmp(locale, locale_table[i].localeName)==0){
			return TRUE;
		}
	}	

    return FALSE;
}

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
LANGID 
WINAPI 
GetThreadUILanguage( void )
{
     LANGID lang;
     //NtQueryDefaultUILanguage( &lang );
     //DbgPrint("GetThreadUILanguage is UNIMPLEMENTED, returning default language.\n");
	 //Windows XP and Server 2003 doesn't use really LANGIID passed how paremeter on SetThreadUILanguage, so, we 
	 //can use to get Thread UI Language;	 
     return SetThreadUILanguage(0);
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

	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);		
	return FALSE;
}

/*
 * @implemented - need test
 */
/******************************************************************************
 *           CompareStringEx    (KERNEL32.@)
 */
INT 
WINAPI 
CompareStringEx(
	LPCWSTR locale, 
	DWORD flags, 
	LPCWSTR str1, 
	INT len1,
    LPCWSTR str2, 
	INT len2, 
	LPNLSVERSIONINFO version, 
	LPVOID reserved, 
	LPARAM lParam
)
{
    INT ret;

    if (version) DbgPrint("unexpected version parameter\n");
    if (reserved) DbgPrint("unexpected reserved value\n");
    if (lParam) DbgPrint("unexpected lParam\n");

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
        DbgPrint("Ignoring unknown flags 0x10000000\n");

    if (len1 < 0) len1 = strlenW(str1);
    if (len2 < 0) len2 = strlenW(str2);

    ret = wine_compare_string(flags, str1, len1, str2, len2);

    if (ret) /* need to translate result */
        return (ret < 0) ? CSTR_LESS_THAN : CSTR_GREATER_THAN;
    return CSTR_EQUAL;
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
INT 
WINAPI 
LCMapStringEx(
	LPCWSTR name, 
	DWORD flags, 
	LPCWSTR src, 
	INT srclen, 
	LPWSTR dst, 
	INT dstlen,
    LPNLSVERSIONINFO version, 
	LPVOID reserved, 
	LPARAM lparam
)
{
    LPWSTR dst_ptr;
	
	DbgPrint("LCMapStringEx called\n");		

    if (version) DbgPrint("unsupported version structure %p\n", version);
    if (reserved) DbgPrint("unsupported reserved pointer %p\n", reserved);
    if (lparam) DbgPrint("unsupported lparam %lx\n", lparam);

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

        DbgPrint("(%s,0x%08x,%s,%d,%p,%d)\n",
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

    DbgPrint("(%s,0x%08x,%s,%d,%p,%d)\n",
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

INT 
WINAPI 
GetUserDefaultLocaleName(
	LPWSTR localename, 
	int buffersize
)
{
     LCID userlcid;
   
	userlcid = GetUserDefaultLCID();
    return LCIDToLocaleName(userlcid, localename, buffersize, 0);
}

static BOOL CALLBACK enum_locale_ex_proc( HMODULE module, LPCWSTR type,
                                          LPCWSTR name, WORD lang, LONG_PTR lparam )
{
    struct enum_locale_ex_data *data = (struct enum_locale_ex_data *)lparam;
    WCHAR buffer[256];
    DWORD neutral;
    unsigned int flags;

    GetLocaleInfoW( MAKELCID( lang, SORT_DEFAULT ), LOCALE_SNAME | LOCALE_NOUSEROVERRIDE,
                    buffer, sizeof(buffer) / sizeof(WCHAR) );
    if (!GetLocaleInfoW( MAKELCID( lang, SORT_DEFAULT ),
                         LOCALE_INEUTRAL | LOCALE_NOUSEROVERRIDE | LOCALE_RETURN_NUMBER,
                         (LPWSTR)&neutral, sizeof(neutral) / sizeof(WCHAR) ))
        neutral = 0;
    flags = LOCALE_WINDOWS;
    flags |= neutral ? LOCALE_NEUTRALDATA : LOCALE_SPECIFICDATA;
    if (data->flags && !(data->flags & flags)) return TRUE;
    return data->proc( buffer, flags, data->lparam );
}

/******************************************************************************
 *           EnumSystemLocalesEx  (KERNEL32.@)
 */
BOOL 
WINAPI 
EnumSystemLocalesEx( 
	LOCALE_ENUMPROCEX proc, 
	DWORD flags, 
	LPARAM lparam, 
	LPVOID reserved )
{
    struct enum_locale_ex_data data;

    if (reserved)
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }
    data.proc   = proc;
    data.flags  = flags;
    data.lparam = lparam;
    EnumResourceLanguagesW( kernel32_handle, (LPCWSTR)RT_STRING,
                            (LPCWSTR)MAKEINTRESOURCE((LOCALE_SNAME >> 4) + 1),
                            enum_locale_ex_proc, (LONG_PTR)&data );
    return TRUE;
}

//Wrapper to special cases of GetLocaleInfoW
int 
WINAPI 
GetpLocaleInfoW(
    LCID Locale,
    LCTYPE LCType,
    LPWSTR lpLCData,
    int cchData)
{
	switch(LCType){
		case ( LOCALE_SNAME ) :
			return LCIDToLocaleName(Locale, lpLCData, LOCALE_NAME_MAX_LENGTH, 0);
		default:
			return GetLocaleInfoW(Locale, LCType, lpLCData, cchData);
	}
}

int 
WINAPI
GeptLocaleInfoA(
  _In_      LCID   Locale,
  _In_      LCTYPE LCType,
  _Out_opt_ LPTSTR lpLCData,
  _In_      int    cchData
)
{
    WCHAR pDTmp[MAX_STRING_LEN];  // tmp Unicode buffer (destination)
    LPWSTR pBuf;                  // ptr to destination buffer	
	int numberCharacters;
	
	pBuf = pDTmp;
	switch(LCType){
		case ( LOCALE_SNAME ) :
			numberCharacters = LCIDToLocaleName(Locale, pBuf, LOCALE_NAME_MAX_LENGTH, 0);
			numberCharacters *= sizeof(WCHAR);
            if (lpLCData)
            {
                if (numberCharacters <= cchData) 
				{
					memcpy( lpLCData, pBuf, numberCharacters );
				}					
			}
			return numberCharacters;
		default:
			return GetLocaleInfoA(Locale, LCType, lpLCData, cchData);
	}	
}

/******************************************************************************
 *           GetLocaleInfoEx (KERNEL32.@)
 */
INT 
WINAPI 
GetLocaleInfoEx(
	LPCWSTR locale, 
	LCTYPE info, 
	LPWSTR buffer, 
	INT len
)
{
    LCID lcid = LocaleNameToLCID(locale, 0);

    if (!lcid) return 0;

    /* special handling for neutral locale names */
    if (info == LOCALE_SNAME && locale && strlenW(locale) == 2)
    {
        if (len && len < 3)
        {
            SetLastError(ERROR_INSUFFICIENT_BUFFER);
            return 0;
        }

        if (len) strcpyW(buffer, locale);
        return 3;
    }

    return GetpLocaleInfoW(lcid, info, buffer, len);
}

/***********************************************************************
 *             GetSystemPreferredUILanguages (KERNEL32.@)
 */
BOOL 
WINAPI 
GetSystemPreferredUILanguages(
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
)
{
	  NTSTATUS status;
	  BOOL result;
	  DWORD error; 

	  status = RtlGetSystemPreferredUILanguages(dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
	  if (!NT_SUCCESS(status) )
	  {
		error = RtlNtStatusToDosError(status);
		SetLastError(error);
		result = FALSE;
	  }
	  else
	  {
		result = TRUE;
	  }
	  return result;
}

/***********************************************************************
 *              GetThreadPreferredUILanguages (KERNEL32.@)
 */
BOOL 
WINAPI 
GetThreadPreferredUILanguages( 
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
)
{
	  NTSTATUS status;
	  BOOL result;
	  DWORD error; 

	  status = RtlGetThreadPreferredUILanguages(dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
	  if (!NT_SUCCESS(status) )
	  {
		error = RtlNtStatusToDosError(status);
		SetLastError(error);
		result = FALSE;
	  }
	  else
	  {
		result = TRUE;
	  }
	  return result;
}

/***********************************************************************
 *              GetThreadPreferredUILanguages (KERNEL32.@)
 */
BOOL 
WINAPI 
GetUserPreferredUILanguages( 
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
)
{
	  NTSTATUS status;
	  BOOL result;
	  DWORD error; 

	  status = RtlGetUserPreferredUILanguages(dwFlags, FALSE, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
	  if (!NT_SUCCESS(status) )
	  {
		error = RtlNtStatusToDosError(status);
		SetLastError(error);
		result = FALSE;
	  }
	  else
	  {
		result = TRUE;
	  }
	  return result;
}

////////////////////////////////////////////////////////////////////////////
//
//  NlsConvertIntegerToString
//
//  This routine converts an integer to a Unicode string.
//
////////////////////////////////////////////////////////////////////////////

ULONG NlsConvertIntegerToString(
    UINT Value,
    UINT Base,
    UINT Padding,
    LPWSTR pResultBuf,
    UINT Size)
{
    UNICODE_STRING ObString;                // value string
    UINT ctr;                               // loop counter
    LPWSTR pBufPtr;                         // ptr to result buffer
    WCHAR pTmpBuf[MAX_PATH_LEN];            // ptr to temp buffer
    ULONG rc = 0L;                          // return code

    //
    //  Set up the Unicode string structure.
    //
    ObString.Length = (USHORT)(Size * sizeof(WCHAR));
    ObString.MaximumLength = (USHORT)(Size * sizeof(WCHAR));
    ObString.Buffer = pTmpBuf;

    //
    //  Get the value as a string.
    //
    if (rc = RtlIntegerToUnicodeString(Value, Base, &ObString))
    {
        return (rc);
    }

    //
    //  Pad the string with the appropriate number of zeros.
    //
    pBufPtr = pResultBuf;
    for (ctr = GET_WC_COUNT(ObString.Length);
         ctr < Padding;
         ctr++, pBufPtr++, Size--)
    {
        if( Size < 1 )
        {
            return(STATUS_UNSUCCESSFUL);
        }

        *pBufPtr = NLS_CHAR_ZERO;
    }

    if(FAILED(StringCchCopyW(pBufPtr, Size, ObString.Buffer)))
    {
        return(STATUS_UNSUCCESSFUL);
    }

    return(STATUS_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////
//
//  QueryRegValue
//
//  This routine queries the given value from the registry.
//
//  NOTE: If pIfAlloc is NULL, then no buffer will be allocated.
//        If this routine is successful, the CALLER must free the
//        ppKeyValueFull information buffer if *pIfAlloc is TRUE.
//
////////////////////////////////////////////////////////////////////////////

ULONG QueryRegValue(
    HANDLE hKeyHandle,
    LPWSTR pValue,
    PKEY_VALUE_FULL_INFORMATION *ppKeyValueFull,
    ULONG Length,
    LPBOOL pIfAlloc)
{
    UNICODE_STRING ObValueName;        // value name
    PVOID pBuffer;                     // ptr to buffer for enum
    ULONG ResultLength;                // # bytes written
    ULONG rc = 0L;                     // return code


    //
    //  Set contents of pIfAlloc to FALSE to show that we did NOT do a
    //  memory allocation (yet).
    //
    if (pIfAlloc)
    {
        *pIfAlloc = FALSE;
    }

    //
    //  Query the value from the registry.
    //
    RtlInitUnicodeString(&ObValueName, pValue);

    RtlZeroMemory(*ppKeyValueFull, Length);
    rc = NtQueryValueKey( hKeyHandle,
                          &ObValueName,
                          KeyValueFullInformation,
                          *ppKeyValueFull,
                          Length,
                          &ResultLength );

    //
    //  Check the error code.  If the buffer is too small, allocate
    //  a new one and try the query again.
    //
    if ((rc == STATUS_BUFFER_OVERFLOW) && (pIfAlloc))
    {
        //
        //  Buffer is too small, so allocate a new one.
        //
        NLS_REG_BUFFER_ALLOC(*ppKeyValueFull, ResultLength, pBuffer, FALSE);
        RtlZeroMemory(*ppKeyValueFull, ResultLength);
        rc = NtQueryValueKey( hKeyHandle,
                              &ObValueName,
                              KeyValueFullInformation,
                              *ppKeyValueFull,
                              ResultLength,
                              &ResultLength );

        //
        //  Set contents of pIfAlloc to TRUE to show that we DID do
        //  a memory allocation.
        //
        *pIfAlloc = TRUE;
    }

    //
    //  If there is an error at this point, then the query failed.
    //
    if (rc != NO_ERROR)
    {
        if ((pIfAlloc) && (*pIfAlloc))
        {
            NLS_REG_BUFFER_FREE(pBuffer);
        }
        return (rc);
    }

    //
    //  Return success.
    //
    return (NO_ERROR);
}

////////////////////////////////////////////////////////////////////////////
//
//  IsValidSortId
//
//  Checks to see if the given locale has a valid sort id.
//
////////////////////////////////////////////////////////////////////////////

BOOL IsValidSortId(
    LCID Locale)
{
    WCHAR pTmpBuf[MAX_PATH];                     // temp buffer
    PKEY_VALUE_FULL_INFORMATION pKeyValueFull;   // ptr to query info
    BYTE pStatic[MAX_KEY_VALUE_FULLINFO];        // ptr to static buffer
    BOOL IfAlloc = FALSE;                        // if buffer was allocated
    ULONG rc = 0L;                               // return code


    //
    //  Make sure there is a sort id.
    //
    if (!SORTIDFROMLCID(Locale))
    {
        return (TRUE);
    }

    //
    //  Open the Alternate Sorts registry key.
    //
    OPEN_ALT_SORTS_KEY(FALSE);


    //
    //  Convert locale value to Unicode string.
    //
    if (NlsConvertIntegerToString(Locale, 16, 8, pTmpBuf, MAX_PATH))
    {
        return (FALSE);
    }

    //
    //  Query the registry for the value.
    //
    pKeyValueFull = (PKEY_VALUE_FULL_INFORMATION)pStatic;
    if (rc = QueryRegValue( hAltSortsKey,
                            pTmpBuf,
                            &pKeyValueFull,
                            MAX_KEY_VALUE_FULLINFO,
                            &IfAlloc ))
    {
        return (FALSE);
    }

    //
    //  Free the buffer used for the query.
    //
    if (IfAlloc)
    {
        NLS_FREE_MEM(pKeyValueFull);
    }

    //
    //  Return success.
    //
    return (TRUE);
}

////////////////////////////////////////////////////////////////////////////
//
//  GetLocHashNode
//
//  Returns a pointer to the appropriate LOC hash node given the locale.
//  If no table could be found for the given locale, NULL is returned.
//
////////////////////////////////////////////////////////////////////////////

PLOC_HASH FASTCALL GetLocHashNode(
    LCID Locale)
{
    PLOC_HASH pHashN;             // ptr to LOC hash node


    //
    //  Get hash node.
    //
    FIND_LOCALE_HASH_NODE(Locale, pHashN);

    //
    //  If the hash node does not exist, try to get the table
    //  from locale.nls.
    //
    //  NOTE:  No need to check error code from GetLocaleFileInfo,
    //         because pHashN is not touched if there was an
    //         error.  Thus, pHashN will still be NULL, and an
    //         "error" will be returned from this routine.
    //
    if (pHashN == NULL)
    {
        //
        //  If a sort id exists, make sure it's valid.
        //
        if (SORTIDFROMLCID(Locale))
        {
            if (!IsValidSortId(Locale))
            {
                return (NULL);
            }
        }

        //
        //  Hash node does NOT exist.
        //
        RtlEnterCriticalSection(&gcsTblPtrs);
        FIND_LOCALE_HASH_NODE(Locale, pHashN);
        if (pHashN == NULL)
        {
            //
            //  Hash node still does NOT exist.
            //
            GetLocaleFileInfo(Locale, &pHashN, TRUE);
            RtlLeaveCriticalSection(&gcsTblPtrs);
            return (pHashN);
        }
        RtlLeaveCriticalSection(&gcsTblPtrs);
    }

    //
    //  Hash node DOES exist.
    //
    if (!EXIST_LOCALE_INFO(pHashN))
    {
        //
        //  Locale tables not yet stored in hash node.
        //
        RtlEnterCriticalSection(&gcsTblPtrs);
        if (!EXIST_LOCALE_INFO(pHashN))
        {
            if (GetLocaleFileInfo(Locale, &pHashN, FALSE))
            {
                RtlLeaveCriticalSection(&gcsTblPtrs);
                return (NULL);
            }
        }
        RtlLeaveCriticalSection(&gcsTblPtrs);
    }

    //
    //  Return pointer to hash node.
    //
    return (pHashN);
}

/*
 * @unimplemented
*/
BOOL 
WINAPI 
GetNLSVersionEx(
  _In_      NLS_FUNCTION function,
  _In_opt_  LPCWSTR lpLocaleName,
  _Inout_   LPNLSVERSIONINFOEX lpVersionInformation
)
{
	NLSVERSIONINFO lpVersionInformationOld;
	LCID lcid = LocaleNameToLCID(lpLocaleName, 0);
	
	lpVersionInformationOld.dwNLSVersionInfoSize = sizeof(LPNLSVERSIONINFO);
	
	if(GetNLSVersion(function, lcid , &lpVersionInformationOld))
	{
		lpVersionInformation->dwNLSVersionInfoSize = sizeof(LPNLSVERSIONINFOEX);
		lpVersionInformation->dwNLSVersion = lpVersionInformationOld.dwNLSVersion;
		lpVersionInformation->dwDefinedVersion = lpVersionInformationOld.dwDefinedVersion;
		lpVersionInformation->dwEffectiveId = LocaleNameToLCID(lpLocaleName, 0);		
		return TRUE;
	}else{
		return FALSE;
	}	
}

////////////////////////////////////////////////////////////////////////////
//
//  MapSection
//
//  This routine maps a view of the section to the current process and adds
//  the appropriate information to the hash table.
//
////////////////////////////////////////////////////////////////////////////

ULONG MapSection(
    HANDLE hSec,
    PVOID *ppBaseAddr,
    ULONG PageProtection,
    BOOL bCloseHandle)
{
    SIZE_T ViewSize;                   // view size of mapped section
    ULONG rc = 0L;                     // return code


    //
    //  Make sure we're in the critical section when entering this call.
    //
    ASSERT(NtCurrentTeb()->ClientId.UniqueThread == gcsTblPtrs.OwningThread);

    //
    //  Map a View of the Section.
    //
    *ppBaseAddr = (PVOID)NULL;
    ViewSize = 0L;

    rc = NtMapViewOfSection( hSec,
                             NtCurrentProcess(),
                             ppBaseAddr,
                             0L,
                             0L,
                             NULL,
                             &ViewSize,
                             ViewUnmap,
                             0L,
                             PageProtection );

    //
    //  Close the handle to the section.  Once the section has been mapped,
    //  the pointer to the base address will remain valid until the section
    //  is unmapped.  It is not necessary to leave the handle to the section
    //  around.
    //
    if (bCloseHandle)
    {
        NtClose(hSec);
    }

    //
    //  Check for error from NtMapViewOfSection.
    //
    if (!NT_SUCCESS(rc))
    {
        DbgPrint("NLSAPI: Could NOT Map View of Section - %lx.\n", rc);
        return (rc);
    }

    //
    //  Return success.
    //
    return (NO_ERROR);
}

////////////////////////////////////////////////////////////////////////////
//
//  OpenSection
//
//  This routine opens the named memory mapped section for the given section
//  name and returns the handle to the section.
//
////////////////////////////////////////////////////////////////////////////

ULONG OpenSection(
    HANDLE *phSec,
    PUNICODE_STRING pObSectionName,
    PVOID *ppBaseAddr,
    ULONG AccessMask,
    BOOL bCloseHandle)
{
    OBJECT_ATTRIBUTES ObjA;            // object attributes structure
    ULONG rc = 0L;                     // return code


    //
    //  Make sure we're in the critical section when entering this call.
    //
    ASSERT(NtCurrentTeb()->ClientId.UniqueThread == gcsTblPtrs.OwningThread);

    //
    //  Open the Section.
    //
    InitializeObjectAttributes( &ObjA,
                                pObSectionName,
                                OBJ_CASE_INSENSITIVE,
                                NULL,
                                NULL );

    rc = NtOpenSection( phSec,
                        AccessMask,
                        &ObjA );

    //
    //  Check for error from NtOpenSection.
    //
    if (!NT_SUCCESS(rc))
    {
        return (rc);
    }

    //
    //  Map a View of the Section.
    //
    if (rc = MapSection( *phSec,
                         ppBaseAddr,
                         PAGE_READONLY,
                         FALSE ))
    {
        NtClose(*phSec);
        return (rc);
    }

    //
    //  Close the handle to the section.  Once the section has been mapped,
    //  the pointer to the base address will remain valid until the section
    //  is unmapped.  It is not necessary to leave the handle to the section
    //  around.
    //
    if (bCloseHandle)
    {
        NtClose(*phSec);
    }

    //
    //  Return success.
    //
    return (NO_ERROR);
}

////////////////////////////////////////////////////////////////////////////
//
//  MakeLocHashNode
//
//  Gets the pointers to the locale tables and stores them in the locale
//  hash node given.
//
//  NOTE:  If a critical section is needed to touch pHashN, then the
//         critical section must be entered before calling this routine.
//
////////////////////////////////////////////////////////////////////////////

ULONG MakeLocHashNode(
    LCID Locale,
    LPWORD pBaseAddr,
    PLOC_HASH *ppNode,
    BOOLEAN fCreateNode)
{
    LANGID Language;              // language id
    PLOC_HASH pHashN;             // ptr to LOC hash node
    DWORD Num;                    // total number of locales
    PLOCALE_HDR pFileHdr;         // ptr to locale header entry
    ULONG rc = 0L;                // return code


    //
    //  Save the language id.
    //
    Language = LANGIDFROMLCID(Locale);

    //
    //  Search for the right locale id information.
    //
    Num = ((PLOC_CAL_HDR)pBaseAddr)->NumLocales;
    pFileHdr = (PLOCALE_HDR)(pBaseAddr + LOCALE_HDR_OFFSET);
    for (; (Num != 0) && (pFileHdr->Locale != Language); Num--, pFileHdr++)
        ;

    //
    //  See if the locale was found in the file.
    //
    if (Num != 0)
    {
        //
        //  Locale id was found, so increment the pointer to point at
        //  the beginning of the locale information.
        //
        pBaseAddr += pFileHdr->Offset;
    }
    else
    {
        //
        //  Return an error.  The given locale is not supported.
        //
        return (ERROR_INVALID_PARAMETER);
    }

    //
    //  If fCreateNode is TRUE, then allocate LOC_HASH structure.
    //
    if (fCreateNode)
    {
        CREATE_LOCALE_HASH_NODE(Locale, pHashN);
    }
    else
    {
        pHashN = *ppNode;
    }

    //
    //  Attach Information to structure.
    //
    //  The pLocaleFixed value must be set LAST, since this is the pointer
    //  that is checked to see that the locale information has been
    //  initialized.
    //
    pHashN->pLocaleHdr   = (PLOCALE_VAR)pBaseAddr;
    pHashN->pLocaleFixed = (PLOCALE_FIXED)(pBaseAddr +
                                           (sizeof(LOCALE_VAR) / sizeof(WORD)));

    //
    //  If fCreateNode is TRUE, then insert hash node and save pointer.
    //
    if (fCreateNode)
    {
        //
        //  Insert LOC hash node into hash table.
        //
        INSERT_LOC_HASH_NODE(pHashN, pBaseAddr);

        //
        //  Save the pointer to the hash node.
        //
        if (ppNode != NULL)
        {
            *ppNode = pHashN;
        }
    }

    //
    //  Return success.
    //
    return (NO_ERROR);
}

////////////////////////////////////////////////////////////////////////////
//
//  GetLocaleFileInfo
//
//  Opens and Maps a view of the section for the given locale.  It then
//  creates and inserts a hash node into the global LOCALE hash table.
//
//  If the section cannot be opened, it then queries the registry to see if
//  the information has been added since the initialization of the DLL.  If
//  so, then it creates the section and then opens and maps a view of it.
//
////////////////////////////////////////////////////////////////////////////

ULONG GetLocaleFileInfo(
    LCID Locale,
    PLOC_HASH *ppNode,
    BOOLEAN fCreateNode)
{
    HANDLE hSec = (HANDLE)0;      // section handle
    UNICODE_STRING ObSecName;     // section name
    LPWORD pBaseAddr;             // ptr to base address of section
    ULONG rc = 0L;                // return code


    //
    //  Make sure we're in the critical section when entering this call.
    //
    ASSERT(NtCurrentTeb()->ClientId.UniqueThread == gcsTblPtrs.OwningThread);

    //
    //  Open and Map a view of the section if it hasn't been done yet.
    //
    if ((pBaseAddr = pTblPtrs->pLocaleInfo) == NULL)
    {
        //
        //  Get the locale file section pointer.
        //
        ObSecName.Buffer = NLS_SECTION_LOCALE;
        ObSecName.Length = sizeof (NLS_SECTION_LOCALE) - sizeof (WCHAR);
        ObSecName.MaximumLength = ObSecName.Length;

        if (rc = OpenSection( &hSec,
                              &ObSecName,
                              (PVOID *)&pBaseAddr,
                              SECTION_MAP_READ,
                              TRUE ))
        {
            return (rc);
        }

        //
        //  Store pointer to locale file and calendar info in table
        //  structure.
        //
        pTblPtrs->pLocaleInfo = pBaseAddr;

        pTblPtrs->NumCalendars = ((PLOC_CAL_HDR)pBaseAddr)->NumCalendars;
        pTblPtrs->pCalendarInfo = pBaseAddr +
                                  ((PLOC_CAL_HDR)pBaseAddr)->CalOffset;
    }

    //
    //  Make the hash node and return the result.
    //
    return (MakeLocHashNode( Locale,
                             pBaseAddr,
                             ppNode,
                             fCreateNode ));
}

////////////////////////////////////////////////////////////////////////////
//
//  UnMapSection
//
//  This routine unmaps a view of the given section to the current process.
//
////////////////////////////////////////////////////////////////////////////

ULONG UnMapSection(
    PVOID pBaseAddr)
{
    ULONG rc = 0L;                     // return code


    //
    //  Make sure we're in the critical section when entering this call.
    //
    ASSERT(NtCurrentTeb()->ClientId.UniqueThread == gcsTblPtrs.OwningThread);

    //
    //  UnMap a View of the Section.
    //
    rc = NtUnmapViewOfSection( NtCurrentProcess(),
                               pBaseAddr );

    //
    //  Check for error from NtUnmapViewOfSection.
    //
    if (!NT_SUCCESS(rc))
    {
        DbgPrint("NLSAPI: Could NOT Unmap View of Section - %lx.\n", rc);
        return (rc);
    }

    //
    //  Return success.
    //
    return (NO_ERROR);
}

////////////////////////////////////////////////////////////////////////////
//
//  OpenRegKey
//
//  This routine opens a key in the registry.
//
////////////////////////////////////////////////////////////////////////////

ULONG OpenRegKey(
    PHANDLE phKeyHandle,
    LPWSTR pBaseName,
    LPWSTR pKey,
    ULONG fAccess)
{
    WCHAR pwszKeyName[MAX_PATH_LEN];   // ptr to the full key name
    HANDLE UserKeyHandle;              // HKEY_CURRENT_USER equivalent
    OBJECT_ATTRIBUTES ObjA;            // object attributes structure
    UNICODE_STRING ObKeyName;          // key name
    ULONG rc = 0L;                     // return code


    //
    //  Get the full key name.
    //
    if (pBaseName == NULL)
    {
        //
        //  Get current user's key handle.
        //
        rc = RtlOpenCurrentUser(MAXIMUM_ALLOWED, &UserKeyHandle);
        if (!NT_SUCCESS(rc))
        {
            DbgPrint("NLSAPI: Could NOT Open HKEY_CURRENT_USER - %lx.\n", rc);
            return (rc);
        }
        pwszKeyName[0] = UNICODE_NULL;
    }
    else
    {
        //
        //  Base name exists, so not current user.
        //
        UserKeyHandle = NULL;
        if(FAILED(StringCchCopyW(pwszKeyName, ARRAYSIZE(pwszKeyName), pBaseName)))
        {
            return(STATUS_UNSUCCESSFUL);
        }
    }

    if (pKey)
    {
        if(FAILED(StringCchCatW(pwszKeyName, ARRAYSIZE(pwszKeyName), pKey)))
        {
            if (UserKeyHandle != NULL)
            {
                NtClose(UserKeyHandle);
            }
            return(STATUS_UNSUCCESSFUL);
        }
    }

    //
    //  Open the registry key.
    //
    RtlInitUnicodeString(&ObKeyName, pwszKeyName);
    InitializeObjectAttributes( &ObjA,
                                &ObKeyName,
                                OBJ_CASE_INSENSITIVE,
                                UserKeyHandle,
                                NULL );
    rc = NtOpenKey( phKeyHandle,
                    fAccess,
                    &ObjA );

    //
    //  Close the current user handle, if necessary.
    //
    if (UserKeyHandle != NULL)
    {
        NtClose(UserKeyHandle);
    }

    //
    //  Check for error from NtOpenKey.
    //
    if (!NT_SUCCESS(rc))
    {
        *phKeyHandle = NULL;
    }

    //
    //  Return the status from NtOpenKey.
    //
    return (rc);
}

////////////////////////////////////////////////////////////////////////////
//
//  GetNLSVersion
//
//  Return the version of a specific NLS function.
//
////////////////////////////////////////////////////////////////////////////
BOOL WINAPI GetNLSVersion(
    NLS_FUNCTION     function,
    LCID             locale,
    LPNLSVERSIONINFO lpVersionInformation)
{
	pGetNLSVersion nlsVersion;
	PLOC_HASH pHashN;
	
	nlsVersion = (pGetNLSVersion) GetProcAddress(
                            GetModuleHandleW(L"kernelfull"),
                            "GetNLSVersion");
							
	if(nlsVersion!=NULL){
		return nlsVersion(function, locale, lpVersionInformation);
	}
   
    
    //
    //  Invalid parameter check:
    //    - validate LCID
    //    - NULL data pointer
    //
    //  NOTE: invalid function is checked in the switch statement below.
    //
    VALIDATE_LOCALE(locale, pHashN, FALSE);
    if ((lpVersionInformation == NULL) || (pHashN == NULL))
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return (FALSE);
    }

    //
    //  Buffer size check.
    //
    if (lpVersionInformation->dwNLSVersionInfoSize != sizeof(NLSVERSIONINFO)) 
    {
        SetLastError( ERROR_INSUFFICIENT_BUFFER );
        return (FALSE);
    }

    //
    //  Make sure the appropriate tables are available.  If not,
    //  return an error.
    //
    if (pTblPtrs->pSortVersion == NULL )
    {
        DbgPrint(("NLSAPI: Appropriate Versioning Table Not Loaded.\n"));
        SetLastError(ERROR_FILE_NOT_FOUND);
        return (FALSE);
    }

    //
    //  Make sure the appropriate tables are available.  If not,
    //  return an error.
    //
    if (pTblPtrs->pDefinedVersion == NULL)
    {
        DbgPrint(("NLSAPI: Appropriate Defined Code Point Table Not Loaded.\n"));
        SetLastError(ERROR_FILE_NOT_FOUND);
        return (FALSE);
    }
    
    //
    //  Check which NLS functionnality version is requested.
    //
    switch (function)
    {
        case (COMPARE_STRING):
            {
                UINT i;
                
                //
                //  Get the Defined version. Always the first entry
                //  in the Defined Code Point version table. The first
                //  entry represent the current Defined version.
                //
                
                lpVersionInformation->dwDefinedVersion = (pTblPtrs->pDefinedVersion)[0].Version;
                
                //
                //  Get the NLS sorting version. Search specific locale 
                //  version info. Start from the second entry; after default
                //  value.
                //
                lpVersionInformation->dwNLSVersion = (pTblPtrs->pSortVersion)[0].Version;
                for (i = 1; i < pTblPtrs->NumSortVersion; i++)
                {
                    if (pHashN->Locale == (pTblPtrs->pSortVersion)[i].Locale)
                    {
                        lpVersionInformation->dwNLSVersion = (pTblPtrs->pSortVersion)[i].Version;
                        break;
                    }
                }

                break;
            }
//        case (NORMALIZE_STRING):
//            {
//                //
//                //  Not implemented yet.
//                //
//                SetLastError(ERROR_NOT_SUPPORTED);
//                return (FALSE);
//            }
        default:
            {
                SetLastError(ERROR_INVALID_FLAGS);
                return (FALSE);
            }
    }

    return (TRUE);
}