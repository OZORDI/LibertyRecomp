// SSAO Composite Pixel Shader
// Applies the ambient occlusion to the scene color

Texture2D g_colorTex : register(t0);   // Scene color
Texture2D g_aoTex : register(t1);      // AO texture (R=AO value)

SamplerState g_linearSampler : register(s0);
SamplerState g_pointSampler : register(s1);

cbuffer CompositeConstants : register(b0)
{
    float4 g_resolution;          // (1/width, 1/height, width, height)
    float g_aoIntensity;          // AO intensity multiplier
    float g_aoMinValue;           // Minimum AO value (prevents pure black)
    float g_debugMode;            // 0=composite, 1=AO only, 2=AO inverted
    float g_padding;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    
    // Sample scene color
    float4 sceneColor = g_colorTex.SampleLevel(g_linearSampler, texCoord, 0);
    
    // Sample AO (R channel contains the AO value)
    float ao = g_aoTex.SampleLevel(g_pointSampler, texCoord, 0).r;
    
    // Apply intensity and clamp to minimum
    ao = lerp(1.0, ao, g_aoIntensity);
    ao = max(ao, g_aoMinValue);
    
    // Debug modes
    if (g_debugMode > 0.5 && g_debugMode < 1.5)
    {
        // AO visualization only
        return float4(ao, ao, ao, 1.0);
    }
    else if (g_debugMode > 1.5)
    {
        // Inverted AO (occlusion shown as white)
        float occl = 1.0 - ao;
        return float4(occl, occl, occl, 1.0);
    }
    
    // Apply AO to scene color (multiply with ambient/indirect lighting)
    // This affects the overall scene but preserves direct lighting highlights
    float3 result = sceneColor.rgb * ao;
    
    return float4(result, sceneColor.a);
}
