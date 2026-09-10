#ifndef REX_GRAPHICS_GTA4_NATIVE_NATIVE_FRAME_SCHEDULING_H_
#define REX_GRAPHICS_GTA4_NATIVE_NATIVE_FRAME_SCHEDULING_H_

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace rex::graphics::gta4_native {

// Native texture images are destroyed only after going unreferenced for a
// full grace window to avoid repeatedly destroying and recreating resources
// that the title cycles across frames.
inline constexpr uint32_t kNativeTextureEvictionGraceFrames = 240;
inline constexpr uint32_t kNativeTextureCacheRetentionFrames = 600;
inline constexpr uint32_t kNativeTextureBudgetReserveDivisor = 10;
inline constexpr uint32_t kNativeTextureBudgetPollFrames = 120;
inline constexpr uint32_t kNativeTextureBudgetPollPhaseFrames = 40;
inline constexpr uint32_t kNativeTexturePressureEnterPercent = 90;
inline constexpr uint32_t kNativeTexturePressureExitPercent = 85;
inline constexpr size_t kNativeBufferShadowValidationSlices = 64;
inline constexpr uint32_t kNativeBufferCacheRetentionFrames = 600;
inline constexpr uint32_t kNativeBufferCachePollFrames = 120;
inline constexpr uint32_t kNativeBufferCachePollPhaseFrames = 80;
inline constexpr uint32_t kNativeBufferCacheLimitMiB = 256;
inline constexpr size_t kNativeMaximumVertexConversionsPerBuffer = 2;
inline constexpr uint32_t kNativeUploadShrinkObservationFrames = 120;

// Internal lock flushes submit GPU work but are not title presents. Keep the
// resource-maintenance clock on the most recent title frame. A real present
// with frame zero (startup, reset, or wrap) remains a real clock observation.
constexpr uint32_t NativeResourceFrameForBatch(uint32_t previous_title_frame,
                                               uint32_t submitted_frame,
                                               bool is_title_present) {
  return is_title_present ? submitted_frame : previous_title_frame;
}

// Cache eviction may drop only a reproducible image. GPU-produced textures are
// authoritative content, not disposable copies of CPU snapshots: clearing a new
// allocation cannot restore a previous resolve. Guest release/supersession still
// retires these images through the separate submission-safe release path.
constexpr bool CanDiscardNativeTextureImageContents(bool gpu_produced,
                                                      bool has_cpu_payload) {
  return !gpu_produced && has_cpu_payload;
}

constexpr bool ShouldEvictNativeTexture(uint32_t current_frame, uint32_t last_used_frame,
                                        uint32_t grace_frames) {
  const uint32_t age = current_frame >= last_used_frame ? current_frame - last_used_frame : 0u;
  return age > grace_frames;
}

// A lock/unlock notification is conservative: the guest may unlock a resource
// without changing its bytes. Once capture has proved that the complete native
// identity is unchanged, keeping the dirty bit would force every later bind to
// repeat the expensive untile/convert/hash path forever.
constexpr bool ShouldClearNativeTextureDirtyFlag(bool cache_entry_dirty,
                                                 bool complete_identity_matches) {
  return cache_entry_dirty && complete_identity_matches;
}

constexpr bool CanReuseNativeBufferCapture(bool cache_entry_present, bool cache_entry_dirty,
                                           bool metadata_matches) {
  return cache_entry_present && !cache_entry_dirty && metadata_matches;
}

struct NativeBufferShadowValidationRange {
  size_t offset = 0;
  size_t length = 0;
  size_t next_offset = 0;
  bool completes_sweep = false;
};

// Validate one contiguous part of every clean reuse rather than comparing a
// whole resource in one command-capture transaction. Consecutive ranges cover
// every payload byte, so this is a bounded sweep rather than sparse sampling.
constexpr NativeBufferShadowValidationRange GetNativeBufferShadowValidationRange(
    size_t payload_size, size_t requested_offset,
    size_t slice_count = kNativeBufferShadowValidationSlices) {
  if (!payload_size || !slice_count) {
    return {};
  }
  const size_t offset = requested_offset < payload_size ? requested_offset : 0;
  size_t slice_size = payload_size / slice_count;
  if (payload_size % slice_count) {
    ++slice_size;
  }
  if (!slice_size) {
    slice_size = 1;
  }
  const size_t remaining = payload_size - offset;
  const size_t length = slice_size < remaining ? slice_size : remaining;
  const bool completes_sweep = length == remaining;
  return {offset, length, completes_sweep ? 0 : offset + length, completes_sweep};
}

inline bool NativeBufferShadowPayloadMatches(const uint8_t* guest_payload,
                                             const uint8_t* captured_payload,
                                             size_t payload_size) {
  if (!guest_payload || !captured_payload) {
    return false;
  }
  return !std::memcmp(guest_payload, captured_payload, payload_size);
}

inline bool NativeBufferShadowPayloadRangeMatches(
    const uint8_t* guest_payload, const uint8_t* captured_payload, size_t payload_size,
    const NativeBufferShadowValidationRange& range) {
  if (!guest_payload || !captured_payload || !range.length || range.offset > payload_size ||
      range.length > payload_size - range.offset) {
    return false;
  }
  return !std::memcmp(guest_payload + range.offset, captured_payload + range.offset,
                      range.length);
}

constexpr bool ShouldDisableNativeBufferFastPath(bool validation_requested,
                                                 bool payload_matches) {
  return validation_requested && !payload_matches;
}

constexpr bool IsNativeTextureHeapUnderPressure(bool budget_available, uint64_t heap_usage,
                                                uint64_t heap_budget) {
  if (!budget_available || !heap_budget) {
    return false;
  }
  const uint64_t reserve = heap_budget / kNativeTextureBudgetReserveDivisor;
  return heap_usage >= heap_budget - reserve;
}

constexpr bool IsNativeTextureHeapAtOrAbovePercent(bool budget_available, uint64_t heap_usage,
                                                   uint64_t heap_budget, uint32_t percent) {
  if (!budget_available || !heap_budget || percent > 100) {
    return false;
  }
  return static_cast<unsigned __int128>(heap_usage) * 100 >=
         static_cast<unsigned __int128>(heap_budget) * percent;
}

constexpr bool UpdateNativeTexturePressure(bool was_under_pressure, bool budget_available,
                                           uint64_t heap_usage, uint64_t heap_budget) {
  const uint32_t threshold = was_under_pressure ? kNativeTexturePressureExitPercent
                                                : kNativeTexturePressureEnterPercent;
  return IsNativeTextureHeapAtOrAbovePercent(budget_available, heap_usage, heap_budget,
                                             threshold);
}

constexpr bool ShouldPollNativeTextureBudget(uint32_t current_frame, uint32_t last_poll_frame,
                                             uint32_t interval) {
  if (!interval || current_frame < last_poll_frame) {
    return true;
  }
  return current_frame - last_poll_frame >= interval;
}

// A relative countdown avoids uint32 frame-number overflow and lets independent
// maintenance classes start at different phases. Missed frames run the work at
// most once, then begin a fresh interval from the observation that performed it.
class NativePeriodicWorkSchedule {
 public:
  constexpr explicit NativePeriodicWorkSchedule(uint32_t initial_delay_frames)
      : initial_delay_frames_(initial_delay_frames) {}

  constexpr bool ShouldRun(uint32_t current_frame, uint32_t interval_frames) {
    if (!interval_frames) {
      initialized_ = true;
      last_observed_frame_ = current_frame;
      frames_until_due_ = 0;
      return true;
    }
    if (!initialized_ || current_frame < last_observed_frame_) {
      initialized_ = true;
      last_observed_frame_ = current_frame;
      frames_until_due_ = initial_delay_frames_;
      if (!frames_until_due_) {
        frames_until_due_ = interval_frames;
        return true;
      }
      return false;
    }
    const uint32_t elapsed_frames = current_frame - last_observed_frame_;
    last_observed_frame_ = current_frame;
    if (elapsed_frames < frames_until_due_) {
      frames_until_due_ -= elapsed_frames;
      return false;
    }
    frames_until_due_ = interval_frames;
    return true;
  }

  constexpr void Reset() {
    initialized_ = false;
    last_observed_frame_ = 0;
    frames_until_due_ = 0;
  }

 private:
  uint32_t initial_delay_frames_ = 0;
  uint32_t last_observed_frame_ = 0;
  uint32_t frames_until_due_ = 0;
  bool initialized_ = false;
};

constexpr bool ShouldEvictNativeTextureCandidate(bool referenced, bool budget_pressure,
                                                 uint32_t current_frame,
                                                 uint32_t last_used_frame,
                                                 uint32_t grace_frames) {
  if (referenced) {
    return false;
  }
  return grace_frames != 0
             ? ShouldEvictNativeTexture(current_frame, last_used_frame, grace_frames)
             : budget_pressure;
}

constexpr bool ShouldSortNativeBufferReclamation(uint64_t retained_bytes, uint64_t limit_bytes) {
  return retained_bytes > limit_bytes;
}

constexpr bool ShouldReclaimNativeBuffer(bool exclusively_cached, bool over_budget,
                                         uint32_t current_frame, uint32_t last_used_frame,
                                         uint32_t retention_frames) {
  if (!exclusively_cached) {
    return false;
  }
  return over_budget ||
         (retention_frames != 0 &&
          ShouldEvictNativeTexture(current_frame, last_used_frame, retention_frames));
}

constexpr bool ShouldShrinkNativeUploadBuffer(uint64_t current_capacity,
                                              uint64_t default_capacity,
                                              uint64_t required_capacity,
                                              uint64_t target_capacity,
                                              uint32_t underutilized_frames,
                                              uint32_t observation_frames) {
  return current_capacity > default_capacity && target_capacity <= current_capacity / 2 &&
         required_capacity <= current_capacity && observation_frames != 0 &&
         underutilized_frames >= observation_frames;
}

constexpr bool ShouldRetireSupersededNativeTextureGeneration(uint64_t previous_generation,
                                                              uint64_t replacement_generation) {
  return previous_generation && previous_generation != replacement_generation;
}

enum class NativeTextureReleaseAction : uint8_t {
  kForget,
  kKeepPending,
  kDestroyImage,
};

constexpr NativeTextureReleaseAction ClassifyNativeTextureRelease(bool referenced,
                                                                  bool image_exists) {
  if (referenced) {
    return NativeTextureReleaseAction::kKeepPending;
  }
  return image_exists ? NativeTextureReleaseAction::kDestroyImage
                      : NativeTextureReleaseAction::kForget;
}

struct NativeTextureLruKey {
  uint64_t last_use_serial = 0;
  uint64_t generation = 0;

  constexpr bool operator<(const NativeTextureLruKey& other) const {
    return last_use_serial < other.last_use_serial ||
           (last_use_serial == other.last_use_serial && generation < other.generation);
  }
};

// attempt is zero for the initial allocation and one for its sole retry.
constexpr bool ShouldRetryNativeTextureAllocation(uint32_t attempt) { return attempt == 0; }

}  // namespace rex::graphics::gta4_native

#endif  // REX_GRAPHICS_GTA4_NATIVE_NATIVE_FRAME_SCHEDULING_H_
