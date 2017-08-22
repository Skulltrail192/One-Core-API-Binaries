/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    appmodel.c

Abstract:

    This module implements Windows Store APIs

Author:

    Skulltrail 19-July-2017

Revision History:

--*/

#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(appmodel);

/***********************************************************************
 *           GetCurrentPackageId       (KERNEL32.@)
 */
LONG WINAPI GetCurrentPackageId(UINT32 *len, BYTE *buffer)
{
    FIXME("(%p %p): stub\n", len, buffer);
    return APPMODEL_ERROR_NO_PACKAGE;
}

/***********************************************************************
 *           GetCurrentPackageFamilyName       (KERNEL32.@)
 */
LONG WINAPI GetCurrentPackageFamilyName(UINT32 *length, PWSTR name)
{
    FIXME("(%p %p): stub\n", length, name);
    return APPMODEL_ERROR_NO_PACKAGE;
}

/***********************************************************************
 *           GetCurrentPackageFullName       (KERNEL32.@)
 */
LONG WINAPI GetCurrentPackageFullName(UINT32 *length, PWSTR name)
{
    FIXME("(%p %p): stub\n", length, name);
    return APPMODEL_ERROR_NO_PACKAGE;
}

/***********************************************************************
 *           GetPackageFullName       (KERNEL32.@)
 */
LONG WINAPI GetPackageFullName(HANDLE process, UINT32 *length, PWSTR name)
{
    FIXME("(%p %p %p): stub\n", process, length, name);
    return APPMODEL_ERROR_NO_PACKAGE;
}
