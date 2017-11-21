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
 * Copyright (c) 2015  Microsoft Corporation 
 *  
 * Module Name:
 *
 *  message.c 
 */
  
#define NDEBUG

#include <main.h>

NTAPI
RtlFormatMessage(
    IN PWSTR Message,
    IN ULONG MaxWidth OPTIONAL,
    IN BOOLEAN IgnoreInserts,
    IN BOOLEAN ArgumentsAreAnsi,
    IN BOOLEAN ArgumentsAreAnArray,
    IN va_list* Arguments,
    OUT PWSTR Buffer,
    IN ULONG BufferSize,
    OUT PULONG ReturnLength OPTIONAL);

NTSTATUS
NTAPI
RtlFormatMessageEx(
    IN PWSTR Message,
    IN ULONG MaxWidth OPTIONAL,
    IN BOOLEAN IgnoreInserts,
    IN BOOLEAN ArgumentsAreAnsi,
    IN BOOLEAN ArgumentsAreAnArray,
    IN va_list* Arguments,
    OUT PWSTR Buffer,
    IN ULONG BufferSize,
    OUT PULONG ReturnLength OPTIONAL,
    IN ULONG Flags)
{
    return RtlFormatMessage(Message,
							MaxWidth,
							IgnoreInserts,
							ArgumentsAreAnsi,
							ArgumentsAreAnArray,
							Arguments,
							Buffer,
							BufferSize,
							ReturnLength);
}
