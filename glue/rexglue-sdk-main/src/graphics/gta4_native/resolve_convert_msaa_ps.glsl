#version 450

layout(set = 0, binding = 0) uniform sampler2DMS source_image;

layout(push_constant) uniform ResolveConvertConstants {
  ivec2 source_origin;
  ivec2 destination_origin;
  uint source_sample_type;
  uint requested_sample_type;
  uint destination_sample_type;
  uint sample_select;
  uint mode;
  uvec3 reserved;
} resolve_constants;

layout(location = 0) out vec4 output_color;

ivec2 sample_scale(uint sample_type) {
  return ivec2(sample_type >= 2u ? 2 : 1, sample_type >= 1u ? 2 : 1);
}

ivec2 sample_offset(uint sample_type, uint sample_index) {
  return ivec2(sample_type >= 2u ? int((sample_index >> 1u) & 1u) : 0,
                sample_type >= 1u ? int(sample_index & 1u) : 0);
}

vec4 fetch_owner(ivec2 sample_coordinate) {
  ivec2 scale = sample_scale(resolve_constants.source_sample_type);
  ivec2 pixel = sample_coordinate / scale;
  ivec2 within_pixel = sample_coordinate - pixel * scale;
  int sample_index = resolve_constants.source_sample_type >= 2u
                         ? within_pixel.x * 2 + within_pixel.y
                         : resolve_constants.source_sample_type >= 1u ? within_pixel.y : 0;
  return texelFetch(source_image, pixel, sample_index);
}

vec4 fetch_requested_sample(ivec2 pixel, uint sample_index) {
  ivec2 sample_coordinate = pixel * sample_scale(resolve_constants.requested_sample_type) +
                            sample_offset(resolve_constants.requested_sample_type, sample_index);
  return fetch_owner(sample_coordinate);
}

vec4 resolve_requested(ivec2 pixel) {
  uint select = resolve_constants.sample_select;
  if (select <= 3u) {
    return fetch_requested_sample(pixel, select);
  }
  if (select == 4u) {
    return (fetch_requested_sample(pixel, 0u) + fetch_requested_sample(pixel, 1u)) * 0.5;
  }
  if (select == 5u) {
    return (fetch_requested_sample(pixel, 2u) + fetch_requested_sample(pixel, 3u)) * 0.5;
  }
  return (fetch_requested_sample(pixel, 0u) + fetch_requested_sample(pixel, 1u) +
          fetch_requested_sample(pixel, 2u) + fetch_requested_sample(pixel, 3u)) * 0.25;
}

vec4 pack_resolve_color(vec4 color) {
  if (resolve_constants.reserved.x == 0u) {
    return color;
  }
  // Xenos 16_16_16_16_FLOAT resolve packing flushes NaN to zero and clamps
  // infinities / out-of-range values to the finite IEEE binary16 range.
  bvec4 nan_components = isnan(color);
  vec4 finite_color = clamp(color, vec4(-65504.0), vec4(65504.0));
  return mix(finite_color, vec4(0.0), nan_components);
}

void main() {
  ivec2 destination = ivec2(gl_FragCoord.xy);
  ivec2 requested_pixel = resolve_constants.source_origin + destination -
                          resolve_constants.destination_origin;
  if (resolve_constants.mode == 0u) {
    output_color = pack_resolve_color(resolve_requested(requested_pixel));
    return;
  }
  uint destination_sample = resolve_constants.destination_sample_type == 0u
                                ? 0u
                                : uint(gl_SampleID);
  ivec2 sample_coordinate =
      requested_pixel * sample_scale(resolve_constants.destination_sample_type) +
      sample_offset(resolve_constants.destination_sample_type, destination_sample);
  output_color = pack_resolve_color(fetch_owner(sample_coordinate));
}
