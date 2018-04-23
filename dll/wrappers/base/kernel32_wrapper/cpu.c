/*++

Copyright (c) 2018  Shorthorn Project

Module Name:

    curdir.c

Abstract:

    CPU specific functions

Author:

    Skulltrail 22-April-2018

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(kernel32);

/***********************************************************************
  *           GetEnabledXStateFeatures (KERNEL32.@)
  */
DWORD64 
WINAPI 
GetEnabledXStateFeatures(void)
{
     FIXME("\n");
     return 0;
}