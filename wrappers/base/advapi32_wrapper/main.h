
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
#include <perflib.h>
#include <ndk/cmfuncs.h>
#include <wine/config.h>
#include <accctrl.h>
#include <aclapi.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <winuser.h>
#include <wine/debug.h>
#include <wine/unicode.h>
#include <wine/list.h>
#include <wine/heap.h>

#define SECURITY_APP_PACKAGE_AUTHORITY {0,0,0,0,0,15}
#define SECURITY_APP_PACKAGE_BASE_RID           __MSABI_LONG(0x000000002)
#define SECURITY_BUILTIN_PACKAGE_ANY_PACKAGE    __MSABI_LONG(0x000000001)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* FUNCTIONS ****************************************************************/
FORCEINLINE
BOOL
IsHKCRKey(_In_ HKEY hKey)
{
    return ((ULONG_PTR)hKey & 0x2) != 0;
}

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

#define ARGUMENT_PRESENT(ArgumentPointer)((CHAR*)((ULONG_PTR)(ArgumentPointer)) != (CHAR*)NULL)

typedef VOID( CALLBACK * PFN_SC_NOTIFY_CALLBACK ) (
    IN PVOID pParameter 
);

typedef PVOID LSA_HANDLE, *PLSA_HANDLE;

typedef ULONG64 TRACEHANDLE,*PTRACEHANDLE;

typedef enum _EVENT_INFO_CLASS { 
  EventProviderBinaryTrackInfo    = 0,
  EventProviderSetTraits          = 1,
  EventProviderUseDescriptorType  = 2,
  MaxEventInfo                    = 3
} EVENT_INFO_CLASS;

/* set last error code from NT status and get the proper boolean return value */
 /* used for functions that are a simple wrapper around the corresponding ntdll API */
 static inline BOOL set_ntstatus( NTSTATUS status )
 {
     if (status) SetLastError( RtlNtStatusToDosError( status ));
     return !status;
}

typedef enum _TAG_INFO_LEVEL {
    TagInfoLevelNameFromTag = 1,
} TAG_INFO_LEVEL;

typedef enum _TAG_TYPE {
    TagTypeService = 1,
} TAG_TYPE;

typedef struct _TAG_INFO_NAME_FROM_TAG_IN_PARAMS {
    DWORD dwPid;
    DWORD dwTag;
} TAG_INFO_NAME_FROM_TAG_IN_PARAMS, *PTAG_INFO_NAME_FROM_TAG_IN_PARAMS;

typedef struct _TAG_INFO_NAME_FROM_TAG_OUT_PARAMS {
    TAG_TYPE TagType;
    LPWSTR pszName;
} TAG_INFO_NAME_FROM_TAG_OUT_PARAMS, *PTAG_INFO_NAME_FROM_TAG_OUT_PARAMS;

typedef struct _TAG_INFO_NAME_FROM_TAG
{
    TAG_INFO_NAME_FROM_TAG_IN_PARAMS InParams;
    TAG_INFO_NAME_FROM_TAG_OUT_PARAMS OutParams;
} TAG_INFO_NAME_FROM_TAG, *PTAG_INFO_NAME_FROM_TAG;

typedef struct _PERF_COUNTER_INFO {
  ULONG     CounterId;
  ULONG     Type;
  ULONGLONG Attrib;
  ULONG     Size;
  ULONG     DetailLevel;
  LONG      Scale;
  ULONG     Offset;
} PERF_COUNTER_INFO, *PPERF_COUNTER_INFO;

typedef enum
{
    ADS_RIGHT_DS_CREATE_CHILD         = 0x00000001,
    ADS_RIGHT_DS_DELETE_CHILD         = 0x00000002,
    ADS_RIGHT_ACTRL_DS_LIST           = 0x00000004,
    ADS_RIGHT_DS_SELF                 = 0x00000008,
    ADS_RIGHT_DS_READ_PROP            = 0x00000010,
    ADS_RIGHT_DS_WRITE_PROP           = 0x00000020,
    ADS_RIGHT_DS_DELETE_TREE          = 0x00000040,
    ADS_RIGHT_DS_LIST_OBJECT          = 0x00000080,
    ADS_RIGHT_DS_CONTROL_ACCESS       = 0x00000100,

    ADS_RIGHT_DELETE                  = 0x00010000,
    ADS_RIGHT_READ_CONTROL            = 0x00020000,
    ADS_RIGHT_WRITE_DAC               = 0x00040000,
    ADS_RIGHT_WRITE_OWNER             = 0x00080000,
    ADS_RIGHT_SYNCHRONIZE             = 0x00100000,
    ADS_RIGHT_ACCESS_SYSTEM_SECURITY  = 0x00200000,

    ADS_RIGHT_GENERIC_ALL             = 0x10000000,
    ADS_RIGHT_GENERIC_EXECUTE         = 0x20000000,
    ADS_RIGHT_GENERIC_WRITE           = 0x40000000,
    ADS_RIGHT_GENERIC_READ            = 0x80000000
} ADS_RIGHTS_ENUM;

/* memory allocation functions */

static inline WCHAR *strdupAW( const char *src )
{
    WCHAR *dst = NULL;
    if (src)
    {
        DWORD len = MultiByteToWideChar( CP_ACP, 0, src, -1, NULL, 0 );
        if ((dst = heap_alloc( len * sizeof(WCHAR) ))) MultiByteToWideChar( CP_ACP, 0, src, -1, dst, len );
    }
    return dst;
}