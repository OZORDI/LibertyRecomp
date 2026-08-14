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

ivec2 sample_scale(uint sample_type) {
  return ivec2(sample_type >= 2u ? 2 : 1, sample_type >= 1u ? 2 : 1);
}

ivec2 sample_offset(uint sample_type, uint sample_index) {
  return ivec2(sample_type >= 2u ? int((sample_index >> 1u) & 1u) : 0,
                sample_type >= 1u ? int(sample_index & 1u) : 0);
}

float fetch_owner_depth(ivec2 sample_coordinate) {
  ivec2 scale = sample_scale(resolve_constants.source_sample_type);
  ivec2 pixel = sample_coordinate / scale;
  ivec2 within_pixel = sample_coordinate - pixel * scale;
  int sample_index = resolve_constants.source_sample_type >= 2u
                         ? within_pixel.x * 2 + within_pixel.y
                         : resolve_constants.source_sample_type >= 1u ? within_pixel.y : 0;
  return texelFetch(source_image, pixel, sample_index).x;
}

float fetch_requested_depth(ivec2 pixel, uint sample_index) {
  ivec2 sample_coordinate = pixel * sample_scale(resolve_constants.requested_sample_type) +
                            sample_offset(resolve_constants.requested_sample_type, sample_index);
  return fetch_owner_depth(sample_coordinate);
}

void main() {
  ivec2 destination = ivec2(gl_FragCoord.xy);
  ivec2 requested_pixel = resolve_constants.source_origin + destination -
                          resolve_constants.destination_origin;
  gl_FragDepth = fetch_requested_depth(requested_pixel, resolve_constants.sample_select);
}
