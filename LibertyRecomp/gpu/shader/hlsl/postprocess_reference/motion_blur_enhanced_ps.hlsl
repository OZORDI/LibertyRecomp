// Enhanced Motion Blur Pixel Shader
// Based on UnleashedRecomp's implementation
// Uses velocity buffer for per-object motion + camera motion
// Implements bidirectional sampling for proper motion blur appearance

Texture2D g_colorTex : register(t0);
Texture2D g_velocityTex : register(t1);
Texture2D g_depthTex : register(t2);
SamplerState g_linearSampler : register(s0);
SamplerState g_pointSampler : register(s1);

cbuffer MotionBlurConstants : register(b0)
{
    float4 g_resolution;     // (1/width, 1/height, width, height)
    float g_blurStrength;    // Motion blur intensity
    float g_maxSamples;      // Maximum sample count (default 64)
    float g_depthThreshold;  // Depth comparison threshold
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
    
    // Sample depth at center
    float centerDepth = g_depthTex.SampleLevel(g_pointSampler, texCoord, 0).r;
    
    // Sample velocity map
    // Velocity is encoded as: (velocityMap.xz + velocityMap.yw / 255.0) * 2.0 - 1.0
    // This gives 16-bit precision per channel
    float4 velocityMap = g_velocityTex.SampleLevel(g_pointSampler, texCoord, 0);
    float2 velocity = (velocityMap.xz + velocityMap.yw / 255.0) * 2.0 - 1.0;
    
    // Apply blur strength
    velocity *= g_blurStrength;
    
    // Calculate velocity magnitude in pixels
    float velocityPixels = length(velocity * g_resolution.zw);
    
    // Early out for minimal motion
    if (velocityPixels < 0.5)
    {
        return g_colorTex.SampleLevel(g_linearSampler, texCoord, 0);
    }
    
    // Adaptive sample count based on velocity (half in each direction for bidirectional)
    int halfSampleCount = min((int)(g_maxSamples * 0.5), max(1, (int)round(velocityPixels * 0.5)));
    int totalSamples = halfSampleCount * 2;
    
    // Calculate step size for bidirectional sampling
    float2 sampleStep = velocity / (float)totalSamples;
    
    // Start with center sample
    float3 color = g_colorTex.SampleLevel(g_linearSampler, texCoord, 0).rgb;
    float totalWeight = 1.0;
    
    // Bidirectional sampling: sample both forward and backward along velocity
    for (int i = 1; i <= halfSampleCount; i++)
    {
        float t = (float)i / (float)halfSampleCount;
        float weight = 1.0 - t * 0.5;  // Falloff weight
        
        // Forward sample (in direction of motion)
        float2 forwardCoord = texCoord + sampleStep * (float)i;
        forwardCoord = saturate(forwardCoord);
        
        float forwardDepth = g_depthTex.SampleLevel(g_pointSampler, forwardCoord, 0).r;
        
        // Depth test: use absolute difference to prevent bleeding in both directions
        if (abs(forwardDepth - centerDepth) < g_depthThreshold)
        {
            float3 forwardColor = g_colorTex.SampleLevel(g_linearSampler, forwardCoord, 0).rgb;
            color += forwardColor * weight;
            totalWeight += weight;
        }
        
        // Backward sample (opposite direction of motion)
        float2 backwardCoord = texCoord - sampleStep * (float)i;
        backwardCoord = saturate(backwardCoord);
        
        float backwardDepth = g_depthTex.SampleLevel(g_pointSampler, backwardCoord, 0).r;
        
        // Depth test: use absolute difference to prevent bleeding in both directions
        if (abs(backwardDepth - centerDepth) < g_depthThreshold)
        {
            float3 backwardColor = g_colorTex.SampleLevel(g_linearSampler, backwardCoord, 0).rgb;
            color += backwardColor * weight;
            totalWeight += weight;
        }
    }
    
    // Average accumulated samples
    color /= totalWeight;
    
    // Output with alpha for blend intensity
    // Alpha encodes blur intensity for potential compositing
    float blurIntensity = saturate(velocityPixels / 16.0) * saturate(totalWeight - 1.0);
    
    return float4(color, blurIntensity);
}
