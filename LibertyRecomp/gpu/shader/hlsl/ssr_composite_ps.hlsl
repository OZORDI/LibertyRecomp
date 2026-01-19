// =============================================================================
// Screen-Space Reflections (SSR) - Composite Pass
// Blends SSR results with scene color based on material properties
// =============================================================================

cbuffer SSRCompositeConstants : register(b0)
{
    float4 g_resolution;          // (1/width, 1/height, width, height)
    float g_intensity;            // Overall SSR intensity
    float g_maxRoughness;         // Maximum roughness for reflections
    float2 g_padding;
};

// Textures
Texture2D<float4> g_colorTexture : register(t0);      // Original scene color
Texture2D<float4> g_reflectionTexture : register(t1); // SSR result (RGB=color, A=confidence)
Texture2D<float> g_depthTexture : register(t2);       // Depth buffer
SamplerState g_linearSampler : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

float4 main(PSInput input) : SV_Target0
{
    float2 uv = input.texCoord;
    
    // Sample original color
    float4 sceneColor = g_colorTexture.Sample(g_linearSampler, uv);
    
    // Sample reflection
    float4 reflection = g_reflectionTexture.Sample(g_linearSampler, uv);
    
    // Sample depth for sky detection
    float depth = g_depthTexture.Sample(g_linearSampler, uv).r;
    
    // Skip sky pixels
    if (depth >= 1.0)
        return sceneColor;
    
    // Get reflection confidence from alpha
    float confidence = reflection.a * g_intensity;
    
    // Blend reflection with scene
    float3 finalColor = lerp(sceneColor.rgb, reflection.rgb, confidence);
    
    return float4(finalColor, sceneColor.a);
}
