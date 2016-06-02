/*++

Copyright (c) 1991  Microsoft Corporation

Module Name:

    uilist.c

Abstract:

    Contains routine to convert a list of workstation names from UI/Service
    list format to API list format

    Contents:
        RtlConvertUiListToApiList
        (NextElement)
        (ValidateName)

Author:

    Richard L Firth (rfirth) 01-May-1992

Environment:

    User mode (makes Windows calls)

Revision History:

--*/

#include <rtl.h>

#define NDEBUG
#include <debug.h>
#include <..\..\include\reactos\wine\exception.h>

int NTAPI RtlGetFrame()
{
  return *(DWORD *)(__readfsdword(24) + 4016);
}