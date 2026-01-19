// SMAA Neighborhood Blending Pixel Shader - Metal version
// Final pass that blends the color buffer based on calculated blend weights

#include <metal_stdlib>
using namespace metal;

struct SMAAConstants
{
    float4 resolution;  // (1/width, 1/height, width, height)
};

struct PSInput
{
    float4 position [[position]];
    float2 texCoord;
};

// SMAA neighborhood blending
fragment float4 shaderMain(
    PSInput input [[stage_in]],
    texture2d<float> g_colorTex [[texture(0)]],
    texture2d<float> g_blendTex [[texture(1)]],
    sampler g_linearSampler [[sampler(0)]],
    sampler g_pointSampler [[sampler(1)]],
    constant SMAAConstants& constants [[buffer(0)]])
{
    float2 texCoord = input.texCoord;
    float2 pixelSize = constants.resolution.xy;
    
    // Fetch the blending weights for the current pixel
    float4 blendWeights = g_blendTex.sample(g_pointSampler, texCoord);
    
    // Check if there's any blending to do
    float blendSum = dot(blendWeights, float4(1.0, 1.0, 1.0, 1.0));
    
    if (blendSum < 1e-5)
    {
        // No blending needed, return original color
        return g_colorTex.sample(g_linearSampler, texCoord);
    }
    
    // Calculate the blending offset for each direction
    float4 a;
    a.x = blendWeights.x;  // top
    a.y = blendWeights.z;  // left
    a.w = blendWeights.y;  // bottom
    a.z = blendWeights.w;  // right
    
    // Horizontal and vertical blend directions
    float2 offset;
    offset.x = a.x > a.y ? a.x : -a.y;
    offset.y = a.z > a.w ? a.z : -a.w;
    
    // If there's a horizontal/vertical edge
    if (abs(offset.x) > abs(offset.y))
    {
        offset.y = 0.0;
    }
    else
    {
        offset.x = 0.0;
    }
    
    // Sample the two neighboring pixels
    float4 color = g_colorTex.sample(g_linearSampler, texCoord);
    
    // Fetch the two neighboring pixels
    float2 offsetCoord1 = texCoord + offset * pixelSize;
    float2 offsetCoord2 = texCoord - offset * pixelSize;
    
    float4 neighbor1 = g_colorTex.sample(g_linearSampler, offsetCoord1);
    float4 neighbor2 = g_colorTex.sample(g_linearSampler, offsetCoord2);
    
    // Calculate the blend factor
    float weight = max(abs(offset.x), abs(offset.y));
    
    // Blend based on the maximum weight
    float4 result = mix(color, mix(neighbor1, neighbor2, 0.5), weight * 0.5);
    
    return result;
}
