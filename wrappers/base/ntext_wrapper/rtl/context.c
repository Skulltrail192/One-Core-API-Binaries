 /*
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
 *
Copyright (c) 2015  Microsoft Corporation 
 
Module Name:

    cnvint.c
 
 */
 
#define NDEBUG

#include <main.h> 

VOID
NTAPI
RtlSetExtendedFeaturesMask (
    __out PCONTEXT ContextEx,
    __in DWORD64 FeatureMask
)
{
	DbgPrint("UNIMPLEMENTED: RtlSetExtendedFeaturesMask");
	;
}

DWORD   
NTAPI
RtlInitializeExtendedContext (
    __out PVOID Context,
    __in DWORD ContextFlags,
    __out PCONTEXT* ContextEx
)
{
	DbgPrint("UNIMPLEMENTED: RtlInitializeExtendedContext");
	return 0;
}

NTSTATUS 
NTAPI 
RtlCopyContext(
	PCONTEXT Destination, 
	DWORD ContextFlags, 
	PCONTEXT Source
)
{
	DbgPrint("UNIMPLEMENTED: RtlCopyContext");	
	return STATUS_SUCCESS;
}

PCONTEXT 
NTAPI 
RtlLocateLegacyContext(
	PCONTEXT oldContext, 
	BOOL other
)
{
	DbgPrint("UNIMPLEMENTED: RtlLocateLegacyContext");	
	return oldContext;
}

NTSTATUS 
NTAPI 
RtlGetExtendedContextLength(
	DWORD flags, 
	LPDWORD ContextFlags)
{
	DbgPrint("UNIMPLEMENTED: RtlGetExtendedContextLength");	
	return STATUS_SUCCESS;
}