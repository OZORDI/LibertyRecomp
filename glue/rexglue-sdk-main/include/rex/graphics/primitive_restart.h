/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2021 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#pragma once

#include <cstdint>

#include <rex/graphics/xenos.h>

namespace rex::graphics::primitive_restart {

inline bool IsDisabledRestartSentinelUsed(const uint16_t* indices, uint32_t count) {
  while (count--) {
    if (*(indices++) == UINT16_MAX) {
      return true;
    }
  }
  return false;
}

inline bool IsDisabledRestartSentinelUsed(const uint32_t* indices, uint32_t count) {
  while (count--) {
    if (*(indices++) == UINT32_MAX) {
      return true;
    }
  }
  return false;
}

inline void ConvertDisabledRestartIndices(uint32_t* destination, const uint16_t* source,
                                          uint32_t count, xenos::Endian guest_endian) {
  while (count--) {
    *(destination++) = xenos::GpuSwap(*(source++), guest_endian);
  }
}

inline void ConvertDisabledRestartIndices(uint32_t* destination, const uint32_t* source,
                                          uint32_t count, xenos::Endian guest_endian) {
  while (count--) {
    *(destination++) = xenos::GpuSwap(*(source++), guest_endian) & xenos::kVertexIndexMask;
  }
}

}  // namespace rex::graphics::primitive_restart
