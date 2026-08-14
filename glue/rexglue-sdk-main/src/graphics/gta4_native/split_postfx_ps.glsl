#version 450

layout(set = 0, binding = 0) uniform sampler2D color_image;
layout(set = 0, binding = 1) uniform sampler2D blur_image;
layout(set = 0, binding = 2) uniform sampler2D depth_image;
layout(set = 0, binding = 3) uniform sampler2D stipple_mask_image;

layout(push_constant) uniform SplitPostFxConstants {
  ivec2 source_extent;
  ivec2 destination_extent;
  uint pass_index;
  uint depth_source;
  uint reserved0;
  uint reserved1;
  vec4 dof_projection;
  vec4 dof_distance;
  vec4 dof_blur;
} constants;

layout(location = 0) out vec4 output_color;

const vec2 disk_kernel[16] = vec2[](
    vec2(0.0, 0.0), vec2(0.16855472, 0.5187581),
    vec2(-0.44128203, 0.3206101), vec2(-0.44128197, -0.3206102),
    vec2(0.1685548, -0.5187581), vec2(1.0, 0.0),
    vec2(0.809017, 0.58778524), vec2(0.30901697, 0.95105654),
    vec2(-0.30901703, 0.9510565), vec2(-0.80901706, 0.5877852),
    vec2(-1.0, 0.0), vec2(-0.80901694, -0.58778536),
    vec2(-0.30901664, -0.9510566), vec2(0.30901712, -0.9510565),
    vec2(0.80901694, -0.5877853), vec2(0.54545456, 0.0));

vec2 pixel_uv() {
  return gl_FragCoord.xy / vec2(constants.destination_extent);
}

float luminance(vec3 color) {
  return dot(color, vec3(0.2125, 0.7154, 0.0721));
}

vec4 apply_stipple_filter(vec2 uv) {
  vec2 texel = 1.0 / vec2(constants.source_extent);
  vec4 center = textureLod(color_image, uv, 0.0);
  vec4 samples[4] = vec4[](
      textureLod(color_image, uv + texel * vec2(-0.5, -1.5), 0.0),
      textureLod(color_image, uv + texel * vec2(1.5, -0.5), 0.0),
      textureLod(color_image, uv + texel * vec2(0.5, 1.5), 0.0),
      textureLod(color_image, uv + texel * vec2(-1.5, 0.5), 0.0));
  vec4 average = (samples[0] + samples[1] + samples[2] + samples[3]) * 0.25;
  vec4 deviations = vec4(luminance(samples[0].rgb), luminance(samples[1].rgb),
                         luminance(samples[2].rgb), luminance(samples[3].rgb)) -
                    luminance(average.rgb);
  float center_deviation = luminance(center.rgb) - luminance(average.rgb);
  float detected = center_deviation * center_deviation > dot(deviations, deviations) ? 1.0 : 0.0;
  float mask = 0.25 * (
      textureLod(stipple_mask_image, uv, 0.0).a +
      textureLod(stipple_mask_image, uv + vec2(texel.x, 0.0), 0.0).a +
      textureLod(stipple_mask_image, uv + vec2(0.0, texel.y), 0.0).a +
      textureLod(stipple_mask_image, uv + texel, 0.0).a);
  return vec4(mix(center.rgb, average.rgb, detected * clamp(mask, 0.0, 1.0)), center.a);
}

vec4 gather_bokeh(vec2 uv) {
  vec2 texel = 1.0 / vec2(constants.source_extent);
  float radius_pixels = 2.0 * float(constants.source_extent.y) / 720.0;
  vec3 sum = vec3(0.0);
  vec3 positive_detail = vec3(0.0);
  for (int index = 0; index < 16; ++index) {
    vec3 sample_color = textureLod(color_image, uv + disk_kernel[index] * texel * radius_pixels,
                                   0.0).rgb;
    sum += sample_color;
  }
  vec3 average = sum * 0.0625;
  for (int index = 0; index < 16; ++index) {
    vec3 sample_color = textureLod(color_image, uv + disk_kernel[index] * texel * radius_pixels,
                                   0.0).rgb;
    positive_detail += max(sample_color - average, vec3(0.0));
  }
  return vec4(average + positive_detail * 0.0625, 1.0);
}

vec4 tent_filter(vec2 uv) {
  vec2 texel = 1.0 / vec2(constants.source_extent);
  float radius = clamp(float(constants.destination_extent.y) / 720.0 - 0.5, 0.0, 1.0);
  vec4 taps[4] = vec4[](
      textureLod(color_image, uv + texel * vec2(-radius, -radius), 0.0),
      textureLod(color_image, uv + texel * vec2(radius, -radius), 0.0),
      textureLod(color_image, uv + texel * vec2(-radius, radius), 0.0),
      textureLod(color_image, uv + texel * vec2(radius, radius), 0.0));
  vec4 average = (taps[0] + taps[1] + taps[2] + taps[3]) * 0.25;
  vec4 positive_detail = max(taps[0] - average, vec4(0.0)) +
                         max(taps[1] - average, vec4(0.0)) +
                         max(taps[2] - average, vec4(0.0)) +
                         max(taps[3] - average, vec4(0.0));
  return vec4((average + positive_detail * 0.25).rgb, 1.0);
}

vec4 combine_dof(vec2 uv) {
  vec4 sharp = textureLod(color_image, uv, 0.0);
  vec3 blurred = textureLod(blur_image, uv, 0.0).rgb;
  float encoded_depth = textureLod(depth_image, uv, 0.0).r;
  float depth_base = abs(constants.dof_projection.z);
  float view_depth = depth_base > 0.0
                         ? pow(depth_base, encoded_depth) * constants.dof_projection.w
                         : encoded_depth;
  float focus_width = constants.dof_distance.y * 0.5;
  float near_amount = max(constants.dof_distance.w - view_depth - focus_width, 0.0) /
                      max(abs(constants.dof_distance.x), 1e-6);
  float far_amount = max(view_depth - constants.dof_distance.w - focus_width, 0.0) /
                     max(abs(constants.dof_distance.z), 1e-6);
  float near_blur = min(mix(constants.dof_blur.y, constants.dof_blur.x, near_amount),
                        constants.dof_blur.x);
  float far_blur = min(mix(constants.dof_blur.y, constants.dof_blur.z, far_amount),
                       constants.dof_blur.z);
  float coc = clamp(max(near_blur, far_blur), 0.0, 1.0);
  return vec4(mix(sharp.rgb, blurred, coc * coc), sharp.a);
}

void main() {
  vec2 uv = pixel_uv();
  if (constants.pass_index == 0u) {
    output_color = apply_stipple_filter(uv);
  } else if (constants.pass_index == 1u) {
    output_color = gather_bokeh(uv);
  } else if (constants.pass_index == 2u) {
    output_color = tent_filter(uv);
  } else {
    output_color = combine_dof(uv);
  }
}
