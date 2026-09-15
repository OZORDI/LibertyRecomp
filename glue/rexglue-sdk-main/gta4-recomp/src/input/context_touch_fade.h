#pragma once

#include <algorithm>
#include <cstdint>

namespace gta4::input {

// Each visibility transition lasts one second from its current opacity.
// Reversing an unfinished transition preserves the opacity at that instant.
class ContextTouchFade {
 public:
  static constexpr uint64_t kDurationNanoseconds = 1000000000;

  float Advance(bool visible, uint64_t monotonic_ns) noexcept {
    if (!initialized_) {
      initialized_ = true;
      target_ = visible;
      started_ns_ = last_ns_ = monotonic_ns;
      return 0.0f;
    }
    monotonic_ns = std::max(monotonic_ns, last_ns_);
    const double fraction = std::min(double(monotonic_ns - started_ns_) /
                                         double(kDurationNanoseconds), 1.0);
    const float opacity = static_cast<float>(
        start_opacity_ + ((target_ ? 1.0 : 0.0) - start_opacity_) * fraction);
    if (target_ != visible) {
      target_ = visible;
      started_ns_ = monotonic_ns;
      start_opacity_ = opacity;
    }
    last_ns_ = monotonic_ns;
    return opacity;
  }

  void Reset() noexcept { *this = {}; }

 private:
  uint64_t started_ns_ = 0;
  uint64_t last_ns_ = 0;
  float start_opacity_ = 0.0f;
  bool initialized_ = false;
  bool target_ = false;
};

}  // namespace gta4::input
