
#pragma once

/* PSDK/NDK Headers */
#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <rtl.h>
#include <ndk/rtlfuncs.h>
#include <winsvc.h>
#include <evntprov.h>
#include <ntsecapi.h>
#include <evtlib.h>
#include <strsafe.h>
#include <ndk/cmfuncs.h>
#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <winuser.h>
#include <wine/debug.h>
#include <wine/unicode.h>

typedef PVOID IELF_HANDLE;

typedef ULONG64 TRACEHANDLE, *PTRACEHANDLE;

typedef struct _ENABLE_TRACE_PARAMETERS
{
    ULONG                            Version;
    ULONG                            EnableProperty;
    ULONG                            ControlFlags;
    GUID                             SourceId;
    struct _EVENT_FILTER_DESCRIPTOR *EnableFilterDesc;
    ULONG                            FilterDescCount;
} ENABLE_TRACE_PARAMETERS, *PENABLE_TRACE_PARAMETERS;

typedef struct _RPC_UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    wchar_t *Buffer;
} RPC_UNICODE_STRING, *PRPC_UNICODE_STRING;

typedef struct _RPC_SID
{
    UCHAR Revision;
    UCHAR SubAuthorityCount;
    SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
    DWORD SubAuthority[];
} RPC_SID, *PRPC_SID;

/* Defined in evtlib.h */
// #define LOGFILE_SIGNATURE   0x654c664c  // "LfLe"

typedef struct _LOGFILE
{
    EVTLOGFILE LogFile;
    HANDLE FileHandle;
    WCHAR *LogName;
    RTL_RESOURCE Lock;
    BOOL Permanent;
    LIST_ENTRY ListEntry;
} LOGFILE, *PLOGFILE;

typedef struct _EVENTSOURCE
{
    LIST_ENTRY EventSourceListEntry;
    PLOGFILE LogFile;
    WCHAR szName[1];
} EVENTSOURCE, *PEVENTSOURCE;

typedef struct _LOGHANDLE
{
    LIST_ENTRY LogHandleListEntry;
    PEVENTSOURCE EventSource;
    PLOGFILE LogFile;
    ULONG CurrentRecord;
    ULONG Flags;
    WCHAR szName[1];
} LOGHANDLE, *PLOGHANDLE;


static __inline void LogfFreeRecord(PEVENTLOGRECORD Record)
{
    RtlFreeHeap(GetProcessHeap(), 0, Record);
}

DWORD
WINAPI
BaseSetLastNTError(IN NTSTATUS Status);

/* EOF */
