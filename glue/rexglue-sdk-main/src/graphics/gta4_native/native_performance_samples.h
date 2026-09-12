#ifndef REX_GRAPHICS_GTA4_NATIVE_NATIVE_PERFORMANCE_SAMPLES_H_
#define REX_GRAPHICS_GTA4_NATIVE_NATIVE_PERFORMANCE_SAMPLES_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>

namespace rex::graphics::gta4_native::performance {

// Coarse, stable ranges only. Unlike command-detail diagnostics, these ranges
// are intended to remain cheap enough for representative multi-frame captures.
enum class GpuRange : uint8_t {
  kFrame,
  kFrameSetup,
  kTexturePreparation,
  kOpaqueControl,
  kMirrorReflections,
  kWaterReflections,
  kEnvironmentReflections,
  kResolve,
  kDepthResolve,
  kStencilSave,
  kDepthCopy,
  kStencilRestore,
  kLightSetup,
  kDeferredLightVolumes,
  kRadar,
  kTranslucentWaterSurface,
  kTranslucentWaterTexture,
  kTranslucentVehicleGlass,
  kTranslucentGeneralGlass,
  kTranslucentVehicleLights,
  kTranslucentLightSprites,
  kTranslucentOther,
  kComposite,
  kSceneSnapshot,
  kPostFxStipple,
  kPostFxBokeh,
  kPostFxBlur,
  kPostFxDofCombine,
  kPostFxCopyBack,
  kSunShaftPrepass,
  kSunShaftRadialFirst,
  kSunShaftRadialSecond,
  kSunShaftComposite,
  kSunShaftCopyBack,
  kSmaaLookupUpload,
  kSmaaEdges,
  kSmaaWeights,
  kSmaaNeighborhood,
  kPresent,
  kFrameRelease,
  kUnattributed,
  kDirectionalShadowShaders,
  kLocalShadowShaders,
  kMaterialShaders,
  kSkyShaders,
  kImmediateShaders,
  kParticleShaders,
  kRetailSceneToGBuffer,kRetailLightsToScreen,kRetailDrawScene,kRetailScript2d,
  kRetailFrontendPhone,kRetailHtml,kRetailBlit,kRetailViewport,kRetailTreeImposters,
  kRetailWaterSurface,kRetailCloudGeneration,kRetailRainUpdate,kRetailWarpShadow,
  kRetailInteriorReflection,kRetailPlayerSettings,
  kCount,
};

enum class CpuRange : uint8_t {
  kFrameInterval,
  kFenceWait,
  kHousekeeping,
  kUploadCapacity,
  kCommandSetup,
  kTexturePreparation,
  kCommandRecording,
  kCommandFinalize,
  kQueueSubmit,
  kRenderCallback,
  kPublish,
  kPresenterAcquire,
  kPresenterSubmit,
  kPresenterPresent,
  kPresenterTotal,
  kOutsideRenderer,
  kSlotCleanup,
  kProfileReadback,
  kPipelineCompileJobs,
  kPipelineWait,
  // Nested components of kHousekeeping, excluding kProfileReadback. These
  // explain the parent range and must not be added to it as separate costs.
  kHousekeepingSurfaceRelease,
  kHousekeepingTextureReclamation,
  kHousekeepingTextureRetirement,
  kHousekeepingBufferReclamation,
  kHousekeepingPersistentBufferReclamation,
  kCount,
};

enum class Counter : uint8_t {
  kUploadBytes,
  kTextureUploadBytes,
  kVertexUploadBytes,
  kIndexUploadBytes,
  kPersistentBufferUploadBytes,
  kPersistentBufferHits,
  kPersistentBufferMisses,
  kPersistentBufferResidentBytes,
  kPersistentBufferLiveAllocations,
  kVertexConstantUploadBytes,
  kPixelConstantUploadBytes,
  kSharedConstantUploadBytes,
  kDrawUpUploadBytes,
  kDescriptorSetsAllocated,
  kDescriptorEntriesWritten,
  kPipelineLookups,
  kPipelineMisses,
  kPipelineCreates,
  kSurfaceImagesLive,
  kSurfaceImageBytes,
  kTextureImagesLive,
  kTextureImageBytes,
  kBufferCaptureReuses,
  kBufferShadowValidations,
  kBufferShadowMismatches,
  kBufferFastPathDisables,
  kTextureImageEvictions,
  kTextureImageEvictedBytes,
  kTextureAllocationRetries,
  kTextureAllocationFailures,
  kTextureHeapUsage,
  kTextureHeapBudget,
  kProcessPhysicalFootprintBytes,
  kProcessResidentBytes,
  kUploadBufferCapacityBytes,
  kUploadBufferAllocationBytes,
  kPendingTextureReleases,
  kPipelinesLive,
  kUnavailableGpuRanges,
  kDroppedGpuRanges,
  kCoarseGpuTiming,
  kPipelineRequestReuses,
  kIndexedDescriptorPages,
  kTextureAllocationReuses,
  kTextureAllocationPoolBytes,
  kPipelineSnapshotReuses,
  kShaderSnapshotReuses,
  kZeroDofSkips,
  kPostFxDirectWrites,
  kConstantVersionHits,
  kConstantContentHits,
  kConstantBindingUploads,
  kTextureBindingReuses,
  kConstantBindingOwners,
  kCount,
};

constexpr size_t kGpuRangeCount = size_t(GpuRange::kCount);
constexpr size_t kCpuRangeCount = size_t(CpuRange::kCount);
constexpr size_t kCounterCount = size_t(Counter::kCount);
// The profiler records one timestamp at frame start and one at every exclusive
// range boundary. This deliberately bounded pool keeps profiling overhead
// independent of draw count; each capture selects a limit within this capacity.
constexpr size_t kMaximumGpuQueriesPerFrame = 1024;
constexpr uint32_t kProfileSchemaVersion = 2;
constexpr uint32_t kFrameFlagCaptureStart = 1u << 0;
constexpr uint32_t kFrameFlagGpuPartial = 1u << 1;
constexpr uint32_t kFrameFlagInternalFlush = 1u << 2;
constexpr size_t kQueriesPerGpuSpan = 2;
constexpr size_t kMaximumGpuSpansPerFrame = kMaximumGpuQueriesPerFrame / kQueriesPerGpuSpan;
constexpr size_t kFrameSampleCapacity = 600;
constexpr size_t kFrameSampleCompletionSlotCount = 2;

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
  uint64_t capture_sequence = 0, native_submission = 0;
  uint32_t frame_slot = 0;
  std::array<uint32_t, kGpuRangeCount> gpu_unavailable_counts{};
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

  GpuSpanToken BeginGpuRange(GpuRange range, uint32_t command_index, uint32_t begin_query,
                             uint32_t end_query);
  bool EndGpuRange(GpuSpanToken token);
  bool ResolveGpuRange(GpuSpanToken token, uint64_t elapsed_ticks, bool available);
  void AddGpuRange(GpuRange range, uint64_t elapsed_ticks, bool available = true);
  const FrameSample& Finish();
  void AddCpuRange(CpuRange range, uint64_t elapsed_ticks);
  void AddCounter(Counter counter, uint64_t value = 1);
  void SetCounter(Counter counter, uint64_t value);
  void SetIdentity(uint64_t sequence, uint64_t submission, uint32_t slot) {
    sample_.capture_sequence=sequence;sample_.native_submission=submission;sample_.frame_slot=slot;
  }
  void SetFlags(uint32_t flags) { sample_.flags |= flags; }

  const FrameSample& sample() const { return sample_; }
  const std::array<GpuSpan, kMaximumGpuSpansPerFrame>& spans() const { return spans_; }
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

// Holds immutable samples after their exact frame-slot fence completes. A
// sequence may become ready in either CPU-observed slot order, but PopNext only
// publishes the requested capture sequence. This keeps export rows ordered and
// prevents a reused slot from overwriting a not-yet-published sample.
class FrameSampleCompletionQueue {
 public:
  void Clear();
  bool Complete(size_t slot, uint64_t sequence, bool publish, const FrameSample& sample);
  bool PopNext(uint64_t sequence, bool* publish, FrameSample* sample);

  bool occupied(size_t slot) const;
  size_t size() const { return size_; }

 private:
  struct CompletedSample {
    uint64_t sequence = 0;
    bool publish = false;
    FrameSample sample{};
  };

  std::array<std::optional<CompletedSample>, kFrameSampleCompletionSlotCount> slots_{};
  size_t size_ = 0;
};

const char* GpuRangeName(GpuRange range);
// Source phase IDs read from the executing retail draw-list header/task.
GpuRange PerformanceRangeForRetailPhase(uint32_t phase);
const char* CpuRangeName(CpuRange range);
const char* CounterName(Counter counter);
bool CalculateTimestampDelta(uint64_t begin, uint64_t end, uint32_t valid_bits, uint64_t* delta);

}  // namespace rex::graphics::gta4_native::performance

#endif  // REX_GRAPHICS_GTA4_NATIVE_NATIVE_PERFORMANCE_SAMPLES_H_
