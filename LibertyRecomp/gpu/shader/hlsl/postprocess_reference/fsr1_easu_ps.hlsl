// AMD FidelityFX Super Resolution 1.0 - EASU (Edge Adaptive Spatial Upsampling)
// Simplified implementation based on AMD FSR 1.0 specification

Texture2D g_inputTex : register(t0);
SamplerState g_linearSampler : register(s0);

cbuffer FSR1Constants : register(b0)
{
    float4 g_const0; // (inputWidth, inputHeight, 1/inputWidth, 1/inputHeight)
    float4 g_const1; // (outputWidth, outputHeight, 1/outputWidth, 1/outputHeight)
    float4 g_const2; // (inputWidth/outputWidth, inputHeight/outputHeight, 0.5*inputWidth/outputWidth, 0.5*inputHeight/outputHeight)
    float4 g_const3; // Reserved
};

struct PSInput
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

// Fast approximation of reciprocal square root
float APrxLoRcpF1(float a) { return rcp(a); }
float APrxLoRsqF1(float a) { return rsqrt(a); }

// Get luma from RGB
float Luma(float3 rgb)
{
    return dot(rgb, float3(0.299, 0.587, 0.114));
}

float4 shaderMain(PSInput input) : SV_Target
{
    float2 outputUV = input.texCoord;
    float2 inputSize = g_const0.xy;
    float2 inputRcpSize = g_const0.zw;
    float2 outputSize = g_const1.xy;
    
    // Calculate input pixel position
    float2 inputPixelPos = outputUV * inputSize - 0.5;
    float2 inputPixelFrac = frac(inputPixelPos);
    float2 inputPixelBase = floor(inputPixelPos);
    
    // Sample 12 taps in a cross pattern (optimized from 16-tap)
    float2 baseUV = (inputPixelBase + 0.5) * inputRcpSize;
    
    // Row -1
    float3 b = g_inputTex.Sample(g_linearSampler, baseUV + float2(0, -1) * inputRcpSize).rgb;
    
    // Row 0
    float3 d = g_inputTex.Sample(g_linearSampler, baseUV + float2(-1, 0) * inputRcpSize).rgb;
    float3 e = g_inputTex.Sample(g_linearSampler, baseUV + float2(0, 0) * inputRcpSize).rgb;
    float3 f = g_inputTex.Sample(g_linearSampler, baseUV + float2(1, 0) * inputRcpSize).rgb;
    
    // Row 1
    float3 h = g_inputTex.Sample(g_linearSampler, baseUV + float2(0, 1) * inputRcpSize).rgb;
    
    // Additional samples for edge detection
    float3 a = g_inputTex.Sample(g_linearSampler, baseUV + float2(-1, -1) * inputRcpSize).rgb;
    float3 c = g_inputTex.Sample(g_linearSampler, baseUV + float2(1, -1) * inputRcpSize).rgb;
    float3 g = g_inputTex.Sample(g_linearSampler, baseUV + float2(-1, 1) * inputRcpSize).rgb;
    float3 i = g_inputTex.Sample(g_linearSampler, baseUV + float2(1, 1) * inputRcpSize).rgb;
    
    // Calculate luma for edge detection
    float lB = Luma(b);
    float lD = Luma(d);
    float lE = Luma(e);
    float lF = Luma(f);
    float lH = Luma(h);
    
    // Directional edge detection
    float dirX = lD - lF;
    float dirY = lB - lH;
    
    // Calculate edge direction and length
    float2 dir = float2(dirX, dirY);
    float dirLength = length(dir);
    dir = dirLength > 0.0 ? dir / dirLength : float2(0, 0);
    
    // Edge-aware interpolation weights
    float edgeStrength = saturate(dirLength * 4.0);
    
    // Bilinear base weights
    float2 w = inputPixelFrac;
    float w00 = (1 - w.x) * (1 - w.y);
    float w10 = w.x * (1 - w.y);
    float w01 = (1 - w.x) * w.y;
    float w11 = w.x * w.y;
    
    // Sample the four corners
    float3 p00 = e;
    float3 p10 = f;
    float3 p01 = h;
    float3 p11 = g_inputTex.Sample(g_linearSampler, baseUV + float2(1, 1) * inputRcpSize).rgb;
    
    // Apply edge-aware modification
    float3 bilinear = p00 * w00 + p10 * w10 + p01 * w01 + p11 * w11;
    
    // Edge-directed sample
    float2 edgeOffset = dir * 0.5 * inputRcpSize;
    float3 edgeSample = g_inputTex.Sample(g_linearSampler, outputUV + edgeOffset).rgb;
    
    // Blend based on edge strength
    float3 result = lerp(bilinear, edgeSample, edgeStrength * 0.5);
    
    return float4(result, 1.0);
}
