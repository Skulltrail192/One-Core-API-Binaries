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

// /* return a monotonic time counter, in Win32 ticks */
// static ULONGLONG monotonic_counter(void)
// {
    // struct timeval now;

// #ifdef HAVE_CLOCK_GETTIME
    // struct timespec ts;
// #ifdef CLOCK_MONOTONIC_RAW
    // if (!clock_gettime( CLOCK_MONOTONIC_RAW, &ts ))
        // return ts.tv_sec * (ULONGLONG)TICKSPERSEC + ts.tv_nsec / 100;
// #endif
    // if (!clock_gettime( CLOCK_MONOTONIC, &ts ))
        // return ts.tv_sec * (ULONGLONG)TICKSPERSEC + ts.tv_nsec / 100;
// #elif defined(__APPLE__)
    // static mach_timebase_info_data_t timebase;

    // if (!timebase.denom) mach_timebase_info( &timebase );
    // return mach_absolute_time() * timebase.numer / timebase.denom / 100;
// #endif

    // gettimeofday( &now, 0 );
    // return now.tv_sec * (ULONGLONG)TICKSPERSEC + now.tv_usec * 10 + TICKS_1601_TO_1970 - server_start_time;
// }

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
    //*time = monotonic_counter();
    return STATUS_SUCCESS;
}