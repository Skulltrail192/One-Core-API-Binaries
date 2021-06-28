/*
 * Copyright (C) 2019 Daniel Lehman
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
 */

#ifndef _APISETTIMEZONE_
#define _APISETTIMEZONE_

#ifdef __cplusplus
extern "C" {
#endif

WINBASEAPI DWORD WINAPI EnumDynamicTimeZoneInformation(DWORD, DYNAMIC_TIME_ZONE_INFORMATION *);
WINBASEAPI DWORD WINAPI GetDynamicTimeZoneInformation(PDYNAMIC_TIME_ZONE_INFORMATION);
WINBASEAPI DWORD WINAPI GetDynamicTimeZoneInformationEffectiveYears(const DYNAMIC_TIME_ZONE_INFORMATION*,DWORD*,DWORD*);
WINBASEAPI DWORD WINAPI GetTimeZoneInformation(LPTIME_ZONE_INFORMATION);
WINBASEAPI BOOL  WINAPI GetTimeZoneInformationForYear(USHORT,DYNAMIC_TIME_ZONE_INFORMATION*,TIME_ZONE_INFORMATION*);
WINBASEAPI BOOL  WINAPI SetDynamicTimeZoneInformation(const DYNAMIC_TIME_ZONE_INFORMATION*);
WINBASEAPI BOOL  WINAPI SetTimeZoneInformation(const TIME_ZONE_INFORMATION *);
WINBASEAPI BOOL  WINAPI SystemTimeToTzSpecificLocalTime(const TIME_ZONE_INFORMATION*,const SYSTEMTIME*,LPSYSTEMTIME);
WINBASEAPI BOOL  WINAPI SystemTimeToTzSpecificLocalTimeEx(const DYNAMIC_TIME_ZONE_INFORMATION*,const SYSTEMTIME*,LPSYSTEMTIME);
WINBASEAPI BOOL  WINAPI TzSpecificLocalTimeToSystemTimeEx(const DYNAMIC_TIME_ZONE_INFORMATION*,const SYSTEMTIME*,LPSYSTEMTIME);

#ifdef __cplusplus
}
#endif

#endif  /* _APISETTIMEZONE_ */
