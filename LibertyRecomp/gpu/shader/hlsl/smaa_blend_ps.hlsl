// SMAA Neighborhood Blending Pixel Shader
// Final pass that applies the calculated blend weights

Texture2D g_colorTex : register(t0);
Texture2D g_blendTex : register(t1);
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

float4 shaderMain(PSInput input) : SV_Target
{
    float2 texCoord = input.texCoord;
    float2 pixelSize = g_resolution.xy;
    
    // Fetch blending weights
    float4 blend = g_blendTex.Sample(g_linearSampler, texCoord);
    
    // Early out if no blending needed
    if (dot(blend, float4(1, 1, 1, 1)) < 1e-5)
    {
        return g_colorTex.Sample(g_linearSampler, texCoord);
    }
    
    // Calculate blend offsets
    float4 offset;
    bool useRG = (blend.r + blend.g) > (blend.b + blend.a);
    offset.xy = useRG ? float2(blend.r, -blend.g) : float2(-blend.b, blend.a);
    offset.zw = useRG ? float2(blend.r, blend.g) : float2(blend.b, blend.a);
    
    // Normalized blend factor
    float blendFactor = max(offset.x, offset.y);
    
    if (blendFactor > 0.0)
    {
        // Fetch neighboring colors and blend
        float2 blendOffset = offset.xy * pixelSize;
        float4 colorN = g_colorTex.Sample(g_linearSampler, texCoord + blendOffset);
        float4 colorP = g_colorTex.Sample(g_linearSampler, texCoord - blendOffset);
        float4 colorC = g_colorTex.Sample(g_linearSampler, texCoord);
        
        return lerp(colorC, (colorN + colorP) * 0.5, blendFactor);
    }
    
    return g_colorTex.Sample(g_linearSampler, texCoord);
}
