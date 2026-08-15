/**
 * @file        rex/input/mnk/pointer_motion.h
 * @brief       Source-aware pointer motion accumulation and quantization.
 */
#pragma once

#include <cstdint>

#include <rex/ui/ui_event.h>

namespace rex::input::mnk {

struct PointerMotionSample {
  double delta_x = 0.0;
  double delta_y = 0.0;
  rex::ui::MouseEvent::MotionSource source = rex::ui::MouseEvent::MotionSource::kGeneric;
  bool has_motion = false;
};

class PointerMotionAccumulator {
 public:
  void Add(rex::ui::MouseEvent::MotionSource source, double delta_x, double delta_y);
  PointerMotionSample Consume();
  void Reset();

 private:
  struct Bucket {
    double delta_x = 0.0;
    double delta_y = 0.0;
    bool seen = false;
  };

  Bucket generic_;
  Bucket raw_mouse_;
  Bucket system_accelerated_;
};

class MouseAxisQuantizer {
 public:
  explicit MouseAxisQuantizer(int32_t maximum_magnitude = 255);

  int32_t Quantize(double delta, double sensitivity, double units_per_count, double frame_seconds,
                   double reference_frame_seconds);
  void Reset();

 private:
  double residual_ = 0.0;
  int32_t maximum_magnitude_ = 255;
};

}  // namespace rex::input::mnk
