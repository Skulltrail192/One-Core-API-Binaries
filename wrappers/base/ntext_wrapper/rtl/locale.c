/*++

Copyright (c) 2021 Shorthorn Project.

Module Name:

    locale.c

Abstract:

    This file contains functions that return information about a
    language group, a UI language, a locale, or a calendar on ntdll.

Revision History:

    Skulltrail 22-03-2021

--*/

#include <main.h>
#include <locale.h>

#define MUI_LANGUAGE_ID 1
#define MUI_LANGUAGE_NAME 2

/* NLS normalization file */
struct norm_table
{
    WCHAR   name[13];      /* 00 file name */
    USHORT  checksum[3];   /* 1a checksum? */
    USHORT  version[4];    /* 20 Unicode version */
    USHORT  form;          /* 28 normalization form */
    USHORT  len_factor;    /* 2a factor for length estimates */
    USHORT  unknown1;      /* 2c */
    USHORT  decomp_size;   /* 2e decomposition hash size */
    USHORT  comp_size;     /* 30 composition hash size */
    USHORT  unknown2;      /* 32 */
    USHORT  classes;       /* 34 combining classes table offset */
    USHORT  props_level1;  /* 36 char properties table level 1 offset */
    USHORT  props_level2;  /* 38 char properties table level 2 offset */
    USHORT  decomp_hash;   /* 3a decomposition hash table offset */
    USHORT  decomp_map;    /* 3c decomposition character map table offset */
    USHORT  decomp_seq;    /* 3e decomposition character sequences offset */
    USHORT  comp_hash;     /* 40 composition hash table offset */
    USHORT  comp_seq;      /* 42 composition character sequences offset */
    /* BYTE[]       combining class values */
    /* BYTE[0x2200] char properties index level 1 */
    /* BYTE[]       char properties index level 2 */
    /* WORD[]       decomposition hash table */
    /* WORD[]       decomposition character map */
    /* WORD[]       decomposition character sequences */
    /* WORD[]       composition hash table */
    /* WORD[]       composition character sequences */
};

static struct norm_table *norm_tables[16];

LCID user_lcid = 0, system_lcid = 0;

enum nls_section_type
{
    NLS_SECTION_SORTKEYS = 9,
    NLS_SECTION_CASEMAP = 10,
    NLS_SECTION_CODEPAGE = 11,
    NLS_SECTION_NORMALIZE = 12
};

static BYTE get_char_props( const struct norm_table *info, unsigned int ch )
{
    const BYTE *level1 = (const BYTE *)((const USHORT *)info + info->props_level1);
    const BYTE *level2 = (const BYTE *)((const USHORT *)info + info->props_level2);
    BYTE off = level1[ch / 128];

    if (!off || off >= 0xfb) return rol( off, 5 );
    return level2[(off - 1) * 128 + ch % 128];
}

static void put_utf16( WCHAR *dst, unsigned int ch )
{
    if (ch >= 0x10000)
    {
        ch -= 0x10000;
        dst[0] = 0xd800 | (ch >> 10);
        dst[1] = 0xdc00 | (ch & 0x3ff);
    }
    else dst[0] = ch;
}

/* Punycode parameters */
enum { BASE = 36, TMIN = 1, TMAX = 26, SKEW = 38, DAMP = 700 };

#define HANGUL_SBASE  0xac00
#define HANGUL_LBASE  0x1100
#define HANGUL_VBASE  0x1161
#define HANGUL_TBASE  0x11a7
#define HANGUL_LCOUNT 19
#define HANGUL_VCOUNT 21
#define HANGUL_TCOUNT 28
#define HANGUL_NCOUNT (HANGUL_VCOUNT * HANGUL_TCOUNT)
#define HANGUL_SCOUNT (HANGUL_LCOUNT * HANGUL_NCOUNT)

static NTSTATUS open_nls_data_file( ULONG type, ULONG id, HANDLE *file )
{
    static const WCHAR pathfmtW[] = {'\\','?','?','\\','%','s','%','s',0};
    static const WCHAR keyfmtW[] =
    {'\\','R','e','g','i','s','t','r','y','\\','M','a','c','h','i','n','e','\\','S','y','s','t','e','m','\\',
     'C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\',
     'C','o','n','t','r','o','l','\\','N','l','s','\\','%','s',0};
    static const WCHAR sortdirW[] = {'C',':','\\','w','i','n','d','o','w','s','\\',
                                     'g','l','o','b','a','l','i','z','a','t','i','o','n','\\',
                                     's','o','r','t','i','n','g','\\',0};
    static const WCHAR cpW[] = {'C','o','d','e','p','a','g','e',0};
    static const WCHAR normW[] = {'N','o','r','m','a','l','i','z','a','t','i','o','n',0};
    static const WCHAR langW[] = {'L','a','n','g','u','a','g','e',0};
    static const WCHAR cpfmtW[] = {'%','u',0};
    static const WCHAR normfmtW[] = {'%','x',0};
    static const WCHAR langfmtW[] = {'%','0','4','x',0};
    static const WCHAR winedatadirW[] = {'W','I','N','E','D','A','T','A','D','I','R',0};
    static const WCHAR winebuilddirW[] = {'W','I','N','E','B','U','I','L','D','D','I','R',0};
    static const WCHAR dataprefixW[] = {'\\','n','l','s','\\',0};
    static const WCHAR cpdefaultW[] = {'c','_','%','0','3','d','.','n','l','s',0};
    static const WCHAR intlW[] = {'l','_','i','n','t','l','.','n','l','s',0};
    static const WCHAR normnfcW[] = {'n','o','r','m','n','f','c','.','n','l','s',0};
    static const WCHAR normnfdW[] = {'n','o','r','m','n','f','d','.','n','l','s',0};
    static const WCHAR normnfkcW[] = {'n','o','r','m','n','f','k','c','.','n','l','s',0};
    static const WCHAR normnfkdW[] = {'n','o','r','m','n','f','k','d','.','n','l','s',0};
    static const WCHAR normidnaW[] = {'n','o','r','m','i','d','n','a','.','n','l','s',0};
    static const WCHAR sortkeysW[] = {'s','o','r','t','d','e','f','a','u','l','t','.','n','l','s',0};

    DWORD size;
    HANDLE handle;
    NTSTATUS status = STATUS_OBJECT_NAME_NOT_FOUND;
    IO_STATUS_BLOCK io;
    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING nameW, valueW;
    WCHAR buffer[MAX_PATH], value[10];
    const WCHAR *name = NULL, *dir = L"\\SystemRoot";
    KEY_VALUE_PARTIAL_INFORMATION *info;

    /* get filename from registry */

    switch (type)
    {
    case NLS_SECTION_SORTKEYS:
        if (id) return STATUS_INVALID_PARAMETER_1;
        buffer[0] = 0;
        break;
    case NLS_SECTION_CASEMAP:
        if (id) return STATUS_UNSUCCESSFUL;
        swprintf( buffer, keyfmtW, langW );
        swprintf( value, langfmtW, LANGIDFROMLCID(system_lcid) );
        break;
    case NLS_SECTION_CODEPAGE:
        swprintf( buffer, keyfmtW, cpW );
        swprintf( value, cpfmtW, id );
        break;
    case NLS_SECTION_NORMALIZE:
        swprintf( buffer, keyfmtW, normW );
        swprintf( value, normfmtW, id );
        break;
    default:
        return STATUS_INVALID_PARAMETER_1;
    }

    if (buffer[0])
    {
        RtlInitUnicodeString( &nameW, buffer );
        RtlInitUnicodeString( &valueW, value );
        InitializeObjectAttributes( &attr, &nameW, 0, 0, NULL );
        if (!(status = NtOpenKey( &handle, KEY_READ, &attr )))
        {
            info = (KEY_VALUE_PARTIAL_INFORMATION *)buffer;
            size = sizeof(buffer) - sizeof(WCHAR);
            if (!(status = NtQueryValueKey( handle, &valueW, KeyValuePartialInformation, info, size, &size )))
            {
                ((WCHAR *)info->Data)[info->DataLength / sizeof(WCHAR)] = 0;
                name = (WCHAR *)info->Data;
            }
            NtClose( handle );
        }
    }

    if (!name || !*name)  /* otherwise some hardcoded defaults */
    {
        switch (type)
        {
        case NLS_SECTION_SORTKEYS:
            name = sortkeysW;
            dir = sortdirW;
            break;
        case NLS_SECTION_CASEMAP:
            name = intlW;
            break;
        case NLS_SECTION_CODEPAGE:
            swprintf( buffer, cpdefaultW, id );
            name = buffer;
            break;
        case NLS_SECTION_NORMALIZE:
            switch (id)
            {
            case NormalizationC: name = normnfcW; break;
            case NormalizationD: name = normnfdW; break;
            case NormalizationKC: name = normnfkcW; break;
            case NormalizationKD: name = normnfkdW; break;
            case 13: name = normidnaW; break;
            }
            break;
        }
        if (!name) return status;
    }

    /* try to open file in system dir */

    valueW.MaximumLength = (wcslen(name) + wcslen(dir) + 5) * sizeof(WCHAR);
    if (!(valueW.Buffer = RtlAllocateHeap( RtlGetProcessHeap(), 0, valueW.MaximumLength )))
        return STATUS_NO_MEMORY;
    valueW.Length = swprintf( valueW.Buffer,
                              pathfmtW, dir, name ) * sizeof(WCHAR);
    InitializeObjectAttributes( &attr, &valueW, 0, 0, NULL );
    status = NtOpenFile( file, GENERIC_READ, &attr, &io, FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_ALERT );
    if (!status) DbgPrint( "found %s\n", valueW.Buffer);
    RtlFreeUnicodeString( &valueW );
    if (status != STATUS_OBJECT_NAME_NOT_FOUND && status != STATUS_OBJECT_PATH_NOT_FOUND) return status;

    /* not found, try in build or data dir */

    RtlInitUnicodeString( &nameW, winebuilddirW );
    valueW.MaximumLength = 0;
    if (RtlQueryEnvironmentVariable_U( NULL, &nameW, &valueW ) != STATUS_BUFFER_TOO_SMALL)
    {
        RtlInitUnicodeString( &nameW, winedatadirW );
        if (RtlQueryEnvironmentVariable_U( NULL, &nameW, &valueW ) != STATUS_BUFFER_TOO_SMALL)
            return status;
    }
    valueW.MaximumLength = valueW.Length + sizeof(dataprefixW) + wcslen(name) * sizeof(WCHAR);
    if (!(valueW.Buffer = RtlAllocateHeap( RtlGetProcessHeap(), 0, valueW.MaximumLength )))
        return STATUS_NO_MEMORY;
    if (!RtlQueryEnvironmentVariable_U( NULL, &nameW, &valueW ))
    {
        wcscat( valueW.Buffer, dataprefixW );
        wcscat( valueW.Buffer, name );
        valueW.Length = wcslen(valueW.Buffer) * sizeof(WCHAR);
        InitializeObjectAttributes( &attr, &valueW, 0, 0, NULL );
        status = NtOpenFile( file, GENERIC_READ, &attr, &io, FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_ALERT );
        if (!status) DbgPrint( "found %s\n",  valueW.Buffer );
    }
    RtlFreeUnicodeString( &valueW );
    return status;
}

/***********************************************************************
 *           LCIDToLocaleName  (KERNEL32.@)
 */
INT 
WINAPI 
RtlpLCIDToLocaleName( 
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

NTSTATUS 
NTAPI 	
RtlLcidToLocaleName(
	_In_ LCID lcid, 
	_Inout_ PUNICODE_STRING locale, 
	_In_ ULONG Flags, 
	_In_ BOOLEAN AllocateDestinationString
)
{
	LPWSTR lpName = L"";
	
	if(RtlpLCIDToLocaleName(lcid, lpName, 0, 0)>0){
		RtlInitUnicodeString(locale, lpName);
		return STATUS_SUCCESS;
	}else{
		return STATUS_UNSUCCESSFUL;		
	}

}

/*
 * @implemented - new - locale
*/
NTSTATUS 
NTAPI 
RtlLCIDToCultureName(
	IN LCID lcid, 
	OUT PUNICODE_STRING lpName
)
{
	return RtlLcidToLocaleName(lcid, lpName, 0, TRUE);
}

/*
 * @implemented - need test
*/
BOOLEAN 
NTAPI 
RtlCultureNameToLCID(
	__in PUNICODE_STRING String, 
	__out PLCID Lcid
)
{
	int i;
	
	for(i=0;i<LOCALE_TABLE_SIZE;i++){
		if(wcscmp(String->Buffer, locale_table[i].localeName)==0){
			*Lcid = locale_table[i].lcid;
			return TRUE;
		}
	}
	return FALSE;
}

NTSTATUS
NTAPI
RtlLocaleNameToLcid(
    _In_ PWSTR LocaleName,
    _Out_ PLCID lcid,
    _In_ ULONG Flags
)
{
    int i;

	for(i=0;i<LOCALE_TABLE_SIZE;i++){
		if(wcscmp(LocaleName, locale_table[i].localeName)==0){
			*lcid = locale_table[i].lcid;
			return STATUS_SUCCESS;
		}
	}
	
	return STATUS_UNSUCCESSFUL;
}

NTSTATUS 
WINAPI 
RtlGetUserPreferredUILanguages(
	DWORD dwFlags, 
	BOOL verification,
	PULONG pulNumLanguages, 
	PZZWSTR pwszLanguagesBuffer, 
	PULONG pcchLanguagesBuffer
)
{
	WCHAR locale[LOCALE_NAME_MAX_LENGTH];
	UINT length;	
	LCID UILangId;// = ((LCID)(NtCurrentTeb()->CurrentLocale));
	NTSTATUS status;
	
	status = NtQueryDefaultLocale(FALSE, &UILangId);
	
	if(!NT_SUCCESS(status)){
		return status;
	}

	if ( pwszLanguagesBuffer ){
		*pulNumLanguages = 2;	
		length = *pcchLanguagesBuffer;	
		if(dwFlags == MUI_LANGUAGE_ID){
			wcscpy(locale, _itow(UILangId, locale, 16));
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0409\0\0", sizeof(WCHAR)*(5));				
		}else{
			RtlpLCIDToLocaleName(UILangId, locale, LOCALE_NAME_MAX_LENGTH, 0);
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0en-US\0\0", sizeof(WCHAR)*(7));				
		}		
		return STATUS_SUCCESS;
	}else{
		*pulNumLanguages = 2;
		if(dwFlags == MUI_LANGUAGE_ID){
			length = 9;
		}else{
			length = (7 + RtlpLCIDToLocaleName(UILangId, NULL, 0, 0));
		}		
		*pcchLanguagesBuffer = length;
		return STATUS_INVALID_PARAMETER;
	}	
}

BOOL 
WINAPI 
RtlGetSystemPreferredUILanguages(
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
)
{
	WCHAR locale[LOCALE_NAME_MAX_LENGTH];
	UINT length;	
	LCID lcid = ((LCID)(NtCurrentTeb()->CurrentLocale));

	if ( pwszLanguagesBuffer ){
		*pulNumLanguages = 2;	
		length = *pcchLanguagesBuffer;	
		if(dwFlags == MUI_LANGUAGE_ID){
			wcscpy(locale, _itow(lcid, locale, 16));
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0409\0\0", sizeof(WCHAR)*(5));				
		}else{
			RtlpLCIDToLocaleName(lcid, locale, LOCALE_NAME_MAX_LENGTH, 0);
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0en-US\0\0", sizeof(WCHAR)*(7));				
		}		
		return STATUS_SUCCESS;
	}else{
		*pulNumLanguages = 2;
		if(dwFlags == MUI_LANGUAGE_ID){
			length = 9;
		}else{
			length = (7 + RtlpLCIDToLocaleName(lcid, NULL, 0, 0));
		}		
		*pcchLanguagesBuffer = length;
		return STATUS_INVALID_PARAMETER;
	}		
}

/*
 * @unimplemented - need reimplementation
 */
NTSTATUS 
WINAPI 
RtlGetThreadPreferredUILanguages(
	_In_       DWORD dwFlags,
	_Out_      PULONG pulNumLanguages,
	_Out_opt_  PZZWSTR pwszLanguagesBuffer,
	_Inout_    PULONG pcchLanguagesBuffer
)
{
	WCHAR locale[LOCALE_NAME_MAX_LENGTH];
	UINT length;	
	LCID lcid;
	
	lcid  = ((LCID)(NtCurrentTeb()->CurrentLocale));

	if ( pwszLanguagesBuffer ){
		*pulNumLanguages = 2;	
		length = *pcchLanguagesBuffer;	
		if(dwFlags == MUI_LANGUAGE_ID){
			wcscpy(locale, _itow(lcid, locale, 16));
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0409\0\0", sizeof(WCHAR)*(5));				
		}else{
			RtlpLCIDToLocaleName(lcid, locale, LOCALE_NAME_MAX_LENGTH, 0);
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0en-US\0\0", sizeof(WCHAR)*(7));				
		}		
		return STATUS_SUCCESS;
	}else{
		*pulNumLanguages = 2;
		if(dwFlags == MUI_LANGUAGE_ID){
			length = 9;
		}else{
			length = (7 + RtlpLCIDToLocaleName(lcid, NULL, 0, 0));
		}		
		*pcchLanguagesBuffer = length;
		return STATUS_INVALID_PARAMETER;
	}	
}

NTSTATUS 
WINAPI 
RtlGetUILanguageInfo(
	DWORD dwFlags, 
	PCZZWSTR pwmszLanguage, 
	PZZWSTR pwszFallbackLanguages,
	PDWORD pcchFallbackLanguages,
	PDWORD pdwAttributes
)
{
	pwmszLanguage = L"en-US";
	pwszFallbackLanguages = L"en-US";
	*pcchFallbackLanguages = 1;
	return STATUS_SUCCESS;
}

/*
 * @unimplemented - need reimplementation
 */
NTSTATUS 
WINAPI 
RtlSetThreadPreferredUILanguages(
  _In_       DWORD dwFlags,
  _In_opt_   PCZZWSTR pwszLanguagesBuffer,
  _Out_opt_  PULONG pulNumLanguages
)
{
	//Use Longhorn ntdll
	// LCID langId;
	
	// *pulNumLanguages = 1;
	
	// RtlLocaleNameToLcid(pwszLanguagesBuffer, langId, 0);
	// return NtSetDefaultUILanguage(&langId);
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
RtlpQueryDefaultUILanguage(
	LANGID langid, 
	BOOLEAN verification
)
{
	NTSTATUS Status; // eax@1

	Status = NtQueryDefaultUILanguage(&langid);
	if ( (Status & 0xC0000000) == 0xC0000000 )
		Status = NtQueryInstallUILanguage(&langid);
	return Status;
}

NTSTATUS 
NTAPI 
RtlpSetDefaultUILanguage(LANGID langId)
{
	return NtSetDefaultUILanguage(langId);
}

NTSTATUS 
NTAPI 
RtlpSetCurrentUserUILanguage(LANGID langId)
{
	return NtSetDefaultUILanguage(langId);
}

BOOL 
NTAPI 
RtlGetFileMUIPath(
  _In_         DWORD dwFlags,
  _In_         PCWSTR pcwszFilePath,
  _Inout_opt_  PWSTR pwszLanguage,
  _Inout_      PULONG pcchLanguage,
  _Out_opt_    PWSTR pwszFileMUIPath,
  _Inout_      PULONG pcchFileMUIPath,
  _Inout_      PULONGLONG pululEnumerator
)
{
	return FALSE;
}

/**************************************************************************
 *      NtGetNlsSectionPtr   (NTDLL.@)
 */
NTSTATUS WINAPI NtGetNlsSectionPtr( ULONG type, ULONG id, void *unknown, void **ptr, SIZE_T *size )
{
    FILE_END_OF_FILE_INFORMATION info;
    IO_STATUS_BLOCK io;
    HANDLE file;
    NTSTATUS status;

    if ((status = open_nls_data_file( type, id, &file ))) return status;

    if ((status = NtQueryInformationFile( file, &io, &info, sizeof(info), FileEndOfFileInformation )))
        goto done;
    /* FIXME: return a heap block instead of a file mapping for now */
    if (!(*ptr = RtlAllocateHeap( RtlGetProcessHeap(), 0, info.EndOfFile.QuadPart )))
    {
        status = STATUS_NO_MEMORY;
        goto done;
    }
    status = NtReadFile( file, 0, NULL, NULL, &io, *ptr, info.EndOfFile.QuadPart, NULL, NULL );
    if (!status && io.Information != info.EndOfFile.QuadPart) status = STATUS_INVALID_FILE_FOR_SECTION;
    if (!status) *size = io.Information;
    else RtlFreeHeap( RtlGetProcessHeap(), 0, *ptr );
done:
    NtClose( file );
    return status;
}

static int get_utf16( const WCHAR *src, unsigned int srclen, unsigned int *ch )
{
    if (IS_HIGH_SURROGATE( src[0] ))
    {
        if (srclen <= 1) return 0;
        if (!IS_LOW_SURROGATE( src[1] )) return 0;
        *ch = 0x10000 + ((src[0] & 0x3ff) << 10) + (src[1] & 0x3ff);
        return 2;
    }
    if (IS_LOW_SURROGATE( src[0] )) return 0;
    *ch = src[0];
    return 1;
}

/* helper for the various utf8 mbstowcs functions */
static unsigned int decode_utf8_char( unsigned char ch, const char **str, const char *strend )
{
    /* number of following bytes in sequence based on first byte value (for bytes above 0x7f) */
    static const char utf8_length[128] =
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x80-0x8f */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0x90-0x9f */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xa0-0xaf */
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 0xb0-0xbf */
        0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /* 0xc0-0xcf */
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, /* 0xd0-0xdf */
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, /* 0xe0-0xef */
        3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0  /* 0xf0-0xff */
    };

    /* first byte mask depending on UTF-8 sequence length */
    static const unsigned char utf8_mask[4] = { 0x7f, 0x1f, 0x0f, 0x07 };

    unsigned int len = utf8_length[ch - 0x80];
    unsigned int res = ch & utf8_mask[len];
    const char *end = *str + len;

    if (end > strend)
    {
        *str = end;
        return ~0;
    }
    switch (len)
    {
    case 3:
        if ((ch = end[-3] ^ 0x80) >= 0x40) break;
        res = (res << 6) | ch;
        (*str)++;
        if (res < 0x10) break;
    case 2:
        if ((ch = end[-2] ^ 0x80) >= 0x40) break;
        res = (res << 6) | ch;
        if (res >= 0x110000 >> 6) break;
        (*str)++;
        if (res < 0x20) break;
        if (res >= 0xd800 >> 6 && res <= 0xdfff >> 6) break;
    case 1:
        if ((ch = end[-1] ^ 0x80) >= 0x40) break;
        res = (res << 6) | ch;
        (*str)++;
        if (res < 0x80) break;
        return res;
    }
    return ~0;
}

/**************************************************************************
 *	RtlUTF8ToUnicodeN   (NTDLL.@)
 */
NTSTATUS WINAPI RtlUTF8ToUnicodeN( WCHAR *dst, DWORD dstlen, DWORD *reslen, const char *src, DWORD srclen )
{
    unsigned int res, len;
    NTSTATUS status = STATUS_SUCCESS;
    const char *srcend = src + srclen;
    WCHAR *dstend;

    if (!src) return STATUS_INVALID_PARAMETER_4;
    if (!reslen) return STATUS_INVALID_PARAMETER;

    dstlen /= sizeof(WCHAR);
    dstend = dst + dstlen;
    if (!dst)
    {
        for (len = 0; src < srcend; len++)
        {
            unsigned char ch = *src++;
            if (ch < 0x80) continue;
            if ((res = decode_utf8_char( ch, &src, srcend )) > 0x10ffff)
                status = STATUS_SOME_NOT_MAPPED;
            else
                if (res > 0xffff) len++;
        }
        *reslen = len * sizeof(WCHAR);
        return status;
    }

    while ((dst < dstend) && (src < srcend))
    {
        unsigned char ch = *src++;
        if (ch < 0x80)  /* special fast case for 7-bit ASCII */
        {
            *dst++ = ch;
            continue;
        }
        if ((res = decode_utf8_char( ch, &src, srcend )) <= 0xffff)
        {
            *dst++ = res;
        }
        else if (res <= 0x10ffff)  /* we need surrogates */
        {
            res -= 0x10000;
            *dst++ = 0xd800 | (res >> 10);
            if (dst == dstend) break;
            *dst++ = 0xdc00 | (res & 0x3ff);
        }
        else
        {
            *dst++ = 0xfffd;
            status = STATUS_SOME_NOT_MAPPED;
        }
    }
    if (src < srcend) status = STATUS_BUFFER_TOO_SMALL;  /* overflow */
    *reslen = (dstlen - (dstend - dst)) * sizeof(WCHAR);
    return status;
}

/**************************************************************************
 *	RtlUnicodeToUTF8N   (NTDLL.@)
 */
NTSTATUS WINAPI RtlUnicodeToUTF8N( char *dst, DWORD dstlen, DWORD *reslen, const WCHAR *src, DWORD srclen )
{
    char *end;
    unsigned int val, len;
    NTSTATUS status = STATUS_SUCCESS;

    if (!src) return STATUS_INVALID_PARAMETER_4;
    if (!reslen) return STATUS_INVALID_PARAMETER;
    if (dst && (srclen & 1)) return STATUS_INVALID_PARAMETER_5;

    srclen /= sizeof(WCHAR);

    if (!dst)
    {
        for (len = 0; srclen; srclen--, src++)
        {
            if (*src < 0x80) len++;  /* 0x00-0x7f: 1 byte */
            else if (*src < 0x800) len += 2;  /* 0x80-0x7ff: 2 bytes */
            else
            {
                if (!get_utf16( src, srclen, &val ))
                {
                    val = 0xfffd;
                    status = STATUS_SOME_NOT_MAPPED;
                }
                if (val < 0x10000) len += 3; /* 0x800-0xffff: 3 bytes */
                else   /* 0x10000-0x10ffff: 4 bytes */
                {
                    len += 4;
                    src++;
                    srclen--;
                }
            }
        }
        *reslen = len;
        return status;
    }

    for (end = dst + dstlen; srclen; srclen--, src++)
    {
        WCHAR ch = *src;

        if (ch < 0x80)  /* 0x00-0x7f: 1 byte */
        {
            if (dst > end - 1) break;
            *dst++ = ch;
            continue;
        }
        if (ch < 0x800)  /* 0x80-0x7ff: 2 bytes */
        {
            if (dst > end - 2) break;
            dst[1] = 0x80 | (ch & 0x3f);
            ch >>= 6;
            dst[0] = 0xc0 | ch;
            dst += 2;
            continue;
        }
        if (!get_utf16( src, srclen, &val ))
        {
            val = 0xfffd;
            status = STATUS_SOME_NOT_MAPPED;
        }
        if (val < 0x10000)  /* 0x800-0xffff: 3 bytes */
        {
            if (dst > end - 3) break;
            dst[2] = 0x80 | (val & 0x3f);
            val >>= 6;
            dst[1] = 0x80 | (val & 0x3f);
            val >>= 6;
            dst[0] = 0xe0 | val;
            dst += 3;
        }
        else   /* 0x10000-0x10ffff: 4 bytes */
        {
            if (dst > end - 4) break;
            dst[3] = 0x80 | (val & 0x3f);
            val >>= 6;
            dst[2] = 0x80 | (val & 0x3f);
            val >>= 6;
            dst[1] = 0x80 | (val & 0x3f);
            val >>= 6;
            dst[0] = 0xf0 | val;
            dst += 4;
            src++;
            srclen--;
        }
    }
    if (srclen) status = STATUS_BUFFER_TOO_SMALL;
    *reslen = dstlen - (end - dst);
    return status;
}

static NTSTATUS load_norm_table( ULONG form, const struct norm_table **info )
{
    unsigned int i;
    USHORT *data, *tables;
    SIZE_T size;
    NTSTATUS status;

    if (!form) return STATUS_INVALID_PARAMETER;
    if (form >= ARRAY_SIZE(norm_tables)) return STATUS_OBJECT_NAME_NOT_FOUND;

    if (!norm_tables[form])
    {
        if ((status = NtGetNlsSectionPtr( NLS_SECTION_NORMALIZE, form, NULL, (void **)&data, &size )))
            return status;

        /* sanity checks */

        if (size <= 0x44) goto invalid;
        if (data[0x14] != form) goto invalid;
        tables = data + 0x1a;
        for (i = 0; i < 8; i++)
        {
            if (tables[i] > size / sizeof(USHORT)) goto invalid;
            if (i && tables[i] < tables[i-1]) goto invalid;
        }

        if (InterlockedCompareExchangePointer( (void **)&norm_tables[form], data, NULL ))
            RtlFreeHeap( RtlGetProcessHeap(), 0, data );
    }
    *info = norm_tables[form];
    return STATUS_SUCCESS;

invalid:
    RtlFreeHeap( RtlGetProcessHeap(), 0, data );
    return STATUS_INVALID_PARAMETER;
}

static const WCHAR *get_decomposition( const struct norm_table *info, unsigned int ch,
                                       BYTE props, WCHAR *buffer, unsigned int *ret_len )
{
    const struct pair { WCHAR src; USHORT dst; } *pairs;
    const USHORT *hash_table = (const USHORT *)info + info->decomp_hash;
    const WCHAR *ret;
    unsigned int i, pos, end, len, hash;

    /* default to no decomposition */
    put_utf16( buffer, ch );
    *ret_len = 1 + (ch >= 0x10000);
    if (!props || props == 0x7f) return buffer;

    if (props == 0xff)  /* Hangul or invalid char */
    {
        if (ch >= HANGUL_SBASE && ch < HANGUL_SBASE + HANGUL_SCOUNT)
        {
            unsigned short sindex = ch - HANGUL_SBASE;
            unsigned short tindex = sindex % HANGUL_TCOUNT;
            buffer[0] = HANGUL_LBASE + sindex / HANGUL_NCOUNT;
            buffer[1] = HANGUL_VBASE + (sindex % HANGUL_NCOUNT) / HANGUL_TCOUNT;
            if (tindex) buffer[2] = HANGUL_TBASE + tindex;
            *ret_len = 2 + !!tindex;
            return buffer;
        }
        /* ignore other chars in Hangul range */
        if (ch >= HANGUL_LBASE && ch < HANGUL_LBASE + 0x100) return buffer;
        if (ch >= HANGUL_SBASE && ch < HANGUL_SBASE + 0x2c00) return buffer;
        return NULL;
    }

    hash = ch % info->decomp_size;
    pos = hash_table[hash];
    if (pos >> 13)
    {
        if (props != 0xbf) return buffer;
        ret = (const USHORT *)info + info->decomp_seq + (pos & 0x1fff);
        len = pos >> 13;
    }
    else
    {
        pairs = (const struct pair *)((const USHORT *)info + info->decomp_map);

        /* find the end of the hash bucket */
        for (i = hash + 1; i < info->decomp_size; i++) if (!(hash_table[i] >> 13)) break;
        if (i < info->decomp_size) end = hash_table[i];
        else for (end = pos; pairs[end].src; end++) ;

        for ( ; pos < end; pos++)
        {
            if (pairs[pos].src != (WCHAR)ch) continue;
            ret = (const USHORT *)info + info->decomp_seq + (pairs[pos].dst & 0x1fff);
            len = pairs[pos].dst >> 13;
            break;
        }
        if (pos >= end) return buffer;
    }

    if (len == 7) while (ret[len]) len++;
    if (!ret[0]) len = 0;  /* ignored char */
    *ret_len = len;
    return ret;
}

static BYTE get_combining_class( const struct norm_table *info, unsigned int c )
{
    const BYTE *classes = (const BYTE *)((const USHORT *)info + info->classes);
    BYTE class = get_char_props( info, c ) & 0x3f;

    if (class == 0x3f) return 0;
    return classes[class];
}

static BOOL is_starter( const struct norm_table *info, unsigned int c )
{
    return !get_combining_class( info, c );
}

static BOOL reorderable_pair( const struct norm_table *info, unsigned int c1, unsigned int c2 )
{
    BYTE ccc1, ccc2;

    /* reorderable if ccc1 > ccc2 > 0 */
    ccc1 = get_combining_class( info, c1 );
    if (ccc1 < 2) return FALSE;
    ccc2 = get_combining_class( info, c2 );
    return ccc2 && (ccc1 > ccc2);
}

static void canonical_order_substring( const struct norm_table *info, WCHAR *str, unsigned int len )
{
    unsigned int i, ch1, ch2, len1, len2;
    BOOL swapped;

    do
    {
        swapped = FALSE;
        for (i = 0; i < len - 1; i += len1)
        {
            if (!(len1 = get_utf16( str + i, len - i, &ch1 ))) break;
            if (i + len1 >= len) break;
            if (!(len2 = get_utf16( str + i + len1, len - i - len1, &ch2 ))) break;
            if (reorderable_pair( info, ch1, ch2 ))
            {
                WCHAR tmp[2];
                memcpy( tmp, str + i, len1 * sizeof(WCHAR) );
                memcpy( str + i, str + i + len1, len2 * sizeof(WCHAR) );
                memcpy( str + i + len2, tmp, len1 * sizeof(WCHAR) );
                swapped = TRUE;
                i += len2 - len1;
            }
        }
    } while (swapped);
}

/****************************************************************************
 *             canonical_order_string
 *
 * Reorder the string into canonical order - D108/D109.
 *
 * Starters (chars with combining class == 0) don't move, so look for continuous
 * substrings of non-starters and only reorder those.
 */
static void canonical_order_string( const struct norm_table *info, WCHAR *str, unsigned int len )
{
    unsigned int ch, i, r, next = 0;

    for (i = 0; i < len; i += r)
    {
        if (!(r = get_utf16( str + i, len - i, &ch ))) return;
        if (i && is_starter( info, ch ))
        {
            if (i > next + 1) /* at least two successive non-starters */
                canonical_order_substring( info, str + next, i - next );
            next = i + r;
        }
    }
    if (i > next + 1) canonical_order_substring( info, str + next, i - next );
}

static NTSTATUS decompose_string( const struct norm_table *info, const WCHAR *src, int src_len,
                                  WCHAR *dst, int *dst_len )
{
    BYTE props;
    int src_pos, dst_pos;
    unsigned int ch, len, decomp_len;
    WCHAR buffer[3];
    const WCHAR *decomp;

    for (src_pos = dst_pos = 0; src_pos < src_len; src_pos += len)
    {
        if (!(len = get_utf16( src + src_pos, src_len - src_pos, &ch )))
        {
            *dst_len = src_pos + IS_HIGH_SURROGATE( src[src_pos] );
            return STATUS_NO_UNICODE_TRANSLATION;
        }
        props = get_char_props( info, ch );
        if (!(decomp = get_decomposition( info, ch, props, buffer, &decomp_len )))
        {
            /* allow final null */
            if (!ch && src_pos == src_len - 1 && dst_pos < *dst_len)
            {
                dst[dst_pos++] = 0;
                break;
            }
            *dst_len = src_pos;
            return STATUS_NO_UNICODE_TRANSLATION;
        }
        if (dst_pos + decomp_len > *dst_len)
        {
            *dst_len += (src_len - src_pos) * info->len_factor;
            return STATUS_BUFFER_TOO_SMALL;
        }
        memcpy( dst + dst_pos, decomp, decomp_len * sizeof(WCHAR) );
        dst_pos += decomp_len;
    }

    canonical_order_string( info, dst, dst_pos );
    *dst_len = dst_pos;
    return STATUS_SUCCESS;
}

static unsigned int compose_chars( const struct norm_table *info, unsigned int ch1, unsigned int ch2 )
{
    const USHORT *table = (const USHORT *)info + info->comp_hash;
    const WCHAR *chars = (const USHORT *)info + info->comp_seq;
    unsigned int hash, start, end, i, len, ch[3];

    hash = (ch1 + 95 * ch2) % info->comp_size;
    start = table[hash];
    end = table[hash + 1];
    while (start < end)
    {
        for (i = 0; i < 3; i++, start += len) len = get_utf16( chars + start, end - start, ch + i );
        if (ch[0] == ch1 && ch[1] == ch2) return ch[2];
    }
    return 0;
}

static BOOL check_invalid_chars( const struct norm_table *info, DWORD flags,
                                 const unsigned int *buffer, int len )
{
    int i;

    for (i = 0; i < len; i++)
    {
        switch (buffer[i])
        {
        case 0x200c:  /* zero-width non-joiner */
        case 0x200d:  /* zero-width joiner */
            if (!i || get_combining_class( info, buffer[i - 1] ) != 9) return TRUE;
            break;
        case 0x2260:  /* not equal to */
        case 0x226e:  /* not less than */
        case 0x226f:  /* not greater than */
            if (flags & IDN_USE_STD3_ASCII_RULES) return TRUE;
            break;
        }
        switch (get_char_props( info, buffer[i] ))
        {
        case 0xbf:
            return TRUE;
        case 0xff:
            if (buffer[i] >= HANGUL_SBASE && buffer[i] < HANGUL_SBASE + 0x2c00) break;
            return TRUE;
        case 0x7f:
            if (!(flags & IDN_ALLOW_UNASSIGNED)) return TRUE;
            break;
        }
    }

    if ((flags & IDN_USE_STD3_ASCII_RULES) && len && (buffer[0] == '-' || buffer[len - 1] == '-'))
        return TRUE;

    return FALSE;
}

static unsigned int compose_hangul( unsigned int ch1, unsigned int ch2 )
{
    if (ch1 >= HANGUL_LBASE && ch1 < HANGUL_LBASE + HANGUL_LCOUNT)
    {
        int lindex = ch1 - HANGUL_LBASE;
        int vindex = ch2 - HANGUL_VBASE;
        if (vindex >= 0 && vindex < HANGUL_VCOUNT)
            return HANGUL_SBASE + (lindex * HANGUL_VCOUNT + vindex) * HANGUL_TCOUNT;
    }
    if (ch1 >= HANGUL_SBASE && ch1 < HANGUL_SBASE + HANGUL_SCOUNT)
    {
        int sindex = ch1 - HANGUL_SBASE;
        if (!(sindex % HANGUL_TCOUNT))
        {
            int tindex = ch2 - HANGUL_TBASE;
            if (tindex > 0 && tindex < HANGUL_TCOUNT) return ch1 + tindex;
        }
    }
    return 0;
}

static unsigned int compose_string( const struct norm_table *info, WCHAR *str, unsigned int srclen )
{
    unsigned int i, ch, comp, len, start_ch = 0, last_starter = srclen;
    BYTE class, prev_class = 0;

    for (i = 0; i < srclen; i += len)
    {
        if (!(len = get_utf16( str + i, srclen - i, &ch ))) return 0;
        class = get_combining_class( info, ch );
        if (last_starter == srclen || (prev_class && prev_class >= class) ||
            (!(comp = compose_hangul( start_ch, ch )) &&
             !(comp = compose_chars( info, start_ch, ch ))))
        {
            if (!class)
            {
                last_starter = i;
                start_ch = ch;
            }
            prev_class = class;
        }
        else
        {
            int comp_len = 1 + (comp >= 0x10000);
            int start_len = 1 + (start_ch >= 0x10000);

            if (comp_len != start_len)
                memmove( str + last_starter + comp_len, str + last_starter + start_len,
                         (i - (last_starter + start_len)) * sizeof(WCHAR) );
            memmove( str + i + comp_len - start_len, str + i + len, (srclen - i - len) * sizeof(WCHAR) );
            srclen += comp_len - start_len - len;
            start_ch = comp;
            i = last_starter;
            len = comp_len;
            prev_class = 0;
            put_utf16( str + i, comp );
        }
    }
    return srclen;
}

/******************************************************************************
 *      RtlNormalizeString   (NTDLL.@)
 */
NTSTATUS WINAPI RtlNormalizeString( ULONG form, const WCHAR *src, INT src_len, WCHAR *dst, INT *dst_len )
{
    int buf_len;
    WCHAR *buf = NULL;
    const struct norm_table *info;
    NTSTATUS status = STATUS_SUCCESS;

    DbgPrint( "%x %s %d %p %d\n", form, src, src_len, src_len, dst, *dst_len );

    if ((status = load_norm_table( form, &info ))) return status;

    if (src_len == -1) src_len = wcslen(src) + 1;

    if (!*dst_len)
    {
        *dst_len = src_len * info->len_factor;
        if (*dst_len > 64) *dst_len = max( 64, src_len + src_len / 8 );
        return STATUS_SUCCESS;
    }
    if (!src_len)
    {
        *dst_len = 0;
        return STATUS_SUCCESS;
    }

    if (!info->comp_size) return decompose_string( info, src, src_len, dst, dst_len );

    buf_len = src_len * 4;
    for (;;)
    {
        buf = RtlAllocateHeap( RtlGetProcessHeap(), 0, buf_len * sizeof(WCHAR) );
        if (!buf) return STATUS_NO_MEMORY;
        status = decompose_string( info, src, src_len, buf, &buf_len );
        if (status != STATUS_BUFFER_TOO_SMALL) break;
        RtlFreeHeap( RtlGetProcessHeap(), 0, buf );
    }
    if (!status)
    {
        buf_len = compose_string( info, buf, buf_len );
        if (*dst_len >= buf_len) memcpy( dst, buf, buf_len * sizeof(WCHAR) );
        else status = STATUS_BUFFER_TOO_SMALL;
    }
    RtlFreeHeap( RtlGetProcessHeap(), 0, buf );
    *dst_len = buf_len;
    return status;
}

/******************************************************************************
 *      RtlIsNormalizedString   (NTDLL.@)
 */
NTSTATUS WINAPI RtlIsNormalizedString( ULONG form, const WCHAR *str, INT len, BOOLEAN *res )
{
    const struct norm_table *info;
    NTSTATUS status;
    BYTE props, class, last_class = 0;
    unsigned int ch;
    int i, r, result = 1;

    if ((status = load_norm_table( form, &info ))) return status;

    if (len == -1) len = wcslen( str );

    for (i = 0; i < len && result; i += r)
    {
        if (!(r = get_utf16( str + i, len - i, &ch ))) return STATUS_NO_UNICODE_TRANSLATION;
        if (info->comp_size)
        {
            if ((ch >= HANGUL_VBASE && ch < HANGUL_VBASE + HANGUL_VCOUNT) ||
                (ch >= HANGUL_TBASE && ch < HANGUL_TBASE + HANGUL_TCOUNT))
            {
                result = -1;  /* QC=Maybe */
                continue;
            }
        }
        else if (ch >= HANGUL_SBASE && ch < HANGUL_SBASE + HANGUL_SCOUNT)
        {
            result = 0;  /* QC=No */
            break;
        }
        props = get_char_props( info, ch );
        class = props & 0x3f;
        if (class == 0x3f)
        {
            last_class = 0;
            if (props == 0xbf) result = 0;  /* QC=No */
            else if (props == 0xff)
            {
                /* ignore other chars in Hangul range */
                if (ch >= HANGUL_LBASE && ch < HANGUL_LBASE + 0x100) continue;
                if (ch >= HANGUL_SBASE && ch < HANGUL_SBASE + 0x2c00) continue;
                /* allow final null */
                if (!ch && i == len - 1) continue;
                return STATUS_NO_UNICODE_TRANSLATION;
            }
        }
        else if (props & 0x80)
        {
            if ((props & 0xc0) == 0xc0) result = -1;  /* QC=Maybe */
            if (class && class < last_class) result = 0;  /* QC=No */
            last_class = class;
        }
        else last_class = 0;
    }

    if (result == -1)
    {
        int dstlen = len * 4;
        NTSTATUS status;
        WCHAR *buffer = RtlAllocateHeap( RtlGetProcessHeap(), 0, dstlen * sizeof(WCHAR) );
        if (!buffer) return STATUS_NO_MEMORY;
        status = RtlNormalizeString( form, str, len, buffer, &dstlen );
        result = !status && (dstlen == len) && !wcsncmp( buffer, str, len );
        RtlFreeHeap( RtlGetProcessHeap(), 0, buffer );
    }
    *res = result;
    return STATUS_SUCCESS;
}

/******************************************************************************
 *      RtlIdnToNameprepUnicode   (NTDLL.@)
 */
NTSTATUS WINAPI RtlIdnToNameprepUnicode( DWORD flags, const WCHAR *src, INT srclen,
                                         WCHAR *dst, INT *dstlen )
{
    const struct norm_table *info;
    unsigned int ch;
    NTSTATUS status;
    WCHAR buf[256];
    int i, start, len, buflen = ARRAY_SIZE(buf);

    if (flags & ~(IDN_ALLOW_UNASSIGNED | IDN_USE_STD3_ASCII_RULES)) return STATUS_INVALID_PARAMETER;
    if (!src || srclen < -1) return STATUS_INVALID_PARAMETER;

    DbgPrint( "%x %s %p %d\n", flags, src, dst, *dstlen );

    if ((status = load_norm_table( 13, &info ))) return status;

    if (srclen == -1) srclen = wcslen(src) + 1;

    for (i = 0; i < srclen; i++) if (src[i] < 0x20 || src[i] >= 0x7f) break;

    if (i == srclen || (i == srclen - 1 && !src[i]))  /* ascii only */
    {
        if (srclen > buflen) return STATUS_INVALID_IDN_NORMALIZATION;
        memcpy( buf, src, srclen * sizeof(WCHAR) );
        buflen = srclen;
    }
    else if ((status = RtlNormalizeString( 13, src, srclen, buf, &buflen )))
    {
        if (status == STATUS_NO_UNICODE_TRANSLATION) status = STATUS_INVALID_IDN_NORMALIZATION;
        return status;
    }

    for (i = start = 0; i < buflen; i += len)
    {
        if (!(len = get_utf16( buf + i, buflen - i, &ch ))) break;
        if (!ch) break;
        if (ch == '.')
        {
            if (start == i) return STATUS_INVALID_IDN_NORMALIZATION;
            /* maximal label length is 63 characters */
            if (i - start > 63) return STATUS_INVALID_IDN_NORMALIZATION;
            if ((flags & IDN_USE_STD3_ASCII_RULES) && (buf[start] == '-' || buf[i-1] == '-'))
                return STATUS_INVALID_IDN_NORMALIZATION;
            start = i + 1;
            continue;
        }
        if (flags & IDN_USE_STD3_ASCII_RULES)
        {
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                (ch >= '0' && ch <= '9') || ch == '-') continue;
            return STATUS_INVALID_IDN_NORMALIZATION;
        }
        if (!(flags & IDN_ALLOW_UNASSIGNED))
        {
            if (get_char_props( info, ch ) == 0x7f) return STATUS_INVALID_IDN_NORMALIZATION;
        }
    }
    if (!i || i - start > 63) return STATUS_INVALID_IDN_NORMALIZATION;
    if ((flags & IDN_USE_STD3_ASCII_RULES) && (buf[start] == '-' || buf[i-1] == '-'))
        return STATUS_INVALID_IDN_NORMALIZATION;

    if (*dstlen)
    {
        if (buflen <= *dstlen) memcpy( dst, buf, buflen * sizeof(WCHAR) );
        else status = STATUS_BUFFER_TOO_SMALL;
    }
    *dstlen = buflen;
    return status;
}

/******************************************************************************
 *      RtlIdnToAscii   (NTDLL.@)
 */
NTSTATUS WINAPI RtlIdnToAscii( DWORD flags, const WCHAR *src, INT srclen, WCHAR *dst, INT *dstlen )
{
    static const WCHAR prefixW[] = {'x','n','-','-'};
    const struct norm_table *info;
    NTSTATUS status;
    WCHAR normstr[256], res[256];
    unsigned int ch, buffer[64];
    int i, len, start, end, out_label, out = 0, normlen = ARRAY_SIZE(normstr);

    DbgPrint( "%x %s %p %d\n", flags, src, dst, *dstlen );

    if ((status = load_norm_table( 13, &info ))) return status;

    if ((status = RtlIdnToNameprepUnicode( flags, src, srclen, normstr, &normlen ))) return status;

    /* implementation of Punycode based on RFC 3492 */

    for (start = 0; start < normlen; start = end + 1)
    {
        int n = 0x80, bias = 72, delta = 0, b = 0, h, buflen = 0;

        out_label = out;
        for (i = start; i < normlen; i += len)
        {
            if (!(len = get_utf16( normstr + i, normlen - i, &ch ))) break;
            if (!ch || ch == '.') break;
            if (ch < 0x80) b++;
            buffer[buflen++] = ch;
        }
        end = i;

        if (b == end - start)
        {
            if (end < normlen) b++;
            if (out + b > ARRAY_SIZE(res)) return STATUS_INVALID_IDN_NORMALIZATION;
            memcpy( res + out, normstr + start, b * sizeof(WCHAR) );
            out += b;
            continue;
        }

        if (buflen >= 4 && buffer[2] == '-' && buffer[3] == '-') return STATUS_INVALID_IDN_NORMALIZATION;
        if (check_invalid_chars( info, flags, buffer, buflen )) return STATUS_INVALID_IDN_NORMALIZATION;

        if (out + 5 + b > ARRAY_SIZE(res)) return STATUS_INVALID_IDN_NORMALIZATION;
        memcpy( res + out, prefixW, sizeof(prefixW) );
        out += ARRAY_SIZE(prefixW);
        if (b)
        {
            for (i = start; i < end; i++) if (normstr[i] < 0x80) res[out++] = normstr[i];
            res[out++] = '-';
        }

        for (h = b; h < buflen; delta++, n++)
        {
            int m = 0x10ffff, q, k;

            for (i = 0; i < buflen; i++) if (buffer[i] >= n && m > buffer[i]) m = buffer[i];
            delta += (m - n) * (h + 1);
            n = m;

            for (i = 0; i < buflen; i++)
            {
                if (buffer[i] == n)
                {
                    for (q = delta, k = BASE; ; k += BASE)
                    {
                        int t = k <= bias ? TMIN : k >= bias + TMAX ? TMAX : k - bias;
                        int disp = q < t ? q : t + (q - t) % (BASE - t);
                        if (out + 1 > ARRAY_SIZE(res)) return STATUS_INVALID_IDN_NORMALIZATION;
                        res[out++] = disp <= 25 ? 'a' + disp : '0' + disp - 26;
                        if (q < t) break;
                        q = (q - t) / (BASE - t);
                    }
                    delta /= (h == b ? DAMP : 2);
                    delta += delta / (h + 1);
                    for (k = 0; delta > ((BASE - TMIN) * TMAX) / 2; k += BASE) delta /= BASE - TMIN;
                    bias = k + ((BASE - TMIN + 1) * delta) / (delta + SKEW);
                    delta = 0;
                    h++;
                }
                else if (buffer[i] < n) delta++;
            }
        }

        if (out - out_label > 63) return STATUS_INVALID_IDN_NORMALIZATION;

        if (end < normlen)
        {
            if (out + 1 > ARRAY_SIZE(res)) return STATUS_INVALID_IDN_NORMALIZATION;
            res[out++] = normstr[end];
        }
    }

    if (*dstlen)
    {
        if (out <= *dstlen) memcpy( dst, res, out * sizeof(WCHAR) );
        else status = STATUS_BUFFER_TOO_SMALL;
    }
    *dstlen = out;
    return status;
}

/******************************************************************************
 *      RtlIdnToUnicode   (NTDLL.@)
 */
NTSTATUS WINAPI RtlIdnToUnicode( DWORD flags, const WCHAR *src, INT srclen, WCHAR *dst, INT *dstlen )
{
    const struct norm_table *info;
    int i, buflen, start, end, out_label, out = 0;
    NTSTATUS status;
    UINT buffer[64];
    WCHAR ch;

    if (!src || srclen < -1) return STATUS_INVALID_PARAMETER;
    if (srclen == -1) srclen = wcslen( src ) + 1;

    DbgPrint( "%x %s %p %d\n", flags, src, dst, *dstlen );

    if ((status = load_norm_table( 13, &info ))) return status;

    for (start = 0; start < srclen; )
    {
        int n = 0x80, bias = 72, pos = 0, old_pos, w, k, t, delim = 0, digit, delta;

        out_label = out;
        for (i = start; i < srclen; i++)
        {
            ch = src[i];
            if (ch > 0x7f || (i != srclen - 1 && !ch)) return STATUS_INVALID_IDN_NORMALIZATION;
            if (!ch || ch == '.') break;
            if (ch == '-') delim = i;

            if (!(flags & IDN_USE_STD3_ASCII_RULES)) continue;
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                (ch >= '0' && ch <= '9') || ch == '-')
                continue;
            return STATUS_INVALID_IDN_NORMALIZATION;
        }
        end = i;

        /* last label may be empty */
        if (start == end && ch) return STATUS_INVALID_IDN_NORMALIZATION;

        if (end - start < 4 ||
            (src[start] != 'x' && src[start] != 'X') ||
            (src[start + 1] != 'n' && src[start + 1] != 'N') ||
            src[start + 2] != '-' || src[start + 3] != '-')
        {
            if (end - start > 63) return STATUS_INVALID_IDN_NORMALIZATION;

            if ((flags & IDN_USE_STD3_ASCII_RULES) && (src[start] == '-' || src[end - 1] == '-'))
                return STATUS_INVALID_IDN_NORMALIZATION;

            if (end < srclen) end++;
            if (*dstlen)
            {
                if (out + end - start <= *dstlen)
                    memcpy( dst + out, src + start, (end - start) * sizeof(WCHAR));
                else return STATUS_BUFFER_TOO_SMALL;
            }
            out += end - start;
            start = end;
            continue;
        }

        if (delim == start + 3) delim++;
        buflen = 0;
        for (i = start + 4; i < delim && buflen < ARRAY_SIZE(buffer); i++) buffer[buflen++] = src[i];
        if (buflen) i++;
        while (i < end)
        {
            old_pos = pos;
            w = 1;
            for (k = BASE; ; k += BASE)
            {
                if (i >= end) return STATUS_INVALID_IDN_NORMALIZATION;
                ch = src[i++];
                if (ch >= 'a' && ch <= 'z') digit = ch - 'a';
                else if (ch >= 'A' && ch <= 'Z') digit = ch - 'A';
                else if (ch >= '0' && ch <= '9') digit = ch - '0' + 26;
                else return STATUS_INVALID_IDN_NORMALIZATION;
                pos += digit * w;
                t = k <= bias ? TMIN : k >= bias + TMAX ? TMAX : k - bias;
                if (digit < t) break;
                w *= BASE - t;
            }

            delta = (pos - old_pos) / (!old_pos ? DAMP : 2);
            delta += delta / (buflen + 1);
            for (k = 0; delta > ((BASE - TMIN) * TMAX) / 2; k += BASE) delta /= BASE - TMIN;
            bias = k + ((BASE - TMIN + 1) * delta) / (delta + SKEW);
            n += pos / (buflen + 1);
            pos %= buflen + 1;

            if (buflen >= ARRAY_SIZE(buffer) - 1) return STATUS_INVALID_IDN_NORMALIZATION;
            memmove( buffer + pos + 1, buffer + pos, (buflen - pos) * sizeof(*buffer) );
            buffer[pos++] = n;
            buflen++;
        }

        if (check_invalid_chars( info, flags, buffer, buflen )) return STATUS_INVALID_IDN_NORMALIZATION;

        for (i = 0; i < buflen; i++)
        {
            int len = 1 + (buffer[i] >= 0x10000);
            if (*dstlen)
            {
                if (out + len <= *dstlen) put_utf16( dst + out, buffer[i] );
                else return STATUS_BUFFER_TOO_SMALL;
            }
            out += len;
        }

        if (out - out_label > 63) return STATUS_INVALID_IDN_NORMALIZATION;

        if (end < srclen)
        {
            if (*dstlen)
            {
                if (out + 1 <= *dstlen) dst[out] = src[end];
                else return STATUS_BUFFER_TOO_SMALL;
            }
            out++;
        }
        start = end + 1;
    }
    *dstlen = out;
    return STATUS_SUCCESS;
}