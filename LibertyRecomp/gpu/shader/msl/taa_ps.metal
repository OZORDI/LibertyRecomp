// Temporal Anti-Aliasing Pixel Shader (Metal)
#include <metal_stdlib>
using namespace metal;

struct TAAConstants {
    float4 resolution;      // (1/width, 1/height, width, height)
    float4 jitterOffset;    // (jitterX, jitterY, prevJitterX, prevJitterY)
    float blendFactor;
    float motionScale;
    float2 padding;
};

struct PSInput {
    float4 position [[position]];
    float2 texCoord;
};

// Convert RGB to YCoCg for better color clamping
float3 RGBToYCoCg(float3 rgb) {
    return float3(
        0.25 * rgb.r + 0.5 * rgb.g + 0.25 * rgb.b,
        0.5 * rgb.r - 0.5 * rgb.b,
        -0.25 * rgb.r + 0.5 * rgb.g - 0.25 * rgb.b
    );
}

float3 YCoCgToRGB(float3 ycocg) {
    return float3(
        ycocg.x + ycocg.y - ycocg.z,
        ycocg.x + ycocg.z,
        ycocg.x - ycocg.y - ycocg.z
    );
}

float3 ClipToAABB(float3 color, float3 boxMin, float3 boxMax) {
    float3 center = 0.5 * (boxMax + boxMin);
    float3 extents = 0.5 * (boxMax - boxMin);
    
    float3 offset = color - center;
    float3 ts = abs(extents / (offset + 0.0001));
    float t = saturate(min(min(ts.x, ts.y), ts.z));
    
    return center + offset * t;
}

fragment float4 shaderMain(PSInput input [[stage_in]],
                           texture2d<float> colorTex [[texture(0)]],
                           texture2d<float> historyTex [[texture(1)]],
                           texture2d<float> motionTex [[texture(2)]],
                           texture2d<float> depthTex [[texture(3)]],
                           sampler linearSampler [[sampler(0)]],
                           sampler pointSampler [[sampler(1)]],
                           constant TAAConstants& constants [[buffer(0)]]) {
    float2 texCoord = input.texCoord;
    float2 pixelSize = constants.resolution.xy;
    
    float2 jitteredCoord = texCoord - constants.jitterOffset.xy * pixelSize;
    float3 currentColor = colorTex.sample(linearSampler, jitteredCoord).rgb;
    
    float2 motion = motionTex.sample(pointSampler, texCoord).rg * constants.motionScale;
    float2 historyCoord = texCoord - motion;
    
    bool historyValid = all(historyCoord >= 0.0) && all(historyCoord <= 1.0);
    
    if (!historyValid) {
        return float4(currentColor, 1.0);
    }
    
    float3 historyColor = historyTex.sample(linearSampler, historyCoord).rgb;
    
    float3 neighborMin = currentColor;
    float3 neighborMax = currentColor;
    
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            if (x == 0 && y == 0) continue;
            
            float2 offset = float2(x, y) * pixelSize;
            float3 neighbor = colorTex.sample(linearSampler, jitteredCoord + offset).rgb;
            
            neighborMin = min(neighborMin, neighbor);
            neighborMax = max(neighborMax, neighbor);
        }
    }
    
    float3 currentYCoCg = RGBToYCoCg(currentColor);
    float3 historyYCoCg = RGBToYCoCg(historyColor);
    float3 minYCoCg = RGBToYCoCg(neighborMin);
    float3 maxYCoCg = RGBToYCoCg(neighborMax);
    
    float3 clampedHistoryYCoCg = ClipToAABB(historyYCoCg, minYCoCg, maxYCoCg);
    float3 clampedHistory = YCoCgToRGB(clampedHistoryYCoCg);
    
    float motionLength = length(motion * constants.resolution.zw);
    float dynamicBlend = mix(constants.blendFactor, 0.2, saturate(motionLength * 0.1));
    
    float3 result = mix(clampedHistory, currentColor, dynamicBlend);
    
    return float4(result, 1.0);
}
