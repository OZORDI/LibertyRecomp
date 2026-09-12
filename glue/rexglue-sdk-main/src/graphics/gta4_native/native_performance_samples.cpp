#include "native_performance_samples.h"

#include <algorithm>

namespace rex::graphics::gta4_native::performance {

namespace {

template <typename Enum>
constexpr size_t EnumIndex(Enum value) {
  return size_t(value);
}

}  // namespace

void FrameBuilder::Begin(uint32_t frame) {
  ++generation_;
  if (!generation_) {
    ++generation_;
  }
  sample_ = {};
  sample_.frame = frame;
  spans_ = {};
  span_count_ = 0;
  open_span_slots_ = {};
  open_span_count_ = 0;
  query_in_use_ = {};
  active_ = frame != 0;
}

void FrameBuilder::Cancel() {
  sample_ = {};
  spans_ = {};
  span_count_ = 0;
  open_span_slots_ = {};
  open_span_count_ = 0;
  query_in_use_ = {};
  active_ = false;
}

GpuSpanToken FrameBuilder::BeginGpuRange(GpuRange range, uint32_t command_index,
                                         uint32_t begin_query, uint32_t end_query) {
  if (!active_ || EnumIndex(range) >= kGpuRangeCount || span_count_ >= spans_.size() ||
      begin_query == UINT32_MAX || end_query == UINT32_MAX ||
      begin_query >= kMaximumGpuQueriesPerFrame || end_query >= kMaximumGpuQueriesPerFrame ||
      begin_query == end_query || query_in_use_[begin_query] || query_in_use_[end_query] ||
      open_span_count_ >= open_span_slots_.size()) {
    if (active_) {
      AddCounter(Counter::kDroppedGpuRanges);
    }
    return {};
  }

  const size_t slot = span_count_++;
  GpuSpan& span = spans_[slot];
  span.range = range;
  span.command_index = command_index;
  span.begin_query = begin_query;
  span.end_query = end_query;
  span.generation = generation_;
  span.ended = false;
  span.resolved = false;
  open_span_slots_[open_span_count_++] = uint16_t(slot);
  query_in_use_[begin_query] = true;
  query_in_use_[end_query] = true;
  return GpuSpanToken{uint16_t(slot), 0, generation_};
}

bool FrameBuilder::EndGpuRange(GpuSpanToken token) {
  if (!active_ || !token || token.generation != generation_ || size_t(token.slot) >= span_count_) {
    return false;
  }
  if (!open_span_count_ || open_span_slots_[open_span_count_ - 1] != token.slot) {
    return false;
  }
  GpuSpan& span = spans_[token.slot];
  if (span.generation != token.generation || span.ended) {
    return false;
  }
  span.ended = true;
  --open_span_count_;
  return true;
}

bool FrameBuilder::ResolveGpuRange(GpuSpanToken token, uint64_t elapsed_ticks, bool available) {
  if (!active_ || !token || token.generation != generation_ || size_t(token.slot) >= span_count_) {
    return false;
  }
  GpuSpan& span = spans_[token.slot];
  if (span.generation != token.generation || !span.ended || span.resolved) {
    return false;
  }
  span.resolved = true;
  if (!available) {
    AddCounter(Counter::kUnavailableGpuRanges);
    ++sample_.gpu_unavailable_counts[EnumIndex(span.range)];
    return true;
  }
  const size_t index = EnumIndex(span.range);
  sample_.gpu_ticks[index] += elapsed_ticks;
  ++sample_.gpu_range_counts[index];
  return true;
}

void FrameBuilder::AddGpuRange(GpuRange range, uint64_t elapsed_ticks, bool available) {
  if (!active_ || EnumIndex(range) >= kGpuRangeCount) {
    return;
  }
  if (!available) {
    AddCounter(Counter::kUnavailableGpuRanges);
    ++sample_.gpu_unavailable_counts[EnumIndex(range)];
    return;
  }
  const size_t index = EnumIndex(range);
  sample_.gpu_ticks[index] += elapsed_ticks;
  ++sample_.gpu_range_counts[index];
}

const FrameSample& FrameBuilder::Finish() {
  if (!active_) {
    return sample_;
  }
  for (size_t index = 0; index < span_count_; ++index) {
    const GpuSpan& span = spans_[index];
    if (!span.ended || !span.resolved) {
      AddCounter(Counter::kDroppedGpuRanges);
    }
  }
  active_ = false;
  return sample_;
}

void FrameBuilder::AddCpuRange(CpuRange range, uint64_t elapsed_ticks) {
  if (!active_ || EnumIndex(range) >= kCpuRangeCount) {
    return;
  }
  const size_t index = EnumIndex(range);
  sample_.cpu_ticks[index] += elapsed_ticks;
  ++sample_.cpu_range_counts[index];
}

void FrameBuilder::AddCounter(Counter counter, uint64_t value) {
  if (!active_ || EnumIndex(counter) >= kCounterCount) {
    return;
  }
  sample_.counters[EnumIndex(counter)] += value;
}

void FrameBuilder::SetCounter(Counter counter, uint64_t value) {
  if (!active_ || EnumIndex(counter) >= kCounterCount) {
    return;
  }
  sample_.counters[EnumIndex(counter)] = value;
}

void FrameSampleRing::Clear() {
  // Clearing the logical contents is sufficient: CopyOldest/CopyNewest reject
  // every slot while size_ is zero, and Push overwrites a slot before exposing
  // it again. Assigning an empty std::array here makes Clang materialize a
  // roughly 574 KiB zero-filled temporary on the render worker stack. The
  // worker's guard page can be reached while zeroing that temporary, which made
  // manually arming a capture fault in __bzero before the first sampled frame.
  head_ = 0;
  size_ = 0;
}

void FrameSampleRing::Push(const FrameSample& sample) {
  samples_[head_] = sample;
  head_ = (head_ + 1) % samples_.size();
  size_ = std::min(size_ + 1, samples_.size());
}

bool FrameSampleRing::CopyOldest(size_t offset, FrameSample* sample) const {
  if (!sample || offset >= size_) {
    return false;
  }
  const size_t oldest = (head_ + samples_.size() - size_) % samples_.size();
  *sample = samples_[(oldest + offset) % samples_.size()];
  return true;
}

bool FrameSampleRing::CopyNewest(size_t offset, FrameSample* sample) const {
  if (!sample || offset >= size_) {
    return false;
  }
  const size_t newest = (head_ + samples_.size() - 1) % samples_.size();
  *sample = samples_[(newest + samples_.size() - offset) % samples_.size()];
  return true;
}

void FrameSampleCompletionQueue::Clear() {
  slots_ = {};
  size_ = 0;
}

bool FrameSampleCompletionQueue::Complete(size_t slot, uint64_t sequence, bool publish,
                                          const FrameSample& sample) {
  if (slot >= slots_.size() || !sequence || slots_[slot]) {
    return false;
  }
  for (const auto& completed : slots_) {
    if (completed && completed->sequence == sequence) {
      return false;
    }
  }
  slots_[slot] = CompletedSample{sequence, publish, sample};
  ++size_;
  return true;
}

bool FrameSampleCompletionQueue::PopNext(uint64_t sequence, bool* publish, FrameSample* sample) {
  if (!sequence || !publish || !sample) {
    return false;
  }
  for (auto& completed : slots_) {
    if (!completed || completed->sequence != sequence) {
      continue;
    }
    *publish = completed->publish;
    *sample = completed->sample;
    completed.reset();
    --size_;
    return true;
  }
  return false;
}

bool FrameSampleCompletionQueue::occupied(size_t slot) const {
  return slot < slots_.size() && slots_[slot].has_value();
}

GpuRange PerformanceRangeForRetailPhase(uint32_t phase) {
  switch(phase){
    case 1:return GpuRange::kRetailLightsToScreen;case 2:return GpuRange::kRadar;
    case 3:return GpuRange::kRetailBlit;
    case 6:case 9:case 10:return GpuRange::kRetailViewport;
    case 13:return GpuRange::kRetailScript2d;case 14:return GpuRange::kRetailTreeImposters;
    case 15:return GpuRange::kRetailDrawScene;case 16:return GpuRange::kRetailWaterSurface;
    case 17:return GpuRange::kWaterReflections;case 19:return GpuRange::kEnvironmentReflections;
    case 20:return GpuRange::kRetailInteriorReflection;case 21:return GpuRange::kRetailWarpShadow;
    case 23:return GpuRange::kRetailFrontendPhone;case 24:return GpuRange::kRetailHtml;
    case 31:return GpuRange::kRetailSceneToGBuffer;case 32:return GpuRange::kRetailCloudGeneration;
    case 34:return GpuRange::kRetailPlayerSettings;case 35:return GpuRange::kRetailRainUpdate;
    case 36:return GpuRange::kMirrorReflections;default:return GpuRange::kUnattributed;
  }
}

const char* GpuRangeName(GpuRange range) {
  static constexpr std::array<const char*, kGpuRangeCount> kNames = {
      "frame",
      "frame-setup",
      "texture-preparation",
      "opaque-control",
      "mirror-reflections",
      "water-reflections",
      "environment-reflections",
      "resolve",
      "depth-resolve",
      "stencil-save",
      "depth-copy",
      "stencil-restore",
      "light-setup",
      "deferred-light-volumes",
      "radar",
      "translucent-water-surface",
      "translucent-water-texture",
      "translucent-vehicle-glass",
      "translucent-general-glass",
      "translucent-vehicle-lights",
      "translucent-light-sprites",
      "translucent-other",
      "composite",
      "scene-snapshot",
      "postfx-stipple",
      "postfx-bokeh",
      "postfx-blur",
      "postfx-dof-combine",
      "postfx-copy-back",
      "sun-shaft-prepass",
      "sun-shaft-radial-first",
      "sun-shaft-radial-second",
      "sun-shaft-composite",
      "sun-shaft-copy-back",
      "smaa-lookup-upload",
      "smaa-edges",
      "smaa-weights",
      "smaa-neighborhood",
      "present",
      "frame-release",
      "unattributed", "directional-shadow-shaders", "local-shadow-shaders", "material-shaders",
      "sky-shaders", "immediate-shaders", "particle-shaders",
      "retail-scene-to-gbuffer","retail-lights-to-screen","retail-draw-scene","retail-script-2d",
      "retail-frontend-or-phone-model","retail-html","retail-blit","retail-viewport",
      "retail-tree-imposters","retail-water-surface","retail-cloud-generation","retail-rain-update",
      "retail-warp-shadow","retail-interior-reflection","retail-player-settings",
  };
  const size_t index = EnumIndex(range);
  return index < kNames.size() ? kNames[index] : "unknown";
}

const char* CpuRangeName(CpuRange range) {
  static constexpr std::array<const char*, kCpuRangeCount> kNames = {
      "frame-interval",   "fence-wait",       "housekeeping",    "upload-capacity",
      "command-setup",   "texture-preparation", "command-recording", "command-finalize",
      "queue-submit",      "render-callback", "publish",       "presenter-acquire",
      "presenter-submit",  "presenter-present", "presenter-total", "outside-renderer",
      "slot-cleanup", "profile-readback",
      "pipeline-compile-jobs", "pipeline-wait",
      "housekeeping-surface-release", "housekeeping-texture-reclamation",
      "housekeeping-texture-retirement", "housekeeping-buffer-reclamation",
      "housekeeping-persistent-buffer-reclamation",
  };
  const size_t index = EnumIndex(range);
  return index < kNames.size() ? kNames[index] : "unknown";
}

const char* CounterName(Counter counter) {
  static constexpr std::array<const char*, kCounterCount> kNames = {
      "upload-bytes",
      "texture-upload-bytes",
      "vertex-upload-bytes",
      "index-upload-bytes",
      "persistent-buffer-upload-bytes",
      "persistent-buffer-hits",
      "persistent-buffer-misses",
      "persistent-buffer-resident-bytes",
      "persistent-buffer-live-allocations",
      "vertex-constant-upload-bytes",
      "pixel-constant-upload-bytes",
      "shared-constant-upload-bytes",
      "draw-up-upload-bytes",
      "descriptor-sets-allocated",
      "descriptor-entries-written",
      "pipeline-lookups",
      "pipeline-misses",
      "pipeline-creates",
      "surface-images-live",
      "surface-image-bytes",
      "texture-images-live",
      "texture-image-bytes",
      "buffer-capture-reuses",
      "buffer-shadow-validations",
      "buffer-shadow-mismatches",
      "buffer-fast-path-disables",
      "texture-image-evictions",
      "texture-image-evicted-bytes",
      "texture-allocation-retries",
      "texture-allocation-failures",
      "texture-heap-usage",
      "texture-heap-budget",
      "process-physical-footprint-bytes",
      "process-resident-bytes",
      "upload-buffer-capacity-bytes",
      "upload-buffer-allocation-bytes",
      "pending-texture-releases",
      "pipelines-live",
      "unavailable-gpu-ranges",
      "dropped-gpu-ranges",
      "coarse-gpu-timing",
      "pipeline-request-reuses",
      "indexed-descriptor-pages",
      "texture-allocation-reuses",
      "texture-allocation-pool-bytes",
      "pipeline-snapshot-reuses",
      "shader-snapshot-reuses",
      "zero-dof-skips",
      "postfx-direct-writes", "constant-version-hits", "constant-content-hits", "constant-binding-uploads", "texture-binding-reuses", "constant-binding-owners",
  };
  const size_t index = EnumIndex(counter);
  return index < kNames.size() ? kNames[index] : "unknown";
}

bool CalculateTimestampDelta(uint64_t begin, uint64_t end, uint32_t valid_bits, uint64_t* delta) {
  if (!delta || !valid_bits || valid_bits > 64) {
    return false;
  }
  const uint64_t mask = valid_bits == 64 ? UINT64_MAX : (uint64_t(1) << valid_bits) - 1;
  *delta = ((end & mask) - (begin & mask)) & mask;
  return true;
}

}  // namespace rex::graphics::gta4_native::performance
