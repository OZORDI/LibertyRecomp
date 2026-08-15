/**
 * @file        input/mnk/pointer_motion.cpp
 * @brief       Source-aware pointer motion accumulation and quantization.
 */
#include <rex/input/mnk/pointer_motion.h>

#include <algorithm>
#include <cmath>

namespace rex::input::mnk {

void PointerMotionAccumulator::Add(rex::ui::MouseEvent::MotionSource source, double delta_x,
                                   double delta_y) {
  if (!std::isfinite(delta_x) || !std::isfinite(delta_y)) {
    return;
  }
  if (delta_x == 0.0 && delta_y == 0.0) {
    return;
  }

  Bucket* bucket = nullptr;
  switch (source) {
    case rex::ui::MouseEvent::MotionSource::kRawMouse:
      bucket = &raw_mouse_;
      break;
    case rex::ui::MouseEvent::MotionSource::kSystemAccelerated:
      bucket = &system_accelerated_;
      break;
    case rex::ui::MouseEvent::MotionSource::kGeneric:
    default:
      bucket = &generic_;
      break;
  }

  bucket->delta_x += delta_x;
  bucket->delta_y += delta_y;
  bucket->seen = true;
}

PointerMotionSample PointerMotionAccumulator::Consume() {
  PointerMotionSample sample;
  const Bucket* selected = nullptr;

  // Raw GCMouse input wins only for a sample where the mouse moved. This
  // rejects its accelerated NSEvent duplicate without disabling an idle
  // mouse's simultaneously connected trackpad.
  if (raw_mouse_.seen) {
    selected = &raw_mouse_;
    sample.source = rex::ui::MouseEvent::MotionSource::kRawMouse;
  } else if (system_accelerated_.seen) {
    selected = &system_accelerated_;
    sample.source = rex::ui::MouseEvent::MotionSource::kSystemAccelerated;
  } else if (generic_.seen) {
    selected = &generic_;
    sample.source = rex::ui::MouseEvent::MotionSource::kGeneric;
  }

  if (selected) {
    sample.delta_x = selected->delta_x;
    sample.delta_y = selected->delta_y;
    sample.has_motion = true;
  }

  Reset();
  return sample;
}

void PointerMotionAccumulator::Reset() {
  generic_ = {};
  raw_mouse_ = {};
  system_accelerated_ = {};
}

MouseAxisQuantizer::MouseAxisQuantizer(int32_t maximum_magnitude)
    : maximum_magnitude_(std::max(maximum_magnitude, int32_t{1})) {}

int32_t MouseAxisQuantizer::Quantize(double delta, double sensitivity, double units_per_count,
                                     double frame_seconds, double reference_frame_seconds) {
  if (!std::isfinite(delta) || !std::isfinite(sensitivity) || !std::isfinite(units_per_count)) {
    Reset();
    return 0;
  }

  double frame_normalization = 1.0;
  if (std::isfinite(frame_seconds) && std::isfinite(reference_frame_seconds) &&
      frame_seconds > 0.0 && reference_frame_seconds > 0.0) {
    frame_normalization = reference_frame_seconds / frame_seconds;
  }

  const double scaled = delta * sensitivity * units_per_count * frame_normalization + residual_;
  if (!std::isfinite(scaled)) {
    Reset();
    return 0;
  }

  const double maximum = static_cast<double>(maximum_magnitude_);
  if (scaled >= maximum) {
    residual_ = 0.0;
    return maximum_magnitude_;
  }
  if (scaled <= -maximum) {
    residual_ = 0.0;
    return -maximum_magnitude_;
  }

  const int32_t result = static_cast<int32_t>(std::trunc(scaled));
  residual_ = scaled - static_cast<double>(result);
  return result;
}

void MouseAxisQuantizer::Reset() {
  residual_ = 0.0;
}

}  // namespace rex::input::mnk
