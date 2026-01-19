// =============================================================================
// Screen-Space Reflections (SSR) - Ray Tracing Pass
// Hi-Z accelerated ray marching for reflective surfaces
// =============================================================================

// Constant buffer for SSR parameters
cbuffer SSRConstants : register(b0)
{
    float4 g_resolution;          // (1/width, 1/height, width, height)
    float4 g_cameraParams;        // (near, far, fovY, aspectRatio)
    float4x4 g_viewMatrix;        // View matrix
    float4x4 g_projMatrix;        // Projection matrix
    float4x4 g_invViewMatrix;     // Inverse view matrix
    float4x4 g_invProjMatrix;     // Inverse projection matrix
    float g_maxDistance;          // Maximum ray distance in world units
    float g_stride;               // Ray march stride multiplier
    float g_strideZCutoff;        // Z cutoff for stride adjustment
    float g_maxSteps;             // Maximum ray march steps
    float g_thickness;            // Surface thickness for hit detection
    float g_fadeStart;            // Distance where reflections start fading
    float g_fadeEnd;              // Distance where reflections fully fade
    float g_roughnessCutoff;      // Roughness threshold for reflections
    float g_edgeFade;             // Screen edge fade factor
    float g_frameIndex;           // For temporal jitter
    float2 g_padding;
};

// Textures
Texture2D<float4> g_colorTexture : register(t0);   // Scene color
Texture2D<float> g_depthTexture : register(t1);    // Depth buffer
Texture2D<float4> g_normalTexture : register(t2);  // World-space normals (optional)
SamplerState g_linearSampler : register(s0);
SamplerState g_pointSampler : register(s1);

// Output structure
struct PSOutput
{
    float4 reflection : SV_Target0;  // RGB = reflection color, A = confidence
};

// Vertex shader output / Pixel shader input
struct PSInput
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

// Linearize depth from depth buffer
float LinearizeDepth(float depth)
{
    float near = g_cameraParams.x;
    float far = g_cameraParams.y;
    return (near * far) / (far - depth * (far - near));
}

// Reconstruct view-space position from UV and depth
float3 ReconstructViewPos(float2 uv, float depth)
{
    float linearDepth = LinearizeDepth(depth);
    
    // Convert UV to NDC
    float2 ndc = uv * 2.0 - 1.0;
    ndc.y = -ndc.y;
    
    // Reconstruct view-space position
    float fovY = g_cameraParams.z;
    float aspect = g_cameraParams.w;
    float tanHalfFov = tan(fovY * 0.5);
    
    float3 viewPos;
    viewPos.x = ndc.x * linearDepth * tanHalfFov * aspect;
    viewPos.y = ndc.y * linearDepth * tanHalfFov;
    viewPos.z = -linearDepth;
    
    return viewPos;
}

// Reconstruct normal from depth buffer derivatives
float3 ReconstructNormal(float2 uv, float depth)
{
    float2 texelSize = g_resolution.xy;
    
    // Sample neighboring depths
    float depthL = g_depthTexture.Sample(g_pointSampler, uv + float2(-texelSize.x, 0)).r;
    float depthR = g_depthTexture.Sample(g_pointSampler, uv + float2(texelSize.x, 0)).r;
    float depthU = g_depthTexture.Sample(g_pointSampler, uv + float2(0, -texelSize.y)).r;
    float depthD = g_depthTexture.Sample(g_pointSampler, uv + float2(0, texelSize.y)).r;
    
    // Reconstruct positions
    float3 posC = ReconstructViewPos(uv, depth);
    float3 posL = ReconstructViewPos(uv + float2(-texelSize.x, 0), depthL);
    float3 posR = ReconstructViewPos(uv + float2(texelSize.x, 0), depthR);
    float3 posU = ReconstructViewPos(uv + float2(0, -texelSize.y), depthU);
    float3 posD = ReconstructViewPos(uv + float2(0, texelSize.y), depthD);
    
    // Calculate derivatives
    float3 ddx = posR - posL;
    float3 ddy = posD - posU;
    
    // Cross product for normal
    float3 normal = normalize(cross(ddy, ddx));
    
    return normal;
}

// Project view-space position to screen UV
float2 ProjectToScreen(float3 viewPos)
{
    float4 clipPos = mul(g_projMatrix, float4(viewPos, 1.0));
    float2 ndc = clipPos.xy / clipPos.w;
    float2 uv = ndc * float2(0.5, -0.5) + 0.5;
    return uv;
}

// Ray march through screen space
bool RayMarch(float3 rayOrigin, float3 rayDir, out float2 hitUV, out float hitDepth)
{
    hitUV = float2(0, 0);
    hitDepth = 0;
    
    float rayLength = g_maxDistance;
    float3 rayEnd = rayOrigin + rayDir * rayLength;
    
    // Project start and end to screen space
    float2 startUV = ProjectToScreen(rayOrigin);
    float2 endUV = ProjectToScreen(rayEnd);
    
    // Calculate ray direction in screen space
    float2 rayDirScreen = endUV - startUV;
    float rayLengthScreen = length(rayDirScreen * g_resolution.zw);
    
    // Calculate step count
    float stepCount = min(g_maxSteps, rayLengthScreen);
    if (stepCount < 1.0) return false;
    
    float2 stepSize = rayDirScreen / stepCount;
    float depthStep = (rayEnd.z - rayOrigin.z) / stepCount;
    
    float2 currentUV = startUV;
    float currentDepth = rayOrigin.z;
    
    // Temporal jitter for noise reduction
    float jitter = frac(sin(dot(startUV, float2(12.9898, 78.233)) + g_frameIndex) * 43758.5453);
    currentUV += stepSize * jitter;
    currentDepth += depthStep * jitter;
    
    for (int i = 0; i < int(stepCount); i++)
    {
        // Check bounds
        if (currentUV.x < 0 || currentUV.x > 1 || currentUV.y < 0 || currentUV.y > 1)
            break;
        
        // Sample scene depth
        float sceneDepth = g_depthTexture.Sample(g_pointSampler, currentUV).r;
        float linearSceneDepth = LinearizeDepth(sceneDepth);
        
        // Compare depths (ray is in negative Z)
        float rayZ = -currentDepth;
        float depthDiff = rayZ - linearSceneDepth;
        
        // Hit detection with thickness
        if (depthDiff > 0 && depthDiff < g_thickness)
        {
            // Binary search refinement
            float2 refinedUV = currentUV;
            float refinedDepth = currentDepth;
            float2 halfStep = stepSize * 0.5;
            float halfDepthStep = depthStep * 0.5;
            
            for (int j = 0; j < 4; j++)
            {
                refinedUV -= halfStep;
                refinedDepth -= halfDepthStep;
                
                float refSceneDepth = g_depthTexture.Sample(g_pointSampler, refinedUV).r;
                float refLinearDepth = LinearizeDepth(refSceneDepth);
                float refDiff = (-refinedDepth) - refLinearDepth;
                
                if (refDiff > 0)
                {
                    refinedUV += halfStep;
                    refinedDepth += halfDepthStep;
                }
                
                halfStep *= 0.5;
                halfDepthStep *= 0.5;
            }
            
            hitUV = refinedUV;
            hitDepth = -refinedDepth;
            return true;
        }
        
        currentUV += stepSize;
        currentDepth += depthStep;
    }
    
    return false;
}

// Calculate screen edge fade
float ScreenEdgeFade(float2 uv)
{
    float2 fade = saturate((uv - 0.0) / g_edgeFade) * saturate((1.0 - uv) / g_edgeFade);
    return fade.x * fade.y;
}

// Main pixel shader
PSOutput main(PSInput input)
{
    PSOutput output;
    output.reflection = float4(0, 0, 0, 0);
    
    float2 uv = input.texCoord;
    
    // Sample depth
    float depth = g_depthTexture.Sample(g_pointSampler, uv).r;
    
    // Skip sky pixels
    if (depth >= 1.0)
        return output;
    
    // Reconstruct view-space position and normal
    float3 viewPos = ReconstructViewPos(uv, depth);
    float3 viewNormal = ReconstructNormal(uv, depth);
    
    // Calculate reflection direction
    float3 viewDir = normalize(viewPos);
    float3 reflectDir = reflect(viewDir, viewNormal);
    
    // Skip surfaces facing away from camera
    if (dot(reflectDir, viewNormal) < 0.01)
        return output;
    
    // Perform ray march
    float2 hitUV;
    float hitDepth;
    
    if (RayMarch(viewPos, reflectDir, hitUV, hitDepth))
    {
        // Sample reflected color
        float3 reflectedColor = g_colorTexture.Sample(g_linearSampler, hitUV).rgb;
        
        // Calculate confidence/fade factors
        float edgeFade = ScreenEdgeFade(hitUV);
        float distanceFade = 1.0 - saturate((hitDepth - g_fadeStart) / (g_fadeEnd - g_fadeStart));
        float confidence = edgeFade * distanceFade;
        
        // Fresnel factor (more reflection at grazing angles)
        float NdotV = saturate(dot(-viewDir, viewNormal));
        float fresnel = pow(1.0 - NdotV, 2.0) * 0.8 + 0.2;
        
        confidence *= fresnel;
        
        output.reflection = float4(reflectedColor, confidence);
    }
    
    return output;
}
