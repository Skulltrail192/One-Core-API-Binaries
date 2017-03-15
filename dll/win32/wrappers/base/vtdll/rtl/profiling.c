 /*
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
Copyright (c) 2015  Microsoft Corporation 
 
Module Name:

    cnvint.c
 
 */
 
#define NDEBUG

#include <main.h> 

NTSTATUS 
NTAPI 
RtlDisableThreadProfiling(
	HANDLE PerformanceDataHandle
)
{
	DbgPrint("UNIMPLEMENTED: RtlDisableThreadProfiling");	
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
RtlEnableThreadProfiling(
	HANDLE ThreadHandle, 
	DWORD Flags, 
	DWORD64 HardwareCounters, 
	HANDLE PerformanceDataHandle
)
{
	DbgPrint("UNIMPLEMENTED: RtlEnableThreadProfiling");	
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
RtlQueryThreadProfiling(
	HANDLE HANDLE, 
	PBOOLEAN Enabled
)
{
	DbgPrint("UNIMPLEMENTED: RtlQueryThreadProfiling");	
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
RtlReadThreadProfilingData(
	HANDLE PerformanceDataHandle, 
	DWORD Flags, 
	PPERFORMANCE_DATA PerformanceData
)
{
	DbgPrint("UNIMPLEMENTED: RtlReadThreadProfilingData");	
	return STATUS_SUCCESS;
}