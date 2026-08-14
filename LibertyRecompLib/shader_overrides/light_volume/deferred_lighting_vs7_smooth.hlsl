// Exact XenosRecomp source for deferred_lighting_vs7.bin
// (XXH3 0xDF64C22EC010C136), with only the FusionShaders smooth-volume
// conservative bound enabled below.
#ifndef SHADER_COMMON_H_INCLUDED
#define SHADER_COMMON_H_INCLUDED

#define SPEC_CONSTANT_R11G11B10_NORMAL  (1 << 0)
#define SPEC_CONSTANT_ALPHA_TEST        (1 << 1)

#ifdef UNLEASHED_RECOMP
    #define SPEC_CONSTANT_BICUBIC_GI_FILTER (1 << 2)
    #define SPEC_CONSTANT_ALPHA_TO_COVERAGE (1 << 3)
    #define SPEC_CONSTANT_REVERSE_Z         (1 << 4)
#endif

#ifdef MARATHON_RECOMP
    #define SPEC_CONSTANT_CONDITIONAL_SURVEY    (1 << 5)
    #define SPEC_CONSTANT_CONDITIONAL_RENDERING (1 << 6)
#endif

#if defined(__air__) || !defined(__cplusplus) || defined(__INTELLISENSE__)

#ifndef __air__
#define FLT_MIN asfloat(0xff7fffff)
#define FLT_MAX asfloat(0x7f7fffff)
#endif

#ifdef __spirv__

struct PushConstants
{
    uint64_t VertexShaderConstants;
    uint64_t PixelShaderConstants;
    uint64_t SharedConstants;
};

[[vk::push_constant]] ConstantBuffer<PushConstants> g_PushConstants;

#ifdef GTA4_RECOMP
#define g_Booleans                  vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 528)
#define g_SwappedTexcoords          vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 532)
#define g_SwappedNormals            vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 536)
#define g_SwappedBinormals          vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 540)
#define g_SwappedTangents           vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 544)
#define g_SwappedBlendWeights       vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 548)
#define g_HalfPixelOffset           vk::RawBufferLoad<float2>(g_PushConstants.SharedConstants + 552)
#define g_ClipPlane                 vk::RawBufferLoad<float4>(g_PushConstants.SharedConstants + 560)
#define g_ClipPlaneEnabled          vk::RawBufferLoad<bool>(g_PushConstants.SharedConstants + 576)
#define g_AlphaThreshold            vk::RawBufferLoad<float>(g_PushConstants.SharedConstants + 580)
#define g_conditionalSurveyIndex    vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 584)
#define g_conditionalRenderingIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 588)
#else
#define g_Booleans                  vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 320)
#define g_SwappedTexcoords          vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 324)
#define g_SwappedNormals            vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 328)
#define g_SwappedBinormals          vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 332)
#define g_SwappedTangents           vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 336)
#define g_SwappedBlendWeights       vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 340)
#define g_HalfPixelOffset           vk::RawBufferLoad<float2>(g_PushConstants.SharedConstants + 344)
#define g_ClipPlane                 vk::RawBufferLoad<float4>(g_PushConstants.SharedConstants + 352)
#define g_ClipPlaneEnabled          vk::RawBufferLoad<bool>(g_PushConstants.SharedConstants + 368)
#define g_AlphaThreshold            vk::RawBufferLoad<float>(g_PushConstants.SharedConstants + 372)
#define g_conditionalSurveyIndex    vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 376)
#define g_conditionalRenderingIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 380)
#endif

[[vk::constant_id(0)]] const uint g_SpecConstants = 0;

#define g_SpecConstants() g_SpecConstants

#elif defined(__air__)

#include <metal_stdlib>

using namespace metal;

constant uint G_SPEC_CONSTANTS [[function_constant(0)]];
constant uint G_SPEC_CONSTANTS_VAL = is_function_constant_defined(G_SPEC_CONSTANTS) ? G_SPEC_CONSTANTS : 0;

uint g_SpecConstants()
{
    return G_SPEC_CONSTANTS_VAL;
}

struct PushConstants
{
    ulong VertexShaderConstants;
    ulong PixelShaderConstants;
    ulong SharedConstants;
};

#ifdef GTA4_RECOMP
#define g_Booleans (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 528)))
#define g_SwappedTexcoords (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 532)))
#define g_SwappedNormals (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 536)))
#define g_SwappedBinormals (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 540)))
#define g_SwappedTangents (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 544)))
#define g_SwappedBlendWeights (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 548)))
#define g_HalfPixelOffset (*(reinterpret_cast<device float2*>(g_PushConstants.SharedConstants + 552)))
#define g_ClipPlane (*(reinterpret_cast<device float4*>(g_PushConstants.SharedConstants + 560)))
#define g_ClipPlaneEnabled (*(reinterpret_cast<device bool*>(g_PushConstants.SharedConstants + 576)))
#define g_AlphaThreshold (*(reinterpret_cast<device float*>(g_PushConstants.SharedConstants + 580)))
#define g_conditionalSurveyIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 584)))
#define g_conditionalRenderingIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 588)))
#else
#define g_Booleans (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 320)))
#define g_SwappedTexcoords (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 324)))
#define g_SwappedNormals (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 328)))
#define g_SwappedBinormals (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 332)))
#define g_SwappedTangents (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 336)))
#define g_SwappedBlendWeights (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 340)))
#define g_HalfPixelOffset (*(reinterpret_cast<device float2*>(g_PushConstants.SharedConstants + 344)))
#define g_ClipPlane (*(reinterpret_cast<device float4*>(g_PushConstants.SharedConstants + 352)))
#define g_ClipPlaneEnabled (*(reinterpret_cast<device bool*>(g_PushConstants.SharedConstants + 368)))
#define g_AlphaThreshold (*(reinterpret_cast<device float*>(g_PushConstants.SharedConstants + 372)))
#define g_conditionalSurveyIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 376)))
#define g_conditionalRenderingIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 380)))
#endif

#else

#ifdef GTA4_RECOMP
#define DEFINE_SHARED_CONSTANTS() \
    uint g_Booleans : packoffset(c33.x); \
    uint g_SwappedTexcoords : packoffset(c33.y); \
    uint g_SwappedNormals : packoffset(c33.z); \
    uint g_SwappedBinormals : packoffset(c33.w); \
    uint g_SwappedTangents : packoffset(c34.x);  \
    uint g_SwappedBlendWeights : packoffset(c34.y); \
    float2 g_HalfPixelOffset : packoffset(c34.z); \
    float4 g_ClipPlane : packoffset(c35.x); \
    bool g_ClipPlaneEnabled : packoffset(c36.x); \
    float g_AlphaThreshold : packoffset(c36.y); \
    uint g_conditionalSurveyIndex : packoffset(c36.z); \
    uint g_conditionalRenderingIndex : packoffset(c36.w);
#else
#define DEFINE_SHARED_CONSTANTS() \
    uint g_Booleans : packoffset(c20.x); \
    uint g_SwappedTexcoords : packoffset(c20.y); \
    uint g_SwappedNormals : packoffset(c20.z); \
    uint g_SwappedBinormals : packoffset(c20.w); \
    uint g_SwappedTangents : packoffset(c21.x);  \
    uint g_SwappedBlendWeights : packoffset(c21.y); \
    float2 g_HalfPixelOffset : packoffset(c21.z); \
    float4 g_ClipPlane : packoffset(c22.x); \
    bool g_ClipPlaneEnabled : packoffset(c23.x); \
    float g_AlphaThreshold : packoffset(c23.y); \
    uint g_conditionalSurveyIndex : packoffset(c23.z); \
    uint g_conditionalRenderingIndex : packoffset(c23.w);
#endif

uint g_SpecConstants();

#endif

float4 cube(float4 value)
{
    float3 src = value.zwx;
    float3 abs_src = abs(src);

    float sc, tc, ma, id;

    if (abs_src.z >= abs_src.x && abs_src.z >= abs_src.y)
    {
        // Z major axis
        tc = -src.y;
        sc = src.z < 0.0 ? -src.x : src.x;
        ma = 2.0 * src.z;
        id = src.z < 0.0 ? 5.0 : 4.0;
    }
    else if (abs_src.y >= abs_src.x)
    {
        // Y major axis
        tc = src.y < 0.0 ? -src.z : src.z;
        sc = src.x;
        ma = 2.0 * src.y;
        id = src.y < 0.0 ? 3.0 : 2.0;
    }
    else
    {
        // X major axis
        tc = -src.y;
        sc = src.x < 0.0 ? src.z : -src.z;
        ma = 2.0 * src.x;
        id = src.x < 0.0 ? 1.0 : 0.0;
    }

    // Return as per Xbox 360 cube instruction output format:
    // x = t coordinate
    // y = s coordinate
    // z = 2 * major axis
    // w = face ID
    return float4(tc, sc, ma, id);
}

float3 cubeDir(float3 texCoord)
{
    // Move from 1...2 to -1...1
    float sc = (texCoord.x * 2.0) - 3.0;
    float tc = (texCoord.y * 2.0) - 3.0;

    uint face = uint(clamp(texCoord.z, 0.0, 5.0));

    // Split face into axis and sign
    uint axis = face >> 1;
    uint neg = face & 1;

    float3 dir;

    switch(axis)
    {
    case 0: // X major axis
        dir.y = -tc;
        dir.z = neg ? sc : -sc;
        dir.x = neg ? -1.0 : 1.0;
        break;

    case 1: // Y major axis
        dir.x = sc;
        dir.z = neg ? -tc : tc;
        dir.y = neg ? -1.0 : 1.0;
        break;

    default: // Z major axis
        dir.x = neg ? -sc : sc;
        dir.y = -tc;
        dir.z = neg ? -1.0 : 1.0;
        break;
    }

    return dir;
}

#ifdef __air__

struct Texture2DDescriptorHeap
{
    texture2d<float> tex;
};

struct Texture2DArrayDescriptorHeap
{
    texture2d_array<float> tex;
};

struct Texture3DDescriptorHeap
{
    texture3d<float> tex;
};

struct TextureCubeDescriptorHeap
{
    texturecube<float> tex;
};

struct SamplerDescriptorHeap
{
    sampler samp;
};

struct AtomicUintBuffer
{
    device atomic_uint* buffer;
};

uint2 getTexture2DDimensions(texture2d<float> texture)
{
    return uint2(texture.get_width(), texture.get_height());
}

uint3 getTexture2DArrayDimensions(texture2d_array<float> texture)
{
    return uint3(texture.get_width(), texture.get_height(), texture.get_array_size());
}

uint3 getTexture3DDimensions(texture3d<float> texture)
{
    return uint3(texture.get_width(), texture.get_height(), texture.get_depth());
}

float4 tfetch2D(constant Texture2DDescriptorHeap* textureHeap,
                constant SamplerDescriptorHeap* samplerHeap,
                uint resourceDescriptorIndex,
                uint samplerDescriptorIndex,
                float2 texCoord, float2 offset)
{
    texture2d<float> texture = textureHeap[resourceDescriptorIndex].tex;
    sampler sampler = samplerHeap[samplerDescriptorIndex].samp;
    return texture.sample(sampler, texCoord + offset / (float2)getTexture2DDimensions(texture));
}

float4 tfetch2DArray(constant Texture2DArrayDescriptorHeap* textureHeap,
                     constant SamplerDescriptorHeap* samplerHeap,
                     uint resourceDescriptorIndex,
                     uint samplerDescriptorIndex,
                     float3 texCoord, float3 offset)
{
    texture2d_array<float> texture = textureHeap[resourceDescriptorIndex].tex;
    sampler sampler = samplerHeap[samplerDescriptorIndex].samp;
    uint3 dimensions = getTexture2DArrayDimensions(texture);
    return texture.sample(sampler, texCoord.xy + offset.xy / float2(dimensions.xy), uint(texCoord.z * dimensions.z));
}

float4 tfetch3D(constant Texture3DDescriptorHeap* textureHeap,
                constant SamplerDescriptorHeap* samplerHeap,
                uint resourceDescriptorIndex,
                uint samplerDescriptorIndex,
                float3 texCoord, float3 offset)
{
    texture3d<float> texture = textureHeap[resourceDescriptorIndex].tex;
    sampler sampler = samplerHeap[samplerDescriptorIndex].samp;
    return texture.sample(sampler, texCoord + offset / float3(getTexture3DDimensions(texture)));
}

float4 tfetchCube(constant TextureCubeDescriptorHeap* textureHeap,
                  constant SamplerDescriptorHeap* samplerHeap,
                  uint resourceDescriptorIndex,
                  uint samplerDescriptorIndex,
                  float3 texCoord)
{
    texturecube<float> texture = textureHeap[resourceDescriptorIndex].tex;
    sampler sampler = samplerHeap[samplerDescriptorIndex].samp;
    float3 dir = cubeDir(texCoord);
    return texture.sample(sampler, dir);
}

float2 getWeights2D(constant Texture2DDescriptorHeap* textureHeap,
                    constant SamplerDescriptorHeap* samplerHeap,
                    uint resourceDescriptorIndex,
                    uint samplerDescriptorIndex,
                    float2 texCoord, float2 offset)
{
    texture2d<float> texture = textureHeap[resourceDescriptorIndex].tex;
    return select(fract(texCoord * float2(getTexture2DDimensions(texture)) + offset - 0.5), 0.0, isnan(texCoord));
}

float3 getWeights2DArray(constant Texture2DArrayDescriptorHeap* textureHeap,
                         constant SamplerDescriptorHeap* samplerHeap,
                         uint resourceDescriptorIndex,
                         uint samplerDescriptorIndex,
                         float3 texCoord, float3 offset)
{
    texture2d_array<float> texture = textureHeap[resourceDescriptorIndex].tex;
    return select(fract(texCoord * float3(getTexture2DArrayDimensions(texture)) + offset - 0.5), 0.0, isnan(texCoord));
}

float3 getWeights3D(constant Texture3DDescriptorHeap* textureHeap,
                    constant SamplerDescriptorHeap* samplerHeap,
                    uint resourceDescriptorIndex,
                    uint samplerDescriptorIndex,
                    float3 texCoord, float3 offset)
{
    texture3d<float> texture = textureHeap[resourceDescriptorIndex].tex;
    return select(fract(texCoord * float3(getTexture3DDimensions(texture)) + offset - 0.5),
                  0.0, isnan(texCoord));
}

#else

Texture2D<float4> g_Texture2DDescriptorHeap[] : register(t0, space0);
Texture2DArray<float4> g_Texture2DArrayDescriptorHeap[] : register(t0, space1);
Texture3D<float4> g_Texture3DDescriptorHeap[] : register(t0, space2);
TextureCube<float4> g_TextureCubeDescriptorHeap[] : register(t0, space3);
SamplerState g_SamplerDescriptorHeap[] : register(s0, space4);

#ifdef MARATHON_RECOMP
RWStructuredBuffer<uint> g_ConditionalSurveyBuffer : register(u0, space5);
#endif

uint2 getTexture2DDimensions(Texture2D<float4> texture)
{
    uint2 dimensions;
    texture.GetDimensions(dimensions.x, dimensions.y);
    return dimensions;
}

uint3 getTexture2DArrayDimensions(Texture2DArray<float4> texture)
{
    uint4 dimensions;
    texture.GetDimensions(0, dimensions.x, dimensions.y, dimensions.z, dimensions.w);
    return dimensions.xyz;
}

uint3 getTexture3DDimensions(Texture3D<float4> texture)
{
    uint3 dimensions;
    texture.GetDimensions(dimensions.x, dimensions.y, dimensions.z);
    return dimensions;
}

// Pixel shaders need implicit derivatives for authored mip selection,
// trilinear filtering and anisotropic footprints. Vertex shaders still use
// explicit level zero because implicit derivatives are not available there.
float4 tfetch2D(uint resourceDescriptorIndex, uint samplerDescriptorIndex, float2 texCoord, float2 offset)
{
    Texture2D<float4> texture = g_Texture2DDescriptorHeap[resourceDescriptorIndex];
#ifdef XENOS_RECOMP_PIXEL_SHADER
    return texture.Sample(g_SamplerDescriptorHeap[samplerDescriptorIndex], texCoord + offset / getTexture2DDimensions(texture));
#else
    return texture.SampleLevel(g_SamplerDescriptorHeap[samplerDescriptorIndex], texCoord + offset / getTexture2DDimensions(texture), 0);
#endif
}

float4 tfetch2DArray(uint resourceDescriptorIndex, uint samplerDescriptorIndex, float3 texCoord, float3 offset)
{
    Texture2DArray<float4> texture = g_Texture2DArrayDescriptorHeap[resourceDescriptorIndex];
    uint3 dimensions = getTexture2DArrayDimensions(texture);
#ifdef XENOS_RECOMP_PIXEL_SHADER
    return texture.Sample(g_SamplerDescriptorHeap[samplerDescriptorIndex], float3(texCoord.xy + offset.xy / dimensions.xy, texCoord.z * dimensions.z));
#else
    return texture.SampleLevel(g_SamplerDescriptorHeap[samplerDescriptorIndex], float3(texCoord.xy + offset.xy / dimensions.xy, texCoord.z * dimensions.z), 0);
#endif
}

float4 tfetch3D(uint resourceDescriptorIndex, uint samplerDescriptorIndex, float3 texCoord, float3 offset)
{
    Texture3D<float4> texture = g_Texture3DDescriptorHeap[resourceDescriptorIndex];
    uint3 dimensions = getTexture3DDimensions(texture);
#ifdef XENOS_RECOMP_PIXEL_SHADER
    return texture.Sample(g_SamplerDescriptorHeap[samplerDescriptorIndex],
                          texCoord + offset / dimensions);
#else
    return texture.SampleLevel(g_SamplerDescriptorHeap[samplerDescriptorIndex],
                               texCoord + offset / dimensions, 0);
#endif
}

float4 tfetchCube(uint resourceDescriptorIndex, uint samplerDescriptorIndex, float3 texCoord)
{
    float3 dir = cubeDir(texCoord);
#ifdef XENOS_RECOMP_PIXEL_SHADER
    return g_TextureCubeDescriptorHeap[resourceDescriptorIndex].Sample(
        g_SamplerDescriptorHeap[samplerDescriptorIndex], dir);
#else
    return g_TextureCubeDescriptorHeap[resourceDescriptorIndex].SampleLevel(
        g_SamplerDescriptorHeap[samplerDescriptorIndex], dir, 0);
#endif
}

float2 getWeights2D(uint resourceDescriptorIndex, uint samplerDescriptorIndex, float2 texCoord, float2 offset)
{
    Texture2D<float4> texture = g_Texture2DDescriptorHeap[resourceDescriptorIndex];
    return select(isnan(texCoord), 0.0, frac(texCoord * getTexture2DDimensions(texture) + offset - 0.5));
}

float3 getWeights2DArray(uint resourceDescriptorIndex, uint samplerDescriptorIndex, float3 texCoord, float3 offset)
{
    Texture2DArray<float4> texture = g_Texture2DArrayDescriptorHeap[resourceDescriptorIndex];
    return select(isnan(texCoord), 0.0, frac(texCoord * getTexture2DArrayDimensions(texture) + offset - 0.5));
}

float3 getWeights3D(uint resourceDescriptorIndex, uint samplerDescriptorIndex, float3 texCoord, float3 offset)
{
    Texture3D<float4> texture = g_Texture3DDescriptorHeap[resourceDescriptorIndex];
    return select(isnan(texCoord), 0.0,
                  frac(texCoord * getTexture3DDimensions(texture) + offset - 0.5));
}

#endif

#ifdef __air__
#define selectWrapper(a, b, c) select(c, b, a)
#else
#define selectWrapper(a, b, c) select(a, b, c)
#endif

#ifdef __air__
#define frac(X) fract(X)

template<typename T>
void clip(T a)
{
    if (a < 0.0) {
        discard_fragment();
    }
}

template<typename T>
float rcp(T a)
{
    return 1.0 / a;
}

template<typename T>
float4x4 mul(T a, T b)
{
    return b * a;
}
#endif

#ifdef __air__
#define UNROLL
#define BRANCH
#else
#define UNROLL [unroll]
#define BRANCH [branch]
#endif

float w0(float a)
{
    return (1.0f / 6.0f) * (a * (a * (-a + 3.0f) - 3.0f) + 1.0f);
}

float w1(float a)
{
    return (1.0f / 6.0f) * (a * a * (3.0f * a - 6.0f) + 4.0f);
}

float w2(float a)
{
    return (1.0f / 6.0f) * (a * (a * (-3.0f * a + 3.0f) + 3.0f) + 1.0f);
}

float w3(float a)
{
    return (1.0f / 6.0f) * (a * a * a);
}

float g0(float a)
{
    return w0(a) + w1(a);
}

float g1(float a)
{
    return w2(a) + w3(a);
}

float h0(float a)
{
    return -1.0f + w1(a) / (w0(a) + w1(a)) + 0.5f;
}

float h1(float a)
{
    return 1.0f + w3(a) / (w2(a) + w3(a)) + 0.5f;
}

#ifdef __air__

float4 tfetch2DBicubic(constant Texture2DDescriptorHeap* textureHeap,
                       constant SamplerDescriptorHeap* samplerHeap,
                       uint resourceDescriptorIndex,
                       uint samplerDescriptorIndex,
                       float2 texCoord, float2 offset)
{
    texture2d<float> texture = textureHeap[resourceDescriptorIndex].tex;
    sampler sampler = samplerHeap[samplerDescriptorIndex].samp;
    uint2 dimensions = getTexture2DDimensions(texture);

    float x = texCoord.x * dimensions.x + offset.x;
    float y = texCoord.y * dimensions.y + offset.y;

    x -= 0.5f;
    y -= 0.5f;
    float px = floor(x);
    float py = floor(y);
    float fx = x - px;
    float fy = y - py;

    float g0x = g0(fx);
    float g1x = g1(fx);
    float h0x = h0(fx);
    float h1x = h1(fx);
    float h0y = h0(fy);
    float h1y = h1(fy);

    float4 r =
        g0(fy) * (g0x * texture.sample(sampler, float2(px + h0x, py + h0y) / float2(dimensions)) +
              g1x * texture.sample(sampler, float2(px + h1x, py + h0y) / float2(dimensions))) +
        g1(fy) * (g0x * texture.sample(sampler, float2(px + h0x, py + h1y) / float2(dimensions)) +
              g1x * texture.sample(sampler, float2(px + h1x, py + h1y) / float2(dimensions)));

    return r;
}

#else

float4 tfetch2DBicubic(uint resourceDescriptorIndex, uint samplerDescriptorIndex, float2 texCoord, float2 offset)
{
    Texture2D<float4> texture = g_Texture2DDescriptorHeap[resourceDescriptorIndex];
    SamplerState samplerState = g_SamplerDescriptorHeap[samplerDescriptorIndex];
    uint2 dimensions = getTexture2DDimensions(texture);
    
    float x = texCoord.x * dimensions.x + offset.x;
    float y = texCoord.y * dimensions.y + offset.y;

    x -= 0.5f;
    y -= 0.5f;
    float px = floor(x);
    float py = floor(y);
    float fx = x - px;
    float fy = y - py;

    float g0x = g0(fx);
    float g1x = g1(fx);
    float h0x = h0(fx);
    float h1x = h1(fx);
    float h0y = h0(fy);
    float h1y = h1(fy);

    float4 r =
        g0(fy) * (g0x * texture.Sample(samplerState, float2(px + h0x, py + h0y) / float2(dimensions)) +
            g1x * texture.Sample(samplerState, float2(px + h1x, py + h0y) / float2(dimensions))) +
        g1(fy) * (g0x * texture.Sample(samplerState, float2(px + h0x, py + h1y) / float2(dimensions)) +
            g1x * texture.Sample(samplerState, float2(px + h1x, py + h1y) / float2(dimensions)));

    return r;
}

#endif

float4 tfetchR11G11B10(uint4 value)
{
    if (g_SpecConstants() & SPEC_CONSTANT_R11G11B10_NORMAL)
    {
        return float4(
            (value.x & 0x00000400 ? -1.0 : 0.0) + ((value.x & 0x3FF) / 1024.0),
            (value.x & 0x00200000 ? -1.0 : 0.0) + (((value.x >> 11) & 0x3FF) / 1024.0),
            (value.x & 0x80000000 ? -1.0 : 0.0) + (((value.x >> 22) & 0x1FF) / 512.0),
            0.0);
    }
    else
    {
#ifdef __air__
        return as_type<float4>(value);
#else
        return asfloat(value);
#endif
    }
}

float4 swapFloats(uint swappedFloats, float4 value, uint semanticIndex)
{
    return (swappedFloats & (1ull << semanticIndex)) != 0 ? value.yxwz : value;
}

float4 dst(float4 src0, float4 src1)
{
    float4 dest;
    dest.x = 1.0;
    dest.y = src0.y * src1.y;
    dest.z = src0.z;
    dest.w = src1.w;
    return dest;
}

float4 max4(float4 src0)
{
    return max(max(src0.x, src0.y), max(src0.z, src0.w));
}

#ifdef __air__

float2 getPixelCoord(constant Texture2DDescriptorHeap* textureHeap,
                     uint resourceDescriptorIndex,
                     float2 texCoord)
{
    texture2d<float> texture = textureHeap[resourceDescriptorIndex].tex;
    return (float2)getTexture2DDimensions(texture) * texCoord;
}

#else

float2 getPixelCoord(uint resourceDescriptorIndex, float2 texCoord)
{
    return getTexture2DDimensions(g_Texture2DDescriptorHeap[resourceDescriptorIndex]) * texCoord;
}

#endif

float computeMipLevel(float2 pixelCoord)
{
#ifdef __air__
    float2 dx = dfdx(pixelCoord);
    float2 dy = dfdy(pixelCoord);
#else
    float2 dx = ddx(pixelCoord);
    float2 dy = ddy(pixelCoord);
#endif
    float deltaMaxSqr = max(dot(dx, dx), dot(dy, dy));
    return max(0.0, 0.5 * log2(deltaMaxSqr));
}

#ifdef __air__

uint atomicLoadUint(device AtomicUintBuffer* buffer, uint index)
{
    return atomic_load_explicit(&buffer->buffer[index], memory_order_relaxed);
}

uint atomicFetchAddUint(device AtomicUintBuffer* buffer, uint index, uint value)
{
    return atomic_fetch_add_explicit(&buffer->buffer[index], value, memory_order_relaxed);
}

#else

uint atomicLoadUint(RWStructuredBuffer<uint> buffer, uint index)
{
    return buffer[index];
}

uint atomicFetchAddUint(RWStructuredBuffer<uint> buffer, uint index, uint value)
{
    uint originalValue;
    InterlockedAdd(buffer[index], value, originalValue);
    return originalValue;
}

#endif

#endif

#endif

#ifdef __spirv__

#define gDeferredLightColourAndIntensity vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + 3456, 0x10)
#define gDeferredLightConeAngle vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + 3440, 0x10)
#define gDeferredLightConeAngleI vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + 3424, 0x10)
#define gDeferredLightDirection vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + 3360, 0x10)
#define gDeferredLightPosition vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + 3344, 0x10)
#define gDeferredLightRadius vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + 3392, 0x10)
#define gDeferredLightTangent vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + 3376, 0x10)
#define gDeferredLightType vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + 3328, 0x10)
#define gDeferredLightVolumeParams vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + 3472, 0x10)
#define gDeferredVolumeRadiusScale vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + 3408, 0x10)
#define gViewInverse(INDEX) selectWrapper((INDEX) < 244, vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + (12 + min(INDEX, 243)) * 16, 0x10), 0.0)
#define gWorldViewProj(INDEX) selectWrapper((INDEX) < 248, vk::RawBufferLoad<float4>(g_PushConstants.VertexShaderConstants + (8 + min(INDEX, 247)) * 16, 0x10), 0.0)

#elif defined(__air__)

#define gDeferredLightColourAndIntensity (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + 3456)))
#define gDeferredLightConeAngle (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + 3440)))
#define gDeferredLightConeAngleI (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + 3424)))
#define gDeferredLightDirection (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + 3360)))
#define gDeferredLightPosition (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + 3344)))
#define gDeferredLightRadius (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + 3392)))
#define gDeferredLightTangent (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + 3376)))
#define gDeferredLightType (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + 3328)))
#define gDeferredLightVolumeParams (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + 3472)))
#define gDeferredVolumeRadiusScale (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + 3408)))
#define gViewInverse(INDEX) selectWrapper((INDEX) < 244, (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + (12 + min((uint)(INDEX), (uint)243)) * 16))), 0.0)
#define gWorldViewProj(INDEX) selectWrapper((INDEX) < 248, (*(reinterpret_cast<device float4*>(g_PushConstants.VertexShaderConstants + (8 + min((uint)(INDEX), (uint)247)) * 16))), 0.0)

#else

cbuffer VertexShaderConstants : register(b0, space4)
{
	float4 gDeferredLightColourAndIntensity : packoffset(c216);
	float4 gDeferredLightConeAngle : packoffset(c215);
	float4 gDeferredLightConeAngleI : packoffset(c214);
	float4 gDeferredLightDirection : packoffset(c210);
	float4 gDeferredLightPosition : packoffset(c209);
	float4 gDeferredLightRadius : packoffset(c212);
	float4 gDeferredLightTangent : packoffset(c211);
	float4 gDeferredLightType : packoffset(c208);
	float4 gDeferredLightVolumeParams : packoffset(c217);
	float4 gDeferredVolumeRadiusScale : packoffset(c213);
	float4 gViewInverse[4] : packoffset(c12);
#define gViewInverse(INDEX) selectWrapper((INDEX) < 244, gViewInverse[min(INDEX, 243)], 0.0)
	float4 gWorldViewProj[4] : packoffset(c8);
#define gWorldViewProj(INDEX) selectWrapper((INDEX) < 248, gWorldViewProj[min(INDEX, 247)], 0.0)
};

cbuffer SharedConstants : register(b2, space4)
{
	DEFINE_SHARED_CONSTANTS();
};

#endif

struct VertexShaderInput
{
#ifdef __air__
	float4 iPosition0 [[attribute(0)]];
#else
	[[vk::location(0)]] float4 iPosition0 : POSITION0;
#endif
};
struct Interpolators
{
#ifdef __air__
	float4 oPos [[position]] [[invariant]];
	float4 oTexCoord0 [[user(TEXCOORD0)]];
	float4 oTexCoord1 [[user(TEXCOORD1)]];
	float4 oTexCoord2 [[user(TEXCOORD2)]];
	float4 oTexCoord3 [[user(TEXCOORD3)]];
	float4 oTexCoord4 [[user(TEXCOORD4)]];
	float4 oTexCoord5 [[user(TEXCOORD5)]];
	float4 oTexCoord6 [[user(TEXCOORD6)]];
	float4 oTexCoord7 [[user(TEXCOORD7)]];
	float4 oTexCoord8 [[user(TEXCOORD8)]];
	float4 oTexCoord9 [[user(TEXCOORD9)]];
	float4 oTexCoord10 [[user(TEXCOORD10)]];
	float4 oTexCoord11 [[user(TEXCOORD11)]];
	float4 oTexCoord12 [[user(TEXCOORD12)]];
	float4 oTexCoord13 [[user(TEXCOORD13)]];
	float4 oTexCoord14 [[user(TEXCOORD14)]];
	float4 oTexCoord15 [[user(TEXCOORD15)]];
	float4 oColor0 [[user(COLOR0)]];
	float4 oColor1 [[user(COLOR1)]];
	float clipDistance [[clip_distance]];
#else
	precise float4 oPos : SV_Position;
	float4 oTexCoord0 : TEXCOORD0;
	float4 oTexCoord1 : TEXCOORD1;
	float4 oTexCoord2 : TEXCOORD2;
	float4 oTexCoord3 : TEXCOORD3;
	float4 oTexCoord4 : TEXCOORD4;
	float4 oTexCoord5 : TEXCOORD5;
	float4 oTexCoord6 : TEXCOORD6;
	float4 oTexCoord7 : TEXCOORD7;
	float4 oTexCoord8 : TEXCOORD8;
	float4 oTexCoord9 : TEXCOORD9;
	float4 oTexCoord10 : TEXCOORD10;
	float4 oTexCoord11 : TEXCOORD11;
	float4 oTexCoord12 : TEXCOORD12;
	float4 oTexCoord13 : TEXCOORD13;
	float4 oTexCoord14 : TEXCOORD14;
	float4 oTexCoord15 : TEXCOORD15;
	float4 oColor0 : COLOR0;
	float4 oColor1 : COLOR1;
	float clipDistance : SV_ClipDistance;
#endif
};
#ifdef __air__
[[vertex]]
#else
#if !defined(__spirv__)
[shader("vertex")]
#endif
#endif
Interpolators shaderMain(
#ifdef __air__
	constant Texture2DDescriptorHeap* g_Texture2DDescriptorHeap [[buffer(0)]],
	constant Texture2DArrayDescriptorHeap* g_Texture2DArrayDescriptorHeap [[buffer(1)]],
	constant Texture3DDescriptorHeap* g_Texture3DDescriptorHeap [[buffer(2)]],
	constant TextureCubeDescriptorHeap* g_TextureCubeDescriptorHeap [[buffer(3)]],
	constant SamplerDescriptorHeap* g_SamplerDescriptorHeap [[buffer(4)]],
	constant PushConstants& g_PushConstants [[buffer(8)]],
	VertexShaderInput input [[stage_in]]
#else
	VertexShaderInput input
#endif
)
{
#ifdef __air__
	Interpolators output = Interpolators{};
#else
	Interpolators output = (Interpolators)0;
#endif
#ifdef __air__
	float4 c252 = as_type<float4>(uint4(0x0, 0x0, 0x0, 0x0));
#else
	float4 c252 = asfloat(uint4(0x0, 0x0, 0x0, 0x0));
#endif
#ifdef __air__
	float4 c253 = as_type<float4>(uint4(0x3F000000, 0x3FC90FDB, 0xC0490FDB, 0x40000000));
#else
	float4 c253 = asfloat(uint4(0x3F000000, 0x3FC90FDB, 0xC0490FDB, 0x40000000));
#endif
#ifdef __air__
	float4 c254 = as_type<float4>(uint4(0x0, 0x3F800000, 0x38D1B717, 0x40C90FDB));
#else
	float4 c254 = asfloat(uint4(0x0, 0x3F800000, 0x38D1B717, 0x40C90FDB));
#endif
#ifdef __air__
	float4 c255 = as_type<float4>(uint4(0x3E800000, 0x3F2978D5, 0x40800000, 0x3E22F983));
#else
	// FusionShaders smooth mode expands the conservative light-volume bound
	// from the authored 0.662 falloff radius to 0.85 of the light radius. The
	// pixel shader below reaches zero at that same boundary.
	float4 c255 = asfloat(uint4(0x3E800000, 0x3F59999A, 0x40800000, 0x3E22F983));
#endif

	output.oTexCoord0 = 0.0;
	output.oTexCoord1 = 0.0;
	output.oTexCoord2 = 0.0;
	output.oTexCoord3 = 0.0;
	output.oTexCoord4 = 0.0;
	output.oTexCoord5 = 0.0;
	output.oTexCoord6 = 0.0;
	output.oTexCoord7 = 0.0;
	output.oTexCoord8 = 0.0;
	output.oTexCoord9 = 0.0;
	output.oTexCoord10 = 0.0;
	output.oTexCoord11 = 0.0;
	output.oTexCoord12 = 0.0;
	output.oTexCoord13 = 0.0;
	output.oTexCoord14 = 0.0;
	output.oTexCoord15 = 0.0;
	output.oColor0 = 0.0;
	output.oColor1 = 0.0;

	float4 r0 = 0.0;
	float4 r1 = 0.0;
	float4 r2 = 0.0;
	float4 r3 = 0.0;
	float4 r4 = 0.0;
	float4 r5 = 0.0;
	float4 r6 = 0.0;
	float4 r7 = 0.0;
	float4 r8 = 0.0;
	float4 r9 = 0.0;
	float4 r10 = 0.0;
	float4 r11 = 0.0;
	float4 r12 = 0.0;
	float4 r13 = 0.0;
	float4 r14 = 0.0;
	float4 r15 = 0.0;
	float4 r16 = 0.0;
	float4 r17 = 0.0;
	float4 r18 = 0.0;
	float4 r19 = 0.0;
	float4 r20 = 0.0;
	float4 r21 = 0.0;
	float4 r22 = 0.0;
	float4 r23 = 0.0;
	float4 r24 = 0.0;
	float4 r25 = 0.0;
	float4 r26 = 0.0;
	float4 r27 = 0.0;
	float4 r28 = 0.0;
	float4 r29 = 0.0;
	float4 r30 = 0.0;
	float4 r31 = 0.0;
	int a0 = 0;
	int aL = 0;
	bool p0 = false;
	float ps = 0.0;

	uint pc = 0;
	while (true)
	{
		switch (pc)
		{
		case 0:
			r1.xyz = (float3)((input.iPosition0)).xyz;
			r0.w = (float)((gDeferredLightType.x == c255.z));
			r0.xyz = (float3)((gDeferredLightTangent.yxz * gDeferredLightDirection.xzy));
			p0 = r0.w == 0.0;
			ps = p0 ? 0.0 : 1.0;
			r2.xyz = (float3)((gDeferredLightTangent.yzx * gDeferredLightDirection.zxy + -r0.zyx));
			r0.z = (float)((dot(r1.xy, r1.xy) + c254.x));
		case 1:
			if (!p0)
			{
				pc = 10;
				continue;
			}
		case 2:
			r0.x = (float)((gDeferredLightType.x == c253.w));
			p0 = r0.x != 0.0;
			ps = p0 ? 0.0 : 1.0;
		case 3:
			if (!p0)
			{
				pc = 8;
				continue;
			}
		case 4:
			r1.w = (float)((r0.z >= c254.z));
			ps = saturate((float)(max(-r1.z, -r1.z)));
			r0.x = ps;
			ps = c254.y - r0.x;
			r0.y = ps;
			ps = gDeferredLightConeAngle.x * r0.y;
			r0.x = ps;
			r0.x = (float)((r0.x * c255.w + c253.x));
			r0.x = (float)((frac(r0.x)));
			ps = clamp(rcp(r0.z), -FLT_MAX, FLT_MAX);
			r0.y = ps;
			r0.z = (float)((r0.x * c254.w + c253.z));
		case 5:
			r0.x = (float)((abs(r1.z) >= c254.z));
			ps = cos(r0.z);
			r1.z = ps;
			r0.z = (float)((-r1.z * r1.z + c254.y));
			r0.y = (float)((r0.z * r0.y));
			r0.xzw = (float3)((r0.xxx * gDeferredLightDirection.xyz));
			ps = sqrt(abs(r0.y));
			r0.y = ps;
			r4.xyz = (float3)((r1.zzz * gDeferredLightDirection.xyz));
			ps = gDeferredVolumeRadiusScale.x * r0.y;
			r0.y = ps;
			r3.xy = (float2)((r0.yy * r1.xy));
		case 6:
			r1.xyz = (float3)((r3.yyy * r2.zyx + r4.zyx));
			r1.xyz = (float3)((r3.xxx * gDeferredLightTangent.xyz + r1.zyx));
			r0.xyz = (float3)((selectWrapper(r1.www == 0.0, r0.xzw, r1.xyz)));
		case 7:
			pc = 9;
			continue;
		case 8:
			r0.x = (float)((r1.z * r1.z + r0.z));
			ps = clamp(rsqrt(abs(r0.x)), -FLT_MAX, FLT_MAX);
			r0.x = ps;
			r0.xyz = (float3)((r0.xxx * r1.xyz));
		case 9:
			pc = 15;
			continue;
		case 10:
			r0.w = (float)((gDeferredLightConeAngleI.x * c255.w));
			ps = max(gDeferredLightConeAngle.x, gDeferredLightConeAngle.x);
			r0.x = (float)((r1.z * c253.y + c253.y));
			r0.x = (float)((r0.x * c255.w));
			ps = c255.w * ps;
			r0.y = ps;
			r0.xyw = (float3)((r0.xyw + c253.xxx));
			r0.xyw = (float3)((frac(r0.xyw)));
			r3.xyz = (float3)((r0.xwy * c254.www + c253.zzz));
		case 11:
			ps = cos(r3.z);
			r0.w = ps;
			ps = cos(r3.x);
			r0.x = ps;
			r1.z = (float)((r0.w >= r0.x));
			ps = clamp(rcp(r0.z), -FLT_MAX, FLT_MAX);
			r0.y = ps;
			r0.z = (float)((-r0.x * r0.x + c254.y));
			r0.y = (float)((r0.z * r0.y));
			ps = cos(r3.y);
			r0.z = ps;
			p0 = c254.x == 0.0 && r1.z != 0.0;
			r1.z = (float)((p0 ? 0.0 : c254.x + 1.0));
			ps = sqrt(abs(r0.y));
			r0.y = ps;
		case 12:
			if (p0)
			{
				r3.xyz = (float3)((r0.www * gDeferredLightDirection.xyz));
				ps = gDeferredVolumeRadiusScale.x * r0.y;
				r0.x = ps;
			}
			if (p0)
			{
				r0.xw = (float2)((r0.xx * r1.yx));
			}
			if (p0)
			{
				r0.xyz = (float3)((r0.xxx * r2.zyx + r3.zyx));
			}
			if (p0)
			{
				r0.xyz = (float3)((r0.www * gDeferredLightTangent.xyz + r0.zyx));
			}
			p0 = r1.z == 1.0;
			ps = p0 ? 0.0 : r1.z == 0.0 ? 1.0 : r1.z;
			r1.z = ps;
			if (p0)
			{
				r0.w = (float)((r0.x >= r0.z));
			}
		case 13:
			p0 = r1.z == 0.0 && r0.w != 0.0;
			r1.z = (float)((p0 ? 0.0 : r1.z + 1.0));
			if (p0)
			{
				r3.xyz = (float3)((r0.zzz * gDeferredLightDirection.xyz));
				ps = gDeferredVolumeRadiusScale.x * r0.y;
				r0.x = ps;
			}
			if (p0)
			{
				r0.xw = (float2)((r0.xx * r1.yx));
			}
			if (p0)
			{
				r0.xyz = (float3)((r0.xxx * r2.zyx + r3.zyx));
			}
			if (p0)
			{
				r0.xyz = (float3)((r0.www * gDeferredLightTangent.xyz + r0.zyx));
			}
			p0 = r1.z == 1.0;
			ps = p0 ? 0.0 : r1.z == 0.0 ? 1.0 : r1.z;
			r1.z = ps;
		case 14:
			if (p0)
			{
				r3.xyz = (float3)((r0.xxx * gDeferredLightDirection.xyz));
			}
			if (p0)
			{
				r0.xw = (float2)((r0.yy * r1.yx));
			}
			if (p0)
			{
				r0.xyz = (float3)((r0.xxx * r2.zyx + r3.zyx));
			}
			if (p0)
			{
				r0.xyz = (float3)((r0.www * gDeferredLightTangent.xyz + r0.zyx));
			}
		case 15:
		case 16:
			r1.x = (float)((gDeferredLightColourAndIntensity.w * gDeferredLightVolumeParams.x));
			r1.y = (float)((gDeferredLightRadius.x * gDeferredVolumeRadiusScale.x));
			r1.xy = (float2)((r1.xy * c255.xy));
			r2.xyz = (float3)((r1.yyy * r0.xyz + gDeferredLightPosition.xyz));
			r0.xyzw = (float4)((r2.zzzz * gWorldViewProj(2).xyzw + gWorldViewProj(3).xyzw));
			r0.xyzw = (float4)((r2.yyyy * gWorldViewProj(1).xyzw + r0.xyzw));
		case 17:
			r0.xyzw = (float4)((r2.xxxx * gWorldViewProj(0).xyzw + r0.xyzw));
			output.oPos.xyzw = (float4)((max(r0.xyzw, r0.xyzw)));
		case 18:
		case 19:
			output.oTexCoord1.xyz = (float3)((r2.xyz + -gViewInverse(3).xyz));
			output.oTexCoord2.xyz = (float3)((r1.xxx * gDeferredLightColourAndIntensity.xyz));
			output.oTexCoord0.xyz = (float3)((gViewInverse(3).xyz + -gDeferredLightPosition.xyz));
			ps = max(r0.w, r0.w);
			output.oTexCoord0.w = ps;
	if (g_ClipPlaneEnabled) output.clipDistance = dot(output.oPos, g_ClipPlane);
	output.oPos.xy += g_HalfPixelOffset * output.oPos.w;
			break;
			break;
		}
		break;
	}
	return output;
}
