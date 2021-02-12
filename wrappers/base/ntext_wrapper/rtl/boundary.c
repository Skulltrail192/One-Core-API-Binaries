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
RtlAddIntegrityLabelToBoundaryDescriptor(
	HANDLE *BoundaryDescriptor, 
	PSID IntegrityLabel
)
{
	DbgPrint("UNIMPLEMENTED: RtlAddIntegrityLabelToBoundaryDescriptor\n");
	return STATUS_SUCCESS;
}

NTSTATUS 
NTAPI
RtlAddSIDToBoundaryDescriptor(
  _Inout_  HANDLE *BoundaryDescriptor,
  _In_     PSID RequiredSid
)
{
	DbgPrint("UNIMPLEMENTED: RtlAddSIDToBoundaryDescriptor");
	return STATUS_SUCCESS;
}

HANDLE 
NTAPI
RtlCreateBoundaryDescriptor(
	LSA_UNICODE_STRING *string, 
	ULONG Flags
)
{
	return NULL;
}

VOID 
WINAPI 
RtlDeleteBoundaryDescriptor(
  _In_ HANDLE BoundaryDescriptor
)
{
	RtlFreeHeap(NtCurrentTeb()->ProcessEnvironmentBlock->ProcessHeap, 8, BoundaryDescriptor);
}
