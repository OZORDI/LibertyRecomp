// AMD FidelityFX Super Resolution 1.0 - RCAS (Robust Contrast Adaptive Sharpening)
// Simplified implementation based on AMD FSR 1.0 specification

Texture2D g_inputTex : register(t0);
SamplerState g_pointSampler : register(s0);

cbuffer RCASConstants : register(b0)
{
    float4 g_resolution; // (1/width, 1/height, width, height)
    float g_sharpness;   // Sharpening amount (0.0 - 2.0, default 0.2)
    float3 g_padding;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    float2 pixelSize = g_resolution.xy;
    
    // Sample center and 4 neighbors (cross pattern)
    float3 e = g_inputTex.Sample(g_pointSampler, texCoord).rgb;
    float3 b = g_inputTex.Sample(g_pointSampler, texCoord + float2(0, -pixelSize.y)).rgb;
    float3 d = g_inputTex.Sample(g_pointSampler, texCoord + float2(-pixelSize.x, 0)).rgb;
    float3 f = g_inputTex.Sample(g_pointSampler, texCoord + float2(pixelSize.x, 0)).rgb;
    float3 h = g_inputTex.Sample(g_pointSampler, texCoord + float2(0, pixelSize.y)).rgb;
    
    // Calculate luma
    float3 lumaWeights = float3(0.299, 0.587, 0.114);
    float lB = dot(b, lumaWeights);
    float lD = dot(d, lumaWeights);
    float lE = dot(e, lumaWeights);
    float lF = dot(f, lumaWeights);
    float lH = dot(h, lumaWeights);
    
    // Find min and max luma in cross pattern
    float minLuma = min(min(min(lB, lD), min(lF, lH)), lE);
    float maxLuma = max(max(max(lB, lD), max(lF, lH)), lE);
    
    // Calculate sharpening amount based on local contrast
    float range = maxLuma - minLuma;
    float sharpLimit = min(minLuma, 1.0 - maxLuma) / range;
    
    // Limit sharpening to prevent ringing
    float sharpAmount = sqrt(saturate(sharpLimit)) * g_sharpness;
    
    // Calculate negative lobe weights
    float w = sharpAmount * 0.25;
    
    // Apply sharpening kernel: center weight = 1 + 4w, neighbor weight = -w
    float3 result = e * (1.0 + 4.0 * w) - (b + d + f + h) * w;
    
    // Clamp to prevent artifacts
    float3 minRGB = min(min(min(b, d), min(f, h)), e);
    float3 maxRGB = max(max(max(b, d), max(f, h)), e);
    result = clamp(result, minRGB, maxRGB);
    
    return float4(result, 1.0);
}
