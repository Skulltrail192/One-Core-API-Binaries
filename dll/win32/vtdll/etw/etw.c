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

#include <rtl.h>
#include <evntprov.h>
#include <main.h>
#include <strsafe.h>

/*unimplemented */
void 
WINAPI 
WinSqmEventWrite(int a1, int a2, int a3)
{
  ;
}

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
EtwReplyNotification(ULONG parameter)
{
	return 0xC000000Du;
}

ULONG 
WINAPI 
EtwTraceDiagnosticEvent(BOOLEAN one, int two, DWORD three, BOOL four, BOOL five)
{
	return ERROR_SUCCESS;
}

BOOL 
WINAPI 
EtwpIsHeapRangeLoggingEnabled()
{
	return TRUE;
}

BOOL WINAPI EtwpIsPrivateLoggerOn()
{
	return TRUE;
}

NTSTATUS 
NTAPI EtwpGetCpuSpeed(
	int arg, 
	ULONG Speed
)
{
  LPWSTR pszDestTemp; // edi@1
  PVOID KeyValueInformation; // edi@4
  NTSTATUS result; // eax@10
  OBJECT_ATTRIBUTES ObjectAttributes; // [sp+Ch] [bp-34h]@3
  UNICODE_STRING ValueName; // [sp+24h] [bp-1Ch]@5
  UNICODE_STRING ObjectName; // [sp+2Ch] [bp-14h]@3
  ULONG ResultLength; // [sp+34h] [bp-Ch]@5
  HANDLE key; // [sp+38h] [bp-8h]@1
  LPWSTR pszDest; // [sp+3Ch] [bp-4h]@1
  NTSTATUS Status; // [sp+48h] [bp+8h]@3

  key = (HANDLE)-1;
  Speed = 0;
  pszDestTemp = (LPWSTR)RtlAllocateHeap((PVOID)NtCurrentTeb()->ProcessEnvironmentBlock->SubSystemData, 0, 0x1000u);
  pszDest = pszDestTemp;
  if ( pszDestTemp )
  {
    if ( StringCbPrintfW(
           pszDestTemp,
           0x1000u,
           L"%ws\\%u",
           L"\\Registry\\Machine\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor",
           arg) < 0 )
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
        if ( KeyValueInformation )
        {
          StringCbCopyW(pszDest, 0x1000u, L"~MHz");
          RtlInitUnicodeString(&ValueName, pszDest);
          Status = ZwQueryValueKey(key, &ValueName, 2, KeyValueInformation, 16, &ResultLength);
          if ( Status >= 0 )
            Speed = ((ULONG)KeyValueInformation + 3);
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
WINAPI 
EtwDeliverDataBlock(int a1)
{
	return 0;
}

DWORD 
WINAPI 
EtwEnumerateProcessRegGuids(
	PVOID a1, 
	ULONG a2, 
	DWORD a3)
{
	return 0;
}

PVOID 
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
WINAPI 
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
WINAPI 
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
WINAPI 
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
WINAPI 
EtwProcessPrivateLoggerRequest(
	PVOID number
)
{
	DbgPrint("UNIMPLEMENTED: EtwSendNotification");		
	return ERROR_SUCCESS;
}

//unknown prototype
DWORD 
WINAPI 
EtwLogTraceEvent(
	DWORD a1, 
	DWORD a2, 
	DWORD a3
)
{
	DbgPrint("UNIMPLEMENTED: EtwSendNotification");		
	return ERROR_SUCCESS;
}