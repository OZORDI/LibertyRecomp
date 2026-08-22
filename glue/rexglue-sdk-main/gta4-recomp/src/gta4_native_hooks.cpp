#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <fmt/format.h>
#include <xxhash.h>

#include <rex/cvar.h>
#include <rex/diagnostics/policy.h>
#include <rex/graphics/video_mode_util.h>
#include <rex/graphics/gta4_native/surface_view.h>
#include <rex/graphics/gta4_native/title_commands.h>
#include <rex/logging.h>
#include <rex/runtime.h>
#include <rex/ui/flags.h>
#include <rex/ui/window.h>

#include "gta4_init.h"

REXCVAR_DECLARE(uint32_t, gta4_shadow_map_base_size);
REXCVAR_DECLARE(double, gta4_shadow_distance_scale);
REXCVAR_DECLARE(std::string, gta4_reflection_resolution);
REXCVAR_DECLARE(std::string, gta4_reflection_resolution_cap);
REXCVAR_DECLARE(std::string, gta4_mirror_reflection_resolution);
REXCVAR_DECLARE(std::string, gta4_water_reflection_resolution);
REXCVAR_DECLARE(std::string, gta4_environment_reflection_resolution);
REXCVAR_DECLARE(std::string, gta4_reflection_aa);
REXCVAR_DECLARE(std::string, gta4_reflection_capture_distance);
REXCVAR_DECLARE(std::string, gta4_native_upscaler);
REXCVAR_DECLARE(std::string, gta4_fsr1_quality);
REXCVAR_DECLARE(std::string, gta4_aspect_ratio);
REXCVAR_DECLARE(bool, gta4_force_highest_lod);
REXCVAR_DECLARE(double, gta4_draw_distance_scale);
REXCVAR_DECLARE(bool, gta4_disable_timecycle_far_clip);
REXCVAR_DECLARE(uint32_t, gta4_drawable_reference_limit);
REXCVAR_DECLARE(bool, gta4_disable_model_budget_reduction);
REXCVAR_DEFINE_BOOL(gta4_native_pixel_snap_fonts, true, "GTA IV/Graphics/Text",
                    "Snap GTA IV font quads to native framebuffer pixels");

namespace {

using namespace rex::graphics::gta4_native;
using GuestFunction = void (*)(PPCContext&, uint8_t*);

constexpr uint32_t kOutOfMemory = 0x8007000E;
constexpr uint32_t kAlignedAllocationFreeFlags = 0x24800000;
constexpr uint32_t kFallbackAllocationFreeFlags = 0xB1800000;
constexpr uint32_t kVdGlobalDeviceImport = 0x8200078C;
constexpr uint32_t kStreamBufferBase = 12452;
constexpr uint32_t kStreamSizeBase = 1916;
constexpr uint32_t kStreamFetchBase = 1912;
constexpr uint32_t kIndexBufferOffset = 12428;
constexpr uint32_t kDepthStencilOffset = 12448;
constexpr uint32_t kRenderTargetBase = 3108;
constexpr uint32_t kTextureHandleBase = 12536;
constexpr uint32_t kTextureFetchBase = 1152;
constexpr uint32_t kResourceFenceOffset = 8;
constexpr uint32_t kResourceSizeMask = 0x0FFFFFFF;
constexpr uint32_t kLiveResourceFenceOffset = 10908;
constexpr uint32_t kPendingResourceMaskOffset = 10912;
constexpr uint32_t kCommandScratchSize = 4800;
constexpr uint32_t kCommandScratchHeadOffset = 13504;
constexpr uint32_t kCommandScratchWriteOffset = 13508;
constexpr uint32_t kCommandScratchEndOffset = 13512;
constexpr uint32_t kVertexDeclarationOffset = 11812;
constexpr uint32_t kLegacyVertexDeclarationOffset = 10456;
constexpr uint32_t kVertexDeclarationElementCountOffset = 0x18;
constexpr uint32_t kVertexDeclarationMaximumStreamOffset = 0x1C;
constexpr uint32_t kVertexDeclarationElementsOffset = 0x34;
constexpr uint32_t kVertexDeclarationMagic = 0x00100005;
constexpr size_t kVertexDeclarationDiagnosticLimit = 256;
constexpr uint32_t kUpCommandWriteOffset = 13428;
constexpr uint32_t kUpVertexDataOffset = 13432;
constexpr uint32_t kUpVertexWordCountOffset = 13440;
constexpr uint32_t kSubmittedFrameOffset = 16544;
constexpr uint32_t kDrawParameterVertexRangeOffset = 36;
constexpr uint32_t kDrawParameterStreamFlagsOffset = 40;
constexpr uint32_t kDrawParameterTileFlagsOffset = 48;
constexpr uint64_t kNativeDirtyMask16 = 0x000013A0001809E8;
constexpr uint64_t kNativeDirtyMask24 = 0x0040000880000000;
constexpr uint64_t kNativeDirtyMask32 = 0x0000000800080000;
constexpr uint32_t kReplayFetchStateOffset = 0x480;
constexpr uint32_t kReplayFetchStateSize = 0x180;
constexpr uint32_t kReplayShaderConstantsOffset = 0x780;
constexpr uint32_t kReplayShaderConstantsSize = 0x1E00;
// c8-c11, derived from kReplayShaderConstantsOffset + register * 16.
constexpr uint32_t kReplayVertexTransformConstantsOffset = 0x800;
constexpr uint32_t kReplayVertexTransformConstantsSize = 0x40;
// Offsets within CapturedDrawSnapshot::shader_constants. Calculated by
// /tmp/calc_cached_snapshot_offsets.py rather than by hand.
constexpr uint32_t kReplayVertexTransformSnapshotOffset = 0x80;
constexpr uint32_t kReplayVertexBooleansOffset = 0x2780;
constexpr uint32_t kReplayPixelBooleansOffset = 0x2790;
constexpr uint32_t kReplayFixedStateOffset = 0x28A0;
constexpr uint32_t kReplayFixedStateSize = 0x8FC;
constexpr uint32_t kDeferredWidthGlobal = 0x83016B1C;
constexpr uint32_t kDeferredHeightGlobal = 0x83016B20;
constexpr uint32_t kDeferredOutputsGlobal = 0x83016B24;
constexpr uint32_t kDeferredAliasGlobal = 0x83016B44;
constexpr uint32_t kDeferredDeviceGlobal = 0x831C22A4;
// Created by sub_828BF280 and bound by sub_828BF8A8 for the forward path.
constexpr uint32_t kPrimaryDepthSurfaceGlobal = 0x831C23B0;
constexpr uint32_t kDeferredPhaseMarkerEndGlobal = 0x82AA858C;
constexpr uint32_t kDeferredPhaseMarkerBeginGlobal = 0x82AA8588;
constexpr uint32_t kDeferredPhaseTailGlobal = 0x82D475D0;
constexpr uint32_t kDeferredPhaseBeginReturnAddress = 0x824F51C0;
constexpr uint32_t kEaaParameterUploadReturnAddress = 0x822D07E0;
constexpr uint32_t kDeferredResolutionSetupReturnAddress = 0x824F8028;
constexpr uint32_t kVideoGlobalsReadyReturnAddress = 0x828C0C70;
constexpr uint32_t kPrimaryVideoWidthGlobal = 0x82B0B454;
constexpr uint32_t kPrimaryVideoHeightGlobal = 0x82B0B458;
constexpr uint32_t kSecondaryVideoWidthGlobal = 0x82B0B45C;
constexpr uint32_t kSecondaryVideoHeightGlobal = 0x82B0B460;
constexpr uint32_t kForceHighLodGlobal = 0x83016A20;
constexpr uint32_t kHighestLodDrawableOffset = 0x40;
constexpr uint32_t kDistanceScaleOutputGlobal = 0x82A931B4;
constexpr uint32_t kDistanceScaleInputGlobal = 0x82A931BC;
constexpr uint32_t kDisableTimecycleFarClipGlobal = 0x82BF3900;
constexpr uint32_t kGuestTimeStepGlobal = 0x82B06FF8;
constexpr uint32_t kCurrentViewportGlobal = 0x831C2200;
constexpr uint32_t kPostFxTimecycleIndexGlobal = 0x82B307A4;
constexpr uint32_t kPostFxTimecycleStride = 0xF0;
constexpr uint32_t kPostFxDirectionalMotionBlurLengthOffset = 0x168;
// Verified in the generated sub_8266F7D8 and sub_821B5B20 implementations.
// The worker flips the index after a submitted command batch, and the cloud
// callback reads the selected 528-byte slot at the offsets below.
constexpr uint32_t kCloudDoubleBufferIndexGlobal = 0x82B307A4;
constexpr uint32_t kCloudDoubleBufferProducerIndexGlobal = 0x82B307A0;
constexpr uint32_t kCloudDoubleBufferBase = 0x82D41C40;
constexpr uint32_t kCloudDoubleBufferStride = 528;
// sub_8266F7D8 loads the global sky pointer from 0x830BB03C. The embedded
// procedural-cloud object starts at sky+0x240 and holds its SkyhatPerlinNoise
// backing pointer at +0x44 (constructed by sub_827D4268).
constexpr uint32_t kCloudSkyObjectGlobal = 0x830BB03C;
constexpr uint32_t kCloudProceduralObjectOffset = 0x240;
constexpr uint32_t kCloudProceduralBackingPointerOffset = 0x44;
constexpr uint32_t kCloudProceduralBackingVtable = 0x8207B630;
constexpr std::array<uint32_t, 8> kCloudSkyProceduralFieldOffsets = {
    0x40, 0x50, 0x54, 0xB4, 0x194, 0x1E4, 0x268, 0x26C};
constexpr std::array<uint32_t, 2> kCloudProceduralFieldOffsets = {
    0x28, 0x2C};
constexpr std::array<uint32_t, 7> kCloudProceduralBackingFieldOffsets = {
    0x10, 0x14, 0x18, 0x1C, 0x20, 0x24, 0x28};
// Xbox generated code proves these float reads. FusionFix's named PC layout
// cross-reference identifies them as SkyLightMultiplier, CloudWarp,
// DetailNoiseOffset, unknown_200, and SkyBrightness respectively.
constexpr std::array<uint32_t, 5> kCloudConsumedFieldOffsets = {
    36, 356, 436, 512, 516};
constexpr std::array<uint32_t, 6> kCloudClockGlobals = {
    0x82DF3918, 0x82DF391C, 0x82DF3924,
    0x82DF3928, 0x82DF392C, 0x82DF3930};
constexpr uint32_t kMaximumEnvironmentalContextIndex = 8;
constexpr uint32_t kViewportCameraPositionOffset = 0x70;
constexpr uint32_t kViewportViewProjectionOffset = 0x100;
constexpr uint32_t kViewportViewInverseOffset = 0x140;
constexpr uint32_t kViewportViewOffset = 0x180;
constexpr uint32_t kViewportProjectionOffset = 0x1C0;
// sub_82670840 uploads the sky shader's SunDirection handle (constructed by
// sub_8266FD88 at shader-parameter object +0x14) from sky state +0x90, and its
// SunColor handle (+0x24) from sky state +0x1C0.
constexpr uint32_t kSkySunDirectionOffset = 0x90;
constexpr uint32_t kSkySunColorOffset = 0x1C0;
constexpr uint32_t kOriginalShadowMapBaseSize = 256;
constexpr uint32_t kOriginalDrawableReferenceLimit = 13000;
constexpr uint32_t kPointShadowCacheBaseMultiplier = 8;
constexpr uint32_t kShadowQualityTable = 0x82C595C0;
constexpr uint32_t kShadowQualityProfileCount = 8;
constexpr uint32_t kShadowQualityProfileStride = 256;
constexpr uint32_t kShadowQualityRangeOffset = 20;
constexpr uint32_t kReducePedModelBudgetGlobal = 0x82C6C1B1;
constexpr uint32_t kReduceVehicleModelBudgetGlobal = 0x82C6C1B2;
// Native rendering consumes the resource descriptors, not their Xbox GPU
// backing allocation. Keep the guest allocation at the API-valid minimum and
// patch only the descriptor extents after the trusted D3D constructor returns.
// This prevents host-resolution display resources from exhausting the guest
// physical heap without introducing an EDRAM buffer or tile emulation.
constexpr uint32_t kNativeBackingWidth = 1;
constexpr uint32_t kNativeBackingHeight = 1;
constexpr uint32_t kOriginalRenderTargetWidth = 1280;
constexpr uint32_t kOriginalRenderTargetHeight = 720;
constexpr uint32_t kResourcePackedDimensionsOffset = 0x24;
constexpr uint32_t kResourceNonDimensionMask = 0x00000007;
constexpr uint32_t kTextureDimensionFieldMask = 0x00001FFF;
constexpr uint32_t kTextureNonDimensionMask = 0xFC000000;
constexpr uint32_t kDeferredWrapperPhysicalWidthOffset = 44;
constexpr uint32_t kDeferredWrapperPhysicalHeightOffset = 46;
constexpr uint32_t kDeferredWrapperLogicalWidthOffset = 52;
constexpr uint32_t kDeferredWrapperLogicalHeightOffset = 54;
constexpr uint32_t kDeferredWrapperSurfaceOffset = 64;
constexpr uint32_t kDeferredWrapperTextureOffset = 72;
constexpr std::array<uint32_t, 9> kDeferredFullSizeWrapperGlobals = {
    0x83016B24, 0x83016B28, 0x83016B2C, 0x83016B30, 0x83016B34,
    0x83016B38, 0x83016B3C, 0x83016B40, 0x83016B48,
};
// The deferred phase renders depth into the title-owned multisampled
// gbuffer-z-aa wrapper. outputs[3] is its resolved sampled-depth wrapper, not
// the attachment that carries the current depth state.
constexpr uint32_t kDeferredDepthAaWrapperGlobal = 0x83016B40;
constexpr uint32_t kDeferredHizRestoreWrapperGlobal = 0x83016B4C;
constexpr uint32_t kDeferredOriginalRectangleCount = 3;
constexpr uint32_t kDeferredNativeRectangleCount = 1;
constexpr uint32_t kDeferredPhaseMarkerLimit = 128;
constexpr uint32_t kExteriorReflectionProjectionReturnAddress = 0x82522644;

struct PendingDrawPrimitiveUp {
  uint32_t device = 0;
  uint32_t primitive_type = 0;
  uint32_t vertex_count = 0;
  uint32_t stride = 0;
  uint32_t vertex_data = 0;
  uint32_t vertex_data_size = 0;
};

struct CapturedSunPayload {
  std::array<float, 4> direction{};
  std::array<float, 4> color{};
  bool direction_valid = false;
  bool color_valid = false;
};

thread_local CapturedSunPayload g_captured_sun_payload;

thread_local PendingDrawPrimitiveUp g_pending_draw_primitive_up;
thread_local uint32_t g_native_deferred_target_width = 0;
thread_local uint32_t g_native_deferred_target_height = 0;
thread_local uint32_t g_native_render_width = kOriginalRenderTargetWidth;
thread_local uint32_t g_native_render_height = kOriginalRenderTargetHeight;
thread_local uint32_t g_native_display_width = kOriginalRenderTargetWidth;
thread_local uint32_t g_native_display_height = kOriginalRenderTargetHeight;
thread_local uint32_t g_last_title_depth_wrapper = std::numeric_limits<uint32_t>::max();
thread_local uint32_t g_last_title_depth_surface = std::numeric_limits<uint32_t>::max();
thread_local uint32_t g_vector_font_id = 0;
thread_local uint32_t g_vector_font_owner = 0;
thread_local uint32_t g_vector_font_outer_stage = 0;
std::atomic<uint32_t> g_last_present_frontbuffer{0};
std::atomic<uint64_t> g_indexed_draw_invocation_id{0};
std::atomic<uint64_t> g_vector_font_owner_trace_count{0};
std::atomic<uint64_t> g_vector_font_texture_trace_count{0};
std::atomic<uint64_t> g_vector_font_rectangle_trace_count{0};

struct VectorFontOwnerBinding {
  uint32_t owner_slot;
  uint32_t logical_font_id;
};

// sub_821F6098 initializes these owners in the retail order font1, font3,
// then the conditionally loaded font2. The owner is not itself the D3D texture
// handle: sub_828E0048 obtains that handle through the owner's virtual method.
constexpr std::array<VectorFontOwnerBinding, 3> kVectorFontOwnerBindings = {{
    {0x82B99B9C, 1},
    {0x82B99D90, 3},
    {0x82B99F84, 2},
}};

uint32_t LoadU32(uint8_t* base, uint32_t address);

uint32_t IdentifyVectorFontOwner(uint8_t* base, uint32_t owner) {
  if (!owner) {
    return 0;
  }
  for (const auto& binding : kVectorFontOwnerBindings) {
    if (LoadU32(base, binding.owner_slot) == owner) {
      return binding.logical_font_id;
    }
  }
  return 0;
}

class ScopedVectorFontBinding final {
 public:
  ScopedVectorFontBinding(uint32_t logical_font_id, uint32_t owner, uint32_t outer_stage)
      : previous_id_(g_vector_font_id),
        previous_owner_(g_vector_font_owner),
        previous_outer_stage_(g_vector_font_outer_stage) {
    g_vector_font_id = logical_font_id;
    g_vector_font_owner = owner;
    g_vector_font_outer_stage = outer_stage;
  }

  ~ScopedVectorFontBinding() {
    g_vector_font_id = previous_id_;
    g_vector_font_owner = previous_owner_;
    g_vector_font_outer_stage = previous_outer_stage_;
  }

 private:
  uint32_t previous_id_;
  uint32_t previous_owner_;
  uint32_t previous_outer_stage_;
};

struct CapturedDrawSnapshot {
  std::array<uint8_t, kReplayFetchStateSize> fetch_state{};
  std::array<uint8_t, kReplayShaderConstantsSize> shader_constants{};
  std::array<uint8_t, sizeof(uint32_t)> vertex_booleans{};
  std::array<uint8_t, sizeof(uint32_t)> pixel_booleans{};
  std::array<uint8_t, kReplayFixedStateSize> fixed_state{};
  bool valid = false;
};

struct CapturedNativeCommand {
  CommandType type = CommandType::kPresent;
  uint32_t selector_mask = 0;
  std::vector<uint8_t> bytes;
  std::vector<uint8_t> payload;
  CapturedDrawSnapshot draw_snapshot;
  bool draw_diagnostic_valid = false;
  uint32_t capture_frame = 0;
  uint32_t capture_device = 0;
  uint32_t capture_build_object = 0;
  uint32_t capture_command_ordinal = 0;
  uint32_t capture_vertex_shader = 0;
  uint32_t capture_pixel_shader = 0;
  uint32_t capture_vertex_declaration = 0;
  uint32_t capture_index_buffer = 0;
  uint64_t capture_constants_hash = 0;
  uint64_t capture_transform_hash = 0;
  std::array<uint32_t, 16> capture_transform{};
};

struct CachedNativeCommandList {
  std::vector<CapturedNativeCommand> commands;
};

struct ActiveNativeCommandCapture {
  bool active = false;
  uint8_t* base = nullptr;
  uint32_t build_object = 0;
  uint32_t selector_mask = 0;
  std::vector<CapturedNativeCommand> commands;
};

thread_local ActiveNativeCommandCapture g_active_native_capture;
std::mutex g_cached_native_command_lists_mutex;
std::unordered_map<uint32_t, std::shared_ptr<const CachedNativeCommandList>>
    g_cached_native_command_lists;
std::mutex g_vertex_declaration_diagnostic_mutex;
std::unordered_map<uint32_t, bool> g_logged_vertex_declarations;

struct NativeShaderBindingDiagnosticState {
  uint32_t vertex_shader = 0;
  uint32_t pixel_shader = 0;
};

std::mutex g_shader_binding_diagnostic_mutex;
std::unordered_map<uint32_t, NativeShaderBindingDiagnosticState>
    g_shader_bindings_by_device;

uint8_t* GuestPointer(uint8_t* base, uint32_t address) {
  return base + address + REX_PHYS_HOST_OFFSET(address);
}

void TrackNativeShaderBinding(uint32_t device, ShaderStage stage,
                              uint32_t shader) {
  if (!rex::diagnostics::IsEnabled(
          rex::diagnostics::Category::kNativeTrace)) {
    return;
  }
  std::lock_guard lock(g_shader_binding_diagnostic_mutex);
  NativeShaderBindingDiagnosticState& state =
      g_shader_bindings_by_device[device];
  if (stage == ShaderStage::kVertex) {
    state.vertex_shader = shader;
  } else {
    state.pixel_shader = shader;
  }
}

NativeShaderBindingDiagnosticState CaptureNativeShaderBindings(
    uint32_t device) {
  std::lock_guard lock(g_shader_binding_diagnostic_mutex);
  const auto found = g_shader_bindings_by_device.find(device);
  return found != g_shader_bindings_by_device.end()
             ? found->second
             : NativeShaderBindingDiagnosticState{};
}

void CaptureDrawState(uint8_t* base, uint32_t device, CapturedDrawSnapshot& snapshot) {
  if (!base || !device) {
    return;
  }

  std::memcpy(snapshot.fetch_state.data(), GuestPointer(base, device + kReplayFetchStateOffset),
              snapshot.fetch_state.size());
  std::memcpy(snapshot.shader_constants.data(),
              GuestPointer(base, device + kReplayShaderConstantsOffset),
              snapshot.shader_constants.size());
  std::memcpy(snapshot.vertex_booleans.data(),
              GuestPointer(base, device + kReplayVertexBooleansOffset),
              snapshot.vertex_booleans.size());
  std::memcpy(snapshot.pixel_booleans.data(),
              GuestPointer(base, device + kReplayPixelBooleansOffset),
              snapshot.pixel_booleans.size());
  std::memcpy(snapshot.fixed_state.data(), GuestPointer(base, device + kReplayFixedStateOffset),
              snapshot.fixed_state.size());
  snapshot.valid = true;
}

class ScopedReplayDrawState {
 public:
  ScopedReplayDrawState(uint8_t* base, uint32_t device, const CapturedDrawSnapshot& snapshot)
      : base_(base), device_(device), active_(base && device && snapshot.valid) {
    if (!active_) {
      return;
    }

    std::memcpy(backup_.fetch_state.data(), GuestPointer(base_, device_ + kReplayFetchStateOffset),
                backup_.fetch_state.size());
    std::memcpy(backup_.shader_constants.data(),
                GuestPointer(base_, device_ + kReplayShaderConstantsOffset),
                backup_.shader_constants.size());
    std::memcpy(backup_.vertex_booleans.data(),
                GuestPointer(base_, device_ + kReplayVertexBooleansOffset),
                backup_.vertex_booleans.size());
    std::memcpy(backup_.pixel_booleans.data(),
                GuestPointer(base_, device_ + kReplayPixelBooleansOffset),
                backup_.pixel_booleans.size());
    std::memcpy(backup_.fixed_state.data(), GuestPointer(base_, device_ + kReplayFixedStateOffset),
                backup_.fixed_state.size());

    std::memcpy(GuestPointer(base_, device_ + kReplayFetchStateOffset), snapshot.fetch_state.data(),
                snapshot.fetch_state.size());
    std::memcpy(GuestPointer(base_, device_ + kReplayShaderConstantsOffset),
                snapshot.shader_constants.data(), snapshot.shader_constants.size());
    std::memcpy(GuestPointer(base_, device_ + kReplayVertexBooleansOffset),
                snapshot.vertex_booleans.data(), snapshot.vertex_booleans.size());
    std::memcpy(GuestPointer(base_, device_ + kReplayPixelBooleansOffset),
                snapshot.pixel_booleans.data(), snapshot.pixel_booleans.size());
    std::memcpy(GuestPointer(base_, device_ + kReplayFixedStateOffset), snapshot.fixed_state.data(),
                snapshot.fixed_state.size());
  }

  ~ScopedReplayDrawState() {
    if (!active_) {
      return;
    }

    std::memcpy(GuestPointer(base_, device_ + kReplayFetchStateOffset), backup_.fetch_state.data(),
                backup_.fetch_state.size());
    std::memcpy(GuestPointer(base_, device_ + kReplayShaderConstantsOffset),
                backup_.shader_constants.data(), backup_.shader_constants.size());
    std::memcpy(GuestPointer(base_, device_ + kReplayVertexBooleansOffset),
                backup_.vertex_booleans.data(), backup_.vertex_booleans.size());
    std::memcpy(GuestPointer(base_, device_ + kReplayPixelBooleansOffset),
                backup_.pixel_booleans.data(), backup_.pixel_booleans.size());
    std::memcpy(GuestPointer(base_, device_ + kReplayFixedStateOffset), backup_.fixed_state.data(),
                backup_.fixed_state.size());
  }

  ScopedReplayDrawState(const ScopedReplayDrawState&) = delete;
  ScopedReplayDrawState& operator=(const ScopedReplayDrawState&) = delete;

 private:
  uint8_t* base_ = nullptr;
  uint32_t device_ = 0;
  bool active_ = false;
  CapturedDrawSnapshot backup_;
};

uint8_t LoadU8(uint8_t* base, uint32_t address) {
  return *reinterpret_cast<volatile uint8_t*>(GuestPointer(base, address));
}

uint16_t LoadU16(uint8_t* base, uint32_t address) {
  return __builtin_bswap16(
      *reinterpret_cast<volatile uint16_t*>(GuestPointer(base, address)));
}

uint32_t LoadU32(uint8_t* base, uint32_t address) {
  return __builtin_bswap32(*reinterpret_cast<volatile uint32_t*>(GuestPointer(base, address)));
}

struct CloudGuestStateSnapshot {
  uint32_t selected_slot = 0;
  uint32_t producer_slot = 0;
  std::array<uint64_t, 2> slot_hashes{};
  std::array<std::array<uint32_t, kCloudConsumedFieldOffsets.size()>, 2>
      consumed_fields{};
};

struct CloudProceduralSnapshot {
  uint32_t sky = 0;
  uint32_t procedural = 0;
  uint32_t backing = 0;
  uint32_t backing_vtable = 0;
  bool sky_valid = false;
  bool backing_valid = false;
  std::array<uint32_t, kCloudSkyProceduralFieldOffsets.size()> sky_fields{};
  std::array<uint32_t, kCloudProceduralFieldOffsets.size()> procedural_fields{};
  std::array<uint32_t, kCloudProceduralBackingFieldOffsets.size()> backing_fields{};
};

CloudGuestStateSnapshot CaptureCloudGuestState(uint8_t* base) {
  CloudGuestStateSnapshot snapshot{};
  snapshot.selected_slot = LoadU32(base, kCloudDoubleBufferIndexGlobal);
  snapshot.producer_slot =
      LoadU32(base, kCloudDoubleBufferProducerIndexGlobal);
  for (uint32_t slot = 0; slot < snapshot.slot_hashes.size(); ++slot) {
    const uint32_t slot_address =
        kCloudDoubleBufferBase + slot * kCloudDoubleBufferStride;
    snapshot.slot_hashes[slot] =
        XXH3_64bits(GuestPointer(base, slot_address), kCloudDoubleBufferStride);
    for (uint32_t field = 0; field < kCloudConsumedFieldOffsets.size(); ++field) {
      snapshot.consumed_fields[slot][field] =
          LoadU32(base, slot_address + kCloudConsumedFieldOffsets[field]);
    }
  }
  return snapshot;
}

CloudProceduralSnapshot CaptureCloudProceduralState(uint8_t* base) {
  CloudProceduralSnapshot snapshot{};
  snapshot.sky = LoadU32(base, kCloudSkyObjectGlobal);
  if (!snapshot.sky) {
    return snapshot;
  }
  snapshot.sky_valid = true;
  snapshot.procedural = snapshot.sky + kCloudProceduralObjectOffset;
  for (uint32_t field = 0; field < snapshot.sky_fields.size(); ++field) {
    snapshot.sky_fields[field] =
        LoadU32(base, snapshot.sky + kCloudSkyProceduralFieldOffsets[field]);
  }
  for (uint32_t field = 0; field < snapshot.procedural_fields.size(); ++field) {
    snapshot.procedural_fields[field] =
        LoadU32(base, snapshot.procedural + kCloudProceduralFieldOffsets[field]);
  }
  snapshot.backing =
      LoadU32(base, snapshot.procedural + kCloudProceduralBackingPointerOffset);
  if (!snapshot.backing) {
    return snapshot;
  }
  snapshot.backing_vtable = LoadU32(base, snapshot.backing);
  if (snapshot.backing_vtable != kCloudProceduralBackingVtable) {
    return snapshot;
  }
  snapshot.backing_valid = true;
  for (uint32_t field = 0; field < snapshot.backing_fields.size(); ++field) {
    snapshot.backing_fields[field] =
        LoadU32(base, snapshot.backing + kCloudProceduralBackingFieldOffsets[field]);
  }
  return snapshot;
}

std::string FormatCloudGuestState(const CloudGuestStateSnapshot& snapshot) {
  std::string result = fmt::format(
      "producer={} consumer={} hashes={:016X}/{:016X}",
      snapshot.producer_slot, snapshot.selected_slot, snapshot.slot_hashes[0],
      snapshot.slot_hashes[1]);
  for (uint32_t slot = 0; slot < snapshot.consumed_fields.size(); ++slot) {
    result += fmt::format(" slot{}=[", slot);
    for (uint32_t field = 0; field < kCloudConsumedFieldOffsets.size(); ++field) {
      const uint32_t word = snapshot.consumed_fields[slot][field];
      result += fmt::format(
          "{}+{}:{:08X}:{:.9g}", field ? "," : "",
          kCloudConsumedFieldOffsets[field], word, std::bit_cast<float>(word));
    }
    result += "]";
  }
  return result;
}

std::string FormatCloudProceduralState(const CloudProceduralSnapshot& snapshot) {
  std::string result = fmt::format(
      "sky={:08X}:{} proc={:08X} backing={:08X} vtable={:08X}:{}",
      snapshot.sky, snapshot.sky_valid, snapshot.procedural, snapshot.backing,
      snapshot.backing_vtable, snapshot.backing_valid);
  if (!snapshot.sky_valid) {
    return result;
  }
  result += " sky-fields=[";
  for (uint32_t field = 0; field < snapshot.sky_fields.size(); ++field) {
    const uint32_t word = snapshot.sky_fields[field];
    result += fmt::format(
        "{}+{:X}:{:08X}:{:.9g}", field ? "," : "",
        kCloudSkyProceduralFieldOffsets[field], word, std::bit_cast<float>(word));
  }
  result += "] proc-fields=[";
  for (uint32_t field = 0; field < snapshot.procedural_fields.size(); ++field) {
    const uint32_t word = snapshot.procedural_fields[field];
    result += fmt::format(
        "{}+{:X}:{:08X}:{:.9g}", field ? "," : "",
        kCloudProceduralFieldOffsets[field], word, std::bit_cast<float>(word));
  }
  result += "] backing-fields=[";
  if (snapshot.backing_valid) {
    for (uint32_t field = 0; field < snapshot.backing_fields.size(); ++field) {
      const uint32_t word = snapshot.backing_fields[field];
      result += fmt::format(
          "{}+{:X}:{:08X}:{:.9g}", field ? "," : "",
          kCloudProceduralBackingFieldOffsets[field], word,
          std::bit_cast<float>(word));
    }
  }
  result += "]";
  return result;
}

uint32_t BeginCloudGuestTrace(std::atomic<uint32_t>& counter) {
  const uint32_t limit =
      REXCVAR_QUERY(uint32_t, gta4_trace_cloud_frames);
  if (!limit) {
    return 0;
  }
  const uint32_t ordinal = counter.fetch_add(1, std::memory_order_relaxed) + 1;
  return ordinal <= limit ? ordinal : 0;
}

const char* IdentifyDeferredWrapperRole(uint8_t* base, uint32_t wrapper) {
  if (!wrapper) {
    return "none";
  }
  constexpr std::array<const char*, 9> roles = {
      "gbuffer-0", "gbuffer-1", "gbuffer-2", "gbuffer-z", "gbuffer-0-aa",
      "gbuffer-1-aa", "gbuffer-2-aa", "gbuffer-z-aa", "depth-alias"};
  for (size_t index = 0; index < kDeferredFullSizeWrapperGlobals.size(); ++index) {
    if (LoadU32(base, kDeferredFullSizeWrapperGlobals[index]) == wrapper) {
      return roles[index];
    }
  }
  if (LoadU32(base, kDeferredAliasGlobal) == wrapper) {
    return "gbuffer-2-alias";
  }
  return "other";
}

uint64_t LoadU64(uint8_t* base, uint32_t address) {
  return __builtin_bswap64(*reinterpret_cast<volatile uint64_t*>(GuestPointer(base, address)));
}

void StoreU8(uint8_t* base, uint32_t address, uint8_t value) {
  *reinterpret_cast<volatile uint8_t*>(GuestPointer(base, address)) = value;
}

void StoreU16(uint8_t* base, uint32_t address, uint16_t value) {
  *reinterpret_cast<volatile uint16_t*>(GuestPointer(base, address)) = __builtin_bswap16(value);
}

void StoreU32(uint8_t* base, uint32_t address, uint32_t value) {
  *reinterpret_cast<volatile uint32_t*>(GuestPointer(base, address)) = __builtin_bswap32(value);
}

void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  *reinterpret_cast<volatile uint64_t*>(GuestPointer(base, address)) = __builtin_bswap64(value);
}

void StoreF32(uint8_t* base, uint32_t address, float value) {
  StoreU32(base, address, std::bit_cast<uint32_t>(value));
}

std::array<uint32_t, 16> CaptureLiveVertexTransform(uint8_t* base,
                                                   uint32_t device) {
  std::array<uint32_t, 16> transform{};
  if (!base || !device) {
    return transform;
  }
  for (uint32_t index = 0; index < transform.size(); ++index) {
    transform[index] = LoadU32(
        base, device + kReplayVertexTransformConstantsOffset +
                  index * sizeof(uint32_t));
  }
  return transform;
}

std::array<uint32_t, 16> CaptureSnapshotVertexTransform(
    const CapturedDrawSnapshot& snapshot) {
  std::array<uint32_t, 16> transform{};
  if (!snapshot.valid) {
    return transform;
  }
  for (uint32_t index = 0; index < transform.size(); ++index) {
    uint32_t raw = 0;
    std::memcpy(&raw,
                snapshot.shader_constants.data() +
                    kReplayVertexTransformSnapshotOffset +
                    index * sizeof(uint32_t),
                sizeof(raw));
    transform[index] = __builtin_bswap32(raw);
  }
  return transform;
}

bool IsKnownOffscreenVertexTransform(
    const std::array<uint32_t, 16>& transform) {
  return transform[12] == 0x431B959E && transform[13] == 0x4311C5BE &&
         transform[14] == 0x3C7DD5B0 && transform[15] == 0x415D1833;
}

float SnapFontCoordinate(float coordinate, uint32_t extent) {
  if (!extent || !std::isfinite(coordinate)) {
    return coordinate;
  }
  const float extent_float = float(extent);
  return std::round(coordinate * extent_float) / extent_float;
}

template <size_t Size>
bool GuestStringEquals(uint8_t* base, uint32_t address, const char (&expected)[Size]) {
  if (!address) {
    return false;
  }
  for (size_t index = 0; index < Size; ++index) {
    if (LoadU8(base, address + uint32_t(index)) != uint8_t(expected[index])) {
      return false;
    }
  }
  return true;
}

bool IsDeferredAaResourceName(uint8_t* base, uint32_t address) {
  return GuestStringEquals(base, address, "_DEFERRED_GBUFFER_Z_AA_") ||
         GuestStringEquals(base, address, "_DEFERRED_GBUFFER_0_AA_") ||
         GuestStringEquals(base, address, "_DEFERRED_GBUFFER_1_AA_") ||
         GuestStringEquals(base, address, "_DEFERRED_GBUFFER_2_AA_");
}

struct ReflectionResourceName {
  ReflectionFamily family;
  ReflectionRole role;
  uint32_t logical_width;
  uint32_t logical_height;
  const char* name;
};

bool GetReflectionResourceName(uint8_t* base, uint32_t address,
                               ReflectionResourceName& result) {
  if (GuestStringEquals(base, address, "MIRROR_RT")) {
    result = {ReflectionFamily::kMirror, ReflectionRole::kColor, 320, 180, "MIRROR_RT"};
  } else if (GuestStringEquals(base, address, "MIRROR_DT")) {
    result = {ReflectionFamily::kMirror, ReflectionRole::kDepth, 320, 180, "MIRROR_DT"};
  } else if (GuestStringEquals(base, address, "WATER_REFLECTION_COLOUR")) {
    result = {ReflectionFamily::kWater, ReflectionRole::kColor, 320, 180,
              "WATER_REFLECTION_COLOUR"};
  } else if (GuestStringEquals(base, address, "WATER_REFLECTION_DEPTH")) {
    result = {ReflectionFamily::kWater, ReflectionRole::kDepth, 320, 180,
              "WATER_REFLECTION_DEPTH"};
  } else if (GuestStringEquals(base, address, "REFLECTION_MAP_COLOUR")) {
    result = {ReflectionFamily::kEnvironment, ReflectionRole::kColor, 256, 256,
              "REFLECTION_MAP_COLOUR"};
  } else if (GuestStringEquals(base, address, "REFLECTION_MAP_DEPTH")) {
    result = {ReflectionFamily::kEnvironment, ReflectionRole::kDepth, 256, 256,
              "REFLECTION_MAP_DEPTH"};
  } else {
    return false;
  }
  return true;
}

std::string_view GetReflectionResolutionSelection(ReflectionFamily family) {
  const std::string* override_value = nullptr;
  switch (family) {
    case ReflectionFamily::kMirror:
      override_value = &REXCVAR_GET(gta4_mirror_reflection_resolution);
      break;
    case ReflectionFamily::kWater:
      override_value = &REXCVAR_GET(gta4_water_reflection_resolution);
      break;
    case ReflectionFamily::kEnvironment:
      override_value = &REXCVAR_GET(gta4_environment_reflection_resolution);
      break;
  }
  if (override_value && *override_value != "inherit") {
    return *override_value;
  }
  return REXCVAR_GET(gta4_reflection_resolution);
}

std::pair<uint32_t, uint32_t> GetReflectionPhysicalExtent(
    const ReflectionResourceName& resource) {
  const std::string_view selection = GetReflectionResolutionSelection(resource.family);
  if (selection == "original") {
    return {resource.logical_width, resource.logical_height};
  }
  if (selection == "1080p") {
    return resource.family == ReflectionFamily::kEnvironment
               ? std::pair<uint32_t, uint32_t>{1024, 1024}
               : std::pair<uint32_t, uint32_t>{1920, 1080};
  }

  uint32_t target_height = 1440;
  const std::string_view cap = REXCVAR_GET(gta4_reflection_resolution_cap);
  if (cap == "1080p") {
    target_height = 1080;
  } else if (cap == "display") {
    auto* runtime = rex::Runtime::instance();
    auto* window = runtime ? runtime->display_window() : nullptr;
    if (window && window->GetActualPhysicalHeight()) {
      target_height = window->GetActualPhysicalHeight();
    }
  }

  if (resource.family == ReflectionFamily::kEnvironment) {
    const uint32_t square_extent = target_height <= 1080 ? 1024 : target_height <= 2160 ? 2048 : 4096;
    return {square_extent, square_extent};
  }
  const uint64_t scaled_width = uint64_t(target_height) * 16 + 4;
  return {uint32_t(scaled_width / 9), target_height};
}

uint32_t GetReflectionSampleCountOverride() {
  const std::string_view selection = REXCVAR_GET(gta4_reflection_aa);
  if (selection == "off") {
    return 1;
  }
  if (selection == "2x") {
    return 2;
  }
  if (selection == "4x") {
    return 4;
  }
  return 0;
}

double GetExteriorReflectionCaptureDistance() {
  const std::string_view selection = REXCVAR_GET(gta4_reflection_capture_distance);
  if (selection == "extended") {
    return 60.0;
  }
  if (selection == "far") {
    return 80.0;
  }
  return 40.0;
}

rex::system::IGraphicsSystem* GetNativeGraphicsSystem() {
  auto* runtime = rex::Runtime::instance();
  if (!runtime) {
    return nullptr;
  }
  auto* graphics = runtime->graphics_system();
  if (!graphics || graphics->GetTitleCommandAbi(kTitleId) != kTitleCommandAbi) {
    return nullptr;
  }
  return graphics;
}

bool IsNativeMode() {
  return GetNativeGraphicsSystem() != nullptr;
}

bool ShouldLogNativeHookCall(uint64_t call_count);
uint64_t NextNativeHookDiagnosticCall(std::atomic<uint64_t>& counter);

bool QueryNativeDeviceCapabilities(DeviceCapabilitiesResult& result) {
  QueryDeviceCapabilitiesCommand command{};
  if (auto* graphics = GetNativeGraphicsSystem()) {
    return graphics->ExecuteTitleCommand(kTitleId, kTitleCommandAbi, &command, sizeof(command),
                                         &result, sizeof(result));
  }
  return false;
}

void ApplyModelBudgetOverrides(uint8_t* base) {
  if (!REXCVAR_GET(gta4_disable_model_budget_reduction)) {
    return;
  }
  StoreU8(base, kReducePedModelBudgetGlobal, 0);
  StoreU8(base, kReduceVehicleModelBudgetGlobal, 0);
}

void ApplyShadowDistanceScale(uint8_t* base) {
  const double configured_scale = REXCVAR_GET(gta4_shadow_distance_scale);
  static std::mutex shadow_range_mutex;
  static bool originals_captured = false;
  static std::array<float, kShadowQualityProfileCount> original_ranges{};

  std::lock_guard lock(shadow_range_mutex);
  if (!originals_captured) {
    for (uint32_t profile = 0; profile < kShadowQualityProfileCount; ++profile) {
      const uint32_t address =
          kShadowQualityTable + profile * kShadowQualityProfileStride + kShadowQualityRangeOffset;
      const float range = std::bit_cast<float>(LoadU32(base, address));
      if (!std::isfinite(range) || range <= 0.0f) {
        REXLOG_WARN(
            "gta4-native-quality: shadow range table is not initialized profile={} "
            "address={:08X} value={}; leaving ranges unchanged",
            profile, address, range);
        return;
      }
      original_ranges[profile] = range;
    }
    originals_captured = true;
  }

  for (uint32_t profile = 0; profile < kShadowQualityProfileCount; ++profile) {
    const uint32_t address =
        kShadowQualityTable + profile * kShadowQualityProfileStride + kShadowQualityRangeOffset;
    StoreF32(base, address, original_ranges[profile] * float(configured_scale));
  }

  static std::atomic<uint64_t> override_count{0};
  const uint64_t override = NextNativeHookDiagnosticCall(override_count);
  if (ShouldLogNativeHookCall(override)) {
    REXLOG_INFO(
        "gta4-native-quality: shadow distance #{} multiplier={} profile0={} -> {} "
        "profile7={} -> {}",
        override, configured_scale, original_ranges.front(),
        original_ranges.front() * float(configured_scale), original_ranges.back(),
        original_ranges.back() * float(configured_scale));
  }
}

struct NativeResolutionOverride {
  uint32_t width;
  uint32_t height;
  uint32_t display_width;
  uint32_t display_height;
  bool override_width;
  bool override_height;
  bool automatic_display;
  bool fsr1_active;

  bool active() const { return override_width || override_height; }
};

std::pair<uint32_t, uint32_t> FitOriginalAspectRatio(uint32_t width, uint32_t height) {
  // Fit the Xbox 360 title's 16:9 image inside the selected output bounds.
  // Cross-products avoid floating-point rounding when deciding which edge limits the fit.
  if (uint64_t(width) * 9 > uint64_t(height) * 16) {
    return {uint32_t(uint64_t(height) * 16 / 9), height};
  }
  return {width, uint32_t(uint64_t(width) * 9 / 16)};
}

NativeResolutionOverride GetNativeResolutionOverride(uint32_t requested_width,
                                                     uint32_t requested_height) {
  int32_t configured_width = REXCVAR_GET(video_mode_width);
  int32_t configured_height = REXCVAR_GET(video_mode_height);
  bool override_width = rex::cvar::HasNonDefaultValue("video_mode_width");
  bool override_height = rex::cvar::HasNonDefaultValue("video_mode_height");
  bool automatic_display = false;

  if (!override_width && !override_height) {
    int32_t preset_width = 0;
    int32_t preset_height = 0;
    if (rex::graphics::video_mode_util::TryGetResolutionPresetFromCVar(preset_width,
                                                                       preset_height)) {
      configured_width = preset_width;
      configured_height = preset_height;
      override_width = true;
      override_height = true;
    }
  }

  // Presentation is initialized before the runtime and the guest thread. If
  // no video mode was explicitly selected, use the finalized drawable size
  // so GTA IV constructs its render graph and projection for the display's
  // physical pixel resolution and aspect ratio from the first frame.
  if (!override_width && !override_height) {
    auto* runtime = rex::Runtime::instance();
    auto* window = runtime ? runtime->display_window() : nullptr;
    const uint32_t display_width = window ? window->GetActualPhysicalWidth() : 0;
    const uint32_t display_height = window ? window->GetActualPhysicalHeight() : 0;
    if (display_width && display_height) {
      configured_width = int32_t(display_width);
      configured_height = int32_t(display_height);
      override_width = true;
      override_height = true;
      automatic_display = true;
    }
  }

  NativeResolutionOverride result{requested_width, requested_height, requested_width,
                                  requested_height, override_width, override_height,
                                  automatic_display, false};
  if (override_width) {
    result.display_width = uint32_t(std::clamp(configured_width, 640, 0x0FFF));
  }
  if (override_height) {
    result.display_height = uint32_t(std::clamp(configured_height, 480, 0x0FFF));
  }
  result.width = result.display_width;
  result.height = result.display_height;

  if (REXCVAR_GET(gta4_aspect_ratio) == "original") {
    const auto [aspect_width, aspect_height] =
        FitOriginalAspectRatio(result.display_width, result.display_height);
    result.display_width = aspect_width;
    result.display_height = aspect_height;
    result.width = aspect_width;
    result.height = aspect_height;
  }

  const bool fsr1_requested = REXCVAR_GET(gta4_native_upscaler) == "fsr1";
  const bool hdr_requested = rex::cvar::Query<bool>("vulkan_hdr");
  if (fsr1_requested && !hdr_requested) {
    const std::string& quality = REXCVAR_GET(gta4_fsr1_quality);
    const double scale = quality == "ultra_quality" ? 1.3
                         : quality == "balanced"    ? 1.7
                         : quality == "performance" ? 2.0
                                                     : 1.5;
    const uint32_t candidate_width =
        uint32_t(std::max(1.0, std::round(double(result.display_width) / scale)));
    const uint32_t candidate_height =
        uint32_t(std::max(1.0, std::round(double(result.display_height) / scale)));
    if (candidate_width >= 640 && candidate_height >= 360) {
      result.width = candidate_width;
      result.height = candidate_height;
      result.fsr1_active = result.width < result.display_width ||
                           result.height < result.display_height;
    } else {
      static std::atomic<bool> logged_small_fsr_input{false};
      if (!logged_small_fsr_input.exchange(true)) {
        REXLOG_WARN(
            "gta4-native-upscaler: FSR 1 {} input {}x{} for display {}x{} is below "
            "the validated 640x360 render floor; using native resolution",
            quality, candidate_width, candidate_height, result.display_width,
            result.display_height);
      }
    }
  } else if (fsr1_requested && hdr_requested) {
    static std::atomic<bool> logged_hdr_fsr_fallback{false};
    if (!logged_hdr_fsr_fallback.exchange(true)) {
      REXLOG_WARN(
          "gta4-native-upscaler: FSR 1 requires normalized perceptual input; "
          "HDR requested, so internal rendering remains native resolution");
    }
  }
  g_native_render_width = result.width;
  g_native_render_height = result.height;
  g_native_display_width = result.display_width;
  g_native_display_height = result.display_height;
  return result;
}

bool ShouldLogNativeHookCall(uint64_t call_count);

bool IsNativeDisplayResourceConstructorCaller(uint32_t return_address) {
  switch (return_address) {
    case 0x828BF2C0:
    case 0x828BF2F0:
    case 0x828BF318:
    case 0x828BF334:
    case 0x828BF354:
    case 0x828DBFE0:
    case 0x828DC2A8:
    case 0x82A504F4:
    case 0x82A5052C:
    case 0x82A50568:
      return true;
    default:
      return false;
  }
}

bool IsRageRenderTargetTextureConstructorCaller(uint32_t return_address) {
  switch (return_address) {
    // sub_828BEC78 creates the texture half of a RAGE render target through
    // one of these three D3D texture paths (2D, cube, or the default type).
    case 0x828BEFA8:
    case 0x828BEFEC:
    case 0x828BF014:
      return true;
    default:
      return false;
  }
}

bool ShouldUseNativeDisplayBacking(const PPCContext& ctx, uint32_t width, uint32_t height) {
  if (!IsNativeMode()) {
    return false;
  }
  if (IsNativeDisplayResourceConstructorCaller(ctx.lr)) {
    return width > kNativeBackingWidth || height > kNativeBackingHeight;
  }

  // The generic RAGE render-target factory also creates CPU-lockable textures
  // such as PHONE_SCREEN and PHOTO. Keep those smaller resources concretely
  // backed, and virtualize only targets exceeding the Xbox 360 render envelope.
  return IsRageRenderTargetTextureConstructorCaller(ctx.lr) &&
         (width > kOriginalRenderTargetWidth || height > kOriginalRenderTargetHeight);
}

uint32_t EncodeSurfaceResourceDimensions(uint32_t original, uint32_t width, uint32_t height) {
  return (original & kResourceNonDimensionMask) | ((width - 1) << 18) | ((height - 1) << 3);
}

uint32_t EncodeTextureResourceDimensions(uint32_t original, uint32_t width, uint32_t height) {
  return (original & kTextureNonDimensionMask) |
         ((width - 1) & kTextureDimensionFieldMask) |
         (((height - 1) & kTextureDimensionFieldMask) << 13);
}

void PatchNativeDisplayResourceDimensions(uint8_t* base, uint32_t resource, uint32_t width,
                                          uint32_t height, uint32_t return_address,
                                          const char* resource_kind, bool texture_layout) {
  if (!resource) {
    REXLOG_ERROR(
        "gta4-native-resolution: {} construction failed caller={:08X} requested={}x{}",
        resource_kind, return_address, width, height);
    return;
  }

  const uint32_t dimensions_address = resource + kResourcePackedDimensionsOffset;
  const uint32_t original_dimensions = LoadU32(base, dimensions_address);
  const uint32_t native_dimensions =
      texture_layout ? EncodeTextureResourceDimensions(original_dimensions, width, height)
                     : EncodeSurfaceResourceDimensions(original_dimensions, width, height);
  StoreU32(base, dimensions_address, native_dimensions);

  static std::atomic<uint64_t> patch_count{0};
  const uint64_t patch = NextNativeHookDiagnosticCall(patch_count);
  if (ShouldLogNativeHookCall(patch)) {
    REXLOG_INFO(
        "gta4-native-resolution: {} descriptor #{} caller={:08X} resource={:08X} "
        "requested={}x{} backing={}x{} packed={:08X}->{:08X}",
        resource_kind, patch, return_address, resource, width, height,
        std::min(width, kNativeBackingWidth), std::min(height, kNativeBackingHeight),
        original_dimensions, native_dimensions);
  }
}

bool PatchDeferredWrapperDimensions(uint8_t* base, uint32_t wrapper, uint32_t width,
                                    uint32_t height) {
  if (!wrapper || !width || !height || width > UINT16_MAX || height > UINT16_MAX) {
    return false;
  }

  StoreU16(base, wrapper + kDeferredWrapperPhysicalWidthOffset, uint16_t(width));
  StoreU16(base, wrapper + kDeferredWrapperPhysicalHeightOffset, uint16_t(height));
  StoreU16(base, wrapper + kDeferredWrapperLogicalWidthOffset, uint16_t(width));
  StoreU16(base, wrapper + kDeferredWrapperLogicalHeightOffset, uint16_t(height));

  const uint32_t surface = LoadU32(base, wrapper + kDeferredWrapperSurfaceOffset);
  if (surface) {
    const uint32_t dimensions_address = surface + kResourcePackedDimensionsOffset;
    StoreU32(base, dimensions_address,
             EncodeSurfaceResourceDimensions(LoadU32(base, dimensions_address), width, height));
  }

  const uint32_t texture = LoadU32(base, wrapper + kDeferredWrapperTextureOffset);
  if (texture) {
    const uint32_t dimensions_address = texture + kResourcePackedDimensionsOffset;
    StoreU32(base, dimensions_address,
             EncodeTextureResourceDimensions(LoadU32(base, dimensions_address), width, height));
  }

  return surface || texture;
}

bool IsCachedListCommandType(CommandType type) {
  switch (type) {
    case CommandType::kSetRenderState:
    case CommandType::kSetPixelShader:
    case CommandType::kSetVertexShader:
    case CommandType::kSetVertexDeclaration:
    case CommandType::kSetTexture:
    case CommandType::kSetDepthStencil:
    case CommandType::kSetRenderTarget:
    case CommandType::kSetVertexStream:
    case CommandType::kSetIndexBuffer:
    case CommandType::kDrawPrimitive:
    case CommandType::kDrawPrimitiveUp:
    case CommandType::kDrawIndexedPrimitive:
    case CommandType::kResolve:
    case CommandType::kClear:
      return true;
    case CommandType::kDeviceCreated:
    case CommandType::kDeviceDestroyed:
    case CommandType::kRegisterShader:
    case CommandType::kRegisterVertexDeclaration:
    case CommandType::kInvalidateTexture:
    case CommandType::kTextureLock:
    case CommandType::kRenderPhaseMarker:
    case CommandType::kPresent:
    case CommandType::kQueryDeviceCapabilities:
    case CommandType::kRegisterReflectionTarget:
    case CommandType::kReleaseResource:
    case CommandType::kUpdateEnvironmentalData:
    case CommandType::kDepthSurfaceHandoff:
      return false;
  }
  return false;
}

template <typename Command>
bool CaptureNativeCommand(const Command& command) {
  if (!g_active_native_capture.active || !IsCachedListCommandType(command.header.type)) {
    return false;
  }

  CapturedNativeCommand captured;
  captured.type = command.header.type;
  captured.selector_mask = g_active_native_capture.selector_mask;
  captured.bytes.resize(sizeof(command));
  std::memcpy(captured.bytes.data(), &command, sizeof(command));

  if constexpr (std::is_same_v<Command, DrawPrimitiveCommand> ||
                std::is_same_v<Command, DrawPrimitiveUpCommand> ||
                std::is_same_v<Command, DrawIndexedPrimitiveCommand>) {
    CaptureDrawState(g_active_native_capture.base, command.device, captured.draw_snapshot);
  }

  if constexpr (std::is_same_v<Command, DrawIndexedPrimitiveCommand>) {
    const bool trace_gbuffer_snapshot =
        rex::diagnostics::IsEnabled(
            rex::diagnostics::Category::kNativeTrace) &&
        (command.index_count == 5961 || command.index_count == 1056);
    if (trace_gbuffer_snapshot) {
      captured.draw_diagnostic_valid = true;
      captured.capture_frame =
          LoadU32(g_active_native_capture.base,
                  command.device + kSubmittedFrameOffset);
      captured.capture_device = command.device;
      captured.capture_build_object = g_active_native_capture.build_object;
      captured.capture_command_ordinal =
          uint32_t(g_active_native_capture.commands.size() + 1);
      const NativeShaderBindingDiagnosticState bindings =
          CaptureNativeShaderBindings(command.device);
      captured.capture_vertex_shader = bindings.vertex_shader;
      captured.capture_pixel_shader = bindings.pixel_shader;
      captured.capture_vertex_declaration =
          LoadU32(g_active_native_capture.base,
                  command.device + kVertexDeclarationOffset);
      captured.capture_index_buffer =
          LoadU32(g_active_native_capture.base,
                  command.device + kIndexBufferOffset);
      captured.capture_constants_hash = XXH3_64bits(
          captured.draw_snapshot.shader_constants.data(),
          captured.draw_snapshot.shader_constants.size());
      captured.capture_transform_hash = XXH3_64bits(
          captured.draw_snapshot.shader_constants.data() +
              kReplayVertexTransformSnapshotOffset,
          kReplayVertexTransformConstantsSize);
      captured.capture_transform =
          CaptureSnapshotVertexTransform(captured.draw_snapshot);
      REXLOG_INFO(
          "gta4-native-cause: point=cached-draw-capture frame={} build={:08X} "
          "ordinal={} selector={:08X} device={:08X} caller={:08X} draw={} "
          "primitive={} base={} start={} indices={} vs-handle={:08X} "
          "ps-handle={:08X} vdecl={:08X} ib={:08X} constants={:016X} "
          "transform={:016X} known-offscreen={} "
          "c8={:08X},{:08X},{:08X},{:08X} "
          "c9={:08X},{:08X},{:08X},{:08X} "
          "c10={:08X},{:08X},{:08X},{:08X} "
          "c11={:08X},{:08X},{:08X},{:08X}",
          captured.capture_frame, captured.capture_build_object,
          captured.capture_command_ordinal, captured.selector_mask,
          captured.capture_device, command.caller, command.draw_id,
          command.primitive_type, command.base_vertex, command.start_index,
          command.index_count, captured.capture_vertex_shader,
          captured.capture_pixel_shader,
          captured.capture_vertex_declaration, captured.capture_index_buffer,
          captured.capture_constants_hash, captured.capture_transform_hash,
          IsKnownOffscreenVertexTransform(captured.capture_transform),
          captured.capture_transform[0], captured.capture_transform[1],
          captured.capture_transform[2], captured.capture_transform[3],
          captured.capture_transform[4], captured.capture_transform[5],
          captured.capture_transform[6], captured.capture_transform[7],
          captured.capture_transform[8], captured.capture_transform[9],
          captured.capture_transform[10], captured.capture_transform[11],
          captured.capture_transform[12], captured.capture_transform[13],
          captured.capture_transform[14], captured.capture_transform[15]);
    }
  }

  if constexpr (std::is_same_v<Command, DrawPrimitiveUpCommand>) {
    if (command.vertex_data && command.vertex_data_size) {
      captured.payload.resize(command.vertex_data_size);
      std::memcpy(captured.payload.data(),
                  GuestPointer(g_active_native_capture.base, command.vertex_data),
                  captured.payload.size());
    }
  }

  g_active_native_capture.commands.push_back(std::move(captured));
  return true;
}

template <typename Command>
bool SubmitNativeCommand(const Command& command) {
  if (CaptureNativeCommand(command)) {
    return true;
  }
  if (auto* graphics = GetNativeGraphicsSystem()) {
    const bool accepted =
        graphics->SubmitTitleCommand(kTitleId, kTitleCommandAbi, &command, sizeof(command));
    if (!accepted &&
        rex::diagnostics::IsEnabled(rex::diagnostics::Category::kGuestHooks)) {
      static std::atomic<uint64_t> rejection_count{0};
      const uint64_t count = NextNativeHookDiagnosticCall(rejection_count);
      if (ShouldLogNativeHookCall(count)) {
        REXLOG_WARN("gta4-native-hook: rejected command #{} type={} size={}", count,
                    uint32_t(command.header.type), sizeof(command));
      }
    }
    return accepted;
  }
  return false;
}

bool ShouldLogNativeHookCall(uint64_t call_count);

template <size_t Count>
bool LoadFiniteGuestFloats(uint8_t* base, uint32_t address,
                           std::array<float, Count>& values) {
  uint32_t cursor = address;
  for (float& value : values) {
    value = std::bit_cast<float>(LoadU32(base, cursor));
    if (!std::isfinite(value)) {
      values = {};
      return false;
    }
    cursor += sizeof(uint32_t);
  }
  return true;
}

EnvironmentalDataV1 CaptureEnvironmentalData(uint8_t* base, uint32_t postfx) {
  static std::atomic<uint64_t> source_sequence{0};
  EnvironmentalDataV1 data{};
  data.byte_size = sizeof(data);
  data.source_sequence = source_sequence.fetch_add(1, std::memory_order_relaxed) + 1;

  // FusionShaders' timecycext contract defaults. The Xbox executable has no
  // timecycext table or proven clock/weather ABI to index one, so expose the
  // documented neutral row through the typed bridge instead of guessing guest
  // globals. A future table parser may replace these values without changing
  // the EnvironmentalDataV1 contract.
  data.fog_density = 0.004f;
  data.fog_height_falloff = 0.015f;
  data.fog_altitude_tweak = 0.7f;
  data.fog_power = 1.0f;
  data.valid_fields |= EnvironmentalFieldBit(EnvironmentalField::kFogDensity) |
                       EnvironmentalFieldBit(EnvironmentalField::kFogHeightFalloff) |
                       EnvironmentalFieldBit(EnvironmentalField::kFogAltitudeTweak) |
                       EnvironmentalFieldBit(EnvironmentalField::kFogPower);

  if (g_captured_sun_payload.direction_valid) {
    data.sun_direction = g_captured_sun_payload.direction;
    data.valid_fields |= EnvironmentalFieldBit(EnvironmentalField::kSunDirection);
  }
  if (g_captured_sun_payload.color_valid) {
    data.sun_color = g_captured_sun_payload.color;
    data.valid_fields |= EnvironmentalFieldBit(EnvironmentalField::kSunColor);
  }

  data.time_step_seconds = std::bit_cast<float>(LoadU32(base, kGuestTimeStepGlobal));
  if (std::isfinite(data.time_step_seconds) && data.time_step_seconds >= 0.0f) {
    data.valid_fields |= EnvironmentalFieldBit(EnvironmentalField::kTimeStepSeconds);
  } else {
    data.time_step_seconds = 0.0f;
  }

  // sub_822CFC00 uploads postfx[timecycle_index].field_168 through the
  // PPPDirectionalMotionBlurLength handle stored at postfx+0x2D8. Preserve the
  // uploaded scalar independently from the currently unavailable user scale.
  const uint32_t timecycle_index = LoadU32(base, kPostFxTimecycleIndexGlobal);
  if (postfx && timecycle_index <= kMaximumEnvironmentalContextIndex) {
    const uint32_t record = postfx + timecycle_index * kPostFxTimecycleStride;
    data.directional_motion_blur_length = std::bit_cast<float>(
        LoadU32(base, record + kPostFxDirectionalMotionBlurLengthOffset));
    if (std::isfinite(data.directional_motion_blur_length)) {
      data.valid_fields |=
          EnvironmentalFieldBit(EnvironmentalField::kDirectionalMotionBlurLength);
    } else {
      data.directional_motion_blur_length = 0.0f;
    }
  }

  // sub_828BD648 publishes the active grcViewport through this guest global.
  // FusionFix's validated grcViewport layout names these raw float[4][4]
  // regions and the camera row consumed for altitude-dependent effects.
  const uint32_t viewport = LoadU32(base, kCurrentViewportGlobal);
  if (viewport) {
    if (LoadFiniteGuestFloats(base, viewport + kViewportViewOffset, data.view_matrix)) {
      data.valid_fields |= EnvironmentalFieldBit(EnvironmentalField::kViewMatrix);
    }
    if (LoadFiniteGuestFloats(base, viewport + kViewportViewInverseOffset,
                              data.view_inverse_matrix)) {
      data.valid_fields |= EnvironmentalFieldBit(EnvironmentalField::kViewInverseMatrix);
    }
    if (LoadFiniteGuestFloats(base, viewport + kViewportProjectionOffset,
                              data.projection_matrix)) {
      data.valid_fields |= EnvironmentalFieldBit(EnvironmentalField::kProjectionMatrix);
    }
    if (LoadFiniteGuestFloats(base, viewport + kViewportViewProjectionOffset,
                              data.view_projection_matrix)) {
      data.valid_fields |= EnvironmentalFieldBit(EnvironmentalField::kViewProjectionMatrix);
    }
    if (LoadFiniteGuestFloats(base, viewport + kViewportCameraPositionOffset,
                              data.camera_position)) {
      data.camera_altitude = data.camera_position[2];
      data.valid_fields |= EnvironmentalFieldBit(EnvironmentalField::kCameraPosition) |
                           EnvironmentalFieldBit(EnvironmentalField::kCameraAltitude);
    }
  }

  return data;
}

void SubmitEnvironmentalData(uint8_t* base, uint32_t device, uint32_t postfx) {
  if (!device) {
    return;
  }
  UpdateEnvironmentalDataCommand command{};
  command.device = device;
  command.data = CaptureEnvironmentalData(base, postfx);
  if (!SubmitNativeCommand(command)) {
    static std::atomic<uint64_t> rejection_count{0};
    const uint64_t count = NextNativeHookDiagnosticCall(rejection_count);
    if (ShouldLogNativeHookCall(count)) {
      REXLOG_WARN(
          "gta4-native-environment: rejected snapshot #{} sequence={} valid={:016X}",
          count, command.data.source_sequence, command.data.valid_fields);
    }
  }
}

bool ShouldLogNativeHookCall(uint64_t call_count) {
  return call_count != 0 &&
         rex::diagnostics::IsEnabled(rex::diagnostics::Category::kGuestHooks) &&
         (call_count <= 32 || !(call_count % 4096));
}

uint64_t NextNativeHookDiagnosticCall(std::atomic<uint64_t>& counter) {
  if (!rex::diagnostics::IsEnabled(rex::diagnostics::Category::kGuestHooks)) {
    return 0;
  }
  return counter.fetch_add(1, std::memory_order_relaxed) + 1;
}

void SubmitRenderPhaseMarker(uint32_t device, RenderPhase phase, RenderPhaseEvent event,
                             uint32_t object, uint32_t caller) {
  if (!device) {
    return;
  }
  RenderPhaseMarkerCommand marker{};
  marker.device = device;
  marker.phase = phase;
  marker.event = event;
  marker.object = object;
  marker.caller = caller;
  SubmitNativeCommand(marker);
}

class ScopedRenderPhaseMarker {
 public:
  ScopedRenderPhaseMarker(uint32_t device, RenderPhase phase, uint32_t object,
                          uint32_t caller)
      : device_(device), phase_(phase), object_(object), caller_(caller) {
    SubmitRenderPhaseMarker(device_, phase_, RenderPhaseEvent::kBegin, object_, caller_);
  }

  ~ScopedRenderPhaseMarker() {
    SubmitRenderPhaseMarker(device_, phase_, RenderPhaseEvent::kEnd, object_, caller_);
  }

  ScopedRenderPhaseMarker(const ScopedRenderPhaseMarker&) = delete;
  ScopedRenderPhaseMarker& operator=(const ScopedRenderPhaseMarker&) = delete;

 private:
  uint32_t device_;
  RenderPhase phase_;
  uint32_t object_;
  uint32_t caller_;
};

template <typename Command, typename Result>
bool ExecuteNativeCommand(const Command& command, Result& result) {
  if (auto* graphics = GetNativeGraphicsSystem()) {
    return graphics->ExecuteTitleCommand(kTitleId, kTitleCommandAbi, &command, sizeof(command),
                                         &result, sizeof(result));
  }
  return false;
}

void DumpXenosShaderContainer(uint8_t* base, uint32_t shader_container, uint32_t total_size,
                              uint64_t hash, ShaderStage stage,
                              const std::filesystem::path& dump_root) {
  static std::mutex dump_mutex;
  std::lock_guard lock(dump_mutex);

  const std::filesystem::path target_directory = dump_root / "gta4_containers";
  std::error_code error;
  std::filesystem::create_directories(target_directory, error);
  if (error) {
    REXLOG_ERROR("GTA IV shader dump: failed to create '{}': {}", target_directory.string(),
                 error.message());
    return;
  }

  const char* stage_extension = stage == ShaderStage::kVertex ? "vert" : "frag";
  const std::filesystem::path output_path =
      target_directory / fmt::format("gta4_{:016X}.container.{}", hash, stage_extension);
  if (std::filesystem::exists(output_path, error) && !error) {
    return;
  }

  std::filesystem::path temporary_path = output_path;
  temporary_path += ".tmp";
  FILE* output = std::fopen(temporary_path.string().c_str(), "wb");
  if (!output) {
    REXLOG_ERROR("GTA IV shader dump: failed to open '{}'", temporary_path.string());
    return;
  }

  const size_t bytes_written =
      std::fwrite(GuestPointer(base, shader_container), 1, total_size, output);
  const bool close_succeeded = std::fclose(output) == 0;
  if (bytes_written != total_size || !close_succeeded) {
    std::filesystem::remove(temporary_path, error);
    REXLOG_ERROR("GTA IV shader dump: incomplete write for {:016X}", hash);
    return;
  }

  std::filesystem::rename(temporary_path, output_path, error);
  if (error) {
    std::filesystem::remove(temporary_path, error);
    REXLOG_ERROR("GTA IV shader dump: failed to publish '{}'", output_path.string());
  }
}

uint32_t UpdateResourceLockCount(uint8_t* base, uint32_t resource, int32_t delta) {
  auto* raw_value = reinterpret_cast<uint32_t*>(GuestPointer(base, resource));
  uint32_t expected_raw = __atomic_load_n(raw_value, __ATOMIC_SEQ_CST);
  while (true) {
    const uint32_t previous = __builtin_bswap32(expected_raw);
    const uint32_t updated = previous + uint32_t(delta);
    const uint32_t desired_raw = __builtin_bswap32(updated);
    if (__atomic_compare_exchange_n(raw_value, &expected_raw, desired_raw, false, __ATOMIC_SEQ_CST,
                                    __ATOMIC_SEQ_CST)) {
      return previous;
    }
  }
}

void RegisterNativeShader(PPCContext& ctx, uint8_t* base, GuestFunction implementation,
                          ShaderStage stage) {
  const bool native_mode = IsNativeMode();
  const std::string dump_path =
      native_mode ? std::string() : rex::cvar::Query<std::string>("dump_shaders");
  if (!native_mode && dump_path.empty()) {
    implementation(ctx, base);
    return;
  }

  const uint32_t shader_container = ctx.r3.u32;
  const uint32_t virtual_size = shader_container ? LoadU32(base, shader_container + 4) : 0;
  const uint32_t physical_size = shader_container ? LoadU32(base, shader_container + 8) : 0;
  const bool size_valid = virtual_size <= UINT32_MAX - physical_size;
  const uint32_t total_size = size_valid ? virtual_size + physical_size : 0;
  const uint64_t hash =
      total_size ? XXH3_64bits(GuestPointer(base, shader_container), total_size) : 0;

  implementation(ctx, base);

  if (ctx.r3.u32 && hash) {
    if (native_mode) {
      RegisterShaderCommand command;
      command.shader = ctx.r3.u32;
      command.stage = stage;
      command.hash = hash;
      SubmitNativeCommand(command);
    } else {
      DumpXenosShaderContainer(base, shader_container, total_size, hash, stage, dump_path);
    }
  }
}

bool SubmitNativeVertexDeclaration(uint8_t* base, uint32_t device, uint32_t declaration) {
  // The GTA IV default-state table binds 0xFFFF before any declaration object
  // exists. The generated setter stores this value but never dereferences it.
  if (declaration <= UINT16_MAX || declaration == UINT32_MAX ||
      LoadU32(base, declaration) != kVertexDeclarationMagic) {
    return false;
  }

  const uint32_t element_count = LoadU32(base, declaration + kVertexDeclarationElementCountOffset);
  const uint32_t maximum_stream =
      LoadU32(base, declaration + kVertexDeclarationMaximumStreamOffset);
  if (!element_count || element_count > kMaximumVertexElementCount ||
      maximum_stream >= kVertexStreamCount) {
    return false;
  }

  RegisterVertexDeclarationCommand command;
  command.device = device;
  command.declaration = declaration;
  command.element_count = element_count;
  command.maximum_stream = maximum_stream;
  const uint32_t elements = declaration + kVertexDeclarationElementsOffset;
  for (uint32_t index = 0; index < element_count; ++index) {
    const uint32_t element = elements + index * sizeof(VertexElement);
    VertexElement& output = command.elements[index];
    output.stream =
        __builtin_bswap16(*reinterpret_cast<volatile uint16_t*>(GuestPointer(base, element)));
    output.offset =
        __builtin_bswap16(*reinterpret_cast<volatile uint16_t*>(GuestPointer(base, element + 2)));
    output.type = LoadU32(base, element + 4);
    output.method = LoadU8(base, element + 8);
    output.usage = LoadU8(base, element + 9);
    output.usage_index = LoadU8(base, element + 10);
    output.padding = 0;
  }

  bool log_declaration = false;
  {
    std::lock_guard lock(g_vertex_declaration_diagnostic_mutex);
    if (g_logged_vertex_declarations.size() < kVertexDeclarationDiagnosticLimit) {
      log_declaration = g_logged_vertex_declarations.emplace(declaration, true).second;
    }
  }
  if (log_declaration) {
    std::string elements_summary;
    for (uint32_t index = 0; index < element_count; ++index) {
      const VertexElement& element = command.elements[index];
      elements_summary += fmt::format("{}s{}+{} type={:08X} method={} usage={}/{}",
                                      index ? ", " : "", element.stream, element.offset,
                                      element.type, element.method, element.usage,
                                      element.usage_index);
    }
    REXLOG_INFO(
        "gta4-native-vertex-declaration: handle={:08X} elements={} maximum-stream={} [{}]",
        declaration, element_count, maximum_stream, elements_summary);
  }
  SubmitNativeCommand(command);
  return true;
}

PPCContext InvokeGuest(PPCContext& parent, uint8_t* base, GuestFunction function, uint64_t r3 = 0,
                       uint64_t r4 = 0, uint64_t r5 = 0, uint64_t r6 = 0, uint64_t r7 = 0) {
  PPCContext nested = parent;
  nested.r3.u64 = r3;
  nested.r4.u64 = r4;
  nested.r5.u64 = r5;
  nested.r6.u64 = r6;
  nested.r7.u64 = r7;
  function(nested, base);
  return nested;
}

void FreeAlignedGuestAllocation(PPCContext& ctx, uint8_t* base, uint32_t address) {
  if (!address) {
    return;
  }
  const uint32_t allocation = LoadU32(base, address - 4);
  if (allocation) {
    InvokeGuest(ctx, base, sub_821B3700, allocation, kAlignedAllocationFreeFlags);
  }
}

bool CachedSelectorMatches(uint32_t command_mask, uint32_t replay_mask) {
  return !command_mask || !replay_mask || (command_mask & replay_mask) != 0;
}

bool IsDrawCommandType(CommandType type) {
  return type == CommandType::kDrawPrimitive || type == CommandType::kDrawPrimitiveUp ||
         type == CommandType::kDrawIndexedPrimitive;
}

void PatchCommandDevice(std::vector<uint8_t>& bytes, uint32_t device) {
  if (bytes.size() < sizeof(CommandHeader) + sizeof(device)) {
    return;
  }
  std::memcpy(bytes.data() + sizeof(CommandHeader), &device, sizeof(device));
}

bool SubmitCapturedNativeCommand(PPCContext& ctx, uint8_t* base, uint32_t device,
                                 uint32_t command_list, uint32_t replay_mask,
                                 uint32_t replay_command_ordinal,
                                 const CapturedNativeCommand& captured) {
  auto* graphics = GetNativeGraphicsSystem();
  if (!graphics || captured.bytes.empty()) {
    return false;
  }

  std::vector<uint8_t> bytes = captured.bytes;
  PatchCommandDevice(bytes, device);

  // A cached command may execute many times. Give each execution unique
  // provenance at submission rather than reusing the ID and LR from the list
  // build, otherwise transition events from distinct frames collide.
  DrawIndexedPrimitiveCommand indexed_draw{};
  if (captured.type == CommandType::kDrawIndexedPrimitive) {
    if (bytes.size() != sizeof(DrawIndexedPrimitiveCommand)) {
      return false;
    }
    DrawIndexedPrimitiveCommand draw;
    std::memcpy(&draw, bytes.data(), sizeof(draw));
    draw.caller = static_cast<uint32_t>(ctx.lr);
    draw.origin_flags = kDrawCommandOriginCachedReplay;
    draw.command_list = command_list;
    draw.draw_id =
        g_indexed_draw_invocation_id.fetch_add(1, std::memory_order_relaxed) + 1;
    indexed_draw = draw;
    std::memcpy(bytes.data(), &draw, sizeof(draw));
  }

  uint32_t replay_vertex_data = 0;
  if (captured.type == CommandType::kDrawPrimitiveUp) {
    if (bytes.size() != sizeof(DrawPrimitiveUpCommand)) {
      return false;
    }
    DrawPrimitiveUpCommand draw;
    std::memcpy(&draw, bytes.data(), sizeof(draw));
    if (!captured.payload.empty()) {
      replay_vertex_data =
          InvokeGuest(ctx, base, D3D_MemAllocAligned, captured.payload.size(), 16).r3.u32;
      if (!replay_vertex_data) {
        return false;
      }
      std::memcpy(GuestPointer(base, replay_vertex_data), captured.payload.data(),
                  captured.payload.size());
      draw.vertex_data = replay_vertex_data;
      draw.vertex_data_size = uint32_t(captured.payload.size());
      std::memcpy(bytes.data(), &draw, sizeof(draw));
    }
  }

  const bool trace_gbuffer_replay =
      captured.draw_diagnostic_valid &&
      rex::diagnostics::IsEnabled(
          rex::diagnostics::Category::kNativeTrace);
  const uint32_t replay_frame =
      trace_gbuffer_replay
          ? LoadU32(base, device + kSubmittedFrameOffset)
          : 0;
  auto log_replay_state =
      [&](std::string_view point,
          const std::array<uint32_t, 16>& transform,
          uint64_t constants_hash, uint64_t transform_hash, bool accepted) {
        if (!trace_gbuffer_replay) {
          return;
        }
        REXLOG_INFO(
            "gta4-native-cause: point={} frame={} capture-frame={} "
            "build={:08X} command-list={:08X} capture-ordinal={} "
            "replay-ordinal={} selector={:08X} replay-mask={:08X} "
            "capture-device={:08X} replay-device={:08X} caller={:08X} "
            "draw={} indices={} accepted={} capture-constants={:016X} "
            "capture-transform={:016X} constants={:016X} "
            "transform={:016X} capture-known-offscreen={} "
            "known-offscreen={} vs-handle={:08X} ps-handle={:08X} "
            "capture-vdecl={:08X} live-vdecl={:08X} capture-ib={:08X} "
            "live-ib={:08X} c8={:08X},{:08X},{:08X},{:08X} "
            "c9={:08X},{:08X},{:08X},{:08X} "
            "c10={:08X},{:08X},{:08X},{:08X} "
            "c11={:08X},{:08X},{:08X},{:08X}",
            point, replay_frame, captured.capture_frame,
            captured.capture_build_object, command_list,
            captured.capture_command_ordinal, replay_command_ordinal,
            captured.selector_mask, replay_mask, captured.capture_device,
            device, indexed_draw.caller, indexed_draw.draw_id,
            indexed_draw.index_count, accepted,
            captured.capture_constants_hash,
            captured.capture_transform_hash, constants_hash, transform_hash,
            IsKnownOffscreenVertexTransform(captured.capture_transform),
            IsKnownOffscreenVertexTransform(transform),
            captured.capture_vertex_shader, captured.capture_pixel_shader,
            captured.capture_vertex_declaration,
            LoadU32(base, device + kVertexDeclarationOffset),
            captured.capture_index_buffer,
            LoadU32(base, device + kIndexBufferOffset), transform[0],
            transform[1], transform[2], transform[3], transform[4],
            transform[5], transform[6], transform[7], transform[8],
            transform[9], transform[10], transform[11], transform[12],
            transform[13], transform[14], transform[15]);
      };
  auto capture_live_hashes = [&]() {
    return std::pair<uint64_t, uint64_t>{
        XXH3_64bits(
            GuestPointer(base, device + kReplayShaderConstantsOffset),
            kReplayShaderConstantsSize),
        XXH3_64bits(
            GuestPointer(base,
                         device + kReplayVertexTransformConstantsOffset),
            kReplayVertexTransformConstantsSize)};
  };

  if (trace_gbuffer_replay) {
    const auto [constants_hash, transform_hash] = capture_live_hashes();
    log_replay_state("cached-draw-replay-before",
                     CaptureLiveVertexTransform(base, device), constants_hash,
                     transform_hash, false);
  }

  bool accepted = false;
  {
    ScopedReplayDrawState replay_state(base, device, captured.draw_snapshot);
    if (trace_gbuffer_replay) {
      const auto [constants_hash, transform_hash] = capture_live_hashes();
      log_replay_state("cached-draw-replay-applied",
                       CaptureLiveVertexTransform(base, device),
                       constants_hash, transform_hash, false);
    }
    accepted = graphics->SubmitTitleCommand(kTitleId, kTitleCommandAbi, bytes.data(), bytes.size());
  }
  if (trace_gbuffer_replay) {
    const auto [constants_hash, transform_hash] = capture_live_hashes();
    log_replay_state("cached-draw-replay-restored",
                     CaptureLiveVertexTransform(base, device), constants_hash,
                     transform_hash, accepted);
  }
  FreeAlignedGuestAllocation(ctx, base, replay_vertex_data);
  return accepted;
}

std::shared_ptr<const CachedNativeCommandList> FindCachedNativeCommandList(uint32_t command_list) {
  std::lock_guard lock(g_cached_native_command_lists_mutex);
  const auto found = g_cached_native_command_lists.find(command_list);
  return found != g_cached_native_command_lists.end() ? found->second : nullptr;
}

void PublishCachedNativeCommandList(uint32_t command_list,
                                    std::vector<CapturedNativeCommand> commands) {
  if (!command_list) {
    return;
  }
  auto cached = std::make_shared<CachedNativeCommandList>();
  cached->commands = std::move(commands);
  std::lock_guard lock(g_cached_native_command_lists_mutex);
  g_cached_native_command_lists[command_list] = std::move(cached);
}

void EraseCachedNativeCommandList(uint32_t command_list) {
  if (!command_list) {
    return;
  }
  std::lock_guard lock(g_cached_native_command_lists_mutex);
  g_cached_native_command_lists.erase(command_list);
}

bool ReplayCachedNativeCommandList(PPCContext& ctx, uint8_t* base, uint32_t device,
                                   uint32_t command_list, uint32_t replay_mask,
                                   uint64_t& selected_count, uint64_t& submitted_count) {
  const auto cached = FindCachedNativeCommandList(command_list);
  if (!cached) {
    return false;
  }

  selected_count = 0;
  submitted_count = 0;
  if (g_active_native_capture.active) {
    for (const auto& command : cached->commands) {
      if (!CachedSelectorMatches(command.selector_mask, replay_mask)) {
        continue;
      }
      CapturedNativeCommand flattened = command;
      flattened.selector_mask = g_active_native_capture.selector_mask;
      g_active_native_capture.commands.push_back(std::move(flattened));
      ++selected_count;
      ++submitted_count;
    }
    return true;
  }

  for (size_t command_index = 0; command_index < cached->commands.size();
       ++command_index) {
    const auto& command = cached->commands[command_index];
    if (!CachedSelectorMatches(command.selector_mask, replay_mask)) {
      continue;
    }
    ++selected_count;
    if (command.type == CommandType::kResolve &&
        command.bytes.size() == sizeof(ResolveCommand)) {
      ResolveCommand resolve{};
      std::memcpy(&resolve, command.bytes.data(), sizeof(resolve));
      const uint32_t known_frontbuffer =
          g_last_present_frontbuffer.load(std::memory_order_relaxed);
      if (rex::diagnostics::IsEnabled(rex::diagnostics::Category::kGuestHooks) &&
          ((known_frontbuffer && resolve.destination_texture == known_frontbuffer) ||
           (resolve.flags & 0x04000000u))) {
        std::fprintf(
            stderr,
            "[ResolveOriginTrace] origin=replay caller=%08X command-list=%08X "
            "replay-mask=%08X selector=%08X "
            "source=%08X/%08X destination=%08X flags=%08X\n",
            uint32_t(ctx.lr), command_list, replay_mask, command.selector_mask, resolve.source.handle,
            resolve.source.address, resolve.destination_texture, resolve.flags);
        std::fflush(stderr);
      }
    }
    if (SubmitCapturedNativeCommand(ctx, base, device, command_list,
                                    replay_mask,
                                    uint32_t(command_index + 1), command)) {
      ++submitted_count;
    }
  }
  return true;
}

void StoreAllDirty(uint8_t* base, uint32_t device) {
  StoreU64(base, device, UINT64_MAX);
  StoreU64(base, device + 8, UINT64_MAX);
  StoreU64(base, device + 16, UINT64_MAX);
  StoreU64(base, device + 24, UINT64_MAX);
  StoreU64(base, device + 32, UINT64_MAX);
}

void ApplyNativeDirtyDefaults(uint8_t* base, uint32_t device) {
  StoreU64(base, device + 16, LoadU64(base, device + 16) | kNativeDirtyMask16);
  StoreU64(base, device + 24, LoadU64(base, device + 24) | kNativeDirtyMask24);
  StoreU64(base, device + 32, LoadU64(base, device + 32) | kNativeDirtyMask32);
}

void ApplyDeviceCpuPrefix(uint8_t* base, uint32_t device) {
  StoreU64(base, device + 11816, UINT32_MAX);
  for (uint32_t index = 0; index < 26; ++index) {
    const uint32_t address = device + 1152 + index * 24;
    StoreU32(base, address, LoadU32(base, address) & ~uint32_t(3));
  }
  for (uint32_t index = 0; index < 18; ++index) {
    const uint32_t address = device + 1776 + index * 8;
    StoreU32(base, address, (LoadU32(base, address) & ~uint32_t(3)) | 1);
  }

  StoreU32(base, device + 10428, 0x20002000);
  StoreU32(base, device + 10604, 8);
  StoreU32(base, device + 10708, 0x000FF000);
  StoreU32(base, device + 10712, 0x000FF100);
  StoreU32(base, device + 10564, LoadU32(base, device + 10564) | 0x00080000);
  StoreU32(base, device + 10628, 14);
  StoreU32(base, device + 10580, 4);
  StoreU32(base, device + 10688, 4);
  StoreU32(base, device + 10768, 14);
  StoreU32(base, device + 10772, 16);
  StoreU32(base, device + 10568, LoadU32(base, device + 10568) | 0x00010000);
  StoreU32(base, device + 10444, 0x00FFFFFF);
  StoreU32(base, device + 10824, 2);
  StoreU32(base, device + 10916, 14);
  StoreAllDirty(base, device);
}

void PublishGlobalDevice(uint8_t* base, uint32_t device) {
  const uint32_t global_device = LoadU32(base, kVdGlobalDeviceImport);
  if (global_device) {
    StoreU32(base, global_device, device);
  }
}

void FreeNativeDeviceAllocation(PPCContext& ctx, uint8_t* base, uint32_t device) {
  const uint32_t fallback = LoadU32(base, device + 16712);
  if (fallback) {
    InvokeGuest(ctx, base, sub_821B3700, fallback, kFallbackAllocationFreeFlags);
  }
  FreeAlignedGuestAllocation(ctx, base, device);
}

void InitializeNativeCommandScratch(uint8_t* base, uint32_t device) {
  const uint32_t scratch = LoadU32(base, device + 16712);
  StoreU32(base, device + kCommandScratchHeadOffset, 0);
  StoreU32(base, device + kCommandScratchWriteOffset, scratch);
  StoreU32(base, device + kCommandScratchEndOffset, scratch ? scratch + kCommandScratchSize : 0);
}

void ConsumeNativeDrawDirtyState(uint8_t* base, uint32_t device) {
  StoreU64(base, device, 0);
  StoreU64(base, device + 8, 0);
  StoreU64(base, device + 16, 0);
  StoreU64(base, device + 24, 0);
  StoreU64(base, device + 32, 0);
}

bool InitializePrimaryCpuState(PPCContext& ctx, uint8_t* base, uint32_t device,
                               uint32_t presentation) {
  InvokeGuest(ctx, base, __imp__RtlInitializeCriticalSection, device + 14928);
  InvokeGuest(ctx, base, __imp__RtlInitializeCriticalSection, device + 14956);
  StoreU8(base, device + 10942, LoadU8(base, device + 10942) | 4);
  PublishGlobalDevice(base, device);

  const uint32_t scratch = LoadU32(base, device + 16712);
  if (!scratch) {
    return false;
  }
  StoreU16(base, scratch, 0);
  PPCContext config_result =
      InvokeGuest(ctx, base, __imp__ExGetXConfigSetting, 3, 10, device + 16700, 4, scratch);
  if (config_result.r3.s32 < 0) {
    return false;
  }

  StoreU32(base, device + 21540, UINT32_MAX);
  StoreU32(base, device + 21544, UINT32_MAX);
  ApplyDeviceCpuPrefix(base, device);

  PPCContext display_result = InvokeGuest(ctx, base, sub_82A503C8, device, presentation);
  if (!display_result.r3.u32) {
    return false;
  }
  InvokeGuest(ctx, base, sub_82A50160, device);

  PPCContext frequency_result = InvokeGuest(ctx, base, sub_82A153C0, scratch);
  (void)frequency_result;
  const int64_t frequency = int64_t(LoadU64(base, scratch));
  if (frequency > 0) {
    const float frequency_float = float(frequency);
    StoreU32(base, device + 21568, std::bit_cast<uint32_t>(frequency_float));
    StoreU32(base, device + 21572, std::bit_cast<uint32_t>(1.0f / frequency_float));
  }

  InvokeGuest(ctx, base, sub_82A4DAB0, device);
  InvokeGuest(ctx, base, sub_82A3BAA8, device, 0);
  return InvokeGuest(ctx, base, sub_82A53058, device).r3.u32 != 0;
}

bool InitializeSecondaryCpuState(PPCContext& ctx, uint8_t* base, uint32_t device) {
  StoreU8(base, device + 10940, LoadU8(base, device + 10940) | 0x80);
  StoreU32(base, device + 14920, UINT32_MAX);
  StoreU32(base, device + 21540, UINT32_MAX);
  StoreU32(base, device + 21544, UINT32_MAX);
  ApplyDeviceCpuPrefix(base, device);
  InvokeGuest(ctx, base, sub_82A50160, device);
  return true;
}

void RetireNativeBoundResource(uint8_t* base, uint32_t device, uint32_t resource) {
  if (!resource) {
    return;
  }
  const uint32_t live_fence = LoadU32(base, device + kLiveResourceFenceOffset);
  if (live_fence) {
    StoreU32(base, resource + kResourceFenceOffset, live_fence);
  }
}

void ApplyNativeShaderState(PPCContext& ctx, uint8_t* base, GuestFunction implementation) {
  const uint32_t device = ctx.r3.u32;
  const uint32_t pending_resource_mask = LoadU32(base, device + kPendingResourceMaskOffset);

  // The generated shader setters combine CPU-visible shader state expansion with an
  // eight-byte PM4 resource-retirement packet. Native mode retains the former, while
  // making the packet predicate false for this synchronous call.
  StoreU32(base, device + kPendingResourceMaskOffset, 0);
  implementation(ctx, base);
  StoreU32(base, device + kPendingResourceMaskOffset, pending_resource_mask);
}

extern "C" void sub_82A42168(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode() ||
      !rex::diagnostics::IsEnabled(
          rex::diagnostics::Category::kNativeTrace)) {
    __imp__sub_82A42168(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  const uint32_t first_register = ctx.r4.u32;
  const uint32_t source = ctx.r5.u32;
  const uint32_t register_count = ctx.r6.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  const uint64_t register_end =
      uint64_t(first_register) + uint64_t(register_count);
  const bool touches_gbuffer_transform =
      register_count && first_register < 12 && register_end > 8;

  std::array<uint32_t, 16> transform_before{};
  if (touches_gbuffer_transform) {
    for (uint32_t index = 0; index < transform_before.size(); ++index) {
      transform_before[index] = LoadU32(
          base, device + kReplayVertexTransformConstantsOffset +
                    index * sizeof(uint32_t));
    }
  }

  __imp__sub_82A42168(ctx, base);

  if (!touches_gbuffer_transform) {
    return;
  }

  std::array<uint32_t, 16> transform_after{};
  for (uint32_t index = 0; index < transform_after.size(); ++index) {
    transform_after[index] = LoadU32(
        base, device + kReplayVertexTransformConstantsOffset +
                  index * sizeof(uint32_t));
  }
  static std::atomic<uint64_t> transform_upload_count{0};
  static std::atomic<uint64_t> bad_transform_upload_count{0};
  const uint64_t ordinal =
      transform_upload_count.fetch_add(1, std::memory_order_relaxed) + 1;
  const bool known_offscreen_transform =
      transform_after[12] == 0x431B959E &&
      transform_after[13] == 0x4311C5BE &&
      transform_after[14] == 0x3C7DD5B0 &&
      transform_after[15] == 0x415D1833;
  const uint64_t bad_ordinal =
      known_offscreen_transform
          ? bad_transform_upload_count.fetch_add(1,
                                                 std::memory_order_relaxed) +
                1
          : 0;
  if (ordinal > 64 && (!bad_ordinal || bad_ordinal > 32)) {
    return;
  }

  uint64_t source_hash = 0;
  if (source && register_count <= 256) {
    source_hash = XXH3_64bits(GuestPointer(base, source),
                              size_t(register_count) * 16);
  }
  const uint32_t submitted_frame =
      LoadU32(base, device + kSubmittedFrameOffset);
  REXLOG_INFO(
      "gta4-native-cause: point=vertex-constant-producer frame={} ordinal={} "
      "bad-ordinal={} caller={:08X} device={:08X} first={} count={} "
      "source={:08X} source-hash={:016X} known-offscreen={} "
      "before-c11={:08X},{:08X},{:08X},{:08X} "
      "after-c8={:08X},{:08X},{:08X},{:08X} "
      "after-c9={:08X},{:08X},{:08X},{:08X} "
      "after-c10={:08X},{:08X},{:08X},{:08X} "
      "after-c11={:08X},{:08X},{:08X},{:08X}",
      submitted_frame, ordinal, bad_ordinal, caller, device, first_register,
      register_count, source, source_hash, known_offscreen_transform,
      transform_before[12], transform_before[13], transform_before[14],
      transform_before[15], transform_after[0], transform_after[1],
      transform_after[2], transform_after[3], transform_after[4],
      transform_after[5], transform_after[6], transform_after[7],
      transform_after[8], transform_after[9], transform_after[10],
      transform_after[11], transform_after[12], transform_after[13],
      transform_after[14], transform_after[15]);
}

void LogKnownOffscreenTransformBoundary(std::string_view point,
                                        const PPCContext& ctx, uint8_t* base,
                                        uint32_t object,
                                        uint32_t explicit_device = 0) {
  if (!IsNativeMode() ||
      !rex::diagnostics::IsEnabled(
          rex::diagnostics::Category::kNativeTrace)) {
    return;
  }
  const uint32_t device = explicit_device
                              ? explicit_device
                              : LoadU32(base, kDeferredDeviceGlobal);
  if (!device) {
    return;
  }
  const std::array<uint32_t, 16> transform =
      CaptureLiveVertexTransform(base, device);
  if (!IsKnownOffscreenVertexTransform(transform)) {
    return;
  }
  const uint64_t constants_hash = XXH3_64bits(
      GuestPointer(base, device + kReplayShaderConstantsOffset),
      kReplayShaderConstantsSize);
  const uint64_t transform_hash = XXH3_64bits(
      GuestPointer(base, device + kReplayVertexTransformConstantsOffset),
      kReplayVertexTransformConstantsSize);
  const NativeShaderBindingDiagnosticState bindings =
      CaptureNativeShaderBindings(device);
  REXLOG_INFO(
      "gta4-native-cause: point={} frame={} caller={:08X} object={:08X} "
      "device={:08X} args={:08X},{:08X},{:08X},{:08X},{:08X} "
      "vs-handle={:08X} ps-handle={:08X} vdecl={:08X} ib={:08X} "
      "constants={:016X} transform={:016X} "
      "c8={:08X},{:08X},{:08X},{:08X} "
      "c9={:08X},{:08X},{:08X},{:08X} "
      "c10={:08X},{:08X},{:08X},{:08X} "
      "c11={:08X},{:08X},{:08X},{:08X}",
      point, LoadU32(base, device + kSubmittedFrameOffset), uint32_t(ctx.lr),
      object, device, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
      ctx.r7.u32, bindings.vertex_shader, bindings.pixel_shader,
      LoadU32(base, device + kVertexDeclarationOffset),
      LoadU32(base, device + kIndexBufferOffset), constants_hash,
      transform_hash, transform[0], transform[1], transform[2],
      transform[3], transform[4], transform[5], transform[6], transform[7],
      transform[8], transform[9], transform[10], transform[11],
      transform[12], transform[13], transform[14], transform[15]);
}

void LogKnownOffscreenTransformTransition(
    std::string_view point, uint8_t* base, uint32_t device,
    uint32_t caller, uint32_t object,
    const std::array<uint32_t, 5>& args,
    const std::array<uint32_t, 16>& before,
    const std::array<uint32_t, 16>& after) {
  if (!IsNativeMode() ||
      !rex::diagnostics::IsEnabled(
          rex::diagnostics::Category::kNativeTrace) ||
      !device) {
    return;
  }
  const bool before_known = IsKnownOffscreenVertexTransform(before);
  const bool after_known = IsKnownOffscreenVertexTransform(after);
  if (!before_known && !after_known) {
    return;
  }
  const uint64_t before_hash =
      XXH3_64bits(before.data(), sizeof(before));
  const uint64_t after_hash = XXH3_64bits(after.data(), sizeof(after));
  REXLOG_INFO(
      "gta4-native-cause: point={} frame={} caller={:08X} object={:08X} "
      "device={:08X} args={:08X},{:08X},{:08X},{:08X},{:08X} "
      "before-known={} after-known={} changed={} "
      "before-transform={:016X} after-transform={:016X} "
      "before-c8={:08X},{:08X},{:08X},{:08X} "
      "before-c9={:08X},{:08X},{:08X},{:08X} "
      "before-c10={:08X},{:08X},{:08X},{:08X} "
      "before-c11={:08X},{:08X},{:08X},{:08X} "
      "after-c8={:08X},{:08X},{:08X},{:08X} "
      "after-c9={:08X},{:08X},{:08X},{:08X} "
      "after-c10={:08X},{:08X},{:08X},{:08X} "
      "after-c11={:08X},{:08X},{:08X},{:08X}",
      point, LoadU32(base, device + kSubmittedFrameOffset), caller, object,
      device, args[0], args[1], args[2], args[3], args[4], before_known,
      after_known, before != after, before_hash, after_hash, before[0],
      before[1], before[2], before[3], before[4], before[5], before[6],
      before[7], before[8], before[9], before[10], before[11], before[12],
      before[13], before[14], before[15], after[0], after[1], after[2],
      after[3], after[4], after[5], after[6], after[7], after[8], after[9],
      after[10], after[11], after[12], after[13], after[14], after[15]);
}

template <typename Function>
void TraceKnownOffscreenTransformTransition(std::string_view point,
                                            PPCContext& ctx, uint8_t* base,
                                            Function function) {
  if (!IsNativeMode() ||
      !rex::diagnostics::IsEnabled(
          rex::diagnostics::Category::kNativeTrace)) {
    function(ctx, base);
    return;
  }
  const uint32_t device = LoadU32(base, kDeferredDeviceGlobal);
  if (!device) {
    function(ctx, base);
    return;
  }
  const uint32_t caller = uint32_t(ctx.lr);
  const uint32_t object = ctx.r3.u32;
  const std::array<uint32_t, 5> args = {
      ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32};
  const std::array<uint32_t, 16> before =
      CaptureLiveVertexTransform(base, device);
  function(ctx, base);
  const std::array<uint32_t, 16> after =
      CaptureLiveVertexTransform(base, device);
  LogKnownOffscreenTransformTransition(point, base, device, caller, object,
                                       args, before, after);
}

extern "C" void sub_828C6568(PPCContext& ctx, uint8_t* base) {
  TraceKnownOffscreenTransformTransition(
      "offscreen-transition-material-list-select", ctx, base,
      __imp__sub_828C6568);
}

extern "C" void sub_828C6620(PPCContext& ctx, uint8_t* base) {
  TraceKnownOffscreenTransformTransition(
      "offscreen-transition-material-list-render", ctx, base,
      __imp__sub_828C6620);
}

extern "C" void sub_828C4338(PPCContext& ctx, uint8_t* base) {
  TraceKnownOffscreenTransformTransition(
      "offscreen-transition-model-materials", ctx, base,
      __imp__sub_828C4338);
}

extern "C" void sub_821BE8A0(PPCContext& ctx, uint8_t* base) {
  TraceKnownOffscreenTransformTransition(
      "offscreen-transition-model-materials-legacy-caller", ctx, base,
      __imp__sub_821BE8A0);
}

extern "C" void sub_828D41A8(PPCContext& ctx, uint8_t* base) {
  TraceKnownOffscreenTransformTransition(
      "offscreen-transition-model-list-caller", ctx, base,
      __imp__sub_828D41A8);
}

extern "C" void sub_828D4268(PPCContext& ctx, uint8_t* base) {
  TraceKnownOffscreenTransformTransition(
      "offscreen-transition-model-transform-caller", ctx, base,
      __imp__sub_828D4268);
}

extern "C" void sub_828BD250(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode() ||
      !rex::diagnostics::IsEnabled(
          rex::diagnostics::Category::kNativeTrace)) {
    __imp__sub_828BD250(ctx, base);
    return;
  }
  const uint32_t device = LoadU32(base, kDeferredDeviceGlobal);
  const uint32_t caller = uint32_t(ctx.lr);
  const uint32_t state = ctx.r3.u32;
  const uint32_t source = ctx.r4.u32;
  const std::array<uint32_t, 16> before =
      device ? CaptureLiveVertexTransform(base, device)
             : std::array<uint32_t, 16>{};
  std::array<uint32_t, 16> source_matrix{};
  if (source) {
    for (uint32_t index = 0; index < source_matrix.size(); ++index) {
      source_matrix[index] =
          LoadU32(base, source + index * sizeof(uint32_t));
    }
  }
  __imp__sub_828BD250(ctx, base);
  const std::array<uint32_t, 16> after =
      device ? CaptureLiveVertexTransform(base, device)
             : std::array<uint32_t, 16>{};
  if (!device ||
      (!IsKnownOffscreenVertexTransform(before) &&
       !IsKnownOffscreenVertexTransform(after))) {
    return;
  }
  const uint64_t source_hash =
      XXH3_64bits(source_matrix.data(), sizeof(source_matrix));
  REXLOG_INFO(
      "gta4-native-cause: point=offscreen-transition-matrix-setter-source "
      "frame={} caller={:08X} state={:08X} source={:08X} "
      "source-transform={:016X} "
      "source-r0={:08X},{:08X},{:08X},{:08X} "
      "source-r1={:08X},{:08X},{:08X},{:08X} "
      "source-r2={:08X},{:08X},{:08X},{:08X} "
      "source-r3={:08X},{:08X},{:08X},{:08X}",
      LoadU32(base, device + kSubmittedFrameOffset), caller, state, source,
      source_hash, source_matrix[0], source_matrix[1], source_matrix[2],
      source_matrix[3], source_matrix[4], source_matrix[5], source_matrix[6],
      source_matrix[7], source_matrix[8], source_matrix[9], source_matrix[10],
      source_matrix[11], source_matrix[12], source_matrix[13],
      source_matrix[14], source_matrix[15]);
  const std::array<uint32_t, 5> args = {state, source, 0, 0, 0};
  LogKnownOffscreenTransformTransition(
      "offscreen-transition-matrix-setter", base, device, caller, state,
      args, before, after);
}

extern "C" void sub_828E7000(PPCContext& ctx, uint8_t* base) {
  LogKnownOffscreenTransformBoundary("offscreen-boundary-render-entry", ctx,
                                     base, ctx.r3.u32);
  __imp__sub_828E7000(ctx, base);
}

extern "C" void sub_828BFC00(PPCContext& ctx, uint8_t* base) {
  LogKnownOffscreenTransformBoundary("offscreen-boundary-indexed-entry", ctx,
                                     base, ctx.r3.u32);
  __imp__sub_828BFC00(ctx, base);
}

extern "C" void sub_828C8DB0(PPCContext& ctx, uint8_t* base) {
  LogKnownOffscreenTransformBoundary("offscreen-boundary-material-vertex", ctx,
                                     base, ctx.r3.u32);
  __imp__sub_828C8DB0(ctx, base);
}

extern "C" void sub_828C8E80(PPCContext& ctx, uint8_t* base) {
  LogKnownOffscreenTransformBoundary("offscreen-boundary-material-pixel", ctx,
                                     base, ctx.r3.u32);
  __imp__sub_828C8E80(ctx, base);
}

extern "C" void sub_828C8F38(PPCContext& ctx, uint8_t* base) {
  LogKnownOffscreenTransformBoundary("offscreen-boundary-material-apply", ctx,
                                     base, ctx.r3.u32);
  __imp__sub_828C8F38(ctx, base);
}

extern "C" void sub_828DFF00(PPCContext& ctx, uint8_t* base) {
  LogKnownOffscreenTransformBoundary("offscreen-boundary-shader-select", ctx,
                                     base, ctx.r3.u32);
  __imp__sub_828DFF00(ctx, base);
}

SurfaceDescriptor CaptureSurfaceDescriptor(uint8_t* base, uint32_t surface) {
  SurfaceDescriptor descriptor;
  descriptor.handle = surface;
  if (!surface) {
    return descriptor;
  }

  descriptor.flags = LoadU32(base, surface);
  descriptor.base = LoadU32(base, surface + 24);
  descriptor.address = LoadU32(base, surface + 28);
  descriptor.packed_dimensions = LoadU32(base, surface + 36);
  descriptor.format = LoadU32(base, surface + 40);
  descriptor.width = (std::rotl(descriptor.packed_dimensions, 14) & 0x3FFF) + 1;
  descriptor.height = (std::rotl(descriptor.packed_dimensions, 29) & 0x7FFF) + 1;
  descriptor.sample_type = DecodeSurfaceSampleType(descriptor.base);
  return descriptor;
}

uint32_t EncodeNativeFetchAddress(uint32_t address) {
  const uint32_t page = std::rotl(address, 12) & 0xFFF;
  const uint32_t bank = (page + 512) & 0x1000;
  return bank + (address & 0x1FFFFFFF);
}

int32_t TruncateViewportCoordinate(float value) {
  if (std::isnan(value) || value <= float(std::numeric_limits<int32_t>::min())) {
    return std::numeric_limits<int32_t>::min();
  }
  if (value >= float(std::numeric_limits<int32_t>::max())) {
    return std::numeric_limits<int32_t>::max();
  }
  return int32_t(value);
}

}  // namespace

extern "C" void sub_822CD0E0(PPCContext& ctx, uint8_t* base) {
  static std::atomic<uint32_t> trace_counter{0};
  const uint32_t trace = BeginCloudGuestTrace(trace_counter);
  CloudGuestStateSnapshot before{};
  if (trace) {
    before = CaptureCloudGuestState(base);
  }
  __imp__sub_822CD0E0(ctx, base);
  if (trace) {
    const CloudGuestStateSnapshot after = CaptureCloudGuestState(base);
    REXLOG_WARN(
        "gta4-native-cloud: point=guest-publish seq={} before=[{}] after=[{}]",
        trace, FormatCloudGuestState(before), FormatCloudGuestState(after));
  }
}

extern "C" void sub_82521D10(PPCContext& ctx, uint8_t* base) {
  static std::atomic<uint32_t> trace_counter{0};
  const uint32_t trace = BeginCloudGuestTrace(trace_counter);
  CloudGuestStateSnapshot before{};
  std::array<uint32_t, kCloudClockGlobals.size()> clock{};
  if (trace) {
    before = CaptureCloudGuestState(base);
    for (uint32_t index = 0; index < clock.size(); ++index) {
      clock[index] = LoadU32(base, kCloudClockGlobals[index]);
    }
  }
  __imp__sub_82521D10(ctx, base);
  if (trace) {
    const CloudGuestStateSnapshot after = CaptureCloudGuestState(base);
    REXLOG_WARN(
        "gta4-native-cloud: point=guest-phase seq={} "
        "clock={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} "
        "before=[{}] after=[{}]",
        trace, clock[0], clock[1], clock[2], clock[3], clock[4], clock[5],
        FormatCloudGuestState(before), FormatCloudGuestState(after));
  }
}

extern "C" void sub_8266F7D8(PPCContext& ctx, uint8_t* base) {
  static std::atomic<uint32_t> trace_counter{0};
  const uint32_t trace = BeginCloudGuestTrace(trace_counter);
  const float time_of_day = float(ctx.f1.f64);
  const uint32_t argument_4 = ctx.r4.u32;
  const uint32_t argument_5 = ctx.r5.u32;
  const uint32_t argument_6 = ctx.r6.u32;
  CloudGuestStateSnapshot before{};
  CloudProceduralSnapshot procedural_before{};
  if (trace) {
    before = CaptureCloudGuestState(base);
    procedural_before = CaptureCloudProceduralState(base);
  }
  __imp__sub_8266F7D8(ctx, base);
  if (trace) {
    const CloudGuestStateSnapshot after = CaptureCloudGuestState(base);
    const CloudProceduralSnapshot procedural_after =
        CaptureCloudProceduralState(base);
    REXLOG_WARN(
        "gta4-native-cloud: point=guest-exec seq={} tod={:.9g} args={},{},{} "
        "before=[{}] after=[{}] proc-before=[{}] proc-after=[{}]",
        trace, time_of_day, argument_4, argument_5, argument_6,
        FormatCloudGuestState(before), FormatCloudGuestState(after),
        FormatCloudProceduralState(procedural_before),
        FormatCloudProceduralState(procedural_after));
  }
}

extern "C" void sub_82670840(PPCContext& ctx, uint8_t* base) {
  if (IsNativeMode() && ctx.r3.u32) {
    CapturedSunPayload captured = g_captured_sun_payload;
    captured.direction_valid =
        LoadFiniteGuestFloats(base, ctx.r3.u32 + kSkySunDirectionOffset, captured.direction);
    captured.color_valid =
        LoadFiniteGuestFloats(base, ctx.r3.u32 + kSkySunColorOffset, captured.color);
    if (captured.direction_valid) {
      // The Xbox upload replaces the fourth source lane with abs(direction.y).
      // The native projection contract consumes a world-space direction (w=0).
      captured.direction[3] = 0.0f;
    }
    g_captured_sun_payload = captured;
  }
  __imp__sub_82670840(ctx, base);
}

extern "C" void sub_821F1670(PPCContext& ctx, uint8_t* base) {
  const uint32_t rectangle = ctx.r3.u32;
  if (!IsNativeMode() || !REXCVAR_GET(gta4_native_pixel_snap_fonts) || !rectangle) {
    __imp__sub_821F1670(ctx, base);
    return;
  }

  uint32_t width = LoadU32(base, kPrimaryVideoWidthGlobal);
  uint32_t height = LoadU32(base, kPrimaryVideoHeightGlobal);
  if (!width || !height) {
    width = LoadU32(base, kSecondaryVideoWidthGlobal);
    height = LoadU32(base, kSecondaryVideoHeightGlobal);
  }
  if (!width || !height) {
    __imp__sub_821F1670(ctx, base);
    return;
  }

  constexpr std::array<uint32_t, 4> kCoordinateOffsets = {0, 4, 8, 12};
  std::array<uint32_t, 4> original{};
  std::array<uint32_t, 4> snapped{};
  for (size_t index = 0; index < kCoordinateOffsets.size(); ++index) {
    const uint32_t offset = kCoordinateOffsets[index];
    original[index] = LoadU32(base, rectangle + offset);
    const uint32_t extent = (index & 1) == 0 ? width : height;
    StoreF32(base, rectangle + offset,
             SnapFontCoordinate(std::bit_cast<float>(original[index]), extent));
    snapped[index] = LoadU32(base, rectangle + offset);
  }

  __imp__sub_821F1670(ctx, base);

  if (rex::diagnostics::IsEnabled(rex::diagnostics::Category::kNativeTrace) &&
      g_vector_font_rectangle_trace_count.load(std::memory_order_relaxed) < 32 &&
      REXCVAR_QUERY(bool, gta4_trace_vector_fonts)) {
    const uint64_t trace = ++g_vector_font_rectangle_trace_count;
    if (trace <= 32) {
      REXLOG_INFO(
          "gta4-native-font-debug: rectangle #{} object={:08X} lr={:08X} extent={}x{} "
          "input={:08X},{:08X},{:08X},{:08X} snapped={:08X},{:08X},{:08X},{:08X} "
          "retail-output={:08X},{:08X},{:08X},{:08X}",
          trace, rectangle, uint32_t(ctx.lr), width, height, original[0], original[1], original[2],
          original[3], snapped[0], snapped[1], snapped[2], snapped[3], LoadU32(base, rectangle),
          LoadU32(base, rectangle + 4), LoadU32(base, rectangle + 8),
          LoadU32(base, rectangle + 12));
    }
  }

  for (size_t index = 0; index < kCoordinateOffsets.size(); ++index) {
    StoreU32(base, rectangle + kCoordinateOffsets[index], original[index]);
  }
}

extern "C" void sub_82270A08(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode() || ctx.r3.u32 != kOriginalShadowMapBaseSize) {
    __imp__sub_82270A08(ctx, base);
    return;
  }

  const uint32_t configured_base_size = REXCVAR_GET(gta4_shadow_map_base_size);
  uint32_t effective_base_size = configured_base_size;
  DeviceCapabilitiesResult capabilities{};
  if (QueryNativeDeviceCapabilities(capabilities)) {
    const uint32_t maximum_base_size =
        capabilities.max_image_dimension_2d / kPointShadowCacheBaseMultiplier;
    effective_base_size = std::min(effective_base_size, maximum_base_size);
    effective_base_size = std::max(effective_base_size, kOriginalShadowMapBaseSize);
  } else {
    effective_base_size = kOriginalShadowMapBaseSize;
    REXLOG_WARN(
        "gta4-native-quality: unable to query Vulkan image limits; using stock shadow base {}",
        effective_base_size);
  }
  ctx.r3.u32 = effective_base_size;

  static std::atomic<uint64_t> override_count{0};
  const uint64_t override = NextNativeHookDiagnosticCall(override_count);
  if (ShouldLogNativeHookCall(override)) {
    REXLOG_INFO(
        "gta4-native-quality: shadow map base #{} stock={} configured={} effective={} "
        "maxImageDimension2D={}",
        override, kOriginalShadowMapBaseSize, configured_base_size, effective_base_size,
        capabilities.max_image_dimension_2d);
  }

  __imp__sub_82270A08(ctx, base);
  ApplyShadowDistanceScale(base);
}

extern "C" void sub_824F3418(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode() || !REXCVAR_GET(gta4_force_highest_lod) || !ctx.r4.u32) {
    __imp__sub_824F3418(ctx, base);
    return;
  }

  const uint8_t previous_force = LoadU8(base, kForceHighLodGlobal);
  const bool highest_lod_resident =
      LoadU32(base, ctx.r4.u32 + kHighestLodDrawableOffset) != 0;
  if (highest_lod_resident) {
    StoreU8(base, kForceHighLodGlobal, 1);
  }

  __imp__sub_824F3418(ctx, base);
  StoreU8(base, kForceHighLodGlobal, previous_force);

  if (highest_lod_resident) {
    static std::atomic<uint64_t> forced_count{0};
    const uint64_t forced = NextNativeHookDiagnosticCall(forced_count);
    if (ShouldLogNativeHookCall(forced)) {
      REXLOG_INFO("gta4-native-quality: forced highest resident LOD #{} drawable={:08X}",
                  forced, ctx.r4.u32);
    }
  } else {
    static std::atomic<uint64_t> fallback_count{0};
    const uint64_t fallback = NextNativeHookDiagnosticCall(fallback_count);
    if (ShouldLogNativeHookCall(fallback)) {
      REXLOG_INFO(
          "gta4-native-quality: highest LOD unavailable #{} drawable={:08X}; "
          "preserving the game's resident fallback",
          fallback, ctx.r4.u32);
    }
  }
}

extern "C" void sub_821DFFE8(PPCContext& ctx, uint8_t* base) {
  const double configured_scale = REXCVAR_GET(gta4_draw_distance_scale);
  if (!IsNativeMode()) {
    __imp__sub_821DFFE8(ctx, base);
    return;
  }

  ApplyModelBudgetOverrides(base);
  if (configured_scale <= 1.0) {
    __imp__sub_821DFFE8(ctx, base);
    return;
  }

  const uint32_t previous_scale_bits = LoadU32(base, kDistanceScaleInputGlobal);
  const float previous_scale = std::bit_cast<float>(previous_scale_bits);
  const float effective_scale = previous_scale * float(configured_scale);
  StoreU32(base, kDistanceScaleInputGlobal, std::bit_cast<uint32_t>(effective_scale));
  __imp__sub_821DFFE8(ctx, base);
  StoreU32(base, kDistanceScaleInputGlobal, previous_scale_bits);

  static std::atomic<uint64_t> override_count{0};
  const uint64_t override = NextNativeHookDiagnosticCall(override_count);
  if (ShouldLogNativeHookCall(override)) {
    REXLOG_INFO(
        "gta4-native-quality: draw distance #{} game={} multiplier={} effective={} "
        "published={}",
        override, previous_scale, configured_scale, effective_scale,
        std::bit_cast<float>(LoadU32(base, kDistanceScaleOutputGlobal)));
  }
}

extern "C" void sub_82586E70(PPCContext& ctx, uint8_t* base) {
  __imp__sub_82586E70(ctx, base);
  if (!IsNativeMode() || !REXCVAR_GET(gta4_disable_model_budget_reduction)) {
    return;
  }
  StoreU8(base, kReducePedModelBudgetGlobal, 0);
  static std::atomic<uint64_t> override_count{0};
  const uint64_t override = NextNativeHookDiagnosticCall(override_count);
  if (ShouldLogNativeHookCall(override)) {
    REXLOG_INFO("gta4-native-quality: disabled pedestrian model-budget reduction #{}",
                override);
  }
}

extern "C" void sub_82586E98(PPCContext& ctx, uint8_t* base) {
  __imp__sub_82586E98(ctx, base);
  if (!IsNativeMode() || !REXCVAR_GET(gta4_disable_model_budget_reduction)) {
    return;
  }
  StoreU8(base, kReduceVehicleModelBudgetGlobal, 0);
  static std::atomic<uint64_t> override_count{0};
  const uint64_t override = NextNativeHookDiagnosticCall(override_count);
  if (ShouldLogNativeHookCall(override)) {
    REXLOG_INFO("gta4-native-quality: disabled vehicle model-budget reduction #{}",
                override);
  }
}

extern "C" void sub_8223C288(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode() || !REXCVAR_GET(gta4_disable_timecycle_far_clip)) {
    __imp__sub_8223C288(ctx, base);
    return;
  }

  const uint8_t previous_disable = LoadU8(base, kDisableTimecycleFarClipGlobal);
  StoreU8(base, kDisableTimecycleFarClipGlobal, 1);
  __imp__sub_8223C288(ctx, base);
  StoreU8(base, kDisableTimecycleFarClipGlobal, previous_disable);

  static std::atomic<uint64_t> override_count{0};
  const uint64_t override = NextNativeHookDiagnosticCall(override_count);
  if (ShouldLogNativeHookCall(override)) {
    REXLOG_INFO("gta4-native-quality: suppressed timecycle far clip #{}", override);
  }
}

extern "C" void sub_8247E4C0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_8247E4C0(ctx, base);
    return;
  }

  const uint32_t configured_limit = REXCVAR_GET(gta4_drawable_reference_limit);
  ctx.r4.u32 = configured_limit;

  static std::atomic<uint64_t> override_count{0};
  const uint64_t override = NextNativeHookDiagnosticCall(override_count);
  if (ShouldLogNativeHookCall(override)) {
    REXLOG_INFO("gta4-native-quality: drawable reference limit #{} {} -> {}", override,
                kOriginalDrawableReferenceLimit, configured_limit);
  }

  __imp__sub_8247E3B8(ctx, base);
}

extern "C" void sub_828C01E0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_828C01E0(ctx, base);
    return;
  }

  if (g_active_native_capture.active) {
    REXLOG_WARN(
        "gta4-native-cache: replacing unfinished build object={:08X} commands={}",
        g_active_native_capture.build_object, g_active_native_capture.commands.size());
  }
  g_active_native_capture = {};
  g_active_native_capture.active = true;
  g_active_native_capture.base = base;
  g_active_native_capture.build_object = ctx.r3.u32;
  __imp__sub_828C01E0(ctx, base);
}

extern "C" void sub_828BF880(PPCContext& ctx, uint8_t* base) {
  if (IsNativeMode() && g_active_native_capture.active) {
    g_active_native_capture.selector_mask = ctx.r4.u32;
  }
  __imp__sub_828BF880(ctx, base);
}

extern "C" void sub_828C0338(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode() || !g_active_native_capture.active) {
    __imp__sub_828C0338(ctx, base);
    return;
  }

  const uint32_t output = ctx.r3.u32;
  const uint32_t build_object = g_active_native_capture.build_object;
  __imp__sub_828C0338(ctx, base);

  const uint32_t command_list = output ? LoadU32(base, output) : 0;
  std::vector<CapturedNativeCommand> commands = std::move(g_active_native_capture.commands);
  g_active_native_capture = {};

  if (command_list) {
    const size_t command_count = commands.size();
    PublishCachedNativeCommandList(command_list, std::move(commands));
    static std::atomic<uint64_t> publish_count{0};
    const uint64_t publish = NextNativeHookDiagnosticCall(publish_count);
    if (ShouldLogNativeHookCall(publish)) {
      REXLOG_INFO(
          "gta4-native-cache: publish #{} build={:08X} command-list={:08X} commands={}",
          publish, build_object, command_list, command_count);
    }
  } else {
    REXLOG_WARN("gta4-native-cache: build={:08X} finalized without command-list; commands={}",
                build_object, commands.size());
  }
}

extern "C" void sub_82A47E28(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A47E28(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  const uint32_t command_list = ctx.r4.u32;
  const uint32_t replay_mask = ctx.r5.u32;
  if (!device || !command_list) {
    return;
  }

  const uint32_t live_fence = LoadU32(base, device + kLiveResourceFenceOffset);
  StoreU32(base, command_list + kResourceFenceOffset, live_fence);
  const uint32_t selector_tree = LoadU32(base, command_list + 112);
  if (selector_tree) {
    InvokeGuest(ctx, base, __imp__sub_82A46EA8, live_fence, selector_tree, replay_mask);
  }

  StoreU64(base, device + 32, ~LoadU64(base, command_list + 96));
  StoreU64(base, device, ~LoadU64(base, command_list + 64));
  StoreU64(base, device + 8, ~LoadU64(base, command_list + 72));
  StoreU64(base, device + 16, ~LoadU64(base, command_list + 80));
  StoreU64(base, device + 24, ~LoadU64(base, command_list + 88));

  uint64_t selected_count = 0;
  uint64_t submitted_count = 0;
  const bool found = ReplayCachedNativeCommandList(ctx, base, device, command_list, replay_mask,
                                                   selected_count, submitted_count);
  static std::atomic<uint64_t> replay_count{0};
  const uint64_t replay = NextNativeHookDiagnosticCall(replay_count);
  if (ShouldLogNativeHookCall(replay) || !found || selected_count != submitted_count) {
    REXLOG_INFO(
        "gta4-native-cache: replay #{} command-list={:08X} device={:08X} mask={:08X} "
        "found={} selected={} submitted={} capture={}",
        replay, command_list, device, replay_mask, found, selected_count, submitted_count,
        g_active_native_capture.active);
  }
}

extern "C" void CCommandBuffer_Destroy(PPCContext& ctx, uint8_t* base) {
  if (IsNativeMode()) {
    EraseCachedNativeCommandList(ctx.r3.u32);
  }
  __imp__CCommandBuffer_Destroy(ctx, base);
}

extern "C" void sub_82A41520(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A41520(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  StoreU8(base, device + 10941, LoadU8(base, device + 10941) & uint8_t(0xFB));
}

extern "C" void sub_82A416B8(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A416B8(ctx, base);
    return;
  }

  const uint32_t mode = ctx.r4.u32;
  uint32_t behavior = ctx.r6.u32;
  const uint32_t presentation = ctx.r7.u32;
  const uint32_t output = ctx.r8.u32;
  if (output) {
    StoreU32(base, output, 0);
  }

  const uint32_t device = InvokeGuest(ctx, base, D3D_MemAllocAligned, kGuestDeviceSize, 128).r3.u32;
  if (!device) {
    ctx.r3.u32 = kOutOfMemory;
    return;
  }

  if (!InvokeGuest(ctx, base, sub_82A50820, device).r3.u32) {
    FreeNativeDeviceAllocation(ctx, base, device);
    ctx.r3.u32 = kOutOfMemory;
    return;
  }
  InitializeNativeCommandScratch(base, device);

  bool initialized = false;
  if (mode == 2) {
    initialized = InitializeSecondaryCpuState(ctx, base, device);
  } else {
    if (!(behavior & 0x100) && !(behavior & 0x3F000000)) {
      behavior |= 0x0C000000;
    }
    StoreU32(base, device + 22280, behavior);
    initialized = presentation && InitializePrimaryCpuState(ctx, base, device, presentation);
  }

  if (!initialized) {
    FreeNativeDeviceAllocation(ctx, base, device);
    ctx.r3.u32 = kOutOfMemory;
    return;
  }

  if (output) {
    StoreU32(base, output, device);
  }
  DeviceCommand command{{sizeof(DeviceCommand), CommandType::kDeviceCreated}, device, mode};
  SubmitNativeCommand(command);
  ctx.r3.u32 = 0;
}

extern "C" void sub_82A4F7E0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A4F7E0(ctx, base);
    return;
  }
  ApplyDeviceCpuPrefix(base, ctx.r3.u32);
}

extern "C" void sub_82A3BAA8(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3BAA8(ctx, base);
    return;
  }
  StoreAllDirty(base, ctx.r3.u32);
}

extern "C" void sub_82A3F268(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3F268(ctx, base);
    return;
  }
  const uint32_t device = ctx.r3.u32;
  uint32_t value = LoadU32(base, device + 10916);
  value = (value & ~uint32_t(0x3000)) | (std::rotl(ctx.r4.u32, 12) & 0x3000);
  StoreU32(base, device + 10916, value);
}

extern "C" void sub_82A42AC0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A42AC0(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  uint32_t first = ctx.r5.u32;
  uint32_t second = ctx.r6.u32;
  if (!first && !second) {
    first = 64;
    second = 64;
  }

  uint32_t value = 0;
  if (!(ctx.r4.u32 & 1)) {
    second = (second & ~uint32_t(0x7F00)) | (std::rotl(first, 8) & 0x7F00);
    value = LoadU32(base, device + 10920);
    value = (value & ~uint32_t(0x7F0)) | (std::rotl(second, 4) & 0x7F0);
    value = (value & ~uint32_t(0x7F000)) | (std::rotl(second, 4) & 0x7F000);
  }
  StoreU32(base, device + 10920, value);
  ApplyNativeDirtyDefaults(base, device);
}

extern "C" void sub_82A3B540(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3B540(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  const uint32_t rectangle = ctx.r4.u32;
  const int32_t viewport_x =
      TruncateViewportCoordinate(std::bit_cast<float>(LoadU32(base, device + 12640)));
  const int32_t viewport_y =
      TruncateViewportCoordinate(std::bit_cast<float>(LoadU32(base, device + 12644)));
  const int32_t viewport_width =
      TruncateViewportCoordinate(std::bit_cast<float>(LoadU32(base, device + 12648)));
  const int32_t viewport_height =
      TruncateViewportCoordinate(std::bit_cast<float>(LoadU32(base, device + 12652)));

  const int32_t left = int32_t(LoadU32(base, rectangle));
  const int32_t top = int32_t(LoadU32(base, rectangle + 4));
  const int32_t right = int32_t(LoadU32(base, rectangle + 8));
  const int32_t bottom = int32_t(LoadU32(base, rectangle + 12));
  StoreU32(base, device + 12668, uint32_t(left));
  StoreU32(base, device + 12672, uint32_t(top));
  StoreU32(base, device + 12676, uint32_t(right));
  StoreU32(base, device + 12680, uint32_t(bottom));

  int32_t clipped_left = viewport_x;
  int32_t clipped_top = viewport_y;
  int32_t clipped_right = std::bit_cast<int32_t>(uint32_t(viewport_x) + uint32_t(viewport_width));
  int32_t clipped_bottom = std::bit_cast<int32_t>(uint32_t(viewport_y) + uint32_t(viewport_height));
  if (int32_t(LoadU32(base, device + 11848))) {
    clipped_left = std::max(clipped_left, left);
    clipped_top = std::max(clipped_top, top);
    clipped_right = std::min(clipped_right, right);
    clipped_bottom = std::min(clipped_bottom, bottom);
  }

  uint32_t packed_top_left = LoadU32(base, device + 10436);
  packed_top_left = (packed_top_left & 0x8000FFFF) | ((uint32_t(clipped_top) << 16) & 0x7FFF0000);
  packed_top_left = (packed_top_left & 0xFFFF8000) | (uint32_t(clipped_left) & 0x00007FFF);
  uint32_t packed_bottom_right = LoadU32(base, device + 10440);
  packed_bottom_right =
      (packed_bottom_right & 0x8000FFFF) | ((uint32_t(clipped_bottom) << 16) & 0x7FFF0000);
  packed_bottom_right = (packed_bottom_right & 0xFFFF8000) | (uint32_t(clipped_right) & 0x00007FFF);
  StoreU32(base, device + 10436, packed_top_left);
  StoreU32(base, device + 10440, packed_bottom_right);
}

extern "C" void sub_82A3BF50(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3BF50(ctx, base);
    return;
  }
  const uint32_t device = ctx.r3.u32;
  const uint32_t index = ctx.r4.u32;
  const uint32_t surface = ctx.r5.u32;
  __imp__sub_82A3BF50(ctx, base);
  SetRenderTargetCommand command;
  command.device = device;
  command.index = index;
  command.surface = CaptureSurfaceDescriptor(base, surface);
  SubmitNativeCommand(command);
}

extern "C" void sub_828DA250(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_828DA250(ctx, base);
    return;
  }

  const uint32_t owner = ctx.r3.u32;
  const uint32_t slot = ctx.r4.u32;
  const uint32_t color_wrapper = ctx.r5.u32;
  const uint32_t depth_wrapper = ctx.r6.u32;
  const uint32_t subresource = ctx.r7.u32;
  const bool depth_enabled = ctx.r8.u8 != 0;
  const uint32_t caller = uint32_t(ctx.lr);
  const uint32_t color_surface =
      color_wrapper ? LoadU32(base, color_wrapper + kDeferredWrapperSurfaceOffset) : 0;
  const uint32_t depth_surface =
      depth_wrapper && depth_enabled
          ? LoadU32(base, depth_wrapper + kDeferredWrapperSurfaceOffset)
          : 0;

  __imp__sub_828DA250(ctx, base);

  if (rex::diagnostics::IsEnabled(rex::diagnostics::Category::kGuestHooks) && slot == 0 &&
      (depth_wrapper != g_last_title_depth_wrapper ||
       depth_surface != g_last_title_depth_surface)) {
    static std::atomic<uint64_t> transition_count{0};
    const uint64_t transition = NextNativeHookDiagnosticCall(transition_count);
    REXLOG_INFO(
        "gta4-native-architecture: point=title-target-bind transition={} caller={:08X} "
        "owner={:08X} slot={} color-wrapper={:08X} color-role={} color-surface={:08X} "
        "depth-enabled={} depth-wrapper={:08X} depth-role={} depth-surface={:08X} "
        "subresource={} retained-depth-wrapper={:08X}",
        transition, caller, owner, slot, color_wrapper,
        IdentifyDeferredWrapperRole(base, color_wrapper), color_surface, depth_enabled,
        depth_wrapper, IdentifyDeferredWrapperRole(base, depth_wrapper), depth_surface,
        subresource, LoadU32(base, owner + 56));
    g_last_title_depth_wrapper = depth_wrapper;
    g_last_title_depth_surface = depth_surface;
  }
}

extern "C" void sub_82A3C2B8(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3C2B8(ctx, base);
    return;
  }
  const uint32_t device = ctx.r3.u32;
  const uint32_t surface = ctx.r4.u32;
  __imp__sub_82A3C2B8(ctx, base);
  SetDepthStencilCommand command;
  command.device = device;
  command.surface = CaptureSurfaceDescriptor(base, surface);
  command.trace_wrapper = surface;
  command.trace_caller = uint32_t(ctx.lr);
  SubmitNativeCommand(command);
}

extern "C" void sub_82A3B690(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3B690(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  LogKnownOffscreenTransformBoundary(
      "offscreen-boundary-vertex-stream", ctx, base, ctx.r5.u32, device);
  const uint32_t stream = ctx.r4.u32;
  const uint32_t buffer = ctx.r5.u32;
  const uint32_t offset = ctx.r6.u32;
  const uint32_t stride = ctx.r7.u32;
  const uint64_t dirty_mask = ctx.r8.u64;
  if (stream >= kVertexStreamCount) {
    return;
  }

  if (buffer) {
    const uint32_t resource_address = LoadU32(base, buffer + 24) + offset;
    const uint32_t full_resource_size = LoadU32(base, buffer + 28) & kResourceSizeMask;
    const uint32_t resource_size = offset < full_resource_size ? full_resource_size - offset : 0;
    StoreU32(base, device + kStreamSizeBase - stream * 8, resource_size);
    StoreU32(base, device + kStreamFetchBase - stream * 8,
             EncodeNativeFetchAddress(resource_address));
    StoreU64(base, device + 24, LoadU64(base, device + 24) | dirty_mask);
  }

  const uint32_t slot = device + kStreamBufferBase + stream * 4;
  RetireNativeBoundResource(base, device, LoadU32(base, slot));
  StoreU32(base, slot, buffer);
  const uint32_t stride_words = (stride >> 2) & 0xFF;
  StoreU8(base, device + 12520 + stream, uint8_t(stride_words));
  if (stride_words && stride_words != LoadU8(base, device + 11824 + stream)) {
    StoreU64(base, device + 16, LoadU64(base, device + 16) | uint64_t(0x00080000));
  }

  SetVertexStreamCommand command;
  command.device = device;
  command.stream = stream;
  command.buffer = buffer;
  command.offset = offset;
  command.stride = stride;
  command.stride_words = stride_words;
  SubmitNativeCommand(command);
}

extern "C" void sub_82A3B7B0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3B7B0(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  LogKnownOffscreenTransformBoundary(
      "offscreen-boundary-index-buffer", ctx, base, ctx.r4.u32, device);
  const uint32_t buffer = ctx.r4.u32;
  const uint32_t slot = device + kIndexBufferOffset;
  RetireNativeBoundResource(base, device, LoadU32(base, slot));
  StoreU32(base, slot, buffer);

  SetIndexBufferCommand command;
  command.device = device;
  command.buffer = buffer;
  SubmitNativeCommand(command);
}

extern "C" void sub_82A44168(PPCContext& ctx, uint8_t* base) {
  const uint32_t texture = ctx.r3.u32;
  if (IsNativeMode() && texture && ctx.r8.u32 == 0) {
    TextureLockCommand command;
    command.texture = texture;
    command.array_index = ctx.r4.u32;
    command.level = ctx.r5.u32;
    command.flags = ctx.r8.u32;
    command.dimension = TextureLockDimension::k2D;
    TextureLockResult result{};
    ExecuteNativeCommand(command, result);
  }
  __imp__sub_82A44168(ctx, base);
}

extern "C" void sub_82A441F8(PPCContext& ctx, uint8_t* base) {
  const uint32_t texture = ctx.r3.u32;
  if (IsNativeMode() && texture && ctx.r7.u32 == 0) {
    TextureLockCommand command;
    command.texture = texture;
    command.array_index = 0;
    command.level = ctx.r4.u32;
    command.flags = ctx.r7.u32;
    command.dimension = TextureLockDimension::k3D;
    TextureLockResult result{};
    ExecuteNativeCommand(command, result);
  }
  __imp__sub_82A441F8(ctx, base);
}

extern "C" void D3DResource_Release(PPCContext& ctx, uint8_t* base) {
  const bool native_mode = IsNativeMode();
  const uint32_t resource = ctx.r3.u32;
  __imp__D3DResource_Release(ctx, base);

  // The generated implementation returns zero only after the guest reference
  // count reaches zero and the resource destructor has run. Notify the native
  // renderer at that exact ownership boundary. Previously its handle caches
  // retained the final shared_ptr forever, which also kept the corresponding
  // MoltenVK image and device memory alive.
  if (native_mode && resource && ctx.r3.u32 == 0) {
    ReleaseResourceCommand command;
    command.resource = resource;
    SubmitNativeCommand(command);
  }
}

extern "C" void sub_82A4A3C8(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A4A3C8(ctx, base);
    return;
  }

  const uint32_t resource = ctx.r3.u32;
  if (!resource) {
    ctx.r3.u32 = 0;
    return;
  }
  UpdateResourceLockCount(base, resource, 256);
  ctx.r3.u32 = ctx.r8.u32;
}

extern "C" void sub_82A4A600(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A4A600(ctx, base);
    return;
  }

  const uint32_t resource = ctx.r3.u32;
  if (!resource) {
    return;
  }
  const uint32_t previous = UpdateResourceLockCount(base, resource, -256);
  if ((previous & 0xF00) == 0x100) {
    if (LoadU32(base, resource + 20) != 0xFFFF0000) {
      StoreU32(base, resource + 20, 0xFFFF0000);
    }
    if (ctx.r5.u32 && LoadU32(base, resource + 24) != 0xFFFF0000) {
      StoreU32(base, resource + 24, 0xFFFF0000);
    }
  }

  InvalidateTextureCommand command;
  command.texture = resource;
  SubmitNativeCommand(command);
}

extern "C" void sub_828E0048(PPCContext& ctx, uint8_t* base) {
  const uint32_t resource_owner = ctx.r4.u32;
  const uint32_t logical_font_id =
      IsNativeMode() ? IdentifyVectorFontOwner(base, resource_owner) : 0;
  ScopedVectorFontBinding binding(logical_font_id, resource_owner, ctx.r3.u32);

  if (rex::diagnostics::IsEnabled(rex::diagnostics::Category::kNativeTrace) && logical_font_id &&
      g_vector_font_owner_trace_count.load(std::memory_order_relaxed) < 64 &&
      REXCVAR_QUERY(bool, gta4_trace_vector_fonts)) {
    const uint64_t trace = ++g_vector_font_owner_trace_count;
    if (trace <= 64) {
      REXLOG_INFO(
          "gta4-native-font-debug: owner-bind #{} font={} outer-stage={} owner={:08X} "
          "lr={:08X} live-slots=font1:{:08X},font3:{:08X},font2:{:08X}",
          trace, logical_font_id, ctx.r3.u32, resource_owner, uint32_t(ctx.lr),
          LoadU32(base, kVectorFontOwnerBindings[0].owner_slot),
          LoadU32(base, kVectorFontOwnerBindings[1].owner_slot),
          LoadU32(base, kVectorFontOwnerBindings[2].owner_slot));
    }
  }

  // Preserve the complete retail binding path, including the virtual resource
  // accessor that converts the owner into the inner D3D texture handle.
  __imp__sub_828E0048(ctx, base);
}

extern "C" void sub_82A44B78(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A44B78(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  const uint32_t stage = ctx.r4.u32;
  const uint32_t texture = ctx.r5.u32;
  const uint64_t dirty_mask = ctx.r6.u64;
  if (stage >= kTextureStageCount) {
    return;
  }

  const uint32_t slot = device + kTextureHandleBase + stage * 4;
  const uint32_t previous = LoadU32(base, slot);
  std::array<uint32_t, 6> source_descriptor{};
  std::array<uint32_t, 6> final_fetch{};
  if (texture) {
    const uint32_t fetch = device + kTextureFetchBase + stage * 24;
    for (size_t index = 0; index < source_descriptor.size(); ++index) {
      source_descriptor[index] = LoadU32(base, texture + 28 + uint32_t(index) * 4);
    }
    uint32_t word0 = source_descriptor[0];
    uint32_t word4 = EncodeNativeFetchAddress(source_descriptor[1]);
    const uint32_t word8 = source_descriptor[2];
    uint32_t word12 = source_descriptor[3];
    uint32_t word16 = source_descriptor[4];
    uint32_t word20 = EncodeNativeFetchAddress(source_descriptor[5]);
    word0 = (word0 & 0xFFC003FF) | (LoadU32(base, fetch) & 0x003FFC00);
    word4 = (word4 & 0xFFFFF7FF) | (LoadU32(base, fetch + 4) & 0x00000800);
    word12 = (word12 & 0x8007FFFF) | (LoadU32(base, fetch + 12) & 0x7FF80000);
    word16 = (word16 & 0x000003FC) | (LoadU32(base, fetch + 16) & 0xFFFFFC03);

    uint32_t lower_mip = std::rotl(LoadU32(base, texture + 44), 30) & 0xF;
    lower_mip = std::max<uint32_t>(lower_mip, LoadU8(base, device + 11942 + stage));
    word16 = (word16 & 0xFFFFFFC3) | ((lower_mip << 2) & 0x3C);
    uint32_t upper_mip = std::rotl(LoadU32(base, texture + 44), 26) & 0xF;
    upper_mip = std::min<uint32_t>(upper_mip, LoadU8(base, device + 11968 + stage));
    word16 = (word16 & 0xFFFFFC3F) | ((upper_mip << 6) & 0x3C0);
    word20 = (word20 & 0xFFFFFE00) | (LoadU32(base, fetch + 20) & 0x000001FF);

    StoreU32(base, fetch, word0);
    StoreU32(base, fetch + 4, word4);
    StoreU32(base, fetch + 8, word8);
    StoreU32(base, fetch + 12, word12);
    StoreU32(base, fetch + 16, word16);
    StoreU32(base, fetch + 20, word20);
    final_fetch = {word0, word4, word8, word12, word16, word20};
    StoreU64(base, device + 24, LoadU64(base, device + 24) | dirty_mask);
  }

  if (rex::diagnostics::IsEnabled(rex::diagnostics::Category::kNativeTrace) &&
      g_vector_font_id &&
      g_vector_font_texture_trace_count.load(std::memory_order_relaxed) < 64 &&
      REXCVAR_QUERY(bool, gta4_trace_vector_fonts)) {
    const uint64_t trace = ++g_vector_font_texture_trace_count;
    if (trace <= 64) {
      REXLOG_INFO(
          "gta4-native-font-debug: texture-bind #{} font={} owner={:08X} "
          "outer-stage={} device={:08X} stage={} texture={:08X} previous={:08X} "
          "descriptor={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} "
          "fetch={:08X},{:08X},{:08X},{:08X},{:08X},{:08X} dirty={:016X}",
          trace, g_vector_font_id, g_vector_font_owner, g_vector_font_outer_stage, device, stage,
          texture, previous, source_descriptor[0], source_descriptor[1], source_descriptor[2],
          source_descriptor[3], source_descriptor[4], source_descriptor[5], final_fetch[0],
          final_fetch[1], final_fetch[2], final_fetch[3], final_fetch[4], final_fetch[5],
          dirty_mask);
    }
  }

  StoreU32(base, slot, texture);
  RetireNativeBoundResource(base, device, previous);
  SetTextureCommand command;
  command.device = device;
  command.stage = stage;
  command.texture = texture;
  command.vector_font_id = g_vector_font_id;
  SubmitNativeCommand(command);
}

extern "C" void sub_82A46FB0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A46FB0(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  const uint32_t scratch = LoadU32(base, device + 16712);
  if (!scratch) {
    ctx.r3.u32 = 0;
    return;
  }
  StoreU32(base, device + kCommandScratchWriteOffset, scratch);
  StoreU32(base, device + kCommandScratchEndOffset, scratch + kCommandScratchSize);
  ctx.r3.u32 = scratch;
}

extern "C" void sub_82A424A8(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A424A8(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  const uint32_t shader = ctx.r4.u32;
  ApplyNativeShaderState(ctx, base, __imp__sub_82A424A8);
  TrackNativeShaderBinding(device, ShaderStage::kPixel, shader);
  SetShaderCommand command{
      {sizeof(SetShaderCommand), CommandType::kSetPixelShader}, device, shader};
  SubmitNativeCommand(command);
}

extern "C" void sub_82A42BA8(PPCContext& ctx, uint8_t* base) {
  RegisterNativeShader(ctx, base, __imp__sub_82A42BA8, ShaderStage::kPixel);
}

extern "C" void sub_82A42CB8(PPCContext& ctx, uint8_t* base) {
  RegisterNativeShader(ctx, base, __imp__sub_82A42CB8, ShaderStage::kVertex);
}

extern "C" void sub_82A42760(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A42760(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  LogKnownOffscreenTransformBoundary(
      "offscreen-boundary-vertex-shader", ctx, base, ctx.r4.u32, device);
  const uint32_t shader = ctx.r4.u32;
  ApplyNativeShaderState(ctx, base, __imp__sub_82A42760);
  TrackNativeShaderBinding(device, ShaderStage::kVertex, shader);
  SetShaderCommand command{
      {sizeof(SetShaderCommand), CommandType::kSetVertexShader}, device, shader};
  SubmitNativeCommand(command);
}

extern "C" void sub_82A42930(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A42930(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  LogKnownOffscreenTransformBoundary(
      "offscreen-boundary-vertex-declaration", ctx, base, ctx.r4.u32,
      device);
  const uint32_t declaration = ctx.r4.u32;
  StoreU32(base, device + kVertexDeclarationOffset, declaration);
  StoreU64(base, device + 16, LoadU64(base, device + 16) | uint64_t(0x00080000));

  SubmitNativeVertexDeclaration(base, device, declaration);

  SetVertexDeclarationCommand command;
  command.device = device;
  command.declaration = declaration;
  SubmitNativeCommand(command);
}

extern "C" void sub_82A3A890(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3A890(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  const uint32_t declaration = ctx.r4.u32;
  StoreU32(base, device + kLegacyVertexDeclarationOffset, declaration);
  StoreU64(base, device + 16, LoadU64(base, device + 16) | uint64_t(0x4000000000));

  SubmitNativeVertexDeclaration(base, device, declaration);
  SetVertexDeclarationCommand command;
  command.device = device;
  command.declaration = declaration;
  SubmitNativeCommand(command);
}

extern "C" void sub_82A3DAB0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3DAB0(ctx, base);
    return;
  }

  const uint64_t call =
      g_indexed_draw_invocation_id.fetch_add(1, std::memory_order_relaxed) + 1;
  if (ShouldLogNativeHookCall(call)) {
    REXLOG_INFO(
        "gta4-native-hook: DrawPrimitiveUPBegin #{} lr={:08X} device={:08X} primitive={} "
        "vertices={} stride={}",
        call, ctx.lr, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
  }

  if (g_pending_draw_primitive_up.vertex_data) {
    FreeAlignedGuestAllocation(ctx, base, g_pending_draw_primitive_up.vertex_data);
    g_pending_draw_primitive_up = {};
  }

  const uint32_t device = ctx.r3.u32;
  const uint32_t primitive_type = ctx.r4.u32;
  const uint32_t vertex_count = ctx.r5.u32;
  const uint32_t stride = ctx.r6.u32;
  const uint64_t vertex_data_size_64 = uint64_t(vertex_count) * uint64_t(stride);
  if (!vertex_data_size_64 || vertex_data_size_64 > UINT32_MAX) {
    ctx.r3.u32 = 0;
    return;
  }
  const uint32_t vertex_data_size = uint32_t(vertex_data_size_64);
  const uint32_t vertex_data =
      InvokeGuest(ctx, base, D3D_MemAllocAligned, vertex_data_size, 16).r3.u32;
  if (!vertex_data) {
    ctx.r3.u32 = 0;
    return;
  }

  ConsumeNativeDrawDirtyState(base, device);
  StoreU32(base, device + kUpCommandWriteOffset, LoadU32(base, device + 48));
  StoreU32(base, device + kUpVertexDataOffset, vertex_data);
  StoreU32(base, device + kUpVertexWordCountOffset, vertex_data_size >> 2);
  g_pending_draw_primitive_up = {device, primitive_type, vertex_count,
                                 stride, vertex_data,    vertex_data_size};
  ctx.r3.u32 = vertex_data;
}

extern "C" void sub_82A3DF50(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3DF50(ctx, base);
    return;
  }

  static std::atomic<uint64_t> call_count{0};
  const uint64_t call = NextNativeHookDiagnosticCall(call_count);
  if (ShouldLogNativeHookCall(call)) {
    REXLOG_INFO(
        "gta4-native-hook: DrawPrimitiveUPCommit #{} lr={:08X} device={:08X} "
        "pending-device={:08X} pending-data={:08X}",
        call, ctx.lr, ctx.r3.u32, g_pending_draw_primitive_up.device,
        g_pending_draw_primitive_up.vertex_data);
  }

  const uint32_t device = ctx.r3.u32;
  StoreU32(base, device + 48, LoadU32(base, device + kUpCommandWriteOffset));
  if (g_pending_draw_primitive_up.device != device || !g_pending_draw_primitive_up.vertex_data) {
    return;
  }

  DrawPrimitiveUpCommand command;
  command.device = device;
  command.primitive_type = g_pending_draw_primitive_up.primitive_type;
  command.vertex_count = g_pending_draw_primitive_up.vertex_count;
  command.stride = g_pending_draw_primitive_up.stride;
  command.vertex_data = g_pending_draw_primitive_up.vertex_data;
  command.vertex_data_size = g_pending_draw_primitive_up.vertex_data_size;
  SubmitNativeCommand(command);
  FreeAlignedGuestAllocation(ctx, base, g_pending_draw_primitive_up.vertex_data);
  g_pending_draw_primitive_up = {};
}

extern "C" void sub_82A3DF60(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3DF60(ctx, base);
    return;
  }

  static std::atomic<uint64_t> call_count{0};
  const uint64_t call = NextNativeHookDiagnosticCall(call_count);
  if (ShouldLogNativeHookCall(call)) {
    REXLOG_INFO(
        "gta4-native-hook: DrawPrimitive #{} lr={:08X} device={:08X} primitive={} "
        "start={} vertices={}",
        call, ctx.lr, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
  }

  const uint32_t device = ctx.r3.u32;
  DrawPrimitiveCommand command;
  command.device = device;
  command.primitive_type = ctx.r4.u32;
  command.start_vertex = ctx.r5.u32;
  command.vertex_count = ctx.r6.u32;
  ConsumeNativeDrawDirtyState(base, device);
  SubmitNativeCommand(command);
}

void SubmitNativeResolve(uint8_t* base, uint32_t device, uint32_t flags,
                         const ResolveRectangle* source_rectangle, uint32_t destination_texture,
                         const ResolvePoint* destination_point, uint32_t destination_level,
                         uint32_t destination_slice_or_face, const uint32_t* clear_color_bits,
                         double clear_depth, uint32_t clear_stencil, uint32_t parameters,
                         uint32_t trace_origin, uint32_t trace_caller,
                         uint32_t trace_owner, uint32_t trace_source_wrapper = 0) {
  if (!device || !destination_texture) {
    return;
  }

  ResolveCommand command;
  command.device = device;
  command.flags = flags;
  const uint32_t source_index = flags & 7;
  uint32_t source_surface = 0;
  if (source_index == 4) {
    source_surface = LoadU32(base, device + kDepthStencilOffset);
  } else if (source_index < kRenderTargetCount) {
    source_surface = LoadU32(base, device + (kRenderTargetBase + source_index) * sizeof(uint32_t));
  }
  command.source = CaptureSurfaceDescriptor(base, source_surface);
  command.flags = NormalizeResolveSampleFlags(command.flags, command.source.sample_type);

  command.source_rectangle_valid = source_rectangle != nullptr;
  if (source_rectangle) {
    command.source_rectangle = *source_rectangle;
  }

  command.destination_texture = destination_texture;
  command.trace_origin = trace_origin;
  command.trace_caller = trace_caller;
  command.trace_owner = trace_owner;
  command.trace_source_wrapper = trace_source_wrapper;
  for (uint32_t index = 0; index < 6; ++index) {
    command.destination_fetch[index] =
        LoadU32(base, destination_texture + 28 + index * sizeof(uint32_t));
  }
  if (rex::diagnostics::IsEnabled(rex::diagnostics::Category::kGuestHooks) &&
      (flags & 0x04000000u)) {
    std::fprintf(
        stderr,
        "[ResolveOriginTrace] origin=%s caller=%08X device=%08X flags=%08X "
        "source=%08X/%08X source-size=%ux%u destination=%08X "
        "fetch=%08X,%08X,%08X,%08X,%08X,%08X rect=%u:%d,%d,%d,%d "
        "point=%u:%d,%d level=%u slice=%u\n",
        trace_origin == 1 ? "direct" : trace_origin == 2 ? "batch" :
        trace_origin == 3 ? "deferred-full-frame" : "unknown",
        trace_caller, device, flags, command.source.handle, command.source.address,
        command.source.width, command.source.height, destination_texture,
        command.destination_fetch[0], command.destination_fetch[1], command.destination_fetch[2],
        command.destination_fetch[3], command.destination_fetch[4], command.destination_fetch[5],
        unsigned(source_rectangle != nullptr), source_rectangle ? source_rectangle->left : 0,
        source_rectangle ? source_rectangle->top : 0,
        source_rectangle ? source_rectangle->right : 0,
        source_rectangle ? source_rectangle->bottom : 0, unsigned(destination_point != nullptr),
        destination_point ? destination_point->x : 0, destination_point ? destination_point->y : 0,
        destination_level, destination_slice_or_face);
    std::fflush(stderr);
  }
  command.destination_point_valid = destination_point != nullptr;
  if (destination_point) {
    command.destination_point = *destination_point;
  }
  command.destination_level = destination_level;
  command.destination_slice_or_face = destination_slice_or_face;
  if (clear_color_bits) {
    for (uint32_t index = 0; index < 4; ++index) {
      command.clear_color_bits[index] = clear_color_bits[index];
    }
  } else {
    command.clear_color_bits[0] = std::bit_cast<uint32_t>(1.0f);
    command.clear_color_bits[1] = std::bit_cast<uint32_t>(1.0f);
    command.clear_color_bits[2] = std::bit_cast<uint32_t>(1.0f);
    command.clear_color_bits[3] = std::bit_cast<uint32_t>(1.0f);
  }
  command.clear_depth_bits = std::bit_cast<uint64_t>(clear_depth);
  command.clear_stencil = clear_stencil;
  if (parameters) {
    command.color_format = LoadU32(base, parameters);
    command.color_exp_bias = int32_t(LoadU32(base, parameters + 4));
    command.depth_format = LoadU32(base, parameters + 8);
  }
  SubmitNativeCommand(command);
  RetireNativeBoundResource(base, device, destination_texture);
}

ResolveRectangle LoadResolveRectangle(uint8_t* base, uint32_t address) {
  return {int32_t(LoadU32(base, address)), int32_t(LoadU32(base, address + 4)),
          int32_t(LoadU32(base, address + 8)), int32_t(LoadU32(base, address + 12))};
}

ResolvePoint LoadResolvePoint(uint8_t* base, uint32_t address) {
  return {int32_t(LoadU32(base, address)), int32_t(LoadU32(base, address + 4))};
}

void FinalizeNativeResolveBatchState(uint8_t* base, uint32_t device) {
  uint8_t control_flags = LoadU8(base, device + 10940) & uint8_t(0xDF);
  StoreU8(base, device + 10943, LoadU8(base, device + 10943) & uint8_t(0xCF));

  bool render_targets_compatible = false;
  if (!(control_flags & uint8_t(0x0C)) && !LoadU8(base, device + 12179)) {
    if (control_flags & uint8_t(0x10)) {
      render_targets_compatible = true;
    } else if (control_flags & uint8_t(0x20)) {
      render_targets_compatible = true;
      for (uint32_t index = 0; index < 5; ++index) {
        const uint32_t current = LoadU32(base, device + 12432 + index * sizeof(uint32_t));
        const uint32_t cached = LoadU32(base, device + 12720 + index * sizeof(uint32_t));
        if (current && current != cached) {
          render_targets_compatible = false;
          break;
        }
      }
    }
  }

  control_flags =
      uint8_t((control_flags & uint8_t(0xFE)) | uint8_t(render_targets_compatible ? 1 : 0));
  StoreU8(base, device + 10940, control_flags);
  StoreU32(base, device + 12716, 0);
  StoreU32(base, device + 12708, 0);
  StoreU32(base, device + 12704, 0);
  StoreU32(base, device + 10932, 0);
  StoreU32(base, device + 10936, 0);
  StoreU32(base, device + 12700, UINT32_MAX);
}

extern "C" void sub_82A44850(PPCContext& ctx, uint8_t* base) {
  const uint32_t requested_width = ctx.r3.u32;
  const uint32_t requested_height = ctx.r4.u32;
  const uint32_t return_address = ctx.lr;
  if (!ShouldUseNativeDisplayBacking(ctx, requested_width, requested_height)) {
    __imp__sub_82A44850(ctx, base);
    return;
  }

  ctx.r3.u32 = std::min(requested_width, kNativeBackingWidth);
  ctx.r4.u32 = std::min(requested_height, kNativeBackingHeight);
  __imp__sub_82A44850(ctx, base);
  PatchNativeDisplayResourceDimensions(base, ctx.r3.u32, requested_width, requested_height,
                                       return_address, "texture", true);
}

extern "C" void sub_82A44970(PPCContext& ctx, uint8_t* base) {
  const uint32_t requested_width = ctx.r3.u32;
  const uint32_t requested_height = ctx.r4.u32;
  const uint32_t return_address = ctx.lr;
  if (!ShouldUseNativeDisplayBacking(ctx, requested_width, requested_height)) {
    __imp__sub_82A44970(ctx, base);
    return;
  }

  ctx.r3.u32 = std::min(requested_width, kNativeBackingWidth);
  ctx.r4.u32 = std::min(requested_height, kNativeBackingHeight);
  __imp__sub_82A44970(ctx, base);
  PatchNativeDisplayResourceDimensions(base, ctx.r3.u32, requested_width, requested_height,
                                       return_address, "surface", false);
}

extern "C" void sub_828C0160(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode() || ctx.lr != kVideoGlobalsReadyReturnAddress) {
    __imp__sub_828C0160(ctx, base);
    return;
  }

  const uint32_t requested_width = LoadU32(base, kPrimaryVideoWidthGlobal);
  const uint32_t requested_height = LoadU32(base, kPrimaryVideoHeightGlobal);
  const NativeResolutionOverride resolution =
      GetNativeResolutionOverride(requested_width, requested_height);
  if (resolution.active()) {
    StoreU32(base, kPrimaryVideoWidthGlobal, resolution.width);
    StoreU32(base, kPrimaryVideoHeightGlobal, resolution.height);
    StoreU32(base, kSecondaryVideoWidthGlobal, resolution.width);
    StoreU32(base, kSecondaryVideoHeightGlobal, resolution.height);

    static std::atomic<uint64_t> override_count{0};
    const uint64_t override = NextNativeHookDiagnosticCall(override_count);
    if (ShouldLogNativeHookCall(override)) {
      REXLOG_INFO(
          "gta4-native-resolution: video globals #{} requested={}x{} configured={}x{} "
          "display={}x{} fsr1={} explicit-width={} explicit-height={} automatic-display={}",
          override, requested_width, requested_height, resolution.width, resolution.height,
          resolution.display_width, resolution.display_height, resolution.fsr1_active,
          resolution.override_width && !resolution.automatic_display,
          resolution.override_height && !resolution.automatic_display,
          resolution.automatic_display);
    }
  }

  __imp__sub_828C0160(ctx, base);
}

extern "C" void sub_824F4730(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode() || !ctx.r3.u32 || ctx.lr != kDeferredResolutionSetupReturnAddress) {
    __imp__sub_824F4730(ctx, base);
    return;
  }

  const uint32_t mode = ctx.r3.u32;
  const uint32_t requested_width = ctx.r4.u32;
  const uint32_t requested_height = ctx.r5.u32;
  const NativeResolutionOverride resolution =
      GetNativeResolutionOverride(requested_width, requested_height);

  // Build the guest-owned wrappers and allocation identities at the API-valid
  // minimum. The generated setup otherwise allocates a page for every 32x32
  // block of the host resolution (30,081,024 bytes at 3456x2160) from the
  // 512 MiB physical heap. No deferred backing pixels are read in native mode;
  // the host Vulkan images are sized from the descriptors patched below.
  const uint32_t construction_width = kNativeBackingWidth;
  const uint32_t construction_height = kNativeBackingHeight;
  const uint32_t previous_target_width = g_native_deferred_target_width;
  const uint32_t previous_target_height = g_native_deferred_target_height;
  g_native_deferred_target_width = resolution.width;
  g_native_deferred_target_height = resolution.height;
  ctx.r4.u32 = construction_width;
  ctx.r5.u32 = construction_height;
  __imp__sub_824F4730(ctx, base);
  g_native_deferred_target_width = previous_target_width;
  g_native_deferred_target_height = previous_target_height;

  StoreU32(base, kDeferredWidthGlobal, resolution.width);
  StoreU32(base, kDeferredHeightGlobal, resolution.height);

  uint32_t patched_full_size_wrappers = 0;
  for (uint32_t wrapper_global : kDeferredFullSizeWrapperGlobals) {
    const uint32_t wrapper = LoadU32(base, wrapper_global);
    if (PatchDeferredWrapperDimensions(base, wrapper, resolution.width, resolution.height)) {
      ++patched_full_size_wrappers;
    }
  }

  const uint32_t hiz_width = (resolution.width + 1) / 2;
  const uint32_t hiz_height = (resolution.height + 1) / 2;
  const uint32_t hiz_wrapper = LoadU32(base, kDeferredHizRestoreWrapperGlobal);
  const bool patched_hiz =
      PatchDeferredWrapperDimensions(base, hiz_wrapper, hiz_width, hiz_height);

  static std::atomic<uint64_t> override_count{0};
  const uint64_t override_index = NextNativeHookDiagnosticCall(override_count);
  if (ShouldLogNativeHookCall(override_index)) {
    REXLOG_INFO(
        "gta4-native-resolution: graph setup #{} mode={} requested={}x{} construction={}x{} "
        "target={}x{} display={}x{} fsr1={} full-wrappers={}/{} hiz={}x{} "
        "hiz-patched={} explicit-width={} "
        "explicit-height={} automatic-display={}",
        override_index, mode, requested_width, requested_height, construction_width,
        construction_height, resolution.width, resolution.height, resolution.display_width,
        resolution.display_height, resolution.fsr1_active, patched_full_size_wrappers,
        kDeferredFullSizeWrapperGlobals.size(), hiz_width, hiz_height, patched_hiz,
        resolution.override_width && !resolution.automatic_display,
        resolution.override_height && !resolution.automatic_display,
        resolution.automatic_display);
  }
  if (patched_full_size_wrappers != kDeferredFullSizeWrapperGlobals.size() || !patched_hiz) {
    REXLOG_ERROR(
        "gta4-native-resolution: incomplete deferred descriptor normalization "
        "full-wrappers={}/{} hiz-wrapper={:08X} hiz-patched={}",
        patched_full_size_wrappers, kDeferredFullSizeWrapperGlobals.size(), hiz_wrapper,
        patched_hiz);
  }
}

extern "C" void sub_828DC7F0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_828DC7F0(ctx, base);
    return;
  }

  const uint32_t name = ctx.r4.u32;
  const uint32_t requested_width = ctx.r6.u32;
  const uint32_t requested_height = ctx.r7.u32;
  ReflectionResourceName reflection{};
  if (GetReflectionResourceName(base, name, reflection)) {
    if (requested_width != reflection.logical_width ||
        requested_height != reflection.logical_height) {
      REXLOG_ERROR(
          "gta4-native-reflection: rejected unexpected logical size name={} requested={}x{} "
          "expected={}x{}",
          reflection.name, requested_width, requested_height, reflection.logical_width,
          reflection.logical_height);
      __imp__sub_828DC7F0(ctx, base);
      return;
    }

    __imp__sub_828DC7F0(ctx, base);
    const uint32_t wrapper = ctx.r3.u32;
    if (!wrapper) {
      REXLOG_ERROR("gta4-native-reflection: allocation failed name={}", reflection.name);
      return;
    }

    const auto [physical_width, physical_height] = GetReflectionPhysicalExtent(reflection);
    RegisterReflectionTargetCommand command{};
    command.family = reflection.family;
    command.role = reflection.role;
    command.wrapper = wrapper;
    command.surface = LoadU32(base, wrapper + kDeferredWrapperSurfaceOffset);
    command.texture = LoadU32(base, wrapper + kDeferredWrapperTextureOffset);
    command.logical_width = requested_width;
    command.logical_height = requested_height;
    command.physical_width = physical_width;
    command.physical_height = physical_height;
    command.sample_count_override = GetReflectionSampleCountOverride();
    const bool texture_required = reflection.role == ReflectionRole::kColor;
    if (!command.surface || (texture_required && !command.texture) ||
        !SubmitNativeCommand(command)) {
      REXLOG_ERROR(
          "gta4-native-reflection: registration failed name={} wrapper={:08X} surface={:08X} "
          "texture={:08X}",
          reflection.name, wrapper, command.surface, command.texture);
      return;
    }

    REXLOG_INFO(
        "gta4-native-reflection: registered name={} family={} role={} logical={}x{} "
        "physical={}x{} sample-override={} wrapper={:08X} surface={:08X} texture={:08X}",
        reflection.name, uint32_t(reflection.family), uint32_t(reflection.role), requested_width,
        requested_height, physical_width, physical_height, command.sample_count_override,
        wrapper, command.surface, command.texture);
    return;
  }

  if (!IsDeferredAaResourceName(base, name)) {
    __imp__sub_828DC7F0(ctx, base);
    const uint32_t wrapper = ctx.r3.u32;
    if (wrapper) {
      REXLOG_INFO(
          "gta4-native-architecture: point=title-target-created name={:08X} "
          "wrapper={:08X} surface={:08X} texture={:08X} requested={}x{} "
          "physical={}x{} logical={}x{}",
          name, wrapper, LoadU32(base, wrapper + kDeferredWrapperSurfaceOffset),
          LoadU32(base, wrapper + kDeferredWrapperTextureOffset), requested_width,
          requested_height, LoadU16(base, wrapper + kDeferredWrapperPhysicalWidthOffset),
          LoadU16(base, wrapper + kDeferredWrapperPhysicalHeightOffset),
          LoadU16(base, wrapper + kDeferredWrapperLogicalWidthOffset),
          LoadU16(base, wrapper + kDeferredWrapperLogicalHeightOffset));
    }
    return;
  }

  const uint32_t target_width =
      g_native_deferred_target_width ? g_native_deferred_target_width : requested_width;
  const uint32_t target_height = g_native_deferred_target_height
                                     ? g_native_deferred_target_height
                                     : LoadU32(base, kDeferredHeightGlobal);

  ctx.r6.u32 = kNativeBackingWidth;
  ctx.r7.u32 = kNativeBackingHeight;
  __imp__sub_828DC7F0(ctx, base);
  const uint32_t wrapper = ctx.r3.u32;

  static std::atomic<uint64_t> override_count{0};
  const uint64_t override_index = NextNativeHookDiagnosticCall(override_count);
  if (ShouldLogNativeHookCall(override_index)) {
    REXLOG_INFO(
        "gta4-native-notile: deferred AA allocation #{} name={:08X} requested={}x{} "
        "backing={}x{} target={}x{} wrapper={:08X} normalization=deferred",
        override_index, name, requested_width, requested_height, kNativeBackingWidth,
        kNativeBackingHeight, target_width, target_height, wrapper);
  }
  if (!wrapper) {
    REXLOG_ERROR(
        "gta4-native-notile: deferred AA backing construction failed name={:08X} "
        "wrapper={:08X} target={}x{}",
        name, wrapper, target_width, target_height);
  }
}

extern "C" void sub_828BE580(PPCContext& ctx, uint8_t* base) {
  if (IsNativeMode() && ctx.lr == kExteriorReflectionProjectionReturnAddress) {
    const double distance = GetExteriorReflectionCaptureDistance();
    ctx.f4.f64 = distance;
    static std::atomic<uint64_t> capture_count{0};
    const uint64_t count = NextNativeHookDiagnosticCall(capture_count);
    if (ShouldLogNativeHookCall(count)) {
      REXLOG_INFO(
          "gta4-native-reflection: exterior capture #{} fov={} near={} far={}", count,
          ctx.f1.f64, ctx.f3.f64, distance);
    }
  }
  __imp__sub_828BE580(ctx, base);
}

extern "C" void sub_828C8A50(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode() || ctx.lr != kEaaParameterUploadReturnAddress || ctx.r7.u32 != 16 ||
      ctx.r8.u32 != 1 || !ctx.r6.u32) {
    __imp__sub_828C8A50(ctx, base);
    return;
  }

  // sub_822CFC00 uploads EAA_PARAMS2 at this unique call site. Preserve the
  // shader parameter handle and replace only its value, so the remainder of
  // GTACompositePostFx (tone mapping, DOF, bloom and motion blur) remains
  // structurally intact. The native presentation shader performs AA instead.
  const uint32_t parameter_data = ctx.r6.u32;
  std::array<uint32_t, 4> saved{};
  for (size_t index = 0; index < saved.size(); ++index) {
    saved[index] = LoadU32(base, parameter_data + uint32_t(index * sizeof(uint32_t)));
    StoreU32(base, parameter_data + uint32_t(index * sizeof(uint32_t)), 0);
  }
  __imp__sub_828C8A50(ctx, base);
  for (size_t index = 0; index < saved.size(); ++index) {
    StoreU32(base, parameter_data + uint32_t(index * sizeof(uint32_t)), saved[index]);
  }

  static std::atomic<uint64_t> disabled_upload_count{0};
  const uint64_t count = NextNativeHookDiagnosticCall(disabled_upload_count);
  if (ShouldLogNativeHookCall(count)) {
    REXLOG_INFO("gta4-native-aa: neutralized EAA_PARAMS2 upload #{} handle={} source={:08X}",
                count, ctx.r5.u32, parameter_data);
  }
}

extern "C" void sub_822D1710(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_822D1710(ctx, base);
    return;
  }

  // GTACompositePostFx clears its private texture stages and executes the
  // complete stock tone-map/DOF/bloom/motion-blur draw inside this wrapper.
  // Resolve the active device through the same postfx+108 link consumed by
  // sub_822CFC00, and keep the typed phase balanced across the original call.
  const uint32_t postfx = ctx.r3.u32;
  const uint32_t postfx_device_link = postfx ? LoadU32(base, postfx + 108) : 0;
  const uint32_t device = postfx_device_link ? LoadU32(base, postfx_device_link + 24) : 0;
  const uint32_t caller = ctx.lr;

  SubmitEnvironmentalData(base, device, postfx);
  ScopedRenderPhaseMarker phase_scope(device, RenderPhase::kCompositePostFx, postfx, caller);

  __imp__sub_822D1710(ctx, base);
}

extern "C" void sub_821BD0A0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_821BD0A0(ctx, base);
    return;
  }

  const uint32_t radar_map_section = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const uint32_t device = LoadU32(base, kDeferredDeviceGlobal);
  ScopedRenderPhaseMarker phase_scope(device, RenderPhase::kRadarMap, radar_map_section,
                                      caller);
  __imp__sub_821BD0A0(ctx, base);
}

extern "C" void sub_8267D528(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_8267D528(ctx, base);
    return;
  }

  const uint32_t phase_object = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const uint32_t device = LoadU32(base, kDeferredDeviceGlobal);
  ScopedRenderPhaseMarker phase_scope(device, RenderPhase::kSceneToGBuffer, phase_object,
                                      caller);
  __imp__sub_8267D528(ctx, base);
}

extern "C" void sub_8267D750(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_8267D750(ctx, base);
    return;
  }

  const uint32_t phase_object = ctx.r3.u32;
  const uint32_t caller = ctx.lr;
  const uint32_t device = LoadU32(base, kDeferredDeviceGlobal);
  ScopedRenderPhaseMarker phase_scope(device, RenderPhase::kLightsToScreen, phase_object,
                                      caller);
  __imp__sub_8267D750(ctx, base);
}

extern "C" void sub_82A3E910(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode() || ctx.lr != kDeferredPhaseBeginReturnAddress) {
    __imp__sub_82A3E910(ctx, base);
    return;
  }

  const uint32_t rectangles = ctx.r6.u32;
  const uint32_t width = LoadU32(base, kDeferredWidthGlobal);
  const uint32_t height = LoadU32(base, kDeferredHeightGlobal);
  const bool expected = ctx.r5.u32 == kDeferredOriginalRectangleCount && rectangles && width &&
                        height && LoadU32(base, rectangles) == 0 &&
                        LoadU32(base, rectangles + 4) == 0 &&
                        LoadU32(base, rectangles + 8) == width;
  if (!expected) {
    static std::atomic<uint64_t> mismatch_count{0};
    const uint64_t mismatch = NextNativeHookDiagnosticCall(mismatch_count);
    if (ShouldLogNativeHookCall(mismatch)) {
      REXLOG_ERROR(
          "gta4-native-notile: deferred phase begin invariant mismatch lr={:08X} count={} "
          "rectangles={:08X} size={}x{}",
          ctx.lr, ctx.r5.u32, rectangles, width, height);
    }
    __imp__sub_82A3E910(ctx, base);
    return;
  }

  const std::array<uint32_t, 4> saved_rectangle = {
      LoadU32(base, rectangles), LoadU32(base, rectangles + 4),
      LoadU32(base, rectangles + 8), LoadU32(base, rectangles + 12)};
  StoreU32(base, rectangles, 0);
  StoreU32(base, rectangles + 4, 0);
  StoreU32(base, rectangles + 8, width);
  StoreU32(base, rectangles + 12, height);
  ctx.r5.u32 = kDeferredNativeRectangleCount;

  static std::atomic<uint64_t> begin_count{0};
  const uint64_t begin = NextNativeHookDiagnosticCall(begin_count);
  if (ShouldLogNativeHookCall(begin)) {
    REXLOG_INFO("gta4-native-notile: deferred phase begin #{} rectangles={}->{} size={}x{}",
                begin, kDeferredOriginalRectangleCount, kDeferredNativeRectangleCount, width,
                height);
  }

  __imp__sub_82A3E910(ctx, base);
  StoreU32(base, rectangles, saved_rectangle[0]);
  StoreU32(base, rectangles + 4, saved_rectangle[1]);
  StoreU32(base, rectangles + 8, saved_rectangle[2]);
  StoreU32(base, rectangles + 12, saved_rectangle[3]);
}

extern "C" void sub_824F6AF0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_824F6AF0(ctx, base);
    return;
  }

  const uint32_t device = LoadU32(base, kDeferredDeviceGlobal);
  const uint32_t width = LoadU32(base, kDeferredWidthGlobal);
  const uint32_t height = LoadU32(base, kDeferredHeightGlobal);
  std::array<uint32_t, 4> outputs{};
  for (size_t index = 0; index < outputs.size(); ++index) {
    outputs[index] =
        LoadU32(base, kDeferredOutputsGlobal + uint32_t(index * sizeof(uint32_t)));
  }
  const uint32_t alias = LoadU32(base, kDeferredAliasGlobal);
  const uint32_t primary_depth_surface = LoadU32(base, kPrimaryDepthSurfaceGlobal);
  const uint32_t deferred_depth_wrapper =
      LoadU32(base, kDeferredDepthAaWrapperGlobal);
  const uint32_t deferred_depth_surface =
      deferred_depth_wrapper
          ? LoadU32(base, deferred_depth_wrapper + kDeferredWrapperSurfaceOffset)
          : 0;

  bool resources_valid = device && width && height && alias &&
                         primary_depth_surface && deferred_depth_surface;
  for (uint32_t output : outputs) {
    resources_valid = resources_valid && output;
  }
  if (!resources_valid) {
    static std::atomic<uint64_t> fallback_count{0};
    const uint64_t fallback = NextNativeHookDiagnosticCall(fallback_count);
    if (ShouldLogNativeHookCall(fallback)) {
      REXLOG_ERROR(
          "gta4-native-notile: deferred phase end invariant mismatch device={:08X} size={}x{} "
          "outputs={:08X},{:08X},{:08X},{:08X} alias={:08X} "
          "deferred-depth={:08X} primary-depth={:08X}; "
          "using generated path",
          device, width, height, outputs[0], outputs[1], outputs[2], outputs[3], alias,
          deferred_depth_surface, primary_depth_surface);
    }
    __imp__sub_824F6AF0(ctx, base);
    return;
  }

  static std::atomic<uint64_t> object_snapshot_count{0};
  const uint64_t object_snapshot = NextNativeHookDiagnosticCall(object_snapshot_count);
  if (ShouldLogNativeHookCall(object_snapshot)) {
    std::array<uint32_t, 10> wrappers{};
    for (size_t index = 0; index < kDeferredFullSizeWrapperGlobals.size(); ++index) {
      wrappers[index] = LoadU32(base, kDeferredFullSizeWrapperGlobals[index]);
    }
    wrappers.back() = alias;
    constexpr std::array<const char*, 10> roles = {
        "gbuffer-0", "gbuffer-1", "gbuffer-2", "gbuffer-z",
        "gbuffer-0-aa", "gbuffer-1-aa", "gbuffer-2-aa", "gbuffer-z-aa",
        "depth-alias", "gbuffer-2-alias"};
    for (size_t index = 0; index < wrappers.size(); ++index) {
      const uint32_t wrapper = wrappers[index];
      REXLOG_INFO(
          "gta4-native-architecture: point=deferred-wrapper-pair snapshot={} "
          "role={} wrapper={:08X} surface={:08X} texture={:08X} "
          "physical={}x{} logical={}x{}",
          object_snapshot, roles[index], wrapper,
          LoadU32(base, wrapper + kDeferredWrapperSurfaceOffset),
          LoadU32(base, wrapper + kDeferredWrapperTextureOffset),
          LoadU16(base, wrapper + kDeferredWrapperPhysicalWidthOffset),
          LoadU16(base, wrapper + kDeferredWrapperPhysicalHeightOffset),
          LoadU16(base, wrapper + kDeferredWrapperLogicalWidthOffset),
          LoadU16(base, wrapper + kDeferredWrapperLogicalHeightOffset));
    }
  }

  InvokeGuest(ctx, base, sub_824F5230);
  const auto invoke_phase_marker = [&](uint32_t marker_global) {
    const uint32_t marker = LoadU32(base, marker_global);
    InvokeGuest(ctx, base, sub_828BFBE8, marker, kDeferredPhaseMarkerLimit - marker);
  };
  invoke_phase_marker(kDeferredPhaseMarkerEndGlobal);

  const ResolveRectangle full_rectangle{0, 0, int32_t(width), int32_t(height)};
  const ResolvePoint origin{0, 0};
  const uint32_t clear_color_bits[4]{};
  const auto get_surface = [&](uint32_t resource) {
    return InvokeGuest(ctx, base, sub_828D9608, resource).r3.u32;
  };

  const uint32_t depth_output = get_surface(outputs[3]);
  REXLOG_WARN(
      "gta4-native-cause: point=deferred-depth-lifecycle-submit "
      "attachment-wrapper={:08X} attachment-surface={:08X} "
      "resolved-wrapper={:08X} resolved-texture={:08X} "
      "forward-wrapper={:08X} forward-surface={:08X} caller={:08X}",
      deferred_depth_wrapper, deferred_depth_surface, outputs[3], depth_output,
      primary_depth_surface, primary_depth_surface, uint32_t(ctx.lr));
  SubmitNativeResolve(base, device, 20, &full_rectangle, depth_output, &origin, 0, 0,
                      clear_color_bits, 0.0, 0, 0, 3, uint32_t(ctx.lr), outputs[3],
                      deferred_depth_wrapper);
  const uint32_t color_output = get_surface(outputs[0]);
  SubmitNativeResolve(base, device, 16, &full_rectangle, color_output, &origin, 0, 0,
                      clear_color_bits, 0.0, 0, 0, 3, uint32_t(ctx.lr), outputs[0]);
  const uint32_t alias_output = get_surface(alias);
  SubmitNativeResolve(base, device, 768, &full_rectangle, alias_output, &origin, 0, 0,
                      clear_color_bits, 0.0, 0, 0, 3, uint32_t(ctx.lr), alias);
  InvokeGuest(ctx, base, sub_824F56F8, outputs[0], alias);
  const uint32_t second_output = get_surface(outputs[1]);
  SubmitNativeResolve(base, device, 273, &full_rectangle, second_output, &origin, 0, 0,
                      clear_color_bits, 0.0, 0, 0, 3, uint32_t(ctx.lr), outputs[1]);
  const uint32_t third_output = get_surface(outputs[2]);
  SubmitNativeResolve(base, device, 274, &full_rectangle, third_output, &origin, 0, 0,
                      clear_color_bits, 0.0, 0, 0, 3, uint32_t(ctx.lr), outputs[2]);

  DepthSurfaceHandoffCommand depth_handoff;
  depth_handoff.device = device;
  depth_handoff.source = CaptureSurfaceDescriptor(base, deferred_depth_surface);
  depth_handoff.destination = CaptureSurfaceDescriptor(base, primary_depth_surface);
  depth_handoff.source_wrapper = deferred_depth_wrapper;
  depth_handoff.destination_wrapper = primary_depth_surface;
  depth_handoff.source_texture = depth_output;
  depth_handoff.trace_caller = uint32_t(ctx.lr);
  if (!SubmitNativeCommand(depth_handoff)) {
    REXLOG_ERROR(
        "gta4-native-cause: point=explicit-depth-handoff-submit result=rejected "
        "source-wrapper={:08X} source={:08X} destination-wrapper={:08X} "
        "destination={:08X}",
        depth_handoff.source_wrapper, depth_handoff.source.handle,
        depth_handoff.destination_wrapper, depth_handoff.destination.handle);
  }

  invoke_phase_marker(kDeferredPhaseMarkerBeginGlobal);
  PPCContext finalizer = ctx;
  finalizer.r3.u32 = device;
  finalizer.r4.u32 = 0;
  finalizer.r5.u32 = 0;
  finalizer.r6.u32 = 0;
  finalizer.r7.u32 = 0;
  finalizer.r8.u32 = 0;
  finalizer.r9.u32 = 0;
  finalizer.r10.u32 = 0;
  finalizer.f1.f64 = 1.0;
  sub_82A3EDA8(finalizer, base);
  InvokeGuest(ctx, base, sub_824F4E88);
  const PPCContext tail =
      InvokeGuest(ctx, base, sub_822CF200, LoadU32(base, kDeferredPhaseTailGlobal));
  ctx.r3 = tail.r3;

  static std::atomic<uint64_t> phase_count{0};
  const uint64_t phase = NextNativeHookDiagnosticCall(phase_count);
  if (ShouldLogNativeHookCall(phase)) {
    REXLOG_INFO("gta4-native-notile: deferred phase end #{} size={}x{} resolves=5 predicates=0",
                phase, width, height);
  }
}

extern "C" void sub_82A3CC68(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3CC68(ctx, base);
    return;
  }

  static std::atomic<uint64_t> call_count{0};
  const uint64_t call = NextNativeHookDiagnosticCall(call_count);
  if (ShouldLogNativeHookCall(call)) {
    REXLOG_INFO(
        "gta4-native-hook: Resolve #{} lr={:08X} device={:08X} flags={:08X} "
        "source-rect={:08X} destination={:08X}",
        call, ctx.lr, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
  }

  ResolveRectangle source_rectangle{};
  ResolvePoint destination_point{};
  uint32_t clear_color_bits[4]{};
  const ResolveRectangle* source_rectangle_pointer = nullptr;
  const ResolvePoint* destination_point_pointer = nullptr;
  const uint32_t* clear_color_pointer = nullptr;
  if (ctx.r5.u32) {
    source_rectangle = LoadResolveRectangle(base, ctx.r5.u32);
    source_rectangle_pointer = &source_rectangle;
  }
  if (ctx.r7.u32) {
    destination_point = LoadResolvePoint(base, ctx.r7.u32);
    destination_point_pointer = &destination_point;
  }
  if (ctx.r10.u32) {
    for (uint32_t index = 0; index < 4; ++index) {
      clear_color_bits[index] = LoadU32(base, ctx.r10.u32 + index * sizeof(uint32_t));
    }
    clear_color_pointer = clear_color_bits;
  }

  const uint32_t known_frontbuffer =
      g_last_present_frontbuffer.load(std::memory_order_relaxed);
  if (rex::diagnostics::IsEnabled(rex::diagnostics::Category::kGuestHooks) &&
      known_frontbuffer && ctx.r6.u32 == known_frontbuffer) {
    std::array<uint32_t, kRenderTargetCount> render_targets{};
    for (uint32_t index = 0; index < kRenderTargetCount; ++index) {
      render_targets[index] =
          LoadU32(base, ctx.r3.u32 + (kRenderTargetBase + index) * sizeof(uint32_t));
    }
    std::fprintf(
        stderr,
        "[GuestResolveTrace] origin=direct caller=%08X device=%08X flags=%08X "
        "destination=%08X rt=%08X,%08X,%08X,%08X depth=%08X\n",
        uint32_t(ctx.lr), ctx.r3.u32, ctx.r4.u32, ctx.r6.u32, render_targets[0], render_targets[1],
        render_targets[2], render_targets[3], LoadU32(base, ctx.r3.u32 + kDepthStencilOffset));
    std::fflush(stderr);
  }

  SubmitNativeResolve(base, ctx.r3.u32, ctx.r4.u32, source_rectangle_pointer, ctx.r6.u32,
                      destination_point_pointer, ctx.r8.u32, ctx.r9.u32, clear_color_pointer,
                      ctx.f1.f64, LoadU32(base, ctx.r1.u32 + 92), LoadU32(base, ctx.r1.u32 + 100),
                      1, uint32_t(ctx.lr), 0);
}

extern "C" void sub_82A3EDA8(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3EDA8(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  const uint32_t base_flags = ctx.r4.u32;
  uint32_t resolve_records = ctx.r5.u32;
  const uint32_t destination_texture = ctx.r6.u32;
  const uint32_t final_clear_color = ctx.r7.u32;
  const double final_clear_depth = ctx.f1.f64;
  const uint32_t final_clear_stencil = ctx.r9.u32;
  const uint32_t final_parameters = ctx.r10.u32;
  const uint32_t resolve_count = device ? LoadU32(base, device + 12740) : 0;

  if (!device) {
    ctx.r3.u32 = 0;
    return;
  }

  static std::atomic<uint64_t> call_count{0};
  const uint64_t call = NextNativeHookDiagnosticCall(call_count);
  if (ShouldLogNativeHookCall(call)) {
    REXLOG_INFO(
        "gta4-native-hook: ResolveBatch #{} lr={:08X} device={:08X} flags={:08X} "
        "records={:08X} count={} destination={:08X}",
        call, ctx.lr, device, base_flags, resolve_records, resolve_count, destination_texture);
  }

  if (!resolve_records && destination_texture) {
    resolve_records = device + 12744;
  }

  const uint32_t source_surface = device ? LoadU32(base, device + 12716) : 0;
  const uint32_t known_frontbuffer =
      g_last_present_frontbuffer.load(std::memory_order_relaxed);
  if (rex::diagnostics::IsEnabled(rex::diagnostics::Category::kGuestHooks) &&
      known_frontbuffer && destination_texture == known_frontbuffer) {
    std::fprintf(stderr,
                 "[GuestResolveTrace] origin=batch caller=%08X device=%08X flags=%08X "
                 "destination=%08X source=%08X records=%08X count=%u\n",
                 uint32_t(ctx.lr), device, base_flags, destination_texture, source_surface,
                 resolve_records, resolve_count);
    std::fflush(stderr);
  }
  if (destination_texture && resolve_count && source_surface) {
    const uint32_t packed_dimensions = LoadU32(base, source_surface + 36);
    const uint32_t surface_width = (std::rotl(packed_dimensions, 14) & 0x3FFF) + 1;
    const uint32_t surface_height = (std::rotl(packed_dimensions, 29) & 0x7FFF) + 1;

    for (uint32_t index = 0; index < resolve_count; ++index) {
      const uint32_t record = resolve_records + index * 16;
      ResolveRectangle source_rectangle = LoadResolveRectangle(base, record);
      const ResolvePoint destination_point = LoadResolvePoint(base, record);
      if (uint32_t(source_rectangle.right) == surface_width) {
        source_rectangle.right = int32_t((uint32_t(source_rectangle.right) + 7) & ~uint32_t(7));
      }
      if (uint32_t(source_rectangle.bottom) == surface_height) {
        source_rectangle.bottom = int32_t((uint32_t(source_rectangle.bottom) + 7) & ~uint32_t(7));
      }

      uint32_t flags = base_flags;
      uint32_t clear_color_bits[4]{};
      const uint32_t* clear_color_pointer = nullptr;
      double clear_depth = final_clear_depth;
      uint32_t clear_stencil = final_clear_stencil;
      uint32_t parameters = final_parameters;
      if (index + 1 < resolve_count) {
        if (LoadU32(base, device + 12432)) {
          flags |= 0x100;
        }
        if (LoadU32(base, device + 12448)) {
          flags |= 0x200;
        }
        for (uint32_t color_index = 0; color_index < 4; ++color_index) {
          clear_color_bits[color_index] =
              LoadU32(base, device + 13184 + color_index * sizeof(uint32_t));
        }
        clear_color_pointer = clear_color_bits;
        clear_depth = double(std::bit_cast<float>(LoadU32(base, device + 13200)));
        clear_stencil = LoadU32(base, device + 13204);
        parameters = 0;
      } else if (final_clear_color) {
        for (uint32_t color_index = 0; color_index < 4; ++color_index) {
          clear_color_bits[color_index] =
              LoadU32(base, final_clear_color + color_index * sizeof(uint32_t));
        }
        clear_color_pointer = clear_color_bits;
      }

      SubmitNativeResolve(base, device, flags, &source_rectangle, destination_texture,
                          &destination_point, 0, 0, clear_color_pointer, clear_depth, clear_stencil,
                          parameters, 2, uint32_t(ctx.lr), 0);
    }
  } else if (destination_texture && resolve_count) {
    REXLOG_ERROR(
        "gta4-native-hook: ResolveBatch #{} missing source surface device={:08X} count={} "
        "destination={:08X}",
        call, device, resolve_count, destination_texture);
  }

  InvokeGuest(ctx, base, sub_82A3AA08, device, 2);
  FinalizeNativeResolveBatchState(base, device);
  InvokeGuest(ctx, base, sub_82A3BE18, device, 0x820B0314);
  ctx.r3.u32 = 0;
}

extern "C" void sub_82A3E348(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A3E348(ctx, base);
    return;
  }

  static std::atomic<uint64_t> call_count{0};
  const uint64_t call = NextNativeHookDiagnosticCall(call_count);
  if (ShouldLogNativeHookCall(call)) {
    REXLOG_INFO(
        "gta4-native-hook: DrawIndexedPrimitive #{} lr={:08X} device={:08X} primitive={} "
        "base={} start={} indices={}",
        call, ctx.lr, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, ctx.r6.u32, ctx.r7.u32);
  }

  const uint32_t device = ctx.r3.u32;
  DrawIndexedPrimitiveCommand command;
  command.device = device;
  command.primitive_type = ctx.r4.u32;
  command.base_vertex = ctx.r5.s32;
  command.start_index = ctx.r6.u32;
  command.index_count = ctx.r7.u32;
  command.primitive_restart_enabled =
      (LoadU32(base, device + 10568) & uint32_t(1u << 21)) != 0;
  command.primitive_restart_index = LoadU32(base, device + 10444) & 0x00FFFFFF;
  command.caller = static_cast<uint32_t>(ctx.lr);
  command.origin_flags = kDrawCommandOriginDirect;
  command.command_list = 0;
  command.draw_id = call;

  const bool trace_gbuffer_submission =
      rex::diagnostics::IsEnabled(
          rex::diagnostics::Category::kNativeTrace) &&
      (command.index_count == 5961 || command.index_count == 1056);
  uint64_t constants_hash_before = 0;
  uint64_t transform_hash_before = 0;
  std::array<uint64_t, 5> dirty_before{};
  std::array<uint32_t, 16> transform_before{};
  NativeShaderBindingDiagnosticState shader_bindings{};
  if (trace_gbuffer_submission) {
    constants_hash_before = XXH3_64bits(
        GuestPointer(base, device + kReplayShaderConstantsOffset),
        kReplayShaderConstantsSize);
    transform_hash_before = XXH3_64bits(
        GuestPointer(base, device + kReplayVertexTransformConstantsOffset),
        kReplayVertexTransformConstantsSize);
    for (uint32_t index = 0; index < dirty_before.size(); ++index) {
      dirty_before[index] = LoadU64(base, device + index * sizeof(uint64_t));
    }
    for (uint32_t index = 0; index < transform_before.size(); ++index) {
      transform_before[index] = LoadU32(
          base, device + kReplayVertexTransformConstantsOffset +
                    index * sizeof(uint32_t));
    }
    shader_bindings = CaptureNativeShaderBindings(device);
  }
  ConsumeNativeDrawDirtyState(base, device);

  if (trace_gbuffer_submission) {
    const uint64_t constants_hash_after = XXH3_64bits(
        GuestPointer(base, device + kReplayShaderConstantsOffset),
        kReplayShaderConstantsSize);
    const uint64_t transform_hash_after = XXH3_64bits(
        GuestPointer(base, device + kReplayVertexTransformConstantsOffset),
        kReplayVertexTransformConstantsSize);
    std::array<uint64_t, 5> dirty_after{};
    for (uint32_t index = 0; index < dirty_after.size(); ++index) {
      dirty_after[index] =
          LoadU64(base, device + index * sizeof(uint64_t));
    }
    const uint32_t submitted_frame =
        LoadU32(base, device + kSubmittedFrameOffset);
    REXLOG_INFO(
        "gta4-native-cause: point=gbuffer-title-submit frame={} draw={} "
        "caller={:08X} device={:08X} primitive={} base={} start={} indices={} "
        "vs-handle={:08X} ps-handle={:08X} vdecl={:08X} ib={:08X} "
        "constants={:016X}->{:016X}:{} transform={:016X}->{:016X}:{} "
        "dirty={:016X},{:016X},{:016X},{:016X},{:016X}->"
        "{:016X},{:016X},{:016X},{:016X},{:016X} "
        "c8={:08X},{:08X},{:08X},{:08X} "
        "c9={:08X},{:08X},{:08X},{:08X} "
        "c10={:08X},{:08X},{:08X},{:08X} "
        "c11={:08X},{:08X},{:08X},{:08X}",
        submitted_frame, call, command.caller, device,
        command.primitive_type, command.base_vertex, command.start_index,
        command.index_count, shader_bindings.vertex_shader,
        shader_bindings.pixel_shader,
        LoadU32(base, device + kVertexDeclarationOffset),
        LoadU32(base, device + kIndexBufferOffset), constants_hash_before,
        constants_hash_after, constants_hash_before == constants_hash_after,
        transform_hash_before, transform_hash_after,
        transform_hash_before == transform_hash_after, dirty_before[0],
        dirty_before[1], dirty_before[2], dirty_before[3], dirty_before[4],
        dirty_after[0], dirty_after[1], dirty_after[2], dirty_after[3],
        dirty_after[4], transform_before[0], transform_before[1],
        transform_before[2], transform_before[3], transform_before[4],
        transform_before[5], transform_before[6], transform_before[7],
        transform_before[8], transform_before[9], transform_before[10],
        transform_before[11], transform_before[12], transform_before[13],
        transform_before[14], transform_before[15]);
  }
  SubmitNativeCommand(command);
}

extern "C" void sub_82A457B0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A457B0(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  uint32_t flags = ctx.r4.u32;
  const uint32_t rectangle = ctx.r5.u32;
  const uint32_t color = ctx.r6.u32;
  if (!device || !rectangle) {
    return;
  }

  if (!LoadU32(base, device + 12448)) {
    flags &= ~uint32_t(0x30);
  }
  if (!flags) {
    return;
  }

  const int32_t viewport_x =
      TruncateViewportCoordinate(std::bit_cast<float>(LoadU32(base, device + 12640)));
  const int32_t viewport_y =
      TruncateViewportCoordinate(std::bit_cast<float>(LoadU32(base, device + 12644)));
  const int32_t viewport_width =
      TruncateViewportCoordinate(std::bit_cast<float>(LoadU32(base, device + 12648)));
  const int32_t viewport_height =
      TruncateViewportCoordinate(std::bit_cast<float>(LoadU32(base, device + 12652)));

  int32_t left = std::max(int32_t(LoadU32(base, rectangle)), viewport_x);
  int32_t top = std::max(int32_t(LoadU32(base, rectangle + 4)), viewport_y);
  int32_t right = std::min(int32_t(LoadU32(base, rectangle + 8)),
                           std::bit_cast<int32_t>(uint32_t(viewport_x) + uint32_t(viewport_width)));
  int32_t bottom =
      std::min(int32_t(LoadU32(base, rectangle + 12)),
               std::bit_cast<int32_t>(uint32_t(viewport_y) + uint32_t(viewport_height)));
  if (int32_t(LoadU32(base, device + 11848))) {
    left = std::max(left, int32_t(LoadU32(base, device + 12668)));
    top = std::max(top, int32_t(LoadU32(base, device + 12672)));
    right = std::min(right, int32_t(LoadU32(base, device + 12676)));
    bottom = std::min(bottom, int32_t(LoadU32(base, device + 12680)));
  }
  if (right <= left || bottom <= top) {
    return;
  }

  ClearCommand command;
  command.device = device;
  command.flags = flags;
  command.left = left;
  command.top = top;
  command.right = right;
  command.bottom = bottom;
  if (color) {
    for (uint32_t index = 0; index < 4; ++index) {
      command.color_bits[index] = LoadU32(base, color + index * sizeof(uint32_t));
    }
  }
  command.depth_bits = std::bit_cast<uint64_t>(ctx.f1.f64);
  command.stencil = ctx.r8.u32;

  ConsumeNativeDrawDirtyState(base, device);
  SubmitNativeCommand(command);
  StoreU8(base, device + 10941, LoadU8(base, device + 10941) | uint8_t(4));
  StoreU64(base, device + 16, LoadU64(base, device + 16) | uint64_t(0x00020000));
}

extern "C" void sub_82A46DA0(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A46DA0(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  uint32_t behavior = LoadU32(base, device + 22280);
  if (ctx.r4.u32) {
    behavior |= 4;
  } else {
    behavior &= ~uint32_t(4);
  }
  StoreU32(base, device + 22280, behavior);
}

extern "C" void sub_82A467D8(PPCContext& ctx, uint8_t* base) {
  if (!IsNativeMode()) {
    __imp__sub_82A467D8(ctx, base);
    return;
  }

  const uint32_t device = ctx.r3.u32;
  const uint32_t submitted_frame = LoadU32(base, device + kSubmittedFrameOffset) + 1;
  StoreU32(base, device + kSubmittedFrameOffset, submitted_frame);
  StoreU8(base, device + 10941, LoadU8(base, device + 10941) & uint8_t(0xEF));
  StoreAllDirty(base, device);

  PresentCommand command{};
  command.device = device;
  command.frontbuffer_texture = ctx.r4.u32;
  if (command.frontbuffer_texture) {
    for (uint32_t index = 0; index < std::size(command.frontbuffer_fetch); ++index) {
      command.frontbuffer_fetch[index] =
          LoadU32(base, command.frontbuffer_texture + 28 + index * sizeof(uint32_t));
    }
  }
  command.width = g_native_render_width;
  command.height = g_native_render_height;
  command.display_width = g_native_display_width;
  command.display_height = g_native_display_height;
  command.submitted_frame = submitted_frame;
  g_last_present_frontbuffer.store(command.frontbuffer_texture, std::memory_order_relaxed);
  SubmitNativeCommand(command);
  ctx.r3.u32 = device;
}
