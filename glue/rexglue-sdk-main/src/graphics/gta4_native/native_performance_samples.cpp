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

GpuSpanToken FrameBuilder::BeginGpuRange(GpuRange range,
                                         uint32_t command_index,
                                         uint32_t begin_query,
                                         uint32_t end_query) {
  if (!active_ || EnumIndex(range) >= kGpuRangeCount ||
      span_count_ >= spans_.size() || begin_query == UINT32_MAX ||
      end_query == UINT32_MAX ||
      begin_query >= kMaximumGpuQueriesPerFrame ||
      end_query >= kMaximumGpuQueriesPerFrame ||
      begin_query == end_query || query_in_use_[begin_query] ||
      query_in_use_[end_query] ||
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
  if (!active_ || !token || token.generation != generation_ ||
      size_t(token.slot) >= span_count_) {
    return false;
  }
  if (!open_span_count_ ||
      open_span_slots_[open_span_count_ - 1] != token.slot) {
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

bool FrameBuilder::ResolveGpuRange(GpuSpanToken token,
                                   uint64_t elapsed_ticks,
                                   bool available) {
  if (!active_ || !token || token.generation != generation_ ||
      size_t(token.slot) >= span_count_) {
    return false;
  }
  GpuSpan& span = spans_[token.slot];
  if (span.generation != token.generation || !span.ended || span.resolved) {
    return false;
  }
  span.resolved = true;
  if (!available) {
    AddCounter(Counter::kUnavailableGpuRanges);
    return true;
  }
  const size_t index = EnumIndex(span.range);
  sample_.gpu_ticks[index] += elapsed_ticks;
  ++sample_.gpu_range_counts[index];
  return true;
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

void FrameSampleRing::Clear() {
  samples_ = {};
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

const char* GpuRangeName(GpuRange range) {
  static constexpr std::array<const char*, kGpuRangeCount> kNames = {
      "frame",
      "texture-preparation",
      "opaque-control",
      "mirror-reflections",
      "water-reflections",
      "environment-reflections",
      "depth-resolve",
      "stencil-save",
      "depth-copy",
      "stencil-restore",
      "deferred-light-volumes",
      "translucent-water-surface",
      "translucent-water-texture",
      "translucent-vehicle-glass",
      "translucent-general-glass",
      "translucent-vehicle-lights",
      "translucent-light-sprites",
      "composite",
      "present",
  };
  const size_t index = EnumIndex(range);
  return index < kNames.size() ? kNames[index] : "unknown";
}

const char* CpuRangeName(CpuRange range) {
  static constexpr std::array<const char*, kCpuRangeCount> kNames = {
      "fence-wait",
      "texture-preparation",
      "command-recording",
      "queue-submit",
      "render-callback",
  };
  const size_t index = EnumIndex(range);
  return index < kNames.size() ? kNames[index] : "unknown";
}

const char* CounterName(Counter counter) {
  static constexpr std::array<const char*, kCounterCount> kNames = {
      "upload-bytes",
      "unavailable-gpu-ranges",
      "dropped-gpu-ranges",
  };
  const size_t index = EnumIndex(counter);
  return index < kNames.size() ? kNames[index] : "unknown";
}

bool CalculateTimestampDelta(uint64_t begin, uint64_t end,
                             uint32_t valid_bits, uint64_t* delta) {
  if (!delta || !valid_bits || valid_bits > 64) {
    return false;
  }
  const uint64_t mask =
      valid_bits == 64 ? UINT64_MAX : (uint64_t(1) << valid_bits) - 1;
  *delta = ((end & mask) - (begin & mask)) & mask;
  return true;
}

}  // namespace rex::graphics::gta4_native::performance
