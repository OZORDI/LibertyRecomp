// This file is part of the FidelityFX SDK.
//
// Copyright (C) 2025 Advanced Micro Devices, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef SHARED_H
#define SHARED_H

#ifndef MAX_TEXTURES_COUNT
#define MAX_TEXTURES_COUNT 1000
#endif  // !MAX_TEXTURES_COUNT

#ifndef MAX_SAMPLERS_COUNT
#define MAX_SAMPLERS_COUNT 20
#endif  // !MAX_SAMPLERS_COUNT

#define MAX_SHADOW_MAP_TEXTURES_COUNT 15

#define RAYTRACING_INFO_BEGIN_SLOT 20
#define RAYTRACING_INFO_MATERIAL   20
#define RAYTRACING_INFO_INSTANCE   21
#define RAYTRACING_INFO_SURFACE_ID 22
#define RAYTRACING_INFO_SURFACE    23

#define TEXTURE_BEGIN_SLOT    50
#define SHADOW_MAP_BEGIN_SLOT 25
#define SAMPLER_BEGIN_SLOT    10

#define INDEX_BUFFER_BEGIN_SLOT  1050
#define VERTEX_BUFFER_BEGIN_SLOT 21050

#define MAX_BUFFER_COUNT 20000

struct PTMaterialInfo
{
    float albedo_factor_x;
    float albedo_factor_y;
    float albedo_factor_z;
    float albedo_factor_w;

    // A.R.M. Packed texture - Ambient occlusion | Roughness | Metalness
    float arm_factor_x;
    float arm_factor_y;
    float arm_factor_z;
    int arm_tex_id;
    int arm_tex_sampler_id;

    float emission_factor_x;
    float emission_factor_y;
    float emission_factor_z;
    int emission_tex_id;
    int emission_tex_sampler_id;

    int normal_tex_id;
    int normal_tex_sampler_id;
    int albedo_tex_id;
    int albedo_tex_sampler_id;
    float alpha_cutoff;
    int is_opaque;
    int is_double_sided;
};

struct PTInstanceInfo
{
    int surface_id_table_offset;
    int num_opaque_surfaces;
    int node_id;
    int num_surfaces;
};

#define SURFACE_INFO_INDEX_TYPE_U32 0
#define SURFACE_INFO_INDEX_TYPE_U16 1
struct PTSurfaceInfo
{
    int material_id;
    int index_offset;  // Offset for the first index
    int index_type;    // SURFACE_INFO_INDEX_TYPE
    int position_attribute_offset;

    int texcoord0_attribute_offset;
    int texcoord1_attribute_offset;
    int normal_attribute_offset;
    int tangent_attribute_offset;

    int num_indices;
    int num_vertices;
    int weight_attribute_offset;
    int joints_attribute_offset;
};

struct TraceRaysConstants
{
#if __cplusplus
    float clip_to_world[16];
    float camera_to_world[16];

    float inv_render_size[2];
    uint32_t frame_index;
    float ibl_factor;

    uint32_t fuse_mode;
    uint32_t use_dominant_light;
    uint32_t dominant_light_index;
    uint32_t pad;
#else // __cplusplus
    float4x4 clip_to_world;
    float4x4 camera_to_world;

    float2 inv_render_size;
    uint frame_index;
    float ibl_factor;

    uint fuse_mode;
    uint use_dominant_light;
    uint32_t dominant_light_index;
    uint pad;
#endif // __cplusplus
};

#endif  // SHARED_H
