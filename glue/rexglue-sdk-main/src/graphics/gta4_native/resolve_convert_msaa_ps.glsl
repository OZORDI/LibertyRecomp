#version 450

layout(set = 0, binding = 0) uniform sampler2DMS source_image;

layout(push_constant) uniform ResolveConvertConstants {
  ivec2 source_origin;
  ivec2 destination_origin;
  uint source_guest_sample_type;
  uint requested_guest_sample_type;
  uint destination_guest_sample_type;
  uint sample_select;
  uint mode;
  uint physical_source_sample_type;
  uint physical_destination_sample_type;
  uint flags;
  uvec2 source_extent;
  uvec2 destination_extent;
} resolve_constants;

layout(location = 0) out vec4 output_color;
#ifdef GTA4_RESOLVE_HDR_MIRROR
layout(location = 1) out vec4 output_hdr_color;
#endif

const uint kFlagXenosFloat16Pack = 1u;
const uint kFlagDirectPhysicalMaterialization = 2u;
const uint kFlagScaleConversion = 4u;

ivec2 sample_scale(uint sample_type) {
  return ivec2(sample_type >= 2u ? 2 : 1, sample_type >= 1u ? 2 : 1);
}

ivec2 sample_offset(uint sample_type, uint sample_index) {
  return ivec2(sample_type >= 2u ? int((sample_index >> 1u) & 1u) : 0,
                sample_type >= 1u ? int(sample_index & 1u) : 0);
}

vec4 fetch_owner(ivec2 sample_coordinate) {
  ivec2 scale = sample_scale(resolve_constants.source_guest_sample_type);
  ivec2 pixel = sample_coordinate / scale;
  ivec2 within_pixel = sample_coordinate - pixel * scale;
  int sample_index = resolve_constants.source_guest_sample_type >= 2u
                         ? within_pixel.x * 2 + within_pixel.y
                         : resolve_constants.source_guest_sample_type >= 1u ? within_pixel.y : 0;
  return texelFetch(source_image, pixel, sample_index);
}

vec4 fetch_physical_pixel(ivec2 pixel) {
  uint sample_count = 1u << min(resolve_constants.physical_source_sample_type, 2u);
  vec4 sum = vec4(0.0);
  for (uint sample_index = 0u; sample_index < sample_count; ++sample_index) {
    sum += texelFetch(source_image, pixel, int(sample_index));
  }
  return sum / float(sample_count);
}

vec4 fetch_requested_sample(ivec2 pixel, uint sample_index) {
  ivec2 sample_coordinate = pixel * sample_scale(resolve_constants.requested_guest_sample_type) +
                            sample_offset(resolve_constants.requested_guest_sample_type,
                                          sample_index);
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

vec4 filter_physical_region(vec2 region_min, vec2 region_max) {
  vec2 source_min = vec2(resolve_constants.source_origin);
  vec2 source_max = source_min + vec2(resolve_constants.source_extent);
  region_min = clamp(region_min, source_min, source_max);
  region_max = clamp(region_max, source_min, source_max);
  ivec2 texture_max = textureSize(source_image) - ivec2(1);
  ivec2 first_texel = clamp(ivec2(floor(region_min)), ivec2(0), texture_max);
  ivec2 last_texel = clamp(ivec2(ceil(region_max)) - ivec2(1), ivec2(0), texture_max);
  vec4 sum = vec4(0.0);
  float weight_sum = 0.0;
  for (int y = first_texel.y; y <= last_texel.y; ++y) {
    float y_weight = max(0.0, min(region_max.y, float(y + 1)) -
                                  max(region_min.y, float(y)));
    for (int x = first_texel.x; x <= last_texel.x; ++x) {
      float x_weight = max(0.0, min(region_max.x, float(x + 1)) -
                                    max(region_min.x, float(x)));
      float weight = x_weight * y_weight;
      sum += fetch_physical_pixel(ivec2(x, y)) * weight;
      weight_sum += weight;
    }
  }
  if (weight_sum > 0.0) {
    return sum / weight_sum;
  }
  ivec2 fallback = clamp(ivec2(floor((region_min + region_max) * 0.5)), ivec2(0), texture_max);
  return fetch_physical_pixel(fallback);
}

void scaled_pixel_footprint(ivec2 destination, out vec2 source_min, out vec2 source_max) {
  vec2 relative = vec2(destination - resolve_constants.destination_origin);
  vec2 scale = vec2(resolve_constants.source_extent) /
               max(vec2(resolve_constants.destination_extent), vec2(1.0));
  source_min = vec2(resolve_constants.source_origin) + relative * scale;
  source_max = vec2(resolve_constants.source_origin) + (relative + vec2(1.0)) * scale;
}

vec4 filter_scaled_requested_sample(vec2 footprint_min, vec2 footprint_max, uint sample_index) {
  ivec2 requested_scale = sample_scale(resolve_constants.requested_guest_sample_type);
  ivec2 requested_offset = sample_offset(resolve_constants.requested_guest_sample_type, sample_index);
  vec2 sample_size = (footprint_max - footprint_min) / vec2(requested_scale);
  vec2 sample_min = footprint_min + vec2(requested_offset) * sample_size;
  return filter_physical_region(sample_min, sample_min + sample_size);
}

vec4 resolve_scaled_requested(ivec2 destination) {
  vec2 footprint_min;
  vec2 footprint_max;
  scaled_pixel_footprint(destination, footprint_min, footprint_max);
  uint select = resolve_constants.sample_select;
  if (select <= 3u) {
    return filter_scaled_requested_sample(footprint_min, footprint_max, select);
  }
  if (select == 4u) {
    return (filter_scaled_requested_sample(footprint_min, footprint_max, 0u) +
            filter_scaled_requested_sample(footprint_min, footprint_max, 1u)) * 0.5;
  }
  if (select == 5u) {
    return (filter_scaled_requested_sample(footprint_min, footprint_max, 2u) +
            filter_scaled_requested_sample(footprint_min, footprint_max, 3u)) * 0.5;
  }
  return (filter_scaled_requested_sample(footprint_min, footprint_max, 0u) +
          filter_scaled_requested_sample(footprint_min, footprint_max, 1u) +
          filter_scaled_requested_sample(footprint_min, footprint_max, 2u) +
          filter_scaled_requested_sample(footprint_min, footprint_max, 3u)) * 0.25;
}

vec4 filter_scaled_pixel(ivec2 destination) {
  vec2 footprint_min;
  vec2 footprint_max;
  scaled_pixel_footprint(destination, footprint_min, footprint_max);
  return filter_physical_region(footprint_min, footprint_max);
}

vec4 sanitize_float16_color(vec4 color) {
  bvec4 nan_components = isnan(color);
  vec4 finite_color = clamp(color, vec4(-65504.0), vec4(65504.0));
  return mix(finite_color, vec4(0.0), nan_components);
}

vec4 resolve_color() {
  ivec2 destination = ivec2(gl_FragCoord.xy);
  bool scale_conversion = (resolve_constants.flags & kFlagScaleConversion) != 0u;
  if (resolve_constants.mode == 0u && scale_conversion) {
    return resolve_scaled_requested(destination);
  }
  if (resolve_constants.mode != 0u && scale_conversion) {
    return filter_scaled_pixel(destination);
  }
  ivec2 requested_pixel = resolve_constants.source_origin + destination -
                          resolve_constants.destination_origin;
  if (resolve_constants.mode == 0u) {
    return resolve_requested(requested_pixel);
  }
  if ((resolve_constants.flags & kFlagDirectPhysicalMaterialization) != 0u) {
    uint direct_sample = resolve_constants.physical_destination_sample_type == 0u
                             ? 0u
                             : uint(gl_SampleID);
    return texelFetch(source_image, requested_pixel, int(direct_sample));
  }
  uint destination_sample = resolve_constants.physical_destination_sample_type == 0u
                                ? 0u
                                : uint(gl_SampleID);
  ivec2 sample_coordinate =
      requested_pixel * sample_scale(resolve_constants.destination_guest_sample_type) +
      sample_offset(resolve_constants.destination_guest_sample_type, destination_sample);
  return fetch_owner(sample_coordinate);
}

void main() {
  // Resolve exponent is distinct from render-target output exponent. The game
  // requests its reciprocal when moving from the stored scene to a sampled image.
  int exponent = int((resolve_constants.flags >> 8u) & 31u) -
                 int((resolve_constants.flags >> 8u) & 32u);
  vec4 color = resolve_color() * exp2(float(exponent));
  output_color = (resolve_constants.flags & kFlagXenosFloat16Pack) != 0u
                     ? sanitize_float16_color(color) : color;
#ifdef GTA4_RESOLVE_HDR_MIRROR
  // Both outputs share the exact guest/host sample mapping and scale filter.
  output_hdr_color = sanitize_float16_color(color);
#endif
}
