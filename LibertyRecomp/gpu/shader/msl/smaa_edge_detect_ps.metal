// SMAA Edge Detection Pixel Shader (Metal)
#include <metal_stdlib>
using namespace metal;

#define SMAA_THRESHOLD 0.1
#define SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR 2.0

struct SMAAConstants {
    float4 resolution; // (1/width, 1/height, width, height)
};

struct PSInput {
    float4 position [[position]];
    float2 texCoord;
};

fragment float4 shaderMain(PSInput input [[stage_in]],
                           texture2d<float> colorTex [[texture(0)]],
                           sampler linearSampler [[sampler(0)]],
                           constant SMAAConstants& constants [[buffer(0)]]) {
    float2 texCoord = input.texCoord;
    float2 pixelSize = constants.resolution.xy;
    
    float3 weights = float3(0.2126, 0.7152, 0.0722);
    
    float L = dot(colorTex.sample(linearSampler, texCoord).rgb, weights);
    float Lleft = dot(colorTex.sample(linearSampler, texCoord + float2(-pixelSize.x, 0)).rgb, weights);
    float Ltop = dot(colorTex.sample(linearSampler, texCoord + float2(0, -pixelSize.y)).rgb, weights);
    float Lright = dot(colorTex.sample(linearSampler, texCoord + float2(pixelSize.x, 0)).rgb, weights);
    float Lbottom = dot(colorTex.sample(linearSampler, texCoord + float2(0, pixelSize.y)).rgb, weights);
    
    float4 delta = abs(L - float4(Lleft, Ltop, Lright, Lbottom));
    
    float2 edges = step(SMAA_THRESHOLD, delta.xy);
    
    float Lleftleft = dot(colorTex.sample(linearSampler, texCoord + float2(-2.0 * pixelSize.x, 0)).rgb, weights);
    float Ltoptop = dot(colorTex.sample(linearSampler, texCoord + float2(0, -2.0 * pixelSize.y)).rgb, weights);
    
    float maxDelta = max(max(delta.x, delta.y), max(delta.z, delta.w));
    float2 localContrast = float2(abs(Lleftleft - Lleft), abs(Ltoptop - Ltop));
    edges *= step(localContrast * SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR, float2(maxDelta));
    
    return float4(edges, 0.0, 1.0);
}
