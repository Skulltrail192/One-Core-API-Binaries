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

BOOLEAN RtlpEnvironCacheValid;

NTSTATUS
NTAPI
RtlSetEnvironmentStrings(
    IN PWCHAR NewEnvironment,
    IN SIZE_T NewEnvironmentSize
    )
/*++

Routine Description:

    This routine allows the replacement of the current environment block with a new one.

Arguments:

    NewEnvironment - Pointer to a set of zero terminated strings terminated by two terminators

    NewEnvironmentSize - Size of the block to put in place in bytes

Return Value:

    NTSTATUS - Status of function call

--*/
{
    PPEB Peb;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    PVOID pOld, pNew;
    NTSTATUS Status, Status1;
    SIZE_T NewSize, OldSize;
    MEMORY_BASIC_INFORMATION MemoryInformation;


    //
    // Assert if the block is not well formed
    //
    ASSERT (NewEnvironmentSize > sizeof (WCHAR) * 2);
    ASSERT ((NewEnvironmentSize & (sizeof (WCHAR) - 1)) == 0);
    ASSERT (NewEnvironment[NewEnvironmentSize/sizeof(WCHAR)-1] == L'\0');
    ASSERT (NewEnvironment[NewEnvironmentSize/sizeof(WCHAR)-2] == L'\0');

    Peb = NtCurrentPeb ();

    ProcessParameters = Peb->ProcessParameters;

    RtlAcquirePebLock ();

    pOld = ProcessParameters->Environment;

    Status = ZwQueryVirtualMemory (NtCurrentProcess (),
                                   pOld,
                                   MemoryBasicInformation,
                                   &MemoryInformation,
                                   sizeof (MemoryInformation),
                                   NULL);
    if (!NT_SUCCESS (Status)) {
        goto unlock_and_exit;
    }

    if (MemoryInformation.RegionSize >= NewEnvironmentSize) {
        RtlpEnvironCacheValid = FALSE;
        RtlCopyMemory (pOld, NewEnvironment, NewEnvironmentSize);
        Status = STATUS_SUCCESS;
        goto unlock_and_exit;
    }

    //
    // Drop the lock around expensive operations
    //

    RtlReleasePebLock ();

    pOld = NULL;
    pNew = NULL;

    NewSize = NewEnvironmentSize;

    Status = ZwAllocateVirtualMemory (NtCurrentProcess (),
                                      &pNew,
                                      0,
                                      &NewSize,
                                      MEM_COMMIT,
                                      PAGE_READWRITE);
    if (!NT_SUCCESS (Status)) {
        return Status;
    }

    //
    // Fill in the new block.
    //
    RtlCopyMemory (pNew, NewEnvironment, NewEnvironmentSize);

    //
    // Reacquire the lock. The existing block may have been reallocated
    // and so may now be big enough. Ignore this and use the block we
    // have created anyway.
    //

    RtlAcquirePebLock ();

    pOld = ProcessParameters->Environment;

    ProcessParameters->Environment = pNew;

    RtlpEnvironCacheValid = FALSE;

    RtlReleasePebLock ();


    //
    // Release the old block.
    //

    OldSize = 0;

    Status1 = ZwFreeVirtualMemory (NtCurrentProcess(),
                                   &pOld,
                                   &OldSize,
                                   MEM_RELEASE);

    ASSERT (NT_SUCCESS (Status1));

    return STATUS_SUCCESS;


unlock_and_exit:;
    RtlReleasePebLock ();
    return Status;
}

NTSTATUS 
NTAPI 
RtlCreateEnvironmentEx( 	
	_In_ PVOID  	SourceEnv,
	_Out_ PVOID *  	Environment,
	_In_ ULONG  	Flags 
) 	
{
	return RtlCreateEnvironment(FALSE, (PWCHAR*)Environment);
}
