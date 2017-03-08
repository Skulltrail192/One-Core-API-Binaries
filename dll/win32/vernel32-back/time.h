#define WIN32_NO_STATUS

#include <k32.h>

#define ERROR_DATE_OUT_OF_RANGE 32

int GregorianDaysToMonth365 [12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int GregorianDaysToMonth366 [12] = {31,29,31,30,31,30,31,31,30,31,30,31}; 


typedef struct _caldatetime 
{
  CALID CalId;
  UINT  Era;
  UINT  Year;
  UINT  Month;
  UINT  Day;
  UINT  DayOfWeek;
  UINT  Hour;
  UINT  Minute;
  UINT  Second;
  ULONG Tick;
}CALDATETIME, *LPCALDATETIME;

typedef enum _CALDATETIME_DATEUNIT 
{  
  EraUnit = 0x0,
  YearUnit = 0x1,
  MonthUnit = 0x2,
  WeekUnit = 0x3,
  DayUnit = 0x4,
  HourUnit = 0x5,
  MinuteUnit = 0x6,
  SecondUnit = 0x7,
  TickUnit = 0x8,
}CALDATETIME_DATEUNIT;


typedef struct _TIME_DYNAMIC_ZONE_INFORMATION {
  LONG       Bias;
  WCHAR      StandardName[32];
  SYSTEMTIME StandardDate;
  LONG       StandardBias;
  WCHAR      DaylightName[32];
  SYSTEMTIME DaylightDate;
  LONG       DaylightBias;
  WCHAR      TimeZoneKeyName[128];
  BOOLEAN    DynamicDaylightTimeDisabled;
} DYNAMIC_TIME_ZONE_INFORMATION, *PDYNAMIC_TIME_ZONE_INFORMATION;

/*-------------------------------------TIME DATE FUNCTIONS -----------------------------------------*/
ULONG WINAPI GetCalendarDaysInMonth(CALID calId,  UINT month, UINT year, UINT era);
BOOL WINAPI IsValidGregorianTime(IN LPCALDATETIME lpCalDateTime, UINT a2);
BOOL WINAPI IsGregorianLeapYear(UINT year);
ULONG WINAPI GetGregorianDifferenceInDays(const LPCALDATETIME lpCalDateTimeFirst, const LPCALDATETIME lpCalDateTimeSecond);
ULONG WINAPI GetGregorianDayOfWeekHelper(IN LPCALDATETIME lpCalDateTime);
BOOL WINAPI UpdateCalendarDayOfWeek(_Out_ LPCALDATETIME lpCalDateTime);
BOOL WINAPI NlsGregorianLeapYear(IN UINT year);
ULONG WINAPI GetAbsoluteDateFromGregorian(UINT year, UINT month, UINT day);
BOOL WINAPI IsValidGregorianYear(IN UINT year, IN UINT era);
BOOL WINAPI IsGregorianLeapMonth(ULONG month, ULONG year, int era);
BOOL WINAPI IsCalendarLeapMonth(CALID calId, ULONG month, ULONG year, ULONG era);
int WINAPI LastDayOfGregorianMonth( int month, int year );
BOOL WINAPI AdjustGregorianDays(UINT absoluteDate, LPCALDATETIME lpCalDateTime);
BOOL WINAPI AdjustGregorianYearsAndMonths(LPCALDATETIME lpCalDateTime, int amount);
BOOL WINAPI IsValidGregorianMonthAndDay(UINT day, UINT month, UINT year, UINT amount);
BOOL WINAPI IsCalendarLeapDay(CALID calId, UINT day, UINT month, UINT year, UINT era);
BOOL WINAPI IsValidGregorianDateTime(LPCALDATETIME lpCalDateTime, UINT amount, UINT parameter);
BOOL WINAPI IsValidCalDateTime(_In_ LPCALDATETIME lpCalDateTime,_In_ CALID calId);
BOOL WINAPI IsCalendarLeapYear(_In_ CALID calId, _In_  UINT year, _In_  UINT era);
BOOL WINAPI CopyCalDateTime(CALDATETIME *lpCalDateTime, CALID *calId);
BOOL WINAPI AdjustCalendarDate(_Inout_ LPCALDATETIME lpCalDateTime,	_In_ CALDATETIME_DATEUNIT calUnit, int amount);
BOOL WINAPI ConvertCalDateTimeToSystemTime(_In_   const LPCALDATETIME lpCalDateTime, _Out_  SYSTEMTIME *lpSystemTime);
BOOL WINAPI ConvertSystemTimeToCalDateTime(_In_   const LPSYSTEMTIME lpSystemTime, _In_   CALID calId, _Out_  LPCALDATETIME lpCalDateTime);
ULONG WINAPI ConvertNLSDayOfWeekToWin32DayOfWeek(UINT DayOfWeek);
BOOL WINAPI GetCalendarSupportedDateRange(_In_   CALID Calendar, _Out_  LPCALDATETIME lpCalMinDateTime, _Out_  LPCALDATETIME lpCalMaxDateTime);
BOOL WINAPI GetCalendarDifferenceInDays(_In_ const LPCALDATETIME lpCalDateTimeFirst, const _In_ LPCALDATETIME lpCalDateTimeSecond, _Inout_ LPCALDATETIME lpCalDateTimeResult);
BOOL WINAPI IsValidYearAndMonth(LPCALDATETIME lpCalDateTime);
