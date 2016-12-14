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
#include <main.h>
#include <lpcfuncs.h>
#include <cmfuncs.h>

/*subimplemented*/
NTSTATUS 
NTAPI
NtAlpcAcceptConnectPort(
    __out HANDLE                         PortHandle,
    __in HANDLE                          ConnectionPortHandle,
    __in ULONG                           Flags,
    __in POBJECT_ATTRIBUTES              ObjectAttributes,
    __in PALPC_PORT_ATTRIBUTES           PortAttributes,
    __in_opt PVOID                       PortContext, // opaque value
    __in PPORT_MESSAGE                   ConnectionRequest,
    __inout_opt PALPC_MESSAGE_ATTRIBUTES MessageAttributes,
    __in BOOLEAN                         AcceptConnection
    )
{
	NTSTATUS status;						   
	DbgPrint("A syscall numero 296 NtAlpcAcceptConnectPort foi chamada\n");	
	status = NtAcceptConnectPort(PortHandle, 
							   PortContext, 
							   ConnectionRequest, 
							   AcceptConnection,
							   NULL,
							   NULL);
	DbgPrint("Status: %08x\n",status);
	return status;
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcCancelMessage(
		__in HANDLE  	PortHandle,
		__in ULONG  	Flags,
		__in PALPC_CONTEXT_ATTR  	MessageContext 
	) 	
{
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcConnectPort( 	
		__out PHANDLE  	PortHandle,
		__in PUNICODE_STRING  	PortName,
		__in POBJECT_ATTRIBUTES  	ObjectAttributes,
		__in_opt PALPC_PORT_ATTRIBUTES  	PortAttributes,
		__in ULONG  	Flags,
		__in_opt PSID  	RequiredServerSid,
		__inout PPORT_MESSAGE  	ConnectionMessage,
		__inout_opt PULONG  	BufferLength,
		__inout_opt PALPC_MESSAGE_ATTRIBUTES  	OutMessageAttributes,
		__inout_opt PALPC_MESSAGE_ATTRIBUTES  	InMessageAttributes,
		__in_opt PLARGE_INTEGER  	Timeout 
	) 	
{
	NTSTATUS status;
	DbgPrint("A syscall numero 298 NtAlpcConnectPort foi chamada\n");	
	status =  NtConnectPort(PortHandle, 
						 PortName, 
						 &PortAttributes->SecurityQos, 
						 NULL, 
						 NULL, 
						 &PortAttributes->MaxMessageLength,
						 NULL,
						 NULL);
	DbgPrint("Status: %08x\n",status);
	return status;
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcCreatePort( 	
		__out PHANDLE  	PortHandle,
		__in POBJECT_ATTRIBUTES  	ObjectAttributes,
		__in_opt PALPC_PORT_ATTRIBUTES  	PortAttributes 
	) 		
{
	NTSTATUS status;
	DbgPrint("A syscall numero 299 NtAlpcCreatePort foi chamada\n");	
	status = NtCreatePort(PortHandle, 
						  ObjectAttributes, 
						  sizeof(PortAttributes->MaxViewSize), 
						  sizeof(PortAttributes->MaxMessageLength), 
						  PortAttributes->MaxPoolUsage);	
	DbgPrint("Status: %08x\n",status);
	return status;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcCreatePortSection( 	
		__in HANDLE  	PortHandle,
		__in ULONG  	Flags,
		__in_opt HANDLE  	SectionHandle,
		__in SIZE_T  	SectionSize,
		__out PALPC_HANDLE  	AlpcSectionHandle,
		__out PSIZE_T  	ActualSectionSize 
	) 	
{
	NTSTATUS status;						   
	DbgPrint("A syscall numero 300 NtAlpcCreatePortSection foi chamada\n");
	status = NtCreateSection(&PortHandle, Flags, NULL, NULL, 1, 1, AlpcSectionHandle);
	DbgPrint("Status: %08x\n",status);	
	return status;
}

NTSTATUS NTAPI NtAlpcCreateResourceReserve(
		__in HANDLE PortHandle, 
		__reserved ULONG Flags, 
		__in SIZE_T MessageSize, 
		__out PALPC_HANDLE ResourceId)
{
	DbgPrint("A syscall numero 301 NtAlpcCreateResourceReserve foi chamada\n");	
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcCreateSectionView( 	
		__in HANDLE  	PortHandle,
		__reserved ULONG  	Flags,
		__inout PALPC_DATA_VIEW_ATTR  	ViewAttributes 
	) 	
{
	NTSTATUS status;						   
	DbgPrint("A syscall numero 302 NtAlpcCreateSectionView foi chamada\n");
	status = NtMapViewOfSection(ViewAttributes->SectionHandle, PortHandle, ViewAttributes->ViewBase, 0, 0, 0, (PULONG)ViewAttributes->ViewSize, 0, 0, 0);
	DbgPrint("Status: %08x\n",status);	
	return status;	
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcCreateSecurityContext( 	
		__in HANDLE  	PortHandle,
		__reserved ULONG  	Flags,
		__inout PALPC_SECURITY_ATTR  	SecurityAttribute 
	) 	
{
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcDeletePortSection( 	
		__in HANDLE  	PortHandle,
		__reserved ULONG  	Flags,
		__in ALPC_HANDLE  	SectionHandle 
	) 	
{
	return STATUS_SUCCESS;
}

NTSTATUS NTAPI NtAlpcDeleteResourceReserve(	
		__in HANDLE 	PortHandle,
		__reserved ULONG 	Flags,
		__in ALPC_HANDLE 	ResourceId 
)
{
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcDeleteSectionView( 	
		__in HANDLE  	PortHandle,
		__reserved ULONG  	Flags,
		__in PVOID  	ViewBase 
	) 	
{
	DbgPrint("A syscall numero 306 NtAlpcDeleteSectionView foi chamada\n");	
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcDeleteSecurityContext( 	
		__in HANDLE  	PortHandle,
		__reserved ULONG  	Flags,
		__in ALPC_HANDLE  	ContextHandle 
	) 	
{
	ContextHandle = NULL;
	DbgPrint("A syscall numero 307 NtAlpcDeleteSecurityContext foi chamada\n");	
	return STATUS_SUCCESS;
}

NTSTATUS NTAPI NtAlpcDisconnectPort(
		__in HANDLE PortHandle, 
		__in ULONG Flags)
{
	DbgPrint("A syscall numero 308 NtAlpcDisconnectPort foi chamada\n");	
	PortHandle = NULL;
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcImpersonateClientOfPort( 	
		__in HANDLE  	PortHandle,
		__in PPORT_MESSAGE  	PortMessage,
		__reserved PVOID  	Reserved 
	) 	
{
	NTSTATUS status;						   
	DbgPrint("A syscall numero 309 NtAlpcImpersonateClientOfPort foi chamada\n");
	status = NtImpersonateClientOfPort(PortHandle, PortMessage);
	DbgPrint("Status: %08x\n",status);	
	return status;	
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcQueryInformation( 	
		__in HANDLE  	PortHandle,
		__in ALPC_PORT_INFORMATION_CLASS  	PortInformationClass,
		__out_bcount(Length) PVOID  	PortInformation,
		__in ULONG  	Length,
		__out_opt PULONG  	ReturnLength 
	) 	
{
	NTSTATUS status;						   
	DbgPrint("A syscall numero 310 NtAlpcQueryInformation foi chamada\n");	
	status = NtQueryInformationPort(PortHandle, 
								  PortInformationClass, 
								  PortInformation, 
								  Length, 
								  ReturnLength);
	DbgPrint("Status: %08x\n",status);	
	return status;	
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcSendWaitReceivePort( 	
		__in HANDLE  	PortHandle,
		__in ULONG  	Flags,
		__in_opt PPORT_MESSAGE  	SendMessage,
		__in_opt PALPC_MESSAGE_ATTRIBUTES  	SendMessageAttributes,
		__inout_opt PPORT_MESSAGE  	ReceiveMessage,
		__inout_opt PULONG  	BufferLength,
		__inout_opt PALPC_MESSAGE_ATTRIBUTES  	ReceiveMessageAttributes,
		__in_opt PLARGE_INTEGER  	Timeout 
	) 	
{
	NTSTATUS status;
	DbgPrint("A syscall numero 311 NtAlpcSendWaitReceivePort foi chamada\n");	
	status = NtReplyWaitReceivePort(PortHandle, NULL, SendMessage, ReceiveMessage);//NtReplyWaitReceivePortEx(PortHandle, NULL, SendMessage, ReceiveMessage, Timeout);
	DbgPrint("Status: %08x\n", status);	
	return status;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcSetInformation( 	
		__in HANDLE  	PortHandle,
		__in ALPC_PORT_INFORMATION_CLASS  	PortInformationClass,
		__in_bcount(Length) PVOID  	PortInformation,
		__in ULONG  	Length 
	) 	
{
	//status = NtSetInformationObject(PortHandle, PortInformationClass, PortInformation, Length);
	//status = NtSetInformationThread(PortHandle, ThreadImpersonationToken, PortInformation, sizeof(HANDLE));
	//DbgPrint("Status: %08x\n", status);	
	NTSTATUS status;
	DbgPrint("A syscall numero 312 NtAlpcSetInformation foi chamada\n");	
	status = NtQueryInformationPort(PortHandle, PortInformationClass, PortInformation, Length, NULL);
	DbgPrint("Status: %08x\n", status);	
	return status;
}

NTSTATUS 
NTAPI
NtLoadKeyEx(
	IN POBJECT_ATTRIBUTES TargetKey,
	IN POBJECT_ATTRIBUTES SourceFile,
	IN ULONG Flags,
	IN HANDLE TrustClassKey
) 
{
	return NtLoadKey(TargetKey, SourceFile);
}

NTSTATUS 
NTAPI
NtQueryOpenSubKeysEx(
	IN POBJECT_ATTRIBUTES TargetKey,
	IN ULONG BufferLength,
	IN PVOID Buffer,
	IN PULONG RequiredSize
)
{
	return NtQueryOpenSubKeys(TargetKey, RequiredSize);
}

NTSTATUS 
WINAPI 
NtUnmapViewOfSectionEx(HANDLE handle, 
					   PVOID MemoryCache, 
					   ULONG number)
{
	return NtUnmapViewOfSection(handle, MemoryCache);	
}

NTSTATUS
NTAPI
NtCreateMutantEx(
  OUT PHANDLE             MutantHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
  IN BOOLEAN              InitialOwner, 
  IN ULONG 				  Flags
)
{
	return NtCreateMutant(MutantHandle, DesiredAccess, ObjectAttributes, InitialOwner);
}

NTSTATUS 
NTAPI 
NtCreateIoCompletionEx(
	__out PHANDLE IoCompletionHandle, 
	__in ACCESS_MASK DesiredAccess, 
	__in_opt POBJECT_ATTRIBUTES ObjectAttributes, 
	__in_opt ULONG Count
)
{
	return NtCreateIoCompletion(IoCompletionHandle, DesiredAccess, ObjectAttributes, Count);
} 

NTSTATUS 
NTAPI
NtRemoveIoCompletionEx(
    __in HANDLE IoCompletionHandle,
    __out FILE_IO_COMPLETION_INFORMATION IoCompletionInformation,
    __in ULONG Count,
    __out PVOID NumEntriesRemoved,
    __in_opt PLARGE_INTEGER Timeout,
    __in BOOLEAN Alertable
)
{
	return NtRemoveIoCompletion(IoCompletionHandle,
								NULL,
								NULL,
								NULL,
								Timeout);
}

NTSTATUS 
NTAPI 
NtCreateSemaphoreEx(
  IN OPTIONAL   HANDLE lpSemaphoreAttributes,
  IN       LONG                  lInitialCount,
  IN       LONG                  lMaximumCount,
  IN OPTIONAL  LPCTSTR               lpName,
  IN 	   DWORD                 dwFlags,
  IN       DWORD                 dwDesiredAccess
)
{
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
NtQuerySystemInformationEx(SYSTEM_INFORMATION_CLASS Information,
						   PVOID SystemInformation, 
						   ULONG QueryInformationLength, 
						   PVOID SystemInformatiom, 
						   ULONG BufferLength, 
						   PULONG ReturnLength)
{
	return NtQuerySystemInformation(Information,
									SystemInformation,
									BufferLength,
									ReturnLength);
}

NTSTATUS 
NTAPI 
NtCreateKeyTransacted(__out PHANDLE KeyHandle, 
					__in ACCESS_MASK DesiredAccess, 
					__in POBJECT_ATTRIBUTES ObjectAttributes, 
					__reserved ULONG TitleIndex, 
					__in_opt PUNICODE_STRING Class, 
					__in ULONG CreateOptions, 
					__in HANDLE TransactionHandle, 
					__out_opt PULONG Disposition)
{
	return NtCreateKey(KeyHandle, 
					   DesiredAccess, 
					   ObjectAttributes, 
					   TitleIndex, 
					   Class, 
					   CreateOptions, 
					   Disposition);
}

NTSTATUS 
NTAPI 
NtOpenKeyTransacted(
	__out PHANDLE KeyHandle, 
	__in ACCESS_MASK DesiredAccess, 
	__in POBJECT_ATTRIBUTES ObjectAttributes, 
	__in HANDLE TransactionHandle)
{
	return NtOpenKey(KeyHandle, 
					 DesiredAccess, 
					 ObjectAttributes);
}

/* Section Unimplemented*/
NTSTATUS
NTAPI
NtOpenPrivateNamespace(
  __out PHANDLE NamespaceHandle,
  __in ACCESS_MASK DesiredAccess,
  __in_opt POBJECT_ATTRIBUTES ObjectAttributes,
  __in PVOID BoundaryDescriptor
)
{
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
NtSetInformationVirtualMemory(HANDLE hProcess,
							  BOOL access,
							  ULONG_PTR NumberOfEntries, 
							  PWIN32_MEMORY_RANGE_ENTRY VirtualAddresses, 
							  PULONG Flags,
							  DWORD other
)
{
	return STATUS_SUCCESS;
}	

NTSTATUS 
NTAPI 	
NtReplacePartitionUnit(__in PUNICODE_STRING TargetInstancePath, 
					   __in PUNICODE_STRING SpareInstancePath,
					   __in ULONG Flags)
{
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
NtDeletePrivateNamespace(
  __in HANDLE NamespaceHandle
)
{
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
NtCreatePrivateNamespace(
  __out PHANDLE NamespaceHandle,
  __in ACCESS_MASK DesiredAccess,
  __in_opt POBJECT_ATTRIBUTES ObjectAttributes,
  __in PVOID BoundaryDescriptor
)
{
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI
NtTraceControl(
	ULONG FunctionCode, 
	PVOID InBuffer, 
	ULONG InBufferLen, 
	PVOID OutBuffer, 
	ULONG OutBufferLen, 
	PULONG ReturnLength
)
{
	return STATUS_SUCCESS;
}

DWORD WINAPI NtRollbackTransaction(int a, int b)
{
	return 0;
}

/*need implementation*/
NTSTATUS WINAPI NtCommitTransaction(HANDLE hTransaction, DWORD options)
{
	return 0;
}

DWORD WINAPI NtCreateTransaction(int a, int b)
{
	return 0;
}