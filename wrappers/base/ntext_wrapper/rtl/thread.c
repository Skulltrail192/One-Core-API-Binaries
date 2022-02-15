/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    thread.c

Abstract:

    This module implements RTL Thread Object APIs

Author:

    Skulltrail 10-November-2021

Revision History:

--*/

#define NDEBUG

#include <main.h> 

RTL_SRWLOCK RtlpFlsLock=RTL_SRWLOCK_INIT;

typedef struct _FLS_CALLBACK
{
    void                  *unknown;
    PFLS_CALLBACK_FUNCTION callback; /* ~0 if NULL callback is set, NULL if FLS index is free. */
} FLS_CALLBACK, *PFLS_CALLBACK;

typedef struct _FLS_INFO_CHUNK
{
    ULONG           count;         /* number of allocated FLS indexes in the chunk. */
    FLS_CALLBACK    callbacks[1];  /* the size is 0x10 for chunk 0 and is twice as
                                    * the previous chunk size for the rest. */
} FLS_INFO_CHUNK, *PFLS_INFO_CHUNK;

typedef struct _GLOBAL_FLS_DATA
{
    FLS_INFO_CHUNK *fls_callback_chunks[8];
    LIST_ENTRY      fls_list_head;
    ULONG           fls_high_index;
} GLOBAL_FLS_DATA, *PGLOBAL_FLS_DATA;

typedef struct _TEB_FLS_DATA
{
    LIST_ENTRY      fls_list_entry;
    void          **fls_data_chunks[8];
} TEB_FLS_DATA, *PTEB_FLS_DATA;

/***********************************************************************
 * Fibers
 ***********************************************************************/


static GLOBAL_FLS_DATA fls_data = { { NULL }, { &fls_data.fls_list_head, &fls_data.fls_list_head } };

static RTL_CRITICAL_SECTION fls_section;
static RTL_CRITICAL_SECTION_DEBUG fls_critsect_debug =
{
    0, 0, &fls_section,
    { &fls_critsect_debug.ProcessLocksList, &fls_critsect_debug.ProcessLocksList },
            0, 0, { (DWORD_PTR)(__FILE__ ": fls_section") }
};
static RTL_CRITICAL_SECTION fls_section = { &fls_critsect_debug, -1, 0, 0, 0, 0 };

#define MAX_FLS_DATA_COUNT 0xff0

static void lock_fls_data(void)
{
    RtlEnterCriticalSection( &fls_section );
}

static void unlock_fls_data(void)
{
    RtlLeaveCriticalSection( &fls_section );
}

static unsigned int fls_chunk_size( unsigned int chunk_index )
{
    return 0x10 << chunk_index;
}

static unsigned int fls_index_from_chunk_index( unsigned int chunk_index, unsigned int index )
{
    return 0x10 * ((1 << chunk_index) - 1) + index;
}

static unsigned int fls_chunk_index_from_index( unsigned int index, unsigned int *index_in_chunk )
{
    unsigned int chunk_index = 0;

    while (index >= fls_chunk_size( chunk_index ))
        index -= fls_chunk_size( chunk_index++ );

    *index_in_chunk = index;
    return chunk_index;
}

TEB_FLS_DATA *fls_alloc_data(void)
{
    TEB_FLS_DATA *fls;

    if (!(fls = RtlAllocateHeap( RtlProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*fls) )))
        return NULL;

    lock_fls_data();
    InsertTailList( &fls_data.fls_list_head, &fls->fls_list_entry );
    unlock_fls_data();

    return fls;
}


/***********************************************************************
 *              RtlFlsAlloc  (NTDLL.@)
 */
NTSTATUS WINAPI DECLSPEC_HOTPATCH RtlFlsAlloc( PFLS_CALLBACK_FUNCTION callback, ULONG *ret_index )
{
    unsigned int chunk_index, index, i;
    FLS_INFO_CHUNK *chunk;
    TEB_FLS_DATA *fls;

    if (!(fls = NtCurrentTeb()->FlsData)
            && !(NtCurrentTeb()->FlsData = fls = fls_alloc_data()))
        return STATUS_NO_MEMORY;

    lock_fls_data();
    for (i = 0; i < ARRAY_SIZE(fls_data.fls_callback_chunks); ++i)
    {
        if (!fls_data.fls_callback_chunks[i] || fls_data.fls_callback_chunks[i]->count < fls_chunk_size( i ))
            break;
    }

    if ((chunk_index = i) == ARRAY_SIZE(fls_data.fls_callback_chunks))
    {
        unlock_fls_data();
        return STATUS_NO_MEMORY;
    }

    if ((chunk = fls_data.fls_callback_chunks[chunk_index]))
    {
        for (index = 0; index < fls_chunk_size( chunk_index ); ++index)
            if (!chunk->callbacks[index].callback)
                break;
        assert( index < fls_chunk_size( chunk_index ));
    }
    else
    {
        fls_data.fls_callback_chunks[chunk_index] = chunk = RtlAllocateHeap( RtlProcessHeap(), HEAP_ZERO_MEMORY,
                offsetof(FLS_INFO_CHUNK, callbacks) + sizeof(*chunk->callbacks) * fls_chunk_size( chunk_index ));
        if (!chunk)
        {
            unlock_fls_data();
            return STATUS_NO_MEMORY;
        }

        if (chunk_index)
        {
            index = 0;
        }
        else
        {
            chunk->count = 1; /* FLS index 0 is prohibited. */
            chunk->callbacks[0].callback = (void *)~(ULONG_PTR)0;
            index = 1;
        }
    }

    ++chunk->count;
    chunk->callbacks[index].callback = callback ? callback : (PFLS_CALLBACK_FUNCTION)~(ULONG_PTR)0;

    if ((*ret_index = fls_index_from_chunk_index( chunk_index, index )) > fls_data.fls_high_index)
        fls_data.fls_high_index = *ret_index;

    unlock_fls_data();

    return STATUS_SUCCESS;
}


/***********************************************************************
 *              RtlFlsFree   (NTDLL.@)
 */
NTSTATUS WINAPI DECLSPEC_HOTPATCH RtlFlsFree( ULONG index )
{
    PFLS_CALLBACK_FUNCTION callback;
    unsigned int chunk_index, idx;
    FLS_INFO_CHUNK *chunk;
    LIST_ENTRY *entry;

    lock_fls_data();

    if (!index || index > fls_data.fls_high_index)
    {
        unlock_fls_data();
        return STATUS_INVALID_PARAMETER;
    }

    chunk_index = fls_chunk_index_from_index( index, &idx );
    if (!(chunk = fls_data.fls_callback_chunks[chunk_index])
            || !(callback = chunk->callbacks[idx].callback))
    {
        unlock_fls_data();
        return STATUS_INVALID_PARAMETER;
    }

    for (entry = fls_data.fls_list_head.Flink; entry != &fls_data.fls_list_head; entry = entry->Flink)
    {
        TEB_FLS_DATA *fls = CONTAINING_RECORD(entry, TEB_FLS_DATA, fls_list_entry);

        if (fls->fls_data_chunks[chunk_index] && fls->fls_data_chunks[chunk_index][idx + 1])
        {
            if (callback != (void *)~(ULONG_PTR)0)
            {
                DbgPrint("Calling FLS callback %p, arg %p.\n", callback,
                        fls->fls_data_chunks[chunk_index][idx + 1]);

                callback( fls->fls_data_chunks[chunk_index][idx + 1] );
            }
            fls->fls_data_chunks[chunk_index][idx + 1] = NULL;
        }
    }

    --chunk->count;
    chunk->callbacks[idx].callback = NULL;

    unlock_fls_data();
    return STATUS_SUCCESS;
}


/***********************************************************************
 *              RtlFlsSetValue (NTDLL.@)
 */
NTSTATUS WINAPI DECLSPEC_HOTPATCH RtlFlsSetValue( ULONG index, void *data )
{
    unsigned int chunk_index, idx;
    TEB_FLS_DATA *fls;

    if (!index || index >= MAX_FLS_DATA_COUNT)
        return STATUS_INVALID_PARAMETER;

    if (!(fls = NtCurrentTeb()->FlsData)
            && !(NtCurrentTeb()->FlsData = fls = fls_alloc_data()))
        return STATUS_NO_MEMORY;

    chunk_index = fls_chunk_index_from_index( index, &idx );

    if (!fls->fls_data_chunks[chunk_index] &&
            !(fls->fls_data_chunks[chunk_index] = RtlAllocateHeap( RtlProcessHeap(), HEAP_ZERO_MEMORY,
            (fls_chunk_size( chunk_index ) + 1) * sizeof(*fls->fls_data_chunks[chunk_index]) )))
        return STATUS_NO_MEMORY;

    fls->fls_data_chunks[chunk_index][idx + 1] = data;

    return STATUS_SUCCESS;
}


/***********************************************************************
 *              RtlFlsGetValue (NTDLL.@)
 */
NTSTATUS WINAPI DECLSPEC_HOTPATCH RtlFlsGetValue( ULONG index, void **data )
{
    unsigned int chunk_index, idx;
    TEB_FLS_DATA *fls;

    if (!index || index >= MAX_FLS_DATA_COUNT || !(fls = NtCurrentTeb()->FlsData))
        return STATUS_INVALID_PARAMETER;

    chunk_index = fls_chunk_index_from_index( index, &idx );

    *data = fls->fls_data_chunks[chunk_index] ? fls->fls_data_chunks[chunk_index][idx + 1] : NULL;
    return STATUS_SUCCESS;
}

/***********************************************************************
 *              RtlProcessFlsData (NTDLL.@)
 */
void WINAPI DECLSPEC_HOTPATCH RtlProcessFlsData( void *teb_fls_data, ULONG flags )
{
    TEB_FLS_DATA *fls = teb_fls_data;
    unsigned int i, index;

    DbgPrint( "teb_fls_data %p, flags %#x.\n", teb_fls_data, flags );

    if (flags & ~3)
        DbgPrint( "RtlProcessFlsData::Unknown flags %#x.\n", flags );

    if (!fls)
        return;

    if (flags & 1)
    {
        lock_fls_data();
        for (i = 0; i < ARRAY_SIZE(fls->fls_data_chunks); ++i)
        {
            if (!fls->fls_data_chunks[i] || !fls_data.fls_callback_chunks[i]
                    || !fls_data.fls_callback_chunks[i]->count)
                continue;

            for (index = 0; index < fls_chunk_size( i ); ++index)
            {
                PFLS_CALLBACK_FUNCTION callback = fls_data.fls_callback_chunks[i]->callbacks[index].callback;

                if (!fls->fls_data_chunks[i][index + 1])
                    continue;

                if (callback && callback != (void *)~(ULONG_PTR)0)
                {
                    DbgPrint("RtlProcessFlsData::Calling FLS callback %p, arg %p.\n", callback,
                            fls->fls_data_chunks[i][index + 1]);

                    callback( fls->fls_data_chunks[i][index + 1] );
                }
                fls->fls_data_chunks[i][index + 1] = NULL;
            }
        }
        /* Not using RemoveEntryList() as Windows does not zero list entry here. */
        fls->fls_list_entry.Flink->Blink = fls->fls_list_entry.Blink;
        fls->fls_list_entry.Blink->Flink = fls->fls_list_entry.Flink;
        unlock_fls_data();
    }

    if (flags & 2)
    {
        for (i = 0; i < ARRAY_SIZE(fls->fls_data_chunks); ++i)
            RtlFreeHeap( RtlProcessHeap(), 0, fls->fls_data_chunks[i] );

        RtlFreeHeap( RtlProcessHeap(), 0, fls );
    }
}