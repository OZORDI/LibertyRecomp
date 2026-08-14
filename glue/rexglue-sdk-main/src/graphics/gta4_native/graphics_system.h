#pragma once

#include <array>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <rex/graphics/gta4_native/title_commands.h>
#include <rex/graphics/gta4_native/surface_view.h>
#include <rex/graphics/pipeline/texture/info.h>
#include <rex/system/interfaces/graphics.h>
#include <rex/ui/vulkan/device.h>

#include "postfx_resource_pool.h"
#include "smaa_pipeline.h"
#include "split_postfx_pass.h"
#include "sun_shafts_pass.h"

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
  };

  struct NativeShader {
    ShaderStage stage = ShaderStage::kPixel;
    uint64_t hash = 0;
    uint32_t specialization_constants_mask = 0;
    VkShaderModule early_module = VK_NULL_HANDLE;
    VkShaderModule late_module = VK_NULL_HANDLE;
    std::string filename;
    std::vector<NativeVertexInput> vertex_inputs;
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
    uint32_t cull_mode = 0;
    uint32_t blend_enable = 0;
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
    uint32_t color_write_mask = 0;
    std::array<uint32_t, 6> viewport_bits{};
    std::array<int32_t, 4> scissor{};
  };

  struct NativePipelineState {
    struct VertexStream {
      uint32_t buffer = 0;
      uint32_t offset = 0;
      uint32_t stride = 0;
      uint32_t stride_words = 0;
    };

    std::unordered_map<uint32_t, uint32_t> render_states;
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
  };

  struct NativeBufferResource {
    uint32_t handle = 0;
    uint32_t flags = 0;
    uint32_t guest_address = 0;
    uint32_t guest_size = 0;
    uint64_t content_hash = 0;
    uint64_t generation = 0;
    std::vector<uint8_t> payload;
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
    bool gpu_produced = false;
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

  struct NativeCommand {
    CommandType type = CommandType::kPresent;
    std::vector<uint8_t> bytes;
    std::vector<uint8_t> payload;
    std::vector<uint8_t> device_snapshot;
    std::array<std::shared_ptr<const NativeBufferResource>, kVertexStreamCount> vertex_buffers{};
    std::shared_ptr<const NativeBufferResource> index_buffer;
    std::array<std::shared_ptr<const NativeTextureResource>, kTextureStageCount> textures{};
    std::array<xenos::xe_gpu_texture_fetch_t, kTextureStageCount> texture_fetches{};
    std::shared_ptr<const NativeTextureResource> resolve_destination;
    std::shared_ptr<const NativeTextureResource> depth_handoff_source;
    std::shared_ptr<const NativeTextureResource> present_source;
    std::shared_ptr<const EnvironmentalDataV1> environmental_data;
    std::array<SurfaceDescriptor, kRenderTargetCount> snapshot_render_targets{};
    SurfaceDescriptor snapshot_depth_stencil{};
    std::array<VkDescriptorSet, 5> draw_descriptor_sets{};
    std::array<uint32_t, kTextureStageCount> texture_descriptor_indices{};
    std::array<uint32_t, kTextureStageCount> sampler_descriptor_indices{};
    std::shared_ptr<const NativePipelineState> pipeline_state;
    NativeFixedFunctionState fixed_function_state{};
    RenderPhase render_phase = RenderPhase::kUnknown;
    uint32_t render_phase_object = 0;
    uint64_t vertex_constants_hash = 0;
    uint64_t pixel_constants_hash = 0;
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

  struct NativeContentProbeStage {
    bool valid = false;
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
    uint64_t resource_generation = 0;
    uint32_t provenance_handle = 0;
    uint32_t provenance_frame = 0;
    uint32_t provenance_command = UINT32_MAX;
    uint64_t provenance_serial = 0;
    uint32_t provenance_phase = 0;
    uint32_t provenance_kind = 0;
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
    std::array<NativeContentProbeStage, 128> stages{};
  };

  static constexpr uint32_t kTranslucentQueryCapacity = 1024;

  struct NativeTranslucentQuery {
    bool valid = false;
    bool draw_recorded = false;
    uint32_t frame = 0;
    uint32_t command_index = UINT32_MAX;
    uint32_t render_phase = 0;
    uint64_t draw_id = 0;
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
    std::array<NativeTranslucentQuery, kTranslucentQueryCapacity> queries{};
  };

  static constexpr uint32_t kNativeGpuProfileQueryCapacity = 4096;

  enum class NativeGpuProfileScopeKind : uint8_t {
    kFrame,
    kTexturePreparation,
    kNativeFrame,
    kRenderPhase,
    kCommand,
    kPresent,
  };

  struct NativeGpuProfileSpan {
    NativeGpuProfileScopeKind kind = NativeGpuProfileScopeKind::kCommand;
    RenderPhase render_phase = RenderPhase::kUnknown;
    CommandType command_type = CommandType::kPresent;
    uint32_t command_index = UINT32_MAX;
    uint32_t begin_query = UINT32_MAX;
    uint32_t end_query = UINT32_MAX;
    uint32_t target_handle = 0;
    uint64_t vertex_shader_hash = 0;
    uint64_t pixel_shader_hash = 0;
    uint64_t cpu_ticks = 0;
    bool ended = false;
  };

  struct NativeGpuProfileState {
    VkQueryPool pool = VK_NULL_HANDLE;
    float timestamp_period_ns = 0.0f;
    uint32_t timestamp_valid_bits = 0;
    bool support_checked = false;
    bool supported = false;
    bool active = false;
    bool pending = false;
    bool command_detail = false;
    uint32_t frame = 0;
    uint32_t query_count = 0;
    uint32_t dropped_spans = 0;
    uint32_t last_sampled_frame = 0;
    size_t frame_span = SIZE_MAX;
    std::vector<NativeGpuProfileSpan> spans;
    uint64_t cpu_wait_ticks = 0;
    uint64_t cpu_texture_prepare_ticks = 0;
    uint64_t cpu_record_ticks = 0;
    uint64_t cpu_submit_ticks = 0;
    uint64_t cpu_callback_ticks = 0;
    VkDeviceSize upload_bytes = 0;
    size_t pipelines_before = 0;
    size_t pipelines_after = 0;
    size_t texture_images_before = 0;
    size_t texture_images_after = 0;
    size_t surface_images_before = 0;
    size_t surface_images_after = 0;
    size_t samplers_before = 0;
    size_t samplers_after = 0;
  };

  struct NativeImageResource {
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
  };

  struct NativeReflectionTarget {
    ReflectionFamily family = ReflectionFamily::kMirror;
    ReflectionRole role = ReflectionRole::kColor;
    uint32_t wrapper = 0;
    uint32_t logical_width = 0;
    uint32_t logical_height = 0;
    uint32_t physical_width = 0;
    uint32_t physical_height = 0;
    uint32_t sample_count_override = 0;
  };

  struct NativeTextureImage {
    std::shared_ptr<const NativeTextureResource> source;
    NativeImageResource resource;
    VkFormat format = VK_FORMAT_UNDEFINED;
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
    uint32_t guest_mip_levels = 1;
    uint32_t mip_levels = 1;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t logical_width = 0;
    uint32_t logical_height = 0;
    NativeReflectionTarget reflection{};
    bool is_reflection = false;
    std::vector<VkImageView> mip_views;
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
  };

  struct NativeSurfaceImage {
    SurfaceDescriptor descriptor{};
    NativeImageResource resource;
    VkImageView sampled_view = VK_NULL_HANDLE;
    VkBuffer depth_handoff_stencil_scratch_buffer = VK_NULL_HANDLE;
    VkDeviceMemory depth_handoff_stencil_scratch_memory = VK_NULL_HANDLE;
    VkDeviceSize depth_handoff_stencil_scratch_size = 0;
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
    bool is_reflection = false;
    bool ever_written = false;
    GuestPlacementKey materialized_placement{};
    uint64_t materialized_serial = 0;
  };

  struct NativePlacementOwner {
    enum class WriteKind : uint8_t {
      kUnknown,
      kDrawColor,
      kDrawDepth,
      kExplicitClear,
      kResolveClear,
      kExplicitDepthHandoff,
    };
    NativeSurfaceImage* image = nullptr;
    GuestSurfaceView view{};
    uint64_t serial = 0;
    uint32_t frame = 0;
    size_t command_index = SIZE_MAX;
    RenderPhase render_phase = RenderPhase::kUnknown;
    WriteKind write_kind = WriteKind::kUnknown;
  };

  struct NativeResolveConversionPipeline {
    VkFormat destination_format = VK_FORMAT_UNDEFINED;
    enum class Kind : uint8_t {
      kResolve,
      kResolveMultisampled,
      kDepthResolveMultisampled,
      kReflectionMip,
    } kind = Kind::kResolve;
    VkSampleCountFlagBits destination_samples = VK_SAMPLE_COUNT_1_BIT;
    VkPipeline pipeline = VK_NULL_HANDLE;
  };

  struct NativeResolveConversionConstants {
    int32_t source_x = 0;
    int32_t source_y = 0;
    int32_t destination_x = 0;
    int32_t destination_y = 0;
    uint32_t source_sample_type = 0;
    uint32_t requested_sample_type = 0;
    uint32_t destination_sample_type = 0;
    uint32_t sample_select = 0;
    uint32_t mode = 0;
    std::array<uint32_t, 3> reserved{};
  };

  struct NativeReflectionMipConstants {
    uint32_t destination_level = 0;
    std::array<uint32_t, 3> reserved{};
  };

  struct NativeHDRPresentConstants {
    int32_t source_width = 0;
    int32_t source_height = 0;
    int32_t destination_width = 0;
    int32_t destination_height = 0;
    float hdr_headroom = 1.0f;
    uint32_t output_mode = 0;
  };

  struct NativeRenderingTarget {
    std::array<NativeSurfaceImage*, kRenderTargetCount> color_surfaces{};
    std::array<VkImageView, kRenderTargetCount> color_views{};
    std::array<VkFormat, kRenderTargetCount> color_formats{};
    NativeSurfaceImage* depth_surface = nullptr;
    VkImageView depth_view = VK_NULL_HANDLE;
    VkFormat depth_format = VK_FORMAT_UNDEFINED;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t logical_width = 0;
    uint32_t logical_height = 0;
    NativeReflectionTarget reflection{};
    bool is_reflection = false;
    bool uses_presenter = false;
  };

  struct NativePipelineKey {
    uint64_t vertex_shader_hash = 0;
    uint64_t pixel_shader_hash = 0;
    uint64_t vertex_declaration_hash = 0;
    uint32_t primitive_type = 0;
    std::array<uint32_t, kVertexStreamCount> vertex_strides{};
    std::array<VkFormat, kRenderTargetCount> color_formats{};
    VkFormat depth_format = VK_FORMAT_UNDEFINED;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    bool user_pointer = false;
    uint32_t depth_enable = 0;
    uint32_t depth_function = 0;
    uint32_t depth_write_enable = 0;
    uint32_t cull_mode = 0;
    uint32_t blend_enable = 0;
    uint32_t source_blend = 0;
    uint32_t destination_blend = 0;
    uint32_t blend_operation = 0;
    uint32_t source_blend_alpha = 0;
    uint32_t destination_blend_alpha = 0;
    uint32_t blend_operation_alpha = 0;
    uint32_t alpha_test_enable = 0;
    uint32_t alpha_function = 0;
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
    bool depth_bias_enable = false;
    bool primitive_restart_enable = false;

    bool operator==(const NativePipelineKey&) const = default;
  };

  struct NativePipeline {
    NativePipelineKey key{};
    VkPipeline pipeline = VK_NULL_HANDLE;
  };

  struct NativeUploadAllocation {
    VkDeviceSize offset = 0;
    VkDeviceAddress device_address = 0;
    uint8_t* mapping = nullptr;
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
    std::unordered_map<const NativeBufferResource*, std::vector<NativeVertexUpload>>
        vertex_uploads;
    std::unordered_map<const NativeBufferResource*, NativeUploadAllocation> index16_uploads;
    std::unordered_map<const NativeBufferResource*, NativeUploadAllocation> index32_uploads;
  };

  bool ValidateAndCopyCommand(const void* command, size_t command_size,
                              NativeCommand& native_command);
  void TraceNativeRendererEvent(std::string_view point, std::string_view details = {});
  NativeFixedFunctionState DecodeFixedFunctionState(
      const std::vector<uint8_t>& device_snapshot) const;
  std::shared_ptr<const NativeTextureResource> CreateResolvedTextureResource(
      const ResolveCommand& command);
  std::shared_ptr<const NativeBufferResource> CaptureBufferResource(uint32_t handle);
  SurfaceDescriptor CaptureSurfaceDescriptor(uint32_t handle) const;
  std::shared_ptr<const NativeTextureResource> CaptureTextureResource(
      uint32_t handle, const std::vector<uint8_t>& device_snapshot, uint32_t stage);
  void StartRenderWorker();
  void RenderWorkerMain();
  void ApplyStateCommand(const NativeCommand& command);
  bool InitializeShaderCache();
  static bool ReflectVertexInputs(const std::vector<uint32_t>& spirv,
                                  std::vector<NativeVertexInput>& inputs);
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
                        uint32_t display_height, uint32_t submitted_frame,
                        const std::shared_ptr<const NativeTextureResource>& present_source,
                        const std::shared_ptr<const EnvironmentalDataV1>& environmental_data);
  bool InitializeNativeRendererObjects();
  bool InitializeNativePipelineCache();
  void SaveNativePipelineCache();
  bool CreateNativeUploadBuffer(VkDeviceSize capacity, NativeUploadBuffer& upload_buffer);
  void DestroyNativeUploadBuffer(NativeUploadBuffer& upload_buffer);
  bool EnsureFrameUploadCapacity(
      const std::shared_ptr<const NativeTextureResource>& present_source);
  bool InitializeContentProbeBuffer();
  void DestroyContentProbeBuffer();
  void AnalyzePendingContentProbe();
  bool InitializeTranslucentQueryPool();
  void DestroyTranslucentQueryPool();
  void AnalyzePendingTranslucentQueries();
  bool InitializeNativeGpuProfiler();
  void DestroyNativeGpuProfiler();
  void AnalyzePendingNativeGpuProfile();
  bool BeginNativeGpuProfileFrame(VkCommandBuffer command_buffer,
                                  uint32_t submitted_frame,
                                  uint64_t cpu_wait_ticks);
  void EndNativeGpuProfileFrame(VkCommandBuffer command_buffer);
  void CancelNativeGpuProfileFrame();
  size_t BeginNativeGpuProfileSpan(VkCommandBuffer command_buffer,
                                   NativeGpuProfileScopeKind kind,
                                   RenderPhase render_phase = RenderPhase::kUnknown,
                                   CommandType command_type = CommandType::kPresent,
                                   uint32_t command_index = UINT32_MAX,
                                   const NativeCommand* command = nullptr);
  void EndNativeGpuProfileSpan(VkCommandBuffer command_buffer, size_t span_index,
                               uint64_t cpu_begin_ticks = 0);
  bool RecordContentProbe(
      VkCommandBuffer command_buffer, uint32_t submitted_frame,
      NativeSurfaceImage* final_surface, NativeTextureImage* final_composite_input,
      const std::shared_ptr<const NativeTextureResource>& present_source,
      VkImage presenter_image, VkImageLayout presenter_layout,
      uint32_t presenter_width, uint32_t presenter_height);
  bool RecordContentProbeImage(VkCommandBuffer command_buffer, uint32_t stage_index,
                               VkImage image, VkFormat format, VkImageLayout layout,
                               uint32_t width, uint32_t height, uint32_t handle,
                               uint32_t address, uint8_t kind, uint32_t mip_level = 0,
                               VkImageAspectFlags aspect_override = 0);
  bool RecordDepthStencilDiagnosticProbe(VkCommandBuffer command_buffer,
                                         NativeSurfaceImage& image,
                                         uint8_t depth_kind, uint8_t stencil_kind,
                                         std::string_view role = {},
                                         uint32_t trace_wrapper = 0,
                                         uint64_t resource_generation = 0,
                                         const NativePlacementOwner* owner = nullptr);
  bool RecordDepthStencilDiagnosticProbe(VkCommandBuffer command_buffer,
                                         NativeTextureImage& image,
                                         uint8_t depth_kind, uint8_t stencil_kind,
                                         std::string_view role = {},
                                         uint32_t trace_wrapper = 0,
                                         uint64_t resource_generation = 0,
                                         const NativePlacementOwner* owner = nullptr);
  bool RecordStencilDiagnosticProbe(VkCommandBuffer command_buffer,
                                    NativeSurfaceImage& image, uint8_t kind);
  bool CreateNativeDescriptors();
  bool CreateResolveConversionObjects();
  bool CreateNullImage(VkImageType image_type, VkImageViewType view_type, uint32_t array_layers,
                       VkImageCreateFlags flags, NativeImageResource& resource);
  bool RecordNullImageInitialization(VkCommandBuffer command_buffer);
  bool PrepareFrameDescriptorPool(uint32_t draw_count, uint32_t combined_descriptor_count,
                                  uint32_t combined_set_count);
  bool PrepareFrameTextures(VkCommandBuffer command_buffer, bool prepare_present,
                            uint32_t submitted_frame, bool trace_reflections);
  VkSampler GetOrCreateSampler(const xenos::xe_gpu_texture_fetch_t& fetch,
                               const NativeTextureImage* image = nullptr,
                               NativeSamplerKey* effective_key = nullptr);
  void ReleaseUnusedTextureImages();
  NativeTextureImage* GetOrCreateTextureImage(
      VkCommandBuffer command_buffer, const std::shared_ptr<const NativeTextureResource>& texture);
  NativeSurfaceImage* GetOrCreateSurfaceImage(const SurfaceDescriptor& descriptor, bool depth);
  VkImageView GetOrCreateTextureMipView(NativeTextureImage& image, uint32_t mip_level);
  VkPipeline GetOrCreateFullscreenPipeline(
      VkFormat destination_format, NativeResolveConversionPipeline::Kind kind,
      VkSampleCountFlagBits destination_samples = VK_SAMPLE_COUNT_1_BIT);
  VkPipeline GetOrCreateResolveConversionPipeline(VkFormat destination_format,
                                                  bool multisampled_source,
                                                  VkSampleCountFlagBits destination_samples =
                                                      VK_SAMPLE_COUNT_1_BIT);
  VkPipeline GetOrCreateDepthResolvePipeline(VkFormat destination_format);
  VkPipeline GetOrCreateReflectionMipPipeline(VkFormat destination_format);
  VkPipeline GetOrCreateHDRPresentPipeline();
  bool RecordResolveConversion(VkCommandBuffer command_buffer, NativeSurfaceImage& source,
                               NativeTextureImage& destination, uint32_t destination_level,
                               int32_t source_left, int32_t source_top, int32_t destination_x,
                               int32_t destination_y, uint32_t copy_width, uint32_t copy_height,
                               const GuestSurfaceView& source_view,
                               const GuestSurfaceView& requested_view,
                               xenos::CopySampleSelect sample_select);
  bool RecordDepthResolveConversion(VkCommandBuffer command_buffer,
                                    NativeSurfaceImage& source,
                                    NativeTextureImage& destination,
                                    uint32_t destination_level,
                                    int32_t source_left, int32_t source_top,
                                    int32_t destination_x, int32_t destination_y,
                                    uint32_t copy_width, uint32_t copy_height,
                                    const GuestSurfaceView& source_view,
                                    const GuestSurfaceView& requested_view,
                                    xenos::CopySampleSelect sample_select);
  bool RecordSurfaceMaterialization(VkCommandBuffer command_buffer,
                                    const NativePlacementOwner& owner,
                                    NativeSurfaceImage& destination,
                                    const GuestSurfaceView& destination_view);
  bool EnsureDepthHandoffStencilScratch(NativeSurfaceImage& destination);
  bool RecordDepthSurfaceHandoff(VkCommandBuffer command_buffer,
                                 const NativeCommand& native_command,
                                 uint32_t submitted_frame);
  bool PrepareSurfaceContent(VkCommandBuffer command_buffer, NativeSurfaceImage& surface,
                             const SurfaceDescriptor& descriptor, bool depth,
                             uint32_t submitted_frame, RenderPhase render_phase);
  void ClaimSurfaceContent(NativeSurfaceImage& surface, const SurfaceDescriptor& descriptor,
                           bool depth, uint32_t submitted_frame, RenderPhase render_phase,
                           NativePlacementOwner::WriteKind write_kind);
  const NativePlacementOwner* FindPlacementOwner(const SurfaceDescriptor& descriptor,
                                                  bool depth) const;
  bool HasCurrentPlacementContent(const NativeSurfaceImage& surface,
                                  const SurfaceDescriptor& descriptor, bool depth) const;
  bool RecordResolveClears(VkCommandBuffer command_buffer, const NativeCommand& command,
                           const ResolveCommand& resolve, uint32_t submitted_frame);
  bool GenerateReflectionTailMips(VkCommandBuffer command_buffer,
                                  NativeTextureImage& destination);
  bool ResolveRenderingTarget(const NativePipelineState& state, VkImageView presenter_view,
                              uint32_t presenter_width, uint32_t presenter_height,
                              NativeRenderingTarget& target);
  bool TransitionRenderingTarget(VkCommandBuffer command_buffer, NativeRenderingTarget& target);
  bool AllocateUpload(VkDeviceSize size, VkDeviceSize alignment,
                      NativeUploadAllocation& allocation);
  VkPipeline GetOrCreatePipeline(const NativePipelineState& state,
                                 const NativeFixedFunctionState& fixed_function_state,
                                 uint32_t primitive_type, const NativeRenderingTarget& target,
                                 uint32_t user_pointer_stride = 0,
                                 bool primitive_restart_enable = false);
  bool GetRequiredVertexStreams(
      const NativePipelineState& state,
      std::array<bool, kVertexStreamCount>& required_streams) const;
  bool UploadBufferResource(const std::shared_ptr<const NativeBufferResource>& resource,
                            bool index_buffer, bool index32,
                            const NativePipelineState* vertex_state, uint32_t vertex_stream,
                            NativeFrameResources& resources, NativeUploadAllocation& allocation);
  bool BindCommonDrawState(VkCommandBuffer command_buffer, const NativeCommand& command,
                           VkPipeline pipeline, uint32_t width, uint32_t height,
                           uint32_t logical_width, uint32_t logical_height);
  void LogVectorFontDraw(const NativeCommand& command, uint32_t submitted_frame,
                         size_t command_index) const;
  bool RecordNativeFrame(VkCommandBuffer command_buffer, uint32_t width, uint32_t height,
                         VkImage presenter_image, VkImageView presenter_view,
                         uint32_t submitted_frame,
                         const std::shared_ptr<const NativeTextureResource>& present_source,
                         const std::shared_ptr<const EnvironmentalDataV1>& environmental_data,
                         bool hdr_output, float hdr_headroom,
                         bool& presenter_transfer_written, bool trace_stages,
                         uint32_t trace_sequence);
  bool RecordPrimitive(VkCommandBuffer command_buffer, const NativeCommand& command, uint32_t width,
                       uint32_t height, const NativeRenderingTarget& target,
                       NativeFrameResources& resources);
  bool RecordPrimitiveUp(VkCommandBuffer command_buffer, const NativeCommand& command,
                         uint32_t width, uint32_t height, const NativeRenderingTarget& target);
  bool RecordIndexedPrimitive(VkCommandBuffer command_buffer, const NativeCommand& command,
                              uint32_t width, uint32_t height,
                              const NativeRenderingTarget& target,
                              NativeFrameResources& resources);
  bool RecordClear(VkCommandBuffer command_buffer, const NativeCommand& command,
                   const NativeRenderingTarget& target);
  bool RecordResolve(VkCommandBuffer command_buffer, const NativeCommand& command,
                     uint32_t submitted_frame);
  bool RecordPresent(VkCommandBuffer command_buffer, VkImage presenter_image,
                     VkImageView presenter_view, uint32_t presenter_width,
                     uint32_t presenter_height,
                     const std::shared_ptr<const NativeTextureResource>& present_source,
                     NativeSurfaceImage* high_precision_source, bool hdr_output,
                     float hdr_headroom, bool& transfer_written);
  bool ReadbackTextureToGuest(const TextureLockCommand& command, TextureLockResult& result);
  void DestroyNativeRendererObjects();
  void DestroyVulkanWorkerObjects();

  memory::Memory* memory_ = nullptr;
  ui::WindowedAppContext* app_context_ = nullptr;
  std::unique_ptr<ui::GraphicsProvider> provider_;
  std::unique_ptr<ui::Presenter> presenter_;

  std::mutex render_mutex_;
  std::condition_variable render_condition_;
  std::deque<NativeCommand> render_queue_;
  std::atomic<bool> render_worker_running_{false};
  std::thread render_worker_;
  std::shared_ptr<NativePipelineState> pipeline_state_ = std::make_shared<NativePipelineState>();
  std::vector<NativeCommand> current_frame_;
  std::unordered_map<uint32_t, EnvironmentalDataV1> environmental_data_by_device_;
  std::vector<uint8_t> shader_cache_data_;
  std::vector<std::unique_ptr<NativeShader>> shader_resources_;
  std::unordered_map<uint64_t, NativeShader*> pixel_shaders_by_hash_;
  std::unordered_map<uint64_t, NativeShader*> vertex_shaders_by_hash_;
  std::unordered_map<uint32_t, NativeShader*> shader_handles_;
  std::unordered_map<uint32_t, std::shared_ptr<NativeVertexDeclaration>> vertex_declarations_;
  std::mutex buffer_resource_mutex_;
  std::unordered_map<uint32_t, std::shared_ptr<const NativeBufferResource>> buffer_resources_;
  uint64_t next_buffer_generation_ = 1;
  std::mutex texture_resource_mutex_;
  std::unordered_map<uint32_t, std::shared_ptr<const NativeTextureResource>> texture_resources_;
  std::unordered_set<uint32_t> dirty_texture_handles_;
  std::unordered_map<uint32_t, uint32_t> vector_font_ids_;
  uint64_t next_texture_generation_ = 1;
  uint64_t next_vertex_declaration_generation_ = 1;
  bool shader_cache_load_attempted_ = false;
  bool shader_cache_initialized_ = false;
  uint32_t shader_registration_count_ = 0;

  VkCommandPool command_pool_ = VK_NULL_HANDLE;
  VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;
  VkRenderPass clear_render_pass_ = VK_NULL_HANDLE;
  VkFramebuffer clear_framebuffer_ = VK_NULL_HANDLE;
  uint64_t clear_framebuffer_image_version_ = 0;
  std::unique_ptr<ui::vulkan::VulkanSubmissionTracker> submission_tracker_;
  uint64_t command_buffer_submission_ = 0;

  NativeUploadBuffer upload_buffer_;
  NativeContentProbeBuffer content_probe_buffer_;
  NativeTranslucentQueryState translucent_query_state_;
  NativeGpuProfileState native_gpu_profile_state_;
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
  VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;
  std::array<VkDescriptorSet, 6> descriptor_sets_{};
  VkDescriptorPool frame_descriptor_pool_ = VK_NULL_HANDLE;
  uint32_t frame_descriptor_draw_capacity_ = 0;
  uint32_t frame_descriptor_resolve_capacity_ = 0;
  uint32_t frame_descriptor_combined_set_capacity_ = 0;
  VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
  VkPipelineCache native_pipeline_cache_ = VK_NULL_HANDLE;
  std::filesystem::path native_pipeline_cache_root_;
  std::filesystem::path native_pipeline_cache_path_;
  uint32_t native_pipeline_cache_title_id_ = 0;
  bool native_pipeline_cache_dirty_ = false;
  uint32_t diagnostic_submitted_frame_ = 0;
  uint64_t diagnostic_draw_id_ = 0;
  size_t diagnostic_command_index_ = SIZE_MAX;
  uint64_t deterministic_trace_event_ = 0;
  bool deterministic_trace_active_ = false;
  VkDescriptorSetLayout resolve_conversion_descriptor_set_layout_ = VK_NULL_HANDLE;
  VkPipelineLayout resolve_conversion_pipeline_layout_ = VK_NULL_HANDLE;
  std::vector<NativePipeline> native_pipelines_;
  std::vector<NativeResolveConversionPipeline> resolve_conversion_pipelines_;
  VkPipeline hdr_present_pipeline_ = VK_NULL_HANDLE;
  std::vector<NativeSampler> native_samplers_;
  std::unordered_map<uint64_t, std::unique_ptr<NativeTextureImage>> native_texture_images_;
  std::vector<std::unique_ptr<NativeSurfaceImage>> native_surface_images_;
  std::unordered_map<GuestPlacementKey, NativePlacementOwner, GuestPlacementKeyHash>
      native_placement_owners_;
  uint64_t next_surface_write_serial_ = 1;
  std::unordered_map<uint32_t, NativeReflectionTarget> reflection_resources_;
  uint32_t native_descriptor_capacity_ = 0;
  bool null_images_initialized_ = false;
};

}  // namespace rex::graphics::gta4_native
