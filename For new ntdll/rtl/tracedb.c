/*
 * COPYRIGHT:         See COPYING in the top level directory
 * PROJECT:           ReactOS system libraries
 * PURPOSE:           Rtl user thread functions
 * FILE:              lib/rtl/tracedb.c
 * PROGRAMERS:
 *                    Alex Ionescu (alex@relsoft.net)
 *                    Eric Kohl
 *                    KJK::Hyperion
 */
 
#include "tracedb.h"
#include "heap.h"
 
#if DBG
#define TRACE_ASSERT(Expr) {                                              \
    if (!(Expr)) {                                                        \
        DbgPrint ("Page heap: (%s, %d): \" %s \" -- assertion failed \n", \
          __FILE__, __LINE__, #Expr);                                     \
        DbgBreakPoint ();                                                 \
    }}
#else
#define TRACE_ASSERT(Expr)
#endif // #if DBG 

#define RTL_TRACE_BLOCK_MAGIC       0xABCDAAAA
#define RTL_TRACE_SEGMENT_MAGIC     0xABCDBBBB
#define RTL_TRACE_DATABASE_MAGIC    0xABCDCCCC

#define RTL_TRACE_IN_USER_MODE       0x00000001
#define RTL_TRACE_IN_KERNEL_MODE     0x00000002
#define RTL_TRACE_USE_NONPAGED_POOL  0x00000004
#define RTL_TRACE_USE_PAGED_POOL     0x00000008

#define RTL_TRACE_SIZE_INCREMENT PAGE_SIZE

BOOLEAN 
RtlpTraceDatabaseFree (
    PVOID Block,
    IN ULONG Tag    // OPTIONAL in User mode
    )
{
  PVOID receiveBlock; // eax@1
  PVOID localBlock; // [sp+0h] [bp-4h]@1

  receiveBlock = Block;
  Block = 0;
  localBlock = receiveBlock;
  return NtFreeVirtualMemory((HANDLE)-1, &localBlock, (PSIZE_T)Block, 32768) >= 0;
}

BOOLEAN 
RtlpTraceDatabaseAcquireLock (
    IN PRTL_TRACE_DATABASE Database
    )
/*++

Routine Description:
    
    This routine acquires the trace database lock.
    It hides all details about the actual nature of the lock.

Arguments:

    Database - trace database

Return Value:

    TRUE if successful.

Environment:

    Internal trace database module function.

--*/
{   
	RtlEnterCriticalSection(&Database->Lock);
    Database->Owner = 0;
    return TRUE;
}

BOOLEAN 
RtlpTraceDatabaseReleaseLock (
    IN PRTL_TRACE_DATABASE Database
    )
/*++

Routine Description:
    
    This routine acquires the trace database lock.
    It hides all details about the actual nature of the lock.

Arguments:

    Database - trace database

Return Value:

    TRUE if successful.

Environment:

    Internal trace database module function.

--*/
{   
    Database->Owner = 0;
	RtlLeaveCriticalSection(&Database->Lock);
    return TRUE;
}

ULONG 
RtlpTraceStandardHashFunction (
    IN ULONG Count,
    IN PVOID * Trace
    )
/*++

Routine Description:

    This routine is a simple hash function for stack traces in
    the case the caller of RtlTraceDatabaseCreate does not provide
    one. The function just xor's together all the pointers in the
    trace.

Arguments:

    Count - size of trace (in PVOIDs)
    
    Trace - trace

Return Value:

    Hash value. This needs to be reduced to the number of buckets
    in the hash table by a modulo operation (or something similar).

Environment:

    Called internally by RtlpTraceDatabaseInternalAdd/Find.

--*/
{
    ULONG_PTR Value = 0;
    ULONG Index;
    unsigned short * Key; 

    Key = (unsigned short *)Trace;
    for (Index = 0; Index < Count * (sizeof (PVOID) / sizeof(*Key)); Index += 2) {

        Value += Key[Index] ^ Key[Index + 1];
    }

    return PtrToUlong ((PVOID)Value);
}

BOOLEAN
NTAPI
RtlTraceDatabaseValidate (
    IN PRTL_TRACE_DATABASE Database
    )
/*++

Routine Description:

    This routine validates the correctness of a trace database.
    It is intended to be used for testing purposes.

Arguments:

    Database - trace database

Return Value:

    TRUE if the database is ok. For most of the inconsistencies this
    function will break in the debugger.

Environment:

    Any.

--*/
{
    PRTL_TRACE_SEGMENT Segment;
    PRTL_TRACE_BLOCK Block;
    ULONG Index;

    TRACE_ASSERT (Database != NULL);
    TRACE_ASSERT (Database->Magic == RTL_TRACE_DATABASE_MAGIC);

    //
    // Check all segments.
    //

    for (Segment = Database->SegmentList;
         Segment != NULL;
         Segment = Segment->NextSegment) {

        TRACE_ASSERT (Segment->Magic == RTL_TRACE_SEGMENT_MAGIC);
    }

    //
    // Check all blocks.
    //
    
    for (Index = 0; Index < Database->NoOfBuckets; Index++) {

        for (Block = Database->Buckets[Index];
             Block != NULL;
             Block = Block->Next) {

            TRACE_ASSERT (Block->Magic == RTL_TRACE_BLOCK_MAGIC);
        }
    }

    return TRUE;
}

BOOLEAN NTAPI RtlpTraceDatabaseInitializeLock(PRTL_TRACE_DATABASE database)
{
  RtlInitializeCriticalSection(&database->Lock);
  return TRUE;
}

BOOLEAN 
RtlpTraceDatabaseUninitializeLock (
    IN PRTL_TRACE_DATABASE Database
    )
/*++

Routine Description:
    
    This routine uninitializes the trace database lock.
    It hides all details about the actual nature of the lock.
    (e.g. In user mode we need to call RtlDeleteCriticalSection).

Arguments:

    Database - trace database

Return Value:

    TRUE if successful.

Environment:

    Internal trace database module function.

--*/
{

    RtlDeleteCriticalSection(&Database->Lock);
	return TRUE;
}

BOOLEAN
NTAPI
RtlTraceDatabaseDestroy (
    IN PRTL_TRACE_DATABASE Database
    )
/*++

Routine Description:

    This routine destroys a trace database. It takes care of
    deallocating everything, uninitializing synchronization 
    objects, etc.

Arguments:

    Database - trace database

Return Value:

    TRUE if destroy operation was successful. FALSE otherwise.

Environment:

    Any.

--*/
{
    PRTL_TRACE_SEGMENT Current;
    BOOLEAN Success;
    BOOLEAN SomethingFailed = FALSE;
    PRTL_TRACE_SEGMENT NextSegment;

    //
    // Sanity checks.
    //

    TRACE_ASSERT (Database && Database->Magic == RTL_TRACE_DATABASE_MAGIC);
    TRACE_ASSERT (RtlTraceDatabaseValidate (Database));

    //
    // Uninitialize the database lock. Even if we fail
    // we will continue to release memory for all segments.
    //
    // N.B. We cannot acquire the lock here for the last time because this
    // has as a side effect elevating the irql (in K mode) and then the 
    // function will return with raised irql.
    //

    Success = RtlpTraceDatabaseUninitializeLock (Database);

    if (! Success) {
        SomethingFailed = TRUE;
    }

    //
    // Traverse the list of segments and release memory one by one.
    // Special attention with the last segment because it contains
    // the database structure itself and we do not want to shoot.
    // ourselves in the foot.
    //

    for (Current = Database->SegmentList;
         Current != NULL;
         Current = NextSegment) {

        //
        // We save the next segment before freeing the structure.
        //

        NextSegment = Current->NextSegment;

        //
        // If this is the last segment we need to offset Current pointer
        // by the size of the database structure.
        //

        if (NextSegment == NULL) {
            
            Current = (PRTL_TRACE_SEGMENT) ((PRTL_TRACE_DATABASE)Current - 1);
        }
    }

    if (SomethingFailed) {
        return FALSE;
    }
    else {
        return TRUE;
    }
}

BOOLEAN
RtlpTraceDatabaseInternalFind (
    PRTL_TRACE_DATABASE Database,
    IN ULONG Count,
    IN PVOID * Trace,
    OUT PRTL_TRACE_BLOCK * TraceBlock OPTIONAL
    )
/*++

Routine Description:

    This internal routine searches for a trace in the database.

Arguments:

    Database - trace database
    
    Count - size of trace (in PVOIDs)
    
    Trace - trace
    
    TraceBlock - element where the trace is stored.

Return Value:

    TRUE if the trace was found.

Environment:

    Called from RtlTraceDatabaseFind. Assumes the database lock is held.

--*/
{
    ULONG HashValue;
    PRTL_TRACE_BLOCK Current;
    ULONG Index;

    //
    // Find the bucket to search into.
    //

    HashValue = (Database->HashFunction) (Count, Trace);
    Database->HashCounter[HashValue % 16] += 1;
    HashValue %= Database->NoOfBuckets;

    //
    // Traverse the list of blocks for the found bucket
    //

    for (Current = Database->Buckets[HashValue];
         Current != NULL;
         Current = Current->Next) {

        //
        // If the size of the trace matches we might have a chance
        // to find an equal trace.
        //

        if (Count == Current->Size) {

            //
            // Figure out if the whole trace matches.
            //

            for (Index = 0; Index < Count; Index++) {
                if (Current->Trace[Index] != Trace[Index]) {
                    break;
                }
            }

            //
            // If the trace matched completely we have found an entry.
            //

            if (Index == Count) {
                if (TraceBlock) {
                    *TraceBlock = Current;
                }

                return TRUE;
            }
        }
    }

    //
    // If we traversed the whole list for the hashed bucket and did not
    // find anything we will fail the call.
    //

    if (TraceBlock) {
        *TraceBlock = NULL;
    }
    
    return FALSE;
}

PVOID 
RtlpTraceDatabaseAllocate (
    IN SIZE_T Size,
    IN ULONG Flags, // OPTIONAL in User mode
    IN ULONG Tag    // OPTIONAL in User mode
    )
/*++

Routine Description:

    This routine allocates memory and hides all the different
    details for User vs Kernel mode allocation and paged vs
    nonpaged pool.

Arguments:

    Size -  size in bytes
    
    Flags - flags (specify U/K mode and P/NP pool)
    
    Tag - tag used for K mode allocations

Return Value:

    Pointer to memory area allocated or null.

Environment:

    Internal function for trace database module.

--*/
{
	  PVOID result; // eax@2
	  PVOID address; // [sp+0h] [bp-4h]@1

	  address = 0;
	  if ( NtAllocateVirtualMemory((HANDLE)-1, &address, 0, &Size, 12288, 4) < 0 )
		result = 0;
	  else
		result = address;
	  return result;
}

PRTL_TRACE_SEGMENT
RtlpTraceSegmentCreate (
    IN SIZE_T Size,
    IN ULONG Flags, // OPTIONAL in User mode
    IN ULONG Tag    // OPTIONAL in User mode
    )
/*++

Routine Description:

    This routine creates a new segment. The segment is the device
    through which a database can increase in size to accommodata
    more traces.

Arguments:

    Size - size in bytes
    
    Flags - allocation flags (U/K mode, P/NP pool)
    
    Tag - tag for K mode allocations

Return Value:

    New allocated segment or null.

Environment:

    Internal trace database module function.

--*/
{
    PRTL_TRACE_SEGMENT Segment;

    Segment = RtlpTraceDatabaseAllocate (Size, Flags, Tag);
    return Segment;
}

BOOLEAN
RtlpTraceDatabaseInternalAdd (
    IN PRTL_TRACE_DATABASE Database,
    IN ULONG Count,
    IN PVOID * Trace,
    OUT PRTL_TRACE_BLOCK * TraceBlock OPTIONAL
    )
/*++

Routine Description:

    This is the internal routine to add a trace. See RtlTraceDatabaseAdd
    for more details.

Arguments:

    Database - trace database
    
    Count - size of trace (in PVOIDs)
    
    Trace - trace
    
    TraceBlock - address of block where the trace is stored

Return Value:

    TRUE if trace was added.

Environment:

    Called from RtlTraceDatabaseAdd. Assumes the database lock
    is held.

--*/
{
    PRTL_TRACE_BLOCK Block;
    PRTL_TRACE_SEGMENT Segment;
    PRTL_TRACE_SEGMENT TopSegment;
    ULONG HashValue;
    SIZE_T RequestSize;

    //
    // Check if the block is already in the database (hash table).
    // If it is increase the number of hits and return.
    //

    if (RtlpTraceDatabaseInternalFind (Database, Count, Trace, &Block)) {

        Block->Count += 1;

        if (TraceBlock) {
            *TraceBlock = Block;
        }

        Database->NoOfHits += 1;
        return TRUE;
    }

    //
    //  We need to create a new block. First we need to figure out
    // if the current segment can accommodate the new block.
    // 

    RequestSize = sizeof(*Block) + Count * sizeof(PVOID);

    TopSegment = Database->SegmentList;
    if (RequestSize > (SIZE_T)(TopSegment->SegmentEnd - TopSegment->SegmentFree)) {

        //
        // If the database has a maximum size and that limit
        // has been reached then fail the call.
        //

        if (Database->MaximumSize > 0) {
            if (Database->CurrentSize > Database->MaximumSize) {
                
                if (TraceBlock) {
                    *TraceBlock = NULL;
                }

                return FALSE;
            }
        }

        //
        // Allocate a new database segment. Fail call if cannot
        // allocate.
        //

        Segment = RtlpTraceSegmentCreate (RTL_TRACE_SIZE_INCREMENT, 
                                          Database->Flags,
                                          Database->Tag);

        if (Segment == NULL) {
            
            if (TraceBlock) {
                *TraceBlock = NULL;
            }
            
            return FALSE;
        }

        //
        // Add the new segment to the database.
        //

        Segment->Magic = RTL_TRACE_SEGMENT_MAGIC;
        Segment->Database = Database;
        Segment->TotalSize = RTL_TRACE_SIZE_INCREMENT;
        Segment->SegmentStart = (PCHAR)Segment;
        Segment->SegmentEnd = Segment->SegmentStart + RTL_TRACE_SIZE_INCREMENT;
        Segment->SegmentFree = (PCHAR)(Segment + 1);

        Segment->NextSegment = Database->SegmentList;
        Database->SegmentList = Segment;
        TopSegment = Database->SegmentList;

        Database->CurrentSize += RTL_TRACE_SIZE_INCREMENT;
    }

    if (RequestSize > (SIZE_T)(TopSegment->SegmentEnd - TopSegment->SegmentFree)) {

        DbgPrint ("Trace database: failing attempt to save biiiiig trace (size %u) \n", 
                  Count);
        
        if (TraceBlock) {
            *TraceBlock = NULL;
        }

        return FALSE;
    }

    //
    // Finally we can allocate our block.
    //

    Block = (PRTL_TRACE_BLOCK)(TopSegment->SegmentFree);
    TopSegment->SegmentFree += RequestSize;

    //
    // Fill the block with the new trace.
    //

    Block->Magic = RTL_TRACE_BLOCK_MAGIC;
    Block->Size = Count;
    Block->Count = 1;
    Block->Trace = (PVOID *)(Block + 1);

    Block->UserCount = 0;
    Block->UserSize = 0;

    //
    // Copy the trace
    //

    RtlMoveMemory (Block->Trace, Trace, Count * sizeof(PVOID));

    //
    // Add the block to corresponding bucket.
    //

    HashValue = (Database->HashFunction) (Count, Trace);
    HashValue %= Database->NoOfBuckets;
    Database->HashCounter[HashValue / (Database->NoOfBuckets / 16)] += 1;

    Block->Next = Database->Buckets[HashValue];
    Database->Buckets[HashValue] = Block;

    //
    // Loooong function. Finally return success.
    //

    if (TraceBlock) {
        *TraceBlock = Block;
    }

    Database->NoOfTraces += 1;
    return TRUE;
}

BOOLEAN
NTAPI
RtlTraceDatabaseAdd (
    IN PRTL_TRACE_DATABASE Database,
    IN ULONG Count,
    IN PVOID * Trace,
    OUT PRTL_TRACE_BLOCK * TraceBlock OPTIONAL
    )
/*++

Routine Description:

    This routine adds a new stack trace to the database. If the trace
    already exists then only the `Count' field for the trace will be
    incremented.

Arguments:

    Database - trace database
    
    Count - number of pointers (PVOIDs) in the trace
    
    Trace - array of PVOIDs (the trace)
    
    TraceBlock - if not null will contain the address of the block where
        the trace was stored.

Return Value:

    TRUE if a trace was added to the database. TraceBlock will contain
    the address of the block. If the trace was already present in the
    database a block with `Count' greater than 1 will be returned.

Environment:

    Any.

--*/
{
    BOOLEAN Result;

    //
    // Sanity checks.
    //

    TRACE_ASSERT (Database && Database->Magic == RTL_TRACE_DATABASE_MAGIC);


    Result = RtlpTraceDatabaseInternalAdd (
        Database,
        Count,
        Trace,
        TraceBlock);


    return Result;
}

PRTL_TRACE_DATABASE
NTAPI
RtlTraceDatabaseCreate (
    IN ULONG Buckets,
    IN SIZE_T MaximumSize OPTIONAL,
    IN ULONG Flags, // OPTIONAL in User mode
    IN ULONG Tag,   // OPTIONAL in User mode
    IN RTL_TRACE_HASH_FUNCTION HashFunction OPTIONAL
    )
/*++

Routine Description:

    This routine creates a trace database, that is a hash table
    to store stack traces.

Arguments:

    Buckets - no of buckets of the hash table with simple chaining.
    
    MaximumSize - maximum amount of memory that the database can use.
        When limit is hit, database add operations will start to fail.
        If the value is zero then no limit will be imposed.
        
    Flags - flags to control if allocation in K mode is done in P or NP
        pool. The possible bits that can be used right now are:
        RTL_TRACE_USE_PAGED_POOL
        RTL_TRACE_USE_NONPAGED_POOL
        
    Tag - tag used for K mode allocations.
    
    HashFunction - hash function to be used. If null passed a standard hash 
        function will be provided by the module.    

Return Value:

    Pointer to an initialized trace database structure.

Environment:

    Any.

--*/
{
    PVOID RawArea;
    SIZE_T RawSize;
    PRTL_TRACE_DATABASE Database;
    PRTL_TRACE_SEGMENT Segment;
    ULONG FirstFlags;

    //
    // Prepare trace database flags. The first segment of
    // the database will be allocated in nonpaged pool
    // no matter what flags are used because it contains
    // kernel synchronization objects that need to be in
    // that pool. 
    //

    Flags |= RTL_TRACE_IN_KERNEL_MODE;
    FirstFlags = RTL_TRACE_IN_KERNEL_MODE | RTL_TRACE_USE_NONPAGED_POOL;

    //
    // Allocate first segment of trace database that will contain
    // DATABASE, SEGMENT, buckets of the hash table and later traces.
    //

    RawSize = sizeof (RTL_TRACE_DATABASE) +
        sizeof (RTL_TRACE_SEGMENT) +
        Buckets * sizeof (PRTL_TRACE_BLOCK);

    RawSize += RTL_TRACE_SIZE_INCREMENT;
    RawSize &= ~(RTL_TRACE_SIZE_INCREMENT - 1);

    RawArea = RtlpTraceDatabaseAllocate (
        RawSize, 
        FirstFlags,
        Tag);

    if (RawArea == NULL) {
        return NULL;
    }

    Database = (PRTL_TRACE_DATABASE)RawArea;
    Segment = (PRTL_TRACE_SEGMENT)(Database + 1);

    //
    // Initialize the database
    //

    Database->Magic = RTL_TRACE_DATABASE_MAGIC;
    Database->Flags = Flags;
    Database->Tag = Tag;
    Database->SegmentList = NULL;
    Database->MaximumSize = MaximumSize;
    Database->CurrentSize = RTL_TRACE_SIZE_INCREMENT;
    Database->Owner = NULL;

    Database->NoOfHits = 0;
    Database->NoOfTraces = 0;
    RtlZeroMemory (Database->HashCounter, sizeof (Database->HashCounter));

    if (! RtlpTraceDatabaseInitializeLock (Database)) {
        RtlpTraceDatabaseFree (RawArea, Tag);
        return NULL;
    }

    Database->NoOfBuckets = Buckets;      

    if (HashFunction == NULL) {
        Database->HashFunction = RtlpTraceStandardHashFunction;
    }
    else {
        Database->HashFunction = HashFunction;
    }

    //
    // Initialize first segment of the database
    //

    Segment->Magic = RTL_TRACE_SEGMENT_MAGIC;
    Segment->Database = Database;
    Segment->NextSegment = NULL;
    Segment->TotalSize = RTL_TRACE_SIZE_INCREMENT;

    Database->SegmentList = Segment;

    //
    // Initialize the buckets of the database.
    //

    Database->Buckets = (PRTL_TRACE_BLOCK *)(Segment + 1);
    RtlZeroMemory (Database->Buckets, Database->NoOfBuckets * sizeof(PRTL_TRACE_BLOCK));

    //
    // Initialize free pointer for segment
    //

    Segment->SegmentStart = (PCHAR)RawArea;
    Segment->SegmentEnd = Segment->SegmentStart + RTL_TRACE_SIZE_INCREMENT;
    Segment->SegmentFree = (PCHAR)(Segment + 1) + Database->NoOfBuckets * sizeof(PRTL_TRACE_BLOCK);

    return Database;
}

BOOLEAN
NTAPI
RtlTraceDatabaseEnumerate (
    PRTL_TRACE_DATABASE Database,
    OUT PRTL_TRACE_ENUMERATE Enumerate,
    OUT PRTL_TRACE_BLOCK * TraceBlock
    )
/*++

Routine Description:

    This function enumerates all traces in the database. It requires a
    RTL_TRACE_ENUMERATE function (zeroed initially) to keep the state of
    the enumeration. Since the trace database does not support delete
    operations we do not need to keep a lock across multiple calls to
    Enumerate(). However this can change if we add support for deletions.

Arguments:

    Database - trace database pointer
    
    Enumerate - enumeration opaque structure. Used to keep the state of 
        the enumeration.
        
    TraceBlock - on each successful return this pointer gets filled with
        the address of a trace block from the database.        

Return Value:

    TRUE if a trace block was found (during enumeration) and FALSE if there
    are no more blocks in the database.

Environment:

    User/Kernel mode.

--*/

{
    BOOLEAN Result;
    
    TRACE_ASSERT (Database != NULL);
    TRACE_ASSERT (Database->Magic == RTL_TRACE_DATABASE_MAGIC);

    RtlpTraceDatabaseAcquireLock (Database);
    
    //
    // Start the search process if this is the first call.
    // If this is not the first call try to validate what
    // we have inside the enumerator.
    //

    if (Enumerate->Database == NULL) {

        Enumerate->Database = Database;
        Enumerate->Index = 0;
        Enumerate->Block = Database->Buckets[0];
    }
    else {

        if (Enumerate->Database != Database) {
            Result = FALSE;
            goto Exit;
        }

        if (Enumerate->Index >= Database->NoOfBuckets) {
            Result = FALSE;
            goto Exit;
        }
    }

    //
    // Find out the next trace block in case we are at the end
    // of a bucket or the bucket was empty.
    //

    while (Enumerate->Block == NULL) {
        
        Enumerate->Index += 1;
        
        if (Enumerate->Index >= Database->NoOfBuckets) {
            break;
        }
        
        Enumerate->Block = Database->Buckets[Enumerate->Index];
    }
    
    //
    // Figure out if we have finished the enumeration.
    //

    if (Enumerate->Index >= Database->NoOfBuckets && Enumerate->Block == NULL) {

        *TraceBlock = NULL;
        Result = FALSE;
        goto Exit;
    }

    //
    // Fill out the next trace block and advance the enumerator.
    //

    *TraceBlock = Enumerate->Block;
    Enumerate->Block = Enumerate->Block->Next;
    Result = TRUE;

    //
    // Clean up and exit
    //

    Exit:

    RtlpTraceDatabaseReleaseLock (Database);
    return Result;
}

BOOLEAN
NTAPI
RtlTraceDatabaseFind (
    PRTL_TRACE_DATABASE Database,
    IN ULONG Count,
    IN PVOID * Trace,
    OUT PRTL_TRACE_BLOCK * TraceBlock OPTIONAL
    )
/*++

Routine Description:

    This routine searches a trace in the database. If the trace
    is found then the address of the block that stores the trace
    will be returned.

Arguments:

    Database - trace database
    
    Count - number of pointers (PVOIDs) in the trace
    
    Trace - array of PVOIDs (the trace)
    
    TraceBlock - if not null will contain the address of the block where
        the trace is stored.

Return Value:

    TRUE if the trace was found in the database. TraceBlock will contain
    the address of the block that stores the trace.

Environment:

    Any.

--*/
{
    BOOLEAN Result;

    //
    // Sanity checks.
    //

    TRACE_ASSERT (Database && Database->Magic == RTL_TRACE_DATABASE_MAGIC);

    RtlpTraceDatabaseAcquireLock (Database);

    Result = RtlpTraceDatabaseInternalFind (
        Database,
        Count,
        Trace,
        TraceBlock);

    if (Result) {
        Database->NoOfHits += 1;
    }

    RtlpTraceDatabaseReleaseLock (Database);

    return Result;
}

VOID 
NTAPI
RtlTraceDatabaseLock (
    IN PRTL_TRACE_DATABASE Database
    )
/*++

Routine Description:
    
    This routine acquires the trace database lock.
    It hides all details about the actual nature of the lock.
    
    The callers needs to acquire the database lock only if 
    a trace block will be modified (UserCount, UserSize fields).
    The lock is not needed for Add/Find/Enumerate operations.

Arguments:

    Database - trace database

Return Value:

    None.

Environment:

    Called if a trace block will be modified.

--*/
{
    RtlpTraceDatabaseAcquireLock(Database);
}

VOID 
NTAPI
RtlTraceDatabaseUnlock (
    IN PRTL_TRACE_DATABASE Database
    )
/*++

Routine Description:
    
    This routine releases the trace database lock.
    It hides all details about the actual nature of the lock.
    
    The callers needs to acquire/release the database lock only if 
    a trace block will be modified (UserCount, UserSize fields).
    The lock is not needed for Add/Find/Enumerate operations.

Arguments:

    Database - trace database

Return Value:

    None.

Environment:

    Called if a trace block will be modified.

--*/
{
    RtlpTraceDatabaseReleaseLock(Database);
}

PSTACK_TRACE_DATABASE
RtlpAcquireStackTraceDataBase( VOID )
{
    if (RtlpStackTraceDataBase != NULL) {
        if (RtlpStackTraceDataBase->DumpInProgress ||
            !(RtlpStackTraceDataBase->OkayToLockRoutine)( &RtlpStackTraceDataBase->CriticalSection )
           ) {
            return( NULL );
            }

        (RtlpStackTraceDataBase->AcquireLockRoutine)( &RtlpStackTraceDataBase->CriticalSection );
        }

    return( RtlpStackTraceDataBase );
}

VOID
RtlpReleaseStackTraceDataBase( VOID )
{
    (RtlpStackTraceDataBase->ReleaseLockRoutine)( &RtlpStackTraceDataBase->CriticalSection );
    return;
}