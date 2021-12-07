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
//#include <stdint.h> // portable: uint64_t   MSVC: __int64 
#include <time.h>
#include <unicode.h>

typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

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

int gettimeofday(struct timeval * tp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    NtQuerySystemTime( (PLARGE_INTEGER)&system_time );
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
    gettimeofday( &now);
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
NTSTATUS WINAPI RtlQueryDynamicTimeZoneInformation(PRTL_TIME_ZONE_INFORMATION tzinfo)
{
	return RtlQueryTimeZoneInformation(tzinfo);
}

/* FIXME: code duplication with kernel32/client/time.c */
ULONG
WINAPI
RtlGetTickCount(VOID)
{
	return ((ULONG)(UInt32x32To64(SharedUserData->TickCountLowDeprecated, \
								  SharedUserData->TickCountMultiplier) >> 24));
}

/******************************************************************************
 *  RtlQueryPerformanceCounter   [NTDLL.@]
 */
BOOL WINAPI DECLSPEC_HOTPATCH RtlQueryPerformanceCounter( LARGE_INTEGER *counter )
{
    NtQueryPerformanceCounter( counter, NULL );
    return TRUE;
}

/******************************************************************************
 *  RtlQueryPerformanceFrequency   [NTDLL.@]
 */
BOOL WINAPI DECLSPEC_HOTPATCH RtlQueryPerformanceFrequency( LARGE_INTEGER *frequency )
{
    frequency->QuadPart = TICKSPERSEC;
    return TRUE;
}