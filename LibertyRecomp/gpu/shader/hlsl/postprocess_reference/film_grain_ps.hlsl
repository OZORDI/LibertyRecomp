// Film Grain Pixel Shader
// Adds cinematic film grain noise to the image

Texture2D g_colorTex : register(t0);
SamplerState g_linearSampler : register(s0);

cbuffer FilmGrainConstants : register(b0)
{
    float4 g_resolution;     // (1/width, 1/height, width, height)
    float g_intensity;       // Grain intensity (0.0-1.0)
    float g_time;            // Time for animation
    float g_luminanceScale;  // How much grain affects bright vs dark areas
    float g_coloredGrain;    // 0 = monochrome, 1 = colored grain
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// High quality noise function
float hash(float2 p)
{
    float3 p3 = frac(float3(p.xyx) * 0.1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return frac((p3.x + p3.y) * p3.z);
}

float noise(float2 p)
{
    float2 i = floor(p);
    float2 f = frac(p);
    
    float a = hash(i);
    float b = hash(i + float2(1.0, 0.0));
    float c = hash(i + float2(0.0, 1.0));
    float d = hash(i + float2(1.0, 1.0));
    
    float2 u = f * f * (3.0 - 2.0 * f);
    
    return lerp(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

// Film grain with temporal variation
float3 filmGrain(float2 uv, float time, float intensity)
{
    // High frequency noise for grain texture
    float2 grainUV = uv * 512.0 + time * 100.0;
    
    float grain = noise(grainUV) * 2.0 - 1.0;
    
    // Add some variation at different frequencies
    grain += (noise(grainUV * 2.0 + 50.0) * 2.0 - 1.0) * 0.5;
    grain += (noise(grainUV * 4.0 + 100.0) * 2.0 - 1.0) * 0.25;
    
    grain *= intensity;
    
    return float3(grain, grain, grain);
}

float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    
    // Sample original color
    float4 color = g_colorTex.SampleLevel(g_linearSampler, texCoord, 0);
    
    // Calculate luminance for grain response
    float luminance = dot(color.rgb, float3(0.299, 0.587, 0.114));
    
    // Grain is more visible in midtones, less in shadows and highlights
    float grainResponse = 1.0 - abs(luminance - 0.5) * 2.0 * g_luminanceScale;
    grainResponse = saturate(grainResponse);
    
    // Pre-calculate grain UV coordinates (used by both mono and colored grain)
    float2 grainUV = texCoord * 512.0 + g_time * 100.0;
    float scaledIntensity = g_intensity * grainResponse;
    
    // Generate grain
    float3 grain;
    
    if (g_coloredGrain > 0.0)
    {
        // Colored grain mode - sample each channel with offset for color variation
        float3 coloredNoise;
        coloredNoise.r = noise(grainUV) * 2.0 - 1.0;
        coloredNoise.g = noise(grainUV + float2(100.0, 0.0)) * 2.0 - 1.0;
        coloredNoise.b = noise(grainUV + float2(0.0, 100.0)) * 2.0 - 1.0;
        
        // Add multi-octave detail
        coloredNoise.r += (noise(grainUV * 2.0 + 50.0) * 2.0 - 1.0) * 0.5;
        coloredNoise.g += (noise(grainUV * 2.0 + float2(150.0, 50.0)) * 2.0 - 1.0) * 0.5;
        coloredNoise.b += (noise(grainUV * 2.0 + float2(50.0, 150.0)) * 2.0 - 1.0) * 0.5;
        
        // Blend between mono and colored based on g_coloredGrain
        float monoGrain = (coloredNoise.r + coloredNoise.g + coloredNoise.b) / 3.0;
        grain = lerp(float3(monoGrain, monoGrain, monoGrain), coloredNoise, g_coloredGrain) * scaledIntensity;
    }
    else
    {
        // Monochrome grain mode - use filmGrain function
        grain = filmGrain(texCoord, g_time, scaledIntensity);
    }
    
    // Apply grain additively
    color.rgb += grain;
    
    return color;
}
