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
#include <ucrtbase.h>

WINE_DEFAULT_DEBUG_CHANNEL(appmodel);

#define MAX_ELEM_LEN 64 /* Max length of country/language/CP string */

/* Note: Flags are weighted in order of matching importance */
#define FOUND_LANGUAGE         0x4
#define FOUND_COUNTRY          0x2
#define FOUND_CODEPAGE         0x1

/* MT */
#define LOCK_LOCALE   _mlock(_SETLOCALE_LOCK);
#define UNLOCK_LOCALE _munlock(_SETLOCALE_LOCK);

#define CONTINUE_LOOKING TRUE
#define STOP_LOOKING     FALSE

/* Index to TLS */
static DWORD msvcrt_tls_index;

thread_data_t *msvcrt_get_thread_data(void) DECLSPEC_HIDDEN;

typedef struct {
  char search_language[MAX_ELEM_LEN];
  char search_country[MAX_ELEM_LEN];
  char search_codepage[MAX_ELEM_LEN];
  char found_codepage[MAX_ELEM_LEN];
  unsigned int match_flags;
  LANGID found_lang_id;
} locale_search_t;

/* Friendly country strings & language names abbreviations. */
static const char * const _country_synonyms[] =
{
    "american", "enu",
    "american english", "enu",
    "american-english", "enu",
    "english-american", "enu",
    "english-us", "enu",
    "english-usa", "enu",
    "us", "enu",
    "usa", "enu",
    "australian", "ena",
    "english-aus", "ena",
    "belgian", "nlb",
    "french-belgian", "frb",
    "canadian", "enc",
    "english-can", "enc",
    "french-canadian", "frc",
    "chinese", "chs",
    "chinese-simplified", "chs",
    "chinese-traditional", "cht",
    "dutch-belgian", "nlb",
    "english-nz", "enz",
    "uk", "eng",
    "english-uk", "eng",
    "french-swiss", "frs",
    "swiss", "des",
    "german-swiss", "des",
    "italian-swiss", "its",
    "german-austrian", "dea",
    "portuguese", "ptb",
    "portuguese-brazil", "ptb",
    "spanish-mexican", "esm",
    "norwegian-bokmal", "nor",
    "norwegian-nynorsk", "non",
    "spanish-modern", "esn"
};

int MSVCRT___lc_collate_cp = 0;
LCID MSVCRT___lc_handle[LC_MAX - LC_MIN + 1] = { 0 };

unsigned int __lc_codepage = 0;
int __mb_cur_max = 1;

/* It seems that the data about valid trail bytes is not available from kernel32
 * so we have to store is here. The format is the same as for lead bytes in CPINFO */
struct cp_extra_info_t
{
    int cp;
    BYTE TrailBytes[MAX_LEADBYTES];
};

const unsigned short _ctype[] = {
	0,			/* <EOF>, 0xFFFF */
	_CONTROL,		/* CTRL+@, 0x00 */
	_CONTROL,		/* CTRL+A, 0x01 */
	_CONTROL,		/* CTRL+B, 0x02 */
	_CONTROL,		/* CTRL+C, 0x03 */
	_CONTROL,		/* CTRL+D, 0x04 */
	_CONTROL,		/* CTRL+E, 0x05 */
	_CONTROL,		/* CTRL+F, 0x06 */
	_CONTROL,		/* CTRL+G, 0x07 */
	_CONTROL,		/* CTRL+H, 0x08 */
	_CONTROL | _SPACE,	/* CTRL+I, 0x09 */
	_CONTROL | _SPACE,	/* CTRL+J, 0x0a */
	_CONTROL | _SPACE,	/* CTRL+K, 0x0b */
	_CONTROL | _SPACE,	/* CTRL+L, 0x0c */
	_CONTROL | _SPACE,	/* CTRL+M, 0x0d */
	_CONTROL,		/* CTRL+N, 0x0e */
	_CONTROL,		/* CTRL+O, 0x0f */
	_CONTROL,		/* CTRL+P, 0x10 */
	_CONTROL,		/* CTRL+Q, 0x11 */
	_CONTROL,		/* CTRL+R, 0x12 */
	_CONTROL,		/* CTRL+S, 0x13 */
	_CONTROL,		/* CTRL+T, 0x14 */
	_CONTROL,		/* CTRL+U, 0x15 */
	_CONTROL,		/* CTRL+V, 0x16 */
	_CONTROL,		/* CTRL+W, 0x17 */
	_CONTROL,		/* CTRL+X, 0x18 */
	_CONTROL,		/* CTRL+Y, 0x19 */
	_CONTROL,		/* CTRL+Z, 0x1a */
	_CONTROL,		/* CTRL+[, 0x1b */
	_CONTROL,		/* CTRL+\, 0x1c */
	_CONTROL,		/* CTRL+], 0x1d */
	_CONTROL,		/* CTRL+^, 0x1e */
	_CONTROL,		/* CTRL+_, 0x1f */
	_SPACE | _BLANK,	/* ` ', 0x20 */
	_PUNCT,			/* `!', 0x21 */
	_PUNCT,			/* 0x22 */
	_PUNCT,			/* `#', 0x23 */
	_PUNCT,			/* `$', 0x24 */
	_PUNCT,			/* `%', 0x25 */
	_PUNCT,			/* `&', 0x26 */
	_PUNCT,			/* 0x27 */
	_PUNCT,			/* `(', 0x28 */
	_PUNCT,			/* `)', 0x29 */
	_PUNCT,			/* `*', 0x2a */
	_PUNCT,			/* `+', 0x2b */
	_PUNCT,			/* `,', 0x2c */
	_PUNCT,			/* `-', 0x2d */
	_PUNCT,			/* `.', 0x2e */
	_PUNCT,			/* `/', 0x2f */
	_DIGIT | _HEX,		/* `0', 0x30 */
	_DIGIT | _HEX,		/* `1', 0x31 */
	_DIGIT | _HEX,		/* `2', 0x32 */
	_DIGIT | _HEX,		/* `3', 0x33 */
	_DIGIT | _HEX,		/* `4', 0x34 */
	_DIGIT | _HEX,		/* `5', 0x35 */
	_DIGIT | _HEX,		/* `6', 0x36 */
	_DIGIT | _HEX,		/* `7', 0x37 */
	_DIGIT | _HEX,		/* `8', 0x38 */
	_DIGIT | _HEX,		/* `9', 0x39 */
	_PUNCT,			/* `:', 0x3a */
	_PUNCT,			/* `;', 0x3b */
	_PUNCT,			/* `<', 0x3c */
	_PUNCT,			/* `=', 0x3d */
	_PUNCT,			/* `>', 0x3e */
	_PUNCT,			/* `?', 0x3f */
	_PUNCT,			/* `@', 0x40 */
	_UPPER | _HEX,		/* `A', 0x41 */
	_UPPER | _HEX,		/* `B', 0x42 */
	_UPPER | _HEX,		/* `C', 0x43 */
	_UPPER | _HEX,		/* `D', 0x44 */
	_UPPER | _HEX,		/* `E', 0x45 */
	_UPPER | _HEX,		/* `F', 0x46 */
	_UPPER,			/* `G', 0x47 */
	_UPPER,			/* `H', 0x48 */
	_UPPER,			/* `I', 0x49 */
	_UPPER,			/* `J', 0x4a */
	_UPPER,			/* `K', 0x4b */
	_UPPER,			/* `L', 0x4c */
	_UPPER,			/* `M', 0x4d */
	_UPPER,			/* `N', 0x4e */
	_UPPER,			/* `O', 0x4f */
	_UPPER,			/* `P', 0x50 */
	_UPPER,			/* `Q', 0x51 */
	_UPPER,			/* `R', 0x52 */
	_UPPER,			/* `S', 0x53 */
	_UPPER,			/* `T', 0x54 */
	_UPPER,			/* `U', 0x55 */
	_UPPER,			/* `V', 0x56 */
	_UPPER,			/* `W', 0x57 */
	_UPPER,			/* `X', 0x58 */
	_UPPER,			/* `Y', 0x59 */
	_UPPER,			/* `Z', 0x5a */
	_PUNCT,			/* `[', 0x5b */
	_PUNCT,			/* 0x5c */
	_PUNCT,			/* `]', 0x5d */
	_PUNCT,			/* `^', 0x5e */
	_PUNCT,			/* `_', 0x5f */
	_PUNCT,			/* 0x60 */
	_LOWER | _HEX,		/* `a', 0x61 */
	_LOWER | _HEX,		/* `b', 0x62 */
	_LOWER | _HEX,		/* `c', 0x63 */
	_LOWER | _HEX,		/* `d', 0x64 */
	_LOWER | _HEX,		/* `e', 0x65 */
	_LOWER | _HEX,		/* `f', 0x66 */
	_LOWER,			/* `g', 0x67 */
	_LOWER,			/* `h', 0x68 */
	_LOWER,			/* `i', 0x69 */
	_LOWER,			/* `j', 0x6a */
	_LOWER,			/* `k', 0x6b */
	_LOWER,			/* `l', 0x6c */
	_LOWER,			/* `m', 0x6d */
	_LOWER,			/* `n', 0x6e */
	_LOWER,			/* `o', 0x6f */
	_LOWER,			/* `p', 0x70 */
	_LOWER,			/* `q', 0x71 */
	_LOWER,			/* `r', 0x72 */
	_LOWER,			/* `s', 0x73 */
	_LOWER,			/* `t', 0x74 */
	_LOWER,			/* `u', 0x75 */
	_LOWER,			/* `v', 0x76 */
	_LOWER,			/* `w', 0x77 */
	_LOWER,			/* `x', 0x78 */
	_LOWER,			/* `y', 0x79 */
	_LOWER,			/* `z', 0x7a */
	_PUNCT,			/* `{', 0x7b */
	_PUNCT,			/* `|', 0x7c */
	_PUNCT,			/* `}', 0x7d */
	_PUNCT,			/* `~', 0x7e */
	_CONTROL,		/* 0x7f */
	0,			/* 0x80 */
	0,			/* 0x81 */
	0,			/* 0x82 */
	0,			/* 0x83 */
	0,			/* 0x84 */
	0,			/* 0x85 */
	0,			/* 0x86 */
	0,			/* 0x87 */
	0,			/* 0x88 */
	0,			/* 0x89 */
	0,			/* 0x8a */
	0,			/* 0x8b */
	0,			/* 0x8c */
	0,			/* 0x8d */
	0,			/* 0x8e */
	0,			/* 0x8f */
	0,			/* 0x90 */
	0,			/* 0x91 */
	0,			/* 0x92 */
	0,			/* 0x93 */
	0,			/* 0x94 */
	0,			/* 0x95 */
	0,			/* 0x96 */
	0,			/* 0x97 */
	0,			/* 0x98 */
	0,			/* 0x99 */
	0,			/* 0x9a */
	0,			/* 0x9b */
	0,			/* 0x9c */
	0,			/* 0x9d */
	0,			/* 0x9e */
	0,			/* 0x9f */
	0,			/* 0xa0 */
	0,			/* 0xa1 */
	0,			/* 0xa2 */
	0,			/* 0xa3 */
	0,			/* 0xa4 */
	0,			/* 0xa5 */
	0,			/* 0xa6 */
	0,			/* 0xa7 */
	0,			/* 0xa8 */
	0,			/* 0xa9 */
	0,			/* 0xaa */
	0,			/* 0xab */
	0,			/* 0xac */
	0,			/* 0xad */
	0,			/* 0xae */
	0,			/* 0xaf */
	0,			/* 0xb0 */
	0,			/* 0xb1 */
	0,			/* 0xb2 */
	0,			/* 0xb3 */
	0,			/* 0xb4 */
	0,			/* 0xb5 */
	0,			/* 0xb6 */
	0,			/* 0xb7 */
	0,			/* 0xb8 */
	0,			/* 0xb9 */
	0,			/* 0xba */
	0,			/* 0xbb */
	0,			/* 0xbc */
	0,			/* 0xbd */
	0,			/* 0xbe */
	0,			/* 0xbf */
	0,			/* 0xc0 */
	0,			/* 0xc1 */
	0,			/* 0xc2 */
	0,			/* 0xc3 */
	0,			/* 0xc4 */
	0,			/* 0xc5 */
	0,			/* 0xc6 */
	0,			/* 0xc7 */
	0,			/* 0xc8 */
	0,			/* 0xc9 */
	0,			/* 0xca */
	0,			/* 0xcb */
	0,			/* 0xcc */
	0,			/* 0xcd */
	0,			/* 0xce */
	0,			/* 0xcf */
	0,			/* 0xd0 */
	0,			/* 0xd1 */
	0,			/* 0xd2 */
	0,			/* 0xd3 */
	0,			/* 0xd4 */
	0,			/* 0xd5 */
	0,			/* 0xd6 */
	0,			/* 0xd7 */
	0,			/* 0xd8 */
	0,			/* 0xd9 */
	0,			/* 0xda */
	0,			/* 0xdb */
	0,			/* 0xdc */
	0,			/* 0xdd */
	0,			/* 0xde */
	0,			/* 0xdf */
	0,			/* 0xe0 */
	0,			/* 0xe1 */
	0,			/* 0xe2 */
	0,			/* 0xe3 */
	0,			/* 0xe4 */
	0,			/* 0xe5 */
	0,			/* 0xe6 */
	0,			/* 0xe7 */
	0,			/* 0xe8 */
	0,			/* 0xe9 */
	0,			/* 0xea */
	0,			/* 0xeb */
	0,			/* 0xec */
	0,			/* 0xed */
	0,			/* 0xee */
	0,			/* 0xef */
	0,			/* 0xf0 */
	0,			/* 0xf1 */
	0,			/* 0xf2 */
	0,			/* 0xf3 */
	0,			/* 0xf4 */
	0,			/* 0xf5 */
	0,			/* 0xf6 */
	0,			/* 0xf7 */
	0,			/* 0xf8 */
	0,			/* 0xf9 */
	0,			/* 0xfa */
	0,			/* 0xfb */
	0,			/* 0xfc */
	0,			/* 0xfd */
	0,			/* 0xfe */
	0			/* 0xff */
};

static struct cp_extra_info_t g_cpextrainfo[] =
{
    {932, {0x40, 0x7e, 0x80, 0xfc, 0, 0}},
    {936, {0x40, 0xfe, 0, 0}},
    {949, {0x41, 0xfe, 0, 0}},
    {950, {0x40, 0x7e, 0xa1, 0xfe, 0, 0}},
    {1361, {0x31, 0x7e, 0x81, 0xfe, 0, 0}},
    {20932, {1, 255, 0, 0}},  /* seems to give different results on different systems */
    {0, {1, 255, 0, 0}}       /* match all with FIXME */
};

unsigned char _mbctype[257] = { 0 };

char __syserr00[] = "No Error";
char __syserr01[] = "Operation not permitted (EPERM)";
char __syserr02[] = "No such file or directory (ENOENT)";
char __syserr03[] = "No such process (ESRCH)";
char __syserr04[] = "Interrupted system call (EINTR)";
char __syserr05[] = "Input or output error (EIO)";
char __syserr06[] = "No such device or address (ENXIO)";
char __syserr07[] = "Argument list too long (E2BIG)";
char __syserr08[] = "Unable to execute file (ENOEXEC)";
char __syserr09[] = "Bad file descriptor (EBADF)";
char __syserr10[] = "No child processes (ECHILD)";
char __syserr11[] = "Resource temporarily unavailable (EAGAIN)";
char __syserr12[] = "Not enough memory (ENOMEM)";
char __syserr13[] = "Permission denied (EACCES)";
char __syserr14[] = "Bad address (EFAULT)";
char __syserr15[] = "Unknown Error: 15";
char __syserr16[] = "Resource busy (EBUSY)";
char __syserr17[] = "File exists (EEXIST)";
char __syserr18[] = "Improper link (EXDEV)";
char __syserr19[] = "No such device (ENODEV)";
char __syserr20[] = "Not a directory (ENOTDIR)";
char __syserr21[] = "Is a directory (EISDIR)";
char __syserr22[] = "Invalid argument (EINVAL)";
char __syserr23[] = "Too many open files in system (ENFILE)";
char __syserr24[] = "Too many open files (EMFILE)";
char __syserr25[] = "Inappropriate I/O control operation (ENOTTY)";
char __syserr26[] = "Unknown error: 26";
char __syserr27[] = "File too large (EFBIG)";
char __syserr28[] = "No space left on drive (ENOSPC)";
char __syserr29[] = "Invalid seek (ESPIPE)";
char __syserr30[] = "Read-only file system (EROFS)";
char __syserr31[] = "Too many links (EMLINK)";
char __syserr32[] = "Broken pipe (EPIPE)";
char __syserr33[] = "Input to function out of range (EDOM)";
char __syserr34[] = "Output of function out of range (ERANGE)";
char __syserr35[] = "Unknown error: 35";
char __syserr36[] = "Resource deadlock avoided (EDEADLK)";
char __syserr37[] = "Unknown error: 37";
char __syserr38[] = "File name too long (ENAMETOOLONG)";
char __syserr39[] = "No locks available (ENOLCK)";
char __syserr40[] = "Function not implemented (ENOSYS)";
char __syserr41[] = "Directory not empty (ENOTEMPTY)";
char __syserr42[] = "Illegal byte sequence (EILSEQ)";

char *_sys_errlist[] = {
__syserr00, __syserr01, __syserr02, __syserr03, __syserr04,
__syserr05, __syserr06, __syserr07, __syserr08, __syserr09,
__syserr10, __syserr11, __syserr12, __syserr13, __syserr14,
__syserr15, __syserr16, __syserr17, __syserr18, __syserr19,
__syserr20, __syserr21, __syserr22, __syserr23, __syserr24,
__syserr25, __syserr26, __syserr27, __syserr28, __syserr29,
__syserr30, __syserr31, __syserr32, __syserr33, __syserr34,
__syserr35, __syserr36, __syserr37, __syserr38, __syserr39,
__syserr40, __syserr41, __syserr42
};

int _sys_nerr = sizeof(_sys_errlist) / sizeof(_sys_errlist[0]) - 1;

char* CDECL strupr_l(char *str, HANDLE locale)
{
    return str;
}

BOOL msvcrt_init_tls(void)
{
  msvcrt_tls_index = TlsAlloc();

  if (msvcrt_tls_index == TLS_OUT_OF_INDEXES)
  {
    ERR("TlsAlloc() failed!\n");
    return FALSE;
  }
  return TRUE;
}

BOOL msvcrt_free_tls(void)
{
  if (!TlsFree(msvcrt_tls_index))
  {
    ERR("TlsFree() failed!\n");
    return FALSE;
  }
  return TRUE;
}

/* INTERNAL: Map a synonym to an ISO code */
static void remap_synonym(char *name)
{
  unsigned int i;
  for (i = 0; i < sizeof(_country_synonyms)/sizeof(char*); i += 2 )
  {
    if (!strcasecmp(_country_synonyms[i],name))
    {
      TRACE(":Mapping synonym %s to %s\n",name,_country_synonyms[i+1]);
      strcpy(name, _country_synonyms[i+1]);
      return;
    }
  }
}

/* INTERNAL: Get and compare locale info with a given string */
static int compare_info(LCID lcid, DWORD flags, char* buff, const char* cmp, BOOL exact)
{
  int len;

  if(!cmp[0])
      return 0;

  buff[0] = 0;
  GetLocaleInfoA(lcid, flags|LOCALE_NOUSEROVERRIDE, buff, MAX_ELEM_LEN);
  if (!buff[0])
    return 0;

  /* Partial matches are only allowed on language/country names */
  len = strlen(cmp);
  if(exact || len<=3)
    return !strcasecmp(cmp, buff);
  else
    return !strncasecmp(cmp, buff, len);
}

static BOOL CALLBACK
find_best_locale_proc(HMODULE hModule, LPCSTR type, LPCSTR name, WORD LangID, LONG_PTR lParam)
{
  locale_search_t *res = (locale_search_t *)lParam;
  const LCID lcid = MAKELCID(LangID, SORT_DEFAULT);
  char buff[MAX_ELEM_LEN];
  unsigned int flags = 0;

  if(PRIMARYLANGID(LangID) == LANG_NEUTRAL)
    return CONTINUE_LOOKING;

  /* Check Language */
  if (compare_info(lcid,LOCALE_SISO639LANGNAME,buff,res->search_language, TRUE) ||
      compare_info(lcid,LOCALE_SABBREVLANGNAME,buff,res->search_language, TRUE) ||
      compare_info(lcid,LOCALE_SENGLANGUAGE,buff,res->search_language, FALSE))
  {
    TRACE(":Found language: %s->%s\n", res->search_language, buff);
    flags |= FOUND_LANGUAGE;
  }
  else if (res->match_flags & FOUND_LANGUAGE)
  {
    return CONTINUE_LOOKING;
  }

  /* Check Country */
  if (compare_info(lcid,LOCALE_SISO3166CTRYNAME,buff,res->search_country, TRUE) ||
      compare_info(lcid,LOCALE_SABBREVCTRYNAME,buff,res->search_country, TRUE) ||
      compare_info(lcid,LOCALE_SENGCOUNTRY,buff,res->search_country, FALSE))
  {
    TRACE("Found country:%s->%s\n", res->search_country, buff);
    flags |= FOUND_COUNTRY;
  }
  else if (!flags && (res->match_flags & FOUND_COUNTRY))
  {
    return CONTINUE_LOOKING;
  }

  /* Check codepage */
  if (compare_info(lcid,LOCALE_IDEFAULTCODEPAGE,buff,res->search_codepage, TRUE) ||
      (compare_info(lcid,LOCALE_IDEFAULTANSICODEPAGE,buff,res->search_codepage, TRUE)))
  {
    TRACE("Found codepage:%s->%s\n", res->search_codepage, buff);
    flags |= FOUND_CODEPAGE;
    memcpy(res->found_codepage,res->search_codepage,MAX_ELEM_LEN);
  }
  else if (!flags && (res->match_flags & FOUND_CODEPAGE))
  {
    return CONTINUE_LOOKING;
  }

  if (flags > res->match_flags)
  {
    /* Found a better match than previously */
    res->match_flags = flags;
    res->found_lang_id = LangID;
  }
  if ((flags & (FOUND_LANGUAGE | FOUND_COUNTRY | FOUND_CODEPAGE)) ==
        (FOUND_LANGUAGE | FOUND_COUNTRY | FOUND_CODEPAGE))
  {
    TRACE(":found exact locale match\n");
    return STOP_LOOKING;
  }
  return CONTINUE_LOOKING;
}

/* INTERNAL: Set lc_handle, lc_id and lc_category in threadlocinfo struct */
static BOOL update_threadlocinfo_category(LCID lcid, unsigned short cp,
        MSVCRT__locale_t loc, int category)
{
    char buf[256], *p;
    int len;

    if(GetLocaleInfoA(lcid, LOCALE_ILANGUAGE|LOCALE_NOUSEROVERRIDE, buf, 256)) {
        p = buf;

        loc->locinfo->lc_id[category].wLanguage = 0;
        while(*p) {
            loc->locinfo->lc_id[category].wLanguage *= 16;

            if(*p <= '9')
                loc->locinfo->lc_id[category].wLanguage += *p-'0';
            else
                loc->locinfo->lc_id[category].wLanguage += *p-'a'+10;

            p++;
        }

        loc->locinfo->lc_id[category].wCountry =
            loc->locinfo->lc_id[category].wLanguage;
    }

    loc->locinfo->lc_id[category].wCodePage = cp;

    loc->locinfo->lc_handle[category] = lcid;

    len = 0;
    len += GetLocaleInfoA(lcid, LOCALE_SENGLANGUAGE
            |LOCALE_NOUSEROVERRIDE, buf, 256);
    buf[len-1] = '_';
    len += GetLocaleInfoA(lcid, LOCALE_SENGCOUNTRY
            |LOCALE_NOUSEROVERRIDE, &buf[len], 256-len);
    buf[len-1] = '.';
    sprintf(buf+len, "%u", cp);
    len += strlen(buf+len)+1;

    loc->locinfo->lc_category[category].locale = MSVCRT_malloc(len);
    loc->locinfo->lc_category[category].refcount = MSVCRT_malloc(sizeof(int));
    if(!loc->locinfo->lc_category[category].locale
            || !loc->locinfo->lc_category[category].refcount) {
        MSVCRT_free(loc->locinfo->lc_category[category].locale);
        MSVCRT_free(loc->locinfo->lc_category[category].refcount);
        loc->locinfo->lc_category[category].locale = NULL;
        loc->locinfo->lc_category[category].refcount = NULL;
        return TRUE;
    }
    memcpy(loc->locinfo->lc_category[category].locale, buf, len);
    *loc->locinfo->lc_category[category].refcount = 1;

    return FALSE;
}

/* _create_locale - not exported in native msvcrt */
MSVCRT__locale_t CDECL MSVCRT__create_locale(int category, const char *locale)
{
    static const DWORD time_data[] = {
        LOCALE_SABBREVDAYNAME7, LOCALE_SABBREVDAYNAME1, LOCALE_SABBREVDAYNAME2,
        LOCALE_SABBREVDAYNAME3, LOCALE_SABBREVDAYNAME4, LOCALE_SABBREVDAYNAME5,
        LOCALE_SABBREVDAYNAME6,
        LOCALE_SDAYNAME7, LOCALE_SDAYNAME1, LOCALE_SDAYNAME2, LOCALE_SDAYNAME3,
        LOCALE_SDAYNAME4, LOCALE_SDAYNAME5, LOCALE_SDAYNAME6,
        LOCALE_SABBREVMONTHNAME1, LOCALE_SABBREVMONTHNAME2, LOCALE_SABBREVMONTHNAME3,
        LOCALE_SABBREVMONTHNAME4, LOCALE_SABBREVMONTHNAME5, LOCALE_SABBREVMONTHNAME6,
        LOCALE_SABBREVMONTHNAME7, LOCALE_SABBREVMONTHNAME8, LOCALE_SABBREVMONTHNAME9,
        LOCALE_SABBREVMONTHNAME10, LOCALE_SABBREVMONTHNAME11, LOCALE_SABBREVMONTHNAME12,
        LOCALE_SMONTHNAME1, LOCALE_SMONTHNAME2, LOCALE_SMONTHNAME3, LOCALE_SMONTHNAME4,
        LOCALE_SMONTHNAME5, LOCALE_SMONTHNAME6, LOCALE_SMONTHNAME7, LOCALE_SMONTHNAME8,
        LOCALE_SMONTHNAME9, LOCALE_SMONTHNAME10, LOCALE_SMONTHNAME11, LOCALE_SMONTHNAME12,
        LOCALE_S1159, LOCALE_S2359,
        LOCALE_SSHORTDATE, LOCALE_SLONGDATE,
        LOCALE_STIMEFORMAT
    };
    static const char collate[] = "COLLATE=";
    static const char ctype[] = "CTYPE=";
    static const char monetary[] = "MONETARY=";
    static const char numeric[] = "NUMERIC=";
    static const char time[] = "TIME=";
    static const char cloc_short_date[] = "MM/dd/yy";
    static const wchar_t cloc_short_dateW[] = {'M','M','/','d','d','/','y','y',0};
    static const char cloc_long_date[] = "dddd, MMMM dd, yyyy";
    static const wchar_t cloc_long_dateW[] = {'d','d','d','d',',',' ','M','M','M','M',' ','d','d',',',' ','y','y','y','y',0};
    static const char cloc_time[] = "HH:mm:ss";
    static const wchar_t cloc_timeW[] = {'H','H',':','m','m',':','s','s',0};

    MSVCRT__locale_t loc;
    LCID lcid[6] = { 0 }, lcid_tmp;
    unsigned short cp[6] = { 0 };
    char buf[256];
    int i, ret, size;

    TRACE("(%d %s)\n", category, locale);

    if(category<MSVCRT_LC_MIN || category>MSVCRT_LC_MAX || !locale)
        return NULL;

    if(locale[0]=='C' && !locale[1]) {
        lcid[0] = 0;
        cp[0] = CP_ACP;
    } else if(!locale[0]) {
        lcid[0] = GetSystemDefaultLCID();
        GetLocaleInfoA(lcid[0], LOCALE_IDEFAULTANSICODEPAGE
                |LOCALE_NOUSEROVERRIDE, buf, sizeof(buf));
        cp[0] = atoi(buf);

        for(i=1; i<6; i++) {
            lcid[i] = lcid[0];
            cp[i] = cp[0];
        }
    } else if (locale[0] == 'L' && locale[1] == 'C' && locale[2] == '_') {
        const char *p;

        while(1) {
            locale += 3; /* LC_ */
            if(!memcmp(locale, collate, sizeof(collate)-1)) {
                i = MSVCRT_LC_COLLATE;
                locale += sizeof(collate)-1;
            } else if(!memcmp(locale, ctype, sizeof(ctype)-1)) {
                i = MSVCRT_LC_CTYPE;
                locale += sizeof(ctype)-1;
            } else if(!memcmp(locale, monetary, sizeof(monetary)-1)) {
                i = MSVCRT_LC_MONETARY;
                locale += sizeof(monetary)-1;
            } else if(!memcmp(locale, numeric, sizeof(numeric)-1)) {
                i = MSVCRT_LC_NUMERIC;
                locale += sizeof(numeric)-1;
            } else if(!memcmp(locale, time, sizeof(time)-1)) {
                i = MSVCRT_LC_TIME;
                locale += sizeof(time)-1;
            } else
                return NULL;

            p = strchr(locale, ';');
            if(locale[0]=='C' && (locale[1]==';' || locale[1]=='\0')) {
                lcid[i] = 0;
                cp[i] = CP_ACP;
            } else if(p) {
                memcpy(buf, locale, p-locale);
                buf[p-locale] = '\0';
                lcid[i] = MSVCRT_locale_to_LCID(buf, &cp[i]);
            } else
                lcid[i] = MSVCRT_locale_to_LCID(locale, &cp[i]);

            if(lcid[i] == -1)
                return NULL;

            if(!p || *(p+1)!='L' || *(p+2)!='C' || *(p+3)!='_')
                break;

            locale = p+1;
        }
    } else {
        lcid[0] = MSVCRT_locale_to_LCID(locale, &cp[0]);
        if(lcid[0] == -1)
            return NULL;

        for(i=1; i<6; i++) {
            lcid[i] = lcid[0];
            cp[i] = cp[0];
        }
    }

    loc = MSVCRT_malloc(sizeof(MSVCRT__locale_tstruct));
    if(!loc)
        return NULL;

    loc->locinfo = MSVCRT_malloc(sizeof(MSVCRT_threadlocinfo));
    if(!loc->locinfo) {
        MSVCRT_free(loc);
        return NULL;
    }

    loc->mbcinfo = MSVCRT_malloc(sizeof(MSVCRT_threadmbcinfo));
    if(!loc->mbcinfo) {
        MSVCRT_free(loc->locinfo);
        MSVCRT_free(loc);
        return NULL;
    }

    memset(loc->locinfo, 0, sizeof(MSVCRT_threadlocinfo));
    loc->locinfo->refcount = 1;
    loc->mbcinfo->refcount = 1;

    loc->locinfo->lconv = MSVCRT_malloc(sizeof(struct MSVCRT_lconv));
    if(!loc->locinfo->lconv) {
        MSVCRT__free_locale(loc);
        return NULL;
    }
    memset(loc->locinfo->lconv, 0, sizeof(struct MSVCRT_lconv));

    loc->locinfo->pclmap = MSVCRT_malloc(sizeof(char[256]));
    loc->locinfo->pcumap = MSVCRT_malloc(sizeof(char[256]));
    if(!loc->locinfo->pclmap || !loc->locinfo->pcumap) {
        MSVCRT__free_locale(loc);
        return NULL;
    }

    if(lcid[MSVCRT_LC_COLLATE] && (category==MSVCRT_LC_ALL || category==MSVCRT_LC_COLLATE)) {
        if(update_threadlocinfo_category(lcid[MSVCRT_LC_COLLATE], cp[MSVCRT_LC_COLLATE], loc, MSVCRT_LC_COLLATE)) {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        loc->locinfo->lc_collate_cp = loc->locinfo->lc_id[MSVCRT_LC_COLLATE].wCodePage;
    } else
        loc->locinfo->lc_category[LC_COLLATE].locale = _strdup("C");

    if(lcid[MSVCRT_LC_CTYPE] && (category==MSVCRT_LC_ALL || category==MSVCRT_LC_CTYPE)) {
        CPINFO cp_info;
        int j;

        if(update_threadlocinfo_category(lcid[MSVCRT_LC_CTYPE], cp[MSVCRT_LC_CTYPE], loc, MSVCRT_LC_CTYPE)) {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        loc->locinfo->lc_codepage = loc->locinfo->lc_id[MSVCRT_LC_CTYPE].wCodePage;
        loc->locinfo->lc_clike = 1;
        if(!GetCPInfo(loc->locinfo->lc_codepage, &cp_info)) {
            MSVCRT__free_locale(loc);
            return NULL;
        }
        loc->locinfo->mb_cur_max = cp_info.MaxCharSize;

        loc->locinfo->ctype1_refcount = MSVCRT_malloc(sizeof(int));
        loc->locinfo->ctype1 = MSVCRT_malloc(sizeof(short[257]));
        if(!loc->locinfo->ctype1_refcount || !loc->locinfo->ctype1) {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        *loc->locinfo->ctype1_refcount = 1;
        loc->locinfo->ctype1[0] = 0;
        loc->locinfo->pctype = loc->locinfo->ctype1+1;

        buf[1] = buf[2] = '\0';
        for(i=1; i<257; i++) {
            buf[0] = i-1;

            /* builtin GetStringTypeA doesn't set output to 0 on invalid input */
            loc->locinfo->ctype1[i] = 0;

            GetStringTypeA(lcid[MSVCRT_LC_CTYPE], CT_CTYPE1, buf,
                    1, loc->locinfo->ctype1+i);
        }

        for(i=0; cp_info.LeadByte[i+1]!=0; i+=2)
            for(j=cp_info.LeadByte[i]; j<=cp_info.LeadByte[i+1]; j++)
                loc->locinfo->ctype1[j+1] |= _LEADBYTE;
    } else {
        loc->locinfo->lc_clike = 1;
        loc->locinfo->mb_cur_max = 1;
        loc->locinfo->pctype = _ctype+1;
        loc->locinfo->lc_category[LC_CTYPE].locale = _strdup("C");
    }

    for(i=0; i<256; i++) {
        if(loc->locinfo->pctype[i] & _LEADBYTE)
            buf[i] = ' ';
        else
            buf[i] = i;

    }

    if(lcid[MSVCRT_LC_CTYPE]) {
        LCMapStringA(lcid[MSVCRT_LC_CTYPE], LCMAP_LOWERCASE, buf, 256,
                (char*)loc->locinfo->pclmap, 256);
        LCMapStringA(lcid[MSVCRT_LC_CTYPE], LCMAP_UPPERCASE, buf, 256,
                (char*)loc->locinfo->pcumap, 256);
    } else {
        for(i=0; i<256; i++) {
            loc->locinfo->pclmap[i] = (i>='A' && i<='Z' ? i-'A'+'a' : i);
            loc->locinfo->pcumap[i] = (i>='a' && i<='z' ? i-'a'+'A' : i);
        }
    }

    _setmbcp_l(loc->locinfo->lc_id[MSVCRT_LC_CTYPE].wCodePage, lcid[MSVCRT_LC_CTYPE], loc->mbcinfo);

    if(lcid[MSVCRT_LC_MONETARY] && (category==MSVCRT_LC_ALL || category==MSVCRT_LC_MONETARY)) {
        if(update_threadlocinfo_category(lcid[MSVCRT_LC_MONETARY], cp[MSVCRT_LC_MONETARY], loc, MSVCRT_LC_MONETARY)) {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        loc->locinfo->lconv_intl_refcount = MSVCRT_malloc(sizeof(int));
        loc->locinfo->lconv_mon_refcount = MSVCRT_malloc(sizeof(int));
        if(!loc->locinfo->lconv_intl_refcount || !loc->locinfo->lconv_mon_refcount) {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        *loc->locinfo->lconv_intl_refcount = 1;
        *loc->locinfo->lconv_mon_refcount = 1;

        i = GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_SINTLSYMBOL
                |LOCALE_NOUSEROVERRIDE, buf, 256);
        if(i && (loc->locinfo->lconv->int_curr_symbol = MSVCRT_malloc(i)))
            memcpy(loc->locinfo->lconv->int_curr_symbol, buf, i);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        i = GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_SCURRENCY
                |LOCALE_NOUSEROVERRIDE, buf, 256);
        if(i && (loc->locinfo->lconv->currency_symbol = MSVCRT_malloc(i)))
            memcpy(loc->locinfo->lconv->currency_symbol, buf, i);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        i = GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_SMONDECIMALSEP
                |LOCALE_NOUSEROVERRIDE, buf, 256);
        if(i && (loc->locinfo->lconv->mon_decimal_point = MSVCRT_malloc(i)))
            memcpy(loc->locinfo->lconv->mon_decimal_point, buf, i);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        i = GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_SMONTHOUSANDSEP
                |LOCALE_NOUSEROVERRIDE, buf, 256);
        if(i && (loc->locinfo->lconv->mon_thousands_sep = MSVCRT_malloc(i)))
            memcpy(loc->locinfo->lconv->mon_thousands_sep, buf, i);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        i = GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_SMONGROUPING
                |LOCALE_NOUSEROVERRIDE, buf, 256);
        if(i>1)
            i = i/2 + (buf[i-2]=='0'?0:1);
        if(i && (loc->locinfo->lconv->mon_grouping = MSVCRT_malloc(i))) {
            for(i=0; buf[i+1]==';'; i+=2)
                loc->locinfo->lconv->mon_grouping[i/2] = buf[i]-'0';
            loc->locinfo->lconv->mon_grouping[i/2] = buf[i]-'0';
            if(buf[i] != '0')
                loc->locinfo->lconv->mon_grouping[i/2+1] = 127;
        } else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        i = GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_SPOSITIVESIGN
                |LOCALE_NOUSEROVERRIDE, buf, 256);
        if(i && (loc->locinfo->lconv->positive_sign = MSVCRT_malloc(i)))
            memcpy(loc->locinfo->lconv->positive_sign, buf, i);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        i = GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_SNEGATIVESIGN
                |LOCALE_NOUSEROVERRIDE, buf, 256);
        if(i && (loc->locinfo->lconv->negative_sign = MSVCRT_malloc(i)))
            memcpy(loc->locinfo->lconv->negative_sign, buf, i);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        if(GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_IINTLCURRDIGITS
                    |LOCALE_NOUSEROVERRIDE, buf, 256))
            loc->locinfo->lconv->int_frac_digits = atoi(buf);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        if(GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_ICURRDIGITS
                    |LOCALE_NOUSEROVERRIDE, buf, 256))
            loc->locinfo->lconv->frac_digits = atoi(buf);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        if(GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_IPOSSYMPRECEDES
                    |LOCALE_NOUSEROVERRIDE, buf, 256))
            loc->locinfo->lconv->p_cs_precedes = atoi(buf);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        if(GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_IPOSSEPBYSPACE
                    |LOCALE_NOUSEROVERRIDE, buf, 256))
            loc->locinfo->lconv->p_sep_by_space = atoi(buf);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        if(GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_INEGSYMPRECEDES
                    |LOCALE_NOUSEROVERRIDE, buf, 256))
            loc->locinfo->lconv->n_cs_precedes = atoi(buf);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        if(GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_INEGSEPBYSPACE
                    |LOCALE_NOUSEROVERRIDE, buf, 256))
            loc->locinfo->lconv->n_sep_by_space = atoi(buf);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        if(GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_IPOSSIGNPOSN
                    |LOCALE_NOUSEROVERRIDE, buf, 256))
            loc->locinfo->lconv->p_sign_posn = atoi(buf);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        if(GetLocaleInfoA(lcid[MSVCRT_LC_MONETARY], LOCALE_INEGSIGNPOSN
                    |LOCALE_NOUSEROVERRIDE, buf, 256))
            loc->locinfo->lconv->n_sign_posn = atoi(buf);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }
    } else {
        loc->locinfo->lconv->int_curr_symbol = MSVCRT_malloc(sizeof(char));
        loc->locinfo->lconv->currency_symbol = MSVCRT_malloc(sizeof(char));
        loc->locinfo->lconv->mon_decimal_point = MSVCRT_malloc(sizeof(char));
        loc->locinfo->lconv->mon_thousands_sep = MSVCRT_malloc(sizeof(char));
        loc->locinfo->lconv->mon_grouping = MSVCRT_malloc(sizeof(char));
        loc->locinfo->lconv->positive_sign = MSVCRT_malloc(sizeof(char));
        loc->locinfo->lconv->negative_sign = MSVCRT_malloc(sizeof(char));

        if(!loc->locinfo->lconv->int_curr_symbol || !loc->locinfo->lconv->currency_symbol
                || !loc->locinfo->lconv->mon_decimal_point || !loc->locinfo->lconv->mon_thousands_sep
                || !loc->locinfo->lconv->mon_grouping || !loc->locinfo->lconv->positive_sign
                || !loc->locinfo->lconv->negative_sign) {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        loc->locinfo->lconv->int_curr_symbol[0] = '\0';
        loc->locinfo->lconv->currency_symbol[0] = '\0';
        loc->locinfo->lconv->mon_decimal_point[0] = '\0';
        loc->locinfo->lconv->mon_thousands_sep[0] = '\0';
        loc->locinfo->lconv->mon_grouping[0] = '\0';
        loc->locinfo->lconv->positive_sign[0] = '\0';
        loc->locinfo->lconv->negative_sign[0] = '\0';
        loc->locinfo->lconv->int_frac_digits = 127;
        loc->locinfo->lconv->frac_digits = 127;
        loc->locinfo->lconv->p_cs_precedes = 127;
        loc->locinfo->lconv->p_sep_by_space = 127;
        loc->locinfo->lconv->n_cs_precedes = 127;
        loc->locinfo->lconv->n_sep_by_space = 127;
        loc->locinfo->lconv->p_sign_posn = 127;
        loc->locinfo->lconv->n_sign_posn = 127;

        loc->locinfo->lc_category[LC_MONETARY].locale = _strdup("C");
    }

    if(lcid[MSVCRT_LC_NUMERIC] && (category==MSVCRT_LC_ALL || category==MSVCRT_LC_NUMERIC)) {
        if(update_threadlocinfo_category(lcid[MSVCRT_LC_NUMERIC], cp[MSVCRT_LC_NUMERIC], loc, MSVCRT_LC_NUMERIC)) {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        if(!loc->locinfo->lconv_intl_refcount)
            loc->locinfo->lconv_intl_refcount = MSVCRT_malloc(sizeof(int));
        loc->locinfo->lconv_num_refcount = MSVCRT_malloc(sizeof(int));
        if(!loc->locinfo->lconv_intl_refcount || !loc->locinfo->lconv_num_refcount) {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        *loc->locinfo->lconv_intl_refcount = 1;
        *loc->locinfo->lconv_num_refcount = 1;

        i = GetLocaleInfoA(lcid[MSVCRT_LC_NUMERIC], LOCALE_SDECIMAL
                |LOCALE_NOUSEROVERRIDE, buf, 256);
        if(i && (loc->locinfo->lconv->decimal_point = MSVCRT_malloc(i)))
            memcpy(loc->locinfo->lconv->decimal_point, buf, i);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        i = GetLocaleInfoA(lcid[MSVCRT_LC_NUMERIC], LOCALE_STHOUSAND
                |LOCALE_NOUSEROVERRIDE, buf, 256);
        if(i && (loc->locinfo->lconv->thousands_sep = MSVCRT_malloc(i)))
            memcpy(loc->locinfo->lconv->thousands_sep, buf, i);
        else {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        i = GetLocaleInfoA(lcid[MSVCRT_LC_NUMERIC], LOCALE_SGROUPING
                |LOCALE_NOUSEROVERRIDE, buf, 256);
        if(i>1)
            i = i/2 + (buf[i-2]=='0'?0:1);
        if(i && (loc->locinfo->lconv->grouping = MSVCRT_malloc(i))) {
            for(i=0; buf[i+1]==';'; i+=2)
                loc->locinfo->lconv->grouping[i/2] = buf[i]-'0';
            loc->locinfo->lconv->grouping[i/2] = buf[i]-'0';
            if(buf[i] != '0')
                loc->locinfo->lconv->grouping[i/2+1] = 127;
        } else {
            MSVCRT__free_locale(loc);
            return NULL;
        }
    } else {
        loc->locinfo->lconv->decimal_point = MSVCRT_malloc(sizeof(char[2]));
        loc->locinfo->lconv->thousands_sep = MSVCRT_malloc(sizeof(char));
        loc->locinfo->lconv->grouping = MSVCRT_malloc(sizeof(char));
        if(!loc->locinfo->lconv->decimal_point || !loc->locinfo->lconv->thousands_sep
                || !loc->locinfo->lconv->grouping) {
            MSVCRT__free_locale(loc);
            return NULL;
        }

        loc->locinfo->lconv->decimal_point[0] = '.';
        loc->locinfo->lconv->decimal_point[1] = '\0';
        loc->locinfo->lconv->thousands_sep[0] = '\0';
        loc->locinfo->lconv->grouping[0] = '\0';

        loc->locinfo->lc_category[LC_NUMERIC].locale = _strdup("C");
    }

    if(lcid[MSVCRT_LC_TIME] && (category==MSVCRT_LC_ALL || category==MSVCRT_LC_TIME)) {
        if(update_threadlocinfo_category(lcid[MSVCRT_LC_TIME], cp[MSVCRT_LC_TIME], loc, MSVCRT_LC_TIME)) {
            MSVCRT__free_locale(loc);
            return NULL;
        }
    } else
        loc->locinfo->lc_category[LC_TIME].locale = _strdup("C");

    size = sizeof(MSVCRT___lc_time_data);
    lcid_tmp = lcid[MSVCRT_LC_TIME] ? lcid[MSVCRT_LC_TIME] : MAKELCID(LANG_ENGLISH, SORT_DEFAULT);
    for(i=0; i<sizeof(time_data)/sizeof(time_data[0]); i++) {
        if(time_data[i]==LOCALE_SSHORTDATE && !lcid[MSVCRT_LC_TIME]) {
            size += sizeof(cloc_short_date) + sizeof(cloc_short_dateW);
        }else if(time_data[i]==LOCALE_SLONGDATE && !lcid[MSVCRT_LC_TIME]) {
            size += sizeof(cloc_long_date) + sizeof(cloc_long_dateW);
        }else {
            ret = GetLocaleInfoA(lcid_tmp, time_data[i]
                    |LOCALE_NOUSEROVERRIDE, NULL, 0);
            if(!ret) {
                MSVCRT__free_locale(loc);
                return NULL;
            }
            size += ret;

            ret = GetLocaleInfoW(lcid_tmp, time_data[i]
                    |LOCALE_NOUSEROVERRIDE, NULL, 0);
            if(!ret) {
                MSVCRT__free_locale(loc);
                return NULL;
            }
            size += ret*sizeof(wchar_t);
        }
    }

    loc->locinfo->lc_time_curr = MSVCRT_malloc(size);
    if(!loc->locinfo->lc_time_curr) {
        MSVCRT__free_locale(loc);
        return NULL;
    }

    ret = 0;
    for(i=0; i<sizeof(time_data)/sizeof(time_data[0]); i++) {
        loc->locinfo->lc_time_curr->str.str[i] = &loc->locinfo->lc_time_curr->data[ret];
        if(time_data[i]==LOCALE_SSHORTDATE && !lcid[MSVCRT_LC_TIME]) {
            memcpy(&loc->locinfo->lc_time_curr->data[ret], cloc_short_date, sizeof(cloc_short_date));
            ret += sizeof(cloc_short_date);
        }else if(time_data[i]==LOCALE_SLONGDATE && !lcid[MSVCRT_LC_TIME]) {
            memcpy(&loc->locinfo->lc_time_curr->data[ret], cloc_long_date, sizeof(cloc_long_date));
            ret += sizeof(cloc_long_date);
        }else if(time_data[i]==LOCALE_STIMEFORMAT && !lcid[MSVCRT_LC_TIME]) {
            memcpy(&loc->locinfo->lc_time_curr->data[ret], cloc_time, sizeof(cloc_time));
            ret += sizeof(cloc_time);
        }else {
            ret += GetLocaleInfoA(lcid_tmp, time_data[i]|LOCALE_NOUSEROVERRIDE,
                    &loc->locinfo->lc_time_curr->data[ret], size-ret);
        }
    }
    for(i=0; i<sizeof(time_data)/sizeof(time_data[0]); i++) {
        loc->locinfo->lc_time_curr->wstr[i] = (wchar_t*)&loc->locinfo->lc_time_curr->data[ret];
        if(time_data[i]==LOCALE_SSHORTDATE && !lcid[MSVCRT_LC_TIME]) {
            memcpy(&loc->locinfo->lc_time_curr->data[ret], cloc_short_dateW, sizeof(cloc_short_dateW));
            ret += sizeof(cloc_short_dateW);
        }else if(time_data[i]==LOCALE_SLONGDATE && !lcid[MSVCRT_LC_TIME]) {
            memcpy(&loc->locinfo->lc_time_curr->data[ret], cloc_long_dateW, sizeof(cloc_long_dateW));
            ret += sizeof(cloc_long_dateW);
        }else if(time_data[i]==LOCALE_STIMEFORMAT && !lcid[MSVCRT_LC_TIME]) {            memcpy(&loc->locinfo->lc_time_curr->data[ret], cloc_timeW, sizeof(cloc_timeW));
            ret += sizeof(cloc_timeW);
        }else {
            ret += GetLocaleInfoW(lcid_tmp, time_data[i]|LOCALE_NOUSEROVERRIDE,
                    (wchar_t*)&loc->locinfo->lc_time_curr->data[ret], size-ret)*sizeof(wchar_t);
        }
    }
    loc->locinfo->lc_time_curr->lcid = lcid[MSVCRT_LC_TIME];

    return loc;
}

/*********************************************************************
 *              _towlower_l (MSVCRT.@)
 */
int CDECL MSVCRT__towlower_l(wint_t c, HANDLE locale)
{
    return 0;
}

/*********************************************************************
 *              _towupper_l (MSVCRT.@)
 */
int CDECL MSVCRT__towupper_l(wint_t c, HANDLE locale)
{
    return 0;
}

/******************************************************************
 *		_wcslwr_l (MSVCRT.@)
 */
wchar_t* CDECL MSVCRT__wcslwr_l(wchar_t* str, _locale_t locale )
{
    return str;
}

/******************************************************************
 *              _wcsupr_l (MSVCRT.@)
 */
wchar_t* CDECL MSVCRT__wcsupr_l(wchar_t *str, _locale_t locale )
{
    return str;
}

/*********************************************************************
 *		_abs64 (MSVCRT.@)
 */
__int64 CDECL abs64( __int64 n )
{
    return n >= 0 ? n : -n;
}

/*********************************************************************
 * _byteswap_uint64 (MSVCR100.@)
 */
unsigned __int64 CDECL byteswap_uint64(unsigned __int64 i)
{
    return (i<<56) + ((i&0xFF00)<<40) + ((i&0xFF0000)<<24) + ((i&0xFF000000)<<8) +
        ((i>>8)&0xFF000000) + ((i>>24)&0xFF0000) + ((i>>40)&0xFF00) + (i>>56);
}

/*********************************************************************
 * _byteswap_ulong (MSVCR100.@)
 */
ULONG CDECL byteswap_ulong(ULONG l)
{
    return (l<<24) + ((l<<8)&0xFF0000) + ((l>>8)&0xFF00) + (l>>24);
}

/*********************************************************************
 * _byteswap_ushort (MSVCR100.@)
 */
unsigned short CDECL byteswap_ushort(unsigned short s)
{
    return (s<<8) + (s>>8);
}

/*********************************************************************
 *		_rotl64 (MSVCRT.@)
 */
unsigned __int64 CDECL rotl64(unsigned __int64 num, int shift)
{
  shift &= 63;
  return (num << shift) | (num >> (64-shift));
}

/*********************************************************************
 *		_rotr64 (MSVCRT.@)
 */
unsigned __int64 CDECL rotr64(unsigned __int64 num, int shift)
{
    shift &= 63;
    return (num >> shift) | (num << (64-shift));
}

/*********************************************************************
 *		strtok_s  (MSVCRT.@)
 */
char * CDECL strtok_s(char *str, const char *delim, char **ctx)
{
    if (!MSVCRT_CHECK_PMT(delim != NULL)) return NULL;
    if (!MSVCRT_CHECK_PMT(ctx != NULL)) return NULL;
    if (!MSVCRT_CHECK_PMT(str != NULL || *ctx != NULL)) return NULL;

    if(!str)
        str = *ctx;

    while(*str && strchr(delim, *str))
        str++;
    if(!*str)
    {
        *ctx = str;
        return NULL;
    }

    *ctx = str+1;
    while(**ctx && !strchr(delim, **ctx))
        (*ctx)++;
    if(**ctx)
        *(*ctx)++ = 0;

    return str;
}


/*********************************************************************
 *		_wcserror_s (MSVCRT.@)
 */
int CDECL _wcserror_s(wchar_t* buffer, size_t nc, int err)
{
    if (!MSVCRT_CHECK_PMT(buffer != NULL) || !MSVCRT_CHECK_PMT(nc > 0))
    {
        _set_errno(EINVAL);
        return EINVAL;
    }
    if (err < 0 || err > _sys_nerr) err = _sys_nerr;
    MultiByteToWideChar(CP_ACP, 0, _sys_errlist[err], -1, buffer, nc);
    return 0;
}

/*********************************************************************
 *		_errno (MSVCRT.@)
 */
int* CDECL _errno(void)
{
    return &(msvcrt_get_thread_data()->thread_errno);
}

thread_data_t *msvcrt_get_thread_data(void)
{
    thread_data_t *ptr;
    DWORD err = GetLastError();  /* need to preserve last error */

    if (!(ptr = TlsGetValue( msvcrt_tls_index )))
    {
        if (!(ptr = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*ptr) )))
            _amsg_exit( _RT_THREAD );
        if (!TlsSetValue( msvcrt_tls_index, ptr )) _amsg_exit( _RT_THREAD );
        ptr->tid = GetCurrentThreadId();
        ptr->handle = INVALID_HANDLE_VALUE;
        ptr->random_seed = 1;
        ptr->locinfo = MSVCRT_locale->locinfo;
        ptr->mbcinfo = MSVCRT_locale->mbcinfo;
    }
    SetLastError( err );
    return ptr;
}

/* LIBRARY ENTRY POINT ********************************************************/

BOOL
WINAPI
DllMain(PVOID hinstDll, ULONG dwReason, PVOID reserved)
{
    switch (dwReason){
	case DLL_PROCESS_ATTACH:	
        /* create tls stuff */
        if (!msvcrt_init_tls())
          return FALSE;	
		break;
	case DLL_THREAD_DETACH:
        msvcrt_free_tls_mem();
        break;		
	}
    return TRUE;
}

MSVCRT__locale_t global_locale = NULL;
void __init_global_locale()
{
    unsigned i;

    LOCK_LOCALE;
    /* Someone created it before us */
    if(global_locale)
        return;
    global_locale = MSVCRT__create_locale(0, "C");

    __lc_codepage = MSVCRT_locale->locinfo->lc_codepage;
    MSVCRT___lc_collate_cp = MSVCRT_locale->locinfo->lc_collate_cp;
    __mb_cur_max = MSVCRT_locale->locinfo->mb_cur_max;
    for(i=LC_MIN; i<=LC_MAX; i++)
        MSVCRT___lc_handle[i] = MSVCRT_locale->locinfo->lc_handle[i];
    _setmbcp(_MB_CP_ANSI);
    UNLOCK_LOCALE;
}


/*********************************************************************
 * INTERNAL: _setmbcp_l
 */
int _setmbcp_l(int cp, LCID lcid, MSVCRT_pthreadmbcinfo mbcinfo)
{
  const char format[] = ".%d";

  int newcp;
  CPINFO cpi;
  BYTE *bytes;
  WORD chartypes[256];
  char bufA[256];
  WCHAR bufW[256];
  int charcount;
  int ret;
  int i;

  if(!mbcinfo)
      mbcinfo = get_mbcinfo();

  switch (cp)
  {
    case _MB_CP_ANSI:
      newcp = GetACP();
      break;
    case _MB_CP_OEM:
      newcp = GetOEMCP();
      break;
    case _MB_CP_LOCALE:
      newcp = get_locinfo()->lc_codepage;
      if(newcp)
          break;
      /* fall through (C locale) */
    case _MB_CP_SBCS:
      newcp = 20127;   /* ASCII */
      break;
    default:
      newcp = cp;
      break;
  }

  if(lcid == -1) {
    sprintf(bufA, format, newcp);
    mbcinfo->mblcid = MSVCRT_locale_to_LCID(bufA, NULL);
  } else {
    mbcinfo->mblcid = lcid;
  }

  if(mbcinfo->mblcid == -1)
  {
    WARN("Can't assign LCID to codepage (%d)\n", mbcinfo->mblcid);
    mbcinfo->mblcid = 0;
  }

  if (!GetCPInfo(newcp, &cpi))
  {
    WARN("Codepage %d not found\n", newcp);
    *_errno() = EINVAL;
    return -1;
  }

  /* setup the _mbctype */
  memset(mbcinfo->mbctype, 0, sizeof(unsigned char[257]));
  memset(mbcinfo->mbcasemap, 0, sizeof(unsigned char[256]));

  bytes = cpi.LeadByte;
  while (bytes[0] || bytes[1])
  {
    for (i = bytes[0]; i <= bytes[1]; i++)
      mbcinfo->mbctype[i + 1] |= _M1;
    bytes += 2;
  }

  if (cpi.MaxCharSize > 1)
  {
    /* trail bytes not available through kernel32 but stored in a structure in msvcrt */
    struct cp_extra_info_t *cpextra = g_cpextrainfo;

    mbcinfo->ismbcodepage = 1;
    while (TRUE)
    {
      if (cpextra->cp == 0 || cpextra->cp == newcp)
      {
        if (cpextra->cp == 0)
          FIXME("trail bytes data not available for DBCS codepage %d - assuming all bytes\n", newcp);

        bytes = cpextra->TrailBytes;
        while (bytes[0] || bytes[1])
        {
          for (i = bytes[0]; i <= bytes[1]; i++)
            mbcinfo->mbctype[i + 1] |= _M2;
          bytes += 2;
        }
        break;
      }
      cpextra++;
    }
  }
  else
    mbcinfo->ismbcodepage = 0;

  /* we can't use GetStringTypeA directly because we don't have a locale - only a code page
   */
  charcount = 0;
  for (i = 0; i < 256; i++)
    if (!(mbcinfo->mbctype[i + 1] & _M1))
      bufA[charcount++] = i;

  ret = MultiByteToWideChar(newcp, 0, bufA, charcount, bufW, charcount);
  if (ret != charcount)
    ERR("MultiByteToWideChar of chars failed for cp %d, ret=%d (exp %d), error=%d\n", newcp, ret, charcount, GetLastError());

  GetStringTypeW(CT_CTYPE1, bufW, charcount, chartypes);

  charcount = 0;
  for (i = 0; i < 256; i++)
    if (!(mbcinfo->mbctype[i + 1] & _M1))
    {
      if (chartypes[charcount] & C1_UPPER)
      {
        mbcinfo->mbctype[i + 1] |= _SBUP;
        bufW[charcount] = tolowerW(bufW[charcount]);
      }
      else if (chartypes[charcount] & C1_LOWER)
      {
	mbcinfo->mbctype[i + 1] |= _SBLOW;
        bufW[charcount] = toupperW(bufW[charcount]);
      }
      charcount++;
    }

  ret = WideCharToMultiByte(newcp, 0, bufW, charcount, bufA, charcount, NULL, NULL);
  if (ret != charcount)
    ERR("WideCharToMultiByte failed for cp %d, ret=%d (exp %d), error=%d\n", newcp, ret, charcount, GetLastError());

  charcount = 0;
  for (i = 0; i < 256; i++)
  {
    if(!(mbcinfo->mbctype[i + 1] & _M1))
    {
      if(mbcinfo->mbctype[i] & (C1_UPPER|C1_LOWER))
        mbcinfo->mbcasemap[i] = bufA[charcount];
      charcount++;
    }
  }

  if (newcp == 932)   /* CP932 only - set _MP and _MS */
  {
    /* On Windows it's possible to calculate the _MP and _MS from CT_CTYPE1
     * and CT_CTYPE3. But as of Wine 0.9.43 we return wrong values what makes
     * it hard. As this is set only for codepage 932 we hardcode it what gives
     * also faster execution.
     */
    for (i = 161; i <= 165; i++)
      mbcinfo->mbctype[i + 1] |= _MP;
    for (i = 166; i <= 223; i++)
      mbcinfo->mbctype[i + 1] |= _MS;
  }

  mbcinfo->mbcodepage = newcp;
  if(global_locale && mbcinfo == MSVCRT_locale->mbcinfo)
    memcpy(_mbctype, MSVCRT_locale->mbcinfo->mbctype, sizeof(_mbctype));

  return 0;
}

/* INTERNAL: returns pthreadlocinfo struct */
MSVCRT_pthreadmbcinfo get_mbcinfo(void) {
    thread_data_t *data = msvcrt_get_thread_data();

    if(!data || !data->have_locale)
        return MSVCRT_locale->mbcinfo;

    return data->mbcinfo;
}


/* Internal: Find the LCID for a locale specification */
LCID MSVCRT_locale_to_LCID(const char *locale, unsigned short *codepage)
{
    LCID lcid;
    locale_search_t search;
    const char *cp, *region;

    memset(&search, 0, sizeof(locale_search_t));

    cp = strchr(locale, '.');
    region = strchr(locale, '_');

    lstrcpynA(search.search_language, locale, MAX_ELEM_LEN);
    if(region) {
        lstrcpynA(search.search_country, region+1, MAX_ELEM_LEN);
        if(region-locale < MAX_ELEM_LEN)
            search.search_language[region-locale] = '\0';
    } else
        search.search_country[0] = '\0';

    if(cp) {
        lstrcpynA(search.search_codepage, cp+1, MAX_ELEM_LEN);
        if(region && cp-region-1<MAX_ELEM_LEN)
          search.search_country[cp-region-1] = '\0';
        if(cp-locale < MAX_ELEM_LEN)
            search.search_language[cp-locale] = '\0';
    } else
        search.search_codepage[0] = '\0';

    if(!search.search_country[0] && !search.search_codepage[0])
        remap_synonym(search.search_language);

    EnumResourceLanguagesA(GetModuleHandleA("KERNEL32"), (LPSTR)RT_STRING,
            (LPCSTR)LOCALE_ILANGUAGE,find_best_locale_proc,
            (LONG_PTR)&search);

    if (!search.match_flags)
        return -1;

    /* If we were given something that didn't match, fail */
    if (search.search_country[0] && !(search.match_flags & FOUND_COUNTRY))
        return -1;

    lcid =  MAKELCID(search.found_lang_id, SORT_DEFAULT);

    /* Populate partial locale, translating LCID to locale string elements */
    if (!(search.match_flags & FOUND_CODEPAGE)) {
        /* Even if a codepage is not enumerated for a locale
         * it can be set if valid */
        if (search.search_codepage[0]) {
            if (IsValidCodePage(atoi(search.search_codepage)))
                memcpy(search.found_codepage,search.search_codepage,MAX_ELEM_LEN);
            else {
                /* Special codepage values: OEM & ANSI */
                if (!strcasecmp(search.search_codepage,"OCP")) {
                    GetLocaleInfoA(lcid, LOCALE_IDEFAULTCODEPAGE,
                            search.found_codepage, MAX_ELEM_LEN);
                } else if (!strcasecmp(search.search_codepage,"ACP")) {
                    GetLocaleInfoA(lcid, LOCALE_IDEFAULTANSICODEPAGE,
                            search.found_codepage, MAX_ELEM_LEN);
                } else
                    return -1;

                if (!atoi(search.found_codepage))
                    return -1;
            }
        } else {
            /* Prefer ANSI codepages if present */
            GetLocaleInfoA(lcid, LOCALE_IDEFAULTANSICODEPAGE,
                    search.found_codepage, MAX_ELEM_LEN);
            if (!search.found_codepage[0] || !atoi(search.found_codepage))
                GetLocaleInfoA(lcid, LOCALE_IDEFAULTCODEPAGE,
                        search.found_codepage, MAX_ELEM_LEN);
        }
    }
    if (codepage)
        *codepage = atoi(search.found_codepage);

    return lcid;
}

/* INTERNAL: returns pthreadlocinfo struct */
MSVCRT_pthreadlocinfo get_locinfo(void) {
    thread_data_t *data = msvcrt_get_thread_data();

    if(!data || !data->have_locale)
        return MSVCRT_locale->locinfo;

    return data->locinfo;
}


/* INTERNAL: frees MSVCRT_pthreadlocinfo struct */
void free_locinfo(MSVCRT_pthreadlocinfo locinfo)
{
    int i;

    if(!locinfo)
        return;

    if(InterlockedDecrement(&locinfo->refcount))
        return;

    for(i=MSVCRT_LC_MIN+1; i<=MSVCRT_LC_MAX; i++) {
        MSVCRT_free(locinfo->lc_category[i].locale);
        MSVCRT_free(locinfo->lc_category[i].refcount);
    }

    if(locinfo->lconv) {
        MSVCRT_free(locinfo->lconv->decimal_point);
        MSVCRT_free(locinfo->lconv->thousands_sep);
        MSVCRT_free(locinfo->lconv->grouping);
        MSVCRT_free(locinfo->lconv->int_curr_symbol);
        MSVCRT_free(locinfo->lconv->currency_symbol);
        MSVCRT_free(locinfo->lconv->mon_decimal_point);
        MSVCRT_free(locinfo->lconv->mon_thousands_sep);
        MSVCRT_free(locinfo->lconv->mon_grouping);
        MSVCRT_free(locinfo->lconv->positive_sign);
        MSVCRT_free(locinfo->lconv->negative_sign);
    }
    MSVCRT_free(locinfo->lconv_intl_refcount);
    MSVCRT_free(locinfo->lconv_num_refcount);
    MSVCRT_free(locinfo->lconv_mon_refcount);
    MSVCRT_free(locinfo->lconv);

    MSVCRT_free(locinfo->ctype1_refcount);
    MSVCRT_free(locinfo->ctype1);

    MSVCRT_free(locinfo->pclmap);
    MSVCRT_free(locinfo->pcumap);

    MSVCRT_free(locinfo->lc_time_curr);

    MSVCRT_free(locinfo);
}

/* _free_locale - not exported in native msvcrt */
void CDECL MSVCRT__free_locale(MSVCRT__locale_t locale)
{
    if (!locale)
        return;

    free_locinfo(locale->locinfo);
    free_mbcinfo(locale->mbcinfo);
    MSVCRT_free(locale);
}

/* INTERNAL: frees MSVCRT_pthreadmbcinfo struct */
void free_mbcinfo(MSVCRT_pthreadmbcinfo mbcinfo)
{
    if(!mbcinfo)
        return;

    if(InterlockedDecrement(&mbcinfo->refcount))
        return;

    MSVCRT_free(mbcinfo);
}

void msvcrt_free_tls_mem(void)
{
  thread_data_t *tls = TlsGetValue(msvcrt_tls_index);

  if (tls)
  {
    CloseHandle(tls->handle);
    HeapFree(GetProcessHeap(),0,tls->efcvt_buffer);
    HeapFree(GetProcessHeap(),0,tls->asctime_buffer);
    HeapFree(GetProcessHeap(),0,tls->wasctime_buffer);
    HeapFree(GetProcessHeap(),0,tls->strerror_buffer);
    HeapFree(GetProcessHeap(),0,tls->wcserror_buffer);
    HeapFree(GetProcessHeap(),0,tls->time_buffer);
    //if(tls->have_locale) {
    //    free_locinfo(tls->locinfo);
    //    free_mbcinfo(tls->mbcinfo);
    //}
  }
  HeapFree(GetProcessHeap(), 0, tls);
}

/******************************************************************
 *		wcsncpy_s (MSVCRT.@)
 */
INT CDECL wcsncpy_s( wchar_t* wcDest, size_t numElement, const wchar_t *wcSrc,
                            size_t count )
{
    size_t size = 0;

    if (!wcDest || !numElement)
        return EINVAL;

    wcDest[0] = 0;

    if (!wcSrc)
    {
        return EINVAL;
    }

    size = min(strlenW(wcSrc), count);

    if (size >= numElement)
    {
        return ERANGE;
    }

    memcpy( wcDest, wcSrc, size*sizeof(WCHAR) );
    wcDest[size] = '\0';

    return 0;
}