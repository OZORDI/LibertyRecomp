// AMD FidelityFX Super Resolution 1.0 - RCAS (Metal)
// Robust Contrast Adaptive Sharpening
#include <metal_stdlib>
using namespace metal;

struct RCASConstants {
    float4 const0; // (sharpness, 0, 0, 0)
    float4 resolution; // (1/width, 1/height, width, height)
};

struct PSInput {
    float4 position [[position]];
    float2 texCoord;
};

fragment float4 shaderMain(PSInput input [[stage_in]],
                           texture2d<float> inputTex [[texture(0)]],
                           sampler pointSampler [[sampler(0)]],
                           constant RCASConstants& constants [[buffer(0)]]) {
    float2 texCoord = input.texCoord;
    float2 pixelSize = constants.resolution.xy;
    float sharpness = constants.const0.x;
    
    // Sample center and 4 neighbors
    float3 e = inputTex.sample(pointSampler, texCoord).rgb;
    float3 b = inputTex.sample(pointSampler, texCoord + float2(0, -pixelSize.y)).rgb;
    float3 d = inputTex.sample(pointSampler, texCoord + float2(-pixelSize.x, 0)).rgb;
    float3 f = inputTex.sample(pointSampler, texCoord + float2(pixelSize.x, 0)).rgb;
    float3 h = inputTex.sample(pointSampler, texCoord + float2(0, pixelSize.y)).rgb;
    
    // Luma for edge detection
    float3 lumaWeights = float3(0.299, 0.587, 0.114);
    float lB = dot(b, lumaWeights);
    float lD = dot(d, lumaWeights);
    float lE = dot(e, lumaWeights);
    float lF = dot(f, lumaWeights);
    float lH = dot(h, lumaWeights);
    
    // Compute min and max luma
    float minLuma = min(min(min(lB, lD), min(lE, lF)), lH);
    float maxLuma = max(max(max(lB, lD), max(lE, lF)), lH);
    
    // Compute sharpening amount based on local contrast
    float contrast = maxLuma - minLuma;
    float sharpenAmount = sqrt(saturate(1.0 - contrast)) * sharpness;
    
    // Compute sharpening kernel weights
    float w = sharpenAmount * 0.25;
    
    // Apply sharpening: enhance center, reduce neighbors
    float3 result = e + (e * 4.0 - b - d - f - h) * w;
    
    // Clamp to prevent ringing artifacts
    float3 minColor = min(min(min(b, d), min(e, f)), h);
    float3 maxColor = max(max(max(b, d), max(e, f)), h);
    result = clamp(result, minColor, maxColor);
    
    return float4(result, 1.0);
}
