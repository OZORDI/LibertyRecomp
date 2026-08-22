#ifndef REX_GRAPHICS_GTA4_NATIVE_NATIVE_PERFORMANCE_SAMPLES_H_
#define REX_GRAPHICS_GTA4_NATIVE_NATIVE_PERFORMANCE_SAMPLES_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace rex::graphics::gta4_native::performance {

// Coarse, stable ranges only. Unlike command-detail diagnostics, these ranges
// are intended to remain cheap enough for representative multi-frame captures.
enum class GpuRange : uint8_t {
  kFrame,
  kTexturePreparation,
  kOpaqueControl,
  kMirrorReflections,
  kWaterReflections,
  kEnvironmentReflections,
  kDepthResolve,
  kStencilSave,
  kDepthCopy,
  kStencilRestore,
  kDeferredLightVolumes,
  kTranslucentWaterSurface,
  kTranslucentWaterTexture,
  kTranslucentVehicleGlass,
  kTranslucentGeneralGlass,
  kTranslucentVehicleLights,
  kTranslucentLightSprites,
  kComposite,
  kPresent,
  kCount,
};

enum class CpuRange : uint8_t {
  kFenceWait,
  kTexturePreparation,
  kCommandRecording,
  kQueueSubmit,
  kRenderCallback,
  kCount,
};

enum class Counter : uint8_t {
  kUploadBytes,
  kUnavailableGpuRanges,
  kDroppedGpuRanges,
  kCount,
};

constexpr size_t kGpuRangeCount = size_t(GpuRange::kCount);
constexpr size_t kCpuRangeCount = size_t(CpuRange::kCount);
constexpr size_t kCounterCount = size_t(Counter::kCount);
// Matches the native timestamp pool budget. Every span owns exactly two
// queries, so the metadata store can represent every legal query-pool pair.
constexpr size_t kMaximumGpuQueriesPerFrame = 4096;
constexpr size_t kQueriesPerGpuSpan = 2;
constexpr size_t kMaximumGpuSpansPerFrame =
    kMaximumGpuQueriesPerFrame / kQueriesPerGpuSpan;
constexpr size_t kFrameSampleCapacity = 256;

struct GpuSpan {
  GpuRange range = GpuRange::kFrame;
  uint32_t command_index = UINT32_MAX;
  uint32_t begin_query = UINT32_MAX;
  uint32_t end_query = UINT32_MAX;
  uint32_t generation = 0;
  bool ended = false;
  bool resolved = false;
};

struct GpuSpanToken {
  uint16_t slot = UINT16_MAX;
  uint16_t reserved = 0;
  uint32_t generation = 0;

  explicit operator bool() const { return slot != UINT16_MAX; }
};

struct FrameSample {
  uint32_t frame = 0;
  uint32_t flags = 0;
  std::array<uint64_t, kGpuRangeCount> gpu_ticks{};
  std::array<uint32_t, kGpuRangeCount> gpu_range_counts{};
  std::array<uint64_t, kCpuRangeCount> cpu_ticks{};
  std::array<uint32_t, kCpuRangeCount> cpu_range_counts{};
  std::array<uint64_t, kCounterCount> counters{};
};

static_assert(std::is_trivially_copyable_v<GpuSpan>);
static_assert(std::is_trivially_copyable_v<GpuSpanToken>);
static_assert(std::is_trivially_copyable_v<FrameSample>);

class FrameBuilder {
 public:
  void Begin(uint32_t frame);
  void Cancel();
  bool active() const { return active_; }
  uint32_t frame() const { return sample_.frame; }

  GpuSpanToken BeginGpuRange(GpuRange range, uint32_t command_index,
                             uint32_t begin_query, uint32_t end_query);
  bool EndGpuRange(GpuSpanToken token);
  bool ResolveGpuRange(GpuSpanToken token, uint64_t elapsed_ticks,
                       bool available);
  const FrameSample& Finish();
  void AddCpuRange(CpuRange range, uint64_t elapsed_ticks);
  void AddCounter(Counter counter, uint64_t value = 1);

  const FrameSample& sample() const { return sample_; }
  const std::array<GpuSpan, kMaximumGpuSpansPerFrame>& spans() const {
    return spans_;
  }
  size_t span_count() const { return span_count_; }

 private:
  FrameSample sample_{};
  std::array<GpuSpan, kMaximumGpuSpansPerFrame> spans_{};
  size_t span_count_ = 0;
  std::array<uint16_t, kMaximumGpuSpansPerFrame> open_span_slots_{};
  size_t open_span_count_ = 0;
  std::array<bool, kMaximumGpuQueriesPerFrame> query_in_use_{};
  uint32_t generation_ = 0;
  bool active_ = false;
};

// Single-thread owned. Exporters copy values on the owner thread; no internal
// sample address is exposed across thread boundaries.
class FrameSampleRing {
 public:
  void Clear();
  void Push(const FrameSample& sample);

  size_t size() const { return size_; }
  constexpr size_t capacity() const { return samples_.size(); }
  bool empty() const { return size_ == 0; }
  bool CopyOldest(size_t offset, FrameSample* sample) const;
  bool CopyNewest(size_t offset, FrameSample* sample) const;

 private:
  std::array<FrameSample, kFrameSampleCapacity> samples_{};
  size_t head_ = 0;
  size_t size_ = 0;
};

const char* GpuRangeName(GpuRange range);
const char* CpuRangeName(CpuRange range);
const char* CounterName(Counter counter);
bool CalculateTimestampDelta(uint64_t begin, uint64_t end,
                             uint32_t valid_bits, uint64_t* delta);

}  // namespace rex::graphics::gta4_native::performance

#endif  // REX_GRAPHICS_GTA4_NATIVE_NATIVE_PERFORMANCE_SAMPLES_H_
