// Temporal Anti-Aliasing Pixel Shader
// Blends current frame with history buffer using motion vectors and neighborhood clamping

Texture2D g_colorTex : register(t0);      // Current frame color
Texture2D g_historyTex : register(t1);    // Previous frame color
Texture2D g_motionTex : register(t2);     // Motion vectors (RG = velocity)
Texture2D g_depthTex : register(t3);      // Depth buffer

SamplerState g_linearSampler : register(s0);
SamplerState g_pointSampler : register(s1);

cbuffer TAAConstants : register(b0)
{
    float4 g_resolution;      // (1/width, 1/height, width, height)
    float4 g_jitterOffset;    // (jitterX, jitterY, prevJitterX, prevJitterY)
    float g_blendFactor;      // Blend factor (0.05 - 0.1 typical)
    float g_motionScale;      // Motion vector scale
    float2 g_padding;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// Convert RGB to YCoCg for better color clamping
float3 RGBToYCoCg(float3 rgb)
{
    return float3(
        0.25 * rgb.r + 0.5 * rgb.g + 0.25 * rgb.b,
        0.5 * rgb.r - 0.5 * rgb.b,
        -0.25 * rgb.r + 0.5 * rgb.g - 0.25 * rgb.b
    );
}

float3 YCoCgToRGB(float3 ycocg)
{
    return float3(
        ycocg.x + ycocg.y - ycocg.z,
        ycocg.x + ycocg.z,
        ycocg.x - ycocg.y - ycocg.z
    );
}

// Neighborhood color clamping to reduce ghosting
float3 ClipToAABB(float3 color, float3 boxMin, float3 boxMax)
{
    float3 center = 0.5 * (boxMax + boxMin);
    float3 extents = 0.5 * (boxMax - boxMin);
    
    float3 offset = color - center;
    float3 ts = abs(extents / (offset + 0.0001));
    float t = saturate(min(min(ts.x, ts.y), ts.z));
    
    return center + offset * t;
}

float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    float2 pixelSize = g_resolution.xy;
    
    // Sample current frame with jitter removed
    float2 jitteredCoord = texCoord - g_jitterOffset.xy * pixelSize;
    float3 currentColor = g_colorTex.Sample(g_linearSampler, jitteredCoord).rgb;
    
    // Get motion vector and calculate history UV
    float2 motion = g_motionTex.Sample(g_pointSampler, texCoord).rg * g_motionScale;
    float2 historyCoord = texCoord - motion;
    
    // Check if history is valid (inside screen bounds)
    bool historyValid = all(historyCoord >= 0.0) && all(historyCoord <= 1.0);
    
    if (!historyValid)
    {
        return float4(currentColor, 1.0);
    }
    
    // Sample history
    float3 historyColor = g_historyTex.Sample(g_linearSampler, historyCoord).rgb;
    
    // Sample 3x3 neighborhood for color clamping
    float3 neighborMin = currentColor;
    float3 neighborMax = currentColor;
    
    [unroll]
    for (int y = -1; y <= 1; y++)
    {
        [unroll]
        for (int x = -1; x <= 1; x++)
        {
            if (x == 0 && y == 0) continue;
            
            float2 offset = float2(x, y) * pixelSize;
            float3 neighbor = g_colorTex.Sample(g_linearSampler, jitteredCoord + offset).rgb;
            
            neighborMin = min(neighborMin, neighbor);
            neighborMax = max(neighborMax, neighbor);
        }
    }
    
    // Convert to YCoCg for clamping
    float3 currentYCoCg = RGBToYCoCg(currentColor);
    float3 historyYCoCg = RGBToYCoCg(historyColor);
    float3 minYCoCg = RGBToYCoCg(neighborMin);
    float3 maxYCoCg = RGBToYCoCg(neighborMax);
    
    // Clamp history to neighborhood bounds
    float3 clampedHistoryYCoCg = ClipToAABB(historyYCoCg, minYCoCg, maxYCoCg);
    float3 clampedHistory = YCoCgToRGB(clampedHistoryYCoCg);
    
    // Calculate blend factor based on motion
    float motionLength = length(motion * g_resolution.zw);
    float dynamicBlend = lerp(g_blendFactor, 0.2, saturate(motionLength * 0.1));
    
    // Blend current and history
    float3 result = lerp(clampedHistory, currentColor, dynamicBlend);
    
    return float4(result, 1.0);
}
