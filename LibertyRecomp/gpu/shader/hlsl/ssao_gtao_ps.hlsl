// Ground Truth Ambient Occlusion (GTAO) Pixel Shader
// High-quality screen-space ambient occlusion with temporal stability
// Based on "Practical Realtime Strategies for Accurate Indirect Occlusion"

Texture2D g_depthTex : register(t0);
Texture2D g_noiseTex : register(t1);  // 4x4 blue noise texture for sample rotation

SamplerState g_pointSampler : register(s0);
SamplerState g_linearSampler : register(s1);

cbuffer SSAOConstants : register(b0)
{
    float4 g_resolution;          // (1/width, 1/height, width, height)
    float4 g_cameraParams;        // (near, far, fovY, aspectRatio)
    float4x4 g_projMatrix;        // Projection matrix for reprojection
    float4x4 g_invProjMatrix;     // Inverse projection matrix
    float g_radius;               // World-space AO radius
    float g_intensity;            // AO intensity multiplier
    float g_bias;                 // Depth comparison bias
    float g_falloffDistance;      // Distance at which AO fades out
    int g_sampleCount;            // Number of direction samples (4-12)
    int g_stepsPerSample;         // Steps per direction (2-8)
    float g_frameIndex;           // For temporal jitter
    float g_padding;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// Constants
static const float PI = 3.14159265359;
static const float TWO_PI = 6.28318530718;

// Linearize depth from depth buffer (reverse-Z compatible)
float LinearizeDepth(float depth)
{
    float near = g_cameraParams.x;
    float far = g_cameraParams.y;
    
    // Standard perspective projection depth linearization
    // For reverse-Z: depth = near / (far + depth * (near - far))
    float z = depth;
    return (near * far) / (far - z * (far - near));
}

// Reconstruct view-space position from UV and depth
float3 ReconstructViewPos(float2 uv, float depth)
{
    float linearDepth = LinearizeDepth(depth);
    
    // Convert UV to NDC [-1, 1]
    float2 ndc = uv * 2.0 - 1.0;
    ndc.y = -ndc.y;  // Flip Y for D3D
    
    // Calculate view-space position using FOV
    float tanHalfFovY = tan(g_cameraParams.z * 0.5);
    float aspectRatio = g_cameraParams.w;
    
    float3 viewPos;
    viewPos.x = ndc.x * tanHalfFovY * aspectRatio * linearDepth;
    viewPos.y = ndc.y * tanHalfFovY * linearDepth;
    viewPos.z = linearDepth;
    
    return viewPos;
}

// Reconstruct view-space normal from depth buffer derivatives
float3 ReconstructNormal(float2 uv)
{
    float2 texelSize = g_resolution.xy;
    
    // Sample depth at surrounding pixels
    float depthC = g_depthTex.SampleLevel(g_pointSampler, uv, 0).r;
    float depthL = g_depthTex.SampleLevel(g_pointSampler, uv - float2(texelSize.x, 0), 0).r;
    float depthR = g_depthTex.SampleLevel(g_pointSampler, uv + float2(texelSize.x, 0), 0).r;
    float depthU = g_depthTex.SampleLevel(g_pointSampler, uv - float2(0, texelSize.y), 0).r;
    float depthD = g_depthTex.SampleLevel(g_pointSampler, uv + float2(0, texelSize.y), 0).r;
    
    // Reconstruct positions
    float3 posC = ReconstructViewPos(uv, depthC);
    float3 posL = ReconstructViewPos(uv - float2(texelSize.x, 0), depthL);
    float3 posR = ReconstructViewPos(uv + float2(texelSize.x, 0), depthR);
    float3 posU = ReconstructViewPos(uv - float2(0, texelSize.y), depthU);
    float3 posD = ReconstructViewPos(uv + float2(0, texelSize.y), depthD);
    
    // Use the pair with smaller depth difference for better edges
    float3 ddx = (abs(posL.z - posC.z) < abs(posR.z - posC.z)) ? (posC - posL) : (posR - posC);
    float3 ddy = (abs(posU.z - posC.z) < abs(posD.z - posC.z)) ? (posC - posU) : (posD - posC);
    
    // Cross product gives normal (flipped for view-space convention)
    float3 normal = normalize(cross(ddy, ddx));
    
    return normal;
}

// Fast pseudo-random based on position
float InterleavedGradientNoise(float2 position)
{
    float3 magic = float3(0.06711056, 0.00583715, 52.9829189);
    return frac(magic.z * frac(dot(position, magic.xy)));
}

// GTAO horizon search in a single direction
float HorizonSearch(float2 uv, float3 viewPos, float3 viewNormal, float2 direction, float jitter)
{
    float2 texelSize = g_resolution.xy;
    
    // Project radius to screen space
    float screenRadius = g_radius / viewPos.z * g_resolution.z * 0.5;
    screenRadius = min(screenRadius, 64.0);  // Clamp to prevent excessive sampling
    
    float stepSize = screenRadius / float(g_stepsPerSample);
    
    // Track the maximum horizon angle
    float maxHorizon = -1.0;  // cos(angle) from tangent plane
    
    // Add jitter to starting position
    float2 startOffset = direction * stepSize * jitter;
    
    [unroll]
    for (int step = 1; step <= g_stepsPerSample; step++)
    {
        float2 sampleOffset = direction * stepSize * float(step) + startOffset;
        float2 sampleUV = uv + sampleOffset * texelSize;
        
        // Clamp to valid range
        if (any(sampleUV < 0.0) || any(sampleUV > 1.0))
            continue;
        
        float sampleDepth = g_depthTex.SampleLevel(g_pointSampler, sampleUV, 0).r;
        float3 samplePos = ReconstructViewPos(sampleUV, sampleDepth);
        
        // Vector from center to sample
        float3 horizonVec = samplePos - viewPos;
        float horizonLen = length(horizonVec);
        
        // Skip samples outside radius
        if (horizonLen > g_radius)
            continue;
        
        // Calculate horizon angle (dot product with normal gives cos(angle))
        float horizonAngle = dot(normalize(horizonVec), viewNormal);
        
        // Apply distance falloff
        float falloff = saturate(1.0 - horizonLen / g_radius);
        falloff *= falloff;  // Quadratic falloff
        
        // Track maximum horizon with falloff
        maxHorizon = max(maxHorizon, horizonAngle * falloff);
    }
    
    return maxHorizon;
}

// Main GTAO calculation
float CalculateGTAO(float2 uv, float3 viewPos, float3 viewNormal, float jitter)
{
    // Sample noise for direction rotation
    float2 noiseUV = uv * g_resolution.zw / 4.0;  // 4x4 noise texture tiling
    float rotationNoise = g_noiseTex.SampleLevel(g_pointSampler, noiseUV, 0).r;
    rotationNoise = rotationNoise * TWO_PI + jitter * PI;
    
    float ao = 0.0;
    float sampleCount = float(g_sampleCount);
    
    // Sample multiple directions around the hemisphere
    [unroll]
    for (int i = 0; i < g_sampleCount; i++)
    {
        // Direction angle with noise offset
        float angle = (float(i) + 0.5) / sampleCount * PI + rotationNoise;
        float2 direction = float2(cos(angle), sin(angle));
        
        // Search both positive and negative directions
        float horizon1 = HorizonSearch(uv, viewPos, viewNormal, direction, jitter);
        float horizon2 = HorizonSearch(uv, viewPos, viewNormal, -direction, jitter);
        
        // Average the two horizon angles
        float avgHorizon = (horizon1 + horizon2) * 0.5;
        
        // Convert horizon to occlusion (higher horizon = more occlusion)
        // Add bias to prevent self-occlusion
        float occlusion = saturate(avgHorizon - g_bias);
        
        ao += occlusion;
    }
    
    ao /= sampleCount;
    
    // Apply intensity and invert (1 = no occlusion, 0 = full occlusion)
    ao = 1.0 - saturate(ao * g_intensity);
    
    // Apply distance falloff
    float distanceFade = saturate(1.0 - viewPos.z / g_falloffDistance);
    ao = lerp(1.0, ao, distanceFade);
    
    return ao;
}

float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    
    // Sample depth
    float depth = g_depthTex.SampleLevel(g_pointSampler, texCoord, 0).r;
    
    // Early out for sky/far plane
    if (depth <= 0.0001 || depth >= 0.9999)
    {
        return float4(1.0, 1.0, 1.0, 1.0);
    }
    
    // Reconstruct position and normal
    float3 viewPos = ReconstructViewPos(texCoord, depth);
    float3 viewNormal = ReconstructNormal(texCoord);
    
    // Temporal jitter for stability
    float jitter = InterleavedGradientNoise(input.position.xy + g_frameIndex * 7.0);
    
    // Calculate GTAO
    float ao = CalculateGTAO(texCoord, viewPos, viewNormal, jitter);
    
    // Output AO value (R channel) and packed data for blur
    // G channel stores depth for edge-aware blur
    float packedDepth = saturate(LinearizeDepth(depth) / g_falloffDistance);
    
    return float4(ao, packedDepth, 0.0, 1.0);
}
