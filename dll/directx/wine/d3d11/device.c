/*
 * Copyright 2008-2012 Henri Verbeet for CodeWeavers
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
#include "wine/list.h"

#define NONAMELESSUNION
#include "d3d11_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(d3d11);

enum deferred_cmd
{
    DEFERRED_IASETVERTEXBUFFERS,        /* vbuffer_info */
    DEFERRED_IASETPRIMITIVETOPOLOGY,    /* topology_info */
    DEFERRED_IASETINDEXBUFFER,          /* index_buffer_info */
    DEFERRED_IASETINPUTLAYOUT,          /* input_layout_info */

    DEFERRED_RSSETSTATE,                /* rstate_info */
    DEFERRED_RSSETVIEWPORTS,            /* viewport_info */
    DEFERRED_RSSETSCISSORRECTS,         /* rs_set_scissor_rects_info */

    DEFERRED_OMSETDEPTHSTENCILSTATE,    /* stencil_state_info */
    DEFERRED_OMSETBLENDSTATE,           /* blend_state_info */
    DEFERRED_OMSETRENDERTARGETS,        /* render_target_info */
    DEFERRED_OMSETRENDERTARGETSANDUNORDEREDACCESVIEWS,  /* render_targets_and_unordered_access_views_info */

    DEFERRED_COPYRESOURCE,              /* copy_resource_info */
    DEFERRED_SETRESOURCEMINLOD,         /* set_resource_min_lod_info */
    DEFERRED_COPYSUBRESOURCEREGION,     /* copy_subresource_region_info */
    DEFERRED_UPDATESUBRESOURCE,         /* update_subresource_info */
    DEFERRED_RESOLVESUBRESOURCE,        /* resolve_subresource_info */
    DEFERRED_COPYSTRUCTURECOUNT,        /* copy_structure_count_info */

    DEFERRED_CSSETSHADER,               /* cs_info */
    DEFERRED_DSSETSHADER,               /* ds_info */
    DEFERRED_GSSETSHADER,               /* gs_info */
    DEFERRED_HSSETSHADER,               /* hs_info */
    DEFERRED_PSSETSHADER,               /* ps_info */
    DEFERRED_VSSETSHADER,               /* vs_info */

    DEFERRED_CSSETSHADERRESOURCES,      /* res_info */
    DEFERRED_DSSETSHADERRESOURCES,      /* res_info */
    DEFERRED_GSSETSHADERRESOURCES,      /* res_info */
    DEFERRED_HSSETSHADERRESOURCES,      /* res_info */
    DEFERRED_PSSETSHADERRESOURCES,      /* res_info */
    DEFERRED_VSSETSHADERRESOURCES,      /* res_info */

    DEFERRED_CSSETSAMPLERS,             /* samplers_info */
    DEFERRED_DSSETSAMPLERS,             /* samplers_info */
    DEFERRED_GSSETSAMPLERS,             /* samplers_info */
    DEFERRED_HSSETSAMPLERS,             /* samplers_info */
    DEFERRED_PSSETSAMPLERS,             /* samplers_info */
    DEFERRED_VSSETSAMPLERS,             /* samplers_info */

    DEFERRED_CSSETCONSTANTBUFFERS,      /* constant_buffers_info */
    DEFERRED_DSSETCONSTANTBUFFERS,      /* constant_buffers_info */
    DEFERRED_GSSETCONSTANTBUFFERS,      /* constant_buffers_info */
    DEFERRED_HSSETCONSTANTBUFFERS,      /* constant_buffers_info */
    DEFERRED_PSSETCONSTANTBUFFERS,      /* constant_buffers_info */
    DEFERRED_VSSETCONSTANTBUFFERS,      /* constant_buffers_info */

    DEFERRED_CSSETUNORDEREDACCESSVIEWS, /* unordered_view */
    DEFERRED_SOSETTARGETS,              /* so_set_targets_info */
    DEFERRED_GENERATEMIPS,              /* generate_mips_info */

    DEFERRED_DRAW,                      /* draw_info */
    DEFERRED_DRAWINDEXED,               /* draw_indexed_info */
    DEFERRED_DRAWINDEXEDINSTANCED,      /* draw_indexed_inst_info */
    DEFERRED_DRAWAUTO,
    DEFERRED_DRAWINSTANCED,                 /* draw_instanced_info */
    DEFERRED_DRAWINSTANCEDINDIRECT,         /* draw_instanced_indirect_info */
    DEFERRED_DRAWINDEXEDINSTANCEDINDIRECT,  /* draw_instanced_indirect_info */

    DEFERRED_MAP,                       /* map_info */
    DEFERRED_DISPATCH,                  /* dispatch_info */
    DEFERRED_DISPATCHINDIRECT,          /* dispatch_indirect_info */
    DEFERRED_SETPREDICATION,            /* set_predication_info */

    DEFERRED_CLEARSTATE,
    DEFERRED_CLEARRENDERTARGETVIEW,     /* clear_rtv_info */
    DEFERRED_CLEARDEPTHSTENCILVIEW,     /* clear_depth_info */
    DEFERRED_CLEARUNORDEREDACCESSVIEWUINT,  /* clear_unordered_access_view_uint */
    DEFERRED_CLEARUNORDEREDACCESSVIEWFLOAT, /* clear_unordered_access_view_float */

    DEFERRED_BEGIN,                     /* async_info */
    DEFERRED_END,                       /* async_info */
};

struct deferred_call
{
    struct list entry;
    enum deferred_cmd cmd;
    union
    {
        struct
        {
            UINT start_slot;
            UINT num_buffers;
            ID3D11Buffer **buffers;
            UINT *strides;
            UINT *offsets;
        } vbuffer_info;
        struct
        {
            D3D11_PRIMITIVE_TOPOLOGY topology;
        } topology_info;
        struct
        {
            ID3D11Buffer *buffer;
            DXGI_FORMAT format;
            UINT offset;
        } index_buffer_info;
        struct
        {
            ID3D11InputLayout *layout;
        } input_layout_info;
        struct
        {
            ID3D11RasterizerState *state;
        } rstate_info;
        struct
        {
            UINT rect_count;
            D3D11_RECT *rects;
        } rs_set_scissor_rects_info;
        struct
        {
            UINT num_viewports;
            D3D11_VIEWPORT *viewports;
        } viewport_info;
        struct
        {
            ID3D11DepthStencilState *state;
            UINT stencil_ref;
        } stencil_state_info;
        struct
        {
            ID3D11BlendState *state;
            FLOAT factor[4];
            UINT mask;
        } blend_state_info;
        struct
        {
            UINT num_views;
            ID3D11RenderTargetView **render_targets;
            ID3D11DepthStencilView *depth_stencil;
        } render_target_info;
        struct
        {
            ID3D11Resource *dst_resource;
            ID3D11Resource *src_resource;
        } copy_resource_info;
        struct
        {
            ID3D11Resource *resource;
            FLOAT min_lod;
        } set_resource_min_lod_info;
        struct
        {
            ID3D11Resource *dst_resource;
            UINT dst_subresource_idx;
            UINT dst_x;
            UINT dst_y;
            UINT dst_z;
            ID3D11Resource *src_resource;
            UINT src_subresource_idx;
            D3D11_BOX *src_box;
        } copy_subresource_region_info;
        struct
        {
            ID3D11Resource *resource;
            UINT subresource_idx;
            D3D11_BOX *box;
            void *data;
            UINT row_pitch;
            UINT depth_pitch;
        } update_subresource_info;
        struct
        {
            ID3D11Resource *dst_resource;
            UINT dst_subresource_idx;
            ID3D11Resource *src_resource;
            UINT src_subresource_idx;
            DXGI_FORMAT format;
        } resolve_subresource_info;
        struct
        {
            ID3D11Buffer *dst_buffer;
            UINT dst_offset;
            ID3D11UnorderedAccessView *src_view;
        } copy_structure_count_info;
        struct
        {
            UINT render_target_view_count;
            ID3D11RenderTargetView **render_target_views;
            ID3D11DepthStencilView *depth_stencil_view;
            UINT unordered_access_view_start_slot;
            UINT unordered_access_view_count;
            ID3D11UnorderedAccessView **unordered_access_views;
            UINT *initial_counts;
        } render_targets_and_unordered_access_views_info;
        struct
        {
            UINT buffer_count;
            ID3D11Buffer **buffers;
            UINT *offsets;
        } so_set_targets_info;
        struct
        {
            ID3D11ComputeShader *shader;
            /* FIXME: add class instances */
        } cs_info;
        struct
        {
            ID3D11DomainShader *shader;
            /* FIXME: add class instances */
        } ds_info;
        struct
        {
            ID3D11GeometryShader *shader;
            /* FIXME: add class instances */
        } gs_info;
        struct
        {
            ID3D11HullShader *shader;
            /* FIXME: add class instances */
        } hs_info;
        struct
        {
            ID3D11PixelShader *shader;
            /* FIXME: add class instances */
        } ps_info;
        struct
        {
            ID3D11VertexShader *shader;
            /* FIXME: add class instances */
        } vs_info;
        struct
        {
            UINT start_slot;
            UINT num_views;
            ID3D11ShaderResourceView **views;
        } res_info;
        struct
        {
            UINT start_slot;
            UINT num_samplers;
            ID3D11SamplerState **samplers;
        } samplers_info;
        struct
        {
            UINT start_slot;
            UINT num_buffers;
            ID3D11Buffer **buffers;
        } constant_buffers_info;
        struct
        {
            UINT start_slot;
            UINT num_views;
            ID3D11UnorderedAccessView **views;
            UINT *initial_counts;
        } unordered_view;
        struct
        {
            UINT count;
            UINT start;
        } draw_info;
        struct
        {
            UINT count;
            UINT start_index;
            INT  base_vertex;
        } draw_indexed_info;
        struct
        {
            UINT count_per_instance;
            UINT instance_count;
            UINT start_index;
            INT  base_vertex;
            UINT start_instance;
        } draw_indexed_inst_info;
        struct
        {
            UINT instance_vertex_count;
            UINT instance_count;
            UINT start_vertex_location;
            UINT start_instance_location;
        } draw_instanced_info;
        struct
        {
            ID3D11Buffer *buffer;
            UINT offset;
        } draw_instanced_indirect_info;
        struct
        {
            ID3D11Resource *resource;
            UINT subresource_idx;
            D3D11_MAP map_type;
            UINT map_flags;
            void *buffer;
            UINT size;
        } map_info;
        struct
        {
            UINT count_x;
            UINT count_y;
            UINT count_z;
        } dispatch_info;
        struct
        {
            ID3D11Buffer *buffer;
            UINT offset;
        } dispatch_indirect_info;
        struct
        {
            ID3D11Predicate *predicate;
            BOOL value;
        } set_predication_info;
        struct
        {
            ID3D11ShaderResourceView *view;
        } generate_mips_info;
        struct
        {
            ID3D11RenderTargetView *rtv;
            float color[4];
        } clear_rtv_info;
        struct
        {
            ID3D11DepthStencilView *view;
            UINT flags;
            FLOAT depth;
            UINT8 stencil;
        } clear_depth_info;
        struct
        {
            ID3D11UnorderedAccessView *unordered_access_view;
            UINT values[4];
        } clear_unordered_access_view_uint;
        struct
        {
            ID3D11UnorderedAccessView *unordered_access_view;
            float values[4];
        } clear_unordered_access_view_float;
        struct
        {
            ID3D11Asynchronous *asynchronous;
        } async_info;
    };
};

/* Replace with D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT as soon as wined3d supports >= 32 streams */
#define MAX_WINED3D_STREAMS 16

struct d3d11_state
{
    ID3D11ShaderResourceView *vs_resources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
    ID3D11SamplerState *vs_samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
    ID3D11Buffer *vs_cbs[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
    ID3D11VertexShader *vs;

    ID3D11ShaderResourceView *gs_resources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
    ID3D11SamplerState *gs_samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
    ID3D11Buffer *gs_cbs[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
    ID3D11GeometryShader *gs;

    ID3D11ShaderResourceView *ps_resources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
    ID3D11SamplerState *ps_samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
    ID3D11Buffer *ps_cbs[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
    ID3D11PixelShader *ps;

    ID3D11ShaderResourceView *hs_resources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
    ID3D11SamplerState *hs_samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
    ID3D11Buffer *hs_cbs[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
    ID3D11HullShader *hs;

    ID3D11ShaderResourceView *ds_resources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
    ID3D11SamplerState *ds_samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
    ID3D11Buffer *ds_cbs[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
    ID3D11DomainShader *ds;

    ID3D11Buffer *vbs[MAX_WINED3D_STREAMS];
    UINT vb_strides[MAX_WINED3D_STREAMS];
    UINT vb_offsets[MAX_WINED3D_STREAMS];
    D3D11_PRIMITIVE_TOPOLOGY topology;
    ID3D11InputLayout *il;
    DXGI_FORMAT ib_format;
    ID3D11Buffer *ib;
    UINT ib_offset;

    ID3D11RenderTargetView *rtvs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
    ID3D11DepthStencilView *dsv;
    ID3D11DepthStencilState *dss;
    float blend_factor[4];
    ID3D11BlendState *bs;
    UINT stencil_ref;
    UINT sample_mask;

    D3D11_RECT scissor_rects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    D3D11_VIEWPORT vps[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
    ID3D11Buffer *so_buffers[D3D11_SO_BUFFER_SLOT_COUNT];
    UINT so_offsets[D3D11_SO_BUFFER_SLOT_COUNT];
    ID3D11Predicate *predicate;
    ID3D11RasterizerState *rs;
    BOOL predicate_value;
};

/* ID3D11CommandList - command list */
struct d3d11_command_list
{
    ID3D11CommandList ID3D11CommandList_iface;
    ID3D11Device *device;
    LONG refcount;

    struct list commands;

    struct wined3d_private_store private_store;
};

/* ID3D11DeviceContext - deferred context */
struct d3d11_deferred_context
{
    ID3D11DeviceContext ID3D11DeviceContext_iface;
    ID3D11Device *device;
    LONG refcount;

    struct list commands;

    struct wined3d_private_store private_store;
};

static struct deferred_call *add_deferred_call(struct d3d11_deferred_context *context, size_t extra_size)
{
    struct deferred_call *call;

    if (!(call = HeapAlloc(GetProcessHeap(), 0, sizeof(*call) + extra_size)))
        return NULL;

    call->cmd = 0xdeadbeef;
    list_add_tail(&context->commands, &call->entry);
    return call;
}

/* for DEFERRED_CSSETSHADERRESOURCES, DEFERRED_DSSETSHADERRESOURCES, DEFERRED_GSSETSHADERRESOURCES,
 * DEFERRED_HSSETSHADERRESOURCES, DEFERRED_PSSETSHADERRESOURCES and DEFERRED_VSSETSHADERRESOURCES */
static void add_deferred_set_shader_resources(struct d3d11_deferred_context *context, enum deferred_cmd cmd,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct deferred_call *call;
    int i;

    if (!(call = add_deferred_call(context, sizeof(*views) * view_count)))
        return;

    call->cmd = cmd;
    call->res_info.start_slot = start_slot;
    call->res_info.num_views = view_count;
    call->res_info.views = (void *)(call + 1);
    for (i = 0; i < view_count; i++)
    {
        if (views[i]) ID3D11ShaderResourceView_AddRef(views[i]);
        call->res_info.views[i] = views[i];
    }
}

/* for DEFERRED_CSSETSAMPLERS, DEFERRED_DSSETSAMPLERS, DEFERRED_GSSETSAMPLERS,
 * DEFERRED_HSSETSAMPLERS, DEFERRED_PSSETSAMPLERS and DEFERRED_VSSETSAMPLERS */
static void add_deferred_set_samplers(struct d3d11_deferred_context *context, enum deferred_cmd cmd,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct deferred_call *call;
    int i;

    if (!(call = add_deferred_call(context, sizeof(*samplers) * sampler_count)))
        return;

    call->cmd = cmd;
    call->samplers_info.start_slot = start_slot;
    call->samplers_info.num_samplers = sampler_count;
    call->samplers_info.samplers = (void *)(call + 1);
    for (i = 0; i < sampler_count; i++)
    {
        if (samplers[i]) ID3D11SamplerState_AddRef(samplers[i]);
        call->samplers_info.samplers[i] = samplers[i];
    }
}

/* for DEFERRED_CSSETCONSTANTBUFFERS. DEFERRED_DSSETCONSTANTBUFFERS, DEFERRED_GSSETCONSTANTBUFFERS,
 * DEFERRED_HSSETCONSTANTBUFFERS, DEFERRED_PSSETCONSTANTBUFFERS and DEFERRED_VSSETCONSTANTBUFFERS */
static void add_deferred_set_constant_buffers(struct d3d11_deferred_context *context, enum deferred_cmd cmd,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct deferred_call *call;
    int i;

    if (!(call = add_deferred_call(context, sizeof(*buffers) * buffer_count)))
        return;

    call->cmd = cmd;
    call->constant_buffers_info.start_slot = start_slot;
    call->constant_buffers_info.num_buffers = buffer_count;
    call->constant_buffers_info.buffers = (void *)(call + 1);
    for (i = 0; i < buffer_count; i++)
    {
        if (buffers[i]) ID3D11Buffer_AddRef(buffers[i]);
        call->constant_buffers_info.buffers[i] = buffers[i];
    }
}

static void free_deferred_calls(struct list *commands)
{
    struct deferred_call *call, *call2;
    int i;

    LIST_FOR_EACH_ENTRY_SAFE(call, call2, commands, struct deferred_call, entry)
    {
        switch (call->cmd)
        {
            case DEFERRED_IASETVERTEXBUFFERS:
            {
                for (i = 0; i < call->vbuffer_info.num_buffers; i++)
                {
                    if (call->vbuffer_info.buffers[i])
                        ID3D11Buffer_Release(call->vbuffer_info.buffers[i]);
                }
                break;
            }
            case DEFERRED_IASETPRIMITIVETOPOLOGY:
            {
                break; /* nothing to do */
            }
            case DEFERRED_IASETINDEXBUFFER:
            {
                if (call->index_buffer_info.buffer)
                    ID3D11Buffer_Release(call->index_buffer_info.buffer);
                break;
            }
            case DEFERRED_IASETINPUTLAYOUT:
            {
                if (call->input_layout_info.layout)
                    ID3D11InputLayout_Release(call->input_layout_info.layout);
                break;
            }
            case DEFERRED_RSSETSTATE:
            {
                if (call->rstate_info.state)
                    ID3D11RasterizerState_Release(call->rstate_info.state);
                break;
            }
            case DEFERRED_RSSETVIEWPORTS:
            {
                break; /* nothing to do */
            }
            case DEFERRED_RSSETSCISSORRECTS:
            {
                break; /* nothing to do */
            }
            case DEFERRED_OMSETDEPTHSTENCILSTATE:
            {
                if (call->stencil_state_info.state)
                    ID3D11DepthStencilState_Release(call->stencil_state_info.state);
                break;
            }
            case DEFERRED_OMSETBLENDSTATE:
            {
                if (call->blend_state_info.state)
                    ID3D11BlendState_Release(call->blend_state_info.state);
                break;
            }
            case DEFERRED_OMSETRENDERTARGETS:
            {
                for (i = 0; i < call->render_target_info.num_views; i++)
                {
                    if (call->render_target_info.render_targets[i])
                        ID3D11RenderTargetView_Release(call->render_target_info.render_targets[i]);
                }
                if (call->render_target_info.depth_stencil)
                    ID3D11DepthStencilView_Release(call->render_target_info.depth_stencil);
                break;
            }
            case DEFERRED_OMSETRENDERTARGETSANDUNORDEREDACCESVIEWS:
            {
                for (i = 0; i < call->render_targets_and_unordered_access_views_info.render_target_view_count; i++)
                {
                    if (call->render_targets_and_unordered_access_views_info.render_target_views[i])
                        ID3D11RenderTargetView_Release(call->render_targets_and_unordered_access_views_info.render_target_views[i]);
                }
                if (call->render_targets_and_unordered_access_views_info.depth_stencil_view)
                    ID3D11DepthStencilView_Release(call->render_targets_and_unordered_access_views_info.depth_stencil_view);
                for (i = 0; i < call->render_targets_and_unordered_access_views_info.unordered_access_view_count; i++)
                {
                    if (call->render_targets_and_unordered_access_views_info.unordered_access_views[i])
                        ID3D11UnorderedAccessView_Release(call->render_targets_and_unordered_access_views_info.unordered_access_views[i]);
                }
                break;
            }
            case DEFERRED_COPYRESOURCE:
            {
                if (call->copy_resource_info.dst_resource)
                    ID3D11Resource_Release(call->copy_resource_info.dst_resource);
                if (call->copy_resource_info.src_resource)
                    ID3D11Resource_Release(call->copy_resource_info.src_resource);
                break;
            }
            case DEFERRED_SETRESOURCEMINLOD:
            {
                if (call->set_resource_min_lod_info.resource)
                    ID3D11Resource_Release(call->set_resource_min_lod_info.resource);
                break;
            }
            case DEFERRED_COPYSUBRESOURCEREGION:
            {
                if (call->copy_subresource_region_info.dst_resource)
                    ID3D11Resource_Release(call->copy_subresource_region_info.dst_resource);
                if (call->copy_subresource_region_info.src_resource)
                    ID3D11Resource_Release(call->copy_subresource_region_info.src_resource);
                break;
            }
            case DEFERRED_UPDATESUBRESOURCE:
            {
                if (call->update_subresource_info.resource)
                    ID3D11Resource_Release(call->update_subresource_info.resource);
                break;
            }
            case DEFERRED_RESOLVESUBRESOURCE:
            {
                if (call->resolve_subresource_info.dst_resource)
                    ID3D11Resource_Release(call->resolve_subresource_info.dst_resource);
                if (call->resolve_subresource_info.src_resource)
                    ID3D11Resource_Release(call->resolve_subresource_info.src_resource);
                break;
            }
            case DEFERRED_COPYSTRUCTURECOUNT:
            {
                if (call->copy_structure_count_info.dst_buffer)
                    ID3D11Buffer_Release(call->copy_structure_count_info.dst_buffer);
                if (call->copy_structure_count_info.src_view)
                    ID3D11UnorderedAccessView_Release(call->copy_structure_count_info.src_view);
                break;
            }
            case DEFERRED_CSSETSHADER:
            {
                if (call->cs_info.shader)
                    ID3D11ComputeShader_Release(call->cs_info.shader);
                break;
            }
            case DEFERRED_DSSETSHADER:
            {
                if (call->ds_info.shader)
                    ID3D11DomainShader_Release(call->ds_info.shader);
                break;
            }
            case DEFERRED_GSSETSHADER:
            {
                if (call->gs_info.shader)
                    ID3D11GeometryShader_Release(call->gs_info.shader);
                break;
            }
            case DEFERRED_HSSETSHADER:
            {
                if (call->hs_info.shader)
                    ID3D11HullShader_Release(call->hs_info.shader);
                break;
            }
            case DEFERRED_PSSETSHADER:
            {
                if (call->ps_info.shader)
                    ID3D11PixelShader_Release(call->ps_info.shader);
                break;
            }
            case DEFERRED_VSSETSHADER:
            {
                if (call->vs_info.shader)
                    ID3D11VertexShader_Release(call->vs_info.shader);
                break;
            }
            case DEFERRED_CSSETSHADERRESOURCES:
            case DEFERRED_DSSETSHADERRESOURCES:
            case DEFERRED_GSSETSHADERRESOURCES:
            case DEFERRED_HSSETSHADERRESOURCES:
            case DEFERRED_PSSETSHADERRESOURCES:
            case DEFERRED_VSSETSHADERRESOURCES:
            {
                for (i = 0; i < call->res_info.num_views; i++)
                {
                    if (call->res_info.views[i])
                        ID3D11ShaderResourceView_Release(call->res_info.views[i]);
                }
                break;
            }
            case DEFERRED_CSSETSAMPLERS:
            case DEFERRED_DSSETSAMPLERS:
            case DEFERRED_GSSETSAMPLERS:
            case DEFERRED_HSSETSAMPLERS:
            case DEFERRED_PSSETSAMPLERS:
            case DEFERRED_VSSETSAMPLERS:
            {
                for (i = 0; i < call->samplers_info.num_samplers; i++)
                {
                    if (call->samplers_info.samplers[i])
                        ID3D11SamplerState_Release(call->samplers_info.samplers[i]);
                }
                break;
            }
            case DEFERRED_CSSETCONSTANTBUFFERS:
            case DEFERRED_DSSETCONSTANTBUFFERS:
            case DEFERRED_GSSETCONSTANTBUFFERS:
            case DEFERRED_HSSETCONSTANTBUFFERS:
            case DEFERRED_PSSETCONSTANTBUFFERS:
            case DEFERRED_VSSETCONSTANTBUFFERS:
            {
                for (i = 0; i < call->constant_buffers_info.num_buffers; i++)
                {
                    if (call->constant_buffers_info.buffers[i])
                        ID3D11Buffer_Release(call->constant_buffers_info.buffers[i]);
                }
                break;
            }
            case DEFERRED_CSSETUNORDEREDACCESSVIEWS:
            {
                for (i = 0; i < call->unordered_view.num_views; i++)
                {
                    if (call->unordered_view.views[i])
                        ID3D11UnorderedAccessView_Release(call->unordered_view.views[i]);
                }
                break;
            }
            case DEFERRED_SOSETTARGETS:
            {
                for (i = 0; i < call->so_set_targets_info.buffer_count; i++)
                {
                    if (call->so_set_targets_info.buffers[i])
                        ID3D11Buffer_Release(call->so_set_targets_info.buffers[i]);
                }
                break;
            }
            case DEFERRED_GENERATEMIPS:
            {
                if (call->generate_mips_info.view)
                    ID3D11ShaderResourceView_Release(call->generate_mips_info.view);
                break;
            }
            case DEFERRED_DRAW:
            case DEFERRED_DRAWINDEXED:
            case DEFERRED_DRAWINDEXEDINSTANCED:
            {
                break; /* nothing to do */
            }
            case DEFERRED_DRAWAUTO:
            {
                break; /* nothing to do */
            }
            case DEFERRED_DRAWINSTANCED:
            {
                break; /* nothing to do */
            }
            case DEFERRED_DRAWINSTANCEDINDIRECT:
            case DEFERRED_DRAWINDEXEDINSTANCEDINDIRECT:
            {
                if (call->draw_instanced_indirect_info.buffer)
                    ID3D11Buffer_Release(call->draw_instanced_indirect_info.buffer);
                break;
            }
            case DEFERRED_MAP:
            {
                ID3D11Resource_Release(call->map_info.resource);
                break;
            }
            case DEFERRED_DISPATCH:
            {
                break; /* nothing to do */
            }
            case DEFERRED_DISPATCHINDIRECT:
            {
                if (call->dispatch_indirect_info.buffer)
                    ID3D11Buffer_Release(call->dispatch_indirect_info.buffer);
                break;
            }
            case DEFERRED_SETPREDICATION:
            {
                if (call->set_predication_info.predicate)
                    ID3D11Predicate_Release(call->set_predication_info.predicate);
                break;
            }
            case DEFERRED_CLEARSTATE:
            {
                break; /* nothing to do */
            }
            case DEFERRED_CLEARRENDERTARGETVIEW:
            {
                if (call->clear_rtv_info.rtv)
                    ID3D11RenderTargetView_Release(call->clear_rtv_info.rtv);
                break;
            }
            case DEFERRED_CLEARDEPTHSTENCILVIEW:
            {
                if (call->clear_depth_info.view)
                    ID3D11DepthStencilView_Release(call->clear_depth_info.view);
                break;
            }
            case DEFERRED_CLEARUNORDEREDACCESSVIEWUINT:
            {
                if (call->clear_unordered_access_view_uint.unordered_access_view)
                    ID3D11UnorderedAccessView_Release(call->clear_unordered_access_view_uint.unordered_access_view);
                break;
            }
            case DEFERRED_CLEARUNORDEREDACCESSVIEWFLOAT:
            {
                if (call->clear_unordered_access_view_float.unordered_access_view)
                    ID3D11UnorderedAccessView_Release(call->clear_unordered_access_view_float.unordered_access_view);
                break;
            }
            case DEFERRED_BEGIN:
            case DEFERRED_END:
            {
                if (call->async_info.asynchronous)
                    ID3D11Asynchronous_Release(call->async_info.asynchronous);
                break;
            }
            default:
            {
                FIXME("Unimplemented command type %u\n", call->cmd);
                break;
            }
        }

        list_remove(&call->entry);
        HeapFree(GetProcessHeap(), 0, call);
    }
}

static void exec_deferred_calls(ID3D11DeviceContext *iface, struct list *commands)
{
    struct deferred_call *call;

    LIST_FOR_EACH_ENTRY(call, commands, struct deferred_call, entry)
    {
        switch (call->cmd)
        {
            case DEFERRED_IASETVERTEXBUFFERS:
            {
                ID3D11DeviceContext_IASetVertexBuffers(iface, call->vbuffer_info.start_slot,
                        call->vbuffer_info.num_buffers, call->vbuffer_info.buffers,
                        call->vbuffer_info.strides, call->vbuffer_info.offsets);
                break;
            }
            case DEFERRED_IASETPRIMITIVETOPOLOGY:
            {
                ID3D11DeviceContext_IASetPrimitiveTopology(iface, call->topology_info.topology);
                break;
            }
            case DEFERRED_IASETINDEXBUFFER:
            {
                ID3D11DeviceContext_IASetIndexBuffer(iface, call->index_buffer_info.buffer,
                        call->index_buffer_info.format, call->index_buffer_info.offset);
                break;
            }
            case DEFERRED_IASETINPUTLAYOUT:
            {
                ID3D11DeviceContext_IASetInputLayout(iface, call->input_layout_info.layout);
                break;
            }
            case DEFERRED_RSSETSTATE:
            {
                ID3D11DeviceContext_RSSetState(iface, call->rstate_info.state);
                break;
            }
            case DEFERRED_RSSETVIEWPORTS:
            {
                ID3D11DeviceContext_RSSetViewports(iface, call->viewport_info.num_viewports,
                        call->viewport_info.viewports);
                break;
            }
            case DEFERRED_RSSETSCISSORRECTS:
            {
                ID3D11DeviceContext_RSSetScissorRects(iface,
                        call->rs_set_scissor_rects_info.rect_count,
                        call->rs_set_scissor_rects_info.rects);
                break;
            }
            case DEFERRED_OMSETDEPTHSTENCILSTATE:
            {
                ID3D11DeviceContext_OMSetDepthStencilState(iface, call->stencil_state_info.state,
                        call->stencil_state_info.stencil_ref);
                break;
            }
            case DEFERRED_OMSETBLENDSTATE:
            {
                ID3D11DeviceContext_OMSetBlendState(iface, call->blend_state_info.state,
                        call->blend_state_info.factor, call->blend_state_info.mask);
                break;
            }
            case DEFERRED_OMSETRENDERTARGETS:
            {
                ID3D11DeviceContext_OMSetRenderTargets(iface, call->render_target_info.num_views,
                        call->render_target_info.render_targets, call->render_target_info.depth_stencil);
                break;
            }
            case DEFERRED_OMSETRENDERTARGETSANDUNORDEREDACCESVIEWS:
            {
                ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews(iface,
                        call->render_targets_and_unordered_access_views_info.render_target_view_count,
                        call->render_targets_and_unordered_access_views_info.render_target_views,
                        call->render_targets_and_unordered_access_views_info.depth_stencil_view,
                        call->render_targets_and_unordered_access_views_info.unordered_access_view_start_slot,
                        call->render_targets_and_unordered_access_views_info.unordered_access_view_count,
                        call->render_targets_and_unordered_access_views_info.unordered_access_views,
                        call->render_targets_and_unordered_access_views_info.initial_counts);
                break;
            }
            case DEFERRED_COPYRESOURCE:
            {
                ID3D11DeviceContext_CopyResource(iface,
                        call->copy_resource_info.dst_resource,
                        call->copy_resource_info.src_resource);
                break;
            }
            case DEFERRED_SETRESOURCEMINLOD:
            {
                ID3D11DeviceContext_SetResourceMinLOD(iface,
                        call->set_resource_min_lod_info.resource,
                        call->set_resource_min_lod_info.min_lod);
                break;
            }
            case DEFERRED_COPYSUBRESOURCEREGION:
            {
                ID3D11DeviceContext_CopySubresourceRegion(iface,
                        call->copy_subresource_region_info.dst_resource,
                        call->copy_subresource_region_info.dst_subresource_idx,
                        call->copy_subresource_region_info.dst_x,
                        call->copy_subresource_region_info.dst_y,
                        call->copy_subresource_region_info.dst_z,
                        call->copy_subresource_region_info.src_resource,
                        call->copy_subresource_region_info.src_subresource_idx,
                        call->copy_subresource_region_info.src_box);
                break;
            }
            case DEFERRED_RESOLVESUBRESOURCE:
            {
                ID3D11DeviceContext_ResolveSubresource(iface,
                        call->resolve_subresource_info.dst_resource,
                        call->resolve_subresource_info.dst_subresource_idx,
                        call->resolve_subresource_info.src_resource,
                        call->resolve_subresource_info.src_subresource_idx,
                        call->resolve_subresource_info.format);
                break;
            }
            case DEFERRED_COPYSTRUCTURECOUNT:
            {
                ID3D11DeviceContext_CopyStructureCount(iface,
                        call->copy_structure_count_info.dst_buffer,
                        call->copy_structure_count_info.dst_offset,
                        call->copy_structure_count_info.src_view);
                break;
            }
            case DEFERRED_UPDATESUBRESOURCE:
            {
                ID3D11DeviceContext_UpdateSubresource(iface,
                        call->update_subresource_info.resource,
                        call->update_subresource_info.subresource_idx,
                        call->update_subresource_info.box,
                        call->update_subresource_info.data,
                        call->update_subresource_info.row_pitch,
                        call->update_subresource_info.depth_pitch);
                break;
            }
            case DEFERRED_CSSETSHADER:
            {
                ID3D11DeviceContext_CSSetShader(iface, call->cs_info.shader, NULL, 0);
                break;
            }
            case DEFERRED_DSSETSHADER:
            {
                ID3D11DeviceContext_DSSetShader(iface, call->ds_info.shader, NULL, 0);
                break;
            }
            case DEFERRED_GSSETSHADER:
            {
                ID3D11DeviceContext_GSSetShader(iface, call->gs_info.shader, NULL, 0);
                break;
            }
            case DEFERRED_HSSETSHADER:
            {
                ID3D11DeviceContext_HSSetShader(iface, call->hs_info.shader, NULL, 0);
                break;
            }
            case DEFERRED_PSSETSHADER:
            {
                ID3D11DeviceContext_PSSetShader(iface, call->ps_info.shader, NULL, 0);
                break;
            }
            case DEFERRED_VSSETSHADER:
            {
                ID3D11DeviceContext_VSSetShader(iface, call->vs_info.shader, NULL, 0);
                break;
            }
            case DEFERRED_CSSETSHADERRESOURCES:
            {
                ID3D11DeviceContext_CSSetShaderResources(iface, call->res_info.start_slot,
                    call->res_info.num_views, call->res_info.views);
                break;
            }
            case DEFERRED_DSSETSHADERRESOURCES:
            {
                ID3D11DeviceContext_DSSetShaderResources(iface, call->res_info.start_slot,
                    call->res_info.num_views, call->res_info.views);
                break;
            }
            case DEFERRED_GSSETSHADERRESOURCES:
            {
                ID3D11DeviceContext_GSSetShaderResources(iface, call->res_info.start_slot,
                    call->res_info.num_views, call->res_info.views);
                break;
            }
            case DEFERRED_HSSETSHADERRESOURCES:
            {
                ID3D11DeviceContext_HSSetShaderResources(iface, call->res_info.start_slot,
                    call->res_info.num_views, call->res_info.views);
                break;
            }
            case DEFERRED_PSSETSHADERRESOURCES:
            {
                ID3D11DeviceContext_PSSetShaderResources(iface, call->res_info.start_slot,
                        call->res_info.num_views, call->res_info.views);
                break;
            }
            case DEFERRED_VSSETSHADERRESOURCES:
            {
                ID3D11DeviceContext_VSSetShaderResources(iface, call->res_info.start_slot,
                        call->res_info.num_views, call->res_info.views);
                break;
            }
            case DEFERRED_CSSETSAMPLERS:
            {
                ID3D11DeviceContext_CSSetSamplers(iface, call->samplers_info.start_slot,
                        call->samplers_info.num_samplers, call->samplers_info.samplers);
                break;
            }
            case DEFERRED_DSSETSAMPLERS:
            {
                ID3D11DeviceContext_DSSetSamplers(iface, call->samplers_info.start_slot,
                        call->samplers_info.num_samplers, call->samplers_info.samplers);
                break;
            }
            case DEFERRED_GSSETSAMPLERS:
            {
                ID3D11DeviceContext_GSSetSamplers(iface, call->samplers_info.start_slot,
                        call->samplers_info.num_samplers, call->samplers_info.samplers);
                break;
            }
            case DEFERRED_HSSETSAMPLERS:
            {
                ID3D11DeviceContext_HSSetSamplers(iface, call->samplers_info.start_slot,
                        call->samplers_info.num_samplers, call->samplers_info.samplers);
                break;
            }
            case DEFERRED_PSSETSAMPLERS:
            {
                ID3D11DeviceContext_PSSetSamplers(iface, call->samplers_info.start_slot,
                        call->samplers_info.num_samplers, call->samplers_info.samplers);
                break;
            }
            case DEFERRED_VSSETSAMPLERS:
            {
                ID3D11DeviceContext_VSSetSamplers(iface, call->samplers_info.start_slot,
                        call->samplers_info.num_samplers, call->samplers_info.samplers);
                break;
            }
            case DEFERRED_CSSETCONSTANTBUFFERS:
            {
                ID3D11DeviceContext_CSSetConstantBuffers(iface, call->constant_buffers_info.start_slot,
                        call->constant_buffers_info.num_buffers, call->constant_buffers_info.buffers);
                break;
            }
            case DEFERRED_DSSETCONSTANTBUFFERS:
            {
                ID3D11DeviceContext_DSSetConstantBuffers(iface, call->constant_buffers_info.start_slot,
                        call->constant_buffers_info.num_buffers, call->constant_buffers_info.buffers);
                break;
            }
            case DEFERRED_GSSETCONSTANTBUFFERS:
            {
                ID3D11DeviceContext_GSSetConstantBuffers(iface, call->constant_buffers_info.start_slot,
                        call->constant_buffers_info.num_buffers, call->constant_buffers_info.buffers);
                break;
            }
            case DEFERRED_HSSETCONSTANTBUFFERS:
            {
                ID3D11DeviceContext_HSSetConstantBuffers(iface, call->constant_buffers_info.start_slot,
                        call->constant_buffers_info.num_buffers, call->constant_buffers_info.buffers);
                break;
            }
            case DEFERRED_PSSETCONSTANTBUFFERS:
            {
                ID3D11DeviceContext_PSSetConstantBuffers(iface, call->constant_buffers_info.start_slot,
                        call->constant_buffers_info.num_buffers, call->constant_buffers_info.buffers);
                break;
            }
            case DEFERRED_VSSETCONSTANTBUFFERS:
            {
                ID3D11DeviceContext_VSSetConstantBuffers(iface, call->constant_buffers_info.start_slot,
                        call->constant_buffers_info.num_buffers, call->constant_buffers_info.buffers);
                break;
            }
            case DEFERRED_CSSETUNORDEREDACCESSVIEWS:
            {
                ID3D11DeviceContext_CSSetUnorderedAccessViews(iface, call->unordered_view.start_slot,
                        call->unordered_view.num_views, call->unordered_view.views, call->unordered_view.initial_counts);
                break;
            }
            case DEFERRED_SOSETTARGETS:
            {
                ID3D11DeviceContext_SOSetTargets(iface,
                        call->so_set_targets_info.buffer_count,
                        call->so_set_targets_info.buffers,
                        call->so_set_targets_info.offsets);
                break;
            }
            case DEFERRED_GENERATEMIPS:
            {
                ID3D11DeviceContext_GenerateMips(iface,
                        call->generate_mips_info.view);
                break;
            }
            case DEFERRED_DRAW:
            {
                ID3D11DeviceContext_Draw(iface, call->draw_info.count, call->draw_info.start);
                break;
            }
            case DEFERRED_DRAWINDEXED:
            {
                ID3D11DeviceContext_DrawIndexed(iface, call->draw_indexed_info.count,
                        call->draw_indexed_info.start_index, call->draw_indexed_info.base_vertex);
                break;
            }
            case DEFERRED_DRAWINDEXEDINSTANCED:
            {
                ID3D11DeviceContext_DrawIndexedInstanced(iface, call->draw_indexed_inst_info.count_per_instance,
                        call->draw_indexed_inst_info.instance_count, call->draw_indexed_inst_info.start_index,
                        call->draw_indexed_inst_info.base_vertex, call->draw_indexed_inst_info.start_instance);
                break;
            }
            case DEFERRED_DRAWAUTO:
            {
                ID3D11DeviceContext_DrawAuto(iface);
                break;
            }
            case DEFERRED_DRAWINSTANCED:
            {
                ID3D11DeviceContext_DrawInstanced(iface,
                        call->draw_instanced_info.instance_vertex_count,
                        call->draw_instanced_info.instance_count,
                        call->draw_instanced_info.start_vertex_location,
                        call->draw_instanced_info.start_instance_location);
                break;
            }
            case DEFERRED_DRAWINSTANCEDINDIRECT:
            {
                ID3D11DeviceContext_DrawInstancedIndirect(iface,
                        call->draw_instanced_indirect_info.buffer,
                        call->draw_instanced_indirect_info.offset);
                break;
            }
            case DEFERRED_DRAWINDEXEDINSTANCEDINDIRECT:
            {
                ID3D11DeviceContext_DrawIndexedInstancedIndirect(iface,
                        call->draw_instanced_indirect_info.buffer,
                        call->draw_instanced_indirect_info.offset);
                break;
            }
            case DEFERRED_MAP:
            {
                D3D11_MAPPED_SUBRESOURCE mapped;
                HRESULT hr;

                hr = ID3D11DeviceContext_Map(iface, call->map_info.resource, call->map_info.subresource_idx,
                        call->map_info.map_type, call->map_info.map_flags, &mapped);
                if (SUCCEEDED(hr))
                {
                    memcpy(mapped.pData, call->map_info.buffer, call->map_info.size);
                    ID3D11DeviceContext_Unmap(iface, call->map_info.resource, call->map_info.subresource_idx);
                }
                else
                    FIXME("Failed to map subresource!\n");

                break;
            }
            case DEFERRED_DISPATCH:
            {
                ID3D11DeviceContext_Dispatch(iface, call->dispatch_info.count_x,
                        call->dispatch_info.count_y, call->dispatch_info.count_z);
                break;
            }
            case DEFERRED_DISPATCHINDIRECT:
            {
                ID3D11DeviceContext_DispatchIndirect(iface,
                        call->dispatch_indirect_info.buffer,
                        call->dispatch_indirect_info.offset);
                break;
            }
            case DEFERRED_SETPREDICATION:
            {
                ID3D11DeviceContext_SetPredication(iface,
                        call->set_predication_info.predicate,
                        call->set_predication_info.value);
                break;
            }
            case DEFERRED_CLEARSTATE:
            {
                ID3D11DeviceContext_ClearState(iface);
                break;
            }
            case DEFERRED_CLEARRENDERTARGETVIEW:
            {
                ID3D11DeviceContext_ClearRenderTargetView(iface, call->clear_rtv_info.rtv,
                        call->clear_rtv_info.color);
                break;
            }
            case DEFERRED_CLEARDEPTHSTENCILVIEW:
            {
                ID3D11DeviceContext_ClearDepthStencilView(iface, call->clear_depth_info.view,
                        call->clear_depth_info.flags, call->clear_depth_info.depth,
                        call->clear_depth_info.stencil);
                break;
            }
            case DEFERRED_CLEARUNORDEREDACCESSVIEWUINT:
            {
                ID3D11DeviceContext_ClearUnorderedAccessViewUint(iface,
                        call->clear_unordered_access_view_uint.unordered_access_view,
                        call->clear_unordered_access_view_uint.values);
                break;
            }
            case DEFERRED_CLEARUNORDEREDACCESSVIEWFLOAT:
            {
                ID3D11DeviceContext_ClearUnorderedAccessViewFloat(iface,
                        call->clear_unordered_access_view_float.unordered_access_view,
                        call->clear_unordered_access_view_float.values);
                break;
            }
            case DEFERRED_BEGIN:
            {
                ID3D11DeviceContext_Begin(iface, call->async_info.asynchronous);
                break;
            }
            case DEFERRED_END:
            {
                ID3D11DeviceContext_End(iface, call->async_info.asynchronous);
                break;
            }
            default:
            {
                FIXME("Unimplemented command type %u\n", call->cmd);
                break;
            }
        }
    }
}

/* ID3D11CommandList - command list methods */

static inline struct d3d11_command_list *impl_from_ID3D11CommandList(ID3D11CommandList *iface)
{
    return CONTAINING_RECORD(iface, struct d3d11_command_list, ID3D11CommandList_iface);
}

static HRESULT STDMETHODCALLTYPE d3d11_command_list_QueryInterface(ID3D11CommandList *iface,
        REFIID riid, void **out)
{
    TRACE("iface %p, riid %s, out %p.\n", iface, debugstr_guid(riid), out);

    if (IsEqualGUID(riid, &IID_ID3D11CommandList)
            || IsEqualGUID(riid, &IID_ID3D11DeviceChild)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        ID3D11CommandList_AddRef(iface);
        *out = iface;
        return S_OK;
    }

    WARN("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(riid));
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE d3d11_command_list_AddRef(ID3D11CommandList *iface)
{
    struct d3d11_command_list *cmdlist = impl_from_ID3D11CommandList(iface);
    ULONG refcount = InterlockedIncrement(&cmdlist->refcount);

    TRACE("%p increasing refcount to %u.\n", cmdlist, refcount);

    return refcount;
}

static ULONG STDMETHODCALLTYPE d3d11_command_list_Release(ID3D11CommandList *iface)
{
    struct d3d11_command_list *cmdlist = impl_from_ID3D11CommandList(iface);
    ULONG refcount = InterlockedDecrement(&cmdlist->refcount);

    TRACE("%p decreasing refcount to %u.\n", cmdlist, refcount);

    if (!refcount)
    {
        free_deferred_calls(&cmdlist->commands);
        wined3d_private_store_cleanup(&cmdlist->private_store);
        HeapFree(GetProcessHeap(), 0, cmdlist);
    }

    return refcount;
}

static void STDMETHODCALLTYPE d3d11_command_list_GetDevice(ID3D11CommandList *iface, ID3D11Device **device)
{
    struct d3d11_command_list *cmdlist = impl_from_ID3D11CommandList(iface);

    TRACE("iface %p, device %p.\n", iface, device);

    ID3D11Device_AddRef(cmdlist->device);
    *device = cmdlist->device;
}

static HRESULT STDMETHODCALLTYPE d3d11_command_list_GetPrivateData(ID3D11CommandList *iface, REFGUID guid,
        UINT *data_size, void *data)
{
    struct d3d11_command_list *cmdlist = impl_from_ID3D11CommandList(iface);

    TRACE("iface %p, guid %s, data_size %p, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return d3d_get_private_data(&cmdlist->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_command_list_SetPrivateData(ID3D11CommandList *iface, REFGUID guid,
        UINT data_size, const void *data)
{
    struct d3d11_command_list *cmdlist = impl_from_ID3D11CommandList(iface);

    TRACE("iface %p, guid %s, data_size %u, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return d3d_set_private_data(&cmdlist->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_command_list_SetPrivateDataInterface(ID3D11CommandList *iface,
        REFGUID guid, const IUnknown *data)
{
    struct d3d11_command_list *cmdlist = impl_from_ID3D11CommandList(iface);

    TRACE("iface %p, guid %s, data %p.\n", iface, debugstr_guid(guid), data);

    return d3d_set_private_data_interface(&cmdlist->private_store, guid, data);
}

static UINT STDMETHODCALLTYPE d3d11_command_list_GetContextFlags(ID3D11CommandList *iface)
{
    TRACE("iface %p.\n", iface);

    return 0;
}

static const struct ID3D11CommandListVtbl d3d11_command_list_vtbl =
{
    /* IUnknown methods */
    d3d11_command_list_QueryInterface,
    d3d11_command_list_AddRef,
    d3d11_command_list_Release,
    /* ID3D11DeviceChild methods */
    d3d11_command_list_GetDevice,
    d3d11_command_list_GetPrivateData,
    d3d11_command_list_SetPrivateData,
    d3d11_command_list_SetPrivateDataInterface,
    /* ID3D11CommandList methods */
    d3d11_command_list_GetContextFlags,
};

static inline struct d3d11_command_list *unsafe_impl_from_ID3D11CommandList(ID3D11CommandList *iface)
{
    if (!iface)
        return NULL;
    assert(iface->lpVtbl == &d3d11_command_list_vtbl);
    return CONTAINING_RECORD(iface, struct d3d11_command_list, ID3D11CommandList_iface);
}

static void STDMETHODCALLTYPE d3d_null_wined3d_object_destroyed(void *parent) {}

static const struct wined3d_parent_ops d3d_null_wined3d_parent_ops =
{
    d3d_null_wined3d_object_destroyed,
};

/* ID3D11DeviceContext - immediate context methods */

static inline struct d3d11_immediate_context *impl_from_ID3D11DeviceContext(ID3D11DeviceContext *iface)
{
    return CONTAINING_RECORD(iface, struct d3d11_immediate_context, ID3D11DeviceContext_iface);
}

static inline struct d3d_device *device_from_immediate_ID3D11DeviceContext(ID3D11DeviceContext *iface)
{
    struct d3d11_immediate_context *context = impl_from_ID3D11DeviceContext(iface);
    return CONTAINING_RECORD(context, struct d3d_device, immediate_context);
}

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_QueryInterface(ID3D11DeviceContext *iface,
        REFIID riid, void **out)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, riid %s, out %p.\n", iface, debugstr_guid(riid), out);

    if (IsEqualGUID(riid, &IID_ID3D11DeviceContext)
            || IsEqualGUID(riid, &IID_ID3D11DeviceChild)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        ID3D11DeviceContext_AddRef(iface);
        *out = iface;
        return S_OK;
    }
    else if (IsEqualGUID(riid, &IID_IWineD3DDevice))
    {
        *out = device->wined3d_device;
        return S_OK;
    }

    WARN("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(riid));
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE d3d11_immediate_context_AddRef(ID3D11DeviceContext *iface)
{
    struct d3d11_immediate_context *context = impl_from_ID3D11DeviceContext(iface);
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    ULONG refcount = InterlockedIncrement(&context->refcount);

    TRACE("%p increasing refcount to %u.\n", context, refcount);

    if (refcount == 1)
    {
        ID3D11Device_AddRef(&device->ID3D11Device_iface);
    }

    return refcount;
}

static ULONG STDMETHODCALLTYPE d3d11_immediate_context_Release(ID3D11DeviceContext *iface)
{
    struct d3d11_immediate_context *context = impl_from_ID3D11DeviceContext(iface);
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    ULONG refcount = InterlockedDecrement(&context->refcount);

    TRACE("%p decreasing refcount to %u.\n", context, refcount);

    if (!refcount)
    {
        ID3D11Device_Release(&device->ID3D11Device_iface);
    }

    return refcount;
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GetDevice(ID3D11DeviceContext *iface, ID3D11Device **device)
{
    struct d3d_device *device_object = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, device %p.\n", iface, device);

    *device = &device_object->ID3D11Device_iface;
    ID3D11Device_AddRef(*device);
}

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_GetPrivateData(ID3D11DeviceContext *iface, REFGUID guid,
        UINT *data_size, void *data)
{
    struct d3d11_immediate_context *context = impl_from_ID3D11DeviceContext(iface);

    TRACE("iface %p, guid %s, data_size %p, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return d3d_get_private_data(&context->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_SetPrivateData(ID3D11DeviceContext *iface, REFGUID guid,
        UINT data_size, const void *data)
{
    struct d3d11_immediate_context *context = impl_from_ID3D11DeviceContext(iface);

    TRACE("iface %p, guid %s, data_size %u, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return d3d_set_private_data(&context->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_SetPrivateDataInterface(ID3D11DeviceContext *iface,
        REFGUID guid, const IUnknown *data)
{
    struct d3d11_immediate_context *context = impl_from_ID3D11DeviceContext(iface);

    TRACE("iface %p, guid %s, data %p.\n", iface, debugstr_guid(guid), data);

    return d3d_set_private_data_interface(&context->private_store, guid, data);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_VSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D11Buffer(buffers[i]);

        wined3d_device_set_vs_cb(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_PSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D11ShaderResourceView(views[i]);

        wined3d_device_set_ps_resource_view(device->wined3d_device, start_slot + i,
                view ? view->wined3d_view : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_PSSetShader(ID3D11DeviceContext *iface,
        ID3D11PixelShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_pixel_shader *ps = unsafe_impl_from_ID3D11PixelShader(shader);

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances)
        FIXME("Dynamic linking is not implemented yet.\n");

    wined3d_mutex_lock();
    wined3d_device_set_pixel_shader(device->wined3d_device, ps ? ps->wined3d_shader : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_PSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler = unsafe_impl_from_ID3D11SamplerState(samplers[i]);

        wined3d_device_set_ps_sampler(device->wined3d_device, start_slot + i,
                sampler ? sampler->wined3d_sampler : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_VSSetShader(ID3D11DeviceContext *iface,
        ID3D11VertexShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_vertex_shader *vs = unsafe_impl_from_ID3D11VertexShader(shader);

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances)
        FIXME("Dynamic linking is not implemented yet.\n");

    wined3d_mutex_lock();
    wined3d_device_set_vertex_shader(device->wined3d_device, vs ? vs->wined3d_shader : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawIndexed(ID3D11DeviceContext *iface,
        UINT index_count, UINT start_index_location, INT base_vertex_location)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, index_count %u, start_index_location %u, base_vertex_location %d.\n",
            iface, index_count, start_index_location, base_vertex_location);

    wined3d_mutex_lock();
    wined3d_device_set_base_vertex_index(device->wined3d_device, base_vertex_location);
    wined3d_device_draw_indexed_primitive(device->wined3d_device, start_index_location, index_count);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_Draw(ID3D11DeviceContext *iface,
        UINT vertex_count, UINT start_vertex_location)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, vertex_count %u, start_vertex_location %u.\n",
            iface, vertex_count, start_vertex_location);

    wined3d_mutex_lock();
    wined3d_device_draw_primitive(device->wined3d_device, start_vertex_location, vertex_count);
    wined3d_mutex_unlock();
}

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_Map(ID3D11DeviceContext *iface, ID3D11Resource *resource,
        UINT subresource_idx, D3D11_MAP map_type, UINT map_flags, D3D11_MAPPED_SUBRESOURCE *mapped_subresource)
{
    struct wined3d_resource *wined3d_resource;
    struct wined3d_map_desc map_desc;
    HRESULT hr;

    TRACE("iface %p, resource %p, subresource_idx %u, map_type %u, map_flags %#x, mapped_subresource %p.\n",
            iface, resource, subresource_idx, map_type, map_flags, mapped_subresource);

    if (map_flags)
        FIXME("Ignoring map_flags %#x.\n", map_flags);

    wined3d_resource = wined3d_resource_from_d3d11_resource(resource);

    wined3d_mutex_lock();
    hr = wined3d_resource_map(wined3d_resource, subresource_idx,
            &map_desc, NULL, wined3d_map_flags_from_d3d11_map_type(map_type));
    wined3d_mutex_unlock();

    mapped_subresource->pData = map_desc.data;
    mapped_subresource->RowPitch = map_desc.row_pitch;
    mapped_subresource->DepthPitch = map_desc.slice_pitch;

    return hr;
}

static void STDMETHODCALLTYPE d3d11_immediate_context_Unmap(ID3D11DeviceContext *iface, ID3D11Resource *resource,
        UINT subresource_idx)
{
    struct wined3d_resource *wined3d_resource;

    TRACE("iface %p, resource %p, subresource_idx %u.\n", iface, resource, subresource_idx);

    wined3d_resource = wined3d_resource_from_d3d11_resource(resource);

    wined3d_mutex_lock();
    wined3d_resource_unmap(wined3d_resource, subresource_idx);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_PSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D11Buffer(buffers[i]);

        wined3d_device_set_ps_cb(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_IASetInputLayout(ID3D11DeviceContext *iface,
        ID3D11InputLayout *input_layout)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_input_layout *layout = unsafe_impl_from_ID3D11InputLayout(input_layout);

    TRACE("iface %p, input_layout %p.\n", iface, input_layout);

    wined3d_mutex_lock();
    wined3d_device_set_vertex_declaration(device->wined3d_device, layout ? layout->wined3d_decl : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_IASetVertexBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers, const UINT *strides, const UINT *offsets)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p, strides %p, offsets %p.\n",
            iface, start_slot, buffer_count, buffers, strides, offsets);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D11Buffer(buffers[i]);

        wined3d_device_set_stream_source(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL, offsets[i], strides[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_IASetIndexBuffer(ID3D11DeviceContext *iface,
        ID3D11Buffer *buffer, DXGI_FORMAT format, UINT offset)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_buffer *buffer_impl = unsafe_impl_from_ID3D11Buffer(buffer);

    TRACE("iface %p, buffer %p, format %s, offset %u.\n",
            iface, buffer, debug_dxgi_format(format), offset);

    wined3d_mutex_lock();
    wined3d_device_set_index_buffer(device->wined3d_device,
            buffer_impl ? buffer_impl->wined3d_buffer : NULL,
            wined3dformat_from_dxgi_format(format), offset);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawIndexedInstanced(ID3D11DeviceContext *iface,
        UINT instance_index_count, UINT instance_count, UINT start_index_location, INT base_vertex_location,
        UINT start_instance_location)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, instance_index_count %u, instance_count %u, start_index_location %u, "
            "base_vertex_location %d, start_instance_location %u.\n",
            iface, instance_index_count, instance_count, start_index_location,
            base_vertex_location, start_instance_location);

    wined3d_mutex_lock();
    wined3d_device_set_base_vertex_index(device->wined3d_device, base_vertex_location);
    wined3d_device_draw_indexed_primitive_instanced(device->wined3d_device, start_index_location,
            instance_index_count, start_instance_location, instance_count);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawInstanced(ID3D11DeviceContext *iface,
        UINT instance_vertex_count, UINT instance_count, UINT start_vertex_location, UINT start_instance_location)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, instance_vertex_count %u, instance_count %u, start_vertex_location %u, "
            "start_instance_location %u.\n",
            iface, instance_vertex_count, instance_count, start_vertex_location,
            start_instance_location);

    wined3d_mutex_lock();
    wined3d_device_draw_primitive_instanced(device->wined3d_device, start_vertex_location,
            instance_vertex_count, start_instance_location, instance_count);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D11Buffer(buffers[i]);

        wined3d_device_set_gs_cb(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GSSetShader(ID3D11DeviceContext *iface,
        ID3D11GeometryShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_geometry_shader *gs = unsafe_impl_from_ID3D11GeometryShader(shader);

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances)
        FIXME("Dynamic linking is not implemented yet.\n");

    wined3d_mutex_lock();
    wined3d_device_set_geometry_shader(device->wined3d_device, gs ? gs->wined3d_shader : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_IASetPrimitiveTopology(ID3D11DeviceContext *iface,
        D3D11_PRIMITIVE_TOPOLOGY topology)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    enum wined3d_primitive_type primitive_type;
    unsigned int patch_vertex_count;

    TRACE("iface %p, topology %#x.\n", iface, topology);

    wined3d_primitive_type_from_d3d11_primitive_topology(topology, &primitive_type, &patch_vertex_count);

    wined3d_mutex_lock();
    wined3d_device_set_primitive_type(device->wined3d_device, primitive_type, patch_vertex_count);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_VSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n", iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D11ShaderResourceView(views[i]);

        wined3d_device_set_vs_resource_view(device->wined3d_device, start_slot + i,
                view ? view->wined3d_view : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_VSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler = unsafe_impl_from_ID3D11SamplerState(samplers[i]);

        wined3d_device_set_vs_sampler(device->wined3d_device, start_slot + i,
                sampler ? sampler->wined3d_sampler : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_Begin(ID3D11DeviceContext *iface,
        ID3D11Asynchronous *asynchronous)
{
    struct d3d_query *query = unsafe_impl_from_ID3D11Asynchronous(asynchronous);
    HRESULT hr;

    TRACE("iface %p, asynchronous %p.\n", iface, asynchronous);

    wined3d_mutex_lock();
    if (FAILED(hr = wined3d_query_issue(query->wined3d_query, WINED3DISSUE_BEGIN)))
        ERR("Failed to issue query, hr %#x.\n", hr);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_End(ID3D11DeviceContext *iface,
        ID3D11Asynchronous *asynchronous)
{
    struct d3d_query *query = unsafe_impl_from_ID3D11Asynchronous(asynchronous);
    HRESULT hr;

    TRACE("iface %p, asynchronous %p.\n", iface, asynchronous);

    wined3d_mutex_lock();
    if (FAILED(hr = wined3d_query_issue(query->wined3d_query, WINED3DISSUE_END)))
        ERR("Failed to issue query, hr %#x.\n", hr);
    wined3d_mutex_unlock();
}

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_GetData(ID3D11DeviceContext *iface,
        ID3D11Asynchronous *asynchronous, void *data, UINT data_size, UINT data_flags)
{
    struct d3d_query *query = unsafe_impl_from_ID3D11Asynchronous(asynchronous);
    unsigned int wined3d_flags;
    HRESULT hr;

    TRACE("iface %p, asynchronous %p, data %p, data_size %u, data_flags %#x.\n",
            iface, asynchronous, data, data_size, data_flags);

    if (!data && data_size)
        return E_INVALIDARG;

    wined3d_flags = wined3d_getdata_flags_from_d3d11_async_getdata_flags(data_flags);

    wined3d_mutex_lock();
    if (!data_size || wined3d_query_get_data_size(query->wined3d_query) == data_size)
    {
        hr = wined3d_query_get_data(query->wined3d_query, data, data_size, wined3d_flags);
        if (hr == WINED3DERR_INVALIDCALL)
            hr = DXGI_ERROR_INVALID_CALL;
    }
    else
    {
        WARN("Invalid data size %u.\n", data_size);
        hr = E_INVALIDARG;
    }
    wined3d_mutex_unlock();

    return hr;
}

static void STDMETHODCALLTYPE d3d11_immediate_context_SetPredication(ID3D11DeviceContext *iface,
        ID3D11Predicate *predicate, BOOL value)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_query *query;

    TRACE("iface %p, predicate %p, value %#x.\n", iface, predicate, value);

    query = unsafe_impl_from_ID3D11Query((ID3D11Query *)predicate);

    wined3d_mutex_lock();
    wined3d_device_set_predication(device->wined3d_device, query ? query->wined3d_query : NULL, value);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n", iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D11ShaderResourceView(views[i]);

        wined3d_device_set_gs_resource_view(device->wined3d_device, start_slot + i,
                view ? view->wined3d_view : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler = unsafe_impl_from_ID3D11SamplerState(samplers[i]);

        wined3d_device_set_gs_sampler(device->wined3d_device, start_slot + i,
                sampler ? sampler->wined3d_sampler : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_OMSetRenderTargets(ID3D11DeviceContext *iface,
        UINT render_target_view_count, ID3D11RenderTargetView *const *render_target_views,
        ID3D11DepthStencilView *depth_stencil_view)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_depthstencil_view *dsv;
    unsigned int i;

    TRACE("iface %p, render_target_view_count %u, render_target_views %p, depth_stencil_view %p.\n",
            iface, render_target_view_count, render_target_views, depth_stencil_view);

    wined3d_mutex_lock();
    for (i = 0; i < render_target_view_count; ++i)
    {
        struct d3d_rendertarget_view *rtv = unsafe_impl_from_ID3D11RenderTargetView(render_target_views[i]);
        wined3d_device_set_rendertarget_view(device->wined3d_device, i, rtv ? rtv->wined3d_view : NULL, FALSE);
    }
    for (; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
    {
        wined3d_device_set_rendertarget_view(device->wined3d_device, i, NULL, FALSE);
    }

    dsv = unsafe_impl_from_ID3D11DepthStencilView(depth_stencil_view);
    wined3d_device_set_depth_stencil_view(device->wined3d_device, dsv ? dsv->wined3d_view : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_OMSetRenderTargetsAndUnorderedAccessViews(
        ID3D11DeviceContext *iface, UINT render_target_view_count,
        ID3D11RenderTargetView *const *render_target_views, ID3D11DepthStencilView *depth_stencil_view,
        UINT unordered_access_view_start_slot, UINT unordered_access_view_count,
        ID3D11UnorderedAccessView *const *unordered_access_views, const UINT *initial_counts)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, render_target_view_count %u, render_target_views %p, depth_stencil_view %p, "
            "unordered_access_view_start_slot %u, unordered_access_view_count %u, unordered_access_views %p, "
            "initial_counts %p.\n",
            iface, render_target_view_count, render_target_views, depth_stencil_view,
            unordered_access_view_start_slot, unordered_access_view_count, unordered_access_views,
            initial_counts);

    if (render_target_view_count != D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL)
    {
        d3d11_immediate_context_OMSetRenderTargets(iface, render_target_view_count, render_target_views,
                depth_stencil_view);
    }

    if (unordered_access_view_count != D3D11_KEEP_UNORDERED_ACCESS_VIEWS)
    {
        wined3d_mutex_lock();
        for (i = 0; i < unordered_access_view_start_slot; ++i)
        {
            wined3d_device_set_unordered_access_view(device->wined3d_device, i, NULL, ~0u);
        }
        for (i = 0; i < unordered_access_view_count; ++i)
        {
            struct d3d11_unordered_access_view *view
                    = unsafe_impl_from_ID3D11UnorderedAccessView(unordered_access_views[i]);

            wined3d_device_set_unordered_access_view(device->wined3d_device,
                    unordered_access_view_start_slot + i,
                    view ? view->wined3d_view : NULL, initial_counts ? initial_counts[i] : ~0u);
        }
        for (; unordered_access_view_start_slot + i < D3D11_PS_CS_UAV_REGISTER_COUNT; ++i)
        {
            wined3d_device_set_unordered_access_view(device->wined3d_device,
                    unordered_access_view_start_slot + i, NULL, ~0u);
        }
        wined3d_mutex_unlock();
    }
}

static void STDMETHODCALLTYPE d3d11_immediate_context_OMSetBlendState(ID3D11DeviceContext *iface,
        ID3D11BlendState *blend_state, const float blend_factor[4], UINT sample_mask)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    static const float default_blend_factor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    const D3D11_BLEND_DESC *desc;
    int i;

    TRACE("iface %p, blend_state %p, blend_factor %s, sample_mask 0x%08x.\n",
            iface, blend_state, debug_float4(blend_factor), sample_mask);

    if (!blend_factor)
        blend_factor = default_blend_factor;

    wined3d_mutex_lock();
    memcpy(device->blend_factor, blend_factor, 4 * sizeof(*blend_factor));
    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_MULTISAMPLEMASK, sample_mask);
    if (!(device->blend_state = unsafe_impl_from_ID3D11BlendState(blend_state)))
    {
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_ALPHABLENDENABLE, FALSE);
        for (i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        {
            wined3d_device_set_render_state(device->wined3d_device,
                    WINED3D_RS_COLORWRITE(i), D3D11_COLOR_WRITE_ENABLE_ALL);
        }
        wined3d_mutex_unlock();
        return;
    }

    desc = &device->blend_state->desc;
    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_ALPHABLENDENABLE,
            desc->RenderTarget[0].BlendEnable);
    if (desc->RenderTarget[0].BlendEnable)
    {
        const D3D11_RENDER_TARGET_BLEND_DESC *d = &desc->RenderTarget[0];

        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_SRCBLEND, d->SrcBlend);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_DESTBLEND, d->DestBlend);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_BLENDOP, d->BlendOp);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_SEPARATEALPHABLENDENABLE, TRUE);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_SRCBLENDALPHA, d->SrcBlendAlpha);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_DESTBLENDALPHA, d->DestBlendAlpha);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_BLENDOPALPHA, d->BlendOpAlpha);

        if (memcmp(blend_factor, default_blend_factor, sizeof(default_blend_factor))
                && (d->SrcBlend == D3D11_BLEND_BLEND_FACTOR || d->SrcBlend == D3D11_BLEND_INV_BLEND_FACTOR
                || d->DestBlend == D3D11_BLEND_BLEND_FACTOR || d->DestBlend == D3D11_BLEND_INV_BLEND_FACTOR
                || d->SrcBlendAlpha == D3D11_BLEND_BLEND_FACTOR || d->SrcBlendAlpha == D3D11_BLEND_INV_BLEND_FACTOR
                || d->DestBlendAlpha == D3D11_BLEND_BLEND_FACTOR || d->DestBlendAlpha == D3D11_BLEND_INV_BLEND_FACTOR))
            FIXME("Ignoring blend factor %s.\n", debug_float4(blend_factor));
    }
    for (i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
    {
        DWORD src_index = desc->IndependentBlendEnable ? i : 0;

        wined3d_device_set_render_state(device->wined3d_device,
                WINED3D_RS_COLORWRITE(i), desc->RenderTarget[src_index].RenderTargetWriteMask);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_OMSetDepthStencilState(ID3D11DeviceContext *iface,
        ID3D11DepthStencilState *depth_stencil_state, UINT stencil_ref)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    const D3D11_DEPTH_STENCILOP_DESC *front, *back;
    const D3D11_DEPTH_STENCIL_DESC *desc;

    TRACE("iface %p, depth_stencil_state %p, stencil_ref %u.\n",
            iface, depth_stencil_state, stencil_ref);

    wined3d_mutex_lock();
    device->stencil_ref = stencil_ref;
    if (!(device->depth_stencil_state = unsafe_impl_from_ID3D11DepthStencilState(depth_stencil_state)))
    {
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_ZENABLE, TRUE);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_ZWRITEENABLE, D3D11_DEPTH_WRITE_MASK_ALL);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_ZFUNC, D3D11_COMPARISON_LESS);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_STENCILENABLE, FALSE);
        wined3d_mutex_unlock();
        return;
    }

    desc = &device->depth_stencil_state->desc;

    front = &desc->FrontFace;
    back = &desc->BackFace;

    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_ZENABLE, desc->DepthEnable);
    if (desc->DepthEnable)
    {
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_ZWRITEENABLE, desc->DepthWriteMask);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_ZFUNC, desc->DepthFunc);
    }

    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_STENCILENABLE, desc->StencilEnable);
    if (desc->StencilEnable)
    {
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_STENCILMASK, desc->StencilReadMask);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_STENCILWRITEMASK, desc->StencilWriteMask);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_STENCILREF, stencil_ref);

        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_STENCILFAIL, front->StencilFailOp);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_STENCILZFAIL, front->StencilDepthFailOp);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_STENCILPASS, front->StencilPassOp);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_STENCILFUNC, front->StencilFunc);
        if (front->StencilFailOp != back->StencilFailOp
                || front->StencilDepthFailOp != back->StencilDepthFailOp
                || front->StencilPassOp != back->StencilPassOp
                || front->StencilFunc != back->StencilFunc)
        {
            wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_TWOSIDEDSTENCILMODE, TRUE);
            wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_BACK_STENCILFAIL, back->StencilFailOp);
            wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_BACK_STENCILZFAIL,
                    back->StencilDepthFailOp);
            wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_BACK_STENCILPASS, back->StencilPassOp);
            wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_BACK_STENCILFUNC, back->StencilFunc);
        }
        else
        {
            wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_TWOSIDEDSTENCILMODE, FALSE);
        }
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_SOSetTargets(ID3D11DeviceContext *iface, UINT buffer_count,
        ID3D11Buffer *const *buffers, const UINT *offsets)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int count, i;

    TRACE("iface %p, buffer_count %u, buffers %p, offsets %p.\n", iface, buffer_count, buffers, offsets);

    count = min(buffer_count, D3D11_SO_BUFFER_SLOT_COUNT);
    wined3d_mutex_lock();
    for (i = 0; i < count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D11Buffer(buffers[i]);

        wined3d_device_set_stream_output(device->wined3d_device, i,
                buffer ? buffer->wined3d_buffer : NULL, offsets ? offsets[i] : 0);
    }
    for (; i < D3D11_SO_BUFFER_SLOT_COUNT; ++i)
    {
        wined3d_device_set_stream_output(device->wined3d_device, i, NULL, 0);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawAuto(ID3D11DeviceContext *iface)
{
    FIXME("iface %p stub!\n", iface);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawIndexedInstancedIndirect(ID3D11DeviceContext *iface,
        ID3D11Buffer *buffer, UINT offset)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_buffer *d3d_buffer;

    TRACE("iface %p, buffer %p, offset %u.\n", iface, buffer, offset);

    d3d_buffer = unsafe_impl_from_ID3D11Buffer(buffer);

    wined3d_mutex_lock();
    wined3d_device_draw_indexed_primitive_instanced_indirect(device->wined3d_device,
            d3d_buffer->wined3d_buffer, offset);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DrawInstancedIndirect(ID3D11DeviceContext *iface,
        ID3D11Buffer *buffer, UINT offset)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_buffer *d3d_buffer;

    TRACE("iface %p, buffer %p, offset %u.\n", iface, buffer, offset);

    d3d_buffer = unsafe_impl_from_ID3D11Buffer(buffer);

    wined3d_mutex_lock();
    wined3d_device_draw_primitive_instanced_indirect(device->wined3d_device,
            d3d_buffer->wined3d_buffer, offset);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_Dispatch(ID3D11DeviceContext *iface,
        UINT thread_group_count_x, UINT thread_group_count_y, UINT thread_group_count_z)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, thread_group_count_x %u, thread_group_count_y %u, thread_group_count_z %u.\n",
            iface, thread_group_count_x, thread_group_count_y, thread_group_count_z);

    wined3d_mutex_lock();
    wined3d_device_dispatch_compute(device->wined3d_device,
            thread_group_count_x, thread_group_count_y, thread_group_count_z);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DispatchIndirect(ID3D11DeviceContext *iface,
        ID3D11Buffer *buffer, UINT offset)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_buffer *buffer_impl;

    TRACE("iface %p, buffer %p, offset %u.\n", iface, buffer, offset);

    buffer_impl = unsafe_impl_from_ID3D11Buffer(buffer);

    wined3d_mutex_lock();
    wined3d_device_dispatch_compute_indirect(device->wined3d_device,
            buffer_impl->wined3d_buffer, offset);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_RSSetState(ID3D11DeviceContext *iface,
        ID3D11RasterizerState *rasterizer_state)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_rasterizer_state *rasterizer_state_impl;
    const D3D11_RASTERIZER_DESC *desc;
    union {DWORD d; float f;} tmpfloat;

    TRACE("iface %p, rasterizer_state %p.\n", iface, rasterizer_state);

    wined3d_mutex_lock();
    if (!(rasterizer_state_impl = unsafe_impl_from_ID3D11RasterizerState(rasterizer_state)))
    {
        wined3d_device_set_rasterizer_state(device->wined3d_device, NULL);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_FILLMODE, WINED3D_FILL_SOLID);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_CULLMODE, WINED3D_CULL_BACK);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_SCISSORTESTENABLE, FALSE);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_MULTISAMPLEANTIALIAS, FALSE);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_ANTIALIASEDLINEENABLE, FALSE);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_DEPTHBIAS, 0);
        tmpfloat.f = 0.0f;
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_DEPTHBIASCLAMP, tmpfloat.d);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_SLOPESCALEDEPTHBIAS, 0);
        wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_DEPTHCLIP, TRUE);
        wined3d_mutex_unlock();
        return;
    }

    wined3d_device_set_rasterizer_state(device->wined3d_device, rasterizer_state_impl->wined3d_state);

    desc = &rasterizer_state_impl->desc;
    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_FILLMODE, desc->FillMode);
    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_CULLMODE, desc->CullMode);

    /* OpenGL style depth bias. */
    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_DEPTHBIAS, desc->DepthBias);
    tmpfloat.f = desc->DepthBiasClamp;
    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_DEPTHBIASCLAMP, tmpfloat.d);
    tmpfloat.f = desc->SlopeScaledDepthBias;
    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_SLOPESCALEDEPTHBIAS, tmpfloat.d);
    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_DEPTHCLIP, (desc->DepthClipEnable != FALSE));

    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_SCISSORTESTENABLE, desc->ScissorEnable);
    wined3d_device_set_render_state(device->wined3d_device, WINED3D_RS_MULTISAMPLEANTIALIAS, desc->MultisampleEnable);
    wined3d_device_set_render_state(device->wined3d_device,
            WINED3D_RS_ANTIALIASEDLINEENABLE, desc->AntialiasedLineEnable);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_RSSetViewports(ID3D11DeviceContext *iface,
        UINT viewport_count, const D3D11_VIEWPORT *viewports)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_viewport wined3d_vp;

    TRACE("iface %p, viewport_count %u, viewports %p.\n", iface, viewport_count, viewports);

    if (viewport_count > 1)
        FIXME("Multiple viewports not implemented.\n");

    if (!viewport_count)
        return;

    wined3d_vp.x = viewports[0].TopLeftX;
    wined3d_vp.y = viewports[0].TopLeftY;
    wined3d_vp.width = viewports[0].Width;
    wined3d_vp.height = viewports[0].Height;
    wined3d_vp.min_z = viewports[0].MinDepth;
    wined3d_vp.max_z = viewports[0].MaxDepth;

    wined3d_mutex_lock();
    wined3d_device_set_viewport(device->wined3d_device, &wined3d_vp);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_RSSetScissorRects(ID3D11DeviceContext *iface,
        UINT rect_count, const D3D11_RECT *rects)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, rect_count %u, rects %p.\n", iface, rect_count, rects);

    if (rect_count > 1)
        FIXME("Multiple scissor rects not implemented.\n");

    if (!rect_count)
        return;

    wined3d_mutex_lock();
    wined3d_device_set_scissor_rect(device->wined3d_device, rects);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CopySubresourceRegion(ID3D11DeviceContext *iface,
        ID3D11Resource *dst_resource, UINT dst_subresource_idx, UINT dst_x, UINT dst_y, UINT dst_z,
        ID3D11Resource *src_resource, UINT src_subresource_idx, const D3D11_BOX *src_box)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_resource *wined3d_dst_resource, *wined3d_src_resource;
    struct wined3d_box wined3d_src_box;

    TRACE("iface %p, dst_resource %p, dst_subresource_idx %u, dst_x %u, dst_y %u, dst_z %u, "
            "src_resource %p, src_subresource_idx %u, src_box %p.\n",
            iface, dst_resource, dst_subresource_idx, dst_x, dst_y, dst_z,
            src_resource, src_subresource_idx, src_box);

    if (src_box)
        wined3d_box_set(&wined3d_src_box, src_box->left, src_box->top,
                src_box->right, src_box->bottom, src_box->front, src_box->back);

    wined3d_dst_resource = wined3d_resource_from_d3d11_resource(dst_resource);
    wined3d_src_resource = wined3d_resource_from_d3d11_resource(src_resource);
    wined3d_mutex_lock();
    wined3d_device_copy_sub_resource_region(device->wined3d_device, wined3d_dst_resource, dst_subresource_idx,
            dst_x, dst_y, dst_z, wined3d_src_resource, src_subresource_idx, src_box ? &wined3d_src_box : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CopyResource(ID3D11DeviceContext *iface,
        ID3D11Resource *dst_resource, ID3D11Resource *src_resource)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_resource *wined3d_dst_resource, *wined3d_src_resource;

    TRACE("iface %p, dst_resource %p, src_resource %p.\n", iface, dst_resource, src_resource);

    wined3d_dst_resource = wined3d_resource_from_d3d11_resource(dst_resource);
    wined3d_src_resource = wined3d_resource_from_d3d11_resource(src_resource);
    wined3d_mutex_lock();
    wined3d_device_copy_resource(device->wined3d_device, wined3d_dst_resource, wined3d_src_resource);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_UpdateSubresource(ID3D11DeviceContext *iface,
        ID3D11Resource *resource, UINT subresource_idx, const D3D11_BOX *box,
        const void *data, UINT row_pitch, UINT depth_pitch)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_resource *wined3d_resource;
    struct wined3d_box wined3d_box;

    TRACE("iface %p, resource %p, subresource_idx %u, box %p, data %p, row_pitch %u, depth_pitch %u.\n",
            iface, resource, subresource_idx, box, data, row_pitch, depth_pitch);

    if (box)
        wined3d_box_set(&wined3d_box, box->left, box->top, box->right, box->bottom, box->front, box->back);

    wined3d_resource = wined3d_resource_from_d3d11_resource(resource);
    wined3d_mutex_lock();
    wined3d_device_update_sub_resource(device->wined3d_device, wined3d_resource,
            subresource_idx, box ? &wined3d_box : NULL, data, row_pitch, depth_pitch);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CopyStructureCount(ID3D11DeviceContext *iface,
        ID3D11Buffer *dst_buffer, UINT dst_offset, ID3D11UnorderedAccessView *src_view)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d11_unordered_access_view *uav;
    struct d3d_buffer *buffer_impl;

    TRACE("iface %p, dst_buffer %p, dst_offset %u, src_view %p.\n",
            iface, dst_buffer, dst_offset, src_view);

    if (!dst_buffer || !src_view)
        return;

    buffer_impl = unsafe_impl_from_ID3D11Buffer(dst_buffer);
    uav = unsafe_impl_from_ID3D11UnorderedAccessView(src_view);

    wined3d_mutex_lock();
    wined3d_device_copy_uav_counter(device->wined3d_device,
            buffer_impl->wined3d_buffer, dst_offset, uav->wined3d_view);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearRenderTargetView(ID3D11DeviceContext *iface,
        ID3D11RenderTargetView *render_target_view, const float color_rgba[4])
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_rendertarget_view *view = unsafe_impl_from_ID3D11RenderTargetView(render_target_view);
    const struct wined3d_color color = {color_rgba[0], color_rgba[1], color_rgba[2], color_rgba[3]};
    HRESULT hr;

    TRACE("iface %p, render_target_view %p, color_rgba %s.\n",
            iface, render_target_view, debug_float4(color_rgba));

    if (!view)
        return;

    wined3d_mutex_lock();
    if (FAILED(hr = wined3d_device_clear_rendertarget_view(device->wined3d_device, view->wined3d_view, NULL,
            WINED3DCLEAR_TARGET, &color, 0.0f, 0)))
        ERR("Failed to clear view, hr %#x.\n", hr);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearUnorderedAccessViewUint(ID3D11DeviceContext *iface,
        ID3D11UnorderedAccessView *unordered_access_view, const UINT values[4])
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d11_unordered_access_view *view;

    TRACE("iface %p, unordered_access_view %p, values {%u, %u, %u, %u}.\n",
            iface, unordered_access_view, values[0], values[1], values[2], values[3]);

    view = unsafe_impl_from_ID3D11UnorderedAccessView(unordered_access_view);
    wined3d_mutex_lock();
    wined3d_device_clear_unordered_access_view_uint(device->wined3d_device,
            view->wined3d_view, (const struct wined3d_uvec4 *)values);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearUnorderedAccessViewFloat(ID3D11DeviceContext *iface,
        ID3D11UnorderedAccessView *unordered_access_view, const float values[4])
{
    FIXME("iface %p, unordered_access_view %p, values %s stub!\n",
            iface, unordered_access_view, debug_float4(values));
}

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearDepthStencilView(ID3D11DeviceContext *iface,
        ID3D11DepthStencilView *depth_stencil_view, UINT flags, FLOAT depth, UINT8 stencil)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_depthstencil_view *view = unsafe_impl_from_ID3D11DepthStencilView(depth_stencil_view);
    DWORD wined3d_flags;
    HRESULT hr;

    TRACE("iface %p, depth_stencil_view %p, flags %#x, depth %.8e, stencil %u.\n",
            iface, depth_stencil_view, flags, depth, stencil);

    if (!view)
        return;

    wined3d_flags = wined3d_clear_flags_from_d3d11_clear_flags(flags);

    wined3d_mutex_lock();
    if (FAILED(hr = wined3d_device_clear_rendertarget_view(device->wined3d_device, view->wined3d_view, NULL,
            wined3d_flags, NULL, depth, stencil)))
        ERR("Failed to clear view, hr %#x.\n", hr);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GenerateMips(ID3D11DeviceContext *iface,
        ID3D11ShaderResourceView *shader_view)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D11ShaderResourceView(shader_view);

    TRACE("iface %p, shader_view %p.\n", iface, shader_view);

    if (!view)
        return;

    wined3d_mutex_lock();
    wined3d_device_generate_mips_view(device->wined3d_device, view->wined3d_view);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_SetResourceMinLOD(ID3D11DeviceContext *iface,
        ID3D11Resource *resource, FLOAT min_lod)
{
    FIXME("iface %p, resource %p, min_lod %f stub!\n", iface, resource, min_lod);
}

static FLOAT STDMETHODCALLTYPE d3d11_immediate_context_GetResourceMinLOD(ID3D11DeviceContext *iface,
        ID3D11Resource *resource)
{
    FIXME("iface %p, resource %p stub!\n", iface, resource);

    return 0.0f;
}

static void STDMETHODCALLTYPE d3d11_immediate_context_ResolveSubresource(ID3D11DeviceContext *iface,
        ID3D11Resource *dst_resource, UINT dst_subresource_idx,
        ID3D11Resource *src_resource, UINT src_subresource_idx,
        DXGI_FORMAT format)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_resource *wined3d_dst_resource, *wined3d_src_resource;

    TRACE("iface %p, dst_resource %p, dst_subresource_idx %u, src_resource %p, src_subresource_idx %u, "
            "format %s stub!\n",
            iface, dst_resource, dst_subresource_idx, src_resource, src_subresource_idx,
            debug_dxgi_format(format));

    wined3d_dst_resource = wined3d_resource_from_d3d11_resource(dst_resource);
    wined3d_src_resource = wined3d_resource_from_d3d11_resource(src_resource);

    /* Multisampled textures are not supported yet, so simply copy the sub resource */
    wined3d_mutex_lock();
    wined3d_device_copy_sub_resource_region(device->wined3d_device, wined3d_dst_resource, dst_subresource_idx,
            0, 0, 0, wined3d_src_resource, src_subresource_idx, NULL);
    wined3d_mutex_unlock();
}

static struct d3d11_state *state_capture(ID3D11DeviceContext *context)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(context);
    unsigned int vp_count = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
    struct d3d11_state *stateblock;
    int i;

    if (!(stateblock = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*stateblock))))
        return NULL;

    ID3D11DeviceContext_VSGetShader(context, &stateblock->vs, NULL, 0);
    ID3D11DeviceContext_VSGetSamplers(context, 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, stateblock->vs_samplers);
    ID3D11DeviceContext_VSGetShaderResources(context, 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, stateblock->vs_resources);
    ID3D11DeviceContext_VSGetConstantBuffers(context, 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, stateblock->vs_cbs);

    ID3D11DeviceContext_GSGetShader(context, &stateblock->gs, NULL, 0);
    ID3D11DeviceContext_GSGetSamplers(context, 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, stateblock->gs_samplers);
    ID3D11DeviceContext_GSGetShaderResources(context, 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, stateblock->gs_resources);
    ID3D11DeviceContext_GSGetConstantBuffers(context, 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, stateblock->gs_cbs);

    ID3D11DeviceContext_PSGetShader(context, &stateblock->ps, NULL, 0);
    ID3D11DeviceContext_PSGetSamplers(context, 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, stateblock->ps_samplers);
    ID3D11DeviceContext_PSGetShaderResources(context, 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, stateblock->ps_resources);
    ID3D11DeviceContext_PSGetConstantBuffers(context, 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, stateblock->ps_cbs);

    ID3D11DeviceContext_HSGetShader(context, &stateblock->hs, NULL, 0);
    ID3D11DeviceContext_HSGetSamplers(context, 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, stateblock->hs_samplers);
    ID3D11DeviceContext_HSGetShaderResources(context, 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, stateblock->hs_resources);
    ID3D11DeviceContext_HSGetConstantBuffers(context, 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, stateblock->hs_cbs);

    ID3D11DeviceContext_DSGetShader(context, &stateblock->ds, NULL, 0);
    ID3D11DeviceContext_DSGetSamplers(context, 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, stateblock->ds_samplers);
    ID3D11DeviceContext_DSGetShaderResources(context, 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, stateblock->ds_resources);
    ID3D11DeviceContext_DSGetConstantBuffers(context, 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, stateblock->ds_cbs);

    ID3D11DeviceContext_IAGetVertexBuffers(context, 0, MAX_WINED3D_STREAMS, stateblock->vbs, stateblock->vb_strides, stateblock->vb_offsets);
    ID3D11DeviceContext_IAGetIndexBuffer(context, &stateblock->ib, &stateblock->ib_format, &stateblock->ib_offset);
    ID3D11DeviceContext_IAGetInputLayout(context, &stateblock->il);
    ID3D11DeviceContext_IAGetPrimitiveTopology(context, &stateblock->topology);

    ID3D11DeviceContext_OMGetRenderTargets(context, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, stateblock->rtvs, &stateblock->dsv);
    ID3D11DeviceContext_OMGetDepthStencilState(context, &stateblock->dss, &stateblock->stencil_ref);
    ID3D11DeviceContext_OMGetBlendState(context, &stateblock->bs, stateblock->blend_factor, &stateblock->sample_mask);

    ID3D11DeviceContext_RSGetViewports(context, &vp_count, stateblock->vps);
    ID3D11DeviceContext_RSGetScissorRects(context, &vp_count, stateblock->scissor_rects);
    ID3D11DeviceContext_RSGetState(context, &stateblock->rs);

    ID3D11DeviceContext_SOGetTargets(context, D3D11_SO_BUFFER_SLOT_COUNT, stateblock->so_buffers);
    /* For some reason the d3d11 get function is missing the offset parameter */
    for (i = 0; i < D3D11_SO_BUFFER_SLOT_COUNT; i++)
        wined3d_device_get_stream_output(device->wined3d_device, i, &stateblock->so_offsets[i]);

    ID3D11DeviceContext_GetPredication(context, &stateblock->predicate, &stateblock->predicate_value);

    return stateblock;
}

static void state_apply(ID3D11DeviceContext *context, struct d3d11_state *stateblock)
{
    static DWORD warn_once;
    int i;

    if (!stateblock)
        return;

    if (!warn_once++) FIXME("restoring state is potentially slow and incomplete!\n");

    ID3D11DeviceContext_VSSetShader(context, stateblock->vs, NULL, 0);
    ID3D11DeviceContext_VSSetSamplers(context, 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, stateblock->vs_samplers);
    ID3D11DeviceContext_VSSetShaderResources(context, 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, stateblock->vs_resources);
    ID3D11DeviceContext_VSSetConstantBuffers(context, 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, stateblock->vs_cbs);

    if (stateblock->vs) ID3D11VertexShader_Release(stateblock->vs);
    for (i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++)
        if (stateblock->vs_samplers[i]) ID3D11SamplerState_Release(stateblock->vs_samplers[i]);
    for (i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++)
        if (stateblock->vs_resources[i]) ID3D11ShaderResourceView_Release(stateblock->vs_resources[i]);
    for (i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++)
        if (stateblock->vs_cbs[i]) ID3D11Buffer_Release(stateblock->vs_cbs[i]);

    ID3D11DeviceContext_GSSetShader(context, stateblock->gs, NULL, 0);
    ID3D11DeviceContext_GSSetSamplers(context, 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, stateblock->gs_samplers);
    ID3D11DeviceContext_GSSetShaderResources(context, 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, stateblock->gs_resources);
    ID3D11DeviceContext_GSSetConstantBuffers(context, 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, stateblock->gs_cbs);

    if (stateblock->gs) ID3D11GeometryShader_Release(stateblock->gs);
    for (i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++)
        if (stateblock->gs_samplers[i]) ID3D11SamplerState_Release(stateblock->gs_samplers[i]);
    for (i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++)
        if (stateblock->gs_resources[i]) ID3D11ShaderResourceView_Release(stateblock->gs_resources[i]);
    for (i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++)
        if (stateblock->gs_cbs[i]) ID3D11Buffer_Release(stateblock->gs_cbs[i]);

    ID3D11DeviceContext_PSSetShader(context, stateblock->ps, NULL, 0);
    ID3D11DeviceContext_PSSetSamplers(context, 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, stateblock->ps_samplers);
    ID3D11DeviceContext_PSSetShaderResources(context, 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, stateblock->ps_resources);
    ID3D11DeviceContext_PSSetConstantBuffers(context, 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, stateblock->ps_cbs);

    if (stateblock->ps) ID3D11PixelShader_Release(stateblock->ps);
    for (i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++)
        if (stateblock->ps_samplers[i]) ID3D11SamplerState_Release(stateblock->ps_samplers[i]);
    for (i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++)
        if (stateblock->ps_resources[i]) ID3D11ShaderResourceView_Release(stateblock->ps_resources[i]);
    for (i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++)
        if (stateblock->ps_cbs[i]) ID3D11Buffer_Release(stateblock->ps_cbs[i]);

    ID3D11DeviceContext_HSSetShader(context, stateblock->hs, NULL, 0);
    ID3D11DeviceContext_HSSetSamplers(context, 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, stateblock->hs_samplers);
    ID3D11DeviceContext_HSSetShaderResources(context, 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, stateblock->hs_resources);
    ID3D11DeviceContext_HSSetConstantBuffers(context, 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, stateblock->hs_cbs);

    if (stateblock->hs) ID3D11HullShader_Release(stateblock->hs);
    for (i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++)
        if (stateblock->hs_samplers[i]) ID3D11SamplerState_Release(stateblock->hs_samplers[i]);
    for (i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++)
        if (stateblock->hs_resources[i]) ID3D11ShaderResourceView_Release(stateblock->hs_resources[i]);
    for (i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++)
        if (stateblock->hs_cbs[i]) ID3D11Buffer_Release(stateblock->hs_cbs[i]);

    ID3D11DeviceContext_DSSetShader(context, stateblock->ds, NULL, 0);
    ID3D11DeviceContext_DSSetSamplers(context, 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, stateblock->ds_samplers);
    ID3D11DeviceContext_DSSetShaderResources(context, 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, stateblock->ds_resources);
    ID3D11DeviceContext_DSSetConstantBuffers(context, 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, stateblock->ds_cbs);

    if (stateblock->ds) ID3D11DomainShader_Release(stateblock->ds);
    for (i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; i++)
        if (stateblock->ds_samplers[i]) ID3D11SamplerState_Release(stateblock->ds_samplers[i]);
    for (i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; i++)
        if (stateblock->ds_resources[i]) ID3D11ShaderResourceView_Release(stateblock->ds_resources[i]);
    for (i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; i++)
        if (stateblock->ds_cbs[i]) ID3D11Buffer_Release(stateblock->ds_cbs[i]);

    ID3D11DeviceContext_IASetVertexBuffers(context, 0, MAX_WINED3D_STREAMS, stateblock->vbs, stateblock->vb_strides, stateblock->vb_offsets);
    ID3D11DeviceContext_IASetIndexBuffer(context, stateblock->ib, stateblock->ib_format, stateblock->ib_offset);
    ID3D11DeviceContext_IASetInputLayout(context, stateblock->il);
    ID3D11DeviceContext_IASetPrimitiveTopology(context, stateblock->topology);

    for (i = 0; i < MAX_WINED3D_STREAMS; i++)
        if (stateblock->vbs[i]) ID3D11Buffer_Release(stateblock->vbs[i]);
    if (stateblock->ib) ID3D11Buffer_Release(stateblock->ib);
    if (stateblock->il) ID3D11InputLayout_Release(stateblock->il);

    ID3D11DeviceContext_OMSetRenderTargets(context, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, stateblock->rtvs, stateblock->dsv);
    ID3D11DeviceContext_OMSetDepthStencilState(context, stateblock->dss, stateblock->stencil_ref);
    ID3D11DeviceContext_OMSetBlendState(context, stateblock->bs, stateblock->blend_factor, stateblock->sample_mask);

    for (i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
        if (stateblock->rtvs[i]) ID3D11RenderTargetView_Release(stateblock->rtvs[i]);
    if (stateblock->dsv) ID3D11DepthStencilView_Release(stateblock->dsv);
    if (stateblock->dss) ID3D11DepthStencilState_Release(stateblock->dss);
    if (stateblock->bs)  ID3D11BlendState_Release(stateblock->bs);

    ID3D11DeviceContext_RSSetViewports(context, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE, stateblock->vps);
    ID3D11DeviceContext_RSSetScissorRects(context, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE, stateblock->scissor_rects);
    ID3D11DeviceContext_RSSetState(context, stateblock->rs);

    if (stateblock->rs) ID3D11RasterizerState_Release(stateblock->rs);

    ID3D11DeviceContext_SOSetTargets(context, D3D11_SO_BUFFER_SLOT_COUNT, stateblock->so_buffers, stateblock->so_offsets);
    ID3D11DeviceContext_SetPredication(context, stateblock->predicate, stateblock->predicate_value);

    for (i = 0; i < D3D11_SO_BUFFER_SLOT_COUNT; i++)
        if (stateblock->so_buffers[i]) ID3D11Buffer_Release(stateblock->so_buffers[i]);
    if (stateblock->predicate) ID3D11Predicate_Release(stateblock->predicate);

    HeapFree(GetProcessHeap(), 0, stateblock);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_ExecuteCommandList(ID3D11DeviceContext *iface,
        ID3D11CommandList *command_list, BOOL restore_state)
{
    struct d3d11_command_list *cmdlist = unsafe_impl_from_ID3D11CommandList(command_list);
    struct d3d11_state *stateblock = NULL;

    TRACE("iface %p, command_list %p, restore_state %#x.\n", iface, command_list, restore_state);

    if (!cmdlist)
        return;

    wined3d_mutex_lock();
    if (restore_state) stateblock = state_capture(iface);
    exec_deferred_calls(iface, &cmdlist->commands);
    if (restore_state) state_apply(iface, stateblock);
    else ID3D11DeviceContext_ClearState(iface);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_HSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D11ShaderResourceView(views[i]);

        wined3d_device_set_hs_resource_view(device->wined3d_device, start_slot + i,
                view ? view->wined3d_view : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_HSSetShader(ID3D11DeviceContext *iface,
        ID3D11HullShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d11_hull_shader *hs = unsafe_impl_from_ID3D11HullShader(shader);

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances)
        FIXME("Dynamic linking is not implemented yet.\n");

    wined3d_mutex_lock();
    wined3d_device_set_hull_shader(device->wined3d_device, hs ? hs->wined3d_shader : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_HSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler = unsafe_impl_from_ID3D11SamplerState(samplers[i]);

        wined3d_device_set_hs_sampler(device->wined3d_device, start_slot + i,
                sampler ? sampler->wined3d_sampler : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_HSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D11Buffer(buffers[i]);

        wined3d_device_set_hs_cb(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D11ShaderResourceView(views[i]);

        wined3d_device_set_ds_resource_view(device->wined3d_device, start_slot + i,
                view ? view->wined3d_view : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DSSetShader(ID3D11DeviceContext *iface,
        ID3D11DomainShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d11_domain_shader *ds = unsafe_impl_from_ID3D11DomainShader(shader);

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances)
        FIXME("Dynamic linking is not implemented yet.\n");

    wined3d_mutex_lock();
    wined3d_device_set_domain_shader(device->wined3d_device, ds ? ds->wined3d_shader : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler = unsafe_impl_from_ID3D11SamplerState(samplers[i]);

        wined3d_device_set_ds_sampler(device->wined3d_device, start_slot + i,
                sampler ? sampler->wined3d_sampler : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D11Buffer(buffers[i]);

        wined3d_device_set_ds_cb(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D11ShaderResourceView(views[i]);

        wined3d_device_set_cs_resource_view(device->wined3d_device, start_slot + i,
                view ? view->wined3d_view : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetUnorderedAccessViews(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11UnorderedAccessView *const *views, const UINT *initial_counts)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p, initial_counts %p.\n",
            iface, start_slot, view_count, views, initial_counts);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct d3d11_unordered_access_view *view = unsafe_impl_from_ID3D11UnorderedAccessView(views[i]);

        wined3d_device_set_cs_uav(device->wined3d_device, start_slot + i,
                view ? view->wined3d_view : NULL, initial_counts ? initial_counts[i] : ~0u);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetShader(ID3D11DeviceContext *iface,
        ID3D11ComputeShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d11_compute_shader *cs = unsafe_impl_from_ID3D11ComputeShader(shader);

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances)
        FIXME("Dynamic linking is not implemented yet.\n");

    wined3d_mutex_lock();
    wined3d_device_set_compute_shader(device->wined3d_device, cs ? cs->wined3d_shader : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler = unsafe_impl_from_ID3D11SamplerState(samplers[i]);

        wined3d_device_set_cs_sampler(device->wined3d_device, start_slot + i,
                sampler ? sampler->wined3d_sampler : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D11Buffer(buffers[i]);

        wined3d_device_set_cs_cb(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_VSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_vs_cb(device->wined3d_device, start_slot + i)))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        buffers[i] = &buffer_impl->ID3D11Buffer_iface;
        ID3D11Buffer_AddRef(buffers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_PSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct wined3d_shader_resource_view *wined3d_view;
        struct d3d_shader_resource_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_ps_resource_view(device->wined3d_device, start_slot + i)))
        {
            views[i] = NULL;
            continue;
        }

        view_impl = wined3d_shader_resource_view_get_parent(wined3d_view);
        views[i] = &view_impl->ID3D11ShaderResourceView_iface;
        ID3D11ShaderResourceView_AddRef(views[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_PSGetShader(ID3D11DeviceContext *iface,
        ID3D11PixelShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_shader *wined3d_shader;
    struct d3d_pixel_shader *shader_impl;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %p.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances || class_instance_count)
        FIXME("Dynamic linking not implemented yet.\n");

    wined3d_mutex_lock();
    if (!(wined3d_shader = wined3d_device_get_pixel_shader(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *shader = NULL;
        return;
    }

    shader_impl = wined3d_shader_get_parent(wined3d_shader);
    wined3d_mutex_unlock();
    *shader = &shader_impl->ID3D11PixelShader_iface;
    ID3D11PixelShader_AddRef(*shader);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_PSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct wined3d_sampler *wined3d_sampler;
        struct d3d_sampler_state *sampler_impl;

        if (!(wined3d_sampler = wined3d_device_get_ps_sampler(device->wined3d_device, start_slot + i)))
        {
            samplers[i] = NULL;
            continue;
        }

        sampler_impl = wined3d_sampler_get_parent(wined3d_sampler);
        samplers[i] = &sampler_impl->ID3D11SamplerState_iface;
        ID3D11SamplerState_AddRef(samplers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_VSGetShader(ID3D11DeviceContext *iface,
        ID3D11VertexShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_vertex_shader *shader_impl;
    struct wined3d_shader *wined3d_shader;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %p.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances || class_instance_count)
        FIXME("Dynamic linking not implemented yet.\n");

    wined3d_mutex_lock();
    if (!(wined3d_shader = wined3d_device_get_vertex_shader(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *shader = NULL;
        return;
    }

    shader_impl = wined3d_shader_get_parent(wined3d_shader);
    wined3d_mutex_unlock();
    *shader = &shader_impl->ID3D11VertexShader_iface;
    ID3D11VertexShader_AddRef(*shader);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_PSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_ps_cb(device->wined3d_device, start_slot + i)))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        buffers[i] = &buffer_impl->ID3D11Buffer_iface;
        ID3D11Buffer_AddRef(buffers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_IAGetInputLayout(ID3D11DeviceContext *iface,
        ID3D11InputLayout **input_layout)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_vertex_declaration *wined3d_declaration;
    struct d3d_input_layout *input_layout_impl;

    TRACE("iface %p, input_layout %p.\n", iface, input_layout);

    wined3d_mutex_lock();
    if (!(wined3d_declaration = wined3d_device_get_vertex_declaration(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *input_layout = NULL;
        return;
    }

    input_layout_impl = wined3d_vertex_declaration_get_parent(wined3d_declaration);
    wined3d_mutex_unlock();
    *input_layout = &input_layout_impl->ID3D11InputLayout_iface;
    ID3D11InputLayout_AddRef(*input_layout);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_IAGetVertexBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers, UINT *strides, UINT *offsets)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p, strides %p, offsets %p.\n",
            iface, start_slot, buffer_count, buffers, strides, offsets);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer = NULL;
        struct d3d_buffer *buffer_impl;

        if (FAILED(wined3d_device_get_stream_source(device->wined3d_device, start_slot + i,
                &wined3d_buffer, &offsets[i], &strides[i])))
        {
            FIXME("Failed to get vertex buffer %u.\n", start_slot + i);
            if (strides)
                strides[i] = 0;
            if (offsets)
                offsets[i] = 0;
        }

        if (!wined3d_buffer)
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        ID3D11Buffer_AddRef(buffers[i] = &buffer_impl->ID3D11Buffer_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_IAGetIndexBuffer(ID3D11DeviceContext *iface,
        ID3D11Buffer **buffer, DXGI_FORMAT *format, UINT *offset)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    enum wined3d_format_id wined3d_format;
    struct wined3d_buffer *wined3d_buffer;
    struct d3d_buffer *buffer_impl;

    TRACE("iface %p, buffer %p, format %p, offset %p.\n", iface, buffer, format, offset);

    wined3d_mutex_lock();
    wined3d_buffer = wined3d_device_get_index_buffer(device->wined3d_device, &wined3d_format, offset);
    *format = dxgi_format_from_wined3dformat(wined3d_format);
    if (!wined3d_buffer)
    {
        wined3d_mutex_unlock();
        *buffer = NULL;
        return;
    }

    buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
    wined3d_mutex_unlock();
    ID3D11Buffer_AddRef(*buffer = &buffer_impl->ID3D11Buffer_iface);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_gs_cb(device->wined3d_device, start_slot + i)))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        buffers[i] = &buffer_impl->ID3D11Buffer_iface;
        ID3D11Buffer_AddRef(buffers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GSGetShader(ID3D11DeviceContext *iface,
        ID3D11GeometryShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_geometry_shader *shader_impl;
    struct wined3d_shader *wined3d_shader;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %p.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances || class_instance_count)
        FIXME("Dynamic linking not implemented yet.\n");

    wined3d_mutex_lock();
    if (!(wined3d_shader = wined3d_device_get_geometry_shader(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *shader = NULL;
        return;
    }

    shader_impl = wined3d_shader_get_parent(wined3d_shader);
    wined3d_mutex_unlock();
    *shader = &shader_impl->ID3D11GeometryShader_iface;
    ID3D11GeometryShader_AddRef(*shader);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_IAGetPrimitiveTopology(ID3D11DeviceContext *iface,
        D3D11_PRIMITIVE_TOPOLOGY *topology)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    enum wined3d_primitive_type primitive_type;
    unsigned int patch_vertex_count;

    TRACE("iface %p, topology %p.\n", iface, topology);

    wined3d_mutex_lock();
    wined3d_device_get_primitive_type(device->wined3d_device, &primitive_type, &patch_vertex_count);
    wined3d_mutex_unlock();

    d3d11_primitive_topology_from_wined3d_primitive_type(primitive_type, patch_vertex_count, topology);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_VSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n", iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct wined3d_shader_resource_view *wined3d_view;
        struct d3d_shader_resource_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_vs_resource_view(device->wined3d_device, start_slot + i)))
        {
            views[i] = NULL;
            continue;
        }

        view_impl = wined3d_shader_resource_view_get_parent(wined3d_view);
        views[i] = &view_impl->ID3D11ShaderResourceView_iface;
        ID3D11ShaderResourceView_AddRef(views[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_VSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct wined3d_sampler *wined3d_sampler;
        struct d3d_sampler_state *sampler_impl;

        if (!(wined3d_sampler = wined3d_device_get_vs_sampler(device->wined3d_device, start_slot + i)))
        {
            samplers[i] = NULL;
            continue;
        }

        sampler_impl = wined3d_sampler_get_parent(wined3d_sampler);
        samplers[i] = &sampler_impl->ID3D11SamplerState_iface;
        ID3D11SamplerState_AddRef(samplers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GetPredication(ID3D11DeviceContext *iface,
        ID3D11Predicate **predicate, BOOL *value)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_query *wined3d_predicate;
    struct d3d_query *predicate_impl;

    TRACE("iface %p, predicate %p, value %p.\n", iface, predicate, value);

    wined3d_mutex_lock();
    if (!(wined3d_predicate = wined3d_device_get_predication(device->wined3d_device, value)))
    {
        wined3d_mutex_unlock();
        *predicate = NULL;
        return;
    }

    predicate_impl = wined3d_query_get_parent(wined3d_predicate);
    wined3d_mutex_unlock();
    *predicate = (ID3D11Predicate *)&predicate_impl->ID3D11Query_iface;
    ID3D11Predicate_AddRef(*predicate);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n", iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct wined3d_shader_resource_view *wined3d_view;
        struct d3d_shader_resource_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_gs_resource_view(device->wined3d_device, start_slot + i)))
        {
            views[i] = NULL;
            continue;
        }

        view_impl = wined3d_shader_resource_view_get_parent(wined3d_view);
        views[i] = &view_impl->ID3D11ShaderResourceView_iface;
        ID3D11ShaderResourceView_AddRef(views[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_GSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler_impl;
        struct wined3d_sampler *wined3d_sampler;

        if (!(wined3d_sampler = wined3d_device_get_gs_sampler(device->wined3d_device, start_slot + i)))
        {
            samplers[i] = NULL;
            continue;
        }

        sampler_impl = wined3d_sampler_get_parent(wined3d_sampler);
        samplers[i] = &sampler_impl->ID3D11SamplerState_iface;
        ID3D11SamplerState_AddRef(samplers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_OMGetRenderTargets(ID3D11DeviceContext *iface,
        UINT render_target_view_count, ID3D11RenderTargetView **render_target_views,
        ID3D11DepthStencilView **depth_stencil_view)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_rendertarget_view *wined3d_view;

    TRACE("iface %p, render_target_view_count %u, render_target_views %p, depth_stencil_view %p.\n",
            iface, render_target_view_count, render_target_views, depth_stencil_view);

    wined3d_mutex_lock();
    if (render_target_views)
    {
        struct d3d_rendertarget_view *view_impl;
        unsigned int i;

        for (i = 0; i < render_target_view_count; ++i)
        {
            if (!(wined3d_view = wined3d_device_get_rendertarget_view(device->wined3d_device, i))
                    || !(view_impl = wined3d_rendertarget_view_get_parent(wined3d_view)))
            {
                render_target_views[i] = NULL;
                continue;
            }

            render_target_views[i] = &view_impl->ID3D11RenderTargetView_iface;
            ID3D11RenderTargetView_AddRef(render_target_views[i]);
        }
    }

    if (depth_stencil_view)
    {
        struct d3d_depthstencil_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_depth_stencil_view(device->wined3d_device))
                || !(view_impl = wined3d_rendertarget_view_get_parent(wined3d_view)))
        {
            *depth_stencil_view = NULL;
        }
        else
        {
            *depth_stencil_view = &view_impl->ID3D11DepthStencilView_iface;
            ID3D11DepthStencilView_AddRef(*depth_stencil_view);
        }
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_OMGetRenderTargetsAndUnorderedAccessViews(
        ID3D11DeviceContext *iface,
        UINT render_target_view_count, ID3D11RenderTargetView **render_target_views,
        ID3D11DepthStencilView **depth_stencil_view,
        UINT unordered_access_view_start_slot, UINT unordered_access_view_count,
        ID3D11UnorderedAccessView **unordered_access_views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_unordered_access_view *wined3d_view;
    struct d3d11_unordered_access_view *view_impl;
    unsigned int i;

    TRACE("iface %p, render_target_view_count %u, render_target_views %p, depth_stencil_view %p, "
            "unordered_access_view_start_slot %u, unordered_access_view_count %u, "
            "unordered_access_views %p.\n",
            iface, render_target_view_count, render_target_views, depth_stencil_view,
            unordered_access_view_start_slot, unordered_access_view_count, unordered_access_views);

    if (render_target_views || depth_stencil_view)
        d3d11_immediate_context_OMGetRenderTargets(iface, render_target_view_count,
                render_target_views, depth_stencil_view);

    if (unordered_access_views)
    {
        wined3d_mutex_lock();
        for (i = 0; i < unordered_access_view_count; ++i)
        {
            if (!(wined3d_view = wined3d_device_get_unordered_access_view(device->wined3d_device,
                    unordered_access_view_start_slot + i)))
            {
                unordered_access_views[i] = NULL;
                continue;
            }

            view_impl = wined3d_unordered_access_view_get_parent(wined3d_view);
            unordered_access_views[i] = &view_impl->ID3D11UnorderedAccessView_iface;
            ID3D11UnorderedAccessView_AddRef(unordered_access_views[i]);
        }
        wined3d_mutex_unlock();
    }
}

static void STDMETHODCALLTYPE d3d11_immediate_context_OMGetBlendState(ID3D11DeviceContext *iface,
        ID3D11BlendState **blend_state, FLOAT blend_factor[4], UINT *sample_mask)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, blend_state %p, blend_factor %p, sample_mask %p.\n",
            iface, blend_state, blend_factor, sample_mask);

    if ((*blend_state = device->blend_state ? &device->blend_state->ID3D11BlendState_iface : NULL))
        ID3D11BlendState_AddRef(*blend_state);
    wined3d_mutex_lock();
    memcpy(blend_factor, device->blend_factor, 4 * sizeof(*blend_factor));
    *sample_mask = wined3d_device_get_render_state(device->wined3d_device, WINED3D_RS_MULTISAMPLEMASK);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_OMGetDepthStencilState(ID3D11DeviceContext *iface,
        ID3D11DepthStencilState **depth_stencil_state, UINT *stencil_ref)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, depth_stencil_state %p, stencil_ref %p.\n",
            iface, depth_stencil_state, stencil_ref);

    if ((*depth_stencil_state = device->depth_stencil_state
            ? &device->depth_stencil_state->ID3D11DepthStencilState_iface : NULL))
        ID3D11DepthStencilState_AddRef(*depth_stencil_state);
    *stencil_ref = device->stencil_ref;
}

static void STDMETHODCALLTYPE d3d11_immediate_context_SOGetTargets(ID3D11DeviceContext *iface,
        UINT buffer_count, ID3D11Buffer **buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, buffer_count %u, buffers %p.\n", iface, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_stream_output(device->wined3d_device, i, NULL)))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        buffers[i] = &buffer_impl->ID3D11Buffer_iface;
        ID3D11Buffer_AddRef(buffers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_RSGetState(ID3D11DeviceContext *iface,
        ID3D11RasterizerState **rasterizer_state)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d_rasterizer_state *rasterizer_state_impl;
    struct wined3d_rasterizer_state *wined3d_state;

    TRACE("iface %p, rasterizer_state %p.\n", iface, rasterizer_state);

    wined3d_mutex_lock();
    if ((wined3d_state = wined3d_device_get_rasterizer_state(device->wined3d_device)))
    {
        rasterizer_state_impl = wined3d_rasterizer_state_get_parent(wined3d_state);
        ID3D11RasterizerState_AddRef(*rasterizer_state = &rasterizer_state_impl->ID3D11RasterizerState_iface);
    }
    else
    {
        *rasterizer_state = NULL;
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_RSGetViewports(ID3D11DeviceContext *iface,
        UINT *viewport_count, D3D11_VIEWPORT *viewports)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct wined3d_viewport wined3d_vp;

    TRACE("iface %p, viewport_count %p, viewports %p.\n", iface, viewport_count, viewports);

    if (!viewports)
    {
        *viewport_count = 1;
        return;
    }

    if (!*viewport_count)
        return;

    wined3d_mutex_lock();
    wined3d_device_get_viewport(device->wined3d_device, &wined3d_vp);
    wined3d_mutex_unlock();

    viewports[0].TopLeftX = wined3d_vp.x;
    viewports[0].TopLeftY = wined3d_vp.y;
    viewports[0].Width = wined3d_vp.width;
    viewports[0].Height = wined3d_vp.height;
    viewports[0].MinDepth = wined3d_vp.min_z;
    viewports[0].MaxDepth = wined3d_vp.max_z;

    if (*viewport_count > 1)
        memset(&viewports[1], 0, (*viewport_count - 1) * sizeof(*viewports));
}

static void STDMETHODCALLTYPE d3d11_immediate_context_RSGetScissorRects(ID3D11DeviceContext *iface,
        UINT *rect_count, D3D11_RECT *rects)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);

    TRACE("iface %p, rect_count %p, rects %p.\n", iface, rect_count, rects);

    if (!rects)
    {
        *rect_count = 1;
        return;
    }

    if (!*rect_count)
        return;

    wined3d_mutex_lock();
    wined3d_device_get_scissor_rect(device->wined3d_device, rects);
    wined3d_mutex_unlock();
    if (*rect_count > 1)
        memset(&rects[1], 0, (*rect_count - 1) * sizeof(*rects));
}

static void STDMETHODCALLTYPE d3d11_immediate_context_HSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n", iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct wined3d_shader_resource_view *wined3d_view;
        struct d3d_shader_resource_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_hs_resource_view(device->wined3d_device, start_slot + i)))
        {
            views[i] = NULL;
            continue;
        }

        view_impl = wined3d_shader_resource_view_get_parent(wined3d_view);
        ID3D11ShaderResourceView_AddRef(views[i] = &view_impl->ID3D11ShaderResourceView_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_HSGetShader(ID3D11DeviceContext *iface,
        ID3D11HullShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d11_hull_shader *shader_impl;
    struct wined3d_shader *wined3d_shader;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %p.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances || class_instance_count)
        FIXME("Dynamic linking not implemented yet.\n");

    wined3d_mutex_lock();
    if (!(wined3d_shader = wined3d_device_get_hull_shader(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *shader = NULL;
        return;
    }

    shader_impl = wined3d_shader_get_parent(wined3d_shader);
    wined3d_mutex_unlock();
    ID3D11HullShader_AddRef(*shader = &shader_impl->ID3D11HullShader_iface);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_HSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct wined3d_sampler *wined3d_sampler;
        struct d3d_sampler_state *sampler_impl;

        if (!(wined3d_sampler = wined3d_device_get_hs_sampler(device->wined3d_device, start_slot + i)))
        {
            samplers[i] = NULL;
            continue;
        }

        sampler_impl = wined3d_sampler_get_parent(wined3d_sampler);
        ID3D11SamplerState_AddRef(samplers[i] = &sampler_impl->ID3D11SamplerState_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_HSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_hs_cb(device->wined3d_device, start_slot + i)))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        ID3D11Buffer_AddRef(buffers[i] = &buffer_impl->ID3D11Buffer_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct wined3d_shader_resource_view *wined3d_view;
        struct d3d_shader_resource_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_ds_resource_view(device->wined3d_device, start_slot + i)))
        {
            views[i] = NULL;
            continue;
        }

        view_impl = wined3d_shader_resource_view_get_parent(wined3d_view);
        ID3D11ShaderResourceView_AddRef(views[i] = &view_impl->ID3D11ShaderResourceView_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DSGetShader(ID3D11DeviceContext *iface,
        ID3D11DomainShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d11_domain_shader *shader_impl;
    struct wined3d_shader *wined3d_shader;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %p.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances || class_instance_count)
        FIXME("Dynamic linking not implemented yet.\n");

    wined3d_mutex_lock();
    if (!(wined3d_shader = wined3d_device_get_domain_shader(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *shader = NULL;
        return;
    }

    shader_impl = wined3d_shader_get_parent(wined3d_shader);
    wined3d_mutex_unlock();
    ID3D11DomainShader_AddRef(*shader = &shader_impl->ID3D11DomainShader_iface);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct wined3d_sampler *wined3d_sampler;
        struct d3d_sampler_state *sampler_impl;

        if (!(wined3d_sampler = wined3d_device_get_ds_sampler(device->wined3d_device, start_slot + i)))
        {
            samplers[i] = NULL;
            continue;
        }

        sampler_impl = wined3d_sampler_get_parent(wined3d_sampler);
        ID3D11SamplerState_AddRef(samplers[i] = &sampler_impl->ID3D11SamplerState_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_DSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_ds_cb(device->wined3d_device, start_slot + i)))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        ID3D11Buffer_AddRef(buffers[i] = &buffer_impl->ID3D11Buffer_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n", iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct wined3d_shader_resource_view *wined3d_view;
        struct d3d_shader_resource_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_cs_resource_view(device->wined3d_device, start_slot + i)))
        {
            views[i] = NULL;
            continue;
        }

        view_impl = wined3d_shader_resource_view_get_parent(wined3d_view);
        ID3D11ShaderResourceView_AddRef(views[i] = &view_impl->ID3D11ShaderResourceView_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetUnorderedAccessViews(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11UnorderedAccessView **views)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n", iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct wined3d_unordered_access_view *wined3d_view;
        struct d3d11_unordered_access_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_cs_uav(device->wined3d_device, start_slot + i)))
        {
            views[i] = NULL;
            continue;
        }

        view_impl = wined3d_unordered_access_view_get_parent(wined3d_view);
        ID3D11UnorderedAccessView_AddRef(views[i] = &view_impl->ID3D11UnorderedAccessView_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetShader(ID3D11DeviceContext *iface,
        ID3D11ComputeShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    struct d3d11_compute_shader *shader_impl;
    struct wined3d_shader *wined3d_shader;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %p.\n",
            iface, shader, class_instances, class_instance_count);

    if (class_instances || class_instance_count)
        FIXME("Dynamic linking not implemented yet.\n");

    wined3d_mutex_lock();
    if (!(wined3d_shader = wined3d_device_get_compute_shader(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *shader = NULL;
        return;
    }

    shader_impl = wined3d_shader_get_parent(wined3d_shader);
    wined3d_mutex_unlock();
    ID3D11ComputeShader_AddRef(*shader = &shader_impl->ID3D11ComputeShader_iface);
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct wined3d_sampler *wined3d_sampler;
        struct d3d_sampler_state *sampler_impl;

        if (!(wined3d_sampler = wined3d_device_get_cs_sampler(device->wined3d_device, start_slot + i)))
        {
            samplers[i] = NULL;
            continue;
        }

        sampler_impl = wined3d_sampler_get_parent(wined3d_sampler);
        ID3D11SamplerState_AddRef(samplers[i] = &sampler_impl->ID3D11SamplerState_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_CSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_cs_cb(device->wined3d_device, start_slot + i)))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        ID3D11Buffer_AddRef(buffers[i] = &buffer_impl->ID3D11Buffer_iface);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_ClearState(ID3D11DeviceContext *iface)
{
    struct d3d_device *device = device_from_immediate_ID3D11DeviceContext(iface);
    static const float blend_factor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    unsigned int i;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    wined3d_device_set_vertex_shader(device->wined3d_device, NULL);
    wined3d_device_set_hull_shader(device->wined3d_device, NULL);
    wined3d_device_set_domain_shader(device->wined3d_device, NULL);
    wined3d_device_set_geometry_shader(device->wined3d_device, NULL);
    wined3d_device_set_pixel_shader(device->wined3d_device, NULL);
    wined3d_device_set_compute_shader(device->wined3d_device, NULL);
    for (i = 0; i < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; ++i)
    {
        wined3d_device_set_vs_sampler(device->wined3d_device, i, NULL);
        wined3d_device_set_hs_sampler(device->wined3d_device, i, NULL);
        wined3d_device_set_ds_sampler(device->wined3d_device, i, NULL);
        wined3d_device_set_gs_sampler(device->wined3d_device, i, NULL);
        wined3d_device_set_ps_sampler(device->wined3d_device, i, NULL);
        wined3d_device_set_cs_sampler(device->wined3d_device, i, NULL);
    }
    for (i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; ++i)
    {
        wined3d_device_set_vs_resource_view(device->wined3d_device, i, NULL);
        wined3d_device_set_hs_resource_view(device->wined3d_device, i, NULL);
        wined3d_device_set_ds_resource_view(device->wined3d_device, i, NULL);
        wined3d_device_set_gs_resource_view(device->wined3d_device, i, NULL);
        wined3d_device_set_ps_resource_view(device->wined3d_device, i, NULL);
        wined3d_device_set_cs_resource_view(device->wined3d_device, i, NULL);
    }
    for (i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; ++i)
    {
        wined3d_device_set_vs_cb(device->wined3d_device, i, NULL);
        wined3d_device_set_hs_cb(device->wined3d_device, i, NULL);
        wined3d_device_set_ds_cb(device->wined3d_device, i, NULL);
        wined3d_device_set_gs_cb(device->wined3d_device, i, NULL);
        wined3d_device_set_ps_cb(device->wined3d_device, i, NULL);
        wined3d_device_set_cs_cb(device->wined3d_device, i, NULL);
    }
    for (i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; ++i)
    {
        wined3d_device_set_stream_source(device->wined3d_device, i, NULL, 0, 0);
    }
    wined3d_device_set_index_buffer(device->wined3d_device, NULL, WINED3DFMT_UNKNOWN, 0);
    wined3d_device_set_vertex_declaration(device->wined3d_device, NULL);
    wined3d_device_set_primitive_type(device->wined3d_device, WINED3D_PT_UNDEFINED, 0);
    for (i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
    {
        wined3d_device_set_rendertarget_view(device->wined3d_device, i, NULL, FALSE);
    }
    wined3d_device_set_depth_stencil_view(device->wined3d_device, NULL);
    for (i = 0; i < D3D11_PS_CS_UAV_REGISTER_COUNT; ++i)
    {
        wined3d_device_set_unordered_access_view(device->wined3d_device, i, NULL, ~0u);
        wined3d_device_set_cs_uav(device->wined3d_device, i, NULL, ~0u);
    }
    ID3D11DeviceContext_OMSetDepthStencilState(iface, NULL, 0);
    ID3D11DeviceContext_OMSetBlendState(iface, NULL, blend_factor, D3D11_DEFAULT_SAMPLE_MASK);
    ID3D11DeviceContext_RSSetViewports(iface, 0, NULL);
    ID3D11DeviceContext_RSSetScissorRects(iface, 0, NULL);
    ID3D11DeviceContext_RSSetState(iface, NULL);
    for (i = 0; i < D3D11_SO_BUFFER_SLOT_COUNT; ++i)
    {
        wined3d_device_set_stream_output(device->wined3d_device, i, NULL, 0);
    }
    wined3d_device_set_predication(device->wined3d_device, NULL, FALSE);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d11_immediate_context_Flush(ID3D11DeviceContext *iface)
{
    FIXME("iface %p stub!\n", iface);
}

static D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE d3d11_immediate_context_GetType(ID3D11DeviceContext *iface)
{
    TRACE("iface %p.\n", iface);

    return D3D11_DEVICE_CONTEXT_IMMEDIATE;
}

static UINT STDMETHODCALLTYPE d3d11_immediate_context_GetContextFlags(ID3D11DeviceContext *iface)
{
    FIXME("iface %p stub!\n", iface);

    return 0;
}

static HRESULT STDMETHODCALLTYPE d3d11_immediate_context_FinishCommandList(ID3D11DeviceContext *iface,
        BOOL restore, ID3D11CommandList **command_list)
{
    FIXME("iface %p, restore %#x, command_list %p stub!\n", iface, restore, command_list);

    return E_NOTIMPL;
}

static const struct ID3D11DeviceContextVtbl d3d11_immediate_context_vtbl =
{
    /* IUnknown methods */
    d3d11_immediate_context_QueryInterface,
    d3d11_immediate_context_AddRef,
    d3d11_immediate_context_Release,
    /* ID3D11DeviceChild methods */
    d3d11_immediate_context_GetDevice,
    d3d11_immediate_context_GetPrivateData,
    d3d11_immediate_context_SetPrivateData,
    d3d11_immediate_context_SetPrivateDataInterface,
    /* ID3D11DeviceContext methods */
    d3d11_immediate_context_VSSetConstantBuffers,
    d3d11_immediate_context_PSSetShaderResources,
    d3d11_immediate_context_PSSetShader,
    d3d11_immediate_context_PSSetSamplers,
    d3d11_immediate_context_VSSetShader,
    d3d11_immediate_context_DrawIndexed,
    d3d11_immediate_context_Draw,
    d3d11_immediate_context_Map,
    d3d11_immediate_context_Unmap,
    d3d11_immediate_context_PSSetConstantBuffers,
    d3d11_immediate_context_IASetInputLayout,
    d3d11_immediate_context_IASetVertexBuffers,
    d3d11_immediate_context_IASetIndexBuffer,
    d3d11_immediate_context_DrawIndexedInstanced,
    d3d11_immediate_context_DrawInstanced,
    d3d11_immediate_context_GSSetConstantBuffers,
    d3d11_immediate_context_GSSetShader,
    d3d11_immediate_context_IASetPrimitiveTopology,
    d3d11_immediate_context_VSSetShaderResources,
    d3d11_immediate_context_VSSetSamplers,
    d3d11_immediate_context_Begin,
    d3d11_immediate_context_End,
    d3d11_immediate_context_GetData,
    d3d11_immediate_context_SetPredication,
    d3d11_immediate_context_GSSetShaderResources,
    d3d11_immediate_context_GSSetSamplers,
    d3d11_immediate_context_OMSetRenderTargets,
    d3d11_immediate_context_OMSetRenderTargetsAndUnorderedAccessViews,
    d3d11_immediate_context_OMSetBlendState,
    d3d11_immediate_context_OMSetDepthStencilState,
    d3d11_immediate_context_SOSetTargets,
    d3d11_immediate_context_DrawAuto,
    d3d11_immediate_context_DrawIndexedInstancedIndirect,
    d3d11_immediate_context_DrawInstancedIndirect,
    d3d11_immediate_context_Dispatch,
    d3d11_immediate_context_DispatchIndirect,
    d3d11_immediate_context_RSSetState,
    d3d11_immediate_context_RSSetViewports,
    d3d11_immediate_context_RSSetScissorRects,
    d3d11_immediate_context_CopySubresourceRegion,
    d3d11_immediate_context_CopyResource,
    d3d11_immediate_context_UpdateSubresource,
    d3d11_immediate_context_CopyStructureCount,
    d3d11_immediate_context_ClearRenderTargetView,
    d3d11_immediate_context_ClearUnorderedAccessViewUint,
    d3d11_immediate_context_ClearUnorderedAccessViewFloat,
    d3d11_immediate_context_ClearDepthStencilView,
    d3d11_immediate_context_GenerateMips,
    d3d11_immediate_context_SetResourceMinLOD,
    d3d11_immediate_context_GetResourceMinLOD,
    d3d11_immediate_context_ResolveSubresource,
    d3d11_immediate_context_ExecuteCommandList,
    d3d11_immediate_context_HSSetShaderResources,
    d3d11_immediate_context_HSSetShader,
    d3d11_immediate_context_HSSetSamplers,
    d3d11_immediate_context_HSSetConstantBuffers,
    d3d11_immediate_context_DSSetShaderResources,
    d3d11_immediate_context_DSSetShader,
    d3d11_immediate_context_DSSetSamplers,
    d3d11_immediate_context_DSSetConstantBuffers,
    d3d11_immediate_context_CSSetShaderResources,
    d3d11_immediate_context_CSSetUnorderedAccessViews,
    d3d11_immediate_context_CSSetShader,
    d3d11_immediate_context_CSSetSamplers,
    d3d11_immediate_context_CSSetConstantBuffers,
    d3d11_immediate_context_VSGetConstantBuffers,
    d3d11_immediate_context_PSGetShaderResources,
    d3d11_immediate_context_PSGetShader,
    d3d11_immediate_context_PSGetSamplers,
    d3d11_immediate_context_VSGetShader,
    d3d11_immediate_context_PSGetConstantBuffers,
    d3d11_immediate_context_IAGetInputLayout,
    d3d11_immediate_context_IAGetVertexBuffers,
    d3d11_immediate_context_IAGetIndexBuffer,
    d3d11_immediate_context_GSGetConstantBuffers,
    d3d11_immediate_context_GSGetShader,
    d3d11_immediate_context_IAGetPrimitiveTopology,
    d3d11_immediate_context_VSGetShaderResources,
    d3d11_immediate_context_VSGetSamplers,
    d3d11_immediate_context_GetPredication,
    d3d11_immediate_context_GSGetShaderResources,
    d3d11_immediate_context_GSGetSamplers,
    d3d11_immediate_context_OMGetRenderTargets,
    d3d11_immediate_context_OMGetRenderTargetsAndUnorderedAccessViews,
    d3d11_immediate_context_OMGetBlendState,
    d3d11_immediate_context_OMGetDepthStencilState,
    d3d11_immediate_context_SOGetTargets,
    d3d11_immediate_context_RSGetState,
    d3d11_immediate_context_RSGetViewports,
    d3d11_immediate_context_RSGetScissorRects,
    d3d11_immediate_context_HSGetShaderResources,
    d3d11_immediate_context_HSGetShader,
    d3d11_immediate_context_HSGetSamplers,
    d3d11_immediate_context_HSGetConstantBuffers,
    d3d11_immediate_context_DSGetShaderResources,
    d3d11_immediate_context_DSGetShader,
    d3d11_immediate_context_DSGetSamplers,
    d3d11_immediate_context_DSGetConstantBuffers,
    d3d11_immediate_context_CSGetShaderResources,
    d3d11_immediate_context_CSGetUnorderedAccessViews,
    d3d11_immediate_context_CSGetShader,
    d3d11_immediate_context_CSGetSamplers,
    d3d11_immediate_context_CSGetConstantBuffers,
    d3d11_immediate_context_ClearState,
    d3d11_immediate_context_Flush,
    d3d11_immediate_context_GetType,
    d3d11_immediate_context_GetContextFlags,
    d3d11_immediate_context_FinishCommandList,
};

static void d3d11_immediate_context_init(struct d3d11_immediate_context *context, struct d3d_device *device)
{
    context->ID3D11DeviceContext_iface.lpVtbl = &d3d11_immediate_context_vtbl;
    context->refcount = 1;

    ID3D11Device_AddRef(&device->ID3D11Device_iface);

    wined3d_private_store_init(&context->private_store);
}

static void d3d11_immediate_context_destroy(struct d3d11_immediate_context *context)
{
    wined3d_private_store_cleanup(&context->private_store);
}

/* ID3D11DeviceContext - deferred context methods */

static inline struct d3d11_deferred_context *impl_from_deferred_ID3D11DeviceContext(ID3D11DeviceContext *iface)
{
    return CONTAINING_RECORD(iface, struct d3d11_deferred_context, ID3D11DeviceContext_iface);
}

static HRESULT STDMETHODCALLTYPE d3d11_deferred_context_QueryInterface(ID3D11DeviceContext *iface,
        REFIID riid, void **out)
{
    TRACE("iface %p, riid %s, out %p.\n", iface, debugstr_guid(riid), out);

    if (IsEqualGUID(riid, &IID_ID3D11DeviceContext)
            || IsEqualGUID(riid, &IID_ID3D11DeviceChild)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        ID3D11DeviceContext_AddRef(iface);
        *out = iface;
        return S_OK;
    }

    WARN("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(riid));
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE d3d11_deferred_context_AddRef(ID3D11DeviceContext *iface)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    ULONG refcount = InterlockedIncrement(&context->refcount);

    TRACE("%p increasing refcount to %u.\n", context, refcount);

    return refcount;
}

static ULONG STDMETHODCALLTYPE d3d11_deferred_context_Release(ID3D11DeviceContext *iface)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    ULONG refcount = InterlockedDecrement(&context->refcount);

    TRACE("%p decreasing refcount to %u.\n", context, refcount);

    if (!refcount)
    {
        free_deferred_calls(&context->commands);
        wined3d_private_store_cleanup(&context->private_store);
        ID3D11Device_Release(context->device);
        HeapFree(GetProcessHeap(), 0, context);
    }

    return refcount;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GetDevice(ID3D11DeviceContext *iface, ID3D11Device **device)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, device %p.\n", iface, device);

    ID3D11Device_AddRef(context->device);
    *device = context->device;
}

static HRESULT STDMETHODCALLTYPE d3d11_deferred_context_GetPrivateData(ID3D11DeviceContext *iface, REFGUID guid,
        UINT *data_size, void *data)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, guid %s, data_size %p, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return d3d_get_private_data(&context->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_deferred_context_SetPrivateData(ID3D11DeviceContext *iface, REFGUID guid,
        UINT data_size, const void *data)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, guid %s, data_size %u, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return d3d_set_private_data(&context->private_store, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d11_deferred_context_SetPrivateDataInterface(ID3D11DeviceContext *iface,
        REFGUID guid, const IUnknown *data)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, guid %s, data %p.\n", iface, debugstr_guid(guid), data);

    return d3d_set_private_data_interface(&context->private_store, guid, data);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_VSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    add_deferred_set_constant_buffers(context, DEFERRED_VSSETCONSTANTBUFFERS, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_PSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    add_deferred_set_shader_resources(context, DEFERRED_PSSETSHADERRESOURCES, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_PSSetShader(ID3D11DeviceContext *iface,
        ID3D11PixelShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_PSSETSHADER;
    if (shader) ID3D11PixelShader_AddRef(shader);
    call->ps_info.shader = shader;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_PSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    add_deferred_set_samplers(context, DEFERRED_PSSETSAMPLERS, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_VSSetShader(ID3D11DeviceContext *iface,
        ID3D11VertexShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_VSSETSHADER;
    if (shader) ID3D11VertexShader_AddRef(shader);
    call->vs_info.shader = shader;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DrawIndexed(ID3D11DeviceContext *iface,
        UINT index_count, UINT start_index_location, INT base_vertex_location)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, index_count %u, start_index_location %u, base_vertex_location %d.\n",
            iface, index_count, start_index_location, base_vertex_location);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_DRAWINDEXED;
    call->draw_indexed_info.count = index_count;
    call->draw_indexed_info.start_index = start_index_location;
    call->draw_indexed_info.base_vertex = base_vertex_location;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_Draw(ID3D11DeviceContext *iface,
        UINT vertex_count, UINT start_vertex_location)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, vertex_count %u, start_vertex_location %u.\n",
            iface, vertex_count, start_vertex_location);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_DRAW;
    call->draw_info.count = vertex_count;
    call->draw_info.start = start_vertex_location;
}

static HRESULT STDMETHODCALLTYPE d3d11_deferred_context_Map(ID3D11DeviceContext *iface, ID3D11Resource *resource,
        UINT subresource_idx, D3D11_MAP map_type, UINT map_flags, D3D11_MAPPED_SUBRESOURCE *mapped_subresource)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct wined3d_resource *wined3d_resource;
    struct wined3d_map_info map_info;
    struct deferred_call *call, *previous = NULL;
    HRESULT hr;

    TRACE("iface %p, resource %p, subresource_idx %u, map_type %u, map_flags %#x, mapped_subresource %p.\n",
            iface, resource, subresource_idx, map_type, map_flags, mapped_subresource);

    if (map_type != D3D11_MAP_WRITE_DISCARD && map_type != D3D11_MAP_WRITE_NO_OVERWRITE)
        FIXME("Map type %u not supported!\n", map_type);

    if (map_type != D3D11_MAP_WRITE_DISCARD)
    {
        LIST_FOR_EACH_ENTRY_REV(call, &context->commands, struct deferred_call, entry)
        {
            if (call->cmd != DEFERRED_MAP) continue;
            if (call->map_info.resource != resource) continue;
            if (call->map_info.subresource_idx != subresource_idx) continue;
            previous = call;
            break;
        }
        if (!previous)
        {
            FIXME("First map in deferred context didn't use D3D11_MAP_WRITE_DISCARD.\n");
            return E_INVALIDARG;
        }
    }

    wined3d_resource = wined3d_resource_from_d3d11_resource(resource);

    wined3d_mutex_lock();
    hr = wined3d_resource_map_info(wined3d_resource, subresource_idx,
            &map_info, wined3d_map_flags_from_d3d11_map_type(map_type));
    wined3d_mutex_unlock();

    if (FAILED(hr))
        return hr;

    if (!(call = add_deferred_call(context, map_info.size)))
        return E_OUTOFMEMORY;

    call->cmd = DEFERRED_MAP;
    ID3D11Resource_AddRef(resource);
    call->map_info.resource = resource;
    call->map_info.subresource_idx = subresource_idx;
    call->map_info.map_type = map_type;
    call->map_info.map_flags = map_flags;
    call->map_info.buffer = (void *)(call + 1);
    call->map_info.size = map_info.size;

    if (previous)
        memcpy(call->map_info.buffer, previous->map_info.buffer, map_info.size);

    mapped_subresource->pData = call->map_info.buffer;
    mapped_subresource->RowPitch = map_info.row_pitch;
    mapped_subresource->DepthPitch = map_info.slice_pitch;

    return S_OK;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_Unmap(ID3D11DeviceContext *iface, ID3D11Resource *resource,
        UINT subresource_idx)
{
    TRACE("iface %p, resource %p, subresource_idx %u.\n", iface, resource, subresource_idx);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_PSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    add_deferred_set_constant_buffers(context, DEFERRED_PSSETCONSTANTBUFFERS, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_IASetInputLayout(ID3D11DeviceContext *iface,
        ID3D11InputLayout *input_layout)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, input_layout %p.\n", iface, input_layout);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_IASETINPUTLAYOUT;
    if (input_layout) ID3D11InputLayout_AddRef(input_layout);
    call->input_layout_info.layout = input_layout;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_IASetVertexBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers, const UINT *strides, const UINT *offsets)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;
    int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p, strides %p, offsets %p.\n",
            iface, start_slot, buffer_count, buffers, strides, offsets);

    if (!(call = add_deferred_call(context, buffer_count * (sizeof(*buffers) + sizeof(UINT) + sizeof(UINT)))))
        return;

    call->cmd = DEFERRED_IASETVERTEXBUFFERS;
    call->vbuffer_info.start_slot = start_slot;
    call->vbuffer_info.num_buffers = buffer_count;

    call->vbuffer_info.buffers = (void *)(call + 1);
    call->vbuffer_info.strides = (void *)&call->vbuffer_info.buffers[buffer_count];
    call->vbuffer_info.offsets = (void *)&call->vbuffer_info.strides[buffer_count];
    for (i = 0; i < buffer_count; i++)
    {
        if (buffers[i]) ID3D11Buffer_AddRef(buffers[i]);
        call->vbuffer_info.buffers[i] = buffers[i];
        call->vbuffer_info.strides[i] = strides[i];
        call->vbuffer_info.offsets[i] = offsets[i];
    }
}

static void STDMETHODCALLTYPE d3d11_deferred_context_IASetIndexBuffer(ID3D11DeviceContext *iface,
        ID3D11Buffer *buffer, DXGI_FORMAT format, UINT offset)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, buffer %p, format %s, offset %u.\n",
            iface, buffer, debug_dxgi_format(format), offset);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_IASETINDEXBUFFER;
    if (buffer) ID3D11Buffer_AddRef(buffer);
    call->index_buffer_info.buffer = buffer;
    call->index_buffer_info.format = format;
    call->index_buffer_info.offset = offset;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DrawIndexedInstanced(ID3D11DeviceContext *iface,
        UINT instance_index_count, UINT instance_count, UINT start_index_location, INT base_vertex_location,
        UINT start_instance_location)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, instance_index_count %u, instance_count %u, start_index_location %u, "
            "base_vertex_location %d, start_instance_location %u.\n",
            iface, instance_index_count, instance_count, start_index_location,
            base_vertex_location, start_instance_location);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_DRAWINDEXEDINSTANCED;
    call->draw_indexed_inst_info.count_per_instance = instance_index_count;
    call->draw_indexed_inst_info.instance_count = instance_count;
    call->draw_indexed_inst_info.start_index = start_index_location;
    call->draw_indexed_inst_info.base_vertex = base_vertex_location;
    call->draw_indexed_inst_info.start_instance = start_instance_location;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DrawInstanced(ID3D11DeviceContext *iface,
        UINT instance_vertex_count, UINT instance_count, UINT start_vertex_location, UINT start_instance_location)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, instance_vertex_count %u, instance_count %u, start_vertex_location %u, "
            "start_instance_location %u.\n",
            iface, instance_vertex_count, instance_count, start_vertex_location,
            start_instance_location);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_DRAWINSTANCED;
    call->draw_instanced_info.instance_vertex_count = instance_vertex_count;
    call->draw_instanced_info.instance_count = instance_count;
    call->draw_instanced_info.start_vertex_location = start_vertex_location;
    call->draw_instanced_info.start_instance_location = start_instance_location;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    add_deferred_set_constant_buffers(context, DEFERRED_GSSETCONSTANTBUFFERS, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GSSetShader(ID3D11DeviceContext *iface,
        ID3D11GeometryShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_GSSETSHADER;
    if (shader) ID3D11GeometryShader_AddRef(shader);
    call->gs_info.shader = shader;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_IASetPrimitiveTopology(ID3D11DeviceContext *iface,
        D3D11_PRIMITIVE_TOPOLOGY topology)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, topology %u.\n", iface, topology);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_IASETPRIMITIVETOPOLOGY;
    call->topology_info.topology = topology;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_VSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    add_deferred_set_shader_resources(context, DEFERRED_VSSETSHADERRESOURCES, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_VSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    add_deferred_set_samplers(context, DEFERRED_VSSETSAMPLERS, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_Begin(ID3D11DeviceContext *iface,
        ID3D11Asynchronous *asynchronous)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, asynchronous %p.\n", iface, asynchronous);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_BEGIN;
    if (asynchronous) ID3D11Asynchronous_AddRef(asynchronous);
    call->async_info.asynchronous = asynchronous;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_End(ID3D11DeviceContext *iface,
        ID3D11Asynchronous *asynchronous)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, asynchronous %p.\n", iface, asynchronous);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_END;
    if (asynchronous) ID3D11Asynchronous_AddRef(asynchronous);
    call->async_info.asynchronous = asynchronous;
}

static HRESULT STDMETHODCALLTYPE d3d11_deferred_context_GetData(ID3D11DeviceContext *iface,
        ID3D11Asynchronous *asynchronous, void *data, UINT data_size, UINT data_flags)
{
    FIXME("iface %p, asynchronous %p, data %p, data_size %u, data_flags %#x stub!\n",
            iface, asynchronous, data, data_size, data_flags);

    return E_NOTIMPL;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_SetPredication(ID3D11DeviceContext *iface,
        ID3D11Predicate *predicate, BOOL value)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, predicate %p, value %#x.\n", iface, predicate, value);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_SETPREDICATION;
    if (predicate) ID3D11Predicate_AddRef(predicate);
    call->set_predication_info.predicate = predicate;
    call->set_predication_info.value = value;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    add_deferred_set_shader_resources(context, DEFERRED_GSSETSHADERRESOURCES, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    add_deferred_set_samplers(context, DEFERRED_GSSETSAMPLERS, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_OMSetRenderTargets(ID3D11DeviceContext *iface,
        UINT render_target_view_count, ID3D11RenderTargetView *const *render_target_views,
        ID3D11DepthStencilView *depth_stencil_view)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;
    int i;

    TRACE("iface %p, render_target_view_count %u, render_target_views %p, depth_stencil_view %p.\n",
            iface, render_target_view_count, render_target_views, depth_stencil_view);

    if (!(call = add_deferred_call(context, sizeof(*render_target_views) * render_target_view_count)))
        return;

    call->cmd = DEFERRED_OMSETRENDERTARGETS;
    call->render_target_info.num_views = render_target_view_count;
    call->render_target_info.render_targets = (void *)(call + 1);
    for (i = 0; i < render_target_view_count; i++)
    {
        if (render_target_views[i])
            ID3D11RenderTargetView_AddRef(render_target_views[i]);
        call->render_target_info.render_targets[i] = render_target_views[i];
    }
    if (depth_stencil_view)
        ID3D11DepthStencilView_AddRef(depth_stencil_view);
    call->render_target_info.depth_stencil = depth_stencil_view;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_OMSetRenderTargetsAndUnorderedAccessViews(
        ID3D11DeviceContext *iface, UINT render_target_view_count,
        ID3D11RenderTargetView *const *render_target_views, ID3D11DepthStencilView *depth_stencil_view,
        UINT unordered_access_view_start_slot, UINT unordered_access_view_count,
        ID3D11UnorderedAccessView *const *unordered_access_views, const UINT *initial_counts)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;
    UINT i;

    TRACE("iface %p, render_target_view_count %u, render_target_views %p, depth_stencil_view %p, "
            "unordered_access_view_start_slot %u, unordered_access_view_count %u, unordered_access_views %p, "
            "initial_counts %p.\n",
            iface, render_target_view_count, render_target_views, depth_stencil_view,
            unordered_access_view_start_slot, unordered_access_view_count, unordered_access_views,
            initial_counts);

    if (!(call = add_deferred_call(context, sizeof(ID3D11RenderTargetView *) * render_target_view_count +
                                   (sizeof(ID3D11UnorderedAccessView *) + sizeof(UINT)) * unordered_access_view_count)))
        return;

    call->cmd = DEFERRED_OMSETRENDERTARGETSANDUNORDEREDACCESVIEWS;
    call->render_targets_and_unordered_access_views_info.render_target_view_count = render_target_view_count;
    call->render_targets_and_unordered_access_views_info.unordered_access_view_start_slot = unordered_access_view_start_slot;
    call->render_targets_and_unordered_access_views_info.unordered_access_view_count = unordered_access_view_count;

    if (depth_stencil_view) ID3D11DepthStencilView_AddRef(depth_stencil_view);
    call->render_targets_and_unordered_access_views_info.depth_stencil_view = depth_stencil_view;

    call->render_targets_and_unordered_access_views_info.render_target_views = (void *)(call + 1);
    call->render_targets_and_unordered_access_views_info.unordered_access_views =
            (void *)&call->render_targets_and_unordered_access_views_info.render_target_views[render_target_view_count];
    call->render_targets_and_unordered_access_views_info.initial_counts = initial_counts ?
            (void *)&call->render_targets_and_unordered_access_views_info.unordered_access_views[unordered_access_view_count] : NULL;

    for (i = 0; i < render_target_view_count; i++)
    {
        if (render_target_views[i]) ID3D11RenderTargetView_AddRef(render_target_views[i]);
        call->render_targets_and_unordered_access_views_info.render_target_views[i] = render_target_views[i];
    }
    for (i = 0; i < unordered_access_view_count; i++)
    {
        if (unordered_access_views[i]) ID3D11UnorderedAccessView_AddRef(unordered_access_views[i]);
        call->render_targets_and_unordered_access_views_info.unordered_access_views[i] = unordered_access_views[i];
        if (initial_counts) call->render_targets_and_unordered_access_views_info.initial_counts[i] = initial_counts[i];
    }
}

static void STDMETHODCALLTYPE d3d11_deferred_context_OMSetBlendState(ID3D11DeviceContext *iface,
        ID3D11BlendState *blend_state, const float blend_factor[4], UINT sample_mask)
{
    static const float default_blend_factor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;
    int i;

    TRACE("iface %p, blend_state %p, blend_factor %s, sample_mask 0x%08x.\n",
            iface, blend_state, debug_float4(blend_factor), sample_mask);

    if (!blend_factor)
        blend_factor = default_blend_factor;

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_OMSETBLENDSTATE;
    if (blend_state) ID3D11BlendState_AddRef(blend_state);
    call->blend_state_info.state = blend_state;
    for (i = 0; i < 4; i++)
        call->blend_state_info.factor[i] = blend_factor[i];
    call->blend_state_info.mask = sample_mask;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_OMSetDepthStencilState(ID3D11DeviceContext *iface,
        ID3D11DepthStencilState *depth_stencil_state, UINT stencil_ref)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, depth_stencil_state %p, stencil_ref %u.\n",
            iface, depth_stencil_state, stencil_ref);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_OMSETDEPTHSTENCILSTATE;
    if (depth_stencil_state) ID3D11DepthStencilState_AddRef(depth_stencil_state);
    call->stencil_state_info.state = depth_stencil_state;
    call->stencil_state_info.stencil_ref = stencil_ref;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_SOSetTargets(ID3D11DeviceContext *iface,
        UINT buffer_count, ID3D11Buffer *const *buffers, const UINT *offsets)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;
    UINT i;

    TRACE("iface %p, buffer_count %u, buffers %p, offsets %p.\n", iface, buffer_count, buffers, offsets);

    if (!(call = add_deferred_call(context, (sizeof(ID3D11Buffer *) + sizeof(UINT)) * buffer_count)))
        return;

    call->cmd = DEFERRED_SOSETTARGETS;
    call->so_set_targets_info.buffers = (void *)(call + 1);
    call->so_set_targets_info.offsets = (void *)&call->so_set_targets_info.buffers[buffer_count];

    for (i = 0; i < buffer_count; i++)
    {
        if (buffers[i]) ID3D11Buffer_AddRef(buffers[i]);
        call->so_set_targets_info.buffers[i] = buffers[i];
        call->so_set_targets_info.offsets[i] = offsets[i];
    }
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DrawAuto(ID3D11DeviceContext *iface)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p.\n", iface);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_DRAWAUTO;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DrawIndexedInstancedIndirect(ID3D11DeviceContext *iface,
        ID3D11Buffer *buffer, UINT offset)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, buffer %p, offset %u.\n", iface, buffer, offset);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_DRAWINDEXEDINSTANCEDINDIRECT;
    if (buffer) ID3D11Buffer_AddRef(buffer);
    call->draw_instanced_indirect_info.buffer = buffer;
    call->draw_instanced_indirect_info.offset = offset;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DrawInstancedIndirect(ID3D11DeviceContext *iface,
        ID3D11Buffer *buffer, UINT offset)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, buffer %p, offset %u.\n", iface, buffer, offset);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_DRAWINSTANCEDINDIRECT;
    if (buffer) ID3D11Buffer_AddRef(buffer);
    call->draw_instanced_indirect_info.buffer = buffer;
    call->draw_instanced_indirect_info.offset = offset;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_Dispatch(ID3D11DeviceContext *iface,
        UINT thread_group_count_x, UINT thread_group_count_y, UINT thread_group_count_z)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, thread_group_count_x %u, thread_group_count_y %u, thread_group_count_z %u.\n",
            iface, thread_group_count_x, thread_group_count_y, thread_group_count_z);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_DISPATCH;
    call->dispatch_info.count_x = thread_group_count_x;
    call->dispatch_info.count_y = thread_group_count_y;
    call->dispatch_info.count_z = thread_group_count_z;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DispatchIndirect(ID3D11DeviceContext *iface,
        ID3D11Buffer *buffer, UINT offset)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, buffer %p, offset %u.\n", iface, buffer, offset);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_DISPATCHINDIRECT;
    if (buffer) ID3D11Buffer_AddRef(buffer);
    call->dispatch_indirect_info.buffer = buffer;
    call->dispatch_indirect_info.offset = offset;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_RSSetState(ID3D11DeviceContext *iface,
        ID3D11RasterizerState *rasterizer_state)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, rasterizer_state %p.\n", iface, rasterizer_state);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_RSSETSTATE;
    if (rasterizer_state) ID3D11RasterizerState_AddRef(rasterizer_state);
    call->rstate_info.state = rasterizer_state;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_RSSetViewports(ID3D11DeviceContext *iface,
        UINT viewport_count, const D3D11_VIEWPORT *viewports)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, viewport_count %u, viewports %p.\n", iface, viewport_count, viewports);

    if (!(call = add_deferred_call(context, sizeof(D3D11_VIEWPORT) * viewport_count)))
        return;

    call->cmd = DEFERRED_RSSETVIEWPORTS;
    call->viewport_info.num_viewports = viewport_count;
    call->viewport_info.viewports = (void *)(call + 1);
    memcpy(call->viewport_info.viewports, viewports, sizeof(D3D11_VIEWPORT) * viewport_count);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_RSSetScissorRects(ID3D11DeviceContext *iface,
        UINT rect_count, const D3D11_RECT *rects)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, rect_count %u, rects %p.\n", iface, rect_count, rects);

    if (!(call = add_deferred_call(context, sizeof(D3D11_RECT) * rect_count)))
        return;

    call->cmd = DEFERRED_RSSETSCISSORRECTS;
    call->rs_set_scissor_rects_info.rects = (void *)(call + 1);
    call->rs_set_scissor_rects_info.rect_count = rect_count;
    memcpy(call->rs_set_scissor_rects_info.rects, rects, sizeof(D3D11_RECT) * rect_count);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CopySubresourceRegion(ID3D11DeviceContext *iface,
        ID3D11Resource *dst_resource, UINT dst_subresource_idx, UINT dst_x, UINT dst_y, UINT dst_z,
        ID3D11Resource *src_resource, UINT src_subresource_idx, const D3D11_BOX *src_box)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, dst_resource %p, dst_subresource_idx %u, dst_x %u, dst_y %u, dst_z %u, "
            "src_resource %p, src_subresource_idx %u, src_box %p.\n",
            iface, dst_resource, dst_subresource_idx, dst_x, dst_y, dst_z,
            src_resource, src_subresource_idx, src_box);

    if (!(call = add_deferred_call(context, src_box ? sizeof(D3D11_BOX) : 0)))
        return;

    if (dst_resource) ID3D11Resource_AddRef(dst_resource);
    if (src_resource) ID3D11Resource_AddRef(src_resource);

    call->cmd = DEFERRED_COPYSUBRESOURCEREGION;
    call->copy_subresource_region_info.dst_resource = dst_resource;
    call->copy_subresource_region_info.dst_subresource_idx = dst_subresource_idx;
    call->copy_subresource_region_info.dst_x = dst_x;
    call->copy_subresource_region_info.dst_y = dst_y;
    call->copy_subresource_region_info.dst_z = dst_z;
    call->copy_subresource_region_info.src_resource = src_resource;
    call->copy_subresource_region_info.src_subresource_idx = src_subresource_idx;
    if (src_box)
    {
        call->copy_subresource_region_info.src_box = (void *)(call + 1);
        *call->copy_subresource_region_info.src_box = *src_box;
    }
    else
    {
        call->copy_subresource_region_info.src_box = NULL;
    }
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CopyResource(ID3D11DeviceContext *iface,
        ID3D11Resource *dst_resource, ID3D11Resource *src_resource)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, dst_resource %p, src_resource %p.\n", iface, dst_resource, src_resource);

    if (!(call = add_deferred_call(context, 0)))
        return;

    if (dst_resource) ID3D11Resource_AddRef(dst_resource);
    if (src_resource) ID3D11Resource_AddRef(src_resource);

    call->cmd = DEFERRED_COPYRESOURCE;
    call->copy_resource_info.dst_resource = dst_resource;
    call->copy_resource_info.src_resource = src_resource;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_UpdateSubresource(ID3D11DeviceContext *iface,
        ID3D11Resource *resource, UINT subresource_idx, const D3D11_BOX *box,
        const void *data, UINT row_pitch, UINT depth_pitch)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct wined3d_resource *wined3d_resource;
    struct wined3d_box wined3d_box;
    struct deferred_call *call;
    UINT size;

    TRACE("iface %p, resource %p, subresource_idx %u, box %p, data %p, row_pitch %u, depth_pitch %u.\n",
            iface, resource, subresource_idx, box, data, row_pitch, depth_pitch);

    if (box)
        wined3d_box_set(&wined3d_box, box->left, box->top, box->right, box->bottom, box->front, box->back);

    wined3d_resource = wined3d_resource_from_d3d11_resource(resource);
    wined3d_mutex_lock();
    size = wined3d_resource_update_info(wined3d_resource, subresource_idx,
            box ? &wined3d_box : NULL, row_pitch, depth_pitch);
    wined3d_mutex_unlock();

    if (!size)
    {
        FIXME("Failed to calculate size of data\n");
        return;
    }

    if (!(call = add_deferred_call(context, size + (box ? sizeof(D3D11_BOX) : 0))))
        return;

    call->cmd = DEFERRED_UPDATESUBRESOURCE;
    call->update_subresource_info.resource = resource;
    call->update_subresource_info.subresource_idx = subresource_idx;
    call->update_subresource_info.row_pitch = row_pitch;
    call->update_subresource_info.depth_pitch = depth_pitch;

    ID3D11Resource_AddRef(resource);

    if (box)
    {
        call->update_subresource_info.box  = (void *)(call + 1);
        call->update_subresource_info.data = (void *)(call->update_subresource_info.box + 1);
        *call->update_subresource_info.box = *box;
    }
    else
    {
        call->update_subresource_info.box  = NULL;
        call->update_subresource_info.data = (void *)(call + 1);
    }

    memcpy(call->update_subresource_info.data, data, size);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CopyStructureCount(ID3D11DeviceContext *iface,
        ID3D11Buffer *dst_buffer, UINT dst_offset, ID3D11UnorderedAccessView *src_view)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, dst_buffer %p, dst_offset %u, src_view %p.\n",
          iface, dst_buffer, dst_offset, src_view);

    if (!(call = add_deferred_call(context, 0)))
        return;

    if (dst_buffer) ID3D11Buffer_AddRef(dst_buffer);
    if (src_view) ID3D11UnorderedAccessView_AddRef(src_view);

    call->cmd = DEFERRED_COPYSTRUCTURECOUNT;
    call->copy_structure_count_info.dst_buffer = dst_buffer;
    call->copy_structure_count_info.dst_offset = dst_offset;
    call->copy_structure_count_info.src_view = src_view;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_ClearRenderTargetView(ID3D11DeviceContext *iface,
        ID3D11RenderTargetView *render_target_view, const float color_rgba[4])
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;
    int i;

    TRACE("iface %p, render_target_view %p, color_rgba %s.\n",
            iface, render_target_view, debug_float4(color_rgba));

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_CLEARRENDERTARGETVIEW;
    if (render_target_view) ID3D11RenderTargetView_AddRef(render_target_view);
    call->clear_rtv_info.rtv = render_target_view;
    for (i = 0; i < 4; i++)
        call->clear_rtv_info.color[i] = color_rgba[i];
}

static void STDMETHODCALLTYPE d3d11_deferred_context_ClearUnorderedAccessViewUint(ID3D11DeviceContext *iface,
        ID3D11UnorderedAccessView *unordered_access_view, const UINT values[4])
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;
    int i;

    TRACE("iface %p, unordered_access_view %p, values {%u %u %u %u}.\n",
            iface, unordered_access_view, values[0], values[1], values[2], values[3]);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_CLEARUNORDEREDACCESSVIEWUINT;
    if (unordered_access_view) ID3D11UnorderedAccessView_AddRef(unordered_access_view);
    call->clear_unordered_access_view_uint.unordered_access_view = unordered_access_view;
    for (i = 0; i < 4; i++)
        call->clear_unordered_access_view_uint.values[i] = values[i];
}

static void STDMETHODCALLTYPE d3d11_deferred_context_ClearUnorderedAccessViewFloat(ID3D11DeviceContext *iface,
        ID3D11UnorderedAccessView *unordered_access_view, const float values[4])
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;
    int i;

    TRACE("iface %p, unordered_access_view %p, values %s.\n",
            iface, unordered_access_view, debug_float4(values));

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_CLEARUNORDEREDACCESSVIEWFLOAT;
    if (unordered_access_view) ID3D11UnorderedAccessView_AddRef(unordered_access_view);
    call->clear_unordered_access_view_float.unordered_access_view = unordered_access_view;
    for (i = 0; i < 4; i++)
        call->clear_unordered_access_view_float.values[i] = values[i];
}

static void STDMETHODCALLTYPE d3d11_deferred_context_ClearDepthStencilView(ID3D11DeviceContext *iface,
        ID3D11DepthStencilView *depth_stencil_view, UINT flags, FLOAT depth, UINT8 stencil)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, depth_stencil_view %p, flags %#x, depth %.8e, stencil %u.\n",
            iface, depth_stencil_view, flags, depth, stencil);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_CLEARDEPTHSTENCILVIEW;
    if (depth_stencil_view) ID3D11DepthStencilView_AddRef(depth_stencil_view);
    call->clear_depth_info.view = depth_stencil_view;
    call->clear_depth_info.flags = flags;
    call->clear_depth_info.depth = depth;
    call->clear_depth_info.stencil = stencil;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GenerateMips(ID3D11DeviceContext *iface,
        ID3D11ShaderResourceView *view)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, view %p.\n", iface, view);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_GENERATEMIPS;
    if (view) ID3D11ShaderResourceView_AddRef(view);
    call->generate_mips_info.view = view;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_SetResourceMinLOD(ID3D11DeviceContext *iface,
        ID3D11Resource *resource, FLOAT min_lod)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, resource %p, min_lod %f.\n", iface, resource, min_lod);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_SETRESOURCEMINLOD;
    if (resource) ID3D11Resource_AddRef(resource);
    call->set_resource_min_lod_info.resource = resource;
    call->set_resource_min_lod_info.min_lod = min_lod;
}

static FLOAT STDMETHODCALLTYPE d3d11_deferred_context_GetResourceMinLOD(ID3D11DeviceContext *iface,
        ID3D11Resource *resource)
{
    FIXME("iface %p, resource %p stub!\n", iface, resource);

    return 0.0f;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_ResolveSubresource(ID3D11DeviceContext *iface,
        ID3D11Resource *dst_resource, UINT dst_subresource_idx,
        ID3D11Resource *src_resource, UINT src_subresource_idx,
        DXGI_FORMAT format)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, dst_resource %p, dst_subresource_idx %u, src_resource %p, src_subresource_idx %u, "
            "format %s.\n",
            iface, dst_resource, dst_subresource_idx, src_resource, src_subresource_idx,
            debug_dxgi_format(format));

    if (!(call = add_deferred_call(context, 0)))
        return;

    if (dst_resource) ID3D11Resource_AddRef(dst_resource);
    if (src_resource) ID3D11Resource_AddRef(src_resource);

    call->cmd = DEFERRED_RESOLVESUBRESOURCE;
    call->resolve_subresource_info.dst_resource = dst_resource;
    call->resolve_subresource_info.dst_subresource_idx = dst_subresource_idx;
    call->resolve_subresource_info.src_resource = src_resource;
    call->resolve_subresource_info.src_subresource_idx = src_subresource_idx;
    call->resolve_subresource_info.format = format;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_ExecuteCommandList(ID3D11DeviceContext *iface,
        ID3D11CommandList *command_list, BOOL restore_state)
{
    FIXME("iface %p, command_list %p, restore_state %#x stub!\n", iface, command_list, restore_state);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_HSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    add_deferred_set_shader_resources(context, DEFERRED_HSSETSHADERRESOURCES, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_HSSetShader(ID3D11DeviceContext *iface,
        ID3D11HullShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_HSSETSHADER;
    if (shader) ID3D11HullShader_AddRef(shader);
    call->hs_info.shader = shader;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_HSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    add_deferred_set_samplers(context, DEFERRED_HSSETSAMPLERS, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_HSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    add_deferred_set_constant_buffers(context, DEFERRED_HSSETCONSTANTBUFFERS, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    add_deferred_set_shader_resources(context, DEFERRED_DSSETSHADERRESOURCES, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DSSetShader(ID3D11DeviceContext *iface,
        ID3D11DomainShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_DSSETSHADER;
    if (shader) ID3D11DomainShader_AddRef(shader);
    call->ds_info.shader = shader;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    add_deferred_set_samplers(context, DEFERRED_DSSETSAMPLERS, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    add_deferred_set_constant_buffers(context, DEFERRED_DSSETCONSTANTBUFFERS, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CSSetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView *const *views)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    add_deferred_set_shader_resources(context, DEFERRED_CSSETSHADERRESOURCES, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CSSetUnorderedAccessViews(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11UnorderedAccessView *const *views, const UINT *initial_counts)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;
    int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p, initial_counts %p.\n",
            iface, start_slot, view_count, views, initial_counts);

    if (!(call = add_deferred_call(context, view_count * (sizeof(*views) + sizeof(UINT)))))
        return;

    call->cmd = DEFERRED_CSSETUNORDEREDACCESSVIEWS;
    call->unordered_view.start_slot = start_slot;
    call->unordered_view.num_views = view_count;
    call->unordered_view.views = (void *)(call + 1);
    call->unordered_view.initial_counts = initial_counts ?
        (void *)&call->unordered_view.views[view_count] : NULL;
    for (i = 0; i < view_count; i++)
    {
        if (views[i]) ID3D11UnorderedAccessView_AddRef(views[i]);
        call->unordered_view.views[i] = views[i];
        if (initial_counts) call->unordered_view.initial_counts[i] = initial_counts[i];
    }
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CSSetShader(ID3D11DeviceContext *iface,
        ID3D11ComputeShader *shader, ID3D11ClassInstance *const *class_instances, UINT class_instance_count)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p, shader %p, class_instances %p, class_instance_count %u.\n",
            iface, shader, class_instances, class_instance_count);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_CSSETSHADER;
    if (shader) ID3D11ComputeShader_AddRef(shader);
    call->cs_info.shader = shader;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CSSetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState *const *samplers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    add_deferred_set_samplers(context, DEFERRED_CSSETSAMPLERS, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CSSetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer *const *buffers)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    add_deferred_set_constant_buffers(context, DEFERRED_CSSETCONSTANTBUFFERS, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_VSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    FIXME("iface %p, start_slot %u, buffer_count %u, buffers %p stub!\n",
            iface, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_PSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    FIXME("iface %p, start_slot %u, view_count %u, views %p stub!\n",
            iface, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_PSGetShader(ID3D11DeviceContext *iface,
        ID3D11PixelShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    FIXME("iface %p, shader %p, class_instances %p, class_instance_count %p stub!\n",
            iface, shader, class_instances, class_instance_count);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_PSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    FIXME("iface %p, start_slot %u, sampler_count %u, samplers %p stub!\n",
            iface, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_VSGetShader(ID3D11DeviceContext *iface,
        ID3D11VertexShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    FIXME("iface %p, shader %p, class_instances %p, class_instance_count %p stub!\n",
            iface, shader, class_instances, class_instance_count);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_PSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    FIXME("iface %p, start_slot %u, buffer_count %u, buffers %p stub!\n",
            iface, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_IAGetInputLayout(ID3D11DeviceContext *iface,
        ID3D11InputLayout **input_layout)
{
    FIXME("iface %p, input_layout %p stub!\n", iface, input_layout);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_IAGetVertexBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers, UINT *strides, UINT *offsets)
{
    FIXME("iface %p, start_slot %u, buffer_count %u, buffers %p, strides %p, offsets %p stub!\n",
            iface, start_slot, buffer_count, buffers, strides, offsets);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_IAGetIndexBuffer(ID3D11DeviceContext *iface,
        ID3D11Buffer **buffer, DXGI_FORMAT *format, UINT *offset)
{
    FIXME("iface %p, buffer %p, format %p, offset %p stub!\n", iface, buffer, format, offset);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    FIXME("iface %p, start_slot %u, buffer_count %u, buffers %p stub!\n",
            iface, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GSGetShader(ID3D11DeviceContext *iface,
        ID3D11GeometryShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    FIXME("iface %p, shader %p, class_instances %p, class_instance_count %p stub!\n",
            iface, shader, class_instances, class_instance_count);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_IAGetPrimitiveTopology(ID3D11DeviceContext *iface,
        D3D11_PRIMITIVE_TOPOLOGY *topology)
{
    FIXME("iface %p, topology %p stub!\n", iface, topology);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_VSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    FIXME("iface %p, start_slot %u, view_count %u, views %p stub!\n", iface, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_VSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    FIXME("iface %p, start_slot %u, sampler_count %u, samplers %p stub!\n",
            iface, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GetPredication(ID3D11DeviceContext *iface,
        ID3D11Predicate **predicate, BOOL *value)
{
    FIXME("iface %p, predicate %p, value %p stub!\n", iface, predicate, value);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    FIXME("iface %p, start_slot %u, view_count %u, views %p stub!\n", iface, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_GSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    FIXME("iface %p, start_slot %u, sampler_count %u, samplers %p stub!\n",
            iface, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_OMGetRenderTargets(ID3D11DeviceContext *iface,
        UINT render_target_view_count, ID3D11RenderTargetView **render_target_views,
        ID3D11DepthStencilView **depth_stencil_view)
{
    FIXME("iface %p, render_target_view_count %u, render_target_views %p, depth_stencil_view %p stub!\n",
            iface, render_target_view_count, render_target_views, depth_stencil_view);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_OMGetRenderTargetsAndUnorderedAccessViews(
        ID3D11DeviceContext *iface,
        UINT render_target_view_count, ID3D11RenderTargetView **render_target_views,
        ID3D11DepthStencilView **depth_stencil_view,
        UINT unordered_access_view_start_slot, UINT unordered_access_view_count,
        ID3D11UnorderedAccessView **unordered_access_views)
{
    FIXME("iface %p, render_target_view_count %u, render_target_views %p, depth_stencil_view %p, "
            "unordered_access_view_start_slot %u, unordered_access_view_count %u, "
            "unordered_access_views %p stub!\n",
            iface, render_target_view_count, render_target_views, depth_stencil_view,
            unordered_access_view_start_slot, unordered_access_view_count, unordered_access_views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_OMGetBlendState(ID3D11DeviceContext *iface,
        ID3D11BlendState **blend_state, FLOAT blend_factor[4], UINT *sample_mask)
{
    FIXME("iface %p, blend_state %p, blend_factor %p, sample_mask %p stub!\n",
            iface, blend_state, blend_factor, sample_mask);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_OMGetDepthStencilState(ID3D11DeviceContext *iface,
        ID3D11DepthStencilState **depth_stencil_state, UINT *stencil_ref)
{
    FIXME("iface %p, depth_stencil_state %p, stencil_ref %p stub!\n",
            iface, depth_stencil_state, stencil_ref);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_SOGetTargets(ID3D11DeviceContext *iface,
        UINT buffer_count, ID3D11Buffer **buffers)
{
    FIXME("iface %p, buffer_count %u, buffers %p stub!\n", iface, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_RSGetState(ID3D11DeviceContext *iface,
        ID3D11RasterizerState **rasterizer_state)
{
    FIXME("iface %p, rasterizer_state %p stub!\n", iface, rasterizer_state);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_RSGetViewports(ID3D11DeviceContext *iface,
        UINT *viewport_count, D3D11_VIEWPORT *viewports)
{
    FIXME("iface %p, viewport_count %p, viewports %p stub!\n", iface, viewport_count, viewports);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_RSGetScissorRects(ID3D11DeviceContext *iface,
        UINT *rect_count, D3D11_RECT *rects)
{
    FIXME("iface %p, rect_count %p, rects %p stub!\n", iface, rect_count, rects);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_HSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    FIXME("iface %p, start_slot %u, view_count %u, views %p stub!\n", iface, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_HSGetShader(ID3D11DeviceContext *iface,
        ID3D11HullShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    FIXME("iface %p, shader %p, class_instances %p, class_instance_count %p stub!\n",
            iface, shader, class_instances, class_instance_count);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_HSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    FIXME("iface %p, start_slot %u, sampler_count %u, samplers %p stub!\n",
            iface, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_HSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    FIXME("iface %p, start_slot %u, buffer_count %u, buffers %p stub!\n",
            iface, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    FIXME("iface %p, start_slot %u, view_count %u, views %p stub!\n",
            iface, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DSGetShader(ID3D11DeviceContext *iface,
        ID3D11DomainShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    FIXME("iface %p, shader %p, class_instances %p, class_instance_count %p stub!\n",
            iface, shader, class_instances, class_instance_count);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    FIXME("iface %p, start_slot %u, sampler_count %u, samplers %p stub!\n",
            iface, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_DSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    FIXME("iface %p, start_slot %u, buffer_count %u, buffers %p stub!\n",
            iface, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CSGetShaderResources(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11ShaderResourceView **views)
{
    FIXME("iface %p, start_slot %u, view_count %u, views %p stub!\n", iface, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CSGetUnorderedAccessViews(ID3D11DeviceContext *iface,
        UINT start_slot, UINT view_count, ID3D11UnorderedAccessView **views)
{
    FIXME("iface %p, start_slot %u, view_count %u, views %p stub!\n", iface, start_slot, view_count, views);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CSGetShader(ID3D11DeviceContext *iface,
        ID3D11ComputeShader **shader, ID3D11ClassInstance **class_instances, UINT *class_instance_count)
{
    FIXME("iface %p, shader %p, class_instances %p, class_instance_count %p stub!\n",
            iface, shader, class_instances, class_instance_count);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CSGetSamplers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT sampler_count, ID3D11SamplerState **samplers)
{
    FIXME("iface %p, start_slot %u, sampler_count %u, samplers %p stub!\n",
            iface, start_slot, sampler_count, samplers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_CSGetConstantBuffers(ID3D11DeviceContext *iface,
        UINT start_slot, UINT buffer_count, ID3D11Buffer **buffers)
{
    FIXME("iface %p, start_slot %u, buffer_count %u, buffer %p stub!\n",
            iface, start_slot, buffer_count, buffers);
}

static void STDMETHODCALLTYPE d3d11_deferred_context_ClearState(ID3D11DeviceContext *iface)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct deferred_call *call;

    TRACE("iface %p.\n", iface);

    if (!(call = add_deferred_call(context, 0)))
        return;

    call->cmd = DEFERRED_CLEARSTATE;
}

static void STDMETHODCALLTYPE d3d11_deferred_context_Flush(ID3D11DeviceContext *iface)
{
    FIXME("iface %p stub!\n", iface);
}

static D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE d3d11_deferred_context_GetType(ID3D11DeviceContext *iface)
{
    TRACE("iface %p.\n", iface);

    return D3D11_DEVICE_CONTEXT_DEFERRED;
}

static UINT STDMETHODCALLTYPE d3d11_deferred_context_GetContextFlags(ID3D11DeviceContext *iface)
{
    FIXME("iface %p stub!\n", iface);

    return 0;
}

static HRESULT STDMETHODCALLTYPE d3d11_deferred_context_FinishCommandList(ID3D11DeviceContext *iface,
        BOOL restore, ID3D11CommandList **command_list)
{
    struct d3d11_deferred_context *context = impl_from_deferred_ID3D11DeviceContext(iface);
    struct d3d11_command_list *object;

    TRACE("iface %p, restore %#x, command_list %p.\n", iface, restore, command_list);

    if (restore)
        FIXME("Restoring state is not supported\n");

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        return E_OUTOFMEMORY;

    object->ID3D11CommandList_iface.lpVtbl = &d3d11_command_list_vtbl;
    object->device = context->device;
    object->refcount = 1;

    list_init(&object->commands);
    list_move_tail(&object->commands, &context->commands);

    ID3D11Device_AddRef(context->device);
    wined3d_private_store_init(&object->private_store);

    *command_list = &object->ID3D11CommandList_iface;
    return S_OK;
}

static const struct ID3D11DeviceContextVtbl d3d11_deferred_context_vtbl =
{
    /* IUnknown methods */
    d3d11_deferred_context_QueryInterface,
    d3d11_deferred_context_AddRef,
    d3d11_deferred_context_Release,
    /* ID3D11DeviceChild methods */
    d3d11_deferred_context_GetDevice,
    d3d11_deferred_context_GetPrivateData,
    d3d11_deferred_context_SetPrivateData,
    d3d11_deferred_context_SetPrivateDataInterface,
    /* ID3D11DeviceContext methods */
    d3d11_deferred_context_VSSetConstantBuffers,
    d3d11_deferred_context_PSSetShaderResources,
    d3d11_deferred_context_PSSetShader,
    d3d11_deferred_context_PSSetSamplers,
    d3d11_deferred_context_VSSetShader,
    d3d11_deferred_context_DrawIndexed,
    d3d11_deferred_context_Draw,
    d3d11_deferred_context_Map,
    d3d11_deferred_context_Unmap,
    d3d11_deferred_context_PSSetConstantBuffers,
    d3d11_deferred_context_IASetInputLayout,
    d3d11_deferred_context_IASetVertexBuffers,
    d3d11_deferred_context_IASetIndexBuffer,
    d3d11_deferred_context_DrawIndexedInstanced,
    d3d11_deferred_context_DrawInstanced,
    d3d11_deferred_context_GSSetConstantBuffers,
    d3d11_deferred_context_GSSetShader,
    d3d11_deferred_context_IASetPrimitiveTopology,
    d3d11_deferred_context_VSSetShaderResources,
    d3d11_deferred_context_VSSetSamplers,
    d3d11_deferred_context_Begin,
    d3d11_deferred_context_End,
    d3d11_deferred_context_GetData,
    d3d11_deferred_context_SetPredication,
    d3d11_deferred_context_GSSetShaderResources,
    d3d11_deferred_context_GSSetSamplers,
    d3d11_deferred_context_OMSetRenderTargets,
    d3d11_deferred_context_OMSetRenderTargetsAndUnorderedAccessViews,
    d3d11_deferred_context_OMSetBlendState,
    d3d11_deferred_context_OMSetDepthStencilState,
    d3d11_deferred_context_SOSetTargets,
    d3d11_deferred_context_DrawAuto,
    d3d11_deferred_context_DrawIndexedInstancedIndirect,
    d3d11_deferred_context_DrawInstancedIndirect,
    d3d11_deferred_context_Dispatch,
    d3d11_deferred_context_DispatchIndirect,
    d3d11_deferred_context_RSSetState,
    d3d11_deferred_context_RSSetViewports,
    d3d11_deferred_context_RSSetScissorRects,
    d3d11_deferred_context_CopySubresourceRegion,
    d3d11_deferred_context_CopyResource,
    d3d11_deferred_context_UpdateSubresource,
    d3d11_deferred_context_CopyStructureCount,
    d3d11_deferred_context_ClearRenderTargetView,
    d3d11_deferred_context_ClearUnorderedAccessViewUint,
    d3d11_deferred_context_ClearUnorderedAccessViewFloat,
    d3d11_deferred_context_ClearDepthStencilView,
    d3d11_deferred_context_GenerateMips,
    d3d11_deferred_context_SetResourceMinLOD,
    d3d11_deferred_context_GetResourceMinLOD,
    d3d11_deferred_context_ResolveSubresource,
    d3d11_deferred_context_ExecuteCommandList,
    d3d11_deferred_context_HSSetShaderResources,
    d3d11_deferred_context_HSSetShader,
    d3d11_deferred_context_HSSetSamplers,
    d3d11_deferred_context_HSSetConstantBuffers,
    d3d11_deferred_context_DSSetShaderResources,
    d3d11_deferred_context_DSSetShader,
    d3d11_deferred_context_DSSetSamplers,
    d3d11_deferred_context_DSSetConstantBuffers,
    d3d11_deferred_context_CSSetShaderResources,
    d3d11_deferred_context_CSSetUnorderedAccessViews,
    d3d11_deferred_context_CSSetShader,
    d3d11_deferred_context_CSSetSamplers,
    d3d11_deferred_context_CSSetConstantBuffers,
    d3d11_deferred_context_VSGetConstantBuffers,
    d3d11_deferred_context_PSGetShaderResources,
    d3d11_deferred_context_PSGetShader,
    d3d11_deferred_context_PSGetSamplers,
    d3d11_deferred_context_VSGetShader,
    d3d11_deferred_context_PSGetConstantBuffers,
    d3d11_deferred_context_IAGetInputLayout,
    d3d11_deferred_context_IAGetVertexBuffers,
    d3d11_deferred_context_IAGetIndexBuffer,
    d3d11_deferred_context_GSGetConstantBuffers,
    d3d11_deferred_context_GSGetShader,
    d3d11_deferred_context_IAGetPrimitiveTopology,
    d3d11_deferred_context_VSGetShaderResources,
    d3d11_deferred_context_VSGetSamplers,
    d3d11_deferred_context_GetPredication,
    d3d11_deferred_context_GSGetShaderResources,
    d3d11_deferred_context_GSGetSamplers,
    d3d11_deferred_context_OMGetRenderTargets,
    d3d11_deferred_context_OMGetRenderTargetsAndUnorderedAccessViews,
    d3d11_deferred_context_OMGetBlendState,
    d3d11_deferred_context_OMGetDepthStencilState,
    d3d11_deferred_context_SOGetTargets,
    d3d11_deferred_context_RSGetState,
    d3d11_deferred_context_RSGetViewports,
    d3d11_deferred_context_RSGetScissorRects,
    d3d11_deferred_context_HSGetShaderResources,
    d3d11_deferred_context_HSGetShader,
    d3d11_deferred_context_HSGetSamplers,
    d3d11_deferred_context_HSGetConstantBuffers,
    d3d11_deferred_context_DSGetShaderResources,
    d3d11_deferred_context_DSGetShader,
    d3d11_deferred_context_DSGetSamplers,
    d3d11_deferred_context_DSGetConstantBuffers,
    d3d11_deferred_context_CSGetShaderResources,
    d3d11_deferred_context_CSGetUnorderedAccessViews,
    d3d11_deferred_context_CSGetShader,
    d3d11_deferred_context_CSGetSamplers,
    d3d11_deferred_context_CSGetConstantBuffers,
    d3d11_deferred_context_ClearState,
    d3d11_deferred_context_Flush,
    d3d11_deferred_context_GetType,
    d3d11_deferred_context_GetContextFlags,
    d3d11_deferred_context_FinishCommandList,
};

/* ID3D11Device methods */

static HRESULT STDMETHODCALLTYPE d3d11_device_QueryInterface(ID3D11Device *iface, REFIID riid, void **out)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    return IUnknown_QueryInterface(device->outer_unk, riid, out);
}

static ULONG STDMETHODCALLTYPE d3d11_device_AddRef(ID3D11Device *iface)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    return IUnknown_AddRef(device->outer_unk);
}

static ULONG STDMETHODCALLTYPE d3d11_device_Release(ID3D11Device *iface)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    return IUnknown_Release(device->outer_unk);
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateBuffer(ID3D11Device *iface, const D3D11_BUFFER_DESC *desc,
        const D3D11_SUBRESOURCE_DATA *data, ID3D11Buffer **buffer)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_buffer *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, data %p, buffer %p.\n", iface, desc, data, buffer);

    if (FAILED(hr = d3d_buffer_create(device, desc, data, &object)))
        return hr;

    *buffer = &object->ID3D11Buffer_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateTexture1D(ID3D11Device *iface,
        const D3D11_TEXTURE1D_DESC *desc, const D3D11_SUBRESOURCE_DATA *data, ID3D11Texture1D **texture)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_texture1d *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, data %p, texture %p.\n", iface, desc, data, texture);

    if (FAILED(hr = d3d_texture1d_create(device, desc, data, &object)))
        return hr;

    *texture = &object->ID3D11Texture1D_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateTexture2D(ID3D11Device *iface,
        const D3D11_TEXTURE2D_DESC *desc, const D3D11_SUBRESOURCE_DATA *data, ID3D11Texture2D **texture)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_texture2d *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, data %p, texture %p.\n", iface, desc, data, texture);

    if (FAILED(hr = d3d_texture2d_create(device, desc, data, &object)))
        return hr;

    *texture = &object->ID3D11Texture2D_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateTexture3D(ID3D11Device *iface,
        const D3D11_TEXTURE3D_DESC *desc, const D3D11_SUBRESOURCE_DATA *data, ID3D11Texture3D **texture)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_texture3d *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, data %p, texture %p.\n", iface, desc, data, texture);

    if (FAILED(hr = d3d_texture3d_create(device, desc, data, &object)))
        return hr;

    *texture = &object->ID3D11Texture3D_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateShaderResourceView(ID3D11Device *iface,
        ID3D11Resource *resource, const D3D11_SHADER_RESOURCE_VIEW_DESC *desc, ID3D11ShaderResourceView **view)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_shader_resource_view *object;
    HRESULT hr;

    TRACE("iface %p, resource %p, desc %p, view %p.\n", iface, resource, desc, view);

    if (!resource)
        return E_INVALIDARG;

    if (FAILED(hr = d3d_shader_resource_view_create(device, resource, desc, &object)))
        return hr;

    *view = &object->ID3D11ShaderResourceView_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateUnorderedAccessView(ID3D11Device *iface,
        ID3D11Resource *resource, const D3D11_UNORDERED_ACCESS_VIEW_DESC *desc, ID3D11UnorderedAccessView **view)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d11_unordered_access_view *object;
    HRESULT hr;

    TRACE("iface %p, resource %p, desc %p, view %p.\n", iface, resource, desc, view);

    if (FAILED(hr = d3d11_unordered_access_view_create(device, resource, desc, &object)))
        return hr;

    *view = &object->ID3D11UnorderedAccessView_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateRenderTargetView(ID3D11Device *iface,
        ID3D11Resource *resource, const D3D11_RENDER_TARGET_VIEW_DESC *desc, ID3D11RenderTargetView **view)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_rendertarget_view *object;
    HRESULT hr;

    TRACE("iface %p, resource %p, desc %p, view %p.\n", iface, resource, desc, view);

    if (!resource)
        return E_INVALIDARG;

    if (FAILED(hr = d3d_rendertarget_view_create(device, resource, desc, &object)))
        return hr;

    *view = &object->ID3D11RenderTargetView_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDepthStencilView(ID3D11Device *iface,
        ID3D11Resource *resource, const D3D11_DEPTH_STENCIL_VIEW_DESC *desc, ID3D11DepthStencilView **view)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_depthstencil_view *object;
    HRESULT hr;

    TRACE("iface %p, resource %p, desc %p, view %p.\n", iface, resource, desc, view);

    if (FAILED(hr = d3d_depthstencil_view_create(device, resource, desc, &object)))
        return hr;

    *view = &object->ID3D11DepthStencilView_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateInputLayout(ID3D11Device *iface,
        const D3D11_INPUT_ELEMENT_DESC *element_descs, UINT element_count, const void *shader_byte_code,
        SIZE_T shader_byte_code_length, ID3D11InputLayout **input_layout)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_input_layout *object;
    HRESULT hr;

    TRACE("iface %p, element_descs %p, element_count %u, shader_byte_code %p, shader_byte_code_length %lu, "
            "input_layout %p.\n", iface, element_descs, element_count, shader_byte_code,
            shader_byte_code_length, input_layout);

    if (FAILED(hr = d3d_input_layout_create(device, element_descs, element_count,
            shader_byte_code, shader_byte_code_length, &object)))
        return hr;

    *input_layout = &object->ID3D11InputLayout_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateVertexShader(ID3D11Device *iface, const void *byte_code,
        SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11VertexShader **shader)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_vertex_shader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, class_linkage %p, shader %p.\n",
            iface, byte_code, byte_code_length, class_linkage, shader);

    if (class_linkage)
        FIXME("Class linkage is not implemented yet.\n");

    if (FAILED(hr = d3d_vertex_shader_create(device, byte_code, byte_code_length, &object)))
        return hr;

    *shader = &object->ID3D11VertexShader_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateGeometryShader(ID3D11Device *iface, const void *byte_code,
        SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11GeometryShader **shader)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_geometry_shader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, class_linkage %p, shader %p.\n",
            iface, byte_code, byte_code_length, class_linkage, shader);

    if (class_linkage)
        FIXME("Class linkage is not implemented yet.\n");

    if (FAILED(hr = d3d_geometry_shader_create(device, byte_code, byte_code_length,
            NULL, 0, NULL, 0, 0, &object)))
        return hr;

    *shader = &object->ID3D11GeometryShader_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateGeometryShaderWithStreamOutput(ID3D11Device *iface,
        const void *byte_code, SIZE_T byte_code_length, const D3D11_SO_DECLARATION_ENTRY *so_entries,
        UINT entry_count, const UINT *buffer_strides, UINT strides_count, UINT rasterizer_stream,
        ID3D11ClassLinkage *class_linkage, ID3D11GeometryShader **shader)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_geometry_shader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, so_entries %p, entry_count %u, "
            "buffer_strides %p, strides_count %u, rasterizer_stream %u, class_linkage %p, shader %p.\n",
            iface, byte_code, byte_code_length, so_entries, entry_count, buffer_strides, strides_count,
            rasterizer_stream, class_linkage, shader);

    if (class_linkage)
        FIXME("Class linkage is not implemented yet.\n");

    if (FAILED(hr = d3d_geometry_shader_create(device, byte_code, byte_code_length,
            so_entries, entry_count, buffer_strides, strides_count, rasterizer_stream, &object)))
    {
        *shader = NULL;
        return hr;
    }

    *shader = &object->ID3D11GeometryShader_iface;

    return hr;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreatePixelShader(ID3D11Device *iface, const void *byte_code,
        SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11PixelShader **shader)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_pixel_shader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, class_linkage %p, shader %p.\n",
            iface, byte_code, byte_code_length, class_linkage, shader);

    if (class_linkage)
        FIXME("Class linkage is not implemented yet.\n");

    if (FAILED(hr = d3d_pixel_shader_create(device, byte_code, byte_code_length, &object)))
        return hr;

    *shader = &object->ID3D11PixelShader_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateHullShader(ID3D11Device *iface, const void *byte_code,
        SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11HullShader **shader)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d11_hull_shader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, class_linkage %p, shader %p.\n",
            iface, byte_code, byte_code_length, class_linkage, shader);

    if (class_linkage)
        FIXME("Class linkage is not implemented yet.\n");

    if (FAILED(hr = d3d11_hull_shader_create(device, byte_code, byte_code_length, &object)))
        return hr;

    *shader = &object->ID3D11HullShader_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDomainShader(ID3D11Device *iface, const void *byte_code,
        SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11DomainShader **shader)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d11_domain_shader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, class_linkage %p, shader %p.\n",
            iface, byte_code, byte_code_length, class_linkage, shader);

    if (class_linkage)
        FIXME("Class linkage is not implemented yet.\n");

    if (FAILED(hr = d3d11_domain_shader_create(device, byte_code, byte_code_length, &object)))
        return hr;

    *shader = &object->ID3D11DomainShader_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateComputeShader(ID3D11Device *iface, const void *byte_code,
        SIZE_T byte_code_length, ID3D11ClassLinkage *class_linkage, ID3D11ComputeShader **shader)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d11_compute_shader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, class_linkage %p, shader %p.\n",
            iface, byte_code, byte_code_length, class_linkage, shader);

    if (class_linkage)
        FIXME("Class linkage is not implemented yet.\n");

    if (FAILED(hr = d3d11_compute_shader_create(device, byte_code, byte_code_length, &object)))
        return hr;

    *shader = &object->ID3D11ComputeShader_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateClassLinkage(ID3D11Device *iface,
        ID3D11ClassLinkage **class_linkage)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d11_class_linkage *object;
    HRESULT hr;

    TRACE("iface %p, class_linkage %p.\n", iface, class_linkage);

    if (FAILED(hr = d3d11_class_linkage_create(device, &object)))
        return hr;

    *class_linkage = &object->ID3D11ClassLinkage_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateBlendState(ID3D11Device *iface,
        const D3D11_BLEND_DESC *desc, ID3D11BlendState **blend_state)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_blend_state *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, blend_state %p.\n", iface, desc, blend_state);

    if (FAILED(hr = d3d_blend_state_create(device, desc, &object)))
        return hr;

    *blend_state = &object->ID3D11BlendState_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDepthStencilState(ID3D11Device *iface,
        const D3D11_DEPTH_STENCIL_DESC *desc, ID3D11DepthStencilState **depth_stencil_state)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_depthstencil_state *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, depth_stencil_state %p.\n", iface, desc, depth_stencil_state);

    if (FAILED(hr = d3d_depthstencil_state_create(device, desc, &object)))
        return hr;

    *depth_stencil_state = &object->ID3D11DepthStencilState_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateRasterizerState(ID3D11Device *iface,
        const D3D11_RASTERIZER_DESC *desc, ID3D11RasterizerState **rasterizer_state)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_rasterizer_state *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, rasterizer_state %p.\n", iface, desc, rasterizer_state);

    if (FAILED(hr = d3d_rasterizer_state_create(device, desc, &object)))
        return hr;

    *rasterizer_state = &object->ID3D11RasterizerState_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateSamplerState(ID3D11Device *iface,
        const D3D11_SAMPLER_DESC *desc, ID3D11SamplerState **sampler_state)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_sampler_state *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, sampler_state %p.\n", iface, desc, sampler_state);

    if (FAILED(hr = d3d_sampler_state_create(device, desc, &object)))
        return hr;

    *sampler_state = &object->ID3D11SamplerState_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateQuery(ID3D11Device *iface,
        const D3D11_QUERY_DESC *desc, ID3D11Query **query)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_query *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, query %p.\n", iface, desc, query);

    if (FAILED(hr = d3d_query_create(device, desc, FALSE, &object)))
        return hr;

    if (query)
    {
        *query = &object->ID3D11Query_iface;
        return S_OK;
    }

    ID3D11Query_Release(&object->ID3D11Query_iface);
    return S_FALSE;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreatePredicate(ID3D11Device *iface, const D3D11_QUERY_DESC *desc,
        ID3D11Predicate **predicate)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct d3d_query *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, predicate %p.\n", iface, desc, predicate);

    if (FAILED(hr = d3d_query_create(device, desc, TRUE, &object)))
        return hr;

    if (predicate)
    {
        *predicate = (ID3D11Predicate *)&object->ID3D11Query_iface;
        return S_OK;
    }

    ID3D11Query_Release(&object->ID3D11Query_iface);
    return S_FALSE;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateCounter(ID3D11Device *iface, const D3D11_COUNTER_DESC *desc,
        ID3D11Counter **counter)
{
    FIXME("iface %p, desc %p, counter %p stub!\n", iface, desc, counter);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CreateDeferredContext(ID3D11Device *iface, UINT flags,
        ID3D11DeviceContext **context)
{
    struct d3d11_deferred_context *object;

    TRACE("iface %p, flags %#x, context %p.\n", iface, flags, context);

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        return E_OUTOFMEMORY;

    object->ID3D11DeviceContext_iface.lpVtbl = &d3d11_deferred_context_vtbl;
    object->device = iface;
    object->refcount = 1;

    list_init(&object->commands);

    ID3D11Device_AddRef(iface);
    wined3d_private_store_init(&object->private_store);

    *context = &object->ID3D11DeviceContext_iface;
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_OpenSharedResource(ID3D11Device *iface, HANDLE resource, REFIID riid,
        void **out)
{
    FIXME("iface %p, resource %p, riid %s, out %p stub!\n", iface, resource, debugstr_guid(riid), out);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CheckFormatSupport(ID3D11Device *iface, DXGI_FORMAT format,
        UINT *format_support)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct wined3d_device_creation_parameters params;
    enum wined3d_format_id wined3d_format;
    struct wined3d *wined3d;
    unsigned int i;

    static const struct
    {
        enum wined3d_resource_type rtype;
        unsigned int usage;
        D3D11_FORMAT_SUPPORT flag;
    }
    flag_mapping[] =
    {
        {WINED3D_RTYPE_TEXTURE_1D, WINED3DUSAGE_TEXTURE,             D3D11_FORMAT_SUPPORT_TEXTURE1D | D3D11_FORMAT_SUPPORT_MIP},
        {WINED3D_RTYPE_TEXTURE_2D, WINED3DUSAGE_TEXTURE,             D3D11_FORMAT_SUPPORT_TEXTURE2D | D3D11_FORMAT_SUPPORT_MIP},
        {WINED3D_RTYPE_TEXTURE_3D, WINED3DUSAGE_TEXTURE,             D3D11_FORMAT_SUPPORT_TEXTURE3D | D3D11_FORMAT_SUPPORT_MIP},
        {WINED3D_RTYPE_NONE,       WINED3DUSAGE_RENDERTARGET,        D3D11_FORMAT_SUPPORT_RENDER_TARGET | D3D11_FORMAT_SUPPORT_DISPLAY},
        {WINED3D_RTYPE_NONE,       WINED3DUSAGE_DEPTHSTENCIL,        D3D11_FORMAT_SUPPORT_DEPTH_STENCIL},
        {WINED3D_RTYPE_BUFFER,     WINED3DUSAGE_QUERY_VERTEXTEXTURE, D3D11_FORMAT_SUPPORT_BUFFER |
            D3D11_FORMAT_SUPPORT_IA_VERTEX_BUFFER | D3D11_FORMAT_SUPPORT_IA_INDEX_BUFFER | D3D11_FORMAT_SUPPORT_SO_BUFFER
        },
        {WINED3D_RTYPE_TEXTURE_2D, WINED3DUSAGE_TEXTURE | WINED3DUSAGE_LEGACY_CUBEMAP, D3D11_FORMAT_SUPPORT_TEXTURECUBE},
        {WINED3D_RTYPE_TEXTURE_2D, WINED3DUSAGE_TEXTURE | WINED3DUSAGE_AUTOGENMIPMAP,  D3D11_FORMAT_SUPPORT_MIP_AUTOGEN},
        {WINED3D_RTYPE_TEXTURE_2D, WINED3DUSAGE_TEXTURE | WINED3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, D3D11_FORMAT_SUPPORT_BLENDABLE},
    };
    HRESULT hr;

    FIXME("iface %p, format %u, format_support %p partial-stub!\n", iface, format, format_support);

    wined3d_format = wined3dformat_from_dxgi_format(format);
    if (format && !wined3d_format)
    {
        WARN("Invalid format %#x.\n", format);
        *format_support = 0;
        return E_FAIL;
    }

    *format_support = 0;

    wined3d_mutex_lock();
    wined3d = wined3d_device_get_wined3d(device->wined3d_device);
    wined3d_device_get_creation_parameters(device->wined3d_device, &params);
    for (i = 0; i < ARRAY_SIZE(flag_mapping); ++i)
    {
        hr = wined3d_check_device_format(wined3d, params.adapter_idx, params.device_type,
                WINED3DFMT_UNKNOWN, flag_mapping[i].usage, flag_mapping[i].rtype, wined3d_format);
        if (hr == WINED3DERR_NOTAVAILABLE || hr == WINED3DOK_NOAUTOGEN)
            continue;
        if (hr != WINED3D_OK)
        {
            WARN("Failed to check device format support, hr %#x.\n", hr);
            wined3d_mutex_unlock();
            return E_FAIL;
        }

        *format_support |= flag_mapping[i].flag;
    }
    wined3d_mutex_unlock();

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CheckMultisampleQualityLevels(ID3D11Device *iface,
        DXGI_FORMAT format, UINT sample_count, UINT *quality_level_count)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    struct wined3d_device_creation_parameters params;
    struct wined3d *wined3d;
    HRESULT hr;

    TRACE("iface %p, format %s, sample_count %u, quality_level_count %p.\n",
            iface, debug_dxgi_format(format), sample_count, quality_level_count);

    if (!quality_level_count)
        return E_INVALIDARG;

    *quality_level_count = 0;

    if (!sample_count)
        return E_FAIL;
    if (sample_count == 1)
    {
        *quality_level_count = 1;
        return S_OK;
    }
    if (sample_count > D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT)
        return E_FAIL;

    wined3d_mutex_lock();
    wined3d = wined3d_device_get_wined3d(device->wined3d_device);
    wined3d_device_get_creation_parameters(device->wined3d_device, &params);
    hr = wined3d_check_device_multisample_type(wined3d, params.adapter_idx, params.device_type,
            wined3dformat_from_dxgi_format(format), TRUE, sample_count, quality_level_count);
    wined3d_mutex_unlock();

    if (hr == WINED3DERR_INVALIDCALL)
        return E_INVALIDARG;
    if (hr == WINED3DERR_NOTAVAILABLE)
        return S_OK;
    return hr;
}

static void STDMETHODCALLTYPE d3d11_device_CheckCounterInfo(ID3D11Device *iface, D3D11_COUNTER_INFO *info)
{
    FIXME("iface %p, info %p stub!\n", iface, info);
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CheckCounter(ID3D11Device *iface, const D3D11_COUNTER_DESC *desc,
        D3D11_COUNTER_TYPE *type, UINT *active_counter_count, char *name, UINT *name_length,
        char *units, UINT *units_length, char *description, UINT *description_length)
{
    FIXME("iface %p, desc %p, type %p, active_counter_count %p, name %p, name_length %p, "
            "units %p, units_length %p, description %p, description_length %p stub!\n",
            iface, desc, type, active_counter_count, name, name_length,
            units, units_length, description, description_length);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_CheckFeatureSupport(ID3D11Device *iface, D3D11_FEATURE feature,
        void *feature_support_data, UINT feature_support_data_size)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);
    WINED3DCAPS wined3d_caps;
    HRESULT hr;

    TRACE("iface %p, feature %u, feature_support_data %p, feature_support_data_size %u.\n",
            iface, feature, feature_support_data, feature_support_data_size);

    switch (feature)
    {
        case D3D11_FEATURE_THREADING:
        {
            D3D11_FEATURE_DATA_THREADING *threading_data = feature_support_data;
            if (feature_support_data_size != sizeof(*threading_data))
            {
                WARN("Invalid data size.\n");
                return E_INVALIDARG;
            }

            /* We lie about the threading support to make Tomb Raider 2013 and
             * Deus Ex: Human Revolution happy. */
            FIXME("Returning fake threading support data.\n");
            threading_data->DriverConcurrentCreates = TRUE;
            threading_data->DriverCommandLists = TRUE;
            return S_OK;
        }

        case D3D11_FEATURE_DOUBLES:
        {
            D3D11_FEATURE_DATA_DOUBLES *doubles_data = feature_support_data;
            if (feature_support_data_size != sizeof(*doubles_data))
            {
                WARN("Invalid data size.\n");
                return E_INVALIDARG;
            }

            wined3d_mutex_lock();
            hr = wined3d_device_get_device_caps(device->wined3d_device, &wined3d_caps);
            wined3d_mutex_unlock();
            if (FAILED(hr))
            {
                WARN("Failed to get device caps, hr %#x.\n", hr);
                return hr;
            }

            doubles_data->DoublePrecisionFloatShaderOps = wined3d_caps.shader_double_precision;
            return S_OK;
        }

        case D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS:
        {
            D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS *options = feature_support_data;
            if (feature_support_data_size != sizeof(*options))
            {
                WARN("Invalid data size.\n");
                return E_INVALIDARG;
            }

            options->ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x = FALSE;
            return S_OK;
        }

        default:
            FIXME("Unhandled feature %#x.\n", feature);
            return E_NOTIMPL;
    }
}

static HRESULT STDMETHODCALLTYPE d3d11_device_GetPrivateData(ID3D11Device *iface, REFGUID guid,
        UINT *data_size, void *data)
{
    IDXGIDevice *dxgi_device;
    HRESULT hr;

    TRACE("iface %p, guid %s, data_size %p, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    if (FAILED(hr = ID3D11Device_QueryInterface(iface, &IID_IDXGIDevice, (void **)&dxgi_device)))
        return hr;
    hr = IDXGIDevice_GetPrivateData(dxgi_device, guid, data_size, data);
    IDXGIDevice_Release(dxgi_device);

    return hr;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_SetPrivateData(ID3D11Device *iface, REFGUID guid,
        UINT data_size, const void *data)
{
    IDXGIDevice *dxgi_device;
    HRESULT hr;

    TRACE("iface %p, guid %s, data_size %u, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    if (FAILED(hr = ID3D11Device_QueryInterface(iface, &IID_IDXGIDevice, (void **)&dxgi_device)))
        return hr;
    hr = IDXGIDevice_SetPrivateData(dxgi_device, guid, data_size, data);
    IDXGIDevice_Release(dxgi_device);

    return hr;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_SetPrivateDataInterface(ID3D11Device *iface, REFGUID guid,
        const IUnknown *data)
{
    IDXGIDevice *dxgi_device;
    HRESULT hr;

    TRACE("iface %p, guid %s, data %p.\n", iface, debugstr_guid(guid), data);

    if (FAILED(hr = ID3D11Device_QueryInterface(iface, &IID_IDXGIDevice, (void **)&dxgi_device)))
        return hr;
    hr = IDXGIDevice_SetPrivateDataInterface(dxgi_device, guid, data);
    IDXGIDevice_Release(dxgi_device);

    return hr;
}

static D3D_FEATURE_LEVEL STDMETHODCALLTYPE d3d11_device_GetFeatureLevel(ID3D11Device *iface)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);

    TRACE("iface %p.\n", iface);

    return device->feature_level;
}

static UINT STDMETHODCALLTYPE d3d11_device_GetCreationFlags(ID3D11Device *iface)
{
    FIXME("iface %p stub!\n", iface);

    return 0;
}

static HRESULT STDMETHODCALLTYPE d3d11_device_GetDeviceRemovedReason(ID3D11Device *iface)
{
    TRACE("iface %p stub!\n", iface);

    return S_OK;
}

static void STDMETHODCALLTYPE d3d11_device_GetImmediateContext(ID3D11Device *iface,
        ID3D11DeviceContext **immediate_context)
{
    struct d3d_device *device = impl_from_ID3D11Device(iface);

    TRACE("iface %p, immediate_context %p.\n", iface, immediate_context);

    *immediate_context = &device->immediate_context.ID3D11DeviceContext_iface;
    ID3D11DeviceContext_AddRef(*immediate_context);
}

static HRESULT STDMETHODCALLTYPE d3d11_device_SetExceptionMode(ID3D11Device *iface, UINT flags)
{
    FIXME("iface %p, flags %#x stub!\n", iface, flags);

    return E_NOTIMPL;
}

static UINT STDMETHODCALLTYPE d3d11_device_GetExceptionMode(ID3D11Device *iface)
{
    FIXME("iface %p stub!\n", iface);

    return 0;
}

static const struct ID3D11DeviceVtbl d3d11_device_vtbl =
{
    /* IUnknown methods */
    d3d11_device_QueryInterface,
    d3d11_device_AddRef,
    d3d11_device_Release,
    /* ID3D11Device methods */
    d3d11_device_CreateBuffer,
    d3d11_device_CreateTexture1D,
    d3d11_device_CreateTexture2D,
    d3d11_device_CreateTexture3D,
    d3d11_device_CreateShaderResourceView,
    d3d11_device_CreateUnorderedAccessView,
    d3d11_device_CreateRenderTargetView,
    d3d11_device_CreateDepthStencilView,
    d3d11_device_CreateInputLayout,
    d3d11_device_CreateVertexShader,
    d3d11_device_CreateGeometryShader,
    d3d11_device_CreateGeometryShaderWithStreamOutput,
    d3d11_device_CreatePixelShader,
    d3d11_device_CreateHullShader,
    d3d11_device_CreateDomainShader,
    d3d11_device_CreateComputeShader,
    d3d11_device_CreateClassLinkage,
    d3d11_device_CreateBlendState,
    d3d11_device_CreateDepthStencilState,
    d3d11_device_CreateRasterizerState,
    d3d11_device_CreateSamplerState,
    d3d11_device_CreateQuery,
    d3d11_device_CreatePredicate,
    d3d11_device_CreateCounter,
    d3d11_device_CreateDeferredContext,
    d3d11_device_OpenSharedResource,
    d3d11_device_CheckFormatSupport,
    d3d11_device_CheckMultisampleQualityLevels,
    d3d11_device_CheckCounterInfo,
    d3d11_device_CheckCounter,
    d3d11_device_CheckFeatureSupport,
    d3d11_device_GetPrivateData,
    d3d11_device_SetPrivateData,
    d3d11_device_SetPrivateDataInterface,
    d3d11_device_GetFeatureLevel,
    d3d11_device_GetCreationFlags,
    d3d11_device_GetDeviceRemovedReason,
    d3d11_device_GetImmediateContext,
    d3d11_device_SetExceptionMode,
    d3d11_device_GetExceptionMode,
};

/* Inner IUnknown methods */

static inline struct d3d_device *impl_from_IUnknown(IUnknown *iface)
{
    return CONTAINING_RECORD(iface, struct d3d_device, IUnknown_inner);
}

static HRESULT STDMETHODCALLTYPE d3d_device_inner_QueryInterface(IUnknown *iface, REFIID riid, void **out)
{
    struct d3d_device *device = impl_from_IUnknown(iface);

    TRACE("iface %p, riid %s, out %p.\n", iface, debugstr_guid(riid), out);

    if (IsEqualGUID(riid, &IID_ID3D11Device)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        *out = &device->ID3D11Device_iface;
    }
    else if (IsEqualGUID(riid, &IID_ID3D10Device1)
            || IsEqualGUID(riid, &IID_ID3D10Device))
    {
        *out = &device->ID3D10Device1_iface;
    }
    else if (IsEqualGUID(riid, &IID_ID3D10Multithread))
    {
        *out = &device->ID3D10Multithread_iface;
    }
    else if (IsEqualGUID(riid, &IID_IWineDXGIDeviceParent))
    {
        *out = &device->IWineDXGIDeviceParent_iface;
    }
    else if (IsEqualGUID(riid, &IID_IWineD3DDevice))
    {
        *out = device->wined3d_device;
        return S_OK;
    }
    else
    {
        WARN("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(riid));
        *out = NULL;
        return E_NOINTERFACE;
    }

    IUnknown_AddRef((IUnknown *)*out);
    return S_OK;
}

static ULONG STDMETHODCALLTYPE d3d_device_inner_AddRef(IUnknown *iface)
{
    struct d3d_device *device = impl_from_IUnknown(iface);
    ULONG refcount = InterlockedIncrement(&device->refcount);

    TRACE("%p increasing refcount to %u.\n", device, refcount);

    return refcount;
}

static ULONG STDMETHODCALLTYPE d3d_device_inner_Release(IUnknown *iface)
{
    struct d3d_device *device = impl_from_IUnknown(iface);
    ULONG refcount = InterlockedDecrement(&device->refcount);

    TRACE("%p decreasing refcount to %u.\n", device, refcount);

    if (!refcount)
    {
        d3d11_immediate_context_destroy(&device->immediate_context);
        if (device->wined3d_device)
        {
            wined3d_mutex_lock();
            wined3d_device_decref(device->wined3d_device);
            wined3d_mutex_unlock();
        }
        wine_rb_destroy(&device->sampler_states, NULL, NULL);
        wine_rb_destroy(&device->rasterizer_states, NULL, NULL);
        wine_rb_destroy(&device->depthstencil_states, NULL, NULL);
        wine_rb_destroy(&device->blend_states, NULL, NULL);
    }

    return refcount;
}

/* IUnknown methods */

static HRESULT STDMETHODCALLTYPE d3d10_device_QueryInterface(ID3D10Device1 *iface, REFIID riid,
        void **ppv)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    return IUnknown_QueryInterface(device->outer_unk, riid, ppv);
}

static ULONG STDMETHODCALLTYPE d3d10_device_AddRef(ID3D10Device1 *iface)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    return IUnknown_AddRef(device->outer_unk);
}

static ULONG STDMETHODCALLTYPE d3d10_device_Release(ID3D10Device1 *iface)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    return IUnknown_Release(device->outer_unk);
}

/* ID3D10Device methods */

static void STDMETHODCALLTYPE d3d10_device_VSSetConstantBuffers(ID3D10Device1 *iface,
        UINT start_slot, UINT buffer_count, ID3D10Buffer *const *buffers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D10Buffer(buffers[i]);

        wined3d_device_set_vs_cb(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_PSSetShaderResources(ID3D10Device1 *iface,
        UINT start_slot, UINT view_count, ID3D10ShaderResourceView *const *views)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D10ShaderResourceView(views[i]);

        wined3d_device_set_ps_resource_view(device->wined3d_device, start_slot + i,
                view ? view->wined3d_view : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_PSSetShader(ID3D10Device1 *iface,
        ID3D10PixelShader *shader)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_pixel_shader *ps = unsafe_impl_from_ID3D10PixelShader(shader);

    TRACE("iface %p, shader %p\n", iface, shader);

    wined3d_mutex_lock();
    wined3d_device_set_pixel_shader(device->wined3d_device, ps ? ps->wined3d_shader : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_PSSetSamplers(ID3D10Device1 *iface,
        UINT start_slot, UINT sampler_count, ID3D10SamplerState *const *samplers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler = unsafe_impl_from_ID3D10SamplerState(samplers[i]);

        wined3d_device_set_ps_sampler(device->wined3d_device, start_slot + i,
                sampler ? sampler->wined3d_sampler : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_VSSetShader(ID3D10Device1 *iface,
        ID3D10VertexShader *shader)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_vertex_shader *vs = unsafe_impl_from_ID3D10VertexShader(shader);

    TRACE("iface %p, shader %p\n", iface, shader);

    wined3d_mutex_lock();
    wined3d_device_set_vertex_shader(device->wined3d_device, vs ? vs->wined3d_shader : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_DrawIndexed(ID3D10Device1 *iface, UINT index_count,
        UINT start_index_location, INT base_vertex_location)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, index_count %u, start_index_location %u, base_vertex_location %d.\n",
            iface, index_count, start_index_location, base_vertex_location);

    wined3d_mutex_lock();
    wined3d_device_set_base_vertex_index(device->wined3d_device, base_vertex_location);
    wined3d_device_draw_indexed_primitive(device->wined3d_device, start_index_location, index_count);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_Draw(ID3D10Device1 *iface, UINT vertex_count,
        UINT start_vertex_location)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, vertex_count %u, start_vertex_location %u\n",
            iface, vertex_count, start_vertex_location);

    wined3d_mutex_lock();
    wined3d_device_draw_primitive(device->wined3d_device, start_vertex_location, vertex_count);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_PSSetConstantBuffers(ID3D10Device1 *iface,
        UINT start_slot, UINT buffer_count, ID3D10Buffer *const *buffers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D10Buffer(buffers[i]);

        wined3d_device_set_ps_cb(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_IASetInputLayout(ID3D10Device1 *iface,
        ID3D10InputLayout *input_layout)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_input_layout *layout = unsafe_impl_from_ID3D10InputLayout(input_layout);

    TRACE("iface %p, input_layout %p\n", iface, input_layout);

    wined3d_mutex_lock();
    wined3d_device_set_vertex_declaration(device->wined3d_device,
            layout ? layout->wined3d_decl : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_IASetVertexBuffers(ID3D10Device1 *iface, UINT start_slot,
        UINT buffer_count, ID3D10Buffer *const *buffers, const UINT *strides, const UINT *offsets)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p, strides %p, offsets %p\n",
            iface, start_slot, buffer_count, buffers, strides, offsets);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D10Buffer(buffers[i]);

        wined3d_device_set_stream_source(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL, offsets[i], strides[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_IASetIndexBuffer(ID3D10Device1 *iface,
        ID3D10Buffer *buffer, DXGI_FORMAT format, UINT offset)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_buffer *buffer_impl = unsafe_impl_from_ID3D10Buffer(buffer);

    TRACE("iface %p, buffer %p, format %s, offset %u.\n",
            iface, buffer, debug_dxgi_format(format), offset);

    wined3d_mutex_lock();
    wined3d_device_set_index_buffer(device->wined3d_device,
            buffer_impl ? buffer_impl->wined3d_buffer : NULL,
            wined3dformat_from_dxgi_format(format), offset);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_DrawIndexedInstanced(ID3D10Device1 *iface,
        UINT instance_index_count, UINT instance_count, UINT start_index_location,
        INT base_vertex_location, UINT start_instance_location)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, instance_index_count %u, instance_count %u, start_index_location %u, "
            "base_vertex_location %d, start_instance_location %u.\n",
            iface, instance_index_count, instance_count, start_index_location,
            base_vertex_location, start_instance_location);

    wined3d_mutex_lock();
    wined3d_device_set_base_vertex_index(device->wined3d_device, base_vertex_location);
    wined3d_device_draw_indexed_primitive_instanced(device->wined3d_device, start_index_location,
            instance_index_count, start_instance_location, instance_count);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_DrawInstanced(ID3D10Device1 *iface,
        UINT instance_vertex_count, UINT instance_count,
        UINT start_vertex_location, UINT start_instance_location)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, instance_vertex_count %u, instance_count %u, start_vertex_location %u, "
            "start_instance_location %u.\n", iface, instance_vertex_count, instance_count,
            start_vertex_location, start_instance_location);

    wined3d_mutex_lock();
    wined3d_device_draw_primitive_instanced(device->wined3d_device, start_vertex_location,
            instance_vertex_count, start_instance_location, instance_count);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_GSSetConstantBuffers(ID3D10Device1 *iface,
        UINT start_slot, UINT buffer_count, ID3D10Buffer *const *buffers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D10Buffer(buffers[i]);

        wined3d_device_set_gs_cb(device->wined3d_device, start_slot + i,
                buffer ? buffer->wined3d_buffer : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_GSSetShader(ID3D10Device1 *iface, ID3D10GeometryShader *shader)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_geometry_shader *gs = unsafe_impl_from_ID3D10GeometryShader(shader);

    TRACE("iface %p, shader %p.\n", iface, shader);

    wined3d_mutex_lock();
    wined3d_device_set_geometry_shader(device->wined3d_device, gs ? gs->wined3d_shader : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_IASetPrimitiveTopology(ID3D10Device1 *iface,
        D3D10_PRIMITIVE_TOPOLOGY topology)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, topology %s.\n", iface, debug_d3d10_primitive_topology(topology));

    wined3d_mutex_lock();
    wined3d_device_set_primitive_type(device->wined3d_device, (enum wined3d_primitive_type)topology, 0);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_VSSetShaderResources(ID3D10Device1 *iface,
        UINT start_slot, UINT view_count, ID3D10ShaderResourceView *const *views)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D10ShaderResourceView(views[i]);

        wined3d_device_set_vs_resource_view(device->wined3d_device, start_slot + i,
                view ? view->wined3d_view : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_VSSetSamplers(ID3D10Device1 *iface,
        UINT start_slot, UINT sampler_count, ID3D10SamplerState *const *samplers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler = unsafe_impl_from_ID3D10SamplerState(samplers[i]);

        wined3d_device_set_vs_sampler(device->wined3d_device, start_slot + i,
                sampler ? sampler->wined3d_sampler : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_SetPredication(ID3D10Device1 *iface, ID3D10Predicate *predicate, BOOL value)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_query *query;

    TRACE("iface %p, predicate %p, value %#x.\n", iface, predicate, value);

    query = unsafe_impl_from_ID3D10Query((ID3D10Query *)predicate);
    wined3d_mutex_lock();
    wined3d_device_set_predication(device->wined3d_device, query ? query->wined3d_query : NULL, value);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_GSSetShaderResources(ID3D10Device1 *iface,
        UINT start_slot, UINT view_count, ID3D10ShaderResourceView *const *views)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D10ShaderResourceView(views[i]);

        wined3d_device_set_gs_resource_view(device->wined3d_device, start_slot + i,
                view ? view->wined3d_view : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_GSSetSamplers(ID3D10Device1 *iface,
        UINT start_slot, UINT sampler_count, ID3D10SamplerState *const *samplers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler = unsafe_impl_from_ID3D10SamplerState(samplers[i]);

        wined3d_device_set_gs_sampler(device->wined3d_device, start_slot + i,
                sampler ? sampler->wined3d_sampler : NULL);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_OMSetRenderTargets(ID3D10Device1 *iface,
        UINT render_target_view_count, ID3D10RenderTargetView *const *render_target_views,
        ID3D10DepthStencilView *depth_stencil_view)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_depthstencil_view *dsv;
    unsigned int i;

    TRACE("iface %p, render_target_view_count %u, render_target_views %p, depth_stencil_view %p.\n",
            iface, render_target_view_count, render_target_views, depth_stencil_view);

    wined3d_mutex_lock();
    for (i = 0; i < render_target_view_count; ++i)
    {
        struct d3d_rendertarget_view *rtv = unsafe_impl_from_ID3D10RenderTargetView(render_target_views[i]);

        wined3d_device_set_rendertarget_view(device->wined3d_device, i,
                rtv ? rtv->wined3d_view : NULL, FALSE);
    }
    for (; i < D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
    {
        wined3d_device_set_rendertarget_view(device->wined3d_device, i, NULL, FALSE);
    }

    dsv = unsafe_impl_from_ID3D10DepthStencilView(depth_stencil_view);
    wined3d_device_set_depth_stencil_view(device->wined3d_device,
            dsv ? dsv->wined3d_view : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_OMSetBlendState(ID3D10Device1 *iface,
        ID3D10BlendState *blend_state, const float blend_factor[4], UINT sample_mask)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_blend_state *blend_state_object;

    TRACE("iface %p, blend_state %p, blend_factor %s, sample_mask 0x%08x.\n",
            iface, blend_state, debug_float4(blend_factor), sample_mask);

    blend_state_object = unsafe_impl_from_ID3D10BlendState(blend_state);
    d3d11_immediate_context_OMSetBlendState(&device->immediate_context.ID3D11DeviceContext_iface,
            blend_state_object ? &blend_state_object->ID3D11BlendState_iface : NULL, blend_factor, sample_mask);
}

static void STDMETHODCALLTYPE d3d10_device_OMSetDepthStencilState(ID3D10Device1 *iface,
        ID3D10DepthStencilState *depth_stencil_state, UINT stencil_ref)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_depthstencil_state *ds_state_object;

    TRACE("iface %p, depth_stencil_state %p, stencil_ref %u.\n",
            iface, depth_stencil_state, stencil_ref);

    ds_state_object = unsafe_impl_from_ID3D10DepthStencilState(depth_stencil_state);
    d3d11_immediate_context_OMSetDepthStencilState(&device->immediate_context.ID3D11DeviceContext_iface,
            ds_state_object ? &ds_state_object->ID3D11DepthStencilState_iface : NULL, stencil_ref);
}

static void STDMETHODCALLTYPE d3d10_device_SOSetTargets(ID3D10Device1 *iface,
        UINT target_count, ID3D10Buffer *const *targets, const UINT *offsets)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int count, i;

    TRACE("iface %p, target_count %u, targets %p, offsets %p.\n", iface, target_count, targets, offsets);

    count = min(target_count, D3D10_SO_BUFFER_SLOT_COUNT);
    wined3d_mutex_lock();
    for (i = 0; i < count; ++i)
    {
        struct d3d_buffer *buffer = unsafe_impl_from_ID3D10Buffer(targets[i]);

        wined3d_device_set_stream_output(device->wined3d_device, i,
                buffer ? buffer->wined3d_buffer : NULL, offsets[i]);
    }

    for (i = count; i < D3D10_SO_BUFFER_SLOT_COUNT; ++i)
    {
        wined3d_device_set_stream_output(device->wined3d_device, i, NULL, 0);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_DrawAuto(ID3D10Device1 *iface)
{
    FIXME("iface %p stub!\n", iface);
}

static void STDMETHODCALLTYPE d3d10_device_RSSetState(ID3D10Device1 *iface, ID3D10RasterizerState *rasterizer_state)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_rasterizer_state *rasterizer_state_object;

    TRACE("iface %p, rasterizer_state %p.\n", iface, rasterizer_state);

    rasterizer_state_object = unsafe_impl_from_ID3D10RasterizerState(rasterizer_state);
    d3d11_immediate_context_RSSetState(&device->immediate_context.ID3D11DeviceContext_iface,
            rasterizer_state_object ? &rasterizer_state_object->ID3D11RasterizerState_iface : NULL);
}

static void STDMETHODCALLTYPE d3d10_device_RSSetViewports(ID3D10Device1 *iface,
        UINT viewport_count, const D3D10_VIEWPORT *viewports)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct wined3d_viewport wined3d_vp;

    TRACE("iface %p, viewport_count %u, viewports %p.\n", iface, viewport_count, viewports);

    if (viewport_count > 1)
        FIXME("Multiple viewports not implemented.\n");

    if (!viewport_count)
        return;

    wined3d_vp.x = viewports[0].TopLeftX;
    wined3d_vp.y = viewports[0].TopLeftY;
    wined3d_vp.width = viewports[0].Width;
    wined3d_vp.height = viewports[0].Height;
    wined3d_vp.min_z = viewports[0].MinDepth;
    wined3d_vp.max_z = viewports[0].MaxDepth;

    wined3d_mutex_lock();
    wined3d_device_set_viewport(device->wined3d_device, &wined3d_vp);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_RSSetScissorRects(ID3D10Device1 *iface,
        UINT rect_count, const D3D10_RECT *rects)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, rect_count %u, rects %p.\n", iface, rect_count, rects);

    if (rect_count > 1)
        FIXME("Multiple scissor rects not implemented.\n");

    if (!rect_count)
        return;

    wined3d_mutex_lock();
    wined3d_device_set_scissor_rect(device->wined3d_device, rects);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_CopySubresourceRegion(ID3D10Device1 *iface,
        ID3D10Resource *dst_resource, UINT dst_subresource_idx, UINT dst_x, UINT dst_y, UINT dst_z,
        ID3D10Resource *src_resource, UINT src_subresource_idx, const D3D10_BOX *src_box)
{
    struct wined3d_resource *wined3d_dst_resource, *wined3d_src_resource;
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct wined3d_box wined3d_src_box;

    TRACE("iface %p, dst_resource %p, dst_subresource_idx %u, dst_x %u, dst_y %u, dst_z %u, "
            "src_resource %p, src_subresource_idx %u, src_box %p.\n",
            iface, dst_resource, dst_subresource_idx, dst_x, dst_y, dst_z,
            src_resource, src_subresource_idx, src_box);

    if (src_box)
        wined3d_box_set(&wined3d_src_box, src_box->left, src_box->top,
                src_box->right, src_box->bottom, src_box->front, src_box->back);

    wined3d_dst_resource = wined3d_resource_from_d3d10_resource(dst_resource);
    wined3d_src_resource = wined3d_resource_from_d3d10_resource(src_resource);
    wined3d_mutex_lock();
    wined3d_device_copy_sub_resource_region(device->wined3d_device, wined3d_dst_resource, dst_subresource_idx,
            dst_x, dst_y, dst_z, wined3d_src_resource, src_subresource_idx, src_box ? &wined3d_src_box : NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_CopyResource(ID3D10Device1 *iface,
        ID3D10Resource *dst_resource, ID3D10Resource *src_resource)
{
    struct wined3d_resource *wined3d_dst_resource, *wined3d_src_resource;
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, dst_resource %p, src_resource %p.\n", iface, dst_resource, src_resource);

    wined3d_dst_resource = wined3d_resource_from_d3d10_resource(dst_resource);
    wined3d_src_resource = wined3d_resource_from_d3d10_resource(src_resource);
    wined3d_mutex_lock();
    wined3d_device_copy_resource(device->wined3d_device, wined3d_dst_resource, wined3d_src_resource);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_UpdateSubresource(ID3D10Device1 *iface,
        ID3D10Resource *resource, UINT subresource_idx, const D3D10_BOX *box,
        const void *data, UINT row_pitch, UINT depth_pitch)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    ID3D11Resource *d3d11_resource;

    TRACE("iface %p, resource %p, subresource_idx %u, box %p, data %p, row_pitch %u, depth_pitch %u.\n",
            iface, resource, subresource_idx, box, data, row_pitch, depth_pitch);

    ID3D10Resource_QueryInterface(resource, &IID_ID3D11Resource, (void **)&d3d11_resource);
    d3d11_immediate_context_UpdateSubresource(&device->immediate_context.ID3D11DeviceContext_iface,
            d3d11_resource, subresource_idx, (const D3D11_BOX *)box, data, row_pitch, depth_pitch);
    ID3D11Resource_Release(d3d11_resource);
}

static void STDMETHODCALLTYPE d3d10_device_ClearRenderTargetView(ID3D10Device1 *iface,
        ID3D10RenderTargetView *render_target_view, const float color_rgba[4])
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_rendertarget_view *view = unsafe_impl_from_ID3D10RenderTargetView(render_target_view);
    const struct wined3d_color color = {color_rgba[0], color_rgba[1], color_rgba[2], color_rgba[3]};
    HRESULT hr;

    TRACE("iface %p, render_target_view %p, color_rgba %s.\n",
            iface, render_target_view, debug_float4(color_rgba));

    if (!view)
        return;

    wined3d_mutex_lock();
    if (FAILED(hr = wined3d_device_clear_rendertarget_view(device->wined3d_device, view->wined3d_view, NULL,
            WINED3DCLEAR_TARGET, &color, 0.0f, 0)))
        ERR("Failed to clear view, hr %#x.\n", hr);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_ClearDepthStencilView(ID3D10Device1 *iface,
        ID3D10DepthStencilView *depth_stencil_view, UINT flags, FLOAT depth, UINT8 stencil)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_depthstencil_view *view = unsafe_impl_from_ID3D10DepthStencilView(depth_stencil_view);
    DWORD wined3d_flags;
    HRESULT hr;

    TRACE("iface %p, depth_stencil_view %p, flags %#x, depth %.8e, stencil %u.\n",
            iface, depth_stencil_view, flags, depth, stencil);

    if (!view)
        return;

    wined3d_flags = wined3d_clear_flags_from_d3d11_clear_flags(flags);

    wined3d_mutex_lock();
    if (FAILED(hr = wined3d_device_clear_rendertarget_view(device->wined3d_device, view->wined3d_view, NULL,
            wined3d_flags, NULL, depth, stencil)))
        ERR("Failed to clear view, hr %#x.\n", hr);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_GenerateMips(ID3D10Device1 *iface,
        ID3D10ShaderResourceView *shader_resource_view)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_shader_resource_view *view = unsafe_impl_from_ID3D10ShaderResourceView(shader_resource_view);

    TRACE("iface %p, shader_resource_view %p.\n", iface, shader_resource_view);

    if (!view)
        return;

    wined3d_mutex_lock();
    wined3d_device_generate_mips_view(device->wined3d_device, view->wined3d_view);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_ResolveSubresource(ID3D10Device1 *iface,
        ID3D10Resource *dst_resource, UINT dst_subresource_idx,
        ID3D10Resource *src_resource, UINT src_subresource_idx, DXGI_FORMAT format)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct wined3d_resource *wined3d_dst_resource, *wined3d_src_resource;

    TRACE("iface %p, dst_resource %p, dst_subresource_idx %u, src_resource %p, src_subresource_idx %u, "
            "format %s stub!\n",
            iface, dst_resource, dst_subresource_idx, src_resource, src_subresource_idx,
            debug_dxgi_format(format));

    wined3d_dst_resource = wined3d_resource_from_d3d10_resource(dst_resource);
    wined3d_src_resource = wined3d_resource_from_d3d10_resource(src_resource);

    /* Multisampled textures are not supported yet, so simply copy the sub resource */
    wined3d_mutex_lock();
    wined3d_device_copy_sub_resource_region(device->wined3d_device, wined3d_dst_resource, dst_subresource_idx,
            0, 0, 0, wined3d_src_resource, src_subresource_idx, NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_VSGetConstantBuffers(ID3D10Device1 *iface,
        UINT start_slot, UINT buffer_count, ID3D10Buffer **buffers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_vs_cb(device->wined3d_device, start_slot + i)))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        buffers[i] = &buffer_impl->ID3D10Buffer_iface;
        ID3D10Buffer_AddRef(buffers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_PSGetShaderResources(ID3D10Device1 *iface,
        UINT start_slot, UINT view_count, ID3D10ShaderResourceView **views)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct wined3d_shader_resource_view *wined3d_view;
        struct d3d_shader_resource_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_ps_resource_view(device->wined3d_device, start_slot + i)))
        {
            views[i] = NULL;
            continue;
        }

        view_impl = wined3d_shader_resource_view_get_parent(wined3d_view);
        views[i] = (ID3D10ShaderResourceView *)&view_impl->ID3D10ShaderResourceView1_iface;
        ID3D10ShaderResourceView_AddRef(views[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_PSGetShader(ID3D10Device1 *iface, ID3D10PixelShader **shader)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_pixel_shader *shader_impl;
    struct wined3d_shader *wined3d_shader;

    TRACE("iface %p, shader %p.\n", iface, shader);

    wined3d_mutex_lock();
    if (!(wined3d_shader = wined3d_device_get_pixel_shader(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *shader = NULL;
        return;
    }

    shader_impl = wined3d_shader_get_parent(wined3d_shader);
    wined3d_mutex_unlock();
    *shader = &shader_impl->ID3D10PixelShader_iface;
    ID3D10PixelShader_AddRef(*shader);
}

static void STDMETHODCALLTYPE d3d10_device_PSGetSamplers(ID3D10Device1 *iface,
        UINT start_slot, UINT sampler_count, ID3D10SamplerState **samplers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler_impl;
        struct wined3d_sampler *wined3d_sampler;

        if (!(wined3d_sampler = wined3d_device_get_ps_sampler(device->wined3d_device, start_slot + i)))
        {
            samplers[i] = NULL;
            continue;
        }

        sampler_impl = wined3d_sampler_get_parent(wined3d_sampler);
        samplers[i] = &sampler_impl->ID3D10SamplerState_iface;
        ID3D10SamplerState_AddRef(samplers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_VSGetShader(ID3D10Device1 *iface, ID3D10VertexShader **shader)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_vertex_shader *shader_impl;
    struct wined3d_shader *wined3d_shader;

    TRACE("iface %p, shader %p.\n", iface, shader);

    wined3d_mutex_lock();
    if (!(wined3d_shader = wined3d_device_get_vertex_shader(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *shader = NULL;
        return;
    }

    shader_impl = wined3d_shader_get_parent(wined3d_shader);
    wined3d_mutex_unlock();
    *shader = &shader_impl->ID3D10VertexShader_iface;
    ID3D10VertexShader_AddRef(*shader);
}

static void STDMETHODCALLTYPE d3d10_device_PSGetConstantBuffers(ID3D10Device1 *iface,
        UINT start_slot, UINT buffer_count, ID3D10Buffer **buffers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_ps_cb(device->wined3d_device, start_slot + i)))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        buffers[i] = &buffer_impl->ID3D10Buffer_iface;
        ID3D10Buffer_AddRef(buffers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_IAGetInputLayout(ID3D10Device1 *iface, ID3D10InputLayout **input_layout)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct wined3d_vertex_declaration *wined3d_declaration;
    struct d3d_input_layout *input_layout_impl;

    TRACE("iface %p, input_layout %p.\n", iface, input_layout);

    wined3d_mutex_lock();
    if (!(wined3d_declaration = wined3d_device_get_vertex_declaration(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *input_layout = NULL;
        return;
    }

    input_layout_impl = wined3d_vertex_declaration_get_parent(wined3d_declaration);
    wined3d_mutex_unlock();
    *input_layout = &input_layout_impl->ID3D10InputLayout_iface;
    ID3D10InputLayout_AddRef(*input_layout);
}

static void STDMETHODCALLTYPE d3d10_device_IAGetVertexBuffers(ID3D10Device1 *iface,
        UINT start_slot, UINT buffer_count, ID3D10Buffer **buffers, UINT *strides, UINT *offsets)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p, strides %p, offsets %p.\n",
            iface, start_slot, buffer_count, buffers, strides, offsets);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer = NULL;
        struct d3d_buffer *buffer_impl;

        if (FAILED(wined3d_device_get_stream_source(device->wined3d_device, start_slot + i,
                &wined3d_buffer, &offsets[i], &strides[i])))
            ERR("Failed to get vertex buffer.\n");

        if (!wined3d_buffer)
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        buffers[i] = &buffer_impl->ID3D10Buffer_iface;
        ID3D10Buffer_AddRef(buffers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_IAGetIndexBuffer(ID3D10Device1 *iface,
        ID3D10Buffer **buffer, DXGI_FORMAT *format, UINT *offset)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    enum wined3d_format_id wined3d_format;
    struct wined3d_buffer *wined3d_buffer;
    struct d3d_buffer *buffer_impl;

    TRACE("iface %p, buffer %p, format %p, offset %p.\n", iface, buffer, format, offset);

    wined3d_mutex_lock();
    wined3d_buffer = wined3d_device_get_index_buffer(device->wined3d_device, &wined3d_format, offset);
    *format = dxgi_format_from_wined3dformat(wined3d_format);
    if (!wined3d_buffer)
    {
        wined3d_mutex_unlock();
        *buffer = NULL;
        return;
    }

    buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
    wined3d_mutex_unlock();
    *buffer = &buffer_impl->ID3D10Buffer_iface;
    ID3D10Buffer_AddRef(*buffer);
}

static void STDMETHODCALLTYPE d3d10_device_GSGetConstantBuffers(ID3D10Device1 *iface,
        UINT start_slot, UINT buffer_count, ID3D10Buffer **buffers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, buffer_count %u, buffers %p.\n",
            iface, start_slot, buffer_count, buffers);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_gs_cb(device->wined3d_device, start_slot + i)))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        buffers[i] = &buffer_impl->ID3D10Buffer_iface;
        ID3D10Buffer_AddRef(buffers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_GSGetShader(ID3D10Device1 *iface, ID3D10GeometryShader **shader)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_geometry_shader *shader_impl;
    struct wined3d_shader *wined3d_shader;

    TRACE("iface %p, shader %p.\n", iface, shader);

    wined3d_mutex_lock();
    if (!(wined3d_shader = wined3d_device_get_geometry_shader(device->wined3d_device)))
    {
        wined3d_mutex_unlock();
        *shader = NULL;
        return;
    }

    shader_impl = wined3d_shader_get_parent(wined3d_shader);
    wined3d_mutex_unlock();
    *shader = &shader_impl->ID3D10GeometryShader_iface;
    ID3D10GeometryShader_AddRef(*shader);
}

static void STDMETHODCALLTYPE d3d10_device_IAGetPrimitiveTopology(ID3D10Device1 *iface,
        D3D10_PRIMITIVE_TOPOLOGY *topology)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, topology %p.\n", iface, topology);

    wined3d_mutex_lock();
    wined3d_device_get_primitive_type(device->wined3d_device, (enum wined3d_primitive_type *)topology, NULL);
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_VSGetShaderResources(ID3D10Device1 *iface,
        UINT start_slot, UINT view_count, ID3D10ShaderResourceView **views)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct wined3d_shader_resource_view *wined3d_view;
        struct d3d_shader_resource_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_vs_resource_view(device->wined3d_device, start_slot + i)))
        {
            views[i] = NULL;
            continue;
        }

        view_impl = wined3d_shader_resource_view_get_parent(wined3d_view);
        views[i] = (ID3D10ShaderResourceView *)&view_impl->ID3D10ShaderResourceView1_iface;
        ID3D10ShaderResourceView_AddRef(views[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_VSGetSamplers(ID3D10Device1 *iface,
        UINT start_slot, UINT sampler_count, ID3D10SamplerState **samplers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler_impl;
        struct wined3d_sampler *wined3d_sampler;

        if (!(wined3d_sampler = wined3d_device_get_vs_sampler(device->wined3d_device, start_slot + i)))
        {
            samplers[i] = NULL;
            continue;
        }

        sampler_impl = wined3d_sampler_get_parent(wined3d_sampler);
        samplers[i] = &sampler_impl->ID3D10SamplerState_iface;
        ID3D10SamplerState_AddRef(samplers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_GetPredication(ID3D10Device1 *iface,
        ID3D10Predicate **predicate, BOOL *value)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct wined3d_query *wined3d_predicate;
    struct d3d_query *predicate_impl;

    TRACE("iface %p, predicate %p, value %p.\n", iface, predicate, value);

    wined3d_mutex_lock();
    if (!(wined3d_predicate = wined3d_device_get_predication(device->wined3d_device, value)))
    {
        wined3d_mutex_unlock();
        *predicate = NULL;
        return;
    }

    predicate_impl = wined3d_query_get_parent(wined3d_predicate);
    wined3d_mutex_unlock();
    *predicate = (ID3D10Predicate *)&predicate_impl->ID3D10Query_iface;
    ID3D10Predicate_AddRef(*predicate);
}

static void STDMETHODCALLTYPE d3d10_device_GSGetShaderResources(ID3D10Device1 *iface,
        UINT start_slot, UINT view_count, ID3D10ShaderResourceView **views)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, view_count %u, views %p.\n",
            iface, start_slot, view_count, views);

    wined3d_mutex_lock();
    for (i = 0; i < view_count; ++i)
    {
        struct wined3d_shader_resource_view *wined3d_view;
        struct d3d_shader_resource_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_gs_resource_view(device->wined3d_device, start_slot + i)))
        {
            views[i] = NULL;
            continue;
        }

        view_impl = wined3d_shader_resource_view_get_parent(wined3d_view);
        views[i] = (ID3D10ShaderResourceView *)&view_impl->ID3D10ShaderResourceView1_iface;
        ID3D10ShaderResourceView_AddRef(views[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_GSGetSamplers(ID3D10Device1 *iface,
        UINT start_slot, UINT sampler_count, ID3D10SamplerState **samplers)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, start_slot %u, sampler_count %u, samplers %p.\n",
            iface, start_slot, sampler_count, samplers);

    wined3d_mutex_lock();
    for (i = 0; i < sampler_count; ++i)
    {
        struct d3d_sampler_state *sampler_impl;
        struct wined3d_sampler *wined3d_sampler;

        if (!(wined3d_sampler = wined3d_device_get_gs_sampler(device->wined3d_device, start_slot + i)))
        {
            samplers[i] = NULL;
            continue;
        }

        sampler_impl = wined3d_sampler_get_parent(wined3d_sampler);
        samplers[i] = &sampler_impl->ID3D10SamplerState_iface;
        ID3D10SamplerState_AddRef(samplers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_OMGetRenderTargets(ID3D10Device1 *iface,
        UINT view_count, ID3D10RenderTargetView **render_target_views, ID3D10DepthStencilView **depth_stencil_view)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct wined3d_rendertarget_view *wined3d_view;

    TRACE("iface %p, view_count %u, render_target_views %p, depth_stencil_view %p.\n",
            iface, view_count, render_target_views, depth_stencil_view);

    wined3d_mutex_lock();
    if (render_target_views)
    {
        struct d3d_rendertarget_view *view_impl;
        unsigned int i;

        for (i = 0; i < view_count; ++i)
        {
            if (!(wined3d_view = wined3d_device_get_rendertarget_view(device->wined3d_device, i))
                    || !(view_impl = wined3d_rendertarget_view_get_parent(wined3d_view)))
            {
                render_target_views[i] = NULL;
                continue;
            }

            render_target_views[i] = &view_impl->ID3D10RenderTargetView_iface;
            ID3D10RenderTargetView_AddRef(render_target_views[i]);
        }
    }

    if (depth_stencil_view)
    {
        struct d3d_depthstencil_view *view_impl;

        if (!(wined3d_view = wined3d_device_get_depth_stencil_view(device->wined3d_device))
                || !(view_impl = wined3d_rendertarget_view_get_parent(wined3d_view)))
        {
            *depth_stencil_view = NULL;
        }
        else
        {
            *depth_stencil_view = &view_impl->ID3D10DepthStencilView_iface;
            ID3D10DepthStencilView_AddRef(*depth_stencil_view);
        }
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_OMGetBlendState(ID3D10Device1 *iface,
        ID3D10BlendState **blend_state, FLOAT blend_factor[4], UINT *sample_mask)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    ID3D11BlendState *d3d11_blend_state;

    TRACE("iface %p, blend_state %p, blend_factor %p, sample_mask %p.\n",
            iface, blend_state, blend_factor, sample_mask);

    d3d11_immediate_context_OMGetBlendState(&device->immediate_context.ID3D11DeviceContext_iface,
            &d3d11_blend_state, blend_factor, sample_mask);

    if (d3d11_blend_state)
        *blend_state = (ID3D10BlendState *)&impl_from_ID3D11BlendState(d3d11_blend_state)->ID3D10BlendState1_iface;
    else
        *blend_state = NULL;
}

static void STDMETHODCALLTYPE d3d10_device_OMGetDepthStencilState(ID3D10Device1 *iface,
        ID3D10DepthStencilState **depth_stencil_state, UINT *stencil_ref)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    ID3D11DepthStencilState *d3d11_iface;

    TRACE("iface %p, depth_stencil_state %p, stencil_ref %p.\n",
            iface, depth_stencil_state, stencil_ref);

    d3d11_immediate_context_OMGetDepthStencilState(&device->immediate_context.ID3D11DeviceContext_iface,
            &d3d11_iface, stencil_ref);

    if (d3d11_iface)
        *depth_stencil_state = &impl_from_ID3D11DepthStencilState(d3d11_iface)->ID3D10DepthStencilState_iface;
    else
        *depth_stencil_state = NULL;
}

static void STDMETHODCALLTYPE d3d10_device_SOGetTargets(ID3D10Device1 *iface,
        UINT buffer_count, ID3D10Buffer **buffers, UINT *offsets)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    unsigned int i;

    TRACE("iface %p, buffer_count %u, buffers %p, offsets %p.\n",
            iface, buffer_count, buffers, offsets);

    wined3d_mutex_lock();
    for (i = 0; i < buffer_count; ++i)
    {
        struct wined3d_buffer *wined3d_buffer;
        struct d3d_buffer *buffer_impl;

        if (!(wined3d_buffer = wined3d_device_get_stream_output(device->wined3d_device, i, &offsets[i])))
        {
            buffers[i] = NULL;
            continue;
        }

        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        buffers[i] = &buffer_impl->ID3D10Buffer_iface;
        ID3D10Buffer_AddRef(buffers[i]);
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_RSGetState(ID3D10Device1 *iface, ID3D10RasterizerState **rasterizer_state)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_rasterizer_state *rasterizer_state_impl;
    struct wined3d_rasterizer_state *wined3d_state;

    TRACE("iface %p, rasterizer_state %p.\n", iface, rasterizer_state);

    wined3d_mutex_lock();
    if ((wined3d_state = wined3d_device_get_rasterizer_state(device->wined3d_device)))
    {
        rasterizer_state_impl = wined3d_rasterizer_state_get_parent(wined3d_state);
        ID3D10RasterizerState_AddRef(*rasterizer_state = &rasterizer_state_impl->ID3D10RasterizerState_iface);
    }
    else
    {
        *rasterizer_state = NULL;
    }
    wined3d_mutex_unlock();
}

static void STDMETHODCALLTYPE d3d10_device_RSGetViewports(ID3D10Device1 *iface,
        UINT *viewport_count, D3D10_VIEWPORT *viewports)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct wined3d_viewport wined3d_vp;

    TRACE("iface %p, viewport_count %p, viewports %p.\n", iface, viewport_count, viewports);

    if (!viewports)
    {
        *viewport_count = 1;
        return;
    }

    if (!*viewport_count)
        return;

    wined3d_mutex_lock();
    wined3d_device_get_viewport(device->wined3d_device, &wined3d_vp);
    wined3d_mutex_unlock();

    viewports[0].TopLeftX = wined3d_vp.x;
    viewports[0].TopLeftY = wined3d_vp.y;
    viewports[0].Width = wined3d_vp.width;
    viewports[0].Height = wined3d_vp.height;
    viewports[0].MinDepth = wined3d_vp.min_z;
    viewports[0].MaxDepth = wined3d_vp.max_z;

    if (*viewport_count > 1)
        memset(&viewports[1], 0, (*viewport_count - 1) * sizeof(*viewports));
}

static void STDMETHODCALLTYPE d3d10_device_RSGetScissorRects(ID3D10Device1 *iface, UINT *rect_count, D3D10_RECT *rects)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, rect_count %p, rects %p.\n", iface, rect_count, rects);

    if (!rects)
    {
        *rect_count = 1;
        return;
    }

    if (!*rect_count)
        return;

    wined3d_mutex_lock();
    wined3d_device_get_scissor_rect(device->wined3d_device, rects);
    wined3d_mutex_unlock();
    if (*rect_count > 1)
        memset(&rects[1], 0, (*rect_count - 1) * sizeof(*rects));
}

static HRESULT STDMETHODCALLTYPE d3d10_device_GetDeviceRemovedReason(ID3D10Device1 *iface)
{
    TRACE("iface %p.\n", iface);

    /* In the current implementation the device is never removed, so we can
     * just return S_OK here. */

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_SetExceptionMode(ID3D10Device1 *iface, UINT flags)
{
    FIXME("iface %p, flags %#x stub!\n", iface, flags);

    return E_NOTIMPL;
}

static UINT STDMETHODCALLTYPE d3d10_device_GetExceptionMode(ID3D10Device1 *iface)
{
    FIXME("iface %p stub!\n", iface);

    return 0;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_GetPrivateData(ID3D10Device1 *iface,
        REFGUID guid, UINT *data_size, void *data)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, guid %s, data_size %p, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return d3d11_device_GetPrivateData(&device->ID3D11Device_iface, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d10_device_SetPrivateData(ID3D10Device1 *iface,
        REFGUID guid, UINT data_size, const void *data)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, guid %s, data_size %u, data %p.\n", iface, debugstr_guid(guid), data_size, data);

    return d3d11_device_SetPrivateData(&device->ID3D11Device_iface, guid, data_size, data);
}

static HRESULT STDMETHODCALLTYPE d3d10_device_SetPrivateDataInterface(ID3D10Device1 *iface,
        REFGUID guid, const IUnknown *data)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, guid %s, data %p.\n", iface, debugstr_guid(guid), data);

    return d3d11_device_SetPrivateDataInterface(&device->ID3D11Device_iface, guid, data);
}

static void STDMETHODCALLTYPE d3d10_device_ClearState(ID3D10Device1 *iface)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p.\n", iface);

    d3d11_immediate_context_ClearState(&device->immediate_context.ID3D11DeviceContext_iface);
}

static void STDMETHODCALLTYPE d3d10_device_Flush(ID3D10Device1 *iface)
{
    FIXME("iface %p stub!\n", iface);
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateBuffer(ID3D10Device1 *iface,
        const D3D10_BUFFER_DESC *desc, const D3D10_SUBRESOURCE_DATA *data, ID3D10Buffer **buffer)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    D3D11_BUFFER_DESC d3d11_desc;
    struct d3d_buffer *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, data %p, buffer %p.\n", iface, desc, data, buffer);

    d3d11_desc.ByteWidth = desc->ByteWidth;
    d3d11_desc.Usage = d3d11_usage_from_d3d10_usage(desc->Usage);
    d3d11_desc.BindFlags = d3d11_bind_flags_from_d3d10_bind_flags(desc->BindFlags);
    d3d11_desc.CPUAccessFlags = d3d11_cpu_access_flags_from_d3d10_cpu_access_flags(desc->CPUAccessFlags);
    d3d11_desc.MiscFlags = d3d11_resource_misc_flags_from_d3d10_resource_misc_flags(desc->MiscFlags);
    d3d11_desc.StructureByteStride = 0;

    if (FAILED(hr = d3d_buffer_create(device, &d3d11_desc, (const D3D11_SUBRESOURCE_DATA *)data, &object)))
        return hr;

    *buffer = &object->ID3D10Buffer_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateTexture1D(ID3D10Device1 *iface,
        const D3D10_TEXTURE1D_DESC *desc, const D3D10_SUBRESOURCE_DATA *data, ID3D10Texture1D **texture)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    D3D11_TEXTURE1D_DESC d3d11_desc;
    struct d3d_texture1d *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, data %p, texture %p.\n", iface, desc, data, texture);

    d3d11_desc.Width = desc->Width;
    d3d11_desc.MipLevels = desc->MipLevels;
    d3d11_desc.ArraySize = desc->ArraySize;
    d3d11_desc.Format = desc->Format;
    d3d11_desc.Usage = d3d11_usage_from_d3d10_usage(desc->Usage);
    d3d11_desc.BindFlags = d3d11_bind_flags_from_d3d10_bind_flags(desc->BindFlags);
    d3d11_desc.CPUAccessFlags = d3d11_cpu_access_flags_from_d3d10_cpu_access_flags(desc->CPUAccessFlags);
    d3d11_desc.MiscFlags = d3d11_resource_misc_flags_from_d3d10_resource_misc_flags(desc->MiscFlags);

    if (FAILED(hr = d3d_texture1d_create(device, &d3d11_desc, (const D3D11_SUBRESOURCE_DATA *)data, &object)))
        return hr;

    *texture = &object->ID3D10Texture1D_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateTexture2D(ID3D10Device1 *iface,
        const D3D10_TEXTURE2D_DESC *desc, const D3D10_SUBRESOURCE_DATA *data,
        ID3D10Texture2D **texture)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    D3D11_TEXTURE2D_DESC d3d11_desc;
    struct d3d_texture2d *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, data %p, texture %p.\n", iface, desc, data, texture);

    d3d11_desc.Width = desc->Width;
    d3d11_desc.Height = desc->Height;
    d3d11_desc.MipLevels = desc->MipLevels;
    d3d11_desc.ArraySize = desc->ArraySize;
    d3d11_desc.Format = desc->Format;
    d3d11_desc.SampleDesc = desc->SampleDesc;
    d3d11_desc.Usage = d3d11_usage_from_d3d10_usage(desc->Usage);
    d3d11_desc.BindFlags = d3d11_bind_flags_from_d3d10_bind_flags(desc->BindFlags);
    d3d11_desc.CPUAccessFlags = d3d11_cpu_access_flags_from_d3d10_cpu_access_flags(desc->CPUAccessFlags);
    d3d11_desc.MiscFlags = d3d11_resource_misc_flags_from_d3d10_resource_misc_flags(desc->MiscFlags);

    if (FAILED(hr = d3d_texture2d_create(device, &d3d11_desc, (const D3D11_SUBRESOURCE_DATA *)data, &object)))
        return hr;

    *texture = &object->ID3D10Texture2D_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateTexture3D(ID3D10Device1 *iface,
        const D3D10_TEXTURE3D_DESC *desc, const D3D10_SUBRESOURCE_DATA *data,
        ID3D10Texture3D **texture)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    D3D11_TEXTURE3D_DESC d3d11_desc;
    struct d3d_texture3d *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, data %p, texture %p.\n", iface, desc, data, texture);

    d3d11_desc.Width = desc->Width;
    d3d11_desc.Height = desc->Height;
    d3d11_desc.Depth = desc->Depth;
    d3d11_desc.MipLevels = desc->MipLevels;
    d3d11_desc.Format = desc->Format;
    d3d11_desc.Usage = d3d11_usage_from_d3d10_usage(desc->Usage);
    d3d11_desc.BindFlags = d3d11_bind_flags_from_d3d10_bind_flags(desc->BindFlags);
    d3d11_desc.CPUAccessFlags = d3d11_cpu_access_flags_from_d3d10_cpu_access_flags(desc->CPUAccessFlags);
    d3d11_desc.MiscFlags = d3d11_resource_misc_flags_from_d3d10_resource_misc_flags(desc->MiscFlags);

    if (FAILED(hr = d3d_texture3d_create(device, &d3d11_desc, (const D3D11_SUBRESOURCE_DATA *)data, &object)))
        return hr;

    *texture = &object->ID3D10Texture3D_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateShaderResourceView1(ID3D10Device1 *iface,
        ID3D10Resource *resource, const D3D10_SHADER_RESOURCE_VIEW_DESC1 *desc, ID3D10ShaderResourceView1 **view)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_shader_resource_view *object;
    ID3D11Resource *d3d11_resource;
    HRESULT hr;

    TRACE("iface %p, resource %p, desc %p, view %p.\n", iface, resource, desc, view);

    if (!resource)
        return E_INVALIDARG;

    if (FAILED(hr = ID3D10Resource_QueryInterface(resource, &IID_ID3D11Resource, (void **)&d3d11_resource)))
    {
        ERR("Resource does not implement ID3D11Resource.\n");
        return E_FAIL;
    }

    hr = d3d_shader_resource_view_create(device, d3d11_resource, (const D3D11_SHADER_RESOURCE_VIEW_DESC *)desc,
            &object);
    ID3D11Resource_Release(d3d11_resource);
    if (FAILED(hr))
        return hr;

    *view = &object->ID3D10ShaderResourceView1_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateShaderResourceView(ID3D10Device1 *iface,
        ID3D10Resource *resource, const D3D10_SHADER_RESOURCE_VIEW_DESC *desc, ID3D10ShaderResourceView **view)
{
    TRACE("iface %p, resource %p, desc %p, view %p.\n", iface, resource, desc, view);

    return d3d10_device_CreateShaderResourceView1(iface, resource,
            (const D3D10_SHADER_RESOURCE_VIEW_DESC1 *)desc, (ID3D10ShaderResourceView1 **)view);
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateRenderTargetView(ID3D10Device1 *iface,
        ID3D10Resource *resource, const D3D10_RENDER_TARGET_VIEW_DESC *desc, ID3D10RenderTargetView **view)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_rendertarget_view *object;
    ID3D11Resource *d3d11_resource;
    HRESULT hr;

    TRACE("iface %p, resource %p, desc %p, view %p.\n", iface, resource, desc, view);

    if (!resource)
        return E_INVALIDARG;

    if (FAILED(hr = ID3D10Resource_QueryInterface(resource, &IID_ID3D11Resource, (void **)&d3d11_resource)))
    {
        ERR("Resource does not implement ID3D11Resource.\n");
        return E_FAIL;
    }

    hr = d3d_rendertarget_view_create(device, d3d11_resource, (const D3D11_RENDER_TARGET_VIEW_DESC *)desc, &object);
    ID3D11Resource_Release(d3d11_resource);
    if (FAILED(hr))
        return hr;

    *view = &object->ID3D10RenderTargetView_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateDepthStencilView(ID3D10Device1 *iface,
        ID3D10Resource *resource, const D3D10_DEPTH_STENCIL_VIEW_DESC *desc, ID3D10DepthStencilView **view)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    D3D11_DEPTH_STENCIL_VIEW_DESC d3d11_desc;
    struct d3d_depthstencil_view *object;
    ID3D11Resource *d3d11_resource;
    HRESULT hr;

    TRACE("iface %p, resource %p, desc %p, view %p.\n", iface, resource, desc, view);

    if (desc)
    {
        d3d11_desc.Format = desc->Format;
        d3d11_desc.ViewDimension = desc->ViewDimension;
        d3d11_desc.Flags = 0;
        memcpy(&d3d11_desc.u, &desc->u, sizeof(d3d11_desc.u));
    }

    if (FAILED(hr = ID3D10Resource_QueryInterface(resource, &IID_ID3D11Resource, (void **)&d3d11_resource)))
    {
        ERR("Resource does not implement ID3D11Resource.\n");
        return E_FAIL;
    }

    hr = d3d_depthstencil_view_create(device, d3d11_resource, desc ? &d3d11_desc : NULL, &object);
    ID3D11Resource_Release(d3d11_resource);
    if (FAILED(hr))
        return hr;

    *view = &object->ID3D10DepthStencilView_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateInputLayout(ID3D10Device1 *iface,
        const D3D10_INPUT_ELEMENT_DESC *element_descs, UINT element_count,
        const void *shader_byte_code, SIZE_T shader_byte_code_length,
        ID3D10InputLayout **input_layout)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_input_layout *object;
    HRESULT hr;

    TRACE("iface %p, element_descs %p, element_count %u, shader_byte_code %p, "
            "shader_byte_code_length %lu, input_layout %p\n",
            iface, element_descs, element_count, shader_byte_code,
            shader_byte_code_length, input_layout);

    if (FAILED(hr = d3d_input_layout_create(device, (const D3D11_INPUT_ELEMENT_DESC *)element_descs, element_count,
            shader_byte_code, shader_byte_code_length, &object)))
        return hr;

    *input_layout = &object->ID3D10InputLayout_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateVertexShader(ID3D10Device1 *iface,
        const void *byte_code, SIZE_T byte_code_length, ID3D10VertexShader **shader)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_vertex_shader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, shader %p.\n",
            iface, byte_code, byte_code_length, shader);

    if (FAILED(hr = d3d_vertex_shader_create(device, byte_code, byte_code_length, &object)))
        return hr;

    *shader = &object->ID3D10VertexShader_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateGeometryShader(ID3D10Device1 *iface,
        const void *byte_code, SIZE_T byte_code_length, ID3D10GeometryShader **shader)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_geometry_shader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, shader %p.\n",
            iface, byte_code, byte_code_length, shader);

    if (FAILED(hr = d3d_geometry_shader_create(device, byte_code, byte_code_length,
            NULL, 0, NULL, 0, 0, &object)))
        return hr;

    *shader = &object->ID3D10GeometryShader_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateGeometryShaderWithStreamOutput(ID3D10Device1 *iface,
        const void *byte_code, SIZE_T byte_code_length, const D3D10_SO_DECLARATION_ENTRY *output_stream_decls,
        UINT output_stream_decl_count, UINT output_stream_stride, ID3D10GeometryShader **shader)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    D3D11_SO_DECLARATION_ENTRY *so_entries = NULL;
    struct d3d_geometry_shader *object;
    unsigned int i, stride_count = 1;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, output_stream_decls %p, "
            "output_stream_decl_count %u, output_stream_stride %u, shader %p.\n",
            iface, byte_code, byte_code_length, output_stream_decls,
            output_stream_decl_count, output_stream_stride, shader);

    if (!output_stream_decl_count && output_stream_stride)
    {
        WARN("Stride must be 0 when declaration entry count is 0.\n");
        *shader = NULL;
        return E_INVALIDARG;
    }

    if (output_stream_decl_count
            && !(so_entries = d3d11_calloc(output_stream_decl_count, sizeof(*so_entries))))
    {
        ERR("Failed to allocate D3D11 SO declaration array memory.\n");
        *shader = NULL;
        return E_OUTOFMEMORY;
    }

    for (i = 0; i < output_stream_decl_count; ++i)
    {
        so_entries[i].Stream = 0;
        so_entries[i].SemanticName = output_stream_decls[i].SemanticName;
        so_entries[i].SemanticIndex = output_stream_decls[i].SemanticIndex;
        so_entries[i].StartComponent = output_stream_decls[i].StartComponent;
        so_entries[i].ComponentCount = output_stream_decls[i].ComponentCount;
        so_entries[i].OutputSlot = output_stream_decls[i].OutputSlot;

        if (output_stream_decls[i].OutputSlot)
        {
            stride_count = 0;
            if (output_stream_stride)
            {
                WARN("Stride must be 0 when multiple output slots are used.\n");
                HeapFree(GetProcessHeap(), 0, so_entries);
                *shader = NULL;
                return E_INVALIDARG;
            }
        }
    }

    hr = d3d_geometry_shader_create(device, byte_code, byte_code_length,
            so_entries, output_stream_decl_count, &output_stream_stride, stride_count, 0, &object);
    HeapFree(GetProcessHeap(), 0, so_entries);
    if (FAILED(hr))
    {
        *shader = NULL;
        return hr;
    }

    *shader = &object->ID3D10GeometryShader_iface;

    return hr;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreatePixelShader(ID3D10Device1 *iface,
        const void *byte_code, SIZE_T byte_code_length, ID3D10PixelShader **shader)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_pixel_shader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, byte_code_length %lu, shader %p.\n",
            iface, byte_code, byte_code_length, shader);

    if (FAILED(hr = d3d_pixel_shader_create(device, byte_code, byte_code_length, &object)))
        return hr;

    *shader = &object->ID3D10PixelShader_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateBlendState1(ID3D10Device1 *iface,
        const D3D10_BLEND_DESC1 *desc, ID3D10BlendState1 **blend_state)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_blend_state *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, blend_state %p.\n", iface, desc, blend_state);

    if (FAILED(hr = d3d_blend_state_create(device, (const D3D11_BLEND_DESC *)desc, &object)))
        return hr;

    *blend_state = &object->ID3D10BlendState1_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateBlendState(ID3D10Device1 *iface,
        const D3D10_BLEND_DESC *desc, ID3D10BlendState **blend_state)
{
    D3D10_BLEND_DESC1 d3d10_1_desc;
    unsigned int i;

    TRACE("iface %p, desc %p, blend_state %p.\n", iface, desc, blend_state);

    if (!desc)
        return E_INVALIDARG;

    d3d10_1_desc.AlphaToCoverageEnable = desc->AlphaToCoverageEnable;
    d3d10_1_desc.IndependentBlendEnable = FALSE;
    for (i = 0; i < D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT - 1; ++i)
    {
        if (desc->BlendEnable[i] != desc->BlendEnable[i + 1]
                || desc->RenderTargetWriteMask[i] != desc->RenderTargetWriteMask[i + 1])
            d3d10_1_desc.IndependentBlendEnable = TRUE;
    }

    for (i = 0; i < D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
    {
        d3d10_1_desc.RenderTarget[i].BlendEnable = desc->BlendEnable[i];
        d3d10_1_desc.RenderTarget[i].SrcBlend = desc->SrcBlend;
        d3d10_1_desc.RenderTarget[i].DestBlend = desc->DestBlend;
        d3d10_1_desc.RenderTarget[i].BlendOp = desc->BlendOp;
        d3d10_1_desc.RenderTarget[i].SrcBlendAlpha = desc->SrcBlendAlpha;
        d3d10_1_desc.RenderTarget[i].DestBlendAlpha = desc->DestBlendAlpha;
        d3d10_1_desc.RenderTarget[i].BlendOpAlpha = desc->BlendOpAlpha;
        d3d10_1_desc.RenderTarget[i].RenderTargetWriteMask = desc->RenderTargetWriteMask[i];
    }

    return d3d10_device_CreateBlendState1(iface, &d3d10_1_desc, (ID3D10BlendState1 **)blend_state);
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateDepthStencilState(ID3D10Device1 *iface,
        const D3D10_DEPTH_STENCIL_DESC *desc, ID3D10DepthStencilState **depth_stencil_state)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_depthstencil_state *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, depth_stencil_state %p.\n", iface, desc, depth_stencil_state);

    if (FAILED(hr = d3d_depthstencil_state_create(device, (const D3D11_DEPTH_STENCIL_DESC *)desc, &object)))
        return hr;

    *depth_stencil_state = &object->ID3D10DepthStencilState_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateRasterizerState(ID3D10Device1 *iface,
        const D3D10_RASTERIZER_DESC *desc, ID3D10RasterizerState **rasterizer_state)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_rasterizer_state *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, rasterizer_state %p.\n", iface, desc, rasterizer_state);

    if (FAILED(hr = d3d_rasterizer_state_create(device, (const D3D11_RASTERIZER_DESC *)desc, &object)))
        return hr;

    *rasterizer_state = &object->ID3D10RasterizerState_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateSamplerState(ID3D10Device1 *iface,
        const D3D10_SAMPLER_DESC *desc, ID3D10SamplerState **sampler_state)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_sampler_state *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, sampler_state %p.\n", iface, desc, sampler_state);

    if (FAILED(hr = d3d_sampler_state_create(device, (const D3D11_SAMPLER_DESC *)desc, &object)))
        return hr;

    *sampler_state = &object->ID3D10SamplerState_iface;

    return S_OK;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateQuery(ID3D10Device1 *iface,
        const D3D10_QUERY_DESC *desc, ID3D10Query **query)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_query *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, query %p.\n", iface, desc, query);

    if (FAILED(hr = d3d_query_create(device, (const D3D11_QUERY_DESC *)desc, FALSE, &object)))
        return hr;

    if (query)
    {
        *query = &object->ID3D10Query_iface;
        return S_OK;
    }

    ID3D10Query_Release(&object->ID3D10Query_iface);
    return S_FALSE;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreatePredicate(ID3D10Device1 *iface,
        const D3D10_QUERY_DESC *desc, ID3D10Predicate **predicate)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    struct d3d_query *object;
    HRESULT hr;

    TRACE("iface %p, desc %p, predicate %p.\n", iface, desc, predicate);

    if (FAILED(hr = d3d_query_create(device, (const D3D11_QUERY_DESC *)desc, TRUE, &object)))
        return hr;

    if (predicate)
    {
        *predicate = (ID3D10Predicate *)&object->ID3D10Query_iface;
        return S_OK;
    }

    ID3D10Query_Release(&object->ID3D10Query_iface);
    return S_FALSE;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CreateCounter(ID3D10Device1 *iface,
        const D3D10_COUNTER_DESC *desc, ID3D10Counter **counter)
{
    FIXME("iface %p, desc %p, counter %p stub!\n", iface, desc, counter);

    return E_NOTIMPL;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CheckFormatSupport(ID3D10Device1 *iface,
        DXGI_FORMAT format, UINT *format_support)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);
    HRESULT hr;

    TRACE("iface %p, format %s, format_support %p.\n",
            iface, debug_dxgi_format(format), format_support);

    hr = d3d11_device_CheckFormatSupport(&device->ID3D11Device_iface, format, format_support);
    if (SUCCEEDED(hr)) *format_support &= 0x8ffffff;
    return hr;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CheckMultisampleQualityLevels(ID3D10Device1 *iface,
        DXGI_FORMAT format, UINT sample_count, UINT *quality_level_count)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p, format %s, sample_count %u, quality_level_count %p.\n",
            iface, debug_dxgi_format(format), sample_count, quality_level_count);

    return d3d11_device_CheckMultisampleQualityLevels(&device->ID3D11Device_iface, format,
            sample_count, quality_level_count);
}

static void STDMETHODCALLTYPE d3d10_device_CheckCounterInfo(ID3D10Device1 *iface, D3D10_COUNTER_INFO *counter_info)
{
    FIXME("iface %p, counter_info %p stub!\n", iface, counter_info);
}

static HRESULT STDMETHODCALLTYPE d3d10_device_CheckCounter(ID3D10Device1 *iface,
        const D3D10_COUNTER_DESC *desc, D3D10_COUNTER_TYPE *type, UINT *active_counters, char *name,
        UINT *name_length, char *units, UINT *units_length, char *description, UINT *description_length)
{
    FIXME("iface %p, desc %p, type %p, active_counters %p, name %p, name_length %p, "
            "units %p, units_length %p, description %p, description_length %p stub!\n",
            iface, desc, type, active_counters, name, name_length,
            units, units_length, description, description_length);

    return E_NOTIMPL;
}

static UINT STDMETHODCALLTYPE d3d10_device_GetCreationFlags(ID3D10Device1 *iface)
{
    FIXME("iface %p stub!\n", iface);

    return 0;
}

static HRESULT STDMETHODCALLTYPE d3d10_device_OpenSharedResource(ID3D10Device1 *iface,
        HANDLE resource_handle, REFIID guid, void **resource)
{
    FIXME("iface %p, resource_handle %p, guid %s, resource %p stub!\n",
            iface, resource_handle, debugstr_guid(guid), resource);

    return E_NOTIMPL;
}

static void STDMETHODCALLTYPE d3d10_device_SetTextFilterSize(ID3D10Device1 *iface, UINT width, UINT height)
{
    FIXME("iface %p, width %u, height %u stub!\n", iface, width, height);
}

static void STDMETHODCALLTYPE d3d10_device_GetTextFilterSize(ID3D10Device1 *iface, UINT *width, UINT *height)
{
    FIXME("iface %p, width %p, height %p stub!\n", iface, width, height);
}

static D3D10_FEATURE_LEVEL1 STDMETHODCALLTYPE d3d10_device_GetFeatureLevel(ID3D10Device1 *iface)
{
    struct d3d_device *device = impl_from_ID3D10Device(iface);

    TRACE("iface %p.\n", iface);

    return device->feature_level;
}

static const struct ID3D10Device1Vtbl d3d10_device1_vtbl =
{
    /* IUnknown methods */
    d3d10_device_QueryInterface,
    d3d10_device_AddRef,
    d3d10_device_Release,
    /* ID3D10Device methods */
    d3d10_device_VSSetConstantBuffers,
    d3d10_device_PSSetShaderResources,
    d3d10_device_PSSetShader,
    d3d10_device_PSSetSamplers,
    d3d10_device_VSSetShader,
    d3d10_device_DrawIndexed,
    d3d10_device_Draw,
    d3d10_device_PSSetConstantBuffers,
    d3d10_device_IASetInputLayout,
    d3d10_device_IASetVertexBuffers,
    d3d10_device_IASetIndexBuffer,
    d3d10_device_DrawIndexedInstanced,
    d3d10_device_DrawInstanced,
    d3d10_device_GSSetConstantBuffers,
    d3d10_device_GSSetShader,
    d3d10_device_IASetPrimitiveTopology,
    d3d10_device_VSSetShaderResources,
    d3d10_device_VSSetSamplers,
    d3d10_device_SetPredication,
    d3d10_device_GSSetShaderResources,
    d3d10_device_GSSetSamplers,
    d3d10_device_OMSetRenderTargets,
    d3d10_device_OMSetBlendState,
    d3d10_device_OMSetDepthStencilState,
    d3d10_device_SOSetTargets,
    d3d10_device_DrawAuto,
    d3d10_device_RSSetState,
    d3d10_device_RSSetViewports,
    d3d10_device_RSSetScissorRects,
    d3d10_device_CopySubresourceRegion,
    d3d10_device_CopyResource,
    d3d10_device_UpdateSubresource,
    d3d10_device_ClearRenderTargetView,
    d3d10_device_ClearDepthStencilView,
    d3d10_device_GenerateMips,
    d3d10_device_ResolveSubresource,
    d3d10_device_VSGetConstantBuffers,
    d3d10_device_PSGetShaderResources,
    d3d10_device_PSGetShader,
    d3d10_device_PSGetSamplers,
    d3d10_device_VSGetShader,
    d3d10_device_PSGetConstantBuffers,
    d3d10_device_IAGetInputLayout,
    d3d10_device_IAGetVertexBuffers,
    d3d10_device_IAGetIndexBuffer,
    d3d10_device_GSGetConstantBuffers,
    d3d10_device_GSGetShader,
    d3d10_device_IAGetPrimitiveTopology,
    d3d10_device_VSGetShaderResources,
    d3d10_device_VSGetSamplers,
    d3d10_device_GetPredication,
    d3d10_device_GSGetShaderResources,
    d3d10_device_GSGetSamplers,
    d3d10_device_OMGetRenderTargets,
    d3d10_device_OMGetBlendState,
    d3d10_device_OMGetDepthStencilState,
    d3d10_device_SOGetTargets,
    d3d10_device_RSGetState,
    d3d10_device_RSGetViewports,
    d3d10_device_RSGetScissorRects,
    d3d10_device_GetDeviceRemovedReason,
    d3d10_device_SetExceptionMode,
    d3d10_device_GetExceptionMode,
    d3d10_device_GetPrivateData,
    d3d10_device_SetPrivateData,
    d3d10_device_SetPrivateDataInterface,
    d3d10_device_ClearState,
    d3d10_device_Flush,
    d3d10_device_CreateBuffer,
    d3d10_device_CreateTexture1D,
    d3d10_device_CreateTexture2D,
    d3d10_device_CreateTexture3D,
    d3d10_device_CreateShaderResourceView,
    d3d10_device_CreateRenderTargetView,
    d3d10_device_CreateDepthStencilView,
    d3d10_device_CreateInputLayout,
    d3d10_device_CreateVertexShader,
    d3d10_device_CreateGeometryShader,
    d3d10_device_CreateGeometryShaderWithStreamOutput,
    d3d10_device_CreatePixelShader,
    d3d10_device_CreateBlendState,
    d3d10_device_CreateDepthStencilState,
    d3d10_device_CreateRasterizerState,
    d3d10_device_CreateSamplerState,
    d3d10_device_CreateQuery,
    d3d10_device_CreatePredicate,
    d3d10_device_CreateCounter,
    d3d10_device_CheckFormatSupport,
    d3d10_device_CheckMultisampleQualityLevels,
    d3d10_device_CheckCounterInfo,
    d3d10_device_CheckCounter,
    d3d10_device_GetCreationFlags,
    d3d10_device_OpenSharedResource,
    d3d10_device_SetTextFilterSize,
    d3d10_device_GetTextFilterSize,
    d3d10_device_CreateShaderResourceView1,
    d3d10_device_CreateBlendState1,
    d3d10_device_GetFeatureLevel,
};

static const struct IUnknownVtbl d3d_device_inner_unknown_vtbl =
{
    /* IUnknown methods */
    d3d_device_inner_QueryInterface,
    d3d_device_inner_AddRef,
    d3d_device_inner_Release,
};

/* ID3D10Multithread methods */

static inline struct d3d_device *impl_from_ID3D10Multithread(ID3D10Multithread *iface)
{
    return CONTAINING_RECORD(iface, struct d3d_device, ID3D10Multithread_iface);
}

static HRESULT STDMETHODCALLTYPE d3d10_multithread_QueryInterface(ID3D10Multithread *iface, REFIID iid, void **out)
{
    struct d3d_device *device = impl_from_ID3D10Multithread(iface);

    TRACE("iface %p, iid %s, out %p.\n", iface, debugstr_guid(iid), out);

    return IUnknown_QueryInterface(device->outer_unk, iid, out);
}

static ULONG STDMETHODCALLTYPE d3d10_multithread_AddRef(ID3D10Multithread *iface)
{
    struct d3d_device *device = impl_from_ID3D10Multithread(iface);

    TRACE("iface %p.\n", iface);

    return IUnknown_AddRef(device->outer_unk);
}

static ULONG STDMETHODCALLTYPE d3d10_multithread_Release(ID3D10Multithread *iface)
{
    struct d3d_device *device = impl_from_ID3D10Multithread(iface);

    TRACE("iface %p.\n", iface);

    return IUnknown_Release(device->outer_unk);
}

static void STDMETHODCALLTYPE d3d10_multithread_Enter(ID3D10Multithread *iface)
{
    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
}

static void STDMETHODCALLTYPE d3d10_multithread_Leave(ID3D10Multithread *iface)
{
    TRACE("iface %p.\n", iface);

    wined3d_mutex_unlock();
}

static BOOL STDMETHODCALLTYPE d3d10_multithread_SetMultithreadProtected(ID3D10Multithread *iface, BOOL protect)
{
    FIXME("iface %p, protect %#x stub!\n", iface, protect);

    return TRUE;
}

static BOOL STDMETHODCALLTYPE d3d10_multithread_GetMultithreadProtected(ID3D10Multithread *iface)
{
    FIXME("iface %p stub!\n", iface);

    return TRUE;
}

static const struct ID3D10MultithreadVtbl d3d10_multithread_vtbl =
{
    d3d10_multithread_QueryInterface,
    d3d10_multithread_AddRef,
    d3d10_multithread_Release,
    d3d10_multithread_Enter,
    d3d10_multithread_Leave,
    d3d10_multithread_SetMultithreadProtected,
    d3d10_multithread_GetMultithreadProtected,
};

/* IWineDXGIDeviceParent IUnknown methods */

static inline struct d3d_device *device_from_dxgi_device_parent(IWineDXGIDeviceParent *iface)
{
    return CONTAINING_RECORD(iface, struct d3d_device, IWineDXGIDeviceParent_iface);
}

static HRESULT STDMETHODCALLTYPE dxgi_device_parent_QueryInterface(IWineDXGIDeviceParent *iface,
        REFIID riid, void **ppv)
{
    struct d3d_device *device = device_from_dxgi_device_parent(iface);
    return IUnknown_QueryInterface(device->outer_unk, riid, ppv);
}

static ULONG STDMETHODCALLTYPE dxgi_device_parent_AddRef(IWineDXGIDeviceParent *iface)
{
    struct d3d_device *device = device_from_dxgi_device_parent(iface);
    return IUnknown_AddRef(device->outer_unk);
}

static ULONG STDMETHODCALLTYPE dxgi_device_parent_Release(IWineDXGIDeviceParent *iface)
{
    struct d3d_device *device = device_from_dxgi_device_parent(iface);
    return IUnknown_Release(device->outer_unk);
}

static struct wined3d_device_parent * STDMETHODCALLTYPE dxgi_device_parent_get_wined3d_device_parent(
        IWineDXGIDeviceParent *iface)
{
    struct d3d_device *device = device_from_dxgi_device_parent(iface);
    return &device->device_parent;
}

static const struct IWineDXGIDeviceParentVtbl d3d_dxgi_device_parent_vtbl =
{
    /* IUnknown methods */
    dxgi_device_parent_QueryInterface,
    dxgi_device_parent_AddRef,
    dxgi_device_parent_Release,
    /* IWineDXGIDeviceParent methods */
    dxgi_device_parent_get_wined3d_device_parent,
};

static inline struct d3d_device *device_from_wined3d_device_parent(struct wined3d_device_parent *device_parent)
{
    return CONTAINING_RECORD(device_parent, struct d3d_device, device_parent);
}

static void CDECL device_parent_wined3d_device_created(struct wined3d_device_parent *device_parent,
        struct wined3d_device *wined3d_device)
{
    struct d3d_device *device = device_from_wined3d_device_parent(device_parent);

    TRACE("device_parent %p, wined3d_device %p.\n", device_parent, wined3d_device);

    wined3d_device_incref(wined3d_device);
    device->wined3d_device = wined3d_device;
}

static void CDECL device_parent_mode_changed(struct wined3d_device_parent *device_parent)
{
    TRACE("device_parent %p.\n", device_parent);
}

static void CDECL device_parent_activate(struct wined3d_device_parent *device_parent, BOOL activate)
{
    TRACE("device_parent %p, activate %#x.\n", device_parent, activate);
}

static HRESULT CDECL device_parent_sub_resource_created(struct wined3d_device_parent *device_parent,
        struct wined3d_texture *wined3d_texture, unsigned int sub_resource_idx, void **parent,
        const struct wined3d_parent_ops **parent_ops)
{
    TRACE("device_parent %p, wined3d_texture %p, sub_resource_idx %u, parent %p, parent_ops %p.\n",
            device_parent, wined3d_texture, sub_resource_idx, parent, parent_ops);

    *parent = NULL;
    *parent_ops = &d3d_null_wined3d_parent_ops;

    return S_OK;
}

static HRESULT CDECL device_parent_create_swapchain_texture(struct wined3d_device_parent *device_parent,
        void *container_parent, const struct wined3d_resource_desc *wined3d_desc, DWORD texture_flags,
        struct wined3d_texture **wined3d_texture)
{
    struct d3d_device *device = device_from_wined3d_device_parent(device_parent);
    struct d3d_texture2d *texture;
    ID3D10Texture2D *texture_iface;
    D3D10_TEXTURE2D_DESC desc;
    HRESULT hr;

    FIXME("device_parent %p, container_parent %p, wined3d_desc %p, texture flags %#x, "
            "wined3d_texture %p partial stub!\n", device_parent, container_parent,
            wined3d_desc, texture_flags, wined3d_texture);

    FIXME("Implement DXGI<->wined3d usage conversion.\n");

    desc.Width = wined3d_desc->width;
    desc.Height = wined3d_desc->height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = dxgi_format_from_wined3dformat(wined3d_desc->format);
    desc.SampleDesc.Count = wined3d_desc->multisample_type ? wined3d_desc->multisample_type : 1;
    desc.SampleDesc.Quality = wined3d_desc->multisample_quality;
    desc.Usage = D3D10_USAGE_DEFAULT;
    desc.BindFlags = D3D10_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    if (texture_flags & WINED3D_TEXTURE_CREATE_GET_DC)
    {
        desc.MiscFlags |= D3D10_RESOURCE_MISC_GDI_COMPATIBLE;
        texture_flags &= ~WINED3D_TEXTURE_CREATE_GET_DC;
    }

    if (texture_flags)
        FIXME("Unhandled flags %#x.\n", texture_flags);

    if (FAILED(hr = d3d10_device_CreateTexture2D(&device->ID3D10Device1_iface,
            &desc, NULL, &texture_iface)))
    {
        WARN("CreateTexture2D failed, returning %#x.\n", hr);
        return hr;
    }

    texture = impl_from_ID3D10Texture2D(texture_iface);

    *wined3d_texture = texture->wined3d_texture;
    wined3d_texture_incref(*wined3d_texture);
    ID3D10Texture2D_Release(&texture->ID3D10Texture2D_iface);

    return S_OK;
}

static HRESULT CDECL device_parent_create_swapchain(struct wined3d_device_parent *device_parent,
        struct wined3d_swapchain_desc *desc, struct wined3d_swapchain **swapchain)
{
    struct d3d_device *device = device_from_wined3d_device_parent(device_parent);
    IWineDXGIDevice *wine_device;
    HRESULT hr;

    TRACE("device_parent %p, desc %p, swapchain %p.\n", device_parent, desc, swapchain);

    if (FAILED(hr = d3d10_device_QueryInterface(&device->ID3D10Device1_iface,
            &IID_IWineDXGIDevice, (void **)&wine_device)))
    {
        ERR("Device should implement IWineDXGIDevice.\n");
        return E_FAIL;
    }

    hr = IWineDXGIDevice_create_swapchain(wine_device, desc, TRUE, swapchain);
    IWineDXGIDevice_Release(wine_device);
    if (FAILED(hr))
    {
        ERR("Failed to create DXGI swapchain, returning %#x\n", hr);
        return hr;
    }

    return S_OK;
}

static const struct wined3d_device_parent_ops d3d_wined3d_device_parent_ops =
{
    device_parent_wined3d_device_created,
    device_parent_mode_changed,
    device_parent_activate,
    device_parent_sub_resource_created,
    device_parent_sub_resource_created,
    device_parent_create_swapchain_texture,
    device_parent_create_swapchain,
};

static int d3d_sampler_state_compare(const void *key, const struct wine_rb_entry *entry)
{
    const D3D11_SAMPLER_DESC *ka = key;
    const D3D11_SAMPLER_DESC *kb = &WINE_RB_ENTRY_VALUE(entry, const struct d3d_sampler_state, entry)->desc;

    return memcmp(ka, kb, sizeof(*ka));
}

static int d3d_blend_state_compare(const void *key, const struct wine_rb_entry *entry)
{
    const D3D11_BLEND_DESC *ka = key;
    const D3D11_BLEND_DESC *kb = &WINE_RB_ENTRY_VALUE(entry, const struct d3d_blend_state, entry)->desc;

    return memcmp(ka, kb, sizeof(*ka));
}

static int d3d_depthstencil_state_compare(const void *key, const struct wine_rb_entry *entry)
{
    const D3D11_DEPTH_STENCIL_DESC *ka = key;
    const D3D11_DEPTH_STENCIL_DESC *kb = &WINE_RB_ENTRY_VALUE(entry,
            const struct d3d_depthstencil_state, entry)->desc;

    return memcmp(ka, kb, sizeof(*ka));
}

static int d3d_rasterizer_state_compare(const void *key, const struct wine_rb_entry *entry)
{
    const D3D11_RASTERIZER_DESC *ka = key;
    const D3D11_RASTERIZER_DESC *kb = &WINE_RB_ENTRY_VALUE(entry, const struct d3d_rasterizer_state, entry)->desc;

    return memcmp(ka, kb, sizeof(*ka));
}

void d3d_device_init(struct d3d_device *device, void *outer_unknown)
{
    device->IUnknown_inner.lpVtbl = &d3d_device_inner_unknown_vtbl;
    device->ID3D11Device_iface.lpVtbl = &d3d11_device_vtbl;
    device->ID3D10Device1_iface.lpVtbl = &d3d10_device1_vtbl;
    device->ID3D10Multithread_iface.lpVtbl = &d3d10_multithread_vtbl;
    device->IWineDXGIDeviceParent_iface.lpVtbl = &d3d_dxgi_device_parent_vtbl;
    device->device_parent.ops = &d3d_wined3d_device_parent_ops;
    device->refcount = 1;
    /* COM aggregation always takes place */
    device->outer_unk = outer_unknown;

    d3d11_immediate_context_init(&device->immediate_context, device);
    ID3D11DeviceContext_Release(&device->immediate_context.ID3D11DeviceContext_iface);

    device->blend_factor[0] = 1.0f;
    device->blend_factor[1] = 1.0f;
    device->blend_factor[2] = 1.0f;
    device->blend_factor[3] = 1.0f;

    wine_rb_init(&device->blend_states, d3d_blend_state_compare);
    wine_rb_init(&device->depthstencil_states, d3d_depthstencil_state_compare);
    wine_rb_init(&device->rasterizer_states, d3d_rasterizer_state_compare);
    wine_rb_init(&device->sampler_states, d3d_sampler_state_compare);
}
