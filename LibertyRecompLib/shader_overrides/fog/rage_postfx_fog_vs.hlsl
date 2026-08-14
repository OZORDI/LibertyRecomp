// Exact rage_postfx_vs0 interface with one additional, otherwise-unused
// TEXCOORD2 carrying a normalized world-space view ray for the fog compositor.

struct PushConstants {
  uint64_t VertexShaderConstants;
  uint64_t PixelShaderConstants;
  uint64_t SharedConstants;
};

[[vk::push_constant]] ConstantBuffer<PushConstants> g_PushConstants;

struct VertexShaderInput {
  [[vk::location(0)]] float4 iPosition0 : POSITION0;
  [[vk::location(13)]] float4 iTexCoord0 : TEXCOORD0;
};

struct Interpolators {
  precise float4 oPos : SV_Position;
  float4 oTexCoord0 : TEXCOORD0;
  float4 oTexCoord1 : TEXCOORD1;
  float4 oTexCoord2 : TEXCOORD2;
  float4 oTexCoord3 : TEXCOORD3;
  float4 oTexCoord4 : TEXCOORD4;
  float4 oTexCoord5 : TEXCOORD5;
  float4 oTexCoord6 : TEXCOORD6;
  float4 oTexCoord7 : TEXCOORD7;
  float4 oTexCoord8 : TEXCOORD8;
  float4 oTexCoord9 : TEXCOORD9;
  float4 oTexCoord10 : TEXCOORD10;
  float4 oTexCoord11 : TEXCOORD11;
  float4 oTexCoord12 : TEXCOORD12;
  float4 oTexCoord13 : TEXCOORD13;
  float4 oTexCoord14 : TEXCOORD14;
  float4 oTexCoord15 : TEXCOORD15;
  float4 oColor0 : COLOR0;
  float4 oColor1 : COLOR1;
  float clipDistance : SV_ClipDistance;
};

Interpolators shaderMain(VertexShaderInput input) {
  Interpolators output = (Interpolators)0;
  output.oPos = float4(input.iPosition0.xyz, 1.0);
  output.oTexCoord0.xy = input.iTexCoord0.xy;
  output.oTexCoord1.xy = -abs(input.iTexCoord0.xx) > 0.0;

  const uint valid =
      vk::RawBufferLoad<uint>(g_PushConstants.SharedConstants + 0x2D0);
  const uint required = 0x0000B000u;
  if ((valid & required) == required) {
    const float4x4 view_inverse = float4x4(
        vk::RawBufferLoad<float4>(g_PushConstants.SharedConstants + 0x280),
        vk::RawBufferLoad<float4>(g_PushConstants.SharedConstants + 0x290),
        vk::RawBufferLoad<float4>(g_PushConstants.SharedConstants + 0x2A0),
        vk::RawBufferLoad<float4>(g_PushConstants.SharedConstants + 0x2B0));
    const float2 projection_scale =
        vk::RawBufferLoad<float2>(g_PushConstants.SharedConstants + 0x2C0);
    const float2 safe_scale = max(abs(projection_scale), 1.0e-6);
    float2 projected = input.iTexCoord0.xy * 2.0 - 1.0;
    projected.y = -projected.y;
    projected /= safe_scale;
    const float3 view_ray = float3(-projected, 1.0);
    output.oTexCoord2.xyz =
        normalize(mul(float4(view_ray, 0.0), view_inverse).xyz);
  }

  const float4 clip_plane =
      vk::RawBufferLoad<float4>(g_PushConstants.SharedConstants + 0x230);
  const bool clip_enabled =
      vk::RawBufferLoad<bool>(g_PushConstants.SharedConstants + 0x240);
  if (clip_enabled) {
    output.clipDistance = dot(output.oPos, clip_plane);
  }
  const float2 half_pixel =
      vk::RawBufferLoad<float2>(g_PushConstants.SharedConstants + 0x228);
  output.oPos.xy += half_pixel * output.oPos.w;
  return output;
}
