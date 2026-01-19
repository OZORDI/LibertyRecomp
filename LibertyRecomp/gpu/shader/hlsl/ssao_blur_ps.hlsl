// SSAO Bilateral Blur Pixel Shader
// Edge-aware blur that preserves sharp edges using depth comparison
// Uses separable Gaussian blur for efficiency

Texture2D g_aoTex : register(t0);      // AO texture (R=AO, G=packed depth)
Texture2D g_depthTex : register(t1);   // Full-res depth for edge detection

SamplerState g_pointSampler : register(s0);
SamplerState g_linearSampler : register(s1);

cbuffer BlurConstants : register(b0)
{
    float4 g_resolution;          // (1/width, 1/height, width, height)
    float2 g_blurDirection;       // (1,0) for horizontal, (0,1) for vertical
    float g_sharpness;            // Edge sharpness (higher = sharper edges preserved)
    float g_depthThreshold;       // Depth threshold for edge detection
    float g_nearPlane;            // Camera near plane
    float g_farPlane;             // Camera far plane
    float2 g_padding;
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// Linearize depth for comparison
float LinearizeDepth(float depth)
{
    float near = g_nearPlane;
    float far = g_farPlane;
    return (near * far) / (far - depth * (far - near));
}

// Gaussian weights for 9-tap blur
static const int BLUR_RADIUS = 4;
static const float gaussianWeights[5] = 
{
    0.227027027,  // center
    0.194594595,  // ±1
    0.121621622,  // ±2
    0.054054054,  // ±3
    0.016216216   // ±4
};

float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    float2 texelSize = g_resolution.xy;
    
    // Sample center pixel
    float4 centerSample = g_aoTex.SampleLevel(g_pointSampler, texCoord, 0);
    float centerAO = centerSample.r;
    float centerPackedDepth = centerSample.g;
    
    // Get actual depth for edge detection
    float centerDepth = g_depthTex.SampleLevel(g_pointSampler, texCoord, 0).r;
    float centerLinearDepth = LinearizeDepth(centerDepth);
    
    // Early out for sky/far plane
    if (centerDepth <= 0.0001 || centerDepth >= 0.9999)
    {
        return float4(1.0, centerPackedDepth, 0.0, 1.0);
    }
    
    float totalAO = centerAO * gaussianWeights[0];
    float totalWeight = gaussianWeights[0];
    
    // Bilateral blur in specified direction
    [unroll]
    for (int i = 1; i <= BLUR_RADIUS; i++)
    {
        float weight = gaussianWeights[i];
        
        // Positive direction sample
        {
            float2 sampleUV = texCoord + g_blurDirection * float(i) * texelSize;
            
            if (all(sampleUV >= 0.0) && all(sampleUV <= 1.0))
            {
                float4 sampleData = g_aoTex.SampleLevel(g_pointSampler, sampleUV, 0);
                float sampleAO = sampleData.r;
                
                float sampleDepth = g_depthTex.SampleLevel(g_pointSampler, sampleUV, 0).r;
                float sampleLinearDepth = LinearizeDepth(sampleDepth);
                
                // Edge-aware weight based on depth difference
                float depthDiff = abs(sampleLinearDepth - centerLinearDepth);
                float depthWeight = exp(-depthDiff * g_sharpness / centerLinearDepth);
                depthWeight = saturate(depthWeight);
                
                // Additional threshold for hard edges
                if (depthDiff > g_depthThreshold * centerLinearDepth)
                {
                    depthWeight *= 0.1;
                }
                
                float finalWeight = weight * depthWeight;
                totalAO += sampleAO * finalWeight;
                totalWeight += finalWeight;
            }
        }
        
        // Negative direction sample
        {
            float2 sampleUV = texCoord - g_blurDirection * float(i) * texelSize;
            
            if (all(sampleUV >= 0.0) && all(sampleUV <= 1.0))
            {
                float4 sampleData = g_aoTex.SampleLevel(g_pointSampler, sampleUV, 0);
                float sampleAO = sampleData.r;
                
                float sampleDepth = g_depthTex.SampleLevel(g_pointSampler, sampleUV, 0).r;
                float sampleLinearDepth = LinearizeDepth(sampleDepth);
                
                // Edge-aware weight based on depth difference
                float depthDiff = abs(sampleLinearDepth - centerLinearDepth);
                float depthWeight = exp(-depthDiff * g_sharpness / centerLinearDepth);
                depthWeight = saturate(depthWeight);
                
                // Additional threshold for hard edges
                if (depthDiff > g_depthThreshold * centerLinearDepth)
                {
                    depthWeight *= 0.1;
                }
                
                float finalWeight = weight * depthWeight;
                totalAO += sampleAO * finalWeight;
                totalWeight += finalWeight;
            }
        }
    }
    
    // Normalize
    float blurredAO = totalAO / max(totalWeight, 0.001);
    
    return float4(blurredAO, centerPackedDepth, 0.0, 1.0);
}
