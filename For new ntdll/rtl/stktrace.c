/*++

Copyright (c) 1991  Microsoft Corporation

Module Name:

    stktrace.c

Abstract:

    This module implements routines to snapshot a set of stack back traces
    in a data base.  Useful for heap allocators to track allocation requests
    cheaply.

Author:

    Steve Wood (stevewo) 29-Jan-1992

Revision History:

    17-May-1999 (silviuc) : added RtlWalkFrameChain that replaces the
    unsafe RtlCaptureStackBackTrace.

--*/

#include <heap.h>
#include <rtl.h>
#include <tracedb.h>

#define NDEBUG
#include <debug.h>
#include <..\..\include\reactos\wine\exception.h>

#define 	MAX_STACK_DEPTH   64


PRTL_STACK_TRACE_ENTRY
RtlpExtendStackTraceDataBase(
    IN PRTL_STACK_TRACE_ENTRY InitialValue,
    IN ULONG Size
    )
/*++

Routine Description:

    This routine extends the stack trace database in order to accomodate
    the new stack trace that has to be saved.

Arguments:

    InitialValue - stack trace to be saved.
    
    Size - size of the stack trace in bytes. Note that this is not the
        depth of the trace but rather `Depth * sizeof(PVOID)'.

Return Value:

    The address of the just saved stack trace or null in case we have hit
    the maximum size of the database or we get commit errors.

Environment:

    User mode. 
    
    Note. In order to make all this code work in kernel mode we have to
    rewrite this function that relies on VirtualAlloc.

--*/

{
    NTSTATUS Status;
    PRTL_STACK_TRACE_ENTRY p, *pp;
    ULONG CommitSize;
    PSTACK_TRACE_DATABASE DataBase;

    DataBase = RtlpStackTraceDataBase;

    //
    // We will try to find space for one stack trace entry in the
    // upper part of the database.
    //

    pp = (PRTL_STACK_TRACE_ENTRY *)DataBase->NextFreeUpperMemory;

    if ((! DataBase->PreCommitted) &&
        ((PCHAR)(pp - 1) < (PCHAR)DataBase->CurrentUpperCommitLimit)) {

        //
        // No more committed space in the upper part of the database.
        // We need to extend it downwards.
        //

        DataBase->CurrentUpperCommitLimit = 
            (PVOID)((PCHAR)DataBase->CurrentUpperCommitLimit - PAGE_SIZE);

        if (DataBase->CurrentUpperCommitLimit < DataBase->CurrentLowerCommitLimit) {

            //
            // No more space at all. We have got over the lower part of the db.
            // We failed therefore increase back the UpperCommitLimit pointer.
            //

            DataBase->CurrentUpperCommitLimit = 
                (PVOID)((PCHAR)DataBase->CurrentUpperCommitLimit + PAGE_SIZE);

            return( NULL );
        }

        CommitSize = PAGE_SIZE;
        Status = ZwAllocateVirtualMemory( 
            NtCurrentProcess(),
            (PVOID *)&DataBase->CurrentUpperCommitLimit,
            0,
            &CommitSize,
            MEM_COMMIT,
            PAGE_READWRITE
            );

        if (!NT_SUCCESS( Status )) {

            //
            // We tried to increase the upper part of the db by one page.
            // We failed therefore increase back the UpperCommitLimit pointer
            //

            DataBase->CurrentUpperCommitLimit = 
                (PVOID)((PCHAR)DataBase->CurrentUpperCommitLimit + PAGE_SIZE);

            DPRINT( "RTL: Unable to commit space to extend stack trace data base - Status = %lx\n", Status);
            return( NULL );
        }
    }

    //
    // We managed to make sure we have usable space in the upper part
    // therefore we take out one stack trace entry address.
    //

    DataBase->NextFreeUpperMemory -= sizeof( *pp );

    //
    // Now we will try to find space in the lower part of the database for
    // for the eactual stack trace.
    //

    p = (PRTL_STACK_TRACE_ENTRY)DataBase->NextFreeLowerMemory;

    if ((! DataBase->PreCommitted) &&
        (((PCHAR)p + Size) > (PCHAR)DataBase->CurrentLowerCommitLimit)) {

        //
        // We need to extend the lower part.
        //

        if (DataBase->CurrentLowerCommitLimit >= DataBase->CurrentUpperCommitLimit) {

            //
            // We have hit the maximum size of the database.
            //

            return( NULL );
        }

        //
        // Extend the lower part of the database by one page.
        //

        CommitSize = Size;
        Status = ZwAllocateVirtualMemory( 
            NtCurrentProcess(),
            (PVOID *)&DataBase->CurrentLowerCommitLimit,
            0,
            &CommitSize,
            MEM_COMMIT,
            PAGE_READWRITE
            );

        if (! NT_SUCCESS( Status )) {
            DPRINT( "RTL: Unable to commit space to extend stack trace data base - Status = %lx\n",
                Status
                );
            return( NULL );
        }

        DataBase->CurrentLowerCommitLimit =
            (PCHAR)DataBase->CurrentLowerCommitLimit + CommitSize;
    }

    //
    // Take out the space for the stack trace.
    //

    DataBase->NextFreeLowerMemory += Size;

    //
    // Deal with a precommitted database case. If the lower and upper
    // pointers have crossed each other then rollback and return failure.
    //

    if (DataBase->PreCommitted &&
        DataBase->NextFreeLowerMemory >= DataBase->NextFreeUpperMemory) {

        DataBase->NextFreeUpperMemory += sizeof( *pp );
        DataBase->NextFreeLowerMemory -= Size;
        return( NULL );
    }

    //
    // Save the stack trace in the database
    //

    RtlMoveMemory( p, InitialValue, Size );
    p->HashChain = NULL;
    p->TraceCount = 0;
    p->Index = (USHORT)(++DataBase->NumberOfEntriesAdded);

    //
    // Save the address of the new stack trace entry in the
    // upper part of the databse.
    //

    *--pp = p;

    //
    // Return address of the saved stack trace entry.
    //

    return( p );
}

USHORT
NTAPI
RtlLogStackBackTrace( 
    VOID 
    )
/*++

Routine Description:

    This routine will capture the current stacktrace (skipping the 
    present function) and will save it in the global (per process) 
    stack trace database. It should be noted that we do not save
    duplicate traces.

Arguments:

    None.

Return Value:

    Index of the stack trace saved. The index can be used by tools
    to access quickly the trace data. This is the reason at the end of
    the database we save downwards a list of pointers to trace entries.
    This index can be used to find this pointer in constant time.
    
    A zero index will be returned for error conditions (e.g. stack 
    trace database not initialized).

Environment:

    User mode. 

--*/

{
    PSTACK_TRACE_DATABASE DataBase;
    RTL_STACK_TRACE_ENTRY StackTrace;
    PRTL_STACK_TRACE_ENTRY p, *pp;
    ULONG Hash, RequestedSize, DepthSize;

    if (RtlpStackTraceDataBase == NULL) {
        return 0;
        }

    Hash = 0;

    //
    // Capture stack trace. The try/except was useful
    // in the old days when the function did not validate
    // the stack frame chain. We keep it just ot be defensive.
    //

    __TRY {
        StackTrace.Depth = RtlCaptureStackBackTrace(
            1,
            MAX_STACK_DEPTH,
            StackTrace.BackTrace,
            &Hash
            );
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER) {
        StackTrace.Depth = 0;
    }
	__ENDTRY

    if (StackTrace.Depth == 0) {
        return 0;
    }

    //
    // Lock the global per-process stack trace database.
    //

    DataBase = RtlpAcquireStackTraceDataBase();
    
    if (DataBase == NULL) {
        return( 0 );
    }

    DataBase->NumberOfEntriesLookedUp++;

    __TRY {

        //
        // We will try to find out if the trace has been saved in the past.
        // We find the right hash chain and then traverse it.
        //

        DepthSize = StackTrace.Depth * sizeof( StackTrace.BackTrace[ 0 ] );
        pp = &DataBase->Buckets[ Hash % DataBase->NumberOfBuckets ];

        while ((p = *pp)) {
            if (p->Depth == StackTrace.Depth &&
                RtlCompareMemory( &p->BackTrace[ 0 ],
                &StackTrace.BackTrace[ 0 ],
                DepthSize
                ) == DepthSize
                ) {
                break;
            }
            else {
                pp = &p->HashChain;
            }
        }

        if (p == NULL) {

            //
            // We did not find the stack trace. We will extend the database
            // and save the new trace.
            //

            RequestedSize = FIELD_OFFSET( RTL_STACK_TRACE_ENTRY, BackTrace ) +
                DepthSize;

            p = RtlpExtendStackTraceDataBase( &StackTrace, RequestedSize );
            
            //
            // If we managed to stack the trace we need to link it as the last
            // element in the proper hash chain.
            //

            if (p != NULL) {
                *pp = p;
            }
        }
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER) {

        //
        // bugbug (silviuc): We should not be here. Right?
        //

        p = NULL;
    }
	__ENDTRY

    //
    // Release global trace db.
    //

    RtlpReleaseStackTraceDataBase();

    if (p != NULL) {
        p->TraceCount++;
        return( p->Index );
        }
    else {
        return( 0 );
        }

    return 0;
}

