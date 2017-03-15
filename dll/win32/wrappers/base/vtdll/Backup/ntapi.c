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
NTSTATUS NTAPI
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
	/*return NtAcceptConnectPort(PortHandle, 
							   PortContext, 
							   ConnectionRequest, 
							   AcceptConnection,
							   NULL,
							   NULL);*/
	/*char* localPortHandle = (char*)&PortHandle;
	char* localConnectionPortHandle = (char*)&ConnectionPortHandle;
	char* localFlags = (char*)&Flags;
	char* localObjectAttributes = (char*)&ObjectAttributes;
	char* localPortAttributes = (char*)&PortAttributes;
	char* localPortContext = (char*)&PortContext;
	char* localConnectionRequest = (char*)&ConnectionRequest;
	char* localMessageAttributes = (char*)&MessageAttributes;
	char* localAcceptConnection = (char*)&AcceptConnection;	
	__asm{
		mov eax, 128H;
		mov edx, localPortHandle;
		mov edx, localConnectionPortHandle;
		mov edx, localFlags;
		mov edx, localObjectAttributes;
		mov edx, localPortAttributes;
		mov edx, localPortContext;
		mov edx, localConnectionRequest;
		mov edx, localMessageAttributes;
		mov edx, localAcceptConnection;	
		int 2Eh;
	}*/
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
NTSTATUS NTAPI NtAlpcCancelMessage(
		__in HANDLE  	PortHandle,
		__in ULONG  	Flags,
		__in PALPC_CONTEXT_ATTR  	MessageContext 
	) 	
{
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcConnectPort( 	
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
	/*char* localPortHandle = (char*)&PortHandle;
	char* localPortName = (char*)&PortName;
	char* localObjectAttributes = (char*)&ObjectAttributes;
	char* localPortAttributes = (char*)&PortAttributes;
	char* localFlags = (char*)&Flags;
	char* localRequiredServerSid = (char*)&RequiredServerSid;
	char* localConnectionMessage = (char*)&ConnectionMessage;
	char* localBufferLength = (char*)&BufferLength;
	char* localOutMessageAttributes = (char*)&OutMessageAttributes;	
	char* localInMessageAttributes = (char*)&InMessageAttributes;
	char* localTimeout = (char*)&Timeout;	
	__asm{
		mov eax, 12AH;
		mov edx, localPortHandle;
		mov edx, localPortName;
		mov edx, localObjectAttributes;
		mov edx, localPortAttributes;
		mov edx, localFlags;
		mov edx, localRequiredServerSid;
		mov edx, localConnectionMessage;
		mov edx, localBufferLength;
		mov edx, localOutMessageAttributes;	
		mov edx, localInMessageAttributes;
		mov edx, localTimeout;	
		int 2Eh;
	}	*/			
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcCreatePort( 	
		__out PHANDLE  	PortHandle,
		__in POBJECT_ATTRIBUTES  	ObjectAttributes,
		__in_opt PALPC_PORT_ATTRIBUTES  	PortAttributes 
	) 		
{
	/*return NtCreatePort(PortHandle, 
						ObjectAttributes, 
						PortAttributes->MaxViewSize, 
						PortAttributes->MaxMessageLength, 
						PortAttributes->MaxPoolUsage);*/
	/*char* localPortHandle = (char*)&PortHandle;
	char* localObjectAttributes = (char*)&ObjectAttributes;
	char* localPortAttributes = (char*)&PortAttributes;
	__asm{
		mov eax, 12BH;
		mov edx, localPortHandle;
		mov edx, localObjectAttributes;
		mov edx, localPortAttributes;
		int 2Eh;
	}	*/
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
	//return NtCreateSection(&PortHandle, Flags, NULL, NULL, 1, 1, AlpcSectionHandle);
	/*char* localPortHandle = (char*)&PortHandle;
	char* localFlags = (char*)&Flags;
	char* localSectionHandle = (char*)&SectionHandle;
	char* localSectionSize = (char*)&SectionSize;
	char* localAlpcSectionHandle = (char*)&AlpcSectionHandle;
	char* localActualSectionSize = (char*)&ActualSectionSize;
	__asm{
		mov eax, 12CH;
		mov edx, localPortHandle;
		mov edx, localFlags;		
		mov edx, localSectionHandle;
		mov edx, localSectionSize;
		mov edx, localAlpcSectionHandle;
		mov edx, localActualSectionSize;
		int 2Eh;
	}	*/
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
	/*//return STATUS_NOT_IMPLEMENTED;
	char* localPortHandle = (char*)&PortHandle;
	char* localFlags = (char*)&Flags;
	char* localMessageSize = (char*)&MessageSize;
	char* localResourceId = (char*)&ResourceId;
	__asm{
		mov eax, 12DH;
		mov edx, localPortHandle;
		mov edx, localFlags;
		mov edx, localMessageSize;
		mov edx, localResourceId;
		int 2Eh;
	}	*/
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
	//return STATUS_SUCCESS;
	/*char* localPortHandle = (char*)&PortHandle;
	char* localFlags = (char*)&Flags;
	char* localViewAttributes = (char*)&ViewAttributes;
	__asm{
		mov eax, 12EH;
		mov edx, localPortHandle;
		mov edx, localFlags;
		mov edx, localViewAttributes;
		int 2Eh;
	}	*/
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
	//return STATUS_SUCCESS;
	/*
	char* localPortHandle = (char*)&PortHandle;
	char* localFlags = (char*)&Flags;
	char* localSecurityAttribute = (char*)&SecurityAttribute;
	__asm{
		mov eax, 12FH;
		mov edx, localPortHandle;
		mov edx, localFlags;
		mov edx, localSecurityAttribute;
		int 2Eh;
	}	*/
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
	/*char* localPortHandle = (char*)&PortHandle;
	char* localFlags = (char*)&Flags;
	char* localSectionHandle = (char*)&SectionHandle;
	__asm{
		mov eax, 130H;
		mov edx, localPortHandle;
		mov edx, localFlags;
		mov edx, localSectionHandle;
		int 2Eh;
	}*/
}

NTSTATUS NTAPI NtAlpcDeleteResourceReserve(	
		__in HANDLE 	PortHandle,
		__reserved ULONG 	Flags,
		__in ALPC_HANDLE 	ResourceId 
)
{
	return STATUS_SUCCESS;
	/*char* localPortHandle = (char*)&PortHandle;
	char* localFlags = (char*)&Flags;
	char* localResourceId = (char*)&ResourceId;
	__asm{
		mov eax, 131H;
		mov edx, localPortHandle;
		mov edx, localFlags;
		mov edx, localResourceId;
		int 2Eh;
	}*/
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
	/*char* localPortHandle = (char*)&PortHandle;
	char* localFlags = (char*)&Flags;
	char* localViewBase = (char*)&ViewBase;
	__asm{
		mov eax, 132H;
		mov edx, localPortHandle;
		mov edx, localFlags;
		mov edx, localViewBase;
		int 2Eh;
	}*/
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
	/*
	char* localPortHandle = (char*)&PortHandle;
	char* localFlags = (char*)&Flags;
	char* localContextHandle = (char*)&ContextHandle;
	__asm{
		mov eax, 133H;
		mov edx, localPortHandle;
		mov edx, localFlags;
		mov edx, localContextHandle;
		int 2Eh;
	}*/
}

NTSTATUS NTAPI NtAlpcDisconnectPort(
		__in HANDLE PortHandle, 
		__in ULONG Flags)
{
	/*//return STATUS_SUCCESS;
	char* localPortHandle = (char*)&PortHandle;
	char* localFlags = (char*)&Flags;
	__asm{
		mov eax, 134H;
		mov edx, localPortHandle;
		mov edx, localFlags;
		int 2Eh;
	}*/
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
	//return NtImpersonateClientOfPort(PortHandle, PortMessage);
	/*char* localPortHandle = (char*)&PortHandle;
	char* localPortMessage = (char*)&PortMessage;
	char* localReserved = (char*)&Reserved;
	__asm{
		mov eax, 135H;
		mov edx, localPortHandle;
		mov edx, localPortMessage;
		mov edx, localReserved;
		int 2Eh;
	}*/
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
    //PortInformation = NULL;
	//return STATUS_SUCCESS;
	/*char* localPortHandle = (char*)&PortHandle;
	char* localPortInformationClass = (char*)&PortInformationClass;
	char* localPortInformation = (char*)&PortInformation;
	char* localLength = (char*)&Length;
	char* localReturnLength = (char*)&ReturnLength;
	__asm{
		mov eax, 136H;
		mov edx, localPortHandle;
		mov edx, localPortInformationClass;
		mov edx, localPortInformation;
		mov edx, localLength;
		mov edx, localReturnLength;
		int 2Eh;
	}*/
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
	//return NtReplyWaitReceivePortEx(PortHandle, NULL, SendMessage, ReceiveMessage, Timeout);
	//return 2;
	/*char* localPortHandle = (char*)&PortHandle;
	char* localFlags = (char*)&Flags;	
	char* localSendMessage = (char*)&SendMessage;
	char* localSendMessageAttributes = (char*)&SendMessageAttributes;
	char* localReceiveMessage = (char*)&ReceiveMessage;
	char* localBufferLength = (char*)&BufferLength;
	char* localReceiveMessageAttributes = (char*)&ReceiveMessageAttributes;	
	char* localTimeout = (char*)&Timeout;	
	__asm{
		mov eax, 137H;
		mov edx, localPortHandle;
		mov edx, localFlags;		
		mov edx, localSendMessage;
		mov edx, localSendMessageAttributes;
		mov edx, localReceiveMessage;
		mov edx, localBufferLength;
		mov edx, localReceiveMessageAttributes;	
		mov edx, localTimeout;	
		int 2Eh;
	}	*/
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
	DbgPrint("A syscall numero 312 NtAlpcSetInformation foi chamada\n");	
	//status = NtSetInformationObject(PortHandle, PortInformationClass, PortInformation, Length);
	//status = NtSetInformationThread(PortHandle, ThreadImpersonationToken, PortInformation, sizeof(HANDLE));
	//DbgPrint("Status: %08x\n", status);	
	return STATUS_SUCCESS;
	//return STATUS_SUCCESS;
	/*char* localPortHandle = (char*)&PortHandle;
	char* localPortInformationClass = (char*)&PortInformationClass;
	char* localPortInformation = (char*)&PortInformation;
	char* localLength = (char*)&Length;
	__asm{
		mov eax, 138H;
		mov edx, localPortHandle;
		mov edx, localPortInformationClass;
		mov edx, localPortInformation;
		mov edx, localLength;
		int 2Eh;
	}*/
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