/*++

Copyright (c) 2022 Shorthorn Project

Module Name:

    input.c

Abstract:

    Implement Input functions

Author:

    Skulltrail 14-February-2022

Revision History:

--*/

#include <main.h>

/***********************************************************************
 *		EnableMouseInPointer (USER32.@)
 */
BOOL WINAPI EnableMouseInPointer(BOOL enable)
{
    DbgPrint("EnableMouseInPointer stub\n", enable);

    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}