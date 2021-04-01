/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    etw.c

Abstract:

    Private trace libraries and stubs that allows user-mode to reside in NTDLL.

Author:

    Skulltrail 21-November-2018

Revision History:

--*/

#include <rtl.h>
#include <evntprov.h>
#include <main.h>
#include <strsafe.h>

typedef HANDLE TRACEHANDLE;

typedef void (
WINAPI
*NOTIFICATIONCALLBACK)(
    PVOID Wnode,
    UINT_PTR NotificationContext
    );
	
//
// Instance Information to track parent child relationship of Instances.
//
typedef struct EVENT_INSTANCE_INFO {
    HANDLE      RegHandle;
    ULONG       InstanceId;
} EVENT_INSTANCE_INFO, *PEVENT_INSTANCE_INFO;	

typedef struct _EVENT_INSTANCE_HEADER {
    USHORT          Size;
    union {
        USHORT      FieldTypeFlags;     // Indicates valid fields
        struct {
            UCHAR   HeaderType;         // Header type - internal use only
            UCHAR   MarkerFlags;        // Marker - internal use only
        };
    };
    union {
        ULONG       Version;
        struct {
            UCHAR   Type;
            UCHAR   Level;
            USHORT  Version;
        } Class;
    };
    ULONG           ThreadId;
    ULONG           ProcessId;
    LARGE_INTEGER   TimeStamp;
    ULONGLONG       RegHandle;
    ULONG           InstanceId;
    ULONG           ParentInstanceId;
    union {
        struct {
            ULONG   ClientContext;          // Reserved
            ULONG   Flags;                  // Flags for header
        };
        struct {
            ULONG   KernelTime;             // Kernel Mode CPU ticks
            ULONG   UserTime;               // User mode CPU ticks
        };
        ULONG64     ProcessorTime;          // Processor Clock
    };
    ULONGLONG       ParentRegHandle;
} EVENT_INSTANCE_HEADER, *PEVENT_INSTANCE_HEADER;	
	
#define CPU_ROOT \
    L"\\Registry\\Machine\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor"

#define MHZ_VALUE_NAME \
    L"~MHz"

/*unimplemented */
void 
NTAPI 
WinSqmEventWrite(
	int a1, 
	int a2, 
	int a3
)
{
  ;
}

ULONG 
NTAPI
EtwEventUnregister(
  _In_  REGHANDLE RegHandle
)
{
	return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwEventRegister(
  IN LPCGUID ProviderId,
  IN PENABLECALLBACK EnableCallback OPTIONAL,
  IN PVOID CallbackContext OPTIONAL,
  OUT PREGHANDLE RegHandle)
{
	*RegHandle = 0xdeadbeef;
	return ERROR_SUCCESS;
}

BOOLEAN 
NTAPI
EtwEventEnabled(
  _In_  REGHANDLE RegHandle,
  _In_  PCEVENT_DESCRIPTOR EventDescriptor
)
{
	return FALSE;
}

ULONG
NTAPI
EtwEventWrite(
  IN REGHANDLE RegHandle,
  IN PCEVENT_DESCRIPTOR EventDescriptor,
  IN ULONG UserDataCount,
  IN PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}

ULONG
NTAPI
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
NTAPI
EtwWriteUMSecurityEvent (
    PCEVENT_DESCRIPTOR EventDescriptor,
    USHORT EventProperty,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwEventWriteEndScenario (
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwEventWriteStartScenario (
    REGHANDLE RegHandle,
    PCEVENT_DESCRIPTOR EventDescriptor,
    ULONG UserDataCount,
    PEVENT_DATA_DESCRIPTOR UserData)
{
	return ERROR_SUCCESS;
}
	
NTSTATUS
NTAPI
EtwReplyNotification(
	ULONG parameter
)
{
	return STATUS_SUCCESS;
}

ULONG 
NTAPI 
EtwTraceDiagnosticEvent(BOOLEAN one, int two, DWORD three, BOOL four, BOOL five)
{
	return ERROR_SUCCESS;
}

BOOL 
NTAPI 
EtwpIsHeapRangeLoggingEnabled()
{
	return TRUE;
}

BOOL 
NTAPI 
EtwpIsPrivateLoggerOn()
{
	return TRUE;
}

NTSTATUS 
NTAPI 
EtwpGetCpuSpeed(
    IN DWORD* CpuNum,
    OUT DWORD* CpuSpeed
)
{
  LPWSTR pszDestTemp; // edi@1
  PVOID KeyValueInformation; // edi@4
  OBJECT_ATTRIBUTES ObjectAttributes; // [sp+Ch] [bp-34h]@3
  UNICODE_STRING ValueName; // [sp+24h] [bp-1Ch]@5
  UNICODE_STRING ObjectName; // [sp+2Ch] [bp-14h]@3
  ULONG ResultLength; // [sp+34h] [bp-Ch]@5
  HANDLE key; // [sp+38h] [bp-8h]@1
  LPWSTR pszDest; // [sp+3Ch] [bp-4h]@1
  NTSTATUS Status; // [sp+48h] [bp+8h]@3

  key = (HANDLE)-1;
  CpuSpeed = 0;
  pszDestTemp = (LPWSTR)RtlAllocateHeap((PVOID)NtCurrentTeb()->ProcessEnvironmentBlock->SubSystemData, 0, 0x1000u);
  pszDest = pszDestTemp;
  if ( pszDestTemp )
  {
    if ( StringCbPrintfW(
           pszDestTemp,
           0x1000u,
           L"%ws\\%u",
           CPU_ROOT,
           CpuNum) < 0 )
    {
      Status = STATUS_NO_MEMORY;
    }
    else
    {
      RtlInitUnicodeString(&ObjectName, pszDestTemp);
      ObjectAttributes.ObjectName = &ObjectName;
      ObjectAttributes.Length = 24;
      ObjectAttributes.RootDirectory = 0;
      ObjectAttributes.Attributes = 64;
      ObjectAttributes.SecurityDescriptor = 0;
      ObjectAttributes.SecurityQualityOfService = 0;
      Status = ZwOpenKey(&key, 131097, &ObjectAttributes);
      if ( Status >= 0 )
      {
        KeyValueInformation = RtlAllocateHeap((PVOID)NtCurrentTeb()->ProcessEnvironmentBlock->SubSystemData, 0, 0x10u);
        if ((ULONG) KeyValueInformation )
        {
          StringCbCopyW(pszDest, 0x1000u, MHZ_VALUE_NAME);
          RtlInitUnicodeString(&ValueName, pszDest);
          Status = ZwQueryValueKey(key, &ValueName, 2, KeyValueInformation, 16, &ResultLength);
          if ( Status >= 0 )
            *CpuSpeed = ((ULONG)KeyValueInformation);
          RtlFreeHeap(NtCurrentTeb()->ProcessEnvironmentBlock->SubSystemData, 0, KeyValueInformation);
        }
        else
        {
          Status = STATUS_NO_MEMORY;
        }
        NtClose(key);
      }
    }
    RtlFreeHeap(NtCurrentTeb()->ProcessEnvironmentBlock->SubSystemData, 0, pszDest);
  }
  else
  {
    Status = STATUS_NO_MEMORY;
  }
  return Status;
}

ULONG 
NTAPI 
EtwDeliverDataBlock(int a1)
{
	return 0;
}

DWORD 
NTAPI 
EtwEnumerateProcessRegGuids(
	PVOID a1, 
	ULONG a2, 
	DWORD a3)
{
	return 0;
}

PVOID 
NTAPI
EtwpCreateEtwThread(
	PSTARTUP_ROUTINE StartupRoutine, 
	PVOID threadHandle
)
{
  NTSTATUS status; // eax@2

  if ( RtlCreateUserThread((HANDLE)0xFFFFFFFF, 0, TRUE, 0, 0, 0, (PTHREAD_START_ROUTINE)StartupRoutine, threadHandle, &threadHandle, 0) < 0 )
    goto Default_return;
  status = ZwResumeThread(threadHandle, 0);
  if ( status < 0 )
  {
    ZwTerminateThread(threadHandle, status);
    NtClose(threadHandle);
Default_return:
    threadHandle = NULL;
  }
  return threadHandle;
}

//unknown prototype
NTSTATUS 
NTAPI 
EtwNotificationRegister(
	DWORD a1, 
	BOOLEAN a2, 
	DWORD a3, 
	DWORD a4, 
	DWORD a5)
{
	DbgPrint("UNIMPLEMENTED: EtwNotificationRegister");
	return STATUS_SUCCESS;
}	

//unknown prototype
NTSTATUS 
NTAPI 
EtwNotificationUnregister(
	DWORD a1, 
	ULONG a2, 
	DWORD a3)
{
	DbgPrint("UNIMPLEMENTED: EtwNotificationUnregister");	
	return STATUS_SUCCESS;
}	

//unknown prototype
NTSTATUS 
NTAPI 
EtwSendNotification(
	DWORD a1, 
	DWORD a2, 
	DWORD a3, 
	DWORD a4, 
	DWORD a5)
{
	DbgPrint("UNIMPLEMENTED: EtwSendNotification");		
	return STATUS_SUCCESS;
}	

DWORD 
NTAPI 
EtwProcessPrivateLoggerRequest(
	PVOID number
)
{
	DbgPrint("UNIMPLEMENTED: EtwProcessPrivateLoggerRequest");		
	return ERROR_SUCCESS;
}

//unknown prototype
ULONG 
NTAPI
EtwLogTraceEvent(
    IN TRACEHANDLE LoggerHandle,
    IN PEVENT_TRACE_HEADER EventTrace
)
{
	DbgPrint("UNIMPLEMENTED: EtwLogTraceEvent");		
	return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwNotificationRegistrationA(
    IN LPGUID Guid,
    IN BOOLEAN Enable,
    IN PVOID DeliveryInfo,
    IN ULONG_PTR DeliveryContext,
    IN ULONG Flags
    )
{
	return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwNotificationRegistrationW(
    IN LPGUID Guid,
    IN BOOLEAN Enable,
    IN PVOID DeliveryInfo,
    IN ULONG_PTR DeliveryContext,
    IN ULONG Flags
)
{
	return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwReceiveNotificationsW(
    IN ULONG HandleCount,
    IN HANDLE *HandleList,
    IN NOTIFICATIONCALLBACK Callback,
    IN ULONG_PTR DeliveryContext
)
{
    return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwReceiveNotificationsA(
    IN ULONG HandleCount,
    IN HANDLE *HandleList,
    IN NOTIFICATIONCALLBACK Callback,
    IN ULONG_PTR DeliveryContext
)
{
    return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwTraceEventInstance(
    IN TRACEHANDLE  LoggerHandle,
    IN PEVENT_INSTANCE_HEADER EventTrace,
    IN PEVENT_INSTANCE_INFO pInstInfo,
    IN PEVENT_INSTANCE_INFO pParentInstInfo
)
{
    return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwTraceMessageVa(
    IN TRACEHANDLE LoggerHandle,
    IN ULONG       MessageFlags,
    IN LPGUID      MessageGuid,
    IN USHORT      MessageNumber,
    IN va_list     MessageArgList
)
{
    return ERROR_SUCCESS;
}
//Add daqui pra lá
ULONG
NTAPI
EtwpAddLogHeaderToLogFile(
    IN OUT PWMI_LOGGER_INFORMATION LoggerInfo,
    IN PVOID              RefClock,
    IN     ULONG                   Update
)
{
    return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwpFinalizeLogFileHeader(
    IN PWMI_LOGGER_INFORMATION LoggerInfo
)
{
    return ERROR_SUCCESS;
}

ULONG 
NTAPI
EtwpFlushUmLogger(
    IN ULONG WnodeSize,
    IN OUT ULONG *SizeUsed,
    OUT ULONG *SizeNeeded,
    IN OUT PWMI_LOGGER_INFORMATION LoggerInfo
)
{
	*SizeNeeded = 10000;
    return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwpQueryUmLogger(
    IN ULONG WnodeSize,
    IN OUT ULONG *SizeUsed,
    OUT ULONG *SizeNeeded,
    IN OUT PWMI_LOGGER_INFORMATION LoggerInfo
)
{
	*SizeNeeded = 10000;
    return ERROR_SUCCESS;
}

ULONG 
NTAPI
EtwpSendWmiKMRequest(
    HANDLE DeviceHandle,
    ULONG Ioctl,
    PVOID InBuffer,
    ULONG InBufferSize,
    PVOID OutBuffer,
    ULONG MaxBufferSize,
    ULONG *ReturnSize,
    LPOVERLAPPED Overlapped
)
{
	return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwpStartUmLogger(
    IN ULONG WnodeSize,
    IN OUT ULONG *SizeUsed,
    OUT ULONG *SizeNeeded,
    IN OUT PWMI_LOGGER_INFORMATION LoggerInfo
)
{
	*SizeNeeded = 10000;
	return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwpStopUmLogger(
        IN ULONG WnodeSize,
        IN OUT ULONG *SizeUsed,
        OUT ULONG *SizeNeeded,
        IN OUT PWMI_LOGGER_INFORMATION LoggerInfo
)
{
	*SizeNeeded = 10000;
	return ERROR_SUCCESS;
}

ULONG
NTAPI
EtwpUpdateUmLogger(
    IN ULONG WnodeSize,
    IN OUT ULONG *SizeUsed,
    OUT ULONG *SizeNeeded,
    IN OUT PWMI_LOGGER_INFORMATION LoggerInfo
)
{
	*SizeNeeded = 10000;
	return ERROR_SUCCESS;
}

BOOLEAN 
NTAPI
EtwpIsProcessExiting()
{
	return FALSE;
}

/******************************************************************************
 *                  EtwEventActivityIdControl (NTDLL.@)
 */
ULONG WINAPI EtwEventActivityIdControl(ULONG code, GUID *guid)
{
    static int once;

    if (!once++) DbgPrint("0x%x, %p: stub\n", code, guid);
    return ERROR_SUCCESS;
}


/******************************************************************************
 *                  EtwEventProviderEnabled (NTDLL.@)
 */
BOOLEAN WINAPI EtwEventProviderEnabled( REGHANDLE handle, UCHAR level, ULONGLONG keyword )
{
    DbgPrint("%s, %u, %s: stub\n", handle, level, keyword);
    return FALSE;
}

/*********************************************************************
 *                  EtwEventSetInformation   (NTDLL.@)
 */
ULONG WINAPI EtwEventSetInformation( REGHANDLE handle, EVENT_INFO_CLASS class, void *info,
                                     ULONG length )
{
    DbgPrint("(%s, %u, %p, %u) stub\n", handle, class, info, length);
    return ERROR_SUCCESS;
}

/******************************************************************************
 *                  EtwEventWriteString   (NTDLL.@)
 */
ULONG WINAPI EtwEventWriteString( REGHANDLE handle, UCHAR level, ULONGLONG keyword, PCWSTR string )
{
    DbgPrint("%s, %u, %s, %s: stub\n", handle, level,
          keyword, string);
    return ERROR_SUCCESS;
}

/******************************************************************************
 *                  EtwEventWriteTransfer   (NTDLL.@)
 */
ULONG WINAPI EtwEventWriteTransfer( REGHANDLE handle, PCEVENT_DESCRIPTOR descriptor, LPCGUID activity,
                                    LPCGUID related, ULONG count, PEVENT_DATA_DESCRIPTOR data )
{
    DbgPrint("%s, %p, %s, %s, %u, %p: stub\n", handle, descriptor,
          activity, related, count, data);
    return ERROR_SUCCESS;
}