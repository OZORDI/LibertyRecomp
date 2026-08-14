#version 450
#extension GL_GOOGLE_include_directive : require

#define SMAA_GLSL_4 1
#define SMAA_INCLUDE_VS 1
#define SMAA_INCLUDE_PS 1

layout(push_constant) uniform SmaaConstants {
  vec4 rt_metrics;
} smaa_constants;

#define SMAA_RT_METRICS smaa_constants.rt_metrics
#include "SMAA.hlsl"

layout(set = 0, binding = 0) uniform sampler2D color_gamma_tex;
layout(set = 0, binding = 1) uniform sampler2D blend_tex;
layout(location = 0) out vec4 output_color;

vec3 smaa_srgb_to_linear(vec3 color) {
  bvec3 linear_segment = lessThanEqual(color, vec3(0.04045));
  vec3 low = color / 12.92;
  vec3 high = pow((color + 0.055) / 1.055, vec3(2.4));
  return mix(high, low, linear_segment);
}

vec3 smaa_linear_to_srgb(vec3 color) {
  bvec3 linear_segment = lessThanEqual(color, vec3(0.0031308));
  vec3 low = color * 12.92;
  vec3 high = 1.055 * pow(max(color, vec3(0.0)), vec3(1.0 / 2.4)) - 0.055;
  return mix(high, low, linear_segment);
}

vec4 smaa_sample_srgb_linear(vec2 texcoord) {
  ivec2 extent = textureSize(color_gamma_tex, 0);
  vec2 texel_position = texcoord * vec2(extent) - vec2(0.5);
  ivec2 base = ivec2(floor(texel_position));
  vec2 factor = fract(texel_position);
  ivec2 maximum = extent - ivec2(1);
  vec4 encoded_00 = texelFetch(color_gamma_tex, clamp(base, ivec2(0), maximum), 0);
  vec4 encoded_10 = texelFetch(color_gamma_tex, clamp(base + ivec2(1, 0), ivec2(0), maximum), 0);
  vec4 encoded_01 = texelFetch(color_gamma_tex, clamp(base + ivec2(0, 1), ivec2(0), maximum), 0);
  vec4 encoded_11 = texelFetch(color_gamma_tex, clamp(base + ivec2(1, 1), ivec2(0), maximum), 0);
  vec4 linear_00 = vec4(smaa_srgb_to_linear(encoded_00.rgb), encoded_00.a);
  vec4 linear_10 = vec4(smaa_srgb_to_linear(encoded_10.rgb), encoded_10.a);
  vec4 linear_01 = vec4(smaa_srgb_to_linear(encoded_01.rgb), encoded_01.a);
  vec4 linear_11 = vec4(smaa_srgb_to_linear(encoded_11.rgb), encoded_11.a);
  return mix(mix(linear_00, linear_10, factor.x),
             mix(linear_01, linear_11, factor.x), factor.y);
}

// This is the canonical SMAA 1x neighborhood pass with explicit sRGB
// decode/encode. The guest frontbuffer is stored as encoded UNORM, so this is
// equivalent to FusionFix's D3DSAMP_SRGBTEXTURE + D3DRS_SRGBWRITEENABLE path
// without requiring a mutable-format view of a guest-owned Vulkan image.
vec4 smaa_neighborhood_srgb(vec2 texcoord, vec4 offset) {
  vec4 a;
  a.x = texture(blend_tex, offset.xy).a;
  a.y = texture(blend_tex, offset.zw).g;
  a.wz = texture(blend_tex, texcoord).xz;

  if (dot(a, vec4(1.0)) < 1e-5) {
    return textureLod(color_gamma_tex, texcoord, 0.0);
  }

  bool horizontal = max(a.x, a.z) > max(a.y, a.w);
  vec4 blending_offset = horizontal ? vec4(a.x, 0.0, a.z, 0.0)
                                    : vec4(0.0, a.y, 0.0, a.w);
  vec2 blending_weight = horizontal ? a.xz : a.yw;
  blending_weight /= dot(blending_weight, vec2(1.0));
  vec4 blending_coord = fma(
      blending_offset, vec4(SMAA_RT_METRICS.xy, -SMAA_RT_METRICS.xy), texcoord.xyxy);

  vec4 linear_a = smaa_sample_srgb_linear(blending_coord.xy);
  vec4 linear_b = smaa_sample_srgb_linear(blending_coord.zw);
  vec3 linear_color = blending_weight.x * linear_a.rgb + blending_weight.y * linear_b.rgb;
  float alpha = dot(blending_weight, vec2(linear_a.a, linear_b.a));
  return vec4(smaa_linear_to_srgb(linear_color), alpha);
}

void main() {
  vec2 texcoord = gl_FragCoord.xy * SMAA_RT_METRICS.xy;
  vec4 offset;
  SMAANeighborhoodBlendingVS(texcoord, offset);
  output_color = smaa_neighborhood_srgb(texcoord, offset);
}
