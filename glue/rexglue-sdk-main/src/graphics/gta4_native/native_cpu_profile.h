#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>
#include <vector>

namespace rex::graphics::gta4_native::profile {

// All ticks use the host monotonic clock. These are elapsed CPU-thread spans,
// not sampled on-core execution time. Self time excludes instrumented children
// on this same thread; it never subtracts concurrent GPU or producer work.
#define NATIVE_CPU_PHASES(X)                                                          \
  X(Publish, "publish")                                                               \
  X(Completion, "completion") X(Housekeeping, "housekeeping") X(Capacity, "capacity") \
      X(Setup, "command-setup") X(Textures, "texture-preparation")                    \
          X(Recording, "command-recording") X(Finalize, "command-finalize")           \
              X(Submit, "submission") X(Cleanup, "cleanup")
#define NATIVE_CPU_OPERATIONS(X)                                                                     \
  X(PublishFrame, "publish-frame")                                                                   \
  X(RefreshOutput, "refresh-output") X(Callback, "render-callback") X(                               \
      CompleteSlot,                                                                                  \
      "complete-frame-slot") X(FenceWait,                                                            \
                               "fence-wait") X(TextureRelease,                                       \
                                               "texture-logical-release") X(TextureEviction,         \
                                                                            "texture-eviction-"      \
                                                                            "scan")                  \
      X(TextureDestruction, "texture-destruction") X(RetiredTextures, "texture-retirement-drain") X( \
          SurfaceRelease,                                                                            \
          "surface-release") X(BufferReclamation,                                                    \
                               "buffer-reclamation") X(PersistentReclamation,                        \
                                                       "persistent-buffer-retirement")               \
          X(ProtectedResources, "protected-resource-scan") X(UploadCapacity, "upload-capacity") X(   \
              ConstantCapacity,                                                                      \
              "constant-arena-capacity") X(TexturePreparation,                                       \
                                           "prepare-textures") X(TextureLookup,                      \
                                                                 "texture-image-lookup")             \
              X(TextureAllocate, "texture-image-allocation") X(SurfaceLookup, "surface-image-"       \
                                                                              "lookup") X(           \
                  DescriptorPool,                                                                    \
                  "descriptor-pool-preparation") X(DescriptorWorkingSet,                             \
                                                   "descriptor-working-set") X(DescriptorPublish,    \
                                                                               "descriptor-"         \
                                                                               "publication")        \
                  X(DescriptorInvalidate, "descriptor-invalidation") X(                              \
                      SamplerLookup, "sampler-lookup") X(MipView,                                    \
                                                         "mip-view-lookup") X(RecordFrame,           \
                                                                              "record-frame")        \
                      X(RecordCommand, "record-command") X(Primitive, "record-primitive") X(         \
                          PrimitiveUp,                                                               \
                          "record-primitive-up") X(Indexed, "record-indexed") X(Clear,               \
                                                                                "record-clear")      \
                          X(Resolve, "record-resolve") X(DepthHandoff, "depth-stencil-handoff") X(   \
                              PresentCopy,                                                           \
                              "present-copy") X(TargetRealization,                                   \
                                                "target-realization") X(PipelineLookup,              \
                                                                        "pipeline-lookup")           \
                              X(PipelineBuild, "pipeline-key-and-build") X(                          \
                                  PipelineWait,                                                      \
                                  "pipeline-wait") X(CommonBindings,                                 \
                                                     "common-draw-bindings") X(BufferUpload,         \
                                                                               "buffer-upload")      \
                                  X(PersistentBuffer, "persistent-buffer-lookup") X(                 \
                                      UploadAllocate,                                                \
                                      "upload-allocation") X(ConstantMaterialize,                    \
                                                             "constant-materialization")             \
                                      X(ConstantBind, "constant-binding") X(                         \
                                          VertexConvert,                                             \
                                          "vertex-conversion") X(IndexConvert,                       \
                                                                 "index-conversion") X(PostFx,       \
                                                                                       "postfx-"     \
                                                                                       "recordin"    \
                                                                                       "g")          \
                                          X(PackedDepth, "packed-depth-refresh") X(                  \
                                              LifetimeBookkeeping,                                   \
                                              "lifetime-bookkeeping") X(DriverBarrier,               \
                                                                        "vk-barrier")                \
                                              X(DriverDraw, "vk-draw") X(DriverBind, "vk-bind") X(   \
                                                  DriverDynamic,                                     \
                                                  "vk-dynamic-state") X(DriverRendering,             \
                                                                        "vk-render-pass-boundary")   \
                                                  X(DriverCopy, "vk-copy-blit-resolve-clear") X(     \
                                                      DriverDescriptor,                              \
                                                      "vk-descriptor-update") X(DriverAllocation,    \
                                                                                "vk-object-"         \
                                                                                "allocation")        \
                                                      X(DriverDestruction,                           \
                                                        "vk-object-destruction") X(DriverPipeline,   \
                                                                                   "vk-pipeline-"    \
                                                                                   "compilation")    \
                                                          X(DriverBegin,                             \
                                                            "vk-command-buffer-begin-reset")         \
                                                              X(DriverEnd, "vk-command-buffer-"      \
                                                                           "end") X(                 \
                                                                  DriverSubmit, "vk-queue-submit")   \
                                                                  X(DriverMemory,                    \
                                                                    "vk-memory-map-flush")           \
                                                                      X(QueueLock, "queue-lock-"     \
                                                                                   "wait") X(        \
                                                                          Profiler,                  \
                                                                          "profiler-bookkeeping")    \
                                                                          X(TraceFormatting,         \
                                                                            "diagnostic-"            \
                                                                            "formatting")

enum class CpuPhase : uint8_t {
#define X(id, name) k##id,
  NATIVE_CPU_PHASES(X)
#undef X
      kCount
};
enum class CpuOp : uint8_t {
#define X(id, name) k##id,
  NATIVE_CPU_OPERATIONS(X)
#undef X
      kCount
};
inline constexpr size_t kCpuPhaseCount = size_t(CpuPhase::kCount);
inline constexpr size_t kCpuOpCount = size_t(CpuOp::kCount);
inline constexpr size_t kCpuCellCount = kCpuPhaseCount * kCpuOpCount;
inline constexpr size_t kCpuStackLimit = 64;
inline constexpr size_t kCpuEventLimit = 128;
inline constexpr size_t kCpuShaderLimit = 128;
inline const char* CpuPhaseName(CpuPhase phase) {
  static constexpr const char* names[] = {
#define X(id, name) name,
      NATIVE_CPU_PHASES(X)
#undef X
  };
  return size_t(phase) < kCpuPhaseCount ? names[size_t(phase)] : "invalid";
}
inline const char* CpuOpName(CpuOp op) {
  static constexpr const char* names[] = {
#define X(id, name) name,
      NATIVE_CPU_OPERATIONS(X)
#undef X
  };
  return size_t(op) < kCpuOpCount ? names[size_t(op)] : "invalid";
}
#undef NATIVE_CPU_PHASES
#undef NATIVE_CPU_OPERATIONS

inline uint64_t AddSaturated(uint64_t a, uint64_t b) noexcept {
  return b > UINT64_MAX - a ? UINT64_MAX : a + b;
}
struct CpuContext {
  uint32_t command = UINT32_MAX;
  uint32_t render_phase = 0;
  uint64_t vertex_shader = 0, pixel_shader = 0;
};
struct CpuMeasurement {
  CpuPhase phase = CpuPhase::kPublish;
  CpuOp op = CpuOp::kPublishFrame;
  uint64_t calls = 0, inclusive_ticks = 0, self_ticks = 0;
  uint64_t max_ticks = 0, max_self_ticks = 0;
  CpuContext worst{};
};
struct CpuEvent {
  CpuPhase phase = CpuPhase::kPublish;
  CpuOp op = CpuOp::kPublishFrame;
  uint32_t depth = 0;
  uint64_t begin = 0, end = 0, self_ticks = 0;
  CpuContext context{};
  uint64_t duration() const { return end - begin; }
};
struct CpuShaderCost {
  uint64_t vertex_shader = 0, pixel_shader = 0;
  uint32_t render_phase = 0;
  uint64_t commands = 0, inclusive_ticks = 0, self_ticks = 0, max_ticks = 0;
  uint32_t worst_command = UINT32_MAX;
};
struct CpuFrameData {
  uint64_t begin = 0, end = 0, clock_reads = 0, clock_probe_ticks = 0;
  uint64_t omitted_events = 0, invalid_scopes = 0, stack_overflows = 0, shader_overflows = 0;
  std::vector<CpuMeasurement> measurements;
  // Longest N events, not a full trace. All valid calls still enter aggregates.
  std::vector<CpuEvent> events;
  std::vector<CpuShaderCost> shaders;
  bool enabled = false;
};

// Fixed storage on the recorder's owner; never allocate per draw or on Enter/
// Leave. Snapshots allocate only once per sampled frame, outside draw recording.
class CpuRecorder {
 public:
  struct Token {
    uint64_t generation = 0, serial = 0;
    uint32_t depth = UINT32_MAX;
    explicit operator bool() const { return generation != 0 && depth != UINT32_MAX; }
  };
  bool Begin(uint64_t now) {
    if (active_ || generation_ == UINT64_MAX)
      return false;
    ++generation_;
    for (auto& cell : cells_)
      cell = {};
    depth_ = event_count_ = shader_count_ = 0;
    last_shader_index_ = SIZE_MAX;
    next_serial_ = 0;
    calls_ = invalid_ = overflow_ = shader_overflow_ = reads_ = 0;
    phase_ = CpuPhase::kPublish;
    context_ = {};
    begin_ = now;
    active_ = true;
    return true;
  }
  Token Enter(CpuOp op, uint64_t now) {
    if (!active_)
      return {};
    if (size_t(op) >= kCpuOpCount || size_t(phase_) >= kCpuPhaseCount || now < begin_) {
      ++invalid_;
      return {};
    }
    if (depth_ == stack_.size() || next_serial_ == UINT64_MAX) {
      ++overflow_;
      return {};
    }
    const auto slot = uint32_t(depth_++);
    stack_[slot] = {op, phase_, now, 0, context_, ++next_serial_};
    ++reads_;
    return {generation_, next_serial_, slot};
  }
  bool Leave(Token token, uint64_t now) {
    if (!active_ || !token || token.generation != generation_ || !depth_ ||
        token.depth != depth_ - 1 || stack_[depth_ - 1].serial != token.serial) {
      if (active_)
        ++invalid_;
      return false;
    }
    const auto frame = stack_[--depth_];
    ++reads_;
    if (now < frame.start || now - frame.start < frame.children) {
      ++invalid_;
      return false;
    }
    const uint64_t total = now - frame.start, self = total - frame.children;
    if (depth_)
      stack_[depth_ - 1].children = AddSaturated(stack_[depth_ - 1].children, total);
    auto& cell = cells_[size_t(frame.phase) * kCpuOpCount + size_t(frame.op)];
    cell.phase = frame.phase;
    cell.op = frame.op;
    ++cell.calls;
    cell.inclusive_ticks = AddSaturated(cell.inclusive_ticks, total);
    cell.self_ticks = AddSaturated(cell.self_ticks, self);
    if (total >= cell.max_ticks) {
      cell.max_ticks = total;
      cell.worst = frame.context;
    }
    cell.max_self_ticks = std::max(cell.max_self_ticks, self);
    CpuEvent event{frame.phase, frame.op, token.depth, frame.start, now, self, frame.context};
    ++calls_;
    const auto compare = [](const CpuEvent& a, const CpuEvent& b) {
      return a.duration() > b.duration();
    };
    if (event_count_ < events_.size()) {
      events_[event_count_++] = event;
      std::push_heap(events_.begin(), events_.begin() + event_count_, compare);
    } else if (total > events_[0].duration()) {
      std::pop_heap(events_.begin(), events_.end(), compare);
      events_.back() = event;
      std::push_heap(events_.begin(), events_.end(), compare);
    }
    if (frame.op == CpuOp::kRecordCommand)
      AddShader(frame.context, total, self);
    return true;
  }
  CpuFrameData Finish(uint64_t now, uint64_t clock_probe_ticks = 0) {
    CpuFrameData out;
    if (!active_)
      return out;
    if (depth_) {
      invalid_ += depth_;
      depth_ = 0;
    }
    if (now < begin_) {
      ++invalid_;
      now = begin_;
    }
    out.enabled = true;
    out.begin = begin_;
    out.end = now;
    out.clock_reads = reads_;
    out.clock_probe_ticks = clock_probe_ticks;
    out.omitted_events = calls_ - event_count_;
    out.invalid_scopes = invalid_;
    out.stack_overflows = overflow_;
    out.shader_overflows = shader_overflow_;
    out.measurements.reserve(
        std::count_if(cells_.begin(), cells_.end(), [](const auto& c) { return c.calls != 0; }));
    for (const auto& cell : cells_)
      if (cell.calls)
        out.measurements.push_back(cell);
    out.events.assign(events_.begin(), events_.begin() + event_count_);
    std::sort(out.events.begin(), out.events.end(), [](const auto& a, const auto& b) {
      return a.begin < b.begin || (a.begin == b.begin && a.end > b.end);
    });
    out.shaders.assign(shaders_.begin(), shaders_.begin() + shader_count_);
    active_ = false;
    return out;
  }
  void Cancel() {
    active_ = false;
    depth_ = 0;
  }
  bool active() const { return active_; }
  CpuPhase phase() const { return phase_; }
  CpuPhase SetPhase(CpuPhase phase) {
    auto prior = phase_;
    phase_ = phase;
    return prior;
  }
  CpuContext context() const { return context_; }
  CpuContext SetContext(CpuContext context) {
    auto prior = context_;
    context_ = context;
    return prior;
  }

 private:
  struct Open {
    CpuOp op;
    CpuPhase phase;
    uint64_t start, children;
    CpuContext context;
    uint64_t serial;
  };
  void AddShader(const CpuContext& context, uint64_t total, uint64_t self) {
    // Fixed open addressing is unnecessary for this small table: command
    // streams are coherent. A last-key fast path handles the common case.
    size_t i = shader_count_;
    if (last_shader_index_ < shader_count_ && Matches(shaders_[last_shader_index_], context))
      i = last_shader_index_;
    else
      for (size_t j = 0; j < shader_count_; ++j)
        if (Matches(shaders_[j], context)) {
          i = j;
          break;
        }
    if (i == shader_count_) {
      if (shader_count_ == shaders_.size()) {
        ++shader_overflow_;
        return;
      }
      auto& s = shaders_[shader_count_++];
      s = {};
      s.vertex_shader = context.vertex_shader;
      s.pixel_shader = context.pixel_shader;
      s.render_phase = context.render_phase;
    }
    last_shader_index_ = i;
    auto& s = shaders_[i];
    ++s.commands;
    s.inclusive_ticks = AddSaturated(s.inclusive_ticks, total);
    s.self_ticks = AddSaturated(s.self_ticks, self);
    if (total >= s.max_ticks) {
      s.max_ticks = total;
      s.worst_command = context.command;
    }
  }
  static bool Matches(const CpuShaderCost& s, const CpuContext& c) {
    return s.vertex_shader == c.vertex_shader && s.pixel_shader == c.pixel_shader &&
           s.render_phase == c.render_phase;
  }
  std::array<CpuMeasurement, kCpuCellCount> cells_{};
  std::array<Open, kCpuStackLimit> stack_{};
  std::array<CpuEvent, kCpuEventLimit> events_{};
  std::array<CpuShaderCost, kCpuShaderLimit> shaders_{};
  size_t depth_ = 0, event_count_ = 0, shader_count_ = 0;
  size_t last_shader_index_ = SIZE_MAX;
  uint64_t next_serial_ = 0, generation_ = 0, begin_ = 0, calls_ = 0, reads_ = 0, invalid_ = 0,
           overflow_ = 0, shader_overflow_ = 0;
  CpuPhase phase_ = CpuPhase::kPublish;
  CpuContext context_{};
  bool active_ = false;
};

// Transport values are per-batch sums/maxima, not additive frame-time ranges.
// Capture intervals may overlap across producer threads; dwell sums in
// particular must never be described as a serial CPU workload.
struct CommandTransport {
  uint64_t enqueued = 0, capture_ticks = 0, capture_lock_ticks = 0, queue_lock_ticks = 0,
           backpressure_ticks = 0;
};
struct TransportSummary {
  uint64_t commands = 0, measured_commands = 0, capture_ticks = 0, capture_lock_ticks = 0,
           queue_lock_ticks = 0;
  uint64_t backpressure_ticks = 0, dwell_ticks = 0, max_dwell_ticks = 0, queue_peak = 0;
  uint64_t worker_assembly_ticks = 0, worker_idle_ticks = 0, internal_flush_ticks = 0,
           internal_flushes = 0;
  void Observe(const CommandTransport& p, uint64_t dequeued, uint64_t queue_depth) {
    ++commands;
    queue_peak = std::max(queue_peak, queue_depth);
    if (!p.enqueued)
      return;
    ++measured_commands;
    capture_ticks = AddSaturated(capture_ticks, p.capture_ticks);
    capture_lock_ticks = AddSaturated(capture_lock_ticks, p.capture_lock_ticks);
    queue_lock_ticks = AddSaturated(queue_lock_ticks, p.queue_lock_ticks);
    backpressure_ticks = AddSaturated(backpressure_ticks, p.backpressure_ticks);
    if (dequeued >= p.enqueued) {
      const auto d = dequeued - p.enqueued;
      dwell_ticks = AddSaturated(dwell_ticks, d);
      max_dwell_ticks = std::max(max_dwell_ticks, d);
    }
  }
};

}  // namespace rex::graphics::gta4_native::profile
