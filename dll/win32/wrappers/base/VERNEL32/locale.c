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
    struct locale_name locale_name;

    if (name == LOCALE_NAME_USER_DEFAULT)
        return GetUserDefaultLCID();

    /* string parsing */
    parse_locale_name( name, &locale_name );

    if (!locale_name.matches)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return 0;
    }


    return locale_name.lcid;
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

    return GetLocaleInfoW(lcid, info, buffer, len);
}

/***********************************************************************
 *           LCIDToLocaleName  (KERNEL32.@)
 */
INT 
WINAPI 
LCIDToLocaleName( 
	LCID lcid, 
	LPWSTR name, 
	INT count, 
	DWORD flags 
)
{
    return GetLocaleInfoW( lcid, LOCALE_SNAME | LOCALE_NOUSEROVERRIDE, name, count );
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
    struct locale_name locale_name;

    if (!locale)
        return FALSE;

    /* string parsing */
    parse_locale_name( locale, &locale_name );

    return locale_name.matches > 0;
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
     NtQueryDefaultUILanguage( &lang );
     DbgPrint("GetThreadUILanguage is UNIMPLEMENTED, returning default language.\n");
     return lang;
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