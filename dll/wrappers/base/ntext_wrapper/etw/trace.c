/*
 * ntdll.dll Event Tracing Functions
 */
 
#include <main.h> 
#include <wmistr.h>
#include <evntrace.h>

#define NDEBUG
#include <debug.h>

PVOID EtwpDumpHardwareConfig;
PVOID EtwpDumpHWConfig;

/*
 * @unimplemented
 */
 
ULONG 
NTAPI
EtwTraceMessage(
    TRACEHANDLE  SessionHandle,
    ULONG        MessageFlags,
    LPCGUID      MessageGuid,
    USHORT       MessageNumber)
{
    DbgPrint("TraceMessage()\n");
    return ERROR_SUCCESS;
}

TRACEHANDLE
NTAPI
EtwGetTraceLoggerHandle(
    PVOID Buffer
)
{
    DbgPrint("EtwGetTraceLoggerHandle stub()\n");
    return (TRACEHANDLE)-1;
}


ULONG
NTAPI
EtwTraceEvent(
    TRACEHANDLE SessionHandle,
    PEVENT_TRACE_HEADER EventTrace
)
{
    DbgPrint("EtwTraceEvent stub()\n");

    if (!SessionHandle || !EventTrace)
    {
        /* invalid parameters */
        return ERROR_INVALID_PARAMETER;
    }

    if (EventTrace->Size != sizeof(EVENT_TRACE_HEADER))
    {
        /* invalid parameter */
        return ERROR_INVALID_PARAMETER;
    }

    return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwGetTraceEnableFlags(
    TRACEHANDLE TraceHandle
)
{
    DbgPrint("EtwGetTraceEnableFlags stub()\n");
    return 0xFF;
}

UCHAR
NTAPI
EtwGetTraceEnableLevel(
    TRACEHANDLE TraceHandle
)
{
    DbgPrint("EtwGetTraceEnableLevel stub()\n");
    return 0xFF;
}

ULONG
NTAPI
EtwUnregisterTraceGuids(
    TRACEHANDLE RegistrationHandle
)
{
    DbgPrint("EtwUnregisterTraceGuids stub()\n");
    return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwRegisterTraceGuidsA(
    WMIDPREQUEST RequestAddress,
    PVOID RequestContext,
    LPCGUID ControlGuid,
    ULONG GuidCount,
    PTRACE_GUID_REGISTRATION TraceGuidReg,
    LPCSTR MofImagePath,
    LPCSTR MofResourceName,
    PTRACEHANDLE RegistrationHandle
)
{
    DbgPrint("EtwRegisterTraceGuidsA stub()\n");
    return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwRegisterTraceGuidsW(
    WMIDPREQUEST RequestAddress,
    PVOID RequestContext,
    LPCGUID ControlGuid,
    ULONG GuidCount,
    PTRACE_GUID_REGISTRATION TraceGuidReg,
    LPCWSTR MofImagePath,
    LPCWSTR MofResourceName,
    PTRACEHANDLE RegistrationHandle
)
{
    DbgPrint("EtwRegisterTraceGuidsW stub()\n");
    return ERROR_SUCCESS;
}

ULONG WINAPI EtwStartTraceW( PTRACEHANDLE pSessionHandle, LPCWSTR SessionName, PEVENT_TRACE_PROPERTIES Properties )
{
    DbgPrint("(%p, %s, %p) stub\n", pSessionHandle, SessionName, Properties);
    if (pSessionHandle) *pSessionHandle = 0xcafe4242;
    return ERROR_SUCCESS;
}

ULONG WINAPI EtwStartTraceA( PTRACEHANDLE pSessionHandle, LPCSTR SessionName, PEVENT_TRACE_PROPERTIES Properties )
{
    DbgPrint("(%p, %s, %p) stub\n", pSessionHandle, SessionName, Properties);
    if (pSessionHandle) *pSessionHandle = 0xcafe4242;
    return ERROR_SUCCESS;
}

/******************************************************************************
 * EtwControlTraceW [NTDLL.@]
 *
 * Control a givel event trace session
 *
 */
ULONG 
WINAPI
EtwControlTraceW( 
	TRACEHANDLE hSession, 
	LPCWSTR SessionName, 
	PEVENT_TRACE_PROPERTIES Properties, 
	ULONG control 
)
{
    DbgPrint("(%I64x, %s, %p, %d) stub\n", hSession, SessionName, Properties, control);
    return ERROR_SUCCESS;
}

/******************************************************************************
 * EtwControlTraceA [NTDLL.@]
 *
 * See ControlTraceW.
 *
 */
ULONG 
WINAPI 
EtwControlTraceA( 
	TRACEHANDLE hSession, 
	LPCSTR SessionName, 
	PEVENT_TRACE_PROPERTIES Properties, 
	ULONG control 
)
{
    DbgPrint("(%I64x, %s, %p, %d) stub\n", hSession, SessionName, Properties, control);
    return ERROR_SUCCESS;
}

/******************************************************************************
 * EtwEnableTrace [NTDLL.@]
 */
ULONG 
WINAPI 
EtwEnableTrace( 
	ULONG enable, 
	ULONG flag, 
	ULONG level, 
	LPCGUID guid, 
	TRACEHANDLE hSession
)
{
    DbgPrint("(%d, 0x%x, %d, %p, %I64x): stub\n", enable, flag, level,
            guid, hSession);

    return ERROR_SUCCESS;
}

/******************************************************************************
 * EtwQueryAllTracesW [NTDLL.@]
 *
 * Query information for started event trace sessions
 *
 */
ULONG 
WINAPI 
EtwQueryAllTracesW( 
	PEVENT_TRACE_PROPERTIES * parray, 
	ULONG arraycount, 
	PULONG psessioncount 
)
{
    DbgPrint("(%p, %d, %p) stub\n", parray, arraycount, psessioncount);

    if (psessioncount) *psessioncount = 0;
    return ERROR_SUCCESS;
}

/******************************************************************************
 * QueryAllTracesA [NTDLL.@]
 *
 * See EtwQueryAllTracesA.
 */
ULONG 
WINAPI 
EtwQueryAllTracesA( 
	PEVENT_TRACE_PROPERTIES * parray, 
	ULONG arraycount, 
	PULONG psessioncount 
)
{
    DbgPrint("(%p, %d, %p) stub\n", parray, arraycount, psessioncount);

    if (psessioncount) *psessioncount = 0;
    return ERROR_SUCCESS;
}

PVOID
WINAPI
EtwpGetTraceBuffer(
    IN PVOID Logger,
    IN PSYSTEM_THREAD_INFORMATION pThread,
    IN ULONG GroupType,
    IN ULONG RequiredSize
)
{
    return NULL;
}
 
void 
NTAPI
EtwpSetHWConfigFunction(
	PVOID DumpHardwareConfig, 
	ULONG Reason)
{
    if (Reason == DLL_PROCESS_ATTACH)       
    {
        //
        // On DLL Laod Get the pointer in Advapi32.dll
        //

        EtwpDumpHardwareConfig = DumpHardwareConfig;

    } else {

        //
        // On DLL unload point it back to Dummy function.
        //

        EtwpDumpHardwareConfig = EtwpDumpHWConfig;
    }
}
/* EOF */

ULONG 
WINAPI 
EtwUpdateTraceA(
  _In_    TRACEHANDLE             SessionHandle,
  _In_    LPCTSTR                 SessionName,
  _Inout_ PEVENT_TRACE_PROPERTIES Properties
)
{
    return ERROR_SUCCESS;	
}

ULONG 
WINAPI 
EtwUpdateTraceW(
  _In_    TRACEHANDLE             SessionHandle,
  _In_    LPCWSTR                 SessionName,
  _Inout_ PEVENT_TRACE_PROPERTIES Properties
)
{
    return ERROR_SUCCESS;	
}

ULONG WINAPI EtwStopTraceA(
  _In_  TRACEHANDLE             SessionHandle,
  _In_  LPCTSTR                 SessionName,
  _Out_ PEVENT_TRACE_PROPERTIES Properties
)
{
    return ERROR_SUCCESS;	
}

ULONG WINAPI EtwStopTraceW(
  _In_  TRACEHANDLE             SessionHandle,
  _In_  LPCWSTR                 SessionName,
  _Out_ PEVENT_TRACE_PROPERTIES Properties
)
{
    return ERROR_SUCCESS;	
}

ULONG 
WINAPI 
EtwQueryTraceA(
  _In_    TRACEHANDLE             SessionHandle,
  _In_    LPCTSTR                 SessionName,
  _Inout_ PEVENT_TRACE_PROPERTIES Properties
)
{
    return ERROR_SUCCESS;	
}

ULONG 
WINAPI 
EtwQueryTraceW(
  _In_    TRACEHANDLE             SessionHandle,
  _In_    LPCWSTR                 SessionName,
  _Inout_ PEVENT_TRACE_PROPERTIES Properties
)
{
    return ERROR_SUCCESS;	
}

ULONG 
WINAPI 
EtwFlushTraceA(
  _In_    TRACEHANDLE             SessionHandle,
  _In_    LPCTSTR                 SessionName,
  _Inout_ PEVENT_TRACE_PROPERTIES Properties
)
{
    return ERROR_SUCCESS;	
}

ULONG WINAPI EtwFlushTraceW(
  _In_    TRACEHANDLE             SessionHandle,
  _In_    LPCWSTR                 SessionName,
  _Inout_ PEVENT_TRACE_PROPERTIES Properties
)
{
    return ERROR_SUCCESS;	
}

ULONG 
WINAPI 
EtwCreateTraceInstanceId(
  _In_  HANDLE               RegHandle,
  _Out_ PEVENT_INSTANCE_INFO pInstInfo
)
{
    return ERROR_SUCCESS;	
}

ULONG WINAPI EtwEnumerateTraceGuids(
  _Inout_ PTRACE_GUID_PROPERTIES *GuidPropertiesArray,
  _In_    ULONG                  PropertyArrayCount,
  _Out_   PULONG                 GuidCount
)
{
    return ERROR_SUCCESS;	
}