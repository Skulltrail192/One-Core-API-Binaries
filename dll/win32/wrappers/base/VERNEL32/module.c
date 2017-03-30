/*++

Copyright (c) 1990  Microsoft Corporation

Module Name:

    module.c

Abstract:

    This module contains the Win32 Module Management APIs

Author:

    Steve Wood (stevewo) 24-Sep-1990

Revision History:

--*/

#include "main.h"

/***********************************************************************
 *           GetCurrentPackageId       (KERNEL32.@)
 */
LONG 
WINAPI 
GetCurrentPackageId(
	UINT32 *len, 
	BYTE *buffer
)
{
    DbgPrint("kernel32!GetCurrentPackageId is UNIMPLEMENTED\n");
    return APPMODEL_ERROR_NO_PACKAGE;
}
