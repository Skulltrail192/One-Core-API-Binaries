/*++

Copyright (c) Microsoft Corporation. All rights reserved. 

You may only use this code if you agree to the terms of the Windows Research Kernel Source Code License agreement (see License.txt).
If you do not agree to the terms, do not use the code.


Module Name:

    tracedbp.h

Abstract:

    This header contains the private interfaces for the trace database 
    module (hash table to store stack traces in Kernel mode).

--*/

#ifndef _TRACEDBP_H
#define _TRACEDBP_H

PSTACK_TRACE_DATABASE RtlpStackTraceDataBase;

typedef ULONG (* RTL_TRACE_HASH_FUNCTION) (ULONG Count, PVOID * Trace);

//
// RTL_TRACE_SEGMENT
//

typedef struct _RTL_TRACE_DATABASE *PRTL_TRACE_DATABASE; 

typedef struct _RTL_TRACE_SEGMENT *PRTL_TRACE_SEGMENT;
 
typedef struct _RTL_TRACE_SEGMENT
{
     ULONG Magic;
     PRTL_TRACE_DATABASE Database;
     PRTL_TRACE_SEGMENT NextSegment;
     ULONG TotalSize;
     CHAR * SegmentStart;
     CHAR * SegmentEnd;
     CHAR * SegmentFree;
} RTL_TRACE_SEGMENT, *PRTL_TRACE_SEGMENT;
 
typedef struct _RTL_TRACE_DATABASE
{
     ULONG Magic;
     ULONG Flags;
     ULONG Tag;
     PRTL_TRACE_SEGMENT SegmentList;
     ULONG MaximumSize;
     ULONG CurrentSize;
     PVOID Owner;
     RTL_CRITICAL_SECTION Lock;
     ULONG NoOfBuckets;
     PRTL_TRACE_BLOCK * Buckets;
     RTL_TRACE_HASH_FUNCTION HashFunction;
     ULONG NoOfTraces;
     ULONG NoOfHits;
     ULONG HashCounter[16];
} RTL_TRACE_DATABASE, *PRTL_TRACE_DATABASE;

typedef struct _RTL_TRACE_ENUMERATE {

    PRTL_TRACE_DATABASE Database;
    ULONG Index;
    PRTL_TRACE_BLOCK Block;

} RTL_TRACE_ENUMERATE, * PRTL_TRACE_ENUMERATE;

PSTACK_TRACE_DATABASE
RtlpAcquireStackTraceDataBase( VOID );

VOID
RtlpReleaseStackTraceDataBase( VOID );

#endif // #ifndef _TRACEDBP_H

