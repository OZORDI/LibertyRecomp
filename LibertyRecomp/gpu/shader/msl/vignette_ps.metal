// Vignette Post-Process Pixel Shader (Metal)
// Applies a customizable vignette effect with configurable intensity, radius, and softness

#include <metal_stdlib>
using namespace metal;

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

struct PSInput {
    float4 position [[position]];
    float2 texCoord;
};

fragment float4 shaderMain(
    PSInput input [[stage_in]],
    constant VignetteConstants& constants [[buffer(0)]],
    array<texture2d<float>, 4096> textures [[texture(0)]],
    sampler samplerState [[sampler(0)]]
) {
    // Sample the input texture
    float4 color = textures[constants.textureIndex].sample(samplerState, input.texCoord);
    
    // Early out if vignette is disabled
    if (constants.intensity <= 0.0) {
        return color;
    }
    
    // Calculate UV relative to center
    float2 uv = input.texCoord - constants.center;
    
    // Apply aspect ratio correction based on roundness
    float aspectRatio = constants.resolution.x / constants.resolution.y;
    float2 aspectCorrection = mix(float2(aspectRatio, 1.0), float2(1.0, 1.0), constants.roundness);
    uv *= aspectCorrection;
    
    // Calculate distance from center (normalized)
    float dist = length(uv) * 2.0; // *2 to normalize to 0-1 range for corners
    
    // Calculate vignette factor with smooth falloff
    float innerRadius = constants.radius;
    float outerRadius = constants.radius + constants.softness;
    float vignette = 1.0 - smoothstep(innerRadius, outerRadius, dist);
    
    // Apply intensity
    vignette = mix(1.0f, vignette, constants.intensity);
    
    // Apply vignette to color (darken edges)
    color.rgb *= vignette;
    
    return color;
}
