/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    syblink.c

Abstract:

    This module implements Win32 File Information for others functions APIs

Author:

    Skulltrail 25-March-2017

Revision History:

--*/

#include "main.h"

BOOL bIsFileApiAnsi = TRUE; // set the file api to ansi or oem

PWCHAR
FilenameA2W(
	LPCSTR NameA, 
	BOOL alloc
)
{
   ANSI_STRING str;
   UNICODE_STRING strW;
   PUNICODE_STRING pstrW;
   NTSTATUS Status;

   //ASSERT(NtCurrentTeb()->StaticUnicodeString.Buffer == NtCurrentTeb()->StaticUnicodeBuffer);
   ASSERT(NtCurrentTeb()->StaticUnicodeString.MaximumLength == sizeof(NtCurrentTeb()->StaticUnicodeBuffer));

   RtlInitAnsiString(&str, NameA);
   pstrW = alloc ? &strW : &NtCurrentTeb()->StaticUnicodeString;

   if (bIsFileApiAnsi)
        Status= RtlAnsiStringToUnicodeString( pstrW, &str, (BOOLEAN)alloc );
   else
        Status= RtlOemStringToUnicodeString( pstrW, &str, (BOOLEAN)alloc );

    if (NT_SUCCESS(Status))
       return pstrW->Buffer;

    if (Status== STATUS_BUFFER_OVERFLOW)
        SetLastError( ERROR_FILENAME_EXCED_RANGE );
    else
        BaseSetLastNTError(Status);

    return NULL;
}

/**************************************************************************
 *           SetFileCompletionNotificationModes   (KERNEL32.@)
 */
BOOL WINAPI SetFileCompletionNotificationModes( HANDLE handle, UCHAR flags )
{
    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}