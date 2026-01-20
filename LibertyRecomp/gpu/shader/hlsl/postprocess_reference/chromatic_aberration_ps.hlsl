// Chromatic Aberration Pixel Shader
// Simulates lens color fringing by offsetting RGB channels

Texture2D g_colorTex : register(t0);
SamplerState g_linearSampler : register(s0);

cbuffer ChromaticAberrationConstants : register(b0)
{
    float4 g_resolution;     // (1/width, 1/height, width, height)
    float g_intensity;       // Overall intensity multiplier
    float g_redOffset;       // Red channel offset multiplier
    float g_blueOffset;      // Blue channel offset multiplier
    float g_radialFalloff;   // How much effect increases toward edges
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    
    // Calculate distance from center (0-1 range)
    float2 centerOffset = texCoord - 0.5;
    float distFromCenter = length(centerOffset);
    
    // Handle exact center pixel - use symmetric epsilon to avoid directional bias
    // This preserves symmetry across all quadrants
    float2 safeOffset = centerOffset;
    if (distFromCenter < 0.0001)
    {
        // At exact center, no aberration needed
        return g_colorTex.SampleLevel(g_linearSampler, texCoord, 0);
    }
    
    // Radial falloff - more aberration at edges
    float falloff = pow(distFromCenter * 2.0, g_radialFalloff);
    
    // Direction from center (for radial aberration)
    float2 direction = centerOffset / distFromCenter;
    
    // Calculate per-channel offsets
    float2 pixelSize = g_resolution.xy;
    float offsetScale = g_intensity * falloff;
    
    float2 redOffset = direction * pixelSize * g_redOffset * offsetScale;
    float2 blueOffset = direction * pixelSize * g_blueOffset * offsetScale;
    
    // Sample each channel with offset
    // Red shifts outward, blue shifts inward for realistic lens CA
    float r = g_colorTex.SampleLevel(g_linearSampler, texCoord + redOffset, 0).r;
    float g = g_colorTex.SampleLevel(g_linearSampler, texCoord, 0).g;
    float b = g_colorTex.SampleLevel(g_linearSampler, texCoord - blueOffset, 0).b;
    
    // Original alpha
    float a = g_colorTex.SampleLevel(g_linearSampler, texCoord, 0).a;
    
    return float4(r, g, b, a);
}
