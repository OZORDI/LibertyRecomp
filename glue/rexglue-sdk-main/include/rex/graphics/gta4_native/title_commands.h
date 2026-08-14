#pragma once

#include <cstdint>
#include <type_traits>

#include <rex/graphics/gta4_native/environmental_data.h>

namespace rex::graphics::gta4_native {

inline constexpr uint32_t kTitleId = 0x545407F2;
inline constexpr uint32_t kTitleCommandAbi = 23;
inline constexpr uint32_t kGuestDeviceSize = 0x5780;
inline constexpr uint32_t kTextureStageCount = 26;
inline constexpr uint32_t kRenderTargetCount = 4;
inline constexpr uint32_t kVertexStreamCount = 17;
inline constexpr uint32_t kMaximumVertexElementCount = 64;

enum class CommandType : uint32_t {
  kDeviceCreated,
  kDeviceDestroyed,
  kRegisterShader,
  kRegisterVertexDeclaration,
  kSetRenderState,
  kSetPixelShader,
  kSetVertexShader,
  kSetVertexDeclaration,
  kSetTexture,
  kInvalidateTexture,
  kSetDepthStencil,
  kSetRenderTarget,
  kSetVertexStream,
  kSetIndexBuffer,
  kDrawPrimitive,
  kDrawPrimitiveUp,
  kDrawIndexedPrimitive,
  kResolve,
  kTextureLock,
  kClear,
  kRenderPhaseMarker,
  kPresent,
  kQueryDeviceCapabilities,
  kRegisterReflectionTarget,
  kReleaseResource,
  kUpdateEnvironmentalData,
  kDepthSurfaceHandoff,
};

enum class ReflectionFamily : uint32_t {
  kMirror,
  kWater,
  kEnvironment,
};

enum class ReflectionRole : uint32_t {
  kColor,
  kDepth,
};

enum class RenderPhase : uint32_t {
  kUnknown,
  kSceneToGBuffer,
  kLightsToScreen,
  kLightSetup,
  kLightDraw,
  kRadarMap,
  kCompositePostFx,
};

enum class RenderPhaseEvent : uint32_t {
  kBegin,
  kEnd,
};

enum class ShaderStage : uint32_t {
  kPixel,
  kVertex,
};

struct CommandHeader {
  uint32_t size;
  CommandType type;
};

struct UpdateEnvironmentalDataCommand {
  CommandHeader header{sizeof(UpdateEnvironmentalDataCommand),
                       CommandType::kUpdateEnvironmentalData};
  uint32_t device = 0;
  uint32_t reserved = 0;
  EnvironmentalDataV1 data{};
};

struct DeviceCommand {
  CommandHeader header;
  uint32_t device;
  uint32_t mode;
};

struct RegisterShaderCommand {
  CommandHeader header{sizeof(RegisterShaderCommand), CommandType::kRegisterShader};
  uint32_t shader;
  ShaderStage stage;
  uint64_t hash;
};

struct VertexElement {
  uint16_t stream;
  uint16_t offset;
  uint32_t type;
  uint8_t method;
  uint8_t usage;
  uint8_t usage_index;
  uint8_t padding;
};

struct RegisterVertexDeclarationCommand {
  CommandHeader header{sizeof(RegisterVertexDeclarationCommand),
                       CommandType::kRegisterVertexDeclaration};
  uint32_t device;
  uint32_t declaration;
  uint32_t element_count;
  uint32_t maximum_stream;
  VertexElement elements[kMaximumVertexElementCount];
};

struct SetRenderStateCommand {
  CommandHeader header{sizeof(SetRenderStateCommand), CommandType::kSetRenderState};
  uint32_t device;
  uint32_t state;
  uint32_t value;
};

struct SetShaderCommand {
  CommandHeader header;
  uint32_t device;
  uint32_t shader;
};

struct SetVertexDeclarationCommand {
  CommandHeader header{sizeof(SetVertexDeclarationCommand), CommandType::kSetVertexDeclaration};
  uint32_t device;
  uint32_t declaration;
};

struct SetTextureCommand {
  CommandHeader header{sizeof(SetTextureCommand), CommandType::kSetTexture};
  uint32_t device;
  uint32_t stage;
  uint32_t texture;
  // Logical GTA IV font identity carried from the grcTexture resource owner
  // through the title's virtual texture accessor. Zero denotes a non-font
  // texture; values 1, 2, and 3 map to font1, font2, and font3 respectively.
  uint32_t vector_font_id = 0;
};

struct InvalidateTextureCommand {
  CommandHeader header{sizeof(InvalidateTextureCommand), CommandType::kInvalidateTexture};
  uint32_t texture;
};

struct ReleaseResourceCommand {
  CommandHeader header{sizeof(ReleaseResourceCommand), CommandType::kReleaseResource};
  uint32_t resource;
};

struct RegisterReflectionTargetCommand {
  CommandHeader header{sizeof(RegisterReflectionTargetCommand),
                       CommandType::kRegisterReflectionTarget};
  ReflectionFamily family = ReflectionFamily::kMirror;
  ReflectionRole role = ReflectionRole::kColor;
  uint32_t wrapper = 0;
  uint32_t surface = 0;
  uint32_t texture = 0;
  uint32_t logical_width = 0;
  uint32_t logical_height = 0;
  uint32_t physical_width = 0;
  uint32_t physical_height = 0;
  // Zero preserves the title-requested sample count. Otherwise this is the
  // native Vulkan capture sample count (1, 2, or 4).
  uint32_t sample_count_override = 0;
};

struct SurfaceDescriptor {
  uint32_t handle = 0;
  uint32_t flags = 0;
  uint32_t base = 0;
  uint32_t address = 0;
  uint32_t packed_dimensions = 0;
  uint32_t format = 0;
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t sample_type = 0;
};

struct SetDepthStencilCommand {
  CommandHeader header{sizeof(SetDepthStencilCommand), CommandType::kSetDepthStencil};
  uint32_t device;
  SurfaceDescriptor surface;
  uint32_t trace_wrapper;
  uint32_t trace_caller;
};

// GTA IV owns two distinct native depth objects across the deferred-to-forward
// phase boundary. The title names that relationship explicitly so the native
// renderer never has to infer resource identity from Xenos placement fields.
// Only depth is transferred; the destination object's stencil contents remain
// owned by the forward path.
struct DepthSurfaceHandoffCommand {
  CommandHeader header{sizeof(DepthSurfaceHandoffCommand),
                       CommandType::kDepthSurfaceHandoff};
  uint32_t device = 0;
  SurfaceDescriptor source;
  SurfaceDescriptor destination;
  uint32_t source_wrapper = 0;
  uint32_t destination_wrapper = 0;
  uint32_t source_texture = 0;
  uint32_t trace_caller = 0;
};

struct SetRenderTargetCommand {
  CommandHeader header{sizeof(SetRenderTargetCommand), CommandType::kSetRenderTarget};
  uint32_t device;
  uint32_t index;
  SurfaceDescriptor surface;
};

struct SetVertexStreamCommand {
  CommandHeader header{sizeof(SetVertexStreamCommand), CommandType::kSetVertexStream};
  uint32_t device;
  uint32_t stream;
  uint32_t buffer;
  uint32_t offset;
  uint32_t stride;
  uint32_t stride_words;
};

struct SetIndexBufferCommand {
  CommandHeader header{sizeof(SetIndexBufferCommand), CommandType::kSetIndexBuffer};
  uint32_t device;
  uint32_t buffer;
};

struct DrawPrimitiveCommand {
  CommandHeader header{sizeof(DrawPrimitiveCommand), CommandType::kDrawPrimitive};
  uint32_t device;
  uint32_t flags;
  uint32_t primitive_type;
  uint32_t start_vertex;
  uint32_t vertex_count;
  uint32_t instance_flags;
  uint64_t depth_bias_bits;
};

struct DrawPrimitiveUpCommand {
  CommandHeader header{sizeof(DrawPrimitiveUpCommand), CommandType::kDrawPrimitiveUp};
  uint32_t device;
  uint32_t primitive_type;
  uint32_t vertex_count;
  uint32_t stride;
  uint32_t vertex_data;
  uint32_t vertex_data_size;
};

struct DrawIndexedPrimitiveCommand {
  CommandHeader header{sizeof(DrawIndexedPrimitiveCommand), CommandType::kDrawIndexedPrimitive};
  uint32_t device;
  uint32_t primitive_type;
  int32_t base_vertex;
  uint32_t start_index;
  uint32_t index_count;
  uint32_t primitive_restart_enabled;
  uint32_t primitive_restart_index;
  uint32_t caller;
  uint32_t origin_flags;
  uint32_t command_list;
  uint64_t draw_id;
};

enum DrawCommandOriginFlags : uint32_t {
  kDrawCommandOriginDirect = 0,
  kDrawCommandOriginCachedReplay = 1u << 0,
};

struct ResolveRectangle {
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
};

struct ResolvePoint {
  int32_t x;
  int32_t y;
};

struct ResolveCommand {
  CommandHeader header{sizeof(ResolveCommand), CommandType::kResolve};
  uint32_t device;
  uint32_t flags;
  SurfaceDescriptor source;
  uint32_t source_rectangle_valid;
  ResolveRectangle source_rectangle;
  uint32_t destination_texture;
  uint32_t destination_fetch[6];
  uint32_t destination_point_valid;
  ResolvePoint destination_point;
  uint32_t destination_level;
  uint32_t destination_slice_or_face;
  uint32_t clear_color_bits[4];
  uint64_t clear_depth_bits;
  uint32_t clear_stencil;
  uint32_t color_format;
  int32_t color_exp_bias;
  uint32_t depth_format;
  uint32_t trace_origin;
  uint32_t trace_caller;
  uint32_t trace_owner;
  uint32_t trace_source_wrapper;
};

enum class TextureLockDimension : uint32_t {
  k2D,
  k3D,
};

struct TextureLockCommand {
  CommandHeader header{sizeof(TextureLockCommand), CommandType::kTextureLock};
  uint32_t texture;
  uint32_t array_index;
  uint32_t level;
  uint32_t flags;
  TextureLockDimension dimension;
};

struct TextureLockResult {
  uint64_t generation;
  uint32_t copied_to_guest;
  uint32_t reserved;
};

struct QueryDeviceCapabilitiesCommand {
  CommandHeader header{sizeof(QueryDeviceCapabilitiesCommand),
                       CommandType::kQueryDeviceCapabilities};
};

struct DeviceCapabilitiesResult {
  uint32_t max_image_dimension_2d;
  uint32_t reserved;
};

struct ClearCommand {
  CommandHeader header{sizeof(ClearCommand), CommandType::kClear};
  uint32_t device;
  uint32_t flags;
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
  uint32_t color_bits[4];
  uint64_t depth_bits;
  uint32_t stencil;
};

struct RenderPhaseMarkerCommand {
  CommandHeader header{sizeof(RenderPhaseMarkerCommand), CommandType::kRenderPhaseMarker};
  uint32_t device;
  RenderPhase phase;
  RenderPhaseEvent event;
  uint32_t object;
  uint32_t caller;
};

struct PresentCommand {
  CommandHeader header{sizeof(PresentCommand), CommandType::kPresent};
  uint32_t device;
  uint32_t frontbuffer_texture;
  uint32_t frontbuffer_fetch[6];
  uint32_t width;
  uint32_t height;
  uint32_t display_width;
  uint32_t display_height;
  uint32_t submitted_frame;
};

static_assert(std::is_trivially_copyable<CommandHeader>::value);
static_assert(std::is_trivially_copyable<UpdateEnvironmentalDataCommand>::value);
static_assert(sizeof(UpdateEnvironmentalDataCommand) == 416);
static_assert(std::is_trivially_copyable<DeviceCommand>::value);
static_assert(std::is_trivially_copyable<RegisterShaderCommand>::value);
static_assert(std::is_trivially_copyable<VertexElement>::value);
static_assert(sizeof(VertexElement) == 12);
static_assert(std::is_trivially_copyable<RegisterVertexDeclarationCommand>::value);
static_assert(std::is_trivially_copyable<SetRenderStateCommand>::value);
static_assert(std::is_trivially_copyable<SetShaderCommand>::value);
static_assert(std::is_trivially_copyable<SetVertexDeclarationCommand>::value);
static_assert(std::is_trivially_copyable<SetTextureCommand>::value);
static_assert(std::is_trivially_copyable<InvalidateTextureCommand>::value);
static_assert(std::is_trivially_copyable<ReleaseResourceCommand>::value);
static_assert(std::is_trivially_copyable<RegisterReflectionTargetCommand>::value);
static_assert(std::is_trivially_copyable<SurfaceDescriptor>::value);
static_assert(std::is_trivially_copyable<SetDepthStencilCommand>::value);
static_assert(std::is_trivially_copyable<DepthSurfaceHandoffCommand>::value);
static_assert(sizeof(DepthSurfaceHandoffCommand) == 100);
static_assert(std::is_trivially_copyable<SetRenderTargetCommand>::value);
static_assert(std::is_trivially_copyable<SetVertexStreamCommand>::value);
static_assert(std::is_trivially_copyable<SetIndexBufferCommand>::value);
static_assert(std::is_trivially_copyable<DrawPrimitiveCommand>::value);
static_assert(std::is_trivially_copyable<DrawPrimitiveUpCommand>::value);
static_assert(std::is_trivially_copyable<DrawIndexedPrimitiveCommand>::value);
static_assert(std::is_trivially_copyable<ResolveRectangle>::value);
static_assert(std::is_trivially_copyable<ResolvePoint>::value);
static_assert(std::is_trivially_copyable<ResolveCommand>::value);
static_assert(std::is_trivially_copyable<TextureLockCommand>::value);
static_assert(std::is_trivially_copyable<TextureLockResult>::value);
static_assert(std::is_trivially_copyable<QueryDeviceCapabilitiesCommand>::value);
static_assert(std::is_trivially_copyable<DeviceCapabilitiesResult>::value);
static_assert(std::is_trivially_copyable<ClearCommand>::value);
static_assert(std::is_trivially_copyable<RenderPhaseMarkerCommand>::value);
static_assert(std::is_trivially_copyable<PresentCommand>::value);

}  // namespace rex::graphics::gta4_native
