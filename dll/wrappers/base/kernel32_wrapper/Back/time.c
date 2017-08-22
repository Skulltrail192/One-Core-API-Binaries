/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    time.c

Abstract:

    This module implements Time functions for the Win32 APIs.

Author:

    Skulltrail 06-May-2017

Revision History:

--*/

#include <main.h>

#define LL2FILETIME( ll, pft )\
    (pft)->dwLowDateTime = (UINT)(ll); \
    (pft)->dwHighDateTime = (UINT)((ll) >> 32);
#define FILETIME2LL( pft, ll) \
    ll = (((LONGLONG)((pft)->dwHighDateTime))<<32) + (pft)-> dwLowDateTime ;
	
static const int MonthLengths[2][12] =
{
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

static inline BOOL IsLeapYear(int Year)
{
    return Year % 4 == 0 && (Year % 100 != 0 || Year % 400 == 0);
}

/***********************************************************************
 *           QueryUnbiasedInterruptTime   (KERNEL32.@)
 */
BOOL 
WINAPI 
QueryUnbiasedInterruptTime(ULONGLONG *time)
{
    if (!time) return FALSE;
    RtlQueryUnbiasedInterruptTime(time);
    return TRUE;
}

/***********************************************************************
 *  TIME_DayLightCompareDate
 *
 * Compares two dates without looking at the year.
 *
 * PARAMS
 *   date        [in] The local time to compare.
 *   compareDate [in] The daylight savings begin or end date.
 *
 * RETURNS
 *
 *  -1 if date < compareDate
 *   0 if date == compareDate
 *   1 if date > compareDate
 *  -2 if an error occurs
 */
static int TIME_DayLightCompareDate( const SYSTEMTIME *date,
    const SYSTEMTIME *compareDate )
{
    int limit_day, dayinsecs;

    if (date->wMonth < compareDate->wMonth)
        return -1; /* We are in a month before the date limit. */

    if (date->wMonth > compareDate->wMonth)
        return 1; /* We are in a month after the date limit. */

    /* if year is 0 then date is in day-of-week format, otherwise
     * it's absolute date.
     */
    if (compareDate->wYear == 0)
    {
        WORD First;
        /* compareDate->wDay is interpreted as number of the week in the month
         * 5 means: the last week in the month */
        int weekofmonth = compareDate->wDay;
          /* calculate the day of the first DayOfWeek in the month */
        First = ( 6 + compareDate->wDayOfWeek - date->wDayOfWeek + date->wDay 
               ) % 7 + 1;
        limit_day = First + 7 * (weekofmonth - 1);
        /* check needed for the 5th weekday of the month */
        if(limit_day > MonthLengths[date->wMonth==2 && IsLeapYear(date->wYear)]
                [date->wMonth - 1])
            limit_day -= 7;
    }
    else
    {
       limit_day = compareDate->wDay;
    }

    /* convert to seconds */
    limit_day = ((limit_day * 24  + compareDate->wHour) * 60 +
            compareDate->wMinute ) * 60;
    dayinsecs = ((date->wDay * 24  + date->wHour) * 60 +
            date->wMinute ) * 60 + date->wSecond;
    /* and compare */
    return dayinsecs < limit_day ? -1 :
           dayinsecs > limit_day ? 1 :
           0;   /* date is equal to the date limit. */
}

/***********************************************************************
 *  TIME_CompTimeZoneID
 *
 *  Computes the local time bias for a given time and time zone.
 *
 *  PARAMS
 *      pTZinfo     [in] The time zone data.
 *      lpFileTime  [in] The system or local time.
 *      islocal     [in] it is local time.
 *
 *  RETURNS
 *      TIME_ZONE_ID_INVALID    An error occurred
 *      TIME_ZONE_ID_UNKNOWN    There are no transition time known
 *      TIME_ZONE_ID_STANDARD   Current time is standard time
 *      TIME_ZONE_ID_DAYLIGHT   Current time is daylight savings time
 */
static DWORD TIME_CompTimeZoneID ( const TIME_ZONE_INFORMATION *pTZinfo,
    FILETIME *lpFileTime, BOOL islocal )
{
    int ret, year;
    BOOL beforeStandardDate, afterDaylightDate;
    DWORD retval = TIME_ZONE_ID_INVALID;
    LONGLONG llTime = 0; /* initialized to prevent gcc complaining */
    SYSTEMTIME SysTime;
    FILETIME ftTemp;

    if (pTZinfo->DaylightDate.wMonth != 0)
    {
        /* if year is 0 then date is in day-of-week format, otherwise
         * it's absolute date.
         */
        if (pTZinfo->StandardDate.wMonth == 0 ||
            (pTZinfo->StandardDate.wYear == 0 &&
            (pTZinfo->StandardDate.wDay<1 ||
            pTZinfo->StandardDate.wDay>5 ||
            pTZinfo->DaylightDate.wDay<1 ||
            pTZinfo->DaylightDate.wDay>5)))
        {
            SetLastError(ERROR_INVALID_PARAMETER);
            return TIME_ZONE_ID_INVALID;
        }

        if (!islocal) {
            FILETIME2LL( lpFileTime, llTime );
            llTime -= pTZinfo->Bias * (LONGLONG)600000000;
            LL2FILETIME( llTime, &ftTemp);
            lpFileTime = &ftTemp;
        }

        FileTimeToSystemTime(lpFileTime, &SysTime);
        year = SysTime.wYear;

        if (!islocal) {
            llTime -= pTZinfo->DaylightBias * (LONGLONG)600000000;
            LL2FILETIME( llTime, &ftTemp);
            FileTimeToSystemTime(lpFileTime, &SysTime);
        }

        /* check for daylight savings */
        if(year == SysTime.wYear) {
            ret = TIME_DayLightCompareDate( &SysTime, &pTZinfo->StandardDate);
            if (ret == -2)
                return TIME_ZONE_ID_INVALID;

            beforeStandardDate = ret < 0;
        } else
            beforeStandardDate = SysTime.wYear < year;

        if (!islocal) {
            llTime -= ( pTZinfo->StandardBias - pTZinfo->DaylightBias )
                * (LONGLONG)600000000;
            LL2FILETIME( llTime, &ftTemp);
            FileTimeToSystemTime(lpFileTime, &SysTime);
        }

        if(year == SysTime.wYear) {
            ret = TIME_DayLightCompareDate( &SysTime, &pTZinfo->DaylightDate);
            if (ret == -2)
                return TIME_ZONE_ID_INVALID;

            afterDaylightDate = ret >= 0;
        } else
            afterDaylightDate = SysTime.wYear > year;

        retval = TIME_ZONE_ID_STANDARD;
        if( pTZinfo->DaylightDate.wMonth <  pTZinfo->StandardDate.wMonth ) {
            /* Northern hemisphere */
            if( beforeStandardDate && afterDaylightDate )
                retval = TIME_ZONE_ID_DAYLIGHT;
        } else    /* Down south */
            if( beforeStandardDate || afterDaylightDate )
            retval = TIME_ZONE_ID_DAYLIGHT;
    } else 
        /* No transition date */
        retval = TIME_ZONE_ID_UNKNOWN;
        
    return retval;
}

/***********************************************************************
 *  TIME_TimeZoneID
 *
 *  Calculates whether daylight savings is on now.
 *
 *  PARAMS
 *      pTzi [in] Timezone info.
 *
 *  RETURNS
 *      TIME_ZONE_ID_INVALID    An error occurred
 *      TIME_ZONE_ID_UNKNOWN    There are no transition time known
 *      TIME_ZONE_ID_STANDARD   Current time is standard time
 *      TIME_ZONE_ID_DAYLIGHT   Current time is daylight savings time
 */
static DWORD TIME_ZoneID( const TIME_ZONE_INFORMATION *pTzi )
{
    FILETIME ftTime;
    GetSystemTimeAsFileTime( &ftTime);
    return TIME_CompTimeZoneID( pTzi, &ftTime, FALSE);
}

/***********************************************************************
 *           GetDynamicTimeZoneInformation   (KERNEL32.@)
 */
DWORD WINAPI GetDynamicTimeZoneInformation(DYNAMIC_TIME_ZONE_INFORMATION *tzinfo)
{
    NTSTATUS status;

    status = RtlQueryDynamicTimeZoneInformation( (RTL_DYNAMIC_TIME_ZONE_INFORMATION*)tzinfo );
    if ( status != STATUS_SUCCESS )
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return TIME_ZONE_ID_INVALID;
    }
    return TIME_ZoneID( (TIME_ZONE_INFORMATION*)tzinfo );
}

/***********************************************************************
 *  TIME_GetSpecificTimeZoneKey
 *
 *  Opens the registry key for the time zone with the given name.
 *
 * PARAMS
 *  key_name   [in]  The time zone name.
 *  result     [out] The open registry key handle.
 *
 * RETURNS
 *  TRUE if successful.
 */
static BOOL TIME_GetSpecificTimeZoneKey( const WCHAR *key_name, HANDLE *result )
{
    static const WCHAR Time_ZonesW[] = { '\\','R','E','G','I','S','T','R','Y','\\',
        'M','a','c','h','i','n','e','\\',
        'S','o','f','t','w','a','r','e','\\',
        'M','i','c','r','o','s','o','f','t','\\',
        'W','i','n','d','o','w','s',' ','N','T','\\',
        'C','u','r','r','e','n','t','V','e','r','s','i','o','n','\\',
        'T','i','m','e',' ','Z','o','n','e','s',0 };
    HANDLE time_zones_key;
    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING nameW;
    NTSTATUS status;

    attr.Length = sizeof(attr);
    attr.RootDirectory = 0;
    attr.ObjectName = &nameW;
    attr.Attributes = 0;
    attr.SecurityDescriptor = NULL;
    attr.SecurityQualityOfService = NULL;
    RtlInitUnicodeString( &nameW, Time_ZonesW );
    status = NtOpenKey( &time_zones_key, KEY_READ, &attr );
    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }

    attr.RootDirectory = time_zones_key;
    RtlInitUnicodeString( &nameW, key_name );
    status = NtOpenKey( result, KEY_READ, &attr );

    NtClose( time_zones_key );

    if (status)
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }

    return TRUE;
}

static BOOL reg_query_value(HKEY hkey, LPCWSTR name, DWORD type, void *data, DWORD count)
{
    UNICODE_STRING nameW;
    char buf[256];
    KEY_VALUE_PARTIAL_INFORMATION *info = (KEY_VALUE_PARTIAL_INFORMATION *)buf;
    NTSTATUS status;

    if (count > sizeof(buf) - sizeof(KEY_VALUE_PARTIAL_INFORMATION))
        return FALSE;

    RtlInitUnicodeString(&nameW, name);

    if ((status = NtQueryValueKey(hkey, &nameW, KeyValuePartialInformation,
                                  buf, sizeof(buf), &count)))
    {
        SetLastError( RtlNtStatusToDosError(status) );
        return FALSE;
    }

    if (info->Type != type)
    {
        SetLastError( ERROR_DATATYPE_MISMATCH );
        return FALSE;
    }

    memcpy(data, info->Data, info->DataLength);
    return TRUE;
}

/***********************************************************************
 *  TIME_GetSpecificTimeZoneInfo
 *
 *  Returns time zone information for the given time zone and year.
 *
 * PARAMS
 *  key_name   [in]  The time zone name.
 *  year       [in]  The year, if Dynamic DST is used.
 *  dynamic    [in]  Whether to use Dynamic DST.
 *  result     [out] The time zone information.
 *
 * RETURNS
 *  TRUE if successful.
 */
static BOOL TIME_GetSpecificTimeZoneInfo( const WCHAR *key_name, WORD year,
    BOOL dynamic, DYNAMIC_TIME_ZONE_INFORMATION *tzinfo )
{
    static const WCHAR Dynamic_DstW[] = { 'D','y','n','a','m','i','c',' ','D','S','T',0 };
    static const WCHAR fmtW[] = { '%','d',0 };
    static const WCHAR stdW[] = { 'S','t','d',0 };
    static const WCHAR dltW[] = { 'D','l','t',0 };
    static const WCHAR tziW[] = { 'T','Z','I',0 };
    HANDLE time_zone_key, dynamic_dst_key;
    OBJECT_ATTRIBUTES attr;
    UNICODE_STRING nameW;
    WCHAR yearW[16];
    BOOL got_reg_data = FALSE;
    struct tz_reg_data
    {
        LONG bias;
        LONG std_bias;
        LONG dlt_bias;
        SYSTEMTIME std_date;
        SYSTEMTIME dlt_date;
    } tz_data;

    if (!TIME_GetSpecificTimeZoneKey( key_name, &time_zone_key ))
        return FALSE;

    if (!reg_query_value( time_zone_key, stdW, REG_SZ, tzinfo->StandardName, sizeof(tzinfo->StandardName)) ||
        !reg_query_value( time_zone_key, dltW, REG_SZ, tzinfo->DaylightName, sizeof(tzinfo->DaylightName)))
    {
        NtClose( time_zone_key );
        return FALSE;
    }

    lstrcpyW(tzinfo->TimeZoneKeyName, key_name);

    if (dynamic)
    {
        attr.Length = sizeof(attr);
        attr.RootDirectory = time_zone_key;
        attr.ObjectName = &nameW;
        attr.Attributes = 0;
        attr.SecurityDescriptor = NULL;
        attr.SecurityQualityOfService = NULL;
        RtlInitUnicodeString( &nameW, Dynamic_DstW );
        if (!NtOpenKey( &dynamic_dst_key, KEY_READ, &attr ))
        {
            sprintfW( yearW, fmtW, year );
            got_reg_data = reg_query_value( dynamic_dst_key, yearW, REG_BINARY, &tz_data, sizeof(tz_data) );

            NtClose( dynamic_dst_key );
        }
    }

    if (!got_reg_data)
    {
        if (!reg_query_value( time_zone_key, tziW, REG_BINARY, &tz_data, sizeof(tz_data) ))
        {
            NtClose( time_zone_key );
            return FALSE;
        }
    }

    tzinfo->Bias = tz_data.bias;
    tzinfo->StandardBias = tz_data.std_bias;
    tzinfo->DaylightBias = tz_data.dlt_bias;
    tzinfo->StandardDate = tz_data.std_date;
    tzinfo->DaylightDate = tz_data.dlt_date;

    tzinfo->DynamicDaylightTimeDisabled = !dynamic;

    NtClose( time_zone_key );

    return TRUE;
}

/***********************************************************************
 *              GetTimeZoneInformationForYear  (KERNEL32.@)
 */
BOOL 
WINAPI 
GetTimeZoneInformationForYear( 
	USHORT wYear,
    PDYNAMIC_TIME_ZONE_INFORMATION pdtzi, 
	LPTIME_ZONE_INFORMATION ptzi)
{
    DYNAMIC_TIME_ZONE_INFORMATION local_dtzi, result;

    if (!pdtzi)
    {
        if (GetDynamicTimeZoneInformation(&local_dtzi) == TIME_ZONE_ID_INVALID)
            return FALSE;
        pdtzi = &local_dtzi;
    }

    if (!TIME_GetSpecificTimeZoneInfo(pdtzi->TimeZoneKeyName, wYear,
            !pdtzi->DynamicDaylightTimeDisabled, &result))
        return FALSE;

    memcpy(ptzi, &result, sizeof(*ptzi));

    return TRUE;
}