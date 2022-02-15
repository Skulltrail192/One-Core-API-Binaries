/*++

Copyright (c) 2017 Shorthorn Project.

Module Name:

    locale.c

Abstract:

    This file contains functions that return information about a
    language group, a UI language, a locale, or a calendar.

Revision History:

    Skulltrail 22-03-2017

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

#define LOCALE_LOCALEINFOFLAGSMASK (LOCALE_NOUSEROVERRIDE|LOCALE_USE_CP_ACP|\
                                    LOCALE_RETURN_NUMBER|LOCALE_RETURN_GENITIVE_NAMES)
									
#define LOCALE_ALLOW_NEUTRAL_NAMES    0x08000000									
									
static const GUID default_sort_guid = { 0x00000001, 0x57ee, 0x1e5c, { 0x00, 0xb4, 0xd0, 0x00, 0x0b, 0xb1, 0xe1, 0x1e }};									
									
static LPWSTR systemLocale;			

static const struct sortguid *current_locale_sort;						

/* locale ids corresponding to the various Unix locale parameters */
static LCID lcid_LC_COLLATE;
static LCID lcid_LC_CTYPE;
static LCID lcid_LC_MESSAGES;
static LCID lcid_LC_MONETARY;
static LCID lcid_LC_NUMERIC;
static LCID lcid_LC_TIME;
static LCID lcid_LC_PAPER;
static LCID lcid_LC_MEASUREMENT;
static LCID lcid_LC_TELEPHONE;

static HKEY nls_key;
static HKEY tz_key;

static RTL_CRITICAL_SECTION cache_section;

/* move to winnls*/

#define LOCALE_SNAN                 0x0069
#define LOCALE_SNEGINFINITY         0x006B
#define LOCALE_SPOSINFINITY         0x006A
#define LOCALE_SDURATION            0x005D
#define LOCALE_SSHORTESTDAYNAME1    0x0060
#define LOCALE_SSHORTESTDAYNAME2    0x0061
#define LOCALE_SSHORTESTDAYNAME3    0x0062
#define LOCALE_SSHORTESTDAYNAME4    0x0063
#define LOCALE_SSHORTESTDAYNAME5    0x0064
#define LOCALE_SSHORTESTDAYNAME6    0x0065
#define LOCALE_SSHORTESTDAYNAME7    0x0066

WINE_DEFAULT_DEBUG_CHANNEL(locale); 

struct sortguid
{
    GUID  id;          /* sort GUID */
    DWORD flags;       /* flags */
    DWORD compr;       /* offset to compression table */
    DWORD except;      /* exception table offset in sortkey table */
    DWORD ling_except; /* exception table offset for linguistic casing */
    DWORD casemap;     /* linguistic casemap table offset */
};

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

static struct
{
    DWORD           *keys;       /* sortkey table, indexed by char */
    USHORT          *casemap;    /* casemap table, in l_intl.nls format */
    WORD            *ctypes;     /* CT_CTYPE1,2,3 values */
    BYTE            *ctype_idx;  /* index to map char to ctypes array entry */
    DWORD            version;    /* NLS version */
    DWORD            guid_count; /* number of sort GUIDs */
    struct sortguid *guids;      /* table of sort GUIDs */
} sort;

static const WCHAR iCalendarTypeW[] = {'i','C','a','l','e','n','d','a','r','T','y','p','e',0};
static const WCHAR iCountryW[] = {'i','C','o','u','n','t','r','y',0};
static const WCHAR iCurrDigitsW[] = {'i','C','u','r','r','D','i','g','i','t','s',0};
static const WCHAR iCurrencyW[] = {'i','C','u','r','r','e','n','c','y',0};
static const WCHAR iDateW[] = {'i','D','a','t','e',0};
static const WCHAR iDigitsW[] = {'i','D','i','g','i','t','s',0};
static const WCHAR iFirstDayOfWeekW[] = {'i','F','i','r','s','t','D','a','y','O','f','W','e','e','k',0};
static const WCHAR iFirstWeekOfYearW[] = {'i','F','i','r','s','t','W','e','e','k','O','f','Y','e','a','r',0};
static const WCHAR iLDateW[] = {'i','L','D','a','t','e',0};
static const WCHAR iLZeroW[] = {'i','L','Z','e','r','o',0};
static const WCHAR iMeasureW[] = {'i','M','e','a','s','u','r','e',0};
static const WCHAR iNegCurrW[] = {'i','N','e','g','C','u','r','r',0};
static const WCHAR iNegNumberW[] = {'i','N','e','g','N','u','m','b','e','r',0};
static const WCHAR iPaperSizeW[] = {'i','P','a','p','e','r','S','i','z','e',0};
static const WCHAR iTLZeroW[] = {'i','T','L','Z','e','r','o',0};
static const WCHAR iTimePrefixW[] = {'i','T','i','m','e','P','r','e','f','i','x',0};
static const WCHAR iTimeW[] = {'i','T','i','m','e',0};
static const WCHAR s1159W[] = {'s','1','1','5','9',0};
static const WCHAR s2359W[] = {'s','2','3','5','9',0};
static const WCHAR sCountryW[] = {'s','C','o','u','n','t','r','y',0};
static const WCHAR sCurrencyW[] = {'s','C','u','r','r','e','n','c','y',0};
static const WCHAR sDateW[] = {'s','D','a','t','e',0};
static const WCHAR sDecimalW[] = {'s','D','e','c','i','m','a','l',0};
static const WCHAR sGroupingW[] = {'s','G','r','o','u','p','i','n','g',0};
static const WCHAR sLanguageW[] = {'s','L','a','n','g','u','a','g','e',0};
static const WCHAR sListW[] = {'s','L','i','s','t',0};
static const WCHAR sLongDateW[] = {'s','L','o','n','g','D','a','t','e',0};
static const WCHAR sMonDecimalSepW[] = {'s','M','o','n','D','e','c','i','m','a','l','S','e','p',0};
static const WCHAR sMonGroupingW[] = {'s','M','o','n','G','r','o','u','p','i','n','g',0};
static const WCHAR sMonThousandSepW[] = {'s','M','o','n','T','h','o','u','s','a','n','d','S','e','p',0};
static const WCHAR sNativeDigitsW[] = {'s','N','a','t','i','v','e','D','i','g','i','t','s',0};
static const WCHAR sNegativeSignW[] = {'s','N','e','g','a','t','i','v','e','S','i','g','n',0};
static const WCHAR sPositiveSignW[] = {'s','P','o','s','i','t','i','v','e','S','i','g','n',0};
static const WCHAR sShortDateW[] = {'s','S','h','o','r','t','D','a','t','e',0};
static const WCHAR sThousandW[] = {'s','T','h','o','u','s','a','n','d',0};
static const WCHAR sTimeFormatW[] = {'s','T','i','m','e','F','o','r','m','a','t',0};
static const WCHAR sTimeW[] = {'s','T','i','m','e',0};
static const WCHAR sYearMonthW[] = {'s','Y','e','a','r','M','o','n','t','h',0};
static const WCHAR NumShapeW[] = {'N','u','m','s','h','a','p','e',0};

static struct registry_value
{
    DWORD           lctype;
    const WCHAR    *name;
    WCHAR          *cached_value;
} registry_values[] =
{
    { LOCALE_ICALENDARTYPE, iCalendarTypeW },
    { LOCALE_ICURRDIGITS, iCurrDigitsW },
    { LOCALE_ICURRENCY, iCurrencyW },
    { LOCALE_IDIGITS, iDigitsW },
    { LOCALE_IFIRSTDAYOFWEEK, iFirstDayOfWeekW },
    { LOCALE_IFIRSTWEEKOFYEAR, iFirstWeekOfYearW },
    { LOCALE_ILZERO, iLZeroW },
    { LOCALE_IMEASURE, iMeasureW },
    { LOCALE_INEGCURR, iNegCurrW },
    { LOCALE_INEGNUMBER, iNegNumberW },
    { LOCALE_IPAPERSIZE, iPaperSizeW },
    { LOCALE_ITIME, iTimeW },
    { LOCALE_S1159, s1159W },
    { LOCALE_S2359, s2359W },
    { LOCALE_SCURRENCY, sCurrencyW },
    { LOCALE_SDATE, sDateW },
    { LOCALE_SDECIMAL, sDecimalW },
    { LOCALE_SGROUPING, sGroupingW },
    { LOCALE_SLIST, sListW },
    { LOCALE_SLONGDATE, sLongDateW },
    { LOCALE_SMONDECIMALSEP, sMonDecimalSepW },
    { LOCALE_SMONGROUPING, sMonGroupingW },
    { LOCALE_SMONTHOUSANDSEP, sMonThousandSepW },
    { LOCALE_SNEGATIVESIGN, sNegativeSignW },
    { LOCALE_SPOSITIVESIGN, sPositiveSignW },
    { LOCALE_SSHORTDATE, sShortDateW },
    { LOCALE_STHOUSAND, sThousandW },
    { LOCALE_STIME, sTimeW },
    { LOCALE_STIMEFORMAT, sTimeFormatW },
    { LOCALE_SYEARMONTH, sYearMonthW },
    /* The following are not listed under MSDN as supported,
     * but seem to be used and also stored in the registry.
     */
    { LOCALE_ICOUNTRY, iCountryW },
    { LOCALE_IDATE, iDateW },
    { LOCALE_ILDATE, iLDateW },
    { LOCALE_ITLZERO, iTLZeroW },
    { LOCALE_SCOUNTRY, sCountryW },
    { LOCALE_SABBREVLANGNAME, sLanguageW },
    /* The following are used in XP and later */
    { LOCALE_IDIGITSUBSTITUTION, NumShapeW },
    { LOCALE_SNATIVEDIGITS, sNativeDigitsW },
    { LOCALE_ITIMEMARKPOSN, iTimePrefixW }
};

static const struct sortguid *find_sortguid( const GUID *guid )
{
    int pos, ret, min = 0, max = sort.guid_count - 1;

    while (min <= max)
    {
        pos = (min + max) / 2;
        ret = memcmp( guid, &sort.guids[pos].id, sizeof(*guid) );
        if (!ret) return &sort.guids[pos];
        if (ret > 0) min = pos + 1;
        else max = pos - 1;
    }
    ERR( "no sort found for %s\n", debugstr_guid( guid ));
    return NULL;
}

static const struct sortguid *get_language_sort( const WCHAR *locale )
{
    WCHAR *p, *end, buffer[LOCALE_NAME_MAX_LENGTH], guidstr[39];
    const struct sortguid *ret;
    UNICODE_STRING str;
    GUID guid;
    HKEY key = 0;
    DWORD size, type;

    if (locale == LOCALE_NAME_USER_DEFAULT)
    {
        if (current_locale_sort) return current_locale_sort;
        GetUserDefaultLocaleName( buffer, ARRAY_SIZE( buffer ));
    }
    else lstrcpynW( buffer, locale, LOCALE_NAME_MAX_LENGTH );

    if (buffer[0] && !RegOpenKeyExW( nls_key, L"Sorting\\Ids", 0, KEY_READ, &key ))
    {
        for (;;)
        {
            size = sizeof(guidstr);
            if (!RegQueryValueExW( key, buffer, NULL, &type, (BYTE *)guidstr, &size ) && type == REG_SZ)
            {
                RtlInitUnicodeString( &str, guidstr );
                if (!RtlGUIDFromString( &str, &guid ))
                {
                    ret = find_sortguid( &guid );
                    goto done;
                }
                break;
            }
            for (p = end = buffer; *p; p++) if (*p == '-' || *p == '_') end = p;
            if (end == buffer) break;
            *end = 0;
        }
    }
    ret = find_sortguid( &default_sort_guid );
done:
    RegCloseKey( key );
    return ret;
}

/***********************************************************************
 *		init_locale
 */
void init_locale(void)
{
	current_locale_sort = get_language_sort( LOCALE_NAME_USER_DEFAULT );
    RegCreateKeyExW( HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Nls",
                     0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &nls_key, NULL );	
    RegCreateKeyExW( HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones",
                     0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &tz_key, NULL );					 
}

struct enum_locale_ex_data
{
    LOCALE_ENUMPROCEX proc;
    DWORD             flags;
    LPARAM            lparam;
};


void InitializeCriticalForLocaleInfo(){
	 RtlInitializeCriticalSection(&cache_section);
}

static void init_sortkeys( DWORD *ptr )
{
    WORD *ctype;
    DWORD *table;

    sort.keys = (DWORD *)((char *)ptr + ptr[0]);
    sort.casemap = (USHORT *)((char *)ptr + ptr[1]);

    ctype = (WORD *)((char *)ptr + ptr[2]);
    sort.ctypes = ctype + 2;
    sort.ctype_idx = (BYTE *)ctype + ctype[1] + 2;

    table = (DWORD *)((char *)ptr + ptr[3]);
    sort.version = table[0];
    sort.guid_count = table[1];
    sort.guids = (struct sortguid *)(table + 2);
}

/***********************************************************************
 *           is_genitive_name_supported
 *
 * Determine could LCTYPE basically support genitive name form or not.
 */
static BOOL is_genitive_name_supported( LCTYPE lctype )
{
    switch(lctype & 0xffff)
    {
    case LOCALE_SMONTHNAME1:
    case LOCALE_SMONTHNAME2:
    case LOCALE_SMONTHNAME3:
    case LOCALE_SMONTHNAME4:
    case LOCALE_SMONTHNAME5:
    case LOCALE_SMONTHNAME6:
    case LOCALE_SMONTHNAME7:
    case LOCALE_SMONTHNAME8:
    case LOCALE_SMONTHNAME9:
    case LOCALE_SMONTHNAME10:
    case LOCALE_SMONTHNAME11:
    case LOCALE_SMONTHNAME12:
    case LOCALE_SMONTHNAME13:
         return TRUE;
    default:
         return FALSE;
    }
}

/***********************************************************************
 *           convert_default_lcid
 *
 * Get the default LCID to use for a given lctype in GetLocaleInfo.
 */
static LCID convert_default_lcid( LCID lcid, LCTYPE lctype )
{
    if (lcid == LOCALE_SYSTEM_DEFAULT ||
        lcid == LOCALE_USER_DEFAULT ||
        lcid == LOCALE_NEUTRAL)
    {
        LCID default_id = 0;

        switch(lctype & 0xffff)
        {
        case LOCALE_SSORTNAME:
            default_id = lcid_LC_COLLATE;
            break;

        case LOCALE_FONTSIGNATURE:
        case LOCALE_IDEFAULTANSICODEPAGE:
        case LOCALE_IDEFAULTCODEPAGE:
        case LOCALE_IDEFAULTEBCDICCODEPAGE:
        case LOCALE_IDEFAULTMACCODEPAGE:
        case LOCALE_IDEFAULTUNIXCODEPAGE:
            default_id = lcid_LC_CTYPE;
            break;

        case LOCALE_ICURRDIGITS:
        case LOCALE_ICURRENCY:
        case LOCALE_IINTLCURRDIGITS:
        case LOCALE_INEGCURR:
        case LOCALE_INEGSEPBYSPACE:
        case LOCALE_INEGSIGNPOSN:
        case LOCALE_INEGSYMPRECEDES:
        case LOCALE_IPOSSEPBYSPACE:
        case LOCALE_IPOSSIGNPOSN:
        case LOCALE_IPOSSYMPRECEDES:
        case LOCALE_SCURRENCY:
        case LOCALE_SINTLSYMBOL:
        case LOCALE_SMONDECIMALSEP:
        case LOCALE_SMONGROUPING:
        case LOCALE_SMONTHOUSANDSEP:
        case LOCALE_SNATIVECURRNAME:
            default_id = lcid_LC_MONETARY;
            break;

        case LOCALE_IDIGITS:
        case LOCALE_IDIGITSUBSTITUTION:
        case LOCALE_ILZERO:
        case LOCALE_INEGNUMBER:
        case LOCALE_SDECIMAL:
        case LOCALE_SGROUPING:
        case LOCALE_SNAN:
        case LOCALE_SNATIVEDIGITS:
        case LOCALE_SNEGATIVESIGN:
        case LOCALE_SNEGINFINITY:
        case LOCALE_SPOSINFINITY:
        case LOCALE_SPOSITIVESIGN:
        case LOCALE_STHOUSAND:
            default_id = lcid_LC_NUMERIC;
            break;

        case LOCALE_ICALENDARTYPE:
        case LOCALE_ICENTURY:
        case LOCALE_IDATE:
        case LOCALE_IDAYLZERO:
        case LOCALE_IFIRSTDAYOFWEEK:
        case LOCALE_IFIRSTWEEKOFYEAR:
        case LOCALE_ILDATE:
        case LOCALE_IMONLZERO:
        case LOCALE_IOPTIONALCALENDAR:
        case LOCALE_ITIME:
        case LOCALE_ITIMEMARKPOSN:
        case LOCALE_ITLZERO:
        case LOCALE_S1159:
        case LOCALE_S2359:
        case LOCALE_SABBREVDAYNAME1:
        case LOCALE_SABBREVDAYNAME2:
        case LOCALE_SABBREVDAYNAME3:
        case LOCALE_SABBREVDAYNAME4:
        case LOCALE_SABBREVDAYNAME5:
        case LOCALE_SABBREVDAYNAME6:
        case LOCALE_SABBREVDAYNAME7:
        case LOCALE_SABBREVMONTHNAME1:
        case LOCALE_SABBREVMONTHNAME2:
        case LOCALE_SABBREVMONTHNAME3:
        case LOCALE_SABBREVMONTHNAME4:
        case LOCALE_SABBREVMONTHNAME5:
        case LOCALE_SABBREVMONTHNAME6:
        case LOCALE_SABBREVMONTHNAME7:
        case LOCALE_SABBREVMONTHNAME8:
        case LOCALE_SABBREVMONTHNAME9:
        case LOCALE_SABBREVMONTHNAME10:
        case LOCALE_SABBREVMONTHNAME11:
        case LOCALE_SABBREVMONTHNAME12:
        case LOCALE_SABBREVMONTHNAME13:
        case LOCALE_SDATE:
        case LOCALE_SDAYNAME1:
        case LOCALE_SDAYNAME2:
        case LOCALE_SDAYNAME3:
        case LOCALE_SDAYNAME4:
        case LOCALE_SDAYNAME5:
        case LOCALE_SDAYNAME6:
        case LOCALE_SDAYNAME7:
        case LOCALE_SDURATION:
        case LOCALE_SLONGDATE:
        case LOCALE_SMONTHNAME1:
        case LOCALE_SMONTHNAME2:
        case LOCALE_SMONTHNAME3:
        case LOCALE_SMONTHNAME4:
        case LOCALE_SMONTHNAME5:
        case LOCALE_SMONTHNAME6:
        case LOCALE_SMONTHNAME7:
        case LOCALE_SMONTHNAME8:
        case LOCALE_SMONTHNAME9:
        case LOCALE_SMONTHNAME10:
        case LOCALE_SMONTHNAME11:
        case LOCALE_SMONTHNAME12:
        case LOCALE_SMONTHNAME13:
        case LOCALE_SSHORTDATE:
        case LOCALE_SSHORTESTDAYNAME1:
        case LOCALE_SSHORTESTDAYNAME2:
        case LOCALE_SSHORTESTDAYNAME3:
        case LOCALE_SSHORTESTDAYNAME4:
        case LOCALE_SSHORTESTDAYNAME5:
        case LOCALE_SSHORTESTDAYNAME6:
        case LOCALE_SSHORTESTDAYNAME7:
        case LOCALE_STIME:
        case LOCALE_STIMEFORMAT:
        case LOCALE_SYEARMONTH:
            default_id = lcid_LC_TIME;
            break;

        case LOCALE_IPAPERSIZE:
            default_id = lcid_LC_PAPER;
            break;

        case LOCALE_IMEASURE:
            default_id = lcid_LC_MEASUREMENT;
            break;

        case LOCALE_ICOUNTRY:
            default_id = lcid_LC_TELEPHONE;
            break;
        }
        if (default_id) lcid = default_id;
    }
    return ConvertDefaultLocale( lcid );
}

/******************************************************************************
 *		get_locale_registry_value
 *
 * Gets the registry value name and cache for a given lctype.
 */
static struct registry_value *get_locale_registry_value( DWORD lctype )
{
    int i;
    for (i=0; i < sizeof(registry_values)/sizeof(registry_values[0]); i++)
        if (registry_values[i].lctype == lctype)
            return &registry_values[i];
    return NULL;
}

/***********************************************************************
 *		create_registry_key
 *
 * Create the Control Panel\\International registry key.
 */
static inline HANDLE create_registry_key(void)
{
    static const WCHAR cplW[] = {'C','o','n','t','r','o','l',' ','P','a','n','e','l',0};
    static const WCHAR intlW[] = {'I','n','t','e','r','n','a','t','i','o','n','a','l',0};
    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING nameW;
    HANDLE cpl_key, hkey = 0;

    if (RtlOpenCurrentUser( KEY_ALL_ACCESS, &hkey ) != STATUS_SUCCESS) return 0;

    attr.Length = sizeof(attr);
    attr.RootDirectory = hkey;
    attr.ObjectName = &nameW;
    attr.Attributes = 0;
    attr.SecurityDescriptor = NULL;
    attr.SecurityQualityOfService = NULL;
    RtlInitUnicodeString( &nameW, cplW );

    if (!NtCreateKey( &cpl_key, KEY_ALL_ACCESS, &attr, 0, NULL, 0, NULL ))
    {
        NtClose( attr.RootDirectory );
        attr.RootDirectory = cpl_key;
        RtlInitUnicodeString( &nameW, intlW );
        if (NtCreateKey( &hkey, KEY_ALL_ACCESS, &attr, 0, NULL, 0, NULL )) hkey = 0;
    }
    NtClose( attr.RootDirectory );
    return hkey;
}

/******************************************************************************
 *		get_registry_locale_info
 *
 * Retrieve user-modified locale info from the registry.
 * Return length, 0 on error, -1 if not found.
 */
static INT get_registry_locale_info( struct registry_value *registry_value, LPWSTR buffer, INT len )
{
    DWORD size;
    INT ret;
    HANDLE hkey;
    NTSTATUS status;
    UNICODE_STRING nameW;
    KEY_VALUE_PARTIAL_INFORMATION *info;
    static const int info_size = FIELD_OFFSET(KEY_VALUE_PARTIAL_INFORMATION, Data);

    RtlEnterCriticalSection( &cache_section );

    if (!registry_value->cached_value)
    {
        if (!(hkey = create_registry_key()))
        {
            RtlLeaveCriticalSection( &cache_section );
            return -1;
        }

        RtlInitUnicodeString( &nameW, registry_value->name );
        size = info_size + len * sizeof(WCHAR);

        if (!(info = HeapAlloc( GetProcessHeap(), 0, size )))
        {
            NtClose( hkey );
            SetLastError( ERROR_NOT_ENOUGH_MEMORY );
            RtlLeaveCriticalSection( &cache_section );
            return 0;
        }

        status = NtQueryValueKey( hkey, &nameW, KeyValuePartialInformation, info, size, &size );

        /* try again with a bigger buffer when we have to return the correct size */
        if (status == STATUS_BUFFER_OVERFLOW && !buffer && size > info_size)
        {
            KEY_VALUE_PARTIAL_INFORMATION *new_info;
            if ((new_info = HeapReAlloc( GetProcessHeap(), 0, info, size )))
            {
                info = new_info;
                status = NtQueryValueKey( hkey, &nameW, KeyValuePartialInformation, info, size, &size );
            }
        }

        NtClose( hkey );

        if (!status)
        {
            INT length = (size - info_size) / sizeof(WCHAR);
            LPWSTR cached_value;

            if (!length || ((WCHAR *)&info->Data)[length-1])
                length++;

            cached_value = HeapAlloc( GetProcessHeap(), 0, length * sizeof(WCHAR) );

            if (!cached_value)
            {
                HeapFree( GetProcessHeap(), 0, info );
                SetLastError( ERROR_NOT_ENOUGH_MEMORY );
                RtlLeaveCriticalSection( &cache_section );
                return 0;
            }

            memcpy( cached_value, info->Data, (length-1) * sizeof(WCHAR) );
            cached_value[length-1] = 0;
            HeapFree( GetProcessHeap(), 0, info );
            registry_value->cached_value = cached_value;
        }
        else
        {
            if (status == STATUS_BUFFER_OVERFLOW && !buffer)
            {
                ret = (size - info_size) / sizeof(WCHAR);
            }
            else if (status == STATUS_OBJECT_NAME_NOT_FOUND)
            {
                ret = -1;
            }
            else
            {
                SetLastError( RtlNtStatusToDosError(status) );
                ret = 0;
            }
            HeapFree( GetProcessHeap(), 0, info );
            RtlLeaveCriticalSection( &cache_section );
            return ret;
        }
    }

    ret = lstrlenW( registry_value->cached_value ) + 1;

    if (buffer)
    {
        if (ret > len)
        {
            SetLastError( ERROR_INSUFFICIENT_BUFFER );
            ret = 0;
        }
        else
        {
            lstrcpyW( buffer, registry_value->cached_value );
        }
    }

    RtlLeaveCriticalSection( &cache_section );

    return ret;
}

static int get_value_base_by_lctype( LCTYPE lctype )
{
    return lctype == LOCALE_ILANGUAGE || lctype == LOCALE_IDEFAULTLANGUAGE ? 16 : 10;
}

static LANGID get_default_sublang( LANGID lang )
{
    switch (lang)
    {
    case MAKELANGID( LANG_SPANISH, SUBLANG_NEUTRAL ):
        return MAKELANGID( LANG_SPANISH, SUBLANG_SPANISH_MODERN );
    case MAKELANGID( LANG_CHINESE, SUBLANG_NEUTRAL ):
        return MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED );
    case MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_SINGAPORE ):
        return MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED );
    case MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL ):
    case MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_MACAU ):
        return MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_HONGKONG );
    }
    if (SUBLANGID( lang ) == SUBLANG_NEUTRAL) lang = MAKELANGID( PRIMARYLANGID(lang), SUBLANG_DEFAULT );
    return lang;
}

/******************************************************************************
 *		GetLocaleInfoW (KERNEL32.@)
 *
 * See GetLocaleInfoA.
 */
INT WINAPI GetLocaleInfoInternalW( LCID lcid, LCTYPE lctype, LPWSTR buffer, INT len )
{
    LANGID lang_id;
    HRSRC hrsrc;
    HGLOBAL hmem;
    INT ret;
    UINT lcflags;
    const WCHAR *p;
    unsigned int i;

    if (len < 0 || (len && !buffer))
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return 0;
    }
    if (lctype & LOCALE_RETURN_GENITIVE_NAMES &&
       !is_genitive_name_supported( lctype ))
    {
        SetLastError( ERROR_INVALID_FLAGS );
        return 0;
    }

    if (!len) buffer = NULL;

    lcid = convert_default_lcid( lcid, lctype );

    lcflags = lctype & LOCALE_LOCALEINFOFLAGSMASK;
    lctype &= 0xffff;

    TRACE( "(lcid=0x%x,lctype=0x%x,%p,%d)\n", lcid, lctype, buffer, len );

    /* first check for overrides in the registry */

    if (!(lcflags & LOCALE_NOUSEROVERRIDE) &&
        lcid == convert_default_lcid( LOCALE_USER_DEFAULT, lctype ))
    {
        struct registry_value *value = get_locale_registry_value(lctype);

        if (value)
        {
            if (lcflags & LOCALE_RETURN_NUMBER)
            {
                WCHAR tmp[16];
                ret = get_registry_locale_info( value, tmp, sizeof(tmp)/sizeof(WCHAR) );
                if (ret > 0)
                {
                    WCHAR *end;
                    UINT number = strtolW( tmp, &end, get_value_base_by_lctype( lctype ) );
                    if (*end)  /* invalid number */
                    {
                        SetLastError( ERROR_INVALID_FLAGS );
                        return 0;
                    }
                    ret = sizeof(UINT)/sizeof(WCHAR);
                    if (!buffer) return ret;
                    if (ret > len)
                    {
                        SetLastError( ERROR_INSUFFICIENT_BUFFER );
                        return 0;
                    }
                    memcpy( buffer, &number, sizeof(number) );
                }
            }
            else ret = get_registry_locale_info( value, buffer, len );

            if (ret != -1) return ret;
        }
    }

    /* now load it from kernel resources */

    lang_id = LANGIDFROMLCID( lcid );

    /* replace SUBLANG_NEUTRAL by SUBLANG_DEFAULT */
    if (SUBLANGID(lang_id) == SUBLANG_NEUTRAL) lang_id = get_default_sublang( lang_id );

    if (!(hrsrc = FindResourceExW( kernel32_handle, (LPWSTR)RT_STRING,
                                   ULongToPtr((lctype >> 4) + 1), lang_id )))
    {
        SetLastError( ERROR_INVALID_FLAGS );  /* no such lctype */
        return 0;
    }
    if (!(hmem = LoadResource( kernel32_handle, hrsrc )))
        return 0;

    p = LockResource( hmem );
    for (i = 0; i < (lctype & 0x0f); i++) p += *p + 1;

    if (lcflags & LOCALE_RETURN_NUMBER) ret = sizeof(UINT)/sizeof(WCHAR);
    else if (is_genitive_name_supported( lctype ) && *p)
    {
        /* genitive form's stored after a null separator from a nominative */
        for (i = 1; i <= *p; i++) if (!p[i]) break;

        if (i <= *p && (lcflags & LOCALE_RETURN_GENITIVE_NAMES))
        {
            ret = *p - i + 1;
            p += i;
        }
        else ret = i;
    }
    else
        ret = (lctype == LOCALE_FONTSIGNATURE) ? *p : *p + 1;

    if (!buffer) return ret;

    if (ret > len)
    {
        SetLastError( ERROR_INSUFFICIENT_BUFFER );
        return 0;
    }

    if (lcflags & LOCALE_RETURN_NUMBER)
    {
        UINT number;
        WCHAR *end, *tmp = HeapAlloc( GetProcessHeap(), 0, (*p + 1) * sizeof(WCHAR) );
        if (!tmp) return 0;
        memcpy( tmp, p + 1, *p * sizeof(WCHAR) );
        tmp[*p] = 0;
        number = strtolW( tmp, &end, get_value_base_by_lctype( lctype ) );
        if (!*end)
            memcpy( buffer, &number, sizeof(number) );
        else  /* invalid number */
        {
            SetLastError( ERROR_INVALID_FLAGS );
            ret = 0;
        }
        HeapFree( GetProcessHeap(), 0, tmp );

        TRACE( "(lcid=0x%x,lctype=0x%x,%p,%d) returning number %d\n",
               lcid, lctype, buffer, len, number );
    }
    else
    {
        memcpy( buffer, p + 1, ret * sizeof(WCHAR) );
        if (lctype != LOCALE_FONTSIGNATURE) buffer[ret-1] = 0;

        TRACE( "(lcid=0x%x,lctype=0x%x,%p,%d) returning %d %s\n",
               lcid, lctype, buffer, len, ret, debugstr_w(buffer) );
    }
    return ret;
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

//TODO MUI_LANGUAGE_ID
BOOL
WINAPI
EnumPreferredUserUILanguages(
  _In_      DWORD   flags,
  _In_		LANGID langid,
  _Out_     PULONG  count,
  _Out_opt_ PZZWSTR buffer,
  _Inout_   PULONG  buffersize 
)
{
    static const WCHAR formathexW[] = { '%','0','4','x',0 };

    static const WCHAR formatstringW[] = { '%','.','2','s',0 };
	

    FIXME( "semi-stub %u, %p, %p %p\n", flags, count, buffer, buffersize );

    if (!flags)
        flags = MUI_LANGUAGE_NAME;

	if ((flags & (MUI_LANGUAGE_ID | MUI_LANGUAGE_NAME )) == (MUI_LANGUAGE_ID | MUI_LANGUAGE_NAME ))
    {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;

    }
    /* FIXME should we check for too small buffersize too? */
    if (!buffer)
    {
           SetLastError(ERROR_INSUFFICIENT_BUFFER);
           *buffersize = 10;
           *count=2;
           return TRUE;
    }
	
    memset((WCHAR *)buffer,0,*buffersize);
    if ((flags & MUI_LANGUAGE_ID) == MUI_LANGUAGE_ID)  
    { 
           *buffersize = 11; 
           *count=2;
           sprintfW((WCHAR *)buffer, formathexW, langid);
           sprintfW((WCHAR *)buffer+5, formathexW, PRIMARYLANGID(langid)); 
           SetLastError(ERROR_SUCCESS);
    }
    else  
    {
           *buffersize = 10; 
           *count=2;
           //GetLocaleInfoW( MAKELCID(langid, SORT_DEFAULT), LOCALE_SNAME | LOCALE_NOUSEROVERRIDE, (WCHAR *)buffer, *buffersize);
		   LCIDToLocaleName(MAKELCID(langid, SORT_DEFAULT), (WCHAR *)buffer, *buffersize, 0);
           /* FIXME is there no better way to to this? I can't get GetLocaleInfo to return the neutral languagename :( */      
           sprintfW((WCHAR *)buffer+6, formatstringW, buffer);
           SetLastError(ERROR_SUCCESS);

    }
    return TRUE; 	
}

BOOL
WINAPI
EnumPreferredThreadUILanguages(
  _In_      DWORD   flags,
  _In_		LANGID	langid,
  _Out_     PULONG  count,
  _Out_opt_ PZZWSTR buffer,
  _Inout_   PULONG  buffersize 
)
{
    static const WCHAR formathexW[] = { '%','0','4','x',0 };

    static const WCHAR formatstringW[] = { '%','.','2','s',0 };
	

    FIXME( "EnumPreferredThreadUILanguages :: semi-stub %u, %p, %p %p\n", flags, count, buffer, buffersize );

    if (!flags)
        flags = MUI_LANGUAGE_NAME;

	if ((flags & (MUI_LANGUAGE_ID | MUI_LANGUAGE_NAME )) == (MUI_LANGUAGE_ID | MUI_LANGUAGE_NAME ))
    {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;

    }
    /* FIXME should we check for too small buffersize too? */
    if (!buffer)
    {
           SetLastError(ERROR_INSUFFICIENT_BUFFER);
           *buffersize = 10;
           *count=2;
           return TRUE;
    }

    memset((WCHAR *)buffer,0,*buffersize);
    if ((flags & MUI_LANGUAGE_ID) == MUI_LANGUAGE_ID)  
    { 
           *buffersize = 11; 
           *count=2;
           sprintfW((WCHAR *)buffer, formathexW, langid);
           sprintfW((WCHAR *)buffer+5, formathexW, PRIMARYLANGID(langid)); 
           SetLastError(ERROR_SUCCESS);
    }
    else  
    {
           *buffersize = 10; 
           *count=2;
           //GetLocaleInfoW( MAKELCID(langid, SORT_DEFAULT), LOCALE_SNAME | LOCALE_NOUSEROVERRIDE, (WCHAR *)buffer, *buffersize);
		   LCIDToLocaleName(MAKELCID(langid, SORT_DEFAULT), (WCHAR *)buffer, *buffersize, 0);
           /* FIXME is there no better way to to this? I can't get GetLocaleInfo to return the neutral languagename :( */      
           sprintfW((WCHAR *)buffer+6, formatstringW, buffer);
           SetLastError(ERROR_SUCCESS);

    }
    return TRUE; 	
}

BOOL
WINAPI
EnumPreferredSystemUILanguages(
  _In_      DWORD   flags,
  _Out_     PULONG  count,
  _Out_opt_ PZZWSTR buffer,
  _Inout_   PULONG  buffersize 
)
{
    static const WCHAR formathexW[] = { '%','0','4','x',0 };

    static const WCHAR formatstringW[] = { '%','.','2','s',0 };
	

    LANGID langid;

    FIXME( "semi-stub %u, %p, %p %p\n", flags, count, buffer, buffersize );

    if (!flags)
        flags = MUI_LANGUAGE_NAME;

	if ((flags & (MUI_LANGUAGE_ID | MUI_LANGUAGE_NAME )) == (MUI_LANGUAGE_ID | MUI_LANGUAGE_NAME ))
    {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;

    }
    /* FIXME should we check for too small buffersize too? */
    if (!buffer)
    {
           SetLastError(ERROR_INSUFFICIENT_BUFFER);
           *buffersize = 10;
           *count=2;
           return TRUE;
    }

    langid = GetSystemDefaultLangID();
    memset((WCHAR *)buffer,0,*buffersize);
    if ((flags & MUI_LANGUAGE_ID) == MUI_LANGUAGE_ID)  
    { 
           *buffersize = 11; 
           *count=2;
           sprintfW((WCHAR *)buffer, formathexW, langid);
           sprintfW((WCHAR *)buffer+5, formathexW, PRIMARYLANGID(langid)); 
           SetLastError(ERROR_SUCCESS);
    }
    else  
    {
           *buffersize = 10; 
           *count=2;
           //GetLocaleInfoW( MAKELCID(langid, SORT_DEFAULT), LOCALE_SNAME | LOCALE_NOUSEROVERRIDE, (WCHAR *)buffer, *buffersize);
		   LCIDToLocaleName(MAKELCID(langid, SORT_DEFAULT), (WCHAR *)buffer, *buffersize, 0);
           /* FIXME is there no better way to to this? I can't get GetLocaleInfo to return the neutral languagename :( */      
           sprintfW((WCHAR *)buffer+6, formatstringW, buffer);
           SetLastError(ERROR_SUCCESS);

    }
    return TRUE; 	
}

BOOL
WINAPI
EnumPreferredProcessUILanguages(
  _In_      DWORD   flags,
  _Out_     PULONG  count,
  _Out_opt_ PZZWSTR buffer,
  _Inout_   PULONG  buffersize 
)
{
    static const WCHAR formathexW[] = { '%','0','4','x',0 };

    static const WCHAR formatstringW[] = { '%','.','2','s',0 };
	

    LANGID langid;

    FIXME( "semi-stub %u, %p, %p %p\n", flags, count, buffer, buffersize );

    if (!flags)
        flags = MUI_LANGUAGE_NAME;

	if ((flags & (MUI_LANGUAGE_ID | MUI_LANGUAGE_NAME )) == (MUI_LANGUAGE_ID | MUI_LANGUAGE_NAME ))
    {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;

    }
    /* FIXME should we check for too small buffersize too? */
    if (!buffer)
    {
           SetLastError(ERROR_INSUFFICIENT_BUFFER);
           *buffersize = 10;
           *count=2;
           return TRUE;
    }

    langid = GetSystemDefaultLangID();
    memset((WCHAR *)buffer,0,*buffersize);
    if ((flags & MUI_LANGUAGE_ID) == MUI_LANGUAGE_ID)  
    { 
           *buffersize = 11; 
           *count=2;
           sprintfW((WCHAR *)buffer, formathexW, langid);
           sprintfW((WCHAR *)buffer+5, formathexW, PRIMARYLANGID(langid)); 
           SetLastError(ERROR_SUCCESS);
    }
    else  
    {
           *buffersize = 10; 
           *count=2;
           //GetLocaleInfoW( MAKELCID(langid, SORT_DEFAULT), LOCALE_SNAME | LOCALE_NOUSEROVERRIDE, (WCHAR *)buffer, *buffersize);
		   LCIDToLocaleName(MAKELCID(langid, SORT_DEFAULT), (WCHAR *)buffer, *buffersize, 0);
           /* FIXME is there no better way to to this? I can't get GetLocaleInfo to return the neutral languagename :( */      
           sprintfW((WCHAR *)buffer+6, formatstringW, buffer);
           SetLastError(ERROR_SUCCESS);

    }
    return TRUE; 	
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
    return LCIDToLocaleName(GetSystemDefaultLCID(), localename, len, 0);
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
     //LANGID lang;
     //NtQueryDefaultUILanguage( &lang );
     //DbgPrint("GetThreadUILanguage is UNIMPLEMENTED, returning default language.\n");
	 //Windows XP and Server 2003 doesn't use really LANGIID passed how paremeter on SetThreadUILanguage, so, we 
	 //can use to get Thread UI Language;	 
     //return SetThreadUILanguage(0);
    LANGID lang;

    FIXME(": stub, returning default language.\n");
    NtQueryDefaultUILanguage( &lang );
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
	
	//DbgPrint("LCMapStringEx called\n");		

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

        // DbgPrint("(%s,0x%08x,%s,%d,%p,%d)\n",
              // debugstr_w(name), flags, debugstr_wn(src, srclen), srclen, dst, dstlen);

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

    // DbgPrint("(%s,0x%08x,%s,%d,%p,%d)\n",
          // debugstr_w(name), flags, debugstr_wn(src, srclen), srclen, dst, dstlen);

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
	int ret;
	
    ret = LCIDToLocaleName(GetUserDefaultLCID(), localename, buffersize, 0);
	
	DbgPrint("GetUserDefaultLocaleName :: Default Locale: %s\n", localename);
	
	DbgPrint("GetUserDefaultLocaleName :: Return value: %d\n", ret);
	
	return ret;
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

/* retrieve the resource name to pass to the ntdll functions */
static NTSTATUS get_res_nameW( LPCWSTR name, UNICODE_STRING *str )
{
    if (IS_INTRESOURCE(name))
    {
        str->Buffer = ULongToPtr(LOWORD(name));
        return STATUS_SUCCESS;
    }
    if (name[0] == '#')
    {
        ULONG value;
        RtlInitUnicodeString( str, name + 1 );
        if (RtlUnicodeStringToInteger( str, 10, &value ) != STATUS_SUCCESS || HIWORD(value))
            return STATUS_INVALID_PARAMETER;
        str->Buffer = ULongToPtr(value);
        return STATUS_SUCCESS;
    }
    RtlCreateUnicodeString( str, name );
    RtlUpcaseUnicodeString( str, str, FALSE );
    return STATUS_SUCCESS;
}

/* retrieve the resource name to pass to the ntdll functions */
static NTSTATUS get_res_nameA( LPCSTR name, UNICODE_STRING *str )
{
    if (IS_INTRESOURCE(name))
    {
        str->Buffer = ULongToPtr(LOWORD(name));
        return STATUS_SUCCESS;
    }
    if (name[0] == '#')
    {
        ULONG value;
        if (RtlCharToInteger( name + 1, 10, &value ) != STATUS_SUCCESS || HIWORD(value))
            return STATUS_INVALID_PARAMETER;
        str->Buffer = ULongToPtr(value);
        return STATUS_SUCCESS;
    }
    RtlCreateUnicodeStringFromAsciiz( str, name );
    RtlUpcaseUnicodeString( str, str, FALSE );
    return STATUS_SUCCESS;
}

/**********************************************************************
 *	EnumResourceLanguagesExA	(KERNEL32.@)
 */
BOOL WINAPI EnumResourceLanguagesExA( HMODULE hmod, LPCSTR type, LPCSTR name,
                                      ENUMRESLANGPROCA lpfun, LONG_PTR lparam,
                                      DWORD flags, LANGID lang )
{
    int i;
    BOOL ret = FALSE;
    NTSTATUS status;
    UNICODE_STRING typeW, nameW;
    LDR_RESOURCE_INFO info;
    const IMAGE_RESOURCE_DIRECTORY *basedir, *resdir;
    const IMAGE_RESOURCE_DIRECTORY_ENTRY *et;

    TRACE( "%p %s %s %p %lx %x %d\n", hmod, debugstr_a(type), debugstr_a(name),
           lpfun, lparam, flags, lang );

    if (flags & (RESOURCE_ENUM_MUI | RESOURCE_ENUM_MUI_SYSTEM | RESOURCE_ENUM_VALIDATE))
        FIXME( "unimplemented flags: %x\n", flags );

    if (!flags) flags = RESOURCE_ENUM_LN | RESOURCE_ENUM_MUI;

    if (!(flags & RESOURCE_ENUM_LN)) return ret;

    if (!hmod) hmod = GetModuleHandleA( NULL );
    typeW.Buffer = nameW.Buffer = NULL;
    if ((status = LdrFindResourceDirectory_U( hmod, (const LDR_RESOURCE_INFO *)NULL, 0, &basedir )) != STATUS_SUCCESS)
        goto done;
    if ((status = get_res_nameA( type, &typeW )) != STATUS_SUCCESS)
        goto done;
    if ((status = get_res_nameA( name, &nameW )) != STATUS_SUCCESS)
        goto done;
    info.Type = (ULONG_PTR)typeW.Buffer;
    info.Name = (ULONG_PTR)nameW.Buffer;
    if ((status = LdrFindResourceDirectory_U( hmod, &info, 2, &resdir )) != STATUS_SUCCESS)
        goto done;

    et = (const IMAGE_RESOURCE_DIRECTORY_ENTRY *)(resdir + 1);
    _SEH2_TRY
    {
        for (i = 0; i < resdir->NumberOfNamedEntries + resdir->NumberOfIdEntries; i++)
        {
            ret = lpfun( hmod, type, name, et[i].Id, lparam );
            if (!ret) break;
        }
    }
    _SEH2_EXCEPT(UnhandledExceptionFilter(_SEH2_GetExceptionInformation()))
    {
        ret = FALSE;
        status = STATUS_ACCESS_VIOLATION;
    }
    _SEH2_END
done:
    if (!IS_INTRESOURCE(typeW.Buffer)) HeapFree( GetProcessHeap(), 0, typeW.Buffer );
    if (!IS_INTRESOURCE(nameW.Buffer)) HeapFree( GetProcessHeap(), 0, nameW.Buffer );
    if (status != STATUS_SUCCESS) SetLastError( RtlNtStatusToDosError(status) );
    return ret;
}

/**********************************************************************
 *	EnumResourceLanguagesExW	(KERNEL32.@)
 */
BOOL WINAPI EnumResourceLanguagesExW( 
	HMODULE hmod, 
	LPCWSTR type, 
	LPCWSTR name,
    ENUMRESLANGPROCW lpfun, 
	LONG_PTR lparam,
    DWORD flags, 
	LANGID lang 
)
{
    int i;
    BOOL ret = FALSE;
    NTSTATUS status;
    UNICODE_STRING typeW, nameW;
    LDR_RESOURCE_INFO info;
    const IMAGE_RESOURCE_DIRECTORY *basedir, *resdir;
    const IMAGE_RESOURCE_DIRECTORY_ENTRY *et;

    TRACE( "%p %s %s %p %lx %x %d\n", hmod, debugstr_w(type), debugstr_w(name),
           lpfun, lparam, flags, lang );

    if (flags & (RESOURCE_ENUM_MUI | RESOURCE_ENUM_MUI_SYSTEM | RESOURCE_ENUM_VALIDATE))
        FIXME( "unimplemented flags: %x\n", flags );

    if (!flags) flags = RESOURCE_ENUM_LN | RESOURCE_ENUM_MUI;

    if (!(flags & RESOURCE_ENUM_LN)) return ret;

    if (!hmod) hmod = GetModuleHandleW( NULL );
    typeW.Buffer = nameW.Buffer = NULL;
    if ((status = LdrFindResourceDirectory_U( hmod, NULL, 0, &basedir )) != STATUS_SUCCESS)
        goto done;
    if ((status = get_res_nameW( type, &typeW )) != STATUS_SUCCESS)
        goto done;
    if ((status = get_res_nameW( name, &nameW )) != STATUS_SUCCESS)
        goto done;
    info.Type = (ULONG_PTR)typeW.Buffer;
    info.Name = (ULONG_PTR)nameW.Buffer;
    if ((status = LdrFindResourceDirectory_U( hmod, &info, 2, &resdir )) != STATUS_SUCCESS)
        goto done;

    et = (const IMAGE_RESOURCE_DIRECTORY_ENTRY *)(resdir + 1);
    _SEH2_TRY
    {
        for (i = 0; i < resdir->NumberOfNamedEntries + resdir->NumberOfIdEntries; i++)
        {
            ret = lpfun( hmod, type, name, et[i].Id, lparam );
            if (!ret) break;
        }
    }
     _SEH2_EXCEPT(UnhandledExceptionFilter(_SEH2_GetExceptionInformation()))
    {
        ret = FALSE;
        status = STATUS_ACCESS_VIOLATION;
    }
    _SEH2_END
done:
    if (!IS_INTRESOURCE(typeW.Buffer)) HeapFree( GetProcessHeap(), 0, typeW.Buffer );
    if (!IS_INTRESOURCE(nameW.Buffer)) HeapFree( GetProcessHeap(), 0, nameW.Buffer );
    if (status != STATUS_SUCCESS) SetLastError( RtlNtStatusToDosError(status) );
    return ret;
}

BOOL CALLBACK EnumLocalesProc(
  _In_ LPTSTR lpLocaleString
)
{
	DbgPrint("EnumLocalesProc called\n");
	
	if(ARGUMENT_PRESENT(lpLocaleString))
	{
		DbgPrint("EnumLocalesProc:: lpLocaleString is %s\n", lpLocaleString);
		LCIDToLocaleName((LCID)lpLocaleString, systemLocale, 0, 0);
		return TRUE;
	}	
	return FALSE;
}

/******************************************************************************
 *	EnumSystemLocalesEx   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH EnumSystemLocalesEx( LOCALE_ENUMPROCEX proc, DWORD wanted_flags,
                                                   LPARAM param, void *reserved )
{
    WCHAR buffer[256], name[10];
    DWORD name_len, type, neutral, flags, index = 0, alt = 0;
    HKEY key, altkey;
    LCID lcid;

    if (reserved)
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }

    if (RegOpenKeyExW( nls_key, L"Locale", 0, KEY_READ, &key )) return FALSE;
    if (RegOpenKeyExW( key, L"Alternate Sorts", 0, KEY_READ, &altkey )) altkey = 0;

    for (;;)
    {
        name_len = ARRAY_SIZE(name);
        if (RegEnumValueW( alt ? altkey : key, index++, name, &name_len, NULL, &type, NULL, NULL ))
        {
            if (alt++) break;
            index = 0;
            continue;
        }
        if (type != REG_SZ) continue;
        if (!(lcid = wcstoul( name, NULL, 16 ))) continue;

        GetLocaleInfoW( lcid, LOCALE_SNAME | LOCALE_NOUSEROVERRIDE, buffer, ARRAY_SIZE( buffer ));
        if (!GetLocaleInfoW( lcid, LOCALE_INEUTRAL | LOCALE_NOUSEROVERRIDE | LOCALE_RETURN_NUMBER,
                             (LPWSTR)&neutral, sizeof(neutral) / sizeof(WCHAR) ))
            neutral = 0;

        if (alt)
            flags = LOCALE_ALTERNATE_SORTS;
        else
            flags = LOCALE_WINDOWS | (neutral ? LOCALE_NEUTRALDATA : LOCALE_SPECIFICDATA);

        if (wanted_flags && !(flags & wanted_flags)) continue;
        if (!proc( buffer, flags, param )) break;
    }
    RegCloseKey( altkey );
    RegCloseKey( key );
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

    TRACE("%s, lcid=0x%x, 0x%x\n", debugstr_w(locale), lcid, info);

    if (!lcid) return 0;

    /* special handling for neutral locale names */
    if (locale && strlenW(locale) == 2)
    {
        switch (info)
        {
        case LOCALE_SNAME:
            if (len && len < 3)
            {
                SetLastError(ERROR_INSUFFICIENT_BUFFER);
                return 0;
            }
            if (len) strcpyW(buffer, locale);
            return 3;
        case LOCALE_SPARENT:
            if (len) buffer[0] = 0;
            return 1;
        }
    }

    return GetpLocaleInfoW(lcid, info, buffer, len);
}

/******************************************************************************
 *	GetNLSVersionEx   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH GetNLSVersionEx( NLS_FUNCTION func, const WCHAR *locale,
                                               NLSVERSIONINFOEX *info )
{
    LCID lcid = 0;

    if (func != COMPARE_STRING)
    {
        SetLastError( ERROR_INVALID_FLAGS );
        return FALSE;
    }
    if (info->dwNLSVersionInfoSize < sizeof(*info) &&
        (info->dwNLSVersionInfoSize != offsetof( NLSVERSIONINFO, dwNLSVersionInfoSize )))
    {
        SetLastError( ERROR_INSUFFICIENT_BUFFER );
        return FALSE;
    }

    if (!(lcid = LocaleNameToLCID( locale, 0 ))) return FALSE;

    info->dwNLSVersion = info->dwDefinedVersion = sort.version;
    if (info->dwNLSVersionInfoSize >= sizeof(*info))
    {
        const struct sortguid *sortid = get_language_sort( locale );
        info->dwEffectiveId = lcid;
        info->guidCustomVersion = sortid ? sortid->id : default_sort_guid;
    }
    return TRUE;
}

BOOL 
WINAPI 
GetNLSVersion(
    NLS_FUNCTION     func,
    LCID             lcid,
    LPNLSVERSIONINFO info)
{
    WCHAR locale[LOCALE_NAME_MAX_LENGTH];	
	
    if (info->dwNLSVersionInfoSize < offsetof( NLSVERSIONINFO, dwNLSVersionInfoSize ))
    {
        SetLastError( ERROR_INSUFFICIENT_BUFFER );
        return FALSE;
    }
    if (!LCIDToLocaleName( lcid, locale, LOCALE_NAME_MAX_LENGTH, LOCALE_ALLOW_NEUTRAL_NAMES ))
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }
    return GetNLSVersionEx( func, locale, (NLSVERSIONINFOEX *)info );
}

INT 
WINAPI 
FindNLSStringEx(
	const WCHAR *localename, 
	DWORD flags, 
	const WCHAR *src,
    INT src_size, 
	const WCHAR *value, 
	INT value_size,
    INT *found, 
	NLSVERSIONINFO *version_info, 
	void *reserved,
    LPARAM sort_handle
);

int 
WINAPI 
FindNLSString(
  _In_ 		 LCID Locale,
  _In_       DWORD dwFindNLSStringFlags,
  _In_       LPCWSTR lpStringSource,
  _In_       int cchSource,
  _In_       LPCWSTR lpStringValue,
  _In_       int cchValue,
  _Out_opt_  LPINT pcchFound
)
{
	const WCHAR localename;
	
	LCIDToLocaleName(Locale, &localename, MAX_STRING_LEN, 0);
	return FindNLSStringEx(&localename,
						   dwFindNLSStringFlags,
						   lpStringSource,
						   cchSource,
						   lpStringValue,
						   cchValue,
						   pcchFound,
						   NULL,
						   NULL,
						   0);
}

/******************************************************************************
 *           FindNLSStringEx (KERNEL32.@)
 */

INT 
WINAPI 
FindNLSStringEx(
	const WCHAR *localename, 
	DWORD flags, 
	const WCHAR *src,
    INT src_size, 
	const WCHAR *value, 
	INT value_size,
    INT *found, 
	NLSVERSIONINFO *version_info, 
	void *reserved,
    LPARAM sort_handle
)
{

    /* FIXME: this function should normalize strings before calling CompareStringEx() */
    DWORD mask = flags;
    int offset, inc, count;

    TRACE("%s %x %s %d %s %d %p %p %p %ld\n", wine_dbgstr_w(localename), flags,
          wine_dbgstr_w(src), src_size, wine_dbgstr_w(value), value_size, found,
          version_info, reserved, sort_handle);

    if (version_info != NULL || reserved != NULL || sort_handle != 0 ||
        !IsValidLocaleName(localename) || src == NULL || src_size == 0 ||
        src_size < -1 || value == NULL || value_size == 0 || value_size < -1)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return -1;
    }
    if (src_size == -1)
        src_size = strlenW(src);
    if (value_size == -1)
        value_size = strlenW(value);

    src_size -= value_size;
    if (src_size < 0) return -1;

    mask = flags & ~(FIND_FROMSTART | FIND_FROMEND | FIND_STARTSWITH | FIND_ENDSWITH);
    count = flags & (FIND_FROMSTART | FIND_FROMEND) ? src_size + 1 : 1;
    offset = flags & (FIND_FROMSTART | FIND_STARTSWITH) ? 0 : src_size;
    inc = flags & (FIND_FROMSTART | FIND_STARTSWITH) ? 1 : -1;
    while (count--)
    {
        if (CompareStringEx(localename, mask, src + offset, value_size, value, value_size, NULL, NULL, 0) == CSTR_EQUAL)
        {
            if (found)
                *found = value_size;
            return offset;
        }
        offset += inc;
    }

    return -1;
}
/******************************************************************************
 *           ResolveLocaleName (KERNEL32.@)
 */

INT WINAPI ResolveLocaleName(LPCWSTR name, LPWSTR localename, INT len)
{
    FIXME("stub: %s, %p, %d\n", wine_dbgstr_w(name), localename, len);

    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return 0;
}

/******************************************************************************
 *             GetUserPreferredUILanguages (KERNEL32.@)
 */
BOOL 
WINAPI 
GetUserPreferredUILanguages( 
  _In_      DWORD   dwFlags,
  _Out_     PULONG  pulNumLanguages,
  _Out_opt_ PZZWSTR pwszLanguagesBuffer,
  _Inout_   PULONG  pcchLanguagesBuffer
)
{
	LANGID ui_language;
	
	NtQueryDefaultUILanguage( &ui_language );	
	return EnumPreferredUserUILanguages(dwFlags,
										ui_language,
									    pulNumLanguages,
									    pwszLanguagesBuffer,
									    pcchLanguagesBuffer);
}

/***********************************************************************
 *      GetThreadPreferredUILanguages   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH GetThreadPreferredUILanguages( DWORD flags, ULONG *count,
                                                             WCHAR *buffer, ULONG *size )
{
	LANGID ui_language;
	
	NtQueryDefaultUILanguage( &ui_language );
    //return set_ntstatus( RtlGetThreadPreferredUILanguages( flags, count, buffer, size ));
	return EnumPreferredThreadUILanguages(flags,
										  ui_language,
										  count,
										  buffer,
										  size);	
}

/***********************************************************************
 *      GetSystemPreferredUILanguages   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH GetSystemPreferredUILanguages( DWORD flags, ULONG *count,
                                                             WCHAR *buffer, ULONG *size )
{
	LANGID ui_language;
	
	NtQueryInstallUILanguage( &ui_language );	
    //return set_ntstatus( RtlGetSystemPreferredUILanguages( flags, 0, count, buffer, size ));
	return EnumPreferredThreadUILanguages(flags,
										  ui_language,
										  count,
										  buffer,
										  size);		
}

/***********************************************************************
 *      GetProcessPreferredUILanguages   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH GetProcessPreferredUILanguages( DWORD flags, ULONG *count,
                                                              WCHAR *buffer, ULONG *size )
{
    return set_ntstatus( RtlGetProcessPreferredUILanguages( flags, count, buffer, size ));
}

/***********************************************************************
 *      SetThreadPreferredUILanguages   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH SetThreadPreferredUILanguages( DWORD flags, PCZZWSTR buffer, ULONG *count )
{
    return set_ntstatus( RtlSetThreadPreferredUILanguages( flags, buffer, count ));
}

/***********************************************************************
 *      SetProcessPreferredUILanguages   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH SetProcessPreferredUILanguages( DWORD flags, PCZZWSTR buffer, ULONG *count )
{
    return set_ntstatus( RtlSetProcessPreferredUILanguages( flags, buffer, count ));
}

/******************************************************************************
  *           GetFileMUIInfo (KERNEL32.@)
  */

BOOL WINAPI GetFileMUIInfo(DWORD flags, PCWSTR path, FILEMUIINFO *info, DWORD *size)
{
    FIXME("stub: %u, %s, %p, %p\n", flags, debugstr_w(path), info, size);
 
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}


/******************************************************************************
 *	IsValidNLSVersion   (kernelbase.@)
 */
DWORD WINAPI DECLSPEC_HOTPATCH IsValidNLSVersion( NLS_FUNCTION func, const WCHAR *locale,
                                                  NLSVERSIONINFOEX *info )
{
    static const GUID GUID_NULL;
    NLSVERSIONINFOEX infoex;
    DWORD ret;

    if (func != COMPARE_STRING)
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }
    if (info->dwNLSVersionInfoSize < sizeof(*info) &&
        (info->dwNLSVersionInfoSize != offsetof( NLSVERSIONINFOEX, dwEffectiveId )))
    {
        SetLastError( ERROR_INVALID_PARAMETER );
        return FALSE;
    }
    infoex.dwNLSVersionInfoSize = sizeof(infoex);
    if (!GetNLSVersionEx( func, locale, &infoex )) return FALSE;

    ret = (infoex.dwNLSVersion & ~0xff) == (info->dwNLSVersion & ~0xff);
    if (ret && !IsEqualGUID( &info->guidCustomVersion, &GUID_NULL ))
        ret = find_sortguid( &info->guidCustomVersion ) != NULL;

    if (!ret) SetLastError( ERROR_SUCCESS );
    return ret;
}

/******************************************************************************
 *	IdnToAscii   (kernelbase.@)
 */
INT WINAPI DECLSPEC_HOTPATCH IdnToAscii( DWORD flags, const WCHAR *src, INT srclen,
                                         WCHAR *dst, INT dstlen )
{
    NTSTATUS status = RtlIdnToAscii( flags, src, srclen, dst, &dstlen );
    if (!NT_SUCCESS( status )){
		SetLastError( ERROR_SUCCESS );
		return 0;
	}		
    return dstlen;
}

/******************************************************************************
 *	IdnToNameprepUnicode   (kernelbase.@)
 */
INT WINAPI DECLSPEC_HOTPATCH IdnToNameprepUnicode( DWORD flags, const WCHAR *src, INT srclen,
                                                   WCHAR *dst, INT dstlen )
{
    NTSTATUS status = RtlIdnToNameprepUnicode( flags, src, srclen, dst, &dstlen );
    if (!NT_SUCCESS( status )){
		SetLastError( ERROR_SUCCESS );
		return 0;
	}		
    return dstlen;
}

/******************************************************************************
 *	IdnToUnicode   (kernelbase.@)
 */
INT WINAPI DECLSPEC_HOTPATCH IdnToUnicode( DWORD flags, const WCHAR *src, INT srclen,
                                           WCHAR *dst, INT dstlen )
{
    NTSTATUS status = RtlIdnToUnicode( flags, src, srclen, dst, &dstlen );
    if (!NT_SUCCESS( status )){
		SetLastError( ERROR_SUCCESS );
		return 0;
	}		
    return dstlen;
}

/******************************************************************************
 *	IsNormalizedString   (kernelbase.@)
 */
BOOL WINAPI DECLSPEC_HOTPATCH IsNormalizedString( NORM_FORM form, const WCHAR *str, INT len )
{
    BOOLEAN res = TRUE;
    if (!NT_SUCCESS(RtlIsNormalizedString( form, str, len, &res ))){
		SetLastError( ERROR_SUCCESS );
		res = FALSE;
	}		
    return res;
}

/******************************************************************************
 *	NormalizeString   (kernelbase.@)
 */
INT WINAPI DECLSPEC_HOTPATCH NormalizeString(NORM_FORM form, const WCHAR *src, INT src_len,
                                             WCHAR *dst, INT dst_len)
{
    NTSTATUS status = RtlNormalizeString( form, src, src_len, dst, &dst_len );

    switch (status)
    {
    case STATUS_OBJECT_NAME_NOT_FOUND:
        status = STATUS_INVALID_PARAMETER;
        break;
    case STATUS_BUFFER_TOO_SMALL:
    case STATUS_NO_UNICODE_TRANSLATION:
        dst_len = -dst_len;
        break;
    }
    SetLastError( RtlNtStatusToDosError( status ));
    return dst_len;
}

/******************************************************************************
 *	EnumDynamicTimeZoneInformation   (kernelbase.@)
 */
DWORD WINAPI DECLSPEC_HOTPATCH EnumDynamicTimeZoneInformation( DWORD index,
                                                               DYNAMIC_TIME_ZONE_INFORMATION *info )
{
    DYNAMIC_TIME_ZONE_INFORMATION tz;
    LSTATUS ret;
    DWORD size;

    if (!info) return ERROR_INVALID_PARAMETER;

    size = ARRAY_SIZE(tz.TimeZoneKeyName);
    ret = RegEnumKeyExW( tz_key, index, tz.TimeZoneKeyName, &size, NULL, NULL, NULL, NULL );
    if (ret) return ret;

    tz.DynamicDaylightTimeDisabled = TRUE;
    if (!GetTimeZoneInformationForYear( 0, &tz, (TIME_ZONE_INFORMATION *)info )) return GetLastError();

    lstrcpyW( info->TimeZoneKeyName, tz.TimeZoneKeyName );
    info->DynamicDaylightTimeDisabled = FALSE;
    return 0;
}

/******************************************************************************
 *	GetDynamicTimeZoneInformationEffectiveYears   (kernelbase.@)
 */
DWORD WINAPI DECLSPEC_HOTPATCH GetDynamicTimeZoneInformationEffectiveYears( const DYNAMIC_TIME_ZONE_INFORMATION *info,
                                                                            DWORD *first, DWORD *last )
{
    HKEY key, dst_key = 0;
    DWORD type, count, ret = ERROR_FILE_NOT_FOUND;

    if (RegOpenKeyExW( tz_key, info->TimeZoneKeyName, 0, KEY_ALL_ACCESS, &key )) return ret;

    if (RegOpenKeyExW( key, L"Dynamic DST", 0, KEY_ALL_ACCESS, &dst_key )) goto done;
    count = sizeof(DWORD);
    if (RegQueryValueExW( dst_key, L"FirstEntry", NULL, &type, (BYTE *)first, &count )) goto done;
    if (type != REG_DWORD) goto done;
    count = sizeof(DWORD);
    if (RegQueryValueExW( dst_key, L"LastEntry", NULL, &type, (BYTE *)last, &count )) goto done;
    if (type != REG_DWORD) goto done;
    ret = 0;

done:
    RegCloseKey( dst_key );
    RegCloseKey( key );
    return ret;
}