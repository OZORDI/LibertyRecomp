// =============================================================================
// HDR Tone Mapping Pixel Shader
// Converts linear HDR content to scRGB or HDR10 PQ output format
// Uses Config::HDRPaperWhite and Config::HDRMaxLuminance for calibration
// =============================================================================

#include "../../../../tools/XenosRecomp/XenosRecomp/shader_common.h"

#ifdef __spirv__

#define g_HDRMode vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 0)
#define g_PaperWhiteNits vk::RawBufferLoad<float>(g_PushConstants.SharedConstants + 4)
#define g_MaxLuminanceNits vk::RawBufferLoad<float>(g_PushConstants.SharedConstants + 8)
#define g_TextureDescriptorIndex vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 12)
#define g_ViewportOffset vk::RawBufferLoad<int2>(g_PushConstants.SharedConstants + 16)
#define g_ViewportSize vk::RawBufferLoad<int2>(g_PushConstants.SharedConstants + 24)

#else

cbuffer HDRConstants : register(b2, space4)
{
    uint g_HDRMode;              // 0 = SDR, 1 = scRGB, 2 = HDR10
    float g_PaperWhiteNits;      // SDR white level in nits (default 200)
    float g_MaxLuminanceNits;    // Peak HDR brightness (default 1000)
    uint g_TextureDescriptorIndex;
    int2 g_ViewportOffset;
    int2 g_ViewportSize;
};

#endif

// sRGB OETF (Opto-Electronic Transfer Function) - linear to sRGB gamma
float3 LinearToSRGB(float3 linearColor)
{
    float3 lowPart = linearColor * 12.92;
    float3 highPart = 1.055 * pow(linearColor, 1.0 / 2.4) - 0.055;
    return lerp(highPart, lowPart, linearColor < 0.0031308);
}

// sRGB EOTF (Electro-Optical Transfer Function) - sRGB gamma to linear
float3 SRGBToLinear(float3 srgb)
{
    float3 lowPart = srgb / 12.92;
    float3 highPart = pow((srgb + 0.055) / 1.055, 2.4);
    return lerp(highPart, lowPart, srgb < 0.04045);
}

// ST.2084 PQ OETF - linear to PQ (for HDR10)
// Input: linear luminance in nits (0 to 10000)
// Output: PQ encoded value (0 to 1)
float3 LinearToPQ(float3 linearNits)
{
    const float m1 = 0.1593017578125;      // 2610 / 16384
    const float m2 = 78.84375;              // 2523 / 32 * 128
    const float c1 = 0.8359375;             // 3424 / 4096
    const float c2 = 18.8515625;            // 2413 / 128
    const float c3 = 18.6875;               // 2392 / 128
    
    // Normalize to 0-1 range (10000 nits max)
    float3 y = linearNits / 10000.0;
    y = max(y, 0.0);
    
    float3 ym1 = pow(y, m1);
    float3 pq = pow((c1 + c2 * ym1) / (1.0 + c3 * ym1), m2);
    
    return pq;
}

// BT.709 to BT.2020 color space conversion matrix
float3 BT709ToBT2020(float3 color)
{
    const float3x3 mat = float3x3(
        0.6274040,  0.3292820,  0.0433136,
        0.0690970,  0.9195400,  0.0113612,
        0.0163916,  0.0880132,  0.8955950
    );
    return mul(mat, color);
}

// Reinhard tone mapping (simple, keeps highlights)
float3 ReinhardTonemap(float3 hdr, float maxWhite)
{
    float3 numerator = hdr * (1.0 + hdr / (maxWhite * maxWhite));
    return numerator / (1.0 + hdr);
}

// ACES Filmic Tone Mapping
float3 ACESFilmicTonemap(float3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

float4 shaderMain(in float4 position : SV_Position) : SV_Target
{
    Texture2D<float4> texture = g_Texture2DDescriptorHeap[g_TextureDescriptorIndex];
    
    int2 movedPosition = int2(position.xy) - g_ViewportOffset;
    bool boxed = any(movedPosition < 0) || any(movedPosition >= g_ViewportSize);
    if (boxed) movedPosition = 0;
    
    float4 color = boxed ? float4(0, 0, 0, 1) : texture.Load(int3(movedPosition, 0));
    
    // Input is assumed to be linear HDR in scene-referred values
    float3 hdrLinear = color.rgb;
    
    // HDR Mode handling
    if (g_HDRMode == 0)
    {
        // SDR Output - apply tone mapping and gamma
        float3 tonemapped = ACESFilmicTonemap(hdrLinear);
        color.rgb = LinearToSRGB(tonemapped);
    }
    else if (g_HDRMode == 1)
    {
        // scRGB Output (Windows HDR)
        // scRGB uses linear encoding with 80 nits = 1.0
        // Scale paper white: if paper white is 200 nits, SDR 1.0 becomes 200/80 = 2.5 in scRGB
        float scRGBScale = g_PaperWhiteNits / 80.0;
        
        // Apply soft roll-off for highlights above paper white
        float maxHighlight = g_MaxLuminanceNits / g_PaperWhiteNits;
        float3 scaled = hdrLinear * scRGBScale;
        
        // Soft knee compression for values above 1.0 (paper white)
        float3 compressed = scaled;
        for (int i = 0; i < 3; i++)
        {
            if (scaled[i] > 1.0)
            {
                float excess = scaled[i] - 1.0;
                float maxExcess = (g_MaxLuminanceNits / 80.0) - 1.0;
                compressed[i] = 1.0 + excess * (maxExcess / (maxExcess + excess));
            }
        }
        
        color.rgb = compressed;
    }
    else if (g_HDRMode == 2)
    {
        // HDR10 PQ Output
        // Convert to BT.2020 color space
        float3 bt2020 = BT709ToBT2020(hdrLinear);
        
        // Scale to nits based on paper white
        float3 nits = bt2020 * g_PaperWhiteNits;
        
        // Soft clip to max luminance
        nits = min(nits, g_MaxLuminanceNits);
        
        // Apply PQ OETF
        color.rgb = LinearToPQ(nits);
    }
    
    return color;
}
