// Exact XenosRecomp translation of rage_postfx_ps15.bin. The sole behavioral
// change scales the stock directional reprojection vector by a host-supplied
// frame-time correction; Xbox stencil rejection, taps, weights, and clamps are
// otherwise preserved verbatim.
#ifndef SHADER_COMMON_H_INCLUDED
#define SHADER_COMMON_H_INCLUDED

#define SPEC_CONSTANT_R11G11B10_NORMAL  (1 << 0)
#define SPEC_CONSTANT_ALPHA_TEST        (1 << 1)
#define SPEC_CONSTANT_ALPHA_TEST_FUNCTION_SHIFT 8
#define SPEC_CONSTANT_ALPHA_TEST_FUNCTION_MASK  7

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

bool AlphaTestPass(float alpha, float reference, uint function)
{
    if (function == 0u) return false;
    if (function == 1u) return alpha < reference;
    if (function == 2u) return alpha == reference;
    if (function == 3u) return alpha <= reference;
    if (function == 4u) return alpha > reference;
    if (function == 5u) return isnan(alpha) || isnan(reference) || alpha != reference;
    if (function == 6u) return alpha >= reference;
    if (function == 7u) return true;
    return false;
}

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

#define AdapLumSampler_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 16)
#define AdapLumSampler_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 120)
#define AdapLumSampler_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 224)
#define AdapLumSampler_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 328)
#define AdapLumSampler_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 432)
#define BloomSampler_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 12)
#define BloomSampler_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 116)
#define BloomSampler_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 220)
#define BloomSampler_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 324)
#define BloomSampler_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 428)
#define ColorCorrect vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 3504, 0x10)
#define ColorShift vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 3520, 0x10)
#define Exposure vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 3328, 0x10)
#define GBufferTextureSampler2_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 0)
#define GBufferTextureSampler2_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 104)
#define GBufferTextureSampler2_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 208)
#define GBufferTextureSampler2_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 312)
#define GBufferTextureSampler2_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 416)
#define GBufferTextureSampler3_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 4)
#define GBufferTextureSampler3_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 108)
#define GBufferTextureSampler3_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 212)
#define GBufferTextureSampler3_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 316)
#define GBufferTextureSampler3_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 420)
#define HDRSampler_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 8)
#define HDRSampler_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 112)
#define HDRSampler_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 216)
#define HDRSampler_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 320)
#define HDRSampler_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 424)
#define JitterSampler_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 20)
#define JitterSampler_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 124)
#define JitterSampler_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 228)
#define JitterSampler_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 332)
#define JitterSampler_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 436)
#define PLAYER_MASK vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 3536, 0x10)
#define StencilCopySampler_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 24)
#define StencilCopySampler_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 128)
#define StencilCopySampler_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 232)
#define StencilCopySampler_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 336)
#define StencilCopySampler_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 440)
#define ToneMapParams vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 3472, 0x10)
#define deSatContrastGamma vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 3488, 0x10)
#define dofBlur vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 3376, 0x10)
#define dofDist vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 3360, 0x10)
#define dofProj vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 3344, 0x10)
#define gDirectionalMotionBlurLength vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 3392, 0x10)
#define LibertyMotionBlurTimeScale vk::RawBufferLoad<float>(g_PushConstants.SharedConstants + 592)
#define globalScreenSize vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 704, 0x10)
#define motionBlurMatrix(INDEX) selectWrapper((INDEX) < 11, vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + (213 + min(INDEX, 10)) * 16, 0x10), 0.0)

#elif defined(__air__)

#define AdapLumSampler_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 16)))
#define AdapLumSampler_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 120)))
#define AdapLumSampler_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 224)))
#define AdapLumSampler_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 328)))
#define AdapLumSampler_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 432)))
#define BloomSampler_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 12)))
#define BloomSampler_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 116)))
#define BloomSampler_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 220)))
#define BloomSampler_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 324)))
#define BloomSampler_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 428)))
#define ColorCorrect (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 3504)))
#define ColorShift (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 3520)))
#define Exposure (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 3328)))
#define GBufferTextureSampler2_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 0)))
#define GBufferTextureSampler2_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 104)))
#define GBufferTextureSampler2_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 208)))
#define GBufferTextureSampler2_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 312)))
#define GBufferTextureSampler2_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 416)))
#define GBufferTextureSampler3_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 4)))
#define GBufferTextureSampler3_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 108)))
#define GBufferTextureSampler3_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 212)))
#define GBufferTextureSampler3_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 316)))
#define GBufferTextureSampler3_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 420)))
#define HDRSampler_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 8)))
#define HDRSampler_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 112)))
#define HDRSampler_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 216)))
#define HDRSampler_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 320)))
#define HDRSampler_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 424)))
#define JitterSampler_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 20)))
#define JitterSampler_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 124)))
#define JitterSampler_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 228)))
#define JitterSampler_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 332)))
#define JitterSampler_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 436)))
#define PLAYER_MASK (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 3536)))
#define StencilCopySampler_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 24)))
#define StencilCopySampler_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 128)))
#define StencilCopySampler_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 232)))
#define StencilCopySampler_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 336)))
#define StencilCopySampler_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 440)))
#define ToneMapParams (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 3472)))
#define deSatContrastGamma (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 3488)))
#define dofBlur (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 3376)))
#define dofDist (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 3360)))
#define dofProj (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 3344)))
#define gDirectionalMotionBlurLength (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 3392)))
#define LibertyMotionBlurTimeScale (*(reinterpret_cast<device float*>(g_PushConstants.SharedConstants + 592)))
#define globalScreenSize (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 704)))
#define motionBlurMatrix(INDEX) selectWrapper((INDEX) < 11, (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + (213 + min((uint)(INDEX), (uint)10)) * 16))), 0.0)

#else

cbuffer PixelShaderConstants : register(b1, space4)
{
	float4 ColorCorrect : packoffset(c219);
	float4 ColorShift : packoffset(c220);
	float4 Exposure : packoffset(c208);
	float4 PLAYER_MASK : packoffset(c221);
	float4 ToneMapParams : packoffset(c217);
	float4 deSatContrastGamma : packoffset(c218);
	float4 dofBlur : packoffset(c211);
	float4 dofDist : packoffset(c210);
	float4 dofProj : packoffset(c209);
	float4 gDirectionalMotionBlurLength : packoffset(c212);
	float4 globalScreenSize : packoffset(c44);
	float4 motionBlurMatrix[4] : packoffset(c213);
#define motionBlurMatrix(INDEX) selectWrapper((INDEX) < 11, motionBlurMatrix[min(INDEX, 10)], 0.0)
};

cbuffer SharedConstants : register(b2, space4)
{
	uint AdapLumSampler_Texture2DDescriptorIndex : packoffset(c1.x);
	uint AdapLumSampler_Texture2DArrayDescriptorIndex : packoffset(c7.z);
	uint AdapLumSampler_Texture3DDescriptorIndex : packoffset(c14.x);
	uint AdapLumSampler_TextureCubeDescriptorIndex : packoffset(c20.z);
	uint AdapLumSampler_SamplerDescriptorIndex : packoffset(c27.x);
	uint BloomSampler_Texture2DDescriptorIndex : packoffset(c0.w);
	uint BloomSampler_Texture2DArrayDescriptorIndex : packoffset(c7.y);
	uint BloomSampler_Texture3DDescriptorIndex : packoffset(c13.w);
	uint BloomSampler_TextureCubeDescriptorIndex : packoffset(c20.y);
	uint BloomSampler_SamplerDescriptorIndex : packoffset(c26.w);
	uint GBufferTextureSampler2_Texture2DDescriptorIndex : packoffset(c0.x);
	uint GBufferTextureSampler2_Texture2DArrayDescriptorIndex : packoffset(c6.z);
	uint GBufferTextureSampler2_Texture3DDescriptorIndex : packoffset(c13.x);
	uint GBufferTextureSampler2_TextureCubeDescriptorIndex : packoffset(c19.z);
	uint GBufferTextureSampler2_SamplerDescriptorIndex : packoffset(c26.x);
	uint GBufferTextureSampler3_Texture2DDescriptorIndex : packoffset(c0.y);
	uint GBufferTextureSampler3_Texture2DArrayDescriptorIndex : packoffset(c6.w);
	uint GBufferTextureSampler3_Texture3DDescriptorIndex : packoffset(c13.y);
	uint GBufferTextureSampler3_TextureCubeDescriptorIndex : packoffset(c19.w);
	uint GBufferTextureSampler3_SamplerDescriptorIndex : packoffset(c26.y);
	uint HDRSampler_Texture2DDescriptorIndex : packoffset(c0.z);
	uint HDRSampler_Texture2DArrayDescriptorIndex : packoffset(c7.x);
	uint HDRSampler_Texture3DDescriptorIndex : packoffset(c13.z);
	uint HDRSampler_TextureCubeDescriptorIndex : packoffset(c20.x);
	uint HDRSampler_SamplerDescriptorIndex : packoffset(c26.z);
	uint JitterSampler_Texture2DDescriptorIndex : packoffset(c1.y);
	uint JitterSampler_Texture2DArrayDescriptorIndex : packoffset(c7.w);
	uint JitterSampler_Texture3DDescriptorIndex : packoffset(c14.y);
	uint JitterSampler_TextureCubeDescriptorIndex : packoffset(c20.w);
	uint JitterSampler_SamplerDescriptorIndex : packoffset(c27.y);
	uint StencilCopySampler_Texture2DDescriptorIndex : packoffset(c1.z);
	uint StencilCopySampler_Texture2DArrayDescriptorIndex : packoffset(c8.x);
	uint StencilCopySampler_Texture3DDescriptorIndex : packoffset(c14.z);
	uint StencilCopySampler_TextureCubeDescriptorIndex : packoffset(c21.x);
	uint StencilCopySampler_SamplerDescriptorIndex : packoffset(c27.z);
	DEFINE_SHARED_CONSTANTS();
	float LibertyMotionBlurTimeScale : packoffset(c37.x);
};

#endif

struct Interpolators
{
#ifdef __air__
	float4 iPos [[position]];
	float4 iTexCoord0 [[user(TEXCOORD0)]];
	float4 iTexCoord1 [[user(TEXCOORD1)]];
	float4 iTexCoord2 [[user(TEXCOORD2)]];
	float4 iTexCoord3 [[user(TEXCOORD3)]];
	float4 iTexCoord4 [[user(TEXCOORD4)]];
	float4 iTexCoord5 [[user(TEXCOORD5)]];
	float4 iTexCoord6 [[user(TEXCOORD6)]];
	float4 iTexCoord7 [[user(TEXCOORD7)]];
	float4 iTexCoord8 [[user(TEXCOORD8)]];
	float4 iTexCoord9 [[user(TEXCOORD9)]];
	float4 iTexCoord10 [[user(TEXCOORD10)]];
	float4 iTexCoord11 [[user(TEXCOORD11)]];
	float4 iTexCoord12 [[user(TEXCOORD12)]];
	float4 iTexCoord13 [[user(TEXCOORD13)]];
	float4 iTexCoord14 [[user(TEXCOORD14)]];
	float4 iTexCoord15 [[user(TEXCOORD15)]];
	float4 iColor0 [[user(COLOR0)]];
	float4 iColor1 [[user(COLOR1)]];
#else
	float4 iPos : SV_Position;
	float4 iTexCoord0 : TEXCOORD0;
	float4 iTexCoord1 : TEXCOORD1;
	float4 iTexCoord2 : TEXCOORD2;
	float4 iTexCoord3 : TEXCOORD3;
	float4 iTexCoord4 : TEXCOORD4;
	float4 iTexCoord5 : TEXCOORD5;
	float4 iTexCoord6 : TEXCOORD6;
	float4 iTexCoord7 : TEXCOORD7;
	float4 iTexCoord8 : TEXCOORD8;
	float4 iTexCoord9 : TEXCOORD9;
	float4 iTexCoord10 : TEXCOORD10;
	float4 iTexCoord11 : TEXCOORD11;
	float4 iTexCoord12 : TEXCOORD12;
	float4 iTexCoord13 : TEXCOORD13;
	float4 iTexCoord14 : TEXCOORD14;
	float4 iTexCoord15 : TEXCOORD15;
	float4 iColor0 : COLOR0;
	float4 iColor1 : COLOR1;
#endif
};
struct PixelShaderOutput
{
#ifdef __air__
	float4 oC0 [[color(0)]];
#else
	float4 oC0 : SV_Target0;
#endif
};
#ifdef __air__
[[fragment]]
[[early_fragment_tests]]
#else
#if !defined(__spirv__)
[shader("pixel")]
#endif
#ifndef XENOS_RECOMP_LATE_FRAGMENT_TESTS
[earlydepthstencil]
#endif
#endif
PixelShaderOutput shaderMain(
#ifdef __air__
	Interpolators input [[stage_in]],
	bool iFace [[front_facing]],
	constant Texture2DDescriptorHeap* g_Texture2DDescriptorHeap [[buffer(0)]],
	constant Texture2DArrayDescriptorHeap* g_Texture2DArrayDescriptorHeap [[buffer(1)]],
	constant Texture3DDescriptorHeap* g_Texture3DDescriptorHeap [[buffer(2)]],
	constant TextureCubeDescriptorHeap* g_TextureCubeDescriptorHeap [[buffer(3)]],
	constant SamplerDescriptorHeap* g_SamplerDescriptorHeap [[buffer(4)]],
	constant PushConstants& g_PushConstants [[buffer(8)]]
#else
	Interpolators input,
#ifdef __spirv__
	in bool iFace : SV_IsFrontFace
#else
	in uint iFace : SV_IsFrontFace
#endif

#endif
)
{
#ifdef __air__
	PixelShaderOutput output = PixelShaderOutput{};
#else
	PixelShaderOutput output = (PixelShaderOutput)0;
#endif
#ifdef __air__
	float4 c248 = as_type<float4>(uint4(0x0, 0x0, 0x0, 0x0));
#else
	float4 c248 = asfloat(uint4(0x0, 0x0, 0x0, 0x0));
#endif
#ifdef __air__
	float4 c249 = as_type<float4>(uint4(0x0, 0x0, 0x0, 0x0));
#else
	float4 c249 = asfloat(uint4(0x0, 0x0, 0x0, 0x0));
#endif
#ifdef __air__
	float4 c250 = as_type<float4>(uint4(0x3E800000, 0x3E000000, 0x0, 0x0));
#else
	float4 c250 = asfloat(uint4(0x3E800000, 0x3E000000, 0x0, 0x0));
#endif
#ifdef __air__
	float4 c251 = as_type<float4>(uint4(0x3F000000, 0x43800000, 0x3F600000, 0xBF800000));
#else
	float4 c251 = asfloat(uint4(0x3F000000, 0x43800000, 0x3F600000, 0xBF800000));
#endif
#ifdef __air__
	float4 c252 = as_type<float4>(uint4(0x0, 0x3F800000, 0x3EC00000, 0x3F000000));
#else
	float4 c252 = asfloat(uint4(0x0, 0x3F800000, 0x3EC00000, 0x3F000000));
#endif
#ifdef __air__
	float4 c253 = as_type<float4>(uint4(0x3F200000, 0x3F400000, 0x423C851F, 0x4268A7F0));
#else
	float4 c253 = asfloat(uint4(0x3F200000, 0x3F400000, 0x423C851F, 0x4268A7F0));
#endif
#ifdef __air__
	float4 c254 = as_type<float4>(uint4(0x3D93A92A, 0x3E59999A, 0x3F372474, 0xBF000000));
#else
	float4 c254 = asfloat(uint4(0x3D93A92A, 0x3E59999A, 0x3F372474, 0xBF000000));
#endif
#ifdef __air__
	float4 c255 = as_type<float4>(uint4(0x41000000, 0x40800000, 0x40000000, 0x43800000));
#else
	float4 c255 = asfloat(uint4(0x41000000, 0x40800000, 0x40000000, 0x43800000));
#endif

	float4 r0 = input.iTexCoord0;
	float4 r1 = input.iTexCoord1;
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

	r9.z = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		GBufferTextureSampler3_Texture2DDescriptorIndex, GBufferTextureSampler3_SamplerDescriptorIndex, r0.xy, float2(0, 0)).x;
	r6.w = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		GBufferTextureSampler2_Texture2DDescriptorIndex, GBufferTextureSampler2_SamplerDescriptorIndex, r0.xy, float2(0, 0)).w;
	r7.xyz = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r0.xy, float2(-0.5, -1.5)).xyz;
	r6.xyz = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r0.xy, float2(0, 0)).xyz;
	r8.xyz = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r0.xy, float2(1.5, -0.5)).xyz;
	r3.yzw = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r0.xy, float2(0.5, 1.5)).xyz;
	r5.xyz = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r0.xy, float2(-1.5, 0.5)).xyz;
	r0.z = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		StencilCopySampler_Texture2DDescriptorIndex, StencilCopySampler_SamplerDescriptorIndex, r0.xy, float2(0, 0)).z;
	r1.yzw = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		BloomSampler_Texture2DDescriptorIndex, BloomSampler_SamplerDescriptorIndex, r0.xy, float2(0, 0)).xyz;
	r9.xy = (float2)((r0.yx * c255.zz + c251.ww));
	r10.w = (float)((-r9.x * dofProj.w));
	r4.w = (float)((r0.z >= PLAYER_MASK.x));
	r2.w = (float)((dot(r5.zxy, c254.xyz)));
	r11.x = (float)((dot(r3.wyz, c254.xyz)));
	r11.y = (float)((dot(r8.zxy, c254.xyz)));
	r11.z = (float)((dot(r6.zxy, c254.xyz)));
	r11.w = (float)((dot(r7.zxy, c254.xyz)));
	ps = -abs(r0.x) > 0.0;
	r3.x = ps;
	r6.xyzw = (float4)((r6.wyzx * c251.yxxx));
	ps = dofBlur.z - dofBlur.y;
	r2.z = ps;
	r14.xyzw = (float4)((selectWrapper(c252.xyyy == 0.0, r2.wwww, r11.xxwy)));
	r13.xyzw = (float4)((c255.xyzw >= r6.xxxx));
	ps = -r6.x >= 0.0;
	r0.z = ps;
	r12.w = (float)((r0.z + -r13.z));
	ps = dofProj.y * dofProj.x;
	r6.x = ps;
	r9.w = (float)((dot(r14.yzwx, c250.xxxx)));
	ps = r13.x - r13.w;
	r12.x = ps;
	r11.xyzw = (float4)((-r9.wwww + r11.xyzw));
	ps = r13.z - r13.y;
	r12.y = ps;
	r10.xyz = (float3)((r9.yzz * dofProj.zyx));
	ps = r13.y - r13.x;
	r12.z = ps;
	r0.z = (float)((dot(r12.xzyw, c254.wwww)));
	ps = r10.y - r10.z;
	r1.x = ps;
	r12.xyzw = (float4)((r12.wyxz * c254.wwww));
	ps = c252.w + r0.z;
	r0.w = ps;
	r4.xyz = (float3)((r12.xxx * r7.zyx + r6.zyw));
	r4.xyz = (float3)((r12.yyy * r8.xzy + r4.zxy));
	r0.z = (float)((r1.x + dofProj.x));
	ps = clamp(rcp(r0.w), -FLT_MAX, FLT_MAX);
	r0.w = ps;
	r5.w = (float)((r11.z * r11.z));
	ps = clamp(rcp(r0.z), -FLT_MAX, FLT_MAX);
	r0.z = ps;
	r4.xyz = (float3)((r12.www * r3.ywz + r4.xyz));
	r6.yzw = (float3)((r12.zzz * r5.xzy + r4.xyz));
	r6.xyzw = (float4)((r6.xyzw * r0.zwww));
	ps = Exposure.x * r1.w;
	r4.z = ps;
	r7.w = (float)((dot(r11.xwy, r11.xwy)));
	ps = max(r6.x, r6.x);
	r0.z = ps;
	r2.xy = (float2)((r10.xw * r6.xx));
	ps = -dofDist.w - -r0.z;
	r0.z = ps;
	r10.xyz = (float3)((-r6.xxx * motionBlurMatrix(2).zxy + motionBlurMatrix(3).zxy));
	r10.xyz = (float3)((r2.yyy * motionBlurMatrix(1).zxy + r10.xyz));
	r0.z = (float)((-dofDist.y * c252.w + r0.z));
	r1.x = (float)((max(r0.z, c252.x)));
	ps = Exposure.x * r1.z;
	r4.y = ps;
	r10.xyz = (float3)((r2.xxx * motionBlurMatrix(0).zxy + r10.xyz));
	r2.x = (float)((-r10.x * dofProj.z));
	ps = clamp(rcp(dofDist.z), -FLT_MAX, FLT_MAX);
	r0.w = ps;
	r0.z = (float)((r10.x * dofProj.w));
	ps = clamp(rcp(r2.x), -FLT_MAX, FLT_MAX);
	r2.x = ps;
	r10.w = (float)((r1.x * r0.w));
	ps = clamp(rcp(r0.z), -FLT_MAX, FLT_MAX);
	r2.y = ps;
	r2.xyz = (float3)((r10.wyz * r2.zxy));
	ps = Exposure.x * r1.y;
	r4.x = ps;
	r2.yzw = (float3)((-r9.yxw + r2.yzw));
	ps = dofBlur.y + r2.x;
	r0.z = ps;
	r2.x = (float)((min(r0.z, dofBlur.z)));
	ps = (gDirectionalMotionBlurLength.x * LibertyMotionBlurTimeScale) * r2.y;
	r1.x = ps;
	r0.zw = (float2)((r2.xw * r2.xw));
	ps = (gDirectionalMotionBlurLength.x * LibertyMotionBlurTimeScale) * r2.z;
	r1.w = ps;
	r1.y = (float)((r7.w + r0.w));
	ps = c250.y * r1.x;
	r2.x = ps;
	r1.y = (float)((r5.w >= r1.y));
	ps = c252.y - r0.z;
	r1.z = ps;
	r1.z = (float)((r1.z * r1.y + r0.z));
	r0.z = (float)((r1.z * c250.x));
	ps = c250.y * r1.w;
	r2.y = ps;
	r3.yzw = (float3)((r0.zzz * r3.ywz));
	ps = c252.y - r1.z;
	r0.w = ps;
	r3.yzw = (float3)((r0.zzz * r8.xzy + r3.yzw));
	r3.yzw = (float3)((r0.zzz * r7.xzy + r3.yzw));
	r3.yzw = (float3)((r6.yzw * r0.www + r3.yzw));
	r3.zw = (float2)((r0.zz * r5.yz + r3.wz));
	r0.z = (float)((r0.z * r5.x + r3.y));
	r3.y = (float)((r0.z * r1.z));
	// The native split post-FX chain has already applied stipple and DOF to stage 2.
	// Use that prefiltered center color as the motion blur identity input.
	r3.yzw = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r0.xy, float2(0, 0)).xyz;
	p0 = r4.w == 0.0;
	ps = p0 ? 0.0 : 1.0;
	if (p0)
	{
		r0.zw = (float2)((r3.yz * c255.xx));
	}
	if (p0)
	{
		r0.zw = (float2)((r0.xy * c253.wz + r0.zw));
	}
	if (p0)
	{
		r0.z = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			JitterSampler_Texture2DDescriptorIndex, JitterSampler_SamplerDescriptorIndex, r0.zw, float2(0, 0)).x;
	}
	if (p0)
	{
		ps = -c252.w - -r0.z;
		r0.z = ps;
	}
	if (p0)
	{
		r5.zw = (float2)((r2.xy + r2.xy));
		ps = c251.z * r1.x;
		r5.x = ps;
	}
	if (p0)
	{
		r0.xy = (float2)((r2.xy * r0.zz + r0.xy));
	}
	if (p0)
	{
		r8.zw = (float2)((r0.xy + r2.xy));
		ps = c251.z * r1.w;
		r5.y = ps;
	}
	if (p0)
	{
		r13.xyzw = (float4)((r1.xwxw * c252.zzww + r0.xyxy));
	}
	if (p0)
	{
		r14.xyzw = (float4)((r1.xwxw * c253.xxyy + r0.xyxy));
	}
	if (p0)
	{
		r12.xyzw = (float4)((r0.xyxy + r5.xyzw));
	}
	if (p0)
	{
		r2.xyz = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r12.xy, float2(0, 0)).xyz;
	}
	if (p0)
	{
		r5.xyz = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r14.zw, float2(0, 0)).xyz;
	}
	if (p0)
	{
		r6.xyz = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r14.xy, float2(0, 0)).xyz;
	}
	if (p0)
	{
		r7.xyz = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r13.zw, float2(0, 0)).xyz;
	}
	if (p0)
	{
		r10.xyz = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r13.xy, float2(0, 0)).xyz;
	}
	if (p0)
	{
		r9.yzw = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r12.zw, float2(0, 0)).xyz;
	}
	if (p0)
	{
		r11.yzw = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			HDRSampler_Texture2DDescriptorIndex, HDRSampler_SamplerDescriptorIndex, r8.zw, float2(0, 0)).xyz;
	}
	if (p0)
	{
		r0.x = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			StencilCopySampler_Texture2DDescriptorIndex, StencilCopySampler_SamplerDescriptorIndex, r12.xy, float2(0, 0)).z;
	}
	if (p0)
	{
		r0.y = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			StencilCopySampler_Texture2DDescriptorIndex, StencilCopySampler_SamplerDescriptorIndex, r14.xy, float2(0, 0)).z;
	}
	if (p0)
	{
		r0.z = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			StencilCopySampler_Texture2DDescriptorIndex, StencilCopySampler_SamplerDescriptorIndex, r14.zw, float2(0, 0)).z;
	}
	if (p0)
	{
		r8.x = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			StencilCopySampler_Texture2DDescriptorIndex, StencilCopySampler_SamplerDescriptorIndex, r13.zw, float2(0, 0)).z;
	}
	if (p0)
	{
		r8.y = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			StencilCopySampler_Texture2DDescriptorIndex, StencilCopySampler_SamplerDescriptorIndex, r13.xy, float2(0, 0)).z;
	}
	if (p0)
	{
		r8.z = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			StencilCopySampler_Texture2DDescriptorIndex, StencilCopySampler_SamplerDescriptorIndex, r8.zw, float2(0, 0)).z;
	}
	if (p0)
	{
		r8.w = tfetch2D(
#ifdef __air__
			g_Texture2DDescriptorHeap,
			g_SamplerDescriptorHeap,
#endif
			StencilCopySampler_Texture2DDescriptorIndex, StencilCopySampler_SamplerDescriptorIndex, r12.zw, float2(0, 0)).z;
	}
	if (p0)
	{
		r1.xy = (float2)((r1.xw * globalScreenSize.xy));
	}
	if (p0)
	{
		r1.x = (float)((dot(r1.xy, r1.xy) + c252.x));
	}
	if (p0)
	{
		r8.xyzw = (float4)((r8.xyzw >= PLAYER_MASK.xxxx));
	}
	if (p0)
	{
		r0.xzw = (float3)((r0.xyz >= PLAYER_MASK.xxx));
		ps = c252.y + r1.z;
		r0.y = ps;
	}
	if (p0)
	{
		r11.x = (float)((dot(-r0.zxw, c252.yyy)));
		ps = c252.y - r0.z;
		r2.w = ps;
	}
	if (p0)
	{
		r9.x = (float)((dot(-r8.zwyx, c252.yyyy)));
		ps = c252.y - r0.x;
		r1.w = ps;
	}
	if (p0)
	{
		r8.xyzw = (float4)((-r8.ywxz + c252.yyyy));
		ps = sqrt(abs(r1.x));
		r0.x = ps;
	}
	if (p0)
	{
		r11.y = (float)((r8.w * r11.y + r3.y));
	}
	if (p0)
	{
		r11.zw = (float2)((r8.ww * r11.zw + r3.zw));
	}
	if (p0)
	{
		r9.yzw = (float3)((r8.yyy * r9.ywz));
		ps = c252.y - r0.w;
		r0.z = ps;
	}
	if (p0)
	{
		r9.xyzw = (float4)((r11.xyzw + r9.xywz));
		ps = c252.w * r0.x;
		r0.w = ps;
	}
	if (p0)
	{
		r1.xyz = (float3)((r8.xxx * r10.xyz + r9.yzw));
	}
	if (p0)
	{
		r0.x = (float)((r9.x + c255.x));
		ps = clamp(rcp(r0.y), -FLT_MAX, FLT_MAX);
		r0.y = ps;
	}
	if (p0)
	{
		r0.w = (float)((saturate(r0.w * r0.y)));
		ps = clamp(rcp(r0.x), -FLT_MAX, FLT_MAX);
		r0.y = ps;
	}
	if (p0)
	{
		r1.xyz = (float3)((r8.zzz * r7.xyz + r1.xyz));
	}
	if (p0)
	{
		r1.xyz = (float3)((r2.www * r6.xyz + r1.xyz));
	}
	if (p0)
	{
		r1.xyz = (float3)((r0.zzz * r5.xyz + r1.xyz));
	}
	if (p0)
	{
		r1.xyz = (float3)((r1.www * r2.xyz + r1.xyz));
	}
	if (p0)
	{
		r0.x = (float)((r1.x * r0.y + -r3.y));
	}
	if (p0)
	{
		r0.yz = (float2)((r1.yz * r0.yy + -r3.zw));
	}
	if (p0)
	{
		r3.yzw = (float3)((r0.www * r0.xyz + r3.yzw));
	}
	r0.x = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		AdapLumSampler_Texture2DDescriptorIndex, AdapLumSampler_SamplerDescriptorIndex, r3.xx, float2(0, 0)).x;
	ps = clamp(rcp(r0.x), -FLT_MAX, FLT_MAX);
	r0.x = ps;
	ps = ToneMapParams.y * r0.x;
	r0.w = ps;
	r2.x = (float)((deSatContrastGamma.z + -c252.y));
	ps = clamp(rcp(r0.w), -FLT_MAX, FLT_MAX);
	r0.x = ps;
	r0.xyz = (float3)((-r0.xxx * ToneMapParams.xxx + r4.xyz));
	r0.xyz = (float3)((max(r0.xyz, c252.xxx)));
	r0.xyz = (float3)((r0.xyz * ToneMapParams.zzz));
	r0.xyz = (float3)((r0.xyz * c250.xxx));
	r0.xyz = (float3)((r3.yzw * Exposure.xxx + r0.xyz));
	r0.yzw = (float3)((r0.www * r0.xyz));
	r0.x = (float)((dot(r0.wyz, c254.xyz)));
	r2.yzw = (float3)((-r0.xxx + r0.yzw));
	ps = ColorShift.w * r0.x;
	r0.y = ps;
	r1.xyz = (float3)((r0.yyy * ColorShift.xyz));
	r3.xyz = (float3)((r2.yzw * deSatContrastGamma.xxx + r0.xxx));
	r2.yz = (float2)((saturate(max(r0.yx, r0.yx))));
	r0.yzw = (float3)((r3.xyz + -r1.xyz));
	ps = clamp(log2(r2.z), -FLT_MAX, FLT_MAX);
	r0.x = ps;
	r0.xyzw = (float4)((r2.xyyy * r0.xyzw));
	r1.xyz = (float3)((r1.xyz + r0.yzw));
	r1.xyz = (float3)((r1.xyz * ColorCorrect.xyz));
	r0.yzw = (float3)((r1.xyz + r1.xyz));
	ps = exp2(r0.x);
	r0.x = ps;
	output.oC0.xyz = (float3)((r0.yzww * r0.xxxx).xyz);
	output.oC0.w = 1.0;
	BRANCH if (g_SpecConstants() & SPEC_CONSTANT_ALPHA_TEST)
	{
		uint alphaTestFunction = (g_SpecConstants() >> SPEC_CONSTANT_ALPHA_TEST_FUNCTION_SHIFT) & SPEC_CONSTANT_ALPHA_TEST_FUNCTION_MASK;
		bool alphaTestPass = AlphaTestPass(output.oC0.w, g_AlphaThreshold, alphaTestFunction);
		clip(alphaTestPass ? 1.0 : -1.0);
	}
	return output;
}
