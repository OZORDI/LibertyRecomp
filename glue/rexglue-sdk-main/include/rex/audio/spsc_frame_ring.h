/**
 ******************************************************************************
 * ReXGlue : lock-free single-producer/single-consumer audio frame ring       *
 ******************************************************************************
 */

#pragma once

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>

namespace rex::audio {

// The producer and consumer each own one monotonic counter. Storage is never
// allocated, resized, or locked after construction, so Read is safe to call
// from a real-time audio callback.
class SpscFrameRing {
 public:
  SpscFrameRing(uint32_t capacity_frames, uint32_t max_channels)
      : capacity_frames_(capacity_frames),
        frame_mask_(capacity_frames - 1),
        max_channels_(max_channels),
        samples_(std::make_unique<float[]>(size_t(capacity_frames) * max_channels)) {}

  SpscFrameRing(const SpscFrameRing&) = delete;
  SpscFrameRing& operator=(const SpscFrameRing&) = delete;

  bool valid() const {
    return capacity_frames_ != 0 && (capacity_frames_ & frame_mask_) == 0 && max_channels_ != 0;
  }

  uint32_t capacity_frames() const { return capacity_frames_; }
  uint64_t read_frame() const { return read_frame_.load(std::memory_order_acquire); }
  uint64_t write_frame() const { return write_frame_.load(std::memory_order_acquire); }

  uint64_t available_frames() const {
    const uint64_t write = write_frame_.load(std::memory_order_acquire);
    const uint64_t read = read_frame_.load(std::memory_order_acquire);
    return write - read;
  }

  uint64_t free_frames() const { return capacity_frames_ - available_frames(); }

  bool Write(const float* source, uint32_t frame_count, uint32_t channels) {
    if (!source || channels == 0 || channels > max_channels_ || frame_count > capacity_frames_) {
      return false;
    }

    const uint64_t write = write_frame_.load(std::memory_order_relaxed);
    const uint64_t read = read_frame_.load(std::memory_order_acquire);
    if (frame_count > capacity_frames_ - (write - read)) {
      return false;
    }

    const uint32_t first_frame = static_cast<uint32_t>(write) & frame_mask_;
    const uint32_t first_count = std::min(frame_count, capacity_frames_ - first_frame);
    std::memcpy(samples_.get() + size_t(first_frame) * channels, source,
                size_t(first_count) * channels * sizeof(float));
    if (first_count != frame_count) {
      std::memcpy(samples_.get(), source + size_t(first_count) * channels,
                  size_t(frame_count - first_count) * channels * sizeof(float));
    }
    write_frame_.store(write + frame_count, std::memory_order_release);
    return true;
  }

  // Reads up to frame_count frames. If add is true, samples are accumulated
  // into destination; otherwise they replace it. Returns the number of frames
  // actually consumed.
  uint32_t Read(float* destination, uint32_t frame_count, uint32_t channels, bool add) {
    if (!destination || channels == 0 || channels > max_channels_ || frame_count == 0) {
      return 0;
    }

    const uint64_t read = read_frame_.load(std::memory_order_relaxed);
    const uint64_t write = write_frame_.load(std::memory_order_acquire);
    const uint32_t readable = static_cast<uint32_t>(std::min<uint64_t>(frame_count, write - read));
    if (!readable) {
      return 0;
    }

    const uint32_t first_frame = static_cast<uint32_t>(read) & frame_mask_;
    const uint32_t first_count = std::min(readable, capacity_frames_ - first_frame);
    CopyOrAdd(destination, samples_.get() + size_t(first_frame) * channels,
              size_t(first_count) * channels, add);
    if (first_count != readable) {
      CopyOrAdd(destination + size_t(first_count) * channels, samples_.get(),
                size_t(readable - first_count) * channels, add);
    }
    read_frame_.store(read + readable, std::memory_order_release);
    return readable;
  }

  // Only call while the consumer is stopped or guaranteed to skip this ring.
  void DiscardAll() {
    read_frame_.store(write_frame_.load(std::memory_order_acquire), std::memory_order_release);
  }

 private:
  static void CopyOrAdd(float* destination, const float* source, size_t sample_count, bool add) {
    if (!add) {
      std::memcpy(destination, source, sample_count * sizeof(float));
      return;
    }
    for (size_t i = 0; i < sample_count; ++i) {
      destination[i] += source[i];
    }
  }

  const uint32_t capacity_frames_;
  const uint32_t frame_mask_;
  const uint32_t max_channels_;
  std::unique_ptr<float[]> samples_;
  alignas(64) std::atomic<uint64_t> read_frame_{0};
  alignas(64) std::atomic<uint64_t> write_frame_{0};
};

}  // namespace rex::audio
