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
 */
 
#define NDEBUG

#include <main.h>

NTSTATUS 
NTAPI 
RtlWow64EnableFsRedirectionEx(
	IN PVOID Wow64FsEnableRedirection,
	OUT PVOID *  	OldFsRedirectionLevel 
) 		
{
    /* This is what Windows returns on x86 */
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS 
NTAPI 
RtlWow64EnableFsRedirection(
	IN BOOLEAN  Wow64FsEnableRedirection
) 	
{
    /* This is what Windows returns on x86 */
    return STATUS_NOT_IMPLEMENTED;
}

NTAPI 
NtSetSystemPowerState( 	
	IN POWER_ACTION  	SystemAction,
	IN SYSTEM_POWER_STATE  	MinSystemState,
	IN ULONG  	Flags 
)
{
	return STATUS_SUCCESS;
} 	