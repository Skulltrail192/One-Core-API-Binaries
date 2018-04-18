/*
 * Copyright 2018 Andrew Comminos
 *
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

#include "config.h"
#include "wine/port.h"
#include "wine/rbtree.h"
#include "wined3d_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(d3d);
WINE_DECLARE_DEBUG_CHANNEL(d3d_perf);

// Arbitrary binding to use when binding the persistent buffer.
#define BIND_TARGET GL_ARRAY_BUFFER

struct wined3d_buffer_heap_fenced_element
{
    struct wined3d_buffer_heap_bin_set free_list;
    struct wined3d_fence *fence;

    struct wined3d_buffer_heap_fenced_element *next;
};

static struct wined3d_buffer_heap_element* element_new(GLsizei offset, GLsizei size)
{
    struct wined3d_buffer_heap_element* elem;
    elem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct wined3d_buffer_heap_element));
    if (!elem)
        return NULL;
    elem->range.offset = offset;
    elem->range.size = size;
    return elem;
}

static inline int bitwise_log2_floor(GLsizei size)
{
    // XXX(acomminos): I hope this gets unrolled.
    for (int i = 8 * sizeof(GLsizei) - 1; i >= 0; i--)
    {
        if ((size >> i) & 1) {
            return i;
        }
    }
    return 0;
}

static inline int bitwise_log2_ceil(GLsizei size)
{
    // Add one to the floor of size if size isn't a power of two.
    return bitwise_log2_floor(size) + !!(size & (size - 1));
}

static int element_bin(struct wined3d_buffer_heap_element *elem)
{
    return min(WINED3D_BUFFER_HEAP_BINS - 1, bitwise_log2_floor(elem->range.size));
}

// Inserts an element into the appropriate free list bin.
static void element_insert_free_bin(struct wined3d_buffer_heap *heap, struct wined3d_buffer_heap_element *elem)
{
    if (elem->prev || elem->next)
    {
        ERR("Element %p in already in a free list (for some reason).\n", elem);
    }

    int bin = element_bin(elem);

    elem->prev = NULL;
    elem->next = heap->free_list.bins[bin].head;
    if (heap->free_list.bins[bin].head)
        heap->free_list.bins[bin].head->prev = elem;
    heap->free_list.bins[bin].head = elem;

    if (!heap->free_list.bins[bin].tail)
        heap->free_list.bins[bin].tail = elem;

    TRACE("Inserted allocation at %p of size %lld into bin %d\n", elem->range.offset, elem->range.size, bin);
}

// Removes an element from the free tree, its bin, and the coalesce list.
static void element_remove_free(struct wined3d_buffer_heap *heap, struct wined3d_buffer_heap_element *elem)
{
    int bin = element_bin(elem);

    if (elem->prev)
        elem->prev->next = elem->next;

    if (elem->next)
        elem->next->prev = elem->prev;

    if (elem == heap->free_list.bins[bin].head)
        heap->free_list.bins[bin].head = elem->next;

    if (elem == heap->free_list.bins[bin].tail)
        heap->free_list.bins[bin].tail = elem->prev;

    elem->prev = NULL;
    elem->next = NULL;

    TRACE("Freed allocation at %p of size %lld from bin %d\n", elem->range.offset, elem->range.size, bin);
}

static struct wined3d_buffer_heap_fenced_element* fenced_element_new(struct wined3d_buffer_heap_bin_set bins, struct wined3d_fence* fence)
{
    struct wined3d_buffer_heap_fenced_element* elem;
    elem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct wined3d_buffer_heap_fenced_element));
    if (!elem)
        return NULL;
    elem->free_list = bins;
    elem->fence = fence;
    elem->next = NULL;
    return elem;
}

static int free_tree_compare(const void *key, const struct wine_rb_entry *entry)
{
    const GLsizei offset = *(const GLsizei*) key;
    struct wined3d_buffer_heap_element *elem = WINE_RB_ENTRY_VALUE(entry, struct wined3d_buffer_heap_element, entry);

    if (offset < elem->range.offset)
        return -1;
    if (offset > elem->range.offset)
        return 1;
    return 0;
}

/* Context activation is done by the caller. */
HRESULT wined3d_buffer_heap_create(struct wined3d_context *context, GLsizeiptr size, GLsizeiptr alignment, BOOL write_only, struct wined3d_buffer_heap **buffer_heap)
{
    const struct wined3d_gl_info *gl_info = context->gl_info;
    GLbitfield access_flags;
    GLbitfield storage_flags;
    struct wined3d_buffer_heap_element *initial_elem;

    struct wined3d_buffer_heap *object;

    if ((alignment & (alignment - 1)) != 0)
    {
        return E_FAIL;
    }

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
    {
        return E_OUTOFMEMORY;
    }

    access_flags = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT;
    if (!write_only)
    {
        access_flags |= GL_MAP_READ_BIT;
    }

    storage_flags = access_flags;
    // FIXME(acomminos): So, about GL_CLIENT_STORAGE_BIT:
    // - On NVIDIA, DMA CACHED memory is used when this flag is set. SYSTEM HEAP
    //   memory is used without it, which (in my testing) is much faster.
    // - On Mesa, GTT is used when this flag is set. This is what we want- we
    //   upload to VRAM occur otherwise, which is unusably slow (on radeon).
    //
    // Thus, we're only going to set this on mesa for now.
    // Hints are awful anyway.
    if (gl_info->quirks & WINED3D_QUIRK_USE_CLIENT_STORAGE_BIT)
    {
        FIXME_(d3d_perf)("PBA: using GL_CLIENT_STORAGE_BIT quirk");
        storage_flags |= GL_CLIENT_STORAGE_BIT;
    }

    GL_EXTCALL(glGenBuffers(1, &object->buffer_object));
    checkGLcall("glGenBuffers");

    context_bind_bo(context, BIND_TARGET, object->buffer_object);

    GL_EXTCALL(glBufferStorage(BIND_TARGET, size, NULL, storage_flags));
    checkGLcall("glBufferStorage");

    if (!(object->map_ptr = GL_EXTCALL(glMapBufferRange(BIND_TARGET, 0, size, access_flags))))
    {
        ERR("Couldn't map persistent buffer.\n");
        return -1; // FIXME(acomminos): proper error code, cleanup
    }
    context_bind_bo(context, BIND_TARGET, 0);

    object->fenced_head = object->fenced_tail = NULL;
    object->alignment = alignment;
    InitializeCriticalSection(&object->temp_lock);

    initial_elem = element_new(0, size);
    // Don't bother adding the initial allocation to the coalescing tree.
    element_insert_free_bin(object, initial_elem);

    *buffer_heap = object;

    return WINED3D_OK;
}

/* Context activation is done by the caller. */
HRESULT wined3d_buffer_heap_destroy(struct wined3d_buffer_heap *heap, struct wined3d_context *context)
{
    const struct wined3d_gl_info *gl_info = context->gl_info;

    context_bind_bo(context, BIND_TARGET, heap->buffer_object);
    GL_EXTCALL(glUnmapBuffer(BIND_TARGET));
    checkGLcall("glUnmapBuffer");
    context_bind_bo(context, BIND_TARGET, 0);

    GL_EXTCALL(glDeleteBuffers(1, &heap->buffer_object));
    checkGLcall("glDeleteBuffers");

    DeleteCriticalSection(&heap->temp_lock);

    // TODO(acomminos): cleanup free lists, fenced list, etc.

    HeapFree(GetProcessHeap(), 0, heap);

    return WINED3D_OK;
}

HRESULT wined3d_buffer_heap_alloc(struct wined3d_buffer_heap *heap, GLsizeiptr size, struct wined3d_buffer_heap_element **out_elem)
{
    int initial_bin;
    int initial_size = size;

    EnterCriticalSection(&heap->temp_lock);

    // After alignment, reduce fragmentation by rounding to next power of two.
    // If the alignment is a power of two (which it should be), this should be
    // no problem.
    size = 1 << bitwise_log2_ceil(size);

    // Align size values where possible.
    if (heap->alignment && (size % heap->alignment != 0))
        size += heap->alignment - (size % heap->alignment);

    initial_bin = min(WINED3D_BUFFER_HEAP_BINS - 1, bitwise_log2_ceil(size));

    for (int i = initial_bin; i < WINED3D_BUFFER_HEAP_BINS; i++)
    {
        struct wined3d_buffer_heap_element *elem = heap->free_list.bins[i].head;
        if (elem)
        {
            struct wined3d_map_range remaining_range;
            remaining_range.offset = elem->range.offset + size;
            remaining_range.size = elem->range.size - size;

            // Take the element from the free list, transferring ownership to
            // the caller.
            element_remove_free(heap, elem);
            // Resize the element so that we can free the remainder.
            elem->range.size = size;

            *out_elem = elem;

            TRACE_(d3d_perf)("Allocated %d (requested %d) at %p from bin %d (initial %d)\n", size, initial_size, elem->range.offset, i, initial_bin);

            if (remaining_range.size > 0)
            {
                struct wined3d_buffer_heap_element *remaining_elem;

                TRACE_(d3d_perf)("Imperfect fit allocated, fragmenting remainder of %lld at %p.\n", remaining_range.size, remaining_range.offset);

                remaining_elem = element_new(remaining_range.offset, remaining_range.size);
                element_insert_free_bin(heap, remaining_elem);
            }

            LeaveCriticalSection(&heap->temp_lock);
            return WINED3D_OK;
        }
    }

    LeaveCriticalSection(&heap->temp_lock);

    FIXME_(d3d_perf)("Forcing coalesce, not enough free space in buffer heap.\n");
    int num_coalesced;
    if (SUCCEEDED(wined3d_buffer_heap_deferred_coalesce(heap, &num_coalesced)))
    {
        if (num_coalesced > 0)
            return wined3d_buffer_heap_alloc(heap, size, out_elem);
    }

    FIXME_(d3d_perf)("Coalescing did not create new blocks, failing.\n");

    return WINED3DERR_OUTOFVIDEOMEMORY;
}

HRESULT wined3d_buffer_heap_free(struct wined3d_buffer_heap *heap, struct wined3d_buffer_heap_element *elem)
{
    EnterCriticalSection(&heap->temp_lock);

    // Only insert the element into a free bin, coalescing will occur later.
    //
    // Note that the reason that we pass around wined3d_buffer_heap_element
    // instead of a range is to avoid frequent HeapAlloc/HeapFree operations
    // when we're reusing buffers.
    element_insert_free_bin(heap, elem);

    LeaveCriticalSection(&heap->temp_lock);

    return WINED3D_OK;
}

HRESULT wined3d_buffer_heap_free_fenced(struct wined3d_buffer_heap *heap, struct wined3d_device *device, struct wined3d_buffer_heap_element *elem)
{
    int bin_index = element_bin(elem);
    struct wined3d_buffer_heap_bin *bin = &heap->pending_fenced_bins.bins[bin_index];

    if (bin->tail)
    {
        bin->tail->next = elem;
        elem->prev = bin->tail;
        bin->tail = elem;
    }
    else
    {
        bin->head = elem;
        bin->tail = elem;
    }

    return WINED3D_OK;
}

HRESULT wined3d_buffer_heap_cs_fence_issue(struct wined3d_buffer_heap *heap, struct wined3d_device *device)
{
    struct wined3d_buffer_heap_fenced_element *fenced_elem;
    struct wined3d_fence *fence;
    HRESULT hr;

    if (heap->fenced_head)
    {
        // XXX(acomminos): double or triple buffer this?
        wined3d_buffer_heap_cs_fence_wait(heap, device);
    }

    if (FAILED(hr = wined3d_fence_create(device, &fence)))
    {
        ERR("Failed to create fence.\n");
        return hr;
    }

    fenced_elem = fenced_element_new(heap->pending_fenced_bins, fence);
    if (!fenced_elem)
        return E_OUTOFMEMORY;

    TRACE_(d3d_perf)("Dispatching fenced buffer set.\n");
    memset(&heap->pending_fenced_bins, 0, sizeof(heap->pending_fenced_bins));

    // Append to end of fenced list, which works well if you assume that buffers
    // are freed in some ascending draw call ordering.
    if (!heap->fenced_head)
    {
        heap->fenced_head = fenced_elem;
        heap->fenced_tail = fenced_elem;
    }
    else
    {
        heap->fenced_tail->next = fenced_elem;
        heap->fenced_tail = fenced_elem;
    }

    wined3d_fence_issue(fence, device);
    return WINED3D_OK;
}

HRESULT wined3d_buffer_heap_cs_fence_wait(struct wined3d_buffer_heap *heap, struct wined3d_device *device)
{
    enum wined3d_fence_result res;
    struct wined3d_buffer_heap_fenced_element *elem = heap->fenced_head;
    if (!elem)
        return WINED3D_OK;

    res = wined3d_fence_wait(elem->fence, device);
    switch (res)
    {
        case WINED3D_FENCE_OK:
        case WINED3D_FENCE_NOT_STARTED:
            {
                TRACE_(d3d_perf)("Freed fence group.\n");

                EnterCriticalSection(&heap->temp_lock);
                for (int i = 0; i < WINED3D_BUFFER_HEAP_BINS; i++)
                {
                    struct wined3d_buffer_heap_bin *elem_bin = &elem->free_list.bins[i];
                    if (!elem_bin->tail)
                        continue;

                    struct wined3d_buffer_heap_bin *heap_bin = &heap->free_list.bins[i];
                    if (heap_bin->head)
                    {
                        // Insert to front.
                        elem_bin->tail->next = heap_bin->head;
                        heap_bin->head->prev = elem_bin->tail;

                        elem_bin->head->prev = NULL;
                        heap_bin->head = elem_bin->head;
                    }
                    else
                    {
                        elem_bin->head->prev = NULL;
                        heap_bin->head = elem_bin->head;
                        elem_bin->tail->next = NULL;
                        heap_bin->tail = elem_bin->tail;
                    }
                }
                LeaveCriticalSection(&heap->temp_lock);

                wined3d_fence_destroy(elem->fence);

                heap->fenced_head = elem->next;
                HeapFree(GetProcessHeap(), 0, elem);
                // TODO(acomminos): bother to null out fenced_tail?
                break;
            }
        default:
            return WINED3D_OK;
    }

    return WINED3D_OK;
}

HRESULT wined3d_buffer_heap_deferred_coalesce(struct wined3d_buffer_heap *heap, int *coalesced_count)
{
    struct wined3d_buffer_heap_element *elem = NULL;
    struct wined3d_buffer_heap_element *next = NULL;
    struct wine_rb_entry *entry;
    struct wined3d_map_range coalesced_range;

    struct wine_rb_tree free_tree;
    int num_coalesced = 0;

    wine_rb_init(&free_tree, free_tree_compare);

    EnterCriticalSection(&heap->temp_lock);

    // TODO(acomminos): on one hand, if there's a lot of elements in the list,
    // it's highly fragmented. on the other, we can potentially waste a decent
    // sum of time checking for uncoalesced bins.
    for (int i = 0; i < WINED3D_BUFFER_HEAP_BINS; i++)
    {
        elem = heap->free_list.bins[i].head;
        while (elem)
        {
            // Insert a sentry. FIXME(acomminos): can skip this with traversal.
            if (wine_rb_put(&free_tree, &elem->range.offset, &elem->entry) == -1)
            {
                ERR("Failed to insert key %x in tree.\n", elem->range.offset);
                elem = elem->next;
                continue;
            }

            coalesced_range = elem->range;

            // Coalesce right.
            entry = wine_rb_next(&elem->entry);
            if (entry)
            {
                TRACE("Coalesced right.\n");
                struct wined3d_buffer_heap_element *right_elem = WINE_RB_ENTRY_VALUE(entry, struct wined3d_buffer_heap_element, entry);
                if (elem->range.offset + elem->range.size == right_elem->range.offset)
                {
                    coalesced_range.size += right_elem->range.size;

                    wine_rb_remove(&free_tree, entry);
                    element_remove_free(heap, right_elem);
                    HeapFree(GetProcessHeap(), 0, right_elem);

                    num_coalesced++;
                }
            }

            // Coalesce left.
            entry = wine_rb_prev(&elem->entry);
            if (entry)
            {
                TRACE("Coalesced left.\n");
                struct wined3d_buffer_heap_element *left_elem = WINE_RB_ENTRY_VALUE(entry, struct wined3d_buffer_heap_element, entry);
                if (left_elem->range.offset + left_elem->range.size == coalesced_range.offset)
                {
                    coalesced_range.offset = left_elem->range.offset;
                    coalesced_range.size += left_elem->range.size;

                    wine_rb_remove(&free_tree, entry);
                    element_remove_free(heap, left_elem);
                    HeapFree(GetProcessHeap(), 0, left_elem);

                    num_coalesced++;
                }
            }

            next = elem->next;

            if (elem->range.size != coalesced_range.size)
            {
                FIXME_(d3d_perf)("Coalesced range from (%p, %ld) to (%p, %ld)\n", elem->range.offset, elem->range.size, coalesced_range.offset, coalesced_range.size);

                wine_rb_remove(&free_tree, &elem->entry);

                // Move to the correct free bin.
                element_remove_free(heap, elem);
                elem->range = coalesced_range;
                element_insert_free_bin(heap, elem);

                wine_rb_put(&free_tree, &elem->range.offset, &elem->entry);
            }

            elem = next;
        }
    }

    LeaveCriticalSection(&heap->temp_lock);

    FIXME_(d3d_perf)("Performed %d coalesces.\n", num_coalesced);
    if (coalesced_count)
        *coalesced_count = num_coalesced;

    return WINED3D_OK;
}
