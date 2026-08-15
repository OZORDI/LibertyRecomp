/**
 ******************************************************************************
 * ReXGlue : exact rational audio frame clock                                 *
 ******************************************************************************
 */

#pragma once

#include <chrono>
#include <cstdint>

namespace rex::audio {

class RationalFrameClock {
 public:
  RationalFrameClock(uint32_t sample_rate, uint32_t frames_per_quantum)
      : sample_rate_(sample_rate), frames_per_quantum_(frames_per_quantum) {}

  std::chrono::nanoseconds NextDuration() {
    constexpr uint64_t kNanosecondsPerSecond = 1000000000ULL;
    const uint64_t numerator = uint64_t(frames_per_quantum_) * kNanosecondsPerSecond + remainder_;
    const uint64_t nanoseconds = numerator / sample_rate_;
    remainder_ = numerator % sample_rate_;
    return std::chrono::nanoseconds(nanoseconds);
  }

  void Reset() { remainder_ = 0; }

 private:
  uint32_t sample_rate_;
  uint32_t frames_per_quantum_;
  uint64_t remainder_ = 0;
};

}  // namespace rex::audio
