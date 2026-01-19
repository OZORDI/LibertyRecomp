// =============================================================================
// HDR Tone Mapping Pixel Shader - Metal Version
// Converts linear HDR content to scRGB or HDR10 PQ output format
// Uses Config::HDRPaperWhite and Config::HDRMaxLuminance for calibration
// =============================================================================

#include <metal_stdlib>
using namespace metal;

struct HDRConstants {
    uint hdrMode;              // 0 = SDR, 1 = scRGB, 2 = HDR10
    float paperWhiteNits;      // SDR white level in nits (default 200)
    float maxLuminanceNits;    // Peak HDR brightness (default 1000)
    uint textureDescriptorIndex;
    int2 viewportOffset;
    int2 viewportSize;
};

// sRGB OETF (Opto-Electronic Transfer Function) - linear to sRGB gamma
float3 LinearToSRGB(float3 linear)
{
    float3 srgb;
    srgb = select(1.055 * pow(linear, 1.0 / 2.4) - 0.055, linear * 12.92, linear < 0.0031308);
    return srgb;
}

// sRGB EOTF (Electro-Optical Transfer Function) - sRGB gamma to linear
float3 SRGBToLinear(float3 srgb)
{
    float3 linear;
    linear = select(pow((srgb + 0.055) / 1.055, 2.4), srgb / 12.92, srgb < 0.04045);
    return linear;
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
    y = max(y, float3(0.0));
    
    float3 ym1 = pow(y, float3(m1));
    float3 pq = pow((c1 + c2 * ym1) / (1.0 + c3 * ym1), float3(m2));
    
    return pq;
}

// BT.709 to BT.2020 color space conversion matrix
float3 BT709ToBT2020(float3 color)
{
    const float3x3 mat = float3x3(
        float3(0.6274040,  0.3292820,  0.0433136),
        float3(0.0690970,  0.9195400,  0.0113612),
        float3(0.0163916,  0.0880132,  0.8955950)
    );
    return mat * color;
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

fragment float4 shaderMain(
    float4 position [[position]],
    constant HDRConstants& constants [[buffer(0)]],
    texture2d<float, access::sample> inputTexture [[texture(0)]])
{
    int2 movedPosition = int2(position.xy) - constants.viewportOffset;
    bool boxed = any(movedPosition < 0) || any(movedPosition >= constants.viewportSize);
    if (boxed) movedPosition = int2(0);
    
    float4 color = boxed ? float4(0, 0, 0, 1) : inputTexture.read(uint2(movedPosition));
    
    // Input is assumed to be linear HDR in scene-referred values
    float3 hdrLinear = color.rgb;
    
    // HDR Mode handling
    if (constants.hdrMode == 0)
    {
        // SDR Output - apply tone mapping and gamma
        float3 tonemapped = ACESFilmicTonemap(hdrLinear);
        color.rgb = LinearToSRGB(tonemapped);
    }
    else if (constants.hdrMode == 1)
    {
        // scRGB Output (Windows HDR)
        // scRGB uses linear encoding with 80 nits = 1.0
        float scRGBScale = constants.paperWhiteNits / 80.0;
        
        // Apply soft roll-off for highlights above paper white
        float3 scaled = hdrLinear * scRGBScale;
        
        // Soft knee compression for values above 1.0 (paper white)
        float3 compressed = scaled;
        for (int i = 0; i < 3; i++)
        {
            if (scaled[i] > 1.0)
            {
                float excess = scaled[i] - 1.0;
                float maxExcess = (constants.maxLuminanceNits / 80.0) - 1.0;
                compressed[i] = 1.0 + excess * (maxExcess / (maxExcess + excess));
            }
        }
        
        color.rgb = compressed;
    }
    else if (constants.hdrMode == 2)
    {
        // HDR10 PQ Output
        // Convert to BT.2020 color space
        float3 bt2020 = BT709ToBT2020(hdrLinear);
        
        // Scale to nits based on paper white
        float3 nits = bt2020 * constants.paperWhiteNits;
        
        // Soft clip to max luminance
        nits = min(nits, float3(constants.maxLuminanceNits));
        
        // Apply PQ OETF
        color.rgb = LinearToPQ(nits);
    }
    
    return color;
}
