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
#define NDEBUG 
 
#include <main.h>
#include <time.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);


//************************************************* Time Date Functions **********************************************************

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
WINAPI IsValidCalDateTime(_In_ LPCALDATETIME lpCalDateTime,_In_ UINT amount)
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
 * @implemented cledan
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
 * @implemented cledan
 */
BOOL 
WINAPI IsCalendarLeapMonth(CALID calId, ULONG month, ULONG year, ULONG era)
{
	return IsGregorianLeapMonth(month, year, era);
}

/*
 * @implemented cledan
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
  CALDATETIME *localCalDateTime; // esi@1
  UINT localYear; // ebx@3
  UINT localMonth; // eax@3
  signed int localAmount; // ecx@3
  int localAmount2; // eax@4
  int localAmount3; // edi@4
  int localYearAmount; // ebx@6
  int dayCount; // eax@6
  NTSTATUS STATUS; // [sp-Ch] [bp-3Ch]@7
  UINT localEra; // [sp+8h] [bp-28h]@10
  int v16; // [sp+14h] [bp-1Ch]@10
  UINT localHour; // [sp+1Ch] [bp-14h]@10
  UINT localMinute; // [sp+20h] [bp-10h]@10
  UINT localSecond; // [sp+24h] [bp-Ch]@10
  ULONG localTick; // [sp+28h] [bp-8h]@10
  BOOL resp; // [sp+2Ch] [bp-4h]@1
  int localDay; // [sp+38h] [bp+8h]@3

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
WINAPI AdjustCalendarDate(_Inout_ LPCALDATETIME lpCalDateTime,
						  _In_ CALDATETIME_DATEUNIT calUnit,
						   int amount)
{
  	BOOL resp= FALSE;
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
WINAPI ConvertCalDateTimeToSystemTime(
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
WINAPI ConvertSystemTimeToCalDateTime(
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
 * @implemented totally
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
WINAPI GetCalendarSupportedDateRange(
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


LPCWSTR dayofweek = 0; //Hack for work on MSVC
/*
 * @implemented
 */
ULONG 
WINAPI GetGregorianDifferenceInDays(const LPCALDATETIME lpCalDateTimeFirst, const LPCALDATETIME lpCalDateTimeSecond){
	int var;
	var = GetAbsoluteDateFromGregorian(lpCalDateTimeFirst->Year,lpCalDateTimeFirst->Month,lpCalDateTimeFirst->Day);
	return GetAbsoluteDateFromGregorian(lpCalDateTimeSecond->Year,lpCalDateTimeSecond->Month,lpCalDateTimeSecond->Day) - var;
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
	StringCchCat(lpDateStr,256,dayofweek);
	StringCchCat(lpDateStr,256,L",");
	StringCchCat(lpDateStr,256,L" ");
	StringCchCat(lpDateStr,256,(LPCWSTR)lpCalDateTime->Month);
	StringCchCat(lpDateStr,256,L" ");
	StringCchCat(lpDateStr,256,(LPCWSTR)lpCalDateTime->Day);
	StringCchCat(lpDateStr,256,L",");
	StringCchCat(lpDateStr,256,L" ");
	StringCchCat(lpDateStr,256,(LPCWSTR)lpCalDateTime->Year);
	return TRUE;
}

/*
 * @implemented - mudar para time.c
 */
ULONGLONG
WINAPI
GetTickCount64(VOID)
{
    ULARGE_INTEGER TickCount;
    
    while (TRUE)
    {
        TickCount.HighPart = (ULONG)SharedUserData->TickCount.High1Time;
        TickCount.LowPart = SharedUserData->TickCount.LowPart;

        if (TickCount.HighPart == (ULONG)SharedUserData->TickCount.High2Time) break;

        YieldProcessor();
     }
     
     return (UInt32x32To64(TickCount.LowPart, SharedUserData->TickCountMultiplier) >> 24) +
            (UInt32x32To64(TickCount.HighPart, SharedUserData->TickCountMultiplier) << 8);
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

BOOL 
WINAPI 
IsTimeZoneRedirectionEnabled()
{
  return BaseStaticServerData->NlsUserInfo.iFirstDayOfWeek[38] != -1;
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

BOOLEAN WINAPI RTL_DTZItoDTZI(PDYNAMIC_TIME_ZONE_INFORMATION information, PDYNAMIC_TIME_ZONE_INFORMATION recept)
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

DWORD 
WINAPI 
GetDynamicTimeZoneInformationRaw(
	PDYNAMIC_TIME_ZONE_INFORMATION systemTime, 
	BOOL comparation
)
{
  NTSTATUS status; // eax@2
  DWORD resp; // edi@3
  PVOID SystemInformation; // [sp+8h] [bp-1B4h]@2

  if ( IsTimeZoneRedirectionEnabled() )
  {
    resp = GetClientDynamicTimeZoneInformation(&SystemInformation);
  }
  else
  {
    status = NtQuerySystemInformation(SystemLoadAndCallImage, &SystemInformation, 0x1B0u, 0);
    if ( status < 0 )
    {
      BaseSetLastNTError(status);
      return -1;
    }
    resp = TIME_ZONE_ID_INVALID;
  }
  RTL_DTZItoDTZI(SystemInformation, systemTime);
  return resp;
}

DWORD WINAPI GetDynamicTimeZoneInformation(PDYNAMIC_TIME_ZONE_INFORMATION pTimeZoneInformation)
{
  return GetDynamicTimeZoneInformationRaw(pTimeZoneInformation, 1);
}

LONG WINAPI OpenTzSpecificCache()
{
  PVOID alloc; // eax@3
  LONG hum; // edi@4
  LONG resp; // esi@1

  resp = TzSpecificCache;
  if ( !TzSpecificCache )
  {
   #ifdef _M_IX86
		alloc = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), (BaseDllTag + 0x280000) | 8, 0x748u);
   #elif defined(_M_AMD64)
		alloc = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), (BaseDllTag + 0x280000) | 8, 0x748u);
   #endif 
    resp = (LONG)alloc;
    if ( alloc )
    {
      RtlInitializeSRWLock(alloc);
      hum = InterlockedCompareExchange(&TzSpecificCache, resp, 0);
      if ( hum )
      {
		#ifdef _M_IX86
			RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), BaseDllTag + 0x280000, (PVOID)resp);
	    #elif defined(_M_AMD64)
			RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), BaseDllTag + 0x280000, (PVOID)resp);
	    #endif 
        resp = hum;
      }
    }
  }
  return resp;
}

LONG WINAPI InvalidateTzSpecificCache()
{
  LONG result; // eax@1
  PRTL_SRWLOCK resp; // esi@1

  result = OpenTzSpecificCache();
  resp = (PRTL_SRWLOCK)result;
  if ( result )
  {
    RtlAcquireSRWLockExclusive((PRTL_SRWLOCK)result);
    resp[1].Ptr = 0;
    memset(&resp[2], 0, 0x740u);
    RtlReleaseSRWLockExclusive(resp);
  }
  return result;
}

NTSTATUS WINAPI CsrBasepSetClientTimeZoneInformation(LONG parameterOne, PDYNAMIC_TIME_ZONE_INFORMATION other, LONG otherParameter)
{
  PCSR_CAPTURE_BUFFER  capture; // ebx@1
  NTSTATUS client; // esi@2
  NTSTATUS result; // eax@2
  PCSR_API_MESSAGE  message = NULL; // [sp+8h] [bp-D8h]@2
  PVOID captureData; // [sp+30h] [bp-B0h]@2
  LONG one; // [sp+38h] [bp-A8h]@2
  LONG two; // [sp+3Ch] [bp-A4h]@2
  LONG three; // [sp+40h] [bp-A0h]@2
  LONG parameter; // [sp+44h] [bp-9Ch]@2

  capture = CsrAllocateCaptureBuffer(1, 432);
  if ( capture )
  {
    CsrCaptureMessageBuffer(capture, (PVOID)parameterOne, 432, &captureData);
    parameter = otherParameter;
    one = other->Bias;
    two = other->StandardName[0];
    three = other->StandardName[2];
    client = CsrClientCallServer(message, capture, 65558, 24);
    CsrFreeCaptureBuffer(capture);
    result = client;
  }
  else
  {
    result = 0xC0000017u;
  }
  return result;
}

DWORD WINAPI CalcClientTimeZoneIdAndBias(PDYNAMIC_TIME_ZONE_INFORMATION timeZone, LONG *parameterTwo)
{
  PDYNAMIC_TIME_ZONE_INFORMATION v2; // ebx@1
  DWORD resp; // esi@1
  LONG v4; // qax@1
  BOOL verification; // zf@1
  __int32 v7; // eax@7
  LONG v8; // qdi@7
  int v9; // ebx@7
  LONG v10; // qax@7
  int v11; // eax@12
  int v12; // [sp+Ch] [bp-50h]@3
  int v13; // [sp+10h] [bp-4Ch]@3
  int v14; // [sp+14h] [bp-48h]@3
  int v15; // [sp+18h] [bp-44h]@3
  int v16; // [sp+1Ch] [bp-40h]@3
  int v17; // [sp+20h] [bp-3Ch]@3
  int v18; // [sp+24h] [bp-38h]@3
  int v19; // [sp+28h] [bp-34h]@3
  struct _SYSTEMTIME SystemTime; // [sp+2Ch] [bp-30h]@3
  LONG v21 = 0; // [sp+3Ch] [bp-20h]@3
  LONG v22 = 0; // [sp+44h] [bp-18h]@6
  LONG v23; // [sp+4Ch] [bp-10h]@1
  struct _FILETIME FileTime; // [sp+54h] [bp-8h]@3
  __int32 timeZonea; // [sp+64h] [bp+8h]@7

  v2 = timeZone;
  resp = 0;
  v4 = 600000000i64 * timeZone->Bias;
  verification = timeZone->StandardDate.wMonth == 0;
  v23 = 600000000i64 * timeZone->Bias;
  if ( verification || !timeZone->DaylightDate.wMonth )
    goto LABEL_14;
  GetSystemTime(&SystemTime);
  SystemTimeToFileTime(&SystemTime, &FileTime);
  v16 = *&timeZone->StandardDate.wYear;
  v17 = *&timeZone->StandardDate.wDayOfWeek;
  v18 = *&timeZone->StandardDate.wHour;
  v19 = *&timeZone->StandardDate.wSecond;
  v12 = *&timeZone->DaylightDate.wYear;
  v13 = *&timeZone->DaylightDate.wDayOfWeek;
  v14 = *&timeZone->DaylightDate.wHour;
  v15 = *&timeZone->DaylightDate.wSecond;
  v7 = 60 * timeZone->StandardBias;
  timeZonea = v7;
  v8 = v23 + v22 + 10000000i64 * v7;
  v9 = 60 * v2->DaylightBias;
  v10 = v23 + v21 + 10000000i64 * v9;
  if ( v8 >= v10 )
  {
    if (FileTime.dwHighDateTime <= v8
      && (FileTime.dwHighDateTime < v8 || FileTime.dwLowDateTime < v8 ))
      goto LABEL_12;
LABEL_19:
    resp = 2;
    v11 = v9;
    goto LABEL_13;
  }
  if (FileTime.dwHighDateTime <= v10
    && (FileTime.dwHighDateTime < v10 || FileTime.dwLowDateTime < v10) )
    goto LABEL_19;
LABEL_12:
  v11 = timeZonea;
  resp = 1;
LABEL_13:
  v4 = v23 + 10000000i64 * v11;
LABEL_14:
  *parameterTwo = v4;
  parameterTwo[2] = v4;
  parameterTwo[1] = v4;
  return resp;
}

BOOL WINAPI SetClientDynamicTimeZoneInformation(PDYNAMIC_TIME_ZONE_INFORMATION time_zone)
{
  DWORD compose; // eax@1
  NTSTATUS status; // eax@1
  BOOL result; // eax@2
  DWORD error; // eax@4
  LONG number; // [sp+0h] [bp-Ch]@1

  compose = CalcClientTimeZoneIdAndBias(time_zone, &number);
  status = CsrBasepSetClientTimeZoneInformation( number, time_zone, compose);
  if ( status < 0 )
  {
    error = RtlNtStatusToDosError(status);
    SetLastError(error);
    result = 0;
  }
  else
  {
    result = 1;
  }
  return result;
}

BOOL WINAPI SetDynamicTimeZoneInformation(PDYNAMIC_TIME_ZONE_INFORMATION lpTimeZoneInformation)
{
  BOOL verification; // esi@2
  NTSTATUS status; // eax@3
  PDYNAMIC_TIME_ZONE_INFORMATION SystemInformation = NULL; // [sp+4h] [bp-1B4h]@1

  DTZItoRTL_DTZI(lpTimeZoneInformation, SystemInformation);
  if ( IsTimeZoneRedirectionEnabled() )
  {
    verification = SetClientDynamicTimeZoneInformation(SystemInformation);
  }
  else
  {
    status = NtSetSystemInformation(SystemLoadAndCallImage, &SystemInformation, 0x1B0u);
    if ( status >= 0 )
    {
      verification = 1;
    }
    else
    {
      BaseSetLastNTError(status);
      verification = 0;
    }
  }
  InvalidateTzSpecificCache();
  return verification;
}

BOOL WINAPI GetTimeZoneInformationForYear(USHORT wYear, PDYNAMIC_TIME_ZONE_INFORMATION pdtzi, LPTIME_ZONE_INFORMATION ptzi)
{
  UNIMPLEMENTED;
  return TRUE;
}

BOOL WINAPI GetCalendarDateFormatEx(
  _In_   LPCWSTR lpszLocale,
  _In_   DWORD dwFlags,
  _In_   const LPCALDATETIME lpCalDateTime,
  _In_   LPCWSTR lpFormat,
  _Out_  LPWSTR lpDateStr,
  _In_   int cchDate
)
{
  UNIMPLEMENTED;
  return TRUE;
}

BOOL 
WINAPI 
QueryUnbiasedInterruptTime(
  _Out_  PULONGLONG UnbiasedTime
)
{
    UNIMPLEMENTED;
	return TRUE;
}

BOOL WINAPI TzSpecificLocalTimeToSystemTimeEx(
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

VOID 
WINAPI 
GetSystemTimePreciseAsFileTime(
	LPFILETIME lpSystemTimeAsFileTime
)
{
  GetSystemTimeAsFileTime(lpSystemTimeAsFileTime);
}
/*EOF*/