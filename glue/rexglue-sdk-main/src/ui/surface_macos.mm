/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2021 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 *
 * @modified    Tom Clay, 2026 - Adapted for ReXGlue runtime
 */

#include <rex/ui/surface_macos.h>

#include <QuartzCore/CAMetalLayer.h>

namespace rex::ui {

bool MetalLayerSurface::GetSizeImpl(uint32_t& width_out, uint32_t& height_out) const {
  auto* metal_layer = reinterpret_cast<CAMetalLayer*>(layer_);
  if (metal_layer == nil) {
    return false;
  }

  const CGSize bounds_size = metal_layer.bounds.size;
  const CGFloat scale = metal_layer.contentsScale > 0.0 ? metal_layer.contentsScale : 1.0;
  width_out = static_cast<uint32_t>(bounds_size.width * scale);
  height_out = static_cast<uint32_t>(bounds_size.height * scale);
  return true;
}

}  // namespace rex::ui
