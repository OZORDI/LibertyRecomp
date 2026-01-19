// Vignette Post-Process Pixel Shader
// Applies a customizable vignette effect with configurable intensity, radius, and softness

struct VignetteConstants {
    float intensity;      // Overall vignette strength (0 = off, 1 = full)
    float radius;         // Inner radius where vignette starts (0-1, relative to screen)
    float softness;       // Falloff softness (higher = softer edge)
    float roundness;      // Aspect ratio correction (1 = circular, 0 = follows screen aspect)
    float2 center;        // Vignette center (usually 0.5, 0.5)
    float2 resolution;    // Screen resolution for aspect correction
    uint textureIndex;    // Input texture descriptor index
    uint padding[3];
};

[[vk::push_constant]] VignetteConstants constants;

[[vk::binding(0, 0)]] Texture2D<float4> textures[] : register(t0, space0);
[[vk::binding(0, 1)]] SamplerState samplerState : register(s0, space1);

struct PSInput {
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

float4 main(PSInput input) : SV_Target {
    // Sample the input texture
    float4 color = textures[constants.textureIndex].Sample(samplerState, input.texCoord);
    
    // Early out if vignette is disabled
    if (constants.intensity <= 0.0) {
        return color;
    }
    
    // Calculate UV relative to center
    float2 uv = input.texCoord - constants.center;
    
    // Apply aspect ratio correction based on roundness
    float aspectRatio = constants.resolution.x / constants.resolution.y;
    float2 aspectCorrection = lerp(float2(aspectRatio, 1.0), float2(1.0, 1.0), constants.roundness);
    uv *= aspectCorrection;
    
    // Calculate distance from center (normalized)
    float dist = length(uv) * 2.0; // *2 to normalize to 0-1 range for corners
    
    // Calculate vignette factor with smooth falloff
    // Using smoothstep for natural-looking falloff
    float innerRadius = constants.radius;
    float outerRadius = constants.radius + constants.softness;
    float vignette = 1.0 - smoothstep(innerRadius, outerRadius, dist);
    
    // Apply intensity
    vignette = lerp(1.0, vignette, constants.intensity);
    
    // Apply vignette to color (darken edges)
    color.rgb *= vignette;
    
    return color;
}
