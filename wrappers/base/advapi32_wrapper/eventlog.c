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

typedef BOOL (WINAPI *ELF_REPORT_EVENT_AND_SOURCE)(
						 HANDLE,
                         ULONG,
                         PUNICODE_STRING,
                         USHORT,
                         USHORT,
                         ULONG,
                         PSID,
                         PUNICODE_STRING,
                         USHORT,
                         ULONG,
                         PUNICODE_STRING*,
                         PVOID,
                         USHORT,
                         PULONG,
                         PULONG);

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

/*
 * @implemented
 */
NTSTATUS
NTAPI
ElfReportEventAndSourceW(
	IN HANDLE hEventLog,
    IN ULONG Time,
    IN PUNICODE_STRING ComputerName,
    IN USHORT EventType,
    IN USHORT EventCategory,
    IN ULONG EventID,
    IN PSID UserSID,
    IN PUNICODE_STRING SourceName,
    IN USHORT NumStrings,
    IN ULONG DataSize,
    IN PUNICODE_STRING* Strings,
    IN PVOID Data,
    IN USHORT Flags,
    IN OUT PULONG RecordNumber,
    IN OUT PULONG TimeWritten)
{
	ELF_REPORT_EVENT_AND_SOURCE elfReportEventAndSource;
	
    elfReportEventAndSource = (ELF_REPORT_EVENT_AND_SOURCE) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase.dll")),
                            "ElfReportEventAndSourceW");
    if (NULL == elfReportEventAndSource) 
    {	

		return STATUS_NOT_IMPLEMENTED;

    }else{
		return (NTSTATUS)elfReportEventAndSource(hEventLog, 
											Time,
											ComputerName,
											EventType,
											EventCategory,
											EventID,
											UserSID,
											SourceName,
											NumStrings,
											DataSize,
											Strings,
											Data,
											Flags,
											RecordNumber,
											TimeWritten);
	}
}	