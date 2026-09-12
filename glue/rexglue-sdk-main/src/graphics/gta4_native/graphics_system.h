#pragma once

#include <array>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <rex/graphics/gta4_native/title_commands.h>
#include <rex/graphics/gta4_native/native_black_anomaly.h>
#include <rex/graphics/gta4_native/surface_view.h>
#include <rex/graphics/pipeline/texture/info.h>
#include <rex/system/interfaces/graphics.h>
#include <rex/ui/vulkan/device.h>

#include "postfx_resource_pool.h"
#include "dirty_state_delta.h"
#include "frame_constant_arena.h"
#include "native_working_set.h"
#include "native_immutable_bindings.h"
#include "native_texture_protection.h"
#include "native_prepared_bindings.h"
#include "native_image_reuse.h"
#include "native_texture_eviction_index.h"
#include <memory_resource>
#include "stateful_constant_state.h"
#include "native_buffer_arena.h"
#include "native_bulb_appearance.h"
#include "native_aspect_content.h"
#include <rex/graphics/gta4_native/phone_trace.h>
#include <rex/graphics/gta4_native/tv_trace.h>
#include <rex/graphics/gta4_native/fire_escape_trace.h>
#include "native_attachment_policy.h"
#include "native_binding_policy.h"
#include "native_descriptor_backend.h"
#include "native_draw_state_cache.h"
#include "native_frame_context.h"
#include "native_frame_scheduling.h"
#include "native_gpu_attribution.h"
#include <rex/graphics/gta4_native/gpu_pass_origin.h>
#include "native_host_enhancement_policy.h"
#include "native_lighting_lineage.h"
#include "native_memory_samples.h"
#include "native_owner_retirement.h"
#include "native_performance_samples.h"
#include "native_profile_detail.h"
#include "native_pipeline_lookup_memo.h"
#include "native_reflection_registry.h"
#include "native_resolve_policy.h"
#include "native_room_light_probe.h"
#include "native_sampler_cache_key.h"
#include "native_sampler_lod_bias.h"
#include "shader_override_policy.h"
#include "modern_shader_policy.h"
#include "native_virtual_resource_registry.h"
#include "smaa_pipeline.h"
#include "split_postfx_pass.h"
#include "sun_shafts_pass.h"

struct ShaderOverrideCacheEntry;

namespace rex::memory {
class Memory;
}
namespace rex::runtime {
class FunctionDispatcher;
}
namespace rex::ui {
class GraphicsProvider;
class Presenter;
class WindowedAppContext;
namespace vulkan {
class VulkanSubmissionTracker;
}
}  // namespace rex::ui

namespace rex::graphics::gta4_native {

class Gta4NativeGraphicsSystem final : public system::IGraphicsSystem {
 public:
  Gta4NativeGraphicsSystem();
  ~Gta4NativeGraphicsSystem() override;

  X_STATUS SetupPresentation(ui::WindowedAppContext* app_context) override;
  X_STATUS SetupGuestGpu(runtime::FunctionDispatcher* function_dispatcher,
                         system::KernelState* kernel_state) override;
  bool has_presentation() const override { return presenter_ != nullptr; }
  ui::GraphicsProvider* provider() const override { return provider_.get(); }
  ui::Presenter* presenter() const override { return presenter_.get(); }

  uint32_t GetTitleCommandAbi(uint32_t title_id) const override;
  bool SubmitTitleCommand(uint32_t title_id, uint32_t abi_version, const void* command,
                          size_t command_size) override;
  bool ExecuteTitleCommand(uint32_t title_id, uint32_t abi_version, const void* command,
                           size_t command_size, void* result, size_t result_size) override;
  void InitializeShaderStorage(const std::filesystem::path& cache_root, uint32_t title_id,
                               bool blocking) override;

  void Shutdown() override;

 private:
  enum class NativeVertexNumericType : uint8_t {
    kFloat,
    kSignedInteger,
    kUnsignedInteger,
  };

  struct NativeVertexInput {
    uint32_t location = 0;
    NativeVertexNumericType numeric_type = NativeVertexNumericType::kFloat;
    uint32_t component_count = 0;

    bool operator==(const NativeVertexInput&) const = default;
  };

  struct NativeShader {
    ShaderStage stage = ShaderStage::kPixel;
    uint64_t hash = 0;
    uint32_t specialization_constants_mask = 0;
    uint32_t used_texture_mask = 0;
    uint32_t color_output_mask = 0;
    VkShaderModule early_module = VK_NULL_HANDLE;
    VkShaderModule late_module = VK_NULL_HANDLE;
    std::array<uint64_t, 4> module_code_hashes{};
    std::string filename;
    std::vector<NativeVertexInput> vertex_inputs;

    const ShaderOverrideCacheEntry* override_entry = nullptr;
    uint32_t override_specialization_constants_mask = 0;
    uint32_t override_used_texture_mask = 0;
    uint32_t override_color_output_mask = 0;
    VkShaderModule override_early_module = VK_NULL_HANDLE;
    VkShaderModule override_late_module = VK_NULL_HANDLE;
  };

  struct ProducerShaderMetadata {
    ShaderStage stage = ShaderStage::kPixel;
    uint64_t hash = 0;
    uint32_t used_texture_mask = 0;
  };

  struct ProducerDeviceShaderState {
    uint32_t pixel_shader = 0;
    uint32_t vertex_shader = 0;
  };

  struct NativeVertexDeclaration {
    uint32_t handle = 0;
    uint64_t generation = 0;
    uint64_t content_hash = 0;
    uint32_t maximum_stream = 0;
    std::vector<VertexElement> elements;
  };

  struct NativeFixedFunctionState {
    uint32_t depth_enable = 0;
    uint32_t depth_function = 0;
    uint32_t depth_write_enable = 0;
    uint32_t depth_clamp_enable = 0;
    uint32_t clip_control = 0;
    uint32_t user_clip_plane_enable_mask = 0;
    std::array<uint32_t, 4> clip_plane_bits{};
    uint32_t negative_one_to_one_clip_space = 0;
    uint32_t cull_mode = 0;
    uint32_t polygon_mode = 0;
    uint32_t blend_enable = 0;
    std::array<uint32_t, kRenderTargetCount> blend_controls{};
    uint32_t source_blend = 0;
    uint32_t destination_blend = 0;
    uint32_t blend_operation = 0;
    uint32_t source_blend_alpha = 0;
    uint32_t destination_blend_alpha = 0;
    uint32_t blend_operation_alpha = 0;
    std::array<float, 4> blend_constants{};
    uint32_t alpha_test_enable = 0;
    uint32_t alpha_function = 0;
    float alpha_reference = 0.0f;
    // Xenos RB_COLORCONTROL bit 4. This is deliberately captured separately
    // from alpha test: foliage may request alpha-to-mask while alpha test is
    // disabled.
    uint32_t alpha_to_mask_enable = 0;
    // Native shader layout: four RB_COLORCONTROL offsets in bits 0-7 and the
    // enable in bit 8. This remains dynamic and is not a pipeline-key field.
    uint32_t alpha_to_mask = 0;
    uint32_t stencil_enable = 0;
    uint32_t two_sided_stencil = 0;
    uint32_t stencil_fail = 0;
    uint32_t stencil_depth_fail = 0;
    uint32_t stencil_pass = 0;
    uint32_t stencil_function = 0;
    uint32_t stencil_reference = 0;
    uint32_t stencil_mask = 0;
    uint32_t stencil_write_mask = 0;
    uint32_t back_stencil_reference = 0;
    uint32_t back_stencil_mask = 0;
    uint32_t back_stencil_write_mask = 0;
    uint32_t ccw_stencil_fail = 0;
    uint32_t ccw_stencil_depth_fail = 0;
    uint32_t ccw_stencil_pass = 0;
    uint32_t ccw_stencil_function = 0;
    uint32_t scissor_enable = 0;
    uint32_t slope_scaled_depth_bias_bits = 0;
    uint32_t depth_bias_bits = 0;
    bool depth_bias_enable = false;
    bool depth_bias_representable = true;
    uint32_t color_write_mask = 0;
    uint32_t sample_mask = 0xFFFFu;
    std::array<uint32_t, 6> viewport_bits{};
    std::array<int32_t, 4> scissor{};

    bool operator==(const NativeFixedFunctionState&) const = default;
  };

  struct NativePipelineState {
    struct VertexStream {
      uint32_t buffer = 0;
      uint32_t offset = 0;
      uint32_t stride = 0;
      uint32_t stride_words = 0;
    };

    std::array<uint32_t, kTextureStageCount> textures{};
    std::array<SurfaceDescriptor, kRenderTargetCount> render_targets{};
    std::array<VertexStream, kVertexStreamCount> vertex_streams{};
    uint32_t pixel_shader = 0;
    uint32_t vertex_shader = 0;
    const NativeShader* pixel_shader_resource = nullptr;
    const NativeShader* vertex_shader_resource = nullptr;
    uint32_t vertex_declaration = 0;
    std::shared_ptr<const NativeVertexDeclaration> vertex_declaration_resource;
    SurfaceDescriptor depth_stencil{};
    uint32_t depth_stencil_trace_wrapper = 0;
    uint32_t depth_stencil_trace_caller = 0;
    uint32_t index_buffer = 0;
    uint64_t version = 0;
    // Only the render worker updates memoization on an immutable snapshot.
    mutable NativePipelineLookupMemo<NativeFixedFunctionState, kRenderTargetCount, VkPipeline>
        pipeline_lookup_memo;
  };

  struct NativePersistentBufferEntry;

  struct NativeBufferResource {
    struct ConvertedVertexPayload {
      uint64_t declaration_hash = 0;
      uint64_t shader_hash = 0;
      uint32_t stream = 0;
      uint32_t stream_offset = 0;
      uint32_t stride = 0;
      std::vector<uint8_t> payload;
      uint32_t created_frame = 0;
      uint32_t last_used_frame = 0;
    };

    uint32_t handle = 0;
    uint32_t flags = 0;
    uint32_t guest_address = 0;
    uint32_t guest_size = 0;
    uint64_t content_hash = 0;
    uint64_t generation = 0;
    uint32_t created_frame = 0;
    mutable std::atomic<uint32_t> last_used_frame{0};
    mutable std::atomic<size_t> shadow_validation_offset{0};
    std::vector<uint8_t> payload;
    // Built only by the single render worker. Keeping conversions with the
    // immutable source generation makes their lifetime follow the guest
    // resource and avoids repeating format/endian conversion every frame.
    mutable std::vector<ConvertedVertexPayload> converted_vertex_payloads;
    mutable std::vector<uint8_t> host_index16_payload;
    mutable std::vector<uint8_t> host_index32_payload;
    mutable NativeOwnerRetirementWatch<NativePersistentBufferEntry> persistent_retirement;
  };

  enum class NativeTextureOrigin : uint8_t {
    kGuestSnapshot,
    kResolveOutput,
    kHostReplacement,
    kPackedDepthAlias,
  };

  struct NativeTextureResource {
    struct MipLevel {
      uint32_t level = 0;
      uint32_t width = 0;
      uint32_t height = 0;
      uint32_t depth = 1;
      uint32_t base_array_layer = 0;
      uint32_t layer_count = 1;
      uint32_t buffer_row_length = 0;
      uint32_t buffer_image_height = 0;
      size_t payload_offset = 0;
      size_t payload_size = 0;
    };

    uint32_t handle = 0;
    uint64_t content_hash = 0;
    uint64_t generation = 0;
    xenos::xe_gpu_texture_fetch_t fetch{};
    TextureInfo info{};
    std::vector<uint8_t> payload;
    std::vector<MipLevel> mip_levels;
    NativeTextureOrigin origin = NativeTextureOrigin::kGuestSnapshot;
    uint64_t virtual_lifetime = 0;
    bool gpu_produced = false;
    std::shared_ptr<const NativeTextureResource> packed_depth_source;
    bool vector_font_replacement = false;
    uint32_t vector_font_id = 0;
  };

  struct SynchronousCommand {
    std::mutex mutex;
    std::condition_variable condition;
    TextureLockResult result{};
    bool complete = false;
    bool succeeded = false;
  };

  struct NativeDeviceSnapshot {
    std::shared_ptr<const std::vector<uint8_t>> storage;

    size_t size() const { return storage ? storage->size() : 0; }
    const uint8_t* data() const { return storage ? storage->data() : nullptr; }
    const uint8_t& operator[](size_t index) const { return (*storage)[index]; }
    operator const std::vector<uint8_t>&() const {
      static const std::vector<uint8_t> kEmpty;
      return storage ? *storage : kEmpty;
    }
  };

  struct NativeShaderState {
    std::shared_ptr<const ConstantStateVersion> vertex_constants;
    std::shared_ptr<const ConstantStateVersion> pixel_constants;
    uint32_t vertex_booleans = 0;
    uint32_t pixel_booleans = 0;
    StateVersionVector versions{};
  };

  struct NativeShaderConstantDelta {
    ConstantPayloadDelta vertex_constants;
    ConstantPayloadDelta pixel_constants;
    std::array<uint32_t, 2> booleans{};
    bool booleans_present = false;
    bool compare_snapshot = false;
  };

  struct NativeDeviceConstantState {
    NativeDeviceConstantState(size_t vertex_size, size_t pixel_size)
        : vertex_constants(vertex_size), pixel_constants(pixel_size) {}

    AuthoritativeConstantState vertex_constants;
    AuthoritativeConstantState pixel_constants;
    AuthoritativeScalarState<std::array<uint32_t, 2>> booleans;
    StateVersionVector versions{};
  };

  struct NativeRoomLightInputBinding {
    uint32_t texture_stage = UINT32_MAX;
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE;
    uint64_t image_lifetime = 0;
    VkSamplerCreateInfo sampler_info{};
  };

  struct NativeCommand {
    GpuPassOrigin gpu_pass_origin{};
    profile::CommandTransport profile_transport;
    std::shared_ptr<const FireTraceContext> fire_trace;
    std::shared_ptr<const BulbSourceSnapshot> bulb_trace;
    std::shared_ptr<PhoneTraceContext> phone_trace;
    std::shared_ptr<TvTraceContext> tv_trace;
    CommandType type = CommandType::kPresent;
    LightingContext lighting{};
    uint32_t light_trace_id = 0;
    uint32_t light_trace_technique = 0xFFFFFFFFu;
    uint32_t light_trace_mode = 0;
    std::vector<uint8_t> bytes;
    std::vector<uint8_t> payload;
    NativeDeviceSnapshot device_snapshot;
    NativeShaderConstantDelta shader_constant_delta;
    std::shared_ptr<const NativeShaderState> shader_state;
    std::array<std::shared_ptr<const NativeBufferResource>, kVertexStreamCount> vertex_buffers{};
    std::shared_ptr<const NativeBufferResource> index_buffer;
    std::array<std::shared_ptr<const NativeTextureResource>, kTextureStageCount> textures{};
    std::array<xenos::xe_gpu_texture_fetch_t, kTextureStageCount> texture_fetches{};
    uint32_t used_texture_mask = 0;
    std::array<NativeBindingRealization, kTextureStageCount> binding_realization{};
    // Diagnostic-only heap sidecar captured by normal descriptor realization.
    std::shared_ptr<std::vector<NativeRoomLightInputBinding>> room_light_input_bindings;
    uint32_t realized_image_mask = 0;
    uint32_t realized_sampler_mask = 0;
    uint32_t guest_null_texture_mask = 0;
    uint32_t failed_texture_mask = 0;
    bool bindings_prepared = false;
    std::shared_ptr<const NativeTextureResource> resolve_destination;
    std::shared_ptr<const NativeTextureResource> depth_handoff_source;
    std::shared_ptr<const NativeTextureResource> present_source;
    std::shared_ptr<const EnvironmentalDataV1> environmental_data;
    std::array<SurfaceDescriptor, kRenderTargetCount> snapshot_render_targets{};
    SurfaceDescriptor snapshot_depth_stencil{};
    std::array<VkDescriptorSet, 5> draw_descriptor_sets{};
    std::array<uint32_t, kTextureStageCount> texture_descriptor_indices{};
    std::array<uint32_t, kTextureStageCount> sampler_descriptor_indices{};
    uint32_t descriptor_page = UINT32_MAX;
    uint64_t image_descriptor_epoch = 0;
    uint64_t sampler_descriptor_epoch = 0;
    uint64_t cached_descriptor_epoch = 0;
    uint64_t environmental_data_hash = 0;
    uint32_t descriptor_copy = 0;
    std::shared_ptr<const NativePipelineState> pipeline_state;
    NativeFixedFunctionState fixed_function_state{};
    uint64_t captured_fixed_function_state_hash = 0;
    uint64_t recorded_fixed_function_state_hash = 0;
    uint64_t captured_vertex_constants_hash = 0;
    uint64_t captured_pixel_constants_hash = 0;
    RenderPhase render_phase = RenderPhase::kUnknown;
    uint32_t render_phase_object = 0;
    uint64_t vertex_constants_hash = 0;
    uint64_t pixel_constants_hash = 0;
    DirtyStateComponentMask dirty_components = 0;
    uint64_t released_texture_generation = 0;
    // Diagnostic-only producer identity. These fields are native sidecars,
    // not part of the title command ABI, and let traces prove whether a title
    // frame was split before its Present command reaches the render worker.
    uint64_t diagnostic_submit_sequence = 0;
    uint32_t diagnostic_producer_epoch = 0;
    std::shared_ptr<SynchronousCommand> synchronous;
  };

  struct NativeUploadBuffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint8_t* mapping = nullptr;
    VkDeviceAddress device_address = 0;
    VkDeviceSize capacity = 0;
    VkDeviceSize allocation_size = 0;
    VkDeviceSize write_offset = 0;
    uint32_t memory_type = UINT32_MAX;
  };

  struct NativeRoomLightInputTexture {
    NativeRoomLightInputBinding binding;
    uint32_t probe_stage = UINT32_MAX;
    uint32_t descriptor_index = 0;
    uint32_t sampler_index = 0;
    uint32_t handle = 0;
    uint32_t address = 0;
    uint64_t generation = 0;
    uint64_t content_hash = 0;
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkImageAspectFlags aspect = 0;
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkSampleCountFlagBits image_samples = VK_SAMPLE_COUNT_1_BIT;
    VkComponentMapping view_components{};
    VkImageSubresourceRange view_range{};
    VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t logical_width = 0;
    uint32_t logical_height = 0;
    uint32_t bytes_per_sample = 0;
    uint32_t block_extent = 1;
    uint32_t swizzle = 0;
    std::array<uint32_t, 6> fetch{};
    std::string status = "not-recorded";
    uint32_t packed_source_handle = 0;
    uint64_t packed_source_generation = 0;
    uint64_t packed_source_lifetime = 0;
    uint64_t packed_depth_serial = 0;
    uint64_t packed_stencil_serial = 0;
    uint32_t packed_resolve_frame = 0;
    bool packed_source_resolved = false;
  };

  struct NativeRoomLightInputs {
    std::shared_ptr<NativeRoomLightProbeReceipt> receipt;
    uint32_t frame = 0;
    uint32_t command_index = UINT32_MAX;
    uint32_t context = 0;
    uint32_t required_mask = 0;
    uint32_t captured_mask = 0;
    uint32_t recorded_required_mask = 0;
    uint32_t vertex_texture_mask = 0;
    uint32_t pixel_texture_mask = 0;
    uint32_t descriptor_backend = 0;
    uint32_t descriptor_page = UINT32_MAX;
    uint32_t descriptor_copy = 0;
    uint64_t image_descriptor_epoch = 0;
    uint64_t sampler_descriptor_epoch = 0;
    uint64_t cached_descriptor_epoch = 0;
    uint64_t draw_id = 0;
    uint64_t export_id = 0;
    uint64_t vertex_constants_hash = 0;
    uint64_t pixel_constants_hash = 0;
    uint32_t vertex_booleans = 0;
    uint32_t pixel_booleans = 0;
    std::shared_ptr<const std::vector<uint8_t>> vertex_constants;
    std::shared_ptr<const std::vector<uint8_t>> pixel_constants;
    std::vector<NativeRoomLightInputTexture> textures;
    bool scheduled_complete = false;
    std::string failure;
  };

  struct NativeContentProbeStage {
    std::shared_ptr<BulbProbeIdentity> bulb;
    std::shared_ptr<PhoneProbeIdentity> phone;
    std::shared_ptr<TvProbeIdentity> tv;
    bool valid = false;
    bool reserved = false;
    uint8_t kind = 0;
    uint8_t command_type = 0;
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkImageAspectFlags aspect = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t mip_level = 0;
    uint32_t handle = 0;
    uint32_t address = 0;
    uint32_t command_index = UINT32_MAX;
    uint32_t texture_stage = UINT32_MAX;
    uint32_t checkpoint_ordinal = UINT32_MAX;
    uint32_t checkpoint_count = 0;
    uint32_t render_phase = 0;
    uint64_t draw_id = 0;
    uint64_t vertex_shader_hash = 0;
    uint64_t pixel_shader_hash = 0;
    uint32_t trace_wrapper = 0;
    uint32_t sample_count = 0;
    uint8_t vertical_band = 0;
    uint8_t vertical_band_count = 1;
    uint64_t resource_generation = 0;
    uint32_t provenance_handle = 0;
    uint32_t provenance_frame = 0;
    uint32_t provenance_command = UINT32_MAX;
    uint64_t provenance_serial = 0;
    uint32_t provenance_phase = 0;
    uint32_t provenance_kind = 0;
    NativeBlackAnomalyPolicy black_anomaly_policy = NativeBlackAnomalyPolicy::kIgnore;
    uint8_t reflection_family = 0;
    uint8_t reflection_role = 0;
    std::array<uint32_t, 4> texture_handles{};
    std::string_view diagnostic_category;
    std::string_view diagnostic_role;
  };

  struct NativeContentProbeBuffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint8_t* mapping = nullptr;
    VkDeviceSize allocation_size = 0;
    uint32_t memory_type = UINT32_MAX;
    uint32_t pending_frame = 0;
    std::array<NativeContentProbeStage, 256> stages{};
    std::shared_ptr<NativeRoomLightInputs> room_light_inputs;
  };

  struct NativeLightStencilHistogramStage {
    bool valid = false;
    bool before_setup = false;
    VkFormat format = VK_FORMAT_UNDEFINED;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t command_index = UINT32_MAX;
    uint32_t light_trace_id = 0;
    uint32_t light_trace_technique = 0xFFFFFFFFu;
    uint32_t light_trace_mode = 0;
    uint32_t depth_handle = 0;
    uint32_t depth_address = 0;
    uint64_t actual_image = 0;
    uint64_t surface_lifetime = 0;
    std::shared_ptr<NativeRoomLightProbeReceipt> room_receipt;
    uint64_t bulb_instance_id = 0;
    uint64_t draw_id = 0;
    uint64_t light_trace_record = 0;
    uint64_t vertex_shader_hash = 0;
    NativeFixedFunctionState fixed_function{};
  };

  struct NativeLightStencilHistogramBuffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint8_t* mapping = nullptr;
    VkDeviceSize capacity = 0;
    VkDeviceSize copy_stride = 0;
    VkDeviceSize allocation_size = 0;
    uint32_t memory_type = UINT32_MAX;
    uint32_t pending_frame = 0;
    std::array<NativeLightStencilHistogramStage, 2> stages{};
  };

  struct NativeLightColorDeltaStage {
    bool before_valid = false;
    bool after_valid = false;
    VkFormat format = VK_FORMAT_UNDEFINED;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t command_index = UINT32_MAX;
    uint32_t context = 0;
    uint32_t target_handle = 0;
    uint32_t target_address = 0;
    uint32_t recorded_target_handle = 0;
    uint32_t recorded_target_address = 0;
    uint32_t effective_mode = UINT32_MAX;
    uint64_t actual_image = 0;
    uint64_t surface_lifetime = 0;
    std::shared_ptr<NativeRoomLightProbeReceipt> room_receipt;
    uint32_t light_trace_technique = 0xFFFFFFFFu;
    uint32_t light_trace_mode = 0;
    uint64_t instance_id = 0;
    uint64_t draw_id = 0;
    uint64_t vertex_shader_hash = 0;
    uint64_t pixel_shader_hash = 0;
  };

  struct NativeLightColorDeltaBuffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint8_t* mapping = nullptr;
    VkDeviceSize capacity = 0;
    VkDeviceSize copy_stride = 0;
    VkDeviceSize allocation_size = 0;
    uint32_t memory_type = UINT32_MAX;
    uint32_t pending_frame = 0;
    NativeLightColorDeltaStage stage{};
  };

  static constexpr uint32_t kTranslucentQueryCapacity = 1024;

  struct NativeTranslucentQuery {
    bool valid = false;
    bool draw_recorded = false;
    std::shared_ptr<NativeRoomLightProbeReceipt> room_receipt;
    NativeRoomLightProbeKey room_key;
    uint32_t effective_mode = UINT32_MAX;
    uint32_t frame = 0;
    uint32_t command_index = UINT32_MAX;
    uint32_t render_phase = 0;
    uint64_t draw_id = 0;
    uint32_t light_trace_id = 0;
    uint32_t light_trace_technique = 0xFFFFFFFFu;
    uint32_t light_trace_mode = 0;
    uint64_t light_instance_id = 0;
    uint64_t light_trace_record = 0;
    uint32_t shader_override_pair_id = 0;
    uint64_t shader_variant_key = 0;
    uint64_t vertex_shader_hash = 0;
    uint64_t pixel_shader_hash = 0;
    uint32_t pixel_shader_specialization_constants_mask = 0;
    uint32_t pixel_shader_specialization_value = 0;
    bool pixel_shader_uses_late_module = false;
    uint32_t target_handle = 0;
    uint32_t target_address = 0;
    uint32_t depth_handle = 0;
    uint32_t depth_address = 0;
    NativeFixedFunctionState fixed_function{};
    std::string category;
    std::string variant;
    std::string shader_filename;
  };

  struct NativeTranslucentQueryState {
    VkQueryPool pool = VK_NULL_HANDLE;
    uint32_t pending_frame = 0;
    uint32_t pending_count = 0;
    // Frame-slot swaps must not materialize the full census on the render
    // worker's stack. Keep fixed-capacity indexed storage on the heap; moving
    // or swapping the state transfers only vector ownership.
    std::vector<NativeTranslucentQuery> queries =
        std::vector<NativeTranslucentQuery>(kTranslucentQueryCapacity);
  };
  static_assert(sizeof(NativeTranslucentQueryState) <= 64,
                "Query state must remain small enough for safe frame-slot swaps");

  static constexpr uint32_t kNativeGpuProfileQueryCapacity =
      uint32_t(performance::kMaximumGpuQueriesPerFrame);

  struct NativeGpuProfileSlice {
    performance::GpuRange range = performance::GpuRange::kUnattributed;
    uint32_t begin_query = UINT32_MAX;
    uint32_t end_query = UINT32_MAX;
    uint32_t attribution_region = UINT32_MAX;
  };

  struct NativeGpuProfileQueryResult {
    uint64_t timestamp = 0;
    uint64_t available = 0;
  };

  struct NativeGpuProfileFramePayload {
    VkQueryPool pool = VK_NULL_HANDLE;
    bool active = false;
    bool pending = false;
    bool detailed_gpu = false;
    uint32_t query_budget = 0;
    profile::FrameDetail detail;
    uint32_t frame = 0;
    uint32_t query_count = 0;
    uint32_t dropped_spans = 0;
    uint32_t frame_begin_query = UINT32_MAX;
    uint32_t current_range_begin_query = UINT32_MAX;
    performance::GpuRange current_range = performance::GpuRange::kUnattributed;
    uint32_t current_attribution_region = UINT32_MAX;
    uint32_t detail_boundary_budget = 0;
    uint32_t detail_boundary_count = 0;
    uint32_t dropped_attribution_regions = 0;
    std::array<NativeGpuProfileSlice, performance::kMaximumGpuQueriesPerFrame> slices{};
    size_t slice_count = 0;
    std::array<NativeGpuProfileQueryResult, performance::kMaximumGpuQueriesPerFrame>
        query_results{};
    performance::FrameBuilder sample_builder;
    uint64_t capture_sequence = 0;
    uint64_t cpu_frame_interval_ticks = 0;
    uint64_t cpu_housekeeping_ticks = 0;
    uint64_t cpu_slot_cleanup_ticks = 0;
    uint64_t cpu_profile_readback_ticks = 0;
    uint64_t cpu_upload_capacity_ticks = 0;
    uint64_t cpu_command_setup_ticks = 0;
    uint64_t cpu_texture_prepare_ticks = 0;
    uint64_t cpu_record_ticks = 0;
    uint64_t cpu_finalize_ticks = 0;
    uint64_t cpu_submit_ticks = 0;
    uint64_t cpu_callback_ticks = 0;
    uint64_t cpu_publish_ticks = 0;
    VkDeviceSize upload_bytes = 0;
    attribution::NativeAttributionPlan attribution_plan;
    std::optional<attribution::NativeDrilldownReservation> attribution_reservation;
  };

  struct NativeGpuProfileState {
    float timestamp_period_ns = 0.0f;
    uint32_t timestamp_valid_bits = 0;
    bool support_checked = false;
    bool supported = false;
    bool capture_complete = false;
    bool capture_armed = false;
    bool export_started = false;
    uint32_t last_sampled_frame = 0;
    std::array<NativeGpuProfileFramePayload, NativeFrameContextRing::kSlotCount> frames{};
    performance::FrameSampleCompletionQueue completed_samples;
    attribution::NativeAttributionCompletionQueue completed_attribution;
    attribution::NativeDrilldownScheduler drilldown_scheduler;
    uint64_t next_capture_sequence = 1;
    uint64_t next_publish_sequence = 1;
    performance::FrameSampleRing sample_ring;
    std::array<performance::FrameSample, performance::kFrameSampleCapacity> export_samples{};
    size_t export_sample_count = 0;
    std::thread export_thread;
    profile::CaptureMetadata detail_metadata;
    std::vector<profile::FrameDetail> detail_frames;
    std::array<std::optional<profile::FrameDetail>, NativeFrameContextRing::kSlotCount> completed_details;
    uint64_t last_publish_host_tick = 0;
  };

  struct NativeImageResource {
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkDeviceSize allocation_size = 0;
    uint32_t memory_type = UINT32_MAX;
  };

  struct NativeTextureImage {
    std::shared_ptr<const NativeTextureResource> source;
    uint64_t descriptor_lifetime = 0;
    NativeImageResource resource;
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageUsageFlags usage = 0;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    VkComponentMapping view_components{};
    VkImageSubresourceRange view_range{};
    VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D;
    uint32_t guest_mip_levels = 1;
    uint32_t mip_levels = 1;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t logical_width = 0;
    uint32_t logical_height = 0;
    VkDeviceSize allocation_size = 0;
    uint32_t memory_type = UINT32_MAX;
    uint32_t memory_heap = UINT32_MAX;
    bool memory_accounted = false;
    bool allocation_pool_compatible = false;
    NativeImageAllocationKey allocation_key{};
    NativeReflectionTarget reflection{};
    bool is_reflection = false;
    // Resolve destinations persist independently of their transient capture
    // attachments. Content validity applies to every GPU-produced texture;
    // reflections are only one consumer of this general contract.
    NativeResolvedTextureContentState content;
    NativeSurfaceAspectContent aspect_content{};
    // Last submitted frame that referenced this image; eviction waits out
    // kNativeTextureEvictionGraceFrames beyond this.
    uint32_t last_used_frame = 0;
    uint64_t last_use_serial = 0;
    uint64_t last_used_submission = 0;
    uint32_t created_frame = 0;
    NativeDescriptorSlotHandle descriptor_slot{};
    bool descriptor_retirement_queued = false;
    bool descriptor_reclaimed = true;
    std::vector<VkImageView> mip_views;
    VkImageView packed_stencil_view = VK_NULL_HANDLE;
    NativeSurfaceAspectContent packed_source_content{};
    uint64_t packed_source_image_lifetime = 0;
  };

  struct NativeTextureHeapBudgets {
    bool available = false;
    uint32_t heap_count = 0;
    std::array<VkDeviceSize, VK_MAX_MEMORY_HEAPS> usage{};
    std::array<VkDeviceSize, VK_MAX_MEMORY_HEAPS> budget{};
  };

  struct NativeSamplerKey {
    xenos::TextureFilter min_filter = xenos::TextureFilter::kPoint;
    xenos::TextureFilter mag_filter = xenos::TextureFilter::kPoint;
    xenos::TextureFilter mip_filter = xenos::TextureFilter::kPoint;
    xenos::ClampMode clamp_u = xenos::ClampMode::kRepeat;
    xenos::ClampMode clamp_v = xenos::ClampMode::kRepeat;
    xenos::ClampMode clamp_w = xenos::ClampMode::kRepeat;
    xenos::AnisoFilter aniso_filter = xenos::AnisoFilter::kDisabled;
    xenos::BorderColor border_color = xenos::BorderColor::k_ABGR_Black;
    int32_t lod_bias = 0;
    uint32_t mip_min_level = 0;
    uint32_t mip_max_level = 0;

    bool operator==(const NativeSamplerKey&) const = default;
  };

  struct NativeSampler {
    NativeSamplerKey key{};
    VkSampler sampler = VK_NULL_HANDLE;
    NativeDescriptorSlotHandle descriptor_slot{};
    VkSamplerCreateInfo creation_info{};
  };

  struct NativeDescriptorImageSlot {
    uint64_t generation = 0;
    std::array<VkImageView, 4> views{};
    bool operator==(const NativeDescriptorImageSlot&) const = default;
  };

  struct NativeDescriptorSamplerSlot {
    uint64_t generation = 0;
    VkSampler sampler = VK_NULL_HANDLE;
    bool operator==(const NativeDescriptorSamplerSlot&) const = default;
  };

  struct NativeDescriptorPage {
    VkDescriptorPool pool = VK_NULL_HANDLE;
    std::array<VkDescriptorSet, 5> descriptor_sets{};
    std::vector<NativeDescriptorImageSlot> image_slots;
    std::vector<NativeDescriptorSamplerSlot> sampler_slots;
    uint64_t frame_epoch = 0;
    uint32_t published_image_count = 0, published_sampler_count = 0;
    bool images_dirty = false, samplers_dirty = false;
    std::vector<VkDescriptorImageInfo> update_scratch;
  };

  struct NativeDescriptorRetirement {
    NativeTextureImage* image = nullptr;
    NativeDescriptorSlotHandle handle{};
    uint64_t last_submission = 0;
  };

  enum class NativeFlightResourceKind : uint8_t {
    kCommandPool,
    kCommandBuffer,
    kDescriptorPool,
    kUploadBuffer,
    kTextureImage,
    kTextureView,
    kTextureMipView,
    kTextureDescriptorSlot,
    kSurfaceImage,
    kSurfaceView,
    kSurfaceSampledView,
    kSurfaceScratchBuffer,
    kPersistentBuffer,
  };

  struct NativeFlightResourceKey {
    NativeFlightResourceKind kind = NativeFlightResourceKind::kTextureImage;
    uint64_t object = 0;
    uint32_t page = 0;
    uint32_t index = 0;
    uint32_t generation = 0;

    auto operator<=>(const NativeFlightResourceKey&) const = default;
  };

  struct NativeFlightResourceReference {
    NativeFlightResourceKey key{};
    uint32_t guest_handle = 0;
    uint64_t guest_generation = 0;
  };

  struct NativeFlightSubmission {
    uint64_t submission = 0;
    uint32_t frame = 0;
    uint32_t slot = 0;
    std::map<NativeFlightResourceKey, NativeFlightResourceReference> resources;
  };

  struct NativeCachedDescriptorState;
  struct NativePipelineCompilerState;

  struct NativeSurfaceImage {
    uint64_t lifetime_id = 0;
    SurfaceDescriptor descriptor{};
    NativeImageResource resource;
    VkImageView sampled_view = VK_NULL_HANDLE;
    VkBuffer depth_handoff_stencil_scratch_buffer = VK_NULL_HANDLE;
    VkDeviceMemory depth_handoff_stencil_scratch_memory = VK_NULL_HANDLE;
    VkDeviceSize depth_handoff_stencil_scratch_size = 0;
    VkDeviceSize depth_handoff_stencil_scratch_allocation_size = 0;
    uint32_t depth_handoff_stencil_scratch_memory_type = UINT32_MAX;
    VkDeviceSize allocation_size = 0;
    bool depth_handoff_stencil_scratch_initialized = false;
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkImageAspectFlags aspect = 0;
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t logical_width = 0;
    uint32_t logical_height = 0;
    NativeReflectionTarget reflection{};
    NativeReflectionCaptureState reflection_capture{};
    bool is_reflection = false;
    bool ever_written = false;
    NativeSurfaceAspectContent aspect_content{};
    GuestPlacementKey materialized_placement{};
    uint64_t materialized_serial = 0;
    uint32_t created_frame = 0;
    uint32_t last_used_frame = 0;
    uint64_t last_used_submission = 0;
  };

  struct NativePlacementOwner {
    enum class WriteKind : uint8_t {
      kUnknown,
      kDrawColor,
      kDrawDepth,
      kImplicitAttachmentClear,
      kExplicitClear,
      kResolveClear,
      kExplicitDepthHandoff,
      kDrawStencil,
    };
    NativeSurfaceImage* image = nullptr;
    GuestSurfaceView view{};
    uint64_t serial = 0;
    uint32_t frame = 0;
    size_t command_index = SIZE_MAX;
    RenderPhase render_phase = RenderPhase::kUnknown;
    WriteKind write_kind = WriteKind::kUnknown;
  };

  struct NativeProducerDepthResolve {
    NativeSurfaceImage* source = nullptr;
    NativeTextureImage* destination = nullptr;
    NativeProducerDepthResolveStamp stamp{};
    bool attached = false;
    bool complete = false;
  };

  struct NativeResolveConversionPipeline {
    VkFormat destination_format = VK_FORMAT_UNDEFINED;
    enum class Kind : uint8_t {
      kResolve,
      kResolveMultisampled,
      kResolveHDRMirror,
      kResolveHDRMirrorMultisampled,
      kDepthResolveMultisampled,
      kDepthHandoff,
      kSceneDepthHandoff,
      kPackedDepthAlias,
    } kind = Kind::kResolve;
    VkSampleCountFlagBits destination_samples = VK_SAMPLE_COUNT_1_BIT;
    VkPipeline pipeline = VK_NULL_HANDLE;
  };

  struct NativeResolveConversionConstants {
    int32_t source_x = 0;
    int32_t source_y = 0;
    int32_t destination_x = 0;
    int32_t destination_y = 0;
    // Guest sample-space topology and physical Vulkan sample count are
    // intentionally distinct. Native quality overrides can back a guest 2x
    // placement view with a host 4x image.
    uint32_t source_guest_sample_type = 0;
    uint32_t requested_guest_sample_type = 0;
    uint32_t destination_guest_sample_type = 0;
    uint32_t sample_select = 0;
    uint32_t mode = 0;
    uint32_t physical_source_sample_type = 0;
    uint32_t physical_destination_sample_type = 0;
    uint32_t flags = 0;
    uint32_t source_width = 0;
    uint32_t source_height = 0;
    uint32_t destination_width = 0;
    uint32_t destination_height = 0;
  };
  static_assert(sizeof(NativeResolveConversionConstants) == 64);

  struct NativeHDRPresentConstants {
    int32_t source_width = 0;
    int32_t source_height = 0;
    int32_t destination_width = 0;
    int32_t destination_height = 0;
    float hdr_headroom = 1.0f;
    uint32_t output_mode = 0;
    uint32_t hdr_mode = 0;
    float paper_white_nits = 203.0f;
    float peak_nits = 400.0f;
    float shoulder_start = 0.0f;
    float shoulder_power = 2.5f;
  };

  struct NativeRenderingTarget {
    std::array<NativeSurfaceImage*, kRenderTargetCount> color_surfaces{};
    std::array<VkImageView, kRenderTargetCount> color_views{};
    std::array<VkFormat, kRenderTargetCount> color_formats{};
    NativeSurfaceImage* depth_surface = nullptr;
    VkImageView depth_view = VK_NULL_HANDLE;
    VkFormat depth_format = VK_FORMAT_UNDEFINED;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    VkSampleCountFlagBits guest_samples = VK_SAMPLE_COUNT_1_BIT;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t logical_width = 0;
    uint32_t logical_height = 0;
    // Only attachments that the current command can access participate in the
    // render scope. Keeping this separate from the bound target arrays prevents
    // an unwritten MRT alias from being cleared, transitioned, or claimed.
    uint32_t color_attachment_mask = 0;
    uint32_t color_write_mask = 0;
    bool depth_stencil_attachment_active = false;
    NativeReflectionTarget reflection{};
    bool is_reflection = false;
    bool uses_presenter = false;
  };

  struct NativePipelineKey {
    uint64_t vertex_shader_hash = 0;
    uint64_t pixel_shader_hash = 0;
    uint64_t shader_variant_key = 0;
    uint64_t vertex_declaration_hash = 0;
    VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
    std::array<uint32_t, kVertexStreamCount> vertex_strides{};
    std::array<VkFormat, kRenderTargetCount> color_formats{};
    VkFormat depth_format = VK_FORMAT_UNDEFINED;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    bool user_pointer = false;
    bool indexed_descriptors = false;
    uint32_t depth_enable = 0;
    uint32_t depth_function = 0;
    uint32_t depth_write_enable = 0;
    uint32_t depth_clamp_enable = 0;
    uint32_t negative_one_to_one_clip_space = 0;
    uint32_t cull_mode = 0;
    uint32_t polygon_mode = 0;
    uint32_t blend_enable_mask = 0;
    std::array<uint32_t, kRenderTargetCount> blend_controls{};
    uint32_t source_blend = 0;
    uint32_t destination_blend = 0;
    uint32_t blend_operation = 0;
    uint32_t source_blend_alpha = 0;
    uint32_t destination_blend_alpha = 0;
    uint32_t blend_operation_alpha = 0;
    uint32_t alpha_test_enable = 0;
    uint32_t alpha_function = 0;
    uint32_t alpha_to_mask_enable = 0;
    uint32_t stencil_enable = 0;
    uint32_t two_sided_stencil = 0;
    uint32_t stencil_fail = 0;
    uint32_t stencil_depth_fail = 0;
    uint32_t stencil_pass = 0;
    uint32_t stencil_function = 0;
    uint32_t stencil_mask = 0;
    uint32_t stencil_write_mask = 0;
    uint32_t ccw_stencil_fail = 0;
    uint32_t ccw_stencil_depth_fail = 0;
    uint32_t ccw_stencil_pass = 0;
    uint32_t ccw_stencil_function = 0;
    uint32_t color_write_mask = 0;
    uint32_t sample_mask = 0;
    bool depth_bias_enable = false;
    bool primitive_restart_enable = false;

    bool operator==(const NativePipelineKey&) const = default;
  };

  struct NativePipelineKeyHash {
    size_t operator()(const NativePipelineKey& key) const noexcept;
  };

  struct NativePipeline {
    NativePipelineKey key{};
    VkPipeline pipeline = VK_NULL_HANDLE;
  };

  struct NativeUploadAllocation {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceSize offset = 0;
    VkDeviceAddress device_address = 0;
    uint8_t* mapping = nullptr;
    const uint8_t* host_data = nullptr;
  };

  enum class NativePersistentBufferKind : uint8_t {
    kVertex,
    kIndex16,
    kIndex32,
  };

  struct NativePersistentBufferKey {
    uint64_t generation = 0;
    uint64_t declaration_hash = 0;
    uint64_t shader_hash = 0;
    uint32_t stream = 0;
    uint32_t stream_offset = 0;
    uint32_t stride = 0;
    NativePersistentBufferKind kind = NativePersistentBufferKind::kVertex;

    bool operator==(const NativePersistentBufferKey&) const = default;
  };

  struct NativePersistentBufferKeyHash {
    size_t operator()(const NativePersistentBufferKey& key) const noexcept;
  };

  struct NativePersistentBufferBlock {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint8_t* mapping = nullptr;
    VkDeviceAddress device_address = 0;
    VkDeviceSize capacity = 0;
    VkDeviceSize allocation_size = 0;
    uint32_t memory_type = UINT32_MAX;
  };

  struct NativePersistentBufferEntry {
    NativePersistentBufferKey key{};
    uint64_t allocation_id = 0;
    uint64_t block_id = 0;
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceSize offset = 0;
    VkDeviceSize size = 0;
    uint64_t last_used_submission = 0;
    uint32_t last_used_frame = 0;
  };

  using NativeConstantBufferKind = FrameConstantKind;

  using NativeSharedConstantSemanticKey = SharedConstantSemanticKey<kTextureStageCount>;

  struct NativeSharedConstantSemanticKeyHash {
    size_t operator()(const NativeSharedConstantSemanticKey& key) const noexcept;
  };

  struct NativeFrameConstantArena {
    NativeUploadBuffer storage;
    FrameConstantArenaIndex index;
    FrameGenerationMap<NativeSharedConstantSemanticKey, uint64_t,
                       NativeSharedConstantSemanticKeyHash>
        shared_versions;
    NativeImmutableBindings<NativeUploadAllocation> immutable_bindings;
    uint64_t next_shared_identity = 1;
  };

  enum class NativeUploadKind : uint8_t {
    kOther,
    kTexture,
    kVertex,
    kIndex,
    kVertexConstants,
    kPixelConstants,
    kSharedConstants,
    kDrawUp,
  };

  struct NativePushConstants {
    VkDeviceAddress vertex_constants = 0;
    VkDeviceAddress pixel_constants = 0;
    VkDeviceAddress shared_constants = 0;
  };

  struct NativeVertexUpload {
    uint64_t declaration_hash = 0;
    uint64_t shader_hash = 0;
    uint32_t stream = 0;
    uint32_t stream_offset = 0;
    uint32_t stride = 0;
    NativeUploadAllocation allocation{};
  };

  struct NativeFrameResources {
    FrameGenerationMap<NativePersistentBufferKey, NativeUploadAllocation, NativePersistentBufferKeyHash> vertex_uploads;
    FrameGenerationMap<uint64_t, NativeUploadAllocation> index16_uploads, index32_uploads;
    bool Reset() {
      if (!vertex_uploads.CanResetGeneration() || !index16_uploads.CanResetGeneration() ||
          !index32_uploads.CanResetGeneration()) return false;
      return vertex_uploads.ResetGeneration() && index16_uploads.ResetGeneration() &&
             index32_uploads.ResetGeneration();
    }
  };

  bool ValidateAndCopyCommand(const void* command, size_t command_size,
                              NativeCommand& native_command);
  void TraceNativeRendererEvent(std::string_view point, std::string_view details = {});
  void InsertNativeGpuPassLabel(VkCommandBuffer command_buffer,const NativeCommand& command,bool force=false);
  PFN_vkCmdInsertDebugUtilsLabelEXT native_gpu_pass_label_function_=nullptr;
  bool native_gpu_pass_label_queried_=false;
  uint64_t native_gpu_pass_label_scope_=UINT64_MAX,native_gpu_pass_label_pipeline_=0;
  uint32_t native_gpu_pass_label_frame_=UINT32_MAX;

  bool NativeRendererEventTraceEnabled() const {
    return deterministic_trace_active_ || fire_event_active_ || bool(phone_frame_trace_) || bool(tv_frame_trace_);
  }
  NativeFixedFunctionState DecodeFixedFunctionState(std::span<const uint8_t> device_state) const;
  static uint64_t HashFixedFunctionState(const NativeFixedFunctionState& state);
  std::shared_ptr<const NativeTextureResource> CreateResolvedTextureResource(
      const ResolveCommand& command);
  std::shared_ptr<const NativeBufferResource> CaptureBufferResource(uint32_t handle);
  SurfaceDescriptor CaptureSurfaceDescriptor(uint32_t handle) const;
  std::shared_ptr<const NativeTextureResource> CaptureTextureResource(
      uint32_t handle, const xenos::xe_gpu_texture_fetch_t& fetch, uint32_t stage);
  void StartRenderWorker();
  void RenderWorkerMain();
  void BeginModernShaderFrame();
  void TraceModernShaderDraw(const NativeCommand& command, VkPipeline pipeline,
                             VkSampleCountFlagBits samples);
  void ApplyStateCommand(const NativeCommand& command);
  bool ApplyShaderConstantDelta(NativeCommand& command, uint32_t device);
  bool InitializeShaderCache();
  static bool ReflectVertexInputs(const std::vector<uint32_t>& spirv,
                                  std::vector<NativeVertexInput>& inputs);
  ShaderOverrideSelection ResolvePipelineShaderOverrides(
      const NativeShader* vertex_shader, const NativeShader* pixel_shader,
      VkSampleCountFlagBits rasterization_samples) const;
  static VkFormat GetCompatibleVertexFormat(uint32_t element_type,
                                            NativeVertexNumericType numeric_type);
  static VkFormat GetDefaultVertexFormat(NativeVertexNumericType numeric_type,
                                         uint32_t component_count);
  void RegisterShader(const RegisterShaderCommand& command);
  void RegisterVertexDeclaration(const RegisterVertexDeclarationCommand& command);
  const NativeShader* FindRegisteredShader(uint32_t handle, ShaderStage stage) const;
  void DestroyShaderResources();
  bool PublishFrame(const PresentCommand& present,
                    const std::shared_ptr<const NativeTextureResource>& present_source = nullptr,
                    const std::shared_ptr<const EnvironmentalDataV1>& environmental_data = nullptr);
  bool ClearGuestOutput(uint32_t width, uint32_t height, uint32_t display_width,
                        uint32_t display_height, const PresentCommand& present,
                        const std::shared_ptr<const NativeTextureResource>& present_source,
                        const std::shared_ptr<const EnvironmentalDataV1>& environmental_data);
  bool InitializeNativeRendererObjects();
  bool InitializeNativePipelineCache();
  void SaveNativePipelineCache();
  void ScheduleNativePipelineCheckpoint();
  void DrainNativePipelineCompiles();
  void StopNativePipelineCompiler();
  void ReplayNativePipelineRecipes();
  void LoadNativePipelineRecipes();
  VkPipeline PublishNativePipeline(const NativePipelineKey& key, VkPipeline pipeline,
                                    uint64_t compile_ticks);
  void RecordNativePipelineTiming(uint64_t compile_ticks, uint64_t wait_ticks = 0);
  bool CreateNativeUploadBuffer(VkDeviceSize capacity, NativeUploadBuffer& upload_buffer);
  void DestroyNativeUploadBuffer(NativeUploadBuffer& upload_buffer);
  bool EnsureFrameUploadCapacity(
      const std::shared_ptr<const NativeTextureResource>& present_source);
  bool InitializeContentProbeBuffer(NativeContentProbeBuffer* buffer = nullptr);
  void DestroyContentProbeBuffer();
  void AnalyzePendingContentProbe(uint32_t slot);
  bool InitializeLightStencilHistogramBuffer(uint32_t width, uint32_t height);
  void DestroyLightStencilHistogramBuffers();
  void AnalyzePendingLightStencilHistogram(uint32_t slot);
  uint64_t SelectedLightColorDeltaInstance() const;
  void PrepareRoomLightProbes(uint32_t submitted_frame);
  bool RecordLightStencilHistogramProbe(VkCommandBuffer command_buffer, NativeSurfaceImage& image,
                                        const NativeCommand& command, uint32_t submitted_frame,
                                        uint32_t command_index, uint64_t draw_id,
                                        uint64_t light_trace_record, uint64_t bulb_instance_id,
                                        bool after_setup);
  bool InitializeLightColorDeltaBuffer(uint32_t width, uint32_t height, VkFormat format);
  void DestroyLightColorDeltaBuffers();
  void AnalyzePendingLightColorDelta(uint32_t slot);
  bool RecordLightColorDeltaProbe(VkCommandBuffer command_buffer,
                                  NativeSurfaceImage& image,
                                  const NativeCommand& command,
                                  uint32_t submitted_frame,
                                  uint32_t command_index,
                                  uint64_t draw_id,
                                  uint64_t instance_id,
                                  bool after_draw);
  bool InitializeTranslucentQueryPool();
  void DestroyTranslucentQueryPool();
  void AnalyzePendingTranslucentQueries(uint32_t slot);
  bool EnqueueDeferredDiagnosticTask(std::function<void()> task);
  void DeferredDiagnosticWorkerMain();
  void ShutdownDeferredDiagnosticWorker();
  void PublishDeferredNativeTrace();
  bool IsNativeFlightRecorderEnabled() const;
  void StageNativeFlightResource(NativeFlightResourceKind kind, uint64_t object,
                                 uint32_t guest_handle = 0, uint64_t guest_generation = 0);
  void StageNativeFlightDescriptorSlot(NativeDescriptorSlotHandle handle, uint32_t guest_handle,
                                       uint64_t guest_generation);
  void StageNativeTextureFlightResources(const NativeTextureImage& image);
  void StageNativeSurfaceFlightResources(const NativeSurfaceImage& image);
  void MarkNativeSurfaceImageUsed(NativeSurfaceImage& image);
  void CommitNativeFlightSubmission(uint32_t slot, uint64_t submission, uint32_t frame);
  bool CompleteNativeFlightSubmission(uint32_t slot, uint64_t submission);
  void DiscardStagedNativeFlightResources();
  void TraceNativeFlightMutation(std::string_view action, NativeFlightResourceKind kind,
                                 uint64_t object, uint32_t page = 0, uint32_t index = 0,
                                 uint32_t generation = 0);
  void NameNativeFlightObject(VkObjectType type, uint64_t object, std::string_view name);
  bool InitializeNativeGpuProfiler();
  void DestroyNativeGpuProfiler();
  void ExportNativeGpuProfile();
  void AnalyzeCompletedNativeGpuProfile(uint32_t slot,
                                        NativeFrameContextRing::QueryReadbackResources resources);
  void PublishCompletedNativeGpuProfileSamples();
  bool BeginNativeGpuProfileFrame(VkCommandBuffer command_buffer, uint32_t submitted_frame,
                                  uint64_t cpu_wait_ticks);
  void SwitchNativeGpuProfileRange(VkCommandBuffer command_buffer, performance::GpuRange range);
  bool SwitchNativeGpuProfileRegion(VkCommandBuffer command_buffer, performance::GpuRange range,
                                    uint32_t attribution_region);
  void EndNativeGpuProfileFrame(VkCommandBuffer command_buffer);
  void CancelNativeGpuProfileFrame();
  void CancelNativeGpuProfileFrame(uint32_t slot);
  bool IsNativeGpuProfileFrameActive() const;
  void AddNativeGpuProfileCounter(performance::Counter counter, uint64_t value = 1);
  void UpdateNativeMemoryProfile(uint32_t submitted_frame, bool force_sample = false);
  memory::Snapshot CollectNativeMemorySnapshot(uint32_t submitted_frame, uint32_t marker);
  void RecordNativeMemoryLifecycle(memory::ResourceKind kind, memory::LifecycleAction action,
                                   memory::LifecycleReason reason, uint64_t identity,
                                   uint64_t generation, uint64_t parent_identity,
                                   uint64_t logical_bytes, uint64_t retained_bytes,
                                   uint64_t allocation_bytes, uint32_t last_used_frame = 0,
                                   uint32_t auxiliary = 0);
  void CaptureNativeRetainedResources(uint32_t submitted_frame, uint32_t marker);
  void RequestNativeProcessVmScan();
  void JoinNativeProcessVmScan();
  void ExportNativeMemoryProfile(std::vector<memory::Snapshot> samples,
                                 std::vector<memory::LifecycleEvent> events,
                                 std::vector<memory::RetainedResource> retained,
                                 uint64_t overwritten_samples, uint64_t overwritten_events,
                                 uint64_t dropped_retained);
  void FinalizeNativeMemoryProfile(uint32_t submitted_frame);
  bool RecordContentProbe(VkCommandBuffer command_buffer, uint32_t submitted_frame,
                          NativeSurfaceImage* final_surface,
                          NativeTextureImage* final_composite_input,
                          const std::shared_ptr<const NativeTextureResource>& present_source,
                          VkImage presenter_image, VkImageLayout presenter_layout,
                          uint32_t presenter_width, uint32_t presenter_height, bool force = false);
  bool RecordContentProbeImage(VkCommandBuffer command_buffer, uint32_t stage_index, VkImage image,
                               VkFormat format, VkImageLayout layout, uint32_t width,
                               uint32_t height, uint32_t handle, uint32_t address, uint8_t kind,
                               uint32_t mip_level = 0, VkImageAspectFlags aspect_override = 0,
                               uint32_t vertical_band = 0, uint32_t vertical_band_count = 1,
                               NativeContentProbeBuffer* probe_buffer = nullptr,
                               const VkRect2D* sample_region = nullptr);
  bool RecordRoomLightInputs(VkCommandBuffer command_buffer, const NativeCommand& command,
                             uint32_t submitted_frame, uint32_t command_index, uint64_t draw_id);
  static void PublishRoomLightInputs(const std::shared_ptr<NativeRoomLightInputs>& inputs,
                              const std::vector<uint8_t>& payload, bool memory_visible);
  bool RecordDepthStencilDiagnosticProbe(VkCommandBuffer command_buffer, NativeSurfaceImage& image,
                                         uint8_t depth_kind, uint8_t stencil_kind,
                                         std::string_view role = {}, uint32_t trace_wrapper = 0,
                                         uint64_t resource_generation = 0,
                                         const NativePlacementOwner* owner = nullptr);
  bool RecordDepthStencilDiagnosticProbe(VkCommandBuffer command_buffer, NativeTextureImage& image,
                                         uint8_t depth_kind, uint8_t stencil_kind,
                                         std::string_view role = {}, uint32_t trace_wrapper = 0,
                                         uint64_t resource_generation = 0,
                                         const NativePlacementOwner* owner = nullptr);
  bool RecordStencilDiagnosticProbe(VkCommandBuffer command_buffer, NativeSurfaceImage& image,
                                    uint8_t kind);
  bool CreateNativeDescriptors();
  bool EnsureIndexedDescriptorPages(uint32_t page_count);
  bool ActivateCachedDescriptorFallback(NativeDescriptorStatus status, const char* reason);
  bool UpdateIndexedImageDescriptor(NativeDescriptorSlotHandle handle,
                                    const std::array<VkImageView, 4>& views);
  bool UpdateIndexedSamplerDescriptor(NativeDescriptorSlotHandle handle, VkSampler sampler);
  bool ClearIndexedTextureDescriptor(NativeDescriptorSlotHandle handle);
  bool ReclaimCompletedNativeTextureDescriptors(uint64_t completed_submission);
  bool AllocateNativeTextureDescriptor(NativeTextureImage& image);
  bool AllocateNativeSamplerDescriptor(NativeSampler& sampler);
  void RetireNativeTextureDescriptor(NativeTextureImage& image);
  bool CreateResolveConversionObjects();
  bool CreateNullImage(VkImageType image_type, VkImageViewType view_type, uint32_t array_layers,
                       VkImageCreateFlags flags, NativeImageResource& resource);
  bool RecordNullImageInitialization(VkCommandBuffer command_buffer);
  bool PrepareFrameDescriptorPool(uint32_t draw_count, uint32_t combined_descriptor_count,
                                  uint32_t combined_set_count);
  bool SynchronizeCachedDescriptorSlot(uint32_t frame_copy);
  bool EnsureCachedDescriptorCapacity(uint32_t frame_copy, uint32_t additional_entries);
  void InvalidateCachedDescriptors(uint64_t image_lifetime);
  VkFormatProperties GetNativeFormatProperties(VkFormat format);
  bool PrepareFrameTextures(VkCommandBuffer command_buffer, bool prepare_present,
                            uint32_t submitted_frame, bool trace_reflections);
  NativeSampler* GetOrCreateSampler(const xenos::xe_gpu_texture_fetch_t& fetch,
                                    const NativeTextureImage* image = nullptr,
                                    NativeSamplerKey* effective_key = nullptr);
  void ReleaseUnusedTextureImages(
      uint32_t submitted_frame, const std::shared_ptr<const NativeTextureResource>& present_source);
  void ReleaseUnusedBufferResources(uint32_t submitted_frame);
  template <typename Visitor>
  static void VisitProtectedTextureGenerations(const NativeCommand& command, Visitor&& visit) {
    const auto texture = [&](const auto& resource) {
      if (!resource) return;
      visit(resource->generation);
      if (resource->packed_depth_source) visit(resource->packed_depth_source->generation);
    };
    texture(command.resolve_destination); texture(command.depth_handoff_source);
    texture(command.present_source);
    for (const auto& resource : command.textures) texture(resource);
  }
  void QueueTextureProtection(const NativeCommand& command, bool retain);
  void AppendQueuedTextureProtection(std::unordered_set<uint64_t>& generations) const;
  void ClearNativeFrameCommands();
  static void AddProtectedTextureGenerations(const NativeCommand& command,
                                             std::unordered_set<uint64_t>& generations);
  std::unordered_set<uint64_t> CollectProtectedTextureGenerations(
      const std::shared_ptr<const NativeTextureResource>& present_source);
  NativeTextureHeapBudgets QueryNativeTextureHeapBudgets() const;
  bool AllocateNativeTextureImage(const VkImageCreateInfo& image_info, NativeTextureImage& image);
  void EvictNativeTextureImages(uint32_t submitted_frame, bool allocation_recovery);
  void RetireNativeTextureImage(std::unique_ptr<NativeTextureImage> image);
  bool ReleaseRetiredTextureImages(bool drain_all = false);
  void DestroyNativeTextureImage(NativeTextureImage& image);
  void DestroyNativeSurfaceImage(NativeSurfaceImage& image);
  void ReleasePendingSurfaceImages();
  void QueueSurfaceImageRelease(uint32_t handle);
  NativeTextureImage* GetOrCreateTextureImage(
      VkCommandBuffer command_buffer, const std::shared_ptr<const NativeTextureResource>& texture);
  NativeSurfaceImage* GetOrCreateSurfaceImage(
      const SurfaceDescriptor& descriptor, bool depth,
      VkSampleCountFlagBits host_sample_override = VK_SAMPLE_COUNT_FLAG_BITS_MAX_ENUM,
      bool allow_allocation = true);
  VkImageView GetOrCreateTextureMipView(NativeTextureImage& image, uint32_t mip_level);
  VkPipeline GetOrCreateFullscreenPipeline(
      VkFormat destination_format, NativeResolveConversionPipeline::Kind kind,
      VkSampleCountFlagBits destination_samples = VK_SAMPLE_COUNT_1_BIT);
  VkPipeline GetOrCreateResolveConversionPipeline(
      VkFormat destination_format, bool multisampled_source,
      VkSampleCountFlagBits destination_samples = VK_SAMPLE_COUNT_1_BIT);
  VkPipeline GetOrCreateDepthResolvePipeline(VkFormat destination_format);
  VkPipeline GetOrCreateDepthHandoffPipeline(
      VkFormat destination_format,
      VkSampleCountFlagBits destination_samples = VK_SAMPLE_COUNT_1_BIT);
  VkPipeline GetOrCreateHDRPresentPipeline();
  bool RecordResolveConversion(VkCommandBuffer command_buffer, NativeSurfaceImage& source,
                               NativeTextureImage& destination, uint32_t destination_level,
                               int32_t source_left, int32_t source_top, uint32_t source_width,
                               uint32_t source_height, int32_t destination_x, int32_t destination_y,
                               uint32_t destination_width, uint32_t destination_height,
                               const GuestSurfaceView& source_view,
                               const GuestSurfaceView& requested_view,
                               xenos::CopySampleSelect sample_select,
                               int32_t color_exponent,
                               NativeTextureImage* hdr_mirror = nullptr);
  bool RecordDepthResolveConversion(VkCommandBuffer command_buffer, NativeSurfaceImage& source,
                                    NativeTextureImage& destination, uint32_t destination_level,
                                    int32_t source_left, int32_t source_top, int32_t destination_x,
                                    int32_t destination_y, uint32_t copy_width,
                                    uint32_t copy_height, const GuestSurfaceView& source_view,
                                    const GuestSurfaceView& requested_view,
                                    xenos::CopySampleSelect sample_select);
  NativeProducerDepthResolve PrepareProducerDepthResolve(
      VkCommandBuffer command_buffer, size_t command_index, const NativeRenderingTarget& target,
      VkRenderingAttachmentInfo& depth_attachment, VkRenderingAttachmentInfo& stencil_attachment,
      NativeProducerDepthResolveScan& scan);
  NativeProducerDepthResolveStamp CaptureProducerDepthResolveStamp(
      size_t command_index, const NativeSurfaceImage& source,
      const NativeTextureImage& destination) const;
  void CompleteProducerDepthResolve(VkCommandBuffer command_buffer,
                                    NativeProducerDepthResolve& producer);
  bool ConsumeProducerDepthResolve(const NativeProducerDepthResolve& producer,
                                  NativeSurfaceImage& source, NativeTextureImage& destination);
  bool RecordSurfaceMaterialization(VkCommandBuffer command_buffer,
                                    const NativePlacementOwner& owner,
                                    NativeSurfaceImage& destination,
                                    const GuestSurfaceView& destination_view);
  bool EnsureDepthHandoffStencilScratch(NativeSurfaceImage& destination);
  bool RecordDepthSurfaceHandoff(VkCommandBuffer command_buffer,
                                 const NativeCommand& native_command, uint32_t submitted_frame);
  bool RecordPackedDepthAlias(VkCommandBuffer command_buffer, NativeTextureImage& destination);
  bool RefreshPackedDepthAliases(VkCommandBuffer command_buffer, uint64_t source_generation);
  bool PrepareSurfaceContent(VkCommandBuffer command_buffer, NativeSurfaceImage& surface,
                             const SurfaceDescriptor& descriptor, bool depth,
                             uint32_t submitted_frame, RenderPhase render_phase);
  void ClaimSurfaceContent(NativeSurfaceImage& surface, const SurfaceDescriptor& descriptor,
                           bool depth, uint32_t submitted_frame, RenderPhase render_phase,
                           NativePlacementOwner::WriteKind write_kind,
                           VkImageAspectFlags written_aspects = 0,
                           uint64_t source_serial = 0);
  const NativePlacementOwner* FindPlacementOwner(const SurfaceDescriptor& descriptor,
                                                 bool depth) const;
  bool HasCurrentPlacementContent(const NativeSurfaceImage& surface,
                                  const SurfaceDescriptor& descriptor, bool depth,
                                  uint32_t submitted_frame) const;
  bool RecordResolveClears(VkCommandBuffer command_buffer, const NativeCommand& command,
                           const ResolveCommand& resolve, uint32_t submitted_frame);
  bool ResolveRenderingTarget(const NativeCommand& command, VkImageView presenter_view,
                              uint32_t presenter_width, uint32_t presenter_height,
                              NativeRenderingTarget& target, bool allow_allocation = true);
  NativeAttachmentUsage GetRenderingTargetUsage(const NativeCommand& command) const;
  bool TransitionRenderingTarget(VkCommandBuffer command_buffer, NativeRenderingTarget& target);
  bool AllocateUpload(VkDeviceSize size, VkDeviceSize alignment, NativeUploadAllocation& allocation,
                      NativeUploadKind kind = NativeUploadKind::kOther);
  VkPipeline GetOrCreatePipeline(const NativePipelineState& state,
                                 const NativeFixedFunctionState& fixed_function_state,
                                 uint32_t primitive_type, const NativeRenderingTarget& target,
                                 uint32_t user_pointer_stride = 0,
                                 bool primitive_restart_enable = false, bool prewarm = false);
  VkPipeline GetOrCreateDrawPipeline(const NativeCommand& command, uint32_t primitive_type,
                                     const NativeRenderingTarget& target,
                                     uint32_t user_pointer_stride = 0,
                                     bool primitive_restart_enable = false, bool prewarm = false);
  void TryPrewarmDrawPipeline(const NativeCommand& command);
  bool GetRequiredVertexStreams(const NativePipelineState& state,
                                std::array<bool, kVertexStreamCount>& required_streams) const;
  bool UploadBufferResource(const std::shared_ptr<const NativeBufferResource>& resource,
                            VkCommandBuffer command_buffer, bool index_buffer, bool index32,
                            const NativePipelineState* vertex_state, uint32_t vertex_stream,
                            NativeFrameResources& resources, NativeUploadAllocation& allocation);
  bool CreateNativePersistentBufferBlock(uint64_t block_id, VkDeviceSize capacity);
  void DestroyNativePersistentBufferBlock(uint64_t block_id);
  void DestroyNativePersistentBuffers();
  bool GetOrCreatePersistentBuffer(VkCommandBuffer command_buffer,
                                   const std::shared_ptr<const NativeBufferResource>& owner,
                                   const NativePersistentBufferKey& key, const uint8_t* source,
                                   VkDeviceSize size, NativeUploadKind upload_kind,
                                   NativeUploadAllocation& allocation);
  bool EnsureFrameConstantArenaCapacity();
  bool FindFrameConstantBuffer(NativeConstantBufferKind kind, uint64_t immutable_identity,
                               NativeUploadAllocation& allocation) const;
  bool GetOrCreateFrameConstantBuffer(NativeConstantBufferKind kind, uint64_t immutable_identity,
                                      std::span<const uint8_t> source_bytes, bool guest_word_order,
                                      NativeUploadAllocation& allocation);
  bool ResetFrameConstantArena(uint32_t slot, uint64_t completed_submission, bool unsubmitted);
  void DestroyNativeFrameConstantArenas();
  void ReleaseUnusedPersistentBuffers(uint64_t completed_submission);
  bool ActivateNativeFrameSlot(uint32_t slot);
  bool CompleteNativeFrameSlot(uint32_t slot, uint64_t submission,
                               uint64_t* profile_processing_ticks = nullptr,
                               uint64_t* actual_wait_ticks = nullptr);
  bool CompleteActiveNativeFrameSlot(uint64_t* profile_processing_ticks = nullptr,
                                     uint64_t* actual_wait_ticks = nullptr);
  bool CompleteSecondaryNativeFrameSlot(uint64_t* profile_processing_ticks = nullptr,
                                        uint64_t* actual_wait_ticks = nullptr);
  void RollbackActiveNativeFrameSlot();
  bool RecoverFailedNativeFrameRecording();
  bool BindCommonDrawState(VkCommandBuffer command_buffer, const NativeCommand& command,
                           VkPipeline pipeline, uint32_t width, uint32_t height,
                           uint32_t logical_width, uint32_t logical_height,
                           VkSampleCountFlagBits samples, NativeFrameResources& resources,
                           const NativeRenderingTarget& target);
  void LogVectorFontDraw(const NativeCommand& command, uint32_t submitted_frame,
                         size_t command_index) const;
  bool IsHoveGantryGeometryCandidate(const NativeCommand& command) const;
  bool RecordNativeFrame(VkCommandBuffer command_buffer, uint32_t width, uint32_t height,
                         VkImage presenter_image, VkImageView presenter_view,
                         uint32_t submitted_frame,
                         const std::shared_ptr<const NativeTextureResource>& present_source,
                         const std::shared_ptr<const EnvironmentalDataV1>& environmental_data,
                         bool hdr_output, float hdr_headroom, bool& presenter_transfer_written,
                         bool& presenter_written, bool trace_stages, uint32_t trace_sequence,
                         bool force_content_probe);
  bool RecordPrimitive(VkCommandBuffer command_buffer, const NativeCommand& command, uint32_t width,
                       uint32_t height, const NativeRenderingTarget& target,
                       NativeFrameResources& resources);
  bool RecordPrimitiveUp(VkCommandBuffer command_buffer, const NativeCommand& command,
                         uint32_t width, uint32_t height, const NativeRenderingTarget& target,
                         NativeFrameResources& resources);
  bool RecordIndexedPrimitive(VkCommandBuffer command_buffer, const NativeCommand& command,
                              uint32_t width, uint32_t height, const NativeRenderingTarget& target,
                              NativeFrameResources& resources);
  bool RecordClear(VkCommandBuffer command_buffer, const NativeCommand& command,
                   const NativeRenderingTarget& target);
  bool RecordResolve(VkCommandBuffer command_buffer, const NativeCommand& command,
                     uint32_t submitted_frame,
                     NativeTextureImage* high_precision_destination = nullptr,
                     bool* high_precision_written = nullptr,
                     const NativeProducerDepthResolve* producer_depth_resolve = nullptr);
  NativeTextureImage* EnsureHDRPresentMirror(uint32_t width, uint32_t height);
  void DestroyHDRPresentMirrors();
  bool RecordPresent(VkCommandBuffer command_buffer, VkImage presenter_image,
                     VkImageView presenter_view, uint32_t presenter_width,
                     uint32_t presenter_height,
                     const std::shared_ptr<const NativeTextureResource>& present_source,
                     NativeTextureImage* high_precision_source, bool hdr_output, float hdr_headroom,
                     bool& transfer_written);
  bool ReadbackTextureToGuest(const TextureLockCommand& command, TextureLockResult& result);
  void DestroyTextureReadbackObjects();
  void DestroyNativeRendererObjects();
  void DestroyVulkanWorkerObjects();

  rex::memory::Memory* memory_ = nullptr;
  ui::WindowedAppContext* app_context_ = nullptr;
  std::unique_ptr<ui::GraphicsProvider> provider_;
  std::unique_ptr<ui::Presenter> presenter_;

  std::mutex render_mutex_;
  std::condition_variable render_condition_;
  std::pmr::synchronized_pool_resource snapshot_pool_;
  std::deque<NativeCommand> render_queue_;
  NativeTextureProtectionIndex queued_texture_protection_; // render_mutex_ owns this.
  uint32_t queued_title_presents_ = 0;
  bool producer_waiting_ = false;
  uint64_t diagnostic_submit_sequence_ = 0;
  uint32_t diagnostic_producer_epoch_ = 1;
  std::atomic<bool> render_worker_running_{false};
  std::thread render_worker_;
  // Authoritative render-worker state. State commands mutate this object in
  // place; immutable snapshots are created only for commands that are retained
  // for later frame recording.
  NativePipelineState pipeline_state_{};
  uint64_t pipeline_snapshot_reuses_=0,shader_snapshot_reuses_=0;
  uint64_t zero_dof_skips_=0,postfx_direct_writes_=0;
  std::shared_ptr<const NativePipelineState> last_pipeline_snapshot_;
  std::shared_ptr<const NativeShaderState> last_shader_snapshot_;
  std::shared_ptr<const NativePipelineState> SnapshotPipeline(const NativeCommand&, bool);
  std::vector<NativeCommand> current_frame_;
  NativeFrameResources recording_resources_;
  std::unordered_set<uint64_t> frame_texture_protection_;
  const NativeCommand* active_worker_command_ = nullptr;
  NativeLightingLineageLedger semantic_light_setup_lineage_;
  std::unordered_map<uint32_t, EnvironmentalDataV1> environmental_data_by_device_;
  std::unordered_map<uint32_t, uint64_t> environmental_data_hash_by_device_;
  std::unordered_map<uint32_t, NativeDeviceConstantState> device_constant_states_;
  std::vector<uint8_t> shader_cache_data_;
  std::vector<std::unique_ptr<NativeShader>> shader_resources_;
  std::unordered_map<uint64_t, NativeShader*> pixel_shaders_by_hash_;
  std::unordered_map<uint64_t, NativeShader*> vertex_shaders_by_hash_;
  std::unordered_map<uint32_t, NativeShader*> shader_handles_;
  std::unordered_map<uint32_t, ProducerShaderMetadata> producer_shader_metadata_;
  std::unordered_map<uint32_t, ProducerDeviceShaderState> producer_device_shader_states_;
  std::unordered_map<uint32_t, std::shared_ptr<NativeVertexDeclaration>> vertex_declarations_;
  std::mutex buffer_resource_mutex_;
  std::unordered_map<uint32_t, std::shared_ptr<const NativeBufferResource>> buffer_resources_;
  std::unordered_set<uint32_t> dirty_buffer_handles_;
  uint64_t next_buffer_generation_ = 1;
  std::mutex texture_resource_mutex_;
  std::unordered_map<uint32_t, std::shared_ptr<const NativeTextureResource>> texture_resources_;
  std::unordered_set<uint32_t> dirty_texture_handles_;
  std::unordered_map<uint32_t, uint32_t> vector_font_ids_;
  NativeVirtualResourceRegistry virtual_resource_registry_;
  uint64_t next_texture_generation_ = 1;
  std::mutex command_capture_mutex_;
  std::mutex device_snapshot_mutex_;
  std::unordered_map<uint32_t, std::shared_ptr<const std::vector<uint8_t>>> last_device_snapshots_;
  std::unordered_set<uint32_t> initialized_constant_capture_devices_;
  uint64_t next_vertex_declaration_generation_ = 1;
  bool shader_cache_load_attempted_ = false;
  bool shader_cache_initialized_ = false;
  ShaderOverrideMode shader_override_mode_ = ShaderOverrideMode::kPair;
  ModernShaderFramePolicy modern_shader_frame_;
  bool modern_shader_trace_ = false;
  uint64_t modern_shader_change_ = 0;
  std::array<uint32_t, size_t(ModernShaderFamily::kCount)> modern_shader_trace_counts_{};
  uint32_t shader_registration_count_ = 0;

  VkCommandPool command_pool_ = VK_NULL_HANDLE;
  VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;
  VkCommandPool secondary_command_pool_ = VK_NULL_HANDLE;
  VkCommandBuffer secondary_command_buffer_ = VK_NULL_HANDLE;
  VkRenderPass clear_render_pass_ = VK_NULL_HANDLE;
  VkFramebuffer clear_framebuffer_ = VK_NULL_HANDLE;
  uint64_t clear_framebuffer_image_version_ = 0;
  std::shared_ptr<ui::vulkan::VulkanSubmissionTracker> submission_tracker_;
  uint64_t command_buffer_submission_ = 0;
  uint64_t secondary_command_buffer_submission_ = 0;
  uint64_t completed_command_buffer_submission_ = 0;
  bool native_renderer_recovery_failed_ = false;
  uint32_t active_frame_slot_ = 0;
  NativeFrameContextRing frame_context_ring_;
  std::array<std::optional<NativeFrameContextRing::FrameToken>, NativeFrameContextRing::kSlotCount>
      frame_context_tokens_{};
  std::array<uint64_t, NativeFrameContextRing::kSlotCount> frame_context_serials_{};
  NativeUploadBuffer upload_buffer_;
  NativeUploadBuffer secondary_upload_buffer_;
  std::vector<NativeUploadBuffer> overflow_upload_buffers_;
  std::vector<NativeUploadBuffer> secondary_overflow_upload_buffers_;
  std::unique_ptr<NativeBufferArena> persistent_buffer_arena_;
  std::shared_ptr<NativeOwnerRetirementQueue<NativePersistentBufferEntry>>
      persistent_buffer_retirements_;
  std::unordered_map<uint64_t, NativePersistentBufferBlock> persistent_buffer_blocks_;
  uint64_t persistent_arena_reclamation_epoch_ = 0;
  std::unordered_map<NativePersistentBufferKey, NativePersistentBufferEntry,
                     NativePersistentBufferKeyHash>
      persistent_buffers_;
  std::array<NativeFrameConstantArena, NativeFrameContextRing::kSlotCount> frame_constant_arenas_{};
  uint64_t persistent_buffer_hits_ = 0;
  uint64_t persistent_buffer_misses_ = 0;
  uint64_t persistent_buffer_upload_bytes_ = 0;
  uint32_t upload_underutilized_frame_count_ = 0;
  uint32_t secondary_upload_underutilized_frame_count_ = 0;
  NativeContentProbeBuffer content_probe_buffer_;
  NativeContentProbeBuffer secondary_content_probe_buffer_;
  NativeContentProbeBuffer tv_content_probe_buffer_, secondary_tv_content_probe_buffer_;
  std::shared_ptr<TvTraceContext> tv_frame_trace_;
  // Worker-only diagnostic provenance; never participates in image ownership.
  std::shared_ptr<TvTraceContext> tv_lifecycle_trace_;
  std::set<uint32_t> tv_lifecycle_plane_handles_;
  uint64_t tv_lifecycle_id_ = 0, tv_lifecycle_command_sequence_ = 0;
  uint32_t tv_lifecycle_batch_frame_ = 0;
  bool IsTvLifecycleImage(const NativeTextureImage& image) const;
  void TraceTvImageLifecycle(std::string_view point, const NativeTextureImage& image,
                            std::string_view reason, std::string_view details = {});
  std::set<std::pair<uint64_t,uint64_t>> tv_exported_cpu_generations_;
  uint32_t TvCommandRole(const NativeCommand& command) const;
  void TraceTvNativeCommand(const NativeCommand& command);
  void RecordTvImage(VkCommandBuffer cb,const NativeCommand& command,uint32_t checkpoint,
      uint32_t attachment,NativeSurfaceImage* surface,NativeTextureImage* texture,std::string_view role);
  void RecordTvTarget(VkCommandBuffer cb,const NativeCommand& command,
      const NativeRenderingTarget& target,uint32_t checkpoint);
  void RecordTvInputs(VkCommandBuffer cb,const NativeCommand& command);
  void RecordTvResolve(VkCommandBuffer cb,const NativeCommand& command,uint32_t checkpoint);
  static void PublishTvProbe(const NativeContentProbeStage& stage,const uint8_t* bytes,bool visible);
  void AnalyzePendingTvProbe(uint32_t slot,uint64_t submission);
  struct NativeBulbFullProbe {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint8_t* mapping = nullptr;
    VkDeviceSize allocation_size = 0;
    uint32_t memory_type = UINT32_MAX;
    uint32_t frame = 0, command_index = 0, fixture = 0, width = 0, height = 0;
    uint32_t format = 0, aspect = 0, block_extent = 1, bytes_per_block = 0, layout = 0, handle = 0;
    uint64_t run = 0, source_sequence = 0, sequence = 0, submission = 0, bytes = 0;
    uint64_t image = 0, view = 0, lifetime = 0, generation = 0, pixel_shader = 0, vertex_shader = 0;
    std::array<uint32_t, 4> components{};
    std::string role;
  };
  struct FireFullProbe {
    NativeBulbFullProbe data;
    VkImage scratch = VK_NULL_HANDLE;
    VkDeviceMemory scratch_memory = VK_NULL_HANDLE;
    uint64_t occurrence = 0, event = 0;
    uint32_t mip = 0, samples = 1;
    uint32_t crop_x = 0, crop_y = 0, original_width = 0, original_height = 0;
  };
  struct FireQueryRow {
    uint32_t frame = 0, command = 0;
    uint64_t sequence = 0, occurrence = 0, event = 0;
  };
  struct FireQueryBatch {
    VkQueryPool pool = VK_NULL_HANDLE;
    uint64_t submission = 0;
    bool ready = false;
    std::vector<FireQueryRow> rows;
  };
  std::array<std::vector<FireFullProbe>, 2> fire_probes_;
  std::array<FireQueryBatch, 2> fire_queries_;
  bool fire_frame_ = false, fire_images_ = false, fire_event_active_ = false;
  uint32_t fire_last_logged_frame_ = 0;
  size_t fire_first_draw_index_ = SIZE_MAX, fire_last_ui_index_ = SIZE_MAX;
  uint64_t fire_frame_bytes_ = 0, fire_cpu_bytes_ = 0, fire_capture_request_ = 0;
  std::unordered_map<std::string, uint32_t> fire_checkpoints_;
  std::unordered_set<uint64_t> fire_input_images_, fire_cpu_resources_;
  std::chrono::steady_clock::time_point fire_last_seen_, fire_last_images_;
  void BeginFireFrame(VkCommandBuffer cb, uint32_t frame);
  void TraceFireCommand(std::string_view point, const NativeCommand& command, std::string_view result);
  bool SelectFireCheckpoint(const NativeCommand& command, const NativeRenderingTarget& target);
  void FireImage(VkCommandBuffer cb, std::string_view role, const NativeCommand* command = nullptr,
      NativeSurfaceImage* surface = nullptr, NativeTextureImage* texture = nullptr,
      VkImage direct = VK_NULL_HANDLE, VkFormat format = VK_FORMAT_UNDEFINED,
      VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED, uint32_t width = 0, uint32_t height = 0,
      VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT, uint32_t mip = 0);
  void FireTarget(VkCommandBuffer cb, const NativeCommand& command, const NativeRenderingTarget& target, bool after);
  void FireInputs(VkCommandBuffer cb, const NativeCommand& command);
  uint32_t BeginFireQuery(VkCommandBuffer cb, const NativeCommand& command, uint32_t existing);
  void EndFireQuery(VkCommandBuffer cb, uint32_t index);
  void PublishFireProbes(uint32_t slot, uint64_t submission);
  void DestroyFireProbes();

  // Indexed by physical frame slot; never swapped with the hot-member aliases.
  std::array<std::vector<NativeBulbFullProbe>, 2> bulb_full_probes_;
  uint32_t bulb_full_frame_ = 0;
  uint64_t bulb_full_bytes_ = 0;
  bool NeedsBulbInterference(const NativeCommand& command, const NativeRenderingTarget& target) const;
  void RecordBulbFullImage(VkCommandBuffer cb, std::string_view role, const NativeCommand* command = nullptr,
      NativeSurfaceImage* surface = nullptr, NativeTextureImage* texture = nullptr,
      VkImage direct = VK_NULL_HANDLE, VkFormat format = VK_FORMAT_UNDEFINED,
      VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED, uint32_t width = 0, uint32_t height = 0,
      VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT);
  void RecordBulbInterferenceBefore(VkCommandBuffer cb, const NativeCommand& command, const NativeRenderingTarget& target);
  void PublishBulbFullProbe(uint32_t slot, uint64_t submission);
  void DestroyBulbFullProbes();
  struct BulbRoute {
    std::shared_ptr<const BulbSourceSnapshot> source;
    uint64_t sequence = 0;
    uint32_t command_index = 0, writers = 0;
    bool recorded = false, budget_reported = false;
    BulbScreenRegion region;
    std::set<std::pair<uint64_t, uint64_t>> images;
    std::set<uint64_t> generations;
  };
  std::vector<BulbRoute> bulb_routes_;
  std::optional<BulbRoute> bulb_prospective_route_;
  bool bulb_trace_frame_ = false;
  std::atomic<bool> bulb_source_capture_done_{false};
  uint32_t bulb_capture_count_ = 0, bulb_last_frame_ = 0;
  uint64_t bulb_run_ = 0;
  uint32_t bulb_selected_fixture_ = UINT32_MAX;
  std::array<uint32_t,kBulbFixtures.size()> bulb_fixture_visits_{};
  const NativeCommand* bulb_current_command_ = nullptr;
  NativeContentProbeBuffer bulb_content_probe_buffer_, secondary_bulb_content_probe_buffer_;
  void CaptureBulbSource(NativeCommand& command, std::span<const uint8_t> device_state);
  void BeginBulbFrame(uint32_t frame);
  void TraceBulbCommand(std::string_view point, const NativeCommand& command, std::string_view reason);
  bool PrepareBulbDraw(const NativeCommand& command, const NativeRenderingTarget& target);
  bool BulbRouteTouches(const BulbRoute& route, const NativeCommand& command, const NativeRenderingTarget& target) const;
  bool NeedsBulbCheckpoint(const NativeCommand& command, const NativeRenderingTarget& target) const;
  void RecordBulbImage(VkCommandBuffer cb, BulbRoute& route, std::string_view role,
      NativeSurfaceImage* surface = nullptr, NativeTextureImage* texture = nullptr,
      VkImage direct_image = VK_NULL_HANDLE, VkFormat direct_format = VK_FORMAT_UNDEFINED,
      VkImageLayout direct_layout = VK_IMAGE_LAYOUT_UNDEFINED, uint32_t direct_width = 0, uint32_t direct_height = 0,
      VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT, bool full_texture = false);
  void RecordBulbPostFx(VkCommandBuffer cb, const NativeCommand& command, NativeTextureImage* image, std::string_view role, bool recorded);
  void RecordBulbBefore(VkCommandBuffer cb, const NativeCommand& command, const NativeRenderingTarget& target);
  void RecordBulbAfter(VkCommandBuffer cb, const NativeCommand& command, const NativeRenderingTarget& target, bool recorded);
  void TraceBulbResolve(VkCommandBuffer cb, const NativeCommand& command, NativeSurfaceImage* source,
      NativeTextureImage* destination, std::string_view result, std::string_view reason);
  void RecordBulbPresentation(VkCommandBuffer cb, NativeSurfaceImage* final_surface, NativeTextureImage* composite_input,
      const std::shared_ptr<const NativeTextureResource>& present_source, VkImage image, VkImageLayout layout,
      uint32_t width, uint32_t height);
  static void PublishBulbProbe(const NativeContentProbeStage& stage, const uint8_t* bytes, bool visible);
  void AnalyzePendingBulbProbe(uint32_t slot, uint64_t submission);
  NativeContentProbeBuffer phone_content_probe_buffer_;
  NativeContentProbeBuffer secondary_phone_content_probe_buffer_;
  std::shared_ptr<PhoneTraceContext> phone_frame_trace_;
  uint64_t phone_record_event_ = 0;
  void TracePhoneNativeCommand(std::string_view point, const NativeCommand& command);
  void RecordPhoneProbe(VkCommandBuffer command_buffer, const NativeCommand& command,
                        const NativeRenderingTarget& target, uint32_t frame,
                        uint32_t command_index, uint32_t point);
  static void PublishPhoneProbe(const NativeContentProbeStage& stage,
                                const uint8_t* bytes, bool visible);
  void AnalyzePendingPhoneProbe(uint32_t slot, uint64_t submission);
  void RecordPhoneLineageImage(VkCommandBuffer command_buffer, const NativeCommand& command,
      uint32_t frame, uint32_t command_index, uint32_t point, uint32_t attachment,
      NativeSurfaceImage* surface, NativeTextureImage* texture, std::string_view role);
  void RecordPhoneLineageInputs(VkCommandBuffer command_buffer, const NativeCommand& command,
      uint32_t frame, uint32_t command_index);
  void RecordPhoneHandoffProbe(VkCommandBuffer command_buffer, const NativeCommand& command,
      uint32_t frame, uint32_t command_index, uint32_t point);
  bool phone_lineage_draw_active_ = false;
  NativeLightStencilHistogramBuffer light_stencil_histogram_buffer_;
  NativeLightStencilHistogramBuffer secondary_light_stencil_histogram_buffer_;
  uint32_t light_stencil_histogram_capture_frame_ = 0;
  bool light_stencil_histogram_capture_complete_ = false;
  uint32_t light_stencil_histogram_readback_frame_ = 0;
  NativeLightColorDeltaBuffer light_color_delta_buffer_;
  NativeLightColorDeltaBuffer secondary_light_color_delta_buffer_;
  uint32_t light_color_delta_capture_frame_ = 0;
  bool light_color_delta_capture_complete_ = false;
  uint32_t light_color_delta_readback_frame_ = 0;
  NativeRoomLightProbeQueue room_light_probe_queue_;
  std::set<NativeRoomLightProbeKey> room_light_probe_unmapped_;
  std::optional<NativeRoomLightProbeKey> room_light_probe_selection_;
  std::shared_ptr<NativeRoomLightProbeReceipt> room_light_probe_receipt_;
  std::vector<std::shared_ptr<NativeRoomLightProbeReceipt>> room_light_probe_receipts_;
  std::set<NativeRoomLightProbeKey> room_light_probe_previous_visible_;
  std::map<size_t, NativeRoomLightProbeKey> room_light_probe_visible_commands_;
  bool room_light_probe_stop_requested_ = false;
  bool room_light_probe_drained_ = false;
  uint32_t room_light_probe_frame_ = 0;
  size_t room_light_probe_command_index_ = SIZE_MAX;
  size_t room_light_probe_setup_index_ = SIZE_MAX;
  bool room_light_probe_stencil_expected_ = false;
  uint32_t light_depth_matrix_capture_frame_ = 0;
  uint8_t light_depth_matrix_capture_mask_ = 0;
  bool light_depth_matrix_capture_complete_ = false;
  NativeTranslucentQueryState translucent_query_state_;
  NativeTranslucentQueryState secondary_translucent_query_state_;
  NativeGpuProfileState native_gpu_profile_state_;
  profile::CpuRecorder native_cpu_recorder_;
  profile::TransportSummary native_profile_transport_;
  uint64_t native_profile_clock_probe_ticks_ = UINT64_MAX;
  struct NativeMemoryProfileState {
    bool active = false;
    bool autostart_consumed = false;
    uint32_t marker = 0;
    uint32_t observed_marker_request = 0;
    uint64_t start_host_tick = 0;
    uint64_t last_sample_host_tick = 0;
    uint64_t last_vm_scan_request_host_tick = 0;
    uint64_t next_retained_capture_index = 0;
    memory::SnapshotSeries samples;
    memory::LifecycleEventSeries events;
    std::vector<memory::RetainedResource> retained;
    uint64_t dropped_retained = 0;
    std::mutex event_mutex;
    std::mutex vm_scan_mutex;
    memory::Snapshot latest_vm_scan;
    std::atomic<bool> vm_scan_running{false};
    std::thread vm_scan_thread;
    std::thread export_thread;
  } native_memory_profile_state_;
  PostFxResourcePool postfx_resource_pool_;
  SmaaPipeline smaa_pipeline_;
  SplitPostFxPass split_postfx_pass_;
  SunShaftsPass sun_shafts_pass_;
  NativeImageResource null_texture_2d_;
  NativeImageResource null_texture_2d_array_;
  NativeImageResource null_texture_3d_;
  NativeImageResource null_texture_cube_;
  VkSampler null_sampler_ = VK_NULL_HANDLE;
  std::array<VkDescriptorSetLayout, 6> descriptor_set_layouts_{};
  std::array<VkDescriptorSetLayout, 5> cached_descriptor_set_layouts_{};
  VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;
  std::array<std::array<VkDescriptorSet, 6>, 2> descriptor_sets_{};
  NativeDescriptorBackend native_descriptor_backend_ = NativeDescriptorBackend::kCached;
  std::unique_ptr<NativeStableDescriptorSlotTable> native_stable_image_descriptor_table_;
  std::unique_ptr<NativeStableDescriptorSlotTable> native_stable_sampler_descriptor_table_;
  std::vector<NativeDescriptorPage> native_descriptor_pages_;
  bool native_descriptor_paging_ = false;
  std::array<NativeDescriptorWorkingSet<kTextureStageCount>, NativeFrameContextRing::kSlotCount>
      native_descriptor_working_sets_;
  bool BeginIndexedWorkingSet();
  template <typename DescriptorKey>
  bool AssignIndexedWorkingSet(NativeCommand& command, const DescriptorKey& key);
  bool PublishIndexedWorkingSet();
  std::unique_ptr<NativeCachedDescriptorState> native_cached_descriptor_state_;
  std::map<uint64_t, std::vector<NativeDescriptorRetirement>> native_descriptor_retirement_journal_;
  bool native_descriptor_layouts_update_after_bind_ = false;
  uint32_t native_descriptor_maximum_page_count_ = 0;
  NativeDrawStateCache<6> native_draw_state_cache_;
  NativeDescriptorSlotHandle native_null_image_descriptor_{};
  NativeDescriptorSlotHandle native_null_sampler_descriptor_{};
  uint32_t active_descriptor_copy_ = 0;
  VkDescriptorPool frame_descriptor_pool_ = VK_NULL_HANDLE;
  VkDescriptorPool secondary_frame_descriptor_pool_ = VK_NULL_HANDLE;
  uint32_t frame_descriptor_draw_capacity_ = 0;
  uint32_t frame_descriptor_resolve_capacity_ = 0;
  uint32_t frame_descriptor_combined_set_capacity_ = 0;
  uint32_t secondary_frame_descriptor_draw_capacity_ = 0;
  uint32_t frame_descriptor_overflow_idle_frames_ = 0;
  uint32_t secondary_frame_descriptor_overflow_idle_frames_ = 0;
  uint32_t secondary_frame_descriptor_resolve_capacity_ = 0;
  uint32_t secondary_frame_descriptor_combined_set_capacity_ = 0;
  uint32_t frame_descriptor_draws_requested_ = 0;
  uint32_t frame_descriptor_unique_draws_ = 0;
  uint32_t frame_descriptor_sets_allocated_ = 0;
  uint32_t frame_descriptor_entries_written_ = 0;
  uint64_t frame_descriptor_pool_reset_count_ = 0;
  uint64_t frame_descriptor_pool_create_count_ = 0;
  uint64_t next_cached_descriptor_epoch_ = 1;
  uint64_t next_native_image_lifetime_ = 0;
  struct ReusableNativeImage {
    NativeImageResource resource{};
    VkDeviceSize bytes=0;
    uint32_t memory_type=UINT32_MAX, memory_heap=UINT32_MAX;
  };
  NativeImageReusePool<ReusableNativeImage> texture_allocation_pool_{33554432, 64};
  bool image_allocation_pool_enabled_=true;
  uint64_t texture_allocation_reuses_=0;
  void TrimTextureAllocationPool(bool all);
  uint64_t command_pool_reset_count_ = 0;
  VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
  VkPipelineCache native_pipeline_cache_ = VK_NULL_HANDLE;
  std::filesystem::path native_pipeline_cache_root_;
  std::filesystem::path native_pipeline_cache_path_;
  uint32_t native_pipeline_cache_title_id_ = 0;
  std::atomic<uint64_t> native_pipeline_cache_generation_{0};
  uint64_t native_pipeline_cache_saved_generation_ = 0;
  std::unique_ptr<NativePipelineCompilerState> native_pipeline_compiler_;
  uint64_t native_pipeline_unreported_compile_ticks_ = 0;
  uint64_t native_pipeline_unreported_wait_ticks_ = 0;
  uint64_t native_pipeline_unreported_creates_ = 0;
  uint32_t diagnostic_submitted_frame_ = 0;
  uint64_t diagnostic_draw_id_ = 0;
  size_t diagnostic_command_index_ = SIZE_MAX;
  RenderPhase diagnostic_render_phase_ = RenderPhase::kUnknown;
  uint32_t diagnostic_render_phase_object_ = 0;
  uint32_t diagnostic_light_trace_id_ = 0;
  uint64_t deterministic_trace_event_ = 0;
  bool deterministic_trace_active_ = false;
  std::vector<std::string> deferred_native_trace_lines_;
  static constexpr size_t kDeferredDiagnosticTaskCapacity = 8;
  std::mutex deferred_diagnostic_mutex_;
  std::condition_variable deferred_diagnostic_condition_;
  std::deque<std::function<void()>> deferred_diagnostic_tasks_;
  std::thread deferred_diagnostic_worker_;
  bool deferred_diagnostic_shutdown_ = false;
  bool deferred_diagnostic_task_running_ = false;
  std::atomic<uint64_t> deferred_diagnostic_drop_count_{0};
  VkDescriptorSetLayout resolve_conversion_descriptor_set_layout_ = VK_NULL_HANDLE;
  VkSampler packed_depth_stencil_sampler_ = VK_NULL_HANDLE;
  VkPipelineLayout resolve_conversion_pipeline_layout_ = VK_NULL_HANDLE;
  VkPipelineLayout cached_pipeline_layout_ = VK_NULL_HANDLE;
  std::unordered_map<NativePipelineKey, NativePipeline, NativePipelineKeyHash> native_pipelines_;
  NativePipelineLookupLifetime native_pipeline_lookup_lifetime_;
  std::vector<NativeResolveConversionPipeline> resolve_conversion_pipelines_;
  VkPipeline hdr_present_pipeline_ = VK_NULL_HANDLE;
  std::array<NativeTextureImage, NativeFrameContextRing::kSlotCount> hdr_present_mirrors_{};
  std::vector<NativeSampler> native_samplers_;
  std::unordered_map<VkFormat, VkFormatProperties> native_format_properties_;
  FrameGenerationMap<uint64_t, NativeTextureImage*> prepared_texture_images_;
  std::unordered_map<NativeSamplerCacheKey, size_t, NativeSamplerCacheKeyHash>
      native_sampler_indices_;
  std::string active_texture_filtering_;
  bool texture_filtering_trace_pending_ = true;
  std::string active_anisotropic_filtering_;
  bool anisotropic_filtering_trace_pending_ = true;
  std::unordered_map<uint64_t, std::unique_ptr<NativeTextureImage>> native_texture_images_;
  std::map<uint64_t, std::vector<std::unique_ptr<NativeTextureImage>>> retired_texture_images_;
  size_t retired_texture_image_count_ = 0;
  std::unordered_set<uint64_t> protected_texture_generations_;
  std::unordered_set<uint64_t> packed_alias_generations_; // Live aliases, not scene history.
  std::map<NativeFlightResourceKey, NativeFlightResourceReference> staged_native_flight_resources_;
  std::array<std::optional<NativeFlightSubmission>, NativeFrameContextRing::kSlotCount>
      submitted_native_flight_resources_{};
  uint32_t active_texture_frame_ = 0;
  uint64_t next_texture_use_serial_ = 1;
  std::array<VkDeviceSize, VK_MAX_MEMORY_HEAPS> accounted_texture_bytes_{};
  NativeTextureEvictionIndex texture_eviction_scan_;
  size_t texture_eviction_remaining_ = 0;
  NativeTextureHeapBudgets texture_budget_snapshot_;
  uint64_t texture_heap_usage_ = 0;
  uint64_t texture_heap_budget_ = 0;
  NativePeriodicWorkSchedule texture_budget_poll_schedule_{kNativeTextureBudgetPollPhaseFrames};
  bool texture_budget_pressure_active_ = false;
  // Written by the title/producer thread while replacing the handle cache,
  // then drained by the render worker before image retirement. Keeping this
  // separate from pending_texture_release_generations_ avoids cross-thread
  // mutation of render-worker lifecycle state.
  std::unordered_set<uint64_t> superseded_texture_release_generations_;
  std::unordered_set<uint64_t> pending_texture_release_generations_;
  std::atomic<uint64_t> buffer_capture_reuse_count_{0};
  std::atomic<uint64_t> buffer_shadow_validation_count_{0};
  std::atomic<uint64_t> buffer_shadow_mismatch_count_{0};
  std::atomic<uint64_t> buffer_fast_path_disable_count_{0};
  std::atomic<uint64_t> buffer_fast_path_request_count_{0};
  std::atomic<bool> buffer_fast_path_disabled_{false};
  NativePeriodicWorkSchedule buffer_cache_poll_schedule_{kNativeBufferCachePollPhaseFrames};
  bool buffer_cache_reclamation_pending_ = false;
  uint64_t texture_image_eviction_count_ = 0;
  uint64_t texture_image_evicted_bytes_ = 0;
  uint64_t texture_allocation_retry_count_ = 0;
  uint64_t texture_allocation_failure_count_ = 0;
  std::vector<std::unique_ptr<NativeSurfaceImage>> native_surface_images_;
  std::unordered_map<uint32_t, std::vector<NativeSurfaceImage*>> surface_images_by_handle_;
  struct NativeTextureReadback {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint32_t memory_type = UINT32_MAX;
    VkDeviceSize memory_size = 0;
    VkDeviceSize capacity = 0;
    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandBuffer command_buffer = VK_NULL_HANDLE;
    VkFence fence = VK_NULL_HANDLE;
    bool pending = false;
  } texture_readback_;
  std::unordered_set<uint64_t> pending_surface_release_ids_;
  uint64_t next_surface_lifetime_id_ = 1;
  std::unordered_map<GuestPlacementKey, NativePlacementOwner, GuestPlacementKeyHash>
      native_placement_owners_;
  uint64_t next_surface_write_serial_ = 1;
  uint64_t next_reflection_capture_epoch_ = 1;
  NativeReflectionRegistry reflection_resources_;
  uint32_t native_descriptor_capacity_ = 0;
  uint32_t native_sampler_descriptor_capacity_ = 0;
  bool null_images_initialized_ = false;
};

}  // namespace rex::graphics::gta4_native
