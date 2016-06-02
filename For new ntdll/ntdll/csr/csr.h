/*++

Copyright (c) 2014  Microsoft Corporation

Module Name:

    csr.h

Abstract:

    Main include file for Client side of the Client Server Runtime (CSR)

Author:

    Samuel Marins 7-Dec-2014

Revision History:

--*/

//
// Include definitions common between the Client and Server portions.
//

#include <ntdll.h>

#include <ndk/lpcfuncs.h>
#include <csr/csrsrv.h>

#define NDEBUG
#include <debug.h>

ULONG CsrPortBaseTag;

//Definitions

#define 	RTL_HEAP_MAKE_TAG   HEAP_MAKE_TAG_FLAGS

#define MAKE_CSRPORT_TAG( t ) (RTL_HEAP_MAKE_TAG( CsrPortBaseTag, t ))

#define CAPTURE_TAG 0

//
// The CsrNtSysInfo global variable contains NT specific constants of
// interest, such as page size, allocation granularity, etc.  It is filled
// in once during process initialization.
//

SYSTEM_BASIC_INFORMATION CsrNtSysInfo;

//Functions Declarations


NTSTATUS 
NTAPI
CsrClientCallServer(IN OUT PCSR_API_MESSAGE ApiMessage,
                    IN OUT PCSR_CAPTURE_BUFFER CaptureBuffer OPTIONAL,
                    IN CSR_API_NUMBER ApiNumber,
                    IN ULONG DataLength);
					
PCSR_CAPTURE_HEADER
NTAPI
CsrAllocateCaptureBuffer(IN ULONG CountMessagePointers,
                         IN ULONG Size);
						 
VOID
NTAPI
CsrFreeCaptureBuffer(IN PCSR_CAPTURE_BUFFER CaptureBuffer);

/*
 * @implemented
 */
VOID
NTAPI
CsrCaptureMessageBuffer(IN OUT PCSR_CAPTURE_BUFFER CaptureBuffer,
                        IN PVOID MessageBuffer OPTIONAL,
                        IN ULONG MessageLength,
                        OUT PVOID* CapturedData);						 

					 