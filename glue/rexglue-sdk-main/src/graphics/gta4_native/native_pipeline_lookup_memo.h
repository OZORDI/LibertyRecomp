#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace rex::graphics::gta4_native {

// A saturated lifetime disables memoization permanently. Reusing an epoch after
// destroying pipelines could otherwise resurrect an old Vulkan handle.
class NativePipelineLookupLifetime {
 public:
  explicit NativePipelineLookupLifetime(uint64_t epoch = 1) : epoch_(epoch) {}
  uint64_t epoch() const { return epoch_; }
  void Invalidate() {
    epoch_ = epoch_ && epoch_ != std::numeric_limits<uint64_t>::max() ? epoch_ + 1 : 0;
  }

 private:
  uint64_t epoch_;
};

template <size_t ColorTargetCount>
struct NativePipelineLookupContext {
  uint64_t lifetime = 0;
  uint64_t pipeline_layout = 0;
  std::array<uint32_t, ColorTargetCount> color_formats{};
  uint32_t depth_format = 0;
  uint32_t color_attachment_mask = 0;
  uint32_t color_write_mask = 0;
  uint32_t samples = 0;
  uint32_t guest_samples = 0;
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t primitive_type = 0;
  uint32_t user_pointer_stride = 0;
  uint32_t descriptor_backend = 0;
  uint32_t shader_override_mode = 0;
  uint32_t modern_shader_settings = 0;
  bool depth_stencil_attachment_active = false;
  bool uses_presenter = false;
  bool primitive_restart_enable = false;
  bool host_fog = false;

  bool operator==(const NativePipelineLookupContext&) const = default;
};

// Lives inline in an immutable pipeline-state snapshot. Successful validation
// can be reused by its later draw without rebuilding or hashing a pipeline key.
// The owner pointer rejects a memo copied into another snapshot. The context
// covers renderer/target inputs; fixed state is compared memberwise, not hashed
// or compared through padding bytes. Null results are never cached, preserving
// asynchronous compilation and every failure/retry path.
template <typename FixedState, size_t ColorTargetCount, typename Pipeline>
class NativePipelineLookupMemo {
 public:
  using Context = NativePipelineLookupContext<ColorTargetCount>;

  Pipeline Find(const void* owner, const FixedState& fixed, const Context& context) const {
    return pipeline_ && owner && owner == owner_ && context.lifetime && context == context_ &&
                   fixed == fixed_
               ? pipeline_
               : Pipeline{};
  }

  void Store(const void* owner, const FixedState& fixed, const Context& context, Pipeline pipeline) {
    if (!owner || !context.lifetime || !pipeline) {
      return;
    }
    owner_ = owner;
    fixed_ = fixed;
    context_ = context;
    pipeline_ = pipeline;
  }

 private:
  const void* owner_ = nullptr;
  FixedState fixed_{};
  Context context_{};
  Pipeline pipeline_{};
};

}  // namespace rex::graphics::gta4_native
