/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    alpc.c

Abstract:

    Implement functions to manage alpc ports communication

Author:

    Skulltrail 06-March-2018

Revision History:

--*/
 
#define NDEBUG

#include <main.h>

DWORD 
NTAPI
AlpcGetHeaderSize(
	DWORD Buffer
)
{
  DWORD result; // eax@1

  result = 8;
  if ( Buffer < 0 )
    result = 20;
  if ( Buffer & 0x40000000 )
    result += 16;
  if ( Buffer & 0x8000000 )
    result += 24;
  return result;
}

PVOID
NTAPI
AlpcGetMessageAttribute(
	PALPC_MESSAGE_ATTRIBUTES Buffer, 
	ULONG AttributeFlag
)
{
  PVOID result; // eax@3

  if ( !(AttributeFlag & Buffer->AllocatedAttributes) || (AttributeFlag - 1) & AttributeFlag )
    result = 0;
  else
    result = (char *)Buffer + AlpcGetHeaderSize(Buffer->AllocatedAttributes & ~(2 * AttributeFlag - 1));
  return result;
}

NTSTATUS 
NTAPI
AlpcInitializeMessageAttribute(
	ULONG AttributeFlags, 
	PALPC_MESSAGE_ATTRIBUTES Buffer, 
	DWORD BufferSize, 
	PULONG RequiredBufferSize
)
{
  DWORD HeaderSize; // eax@1
  NTSTATUS result; // eax@4

  HeaderSize = AlpcGetHeaderSize(AttributeFlags);
  *RequiredBufferSize = HeaderSize;
  if ( HeaderSize > BufferSize )
  {
    result = STATUS_BUFFER_TOO_SMALL;
  }
  else
  {
    if ( Buffer )
    {
      Buffer->ValidAttributes = 0;
      Buffer->AllocatedAttributes = AttributeFlags;
    }
    result = STATUS_SUCCESS;
  }
  return result;
}

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
	NTSTATUS Status;	
	
	DbgPrint("NtAlpcAcceptConnectPort called\n");	
	
	Status = NtAcceptConnectPort(PortHandle, 
							    PortContext, 
							    ConnectionRequest, 
							    AcceptConnection,
							    NULL,
							    NULL);
							   
	DbgPrint("Status: %08x\n",Status);
	
	return Status;
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
	DbgPrint("NtAlpcCancelMessage called\n");		
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
	NTSTATUS Status;
	
	DbgPrint("NtAlpcConnectPort called\n");	
	
	Status =  NtConnectPort(PortHandle, 
						 PortName, 
						 &PortAttributes->SecurityQos, 
						 NULL, 
						 NULL, 
						 (PULONG)&PortAttributes->MaxMessageLength,
						 NULL,
						 NULL);
						 
	DbgPrint("Status: %08x\n",Status);
	
	return Status;
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
	NTSTATUS Status;
	
	DbgPrint("NtAlpcCreatePort called\n");	
	
	Status = NtCreatePort(PortHandle, 
						  ObjectAttributes, 
						  sizeof(PortAttributes->MaxViewSize), 
						  sizeof(PortAttributes->MaxMessageLength), 
						  PortAttributes->MaxPoolUsage);	
						  
	DbgPrint("Status: %08x\n",Status);
	
	return Status;
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcCreatePortSection( 	
	__in HANDLE  	PortHandle,
	__in ULONG  	Flags,
	__in_opt HANDLE  	SectionHandle,
	__in SIZE_T  	SectionSize,
	__out PALPC_HANDLE  	AlpcSectionHandle,
	__out PSIZE_T  	ActualSectionSize 
) 	
{
	NTSTATUS Status;	
	
	DbgPrint("NtAlpcCreatePortSection called\n");
	
	Status = NtCreateSection(&PortHandle, Flags, NULL, NULL, 1, 1, AlpcSectionHandle);
	
	DbgPrint("Status: %08x\n",Status);	
	
	return Status;
}

NTSTATUS 
NTAPI 
NtAlpcCreateResourceReserve(
	__in HANDLE PortHandle, 
	ULONG Flags, 
	__in SIZE_T MessageSize, 
	__out PALPC_HANDLE ResourceId)
{
	DbgPrint("NtAlpcCreateResourceReserve called\n");	
	
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcCreateSectionView( 	
	__in HANDLE  	PortHandle,
	ULONG  	Flags,
	__inout PALPC_DATA_VIEW_ATTR  	ViewAttributes 
) 	
{
	NTSTATUS Status;	
	
	DbgPrint("NtAlpcCreateSectionView called\n");
	
	Status = NtMapViewOfSection(ViewAttributes->SectionHandle, PortHandle, ViewAttributes->ViewBase, 0, 0, 0, (PSIZE_T)ViewAttributes->ViewSize, 0, 0, 0);
	
	DbgPrint("Status: %08x\n",Status);	
	
	return Status;	
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcCreateSecurityContext( 	
	__in HANDLE  	PortHandle,
	ULONG  	Flags,
	__inout PALPC_SECURITY_ATTR  	SecurityAttribute 
) 	
{
	DbgPrint("NtAlpcCreateSecurityContext called\n");	
	
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcDeletePortSection( 	
	__in HANDLE  	PortHandle,
	ULONG  	Flags,
	__in ALPC_HANDLE  	SectionHandle 
) 	
{
	DbgPrint("NtAlpcDeletePortSection called\n");		
	
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI 
NtAlpcDeleteResourceReserve(	
		__in HANDLE 	PortHandle,
		ULONG 	Flags,
		__in ALPC_HANDLE 	ResourceId 
)
{
	DbgPrint("NtAlpcDeleteResourceReserve called\n");	
	
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcDeleteSectionView( 	
	__in HANDLE  	PortHandle,
	ULONG  	Flags,
	__in PVOID  	ViewBase 
) 	
{
	DbgPrint("NtAlpcDeleteSectionView called\n");	
	
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcDeleteSecurityContext( 	
	__in HANDLE  	PortHandle,
	ULONG  	Flags,
	__in ALPC_HANDLE  	ContextHandle 
) 	
{
	ContextHandle = NULL;
	
	DbgPrint("NtAlpcDeleteSecurityContext called\n");	
	
	return STATUS_SUCCESS;
}

NTSTATUS
NTAPI 
NtAlpcDisconnectPort(
		__in HANDLE PortHandle, 
		__in ULONG Flags)
{
	PortHandle = NULL;
	
	DbgPrint("NtAlpcDisconnectPort called\n");		
	
	return STATUS_SUCCESS;
}

/*subimplemented*/
NTSTATUS NTAPI NtAlpcImpersonateClientOfPort( 	
		__in HANDLE  	PortHandle,
		__in PPORT_MESSAGE  	PortMessage,
		PVOID  	Reserved 
	) 	
{
	NTSTATUS Status;			
	
	DbgPrint("NtAlpcImpersonateClientOfPort called\n");
	
	Status = NtImpersonateClientOfPort(PortHandle, PortMessage);
	
	DbgPrint("Status: %08x\n",Status);	
	
	return Status;	
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcQueryInformation( 	
	__in HANDLE  	PortHandle,
	__in ALPC_PORT_INFORMATION_CLASS  	PortInformationClass,
	__out_bcount(Length) PVOID  	PortInformation,
	__in ULONG  	Length,
	__out_opt PULONG  	ReturnLength 
) 	
{
	NTSTATUS Status;		
	
	DbgPrint("NtAlpcQueryInformation called\n");
	
	Status = NtQueryInformationPort(PortHandle, 
								  PortInformationClass, 
								  PortInformation, 
								  Length, 
								  ReturnLength);
								  
	DbgPrint("Status: %08x\n",Status);	
	
	return Status;	
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcSendWaitReceivePort( 	
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
	NTSTATUS Status;
	
	DbgPrint("NtAlpcSendWaitReceivePort called\n");	
	
	Status = NtReplyWaitReceivePort(PortHandle, NULL, SendMessage, ReceiveMessage);//NtReplyWaitReceivePortEx(PortHandle, NULL, SendMessage, ReceiveMessage, Timeout);
	
	DbgPrint("Status: %08x\n", Status);	
	
	return Status;
}

/*subimplemented*/
NTSTATUS 
NTAPI 
NtAlpcSetInformation( 	
	__in HANDLE  	PortHandle,
	__in ALPC_PORT_INFORMATION_CLASS  	PortInformationClass,
	__in_bcount(Length) PVOID  	PortInformation,
	__in ULONG  	Length 
) 	
{
	//Status = NtSetInformationObject(PortHandle, PortInformationClass, PortInformation, Length);
	//Status = NtSetInformationThread(PortHandle, ThreadImpersonationToken, PortInformation, sizeof(HANDLE));
	//DbgPrint("Status: %08x\n", Status);	
	NTSTATUS Status;
	
	DbgPrint("NtAlpcSetInformation called\n");	
	
	Status = NtQueryInformationPort(PortHandle, PortInformationClass, PortInformation, Length, NULL);
	
	DbgPrint("Status: %08x\n", Status);	
	
	return Status;
}
