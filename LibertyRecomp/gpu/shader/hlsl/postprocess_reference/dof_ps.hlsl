// Depth of Field Pixel Shader
// Optimized DOF with spiral sampling pattern for bokeh effect
// Uses fewer samples than full disc while maintaining quality

Texture2D g_colorTex : register(t0);
Texture2D g_depthTex : register(t1);
SamplerState g_linearSampler : register(s0);
SamplerState g_pointSampler : register(s1);

cbuffer DOFConstants : register(b0)
{
    float4 g_resolution;      // (1/width, 1/height, width, height)
    float g_focusDistance;    // Distance to focus plane
    float g_apertureSize;     // Aperture size (affects blur amount)
    float g_nearPlane;        // Camera near plane
    float g_farPlane;         // Camera far plane
    int g_kernelSize;         // Quality level (3=16 samples, 5=32, 7=48, 9=64)
    float g_maxBlur;          // Maximum blur radius in pixels
    float2 g_padding;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// Golden angle for spiral sampling (produces nice bokeh distribution)
static const float GOLDEN_ANGLE = 2.39996323;

// Convert depth buffer value to linear depth
float LinearizeDepth(float depth)
{
    float z = depth;
    // Guard against division by zero when depth approaches limits
    float denominator = g_farPlane + g_nearPlane - z * (g_farPlane - g_nearPlane);
    denominator = max(abs(denominator), 0.0001) * sign(denominator + 0.0001);
    return (2.0 * g_nearPlane * g_farPlane) / denominator;
}

// Calculate circle of confusion (blur amount) based on depth
float CalculateCoC(float depth)
{
    float linearDepth = LinearizeDepth(depth);
    
    // Guard against division by zero for very close objects
    linearDepth = max(linearDepth, 0.001);
    
    // CoC formula based on thin lens model
    float coc = g_apertureSize * abs(linearDepth - g_focusDistance) / linearDepth;
    
    // Clamp to max blur
    return min(coc * g_resolution.z, g_maxBlur);
}

// Optimized spiral bokeh blur using golden angle distribution
// This gives good coverage with far fewer samples than a full disc
float4 SpiralBokehBlur(float2 uv, float coc)
{
    // Early out for sharp pixels
    if (coc < 0.5)
    {
        return g_colorTex.SampleLevel(g_linearSampler, uv, 0);
    }
    
    // Determine sample count based on quality setting
    // 3->16, 5->32, 7->48, 9->64 samples
    int sampleCount = g_kernelSize * 8 - 8;
    sampleCount = clamp(sampleCount, 16, 64);
    
    float4 color = float4(0, 0, 0, 0);
    float totalWeight = 0.0;
    
    float2 pixelSize = g_resolution.xy;
    float centerDepth = g_depthTex.SampleLevel(g_pointSampler, uv, 0).r;
    float centerCoC = coc;
    
    // Start with center sample
    float4 centerColor = g_colorTex.SampleLevel(g_linearSampler, uv, 0);
    color = centerColor;
    totalWeight = 1.0;
    
    // Spiral sampling pattern using golden angle
    for (int i = 1; i < sampleCount; i++)
    {
        // Golden angle spiral distribution
        float t = (float)i / (float)sampleCount;
        float radius = sqrt(t) * coc;  // Square root for even area distribution
        float angle = (float)i * GOLDEN_ANGLE;
        
        float2 offset = float2(cos(angle), sin(angle)) * radius * pixelSize;
        float2 sampleUV = uv + offset;
        
        // Clamp to valid UV range
        sampleUV = saturate(sampleUV);
        
        // Sample depth for occlusion handling
        float sampleDepth = g_depthTex.SampleLevel(g_pointSampler, sampleUV, 0).r;
        float sampleCoC = CalculateCoC(sampleDepth);
        
        // Weight based on distance from center (simple falloff)
        float distWeight = 1.0 - t * 0.5;
        
        // Prevent sharp foreground objects from bleeding into blurry background
        // If sample is sharper (smaller CoC), reduce its contribution
        float cocWeight = saturate(sampleCoC / (centerCoC + 0.001));
        
        // Also prevent blurry background from bleeding onto sharp foreground
        // by checking if the sample is behind the center pixel
        float depthWeight = 1.0;
        if (sampleCoC > centerCoC * 1.5 && sampleDepth > centerDepth)
        {
            depthWeight = 0.3;  // Reduce background bleeding
        }
        
        float weight = distWeight * cocWeight * depthWeight;
        
        float4 sampleColor = g_colorTex.SampleLevel(g_linearSampler, sampleUV, 0);
        
        color += sampleColor * weight;
        totalWeight += weight;
    }
    
    return color / max(totalWeight, 0.001);
}

float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    
    // Sample depth at center
    float depth = g_depthTex.SampleLevel(g_pointSampler, texCoord, 0).r;
    
    // Calculate circle of confusion
    float coc = CalculateCoC(depth);
    
    // Apply optimized spiral bokeh blur
    return SpiralBokehBlur(texCoord, coc);
}
