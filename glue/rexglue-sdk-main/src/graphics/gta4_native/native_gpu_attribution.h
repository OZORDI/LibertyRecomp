#ifndef REX_GRAPHICS_GTA4_NATIVE_NATIVE_GPU_ATTRIBUTION_H_
#define REX_GRAPHICS_GTA4_NATIVE_NATIVE_GPU_ATTRIBUTION_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <type_traits>
#include <vector>
#include <rex/graphics/gta4_native/gpu_pass_origin.h>

namespace rex::graphics::gta4_native::attribution {

// Attribution borrows a bounded set of optional endpoints from the caller's
// per-frame query budget. Coarse transitions and the closing frame timestamp
// always reserve their own space before detail is admitted.
constexpr uint32_t kRegularDetailBoundaryBudget = 48;
constexpr uint32_t kDrilldownBoundaryBudget = 16;
constexpr uint32_t kMaximumExtraDetailBoundaries =
    kRegularDetailBoundaryBudget + kDrilldownBoundaryBudget;
constexpr uint32_t kFrameEndpointQueryCount = 2;
constexpr uint32_t kHardDetailBoundaryLimit = 512;
constexpr size_t kMaximumPendingDrilldownTargets = 4;
constexpr size_t kAttributionCompletionSlotCount = 2;
constexpr uint32_t kDrilldownExpiryFrames = 8;
// Python verification: 2 milliseconds * 1,000,000 nanoseconds/millisecond.
constexpr uint64_t kSlowUnknownRegionNanoseconds = 2'000'000;

enum class NativePassCommandClass : uint8_t {
  kDraw,
  kDrawUp,
  kDrawIndexed,
  kResolve,
  kClear,
  kDepthHandoff,
  kMixed,
};

enum class NativePassClassification : uint8_t {
  kKnown,
  kOther,
  kUnknown,
};

enum class NativeDepthBlendClass : uint8_t {
  kOpaque,
  kBlended,
  kDepthOnlyRead,
  kDepthOnlyWrite,
  kTransfer,
  kNoAttachments,
  kMixed,
};

enum class NativePassRegionDetail : uint8_t {
  kGrouped,
  kCoalesced,
  kDrilldown,
};

struct NativePassSurfaceKey {
  uint32_t handle = 0;
  uint64_t generation = 0;
  uint32_t format = 0;
  uint32_t sample_count = 0;

  bool operator==(const NativePassSurfaceKey&) const = default;
};

struct NativePassKey {
  NativePassCommandClass command_class = NativePassCommandClass::kMixed;
  std::array<NativePassSurfaceKey, 4> color_targets{};
  NativePassSurfaceKey depth_target{};
  uint32_t reflection_family = UINT32_MAX;
  uint32_t render_phase = 0;
  GpuPassOrigin origin{};
  uint64_t shader_family = 0;
  uint64_t vertex_shader_hash = 0;
  uint64_t pixel_shader_hash = 0;
  NativeDepthBlendClass depth_blend_class = NativeDepthBlendClass::kNoAttachments;

  bool operator==(const NativePassKey&) const = default;
};

static_assert(std::is_trivially_copyable_v<NativePassSurfaceKey>);
static_assert(std::is_trivially_copyable_v<NativePassKey>);

struct NativePassKeyInput {
  NativePassCommandClass command_class = NativePassCommandClass::kMixed;
  std::array<NativePassSurfaceKey, 4> color_targets{};
  NativePassSurfaceKey depth_target{};
  uint32_t reflection_family = UINT32_MAX;
  uint32_t render_phase = 0;
  GpuPassOrigin origin{};
  uint64_t shader_family = 0;
  uint64_t vertex_shader_hash = 0;
  uint64_t pixel_shader_hash = 0;
  uint32_t color_write_mask = 0;
  bool has_color_target = false;
  bool has_pixel_shader = false;
  bool depth_enabled = false;
  bool depth_write_enabled = false;
  bool blend_enabled = false;
};

NativeDepthBlendClass ClassifyNativeDepthBlend(const NativePassKeyInput& input);
NativePassKey BuildNativePassKey(const NativePassKeyInput& input);
bool NativePassKeyLess(const NativePassKey& left, const NativePassKey& right);

// Returns zero for an unknown topology. The numeric values are the stable
// Xenos PrimitiveType ABI values, so the helper stays independent of Vulkan
// and of renderer-owned command objects.
uint64_t EstimateNativePrimitiveCount(uint32_t primitive_type, uint32_t element_count);

struct NativePassObservation {
  uint32_t command_index = UINT32_MAX;
  uint32_t coarse_range = UINT32_MAX;
  NativePassKey key{};
  NativePassClassification classification = NativePassClassification::kUnknown;
  uint32_t draw_count = 0;
  uint64_t primitive_count = 0;
};

static_assert(std::is_trivially_copyable_v<NativePassObservation>);

struct NativePassGroup {
  size_t observation_begin = 0;
  size_t observation_end = 0;
  uint32_t command_start = UINT32_MAX;
  uint32_t command_end = UINT32_MAX;
  uint32_t coarse_range = UINT32_MAX;
  NativePassKey key{};
  NativePassClassification classification = NativePassClassification::kUnknown;
  uint64_t draw_count = 0;
  uint64_t primitive_count = 0;
};

std::vector<NativePassGroup> GroupNativePassObservations(
    std::span<const NativePassObservation> observations);

struct NativeDrilldownTarget {
  uint64_t target_id = 0;
  NativePassKey key{};
};

struct NativePassRegion {
  size_t observation_begin = 0;
  size_t observation_end = 0;
  uint32_t command_start = UINT32_MAX;
  uint32_t command_end = UINT32_MAX;
  uint32_t coarse_range = UINT32_MAX;
  NativePassKey key{};
  NativePassClassification classification = NativePassClassification::kUnknown;
  NativePassRegionDetail detail = NativePassRegionDetail::kGrouped;
  uint32_t pass_key_count = 0;
  uint64_t draw_count = 0;
  uint64_t primitive_count = 0;
};

struct NativeAttributionPlanBudget {
  uint32_t regular_detail_boundaries = kRegularDetailBoundaryBudget;
  uint32_t drilldown_boundaries = kDrilldownBoundaryBudget;
  uint32_t total_detail_boundaries = kMaximumExtraDetailBoundaries;
  // Explicitly authorized by CalculateNativeAttributionQueryBudget. Defaults
  // retain the original legacy contract; hostile callers still have a cap.
  uint32_t hard_detail_boundaries = kMaximumExtraDetailBoundaries;
};

struct NativeAttributionQueryBudgetInput {
  uint32_t query_capacity = 0;
  // At the start of an isolated frame this is the frame-begin timestamp. The
  // renderer passes its actual query count so already-recorded setup ranges
  // are reserved as well.
  uint32_t queries_already_used = 1;
  uint32_t remaining_frame_endpoint_queries = 1;
  uint32_t current_coarse_range = UINT32_MAX;
  uint32_t maximum_detail_boundaries = kMaximumExtraDetailBoundaries;
};

struct NativeAttributionQueryBudget {
  NativeAttributionPlanBudget plan_budget{};
  uint32_t mandatory_coarse_boundaries = 0;
  uint32_t mandatory_attribution_boundaries = 0;
  uint32_t available_detail_boundaries = 0;
  uint32_t maximum_detail_boundaries = 0;
  uint64_t required_query_count = 0;
  uint64_t maximum_planned_query_count = 0;
  bool mandatory_boundaries_fit = false;
};

NativeAttributionQueryBudget CalculateNativeAttributionQueryBudget(
    std::span<const NativePassObservation> observations,
    const NativeAttributionQueryBudgetInput& input);

struct NativeAttributionPlan {
  std::vector<NativePassRegion> regions;
  std::vector<uint32_t> region_by_observation;
  uint32_t regular_detail_boundaries = 0;
  uint32_t drilldown_boundaries = 0;
  uint64_t target_id = 0;
  bool target_matched = false;
};

NativeAttributionPlan BuildNativeAttributionPlan(
    std::span<const NativePassObservation> observations,
    const NativeAttributionPlanBudget& budget = {},
    const std::optional<NativeDrilldownTarget>& drilldown_target = std::nullopt);

struct NativeAttributionCoverage {
  bool complete = false;
  size_t observation_count = 0;
  uint64_t source_draw_count = 0;
  uint64_t planned_draw_count = 0;
  uint64_t source_primitive_count = 0;
  uint64_t planned_primitive_count = 0;
};

NativeAttributionCoverage CheckNativeAttributionCoverage(
    std::span<const NativePassObservation> observations, const NativeAttributionPlan& plan);

struct NativeResolvedPassRegion {
  NativePassRegion region{};
  uint64_t elapsed_ticks = 0;
  uint64_t elapsed_nanoseconds = 0;
  bool available = false;
};

struct NativeAttributionAccounting {
  uint64_t flat_envelope_ticks = 0;
  uint64_t detail_ticks = 0;
  // Detail is diagnostic-only. This is deliberately the flat envelope, not
  // flat_envelope_ticks + detail_ticks.
  uint64_t exported_total_ticks = 0;
};

NativeAttributionAccounting CalculateNativeAttributionAccounting(
    uint64_t flat_envelope_ticks, std::span<const NativeResolvedPassRegion> detail_regions);

struct NativeDrilldownReservation {
  uint64_t target_id = 0;
  uint64_t reservation_id = 0;
  uint32_t expires_after_frame = 0;
  NativePassKey key{};

  explicit operator bool() const { return target_id != 0; }
};

class NativeDrilldownScheduler {
 public:
  NativeDrilldownScheduler() = default;
  explicit NativeDrilldownScheduler(uint64_t slow_region_nanoseconds,
                                    uint32_t expiry_frames = kDrilldownExpiryFrames);

  void Clear();
  void ObserveCompleted(uint32_t frame, uint64_t sequence,
                        std::span<const NativeResolvedPassRegion> regions);
  std::optional<NativeDrilldownReservation> Acquire(uint32_t frame, uint64_t reservation_id);
  bool Complete(uint64_t target_id, uint64_t reservation_id, uint32_t completed_frame,
                bool matched);
  void Expire(uint32_t frame);

  size_t pending_count() const;
  size_t in_flight_count() const;

 private:
  struct PendingTarget {
    uint64_t target_id = 0;
    NativePassKey key{};
    NativePassClassification classification = NativePassClassification::kUnknown;
    uint64_t priority_nanoseconds = 0;
    uint64_t source_sequence = 0;
    uint32_t source_frame = 0;
    uint32_t source_command = UINT32_MAX;
    uint32_t expires_after_frame = 0;
    uint64_t reservation_id = 0;
  };

  uint64_t slow_region_nanoseconds_ = kSlowUnknownRegionNanoseconds;
  uint32_t expiry_frames_ = kDrilldownExpiryFrames;
  uint64_t next_target_id_ = 1;
  std::array<std::optional<PendingTarget>, kMaximumPendingDrilldownTargets> pending_{};
};

struct NativeCompletedAttributionFrame {
  uint32_t frame = 0;
  uint64_t sequence = 0;
  bool publish = false;
  uint64_t target_id = 0;
  uint64_t reservation_id = 0;
  bool target_matched = false;
  std::vector<NativeResolvedPassRegion> regions;
};

class NativeAttributionCompletionQueue {
 public:
  void Clear();
  bool Complete(size_t slot, NativeCompletedAttributionFrame frame);
  bool PopNext(uint64_t sequence, NativeCompletedAttributionFrame* frame);

  bool occupied(size_t slot) const;
  size_t size() const { return size_; }

 private:
  std::array<std::optional<NativeCompletedAttributionFrame>, kAttributionCompletionSlotCount>
      slots_{};
  size_t size_ = 0;
};

const char* NativePassCommandClassName(NativePassCommandClass command_class);
const char* NativePassClassificationName(NativePassClassification classification);
const char* NativeDepthBlendClassName(NativeDepthBlendClass depth_blend_class);
const char* NativePassRegionDetailName(NativePassRegionDetail detail);

}  // namespace rex::graphics::gta4_native::attribution

#endif  // REX_GRAPHICS_GTA4_NATIVE_NATIVE_GPU_ATTRIBUTION_H_
