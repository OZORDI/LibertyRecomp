// Camera Motion Blur Pixel Shader
// Reprojection-based motion blur using previous frame's view-projection matrix
// Calculates per-pixel velocity from depth buffer and camera movement
// Uses bidirectional sampling for proper motion blur appearance

Texture2D g_colorTex : register(t0);
Texture2D g_depthTex : register(t1);
SamplerState g_linearSampler : register(s0);
SamplerState g_pointSampler : register(s1);

cbuffer MotionBlurConstants : register(b0)
{
    float4x4 g_invViewProj;      // Current frame inverse view-projection
    float4x4 g_prevViewProj;     // Previous frame view-projection
    float4 g_resolution;         // (1/width, 1/height, width, height)
    float g_blurStrength;        // Motion blur intensity multiplier
    float g_maxBlurRadius;       // Maximum blur radius in pixels (default 32)
    float g_depthThreshold;      // Depth discontinuity threshold
    float g_padding;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// Reconstruct world position from depth
// Uses consistent coordinate system: UV (0,0) = top-left, clip space Y is flipped
float3 ReconstructWorldPos(float2 uv, float depth)
{
    // Convert UV to NDC: UV (0,0)-(1,1) -> NDC (-1,-1)-(1,1)
    // Note: In D3D, NDC Y+ is up, but UV Y+ is down, so we flip
    float2 ndc = uv * 2.0 - 1.0;
    ndc.y = -ndc.y;  // Flip Y: UV space to NDC space
    
    float4 clipPos = float4(ndc, depth, 1.0);
    float4 worldPos = mul(g_invViewProj, clipPos);
    
    // Perspective divide
    return worldPos.xyz / worldPos.w;
}

// Calculate velocity from reprojection
float2 CalculateVelocity(float2 uv, float depth)
{
    // Reconstruct world position
    float3 worldPos = ReconstructWorldPos(uv, depth);
    
    // Project to previous frame's clip space
    float4 prevClipPos = mul(g_prevViewProj, float4(worldPos, 1.0));
    
    // Perspective divide to get NDC
    float2 prevNDC = prevClipPos.xy / prevClipPos.w;
    
    // Convert NDC back to UV space (flip Y back)
    float2 prevUV;
    prevUV.x = prevNDC.x * 0.5 + 0.5;
    prevUV.y = -prevNDC.y * 0.5 + 0.5;  // Flip Y: NDC space to UV space
    
    // Velocity is current UV - previous UV (where the pixel came from)
    return uv - prevUV;
}

float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    
    // Sample depth
    float depth = g_depthTex.SampleLevel(g_pointSampler, texCoord, 0).r;
    
    // Skip sky (far plane)
    if (depth >= 0.9999)
    {
        return g_colorTex.SampleLevel(g_linearSampler, texCoord, 0);
    }
    
    // Calculate velocity from camera motion
    float2 velocity = CalculateVelocity(texCoord, depth);
    velocity *= g_blurStrength;
    
    // Calculate velocity magnitude in pixels
    float velocityLength = length(velocity * g_resolution.zw);
    
    // Clamp velocity to max blur radius
    if (velocityLength > g_maxBlurRadius)
    {
        velocity *= g_maxBlurRadius / velocityLength;
        velocityLength = g_maxBlurRadius;
    }
    
    // Early out if no significant motion
    if (velocityLength < 0.5)
    {
        return g_colorTex.SampleLevel(g_linearSampler, texCoord, 0);
    }
    
    // Adaptive sample count based on velocity length (half in each direction)
    int halfSampleCount = clamp((int)(velocityLength * 0.5), 2, 16);
    int totalSamples = halfSampleCount * 2;
    
    // Calculate step size for bidirectional sampling
    float2 sampleStep = velocity / (float)totalSamples;
    
    // Start with center sample
    float3 color = g_colorTex.SampleLevel(g_linearSampler, texCoord, 0).rgb;
    float totalWeight = 1.0;
    float centerDepth = depth;
    
    // Bidirectional sampling: sample both forward and backward along velocity
    for (int i = 1; i <= halfSampleCount; i++)
    {
        float t = (float)i / (float)halfSampleCount;
        float weight = 1.0 - t * 0.5;  // Falloff weight
        
        // Forward sample (in direction of motion)
        float2 forwardCoord = texCoord + sampleStep * (float)i;
        forwardCoord = saturate(forwardCoord);
        
        float forwardDepth = g_depthTex.SampleLevel(g_pointSampler, forwardCoord, 0).r;
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
        if (abs(backwardDepth - centerDepth) < g_depthThreshold)
        {
            float3 backwardColor = g_colorTex.SampleLevel(g_linearSampler, backwardCoord, 0).rgb;
            color += backwardColor * weight;
            totalWeight += weight;
        }
    }
    
    color /= totalWeight;
    
    return float4(color, 1.0);
}
