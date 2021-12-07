/*++

Copyright (c) 2021 Shorthorn Project

Module Name:

    virutal.c

Abstract:

    Implement functions for create and free UserStack

Author:

    Skulltrail 30-November-2021

Revision History:

--*/

#define NDEBUG

#include <main.h> 

/**********************************************************************
 *           RtlCreateUserStack (NTDLL.@)
 */
NTSTATUS NTAPI RtlCreateUserStack( SIZE_T commit, SIZE_T reserve, ULONG zero_bits,
                                    SIZE_T commit_align, SIZE_T reserve_align, INITIAL_TEB *stack )
{
    PROCESS_STACK_ALLOCATION_INFORMATION alloc;
    NTSTATUS status;

    DbgPrint("commit %#lx, reserve %#lx, zero_bits %u, commit_align %#lx, reserve_align %#lx, stack %p\n",
            commit, reserve, zero_bits, commit_align, reserve_align, stack);

    if (!commit_align || !reserve_align)
        return STATUS_INVALID_PARAMETER;

    if (!commit || !reserve)
    {
        IMAGE_NT_HEADERS *nt = RtlImageNtHeader( NtCurrentTeb()->ProcessEnvironmentBlock->ImageBaseAddress );
        if (!reserve) reserve = nt->OptionalHeader.SizeOfStackReserve;
        if (!commit) commit = nt->OptionalHeader.SizeOfStackCommit;
    }

    reserve = (reserve + reserve_align - 1) & ~(reserve_align - 1);
    commit = (commit + commit_align - 1) & ~(commit_align - 1);

    if (reserve < commit) reserve = commit;
    if (reserve < 0x100000) reserve = 0x100000;
    reserve = (reserve + 0xffff) & ~0xffff;  /* round to 64K boundary */

    alloc.ReserveSize = reserve;
    alloc.ZeroBits = zero_bits;
    status = NtSetInformationProcess( NtCurrentProcess(), ProcessThreadStackAllocation,
                                      &alloc, sizeof(alloc) );
    if (!status)
    {
        void *addr = alloc.StackBase;
        SIZE_T size = page_size;

        NtAllocateVirtualMemory( NtCurrentProcess(), &addr, 0, &size, MEM_COMMIT, PAGE_NOACCESS );
        addr = (char *)alloc.StackBase + page_size;
        NtAllocateVirtualMemory( NtCurrentProcess(), &addr, 0, &size, MEM_COMMIT, PAGE_READWRITE | PAGE_GUARD );
        addr = (char *)alloc.StackBase + 2 * page_size;
        size = reserve - 2 * page_size;
        NtAllocateVirtualMemory( NtCurrentProcess(), &addr, 0, &size, MEM_COMMIT, PAGE_READWRITE );

        /* note: limit is lower than base since the stack grows down */
        stack->PreviousStackBase = 0;
        stack->PreviousStackLimit = 0;
        stack->AllocatedStackBase = alloc.StackBase;
        stack->StackBase = (char *)alloc.StackBase + reserve;
        stack->StackLimit = (char *)alloc.StackBase + 2 * page_size;
    }
    return status;
}


/**********************************************************************
 *           RtlFreeUserStack (NTDLL.@)
 */
void NTAPI RtlFreeUserStack( void *stack )
{
    SIZE_T size = 0;

    DbgPrint("stack %p\n", stack);

    NtFreeVirtualMemory( NtCurrentProcess(), &stack, &size, MEM_RELEASE );
}