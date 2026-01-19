// SMAA Neighborhood Blending Pixel Shader
// Final pass that blends the color buffer based on calculated blend weights

Texture2D g_colorTex : register(t0);      // Original color buffer
Texture2D g_blendTex : register(t1);      // Blend weight buffer from pass 2

SamplerState g_linearSampler : register(s0);
SamplerState g_pointSampler : register(s1);

cbuffer SMAAConstants : register(b0)
{
    float4 g_resolution;  // (1/width, 1/height, width, height)
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// SMAA neighborhood blending
// Blends current pixel with its neighbors based on the computed blend weights
float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    float2 pixelSize = g_resolution.xy;
    
    // Fetch the blending weights for the current pixel
    float4 blendWeights = g_blendTex.Sample(g_pointSampler, texCoord);
    
    // Check if there's any blending to do
    float blendSum = dot(blendWeights, float4(1.0, 1.0, 1.0, 1.0));
    
    if (blendSum < 1e-5)
    {
        // No blending needed, return original color
        return g_colorTex.Sample(g_linearSampler, texCoord);
    }
    
    // Calculate the blending offset for each direction
    // blendWeights: (top, bottom, left, right)
    float4 blendingOffset = float4(0.0, blendWeights.y, 0.0, blendWeights.w);
    float2 blendingWeight = blendWeights.xz;
    
    // Move to correct location based on weights
    float4 a;
    a.x = blendWeights.x;  // top
    a.y = blendWeights.z;  // left
    a.wz = blendWeights.yw;  // bottom, right
    
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
    float4 color = g_colorTex.Sample(g_linearSampler, texCoord);
    
    // Fetch the two neighboring pixels
    float2 offsetCoord1 = texCoord + offset * pixelSize;
    float2 offsetCoord2 = texCoord - offset * pixelSize;
    
    float4 neighbor1 = g_colorTex.Sample(g_linearSampler, offsetCoord1);
    float4 neighbor2 = g_colorTex.Sample(g_linearSampler, offsetCoord2);
    
    // Calculate the blend factor
    float weight = max(abs(offset.x), abs(offset.y));
    
    // Blend based on the maximum weight
    float4 result = lerp(color, lerp(neighbor1, neighbor2, 0.5), weight * 0.5);
    
    return result;
}
