#include <rtl.h>
#include <evntprov.h>
#include <strsafe.h>

ULONG 
WINAPI
EtwEventUnregister(
  _In_  REGHANDLE RegHandle
)
{
	return ERROR_SUCCESS;
}

ULONG
WINAPI
EtwEventRegister(
  IN LPCGUID ProviderId,
  IN PENABLECALLBACK EnableCallback OPTIONAL,
  IN PVOID CallbackContext OPTIONAL,
  OUT PREGHANDLE RegHandle)
{
	return ERROR_SUCCESS;
}

BOOLEAN WINAPI EtwEventEnabled(
  _In_  REGHANDLE RegHandle,
  _In_  PCEVENT_DESCRIPTOR EventDescriptor
)
{
	return FALSE;
}

ULONG
WINAPI
EtwEventWrite(
  IN REGHANDLE RegHandle,
  IN PCEVENT_DESCRIPTOR EventDescriptor,
  IN ULONG UserDataCount,
  IN PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}

ULONG 
WINAPI
EtwEventActivityIdControl(
  _In_     ULONG ControlCode,
  _Inout_  LPGUID ActivityId
)
{
	return ERROR_SUCCESS;
}

ULONG
WINAPI
EtwEventWriteFull(
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    USHORT EventProperty,
    LPCGUID ActivityId,
    LPCGUID RelatedActivityId,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}

ULONG
WINAPI
EtwWriteUMSecurityEvent (
    PCEVENT_DESCRIPTOR EventDescriptor,
    USHORT EventProperty,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}

ULONG
WINAPI
EtwEventWriteEndScenario (
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}

ULONG
WINAPI
EtwEventWriteStartScenario (
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}
	
NTSTATUS
WINAPI
EtwReplyNotification(
	ULONG parameter
)
{
	return STATUS_SUCCESS;
}