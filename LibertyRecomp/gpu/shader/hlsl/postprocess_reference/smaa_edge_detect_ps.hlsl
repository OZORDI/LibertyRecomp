// SMAA Edge Detection Pixel Shader
// Based on SMAA: Enhanced Subpixel Morphological Antialiasing
// http://www.iryoku.com/smaa/

#define SMAA_THRESHOLD 0.1
#define SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR 2.0

Texture2D g_colorTex : register(t0);
SamplerState g_linearSampler : register(s0);

cbuffer SMAAConstants : register(b0)
{
    float4 g_resolution; // (1/width, 1/height, width, height)
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// Luma-based edge detection
float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    float2 pixelSize = g_resolution.xy;
    
    // Calculate luma for current pixel and neighbors
    float3 weights = float3(0.2126, 0.7152, 0.0722);
    
    float L = dot(g_colorTex.Sample(g_linearSampler, texCoord).rgb, weights);
    float Lleft = dot(g_colorTex.Sample(g_linearSampler, texCoord + float2(-pixelSize.x, 0)).rgb, weights);
    float Ltop = dot(g_colorTex.Sample(g_linearSampler, texCoord + float2(0, -pixelSize.y)).rgb, weights);
    float Lright = dot(g_colorTex.Sample(g_linearSampler, texCoord + float2(pixelSize.x, 0)).rgb, weights);
    float Lbottom = dot(g_colorTex.Sample(g_linearSampler, texCoord + float2(0, pixelSize.y)).rgb, weights);
    
    // Calculate deltas
    float4 delta = abs(L - float4(Lleft, Ltop, Lright, Lbottom));
    
    // Calculate edges using threshold
    float2 edges = step(SMAA_THRESHOLD, delta.xy);
    
    // Local contrast adaptation
    float Lleftleft = dot(g_colorTex.Sample(g_linearSampler, texCoord + float2(-2.0 * pixelSize.x, 0)).rgb, weights);
    float Ltoptop = dot(g_colorTex.Sample(g_linearSampler, texCoord + float2(0, -2.0 * pixelSize.y)).rgb, weights);
    
    float maxDelta = max(max(delta.x, delta.y), max(delta.z, delta.w));
    float2 localContrast = float2(abs(Lleftleft - Lleft), abs(Ltoptop - Ltop));
    edges *= step(localContrast * SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR, maxDelta.xx);
    
    return float4(edges, 0.0, 1.0);
}
