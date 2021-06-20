/*
 * setupapi query functions
 *
 * Copyright 2006 James Hawkins
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
 */
 
#include "setupapi_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(setupapi); 
 
/***********************************************************************
 *      SetupGetInfDriverStoreLocationW (SETUPAPI.@)
 */
BOOL WINAPI SetupGetInfDriverStoreLocationW(
    PCWSTR FileName, PSP_ALTPLATFORM_INFO AlternativePlatformInfo,
    PCWSTR LocaleName, PWSTR ReturnBuffer, DWORD ReturnBufferSize,
    PDWORD RequiredSize)
{
    FIXME("stub: %s %p %s %p %u %p\n", debugstr_w(FileName), AlternativePlatformInfo, debugstr_w(LocaleName), ReturnBuffer, ReturnBufferSize, RequiredSize);

    SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
    return FALSE;
}