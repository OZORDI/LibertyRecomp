/**
 * @file        ui/image_decode.cpp
 * @brief       PNG decode via stb_image. See image_decode.h.
 *
 * @copyright   Copyright (c) 2026 Rien Gupta <rgupta9@scu.edu>
 *              All rights reserved.
 *
 * @license     BSD 3-Clause License
 *              See LICENSE file in the project root for full license text.
 */
#include <rex/ui/image_decode.h>
#include <limits>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STBI_ONLY_PNG
#define STBI_NO_FAILURE_STRINGS
#include <stb_image.h>

namespace rex::ui {

std::vector<uint8_t> DecodeImageRGBA(const uint8_t* data, size_t size, int& out_width,
                                     int& out_height) {
  return DecodeImageRGBA(data, size, out_width, out_height,
                         std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
}

std::vector<uint8_t> DecodeImageRGBA(const uint8_t* data, size_t size, int& out_width,
                                    int& out_height, int max_width, int max_height) {
  out_width = 0;
  out_height = 0;
  if (!data || size == 0 || size > static_cast<size_t>(std::numeric_limits<int>::max()) ||
      max_width <= 0 || max_height <= 0) {
    return {};
  }
  int channels = 0;
  int width = 0, height = 0;
  if (!stbi_info_from_memory(data, static_cast<int>(size), &width, &height, &channels) ||
      width <= 0 || height <= 0 || width > max_width || height > max_height ||
      static_cast<size_t>(width) > std::numeric_limits<size_t>::max() / 4 / static_cast<size_t>(height)) return {};
  stbi_uc* pixels = stbi_load_from_memory(data, static_cast<int>(size), &out_width, &out_height,
                                          &channels, 4 /* force RGBA */);
  if (!pixels) {
    out_width = 0;
    out_height = 0;
    return {};
  }
  const std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> owner(pixels, stbi_image_free);
  const size_t byte_count = static_cast<size_t>(out_width) * out_height * 4;
  std::vector<uint8_t> result(pixels, pixels + byte_count);
  return result;
}

}  // namespace rex::ui
