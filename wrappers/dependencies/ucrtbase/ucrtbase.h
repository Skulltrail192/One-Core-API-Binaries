/*
 * Copyright 2001 Jon Griffiths
 * Copyright 2004 Dimitrie O. Paun
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
 * NOTES
 *   Naming conventions
 *	- Symbols are prefixed with MSVCRT_ if they conflict
 *        with libc symbols
 *      - Internal symbols are usually prefixed by msvcrt_.
 *      - Exported symbols that are not present in the public
 *        headers are usually kept the same as the original.
 *   Other conventions
 *      - To avoid conflicts with the standard C library,
 *        no msvcrt headers are included in the implementation.
 *      - Instead, symbols are duplicated here, prefixed with 
 *        MSVCRT_, as explained above.
 *      - To avoid inconsistencies, a test for each symbol is
 *        added into tests/headers.c. Please always add a
 *        corresponding test when you add a new symbol!
 */

#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>
#include <winternl.h>
#include <internal/wine/msvcrt.h>
#include <windows.h>
#include <stdio.h>
#include <safecrt.h>

#include <internal/rterror.h>
#include <internal/locale.h>
#include <internal/mtdll.h>
#include <locale.h>
#include <mbctype.h>
#include <unicode.h>

#define MSVCRT_INT_MAX     0x7fffffff
#define MSVCRT_LONG_MAX    0x7fffffff
#define MSVCRT_LONG_MIN    (-MSVCRT_LONG_MAX-1)
#define MSVCRT_ULONG_MAX   0xffffffff
#define MSVCRT_I64_MAX    (((__int64)0x7fffffff << 32) | 0xffffffff)
#define MSVCRT_I64_MIN    (-MSVCRT_I64_MAX-1)
#define MSVCRT_UI64_MAX   (((unsigned __int64)0xffffffff << 32) | 0xffffffff)
#define MSVCRT_MB_LEN_MAX 5
#define MSVCRT_FLT_MAX_10_EXP 38
#define MSVCRT_FLT_MIN_10_EXP (-37)
#define MSVCRT_DBL_MAX_10_EXP 308
#define MSVCRT_DBL_MIN_10_EXP (-307)
#define DBL80_MAX_10_EXP 4932
#define DBL80_MIN_10_EXP -4951
#define MSVCRT_DBL_DIG 15
#ifdef _WIN64
#define MSVCRT_SIZE_MAX MSVCRT_UI64_MAX
#else
#define MSVCRT_SIZE_MAX MSVCRT_ULONG_MAX
#endif

#define MSVCRT_EPERM   1
#define MSVCRT_ENOENT  2
#define MSVCRT_ESRCH   3
#define MSVCRT_EINTR   4
#define MSVCRT_EIO     5
#define MSVCRT_ENXIO   6
#define MSVCRT_E2BIG   7
#define MSVCRT_ENOEXEC 8
#define MSVCRT_EBADF   9
#define MSVCRT_ECHILD  10
#define MSVCRT_EAGAIN  11
#define MSVCRT_ENOMEM  12
#define MSVCRT_EACCES  13
#define MSVCRT_EFAULT  14
#define MSVCRT_EBUSY   16
#define MSVCRT_EEXIST  17
#define MSVCRT_EXDEV   18
#define MSVCRT_ENODEV  19
#define MSVCRT_ENOTDIR 20
#define MSVCRT_EISDIR  21
#define MSVCRT_EINVAL  22
#define MSVCRT_ENFILE  23
#define MSVCRT_EMFILE  24
#define MSVCRT_ENOTTY  25
#define MSVCRT_EFBIG   27
#define MSVCRT_ENOSPC  28
#define MSVCRT_ESPIPE  29
#define MSVCRT_EROFS   30
#define MSVCRT_EMLINK  31
#define MSVCRT_EPIPE   32
#define MSVCRT_EDOM    33
#define MSVCRT_ERANGE  34
#define MSVCRT_EDEADLK 36
#define MSVCRT_ENOTEMPTY 41
#define MSVCRT_EILSEQ    42


typedef unsigned char  MSVCRT_bool;
typedef unsigned short MSVCRT_wchar_t;
typedef unsigned short MSVCRT_wint_t;
typedef unsigned short MSVCRT_wctrans_t;
typedef unsigned short MSVCRT_wctype_t;
typedef unsigned short MSVCRT__ino_t;
typedef unsigned int   MSVCRT__fsize_t;
typedef int            MSVCRT_long;
typedef unsigned int   MSVCRT_ulong;
typedef __int64        MSVCRT_longlong;
typedef long long      MSVCRT_intmax_t;
#ifdef _WIN64
typedef unsigned __int64 MSVCRT_size_t;
typedef __int64 MSVCRT_intptr_t;
typedef unsigned __int64 MSVCRT_uintptr_t;
#else
typedef unsigned long MSVCRT_size_t;
typedef long MSVCRT_intptr_t;
typedef unsigned long MSVCRT_uintptr_t;
#endif
#ifdef _CRTDLL
typedef short MSVCRT__dev_t;
#else
typedef unsigned int   MSVCRT__dev_t;
#endif
typedef int MSVCRT__off_t;
typedef int MSVCRT_clock_t;
typedef int MSVCRT___time32_t;
typedef __int64 DECLSPEC_ALIGN(8) MSVCRT___time64_t;
typedef __int64 DECLSPEC_ALIGN(8) MSVCRT_fpos_t;
typedef int MSVCRT_mbstate_t;

typedef void (__cdecl *MSVCRT_terminate_handler)(void);
typedef void (__cdecl *MSVCRT_terminate_function)(void);
typedef void (__cdecl *MSVCRT_unexpected_handler)(void);
typedef void (__cdecl *MSVCRT_unexpected_function)(void);
typedef void (__cdecl *MSVCRT__se_translator_function)(unsigned int code, struct _EXCEPTION_POINTERS *info);
typedef void (__cdecl *MSVCRT__beginthread_start_routine_t)(void *);
typedef unsigned int (__stdcall *MSVCRT__beginthreadex_start_routine_t)(void *);
typedef int (__cdecl *MSVCRT__onexit_t)(void);
typedef void (__cdecl *MSVCRT_invalid_parameter_handler)(const MSVCRT_wchar_t*, const MSVCRT_wchar_t*, const MSVCRT_wchar_t*, unsigned, MSVCRT_uintptr_t);
typedef void (__cdecl *MSVCRT_purecall_handler)(void);
typedef void (__cdecl *MSVCRT_security_error_handler)(int, void *);

typedef struct MSVCRT_tagLC_ID {
    unsigned short wLanguage;
    unsigned short wCountry;
    unsigned short wCodePage;
} MSVCRT_LC_ID, *MSVCRT_LPLC_ID;

typedef struct MSVCRT_threadlocaleinfostruct *MSVCRT_pthreadlocinfo;
typedef struct MSVCRT_threadmbcinfostruct *MSVCRT_pthreadmbcinfo;

typedef struct _frame_info
{
    void *object;
    struct _frame_info *next;
} frame_info;

/* Keep in sync with msvcr90/tests/msvcr90.c */
struct __thread_data {
    DWORD                           tid;
    HANDLE                          handle;
    int                             thread_errno;
    MSVCRT_ulong                    thread_doserrno;
    int                             unk1;
    unsigned int                    random_seed;        /* seed for rand() */
    char                           *strtok_next;        /* next ptr for strtok() */
    MSVCRT_wchar_t                 *wcstok_next;        /* next ptr for wcstok() */
    unsigned char                  *mbstok_next;        /* next ptr for mbstok() */
    char                           *strerror_buffer;    /* buffer for strerror */
    MSVCRT_wchar_t                 *wcserror_buffer;    /* buffer for wcserror */
    char                           *tmpnam_buffer;      /* buffer for tmpname() */
    MSVCRT_wchar_t                 *wtmpnam_buffer;     /* buffer for wtmpname() */
    void                           *unk2[2];
    char                           *asctime_buffer;     /* buffer for asctime */
    MSVCRT_wchar_t                 *wasctime_buffer;    /* buffer for wasctime */
    struct MSVCRT_tm               *time_buffer;        /* buffer for localtime/gmtime */
    char                           *efcvt_buffer;       /* buffer for ecvt/fcvt */
    int                             unk3[2];
    void                           *unk4[3];
    EXCEPTION_POINTERS             *xcptinfo;
    int                             fpecode;
    MSVCRT_pthreadmbcinfo           mbcinfo;
    MSVCRT_pthreadlocinfo           locinfo;
    BOOL                            have_locale;
    int                             unk5[1];
    MSVCRT_terminate_function       terminate_handler;
    MSVCRT_unexpected_function      unexpected_handler;
    MSVCRT__se_translator_function  se_translator;      /* preserve offset to exc_record and processing_throw */
    void                           *unk6;
    EXCEPTION_RECORD               *exc_record;
    CONTEXT                        *ctx_record;
    int                             processing_throw;
    frame_info                     *frame_info_head;
    void                           *unk8[6];
    LCID                            cached_lcid;
    BOOL                            cached_sname;
    int                             unk9[2];
    DWORD                           cached_cp;
    char                            cached_locale[131];
    void                           *unk10[100];
#if _MSVCR_VER >= 140
    MSVCRT_invalid_parameter_handler invalid_parameter_handler;
#endif
};

typedef struct __thread_data thread_data_t;

extern BOOL msvcrt_init_tls(void);
extern void msvcrt_free_tls_mem(void);
int* __cdecl     MSVCRT__errno(void);
extern thread_data_t *msvcrt_get_thread_data(void) DECLSPEC_HIDDEN;