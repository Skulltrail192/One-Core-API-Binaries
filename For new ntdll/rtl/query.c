/* COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * FILE:            lib/rtl/query.c
 * PURPOSE:         RTL Query backend allocator
 * PROGRAMMERS:     Copyright 2010 Aleksey Bragin
 */

/* Useful references:
   http://msdn.microsoft.com/en-us/library/ms810466.aspx
   http://msdn.microsoft.com/en-us/library/ms810603.aspx
   http://www.securitylab.ru/analytics/216376.php
   http://binglongx.spaces.live.com/blog/cns!142CBF6D49079DE8!596.entry
   http://www.phreedom.org/research/exploits/asn1-bitstring/
   http://illmatics.com/Understanding_the_LFH.pdf
   http://www.alex-ionescu.com/?p=18
*/

/* INCLUDES *****************************************************************/

#include <rtl.h>
#include <heap.h>
#include <ntdef.h>
#include <tracedb.h>
#include <..\..\include\reactos\wine\exception.h>

VOID
RtlpDeCommitQueryDebugInfo(
    IN PRTL_DEBUG_INFORMATION Buffer,
    IN PVOID p,
    IN ULONG Size
    )
{
    Size = (Size + 3) & ~3;
    if (p == (PVOID)(Buffer->OffsetFree - Size)) {
        Buffer->OffsetFree -= Size;
        }
}

PVOID
NTAPI
RtlpCommitQueryDebugInfo(
    IN PRTL_DEBUG_INFORMATION Buffer,
    IN ULONG Size
    )
{
    NTSTATUS Status;
    PVOID Result;
    PVOID CommitBase;
    SIZE_T CommitSize;
    SIZE_T NeededSize;

    Size = (Size + 3) & ~3;
    NeededSize = Buffer->OffsetFree + Size;
    if (NeededSize > Buffer->CommitSize) {
        if (NeededSize >= Buffer->ViewSize) {
            return NULL;
            }

        CommitBase = (PCHAR)Buffer + Buffer->CommitSize;
        CommitSize =  NeededSize - Buffer->CommitSize;
        Status = NtAllocateVirtualMemory( NtCurrentProcess(),
                                          &CommitBase,
                                          0,
                                          &CommitSize,
                                          MEM_COMMIT,
                                          PAGE_READWRITE
                                        );
        if (!NT_SUCCESS( Status )) {
            return NULL;
            }


        Buffer->CommitSize += CommitSize;
        }

    Result = (PCHAR)Buffer + Buffer->OffsetFree;
    Buffer->OffsetFree = NeededSize;
    return Result;
}

NTSTATUS
RtlpQueryProcessEnumHeapsRoutine(
    PVOID HeapHandle,
    PVOID Parameter
    )
{
    PRTL_DEBUG_INFORMATION Buffer = (PRTL_DEBUG_INFORMATION)Parameter;
    PRTL_PROCESS_HEAPS Heaps = Buffer->Heaps;
    PHEAP Heap = (PHEAP)HeapHandle;
    PRTL_HEAP_INFORMATION HeapInfo;
    PHEAP_SEGMENT Segment;
    UCHAR SegmentIndex;

    HeapInfo = RtlpCommitQueryDebugInfo( Buffer, sizeof( *HeapInfo ) );
    if (HeapInfo == NULL) {
        return STATUS_NO_MEMORY;
        }

    HeapInfo->BaseAddress = Heap;
    HeapInfo->Flags = Heap->Flags;
    HeapInfo->EntryOverhead = sizeof( HEAP_ENTRY );
    HeapInfo->CreatorBackTraceIndex = Heap->AllocatorBackTraceIndex;
    SegmentIndex = HEAP_MAXIMUM_SEGMENTS;
    while (SegmentIndex--) {
        Segment = Heap->Segments[ SegmentIndex ];
        if (Segment) {
            HeapInfo->BytesCommitted += (Segment->NumberOfPages -
                                         Segment->NumberOfUnCommittedPages
                                        ) * PAGE_SIZE;

            }
        }
    HeapInfo->BytesAllocated = HeapInfo->BytesCommitted -
                               (Heap->TotalFreeSize << HEAP_GRANULARITY_SHIFT);
    Heaps->NumberOfHeaps += 1;
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
RtlQueryProcessLockInformation(
    IN OUT PRTL_DEBUG_INFORMATION Buffer
    )
{
    NTSTATUS Status;
    PLIST_ENTRY Head, Next;
    PRTL_PROCESS_LOCKS Locks;
    PRTL_PROCESS_LOCK_INFORMATION LockInfo;
    PRTL_CRITICAL_SECTION CriticalSection;
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
    PRTL_RESOURCE Resource;
    PRTL_RESOURCE_DEBUG ResourceDebugInfo;

    Locks = RtlpCommitQueryDebugInfo( Buffer, FIELD_OFFSET( RTL_PROCESS_LOCKS, Locks ) );
    if (Locks == NULL) {
        return STATUS_NO_MEMORY;
        }

    RtlEnterCriticalSection( &RtlCriticalSectionLock );
    Head = &RtlCriticalSectionList;
    Next = Head->Flink;
    Status = STATUS_SUCCESS;
    while (Next != Head) {
        DebugInfo = CONTAINING_RECORD( Next,
                                       RTL_CRITICAL_SECTION_DEBUG,
                                       ProcessLocksList
                                     );
        LockInfo = RtlpCommitQueryDebugInfo( Buffer, sizeof( RTL_PROCESS_LOCK_INFORMATION ) );
        if (LockInfo == NULL) {
            Status = STATUS_NO_MEMORY;
            break;
            }

        CriticalSection = DebugInfo->CriticalSection;
        __TRY {
            LockInfo->Address = CriticalSection;
            LockInfo->Type = DebugInfo->Type;
            LockInfo->CreatorBackTraceIndex = DebugInfo->CreatorBackTraceIndex;
            if (LockInfo->Type == RTL_CRITSECT_TYPE) {
                LockInfo->OwnerThreadId = (ULONG)CriticalSection->OwningThread;
                LockInfo->ActiveCount = CriticalSection->LockCount;
                LockInfo->RecursionCount = CriticalSection->RecursionCount;
                LockInfo->ContentionCount = DebugInfo->ContentionCount;
                LockInfo->EntryCount = DebugInfo->EntryCount;
                }
            else {
                Resource = (PRTL_RESOURCE)CriticalSection;
                ResourceDebugInfo = Resource->DebugInfo;
                LockInfo->ContentionCount = ResourceDebugInfo->ContentionCount;
                LockInfo->OwnerThreadId = (ULONG)Resource->OwningThread;
                LockInfo->ActiveCount = Resource->NumberActive;
                LockInfo->NumberOfSharedWaiters    = Resource->SharedWaiters;
                LockInfo->NumberOfExclusiveWaiters = Resource->ExclusiveWaiters;
                }

            Locks->NumberOfLocks++;
            }
        _SEH2_EXCEPT (EXCEPTION_EXECUTE_HANDLER) {
            DbgPrint("NTDLL: Lost critical section %08lX\n", CriticalSection);
            RtlpDeCommitQueryDebugInfo( Buffer, LockInfo, sizeof( RTL_PROCESS_LOCK_INFORMATION ) );
            }
		__ENDTRY
        if (Next == Next->Flink) {
            //
            // Bail if list is circular
            //
            break;
            }
        else {
            Next = Next->Flink;
            }
        }
		
		

    RtlLeaveCriticalSection( &RtlCriticalSectionLock );

    if (NT_SUCCESS( Status )) {
        Buffer->Locks = Locks;
        }

    return Status;
}

NTSTATUS
NTAPI
RtlQueryProcessHeapInformation(
    IN OUT PRTL_DEBUG_INFORMATION Buffer
    )
{
    NTSTATUS Status;
    PHEAP Heap;
    BOOLEAN LockAcquired;
    PRTL_PROCESS_HEAPS Heaps;
    PRTL_HEAP_INFORMATION HeapInfo;
    UCHAR SegmentIndex;
    ULONG i, n, TagIndex;
    PHEAP_SEGMENT Segment;
    PRTL_HEAP_TAG Tags;
    PHEAP_PSEUDO_TAG_ENTRY PseudoTags;
    PRTL_HEAP_ENTRY Entries;
    PHEAP_ENTRY CurrentBlock;
    PHEAP_ENTRY_EXTRA ExtraStuff;
    PLIST_ENTRY Head, Next;
    PHEAP_VIRTUAL_ALLOC_ENTRY VirtualAllocBlock;
    ULONG Size;
    PHEAP_UNCOMMMTTED_RANGE UnCommittedRange;

    Heaps = RtlpCommitQueryDebugInfo( Buffer, FIELD_OFFSET( RTL_PROCESS_HEAPS, Heaps ) );
    if (Heaps == NULL) {
        return STATUS_NO_MEMORY;
        }

    Buffer->Heaps = Heaps;
    Status = RtlEnumProcessHeaps( (PHEAP_ENUMERATION_ROUTINE)RtlpQueryProcessEnumHeapsRoutine, Buffer );
    if (NT_SUCCESS( Status )) {
        if (Buffer->Flags & RTL_QUERY_PROCESS_HEAP_TAGS) {
            Heap = RtlpGlobalTagHeap;
            if (Heap->TagEntries != NULL) {
                HeapInfo = RtlpCommitQueryDebugInfo( Buffer, sizeof( *HeapInfo ) );
                if (HeapInfo == NULL) {
                    return STATUS_NO_MEMORY;
                    }

                HeapInfo->BaseAddress = Heap;
                HeapInfo->Flags = Heap->Flags;
                HeapInfo->EntryOverhead = sizeof( HEAP_ENTRY );
                Heaps->NumberOfHeaps += 1;
                }

            for (i=0; i<Heaps->NumberOfHeaps; i++) {
                HeapInfo = &Heaps->Heaps[ i ];
                if (Buffer->SpecificHeap == NULL ||
                    Buffer->SpecificHeap == HeapInfo->BaseAddress
                   ) {
                    Heap = HeapInfo->BaseAddress;
                    HeapInfo->NumberOfTags = Heap->NextAvailableTagIndex;
                    n = HeapInfo->NumberOfTags * sizeof( RTL_HEAP_TAG );
                    if (Heap->PseudoTagEntries != NULL) {
                        HeapInfo->NumberOfTags += HEAP_MAXIMUM_FREELISTS + 1;
                        n += (HEAP_MAXIMUM_FREELISTS + 1) * sizeof( RTL_HEAP_TAG );
                        }
                    Tags = RtlpCommitQueryDebugInfo( Buffer, n );
                    if (Tags == NULL) {
                        Status = STATUS_NO_MEMORY;
                        break;
                        }
                    HeapInfo->Tags = Tags;
                    if ((PseudoTags = Heap->PseudoTagEntries) != NULL) {
                        HeapInfo->NumberOfPseudoTags = HEAP_NUMBER_OF_PSEUDO_TAG;
                        HeapInfo->PseudoTagGranularity = HEAP_GRANULARITY;
                        for (TagIndex=0; TagIndex<=HEAP_MAXIMUM_FREELISTS; TagIndex++) {
                            Tags->NumberOfAllocations = PseudoTags->Allocs;
                            Tags->NumberOfFrees = PseudoTags->Frees;
                            Tags->BytesAllocated = PseudoTags->Size << HEAP_GRANULARITY_SHIFT;
                            Tags->TagIndex = (USHORT)(TagIndex | HEAP_PSEUDO_TAG_FLAG);
                            if (TagIndex == 0) {
                                swprintf( Tags->TagName, L"Objects>%4u",
                                          HEAP_MAXIMUM_FREELISTS << HEAP_GRANULARITY_SHIFT
                                        );
                                }
                            else
                            if (TagIndex < HEAP_MAXIMUM_FREELISTS) {
                                swprintf( Tags->TagName, L"Objects=%4u",
                                          TagIndex << HEAP_GRANULARITY_SHIFT
                                        );
                                }
                            else {
                                swprintf( Tags->TagName, L"VirtualAlloc",
                                          TagIndex << HEAP_GRANULARITY_SHIFT
                                        );
                                }

                            Tags += 1;
                            PseudoTags += 1;
                            }
                        }

                    RtlMoveMemory( Tags,
                                   Heap->TagEntries,
                                   Heap->NextAvailableTagIndex * sizeof( RTL_HEAP_TAG )
                                 );
                    for (TagIndex=0; TagIndex<Heap->NextAvailableTagIndex; TagIndex++) {
                        Tags->BytesAllocated <<= HEAP_GRANULARITY_SHIFT;
                        Tags += 1;
                        }
                    }
                }
            }
        }
    else {
        Buffer->Heaps = NULL;
        }
    if (NT_SUCCESS( Status )) {
        if (Buffer->Flags & RTL_QUERY_PROCESS_HEAP_ENTRIES) {
            for (i=0; i<Heaps->NumberOfHeaps; i++) {
                HeapInfo = &Heaps->Heaps[ i ];
                Heap = HeapInfo->BaseAddress;
                if (Buffer->SpecificHeap == NULL ||
                    Buffer->SpecificHeap == Heap
                   ) {
                    if (!(Heap->Flags & HEAP_NO_SERIALIZE)) {
                        RtlEnterCriticalSection( (PRTL_CRITICAL_SECTION)Heap->LockVariable );
                        LockAcquired = TRUE;
                        }
                    else {
                        LockAcquired = FALSE;
                        }

                    __TRY {
                        for (SegmentIndex=0; SegmentIndex<HEAP_MAXIMUM_SEGMENTS; SegmentIndex++) {
                            Segment = Heap->Segments[ SegmentIndex ];
                            if (!Segment) {
                                continue;
                                }

                            Entries = RtlpCommitQueryDebugInfo( Buffer, sizeof( *Entries ) );
                            if (Entries == NULL) {
                                Status = STATUS_NO_MEMORY;
                                break;
                                }
                            else
                            if (HeapInfo->Entries == NULL) {
                                HeapInfo->Entries = Entries;
                                }

                            Entries->Flags = RTL_HEAP_SEGMENT;
                            Entries->AllocatorBackTraceIndex = Segment->SegmentAllocatorBackTraceIndex;
                            Entries->Size = Segment->NumberOfPages * PAGE_SIZE;
                            Entries->u.s2.CommittedSize = (Segment->NumberOfPages -
                                                           Segment->NumberOfUnCommittedPages
                                                          ) * PAGE_SIZE;
                            Entries->u.s2.FirstBlock = Segment->FirstEntry;
                            HeapInfo->NumberOfEntries++;

                            UnCommittedRange = (PHEAP_UNCOMMMTTED_RANGE)Segment->NumberOfUnCommittedRanges;
                            CurrentBlock = Segment->FirstEntry;
                            while (CurrentBlock < Segment->LastValidEntry) {
                                Entries = RtlpCommitQueryDebugInfo( Buffer, sizeof( *Entries ) );
                                if (Entries == NULL) {
                                    Status = STATUS_NO_MEMORY;
                                    break;
                                    }

                                Size = CurrentBlock->Size << HEAP_GRANULARITY_SHIFT;
                                Entries->Size = Size;
                                HeapInfo->NumberOfEntries++;
                                if (CurrentBlock->Flags & HEAP_ENTRY_BUSY) {
                                    if (CurrentBlock->Flags & HEAP_ENTRY_EXTRA_PRESENT) {
                                        ExtraStuff = (PHEAP_ENTRY_EXTRA)(CurrentBlock + CurrentBlock->Size - 1);
#if i386
                                        Entries->AllocatorBackTraceIndex = ExtraStuff->AllocatorBackTraceIndex;
#endif // i386
                                        Entries->Flags |= RTL_HEAP_SETTABLE_VALUE;
                                        Entries->u.s1.Settable = ExtraStuff->Settable;
                                        Entries->u.s1.Tag = ExtraStuff->TagIndex;
                                        }
                                    else {
                                        Entries->u.s1.Tag = CurrentBlock->SmallTagIndex;
                                        }

                                    Entries->Flags |= RTL_HEAP_BUSY | (CurrentBlock->Flags & HEAP_ENTRY_SETTABLE_FLAGS);
                                    }
                                else
                                if (CurrentBlock->Flags & HEAP_ENTRY_EXTRA_PRESENT) {
                                    PHEAP_FREE_ENTRY_EXTRA FreeExtra;

                                    FreeExtra = (PHEAP_FREE_ENTRY_EXTRA)(CurrentBlock + CurrentBlock->Size) - 1;
                                    Entries->u.s1.Tag = FreeExtra->TagIndex;
                                    Entries->AllocatorBackTraceIndex = FreeExtra->FreeBackTraceIndex;
                                    }

                                if (CurrentBlock->Flags & HEAP_ENTRY_LAST_ENTRY) {
                                    CurrentBlock += CurrentBlock->Size;
                                    if (UnCommittedRange == NULL) {
                                        CurrentBlock = Segment->LastValidEntry;
                                        }
                                    else {
                                        Entries = RtlpCommitQueryDebugInfo( Buffer, sizeof( *Entries ) );
                                        if (Entries == NULL) {
                                            Status = STATUS_NO_MEMORY;
                                            break;
                                            }

                                        Entries->Flags = RTL_HEAP_UNCOMMITTED_RANGE;
                                        Entries->Size = UnCommittedRange->Size;
                                        HeapInfo->NumberOfEntries++;

                                        CurrentBlock = (PHEAP_ENTRY)
                                            ((PCHAR)UnCommittedRange->Address + UnCommittedRange->Size);
                                        UnCommittedRange = UnCommittedRange->Next;
                                        }
                                    }
                                else {
                                    CurrentBlock += CurrentBlock->Size;
                                    }
                                }
                            }

                        Head = &Heap->VirtualAllocdBlocks;
                        Next = Head->Flink;
                        while (Head != Next) {
                            VirtualAllocBlock = CONTAINING_RECORD( Next, HEAP_VIRTUAL_ALLOC_ENTRY, Entry );
                            CurrentBlock = &VirtualAllocBlock->BusyBlock;
                            Entries = RtlpCommitQueryDebugInfo( Buffer, sizeof( *Entries ) );
                            if (Entries == NULL) {
                                Status = STATUS_NO_MEMORY;
                                break;
                                }
                            else
                            if (HeapInfo->Entries == NULL) {
                                HeapInfo->Entries = Entries;
                                }

                            Entries->Flags = RTL_HEAP_SEGMENT;
                            Entries->Size = VirtualAllocBlock->ReserveSize;
                            Entries->u.s2.CommittedSize = VirtualAllocBlock->CommitSize;
                            Entries->u.s2.FirstBlock = CurrentBlock;
                            HeapInfo->NumberOfEntries++;
                            Entries = RtlpCommitQueryDebugInfo( Buffer, sizeof( *Entries ) );
                            if (Entries == NULL) {
                                Status = STATUS_NO_MEMORY;
                                break;
                                }

                            Entries->Size = VirtualAllocBlock->CommitSize;
                            Entries->Flags = RTL_HEAP_BUSY | (CurrentBlock->Flags & HEAP_ENTRY_SETTABLE_FLAGS);
#if i386
                            Entries->AllocatorBackTraceIndex = VirtualAllocBlock->ExtraStuff.AllocatorBackTraceIndex;
#endif // i386
                            Entries->Flags |= RTL_HEAP_SETTABLE_VALUE;
                            Entries->u.s1.Settable = VirtualAllocBlock->ExtraStuff.Settable;
                            Entries->u.s1.Tag = VirtualAllocBlock->ExtraStuff.TagIndex;
                            HeapInfo->NumberOfEntries++;

                            Next = Next->Flink;
                            }
                        }
                    __FINALLY() {
                        //
                        // Unlock the heap
                        //

                        if (LockAcquired) {
                            RtlLeaveCriticalSection( (PRTL_CRITICAL_SECTION)Heap->LockVariable );
                            }
                        }
					__ENDTRY
                    }

                if (!NT_SUCCESS( Status )) {
                    break;
                    }
                }
            }
        }

    return Status;
}

NTSTATUS
NTAPI
RtlQueryProcessBackTraceInformation(
    IN OUT PRTL_DEBUG_INFORMATION Buffer
    )
{
#if i386
    NTSTATUS Status;
    OUT PRTL_PROCESS_BACKTRACES BackTraces;
    PRTL_PROCESS_BACKTRACE_INFORMATION BackTraceInfo;
    PSTACK_TRACE_DATABASE DataBase;
    PRTL_STACK_TRACE_ENTRY p, *pp;
    ULONG n;

    DataBase = RtlpAcquireStackTraceDataBase();
    if (DataBase == NULL) {
        return STATUS_SUCCESS;
        }

    BackTraces = RtlpCommitQueryDebugInfo( Buffer, FIELD_OFFSET( RTL_PROCESS_BACKTRACES, BackTraces ) );
    if (BackTraces == NULL) {
        return STATUS_NO_MEMORY;
        }

    DataBase->DumpInProgress = TRUE;
    RtlpReleaseStackTraceDataBase();
    __TRY {
        BackTraces->CommittedMemory = (ULONG)DataBase->CurrentUpperCommitLimit -
                                      (ULONG)DataBase->CommitBase;
        BackTraces->ReservedMemory =  (ULONG)DataBase->EntryIndexArray -
                                      (ULONG)DataBase->CommitBase;
        BackTraces->NumberOfBackTraceLookups = DataBase->NumberOfEntriesLookedUp;
        BackTraces->NumberOfBackTraces = DataBase->NumberOfEntriesAdded;
        BackTraceInfo = RtlpCommitQueryDebugInfo( Buffer, (sizeof( *BackTraceInfo ) * BackTraces->NumberOfBackTraces) );
        if (BackTraceInfo == NULL) {
            Status = STATUS_NO_MEMORY;
            }
        else {
            Status = STATUS_SUCCESS;
            n = DataBase->NumberOfEntriesAdded;
            pp = DataBase->EntryIndexArray;
            while (n--) {
                p = *--pp;
                BackTraceInfo->SymbolicBackTrace = NULL;
                BackTraceInfo->TraceCount = p->TraceCount;
                BackTraceInfo->Index = p->Index;
                BackTraceInfo->Depth = p->Depth;
                RtlMoveMemory( BackTraceInfo->BackTrace,
                               p->BackTrace,
                               p->Depth * sizeof( PVOID )
                             );
                BackTraceInfo++;
                }
            }
        }
    __FINALLY() {
        DataBase->DumpInProgress = FALSE;
        }
	__ENDTRY
    if (NT_SUCCESS( Status )) {
        Buffer->BackTraces = BackTraces;
        }

    return Status;
#else
    return STATUS_SUCCESS;
#endif // i386
}