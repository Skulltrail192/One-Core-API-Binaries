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
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64 
#include <time.h>
#include <unicode.h>

RTL_CRITICAL_SECTION TIME_tz_section;

timeout_t server_start_time = 0;  /* time of server startup */

#define TICKSPERSEC        10000000
#define TICKSPERMSEC       10000
#define SECSPERDAY         86400
#define SECSPERHOUR        3600
#define SECSPERMIN         60
#define MINSPERHOUR        60
#define HOURSPERDAY        24
#define EPOCHWEEKDAY       1  /* Jan 1, 1601 was Monday */
#define DAYSPERWEEK        7
#define MONSPERYEAR        12
#define DAYSPERQUADRICENTENNIUM (365 * 400 + 97)
#define DAYSPERNORMALQUADRENNIUM (365 * 4 + 1)

/* 1601 to 1970 is 369 years plus 89 leap days */
#define SECS_1601_TO_1970  ((369 * 365 + 89) * (ULONGLONG)SECSPERDAY)
#define TICKS_1601_TO_1970 (SECS_1601_TO_1970 * TICKSPERSEC)
/* 1601 to 1980 is 379 years plus 91 leap days */
#define SECS_1601_TO_1980  ((379 * 365 + 91) * (ULONGLONG)SECSPERDAY)
#define TICKS_1601_TO_1980 (SECS_1601_TO_1980 * TICKSPERSEC)


// static const int MonthLengths[2][MONSPERYEAR] =
// {
	// { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	// { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
// };

// static inline BOOL IsLeapYear(int Year)
// {
    // return Year % 4 == 0 && (Year % 100 != 0 || Year % 400 == 0);
// }

struct timeval
{
    int tv_sec; /*seconds*/
    int tv_usec; /*microseconds*/
};

/*********************************************************************
 *      SystemTimeToFileTime                            (KERNEL32.@)
 */
BOOL WINAPI RtlSystemTimeToFileTime( const SYSTEMTIME *syst, LPFILETIME ft )
{
    TIME_FIELDS tf;
    LARGE_INTEGER t;

    tf.Year = syst->wYear;
    tf.Month = syst->wMonth;
    tf.Day = syst->wDay;
    tf.Hour = syst->wHour;
    tf.Minute = syst->wMinute;
    tf.Second = syst->wSecond;
    tf.Milliseconds = syst->wMilliseconds;

    if( !RtlTimeFieldsToTime(&tf, &t)) {
        return FALSE;
    }
    ft->dwLowDateTime = t.u.LowPart;
    ft->dwHighDateTime = t.u.HighPart;
    return TRUE;
}

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    NtQuerySystemTime( &system_time );
    RtlSystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}

/* return a monotonic time counter, in Win32 ticks */
static ULONGLONG monotonic_counter(void)
{
    struct timeval now;

#ifdef HAVE_CLOCK_GETTIME
    struct timespec ts;
#ifdef CLOCK_MONOTONIC_RAW
    if (!clock_gettime( CLOCK_MONOTONIC_RAW, &ts ))
        return ts.tv_sec * (ULONGLONG)TICKSPERSEC + ts.tv_nsec / 100;
#endif
    if (!clock_gettime( CLOCK_MONOTONIC, &ts ))
        return ts.tv_sec * (ULONGLONG)TICKSPERSEC + ts.tv_nsec / 100;
#endif
    gettimeofday( &now, 0 );
    return now.tv_sec * (ULONGLONG)TICKSPERSEC + now.tv_usec * 10 + TICKS_1601_TO_1970 - server_start_time;
}

DWORD
NTAPI 
RtlGetSystemTimePrecise()
{
	DbgPrint("UNIMPLEMENTED: RtlGetSystemTimePrecise");
	return ERROR_SUCCESS;
}

/***********************************************************************
*        RtlQueryUnbiasedInterruptTime [NTDLL.@]
*/
NTSTATUS 
WINAPI 
RtlQueryUnbiasedInterruptTime(
	ULONGLONG *time
)
{
    *time = monotonic_counter();
    return STATUS_SUCCESS;
}

static time_t find_dst_change(unsigned long min, unsigned long max, int *is_dst)
{
    time_t start;
    struct tm *tm;

    start = min;
    tm = localtime(&start);
    *is_dst = !tm->tm_isdst;

    while (min <= max)
    {
        time_t pos = (min + max) / 2;
        tm = localtime(&pos);

        if (tm->tm_isdst != *is_dst)
            min = pos + 1;
        else
            max = pos - 1;
    }
    return min;
}

/******************************************************************************
 * RtlpNtEnumerateSubKey [NTDLL.@]
 *
 */
NTSTATUS WINAPI RtlpNtEnumerateSubKey( HANDLE handle, UNICODE_STRING *out, ULONG index )
{
  KEY_BASIC_INFORMATION *info;
  DWORD dwLen, dwResultLen;
  NTSTATUS ret;

  if (out->Length)
  {
    dwLen = out->Length + sizeof(KEY_BASIC_INFORMATION);
    info = RtlAllocateHeap( RtlProcessHeap(), 0, dwLen );
    if (!info)
      return STATUS_NO_MEMORY;
  }
  else
  {
    dwLen = 0;
    info = NULL;
  }

  ret = NtEnumerateKey( handle, index, KeyBasicInformation, info, dwLen, &dwResultLen );
  dwResultLen -= sizeof(KEY_BASIC_INFORMATION);

  if (ret == STATUS_BUFFER_OVERFLOW)
    out->Length = dwResultLen;
  else if (!ret)
  {
    if (out->Length < info->NameLength)
    {
      out->Length = dwResultLen;
      ret = STATUS_BUFFER_OVERFLOW;
    }
    else
    {
      out->Length = info->NameLength;
      memcpy(out->Buffer, info->Name, info->NameLength);
    }
  }

  RtlFreeHeap( RtlProcessHeap(), 0, info );
  return ret;
}

static BOOL reg_query_value(HKEY hkey, LPCWSTR name, DWORD type, void *data, DWORD count)
{
    UNICODE_STRING nameW;
    char buf[256];
    KEY_VALUE_PARTIAL_INFORMATION *info = (KEY_VALUE_PARTIAL_INFORMATION *)buf;

    if (count > sizeof(buf) - sizeof(KEY_VALUE_PARTIAL_INFORMATION))
        return FALSE;

    RtlInitUnicodeString(&nameW, name);

    if (NtQueryValueKey(hkey, &nameW, KeyValuePartialInformation,
                        buf, sizeof(buf), &count))
        return FALSE;

    if (info->Type != type) return FALSE;

    memcpy(data, info->Data, info->DataLength);
    return TRUE;
}

/* calculate the mday of dst change date, so that for instance Sun 5 Oct 2007
 * (last Sunday in October of 2007) becomes Sun Oct 28 2007
 *
 * Note: year, day and month must be in unix format.
 */
static int weekday_to_mday(int year, int day, int mon, int day_of_week)
{
    struct tm date;
    time_t tmp;
    int wday, mday;

    /* find first day in the month matching week day of the date */
    memset(&date, 0, sizeof(date));
    date.tm_year = year;
    date.tm_mon = mon;
    date.tm_mday = -1;
    date.tm_wday = -1;
    do
    {
        date.tm_mday++;
        tmp = mktime(&date);
    } while (date.tm_wday != day_of_week || date.tm_mon != mon);

    mday = date.tm_mday;

    /* find number of week days in the month matching week day of the date */
    wday = 1; /* 1 - 1st, ...., 5 - last */
    while (wday < day)
    {
        struct tm *tm;

        date.tm_mday += 7;
        tmp = mktime(&date);
        tm = localtime(&tmp);
        if (tm->tm_mon != mon)
            break;
        mday = tm->tm_mday;
        wday++;
    }

    return mday;
}

static BOOL match_tz_date(const RTL_SYSTEM_TIME *st, const RTL_SYSTEM_TIME *reg_st)
{
    WORD wDay;

    if (st->wMonth != reg_st->wMonth) return FALSE;

    if (!st->wMonth) return TRUE; /* no transition dates */

    wDay = reg_st->wDay;
    if (!reg_st->wYear) /* date in a day-of-week format */
        wDay = weekday_to_mday(st->wYear - 1900, reg_st->wDay, reg_st->wMonth - 1, reg_st->wDayOfWeek);

    if (st->wDay != wDay ||
        st->wHour != reg_st->wHour ||
        st->wMinute != reg_st->wMinute ||
        st->wSecond != reg_st->wSecond ||
        st->wMilliseconds != reg_st->wMilliseconds) return FALSE;

    return TRUE;
}

static BOOL match_tz_info(const RTL_DYNAMIC_TIME_ZONE_INFORMATION *tzi, const RTL_DYNAMIC_TIME_ZONE_INFORMATION *reg_tzi)
{
    if (tzi->Bias == reg_tzi->Bias &&
        match_tz_date(&tzi->StandardDate, &reg_tzi->StandardDate) &&
        match_tz_date(&tzi->DaylightDate, &reg_tzi->DaylightDate))
        return TRUE;

    return FALSE;
}

static void find_reg_tz_info(RTL_DYNAMIC_TIME_ZONE_INFORMATION *tzi, int year)
{
    static const WCHAR Time_ZonesW[] = { 'M','a','c','h','i','n','e','\\',
        'S','o','f','t','w','a','r','e','\\',
        'M','i','c','r','o','s','o','f','t','\\',
        'W','i','n','d','o','w','s',' ','N','T','\\',
        'C','u','r','r','e','n','t','V','e','r','s','i','o','n','\\',
        'T','i','m','e',' ','Z','o','n','e','s',0 };
    static const WCHAR Dynamic_DstW[] = { 'D','y','n','a','m','i','c',' ','D','S','T',0 };
    static const WCHAR fmtW[] = { '%','d',0 };
    HANDLE hkey;
    ULONG idx;
    OBJECT_ATTRIBUTES attr, attrDynamic;
    UNICODE_STRING nameW, nameDynamicW;
    WCHAR buf[128], yearW[16];

    sprintfW(yearW, fmtW, year);

    attrDynamic.Length = sizeof(attrDynamic);
    attrDynamic.RootDirectory = 0; /* will be replaced later */
    attrDynamic.ObjectName = &nameDynamicW;
    attrDynamic.Attributes = 0;
    attrDynamic.SecurityDescriptor = NULL;
    attrDynamic.SecurityQualityOfService = NULL;
    RtlInitUnicodeString(&nameDynamicW, Dynamic_DstW);

    attr.Length = sizeof(attr);
    attr.RootDirectory = 0;
    attr.ObjectName = &nameW;
    attr.Attributes = 0;
    attr.SecurityDescriptor = NULL;
    attr.SecurityQualityOfService = NULL;
    RtlInitUnicodeString(&nameW, Time_ZonesW);
    if (NtOpenKey(&hkey, KEY_READ, &attr))
    {
        DbgPrint("Unable to open the time zones key\n");
        return;
    }

    idx = 0;
    nameW.Buffer = buf;
    nameW.Length = sizeof(buf);
    nameW.MaximumLength = sizeof(buf);

    while (!RtlpNtEnumerateSubKey(hkey, &nameW, idx++))
    {
        static const WCHAR stdW[] = { 'S','t','d',0 };
        static const WCHAR dltW[] = { 'D','l','t',0 };
        static const WCHAR tziW[] = { 'T','Z','I',0 };
        RTL_DYNAMIC_TIME_ZONE_INFORMATION reg_tzi;
        HANDLE hSubkey, hSubkeyDynamicDST;
        BOOL is_dynamic = FALSE;

        struct tz_reg_data
        {
            LONG bias;
            LONG std_bias;
            LONG dlt_bias;
            SYSTEMTIME std_date;
            SYSTEMTIME dlt_date;
        } tz_data;

        attr.Length = sizeof(attr);
        attr.RootDirectory = hkey;
        attr.ObjectName = &nameW;
        attr.Attributes = 0;
        attr.SecurityDescriptor = NULL;
        attr.SecurityQualityOfService = NULL;
        if (NtOpenKey(&hSubkey, KEY_READ, &attr))
        {
            DbgPrint("Unable to open subkey %s\n", nameW.Buffer);
            continue;
        }

#define get_value(hkey, name, type, data, len) \
    if (!reg_query_value(hkey, name, type, data, len)) \
    { \
        DbgPrint("can't read data from %s\n", name); \
        NtClose(hkey); \
        continue; \
    }

        get_value(hSubkey, stdW, REG_SZ, reg_tzi.StandardName, sizeof(reg_tzi.StandardName));
        get_value(hSubkey, dltW, REG_SZ, reg_tzi.DaylightName, sizeof(reg_tzi.DaylightName));
        memcpy(reg_tzi.TimeZoneKeyName, nameW.Buffer, nameW.Length);
        reg_tzi.TimeZoneKeyName[nameW.Length/sizeof(WCHAR)] = 0;

        /* Check for Dynamic DST entry first */
        attrDynamic.RootDirectory = hSubkey;
        if (!NtOpenKey(&hSubkeyDynamicDST, KEY_READ, &attrDynamic))
        {
            is_dynamic = reg_query_value(hSubkeyDynamicDST, yearW, REG_BINARY, &tz_data, sizeof(tz_data));
            NtClose(hSubkeyDynamicDST);
        }

        if (!is_dynamic)
            get_value(hSubkey, tziW, REG_BINARY, &tz_data, sizeof(tz_data));

#undef get_value

        reg_tzi.Bias = tz_data.bias;
        reg_tzi.StandardBias = tz_data.std_bias;
        reg_tzi.DaylightBias = tz_data.dlt_bias;
        reg_tzi.StandardDate = tz_data.std_date;
        reg_tzi.DaylightDate = tz_data.dlt_date;

        DbgPrint("%s: bias %d\n", nameW.Buffer);
        DbgPrint("std (d/m/y): %u/%02u/%04u day of week %u %u:%02u:%02u.%03u bias %d\n",
            reg_tzi.StandardDate.wDay, reg_tzi.StandardDate.wMonth,
            reg_tzi.StandardDate.wYear, reg_tzi.StandardDate.wDayOfWeek,
            reg_tzi.StandardDate.wHour, reg_tzi.StandardDate.wMinute,
            reg_tzi.StandardDate.wSecond, reg_tzi.StandardDate.wMilliseconds,
            reg_tzi.StandardBias);
        DbgPrint("dst (d/m/y): %u/%02u/%04u day of week %u %u:%02u:%02u.%03u bias %d\n",
            reg_tzi.DaylightDate.wDay, reg_tzi.DaylightDate.wMonth,
            reg_tzi.DaylightDate.wYear, reg_tzi.DaylightDate.wDayOfWeek,
            reg_tzi.DaylightDate.wHour, reg_tzi.DaylightDate.wMinute,
            reg_tzi.DaylightDate.wSecond, reg_tzi.DaylightDate.wMilliseconds,
            reg_tzi.DaylightBias);

        NtClose(hSubkey);

        if (match_tz_info(tzi, &reg_tzi))
        {
            *tzi = reg_tzi;
            NtClose(hkey);
            return;
        }

        /* reset len */
        nameW.Length = sizeof(buf);
        nameW.MaximumLength = sizeof(buf);
    }

    NtClose(hkey);

    DbgPrint("Can't find matching timezone information in the registry for "
          "bias %d, std (d/m/y): %u/%02u/%04u, dlt (d/m/y): %u/%02u/%04u\n",
          tzi->Bias,
          tzi->StandardDate.wDay, tzi->StandardDate.wMonth, tzi->StandardDate.wYear,
          tzi->DaylightDate.wDay, tzi->DaylightDate.wMonth, tzi->DaylightDate.wYear);
}

static int init_tz_info(RTL_DYNAMIC_TIME_ZONE_INFORMATION *tzi)
{
    static RTL_DYNAMIC_TIME_ZONE_INFORMATION cached_tzi;
    static int current_year = -1, current_bias = 65535;
    struct tm *tm;
    time_t year_start, year_end, tmp, dlt = 0, std = 0;
    int is_dst, current_is_dst, bias;

    RtlEnterCriticalSection( &TIME_tz_section );

    year_start = time(NULL);
    tm = gmtime(&year_start);
    bias = (LONG)(mktime(tm) - year_start) / 60;

    tm = localtime(&year_start);
    current_is_dst = tm->tm_isdst;
    if (current_year == tm->tm_year && current_bias == bias)
    {
        *tzi = cached_tzi;
        RtlLeaveCriticalSection( &TIME_tz_section );
        return current_is_dst;
    }

    memset(tzi, 0, sizeof(*tzi));
	
    current_year = tm->tm_year;
    current_bias = bias;

    tzi->Bias = bias;

    tm->tm_isdst = 0;
    tm->tm_mday = 1;
    tm->tm_mon = tm->tm_hour = tm->tm_min = tm->tm_sec = tm->tm_wday = tm->tm_yday = 0;
    year_start = mktime(tm);

    tm->tm_mday = tm->tm_wday = tm->tm_yday = 0;
    tm->tm_mon = 12;
    tm->tm_hour = 23;
    tm->tm_min = tm->tm_sec = 59;
    year_end = mktime(tm);
	
    tmp = find_dst_change(year_start, year_end, &is_dst);
    if (is_dst)
        dlt = tmp;
    else
        std = tmp;

    tmp = find_dst_change(tmp, year_end, &is_dst);
    if (is_dst)
        dlt = tmp;
    else
        std = tmp;
	
    if (dlt == std || !dlt || !std)
        DbgPrint("there is no daylight saving rules in this time zone\n");
    else
    {
        tmp = dlt - tzi->Bias * 60;
        tm = gmtime(&tmp);

        tzi->DaylightBias = -60;
        tzi->DaylightDate.wYear = tm->tm_year + 1900;
        tzi->DaylightDate.wMonth = tm->tm_mon + 1;
        tzi->DaylightDate.wDayOfWeek = tm->tm_wday;
        tzi->DaylightDate.wDay = tm->tm_mday;
        tzi->DaylightDate.wHour = tm->tm_hour;
        tzi->DaylightDate.wMinute = tm->tm_min;
        tzi->DaylightDate.wSecond = tm->tm_sec;
        tzi->DaylightDate.wMilliseconds = 0;

        DbgPrint("daylight (d/m/y): %u/%02u/%04u day of week %u %u:%02u:%02u.%03u bias %d\n",
            tzi->DaylightDate.wDay, tzi->DaylightDate.wMonth,
            tzi->DaylightDate.wYear, tzi->DaylightDate.wDayOfWeek,
            tzi->DaylightDate.wHour, tzi->DaylightDate.wMinute,
            tzi->DaylightDate.wSecond, tzi->DaylightDate.wMilliseconds,
            tzi->DaylightBias);

        tmp = std - tzi->Bias * 60 - tzi->DaylightBias * 60;
        tm = gmtime(&tmp);
        tzi->StandardBias = 0;
        tzi->StandardDate.wYear = tm->tm_year + 1900;
        tzi->StandardDate.wMonth = tm->tm_mon + 1;
        tzi->StandardDate.wDayOfWeek = tm->tm_wday;
        tzi->StandardDate.wDay = tm->tm_mday;
        tzi->StandardDate.wHour = tm->tm_hour;
        tzi->StandardDate.wMinute = tm->tm_min;
        tzi->StandardDate.wSecond = tm->tm_sec;
        tzi->StandardDate.wMilliseconds = 0;

        DbgPrint("standard (d/m/y): %u/%02u/%04u day of week %u %u:%02u:%02u.%03u bias %d\n",
            tzi->StandardDate.wDay, tzi->StandardDate.wMonth,
            tzi->StandardDate.wYear, tzi->StandardDate.wDayOfWeek,
            tzi->StandardDate.wHour, tzi->StandardDate.wMinute,
            tzi->StandardDate.wSecond, tzi->StandardDate.wMilliseconds,
            tzi->StandardBias);
    }

    find_reg_tz_info(tzi, current_year + 1900);
    cached_tzi = *tzi;

    RtlLeaveCriticalSection( &TIME_tz_section );

    return current_is_dst;
}

/***********************************************************************
 *      RtlQueryDynamicTimeZoneInformation [NTDLL.@]
 *
 * Get information about the current timezone.
 *
 * PARAMS
 *   tzinfo [O] Destination for the retrieved timezone info.
 *
 * RETURNS
 *   Success: STATUS_SUCCESS.
 *   Failure: An NTSTATUS error code indicating the problem.
 */
NTSTATUS WINAPI RtlQueryDynamicTimeZoneInformation(RTL_DYNAMIC_TIME_ZONE_INFORMATION *tzinfo)
{
    init_tz_info( tzinfo );

    return STATUS_SUCCESS;
}