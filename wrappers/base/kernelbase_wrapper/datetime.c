/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    datetime.c

Abstract:

    This module implements Win32 time of day, months and years functions

Author:

    Skulltrail 20-March-2017

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(kernel32);

int GregorianDaysToMonth365 [12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int GregorianDaysToMonth366 [12] = {31,29,31,30,31,30,31,31,30,31,30,31}; 

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
            LL2FILETIME( llTime, &ftTemp)
            lpFileTime = &ftTemp;
        }

        FileTimeToSystemTime(lpFileTime, &SysTime);
        year = SysTime.wYear;

        if (!islocal) {
            llTime -= pTZinfo->DaylightBias * (LONGLONG)600000000;
            LL2FILETIME( llTime, &ftTemp)
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
            LL2FILETIME( llTime, &ftTemp)
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

/*
* @implemented
*/
BOOL
WINAPI
IsTimeZoneRedirectionEnabled(VOID)
{
     /* Return if a TS Timezone ID is active */
     return (BaseStaticServerData->TermsrvClientTimeZoneId != TIME_ZONE_ID_INVALID);
}

/***********************************************************************
 *              GetSystemTimePreciseAsFileTime  (KERNEL32.@)
 *
 *  Get the current time in utc format, with <1 us precision.
 *
 *  RETURNS
 *   Nothing.
 */
VOID 
WINAPI 
GetSystemTimePreciseAsFileTime(
	LPFILETIME lpSystemTimeAsFileTime
)
{
	GetSystemTimeAsFileTime(lpSystemTimeAsFileTime);
}

/*
* @semi-implemented need test
*/
int 
WINAPI 
GetTimeFormatEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       DWORD dwFlags,
  _In_opt_   const SYSTEMTIME *lpTime,
  _In_opt_   LPCWSTR lpFormat,
  _Out_opt_  LPWSTR lpTimeStr,
  _In_       int cchTime
)
{
	LCID locale = LocaleNameToLCID(lpLocaleName, 0);
	return GetTimeFormatW(locale,
						  dwFlags,
						  lpTime,
						  lpFormat,
						  lpTimeStr,
						  cchTime);
}

int 
WINAPI 
GetDateFormatEx(
  _In_opt_   LPCWSTR lpLocaleName,
  _In_       DWORD dwFlags,
  _In_opt_   const SYSTEMTIME *lpDate,
  _In_opt_   LPCWSTR lpFormat,
  _Out_opt_  LPWSTR lpDateStr,
  _In_       int cchDate,
  _In_opt_   LPCWSTR lpCalendar
)
{
	LCID locale = LocaleNameToLCID(lpLocaleName, 0);
	return GetDateFormatW(locale,
						  dwFlags,
						  lpDate,
						  lpFormat,
						  lpDateStr,
						  cchDate);	
}

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
	LCID locale = LocaleNameToLCID(lpLocaleName, 0);
	return GetCalendarInfoW(locale ,
							Calendar,
							CalType,
							lpCalData,
							cchData,
							lpValue);
}

//************************************************* Time Date Functions **********************************************************

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
        WARN("Unable to open the time zones key\n");
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

/*
 * @implemented
*/
BOOL 
WINAPI IsGregorianLeapYear(UINT year)
{
  if ((((year % 4) == 0) && ((year % 100) != 0))
        || ((year % 400) == 0))
  {
	return TRUE;
  }else{
	return FALSE;
  }
}

/*
 * @implemented
 */
BOOL
WINAPI IsValidGregorianTime(LPCALDATETIME lpCalDateTime,  UINT a2)
{
  BOOL resp; // esi@1

  resp = TRUE;
  if ( lpCalDateTime->Hour > 23 )
    resp = FALSE;
  if ( lpCalDateTime->Minute > 59 )
    resp = FALSE;
  if ( lpCalDateTime->Second > 59 )
    resp = FALSE;
  if ( lpCalDateTime->Tick > 9999999 )
    resp = FALSE;
  if ( !resp )
    SetLastError(ERROR_DATE_OUT_OF_RANGE);
  return resp;
} 


/*
* @implemented
*/
BOOL 
WINAPI IsValidGregorianYear(UINT year, UINT era)
{
	BOOL result; // eax@1
    result = TRUE;
	if ( era != 1 || (year - 1) > 9998 )
	  {
		SetLastError(ERROR_DATE_OUT_OF_RANGE);
		result = FALSE;
	  }
	return result;
}  

/*
 * @implemented
 */
ULONG 
WINAPI GetAbsoluteDateFromGregorian(UINT year, UINT month, UINT day)
{
	int N = day;           // days this month
	int m;
    for (m = month - 1;  m > 0; m--) // days in prior months this year
      N = N + LastDayOfGregorianMonth(m, year);
    return
      (N                    // days this year
       + 365 * (year - 1)   // days in previous years ignoring leap days
       + (year - 1)/4       // Julian leap days before this year...
       - (year - 1)/100     // ...minus prior century years...
       + (year - 1)/400);
}

/*
 * @implemented
 */
ULONG 
WINAPI GetGregorianDayOfWeekHelper(IN LPCALDATETIME lpCalDateTime)
{
	if(lpCalDateTime)
	{
		return (GetAbsoluteDateFromGregorian(lpCalDateTime->Year, lpCalDateTime->Month, lpCalDateTime->Day))%7;
	}else{
		return -1;
	}
}

/*
 * @implemented unclean
 */
BOOL 
WINAPI IsCalendarLeapDay(CALID calId, UINT day, UINT month, UINT year, UINT era)
{
  BOOL resp; // edi@1
  resp = FALSE;
  if ( (month - 1) > 11 || !IsValidGregorianYear(year, era) )
  {
      SetLastError(ERROR_DATE_OUT_OF_RANGE);
      return FALSE;
  }
  if ( IsGregorianLeapYear(year) )
  {
    if ( day > GregorianDaysToMonth366[month] - GregorianDaysToMonth366[month - 1] )
    {
      SetLastError(ERROR_DATE_OUT_OF_RANGE);
      return FALSE;
    }
    if ( month == 2 )
    {
      if ( day == 29 )
      {
        SetLastError(0);
        resp = TRUE;
      }
    }
  } 
  return resp;
}

/*
* @implemented
*/
BOOL 
WINAPI IsValidGregorianMonthAndDay(UINT day, UINT month, UINT year, UINT amount)
{
  BOOL resp; // edi@1
  char var='\0'; // zf@2
  int *dayPointer; // eax@2

  resp = TRUE;
  if ( (month - 1) > 11 )
  var = IsGregorianLeapYear(year) == 0;
  dayPointer = &GregorianDaysToMonth366[month];
  if ( var )
    dayPointer = &GregorianDaysToMonth365[month];
  if ( day > *dayPointer - *(dayPointer - 1) )
  {
    resp = FALSE;
    SetLastError(ERROR_DATE_OUT_OF_RANGE);
  }
  return resp;
}

/*
* @implemented
*/
BOOL 
WINAPI IsValidGregorianDateTime(LPCALDATETIME lpCalDateTime, UINT amount, UINT parameter)
{
  BOOL resp; 
  UINT localYear; 
  UINT localDay; 
  UINT localDayOfWeek; 

  resp = TRUE;
  if ( !lpCalDateTime )
    return 0;
  localYear = lpCalDateTime->Year;
  localDay = lpCalDateTime->Day;
  if ( lpCalDateTime->Era != 1 )
    resp = FALSE;
  if ( localYear < 1 || localYear > 9999 )
    resp = FALSE;
  if ( localDay < 1 || localDay > 31 )
    resp = FALSE;
  if ( !resp )
  if ( !IsValidGregorianMonthAndDay(localDay, lpCalDateTime->Month, localYear, amount) )
    resp = FALSE;
  if ( !IsValidGregorianTime(lpCalDateTime, amount) )
    return FALSE;
  if ( parameter )
  {
    localDayOfWeek = lpCalDateTime->DayOfWeek;
    if ( localDayOfWeek != -1 )
    {
      if ( localDayOfWeek != GetGregorianDayOfWeekHelper(lpCalDateTime) )
        resp = FALSE;
    }
  }
  if ( !resp )
  {
    SetLastError(ERROR_DATE_OUT_OF_RANGE);
    return FALSE;
  }
  return TRUE;
} 

/*
* @implemented
*/
BOOL 
WINAPI IsValidCalDateTime(_In_ LPCALDATETIME lpCalDateTime,_In_ CALID amount)
{
	return IsValidGregorianDateTime(lpCalDateTime, amount, 1);
}

/*
 * @implemented 
 */
BOOL 
WINAPI IsCalendarLeapYear(_In_  CALID calId,
						  _In_  UINT year,
						  _In_  UINT era)
{
	return IsGregorianLeapYear(year);
}

/*
 * @implemented clean
 */
BOOL 
WINAPI IsGregorianLeapMonth(ULONG month, ULONG year, int era)
{
  DWORD error; // [sp-4h] [bp-4h]@3

  if ( IsValidGregorianYear(year, era) )
  {
    if ( month - 1 > 11 )
      error = 87;
    else
      error = 0;
    SetLastError(error);
  }
  return 0;
}

/*
 * @implemented clean
 */
BOOL 
WINAPI IsCalendarLeapMonth(CALID calId, ULONG month, ULONG year, ULONG era)
{
	return IsGregorianLeapMonth(month, year, era);
}

/*
 * @implemented clean
 */
BOOL
WINAPI UpdateCalendarDayOfWeek(_Out_ LPCALDATETIME lpCalDateTime)
{	
	LPCALDATETIME lpCalDateTimeLocal = lpCalDateTime;
	UINT result;
	
	result = GetGregorianDayOfWeekHelper(lpCalDateTimeLocal);
	if(result!= -1)
	{
		lpCalDateTimeLocal->DayOfWeek = result;
		lpCalDateTimeLocal->Day = lpCalDateTime->Day;
		lpCalDateTimeLocal->Year = lpCalDateTime->Year;
		lpCalDateTimeLocal->Month = lpCalDateTime->Month;
		return TRUE;
	}else{
		return FALSE;
	}
}

/*
 * @implemented 
 */ 
BOOL 
WINAPI NlsGregorianLeapYear(IN UINT year){
	if(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
	{
		return TRUE;
	}else{
		return FALSE;
	}
}  

/*
 * @implemented clean
 */
ULONG
WINAPI GetCalendarDaysInMonth(CALID calId,  UINT month, UINT year, UINT era) {
	BOOL leap;
	int dayCount;
	if((month-1)> 11)
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			return 0;
		}else{
			leap = NlsGregorianLeapYear(year);
			dayCount = GregorianDaysToMonth366[month];
			if(!leap)
			{
				dayCount = GregorianDaysToMonth365[month];
			}			
	}
	return dayCount;
}

/*
 * @implemented 
 */
BOOL 
WINAPI CopyCalDateTime(CALDATETIME *lpCalDateTime, CALID *calId)
{
  BOOL result; 

  if ( lpCalDateTime && calId )
  {
    memcpy(lpCalDateTime, calId, 40);
    result = TRUE;
  }
  else
  {
    result = FALSE;
  }
  return result;
}

/*
 * @implemented 
 */
int 
WINAPI LastDayOfGregorianMonth( int month, int year )
{
switch (month) {
  case 2:
    if ((((year % 4) == 0) && ((year % 100) != 0))
        || ((year % 400) == 0))
      return 29;
    else
      return 28;
  case 4:
  case 6:
  case 9:
  case 11: return 30;
  default: return 31;
  }
}

/*
 * @implemented 
 */
BOOL 
WINAPI AdjustGregorianDays(UINT absoluteDate, LPCALDATETIME lpCalDateTime){
	int year;
	int month;
	int day;
	year = absoluteDate/366;
        while (absoluteDate >= GetAbsoluteDateFromGregorian(year+1,1,1))
		{
           year++;
		}
        month = 1;
        while (absoluteDate > GetAbsoluteDateFromGregorian(year, month, LastDayOfGregorianMonth(month,year)))
		{
           month++;
	    }
        day = absoluteDate - GetAbsoluteDateFromGregorian(year,month,1) + 1; 
		lpCalDateTime->Era = 1;
		lpCalDateTime->Year = year;
		lpCalDateTime->Month = month;
		lpCalDateTime->Day = day;
		lpCalDateTime->DayOfWeek = absoluteDate % 7;
    return TRUE;
}

/*
 * @implemented 
 */
BOOL WINAPI IsValidYearAndMonth(LPCALDATETIME lpCalDateTime)
{
	if(lpCalDateTime->Year>=1980 && lpCalDateTime->Year<=2099 && lpCalDateTime->Month>=1 && lpCalDateTime->Month<=12) 
	{
		return TRUE;
	}else{
		return FALSE;
	}
}

/*
 * @implemented 
 */
BOOL 
WINAPI AdjustGregorianYearsAndMonths(LPCALDATETIME lpCalDateTime, int amount)
{
  CALDATETIME *localCalDateTime;
  UINT localYear; 
  UINT localMonth; 
  signed int localAmount;
  int localAmount2;
  int localAmount3; 
  int localYearAmount; 
  int dayCount;
  NTSTATUS STATUS; 
  UINT localEra;
  int v16; 
  UINT localHour; 
  UINT localMinute; 
  UINT localSecond; 
  ULONG localTick; 
  BOOL resp; 
  int localDay; 

  resp = FALSE;
  localCalDateTime = lpCalDateTime;
  if ( !lpCalDateTime )
    return FALSE;
  localYear = lpCalDateTime->Year;
  localDay = lpCalDateTime->Day;
  localMonth = localCalDateTime->Month;
  localAmount = localMonth + amount - 1;
  if ( localAmount < 0 )
  {
    localAmount3 = (signed int)(localMonth + amount) % 12 + 12;
    localAmount2 = (signed int)(localMonth + amount - 12) / 12;
  }
  else
  {
    localAmount2 = localAmount / 12;
    localAmount3 = localAmount % 12 + 1;
  }
  localYearAmount = localAmount2 + localYear;
  dayCount = GetCalendarDaysInMonth(localCalDateTime->CalId, localAmount3, localYearAmount, localCalDateTime->Era);
  if ( !dayCount )
  {
    STATUS = ERROR_DATE_OUT_OF_RANGE;
    SetLastError(STATUS);
    return FALSE;
  }
  if ( localDay > dayCount )
    localDay = dayCount;
  localEra = localCalDateTime->Era;
  v16 = localDay;
  localHour = localCalDateTime->Hour;
  localMinute = localCalDateTime->Minute;
  localSecond = localCalDateTime->Second;
  localTick = localCalDateTime->Tick;
  localCalDateTime->Year = localYearAmount;
  localCalDateTime->Month = localAmount3;  
  return TRUE;
}


/*
 * @implemented 
 */
BOOL 
WINAPI AdjustCalendarDate(
	_Inout_ LPCALDATETIME lpCalDateTime,
	_In_ CALDATETIME_DATEUNIT calUnit,
	int amount
)
{
	int absoluteDate;
	int year = lpCalDateTime->Year;
	int month = lpCalDateTime->Month;
	int day = lpCalDateTime->Day;
	int localAmount;

	if(calUnit == 1)
	{
		localAmount = 12 * amount;
		return AdjustGregorianYearsAndMonths(lpCalDateTime, localAmount);
	}
	if ( calUnit != 2 )
    {
		if ( calUnit == 3 )
		{
			localAmount = 7 * amount;			
		}else{
			if ( calUnit != 4 )
			{
				return FALSE;
			}
			localAmount = amount;
		}
		absoluteDate = (localAmount + GetAbsoluteDateFromGregorian(year, month, day));
		return AdjustGregorianDays(absoluteDate, lpCalDateTime);
	}
	localAmount = amount;
	return AdjustGregorianYearsAndMonths(lpCalDateTime, localAmount);
}

/*
 * @implemented clean
 */
BOOL 
WINAPI 
ConvertCalDateTimeToSystemTime(
  _In_   const LPCALDATETIME lpCalDateTime,
  _Out_  SYSTEMTIME *lpSystemTime
)
{
	const LPCALDATETIME lpCalDate = lpCalDateTime;
	BOOL result;
	if(lpCalDateTime->Year>=1601)
		{
			lpSystemTime->wYear = lpCalDate->Year;
			lpSystemTime->wMonth = lpCalDate->Month;
			lpSystemTime->wDay = lpCalDate->Day;
			lpSystemTime->wHour = lpCalDate->Hour;
			lpSystemTime->wMinute = lpCalDate->Minute;
			lpSystemTime->wSecond = lpCalDate->Second;
			lpSystemTime->wMilliseconds = lpCalDate->Tick/10000;
			lpSystemTime->wDayOfWeek = lpCalDate->DayOfWeek;
			result=TRUE;
	}else{
		SetLastError(ERROR_DATE_OUT_OF_RANGE);
        result=FALSE;
	}  
	return result;
}

/*
 * @implemented clean
 */
BOOL
WINAPI 
ConvertSystemTimeToCalDateTime(
  _In_   const LPSYSTEMTIME lpSystemTime,
  _In_   CALID calId,
  _Out_  LPCALDATETIME lpCalDateTime
)
{
	lpCalDateTime->CalId = calId;
	lpCalDateTime->Era=1;
	lpCalDateTime->Year=lpSystemTime->wYear;
	lpCalDateTime->Month= lpSystemTime->wMonth;
	lpCalDateTime->Day= lpSystemTime->wDay;
	lpCalDateTime->DayOfWeek=lpSystemTime->wDayOfWeek;
	lpCalDateTime->Hour=lpSystemTime->wHour;
	lpCalDateTime->Minute=lpSystemTime->wMinute;
	lpCalDateTime->Second=lpSystemTime->wSecond;
	lpCalDateTime->Tick=lpSystemTime->wMilliseconds*10000;
	return TRUE;
}

/*
 * @implemented clean
 */
ULONG 
WINAPI ConvertNLSDayOfWeekToWin32DayOfWeek(UINT DayOfWeek)
{
    int result;
	if(DayOfWeek <=6)
	{
		result = DayOfWeek!= 6 ? DayOfWeek + 1:0;
	}else{
		SetLastError(ERROR_INVALID_PARAMETER);
        result=-1;
	}
	return result;
}

/*
 * @is really implemented?
 */
ULONG
WINAPI GetCalendarWeekNumber(_In_ CALID calId, _In_ const LPCALDATETIME lpCalDateTime,const LPCALDATETIME lpCalDate,const LPCALDATETIME lpCal){
	return 1;
}
/*
 * @implemented clean
 */
ULONG 
WINAPI 
GetCalendarMonthsInYear(
	_In_ CALID calId, 
	_In_ UINT year, 
	_In_  UINT era
)
{
	if(IsValidGregorianYear(year,era))
	{
		return 12;
	}else{
		SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
	}
}

/*
 * @implemented clean 
 */
BOOL 
WINAPI 
GetCalendarSupportedDateRange(
	_In_   CALID Calendar,  
	_Out_  LPCALDATETIME lpCalMinDateTime,
	_Out_  LPCALDATETIME lpCalMaxDateTime
)
{
	if(lpCalMinDateTime && lpCalMaxDateTime){
	//Set min supported date:	
		lpCalMinDateTime->Era=1;
		lpCalMinDateTime->Year=1;
		lpCalMinDateTime->Month=1;
		lpCalMinDateTime->Day=1;
		lpCalMinDateTime->DayOfWeek=1;
		lpCalMinDateTime->Hour=0;
		lpCalMinDateTime->Minute=0;
		lpCalMinDateTime->Second=0;
		lpCalMinDateTime->Tick=0;
	//Set max supported date:	
		lpCalMaxDateTime->Era=1;
		lpCalMaxDateTime->Year=9999;
		lpCalMaxDateTime->Month=12;
		lpCalMaxDateTime->Day=31;
		lpCalMaxDateTime->DayOfWeek=5;
		lpCalMaxDateTime->Hour=23;
		lpCalMaxDateTime->Minute=59;
		lpCalMaxDateTime->Second=59;
		lpCalMaxDateTime->Tick=9999999;
		return TRUE;
	}else{
		SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
	}
}

/*
 * @implemented clean verify IsValidCalDateTime
 */
BOOL 
WINAPI 
GetCalendarDifferenceInDays(
	_In_ const LPCALDATETIME lpCalDateTimeFirst, 
	_In_ const LPCALDATETIME lpCalDateTimeSecond,
	 _Inout_ const LPCALDATETIME lpCalDateTimeResult
)
{
		int result;
		result = GetGregorianDifferenceInDays(lpCalDateTimeFirst,lpCalDateTimeSecond);
		lpCalDateTimeResult->CalId = result;
		return TRUE;
}

/*
 * @implemented
 */
ULONG 
WINAPI 
GetGregorianDifferenceInDays(
	const LPCALDATETIME lpCalDateTimeFirst, 
	const LPCALDATETIME lpCalDateTimeSecond
){
	int var;
	var = GetAbsoluteDateFromGregorian(lpCalDateTimeFirst->Year,lpCalDateTimeFirst->Month,lpCalDateTimeFirst->Day);
	return GetAbsoluteDateFromGregorian(lpCalDateTimeSecond->Year,lpCalDateTimeSecond->Month,lpCalDateTimeSecond->Day) - var;
}

BOOL 
WINAPI 
CompareCalendarDates(
	CALDATETIME *dateTimeOne, 
	CALDATETIME *dateTimeTwo, 
	int parameter
)
{
  int hum; // ebx@1
  BOOL result; // eax@2
  UINT five; // ecx@9
  UINT six; // edx@9
  UINT seven; // ecx@11
  UINT eigth; // edx@11
  UINT nine; // ecx@13
  UINT ten; // edx@13
  UINT eleven; // edi@16
  UINT twelve; // esi@16

  hum = parameter;
  if ( !parameter )
  {
    SetLastError(0x57u);
    return 0;
  }
  /*if ( !IsValidCalDateTime(dateTimeOne, &parameter) || !IsValidCalDateTime(dateTimeTwo, &parameter) )
    return 0;*/
  if ( dateTimeOne->CalId != dateTimeTwo->CalId )
  {
    SetLastError(0x57u);
    return 0;
  }
  five = dateTimeOne->Era;
  six = dateTimeTwo->Era;
  result = 1;
  if ( five > six )
    goto LABEL_17;
  if ( five < six )
  {
LABEL_15:
    hum = -1;
    return result;
  }
  seven = dateTimeOne->Year;
  eigth = dateTimeTwo->Year;
  if ( seven > eigth )
  {
LABEL_17:
    hum = 1;
    return result;
  }
  if ( seven < eigth )
    goto LABEL_15;
  nine = dateTimeOne->Month;
  ten = dateTimeTwo->Month;
  if ( nine > ten )
    goto LABEL_17;
  if ( nine < ten )
    goto LABEL_15;
  eleven = dateTimeOne->Day;
  twelve = dateTimeTwo->Day;
  if ( eleven > twelve )
    goto LABEL_17;
  hum = -(eleven < twelve);
  return result;
}

int 
WINAPI 
GetClientDynamicTimeZoneInformation(HANDLE hHandle)
{
  int other; // edx@1
  int result; // eax@1

  while ( 1 )
  {
    other = BaseStaticServerData->NlsUserInfo.iFirstDayOfWeek[42];
    result = BaseStaticServerData->NlsUserInfo.iFirstDayOfWeek[38];
    memcpy(hHandle, &BaseStaticServerData->NlsUserInfo.sYearMonth[56], 0x1B0u);
    if (BaseStaticServerData->NlsUserInfo.iFirstDayOfWeek[42] == other )
    {
      if ( other >= 0 )
        break;
    }
    _mm_pause();
  }
  return result;
}

BOOLEAN 
WINAPI 
RTL_DTZItoDTZI(PDYNAMIC_TIME_ZONE_INFORMATION information, PDYNAMIC_TIME_ZONE_INFORMATION recept)
{
  BOOLEAN result; // al@1

  recept->Bias = information->Bias;
  recept->StandardBias = information->StandardBias;
  recept->DaylightBias = information->DaylightBias;
  recept->StandardDate.wYear = information->StandardDate.wYear;
  recept->StandardDate.wMonth = information->StandardDate.wMonth;
  recept->StandardDate.wDayOfWeek = information->StandardDate.wMilliseconds;
  recept->StandardDate.wDay = information->StandardDate.wDayOfWeek;
  recept->StandardDate.wHour = information->StandardDate.wDay;
  recept->StandardDate.wMinute = information->StandardDate.wHour;
  recept->StandardDate.wSecond = information->StandardDate.wMinute;
  recept->StandardDate.wMilliseconds = information->StandardDate.wSecond;
  recept->DaylightDate.wYear = information->DaylightDate.wYear;
  recept->DaylightDate.wMonth = information->DaylightDate.wMonth;
  recept->DaylightDate.wDayOfWeek = information->DaylightDate.wMilliseconds;
  recept->DaylightDate.wDay = information->DaylightDate.wDayOfWeek;
  recept->DaylightDate.wHour = information->DaylightDate.wDay;
  recept->DaylightDate.wMinute = information->DaylightDate.wHour;
  recept->DaylightDate.wSecond = information->DaylightDate.wMinute;
  recept->DaylightDate.wMilliseconds = information->DaylightDate.wSecond;
  memcpy(recept->TimeZoneKeyName, information->TimeZoneKeyName, sizeof(recept->TimeZoneKeyName));
  result = information->DynamicDaylightTimeDisabled;
  recept->DynamicDaylightTimeDisabled = result;
  return result;
}

BOOLEAN 
WINAPI 
DTZItoRTL_DTZI(
	PDYNAMIC_TIME_ZONE_INFORMATION timeOne, 
	PDYNAMIC_TIME_ZONE_INFORMATION timeTwo
)
{
  BOOLEAN result; // al@1

  timeTwo->Bias = timeOne->Bias;
  timeTwo->StandardBias = timeOne->StandardBias;
  timeTwo->DaylightBias = timeOne->DaylightBias;
  memcpy(timeTwo->StandardName, timeOne->StandardName, sizeof(timeTwo->StandardName));
  memcpy(timeTwo->DaylightName, timeOne->DaylightName, sizeof(timeTwo->DaylightName));
  timeTwo->StandardDate.wYear = timeOne->StandardDate.wYear;
  timeTwo->StandardDate.wMonth = timeOne->StandardDate.wMonth;
  timeTwo->StandardDate.wMilliseconds = timeOne->StandardDate.wDayOfWeek;
  timeTwo->StandardDate.wDayOfWeek = timeOne->StandardDate.wDay;
  timeTwo->StandardDate.wDay = timeOne->StandardDate.wHour;
  timeTwo->StandardDate.wHour = timeOne->StandardDate.wMinute;
  timeTwo->StandardDate.wMinute = timeOne->StandardDate.wSecond;
  timeTwo->StandardDate.wSecond = timeOne->StandardDate.wMilliseconds;
  timeTwo->DaylightDate.wYear = timeOne->DaylightDate.wYear;
  timeTwo->DaylightDate.wMonth = timeOne->DaylightDate.wMonth;
  timeTwo->DaylightDate.wMilliseconds = timeOne->DaylightDate.wDayOfWeek;
  timeTwo->DaylightDate.wDayOfWeek = timeOne->DaylightDate.wDay;
  timeTwo->DaylightDate.wDay = timeOne->DaylightDate.wHour;
  timeTwo->DaylightDate.wHour = timeOne->DaylightDate.wMinute;
  timeTwo->DaylightDate.wMinute = timeOne->DaylightDate.wSecond;
  timeTwo->DaylightDate.wSecond = timeOne->DaylightDate.wMilliseconds;
  memcpy(timeTwo->TimeZoneKeyName, timeOne->TimeZoneKeyName, sizeof(timeTwo->TimeZoneKeyName));
  result = timeOne->DynamicDaylightTimeDisabled;
  timeTwo->DynamicDaylightTimeDisabled = result;
  return result;
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

BOOL 
WINAPI 
SetDynamicTimeZoneInformation(
	const DYNAMIC_TIME_ZONE_INFORMATION* lpTimeZoneDynamicInformation
)
{
	TIME_ZONE_INFORMATION lpTimeZoneInformation;

	RtlCopyMemory(&lpTimeZoneInformation.StandardName,&lpTimeZoneDynamicInformation->StandardName,sizeof(lpTimeZoneInformation.StandardName));
	RtlCopyMemory(&lpTimeZoneInformation.DaylightName,&lpTimeZoneDynamicInformation->DaylightName,sizeof(lpTimeZoneInformation.DaylightName));
	
	lpTimeZoneInformation.Bias = lpTimeZoneDynamicInformation->Bias;
	lpTimeZoneInformation.StandardDate = lpTimeZoneDynamicInformation->StandardDate;
	lpTimeZoneInformation.StandardBias = lpTimeZoneDynamicInformation->StandardBias;
	lpTimeZoneInformation.DaylightDate = lpTimeZoneDynamicInformation->DaylightDate;
	lpTimeZoneInformation.DaylightBias = lpTimeZoneDynamicInformation->DaylightBias;
	
	return SetTimeZoneInformation(&lpTimeZoneInformation);
}

/***********************************************************************
 *              GetTimeZoneInformationForYear  (KERNEL32.@)
 */
BOOL 
WINAPI 
GetTimeZoneInformationForYear( 
	USHORT wYear,
    PDYNAMIC_TIME_ZONE_INFORMATION pdtzi, 
	LPTIME_ZONE_INFORMATION ptzi
)
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

BOOL 
WINAPI 
GetCalendarDateFormatEx(
  _In_   LPCWSTR lpszLocale,
  _In_   DWORD dwFlags,
  _In_   const LPCALDATETIME lpCalDateTime,
  _In_   LPCWSTR lpFormat,
  _Out_  LPWSTR lpDateStr,
  _In_   int cchDate
)
{
	LPCWSTR dayofweek = L"";
	lpFormat = L"dddd, MMMM dd, yyyy";

	switch(lpCalDateTime->DayOfWeek)
	{
		case 0:
			dayofweek = L"Sunday";
			break;
		case 1:
			dayofweek = L"Monday";
			break;	
		case 2:
			dayofweek = L"Tuesday";
			break;	
		case 3:
			dayofweek = L"Wednesday";
			break;	
		case 4:
			dayofweek = L"Thursday";
			break;
		case 5:
			dayofweek = L"Friday";
			break;
		case 6:
			dayofweek = L"Saturday";
			break;
		default:
			break;			
	}
	wcscat(lpDateStr,dayofweek);
	wcscat(lpDateStr,L",");
	wcscat(lpDateStr,L" ");
	wcscat(lpDateStr,(LPCWSTR)lpCalDateTime->Month);
	wcscat(lpDateStr,L" ");
	wcscat(lpDateStr,(LPCWSTR)lpCalDateTime->Day);
	wcscat(lpDateStr,L",");
	wcscat(lpDateStr,L" ");
	wcscat(lpDateStr,(LPCWSTR)lpCalDateTime->Year);
	return TRUE;
}

//TODO
BOOL 
WINAPI 
GetCalendarDateFormat(
  _In_   LCID lpszLocale,
  _In_   DWORD dwFlags,
  _In_   const LPCALDATETIME lpCalDateTime,
  _In_   LPCWSTR lpFormat,
  _Out_  LPWSTR lpDateStr,
  _In_   int cchDate
)
{
  int result; // eax
  WCHAR LCData; // [esp+Ch] [ebp-B0h]

  result = LCIDToLocaleName(lpszLocale, &LCData, 85, 0);
  
  if(result){
	  return GetCalendarDateFormatEx(&LCData, dwFlags, lpCalDateTime, lpFormat, lpDateStr, cchDate);
  }
  return result;
}

BOOL 
WINAPI 
TzSpecificLocalTimeToSystemTimeEx(
  _In_opt_  const DYNAMIC_TIME_ZONE_INFORMATION *lpTimeZoneInformation,
  _In_      const SYSTEMTIME *lpLocalTime,
  _Out_     LPSYSTEMTIME lpUniversalTime
)
{
	LPTIME_ZONE_INFORMATION time_information = NULL;
	time_information->Bias = lpTimeZoneInformation->Bias;
	time_information->StandardName[32] = lpTimeZoneInformation->StandardName[32];
	time_information->StandardDate = lpTimeZoneInformation->StandardDate;
	time_information->StandardBias = lpTimeZoneInformation->StandardBias;
	time_information->DaylightName[32] = lpTimeZoneInformation->DaylightName[32];
	time_information->DaylightDate = lpTimeZoneInformation->DaylightDate;
	time_information->DaylightBias = lpTimeZoneInformation->DaylightBias;	
	return TzSpecificLocalTimeToSystemTime(time_information, (LPSYSTEMTIME)lpLocalTime, lpUniversalTime);
}

BOOL 
WINAPI 
SystemTimeToTzSpecificLocalTimeEx(
  _In_opt_  const DYNAMIC_TIME_ZONE_INFORMATION *lpTimeZoneInformation,
  _In_      const SYSTEMTIME *lpUniversalTime,
  _Out_     LPSYSTEMTIME lpLocalTime
)
{
	LPTIME_ZONE_INFORMATION time_information = NULL;
	time_information->Bias = lpTimeZoneInformation->Bias;
	time_information->StandardName[32] = lpTimeZoneInformation->StandardName[32];
	time_information->StandardDate = lpTimeZoneInformation->StandardDate;
	time_information->StandardBias = lpTimeZoneInformation->StandardBias;
	time_information->DaylightName[32] = lpTimeZoneInformation->DaylightName[32];
	time_information->DaylightDate = lpTimeZoneInformation->DaylightDate;
	time_information->DaylightBias = lpTimeZoneInformation->DaylightBias;	
	return SystemTimeToTzSpecificLocalTime(time_information, (LPSYSTEMTIME)lpUniversalTime, lpLocalTime);
}