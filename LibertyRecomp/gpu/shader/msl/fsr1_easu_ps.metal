// AMD FidelityFX Super Resolution 1.0 - EASU (Metal)
#include <metal_stdlib>
using namespace metal;

struct FSR1Constants {
    float4 const0; // (inputWidth, inputHeight, 1/inputWidth, 1/inputHeight)
    float4 const1; // (outputWidth, outputHeight, 1/outputWidth, 1/outputHeight)
    float4 const2; // (inputWidth/outputWidth, inputHeight/outputHeight, 0.5*scale, 0.5*scale)
    float4 const3; // Reserved
};

struct PSInput {
    float4 position [[position]];
    float2 texCoord;
};

float Luma(float3 rgb) {
    return dot(rgb, float3(0.299, 0.587, 0.114));
}

fragment float4 shaderMain(PSInput input [[stage_in]],
                           texture2d<float> inputTex [[texture(0)]],
                           sampler linearSampler [[sampler(0)]],
                           constant FSR1Constants& constants [[buffer(0)]]) {
    float2 outputUV = input.texCoord;
    float2 inputSize = constants.const0.xy;
    float2 inputRcpSize = constants.const0.zw;
    
    float2 inputPixelPos = outputUV * inputSize - 0.5;
    float2 inputPixelFrac = fract(inputPixelPos);
    float2 inputPixelBase = floor(inputPixelPos);
    
    float2 baseUV = (inputPixelBase + 0.5) * inputRcpSize;
    
    float3 b = inputTex.sample(linearSampler, baseUV + float2(0, -1) * inputRcpSize).rgb;
    float3 d = inputTex.sample(linearSampler, baseUV + float2(-1, 0) * inputRcpSize).rgb;
    float3 e = inputTex.sample(linearSampler, baseUV + float2(0, 0) * inputRcpSize).rgb;
    float3 f = inputTex.sample(linearSampler, baseUV + float2(1, 0) * inputRcpSize).rgb;
    float3 h = inputTex.sample(linearSampler, baseUV + float2(0, 1) * inputRcpSize).rgb;
    
    float lB = Luma(b);
    float lD = Luma(d);
    float lE = Luma(e);
    float lF = Luma(f);
    float lH = Luma(h);
    
    float dirX = lD - lF;
    float dirY = lB - lH;
    
    float2 dir = float2(dirX, dirY);
    float dirLength = length(dir);
    dir = dirLength > 0.0 ? dir / dirLength : float2(0, 0);
    
    float edgeStrength = saturate(dirLength * 4.0);
    
    float2 w = inputPixelFrac;
    float w00 = (1 - w.x) * (1 - w.y);
    float w10 = w.x * (1 - w.y);
    float w01 = (1 - w.x) * w.y;
    float w11 = w.x * w.y;
    
    float3 p00 = e;
    float3 p10 = f;
    float3 p01 = h;
    float3 p11 = inputTex.sample(linearSampler, baseUV + float2(1, 1) * inputRcpSize).rgb;
    
    float3 bilinear = p00 * w00 + p10 * w10 + p01 * w01 + p11 * w11;
    
    float2 edgeOffset = dir * 0.5 * inputRcpSize;
    float3 edgeSample = inputTex.sample(linearSampler, outputUV + edgeOffset).rgb;
    
    float3 result = mix(bilinear, edgeSample, edgeStrength * 0.5);
    
    return float4(result, 1.0);
}
