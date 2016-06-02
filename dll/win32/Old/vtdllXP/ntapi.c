/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
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
 *
 */
#define NDEBUG 
 
#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vtdll);

NTSTATUS NTAPI NtQueryOpenSubKeysEx(IN POBJECT_ATTRIBUTES  	TargetKey,
		IN ULONG  	BufferLength,
		IN PVOID  	Buffer,
		IN PULONG  	RequiredSize 
	) 		
{
    UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS NTAPI NtLoadKeyEx(
	IN POBJECT_ATTRIBUTES  	TargetKey,
	IN POBJECT_ATTRIBUTES  	SourceFile,
	IN ULONG  	Flags,
	IN HANDLE  	TrustClassKey 
)
{
    return 0x00000000;
} 	

