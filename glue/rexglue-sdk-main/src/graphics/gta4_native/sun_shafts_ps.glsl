#version 450

layout(set = 0, binding = 0) uniform sampler2D scene_image;
layout(set = 0, binding = 1) uniform sampler2D pass_image;
layout(set = 0, binding = 2) uniform sampler2D depth_image;

layout(push_constant) uniform SunShaftConstants {
  ivec2 source_extent;
  ivec2 destination_extent;
  uint pass_index;
  uint sample_count;
  float density;
  float decay;
  vec4 sun_screen;
  vec4 sun_color_and_sky_start;
  vec4 sky_end_and_reserved;
} constants;

layout(location = 0) out vec4 output_color;

float luminance(vec3 color) {
  return dot(max(color, vec3(0.0)), vec3(0.2125, 0.7154, 0.0721));
}

float sky_mask(float encoded_depth) {
  return smoothstep(constants.sun_color_and_sky_start.w,
                    constants.sky_end_and_reserved.x, encoded_depth);
}

float edge_fade(vec2 uv) {
  vec2 aspect = vec2(float(constants.source_extent.x) / float(constants.source_extent.y), 1.0);
  vec2 rectangle = min(uv, vec2(1.0) - uv) * aspect;
  return clamp(32.0 * min(rectangle.x, rectangle.y), 0.0, 1.0);
}

vec4 prepass(vec2 uv) {
  vec3 scene = max(textureLod(scene_image, uv, 0.0).rgb, vec3(0.0));
  float depth = textureLod(depth_image, uv, 0.0).r;
  float sky = sky_mask(depth);
  vec2 aspect = vec2(float(constants.source_extent.x) / float(constants.source_extent.y), 1.0);
  float distance_to_sun = length((uv - constants.sun_screen.xy) * aspect);
  float sun_region = 1.0 - smoothstep(0.015, 0.085, distance_to_sun);

  // Xbox has no proven cloud-transmittance MRT at the composite draw. The rendered
  // sky radiance is therefore the deliberate occlusion-contrast substitute: dark
  // cloud pixels attenuate the source while stage-1 depth rejects solid geometry.
  // A dedicated transmittance texture can replace this factor without changing the
  // radial or composite passes.
  float radiance_transmittance = smoothstep(0.002, 0.35, luminance(scene));
  float mask = sky * sun_region * edge_fade(uv) * constants.sun_screen.w;
  vec3 sun_tint = max(constants.sun_color_and_sky_start.rgb, vec3(0.0));
  float tint_peak = max(max(sun_tint.r, sun_tint.g), max(sun_tint.b, 1.0e-5));
  sun_tint /= tint_peak;
  return vec4(scene * sun_tint * radiance_transmittance * mask * constants.sun_screen.z, sky);
}

vec4 radial_scatter(vec2 uv) {
  vec2 delta_uv = (uv - constants.sun_screen.xy) *
                  (constants.density / float(max(constants.sample_count, 1u)));
  vec4 accumulated = textureLod(pass_image, uv, 0.0);
  float weight_sum = 1.0;
  float illumination_decay = 1.0;
  for (uint index = 0u; index < constants.sample_count; ++index) {
    uv -= delta_uv;
    illumination_decay *= constants.decay;
    accumulated += textureLod(pass_image, uv, 0.0) * illumination_decay;
    weight_sum += illumination_decay;
  }
  return accumulated / max(weight_sum, 1.0e-5);
}

vec4 bilateral_upsample(vec2 uv) {
  vec4 scene = textureLod(scene_image, uv, 0.0);
  float center_sky = sky_mask(textureLod(depth_image, uv, 0.0).r);
  vec2 texel = 1.0 / vec2(constants.source_extent);
  vec2 offsets[4] = vec2[](vec2(-0.5, -0.5), vec2(0.5, -0.5),
                           vec2(-0.5, 0.5), vec2(0.5, 0.5));
  vec3 shafts = vec3(0.0);
  float weight_sum = 0.0;
  for (uint index = 0u; index < 4u; ++index) {
    vec4 sample_value = textureLod(pass_image, uv + offsets[index] * texel, 0.0);
    float bilateral_weight = 1.0 / (1.0 + 8.0 * abs(sample_value.a - center_sky));
    shafts += sample_value.rgb * bilateral_weight;
    weight_sum += bilateral_weight;
  }
  shafts /= max(weight_sum, 1.0e-5);
  return vec4(scene.rgb + shafts, scene.a);
}

void main() {
  vec2 uv = gl_FragCoord.xy / vec2(constants.destination_extent);
  if (constants.pass_index == 0u) {
    output_color = prepass(uv);
  } else if (constants.pass_index < 3u) {
    output_color = radial_scatter(uv);
  } else {
    output_color = bilateral_upsample(uv);
  }
}
