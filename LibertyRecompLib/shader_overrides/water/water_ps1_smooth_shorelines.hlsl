// Exact XenosRecomp translation of the Xbox water_ps1 container. The only
// behavioral changes mirror FusionShaders SmoothShorelines: exponential
// shallow-water coverage and three extra spatial foam octaves. The native
// simulation, shadowing, normal/depth inputs, reflection offsets, lighting,
// fog, and console foam path remain in their original instruction order.
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

#define DepthBufferSampler_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 8)
#define DepthBufferSampler_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 112)
#define DepthBufferSampler_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 216)
#define DepthBufferSampler_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 320)
#define DepthBufferSampler_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 424)
#define NormBufferSampler_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 16)
#define NormBufferSampler_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 120)
#define NormBufferSampler_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 224)
#define NormBufferSampler_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 328)
#define NormBufferSampler_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 432)
#define ReflectTextureSampler_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 4)
#define ReflectTextureSampler_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 108)
#define ReflectTextureSampler_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 212)
#define ReflectTextureSampler_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 316)
#define ReflectTextureSampler_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 420)
#define SurfaceTextureSampler_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 0)
#define SurfaceTextureSampler_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 104)
#define SurfaceTextureSampler_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 208)
#define SurfaceTextureSampler_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 312)
#define SurfaceTextureSampler_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 416)
#define bottomSkyColour vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 1152, 0x10)
#define gDirectionalColour vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 288, 0x10)
#define gDirectionalLight vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 272, 0x10)
#define gInvColorExpBias vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 736, 0x10)
#define gLightAmbient0 vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 592, 0x10)
#define gLightAmbient1 vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 608, 0x10)
#define gShadowMatrix(INDEX) selectWrapper((INDEX) < 164, vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + (60 + min(INDEX, 163)) * 16, 0x10), 0.0)
#define gShadowParam0123 vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 912, 0x10)
#define gShadowZSamplerDir_Texture2DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 60)
#define gShadowZSamplerDir_Texture2DArrayDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 164)
#define gShadowZSamplerDir_Texture3DDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 268)
#define gShadowZSamplerDir_TextureCubeDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 372)
#define gShadowZSamplerDir_SamplerDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 476)
#define globalFogColor vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 672, 0x10)
#define globalScalars vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 624, 0x10)
#define globalScreenSize vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 704, 0x10)
#define viewProj vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 1184, 0x10)
#define waterColour vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 1056, 0x10)
#define waterReflectionScale vk::RawBufferLoad<float4>(g_PushConstants.PixelShaderConstants + 1168, 0x10)

#elif defined(__air__)

#define DepthBufferSampler_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 8)))
#define DepthBufferSampler_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 112)))
#define DepthBufferSampler_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 216)))
#define DepthBufferSampler_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 320)))
#define DepthBufferSampler_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 424)))
#define NormBufferSampler_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 16)))
#define NormBufferSampler_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 120)))
#define NormBufferSampler_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 224)))
#define NormBufferSampler_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 328)))
#define NormBufferSampler_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 432)))
#define ReflectTextureSampler_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 4)))
#define ReflectTextureSampler_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 108)))
#define ReflectTextureSampler_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 212)))
#define ReflectTextureSampler_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 316)))
#define ReflectTextureSampler_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 420)))
#define SurfaceTextureSampler_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 0)))
#define SurfaceTextureSampler_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 104)))
#define SurfaceTextureSampler_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 208)))
#define SurfaceTextureSampler_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 312)))
#define SurfaceTextureSampler_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 416)))
#define bottomSkyColour (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 1152)))
#define gDirectionalColour (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 288)))
#define gDirectionalLight (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 272)))
#define gInvColorExpBias (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 736)))
#define gLightAmbient0 (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 592)))
#define gLightAmbient1 (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 608)))
#define gShadowMatrix(INDEX) selectWrapper((INDEX) < 164, (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + (60 + min((uint)(INDEX), (uint)163)) * 16))), 0.0)
#define gShadowParam0123 (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 912)))
#define gShadowZSamplerDir_Texture2DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 60)))
#define gShadowZSamplerDir_Texture2DArrayDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 164)))
#define gShadowZSamplerDir_Texture3DDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 268)))
#define gShadowZSamplerDir_TextureCubeDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 372)))
#define gShadowZSamplerDir_SamplerDescriptorIndex (*(reinterpret_cast<device uint*>(g_PushConstants.SharedConstants + 476)))
#define globalFogColor (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 672)))
#define globalScalars (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 624)))
#define globalScreenSize (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 704)))
#define viewProj (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 1184)))
#define waterColour (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 1056)))
#define waterReflectionScale (*(reinterpret_cast<device float4*>(g_PushConstants.PixelShaderConstants + 1168)))

#else

cbuffer PixelShaderConstants : register(b1, space4)
{
	float4 bottomSkyColour : packoffset(c72);
	float4 gDirectionalColour : packoffset(c18);
	float4 gDirectionalLight : packoffset(c17);
	float4 gInvColorExpBias : packoffset(c46);
	float4 gLightAmbient0 : packoffset(c37);
	float4 gLightAmbient1 : packoffset(c38);
	float4 gShadowMatrix[4] : packoffset(c60);
#define gShadowMatrix(INDEX) selectWrapper((INDEX) < 164, gShadowMatrix[min(INDEX, 163)], 0.0)
	float4 gShadowParam0123 : packoffset(c57);
	float4 globalFogColor : packoffset(c42);
	float4 globalScalars : packoffset(c39);
	float4 globalScreenSize : packoffset(c44);
	float4 viewProj : packoffset(c74);
	float4 waterColour : packoffset(c66);
	float4 waterReflectionScale : packoffset(c73);
};

cbuffer SharedConstants : register(b2, space4)
{
	uint DepthBufferSampler_Texture2DDescriptorIndex : packoffset(c0.z);
	uint DepthBufferSampler_Texture2DArrayDescriptorIndex : packoffset(c7.x);
	uint DepthBufferSampler_Texture3DDescriptorIndex : packoffset(c13.z);
	uint DepthBufferSampler_TextureCubeDescriptorIndex : packoffset(c20.x);
	uint DepthBufferSampler_SamplerDescriptorIndex : packoffset(c26.z);
	uint NormBufferSampler_Texture2DDescriptorIndex : packoffset(c1.x);
	uint NormBufferSampler_Texture2DArrayDescriptorIndex : packoffset(c7.z);
	uint NormBufferSampler_Texture3DDescriptorIndex : packoffset(c14.x);
	uint NormBufferSampler_TextureCubeDescriptorIndex : packoffset(c20.z);
	uint NormBufferSampler_SamplerDescriptorIndex : packoffset(c27.x);
	uint ReflectTextureSampler_Texture2DDescriptorIndex : packoffset(c0.y);
	uint ReflectTextureSampler_Texture2DArrayDescriptorIndex : packoffset(c6.w);
	uint ReflectTextureSampler_Texture3DDescriptorIndex : packoffset(c13.y);
	uint ReflectTextureSampler_TextureCubeDescriptorIndex : packoffset(c19.w);
	uint ReflectTextureSampler_SamplerDescriptorIndex : packoffset(c26.y);
	uint SurfaceTextureSampler_Texture2DDescriptorIndex : packoffset(c0.x);
	uint SurfaceTextureSampler_Texture2DArrayDescriptorIndex : packoffset(c6.z);
	uint SurfaceTextureSampler_Texture3DDescriptorIndex : packoffset(c13.x);
	uint SurfaceTextureSampler_TextureCubeDescriptorIndex : packoffset(c19.z);
	uint SurfaceTextureSampler_SamplerDescriptorIndex : packoffset(c26.x);
	uint gShadowZSamplerDir_Texture2DDescriptorIndex : packoffset(c3.w);
	uint gShadowZSamplerDir_Texture2DArrayDescriptorIndex : packoffset(c10.y);
	uint gShadowZSamplerDir_Texture3DDescriptorIndex : packoffset(c16.w);
	uint gShadowZSamplerDir_TextureCubeDescriptorIndex : packoffset(c23.y);
	uint gShadowZSamplerDir_SamplerDescriptorIndex : packoffset(c29.w);
	DEFINE_SHARED_CONSTANTS();
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
	float4 c236 = as_type<float4>(uint4(0x0, 0x0, 0x0, 0x0));
#else
	float4 c236 = asfloat(uint4(0x0, 0x0, 0x0, 0x0));
#endif
#ifdef __air__
	float4 c237 = as_type<float4>(uint4(0x0, 0x0, 0x0, 0x0));
#else
	float4 c237 = asfloat(uint4(0x0, 0x0, 0x0, 0x0));
#endif
#ifdef __air__
	float4 c238 = as_type<float4>(uint4(0x0, 0x0, 0x0, 0x0));
#else
	float4 c238 = asfloat(uint4(0x0, 0x0, 0x0, 0x0));
#endif
#ifdef __air__
	float4 c239 = as_type<float4>(uint4(0x40C90FDB, 0x3F7DDDDE, 0x0, 0x0));
#else
	float4 c239 = asfloat(uint4(0x40C90FDB, 0x3F7DDDDE, 0x0, 0x0));
#endif
#ifdef __air__
	float4 c240 = as_type<float4>(uint4(0x41200000, 0xC0490FDB, 0x3E22F983, 0x3C088889));
#else
	float4 c240 = asfloat(uint4(0x41200000, 0xC0490FDB, 0x3E22F983, 0x3C088889));
#endif
#ifdef __air__
	float4 c241 = as_type<float4>(uint4(0x3E4CCCCD, 0x3F94F20A, 0x44000002, 0x3D51B718));
#else
	float4 c241 = asfloat(uint4(0x3E4CCCCD, 0x3F94F20A, 0x44000002, 0x3D51B718));
#endif
#ifdef __air__
	float4 c242 = as_type<float4>(uint4(0xBF800000, 0x3D4CCCCD, 0x3CF5C28F, 0x38D1B717));
#else
	float4 c242 = asfloat(uint4(0xBF800000, 0x3D4CCCCD, 0x3CF5C28F, 0x38D1B717));
#endif
#ifdef __air__
	float4 c243 = as_type<float4>(uint4(0x43800000, 0x0, 0x3BA3D70A, 0xBF000000));
#else
	float4 c243 = asfloat(uint4(0x43800000, 0x0, 0x3BA3D70A, 0xBF000000));
#endif
#ifdef __air__
	float4 c244 = as_type<float4>(uint4(0x40000000, 0x3F000000, 0x3E000000, 0xBE800000));
#else
	float4 c244 = asfloat(uint4(0x40000000, 0x3F000000, 0x3E000000, 0xBE800000));
#endif
#ifdef __air__
	float4 c245 = as_type<float4>(uint4(0x40400000, 0x3F19999A, 0x3D4CCCCD, 0x3CA3D70A));
#else
	float4 c245 = asfloat(uint4(0x40400000, 0x3F19999A, 0x3D4CCCCD, 0x3CA3D70A));
#endif
#ifdef __air__
	float4 c246 = as_type<float4>(uint4(0x40000000, 0x3F800000, 0x3C23D70A, 0x3B03126F));
#else
	float4 c246 = asfloat(uint4(0x40000000, 0x3F800000, 0x3C23D70A, 0x3B03126F));
#endif
#ifdef __air__
	float4 c247 = as_type<float4>(uint4(0x3DCCCCCD, 0x3ECCCCCD, 0x3D4CCCCD, 0x3F900000));
#else
	float4 c247 = asfloat(uint4(0x3DCCCCCD, 0x3ECCCCCD, 0x3D4CCCCD, 0x3F900000));
#endif
#ifdef __air__
	float4 c248 = as_type<float4>(uint4(0x3D4CCCCD, 0x3D3A2E8C, 0x40400000, 0x3F000000));
#else
	float4 c248 = asfloat(uint4(0x3D4CCCCD, 0x3D3A2E8C, 0x40400000, 0x3F000000));
#endif
#ifdef __air__
	float4 c249 = as_type<float4>(uint4(0x39D1B717, 0x3A91A2B4, 0x3E800000, 0xC3FA0000));
#else
	float4 c249 = asfloat(uint4(0x39D1B717, 0x3A91A2B4, 0x3E800000, 0xC3FA0000));
#endif
#ifdef __air__
	float4 c250 = as_type<float4>(uint4(0x3FF33333, 0x3F000000, 0x37800000, 0x3FAAAAAB));
#else
	float4 c250 = asfloat(uint4(0x3FF33333, 0x3F000000, 0x37800000, 0x3FAAAAAB));
#endif
#ifdef __air__
	float4 c251 = as_type<float4>(uint4(0x3DCCCCCD, 0xBF000000, 0xBF800000, 0x3F800000));
#else
	float4 c251 = asfloat(uint4(0x3DCCCCCD, 0xBF000000, 0xBF800000, 0x3F800000));
#endif
#ifdef __air__
	float4 c252 = as_type<float4>(uint4(0x3E83126F, 0x3F83126F, 0x40400000, 0x40E46A7F));
#else
	float4 c252 = asfloat(uint4(0x3E83126F, 0x3F83126F, 0x40400000, 0x40E46A7F));
#endif
#ifdef __air__
	float4 c253 = as_type<float4>(uint4(0x3E800000, 0xBF000000, 0x3F000000, 0x447A0000));
#else
	float4 c253 = asfloat(uint4(0x3E800000, 0xBF000000, 0x3F000000, 0x447A0000));
#endif
#ifdef __air__
	float4 c254 = as_type<float4>(uint4(0x3A19999A, 0x3A99999A, 0x3B19999A, 0x3AE66667));
#else
	float4 c254 = asfloat(uint4(0x3A19999A, 0x3A99999A, 0x3B19999A, 0x3AE66667));
#endif
#ifdef __air__
	float4 c255 = as_type<float4>(uint4(0xBF000000, 0xC3FF8000, 0x41100000, 0x3F000000));
#else
	float4 c255 = asfloat(uint4(0xBF000000, 0xC3FF8000, 0x41100000, 0x3F000000));
#endif

	float4 r0 = input.iTexCoord0;
	float4 r1 = input.iTexCoord1;
	float4 r2 = input.iTexCoord2;
	float4 r3 = input.iTexCoord3;
	float4 r4 = input.iTexCoord4;
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

	r5.xyzw = (float4)((r2.xyxy * c248.xxyy));
	r6.xyzw = (float4)((r2.xyxy * c246.zzww));
	r16.yz = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		SurfaceTextureSampler_Texture2DDescriptorIndex, SurfaceTextureSampler_SamplerDescriptorIndex, r6.zw, float2(0, 0)).zw;
	r8.yz = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		SurfaceTextureSampler_Texture2DDescriptorIndex, SurfaceTextureSampler_SamplerDescriptorIndex, r6.xy, float2(0, 0)).zw;
	r6.zw = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		SurfaceTextureSampler_Texture2DDescriptorIndex, SurfaceTextureSampler_SamplerDescriptorIndex, r5.zw, float2(0, 0)).zw;
	r5.zw = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		SurfaceTextureSampler_Texture2DDescriptorIndex, SurfaceTextureSampler_SamplerDescriptorIndex, r5.xy, float2(0, 0)).xy;
	// FusionShaders SmoothShorelines retains the console foam sample above and
	// adds three deterministic spatial octaves at 2x, 4x, and 8x frequency.
	// The fixed UVs inherit the stock water simulation motion, so no frame-time
	// noise or history buffer is introduced.
	float2 smoothShoreFoamUv = r5.xy * 2.0;
#ifdef __air__
	float2 smoothShoreFoamOctave = tfetch2D(
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
		SurfaceTextureSampler_Texture2DDescriptorIndex, SurfaceTextureSampler_SamplerDescriptorIndex, smoothShoreFoamUv, float2(0, 0)).xy;
#else
	float2 smoothShoreFoamOctave = g_Texture2DDescriptorHeap[SurfaceTextureSampler_Texture2DDescriptorIndex].SampleLevel(
		g_SamplerDescriptorHeap[SurfaceTextureSampler_SamplerDescriptorIndex], smoothShoreFoamUv, 0.0).xy;
#endif
	r5.zw += smoothShoreFoamOctave * 0.5;
	smoothShoreFoamUv *= 2.0;
#ifdef __air__
	smoothShoreFoamOctave = tfetch2D(
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
		SurfaceTextureSampler_Texture2DDescriptorIndex, SurfaceTextureSampler_SamplerDescriptorIndex, smoothShoreFoamUv, float2(0, 0)).xy;
#else
	smoothShoreFoamOctave = g_Texture2DDescriptorHeap[SurfaceTextureSampler_Texture2DDescriptorIndex].SampleLevel(
		g_SamplerDescriptorHeap[SurfaceTextureSampler_SamplerDescriptorIndex], smoothShoreFoamUv, 0.0).xy;
#endif
	r5.zw += smoothShoreFoamOctave * 0.25;
	smoothShoreFoamUv *= 2.0;
#ifdef __air__
	smoothShoreFoamOctave = tfetch2D(
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
		SurfaceTextureSampler_Texture2DDescriptorIndex, SurfaceTextureSampler_SamplerDescriptorIndex, smoothShoreFoamUv, float2(0, 0)).xy;
#else
	smoothShoreFoamOctave = g_Texture2DDescriptorHeap[SurfaceTextureSampler_Texture2DDescriptorIndex].SampleLevel(
		g_SamplerDescriptorHeap[SurfaceTextureSampler_SamplerDescriptorIndex], smoothShoreFoamUv, 0.0).xy;
#endif
	r5.zw += smoothShoreFoamOctave * 0.125;
	r5.zw = saturate(r5.zw - 0.4375);
	r6.x = (float)((r2.w + -r4.w));
	r5.xy = (float2)((r1.ww * c249.wz + c253.wz));
	r10.xyz = (float3)((r2.xyz + -r3.xyz));
	ps = max(r6.x, r6.x);
	r6.xy = (float2)((r5.yy * r5.zw));
	ps = saturate((float)(r5.x * ps));
	r5.x = ps;
	r2.w = (float)((dot(r10.zxy, r10.zxy)));
	ps = clamp(rcp(r0.z), -FLT_MAX, FLT_MAX);
	r5.y = ps;
	r12.yz = (float2)((r5.yy * r0.xy));
	ps = c249.x * r2.w;
	r5.y = ps;
	r7.xyzw = (float4)((r6.xyzw + c253.xxyy));
	ps = c249.y * r2.w;
	r5.z = ps;
	r5.xyz = (float3)((-r5.yzx + c251.www));
	ps = max(r0.z, r0.z);
	r5.xy = (float2)((max(r5.xy, c243.yy)));
	ps = c242.z * ps;
	r6.z = ps;
	r9.xy = (float2)((r7.zw * c241.yy));
	ps = c251.w - r5.x;
	r5.w = ps;
	r6.x = (float)((saturate(r5.z * r5.y)));
	ps = clamp(rcp(r1.z), -FLT_MAX, FLT_MAX);
	r6.y = ps;
	r6.w = (float)((r5.x * r2.z));
	ps = clamp(log2(abs(r0.z)), -FLT_MAX, FLT_MAX);
	r5.z = ps;
	r11.xyz = (float3)((r6.www * gShadowMatrix(2).zyx + gShadowMatrix(3).zyx));
	r8.xw = (float2)((r5.xz * c247.zw));
	ps = max(r5.x, r5.x);
	r11.xyz = (float3)((r2.yyy * gShadowMatrix(1).zyx + r11.xyz));
	r2.xyz = (float3)((r2.xxx * gShadowMatrix(0).zyx + r11.xyz));
	r11.xyz = (float3)((r2.xyz + c243.xyy));
	ps = r6.y * ps;
	r6.y = ps;
	r0.x = (float)((dot(r11.xzy, r11.xzy)));
	ps = clamp(rcp(r6.z), -FLT_MAX, FLT_MAX);
	r9.w = ps;
	r6.xyz = (float3)((r6.xyy * r1.wxy));
	ps = saturate((float)(c250.z * r0.x));
	r6.w = ps;
	r13.yzw = (float3)((r8.xyz + c243.zww));
	ps = sqrt(r6.w);
	r9.z = ps;
	r16.xw = (float2)((r9.zw * c250.xy));
	r1.xyz = (float3)((r16.xyz + c251.xyy));
	r9.zw = (float2)((r1.yz * c241.ww));
	r9.xyzw = (float4)((r9.xyzw * r5.xxww));
	r5.xy = (float2)((r6.yz + r9.zw));
	r5.xyzw = (float4)((r13.wzzw * c252.xxyy + r5.yxxy));
	r11.xyzw = (float4)((r9.yxxy * c247.xxyy + r5.xyzw));
	r4.xy = (float2)((-r11.zw + r4.xy));
	r5.xy = (float2)((r6.xx * r4.xy + r11.zw));
	r4.x = (float)((dot(r5.xy, r5.xy) + c251.w));
	r1.yzw = (float3)((r12.zyy * c255.xxw));
	ps = clamp(rsqrt(abs(r4.x)), -FLT_MAX, FLT_MAX);
	r15.x = ps;
	r4.xyz = (float3)((r15.xxx * gShadowMatrix(2).yxz));
	r15.yz = (float2)((r5.xy * r15.xx));
	r4.xyz = (float3)((r15.zzz * gShadowMatrix(1).zyx + r4.zxy));
	r4.xyz = (float3)((r15.yyy * gShadowMatrix(0).zyx + r4.xyz));
	r9.yzw = (float3)((r4.xyz * r1.xxx + r2.xyz));
	r4.yz = (float2)((r9.zw * gShadowParam0123.zz));
	r4.x = (float)((r3.w + c251.w));
	ps = max(abs(r4.z), abs(r4.y));
	r1.x = ps;
	r5.xyzw = (float4)((r1.zyxw + c244.yyxy));
	r1.x = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		DepthBufferSampler_Texture2DDescriptorIndex, DepthBufferSampler_SamplerDescriptorIndex, r5.wy, float2(0.5, 0.5)).x;
	r3.zw = (float2)((globalScreenSize.xy * c252.zw));
	r1.xy = (float2)((r1.xx * viewProj.yx));
	r1.x = (float)((r1.x + -r1.y));
	ps = clamp(rsqrt(abs(r2.w)), -FLT_MAX, FLT_MAX);
	r1.y = ps;
	r2.x = (float)((r1.x + viewProj.x));
	ps = sqrt(abs(r6.x));
	r1.x = ps;
	r10.yzw = (float3)((r10.yxz * r1.yyy));
	ps = sqrt(abs(r1.x));
	r15.w = ps;
	r1.z = (float)((dot(r10.wzy, r15.xyz)));
	r1.xyw = (float3)((r15.wxw * c255.yxz));
	ps = r1.z + r1.z;
	r1.z = ps;
	r2.z = (float)((dot(r15.xyz, -gDirectionalLight.zxy)));
	ps = max(r1.z, r1.z);
	r14.yz = (float2)((r15.yz * r1.zz));
	ps = r15.x * ps;
	r14.x = ps;
	r14.xyz = (float3)((r10.wzy + -r14.xyz));
	ps = viewProj.y * viewProj.x;
	r12.w = ps;
	r4.xzw = (float3)((r4.xyz * c255.www));
	ps = c241.z + r1.x;
	r12.x = ps;
	r1.z = (float)((saturate(dot(r14.xyz, -gDirectionalLight.zxy))));
	ps = clamp(rcp(r2.x), -FLT_MAX, FLT_MAX);
	r3.y = ps;
	r1.z = (float)((r1.z + c242.w));
	ps = clamp(rcp(r5.z), -FLT_MAX, FLT_MAX);
	r2.x = ps;
	r2.xy = (float2)((r4.zw * r2.xx));
	ps = clamp(log2(abs(r1.z)), -FLT_MAX, FLT_MAX);
	r3.x = ps;
	r3.xyzw = (float4)((r12.yxwz * r3.zxyw));
	ps = r3.x + r3.w;
	r1.z = ps;
	r1.z = (float)((r1.z * c240.z + c255.w));
	r3.x = (float)((waterColour.w * c253.x));
	ps = frac(r1.z);
	r1.z = ps;
	r1.z = (float)((r1.z * c239.x + c240.y));
	r2.xyz = (float3)((r2.zxy + c244.wyy));
	ps = cos(r1.z);
	r12.x = ps;
	r2.w = (float)((-r2.y + c251.w));
	ps = sin(r1.z);
	r12.y = ps;
	r17.y = (float)((r12.x * c254.z + r2.z));
	r17.xzw = (float3)((-r12.yyx * c254.zxx + r2.wzw));
	r14.y = (float)((-r12.x * c254.y + r2.z));
	r14.xzw = (float3)((r12.yxy * c254.yww + r2.wwz));
	r12.xyz = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		NormBufferSampler_Texture2DDescriptorIndex, NormBufferSampler_SamplerDescriptorIndex, r5.wy, float2(0.5, 0.5)).xyz;
	r15.x = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		gShadowZSamplerDir_Texture2DDescriptorIndex, gShadowZSamplerDir_SamplerDescriptorIndex, r17.yx, float2(0, 0)).x;
	r15.y = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		gShadowZSamplerDir_Texture2DDescriptorIndex, gShadowZSamplerDir_SamplerDescriptorIndex, r14.wz, float2(0, 0)).x;
	r15.z = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		gShadowZSamplerDir_Texture2DDescriptorIndex, gShadowZSamplerDir_SamplerDescriptorIndex, r17.zw, float2(0, 0)).x;
	r15.w = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		gShadowZSamplerDir_Texture2DDescriptorIndex, gShadowZSamplerDir_SamplerDescriptorIndex, r14.yx, float2(0, 0)).x;
	r14.w = (float)((gShadowParam0123.w * c255.w));
	r14.xyz = (float3)((gDirectionalColour.xyz * gDirectionalColour.www));
	ps = exp2(r3.y);
	r9.x = ps;
	r1.z = (float)((min(r16.w, c246.x)));
	ps = saturate((float)(c250.w * r2.x));
	r2.z = ps;
	r9.xyzw = (float4)((r14.xyzw * r9.xxxy));
	ps = max(r14.x, r14.x);
	r15.xyzw = (float4)((r15.xyzw >= r9.wwww));
	ps = r2.z * ps;
	r2.x = ps;
	r13.x = (float)((dot(r15.zwxy, c253.xxxx)));
	ps = max(r14.y, r14.y);
	r2.w = (float)((-r13.x + c251.w));
	ps = r2.z * ps;
	r2.y = ps;
	r4.y = (float)((r6.w * r2.w));
	ps = max(r14.z, r14.z);
	r5.zw = (float2)((r13.yx + r4.xy));
	ps = r2.z * ps;
	r2.z = ps;
	r4.xyz = (float3)((r2.xyz * r5.www));
	ps = exp2(r8.w);
	r4.w = ps;
	r8.xyzw = (float4)((r4.xyzw * c245.zzzw));
	r12.w = (float)((min(r8.w, c246.y)));
	r13.xyzw = (float4)((r12.yxzw + c242.xxxy));
	r2.x = (float)((r3.z + -r0.z));
	ps = clamp(rcp(r13.w), -FLT_MAX, FLT_MAX);
	r0.x = ps;
	r3.yzw = (float3)((r13.xyz + r12.yxz));
	ps = max(r2.x, r2.x);
	r2.y = (float)((dot(r3.wzy, r3.wzy)));
	ps = saturate((float)(r0.x * ps));
	r0.x = ps;
	r0.x = (float)((-r0.x + c251.w));
	ps = clamp(rsqrt(abs(r2.y)), -FLT_MAX, FLT_MAX);
	r2.y = ps;
	r2.yzw = (float3)((r3.wyz * r2.yyy));
	ps = abs(r0.x) * abs(r0.x);
	r0.x = ps;
	r2.yzw = (float3)((r2.yzw * r0.xxx));
	r3.zw = (float2)((r2.zw * c255.ww + r11.xy));
	r3.y = (float)((r2.y * c255.w + c251.w));
	r0.x = (float)((dot(r3.ywz, r3.ywz)));
	r2.x = (float)((max(r2.x, c243.y)));
	ps = clamp(rsqrt(abs(r0.x)), -FLT_MAX, FLT_MAX);
	r2.y = ps;
	r2.xyzw = (float4)((r3.xzwy * r2.xyyy));
	r0.x = (float)((dot(r10.wzy, r2.wzy)));
	r0.x = (float)((max(-r0.x, c243.y)));
	ps = c251.w - r0.x;
	r2.w = ps;
	ps = abs(r2.w) * abs(r2.w);
	r10.x = ps;
	r3.xyw = (float3)((r10.zyx * r10.zyx));
	r2.w = (float)((r3.w * abs(r2.w)));
	ps = r3.x + r3.y;
	r3.z = ps;
	r11.xyz = (float3)((r2.yyz * c251.zww));
	ps = clamp(rsqrt(abs(r3.z)), -FLT_MAX, FLT_MAX);
	r3.z = ps;
	r10.xy = (float2)((r10.yz * r3.zz));
	r10.xyzw = (float4)((r11.yzxz * r10.xxyy));
	r10.xy = (float2)((r10.yx + r10.zw));
	r5.xy = (float2)((r10.xy * -r1.zz + r5.xy));
	r10.yz = (float2)((max(r5.xz, c240.ww)));
	r10.x = (float)((max(r10.z, r5.y)));
	r10.xy = (float2)((min(r10.xy, c239.yy)));
	r10.xyz = tfetch2D(
#ifdef __air__
		g_Texture2DDescriptorHeap,
		g_SamplerDescriptorHeap,
#endif
		ReflectTextureSampler_Texture2DDescriptorIndex, ReflectTextureSampler_SamplerDescriptorIndex, r10.yx, float2(0.5, 0.5)).xyz;
	r10.xyz = (float3)((r10.xyz * waterReflectionScale.xxx));
	r11.xyz = (float3)((-r10.xyz + globalFogColor.xyz));
	ps = max(bottomSkyColour.w, bottomSkyColour.w);
	r0.yzw = (float3)((r11.xyz * r0.www + r10.xyz));
	r5.xyz = (float3)((r9.xyz * r5.www));
	ps = c255.w * ps;
	r0.x = ps;
	r5.xyz = (float3)((r5.xyz * gDirectionalLight.www));
	ps = c240.x - r1.w;
	r1.z = ps;
	r5.xyz = (float3)((r5.xyz * r1.zzz + r0.yzw));
	r3.xyz = (float3)((r0.xxx * waterColour.xyz + r8.xyz));
	r2.xyz = (float3)((r4.xyz * r7.xxx));
	// r2.x is the stock positive scene-minus-water depth multiplied by
	// waterColour.w * 0.25. Scaling by -12 therefore reproduces the Fusion
	// exponent of -3 * depth * waterColour.w without disturbing reconstruction.
	r0.x = saturate(r1.y + c244.y);
	ps = exp2(-12.0 * r2.x);
	r0.y = saturate(1.00001001 - ps);
	r1.xyz = (float3)((r0.xxx * gLightAmbient1.xyz + gLightAmbient0.xyz));
	r4.xyzw = (float4)((r2.xyzw * c245.xxxy));
	r1.xyz = (float3)((r1.xyz * r7.yyy));
	ps = c241.x + r4.w;
	r0.z = ps;
	r1.w = (float)((max(r0.z, c243.y)));
	r2.xyzw = (float4)((r1.xyzw * c248.zzzw));
	ps = clamp(rcp(r0.y), -FLT_MAX, FLT_MAX);
	r0.z = ps;
	r1.xyz = (float3)((r5.xyz * r0.zzz));
	ps = max(r2.w, r2.w);
	r2.xyz = (float3)((r4.xyz + r2.xyz));
	ps = r0.y * ps;
	r0.z = ps;
	r1.xyz = (float3)((r1.xyz * r0.zzz + r3.xyz));
	r2.xyz = (float3)((r2.xyz + -r1.xyz));
	r1.xyz = (float3)((r6.xxx * r2.xyz + r1.xyz));
	output.oC0.xyz = (float3)((r1.xyz * globalScalars.yyy));
	ps = gInvColorExpBias.x * r0.y;
	output.oC0.w = ps;
	BRANCH if (g_SpecConstants() & SPEC_CONSTANT_ALPHA_TEST)
	{
		uint alphaTestFunction = (g_SpecConstants() >> SPEC_CONSTANT_ALPHA_TEST_FUNCTION_SHIFT) & SPEC_CONSTANT_ALPHA_TEST_FUNCTION_MASK;
		bool alphaTestPass = AlphaTestPass(output.oC0.w, g_AlphaThreshold, alphaTestFunction);
		clip(alphaTestPass ? 1.0 : -1.0);
	}
	return output;
}
