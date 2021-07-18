/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    eventlog.c

Abstract:

    Event Log Handle functions

Author:

    Skulltrail 18-July-2021

Revision History:

--*/

#include "main.h"

ULONG WINAPI
EventWriteEndScenario(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	//return EtwEventWriteEndScenario(RegHandle, EventDescriptor, UserDataCount, UserData);
	return ERROR_SUCCESS;
}

ULONG 
WINAPI
EventWriteStartScenario(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	//return EtwEventWriteStartScenario(RegHandle, EventDescriptor, UserDataCount, UserData);
	return ERROR_SUCCESS;
}

/* unimplemented*/
ULONG WINAPI EventWriteEx(
  _In_      REGHANDLE RegHandle,
  _In_      PCEVENT_DESCRIPTOR EventDescriptor,
  _In_      ULONG64 Filter,
  _In_      ULONG Flags,
  _In_opt_  LPCGUID ActivityId,
  _In_      LPCGUID RelatedActivityId,
  _In_      ULONG UserDataCount,
  _In_opt_  PEVENT_DATA_DESCRIPTOR UserData
)
{
	return ERROR_SUCCESS;
}

ULONG WINAPI EnableTraceEx(
  _In_      LPCGUID ProviderId,
  _In_opt_  LPCGUID SourceId,
  _In_      TRACEHANDLE TraceHandle,
  _In_      ULONG IsEnabled,
  _In_      UCHAR Level,
  _In_      ULONGLONG MatchAnyKeyword,
  _In_      ULONGLONG MatchAllKeyword,
  _In_      ULONG EnableProperty,
  _In_opt_  PEVENT_FILTER_DESCRIPTOR EnableFilterDesc
)
{
	return ERROR_SUCCESS;	
}

/* unimplemented*/
ULONG WINAPI EventAccessControl(
  _In_  LPGUID Guid,
  _In_  ULONG Operation,
  _In_  PSID Sid,
  _In_  ULONG Rights,
  _In_  BOOLEAN AllowOrDeny
)
{
	return ERROR_SUCCESS;
}

ULONG 
WINAPI
EnableTraceEx2(
  _In_     TRACEHANDLE              TraceHandle,
  _In_     LPCGUID                  ProviderId,
  _In_     ULONG                    ControlCode,
  _In_     UCHAR                    Level,
  _In_     ULONGLONG                MatchAnyKeyword,
  _In_     ULONGLONG                MatchAllKeyword,
  _In_     ULONG                    Timeout,
  _In_opt_ PENABLE_TRACE_PARAMETERS EnableParameters
)
{
	return ERROR_SUCCESS;
}