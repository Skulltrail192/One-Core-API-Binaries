/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    heap.c

Abstract:

    This module contains the Win32 Local Memory Management APIs

Author:

    Skulltrail 10-March-2018

Revision History:

--*/

#include "main.h"

BOOL WINAPI HeapSetInformationInternal( HANDLE heap, HEAP_INFORMATION_CLASS infoclass, PVOID info, SIZE_T size)
 {
    NTSTATUS Status = RtlSetHeapInformation( heap, infoclass, info, size );
	
	DbgPrint("HeapSetInformation :: Status: 0x%08x\n", Status);	
	
	if (NT_SUCCESS( Status )) {
		DbgPrint("HeapSetInformation :: Status success, return TRUE\n");			
        return TRUE;
    }else {
		DbgPrint("HeapSetInformation :: Status unsuccess, return FALSE\n");
        BaseSetLastNTError( Status );
    }
    return FALSE;
}